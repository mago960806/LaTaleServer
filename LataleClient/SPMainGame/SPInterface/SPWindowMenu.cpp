#include "SPCommon.h"
#include "SPUtil.h"

#include "SPMainGameDEF.H"
#include "SPGameObjectDEF.h"

#include "SPSubGameManager.h"
#include "SPGameObject.h"
#include "SPManager.h"
#include "SPMouseCursor.h"

#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"
#include "SockWnd.h"
#include "SPNetworkManager.h"
#include "LT_Error.h"

////
#include "SPStage.h"
#include "SPStageManager.h"

#include "SPRenderModelDEF.h"
#include "SPGameObjectDef.h"
#include "SPGameObject.h"
#include "SPGOBStatus.h"
#include "SPGOBManager.h"
#include "SPPlayerStatus.h"
#include "SPPlayerStatusModel.h"
#include "SPPlayerInvenArchive.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGOBCoordPhysics.h"
#include "SPPlayer.h"

#include "SPWindowDEF.h"
#include "SPInterfaceManager.h"
#include "SPWindow.h"

#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPWindowButtonEx.h"
#include "SPWindowMenuBar.h"
#include "SPWindowMenu.h"
#include "SPLocalizeManager.h"
#include "GlobalDefines_Share.h"


const char* MAINMENU_IMAGE_FILE = "DATA/INTERFACE/CONCEPT/UI100.PNG";
const char* MAINMENU_IMAGE_FILE2 = "DATA/INTERFACE/CONCEPT/UI101.PNG";

SPWindowMenu::SPWindowMenu(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_MENU, InstanceID, iX, iY, iCX, iCY, pParent)	
, m_pPopup(NULL)
{
	Init();
}

SPWindowMenu::~SPWindowMenu()
{
	Clean();
}

void SPWindowMenu::Init()
{	
	//g_pVideo->GetTextureMgr()->LoadTexture( MAINMENU_IMAGE_FILE , &m_pImageNormal);	
	//SetRect(&m_rcBodySrc, 416, 385, 418, 415);	
	
	//SetImage(MAINMENU_IMAGE_FILE2 , 198, 175);

	ReposOwnImage();

	SPWindowStatic* pStatic = NULL;
	SPWindowButton* pButton = NULL;
	SPWindowStatic* pCenter = NULL;
	SPWindowStatic* pSkin = NULL;
	//pButton = new SPWindowButton( WIID_MENU_ITEM, 7, 5, 26, 30, this);	//
	//pButton->Show();	
	//pButton->SetImage		(MAINMENU_IMAGE_FILE,	2,	389);	
	//pButton->SetImageHit	(MAINMENU_IMAGE_FILE,	2,	423);	
	//pButton->SetImagePush	(MAINMENU_IMAGE_FILE,	2,	457);	
	//pButton->SetImageDisable(MAINMENU_IMAGE_FILE,	2,	389);	
	//pButton = NULL;
	//
	//pButton = new SPWindowButton( WIID_MENU_EQUIP, 34, 5, 26, 30, this); //
	//pButton->Show();
	//pButton->SetImage		(MAINMENU_IMAGE_FILE,	29,	389);	
	//pButton->SetImageHit	(MAINMENU_IMAGE_FILE,	29,	423);	
	//pButton->SetImagePush	(MAINMENU_IMAGE_FILE,	29,	457);	
	//pButton->SetImageDisable(MAINMENU_IMAGE_FILE,	29,	389);	
	//pButton = NULL;

	//pButton = new SPWindowButton( WIID_MENU_STATUS, 61, 5, 26, 30, this); //
	//pButton->Show();	
	//pButton->SetImage		(MAINMENU_IMAGE_FILE,	56,	389);	
	//pButton->SetImageHit	(MAINMENU_IMAGE_FILE,	56,	423);	
	//pButton->SetImagePush	(MAINMENU_IMAGE_FILE,	56,	457);	
	//pButton->SetImageDisable(MAINMENU_IMAGE_FILE,	56,	389);	
	//pButton = NULL;

	//
	//pButton = new SPWindowButton( WIID_MENU_SKILL, 88, 5, 26, 30, this); //
	//pButton->Show();
	//pButton->SetImage		(MAINMENU_IMAGE_FILE,	83,	389);	
	//pButton->SetImageHit	(MAINMENU_IMAGE_FILE,	83,	423);	
	//pButton->SetImagePush	(MAINMENU_IMAGE_FILE,	83,	457);	
	//pButton->SetImageDisable(MAINMENU_IMAGE_FILE,	83,	389);	
	//pButton = NULL;
	//

	//pButton = new SPWindowButton( WIID_MENU_QUEST, 115, 5, 26, 30, this); //
	//pButton->Show();	
	//pButton->SetImage		(MAINMENU_IMAGE_FILE,	110,	389);	
	//pButton->SetImageHit	(MAINMENU_IMAGE_FILE,	110,	423);	
	//pButton->SetImagePush	(MAINMENU_IMAGE_FILE,	110,	457);	
	//pButton->SetImageDisable(MAINMENU_IMAGE_FILE,	110,	389);	
	//pButton = NULL;

	//pButton = new SPWindowButton( WIID_MENU_TEAM, 142, 5, 26, 30, this); //
	//pButton->Show();
	//pButton->SetImage		(MAINMENU_IMAGE_FILE,	137,	389);	
	//pButton->SetImageHit	(MAINMENU_IMAGE_FILE,	137,	423);	
	//pButton->SetImagePush	(MAINMENU_IMAGE_FILE,	137,	457);	
	//pButton->SetImageDisable(MAINMENU_IMAGE_FILE,	137,	389);	
	//pButton = NULL;

	//pButton = new SPWindowButton( WIID_MENU_FRIEND, 169, 5, 26, 30, this); //
	//pButton->Show();
	//pButton->SetImage		(MAINMENU_IMAGE_FILE,	164,	389);	
	//pButton->SetImageHit	(MAINMENU_IMAGE_FILE,	164,	423);	
	//pButton->SetImagePush	(MAINMENU_IMAGE_FILE,	164,	457);	
	//pButton->SetImageDisable(MAINMENU_IMAGE_FILE,	164,	389);	
	//pButton = NULL;

	//pButton = new SPWindowButton( WIID_MENU_OPTION, 196, 5, 26, 30, this); //
	//pButton->Show();	
	//pButton->SetImage		(MAINMENU_IMAGE_FILE,	191,	389);	
	//pButton->SetImageHit	(MAINMENU_IMAGE_FILE,	191,	423);	
	//pButton->SetImagePush	(MAINMENU_IMAGE_FILE,	191,	457);	
	//pButton->SetImageDisable(MAINMENU_IMAGE_FILE,	191,	389);	
	//pButton = NULL;

	//pButton = new SPWindowButton( WIID_MENU_HELP, 223, 5, 26, 30, this); //
	//pButton->Show();	
	//pButton->SetImage		(MAINMENU_IMAGE_FILE,	218,	389);	
	//pButton->SetImageHit	(MAINMENU_IMAGE_FILE,	218,	423);	
	//pButton->SetImagePush	(MAINMENU_IMAGE_FILE,	218,	457);	
	//pButton->SetImageDisable(MAINMENU_IMAGE_FILE,	218,	389);	
	//pButton = NULL;	

	//pButton = new SPWindowButton(WIID_MENU_EXIT, 250, 5, 26, 30, this); //
	//pButton->Show();	
	//pButton->SetImage		(MAINMENU_IMAGE_FILE,	245,	389);	
	//pButton->SetImageHit	(MAINMENU_IMAGE_FILE,	245,	423);	
	//pButton->SetImagePush	(MAINMENU_IMAGE_FILE,	245,	457);	
	//pButton->SetImageDisable(MAINMENU_IMAGE_FILE,	245,	389);	
	//pButton = NULL;

	if( SPLocalizeManager::GetInstance()->IsEnableValue( LV_LOTTERY ) == true )
	{
		pButton = new SPWindowButton( WIID_MENU_LOTTERY, -6, 0, 98, 28, this); //
		pButton->Show();	
		//pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI_Button_001.PNG",	1,	113);	
		//pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI_Button_001.PNG",	1,	142);	
		//pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI_Button_001.PNG",	1,	171);	
		//pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI_Button_001.PNG",	1,	200);	
		pButton->SetImage		("DATA/INTERFACE/CONCEPT/CN_UI100.PNG",	1,	260);	
		pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/CN_UI100.PNG",	1,	289);	
		pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/CN_UI100.PNG",	1,	318);	
		pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CN_UI100.PNG",	1,	347);	
		pButton = NULL;	
	}

	pButton = new SPWindowButton( WIID_MENU_CACHESHOP, 90, 0, 98, 28, this); //
	pButton->Show();	
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI104.PNG",	1,	186);	
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI104.PNG",	1,	215);	
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI104.PNG",	1,	244);	
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI104.PNG",	1,	273);	
	pButton = NULL;	

	pButton = new SPWindowButton( WIID_MENU_POPUP, 188, 0, 96, 28, this); //
	pButton->Show();	
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI104.PNG",	100,	186);	
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI104.PNG",	100,	215);	
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI104.PNG",	100,	244);	
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI104.PNG",	100,	273);	
	pButton = NULL;		

	//	Auto Y Size
	m_pPopup = new SPWindowMenuBar( WIID_MENUBAR , 124, 0 , 160 , 0 , this );
	
	m_fLimitTime		=	20.0f;
	m_fAccmulateTime	=	0.0f;
}

