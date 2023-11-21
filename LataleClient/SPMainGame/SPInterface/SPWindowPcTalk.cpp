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

#include "Packet.h"

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
#include "SPGuildArchive.h"
#include "SPPlayer.h"
#include "SPGOBManager.h"

////
//#include "SPStage.h"						//[2005/6/10] - jinhee
//#include "SPStageManager.h"

#include "SPWindow.h"
#include "SPWindowButton.h"
#include "SPWindowStatic.h"

#include "SPEventDEF.h"
#include "SPEventNpcHandler.h"
#include "SPEventManager.h"

#include "SPMainGameManager.h"
#include "SPStoryManager.h"
#include "SPWayPointManager.h"

#include "SPLoveManager.h"
#include "SPCheckManager.h"

#include "SPWindowNpcTalk.h"
#include "SPWindowPcTalk.h"
#include "SPLocalizeManager.h"

#include "SPUtil.h"

#include "SPDebug.h"



SPWindowPcTalk::SPWindowPcTalk(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent /*= NULL*/)
: SPWindow(WNDID_PCTALK, InstanceID, iX, iY, iCX, iCY, pParent)
, m_pWindowSkin(NULL)
, m_pSavePoint(NULL)
{
	Init();
}


SPWindowPcTalk::~SPWindowPcTalk()
{
	Clean();
}


void SPWindowPcTalk::Init()
{	
	std::string strTextureFile = "";
	strTextureFile = "DATA/INTERFACE/CONCEPT/CUTUI001.PNG";
	g_pVideo->GetTextureMgr()->LoadTexture(strTextureFile.c_str(), &m_pWindowSkin);

	SetRect(&m_rtSkinSrc, 0, 312, 358, 512);

	m_bFocusLose			= false;
	strTextureFile = "DATA/INTERFACE/CONCEPT/CUTUI003.PNG";
	g_pVideo->GetTextureMgr()->LoadTexture(strTextureFile.c_str(), &m_pFocusLoseSkin);
	SetRect(&m_rtFocusLoseSkinSrc, 0, 157, 179, 258);
	
	GetWindowRect(m_rtSkinBaseDest);

	m_iEndHeight	= m_iSY;
	m_iEndWidth		= m_iSX;

	m_fAccumulateRender		= 0.0f;
	m_fCurDelay				= 0.01f;

	InitSubControl();
}


void SPWindowPcTalk::InitSubControl()
{
	SPWindowStatic* pStatic = NULL;
	SPWindowButton* pButton = NULL;

	char* IMAGE_FILE1 = "DATA/INTERFACE/CONCEPT/CUTUI001.PNG";
	char* IMAGE_FILE2 = "DATA/INTERFACE/CONCEPT/CUTUI002.PNG";


	//WIID_PCTALK_PCNAME	
	pStatic = new SPWindowStatic(WIID_PCTALK_PCNAME, 23, 39, 237, 21, this);
	pStatic->SetFont(FONT_14_BOLD);
	pStatic->SetFontColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	pStatic->SetFontShadowColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	pStatic->SetFontShadow(true);
	pStatic->SetFormat(DT_LEFT | DT_VCENTER);
	pStatic->SetWindowText("PC Name");
	pStatic->Hide();
	m_pName = pStatic;
	
	//WIID_PCTALK_LINE
	pStatic = new SPWindowStatic(WIID_PCTALK_LINE, 0, 67, 147, 10, this);
	pStatic->SetImage(IMAGE_FILE2, 181, 329);
	pStatic->Hide();
	
	//WIID_PCTALK_MESSAGE	
	pStatic = new SPWindowStatic(WIID_PCTALK_MESSAGE, 17, 83, 296, 32, this);
	pStatic->SetFont(FONT_14_BOLD);
	pStatic->SetFontColor(D3DXCOLOR(0.43f, 0.23f, 0.29f, 1.0f));
	pStatic->SetFormat(DT_LEFT | DT_TOP);
	pStatic->SetWindowText("PC Message");
	pStatic->Hide();
	m_pMessage = pStatic;

	//WIID_PCTALK_MESSAGE_L
	pStatic = new SPWindowStatic(WIID_PCTALK_MESSAGE_L, 17, 83, 296, 99, this);
	pStatic->SetFont(FONT_14_BOLD);
	pStatic->SetFontColor(D3DXCOLOR(0.43f, 0.23f, 0.29f, 1.0f));
	pStatic->SetFormat(DT_LEFT | DT_TOP | DT_WORDBREAK);
	pStatic->SetMultiLine(true, 3);
	pStatic->SetWindowText("PC Message");
	pStatic->Hide();
	m_pLMessage = pStatic;	

	// Talk button 1
	pButton  = new SPWindowButton(WIID_PCTALK_BUTTON + PC_MENU_0, 8, 123, 266, 16, this);
	pButton->SetImageHit	(IMAGE_FILE2, 181,	312); //on	
	pButton->SetImagePush	(IMAGE_FILE2, 181,	312); //push
	pButton->SetFormat( DT_LEFT | DT_VCENTER);
	pButton->Hide();	
	m_pButton[PC_MENU_0] = pButton;
	pButton = NULL;	

	pStatic = new SPWindowStatic(WIID_PCTALK_MENU + PC_MENU_0, 32, 125, 270, 12, this);
	pStatic->SetFont(FONT_12_BOLD);
	pStatic->SetFontColor(D3DXCOLOR(0.61f, 0.17f, 0.29f, 1.0f));
	pStatic->SetFormat(DT_LEFT | DT_VCENTER);
	m_pMenu[PC_MENU_0] = pStatic;	

	// Talk button 2
	pButton  = new SPWindowButton(WIID_PCTALK_BUTTON + PC_MENU_1, 8, 138, 266, 16, this);
	pButton->SetImageHit	(IMAGE_FILE2, 181,	312); //on	
	pButton->SetImagePush	(IMAGE_FILE2, 181,	312); //push
	pButton->SetFormat( DT_LEFT | DT_VCENTER);
	pButton->Hide();	
	m_pButton[PC_MENU_1] = pButton;
	pButton = NULL;	


}


