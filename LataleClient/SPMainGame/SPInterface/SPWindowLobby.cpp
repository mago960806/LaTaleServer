
#include <vector>
#include <string>
#include <map>

#include "SPCommon.h"
#include "SPUtil.h"

#include "SPMainGameDEF.H"
#include "SPSubGameManager.h"
#include "SPMainGameManager.h"
#include "SPGameObjectDEF.h"

#include "SPGameObject.h"
#include "SPManager.h"
#include "SPMouseCursor.h"
#include "SPIMEPool.h"

#include "SPWindowDEF.h"
#include "SPSubGameManager.h"
#include "SPInterfaceManager.h"

#include "SPWindow.h"
#include "SPWindowEdit.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"
//#include "SPWindowList.h"
#include "SPWindowSlider.h"
#include "SPWindowLobby.h"
#include "SPNoticeBox.h"

#include "SPResourceDef.h"
#include "SPDebug.h"

#include "SPRenderModelDEF.h"
#include "SPRenderModel.h"
#include "SPGOBClusterDef.h"
#include "SPGOBCoordPhysics.h"
#include "SPGOBStatus.h"
#include "SPPlayerStatusModel.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGOBModelUnit.h"
#include "SPAvatarModelUnit.h"

#include "SPPlayer.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"

#include "SPItemAttr.h"
//#include "SPItem.h"
#include "SPItemCluster.h"			//[2005/6/16] - jinhee

#include "SPCommandConvert.h"
#include "SPImageProcessor.h"
#include "SPWindowPresentation.h"
#include "SPWindowToolTip.h"

#include "SPAbuseFilter.h"
#include "SPLocalizeManager.h"
#include "GlobalDefines_Share.h"
#include "_Error_log.h"

#define _SHOW_USE_WEAPON_	1

using namespace std;

//------------------------------------------------------------------------------------
SPWindowLobby::SPWindowLobby(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
, m_pkCard		(NULL)
, m_pkStatusSkin(NULL)
, m_pSlider		(NULL)
{	
	Init();
}

//------------------------------------------------------------------------------------
SPWindowLobby::SPWindowLobby(INSTANCE_ID InstanceID,
	int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_LOBBY_SERVER, InstanceID, iX, iY, iCX, iCY, pParent)	
, m_pkCard		(NULL)
, m_pkStatusSkin(NULL)
, m_pSlider		(NULL)
{	
	Init();
}

//------------------------------------------------------------------------------------
SPWindowLobby::~SPWindowLobby()
{
	Clean();
}

//------------------------------------------------------------------------------------
void SPWindowLobby::Clean()
{
	int i;

	SAFE_RELEASE(m_pkCard);
	SAFE_RELEASE(m_pkStatusSkin);
	
	ClearChracterList();
	SAFE_DELETE(m_pkCreateCharacter);
	
	for( i = 0; i < 2; i++ )
	{
		m_vHairID[i].clear();
		m_vHairColorID[i].clear();
		m_vExpressionID[i].clear();
		m_vExpressionColorID[i].clear();
		m_vSkinID[i].clear();
		m_vBlouseID[i].clear();
		m_vPantsID[i].clear();
		m_vstrHair[i].clear();
		m_vstrHairColor[i].clear();
		m_vstrExpression[i].clear();
		m_vstrExpressionColor[i].clear();
		m_vstrSkin[i].clear();
		m_vstrBlouse[i].clear();
		m_vstrPants[i].clear();
	}

	ClearImageProcessor();

	m_pCurServerInfo = NULL;
	m_bIsSlide = false ;
	SPWindow::Clean();
}

//------------------------------------------------------------------------------------
void SPWindowLobby::ClearImageProcessor()
{
	vector< SPImageProcessor* >::iterator iter = m_vImageProcessor.begin();
	while( iter != m_vImageProcessor.end() )
	{
		delete (*iter);
		iter++;
	}
	m_vImageProcessor.clear();
}

//------------------------------------------------------------------------------------
void SPWindowLobby::Init()
{
	for( int i = 0; i < 3; i++ )
		m_pkMyCharacter[i] = NULL;

	m_bCharacterNameSuccess = true;
	m_pkCreateCharacter = NULL;
	m_bIsSlide = false ;

	InitServerListWindow();
	InitChannelListWindow();
	InitCharWindow();
	InitCharCreateWindow();

	CreateDialogBox();
	//ShowLobbyType(0);
	m_iImageProcessorType = -1;
	m_iType = -1;
	m_iNextType = -1;
	m_iCardProcessingCompleteCount = 0;
}

//------------------------------------------------------------------------------------
void SPWindowLobby::CreateDialogBox()
{
	DeleteDialogBox();

	SPWindowStatic* pkWindow, *pkChildWindow;
	SPWindowButton* pkButton;
	SPWindowEdit* pkEdit;

	pkWindow = new SPWindowStatic(WIID_LOBBY_DIALOG_JUMIN, 144, 236, 512, 128, this);
	pkWindow->SetImage("DATA/LOBBY/RB002.PNG", 0, 129);
	pkWindow->Hide();

	pkButton = new SPWindowButton(WIID_LOBBY_DIALOG_JUMIN_OK, 359, 84, 64, 26, pkWindow);
	pkButton->SetImageHit		("DATA/LOBBY/RB002.PNG", 1,		258);
	pkButton->SetImagePush		("DATA/LOBBY/RB002.PNG", 66,	258);
	pkButton->SetImageDisable	("DATA/LOBBY/RB002.PNG", 131,	258);

	pkButton = new SPWindowButton(WIID_LOBBY_DIALOG_JUMIN_CANCEL, 431, 84, 64, 26, pkWindow);
	pkButton->SetImageHit		("DATA/LOBBY/RB002.PNG", 196, 258);
	pkButton->SetImagePush		("DATA/LOBBY/RB002.PNG", 261, 258);

	pkEdit = new SPWindowEdit(WIID_LOBBY_DIALOG_JUMIN_EDIT, 197, 73, 118, 14, pkWindow);
	pkEdit->SetFontColor(D3DXCOLOR(0.7058f, 0.2470f, 0.3176f, 1.0f));
	pkEdit->SetLimitText(32);
	//pkEdit->SetPassword(true);

	pkWindow = new SPWindowStatic(WIID_LOBBY_DIALOG_INFOMATION, 144, 236, 512, 128, this);
	pkWindow->SetImage("DATA/LOBBY/RB002.PNG", 0, 0);
	pkWindow->Hide();

	pkButton = new SPWindowButton(WIID_LOBBY_DIALOG_INFOMATION_OK, 431, 84, 64, 26, pkWindow);
	pkButton->SetImageHit	("DATA/LOBBY/RB002.PNG", 1, 258);
	pkButton->SetImagePush	("DATA/LOBBY/RB002.PNG", 66, 258);

	pkChildWindow = new SPWindowStatic(WIID_LOBBY_DIALOG_INFOMATION_LIST, 13, 13, 485, 66, pkWindow);
	pkChildWindow->SetFormat(DT_CENTER | DT_VCENTER);
	pkChildWindow->SetMultiLine(true, 2);
	pkChildWindow->SetFontColor(D3DXCOLOR(0.3294f, 0.4039f, 0.6509f, 1.0f));
	pkChildWindow->Hide();
}

//------------------------------------------------------------------------------------
void SPWindowLobby::DeleteDialogBox()
{
	SPWindow* pkWindow;
	pkWindow = Find(WIID_LOBBY_DIALOG_JUMIN, true);
	CleanChild(pkWindow, true);

	pkWindow = Find(WIID_LOBBY_DIALOG_INFOMATION, true);
	CleanChild(pkWindow, true);
}



bool SPWindowLobby::SetInfomationDialog(bool bShow, int iNoticeType, const TCHAR* szMsg)
{
	//////////////////////////////////////////////////////////////////////////
	// Old Version
	//SPWindow* pkDialog, *pkChildWindow;

	//pkDialog = Find(WIID_LOBBY_DIALOG_INFOMATION, true);
	//if( bShow )
	//{
	//	pkDialog->Show();

	//	pkChildWindow = (SPWindowList*)pkDialog->Find(WIID_LOBBY_DIALOG_INFOMATION_LIST, true);
	//	pkChildWindow->SetWindowText(szMsg);
	//	g_pInterfaceManager->SetModalWindow(pkDialog);
	//}
	//else
	//{
	//	pkDialog->Hide();
	//	g_pInterfaceManager->SetModalWindow(NULL);
	//}

	//////////////////////////////////////////////////////////////////////////
	// New Version
	if( bShow )
	{
		g_pInterfaceManager->ForceReply() ;

		switch(iNoticeType)
		{
		case NOTICE_TYPE_MSG:
			return g_pInterfaceManager->SetNotice(szMsg, this, NOTICE_TYPE_MSG) ;
			break ;

		case NOTICE_TYPE_YES:
			return g_pInterfaceManager->SetNotice(szMsg, this, NOTICE_TYPE_YES) ;
			break ;
		}
	}
	else
	{
		g_pInterfaceManager->ForceReply() ;
	}
	
	return false ;
}


//------------------------------------------------------------------------------------
void SPWindowLobby::EnableChildWindow(int iLobbyType, bool bEnable)
{
	SPWindowButton* pkButton;
	switch( iLobbyType )
	{
	case 0:
		break;

	case 1:
		{
			pkButton = (SPWindowButton*)Find(WIID_LOBBY_CHAR_MENU_SELECT, true);
			if( pkButton )
				pkButton->SetEnable(bEnable);

			pkButton = (SPWindowButton*)Find(WIID_LOBBY_CHAR_MENU_CREATE, true);
			if( pkButton )
				pkButton->SetEnable(bEnable);

			pkButton = (SPWindowButton*)Find(WIID_LOBBY_CHAR_MENU_DELETE, true);
			if( pkButton )
				pkButton->SetEnable(bEnable);

			pkButton = (SPWindowButton*)Find(WIID_LOBBY_CHAR_BACK, true);
			if( pkButton )
				pkButton->SetEnable(bEnable);
		}
		break;
	}
}

//------------------------------------------------------------------------------------
void SPWindowLobby::SetLobbyType(int iType)
{
	m_iType = iType;
}

//------------------------------------------------------------------------------------
void SPWindowLobby::ShowLobbyType(int iType)
{
	
	g_pInterfaceManager->SetCursorWindow(NULL);
	//g_pInterfaceManager->SetFocusWindow(NULL);
	g_pInterfaceManager->SetActiveWindow(NULL);
	g_pInterfaceManager->SetModalWindow(NULL);

	SPWindow* pkWindow;
	pkWindow = Find(WIID_LOBBY_DIALOG_JUMIN, true);
	if( pkWindow )
		pkWindow->Hide();

	pkWindow = Find(WIID_LOBBY_DIALOG_INFOMATION, true);
	if( pkWindow )
		pkWindow->Hide();

	if( m_iType == 0 )
	{
		ClearServerList();
		ClearChannelList();
	}
	else if( m_iType == 1 )
	{
		m_bCardRender = false;
		ClearChracterList();
	}
	else if( m_iType == 2 )
	{
		SetDefaultCharacetLayer();
	}

	if( m_iType < 0 )
		SetImageProcessor(iType, true);
	else
		SetImageProcessor(m_iType, false);

	m_iNextType = iType;
	m_bWaitChageSubGame = true;

	/*
	m_iType = iType;
	m_bWaitChageSubGame = true;
	switch( iType )
	{
	case 0:
		{
			pkWindow = Find(WIID_LOBBY_SERVER_TITLE);
			if( pkWindow )
				pkWindow->Show();
			
			pkWindow = Find(WIID_LOBBY_CHAR_TITLE);
			if( pkWindow )
				pkWindow->Hide();

			pkWindow = Find(WIID_LOBBY_CHAR_CREATE_TITLE);
			if( pkWindow )
				pkWindow->Hide();
		}
		break;

	case 1:
		{
			pkWindow = Find(WIID_LOBBY_SERVER_TITLE);
			if( pkWindow )
				pkWindow->Hide();

			pkWindow = Find(WIID_LOBBY_CHAR_CREATE_TITLE);
			if( pkWindow )
				pkWindow->Hide();

			pkWindow = Find(WIID_LOBBY_CHAR_TITLE);
			if( pkWindow )
				pkWindow->Show();

			m_iSelectCard = -1;
			m_bCardRender = true;

			if( m_pCurServerInfo )
			{
				SPWindow* pkChildWindow;
				pkChildWindow = pkWindow->Find(WIID_LOBBY_CHAR_SERVER_ICON, true);
				pkChildWindow->SetImage("DATA/LOBBY/SERVERICON001.PNG", 0, 0);

				pkChildWindow = pkWindow->Find(WIID_LOBBY_CHAR_SERVER_NAME, true);
				pkChildWindow->SetWindowText(m_pCurServerInfo->name);
			}
		}
		break;

	case 2:
		{
			pkWindow = Find(WIID_LOBBY_SERVER_TITLE);
			if( pkWindow )
				pkWindow->Hide();

			pkWindow = Find(WIID_LOBBY_CHAR_TITLE);
			if( pkWindow )
				pkWindow->Hide();

			pkWindow = Find(WIID_LOBBY_CHAR_CREATE_TITLE);
			if( pkWindow )
				pkWindow->Show();

			SetCurSelectLayer();
		}
		break;
	}
	*/
}

//------------------------------------------------------------------------------------
void SPWindowLobby::InitServerListWindow()
{
	SPWindowStatic* pkServerStatic;
	SPWindowStatic* pkStatic;
	SPWindowButton* pkButton;

	// title
	pkServerStatic = new SPWindowStatic(WIID_LOBBY_SERVER_TITLE, 284, 8, 223, 66, this);
	pkServerStatic->SetImage("DATA/LOBBY/RB001.PNG", 1, 957);
	pkServerStatic->Show();

	pkStatic = new SPWindowStatic(WIID_LOBBY_SERVERLIST_SKIN, -232, 71, 363, 385, pkServerStatic);
	pkStatic->SetImage("DATA/LOBBY/RB001.PNG", 1, 1);
	pkStatic->Show();

	// rgn
	SetRect(&m_rcServerIconBase, 34, 52, 32, 32);
	SetRect(&m_rcServerButtonBase, 73, 56, 138, 25);
	SetRect(&m_rcCharNumBase, 215, 62, 41, 14);
	SetRect(&m_rcServerStateBase, 262, 58, 49, 20);

	SetRect(&m_rcSrcState[0], 365, 55, 414, 75);
	SetRect(&m_rcSrcState[1], 365, 76, 414, 96);
	SetRect(&m_rcSrcState[2], 365, 97, 414, 117);
	SetRect(&m_rcSrcState[3], 415, 97, 464, 117);

	m_iServerIconInterval = 8;
	m_iServerButtonInterval = 15;
	m_iCharNumInterval = 26;
	m_iServerStateInterval = 20;

//	m_iPageServerNum = 8;

	m_iSelectServer = -1;


	// BUDDYBUDDY의 경우 바로 종료 이미지가 나와야 한다.
	//if( SPLocalizeManager::GetInstance()->GetSiteType() == SITE_BUDDYBUDDY )
	if( SPLocalizeManager::GetInstance()->OnlyWebLogin() )
	{
		// back button
		pkStatic = new SPWindowStatic(WIID_LOBBY_SERVER_BACK_SKIN, 680-284, 534-8, 106, 52, pkServerStatic);
		pkStatic->SetImage("DATA/LOBBY/RB001.PNG", 1, 598);
		pkStatic->Show();

		pkButton = new SPWindowButton(WIID_LOBBY_SERVER_BACK, 20, 11, 66, 28, pkStatic);
		pkButton->SetImageHit	("DATA/LOBBY/RB001.PNG", 121, 608);
		pkButton->SetImagePush	("DATA/LOBBY/RB001.PNG", 121, 637);
		pkButton->Show();
	}
	else
	{
		// back button
		pkStatic = new SPWindowStatic(WIID_LOBBY_SERVER_BACK_SKIN, 680-284, 534-8, 106, 52, pkServerStatic);
		pkStatic->SetImage("DATA/LOBBY/RB001.PNG", 1, 545);
		pkStatic->Show();

		pkButton = new SPWindowButton(WIID_LOBBY_SERVER_BACK, 20, 11, 66, 28, pkStatic);
		pkButton->SetImageHit	("DATA/LOBBY/RB001.PNG", 121, 550);
		pkButton->SetImagePush	("DATA/LOBBY/RB001.PNG", 121, 579);
		pkButton->Show();
	}
	
}

//------------------------------------------------------------------------------------
void SPWindowLobby::InitChannelListWindow()
{
	SPWindow* pkServerWindow;
	pkServerWindow = Find(WIID_LOBBY_SERVER_TITLE);

	if( pkServerWindow == NULL )
		return;

	SPWindowStatic* pkChannelStatic;

	pkChannelStatic = new SPWindowStatic(WIID_LOBBY_CHANNELLIST_SKINTOP, 130, 166, 349, 49, pkServerWindow);
	pkChannelStatic->SetImage("DATA/LOBBY/RB001.PNG", 1, 387);
	//pkChannelStatic->Show();


	// rgn
	SetRect(&m_rcChannelButtonBase, 23, 1, 51, 22);
	SetRect(&m_rcChannelStateBase, 23, 24, 49, 20);

	m_iCHANNEL_BTN_VERTICAL_INTERVAL = 50;
	m_iCHANNEL_BTN_HORIZEN_INTERVAL	 = 63;

	m_CHANNEL_MAXX = 5;
	m_CHANNEL_MAXY = 4;

	m_iSelectChannel = -1;
}

//------------------------------------------------------------------------------------
void SPWindowLobby::InitCharWindow()
{
	SPWindowStatic* pkTitleStatic, *pkStatic, *pkServerSkin;
	SPWindowButton* pkButton;

	pkTitleStatic = new SPWindowStatic(WIID_LOBBY_CHAR_TITLE, 0, 0, 800, 600, this);
	pkTitleStatic->Show();

	pkServerSkin = new SPWindowStatic(WIID_LOBBY_CHAR_SERVER_SKIN, 168, 60, 224, 64, pkTitleStatic);
	pkServerSkin->SetImage("DATA/LOBBY/RB001.PNG", 1, 825);
	pkServerSkin->Show();

	pkStatic = new SPWindowStatic(WIID_LOBBY_CHAR_SERVER_ICON, 25, 16, 32, 32, pkServerSkin);
	pkStatic->SetImage("DATA/LOBBY/SERVERICON001.PNG", 0, 0);
	pkStatic->Show();

	pkStatic = new SPWindowStatic(WIID_LOBBY_CHAR_SERVER_NAME, 68, 24, 129, 16, pkServerSkin);
	pkStatic->SetFontColor(0xffffffff);
	pkStatic->SetFontShadow(true);
	pkStatic->Show();


	pkStatic = new SPWindowStatic(WIID_LOBBY_CHAR_SKIN, 110, 118, 578, 311, pkTitleStatic);
	pkStatic->SetImage("DATA/LOBBY/RB001.PNG", 445, 712);
	pkStatic->Show();

	pkButton = new SPWindowButton(WIID_LOBBY_CHAR_CARD, 64, 37, 134, 212, pkStatic);
	pkButton->Show();
	pkButton = new SPWindowButton(WIID_LOBBY_CHAR_CARD + 1, 224, 37, 134, 212, pkStatic);
	pkButton->Show();
	pkButton = new SPWindowButton(WIID_LOBBY_CHAR_CARD + 2, 384, 37, 134, 212, pkStatic);
	pkButton->Show();

	SetRect(&m_rcDstCardBase, 174, 155, 134, 212);
	SetRect(&m_rcDstCardNumBase, 185, 163, 9, 11);
	SetRect(&m_rcDstCardSelectBase, 165, 146, 152, 230);

	SetRect(&m_rcSrcCardFront, 365, 438, 499, 650);
	SetRect(&m_rcSrcCardBack, 365, 225, 499, 437);
	SetRect(&m_rcSrcCardSelect, 292, 793, 444, 1023);

	SetRect(&m_rcSrcCardNum[0], 331, 559, 340, 570);
	SetRect(&m_rcSrcCardNum[1], 341, 559, 350, 570);
	SetRect(&m_rcSrcCardNum[2], 351, 559, 360, 570);

	m_iCardInterval = 26;
	m_iCardSelectInterval = 8;
	m_iSelectCard = -1;
	m_bCardRender = false;
	m_iCardProcessing = 0;

	//g_pVideo->GetTextureMgr()->LoadTexture(
	//	GetAddResPath( "DATA/Lobby/RB001.PNG", RES_WORK_RESOURCE_PATH), &m_pkCard);

	g_pVideo->GetTextureMgr()->LoadTexture(	"DATA/LOBBY/RB001.PNG" , &m_pkCard);

	pkStatic = new SPWindowStatic(WIID_LOBBY_CHAR_MENU_SKIN, 308, 404, 184, 144, pkTitleStatic);
	pkStatic->SetImage("DATA/LOBBY/RB001.PNG", 839, 1);
	pkStatic->Show();

	pkButton = new SPWindowButton(WIID_LOBBY_CHAR_MENU_SELECT, 26, 19, 132, 26, pkStatic);
	pkButton->SetImageHit	("DATA/LOBBY/RB001.PNG", 758, 146);
	pkButton->SetImagePush	("DATA/LOBBY/RB001.PNG", 891, 146);
	pkButton->SetImageDisable("DATA/LOBBY/RB001.PNG", 706, 119);
	pkButton->SetEnable(true);
	pkButton->Show();

	pkButton = new SPWindowButton(WIID_LOBBY_CHAR_MENU_CREATE, 26, 58, 132, 26, pkStatic);
	pkButton->SetImageHit	("DATA/LOBBY/RB001.PNG", 758, 173);
	pkButton->SetImagePush	("DATA/LOBBY/RB001.PNG", 891, 173);
	pkButton->Show();
	pkButton->SetEnable(false);

	pkButton = new SPWindowButton(WIID_LOBBY_CHAR_MENU_DELETE, 26, 97, 132, 26, pkStatic);
	pkButton->SetImageHit	("DATA/LOBBY/RB001.PNG", 758, 200);
	pkButton->SetImagePush	("DATA/LOBBY/RB001.PNG", 891, 200);
	pkButton->Show();

	// back button
	pkStatic = new SPWindowStatic(WIID_LOBBY_CHAR_BACK_SKIN, 680, 534, 106, 52, pkTitleStatic);
	pkStatic->SetImage		("DATA/LOBBY/RB001.PNG", 1, 545);
	pkStatic->Show();
	
	pkButton = new SPWindowButton(WIID_LOBBY_CHAR_BACK, 20, 11, 66, 28, pkStatic);
	pkButton->SetImageHit	("DATA/LOBBY/RB001.PNG", 121, 550);
	pkButton->SetImagePush	("DATA/LOBBY/RB001.PNG", 121, 579);
	pkButton->Show();
}

//------------------------------------------------------------------------------------
void SPWindowLobby::InitCharCreateWindow()
{
	SPWindowStatic* pkTitleStatic, *pkStatic;
	SPWindowButton* pkButton;
	SPWindowEdit* pkEdit;
	int i;

	pkTitleStatic = new SPWindowStatic(WIID_LOBBY_CHAR_CREATE_TITLE, 284, 8, 223, 66, this);
	pkTitleStatic->SetImage("DATA/LOBBY/RB001.PNG", 1, 890);
	pkTitleStatic->Show();

	//------------------------------------------------------
	// Character Basic Info
	if( _SHOW_USE_WEAPON_ )
		pkStatic = new SPWindowStatic(WIID_LOBBY_CHAR_CREATE_INFO_SKIN, -252, 82, 256, 393, pkTitleStatic);
	else
		pkStatic = new SPWindowStatic(WIID_LOBBY_CHAR_CREATE_INFO_SKIN, -252, 104, 256, 393, pkTitleStatic);
	pkStatic->SetToolTipStr("LobbyCreateInfoSkin") ;
	pkStatic->SetImage("DATA/LOBBY/RB001.PNG", 500, 281);
	pkStatic->Show();

	pkEdit = new SPWindowEdit( WIID_LOBBY_CHAR_CREATE_INFO_NAME, 35, 60, 184, 14, pkStatic);
	pkEdit->SetLimitText(LEN_NAME * 2);
	pkEdit->SetFontColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
	pkEdit->Show();	

	//pkButton = new SPWindowButton(WIID_LOBBY_CHAR_CREATE_INFO_NAME_CHECK, 161, 54, 64, 25, pkStatic);
	//pkButton->SetImageHit("DATA/LOBBY/RB001.PNG", 704, 27);
	//pkButton->SetImagePush("DATA/LOBBY/RB001.PNG", 769, 27);
	//pkButton->Show();

	pkButton = new SPWindowButton(WIID_LOBBY_CHAR_CREATE_INFO_ZENDER_M, 85, 113, 36, 36, pkStatic);
	pkButton->SetImageHit	("DATA/LOBBY/RB001.PNG", 767, 244);
	pkButton->SetImagePush	("DATA/LOBBY/RB001.PNG", 767, 281);
	pkButton->SetImageCheck	("DATA/LOBBY/RB001.PNG", 767, 281);
	pkButton->Show();

	pkButton = new SPWindowButton(WIID_LOBBY_CHAR_CREATE_INFO_ZENDER_W, 135, 113, 36, 36, pkStatic);
	pkButton->SetImageHit	("DATA/LOBBY/RB001.PNG", 804, 244);
	pkButton->SetImagePush	("DATA/LOBBY/RB001.PNG", 804, 281);
	pkButton->SetImageCheck	("DATA/LOBBY/RB001.PNG", 804, 281);
	pkButton->Show();

	pkButton = new SPWindowButton(WIID_LOBBY_CHAR_CREATE_INFO_ATTACK, 35, 191, 36, 36, pkStatic);
	pkButton->SetImageHit	("DATA/LOBBY/RB001.PNG", 841, 244);
	pkButton->SetImagePush	("DATA/LOBBY/RB001.PNG", 841, 281);
	pkButton->SetImageCheck	("DATA/LOBBY/RB001.PNG", 841, 281);
	pkButton->Show();

	pkButton = new SPWindowButton(WIID_LOBBY_CHAR_CREATE_INFO_DEFFEN, 85, 191, 36, 36, pkStatic);
	pkButton->SetImageHit	("DATA/LOBBY/RB001.PNG", 878, 244);
	pkButton->SetImagePush	("DATA/LOBBY/RB001.PNG", 878, 281);
	pkButton->SetImageCheck	("DATA/LOBBY/RB001.PNG", 878, 281);
	pkButton->Show();

	pkButton = new SPWindowButton(WIID_LOBBY_CHAR_CREATE_INFO_MAGIC, 135, 191, 36, 36, pkStatic);
	pkButton->SetImageHit	("DATA/LOBBY/RB001.PNG", 915, 244);
	pkButton->SetImagePush	("DATA/LOBBY/RB001.PNG", 915, 281);
	pkButton->SetImageCheck	("DATA/LOBBY/RB001.PNG", 915, 281);
	pkButton->Show();

	pkButton = new SPWindowButton(WIID_LOBBY_CHAR_CREATE_INFO_SCOUT, 185, 191, 36, 36, pkStatic);
	pkButton->SetImageHit	("DATA/LOBBY/RB001.PNG", 952, 244);
	pkButton->SetImagePush	("DATA/LOBBY/RB001.PNG", 952, 281);
	pkButton->SetImageCheck	("DATA/LOBBY/RB001.PNG", 952, 281);
	pkButton->Show();

	SPWindowStatic* pkClassTypeWindow = new SPWindowStatic(WIID_LOBBY_CHAR_CREATE_TYPE_INFO, 32, 240, 192, 98, pkStatic);
	pkClassTypeWindow->SetWindowText("");
	pkClassTypeWindow->SetFontColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
	pkClassTypeWindow->SetFormat(DT_LEFT);
	pkClassTypeWindow->SetMultiLine(true, 4);
	pkClassTypeWindow->Show();

	// _SHOW_USE_WEAPON_
	// Character type Weapon
	if( _SHOW_USE_WEAPON_ )
	{
		SPWindowButton* pkClassWeaponWindow = NULL ;
		pkClassWeaponWindow = new SPWindowButton(WIID_LOBBY_CHAR_CREATE_ICON_KNUCKLE, 32, 365, 24, 18, pkStatic);
		pkClassWeaponWindow->SetImage		("DATA/LOBBY/RB001.PNG", 465, 1);
		pkClassWeaponWindow->SetImageHit	("DATA/LOBBY/RB001.PNG", 465, 1);
		pkClassWeaponWindow->SetImagePush	("DATA/LOBBY/RB001.PNG", 465, 1);
		pkClassWeaponWindow->Show();
		pkClassWeaponWindow = new SPWindowButton(WIID_LOBBY_CHAR_CREATE_ICON_TWO_SD, 60, 365, 24, 18, pkStatic);
		pkClassWeaponWindow->SetImage		("DATA/LOBBY/RB001.PNG", 415, 20);
		pkClassWeaponWindow->SetImageHit	("DATA/LOBBY/RB001.PNG", 415, 20);
		pkClassWeaponWindow->SetImagePush	("DATA/LOBBY/RB001.PNG", 415, 20);
		pkClassWeaponWindow->Show();
		pkClassWeaponWindow = new SPWindowButton(WIID_LOBBY_CHAR_CREATE_ICON_SPERE, 88, 365, 24, 18, pkStatic);
		pkClassWeaponWindow->SetImage		("DATA/LOBBY/RB001.PNG", 440, 20);
		pkClassWeaponWindow->SetImageHit	("DATA/LOBBY/RB001.PNG", 440, 20);
		pkClassWeaponWindow->SetImagePush	("DATA/LOBBY/RB001.PNG", 440, 20);
		pkClassWeaponWindow->Show();
		pkClassWeaponWindow = new SPWindowButton(WIID_LOBBY_CHAR_CREATE_ICON_ONE_SD, 60, 365, 24, 18, pkStatic);
		pkClassWeaponWindow->SetImage		("DATA/LOBBY/RB001.PNG", 390, 20);
		pkClassWeaponWindow->SetImageHit	("DATA/LOBBY/RB001.PNG", 390, 20);
		pkClassWeaponWindow->SetImagePush	("DATA/LOBBY/RB001.PNG", 390, 20);
		pkClassWeaponWindow->Show();
		pkClassWeaponWindow = new SPWindowButton(WIID_LOBBY_CHAR_CREATE_ICON_HAMMER, 88, 365, 24, 18, pkStatic);
		pkClassWeaponWindow->SetImage		("DATA/LOBBY/RB001.PNG", 415, 1);
		pkClassWeaponWindow->SetImageHit	("DATA/LOBBY/RB001.PNG", 415, 1);
		pkClassWeaponWindow->SetImagePush	("DATA/LOBBY/RB001.PNG", 415, 1);
		pkClassWeaponWindow->Show();
		pkClassWeaponWindow = new SPWindowButton(WIID_LOBBY_CHAR_CREATE_ICON_DAGGER, 60, 365, 24, 18, pkStatic);
		pkClassWeaponWindow->SetImage		("DATA/LOBBY/RB001.PNG", 365, 20);
		pkClassWeaponWindow->SetImageHit	("DATA/LOBBY/RB001.PNG", 365, 20);
		pkClassWeaponWindow->SetImagePush	("DATA/LOBBY/RB001.PNG", 365, 20);
		pkClassWeaponWindow->Show();
		pkClassWeaponWindow = new SPWindowButton(WIID_LOBBY_CHAR_CREATE_ICON_BOW, 88, 365, 24, 18, pkStatic);
		pkClassWeaponWindow->SetImage		("DATA/LOBBY/RB001.PNG", 390, 1);
		pkClassWeaponWindow->SetImageHit	("DATA/LOBBY/RB001.PNG", 390, 1);
		pkClassWeaponWindow->SetImagePush	("DATA/LOBBY/RB001.PNG", 390, 1);
		pkClassWeaponWindow->Show();
		pkClassWeaponWindow = new SPWindowButton(WIID_LOBBY_CHAR_CREATE_ICON_CROSSBOW, 116, 365, 24, 18, pkStatic);
		pkClassWeaponWindow->SetImage		("DATA/LOBBY/RB001.PNG", 365, 1);
		pkClassWeaponWindow->SetImageHit	("DATA/LOBBY/RB001.PNG", 365, 1);
		pkClassWeaponWindow->SetImagePush	("DATA/LOBBY/RB001.PNG", 365, 1);
		pkClassWeaponWindow->Show();
		pkClassWeaponWindow = new SPWindowButton(WIID_LOBBY_CHAR_CREATE_ICON_WAND, 88, 365, 24, 18, pkStatic);
		pkClassWeaponWindow->SetImage		("DATA/LOBBY/RB001.PNG", 440, 1);
		pkClassWeaponWindow->SetImageHit	("DATA/LOBBY/RB001.PNG", 440, 1);
		pkClassWeaponWindow->SetImagePush	("DATA/LOBBY/RB001.PNG", 440, 1);
		pkClassWeaponWindow->Show();
	}

	//------------------------------------------------------
	// Character Window
	pkStatic = new SPWindowStatic(WIID_LOBBY_CHAR_CREATE_SKIN, 19, 174, 194, 279, pkTitleStatic);
	pkStatic->SetImage	("DATA/LOBBY/RB001.PNG", 509, 1);
	pkStatic->Show();

	pkButton = new SPWindowButton(WIID_LOBBY_CHAR_CREATE, 65, 209, 64, 25, pkStatic);
	pkButton->SetImageHit		("DATA/LOBBY/RB001.PNG", 704, 1);
	pkButton->SetImagePush		("DATA/LOBBY/RB001.PNG", 769, 1);
	pkButton->SetImageDisable	("DATA/LOBBY/RB001.PNG", 704, 53);
	pkButton->Show();


	//------------------------------------------------------
	// Face Change Window
	pkStatic = new SPWindowStatic(WIID_LOBBY_CHAR_CREATE_CHANGE_SKIN, 228, 82, 256, 393, pkTitleStatic);
	pkStatic->SetImage("DATA/LOBBY/RB001.PNG", 767, 318);
	pkStatic->Show();

	int iPosY;
	for( i = 0; i < 7; i++ )
	{
		iPosY = 34 + (20 + 30) * i;
		pkButton = new SPWindowButton(WIID_LOBBY_CHAR_CREATE_CHANGE_LEFT + i, 133, iPosY, 20, 20, pkStatic);
		pkButton->SetImageHit("DATA/LOBBY/RB001.PNG", 1, 781);
		pkButton->SetImagePush("DATA/LOBBY/RB001.PNG", 1, 802);
		pkButton->Show();
		
		pkButton = new SPWindowButton(WIID_LOBBY_CHAR_CREATE_CHANGE_RIGHT + i, 211, iPosY, 20, 20, pkStatic);
		pkButton->SetImageHit("DATA/LOBBY/RB001.PNG", 22, 781);
		pkButton->SetImagePush("DATA/LOBBY/RB001.PNG", 22, 802);
		pkButton->Show();
	
		SPWindowStatic* pkTextStatic;
		
		D3DXCOLOR color;
		if( i < 3 )
			color = D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f);
		else
			color = D3DXCOLOR(0.5607f, 0.3882f, 0.3882f, 1.0f);

		iPosY = 39 + (20 + 30) * i;
		pkTextStatic = new SPWindowStatic(WIID_LOBBY_CHAR_CREATE_CHANGE_CURNUM + i, 159, iPosY, 14, 12, pkStatic);
		pkTextStatic->SetFontColor(color);
		pkTextStatic->Show();

		pkTextStatic = new SPWindowStatic(WIID_LOBBY_CHAR_CREATE_CHANGE_TOLNUM + i, 190, iPosY, 14, 12, pkStatic);
		pkTextStatic->SetFontColor(color);
		pkTextStatic->Show();

		iPosY = 61 + (20 + 30) * i;
		pkTextStatic = new SPWindowStatic(WIID_LOBBY_CHAR_CREATE_CHANGE_EXPLAIN + i, 31, iPosY, 195, 12, pkStatic);
		pkTextStatic->SetFontColor(color);
		pkTextStatic->Show();
	}

	//------------------------------------------------------
	// back button
	pkStatic = new SPWindowStatic(WIID_LOBBY_CHAR_CREATE_BACK_SKIN, 680-284, 534-8, 106, 52, pkTitleStatic);
	pkStatic->SetImage("DATA/LOBBY/RB001.PNG", 1, 545);
	pkStatic->Show();
	
	pkButton = new SPWindowButton(WIID_LOBBY_CHAR_CREATE_BACK, 20, 11, 66, 28, pkStatic);
	pkButton->SetImageHit("DATA/LOBBY/RB001.PNG", 121, 550);
	pkButton->SetImagePush("DATA/LOBBY/RB001.PNG", 121, 579);
	pkButton->Show();

	//------------------------------------------------------
	g_pVideo->GetTextureMgr()->LoadTexture(	"DATA/LOBBY/RB002.PNG" , &m_pkStatusSkin);
	SetRect(&m_rcStatusSkinDst, 128, 481, 320, 579);
	SetRect(&m_rcStatusSkinSrc, 1, 285, 193, 383);

	m_iMaxStatusWidth = 90;
	for( i = 0; i < 4; i++ )
	{
		SetRect(&m_rcStatusSrc[i], 2, 385 + i * 7, 3, 391 + i * 7);
		m_iMaxStatusDst[i] = 0;
	}

	SetRect(&m_rcStatusDst[0], 209, 500, 209, 506);
	SetRect(&m_rcStatusDst[1], 209, 518, 209, 524);
	SetRect(&m_rcStatusDst[2], 209, 536, 209, 542);
	SetRect(&m_rcStatusDst[3], 209, 554, 209, 560);

	m_fCurStatusWidth[0] = 0.0f;
	m_fCurStatusWidth[1] = 0.0f;
	m_fCurStatusWidth[2] = 0.0f;
	m_fCurStatusWidth[3] = 0.0f;
	
	InitCharCreateItem();
	SetDefaultCharacetLayer();
}

//------------------------------------------------------------------------------------
void SPWindowLobby::InitCharCreateItem()
{
	string strLDTFile;
	strLDTFile = "DATA/LDT/CREATEAVATAR.LDT";

	SPLDTFile* pkLDTFile = NULL;
	if( !g_pResourceManager->GetLDTFromFileName(strLDTFile.c_str(), &pkLDTFile) )
		return;

	int iDataNum, iFieldNum, iZender, i, j;
	long lItemID;
	LDT_Field Field;
	string strFileName;

	iDataNum = pkLDTFile->GetItemCount();
	iFieldNum = pkLDTFile->GetFieldCount();

	for( i = 0; i < iDataNum; i++ )
	{
		lItemID = pkLDTFile->GetPrimaryKey(i);
		if( lItemID < 1000 )
			iZender = 0;
		else
			iZender = 1;

		for( j = 0; j < iFieldNum; j++ )
		{
			pkLDTFile->GetField(lItemID, j, Field);

			if( Field.nFldTyp == fldNum )
			{
				if( Field.uData.lValue == 0 )
					continue;
			}
			else if( Field.nFldTyp == fldString )
			{
				if( (int)strlen(Field.uData.pValue) == 0 )
					continue;
			}

			if( strcmp(Field.pName, "_HairID") == 0 )
				m_vHairID[iZender].push_back(Field.uData.lValue);
			else if( strcmp(Field.pName, "_HairColorID") == 0 )
				m_vHairColorID[iZender].push_back(Field.uData.lValue);
			else if( strcmp(Field.pName, "_ExpressionID") == 0 )
				m_vExpressionID[iZender].push_back(Field.uData.lValue);
			else if( strcmp(Field.pName, "_ExpressionColorID") == 0 )
				m_vExpressionColorID[iZender].push_back(Field.uData.lValue);
			else if( strcmp(Field.pName, "_SkinID") == 0 )
				m_vSkinID[iZender].push_back(Field.uData.lValue);
			else if( strcmp(Field.pName, "_BlouseID") == 0 )
				m_vBlouseID[iZender].push_back(Field.uData.lValue);
			else if( strcmp(Field.pName, "_PantsID") == 0 )
				m_vPantsID[iZender].push_back(Field.uData.lValue);
			else if( strcmp(Field.pName, "_HairExplain") == 0 )
				m_vstrHair[iZender].push_back(Field.uData.pValue);
			else if( strcmp(Field.pName, "_HairColorExplain") == 0 )
				m_vstrHairColor[iZender].push_back(Field.uData.pValue);
			else if( strcmp(Field.pName, "_ExpressionExplain") == 0 )
				m_vstrExpression[iZender].push_back(Field.uData.pValue);
			else if( strcmp(Field.pName, "_ExpressionColorExplain") == 0 )
				m_vstrExpressionColor[iZender].push_back(Field.uData.pValue);
			else if( strcmp(Field.pName, "_SkinExplain") == 0 )
				m_vstrSkin[iZender].push_back(Field.uData.pValue);
			else if( strcmp(Field.pName, "_BlouseExplain") == 0 )
				m_vstrBlouse[iZender].push_back(Field.uData.pValue);
			else if( strcmp(Field.pName, "_PantsExplain") == 0 )
				m_vstrPants[iZender].push_back(Field.uData.pValue);
		}
	}

	m_pkCreateCharacter = new SPPlayer;
	m_pkCreateCharacter->SetLobbyAvatar(LOBBY_INFO);
	m_pkCreateCharacter->Init();
	m_pkCreateCharacter->SetShowValue(false, false, false);

	///> Charactor
	m_pkCreateCharacter->SetPlayerClass(CLASS_A);
	m_pkCreateCharacter->SetAction(ACTION_STOP, true);

	m_pkCreateCharacter->SetPos(400.0f, 348.0f);
}

//------------------------------------------------------------------------------------
void SPWindowLobby::SetDefaultCharacetLayer()
{
	m_iZender = 0;
	m_iBeforeZender = 0;

	m_iClassType = 0;
	int i;
	for( i = 0; i < 7; i++ )
	{
		m_iCurLayerIndex[i] = 0;
		m_iBeforeLayerIndex[i] = -1;
	}

	OnZenderClick(WIID_LOBBY_CHAR_CREATE_INFO_ZENDER_M, 0, 0);
	OnClassClick(WIID_LOBBY_CHAR_CREATE_INFO_ATTACK, 0, 0);

	m_bCharacterNameSuccess = true;
	m_strCharacterName = "";

	SPWindowEdit* pkEdit = (SPWindowEdit*)Find(WIID_LOBBY_CHAR_CREATE_INFO_NAME, true);
	if( pkEdit )
		pkEdit->SetWindowText(NULL);

	//SPWindowButton* pkButton = (SPWindowButton*)Find(WIID_LOBBY_CHAR_CREATE, true);
	//if( pkButton )
	//	pkButton->SetEnable(false);
	
	SetCurSelectLayer();
}

//------------------------------------------------------------------------------------
void SPWindowLobby::SetChangeCharacterLayer()
{
	FIGURE_ITEM figureItem;

	bool bAllReset = false;
	if( m_iBeforeZender != m_iZender )
	{
		m_iBeforeZender = m_iZender;
		bAllReset = true;
	}

	if( m_iCurLayerIndex[0] != m_iBeforeLayerIndex[0] ||
		m_iCurLayerIndex[1] != m_iBeforeLayerIndex[1] ||
		bAllReset )
	{
		if( m_iCurLayerIndex[0] < (int)m_vHairID[m_iZender].size() && 
			m_iCurLayerIndex[1] < (int)m_vHairColorID[m_iZender].size() )
		{
			figureItem.iItemID = m_vHairID[m_iZender][m_iCurLayerIndex[0]] + m_vHairColorID[m_iZender][m_iCurLayerIndex[1]];
			figureItem.SlotInfo.ContainerType = FIGURE_BASIC;
			figureItem.SlotInfo.SlotIndex = EQ_HAIR;
			m_pkCreateCharacter->ItemSetting(figureItem);
		}

		m_iBeforeLayerIndex[0] = m_iCurLayerIndex[0];
		m_iBeforeLayerIndex[1] = m_iCurLayerIndex[1];
	}

	if( m_iCurLayerIndex[2] != m_iBeforeLayerIndex[2] ||
		m_iCurLayerIndex[3] != m_iBeforeLayerIndex[3] ||
		bAllReset )
	{
		if( m_iCurLayerIndex[2] < (int)m_vExpressionID[m_iZender].size() && 
			m_iCurLayerIndex[3] < (int)m_vExpressionColorID[m_iZender].size() )
		{
			figureItem.iItemID = m_vExpressionID[m_iZender][m_iCurLayerIndex[2]] + m_vExpressionColorID[m_iZender][m_iCurLayerIndex[3]];
			figureItem.SlotInfo.ContainerType = FIGURE_BASIC;
			figureItem.SlotInfo.SlotIndex = EQ_EXPRESSION;
			m_pkCreateCharacter->ItemSetting(figureItem);
		}

		m_iBeforeLayerIndex[2] = m_iCurLayerIndex[2];
		m_iBeforeLayerIndex[3] = m_iCurLayerIndex[3];
	}

	if( m_iCurLayerIndex[4] != m_iBeforeLayerIndex[4] ||
		bAllReset )
	{
		if( m_iCurLayerIndex[4] < (int)m_vSkinID[m_iZender].size() )
		{
			figureItem.iItemID = m_vSkinID[m_iZender][m_iCurLayerIndex[4]];
			figureItem.SlotInfo.ContainerType = FIGURE_BASIC;
			figureItem.SlotInfo.SlotIndex = EQ_SKIN;
			m_pkCreateCharacter->ItemSetting(figureItem);
		}

		m_iBeforeLayerIndex[4] = m_iCurLayerIndex[4];
	}

	if( m_iCurLayerIndex[5] != m_iBeforeLayerIndex[5] ||
		bAllReset )
	{
		if( m_iCurLayerIndex[5] < (int)m_vBlouseID[m_iZender].size() )
		{
			figureItem.iItemID = m_vBlouseID[m_iZender][m_iCurLayerIndex[5]];
			figureItem.SlotInfo.ContainerType = FIGURE_BASIC;
			figureItem.SlotInfo.SlotIndex = EQ_BLOUSE;
			m_pkCreateCharacter->ItemSetting(figureItem);
		}

		m_iBeforeLayerIndex[5] = m_iCurLayerIndex[5];
	}

	if( m_iCurLayerIndex[6] != m_iBeforeLayerIndex[6] ||
		bAllReset )
	{
		if( m_iCurLayerIndex[6] < (int)m_vPantsID[m_iZender].size() )
		{
			figureItem.iItemID = m_vPantsID[m_iZender][m_iCurLayerIndex[6]];
			figureItem.SlotInfo.ContainerType = FIGURE_BASIC;
			figureItem.SlotInfo.SlotIndex = EQ_PANTS;
			m_pkCreateCharacter->ItemSetting(figureItem);
		}

		m_iBeforeLayerIndex[6] = m_iCurLayerIndex[6];
	}
}

//------------------------------------------------------------------------------------
void SPWindowLobby::SetCurSelectLayer()
{
	int i;
	char szBuf[32];

	SPWindow* pkCurStatic, *pkTolStatic, *pkExplainStatic;

	for( i = 0; i < 7; i++ )
	{
		pkCurStatic = Find(WIID_LOBBY_CHAR_CREATE_CHANGE_CURNUM + i, true);
		pkTolStatic = Find(WIID_LOBBY_CHAR_CREATE_CHANGE_TOLNUM + i, true);
		pkExplainStatic = Find(WIID_LOBBY_CHAR_CREATE_CHANGE_EXPLAIN + i, true);

		sprintf(szBuf, "%d", m_iCurLayerIndex[i] + 1);
		pkCurStatic->SetWindowText(szBuf);

		switch( i )
		{
		case 0:
			sprintf(szBuf, "%d", (int)m_vHairID[m_iZender].size());
			pkTolStatic->SetWindowText(szBuf);
			if( m_iCurLayerIndex[i] < (int)m_vstrHair[m_iZender].size() )
				pkExplainStatic->SetWindowText(m_vstrHair[m_iZender][m_iCurLayerIndex[i]].c_str());
			break;

		case 1:
			sprintf(szBuf, "%d", (int)m_vstrHairColor[m_iZender].size());
			pkTolStatic->SetWindowText(szBuf);
			if( m_iCurLayerIndex[i] < (int)m_vstrHairColor[m_iZender].size() )
				pkExplainStatic->SetWindowText(m_vstrHairColor[m_iZender][m_iCurLayerIndex[i]].c_str());
			break;

		case 2:
			sprintf(szBuf, "%d", (int)m_vstrExpression[m_iZender].size());
			pkTolStatic->SetWindowText(szBuf);
			if( m_iCurLayerIndex[i] < (int)m_vstrExpression[m_iZender].size() )
				pkExplainStatic->SetWindowText(m_vstrExpression[m_iZender][m_iCurLayerIndex[i]].c_str());
			break;

		case 3:
			sprintf(szBuf, "%d", (int)m_vstrExpressionColor[m_iZender].size());
			pkTolStatic->SetWindowText(szBuf);
			if( m_iCurLayerIndex[i] < (int)m_vstrExpressionColor[m_iZender].size() )
				pkExplainStatic->SetWindowText(m_vstrExpressionColor[m_iZender][m_iCurLayerIndex[i]].c_str());
			break;

		case 4:
			sprintf(szBuf, "%d", (int)m_vstrSkin[m_iZender].size());
			pkTolStatic->SetWindowText(szBuf);
			if( m_iCurLayerIndex[i] < (int)m_vstrSkin[m_iZender].size() )
				pkExplainStatic->SetWindowText(m_vstrSkin[m_iZender][m_iCurLayerIndex[i]].c_str());
			break;

		case 5:
			sprintf(szBuf, "%d", (int)m_vstrBlouse[m_iZender].size());
			pkTolStatic->SetWindowText(szBuf);
			if( m_iCurLayerIndex[i] < (int)m_vstrBlouse[m_iZender].size() )
				pkExplainStatic->SetWindowText(m_vstrBlouse[m_iZender][m_iCurLayerIndex[i]].c_str());
			break;

		case 6:
			sprintf(szBuf, "%d", (int)m_vstrPants[m_iZender].size());
			pkTolStatic->SetWindowText(szBuf);
			if( m_iCurLayerIndex[i] < (int)m_vstrPants[m_iZender].size() )
				pkExplainStatic->SetWindowText(m_vstrPants[m_iZender][m_iCurLayerIndex[i]].c_str());
			break;
		}
	}

	SetChangeCharacterLayer();
}

//------------------------------------------------------------------------------------
void SPWindowLobby::SetCharacterNameCheck(bool bTrue)
{
	m_bCharacterNameSuccess = bTrue;

	SPWindowButton* pkButton;
	pkButton = (SPWindowButton*)Find(WIID_LOBBY_CHAR_CREATE, true);

	if( pkButton )
		pkButton->SetEnable(bTrue);
}

//------------------------------------------------------------------------------------
void SPWindowLobby::SetServerList(vector< WORLD_INFO >& vWorldList)
{
	ClearServerList();

	SPWindow* pkServerWindow;
	pkServerWindow = Find(WIID_LOBBY_SERVERLIST_SKIN, true);

	if( pkServerWindow == NULL )
		return;

	int i;
	m_iServerListSize = (int)vWorldList.size();
	m_iServerSliderTop = 0 ;
	m_pSlider = NULL ;

	SPWindowButton* pkButton;
	SPWindowStatic* pkStatic;
	char szBuf[32];

	int iServerIconPos = m_rcServerIconBase.top;
	int iServerBtnPos = m_rcServerButtonBase.top;
	int iCharNumPos = m_rcCharNumBase.top;
	int iServerStatePos = m_rcServerStateBase.top;

	//////////////////////////////////////////////////////////////////////////
	// Server Scroll ( LOBBY_SERVER_VIEW_MAX )
	if( m_iServerListSize > LOBBY_SERVER_VIEW_MAX )
	{
		m_bIsSlide = true ;
		m_iSliderMoveRange = m_iServerListSize - LOBBY_SERVER_VIEW_MAX + 1 ;
		RECT rtRect;
		m_pSlider	=	new	SPWindowSlider( WIID_LOBBY_SERVERLIST_SLIDER , 320 , 57 , 12 , 294 , pkServerWindow );

		SetRect( &rtRect , 460 , 470 , 472 , 494 );	//	일반
		m_pSlider->SetImageHandle( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );
		SetRect( &rtRect , 473 , 470 , 485 , 494 );	//	마우스 오버
		m_pSlider->SetImageHandleHit( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );
		SetRect( &rtRect , 486 , 470 , 498 , 494 );	//	마우스 클릭
		m_pSlider->SetImageHandlePush( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );
		SetRect( &rtRect , 499 , 470 , 511 , 494 );
		m_pSlider->SetImageHandleDisable( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

		SetRect( &rtRect , 460 , 496 , 472 , 498 );	//	일반
		m_pSlider->SetImagePageNormal( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );
		SetRect( &rtRect , 473 , 496 , 485 , 498 );	//	마우스 오버
		m_pSlider->SetImagePageHit( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );
		SetRect( &rtRect , 486 , 496 , 498 , 498 );	//	마우스 클릭
		m_pSlider->SetImagePageShadow( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );
		SetRect( &rtRect , 499 , 496 , 511 , 498 );
		m_pSlider->SetImagePageDisable( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

		m_pSlider->SPSendMessage( SPIM_SLIDER_PAGESIZE , LOBBY_SERVER_PAGESIZE );
		m_pSlider->SetCurpage( 0 );
		m_pSlider->SPSendMessage( SPIM_SET_MAXVALUE , m_iSliderMoveRange );
		m_pSlider->Show() ;

		pkButton	=	new	SPWindowButton( WIID_LOBBY_SERVERLIST_SLIDER_UP , 320 , 46 , 12 , 11 , pkServerWindow );
		pkButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 460 , 458 );
		pkButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 473 , 458 );
		pkButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 486 , 458 );
		pkButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 499 , 458 );
		pkButton->Show() ;
		pkButton =	NULL;


		pkButton	=	new	SPWindowButton( WIID_LOBBY_SERVERLIST_SLIDER_DOWN , 320 , 351 , 12 , 11 , pkServerWindow );
		pkButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 460 , 500 );
		pkButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 473 , 500 );
		pkButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 486 , 500 );
		pkButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 499 , 500 );
		pkButton->Show() ;
		pkButton =	NULL;
	}
	
	//////////////////////////////////////////////////////////////////////////
	// Slider in Control
	int iMakeCount = ( m_iServerListSize > LOBBY_SERVER_VIEW_MAX ) ? LOBBY_SERVER_VIEW_MAX : m_iServerListSize ;
	int iSrcIconX, iSrcIconY;
	for( i = 0; i < iMakeCount; i++ )
	{
		pkStatic = new SPWindowStatic(WIID_LOBBY_SERVERLIST_ICON + i,
			m_rcServerIconBase.left, iServerIconPos,
			m_rcServerIconBase.right, m_rcServerIconBase.bottom, pkServerWindow);
		
		if( vWorldList[i].wid )
		{
			iSrcIconX = ((vWorldList[i].wid - 1 + m_iServerSliderTop) % 8) * m_rcServerIconBase.right;
			iSrcIconY = ((vWorldList[i].wid - 1 + m_iServerSliderTop) / 8) * m_rcServerIconBase.right;
			pkStatic->SetImage("DATA/LOBBY/SERVERICON001.PNG", iSrcIconX, iSrcIconY);
			pkStatic->Show();
		}
		
		iServerIconPos += m_rcServerIconBase.bottom + m_iServerIconInterval;

		pkButton = new SPWindowButton(WIID_LOBBY_SERVERLIST_BUTTON + i,
			m_rcServerButtonBase.left, iServerBtnPos,
			m_rcServerButtonBase.right, m_rcServerButtonBase.bottom, pkServerWindow);

		pkButton->SetFontColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
		pkButton->SetWindowText(vWorldList[i].name);
		pkButton->SetImage("DATA/LOBBY/RB001.PNG", 365, 118);
		pkButton->SetImageHit("DATA/LOBBY/RB001.PNG", 365, 144);
		pkButton->SetImagePush("DATA/LOBBY/RB001.PNG", 365, 170);
		pkButton->SetImageCheck("DATA/LOBBY/RB001.PNG", 365, 170);
		pkButton->Show();

		iServerBtnPos += m_rcServerButtonBase.bottom + m_iServerButtonInterval;

		pkStatic = new SPWindowStatic(WIID_LOBBY_SERVERLIST_CHARNUM + i,
			m_rcCharNumBase.left, iCharNumPos,
			m_rcCharNumBase.right, m_rcCharNumBase.bottom, pkServerWindow);

		pkStatic->SetFontColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));

		sprintf(szBuf, "%d", vWorldList[i].nCharCount);
		pkStatic->SetWindowText(szBuf);
		pkStatic->Show();

		iCharNumPos += m_rcCharNumBase.bottom + m_iCharNumInterval;

		pkStatic = new SPWindowStatic(WIID_LOBBY_SERVERLIST_STATE + i,
			m_rcServerStateBase.left, iServerStatePos,
			m_rcServerStateBase.right, m_rcServerStateBase.bottom, pkServerWindow);

		RECT rcSrc = m_rcSrcState[vWorldList[i].status];
		pkStatic->SetImage("DATA/LOBBY/RB001.PNG", rcSrc.left, rcSrc.top);
		pkStatic->Show();

		iServerStatePos += m_rcServerStateBase.bottom + m_iServerStateInterval;
	}
	OnSlider(0, 0) ;		// ServerList Update

	// SNDA Notice
	// It Need One Time Notice!!!
	if( SPLocalizeManager::GetInstance()->GetSiteType() == SITE_SANDA && g_pNetworkManager->FDS_IsNeed_Application() && !g_pNetworkManager->FDS_IsShowNotice() )	{
		if( g_pResourceManager->GetGlobalString(15000001 ) ){
			SetInfomationDialog(true, NOTICE_TYPE_YES, g_pResourceManager->GetGlobalString(15000001 ));
		}
		else	{
			SetInfomationDialog(true, NOTICE_TYPE_YES, "Need Authentication!!!");
		}
		g_pNetworkManager->FDS_SetNotice(true) ;
	}
}

//------------------------------------------------------------------------------------
void SPWindowLobby::ClearServerList()
{
	SPWindow* pkServerWindow;
	pkServerWindow = Find(WIID_LOBBY_SERVERLIST_SKIN, true);

	if( pkServerWindow == NULL )
		return;

	pkServerWindow->CleanChildAll();
	m_iSelectServer = -1;
	m_pSlider = NULL ;
}

//------------------------------------------------------------------------------------
void SPWindowLobby::SetChannelList(WORLD_INFO* pServer, vector< CHANNEL_INFO >& vChannelList)
{
	ClearChannelList();

	SPWindow* pkChannelWindow;
	
	pkChannelWindow = Find(WIID_LOBBY_CHANNELLIST_SKINTOP, true);

	if( pkChannelWindow == NULL )
		return;

	int j;
	int iRow = 0 ;
	int iBtnCount = 0 ;
	int iSize = (int)vChannelList.size();
	int iRowSize = iSize / m_CHANNEL_MAXX ;			// 세로줄 라인수
	int iLeftSize = iSize % m_CHANNEL_MAXX ;				// 마지막줄 남은 개수
	//if( iLeftSize != 0 )		iRowSize += 1 ;		// 남은 개수가 없다면 5의 배수

	// Move ChannelList Title
	// TitleTop = ( WinHeight - ( Top + (Middle * Row) + Bottom) ) / 2
	/*int iTitleTop = ( 600 - ( 39 + 50 * iRowSize + 13 + 50 ) ) / 2 ;		
	pkChannelWindow->AdjustCoord(130, iTitleTop, 349, 49) ;*/
	

	SPWindow* pkChannelMiddle;
	SPWindowButton* pkButton;
	SPWindowStatic* pkStatic;

	int iChannelButtonPosX = m_rcChannelButtonBase.left;
	int iChannelButtonPosY = m_rcChannelButtonBase.top;
	int iChannelStatePosX = m_rcChannelStateBase.left;
	int iChannelStatePosY = m_rcChannelStateBase.top;

	
	for( iRow = 0; iRow < iRowSize; iRow++)
	{
		// (WIID_LOBBY_CHANNELLIST_SKINTOP, 130, 162, 325, 288, pkServerWindow);
		pkChannelMiddle = new SPWindowStatic(WIID_LOBBY_CHANNELLIST_SKINMDL+iRow, 0, 39+iRow*50, 349, 50, pkChannelWindow);
		pkChannelMiddle->SetImage("DATA/LOBBY/RB001.PNG", 1, 427);
		pkChannelMiddle->Show() ;

		for(j = 0; j < m_CHANNEL_MAXX; j++)
		{
			iChannelButtonPosX  = m_rcChannelButtonBase.left + m_iCHANNEL_BTN_HORIZEN_INTERVAL * j ;
			iChannelStatePosX = m_rcChannelStateBase.left + m_iCHANNEL_BTN_HORIZEN_INTERVAL * j;

			pkButton = new SPWindowButton(WIID_LOBBY_CHANNELLIST_BUTTON + iBtnCount,
				iChannelButtonPosX, iChannelButtonPosY,
				m_rcChannelButtonBase.right, m_rcChannelButtonBase.bottom, pkChannelMiddle);

			pkButton->SetFontColor(D3DXCOLOR(0.5607f, 0.2980f, 0.3803f, 1.0f));

			pkButton->SetWindowText(vChannelList[iBtnCount].name);
			pkButton->SetImageHit("DATA/LOBBY/RB001.PNG", 456, 47);
			pkButton->SetImagePush("DATA/LOBBY/RB001.PNG", 456, 70);
			pkButton->SetImageCheck("DATA/LOBBY/RB001.PNG", 456, 70);
			pkButton->SetImageDisable("DATA/LOBBY/RB001.PNG", 456, 47);
			pkButton->Show();

			if( vChannelList[iBtnCount].status == 3 )
				pkButton->SetEnable(false);
			else
				pkButton->SetEnable(true);

			pkStatic = new SPWindowStatic(WIID_LOBBY_CHANNELLIST_STATE + iBtnCount,
				iChannelStatePosX, iChannelStatePosY,
				m_rcChannelStateBase.right, m_rcChannelStateBase.bottom, pkChannelMiddle);

			RECT rcSrc = m_rcSrcState[vChannelList[iBtnCount].status];
			pkStatic->SetImage("DATA/LOBBY/RB001.PNG", rcSrc.left, rcSrc.top);
			pkStatic->Show();

			++iBtnCount ;
		}
	}
	if( iLeftSize != 0 )		// Left Button Process
	{
		pkChannelMiddle = new SPWindowStatic(WIID_LOBBY_CHANNELLIST_SKINMDL+iRow, 0, 39+iRow*50, 349, 50, pkChannelWindow);
		pkChannelMiddle->SetImage("DATA/LOBBY/RB001.PNG", 1, 427);
		pkChannelMiddle->Show() ;

		for(j = 0; j < iLeftSize; j++)
		{
			iChannelButtonPosX  = m_rcChannelButtonBase.left + m_iCHANNEL_BTN_HORIZEN_INTERVAL * j;
			iChannelStatePosX = m_rcChannelStateBase.left + m_iCHANNEL_BTN_HORIZEN_INTERVAL * j;

			pkButton = new SPWindowButton(WIID_LOBBY_CHANNELLIST_BUTTON + iBtnCount,
				iChannelButtonPosX, iChannelButtonPosY,
				m_rcChannelButtonBase.right, m_rcChannelButtonBase.bottom, pkChannelMiddle);

			pkButton->SetFontColor(D3DXCOLOR(0.5607f, 0.2980f, 0.3803f, 1.0f));

			pkButton->SetWindowText(vChannelList[iBtnCount].name);
			pkButton->SetImageHit("DATA/LOBBY/RB001.PNG", 456, 47);
			pkButton->SetImagePush("DATA/LOBBY/RB001.PNG", 456, 70);
			pkButton->SetImageCheck("DATA/LOBBY/RB001.PNG", 456, 70);
			pkButton->SetImageDisable("DATA/LOBBY/RB001.PNG", 456, 47);
			pkButton->Show();

			if( vChannelList[iBtnCount].status == 3 )
				pkButton->SetEnable(false);
			else
				pkButton->SetEnable(true);

			pkStatic = new SPWindowStatic(WIID_LOBBY_CHANNELLIST_STATE + iBtnCount,
				iChannelStatePosX, iChannelStatePosY,
				m_rcChannelStateBase.right, m_rcChannelStateBase.bottom, pkChannelMiddle);

			RECT rcSrc = m_rcSrcState[vChannelList[iBtnCount].status];
			pkStatic->SetImage("DATA/LOBBY/RB001.PNG", rcSrc.left, rcSrc.top);
			pkStatic->Show();

			++iBtnCount ;
		}
		++iRowSize ;
	}

	// List is Empty -> Only Background
	if( iSize == 0 )
	{
		pkChannelMiddle = new SPWindowStatic(WIID_LOBBY_CHANNELLIST_SKINMDL, 0, 39, 349, 50, pkChannelWindow);
		pkChannelMiddle->SetImage("DATA/LOBBY/RB001.PNG", 1, 427);
		pkChannelMiddle->Show() ;
		++iRowSize ;
	}
	
	pkStatic = new SPWindowStatic(WIID_LOBBY_CHANNELLIST_SKINBTM, 0, 39+iRowSize*50, 349, 13, pkChannelWindow);
	pkStatic->SetImage("DATA/LOBBY/RB001.PNG", 1, 478);
	pkStatic->Show() ;

	m_pCurServerInfo = pServer;
	pkStatic = new SPWindowStatic(WIID_LOBBY_CHANNELLIST_ICON, 24, 4, 32, 32, pkChannelWindow);
	if( m_pCurServerInfo )
	{
		int iSrcIconX = ((m_pCurServerInfo->wid - 1) % 8) * m_rcServerIconBase.right;
		int iSrcIconY = ((m_pCurServerInfo->wid - 1) / 8) * m_rcServerIconBase.right;
		pkStatic->SetImage("DATA/LOBBY/SERVERICON001.PNG", iSrcIconX, iSrcIconY);
		pkStatic->Show();
	}
	
	pkStatic = new SPWindowStatic(WIID_LOBBY_CHANNELLIST_SERVER, 66, 14, 128, 16, pkChannelWindow);
	pkStatic->SetFontColor(0xFFFFFFFF);
	pkStatic->SetWindowText(pServer->name);
	pkStatic->Show();
	
	pkStatic = new SPWindowStatic(WIID_LOBBY_CHANNELLIST_CONNECT_BG, 160, 360, 106, 52, pkChannelWindow);
	pkStatic->SetImage("DATA/LOBBY/RB001.PNG", 1, 492);
	pkStatic->Show();

	pkButton = new SPWindowButton(WIID_LOBBY_CHANNELLIST_CONNECT, 20, 11, 66, 28, pkStatic);
	pkButton->SetImageHit("DATA/LOBBY/RB001.PNG", 121, 492);
	pkButton->SetImagePush("DATA/LOBBY/RB001.PNG", 121, 521);
	pkButton->Show();

}

//------------------------------------------------------------------------------------
void SPWindowLobby::ClearChannelList()
{
	SPWindow* pkChannelWindow;
	pkChannelWindow = Find(WIID_LOBBY_CHANNELLIST_SKINTOP, true);

	if( pkChannelWindow == NULL )
		return;

	pkChannelWindow->CleanChildAll();
	m_iSelectChannel = -1;
}

//------------------------------------------------------------------------------------
void SPWindowLobby::SetCharacterList(std::vector< CHARACTER_INFO2 >& vCharInfo,
	map< unsigned int, std::vector< CONTAINER_ITEM > >& mItem)
{
	int iNum, i;
	int iCardPosX, iCardPosY;
	
	ClearChracterList();
	iNum = (int)vCharInfo.size();
	for( i = 0; i < iNum; i++ )
	{
		iCardPosX = m_rcDstCardBase.left + (m_rcDstCardBase.right + m_iCardInterval) * i;
		iCardPosY = m_rcDstCardBase.top;

		m_pkMyCharacter[i] = new SPPlayer;
		m_pkMyCharacter[i]->SetLobbyAvatar(LOBBY_INFO);
		m_pkMyCharacter[i]->Init();
		//m_pkMyCharacter[i]->SetAction(ACTION_STOP);
		m_pkMyCharacter[i]->SetShowValue(false, false, false);
		m_pkMyCharacter[i]->SetPos((float)(iCardPosX + 66), (float)(iCardPosY + 134));
		
		m_vMyCharacterInfo.push_back(vCharInfo[i]);
		m_pkMyCharacter[i]->SPGOBPerformMessage(SPGM_ITEM_SC_USE_WEAPON_SET, vCharInfo[i].uiWeaponSet + 1);

		if( m_iType == 1 )
		{
			map< unsigned int, std::vector< CONTAINER_ITEM > >::iterator iterKey;
			iterKey = mItem.find(i);
			if( iterKey != mItem.end() )
			{
				std::vector< CONTAINER_ITEM >::iterator iterValue = iterKey->second.begin();
				while( iterValue != iterKey->second.end() )
				{
					if( g_pItemCluster->GetItemDuration(iterValue->Item.iItemID) != ITEM_DURATION_NULL &&
						iterValue->Item.iLifeTime <= 0 )
					{
						++iterValue;
						continue;
					}
	
					if( iterValue->SlotInfo.ContainerType == FIGURE_BASIC )
						m_pkMyCharacter[i]->SPGOBPerformMessage( SPGM_EQUIP_DEFAULT_CHANGE, (LPARAM)&(*iterValue));
					else
						m_pkMyCharacter[i]->SPGOBPerformMessage( SPGM_EQUIP_CHANGE, (LPARAM)&(*iterValue));
					
					++iterValue;
				}
			}
			
			m_pkMyCharacter[i]->SPGOBPerformMessage(SPGM_ITEM_SC_USE_WEAPON_CHANGE, 0);
		}
		else
			m_mTempCharacterItem = mItem;

		SetAniSeq(i, vCharInfo[i].uiClassType, vCharInfo[i].uiSex, true, false);
	}

	m_iCharacterNum = iNum;
	m_iSelectCard = -1;
}

//------------------------------------------------------------------------------------
void SPWindowLobby::SetMyCharacterItem(int iIndex,
	map< unsigned int, std::vector< CONTAINER_ITEM > >& mItem)
{
	if( m_pkMyCharacter[iIndex] == NULL )
		return;
	
	map< unsigned int, std::vector< CONTAINER_ITEM > >::iterator iterKey;
	iterKey = mItem.find(iIndex);
	if( iterKey != mItem.end() )
	{
		std::vector< CONTAINER_ITEM >::iterator iterValue = iterKey->second.begin();
		while( iterValue != iterKey->second.end() )
		{
			if( g_pItemCluster->GetItemDuration(iterValue->Item.iItemID) != ITEM_DURATION_NULL &&
				iterValue->Item.iLifeTime <= 0 )
			{
				++iterValue;
				continue;
			}
			
			if( iterValue->SlotInfo.ContainerType == FIGURE_BASIC )
				m_pkMyCharacter[iIndex]->SPGOBPerformMessage( SPGM_EQUIP_DEFAULT_CHANGE, (LPARAM)&(*iterValue));
			else
				m_pkMyCharacter[iIndex]->SPGOBPerformMessage( SPGM_EQUIP_CHANGE, (LPARAM)&(*iterValue));

			++iterValue;
		}
	}

	m_pkMyCharacter[iIndex]->SPGOBPerformMessage(SPGM_ITEM_SC_USE_WEAPON_CHANGE, 0);
	SetAniSeq(iIndex, m_vMyCharacterInfo[iIndex].uiClassType, m_vMyCharacterInfo[iIndex].uiSex, true, false);
}

//------------------------------------------------------------------------------------
void SPWindowLobby::SetAniSeq(int iIndex, int iClassType, int iSex, bool bInit, bool bSelect)
{
	if( m_pkMyCharacter[iIndex] == NULL )
		return;

	SPGOBModelUnit*	pGOBModel = m_pkMyCharacter[iIndex]->GetModelUnit();
	
	unsigned int uiAniIndex;
	uiAniIndex = iClassType * 10;

	bool bLoop = false;
	if( bSelect )
		uiAniIndex += 1;
	else
		uiAniIndex += 3;
	
	if( !bInit )
	{
		bLoop = true;
		uiAniIndex += 1;
	}

	if( iSex == 2 )
		uiAniIndex += 4;

	pGOBModel->SPSendMessage(MV_SET_ANIMATION_INDEX, uiAniIndex);
	pGOBModel->SPSendMessage(MV_SET_ANI_LOOP, bLoop);

	m_MyCharacterAniTime[iIndex].bInit = bInit;
	m_MyCharacterAniTime[iIndex].fTime = 0.0f;
	pGOBModel->SPSendMessage(MV_GET_ANI_ACCUMUL_TIME, (WPARAM)&m_MyCharacterAniTime[iIndex].fActionTime);
}

//------------------------------------------------------------------------------------
void SPWindowLobby::InitLobbyType()
{
	m_iType = -1;
	m_iNextType = -1;

	m_bCardRender = false;
	ClearServerList();
	ClearChannelList();

	ClearChracterList();
	ClearImageProcessor();
}

//------------------------------------------------------------------------------------
void SPWindowLobby::ClearChracterList()
{
	int i;
	for( i = 0; i < 3; i++ )
	{
		SAFE_DELETE(m_pkMyCharacter[i]);
	}

	m_iCharacterNum = 0;
	m_vMyCharacterInfo.clear();
	m_mTempCharacterItem.clear();

	m_fCardProcessingTime = 0.0f;
	m_iCardProcessingCount = 0;
	m_iCardProcessingCompleteCount = 0;

	ZeroMemory(m_MyCharacterAniTime, sizeof(CHAR_ANI_TIME) * 3);
}

//------------------------------------------------------------------------------------
void SPWindowLobby::SetImageProcessor(int iType, bool bShow)
{
	SPWindow* pkWindow;
	switch( iType )
	{
	case 0:
		SetServerImageProcessor(bShow);
		break;

	case 1:
		SetCharSelectImageProcessor(bShow);
		break;

	case 2:
		SetCharCreateImageProcessor(bShow);
		break;
	}

	pkWindow = Find(WIID_LOBBY_SERVER_TITLE, true);
	if( pkWindow )
		pkWindow->Hide();

	pkWindow = Find(WIID_LOBBY_CHANNELLIST_SKINTOP, true);
	if( pkWindow )
		pkWindow->Hide();

	pkWindow = Find(WIID_LOBBY_CHAR_TITLE, true);
	if( pkWindow )
		pkWindow->Hide();
		
	pkWindow = Find(WIID_LOBBY_CHAR_CREATE_TITLE, true);
	if( pkWindow )
		pkWindow->Hide();

	if( bShow )
		m_iImageProcessorType = 1;
	else
		m_iImageProcessorType = 0;
}

//------------------------------------------------------------------------------------
void SPWindowLobby::SetServerImageProcessor(bool bShow)
{
	RECT rcSrc;
	SPImageProcessor* pkImageProcessor;
	SPWindow* pkWindow;
	float fStrPosX, fStrPosY, fDstPosX, fDstPosY;

	if( bShow )
	{
		fStrPosX = 284.0f;		fStrPosY = -42.0f;
		fDstPosX = 284.0f;		fDstPosY = 8.0f;
	}
	else
	{
		fStrPosX = 284.0f;		fStrPosY = 8.0f;
		fDstPosX = 284.0f;		fDstPosY = -42.0f;
	}

	SetRect(&rcSrc, 1, 957, 224, 1023);
	pkImageProcessor = new SPWindowPresentation(WIID_LOBBY_SERVER_TITLE,
		"DATA/LOBBY/RB001.PNG", rcSrc, fStrPosX, fStrPosY, fDstPosX, fDstPosY, 0.3f, bShow);

	if( pkImageProcessor )
		m_vImageProcessor.push_back(pkImageProcessor);

	if( bShow )
	{
		fStrPosX = -50.0f;		fStrPosY = 79.0f;
		fDstPosX = 52.0f;		fDstPosY = 79.0f;
	}
	else
	{
		fStrPosX = 52.0f;		fStrPosY = 79.0f;
		fDstPosX = -50.0f;		fDstPosY =79.0f;
	}

	SetRect(&rcSrc, 1, 1, 364, 386);
	pkImageProcessor = new SPWindowPresentation(WIID_LOBBY_SERVERLIST_SKIN,
		"DATA/LOBBY/RB001.PNG", rcSrc, fStrPosX, fStrPosY, fDstPosX, fDstPosY, 0.3f, bShow);

	if( pkImageProcessor )
		m_vImageProcessor.push_back(pkImageProcessor);

	if( !bShow )
	{
		pkWindow = Find(WIID_LOBBY_CHANNELLIST_SKINTOP, true);
		if( pkWindow && pkWindow->IsShow() )
		{
			fStrPosX = 414.0f;		fStrPosY = 174.0f;
			fDstPosX = 474.0f;		fDstPosY = 174.0f;

			//SetRect(&rcSrc, 1, 492, 326, 780);
			SetRect(&rcSrc, 1, 387, 1+349, 387+39);		//일단 타이틀만 Animation
			pkImageProcessor = new SPWindowPresentation(WIID_LOBBY_CHANNELLIST_SKINTOP,
				"DATA/LOBBY/RB001.PNG", rcSrc, fStrPosX, fStrPosY, fDstPosX, fDstPosY, 0.3f, bShow);

			if( pkImageProcessor )
				m_vImageProcessor.push_back(pkImageProcessor);
		}
	}
}

//------------------------------------------------------------------------------------
void SPWindowLobby::SetCharSelectImageProcessor(bool bShow)
{
	RECT rcSrc;
	SPImageProcessor* pkImageProcessor;
	float fStrPosX, fStrPosY, fDstPosX, fDstPosY;

	if( bShow )
	{
		fStrPosX = 168.0f;		fStrPosY = 10.0f;
		fDstPosX = 168.0f;		fDstPosY = 60.0f;
	}
	else
	{
		fStrPosX = 168.0f;		fStrPosY = 60.0f;
		fDstPosX = 168.0f;		fDstPosY = 10.0f;
	}

	SetRect(&rcSrc, 1, 825, 225, 889);
	pkImageProcessor = new SPWindowPresentation(WIID_LOBBY_CHAR_SERVER_SKIN,
		"DATA/LOBBY/RB001.PNG", rcSrc, fStrPosX, fStrPosY, fDstPosX, fDstPosY, 0.3f, bShow);

	if( pkImageProcessor )
		m_vImageProcessor.push_back(pkImageProcessor);

	if( bShow )
	{
		fStrPosX = 10.0f;		fStrPosY = 118.0f;
		fDstPosX = 110.0f;		fDstPosY = 118.0f;
	}
	else
	{
		fStrPosX = 110.0f;		fStrPosY = 118.0f;
		fDstPosX = 10.0f;		fDstPosY = 118.0f;
	}

	SetRect(&rcSrc, 445, 712, 1023, 1023);
	pkImageProcessor = new SPWindowPresentation(WIID_LOBBY_CHAR_SKIN,
		"DATA/LOBBY/RB001.PNG", rcSrc, fStrPosX, fStrPosY, fDstPosX, fDstPosY, 0.3f, bShow);

	if( pkImageProcessor )
		m_vImageProcessor.push_back(pkImageProcessor);

	if( bShow )
	{
		fStrPosX = 408.0f;		fStrPosY = 404.0f;
		fDstPosX = 308.0f;		fDstPosY = 404.0f;
	}
	else
	{
		fStrPosX = 308.0f;		fStrPosY = 404.0f;
		fDstPosX = 408.0f;		fDstPosY = 404.0f;
	}

	SetRect(&rcSrc, 839, 1, 1023, 145);
	pkImageProcessor = new SPWindowPresentation(WIID_LOBBY_CHAR_MENU_SKIN,
		"DATA/LOBBY/RB001.PNG", rcSrc, fStrPosX, fStrPosY, fDstPosX, fDstPosY, 0.3f, bShow);

	if( pkImageProcessor )
		m_vImageProcessor.push_back(pkImageProcessor);

	m_iSelectCard = -1;
	m_bCardRender = false;

	m_iCardProcessing = 1;
}

//------------------------------------------------------------------------------------
void SPWindowLobby::SetCharCreateImageProcessor(bool bShow)
{
	RECT rcSrc;
	SPImageProcessor* pkImageProcessor;
	float fStrPosX, fStrPosY, fDstPosX, fDstPosY;

	if( bShow )
	{
		fStrPosX = 284.0f;		fStrPosY = -42.0f;
		fDstPosX = 284.0f;		fDstPosY = 8.0f;
	}
	else
	{
		fStrPosX = 284.0f;		fStrPosY = 8.0f;
		fDstPosX = 284.0f;		fDstPosY = -42.0f;
	}

	SetRect(&rcSrc, 1, 890, 224, 956);
	pkImageProcessor = new SPWindowPresentation(WIID_LOBBY_CHAR_CREATE_TITLE,
		"DATA/LOBBY/RB001.PNG", rcSrc, fStrPosX, fStrPosY, fDstPosX, fDstPosY, 0.3f, bShow);

	if( pkImageProcessor )
		m_vImageProcessor.push_back(pkImageProcessor);

	if( bShow )
	{
		fStrPosX = -132.0f;		fStrPosY = 112.0f;
		fDstPosX = 32.0f;		fDstPosY = 112.0f;
	}
	else
	{
		fStrPosX = 32.0f;		fStrPosY = 112.0f;
		fDstPosX = -132.0f;		fDstPosY = 112.0f;
	}

	SetRect(&rcSrc, 500, 281, 756, 674);
	pkImageProcessor = new SPWindowPresentation(WIID_LOBBY_CHAR_CREATE_INFO_SKIN,
		"DATA/LOBBY/RB001.PNG", rcSrc, fStrPosX, fStrPosY, fDstPosX, fDstPosY, 0.3f, bShow);

	if( pkImageProcessor )
		m_vImageProcessor.push_back(pkImageProcessor);

	if( bShow )
	{
		fStrPosX = 303.0f;		fStrPosY = 232.0f;
		fDstPosX = 303.0f;		fDstPosY = 182.0f;
	}
	else
	{
		fStrPosX = 303.0f;		fStrPosY = 182.0f;
		fDstPosX = 303.0f;		fDstPosY = 232.0f;
	}

	SetRect(&rcSrc, 509, 1, 703, 280);
	pkImageProcessor = new SPWindowPresentation(WIID_LOBBY_CHAR_CREATE_SKIN,
		"DATA/LOBBY/RB001.PNG", rcSrc, fStrPosX, fStrPosY, fDstPosX, fDstPosY, 0.3f, bShow);

	if( pkImageProcessor )
		m_vImageProcessor.push_back(pkImageProcessor);

	if( bShow )
	{
		fStrPosX = 612.0f;		fStrPosY = 90.0f;
		fDstPosX = 512.0f;		fDstPosY = 90.0f;
	}
	else
	{
		fStrPosX = 512.0f;		fStrPosY = 90.0f;
		fDstPosX = 612.0f;		fDstPosY = 90.0f;
	}

	SetRect(&rcSrc, 767, 318, 1023, 711);
	pkImageProcessor = new SPWindowPresentation(WIID_LOBBY_CHAR_CREATE_CHANGE_SKIN,
		"DATA/LOBBY/RB001.PNG", rcSrc, fStrPosX, fStrPosY, fDstPosX, fDstPosY, 0.3f, bShow);

	if( pkImageProcessor )
		m_vImageProcessor.push_back(pkImageProcessor);
}

//------------------------------------------------------------------------------------
void SPWindowLobby::SetCardImageProcessor(float fElapsedTime)
{
	m_fCardProcessingTime += fElapsedTime;
	if( m_fCardProcessingTime > 0.2f )
	{
		SPImageProcessor* pkImageProcessor;
		float fStrPosX, fStrPosY, fDstPosX, fDstPosY;

		fStrPosX = m_rcDstCardBase.left + (m_rcDstCardBase.right + m_iCardInterval) * m_iCardProcessingCount;
		fDstPosX = fStrPosX;
		
		fStrPosY = (float)m_rcDstCardBase.top - 50.0f;
		fDstPosY = (float)m_rcDstCardBase.top;
		
		pkImageProcessor = new SPWindowPresentation(0xFFFFFFFF,
			"DATA/LOBBY/RB001.PNG", m_rcSrcCardBack, fStrPosX, fStrPosY, fDstPosX, fDstPosY, 0.3f, true);

		if( pkImageProcessor )
			m_vImageProcessor.push_back(pkImageProcessor);

		m_fCardProcessingTime = 0.0f;
		m_iCardProcessingCount++;
		if( m_iCardProcessingCount >= 3 )
		{
			m_iCardProcessing = 0;
			m_iImageProcessorType = 1;
		}
	}
}

//------------------------------------------------------------------------------------
void SPWindowLobby::ProcessImageProcessor(float fElapsedTime)
{
	unsigned int uiWIID = 0;
	SPWindow* pkWindow = NULL;

	vector< SPImageProcessor* >::iterator iter = m_vImageProcessor.begin();
	while( iter != m_vImageProcessor.end() )
	{
		uiWIID = (*iter)->Process(fElapsedTime);
		if( uiWIID )
		{
			if( m_iImageProcessorType == 1 )
			{
				pkWindow = Find(uiWIID, true);
				if( pkWindow )
					pkWindow->Show();

				if( uiWIID == WIID_LOBBY_SERVER_TITLE )
				{
					pkWindow = Find(WIID_LOBBY_CHANNELLIST_SKINTOP, true);
					if( pkWindow && pkWindow->IsShow() )
						pkWindow->Hide();
				}
			}

			if( uiWIID == 0xFFFFFFFF )
				m_iCardProcessingCompleteCount++;

			delete (*iter);
			iter = m_vImageProcessor.erase(iter);
			continue;
		}

		++iter;
	}

	if( m_vImageProcessor.empty() )
	{
		if( m_iImageProcessorType == 0 )
		{
			if( m_iNextType == -1 )
			{
				g_pSubGameManager->SPPerformMessage(SPGM_SERVER_BACK, 0, 0);
				m_iImageProcessorType = -1;
				m_iType = m_iNextType;
				m_bWaitChageSubGame = false;
			}
			else
			{
				SetImageProcessor(m_iNextType, true);
			}
		}
		else if( m_iImageProcessorType == 1 )
		{
			if( m_iNextType == 1 )
			{
				SPWindow* pkWindow = Find(WIID_LOBBY_CHAR_TITLE, true);
				if( pkWindow && !pkWindow->IsShow() )
				{
					pkWindow->Show();

					if( m_pCurServerInfo )
					{
						SPWindow* pkChildWindow;
						pkChildWindow = pkWindow->Find(WIID_LOBBY_CHAR_SERVER_ICON, true);
						
						int iSrcIconX = ((m_pCurServerInfo->wid - 1) % 8) * m_rcServerIconBase.right;
						int iSrcIconY = ((m_pCurServerInfo->wid - 1) / 8) * m_rcServerIconBase.right;
						pkChildWindow->SPSendMessage(SPIM_SET_IMAGECLEAR);
						pkChildWindow->SetImage("DATA/LOBBY/SERVERICON001.PNG", iSrcIconX, iSrcIconY);

						pkChildWindow = pkWindow->Find(WIID_LOBBY_CHAR_SERVER_NAME, true);
						pkChildWindow->SetWindowText(m_pCurServerInfo->name);
					}
				}

				if( m_iCardProcessing == 0 )
				{
					m_bCardRender = true;
					m_iType = m_iNextType;

					if( !m_mTempCharacterItem.empty() )
					{
						for( int i = 0; i < 3; i++ )
							SetMyCharacterItem(i, m_mTempCharacterItem);

						m_mTempCharacterItem.clear();
					}

					SPWindow* pkWindow = Find(WIID_LOBBY_CHAR_MENU_CREATE, true);
					if( pkWindow )
						pkWindow->SetEnable(true);

					pkWindow = Find(WIID_LOBBY_CHAR_MENU_DELETE, true);
					if( pkWindow )
						pkWindow->SetEnable(true);

					pkWindow = Find(WIID_LOBBY_CHAR_BACK, true);
					if( pkWindow )
						pkWindow->SetEnable(true);
					
				}
				else if( m_iCardProcessing == 1 )
				{
					m_fCardProcessingTime = 0.0f;
					m_iCardProcessingCount = 0;
					m_iCardProcessingCompleteCount = 0;
					m_iCardProcessing = 2;

					SPWindow* pkWindow = Find(WIID_LOBBY_CHAR_MENU_CREATE, true);
					if( pkWindow )
						pkWindow->SetEnable(false);

					pkWindow = Find(WIID_LOBBY_CHAR_BACK, true);
					if( pkWindow )
						pkWindow->SetEnable(false);
				}
			}
			else
			{
				m_iType = m_iNextType;
				if( m_iNextType == 2 )
				{
					SetCurSelectLayer();
					
					SPWindowEdit* pkEdit = (SPWindowEdit*)Find(WIID_LOBBY_CHAR_CREATE_INFO_NAME, true);
					if( pkEdit )
						pkEdit->SetWindowText(NULL);
					SetClassWeapon(1) ;
				}
			}
		
			m_iImageProcessorType = -1;
		}
	}

	if( m_iCardProcessing == 2 && m_iCardProcessingCount < 3 )
		SetCardImageProcessor(fElapsedTime);
}

//------------------------------------------------------------------------------------
void SPWindowLobby::RenderImageProcessor()
{
	vector< SPImageProcessor* >::iterator iter = m_vImageProcessor.begin();
	while( iter != m_vImageProcessor.end() )
	{
		(*iter)->Render();
		++iter;
	}
}

//------------------------------------------------------------------------------------
void SPWindowLobby::StatusProcess(float fElapsedTime)
{
	int i;
	for( i = 0; i < 4; i++ )
	{
		if( (int)m_fCurStatusWidth[i] < m_iMaxStatusDst[i] )
		{
			float fDiff = (float)m_iMaxStatusDst[i] - m_fCurStatusWidth[i];
			float fAdd = (fElapsedTime * fDiff * 2.0f);
			if( fDiff < 8.0f && fAdd < 0.15f )//if( fAdd < 2.0f )
				fAdd = 0.15f;

			m_fCurStatusWidth[i] += fAdd;
			if( (int)m_fCurStatusWidth[i] > m_iMaxStatusDst[i] )
				m_fCurStatusWidth[i] = m_iMaxStatusDst[i];

			m_rcStatusDst[i].right = m_rcStatusDst[i].left + (int)m_fCurStatusWidth[i];
		}
	}
}

//------------------------------------------------------------------------------------
void SPWindowLobby::Process(float fTime)
{
	for( int i = 0; i < 3; i++ )
	{
		if( m_pkMyCharacter[i] )
		{
			m_pkMyCharacter[i]->Process(fTime);

			if( m_MyCharacterAniTime[i].bInit )
			{
				m_MyCharacterAniTime[i].fTime += fTime;
				if( m_MyCharacterAniTime[i].fTime > m_MyCharacterAniTime[i].fActionTime )
				{
					bool bSelect;
					if( i == m_iSelectCard )
						bSelect = true;
					else
						bSelect = false;

					SetAniSeq(i, m_vMyCharacterInfo[i].uiClassType, m_vMyCharacterInfo[i].uiSex,
						false, bSelect);
				}
			}
		}
	}

	if( m_iType == 2 )
	{
		m_pkCreateCharacter->Process(fTime);
		StatusProcess(fTime);
	}

	ProcessImageProcessor(fTime);
	SPWindow::Process(fTime);
}

//------------------------------------------------------------------------------------
void SPWindowLobby::Render(float fTime)
{
	if( m_bWaitChageSubGame )
		SPWindow::Render(fTime);

	if( m_vImageProcessor.empty() )
	{
		if( m_bCardRender )
			RenderCard(fTime);

		if( m_iType == 2 && m_iNextType == 2 )
		{
			m_pkCreateCharacter->Render(fTime);
			RenderStatus();
		}

		if( g_pInterfaceManager->GetModalWindow() )
			g_pInterfaceManager->GetModalWindow()->Render(fTime);
	}

	if( m_iNextType == 1 && !m_bCardRender )
		ForceRenderCard(m_iCardProcessingCompleteCount);

	RenderImageProcessor();
}

//------------------------------------------------------------------------------------
void SPWindowLobby::RenderCard(float fTime)
{
	RECT rcDst;
	int iCardPosX, iCardPosY, i;
	char szBuf[32], szNameBuf[LEN_NAME + 1];

	if( g_pVideo == NULL )
		return;

	if( g_pVideo->GetFont() == NULL )
		return;

	bool bTemp = g_pVideo->GetFont()->IsShadow();
	g_pVideo->GetFont()->SetColor(D3DXCOLOR(0,0,0,1));
	g_pVideo->GetFont()->SetShadow(false);

	if( m_iSelectCard >= 0 )
	{
		iCardPosX = m_rcDstCardSelectBase.left + 
			(m_rcDstCardSelectBase.right + m_iCardSelectInterval) * m_iSelectCard;
		iCardPosY = m_rcDstCardSelectBase.top;

		SetRect(&rcDst, iCardPosX, iCardPosY,
			iCardPosX + m_rcDstCardSelectBase.right, iCardPosY + m_rcDstCardSelectBase.bottom);
		m_pkCard->RenderTexture(&rcDst, &m_rcSrcCardSelect);
	}

	for( i = 0; i < 3; i++ )
	{
		iCardPosX = m_rcDstCardBase.left + (m_rcDstCardBase.right + m_iCardInterval) * i;
		iCardPosY = m_rcDstCardBase.top;

		SetRect(&rcDst, iCardPosX, iCardPosY,
			iCardPosX + m_rcDstCardBase.right, iCardPosY + m_rcDstCardBase.bottom);
		if( m_pkMyCharacter[i] == NULL )
		{
			m_pkCard->RenderTexture(&rcDst, &m_rcSrcCardBack);
			continue;
		}

		m_pkCard->RenderTexture(&rcDst, &m_rcSrcCardFront);
		g_pVideo->Flush();

		iCardPosX = m_rcDstCardNumBase.left + (m_rcDstCardBase.right + m_iCardInterval) * i;
		iCardPosY = m_rcDstCardNumBase.top;

		SetRect(&rcDst, iCardPosX, iCardPosY,
			iCardPosX + m_rcDstCardNumBase.right, iCardPosY + m_rcDstCardNumBase.bottom);
		m_pkCard->RenderTexture(&rcDst, &m_rcSrcCardNum[i]);
		g_pVideo->Flush();

		m_pkMyCharacter[i]->Render(fTime);
		g_pVideo->Flush();

		iCardPosX = m_rcDstCardBase.left + (m_rcDstCardBase.right + m_iCardInterval) * i;
		iCardPosY = m_rcDstCardBase.top;
		SetRect(&rcDst, iCardPosX + 12, iCardPosY + 167, iCardPosX + 118, iCardPosY + 181);

		SPFont* pkFont = g_pVideo->GetFont(FONT_12_BOLD);
		if( pkFont == NULL )
			continue;

		pkFont->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		pkFont->SetPeriod(true);
		
		szNameBuf[LEN_NAME] = '\0';
		strncpy(szNameBuf, m_vMyCharacterInfo[i].BaseInfo.szName, LEN_NAME);
		pkFont->RenderText(szNameBuf, &rcDst, DT_CENTER | DT_VCENTER);

		pkFont->SetColor();
		pkFont->SetPeriod(false);

		pkFont = g_pVideo->GetFont(FONT_12_NORMAL);
		if( pkFont == NULL )
			continue;
		
		pkFont->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		SetRect(&rcDst, iCardPosX + 13, iCardPosY + 183, iCardPosX + 59, iCardPosY + 197);
		sprintf(szBuf, g_pResourceManager->GetGlobalString(20000), m_vMyCharacterInfo[i].BaseInfo.uiLevel);
		pkFont->RenderText(szBuf, &rcDst, DT_LEFT);

		SetRect(&rcDst, iCardPosX + 57, iCardPosY + 183, iCardPosX + 117, iCardPosY + 197);
		pkFont->RenderText(
			g_pResourceManager->GetGlobalString(m_vMyCharacterInfo[i].uiClassType + 20000), &rcDst, DT_RIGHT);

		pkFont->SetColor();
	}

	g_pVideo->GetFont()->SetColor();
	g_pVideo->GetFont()->SetShadow(	bTemp );

}

//------------------------------------------------------------------------------------
void SPWindowLobby::ForceRenderCard(int iNum)
{
	int iCardPosX, iCardPosY, i;
	RECT rcDst;

	if( m_pkCard == NULL )
		return;

	for( i = 0; i < iNum; i++ )
	{
		iCardPosX = m_rcDstCardBase.left + (m_rcDstCardBase.right + m_iCardInterval) * i;
		iCardPosY = m_rcDstCardBase.top;

		SetRect(&rcDst, iCardPosX, iCardPosY,
			iCardPosX + m_rcDstCardBase.right, iCardPosY + m_rcDstCardBase.bottom);
		
		m_pkCard->RenderTexture(&rcDst, &m_rcSrcCardBack);
	}
}

//------------------------------------------------------------------------------------
void SPWindowLobby::RenderStatus()
{
	if( m_pkStatusSkin == NULL )
		return;

	int i;
	
	m_pkStatusSkin->RenderTexture(&m_rcStatusSkinDst, &m_rcStatusSkinSrc);
	g_pVideo->Flush();

	for( i = 0; i < 4; i++ )
	{
		m_pkStatusSkin->RenderTexture(&m_rcStatusDst[i], &m_rcStatusSrc[i]);
		g_pVideo->Flush();
	}
}

//------------------------------------------------------------------------------------
void SPWindowLobby::Show(bool bWithChild)
{
	SPWindow::Show(bWithChild);
	//ShowLobbyType(m_iType);

	// 접속할때 초기화 해야 하는것들~ [1/25/2006 AJJIYA]
	//g_pInterfaceManager->SPChildWndSendMessage( WIID_SHORTCUT	, SPIM_PURGE , NULL , NULL );
	g_pInterfaceManager->SPChildWndSendMessage( WIID_HELP		, SPIM_PURGE , NULL , NULL );
	g_pInterfaceManager->SPChildWndSendMessage( WIID_CHAT		, SPIM_PURGE , NULL , NULL );
	g_pInterfaceManager->SPChildWndSendMessage( WIID_WORLDMAP	, SPIM_PURGE , NULL , NULL );
	g_pInterfaceManager->SPChildWndSendMessage( WIID_LOTTERY	, SPIM_PURGE , NULL , NULL );
	g_pInterfaceManager->SPChildWndSendMessage( WIID_QUICK_SLOT	, SPIM_PURGE , (WPARAM)1 , NULL );
	g_pInterfaceManager->SPChildWndSendMessage( WIID_SYSTEM_MESSAGE	, SPIM_PURGE , NULL , NULL );
	g_pInterfaceManager->SPChildWndSendMessage( WIID_GAMBLE		, SPIM_PURGE , NULL , NULL );
	g_pInterfaceManager->SPChildWndSendMessage( WIID_CHATTING_ROOM	, SPIM_PURGE , NULL , NULL );

	g_pItemCluster->SetItemUpgradeState( SPIU_STATE_NULL );
}

//------------------------------------------------------------------------------------
void SPWindowLobby::SetStatusGage(int iType)
{
	SetRect(&m_rcStatusDst[0], 209, 500, 209, 506);
	SetRect(&m_rcStatusDst[1], 209, 518, 209, 524);
	SetRect(&m_rcStatusDst[2], 209, 536, 209, 542);
	SetRect(&m_rcStatusDst[3], 209, 554, 209, 560);

	m_fCurStatusWidth[0] = 0.0f;
	m_fCurStatusWidth[1] = 0.0f;
	m_fCurStatusWidth[2] = 0.0f;
	m_fCurStatusWidth[3] = 0.0f;

	float fRatio = 2.0f;
	switch( iType )
	{
	case 1:
		m_iMaxStatusDst[0] = (int)((float)m_iMaxStatusWidth * 0.4f * fRatio);
		m_iMaxStatusDst[1] = (int)((float)m_iMaxStatusWidth * 0.3f * fRatio);
		m_iMaxStatusDst[2] = (int)((float)m_iMaxStatusWidth * 0.15f * fRatio);
		m_iMaxStatusDst[3] = (int)((float)m_iMaxStatusWidth * 0.15f * fRatio);
		break;
	case 2:
		m_iMaxStatusDst[0] = (int)((float)m_iMaxStatusWidth * 0.25f * fRatio);
		m_iMaxStatusDst[1] = (int)((float)m_iMaxStatusWidth * 0.4f * fRatio);
		m_iMaxStatusDst[2] = (int)((float)m_iMaxStatusWidth * 0.15f * fRatio);
		m_iMaxStatusDst[3] = (int)((float)m_iMaxStatusWidth * 0.2f * fRatio);
		break;
	case 3:
		m_iMaxStatusDst[0] = (int)((float)m_iMaxStatusWidth * 0.15f * fRatio);
		m_iMaxStatusDst[1] = (int)((float)m_iMaxStatusWidth * 0.2f * fRatio);
		m_iMaxStatusDst[2] = (int)((float)m_iMaxStatusWidth * 0.4f * fRatio);
		m_iMaxStatusDst[3] = (int)((float)m_iMaxStatusWidth * 0.25f * fRatio);
		break;
	case 4:
		m_iMaxStatusDst[0] = (int)((float)m_iMaxStatusWidth * 0.2f * fRatio);
		m_iMaxStatusDst[1] = (int)((float)m_iMaxStatusWidth * 0.2f * fRatio);
		m_iMaxStatusDst[2] = (int)((float)m_iMaxStatusWidth * 0.2f * fRatio);
		m_iMaxStatusDst[3] = (int)((float)m_iMaxStatusWidth * 0.4f * fRatio);
		break;
	}
}

//------------------------------------------------------------------------------------
void SPWindowLobby::SetClassWeapon(int iClassType)
{
	SPWindow* pkWindow = NULL ;
	if( !_SHOW_USE_WEAPON_ )		iClassType = 0 ;
	switch( iClassType )
	{
	case 1:			// 공격형
		if( (pkWindow = Find(WIID_LOBBY_CHAR_CREATE_ICON_TWO_SD, true)) != NULL )
			pkWindow->Show() ;
		if( (pkWindow = Find(WIID_LOBBY_CHAR_CREATE_ICON_SPERE, true)) != NULL )
			pkWindow->Show() ;

		if( (pkWindow = Find(WIID_LOBBY_CHAR_CREATE_ICON_ONE_SD, true)) != NULL )
			pkWindow->Hide() ;
		if( (pkWindow = Find(WIID_LOBBY_CHAR_CREATE_ICON_HAMMER, true)) != NULL )
			pkWindow->Hide() ;
		if( (pkWindow = Find(WIID_LOBBY_CHAR_CREATE_ICON_DAGGER, true)) != NULL )
			pkWindow->Hide() ;
		if( (pkWindow = Find(WIID_LOBBY_CHAR_CREATE_ICON_WAND, true)) != NULL )
			pkWindow->Hide() ;
		if( (pkWindow = Find(WIID_LOBBY_CHAR_CREATE_ICON_BOW, true)) != NULL )
			pkWindow->Hide() ;
		if( (pkWindow = Find(WIID_LOBBY_CHAR_CREATE_ICON_CROSSBOW, true)) != NULL )
			pkWindow->Hide() ;
		break;
	case 2:			// 방어형
		if( (pkWindow = Find(WIID_LOBBY_CHAR_CREATE_ICON_ONE_SD, true)) != NULL )
			pkWindow->Show() ;
		if( (pkWindow = Find(WIID_LOBBY_CHAR_CREATE_ICON_HAMMER, true)) != NULL )
			pkWindow->Show() ;

		if( (pkWindow = Find(WIID_LOBBY_CHAR_CREATE_ICON_TWO_SD, true)) != NULL )
			pkWindow->Hide() ;
		if( (pkWindow = Find(WIID_LOBBY_CHAR_CREATE_ICON_SPERE, true)) != NULL )
			pkWindow->Hide() ;
		if( (pkWindow = Find(WIID_LOBBY_CHAR_CREATE_ICON_DAGGER, true)) != NULL )
			pkWindow->Hide() ;
		if( (pkWindow = Find(WIID_LOBBY_CHAR_CREATE_ICON_WAND, true)) != NULL )
			pkWindow->Hide() ;
		if( (pkWindow = Find(WIID_LOBBY_CHAR_CREATE_ICON_BOW, true)) != NULL )
			pkWindow->Hide() ;
		if( (pkWindow = Find(WIID_LOBBY_CHAR_CREATE_ICON_CROSSBOW, true)) != NULL )
			pkWindow->Hide() ;
		break;
	case 3:				// 마법형
		if( (pkWindow = Find(WIID_LOBBY_CHAR_CREATE_ICON_DAGGER, true)) != NULL )
			pkWindow->Show() ;
		if( (pkWindow = Find(WIID_LOBBY_CHAR_CREATE_ICON_WAND, true)) != NULL )
			pkWindow->Show() ;

		if( (pkWindow = Find(WIID_LOBBY_CHAR_CREATE_ICON_TWO_SD, true)) != NULL )
			pkWindow->Hide() ;
		if( (pkWindow = Find(WIID_LOBBY_CHAR_CREATE_ICON_SPERE, true)) != NULL )
			pkWindow->Hide() ;
		if( (pkWindow = Find(WIID_LOBBY_CHAR_CREATE_ICON_ONE_SD, true)) != NULL )
			pkWindow->Hide() ;
		if( (pkWindow = Find(WIID_LOBBY_CHAR_CREATE_ICON_HAMMER, true)) != NULL )
			pkWindow->Hide() ;
		if( (pkWindow = Find(WIID_LOBBY_CHAR_CREATE_ICON_BOW, true)) != NULL )
			pkWindow->Hide() ;
		if( (pkWindow = Find(WIID_LOBBY_CHAR_CREATE_ICON_CROSSBOW, true)) != NULL )
			pkWindow->Hide() ;
		break;
	case 4:					// 탐색형
		if( (pkWindow = Find(WIID_LOBBY_CHAR_CREATE_ICON_DAGGER, true)) != NULL )
			pkWindow->Show() ;
		if( (pkWindow = Find(WIID_LOBBY_CHAR_CREATE_ICON_BOW, true)) != NULL )
			pkWindow->Show() ;
		if( (pkWindow = Find(WIID_LOBBY_CHAR_CREATE_ICON_CROSSBOW, true)) != NULL )
			pkWindow->Show() ;

		if( (pkWindow = Find(WIID_LOBBY_CHAR_CREATE_ICON_TWO_SD, true)) != NULL )
			pkWindow->Hide() ;
		if( (pkWindow = Find(WIID_LOBBY_CHAR_CREATE_ICON_SPERE, true)) != NULL )
			pkWindow->Hide() ;
		if( (pkWindow = Find(WIID_LOBBY_CHAR_CREATE_ICON_ONE_SD, true)) != NULL )
			pkWindow->Hide() ;
		if( (pkWindow = Find(WIID_LOBBY_CHAR_CREATE_ICON_HAMMER, true)) != NULL )
			pkWindow->Hide() ;
		if( (pkWindow = Find(WIID_LOBBY_CHAR_CREATE_ICON_WAND, true)) != NULL )
			pkWindow->Hide() ;
		break;
	default:
		if( (pkWindow = Find(WIID_LOBBY_CHAR_CREATE_ICON_TWO_SD, true)) != NULL )
			pkWindow->Hide() ;
		if( (pkWindow = Find(WIID_LOBBY_CHAR_CREATE_ICON_SPERE, true)) != NULL )
			pkWindow->Hide() ;
		if( (pkWindow = Find(WIID_LOBBY_CHAR_CREATE_ICON_ONE_SD, true)) != NULL )
			pkWindow->Hide() ;
		if( (pkWindow = Find(WIID_LOBBY_CHAR_CREATE_ICON_HAMMER, true)) != NULL )
			pkWindow->Hide() ;
		if( (pkWindow = Find(WIID_LOBBY_CHAR_CREATE_ICON_BOW, true)) != NULL )
			pkWindow->Hide() ;
		if( (pkWindow = Find(WIID_LOBBY_CHAR_CREATE_ICON_CROSSBOW, true)) != NULL )
			pkWindow->Hide() ;
		if( (pkWindow = Find(WIID_LOBBY_CHAR_CREATE_ICON_DAGGER, true)) != NULL )
			pkWindow->Hide() ;
		if( (pkWindow = Find(WIID_LOBBY_CHAR_CREATE_ICON_WAND, true)) != NULL )
			pkWindow->Hide() ;
		break ;

	}
}


//------------------------------------------------------------------------------------
// Message
int SPWindowLobby::OnCursorEnter(WPARAM, LPARAM)
{
	return 0;
}

//------------------------------------------------------------------------------------
int SPWindowLobby::OnCursorOut(WPARAM, LPARAM)
{
	return 0;
}

//------------------------------------------------------------------------------------
// For ToolTip
int SPWindowLobby::OnWeaponTooltipEnter(unsigned int uiID , WPARAM wParam, LPARAM lParam)
{
	SPWindow* pkWindow = g_pInterfaceManager->GetLBToolTipWindow();

	if( pkWindow == NULL )
		return 1;

	pkWindow->SPSendMessage( SPIM_POS_UPDATE , wParam , lParam );
	pkWindow->SPSendMessage(SPIM_TOOLTIP_ENTER, (WPARAM)(INSTANCE_ID)uiID, (WPARAM)SPWindowToolTip::TOOLTIP_SHOW_TYPE_CTRL);

	return 0;
}
//------------------------------------------------------------------------------------
int SPWindowLobby::OnWeaponTooltipOut(unsigned int, WPARAM wParam, LPARAM lParam)
{
	SPWindow* pkWindow = g_pInterfaceManager->GetLBToolTipWindow();

	if( pkWindow == NULL )
		return 1;

	pkWindow->SPSendMessage(SPIM_TOOLTIP_OUT, NULL, (WPARAM)SPWindowToolTip::TOOLTIP_SHOW_TYPE_CTRL);
	return 1;
}
//------------------------------------------------------------------------------------
int SPWindowLobby::OnWeaponTooltipMove(unsigned int, WPARAM wParam, LPARAM lParam)
{
	SPWindow* pkWindow = g_pInterfaceManager->GetLBToolTipWindow();

	if( pkWindow == NULL )
		return 1;

	pkWindow->SPSendMessage( SPIM_POS_UPDATE , wParam , lParam );
	return 1;
}

