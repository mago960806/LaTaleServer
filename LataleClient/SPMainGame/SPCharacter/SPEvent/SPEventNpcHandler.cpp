#include "SPCommon.h"
#include "SPUtil.h"
//#include "PacketHandler.h"
#include "SPManagerDef.h"
#include "SPManager.h"
//#include "SPNetworkDEF.h"
//#include "SPNetworkManager.h"

#include "SPMainGameDEF.H"
#include "SPSubGameManager.h"
#include "SPMainGameManager.h"
#include "SPMouseCursor.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPMouseCursor.h"
#include "SPIMEPool.h"
#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"
#include "SPWindowNpcTalk.h"

#include "SPRenderModel.h"

#include "SPFXDEF.h"
#include "SPFXModel.h"
#include "SPFXUnit.h"
#include "SPFXGroupUnit.h"
#include "SPFXArchive.h"

#include "SPEventDEF.h"
#include "SPGameObjectDEF.h"
#include "SPGameObject.h"
#include "SPEvent.h"
#include "SPEventArchive.h"
#include "SPEventManager.h"

#include "SPQuestManager.h"

#include "SPGiftManager.h"

#include "SPDebug.h"

#include "SPEventNpcHandler.h"

extern SPMouseCursor* g_pMouseCursor;


////////////////////////////////////////////////////////////////////////// SPNpcPortal
void SPNpcPortal::Clear()
{
	m_iItemID			= 0;
	m_iCutInImage		= 0;
	m_strMessage.clear();

	std::vector<SPNpcPortalInfo*>::iterator iter = m_vpNpcPortal.begin();
	for(; iter != m_vpNpcPortal.end(); ++iter) {
		SAFE_DELETE((*iter));
	}

	m_vpNpcPortal.clear();
}


void SPNpcPortal::SetNpcPortal(int iItemID, int iCutInImage, std::string strMsg, int iNpcID)
{
	Clear();
	m_iItemID			= iItemID;
	m_iCutInImage		= iCutInImage;	
	m_strMessage		= strMsg;
	m_iNpcID			= iNpcID;
}


bool SPNpcPortal::AddPortalInfo(SPNpcPortalInfo* pInfo /*= NULL*/)
{
	if(pInfo) {
		m_vpNpcPortal.push_back(pInfo);
		return true;
	}
	return false;
}


int SPNpcPortal::GetSize()
{
	return static_cast<int>(m_vpNpcPortal.size());
}


SPNpcPortalInfo* SPNpcPortal::GetInfo(int iIndex)
{
	if(iIndex < 0 || iIndex >= static_cast<int>(m_vpNpcPortal.size()))
		return NULL;

	return m_vpNpcPortal.at(iIndex);
}


std::vector<SPNpcPortalInfo*>* SPNpcPortal::GetNpcPortalList()
{
	return &m_vpNpcPortal;
}




////////////////////////////////////////////////////////////////////////// SPNpcEvent
void SPNpcEvent::Clear() 
{
	m_iItemID			= 0;
	//m_iCutImage			= 0;
	//m_strMessage.clear();
	m_strName.clear();
	m_iNpcID			= 0;

	m_iRewordType		= NPC_EVENT_REWARD_TYPE_NULL;

	int i = 0;
	for(i = 0; i < MAX_NPC_EVENT_REQUIRE; i++) {
		m_Require[i].Clear();
	}
	for(i = 0; i < MAX_NPC_EVENT_ITEM; i++) {
		m_RequireItem[i].Clear();
	}

	m_iRequireEly		= 0;
	m_iRequirePvpPoint	= 0;
	m_strErr.clear();

	std::vector<SPNpcEventInfo*>::iterator iter = m_vpNpcEventInfo.begin();
	for(; iter != m_vpNpcEventInfo.end(); ++iter) {
		SAFE_DELETE((*iter));
	}
	m_vpNpcEventInfo.clear();
}


//void SPNpcEvent::SetNpcEvent(int iItemID, int iCutImage, std::string strMsg, int iNpcID, NPC_EVENT_REWARD_TYPE iType)
void SPNpcEvent::SetNpcEvent(int iItemID,  std::string strName, int iNpcID, NPC_EVENT_REWARD_TYPE iType)
{
	Clear();
	
	m_iItemID			= iItemID;	
	m_strName			= strName;
	m_iNpcID			= iNpcID;
	m_iRewordType		= iType;
}


bool SPNpcEvent::AddEventInfo(SPNpcEventInfo* pInfo /*= NULL*/)
{
	if(pInfo) {
		m_vpNpcEventInfo.push_back(pInfo);
		return true;
	}
	return false;
}


int SPNpcEvent::GetSize()
{
	return static_cast<int>(m_vpNpcEventInfo.size());
}


SPNpcEventInfo* SPNpcEvent::GetEventInfo(int iIndex)
{
	if(iIndex < 0 || iIndex >= static_cast<int>(m_vpNpcEventInfo.size()))
		return NULL;

	return m_vpNpcEventInfo.at(iIndex);
}



////////////////////////////////////////////////////////////////////////// SPNpcEventList
void SPNpcEventList::Clear()
{
	m_iItemID				= 0;
	m_iCutImage				= 0;
	m_strMessage.clear();
	m_iSelect				= 0;
	
	std::vector<SPNpcEvent*>::iterator iter = m_vpNpcEvent.begin();
	for(; iter != m_vpNpcEvent.end(); ++iter) {
		SAFE_DELETE((*iter));
	}
	m_vpNpcEvent.clear();
}


bool SPNpcEventList::AddNpcEvent(SPNpcEvent* pNpcEvent /*= NULL*/)
{
	if(pNpcEvent == NULL || pNpcEvent->m_iItemID == 0)
		return false;
	
	m_vpNpcEvent.push_back(pNpcEvent);
	return true;
}


int SPNpcEventList::GetSize()
{
	return static_cast<int>(m_vpNpcEvent.size());
}


bool SPNpcEventList::SetSelect(int iIndex)
{
	if(iIndex < 0 || iIndex >= static_cast<int>(m_vpNpcEvent.size())) {
		m_iSelect = 0;
		return false;
	}

	m_iSelect = iIndex;
	return true;
}


void SPNpcEventList::SetSelectReset()
{
	m_iSelect = 0;
}


SPNpcEvent* SPNpcEventList::GetSelectNpcEvent()
{
	int iSize = static_cast<int>(m_vpNpcEvent.size());
	if(m_iSelect >= 0 && m_iSelect < iSize) {
		return m_vpNpcEvent.at(m_iSelect);
	}

	return NULL;
}


SPNpcEvent* SPNpcEventList::GetNpcEvent(int iIndex /*= 0*/)
{
	int iSize = static_cast<int>(m_vpNpcEvent.size());
	if(iIndex < 0 || iIndex >= iSize)
		return NULL;	

	return m_vpNpcEvent.at(iIndex);
}


////////////////////////////////////////////////////////////////////////// SPEventNpcHandler
SPEventNpcHandler::SPEventNpcHandler()
{	
	m_pLDTNpcData = NULL;
	m_pLDTNpcTalk = NULL;
	m_pLDTNpcShop = NULL;
	m_pLDTNpcShopDetail = NULL;
	m_pLDTSavePoint = NULL;
	m_pLDTNpcQuest = NULL;

	m_iNpcID = 0;
	m_pNpc = NULL;
	m_pNpcTalk = NULL;
	m_pNpcShop = NULL;
	m_pNpcStorage = NULL;
	m_pSavePoint = NULL;
	m_pNpcQuestList = NULL;
	m_pNpcPortal = NULL;
	m_pNpcEventList = NULL;	
	
	m_pNpcTexture = NULL;

	Init();
}


SPEventNpcHandler::~SPEventNpcHandler()
{
	Clear();
}


void SPEventNpcHandler::Init()
{	
	m_bCutIn = false;
	m_iCutInState = CUT_IN_STATE_NULL;
	m_iClick = EVENT_CLICK_NULL;

	m_fAccumulateRender = 0.0f;
	m_fCurDelay = 0.02f;

	//Load Texture Image
	m_iNpcMoveUnit = 40;
	m_iNpcX = g_pVideo->GetScreenLenX() + 1;
	m_iNpcY = NPC_RENDER_Y;
	
	m_fNpcFadeUnit = 0.06f;
	m_fNpcFadeAlpha = NPC_FADE_START;
	m_fNpcAlpha = 1.0f;
	
	m_pNpcTexture = NULL;

	m_iNpcTalkWindowMode = NPC_TALK_WINDOW_MODE_MAX;

	//Npc Data Create & Init
	//m_pNpc = new SPNpc;	
	m_pNpc = NULL;

	//Npc Talk Data Create & Init
	m_pNpcTalk = new SPNpcTalk;

	//Npc Shop Data Create & Init
	m_pNpcShop = new SPNpcShop;	
	
	//Npc Storage Data Create & Init
	m_pNpcStorage = new SPNpcStorage;

	//SavePoint Data Create & Init
	m_pSavePoint = new SPSavePoint;

	//Npc Quest Data Create & Init
	m_pNpcQuestList = new SPNpcQuestList;

	//Npc Gift Data Create & Init
	m_pNpcGift = new SPNpcGift;

	//Npc Guild Data Create & Init
	m_pNpcGuild = new SPNpcGuild;

	//Npc Portal Data Create & Init
	m_pNpcPortal = new SPNpcPortal;

	//Npc Event List Data Create & Init
	m_pNpcEventList = new SPNpcEventList;

	//Npc Event Data Create & Init
	//m_pNpcEvent = new SPNpcEvent;

	//CutIn Talk Data
	m_pCutInTalk = NULL;

	//LDT Data
	m_pLDTNpcData = NULL;
	g_pResourceManager->GetLDTFromFileName(RES_FNAME_LDT_NPC, &m_pLDTNpcData);
	//g_pResourceManager->GetLDTFromFileName("DATA/LDT/NPCTEST.LDT", &m_pLDTNpcData);
	
	m_pLDTNpcTalk = NULL;
	g_pResourceManager->GetLDTFromFileName(RES_FNAME_LDT_NPCTALK, &m_pLDTNpcTalk);
	
	m_pLDTNpcShop = NULL;
	g_pResourceManager->GetLDTFromFileName(RES_FNAME_LDT_NPCTRADE, &m_pLDTNpcShop);

	m_pLDTNpcShopDetail = NULL;
	g_pResourceManager->GetLDTFromFileName(RES_FNAME_LDT_NPCTRADE_DETAIL, &m_pLDTNpcShopDetail);

	m_pLDTNpcStorage = NULL;
	g_pResourceManager->GetLDTFromFileName(RES_FNAME_LDT_NPCSTORAGE, &m_pLDTNpcStorage);

	m_pLDTSavePoint = NULL;
	g_pResourceManager->GetLDTFromFileName(RES_FNAME_LDT_SAVEPOINT, &m_pLDTSavePoint);

	m_pLDTNpcQuest = NULL;
	g_pResourceManager->GetLDTFromFileName(RES_FNAME_LDT_NPCQUEST, &m_pLDTNpcQuest);	

	m_pLDTNpcGift = NULL;
	g_pResourceManager->GetLDTFromFileName(RES_FNAME_LDT_NPCGIFT,	&m_pLDTNpcGift);	

	m_pLDTNpcGuild = NULL;
	g_pResourceManager->GetLDTFromFileName(RES_FNAME_LDT_NPCGUILD,	&m_pLDTNpcGuild);	
}


