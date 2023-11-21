#include "SPCommon.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"

#include "SPWindowPlayerInfo.h"

#include "Packet.h"
#include "PacketID.h"
#include "SPNetworkManager.h"

#include "SPGOBManager.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGOBModelUnit.h"
#include "SPAvatarModelUnit.h"
#include "SPGameObjectDEF.h"
#include "SPGameObject.h"
#include "SPGOBStatus.h"
#include "SPPlayerStatus.h"
#include "SPPlayer.h"

#include "SPPlayerInvenArchive.h"
#include "SPGuildArchive.h"

#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"
#include "SPResourceManager.h"

#include "SPCheckManager.h"
#include "SPTitleRenderBase.h"
#include "SPTitleArchive.h"
#include "SPTitleManager.h"

#include "SPItemAttr.h"
#include "SPItem.h"
#include "SPItemCluster.h"

#include "SPWindowSlider.h"
#include "SPWindowFriend.h"
#include "SPWindowFriendFind.h"
#include "SPWindowFriendFollow.h"//[liuyang,2008.10.10]
#include "SPWindowFriendSnatch.h"//[liuyang,2008.10.10]

#include "SPWindowPlayerInfoEquip.h"
#include "SPLocalizeManager.h"

#include "SPPvpManager.h"

#include "SPCashTradeManager.h"

#include "SPLoveManager.h"

#include "SPDebug.h"
#include "SPUtil.h"
#include "SPResourceDef.h"
#include "SPUIManager.h"


//------------------------------------------------------------------------------------
SPWindowPlayerInfo::SPWindowPlayerInfo(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
, m_pkNoUseTitle(NULL)
, m_pkTitle(NULL)
{
	Init();
}

//------------------------------------------------------------------------------------
SPWindowPlayerInfo::SPWindowPlayerInfo(INSTANCE_ID InstanceID,
	int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_INVALID, InstanceID, iX, iY, iCX, iCY, pParent)
, m_pkNoUseTitle(NULL)
, m_pkTitle(NULL)
{
	Init();
}

//------------------------------------------------------------------------------------
SPWindowPlayerInfo::~SPWindowPlayerInfo()
{
	Clean();
}

//------------------------------------------------------------------------------------
void SPWindowPlayerInfo::Clean()
{
	SPWindow::Clean();
	SAFE_DELETE(m_pkNoUseTitle);
	DeletePlayerEquipSlot();
}

//------------------------------------------------------------------------------------
void SPWindowPlayerInfo::Init()
{
	SPWindowStatic* pkSkinWindow, *pkChildWindow, *pkChildWindow2;
	SPWindowButton* pkButton;

	pkSkinWindow = new SPWindowStatic(WIID_PC_INFO_SKIN_UPPER, 0, 0, 242, 4, this);
	pkSkinWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 91);

	pkSkinWindow = new SPWindowStatic(WIID_PC_INFO_SKIN_LOWER, 0, 264, 242, 4, this);
	pkSkinWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 101);

	pkSkinWindow = new SPWindowStatic(WIID_PC_INFO_SKIN_MIDDLE, 0, 4, 242, 260, this);
	pkSkinWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 97);
	pkSkinWindow->SetSrcSize(242, 2);

	pkChildWindow = new SPWindowStatic(WIID_PC_INFO_TITLE_TEXT, 10, 3, 93, 15, pkSkinWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 239, 360);

	pkButton = new SPWindowButton(WIID_PC_INFO_EXIT, 221, 5, 12, 12, pkSkinWindow);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 301, 321);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG", 314, 321);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG", 327, 321);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG", 340, 321);

	pkChildWindow = new SPWindowStatic(WIID_PC_INFO_SUB_BG, 2, 22, 238, 19, pkSkinWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 493, 486);
	pkChildWindow->SetSrcSize(6, 6);

	pkButton = new SPWindowButton(WIID_PC_INFO_TAB_STATUS, 8, 2, 64, 15, pkChildWindow);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 376);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 392);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 408);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 424);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 408);

	pkButton = new SPWindowButton(WIID_PC_INFO_TAB_EQUIP, 71, 2, 64, 15, pkChildWindow);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 66, 376);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG", 66, 392);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG", 66, 408);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG", 66, 424);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/UI103.PNG", 66, 408);

	pkButton = new SPWindowButton(WIID_PC_INFO_TAB_LOVE, 134, 2, 64, 15, pkChildWindow);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/bugle_v2.png.", 362, 263);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/bugle_v2.png.", 362, 279);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/bugle_v2.png.", 362, 295);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/bugle_v2.png.", 362, 311);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/bugle_v2.png.", 362, 295);

	pkChildWindow = new SPWindowStatic(WIID_PC_INFO_SEPARATE_LINEUP, 2, 41, 238, 1, pkSkinWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 493, 494);
	pkChildWindow->SetSrcSize(6, 1);

	pkChildWindow = new SPWindowStatic(WIID_PC_INFO_SEPARATE_LINEDOWN, 2, 253, 238, 1, pkSkinWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 493, 494);
	pkChildWindow->SetSrcSize(6, 1);

	pkChildWindow = new SPWindowStatic(WIID_PC_INFO_SUB_WHITE_BG, 2, 42, 238, 211, pkSkinWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 493, 477);
	pkChildWindow->SetSrcSize(6, 6);

	pkChildWindow2 = new SPWindowStatic(WIID_PC_INFO_STATUS, 0, 0, 238, 211, pkChildWindow);
	InitStatusWindow(pkChildWindow2);
	
	pkChildWindow2 = new SPWindowStatic(WIID_PC_INFO_EQUIP, 0, 0, 238, 211, pkChildWindow);
	InitEquipWindow(pkChildWindow2);

	//LOVE
	bool bLoveEnable  =	SPLocalizeManager::GetInstance()->IsEnableValue( LV_LOVE );
    if ( bLoveEnable )
	{
		pkChildWindow2 = new SPWindowStatic(WIID_OPPSTATUS_LOVE, 0, 0, 238, 211, pkChildWindow);
		InitLoveWindow(pkChildWindow2);
	}

	//
	m_pkNoUseTitle = SPTitleManager::GetInstance()->GetCreteTitleBase(TITLE_ID_NOTUSE, true);

	m_uiPlayerGUID = 0;
	m_pkPlayer = NULL;

	m_bTradeRequest = false;
	m_fTradeDeley = 3.0f;
	m_fTradeAccumulate = 0.0f;
	m_iCategory = -1;
	m_bOnceEquipSend = false;
	CreatePlayerEquipSlot();
}

//------------------------------------------------------------------------------------
void SPWindowPlayerInfo::InitStatusWindow(SPWindow* pkParent)
{
	SPWindowStatic* pkChildWindow;
	SPWindowButton* pkButton;

	pkChildWindow = new SPWindowStatic(WIID_PC_INFO_STATUS_CHAR_CLASS_BG, 8, 23, 25, 8, pkParent);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 484, 496);

	pkChildWindow = new SPWindowStatic(WIID_PC_INFO_STATUS_CHAR_BG, 93, 36, 145, 144, pkParent);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 323, 1);

	pkButton = new SPWindowButton(WIID_PC_INFO_STATUS_TRADE, -87, 98, 68, 17, pkChildWindow);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 440);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 458);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 476);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 494);

	pkButton = new SPWindowButton(WIID_PC_INFO_STATUS_INVITE, -87, 117, 68, 17, pkChildWindow);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 70, 440);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG", 70, 458);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG", 70, 476);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG", 70, 494);

	pkButton = new SPWindowButton(WIID_PC_INFO_STATUS_CLOSED, -87, 136, 68, 17, pkChildWindow);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 139, 440);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG", 139, 458);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG", 139, 476);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG", 139, 494);

	pkButton = new SPWindowButton(WIID_PC_INFO_STATUS_WHISPER, -17, 98, 68, 17, pkChildWindow);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 208, 440);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG", 208, 458);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG", 208, 476);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG", 208, 494);

	pkButton = new SPWindowButton(WIID_PC_INFO_STATUS_FRIEND, -17, 117, 68, 17, pkChildWindow);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 277, 440);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG", 277, 458);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG", 277, 476);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG", 277, 494);

	if( SPLocalizeManager::GetInstance()->IsEnableValue(LV_GUILD) )	{
		pkButton = new SPWindowButton(WIID_PC_INFO_STATUS_GUILD, -17, 117+19, 68, 17, pkChildWindow);
		pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI106.PNG", 171, 135);
		pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI106.PNG", 171, 153);
		pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI106.PNG", 171, 171);
		pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI106.PNG", 171, 189);
	}

	pkChildWindow = new SPWindowStatic(WIID_PC_INFO_STATUS_CHAR_CLASS_IMG, 12, 13, 16, 16, pkParent);

	const char*	pTextureName;
	RECT*		pTextureSrc;

	for( int i = 0 ; i < CLASS_MAX ; ++i )
	{
		pTextureName	=	g_pInterfaceManager->GetClassTextureName( (CLASS_TYPE)i );
		pTextureSrc		=	g_pInterfaceManager->GetClassTextureSrc( (CLASS_TYPE)i );

		if( pTextureName != NULL && pTextureSrc != NULL )
			pkChildWindow->SetImage( pTextureName , pTextureSrc->left , pTextureSrc->top );
	}

	pkChildWindow = new SPWindowStatic(WIID_PC_INFO_STATUS_NAME, 44, 8, 135, 16, pkParent);
	pkChildWindow->SetFontColor(D3DXCOLOR(0.5607f, 0.3882f, 0.3882f, 1.0f));
	pkChildWindow->SetFormat(DT_VCENTER | DT_LEFT);
	pkChildWindow->SetFont(FONT_12_BOLD);
	//pkChildWindow->SetFontShadow(true);

	pkChildWindow = new SPWindowStatic(WIID_PC_INFO_STATUS_CLASS, 49, 27, 135, 12, pkParent);
	pkChildWindow->SetFontColor(D3DXCOLOR(0.5607f, 0.3882f, 0.3882f, 1.0f));
	pkChildWindow->SetFormat(DT_VCENTER | DT_LEFT);

	//pkChildWindow = new SPWindowStatic(WIID_PC_INFO_STATUS_LEVEL, 89, 27, 35, 12, pkParent);
	//pkChildWindow->SetFontColor(D3DXCOLOR(0.5607f, 0.3882f, 0.3882f, 1.0f));
	//pkChildWindow->SetFormat(DT_VCENTER | DT_LEFT);

	pkChildWindow = new SPWindowStatic(WIID_PC_INFO_STATUS_TITLE_BG, 6, 54, 138, 64, pkParent);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 373, 321);

	pkChildWindow = new SPWindowStatic(WIID_PC_INFO_STATUS_TITLE, 33, 67, 108, 12, pkParent);
	pkChildWindow->SetFontColor(D3DXCOLOR(0.5607f, 0.3882f, 0.3882f, 1.0f));

	bool bPvp = SPLocalizeManager::GetInstance()->IsEnableValue( LV_PVP );

	if( bPvp == false )
		pkChildWindow = new SPWindowStatic(WIID_PC_INFO_STATUS_FAME, 63, 87, 48, 12, pkParent);
	else
		pkChildWindow = new SPWindowStatic(WIID_PC_INFO_STATUS_FAME, 56, 87, 78, 12, pkParent);

	pkChildWindow->SetFontColor(D3DXCOLOR(0.5607f, 0.3882f, 0.3882f, 1.0f));

	if( bPvp == false )
		pkChildWindow = new SPWindowStatic(WIID_PC_INFO_STATUS_PVP, 63, 102, 48, 12, pkParent);
	else
		pkChildWindow = new SPWindowStatic(WIID_PC_INFO_STATUS_PVP, 56, 102, 78, 12, pkParent);

	pkChildWindow->SetFontColor(D3DXCOLOR(0.5607f, 0.3882f, 0.3882f, 1.0f));	
}

