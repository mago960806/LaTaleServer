// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-5-9    13:39 
//***************************************************************************

#include <io.h>
#include <fcntl.h>

#ifdef _LATALE_CLIENT
#include "SPCommon.h"	
#else 
#include "SPToolCommon.h"
#include "SPGameMessageDEF.h"
#endif

#include "SPStream.h"

#include "SPFXDEF.h"
#include "SPFileDEF.H"
#include "SPResourceDef.h"
#include "SPUtil.h"
#include "SPRenderModelDEF.h"
#include "SPRenderModel.h"
#include "SPFXModel.h"

#include "SPFXModelBaseLoader.h"

//////////////////////////////////////////////////////////////////////////
//
//	공통으로 사용할 함수들
//

bool IsASCIIFile( const char* pszFileName )
{
	SPStream*	pStream	=	NULL;
	SPResourceBase::GetInstance()->GetStreamData( pszFileName , &pStream);
	if(pStream == NULL || !pStream->Valid())
	{
		SAFE_RELEASE(pStream);
		return false;
	}

	char szBuf[3] = {0};

	ReadLineFromStream( pStream,  szBuf , 2 );

	SAFE_RELEASE( pStream );

	if( strcmp( COMMENT , szBuf ) == 0 )
		return true;

	return false;

}

bool StrToChar	( char* pszOut , std::string *strLine )
{
	std::vector<std::string> vstrArg;
	std::vector<std::string>::iterator Iter;

	if( GetSeperateString( *strLine , vstrArg ) == 1 )
	{
		Iter = vstrArg.begin();

		if(Iter != vstrArg.end())
		{
			strcpy( pszOut , (*Iter).c_str() );
			return true;
		}
	}
	return false;
}

int		StrToInt	( std::string *strLine )
{
	char	pszTemp[MAX_PATH] = {0};

	StrToChar( pszTemp , strLine );

	return atoi( pszTemp );
}

bool	StrToBool	( std::string *strLine )
{
	bool	bTemp = false;

	switch( StrToInt( strLine ) ) {
	case 1:
		bTemp = true;
		break;
	}

	return bTemp;
}

float	StrToFloat	( std::string *strLine )
{
	char	pszTemp[MAX_PATH] = {0};

	StrToChar( pszTemp , strLine );

	return float( atof( pszTemp ) );
}

POINT	StrToPoint	( std::string *strLine )
{
	POINT	ptPoint;
	std::vector<std::string> vstrArg;
	std::vector<std::string>::iterator Iter;

	if( GetSeperateString( *strLine , vstrArg ) == 2 )
	{
		Iter = vstrArg.begin();

		if(Iter != vstrArg.end()) {
			ptPoint.x	= atoi( (*Iter).c_str() );
			++Iter;
		}

		if(Iter != vstrArg.end()) {
			ptPoint.y	= atoi( (*Iter).c_str() );
			++Iter;
		}

	}
	return ptPoint;
}

RECT	StrToRect	( std::string *strLine )
{
	RECT rcRect;
	std::vector<std::string> vstrArg;
	std::vector<std::string>::iterator Iter;

	if( GetSeperateString( *strLine , vstrArg ) == 4 )
	{
		Iter = vstrArg.begin();

		if(Iter != vstrArg.end()) {
			rcRect.left	= atoi( (*Iter).c_str() );
			++Iter;
		}

		if(Iter != vstrArg.end()) {
			rcRect.top	= atoi( (*Iter).c_str() );
			++Iter;
		}

		if(Iter != vstrArg.end()) {
			rcRect.right = atoi( (*Iter).c_str() );
			++Iter;
		}

		if(Iter != vstrArg.end()) {
			rcRect.bottom = atoi( (*Iter).c_str() );
		}
	}
	return rcRect;
}

D3DXCOLOR	StrToColor	( std::string *strLine )
{
	D3DXCOLOR	 tempFrameColor;
	std::vector<std::string> vstrArg;
	std::vector<std::string>::iterator Iter;

	if( GetSeperateString( *strLine , vstrArg ) == 4 )
	{
		Iter = vstrArg.begin();

		if(Iter != vstrArg.end()) {
			tempFrameColor.r	= float ( atof((*Iter).c_str() ) );
			++Iter;
		}

		if(Iter != vstrArg.end()) {
			tempFrameColor.g	= float ( atof((*Iter).c_str() ) );
			++Iter;
		}

		if(Iter != vstrArg.end()) {
			tempFrameColor.b	= float ( atof((*Iter).c_str() ) );
			++Iter;
		}

		if(Iter != vstrArg.end()) {
			tempFrameColor.a	= float ( atof((*Iter).c_str() ) );
		}
	}

	return tempFrameColor;
}

