/********************************************************************
    Copyright (C) 2006 AJJIYA - All Rights Reserved
	created:	2006/06/29
	created:	2006:6:29:   10:22
	filename: 	SPWindowQuestNotify.cpp
	
	purpose:	
*********************************************************************/

#include "SPCommon.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"

#include "SPTreeBoxDEF.h"
#include "SPTreeBoxItemBase.h"
#include "SPTreeBoxItemQuest.h"
#include "SPWindowTreeBox.h"

#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"
#include "SPResourceManager.h"

#include "SPItemAttr.h"
#include "SPItemCluster.h"

#include "SPCheckManager.h"

#include "SPSkill.h"
#include "SPMotionStatus.h"
#include "SPSkillManager.h"

#include "SPEventDEF.h"
#include "SPEventArchive.h"
#include "SPEventManager.h"

#include "SPUIUnit.h"
#include "SPUIGOBUnit.h"

#include "SPQuestManager.h"
#include "SPQuestArchive.h"
#include "SPQuestAttr.h"

#include "SPWindowQuestNotify.h"

//////////////////////////////////////////////////////////////////////////

#define QUESTNOTIFY_DEFAULT_HEIGHT	8
#define QUESTNOTIFY_ITEM_HEIGHT		19
#define QUESTNOTIFY_ITEM_MAX_LINT	15

//////////////////////////////////////////////////////////////////////////