//------------------------------------------------------------------------------------
void SPWindowPlayerInfo::InitEquipWindow(SPWindow* pkParent)
{
	SPWindowStatic* pkChildWindow, *pkChildWindow2;
	SPWindowButton* pkButton;
	int i, j;

	pkChildWindow = new SPWindowStatic(WIID_PC_INFO_EQUIP_CHAR_BG, 62, 30, 176, 144, pkParent);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 323, 1);

	pkButton = new SPWindowButton(WIID_PC_INFO_EQUIP_BATTLE, 16, 178, 68, 17, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 208, 215);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG", 208, 233);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG", 208, 251);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG", 208, 269);

	pkButton = new SPWindowButton(WIID_PC_INFO_EQUIP_FASHION, 85, 178, 68, 17, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 277, 215);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG", 277, 233);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG", 277, 251);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG", 277, 269);

	POINT ptSrcIcon[4][3];
	ptSrcIcon[0][0].x = 1;		ptSrcIcon[0][0].y = 287;
	ptSrcIcon[0][1].x = 59;		ptSrcIcon[0][1].y = 287;
	ptSrcIcon[0][2].x = 88;		ptSrcIcon[0][2].y = 287;

	ptSrcIcon[1][0].x = 146;	ptSrcIcon[1][0].y = 287;
	ptSrcIcon[1][1].x = 175;	ptSrcIcon[1][1].y = 316;
	ptSrcIcon[1][2].x = 117;	ptSrcIcon[1][2].y = 287;

	ptSrcIcon[2][0].x = 30;		ptSrcIcon[2][0].y = 316;
	ptSrcIcon[2][1].x = 59;		ptSrcIcon[2][1].y = 316;
	ptSrcIcon[2][2].x = 175;	ptSrcIcon[2][2].y = 287;

	ptSrcIcon[3][0].x = 30;		ptSrcIcon[3][0].y = 287;
	ptSrcIcon[3][1].x = 88;		ptSrcIcon[3][1].y = 316;
	ptSrcIcon[3][2].x = 1;		ptSrcIcon[3][2].y = 316;

	for( i = 0; i < 4; i++ )
	{
		for( j = 0; j < 3; j++ )
		{
			pkChildWindow2 = new SPWindowStatic(WIID_PC_INFO_EQUIP_SLOT_IMAGE + j + (i * 3),
				-46 + j * 34, 2 + i * 34, 32, 32, pkChildWindow);
			pkChildWindow2->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 203, 375);

			pkChildWindow2 = new SPWindowStatic(WIID_PC_INFO_EQUIP_SLOT_ICON + j + (i * 3),
				-44 + j * 34, 4 + i * 34, 28, 28, pkChildWindow);
			pkChildWindow2->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", ptSrcIcon[i][j].x, ptSrcIcon[i][j].y);

			pkChildWindow2 = new SPWindowStatic(WIID_PC_INFO_EQUIP_SLOT + j + (i * 3),
				-46 + j * 34, 2 + i * 34, 32, 32, pkChildWindow);
		}
	}

	ptSrcIcon[0][0].x = 117;	ptSrcIcon[0][0].y = 316;
	ptSrcIcon[0][1].x = 146;	ptSrcIcon[0][1].y = 316;
	for( i = 0; i < 2; i++ )
	{
		pkChildWindow2 = new SPWindowStatic(WIID_PC_INFO_EQUIP_WP1_SLOT_IMAGE + i,
			128, -1 + i * 34, 32, 32, pkChildWindow);
		pkChildWindow2->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 203, 375);

		pkChildWindow2 = new SPWindowStatic(WIID_PC_INFO_EQUIP_WP1_SLOT_ICON + i,
			130, 1 + i * 34, 28, 28, pkChildWindow);
		pkChildWindow2->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", ptSrcIcon[0][i].x, ptSrcIcon[0][i].y);

		pkChildWindow2 = new SPWindowStatic(WIID_PC_INFO_EQUIP_WP1_SLOT + i,
			128, -1 + i * 34, 32, 32, pkChildWindow);
	}

	for( i = 0; i < 2; i++ )
	{
		pkChildWindow2 = new SPWindowStatic(WIID_PC_INFO_EQUIP_WP2_SLOT_IMAGE + i,
			128, 75 + i * 34, 32, 32, pkChildWindow);
		pkChildWindow2->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 203, 375);

		pkChildWindow2 = new SPWindowStatic(WIID_PC_INFO_EQUIP_WP2_SLOT_ICON + i,
			130, 77 + i * 34, 28, 28, pkChildWindow);
		pkChildWindow2->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", ptSrcIcon[0][i].x, ptSrcIcon[0][i].y);

		pkChildWindow2 = new SPWindowStatic(WIID_PC_INFO_EQUIP_WP2_SLOT + i,
			128, 75 + i * 34, 32, 32, pkChildWindow);
	}
}

//------------------------------------------------------------------------------------
void SPWindowPlayerInfo::InitLoveWindow(SPWindow* pkParent)
{
	SPWindowStatic* pkChildWindow;

	pkChildWindow = new SPWindowStatic(WIID_OPPSTATUS_LOVE_BG_T, 16, 8, 206, 6, pkParent);
	pkChildWindow->SetWindowType(WND_TYPE_LEFTBASE);
	pkChildWindow->SetImageNormalHead("Data/Interface/Concept/UI_Object_002.png", 420, 306, 426, 312);
	pkChildWindow->SetImageNormalBody("Data/Interface/Concept/UI_Object_002.png", 428, 306, 432, 312);
	pkChildWindow->SetImageNormalTail("Data/Interface/Concept/UI_Object_002.png", 434, 306, 440, 312);	

	pkChildWindow = new SPWindowStatic(WIID_OPPSTATUS_LOVE_BG_ML, 16, 14, 6, 87, pkParent);
	pkChildWindow->SetImage("Data/Interface/Concept/UI_Object_002.png", 420, 314);
	pkChildWindow->SetSrcSize(6, 4);

	pkChildWindow = new SPWindowStatic(WIID_OPPSTATUS_LOVE_BG_MM, 22, 14, 194, 87, pkParent);
	pkChildWindow->SetImage("Data/Interface/Concept/UI_Object_002.png", 428, 314);
	pkChildWindow->SetSrcSize(4, 4);

	pkChildWindow = new SPWindowStatic(WIID_OPPSTATUS_LOVE_BG_MR, 216, 14, 6, 87, pkParent);
	pkChildWindow->SetImage("Data/Interface/Concept/UI_Object_002.png", 434, 314);
	pkChildWindow->SetSrcSize(6, 4);

	pkChildWindow = new SPWindowStatic(WIID_OPPSTATUS_LOVE_BG_B, 16, 101, 206, 6, pkParent);
	pkChildWindow->SetWindowType(WND_TYPE_LEFTBASE);
	pkChildWindow->SetImageNormalHead("Data/Interface/Concept/UI_Object_002.png", 420, 320, 426, 326);
	pkChildWindow->SetImageNormalBody("Data/Interface/Concept/UI_Object_002.png", 428, 320, 432, 326);
	pkChildWindow->SetImageNormalTail("Data/Interface/Concept/UI_Object_002.png", 434, 320, 440, 326);	

	//
	pkChildWindow = new SPWindowStatic(WIID_OPPSTATUS_LOVE_ICON, 21, 13, 22, 16, pkParent);
	pkChildWindow->SetImage("Data/Interface/Concept/UI_Object_002.png", 262, 433);

	pkChildWindow = new SPWindowStatic(WIID_OPPSTATUS_LOVE_TITLE, 45, 15, 92, 13, pkParent);
	pkChildWindow->SetImage("Data/Interface/Concept/UI_Object_002.png", 1, 478);

	//
	pkChildWindow = new SPWindowStatic(WIID_OPPSTATUS_LOVE_LV_BG, 21, 31, 197, 22, pkParent);
	pkChildWindow->SetWindowType(WND_TYPE_LEFTBASE);
	pkChildWindow->SetImageNormalHead("Data/Interface/Concept/UI_Object_002.png", 385, 204, 392, 226);
	pkChildWindow->SetImageNormalBody("Data/Interface/Concept/UI_Object_002.png", 394, 204, 399, 226);
	pkChildWindow->SetImageNormalTail("Data/Interface/Concept/UI_Object_002.png", 401, 204, 408, 226);	

	pkChildWindow = new SPWindowStatic(WIID_OPPSTATUS_LOVE_LVINFO_BG, 86, 34, 126, 15, pkParent);
	pkChildWindow->SetWindowType(WND_TYPE_LEFTBASE);
	pkChildWindow->SetImageNormalHead("Data/Interface/Concept/UI_Object_002.png", 438, 290, 440, 305);
	pkChildWindow->SetImageNormalBody("Data/Interface/Concept/UI_Object_002.png", 441, 290, 443, 305);
	pkChildWindow->SetImageNormalTail("Data/Interface/Concept/UI_Object_002.png", 444, 290, 446, 305);	

	pkChildWindow = new SPWindowStatic(WIID_OPPSTATUS_LOVE_LV_TITLE, 28, 35, 54, 13, pkParent);
	pkChildWindow->SetImage("Data/Interface/Concept/UI_Object_002.png", 457, 387);

	pkChildWindow = new SPWindowStatic(WIID_OPPSTATUS_LOVE_LV_TEXT, 94, 36, 110, 12, pkParent);
	pkChildWindow->SetFontColor(0xff934a52);

	//
	pkChildWindow = new SPWindowStatic(WIID_OPPSTATUS_LOVE_PARTNER_BG, 21, 56, 197, 22, pkParent);
	pkChildWindow->SetWindowType(WND_TYPE_LEFTBASE);
	pkChildWindow->SetImageNormalHead("Data/Interface/Concept/UI_Object_002.png", 385, 204, 392, 226);
	pkChildWindow->SetImageNormalBody("Data/Interface/Concept/UI_Object_002.png", 394, 204, 399, 226);
	pkChildWindow->SetImageNormalTail("Data/Interface/Concept/UI_Object_002.png", 401, 204, 408, 226);	

	pkChildWindow = new SPWindowStatic(WIID_OPPSTATUS_LOVE_PARTNERINFO_BG, 86, 59, 126, 15, pkParent);
	pkChildWindow->SetWindowType(WND_TYPE_LEFTBASE);
	pkChildWindow->SetImageNormalHead("Data/Interface/Concept/UI_Object_002.png", 438, 290, 440, 305);
	pkChildWindow->SetImageNormalBody("Data/Interface/Concept/UI_Object_002.png", 441, 290, 443, 305);
	pkChildWindow->SetImageNormalTail("Data/Interface/Concept/UI_Object_002.png", 444, 290, 446, 305);	

	pkChildWindow = new SPWindowStatic(WIID_OPPSTATUS_LOVE_PARTNER_TITLE, 28, 60, 54, 13, pkParent);
	pkChildWindow->SetImage("Data/Interface/Concept/UI_Object_002.png", 457, 401);

	pkChildWindow = new SPWindowStatic(WIID_OPPSTATUS_LOVE_PARTNER_TEXT, 94, 61, 110, 12, pkParent);
	pkChildWindow->SetFontColor(0xff934a52);

	//
	pkChildWindow = new SPWindowStatic(WIID_OPPSTATUS_LOVE_DAY_BG, 21, 81, 197, 22, pkParent);
	pkChildWindow->SetWindowType(WND_TYPE_LEFTBASE);
	pkChildWindow->SetImageNormalHead("Data/Interface/Concept/UI_Object_002.png", 385, 204, 392, 226);
	pkChildWindow->SetImageNormalBody("Data/Interface/Concept/UI_Object_002.png", 394, 204, 399, 226);
	pkChildWindow->SetImageNormalTail("Data/Interface/Concept/UI_Object_002.png", 401, 204, 408, 226);	

	pkChildWindow = new SPWindowStatic(WIID_OPPSTATUS_LOVE_DAYINFO_BG, 86, 84, 126, 15, pkParent);
	pkChildWindow->SetWindowType(WND_TYPE_LEFTBASE);
	pkChildWindow->SetImageNormalHead("Data/Interface/Concept/UI_Object_002.png", 438, 290, 440, 305);
	pkChildWindow->SetImageNormalBody("Data/Interface/Concept/UI_Object_002.png", 441, 290, 443, 305);
	pkChildWindow->SetImageNormalTail("Data/Interface/Concept/UI_Object_002.png", 444, 290, 446, 305);	

	pkChildWindow = new SPWindowStatic(WIID_OPPSTATUS_LOVE_DAY_TITLE, 28, 85, 54, 13, pkParent);
	pkChildWindow->SetImage("Data/Interface/Concept/UI_Object_002.png", 457, 415);

	pkChildWindow = new SPWindowStatic(WIID_OPPSTATUS_LOVE_DAY_TEXT, 116, 86, 65, 12, pkParent);
	pkChildWindow->SetFontColor(0xff934a52);

}

//------------------------------------------------------------------------------------
void SPWindowPlayerInfo::CreatePlayerEquipSlot()
{
	CONTAINER_TYPE iType = FIGURE_EQUIP;
	for(int i = 0; i < MAX_PLAYER_EQUIP_CONTAINER; i++)
	{
		for(int j = 0; j < DEFAULT_EQUIP_SLOT; j++)
		{
			SPItem* pItem = new SPItem( ((CONTAINER_TYPE)(iType + i)), j);
			m_vpPlayerEquip[i].push_back(pItem);
		}
	}
}

//------------------------------------------------------------------------------------
void SPWindowPlayerInfo::DeletePlayerEquipSlot()
{
	std::vector<SPItem*>::iterator iter;
	for(int i = 0; i < MAX_PLAYER_EQUIP_CONTAINER; i++)
	{
		for(iter = m_vpPlayerEquip[i].begin(); iter != m_vpPlayerEquip[i].end(); ++iter)
		{
			SAFE_DELETE((*iter));
		}
		m_vpPlayerEquip[i].clear();
	}
}

//------------------------------------------------------------------------------------
void SPWindowPlayerInfo::ClearPlayerEquipSlot()
{
	std::vector<SPItem*>::iterator iter;

	for(int i = 0; i < MAX_PLAYER_EQUIP_CONTAINER; i++)
	{
		for(iter = m_vpPlayerEquip[i].begin(); iter != m_vpPlayerEquip[i].end(); ++iter)
		{
			(*iter)->SetClear();
		}
	}

	ClearEquipWindow();
}

//------------------------------------------------------------------------------------
void SPWindowPlayerInfo::ClearEquipWindow(bool bWeaponClean)
{
	int i;
	SPWindow* pkWindow;
	
	for( i = 0; i < 12; i++ )
	{
		pkWindow = Find(WIID_PC_INFO_EQUIP_SLOT_IMAGE + i, true);
		if( pkWindow )
			pkWindow->SetColor(RGBA(255, 255, 255, 128));

		pkWindow = Find(WIID_PC_INFO_EQUIP_SLOT_ICON + i, true);
		if( pkWindow )
			pkWindow->Show();

		pkWindow = Find(WIID_PC_INFO_EQUIP_SLOT + i, true);
		if( pkWindow )
		{
			pkWindow->SPSendMessage(SPIM_SET_IMAGECLEAR);
			pkWindow->SetColor(RGBA(255, 255, 255, 255));
		}
	}

	if( bWeaponClean == false )
		return;
	
	for( i = 0; i < 2; i++ )
	{
		pkWindow = Find(WIID_PC_INFO_EQUIP_WP1_SLOT_IMAGE + i, true);
		if( pkWindow )
			pkWindow->SetColor(RGBA(255, 255, 255, 128));

		pkWindow = Find(WIID_PC_INFO_EQUIP_WP1_SLOT_ICON + i, true);
		if( pkWindow )
			pkWindow->Show();

		pkWindow = Find(WIID_PC_INFO_EQUIP_WP1_SLOT + i, true);
		if( pkWindow )
			pkWindow->SPSendMessage(SPIM_SET_IMAGECLEAR);
	}

	for( i = 0; i < 2; i++ )
	{
		pkWindow = Find(WIID_PC_INFO_EQUIP_WP2_SLOT_IMAGE + i, true);
		if( pkWindow )
			pkWindow->SetColor(RGBA(255, 255, 255, 128));

		pkWindow = Find(WIID_PC_INFO_EQUIP_WP2_SLOT_ICON + i, true);
		if( pkWindow )
			pkWindow->Show();

		pkWindow = Find(WIID_PC_INFO_EQUIP_WP2_SLOT + i, true);
		if( pkWindow )
			pkWindow->SPSendMessage(SPIM_SET_IMAGECLEAR);
	}
}

//------------------------------------------------------------------------------------
void SPWindowPlayerInfo::SetCategory(int iType)
{
	if( m_iCategory == iType )
		return;

	SPWindow* pkStatusWindow, *pkEquipWindow;
	SPWindow* pkTabStatus, *pkTabEquip, *pkTabLove;
	
	pkStatusWindow = Find(WIID_PC_INFO_STATUS, true);
	pkEquipWindow = Find(WIID_PC_INFO_EQUIP, true);
	pkTabStatus = Find(WIID_PC_INFO_TAB_STATUS, true);
	pkTabEquip = Find(WIID_PC_INFO_TAB_EQUIP, true);
	pkTabLove = Find(WIID_PC_INFO_TAB_LOVE, true);

	if( pkStatusWindow == NULL || pkEquipWindow == NULL ||
		pkTabStatus == NULL || pkTabEquip == NULL || pkTabLove == NULL )
		return;

	if( iType == 1 )
	{
		HideInfo_Love();
		pkStatusWindow->Show();
		pkEquipWindow->Hide();

		pkTabStatus->SPSendMessage(SPIM_BTN_SELECT, 1, 0);
		pkTabEquip->SPSendMessage(SPIM_BTN_SELECT, 0, 0);
		pkTabLove->SPSendMessage(SPIM_BTN_SELECT, 0, 0);

		SetStatusInfo();
	}
	else if( iType == 2 )
	{
		HideInfo_Love();
		pkStatusWindow->Hide();
		pkEquipWindow->Show();

		pkTabStatus->SPSendMessage(SPIM_BTN_SELECT, 0, 0);
		pkTabLove->SPSendMessage(SPIM_BTN_SELECT, 0, 0);
		pkTabEquip->SPSendMessage(SPIM_BTN_SELECT, 1, 0);

		if( !m_bOnceEquipSend )
		{
			SendPcEquipItem();
			m_bOnceEquipSend = true;
		}
		else
		{
			ClearEquipWindow(false);
			SetEquipInfo();
		}
	}
	else if( iType == 3 )
	{
		ShowInfo_Love();
		pkStatusWindow->Hide();
		pkEquipWindow->Hide();

		pkTabStatus->SPSendMessage(SPIM_BTN_SELECT, 0, 0);
		pkTabEquip->SPSendMessage(SPIM_BTN_SELECT, 0, 0);
		pkTabLove->SPSendMessage(SPIM_BTN_SELECT, 1, 0);
	}

	m_iCategory = iType;
}

//------------------------------------------------------------------------------------
void SPWindowPlayerInfo::SetTradeButton(bool bEnable)
{
	SPWindowButton* pkButton;
	pkButton = static_cast< SPWindowButton* >(Find(WIID_PC_INFO_STATUS_TRADE, true));
	if( pkButton == NULL )
		return;

	pkButton->SetEnable(bEnable);
}

//------------------------------------------------------------------------------------
bool SPWindowPlayerInfo::IsTradeButtonEnable()
{
	SPWindowButton* pkButton;
	pkButton = static_cast< SPWindowButton* >(Find(WIID_PC_INFO_STATUS_TRADE, true));
	if( pkButton == NULL )
		return false;

	return pkButton->IsEnable();
}

//------------------------------------------------------------------------------------
void SPWindowPlayerInfo::SetStatusInfo()
{
	if( m_pkPlayer == NULL )
		return;

	SPGOBStatus* pkStatus = m_pkPlayer->GetStatus();
	if( pkStatus == NULL )
		return;
	
	SPWindow* pkWindow;
	string strStringBuf;
	char szBuf[128];

	pkWindow = Find(WIID_PC_INFO_STATUS_NAME, true);
	if( pkWindow )
		pkWindow->SetWindowText(pkStatus->GetGOBName());

	unsigned int uiClassType = pkStatus->GetStatusValue(STATUS_CLASS_TYPE);
	pkWindow = Find(WIID_PC_INFO_STATUS_CHAR_CLASS_IMG, true);
	if( pkWindow )
		pkWindow->SPSendMessage(SPIM_SET_IMAGEINDEX, (WPARAM)uiClassType);

	pkWindow = Find(WIID_PC_INFO_STATUS_CLASS, true);
	if( pkWindow )
	{
		strStringBuf = g_pResourceManager->GetGlobalString(uiClassType + 20000);
		strStringBuf += " Lv.";
		strStringBuf += itoa(pkStatus->GetStatusValue(STATUS_LV), szBuf, 10);
		pkWindow->SetWindowText(strStringBuf.c_str());
	}

	//pkWindow = Find(WIID_PC_INFO_STATUS_LEVEL, true);
	//if( pkWindow )
	//{
	//	strStringBuf = "Lv.";
	//	strStringBuf += itoa(pkStatus->GetStatusValue(STATUS_LV), szBuf, 10);
	//	pkWindow->SetWindowText(strStringBuf.c_str());
	//}

	pkWindow = Find(WIID_PC_INFO_STATUS_FAME, true);
	if( pkWindow )
		pkWindow->SetWindowText(itoa(pkStatus->GetStatusValue(STATUS_FAME), szBuf, 10));

	if( SPLocalizeManager::GetInstance()->IsEnableValue( LV_PVP ) == true )
	{
		pkWindow = Find(WIID_PC_INFO_STATUS_PVP, true);
		if( pkWindow )
		{
			std::string	strPVPLv	=	SPPvpManager::GetInstance()->GetPvpGloryLevelString( pkStatus->GetStatusValue( STATUS_PVP_LV ) );
			pkWindow->SetWindowText( strPVPLv.c_str() );
		}
	}

	m_pkTitle = m_pkPlayer->GetTitle();
	if( m_pkTitle == NULL )
		m_pkTitle = m_pkNoUseTitle;
}

//------------------------------------------------------------------------------------
void SPWindowPlayerInfo::SetEquipInfo()
{
	int iContainerType = m_iEquipType - FIGURE_EQUIP;
	vector<SPItem*>::iterator iter = m_vpPlayerEquip[iContainerType].begin();
	while( iter != m_vpPlayerEquip[iContainerType].end() )
	{
		CONTAINER_ITEM item;
		item.Item.iItemID = (*iter)->GetItemStatusValue(MV_ITEM_GETUID);
		if( item.Item.iItemID !=0 )
		{
			(*iter)->ConvertContainerItem((CONTAINER_ITEM*)&item);
			SetEquipWindow(&item);
		}
			
		++iter;
	}	
}

//------------------------------------------------------------------------------------
void SPWindowPlayerInfo::ItemSetting(CONTAINER_ITEM item)
{
	int iContainerIndex = (int)item.SlotInfo.ContainerType - (int)FIGURE_EQUIP;
	m_vpPlayerEquip[iContainerIndex].at(item.SlotInfo.SlotIndex)->SetItemStatus(&item);
	m_vpPlayerEquip[iContainerIndex].at(item.SlotInfo.SlotIndex)->SetItem(item.SlotInfo.ContainerType, item.SlotInfo.SlotIndex);
}

//------------------------------------------------------------------------------------
void SPWindowPlayerInfo::SetEquipWindow(CONTAINER_ITEM* pItem)
{
	CONTAINER_TYPE ContainerType = pItem->SlotInfo.ContainerType;
	int iSlotIndex = pItem->SlotInfo.SlotIndex;

	if( ContainerType <  FIGURE_EQUIP || ContainerType >= STORAGE )
		return;

	POINT ptSrcPos;	
	SPTexture* pTexture = g_pItemCluster->GetItemTexture(pItem->Item.iItemID, ptSrcPos);

	if( ContainerType != m_iEquipType )
		return;
	
	int i;
	SPWindow* pkWindow = NULL;
	SPWindow* pkSlotImageWindow = NULL, *pkSlotIconImage = NULL;
	
	if( ContainerType == FIGURE_EQUIP )
	{
		if( iSlotIndex == EQ_WEAPON1 - 1 || iSlotIndex == EQ_BUCKLER1 - 1 )
		{
			pkWindow = Find(WIID_PC_INFO_EQUIP_WP1_SLOT + (iSlotIndex - EQ_WEAPON1 + 1), true);
			pkSlotImageWindow = Find(WIID_PC_INFO_EQUIP_WP1_SLOT_IMAGE + (iSlotIndex - EQ_WEAPON1 + 1), true);
			pkSlotIconImage = Find(WIID_PC_INFO_EQUIP_WP1_SLOT_ICON + (iSlotIndex - EQ_WEAPON1 + 1), true);
		}
		else if( iSlotIndex == EQ_WEAPON2 - 1 || iSlotIndex == EQ_BUCKLER2 - 1 )
		{
			pkWindow = Find(WIID_PC_INFO_EQUIP_WP2_SLOT + (iSlotIndex - EQ_WEAPON2 + 1), true);
			pkSlotImageWindow = Find(WIID_PC_INFO_EQUIP_WP2_SLOT_IMAGE + (iSlotIndex - EQ_WEAPON2 + 1), true);
			pkSlotIconImage = Find(WIID_PC_INFO_EQUIP_WP2_SLOT_ICON + (iSlotIndex - EQ_WEAPON2 + 1), true);
		}
		else
		{
			pkWindow = Find(WIID_PC_INFO_EQUIP_SLOT + iSlotIndex, true);
			pkSlotImageWindow = Find(WIID_PC_INFO_EQUIP_SLOT_IMAGE + iSlotIndex, true);
			pkSlotIconImage = Find(WIID_PC_INFO_EQUIP_SLOT_ICON + iSlotIndex, true);
		}

		if( pkWindow )
		{
			pkWindow->SPSendMessage(SPIM_SET_IMAGECLEAR);
			pkWindow->SetImage(pTexture, ptSrcPos.x, ptSrcPos.y);
		}

		if( pkSlotImageWindow )
			pkSlotImageWindow->SetColor(RGBA(255, 255, 255, 255));
		if( pkSlotIconImage )
			pkSlotIconImage->Hide();
	}
	else if( ContainerType == FIGURE_FASHION )
	{
		if( pItem->SlotInfo.SlotIndex == 3 )
		{
			if( (pItem->Item.iItemID) / 10000000 == 5 )
			{
				for( i = 0; i < 2; i++ )
				{
					pItem->SlotInfo.SlotIndex++;
					pkWindow = Find(WIID_PC_INFO_EQUIP_SLOT + pItem->SlotInfo.SlotIndex, true);
					pkSlotImageWindow = Find(WIID_PC_INFO_EQUIP_SLOT_IMAGE + pItem->SlotInfo.SlotIndex, true);
					pkSlotIconImage = Find(WIID_PC_INFO_EQUIP_SLOT_ICON + pItem->SlotInfo.SlotIndex, true);

					if( pkWindow )
					{
						pkWindow->SPSendMessage(SPIM_SET_IMAGECLEAR);
						pkWindow->SetImage(pTexture, ptSrcPos.x, ptSrcPos.y);
						pkWindow->SetColor(RGBA(255, 255, 255, 128));
					}

					if( pkSlotImageWindow )
						pkSlotImageWindow->SetColor(RGBA(255, 255, 255, 255));
					if( pkSlotIconImage )
						pkSlotIconImage->Hide();
				}
			}
		}

		pkWindow = Find(WIID_PC_INFO_EQUIP_SLOT + iSlotIndex, true);
		pkSlotImageWindow = Find(WIID_PC_INFO_EQUIP_SLOT_IMAGE + iSlotIndex, true);
		pkSlotIconImage = Find(WIID_PC_INFO_EQUIP_SLOT_ICON + iSlotIndex, true);

		if( pkWindow )
		{
			pkWindow->SPSendMessage(SPIM_SET_IMAGECLEAR);
			pkWindow->SetImage(pTexture, ptSrcPos.x, ptSrcPos.y);
		}

		if( pkSlotImageWindow )
			pkSlotImageWindow->SetColor(RGBA(255, 255, 255, 255));
		if( pkSlotIconImage )
			pkSlotIconImage->Hide();
	}
	else if (ContainerType == FIGURE_EFFECT)
	{
		// TODO: 꿴였景槻
	}
}

//------------------------------------------------------------------------------------
void SPWindowPlayerInfo::Show(bool bWithChild)
{
	SPWindow::Show(bWithChild);
	m_bTradeRequest = false;	
	m_fTradeAccumulate = 0.0f;

	m_iCategory = -1;
	m_iEquipType = FIGURE_EQUIP;
	m_bOnceEquipSend = false;
	ClearPlayerEquipSlot();
	SetCategory(1);
}

//------------------------------------------------------------------------------------
void SPWindowPlayerInfo::Hide(bool bSendServer)
{	
	if(m_bShow)
		g_pInterfaceManager->RegisterSoundUnit("DATA/SOUND/SYSTEM_CLOSE_WINDOW.WAV");

	SPWindow::Hide(bSendServer);
}

//------------------------------------------------------------------------------------
void SPWindowPlayerInfo::Process(float fTime)
{
	if( !m_bShow )
		return;	

	SPWindow::Process(fTime);

	m_pkPlayer = (SPPlayer*)g_pGOBManager->Find(m_uiPlayerGUID);
	if( m_pkPlayer == NULL )
	{
		OnExit(0, 0);
		m_szBuf[0] = '\0';
	}

	//
	if(CheckInterval()) {
		if(IsTradeButtonEnable() == false)
			if(!m_bTradeRequest)
				SetTradeButton(true);
	}
	else {
		if(IsTradeButtonEnable() == true)
			if(!m_bTradeRequest)
				SetTradeButton(false);
	}

	//[2006/3/13] 트레이드 버튼 클릭후 일정 시간동안 블럭된 버튼을 풀어준다
	if(m_bTradeRequest) {
		m_fTradeAccumulate += fTime;
		if( m_fTradeAccumulate > m_fTradeDeley) {
			m_fTradeAccumulate -= m_fTradeDeley;
			SetTradeButton(true);
			//m_pTrade->Show();
			m_bTradeRequest = false;
		}
	}
}


//------------------------------------------------------------------------------------
void SPWindowPlayerInfo::ShowInfo_Love()
{
	if( SPLocalizeManager::GetInstance()->IsEnableValue( LV_LOVE ) == false )
		return;

	SPWindow * spwnd_Specified = Find(WIID_OPPSTATUS_LOVE,true);
	if ( spwnd_Specified )
		spwnd_Specified->Show(true);
}

//------------------------------------------------------------------------------------
void SPWindowPlayerInfo::HideInfo_Love()
{
	SPWindow * spwnd_Specified = Find(WIID_OPPSTATUS_LOVE,true);
	if ( spwnd_Specified )
		spwnd_Specified->Hide();
}

//------------------------------------------------------------------------------------
void SPWindowPlayerInfo::SetLoveCharInfo( int iLevel , int iDay , const char* pszCharName )
{
	SPWindow*	pWindow;
	char		szBufTemp[ _MAX_PATH ];

	pWindow	=	Find( WIID_OPPSTATUS_LOVE_LV_TEXT , true );

	if( pWindow != NULL )
	{
		std::string		strLoveState	=	"";
		SPLoveManager*	pLoveManager	=	SPLoveManager::GetInstance();

		if( pLoveManager != NULL )
		{
			LOVE_LEVEL_DATA*	pLoveLevelData	=	pLoveManager->GetLevelData( iLevel );

			if( pLoveLevelData != NULL )
				strLoveState	=	pLoveLevelData->m_strTitle;
		}

		pWindow->SetWindowText( strLoveState.c_str() );
	}

	pWindow	=	Find( WIID_OPPSTATUS_LOVE_DAY_TEXT , true );

	if( pWindow != NULL )
	{
		sprintf( szBufTemp , "%d" , iDay );
		pWindow->SetWindowText( szBufTemp );
	}

	pWindow	=	Find( WIID_OPPSTATUS_LOVE_PARTNER_TEXT , true );

	if( pWindow != NULL )
		pWindow->SetWindowText( pszCharName );
}


//------------------------------------------------------------------------------------
bool SPWindowPlayerInfo::CheckInterval()
{
	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return 0;

	m_pkPlayer = (SPPlayer*)g_pGOBManager->Find(m_uiPlayerGUID);
	if( m_pkPlayer == NULL )
	{
		OnExit(0, 0);
		m_szBuf[0] = '\0';
		return false;
	}

	//같은 맵에 존재하는지 ?
	if(g_pGOBManager->GetLocalPlayer()->GetCurStageID() != m_pkPlayer->GetCurStageID() ||
		g_pGOBManager->GetLocalPlayer()->GetCurMapGroupID() != m_pkPlayer->GetCurMapGroupID())
	{
		return false;
	}

	//트레이드가 가능한 거리에 있는지?
	if(g_pGOBManager->GetLocalPlayer()) {
		float fLocalX = g_pGOBManager->GetLocalPlayer()->GetPosX();
		float fLocalY = g_pGOBManager->GetLocalPlayer()->GetPosY();
		float fPcX = m_pkPlayer->GetPosX();
		float fPcY = m_pkPlayer->GetPosY();

		if((fPcX > fLocalX - (CHECK_INTERVAL_X - 20) && fPcX < fLocalX + (CHECK_INTERVAL_X - 20)) && 
			(fPcY > fLocalY - (CHECK_INTERVAL_Y - 20) && fPcY < fLocalY + (CHECK_INTERVAL_Y - 20)))
		{
			return true;
		}
	}

	return false;
}

//------------------------------------------------------------------------------------
bool SPWindowPlayerInfo::SendTradeRequest()
{
	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return false;	

	m_pkPlayer = (SPPlayer*)g_pGOBManager->Find(m_uiPlayerGUID);
	if( m_pkPlayer == NULL )
	{
		OnExit(0, 0);
		m_szBuf[0] = '\0';
		return false;
	}
	
	//[2007/2/26] - 상대방이 옵션 거래 거부중
	SPPlayerStatus* pPlayerStatus = (SPPlayerStatus*)m_pkPlayer->GetStatus();
	if(pPlayerStatus && pPlayerStatus->IsTrade() == false) {
		char szMsg[256];
		ZeroMemory(szMsg, 256);
		if(g_pResourceManager->GetGlobalString(5017003)) {
			sprintf(szMsg, g_pResourceManager->GetGlobalString(5017003), pPlayerStatus->GetGOBName());
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMsg);
		}
		
		return false;
	}

	if(g_pCheckManager->IsDBBlock()) {
		DXUTOutputDebugString("\tSPWindowPlayerInfo::SendTradeRequest DB Working Block\n");
		return false;
	}

	//[2006/3/6] 이미 트레이드 요청 패킷을 보낸경우
	if(g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->IsSendTradeRequest())
		return false;

	CPacket Packet(TRADE_CS_REQUEST);
	Packet.Add((UINT64)m_uiPlayerGUID);	

	// for debugging
	//{
	static unsigned int iCount = 0;

	++iCount;
	Packet.Add((UINT32)iCount);
	//}

	g_pNetworkManager->SPPerformMessage(TRADE_CS_REQUEST, 0, (LPARAM)&Packet);	

	//[2006/3/6] 트레이드 요청 패킷을 보낸경우 플래그로 응답이 있기전까지 요청 패킷을 막는다
	// 플레그 해제는 인벤아카이브의 트레이드 OnTradeRequest에서 한다 
	g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->SetSendTradeRequest(true);	

	return true;
}


//------------------------------------------------------------------------------------
bool SPWindowPlayerInfo::SendPcEquipItem()
{
	m_pkPlayer = (SPPlayer*)g_pGOBManager->Find(m_uiPlayerGUID);
	if( m_pkPlayer == NULL )
	{
		OnExit(0, 0);
		m_szBuf[0] = '\0';
		return false;
	}

	// GU_ID			iGlobalID				// Target유저의 GU_ID 
	// INT8				iNumOfCONTAINER_TYPE	// 보고싶은 컨테이너번호의 갯수.						//
	// CONTAINER_TYPE	iContainerType[] 		// 컨테이너 타입.

	CPacket Packet(ITEM_CS_FIGURE_INFOS_FOR_SINGLE);
	Packet.Add((UINT64)m_uiPlayerGUID);	
	Packet.Add((UINT8)2);
	Packet.Add((UINT32)FIGURE_EQUIP);
	Packet.Add((UINT32)FIGURE_FASHION);
	g_pNetworkManager->SPPerformMessage(ITEM_CS_FIGURE_INFOS_FOR_SINGLE, 0, (LPARAM)&Packet);	

	return true;
}


//------------------------------------------------------------------------------------
void SPWindowPlayerInfo::Render(float fTime)
{
	if( !m_bShow )
		return;

	SPWindow::Render(fTime);
	if( m_pkPlayer && (m_iCategory == 1) )
	{
		int	iXPos = 0, iYPos = 0;
		GetAbsCoord(iXPos, iYPos);
		SPAvatarModelUnit* pkModelUnit = (SPAvatarModelUnit*)m_pkPlayer->GetModelUnit();
		if( pkModelUnit ) {
			pkModelUnit->ForceRender(iXPos + 186, iYPos + 210, FACING_LEFT, m_pkPlayer->GetWeaponItemSeq(), m_pkPlayer->GetPlayerClass());
		}
	}

	if( m_pkTitle != NULL && (m_iCategory == 1) )
	{
		SPWindow* pkWindow = Find( WIID_PC_INFO_STATUS_TITLE , true);
		int	iXPos = 0, iYPos = 0;

		if( pkWindow )
			pkWindow->GetAbsCoord(iXPos, iYPos);

		m_pkTitle->SetPos(iXPos, iYPos);
		m_pkTitle->Process(fTime);
		m_pkTitle->Render(fTime);
	}
}

