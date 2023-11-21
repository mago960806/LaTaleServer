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
#include "SPWindowCashTradeWait.h"

////
#include "SPStage.h"
#include "SPStageManager.h"
//#include "SPTerrainAttribute.h"
//
//#include "SPCommandConvert.h"
//#include "SPGOBClusterDef.h"
//#include "SPAvatarModelUnitDef.h"
//#include "SPGOBCoordPhysics.h"
//#include "SPPlayerCoordPhysics.h"
//#include "SPPlayerInvenArchive.h"
//
//#include "SPGOBStatus.h"
//#include "SPPlayerStatus.h"
//#include "SPPlayerStatusModel.h"

#include "SPRenderModelDEF.h"
#include "SPGameObjectDef.h"
#include "SPGameObject.h"
#include "SPGOBStatus.h"
#include "SPPlayerStatus.h"
#include "SPPlayerStatusModel.h"
#include "SPPlayerInvenArchive.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGOBCoordPhysics.h"
#include "SPPlayer.h"

#include "Packet.h"
#include "PacketID.h"
#include "SPNetworkManager.h"

#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPItem.h"
#include "SPItemCluster.h"			//[2005/6/16] - jinhee
//#include "SPPlayer.h"

#include "SPCheckManager.h"
#include "SPLocalizeManager.h"

#include "SPCashTradeManager.h"

#include "SPDebug.h"



SPWindowCashTradeWait::SPWindowCashTradeWait( INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent /*= NULL*/)
: SPWindow(WNDID_CASH_TRADE_WAIT, InstanceID, iX, iY, iCX, iCY, pParent)
{
	Init();
}


SPWindowCashTradeWait::~SPWindowCashTradeWait()
{
	Clean();
}


void SPWindowCashTradeWait::Init()
{
	m_fAccumulate = 0.0f;
	m_fDeley = 0.05f;
	
	std::string strTextureFile = "";
	strTextureFile = "DATA/INTERFACE/CONCEPT/cashshop02.png";
	g_pVideo->GetTextureMgr()->LoadTexture(strTextureFile.c_str(), &m_pAnimation);

	InitAnimationInfo();
	InitSubControl();
}


void SPWindowCashTradeWait::InitSubControl()
{
	char* CASH_SHOP1			= "DATA/INTERFACE/CONCEPT/cashshop01.png";
	char* CASH_SHOP2			= "DATA/INTERFACE/CONCEPT/cashshop02.png";
	char* CASH_SHOP3			= "DATA/INTERFACE/CONCEPT/cashshop03.png";
	char* CASH_SHOP4			= "DATA/INTERFACE/CONCEPT/cashshop04.png";
	//char* BG_IMAGE_FILE0		= "DATA/INTERFACE/CONCEPT/UI_WinBG_000.PNG";
	//char* OBJECT_IMAGE_FILE0	= "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG";
	//char* OBJECT_IMAGE_FILE1	= "DATA/INTERFACE/CONCEPT/UI_Object_001.PNG";
	//char* BUTTON_IMAGE_FILE0	= "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG";

	SPWindowStatic* pStatic		= NULL;
	SPWindowStatic* pSkin		= NULL;
	SPWindowStatic* pCenter		= NULL;
	SPWindowButton* pButton		= NULL;	

	//WIID_CASH_TRADE_WAIT_LEFT
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_WAIT_LEFT, 0, 0, 10, 134, this);
	pStatic->SetImage(CASH_SHOP3, 482, 64);
	pStatic->Show();

	//WIID_CASH_TRADE_WAIT_CENTER
	int iCenterX = 10;
	pCenter = new SPWindowStatic(WIID_CASH_TRADE_WAIT_CENTER, 10, 0, 305, 134, this);
	pCenter->SetImage(CASH_SHOP3, 495, 64);
	pCenter->SetSrcSize(3, 134);
	pCenter->Show();	

	////WIID_CASH_TRADE_WAIT_CANCEL
	//pButton = new SPWindowButton(WIID_CASH_TRADE_WAIT_CANCEL, 0, 0, 66, 17, pCenter);
	//pButton->SetImage		(CASH_SHOP2, 389,	280); //일반
	//pButton->SetImageHit	(CASH_SHOP2, 389,	298); //on	
	//pButton->SetImagePush	(CASH_SHOP2, 389,	316); //push
	//pButton->SetImageDisable(CASH_SHOP2, 389,	334); //disable
	//pButton->Show();

	//WIID_CASH_TRADE_WAIT_ITEMIMG
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_WAIT_ITEMIMG, 16 - iCenterX, 32, 32, 32, pCenter);
	pStatic->SetImage(CASH_SHOP2, 292, 56);
	pStatic->Show();
	
	//WIID_CASH_TRADE_WAIT_CUSTOMERIMG
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_WAIT_CUSTOMERIMG, 272 - iCenterX, 32, 30, 32, pCenter);
	pStatic->SetImage(CASH_SHOP2, 481, 352);
	pStatic->Show();
	
	//WIID_CASH_TRADE_WAIT_HELP
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_WAIT_HELP, 8, 83, 303, 29, this);
	pStatic->SetImage(CASH_SHOP3, 1, 28);
	pStatic->Show();


	//WIID_CASH_TRADE_WAIT_RIGHT
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_WAIT_RIGHT, 315, 0, 10, 134, this);
	pStatic->SetImage(CASH_SHOP3, 501, 64);
	pStatic->Show();
}


