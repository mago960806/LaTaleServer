
#include <vector>

#include "SPCommon.h"
#include "SPUtil.h"

#include "SPGameObject.h"
#include "SPGOBManager.h"

#include "SPGameObjectDEF.h"

#include "SPGameObject.h"
#include "SPManager.h"
#include "SPMouseCursor.h"
#include "SPIMEPool.h"
#include "SPSubGameManager.h"

#include "SPWindowDEF.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPWindowEdit.h"
#include "SPWindowPcTrade.h"

#include "SPStage.h"
#include "SPStageManager.h"
#include "SPTerrainAttribute.h"

#include "SPCommandConvert.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGOBCoordPhysics.h"
#include "SPPlayerCoordPhysics.h"
#include "SPPlayerInvenArchive.h"

#include "SPGOBStatus.h"
#include "SPPlayerStatus.h"
#include "SPPlayerStatusModel.h"

#include "Packet.h"
#include "PacketID.h"
#include "SPNetworkManager.h"

#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPItem.h"
#include "SPItemCluster.h"			//[2005/6/16] - jinhee
#include "SPPlayer.h"

#include "SPLocalizeManager.h"
#include "SPCheckManager.h"

#include "SPDebug.h"


SPWindowPcTrade::SPWindowPcTrade(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_PC_TRADE, InstanceID, iX, iY, iCX, iCY, pParent)	
, m_pHelp(NULL)
, m_pEditCash(NULL)
, m_pEditPoint(NULL)
, m_pEditCashDis(NULL)
, m_pEditPointDis(NULL)
, m_pFeeText(NULL)
, m_pFeeBack(NULL)
, m_pFee(NULL)
, m_pFeeImg(NULL)
, m_pPcCash(NULL)
, m_pPcPoint(NULL)
, m_pPcFeeText(NULL)
, m_pPcFee(NULL)
, m_pPcFeeImg(NULL)
{		
	Init();
}

SPWindowPcTrade::~SPWindowPcTrade()
{
	Clean();
}

void SPWindowPcTrade::Init()
{		
	InitPcItem();

	//m_iSrcPcAcceptX[PC_TRADE_ACCEPT_DISABLE] = 154;
	////m_iSrcPcAcceptX[PC_TRADE_ACCEPT_ENABLE] = 103;
	//m_iSrcPcAcceptX[PC_TRADE_ACCEPT_ENABLE] = 1;
	//m_iSrcPcAcceptX[PC_TRADE_ACCEPT_ACCEPT] = 52;
	//m_iSrcPcAcceptY = 431;

	m_bSendCancel = false;
	m_fStartPosX = 0.0f;
	m_fStartPosY = 0.0f;

	std::string strTextureFile = "";
	strTextureFile.clear();
	strTextureFile = "DATA/INTERFACE/CONCEPT/UI100.PNG";
	g_pVideo->GetTextureMgr()->LoadTexture(strTextureFile.c_str(), &m_pDisable);
	
	SetRect(&m_rtSrcDisable, 434 , 37 , 436 , 39);
	SetRect(&m_rtMarkSrc[0], 479, 152, 511, 184);		//ICON_MARK_LOCK
	SetRect(&m_rtMarkSrc[1], 479, 185, 511, 217);		//ICON_MARK_FASHION
	SetRect(&m_rtMarkSrc[2], 479, 339, 511, 371);		//ICON_MARK_EVENT
	SetRect(&m_rtMarkSrc[3], 479, 372, 511, 404);		//ICON_MARK_FANCY

	strTextureFile.clear();
	strTextureFile = "DATA/INTERFACE/CONCEPT/UI_WinBG_000.PNG";
	g_pVideo->GetTextureMgr()->LoadTexture(strTextureFile.c_str(), &m_pBaseSkin);
	
	SetRect(&m_rtBaseSrc[WINDOW_BASE_TOP_LEFT],			482, 482, 486, 486);
	SetRect(&m_rtBaseSrc[WINDOW_BASE_TOP],				487, 482, 491, 486);
	SetRect(&m_rtBaseSrc[WINDOW_BASE_TOP_RIGHT],		492, 482, 496, 486);
	SetRect(&m_rtBaseSrc[WINDOW_BASE_CENTER_LEFT],		482, 487, 486, 491);
	SetRect(&m_rtBaseSrc[WINDOW_BASE_CENTER],			487, 487, 491, 491);
	SetRect(&m_rtBaseSrc[WINDOW_BASE_CENTER_RIGHT],		492, 487, 496, 491);
	SetRect(&m_rtBaseSrc[WINDOW_BASE_BOTTOM_LEFT],		482, 492, 486, 496);
	SetRect(&m_rtBaseSrc[WINDOW_BASE_BOTTOM],			487, 492, 491, 496);
	SetRect(&m_rtBaseSrc[WINDOW_BASE_BOTTOM_RIGHT],		492, 492, 496, 496);
	
	SetWindowBase();

	CreateSubWindow();		

	//m_pMic = new SPWindowMicropone(WIID_MICROPHONE, 400, 0, 267, 173, this);

	m_bChanged = false;
	m_fChangedDeley = 3.0f;
	m_fChangedAccumulate = 0.0f;
}


void SPWindowPcTrade::SetWindowBase()
{
	//
	if(SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_ALL) {
		SetRect(&m_rtBaseDest[WINDOW_BASE_TOP_LEFT],		m_iAX, m_iAY, m_iAX + 4, m_iAY + 4);
		SetRect(&m_rtBaseDest[WINDOW_BASE_TOP],				m_iAX + 4, m_iAY, m_iAX + 4 + 376, m_iAY + 4);	
		SetRect(&m_rtBaseDest[WINDOW_BASE_TOP_RIGHT],		m_iAX + 380, m_iAY, m_iAX + 380 + 4, m_iAY + 4);
		SetRect(&m_rtBaseDest[WINDOW_BASE_CENTER_LEFT],		m_iAX, m_iAY + 4, m_iAX + 4, m_iAY + 4 + 315);
		SetRect(&m_rtBaseDest[WINDOW_BASE_CENTER],			m_iAX + 4, m_iAY + 4, m_iAX + 4 + 376, m_iAY + 4 + 315);
		SetRect(&m_rtBaseDest[WINDOW_BASE_CENTER_RIGHT],	m_iAX + 380, m_iAY + 4, m_iAX + 380 + 4, m_iAY + 4 + 315);
		SetRect(&m_rtBaseDest[WINDOW_BASE_BOTTOM_LEFT],		m_iAX, m_iAY + 319, m_iAX + 4, m_iAY + 319 + 4);
		SetRect(&m_rtBaseDest[WINDOW_BASE_BOTTOM],			m_iAX + 4, m_iAY + 319, m_iAX + 4 + 376, m_iAY + 319 + 4);
		SetRect(&m_rtBaseDest[WINDOW_BASE_BOTTOM_RIGHT],	m_iAX + 380, m_iAY + 319, m_iAX + 380 + 4, m_iAY + 319 + 4);
	}
	else if(SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_CASH || 
		SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_POINT)
	{
		SetRect(&m_rtBaseDest[WINDOW_BASE_TOP_LEFT],		m_iAX, m_iAY, m_iAX + 4, m_iAY + 4);
		SetRect(&m_rtBaseDest[WINDOW_BASE_TOP],				m_iAX + 4, m_iAY, m_iAX + 4 + 376, m_iAY + 4);	
		SetRect(&m_rtBaseDest[WINDOW_BASE_TOP_RIGHT],		m_iAX + 380, m_iAY, m_iAX + 380 + 4, m_iAY + 4);
		SetRect(&m_rtBaseDest[WINDOW_BASE_CENTER_LEFT],		m_iAX, m_iAY + 4, m_iAX + 4, m_iAY + 4 + 300);
		SetRect(&m_rtBaseDest[WINDOW_BASE_CENTER],			m_iAX + 4, m_iAY + 4, m_iAX + 4 + 376, m_iAY + 4 + 300);
		SetRect(&m_rtBaseDest[WINDOW_BASE_CENTER_RIGHT],	m_iAX + 380, m_iAY + 4, m_iAX + 380 + 4, m_iAY + 4 + 300);
		SetRect(&m_rtBaseDest[WINDOW_BASE_BOTTOM_LEFT],		m_iAX, m_iAY + 304, m_iAX + 4, m_iAY + 304 + 4);
		SetRect(&m_rtBaseDest[WINDOW_BASE_BOTTOM],			m_iAX + 4, m_iAY + 304, m_iAX + 4 + 376, m_iAY + 304 + 4);
		SetRect(&m_rtBaseDest[WINDOW_BASE_BOTTOM_RIGHT],	m_iAX + 380, m_iAY + 304, m_iAX + 380 + 4, m_iAY + 304 + 4);
	}
	else {
		SetRect(&m_rtBaseDest[WINDOW_BASE_TOP_LEFT],		m_iAX, m_iAY, m_iAX + 4, m_iAY + 4);
		SetRect(&m_rtBaseDest[WINDOW_BASE_TOP],				m_iAX + 4, m_iAY, m_iAX + 4 + 376, m_iAY + 4);	
		SetRect(&m_rtBaseDest[WINDOW_BASE_TOP_RIGHT],		m_iAX + 380, m_iAY, m_iAX + 380 + 4, m_iAY + 4);
		SetRect(&m_rtBaseDest[WINDOW_BASE_CENTER_LEFT],		m_iAX, m_iAY + 4, m_iAX + 4, m_iAY + 4 + 262);
		SetRect(&m_rtBaseDest[WINDOW_BASE_CENTER],			m_iAX + 4, m_iAY + 4, m_iAX + 4 + 376, m_iAY + 4 + 262);
		SetRect(&m_rtBaseDest[WINDOW_BASE_CENTER_RIGHT],	m_iAX + 380, m_iAY + 4, m_iAX + 380 + 4, m_iAY + 4 + 262);
		SetRect(&m_rtBaseDest[WINDOW_BASE_BOTTOM_LEFT],		m_iAX, m_iAY + 266, m_iAX + 4, m_iAY + 266 + 4);
		SetRect(&m_rtBaseDest[WINDOW_BASE_BOTTOM],			m_iAX + 4, m_iAY + 266, m_iAX + 4 + 376, m_iAY + 266 + 4);
		SetRect(&m_rtBaseDest[WINDOW_BASE_BOTTOM_RIGHT],	m_iAX + 380, m_iAY + 266, m_iAX + 380 + 4, m_iAY + 266 + 4);
	}	
}


void SPWindowPcTrade::CreateSubWindow()
{
///*
	char* BG_IMAGE_FILE0		= "DATA/INTERFACE/CONCEPT/UI_WinBG_000.PNG";
	char* OBJECT_IMAGE_FILE0	= "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG";
	char* BUTTON_IMAGE_FILE0	= "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG";
	
	SPWindowStatic* pStatic;
	SPWindowStatic* pSkin = NULL;
	SPWindowButton* pButton;
	SPWindowEdit* pEdit;
	int iCenterX = 0, iCenterY = 0, iOffset = 0;

	SPWindowStatic* pCenter;
	SPWindowStatic* pSub_Top;
	SPWindowStatic* pSub_Bottom;

	// Top
	//pStatic = new SPWindowStatic(WIID_PC_TRADE_TOP, 0, 0, 395, 4, this);
	//pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 1, 80);
	//pStatic->Show();

	// WIID_PC_TRADE_TITLE
	pStatic = new SPWindowStatic(WIID_PC_TRADE_TITLE, 2, 2, 187, 24, this);
	pStatic->SetImage(OBJECT_IMAGE_FILE0, 1, 338);
	pStatic->Show();

	//WIID_PC_TRADE_LINE1
	pStatic = new SPWindowStatic(WIID_PC_TRADE_LINE1, 2, 26, 380, 1, this);
	pStatic->SetImage(BG_IMAGE_FILE0, 503, 450);
	pStatic->SetSrcSize(2, 1);
	pStatic->Show();

	//WIID_PC_TRADE_HIDE
	pButton = new SPWindowButton(WIID_PC_TRADE_HIDE, 363, 9, 12, 12, this);
	pButton->SetImage		(BUTTON_IMAGE_FILE0, 484,	1); //일반
	pButton->SetImageHit	(BUTTON_IMAGE_FILE0, 497,	1); //on	
	pButton->SetImagePush	(BUTTON_IMAGE_FILE0, 484,	14); //push
	pButton->SetImageDisable(BUTTON_IMAGE_FILE0, 497,	14); //disable
	pButton->Show();

	//WIID_PC_TRADE_CENTER
	iCenterX = 2;
	iCenterY = 27;
	pCenter = NULL;
	if(SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_ALL) {		
		pCenter = new SPWindowStatic(WIID_PC_TRADE_CENTER, iCenterX, iCenterY, 380, 292, this);
		pCenter->SetImage(BG_IMAGE_FILE0, 502, 487);
		pCenter->SetSrcSize(4, 4);
		pCenter->Show();
	}
	else if(SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_CASH || 
		SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_POINT)
	{
		pCenter = new SPWindowStatic(WIID_PC_TRADE_CENTER, iCenterX, iCenterY, 380, 277, this);
		pCenter->SetImage(BG_IMAGE_FILE0, 502, 487);
		pCenter->SetSrcSize(4, 4);
		pCenter->Show();
	}
	else {
		pCenter = new SPWindowStatic(WIID_PC_TRADE_CENTER, iCenterX, iCenterY, 380, 239, this);
		pCenter->SetImage(BG_IMAGE_FILE0, 502, 487);
		pCenter->SetSrcSize(4, 4);
		pCenter->Show();
	}	

	if(pCenter == NULL) {
		return;
	}
	
	
	//// Sub_Top
	//pSub_Top = new SPWindowStatic(WIID_PC_TRADE_SUB_TOP, 2-iCenterX, 26-iCenterY, 391, 25, pCenter);
	//pSub_Top->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 493, 486);
	//pSub_Top->SetSrcSize(6, 6);
	//pSub_Top->Show();

	// WIID_PC_TRADE_PC_NAME_WAIT
	pStatic = new SPWindowStatic(WIID_PC_TRADE_PC_NAME_WAIT, 10-iCenterX, 35-iCenterY, 172, 15, pCenter);
	pStatic->SetFormat(DT_CENTER | DT_VCENTER);
	pStatic->SetWindowType( WND_TYPE_LEFTBASE );
	pStatic->SetImageNormalHead(OBJECT_IMAGE_FILE0, 380, 284, 384, 299);
	pStatic->SetImageNormalBody(OBJECT_IMAGE_FILE0, 384, 284, 388, 299);
	pStatic->SetImageNormalTail(OBJECT_IMAGE_FILE0, 388, 284, 392, 299);
	//pStatic->SetWindowText("캐릭터명");
	pStatic->SetFontColor(RGBA(119, 76, 95, 255));
	pStatic->SetFont(FONT_12_BOLD);
	pStatic->Show();
	m_pPcNameWait = pStatic;

	//WIID_PC_TRADE_PC_NAME_ACCEPT
	pStatic = new SPWindowStatic(WIID_PC_TRADE_PC_NAME_ACCEPT, 10-iCenterX, 35-iCenterY, 172, 15, pCenter);
	pStatic->SetFormat(DT_CENTER | DT_VCENTER);
	pStatic->SetWindowType( WND_TYPE_LEFTBASE );
	pStatic->SetImageNormalHead(OBJECT_IMAGE_FILE0, 380, 300, 384, 315);
	pStatic->SetImageNormalBody(OBJECT_IMAGE_FILE0, 384, 300, 388, 315);
	pStatic->SetImageNormalTail(OBJECT_IMAGE_FILE0, 388, 300, 392, 315);
	//pStatic->SetWindowText("캐릭터명");
	pStatic->SetFontColor(RGBA(255, 255, 255, 255));
	pStatic->SetFont(FONT_12_BOLD);
	pStatic->Show();
	m_pPcNameAccept = pStatic;

	//WIID_PC_TRADE_PC_STATUS_ACCEPT
	pStatic = new SPWindowStatic(WIID_PC_TRADE_PC_STATUS_ACCEPT, 136-iCenterX, 51-iCenterY, 42, 10, pCenter);
	pStatic->SetImage(OBJECT_IMAGE_FILE0, 295, 101);
	pStatic->Hide();
	m_pPcStatus_Accept = pStatic;

	//WIID_PC_TRADE_NAME_WAIT
	pStatic = new SPWindowStatic(WIID_PC_TRADE_NAME_WAIT, 202-iCenterX, 35-iCenterY, 172, 15, pCenter);
	pStatic->SetFormat(DT_CENTER | DT_VCENTER);
	pStatic->SetWindowType( WND_TYPE_LEFTBASE );
	pStatic->SetImageNormalHead(OBJECT_IMAGE_FILE0, 380, 284, 384, 299);
	pStatic->SetImageNormalBody(OBJECT_IMAGE_FILE0, 384, 284, 388, 299);
	pStatic->SetImageNormalTail(OBJECT_IMAGE_FILE0, 388, 284, 392, 299);
	//pStatic->SetWindowText("캐릭터명");
	pStatic->SetFontColor(RGBA(119, 76, 95, 255));
	pStatic->SetFont(FONT_12_BOLD);
	pStatic->Show();
	m_pNameWait = pStatic;
	
	//WIID_PC_TRADE_NAME_ACCEPT
	pStatic = new SPWindowStatic(WIID_PC_TRADE_NAME_ACCEPT, 202-iCenterX, 35-iCenterY, 172, 15, pCenter);
	pStatic->SetFormat(DT_CENTER | DT_VCENTER);
	pStatic->SetWindowType( WND_TYPE_LEFTBASE );
	pStatic->SetImageNormalHead(OBJECT_IMAGE_FILE0, 380, 284, 384, 299);
	pStatic->SetImageNormalBody(OBJECT_IMAGE_FILE0, 384, 284, 388, 299);
	pStatic->SetImageNormalTail(OBJECT_IMAGE_FILE0, 388, 284, 392, 299);
	//pStatic->SetWindowText("캐릭터명");
	pStatic->SetFontColor(RGBA(255, 255, 255, 255));
	pStatic->SetFont(FONT_12_BOLD);
	pStatic->Show();
	m_pNameAccept = pStatic;

	//WIID_PC_TRADE_STATUS_ACCEPT
	pStatic = new SPWindowStatic(WIID_PC_TRADE_STATUS_ACCEPT, 328-iCenterX, 51-iCenterY, 42, 10, pCenter);
	pStatic->SetImage(OBJECT_IMAGE_FILE0, 295, 101);
	pStatic->Hide();
	m_pStatus_Accept = pStatic;		
	
	// WIID_PC_TRADE_VLINE1
	int iVLineHeight = 0;
	if(SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_ALL) {		
		iVLineHeight = 222;
	}
	else if(SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_CASH || 
		SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_POINT)
	{
		iVLineHeight = 207;
	}
	else {
		iVLineHeight = 188;
	}
	pStatic = new SPWindowStatic(WIID_PC_TRADE_VLINE1, 191-iCenterX, 35-iCenterY, 2, iVLineHeight, pCenter);
	pStatic->SetImage(OBJECT_IMAGE_FILE0, 186, 308);
	pStatic->SetSrcSize(1, 3);
	pStatic->Show();
	
	//// PC_Status_Wait
	//pStatic = new SPWindowStatic(WIID_PC_TRADE_PC_STATUS_WAIT, 10-iX, 30-iY, 40, 19, pSub_Top);
	//pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 197, 448);
	//pStatic->Show();
	//m_pPcStatus_Wait = pStatic;

	//// Status_Wait
	//pStatic = new SPWindowStatic(WIID_PC_TRADE_STATUS_WAIT, 206-iX, 30-iY, 40, 19, pSub_Top);
	//pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 197, 448);
	//pStatic->Show();
	//m_pStatus_Wait = pStatic;	

	//// Sub_HLine1
	//pStatic = new SPWindowStatic(WIID_PC_TRADE_SUB_HLINE1, 2-iCenterX, 51-iCenterY, 391, 1, pCenter);
	//pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 7, 92);
	//pStatic->SetSrcSize(2, 1);
	//pStatic->Show();

	//// Sub_Bottom
	//pSub_Bottom = new SPWindowStatic(WIID_PC_TRADE_SUB_BOTTOM, 2-iCenterX, 52-iCenterY, 391, 212, pCenter);
	//pSub_Bottom->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 11, 52);
	//pSub_Bottom->SetSrcSize(2, 2);
	//pSub_Bottom->Show();

	iOffset = 36;
	for(int i=0; i<MAX_PC_TRADE_COUNT; i++)
	{
		// PC_ToolTip & 아이템 올리기 이벤트
		pButton = new SPWindowButton(WIID_PC_TRADE_PC_ITEM_BTN+i, 14-iCenterX, 64-iCenterY+(iOffset*i), 168, 32, pCenter);
		pButton->Show();

		// PC_Item_IconLine
		pStatic = new SPWindowStatic(WIID_PC_TRADE_PC_ITEM_ICONLINE+i, 14-iCenterX, 64-iCenterY+(iOffset*i), 32, 32, pCenter);
		pStatic->SetImage(OBJECT_IMAGE_FILE0, 353, 102);
		pStatic->Show();

		// PC_Item_Icon
		pStatic = new SPWindowStatic(WIID_PC_TRADE_PC_ITEM_ICON+i, 11-iCenterX, 64-iCenterY+(iOffset*i), 32, 32, pCenter);
		pStatic->Show();
		m_pPCItemIcon[i] = pStatic;				

		// 아이템관련 정보 설정
		m_bPcItemAble[i] = true;
		m_iPcItemMark[i] = ICON_MARK_NULL;
		m_iPcStack[i] = 0;
		m_pPCItemIcon[i]->GetWindowRect(m_rtPcStack[i]);

		// PC_Item_Text
		pStatic = new SPWindowStatic(WIID_PC_TRADE_PC_ITEM_TEXT+i, 54-iCenterX, 67-iCenterY+(iOffset*i), 128, 27, pCenter);
		//pStatic->SetWindowText("아이템명");
		pStatic->SetFormat(DT_LEFT | DT_VCENTER);
		pStatic->SetMultiLine(true, 2);
		pStatic->SetFontColor(RGBA(143, 99, 99, 255));
		pStatic->Show();
		m_pPCItemText[i] = pStatic;				

		// ToolTip & 아이템 올리기 이벤트
		pButton = new SPWindowButton(WIID_PC_TRADE_ITEM_BTN+i, 206-iCenterX, 64-iCenterY+(iOffset*i), 168, 32, pCenter);
		pButton->Show();

		// Item_IconLine
		pStatic = new SPWindowStatic(WIID_PC_TRADE_ITEM_ICONLINE+i, 206-iCenterX, 64-iCenterY+(iOffset*i), 32, 32, pCenter);
		pStatic->SetImage(OBJECT_IMAGE_FILE0, 353, 102);
		pStatic->Show();

		// Item_Icon
		pStatic = new SPWindowStatic(WIID_PC_TRADE_ITEM_ICON+i, 206-iCenterX, 64-iCenterY+(iOffset*i), 32, 32, pCenter);
		pStatic->Show();
		m_pItemIcon[i] = pStatic;


		// 아이템관련 정보 설정
		m_bItemAble[i] = true;
		m_iItemMark[i] = ICON_MARK_NULL;
		m_iStack[i] = 0;		
		m_pItemIcon[i]->GetWindowRect(m_rtStack[i]);

		// Item_Text
		pStatic = new SPWindowStatic(WIID_PC_TRADE_ITEM_TEXT+i, 246-iCenterX, 67-iCenterY+(iOffset*i), 128, 27, pCenter);
		//pStatic->SetWindowText("아이템명");
		pStatic->SetFormat(DT_LEFT | DT_VCENTER);
		pStatic->SetMultiLine(true, 2);
		pStatic->SetFontColor(RGBA(143, 99, 99, 255));
		pStatic->Show();
		m_pItemText[i] = pStatic;
	}	
	
	//// PC_Text
	//pStatic = new SPWindowStatic(WIID_PC_TRADE_PC_TEXT, 10-iX, 60-iY, 179, 15, pSub_Bottom);
	//pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 257, 15);
	//pStatic->Show();

	//// Text
	//pStatic = new SPWindowStatic(WIID_PC_TRADE_TEXT, 206-iX, 60-iY, 179, 15, pSub_Bottom);
	//pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 257, 31);
	//pStatic->Show();

	// WIID_PC_TRADE_PC_ELY_BACK
	pStatic = new SPWindowStatic(WIID_PC_TRADE_PC_ELY_BACK, 18-iCenterX, 208-iCenterY, 129, 15, pCenter);	
	pStatic->SetWindowType( WND_TYPE_LEFTBASE );
	pStatic->SetImageNormalHead(OBJECT_IMAGE_FILE0, 380, 284, 384, 299);
	pStatic->SetImageNormalBody(OBJECT_IMAGE_FILE0, 384, 284, 388, 299);
	pStatic->SetImageNormalTail(OBJECT_IMAGE_FILE0, 388, 284, 392, 299);	
	pStatic->Show();

	//WIID_PC_TRADE_PC_ELY
	pStatic = new SPWindowStatic(WIID_PC_TRADE_PC_ELY, 24-iCenterX, 208-iCenterY, 118, 15, pCenter);
	pStatic->SetFormat(DT_RIGHT | DT_VCENTER);	
	pStatic->SetWindowText("123,456,789");
	pStatic->SetFontColor(RGBA(143, 99, 99, 255));
	m_pPcEly = pStatic;

	// PC_ElyImg
	pStatic = new SPWindowStatic(WIID_PC_TRADE_PC_ELYIMG, 154-iCenterX, 212-iCenterY, 17, 5, pCenter);
	pStatic->SetImage(OBJECT_IMAGE_FILE0, 476, 111);
	pStatic->Show();
	
	// WIID_PC_TRADE_ELY_BASE
	pSkin = new SPWindowStatic(WIID_PC_TRADE_ELY_BASE, 210-iCenterX, 208-iCenterY, 129, 15, pCenter);
	pSkin->SetWindowType(WND_TYPE_LEFTBASE);
	pSkin->SetImageNormalHead	(OBJECT_IMAGE_FILE0, 380, 316, 384, 332);
	pSkin->SetImageNormalBody	(OBJECT_IMAGE_FILE0, 384, 316, 388, 332);
	pSkin->SetImageNormalTail	(OBJECT_IMAGE_FILE0, 388, 316, 392, 332);
	pSkin->Show();

	// Ely
	pEdit = new SPWindowEdit(WIID_PC_TRADE_ELY, 4, 0, 121, 15, pSkin);
	pEdit->SetWindowType(WND_TYPE_LEFTBASE);
	pEdit->SetMargin(2, 1);
	//pEdit->SetImageNormalHead	(OBJECT_IMAGE_FILE0, 380, 316, 384, 332);
	//pEdit->SetImageNormalBody	(OBJECT_IMAGE_FILE0, 384, 316, 388, 332);
	//pEdit->SetImageNormalTail	(OBJECT_IMAGE_FILE0, 388, 316, 392, 332);
	//pEdit->SetImageFocusHead	(OBJECT_IMAGE_FILE0, 380, 316, 384, 332);
	//pEdit->SetImageFocusBody	(OBJECT_IMAGE_FILE0, 384, 316, 388, 332);
	//pEdit->SetImageFocusTail	(OBJECT_IMAGE_FILE0, 388, 316, 392, 332);
	pEdit->SetFontColor(RGBA(143, 99, 99, 255));
	pEdit->SetNumber();
	pEdit->SetMinValue(0);
	pEdit->SetMaxValue(999999999);
	pEdit->SetFormat(DT_RIGHT | DT_VCENTER);
	pEdit->Show();
	m_pEditEly = pEdit;

	// ElyImg
	pStatic = new SPWindowStatic(WIID_PC_TRADE_ELYIMG, 347-iCenterX, 212-iCenterY, 17, 5, pCenter);
	pStatic->SetImage(OBJECT_IMAGE_FILE0, 476, 111);
	pStatic->Show();

	int iEditStartY = 208 + 15;
	int iImgStartY = 212 + 5;

	int iEditInterval = 2;
	int iImgInterval = 7;

	//if(SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_ALL || 
	//	SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_CASH)
	//{		
		iEditStartY += iEditInterval;
		iImgStartY += iImgInterval + 3;			//Ely와 Cash 사이는 10
		
		//WIID_PC_TRADE_PC_CASH_BACK
		pStatic = new SPWindowStatic(WIID_PC_TRADE_PC_CASH_BACK, 18-iCenterX, iEditStartY - iCenterY, 129, 15, pCenter);
		pStatic->SetWindowType( WND_TYPE_LEFTBASE );
		pStatic->SetImageNormalHead(OBJECT_IMAGE_FILE0, 380, 284, 384, 299);
		pStatic->SetImageNormalBody(OBJECT_IMAGE_FILE0, 384, 284, 388, 299);
		pStatic->SetImageNormalTail(OBJECT_IMAGE_FILE0, 388, 284, 392, 299);
		pStatic->Hide();
		
		//WIID_PC_TRADE_PC_CASH
		pStatic = new SPWindowStatic(WIID_PC_TRADE_PC_CASH, 24-iCenterX, iEditStartY - iCenterY, 118, 15, pCenter);
		pStatic->SetFormat(DT_RIGHT | DT_VCENTER);		
		pStatic->SetWindowText("PC Cash");
		pStatic->SetFontColor(RGBA(143, 99, 99, 255));
		pStatic->Hide();
		m_pPcCash = pStatic;

		//WIID_PC_TRADE_PC_CASHIMG
		pStatic = new SPWindowStatic(WIID_PC_TRADE_PC_CASHIMG, 154-iCenterX, iImgStartY - iCenterY, 15, 11, pCenter);
		pStatic->SetImage(OBJECT_IMAGE_FILE0, 476, 97);
		pStatic->Hide();

		//WIID_PC_TRADE_PC_CASHDIS	
		pStatic = new SPWindowStatic(WIID_PC_TRADE_PC_CASHDIS, 18-iCenterX, iEditStartY-iCenterY, 156, 16, pCenter);
		pStatic->SetImage(OBJECT_IMAGE_FILE0, 168, 308);
		pStatic->SetSrcSize(3, 3);
		pStatic->Hide();
		m_pPcCashDis = pStatic;

		//WIID_PC_TRADE_CASH_BACK		
		pSkin = new SPWindowStatic(WIID_PC_TRADE_CASH_BACK, 210-iCenterX, iEditStartY-iCenterY, 129, 15, pCenter);
		pSkin->SetWindowType(WND_TYPE_LEFTBASE);
		pSkin->SetImageNormalHead	(OBJECT_IMAGE_FILE0, 380, 316, 384, 332);
		pSkin->SetImageNormalBody	(OBJECT_IMAGE_FILE0, 384, 316, 388, 332);
		pSkin->SetImageNormalTail	(OBJECT_IMAGE_FILE0, 388, 316, 392, 332);
		pSkin->Hide();
		
		//WIID_PC_TRADE_CASH
		pEdit = new SPWindowEdit(WIID_PC_TRADE_CASH, 0, 0, 129, 15, pSkin);
		//pEdit = new SPWindowEdit(WIID_PC_TRADE_CASH, 210-iCenterX, iEditStartY-iCenterY, 129, 15, pCenter);
		pEdit->SetWindowType(WND_TYPE_LEFTBASE);
		pEdit->SetMargin(2, 1);
		//pEdit->SetImageNormalHead	(OBJECT_IMAGE_FILE0, 380, 316, 384, 332);
		//pEdit->SetImageNormalBody	(OBJECT_IMAGE_FILE0, 384, 316, 388, 332);
		//pEdit->SetImageNormalTail	(OBJECT_IMAGE_FILE0, 388, 316, 392, 332);
		//pEdit->SetImageFocusHead	(OBJECT_IMAGE_FILE0, 380, 316, 384, 332);
		//pEdit->SetImageFocusBody	(OBJECT_IMAGE_FILE0, 384, 316, 388, 332);
		//pEdit->SetImageFocusTail	(OBJECT_IMAGE_FILE0, 388, 316, 392, 332);
		pEdit->SetFontColor(RGBA(143, 99, 99, 255));
		pEdit->SetNumber();
		pEdit->SetMinValue(0);
		pEdit->SetMaxValue(999999999);
		pEdit->SetFormat(DT_RIGHT | DT_VCENTER);
		pEdit->Hide();
		m_pEditCash = pEdit;

		//WIID_PC_TRADE_CASH_UNIT
		pStatic = new SPWindowStatic(WIID_PC_TRADE_CASH_UNIT, 0, 0, 1, 1, pSkin);
		pStatic->SetFormat(DT_LEFT | DT_VCENTER);
		//pStatic->SetMargin(2, 1);
		pStatic->SetFontColor(RGBA(143, 99, 99, 255));
		pStatic->Hide();

		//WIID_PC_TRADE_CASHIMG
		pStatic = new SPWindowStatic(WIID_PC_TRADE_CASHIMG, 348-iCenterX, iImgStartY-iCenterY, 15, 11, pCenter);
		pStatic->SetImage(OBJECT_IMAGE_FILE0, 476, 97);
		pStatic->Hide();
		
		//WIID_PC_TRADE_CASHDIS	
		pStatic = new SPWindowStatic(WIID_PC_TRADE_CASHDIS, 210-iCenterX, iEditStartY-iCenterY, 156, 16, pCenter);
		pStatic->SetImage(OBJECT_IMAGE_FILE0, 168, 308);
		pStatic->SetSrcSize(3, 3);
		pStatic->Hide();
		m_pEditCashDis = pStatic;

		iEditStartY += 15;
		iImgStartY += 11;
	//}
	
	//if(SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_ALL || 
	//	SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_POINT)
	//{
		iEditStartY += iEditInterval;
		iImgStartY += iImgInterval;

		//WIID_PC_TRADE_PC_POINT_BACK
		pStatic = new SPWindowStatic(WIID_PC_TRADE_PC_POINT_BACK, 18-iCenterX, iEditStartY - iCenterY, 129, 15, pCenter);		
		pStatic->SetWindowType( WND_TYPE_LEFTBASE );
		pStatic->SetImageNormalHead(OBJECT_IMAGE_FILE0, 380, 284, 384, 299);
		pStatic->SetImageNormalBody(OBJECT_IMAGE_FILE0, 384, 284, 388, 299);
		pStatic->SetImageNormalTail(OBJECT_IMAGE_FILE0, 388, 284, 392, 299);
		pStatic->Hide();
		
		//WIID_PC_TRADE_PC_POINT
		pStatic = new SPWindowStatic(WIID_PC_TRADE_PC_POINT, 24-iCenterX, iEditStartY - iCenterY, 118, 15, pCenter);
		pStatic->SetFormat(DT_RIGHT | DT_VCENTER);
		pStatic->SetWindowText("PC Point");
		pStatic->SetFontColor(RGBA(143, 99, 99, 255));
		pStatic->Hide();
		m_pPcPoint = pStatic;
		
		//WIID_PC_TRADE_PC_POINTIMG
		pStatic = new SPWindowStatic(WIID_PC_TRADE_PC_POINTIMG, 154-iCenterX, iImgStartY - iCenterY, 15, 11, pCenter);
		pStatic->SetImage(OBJECT_IMAGE_FILE0, 492, 97);
		pStatic->Hide();

		//WIID_PC_TRADE_PC_POINTDIS	
		pStatic = new SPWindowStatic(WIID_PC_TRADE_PC_POINTDIS, 18-iCenterX, iEditStartY-iCenterY, 156, 16, pCenter);
		pStatic->SetImage(OBJECT_IMAGE_FILE0, 168, 308);
		pStatic->SetSrcSize(3, 3);
		pStatic->Hide();
		m_pPcPointDis = pStatic;

		//WIID_PC_TRADE_POINT_BACK
		pSkin = new SPWindowStatic(WIID_PC_TRADE_POINT_BACK, 210-iCenterX, iEditStartY-iCenterY, 129, 15, pCenter);
		pSkin->SetWindowType(WND_TYPE_LEFTBASE);
		pSkin->SetImageNormalHead	(OBJECT_IMAGE_FILE0, 380, 316, 384, 332);
		pSkin->SetImageNormalBody	(OBJECT_IMAGE_FILE0, 384, 316, 388, 332);
		pSkin->SetImageNormalTail	(OBJECT_IMAGE_FILE0, 388, 316, 392, 332);
		pSkin->Hide();
		
		//WIID_PC_TRADE_POINT
		pEdit = new SPWindowEdit(WIID_PC_TRADE_POINT, 0, 0, 129, 15, pSkin);
		//pEdit = new SPWindowEdit(WIID_PC_TRADE_POINT, 210-iCenterX, iEditStartY-iCenterY, 129, 15, pCenter);
		pEdit->SetWindowType(WND_TYPE_LEFTBASE);
		pEdit->SetMargin(2, 1);
		//pEdit->SetImageNormalHead	(OBJECT_IMAGE_FILE0, 380, 316, 384, 332);
		//pEdit->SetImageNormalBody	(OBJECT_IMAGE_FILE0, 384, 316, 388, 332);
		//pEdit->SetImageNormalTail	(OBJECT_IMAGE_FILE0, 388, 316, 392, 332);
		//pEdit->SetImageFocusHead	(OBJECT_IMAGE_FILE0, 380, 316, 384, 332);
		//pEdit->SetImageFocusBody	(OBJECT_IMAGE_FILE0, 384, 316, 388, 332);
		//pEdit->SetImageFocusTail	(OBJECT_IMAGE_FILE0, 388, 316, 392, 332);
		pEdit->SetFontColor(RGBA(143, 99, 99, 255));
		pEdit->SetNumber();
		pEdit->SetMinValue(0);
		pEdit->SetMaxValue(999999999);
		pEdit->SetFormat(DT_RIGHT | DT_VCENTER);
		pEdit->Hide();
		m_pEditPoint = pEdit;

		//WIID_PC_TRADE_POINT_UNIT
		pStatic = new SPWindowStatic(WIID_PC_TRADE_POINT_UNIT, 0, 0, 1, 1, pSkin);
		pStatic->SetFormat(DT_LEFT | DT_VCENTER);
		//pStatic->SetMargin(2, 1);
		pStatic->SetFontColor(RGBA(143, 99, 99, 255));
		pStatic->Hide();
		
		//WIID_PC_TRADE_POINTIMG
		pStatic = new SPWindowStatic(WIID_PC_TRADE_POINTIMG, 348-iCenterX, iImgStartY-iCenterY, 15, 11, pCenter);
		pStatic->SetImage(OBJECT_IMAGE_FILE0, 492, 97);
		pStatic->Hide();
		
		//WIID_PC_TRADE_POINTDIS
		pStatic = new SPWindowStatic(WIID_PC_TRADE_CASHDIS, 210-iCenterX, iEditStartY-iCenterY, 156, 16, pCenter);
		pStatic->SetImage(OBJECT_IMAGE_FILE0, 168, 308);
		pStatic->SetSrcSize(3, 3);
		pStatic->Hide();
		m_pEditPointDis = pStatic;

		iEditStartY += 15;
		iImgStartY += 11;
	//}			

	//if(SPLocalizeManager::GetInstance()->GetCashTradeType() > CASH_TRADE_NULL) {
		iEditStartY += 4;
		iImgStartY += iImgInterval;

		//WIID_PC_TRADE_PC_FEE_TEXT
		pStatic = new SPWindowStatic(WIID_PC_TRADE_PC_FEE_TEXT, 6-iCenterX, iEditStartY-iCenterY, 44, 12, pCenter);
		pStatic->SetImage(OBJECT_IMAGE_FILE0, 43, 449);
		pStatic->Hide();
		m_pPcFeeText = pStatic;

		//WIID_PC_TRADE_PC_FEE_BACK
		pStatic = new SPWindowStatic(WIID_PC_TRADE_PC_FEE_BACK, 55-iCenterX, iEditStartY - iCenterY, 92, 15, pCenter);
		pStatic->SetWindowType( WND_TYPE_LEFTBASE );
		pStatic->SetImageNormalHead(OBJECT_IMAGE_FILE0, 380, 284, 384, 299);
		pStatic->SetImageNormalBody(OBJECT_IMAGE_FILE0, 384, 284, 388, 299);
		pStatic->SetImageNormalTail(OBJECT_IMAGE_FILE0, 388, 284, 392, 299);
		pStatic->Hide();
		m_pPcFeeBack = pStatic;
		
		//WIID_PC_TRADE_PC_FEE
		pStatic = new SPWindowStatic(WIID_PC_TRADE_PC_FEE, 63-iCenterX, iEditStartY - iCenterY, 76, 15, pCenter);
		pStatic->SetFormat(DT_RIGHT | DT_VCENTER);		
		pStatic->SetWindowText("PC Fee");
		pStatic->SetFontColor(RGBA(143, 99, 99, 255));
		pStatic->Hide();
		m_pPcFee = pStatic;
		
		//WIID_PC_TRADE_PC_FEEIMG
		pStatic = new SPWindowStatic(WIID_PC_TRADE_PC_FEEIMG, 155-iCenterX, iImgStartY - iCenterY, 15, 11, pCenter);
		pStatic->SetImage(OBJECT_IMAGE_FILE0, 492, 97);
		pStatic->Hide();
		m_pPcFeeImg = pStatic;
		
		//WIID_PC_TRADE_FEE_TEXT
		pStatic = new SPWindowStatic(WIID_PC_TRADE_FEE_TEXT, 199-iCenterX, iImgStartY-iCenterY, 44, 12, pCenter);
		pStatic->SetImage(OBJECT_IMAGE_FILE0, 43, 449);
		pStatic->Hide();
		m_pFeeText = pStatic;

		//WIID_PC_TRADE_FEE_BACK
		pStatic = new SPWindowStatic(WIID_PC_TRADE_FEE_BACK, 248-iCenterX, iEditStartY - iCenterY, 92, 15, pCenter);		
		pStatic->SetWindowType( WND_TYPE_LEFTBASE );
		pStatic->SetImageNormalHead(OBJECT_IMAGE_FILE0, 380, 284, 384, 299);
		pStatic->SetImageNormalBody(OBJECT_IMAGE_FILE0, 384, 284, 388, 299);
		pStatic->SetImageNormalTail(OBJECT_IMAGE_FILE0, 388, 284, 392, 299);
		pStatic->Hide();
		m_pFeeBack = pStatic;
		
		//WIID_PC_TRADE_FEE
		pStatic = new SPWindowStatic(WIID_PC_TRADE_FEE, 256-iCenterX, iEditStartY - iCenterY, 76, 15, pCenter);
		pStatic->SetFormat(DT_RIGHT | DT_VCENTER);		
		pStatic->SetWindowText("123,456,789");
		pStatic->SetFontColor(RGBA(143, 99, 99, 255));
		pStatic->Hide();
		m_pFee = pStatic;

		//WIID_PC_TRADE_FEEIMG
		pStatic = new SPWindowStatic(WIID_PC_TRADE_FEEIMG, 348-iCenterX, iImgStartY-iCenterY, 15, 11, pCenter);
		pStatic->SetImage(OBJECT_IMAGE_FILE0, 492, 97);
		pStatic->Hide();
		m_pFeeImg = pStatic;

		iEditStartY += 15;
		iImgStartY += 11;
	//}			

	//// VLine2
	//pStatic = new SPWindowStatic(WIID_PC_TRADE_VLINE2, 197-iX, 56-iY, 1, 204, pSub_Bottom);
	//pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 268, 19);
	//pStatic->SetSrcSize(1, 2);
	//pStatic->Show();

	iEditStartY += 4;

	//WIID_PC_TRADE_HELP
	pStatic = new SPWindowStatic(WIID_PC_TRADE_HELP, 10-iCenterX, iEditStartY-iCenterY, 364, 12, pCenter);
	pStatic->SetFormat(DT_CENTER | DT_VCENTER);	
	pStatic->SetFontColor(RGBA(255, 0, 0, 255));
	if(g_pResourceManager->GetGlobalString(9001015)) {
		pStatic->SetWindowText(g_pResourceManager->GetGlobalString(9001015));
	}	
	pStatic->Show();
	m_pHelp = pStatic;

	iEditStartY += 12;
	iEditStartY += 4;

	//WIID_PC_TRADE_ACCEPT
	pButton = new SPWindowButton(WIID_PC_TRADE_ACCEPT, 120-iCenterX, iEditStartY - iCenterY, 68, 17, pCenter);
	pButton->SetImage		(BUTTON_IMAGE_FILE0, 346,	289); //일반
	pButton->SetImageHit	(BUTTON_IMAGE_FILE0, 346,	307); //on	
	pButton->SetImagePush	(BUTTON_IMAGE_FILE0, 346,	325); //push
	pButton->SetImageDisable(BUTTON_IMAGE_FILE0, 346,	343); //disable
	pButton->Show();
	m_pAccept = pButton;

	//WIID_PC_TRADE_CANCEL
	pButton = new SPWindowButton(WIID_PC_TRADE_CANCEL, 196-iCenterX, iEditStartY - iCenterY, 68, 17, pCenter);
	pButton->SetImage		(BUTTON_IMAGE_FILE0, 415,	289); //일반
	pButton->SetImageHit	(BUTTON_IMAGE_FILE0, 415,	307); //on	
	pButton->SetImagePush	(BUTTON_IMAGE_FILE0, 415,	325); //push
	pButton->SetImageDisable(BUTTON_IMAGE_FILE0, 415,	343); //disable
	pButton->Show();

	//// Sub_HLine2
	//pStatic = new SPWindowStatic(WIID_PC_TRADE_SUB_HLINE2, 2-iCenterX, 264-iCenterY, 391, 1, pCenter);
	//pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 7, 92);
	//pStatic->SetSrcSize(2, 1);
	//pStatic->Show();

	//// Bottom
	//pStatic = new SPWindowStatic(WIID_PC_TRADE_BOTTOM, 0, 288, 395, 4, this);
	////pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 1, 90);
	//pStatic->Show();

}


std::string SPWindowPcTrade::GetUnitString(int iUnit /*= 0*/, TRADE_FEE iMode /*= TRADE_FEE_NULL*/)
{
	static std::string s_strBuf;

	s_strBuf.clear();
	char szTemp[256];
	sprintf(szTemp, "%d", iUnit);
	int iLength = (int)strlen(szTemp);
	int i, iCount = 0;

	for(i = iLength - 1; i >= 1; i--) {
		s_strBuf.insert((std::string::size_type)0, 1, szTemp[i]);
		iCount++;
	}

	if(iMode == TRADE_FEE_CASH) {
		m_iCashUnitCount = iCount;
	}
	else if(iMode == TRADE_FEE_POINT) {
		m_iPointUnitCount = iCount;
	}

	return s_strBuf.c_str();
}


void SPWindowPcTrade::ArrangeControl()
{
	SPWindow* pCenter = NULL;
	SPWindow* pStatic = NULL;	

	m_strCashUnit = GetUnitString(SPLocalizeManager::GetInstance()->GetCashTradeUnit(), TRADE_FEE_CASH);
	m_strPointUnit = GetUnitString(SPLocalizeManager::GetInstance()->GetPointTradeUnit(), TRADE_FEE_POINT);
	std::string strZeroUnit = GetUnitString(0);
	
	int iCenterX = 2;
	int iCenterY = 27;
	int iVLineHeight = 0;	
	pCenter = Find(WIID_PC_TRADE_CENTER, true);
	if(pCenter) {
		if(SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_ALL) {		
			pCenter->AdjustCoord(iCenterX, iCenterY, 380, 292);
			iVLineHeight = 222;
		}
		else if(SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_CASH || 
			SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_POINT)
		{
			pCenter->AdjustCoord(iCenterX, iCenterY, 380, 277);
			iVLineHeight = 207;
		}
		else {
			pCenter->AdjustCoord(iCenterX, iCenterY, 380, 239);
			iVLineHeight = 188;
		}
		pCenter->Show();

		// WIID_PC_TRADE_VLINE1
		pStatic = Find(WIID_PC_TRADE_VLINE1, true);
		if(pStatic) {
			pStatic->AdjustCoord(191-iCenterX, 35-iCenterY, 2, iVLineHeight);
			pStatic->Show();
		}
	}
	else {
		return;
	}
	
	int iEditStartY = 208 + 15;
	int iImgStartY = 212 + 5;

	int iEditInterval = 2;
	int iImgInterval = 7;
	
	{	
		//WIID_PC_TRADE_PC_CASH_BACK
		pStatic = Find(WIID_PC_TRADE_PC_CASH_BACK, true);
		if(pStatic) {			
			pStatic->Hide();
		}		

		//WIID_PC_TRADE_PC_CASH
		if(m_pPcCash) {
			m_pPcCash->Hide();
		}		

		//WIID_PC_TRADE_PC_CASHIMG
		pStatic = Find(WIID_PC_TRADE_PC_CASHIMG, true);
		if(pStatic) {
			pStatic->Hide();
		}

		//WIID_PC_TRADE_PC_CASHDIS
		if(m_pPcCashDis) {
			m_pPcCashDis->Hide();
		}

		//WIID_PC_TRADE_CASH_BACK
		pStatic = Find(WIID_PC_TRADE_CASH_BACK, true);
		if(pStatic) {
			pStatic->Hide();
		}

		//WIID_PC_TRADE_CASH
		if(m_pEditCash) {
			m_pEditCash->Hide();
		}

		//WIID_PC_TRADE_CASH_UNIT
		pStatic = Find(WIID_PC_TRADE_CASH_UNIT, true);
		if(pStatic) {
			pStatic->Hide();
		}

		//WIID_PC_TRADE_CASHIMG
		pStatic = Find(WIID_PC_TRADE_CASHIMG, true);
		if(pStatic) {
			pStatic->Hide();
		}

		//WIID_PC_TRADE_CASHDIS	
		if(m_pEditCashDis) {
			m_pEditCashDis->Hide();
		}
	}
	if(SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_ALL || 
		SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_CASH)
	{		
		iEditStartY += iEditInterval;
		iImgStartY += iImgInterval + 3;			//Ely와 Cash 사이는 10

		//WIID_PC_TRADE_PC_CASH_BACK
		pStatic = Find(WIID_PC_TRADE_PC_CASH_BACK, true);
		if(pStatic) {
			pStatic->AdjustCoord(18-iCenterX, iEditStartY - iCenterY, 129, 15);
			pStatic->Show();
		}		

		//WIID_PC_TRADE_PC_CASH
		if(m_pPcCash) {
			m_pPcCash->AdjustCoord(24-iCenterX, iEditStartY - iCenterY, 118, 15);
			m_pPcCash->Show();
		}		

		//WIID_PC_TRADE_PC_CASHIMG
		pStatic = Find(WIID_PC_TRADE_PC_CASHIMG, true);
		if(pStatic) {
			pStatic->AdjustCoord(154-iCenterX, iImgStartY - iCenterY, 15, 11);
			pStatic->Show();
		}

		//WIID_PC_TRADE_PC_CASHDIS
		if(m_pPcCashDis) {
			m_pPcCashDis->AdjustCoord(18-iCenterX, iEditStartY - iCenterY, 156, 16);
			m_pPcCashDis->Hide();
		}

		//WIID_PC_TRADE_CASH_BACK
		pStatic = Find(WIID_PC_TRADE_CASH_BACK, true);
		if(pStatic) {
			pStatic->AdjustCoord(210-iCenterX, iEditStartY-iCenterY, 129, 15);
			pStatic->Show();
		}

		//WIID_PC_TRADE_CASH_UNIT
		pStatic = Find(WIID_PC_TRADE_CASH_UNIT, true);
		int iCashUnitWidth = 0;
		int iCashUnitStartX = 0;
		if(m_iCashUnitCount) {
			iCashUnitWidth = 6 * m_iCashUnitCount;
			iCashUnitStartX = 129 - 4 - iCashUnitWidth;

			if(pStatic) {
				pStatic->AdjustCoord(iCashUnitStartX, 0, iCashUnitWidth, 15);
				pStatic->SetWindowText(m_strCashUnit.c_str());
				pStatic->Show();
			}
		}

		//WIID_PC_TRADE_CASH
		if(m_pEditCash) {
			//m_pEditCash->AdjustCoord(210-iCenterX, iEditStartY-iCenterY, 129, 15);
			m_pEditCash->AdjustCoord(4, 0, 121 - iCashUnitWidth, 15);
			m_pEditCash->SetWindowText("0");
			m_pEditCash->Show();
			m_pEditCash->SetEnable(true);
		}

		//WIID_PC_TRADE_CASHIMG
		pStatic = Find(WIID_PC_TRADE_CASHIMG, true);
		if(pStatic) {
			pStatic->AdjustCoord(348-iCenterX, iImgStartY-iCenterY, 15, 11);
			pStatic->Show();
		}

		//WIID_PC_TRADE_CASHDIS	
		if(m_pEditCashDis) {
			m_pEditCashDis->AdjustCoord(210-iCenterX, iEditStartY-iCenterY, 156, 16);
			m_pEditCashDis->Hide();
		}

		iEditStartY += 15;
		iImgStartY += 11;
	}
	
	{
		//WIID_PC_TRADE_PC_POINT_BACK
		pStatic = Find(WIID_PC_TRADE_PC_POINT_BACK, true);
		if(pStatic) {
			pStatic->Hide();
		}		

		//WIID_PC_TRADE_PC_POINT
		if(m_pPcPoint) {
			m_pPcPoint->Hide();
		}		

		//WIID_PC_TRADE_PC_POINTIMG
		pStatic = Find(WIID_PC_TRADE_PC_POINTIMG, true);
		if(pStatic) {
			pStatic->Hide();
		}

		//WIID_PC_TRADE_PC_POINTDIS
		if(m_pPcPointDis) {
			m_pPcPointDis->Hide();
		}

		//WIID_PC_TRADE_POINT_BACK
		pStatic = Find(WIID_PC_TRADE_POINT_BACK, true);
		if(pStatic) {
			pStatic->Hide();
		}

		//WIID_PC_TRADE_POINT
		if(m_pEditPoint) {
			m_pEditPoint->Hide();
		}

		//WIID_PC_TRADE_POINT_UNIT
		pStatic = Find(WIID_PC_TRADE_POINT_UNIT, true);
		if(pStatic) {
			pStatic->Hide();
		}

		//WIID_PC_TRADE_POINTIMG
		pStatic = Find(WIID_PC_TRADE_POINTIMG, true);
		if(pStatic) {
			pStatic->Hide();
		}

		//WIID_PC_TRADE_POINTDIS
		if(m_pEditPointDis) {
			m_pEditPointDis->Hide();
		}
	}
	if(SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_ALL || 
		SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_POINT)
	{
		iEditStartY += iEditInterval;
		iImgStartY += iImgInterval;

		if(SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_POINT)
			iImgStartY += 3;

		//WIID_PC_TRADE_PC_POINT_BACK
		pStatic = Find(WIID_PC_TRADE_PC_POINT_BACK, true);
		if(pStatic) {
			pStatic->AdjustCoord(18-iCenterX, iEditStartY - iCenterY, 129, 15);
			pStatic->Show();
		}		

		//WIID_PC_TRADE_PC_POINT
		if(m_pPcPoint) {
			m_pPcPoint->AdjustCoord(24-iCenterX, iEditStartY - iCenterY, 118, 15);
			m_pPcPoint->Show();
		}		

		//WIID_PC_TRADE_PC_POINTIMG
		pStatic = Find(WIID_PC_TRADE_PC_POINTIMG, true);
		if(pStatic) {
			pStatic->AdjustCoord(154-iCenterX, iImgStartY - iCenterY, 15, 11);
			pStatic->Show();
		}

		//WIID_PC_TRADE_PC_POINTDIS
		if(m_pPcPointDis) {
			m_pPcPointDis->AdjustCoord(18-iCenterX, iEditStartY - iCenterY, 156, 16);
			m_pPcPointDis->Hide();
		}

		//WIID_PC_TRADE_POINT_BACK
		pStatic = Find(WIID_PC_TRADE_POINT_BACK, true);
		if(pStatic) {
			pStatic->AdjustCoord(210-iCenterX, iEditStartY-iCenterY, 129, 15);
			pStatic->Show();
		}

		//WIID_PC_TRADE_POINT_UNIT
		pStatic = Find(WIID_PC_TRADE_POINT_UNIT, true);		
		int iPointUnitWidth = 0;
		int iPointUnitStartX = 0;
		if(m_iPointUnitCount) {
			iPointUnitWidth = 6 * m_iPointUnitCount;
			iPointUnitStartX = 129 - 4 - iPointUnitWidth;

			if(pStatic) {
				pStatic->AdjustCoord(iPointUnitStartX, 0, iPointUnitWidth, 15);
				pStatic->SetWindowText(m_strPointUnit.c_str());
				pStatic->Show();
			}
		}

		//WIID_PC_TRADE_POINT
		if(m_pEditPoint) {
			//m_pEditPoint->AdjustCoord(210-iCenterX, iEditStartY-iCenterY, 129, 15);	
			m_pEditPoint->AdjustCoord(4, 0, 121 - iPointUnitWidth, 15);
			m_pEditPoint->SetWindowText("0");
			m_pEditPoint->Show();
			m_pEditPoint->SetEnable(true);
		}

		//WIID_PC_TRADE_POINTIMG
		pStatic = Find(WIID_PC_TRADE_POINTIMG, true);
		if(pStatic) {
			pStatic->AdjustCoord(348-iCenterX, iImgStartY-iCenterY, 15, 11);
			pStatic->Show();
		}

		//WIID_PC_TRADE_POINTDIS
		if(m_pEditPointDis) {
			m_pEditPointDis->AdjustCoord(210-iCenterX, iEditStartY-iCenterY, 156, 16);
			m_pEditPointDis->Hide();
		}

		iEditStartY += 15;
		iImgStartY += 11;
	}
	
	{
		//WIID_PC_TRADE_PC_FEE_TEXT
		if(m_pPcFeeText) {
			m_pPcFeeText->Hide();
		}

		//WIID_PC_TRADE_PC_FEE_BACK
		if(m_pPcFeeBack) {
			m_pPcFeeBack->Hide();
		}

		//WIID_PC_TRADE_PC_FEE
		if(m_pPcFee) {
			m_pPcFee->Hide();
		}

		//WIID_PC_TRADE_PC_FEEIMG
		if(m_pPcFeeImg) {
			m_pPcFeeImg->Hide();
		}

		//WIID_PC_TRADE_FEE_TEXT
		if(m_pFeeText) {
			m_pFeeText->Hide();
		}

		//WIID_PC_TRADE_FEE_BACK
		if(m_pFeeBack) {
			m_pFeeBack->Hide();
		}

		//WIID_PC_TRADE_FEE
		if(m_pFee) {
			m_pFee->Hide();
		}

		//WIID_PC_TRADE_FEEIMG
		if(m_pFeeImg) {
			m_pFeeImg->Hide();
		}
	}
	if(SPLocalizeManager::GetInstance()->GetCashTradeType() > CASH_TRADE_NULL) {
		iEditStartY += 4;
		iImgStartY += iImgInterval;

		//WIID_PC_TRADE_PC_FEE_TEXT
		if(m_pPcFeeText) {
			m_pPcFeeText->AdjustCoord(6-iCenterX, iEditStartY-iCenterY, 44, 12);
			m_pPcFeeText->Show();
		}

		//WIID_PC_TRADE_PC_FEE_BACK
		if(m_pPcFeeBack) {
			m_pPcFeeBack->AdjustCoord(55-iCenterX, iEditStartY - iCenterY, 92, 15);
			m_pPcFeeBack->Show();
		}

		//WIID_PC_TRADE_PC_FEE
		if(m_pPcFee) {
			m_pPcFee->AdjustCoord(63-iCenterX, iEditStartY - iCenterY, 76, 15);
			m_pPcFee->Show();
		}

		//WIID_PC_TRADE_PC_FEEIMG
		if(m_pPcFeeImg) {
			m_pPcFeeImg->AdjustCoord(155-iCenterX, iImgStartY - iCenterY, 15, 11);
			m_pPcFeeImg->Show();
		}

		//WIID_PC_TRADE_FEE_TEXT
		if(m_pFeeText) {
			m_pFeeText->AdjustCoord(199-iCenterX, iImgStartY-iCenterY, 44, 12);
			m_pFeeText->Show();
		}

		//WIID_PC_TRADE_FEE_BACK
		if(m_pFeeBack) {
			m_pFeeBack->AdjustCoord(248-iCenterX, iEditStartY - iCenterY, 92, 15);
			m_pFeeBack->Show();
		}

		//WIID_PC_TRADE_FEE
		if(m_pFee) {
			m_pFee->AdjustCoord(256-iCenterX, iEditStartY - iCenterY, 76, 15);
			m_pFee->Show();
		}

		//WIID_PC_TRADE_FEEIMG
		if(m_pFeeImg) {
			m_pFeeImg->AdjustCoord(348-iCenterX, iImgStartY-iCenterY, 15, 11);
			m_pFeeImg->Show();
		}

		iEditStartY += 15;
		iImgStartY += 11;
	}	

	iEditStartY += 4;

	//WIID_PC_TRADE_HELP
	if(m_pHelp) {
		m_pHelp->AdjustCoord(10-iCenterX, iEditStartY-iCenterY, 364, 12);
		m_pHelp->Show();
	}

	iEditStartY += 12;
	iEditStartY += 4;

	//WIID_PC_TRADE_ACCEPT
	if(m_pAccept) {
		m_pAccept->AdjustCoord(120-iCenterX, iEditStartY - iCenterY, 68, 17);
		m_pAccept->Show();
	}

	//WIID_PC_TRADE_CANCEL
	pStatic = Find(WIID_PC_TRADE_CANCEL, true);
	if(pStatic) {
		pStatic->AdjustCoord(196-iCenterX, iEditStartY - iCenterY, 68, 17);
		pStatic->Show();
	}
}


void SPWindowPcTrade::Clean()
{	
	DeleteAllPcItem();
	SAFE_RELEASE(m_pDisable);
	SAFE_RELEASE(m_pBaseSkin);
	SPWindow::Clean();
}


void SPWindowPcTrade::Process(float fTime)
{	
	if( !m_bShow )
		return;
	
	//if(CheckInterval() == false) {
	//	OnHide(0, 0);
	//}

	SetWindowBase();

	//현재 양쪽의 거래 창에 무언가가 올라온 상태인지 확인한다 
	if(IsEmptyItem()) {		
		//수락버튼 비활성화
		m_pAccept->SetEnable(false);
		m_pAccept->SPSendMessage(SPIM_BTN_SELECT, false);
		// 내 거래상태를 대기로 설정합니다.
		//m_pStatus_Wait->Show();
		m_pNameAccept->Hide();
		m_pNameWait->Show();		
		m_pStatus_Accept->Hide();
		// 상대 거래상태를 대기로 설정합니다.
		//m_pPcStatus_Wait->Show();
		m_pPcNameAccept->Hide();
		m_pPcNameWait->Show();
		m_pPcStatus_Accept->Hide();
		//m_pPCAccept->SPSendMessage(SPIM_SET_IMAGECLEAR);
		//m_pPCAccept->SetImage(ACCEPT_IMAGE_FILE.c_str(), m_iSrcPcAcceptX[PC_TRADE_ACCEPT_DISABLE], m_iSrcPcAcceptY);
	}

	//트레이드가 가능한 상황인지를 검사한다.
	if(IsTradeEnable() == false) {
		SendTradeCancel();
		//OnHide(w0, 0);
	}

	//[2006/3/13]
	if(m_bChanged /*&& CheckCashTrade()*/) {
		m_fChangedAccumulate += fTime;
		if( m_fChangedAccumulate > m_fChangedDeley) {
			m_fChangedAccumulate -= m_fChangedDeley;			
			
			//if(CheckCashTrade() == false)
			//	return;

			m_pAccept->SetEnable(true);
			// 내 거래상태를 대기로 설정합니다.
			m_pNameAccept->Hide();
			m_pNameWait->Show();
			//m_pStatus_Wait->Show();
			m_pStatus_Accept->Hide();
			m_bChanged = false;
		}
	}

	SPWindow::Process(fTime);	
}


void SPWindowPcTrade::Render(float fTime)
{
	////
	//if(IsShow()) {
	//	if(m_pTexture){
	//		m_pTexture->RenderTexture(&m_rtDestCenter, &m_rtSrcCenter);
	//	}
	//	//SetFormat(DT_TOP | DT_VCENTER);
	//}
	if( !m_bShow )
		return;

	g_pVideo->Flush();
	
	int i = 0;
	if(m_pBaseSkin) {
		for(i = WINDOW_BASE_TOP_LEFT; i < WINDOW_BASE_MAX; i++) {
			m_pBaseSkin->RenderTexture(&m_rtBaseDest[i], &m_rtBaseSrc[i]);
		}
	}

	SPWindow::Render(fTime);

	for(i = 0; i < MAX_PC_TRADE_COUNT; i++) {
		//Icon Disable 표현
		if(m_pDisable) {
			RECT rtDest;
			m_pItemIcon[i]->GetWindowRect(rtDest);
			if(m_bItemAble[i] == false) {		
				m_pDisable->RenderTexture(&rtDest, &m_rtSrcDisable);
			}
			if(m_iItemMark[i] > ICON_MARK_NULL) {
				m_pDisable->RenderTexture(&rtDest, &m_rtMarkSrc[m_iItemMark[i] - 1]);
			}

			RECT rtPcDest;
			m_pPCItemIcon[i]->GetWindowRect(rtPcDest);
			if(m_bPcItemAble[i] == false) {				
				m_pDisable->RenderTexture(&rtPcDest, &m_rtSrcDisable);
			}
			if(m_iPcItemMark[i] > ICON_MARK_NULL) {
				m_pDisable->RenderTexture(&rtPcDest, &m_rtMarkSrc[m_iPcItemMark[i] - 1]);
			}
		}

		//Stack Count Local 표현
		if(m_iStack[i] > 9) {
			RECT rtTenNumTarget, rtNumTarget, rtTenNumSrc, rtNumSrc;
			int iTenStack = m_iStack[i] / 10;
			int iOneStack = m_iStack[i] % 10;
			
			m_pItemIcon[i]->GetWindowRect(m_rtStack[i]);
			SetRect(&rtTenNumTarget, m_rtStack[i].left, m_rtStack[i].bottom - 10, m_rtStack[i].left + 9, m_rtStack[i].bottom);
			SetRect(&rtNumTarget, rtTenNumTarget.right - 2, m_rtStack[i].bottom - 10, rtTenNumTarget.right + 9 - 2, m_rtStack[i].bottom);

			rtTenNumSrc.left = iTenStack * 9;
			rtTenNumSrc.right = rtTenNumSrc.left + 9;
			rtTenNumSrc.top = 0;
			rtTenNumSrc.bottom = 10;

			rtNumSrc.left = iOneStack * 9;
			rtNumSrc.right = rtNumSrc.left + 9;
			rtNumSrc.top = 0;
			rtNumSrc.bottom = 10;

			g_pItemCluster->GetNumTexture()->RenderTexture( &rtTenNumTarget , &rtTenNumSrc);
			g_pItemCluster->GetNumTexture()->RenderTexture( &rtNumTarget , &rtNumSrc);
		}
		else if(m_iStack[i] > 1) {
			int iOneStack = m_iStack[i] % 10;
			m_pItemIcon[i]->GetWindowRect(m_rtStack[i]);
			RECT rtNumTarget, rtNumSrc;
			SetRect(&rtNumTarget, m_rtStack[i].left, m_rtStack[i].bottom - 10, m_rtStack[i].left + 9, m_rtStack[i].bottom);

			rtNumSrc.left = iOneStack * 9;
			rtNumSrc.right = rtNumSrc.left + 9;
			rtNumSrc.top = 0;
			rtNumSrc.bottom = 10;

			g_pItemCluster->GetNumTexture()->RenderTexture( &rtNumTarget , &rtNumSrc);
		}	
		
		////Stack Count 상대 PC 표현
		if(m_iPcStack[i] > 9) {
			RECT rtTenNumTarget, rtNumTarget, rtTenNumSrc, rtNumSrc;
			int iTenStack = m_iPcStack[i] / 10;
			int iOneStack = m_iPcStack[i] % 10;
			
			m_pPCItemIcon[i]->GetWindowRect(m_rtPcStack[i]);
			SetRect(&rtTenNumTarget, m_rtPcStack[i].left, m_rtPcStack[i].bottom - 10, m_rtPcStack[i].left + 9, m_rtPcStack[i].bottom);
			SetRect(&rtNumTarget, rtTenNumTarget.right - 2, m_rtPcStack[i].bottom - 10, rtTenNumTarget.right + 9 - 2, m_rtPcStack[i].bottom);

			rtTenNumSrc.left = iTenStack * 9;
			rtTenNumSrc.right = rtTenNumSrc.left + 9;
			rtTenNumSrc.top = 0;
			rtTenNumSrc.bottom = 10;

			rtNumSrc.left = iOneStack * 9;
			rtNumSrc.right = rtNumSrc.left + 9;
			rtNumSrc.top = 0;
			rtNumSrc.bottom = 10;

			g_pItemCluster->GetNumTexture()->RenderTexture( &rtTenNumTarget , &rtTenNumSrc);
			g_pItemCluster->GetNumTexture()->RenderTexture( &rtNumTarget , &rtNumSrc);
		}
		else if(m_iPcStack[i] > 1) {
			int iOneStack = m_iPcStack[i] % 10;
			RECT rtNumTarget, rtNumSrc;
			m_pPCItemIcon[i]->GetWindowRect(m_rtPcStack[i]);
			SetRect(&rtNumTarget, m_rtPcStack[i].left, m_rtPcStack[i].bottom - 10, m_rtPcStack[i].left + 9, m_rtPcStack[i].bottom);

			rtNumSrc.left = iOneStack * 9;
			rtNumSrc.right = rtNumSrc.left + 9;
			rtNumSrc.top = 0;
			rtNumSrc.bottom = 10;

			g_pItemCluster->GetNumTexture()->RenderTexture( &rtNumTarget , &rtNumSrc);
		}
	}

////
//#ifdef _DEBUG
//	g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));	
//	RECT rtRect;	
//	for(int i = 0; i < MAX_PC_TRADE_COUNT; i++) {
//		m_pItemText[i]->GetWindowRect(rtRect);
//		g_pVideo->GetSysLine()->DrawRect(rtRect);
//	}
//	g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
//#endif
}


void SPWindowPcTrade::Show(bool bWithChild)
{	
	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return ;

	SetWindowBase();	
	
	//윈도우 열리기전에 컨트롤들은 초기화 시킨다 
	ClearAllPcItem();
	
	for(int i = 0; i < MAX_PC_TRADE_COUNT ; i++) {
		ItemClear(i, 0);
		m_bItemAble[i] = true;
		m_iItemMark[i] = ICON_MARK_NULL;
		m_bPcItemAble[i] = true;
		m_iPcItemMark[i] = ICON_MARK_NULL;
	}
	
	ClearAllPcItem();							//상대방의 아이템 리스트 초기화
	m_iPcEly = 0;
	m_pPcEly->SetWindowText(NULL);
	m_iPcCash = 0;
	m_pPcCash->SetWindowText(NULL);
	m_iPcPoint = 0;
	m_pPcPoint->SetWindowText(NULL);
	m_iPcFee = 0;
	m_pPcFee->SetWindowText(NULL);
	
	m_iTradeEly = 0;
	m_pEditEly->SetWindowText(NULL);
	m_iTradeCash = 0;
	m_pEditCash->SetWindowText("0");
	m_iTradePoint = 0;
	m_pEditPoint->SetWindowText("0");
	m_iTradeFee = 0;
	m_pFee->SetWindowText(NULL);

	m_pTargetPlayer = (SPPlayer*)g_pGOBManager->Find(m_uiPlayerGUID);
	if(m_pTargetPlayer) {
		m_pNameWait->SetWindowText(g_pGOBManager->GetLocalPlayer()->GetStatus()->GetGOBName());
		m_pNameAccept->SetWindowText(g_pGOBManager->GetLocalPlayer()->GetStatus()->GetGOBName());
		m_pPcNameWait->SetWindowText(m_pTargetPlayer->GetStatus()->GetGOBName());
		m_pPcNameAccept->SetWindowText(m_pTargetPlayer->GetStatus()->GetGOBName());
	}

	////[2008/3/18]-jinhee 임시
	//if(g_pGOBManager->GetLocalPlayer()) {
	//	int iCurCash = g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValue(STATUS_CASH);
	//	int iCurPoint = g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValue(STATUS_MILE);
	//	if(m_pHelp) {
	//		char szTemp[64];
	//		ZeroMemory(szTemp, 64);
	//		sprintf(szTemp, "Cash[%d] Point[%d]", iCurCash, iCurPoint);
	//		m_pHelp->SetWindowText(szTemp);
	//	}
	//}

	m_bSendCancel = false;	
	m_fStartPosX = g_pGOBManager->GetLocalPlayer()->GetPosX();
	m_fStartPosY = g_pGOBManager->GetLocalPlayer()->GetPosY();

	//수락버튼 비활성화
	m_pAccept->SetEnable(false);
	m_pAccept->SPSendMessage(SPIM_BTN_SELECT, false);
	// 내 거래상태를 대기로 설정합니다.
	//m_pStatus_Wait->Show();
	m_pNameAccept->Hide();
	m_pNameWait->Show();
	m_pStatus_Accept->Hide();
	// 상대 거래상태를 대기로 설정합니다.
	//m_pPcStatus_Wait->Show();
	m_pPcNameAccept->Hide();
	m_pPcNameWait->Show();
	m_pPcStatus_Accept->Hide();
	//m_pPCAccept->SPSendMessage(SPIM_SET_IMAGECLEAR);
	//m_pPCAccept->SetImage(ACCEPT_IMAGE_FILE.c_str(), m_iSrcPcAcceptX[PC_TRADE_ACCEPT_DISABLE], m_iSrcPcAcceptY);

	m_bChanged = false;
	m_fChangedAccumulate = 0.0f;

	m_iTradeMode = TRADE_MODE_NULL;
	
	SPWindow::Show(bWithChild);
	ArrangeControl();
	RefreshRelationCoord();
	
	SetFeeShow(TRADE_USER_PC, false);
	SetFeeShow(TRADE_USER_LOCAL, false);

	m_pPcCashDis->Hide();
	m_pPcPointDis->Hide();

	SetEnableIndun();
	
	Refresh();
}

void SPWindowPcTrade::Refresh()
{

}



//////////////////////////////////////////////////////////////////////////
////
//bool SPWindowPcTrade::CheckInterval()
//{
//	// 타겟 플레이어 가져오기...
//	if(g_pGOBManager->GetLocalPlayer()) {
//		m_pTargetPlayer = (SPPlayer*)g_pGOBManager->Find(m_uiPlayerGUID);
//		if( m_pTargetPlayer == NULL ) {
//			//OnHide(0, 0);
//			return false;
//		}
//
//		float fLocalX = g_pGOBManager->GetLocalPlayer()->GetPosX();
//		float fLocalY = g_pGOBManager->GetLocalPlayer()->GetPosY();
//		float fPcX = m_pTargetPlayer->GetPosX();
//		float fPcY = m_pTargetPlayer->GetPosY();
//
//		if((fPcX > fLocalX - ENABLE_X && fPcX < fLocalX + ENABLE_X) && 
//			(fPcY > fLocalY - ENABLE_Y && fPcY < fLocalY + ENABLE_Y)) 
//		{
//			return true;
//		}
//	}	
//
//	//m_szBuf[0] = '\0';
//	//m_pTargetPlayer->SPGOBPerformMessage(SPGM_GETGOBNAME, (LPARAM)m_szBuf);
//	return false;
//}


void SPWindowPcTrade::CloseTrade()
{
	//실제 데이터 클리어
	SPPlayerInvenArchive* pInven = NULL;
	if(g_pGOBManager->GetLocalPlayer())
		pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	else 
		return ;

	pInven->ClearAllTradeItem();	

	for(int i = 0; i < MAX_PC_TRADE_COUNT ; i++) {	//아이템 컨트롤러(인터페이스) 클리어
		ItemClear(i, 0);
		m_bItemAble[i] = true;
		m_iItemMark[i] = ICON_MARK_NULL;
		m_bPcItemAble[i] = true;
		m_iPcItemMark[i] = ICON_MARK_NULL;
	}	

	ClearAllPcItem();								//상대방의 아이템 클리어
	m_pEditEly->SetNULL();
	m_pEditEly->SetWindowText(NULL);
	m_iPcEly = 0;
	m_pPcEly->SetWindowText(NULL);

	m_pNameWait->SetWindowText(NULL);
	m_pNameAccept->SetWindowText(NULL);
	m_pPcNameWait->SetWindowText(NULL);
	m_pPcNameAccept->SetWindowText(NULL);

	//수락버튼 비활성화
	m_pAccept->SPSendMessage(SPIM_BTN_SELECT, false);
	m_pAccept->SetEnable(false);
	// 내 거래상태를 대기로 설정합니다.
	//m_pStatus_Wait->Show();
	m_pNameAccept->Hide();
	m_pNameWait->Show();
	m_pStatus_Accept->Hide();
	// 상대 거래상태를 대기로 설정합니다.
	//m_pPcStatus_Wait->Show();
	m_pPcNameAccept->Hide();
	m_pPcNameWait->Show();
	m_pPcStatus_Accept->Hide();
	//m_pPCAccept->SPSendMessage(SPIM_SET_IMAGECLEAR);
	//m_pPCAccept->SetImage(ACCEPT_IMAGE_FILE.c_str(), m_iSrcPcAcceptX[PC_TRADE_ACCEPT_DISABLE], m_iSrcPcAcceptY);	

	//임시...
	g_pInterfaceManager->GetInvenWindow()->Hide();
	g_pInterfaceManager->GetEquipWindow()->Hide();	

	g_pGOBManager->GetLocalPlayer()->SetTradeState(TRADE_STATE_NULL);

	m_bSendCancel = false;

	this->Hide();

	SPWindow* pTradeResult = g_pInterfaceManager->FindWindow(WIID_PC_TRADE_RESULT);
	if(pTradeResult) {
		g_pInterfaceManager->SetModalWindow(NULL);
		pTradeResult->Hide();
	}

	return;
}


bool SPWindowPcTrade::IsTradeEnable()
{
	if(m_bSendCancel)						return true;
	
	if(g_pGOBManager->GetLocalPlayer()) {
		// 타겟 플레이어 가져오기...		
		m_pTargetPlayer = (SPPlayer*)g_pGOBManager->Find(m_uiPlayerGUID);
		if( m_pTargetPlayer == NULL ) {
			//OnHide(0, 0);
			return false;
		}

		if(g_pGOBManager->GetLocalPlayer()->IsDead()) {
			return false;
		}
		
		////
		//unsigned int uiCurAction = g_pGOBManager->GetLocalPlayer()->GetCoordPhysics()->GetCurAction();
		//if( uiCurAction & ACTION_BEATTACKED ||
		//	uiCurAction & ACTION_CRITICAL_BEATTACKED ||
		//	uiCurAction & ACTION_BEATTACKED_DOWN ||
		//	uiCurAction & ACTION_GUARD_DAMAGE )
		//{
		//	return false;
		//}
		
		float fCurPosX = g_pGOBManager->GetLocalPlayer()->GetPosX();
		float fCurPosY = g_pGOBManager->GetLocalPlayer()->GetPosY();
		
		////기존의 내 위치와 틱마다 위치를 체크
		//if( (m_fStartPosX != g_pGOBManager->GetLocalPlayer()->GetPosX()) || 
		//	(m_fStartPosY != g_pGOBManager->GetLocalPlayer()->GetPosY()))
		//{
		//	return false;
		//}

		if( (fCurPosX < m_fStartPosX - 30 || m_fStartPosX + 30 < fCurPosX) ||
			(fCurPosY < m_fStartPosY - 30 || m_fStartPosY + 30 < fCurPosY))
		{
			return false;
		}
		
		////
		//if(m_pTargetPlayer->IsDead()) {
		//	return false;
		//}

		////같은 맵에 존재하는지 ?
		//if(g_pGOBManager->GetLocalPlayer()->GetCurStageID() != m_pTargetPlayer->GetCurStageID() ||
		//	g_pGOBManager->GetLocalPlayer()->GetCurMapGroupID() != m_pTargetPlayer->GetCurMapGroupID())
		//{
		//	return false;
		//}
	
		////트레이드가 가능한 거리에 있는지?
		//float fLocalX = g_pGOBManager->GetLocalPlayer()->GetPosX();
		//float fLocalY = g_pGOBManager->GetLocalPlayer()->GetPosY();
		//float fPcX = m_pTargetPlayer->GetPosX();
		//float fPcY = m_pTargetPlayer->GetPosY();
		//if((fPcX > fLocalX - (CHECK_INTERVAL_X - 20) && fPcX < fLocalX + (CHECK_INTERVAL_X - 20)) && 
		//	(fPcY > fLocalY - (CHECK_INTERVAL_Y - 20) && fPcY < fLocalY + (CHECK_INTERVAL_Y - 20)))
		//{
		//	return true;
		//}

		//return false;
		return true;
	}	

	return false;
}


bool SPWindowPcTrade::IsEmptyItem()
{
	//내 Ely 검사
	INT64 iTradeEly = 0;
	iTradeEly = _atoi64(m_pEditEly->GetWindowText());	
	if(iTradeEly > 0)			return false;
	
	//상대 Ely 검사
	if(m_iPcEly > 0)			return false;

	//내 Cash 검사
	int iTradeCash = atoi(m_pEditCash->GetWindowText());
	if(iTradeCash > 0)			return false;	

	//내 Point 검사
	int iTradePoint = atoi(m_pEditPoint->GetWindowText());
	if(iTradePoint > 0)			return false;

	if(m_iPcCash > 0)			return false;
	if(m_iPcPoint > 0)			return false;

	//내 Item리스트 검사
	SPPlayerInvenArchive* pInven = NULL;
	if(g_pGOBManager->GetLocalPlayer())
		pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	else 
		return false;

	for(int i = 0 ; i < MAX_PC_TRADE_COUNT ; i++) {
		//내 아이템 리스트에 아이템이 있는 경우
		if(pInven->GetTradeItem(i)->IsNull() == false)
			return false;
		
		//상대 아이템 리스트에 아이템이 있는 경우
		if(m_vpPCItem.at(i)->GetItemAttr() != 0 && m_vpPCItem.at(i)->GetItemStatusValue(MV_ITEM_GETUID) != 0)
			return false;
	}

	return true;
}


bool SPWindowPcTrade::CheckCashTrade()
{	
	if(SPLocalizeManager::GetInstance()->GetCashTradeType() > CASH_TRADE_NULL) {
		int iCashTradeUnit = SPLocalizeManager::GetInstance()->GetCashTradeUnit();

		int iPointTradeUnit = SPLocalizeManager::GetInstance()->GetPointTradeUnit();

		if(SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_ALL) {
			if(m_iTradePoint == 0 && (m_iTradeCash < iCashTradeUnit))
				return false;

			if(m_iTradeCash == 0 && (m_iTradePoint < iPointTradeUnit))
				return false;

			if(m_iPcPoint == 0 && (m_iPcCash < iCashTradeUnit))
				return false;

			if(m_iPcCash == 0 && (m_iPcPoint < iPointTradeUnit))
				return false;
		}
		else if(SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_CASH) {
			if(m_iTradeCash < iCashTradeUnit)
				return false;

			if(m_iPcCash < iCashTradeUnit)
				return false;
		}
		else if(SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_POINT) {
			if(m_iTradePoint < iPointTradeUnit)
				return false;

			if(m_iPcPoint < iPointTradeUnit)
				return false;
		}

		return false;

	}
	
	return true;
}


void SPWindowPcTrade::InitPcItem()
{
	for(int i = 0; i < MAX_PC_TRADE_COUNT; i++) {
		SPItem* pItem = new SPItem(STORAGE, i);
		m_vpPCItem.push_back(pItem);
		m_iPcStack[i] = 0;
	}

	DXUTOutputDebugString("InitPcItem m_vpPcItem size[%d]\n", static_cast<int>(m_vpPCItem.size()));

	m_iPcEly = 0;
}


void SPWindowPcTrade::DeleteAllPcItem()
{
	std::vector<SPItem*>::iterator iter;
	iter = m_vpPCItem.begin();
	for(; iter != m_vpPCItem.end(); ++iter) {
		SAFE_DELETE(*iter);
	}
	m_vpPCItem.clear();
}


void SPWindowPcTrade::ClearAllPcItem()
{
	std::vector<SPItem*>::iterator iter;
	int iIndex = 0;
	iter = m_vpPCItem.begin();
	for(; iter != m_vpPCItem.end(); ++iter, iIndex++) {
		(*iter)->SetClear();
		
		m_pPCItemIcon[iIndex]->SPSendMessage(SPIM_SET_IMAGECLEAR);
		m_pPCItemIcon[iIndex]->SetWindowText(NULL);
		m_pPCItemText[iIndex]->SetWindowText(NULL);
		
		m_bPcItemAble[iIndex] = true;
		m_iPcItemMark[iIndex] = ICON_MARK_NULL;

		m_iPcStack[iIndex] = 0;
	}
}


bool SPWindowPcTrade::ClearPcItem(int iIndex)
{
	if(iIndex < 0 || iIndex >= MAX_PC_TRADE_COUNT)
		return false;
	
	
	m_pPCItemIcon[iIndex]->SPSendMessage(SPIM_SET_IMAGECLEAR);
	m_pPCItemIcon[iIndex]->SetWindowText(NULL);
	m_pPCItemText[iIndex]->SetWindowText(NULL);
	m_bPcItemAble[iIndex] = true;
	m_iPcItemMark[iIndex] = ICON_MARK_NULL;

	m_iPcStack[iIndex] = 0;
	
	return m_vpPCItem.at(iIndex)->SetClear();
}


bool SPWindowPcTrade::SetPcItem(int iIndex, ITEM* Item)
{
	//데이터 셋팅
	m_vpPCItem.at(iIndex)->SetItemStatus(Item);
	m_vpPCItem.at(iIndex)->SetItem(STORAGE, iIndex);
	
	//인터페이스 셋팅
	int iStackCount = m_vpPCItem.at(iIndex)->GetItemStatusValue(MV_ITEM_GETSTACK_CNT);
	int iRearLevel = m_vpPCItem.at(iIndex)->GetItemStatusValue(MV_ITEM_GET_RARELEVEL);
	
	POINT ptSrcPos;
	SPTexture* pTexture = g_pItemCluster->GetItemTexture(m_vpPCItem.at(iIndex)->GetItemStatus()->GetItemID(), ptSrcPos);

	m_pPCItemIcon[iIndex]->Hide();
	m_pPCItemIcon[iIndex]->SetImage(pTexture, ptSrcPos.x, ptSrcPos.y);
	
	//if(iStackCount > 1) {
	//	char szStack[16];
	//	ZeroMemory(szStack, 16);
	//	wsprintf(szStack, "%d", iStackCount);		
	//	m_pPCItemIcon[iIndex]->SetFontColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	//	m_pPCItemIcon[iIndex]->SetWindowText(szStack);
	//}	
	
	m_pPCItemIcon[iIndex]->Show();
	//m_pPCItemText[iIndex]->SetFontColor(g_pCheckManager->GetItemColor(iRearLevel));
	if(m_vpPCItem.at(iIndex)->GetItemAttr()->m_bNameColor) {
		m_pPCItemText[iIndex]->SetFontColor(m_vpPCItem.at(iIndex)->GetItemAttr()->GetNameColor());
	}
	else {
		m_pPCItemText[iIndex]->SetFontColor(g_pCheckManager->GetItemColor(iRearLevel));
	}
	
	//[2006/7/12] 긴 아이템 명 잘라내기
	if(m_vpPCItem.at(iIndex)->GetItemAttr()->m_strName.size() > 24) {
		char szTemp[32];
		ZeroMemory(szTemp, 32);
		string strSub = m_vpPCItem.at(iIndex)->GetItemAttr()->m_strName.substr(0, 24);
		wsprintf(szTemp, "%s", strSub.c_str());		
		m_pPCItemText[iIndex]->SetWindowText(szTemp);
	}
	else { 
		m_pPCItemText[iIndex]->SetWindowText(m_vpPCItem.at(iIndex)->GetItemAttr()->m_strName.c_str());
	}
	
	m_iPcStack[iIndex] = iStackCount;

	//[2005/10/10] ItemID를 이용하여 내가 착용할수 있는 것인지를 판단
	m_bPcItemAble[iIndex] = g_pCheckManager->CheckItemStatus(m_vpPCItem.at(iIndex)->GetItemStatus()->GetItemID());
	m_iPcItemMark[iIndex] = g_pItemCluster->GetItemIconMark(m_vpPCItem.at(iIndex)->GetItemStatus()->GetItemID());
	
	return true;
}


//void SPWindowPcTrade::ItemClear(int iIndex)
int SPWindowPcTrade::ItemClear(WPARAM wParam, LPARAM lParam)
{
	int iIndex = (int)wParam;
	
	if(iIndex < 0 || iIndex >= MAX_PC_TRADE_COUNT)
		return -1;	

	SPPlayerInvenArchive* pInven = NULL;
	if(g_pGOBManager->GetLocalPlayer())
		pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	else 
		return false;

	pInven->ClearTradeItem(iIndex);

	m_pItemIcon[iIndex]->SPSendMessage(SPIM_SET_IMAGECLEAR);
	m_pItemIcon[iIndex]->SetWindowText(NULL);
	m_pItemText[iIndex]->SetWindowText(NULL);

	m_bItemAble[iIndex] = true;
	m_iItemMark[iIndex] = ICON_MARK_NULL;

	m_iStack[iIndex] = 0;
	
	////트레이드 아이템(돈포함)이 없는경우
	//m_pAccept->SetEnable(false);
	//m_pAccept->SPSendMessage(SPIM_BTN_SELECT, false);

	return 1;
}


void SPWindowPcTrade::SetItemIcon(int iIndex)
{
	if(iIndex < 0 || iIndex >= MAX_PC_TRADE_COUNT)
		return;	
	
	SPPlayerInvenArchive* pInven = NULL;
	if(g_pGOBManager->GetLocalPlayer())
		pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	else 
		return;
	
	SPSelectItem* pTradeSelectItem = pInven->GetTradeItem(iIndex);
	SPItem* pTradeItem = pInven->GetInventoryItem(pTradeSelectItem->m_iContainer, pTradeSelectItem->m_iSlotIndex);
	int iStackCount = pTradeItem->GetItemStatusValue(MV_ITEM_GETSTACK_CNT);
	int iRearLevel = pTradeItem->GetItemStatusValue(MV_ITEM_GET_RARELEVEL);

	POINT ptSrcPos;
	SPTexture* pTexture = g_pItemCluster->GetItemTexture(pTradeItem->GetItemStatus()->GetItemID(), ptSrcPos);
	//m_pIcon[iCounter]->SPSendMessage(SPIM_SET_IMAGECLEAR);
	m_pItemIcon[iIndex]->Hide();
	m_pItemIcon[iIndex]->SetImage(pTexture, ptSrcPos.x, ptSrcPos.y);
	
	//if(iStackCount > 1) {
	//	char szStack[16];
	//	ZeroMemory(szStack, 16);
	//	wsprintf(szStack, "%d", iStackCount);		
	//	m_pItemIcon[iIndex]->SetFontColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	//	m_pItemIcon[iIndex]->SetWindowText(szStack);
	//}	
	
	m_pItemIcon[iIndex]->Show();
	//m_pItemText[iIndex]->SetFontColor(g_pCheckManager->GetItemColor(iRearLevel));
	if(pTradeItem->GetItemAttr()->m_bNameColor) {
		m_pItemText[iIndex]->SetFontColor(pTradeItem->GetItemAttr()->GetNameColor());
	}
	else {
		m_pItemText[iIndex]->SetFontColor(g_pCheckManager->GetItemColor(iRearLevel));
	}

	//[2006/7/12] 긴 아이템 명 잘라내기
	if(pTradeItem->GetItemAttr()->m_strName.size() > 24) {
		char szTemp[32];
		ZeroMemory(szTemp, 32);
		string strSub = pTradeItem->GetItemAttr()->m_strName.substr(0, 24);
		wsprintf(szTemp, "%s", strSub.c_str());		
		m_pItemText[iIndex]->SetWindowText(szTemp);
	}
	else { 
		m_pItemText[iIndex]->SetWindowText(pTradeItem->GetItemAttr()->m_strName.c_str());
	}

	m_iStack[iIndex] = iStackCount;

	//[2005/10/10] 현재 올려놓은 아이템이 내가 사용할수 있는 아이템인지 확인한다 
	m_bItemAble[iIndex] = g_pCheckManager->CheckItemStatus(pTradeItem->GetItemStatus()->GetItemID());
	m_iItemMark[iIndex] = g_pItemCluster->GetItemIconMark(pTradeItem->GetItemStatus()->GetItemID());
	
	m_pAccept->SPSendMessage(SPIM_BTN_SELECT, false);
	// 내 거래상태를 대기로 설정합니다.
	m_pNameAccept->Hide();
	m_pNameWait->Show();
	//m_pStatus_Wait->Show();
	m_pStatus_Accept->Hide();
}


void SPWindowPcTrade::UpdateCashControl()
{
	if(SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_ALL) {
		if(m_iPcCash || m_iPcPoint) {
			m_iTradeCash = 0;
			m_pEditCash->SetWindowText("0");
			m_pEditCash->SetEnable(false);
			m_pEditCash->KillFocus();
			m_pEditCashDis->Show();

			m_iTradePoint = 0;
			m_pEditPoint->SetWindowText("0");
			m_pEditPoint->SetEnable(false);
			m_pEditPoint->KillFocus();
			m_pEditPointDis->Show();

			g_pInterfaceManager->SetFocusWindow(NULL);
		}
		else if(m_iPcCash == 0 && m_iPcPoint == 0) {			
			if(m_iTradePoint == 0) {
				m_pEditCashDis->Hide();
				m_pEditCash->SetEnable(true);
			}			
			
			if(m_iTradeCash == 0) {
				m_pEditPointDis->Hide();
				m_pEditPoint->SetEnable(true);
			}			
		}
	}	
	else if(SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_CASH)	{
		if(m_iPcCash) {
			m_iTradeCash = 0;
			m_pEditCash->SetWindowText("0");
			m_pEditCash->SetEnable(false);
			m_pEditCash->KillFocus();
			m_pEditCashDis->Show();

			g_pInterfaceManager->SetFocusWindow(NULL);
		}
		else {
			m_pEditCashDis->Hide();
			m_pEditCash->SetEnable(true);
		}
	}
	else if(SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_POINT) {
		if(m_iPcPoint) {
			m_iTradePoint = 0;
			m_pEditPoint->SetWindowText("0");
			m_pEditPoint->SetEnable(false);
			m_pEditPoint->KillFocus();
			m_pEditPointDis->Show();

			g_pInterfaceManager->SetFocusWindow(NULL);
		}
		else {
			m_pEditPointDis->Hide();
			m_pEditPoint->SetEnable(true);
		}
	}

	SetEnableIndun();
	
	////
	//if(m_iPcCash || m_iPcPoint) {
	//	m_iTradeCash = 0;
	//	m_pEditCash->SetWindowText(NULL);
	//	m_pEditCash->SetEnable(false);
	//	m_pEditCashDis->Show();
	//	
	//	m_iTradePoint = 0;
	//	m_pEditPoint->SetWindowText(NULL);
	//	m_pEditPoint->SetEnable(false);
	//	m_pEditPointDis->Show();
	//}
	//else if(m_iPcCash == 0 && m_iPcPoint == 0) {
	//	m_iTradeCash = 0;
	//	m_pEditCash->SetWindowText(NULL);
	//	m_pEditCashDis->Hide();
	//	m_pEditCash->SetEnable(true);

	//	m_iTradePoint = 0;
	//	m_pEditPoint->SetWindowText(NULL); 
	//	m_pEditPointDis->Hide();
	//	m_pEditPoint->SetEnable(true);

	//	m_iPcFee = 0;
	//	m_pPcFee->SetWindowText(NULL);
	//	SetFeeShow(TRADE_USER_PC, false);
	//}
}


void SPWindowPcTrade::SetEnableIndun()
{
	//캐시샵 진입이 안되는 맵 관련 처리
	if(g_pGOBManager->GetLocalPlayer()) {
		SPStage* pStage = g_StageManager.GetCurStage(g_pGOBManager->GetLocalPlayer()->GetCurStageID());
		if(pStage) {
			SPMapGroup* pMapGroup = pStage->GetMapGroup(g_pGOBManager->GetLocalPlayer()->GetCurMapGroupID());
			if( SPLocalizeManager::GetInstance()->IsEnableValue(LV_CASHSHOP) &&
				pMapGroup && pMapGroup->IsMapGroupType(GROUP_TYPE_CASH)) {
					//g_pInterfaceManager->SPChildWndSendMessage(WIID_MENU, SPIM_SET_CASH_ENABLE, 1, 0);
					;
				}
			else {
				m_pPcCashDis->Show();
				m_pPcPointDis->Show();

				//m_iTradeCash = 0;
				m_pEditCash->SetWindowText("0");
				m_pEditCash->SetEnable(false);
				m_pEditCashDis->Show();

				//m_iTradePoint = 0;
				m_pEditPoint->SetWindowText("0");
				m_pEditPoint->SetEnable(false);
				m_pEditPointDis->Show();
			}
		}
	}	
}


void SPWindowPcTrade::UpdatePcFee()
{
	int iCashTradeUnit = SPLocalizeManager::GetInstance()->GetCashTradeUnit();
	int	iCashFee = SPLocalizeManager::GetInstance()->GetCashFee();
	
	int iPointTradeUnit = SPLocalizeManager::GetInstance()->GetPointTradeUnit();	
	int iPointFee =  SPLocalizeManager::GetInstance()->GetPointFee();

	int iResultCashFee = 0;
	int iResultPointFee = 0;
	
	if(iCashFee && m_iPcCash) {		
		iResultCashFee = GET_TRADE_CASH_COMM(m_iPcCash, iCashFee);
		m_iPcFee = iResultCashFee;
		if(iResultCashFee) {
			SetFeeShow(TRADE_USER_PC, true);
			m_pPcFee->SetWindowText(ConvertMoneyToString(m_iPcFee));
			m_pPcFeeImg->Hide();
			m_pPcFeeImg->SetImagePos(476, 97);
			m_pPcFeeImg->Show();
		}
	}

	if(iPointFee && m_iPcPoint) {
		iResultPointFee = GET_TRADE_CASH_COMM(m_iPcPoint, iPointFee);
		m_iPcFee = iResultPointFee;
		if(iResultPointFee) {
			SetFeeShow(TRADE_USER_PC, true);
			m_pPcFee->SetWindowText(ConvertMoneyToString(m_iPcFee));
			m_pPcFeeImg->Hide();
			m_pPcFeeImg->SetImagePos(492, 97);
			m_pPcFeeImg->Show();
		}
	}	

	if(SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_ALL) {
		if((m_iPcCash < iCashTradeUnit || iResultCashFee == 0) && (m_iPcPoint < iPointTradeUnit || iResultPointFee == 0)) {
			m_iPcFee = 0;
			SetFeeShow(TRADE_USER_PC, false);
		}
	}
	else if(SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_CASH) {
		if(m_iPcCash < iCashTradeUnit  || iResultCashFee == 0) {
			m_iPcFee = 0;
			SetFeeShow(TRADE_USER_PC, false);
		}
	}
	else if(SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_POINT) {
		if(m_iPcPoint < iPointTradeUnit || iResultPointFee == 0) {
			m_iPcFee = 0;
			SetFeeShow(TRADE_USER_PC, false);
		}
	}
}


