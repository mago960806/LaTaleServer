// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-08-07 오후 2:56:36
//***************************************************************************


#pragma once

const float ACCUMULATE_PROCESS_TICK = 0.02f;
const float ACCUMULATE_TIME_UNIT	= 0.05f;
const float ACCUMULATE_TIME_LIMIT	= 1.0f;

#ifdef _LATALE_CLIENT
	#include "SPGameMessageDEF.h"

	#include "SPManagerDef.h"
	#include "SPManager.h"

	#include "SPCameraManager.h"
	#include "SPInputManager.h"

	#include "SPNetworkDEF.h"
	#include "SPNetworkManager.h"

	#include "SPGameObjectDEF.h"
#endif

enum GAME_STATE {
	GS_INIT,
	GS_WAIT,
	GS_GAME,
	GS_CLEAN,
	GS_RELEASE_READY,
	GS_RELEASE,
	GS_FADE_IN,					//[2005/10/17] - 어두어짐
	GS_FADE_OUT,				//[2005/10/17] - 밝아짐
	GS_STAGE_CHANGE,			//[2005/5/30] - 스테이지 변경
	GS_CASHSHOP,
	GS_STORYVIEW,
	GS_BOOKVIEW,
	GS_PVPWAIT,					//[2007/11/20] - PVP Loading Wait
	GS_LOTTERY,					//[xialin 2008/10/13]add
	GS_NULL,
};