SPWindowQuestNotify::SPWindowQuestNotify(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
{
	Init();
}

SPWindowQuestNotify::SPWindowQuestNotify(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_QUESTNOTIFY, InstanceID, iX, iY, iCX, iCY, pParent)	
{
	Init();
}

SPWindowQuestNotify::~SPWindowQuestNotify()
{
	Clean();
}

void SPWindowQuestNotify::Init()
{
	m_strToolTip	= "[SPWindowQuestNotify]";

	SPWindowTreeBox*	pTreeBox	=	new SPWindowTreeBox( WIID_QUESTNOTIFY_TREE_LIST , 0 , 0 , m_iSX , 198 , this );
	pTreeBox->SetBoxTexture( "DATA/INTERFACE/CONCEPT/UI101.PNG" , 1 , 175 , 13 , 187 , 4 );
	pTreeBox->SetMargin( 0 , 4 , 0 , 4 );
	pTreeBox->UpdateBGPos();

	//pTreeBox->SetScrollUpSize( 12 , 11 );
	//pTreeBox->SetScrollUpTexture( DATA_TEXTURE_ENABLE		, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 460 , 458 );
	//pTreeBox->SetScrollUpTexture( DATA_TEXTURE_HIT			, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 473 , 458 );
	//pTreeBox->SetScrollUpTexture( DATA_TEXTURE_PUSH			, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 486 , 458 );
	//pTreeBox->SetScrollUpTexture( DATA_TEXTURE_DISABLE		, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 499 , 458 );

	//pTreeBox->SetScrollDownSize( 12 , 11 );
	//pTreeBox->SetScrollDownTexture( DATA_TEXTURE_ENABLE		, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 460 , 500 );
	//pTreeBox->SetScrollDownTexture( DATA_TEXTURE_HIT		, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 473 , 500 );
	//pTreeBox->SetScrollDownTexture( DATA_TEXTURE_PUSH		, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 486 , 500 );
	//pTreeBox->SetScrollDownTexture( DATA_TEXTURE_DISABLE	, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 499 , 500 );

	//pTreeBox->SetScrollSliderSize( 12 , 24 );
	//pTreeBox->SetScrollSliderTexture( DATA_TEXTURE_ENABLE	, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 460 , 470 );
	//pTreeBox->SetScrollSliderTexture( DATA_TEXTURE_HIT		, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 473 , 470 );
	//pTreeBox->SetScrollSliderTexture( DATA_TEXTURE_PUSH		, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 486 , 470 );
	//pTreeBox->SetScrollSliderTexture( DATA_TEXTURE_DISABLE	, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 499 , 470 );

	//pTreeBox->SetScrollSliderBGTexture( DATA_TEXTURE_ENABLE	, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 460 , 496 , 472 , 498 );
	//pTreeBox->SetScrollSliderBGTexture( DATA_TEXTURE_HIT	, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 473 , 496 , 485 , 498 );
	//pTreeBox->SetScrollSliderBGTexture( DATA_TEXTURE_PUSH	, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 486 , 496 , 498 , 498 );
	//pTreeBox->SetScrollSliderBGTexture( DATA_TEXTURE_DISABLE, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 499 , 496 , 511 , 498 );

	//pTreeBox->SetScrollMargin( 0 , 0 , 0 , 0 );
	//pTreeBox->UpdateScrollPos();

	pTreeBox->Refresh();

	ResetCategoryItem();

	m_cStringColor[ 0 ]		=	D3DCOLOR_ARGB( 255 , 255 , 221 , 138 );
	m_cStringColor[ 1 ]		=	D3DCOLOR_ARGB( 255 , 122 , 101 , 50 );

	const char*	pszMission	=	g_pResourceManager->GetGlobalString( 6001007 );

	if( pszMission == NULL )
		m_strMissionView	=	"[ ????? ]";
	else
		m_strMissionView	=	pszMission;
}

void SPWindowQuestNotify::Clean()
{
	SPWindow::Clean();

	ResetCategoryItem();
}

void SPWindowQuestNotify::Show( bool bWithChild /* = true  */ )
{
	SPWindowTreeBox*	pTreeBox	=	(SPWindowTreeBox*)( Find( WIID_QUESTNOTIFY_TREE_LIST ) );

	if( pTreeBox != NULL )
	{
		if( pTreeBox->IsEmpty() == true )
			return;
	}

	SPWindow::Show( bWithChild );
}

void SPWindowQuestNotify::Hide(bool bSendServer)
{
	if( m_bShow )
		g_pInterfaceManager->RegisterSoundUnit("DATA/SOUND/SYSTEM_CLOSE_WINDOW.WAV");

	SPWindow::Hide(bSendServer);

	int	iCursorX	=	g_pInterfaceManager->GetCursorX();
	int	iCursorY	=	g_pInterfaceManager->GetCursorY();

	if( IsCursorIn( iCursorX , iCursorY ) == TRUE )
		OnCursorOut( NULL ,NULL );
}

void SPWindowQuestNotify::Process(float fTime)
{
	if( m_bShow == false ) return;

	SPWindow::Process( fTime );
}

void SPWindowQuestNotify::Render(float fTime)
{
	if( m_bShow == false ) return;

	SPWindow::Render(fTime);

	g_pVideo->Flush();
}

void SPWindowQuestNotify::RefreshRelationCoord()
{
	SPWindow::RefreshRelationCoord();
}

SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowQuestNotify )

	SPIMESSAGE_COMMAND(	SPIM_MOVE_BEGIN			,										OnBeginMove			)	// metalgeni [6/2/2006]
	SPIMESSAGE_COMMAND(	SPIM_MOVE_END			,										OnEndMove			)	// Related to window move
	SPIMESSAGE_COMMAND(	SPIM_MOVE				,										OnMove				)

	SPIMESSAGE_COMMAND(	SPIM_PURGE				,										OnPurge				)
	SPIMESSAGE_COMMAND(	SPIM_QUEST_REFRESH		,										OnRefreshQuest		)

	SPIMESSAGE_COMMAND(	SPIM_QUEST_NOTIFY		,										OnQuestNotify		)

	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP			,	WIID_QUESTNOTIFY_TREE_LIST		,	OnTreeBoxLButtonUp	)

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)

//////////////////////////////////////////////////////////////////////////

int SPWindowQuestNotify::OnBeginMove		( WPARAM wParam , LPARAM lParam) 
{
	return SPWindow::OnBeginMove(wParam, lParam);
}

int SPWindowQuestNotify::OnEndMove		( WPARAM wParam , LPARAM lParam )
{
	return SPWindow::OnEndMove(wParam, lParam);
}

int SPWindowQuestNotify::OnMove			( WPARAM wParam , LPARAM lParam )
{
	return SPWindow::OnMove(wParam, lParam);
}

