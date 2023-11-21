#pragma once

#include <Windows.h>
#include <stdio.h>

class CLGClientLog
{
public:
    static bool Init(char * lpInitFile);
    static void LogEx(const char * pszFmt,...);
    static void Log(const char *pszMsg);
    static void UnInit();
	~CLGClientLog();
	CLGClientLog();
protected:
    static BOOL m_bInit;
    static BOOL m_bLogToFile;
    static BOOL m_bLogToDebug;
    static FILE* m_fpLog;
};

extern CLGClientLog g_Log;