//////////////////////////////////////////////////////////////////////////
//
//	SPFXModelBaseLoader 시작
//

SPFXModelBaseLoader::SPFXModelBaseLoader(void)
{
}

SPFXModelBaseLoader::~SPFXModelBaseLoader(void)
{
}

bool SPFXModelBaseLoader::GetTexture( const char* pszPath , SPTexture** ppTexture )
{
	char	szPath[_MAX_PATH];

	strcpy( szPath , pszPath );

	// AJJIYA [4/28/2005]
	// FXM내의 이미지 경로와 파일명을 분리 저장한다.
	GetSeperateString( std::string( szPath ) , m_strImgPath , m_strImgName );

	const char*	pszFullPath	=	szPath;

#ifdef _CHARTOOL
	pszFullPath	= m_strImgName.c_str();
#endif

	g_pVideo->GetTextureMgr()->LoadTexture( pszFullPath , ppTexture , D3DFMT_DXT3 );

	if( *ppTexture == NULL )
		return false;

	return true;
}

WORDCASE_MAP( SPFXModelBaseLoader , OnComment )
{
	return true;
}

WORDCASE_MAP( SPFXModelBaseLoader , OnPath )
{
	SPTexture*	pTexture;
	char	szFileName[_MAX_PATH] = {0};

	if( StrToChar( szFileName , &strParam ) == false )
		return false;

	GetTexture( szFileName , &pTexture );

	m_pModel->SetImage( &pTexture );

	return true;
}

WORDCASE_MAP( SPFXModelBaseLoader , OnLifeTime )
{
	float fLifeTime		=	StrToFloat( &strParam );

	m_pModel->GetMemberValue( MV_SET_FXLIFETIME ,	(LPARAM)&fLifeTime );

	return true;
}

WORDCASE_MAP( SPFXModelBaseLoader , OnFollowParent )
{
	m_pModel->GetMemberValue( MV_SET_FXFOLLOW_PARENT, StrToInt( &strParam ) );

	return true;
}

WORDCASE_MAP( SPFXModelBaseLoader , OnLightMap )
{
//	(*m_ppModel)->GetMemberValue( MV_SET_LIGHTMAP, StrToBool( &strParam ) );
	m_pModel->GetMemberValue( MV_SET_TEXTURE_EFFECT, StrToInt( &strParam ) );

	return true;
}

WORDCASE_MAP( SPFXModelBaseLoader , OnModelType )
{
	m_pModel->GetMemberValue( MV_SET_FXMODEL_TYPE, StrToInt( &strParam ) );

	return true;
}

WORDCASE_MAP( SPFXModelBaseLoader , OnRenderLayer )
{
	m_pModel->GetMemberValue( MV_SET_FX_RENDER_LAYER, StrToInt( &strParam ) );

	return true;
}

bool SPFXModelBaseLoader::LoadBinary( SPFXModel* pModel , SPStream* pFile , SPFXStreamDefaultData& DefaultData , float fVersion )
{
	if( pFile == NULL || pFile->Valid() == false || pModel == NULL )
		return false;

	m_pModel	=	pModel;

	m_pModel->GetMemberValue( MV_SET_FXMODEL_TYPE		,	DefaultData.eFXModelType );
	m_pModel->GetMemberValue( MV_SET_FXMODEL_VERSION	,	(LPARAM)&( fVersion ) );
	m_pModel->SetFXModelID( DefaultData.uiFXModelID );

	m_pModel->GetMemberValue( MV_SET_FXLIFETIME ,		(LPARAM)&( DefaultData.fLifeTime ) );
	m_pModel->GetMemberValue( MV_SET_FXFOLLOW_PARENT,				DefaultData.iFollowParent );
	m_pModel->GetMemberValue( MV_SET_TEXTURE_EFFECT,				DefaultData.eTextureEffect );
	m_pModel->GetMemberValue( MV_SET_FX_RENDER_LAYER,				DefaultData.iFXMRenderLayer );

	SPTexture*	pTexture;

	GetTexture( DefaultData.szPath , &pTexture );

	m_pModel->SetImage( &pTexture );

	return true;
}
