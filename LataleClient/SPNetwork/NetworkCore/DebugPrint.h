// DebugPrint.h

#pragma once

// Detecting Memory Leak
#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>

	extern void DebugInit();
	extern void DebugDumpMemoryLeaks();
#else
	#define DebugInit()
	#define DebugDumpMemoryLeaks()
#endif

extern void SetTraceLevel(int iLevel = 0);
extern void DebugPrint(const TCHAR* fmt, ...);
extern void DebugPrintW(const WCHAR* pwszOutputString, ...);
extern void ErrorLog(const TCHAR* fmt, ...);

#ifndef SC_TRACE_ON
	#define SC_TRACE(msg)
	#define SC_TRACE2(msg,X)
	#define SC_TRACE3(msg,X,Y)
	#define SC_TRACE4(msg,X,Y,Z)
#else
	#ifdef SC_TRACE_FILE_LINE_ON
		#define SC_TRACE(msg) (DebugPrint("%s(%d) : %s",__FILE__,__LINE__,msg))
		#define SC_TRACE2(msg,X) (DebugPrint("%s(%d) : %s%s",__FILE__,__LINE__,msg,X))
		#define SC_TRACE3(msg,X,Y) (DebugPrint("%s(%d) : %s%s%s",__FILE__,__LINE__,msg,X,Y))
		#define SC_TRACE4(msg,X,Y,Z) (DebugPrint("%s(%d) : %s%s%s%s",__FILE__,__LINE__,msg,X,Y,Z))
	#else
		#define SC_TRACE(msg) (DebugPrint("%s",msg))
		#define SC_TRACE2(msg,X) (DebugPrint("%s%s",msg,X))
		#define SC_TRACE3(msg,X,Y) (DebugPrint("%s%s%s",msg,X,Y))
		#define SC_TRACE4(msg,X,Y,Z) (DebugPrint("%s%s%s%s",msg,X,Y,Z))

		#define SC_TRACE_FMT_PARAM(FMT,PARAM)					(DebugPrint(FMT,#PARAM))
		#define SC_TRACE_FMT_PARAM2(FMT,PARAM1,PARAM2)			(DebugPrint(FMT,#PARAM1,#PARAM2))
		#define SC_TRACE_FMT_PARAM3(FMT,PARAM1,PARAM2,PARAM3)	(DebugPrint(FMT,#PARAM1,#PARAM2,#PARAM3))

	#endif

#endif

#ifdef _DEBUG
	#define SC_ERROR(X)			(ErrorLog("%s(%s,%d) ErrCode=[%ld] : %s",__FILE__,__FUNCTION__,__LINE__,::GetLastError(),X))
	#define SC_ERROR2(X,Y)		(ErrorLog("%s(%s,%d) ErrCode=[%ld] : %s%s",__FILE__,__FUNCTION__,__LINE__,::GetLastError(),X,Y))
	#define SC_ERROR3(X,Y,Z)	(ErrorLog("%s(%s,%d) ErrCode=[%ld] : %s%s%s",__FILE__,__FUNCTION__,__LINE__,::GetLastError(),X,Y,Z))

	#define SC_WSAERROR(X)		(ErrorLog("%s(%s,%d) ErrCode=[%ld] : %s\n",__FILE__,__FUNCTION__,__LINE__,::WSAGetLastError(),#X))
	#define SC_WSAERROR2(X,Y)	(ErrorLog("%s(%s,%d) ErrCode=[%ld] : %s%s\n",__FILE__,__FUNCTION__,__LINE__,::WSAGetLastError(),#X,#Y))
	#define SC_WSAERROR3(X,Y,Z)	(ErrorLog("%s(%s,%d) ErrCode=[%ld] : %s%s%s\n",__FILE__,__FUNCTION__,__LINE__,::WSAGetLastError(),#X,#Y,#Z))

	#define SC_WSAERROR_LOG(dwError,X) if(dwError!=WSAECONNRESET && dwError!=WSAECONNABORTED){ErrorLog("%s(%s,%d) ErrCode=[%ld] : %s\n",__FILE__,__FUNCTION__,__LINE__,dwError,#X);}
#else
	#define SC_ERROR(X)			(ErrorLog("(%s,%d) : %s",__FUNCTION__,__LINE__,X))
	#define SC_ERROR2(X,Y)		(ErrorLog("(%s,%d) : %s%s",__FUNCTION__,__LINE__,X,Y))
	#define SC_ERROR3(X,Y,Z)	(ErrorLog("(%s,%d) : %s%s%s",__FUNCTION__,__LINE__,X,Y,Z))

	#define SC_WSAERROR(X)		(ErrorLog("(%s,%d) EC=[%ld] : %s\n",__FUNCTION__,__LINE__,::WSAGetLastError(),#X))
	#define SC_WSAERROR2(X,Y)	(ErrorLog("(%s,%d) EC=[%ld] : %s%s\n",__FUNCTION__,__LINE__,::WSAGetLastError(),#X,#Y))
	#define SC_WSAERROR3(X,Y,Z)	(ErrorLog("(%s,%d) EC=[%ld] : %s%s%s\n",__FUNCTION__,__LINE__,::WSAGetLastError(),#X,#Y,#Z))

	#define SC_WSAERROR_LOG(dwError,X) if(dwError!=WSAECONNRESET && dwError!=WSAECONNABORTED){ErrorLog("(%s,%d) EC=[%ld] : %s\n",__FUNCTION__,__LINE__,dwError,#X);}
#endif

// assert(false && "alert message") 형태

#define SC_ASSERT(X) assert(false && ##X)

// virual Studio - CRT assertion
//
// ms-help://MS.MSDNQTR.2003FEB.1042/vsdebug/html/vxconCRTAssertions.htm
//_ASSERTE(_CrtIsValidPointer( pMapEvent ,sizeof(MAPEVENT_INFO),TRUE));

//  Visual Studio  - 보고서 매크로
//
// ms-help://MS.MSDNQTR.2003FEB.1042/vsdebug/html/_core_using_macros_for_verification_and_reporting.htm
#ifndef _DEBUG                  /* For RELEASE builds */
#define  ALERT_IF2(expr, msg, arg1, arg2)  do {} while (0)
#else                           /* For DEBUG builds   */
#define  ALERT_IF2(expr, msg, arg1, arg2) \
	do { if ((expr) && (1 == _CrtDbgReport(_CRT_ERROR, __FILE__, __LINE__, msg, arg1, arg2))) _CrtDbgBreak( ); } while (0)
#endif

//if (someVar > MAX_SOMEVAR) _RPTF2(_CRT_WARN, "In NameOfThisFunc( ), someVar= %d, otherVar= %d\n", someVar, otherVar );

extern TCHAR g_szLogDir[500];
