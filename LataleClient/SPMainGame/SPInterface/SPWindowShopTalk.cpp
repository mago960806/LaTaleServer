
#include <vector>
#include <string>

#include "SPCommon.h"
#include "SPUtil.h"

#include "SPMainGameDEF.H"
#include "SPGameObjectDEF.h"

#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPSubGameManager.h"
#include "SPGameObject.h"
#include "SPManager.h"
#include "SPMouseCursor.h"
#include "SPIMEPool.h"
#include "SPWindowDEF.h"
#include "SPInterfaceManager.h"

#include "SPWindow.h"
#include "SPWindowButton.h"
#include "SPWindowStatic.h"

//
#include "SPMainGameDEF.H"
#include "SPSubGameManager.h"
#include "SPMainGameManager.h"
#include "SPMouseCursor.h"
#include "SPWindowDEF.h"
#include "SPInterfaceManager.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGOBModel.h"
#include "SPGOBModelUnit.h"
#include "SPMonsterModelUnit.h"
#include "SPAvatarModelUnit.h"
#include "SPGOBCluster.h"
#include "SPGameObject.h"
#include "SPGOBStatus.h"
#include "SPGOBCoordPhysics.h"
#include "SPGOBModelUnit.h"			
#include "SPPlayerEquipInfo.h"
#include "SPGOBStatus.h"			
#include "SPPlayerInvenArchive.h"	
#include "SPPlayerStatusModel.h"
#include "SPEventStatusModel.h"
#include "SPPlayerEquipInfo.h"
#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPPlayer.h"
#include "SPGOBManager.h"

//#include "SPStage.h"
//#include "SPStageManager.h"
//#include "SPStageLoader.h"

#include "SPEventDEF.h"
#include "SPEventNpcHandler.h"
#include "SPEventManager.h"

#include "SPPvpManager.h"

//
//#include "SPMainGameDEF.H"
//#include "SPSubGameManager.h"
#include "SPMainGameManager.h"
//#include "SPMouseCursor.h"
//#include "SPWindowDEF.h"
//#include "SPInterfaceManager.h"

#include "SPWindowShopTalk.h"
#include "SPUtil.h"

#include "SPDebug.h"

SPWindowShopTalk::SPWindowShopTalk(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent /*= NULL*/)
: SPWindow(WNDID_SHOPTALK, InstanceID, iX, iY, iCX, iCY, pParent)
{
	Init();
}


SPWindowShopTalk::~SPWindowShopTalk()
{
	Clean();
}

void SPWindowShopTalk::Init()
{
	m_strMessage = "";
	m_strInfo = "";

	ZeroMemory(m_szMsg, 512);
	
	//컨트롤
	SPWindow* pStatic = NULL;
	SPWindowButton* pButton;	

	SPWindow* pCenter = NULL;
	
	//pStatic = new SPWindowStatic(WIID_SHOPTALK_TOP, 0, 0, 231, 9, this);
	//pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 1,	110);
	//pStatic->Show();
	//pStatic = NULL;

	//pCenter = new SPWindowStatic(WIID_SHOPTALK_CENTER, 0, 9, 231, 80, this);
	//pCenter->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 1,	121);
	//pCenter->SetSrcSize(231, 2);
	//pCenter->Show();

	//pStatic = new SPWindowStatic(WIID_SHOPTALK_BOTTOM, 0, 89, 231, 9, this);
	//pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 1,	125);
	//pStatic->Show();
	//pStatic = NULL;

	//메시지 영역	
	pStatic = new SPWindowStatic(WIID_SHOPTALK_MESSAGE, 35, 20, 207, 62, this);
	pStatic->SetFormat( DT_LEFT | DT_WORDBREAK );
	pStatic->SetMultiLine(true, 5);
	pStatic->Show();
	m_pMessage = pStatic;
	pStatic = NULL;	

	pButton = new SPWindowButton(WIID_SHOPTALK_CLOSE, 224, 0, 34, 34, this);
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/CUTUI001.PNG", 359,	312); //일반 CUTUI001.png
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/CUTUI001.PNG", 359,	347); //on	
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/CUTUI001.PNG", 359,	382); //push
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CUTUI001.PNG", 359,	417); //disable
	pButton->Show();
	pButton = NULL;	

	//pStatic = new SPWindowStatic(WIID_SHOPTALK_TAIL, 133, 94, 22, 20, this);
	//pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 486,	406);
	//pStatic->Show();
	//pStatic = NULL;

	Hide();
}


void SPWindowShopTalk::Clean()
{
	SPWindow::Clean();

	//SAFE_RELEASE(m_pTexture);
	//m_pTexture = NULL;	
}


void SPWindowShopTalk::SetFocus()
{
	// 이윈도우에선 포커스 먹으면 안된돠~~ 
}


void SPWindowShopTalk::Show(bool bWithChild)
{
	//여기로 오면 내부 상점 대화 데이터를 업데이트한다 	
	if(g_pEventManager && g_pEventManager->GetCutInState() > CUT_IN_STATE_NULL){
		UpdateMessage();
	}	
	
	SPWindow::Show(bWithChild);
}


void SPWindowShopTalk::Hide(bool bSendServer /*= true*/)
{
	SPWindow::Hide(bSendServer);
}


void SPWindowShopTalk::Process(float fTime)
{
	SPWindow::Process(fTime);
}


void SPWindowShopTalk::Render(float fTime)
{
	if(IsShow()) {
		////
		//if(m_pTexture){
		//	m_pTexture->SetColor(RGBA(255, 255, 255, 255));
		//	m_pTexture->RenderTexture(&m_rtDestRoof, &m_rtSrcRoof);
		//	m_pTexture->RenderTexture(&m_rtDestCenter, &m_rtSrcCenter);
		//	m_pTexture->RenderTexture(&m_rtDestBottom, &m_rtSrcBottom);
		//	m_pTexture->RenderTexture(&m_rtDestTail, &m_rtSrcTail);
		//}
		//SetFormat(DT_TOP | DT_VCENTER);
	}

	SPWindow::Render(fTime);
}


void SPWindowShopTalk::RenderText()
{
	SPWindow::RenderText();
}


//////////////////////////////////////////////////////////////////////////
void SPWindowShopTalk::UpdateMessage()
{
	m_strMessage.clear();
	m_strMessage = "";
	
	//TRADE인 경우

	if(g_pEventManager->GetCutInState() == CUT_IN_STATE_TRADE) {
		SPNpcShop* pNpcShop = g_pEventManager->GetEventHandler()->GetNpcShopData();		
		
		if(pNpcShop && pNpcShop->m_strCutInMessage != "") {			
			m_strMessage = pNpcShop->m_strCutInMessage;					//[2005/11/22]
		}
		else 
			m_strMessage = "Npc Shop Message String Error";
	}
	else if(g_pEventManager->GetCutInState() == CUT_IN_STATE_STORAGE) {
		SPNpcStorage* pNpcStorage = g_pEventManager->GetEventHandler()->GetNpcStorageData();
		if(pNpcStorage && pNpcStorage->m_strCutInMessage != "") {			
			//m_strMessage = g_pResourceManager->GetGlobalString(pNpcStorage->m_iCutInMessage);			
			m_strMessage = pNpcStorage->m_strCutInMessage;
		}
		else 
			m_strMessage = "NpcStorage String Index Error";

	}
	else if(g_pEventManager->GetCutInState() == CUT_IN_STATE_QUEST) {
		SPNpcQuestList* pNpcQuest = g_pEventManager->GetEventHandler()->GetNpcQuestData();
		if(pNpcQuest && pNpcQuest->m_strMessage != "") {
			m_strMessage = pNpcQuest->m_strMessage;
		}
		else 
			m_strMessage = "NpcQuest String Index Error";
	}
	else if(g_pEventManager->GetCutInState() == CUT_IN_STATE_GIFT) {
		SPNpcGift* pNpcGift = g_pEventManager->GetEventHandler()->GetNpcGift();
		if(pNpcGift && pNpcGift->m_strMessage != "") {
			m_strMessage = pNpcGift->m_strMessage;
		}
		else 
			m_strMessage = "NpcQuest String Index Error";
	}
	else if(g_pEventManager->GetCutInState() == CUT_IN_STATE_GUILD_CREATE) {
		SPNpcGuild* pNpcGuild = g_pEventManager->GetEventHandler()->GetNpcGuild();
		if(pNpcGuild && pNpcGuild->m_strMessage != "") {
			m_strMessage = pNpcGuild->m_strMessage;
		}
		else 
			m_strMessage = "NpcGuild String Empty... ";
	}
	else if(g_pEventManager->GetCutInState() == CUT_IN_STATE_GUILD_DISSOLVE) {
		SPNpcGuild* pNpcGuild = g_pEventManager->GetEventHandler()->GetNpcGuild();
		if(pNpcGuild && pNpcGuild->m_strMessage != "") {
			m_strMessage = pNpcGuild->m_strMessage;
		}
		else 
			m_strMessage = "NpcGuild String Empty... ";
	}
	else if(g_pEventManager->GetCutInState() == CUT_IN_STATE_NPC_PORTAL) {
		SPNpcPortal* pNpcPortal = g_pEventManager->GetEventHandler()->GetNpcPortal();
		if(pNpcPortal && pNpcPortal->m_strMessage != "") {
			m_strMessage = pNpcPortal->m_strMessage;
		}
		else 
			m_strMessage = "NpcPortal String Index Error";
	}
	else if(g_pEventManager->GetCutInState() == CUT_IN_STATE_NPC_CHANGE) {
		SPNpcEventList* pNpcEvent = g_pEventManager->GetEventHandler()->GetNpcEventList();
		if(pNpcEvent && pNpcEvent->m_strMessage != "") {
			m_strMessage = pNpcEvent->m_strMessage;
		}
		else 
			m_strMessage = "NpcEvent String Index Error";
	}
	else if(g_pEventManager->GetCutInState() == CUT_IN_STATE_GUILD_LEVEL_UP) {
		SPNpcGuild* pNpcGuild = g_pEventManager->GetEventHandler()->GetNpcGuild();
		if(pNpcGuild && pNpcGuild->m_strMessage != "") {
			m_strMessage = pNpcGuild->m_strMessage;
		}
		else 
			m_strMessage = "NpcGuild String Empty... ";
	}
	else if(g_pEventManager->GetCutInState() == CUT_IN_STATE_GUILD_EMBLEM) {
		SPNpcGuild* pNpcGuild = g_pEventManager->GetEventHandler()->GetNpcGuild();
		if(pNpcGuild && pNpcGuild->m_strMessage != "") {
			m_strMessage = pNpcGuild->m_strMessage;
		}
		else 
			m_strMessage = "NpcGuild String Empty... ";
	}
	else if(g_pEventManager->GetCutInState() == CUT_IN_STATE_PVP_RESERVE) {
		//PVP에서도 대화와 이미지 정보는 길드 테이블에서 참조.
		SPNpcGuild* pNpcGuild = g_pEventManager->GetEventHandler()->GetNpcGuild();
		if(pNpcGuild && pNpcGuild->m_strMessage != "") {
			m_strMessage = pNpcGuild->m_strMessage;
		}
		else 
			m_strMessage = "PVP - NpcGuild String Empty...";
	}

	ZeroMemory(m_szMsg, 512);		
	if(g_pGOBManager->GetLocalPlayer()) {
		sprintf(m_szMsg, m_strMessage.c_str(), g_pGOBManager->GetLocalPlayer()->GetStatus()->GetGOBName());
		m_pMessage->SetWindowText(m_szMsg);
	}
	else {
		m_pMessage->SetWindowText(m_strMessage.c_str());
	}	

	//Quest인 경우
}



