// ***************************************************************
//  SPTitleManager   version:  1.0   ¡¤  date: 03/29/2006
//  -------------------------------------------------------------
//  
//	AJJIYA
//
//  -------------------------------------------------------------
//  Copyright (C) 2006 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "SPCommon.h"

#include "SPResourceDef.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"

#include "SPTitleRenderDEF.h"
#include "SPTitleRenderBase.h"
#include "SPTitleRenderTexture.h"

#include "SPTitleAttr.h"

#include "SPTitleArchive.h"

#include "SPDebug.h"

#include "PacketID.h"
#include "Packet.h"
#include "SPNetworkManager.h"

#include "SPTitleManager.h"

SPTitleManager::SPTitleManager(void)
{
	m_pTitleManagerInstance	=	NULL;

	Init();
}

SPTitleManager::~SPTitleManager(void)
{
	m_TitleArchive.Destroy();
}

SPTitleManager* SPTitleManager::m_pTitleManagerInstance	=	NULL;

SPTitleManager* SPTitleManager::GetInstance()
{
	if( m_pTitleManagerInstance	==	NULL )
		m_pTitleManagerInstance	=	new	SPTitleManager;

	return	m_pTitleManagerInstance;
}

void SPTitleManager::DelInstance()
{
	delete m_pTitleManagerInstance;
}

//////////////////////////////////////////////////////////////////////////
//
//

SPTitleAttr* SPTitleManager::GetTitleAttr( UINT32 uiTitleID )
{
	STD_MAP_TITLE_ATTR::iterator	mIter	=	m_mTitleAttr.find( uiTitleID );

	if( mIter == m_mTitleAttr.end() )
		return NULL;

	return &((*mIter).second);
}

SPTitleRenderBase* SPTitleManager::GetCreteTitleBase( UINT32 uiTitleID , bool bNoUse )
{
	if( bNoUse == false && uiTitleID == TITLE_ID_NOTUSE )
		return NULL;

	SPTitleAttr*	pTitleAttr	=	GetTitleAttr( uiTitleID );

	if( pTitleAttr == NULL )
		return NULL;

	SPTitleRenderBase*		pTitleRenderBaase	=	NULL;
	SPTitleAttr::TITLE_TYPE	eTitleType			=	pTitleAttr->GetType();

	if( eTitleType == SPTitleAttr::TITLE_TYPE_TEXTURE )
	{
		if( pTitleRenderBaase == NULL )
		{
			pTitleRenderBaase	=	new	SPTitleRenderTexture;
		}

		SPTitleRenderTexture* pTitleRenderTexture = (SPTitleRenderTexture*)(pTitleRenderBaase);

		if( pTitleRenderTexture != NULL )
		{
			RECT	rcSrc	=	pTitleAttr->GetSrcRect();
			pTitleRenderTexture->SetTexture( pTitleAttr->GetBGTexture() , rcSrc.left , rcSrc.top , rcSrc.right , rcSrc.bottom );
		}
	}

	if( eTitleType == SPTitleAttr::TITLE_TYPE_STRING )
	{
		if( pTitleRenderBaase == NULL )
		{
			pTitleRenderBaase	=	new	SPTitleRenderBase;
		}

		if( pTitleRenderBaase != NULL )
		{
			pTitleRenderBaase->SetText( pTitleAttr->GetName() );
			pTitleRenderBaase->SetTextColor( pTitleAttr->GetFontColor() );
			pTitleRenderBaase->SetTextAlign( pTitleAttr->GetAlign() );
			pTitleRenderBaase->SetTextMargin( pTitleAttr->GetMargin() , pTitleAttr->GetVMargin() , 0 , pTitleAttr->GetVMargin() );
		}
	}

	if( pTitleRenderBaase != NULL )
	{
		POINT	ptSize	=	pTitleAttr->GetSize();
		pTitleRenderBaase->SetSize( ptSize.x , ptSize.y );
		pTitleRenderBaase->SetID( pTitleAttr->GetID() );
	}

	return pTitleRenderBaase;
}