void SPEventNpcHandler::Clear()
{
	//SAFE_RELEASE(m_pTexture);
	
	SAFE_RELEASE(m_pNpcTexture);
	
	SAFE_RELEASE(m_pLDTNpcData);
	SAFE_RELEASE(m_pLDTNpcTalk);
	SAFE_RELEASE(m_pLDTNpcShop);
	SAFE_RELEASE(m_pLDTNpcShopDetail);
	SAFE_RELEASE(m_pLDTNpcStorage);
	SAFE_RELEASE(m_pLDTSavePoint);
	SAFE_RELEASE(m_pLDTNpcQuest);
	SAFE_RELEASE(m_pLDTNpcGift);
	SAFE_RELEASE(m_pLDTNpcGuild);
	
	//SAFE_DELETE(m_pNpc);
	SAFE_DELETE(m_pNpcShop);
	SAFE_DELETE(m_pNpcTalk);
	SAFE_DELETE(m_pNpcStorage);
	SAFE_DELETE(m_pSavePoint);
	SAFE_DELETE(m_pNpcQuestList);
	SAFE_DELETE(m_pNpcGift);
	SAFE_DELETE(m_pNpcGuild);
	SAFE_DELETE(m_pNpcPortal);
	SAFE_DELETE(m_pNpcEventList);
	//SAFE_DELETE(m_pNpcEvent);
}


void SPEventNpcHandler::Process(float fTime)
{
	if(m_bCutIn) {
		CutInProcess(fTime);
		CutInKeyProcess(fTime);
	}
}


void SPEventNpcHandler::CutInKeyProcess(float fTime)
{
	//if( m_bActiveErrorMsg )
	//	return;

	SPInputStruct* pInputStruct = g_pInputManager->GetInputStruct();

	if( g_pInterfaceManager->IsKeyInput() == FALSE )
	{
		//m_iKeyLeft			= pInputStruct->ikArrowLeft.InputState;
		//m_iKeyRight			= pInputStruct->ikArrowRight.InputState;
		m_iKey1				= pInputStruct->ik1.InputState;
		m_iKey2				= pInputStruct->ik2.InputState;
		m_iKey3				= pInputStruct->ik3.InputState;
		m_iKey4				= pInputStruct->ik4.InputState;
		//m_iKeyEsc			= pInputStruct->ikEscape.InputState;
		m_iSpace			= pInputStruct->ikSpaceBar.InputState;
	}
	else
	{
		//m_iKeyLeft			= INPUT_UNPRESS_REPEAT;
		//m_iKeyRight			= INPUT_UNPRESS_REPEAT;
		m_iKey1				= INPUT_UNPRESS_REPEAT;
		m_iKey2				= INPUT_UNPRESS_REPEAT;
		m_iKey3				= INPUT_UNPRESS_REPEAT;
		m_iKey4				= INPUT_UNPRESS_REPEAT;
		//m_iKeyEsc			= INPUT_UNPRESS_REPEAT;
		m_iSpace			= INPUT_UNPRESS_REPEAT;
	}	
	
	////
	//if(m_iKeyEsc == INPUT_PRESS) {
	//	if(m_iCutInState > CUT_IN_STATE_SCROLL_END) {
	//		SetCutInState(CUT_IN_STATE_SCROLL_END);
	//	}
	//	else if(m_iCutInState == CUT_IN_STATE_SCROLL_END) {
	//		SetCutInState(CUT_IN_STATE_NULL);
	//	}
	//}	
	
	if(m_iCutInState == CUT_IN_STATE_SCROLL_END || m_iCutInState == CUT_IN_STATE_TALK || m_iCutInState == CUT_IN_STATE_SAVEPOINT) {
		//SPWindowNpcTalk* pWindow = NULL;
		//pWindow = (SPWindowNpcTalk*)(g_pInterfaceManager->GetNpcTalkWindow());
		SPWindow* pWindow = NULL;

		//if(pWindow == NULL)
		//	return;
		
		////키 입력들은 NpcTalk 윈도우에게만 상용되어진다 
		//if(m_iCutInState == CUT_IN_STATE_TALK || m_iCutInState == CUT_IN_STATE_SAVEPOINT) {
		//	if(m_iKeyLeft == INPUT_PRESS) {
		//		//pWindow->
		//		//pWindow->SetKeyPrevPage();
		//		return;
		//	}
		//	else if(m_iKeyRight == INPUT_PRESS) {
		//		//pWindow->SetKeyNextPage();
		//		return;
		//	}
		//}
		
		if(m_iKey1 == INPUT_PRESS) {
			pWindow = g_pInterfaceManager->FindWindow(WIID_PCTALK);
			if(pWindow) {
				pWindow->SPSendMessage(SPIM_SET_NPCTALK_ITEMSELECT, (WPARAM)0, (LPARAM)0);
			}
			return;
		}
		else if(m_iKey2 == INPUT_PRESS) {
			pWindow = g_pInterfaceManager->FindWindow(WIID_PCTALK);
			if(pWindow) {
				pWindow->SPSendMessage(SPIM_SET_NPCTALK_ITEMSELECT, (WPARAM)1, (LPARAM)0);
			}
			return;
		}
		else if(m_iKey3 == INPUT_PRESS) {
			pWindow = g_pInterfaceManager->FindWindow(WIID_PCTALK);
			if(pWindow) {
				pWindow->SPSendMessage(SPIM_SET_NPCTALK_ITEMSELECT, (WPARAM)2, (LPARAM)0);
			}
			return;
		}
		else if(m_iKey4 == INPUT_PRESS) {
			pWindow = g_pInterfaceManager->FindWindow(WIID_PCTALK);
			if(pWindow) {
				pWindow->SPSendMessage(SPIM_SET_NPCTALK_ITEMSELECT, (WPARAM)3, (LPARAM)0);
			}
			return;
		}
		else if(m_iSpace == INPUT_PRESS) {
			pWindow = g_pInterfaceManager->FindWindow(WIID_NPCTALK);
			if(pWindow) {
				pWindow->SPSendMessage(SPIM_SET_CUTIN_CLICK, (WPARAM)0, (LPARAM)0);
			}
			return;
		}
		else {
		}
	}
}


void SPEventNpcHandler::CutInProcess (float fTime)
{
	m_fAccumulateRender += fTime;
	if( m_fAccumulateRender > m_fCurDelay) {
		m_fAccumulateRender -= m_fCurDelay;
		
		if(m_iCutInState == CUT_IN_STATE_RELEASE){
			if(m_pNpcTexture) {
				if(m_iNpcMoveUnit < 40 && m_iNpcMoveUnit > 15) {
					m_iNpcMoveCount += 2;
					m_iNpcMoveUnit -= m_iNpcMoveCount;
				}				

				m_iNpcX += m_iNpcMoveUnit;
				if(m_iNpcX >= g_pVideo->GetScreenLenX() + 1)
					m_iNpcX = g_pVideo->GetScreenLenX() + 1;

				m_fNpcFadeAlpha -= m_fNpcFadeUnit;
				if(m_fNpcFadeAlpha <= 0.0f)
					m_fNpcFadeAlpha = 0.0f;
			}
		}		
		else if( CUT_IN_STATE_NULL < m_iCutInState) {			
			if(m_pNpcTexture) {
				if(m_iNpcMoveUnit <= 40) {
					m_iNpcMoveCount += 2;
					m_iNpcMoveUnit += m_iNpcMoveCount;
				}
				else {
                   m_iNpcMoveUnit == 40;
				}

				if(m_iNpcX - NPC_RENDER_X < m_iNpcMoveUnit) {
					m_iNpcX = NPC_RENDER_X;
				}
				else {
					m_iNpcX -= m_iNpcMoveUnit;
				}
				
				m_fNpcFadeAlpha += m_fNpcFadeUnit;
				if(m_fNpcFadeAlpha >= 1.0f)
					m_fNpcFadeAlpha = 1.0f;

			}
		}
	}
}


void SPEventNpcHandler::Render(float fTime)
{
	if(m_bCutIn)
		CutInRender(fTime);
}


void SPEventNpcHandler::CutInRender (float fTime)
{	
	if(m_iCutInState == CUT_IN_STATE_NULL )	return;

	if(m_pNpcTexture) {
		if(m_iCutInState == CUT_IN_STATE_SCROLL || CUT_IN_STATE_RELEASE) {
			m_pNpcTexture->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fNpcFadeAlpha));
			m_pNpcTexture->RenderTexture(m_iNpcX, m_iNpcY);
			m_pNpcTexture->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else {
			m_pNpcTexture->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fNpcAlpha));
			m_pNpcTexture->RenderTexture(m_iNpcX, m_iNpcY);
			m_pNpcTexture->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
	
	////
	//if( m_iCutInState < CUT_IN_STATE_TALK ) {
	//	//if(m_pNpc->m_pCutInImage1) {
	//	//	m_pNpc->m_pCutInImage1->RenderTexture(m_iNpcX, m_iNpcY);
	//	//}
	//	if(m_pNpcTexture) {
	//		m_pNpcTexture->RenderTexture(m_iNpcX, m_iNpcY);
	//	}
	//}
	//else if(m_iCutInState == CUT_IN_STATE_TALK) {
	//	if(m_pNpcTexture) {
	//		m_pNpcTexture->RenderTexture(m_iNpcX, m_iNpcY);
	//	}
	//}
	//else if(m_iCutInState == CUT_IN_STATE_TRADE) {
	//	if(m_pNpcTexture) {
	//		m_pNpcTexture->RenderTexture(m_iNpcX, m_iNpcY);
	//	}
	//}

	g_pMouseCursor->Render(fTime);
}


//////////////////////////////////////////////////////////////////////////
void SPEventNpcHandler::SetCutIn(bool bCutIn /*= false*/)
{	
	m_bCutIn = bCutIn;

	//if(m_bCutIn)
	//	ResetRect(NPC_FADE_IN);	

	g_pInterfaceManager->SetCutIn(m_bCutIn);
}


bool SPEventNpcHandler::IsCutIn()
{
	return m_bCutIn;
}


void SPEventNpcHandler::SetCutInState(CUT_IN_STATE iState /*= CUT_IN_STATE_NULL*/)
{
	//[2006/1/2] - 이미 주어진 컷인 스테이터스와 동일한 상황인 경우 넘어간다 
	if(m_iCutInState == iState)
		return ;
	
	
	if(iState == CUT_IN_STATE_RELEASE || iState == CUT_IN_STATE_NULL) {
		if(m_iCutInState == CUT_IN_STATE_SCROLL_END || m_iCutInState == CUT_IN_STATE_TALK
			 || m_iCutInState == CUT_IN_STATE_SAVEPOINT) 
		{
			ResetRect(NPC_FADE_OUT);
		}
		else {
			SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_NPCTALK);
			if(pWindow /*&& pWindow->IsShow()*/) {
				pWindow->SPSendMessage(SPIM_SET_HIDE_START, (WPARAM)0, (LPARAM)0);
			}

			pWindow = g_pInterfaceManager->FindWindow(WIID_PCTALK);
			if(pWindow && pWindow->IsShow()) {
				pWindow->Hide();
				//pWindow->SPSendMessage(SPIM_SET_HIDE_START, (WPARAM)0, (LPARAM)0);
			}
			m_iClick = EVENT_CLICK_NULL;
		}
	}
	
	m_iCutInState = iState;

	if(m_iCutInState == CUT_IN_STATE_NULL) {
		//ResetRect(NPC_FADE_OUT);
		m_iNpcID = 0;		
	}


	//if(m_iCutInState == CUT_IN_STATE_RELEASE) {
	//	ResetRect(NPC_FADE_OUT);
	//}
	if(m_iCutInState == CUT_IN_STATE_SCROLL) {
		ResetRect(NPC_FADE_IN);	
	}

	
	DXUTOutputDebugString("SPEventNpcHandler::SetCutInState %d\n", m_iCutInState);

	g_pInterfaceManager->SPPerformMessage(SPIM_SET_CUTIN_WINDOW);
}


CUT_IN_STATE SPEventNpcHandler::GetCutInState()
{
	return m_iCutInState;
}


int SPEventNpcHandler::PerformMouseLUp(int iX, int iY)
{
	//if(m_iCutInState != CUT_IN_STATE_TALK)
	//	return 1;

	if(m_iClick == EVENT_CLICK_NULL)
		return 1;
	
	if(m_pNpcTalk && m_pNpcTalk->m_iID) {
		if(m_iClick == EVENT_CLICK_NPCTALK) {
			SPWindow* pWindow = NULL;
			if(m_pNpcTalk->m_iBoxPos == NPC_TALK_WINDOW_NPC) {
				pWindow = g_pInterfaceManager->FindWindow(WIID_NPCTALK);
				if(pWindow) {
					pWindow->SPSendMessage(SPIM_SET_CUTIN_CLICK, (WPARAM)0, (LPARAM)0);
				}
			}
		}
		////		
		//if(m_pNpcTalk->m_iBoxPos == NPC_TALK_WINDOW_PC) {
		//}
	}

	return 1;
}


void SPEventNpcHandler::SetNPCTalk()
{
	if(m_iCutInState != CUT_IN_STATE_TALK)
		return;

	SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_NPCTALK);
	if(pWindow) {
		if(!pWindow->IsShow())
			return;
	}
	else
		return;

	//여기서 닫고 전환할수 있는 장치가 필요...
}


void SPEventNpcHandler::SetNpcTalkWindowMode(NPC_TALK_WINDOW_MODE iType /*= NPC_TALK_TYPE_NORMAL*/)
{
	m_iNpcTalkWindowMode = iType;
	SetNPCTalk();
}


int	SPEventNpcHandler::GetNpcID()
{
	return m_iNpcID;
}


void SPEventNpcHandler::ResetRect(NPC_FADE iFade /*= NPC_FADE_IN*/)
{
	if(iFade == NPC_FADE_IN) {
		//m_iNpcX = 801;
		m_iNpcX = g_pVideo->GetScreenLenX() - 100;
		m_fNpcFadeAlpha = NPC_FADE_START;
		m_iNpcMoveUnit = 40;
		m_iNpcMoveCount = 0;
		m_iClick = EVENT_CLICK_NULL;
	}
	else if(iFade == NPC_FADE_OUT) {
		m_iNpcMoveUnit = 40;
		m_iNpcMoveCount = 0;
		SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_NPCTALK);
		if(pWindow /*&& pWindow->IsShow()*/) {
			pWindow->SPSendMessage(SPIM_SET_HIDE_START, (WPARAM)0, (LPARAM)0);
		}

		pWindow = g_pInterfaceManager->FindWindow(WIID_PCTALK);
		if(pWindow /*&& pWindow->IsShow()*/) {
			pWindow->SPSendMessage(SPIM_SET_HIDE_START, (WPARAM)0, (LPARAM)0);
		}
		m_iClick = EVENT_CLICK_NULL;
	}
}


bool SPEventNpcHandler::LoadNpc(int iItemID /*= 0*/)
{
	if(LoadNpcDataLDT(iItemID) == NULL)			return false;	
	
	g_pInterfaceManager->GetNpcTalkWindow()->SPSendMessage(SPIM_SET_NPCDATA);	
	
	SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_PCTALK);		//[2007/9/13]
	if(pWindow) {
		pWindow->SPSendMessage(SPIM_SET_NPCDATA);
	}

	m_iNpcID = iItemID;

	return true;
}


SPNpc* SPEventNpcHandler::LoadNpcDataLDT(int iItemID /*= 0*/)
{
	/*
	assert(m_pLDTNpcData != NULL && "m_pLDTNpcData is NULL..");	
	
	int recordCount = m_pLDTNpcData->GetItemCount();
	
	LDT_Field ldtFieldCutImage;
	LDT_Field ldtFieldName;
	LDT_Field ldtFieldMessage;	
	
	LDT_Field ldtEventType1;
	LDT_Field ldtTypeID1;
	LDT_Field ldtTypeMenu1;
	LDT_Field ldtEventType2;
	LDT_Field ldtTypeID2;
	LDT_Field ldtTypeMenu2;
	LDT_Field ldtEventType3;
	LDT_Field ldtTypeID3;
	LDT_Field ldtTypeMenu3;
	LDT_Field ldtEventType4;
	LDT_Field ldtTypeID4;
	LDT_Field ldtTypeMenu4;
	
	m_pLDTNpcData->GetFieldFromLable( iItemID, "_CutInImage",	ldtFieldCutImage);
	//m_pLDTNpcData->GetFieldFromLable( iItemID, "_Npcname",		ldtFieldName); 
	m_pLDTNpcData->GetFieldFromLable( iItemID, "_Message",		ldtFieldMessage); 
	
	m_pLDTNpcData->GetFieldFromLable( iItemID, "_EventType1",	ldtEventType1); 
	m_pLDTNpcData->GetFieldFromLable( iItemID, "_TypeID1",		ldtTypeID1); 
	m_pLDTNpcData->GetFieldFromLable( iItemID, "_Text1",		ldtTypeMenu1); 
	
	m_pLDTNpcData->GetFieldFromLable( iItemID, "_EventType2",	ldtEventType2); 
	m_pLDTNpcData->GetFieldFromLable( iItemID, "_TypeID2",		ldtTypeID2); 
	m_pLDTNpcData->GetFieldFromLable( iItemID, "_Text2",		ldtTypeMenu2); 
	
	m_pLDTNpcData->GetFieldFromLable( iItemID, "_EventType3",	ldtEventType3); 	
	m_pLDTNpcData->GetFieldFromLable( iItemID, "_TypeID3",		ldtTypeID3); 
	m_pLDTNpcData->GetFieldFromLable( iItemID, "_Text3",		ldtTypeMenu3); 

	m_pLDTNpcData->GetFieldFromLable( iItemID, "_EventType4",	ldtEventType4); 
	m_pLDTNpcData->GetFieldFromLable( iItemID, "_TypeID4",		ldtTypeID4);
	m_pLDTNpcData->GetFieldFromLable( iItemID, "_Text4",		ldtTypeMenu4); 

	int iCutImage = ldtFieldCutImage.uData.lValue;

	std::string strName;
	strName.clear();
	//if(ldtFieldName.uData.pValue) {
	//	strName = ldtFieldName.uData.pValue;
	//}

	std::string strMessage = "";
	if(ldtFieldMessage.uData.pValue == NULL){
		 return NULL;
	}
	strMessage = ldtFieldMessage.uData.pValue;

	int iType1 = ldtEventType1.uData.lValue;
	int iTypeID1 = ldtTypeID1.uData.lValue;
	std::string strMenu1 = "";
	if(ldtTypeMenu1.uData.pValue)
	strMenu1 = ldtTypeMenu1.uData.pValue;

	int iType2 = ldtEventType2.uData.lValue;
	int iTypeID2 = ldtTypeID2.uData.lValue;
	std::string strMenu2 = "";
	if(ldtTypeMenu2.uData.pValue)
		strMenu2 = ldtTypeMenu2.uData.pValue;
	
	int iType3 = ldtEventType3.uData.lValue;
	int iTypeID3 = ldtTypeID3.uData.lValue;
	std::string strMenu3 = "";
	if(ldtTypeMenu3.uData.pValue)
		strMenu3 = ldtTypeMenu3.uData.pValue;
	
	int iType4 = ldtEventType4.uData.lValue;
	int iTypeID4 = ldtTypeID4.uData.lValue;
	std::string strMenu4 = "";
	if(ldtTypeMenu4.uData.pValue)
		strMenu4 = ldtTypeMenu4.uData.pValue;
	
	m_pNpc->SetNpc(iItemID, iCutImage, 
		iType1, iTypeID1, strMenu1,
		iType2, iTypeID2, strMenu2,
		iType3, iTypeID3, strMenu3,
		iType4, iTypeID4, strMenu4,
		strName,
		strMessage);	
	*/	

	if(g_pEventManager->GetEventArchive()->GetNpc(iItemID))
	{
		m_pNpc = g_pEventManager->GetEventArchive()->GetNpc(iItemID);
		SetNpcTalkImage(m_pNpc->m_iCutImage);
	}	

	return m_pNpc;
}


SPNpc*	SPEventNpcHandler::GetNpcData(int iItemID /*= 0*/)
{
	//if(iItemID < 1)
	//	return NULL;

	return m_pNpc;
}


