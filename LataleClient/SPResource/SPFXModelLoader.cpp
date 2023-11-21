// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-5-9    14:15 
//***************************************************************************

#include <io.h>
#include <fcntl.h>

#ifdef _LATALE_CLIENT
#include "SPCommon.h"	
#include "SPDebug.h"
#else 
#include "SPToolCommon.h"
#endif

#include "SPFXDEF.h"
#include "SPFileDEF.H"
#include "SPResourceDef.h"
#include "SPUtil.h"
#include "SPGameMessageDEF.H"
#include "SPRenderModelDEF.h"
#include "SPRenderModel.h"
#include "SPFXModel.h"
#include "SPFXFactory.h"
#include "SPFXModelBaseLoader.h"
#include "SPFXModelFrameAnimationLoader.h"
#include "SPFXModelParticleLoader.h"
#include "SPFXModelFrameAnimation.h"
#include "SPFXModelParticle.h"
#include "SPStream.h"

//#ifdef _DEBUG
//#include "SPDebug.h"
//#endif

#include "SPFXModelLoader.h"

//////////////////////////////////////////////////////////////////////////
//
//	SPFXModelLoader 시작
//

SPFXModelLoader::SPFXModelLoader(void)
{
	m_pFileHandle	=	NULL;
	m_LoaderFactory.RegisterClass<SPFXModelFrameAnimationLoader>( IDFXM_FRAMEANIMATION );
	m_LoaderFactory.RegisterClass<SPFXModelParticleLoader>( IDFXM_PARTICLE );
	m_FXModelFactory.RegisterClass<SPFXModelFrameAnimation>( IDFXM_FRAMEANIMATION );
	m_FXModelFactory.RegisterClass<SPFXModelParticle>( IDFXM_PARTICLE );
}

SPFXModelLoader::~SPFXModelLoader(void)
{
	SAFE_RELEASE( m_pFileHandle );
}

SPFXModel* SPFXModelLoader::LoadFile( std::string strFileName /* = ""  */ )
{
	m_pModel = NULL;

	if( LoadFileBinary( strFileName ) == false )
	{
		if( IsASCIIFile( strFileName.c_str() ) == true )
		{
			if( LoadFileASCII( strFileName ) == false )
				return NULL;
		}
		else
		{
			return NULL;
		}
	}

	return m_pModel;
}


bool SPFXModelLoader::LoadFileASCII( std::string strFileName /* = ""  */ )
{
	SPResourceBase::GetInstance()->GetStreamData( strFileName.c_str(), &m_pFileHandle );

	if( m_pFileHandle == NULL && m_pFileHandle->Valid() == false )
		return false;

#ifdef _LATALE_CLIENT
	#ifdef _DEBUG
		DXUTOutputDebugString( "----------------------------------->FX Model Load ASCII File [%s] \n" , strFileName.c_str() );
	#endif
#endif


	m_eFXModelType		=	IDFXM_NULL;
	m_fFXModelVersion	=	0.0f;

	std::string	str;

	while ( ReadLineFromStream( m_pFileHandle , str ) != -1)
	{
		Trim(str);
		EXCUTE_WORDCASE_MAP ( COMMENT				,		OnComment				)
		EXCUTE_WORDCASE_MAP ( "FX_TYPE"				,		OnFXType				)
		EXCUTE_WORDCASE_MAP ( "FX_VERSION"			,		OnFXVersion				)
		EXCUTE_WORDCASE_MAP ( "FX_ID"				,		OnFXId					)
	}

	SAFE_RELEASE( m_pFileHandle );

	return true;
}

bool SPFXModelLoader::ExeSubLoader( int iFXModelID )
{
	if( m_eFXModelType == IDFXM_NULL )
	{
		if( iFXModelID >= 10000 )
			m_eFXModelType	=	IDFXM_PARTICLE;
		else
			m_eFXModelType	=	IDFXM_FRAMEANIMATION;
	}

	//	종류를 걸러내기 위함.
	SPFXModelBaseLoader*	pSubLoader	=	m_LoaderFactory.CreateClass( m_eFXModelType );

	if( m_fFXModelVersion < 3.0f && m_eFXModelType == IDFXM_PARTICLE )
	{
		SAFE_DELETE( pSubLoader );
//		pSubLoader	=	new	SPFXModelParticleLoader21;
		return false;
	}

	SAFE_DELETE( m_pModel );

	m_pModel	=	m_FXModelFactory.CreateClass( m_eFXModelType );

	if( m_pModel == NULL )
		return false;

	m_pModel->GetMemberValue( MV_SET_FXMODEL_TYPE		,	m_eFXModelType );
	m_pModel->GetMemberValue( MV_SET_FXMODEL_VERSION	,	(LPARAM)&( m_fFXModelVersion ) );
	m_pModel->SetFXModelID( iFXModelID );

	bool bResult = pSubLoader->LoadASCII( m_pModel , m_pFileHandle );

	if( bResult == false )
		SAFE_DELETE( m_pModel );

	//	이미지 경로와 파일명 저장하기
	m_strImgPath = pSubLoader->GetImgPath();
	m_strImgName = pSubLoader->GetImgName();

	SAFE_DELETE( pSubLoader );

	return bResult;
}

WORDCASE_MAP( SPFXModelLoader , OnComment )
{
	return true;
}

