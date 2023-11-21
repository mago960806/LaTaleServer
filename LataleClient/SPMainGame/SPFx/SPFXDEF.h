// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPNetworkDEF
// Comment     : 
// Creation    : metalgeni 2004-08-25 오전 10:26:19
//***************************************************************************


#pragma once

enum SPID_FX_GROUP
{
	IDFXGR_NULL				= 0			,
	IDFXGR_LEVEL_UP			= 506		,
	IDFXGR_ASSULTED_SPEEDY	= 507		,
	IDFXGR_ASSULTED_MIGHTY	= 508		,
	IDFXGR_CRITICAL			= 1002		,
	IDFXGR_DAMAGE			= 5000		,
	IDFXGR_DAMAGE_MISS		= 5003		,
	IDFXGR_DAMAGE_IMMUNE	= 5004		,

	IDFXGR_PET_SHOP			= 1010		,
};

#ifndef _LATALE_SERVER

class SPFXUnit;
class SPFXParticleObject;

typedef	std::vector<SPFXUnit*>						STD_VECTOR_SPFXUNIT;
typedef std::vector<SPFXParticleObject*>			STD_VECTOR_SPFXPARTICLEOBJECT;


enum SPID_FX_MODEL
{
	IDFXM_NULL				= 0			,
	IDFXM_FRAMEANIMATION				,
	IDFXM_PARTICLE						,
};

enum SP_FX_UNIT
{
	FXUNIT_NULL				=	0		,
	FXUNIT_FRAMEANIMATION				,
	FXUNIT_DAMAGE						,
	FXUNIT_CRITICAL						,
	FXUNIT_PARTICLE						,

	FXUNIT_COUNT						,
};

enum SP_FXM_RENDER_LAYER
{
	FXM_RENDER_LAYER_OBJECT_FRONT	=	0	,
	FXM_RENDER_LAYER_OBJECT_BACK			,
	FXM_RENDER_LAYER_UI_FRONT				,
	FXM_RENDER_LAYER_UI_BACK				,

	FXM_RENDER_LAYER_COUNT					,
	FXM_RENDER_LAYER_ALL					,
};

//////////////////////////////////////////////////////////////////////////
//
//	FX Model Version 2.1 일때 사용 하는 클래스
//

enum SPFX_PARTICLE_MOVE
{
	PARTICLE_NULL			=	0		,
	PARTICLE_CHARGE						,
	PARTICLE_SPREAD						,
	PARTICLE_SPREAD_BACK				,
	PARTICLE_DROP						,
	PARTICLE_DROP_BACK					,
};

struct SPFXParticleInstance
{
	D3DXVECTOR2 vec2Pos;
	D3DXVECTOR2 vec2Velocity;
	D3DXVECTOR2 vec2Accel;
};

//////////////////////////////////////////////////////////////////////////

enum SPFX_FOLLOW
{
	FX_FOLLOW_NULL			=	0x0000	,
	FX_FOLLOW_PARENT		=	0x0001	,
	FX_FOLLOW_FACINGPARENT	=	0x0002	,
	FX_FOLLOW_BOTH			=	0x0003	,
	FX_FOLLOW_LOOP			=	0x1000	,
	FX_FOLLOW_SCREENRENDER	=	0x2000	,
};

enum SPFX_PARTICLE_FRAME_DATA_TYPE
{
	PARTICLE_FRAME_DATA_NULL			=	0	,
	PARTICLE_FRAME_DATA_COMMON					,				//	파티클 공통 프레임 데이터			
	PARTICLE_FRAME_DATA_IMAGE					,				//	파티클 이미지 프레임 데이터
};

enum SPFX_PARTICLE_AREA_TYPE
{
	SPFX_PARTICLE_AREA_NULL				=	0	,
	SPFX_PARTICLE_AREA_ELLIPSE					,				//	원
	SPFX_PARTICLE_AREA_AREA						,				//	사각형
};

enum SPFX_PARTICLE_DATA_TYPE
{
	PARTICLE_DATA_NULL					=	0	,
	PARTICLE_DATA_ALPHA							,				//	활성 상태	- 시간마다 전체 파티클로 적용된다.
	PARTICLE_DATA_DIRECTION						,				//	진행 방향	- 생성시 한번만 파티클로 설정된다. 
	PARTICLE_DATA_GRAVITY						,				//	중력		- 해당 시간때의 값이 파티클로 적용된다.
	PARTICLE_DATA_RANGE							,				//	진행 범위	- 생성될때의 정보값으로 참조한다.
	PARTICLE_DATA_MAGNET_POINT					,				//	자석 포인터 - 시간마다 전체 파티클로 적용된다.
	PARTICLE_DATA_RED							,				//	COLOR 빨강	- 시간마다 전체 파티클로 적용된다.
	PARTICLE_DATA_GREEN							,				//	COLOR 녹색	- 시간마다 전체 파티클로 적용된다.
	PARTICLE_DATA_BLUE							,				//	COLOR 파랑	- 시간마다 전체 파티클로 적용된다.
	PARTICLE_DATA_COUNT							,
};