SPNpcTalk* SPEventNpcHandler::LoadNpcTalkDataLDT(int iItemID /*= 0*/)
{	
	assert(m_pLDTNpcTalk != NULL && "m_pLDTNpcTalk is NULL..");

	int recordCount = m_pLDTNpcTalk->GetItemCount();
	LDT_Field LDTField;
	
	if(m_pNpcTalk->m_iID == iItemID)
		return m_pNpcTalk;	

	m_pNpcTalk->Clear();	
	
	bool bReturn = true;
	bReturn = m_pLDTNpcTalk->GetField( iItemID, 0,		LDTField);		//_BGM	
	
	if(!bReturn)
		return m_pNpcTalk;

	if(LDTField.uData.pValue)
		m_pNpcTalk->m_strBGM = LDTField.uData.pValue;

	m_pLDTNpcTalk->GetField( iItemID, 1,		LDTField);		//_CPEvent
	m_pNpcTalk->m_bCompel = static_cast<bool>(LDTField.uData.lValue);

	m_pLDTNpcTalk->GetField( iItemID, 2,		LDTField);		//_TalkType
	m_pNpcTalk->m_iTalkType = (TALK_TYPE)LDTField.uData.lValue;
	
	m_pLDTNpcTalk->GetField( iItemID, 3,		LDTField);		//_NpcName
	if(LDTField.uData.pValue)
		m_pNpcTalk->m_strName = LDTField.uData.pValue;
	
	m_pLDTNpcTalk->GetField( iItemID, 4,		LDTField);		//_Message
	if(LDTField.uData.pValue)
		m_pNpcTalk->m_strMessage = LDTField.uData.pValue;

	m_pLDTNpcTalk->GetField( iItemID, 5,		LDTField);		//_BoxPosition
	m_pNpcTalk->m_iBoxPos = (NPC_TALK_WINDOW)LDTField.uData.lValue;

	m_pLDTNpcTalk->GetField( iItemID, 6,		LDTField);		//_BoxType
	m_pNpcTalk->m_iBoxType = (NPC_TALK_WINDOW_MODE)LDTField.uData.lValue;

	m_pLDTNpcTalk->GetField( iItemID, 7,		LDTField);		//_CutInImage
	m_pNpcTalk->m_iCutInImage = LDTField.uData.lValue;

	m_pLDTNpcTalk->GetField( iItemID, 8,		LDTField);		//_CutInEffect
	m_pNpcTalk->m_iCutInEffect = (NPC_TALK_EFFECT)LDTField.uData.lValue; 

	m_pLDTNpcTalk->GetField( iItemID, 9,		LDTField);		//_Image
	m_pNpcTalk->m_iAttachImage = LDTField.uData.lValue;
	if(m_pNpcTalk->m_iAttachImage)
		m_pNpcTalk->m_bShowAttach = false;

	m_pLDTNpcTalk->GetField( iItemID, 10,		LDTField);		//_ImageEffect
	m_pNpcTalk->m_iAttachEffect = (NPC_TALK_EFFECT)LDTField.uData.lValue; 	
	
	std::string strQuestion = "";
	NPC_EVENT_TYPE iEventType = NPC_EVENT_TYPE_NULL;
	int iEventID = 0;
	for(int i = 0; i < MAX_NPC_EVENT_TYPE; i++) {
		strQuestion = "";
		iEventType = NPC_EVENT_TYPE_NULL;
		iEventID = 0;
		
		m_pLDTNpcTalk->GetField( iItemID, 11 + (i * 3),		LDTField);		//_EventTypeID1
		iEventID = LDTField.uData.lValue;

		m_pLDTNpcTalk->GetField( iItemID, 12 + (i * 3),		LDTField);		//_EventType1	
		iEventType = (NPC_EVENT_TYPE)LDTField.uData.lValue;

		m_pLDTNpcTalk->GetField( iItemID, 13 + (i * 3),		LDTField);		//_Question1
		if(LDTField.uData.pValue)
			strQuestion = LDTField.uData.pValue;

		if(m_pNpcTalk->m_iTalkType == TALK_TYPE_SELECT && strQuestion.empty())
			break;
		
		m_pNpcTalk->m_NpcEventType[i].SetEventType(iEventType, iEventID, strQuestion);
	}

	m_pLDTNpcTalk->GetField( iItemID, 23,		LDTField);					//_Mission ID
	m_pNpcTalk->m_iMissionID = LDTField.uData.lValue; 	

	m_pLDTNpcTalk->GetField( iItemID, 24,		LDTField);					//_Class_ChageID
	m_pNpcTalk->m_iClassChangeID = LDTField.uData.lValue; 	

	m_pLDTNpcTalk->GetField( iItemID, 25,		LDTField);					//_Class_ChageID
	m_pNpcTalk->m_iStoryID = LDTField.uData.lValue; 	
		
	m_pNpcTalk->m_iID = iItemID;
	return m_pNpcTalk;
	
	////
	//strQuestion	= "";
	//iEventType	= NPC_EVENT_TYPE_NULL;
	//iEventID	= 0;	
	//m_pLDTNpcTalk->GetField( iItemID, 14,		LDTField);		//_EventTypeID2
	//iEventID = LDTField.uData.lValue;

	//m_pLDTNpcTalk->GetField( iItemID, 15,		LDTField);		//_EventType2
	//iEventType = (NPC_EVENT_TYPE)LDTField.uData.lValue;	
	//
	//m_pLDTNpcTalk->GetField( iItemID, 16,		LDTField);		//_Question2
	//if(LDTField.uData.pValue)
	//	strQuestion = LDTField.uData.pValue;		

	//m_pNpcTalk->m_NpcEventType[1].SetEventType(iEventType, iEventID, strQuestion);
	//
	//strQuestion	= "";
	//iEventType	= NPC_EVENT_TYPE_NULL;
	//iEventID	= 0;
	//m_pLDTNpcTalk->GetField( iItemID, 17,		LDTField);		//_EventTypeID3
	//iEventID = LDTField.uData.lValue;

	//m_pLDTNpcTalk->GetField( iItemID, 18,		LDTField);		//_EventType3	
	//iEventType = (NPC_EVENT_TYPE)LDTField.uData.lValue;	
	//
	//m_pLDTNpcTalk->GetField( iItemID, 19,		LDTField);		//_Question3
	//if(LDTField.uData.pValue)
	//	strQuestion = LDTField.uData.pValue;	

	//m_pNpcTalk->m_NpcEventType[2].SetEventType(iEventType, iEventID, strQuestion);
	//
	//strQuestion	= "";
	//iEventType	= NPC_EVENT_TYPE_NULL;
	//iEventID	= 0;
	//m_pLDTNpcTalk->GetField( iItemID, 20,		LDTField);		//_EventTypeID4
	//iEventID = LDTField.uData.lValue;

	//m_pLDTNpcTalk->GetField( iItemID, 21,		LDTField);		//_EventType4	
	//iEventType = (NPC_EVENT_TYPE)LDTField.uData.lValue;	
	//
	//m_pLDTNpcTalk->GetField( iItemID, 22,		LDTField);		//_Question4	
	//if(LDTField.uData.pValue)
	//	strQuestion = LDTField.uData.pValue;	
	//
	//m_pNpcTalk->m_NpcEventType[3].SetEventType(iEventType, iEventID, strQuestion);

	//m_pLDTNpcTalk->GetField( iItemID, 23,		LDTField);		//_Mission ID  
	//m_pNpcTalk->m_iMissionID = LDTField.uData.lValue;	
	
	
	////
	//LDT_Field ldtFieldMessage1;
	//LDT_Field ldtFieldMessage2;
	//LDT_Field ldtFieldMessage3;
	//LDT_Field ldtFieldMessage4;
	//LDT_Field ldtFieldImage1;
	//LDT_Field ldtFieldImage2;
	//LDT_Field ldtFieldImage3;
	//LDT_Field ldtFieldImage4;
	//LDT_Field ldtFieldEndMessage;
	//LDT_Field ldtFieldEndImage;
	//
	//LDT_Field ldtFieldStrQuestion1;
	//LDT_Field ldtFieldStrQuestion2;
	//LDT_Field ldtFieldStrQuestion3;
	//LDT_Field ldtFieldStrQuestion4;
	//
	//LDT_Field ldtFieldQuestion1;
	//LDT_Field ldtFieldQuestion2;
	//LDT_Field ldtFieldQuestion3;
	//LDT_Field ldtFieldQuestion4;

	//m_pLDTNpcTalk->GetFieldFromLable( iItemID, "_Message1",		ldtFieldMessage1);
	//m_pLDTNpcTalk->GetFieldFromLable( iItemID, "_Message2",		ldtFieldMessage2);
	//m_pLDTNpcTalk->GetFieldFromLable( iItemID, "_Message3",		ldtFieldMessage3);
	//m_pLDTNpcTalk->GetFieldFromLable( iItemID, "_Message4",		ldtFieldMessage4);

	//m_pLDTNpcTalk->GetFieldFromLable( iItemID, "_CutInImage1",	ldtFieldImage1);
	//m_pLDTNpcTalk->GetFieldFromLable( iItemID, "_CutInImage2",	ldtFieldImage2);
	//m_pLDTNpcTalk->GetFieldFromLable( iItemID, "_CutInImage3",	ldtFieldImage3);
	//m_pLDTNpcTalk->GetFieldFromLable( iItemID, "_CutInImage4",	ldtFieldImage4);

	//m_pLDTNpcTalk->GetFieldFromLable( iItemID, "_EventMessage",	ldtFieldEndMessage);
	//m_pLDTNpcTalk->GetFieldFromLable( iItemID, "_EventImage",	ldtFieldEndImage);

	//m_pLDTNpcTalk->GetFieldFromLable( iItemID, "_Question1",	ldtFieldStrQuestion1);
	//m_pLDTNpcTalk->GetFieldFromLable( iItemID, "_Question2",	ldtFieldStrQuestion2);
	//m_pLDTNpcTalk->GetFieldFromLable( iItemID, "_Question3",	ldtFieldStrQuestion3);
	//m_pLDTNpcTalk->GetFieldFromLable( iItemID, "_Question4",	ldtFieldStrQuestion4);

	//m_pLDTNpcTalk->GetFieldFromLable( iItemID, "_Question1_ID",	ldtFieldQuestion1);
	//m_pLDTNpcTalk->GetFieldFromLable( iItemID, "_Question2_ID",	ldtFieldQuestion2);
	//m_pLDTNpcTalk->GetFieldFromLable( iItemID, "_Question3_ID",	ldtFieldQuestion3);
	//m_pLDTNpcTalk->GetFieldFromLable( iItemID, "_Question4_ID",	ldtFieldQuestion4);
	//
	//std::string strMessage1 = "";
	//if(ldtFieldMessage1.uData.pValue)
	//	strMessage1 = ldtFieldMessage1.uData.pValue;
	//
	//std::string strMessage2 = "";
	//if(ldtFieldMessage2.uData.pValue)
	//	strMessage2 = ldtFieldMessage2.uData.pValue;
	//
	//std::string strMessage3 = "";
	//if(ldtFieldMessage3.uData.pValue)
	//	strMessage3 = ldtFieldMessage3.uData.pValue;
	//
	//std::string strMessage4 = "";
	//if(ldtFieldMessage4.uData.pValue)
	//	strMessage4 = ldtFieldMessage4.uData.pValue;
	//
	//int iCutImage1 = ldtFieldImage1.uData.lValue;
	//int iCutImage2 = ldtFieldImage2.uData.lValue;
	//int iCutImage3 = ldtFieldImage3.uData.lValue;
	//int iCutImage4 = ldtFieldImage4.uData.lValue;

	//std::string strMessageEnd = "";
	//if(ldtFieldEndMessage.uData.pValue)
	//	strMessageEnd = ldtFieldEndMessage.uData.pValue;
	//
	//int iCutImageEnd = ldtFieldEndImage.uData.lValue;

	//std::string strQuestion1 = "";
	//if(ldtFieldStrQuestion1.uData.pValue)
	//	strQuestion1 = ldtFieldStrQuestion1.uData.pValue;
	//
	//std::string strQuestion2 = "";
	//if(ldtFieldStrQuestion2.uData.pValue)
	//	strQuestion2 = ldtFieldStrQuestion2.uData.pValue;
	//
	//std::string strQuestion3 = "";
	//if(ldtFieldStrQuestion3.uData.pValue)
	//	strQuestion3 = ldtFieldStrQuestion3.uData.pValue;
	//
	//std::string strQuestion4 = "";
	//if(ldtFieldStrQuestion4.uData.pValue)
	//	strQuestion4 = ldtFieldStrQuestion4.uData.pValue;
	//
	//int iQuestion1 = ldtFieldQuestion1.uData.lValue;
	//int iQuestion2 = ldtFieldQuestion2.uData.lValue;
	//int iQuestion3 = ldtFieldQuestion3.uData.lValue;
	//int iQuestion4 = ldtFieldQuestion4.uData.lValue;	
	//
	//m_pNpcTalk->SetNpcTalk(strMessage1, strMessage2, strMessage3, strMessage4, 
	//						iCutImage1, iCutImage2, iCutImage3, iCutImage4,
	//						strMessageEnd, iCutImageEnd, 
	//						strQuestion1, strQuestion2, strQuestion3, strQuestion4,
	//						iQuestion1, iQuestion2, iQuestion3, iQuestion4);
	//
	//return m_pNpcTalk;
}


SPNpcTalk* SPEventNpcHandler::GetNpcTalkData(int iItemID /*= 0*/)
{
	return m_pNpcTalk;
}


bool SPEventNpcHandler::SetNpcTalkImage(int iIndex /*= 0*/)
{
	if(iIndex == 0)		return false;
	
	if(m_pNpcTexture)
		SAFE_RELEASE(m_pNpcTexture);	

	std::string strResFullPath; // = RES_WORK_RESOURCE_PATH;
	std::string strResFile = "";
	
	if ( g_pResourceManager->GetGlobalFileName(iIndex) )
		strResFile = g_pResourceManager->GetGlobalFileName(iIndex);

	if(strResFile != ""){
		strResFullPath = strResFile;
	}

	if(false == g_pVideo->GetTextureMgr()->LoadTexture(strResFullPath.c_str(), &m_pNpcTexture))
		return false;

	return true;
}


bool SPEventNpcHandler::SetNpcBGM(std::string strBGM)
{
	if(strBGM.empty())
		return false;

	
	g_pSubGameManager->SPPerformMessage(SPGM_SUG_SETBGM , (WPARAM)strBGM.c_str());
	return true;	
}


SPNpcShop* SPEventNpcHandler::LoadNpcShopDataLDT(int iItemID /*= 0*/)
{	
	assert(m_pLDTNpcShop != NULL && "m_pLDTNpcShop is NULL..");

	int recordCount = m_pLDTNpcShop->GetItemCount();

	LDT_Field ldtFieldBuyFactor;
	LDT_Field ldtFieldImage;
	LDT_Field ldtFieldMessage;

	m_pLDTNpcShop->GetFieldFromLable( iItemID, "_BuyFactor",	ldtFieldBuyFactor);
	m_pLDTNpcShop->GetFieldFromLable( iItemID, "CutInImage",	ldtFieldImage);
	m_pLDTNpcShop->GetFieldFromLable( iItemID, "Message",		ldtFieldMessage);
	
	int iBuyFactor = ldtFieldBuyFactor.uData.lValue;
	int iImage = ldtFieldImage.uData.lValue;
	
	std::string strMessage = "";
	if(ldtFieldMessage.uData.pValue)
		strMessage = ldtFieldMessage.uData.pValue;	
	
	m_pNpcShop->SetNpcShop(m_pNpc->m_iNpcID, iItemID, iBuyFactor, iImage, strMessage);

	
	LDT_Field ldtFieldID;
	LDT_Field ldtFieldItemID;
	LDT_Field ldtFieldRare;
	LDT_Field ldtFieldStack;
	LDT_Field ldtFieldSellFactor;

	char szFieldID[64];
	//char szFieldItemID[64];	
	//char szFieldRear[64];
	//char szFieldSellFactor[64];
	
	int iItemSize = 0;
	for(int i = 0 ; i < MAX_NPC_TRADE_ITEM ; i++) {	
		ZeroMemory(szFieldID, 64);
		wsprintf(szFieldID, "_ID%d", i+1);

		m_pLDTNpcShop->GetFieldFromLable( iItemID, szFieldID,	ldtFieldID);
		int iID = ldtFieldID.uData.lValue;		

		m_pLDTNpcShopDetail->GetFieldFromLable( iID, "_ItemID",		ldtFieldItemID);
		m_pLDTNpcShopDetail->GetFieldFromLable( iID, "_ItemRare",	ldtFieldRare);
		m_pLDTNpcShopDetail->GetFieldFromLable( iID, "_ItemCount",	ldtFieldStack);
		m_pLDTNpcShopDetail->GetFieldFromLable( iID, "_SellFactor",	ldtFieldSellFactor);

		int iItemID = ldtFieldItemID.uData.lValue;
		int iRear = ldtFieldRare.uData.lValue;
		int iStack = ldtFieldStack.uData.lValue;
		int iSellFactor = ldtFieldSellFactor.uData.lValue;

		if(iItemID == 0)		break;

		m_pNpcShop->SetNpcShopSellItem(i, iItemID, iRear, iStack, iSellFactor);
		
		iItemSize++;
	}

	m_pNpcShop->SetSellItemSize(iItemSize);
	
	SetNpcTalkImage(m_pNpcShop->m_iCutInImage);		//Shop에서 CutIn Image 로드 하는 부분 다시 살펴볼것

	return m_pNpcShop;
}


SPNpcShop* SPEventNpcHandler::GetNpcShopData(int iItemID /*= 0*/)
{
	return m_pNpcShop;
}


SPNpcStorage* SPEventNpcHandler::LoadNpcStorageDataLDT(int iItemID /*= 0*/)
{
	assert(m_pLDTNpcStorage != NULL && "m_pLDTNpcStorage is NULL..");

	int recordCount = m_pLDTNpcStorage->GetItemCount();	
	
	LDT_Field ldtFieldImage;
	LDT_Field ldtFieldMessage;
	
	m_pLDTNpcStorage->GetFieldFromLable( iItemID, "_CutInImage",	ldtFieldImage);
	m_pLDTNpcStorage->GetFieldFromLable( iItemID, "_Message",		ldtFieldMessage);
	
	int iImage = ldtFieldImage.uData.lValue;
	std::string iMessage = "";
	if(ldtFieldMessage.uData.pValue)
		iMessage = ldtFieldMessage.uData.pValue;
	
	m_pNpcStorage->SetNpcStorage(m_pNpc->m_iNpcID, iImage, iMessage);

	SetNpcTalkImage(m_pNpcStorage->m_iCutInImage);

	return m_pNpcStorage;
}

SPNpcStorage* SPEventNpcHandler::GetNpcStorageData(int iItemID /*= 0*/)
{
	return m_pNpcStorage;
}


SPSavePoint* SPEventNpcHandler::LoadSavePointData(int iItemID /*= 0*/)
{
	assert(m_pLDTSavePoint != NULL && "m_pLDTSavePoint is NULL..");

	int recordCount = m_pLDTSavePoint->GetItemCount();
	LDT_Field LDTField;
	int i = 0;

	if(m_pSavePoint->m_iEventID == iItemID) {
		if(m_pSavePoint->m_iEventID){
			g_pInterfaceManager->GetNpcTalkWindow()->SPSendMessage(SPIM_SET_SAVEPOINT);
			SetNpcTalkImage(m_pSavePoint->m_MsgInfo[SAVE_POINT_0].m_iCutInImage);
			return m_pSavePoint;
		}		
	}

	m_pSavePoint->Clear();	

	bool bReturn = true;
	
	for(i = 0; i < SAVE_POINT_MAX; i++) {
		bReturn = m_pLDTSavePoint->GetField( iItemID, 0 + (i * 6),		LDTField);		//_BGM
		if(LDTField.uData.pValue)
			m_pSavePoint->m_MsgInfo[i].m_strBGM = LDTField.uData.pValue;

		bReturn = m_pLDTSavePoint->GetField( iItemID, 1 + (i * 6),		LDTField);		//_NpcName1
		if(LDTField.uData.pValue)
			m_pSavePoint->m_MsgInfo[i].m_strNpcName = LDTField.uData.pValue;

		bReturn = m_pLDTSavePoint->GetField( iItemID, 2 + (i * 6),		LDTField);		//_Message1
		if(LDTField.uData.pValue)
			m_pSavePoint->m_MsgInfo[i].m_strMessage = LDTField.uData.pValue;

		if(i < SAVE_POINT_SELECT){
			if(!m_pSavePoint->m_MsgInfo[i].m_strMessage.empty())
				m_pSavePoint->m_iMsgSize++;
		}
		
		bReturn = m_pLDTSavePoint->GetField( iItemID, 3 + (i * 6),		LDTField);		//_BoxType1		
		m_pSavePoint->m_MsgInfo[i].m_iBoxType = (NPC_TALK_WINDOW_MODE)LDTField.uData.lValue;
		
		bReturn = m_pLDTSavePoint->GetField( iItemID, 4 + (i * 6),		LDTField);		//_CutInImage1
		m_pSavePoint->m_MsgInfo[i].m_iCutInImage = LDTField.uData.lValue;
		
		bReturn = m_pLDTSavePoint->GetField( iItemID, 5 + (i * 6),		LDTField);		//_CutInEffect1
		m_pSavePoint->m_MsgInfo[i].m_iCutInEffect = (NPC_TALK_EFFECT)LDTField.uData.lValue;
	}

	bReturn = m_pLDTSavePoint->GetField( iItemID, 42,		LDTField);		//_Story
	m_pSavePoint->m_iStoryID = LDTField.uData.lValue;
	m_pSavePoint->m_iEventID = iItemID;

	g_pInterfaceManager->GetNpcTalkWindow()->SPSendMessage(SPIM_SET_SAVEPOINT);
	SetNpcTalkImage(m_pSavePoint->m_MsgInfo[SAVE_POINT_0].m_iCutInImage);

	return m_pSavePoint;

	////
	//assert(m_pLDTSavePoint != NULL && "m_pLDTSavePoint is NULL..");
	//
	//int recordCount = m_pLDTSavePoint->GetItemCount();
	//int i = 0;

	//LDT_Field ldtFieldBGM;	
	//
	//LDT_Field ldtFieldMsg;	
	//LDT_Field ldtFieldCutIn;	

	//LDT_Field ldtFieldMessage;
	//LDT_Field ldtFieldImage;

	//LDT_Field ldtFieldMessageYes;
	//LDT_Field ldtFieldImageYes;

	//LDT_Field ldtFieldMessageNo;
	//LDT_Field ldtFieldImageNo;

	//LDT_Field ldtFieldStory ;


	//std::string strBGM = "";
	//strBGM.clear();

	//std::string strMsg[4];	
	//int iCutIn[4];
	//for(i = 0; i < 4; i++) {
	//	strMsg[i] = "";
	//	strMsg[i].clear();
	//	iCutIn[i] = 0;
	//}

	//std::string strMessage = "";
	//strMessage.clear();
	//int iImage = 0;

	//std::string strMessageYes = "";
	//strMessageYes.clear();
	//int iImageYes = 0;

	//std::string strMessageNo = "";
	//strMessageNo.clear();
	//int iImageNo = 0;

	//int iStoryID = 0;

	//m_pLDTSavePoint->GetField(iItemID, 0, ldtFieldBGM);
	//if( m_pLDTSavePoint->CheckLDTField(ldtFieldBGM) )		strBGM = ldtFieldBGM.uData.pValue;
	//
	//for(i = 0; i < 4 ; i++) {
	//	m_pLDTSavePoint->GetField(iItemID, 1+i, ldtFieldMsg);
	//	if( m_pLDTSavePoint->CheckLDTField(ldtFieldMsg) )		strMsg[i] = ldtFieldMsg.uData.pValue;

	//	m_pLDTSavePoint->GetField(iItemID, 5+i, ldtFieldCutIn);
	//	if( m_pLDTSavePoint->CheckLDTField(ldtFieldCutIn) )		iCutIn[i] = ldtFieldCutIn.uData.lValue;
	//}

	//m_pLDTSavePoint->GetField(iItemID, 9, ldtFieldMessage);
	//if( m_pLDTSavePoint->CheckLDTField(ldtFieldMessage) )		strMessage = ldtFieldMessage.uData.pValue;

	//m_pLDTSavePoint->GetField(iItemID, 10, ldtFieldImage);
	//if( m_pLDTSavePoint->CheckLDTField(ldtFieldImage) )		iImage = ldtFieldImage.uData.lValue;

	//m_pLDTSavePoint->GetField(iItemID, 11, ldtFieldMessageYes);
	//if( m_pLDTSavePoint->CheckLDTField(ldtFieldMessageYes) )		strMessageYes = ldtFieldMessageYes.uData.pValue;

	//m_pLDTSavePoint->GetField(iItemID, 12, ldtFieldImageYes);
	//if( m_pLDTSavePoint->CheckLDTField(ldtFieldImageYes) )		iImageYes = ldtFieldImageYes.uData.lValue;

	//m_pLDTSavePoint->GetField(iItemID, 13, ldtFieldMessageNo);
	//if( m_pLDTSavePoint->CheckLDTField(ldtFieldMessageNo) )		strMessageNo = ldtFieldMessageNo.uData.pValue;

	//m_pLDTSavePoint->GetField(iItemID, 14, ldtFieldImageNo);
	//if( m_pLDTSavePoint->CheckLDTField(ldtFieldImageNo) )		iImageNo = ldtFieldImageNo.uData.lValue;

	//m_pLDTSavePoint->GetField(iItemID, 15, ldtFieldStory);
	//if( m_pLDTSavePoint->CheckLDTField(ldtFieldStory) )		iStoryID = ldtFieldStory.uData.lValue;

	//m_pSavePoint->SetSavePoint(iItemID, strBGM, strMessage, iImage, strMessageYes, iImageYes, strMessageNo, iImageNo, iStoryID);
	//
	//int iMsgSize = 0;

	//for(i = 0; i < 4; i++) {
	//	if(strMsg[i] != "") {
	//		m_pSavePoint->m_MsgInfo[i].SetSavePointMsg(strMsg[i], iCutIn[i]);
	//		iMsgSize++;
	//	}
	//}	
	//
	//m_pSavePoint->m_iMsgSize = iMsgSize;
	//
	//g_pInterfaceManager->GetNpcTalkWindow()->SPSendMessage(SPIM_SET_SAVEPOINT);
	//SetNpcTalkImage(m_pSavePoint->m_MsgInfo[0].m_iCutInImge);
	//
	//return m_pSavePoint;
}


SPSavePoint* SPEventNpcHandler::GetSavePointData(int iItemID /*= 0*/)
{
	return m_pSavePoint;
}


SPNpcQuestList* SPEventNpcHandler::LoadNpcQuestDataLDT(int iItemID /*= 0*/)
{
	assert(m_pLDTNpcQuest != NULL && "m_pLDTNpcQuest is NULL..");

	int recordCount = m_pLDTNpcQuest->GetItemCount();
	int i = 0;

	LDT_Field ldtFieldCutIn;
	LDT_Field ldtFieldMsg;		
	LDT_Field ldtFieldQuestID;
	LDT_Field ldtFieldTitle;

	int iImage = 0;
	std::string strMsg = "";
	int iQuestID = 0;
	std::string strTitle = "";
	
	m_pLDTNpcQuest->GetFieldFromLable(iItemID, "_CutInImage",	ldtFieldCutIn);
	iImage = ldtFieldCutIn.uData.lValue;
	
	m_pLDTNpcQuest->GetFieldFromLable(iItemID, "_Message",	ldtFieldMsg);
	if(ldtFieldMsg.uData.pValue)
		strMsg = ldtFieldMsg.uData.pValue;

	m_pLDTNpcQuest->GetFieldFromLable(iItemID, "_Name",	ldtFieldTitle);
	if(ldtFieldTitle.uData.pValue)
		strTitle = ldtFieldTitle.uData.pValue;

	m_pNpcQuestList->SetNpcQuest(m_iNpcID, iImage, strMsg, strTitle);

	char szFieldQuestID[64];	
	for(i = 0; i < MAX_QUEST_LIST; i++) {
		ZeroMemory(szFieldQuestID, 64);
		wsprintf(szFieldQuestID, "_QuestID%d", i+1);		
		m_pLDTNpcQuest->GetFieldFromLable(iItemID, szFieldQuestID,	ldtFieldQuestID);
		iQuestID = ldtFieldQuestID.uData.lValue;
		
		if(iQuestID == 0)
			break;

		m_pNpcQuestList->m_iQuestID[i] = iQuestID;
		m_pNpcQuestList->m_iQuestCount++;
	}	

	g_pEventManager->GetQuestManager()->LoadNpcQuest();

	SetNpcTalkImage(m_pNpcQuestList->m_iCutInImage);

	return NULL;
}


SPNpcQuestList* SPEventNpcHandler::GetNpcQuestData(int iItemID /*= 0*/)
{
	return m_pNpcQuestList;
}

SPNpcGift*	SPEventNpcHandler::LoadNpcGiftDataLDT(int iItemID /*= 0*/)
{
	assert(m_pLDTNpcGift != NULL && "m_pLDTNpcGift is NULL..");

	int recordCount = m_pLDTNpcGift->GetItemCount();	

	LDT_Field ldtFieldImage;
	LDT_Field ldtFieldMessage;

	m_pLDTNpcGift->GetFieldFromLable( iItemID, "_CutInImage",	ldtFieldImage);
	m_pLDTNpcGift->GetFieldFromLable( iItemID, "_Message",		ldtFieldMessage);

	int iImage = ldtFieldImage.uData.lValue;	
	std::string iMessage = "";
	if(ldtFieldMessage.uData.pValue)
		iMessage = ldtFieldMessage.uData.pValue;

	m_pNpcGift->SetNpcGift(m_pNpc->m_iNpcID, iImage, iMessage);	

	SetNpcTalkImage(m_pNpcGift->m_iImage);

	return m_pNpcGift;
}


SPCutInTalk* SPEventNpcHandler::LoadCutInTalkDataLDT(int iItemID /*= 0*/)
{
	if(m_pCutInTalk && m_pCutInTalk->m_iID == iItemID) {
		//m_pCutInTalk->SetNpcID(m_pNpcGuild->m_iNpcID);
		SetNpcTalkImage(m_pCutInTalk->m_iImageID);
		return m_pCutInTalk;
	}

	if(g_pEventManager->GetEventArchive()->GetCutInTalk(iItemID))
	{
		m_pCutInTalk = g_pEventManager->GetEventArchive()->GetCutInTalk(iItemID);
		//m_pCutInTalk->SetNpcID(m_pNpcGuild->m_iNpcID);
		SetNpcTalkImage(m_pCutInTalk->m_iImageID);
		return m_pCutInTalk;
	}

	return NULL;
}

SPCutInTalk* SPEventNpcHandler::GetCutInTalk(int iItemID /*= 0*/)
{
	return m_pCutInTalk;
}

SPNpcGift*	SPEventNpcHandler::GetNpcGift(int iItemID /*= 0*/)
{
	return m_pNpcGift;
}