void SPWindowPcTrade::UpdateLocalFee()
{
	int iCashTradeUnit = SPLocalizeManager::GetInstance()->GetCashTradeUnit();
	int	iCashFee = SPLocalizeManager::GetInstance()->GetCashFee();

	int iPointTradeUnit = SPLocalizeManager::GetInstance()->GetPointTradeUnit();	
	int iPointFee =  SPLocalizeManager::GetInstance()->GetPointFee();

	int iResultCashFee = 0;
	int iResultPointFee = 0;

	if(iCashFee && m_iTradeCash) {		
		iResultCashFee = GET_TRADE_CASH_COMM(m_iTradeCash, iCashFee);
		m_iTradeFee = iResultCashFee;
		if(iResultCashFee) {
			SetFeeShow(TRADE_USER_LOCAL, true);
			m_pFee->SetWindowText(ConvertMoneyToString(m_iTradeFee));
			m_pFeeImg->Hide();
			m_pFeeImg->SetImagePos(476, 97);
			m_pFeeImg->Show();
		}
	}

	if(iPointFee && m_iTradePoint) {
		iResultPointFee = GET_TRADE_CASH_COMM(m_iTradePoint, iPointFee);
		m_iTradeFee = iResultPointFee;
		if(iResultPointFee) {
			SetFeeShow(TRADE_USER_LOCAL, true);
			m_pFee->SetWindowText(ConvertMoneyToString(m_iTradeFee));
			m_pFeeImg->Hide();
			m_pFeeImg->SetImagePos(492, 97);
			m_pFeeImg->Show();
		}
	}
	
	if(SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_ALL) {
		if((m_iTradeCash < iCashTradeUnit || iResultCashFee == 0) && (m_iTradePoint < iPointTradeUnit || iResultPointFee == 0)) {
			m_iTradeFee = 0;
			SetFeeShow(TRADE_USER_LOCAL, false);
		}
	}
	else if(SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_CASH) {
		if(m_iTradeCash < iCashTradeUnit || iResultCashFee == 0) {
			m_iTradeFee = 0;
			SetFeeShow(TRADE_USER_LOCAL, false);
		}
	}
	else if(SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_POINT) {
		if(m_iTradePoint < iPointTradeUnit || iResultPointFee == 0) {
			m_iTradeFee = 0;
			SetFeeShow(TRADE_USER_LOCAL, false);
		}
	}
}


void SPWindowPcTrade::UpdateEditControl()
{	
	if(SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_ALL) {		
		if(m_iTradeCash) {
			m_pEditPoint->SetEnable(false);
			m_pEditPointDis->Show();	
		}
		else if(m_iPcCash == 0 && m_iPcPoint == 0) {
			m_pEditPointDis->Hide();
			m_pEditPoint->SetEnable(true);
		}

		if(m_iTradePoint) {
			m_pEditCash->SetEnable(false);
			m_pEditCashDis->Show();
		}
		else if(m_iPcCash == 0 && m_iPcPoint == 0) {
			m_pEditCashDis->Hide();
			m_pEditCash->SetEnable(true);
		}

	}

	//UpdateCashControl();
}


void SPWindowPcTrade::SetFeeShow(TRADE_USER iUser, bool bShow)
{
	if(iUser == TRADE_USER_LOCAL) {
		if(bShow){
			m_pFeeText->Show();
			m_pFeeBack->Show();
			m_pFee->Show();
			m_pFeeImg->Show();
		}
		else {
			m_pFeeText->Hide();
			m_pFeeBack->Hide();
			m_pFee->Hide();
			m_pFeeImg->Hide();
		}
	}
	else if(iUser == TRADE_USER_PC) {
		if(bShow){
			m_pPcFeeText->Show();
			m_pPcFeeBack->Show();
			m_pPcFee->Show();
			m_pPcFeeImg->Show();
		}
		else {
			m_pPcFeeText->Hide();
			m_pPcFeeBack->Hide();
			m_pPcFee->Hide();
			m_pPcFeeImg->Hide();
		}
	}
}


SPItem* SPWindowPcTrade::GetPcItem(int iIndex)
{
	if(iIndex < 0 || iIndex >= static_cast<int>(m_vpPCItem.size()))
		return NULL;

	//SPItem* pItem  = m_vpPCItem.at(iIndex);
	
	return m_vpPCItem.at(iIndex);
}


const char* SPWindowPcTrade::GetPcName()
{
	if(m_pPcNameWait) {
		return m_pPcNameWait->GetWindowText();
	}

	return NULL;
}


INT64 SPWindowPcTrade::GetPcEly()
{
	return m_iPcEly;
}


INT64 SPWindowPcTrade::GetEly()
{
	return m_iTradeEly;
}


int	SPWindowPcTrade::GetCash()
{
	return m_iTradeCash;
}


int	SPWindowPcTrade::GetPcCash()
{
	return m_iPcCash;
}


int	SPWindowPcTrade::GetPoint()
{
	return m_iTradePoint;
}


int	SPWindowPcTrade::GetPcPoint()
{
	return m_iPcPoint;
}


int	SPWindowPcTrade::GetFee()
{
	return m_iTradeFee;
}


int	SPWindowPcTrade::GetPcFee()
{
	return m_iPcFee;
}


//////////////////////////////////////////////////////////////////////////
SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowPcTrade )
SPIMESSAGE_COMMAND(	SPIM_SET_PLAYER_INFO,	OnSetPlayerID	)

SPIMESSAGE_COMMAND(	SPIM_CURSOR_ENTER,		OnCursorEnter	)
SPIMESSAGE_COMMAND(	SPIM_CURSOR_OUT,		OnCursorOut		)
SPIMESSAGE_COMMAND(	SPIM_LBUTTON_DOWN,		OnCursorLDown	)
SPIMESSAGE_COMMAND(	SPIM_LBUTTON_UP,		OnCursorLUp		)
SPIMESSAGE_COMMAND(	SPIM_REFRESH,			OnRefresh		)

SPIMESSAGE_COMMAND( SPIM_TRADE_PUSHITEM,	OnPushItem		)
SPIMESSAGE_COMMAND( SPIM_TRADE_POPITEM,		OnPopItem		)
SPIMESSAGE_COMMAND( SPIM_TRADE_PUSHMONEY,	OnPushMoney		)
SPIMESSAGE_COMMAND( SPIM_TRADE_CANCEL,		OnTradeCancel	)
SPIMESSAGE_COMMAND( SPIM_TRADE_WAIT,		OnTradeWait		)
SPIMESSAGE_COMMAND( SPIM_TRADE_ACCEPT,		OnTradeAccept	)

SPIMESSAGE_COMMAND(	SPIM_TRADE_ITEM_CLEAR,	ItemClear		)
SPIMESSAGE_COMMAND( SPIM_TRADE_ITEM_CANCEL,	OnCloseTrade	)

SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_PC_TRADE_HIDE,				OnHide	)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_PC_TRADE_CANCEL,			OnHide	)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_PC_TRADE_ACCEPT,			OnAccept)

//SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_PC_TRADE_TOP,				OnCursorLUp)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_PC_TRADE_CENTER,			OnCursorLUp)
//SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_PC_TRADE_BOTTOM,			OnCursorLUp)

//SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_PC_TRADE_PC_STATUS_WAIT,	OnCursorLUp)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_PC_TRADE_PC_STATUS_ACCEPT, OnCursorLUp)
//SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_PC_TRADE_STATUS_WAIT,		OnCursorLUp)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_PC_TRADE_STATUS_ACCEPT,	OnCursorLUp)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_PC_TRADE_PC_ELY,			OnCursorLUp)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_PC_TRADE_ELY,				OnCursorLUp)

SPIMESSAGE_CONTROL( SPIM_REFRESH,			WIID_PC_TRADE_ELY,				OnUpdateEly)
SPIMESSAGE_CONTROL( SPIM_REFRESH,			WIID_PC_TRADE_CASH,				OnUpdateCash)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_PC_TRADE_CASH,				OnClickCash)
SPIMESSAGE_CONTROL( SPIM_REFRESH,			WIID_PC_TRADE_POINT,			OnUpdatePoint)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_PC_TRADE_POINT,			OnClickPoint)


SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_ENTER,	WIID_PC_TRADE_ITEM_BTN,		WIID_PC_TRADE_ITEM_BTN + MAX_PC_TRADE_COUNT,	OnToolTipEnter)
SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_OUT,		WIID_PC_TRADE_ITEM_BTN,		WIID_PC_TRADE_ITEM_BTN + MAX_PC_TRADE_COUNT,	OnToolTipOut)
SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_MOVE,	WIID_PC_TRADE_ITEM_BTN,		WIID_PC_TRADE_ITEM_BTN + MAX_PC_TRADE_COUNT,	OnToolTipMove)

SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP,		WIID_PC_TRADE_ITEM_BTN,		WIID_PC_TRADE_ITEM_BTN + MAX_PC_TRADE_COUNT,	OnItemClickUp)
SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_DOWN,	WIID_PC_TRADE_ITEM_BTN,		WIID_PC_TRADE_ITEM_BTN + MAX_PC_TRADE_COUNT,	OnItemClickDown)

SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP,		WIID_PC_TRADE_PC_ITEM_BTN,		WIID_PC_TRADE_PC_ITEM_BTN + MAX_PC_TRADE_COUNT,	OnPCItemClickUp)

SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_ENTER,	WIID_PC_TRADE_PC_ITEM_BTN,		WIID_PC_TRADE_PC_ITEM_BTN + MAX_PC_TRADE_COUNT,	OnPCToolTipEnter)
SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_OUT,		WIID_PC_TRADE_PC_ITEM_BTN,		WIID_PC_TRADE_PC_ITEM_BTN + MAX_PC_TRADE_COUNT,	OnPCToolTipOut)
SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_MOVE,	WIID_PC_TRADE_PC_ITEM_BTN,		WIID_PC_TRADE_PC_ITEM_BTN + MAX_PC_TRADE_COUNT,	OnPCToolTipMove)

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)



//////////////////////////////////////////////////////////////////////////
int SPWindowPcTrade::OnSetPlayerID	(WPARAM wParam, LPARAM lParam)
{
	GU_ID* pID = (GU_ID*)wParam;
	m_uiPlayerGUID = *pID;

	return 1;
}


int SPWindowPcTrade::OnCursorEnter	( WPARAM, LPARAM )
{
	return 1;
}

int SPWindowPcTrade::OnCursorOut	( WPARAM, LPARAM )
{
	return 1;
}

int SPWindowPcTrade::OnCursorLDown	( WPARAM, LPARAM )
{
	return 1;
}

int SPWindowPcTrade::OnCursorLUp	( WPARAM, LPARAM )
{
	if(g_pGOBManager->GetLocalPlayer())
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_INVEN_RECOVER_ICON );
	else
		return false;
	
	return 1;
}

int SPWindowPcTrade::OnRefresh	( WPARAM wParam, LPARAM lParam)
{		
	Refresh();
	return 1;
}


int SPWindowPcTrade::OnHide( WPARAM wParam, LPARAM lParam)
{	
	//거래 취소 메시지 보냄...
	SendTradeCancel();
	return 1;

	return SPWindow::OnHide(wParam, lParam);;
}


int SPWindowPcTrade::OnAccept(WPARAM wParam, LPARAM lParam)
{	
	//m_pAccept->SetEnable(true);
	//m_pAccept->setCheck()
	if(g_pGOBManager->GetLocalPlayer())
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_INVEN_RECOVER_ICON );
	else
		return false;	
	
	//[2006/7/18] 상대 아이템이 들어올 인벤공간이 있는지 확인한다
	std::vector<SPItem*>::iterator iter;
	int iRequireSlot[FIGURE_EQUIP] = {0, 0, 0, 0, 0, 0};		//Inventroy별 size 6

	//보상 타입에 따라 체크해야할 보상 정보를 설정해준다 
	for(iter = m_vpPCItem.begin(); iter != m_vpPCItem.end(); ++iter) {
		CONTAINER_TYPE iContainer = CONTAINER_TYPE_INVALID;
		SPItem* pItem = (*iter);
		
		if(pItem == NULL || pItem->GetItemStatus() == NULL || pItem->GetItemStatus()->GetItemID() == 0)
			continue;

		iContainer = (CONTAINER_TYPE)(pItem->GetItemAttr()->m_eType - 1);
		iRequireSlot[iContainer]++;
	}

	for(int i = 0; i < FIGURE_EQUIP; i++) {
		if(iRequireSlot[i] == 0) 
			continue;

		if(false == g_pCheckManager->CheckRequireSlot(i, iRequireSlot[i])) {
			if(g_pResourceManager->GetGlobalString(6001001)) {
				std::string strMsg = g_pResourceManager->GetGlobalString(6001001);	//Msg 상대방을 찾을수 없습니다.
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
			}				
			return 1;
		}
	}

	// 퀘스트 보상시 함께 들어와야 할 아이템을 저장할 공간이 있는지 확인
	for(iter = m_vpPCItem.begin(); iter != m_vpPCItem.end(); ++iter) {
		SPItem* pItem = (*iter);
		if(pItem && pItem->GetItemStatus() && pItem->GetItemStatus()->GetItemID()) {
			if(false == g_pCheckManager->CheckEnableSlot(pItem->GetItemStatus()->GetItemID(), pItem->GetItemStatus()->GetStackCount())) {
				if(g_pResourceManager->GetGlobalString(6001001)) {
					std::string strMsg = g_pResourceManager->GetGlobalString(6001001);	//Msg 상대방을 찾을수 없습니다.
					g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
				}				
				return 1;
			}
		}
	}
	
	//SendTradeAccept();
	SendTradeWait();

	//m_pAccept->SPSendMessage(SPIM_BTN_SELECT, true);
	return 1;
}


int SPWindowPcTrade::OnUpdateEly(WPARAM wParam, LPARAM lParam)
{
	if(g_pGOBManager->GetLocalPlayer())
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_INVEN_RECOVER_ICON );
	else
		return false;
	
	//INT64 iTradeEly = 0;
	m_iTradeEly = 0;
	INT64 iUserEly = 0;

	m_iTradeEly = _atoi64(m_pEditEly->GetWindowText());	
	//iTradeEly = atoi((char*)&wParam);	

	if(g_pGOBManager->GetLocalPlayer()) {
		iUserEly = g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValueEX(STATUS_EX_ELY);

		if(m_iTradeEly > iUserEly || m_iTradeEly < 0) {
			m_pEditEly->SetNULL();
			m_pEditEly->SetWindowText(NULL);
			m_pEditEly->SPSendMessage(SPIM_KILLFOCUS, 0, 0);
			m_iTradeEly = 0;
		}
		//DXUTOutputDebugString("!");
	}
	//DXUTOutputDebugString("\nOnUpdateEly [%d] [%d]\n", atoi(m_pEditEly->GetWindowText()), atoi((char*)&wParam));	
	
	SendPushMoney(m_iTradeEly, m_iTradeCash, m_iTradePoint);


	return 1;
}


int SPWindowPcTrade::OnUpdateCash(WPARAM wParam, LPARAM lParam)
{
	if(g_pGOBManager->GetLocalPlayer())
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_INVEN_RECOVER_ICON );
	else
		return false;

	if(SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_NULL || 
		SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_POINT)
	{
		return false;
	}
	
	int iInputCash = atoi(m_pEditCash->GetWindowText());		
	int iTradeCash = 0;
	int iCashTradeUnit = SPLocalizeManager::GetInstance()->GetCashTradeUnit();
	
	int iCurCash = g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValue(STATUS_CASH);
	
	int iEnableCash = 0;
	if(iCashTradeUnit)	
		iEnableCash = iCurCash / iCashTradeUnit;

	if(iInputCash > iEnableCash) {
		char szTemp[32];
		ZeroMemory(szTemp, 32);
		sprintf(szTemp, "%d", iEnableCash);
		m_pEditCash->SetWindowText(szTemp);
		m_pEditCash->SPSendMessage(SPIM_KILLFOCUS);	

		iTradeCash =  iEnableCash * iCashTradeUnit;
	}
	else {
		iTradeCash = iInputCash * iCashTradeUnit;
	}

	m_iTradeCash = iTradeCash;

	SendPushMoney(m_iTradeEly, m_iTradeCash, m_iTradePoint);		
	SetFeeShow(TRADE_USER_LOCAL, true);
	UpdateLocalFee();

	UpdateEditControl();
	////
	//if(iInputCash) {
	//	m_pEditPoint->SetEnable(false);
	//	m_pEditPointDis->Show();		
	//}
	//else {
	//	m_pEditPointDis->Hide();
	//	m_pEditPoint->SetEnable(true);
	//}
		
	////[2008/3/14] - 실제 스탯 추가시 풀어줄것
	//int iCurCash = g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValue(STATUS_CASH);
	//if(iInputCash > iCurCash) {
	//	iInputCash = iCurCash;
	//	
	//	char szTemp[32];
	//	ZeroMemory(szTemp, 32);
	//	sprintf(szTemp, "%d", iInputCash);
	//	m_pEditCash->SetWindowText(szTemp);
	//	m_pEditCash->SPSendMessage(SPIM_KILLFOCUS);
	//}

	//if(iInputCash >= iCashTradeUnit) {
	//	int iZero = iInputCash % iCashTradeUnit;
	//	if(iZero) {
	//		if(iInputCash > iCashTradeUnit) {
	//			iTradeCash = (iInputCash / iCashTradeUnit) * iCashTradeUnit;
	//			char szTemp[32];
	//			ZeroMemory(szTemp, 32);
	//			sprintf(szTemp, "%d", iTradeCash);
	//			m_pEditCash->SetWindowText(szTemp);
	//			//m_pEditCash->SPSendMessage(SPIM_KILLFOCUS);
	//			m_pEditCash->SPSendMessage(SPIM_SETFOCUS);
	//			int iCaret = g_pInterfaceManager->GetIMEPool()->GetCaretPos();	
	//			g_pInterfaceManager->GetIMEPool()->Deselect();
	//			m_pEditCash->SPSendMessage( SPIM_SET_CARET_POS, CPOS_CUR, iCaret);

	//			m_iTradeCash = iTradeCash;
	//		}
	//	}
	//	else 
	//		m_iTradeCash = iInputCash;
	//}	
	//else if(iInputCash == 0) {
	//	iTradeCash = 0;
	//	char szTemp[32];
	//	ZeroMemory(szTemp, 32);
	//	sprintf(szTemp, "%d", iTradeCash);
	//	m_pEditCash->SetWindowText(szTemp);
	//	//m_pEditCash->SPSendMessage(SPIM_KILLFOCUS);
	//	//m_pEditCash->SPSendMessage(SPIM_SETFOCUS);
	//	//int iCaret = g_pInterfaceManager->GetIMEPool()->GetCaretPos();	
	//	//g_pInterfaceManager->GetIMEPool()->Deselect();
	//	//m_pEditCash->SPSendMessage( SPIM_SET_CARET_POS, CPOS_CUR, iCaret);

	//	if(m_iTradeCash > iInputCash) {
	//		m_iTradeCash = iTradeCash;
	//		
	//		m_iTradeFee = 0;
	//		m_pFee->SetWindowText(NULL);
	//		SetFeeShow(TRADE_USER_LOCAL, false);
	//		
	//		SendPushMoney(m_iTradeEly, m_iTradeCash, m_iTradePoint);

	//		m_pEditCash->SPSendMessage(SPIM_SETFOCUS);			
	//		m_pEditPointDis->Hide();
	//		m_pEditPoint->SetEnable(true);
	//	}
	//	//m_iTradeCash = iTradeCash;
	//}
	//else {			//iInputCash > 0 && iInputCash < iCashTradeUnit
	//	m_iTradeCash = iInputCash;
	//	
	//	m_iTradeFee = 0;
	//	m_pFee->SetWindowText(NULL);
	//	SetFeeShow(TRADE_USER_LOCAL, false);
	//	UpdateLocalFee();

	//	SendPushMoney(m_iTradeEly, m_iTradeCash, m_iTradePoint);
	//}


	//if(m_iTradeCash >= iCashTradeUnit /*|| m_iTradeCash == 0*/) {
	//	m_pEditPoint->SetEnable(false);
	//	m_pEditPointDis->Show();

	//	SendPushMoney(m_iTradeEly, m_iTradeCash, m_iTradePoint);
	//	////
	//	//if(m_iTradeCash == 0) {
	//	//	m_iTradeFee = 0;
	//	//	m_pFee->SetWindowText(NULL);
	//	//	SetFeeShow(TRADE_USER_LOCAL, false);
	//	//}
	//	//else {
	//	//	SetFeeShow(TRADE_USER_LOCAL, true);
	//	//	UpdateLocalFee();
	//	//}
	//	SetFeeShow(TRADE_USER_LOCAL, true);
	//	UpdateLocalFee();
	//}
	
	return 1;
}