void SPWindowCashTradeWait::InitAnimationInfo()
{
	SetRect(&m_rtAniSrc[0], 	343, 82, 355, 94);
	SetRect(&m_rtAniSrc[1], 	356, 82, 368, 94);
	SetRect(&m_rtAniSrc[2], 	369, 82, 381, 94);
	SetRect(&m_rtAniSrc[3], 	356, 52, 368, 64);
	SetRect(&m_rtAniSrc[4], 	369, 52, 381, 64);
	
	SetRect(&m_rtAniSrc[5], 	369, 52, 381, 64);
	SetRect(&m_rtAniSrc[6], 	356, 52, 368, 64);
	SetRect(&m_rtAniSrc[7], 	369, 82, 381, 94);
	SetRect(&m_rtAniSrc[8], 	356, 82, 368, 94);
	SetRect(&m_rtAniSrc[9], 	343, 82, 355, 94);	
	
	SetRect(&m_rtAniDesc[0], 	m_iAX + 50, m_iAY + 40, m_iAX + 50 + 12, m_iAY + 40 + 12);
	SetRect(&m_rtAniDesc[1], 	m_iAX + 66, m_iAY + 32, m_iAX + 66 + 12, m_iAY + 32 + 12);
	SetRect(&m_rtAniDesc[2], 	m_iAX + 82, m_iAY + 28, m_iAX + 82 + 12, m_iAY + 28 + 12);
	SetRect(&m_rtAniDesc[3], 	m_iAX + 98, m_iAY + 27, m_iAX + 98 + 12, m_iAY + 27 + 12);
	SetRect(&m_rtAniDesc[4], 	m_iAX + 114, m_iAY + 28, m_iAX + 114 + 12, m_iAY + 28 + 12);
	SetRect(&m_rtAniDesc[5], 	m_iAX + 130, m_iAY + 32, m_iAX + 130 + 12, m_iAY + 32 + 12);
	SetRect(&m_rtAniDesc[6], 	m_iAX + 146, m_iAY + 37, m_iAX + 146 + 12, m_iAY + 37 + 12);
	SetRect(&m_rtAniDesc[7], 	m_iAX + 162, m_iAY + 42, m_iAX + 162 + 12, m_iAY + 42 + 12);
	SetRect(&m_rtAniDesc[8], 	m_iAX + 178, m_iAY + 47, m_iAX + 178 + 12, m_iAY + 47 + 12);
	SetRect(&m_rtAniDesc[9], 	m_iAX + 194, m_iAY + 51, m_iAX + 194 + 12, m_iAY + 51 + 12);
	SetRect(&m_rtAniDesc[10], 	m_iAX + 210, m_iAY + 52, m_iAX + 210 + 12, m_iAY + 52 + 12);
	SetRect(&m_rtAniDesc[11], 	m_iAX + 226, m_iAY + 51, m_iAX + 226 + 12, m_iAY + 51 + 12);
	SetRect(&m_rtAniDesc[12], 	m_iAX + 242, m_iAY + 47, m_iAX + 242 + 12, m_iAY + 47 + 12);
	SetRect(&m_rtAniDesc[13], 	m_iAX + 258, m_iAY + 39, m_iAX + 258 + 12, m_iAY + 39 + 12);	
	
	for(int i = 0; i < WAITING_FRAME; i++) {
		m_AniInfo[i].Clear();
	}
}


