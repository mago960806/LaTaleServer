// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-5-9    13:52 
//***************************************************************************

#ifdef _LATALE_CLIENT
#include "SPCommon.h"	
#else 
#include "SPToolCommon.h"
#include "SPGameMessageDEF.h"
#endif

#include "SPFXDEF.h"
#include "SPFileDEF.H"
#include "SPResourceDef.h"
#include "SPUtil.h"
#include "SPRenderModelDEF.h"
#include "SPRenderModel.h"
#include "SPFXModel.h"
#include "SPFXModelBaseLoader.h"
#include "SPFXModelParticle.h"
#include "SPStream.h"

#include "SPFXModelParticleLoader.h"

//////////////////////////////////////////////////////////////////////////
//
//	SPFXModelParticleLoader 시작
//

SPFXModelParticleLoader::SPFXModelParticleLoader(void)
{
}

SPFXModelParticleLoader::~SPFXModelParticleLoader(void)
{
}

bool SPFXModelParticleLoader::LoadASCII( SPFXModel* pModel , SPStream* pFile )
{
	if( pFile == NULL || pFile->Valid() == false || pModel == NULL )
		return false;

	std::string				str;
	m_iDeep					=	0;
	m_pModel				=	pModel;
	m_eFrameDataType		=	PARTICLE_FRAME_DATA_NULL;
	m_eValueDataType		=	PARTICLE_DATA_NULL;
	m_eObjectValueDataType	=	PARTICLE_OBJECT_DATA_NULL;
	m_ParticleFrameData.SetNull();
	m_FrameData.SetNull();

	while ( ReadLineFromStream( pFile , str ) != -1 )
	{
		Trim(str);

		EXCUTE_WORDCASE_MAP ( COMMENT						,		OnComment				)
		EXCUTE_WORDCASE_MAP ( "PATH"						,		OnPath					)
		EXCUTE_WORDCASE_MAP ( "LIFE_TIME"					,		OnLifeTime				)
		EXCUTE_WORDCASE_MAP ( "FOLLOW_PARENT"				,		OnFollowParent			)
		EXCUTE_WORDCASE_MAP ( "LIGHT_MAP"					,		OnLightMap				)
		EXCUTE_WORDCASE_MAP ( "FX_RENDER_LAYER"				,		OnRenderLayer			)
		EXCUTE_WORDCASE_MAP ( "AREA_TYPE"					,		OnAreaType				)
		EXCUTE_WORDCASE_MAP ( "PATH_ROTATION"				,		OnPathRotation			)
		EXCUTE_WORDCASE_MAP ( "PARTICLE_FRAME_DATA_TYPE"	,		OnFrameDataType			)
		EXCUTE_WORDCASE_MAP ( "PARTICLE_DATA_TYPE"			,		OnValueDataType			)
		EXCUTE_WORDCASE_MAP ( "PARTICLE_OBJECT_DATA_TYPE"	,		OnObjectValueDataType	)
		EXCUTE_WORDCASE_MAP ( BLOCK_BEGIN					,		OnBlockBegin			)
		EXCUTE_WORDCASE_MAP ( "TIME"						,		OnTime					)
		EXCUTE_WORDCASE_MAP ( "SCALE_X"						,		OnScaleX				)
		EXCUTE_WORDCASE_MAP ( "SCALE_Y"						,		OnScaleY				)
		EXCUTE_WORDCASE_MAP ( "RADIAN_DEGREE"				,		OnRadianDegree			)
		EXCUTE_WORDCASE_MAP ( "LRSWAP"						,		OnLRSwap				)
		EXCUTE_WORDCASE_MAP ( "COLOR"						,		OnColor					)
		EXCUTE_WORDCASE_MAP ( "SRC_RECT"					,		OnSrcRect				)
		EXCUTE_WORDCASE_MAP ( "ADJUST_RECT"					,		OnAdjustRect			)
		EXCUTE_WORDCASE_MAP ( "AREA_RECT"					,		OnAreaRect				)
		EXCUTE_WORDCASE_MAP ( "CREATE_TIME"					,		OnCreateTime			)
		EXCUTE_WORDCASE_MAP ( "CREATE_COUNT"				,		OnCreateCount			)
		EXCUTE_WORDCASE_MAP ( "MAGNET_POS"					,		OnMagnetPos				)
		EXCUTE_WORDCASE_MAP ( "FIRST_KEY_TIME"				,		OnFirstKeyTime			)
		EXCUTE_WORDCASE_MAP ( "SECOND_KEY_VALUE"			,		OnSecondKeyValue		)
		EXCUTE_WORDCASE_MAP ( BLOCK_END						,		OnBlockEnd				)	
	}

	return true;
}

WORDCASE_MAP( SPFXModelParticleLoader , OnAreaType )
{
	m_pModel->GetMemberValue( MV_SET_PARTICLE_AREA_TYPE ,	(LPARAM)(StrToInt( &strParam ) ) );

	return true;
}

WORDCASE_MAP( SPFXModelParticleLoader , OnPathRotation )
{
	m_pModel->GetMemberValue( MV_SET_PARTICLE_PATH_FOLLOW , (LPARAM)StrToBool( &strParam ) );

	return true;
}

WORDCASE_MAP( SPFXModelParticleLoader , OnFrameDataType )
{
	m_eFrameDataType		=	(SPFX_PARTICLE_FRAME_DATA_TYPE)StrToInt( &strParam );
	m_eValueDataType		=	PARTICLE_DATA_NULL;
	m_eObjectValueDataType	=	PARTICLE_OBJECT_DATA_NULL;

	return true;
}

WORDCASE_MAP( SPFXModelParticleLoader , OnValueDataType )
{
	if( m_eValueDataType != PARTICLE_DATA_NULL && m_mValueData.empty() == false )
	{
		m_pModel->GetMemberValue( MV_SET_PARTICLE_ITEM_TYPE , (LPARAM)m_eValueDataType );
		m_pModel->GetMemberValue( MV_SET_PARTICLE_ITEM_DATA , (LPARAM)&m_mValueData );
		m_mValueData.clear();
	}

	m_eFrameDataType		=	PARTICLE_FRAME_DATA_NULL;
	m_eValueDataType		=	(SPFX_PARTICLE_DATA_TYPE)StrToInt( &strParam );
	m_eObjectValueDataType	=	PARTICLE_OBJECT_DATA_NULL;

	return true;
}

WORDCASE_MAP( SPFXModelParticleLoader , OnObjectValueDataType )
{
	if( m_eObjectValueDataType != PARTICLE_OBJECT_DATA_NULL && m_mValueData.empty() == false )
	{
		m_pModel->GetMemberValue( MV_SET_PARTICLE_OBJECT_ITEM_TYPE , (LPARAM)m_eObjectValueDataType );
		m_pModel->GetMemberValue( MV_SET_PARTICLE_OBJECT_ITEM_DATA , (LPARAM)&m_mValueData );
		m_mValueData.clear();
	}

	m_eFrameDataType		=	PARTICLE_FRAME_DATA_NULL;
	m_eValueDataType		=	PARTICLE_DATA_NULL;
	m_eObjectValueDataType	=	(SPFX_PARTICLE_OBJECT_DATA_TYPE)StrToInt( &strParam );

	return true;
}

WORDCASE_MAP( SPFXModelParticleLoader , OnBlockBegin )
{
	m_iDeep++;		

	switch(m_iDeep) {
		case 1:
			{
				//	실제적으로 기존 데이터를 초기화 해야 되는데 실제 생성을
				//	SPFXModelFrameAnimation 에서 생성해서 값에 의한 복사를 해서
				//	여기선 값을 초기화 해줄 필요가 없을 듯 싶다.
			}
			break;
	}

	return true;
}