SPNpcGuild*	SPEventNpcHandler::LoadNpcGuildDataLDT(int iItemID /*= 0*/)
{
	assert(m_pLDTNpcGuild != NULL && "m_pLDTNpcGuild is NULL..");

	int recordCount = m_pLDTNpcGuild->GetItemCount();	

	LDT_Field ldtFieldImage;
	LDT_Field ldtFieldMessage;

	m_pLDTNpcGuild->GetFieldFromLable( iItemID, "_CutInImage",	ldtFieldImage);
	m_pLDTNpcGuild->GetFieldFromLable( iItemID, "_Message",		ldtFieldMessage);

	int iImage = ldtFieldImage.uData.lValue;	
	std::string iMessage = "";
	if(ldtFieldMessage.uData.pValue)
		iMessage = ldtFieldMessage.uData.pValue;

	m_pNpcGuild->SetNpcGuild(m_pNpc->m_iNpcID, iImage, iMessage);	

	SetNpcTalkImage(m_pNpcGuild->m_iImage);

	return m_pNpcGuild;
}


SPNpcGuild*	SPEventNpcHandler::GetNpcGuild(int iItemID /*= 0*/)
{
	return m_pNpcGuild;
}


SPNpcPortal* SPEventNpcHandler::LoadNpcPortalDataLDT(int iItemID /*= 0*/)
{
	if(m_pNpcPortal == NULL)
		return NULL;
	
	//새로 불러들이는것이 지금것과 같다면
	if(m_pNpcPortal->m_iItemID == iItemID)
		return m_pNpcPortal;
	
	SPLDTFile* pLDTPortal;
	SPLDTFile* pLDTPortalInfo;
	
	g_pResourceManager->GetLDTFromFileName(RES_FNAME_LDT_NPCPORTAL, &pLDTPortal);	
	if(pLDTPortal == NULL) 
		return NULL;

	g_pResourceManager->GetLDTFromFileName(RES_FNAME_LDT_PORTAL_DETAIL, &pLDTPortalInfo);
	if(pLDTPortalInfo == NULL) 
		return NULL;
	
	LDT_Field ldtFieldImage;
	LDT_Field ldtFieldMessage;

	pLDTPortal->GetFieldFromLable( iItemID, "_CutInImage",	ldtFieldImage);
	pLDTPortal->GetFieldFromLable( iItemID, "_Message",		ldtFieldMessage);

	int iImage = ldtFieldImage.uData.lValue;
	std::string strCutInMsg = "";
	if(ldtFieldMessage.uData.pValue)
		strCutInMsg = ldtFieldMessage.uData.pValue;

	m_pNpcPortal->SetNpcPortal(iItemID, iImage, strCutInMsg, m_pNpc->m_iNpcID);

	LDT_Field LDTField;
	for(int i = 2; i < 21; i++) {
		pLDTPortal->GetField( iItemID, i,			LDTField);
		if(LDTField.uData.lValue == 0)
			break;
		int iPortalID = LDTField.uData.lValue;

		SPNpcPortalInfo* pPortalInfo = new SPNpcPortalInfo;
		LDT_Field LDTFieldInfo;

		pPortalInfo->m_iItemID = iPortalID;
		
		pLDTPortalInfo->GetField(iPortalID,		0,	LDTFieldInfo);				//_Name
		if(LDTFieldInfo.uData.pValue) {
			pPortalInfo->m_strName = LDTFieldInfo.uData.pValue;
		}

		pLDTPortalInfo->GetField(iPortalID,		1,	LDTFieldInfo);				//_DestinationID
		pPortalInfo->m_iDestID = LDTFieldInfo.uData.lValue;

		pLDTPortalInfo->GetField(iPortalID,		2,	LDTFieldInfo);				//_StageID
		pPortalInfo->m_iStage = LDTFieldInfo.uData.lValue;

		pLDTPortalInfo->GetField(iPortalID,		3,	LDTFieldInfo);				//_MapGroupID
		pPortalInfo->m_iMapGroup = LDTFieldInfo.uData.lValue;

		pLDTPortalInfo->GetField(iPortalID,		4,	LDTFieldInfo);				//_MapID
		pPortalInfo->m_iMapIndex = LDTFieldInfo.uData.lValue;

		int iStat = 0;
		int iStatValue = 0;
		int iValue1 = 0;
		int iValue2 = 0;
		pLDTPortalInfo->GetField(iPortalID,		5,	LDTFieldInfo);				//_Require1_Type
		iStat = LDTFieldInfo.uData.lValue;

		pLDTPortalInfo->GetField(iPortalID,		6,	LDTFieldInfo);				//_Require1_ID
		iStatValue = LDTFieldInfo.uData.lValue;

		pLDTPortalInfo->GetField(iPortalID,		7,	LDTFieldInfo);				//_Require1_Value1
		iValue1 = LDTFieldInfo.uData.lValue;

		pLDTPortalInfo->GetField(iPortalID,		8,	LDTFieldInfo);				//_Require1_Value2
		iValue2 = LDTFieldInfo.uData.lValue;

		pPortalInfo->m_Require[0].SetRequire(iStat, iStatValue, iValue1, iValue2);
		iStat = iStatValue = iValue1 = iValue2 = 0;
		
		pLDTPortalInfo->GetField(iPortalID,		9,	LDTFieldInfo);				//_Require2_Type
		iStat = LDTFieldInfo.uData.lValue;

		pLDTPortalInfo->GetField(iPortalID,		10,	LDTFieldInfo);				//_Require2_ID
		iStatValue = LDTFieldInfo.uData.lValue;

		pLDTPortalInfo->GetField(iPortalID,		11,	LDTFieldInfo);				//_Require2_Value1
		iValue1 = LDTFieldInfo.uData.lValue;

		pLDTPortalInfo->GetField(iPortalID,		12,	LDTFieldInfo);				//_Require2_Value2
		iValue2 = LDTFieldInfo.uData.lValue;
		
		pPortalInfo->m_Require[1].SetRequire(iStat, iStatValue, iValue1, iValue2);

		pLDTPortalInfo->GetField(iPortalID,		13,	LDTFieldInfo);				//_RequireItem
		pPortalInfo->m_iRequireItem = LDTFieldInfo.uData.lValue;
		
		pLDTPortalInfo->GetField(iPortalID,		14,	LDTFieldInfo);				//_ReueireEly
		pPortalInfo->m_iRequireEly = LDTFieldInfo.uData.lValue;

		pLDTPortalInfo->GetField(iPortalID,		15,	LDTFieldInfo);				//_Fx
		pPortalInfo->m_iFxID = LDTFieldInfo.uData.lValue;

		pLDTPortalInfo->GetField(iPortalID,		16,	LDTFieldInfo);				//_Delay
		pPortalInfo->m_fDelay = LDTFieldInfo.uData.lValue;
		pPortalInfo->m_fDelay = pPortalInfo->m_fDelay * 0.1f;

		pLDTPortalInfo->GetField(iPortalID,		17,	LDTFieldInfo);				//_ErrMessage
		if(LDTFieldInfo.uData.pValue) {
			pPortalInfo->m_strErrMsg = LDTFieldInfo.uData.pValue;
		}

		m_pNpcPortal->AddPortalInfo(pPortalInfo);
	}

	SAFE_RELEASE(pLDTPortal);
	SAFE_RELEASE(pLDTPortalInfo);

	SetNpcTalkImage(m_pNpcPortal->m_iCutInImage);
	
	return m_pNpcPortal;
}


SPNpcPortal* SPEventNpcHandler::GetNpcPortal()
{
	return m_pNpcPortal;
}


SPNpcEventList* SPEventNpcHandler::LoadNpcEventListDataLDT(int iItemID /*= 0*/)
{
	if(m_pNpcEventList == NULL)
		return NULL;

	//새로 불러들이는것이 지금것과 같다면
	if(m_pNpcEventList->m_iItemID == iItemID)
		return m_pNpcEventList;	

	SPLDTFile* pLDTNpcEventList;			//LDT NpcEvent List
	SPLDTFile* pLDTNpcEvent;				//LDT NpcEvent
	SPLDTFile* pLDTNpcEventInfo;			//LDT NpcEventInfo

	g_pResourceManager->GetLDTFromFileName(RES_FNAME_LDT_NPCEVENT_LIST, &pLDTNpcEventList);	
	if(pLDTNpcEventList == NULL) 
		return NULL;	

	g_pResourceManager->GetLDTFromFileName(RES_FNAME_LDT_NPCEVENT, &pLDTNpcEvent);	
	if(pLDTNpcEvent == NULL) 
		return NULL;

	g_pResourceManager->GetLDTFromFileName(RES_FNAME_LDT_NPCEVENT_DETAIL, &pLDTNpcEventInfo);
	if(pLDTNpcEventInfo == NULL) 
		return NULL;

	m_pNpcEventList->Clear();

	LDT_Field LDTField;	
	//NPC_EVENT_REWARD_TYPE iType = NPC_EVENT_REWARD_TYPE_NULL;

	bool bReturn = true;
	bReturn = pLDTNpcEventList->GetFieldFromLable( iItemID, "_CutInImage",	LDTField);
	if(!bReturn) 
		return NULL;	
	
	m_pNpcEventList->m_iCutImage = LDTField.uData.lValue;

	bReturn = pLDTNpcEventList->GetFieldFromLable( iItemID, "_Message",		LDTField);
	if(!bReturn)
		return NULL;
	
	if(LDTField.uData.pValue)
		m_pNpcEventList->m_strMessage = LDTField.uData.pValue;
	
	int iNpcEventID = 0;
	for(int i = 0; i < MAX_NPC_EVENT_LIST; i++) {
		pLDTNpcEventList->GetField(iItemID, 2 + i,		LDTField);				//_event1 ~ 50
		iNpcEventID = LDTField.uData.lValue;
		if(iNpcEventID)	{
			LoadNpcEventDataLDT(pLDTNpcEvent, pLDTNpcEventInfo, iNpcEventID);
		}
	}

	SetNpcTalkImage(m_pNpcEventList->m_iCutImage);

	SAFE_RELEASE(pLDTNpcEventList);
	SAFE_RELEASE(pLDTNpcEvent);
	SAFE_RELEASE(pLDTNpcEventInfo);

	return m_pNpcEventList;
}



