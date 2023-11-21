
// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment : 
// Creation : metalgeni 2004-08-16 ¿ÀÈÄ 4:48:46
//***************************************************************************

#pragma once



#ifdef _SP_VIDEO_DEBUG
	#define COMPILED "Debug"
#endif

#ifdef _SP_VIDEO_RDEBUG
	#define COMPILED "ReleaseDebug"
#endif

#ifdef _SP_VIDEO_RELEASE
	#define COMPILED "Release"
#endif

const int GAMMA_STEP		= 1;		// up, down
const int GAMMA_PAGE_STEP	= 5;		// page up, page down
const int GAMMA_MIN			= 10;		// home
const int GAMMA_MAX			= 200;		// end
const int GAMMA_INITIAL		= 130;		// default

extern CRITICAL_SECTION g_TextureLoadSection;

enum FONT_ENUMERATE {	
	FONT_12_NORMAL	= 0,
	FONT_12_BOLD	= 1,
	FONT_14_NORMAL	= 2,
	FONT_14_BOLD	= 3,
	FONT_16_BOLD	= 4,
	FONT_18_BOLD	= 5,

	FONT_MAX		= 6,
};


#define D3DAPPERR_NODIRECT3D          0x82000001
#define D3DAPPERR_NOWINDOW            0x82000002
#define D3DAPPERR_NOCOMPATIBLEDEVICES 0x82000003
#define D3DAPPERR_NOWINDOWABLEDEVICES 0x82000004
#define D3DAPPERR_NOHARDWAREDEVICE    0x82000005
#define D3DAPPERR_HALNOTCOMPATIBLE    0x82000006
#define D3DAPPERR_NOWINDOWEDHAL       0x82000007
#define D3DAPPERR_NODESKTOPHAL        0x82000008
#define D3DAPPERR_NOHALTHISMODE       0x82000009
#define D3DAPPERR_NONZEROREFCOUNT     0x8200000a
#define D3DAPPERR_MEDIANOTFOUND       0x8200000b
#define D3DAPPERR_RESETFAILED         0x8200000c
#define D3DAPPERR_NULLREFDEVICE       0x8200000d