//////////////////////////////////////////////////////////////////////////
SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowShopTalk )
//SPIMESSAGE_COMMAND( SPIM_SET_CAMERA_POS,	OnSetCameraPos	)
//SPIMESSAGE_COMMAND( SPIM_SET_LOCAL_PLAYER_POS, OnSetPlayerPos)

SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_SHOPTALK_CLOSE	, OnShopTalkClose)

//SPIMESSAGE_COMMAND( SPIM_SET_SHOPTALK_UPDATE,	OnMessageUpdate )
SPIMESSAGE_COMMAND( SPIM_SET_SHOPTALK_MSG,		OnSetMessage	)

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)


int SPWindowShopTalk::OnShopTalkClose( WPARAM wParam, LPARAM lParam )
{
	if(g_pEventManager->GetCutInState() == CUT_IN_STATE_PVP_RESERVE &&
		g_pkPvpManager->GetPvpStatus() == PVP_STATUS_ENTERWAIT)
	{
		g_pkPvpManager->SendPVPEnterCancel();
	}
	else {
		g_pEventManager->SetCutInState(CUT_IN_STATE_RELEASE);
	}
	
	DXUTOutputDebugString("SPWindowShopTalk::CUT_IN_STATE_NULL\n");
	
	//g_pInterfaceManager->GetItemSellWindow()->SPSendMessage(SPIM_SET_ITEMSELL_RESET);

	Hide();	
	return 1;
}

////
//int SPWindowShopTalk::OnMessageUpdate(WPARAM wParam, LPARAM lParam)
//{
//	UpdateMessage();
//	return 1;
//}


int	SPWindowShopTalk::OnSetMessage(WPARAM wParam, LPARAM lParam)
{
	std::string strMessage = (char*)(wParam);
	
	if(strMessage != "") {
		m_pMessage->SetWindowText(strMessage.c_str());
	}

	return 1; 
}


////
//int SPWindowShopTalk::OnMessageShow( WPARAM wParam, LPARAM lParam )
//{
//	//if(m_bShow)
//	//	m_bShow = false;
//	//else 
//	//	m_bShow = true;
//
//	m_bShow = (bool)wParam;
//
//	if(!m_bShow) {
//		Hide();
//		m_strMessage.clear();
//		//m_strInfo.clear();
//	} else {
//		Show();	
//	}
//
//	return 1;
//}
//
//
//int SPWindowShopTalk::OnMessageTextSet( WPARAM wParam, LPARAM lParam )
//{
//	unsigned int iMessageStringID = (unsigned int)wParam;			//Tip Message
//	//unsigned int iMessageInfoID = (unsigned int)lParam;			//EventType에 따른 Message (포탈인 경우 이동, 세이브 포인트의 경우 작동)
//
//	m_strMessage = "";
//	if(iMessageStringID != 0) {
//		if(g_pResourceManager->GetGlobalString(iMessageStringID) != NULL)
//			m_strMessage = g_pResourceManager->GetGlobalString(iMessageStringID);
//		else 
//			m_strMessage = "Global String Index Error";				//Global LDT 에서 읽을수 있도록 할것
//	}	
//
//	m_pMessage->SetWindowText(m_strMessage.c_str());
//
//	m_strInfo = g_pResourceManager->GetGlobalString(3000000);
//	m_pInfo->SetWindowText(m_strInfo.c_str());
//
//	return 1;
//}