SPTitleArchive* SPTitleManager::GetTitleArchive()
{
	return &m_TitleArchive;
}

void SPTitleManager::SendChangeTitle( SPTitleData& stTitleData )
{
	SPTitleAttr*	pTitleAttr	=	GetTitleAttr( stTitleData.m_uiTitleID );

	if( pTitleAttr == NULL )
		return;

	int	iTitleCount = m_TitleArchive.GetCount();

	SPTitleData*	pTitleData	=	NULL;
	BOOL			bFind		=	FALSE;

	if( stTitleData.m_uiTitleID == TITLE_ID_NOTUSE )
	{
		bFind	=	TRUE;
	}
	else
	{
		for( int i = 0 ; i < iTitleCount ; ++i )
		{
			pTitleData	=	m_TitleArchive.GetTitleData( i );

			if( pTitleData != NULL )
			{
				if( pTitleData->m_uiTitleID == stTitleData.m_uiTitleID )
				{
					bFind	=	TRUE;
					break;
				}
			}
		}
	}

	if( bFind == TRUE )
	{
		CPacket Packet;
		Packet.Add( (UINT32)TITLE_CS_CHANGE );
		Packet.Add( (UINT32)stTitleData.m_uiTitleID );

		g_pNetworkManager->SPPerformMessage( TITLE_CS_CHANGE , 0, (LPARAM)&Packet );
	}
}

void SPTitleManager::Init()
{
	m_mTitleAttr.clear();
	m_TitleArchive.Init();

	LoadLDTFile();

	m_iSelectTitle	=	0;
}

