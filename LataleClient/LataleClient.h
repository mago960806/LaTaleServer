#pragma once


extern HWND			g_hWnd;
extern HINSTANCE	g_hInst;

extern TCHAR		g_szLoginID[24];
extern TCHAR		g_szCharName[24];
extern bool			g_LocalPlayerMoveDisable;

extern bool			g_bRunApproval;

extern VIDEO_QUALITY g_VideoQuality;

extern CRITICAL_SECTION g_CriticalSection;

extern COUNTRY_LOCALE	g_eCountryLocale;		// AJJIYA [11/1/2006 AJJIYA]
extern bool				g_bGameGuardPatch;		// AJJIYA [5/26/2008 AJJIYA]