int SPWindowQuestNotify::OnPurge( WPARAM wParam , LPARAM lParam )
{
	ResetCategoryItem();

	SPWindowTreeBox*	pTreeBox	=	(SPWindowTreeBox*)( Find( WIID_QUESTNOTIFY_TREE_LIST ) );

	if( pTreeBox != NULL )
	{
		pTreeBox->DelItemAll();
	}

	Hide();

	return 1;
}

int SPWindowQuestNotify::OnRefreshQuest( WPARAM wParam , LPARAM lParam )
{
	STD_MAP_TREEBOX_ITEM::iterator	mIter	=	m_mpCategory.begin();

	while( mIter != m_mpCategory.end() )
	{
		UpdateQuest( (*mIter).second );
		++mIter;
	}

	return 1;
}

int SPWindowQuestNotify::OnQuestNotify( WPARAM wParam, LPARAM lParam )
{
	int		iQuestID	=	(int)wParam;
	int		iAddFlag	=	(int)lParam;

	if( iQuestID <= 0 )
		return 0;

	SPQuestAttr*	pQuestAttr	=	GetQuestAttr( iQuestID );

	if( pQuestAttr == NULL )
		return 0;

	SPWindowTreeBox*	pTreeBox	=	(SPWindowTreeBox*)( Find( WIID_QUESTNOTIFY_TREE_LIST ) );

	if( pTreeBox == NULL )
		return 0;

	if( pTreeBox->GetItemCount( true ) > QUESTNOTIFY_ITEM_MAX_LINT && iAddFlag == 1 )
		return 0;

	TREEBOX_ITEM*	pQuestNameItem	=	CreateCategoryItem( iQuestID );

	if( pQuestNameItem == NULL )
		return 0;

	pTreeBox->DelItem( &pQuestNameItem );
	DeleteCategoryItem( iQuestID );

	int	iResult	=	0;

	if( iAddFlag == 1 )
	{
		pQuestNameItem	=	CreateCategoryItem( iQuestID );

		if( pQuestNameItem == NULL )
			return 0;

		SPTreeBoxItemQuest* pQuestNameTreeItem	=	(SPTreeBoxItemQuest*)( pQuestNameItem->m_pItemBase );

		if( pQuestNameTreeItem != NULL )
		{
			pQuestNameTreeItem->SetQuestComplete( true );
			UpdateQuestColor( pQuestNameTreeItem , true );
		}

		std::string				strMission;
		bool					bComplete;
		SPTreeBoxItemQuest*		pMissionItem;

		for( int i = 0 ; i < QUEST_MAX_MISSION ; ++i )
		{
			bComplete	=	GetMissionString( strMission , iQuestID , i );

			if( strMission.empty() == true )
				continue;

			pMissionItem	=	CreateTreeBoxItem( false );

			if( pMissionItem == NULL )
				continue;

			pMissionItem->SetText( strMission.c_str() );
			pMissionItem->SetReturnValue( i );
			pMissionItem->SetQuestComplete( bComplete );
			UpdateQuestColor( pMissionItem , bComplete );

			pTreeBox->AddItem( pQuestNameItem , pMissionItem );

			if( bComplete == false && pQuestNameTreeItem != NULL )
			{
				pQuestNameTreeItem->SetQuestComplete( false );
				UpdateQuestColor( pQuestNameTreeItem , false );
			}
		}

		iResult	=	1;
	}

	m_iSY  = pTreeBox->GetItemCount( true ) * QUESTNOTIFY_ITEM_HEIGHT + QUESTNOTIFY_DEFAULT_HEIGHT;
	pTreeBox->SetRectSize( m_iSX , m_iSY );
	pTreeBox->UpdateBGPos();
	pTreeBox->Refresh();

	if( pTreeBox->IsEmpty() == true )
		Hide();
	else
		Show();

	return iResult;
}

int SPWindowQuestNotify::OnTreeBoxLButtonUp( WPARAM wParam , LPARAM lParam )
{
	return 1;
}

//////////////////////////////////////////////////////////////////////////

