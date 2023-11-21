// ***************************************************************
//  SPWindowGuideBook   version:  1.0   ¡¤  date: 11/19/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "SPCommon.h"
#include "SPUtil.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"

#include "SPTreeBoxDEF.h"
#include "SPTreeBoxItemBase.h"
#include "SPTreeBoxItemTexture.h"

#include "SPWindowTreeBox.h"

#include "SPResourceDef.h"
#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"

#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "SPCommandConvert.h"
#include "SPGOBStatus.h"
#include "SPGOBManager.h"
#include "SPGOBCoordPhysics.h"
#include "SPGameObject.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPPlayerInvenArchive.h"
#include "SPPlayer.h"

#include "SPCheckManager.h"
#include "SPLocalizeManager.h"

#include "SPQuestAttr.h"

#ifdef _DEBUG
#include "SPDebug.h"
#endif

#include "SPWindowGuideBook.h"

SPWindowGuideBook::SPWindowGuideBook( WND_ID WndClassID , INSTANCE_ID InstanceID )
: SPWindow( WndClassID , InstanceID )
{
	Init();
}

SPWindowGuideBook::SPWindowGuideBook( INSTANCE_ID InstanceID , int iX , int iY , int iCX , int iCY , SPWindow* pParent /* = NULL  */ )
: SPWindow( WNDID_GUIDE_BOOK , InstanceID , iX , iY , iCX , iCY , pParent )	
{
	Init();
}

SPWindowGuideBook::~SPWindowGuideBook()
{
	Clean();
}

void SPWindowGuideBook::Init()
{
	m_strToolTip	=	"[SPWindowGuideBook]";
	m_bUse			=	SPLocalizeManager::GetInstance()->IsEnableValue( LV_GUIDEBOOK ) ;
	m_bView			=	false;

	//////////////////////////////////////////////////////////////////////////

	LoadGlobalSysLDT();
	LoadGuideBookLDT();

	//////////////////////////////////////////////////////////////////////////

	SetImage( "DATA/INTERFACE/CONCEPT/UI_WinBG_001.PNG" , 1 , 1 );
	SetSrcSize( 320 , 213 );

	SPWindowButton*		pWindowButton;
	SPWindowStatic*		pWindowStatic;

	pWindowButton	=	new	SPWindowButton( WIID_GUIDE_BOOK_CLOSE_ICON , 422 , 18 , 34 , 34 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 417 , 372 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 417 , 407 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 417 , 442 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 417 , 477 );
	pWindowButton	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_GUIDE_BOOK_TITLE_ICON , 36 , 35 , 186 , 25 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 271 , 67 );

	pWindowStatic	=	new SPWindowStatic( WIID_GUIDE_BOOK_INDEX_ICON , 42 , 79 , 30 , 5 , this );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 267 , 61 );

	pWindowStatic	=	new SPWindowStatic( WIID_GUIDE_BOOK_SUBJECT , 267 , 56 , 181 , 14 , this );
	pWindowStatic->SetFontColor( RGBA( 180 , 63 , 81 , 255 ) );
	pWindowStatic->SetFormat( DT_CENTER | DT_VCENTER );
	pWindowStatic->SetFont( FONT_14_BOLD );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_GUIDE_BOOK_CONTENTS , 267 , 87 , 181 , 188 , this );
	pWindowStatic->SetFontColor( RGBA( 84, 69 , 44 , 255 ) );
	pWindowStatic->SetFormat( DT_LEFT | DT_TOP );
	pWindowStatic->SetFont( FONT_12_NORMAL );
	pWindowStatic->SetMultiLine( true , 2 );
	pWindowStatic	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_GUIDE_BOOK_TYPE + GUIDE_BOOK_TYPE_GENERAL , 79 , 68 , 30 , 25 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" ,  90 , 408 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 121 , 408 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 152 , 408 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 183 , 408 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 214 , 408 );
	pWindowButton =	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_GUIDE_BOOK_TYPE + GUIDE_BOOK_TYPE_QUEST , 109 , 68 , 30 , 25 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" ,  90 , 434 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 121 , 434 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 152 , 434 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 183 , 434 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 214 , 434 );
	pWindowButton =	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_GUIDE_BOOK_TYPE + GUIDE_BOOK_TYPE_SCENARIO , 139 , 68 , 30 , 25 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" ,  90 , 460 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 121 , 460 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 152 , 460 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 183 , 460 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 214 , 460 );
	pWindowButton =	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_GUIDE_BOOK_TYPE + GUIDE_BOOK_TYPE_SKILL , 169 , 68 , 30 , 25 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" ,  90 , 486 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 121 , 486 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 152 , 486 );
	pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 183 , 486 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 214 , 486 );
	pWindowButton =	NULL;

	SPWindowTreeBox*	pTreeBox	=	new SPWindowTreeBox( WIID_GUIDE_BOOK_SUBJECT_TREE , 37 , 96 , 187 , 181 , this );