WORDCASE_MAP( SPFXModelLoader , OnFXType )
{
	m_eFXModelType	=	(SPID_FX_MODEL)StrToInt( &strParam );

	return true;
}

WORDCASE_MAP( SPFXModelLoader , OnFXVersion )
{
	m_fFXModelVersion	=	StrToFloat( &strParam );

	return true;
}

WORDCASE_MAP( SPFXModelLoader , OnFXId )
{
	ExeSubLoader( StrToInt( &strParam ) );

	return true;
}

bool SPFXModelLoader::LoadFileBinary( std::string strFileName /* = ""  */ )
{
	SPResourceBase::GetInstance()->GetStreamData( strFileName.c_str(), &m_pFileHandle );

	if( m_pFileHandle == NULL || m_pFileHandle->Valid() == false )
	{
		SAFE_RELEASE( m_pFileHandle );
		return false;
	}

	//	파일 헤더 로드
	SPFXStreamHeader	FileHeader;

	m_pFileHandle->Read( &FileHeader , sizeof( SPFXStreamHeader ) );

	if( FileHeader.iBinary != SPFXStreamHeader::BINARY_CHECK )
	{
		SAFE_RELEASE( m_pFileHandle );
		return false;
	}

	//	FX Model 데이터 로드.
	SPFXStreamDefaultData	DefaultData;

	if( FileHeader.fVersion < 3.1f )
	{
		//	FX Model ID 로드.
		int iModelID	=	0;
		m_pFileHandle->Read( &iModelID , sizeof( iModelID ) );

		if( iModelID <= 0 )
		{
			SAFE_RELEASE( m_pFileHandle );
			return false;
		}

		if( iModelID >= 10000 )
			m_eFXModelType	=	IDFXM_PARTICLE;
		else
			m_eFXModelType	=	IDFXM_FRAMEANIMATION;

		DefaultData.eFXModelType	=	m_eFXModelType;
		DefaultData.uiFXModelID		=	iModelID;

		if( FileHeader.fVersion <= 1.1f )
		{
			SPFXStreamDefaultData11	DefaultData11;

			m_pFileHandle->Read( &DefaultData11 , sizeof( DefaultData11 ) );

			DefaultData.eTextureEffect	=	(TEXTURE_EFFECT)DefaultData11.bLightMap;
			DefaultData.iFollowParent	=	DefaultData11.iFollowParent;
			DefaultData.fLifeTime		=	DefaultData11.fLifeTime;
//			DefaultData.fDelayTime		=	DefaultData11.fDelayTime;

			strcpy( DefaultData.szPath , DefaultData11.szPath );
		}
		else
		{
			SPFXStreamDefaultData30 DefaultData30;

			m_pFileHandle->Read( &DefaultData30 , sizeof( DefaultData30 ) );

			DefaultData.eTextureEffect	=	DefaultData30.eTextureEffect;
			DefaultData.iFollowParent	=	DefaultData30.iFollowParent;
			DefaultData.fLifeTime		=	DefaultData30.fLifeTime;
//			DefaultData.fDelayTime		=	DefaultData30.fDelayTime;

			strcpy( DefaultData.szPath , DefaultData30.szPath );
		}
	}
	else if( FileHeader.fVersion == 3.1f )
	{
		SPFXStreamDefaultData31 DefaultData31;

		m_pFileHandle->Read( &DefaultData31 , sizeof( DefaultData31 ) );

		DefaultData.eTextureEffect	=	DefaultData31.eTextureEffect;
		DefaultData.iFollowParent	=	DefaultData31.iFollowParent;
		DefaultData.fLifeTime		=	DefaultData31.fLifeTime;
		DefaultData.uiFXModelID		=	DefaultData31.uiFXModelID;
		DefaultData.eFXModelType	=	DefaultData31.eFXModelType;

		strcpy( DefaultData.szPath , DefaultData31.szPath );
	}
	else
	{
		m_pFileHandle->Read( &DefaultData , sizeof( DefaultData ) );
	}

	for( int i = 0 ; i < sizeof( DefaultData.szPath ) ; i++ )
	{
		if( DefaultData.szPath[i] > 0 )
			DefaultData.szPath[i] -= 255;
	}

	//	종류를 걸러내기 위함.
	SPFXModelBaseLoader*	pSubLoader	=	m_LoaderFactory.CreateClass( DefaultData.eFXModelType );

	if( FileHeader.fVersion < 3.0f && DefaultData.eFXModelType == IDFXM_PARTICLE )
	{
		SAFE_DELETE( pSubLoader );
//		pSubLoader	=	new	SPFXModelParticleLoader21;
		return false;
	}

	SAFE_DELETE( m_pModel );

	m_pModel	=	m_FXModelFactory.CreateClass( DefaultData.eFXModelType );

	if( m_pModel == NULL )
		return false;

	bool bResult = pSubLoader->LoadBinary( m_pModel , m_pFileHandle , DefaultData , FileHeader.fVersion );

	if( bResult == false )
		SAFE_DELETE( m_pModel );

	//	이미지 경로와 파일명 저장하기
	m_strImgPath = pSubLoader->GetImgPath();
	m_strImgName = pSubLoader->GetImgName();

	delete pSubLoader;

	SAFE_RELEASE( m_pFileHandle );

	return bResult;
}