int SPWindowPcTrade::OnUpdatePoint(WPARAM wParam, LPARAM lParam)
{
	if(g_pGOBManager->GetLocalPlayer())
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_INVEN_RECOVER_ICON );
	else
		return false;

	if(SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_NULL || 
		SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_CASH)
	{
		return false;
	}

	int iInputPoint = atoi(m_pEditPoint->GetWindowText());		
	int iTradePoint = 0;
	int iPointTradeUnit = SPLocalizeManager::GetInstance()->GetPointTradeUnit();

	int iCurPoint = g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValue(STATUS_MILE);	
	int iEnablePoint = 0;
	if(iPointTradeUnit)	
		iEnablePoint = iCurPoint / iPointTradeUnit;

	if(iInputPoint > iEnablePoint) {
		char szTemp[32];
		ZeroMemory(szTemp, 32);
		sprintf(szTemp, "%d", iEnablePoint);
		m_pEditPoint->SetWindowText(szTemp);
		m_pEditPoint->SPSendMessage(SPIM_KILLFOCUS);	

		iTradePoint =  iEnablePoint * iPointTradeUnit;
	}
	else {
		iTradePoint = iInputPoint * iPointTradeUnit;
	}

	m_iTradePoint = iTradePoint;

	SendPushMoney(m_iTradeEly, m_iTradeCash, m_iTradePoint);	
	SetFeeShow(TRADE_USER_LOCAL, true);
	UpdateLocalFee();

	UpdateEditControl();

	////[2008/3/14] - 실제 스탯 추가시 풀어줄것
	//int iCurPoint = g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValue(STATUS_MILE);
	//if(iInputPoint > iCurPoint) {
	//	iInputPoint = iCurPoint;

	//	char szTemp[32];
	//	ZeroMemory(szTemp, 32);
	//	sprintf(szTemp, "%d", iInputPoint);
	//	m_pEditPoint->SetWindowText(szTemp);
	//	m_pEditPoint->SPSendMessage(SPIM_KILLFOCUS);
	//}

	//if(iInputPoint >= iPointTradeUnit) {
	//	int iZero = iInputPoint % iPointTradeUnit;
	//	if(iZero) {
	//		if(iInputPoint > iPointTradeUnit) {
	//			iTradePoint = (iInputPoint / iPointTradeUnit) * iPointTradeUnit;
	//			char szTemp[32];
	//			ZeroMemory(szTemp, 32);
	//			sprintf(szTemp, "%d", iTradePoint);
	//			m_pEditPoint->SetWindowText(szTemp);
	//			//m_pEditPoint->SPSendMessage(SPIM_KILLFOCUS);
	//			m_pEditPoint->SPSendMessage(SPIM_SETFOCUS);
	//			int iCaret = g_pInterfaceManager->GetIMEPool()->GetCaretPos();	
	//			g_pInterfaceManager->GetIMEPool()->Deselect();
	//			m_pEditPoint->SPSendMessage( SPIM_SET_CARET_POS, CPOS_CUR, iCaret);

	//			m_iTradePoint = iTradePoint;
	//		}
	//	}
	//	else 
	//		m_iTradePoint = iInputPoint;
	//}	
	//else if(iInputPoint == 0) {
	//	iTradePoint = 0;
	//	char szTemp[32];
	//	ZeroMemory(szTemp, 32);
	//	sprintf(szTemp, "%d", iTradePoint);
	//	m_pEditPoint->SetWindowText(szTemp);
	//	//m_pEditPoint->SPSendMessage(SPIM_KILLFOCUS);
	//	//m_pEditPoint->SPSendMessage(SPIM_SETFOCUS);
	//	//int iCaret = g_pInterfaceManager->GetIMEPool()->GetCaretPos();	
	//	//g_pInterfaceManager->GetIMEPool()->Deselect();
	//	//m_pEditPoint->SPSendMessage( SPIM_SET_CARET_POS, CPOS_CUR, iCaret);

	//	if(m_iTradePoint > iInputPoint) {
	//		m_iTradePoint = iTradePoint;

	//		m_iTradeFee = 0;
	//		m_pFee->SetWindowText(NULL);
	//		SetFeeShow(TRADE_USER_LOCAL, false);

	//		SendPushMoney(m_iTradeEly, m_iTradeCash, m_iTradePoint);

	//		m_pEditPoint->SPSendMessage(SPIM_SETFOCUS);			
	//		m_pEditCashDis->Hide();
	//		m_pEditCash->SetEnable(true);
	//	}
	//	//m_iTradePoint = iTradePoint;
	//}
	//else {			//iInputPoint > 0 && iInputPoint < iPointTradeUnit
	//	m_iTradePoint = iInputPoint;

	//	m_iTradeFee = 0;
	//	m_pFee->SetWindowText(NULL);
	//	SetFeeShow(TRADE_USER_LOCAL, false);
	//	UpdateLocalFee();

	//	SendPushMoney(m_iTradeEly, m_iTradeCash, m_iTradePoint);
	//}


	//if(m_iTradePoint >= iPointTradeUnit /*|| m_iTradePoint == 0*/) {
	//	m_pEditCash->SetEnable(false);
	//	m_pEditCashDis->Show();

	//	SendPushMoney(m_iTradeEly, m_iTradeCash, m_iTradePoint);
	//	////
	//	//if(m_iTradePoint == 0) {
	//	//	m_iTradeFee = 0;
	//	//	m_pFee->SetWindowText(NULL);
	//	//	SetFeeShow(TRADE_USER_LOCAL, false);
	//	//}
	//	//else {
	//	//	SetFeeShow(TRADE_USER_LOCAL, true);
	//	//	UpdateLocalFee();
	//	//}
	//	SetFeeShow(TRADE_USER_LOCAL, true);
	//	UpdateLocalFee();		
	//}

	return 1;
}


int SPWindowPcTrade::OnClickCash(WPARAM wParam, LPARAM lParam)
{
	if(SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_NULL || 
		SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_POINT)
	{
		return false;
	}

	//m_pEditPoint->SetEnable(false);
	//m_pEditPointDis->Show();
	return 1;
}


int SPWindowPcTrade::OnClickPoint(WPARAM wParam, LPARAM lParam)
{
	if(SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_NULL || 
		SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_CASH)
	{
		return false;
	}

	//m_pEditCash->SetEnable(false);
	//m_pEditCashDis->Show();
	return 1;
}


int SPWindowPcTrade::OnToolTipMove(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	int iSelectNum = iUiID - WIID_PC_TRADE_ITEM_BTN;

	SPPlayerInvenArchive* pInven = NULL;
	if(g_pGOBManager->GetLocalPlayer())
		pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	else 
		return false;
	
	SPSelectItem* pSelectItem = pInven->GetTradeItem(iSelectNum);

	if(pSelectItem->m_iContainer < INVENTORY_EQUIP || pSelectItem->m_iContainer > INVENTORY_HOUSING) {
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
		return 0;
	}
	
	if(iSelectNum < 0 || iSelectNum >= pInven->GetInventorySize(pSelectItem->m_iContainer)) {
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
		return 0;
	}

	if(pInven->IsItemExist(pSelectItem->m_iSlotIndex, pSelectItem->m_iContainer) == false) {
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
		return 0;
	}

	SPItem* pItem = pInven->GetInventoryItem(pSelectItem->m_iContainer, pSelectItem->m_iSlotIndex);
	
	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage( SPIM_TOOLTIP_MOVE, (WPARAM)pItem->GetItemStatus(), lParam);
	pWindow->Show();

	return 1;
}


int SPWindowPcTrade::OnToolTipEnter(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	int iSelectNum = iUiID - WIID_PC_TRADE_ITEM_BTN;

	SPPlayerInvenArchive* pInven = NULL;
	if(g_pGOBManager->GetLocalPlayer())
		pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	else 
		return false;

	SPSelectItem* pSelectItem = pInven->GetTradeItem(iSelectNum);

	if(pSelectItem->m_iContainer < INVENTORY_EQUIP || pSelectItem->m_iContainer > INVENTORY_HOUSING) {
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
		return 0;
	}

	if(iSelectNum < 0 || iSelectNum >= pInven->GetInventorySize(pSelectItem->m_iContainer)) {
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
		return 0;
	}

	if(pInven->IsItemExist(pSelectItem->m_iSlotIndex, pSelectItem->m_iContainer) == false) {
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
		return 0;
	}

	SPItem* pItem = pInven->GetInventoryItem(pSelectItem->m_iContainer, pSelectItem->m_iSlotIndex);

	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage( SPIM_TOOLTIP_MOVE, (WPARAM)pItem->GetItemStatus(), lParam);
	pWindow->Show();

	return 1;
}


int SPWindowPcTrade::OnToolTipOut(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	int iSelectNum = iUiID - WIID_PC_TRADE_ITEM_BTN;
	
	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
	pWindow->Show();

	return 1;
}


int SPWindowPcTrade::OnItemClickUp(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	int iSelectNum = iUiID - WIID_PC_TRADE_ITEM_BTN;
	DXUTOutputDebugString("SPWindowPcTrade::OnItemClickUp selectIndex[%d]\n", iSelectNum);

	SPPlayerInvenArchive* pInven = NULL;
	if(g_pGOBManager->GetLocalPlayer())
		pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	else
		return false;

	//이 앞에서 현재 트레이드 슬롯에 아이템이 있는지 확인
	SPSelectItem* pCurItem = g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetTradeItem(iSelectNum);	
	
	if(g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->IsItemExist(pCurItem->m_iSlotIndex, pCurItem->m_iContainer)) {
		//if(pInven->GetPickupItem().IsNull()) {
			SendPopItem(iSelectNum);			//메시지 아이템 내림 
		//}
		//else {
			g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
		//}	
			
			//[2006/3/13]
			m_bChanged = true;
			m_fChangedAccumulate = 0.0f;
			m_pAccept->SetEnable(false);
			// 내 거래상태를 대기로 설정합니다.
			//m_pStatus_Wait->Show();
			m_pNameAccept->Hide();
			m_pNameWait->Show();
			m_pStatus_Accept->Hide();
		
		//ItemClear(iSelectNum, 0);		
		return 1;
	}

	////
	//if(g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->SetTradeItem(iSelectNum) == false) {		
	//	if(g_pGOBManager->GetLocalPlayer())
	//		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
	//	return -1;
	//}
	//else {		//거래창에 아이콘 셋팅		
	//	//SetItemIcon(iSelectNum);
	//}
	
	//if(g_pGOBManager->GetLocalPlayer())
	//	g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);

	if(IsTradeEnable()) {
		if(pInven->GetPickupItem().IsNull() == false) {
			//[2005/9/2] 장비창에서 교환창으로 내려가는것을 맊는다...
			if(pInven->GetPickupItem().m_iContainer > INVENTORY_HOUSING) {
				g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
				return 1;
			}
			
			//[2005/10/13] 아이템이 교환 불가인 경우 내려가는것을 맊는다...
			if(pInven->IsTradeEnable() == false) {				//픽업되어 있는 아이템을 체크한다 
				g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
				return 1;
			}
			
			SendPushItem(iSelectNum);							//메시지 전송
			
			//[2006/3/13]
			m_bChanged = true;
			m_fChangedAccumulate = 0.0f;
			m_pAccept->SetEnable(false);
			// 내 거래상태를 대기로 설정합니다.
			//m_pStatus_Wait->Show();
			m_pNameAccept->Hide();
			m_pNameWait->Show();
			m_pStatus_Accept->Hide();
		}
	}

	return 1;
}


int SPWindowPcTrade::OnItemClickDown(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	int iSelectNum = iUiID - WIID_PC_TRADE_ITEM_BTN;
	DXUTOutputDebugString("SPWindowPcTrade::OnItemClickDown selectIndex[%d]\n", iSelectNum);
	//ItemClear(iSelectNum, 0);	

	return 1;
}


int SPWindowPcTrade::OnPCToolTipMove(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	int iSelectNum = iUiID - WIID_PC_TRADE_PC_ITEM_BTN;	

	SPItem* pItem  = m_vpPCItem.at(iSelectNum);

	if(m_vpPCItem.at(iSelectNum)->GetItemAttr() == 0) {
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
		return 0;
	}

	if(m_vpPCItem.at(iSelectNum)->GetItemStatusValue(MV_ITEM_GETUID) == 0) {
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
		return 0;
	}

	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage( SPIM_TOOLTIP_MOVE, (WPARAM)m_vpPCItem.at(iSelectNum)->GetItemStatus(), lParam);
	pWindow->Show();

	return 1;
}


int SPWindowPcTrade::OnPCToolTipEnter(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	int iSelectNum = iUiID - WIID_PC_TRADE_PC_ITEM_BTN;	

	SPItem* pItem  = m_vpPCItem.at(iSelectNum);

	if(m_vpPCItem.at(iSelectNum)->GetItemAttr() == 0) {
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
		return 0;
	}

	if(m_vpPCItem.at(iSelectNum)->GetItemStatusValue(MV_ITEM_GETUID) == 0) {
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
		return 0;
	}

	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage( SPIM_TOOLTIP_MOVE, (WPARAM)m_vpPCItem.at(iSelectNum)->GetItemStatus(), lParam);
	pWindow->Show();

	return 1;
}


int SPWindowPcTrade::OnPCToolTipOut(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	int iSelectNum = iUiID - WIID_PC_TRADE_PC_ITEM_BTN;

	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
	pWindow->Show();
	
	return 1;
}


int SPWindowPcTrade::OnPCItemClickUp(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	int iSelectNum = iUiID - WIID_PC_TRADE_PC_ITEM_BTN;
	
	if(g_pGOBManager->GetLocalPlayer()) {
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_INVEN_RECOVER_ICON );
	}
	else
		return false;

	return 1;
}



////////////////////////////////////////////////////////////////////////// Network Message
bool SPWindowPcTrade::SendPushItem(int iIndex) {	
	TRADE_ITEM TradeItem;
	SPSelectItem pSelectItem;

	SPPlayerInvenArchive* pInven = NULL;
	if(g_pGOBManager->GetLocalPlayer())
		pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	else
		return false;

	//pSelectItem = pInven->GetPickupItem();

	if(pInven->GetPickupItem().IsNull())
		return false;

	//[2005/10/11]	인벤토리 해당 슬롯에 아이템이 없으면 리턴
	if(pInven->IsItemExist(pInven->GetPickupItem().m_iSlotIndex, pInven->GetPickupItem().m_iContainer) == false)
		return false;

	TradeItem.stActSlotInfo.ContainerType = pInven->GetPickupItem().m_iContainer;
	TradeItem.stActSlotInfo.SlotIndex = pInven->GetPickupItem().m_iSlotIndex + 1;
	TradeItem.TradeSlotIndex = iIndex + 1;

	if(g_pCheckManager->IsDBBlock()) {
		
		DXUTOutputDebugString("\tSPWindowPcTrade::SendPushItem DB Working Block\n");
		return false;
	}

	CPacket Packet(TRADE_CS_PUSHITEM);
	Packet.AddData(&TradeItem, sizeof(TRADE_ITEM));	
	g_pNetworkManager->SPPerformMessage(TRADE_CS_PUSHITEM, 0, (LPARAM)&Packet);		

	return true;
}


int SPWindowPcTrade::OnPushItem(WPARAM wParam, LPARAM lParam)
{
	CPacket* pPacket = (CPacket*)lParam;	
	
	GU_ID			ActCharGUID;
	TRADE_ITEM		stTradeItem;
	ITEM			stItem;		
	pPacket->ExtractUINT64(&ActCharGUID);
	pPacket->ExtractStruct(&stTradeItem, sizeof(TRADE_ITEM));
	pPacket->ExtractStruct(&stItem, sizeof(ITEM));	
	
	stTradeItem.stActSlotInfo.SlotIndex = stTradeItem.stActSlotInfo.SlotIndex - 1;	//클라이언트는 인덱스가 0베이스임...
	stTradeItem.TradeSlotIndex = stTradeItem.TradeSlotIndex - 1;					//클라이언트는 인덱스가 0베이스임...

	SPPlayerInvenArchive* pInven = NULL;
	if(g_pGOBManager->GetLocalPlayer())
		pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	else 
		return false;
	
	if(g_pGOBManager->GetLocalPlayer()->GetGUID() == ActCharGUID) {		//로컬 플레이어
		//SPSelectItem PickUpItem = pInven->GetPickupItem();
		//if(PickUpItem.m_iContainer == stTradeItem.stActSlotInfo.ContainerType && 
		//	PickUpItem.m_iSlotIndex == stTradeItem.stActSlotInfo.SlotIndex) 
		//{
		if(pInven->IsItemExist(stTradeItem.stActSlotInfo.SlotIndex, stTradeItem.stActSlotInfo.ContainerType)) {
			if(pInven->SetTradeItem(stTradeItem.TradeSlotIndex, stTradeItem.stActSlotInfo.ContainerType, stTradeItem.stActSlotInfo.SlotIndex))		//SetTradeItem 에서 RecoverIcon 된다
				SetItemIcon(stTradeItem.TradeSlotIndex);
		}
		//m_pAccept->SPSendMessage(SPIM_BTN_SELECT, false);
		//m_pAccept->SetEnable(true);
	}
	else if(m_uiPlayerGUID == ActCharGUID) {							//대상 PC
		SetPcItem(stTradeItem.TradeSlotIndex, &stItem);		
		
		//m_pPCAccept->SPSendMessage(SPIM_SET_IMAGECLEAR);
		//m_pPCAccept->SetImage(ACCEPT_IMAGE_FILE.c_str(), m_iSrcPcAcceptX[PC_TRADE_ACCEPT_ENABLE], m_iSrcPcAcceptY);
	}
	else {
		DXUTOutputDebugString("SPWindowPcTrade::OnPushItem UserID Error\n");
	}		
	
	if(!m_bChanged /*&& CheckCashTrade()*/) {
		m_pAccept->SetEnable(true);
		m_pAccept->SPSendMessage(SPIM_BTN_SELECT, false);
		// 내 거래상태를 대기로 설정합니다.
		//m_pStatus_Wait->Show();
		m_pNameAccept->Hide();
		m_pNameWait->Show();
		m_pStatus_Accept->Hide();
	}

	// 상대 거래상태를 대기로 설정합니다.
	//m_pPcStatus_Wait->Show();
	m_pPcNameAccept->Hide();
	m_pPcNameWait->Show();
	m_pPcStatus_Accept->Hide();
	//m_pPCAccept->SPSendMessage(SPIM_SET_IMAGECLEAR);
	//m_pPCAccept->SetImage(ACCEPT_IMAGE_FILE.c_str(), m_iSrcPcAcceptX[PC_TRADE_ACCEPT_ENABLE], m_iSrcPcAcceptY);


	return 1;
}


bool SPWindowPcTrade::SendPopItem(int iIndex)
{
	if(g_pCheckManager->IsDBBlock()) {
		
		DXUTOutputDebugString("\tSPWindowPcTrade::SendPopItem DB Working Block\n");
		return false;
	}
	
	CPacket Packet(TRADE_CS_POPITEM);
	Packet.AddUINT8((UINT8)iIndex + 1);
	g_pNetworkManager->SPPerformMessage(TRADE_CS_POPITEM, 0, (LPARAM)&Packet);

	return true;
}


