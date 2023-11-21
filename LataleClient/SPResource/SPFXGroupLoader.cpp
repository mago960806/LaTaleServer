// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-5-9    13:19 
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
#include "SPUtil.h"
#include "SPFXModelBaseLoader.h"
#include "SPStream.h"


#include "SPFXGroupLoader.h"

//////////////////////////////////////////////////////////////////////////
//
//	SPFXGroupLoader 시작
//

SPFXGroupLoader::SPFXGroupLoader(void)
:	m_pSPFXGroup( NULL )
{
}

SPFXGroupLoader::~SPFXGroupLoader(void)
{
}

SPFXGroup* SPFXGroupLoader::LoadFile( std::string strFileName /* = ""  */ )
{
	m_pSPFXGroup	=	new SPFXGroup;

	if( LoadFileBinary( strFileName ) == false )
	{
		if( IsASCIIFile( strFileName.c_str() ) == true )
		{
			if( LoadFileASCII( strFileName) == false )
				SAFE_DELETE( m_pSPFXGroup );
		}
		else
		{
			SAFE_DELETE( m_pSPFXGroup );
		}
	}

	return m_pSPFXGroup;
}

bool SPFXGroupLoader::LoadFileASCII( std::string strFileName /* = ""  */ )
{
	
	SPStream* pStream;
	SPResourceBase::GetInstance()->GetStreamData( strFileName.c_str(), &pStream);
	if( pStream == NULL || !pStream->Valid() )
	{
		SAFE_RELEASE(pStream);
		return false;
	}

#if defined(_LATALE_CLIENT)
	#if defined(_DEBUG)
		DXUTOutputDebugString( "----------------------------------->FX Group Load ASCII File [%s] \n" , strFileName.c_str() );
	#endif
#endif

	std::string	str;

	while ( ReadLineFromStream( pStream, str ) != -1) 
	{
		Trim(str);

		EXCUTE_WORDCASE_MAP ( COMMENT				,		OnComment				)
		EXCUTE_WORDCASE_MAP ( "GROUP_ID"			,		OnGroupId				)
		EXCUTE_WORDCASE_MAP ( "MODEL_ID"			,		OnModelId				)

	}

	pStream->Release();
	//close( iFileHandle );	

	return true;
}

WORDCASE_MAP( SPFXGroupLoader , OnComment )
{
	return true;
}

WORDCASE_MAP( SPFXGroupLoader , OnGroupId )
{
	if( m_pSPFXGroup == NULL )
		return false;
    
	m_pSPFXGroup->FXGroupID = (SPID_FX_GROUP)( StrToInt( &strParam ) );

	return true;
}

WORDCASE_MAP( SPFXGroupLoader , OnModelId )
{
	if( m_pSPFXGroup == NULL )
		return false;

	m_pSPFXGroup->vFXID.push_back( (UINT)( StrToInt( &strParam ) ) );

	return true;
}

bool SPFXGroupLoader::LoadFileBinary( std::string strFileName /* = ""  */ )
{
	SPStream*	pFile	=	NULL;
	SPResourceBase::GetInstance()->GetStreamData( strFileName.c_str() , &pFile );

	if( pFile == NULL || pFile->Valid() == false )
	{
		SAFE_RELEASE( pFile );
		return false;
	}

	//	파일 헤더 로드
	SPFXStreamHeader	FileHeader;

	pFile->Read( &FileHeader , sizeof( SPFXStreamHeader ) );

	if( FileHeader.iBinary != SPFXStreamHeader::BINARY_CHECK )
	{
		SAFE_RELEASE( pFile );
		return false;
	}

	//	FX Group ID 로드
	int iFXGroupID	=	0;
	pFile->Read( &iFXGroupID , sizeof( iFXGroupID ) );

	if( iFXGroupID <= 0 )
	{
		SAFE_RELEASE( pFile );
		return false;
	}

	m_pSPFXGroup->FXGroupID = (SPID_FX_GROUP)iFXGroupID;

	//	FX Model ID 로드
	int	iFXModelCount	=	0;
	int	iFXModelID		=	0;
	pFile->Read( &iFXModelCount , sizeof( iFXModelCount ) );

	if( iFXModelCount <= 0 )
	{
		SAFE_RELEASE( pFile );
		return false;
	}

	for ( int i = 0 ; i < iFXModelCount ; i++ )
	{
		pFile->Read( &iFXModelID , sizeof( iFXModelID ) );

		if( iFXModelID <= 0 )
		{
			SAFE_RELEASE( pFile );
			return false;
		}

		m_pSPFXGroup->vFXID.push_back( (UINT)( iFXModelID ) );
	}

	SAFE_RELEASE( pFile );
	return true;
}