SPQuestAttr* SPWindowQuestNotify::GetQuestAttr( int iQuestID )
{
	SPQuestAttr*	pQuestAttr		=	NULL;
	SPQuestManager*	pQuestManager	=	SPQuestManager::GetInstance();

	if( pQuestManager == NULL )
		return pQuestAttr;

	SPQuestArchive*	pPlayerQuestArchive	=	pQuestManager->GetPlayerArchive();

	if( pPlayerQuestArchive == NULL )
		return pQuestAttr;

	pQuestAttr	=	pPlayerQuestArchive->GetQuest( iQuestID );

	return pQuestAttr;
}

void SPWindowQuestNotify::ResetCategoryItem()
{
	m_mpCategory.clear();
}

TREEBOX_ITEM* SPWindowQuestNotify::CreateCategoryItem( int iCategory )
{
	STD_MAP_TREEBOX_ITEM::iterator	mIter	=	m_mpCategory.find( iCategory );

	if( mIter != m_mpCategory.end() )
		return	(*mIter).second;

	SPQuestAttr*	pQuestAttr	=	GetQuestAttr( iCategory );

	if( pQuestAttr == NULL )
		return NULL;

	SPWindowTreeBox*	pTreeBox	=	(SPWindowTreeBox*)( Find( WIID_QUESTNOTIFY_TREE_LIST ) );

	if( pTreeBox == NULL )
		return NULL;

	SPTreeBoxItemQuest*	pQuestItem	=	CreateTreeBoxItem( true );

	if( pQuestItem == NULL )
		return NULL;

	pQuestItem->SetText( pQuestAttr->m_strQuestName.c_str() );
	pQuestItem->SetReturnValue( iCategory );

	TREEBOX_ITEM*	pTreeBoxItem	=	pTreeBox->AddItem( NULL , pQuestItem );

	if( pTreeBoxItem != NULL )
	{
		pTreeBoxItem->m_bSelect	=	true;

		if( pTreeBoxItem->m_pItemBase != NULL )
		{
			pTreeBoxItem->m_pItemBase->AddStatus( DATA_STATUS_PULLDOWN );
			pTreeBoxItem->m_pItemBase->AddStatus( DATA_STATUS_DISABLE );
		}
	}

	m_mpCategory.insert( STD_MAP_TREEBOX_ITEM::value_type( iCategory , pTreeBoxItem ) );

	return pTreeBoxItem;
}

bool SPWindowQuestNotify::DeleteCategoryItem( int iCategory )
{
	STD_MAP_TREEBOX_ITEM::iterator	mIter	=	m_mpCategory.find( iCategory );

	if( mIter != m_mpCategory.end() )
	{
		m_mpCategory.erase( mIter );
		return	true;
	}

	return false;
}

TREEBOX_ITEM* SPWindowQuestNotify::FindQuestItem( int iQuestID , int iMissionID )
{
	TREEBOX_ITEM*	pItem			=	NULL;
	TREEBOX_ITEM*	pCategoryItem	=	CreateCategoryItem( iQuestID );

	if( pCategoryItem == NULL )
		return pItem;

	std::vector< TREEBOX_ITEM* >*			pvpChild		=	&( pCategoryItem->m_vpChild );
	std::vector< TREEBOX_ITEM* >::iterator	Iter			=	pvpChild->begin();
	TREEBOX_ITEM*							pIterItem		=	NULL;
	SPTreeBoxItemQuest*						pIterQuestItem	=	NULL;

	while( Iter != pvpChild->end() )
	{
		pIterItem	=	(*Iter);

		if( pIterItem != NULL )
		{
			pIterQuestItem	=	(SPTreeBoxItemQuest*)( pIterItem->m_pItemBase );

			if( pIterQuestItem != NULL )
			{
				if( pIterQuestItem->GetReturnValue() == iMissionID )
				{
					pItem	=	pIterItem;
					break;
				}

			}
		}
		++Iter;
	}

	return pItem;
}

//SPTreeBoxItemQuest* SPWindowQuestNotify::FindQuestItem( int iQuestID , int iMissionID )
//{
//	SPTreeBoxItemQuest*	pQuestItem = NULL;
//
//	TREEBOX_ITEM*	pQuestTreeItem	=	FindQuestItem( iQuestID ,iMissionID );
//
//	if( pQuestTreeItem == NULL )
//		return pQuestItem;
//
//	pQuestItem	=	(SPTreeBoxItemQuest*)( pQuestTreeItem->m_pItemBase );
//
//	return pQuestItem;
//}