void SPWindowCashTradeWait::ResetAnimation()
{
	for(int i = 0; i < WAITING_FRAME; i++) {
		m_AniInfo[i].Clear();
	}

	m_AniInfo[0].m_iAniStatus = WAIT_ANI_STATUS_PLAY;
}


void SPWindowCashTradeWait::Clean()
{
	SAFE_RELEASE(m_pAnimation);
	SPWindow::Clean();
}


void SPWindowCashTradeWait::Show(bool bWithChild /*= true*/)
{
	//g_pInterfaceManager->ShowWindow(WIID_ITEM		, 	SP_HIDE);	
	//g_pInterfaceManager->ShowWindow(WIID_EQUIP		, 	SP_HIDE);
	//g_pInterfaceManager->ShowWindow(WIID_SKILL		, 	SP_HIDE);
	//g_pInterfaceManager->ShowWindow(WIID_STATUS		, 	SP_HIDE);
	//g_pInterfaceManager->ShowWindow(WIID_QUEST		, 	SP_HIDE);
	//g_pInterfaceManager->ShowWindow(WIID_TEAM		, 	SP_HIDE);
	////g_pInterfaceManager->ShowWindow(WIID_MENU		, 	SP_HIDE);
	//g_pInterfaceManager->ShowWindow(WIID_PC_INFO	, 	SP_HIDE);
	//g_pInterfaceManager->ShowWindow(WIID_CHANNEL_CHANGE		, 	SP_HIDE);
	//g_pInterfaceManager->ShowWindow(WIID_FRIEND		, 	SP_HIDE);
	//g_pInterfaceManager->ShowWindow(WIID_OPTION		,	SP_HIDE);
	//g_pInterfaceManager->ShowWindow(WIID_HELP		,	SP_HIDE);
	//g_pInterfaceManager->ShowWindow(WIID_WORLDMAP	,	SP_HIDE);
	//g_pInterfaceManager->ShowWindow(WIID_WAYPOINT	,	SP_HIDE);
	//g_pInterfaceManager->ShowWindow(WIID_MICROPHONE	,	SP_HIDE);
	//g_pInterfaceManager->ShowWindow(WIID_EXIT		,	SP_HIDE);

	//[2006/7/12] - 하단 메뉴 비활성
	SPWindow* pWindow = NULL;
	pWindow = g_pInterfaceManager->FindWindow(WIID_MENU);
	if(pWindow) {
		pWindow->SPSendMessage(SPIM_MENU_POPUP, WPARAM(0), LPARAM(0));
		pWindow->SetEnable(false);
	}

	g_pInterfaceManager->ForceReply();

	//g_pInterfaceManager->SetModalWindow(this);	
	//g_pInterfaceManager->SetFocusWindow(this);
	
	SPWindow::Show(bWithChild);
	
	ResetAnimation();
}


