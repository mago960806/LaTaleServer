#pragma once



#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Windows 헤더에서 거의 사용되지 않는 내용을 제외시킵니다.
#endif

#ifndef WINVER				
#define WINVER 0x0500		
#endif

#ifndef _WIN32_WINNT	
#define _WIN32_WINNT 0x0500
#endif						

#ifndef _WIN32_WINDOWS		
#define _WIN32_WINDOWS 0x0500
#endif

#ifndef _WIN32_IE			
#define _WIN32_IE 0x0600	
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif


#pragma warning(disable : 4702)

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <assert.h>
#include <tchar.h>
#include <time.h>

#include "../../../stdafx.h"

#include "BaseTypes.h"
#include "DebugPrint.h"

#include "PacketID.h"
#include "CPacketComposer.h"

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }


