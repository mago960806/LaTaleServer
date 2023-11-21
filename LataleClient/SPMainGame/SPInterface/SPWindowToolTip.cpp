
#include <vector>
#include <string>
#include <shlwapi.h>

#include "SPCommon.h"
#include "SPUtil.h"

#include "SPMainGameDEF.H"
#include "SPGameObjectDEF.h"
#include "SPResourceDef.h"

#include "SPSubGameManager.h"
#include "SPGameObject.h"
#include "SPManager.h"
#include "SPMouseCursor.h"
#include "SPIMEPool.h"
#include "SPWindowDEF.h"
#include "SPInterfaceManager.h"

#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGOBStatus.h"
#include "SPPlayerStatus.h"
#include "SPPlayerStatusModel.h"
#include "SPGOBCoordPhysics.h"
#include "SPPlayerCoordPhysics.h"
#include "SPPlayer.h"
#include "SPGOBManager.h"
#include "SPResourceManager.h"
#include "SPCheckManager.h"

#include "SPMonsterModelUnitDef.h"
#include "SPMOBCluster.h"

#include "SPEventDEF.h"
#include "SPEventManager.h"

#include "SPQuestManager.h"
#include "SPQuestArchive.h"
#include "SPQuestAttr.h"

#include "SPItemAttr.h"
#include "SPItemStatus.h"
//#include "SPItem.h"
#include "SPItemCluster.h"			//[2005/6/16] - jinhee

#include "SPEventNpcHandler.h"

#include "SPUIUnit.h"				// AJJIYA [7/13/2005]
#include "SPUISkillUnit.h"			// AJJIYA [7/13/2005]
#include "SPUIGOBUnit.h"			// AJJIYA [10/6/2005]
#include "SPUIQuestUnit.h"			// AJJIYA [10/6/2005]
#include "SPUIBuffUnit.h"			// AJJIYA [10/11/2005]
#include "SPUIItemUnit.h"
#include "SPUIUnitManager.h"

#include "SPSkill.h"				// AJJIYA [7/13/2005]
#include "SPSkillManager.h"			// AJJIYA [7/13/2005]
#include "SPMotionStatus.h"			// AJJIYA [7/13/2005]
#include "SPSkillArchive.h"

#include "SPItem.h"
#include "SPPlayerInvenArchive.h"

#include "SPEffect.h"
#include "SPEffectArchive.h"
#include "SPEffectManager.h"

#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowLottery.h"
#include "SPWindowNpcEvent.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"
#include "SPResourceDef.h"

#include "SPWindowStatus.h"			// 툴팁 특별처리 Instance ID [2/28/2006 AJJIYA]

#ifdef _DEBUG
	#include "SPDebug.h"
#endif

#include "SPTitleAttr.h"
#include "SPTitleArchive.h"
#include "SPTitleManager.h"

#include "SPPvpManager.h"

#include "SPCoolTimeManager.h"

#include "PacketID.h"
#include "GlobalDefines_Share.h"

#include "SPWindowToolTip.h"

//////////////////////////////////////////////////////////////////////////

SPWindowToolTip::SPWindowToolTip(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_TOOLTIP, InstanceID, iX, iY, iCX, iCY, pParent)	
{	
	Init();
}

SPWindowToolTip::~SPWindowToolTip()
{
	Clean();
}

void SPWindowToolTip::Init()
{
	m_eType	=	TOOLTIP_SHOW_TYPE_NULL;

	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI102.PNG" , & m_pBGTexture );

	m_iStep	=	8;

	SetBGRect( m_rcBGSrc , 252 , 76 , 276 , 100 , m_iStep );

	m_rcMargin.left		=	8;
	m_rcMargin.top		=	10;
	m_rcMargin.right	=	8;
	m_rcMargin.bottom	=	10;

	int	iXPos	=	m_rcMargin.left;
	int	iYPos	=	m_rcMargin.top;

	int	iXSize	=	m_iSX	-	m_rcMargin.left	-	m_rcMargin.right;
	int	iYSize	=	19;

	m_pTitle	=	new SPWindowStatic( WIID_TOOLTIP_TITLE , iXPos , iYPos , iXSize , iYSize , this );
	m_pTitle->SetFontShadow( false );
	m_pTitle->SetFont( FONT_14_BOLD );
	m_pTitle->SetFormat( DT_CENTER | DT_VCENTER );
	m_pTitle->SetTextAutoWidthMin( iXSize );
	m_pTitle->SetTextPeriod( false );

	iYPos	+=	iYSize;
	iYSize	=	15;

	for( int i = 0 ; i < MAX_TOOLTIP_CONTENTS_LINE ; ++i )
	{
		m_pContents[ i ]	=	new	SPWindowStatic( WIID_TOOLTIP_CONTENTS + i , iXPos , iYPos , iXSize , iYSize , this );
		m_pContents[ i ]->SetFontShadow( false );
		m_pContents[ i ]->SetFormat( DT_LEFT | DT_TOP );
		m_pContents[ i ]->SetTextAutoWidthMin( iXSize );
		m_pContents[ i ]->SetTextPeriod( false );

		iYPos	+=	iYSize;
	}

	m_pContents[ 0 ]->SetFormat( DT_CENTER | DT_TOP );

	iYSize	=	15 * 4;

	m_pDesc		=	new	SPWindowStatic( WIID_TOOLTIP_DESC , iXPos , iYPos , iXSize , iYSize , this );
	m_pDesc->SetFontShadow( false );
	m_pDesc->SetMultiLine( true ,1 );
	m_pDesc->SetFormat( DT_LEFT | DT_TOP | DT_WORDBREAK );
	m_pDesc->SetTextPeriod( false );

	//////////////////////////////////////////////////////////////////////////

	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI104.PNG" , & m_pBGTexture2 );

	m_iStep2			=	8;

	m_rcMargin2.left	=	13;
	m_rcMargin2.top		=	14;
	m_rcMargin2.right	=	11;
	m_rcMargin2.bottom	=	12;

	m_iSX2				=	211;

	SetBGRect( m_rcBGSrc2 , 213 , 255 , 233 , 275 , m_iStep2 );

	iXPos	=	m_iSX	+	m_rcMargin2.left;
	iYPos	=	m_rcMargin2.top;

	iXSize	=	m_iSX2	-	m_rcMargin2.left	-	m_rcMargin2.right;
	iYSize	=	15;

	m_pTitle2	=	new SPWindowStatic( WIID_TOOLTIP_TITLE2 , iXPos , iYPos , iXSize , iYSize , this );
	m_pTitle2->SetFontShadow( false );
	m_pTitle2->SetFont( FONT_14_BOLD );
	m_pTitle2->SetFormat( DT_CENTER | DT_VCENTER );
	m_pTitle2->SetTextAutoWidthMin( iXSize );
	m_pTitle2->SetTextPeriod( false );

	iYPos	+=	iYSize;
	iYPos	+=	19;
	iYSize	=	11;

	for( i = 0 ; i < MAX_TOOLTIP_CONTENTS_LINE ; ++i )
	{
		m_pContents2[ i ]	=	new	SPWindowStatic( WIID_TOOLTIP_CONTENTS2 + i , iXPos , iYPos , iXSize , iYSize , this );
		m_pContents2[ i ]->SetFontShadow( false );
		m_pContents2[ i ]->SetFormat( DT_LEFT | DT_TOP );
		m_pContents2[ i ]->SetTextAutoWidthMin( iXSize );
		m_pContents2[ i ]->SetTextPeriod( false );

		iYPos	+=	iYSize;
		iYPos	+=	3;
	}

	SPWindow*	pWindow	=	new	SPWindow( WNDID_CTRL_STATIC , WIID_TOOLTIP_ARROW , -21 + m_iSX , 16 , 21 , 41 , this );
	pWindow->SetImage( "DATA/INTERFACE/CONCEPT/UI106.PNG" , 279 , 243 );

	//////////////////////////////////////////////////////////////////////////

	m_cColors[ TCC_DISABLE ]						=	D3DCOLOR_ARGB(	255	,	255	,	0	,	0	);
	m_cColors[ TCC_BLACK ]							=	D3DCOLOR_ARGB(	255	,	0	,	0	,	0	);
	m_cColors[ TCC_WHITE ]							=	D3DCOLOR_ARGB(	255	,	255	,	255	,	255	);
	m_cColors[ TCC_RED ]							=	D3DCOLOR_ARGB(	255	,	255	,	0	,	0	);
	m_cColors[ TCC_BLUE ]							=	D3DCOLOR_ARGB(	255	,	0	,	0	,	255	);

	m_cColors[ TCC_REQUIRE ]						=	D3DCOLOR_ARGB(	255	,	180	,	43	,	69	);
	m_cColors[ TCC_ITEM_OPT ]						=	D3DCOLOR_ARGB(	255	,	0	,	0	,	0	);
	m_cColors[ TCC_ITEM_OPTION_LIMIT ]				=	D3DCOLOR_ARGB(	255	,	0	,	0	,	0	);
	m_cColors[ TCC_ITEM_EFFECT ]					=	D3DCOLOR_ARGB(	255	,	0	,	0	,	0	);
	m_cColors[ TCC_ITEM_OPTION ]					=	D3DCOLOR_ARGB(	255	,	40	,	62	,	135	);
	m_cColors[ TCC_ITEM_OPTION_ENCHANT ]			=	D3DCOLOR_ARGB(	255	,	154	,	66	,	176	);
	m_cColors[ TCC_ITEM_TIME ]						=	D3DCOLOR_ARGB(	255	,	210	,	148	,	73	);
	m_cColors[ TCC_ITEM_COOLTIME ]					=	D3DCOLOR_ARGB(	255	,	210	,	103	,	74	);

	m_cColors[ TCC_ITEM_UPGRADE_REQUIRE ]			=	D3DCOLOR_ARGB(	255	,	18	,	138	,	125	);
	m_cColors[ TCC_ITEM_UPGRADE_EFFECT ]			=	D3DCOLOR_ARGB(	255	,	70	,	73	,	255	);

	m_cColors[ TCC_ITEM_ENCHANT_REQUIRE ]			=	D3DCOLOR_ARGB(	255	,	18	,	138	,	125	);
	m_cColors[ TCC_ITEM_ENCHANT_OPTION ]			=	D3DCOLOR_ARGB(	255	,	70	,	73	,	255	);

	m_cColors[ TCC_SKILL_NOT_LEARN_TITLE ]			=	D3DCOLOR_ARGB(	255	,	156	,	156	,	156	);
	m_cColors[ TCC_SKILL_NOT_LEARN_CONTENTS ]		=	D3DCOLOR_ARGB(	255	,	156	,	156	,	156	);

	m_cColors[ TCC_SKILL_LEARN_REQUIRE ]			=	D3DCOLOR_ARGB(	255	,	18	,	138	,	125	);

	m_cColors[ TCC_ITEM_SET_OPTION_DISABLE ]		=	D3DCOLOR_ARGB(	255	,	153	,	153	,	153	);
	m_cColors[ TCC_ITEM_SET_OPTION_ENABLE ]			=	D3DCOLOR_ARGB(	255	,	40	,	62	,	135	);
	m_cColors[ TCC_ITEM_SET_FULL_OPTION_DISABLE ]	=	D3DCOLOR_ARGB(	255	,	153	,	153	,	153	);
	m_cColors[ TCC_ITEM_SET_FULL_OPTION_ENABLE ]	=	D3DCOLOR_ARGB(	255	,	154	,	65	,	176	);

	m_cColors[ TCC_ITEM_MIX_OPT ]					=	D3DCOLOR_ARGB(	255	,	44	,	143	,	53	);

	//////////////////////////////////////////////////////////////////////////

	m_pItem	=	new	SPItem( STORAGE , 1 );

	m_iOldToolTipID		=	-1;

	LoadToolTipLDT();

	//////////////////////////////////////////////////////////////////////////
	
	ClearContents();

	m_bShowToolTipID	=	false;
}

void SPWindowToolTip::Clean()
{
	SPWindow::Clean();

	SAFE_DELETE( m_pItem );

	SAFE_RELEASE( m_pBGTexture );
	SAFE_RELEASE( m_pBGTexture2 );
}

void SPWindowToolTip::Process(float fTime)
{
	SPWindow::Process(fTime);

	m_bShow	=	g_pMouseCursor->IsShowCursor();
}

void SPWindowToolTip::Render(float fTime)
{
	if( m_strName.empty() || m_bShow == false || m_eType == TOOLTIP_SHOW_TYPE_NULL ) return;

	int i;

	g_pVideo->Flush();

	if( m_pBGTexture != NULL )
	{
		for( i = 0 ; i < TOOLTIP_BG_COUNT ; ++i )
		{
			m_pBGTexture->RenderTexture( &m_rcBGDest[ i ] , &m_rcBGSrc[ i ] );
		}
		g_pVideo->Flush();
	}

	if( m_pBGTexture2 != NULL && m_iWriteContentPos2 > 0 )
	{
		for( i = 0 ; i < TOOLTIP_BG_COUNT ; ++i )
		{
			m_pBGTexture2->RenderTexture( &m_rcBGDest2[ i ] , &m_rcBGSrc2[ i ] );
		}
		g_pVideo->Flush();
	}

	SPWindow::Render( fTime );

	g_pVideo->Flush();
}

void SPWindowToolTip::Show( bool bWithChild /* = true */ )
{
	SPWindow::Show( bWithChild );

	SPWindow*	pWindow	=	Find( WIID_TOOLTIP_ARROW );

	if( pWindow != NULL )
	{
		if( m_iWriteContentPos2 <= 0 )
			pWindow->Hide();
		else
			pWindow->Show();
	}
}

SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowToolTip )
	SPIMESSAGE_COMMAND(SPIM_TOOLTIP_ENTER,	OnToolTipEnter	)
	SPIMESSAGE_COMMAND(SPIM_TOOLTIP_OUT,	OnToolTipOut	)
	SPIMESSAGE_COMMAND(SPIM_TOOLTIP_MOVE,	OnToolTipMove	)
	SPIMESSAGE_COMMAND(SPIM_REFRESH,		OnRefresh		)
	SPIMESSAGE_COMMAND(SPIM_POS_UPDATE,		OnPosUpdate		)
SPIMESSAGE_OBJ_MAP_END	( SPWindow	)

int SPWindowToolTip::OnToolTipEnter	( WPARAM wparam, LPARAM lparam )
{
	//DXUTOutputDebugString( "SPWindowToolTip::OnToolTipEnter\n" );
	OnToolTipShow( wparam , lparam );
	return 1;
}

int SPWindowToolTip::OnToolTipOut	( WPARAM wparam, LPARAM lparam )
{
	//DXUTOutputDebugString( "SPWindowToolTip::OnToolTipOut\n" );
	m_strName.clear();
	m_bShow	=	false;
	return 1;
}

int SPWindowToolTip::OnToolTipMove	( WPARAM wparam, LPARAM lparam )
{
	//DXUTOutputDebugString( "SPWindowToolTip::OnToolTipMove\n" );
	OnToolTipShow( wparam , lparam );
	return 1;
}

int SPWindowToolTip::OnRefresh( WPARAM wparam, LPARAM lparam )
{
	int	iVal	=	(int)wparam;

	if( iVal == 0 )
		m_bShowToolTipID	=	false;
	else
		m_bShowToolTipID	=	true;

	return 1;
}

int SPWindowToolTip::OnPosUpdate(WPARAM, LPARAM )
{
	UpdatePos();
	CalBGSize();

	return 1;
}

