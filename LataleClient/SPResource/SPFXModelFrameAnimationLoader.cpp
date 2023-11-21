// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-4-20    15:37 
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
#include "SPFXModelFrameAnimation.h"
#include "SPStream.h"

#include "SPFXModelFrameAnimationLoader.h"

//////////////////////////////////////////////////////////////////////////
//
//	SPFXModelFrameAnimationLoader 시작
//

SPFXModelFrameAnimationLoader::SPFXModelFrameAnimationLoader(void)
{
}

SPFXModelFrameAnimationLoader::~SPFXModelFrameAnimationLoader(void)
{
}

bool SPFXModelFrameAnimationLoader::LoadASCII( SPFXModel* pModel , SPStream* pFile )
{
	if( pFile == NULL || pFile->Valid() == false || pModel == NULL )
		return false;

	std::string	str;
	m_iDeep		=	0;
	m_pModel	=	pModel;

	m_FrameData.SetNull();

	while ( ReadLineFromStream( pFile , str ) != -1 )
	{
		Trim(str);

		EXCUTE_WORDCASE_MAP ( COMMENT				,		OnComment				)
		EXCUTE_WORDCASE_MAP ( "PATH"				,		OnPath					)
		EXCUTE_WORDCASE_MAP ( "LIFE_TIME"			,		OnLifeTime				)
		EXCUTE_WORDCASE_MAP ( "FOLLOW_PARENT"		,		OnFollowParent			)
		EXCUTE_WORDCASE_MAP ( "LIGHT_MAP"			,		OnLightMap				)
		EXCUTE_WORDCASE_MAP ( "FX_RENDER_LAYER"		,		OnRenderLayer			)
		EXCUTE_WORDCASE_MAP ( BLOCK_BEGIN			,		OnBlockBegin			)
		EXCUTE_WORDCASE_MAP ( "TIME"				,		OnTime					)
		EXCUTE_WORDCASE_MAP ( "SCALE"				,		OnScale					)
		EXCUTE_WORDCASE_MAP ( "SCALE_X"				,		OnScaleX				)
		EXCUTE_WORDCASE_MAP ( "SCALE_Y"				,		OnScaleY				)
		EXCUTE_WORDCASE_MAP ( "RADIAN_DEGREE"		,		OnRadianDegree			)
		EXCUTE_WORDCASE_MAP ( "LRSWAP"				,		OnLRSwap				)
		EXCUTE_WORDCASE_MAP ( "COLOR"				,		OnColor					)
		EXCUTE_WORDCASE_MAP ( "SRC_RECT"			,		OnSrcRect				)
		EXCUTE_WORDCASE_MAP ( "ADJUST_RECT"			,		OnAdjustRect			)
		EXCUTE_WORDCASE_MAP ( BLOCK_END				,		OnBlockEnd				)	
	}

	return true;
}

WORDCASE_MAP( SPFXModelFrameAnimationLoader , OnBlockBegin )
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


WORDCASE_MAP( SPFXModelFrameAnimationLoader , OnTime )
{
	m_FrameData.fFrameTime = StrToFloat( &strParam );

	return true;
}

WORDCASE_MAP( SPFXModelFrameAnimationLoader , OnScale )
{
	m_FrameData.fFrameScaleX = StrToFloat( &strParam );
	m_FrameData.fFrameScaleY = m_FrameData.fFrameScaleX;

	return true;
}

WORDCASE_MAP( SPFXModelFrameAnimationLoader , OnScaleX )
{
	m_FrameData.fFrameScaleX = StrToFloat( &strParam );

	return true;
}

WORDCASE_MAP( SPFXModelFrameAnimationLoader , OnScaleY )
{
	m_FrameData.fFrameScaleY = StrToFloat( &strParam );

	return true;
}

WORDCASE_MAP( SPFXModelFrameAnimationLoader , OnRadianDegree )
{
	m_FrameData.fFrameDegree = StrToFloat( &strParam );

	return true;
}

WORDCASE_MAP( SPFXModelFrameAnimationLoader , OnLRSwap )
{
	m_FrameData.bLRSwap = StrToBool( &strParam );

	return true;
}

WORDCASE_MAP( SPFXModelFrameAnimationLoader , OnColor )
{
	m_FrameData.FrameColor = StrToColor( &strParam );

	return true;
}

WORDCASE_MAP( SPFXModelFrameAnimationLoader , OnSrcRect )
{
	m_FrameData.RectSrc = StrToRect( &strParam );

	return true;
}

WORDCASE_MAP( SPFXModelFrameAnimationLoader , OnAdjustRect )
{
	m_FrameData.RectAdjust = StrToRect( &strParam );

	return true;
}

WORDCASE_MAP( SPFXModelFrameAnimationLoader , OnBlockEnd )
{
	assert(m_iDeep);	

	switch(m_iDeep) 
	{
	case 1:
		m_pModel->GetMemberValue( MV_PUSHBACK_FRAMEDATA , (LPARAM)&m_FrameData );
		break;
	default:
		break;
	}
	m_iDeep--;
	return true;
}

bool SPFXModelFrameAnimationLoader::LoadBinary( SPFXModel* pModel , SPStream* pFile , SPFXStreamDefaultData& DefaultData , float fVersion )
{
	if( SPFXModelBaseLoader::LoadBinary( pModel , pFile , DefaultData , fVersion ) == false )
		return false;

	int	iFrameDataCount = 0;

	pFile->Read( &iFrameDataCount , sizeof( iFrameDataCount ) );

	if( iFrameDataCount <= 0 )
		return false;

	SPFXFrameData20	OldFrameData;

	for( int i = 0 ; i < iFrameDataCount ; i++ )
	{
		if( fVersion <= 2.0f )
		{
			pFile->Read( &OldFrameData , sizeof( OldFrameData ) );

			m_FrameData.fFrameTime		= OldFrameData.fFrameTime;
			m_FrameData.fFrameDegree	= OldFrameData.fFrameDegree;
			m_FrameData.fFrameScaleX	= OldFrameData.fFrameScale;
			m_FrameData.fFrameScaleY	= OldFrameData.fFrameScale;
			m_FrameData.RectSrc			= OldFrameData.RectSrc;
			m_FrameData.RectAdjust		= OldFrameData.RectAdjust;
			m_FrameData.FrameColor		= OldFrameData.FrameColor;
			m_FrameData.bLRSwap			= OldFrameData.bLRSwap;
		}
		else
		{
			pFile->Read( &m_FrameData , sizeof( m_FrameData ) );
		}

		m_pModel->GetMemberValue( MV_PUSHBACK_FRAMEDATA , (LPARAM)&m_FrameData );
	}

	return true;
}