void SPWindowPcTalk::Clean()
{
	SAFE_RELEASE(m_pFocusLoseSkin);
	SAFE_RELEASE(m_pWindowSkin);
	SPWindow::Clean();
}


//void SPWindowPcTalk::SetFocus()
//{
//
//}


void SPWindowPcTalk::Show(bool bWithChild /*= true*/)
{
	m_iTalkFadeMode = TALK_FADE_IN;

	//g_pEventManager->GetEventHandler()->SetNpcTalkType(NPC_TALK_TYPE_);//[2007/9/12]-jinhee

	m_fAccumulateRender = 0.0f;
	m_iMoveSkinHeight = 0;
	m_iMoveSkinWidth = 0;
	m_iSkinMoveX = 64;
	m_iSkinMoveY = 42;
	m_iSkinMoveUnitX = 1;
	m_iSkinMoveUnitY = 1;

	m_fFadeAlpha = 0.0f;
	m_fFadeAlphaUnit = 0.07f;
	

}


void SPWindowPcTalk::ShowComplete()
{
	SPWindow::Show(true);	
	SetFocusLose(false);	
	
	if(g_pEventManager->GetCutInState() == CUT_IN_STATE_SCROLL_END){		
		SPNpc* pNpc = g_pEventManager->GetEventHandler()->GetNpcData();

		if(pNpc == NULL || pNpc->m_iNpcID == 0) {
			return;
		}

		m_pMessage->Show();
		m_pLMessage->Hide();
	}	
	else if(g_pEventManager->GetCutInState() == CUT_IN_STATE_TALK) {		
		if(m_pNpcTalk && m_pNpcTalk->m_iID) {
			if(m_pNpcTalk->m_iBoxPos == NPC_TALK_WINDOW_PC){
				if(m_pNpcTalk->m_iTalkType == TALK_TYPE_NORMAL) {
					m_pMessage->Hide();
					m_pLMessage->Show();
				}
				else if(m_pNpcTalk->m_iTalkType == TALK_TYPE_SELECT) {
					m_pMessage->Show();
					m_pLMessage->Hide();
				}
			}			
		}
	}

}


void SPWindowPcTalk::Hide(bool bSendServer /*= true*/)
{
	if(m_bShow)
		SPWindow::Hide(bSendServer);

	if(m_iTalkFadeMode == TALK_FADE_STOP)
		return;

	m_iTalkFadeMode = TALK_FADE_OUT;
	return;

	//SPWindow::Hide(bSendServer);
}


void SPWindowPcTalk::Process(float fTime)
{
	if(g_pEventManager->GetCutInState() > CUT_IN_STATE_NULL) {
		if(m_iTalkFadeMode != TALK_FADE_STOP) {
			m_fAccumulateRender += fTime;
			if( m_fAccumulateRender > m_fCurDelay) {
				m_fAccumulateRender -= m_fCurDelay;		

				if(m_iTalkFadeMode == TALK_FADE_IN) {
					m_iSkinMoveY -= m_iSkinMoveUnitY;				

					if(m_iSkinMoveY < 6)
						m_iSkinMoveY = 6;

					m_iMoveSkinHeight += m_iSkinMoveY;
					if(m_iMoveSkinHeight > m_iEndHeight)
						m_iMoveSkinHeight = m_iEndHeight;					

					m_iSkinMoveX -= m_iSkinMoveUnitX;
					if(m_iSkinMoveX < 4)
						m_iSkinMoveX = 4;

					m_iMoveSkinWidth += m_iSkinMoveX;
					if(m_iMoveSkinWidth >= m_iEndWidth)
						m_iMoveSkinWidth = m_iEndWidth;

					m_fFadeAlpha += m_fFadeAlphaUnit;
					if(m_fFadeAlpha > 1.0f)
						m_fFadeAlpha = 1.0f;

					if(m_iMoveSkinHeight == m_iEndHeight && m_iMoveSkinWidth == m_iEndWidth) {
						m_iTalkFadeMode = TALK_FADE_STOP;
						m_fFadeAlpha = 1.0f;
						ShowComplete();
					}
				}
				else if(m_iTalkFadeMode == TALK_FADE_OUT) {
					m_iSkinMoveUnitY *= 2;
					m_iSkinMoveY += m_iSkinMoveUnitY;
					if(m_iSkinMoveY > 32)
						m_iSkinMoveY = 32;

					m_iMoveSkinHeight -= m_iSkinMoveY;				
					if(m_iMoveSkinHeight < 0)
						m_iMoveSkinHeight = 0;
					
					m_iSkinMoveUnitX *= 2;
					m_iSkinMoveX += m_iSkinMoveUnitX;
					if(m_iSkinMoveX > 64)
						m_iSkinMoveX = 64;			

					m_iMoveSkinWidth -= m_iSkinMoveX;
					if(m_iMoveSkinWidth < 0)
						m_iMoveSkinWidth = 0;

					m_fFadeAlpha -= m_fFadeAlphaUnit;
					if(m_fFadeAlpha < 0.0f)
						m_fFadeAlpha = 0.0f;

					if(m_iMoveSkinHeight == 0 && m_iMoveSkinWidth == 0) {
						m_iTalkFadeMode = TALK_FADE_STOP;
						m_fFadeAlpha = 0.0f;
						//SPWindow::Hide(true);
						Hide();
					}
				}			

				SetRect(&m_rtSkinDest, m_rtSkinBaseDest.left, m_rtSkinBaseDest.bottom - m_iMoveSkinHeight, m_rtSkinBaseDest.left + m_iMoveSkinWidth, m_rtSkinBaseDest.bottom);
				//SetRect(&m_rtSkinDest, m_rtSkinBaseDest.right - m_iMoveSkinWidth, m_rtSkinBaseDest.bottom - m_iMoveSkinHeight, m_rtSkinBaseDest.right, m_rtSkinBaseDest.bottom);
			}
		}
	}

	if(IsShow() == false) {
		return;
	}
	SPWindow::Process(fTime);
}


void SPWindowPcTalk::Render(float fTime)
{
	if(g_pEventManager->GetCutInState() > CUT_IN_STATE_NULL && m_pWindowSkin) {
		if(m_iTalkFadeMode != TALK_FADE_STOP) {		
			g_pVideo->Flush();
			m_pWindowSkin->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fFadeAlpha));
			m_pWindowSkin->RenderTexture(&m_rtSkinDest, &m_rtSkinSrc);
			m_pWindowSkin->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}

		if(IsShow() == false) {
			return;
		}

		g_pVideo->Flush();
		m_pWindowSkin->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fFadeAlpha));
		m_pWindowSkin->RenderTexture(&m_rtSkinDest, &m_rtSkinSrc);
		m_pWindowSkin->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		if(m_iTalkFadeMode == TALK_FADE_STOP) {
			g_pVideo->Flush();
			SPWindow::Render(fTime);
			
			if(m_bFocusLose && m_pFocusLoseSkin){
				g_pVideo->Flush();
				m_pFocusLoseSkin->RenderTexture(&m_rtSkinDest, &m_rtFocusLoseSkinSrc);
			}
		}
	
//
#ifdef _DEBUG
		RECT rtRect = {0, 0, 0, 0};		
		////
		//if(m_pLMessage && m_pLMessage->IsShow()){
		//	m_pLMessage->GetWindowRect(rtRect);
		//	g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		//	g_pVideo->GetSysLine()->DrawRect(rtRect);
		//	g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));			
		//}

		for(int i = 0; i < PC_MENU_MAX; i++) {
			if(m_pMenu[i] && m_pMenu[i]->IsShow()) {
				m_pMenu[i]->GetWindowRect(rtRect);
				g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
				g_pVideo->GetSysLine()->DrawRect(rtRect);
				g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}
#endif
	}
}


void SPWindowPcTalk::ClearMenuText(bool bMenu)
{
	m_pMessage->SetWindowText(NULL);
	m_pLMessage->SetWindowText(NULL);
	for(int i = 0; i < MAX_NPC_EVENT_TYPE; i++) {
		m_pMenu[i]->SetEnable(false);
		m_pMenu[i]->SetWindowText(NULL);
		m_pMenu[i]->Hide();
		m_pButton[i]->SetEnable(false);
		m_pButton[i]->Hide();
	}

	
	if(bMenu) {
		m_pLMessage->Hide();
	}
	else {
		m_pMessage->Hide();
	}
}