int SPWindowPcTrade::OnPopItem(WPARAM wParam, LPARAM lParam)
{
	CPacket* pPacket = (CPacket*)lParam;
	
	GU_ID			ActCharGUID;	// 아이템을 내린 아바타키
	SLOT_INDEX		TradeSlotIndex;	// 트레이드슬롯

	pPacket->ExtractUINT64(&ActCharGUID);
	pPacket->ExtractUINT8(&TradeSlotIndex);

	int iSelectNum = TradeSlotIndex - 1;

	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return false;
	
	if(g_pGOBManager->GetLocalPlayer()->GetGUID() == ActCharGUID) {		//로컬 플레이어
		ItemClear(iSelectNum, 0);
		
		//m_pAccept->SPSendMessage(SPIM_BTN_SELECT, false);
		//m_pAccept->SetEnable(true);
	}
	else if(m_uiPlayerGUID == ActCharGUID) {							//대상 PC
		ClearPcItem(iSelectNum);
		
		//m_pPCAccept->SPSendMessage(SPIM_SET_IMAGECLEAR);
		//m_pPCAccept->SetImage(ACCEPT_IMAGE_FILE.c_str(), m_iSrcPcAcceptX[PC_TRADE_ACCEPT_ENABLE], m_iSrcPcAcceptY);		
	}
	else {
		DXUTOutputDebugString("SPWindowPcTrade::OnPopItem UserID Error\n");
	}
	
	if(!m_bChanged /*&& CheckCashTrade()*/) {
		m_pAccept->SetEnable(true);
		m_pAccept->SPSendMessage(SPIM_BTN_SELECT, false);
		// 내 거래상태를 대기로 설정합니다.
		//m_pStatus_Wait->Show();
		m_pNameAccept->Hide();
		m_pNameWait->Show();
		m_pStatus_Accept->Hide();
	}

	// 상대 거래상태를 대기로 설정합니다.
	//m_pPcStatus_Wait->Show();
	m_pPcNameAccept->Hide();
	m_pPcNameWait->Show();
	m_pPcStatus_Accept->Hide();
	//m_pPCAccept->SPSendMessage(SPIM_SET_IMAGECLEAR);
	//m_pPCAccept->SetImage(ACCEPT_IMAGE_FILE.c_str(), m_iSrcPcAcceptX[PC_TRADE_ACCEPT_ENABLE], m_iSrcPcAcceptY);

	return 1;
}


bool SPWindowPcTrade::SendPushMoney(INT64 uiMoney, int iCash /*= 0*/, int iPoint /*= 0*/)
{
	DXUTOutputDebugString("SendPushMoney C[%d] P[%d] E[%d]\n", iCash, iPoint, uiMoney);

	if(g_pCheckManager->IsDBBlock()) {		
		DXUTOutputDebugString("\tSPWindowPcTrade::SendPushMoney DB Working Block\n");
		return false;
	}

	////
	//int iCashTradeUnit = SPLocalizeManager::GetInstance()->GetCashTradeUnit();
	//int	iCashFee = SPLocalizeManager::GetInstance()->GetCashFee();

	//int iPointTradeUnit = SPLocalizeManager::GetInstance()->GetPointTradeUnit();	
	//int iPointFee =  SPLocalizeManager::GetInstance()->GetPointFee();
	//
	//if(SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_ALL || 
	//	SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_CASH)
	//{
	//	if(iCash < iCashTradeUnit) {
	//		iCash = 0;
	//	}
	//}

	//if(SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_ALL || 
	//	SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_POINT)
	//{
	//	if(iPoint < iPointTradeUnit) {
	//		iPoint = 0;
	//	}
	//}

	if(SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_ALL) {
		if(iPoint && iCash) {			//캐시와 포인트 둘다 존재하는 경우
			return false;
		}
	}
	
	//if(CheckCashTrade()) {
	//[2006/3/13]
	m_bChanged = true;
	m_fChangedAccumulate = 0.0f;
	m_pAccept->SetEnable(false);
	m_pAccept->SPSendMessage(SPIM_BTN_SELECT, false);
	// 내 거래상태를 대기로 설정합니다.
	//m_pStatus_Wait->Show();
	m_pNameAccept->Hide();
	m_pNameWait->Show();
	m_pStatus_Accept->Hide();
	//}	

	CPacket Packet(TRADE_CS_PUSHMONEY);
	Packet.AddUINT64((UINT64)uiMoney);
	Packet.AddUINT32(iCash);
	Packet.AddUINT32(iPoint);	
	g_pNetworkManager->SPPerformMessage(TRADE_CS_PUSHMONEY, 0, (LPARAM)&Packet);
	return true;
}


int SPWindowPcTrade::OnPushMoney(WPARAM wParam, LPARAM lParam)
{
	CPacket* pPacket = (CPacket*)lParam;
	
	GU_ID		ActCharGUID;	// 돈을 올린 아바타의 키
	UINT64		uiMoney;		// 올린 돈
	UINT32		iCash;
	UINT32		iPoint;
	
	pPacket->ExtractUINT64(&ActCharGUID);
	pPacket->ExtractUINT64(&uiMoney);
	pPacket->ExtractUINT32(&iCash);
	pPacket->ExtractUINT32(&iPoint);	

	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return false;

	if(g_pGOBManager->GetLocalPlayer()->GetGUID() == ActCharGUID) {		//로컬 플레이어
		DXUTOutputDebugString("OnPushMoney Local C[%d] P[%d] E[%d]\n", iCash, iPoint, uiMoney);
		//m_pAccept->SPSendMessage(SPIM_BTN_SELECT, false);
		//m_pAccept->SetEnable(true);
	}
	else if(m_uiPlayerGUID == ActCharGUID) {							//대상 PC
		DXUTOutputDebugString("OnPushMoney PC C[%d] P[%d] E[%d]\n", iCash, iPoint, uiMoney);
		
		m_iPcEly = uiMoney;
		m_iPcCash = iCash;
		m_iPcPoint = iPoint;
		//char szMoney[32];
		//ZeroMemory(szMoney, 32);
		//wsprintf(szMoney, "%d", m_iPcEly);
		//m_pPcEly->SetWindowText(szMoney);
		m_pPcEly->SetWindowText(ConvertMoneyToString(m_iPcEly));
		
		//m_pPCAccept->SPSendMessage(SPIM_SET_IMAGECLEAR);
		//m_pPCAccept->SetImage(ACCEPT_IMAGE_FILE.c_str(), m_iSrcPcAcceptX[PC_TRADE_ACCEPT_ENABLE], m_iSrcPcAcceptY);

		if(m_pPcCash)
			m_pPcCash->SetWindowText(ConvertMoneyToString(m_iPcCash));

		if(m_pPcPoint)
			m_pPcPoint->SetWindowText(ConvertMoneyToString(m_iPcPoint));

		UpdateCashControl();
		UpdatePcFee();
	}
	else {
		DXUTOutputDebugString("SPWindowPcTrade::OnPopItem UserID Error\n");
	}	
	
	if(!m_bChanged /*&& CheckCashTrade()*/)	{		//[2006/3/13]
		m_pAccept->SPSendMessage(SPIM_BTN_SELECT, false);
		m_pAccept->SetEnable(true);
		// 내 거래상태를 대기로 설정합니다.
		//m_pStatus_Wait->Show();
		m_pNameAccept->Hide();
		m_pNameWait->Show();
		m_pStatus_Accept->Hide();
	}
	
	// 상대 거래상태를 대기로 설정합니다.
	//m_pPcStatus_Wait->Show();
	m_pPcNameAccept->Hide();
	m_pPcNameWait->Show();
	m_pPcStatus_Accept->Hide();
	//m_pPCAccept->SPSendMessage(SPIM_SET_IMAGECLEAR);
	//m_pPCAccept->SetImage(ACCEPT_IMAGE_FILE.c_str(), m_iSrcPcAcceptX[PC_TRADE_ACCEPT_ENABLE], m_iSrcPcAcceptY);

	return 1;
}


bool SPWindowPcTrade::SendTradeCancel()
{
	if(m_bSendCancel)				return false;
	
	CPacket Packet(TRADE_CS_CANCEL);
	g_pNetworkManager->SPPerformMessage(TRADE_CS_CANCEL, 0, (LPARAM)&Packet);

	DXUTOutputDebugString("SendTradeCancel()\n");
	
	m_bSendCancel = true;
	
	return true;
}


int SPWindowPcTrade::OnTradeCancel(WPARAM wParam, LPARAM lParam)
{
	CPacket* pPacket = (CPacket*)lParam;
	
	GU_ID			ActCharGUID;	// 거래를 취소한 아바타키	
	
	pPacket->ExtractUINT64(&ActCharGUID);
	
	char szName[32];
	char szMsg[512];
	ZeroMemory(szName, 32);
	ZeroMemory(szMsg, 512);

	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return false;
	
	if(g_pGOBManager->GetLocalPlayer()->GetGUID() == ActCharGUID) {		//로컬 플레이어
		SPPlayer* pPlayer = NULL;
		pPlayer = (SPPlayer*)g_pGOBManager->Find(m_uiPlayerGUID);
		if(pPlayer == NULL) {						//클라이언트가 찾을수 없는경우				
			std::string strMsg = g_pResourceManager->GetGlobalString(4000001);	//Msg 상대방을 찾을수 없습니다.
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
			g_pGOBManager->GetLocalPlayer()->SetTradeState(TRADE_STATE_NULL);
		}
		else {
			pPlayer->SPGOBPerformMessage(SPGM_GETGOBNAME, (LPARAM)szName);
			std::string strMsg = g_pResourceManager->GetGlobalString(4001002);	//Msg %s님과 거래가 취소되었습니다.
			wsprintf(szMsg, strMsg.c_str(), szName);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMsg);
		}
	}
	else if(m_uiPlayerGUID == ActCharGUID) {							//대상 PC		
		SPPlayer* pPlayer = NULL;
		pPlayer = (SPPlayer*)g_pGOBManager->Find(ActCharGUID);
		if(pPlayer == NULL) {						//클라이언트가 찾을수 없는경우				
			std::string strMsg = g_pResourceManager->GetGlobalString(4000001);	//Msg 상대방을 찾을수 없습니다.
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
			g_pGOBManager->GetLocalPlayer()->SetTradeState(TRADE_STATE_NULL);
		}
		else {
			pPlayer->SPGOBPerformMessage(SPGM_GETGOBNAME, (LPARAM)szName);
			std::string strMsg = g_pResourceManager->GetGlobalString(4001012);	//Msg %s님이 거래를 취소했습니다.
			wsprintf(szMsg, strMsg.c_str(), szName);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMsg);
		}		
	}
	else {		
		if(ActCharGUID == 0){
			std::string strMsg = g_pResourceManager->GetGlobalString(4000001);	//Msg 상대방을 찾을수 없습니다.
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
			g_pGOBManager->GetLocalPlayer()->SetTradeState(TRADE_STATE_NULL);
		}
		DXUTOutputDebugString("SPWindowPcTrade::OnPopItem UserID Error\n");
	}

	////실제 데이터 클리어
	//SPPlayerInvenArchive* pInven = NULL;
	//if(g_pGOBManager->GetLocalPlayer())
	//	pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();

	//pInven->ClearAllTradeItem();	

	//for(int i = 0; i < MAX_PC_TRADE_COUNT ; i++) {	//아이템 컨트롤러(인터페이스) 클리어
	//	ItemClear(i, 0);
	//}	

	//ClearAllPcItem();								//상대방의 아이템 클리어
	//m_pPcEly->SetWindowText(NULL);

	////임시...
	//g_pInterfaceManager->GetInvenWindow()->Hide();
	//g_pInterfaceManager->GetEquipWindow()->Hide();	

	//this->Hide();

	//char szTemp[256];
	//wsprintf(szTemp, "거래가 취소되었습니다.\n");
	//g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szTemp );

	CloseTrade();

	return 1;
}


bool SPWindowPcTrade::SendTradeWait()
{
	CPacket Packet(TRADE_CS_WAIT);
	g_pNetworkManager->SPPerformMessage(TRADE_CS_WAIT, 0, (LPARAM)&Packet);
	DXUTOutputDebugString("SendTradeWait()\n");		

	m_iTradeMode = TRADE_MODE_WAIT;
	m_pAccept->SetEnable(false);
	return true;
}


int SPWindowPcTrade::OnTradeWait(WPARAM wParam, LPARAM lParam)
{
	CPacket* pPacket = (CPacket*)lParam;

	GU_ID			ActCharGUID;	// 거래를 취소한 아바타키	
	UINT8			byAllWait;		// accept를 누른 아바타키

	pPacket->ExtractUINT64(&ActCharGUID);
	pPacket->ExtractUINT8(&byAllWait);
	
	if(byAllWait) {
		SPWindow* pTradeResult = g_pInterfaceManager->FindWindow(WIID_PC_TRADE_RESULT);
		if(pTradeResult) {
			pTradeResult->Show();
		}		

		Hide();
	}
	else {
		if(g_pGOBManager->GetLocalPlayer()->GetGUID() == ActCharGUID) {		//로컬 플레이어		
			m_pAccept->SPSendMessage(SPIM_BTN_SELECT, true);
			// 내 거래상태를 수락으로 설정합니다.
			//m_pStatus_Wait->Hide();
			m_pNameWait->Hide();
			m_pNameAccept->Show();
			m_pStatus_Accept->Show();
		}
		else if(m_uiPlayerGUID == ActCharGUID) {							//대상 PC			
			// 상대 거래상태를 수락으로 설정합니다.
			//m_pPcStatus_Wait->Hide();
			m_pPcNameWait->Hide();
			m_pPcNameAccept->Show();
			m_pPcStatus_Accept->Show();
			//m_pPCAccept->SPSendMessage(SPIM_SET_IMAGECLEAR);
			//m_pPCAccept->SetImage(ACCEPT_IMAGE_FILE.c_str(), m_iSrcPcAcceptX[PC_TRADE_ACCEPT_ACCEPT], m_iSrcPcAcceptY);
		}
		else {
			DXUTOutputDebugString("SPWindowPcTrade::OnPopItem UserID Error\n");
		}

		return 1;
	}
	
	return 1;
}


bool SPWindowPcTrade::SendTradeAccept()
{
	if(g_pCheckManager->IsDBBlock()) {
		
		DXUTOutputDebugString("\tSPWindowPcTrade::SendTradeAccept DB Working Block\n");
		return false;
	}
	
	CPacket Packet(TRADE_CS_ACCEPT);
	g_pNetworkManager->SPPerformMessage(TRADE_CS_ACCEPT, 0, (LPARAM)&Packet);

	return true;
}


int SPWindowPcTrade::OnTradeAccept(WPARAM wParam, LPARAM lParam)
{
	CPacket* pPacket = (CPacket*)lParam;
	GU_ID			ActCharGUID;	// accept를 누른 아바타키
	UINT8			byFinish;		// 거래완료여부
	//UINT8			byResult;		// 거래결과
	GLOBAL_STRING_ID	iErrorID;	// 에러아이디(0이면 에러없음)
	GU_ID			ErrCharGUID;	// 오류시 오류제공자 아바타키. 누구누구님땜시 거래실패 메시지를 보내야해서 추가

	pPacket->ExtractUINT64(&ActCharGUID);

	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return false;
	
	pPacket->ExtractUINT8(&byFinish);
	if(0 == byFinish)
	{
		// 한쪽에서 수락만한 경우
		if(g_pGOBManager->GetLocalPlayer()->GetGUID() == ActCharGUID) {		//로컬 플레이어		
			m_pAccept->SPSendMessage(SPIM_BTN_SELECT, true);
			// 내 거래상태를 수락으로 설정합니다.
			//m_pStatus_Wait->Hide();
			m_pNameWait->Hide();
			m_pNameAccept->Show();
			m_pStatus_Accept->Show();
		}
		else if(m_uiPlayerGUID == ActCharGUID) {							//대상 PC			
			// 상대 거래상태를 수락으로 설정합니다.
			//m_pPcStatus_Wait->Hide();
			m_pPcNameWait->Hide();
			m_pPcNameAccept->Show();
			m_pPcStatus_Accept->Show();
			//m_pPCAccept->SPSendMessage(SPIM_SET_IMAGECLEAR);
			//m_pPCAccept->SetImage(ACCEPT_IMAGE_FILE.c_str(), m_iSrcPcAcceptX[PC_TRADE_ACCEPT_ACCEPT], m_iSrcPcAcceptY);
		}
		else {
			DXUTOutputDebugString("SPWindowPcTrade::OnPopItem UserID Error\n");
		}
		
		return 1;
	}

	//pPacket->ExtractUINT8(&byResult);
	pPacket->ExtractUINT32(&iErrorID);

	char szName[32];
	char szMsg[512];
	ZeroMemory(szName, 32);
	ZeroMemory(szMsg, 512);

	if(0 == iErrorID)		// 거래가 성공적 완료된 경우
	{		
		if(g_pGOBManager->GetLocalPlayer()->GetGUID() == ActCharGUID) {		//로컬 플레이어			
			SPPlayer* pPlayer = NULL;
			pPlayer = (SPPlayer*)g_pGOBManager->Find(m_uiPlayerGUID);
			if(pPlayer == NULL) {						//클라이언트가 찾을수 없는경우				
				std::string strMsg = g_pResourceManager->GetGlobalString(4000001);	//Msg 상대방을 찾을수 없습니다.
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
				g_pGOBManager->GetLocalPlayer()->SetTradeState(TRADE_STATE_NULL);
			}
			else {
				pPlayer->SPGOBPerformMessage(SPGM_GETGOBNAME, (LPARAM)szName);
				std::string strMsg = g_pResourceManager->GetGlobalString(4001020);	//Msg %s님과 거래가 종료 되었습니다.
				wsprintf(szMsg, strMsg.c_str(), szName);
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMsg);
			}
		}
		else if(m_uiPlayerGUID == ActCharGUID) {							//대상 PC
			SPPlayer* pPlayer = NULL;
			pPlayer = (SPPlayer*)g_pGOBManager->Find(ActCharGUID);
			if(pPlayer == NULL) {						//클라이언트가 찾을수 없는경우				
				std::string strMsg = g_pResourceManager->GetGlobalString(4000001);	//Msg 상대방을 찾을수 없습니다.
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
				g_pGOBManager->GetLocalPlayer()->SetTradeState(TRADE_STATE_NULL);
			}
			else {
				pPlayer->SPGOBPerformMessage(SPGM_GETGOBNAME, (LPARAM)szName);
				std::string strMsg = g_pResourceManager->GetGlobalString(4001020);	//Msg %s님과 거래가 종료 되었습니다.
				wsprintf(szMsg, strMsg.c_str(), szName);
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMsg);
			}			
		}
		else {
			DXUTOutputDebugString("SPWindowPcTrade::OnPopItem UserID Error\n");
		}

		//char szTemp[256];
		//wsprintf(szTemp, "거래가 성공하였습니다.\n");
		//g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szTemp );
		
		CloseTrade();
		return 1;
	}
	else {					// 거래가 실패한 경우
		pPacket->ExtractUINT64(&ErrCharGUID);
		// 완료했지만 거래는 실패한 경우
		
		SPPlayer* pPlayer = NULL;
		pPlayer = (SPPlayer*)g_pGOBManager->Find(ErrCharGUID);
		if(pPlayer == NULL) {						//클라이언트가 찾을수 없는경우				
			std::string strMsg = g_pResourceManager->GetGlobalString(4000001);	//Msg 상대방을 찾을수 없습니다.
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
			g_pGOBManager->GetLocalPlayer()->SetTradeState(TRADE_STATE_NULL);
		}
		else {
			pPlayer->SPGOBPerformMessage(SPGM_GETGOBNAME, (LPARAM)szName);
			std::string strMsg = g_pResourceManager->GetGlobalString(4001040);	//Msg %s님이 더 이상 아이템을 저장핤 없어 거래 취소
			wsprintf(szMsg, strMsg.c_str(), szName);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMsg);
		}	
		
		CloseTrade();
		return 1;
	}

	//pPacket->ExtractUINT64(&ErrCharGUID);
	// 완료했지만 거래는 실패한 경우
	
	return 1;
}


int SPWindowPcTrade::OnCloseTrade(WPARAM wParam, LPARAM lParam)
{
	CloseTrade();
	return 1;
}