void SPWindowCashTradeWait::Hide(bool bSendServer /*= true*/)
{
	//g_pInterfaceManager->SetFocusWindow(NULL);
	//g_pInterfaceManager->SetModalWindow(NULL);
	
	//[2006/7/12] - 하단 메뉴 활성
	SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_MENU);
	if(pWindow) {
		pWindow->SetEnable(true);

		////
		//if(g_pGOBManager->GetLocalPlayer()) {
		//	int iStage = g_pGOBManager->GetLocalPlayer()->GetCurStageID();
		//	int iMapGroup = g_pGOBManager->GetLocalPlayer()->GetCurMapGroupID();
		//	//[2006/3/17] - 이벤트 맵 예외처리 채널이동 버튼을 막는다
		//	SPStage* pStage = g_StageManager.GetCurStage(iStage);
		//	if(pStage) {
		//		SPMapGroup* pMapGroup = pStage->GetMapGroup(iMapGroup);
		//		if(pMapGroup && pMapGroup->IsMapGroupType(GROUP_TYPE_EVENT)) {
		//			g_pInterfaceManager->SPChildWndSendMessage(WIID_EXIT, SPIM_EXIT_CHANNEL_ENABLE, 0, 0);
		//		}
		//		else {
		//			g_pInterfaceManager->SPChildWndSendMessage(WIID_EXIT, SPIM_EXIT_CHANNEL_ENABLE, 1, 0);
		//		}

		//		if(SPLocalizeManager::GetInstance()->IsEnableValue(LV_CASHSHOP) && 
		//			pMapGroup && pMapGroup->IsMapGroupType(GROUP_TYPE_CASH)) {
		//				g_pInterfaceManager->SPChildWndSendMessage(WIID_MENU, SPIM_SET_CASH_ENABLE, 1, 0);
		//			}
		//		else {
		//			g_pInterfaceManager->SPChildWndSendMessage(WIID_MENU, SPIM_SET_CASH_ENABLE, 0, 0);
		//		}
		//	}
		//}		
	}

	SPWindow::Hide(bSendServer);
}


void SPWindowCashTradeWait::Process(float fTime)
{
	if(!m_bShow) 
		return;
	
	SPWindow::Process(fTime);

	ProcessAnimation(fTime);
}


void SPWindowCashTradeWait::ProcessAnimation(float fTime)
{
	m_fAccumulate += fTime;
	if( m_fAccumulate > m_fDeley) {
		m_fAccumulate -= m_fDeley;

		int iNext = 0;
		for(int i = 0; i < WAITING_FRAME; i++) {
			if(m_AniInfo[i].m_iAniStatus == WAIT_ANI_STATUS_PLAY) {
				if(m_AniInfo[i].m_iSrcIndex < WAITING_SRC) {
					m_AniInfo[i].m_iSrcIndex++;
					
					if(m_AniInfo[i].m_iSrcIndex == 1) {
						iNext = i + 1;
					}
				}
				else {
					m_AniInfo[i].m_iAniStatus = WAIT_ANI_STATUS_END;
				}
			}
		}

		if(iNext < WAITING_FRAME) {
			m_AniInfo[iNext].m_iAniStatus = WAIT_ANI_STATUS_PLAY;
		}

		if(m_AniInfo[WAITING_FRAME-1].m_iAniStatus == WAIT_ANI_STATUS_END) {
			ResetAnimation();
			SPCashTradeManager::GetInstance()->SetWaitAniComplete();
		}
	}
}


void SPWindowCashTradeWait::Render(float fTime)
{
	if(!m_bShow)
		return;
	SPWindow::Render(fTime);	

	if(m_pAnimation){
		for(int i = 0; i < WAITING_FRAME; i++) {
			if(m_AniInfo[i].m_iAniStatus == WAIT_ANI_STATUS_PLAY) {
				if(m_AniInfo[i].m_iSrcIndex < WAITING_SRC)
					m_pAnimation->RenderTexture(&m_rtAniDesc[i], &m_rtAniSrc[m_AniInfo[i].m_iSrcIndex]);
			}			
		}
	}

////
//#ifdef _DEBUG	
//	RECT rtRect;	
//	SPWindow* pwindow = Find(WIID_CASH_TRADE_WAIT_CUSTOMERIMG, true);
//	if(pwindow) {
//		g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
//		pwindow->GetWindowRect(rtRect);
//		g_pVideo->GetSysLine()->DrawRect(rtRect);
//		g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
//	}
//#endif
}



SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowCashTradeWait )

SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_CASH_TRADE_WAIT_CANCEL,			OnCancel	)

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)



int SPWindowCashTradeWait::OnCancel(WPARAM wParam, LPARAM lParam)
{
	//Hide();
	return 1;
}