//------------------------------------------------------------------------------------
// Message
int SPWindowPlayerInfo::OnSetPlayerID(WPARAM wParam, LPARAM lParam)
{
	GU_ID* pID = (GU_ID*)wParam;
	m_uiPlayerGUID = *pID;

	m_pkPlayer = (SPPlayer*)g_pGOBManager->Find(m_uiPlayerGUID);
	m_pkPlayer->SPGOBPerformMessage(SPGM_GETGOBNAME, (LPARAM)m_szBuf);

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowPlayerInfo::OnExit(WPARAM wParam, LPARAM lParam)
{
	//Hide();
	g_pInterfaceManager->ShowWindow(m_InstanceID, false);
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowPlayerInfo::OnTabStatus(WPARAM wParam, LPARAM lParam)
{
	SetCategory(1);
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowPlayerInfo::OnTabEquip(WPARAM wParam, LPARAM lParam)
{
	SetCategory(2);
	return 1;
}

int SPWindowPlayerInfo::OnTabLove(WPARAM wParam, LPARAM lParam)
{
	SetCategory(3);
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowPlayerInfo::OnEquipBattle(WPARAM wParam, LPARAM lParam)
{
	if( m_iEquipType == FIGURE_EQUIP )
		return 1;
	
	m_iEquipType = FIGURE_EQUIP;
	ClearEquipWindow(true);
	SetEquipInfo();
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowPlayerInfo::OnEquipFashion(WPARAM wParam, LPARAM lParam)
{
	if( m_iEquipType == FIGURE_FASHION )
		return 1;

	m_iEquipType = FIGURE_FASHION;
	ClearEquipWindow(false);
	SetEquipInfo();
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowPlayerInfo::OnTrade(WPARAM wParam, LPARAM lParam)
{
	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return 0;

	// 트레이드 요청 패킷을 보내는 역할을 한다 
	if(CheckInterval()) {
		if( g_pGOBManager->GetLocalPlayer()->GetTradeState() == TRADE_STATE_NULL || 
			g_pGOBManager->GetLocalPlayer()->GetTradeState() == TRADE_STATE_RESPONSE)

			//내가 이미 요청을 받은 상태라면 이전 요청은 캔슬시키고 거래를 보낸다
			if(g_pGOBManager->GetLocalPlayer()->GetTradeState() == TRADE_STATE_RESPONSE) {
				g_pInterfaceManager->ForceReply();
			}

			SendTradeRequest();

			//[2006/3/13] 트레이드 클릭후 일정시간동안 버튼을 불럭한다 
			SetTradeButton(false);
			m_bTradeRequest = true;
			m_fTradeAccumulate = 0.0f;

			//// 임시!! 로 UI열기 작업을 한다	
			//g_pInterfaceManager->GetInvenWindow()->Show();
			//g_pInterfaceManager->GetInvenWindow()->SPSendMessage(SPIM_SET_EXIT_SHOW, 0, (LPARAM)false);	
			//g_pInterfaceManager->GetEquipWindow()->Show();
			//g_pInterfaceManager->GetEquipWindow()->SPSendMessage(SPIM_SET_EXIT_SHOW, 0, (LPARAM)false);

			//g_pInterfaceManager->GetPcTradeWindow()->SPSendMessage(SPIM_SET_PLAYER_INFO, (WPARAM)&m_uiPlayerGUID);
			//g_pInterfaceManager->GetPcTradeWindow()->Show();
			//this->Hide();
	}
	else {
		//m_pTrade->SetEnable(false);
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowPlayerInfo::OnWhisper(WPARAM wParam, LPARAM lParam)
{
	m_pkPlayer = (SPPlayer*)g_pGOBManager->Find( m_uiPlayerGUID );
	m_pkPlayer->SPGOBPerformMessage(SPGM_GETGOBNAME, (LPARAM)m_szBuf );

	return g_pInterfaceManager->SPChildWndSendMessage( WIID_CHAT , SPIM_WNDCHAT_SET_WHISPER , (WPARAM)m_szBuf , lParam );
}

//------------------------------------------------------------------------------------
int SPWindowPlayerInfo::OnInvite(WPARAM wParam, LPARAM lParam)
{
	SPPlayer* pkLocalPlayer = g_pGOBManager->GetLocalPlayer();
	if( pkLocalPlayer == NULL )
		return 1;

	m_pkPlayer = (SPPlayer*)g_pGOBManager->Find(m_uiPlayerGUID);
	if( m_pkPlayer == NULL )
	{
		OnExit(0, 0);
		m_szBuf[0] = '\0';
		return 1;
	}
	
	//[2007/2/26] - 상대방이 옵션 파티초대 거부중
	SPPlayerStatus* pPlayerStatus = (SPPlayerStatus*)m_pkPlayer->GetStatus();
	if(pPlayerStatus && pPlayerStatus->IsTeam() == false) {
		char szMsg[256];
		ZeroMemory(szMsg, 256);
		if(g_pResourceManager->GetGlobalString(5017004)) {
			sprintf(szMsg, g_pResourceManager->GetGlobalString(5017004), pPlayerStatus->GetGOBName());
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMsg);
		}

		return 1;
	}

	if( m_pkPlayer )
	{
		if( strlen(m_szBuf) )
			pkLocalPlayer->SPGOBPerformMessage(SPGM_TEAM_INVITE_REQ, (LPARAM)m_szBuf);
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowPlayerInfo::OnFriend(WPARAM wParam, LPARAM lParam)
{
	m_pkPlayer = (SPPlayer*)g_pGOBManager->Find( m_uiPlayerGUID );
	m_pkPlayer->SPGOBPerformMessage(SPGM_GETGOBNAME, (LPARAM)m_szBuf );

	return g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND , SPIM_USER_ADD_FRIEND , (WPARAM)FRIEND_TYPE_WHITE , (LPARAM)m_szBuf );
}

//------------------------------------------------------------------------------------
int SPWindowPlayerInfo::OnInviteGuild(WPARAM wParam, LPARAM lParam)
{
	m_pkPlayer = (SPPlayer*)g_pGOBManager->Find( m_uiPlayerGUID );
	m_pkPlayer->SPGOBPerformMessage(SPGM_GETGOBNAME, (LPARAM)m_szBuf );

	SPGuildArchive*	pkGuildArchive	=	g_pGOBManager->GetGuildArchive();

	if( pkGuildArchive != NULL )
		pkGuildArchive->SendGuildInvite( m_szBuf );
	return 1 ;
}

//------------------------------------------------------------------------------------
int SPWindowPlayerInfo::OnClosed(WPARAM wParam, LPARAM lParam)
{
	m_pkPlayer = (SPPlayer*)g_pGOBManager->Find( m_uiPlayerGUID );
	m_pkPlayer->SPGOBPerformMessage(SPGM_GETGOBNAME, (LPARAM)m_szBuf );

	return g_pInterfaceManager->SPChildWndSendMessage( WIID_FRIEND , SPIM_USER_ADD_FRIEND , (WPARAM)FRIEND_TYPE_BLACK , (LPARAM)m_szBuf );
}

//------------------------------------------------------------------------------------
int SPWindowPlayerInfo::OnTradeRequest(WPARAM wParam, LPARAM lParam)
{
	std::string	strCharName	=	(const char*)wParam;

	if( strCharName.empty() == true )
		return 0;

	// If PVP Zone then Block
	if( g_pkPvpManager->IsPvpZone() )
	{
		char	szMsg[256];
		if( g_pResourceManager->GetGlobalString(51000021) )
			wsprintf(szMsg, g_pResourceManager->GetGlobalString(51000021));
		else
			wsprintf(szMsg, "[Trade]You can't use it in PVP zone!!");
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMsg);
		return 0 ;
	}

	std::vector< SPGameObject* >	vPlayerList;

	int	iPlayerCount	=	g_pGOBManager->GetAllPlayer( vPlayerList );

	if( iPlayerCount <= 0 )
		return 0;

	SPGameObject*	pGameObject	=	NULL;
	char	szCharName[ LEN_NAME + 1 ];

	for( int i = 0 ; i < iPlayerCount ; i++ )
	{
		pGameObject	=	vPlayerList.at( i );

		if( pGameObject == NULL )
			continue;

		pGameObject->SPGOBPerformMessage( SPGM_GETGOBNAME , (LPARAM)szCharName );

		if( strCharName.compare( szCharName ) == 0 )
		{
			m_uiPlayerGUID	=	pGameObject->GetGUID();
			OnTrade( NULL , NULL );
			return 1;
		}
	}

	return 0;
}

//------------------------------------------------------------------------------------
int SPWindowPlayerInfo::OnSetEquip(WPARAM wParam, LPARAM lParam)
{
	ClearPlayerEquipSlot();
	CPacket* pkPacket = (CPacket*)lParam;

	GU_ID				ActCharGUID;
	UINT16				iItemCount;
	CONTAINER_ITEM		ContainerItem;

	pkPacket->ExtractUINT64(&ActCharGUID);
	pkPacket->ExtractUINT16(&iItemCount);

	for(int i = 0 ; i < iItemCount; i++ )
	{
		pkPacket->ExtractStruct(&ContainerItem, sizeof(CONTAINER_ITEM));	

		ContainerItem.SlotInfo.SlotIndex--;
		if( ContainerItem.SlotInfo.SlotIndex < 0 )
			break;

		ItemSetting(ContainerItem);
	}	

	SetEquipInfo();
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowPlayerInfo::OnSlotToolTipEnter(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	int iContainerIndex = m_iEquipType - FIGURE_EQUIP;	
	int iSlotIndex = iID - WIID_PC_INFO_EQUIP_SLOT_IMAGE;

	if( iContainerIndex > MAX_PLAYER_EQUIP_CONTAINER )
		return 1;

	if( iSlotIndex < 0 || iSlotIndex >= static_cast<int>(m_vpPlayerEquip[iContainerIndex].size()))
		return 1;

	SPItemStatus* pItemStatus = m_vpPlayerEquip[iContainerIndex].at(iSlotIndex)->GetItemStatus();

	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage( SPIM_TOOLTIP_ENTER, (WPARAM)pItemStatus, lParam);
	pWindow->Show();
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowPlayerInfo::OnSlotToolTipOut(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
	pWindow->Show();
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowPlayerInfo::OnSlotToolTipMove(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	int iContainerIndex = m_iEquipType - FIGURE_EQUIP;	
	int iSlotIndex = iID - WIID_PC_INFO_EQUIP_SLOT_IMAGE;

	if( iContainerIndex > MAX_PLAYER_EQUIP_CONTAINER )
		return 1;

	if( iSlotIndex < 0 || iSlotIndex >= static_cast<int>(m_vpPlayerEquip[iContainerIndex].size()))
		return 1;

	SPItemStatus* pItemStatus = m_vpPlayerEquip[iContainerIndex].at(iSlotIndex)->GetItemStatus();

	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage(SPIM_TOOLTIP_MOVE, (WPARAM)pItemStatus, lParam);
	pWindow->Show();

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowPlayerInfo::OnWP1ToolTipEnter(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	int iContainerIndex = 0;
	int iSlotIndex = iID - WIID_PC_INFO_EQUIP_WP1_SLOT_IMAGE;
	
	if(iSlotIndex == 0)
		iSlotIndex = EQ_WEAPON1 - 1;		//12
	else 
		iSlotIndex = EQ_BUCKLER1 - 1;		//13

	if( iContainerIndex > MAX_PLAYER_EQUIP_CONTAINER )
		return 1;
	if( iSlotIndex < 0 || iSlotIndex >= static_cast<int>(m_vpPlayerEquip[iContainerIndex].size()) )
		return 1;

	SPItemStatus* pItemStatus = m_vpPlayerEquip[iContainerIndex].at(iSlotIndex)->GetItemStatus();

	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage(SPIM_TOOLTIP_ENTER, (WPARAM)pItemStatus, lParam);
	pWindow->Show();

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowPlayerInfo::OnWP1ToolTipMove(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	int iContainerIndex = 0;
	int iSlotIndex = iID - WIID_PC_INFO_EQUIP_WP1_SLOT_IMAGE;

	if(iSlotIndex == 0)
		iSlotIndex = EQ_WEAPON1 - 1;		//12
	else 
		iSlotIndex = EQ_BUCKLER1 - 1;		//13

	if( iContainerIndex > MAX_PLAYER_EQUIP_CONTAINER )
		return 1;
	if( iSlotIndex < 0 || iSlotIndex >= static_cast<int>(m_vpPlayerEquip[iContainerIndex].size()) )
		return 1;

	SPItemStatus* pItemStatus = m_vpPlayerEquip[iContainerIndex].at(iSlotIndex)->GetItemStatus();

	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage(SPIM_TOOLTIP_MOVE, (WPARAM)pItemStatus, lParam);
	pWindow->Show();

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowPlayerInfo::OnWP2ToolTipEnter(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	int iContainerIndex = 0;
	int iSlotIndex = iID - WIID_PC_INFO_EQUIP_WP2_SLOT_IMAGE;

	if(iSlotIndex == 0)
		iSlotIndex = EQ_WEAPON2 - 1;
	else 
		iSlotIndex = EQ_BUCKLER2 - 1;

	if( iContainerIndex > MAX_PLAYER_EQUIP_CONTAINER )
		return 1;
	if( iSlotIndex < 0 || iSlotIndex >= static_cast<int>(m_vpPlayerEquip[iContainerIndex].size()) )
		return 1;

	SPItemStatus* pItemStatus = m_vpPlayerEquip[iContainerIndex].at(iSlotIndex)->GetItemStatus();

	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage(SPIM_TOOLTIP_ENTER, (WPARAM)pItemStatus, lParam);
	pWindow->Show();

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowPlayerInfo::OnWP2ToolTipMove(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	int iContainerIndex = 0;
	int iSlotIndex = iID - WIID_PC_INFO_EQUIP_WP2_SLOT_IMAGE;

	if(iSlotIndex == 0)
		iSlotIndex = EQ_WEAPON2 - 1;
	else 
		iSlotIndex = EQ_BUCKLER2 - 1;

	if( iContainerIndex > MAX_PLAYER_EQUIP_CONTAINER )
		return 1;
	if( iSlotIndex < 0 || iSlotIndex >= static_cast<int>(m_vpPlayerEquip[iContainerIndex].size()) )
		return 1;

	SPItemStatus* pItemStatus = m_vpPlayerEquip[iContainerIndex].at(iSlotIndex)->GetItemStatus();

	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage(SPIM_TOOLTIP_MOVE, (WPARAM)pItemStatus, lParam);
	pWindow->Show();

	return 1;
}


int SPWindowPlayerInfo::OnSlotRClick(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	int iContainerIndex = m_iEquipType - FIGURE_EQUIP;	
	int iSlotIndex = iID - WIID_PC_INFO_EQUIP_SLOT_IMAGE;

	if( iContainerIndex > MAX_PLAYER_EQUIP_CONTAINER )
		return 1;

	if( iSlotIndex < 0 || iSlotIndex >= static_cast<int>(m_vpPlayerEquip[iContainerIndex].size()))
		return 1;	
	
	SPItem* pItem = m_vpPlayerEquip[iContainerIndex].at(iSlotIndex);
	if(!pItem->IsEmptyItem()) {
		if(pItem->GetItemAttr()->m_bCashCheck && pItem->GetItemAttr()->m_iShopID) {
			SPCashTradeManager::GetInstance()->SetSelectCashItem(pItem->GetItemAttr()->m_iShopID);
		}
	}

	return 1;
}


int SPWindowPlayerInfo::OnWP1RClick(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	int iContainerIndex = 0;
	int iSlotIndex = iID - WIID_PC_INFO_EQUIP_WP1_SLOT_IMAGE;

	if(iSlotIndex == 0)
		iSlotIndex = EQ_WEAPON1 - 1;		//12
	else 
		iSlotIndex = EQ_BUCKLER1 - 1;		//13

	if( iContainerIndex > MAX_PLAYER_EQUIP_CONTAINER )
		return 1;
	if( iSlotIndex < 0 || iSlotIndex >= static_cast<int>(m_vpPlayerEquip[iContainerIndex].size()) )
		return 1;

	SPItem* pItem = m_vpPlayerEquip[iContainerIndex].at(iSlotIndex);
	if(!pItem->IsEmptyItem()) {
		if(pItem->GetItemAttr()->m_bCashCheck && pItem->GetItemAttr()->m_iShopID) {
			SPCashTradeManager::GetInstance()->SetSelectCashItem(pItem->GetItemAttr()->m_iShopID);
		}
	}
	
	return 1;
}


int SPWindowPlayerInfo::OnWP2RClick(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	int iContainerIndex = 0;
	int iSlotIndex = iID - WIID_PC_INFO_EQUIP_WP2_SLOT_IMAGE;

	if(iSlotIndex == 0)
		iSlotIndex = EQ_WEAPON2 - 1;
	else 
		iSlotIndex = EQ_BUCKLER2 - 1;

	if( iContainerIndex > MAX_PLAYER_EQUIP_CONTAINER )
		return 1;
	if( iSlotIndex < 0 || iSlotIndex >= static_cast<int>(m_vpPlayerEquip[iContainerIndex].size()) )
		return 1;

	SPItem* pItem = m_vpPlayerEquip[iContainerIndex].at(iSlotIndex);
	if(!pItem->IsEmptyItem()) {
		if(pItem->GetItemAttr()->m_bCashCheck && pItem->GetItemAttr()->m_iShopID) {
			SPCashTradeManager::GetInstance()->SetSelectCashItem(pItem->GetItemAttr()->m_iShopID);
		}
	}
	
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowPlayerInfo::OnLoveCharInfo(WPARAM wParam, LPARAM lParam)
{
	CPacket*		pPacket		=	(CPacket*)lParam;

	if( pPacket == NULL )
		return 1;

	bool			bWedding						=	false;	// 결혼정보가 있을때 밑에 정보를 같이 보낸다
	UINT8			iWeddingLv						=	0;		// 결혼레벨
	int				iWeddingDay						=	0;		// 교제일수 또는 결혼일수
	TCHAR			szWeddingPartName[LEN_NAME+1]	=	{ 0, };	// 결혼상대방이름

	//////////////////////////////////////////////////////////////////////////

	pPacket->ExtractStruct( &bWedding , sizeof( bWedding ) );

	if( bWedding == true )
	{
		pPacket->ExtractStruct( &iWeddingLv			, sizeof( iWeddingLv )			);
		pPacket->ExtractStruct( &iWeddingDay		, sizeof( iWeddingDay )			);
		pPacket->ExtractStruct( szWeddingPartName	, sizeof( szWeddingPartName )	);
	}

	SetLoveCharInfo( iWeddingLv , iWeddingDay , szWeddingPartName );

	return 1;
}


//------------------------------------------------------------------------------------
SPIMESSAGE_OBJ_MAP_BEGIN(SPWindowPlayerInfo)

SPIMESSAGE_COMMAND(	SPIM_SET_PLAYER_INFO,		OnSetPlayerID	)
SPIMESSAGE_COMMAND(	SPIM_TRADE_REQUEST,			OnTradeRequest	)
SPIMESSAGE_COMMAND(	SPIM_PC_INFO_SET_EQUIP,		OnSetEquip		)
SPIMESSAGE_COMMAND( SPIM_LOVE_CHAR_INFO,		OnLoveCharInfo	)

SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_PC_INFO_TAB_STATUS,		OnTabStatus		)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_PC_INFO_TAB_EQUIP,			OnTabEquip		)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_PC_INFO_TAB_LOVE,			OnTabLove		)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_PC_INFO_EQUIP_BATTLE,		OnEquipBattle	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_PC_INFO_EQUIP_FASHION,		OnEquipFashion	)

SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_PC_INFO_EXIT,				OnExit		)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_PC_INFO_STATUS_TRADE,		OnTrade		)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_PC_INFO_STATUS_WHISPER,	OnWhisper	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_PC_INFO_STATUS_INVITE,		OnInvite	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_PC_INFO_STATUS_FRIEND,		OnFriend	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_PC_INFO_STATUS_GUILD,		OnInviteGuild	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_PC_INFO_STATUS_CLOSED,		OnClosed	)

SPIMESSAGE_CONTROL_RANGE(SPIM_TOOLTIP_ENTER,	WIID_PC_INFO_EQUIP_SLOT_IMAGE, WIID_PC_INFO_EQUIP_SLOT_IMAGE + 11,	OnSlotToolTipEnter	)
SPIMESSAGE_CONTROL_RANGE(SPIM_TOOLTIP_OUT,		WIID_PC_INFO_EQUIP_SLOT_IMAGE, WIID_PC_INFO_EQUIP_SLOT_IMAGE + 11,	OnSlotToolTipOut	)
SPIMESSAGE_CONTROL_RANGE(SPIM_TOOLTIP_MOVE,		WIID_PC_INFO_EQUIP_SLOT_IMAGE, WIID_PC_INFO_EQUIP_SLOT_IMAGE + 11,	OnSlotToolTipMove	)

SPIMESSAGE_CONTROL_RANGE(SPIM_TOOLTIP_ENTER,	WIID_PC_INFO_EQUIP_WP1_SLOT_IMAGE, WIID_PC_INFO_EQUIP_WP1_SLOT_IMAGE + 1,	OnWP1ToolTipEnter	)
SPIMESSAGE_CONTROL_RANGE(SPIM_TOOLTIP_OUT,		WIID_PC_INFO_EQUIP_WP1_SLOT_IMAGE, WIID_PC_INFO_EQUIP_WP1_SLOT_IMAGE + 1,	OnSlotToolTipOut		)
SPIMESSAGE_CONTROL_RANGE(SPIM_TOOLTIP_MOVE,		WIID_PC_INFO_EQUIP_WP1_SLOT_IMAGE, WIID_PC_INFO_EQUIP_WP1_SLOT_IMAGE + 1,	OnWP1ToolTipMove	)

SPIMESSAGE_CONTROL_RANGE(SPIM_TOOLTIP_ENTER,	WIID_PC_INFO_EQUIP_WP2_SLOT_IMAGE, WIID_PC_INFO_EQUIP_WP2_SLOT_IMAGE + 1,	OnWP2ToolTipEnter	)
SPIMESSAGE_CONTROL_RANGE(SPIM_TOOLTIP_OUT,		WIID_PC_INFO_EQUIP_WP2_SLOT_IMAGE, WIID_PC_INFO_EQUIP_WP2_SLOT_IMAGE + 1,	OnSlotToolTipOut	)
SPIMESSAGE_CONTROL_RANGE(SPIM_TOOLTIP_MOVE,		WIID_PC_INFO_EQUIP_WP2_SLOT_IMAGE, WIID_PC_INFO_EQUIP_WP2_SLOT_IMAGE + 1,	OnWP2ToolTipMove	)

SPIMESSAGE_CONTROL_RANGE(SPIM_RBUTTON_UP,		WIID_PC_INFO_EQUIP_SLOT_IMAGE, WIID_PC_INFO_EQUIP_SLOT_IMAGE + 11,	OnSlotRClick	)
SPIMESSAGE_CONTROL_RANGE(SPIM_RBUTTON_UP,		WIID_PC_INFO_EQUIP_WP1_SLOT_IMAGE, WIID_PC_INFO_EQUIP_WP1_SLOT_IMAGE + 1,	OnWP1RClick	)
SPIMESSAGE_CONTROL_RANGE(SPIM_RBUTTON_UP,		WIID_PC_INFO_EQUIP_WP2_SLOT_IMAGE, WIID_PC_INFO_EQUIP_WP2_SLOT_IMAGE + 1,	OnWP2RClick	)

SPIMESSAGE_OBJ_MAP_END(SPWindow)