void SPWindowMenu::Clean()
{
	SPWindow::Clean();
}

void SPWindowMenu::RefreshRelationCoord()
{
	SPWindow::RefreshRelationCoord();
	ReposOwnImage();
}

void SPWindowMenu::ReposOwnImage()
{
	SetRect(&m_rcCalcHead, m_iAX, m_iAY, m_iAX+2, m_iAY+30);	
	SetRect(&m_rcCalcBody, m_iAX+2, m_iAY, m_iAX+2+307, m_iAY+30);	
	SetRect(&m_rcCalcTail, m_iAX+309, m_iAY, m_iAX+309+2, m_iAY+30);
}


void SPWindowMenu::Process(float fTime)
{
	SPWindow::Process(fTime);
	return;

	// AJJIYA [9/9/2005]
	// 임시임 나중에 다시 수정 요망!!!
//	SPWindow::Process(fTime);

	//if(!m_bShow) return;

	//m_fAccmulateTime	+=	fTime;

	//if( m_fAccmulateTime >= m_fLimitTime )
	//{
	//	BYTE	Red		=	(BYTE)m_RGBA;
	//	BYTE	Green	=	(BYTE)( ((WORD)(m_RGBA)) >> 8 );
	//	BYTE	Blue	=	(BYTE)( (m_RGBA) >> 16 );
	//	BYTE	Alpha	=	(BYTE)( (m_RGBA) >> 24 );

	//	Alpha--;

	//	m_RGBA	=	RGBA( Red , Green , Blue , Alpha );

	//	if( Alpha <= 0 )
	//	{
	//		m_RGBA	=	RGBA( Red , Green , Blue , 255 );
	//		Hide();
	//	}
	//}

	//std::vector<SPWindow*>::iterator Iter = m_vpChildWindow.begin();
	//for(;Iter != m_vpChildWindow.end(); ++Iter) {
	////	(*Iter)->SPSendMessage( SPIM_SET_COLOR , (WPARAM)m_RGBA );
	//	(*Iter)->Process(fTime);
	//}
}

void SPWindowMenu::Render(float fTime)
{
	SPWindow::RenderReverse(fTime);
	//g_pVideo->Flush();
	//if(m_pImageNormal) {
	//	m_pImageNormal->RenderTexture(&m_rcCalcHead, &m_rcHeadSrc);
	//	m_pImageNormal->RenderTexture(&m_rcCalcBody, &m_rcBodySrc);
	//	m_pImageNormal->RenderTexture(&m_rcCalcTail, &m_rcTailSrc);
	//}	
	//g_pVideo->Flush();

	//std::vector<SPWindow*>::iterator Iter = m_vpChildWindow.begin();
	//for(;Iter != m_vpChildWindow.end(); ++Iter) {
	//	(*Iter)->Render(fTime);
	//}
}