SPTreeBoxItemQuest* SPWindowQuestNotify::CreateTreeBoxItem( bool bMenuType )
{
	SPTreeBoxItemQuest*	pQuestItem	=	new SPTreeBoxItemQuest;

	if( pQuestItem == NULL )
		return pQuestItem;

	pQuestItem->SetWindow( this );

	if( bMenuType == true )
	{
		pQuestItem->SetSize( 157 , 12 );
		pQuestItem->SetTextMargin( 26 , 3 );
	}
	else
	{
		pQuestItem->SetSize( 144 , 12 );
		pQuestItem->SetTextMargin( 39 , 3 );
	}

	for( int j = 0 ; j < DATA_STRING_COUNT ; ++j )
	{
		pQuestItem->SetTextColor( j , m_cStringColor[ 0 ] );
	}

	pQuestItem->SetQuestSize( 187 , QUESTNOTIFY_ITEM_HEIGHT );
	pQuestItem->SetQuestOffset( 0 , 0 );

	if( bMenuType == true )
	{
		pQuestItem->SetTexture( SPTreeBoxItemQuest::QUEST_TEXTURE_LEFT_ENABLE * 2 ,				"DATA/INTERFACE/CONCEPT/UI102.PNG" , 115 , 76 , 133 , 94 );
		pQuestItem->SetTexture( SPTreeBoxItemQuest::QUEST_TEXTURE_LEFT_ENABLE * 2 + 1 ,			"DATA/INTERFACE/CONCEPT/UI102.PNG" , 134 , 76 , 152 , 94 );
	}
	else
	{
		pQuestItem->SetTexture( SPTreeBoxItemQuest::QUEST_TEXTURE_LEFT_ENABLE * 2 ,				"DATA/INTERFACE/CONCEPT/UI102.PNG" , 172 , 76 , 186 , 88 );
		pQuestItem->SetTexture( SPTreeBoxItemQuest::QUEST_TEXTURE_LEFT_ENABLE * 2 + 1 ,			"DATA/INTERFACE/CONCEPT/UI102.PNG" , 187 , 76 , 201 , 88 );
	}

	//////////////////////////////////////////////////////////////////////////

	if( bMenuType == true )
	{
		pQuestItem->SetTextureOffset( SPTreeBoxItemQuest::QUEST_TEXTURE_LEFT_ENABLE * 2 ,			4 , 0 );
		pQuestItem->SetTextureOffset( SPTreeBoxItemQuest::QUEST_TEXTURE_LEFT_ENABLE * 2 + 1 ,		4 , 0 );
	}
	else
	{
		pQuestItem->SetTextureOffset( SPTreeBoxItemQuest::QUEST_TEXTURE_LEFT_ENABLE * 2 ,			21 , 3 );
		pQuestItem->SetTextureOffset( SPTreeBoxItemQuest::QUEST_TEXTURE_LEFT_ENABLE * 2 + 1 ,		21 , 3 );
	}

	//////////////////////////////////////////////////////////////////////////

	return pQuestItem;
}

