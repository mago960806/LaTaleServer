// DebugPrint.cpp

#include "stdafx.h"
#include <stdio.h>
#include <tchar.h>

int g_iTraceMsgLevel = 0;
TCHAR g_szLogDir[500] = "";

#ifdef _LATALE_SERVER
	extern UINT16 g_ServerID;

	#include "Util.h"
	#include "LogFile.h"
	#include "MiniDump.h"
	MiniDumper g_MiniDumper(true);

	#ifdef _DEBUG
		// 메모리 leak 검출
		// 관련 정보 : ms-help://MS.VSCC.2003/MS.MSDNQTR.2003FEB.1042/dnvc60/html/memleaks.htm
		//
		void DebugInit()
		{
			_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);	// 종료시 자동으로 _CrtDumpMemoryLeaks() 호출됨
			_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );

			// Send all reports to STDOUT 
			//_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE ); 
			//_CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDOUT ); 
			//_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_FILE ); 
			//_CrtSetReportFile( _CRT_ERROR, _CRTDBG_FILE_STDOUT ); 
			//_CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_FILE ); 
			//_CrtSetReportFile( _CRT_ASSERT, _CRTDBG_FILE_STDOUT ); 

	#if 0
			//_CrtDumpMemoryLeaks();	// 내부적으로 아래와 같은 함수를 이용한다

			_CrtMemState s1, s2, s3;
			_CrtMemCheckpoint( &s1 );
			// memory allocations take place here
			_CrtMemCheckpoint( &s2 );

			if ( _CrtMemDifference( &s3, &s1, &s2) ) 
				_CrtMemDumpStatistics( &s3 );
	#endif
		}

		void DebugDumpMemoryLeaks()
		{
			_CrtDumpMemoryLeaks();
		}

	#endif
#else
	#include <windows.h>
#endif

void SetTraceLevel(int iLevel/* = 0*/)
{
	g_iTraceMsgLevel = iLevel;
}

void DebugPrint(const TCHAR* lpOutputString, ...)
{
	try
	{
		g_iTraceMsgLevel = 1;
		if(g_iTraceMsgLevel > 0)
		{
			va_list args;
			char tmp[5000];

			va_start(args, lpOutputString);
			vsprintf(tmp, lpOutputString, args);
			va_end(args);

			OutputDebugString(tmp);
		}
	}catch (...) 
	{
	}
}

void DebugPrintW(const WCHAR* pwszOutputString, ...)
{
	try
	{
		g_iTraceMsgLevel = 1;
		if(g_iTraceMsgLevel > 0)
		{
			va_list args;
			WCHAR tmp[5000];

			va_start(args, pwszOutputString);
			vswprintf(tmp, pwszOutputString, args);
			va_end(args);

			OutputDebugStringW(tmp);
		}
	}catch (...) 
	{
	}
}




void ErrorLog(const TCHAR* lpOutputString, ...)
{
	if(g_szLogDir[0] == NULL)
	{
		TCHAR szBuf[MAX_PATH];
#ifdef _LATALE_SERVER
		GetCurrentDir(szBuf);
		_stprintf(g_szLogDir,"%s%s",szBuf,"Log\\");
#else
		::GetCurrentDirectory(MAX_PATH,szBuf);		// Service 상에서는 c:\winnt\system32 가 얻어짐
		_stprintf(g_szLogDir,"%s%s",szBuf,"\\Log\\");
#endif
	}

	try
	{
		va_list args;
		char tmp[5000];

		va_start(args, lpOutputString);
		vsprintf(tmp, lpOutputString, args);
		va_end(args);

	#ifdef _LATALE_SERVER
		CLogFile logFile;
		TCHAR	szFileName[100]={0,};
		sprintf(szFileName,"_Error_%d",g_ServerID);
		if(logFile.Create2("_Error",g_szLogDir))
		{
			logFile.SetDateTimeFormat();

			logFile.WriteString(tmp,true);
			logFile.Flush();
			logFile.Close();
		}
	#else
		OutputDebugString(tmp);
	#endif

	}catch (...) 
	{
	}
}