enum SPFX_PARTICLE_OBJECT_DATA_TYPE
{
	PARTICLE_OBJECT_DATA_NULL			=	0	,
	PARTICLE_OBJECT_DATA_VELOCITY				,				//	이동 속도
	PARTICLE_OBJECT_DATA_X_SCALE				,				//	가로 크기
	PARTICLE_OBJECT_DATA_Y_SCALE				,				//	세로 크기
	PARTICLE_OBJECT_DATA_DIRECTION_MOVE			,				//	상대 Y 이동
	PARTICLE_OBJECT_DATA_LIFETIME				,				//	수명
	PARTICLE_OBJECT_DATA_ROTATION_VELOCITY		,				//	회전 속도
	PARTICLE_OBJECT_DATA_ROTATION				,				//	각도
	PARTICLE_OBJECT_DATA_ALPHA					,				//	사라짐
	PARTICLE_OBJECT_DATA_MAGNET_POINT			,				//	자석 포인터
	PARTICLE_OBJECT_DATA_RED					,				//	COLOR 빨강
	PARTICLE_OBJECT_DATA_GREEN					,				//	COLOR 녹색
	PARTICLE_OBJECT_DATA_BLUE					,				//	COLOR 파랑
	PARTICLE_OBJECT_DATA_COUNT					,
};

struct SPFXGroup
{
	SPID_FX_GROUP			FXGroupID;
	std::vector<UINT>		vFXID;
};

typedef std::map< float , float >																STD_MAP_PARTICLE_VALUE;
typedef std::pair< STD_MAP_PARTICLE_VALUE::iterator , STD_MAP_PARTICLE_VALUE::iterator >		STD_PAIR_PARTICLE_VALUE;

#ifndef GetRandom
#define GetRandom(X)						( (float)(X) * rand() / ( RAND_MAX ) )
#endif

// 현재 파일버전 [2/14/2006 AJJIYA]
struct SPFXFrameData
{
	FLOAT		fFrameTime;			// 각 프레임의 Delay.
	FLOAT		fFrameDegree;		// 각 프레임의 회전값.
	FLOAT		fFrameScaleX;		// 각 프레임의 Scale X.
	FLOAT		fFrameScaleY;		// 각 프레임의 Scale Y.
	RECT		RectSrc;			// 각 프레임의 텍스쳐소스.
	RECT		RectAdjust;			// 각 프레임의 렌더타겟의 위치보정값 top, left만 씀.	파티클에서 렌더시 회전 기준 포인트로 사용 Left , Top , 시작프레임 랜덤은 Bottom 의 값 유무로 체크.
	D3DXCOLOR	FrameColor;			// 각 프레임의 컬러.
	bool		bLRSwap;			// 좌우측 뒤집기

	SPFXFrameData() 
		: fFrameTime(0.0f)
		, fFrameDegree(0.0f)
		, fFrameScaleX(0.0f)
		, fFrameScaleY(0.0f)
		, FrameColor(1.0f, 1.0f, 1.0f, 1.0f)
		, bLRSwap(false)
	{
		ZeroMemory(&RectSrc,		sizeof(RECT));
		ZeroMemory(&RectAdjust,		sizeof(RECT));
	}

	void SetNull()
	{
		fFrameTime		=	(0.0f);
		fFrameDegree	=	(0.0f);
		fFrameScaleX	=	(0.0f);
		fFrameScaleY	=	(0.0f);
		FrameColor		=	D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		bLRSwap			=	false;
		ZeroMemory(&RectSrc,		sizeof(RECT));
		ZeroMemory(&RectAdjust,		sizeof(RECT));
	}

	void operator=(const SPFXFrameData Src)
	{
		fFrameTime		= Src.fFrameTime;
		fFrameDegree	= Src.fFrameDegree;
		fFrameScaleX	= Src.fFrameScaleX;
		fFrameScaleY	= Src.fFrameScaleY;
		RectSrc			= Src.RectSrc;
		RectAdjust		= Src.RectAdjust;
		FrameColor		= Src.FrameColor;
		bLRSwap			= Src.bLRSwap;
	}
};