int SPWindowPcTalk::SetSelectItem(int iItem)
{
	if(g_pEventManager->GetCurEvent()->eventType == EVENT_SAVEPOINT)
		return 1;
	
	if(g_pEventManager->GetCutInState() == CUT_IN_STATE_SAVEPOINT && g_pEventManager->GetEventHandler()->GetNpcID() == 0) {			//SavePoint의 경우
		return -1;
	}
	
	//NpcData가 없는 경우에 
	if(g_pEventManager->GetCurEvent()->eventType != EVENT_CORE && g_pEventManager->GetEventHandler()->GetNpcID() == 0 && g_pEventManager->GetEventHandler()->GetNpcData() == NULL)
		return 1;	
	
	m_iSelectItem = iItem + 1;

	DXUTOutputDebugString("SPWindowNpcTalk select Itme[%d] - Item[%d]\n", m_iSelectItem, iItem);

	if(g_pEventManager->GetCutInState() == CUT_IN_STATE_SCROLL_END) {
		if(g_pEventManager->GetEventHandler()->GetNpcData()->GetNpcEventType(iItem) == NPC_EVENT_TYPE_TALK) {
			DXUTOutputDebugString("\tButton[0] is NPC_EVENT_TYPE_TALK\n");

			//SetNpcTalkData(g_pEventManager->GetEventHandler()->GetNpcData()->m_iTalk);
			SetNpcTalkData(g_pEventManager->GetEventHandler()->GetNpcData()->GetNpcEventTypeID(iItem));			
			m_iTalkState = TALK_STATE_START;
		}
		else if(g_pEventManager->GetEventHandler()->GetNpcData()->GetNpcEventType(iItem) == NPC_EVENT_TYPE_TRADE) {
			DXUTOutputDebugString("\tButton[0] is NPC_EVENT_TYPE_TRADE\n");
			//SetNpcShopData(g_pEventManager->GetEventHandler()->GetNpcData()->m_iTrade);
			SetNpcShopData(g_pEventManager->GetEventHandler()->GetNpcData()->GetNpcEventTypeID(iItem));
			m_iTalkState = TALK_STATE_NULL;
		}
		else if(g_pEventManager->GetEventHandler()->GetNpcData()->GetNpcEventType(iItem) == NPC_EVENT_TYPE_STORAGE) {
			SetNpcStorage(g_pEventManager->GetEventHandler()->GetNpcData()->GetNpcEventTypeID(iItem));
			m_iTalkState = TALK_STATE_NULL;
		}
		else if(g_pEventManager->GetEventHandler()->GetNpcData()->GetNpcEventType(iItem) == NPC_EVENT_TYPE_QUEST) {
			SetNpcQuest(g_pEventManager->GetEventHandler()->GetNpcData()->GetNpcEventTypeID(iItem));
			m_iTalkState = TALK_STATE_NULL;
		}
		else if(g_pEventManager->GetEventHandler()->GetNpcData()->GetNpcEventType(iItem) == NPC_EVENT_TYPE_GIFT) {
			SetNpcGift(g_pEventManager->GetEventHandler()->GetNpcData()->GetNpcEventTypeID(iItem));
			m_iTalkState = TALK_STATE_NULL;
		}
		else if(g_pEventManager->GetEventHandler()->GetNpcData()->GetNpcEventType(iItem) == NPC_EVENT_TYPE_GUILD_CREATE) {
			SetNpcGuildCreate(g_pEventManager->GetEventHandler()->GetNpcData()->GetNpcEventTypeID(iItem));			
			m_iTalkState = TALK_STATE_NULL;
		}
		else if(g_pEventManager->GetEventHandler()->GetNpcData()->GetNpcEventType(iItem) == NPC_EVENT_TYPE_GUILD_DISSOLVE) {
			SetNpcGuildDissolve(g_pEventManager->GetEventHandler()->GetNpcData()->GetNpcEventTypeID(iItem));			
			m_iTalkState = TALK_STATE_NULL;
		}
		else if(g_pEventManager->GetEventHandler()->GetNpcData()->GetNpcEventType(iItem) == NPC_EVENT_TYPE_GUILD_ENTER) {
			SetNpcGuildEnter();
			m_iTalkState = TALK_STATE_NULL;
		}
		else if(g_pEventManager->GetEventHandler()->GetNpcData()->GetNpcEventType(iItem) == NPC_EVENT_TYPE_PORTAL) {
			SetNpcPortal(g_pEventManager->GetEventHandler()->GetNpcData()->GetNpcEventTypeID(iItem));
			m_iTalkState = TALK_STATE_NULL;
		}
		else if(g_pEventManager->GetEventHandler()->GetNpcData()->GetNpcEventType(iItem) == NPC_EVENT_TYPE_CHANGE) {
			SetNpcEvent(g_pEventManager->GetEventHandler()->GetNpcData()->GetNpcEventTypeID(iItem));
			m_iTalkState = TALK_STATE_NULL;
		}
		else if(g_pEventManager->GetEventHandler()->GetNpcData()->GetNpcEventType(iItem) == NPC_EVENT_TYPE_GUILD_LEVELUP) {
			SetNpcGuildLevelUp(g_pEventManager->GetEventHandler()->GetNpcData()->GetNpcEventTypeID(iItem));
			m_iTalkState = TALK_STATE_NULL;
		}
		else if(g_pEventManager->GetEventHandler()->GetNpcData()->GetNpcEventType(iItem) == NPC_EVENT_TYPE_GUILD_EMBLEM) {
			SetNpcGuildEmblemChange(g_pEventManager->GetEventHandler()->GetNpcData()->GetNpcEventTypeID(iItem));
			m_iTalkState = TALK_STATE_NULL;
		}
		else if(g_pEventManager->GetEventHandler()->GetNpcData()->GetNpcEventType(iItem) == NPC_EVENT_TYPE_PVP_RESERVE) {
			SetNpcPvpReserve(g_pEventManager->GetEventHandler()->GetNpcData()->GetNpcEventTypeID(iItem));
			m_iTalkState = TALK_STATE_NULL;
		}
		else if(g_pEventManager->GetEventHandler()->GetNpcData()->GetNpcEventType(iItem) == NPC_EVENT_TYPE_AUCTION) {
			SetNpcAuction(g_pEventManager->GetEventHandler()->GetNpcData()->GetNpcEventTypeID(iItem));
			m_iTalkState = TALK_STATE_NULL;
		}
		else if(g_pEventManager->GetEventHandler()->GetNpcData()->GetNpcEventType(iItem) == NPC_EVENT_TYPE_REOPTION) {
			SetNpcItemReOption(g_pEventManager->GetEventHandler()->GetNpcData()->GetNpcEventTypeID(iItem));
			m_iTalkState = TALK_STATE_NULL;
		}
		else if(g_pEventManager->GetEventHandler()->GetNpcData()->GetNpcEventType(iItem) == NPC_EVENT_TYPE_PARTY_PVP_RESERVE) {
			SetNpcPvpReserve(g_pEventManager->GetEventHandler()->GetNpcData()->GetNpcEventTypeID(iItem), true);
			m_iTalkState = TALK_STATE_NULL;
		}
		else if(g_pEventManager->GetEventHandler()->GetNpcData()->GetNpcEventType(iItem) == NPC_EVENT_TYPE_RANKING) //- Ranking. : W.Fairy - 2008.10.27
		{
			SetNpcRanking(iItem);
			m_iTalkState = TALK_STATE_NULL;
		}
		else if(g_pEventManager->GetEventHandler()->GetNpcData()->GetNpcEventType(iItem) == NPC_EVENT_TYPE_WEDDING_RESERVE) {			
			bool bResult = SPLoveManager::GetInstance()->SetWeddingRequest();
			
			//결과 상관없이 컷인은 해제
			g_pEventManager->SetCutInState(CUT_IN_STATE_RELEASE);
			return 1;
		}
		else if(g_pEventManager->GetEventHandler()->GetNpcData()->GetNpcEventType(iItem) == NPC_EVENT_TYPE_WEDDING_DIVORCE) {
			if(g_pGOBManager->GetLocalPlayer() == NULL)
				return 1;			
			
			bool bResult = SPLoveManager::GetInstance()->GetWeddingDivorceMsgIndex();			
			
			//결과 상관없이 컷인은 해제
			g_pEventManager->SetCutInState(CUT_IN_STATE_RELEASE);
			return 1;
		}
	}
	else if(g_pEventManager->GetCutInState() == CUT_IN_STATE_TALK) {
		if(m_pNpcTalk == NULL || m_pNpcTalk->m_iID == 0)
			return 1;		
		
		//if(g_pEventManager->GetEventHandler()->GetNpcTalkData()->m_NpcEventType[iItem].m_iTypeID == 0) {
		//	return 1;
		//}

		//if(m_iTalkState == TALK_STATE_TALK)
		//	return 1;
		if(m_pNpcTalk->m_iTalkType != TALK_TYPE_SELECT)
			return 1;

		if(m_pNpcTalk->m_iBoxPos != NPC_TALK_WINDOW_PC)
			return 1;		

		if(m_pNpcTalk->m_NpcEventType[iItem].m_iEventType != NPC_EVENT_TYPE_TALK)
			return 1;
		
		int iTalkID = m_pNpcTalk->m_NpcEventType[iItem].m_iTypeID;
		if(iTalkID == 0){
			g_pEventManager->SetCutInState(CUT_IN_STATE_RELEASE);
			return 1;
		}
		
		SetNpcTalkData(iTalkID);

		//SetNpcTalkData(g_pEventManager->GetEventHandler()->GetNpcTalkData()->m_iQuestion[iItem]);
	}

	return iItem;
}


