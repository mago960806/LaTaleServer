#include "StdAfx.h"
#include ".\LGClientLog.h"


BOOL CLGClientLog::m_bInit = FALSE;
BOOL CLGClientLog::m_bLogToFile = FALSE;
BOOL CLGClientLog::m_bLogToDebug = FALSE;
FILE* CLGClientLog::m_fpLog = NULL;

CLGClientLog    g_Log;

bool CLGClientLog::Init(LPSTR lpInitFile)
{
    if (m_bInit)
    {
        return true;
    }

    //m_bLogToFile    =(BOOL)::GetPrivateProfileInt("LoginClientDebConfig", "LogToFile",  0, lpInitFile);
    //m_bLogToDebug   =(BOOL)::GetPrivateProfileInt("LoginClientDebConfig", "LogToDB",    0, lpInitFile);

    //if (FALSE == m_bLogToFile)
    //{
    //    return true;
    //}

    char szFile[MAX_PATH];
    sprintf(szFile, "LGClient.log", "");
    m_fpLog = fopen(szFile, "a");

    if(NULL == m_fpLog)
    {
        return false;
    }

    SYSTEMTIME stTime;
    GetLocalTime(&stTime);
    fprintf(m_fpLog, "%04d-%02d-%02d %02d:%02d:%02d ----------------Start Log----------------\n", 
        stTime.wYear, stTime.wMonth, stTime.wDay, 
        stTime.wHour, stTime.wMinute, stTime.wSecond);

    m_bInit = TRUE;

    return true;
}

CLGClientLog::CLGClientLog()
{
	Init("");
}

CLGClientLog::~CLGClientLog()
{
	UnInit();
}

void CLGClientLog::UnInit()
{
    if (NULL != m_fpLog)
    {
        fclose(m_fpLog);
        m_fpLog = NULL;
    }
}

void CLGClientLog::Log(const char *pszMsg)
{    
    if(NULL == m_fpLog)
        return;

    SYSTEMTIME stTime;
    GetLocalTime(&stTime);

    fprintf(m_fpLog, "%04d-%02d-%02d %02d:%02d:%02d %s\n", 
        stTime.wYear, stTime.wMonth, stTime.wDay, 
        stTime.wHour, stTime.wMinute, stTime.wSecond,
        pszMsg);
}

void CLGClientLog::LogEx(const char * pszFmt,...)
{
    char LogBuffer[1024];
    va_list FmtV;

    va_start(FmtV,pszFmt);
    int nLen = _vsnprintf( LogBuffer,1000, pszFmt, FmtV);
    va_end(FmtV);

    LogBuffer[nLen] = 0;

    if (m_bLogToFile)
    {
        Log(LogBuffer);
    }

    LogBuffer[nLen] = '\n';
    LogBuffer[nLen+1] = '\0';

    if (m_bLogToDebug)
    {
        OutputDebugString(LogBuffer);
    }
}