bool SPWindowQuestNotify::GetMissionString( std::string& strMission , int iQuestID , int iMissionID )
{
	strMission.clear();

	SPQuestAttr*	pQuestAttr	=	GetQuestAttr( iQuestID );

	if( pQuestAttr == NULL )
		return false;

	//	목적
	SPItemAttr*				pItemAttr;
	SPUIGOBUnit				UIGOBUnit;
	SPSkillManager*			pSkillManager		=	SPSkillManager::GetInstance();
	SPActionInterface*		pActionInterface	=	NULL;
	SPEventArchive*			pEventArchive		=	g_pEventManager->GetEventArchive();
	char					szBuf[ _MAX_PATH ];

	SPMission*				pMission	=	&pQuestAttr->m_Mission[ iMissionID ];
	std::string				strName;

	strName.clear();

	// 미션 Y/n에 따라 처리한다. [1/8/2007 AJJIYA]
	if( pMission->m_bShow == true )
	{
		if( pMission->m_iMissionType == MISSION_TYPE_ITEM )
		{
			if( g_pItemCluster->GetItemInfo( pMission->m_iMissionTypeID , pItemAttr ) == true )
			{
				strName	=	pItemAttr->m_strName.c_str();
			}
		}
		else if( pMission->m_iMissionType == MISSION_TYPE_MONSTER )
		{
			UIGOBUnit.SetClassID( (CLASS_ID)pMission->m_iMissionTypeID );
			UIGOBUnit.ReResourceLoad();

			strName	=	UIGOBUnit.GetName();
		}
		else if( pMission->m_iMissionType == MISSION_TYPE_ACTION )
		{
			pActionInterface	=	pSkillManager->GetActionInterface( pMission->m_iMissionTypeID );

			if( pActionInterface != NULL )
			{
				strName	=	pActionInterface->GetName();
			}
		}
		else if( pMission->m_iMissionType == MISSION_TYPE_EVENT )
		{
			if( pEventArchive != NULL )
			{
				MAPEVENT_INFO*	pMapEventInfo	=	pEventArchive->GetEvent( pMission->m_iMissionTypeID );

				if( pMapEventInfo != NULL )
				{
					strName	=	pMapEventInfo->name;
				}
			}
		}
	}
	else
	{
		if( pMission->m_iMissionType != MISSION_TYPE_NULL )
			strName	=	m_strMissionView;
	}

	bool	bComplete	=	false;

	if( pMission->m_bClear == true && pMission->m_iCurCount >= pMission->m_iMissionCount )
		bComplete	=	true;

	if( strName.empty() == false )
	{
		if( pMission->m_bShow == true )
			sprintf( szBuf , "%s (%d/%d)" , strName.c_str() , pMission->m_iCurCount , pMission->m_iMissionCount );
		else
			sprintf( szBuf , "%s" , strName.c_str() );

		strMission	=	szBuf;
	}

	return bComplete;
}

void SPWindowQuestNotify::UpdateQuest( TREEBOX_ITEM* pItem )
{
	if( pItem == NULL )
		return;

	SPTreeBoxItemQuest*	pQuestNameItem	=	(SPTreeBoxItemQuest*)( pItem->m_pItemBase );

	if( pQuestNameItem == NULL )
		return;

	int	iQuestID	=	pQuestNameItem->GetReturnValue();

	SPQuestAttr*	pQuestAttr = GetQuestAttr( iQuestID );

	if( pQuestAttr == NULL )
		return;

	pQuestNameItem->SetQuestComplete( true );
	UpdateQuestColor( pQuestNameItem , true );

	std::string				strMission;
	bool					bComplete;
	SPTreeBoxItemQuest*		pMissionItem;
	TREEBOX_ITEM*			pQuestMission;

	int	iMaxCount	=	(int)pItem->m_vpChild.size();

	for( int i = 0 ; i < iMaxCount ; ++i )
	{
		bComplete	=	GetMissionString( strMission , iQuestID , i );

		if( strMission.empty() == true )
			continue;

		pQuestMission	=	pItem->m_vpChild.at( i );

		if( pQuestMission == NULL )
			continue;

		pMissionItem	=	(SPTreeBoxItemQuest*)( pQuestMission->m_pItemBase );

		if( pMissionItem == NULL )
			continue;

		pMissionItem->SetText( strMission.c_str() );
		pMissionItem->SetReturnValue( i );
		pMissionItem->SetQuestComplete( bComplete );
		UpdateQuestColor( pMissionItem , bComplete );

		if( bComplete == false && pQuestNameItem != NULL )
		{
			pQuestNameItem->SetQuestComplete( false );
			UpdateQuestColor( pQuestNameItem , false );
		}
	}
}

void SPWindowQuestNotify::UpdateQuestColor( SPTreeBoxItemQuest* pQuestItem , bool bComplete )
{
	if( pQuestItem == NULL )
		return;

	int	iColor	=	0;

	if( bComplete == true )
		iColor	=	1;

	for( int j = 0 ; j < DATA_STRING_COUNT ; ++j )
	{
		pQuestItem->SetTextColor( j , m_cStringColor[ iColor ] );
	}
}
