// ***************************************************************
//  MiniDumper.cpp   version:  1.0   date:2006/01/26
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2006 AJJIYA - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "StdAfx.h"

#include <Windows.h>
#include <tchar.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <DbgHelp.h>
#include "SPStream.h"

#include "LataleVersionDef.h"

#include "SPCommon.h"

#include "MiniDumper.h"

// based on dbghelp.h
typedef BOOL (WINAPI *MINIDUMPWRITEDUMP)(
	HANDLE hProcess, DWORD dwPid, HANDLE hFile, MINIDUMP_TYPE DumpType,
	CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
	CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
	CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam
	);

MiniDumper::DumpLevel MiniDumper::s_DumpLevel                = MiniDumper::DUMP_LEVEL_0;
bool                  MiniDumper::s_bAddTimeStamp            = true;
TCHAR                 MiniDumper::s_szAppName[_MAX_PATH]     = {0,};
TCHAR                 MiniDumper::s_szFaultReason[2048]      = {0,};

#define chDIMOF(Array) (sizeof(Array) / sizeof(Array[0]))

//////////////////////////////////////////////////////////////////////////////
/// \brief 생성자
/// \param DL 덤프 레벨
/// \param bAddTimeStamp 덤프 파일 이름에다가 덤프 파일이 생성된 날짜를
/// 집어넣는가의 여부.
//////////////////////////////////////////////////////////////////////////////
MiniDumper::MiniDumper(DumpLevel DL, bool bAddTimeStamp)
{
	assert(s_szAppName[0] == 0);
	assert(DL >= 0);
	assert(DL <= DUMP_LEVEL_3);

	s_DumpLevel     = DL;
	s_bAddTimeStamp = bAddTimeStamp;

	// 모듈 경로를 알아낸다.
	TCHAR szFilename[_MAX_PATH];
	::GetModuleFileName(NULL, szFilename, _MAX_PATH);

	// 확장자를 제거한 모듈 경로를 준비해두고...
	TCHAR* dot = strrchr(szFilename, '.');
	::lstrcpyn(s_szAppName, szFilename, (int)(dot - szFilename + 1));

	// 예외 처리 핸들러를 설정한다.
	::SetUnhandledExceptionFilter(TopLevelFilter);
}

//////////////////////////////////////////////////////////////////////////////
/// \brief 소멸자
//////////////////////////////////////////////////////////////////////////////
MiniDumper::~MiniDumper()
{
}

