// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPTerrainDEF
// Comment     : 
// Creation    : metalgeni 2004-08-25 ¿ÀÀü 10:26:19
//***************************************************************************


#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Terrain Message List
typedef enum SPTM {
	SPTM_NULL						= 0,

	SPTM_PURGEALL					= 0x10001010,
	SPTM_INIT						= 0x10001020,
	SPTM_LOADBGI	,
	SPTM_LOADPALLET	,
	SPTM_LOADFORM	,
	SPTM_LOADATTRIBUTE	,
} SPTM;



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Terrain Object Message Function Macro

#define SPTMESSAGE_FUNC SPTMessageMapFunc
#define SPTMESSAGE_MAP_DECLARE() \
protected : \
	virtual int SPTMESSAGE_FUNC ( UINT SPTM, WPARAM wParam, LPARAM lParam);	

#define SPTMESSAGE_MAP_BEGIN( __class ) \
	int __class::SPTMESSAGE_FUNC ( UINT SPTM, WPARAM wParam, LPARAM lParam) {

#define SPTMESSAGE_MAP_END_BASE() \
	return -1;\
	}

#define SPTMESSAGE_COMMAND( message, msg_func )		\
	if ( SPTM == (UINT)message )	{ return msg_func( wParam, lParam ); }




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	SPRotateScrollImage Struct 04.10.15 - jinhee
struct SPRotateScrollImage {
	float			fX;				// start position.			In case fFlowDX is not 0, hit by the effects of the fFlowDX.
	float			fY;			
	float			fSX;			// size.					In case it is not 0, draw position be effected by terrain size.
	float			fSY;
	float			fDX;			// velocity based camera.	In case fSX is not 0, no effect.
	float			fDY;
	float			fFlowDX;		// flow velocity.			In case it 0, no effect.
	float			fFlowDY;	
	float			fRotateStepX;	// rotate step.				In case it 0, no effect.
	float			fRotateStepY;	// rotate step.				In case it 0, no effect.

	float			fScaleX;		// ScaleX jinhee - 04.10.20
	float			fScaleY;		// ScaleY jinhee - 04.10.20

	int				iLightMap;		// LightMap jinhee - 04.11.03

	SPTexture*		pImage;		
	BG_ID			PicID;

	int				iResultRotateX;
	int				iResultRotateY;
	float			fReservedX;		// temp
	float			fReservedY;
	float			fResultX;		// Draw Position
	float			fResultY;

	std::string		strFile;		// Save FileName - jinhee

	SPRotateScrollImage() { SetNull();}
	~SPRotateScrollImage() 
	{
		SAFE_RELEASE(pImage);
	}
	void SetNull() 
	{
		fX  = 0.f;			
		fY  = 0.f;			
		fSX  = 0.f;		
		fSY  = 0.f;
		fDX  = 0.f;		
		fDY  = 0.f;
		fFlowDX = 0.f;		
		fFlowDY = 0.f;		
		fRotateStepX	= 0.f;
		fRotateStepY	= 0.0f;	
		fScaleX = 1.0f;
		fScaleY = 1.0f;
		iLightMap = 0;

		pImage  = NULL;
		PicID  = BG_ID_LAYER_NULL;

		iResultRotateX = 0;
		iResultRotateY = 0;

		fReservedX = 0.f;		// temp
		fReservedY = 0.f;

		fResultX = 0.f;	
		fResultY = 0.f;

		strFile = "";
	}

	void operator=(const SPRotateScrollImage Src)
	{
		fX				= Src.fX			;				
		fY				= Src.fY			;			
		fSX				= Src.fSX			;			
		fSY				= Src.fSY			;
		fDX				= Src.fDX			;			
		fDY				= Src.fDY			;
		fFlowDX			= Src.fFlowDX		;		
		fFlowDY			= Src.fFlowDY		;	
		fRotateStepX	= Src.fRotateStepX	;	
		fRotateStepY	= Src.fRotateStepY	;
		fScaleX			= Src.fScaleX		;
		fScaleY			= Src.fScaleY		;
		iLightMap		= Src.iLightMap		;

		pImage			= Src.pImage		;		
		PicID			= Src.PicID			;

		iResultRotateX	= Src.iResultRotateX	;
		iResultRotateY	= Src.iResultRotateY	;
		fReservedX		= Src.fReservedX	;		
		fReservedY		= Src.fReservedY	;
		fResultX		= Src.fResultX		;		
		fResultY		= Src.fResultY		;

		strFile		= Src.strFile	;
	}

};
