WORDCASE_MAP( SPFXModelParticleLoader , OnTime )
{
	float	fFrameTime = StrToFloat( &strParam );

	if( m_eFrameDataType == PARTICLE_FRAME_DATA_COMMON )
	{
		m_ParticleFrameData.fFrameTime	=	fFrameTime;
	}
	else if( m_eFrameDataType == PARTICLE_FRAME_DATA_IMAGE )
	{
		m_FrameData.fFrameTime	=	fFrameTime;
	}

	return true;
}

WORDCASE_MAP( SPFXModelParticleLoader , OnScaleX )
{
	m_FrameData.fFrameScaleX = StrToFloat( &strParam );

	return true;
}

WORDCASE_MAP( SPFXModelParticleLoader , OnScaleY )
{
	m_FrameData.fFrameScaleY = StrToFloat( &strParam );

	return true;
}

WORDCASE_MAP( SPFXModelParticleLoader , OnRadianDegree )
{
	float fFrameDegree = StrToFloat( &strParam );

	if( m_eFrameDataType == PARTICLE_FRAME_DATA_COMMON )
	{
		m_ParticleFrameData.fFrameDegree	=	fFrameDegree;
	}
	else if( m_eFrameDataType == PARTICLE_FRAME_DATA_IMAGE )
	{
		m_FrameData.fFrameDegree	=	fFrameDegree;
	}

	return true;
}

WORDCASE_MAP( SPFXModelParticleLoader , OnLRSwap )
{
	m_FrameData.bLRSwap = StrToBool( &strParam );

	return true;
}

WORDCASE_MAP( SPFXModelParticleLoader , OnColor )
{
	m_FrameData.FrameColor = StrToColor( &strParam );

	return true;
}

WORDCASE_MAP( SPFXModelParticleLoader , OnSrcRect )
{
	m_FrameData.RectSrc = StrToRect( &strParam );

	return true;
}

WORDCASE_MAP( SPFXModelParticleLoader , OnAdjustRect )
{
	m_FrameData.RectAdjust = StrToRect( &strParam );

	return true;
}

WORDCASE_MAP( SPFXModelParticleLoader , OnAreaRect )
{
	m_ParticleFrameData.RectArea = StrToRect( &strParam );

	return true;
}

WORDCASE_MAP( SPFXModelParticleLoader , OnCreateTime )
{
	m_ParticleFrameData.fCreateTime = StrToFloat( &strParam );

	return true;
}

WORDCASE_MAP( SPFXModelParticleLoader , OnCreateCount )
{
	m_ParticleFrameData.iCreateCount = StrToInt( &strParam );

	return true;
}

WORDCASE_MAP( SPFXModelParticleLoader , OnMagnetPos )
{
	POINT	ptMagnetPos	=	StrToPoint( &strParam );

	m_ParticleFrameData.iMagnetPosX	=	ptMagnetPos.x;
	m_ParticleFrameData.iMagnetPosY	=	ptMagnetPos.y;

	return true;
}

WORDCASE_MAP( SPFXModelParticleLoader , OnFirstKeyTime )
{
	m_fFirstKeyTime	=	StrToFloat( &strParam );

	return true;
}

WORDCASE_MAP( SPFXModelParticleLoader , OnSecondKeyValue )
{
	m_fSecondKeyValue	=	StrToFloat( &strParam );

	return true;
}

WORDCASE_MAP( SPFXModelParticleLoader , OnBlockEnd )
{
	assert(m_iDeep);	

	switch(m_iDeep) 
	{
	case 1:
		{
			if( m_eFrameDataType != PARTICLE_FRAME_DATA_NULL )
			{
				if( m_eFrameDataType == PARTICLE_FRAME_DATA_COMMON )
				{
					m_pModel->GetMemberValue( MV_PUSHBACK_FRAMEDATA , (LPARAM)&m_ParticleFrameData );
				}
				else if( m_eFrameDataType == PARTICLE_FRAME_DATA_IMAGE )
				{
					m_pModel->GetMemberValue( MV_SET_PARTICLE_PUSHBACK_IMAGE_FRAMEDATA , (LPARAM)&m_FrameData );
				}
			}
			else
			{
				m_mValueData.insert( STD_MAP_PARTICLE_VALUE::value_type( m_fFirstKeyTime , m_fSecondKeyValue ) );
			}
		}
		break;
	default:
		break;
	}
	m_iDeep--;
	return true;
}