//////////////////////////////////////////////////////////////////////////////
/// \brief 예외에 대한 정보를 받아서, 미니 덤프 파일을 생성한다. 
/// 
/// SetUnhandledExceptionFilter() API에 의해서 설정되고, 프로세스 내부에서 
/// Unhandled Exception이 발생될 경우, 호출되게 된다. 단 디버거가 붙어있는 
/// 경우, Unhandled Exception Filter는 호출되지 않는다. 이 말은 이 함수 
/// 내부를 디버깅할 수는 없다는 말이다. 이 함수 내부를 디버깅하기 위해서는 
/// 메시지 박스 또는 파일을 이용해야한다.
/// 
/// \param pExceptionInfo 예외 정보
/// \return LONG 이 함수를 실행하고 난 다음, 취할 행동값. 자세한 것은 SEH
/// 문서를 참고하도록.
//////////////////////////////////////////////////////////////////////////////
LONG WINAPI MiniDumper::TopLevelFilter(struct _EXCEPTION_POINTERS* pExPtr)
{
	LONG    retval                   = EXCEPTION_CONTINUE_SEARCH;
	HMODULE hDLL                     = NULL;
	TCHAR   szDbgHelpPath[_MAX_PATH] = {0,};
	TCHAR   szDumpPath[MAX_PATH * 2] = {0,};
	TCHAR   szFilePath[MAX_PATH * 2] = {0,};
	TCHAR	szBuf[_MAX_PATH * 5]	 = {0,};

	if( g_eCountryLocale == CL_JAPAN )
		return retval;

	// 먼저 실행 파일이 있는 디렉토리에서 DBGHELP.DLL을 로드해 본다.
	// Windows 2000 의 System32 디렉토리에 있는 DBGHELP.DLL 파일은 버전이 
	// 오래된 것일 수 있기 때문이다. (최소 5.1.2600.0 이상이어야 한다.)
	if (::GetModuleFileName(NULL, szDbgHelpPath, _MAX_PATH))
	{
		LPTSTR pSlash = ::strrchr(szDbgHelpPath, '\\');
		if (pSlash)
		{
			::lstrcpy(pSlash + 1, "DBGHELP.DLL");
			hDLL = ::LoadLibrary(szDbgHelpPath);
		}
	}

	// 현재 디렉토리에 없다면, 아무 버전이나 로드한다.
	if (hDLL == NULL) hDLL = ::LoadLibrary("DBGHELP.DLL");

	// 현재 시간을 얻어온다.
	SYSTEMTIME t;
	::GetLocalTime(&t);

	// 시간 문자열을 준비한다.
	TCHAR szTail[_MAX_PATH] = {0,};
	TCHAR szErrorUnique[_MAX_PATH] = {0,};
	_snprintf(szTail, chDIMOF(szTail)-1, " %04d-%02d-%02d %02d-%02d-%02d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
	_snprintf(szErrorUnique, chDIMOF(szErrorUnique)-1, "LaTaleClient/%d.%d/%04d-%02d-%02d/%02d-%02d-%02d", VER_MAJOR , VER_MINOR , t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond );

	//	시간 체크
	if (s_bAddTimeStamp)
	{
		// 덤프 파일 이름 += 시간 문자열
		::lstrcat(szDumpPath, s_szAppName);
		::lstrcat(szDumpPath, szTail);
	}

	::lstrcpy( szFilePath , szDumpPath );
	::lstrcat( szFilePath , ".ERR");

	SPFileStream*	pLogFile	=	new	SPFileStream( szFilePath , SPFileStream::OPEN_CREATE | SPFileStream::OPEN_WRITE );

	if( g_eCountryLocale == CL_JAPAN )
	{
#include "MiniDumper-Japan.cpp"
	}
	else if( g_eCountryLocale == CL_CHINA )
	{
		if (hDLL == NULL)
			return retval;

		MINIDUMPWRITEDUMP pfnMiniDumpWriteDump = (MINIDUMPWRITEDUMP)::GetProcAddress(hDLL, "MiniDumpWriteDump");

		if (pfnMiniDumpWriteDump == NULL)
			return retval;

		::lstrcat(szDumpPath, ".DMP");

		HANDLE hFile = ::CreateFile( szDumpPath, GENERIC_WRITE , FILE_SHARE_WRITE , NULL , CREATE_ALWAYS , FILE_ATTRIBUTE_NORMAL , NULL );

		if (hFile == INVALID_HANDLE_VALUE)
			return retval;

		MINIDUMP_EXCEPTION_INFORMATION ExceptionParam;

		ExceptionParam.ThreadId = ::GetCurrentThreadId();
		ExceptionParam.ExceptionPointers = pExPtr;
		ExceptionParam.ClientPointers = FALSE;

		BOOL bResult = FALSE;

		switch (s_DumpLevel)
		{
		case DUMP_LEVEL_0: // MiniDumpNormal
			bResult = pfnMiniDumpWriteDump(
				::GetCurrentProcess(), ::GetCurrentProcessId(), 
				hFile, MiniDumpNormal, &ExceptionParam, NULL, NULL);
			break;
		case DUMP_LEVEL_1: // MiniDumpWithDataSegs 
			bResult = pfnMiniDumpWriteDump(
				::GetCurrentProcess(), ::GetCurrentProcessId(), 
				hFile, MiniDumpWithDataSegs, &ExceptionParam, NULL, NULL);
			break;
		case DUMP_LEVEL_2: // MiniDumpWithFullMemory 
			bResult = pfnMiniDumpWriteDump(
				::GetCurrentProcess(), ::GetCurrentProcessId(), 
				hFile, (MINIDUMP_TYPE)( MiniDumpNormal | MiniDumpWithIndirectlyReferencedMemory ) , &ExceptionParam, NULL, NULL);
			break;
		case DUMP_LEVEL_3: // MiniDumpWithFullMemory 
			bResult = pfnMiniDumpWriteDump(
				::GetCurrentProcess(), ::GetCurrentProcessId(), 
				hFile, MiniDumpWithFullMemory, &ExceptionParam, NULL, NULL);
			break;
		default:
			bResult = pfnMiniDumpWriteDump(
				::GetCurrentProcess(), ::GetCurrentProcessId(), 
				hFile, MiniDumpNormal, &ExceptionParam, NULL, NULL);
			break;
		}

		_snprintf( szBuf , chDIMOF( szBuf ) - 1 , "LaTaleClient Version : %d.%d\r\n\r\nLaTaleClient Error : [%s]\r\n\r\n" , VER_MAJOR , VER_MINOR , GetFaultReason(pExPtr) );

		pLogFile->Write( szBuf , (unsigned int)strlen( szBuf ) );
		pLogFile->Release();

		if( bResult == FALSE )
		{
			bResult = pfnMiniDumpWriteDump( ::GetCurrentProcess(), ::GetCurrentProcessId(), hFile, MiniDumpNormal, &ExceptionParam, NULL, NULL);
		}
	}
	else
	{
		// DBGHELP.DLL을 찾을 수 없다면 더 이상 진행할 수 없다.
		if (hDLL == NULL)
		{
			strcpy( szBuf , "LaTaleClient가 비정상적으로 종료 되었습니다. DBGHELP.DLL 파일이 없어서 정보를 저장할수 없습니다." );

			pLogFile->Write( szBuf , (unsigned int)strlen( szBuf ) );
			pLogFile->Release();

			::MessageBox( NULL , szBuf , "LaTaleClient" , MB_ICONWARNING );

			return retval;
		}

		// DLL 내부에서 MiniDumpWriteDump API를 찾는다.
		MINIDUMPWRITEDUMP pfnMiniDumpWriteDump = (MINIDUMPWRITEDUMP)::GetProcAddress(hDLL, "MiniDumpWriteDump");

		// 미니덤프 함수를 찾을 수 없다면 더 이상 진행할 수 없다.
		if (pfnMiniDumpWriteDump == NULL)
		{
			strcpy( szBuf , "LaTaleClient가 비정상적으로 종료 되었습니다. DBGHELP.DLL의 버전이 낮아서 정보를 저장할수 없습니다." );

			pLogFile->Write( szBuf , (unsigned int)strlen( szBuf ) );
			pLogFile->Release();

			::MessageBox( NULL , szBuf , "LaTaleClient" , MB_ICONWARNING );

			return retval;
		}

		// 덤프 파일 이름 += 확장자
		::lstrcat(szDumpPath, ".DMP");

		// 파일을 생성한다.
		HANDLE hFile = ::CreateFile(
			szDumpPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, 
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		// 파일을 생성할 수 없다면 더 이상 진행할 수 없다.
		if (hFile == INVALID_HANDLE_VALUE)
		{
	//		sprintf( szBuf , "Failed to create dump file '%s'", szDumpPath );
			sprintf( szBuf , "LaTaleClient가 비정상적으로 종료 되었습니다. %s 파일을 생성할 수 없습니다." , szDumpPath );

			pLogFile->Write( szBuf , (unsigned int)strlen( szBuf ) );
			pLogFile->Release();

			::MessageBox( NULL , szBuf , "LaTaleClient" , MB_ICONWARNING );

			return retval;
		}

		MINIDUMP_EXCEPTION_INFORMATION ExceptionParam;

		ExceptionParam.ThreadId = ::GetCurrentThreadId();
		ExceptionParam.ExceptionPointers = pExPtr;
		ExceptionParam.ClientPointers = FALSE;

		// 옵션에 따라 덤프 파일을 생성한다. 
		BOOL bResult = FALSE;
		switch (s_DumpLevel)
		{
		case DUMP_LEVEL_0: // MiniDumpNormal
			bResult = pfnMiniDumpWriteDump(
				::GetCurrentProcess(), ::GetCurrentProcessId(), 
				hFile, MiniDumpNormal, &ExceptionParam, NULL, NULL);
			break;
		case DUMP_LEVEL_1: // MiniDumpWithDataSegs 
			bResult = pfnMiniDumpWriteDump(
				::GetCurrentProcess(), ::GetCurrentProcessId(), 
				hFile, MiniDumpWithDataSegs, &ExceptionParam, NULL, NULL);
			break;
		case DUMP_LEVEL_2: // MiniDumpWithFullMemory 
			bResult = pfnMiniDumpWriteDump(
				::GetCurrentProcess(), ::GetCurrentProcessId(), 
				hFile, (MINIDUMP_TYPE)( MiniDumpNormal | MiniDumpWithIndirectlyReferencedMemory ) , &ExceptionParam, NULL, NULL);
			break;
		case DUMP_LEVEL_3: // MiniDumpWithFullMemory 
			bResult = pfnMiniDumpWriteDump(
				::GetCurrentProcess(), ::GetCurrentProcessId(), 
				hFile, MiniDumpWithFullMemory, &ExceptionParam, NULL, NULL);
			break;
		default:
			bResult = pfnMiniDumpWriteDump(
				::GetCurrentProcess(), ::GetCurrentProcessId(), 
				hFile, MiniDumpNormal, &ExceptionParam, NULL, NULL);
			break;
		}

		if( bResult == FALSE )
		{
			bResult = pfnMiniDumpWriteDump( ::GetCurrentProcess(), ::GetCurrentProcessId(), hFile, MiniDumpNormal, &ExceptionParam, NULL, NULL);
		}

		UINT	uiType	=	MB_ICONWARNING;
		BOOL	bIsLaTaleErrorExits	=	FALSE;		// LaTaleError.EXE 가 있는지 확인한다.
		TCHAR   szLaTaleErrorFullPath[_MAX_PATH] = {0,};
		TCHAR   szLaTaleErrorPath[_MAX_PATH] = {0,};
		TCHAR   szLaTaleErrorName[_MAX_PATH] = {0,};
		TCHAR   szLaTaleErrorCmdLine[_MAX_PATH] = {0,};

		// 덤프 파일 생성 결과를 로그 파일에다 기록한다.
		if (bResult)
		{
	//		TCHAR szMessage[8192] = {0,};
	//		lstrcat(szMessage, "Saved dump file to '");
	//		lstrcat(szMessage, szDumpPath);
	//		lstrcat(szMessage, "'.\nFault Reason : ");
	//		lstrcat(szMessage, GetFaultReason(pExPtr));

			retval = EXCEPTION_EXECUTE_HANDLER;

			// LaTaleError.EXE 가 있는지 확인한다.
			if (::GetModuleFileName(NULL, szLaTaleErrorFullPath, _MAX_PATH))
			{
				::lstrcpy( szLaTaleErrorName , "LaTaleError.EXE" );

				TCHAR* pszSlash = ::strrchr( szLaTaleErrorFullPath , '\\' );

				if (pszSlash)
				{
					::lstrcpyn( szLaTaleErrorPath , szLaTaleErrorFullPath , (int)( pszSlash - szLaTaleErrorFullPath + 1 ) );
					::lstrcpy( pszSlash + 1, szLaTaleErrorName );

					if( ( _taccess( szLaTaleErrorFullPath , 0 ) ) != -1 )
						bIsLaTaleErrorExits	=	TRUE;
				}
			}

			_snprintf( szBuf , chDIMOF( szBuf ) - 1 , "LaTaleClient Version : %d.%d\r\n\r\nLaTaleClient가 Error : [%s] 문제로 비정상적으로 종료 되었습니다.\r\n\r\nhttp://www.LaTale.com 홈페이지의 메일문의 메뉴로\r\n\r\n %s \r\n %s \r\n\r\n" , VER_MAJOR , VER_MINOR , GetFaultReason(pExPtr) , szDumpPath , szFilePath );

			if( bIsLaTaleErrorExits == TRUE )
			{
				::lstrcat( szBuf , "위 두개의 정보 파일과 추가적으로 고객님 PC의 Direct X의 정보를,\r\n\r\n" );
				::lstrcat( szBuf , "지금 LaTaleError를 사용하여 정보 파일을 첨부해서 보내시겠습니까 ? \r\n" );
				uiType	|=	MB_YESNO;

				TCHAR* pszFileSlash = ::strrchr( szFilePath , '\\' );
				TCHAR* pszDumpSlash = ::strrchr( szDumpPath , '\\' );

				if( pszFileSlash )
				{
					::lstrcpy( szFilePath , pszFileSlash + 1 );
				}

				if( pszDumpSlash )
				{
					::lstrcpy( szDumpPath , pszDumpSlash + 1 );
				}

				::lstrcat( szLaTaleErrorCmdLine , UNIQUE_LATALE_ERROR );
				::lstrcat( szLaTaleErrorCmdLine , szErrorUnique );
				::lstrcat( szLaTaleErrorCmdLine , " " );
				::lstrcat( szLaTaleErrorCmdLine , CMD_INFO );
				::lstrcat( szLaTaleErrorCmdLine , szFilePath );
				::lstrcat( szLaTaleErrorCmdLine , " " );
				::lstrcat( szLaTaleErrorCmdLine , CMD_DMP );
				::lstrcat( szLaTaleErrorCmdLine , szDumpPath );
				::lstrcat( szLaTaleErrorCmdLine , CMD_END );
			}
			else
			{
				::lstrcat( szBuf , "위 두개의 정보 파일과 추가적으로 고객님 PC의 Direct X의 정보가 필요하오니,\r\n" );
				::lstrcat( szBuf , "시작 > 실행  에서 dxdiag 를 입력하시고 확인 버튼을 누르시면,\r\n" );
				::lstrcat( szBuf , "Direct X 진단도구가 실행되오니 모든 정보 저장 를 클릭하시어 저장을 하신 후\r\n\r\n" );
				::lstrcat( szBuf , "( 시작 -> 실행 -> dxdiag 입력 -> 모든 정보 저장  )\r\n\r\n" );
				::lstrcat( szBuf , "해당 파일을 첨부하여 메일제목을 [DMP오류 관련 dxdiag파일 첨부]로 작성한 후\r\n" );
				::lstrcat( szBuf , "발송하여 주시기를 부탁드립니다.\r\n\r\n" );
				::lstrcat( szBuf , "이메일에 첨부파일 용량은 2메가가 제한이오니 용량확인을 잘해주시고\r\n" );
				::lstrcat( szBuf , "옆에 첨부 버튼을 꼭 눌러 주시어 하단의 첨부 파일 목록에\r\n" );
				::lstrcat( szBuf , "파일이 첨부가 되었는지를 확인을 해주십시요." );
			}
		}
		else
		{
	//		sprintf( szBuf , "Failed to save dump file to '%s' (error %d)", szDumpPath, ::GetLastError() );
			sprintf( szBuf , "LaTaleClient가 비정상적으로 종료 되었습니다. %s 정보 파일을 저장할수 없습니다.\n Error Code : [ %d ]" , szDumpPath , ::GetLastError() );

			//filelog(NULL, "Failed to save dump file to '%s' (error %d,%s)", 
			//	szDumpPath, ::GetLastError(), GetLastErrorString().c_str());

			assert(false);
		}

		pLogFile->Write( szBuf , (unsigned int)strlen( szBuf ) );
		pLogFile->Release();

		::CloseHandle(hFile);
		int	iResult = ::MessageBox( NULL , szBuf , "LaTaleClient" , uiType );

		//	::MessageBox( NULL , szLaTaleErrorCmdLine , "LaTaleClient" , MB_ICONWARNING );

		if( bIsLaTaleErrorExits == TRUE && iResult == IDYES )
		{
			SHELLEXECUTEINFO	si;

			memset( &si, 0, sizeof(SHELLEXECUTEINFO) );

			si.cbSize		= sizeof(si);
			si.lpVerb		= _T("Open");
			si.nShow		|= SW_SHOWNA;
			si.lpDirectory	= (LPCSTR)szLaTaleErrorPath;
			si.lpFile		= (LPCSTR)szLaTaleErrorName;
			si.lpParameters	= (LPCSTR)szLaTaleErrorCmdLine;

			if( ShellExecuteEx( &si ) == FALSE )
			{   
				int res = GetLastError();

				if( res == ERROR_FILE_NOT_FOUND )
					::MessageBox( NULL , "프로그램을 찾을 수 없습니다." , "LaTaleClient" , MB_ICONWARNING );
				if( res == ERROR_PATH_NOT_FOUND )
					::MessageBox( NULL , "프로그램 경로가 올바르지 않습니다." , "LaTaleClient" , MB_ICONWARNING );
				if( res == ERROR_ACCESS_DENIED )
					::MessageBox( NULL , "프로그램 액세스가 거부되었습니다." , "LaTaleClient" , MB_ICONWARNING );
			}
		}
	}

	return retval;
}

//////////////////////////////////////////////////////////////////////////////
/// \brief 
/// 
/// \param pExPtrs 
/// \return LPCTSTR
//////////////////////////////////////////////////////////////////////////////
LPCTSTR MiniDumper::GetFaultReason(struct _EXCEPTION_POINTERS* pExPtrs)
{
	if (::IsBadReadPtr(pExPtrs, sizeof(EXCEPTION_POINTERS))) 
		return "BAD EXCEPTION POINTERS";

	// 간단한 에러 코드라면 그냥 변환할 수 있다.
	switch (pExPtrs->ExceptionRecord->ExceptionCode)
	{
	case EXCEPTION_ACCESS_VIOLATION:         return "EXCEPTION_ACCESS_VIOLATION";
	case EXCEPTION_DATATYPE_MISALIGNMENT:    return "EXCEPTION_DATATYPE_MISALIGNMENT";
	case EXCEPTION_BREAKPOINT:               return "EXCEPTION_BREAKPOINT";
	case EXCEPTION_SINGLE_STEP:              return "EXCEPTION_SINGLE_STEP";
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:    return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
	case EXCEPTION_FLT_DENORMAL_OPERAND:     return "EXCEPTION_FLT_DENORMAL_OPERAND";
	case EXCEPTION_FLT_DIVIDE_BY_ZERO:       return "EXCEPTION_FLT_DIVIDE_BY_ZERO";
	case EXCEPTION_FLT_INEXACT_RESULT:       return "EXCEPTION_FLT_INEXACT_RESULT";
	case EXCEPTION_FLT_INVALID_OPERATION:    return "EXCEPTION_FLT_INVALID_OPERATION";
	case EXCEPTION_FLT_OVERFLOW:             return "EXCEPTION_FLT_OVERFLOW";
	case EXCEPTION_FLT_STACK_CHECK:          return "EXCEPTION_FLT_STACK_CHECK";
	case EXCEPTION_FLT_UNDERFLOW:            return "EXCEPTION_FLT_UNDERFLOW";
	case EXCEPTION_INT_DIVIDE_BY_ZERO:       return "EXCEPTION_INT_DIVIDE_BY_ZERO";
	case EXCEPTION_INT_OVERFLOW:             return "EXCEPTION_INT_OVERFLOW";
	case EXCEPTION_PRIV_INSTRUCTION:         return "EXCEPTION_PRIV_INSTRUCTION";
	case EXCEPTION_IN_PAGE_ERROR:            return "EXCEPTION_IN_PAGE_ERROR";
	case EXCEPTION_ILLEGAL_INSTRUCTION:      return "EXCEPTION_ILLEGAL_INSTRUCTION";
	case EXCEPTION_NONCONTINUABLE_EXCEPTION: return "EXCEPTION_NONCONTINUABLE_EXCEPTION";
	case EXCEPTION_STACK_OVERFLOW:           return "EXCEPTION_STACK_OVERFLOW";
	case EXCEPTION_INVALID_DISPOSITION:      return "EXCEPTION_INVALID_DISPOSITION";
	case EXCEPTION_GUARD_PAGE:               return "EXCEPTION_GUARD_PAGE";
	case EXCEPTION_INVALID_HANDLE:           return "EXCEPTION_INVALID_HANDLE";
	case 0xE06D7363:                         return "Microsoft C++ Exception";
	default:
		break;
	}

	// 뭔가 좀 더 복잡한 에러라면...
	lstrcpy(s_szFaultReason, "Unknown"); 
	::FormatMessage(
		FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS,
		::GetModuleHandle("NTDLL.DLL"),
		pExPtrs->ExceptionRecord->ExceptionCode, 
		0,
		s_szFaultReason,
		0,
		NULL);

	return s_szFaultReason;
}