void SPWindowMenu::Show(bool bWithChild)
{	
	SPWindow::Show(bWithChild);
	m_pPopup->Hide();

	if(g_pGOBManager->GetLocalPlayer()) {
		int iStage = g_pGOBManager->GetLocalPlayer()->GetCurStageID();
		int iMapGroup = g_pGOBManager->GetLocalPlayer()->GetCurMapGroupID();
		//[2006/3/17] - 이벤트 맵 예외처리 채널이동 버튼을 막는다
		SPStage* pStage = g_StageManager.GetCurStage(iStage);
		if(pStage) {
			SPMapGroup* pMapGroup = pStage->GetMapGroup(iMapGroup);
			if(pMapGroup && (pMapGroup->IsMapGroupType(GROUP_TYPE_EVENT) || pMapGroup->IsMapGroupType(GROUP_TYPE_INDUN))) {
				g_pInterfaceManager->SPChildWndSendMessage(WIID_EXIT, SPIM_EXIT_CHANNEL_ENABLE, 0, 0);
			}
			else {
				g_pInterfaceManager->SPChildWndSendMessage(WIID_EXIT, SPIM_EXIT_CHANNEL_ENABLE, 1, 0);
			}

			if(SPLocalizeManager::GetInstance()->IsEnableValue(LV_CASHSHOP) && 
				pMapGroup && pMapGroup->IsMapGroupType(GROUP_TYPE_CASH)) {
					g_pInterfaceManager->SPChildWndSendMessage(WIID_MENU, SPIM_SET_CASH_ENABLE, 1, 0);
				}
			else {
				g_pInterfaceManager->SPChildWndSendMessage(WIID_MENU, SPIM_SET_CASH_ENABLE, 0, 0);
			}
		}
	}
}

void SPWindowMenu::Hide()
{
	SPWindow::Hide();	
}



SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowMenu )
SPIMESSAGE_COMMAND(	SPIM_CURSOR_ENTER,		OnCursorEnter	)
SPIMESSAGE_COMMAND(	SPIM_CURSOR_OUT,		OnCursorOut		)
SPIMESSAGE_COMMAND(	SPIM_LBUTTON_UP,		OnLButtonUp		)	
SPIMESSAGE_COMMAND(	SPIM_LBUTTON_DOWN,		OnLButtonDown	)
SPIMESSAGE_COMMAND( SPIM_SET_CASH_ENABLE,	OnCashEnable	)
SPIMESSAGE_COMMAND( SPIM_MENU_POPUP,		OnShowPopup		)
SPIMESSAGE_COMMAND( SPIM_CASHSHOP_ENTER,	OnCashShopEnter	)

//SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP, WIID_MENU_ITEM,		OnItem		)
//SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP, WIID_MENU_EQUIP,		OnEquip		)
//SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP, WIID_MENU_SKILL,		OnSkill		)
//SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP, WIID_MENU_STATUS,		OnStatus	)
//SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP, WIID_MENU_QUEST,		OnQuest 	)
//SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP, WIID_MENU_TEAM,		OnTeam		)
//SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP, WIID_MENU_FRIEND,		OnFriend	)
//SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP, WIID_MENU_OPTION,		OnOption 	)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP, WIID_MENU_CACHESHOP,	OnCacheShop )
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP, WIID_MENU_POPUP,		OnMenuPopUp )
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP, WIID_MENU_LOTTERY,		OnLottery )
//SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP, WIID_MENU_EXIT,		OnExit		)

//SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP, WIID_MENU_HELP,		OnHelp		)

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)


int SPWindowMenu::OnCursorEnter( WPARAM, LPARAM )
{
	return 0;
}

int SPWindowMenu::OnCursorOut( WPARAM, LPARAM )
{
	return 0;
}

int SPWindowMenu::OnLButtonDown( WPARAM, LPARAM )
{
	return 0;
}

int SPWindowMenu::OnLButtonUp( WPARAM, LPARAM )
{
	return 0;
}

int SPWindowMenu::OnCashEnable( WPARAM wParam, LPARAM lParam)
{
	bool bEnable = (bool)wParam;
	SPWindow* pWindow = Find(WIID_MENU_CACHESHOP);
	if(pWindow) {
		pWindow->SetEnable(bEnable);
	}

	return 1;
}

int SPWindowMenu::OnShowPopup( WPARAM wParam, LPARAM lParam)
{
	if(m_pPopup && m_pPopup->IsShow()) {
		m_pPopup->Hide();
		return 2;
	}
	
	return 1;
}