//------------------------------------------------------------------------------------
//젯쌈눈뒵륩蛟포뵨틉돛뵀
int SPWindowLobby::OnConnect(WPARAM, LPARAM)
{
	assert(g_pSubGameManager->GetSubGameID() == SUBGAME_LOBBY);

	if( m_iSelectServer < 0 || m_iSelectChannel < 0 )
		return 1;

	g_pSubGameManager->SPPerformMessage(SPGM_GAMESERVER_CONNECT_REQ, m_iSelectServer, m_iSelectChannel);
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLobby::OnServerBack(WPARAM, LPARAM)
{
	ClearImageProcessor();
	ShowLobbyType(-1);
	//g_pSubGameManager->SPPerformMessage(SPGM_SERVER_BACK, 0, 0);
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLobby::OnCharMenuSelect(WPARAM, LPARAM)
{
	if( m_iSelectCard >= 0 )
	{
		SPWindow* pkWindow = Find(WIID_LOBBY_CHAR_MENU_SELECT, true);
		if( pkWindow )
		{
			if( !pkWindow->IsEnable() )
				return 1;

			pkWindow->SetEnable(false);
		}

		g_pSubGameManager->SPPerformMessage(SPGM_CHAR_MENU_SELECT, (WPARAM)m_iSelectCard, 0);
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLobby::OnCharMenuCreate(WPARAM, LPARAM)
{
	if( m_iCharacterNum < 3 )
		g_pSubGameManager->SPPerformMessage(SPGM_CHAR_MENU_CREATE, 0, 0);

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLobby::OnCharMenuDelete(WPARAM, LPARAM)
{
	if( m_iSelectCard >= 0 )
	{
		SPWindow* pkWindow = Find(WIID_LOBBY_DIALOG_JUMIN, true);
		if( pkWindow )
		{
			pkWindow->Show();
			
			SPWindowEdit* pkWindowEdit = (SPWindowEdit*)pkWindow->Find(WIID_LOBBY_DIALOG_JUMIN_EDIT, true);
			
			pkWindowEdit->SetFocus();
			g_pInterfaceManager->SetModalWindow(pkWindow);
			
			//SPWindowButton* pkButton;
			//pkButton = (SPWindowButton*)pkWindow->Find(WIID_LOBBY_DIALOG_JUMIN_OK, true);
			//pkButton->SetEnable(false);
		}
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLobby::OnJuminOk(WPARAM, LPARAM)
{
	SPWindow* pkDialog = Find(WIID_LOBBY_DIALOG_JUMIN, true);
	if( pkDialog )
	{
		SPWindowEdit* pkWindowEdit = (SPWindowEdit*)pkDialog->Find(WIID_LOBBY_DIALOG_JUMIN_EDIT, true);
		string strPass = pkWindowEdit->GetWindowText();

		pkWindowEdit->SetFontColor(0xFFFFFFFF);
		pkWindowEdit->SetFontShadow(true) ;
		pkWindowEdit->SetWindowText("");

		pkDialog->Hide();
		g_pInterfaceManager->SetModalWindow(NULL);

		string strDelete = g_pResourceManager->GetGlobalString(60011);				///> 삭제하겠습니다.
		if( strPass == strDelete )
		{
			SetInfomationDialog(true, NOTICE_TYPE_MSG, g_pResourceManager->GetGlobalString(60010));		///> 캐릭터 삭제중
			g_pSubGameManager->SPPerformMessage(SPGM_CHAR_MENU_DELETE, (WPARAM)m_iSelectCard, (LPARAM)strPass.c_str());
		}
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLobby::OnJuminCancel(WPARAM, LPARAM)
{
	SPWindow* pkDialog = Find(WIID_LOBBY_DIALOG_JUMIN, true);
	if( pkDialog )
	{
		pkDialog->Hide();
		SPWindowEdit* pkWindowEdit = (SPWindowEdit*)pkDialog->Find(WIID_LOBBY_DIALOG_JUMIN_EDIT, true);
		if( pkWindowEdit )
			pkWindowEdit->SetWindowText("");

		g_pInterfaceManager->SetModalWindow(NULL);
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLobby::OnCharBack(WPARAM, LPARAM)
{
	g_pSubGameManager->SPPerformMessage(SPGM_CHAR_BACK, 0, 0);
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLobby::OnCharCrateNameFocus(WPARAM, LPARAM)
{
	//SetCharacterNameCheck(false);
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLobby::OnCharCreateNameCheck(WPARAM, LPARAM)
{
	SPWindowEdit* pkWindow = (SPWindowEdit*)Find(WIID_LOBBY_CHAR_CREATE_INFO_NAME, true);
	if( pkWindow )
	{
		const char* pszName = pkWindow->GetWindowText();
		if( (int)strlen(pszName) > 0 )
		{
			m_strCharacterName = pszName;
			g_pSubGameManager->SPPerformMessage(SPGM_CHAR_CREATE_NAME_CHECK_REQ, (WPARAM)m_strCharacterName.c_str(), 0);
		}
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLobby::OnCharCreateRun(WPARAM, LPARAM)
{
	CHARACTER_INFO2	kCharInfo;
	CHAR_FIGURE kFiqureInfo;
	
	ZeroMemory(&kCharInfo, sizeof(CHARACTER_INFO2));
	ZeroMemory(&kFiqureInfo, sizeof(CHAR_FIGURE));

	kCharInfo.uiClassType = m_iClassType + 1;
	kCharInfo.uiSex = m_iZender + 1;
	kCharInfo.BaseInfo.uiLevel = 1;
	kCharInfo.uiUserID = g_pNetworkManager->GetUserID();

	SPWindowEdit* pkWindow = (SPWindowEdit*)Find(WIID_LOBBY_CHAR_CREATE_INFO_NAME, true);
	m_strCharacterName = pkWindow->GetWindowText();

	int iLength = (int)m_strCharacterName.size();
	if( iLength == 0 || iLength > LEN_NAME )
	{
		SetInfomationDialog(true, NOTICE_TYPE_YES, g_pResourceManager->GetGlobalString(40003));				///> Character Name Length Illegal
		return 1;
	}

	char szBuffer[128];
	int iReplace;
	SPAbuseFilter::GetInstance()->Filter(m_strCharacterName.c_str(), szBuffer, 127, &iReplace);

	bool bRetVal = false;
	
	static COUNTRY_TYPE CountryType = SPLocalizeManager::GetInstance()->GetCountryType();

	switch(CountryType) 
	{
		case CT_BUDDYBUDDY:			// SITE_GAMEPOT
		case CT_KOREA:				// SITE_ACTOZ
			bRetVal = SPAbuseFilter::GetInstance()->IsStringWithSpecialChar(m_strCharacterName.c_str());
			break;

		case CT_JAPAN:				// SITE_GAMEPOT
			bRetVal = SPAbuseFilter::GetInstance()->IsStringWithSpecialCharJapan(m_strCharacterName.c_str());
			break;

		case CT_CHINA:				// SITE_SANDA
			// 중국 버전에 합당한 코딩 필요
			//assert(false && "Please verify CT_CHINA value(CountryType).!!!");
			bRetVal = SPAbuseFilter::GetInstance()->IsStringWithSpecialCharJapan(m_strCharacterName.c_str());
			break;
	}	

	bool bRetVal2 = false;	
	bRetVal2 = SPAbuseFilter::GetInstance()->IsBanName(m_strCharacterName.c_str());


	if( iReplace || bRetVal || bRetVal2 )
	{
		SetInfomationDialog(true, NOTICE_TYPE_YES, g_pResourceManager->GetGlobalString(40002));			///> Invalid Name
		return 1;
	}

	strncpy(kCharInfo.BaseInfo.szName, m_strCharacterName.c_str(), sizeof(TCHAR) * LEN_NAME);

	kFiqureInfo.FIGURE[FIGURE_BASIC_NULL] = 0;
	kFiqureInfo.FIGURE[FIGURE_BASIC_UNDER_SHIRT] = m_vBlouseID[m_iZender][m_iCurLayerIndex[5]];
	kFiqureInfo.FIGURE[FIGURE_BASIC_UNDER_PANTS] = m_vPantsID[m_iZender][m_iCurLayerIndex[6]];
	kFiqureInfo.FIGURE[FIGURE_BASIC_HAIR] = m_vHairID[m_iZender][m_iCurLayerIndex[0]] + m_vHairColorID[m_iZender][m_iCurLayerIndex[1]];
	kFiqureInfo.FIGURE[FIGURE_BASIC_FACE] = m_vExpressionID[m_iZender][m_iCurLayerIndex[2]] + m_vExpressionColorID[m_iZender][m_iCurLayerIndex[3]];
	kFiqureInfo.FIGURE[FIGURE_BASIC_SKIN] = m_vSkinID[m_iZender][m_iCurLayerIndex[4]];

	g_pSubGameManager->SPPerformMessage(SPGM_CHAR_CREATE_REQ, (WPARAM)&kCharInfo, (LPARAM)&kFiqureInfo);
	SetInfomationDialog(true, NOTICE_TYPE_MSG, g_pResourceManager->GetGlobalString(40000));			///> Character Creating...
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLobby::OnCharCreateBack(WPARAM, LPARAM)
{
	g_pSubGameManager->SPPerformMessage(SPGM_CHAR_CREATE_BACK, 0, 0);
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLobby::OnInfomationOk(WPARAM, LPARAM)
{
	SPWindow* pkDialog;

	pkDialog = Find(WIID_LOBBY_DIALOG_INFOMATION, true);
	pkDialog->Hide();
	g_pInterfaceManager->SetModalWindow(NULL);

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLobby::OnKeyReturn(WPARAM, LPARAM)
{
	SPWindow* pkWindow = Find(WIID_LOBBY_DIALOG_JUMIN, true);
	if( pkWindow && pkWindow->IsShow() )
		OnJuminOk(0, 0);

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLobby::OnServerClick(unsigned int uiID, WPARAM, LPARAM)
{
	if( m_vImageProcessor.size() )
		return 1;

	SPWindow* pkWindow;
	m_iSelectServer = uiID - WIID_LOBBY_SERVERLIST_BUTTON + m_iServerSliderTop;

	for( int i = 0; i < LOBBY_SERVER_VIEW_MAX; i++ )
	{
		pkWindow = Find(WIID_LOBBY_SERVERLIST_BUTTON + i, true);
		if( pkWindow == NULL )
			continue;

		//if( uiID == (WIID_LOBBY_SERVERLIST_BUTTON + i) )
		if( i == m_iSelectServer - m_iServerSliderTop )
			pkWindow->SPSendMessage(SPIM_BTN_SELECT, TRUE);
		else
			pkWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE);
	}
	
	ClearChannelList();
	g_pSubGameManager->SPPerformMessage(SPGM_CHANNEL_LIST_REQ, m_iSelectServer, 0);
	
	pkWindow = Find(WIID_LOBBY_CHANNELLIST_SKINTOP, true);
	//if( pkWindow && !pkWindow->IsShow() )
	if( pkWindow )
	{
		pkWindow->Hide() ;
		float fStrPosX, fStrPosY, fDstPosX, fDstPosY;
		RECT rcSrc;
		SPImageProcessor* pkImageProcessor;

		fStrPosX = 474.0f;		fStrPosY = 174.0f;
		fDstPosX = 414.0f;		fDstPosY = 174.0f;

		//SetRect(&rcSrc, 1, 492, 326, 780);
		SetRect(&rcSrc, 1, 387, 1+349, 387+39);		// 일단 타이틀만
		pkImageProcessor = new SPWindowPresentation(WIID_LOBBY_CHANNELLIST_SKINTOP,
			"DATA/LOBBY/RB001.PNG", rcSrc, fStrPosX, fStrPosY, fDstPosX, fDstPosY, 0.3f, true);
		m_vImageProcessor.push_back(pkImageProcessor);

		m_iImageProcessorType = 1;
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLobby::OnChannelClick(unsigned int uiID, WPARAM, LPARAM)
{
	m_iSelectChannel = uiID - WIID_LOBBY_CHANNELLIST_BUTTON;

	for( int i = 0; i < 20; i++ )
	{
		SPWindow* pkWindow = Find(WIID_LOBBY_CHANNELLIST_BUTTON + i, true);
		if( pkWindow == NULL )
			continue;

		if( uiID == (WIID_LOBBY_CHANNELLIST_BUTTON + i) )
			pkWindow->SPSendMessage(SPIM_BTN_SELECT, TRUE);
		else
			pkWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE);
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLobby::OnChannelDBLClick(unsigned int uiID, WPARAM, LPARAM)
{
	OnConnect(0, 0);	
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLobby::OnCardClick(unsigned int uiID, WPARAM, LPARAM)
{
	int iSize = 0;
	if( (iSize = (int)m_vMyCharacterInfo.size()) == 0 )
		return 1;

	int iSelectCard = uiID - WIID_LOBBY_CHAR_CARD;
	if( iSelectCard < m_iCharacterNum )
	{
		if( m_iSelectCard != iSelectCard )
		{
			vector< CHARACTER_INFO2 >::iterator iter;
			if( m_iSelectCard >= 0 && m_iSelectCard < iSize )
			{
				iter = m_vMyCharacterInfo.begin() + m_iSelectCard;
				SetAniSeq(m_iSelectCard, (*iter).uiClassType,
					(*iter).uiSex, true, false);
			}

			if( iSelectCard >= 0 && iSelectCard < iSize )
			{
				iter = m_vMyCharacterInfo.begin() + iSelectCard;
				SetAniSeq(iSelectCard, (*iter).uiClassType,
					(*iter).uiSex, true, true);
			}
		}

		m_iSelectCard = iSelectCard;
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLobby::OnCardDBLClick(unsigned int uiID, WPARAM, LPARAM)
{
	int iSelectCard = uiID - WIID_LOBBY_CHAR_CARD;
	if( iSelectCard < m_iCharacterNum )
		m_iSelectCard = iSelectCard;

	OnCharMenuSelect(0, 0);
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLobby::OnZenderClick(unsigned int uiID, WPARAM, LPARAM)
{
	m_iZender = uiID - WIID_LOBBY_CHAR_CREATE_INFO_ZENDER_M;
	for( int i = 0; i < 2; i++ )
	{
		SPWindow* pkWindow = Find(WIID_LOBBY_CHAR_CREATE_INFO_ZENDER_M + i, true);
		if( pkWindow == NULL )
			continue;

		if( uiID == (WIID_LOBBY_CHAR_CREATE_INFO_ZENDER_M + i) )
			pkWindow->SPSendMessage(SPIM_BTN_SELECT, TRUE);
		else
			pkWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE);
	}

	for( i = 0; i < 7; i++ )
		m_iCurLayerIndex[i] = 0;

	SetCurSelectLayer();
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLobby::OnClassClick(unsigned int uiID, WPARAM, LPARAM)
{
	SPWindow* pkWindow;

	m_iClassType = uiID - WIID_LOBBY_CHAR_CREATE_INFO_ATTACK;
	for( int i = 0; i < 4; i++ )
	{
		pkWindow = Find(WIID_LOBBY_CHAR_CREATE_INFO_ATTACK + i, true);
		if( pkWindow == NULL )
			continue;

		if( uiID == (WIID_LOBBY_CHAR_CREATE_INFO_ATTACK + i) )
			pkWindow->SPSendMessage(SPIM_BTN_SELECT, TRUE);
		else
			pkWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE);
	}

	int iTypeID = 10001 + m_iClassType;
	pkWindow = Find(WIID_LOBBY_CHAR_CREATE_TYPE_INFO, true);
	if( pkWindow )
		pkWindow->SetWindowText(g_pResourceManager->GetGlobalString(iTypeID));

	if( _SHOW_USE_WEAPON_ )
		SetClassWeapon(m_iClassType + 1) ;
	SetStatusGage(m_iClassType + 1);

	///> Charactor
	if( m_pkCreateCharacter ) {
		m_pkCreateCharacter->SetPlayerClass( m_iClassType+1 ) ;
	}

	return 1;
}

int SPWindowLobby::OnClickTest(unsigned int uiID, WPARAM, LPARAM)
{
	int iIndex = uiID - WIID_LOBBY;
	return 0 ;
}
//------------------------------------------------------------------------------------
int SPWindowLobby::OnChangeLeftClick(unsigned int uiID, WPARAM, LPARAM)
{
	int iIndex = uiID - WIID_LOBBY_CHAR_CREATE_CHANGE_LEFT;
	m_iCurLayerIndex[iIndex]--;

	if( m_iCurLayerIndex[iIndex] < 0 )
		m_iCurLayerIndex[iIndex] = 0;

	SetCurSelectLayer();
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLobby::OnChangeRightClick(unsigned int uiID, WPARAM, LPARAM)
{
	int iIndex = uiID - WIID_LOBBY_CHAR_CREATE_CHANGE_RIGHT;
	m_iCurLayerIndex[iIndex]++;

	int iTotalNum = 0;
	switch( iIndex )
	{
	case 0:
		iTotalNum = (int)m_vHairID[m_iZender].size();
		break;
	case 1:
		iTotalNum = (int)m_vHairColorID[m_iZender].size();
		break;
	case 2:
		iTotalNum = (int)m_vExpressionID[m_iZender].size();
		break;
	case 3:
		iTotalNum = (int)m_vExpressionColorID[m_iZender].size();
		break;
	case 4:
		iTotalNum = (int)m_vSkinID[m_iZender].size();
		break;
	case 5:
		iTotalNum = (int)m_vBlouseID[m_iZender].size();
		break;
	case 6:
		iTotalNum = (int)m_vPantsID[m_iZender].size();
		break;
	}

	if( m_iCurLayerIndex[iIndex] >= iTotalNum )
		m_iCurLayerIndex[iIndex] = iTotalNum - 1;

	SetCurSelectLayer();
	return 1;
}

//------------------------------------------------------------------------------------
SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowLobby )

SPIMESSAGE_COMMAND(	SPIM_CURSOR_ENTER,		OnCursorEnter	)
SPIMESSAGE_COMMAND(	SPIM_CURSOR_OUT,		OnCursorOut		)
SPIMESSAGE_COMMAND(	SPIM_KEY_RETURN,		OnKeyReturn		)

SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_LOBBY_CHANNELLIST_CONNECT,		OnConnect		)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_LOBBY_SERVER_BACK,				OnServerBack	)

SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_ENTER,	WIID_LOBBY_CHAR_CREATE_ICON_KNUCKLE,	WIID_LOBBY_CHAR_CREATE_ICON_WAND,	OnWeaponTooltipEnter)
SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_OUT,		WIID_LOBBY_CHAR_CREATE_ICON_KNUCKLE,	WIID_LOBBY_CHAR_CREATE_ICON_WAND,	OnWeaponTooltipOut)
SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_MOVE,	WIID_LOBBY_CHAR_CREATE_ICON_KNUCKLE,	WIID_LOBBY_CHAR_CREATE_ICON_WAND,	OnWeaponTooltipMove)

SPIMESSAGE_COMMAND( SPIM_WHEEL_UP		,	OnSliderUp		)
SPIMESSAGE_COMMAND( SPIM_WHEEL_DOWN		,	OnSliderDown	)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP		,	WIID_LOBBY_SERVERLIST_SLIDER_UP		,	OnSliderUp	)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP		,	WIID_LOBBY_SERVERLIST_SLIDER_DOWN	,	OnSliderDown)
SPIMESSAGE_CONTROL( SPIM_SLIDER_UP		,	WIID_LOBBY_SERVERLIST_SLIDER		,	OnSlider	)
SPIMESSAGE_CONTROL( SPIM_SLIDER_DOWN	,	WIID_LOBBY_SERVERLIST_SLIDER		,	OnSlider	)
SPIMESSAGE_CONTROL( SPIM_SLIDER_PAGEUP	,	WIID_LOBBY_SERVERLIST_SLIDER		,	OnSlider	)
SPIMESSAGE_CONTROL( SPIM_SLIDER_PAGEDOWN,	WIID_LOBBY_SERVERLIST_SLIDER		,	OnSlider	)
SPIMESSAGE_CONTROL( SPIM_SET_CURVALUE	,	WIID_LOBBY_SERVERLIST_SLIDER		,	OnSlider	)

SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_LOBBY_CHAR_MENU_SELECT,		OnCharMenuSelect	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_LOBBY_CHAR_MENU_CREATE,		OnCharMenuCreate	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_LOBBY_CHAR_MENU_DELETE,		OnCharMenuDelete	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_LOBBY_DIALOG_JUMIN_OK,			OnJuminOk	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_LOBBY_DIALOG_JUMIN_CANCEL,		OnJuminCancel	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_LOBBY_CHAR_BACK,				OnCharBack	)

SPIMESSAGE_CONTROL( SPIM_SETFOCUS,			WIID_LOBBY_CHAR_CREATE_INFO_NAME,	OnCharCrateNameFocus	)
//SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_LOBBY_CHAR_CREATE_INFO_NAME_CHECK,	OnCharCreateNameCheck	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_LOBBY_CHAR_CREATE,				OnCharCreateRun	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_LOBBY_CHAR_CREATE_BACK,		OnCharCreateBack	)

SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_LOBBY_DIALOG_INFOMATION_OK,	OnInfomationOk	)

SPIMESSAGE_CONTROL_RANGE( SPIM_WHEEL_UP		,	WIID_LOBBY_SERVERLIST_SKIN	,	WIID_LOBBY_SERVERLIST_STATE + LOBBY_SERVER_VIEW_MAX,	OnWheelSliderUp		)
SPIMESSAGE_CONTROL_RANGE( SPIM_WHEEL_DOWN	,	WIID_LOBBY_SERVERLIST_SKIN	,	WIID_LOBBY_SERVERLIST_STATE + LOBBY_SERVER_VIEW_MAX,	OnWheelSliderDown	)

SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP,	WIID_LOBBY_SERVERLIST_BUTTON,	WIID_LOBBY_SERVERLIST_BUTTON + 8,	OnServerClick)
SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP,	WIID_LOBBY_CHANNELLIST_BUTTON,	WIID_LOBBY_CHANNELLIST_BUTTON + 19,	OnChannelClick)
SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_DBLCLICK,	WIID_LOBBY_CHANNELLIST_BUTTON,	WIID_LOBBY_CHANNELLIST_BUTTON + 19,	OnChannelDBLClick)

SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP,			WIID_LOBBY_CHAR_CARD,	WIID_LOBBY_CHAR_CARD + 2,			OnCardClick)
SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_DBLCLICK,	WIID_LOBBY_CHAR_CARD,	WIID_LOBBY_CHAR_CARD + 2,			OnCardDBLClick)

SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP, WIID_LOBBY_CHAR_CREATE_INFO_ZENDER_M,	WIID_LOBBY_CHAR_CREATE_INFO_ZENDER_W,	OnZenderClick)
SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP, WIID_LOBBY_CHAR_CREATE_INFO_ATTACK,	WIID_LOBBY_CHAR_CREATE_INFO_SCOUT,	OnClassClick)

SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP, WIID_LOBBY_CHAR_CREATE_CHANGE_LEFT,	WIID_LOBBY_CHAR_CREATE_CHANGE_LEFT + 7,		OnChangeLeftClick)
SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP, WIID_LOBBY_CHAR_CREATE_CHANGE_RIGHT,	WIID_LOBBY_CHAR_CREATE_CHANGE_RIGHT + 7,	OnChangeRightClick)

SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP,			WIID_LOBBY_SERVER_TITLE,	WIID_LOBBY_DIALOG_INFOMATION_LIST,			OnClickTest)
	
SPIMESSAGE_OBJ_MAP_END	( SPWindow	)





int SPWindowLobby::OnSliderUp( WPARAM wParam, LPARAM lParam )
{
	if( m_pSlider )
		m_pSlider->SPSendMessage( SPIM_SLIDER_UP );
	return 1;
}

int SPWindowLobby::OnSliderDown( WPARAM wParam, LPARAM lParam )
{
	if( m_pSlider )
		m_pSlider->SPSendMessage( SPIM_SLIDER_DOWN );
	return 1;
}


int SPWindowLobby::OnWheelSliderUp( unsigned int uiID, WPARAM wParam, LPARAM lParam )
{
	if( m_pSlider )
		m_pSlider->SPSendMessage( SPIM_SLIDER_UP );
	return 1;
}

int SPWindowLobby::OnWheelSliderDown( unsigned int uiID, WPARAM wParam, LPARAM lParam )
{
	if( m_pSlider )
		m_pSlider->SPSendMessage( SPIM_SLIDER_DOWN );
	return 1;
}


/**
	OnSlider -> Slider Event
	1) SPWindowLobby::OnSlider
	2) SPLobby::OnWorldListScroll
	3) SPWindowLobby::UpdateServerList
*/
int SPWindowLobby::OnSlider( WPARAM wParam, LPARAM lParam )
{
	g_pSubGameManager->SPPerformMessage(SPGM_SERVER_LIST_SCROLL, wParam, 0);
	return 1;
}


/**
	Call By SPLobby::OnWorldListScroll()
	Receive WorldList and Make ServerList after Scroll iPage
*/
int SPWindowLobby::UpdateServerList( int iPage, vector< WORLD_INFO >& vWorldList )
{
	if( !m_bIsSlide )	return 0 ;

	m_iServerSliderTop = iPage ;
	
	SPWindowButton* pkButton;
	SPWindowStatic* pkStatic;
	char szBuf[32];

	int iServerIconPos = m_rcServerIconBase.top;
	int iServerBtnPos = m_rcServerButtonBase.top;
	int iCharNumPos = m_rcCharNumBase.top;
	int iServerStatePos = m_rcServerStateBase.top;
	int iSrcIconX, iSrcIconY;

	int iLoop = m_iServerListSize - m_iServerSliderTop ;
	iLoop = (iLoop > LOBBY_SERVER_VIEW_MAX) ? LOBBY_SERVER_VIEW_MAX : iLoop ;

	for(int i = 0; i < iLoop; i++)
	{
		int iCurList = i + m_iServerSliderTop ;
		if( (pkStatic = (SPWindowStatic*)Find(WIID_LOBBY_SERVERLIST_ICON + i, true)) != NULL)
		{
			if( vWorldList[iCurList].wid )
			{
				iSrcIconX = ((vWorldList[iCurList].wid - 1) % 8) * m_rcServerIconBase.right;
				iSrcIconY = ((vWorldList[iCurList].wid - 1) / 8) * m_rcServerIconBase.right;
				pkStatic->SPSendMessage(SPIM_SET_IMAGECLEAR);
				pkStatic->SetImage("DATA/LOBBY/SERVERICON001.PNG", iSrcIconX, iSrcIconY);
				pkStatic->Show();
			}
			iServerIconPos += m_rcServerIconBase.bottom + m_iServerIconInterval;

			pkButton = (SPWindowButton*)Find(WIID_LOBBY_SERVERLIST_BUTTON + i, true);
			pkButton->SetFontColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
			pkButton->SetWindowText(vWorldList[iCurList].name);
			pkButton->Show();
			iServerBtnPos += m_rcServerButtonBase.bottom + m_iServerButtonInterval;
			// Select Check
			if( i == m_iSelectServer - m_iServerSliderTop )
				pkButton->SPSendMessage(SPIM_BTN_SELECT, TRUE);
			else
				pkButton->SPSendMessage(SPIM_BTN_SELECT, FALSE);
			

			pkStatic = (SPWindowStatic*)Find(WIID_LOBBY_SERVERLIST_CHARNUM + i, true);
			pkStatic->SetFontColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
			sprintf(szBuf, "%d", vWorldList[iCurList].nCharCount);
			pkStatic->SetWindowText(szBuf);
			pkStatic->Show();
			iCharNumPos += m_rcCharNumBase.bottom + m_iCharNumInterval;

			pkStatic = (SPWindowStatic*)Find(WIID_LOBBY_SERVERLIST_STATE + i, true);
			RECT rcSrc = m_rcSrcState[vWorldList[iCurList].status];
			pkStatic->SPSendMessage(SPIM_SET_IMAGECLEAR);
			pkStatic->SetImage("DATA/LOBBY/RB001.PNG", rcSrc.left, rcSrc.top);
			pkStatic->Show();
			iServerStatePos += m_rcServerStateBase.bottom + m_iServerStateInterval;
		}
	}
	return 1 ;
}









