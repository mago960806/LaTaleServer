// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPRenderModelDEF
// Comment     : 
// Creation    : metalgeni 2004-08-25 ¿ÀÀü 10:26:19
//***************************************************************************

#pragma once

enum MODEL_CLASS_ID {
	MODEL_CLASS_NULL		= 0x0000,

	/////////////////////////////////////////////////////////////////////////////////////////
	// SPGOBModel 
	GOB_MODEL_AVATAR						= 0x10010001,
	GOB_MODEL_NPC_AVATAR,	
	GOB_MODEL_MONSTER,


	/////////////////////////////////////////////////////////////////////////////////////////
	// SPRenderModel
	// Terrain Model
	TERRAIN_MODEL_CLASS_STATIC				= 0x20010001,
	TERRAIN_MODEL_CLASS_FRAME_ANIMATION,
	TERRAIN_MODEL_CLASS_CIRCULAR_MOTION,

	// FX Model
	FX_MODEL_CLASS_FRAME_ANIMATION			= 0x20020001,
	FX_MODEL_CLASS_PARTICLE,
};






