void SPWindowPcTalk::SetNpcTalkData(int iTalkIndex /*= 0*/)
{	
	////
	m_pNpcTalk = g_pEventManager->GetEventHandler()->LoadNpcTalkDataLDT(iTalkIndex);	

	if(m_pNpcTalk == NULL)
		return;
	if(m_pNpcTalk->m_iID == 0)
		return;

	if(g_pEventManager->GetCutInState() != CUT_IN_STATE_TALK)
		g_pEventManager->SetCutInState(CUT_IN_STATE_TALK);	


}


void SPWindowPcTalk::SetNpcTalk()
{	
	if(m_pNpcTalk->m_iBoxPos == NPC_TALK_WINDOW_NPC) {
		SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_NPCTALK);
		if(pWindow) {			
			pWindow->SPSendMessage(SPIM_SET_NPCTALK);			
		}
		
		SetFocusLose(true);
	}
	else if(m_pNpcTalk->m_iBoxPos == NPC_TALK_WINDOW_PC) {
		m_bFocusLose = false;
		if(m_pNpcTalk->m_iTalkType == TALK_TYPE_NORMAL) {
			ClearMenuText(false);
			m_pLMessage->SetWindowText(m_pNpcTalk->m_strMessage.c_str());
			m_pLMessage->Show();			
		}
		else if(m_pNpcTalk->m_iTalkType == TALK_TYPE_SELECT) {
			ClearMenuText(true);
			m_pMessage->SetWindowText(m_pNpcTalk->m_strMessage.c_str());
			m_pMessage->Show();		
			
			std::string strMenu;
			strMenu.clear();
			for(int i = 0; i < MAX_NPC_EVENT_TYPE; i++) {
				if(m_pNpcTalk->m_NpcEventType[i].m_strMemu.empty() == false) {
					m_pMenu[i]->SetWindowText(m_pNpcTalk->m_NpcEventType[i].m_strMemu.c_str());
					m_pMenu[i]->SetEnable(true);
					m_pMenu[i]->Show();
					m_pButton[i]->SetEnable(true);
					m_pButton[i]->Show();
				}
			}
		}

	}
}


bool SPWindowPcTalk::SendEventPacket()
{
	//		
	CPacket Packet;
	Packet.Add((UINT32)EVENT_CS_RAISE);
	Packet.Add((UINT8)m_pSavePoint->m_iEventType);
	Packet.Add((UINT32)m_pSavePoint->m_iEventID);

	//이부분 리턴값 문제 있을수 있음...
	return g_pNetworkManager->SPPerformMessage(EVENT_CS_RAISE, 0, (LPARAM)&Packet);

	return true;
}


void SPWindowPcTalk::SetNpcShopData(int iTradeIndex /*= 0*/)
{
	//iTradeIndex 받은거로 Trade Data를 Handler가 로드할수 있도록 한뒤
	g_pEventManager->GetEventHandler()->LoadNpcShopDataLDT(iTradeIndex);

	//Cutin 을 TRADE로 변경
	g_pEventManager->SetCutInState(CUT_IN_STATE_TRADE);

	//Shop Window를 Show시긴다
	g_pInterfaceManager->SPPerformMessage(SPIM_SET_NPCSHOP_SHOW, int(CUT_IN_STATE_TRADE));
}


void SPWindowPcTalk::SetNpcStorage(int iStorageIndex /*= 0*/)
{	
	g_pEventManager->GetEventHandler()->LoadNpcStorageDataLDT(iStorageIndex);

	g_pEventManager->SetCutInState(CUT_IN_STATE_STORAGE);	

	//Shop Window를 Show시긴다
	g_pInterfaceManager->SPPerformMessage(SPIM_SET_NPCSHOP_SHOW, int(CUT_IN_STATE_STORAGE));
}


void SPWindowPcTalk::SetNpcQuest(int iQuestIndex /*= 0*/)
{
	g_pEventManager->GetEventHandler()->LoadNpcQuestDataLDT(iQuestIndex);

	g_pEventManager->SetCutInState(CUT_IN_STATE_QUEST);	

	//Shop Window를 Show시긴다
	//g_pInterfaceManager->SPPerformMessage(SPIM_SET_NPCSHOP_SHOW, int(CUT_IN_STATE_STORAGE));
	g_pInterfaceManager->SPPerformMessage(SPIM_SET_NPCSHOP_SHOW, int(CUT_IN_STATE_STORAGE));
}