//	pTreeBox->SetBoxTexture( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 349 , 321 , 355 , 327 , 2 );
	pTreeBox->SetMargin( 0 , 7 , 0 , 0 );
	pTreeBox->SetInterval( 4 );
	pTreeBox->UpdateBGPos();

	pTreeBox->SetScrollUpSize( 14 , 14 );
	pTreeBox->SetScrollUpTexture( DATA_TEXTURE_ENABLE		, "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 452 , 301 );
	pTreeBox->SetScrollUpTexture( DATA_TEXTURE_HIT			, "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 467 , 301 );
	pTreeBox->SetScrollUpTexture( DATA_TEXTURE_PUSH			, "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 482 , 301 );
	pTreeBox->SetScrollUpTexture( DATA_TEXTURE_DISABLE		, "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 497 , 301 );

	pTreeBox->SetScrollDownSize( 14 , 14 );
	pTreeBox->SetScrollDownTexture( DATA_TEXTURE_ENABLE		, "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 452 , 497 );
	pTreeBox->SetScrollDownTexture( DATA_TEXTURE_HIT		, "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 467 , 497 );
	pTreeBox->SetScrollDownTexture( DATA_TEXTURE_PUSH		, "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 482 , 497 );
	pTreeBox->SetScrollDownTexture( DATA_TEXTURE_DISABLE	, "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 497 , 497 );

	pTreeBox->SetScrollSliderSize( 14 , 26 );
	pTreeBox->SetScrollSliderTexture( DATA_TEXTURE_ENABLE	, "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 452 , 470 );
	pTreeBox->SetScrollSliderTexture( DATA_TEXTURE_HIT		, "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 467 , 470 );
	pTreeBox->SetScrollSliderTexture( DATA_TEXTURE_PUSH		, "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 482 , 470 );
	pTreeBox->SetScrollSliderTexture( DATA_TEXTURE_DISABLE	, "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 497 , 470 );

	pTreeBox->SetScrollSliderBGTexture( DATA_TEXTURE_ENABLE	, "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 452 , 316 , 466 , 469 );
	pTreeBox->SetScrollSliderBGTexture( DATA_TEXTURE_HIT	, "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 467 , 316 , 481 , 469 );
	pTreeBox->SetScrollSliderBGTexture( DATA_TEXTURE_PUSH	, "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 482 , 316 , 496 , 469 );
	pTreeBox->SetScrollSliderBGTexture( DATA_TEXTURE_DISABLE, "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 497 , 316 , 511 , 469 );

	pTreeBox->SetScrollMargin( 0 , 0 , 0 , 0 );
	pTreeBox->UpdateScrollPos();

	pTreeBox->Refresh();

	//////////////////////////////////////////////////////////////////////////

	OnType( WIID_GUIDE_BOOK_TYPE + GUIDE_BOOK_TYPE_GENERAL , NULL , NULL );

	m_pCurSelectItem	=	NULL;
}

void SPWindowGuideBook::Clean()
{
	SPWindow::Clean();
}

void SPWindowGuideBook::Show( bool bWithChild /* = true  */ )
{
	if( m_bUse == false )
		return;

	if( m_bShow == true )
		g_pInterfaceManager->RegisterSoundUnit( "DATA/SOUND/SYSTEM_CLOSE_WINDOW.WAV" );

	SPWindow::Show( bWithChild );

	if( m_bView == false )
	{
		if( g_pGOBManager != NULL )
		{
			SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

			if( pLocalPlayer != NULL )
			{
				SPGOBStatus* pPlayerStatus	=	pLocalPlayer->GetStatus();

				if( pPlayerStatus != NULL )
				{
					UpdateLevel( ConvertLevelToKey( pPlayerStatus->GetStatusValue( STATUS_LV ) ) );
				}
			}
		}
	}
}

void SPWindowGuideBook::Hide( bool bSendServer /* = true */ )
{
	if( m_bShow == true )
		g_pInterfaceManager->RegisterSoundUnit("DATA/SOUND/SYSTEM_CLOSE_WINDOW.WAV");

	SPWindow::Hide( bSendServer );
}

void SPWindowGuideBook::Process( float fTime )
{
	if( m_bShow == false )
		return;

	SPWindow::Process( fTime );
}

void SPWindowGuideBook::Render( float fTime )
{
	if( m_bShow == false )
		return;

	g_pVideo->Flush();

	SPWindow::Render( fTime );

	g_pVideo->Flush();
}

//void SPWindowGuideBook::RefreshRelationCoord()
//{
//	SPWindow::RefreshRelationCoord();
//}

SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowGuideBook )
	SPIMESSAGE_COMMAND(	SPIM_GUIDE_BOOK_VIEW		,											OnView				)

	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_GUIDE_BOOK_CLOSE_ICON			,	OnClose				)
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP				,	WIID_GUIDE_BOOK_SUBJECT_TREE		,	OnTreeBoxLButtonUp	)

	SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP		,	WIID_GUIDE_BOOK_TYPE	,	WIID_GUIDE_BOOK_TYPE + GUIDE_BOOK_TYPE_COUNT	,	OnType				)
SPIMESSAGE_OBJ_MAP_END	( SPWindow	)

int SPWindowGuideBook::OnView( WPARAM wParam, LPARAM lParam )
{
	int	iGuideBookID	=	(int)wParam;

	if( iGuideBookID <= 0 )
		return 1;

	UpdateItemNo( iGuideBookID );

	m_bView	=	true;

	Show();

	m_bView	=	false;

	return 1;
}

int SPWindowGuideBook::OnClose( WPARAM wParam, LPARAM lParam )
{
	Hide();
	return 1;
}

int SPWindowGuideBook::OnTreeBoxLButtonUp( WPARAM wParam, LPARAM lParam )
{
	TREEBOX_ITEM*	pSelectQuestItem	=	(TREEBOX_ITEM*)wParam;

	if( pSelectQuestItem == NULL )
		return 1;

	if( pSelectQuestItem->m_pItemBase == NULL )
		return 1;

	GUIDE_BOOK_LDT*	pGuildBookLDT	=	GetGuideBookItemNo( pSelectQuestItem->m_pItemBase->GetReturnValue() );

	if( pGuildBookLDT == NULL )
		return 1;

	SetSubject( pGuildBookLDT->m_strSubject.c_str() );
	SetContents( pGuildBookLDT->m_strContent.c_str() );

	pSelectQuestItem->m_pItemBase->SetFontEnumerate( FONT_12_BOLD );

	if( m_pCurSelectItem != NULL )
	{
		pGuildBookLDT	=	GetGuideBookItemNo( m_pCurSelectItem->GetReturnValue() );

		if( pGuildBookLDT != NULL )
		{
			if( pGuildBookLDT->m_bBold == true )
				m_pCurSelectItem->SetFontEnumerate( FONT_12_BOLD );
			else
				m_pCurSelectItem->SetFontEnumerate( FONT_12_NORMAL );
		}
	}

	m_pCurSelectItem	=	(SPTreeBoxItemTexture*)pSelectQuestItem->m_pItemBase;

	return 1;
}

int SPWindowGuideBook::OnType( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	GUIDE_BOOK_TYPE	eType	=	(GUIDE_BOOK_TYPE)( iID - WIID_GUIDE_BOOK_TYPE );

	SetType( eType );
	SetTypeButton( eType );

	if( g_pGOBManager != NULL )
	{
		SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

		if( pLocalPlayer != NULL )
		{
			SPGOBStatus* pPlayerStatus	=	pLocalPlayer->GetStatus();

			if( pPlayerStatus != NULL )
				UpdateLevel( ConvertLevelToKey( pPlayerStatus->GetStatusValue( STATUS_LV ) ) );
		}
	}

	return 1;
}

void SPWindowGuideBook::LoadGlobalSysLDT()
{
	m_iLevelRange			=	0;

	SPLDTFile*	pLDTFile	=	NULL;

	g_pResourceManager->GetLDTFromFileName( RES_FNAME_LDT_GLOBAL_SYS , &pLDTFile );

	if( pLDTFile == NULL )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "-----------------------------------> LDT Load Fail [ %s ] \n" , RES_FNAME_LDT_GLOBAL_SYS );
#endif
		return;
	}

	LDT_Field	ldtFieldLevelRange;

	pLDTFile->GetField( 7 , 0 ,	ldtFieldLevelRange );		//	_LevelRange

	m_iLevelRange	=	ldtFieldLevelRange.uData.lValue;

	SAFE_RELEASE( pLDTFile );
}

void SPWindowGuideBook::LoadGuideBookLDT()
{
	SPLDTFile*	pLDTFile	=	NULL;

	g_pResourceManager->GetLDTFromFileName( RES_FNAME_LDT_GUIDE_BOOK , &pLDTFile );

	if( pLDTFile == NULL )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "-----------------------------------> LDT Load Fail [ %s ] \n" , RES_FNAME_LDT_GUIDE_BOOK );
#endif
		return;
	}

	LDT_Field					ldtFieldGroup;
	LDT_Field					ldtFieldRed;
	LDT_Field					ldtFieldGreen;
	LDT_Field					ldtFieldBlue;
	LDT_Field					ldtFieldBold;
	LDT_Field					ldtFieldSubject;
	LDT_Field					ldtFieldContent;
	LDT_Field					ldtFieldSectionLevel;
	LDT_Field					ldtFieldRequireType;
	LDT_Field					ldtFieldRequireID;
	LDT_Field					ldtFieldRequireValue1;
	LDT_Field					ldtFieldRequireValue2;

	int							iRecordCount	=	pLDTFile->GetItemCount();
	int							iItemID;
	int							iItemIDStep;

	GUIDE_BOOK_LDT				stGuideBookLDT;
	SPRequire					stRequire;

	for( int iRecord = 0 ; iRecord < iRecordCount ; iRecord++ )
	{
		iItemID		=	pLDTFile->GetPrimaryKey( iRecord );

		if( iItemID <= 0 )
			continue;

		pLDTFile->GetField( iItemID, 0	,		ldtFieldGroup						);			//	_Group
		pLDTFile->GetField( iItemID, 1	,		ldtFieldSubject						);			//	_Subject
		pLDTFile->GetField( iItemID, 2	,		ldtFieldRed							);			//	_Subject_Red
		pLDTFile->GetField( iItemID, 3	,		ldtFieldGreen						);			//	_Subject_Green
		pLDTFile->GetField( iItemID, 4	,		ldtFieldBlue						);			//	_Subject_Blue
		pLDTFile->GetField( iItemID, 5	,		ldtFieldBold						);			//	_Subject_Bold
		pLDTFile->GetField( iItemID, 6	,		ldtFieldContent						);			//	_Content
		pLDTFile->GetField( iItemID, 7	,		ldtFieldSectionLevel				);			//	_Section_Lv

		iItemIDStep	=	8;

		for( int i = 0 ; i < 4 ; ++i )
		{
				pLDTFile->GetField( iItemID, iItemIDStep++		,	ldtFieldRequireType		);			//	_Require_Type
				pLDTFile->GetField( iItemID, iItemIDStep++		,	ldtFieldRequireID		);			//	_Require_ID
				pLDTFile->GetField( iItemID, iItemIDStep++		,	ldtFieldRequireValue1	);			//	_Require_Value1
				pLDTFile->GetField( iItemID, iItemIDStep++		,	ldtFieldRequireValue2	);			//	_Require_Value2

				stRequire.SetRequire( (int)ldtFieldRequireType.uData.lValue , (int)ldtFieldRequireID.uData.lValue , ldtFieldRequireValue1.uData.lValue , ldtFieldRequireValue2.uData.lValue );

				stGuideBookLDT.m_vRequireBuffer.push_back( stRequire );
		}

		stGuideBookLDT.m_iItemNo		=	iItemID;
		stGuideBookLDT.m_eGroup			=	(GUIDE_BOOK_TYPE)ldtFieldGroup.uData.lValue;
		stGuideBookLDT.m_iSectionLevel	=	ldtFieldSectionLevel.uData.lValue;

		stGuideBookLDT.m_cSubjectColor	=	D3DCOLOR_ARGB( 255 ,	ldtFieldRed.uData.lValue	,
																	ldtFieldGreen.uData.lValue	,
																	ldtFieldBlue.uData.lValue	);
		stGuideBookLDT.m_bBold			=	ldtFieldBold.uData.lValue ? true : false;

		if( ldtFieldSubject.uData.pValue != NULL )
			stGuideBookLDT.m_strSubject	=	ldtFieldSubject.uData.pValue;

		if( ldtFieldContent.uData.pValue != NULL )
			stGuideBookLDT.m_strContent	=	ldtFieldContent.uData.pValue;

		AddGuideBookItemNo( stGuideBookLDT );
		AddGuideBookLevel( GetGuideBookItemNo( stGuideBookLDT.m_iItemNo ) );

		stGuideBookLDT.m_strSubject.clear();
		stGuideBookLDT.m_strContent.clear();
		stGuideBookLDT.m_vRequireBuffer.clear();
	}

	SAFE_RELEASE( pLDTFile );
}

int SPWindowGuideBook::ConvertLevelToKey( int iLevel )
{
	if( m_iLevelRange <= 0 || iLevel <= 0 )
		return 0;

	return ( iLevel / m_iLevelRange ) * m_iLevelRange;
}

void SPWindowGuideBook::AddGuideBookItemNo( GUIDE_BOOK_LDT& stGuideBook )
{
	m_mGuideBookItemNo.insert( STD_MAP_GUIDE_BOOK_LDT::value_type( stGuideBook.m_iItemNo , stGuideBook ) );
}

void SPWindowGuideBook::AddGuideBookLevel( GUIDE_BOOK_LDT* pstGuideBook )
{
	if( pstGuideBook == NULL )
		return;

	int	iKey	=	ConvertLevelToKey( pstGuideBook->m_iSectionLevel );

	STD_MAP_GUIDE_BOOK_LDT_P*	pmGuideBookLDT	=	GetGuideBookLevel( iKey );

	if( pmGuideBookLDT != NULL )
	{
		pmGuideBookLDT->insert( STD_MAP_GUIDE_BOOK_LDT_P::value_type( pstGuideBook->m_iItemNo , pstGuideBook ) );
	}
	else
	{
		STD_MAP_GUIDE_BOOK_LDT_P	mGuideBookLDT;

		mGuideBookLDT.insert( STD_MAP_GUIDE_BOOK_LDT_P::value_type( pstGuideBook->m_iItemNo , pstGuideBook ) );

		m_mGuideBookLevel.insert( STD_MAP_GUIDE_BOOK::value_type( iKey , mGuideBookLDT ) );
	}
}

GUIDE_BOOK_LDT* SPWindowGuideBook::GetGuideBookItemNo( int iItemNo )
{
	GUIDE_BOOK_LDT*						pLdtGuideBook	=	NULL;
	STD_MAP_GUIDE_BOOK_LDT::iterator	mIter			=	m_mGuideBookItemNo.find( iItemNo );

	if( mIter != m_mGuideBookItemNo.end() )
		pLdtGuideBook	=	&((*mIter).second);

	return pLdtGuideBook;
}

STD_MAP_GUIDE_BOOK_LDT_P* SPWindowGuideBook::GetGuideBookLevel( int iLevel )
{
	STD_MAP_GUIDE_BOOK_LDT_P*		pmGuideBookLDT	=	NULL;
	STD_MAP_GUIDE_BOOK::iterator	mIter			=	m_mGuideBookLevel.find( iLevel );

	if( mIter != m_mGuideBookLevel.end() )
		pmGuideBookLDT	=	&((*mIter).second);

	return pmGuideBookLDT;
}

SPTreeBoxItemTexture* SPWindowGuideBook::CreateTreeBoxItem()
{
	SPTreeBoxItemTexture*	pGuideBookItem	=	new SPTreeBoxItemTexture;

	if( pGuideBookItem == NULL )
		return NULL;

	//////////////////////////////////////////////////////////////////////////

	pGuideBookItem->SetTexture( DATA_TEXTURE_HIT	*	2		, "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 245 , 480 , 405 , 495 );
	pGuideBookItem->SetTexture( DATA_TEXTURE_HIT	*	2 + 1	, "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 245 , 480 , 405 , 495 );
	pGuideBookItem->SetTexture( DATA_TEXTURE_PUSH	*	2		, "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 245 , 480 , 405 , 495 );
	pGuideBookItem->SetTexture( DATA_TEXTURE_PUSH	*	2 + 1	, "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 245 , 480 , 405 , 495 );
	pGuideBookItem->SetTexture( DATA_TEXTURE_SELECT	*	2		, "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 245 , 496 , 405 , 511 );
	pGuideBookItem->SetTexture( DATA_TEXTURE_SELECT	*	2 + 1	, "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG" , 245 , 496 , 405 , 511 );
	pGuideBookItem->SetSize( 160 , 15 );
	pGuideBookItem->SetTextMargin( 22 , 0 );

	//////////////////////////////////////////////////////////////////////////

	return pGuideBookItem;
}

void SPWindowGuideBook::SetTreeBoxItemData( SPTreeBoxItemTexture* pItem , GUIDE_BOOK_LDT* pstGuideBook )
{
	if( pItem == NULL || pstGuideBook == NULL )
		return;

	for( int j = 0 ; j < DATA_STRING_COUNT ; ++j )
	{
		pItem->SetTextColor( j , pstGuideBook->m_cSubjectColor );
	}

	if( pstGuideBook->m_bBold == true )
		pItem->SetFontEnumerate( FONT_12_BOLD );
	else
		pItem->SetFontEnumerate( FONT_12_NORMAL );

	pItem->SetText( pstGuideBook->m_strSubject.c_str() );
	pItem->SetReturnValue( pstGuideBook->m_iItemNo );
}

void SPWindowGuideBook::SetTreeBoxItem( int iPos , GUIDE_BOOK_LDT* pstGuideBook )
{
	if( iPos < 0 )
		return;

	SPWindowTreeBox*	pTreeBox	=	(SPWindowTreeBox*)( Find( WIID_GUIDE_BOOK_SUBJECT_TREE ) );

	if( pTreeBox == NULL )
		return;

	TREEBOX_ITEM*	pRootItem	=	pTreeBox->GetRootItem();

	if( pRootItem == NULL )
		return;

	int iCount = (int)pRootItem->m_vpChild.size();

	TREEBOX_ITEM*	pCurItem	=	NULL;

	if( iPos < iCount )
		pCurItem	=	pRootItem->m_vpChild.at( iPos );

	if( pCurItem == NULL )
		pCurItem	=	pTreeBox->AddItem( NULL , (SPTreeBoxItemBase*)CreateTreeBoxItem() );

	if( pCurItem == NULL )
		return;

	if( pstGuideBook == NULL )
	{
		pCurItem->m_bShow = false;
	}
	else
	{
		pCurItem->m_bShow = true;
		SetTreeBoxItemData( (SPTreeBoxItemTexture*)pCurItem->m_pItemBase , pstGuideBook );
	}
}

void SPWindowGuideBook::UpdateItemNo( int iItemNo )
{
	GUIDE_BOOK_LDT*	pGuideBookLDT	=	GetGuideBookItemNo( iItemNo );

	if( pGuideBookLDT == NULL )
		return;

	SPWindowTreeBox*	pTreeBox	=	(SPWindowTreeBox*)( Find( WIID_GUIDE_BOOK_SUBJECT_TREE ) );

	if( pTreeBox == NULL )
		return;

	int	iCount	=	pTreeBox->GetItemCount( true );

	SetTreeBoxItem( 0 , pGuideBookLDT );

	for( int i = 1 ; i < iCount ; ++i )
	{
		SetTreeBoxItem( i , NULL );
	}

	SetSubject( pGuideBookLDT->m_strSubject.c_str() );
	SetContents( pGuideBookLDT->m_strContent.c_str() );

	TREEBOX_ITEM*	pSelectItem	=	pTreeBox->GetSelectItem();

	if( pSelectItem != NULL )
		pSelectItem->m_pItemBase->SetStatus( DATA_STATUS_SELECT );

	pTreeBox->SetSelectItem( NULL );
	pTreeBox->Refresh();

	m_pCurSelectItem	=	NULL;
}

void SPWindowGuideBook::UpdateLevel( int iLevel )
{
	SetSubject( NULL );
	SetContents( NULL );

	SPWindowTreeBox*	pTreeBox	=	(SPWindowTreeBox*)( Find( WIID_GUIDE_BOOK_SUBJECT_TREE ) );

	if( pTreeBox == NULL )
		return;

	int	iCurPos	=	0;

	iCurPos		+=	UpdateLevel( iCurPos , GetGuideBookLevel( iLevel - m_iLevelRange ) , (SPWindow*)pTreeBox );
	iCurPos		+=	UpdateLevel( iCurPos , GetGuideBookLevel( iLevel ) , (SPWindow*)pTreeBox );

	int	iUICount	=	pTreeBox->GetItemCount( true );

	for( int i = iCurPos ; i < iUICount ; ++i )
	{
		SetTreeBoxItem( i , NULL );
	}

	TREEBOX_ITEM*	pSelectItem	=	pTreeBox->GetSelectItem();

	if( pSelectItem != NULL )
		pSelectItem->m_pItemBase->SetStatus( DATA_STATUS_ENABLE );

	pTreeBox->SetSelectItem( NULL );
	pTreeBox->Refresh();

	m_pCurSelectItem	=	NULL;
}

int SPWindowGuideBook::UpdateLevel( int iStartItemPos , STD_MAP_GUIDE_BOOK_LDT_P* pmGuideBookLDT , SPWindow* pTreeBoxWindow )
{
	if( pmGuideBookLDT == NULL )
		return 0;

	SPWindowTreeBox*	pTreeBox	=	(SPWindowTreeBox*)pTreeBoxWindow;

	if( pTreeBox == NULL )
		return 0;

	int										iUpdateCount	=	0;
	STD_MAP_GUIDE_BOOK_LDT_P::iterator		mIter			=	pmGuideBookLDT->begin();
	GUIDE_BOOK_LDT*							pGuideBookLDT	=	NULL;
	STD_VECTOR_GUIDE_BOOK_REQUIRE::iterator	Iter;
	bool									bCheckResult	=	false;

	while( mIter != pmGuideBookLDT->end() )
	{
		pGuideBookLDT	=	(*mIter).second;

		if( pGuideBookLDT != NULL )
		{
			if( pGuideBookLDT->m_eGroup == m_eType )
			{
				Iter	=	pGuideBookLDT->m_vRequireBuffer.begin();

				while( Iter != pGuideBookLDT->m_vRequireBuffer.end() )
				{
					bCheckResult	=	CheckRequire( (*Iter) );

					if( bCheckResult == false )
						break;

					++Iter;
				}

				if( bCheckResult == true )
				{
					SetTreeBoxItem( iStartItemPos + iUpdateCount , pGuideBookLDT );
					++iUpdateCount;
				}
			}
		}

		++mIter;
	}

	return iUpdateCount;
}

bool SPWindowGuideBook::CheckRequire( SPRequire& stRequire )
{
	return g_pCheckManager->CheckCondition( stRequire.m_iRequireStat , stRequire.m_iRequireStatValue , stRequire.m_iValue1 , stRequire.m_iValue2 );
}

void SPWindowGuideBook::SetType( GUIDE_BOOK_TYPE eType )
{
	m_eType	=	eType;
}

GUIDE_BOOK_TYPE SPWindowGuideBook::GetType()
{
	return m_eType;
}

void SPWindowGuideBook::SetTypeButton( GUIDE_BOOK_TYPE eType )
{
	SPWindow*	pWindow	=	NULL;
	bool		bSelect	=	false;

	for( int i = 0 ; i < GUIDE_BOOK_TYPE_COUNT ; ++i )
	{
		pWindow	=	Find( WIID_GUIDE_BOOK_TYPE + i );

		if( pWindow != NULL )
		{
			if( i == (int)eType )
				bSelect	=	true;
			else
				bSelect	=	false;

			pWindow->SPSendMessage( SPIM_BTN_SELECT , bSelect );
		}
	}
}

void SPWindowGuideBook::SetSubject( const char* pszSubject )
{
	SPWindow*	pWindow	=	Find( WIID_GUIDE_BOOK_SUBJECT );

	if( pWindow == NULL )
		return;

	pWindow->SetWindowText( pszSubject );
}

void SPWindowGuideBook::SetContents( const char* pszContents )
{
	SPWindow*	pWindow	=	Find( WIID_GUIDE_BOOK_CONTENTS );

	if( pWindow == NULL )
		return;

	pWindow->SetWindowText( pszContents );
}