// 2.0 버전 [2/14/2006 AJJIYA]
struct SPFXFrameData20
{
	FLOAT		fFrameTime;			// 각 프레임의 Delay.
	FLOAT		fFrameDegree;		// 각 프레임의 회전값.
	FLOAT		fFrameScale;		// 각 프레임의 Scale.
	RECT		RectSrc;			// 각 프레임의 텍스쳐소스.
	RECT		RectAdjust;			// 각 프레임의 렌더타겟의 위치보정값 top, left만 씀.
	D3DXCOLOR	FrameColor;			// 각 프레임의 컬러.
	bool		bLRSwap;			// 좌우측 뒤집기

	SPFXFrameData20() 
		: fFrameTime(0.0f)
		, fFrameDegree(0.0f)
		, fFrameScale(0.0f)
		, FrameColor(1.0f, 1.0f, 1.0f, 1.0f)
		, bLRSwap(false)
	{
		ZeroMemory(&RectSrc,		sizeof(RECT));
		ZeroMemory(&RectAdjust,		sizeof(RECT));
	}

	void SetNull()
	{
		fFrameTime		=	(0.0f);
		fFrameDegree	=	(0.0f);
		fFrameScale		=	(0.0f);
		FrameColor		=	D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		bLRSwap			=	false;
		ZeroMemory(&RectSrc,		sizeof(RECT));
		ZeroMemory(&RectAdjust,		sizeof(RECT));
	}

	void operator=(const SPFXFrameData20 Src)
	{
		fFrameTime		= Src.fFrameTime;		
		fFrameDegree	= Src.fFrameDegree;	
		fFrameScale		= Src.fFrameScale;	
		RectSrc			= Src.RectSrc;		
		RectAdjust		= Src.RectAdjust;		
		FrameColor		= Src.FrameColor;
		bLRSwap			= Src.bLRSwap;
	}
};

//////////////////////////////////////////////////////////////////////////
//	Particle FX Model 데이터

struct SPFXParticleFrameData
{
	FLOAT		fFrameTime;			// 각 프레임의 Delay.
	FLOAT		fFrameDegree;		// 각 프레임의 회전값.
	RECT		RectArea;			// 각 프레임의 영역.
	FLOAT		fCreateTime;		// 각 프레임의 발생 시간.
	int			iCreateCount;		// 각 프레임의 발생 갯수.
	int			iMagnetPosX;		// 각 프레임의 자석포인터 X
	int			iMagnetPosY;		// 각 프레임의 자석포인터 Y

	SPFXParticleFrameData() 
		: fFrameTime(0.0f)
		, fFrameDegree(0.0f)
		, fCreateTime(0.0f)
		, iCreateCount(0)
		, iMagnetPosX(0)
		, iMagnetPosY(0)
	{
		ZeroMemory(&RectArea,		sizeof(RECT));
	}

	void SetNull()
	{
		fFrameTime		=	(0.0f);
		fFrameDegree	=	(0.0f);
		fCreateTime		=	(0.0f);
		iCreateCount	=	0;
		iMagnetPosX		=	0;
		iMagnetPosY		=	0;
		ZeroMemory(&RectArea,		sizeof(RECT));
	}

	void operator=(const SPFXParticleFrameData Src)
	{
		fFrameTime		= Src.fFrameTime;
		fFrameDegree	= Src.fFrameDegree;
		RectArea		= Src.RectArea;
		fCreateTime		= Src.fCreateTime;
		iCreateCount	= Src.iCreateCount;
		iMagnetPosX		= Src.iMagnetPosX;
		iMagnetPosY		= Src.iMagnetPosY;
	}
};

struct SPFXStreamPartcleData {		// AJJIYA Ver.Now!!! [9/28/2006 AJJIYA]
public:
	SPFX_PARTICLE_AREA_TYPE		m_eAreaType;
	BOOL						m_bPathRotation;

	SPFXStreamPartcleData()
	{
		m_eAreaType			=	SPFX_PARTICLE_AREA_NULL;
		m_bPathRotation		=	FALSE;
	}
};

struct SPFXStreamPartcleData21 {		// AJJIYA Ver2.1 [9/28/2006 AJJIYA]
public:
	int			iMaxRenderCount;
	int			iMaxCount;
	int			iMove;
	float		fDelayTime;
	float		fRadianDegree;
	float		fScale;
	D3DXCOLOR	cRGBA;
	RECT		rtSrc;
	RECT		rtAdjust;

	SPFXStreamPartcleData21()
	{
		iMaxRenderCount	=	0;
		iMaxCount		=	0;
		iMove			=	0;
		fDelayTime		=	0.0f;
		fRadianDegree	=	0.0f;
		fScale			=	0.0f;
		cRGBA			=	D3DXCOLOR( 0.0f , 0.0f , 0.0f , 0.0f );
		SetRect( &rtSrc		, 0 , 0 , 0 , 0 );
		SetRect( &rtAdjust	, 0 , 0 , 0 , 0 );
	}
};

#endif