void SPTitleManager::LoadLDTFile()
{

	SPLDTFile*	pLDTFile	=	NULL;

	//
	// Title LDT LOAD [3/29/2006 AJJIYA]
	g_pResourceManager->GetLDTFromFileName( RES_FNAME_LDT_TITLE , &pLDTFile );

	if( pLDTFile == NULL )
	{
		DXUTOutputDebugString( "----------------------------------->TITLE LDT Load Fail [%s] \n" , RES_FNAME_LDT_TITLE );
		return;
	}

	LDT_Field ldtFieldName;
	LDT_Field ldtFieldDesc;
	LDT_Field ldtFieldType;
	LDT_Field ldtFieldBGTexture;
	LDT_Field ldtFieldWidth;
	LDT_Field ldtFieldHeight;
	LDT_Field ldtFieldSrcRectLeft;
	LDT_Field ldtFieldSrcRectTop;
	LDT_Field ldtFieldSrcRectRight;
	LDT_Field ldtFieldSrcRectBottom;
	LDT_Field ldtFieldAlign;
	LDT_Field ldtFieldMarjin;
	LDT_Field ldtFieldVMarjin;
	LDT_Field ldtFieldStatus;
	LDT_Field ldtFieldStatusValue;
	LDT_Field ldtFieldEffect;
	LDT_Field ldtFieldEffectDesc;
	LDT_Field ldtFieldFontColorR;
	LDT_Field ldtFieldFontColorG;
	LDT_Field ldtFieldFontColorB;

	int		iRecordCount	= pLDTFile->GetItemCount();
	int		iItemID , iNumber;
	char	szBuf[_MAX_PATH];
	SPTitleAttr	TitleAttrData;

	for( int iRecord = 0 ; iRecord < iRecordCount ; iRecord++ )
	{
		iItemID = pLDTFile->GetPrimaryKey( iRecord );

		TitleAttrData.Init();

		pLDTFile->GetFieldFromLable( iItemID, "_Name",			ldtFieldName			);
		pLDTFile->GetFieldFromLable( iItemID, "_Description",	ldtFieldDesc			);
		pLDTFile->GetFieldFromLable( iItemID, "_Type",			ldtFieldType			);
		pLDTFile->GetFieldFromLable( iItemID, "_ImageFile",		ldtFieldBGTexture		);
		pLDTFile->GetFieldFromLable( iItemID, "_Range_Width",	ldtFieldWidth			);
		pLDTFile->GetFieldFromLable( iItemID, "_Range_height",	ldtFieldHeight			);
		pLDTFile->GetFieldFromLable( iItemID, "_Input1_Left",	ldtFieldSrcRectLeft		);
		pLDTFile->GetFieldFromLable( iItemID, "_Input1_Top",	ldtFieldSrcRectTop		);
		pLDTFile->GetFieldFromLable( iItemID, "_Input1_Right",	ldtFieldSrcRectRight	);
		pLDTFile->GetFieldFromLable( iItemID, "_Input1_Bottom",	ldtFieldSrcRectBottom	);
		pLDTFile->GetFieldFromLable( iItemID, "_Array",			ldtFieldAlign			);
		pLDTFile->GetFieldFromLable( iItemID, "_Margine",		ldtFieldMarjin			);
		pLDTFile->GetFieldFromLable( iItemID, "_Vmargine",		ldtFieldVMarjin			);
		pLDTFile->GetFieldFromLable( iItemID, "_Red",			ldtFieldFontColorR		);
		pLDTFile->GetFieldFromLable( iItemID, "_Green",			ldtFieldFontColorG		);
		pLDTFile->GetFieldFromLable( iItemID, "_Blue",			ldtFieldFontColorB		);

		for( iNumber = 1 ; iNumber <= SPTitleAttr::TITLE_STATUS_COUNT ; ++iNumber )
		{
			sprintf( szBuf , "_StatusType%d" , iNumber );
			pLDTFile->GetFieldFromLable( iItemID, szBuf,		ldtFieldStatus			);
			sprintf( szBuf , "_StatusValue%d" , iNumber );
			pLDTFile->GetFieldFromLable( iItemID, szBuf,		ldtFieldStatusValue		);

			TitleAttrData.SetStatusType( iNumber - 1  , ldtFieldStatus.uData.lValue , ldtFieldStatusValue.uData.lValue );
		}

		for( iNumber = 1 ; iNumber <= SPTitleAttr::TITLE_EFFECT_COUNT ; ++iNumber )
		{
			sprintf( szBuf , "_EffectID%d" , iNumber );
			pLDTFile->GetFieldFromLable( iItemID, szBuf,		ldtFieldEffect			);

			sprintf( szBuf , "_EffectDescription%d" , iNumber );
			pLDTFile->GetFieldFromLable( iItemID, szBuf,		ldtFieldEffectDesc		);

			TitleAttrData.SetEffect( iNumber - 1 , ldtFieldEffect.uData.lValue , ldtFieldEffectDesc.uData.pValue );
		}

		TitleAttrData.SetID( iItemID );
		TitleAttrData.SetName( ldtFieldName.uData.pValue );
		TitleAttrData.SetDesc( ldtFieldDesc.uData.pValue );
		TitleAttrData.SetType( ldtFieldType.uData.lValue );
		TitleAttrData.SetBGTexture( ldtFieldBGTexture.uData.pValue );
		TitleAttrData.SetSize( ldtFieldWidth.uData.lValue , ldtFieldHeight.uData.lValue );
		TitleAttrData.SetSrcRect( ldtFieldSrcRectLeft.uData.lValue , ldtFieldSrcRectTop.uData.lValue , ldtFieldSrcRectRight.uData.lValue , ldtFieldSrcRectBottom.uData.lValue );
		TitleAttrData.SetAlign( ldtFieldAlign.uData.lValue );
		TitleAttrData.SetMargin( ldtFieldMarjin.uData.lValue );
		TitleAttrData.SetVMargin( ldtFieldVMarjin.uData.lValue );
		TitleAttrData.SetFontColor( ldtFieldFontColorR.uData.lValue , ldtFieldFontColorG.uData.lValue , ldtFieldFontColorB.uData.lValue );

		AddTitleAttr( iItemID , TitleAttrData );
	}

	SAFE_RELEASE(pLDTFile);

	//	TITLE LDT LOAD END
}

void SPTitleManager::AddTitleAttr( UINT32 uiTitleID , SPTitleAttr& TitleAttr )
{
	m_mTitleAttr.insert( STD_MAP_TITLE_ATTR::value_type( uiTitleID , TitleAttr ) );
}