int SPWindowMenu::OnCashShopEnter( WPARAM wParam , LPARAM lParam )
{
	// 캐시샵에서 친구리스트를 사용하기 위해 친구 리스트 호출[2/13/2007 AJJIYA]
	CPacket	FriendPacket;
	FriendPacket.AddUINT32( FRIEND_CS_GETLIST );
	( g_pNetworkManager->GetPacketHandler() )->PostMsg( &FriendPacket );
	//////////////////////////////////////////////////////////////////////////

	if( g_pSubGameManager )
		g_pSubGameManager->SPPerformMessage(SPGM_CASHSHOP_IN_REQUEST, 1, 0);

	return 1;
}

//////////////////////////////////////////////////////////////////////////
//int SPWindowMenu::OnItem( WPARAM, LPARAM )
//{
//	g_pInterfaceManager->ShowWindow(WIID_ITEM, SP_NONE, SP_TOGGLE_ON);
//	
//	return 1;
//}
//
//int SPWindowMenu::OnEquip( WPARAM, LPARAM )
//{
//	g_pInterfaceManager->ShowWindow(WIID_EQUIP, SP_NONE, SP_TOGGLE_ON);
//	
//	return 1;
//}
//
//int SPWindowMenu::OnSkill( WPARAM, LPARAM )
//{
//	g_pInterfaceManager->ShowWindow(WIID_SKILL, SP_NONE, SP_TOGGLE_ON);
//	
//	return 1;
//}
//
//int SPWindowMenu::OnStatus( WPARAM, LPARAM )
//{
//	g_pInterfaceManager->ShowWindow(WIID_STATUS, SP_NONE, SP_TOGGLE_ON);
//	
//	return 1;
//}
//
//int SPWindowMenu::OnQuest( WPARAM, LPARAM )
//{
//	g_pInterfaceManager->ShowWindow(WIID_QUEST, SP_NONE, SP_TOGGLE_ON);
//	
//	return 1;
//}
//
//int SPWindowMenu::OnTeam( WPARAM, LPARAM )
//{
//	g_pInterfaceManager->ShowWindow(WIID_TEAM, SP_NONE, SP_TOGGLE_ON);
//	
//	return 1;
//}
//
//int SPWindowMenu::OnFriend( WPARAM, LPARAM )
//{
//	g_pInterfaceManager->ShowWindow(WIID_FRIEND, SP_NONE, SP_TOGGLE_ON);
//	
//	return 1;
//}
//
//int SPWindowMenu::OnOption( WPARAM, LPARAM )
//{
//	g_pInterfaceManager->ShowWindow(WIID_OPTION, SP_NONE, SP_TOGGLE_ON);
//	
//	return 1;
//}

int SPWindowMenu::OnCacheShop( WPARAM, LPARAM )
{
	m_pPopup->Hide();

	OnCashShopEnter( NULL , NULL );

	return 1;
}

//int SPWindowMenu::OnExit( WPARAM, LPARAM )
//{
//	g_pInterfaceManager->ShowWindow(WIID_EXIT, SP_NONE, SP_TOGGLE_ON);
//	
//	return 1;
//}
//	
//int SPWindowMenu::OnHelp( WPARAM, LPARAM )
//{
//	g_pInterfaceManager->ShowWindow(WIID_HELP, SP_NONE, SP_TOGGLE_ON);
//
//	return 1;
//}
//
//

int SPWindowMenu::OnMenuPopUp( WPARAM, LPARAM )
{
	if(m_pPopup) {
		if(m_pPopup->IsShow())
			m_pPopup->Hide();
		else 
			m_pPopup->Show();
	}

	return 1;
}

int SPWindowMenu::OnLottery( WPARAM, LPARAM )
{
	if( SPLocalizeManager::GetInstance()->IsEnableValue( LV_LOTTERY ) )
	{
		if( m_pPopup != NULL )
			m_pPopup->Hide();

		//[xialin 2008/10/13] edit >>>
		//g_pInterfaceManager->ShowWindow(WIID_LOTTERY, SP_NONE, SP_TOGGLE_ON);
		if( g_pSubGameManager )
			g_pSubGameManager->SPPerformMessage(SPGM_LOTTREY_IN_REQUEST, 1, 0);
		//[xialin 2008/10/13] edit <<<
	}

	return 1;
}