void SPWindowToolTip::OnToolTipShow( WPARAM wParam , LPARAM lParam )
{
	m_bShow	=	g_pMouseCursor->IsShowCursor();

	if( m_bShow == false )
		return;

	//	Skill , QuickSlot에서 툴팁 사용
	//	AJJIYA [7/7/2005]
	m_eType	=	(TOOLTIP_SHOW_TYPE)lParam;

	//	스킬창 , 퀘스트창 , 큇슬롯 창에서 중복 툴팁 발생을 스킵한다.
	//	아이템창 , 장비창 같은곳은 스킵하지 않는다. 아이템 옵션 때문에.
	if( m_eType == TOOLTIP_SHOW_TYPE_UIUNIT )
	{
		int			iOldToolTipID	=	-2;
		INSTANCE_ID	eOldToolTipID	=	0;

		SPUISkillUnit*	pUISkill	=	(SPUISkillUnit*)wParam;

		if( pUISkill != NULL )
			iOldToolTipID	=	pUISkill->GetSkillID();

		SPUIItemUnit*	pUIItem		=	(SPUIItemUnit*)wParam;

		if( pUIItem != NULL )
			iOldToolTipID	=	pUIItem->GetItemID();

		SPUIQuestUnit*	pUIQuest	=	(SPUIQuestUnit*)wParam;

		if( pUIQuest != NULL )
			iOldToolTipID	=	pUIQuest->GetQuestID();

		SPUIUnit*	pUIUnit			=	(SPUIUnit*)wParam;

		if( pUIUnit != NULL )
			eOldToolTipID	=	pUIUnit->GetInstanceID();
		
		if( m_iOldToolTipID == iOldToolTipID && m_eOldToolTipID == eOldToolTipID && iOldToolTipID != -2 && m_strName.empty() == false )
		{
			return;
		}

		m_iOldToolTipID	=	iOldToolTipID;
		m_eOldToolTipID	=	eOldToolTipID;
	}

	Show();

	if( m_eType != TOOLTIP_SHOW_TYPE_CTRL )
	{
		ClearContents();
	}

	switch( m_eType )
	{
	case TOOLTIP_SHOW_TYPE_UIUNIT:			//	Skill , Buf , Quest
		ShowToolTip( (SPUIUnit*)wParam );
		break;
	case TOOLTIP_SHOW_TYPE_CTRL:			//	일반 컨트롤
		CreateCtrl( (INSTANCE_ID)wParam );
	    break;
	case TOOLTIP_SHOW_TYPE_TITLE:			//	타이틀
		CreateTitle( (UINT32)wParam );
		break;
	case TOOLTIP_SHOW_TYPE_CASHITEM:		//	캐시아이템
		CreateCashItem( (SPCashItemAttr*)wParam );
		break;
	case TOOLTIP_SHOW_TYPE_ENCHANT:			//	인챈트
		CreateEnchant( (SPItemEnchantResult*)wParam );
		break;
	case TOOLTIP_SHOW_TYPE_ITEMUPGRADE:		//	아이템업그레이드
		CreateItemUpgrade( (SPItemStatus*)wParam );
		break;
	case TOOLTIP_SHOW_TYPE_SKILL_LVUP:		//	스킬 레벨업 버튼
		CreateSkillLVUP( (SPUIUnit*)wParam );
		break;
	case TOOLTIP_SHOW_TYPE_FRIEND_WHITE:	//	친구 목록
		CreateFriendWhite( (FRIEND_INFO*)wParam );
		break;
	case TOOLTIP_SHOW_TYPE_LOTTERY:			//	뽑기 아이템
		CreateLottery( wParam );
		break;
	case TOOLTIP_SHOW_TYPE_NPC_EVENT:		//	NPC 이벤트
		CreateNpcEvent( wParam );
		break;
	case TOOLTIP_SHOW_TYPE_NORMAL:			//	Normal
		CreateNormal( (TOOLTIP_NORMAL_DATA*)wParam );
		break;
	case TOOLTIP_SHOW_TYPE_ITEMMIX:			//	Normal
		CreateItemMix( (SPItemMix*)wParam );
		break;
	case TOOLTIP_SHOW_TYPE_PUSHITEM:
		CreatePushItem( (SPCashItemAttr*)wParam );
		break;
	default:	//	Item
		if( m_eType >= TOOLTIP_SHOW_TYPE_NULL )
			m_eType	=	TOOLTIP_SHOW_TYPE_ITEM;

		CreateItemData( (SPItemStatus*)wParam );
		break;
	}

	if( IsShow() == false )
		return;

	UpdatePos();
	CalAutoWidth();
	CalDescPos();
	CalBGSize();

	//////////////////////////////////////////////////////////////////////////

	SPWindow*	pWindow	=	Find( WIID_TOOLTIP_ARROW );

	if( pWindow != NULL )
	{
		if( m_iWriteContentPos2 <= 0 )
			pWindow->Hide();
		else
			pWindow->Show();
	}
}

void SPWindowToolTip::UpdatePos()
{
	int	iSX	=	m_iSX;
	int	iSY	=	m_iSY;

	if( m_iWriteContentPos2 > 0 )
	{
		iSX	+=	m_iSX2;

		if( iSY < m_iSY2 )
            iSY	=	m_iSY2;
	}

	SIZE ptMouseSize	= g_pMouseCursor->GetMouseSize();
	int iXPos			= g_pInterfaceManager->GetCursorX();
	int iYPos			= g_pInterfaceManager->GetCursorY() + ptMouseSize.cy;
	int	iXScreenSize	= (int)g_pVideo->GetScreenLenX();
	int iYScreenSize	= (int)g_pVideo->GetScreenLenY();

	
	//  마우스 X 위치가 화면 오른쪽으로 툴팁이 잘릴경우  [6/30/2006]

	if( iXPos + iSX >= iXScreenSize )
	{
		iXPos	=	iXPos - iSX;
	}

	//	마우스 Y 위치가 화면 아래쪽으로 툴팁이 잘릴경우  [6/30/2006]

	if( iYPos + ptMouseSize.cy + iSY >= iYScreenSize )
	{
		iYPos	=	iYPos - ptMouseSize.cy - iSY;
	}

	if( iXPos < 0 )
		iXPos	=	0;

	if( iYPos < 0 )
		iYPos	=	0;

	AdjustCoord( iXPos , iYPos );
}

void SPWindowToolTip::CreateCtrl( INSTANCE_ID InstanceID )
{
	if( InstanceID == 0 )
	{
		Hide();
		return;
	}

	if( m_strName.empty() == false )
		return;

	ClearContents();

	//	AJJIYA [7/6/2006]
	//	인스턴스 아이디 출력
	if( m_bShowToolTipID == true )
	{
		m_strName = "Ctrl";
		char		szBufInstanceID[ _MAX_PATH ]	=	{};
		sprintf( szBufInstanceID , "Instance ID : %d" , InstanceID );

		m_pTitle->SetWindowText( szBufInstanceID );
		m_pTitle->SetFontColor( m_cColors[ TCC_BLACK ] );
		SetContents( TCT_STRING , szBufInstanceID ,	m_cColors[ TCC_BLACK ] );

		SetDesc( NULL );
	}

	STD_MAP_TOOLTIP::iterator	mIter	=	m_mToolTip.find( InstanceID );

	if( mIter == m_mToolTip.end() )
		return;

	TOOLTIP_DATA*	pToolTipData	=	&( (*mIter).second );
	CUT_IN_STATE	eCutInState		=	g_pEventManager->GetCutInState();

	if( pToolTipData->m_bCutInCheck == false && eCutInState != CUT_IN_STATE_NULL )
		return;

	m_strName = "Ctrl";
	m_pTitle->SetWindowText( pToolTipData->m_strName.c_str() );
	m_pTitle->SetFontColor( m_cColors[ TCC_BLACK ] );

	// 툴팁 표시중 정보를 더 표시한다. [2/28/2006 AJJIYA]

	//char			szBuf[ _MAX_PATH * 2 ];
	//SPPlayer*		pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();
	//SPGOBStatus*	pPlayerStatus	=	NULL;
	//INT64			iValue			=	0;
	//float			fValue			=	0.0f;

	//if( pLocalPlayer != NULL )
	//	pPlayerStatus	=	pLocalPlayer->GetStatus();

	//switch( InstanceID )
	//{
	////case WIID_STATUS_WABIL_ATK_RESULT:
	////	if( pPlayerStatus != NULL )
	////	{
	////		iValue = pPlayerStatus->GetStatusValue( STATUS_ATK );
	////		fValue = (float)iValue;
	////		sprintf( szBuf , pToolTipData->m_strDesc.c_str() , fValue );
	////		SetWindowDescText( szBuf );
	////	}
	////	break;
	////case WIID_STATUS_WABIL_DEF_RESULT:
	////	if( pPlayerStatus != NULL )
	////	{
	////		iValue = pPlayerStatus->GetStatusValue( STATUS_DEF );
	////		fValue = (float)( ( (float)iValue / ( iValue + 100 ) ) * 100.0f );
	////		sprintf( szBuf , pToolTipData->m_strDesc.c_str() , fValue );
	////		SetWindowDescText( szBuf );
	////	}
	////	break;
	////case WIID_STATUS_WABIL_MGC_RESULT:
	////	if( pPlayerStatus != NULL )
	////	{
	////		iValue = pPlayerStatus->GetStatusValue( STATUS_ELE );
	////		fValue = (float)iValue;
	////		sprintf( szBuf , pToolTipData->m_strDesc.c_str() , fValue );
	////		SetWindowDescText( szBuf );
	////	}
	////	break;
	//default:
	//	//	설명
	//    break;
	//}

	SetDesc( pToolTipData->m_strDesc.c_str() );
}

void SPWindowToolTip::CreateItemData( SPItemStatus* pItemStatus )
{
	if( pItemStatus == NULL )
	{
		Hide();
		return;
	}

	SPItemAttr*	pItemAttr	=	pItemStatus->GetItemAttr();

	if( pItemAttr == NULL )
	{
		Hide();
		return;
	}

	m_strName = "ItemData";

	char		szBuf[ MAX_PATH ]	=	{};
	const char*	pStrGlobalString	=	NULL;
	const char*	pStrGlobalString1	=	NULL;
	const char*	pStrGlobalString2	=	NULL;

	int			i				=	0;
	int			iGSNumber		=	0;
	int			iItemRareLevel	=	pItemStatus->GetRareLevel();
	bool		bExits			=	false;
	D3DXCOLOR	cItemColor;

	if( pItemAttr->m_bNameColor == true )
		cItemColor	=	pItemAttr->GetNameColor();
	else
        cItemColor	=	g_pCheckManager->GetItemColor( iItemRareLevel );

    //	제목
	m_pTitle->SetWindowText( pItemAttr->m_strName.c_str() );
	m_pTitle->SetFontColor( cItemColor );

	//	장비종류
	iGSNumber	=	GS_TOOLTIP_ITEM_TYPE + (int)pItemAttr->m_eType * 10 + (int)pItemAttr->m_eSubType;
	pStrGlobalString1 = g_pResourceManager->GetGlobalString( iGSNumber );

	//	세부종류
	iGSNumber	=	GS_TOOLTIP_ITEM_SUBTYPE + (int)pItemAttr->m_eKind;
	pStrGlobalString2 = g_pResourceManager->GetGlobalString( iGSNumber );

	MakeContentsOne( pStrGlobalString1 , pStrGlobalString2 , m_cColors[ TCC_BLACK ] );

	//////////////////////////////////////////////////////////////////////////
	//[xialin 2008/10/22]add
	std::string	strLotteryName	=	pItemStatus->GetLotteryName();
	if( strLotteryName.empty() == false )
	{
		SetContents( TCT_STRING , strLotteryName.c_str() , m_cColors[ TCC_BLACK ] );
		SetContents( TCT_LINE );
	}

	//////////////////////////////////////////////////////////////////////////

	//	제한 조건
	SPItemRequire*	pItemReqire		=	NULL;

	for( i = 0 ; i < MAX_ITEM_OPTION ; ++i )
	{
		pItemReqire	=	&pItemAttr->m_RequireType[i];

		if( pItemReqire->m_iType == 0 || pItemReqire->m_iType > 5 )
			continue;

		SetWindowRequire(	pItemReqire->m_iType , pItemReqire->m_iID ,
							pItemReqire->m_iValue1 , 0 ,pItemReqire->m_iValue2 , 0 , 0 ,
							GS_TOOLTIP_COMMON_REQUIRE , m_cColors[ TCC_REQUIRE ] , m_cColors[ TCC_DISABLE ] );
		bExits	=	true;
	}

	//////////////////////////////////////////////////////////////////////////
	
	//	옵션 포인트
	pStrGlobalString = g_pResourceManager->GetGlobalString( GS_TOOLTIP_ITEM_OPTION_POINT );

	if( pStrGlobalString != NULL )
	{
		int	iOpt	=	(int)( pItemStatus->SPSendMessage( MV_ITEM_GET_OPTPOINT ) );

		if( pItemAttr->m_iOptionType != 0 && iOpt != 0 )
		{
			sprintf( szBuf , pStrGlobalString , iOpt);
			SetContents( TCT_STRING , szBuf , m_cColors[ TCC_ITEM_OPT ] );
			bExits	=	true;
		}
	}

	//////////////////////////////////////////////////////////////////////////

	//	옵션 등급
	pStrGlobalString = g_pResourceManager->GetGlobalString( GS_TOOLTIP_ITEM_OPTION_RARE );

	if( pStrGlobalString != NULL )
	{
		if( pItemAttr->m_iOptionClass != 0 )
		{
			sprintf( szBuf , pStrGlobalString , pItemAttr->m_iOptionClass );
			SetContents( TCT_STRING , szBuf , m_cColors[ TCC_ITEM_OPTION_LIMIT ] );
			bExits	=	true;
		}
	}

	//////////////////////////////////////////////////////////////////////////

	//	줄 뛰우기

	if( bExits == true )
		SetContents( TCT_SPACE );

	//////////////////////////////////////////////////////////////////////////
	
	//	능력

	bExits		=	false;
	SPEffectValue*	pEffectValue	=	NULL;
	bool			bPvpZone		=	SPPvpManager::GetInstance()->IsPvpZone();

	for( i = 0 ; i < MAX_ITEM_OPTION ; ++i )
	{
		if( bPvpZone == false )
			pEffectValue	=	&pItemAttr->m_StatusType[ i ];
		else
			pEffectValue	=	&pItemAttr->m_StatusTypePvp[ i ];

		if( pEffectValue->Effect == ITEM_EQUIP_EFFECT_NULL )
			continue;

		iGSNumber	=	GS_TOOLTIP_ITEM_EFFECT	+	(int)pEffectValue->Effect;
		pStrGlobalString = g_pResourceManager->GetGlobalString( iGSNumber );

		if( pStrGlobalString == NULL )
			continue;

		sprintf( szBuf , pStrGlobalString , pEffectValue->iValue );

		SetContents( TCT_STRING , szBuf , m_cColors[ TCC_ITEM_EFFECT ] );
		bExits	=	true;
	}

	//////////////////////////////////////////////////////////////////////////
	
	//	이펙트 ID

	//	장비 아이템일 경우만 출력
	if( pItemAttr->m_eType == ITEM_TYPE_FIGURE )
	{
		SPEffectManager*	pEffectManager	=	SPEffectManager::GetInstance();
		SPEffect*			pEffect			=	NULL;
		int					iEffectID		=	0;
		const char*			pszName;

		for( i = 0 ; i < MAX_ITEM_EFFECT ; ++i )
		{
			iEffectID	=	pItemAttr->m_iEffectID[ i ];

			if( iEffectID <= 0 )
				continue;

			pEffect	=	pEffectManager->GetEffect( iEffectID );

			if( pEffect == NULL )
				continue;

			pszName	=	pEffect->GetName();

			if( pszName == NULL )
				continue;

			if( strlen( pszName ) <= 0 )
				continue;

			SetContents( TCT_STRING , pszName , m_cColors[ TCC_ITEM_EFFECT ] );
			bExits	=	true;
		}
	}

	//////////////////////////////////////////////////////////////////////////

	//	줄 뛰우기

	if( bExits == true )
		SetContents( TCT_SPACE );

	//////////////////////////////////////////////////////////////////////////
	
	//	옵션

	bExits	=	false;
	PACK_OPT*		pPackOption		=	NULL;
	SPItemOption*	pItemOption		=	NULL;
	D3DXCOLOR		cColors;

	--iItemRareLevel;

	for( i = 0 ; i < MAX_ITEM_OPT ; ++i )
	{
		pPackOption	=	pItemStatus->GetOption( i );

		if( pPackOption == NULL )
			continue;

		if( pPackOption->iID == 0 )
			continue;

		pItemOption	=	g_pItemCluster->GetItemOption( pPackOption->iID );

		if( pItemOption == NULL )
			continue;

		if( pItemOption->m_strName.empty() == true )
			continue;

		sprintf( szBuf , pItemOption->m_strName.c_str() , pPackOption->iValue );

		if( iItemRareLevel == i )
			cColors	=	m_cColors[ TCC_ITEM_OPTION_ENCHANT ];
		else
			cColors	=	m_cColors[ TCC_ITEM_OPTION ];

		SetContents( TCT_STRING , szBuf , cColors );
		bExits	=	true;
	}

	//////////////////////////////////////////////////////////////////////////

	//	줄 뛰우기

	if( bExits == true )
		SetContents( TCT_SPACE );

	//////////////////////////////////////////////////////////////////////////

	//	아이템 시간

	strcpy( szBuf , "" );

	int	iItemSec	=	pItemStatus->GetLifeTime();
	int	iItemMin	=	iItemSec	/	60;
	int	iItemHour	=	iItemMin	/	60;
	int	iItemDay	=	iItemHour	/	24;

	iItemSec		=	iItemSec	-	iItemMin	*	60;
	iItemMin		=	iItemMin	-	iItemHour	*	60;
	iItemHour		=	iItemHour	-	iItemDay	*	24;

	//	정액 시간제
	if( pItemAttr->m_eDuration == ITEM_DURATION_AMOUNT )
	{
		if( iItemDay > 0 )
		{
			pStrGlobalString = g_pResourceManager->GetGlobalString( GS_TOOLTIP_ITEM_DURATION_AMOUNT_DAY );
			sprintf( szBuf , pStrGlobalString , iItemDay , iItemHour );
		}
		else if( iItemHour > 0 )
		{
			pStrGlobalString = g_pResourceManager->GetGlobalString( GS_TOOLTIP_ITEM_DURATION_AMOUNT_HOUR );
			sprintf( szBuf , pStrGlobalString , iItemHour , iItemMin );
		}
		else
		{
			pStrGlobalString = g_pResourceManager->GetGlobalString( GS_TOOLTIP_ITEM_DURATION_AMOUNT_ETC );
			sprintf( szBuf , pStrGlobalString , iItemMin );
		}
	}
	//	정량 시간제
	else if( pItemAttr->m_eDuration == ITEM_DURATION_QUANTITY )
	{
		if( iItemDay > 0 )
		{
			pStrGlobalString = g_pResourceManager->GetGlobalString( GS_TOOLTIP_ITEM_DURATION_QUANTITY_DAY );
			sprintf( szBuf , pStrGlobalString , iItemDay , iItemHour );
		}
		else if( iItemHour > 0 )
		{
			pStrGlobalString = g_pResourceManager->GetGlobalString( GS_TOOLTIP_ITEM_DURATION_QUANTITY_HOUR );
			sprintf( szBuf , pStrGlobalString , iItemHour , iItemMin );
		}
		else
		{
			pStrGlobalString = g_pResourceManager->GetGlobalString( GS_TOOLTIP_ITEM_DURATION_QUANTITY_ETC );
			sprintf( szBuf , pStrGlobalString , iItemMin );
		}
	}

	if( pItemAttr->m_eDuration != ITEM_DURATION_NULL &&
		iItemDay <= 0 && iItemHour <= 0 && iItemMin <= 0 && iItemSec <= 0 )
	{
		pStrGlobalString = g_pResourceManager->GetGlobalString( GS_TOOLTIP_ITEM_EXPIRE );

		if( pStrGlobalString )
			strcpy( szBuf , pStrGlobalString );
	}

	if( strlen( szBuf ) > 0 )
	{
		SetContents( TCT_STRING , szBuf , m_cColors[ TCC_ITEM_TIME ] );
		SetContents( TCT_SPACE );
	}

	//	아이템 쿨타임
	SPCoolTimeManager*	pCoolTimeManager	=	SPCoolTimeManager::GetInstance();

	if( pCoolTimeManager != NULL )
	{
		COOLTIME_INFO*		pCoolTimeInfo		=	pCoolTimeManager->GetCoolTime( pItemAttr->m_iItemCoolTimeID );

		if( pCoolTimeInfo != NULL )
		{
			int	iSec	=	pCoolTimeInfo->fCoolTime - pCoolTimeInfo->fAccmulateTime;
			int	iMin	=	iSec	/	60;
			int	iHour	=	iMin	/	60;
			int	iDay	=	iHour	/	24;

			iSec		=	iSec	-	iMin	*	60;
			iMin		=	iMin	-	iHour	*	60;
			iHour		=	iHour	-	iDay	*	24;

			strcpy( szBuf , "" );

			if( iDay > 0 )
			{
				pStrGlobalString = g_pResourceManager->GetGlobalString( GS_TOOLTIP_ITEM_COOLTIME_DAY );
				sprintf( szBuf , pStrGlobalString , iDay , iHour );
			}
			else if( iHour > 0 )
			{
				pStrGlobalString = g_pResourceManager->GetGlobalString( GS_TOOLTIP_ITEM_COOLTIME_HOUR );
				sprintf( szBuf , pStrGlobalString , iHour , iMin );
			}
			else
			{
				if( iMin > 0 )
				{
					pStrGlobalString = g_pResourceManager->GetGlobalString( GS_TOOLTIP_ITEM_COOLTIME_ETC );
					sprintf( szBuf , pStrGlobalString , iMin , iSec );
				}
			}

			if( strlen( szBuf ) > 0 )
			{
				SetContents( TCT_STRING , szBuf , m_cColors[ TCC_ITEM_COOLTIME ] );
				SetContents( TCT_SPACE );
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////

	//	아이템 되팔기 가격

	if( g_pEventManager != NULL )
	{
		ITEMNO iItemNo = *((ITEMNO*)pItemStatus->SPSendMessage( MV_ITEM_GET_ITEMNO ));

		if( iItemNo > 0)
		{
			int iPrice	=	0;

			if( g_pEventManager->GetCutInState() == CUT_IN_STATE_TRADE )
			{
				SPEventNpcHandler*	pEventNpcHandler	=	g_pEventManager->GetEventHandler();

				if( pEventNpcHandler != NULL )
				{
					SPNpcShop* pNpcShop	=	pEventNpcHandler->GetNpcShopData();

					if( pNpcShop != NULL )
					{
						iPrice = (int)( GET_SELL_PRICE( pItemAttr->m_iGamePrice , pItemStatus->GetStackCount() , pNpcShop->m_iBuyFactor , 100 ) );
					}
				}
			}

			if( g_pCheckManager->IsPetShopOpen() == true )
			{
				iPrice = (int)( GET_SELL_PRICE( pItemAttr->m_iGamePrice , pItemStatus->GetStackCount() , ITEMSKILL_SELL_PERCENT , 100 ) );
			}

			if( iPrice > 0 )
			{
				pStrGlobalString = g_pResourceManager->GetGlobalString( GS_TOOLTIP_ITEM_SELL_PRICE );

				if( pStrGlobalString != NULL )
				{
					sprintf( szBuf , pStrGlobalString , iPrice );

					SetContents( TCT_STRING , szBuf , m_cColors[ TCC_BLACK ] );
					SetContents( TCT_SPACE );
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////

	//	판매 가격

	int	iSellPrice	=	0;
	pStrGlobalString	=	NULL;

	if( pItemAttr->m_iCash > 0 )
	{
		iSellPrice	=	pItemAttr->m_iCash;
		pStrGlobalString = g_pResourceManager->GetGlobalString( GS_TOOLTIP_ITEM_UNIT_OZ );
	}
	else if( pItemAttr->m_iPoint > 0 )
	{
		iSellPrice	=	pItemAttr->m_iPoint;
		pStrGlobalString = g_pResourceManager->GetGlobalString( GS_TOOLTIP_ITEM_UNIT_PT );
	}

	if( iSellPrice > 0 && pStrGlobalString != NULL )
	{
		sprintf( szBuf , pStrGlobalString , ConvertMoneyToString ( iSellPrice ) );

		SetContents( TCT_STRING , szBuf , m_cColors[ TCC_BLUE ] );
		SetContents( TCT_SPACE );
	}

	//////////////////////////////////////////////////////////////////////////

	//	설명
	std::string	strDescription	=	pItemAttr->m_strDescription;
	strDescription				+=	pItemStatus->GetHotDesc();

	if( strDescription.empty() == false )
	{
		SetDesc( strDescription.c_str() );
	}

	//////////////////////////////////////////////////////////////////////////

	//	Set
	if( pItemAttr->m_iSetID > 0 )
	{
		CreateItemDataSet( pItemAttr->m_iSetID , cItemColor );
	}
}

void SPWindowToolTip::CreateTitle( UINT32 uiTitleID )
{
	CUT_IN_STATE	eCutInState		=	g_pEventManager->GetCutInState();

	if( eCutInState != CUT_IN_STATE_NULL )
		return;

	SPTitleManager*	pTitleManager	=	SPTitleManager::GetInstance();

	if( pTitleManager == NULL )
		return;

	SPTitleAttr*	pTitleAttr	=	pTitleManager->GetTitleAttr( uiTitleID );

	if( pTitleAttr == NULL )
		return;

	m_strName = "TitleData";

	char		szBuf[ MAX_PATH ]	=	{};
	const char*	pStrGlobalString	= NULL;

	int	i				=	0;
	int	iGSNumber		=	0;

	//	제목
	m_pTitle->SetWindowText( pTitleAttr->GetName() );
	m_pTitle->SetFontColor( m_cColors[ TCC_BLACK ] );

	//	종류
	iGSNumber	=	GS_TOOLTIP_TITLE_TYPE;
	pStrGlobalString = g_pResourceManager->GetGlobalString( iGSNumber );

	MakeContentsOne( pStrGlobalString , NULL , m_cColors[ TCC_BLACK ] );

	DWORD	dwStatusType		=	0;
	DWORD	dwEffectType		=	0;
	int		iTitleStatusCount	=	0;
	int		iTitleEffectCount	=	0;

	//	상태
	for( i = 0 ; i < SPTitleAttr::TITLE_STATUS_COUNT ; ++i )
	{
		dwStatusType	=	pTitleAttr->GetStatusType( i );

		if( dwStatusType != 0 )
		{
			iGSNumber	=	GS_TOOLTIP_ITEM_EFFECT	+	dwStatusType;
			pStrGlobalString = g_pResourceManager->GetGlobalString( iGSNumber );

			if( pStrGlobalString == NULL )
				continue;

			sprintf( szBuf , pStrGlobalString , pTitleAttr->GetStatusTypeValue( i ) );

			SetContents( TCT_STRING , szBuf , m_cColors[ TCC_BLACK ] );
		}
	}

	//	이펙트
	for( i = 0 ; i < SPTitleAttr::TITLE_EFFECT_COUNT ; ++i )
	{
		dwEffectType	=	pTitleAttr->GetEffect( i );

		if( dwEffectType != 0 )
		{
			pStrGlobalString	=	pTitleAttr->GetEffectDesc( i );

			if( pStrGlobalString != NULL )
				SetContents( TCT_STRING , pStrGlobalString , m_cColors[ TCC_BLACK ] );
		}
	}

	//	설명
	SetDesc( pTitleAttr->GetDesc() );
}

void SPWindowToolTip::CreateCashItem( SPCashItemAttr* pCashItemAttr )
{
	if( pCashItemAttr == NULL )
	{
		Hide();
		return;
	}

	m_strName = "CashItemData";

	char		szBuf[ MAX_PATH ]	=	{};
	const char*	pStrGlobalString	= NULL;
	const char*	pStrGlobalString1	= NULL;
	const char*	pStrGlobalString2	= NULL;

	int	i				=	0;
	int	iGSNumber		=	0;

	if( pCashItemAttr->ItemInfo[0].iID == 0 )
	{
		Hide();
		return;
	}

	SPItemAttr*	pItemAttr = NULL;

	g_pItemCluster->GetItemInfo( pCashItemAttr->ItemInfo[0].iID , pItemAttr );

	if( pItemAttr == NULL )
	{
		Hide();
		return;
	}

	//	제목
	m_pTitle->SetWindowText( pCashItemAttr->strName.c_str() );
	m_pTitle->SetFontColor( g_pCheckManager->GetItemColor( 1 ) );

	//	종류
	iGSNumber	=	GS_TOOLTIP_ITEM_TYPE + (int)pItemAttr->m_eType * 10 + (int)pItemAttr->m_eSubType;
	pStrGlobalString1 = g_pResourceManager->GetGlobalString( iGSNumber );

	//	세부 종류
	iGSNumber	=	GS_TOOLTIP_ITEM_SUBTYPE + (int)pItemAttr->m_eKind;
	pStrGlobalString2 = g_pResourceManager->GetGlobalString( iGSNumber );

	MakeContentsOne( pStrGlobalString1 , pStrGlobalString2 , m_cColors[ TCC_BLACK ] );

	//////////////////////////////////////////////////////////////////////////

	//	전체 갯수를 체크한다.
	int	iItemCount	=	0;

	for( i = 0 ; i < CASHITEMINFO_COUNT ; ++i )
	{
		if( pCashItemAttr->ItemInfo[ i ].iID != 0 )
		{
			++iItemCount;
		}
	}

	//	구성내용
	int	iCashItemID	=	0;

	if( iItemCount > 0 )
	{
		SetContents( TCT_STRING , g_pResourceManager->GetGlobalString( GS_TOOLTIP_CASHITEM_LINEUP ) , m_cColors[ TCC_BLACK ] );
		SetContents( TCT_LINE );

		char	szBuf[ _MAX_PATH ];
		char	szBufCount[ _MAX_PATH ];
		const char*	pszCount	=	g_pResourceManager->GetGlobalString( GS_TOOLTIP_CASHITEM_COUNT );

		if( pszCount == NULL )
			pszCount	=	" %dEA";

		for( i = 0 ; i < CASHITEMINFO_COUNT ; ++i )
		{
			iCashItemID	=	pCashItemAttr->ItemInfo[ i ].iID;

			if( iCashItemID == 0 )
				continue;

			pItemAttr	=	NULL;

			g_pItemCluster->GetItemInfo( iCashItemID , pItemAttr );

			if( pItemAttr == NULL )
				continue;

			if( pCashItemAttr->ItemInfo[ i ].iCount > 1 )
			{
				sprintf( szBufCount , pszCount , pCashItemAttr->ItemInfo[ i ].iCount );
				strcpy( szBuf , pItemAttr->m_strName.c_str() );
				strcat( szBuf , szBufCount );

				SetContents( TCT_STRING , szBuf , m_cColors[ TCC_BLACK ] );
			}
			else
			{
				SetContents( TCT_STRING , pItemAttr->m_strName.c_str() , m_cColors[ TCC_BLACK ] );
			}
		}
	}

	//	설명
	std::string	strDescription	=	pCashItemAttr->strDescription;
	strDescription				+=	pCashItemAttr->strHotDesc;

	if( strDescription.empty() == false )
		SetDesc( strDescription.c_str() );
}

void SPWindowToolTip::CreatePushItem( SPCashItemAttr* pCashItemAttr )
{
	if( pCashItemAttr == NULL )
	{
		Hide();
		return;
	}

	m_strName = "CashItemData";

	char		szBuf[ MAX_PATH ]	=	{};
	const char*	pStrGlobalString	= NULL;
	const char*	pStrGlobalString1	= NULL;
	const char*	pStrGlobalString2	= NULL;

	int	i				=	0;
	int	iGSNumber		=	0;

	if( pCashItemAttr->ItemInfo[0].iID == 0 )
	{
		Hide();
		return;
	}

	SPItemAttr*	pItemAttr = NULL;

	g_pItemCluster->GetItemInfo( pCashItemAttr->ItemInfo[0].iID , pItemAttr );

	if( pItemAttr == NULL )
	{
		Hide();
		return;
	}

	//	제목
	m_pTitle->SetWindowText( pCashItemAttr->strName.c_str() );
	m_pTitle->SetFontColor( g_pCheckManager->GetItemColor( 1 ) );

	//	종류
	iGSNumber	=	GS_TOOLTIP_ITEM_TYPE + (int)pItemAttr->m_eType * 10 + (int)pItemAttr->m_eSubType;
	pStrGlobalString1 = g_pResourceManager->GetGlobalString( iGSNumber );

	//	세부 종류
	iGSNumber	=	GS_TOOLTIP_ITEM_SUBTYPE + (int)pItemAttr->m_eKind;
	pStrGlobalString2 = g_pResourceManager->GetGlobalString( iGSNumber );

	MakeContentsOne( pStrGlobalString1 , pStrGlobalString2 , m_cColors[ TCC_BLACK ] );

	//////////////////////////////////////////////////////////////////////////

	//	전체 갯수를 체크한다.
	int	iItemCount	=	0;

	for( i = 0 ; i < CASHITEMINFO_COUNT ; ++i )
	{
		if( pCashItemAttr->ItemInfo[ i ].iID != 0 )
		{
			++iItemCount;
		}
	}

	//	구성내용
	int	iCashItemID	=	0;

	if( iItemCount > 0 )
	{
		SetContents( TCT_STRING , g_pResourceManager->GetGlobalString( GS_TOOLTIP_CASHITEM_LINEUP ) , m_cColors[ TCC_BLACK ] );
		SetContents( TCT_LINE );

		char	szBuf[ _MAX_PATH ];
		char	szBufCount[ _MAX_PATH ];
		const char*	pszCount	=	g_pResourceManager->GetGlobalString( GS_TOOLTIP_CASHITEM_COUNT );

		if( pszCount == NULL )
			pszCount	=	" %dEA";

		for( i = 0 ; i < CASHITEMINFO_COUNT ; ++i )
		{
			iCashItemID	=	pCashItemAttr->ItemInfo[ i ].iID;

			if( iCashItemID == 0 )
				continue;

			pItemAttr	=	NULL;

			g_pItemCluster->GetItemInfo( iCashItemID , pItemAttr );

			if( pItemAttr == NULL )
				continue;

			if( pCashItemAttr->ItemInfo[ i ].iCount > 1 )
			{
				sprintf( szBufCount , pszCount , pCashItemAttr->ItemInfo[ i ].iCount );
				strcpy( szBuf , pItemAttr->m_strName.c_str() );
				strcat( szBuf , szBufCount );

				SetContents( TCT_STRING , szBuf , m_cColors[ TCC_BLACK ] );
			}
			else
			{
				SetContents( TCT_STRING , pItemAttr->m_strName.c_str() , m_cColors[ TCC_BLACK ] );
			}
		}
	}

	int	iSellPrice	=	0;
	pStrGlobalString	=	NULL;

	if( pCashItemAttr->iSalePrice > 0 )
	{
		iSellPrice	=	pCashItemAttr->iSalePrice;
		pStrGlobalString = g_pResourceManager->GetGlobalString( GS_TOOLTIP_ITEM_UNIT_OZ );
	}
	else if( pCashItemAttr->iPrice > 0 )
	{
		iSellPrice	=	pCashItemAttr->iPrice;
		pStrGlobalString = g_pResourceManager->GetGlobalString( GS_TOOLTIP_ITEM_UNIT_OZ );
	}

	if( iSellPrice > 0 && pStrGlobalString != NULL )
	{
		sprintf( szBuf , pStrGlobalString , ConvertMoneyToString ( iSellPrice ) );

		SetContents( TCT_SPACE );
		SetContents( TCT_STRING , szBuf , m_cColors[ TCC_BLUE ] );
		SetContents( TCT_SPACE );
	}

	//	설명
	std::string	strDescription	=	pCashItemAttr->strDescription;
	strDescription				+=	pCashItemAttr->strHotDesc;

	if( strDescription.empty() == false )
		SetDesc( strDescription.c_str() );
}

void SPWindowToolTip::CreateEnchant( SPItemEnchantResult* pEnchantResult )
{
	if( pEnchantResult == NULL )
	{
		Hide();
		return;
	}

	SPItem*	pEnchantItem	=	(SPItem*)pEnchantResult->pItem;

	if( pEnchantItem == NULL )
	{
		Hide();
		return;
	}

	SPItemAttr*		pItemAttr	=	pEnchantItem->GetItemAttr();
	SPItemStatus*	pItemStatus	=	pEnchantItem->GetItemStatus();

	if( pItemAttr == NULL || pItemStatus == NULL || pEnchantResult->pItemEnchant == NULL )
	{
		Hide();
		return;
	}

	SPItemOption*	pItemOption		=	g_pItemCluster->GetItemOption( pEnchantResult->pItemEnchant->iResult );

	if( pItemOption == NULL )
	{
		Hide();
		return;
	}

	m_strName = "Enchant";

	char		szBuf[ MAX_PATH ]	=	{};
	const char*	pStrGlobalString	= NULL;
	const char*	pStrGlobalString1	= NULL;
	const char*	pStrGlobalString2	= NULL;

	int			i				=	0;
	int			iGSNumber		=	0;
	D3DXCOLOR	cItemColor;

	std::string	strGS1;
	std::string	strGS2;

	if( pItemAttr->m_bNameColor == true )
		cItemColor	=	pItemAttr->GetNameColor();
	else
		cItemColor	=	g_pCheckManager->GetItemColor( pItemStatus->GetRareLevel() );

	//	제목
	m_pTitle->SetWindowText( pItemAttr->m_strName.c_str() );
	m_pTitle->SetFontColor( cItemColor );

	//	장비종류
	iGSNumber	=	GS_TOOLTIP_ITEM_TYPE + (int)pItemAttr->m_eType * 10 + (int)pItemAttr->m_eSubType;
	pStrGlobalString1 = g_pResourceManager->GetGlobalString( iGSNumber );

	//	세부종류
	iGSNumber	=	GS_TOOLTIP_ITEM_SUBTYPE + (int)pItemAttr->m_eKind;
	pStrGlobalString2 = g_pResourceManager->GetGlobalString( iGSNumber );

	MakeContentsOne( pStrGlobalString1 , pStrGlobalString2 , m_cColors[ TCC_BLACK ] );

	//////////////////////////////////////////////////////////////////////////

	//	인챈트 가능 조건

	int				iItemRequireCount	=	0;
	SPItemRequire*	pItemReqire			=	NULL;

	for( i = 0 ; i < MAX_ITEM_OPTION ; ++i )
	{
		pItemReqire	=	&( pEnchantResult->pItemEnchant->RequireType[ i ] );

		if( pItemReqire->m_iType != 0 )
		{
			++iItemRequireCount;
			break;
		}
	}

	if( iItemRequireCount > 0 )
	{
		SetContents( TCT_STRING , g_pResourceManager->GetGlobalString( GS_TOOLTIP_ENCHANT_CONDITION ) );

		for( i = 0 ; i < MAX_ITEM_OPTION ; ++i )
		{
			pItemReqire	=	&( pEnchantResult->pItemEnchant->RequireType[ i ] );

			if( pItemReqire->m_iType == 0 )
				continue;

			SetWindowRequire(	pItemReqire->m_iType , pItemReqire->m_iID ,
				pItemReqire->m_iValue1 , 0 ,pItemReqire->m_iValue2 , 0 , 0 ,
				GS_TOOLTIP_COMMON_REQUIRE , m_cColors[ TCC_ITEM_ENCHANT_REQUIRE ] , m_cColors[ TCC_DISABLE ] );
		}

		SetContents( TCT_LINE );
	}

	//////////////////////////////////////////////////////////////////////////

	//	추가 및 변경 옵션
	if( pItemOption->m_strDesc.empty() == false )
	{
		SetContents( TCT_STRING , g_pResourceManager->GetGlobalString( GS_TOOLTIP_ENCHANT_OPTION ) , m_cColors[ TCC_BLACK ] );

		int				iFindOptionID		=	pEnchantResult->pItemEnchant->iItemOptionGroup + pEnchantResult->pItemEnchant->iItemOptionLevel;
		bool			bFindOptionID		=	false;
		PACK_OPT*		pPackOption			=	NULL;

		for( i = 0 ; i < MAX_ITEM_OPT ; ++i )
		{
			pPackOption	=	pItemStatus->GetOption( i );

			if( pPackOption == NULL )
				continue;

			if( pPackOption->iID == 0 )
				continue;

			if( iFindOptionID == pPackOption->iID )
			{
				bFindOptionID	=	true;
				break;
			}
		}

		int	iMin	=	0;
		int iMax	=	0;

		if( bFindOptionID == true && pPackOption != NULL )
		{
			int	iMin	=	pPackOption->iValue;
			int	iMax	=	pPackOption->iValue;
		}

		switch( pItemOption->m_iAppType )
		{
		case 0:	//	첨가 : 값을 기존에서 더한다.
			iMin	+=	pItemOption->m_iEnchantMin;
			iMax	+=	pItemOption->m_iEnchantMax;
			break;
		case 1:	//	변화 : 값을 기존에서 변화한다.
			iMin	=	pItemOption->m_iEnchantMin;
			iMax	=	pItemOption->m_iEnchantMax;
			break;
		}

		sprintf( szBuf , pItemOption->m_strDesc.c_str() , iMin , iMax );
		SetContents( TCT_STRING , szBuf , m_cColors[ TCC_ITEM_ENCHANT_OPTION ] );
		SetContents( TCT_LINE );
	}

	//////////////////////////////////////////////////////////////////////////
	
	//	설명
	SetDesc( NULL );
}

void SPWindowToolTip::CreateItemUpgrade( SPItemStatus* pItemUpgradeStatus )
{
	if( pItemUpgradeStatus == NULL )
	{
		Hide();
		return;
	}

	SPItemUpgrade*	pItemUpgrade	=	g_pItemCluster->GetItemUpgrade( pItemUpgradeStatus->GetItemID() , pItemUpgradeStatus->GetRareLevel() );

	if( pItemUpgrade == NULL )
	{
		Hide();
		return;
	}

	m_pItem->SetItemStatus( pItemUpgrade->m_iResult , pItemUpgrade->m_eRare_Original );

	SPItemAttr*		pItemAttr	=	m_pItem->GetItemAttr();
	SPItemStatus*	pItemStatus	=	m_pItem->GetItemStatus();

	if( pItemAttr == NULL || pItemStatus == NULL )
	{
		Hide();
		return;
	}

	m_strName = "ItemUpgrade";

	char		szBuf[ MAX_PATH ]	=	{};
	const char*	pStrGlobalString	= NULL;
	const char*	pStrGlobalString1	= NULL;
	const char*	pStrGlobalString2	= NULL;

	int			i				=	0;
	int			iGSNumber		=	0;
	int			iItemRareLevel	=	pItemStatus->GetRareLevel();
	D3DXCOLOR	cItemColor;

	if( pItemAttr->m_bNameColor == true )
		cItemColor	=	pItemAttr->GetNameColor();
	else
		cItemColor	=	g_pCheckManager->GetItemColor( iItemRareLevel );

	//	제목
	m_pTitle->SetWindowText( pItemAttr->m_strName.c_str() );
	m_pTitle->SetFontColor( cItemColor );

	//	장비종류
	iGSNumber	=	GS_TOOLTIP_ITEM_TYPE + (int)pItemAttr->m_eType * 10 + (int)pItemAttr->m_eSubType;
	pStrGlobalString1 = g_pResourceManager->GetGlobalString( iGSNumber );

	//	세부종류
	iGSNumber	=	GS_TOOLTIP_ITEM_SUBTYPE + (int)pItemAttr->m_eKind;
	pStrGlobalString2 = g_pResourceManager->GetGlobalString( iGSNumber );

	MakeContentsOne( pStrGlobalString1 , pStrGlobalString2 , m_cColors[ TCC_BLACK ] );

	//////////////////////////////////////////////////////////////////////////

	//	제한

	bool	bIsExit	=	false;
	SPItemRequire*	pItemRequire			=	NULL;

	for( i = 0 ; i < MAX_ITEM_OPTION ; ++i )
	{
		pItemRequire	=	&pItemAttr->m_RequireType[i];

		if( pItemRequire->m_iType == 0 || pItemRequire->m_iType > 5 )
			continue;

		SetWindowRequire(	pItemRequire->m_iType , pItemRequire->m_iID ,
							pItemRequire->m_iValue1 , 0 ,pItemRequire->m_iValue2 , 0 , 0 ,
							GS_TOOLTIP_COMMON_REQUIRE , m_cColors[ TCC_BLACK ] , m_cColors[ TCC_DISABLE ] );

		bIsExit	=	true;
	}

	if( bIsExit == true )
		SetContents( TCT_LINE );

	//////////////////////////////////////////////////////////////////////////
	
	//	업그레이드 제한

	int				iItemUpgradeRequireCount	=	0;
	SPItemRequire*	pItemUpgradeRequire			=	NULL;

	for( i = 0 ; i < MAX_ITEM_UPGRADE_REQUIRE ; ++i )
	{
		pItemUpgradeRequire	=	&pItemUpgrade->m_stRequireType[ i ];

		if( pItemUpgradeRequire->m_iType != 0 && pItemUpgradeRequire->m_iType < 6 )
		{
			++iItemUpgradeRequireCount;
			break;
		}
	}

	if( iItemUpgradeRequireCount > 0 )
	{
		SetContents( TCT_STRING , g_pResourceManager->GetGlobalString( GS_TOOLTIP_ITEMUPGRADE_REQUIRE ) , m_cColors[ TCC_BLACK ] );

		for( i = 0 ; i < MAX_ITEM_UPGRADE_REQUIRE ; ++i )
		{
			pItemUpgradeRequire	=	&pItemUpgrade->m_stRequireType[ i ];

			if( pItemUpgradeRequire->m_iType == 0 || pItemUpgradeRequire->m_iType > 5 )
				continue;

			SetWindowRequire(	pItemUpgradeRequire->m_iType , pItemUpgradeRequire->m_iID ,
								pItemUpgradeRequire->m_iValue1 , 0 ,pItemUpgradeRequire->m_iValue2 , 0 , 0 ,
								GS_TOOLTIP_COMMON_REQUIRE , m_cColors[ TCC_ITEM_UPGRADE_REQUIRE ] , m_cColors[ TCC_DISABLE ] );
		}

		SetContents( TCT_LINE );
	}

	//////////////////////////////////////////////////////////////////////////

	//	능력

	bIsExit	=	false;
	SPEffectValue*	pEffectValue	=	NULL;
	bool			bPvpZone		=	SPPvpManager::GetInstance()->IsPvpZone();

	for( i = 0 ; i < MAX_ITEM_OPTION ; ++i )
	{
		if( bPvpZone == false )
			pEffectValue	=	&pItemAttr->m_StatusType[ i ];
		else
			pEffectValue	=	&pItemAttr->m_StatusTypePvp[ i ];

		if( pEffectValue->Effect == ITEM_EQUIP_EFFECT_NULL )
			continue;

		iGSNumber	=	GS_TOOLTIP_ITEM_EFFECT	+	(int)pEffectValue->Effect;
		pStrGlobalString = g_pResourceManager->GetGlobalString( iGSNumber );

		if( pStrGlobalString == NULL )
			continue;

		sprintf( szBuf , pStrGlobalString , pEffectValue->iValue );

		SetContents( TCT_STRING , szBuf , m_cColors[ TCC_ITEM_UPGRADE_EFFECT ] );

		bIsExit	=	true;

	}

	//////////////////////////////////////////////////////////////////////////

	//	이펙트 ID

	//	장비 아이템일 경우만 출력
	if( pItemAttr->m_eType == ITEM_TYPE_FIGURE )
	{
		SPEffectManager*	pEffectManager	=	SPEffectManager::GetInstance();
		SPEffect*			pEffect			=	NULL;
		int					iEffectID		=	0;
		const char*			pszName;

		for( i = 0 ; i < MAX_ITEM_EFFECT ; ++i )
		{
			iEffectID	=	pItemAttr->m_iEffectID[ i ];

			if( iEffectID <= 0 )
				continue;

			pEffect	=	pEffectManager->GetEffect( iEffectID );

			if( pEffect == NULL )
				continue;

			pszName	=	pEffect->GetName();

			if( pszName == NULL )
				continue;

			if( strlen( pszName ) <= 0 )
				continue;

			SetContents( TCT_STRING , pszName , m_cColors[ TCC_ITEM_EFFECT ] );
			bIsExit	=	true;
		}
	}

	//////////////////////////////////////////////////////////////////////////

	//	줄 뛰우기

	if( bIsExit == true )
		SetContents( TCT_LINE );

	//////////////////////////////////////////////////////////////////////////

	//	옵션	은 원래 아이템에서 상속 받는다

	bIsExit	=	false;
	PACK_OPT*		pPackOption		=	NULL;
	SPItemOption*	pItemOption		=	NULL;
	D3DXCOLOR		cColor;

	--iItemRareLevel;

	for( i = 0 ; i < MAX_ITEM_OPT ; ++i )
	{
		pPackOption	=	pItemUpgradeStatus->GetOption( i );

		if( pPackOption == NULL )
			continue;

		if( pPackOption->iID == 0 )
			continue;

		pItemOption	=	g_pItemCluster->GetItemOption( pPackOption->iID );

		if( pItemOption == NULL )
			continue;

		if( pItemOption->m_strName.empty() == true )
			continue;

		sprintf( szBuf , pItemOption->m_strName.c_str() , pPackOption->iValue );

		if( iItemRareLevel == i )
			cColor	=	m_cColors[ TCC_ITEM_OPTION_ENCHANT ];
		else
			cColor	=	m_cColors[ TCC_ITEM_OPTION ];

		SetContents( TCT_STRING , szBuf , cColor );
		bIsExit	=	true;
	}

	//////////////////////////////////////////////////////////////////////////

	//	줄 뛰우기

	if( bIsExit == true )
		SetContents( TCT_SPACE );

	//////////////////////////////////////////////////////////////////////////
	
	//	설명
	SetDesc( NULL );
}

void SPWindowToolTip::CreateSkillLVUP( SPUIUnit* pUIUnit )
{
	SPUISkillUnit* pUISkillUnit	=	(SPUISkillUnit*)pUIUnit;

	if( pUISkillUnit == NULL )
	{
		Hide();
		return;
	}

	SPPlayer* pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
	{
		Hide();
		return;
	}

	int					iSkillID		=	pUISkillUnit->GetSkillID();
	SPSkillManager*		pSkillManager	=	SPSkillManager::GetInstance();
	SPSkillArchive*		pSkillArchive	=	pLocalPlayer->GetSkillArchive();
	SPSkillActivity*	pSkillActivity	=	pSkillArchive->GetSkillActivity( iSkillID );
	SPSkill*			pSkill			=	NULL;

	if( pSkillActivity != NULL )
		pSkill	=	pSkillActivity->GetSkill();
	else
		pSkill	=	pSkillManager->GetSkill( iSkillID );

	if( pSkill == NULL )
	{
		Hide();
		return;
	}

	m_strName = "SkillLVUP";

	//	제목
	m_pTitle->SetWindowText( pSkill->GetName() );
	m_pTitle->SetFontColor( m_cColors[ TCC_BLACK ] );

	SKILL_UI_INFO*	pUIInfo	=	pSkill->GetUIInfo();

	char			szBuf[_MAX_PATH]	=	{};
	char			szBuf2[_MAX_PATH]	=	{};
	const	char*	pStrFormat			=	NULL;
	const	char*	pStrGlobalString	=	NULL;
	const	char*	pStrGlobalString1	=	NULL;
	const	char*	pStrGlobalString2	=	NULL;

	pStrGlobalString1	=	g_pResourceManager->GetGlobalString( GS_TOOLTIP_SKILL_INVEN_TYPE + (int)(pSkill->GetType()) );

	SPSkillActivityRequire* pActivityRequire	=	pSkill->GetActivityRequire();

	if( pActivityRequire != NULL )
	{
		if( pActivityRequire->m_iItemKind != 0)
		{
			pStrGlobalString2	=	g_pResourceManager->GetGlobalString( GS_TOOLTIP_SKILL_REQUIRE_ITEM_KIND + pActivityRequire->m_iItemKind );

			//if( g_pCheckManager->CheckEquipItem( (EQUIP_ID)pActivityRequire->m_iEquipType , pActivityRequire->m_iItemKind ) == false )
			//	m_pWindowRequire->SetFontColor( m_DisableColor );
		}
	}

	MakeContentsOne( pStrGlobalString1 , pStrGlobalString2 , m_cColors[ TCC_BLACK ] );

	//////////////////////////////////////////////////////////////////////////

	int					iSkillLevel			=	0;
	int					iSkillCurLevel		=	0;
	int					iSkillAddLevel		=	0;
	int					iRequireStateCount	=	(int)( pActivityRequire->m_vRequireStateList.size() );
	SKILL_REQUIRE_STAT	stUpRequireState;

	if( pSkillActivity != NULL )
	{
		iSkillLevel		=	pSkillActivity->GetLevel();
		iSkillCurLevel	=	pSkillActivity->GetCurLevel();
		iSkillAddLevel	=	iSkillCurLevel - iSkillLevel;
	}

	if( iSkillAddLevel <= 0 )
	{
		pStrFormat	=	g_pResourceManager->GetGlobalString( GS_TOOLTIP_SKILL_ADD_LEVEL_0 );

		if( pStrFormat != NULL )
		{
			sprintf( szBuf2 , pStrFormat , iSkillLevel , pSkill->GetMaxLevel() );
		}
	}
	else
	{
		pStrFormat	=	g_pResourceManager->GetGlobalString( GS_TOOLTIP_SKILL_ADD_LEVEL_0_NOT );

		if( pStrFormat != NULL )
		{
			sprintf( szBuf2 , pStrFormat , iSkillLevel , iSkillAddLevel , pSkill->GetMaxLevel() );
		}
	}

	if( strlen( szBuf2 ) > 0 )
	{
		SetContents( TCT_STRING , szBuf2 , m_cColors[ TCC_BLACK ] );
		SetContents( TCT_LINE );
	}

	pStrFormat	=	g_pResourceManager->GetGlobalString( GS_TOOLTIP_SKILL_POINT_TITLE );

	SPSkillUpRequire*	pUpRequire	=	pSkill->GetUpRequre();

	if( pUpRequire != NULL )
	{
		int	iUpRequireSkillPoint	=	pUpRequire->m_iSkillPoint	+	pUpRequire->m_iSkillPointSlv	*	iSkillLevel;

		if( iUpRequireSkillPoint > 0 )
		{
			int iSkillPoint				=	g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValue( STATUS_SKILL_POINT );
			int	iSkillMax				=	pSkill->GetMaxLevel();

			sprintf( szBuf , pStrFormat , iUpRequireSkillPoint );

			if( iSkillLevel < iSkillMax )
			{
				if( iSkillPoint < iUpRequireSkillPoint )
					SetContents( TCT_STRING , szBuf , m_cColors[ TCC_DISABLE ] );
				else
					SetContents( TCT_STRING , szBuf , m_cColors[ TCC_BLACK ] );
			}
		}

		int	iUpRequireStateCount	=	(int)pUpRequire->m_vRequireStateList.size();

		for( int i = 0 ; i < iUpRequireStateCount ; ++i )
		{
			stUpRequireState	=	pUpRequire->m_vRequireStateList.at( i );

			SetWindowRequire(	stUpRequireState.iType		, stUpRequireState.iID			,
								stUpRequireState.iValue		, stUpRequireState.iValueSlv	,
								stUpRequireState.iValue2	, stUpRequireState.iValueSlv2	,
								iSkillLevel , GS_TOOLTIP_SKILL_UP_REQUIRE , m_cColors[ TCC_BLACK ] , m_cColors[ TCC_DISABLE ] );
		}
	}

	//////////////////////////////////////////////////////////////////////////
	
	//	설명

	SetDesc( NULL );
}

void SPWindowToolTip::CreateFriendWhite( FRIEND_INFO* pFriendInfo )
{
	if( pFriendInfo == NULL )
	{
		Hide();
		return;
	}

	m_strName = "FriendWhite";

	char		szBuf[ MAX_PATH ]	=	{};
	char		szBuf1[ MAX_PATH ]	=	{};
	const char*	pStrGlobalString	= NULL;

	int	i				=	0;

	//	제목
	m_pTitle->SetWindowText( pFriendInfo->szCharName );
	m_pTitle->SetFontColor( m_cColors[ TCC_BLACK ] );

	//	종류
	pStrGlobalString = g_pResourceManager->GetGlobalString( GS_TOOLTIP_FRIEND_WHITE_LV );

	if( pStrGlobalString != NULL )
	{
		sprintf( szBuf , pStrGlobalString , pFriendInfo->uiLevel );
	}

	//	세부 종류
	int			iGlobalStringNumber	=	GS_TOOLTIP_FRIEND_WHITE_CHANNEL;
	std::string	strChannel;

	int	iChannelID	=	pFriendInfo->uiServerID	%	100;

	if( iChannelID > INDUN_PVP_CHANNEL_MAX )
	{
		iGlobalStringNumber	=	GS_TOOLTIP_FRIEND_WHITE_PVP;
	}
	else if( iChannelID > INDUN_CHANNEL_MAX )
	{
		iGlobalStringNumber	=	GS_TOOLTIP_FRIEND_WHITE_INDUN;
	}

	pStrGlobalString = g_pResourceManager->GetGlobalString( iGlobalStringNumber );

	if( pStrGlobalString != NULL )
		strChannel	=	pStrGlobalString;

	if( iChannelID > 0 )
		sprintf( szBuf1 , strChannel.c_str() , iChannelID );
   
	MakeContentsOne( szBuf , szBuf1 , m_cColors[ TCC_BLACK ] );

	//	설명
	SetDesc( NULL );
}

void SPWindowToolTip::CreateLottery( WPARAM wParam )
{
	LOTTERY_LDT* pLotterLdt	=	(LOTTERY_LDT*)wParam;

	if( pLotterLdt == NULL )
	{
		Hide();
		return;
	}

	std::string	strDesc;

	if( pLotterLdt->m_iItemID > 0 )
	{
		m_pItem->SetItemStatus( pLotterLdt->m_iItemID );

		SPItemStatus* pItemStatus = m_pItem->GetItemStatus();

		if( pItemStatus != NULL )
		{
			SPItemAttr*	pItemAttr	=	pItemStatus->GetItemAttr();

			if( pItemAttr != NULL )
			{
				if( pItemAttr->GetDurationType() != ITEM_DURATION_NULL )
				{
					pItemStatus->SetLifeTime( pItemAttr->m_iLifeTime * 3600 );
				}
			}
		}

		CreateItemData( pItemStatus );

		if( m_pDesc != NULL )
			strDesc	=	m_strDesc;
//				strDesc = m_pDesc->GetWindowText();

		strDesc	+=	"";//pLotterLdt->m_strDescription;
	}
	else
	{
		strDesc	=	"";//pLotterLdt->m_strDescription;
	}

	m_strName = "Lottery";

	SetDesc( strDesc.c_str() );
}

void SPWindowToolTip::CreateItemDataSet( int iSetItemID , D3DXCOLOR cItemColor )
{
	m_strName = "ItemDataSet";

	SPItemSet* pItemSet = g_pItemCluster->GetItemSet( iSetItemID );

	if( pItemSet == NULL )
	{
		Hide();
		return;
	}

	//	제목
	m_pTitle2->SetWindowText( pItemSet->m_strName.c_str() );
	m_pTitle2->SetFontColor( cItemColor );

	int		iCurrentPieces = pItemSet->m_iCurrentPieces - 1;
	bool	bEnable;
	int		i;

	for( i = 0 ; i < ITEM_SET_A_MAX ; ++i )
	{
		if( iCurrentPieces > i && m_eType == TOOLTIP_SHOW_TYPE_ITEM_SET )
			bEnable	=	true;
		else
			bEnable	=	false;

		CreateItemDataSet( &pItemSet->m_stSetEffect[ i ] , i + 2 , bEnable , m_cColors[ TCC_ITEM_SET_OPTION_ENABLE ] , m_cColors[ TCC_ITEM_SET_OPTION_DISABLE ] , g_pResourceManager->GetGlobalString( GS_TOOLTIP_ITEM_SET_OPTION_LEFT_STRING ) );
	}

	SetContents2( TCT_LINE );
	SetContents2( TCT_STRING , g_pResourceManager->GetGlobalString( GS_TOOLTIP_ITEM_SET_FULL_TITLE ) , m_cColors[ TCC_ITEM_SET_FULL_OPTION_ENABLE ] );

	bEnable	=	pItemSet->m_bFull;

	if( m_eType != TOOLTIP_SHOW_TYPE_ITEM_SET )
		bEnable	=	false;

	for( i = 0 ; i < ITEM_SET_FUUL_OPTION ; ++i )
	{
		CreateItemDataSet( &pItemSet->m_stFullSetEffect[ i ] , i , bEnable , m_cColors[ TCC_ITEM_SET_FULL_OPTION_ENABLE ] , m_cColors[ TCC_ITEM_SET_FULL_OPTION_DISABLE ] , NULL );
	}
}

void SPWindowToolTip::CreateItemDataSet( SPSetEffect* pSetEffect , int iSetNumber , bool bEnable , D3DXCOLOR cEnableColor , D3DXCOLOR cDisableColor , const char* pszLeftString )
{
	if( pSetEffect == NULL )
		return;

	if( pSetEffect->m_iType == ITEM_SET_OPTION_TYPE_NULL )
		return;

	D3DXCOLOR cItemColor	=	cDisableColor;

	if( bEnable == true )
		cItemColor = cEnableColor;

	int				iGSNumber				=	0;
	char			szBuf[ MAX_PATH ]		=	{};
	char			szBufLeft[ MAX_PATH ]	=	{};
	const char*		pStrGlobalString		=	NULL;

	if( pSetEffect->m_iType == ITEM_SET_OPTION_TYPE_STAT )
	{
		SPEffectValue*	pEffectValue	=	&pSetEffect->m_stEffect;

		if( pEffectValue->Effect == ITEM_EQUIP_EFFECT_NULL )
			return;

		iGSNumber	=	GS_TOOLTIP_ITEM_EFFECT	+	(int)pEffectValue->Effect;
		pStrGlobalString = g_pResourceManager->GetGlobalString( iGSNumber );

		if( pStrGlobalString == NULL )
			return;

		sprintf( szBuf , pStrGlobalString , pEffectValue->iValue );

		if( pszLeftString == NULL )
		{
			SetContents2( TCT_STRING , szBuf , cItemColor );
		}
		else
		{
			sprintf( szBufLeft , pszLeftString , iSetNumber , szBuf );
			SetContents2( TCT_STRING , szBufLeft , cItemColor );
		}

	}
	else if( pSetEffect->m_iType == ITEM_SET_OPTION_TYPE_EFFECT )
	{
		SPEffectManager*	pEffectManager	=	SPEffectManager::GetInstance();

		if( pEffectManager == NULL )
			return;

		int iEffectID	=	pSetEffect->m_iEffectID;

		if( iEffectID <= 0 )
			return;

		SPEffect* pEffect	=	pEffectManager->GetEffect( iEffectID );

		if( pEffect == NULL )
			return;

		const char*			pszName	=	pEffect->GetName();

		if( pszName == NULL )
			return;

		if( strlen( pszName ) <= 0 )
			return;

		if( pszLeftString == NULL )
		{
			SetContents2( TCT_STRING , pszName , cItemColor );
		}
		else
		{
			sprintf( szBufLeft , pszLeftString , iSetNumber , pszName );
			SetContents2( TCT_STRING , szBufLeft , cItemColor );
		}
	}
}

void SPWindowToolTip::CreateNpcEvent( WPARAM wParam )
{
	SPNpcEventToolTip*	pNpcEventTooltip	=	(SPNpcEventToolTip*)wParam;

	if( pNpcEventTooltip == NULL )
	{
		Hide();
		return;
	}

	if( pNpcEventTooltip->m_strTitle.empty() == true )
	{
		CreateItemData( &(pNpcEventTooltip->m_pItem) );
	}
	else
	{
		//	제목
		m_pTitle->SetWindowText( pNpcEventTooltip->m_strTitle.c_str() );
		m_pTitle->SetFontColor( m_cColors[ TCC_BLACK ] );
	}

	m_strName = "NpcEventTooltip";

	if( pNpcEventTooltip->m_strDesc.empty() == false )
		SetDesc( pNpcEventTooltip->m_strDesc.c_str() );
}

void SPWindowToolTip::CreateNormal( TOOLTIP_NORMAL_DATA* pToolTipNomal )
{
	if( pToolTipNomal == NULL )
	{
		Hide();
		return;
	}

	m_strName = "NormalTooltip";

	//	제목
	m_pTitle->SetWindowText( pToolTipNomal->m_strName.c_str() );
	m_pTitle->SetFontColor( pToolTipNomal->m_cName );

	int	iContentsCount	=	pToolTipNomal->m_iContentsCount;

	if( iContentsCount >= MAX_TOOLTIP_CONTENTS_LINE )
		iContentsCount	=	MAX_TOOLTIP_CONTENTS_LINE - 1;

	for( int i = 0 ; i < iContentsCount ; ++i )
	{
		SetContents( TCT_STRING , pToolTipNomal->m_strContents[ i ].c_str() , pToolTipNomal->m_cContents[ i ] );
	}

	SetDesc( pToolTipNomal->m_strDesc.c_str() , pToolTipNomal->m_cDesc );
}

void SPWindowToolTip::CreateItemMix( SPItemMix* pItemMix )
{
	if( pItemMix == NULL )
	{
		Hide();
		return;
	}

	m_pItem->SetItemStatus( pItemMix->m_iResult , 1 );

	SPItemAttr*		pItemAttr	=	m_pItem->GetItemAttr();
	SPItemStatus*	pItemStatus	=	m_pItem->GetItemStatus();

	if( pItemAttr == NULL || pItemStatus == NULL )
	{
		Hide();
		return;
	}

	m_strName = "ItemMix";

	char		szBuf[ MAX_PATH ]	=	{};
	const char*	pStrGlobalString	= NULL;
	const char*	pStrGlobalString1	= NULL;
	const char*	pStrGlobalString2	= NULL;

	int			i				=	0;
	int			iGSNumber		=	0;
	int			iItemRareLevel	=	pItemStatus->GetRareLevel();
	D3DXCOLOR	cItemColor;

	if( pItemAttr->m_bNameColor == true )
		cItemColor	=	pItemAttr->GetNameColor();
	else
		cItemColor	=	g_pCheckManager->GetItemColor( iItemRareLevel );

	//	제목
	m_pTitle->SetWindowText( pItemAttr->m_strName.c_str() );
	m_pTitle->SetFontColor( cItemColor );

	//	장비종류
	iGSNumber	=	GS_TOOLTIP_ITEM_TYPE + (int)pItemAttr->m_eType * 10 + (int)pItemAttr->m_eSubType;
	pStrGlobalString1 = g_pResourceManager->GetGlobalString( iGSNumber );

	//	세부종류
	iGSNumber	=	GS_TOOLTIP_ITEM_SUBTYPE + (int)pItemAttr->m_eKind;
	pStrGlobalString2 = g_pResourceManager->GetGlobalString( iGSNumber );

	MakeContentsOne( pStrGlobalString1 , pStrGlobalString2 , m_cColors[ TCC_BLACK ] );

	//////////////////////////////////////////////////////////////////////////

	//	조합 제한

	int				iItemMixRequireCount	=	0;
	SPItemRequire*	pItemMixRequire			=	NULL;
	bool			bItemMixRequire			=	false;

	for( i = 0 ; i < MAX_ITEM_MIX_REQUIRE ; ++i )
	{
		pItemMixRequire	=	&pItemMix->m_stRequireType[ i ];

		if( pItemMixRequire->m_iType == 0 || pItemMixRequire->m_iType > 5 )
			continue;

		if( g_pCheckManager->CheckCondition( pItemMixRequire->m_iType , pItemMixRequire->m_iID , pItemMixRequire->m_iValue1 , pItemMixRequire->m_iValue2 ) == false )
		{
			bItemMixRequire	=	true;
			break;
		}
	}

	if( bItemMixRequire == true )
	{
		for( i = 0 ; i < MAX_ITEM_MIX_REQUIRE ; ++i )
		{
			pItemMixRequire	=	&pItemMix->m_stRequireType[ i ];

			if( pItemMixRequire->m_iType == 0 || pItemMixRequire->m_iType > 5 )
				continue;

			SetWindowRequire(	pItemMixRequire->m_iType , pItemMixRequire->m_iID ,
								pItemMixRequire->m_iValue1 , 0 ,pItemMixRequire->m_iValue2 , 0 , 0 ,
								GS_TOOLTIP_COMMON_REQUIRE , m_cColors[ TCC_BLACK ] , m_cColors[ TCC_DISABLE ] );
		}

		return;
	}

	//////////////////////////////////////////////////////////////////////////

	//	제한

	bool			bIsExit			=	false;
	SPItemRequire*	pItemRequire	=	NULL;

	for( i = 0 ; i < MAX_ITEM_OPTION ; ++i )
	{
		pItemRequire	=	&pItemAttr->m_RequireType[i];

		if( pItemRequire->m_iType == 0 || pItemRequire->m_iType > 5 )
			continue;

		SetWindowRequire(	pItemRequire->m_iType , pItemRequire->m_iID ,
							pItemRequire->m_iValue1 , 0 ,pItemRequire->m_iValue2 , 0 , 0 ,
							GS_TOOLTIP_COMMON_REQUIRE , m_cColors[ TCC_BLACK ] , m_cColors[ TCC_DISABLE ] );

		bIsExit	=	true;
	}

	if( bIsExit == true )
		SetContents( TCT_LINE );

	//////////////////////////////////////////////////////////////////////////

	//	능력

	bIsExit	=	false;
	SPEffectValue*	pEffectValue	=	NULL;
	bool			bPvpZone		=	SPPvpManager::GetInstance()->IsPvpZone();

	for( i = 0 ; i < MAX_ITEM_OPTION ; ++i )
	{
		if( bPvpZone == false )
			pEffectValue	=	&pItemAttr->m_StatusType[ i ];
		else
			pEffectValue	=	&pItemAttr->m_StatusTypePvp[ i ];

		if( pEffectValue->Effect == ITEM_EQUIP_EFFECT_NULL )
			continue;

		iGSNumber	=	GS_TOOLTIP_ITEM_EFFECT	+	(int)pEffectValue->Effect;
		pStrGlobalString = g_pResourceManager->GetGlobalString( iGSNumber );

		if( pStrGlobalString == NULL )
			continue;

		sprintf( szBuf , pStrGlobalString , pEffectValue->iValue );

		SetContents( TCT_STRING , szBuf , m_cColors[ TCC_BLACK ] );

		bIsExit	=	true;
	}

	if( bIsExit == true )
		SetContents( TCT_LINE );

	//////////////////////////////////////////////////////////////////////////

	//	옵션

	bIsExit	=	false;

	if( g_pGOBManager != NULL )
	{
		SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

		if( pLocalPlayer != NULL )
		{
			SPPlayerStatus*	pLocalPlayerStatus	=	(SPPlayerStatus*)pLocalPlayer->GetStatus();

			if( pLocalPlayerStatus != NULL )
			{
				ITEMMIX_INFO*	pItemMixInfo	=	pLocalPlayerStatus->GetItemMix();

				if( pItemMixInfo != NULL )
				{
					int	iOptMin	=	pItemMix->m_iOptMin + pItemMixInfo->iOptProb * ( pItemMixInfo->iLv - pItemMix->m_iBase_Lv ) * 0.1f;
					int	iOptMax	=	pItemMix->m_iOptMax + pItemMixInfo->iOptProb * ( pItemMixInfo->iLv - pItemMix->m_iBase_Lv ) * 0.1f;

					if( iOptMin < 0 )
						iOptMin = 0;

					if( iOptMax < 0 )
						iOptMax = 0;

					if( iOptMin > 0 && iOptMax > 0 )
					{
						pStrGlobalString = g_pResourceManager->GetGlobalString( 16020001 );

						if( pStrGlobalString != NULL )
						{
							sprintf( szBuf , pStrGlobalString , iOptMin , iOptMax );
							SetContents( TCT_STRING , szBuf , m_cColors[ TCC_BLACK ] );
							bIsExit	=	true;
						}
					}
				}
			}
		}
	}

	if( bIsExit == true )
		SetContents( TCT_SPACE );

	//////////////////////////////////////////////////////////////////////////

	//	옵션 등급

	bIsExit	=	false;
	pStrGlobalString = g_pResourceManager->GetGlobalString( GS_TOOLTIP_ITEM_OPTION_RARE );

	if( pStrGlobalString != NULL )
	{
		if( pItemAttr->m_iOptionClass != 0 )
		{
			sprintf( szBuf , pStrGlobalString , pItemAttr->m_iOptionClass );
			SetContents( TCT_STRING , szBuf , m_cColors[ TCC_ITEM_OPTION_LIMIT ] );
			bIsExit	=	true;
		}
	}

	if( bIsExit == true )
		SetContents( TCT_SPACE );

	//////////////////////////////////////////////////////////////////////////

	//	설명
	if( pItemAttr->m_strDescription.empty() == false )
	{
		SetDesc( pItemAttr->m_strDescription.c_str() );
	}
}

void SPWindowToolTip::ShowToolTip( SPUIUnit* pUIUnit )
{
	if( pUIUnit == NULL || g_pGOBManager->GetLocalPlayer() == NULL )
	{
		Hide();
		return;
	}

	if( pUIUnit->GetStatus( SPUIUnit::STATUS_UI_UNIT_NO_TOOLTIP ) == TRUE )
	{
		Hide();
		return;
	}

	switch( pUIUnit->GetInstanceID() ) 
	{
	case WIID_ITEM:
		{
			SPUIItemUnit*	pUIItemUnit	=	(SPUIItemUnit*)pUIUnit;

			if( pUIItemUnit == NULL )
				return;

			m_pItem->SetItemStatus( pUIItemUnit->GetItemID() );

			SPItemStatus* pItemStatus = m_pItem->GetItemStatus();

			if( pUIItemUnit->GetDuration() == TRUE && pItemStatus != NULL )
			{
				SPItemAttr*	pItemAttr	=	pItemStatus->GetItemAttr();

				if( pItemAttr != NULL )
				{
					if( pItemAttr->GetDurationType() != ITEM_DURATION_NULL )
					{
						pItemStatus->SetLifeTime( pItemAttr->m_iLifeTime * 3600 );
					}
				}
			}

			CreateItemData( pItemStatus );
		}
		break;
	case WIID_SKILL:
		CreateSkillData( pUIUnit );
		break;
	case WIID_QUEST:
		CreateQuestData( pUIUnit );
		break;
	case WIID_NPC_QUEST:
		CreateGOBData( pUIUnit );
		break;
	case WIID_BUFF:
		CreateBuffData( pUIUnit );
		break;
	}
}

int SPWindowToolTip::CreateSkillData( SPUIUnit* pUIUnit )
{
	SPUISkillUnit* pUISkillUnit	=	(SPUISkillUnit*)pUIUnit;

	if( pUISkillUnit == NULL )
		return 1;

	m_strName = "SkillData";
	
	SKILL_INVENTORY_TYPE	eSkillType	=	pUISkillUnit->GetType();

	if( eSkillType == SIT_ACTION || eSkillType == SIT_NULL )
		CreateSkillDataAction( pUISkillUnit );
	else
		CreateSkillDataEtc( pUISkillUnit );

	return 1;
}

void SPWindowToolTip::CreateSkillDataAction( SPUIUnit* pUIUnit )
{
	SPUISkillUnit* pUISkillUnit	=	(SPUISkillUnit*)pUIUnit;

	if( pUISkillUnit == NULL )
		return;

	int					iSkillID			=	pUISkillUnit->GetSkillID();
	SPSkillManager*		pSkillManager		=	SPSkillManager::GetInstance();
	SPActionInterface*	pActionInterface	=	pSkillManager->GetActionInterface( iSkillID );

	if( pActionInterface == NULL )
		return;

	//	타이틀
	m_pTitle->SetWindowText( pActionInterface->GetName() );
	m_pTitle->SetFontColor( m_cColors[ TCC_BLACK ] );

	SKILL_UI_INFO*	pUIInfo	=	pActionInterface->GetUIInfo();

	int	iGSStringNum	=	GS_TOOLTIP_SKILL_INVEN_TYPE + (int)(pUIInfo->eInventoryType);

	if( iGSStringNum == GS_TOOLTIP_SKILL_INVEN_TYPE )
		iGSStringNum++;

	if( pUIInfo != NULL )
		MakeContentsOne( g_pResourceManager->GetGlobalString( iGSStringNum ) , NULL , m_cColors[ TCC_BLACK ] );

	int	iEquipWeapon	=	g_pGOBManager->GetLocalPlayer()->GetWeaponItemSeq();
	int	iMotionType		=	pActionInterface->GetMotionType();

	int				iConsumptionStatTable	=	( iEquipWeapon	*	100 )	+	iMotionType;
	SPMotionStatus*		pMotionStatus		=	pSkillManager->GetMotionStatus( iConsumptionStatTable );

	if( pMotionStatus != NULL )
	{
		MOTION_STATUS_STAT*	pMotionStatusType;
		int					iMotionStatusType;
		int					iRequireStatListCount	=	pMotionStatus->GetRequireStatListCount();
		bool				bIsExit					=	false;

		for( int i = 0 ; i < iRequireStatListCount ; i++ )
		{
			pMotionStatusType	=	pMotionStatus->GetRequireStatList( i );

			if( pMotionStatusType == NULL )
				continue;

			iMotionStatusType	=	(int)pMotionStatusType->eType;

			if( iMotionStatusType == 0 )
				continue;

			SetWindowRequire(	iMotionStatusType , pMotionStatusType->iID ,
								pMotionStatusType->iValue	, 0 , 
								pMotionStatusType->iValue2	, 0 , 0 ,
								GS_TOOLTIP_COMMON_REQUIRE , m_cColors[ TCC_BLACK ] , m_cColors[ TCC_DISABLE ] );

			bIsExit	=	true;
		}

		if( bIsExit == true )
			SetContents( TCT_LINE );
	}

	// 설명
	SetDesc( pUIInfo->strDescription.c_str() );
}

void SPWindowToolTip::CreateSkillDataEtc( SPUIUnit* pUIUnit )
{
	SPUISkillUnit* pUISkillUnit	=	(SPUISkillUnit*)pUIUnit;

	if( pUISkillUnit == NULL )
		return;

	SPPlayer* pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();
	if( pLocalPlayer == NULL )
		return;

	int					iSkillID		=	pUISkillUnit->GetSkillID();
	SPSkillManager*		pSkillManager	=	SPSkillManager::GetInstance();
	SPSkillArchive*		pSkillArchive	=	pLocalPlayer->GetSkillArchive();
	SPSkillActivity*	pSkillActivity	=	pSkillArchive->GetSkillActivity( iSkillID );
	SPSkill*			pSkill			=	NULL;

	if( pSkillActivity != NULL )
		pSkill	=	pSkillActivity->GetSkill();
	else
		pSkill	=	pSkillManager->GetSkill( iSkillID );

	if( pSkill == NULL )
		return;

	//	타이틀
	m_pTitle->SetWindowText( pSkill->GetName() );
	m_pTitle->SetFontColor( m_cColors[ TCC_BLACK ] );

	char			szBuf[_MAX_PATH]	=	{};
	char			szBuf2[_MAX_PATH]	=	{};
	const	char*	pStrFormat			=	NULL;
	const	char*	pStrGlobalString	=	NULL;
	const	char*	pStrGlobalString1	=	NULL;
	const	char*	pStrGlobalString2	=	NULL;

	SKILL_UI_INFO*	pUIInfo	=	pSkill->GetUIInfo();

	pStrGlobalString1 = g_pResourceManager->GetGlobalString( GS_TOOLTIP_SKILL_INVEN_TYPE + (int)(pSkill->GetType()) );

	SPSkillActivityRequire* pActivityRequire	=	pSkill->GetActivityRequire();

	if( pActivityRequire != NULL )
	{
		if( pActivityRequire->m_iItemKind != 0)
		{
			pStrGlobalString2 = g_pResourceManager->GetGlobalString( GS_TOOLTIP_SKILL_REQUIRE_ITEM_KIND + pActivityRequire->m_iItemKind );

			//if( g_pCheckManager->CheckEquipItem( (EQUIP_ID)pActivityRequire->m_iEquipType , pActivityRequire->m_iItemKind ) == false )
			//	m_pWindowRequire->SetFontColor( m_DisableColor );
		}
	}

	MakeContentsOne( pStrGlobalString1 , pStrGlobalString2 , m_cColors[ TCC_BLACK ] );

	//////////////////////////////////////////////////////////////////////////

	int					iSkillLevel			=	0;
	int					iSkillCurLevel		=	0;
	int					iSkillAddLevel		=	0;
	int					iRequireStateCount	=	(int)( pActivityRequire->m_vRequireStateList.size() );
	SKILL_REQUIRE_STAT	stRequireState;

	if( pSkillActivity != NULL )
	{
		iSkillLevel		=	pSkillActivity->GetLevel();
		iSkillCurLevel	=	pSkillActivity->GetCurLevel();
		iSkillAddLevel	=	iSkillCurLevel - iSkillLevel;
	}
	else
	{
		if( pUIInfo != NULL )
		{
			if( strlen( szBuf ) > 0 )
			{
				m_pTitle->SetFontColor( m_cColors[ TCC_SKILL_NOT_LEARN_TITLE ] );
				SetContents( TCT_STRING , szBuf , m_cColors[ TCC_SKILL_NOT_LEARN_CONTENTS ] );
				SetContents( TCT_LINE );
			}

			SetDesc( pUIInfo->strDisableDescription.c_str() );
			return;
		}
	}

	if( strlen( szBuf ) > 0 )
	{
		SetContents( TCT_STRING , szBuf , m_cColors[ TCC_BLACK ] );
		SetContents( TCT_LINE );
	}

	if( iSkillAddLevel <= 0 )
	{
		pStrFormat	=	g_pResourceManager->GetGlobalString( GS_TOOLTIP_SKILL_ADD_LEVEL_0 );

		if( pStrFormat != NULL )
		{
			sprintf( szBuf2 , pStrFormat , iSkillLevel , pSkill->GetMaxLevel() );
		}
	}
	else
	{
		pStrFormat	=	g_pResourceManager->GetGlobalString( GS_TOOLTIP_SKILL_ADD_LEVEL_0_NOT );

		if( pStrFormat != NULL )
		{
			sprintf( szBuf2 , pStrFormat , iSkillLevel , iSkillAddLevel , pSkill->GetMaxLevel() );
		}
	}

	if( strlen( szBuf2 ) > 0 )
	{
		SetContents( TCT_STRING , szBuf2 , m_cColors[ TCC_BLACK ] );
		SetContents( TCT_LINE );
	}

	if( pActivityRequire != NULL )
	{
		for( int i = 0 ; i < iRequireStateCount ; i++ )
		{
			stRequireState	=	pActivityRequire->m_vRequireStateList.at( i );

			SetWindowRequire(	stRequireState.iType	, stRequireState.iID		,
								stRequireState.iValue	, stRequireState.iValueSlv	,
								stRequireState.iValue2	, stRequireState.iValueSlv2	,
								iSkillLevel , GS_TOOLTIP_COMMON_REQUIRE , m_cColors[ TCC_SKILL_LEARN_REQUIRE ] , m_cColors[ TCC_DISABLE ] );
		}

		if( iRequireStateCount > 0 )
			SetContents( TCT_LINE );
	}

	SPCoolTimeManager*	pCoolTimeManager	=	SPCoolTimeManager::GetInstance();

	if( pCoolTimeManager != NULL )
	{
		COOLTIME_INFO*		pCoolTimeInfo		=	pCoolTimeManager->GetCoolTime( pSkill->GetCoolTimeID() );

		if( pCoolTimeInfo != NULL )
		{
			int	iSec	=	pCoolTimeInfo->fCoolTime - pCoolTimeInfo->fAccmulateTime;
			int	iMin	=	iSec	/	60;
			int	iHour	=	iMin	/	60;
			int	iDay	=	iHour	/	24;

			iSec		=	iSec	-	iMin	*	60;
			iMin		=	iMin	-	iHour	*	60;
			iHour		=	iHour	-	iDay	*	24;

			strcpy( szBuf , "" );

			if( iDay > 0 )
			{
				pStrGlobalString = g_pResourceManager->GetGlobalString( GS_TOOLTIP_ITEM_COOLTIME_DAY );
				if(pStrGlobalString)
                    sprintf( szBuf , pStrGlobalString , iDay , iHour );
			}
			else if( iHour > 0 )
			{
				pStrGlobalString = g_pResourceManager->GetGlobalString( GS_TOOLTIP_ITEM_COOLTIME_HOUR );
				if(pStrGlobalString)
					sprintf( szBuf , pStrGlobalString , iHour , iMin );
			}
			else
			{
				if( iMin > 0 )
				{
					pStrGlobalString = g_pResourceManager->GetGlobalString( GS_TOOLTIP_ITEM_COOLTIME_ETC );
					if(pStrGlobalString)
						sprintf( szBuf , pStrGlobalString , iMin , iSec );
				}
			}

			if( strlen( szBuf ) > 0 )
			{
				SetContents( TCT_STRING , szBuf , m_cColors[ TCC_ITEM_COOLTIME ] );
				SetContents( TCT_SPACE );
			}
		}
	}

	if( pUIInfo != NULL )
		SetDesc( pUIInfo->strDescription.c_str() );
}

void SPWindowToolTip::CreateQuestData( SPUIUnit* pUIUnit )
{
	SPUIQuestUnit* pUIQuestUnit	=	(SPUIQuestUnit*)pUIUnit;

	if( pUIQuestUnit == NULL )
		return;

	SPQuestArchive*	pQuestArchive	=	pUIQuestUnit->GetQuestArchive();

	if( pQuestArchive == NULL )
		return;

	SPQuestAttr*	pQuestAttr	=	pQuestArchive->GetQuest( pUIQuestUnit->GetQuestID() );

	if( pQuestAttr == NULL )
		return;

	m_strName = "QuestData";

	SPQuestNpc*	pQuestNpc	=	&pQuestAttr->m_QuestNpc[QUEST_NPC_REWORD];

	//	타이틀
	m_pTitle->SetWindowText( pQuestNpc->m_strName.c_str() );
	m_pTitle->SetFontColor( m_cColors[ TCC_BLACK ] );

	QUEST_UI_SHOW	eUIShow	=	pQuestAttr->GetQuestUIShow();

	char			szBuf[_MAX_PATH]	=	{};
	char			szBuf2[_MAX_PATH]	=	{};
	const	char*	pStrGlobalString	=	NULL;
	const	char*	pStrGlobalString2	=	NULL;

	pStrGlobalString	=	g_pResourceManager->GetGlobalString( GS_TOOLTIP_QUEST_ING );

	if( eUIShow == QUEST_UI_SHOW_COMPLETE || eUIShow == QUEST_UI_SHOW_REWORD )
		pStrGlobalString	=	g_pResourceManager->GetGlobalString( GS_TOOLTIP_QUEST_COMPLETE );

	sprintf( szBuf2 , "%d" , pQuestAttr->m_iRewordCount );

	MakeContentsOne( pStrGlobalString , szBuf2 , m_cColors[ TCC_BLACK ] );

	//////////////////////////////////////////////////////////////////////////
	
	int		iNowTextLine	=	0;
	std::string strCondition;

	for( int i = 0 ; i < 4 ; i++ )
	{
		if( pQuestAttr->m_Require[i].m_bOK == false )
		{
			strCondition.clear();

			pStrGlobalString	=	g_pResourceManager->GetGlobalString( GS_TOOLTIP_QUEST_CONDITION );

			if( pStrGlobalString != NULL )
			{
				strCondition		=	pStrGlobalString;
				strCondition		+=	" : ";
				pStrGlobalString2	=	g_pResourceManager->GetGlobalString( GS_TOOLTIP_QUEST_CLASSTYPE + i );

				if( pStrGlobalString2 != NULL )
					strCondition	+=	pStrGlobalString2;
			}

			if( strCondition.empty() == false )
				SetContents( TCT_STRING , strCondition.c_str() , m_cColors[ TCC_BLACK ] );
		}
	}

	std::string	strDesc;

	strDesc	=	pQuestAttr->m_strQuestName;
	strDesc	+=	"\n";
	strDesc	+=	pQuestNpc->m_strPlace;

	if( strDesc.empty() == false )
		SetDesc( strDesc.c_str() );
}

void SPWindowToolTip::CreateGOBData( SPUIUnit* pUIUnit )
{
	SPUIGOBUnit* pUIQuestUnit	=	(SPUIGOBUnit*)pUIUnit;

	if( pUIQuestUnit == NULL )
		return;

	SPMOBUIInfo*	pMOBUIInfo	=	g_pMOBCluster->GetMOBUIInfo( pUIQuestUnit->GetClassID() );

	if( pMOBUIInfo == NULL )
		return;

	m_strName = "GOBData";

	//	타이틀
	m_pTitle->SetWindowText( pMOBUIInfo->m_strMOBName.c_str() );
	m_pTitle->SetFontColor( m_cColors[ TCC_BLACK ] );

	MakeContentsOne( pMOBUIInfo->m_strMOBName.c_str() , pMOBUIInfo->m_strMOBTip.c_str() , m_cColors[ TCC_BLACK ] );

	//	설명

	if( pMOBUIInfo->m_strMOBDesc.empty() == false )
		SetDesc( pMOBUIInfo->m_strMOBDesc.c_str() );
}

void SPWindowToolTip::CreateBuffData( SPUIUnit* pUIUnit )
{
	SPUIBuffUnit*	pUIBuffUnit	=	(SPUIBuffUnit*)pUIUnit;

	if( pUIBuffUnit == NULL )
		return;

	m_strName = "BuffData";

	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return;

	SPEffectArchive*	pEffectArchive	=	pLocalPlayer->GetEffectArchive();

	if( pEffectArchive == NULL )
		return;

	SPEffectActivity*	pEffectActivity	=	pEffectArchive->GetUniqueKeyActivity( pUIBuffUnit->GetBuffID() );

	if( pEffectActivity == NULL )
		return;

	SPEffect*	pEffect	=	pEffectActivity->GetEffect();

	if( pEffect == NULL )
		return;

	EFFECT_BUFF*	pEffect_Buff	=	pEffect->GetBuff();

	if( pEffect_Buff == NULL )
		return;

	//	타이틀
	m_pTitle->SetWindowText( pEffect_Buff->strName.c_str() );
	m_pTitle->SetFontColor( m_cColors[ TCC_BLACK ] );

	char			szBuf[_MAX_PATH]	=	{};
	const	char*	pStrGlobalString	=	NULL;
	const	char*	pStrGlobalString2	=	NULL;

	pStrGlobalString2 = g_pResourceManager->GetGlobalString( GS_TOOLTIP_BUFF_REMAINTIME );

	float	fRemainTime =	pEffectActivity->GetApplyTime() / 10.0f - pEffectActivity->GetAccmulateTime();

	int		iRemainTime[ 4 ];

	//	0	Day
	//	1	Hour
	//	2	Min
	//	3	Sec

	iRemainTime[ 3 ]	=	(int)( fRemainTime );
	iRemainTime[ 2 ]	=	iRemainTime[ 3 ]	/	60;
	iRemainTime[ 1 ]	=	iRemainTime[ 2 ]	/	60;
	iRemainTime[ 0 ]	=	iRemainTime[ 1 ]	/	24;

	iRemainTime[ 3 ]	=	iRemainTime[ 3 ]	-	iRemainTime[ 2 ] * 60;
	iRemainTime[ 2 ]	=	iRemainTime[ 2 ]	-	iRemainTime[ 1 ] * 60;
	iRemainTime[ 1 ]	=	iRemainTime[ 1 ]	-	iRemainTime[ 0 ] * 24;

	int		iFirst		=	iRemainTime[ 2 ];
	int		iSecond		=	iRemainTime[ 3 ];
	int		iBuffString	=	GS_TOOLTIP_BUFF_REMAINTIME	+	3;
	char	szTimeBuf[MAX_PATH];

	memset( szTimeBuf , 0 , sizeof( szTimeBuf ) );

	for( int i = 2 ; i >= 0 ; i-- )
	{
		if( iRemainTime[ i ] > 0 )
		{
			iFirst	=	iRemainTime[ i ];
			iSecond	=	iRemainTime[ i + 1];

			iBuffString	=	GS_TOOLTIP_BUFF_DAY + i;

			if( iSecond > 0 && i + 1 != 3 )
				sprintf( szTimeBuf , "%s %s" , g_pResourceManager->GetGlobalString( iBuffString ) , g_pResourceManager->GetGlobalString( iBuffString + 1 ) );
			else
				sprintf( szTimeBuf , "%s" , g_pResourceManager->GetGlobalString( iBuffString ) );
		}
	}

	if( strlen( szTimeBuf ) > 0 )
		sprintf( szTimeBuf , szTimeBuf , iFirst , iSecond );
	else
	{
		if( iSecond > 0 )
		{
//			strcpy( szTimeBuf , "1분미만" );
			sprintf( szTimeBuf , "%s" , g_pResourceManager->GetGlobalString( GS_TOOLTIP_BUFF_SEC ) );
		}
	}

	if( fRemainTime > 0.0f )
		MakeContentsOne( pStrGlobalString2 , szTimeBuf , m_cColors[ TCC_BLACK ] );

	//	설명
	if( pEffect_Buff->strDescription.empty() == false )
		SetDesc( pEffect_Buff->strDescription.c_str() );
}

void SPWindowToolTip::SetWindowRequire( int iType , int iID , int iValue1 , int iValueSlv1 , int iValue2 , int iValueSlv2 , int iLevel , int iGSNumber , D3DXCOLOR cDefaltColor , D3DXCOLOR cDisableColor )
{
	if( iType == 0 )
		return;

	int iRequireGlobalStringNum = iGSNumber + iType * 1000 + iID;
	const char*	pStrFormat		=	g_pResourceManager->GetGlobalString( iRequireGlobalStringNum );

	if( pStrFormat == NULL )
		return;

	int	iRequireState1	=	iValue1	+ iValueSlv1	* iLevel;
	int iRequireState2	=	iValue2	+ iValueSlv2	* iLevel;

	char			szBuf		[_MAX_PATH]	=	{};
	char			szBufLine	[_MAX_PATH]	=	{};
	char			szBufTemp	[_MAX_PATH]	=	{};
	const char*		pTemp;
	D3DXCOLOR		FontColor				=	cDefaltColor;

	//	Stat일경우
	if( iType == 1 )
	{
		//	캐릭터 유형
		if( iID == 1 )
		{
			//	공 방 마 탐 순으로 표시!!
			int			iCompareClassType;

			for( int i = 0 ; i < CLASS_MAX ; i++ )
			{
				iCompareClassType	=	1	<<	i;

				if( ( iRequireState1 & iCompareClassType ) == iCompareClassType )
				{
					pTemp	=	g_pResourceManager->GetGlobalString( GS_TOOLTIP_COMMON_CLASS + i );

					if( pTemp != NULL )
					{
						strcat( szBufLine , pTemp );
						strcat( szBufLine , ". " );
					}
				}
			}

			sprintf( szBuf , pStrFormat , szBufLine );
		}
		//	캐릭터 성별
		else if( iID == 2)
		{
			const char* pstrGender	=	g_pResourceManager->GetGlobalString( GS_TOOLTIP_COMMON_GENDER + iRequireState1 );

			if( pstrGender != NULL )
				sprintf( szBuf , pStrFormat , pstrGender );
		}
		else
		{
			if( iValue1 != 0 || iValueSlv1 != 0 )
			{
				pTemp	=	g_pResourceManager->GetGlobalString( GS_TOOLTIP_COMMON_UP_CHAR );
				if( pTemp != NULL )
				{
					sprintf( szBufTemp , pTemp , iRequireState1 );
					strcat( szBufLine , szBufTemp );
				}
			}

			if( ( iValue1 != 0 || iValueSlv1 != 0 ) &&
				( iValue2 != 0 || iValueSlv2 != 0 ) )
			{
				pTemp	=	g_pResourceManager->GetGlobalString( GS_TOOLTIP_COMMON_RANGE_CHAR );
				if( pTemp != NULL )
				{
					strcat( szBufLine , pTemp );
				}
			}

			if( iValue2 != 0 || iValueSlv2 != 0 )
			{
				pTemp	=	g_pResourceManager->GetGlobalString( GS_TOOLTIP_COMMON_DOWN_CHAR );
				if( pTemp != NULL )
				{
					sprintf( szBufTemp , pTemp , iRequireState2 );
					strcat( szBufLine , szBufTemp );
				}
			}

			sprintf( szBuf , pStrFormat , szBufLine );
		}

		if( g_pCheckManager->CheckPCStatus( (CHECK_STATUS)iID , iRequireState1 , iRequireState2 ) == false )
			FontColor	=	cDisableColor;

	}
	//	아이템일경우
	else if( iType == 2 )
	{
		//	갯수체크
		if( iID == 1 )
		{
			int iItemCount	=	g_pCheckManager->GetTotalPCItemCount( iRequireState1 );

			if( iRequireState2 <= iItemCount )
			{
				//	갯수 충족
			}
			else
			{
				//	갯수 불충족
				FontColor	=	cDisableColor;
			}

			SPItemAttr*	pItem;
			g_pItemCluster->GetItemInfo( iRequireState1 , pItem );

			if( pItem != NULL )
				sprintf( szBuf , pStrFormat , pItem->m_strName.c_str() , iRequireState2 );
		}
	}
	//	스킬일경우
	else if( iType == 3 )
	{
		SPSkill*	pSkill	=	SPSkillManager::GetInstance()->GetSkill( iRequireState1 );

		//	스킬 존재 유무
		if( iID == 1 )
		{
			if( g_pCheckManager->CheckPCSkill( iRequireState1 , 2 , 0 ) == false )
				FontColor	=	cDisableColor;

			if( pSkill != NULL )
				sprintf( szBuf , pStrFormat , pSkill->GetName() );
		}
		//	스킬 레벨 체크
		else if( iID == 2 )
		{
			if( g_pCheckManager->CheckPCSkill( iRequireState1 , 2 , iRequireState2 ) == false )
				FontColor	=	cDisableColor;

			if( pSkill != NULL )
				sprintf( szBuf , pStrFormat , pSkill->GetName() , iRequireState2 );
		}

	}
	//	퀘스트일경우
	else if( iType == 4 )
	{
		//	반복 횟수 이하
		if( iID == 1)
		{
			if( g_pCheckManager->CheckPCQuest( iRequireState1 , 1 , iRequireState2 ) == false )
				FontColor	=	cDisableColor;

			pTemp	=	g_pResourceManager->GetGlobalString( GS_TOOLTIP_COMMON_DOWN_CHAR );
			if( pTemp != NULL )
			{
				sprintf( szBufTemp , pTemp , iRequireState2 );
				strcat( szBufLine , szBufTemp );
			}
		}
		//	반복 횟수 초과
		else if( iID == 2)
		{
			if( g_pCheckManager->CheckPCQuest( iRequireState1 , 2 , iRequireState2 ) == false )
				FontColor	=	cDisableColor;

			pTemp	=	g_pResourceManager->GetGlobalString( GS_TOOLTIP_COMMON_UP_CHAR );
			if( pTemp != NULL )
			{
				sprintf( szBufTemp , pTemp , iRequireState1 );
				strcat( szBufLine , szBufTemp );
			}
		}

		SPQuestAttr*	pQuestAttr		=	new SPQuestAttr;

		if( SPQuestManager::GetInstance()->LoadLDTFile( iRequireState1 , pQuestAttr ) == true )
		{
			sprintf( szBuf , pStrFormat , szBufLine );
			sprintf( szBuf , pStrFormat , pQuestAttr->m_strQuestName.c_str() , iRequireState2 );
		}

		SAFE_DELETE( pQuestAttr );
	}
	//	타이틀일 경우
	else if( iType == 5 )
	{
		if( pStrFormat == NULL )
			return;

		if( g_pCheckManager->CheckCondition( iType , iID , iRequireState1 , iRequireState2  ) == false )
			FontColor	=	cDisableColor;

		SPTitleManager*	pTitleManager	=	SPTitleManager::GetInstance();

		if( pTitleManager == NULL )
			return;

		SPTitleArchive*	pTitleArchive	=	pTitleManager->GetTitleArchive();

		if( pTitleArchive == NULL )
			return;

		SPTitleAttr*	pTitleAttr	=	pTitleManager->GetTitleAttr( iRequireState1 );

		if( pTitleAttr == NULL )
			return;

		sprintf( szBuf , pStrFormat , pTitleAttr->GetName() );
	}
	else
		return;

	if( strlen( szBuf ) == 0 )
		return;

	SetContents( TCT_STRING , szBuf , FontColor );
}

void SPWindowToolTip::ClearContents()
{
	for( int i = 0 ; i < MAX_TOOLTIP_CONTENTS_LINE ; ++i )
	{
		m_pContents[ i ]->SetWindowText( NULL );
		m_pContents[ i ]->SetFontColor( m_cColors[ TCC_BLACK ] );
		m_pContents[ i ]->SetFont( FONT_12_NORMAL );

		m_pContents2[ i ]->SetWindowText( NULL );
		m_pContents2[ i ]->SetFontColor( m_cColors[ TCC_BLACK ] );
		m_pContents2[ i ]->SetFont( FONT_12_NORMAL );
	}

	m_pTitle2->SetWindowText( NULL );

	m_iWriteContentPos	=	0;
	m_iWriteContentPos2	=	0;
	m_strName.clear();

	SetDesc( NULL );
}

void SPWindowToolTip::MakeContentsOne( const char* pstrString1 , const char* pstrString2 , D3DXCOLOR cColor )
{
	const char* pstrFormat	=	NULL;
	std::string	strString1;
	std::string strString2;
	char	szBuf[ _MAX_PATH ]	=	{};

	if( pstrString1 == NULL && pstrString2 == NULL )
		return;

	if( pstrString1 != NULL )
		strString1	=	pstrString1;

	if( pstrString2 != NULL )
		strString2	=	pstrString2;

	if( strString1.empty() == false && strString2.empty() == false )
	{
		pstrFormat	=	g_pResourceManager->GetGlobalString( GS_TOOLTIP_COMMON_FIRST_CONTENT_2 );

		if( pstrFormat != NULL )
			sprintf( szBuf , pstrFormat , strString1.c_str() , strString2.c_str() );
	}
	else if( strString1.empty() == false )
	{
		pstrFormat	=	g_pResourceManager->GetGlobalString( GS_TOOLTIP_COMMON_FIRST_CONTENT_1 );

		if( pstrFormat != NULL )
			sprintf( szBuf , pstrFormat , strString1.c_str() );
	}
	else if( strString2.empty() == false )
	{
		pstrFormat	=	g_pResourceManager->GetGlobalString( GS_TOOLTIP_COMMON_FIRST_CONTENT_1 );

		if( pstrFormat != NULL )
			sprintf( szBuf , pstrFormat , strString2.c_str() );
	}

	if( strlen( szBuf ) > 0 )
	{
		SetContents( TCT_STRING , szBuf , cColor );
		SetContents( TCT_LINE );
	}
}

void SPWindowToolTip::SetContents( TOOLTIP_CONTENT_TYPE eContentType , const char* pstrString /* = NULL  */, D3DXCOLOR cColor /* = D3DXCOLOR( 0.0f  , 0.0f , 0.0f , 1.0f ) */, FONT_ENUMERATE eFont /* = FONT_12_NORMAL */ )
{
	if( m_iWriteContentPos < 0 || m_iWriteContentPos > MAX_TOOLTIP_CONTENTS_LINE - 1 )
		return;

	if( eContentType == TCT_SPACE )
	{
		pstrString	=	NULL;
	}
	else if( eContentType == TCT_LINE )
	{
		pstrString	=	NULL;
	}

	m_pContents[ m_iWriteContentPos ]->SetFont( eFont );
	m_pContents[ m_iWriteContentPos ]->SetFontColor( cColor );
	m_pContents[ m_iWriteContentPos ]->SetWindowText( pstrString );

	++m_iWriteContentPos;
}

void SPWindowToolTip::SetContents2( TOOLTIP_CONTENT_TYPE eContentType , const char* pstrString /* = NULL  */, D3DXCOLOR cColor /* = D3DXCOLOR( 0.0f  , 0.0f , 0.0f , 1.0f ) */, FONT_ENUMERATE eFont /* = FONT_12_NORMAL */ )
{
	if( m_iWriteContentPos2 < 0 || m_iWriteContentPos2 > MAX_TOOLTIP_CONTENTS_LINE - 1 )
		return;

	if( eContentType == TCT_SPACE )
	{
		pstrString	=	NULL;
	}
	else if( eContentType == TCT_LINE )
	{
		pstrString	=	NULL;
	}

	m_pContents2[ m_iWriteContentPos2 ]->SetFont( eFont );
	m_pContents2[ m_iWriteContentPos2 ]->SetFontColor( cColor );
	m_pContents2[ m_iWriteContentPos2 ]->SetWindowText( pstrString );

	++m_iWriteContentPos2;
}

void SPWindowToolTip::SetDesc( const char* pstrString , D3DXCOLOR cColor /* = D3DCOLOR_ARGB */ )
{
	if( m_pDesc == NULL )
		return;

	if( pstrString == NULL )
		m_strDesc.clear();
	else
        m_strDesc	=	pstrString;

//	m_pDesc->SetWindowText( pstrString );
	m_pDesc->SetFontColor( cColor );

	if( pstrString == NULL || pstrString[0] == '\0')
		m_pDesc->Hide();
	else
		m_pDesc->Show();
}

void SPWindowToolTip::CalAutoWidth()
{
	int		iCalWidth	=	0;
	RECT	rcWindow;

	if( m_pTitle != NULL )
	{
		m_pTitle->CalTextAutoWidth();
		m_pTitle->GetWindowRect( rcWindow );
		iCalWidth	=	rcWindow.right	-	rcWindow.left;
	}

	int	iMaxContentPos	=	MAX_TOOLTIP_CONTENTS_LINE - 1;
	int	iLastContentPos	=	m_iWriteContentPos;

	if( iLastContentPos < 0 )
		iLastContentPos	=	0;

	if( iLastContentPos > iMaxContentPos )
		iLastContentPos	=	iMaxContentPos;

	int i;

	for( i = 0 ; i < iLastContentPos ; ++i )
	{
		if( m_pContents[ i ] != NULL )
		{
			m_pContents[ i ]->CalTextAutoWidth();
			m_pContents[ i ]->GetWindowRect( rcWindow );
			iCalWidth	=	max( iCalWidth , rcWindow.right	-	rcWindow.left );
		}
	}

	//////////////////////////////////////////////////////////////////////////

	m_iSX	=	m_rcMargin.left	+	m_rcMargin.right	+	iCalWidth;

	if( m_pTitle != NULL )
	{
		m_pTitle->GetWindowRect( rcWindow );
		m_pTitle->SetRectSize( iCalWidth , rcWindow.bottom - rcWindow.top );
	}

	if( m_pContents[ 0 ] != NULL )
	{
		m_pContents[ 0 ]->GetWindowRect( rcWindow );
		m_pContents[ 0 ]->SetRectSize( iCalWidth , rcWindow.bottom - rcWindow.top );
	}

	if( m_pDesc != NULL )
	{
		m_pDesc->GetWindowRect( rcWindow );
		m_pDesc->SetRectSize( iCalWidth , rcWindow.bottom - rcWindow.top );

		m_pDesc->SetWindowText( m_strDesc.c_str() );
	}

	//////////////////////////////////////////////////////////////////////////

	if( m_iWriteContentPos2 > 0 )
	{
		int iXPos	=	m_iSX	+	m_rcMargin2.left;

		if( m_pTitle2 != NULL )
		{
			m_pTitle2->CalTextAutoWidth();
			m_pTitle2->GetWindowRect( rcWindow );
			iCalWidth	=	rcWindow.right	-	rcWindow.left;
		}

		iLastContentPos	=	m_iWriteContentPos2;

		if( iLastContentPos < 0 )
			iLastContentPos	=	0;

		if( iLastContentPos > iMaxContentPos )
			iLastContentPos	=	iMaxContentPos;

		for( i = 0 ; i < iLastContentPos ; ++i )
		{
			if( m_pContents2[ i ] != NULL )
			{
				m_pContents2[ i ]->CalTextAutoWidth();
				m_pContents2[ i ]->GetWindowRect( rcWindow );
				m_pContents2[ i ]->AdjustCoord( iXPos , rcWindow.top - m_iAY );

				iCalWidth	=	max( iCalWidth , rcWindow.right	-	rcWindow.left );
			}
		}

		//////////////////////////////////////////////////////////////////////////

		m_iSX2	=	m_rcMargin2.left	+	m_rcMargin2.right	+	iCalWidth;

		if( m_pTitle2 != NULL )
		{
			m_pTitle2->GetWindowRect( rcWindow );
			m_pTitle2->AdjustCoord( iXPos , rcWindow.top - m_iAY , iCalWidth , rcWindow.bottom - rcWindow.top );
		}

		//////////////////////////////////////////////////////////////////////////

		SPWindow*	pWindow	=	Find( WIID_TOOLTIP_ARROW );

		if( pWindow != NULL )
		{
			pWindow->GetWindowRect( rcWindow );
			pWindow->AdjustCoord( -21 + m_iSX , rcWindow.top - m_iAY );
		}
	}
}

void SPWindowToolTip::CalDescPos()
{
	int	iMaxContentPos	=	MAX_TOOLTIP_CONTENTS_LINE - 1;

	//////////////////////////////////////////////////////////////////////////

	if( m_pDesc->IsShow() == false )
	{
		int	iBeforePos			=	m_iWriteContentPos;

		if( iBeforePos < 0 )
			iBeforePos	=	0;

		if( iBeforePos > iMaxContentPos )
			iBeforePos	=	iMaxContentPos;

		std::string	strBlank	=	m_pContents[ iBeforePos ]->GetWindowText();

		if( strBlank.empty() == true )
		{
			int	iDoubleBeforePos	=	iBeforePos		- 1;

			if( iDoubleBeforePos < 0 )
				iDoubleBeforePos	=	0;

			if( iDoubleBeforePos > iMaxContentPos )
				iDoubleBeforePos	=	iMaxContentPos;

			if( iBeforePos != iDoubleBeforePos )
			{
				std::string	strDoubleBlank	=	m_pContents[ iDoubleBeforePos ]->GetWindowText();

				if( strDoubleBlank.empty() == true )
					--m_iWriteContentPos;

			}
		}
	}

	//////////////////////////////////////////////////////////////////////////

	int	iLastContentPos	=	m_iWriteContentPos;

	if( iLastContentPos < 0 )
		iLastContentPos	=	0;

	if( iLastContentPos > iMaxContentPos )
		iLastContentPos	=	iMaxContentPos;

	//////////////////////////////////////////////////////////////////////////
	
	RECT	rtDesc , rtLastContent;

	m_pDesc->GetWindowRect( rtDesc );
	m_pContents[ iLastContentPos ]->GetWindowRect( rtLastContent );

	int	iXPos	=	rtDesc.left			-	m_iAX;
	int	iYPos	=	rtLastContent.top	-	m_iAY;
	int iWidth	=	rtDesc.right - rtDesc.left;
	int	iHeight	=	m_pDesc->GetTextPixelHeight();

	m_pDesc->AdjustCoord( iXPos , iYPos , iWidth , iHeight );
}

void SPWindowToolTip::CalBGSize()
{
	int	iLastContentPos	=	m_iWriteContentPos	-	1;

	if( iLastContentPos < 0 )
		iLastContentPos	=	0;

	if( iLastContentPos > MAX_TOOLTIP_CONTENTS_LINE - 1 )
		iLastContentPos	=	MAX_TOOLTIP_CONTENTS_LINE - 1;

	RECT	rtBottom;

	if( m_pDesc->IsShow() == false )
	{
		m_pContents[ iLastContentPos ]->GetWindowRect( rtBottom );
	}
	else
	{
		m_pDesc->GetWindowRect( rtBottom );
	}

	rtBottom.bottom	+=	m_rcMargin.bottom;

	int	iYSize	=	rtBottom.bottom - m_iAY;

	SetRectSize( m_iSX , iYSize );
	SetBGRect( m_rcBGDest , m_iAX , m_iAY , m_iAX + m_iSX , m_iAY + m_iSY , m_iStep );

	if( m_iWriteContentPos2 > 0 )
	{
		iLastContentPos	=	m_iWriteContentPos2	-	1;

		if( iLastContentPos < 0 )
			iLastContentPos	=	0;

		if( iLastContentPos > MAX_TOOLTIP_CONTENTS_LINE - 1 )
			iLastContentPos	=	MAX_TOOLTIP_CONTENTS_LINE - 1;

		if( m_pContents2[ iLastContentPos ] != NULL )
			m_pContents2[ iLastContentPos ]->GetWindowRect( rtBottom );

		rtBottom.bottom	+=	m_rcMargin2.bottom;

		iYSize	=	rtBottom.bottom - m_iAY;

		if( m_iSY > iYSize )
			iYSize	=	m_iSY;

		m_iSY2	=	iYSize;

		SetBGRect( m_rcBGDest2 , m_iAX + m_iSX , m_iAY , m_iAX + m_iSX + m_iSX2 , m_iAY + m_iSY2 , m_iStep2 );
	}
}

void SPWindowToolTip::SetBGRect( RECT* prcOutput , int iStartX , int iStartY , int iEndX , int iEndY , int iStep )
{
	//////////////////////////////////////////////////////////////////////////

	prcOutput[ 0 ].left		=	iStartX;
	prcOutput[ 0 ].top		=	iStartY;
	prcOutput[ 0 ].right	=	iStartX + iStep;
	prcOutput[ 0 ].bottom	=	iStartY + iStep;

	prcOutput[ 2 ].left		=	iEndX - iStep;
	prcOutput[ 2 ].top		=	prcOutput[ 0 ].top;
	prcOutput[ 2 ].right	=	iEndX;
	prcOutput[ 2 ].bottom	=	prcOutput[ 0 ].bottom;

	prcOutput[ 1 ].left		=	prcOutput[ 0 ].right;
	prcOutput[ 1 ].top		=	prcOutput[ 0 ].top;
	prcOutput[ 1 ].right	=	prcOutput[ 2 ].left;
	prcOutput[ 1 ].bottom	=	prcOutput[ 0 ].bottom;

	//////////////////////////////////////////////////////////////////////////

	prcOutput[ 6 ].left		=	prcOutput[ 0 ].left;
	prcOutput[ 6 ].top		=	iEndY - iStep;
	prcOutput[ 6 ].right	=	prcOutput[ 0 ].right;
	prcOutput[ 6 ].bottom	=	iEndY;

	prcOutput[ 8 ].left		=	prcOutput[ 2 ].left;
	prcOutput[ 8 ].top		=	prcOutput[ 6 ].top;
	prcOutput[ 8 ].right	=	prcOutput[ 2 ].right;
	prcOutput[ 8 ].bottom	=	prcOutput[ 6 ].bottom;

	prcOutput[ 7 ].left		=	prcOutput[ 6 ].right;
	prcOutput[ 7 ].top		=	prcOutput[ 6 ].top;
	prcOutput[ 7 ].right	=	prcOutput[ 8 ].left;
	prcOutput[ 7 ].bottom	=	prcOutput[ 6 ].bottom;

	//////////////////////////////////////////////////////////////////////////

	prcOutput[ 3 ].left		=	prcOutput[ 0 ].left;
	prcOutput[ 3 ].top		=	prcOutput[ 0 ].bottom;
	prcOutput[ 3 ].right	=	prcOutput[ 0 ].right;
	prcOutput[ 3 ].bottom	=	prcOutput[ 6 ].top;

	prcOutput[ 5 ].left		=	prcOutput[ 2 ].left;
	prcOutput[ 5 ].top		=	prcOutput[ 3 ].top;
	prcOutput[ 5 ].right	=	prcOutput[ 2 ].right;
	prcOutput[ 5 ].bottom	=	prcOutput[ 3 ].bottom;

	prcOutput[ 4 ].left		=	prcOutput[ 3 ].right;
	prcOutput[ 4 ].top		=	prcOutput[ 3 ].top;
	prcOutput[ 4 ].right	=	prcOutput[ 5 ].left;
	prcOutput[ 4 ].bottom	=	prcOutput[ 3 ].bottom;

	//////////////////////////////////////////////////////////////////////////
}

void SPWindowToolTip::LoadToolTipLDT()
{
	SPLDTFile*	pLDTFile	=	NULL;

	g_pResourceManager->GetLDTFromFileName( RES_FNAME_LDT_UITOOLTIP , &pLDTFile );

	if( pLDTFile == NULL )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "----------------------------------->UI ToolTip Load Fail [%s] \n" , RES_FNAME_LDT_UITOOLTIP );
#endif
		return;
	}

	LDT_Field ldtFieldName;
	LDT_Field ldtFieldDesc;
	LDT_Field ldtFieldCutInCheck;

	int				iRecordCount	=	pLDTFile->GetItemCount();
	int				iItemID;
	TOOLTIP_DATA	stToolTipData;

	for( int iRecord = 0 ; iRecord < iRecordCount ; iRecord++ )
	{
		iItemID = pLDTFile->GetPrimaryKey( iRecord );

		pLDTFile->GetFieldFromLable( iItemID, "_Name",			ldtFieldName		);
		pLDTFile->GetFieldFromLable( iItemID, "_Description",	ldtFieldDesc		);
		pLDTFile->GetFieldFromLable( iItemID, "_CutIn_Check",	ldtFieldCutInCheck	);

		stToolTipData.m_strName		=	ldtFieldName.uData.pValue;
		stToolTipData.m_strDesc		=	ldtFieldDesc.uData.pValue;

		if( ldtFieldCutInCheck.uData.lValue == 1 )
			stToolTipData.m_bCutInCheck	=	true;
		else
			stToolTipData.m_bCutInCheck	=	false;

		m_mToolTip.insert( STD_MAP_TOOLTIP::value_type( iItemID , stToolTipData ) );
	}

	SAFE_RELEASE(pLDTFile);
}