bool SPFXModelParticleLoader::LoadBinary( SPFXModel* pModel , SPStream* pFile , SPFXStreamDefaultData& DefaultData , float fVersion )
{
	if( SPFXModelBaseLoader::LoadBinary( pModel , pFile , DefaultData , fVersion ) == false )
		return false;

	SPFXStreamPartcleData	PartcleData;

	pFile->Read( &PartcleData , sizeof( PartcleData ) );

	m_pModel->GetMemberValue( MV_SET_PARTICLE_AREA_TYPE ,	(LPARAM)PartcleData.m_eAreaType );
	m_pModel->GetMemberValue( MV_SET_PARTICLE_PATH_FOLLOW , (LPARAM)PartcleData.m_bPathRotation );

	//////////////////////////////////////////////////////////////////////////

	int	iFrameDataCount = 0;
	int i;

	pFile->Read( &iFrameDataCount , sizeof( iFrameDataCount ) );

	if( iFrameDataCount <= 0 )
		return false;

	for( i = 0 ; i < iFrameDataCount ; i++ )
	{
		pFile->Read( &m_ParticleFrameData , sizeof( m_ParticleFrameData ) );
		m_pModel->GetMemberValue( MV_PUSHBACK_FRAMEDATA , (LPARAM)&m_ParticleFrameData );
	}

	//////////////////////////////////////////////////////////////////////////

	iFrameDataCount = 0;

	pFile->Read( &iFrameDataCount , sizeof( iFrameDataCount ) );

	if( iFrameDataCount <= 0 )
		return false;

	for( i = 0 ; i < iFrameDataCount ; i++ )
	{
		pFile->Read( &m_FrameData , sizeof( m_FrameData ) );
		m_pModel->GetMemberValue( MV_SET_PARTICLE_PUSHBACK_IMAGE_FRAMEDATA , (LPARAM)&m_FrameData );
	}

	//////////////////////////////////////////////////////////////////////////

	int		iCount	=	0;

	for( i = 0 ; i < PARTICLE_DATA_COUNT ; ++i )
	{
		pFile->Read( &m_eValueDataType , sizeof( m_eValueDataType ) );

		if( m_eValueDataType == PARTICLE_DATA_NULL )
			break;

		iCount	=	0;

		pFile->Read( &iCount , sizeof( iCount ) );

		if( iCount < 1 )
			return false;

		for( int j = 0 ; j < iCount ; ++j )
		{
			pFile->Read( &m_fFirstKeyTime , sizeof( m_fFirstKeyTime ) );
			pFile->Read( &m_fSecondKeyValue , sizeof( m_fSecondKeyValue ) );

			m_mValueData.insert( STD_MAP_PARTICLE_VALUE::value_type( m_fFirstKeyTime , m_fSecondKeyValue ) );
		}

		m_pModel->GetMemberValue( MV_SET_PARTICLE_ITEM_TYPE , (LPARAM)m_eValueDataType );
		m_pModel->GetMemberValue( MV_SET_PARTICLE_ITEM_DATA , (LPARAM)&m_mValueData );
		m_mValueData.clear();
	}

	//////////////////////////////////////////////////////////////////////////

	for( i = 0 ; i < PARTICLE_OBJECT_DATA_COUNT ; ++i )
	{
		pFile->Read( &m_eObjectValueDataType , sizeof( m_eObjectValueDataType ) );

		if( m_eObjectValueDataType == PARTICLE_OBJECT_DATA_NULL )
			break;

		iCount	=	0;

		pFile->Read( &iCount , sizeof( iCount ) );

		if( iCount < 1 )
			return false;

		for( int j = 0 ; j < iCount ; ++j )
		{
			pFile->Read( &m_fFirstKeyTime , sizeof( m_fFirstKeyTime ) );
			pFile->Read( &m_fSecondKeyValue , sizeof( m_fSecondKeyValue ) );

			m_mValueData.insert( STD_MAP_PARTICLE_VALUE::value_type( m_fFirstKeyTime , m_fSecondKeyValue ) );
		}

		m_pModel->GetMemberValue( MV_SET_PARTICLE_OBJECT_ITEM_TYPE , (LPARAM)m_eObjectValueDataType );
		m_pModel->GetMemberValue( MV_SET_PARTICLE_OBJECT_ITEM_DATA , (LPARAM)&m_mValueData );
		m_mValueData.clear();
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
//	FXM 모델 2.1 일때 파티클
//
/*
SPFXModelParticleLoader21::SPFXModelParticleLoader21(void)
{
}

SPFXModelParticleLoader21::~SPFXModelParticleLoader21(void)
{
}

bool SPFXModelParticleLoader21::LoadASCII( SPFXModel** ppModel , SPStream* pFile , SPID_FX eSPID_FX )
{
	if( pFile == NULL || pFile->Valid() == false )
		return false;

	std::string		str;	
	m_ppModel	=	ppModel;
	m_eSPID_FX	=	eSPID_FX;

	while ( ReadLineFromStream( pFile , str ) != -1 )
	{
		Trim(str);

		EXCUTE_WORDCASE_MAP ( COMMENT				,		OnComment				)
		EXCUTE_WORDCASE_MAP ( "DEALY"				,		OnDelay					)
		EXCUTE_WORDCASE_MAP ( "PATH"				,		OnPath					)
		EXCUTE_WORDCASE_MAP ( "LIFE_TIME"			,		OnLifeTime				)
		EXCUTE_WORDCASE_MAP ( "LIGHT_MAP"			,		OnLightMap				)
		EXCUTE_WORDCASE_MAP ( "MAX_RENDER_PARTICLE"	,		OnMaxRenderParticle		)
		EXCUTE_WORDCASE_MAP ( "MAX_PARTICLE"		,		OnMaxParticle			)
		EXCUTE_WORDCASE_MAP ( "FOLLOW_PARENT"		,		OnFollowParent			)
		EXCUTE_WORDCASE_MAP ( "RADIAN_DEGREE"		,		OnRadianDegree			)
		EXCUTE_WORDCASE_MAP ( "SCALE"				,		OnScale					)
		EXCUTE_WORDCASE_MAP ( "PARTICLE_MOVE"		,		OnParticleMove			)
		EXCUTE_WORDCASE_MAP ( "COLOR"				,		OnColor					)
		EXCUTE_WORDCASE_MAP ( "SRC_RECT"			,		OnSrcRect				)
		EXCUTE_WORDCASE_MAP ( "ADJUST_RECT"			,		OnAdjustRect			)
	}

	return true;
}

WORDCASE_MAP( SPFXModelParticleLoader21 , OnDelay )
{
	*m_ppModel	=	new SPFXModelParticle21( m_eSPID_FX, StrToFloat( &strParam ) );

	if( *m_ppModel == NULL )
		return false;

	return true;
}

WORDCASE_MAP( SPFXModelParticleLoader21 , OnMaxRenderParticle )
{
	(*m_ppModel)->GetMemberValue( MV_PUSHBACK_MAXRENDERPARTICLE ,	(LPARAM)(StrToInt( &strParam ) ) );

	return true;
}

WORDCASE_MAP( SPFXModelParticleLoader21 , OnMaxParticle )
{
	(*m_ppModel)->GetMemberValue( MV_PUSHBACK_MAXPARTICLE ,	(LPARAM)(StrToInt( &strParam ) ) );

	return true;
}

WORDCASE_MAP( SPFXModelParticleLoader21 , OnRadianDegree )
{
	float fRadianDegree = StrToFloat( &strParam );

	(*m_ppModel)->GetMemberValue( MV_SET_RADIANDEGREE , (LPARAM)&fRadianDegree );	

	return true;
}

WORDCASE_MAP( SPFXModelParticleLoader21 , OnScale )
{
	float fScale = StrToFloat( &strParam );

	(*m_ppModel)->GetMemberValue( MV_PUSHBACK_SCALE , (LPARAM)&fScale );

	return true;
}

WORDCASE_MAP( SPFXModelParticleLoader21 , OnParticleMove )
{
	(*m_ppModel)->GetMemberValue( MV_SET_PARTICLE_MOVE, StrToInt( &strParam ) );

	return true;
}

WORDCASE_MAP( SPFXModelParticleLoader21 , OnColor )
{
	(*m_ppModel)->GetMemberValue( MV_PUSHBACK_COLOR, (LPARAM)&( StrToColor( &strParam ) ) );

	return true;
}

WORDCASE_MAP( SPFXModelParticleLoader21 , OnSrcRect )
{
	RECT rcSrc = StrToRect( &strParam );

	(*m_ppModel)->GetMemberValue( MV_PUSHBACK_SRC_RECT , (LPARAM)&rcSrc );

	return true;
}

WORDCASE_MAP( SPFXModelParticleLoader21 , OnAdjustRect )
{
	RECT rcSrc = StrToRect( &strParam );

	//	AJJIYA	[5/3/2005]
	//	MV_PUSHBACK_ADJUST시 Offset을 함.. 왜 할까?

	RECT*	prcSrc	= (RECT*)(*m_ppModel)->GetMemberValue(MV_GET_ADJUST); 

	*prcSrc = rcSrc;

	//	(*m_ppModel)->GetMemberValue( MV_PUSHBACK_ADJUST , (LPARAM)&rcSrc);

	return true;
}

bool SPFXModelParticleLoader21::LoadBinary( SPFXModel** ppModel , SPStream* pFile , SPID_FX eSPID_FX , SPFXStreamDefaultData& DefaultData , float fVersion )
{
	SPFXStreamPartcleData21	PartcleData;

	pFile->Read( &PartcleData , sizeof( PartcleData ) );

	m_ppModel	=	ppModel;
	*m_ppModel	=	new SPFXModelParticle21( eSPID_FX, DefaultData.fDelayTime );

	SPFXModelBaseLoader::LoadBinary( m_ppModel , pFile , eSPID_FX , DefaultData , fVersion );

	(*m_ppModel)->GetMemberValue( MV_PUSHBACK_MAXRENDERPARTICLE ,	(LPARAM)( PartcleData.iMaxRenderCount ) );
	(*m_ppModel)->GetMemberValue( MV_PUSHBACK_MAXPARTICLE ,			(LPARAM)( PartcleData.iMaxCount ) );
	(*m_ppModel)->GetMemberValue( MV_SET_RADIANDEGREE ,				(LPARAM)&( PartcleData.fRadianDegree ) );	
	(*m_ppModel)->GetMemberValue( MV_PUSHBACK_SCALE ,				(LPARAM)&( PartcleData.fScale ) );
	(*m_ppModel)->GetMemberValue( MV_SET_PARTICLE_MOVE,							PartcleData.iMove );
	(*m_ppModel)->GetMemberValue( MV_PUSHBACK_COLOR,				(LPARAM)&( PartcleData.cRGBA ) );
	(*m_ppModel)->GetMemberValue( MV_PUSHBACK_SRC_RECT ,			(LPARAM)&( PartcleData.rtSrc ) );

	//	AJJIYA	[5/3/2005]
	//	MV_PUSHBACK_ADJUST시 Offset을 함.. 왜 할까?

	RECT*	prcSrc	= (RECT*)(*m_ppModel)->GetMemberValue(MV_GET_ADJUST); 
	*prcSrc = PartcleData.rtAdjust;

	return true;
}

*/