void SPWindowPcTalk::SetNpcGift(int iGiftIndex /*= 0*/)
{
	g_pEventManager->GetEventHandler()->LoadNpcGiftDataLDT(iGiftIndex);
	g_pEventManager->SetCutInState(CUT_IN_STATE_GIFT);

	//Shop Window를 Show시긴다
	g_pInterfaceManager->SPPerformMessage(SPIM_SET_NPCSHOP_SHOW, int(CUT_IN_STATE_GIFT));
}


void SPWindowPcTalk::SetNpcGuildCreate(int iGuildIndex /*= 0*/)
{
	g_pEventManager->GetEventHandler()->LoadNpcGuildDataLDT(iGuildIndex);
	g_pEventManager->SetCutInState(CUT_IN_STATE_GUILD_CREATE);	

	if( !(g_pGOBManager->GetGuildArchive()->GetBasicAvailable() && SPLocalizeManager::GetInstance()->IsEnableValue(LV_GUILD)) ) {
		//길드 생성 Window 를 Show 시킨다 
		g_pInterfaceManager->SPPerformMessage(SPIM_SET_NPCSHOP_SHOW, int(CUT_IN_STATE_GUILD_CREATE));
	}	
}


void SPWindowPcTalk::SetNpcGuildDissolve(int iGuildIndex /*= 0*/)
{
	g_pEventManager->GetEventHandler()->LoadNpcGuildDataLDT(iGuildIndex);
	g_pEventManager->SetCutInState(CUT_IN_STATE_GUILD_DISSOLVE);	

	if(g_pGOBManager->GetGuildArchive()->CheckGuildDestroy() == 0) {
		//길드 해체 Window 를 Show 시킨다
		g_pInterfaceManager->SPPerformMessage(SPIM_SET_NPCSHOP_SHOW, int(CUT_IN_STATE_GUILD_DISSOLVE));
	}	
}

void SPWindowPcTalk::SetNpcGuildEnter()
{
	if( g_pSubGameManager )
		g_pSubGameManager->SPPerformMessage(SPGM_GUILD_INDUN_ENTER);

	//OnNpcTalkClose(0, 0);
	g_pEventManager->SetCutInState(CUT_IN_STATE_RELEASE);
}


void SPWindowPcTalk::SetNpcGuildLevelUp(int iGuildIndex /*= 0*/)
{
	g_pEventManager->GetEventHandler()->LoadNpcGuildDataLDT(iGuildIndex);
	g_pEventManager->SetCutInState(CUT_IN_STATE_GUILD_LEVEL_UP);
}


void SPWindowPcTalk::SetNpcGuildEmblemChange(int iGuildIndex /*= 0*/)
{
	g_pEventManager->GetEventHandler()->LoadNpcGuildDataLDT(iGuildIndex);
	g_pEventManager->SetCutInState(CUT_IN_STATE_GUILD_EMBLEM);
}


void SPWindowPcTalk::SetNpcPortal(int iPortalIndex /*= 0*/)
{
	g_pEventManager->GetEventHandler()->LoadNpcPortalDataLDT(iPortalIndex);
	g_pEventManager->SetCutInState(CUT_IN_STATE_NPC_PORTAL);
	g_pInterfaceManager->SPPerformMessage(SPIM_SET_NPCSHOP_SHOW, int(CUT_IN_STATE_NPC_PORTAL));
}


void SPWindowPcTalk::SetNpcEvent(int iEventIndex /*= 0*/)
{
	g_pEventManager->GetEventHandler()->LoadNpcEventListDataLDT(iEventIndex);
	g_pEventManager->SetCutInState(CUT_IN_STATE_NPC_CHANGE);
	g_pInterfaceManager->SPPerformMessage(SPIM_SET_NPCSHOP_SHOW, int(CUT_IN_STATE_NPC_CHANGE));
}


void SPWindowPcTalk::SetNpcPvpReserve(int iPvpIndex /*= 0*/, bool bParty /*= false*/)
{
	g_pEventManager->GetEventHandler()->LoadNpcGuildDataLDT(iPvpIndex);
	g_pEventManager->SetCutInState(CUT_IN_STATE_PVP_RESERVE);
}


void SPWindowPcTalk::SetNpcAuction(int iAuctionIndex /*= 0*/)
{
	g_pEventManager->GetEventHandler()->LoadNpcGuildDataLDT(iAuctionIndex);
	g_pEventManager->SetCutInState(CUT_IN_STATE_NPC_AUCTION);
}


void SPWindowPcTalk::SetFocusLose(bool bLose /*= false*/)
{
	m_bFocusLose = bLose;
	
	if(m_bFocusLose) {
		for(int i = 0; i < MAX_NPC_EVENT_TYPE; i++) {
			m_pButton[i]->Hide();
		}
	}
	else {
		for(int i = 0; i < MAX_NPC_EVENT_TYPE; i++) {
			m_pButton[i]->Show();
		}
	}
}


void SPWindowPcTalk::SetNpcItemReOption(int iReOptionIndex /*= 0*/)
{
	g_pEventManager->GetEventHandler()->LoadCutInTalkDataLDT(iReOptionIndex);
	g_pEventManager->SetCutInState(CUT_IN_STATE_NPC_REOPTION);
}

void SPWindowPcTalk::SetNpcRanking(int rec_Index) 
{
	//g_pEventManager->GetEventHandler()->SetNpcTalkImage(0);
	g_pEventManager->SetCutInState(CUT_IN_STATE_RANKING);
}



SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowPcTalk )

////
SPIMESSAGE_COMMAND(	SPIM_SET_HIDE_START,				OnHideStart)
SPIMESSAGE_COMMAND( SPIM_SET_NPCDATA,					OnSetNpcData)
SPIMESSAGE_COMMAND( SPIM_SET_SAVEPOINT,					OnSetSavePoint)
SPIMESSAGE_COMMAND(	SPIM_SET_NPCTALK,					OnSetNpcTalk)
SPIMESSAGE_COMMAND(	SPIM_SET_NPCTALK_ITEMSELECT,		OnNpcTalkItemSelect)
SPIMESSAGE_COMMAND(	SPIM_SET_NPCTALK_FOCUSLOSE,			OnSetFocusLose)
//SPIMESSAGE_COMMAND( SPIM_SET_SAVEPOINT,		OnSetSavePoint)
//SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_PCTALK,				OnClick)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_PCTALK_BASE,			OnClick)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_PCTALK_MESSAGE,		OnClick)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_PCTALK_MESSAGE_L,		OnClick)

SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP,	WIID_PCTALK_BUTTON,	WIID_PCTALK_BUTTON + PC_MENU_MAX,	OnItemSelect)

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)


int SPWindowPcTalk::OnHideStart(WPARAM wParam, LPARAM lParam)
{
	for(int i = 0 ; i < MAX_NPC_EVENT_TYPE ; i++){
		m_pButton[i]->Hide();
	}

	if(m_iTalkFadeMode != TALK_FADE_IN){
		if(!m_bShow)
			return 1;
	}

	
	return 1;
}


int SPWindowPcTalk::OnSetNpcData( WPARAM wParam, LPARAM lParam )
{
	DXUTOutputDebugString("SPWindowPcTalk::OnSetNpcData\n");
	
	int iCounter = 0;
	std::string strMenu;

	if(g_pEventManager->GetCutInState() != CUT_IN_STATE_NULL) {
		ClearMenuText(true);

		SPNpc* pNpc = g_pEventManager->GetEventHandler()->GetNpcData();

		if(pNpc->m_iNpcID == 0)
			return 1;		

		if(pNpc->GetNpcEventType(iCounter) > 0) {
			strMenu = pNpc->GetNpcEventMenu(iCounter);
			m_pButton[iCounter]->SetEnable(true);
			m_pButton[iCounter]->Show();
			m_pMenu[iCounter]->SetWindowText(strMenu.c_str());
		}
		else {
			m_pButton[iCounter]->Hide();
		}
		iCounter++;

		if(pNpc->GetNpcEventType(iCounter) > 0) {
			strMenu = pNpc->GetNpcEventMenu(iCounter);
			m_pButton[iCounter]->SetEnable(true);
			m_pButton[iCounter]->Show();
			m_pMenu[iCounter]->SetWindowText(strMenu.c_str());
		}
		else {
			m_pButton[iCounter]->Hide();
		}
		iCounter++;

		if(pNpc->GetNpcEventType(iCounter) > 0) {
			strMenu = pNpc->GetNpcEventMenu(iCounter);
			m_pButton[iCounter]->SetEnable(true);
			m_pButton[iCounter]->Show();
			m_pMenu[iCounter]->SetWindowText(strMenu.c_str());
		}
		else {
			m_pButton[iCounter]->Hide();
		}
		iCounter++;

		if(pNpc->GetNpcEventType(iCounter) > 0) {
			strMenu = pNpc->GetNpcEventMenu(iCounter);
			m_pButton[iCounter]->SetEnable(true);
			m_pButton[iCounter]->Show();
			m_pMenu[iCounter]->SetWindowText(strMenu.c_str());
		}
		else {
			m_pButton[iCounter]->Hide();
		}
		//m_iTalkState = TALK_STATE_MENU;
	}
	return 1;
}


int SPWindowPcTalk::OnSetSavePoint(WPARAM wParam, LPARAM lParam)
{
	m_pSavePoint = g_pEventManager->GetEventHandler()->GetSavePointData();	

	if(m_pSavePoint == NULL || m_pSavePoint->m_iEventID == 0) {
		g_pEventManager->SetCutInState(CUT_IN_STATE_RELEASE);
		return 1;
	}

	ClearMenuText(true);

	//[2007/9/21]-jinhee 이부분을 SPWindowPCTalk쪽으로 
	std::string strMsg = "";
	if(g_pResourceManager->GetGlobalString(3000001) != NULL) {
		strMsg = g_pResourceManager->GetGlobalString(3000001);
		
		m_pButton[0]->SetEnable(true);
		m_pButton[0]->Show();
		m_pMenu[0]->SetWindowText(strMsg.c_str());
	}

	strMsg = "";
	strMsg.clear();		
	if(g_pResourceManager->GetGlobalString(3000002) != NULL) {
		strMsg = g_pResourceManager->GetGlobalString(3000002);
		
		m_pButton[1]->SetEnable(true);
		m_pButton[1]->Show();
		m_pMenu[1]->SetWindowText(strMsg.c_str());
	}

	m_iTalkState = TALK_STATE_END;
	
	return 1;
}


int SPWindowPcTalk::OnSetNpcTalk(WPARAM wParam, LPARAM lParam)
{
	if(g_pEventManager->GetCutInState() != CUT_IN_STATE_NULL) {
		m_pNpcTalk = g_pEventManager->GetEventHandler()->GetNpcTalkData();
		if(m_pNpcTalk == NULL)
			return 1;
		if(m_pNpcTalk->m_iID == 0)
			return 1;

		SetNpcTalk();
	}
	return 1;
}