SPNpcEvent*	SPEventNpcHandler::LoadNpcEventDataLDT(SPLDTFile* pLDTNpcEvent, SPLDTFile* pLDTNpcEventInfo, int iItemID /*= 0*/)
{
	if(m_pNpcEventList == NULL)
		return NULL;
	
	//if(m_pNpcEvent == NULL)
	//	return NULL;

	//새로 불러들이는것이 지금것과 같다면
	//if(m_pNpcEvent->m_iItemID == iItemID)
	//	return m_pNpcEvent;	

	SPNpcEvent* pNpcEvent = new SPNpcEvent;

	LDT_Field LDTField;
	//int iImage = 0;
	//std::string strCutInMsg = "";
	std::string strName = "";
	NPC_EVENT_REWARD_TYPE iType = NPC_EVENT_REWARD_TYPE_NULL;
	
	//pLDTNpcEvent->GetFieldFromLable( iItemID, "_CutInImage",	LDTField);
	//iImage = LDTField.uData.lValue;
	
	//pLDTNpcEvent->GetFieldFromLable( iItemID, "_Message",		LDTField);		
	//if(LDTField.uData.pValue)
	//	strCutInMsg = LDTField.uData.pValue;

	pLDTNpcEvent->GetField( iItemID, 0,		LDTField);				//_EventName
	//if(LDTField.uData.pValue)
	if(LDTField.nFldTyp == 1  && pLDTNpcEvent->CheckLDTField(LDTField))
		strName = LDTField.uData.pValue;

	//[2008/6/17] - 문자열 길이 테스트용
	//strName = "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW";

	pLDTNpcEvent->GetField( iItemID, 1,		LDTField);				//_RewardType
	iType = static_cast<NPC_EVENT_REWARD_TYPE>(LDTField.uData.lValue);
	
	pNpcEvent->SetNpcEvent(iItemID, strName, m_pNpc->m_iNpcID, iType);

	int iStat = 0;
	int iStatValue = 0;
	int iValue1 = 0;
	int iValue2 = 0;
	pLDTNpcEvent->GetField( iItemID, 2,		LDTField);				//_Require1_Type
	iStat = LDTField.uData.lValue;

	pLDTNpcEvent->GetField( iItemID, 3,		LDTField);				//_Require1_ID
	iStatValue = LDTField.uData.lValue;

	pLDTNpcEvent->GetField( iItemID, 4,		LDTField);				//_Require1_Value1
	iValue1 = LDTField.uData.lValue;

	pLDTNpcEvent->GetField( iItemID, 5,		LDTField);				//_Require1_Value2
	iValue2 = LDTField.uData.lValue;
	pNpcEvent->m_Require[0].SetRequire(iStat, iStatValue, iValue1, iValue2);

	iStat = 0;
	iStatValue = 0;
	iValue1 = 0;
	iValue2 = 0;
	pLDTNpcEvent->GetField( iItemID, 6,		LDTField);				//_Require2_Type
	iStat = LDTField.uData.lValue;

	pLDTNpcEvent->GetField( iItemID, 7,		LDTField);				//_Require2_ID
	iStatValue = LDTField.uData.lValue;

	pLDTNpcEvent->GetField( iItemID, 8,		LDTField);				//_Require2_Value1
	iValue1 = LDTField.uData.lValue;

	pLDTNpcEvent->GetField( iItemID, 9,		LDTField);			//_Require2_Value2
	iValue2 = LDTField.uData.lValue;
	pNpcEvent->m_Require[0].SetRequire(iStat, iStatValue, iValue1, iValue2);
	
	pLDTNpcEvent->GetField( iItemID, 10,		LDTField);			//_RequireItem1
	pNpcEvent->m_RequireItem[0].m_iItemID = LDTField.uData.lValue;

	pLDTNpcEvent->GetField( iItemID, 11,		LDTField);			//_Count1
	pNpcEvent->m_RequireItem[0].m_iStack = LDTField.uData.lValue;

	pLDTNpcEvent->GetField( iItemID, 12,		LDTField);			//_RequireItem2
	pNpcEvent->m_RequireItem[1].m_iItemID = LDTField.uData.lValue;

	pLDTNpcEvent->GetField( iItemID, 13,		LDTField);			//_Count2
	pNpcEvent->m_RequireItem[1].m_iStack = LDTField.uData.lValue;

	pLDTNpcEvent->GetField( iItemID, 14,		LDTField);			//_ReueireEly
	pNpcEvent->m_iRequireEly = LDTField.uData.lValue;

	pLDTNpcEvent->GetField( iItemID, 15,		LDTField);			//_ErrMessage
	if(LDTField.uData.pValue)
		pNpcEvent->m_strErr = LDTField.uData.pValue;

	for(int i = 0; i < 50; i++) {
		int iInfoID = 0;		
		pLDTNpcEvent->GetField(iItemID, 16 + (i * 2),	LDTField);	//_Item1
		if(LDTField.uData.lValue == 0)
			break;
		
		iInfoID = LDTField.uData.lValue;

		SPNpcEventInfo* pEventInfo = new SPNpcEventInfo;
		LDT_Field LDTFieldInfo;

		pEventInfo->m_iID = iInfoID;

		pLDTNpcEventInfo->GetField(iInfoID,		0,	LDTFieldInfo);				//_ItemID
		pEventInfo->m_RewordItem.m_iItemID = LDTFieldInfo.uData.lValue;

		pLDTNpcEventInfo->GetField(iInfoID,		1,	LDTFieldInfo);				//_ItemName
		if(LDTFieldInfo.uData.pValue) {
			pEventInfo->m_strTitleName = LDTFieldInfo.uData.pValue;
		}

		pLDTNpcEventInfo->GetField(iInfoID,		2,	LDTFieldInfo);				//_Icon
		pEventInfo->m_iIcon = LDTFieldInfo.uData.lValue;

		pLDTNpcEventInfo->GetField(iInfoID,		3,	LDTFieldInfo);				//_IconIndex
		pEventInfo->m_iIconIndex = LDTFieldInfo.uData.lValue;

		pLDTNpcEventInfo->GetField(iInfoID,		4,	LDTFieldInfo);				//_ItemRare
		pEventInfo->m_RewordItem.m_iRear = LDTFieldInfo.uData.lValue;

		pLDTNpcEventInfo->GetField(iInfoID,		5,	LDTFieldInfo);				//_ItemCount
		pEventInfo->m_RewordItem.m_iStack = LDTFieldInfo.uData.lValue;

		pLDTNpcEventInfo->GetField(iInfoID,		6,	LDTFieldInfo);				//_Ely
		pEventInfo->m_iEly = LDTFieldInfo.uData.lValue;

		pLDTNpcEventInfo->GetField(iInfoID,		7,	LDTFieldInfo);				//_Description
		if(LDTFieldInfo.uData.pValue) {
			pEventInfo->m_strDesc = LDTFieldInfo.uData.pValue;
		}
		
		iStat = 0;
		iStatValue = 0;
		iValue1 = 0;
		iValue2 = 0;
		pLDTNpcEventInfo->GetField(iInfoID,		8,	LDTFieldInfo);				//_Require1_Type
		iStat = LDTFieldInfo.uData.lValue;

		pLDTNpcEventInfo->GetField(iInfoID,		9,	LDTFieldInfo);				//_Require1_ID
		iStatValue = LDTFieldInfo.uData.lValue;

		pLDTNpcEventInfo->GetField(iInfoID,		10,	LDTFieldInfo);				//_Require1_Value1
		iValue1 = LDTFieldInfo.uData.lValue;

		pLDTNpcEventInfo->GetField(iInfoID,		11,	LDTFieldInfo);				//_Require1_Value2
		iValue2 = LDTFieldInfo.uData.lValue;
		pEventInfo->m_Require[0].SetRequire(iStat, iStatValue, iValue1, iValue2);

		iStat = 0;
		iStatValue = 0;
		iValue1 = 0;
		iValue2 = 0;
		pLDTNpcEventInfo->GetField(iInfoID,		12,	LDTFieldInfo);				//_Require2_Type
		iStat = LDTFieldInfo.uData.lValue;

		pLDTNpcEventInfo->GetField(iInfoID,		13,	LDTFieldInfo);				//_Require2_ID
		iStatValue = LDTFieldInfo.uData.lValue;

		pLDTNpcEventInfo->GetField(iInfoID,		14,	LDTFieldInfo);				//_Require2_Value1
		iValue1 = LDTFieldInfo.uData.lValue;

		pLDTNpcEventInfo->GetField(iInfoID,		15,	LDTFieldInfo);				//_Require2_Value2
		iValue2 = LDTFieldInfo.uData.lValue;
		pEventInfo->m_Require[1].SetRequire(iStat, iStatValue, iValue1, iValue2);

		pLDTNpcEventInfo->GetField(iInfoID,		16,	LDTFieldInfo);				//_RequireItem1
		pEventInfo->m_RequireItem[0].m_iItemID = LDTFieldInfo.uData.lValue;

		pLDTNpcEventInfo->GetField(iInfoID,		17,	LDTFieldInfo);				//_Count1
		pEventInfo->m_RequireItem[0].m_iStack = LDTFieldInfo.uData.lValue;

		pLDTNpcEventInfo->GetField(iInfoID,		18,	LDTFieldInfo);				//_RequireItem2
		pEventInfo->m_RequireItem[1].m_iItemID = LDTFieldInfo.uData.lValue;
		
		pLDTNpcEventInfo->GetField(iInfoID,		19,	LDTFieldInfo);				//_Count2
		pEventInfo->m_RequireItem[1].m_iStack = LDTFieldInfo.uData.lValue;

		pLDTNpcEventInfo->GetField(iInfoID,		20,	LDTFieldInfo);				//_ReueireEly
		pEventInfo->m_iRequireEly = LDTFieldInfo.uData.lValue;

		pLDTNpcEventInfo->GetField(iInfoID,		21,	LDTFieldInfo);				//_ErrMessage
		if(LDTFieldInfo.uData.pValue) {
			pEventInfo->m_strErr = LDTFieldInfo.uData.pValue;
		}

		pLDTNpcEventInfo->GetField(iInfoID,		22,	LDTFieldInfo);				//_RequirePpt
		pEventInfo->m_iRequirePvpPoint = LDTFieldInfo.uData.lValue;

		pNpcEvent->AddEventInfo(pEventInfo);
	}

	pLDTNpcEvent->GetField( iItemID, 116,		LDTField);			//_RequirePpt	
	pNpcEvent->m_iRequirePvpPoint = LDTField.uData.lValue;	

	if(m_pNpcEventList->AddNpcEvent(pNpcEvent) == false) {
		SAFE_DELETE(pNpcEvent);
	}
	
	//SetNpcTalkImage(m_pNpcEvent->m_iCutImage);	
	//return m_pNpcEvent;
	return NULL;
}


SPNpcEventList* SPEventNpcHandler::GetNpcEventList()
{
	return m_pNpcEventList;
}


SPNpcEvent* SPEventNpcHandler::GetNpcEvent()
{
	if(m_pNpcEventList) {
		return m_pNpcEventList->GetSelectNpcEvent();
	}
	
	return NULL;
}


