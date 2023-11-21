#pragma once

#define WIN32_LEAN_AND_MEAN		// 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.

#if defined(_DUMMY_CLIENT)
	#include "stdafx.h"
#endif

#if defined(_LATALE_TOOL)
	#include "stdafx.h"
#endif

#include "SPCoreDef.h"			// SPCore에 관한 헤더 묶음.
#include "LaTaleCommonDef.h"	// Launcher , Error 통신용 규칙
#include "LataleClient.h"		// 윈도우즈 관련
#include "SPMainGameDef.h"		// 메인게임 관련
#include "findClass.h"
#include "IGWInterface.h"
#include "IGWLoaderDx9.h"



#define    ARGB(a,r,g,b)       (a<<24 | r<<16 | g<<8 | b)    //[xialin 2008/07/24]add
