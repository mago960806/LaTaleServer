// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-4-20    15:37 
//***************************************************************************

#include <io.h>
#include <fcntl.h>

#ifdef _LATALE_CLIENT
#include "SPCommon.h"	
#else 
#include "SPToolCommon.h"
#endif

#include "SPFXDEF.h"
#include "SPFileDEF.H"
#include "SPUtil.h"
#include "SPFXGroupLoader.h"
#include "SPFXFactory.h"
#include "SPFXModelLoader.h"

#include "SPRenderModelDEF.h"
#include "SPRenderModel.h"
#include "SPFXModel.h"

#include "SPFXLoader.h"

//////////////////////////////////////////////////////////////////////////
//
//	SPFXLoader Ω√¿€
//

SPFXLoader::SPFXLoader(void)
{
}

SPFXLoader::~SPFXLoader(void)
{
}

SPFXGroup* SPFXLoader::LoadGroupFile( std::string strFileName /* = ""  */ )
{
	if( strFileName.empty() )
		return false;

	return m_FXGroupLoader.LoadFile( strFileName );
}

SPFXModel* SPFXLoader::LoadModelFile( std::string strFileName /* = ""  */ )
{
	if( strFileName.empty() )
		return false;

	return m_FXModelLoader.LoadFile( strFileName );
}