int SPWindowPcTalk::OnNpcTalkItemSelect	( WPARAM wParam, LPARAM lParam )
{
	int iSelect = (int)wParam;
	//
	if(g_pEventManager->GetEventHandler()->GetCutInState() == CUT_IN_STATE_NULL)
		return 1;

	switch(g_pEventManager->GetEventHandler()->GetCutInState()) {
		case CUT_IN_STATE_SAVEPOINT:
		case CUT_IN_STATE_SCROLL_END:			
		case CUT_IN_STATE_TALK:
			break;
		default :
			return 1;
	}

	//if(g_pEventManager->GetEventHandler()->GetCutInState() != CUT_IN_STATE_SAVEPOINT || 
	//	g_pEventManager->GetEventHandler()->GetCutInState() != CUT_IN_STATE_SCROLL_END ||
	//	g_pEventManager->GetEventHandler()->GetCutInState() != CUT_IN_STATE_TALK)
	//	return 1;

	switch(iSelect) {
		case PC_MENU_0:
			if(g_pEventManager->GetCutInState() == CUT_IN_STATE_SAVEPOINT && g_pEventManager->GetEventHandler()->GetNpcID() == 0) {			//SavePoint의 경우
				DXUTOutputDebugString("-\tSavePoint 0", iSelect);
				if(m_iTalkState == TALK_STATE_TALK || m_iTalkState == TALK_STATE_NULL) {
					//g_pEventManager->SetCutInState(CUT_IN_STATE_RELEASE);
					return 1;					
				}
				if( m_pSavePoint == NULL )
					return 1 ;

				g_pkStoryManger->SetStoryViewID(m_pSavePoint->m_iStoryID) ;			// Save StoryID for story view				
				//////////////////////////////////////////////////////////////////////////
				// SavePoint.ldt에서 StoryID를 찾아서 있으면 보여주고 없으면 다음으로 넘어간다.
				if( m_pSavePoint->m_iStoryID > 0 && g_pEventManager->GetWayPointManager()->IsNewWayPoint(m_pSavePoint->m_iEventID))
				{
					// BGM은 StoryBGM으로 셋팅된다.
					SendEventPacket();
					//Hide();
				}
				else
				{
					//Packet Send...
					SendEventPacket();

					//NPC Talk쪽으로 사용자가 Yes를 선택했음을 알림
					SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_NPCTALK);
					if(pWindow) {
						pWindow->SPSendMessage(SPIM_SET_SAVEPOINT, (WPARAM)1, (LPARAM)0);
					}

					//이미지 로드
					g_pEventManager->GetEventHandler()->SetNpcTalkImage(m_pSavePoint->m_MsgInfo[SAVE_POINT_YES].m_iCutInImage);
					m_iTalkState = TALK_STATE_NULL;
					SetFocusLose(true);
				}				
				return 1;				
			}
			else {
			}
			SetSelectItem(iSelect);
			break;
		case PC_MENU_1:
			if(g_pEventManager->GetCutInState() == CUT_IN_STATE_SAVEPOINT && g_pEventManager->GetEventHandler()->GetNpcID() == 0) {			//SavePoint의 경우
				DXUTOutputDebugString("-\tSavePoint 1", iSelect);
				if(m_iTalkState == TALK_STATE_TALK || m_iTalkState == TALK_STATE_NULL) {
					//g_pEventManager->SetCutInState(CUT_IN_STATE_RELEASE);
					return 1;
				}

				if( m_pSavePoint == NULL )
					return 1 ;

				//NPC Talk쪽으로 사용자가 No를 선택했음을 알림
				SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_NPCTALK);
				if(pWindow) {
					pWindow->SPSendMessage(SPIM_SET_SAVEPOINT, (WPARAM)2, (LPARAM)0);
				}

				//이미지 로드
				g_pEventManager->GetEventHandler()->SetNpcTalkImage(m_pSavePoint->m_MsgInfo[SAVE_POINT_NO].m_iCutInImage);
				m_iTalkState = TALK_STATE_NULL;
				SetFocusLose(true);
				return 1;
			}
			else {
			}

			SetSelectItem(iSelect);
			break;
		case PC_MENU_2:
			if(g_pEventManager->GetCutInState() == CUT_IN_STATE_SAVEPOINT && g_pEventManager->GetEventHandler()->GetNpcID() == 0)				//SavePoint의 경우
				return 1;

			SetSelectItem(iSelect);
			break;
		case PC_MENU_3:
			if(g_pEventManager->GetCutInState() == CUT_IN_STATE_SAVEPOINT && g_pEventManager->GetEventHandler()->GetNpcID() == 0)				//SavePoint의 경우
				return 1;

			SetSelectItem(iSelect);
			break;
	}
	return 1;
}


int SPWindowPcTalk::OnClick(WPARAM wParam, LPARAM lParam)
{

	if(g_pEventManager->GetCutInState() != CUT_IN_STATE_NULL) {
		SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_NPCTALK);
		if(pWindow) {
			pWindow->SPSendMessage(SPIM_SET_CUTIN_CLICK, (WPARAM)0, (LPARAM)0);
		}
	}
	return 1;
}


int SPWindowPcTalk::OnSetFocusLose(WPARAM wParam, LPARAM lParam)
{
	bool bLose = (bool)wParam;
	SetFocusLose(bLose);
	return 1;
}


int SPWindowPcTalk::OnItemSelect(unsigned int uiID, WPARAM wParam, LPARAM lParam)
{
	int iSelect = uiID - WIID_PCTALK_BUTTON;	
	OnNpcTalkItemSelect((WPARAM)iSelect, (LPARAM)lParam);
	return 1;
}



