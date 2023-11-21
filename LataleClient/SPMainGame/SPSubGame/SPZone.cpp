#include "SPCommon.h"
#include "DXUtil.h"

#include "SPManager.h"
#include "SPMouseCursor.h"
#include "SPIMEPool.h"

#include "SPSubGameManager.h"
#include "SPMainGameManager.h"
#include "SPWindowDEF.h"
#include "SPInterfaceManager.h"
#include "SPTerrainDEF.h"
#include "SPTerrainCluster.h"
#include "SPTerrainManager.h"
#include "SPWindow.h"

#include "SPGOBManager.h"
#include "SPRenderModelDEF.h"
#include "SPRenderModel.h"
#include "SPFXDEF.h"
#include "SPFXModel.h"
#include "SPFXManager.h"

//
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGOBModel.h"
#include "SPGOBModelUnit.h"
#include "SPAvatarModelUnit.h"
#include "SPGOBCluster.h"
#include "SPGameObject.h"
#include "SPGOBStatus.h"
#include "SPGOBCoordPhysics.h"
#include "SPGOBModelUnit.h"			
#include "SPPlayerEquipInfo.h"
#include "SPGOBStatus.h"
#include "SPPlayerStatus.h"
#include "SPPlayerInvenArchive.h"	
#include "SPPlayerStatusModel.h"	
#include "SPPlayerEquipInfo.h"
#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPItemCluster.h"
#include "SPSkill.h"
#include "SPSkillArchive.h"
#include "SPPlayer.h"
#include "SPGOBManager.h"
#include "SPCommandConvert.h"

#include "SPWindowBeExact.h"

#include "SPEventDEF.h"
#include "SPEventNpcHandler.h"
#include "SPEventManager.h"			//[2005/5/9] - jinhee
#include "SPCashTradeManager.h"

#include "SPUtil.h"
#include "SPResourceDef.h"
#include "SPZone.h"

#include "SPStage.h"				//[2005/5/31]
#include "SPStageManager.h"

#include "SPCoolTimeManager.h"
#include "SPComboManager.h"

#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"
#include "SockWnd.h"
#include "SPNetworkManager.h"
#include "LT_Error.h"

#include "SPResourceManager.h"
#include "SPCheckManager.h"
#include "SPStoryManager.h"

#include "SPSoundArchive.h"
#include "SPSoundManager.h"

#include "SPLocalizeManager.h"
#include "GlobalDefines_Share.h"

#include "SPRefOLE.h" // for IngameWB
#include "SPIngameWebBrowser.h"

#include "SPGuildArchive.h"
#include "SPGuildLDT.h"

#include "SPIndun.h"
#include "SPIndunManager.h"

#include "SPItem.h"

#include "SPPvpManager.h"

#include "_Error_log.h"
#include "lgclientlog.h"
#include "SPWindowPushprop.h"

#include "AutoInterface.h"


extern WORLD_ID g_uiWorldID;

SPZone::SPZone()
: SPSubGameManager	(SUBGAME_ZONE)
, m_bActive			(false)
, m_iAlpha			(MAX_FADE_ALPHA)
, m_bSendDirectExit	(false)
, m_iNextState		(GS_NULL)
, m_pPreloadSymbol	(NULL)
, m_pPreloadBG		(NULL)
{	
}

SPZone::~SPZone() 
{	
}

bool SPZone::Init()
{		

	//Version Check
	//{
	//if(g_pNetworkManager->CheckVersion(VER_MAJOR,VER_MINOR))
	//{
	//	m_strLog = "Version Checking...";
	//} 
	//else 
	//{
	//	m_strLog = "Version Checking : FAILED";
	//}
	//}


	//g_pNetworkManager->SPPerformMessage(GAME_CS_START);
	//OnSetBGM( (WPARAM)"DATA/SOUND/BELOS.MP3", 0);

	m_bChannelWait = false;
	m_fChannelWaitTime = 0.0f;
	m_iAlpha = MAX_FADE_ALPHA;

	m_bChannelChangeRetry = false;
	m_iChannelChangeRetryCount = 0;

	std::string strTextureFile = "";
	strTextureFile.clear();
	strTextureFile = "DATA/INTERFACE/CONCEPT/UI100.PNG";
	g_pVideo->GetTextureMgr()->LoadTexture(strTextureFile.c_str(), &m_pTexture);

	//SetRect(&m_rtSrcFade, 128, 392, 132, 396);
	SetRect(&m_rtSrcFade, 255, 21, 257, 23);
	SetRect(&m_rtDestFade, 0, 0, 800, 600);
	m_fAccumulateRender = 0.0f;
	m_fCurDelay = 0.02f;					//∞‘¿Ã¡ÅEø√∏Æ¥¬ º”µµ ¡∂¿˝...

	m_iChangeServerID = 0;
	m_bCashShopInReq = false;
	m_bSendIndunInReq = false ;
	m_bLotteryInReq = false;

	m_iChannelChangeType = 0;

	if(g_pNetworkManager->IsChannelEntering())
	{
		g_pNetworkManager->SetChannelEnteringFlag(false);
		g_pNetworkManager->GameChannel_IN();
	}

	//if( SPLocalizeManager::GetInstance()->GetCountryType() == CT_CHINA && !g_pNetworkManager->FDS_IsShowChat() )
	//{
	//	// MainGameManagerø°º≠ SubGame->Init() ¿Ã»ƒ InterfaceManager ¿« SetSubGame¿ª «œ±‚∂ßπÆø°
	//	// ¡˜¡¢ ¬Ô¿ªºˆ¥¬ æ¯∞Ì, ∆–≈∂¿ª πﬁ¿∫ ¿Ã»ƒø° ¬Ô»˘¥Ÿ.
	//	g_pNetworkManager->FDS_QueryState() ;
	//	// g_pNetworkManager->FDS_SetChat(true) ;
	//}

	//---------------------------------------------------------------------
	// PreLoad Guild Emblem Icons
	std::string strFileName ;
	if( g_pkGuildManager->GetSymbolFilename() != NULL ) {
		strFileName = g_pkGuildManager->GetSymbolFilename() ;
		g_pVideo->GetTextureMgr()->LoadTexture(strFileName.c_str(), &m_pPreloadSymbol);

	}
	if( g_pkGuildManager->GetEmblemBGFilename() != NULL ) {
		strFileName = g_pkGuildManager->GetEmblemBGFilename() ;
		g_pVideo->GetTextureMgr()->LoadTexture(strFileName.c_str(), &m_pPreloadBG);
	}
	

	return SPSubGameManager::Init();
}

void SPZone::Clean()
{
	g_pInterfaceManager->ForceReply();

	g_pGOBManager->PurgeAll();
	g_pTerrainManager->PurgeAll();
	g_pInterfaceManager->PurgeAll();

	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pPreloadSymbol) ;
	SAFE_RELEASE(m_pPreloadBG) ;

	SPSubGameManager::Clean();
}

bool SPZone::WndProc(UINT message, WPARAM wParam, LPARAM lParam)
{	
	bool bResult = false;	
	
	(CAutoInterface::GetInstance().WndProc(message, wParam, lParam) > 0) ? bResult = true : bResult = false;	

	(g_pInterfaceManager->WndProc(message, wParam, lParam) > 0) ? bResult = true : bResult = false;	
	
	if(!bResult)	
		(g_pGOBManager->WndProc(message, wParam, lParam) > 0) ?  bResult = true : bResult = false;//[2005/5/6] - jinhee	
	
	switch (message) {
	case WM_CLOSE:						
		if(g_pGOBManager->GetLocalPlayer()) {							//[2005/10/17]
			if(g_pGOBManager->GetLocalPlayer()->GetTradeState() == TRADE_STATE_TRADE) {		//∞≈∑°¡ﬂ¿Œ ∞ÊøÅE√ÅE?
				CPacket Packet(TRADE_CS_CANCEL);
				g_pNetworkManager->SPPerformMessage(TRADE_CS_CANCEL, 0, (LPARAM)&Packet);				
			}
			
			if(g_pEventManager->GetCutInState() ==  CUT_IN_STATE_SAVEPOINT) {				//Save Point¿« ∞ÊøÅEBGM ∫Ø∞ÅE
				if(g_pEventManager->GetEventHandler()->GetNpcID() == 0) {
					int iStage = g_pGOBManager->GetLocalPlayer()->GetCurStageID();
					int iGroup = g_pGOBManager->GetLocalPlayer()->GetCurMapGroupID();		
					std::string strBGM = g_StageManager.GetCurStage(iStage)->GetMapGroup(iGroup)->m_strBGMFile;
					int iSoundEffect = g_StageManager.GetCurStage(iStage)->GetMapGroup(iGroup)->m_iSoundEffectType;

					if(strBGM != ""){
						g_pSubGameManager->SPPerformMessage(SPGM_SUG_SETBGM , (WPARAM)strBGM.c_str());
						SPSoundBase::GetInstance()->SetRoomType(iSoundEffect);
					}
				}
			}
		}
		
		g_pEventManager->SetCutInState(CUT_IN_STATE_NULL);				//[2005/10/10]

		//////////////////////////////////////////////////////////////////////////
		//
		//	∞‘¿” ∞°µÂø°º≠ ¡æ∑ÅE∏ﬁºº¡ÅE≥Ø∏±∂ß!!
		BOOL	bAppExit	=	(BOOL)wParam;
		int		iAppExit	=	(int)lParam;

		//SPZone ø°º≠ Alt+F4 ∑Œ ¡◊∞‘≤ÅE
		DirectExit();
		//if( bAppExit == TRUE && iAppExit == 1 )
		//	DirectExit();
		//else
		//	g_pInterfaceManager->ShowWindow(WIID_EXIT, SP_NONE, SP_TOGGLE_ON);

		return true;
	}
	return bResult;
}

void SPZone::Process(float fTime)
{
	if( m_bChannelWait || m_bChannelChangeRetry )
	{
		m_fChannelWaitTime += fTime;
		if( m_fChannelWaitTime > 3.0f )
		{
			if( m_bChannelWait )
			{
				string strGameServerIP = g_pNetworkManager->GetGameServerIP().c_str();
				int iGameServerPort = g_pNetworkManager->GetGameServerPort();
				if( !g_pNetworkManager->ConnectToServer(strGameServerIP.c_str(), iGameServerPort) )
					SPPerformMessage(SPGM_SUG_SOCKET_FAILED, 0, 0);
				
				m_bChannelWait = false;
				m_fChannelWaitTime = 0.0f;
			}
			else if( m_bChannelChangeRetry )
			{
				g_pNetworkManager->UserInRequest();
				m_iChannelChangeRetryCount++;
				m_fChannelWaitTime = 0.0f;
				m_bChannelChangeRetry = false;
			}
		}

		//return;
	}

	switch(m_iGameState) {
	case GS_GAME:
		{	
			g_pNetworkManager->Process(fTime);

			if ( CAutoInterface::GetInstance().isStart() )
				CAutoInterface::GetInstance().Process(fTime);
			else
				g_pInputManager->Process(fTime);
			
			if( m_iGameState != GS_CASHSHOP || m_iGameState != GS_STORYVIEW || m_iGameState != GS_BOOKVIEW)
				g_pCameraManagr->Process(fTime);

			g_pGOBManager->Process(fTime);		
			g_pInterfaceManager->Process(fTime);	
			g_pTerrainManager->Process(fTime);
			g_pFXManager->Process(fTime);
			g_pEventManager->Process(fTime);		//[2005/5/9] - jinhee

			SPCoolTimeManager::GetInstance()->Process(fTime);
			SPComboManager::GetInstance()->Process(fTime);
		}
		break;
	case GS_RELEASE_READY:
		{
			m_fWaitTime -= fTime;
			if(m_fWaitTime <= 0) {
				m_iGameState = GS_RELEASE;				

				if( m_NextSubGameID	== SUBGAME_LOGGIN )
				{	
					g_pNetworkManager->Disconnect2();

				}
				g_pInterfaceManager->SPChildWndSendMessage(WIID_EXIT, SPIM_BTN_ENABLE, 1, 0);
				//µ«≥ˆ
				if ( g_pCIGWLoaderDx9 )
				{
					g_pCIGWLoaderDx9->LogoutA();
				}
			} else {
				g_pNetworkManager->Process(fTime);
				g_pInputManager->Process(fTime);
				g_pGOBManager->Process(fTime);		
				g_pInterfaceManager->Process(fTime);	
				g_pCameraManagr->Process(fTime);
				g_pTerrainManager->Process(fTime);
				g_pFXManager->Process(fTime);
			}
		}
		break;
	case GS_FADE_IN:
		g_pNetworkManager->Process(fTime);
		g_pInputManager->Process(fTime);
		g_pCameraManagr->Process(fTime);
		g_pGOBManager->Process(fTime);		
		g_pInterfaceManager->Process(fTime);	
		g_pTerrainManager->Process(fTime);
		g_pFXManager->Process(fTime);
		g_pEventManager->Process(fTime);		//[2005/5/9] - jinhee

		SPCoolTimeManager::GetInstance()->Process(fTime);
		SPComboManager::GetInstance()->Process(fTime);

		//if(m_fLastRenderDelay > 0.0f && m_fLastRenderDelay < fTime) {
		//	//fTime -= m_fLastRenderDelay;
		//	fTime = 0.0f;
		//}

		m_fAccumulateRender += fTime;
		if( m_fAccumulateRender > m_fCurDelay) {
			m_fAccumulateRender -= m_fCurDelay;						
			//ProcessFade(fTime);
			ProcessNewFade(fTime);
		}
		break;
	case GS_FADE_OUT:
		g_pNetworkManager->Process(fTime);
		g_pInputManager->Process(fTime);
		g_pCameraManagr->Process(fTime);
		g_pGOBManager->Process(fTime);		
		g_pInterfaceManager->Process(fTime);	
		g_pTerrainManager->Process(fTime);
		g_pFXManager->Process(fTime);
		g_pEventManager->Process(fTime);		//[2005/5/9] - jinhee

		SPCoolTimeManager::GetInstance()->Process(fTime);
		SPComboManager::GetInstance()->Process(fTime);

		//if(m_fLastRenderDelay > 0.0f && m_fLastRenderDelay < fTime) {
		//	//fTime -= m_fLastRenderDelay;
		//	fTime = 0.0f;
		//}

		m_fAccumulateRender += fTime;
		if( m_fAccumulateRender > m_fCurDelay) {
			m_fAccumulateRender -= m_fCurDelay;						
			//ProcessFade(fTime);
			ProcessNewFade(fTime);
		}
		
		break;
	case GS_STAGE_CHANGE:								//[2005/5/30] - jinhee
		m_fWaitTime -= fTime;
		if(m_fWaitTime <= 0) {
			m_iGameState = GS_STAGE_CHANGE;
			//m_NextSubGameID	= SUBGAME_LOADING;
		} else {
			g_pNetworkManager->Process(fTime);
			g_pInputManager->Process(fTime);
			//g_pGOBManager->Process(fTime);		
			g_pInterfaceManager->Process(fTime);	
			g_pCameraManagr->Process(fTime);
			g_pTerrainManager->Process(fTime);
			g_pFXManager->Process(fTime);
		}
		break;

	case GS_STORYVIEW:		// Only Stop -> Watch StoryView
	case GS_BOOKVIEW:		// Only Stop -> Watch StoryView
		if(g_pGOBManager->GetLocalPlayer()) {
			if((g_pGOBManager->GetLocalPlayer()->GetCurAction() != ACTION_STOP)){
				//g_pInterfaceManager->SPChildWndSendMessage(WIID_STORY, SPIM_STORY_FORCE_EXIT, 0, 0);
				if( m_iGameState == GS_STORYVIEW )
					OnSetStoryView(-1, 0) ;
				else if( m_iGameState == GS_BOOKVIEW )
					OnSetTextView(-1, 0) ;
			}
		}
	case GS_CASHSHOP:
		g_pNetworkManager->Process(fTime);
		g_pInputManager->Process(fTime);
		g_pCameraManagr->Process(fTime);   //[xialin 2008/07/29] add
		g_pGOBManager->Process(fTime);
		g_pInterfaceManager->Process(fTime);	
		g_pTerrainManager->Process(fTime);  //[xialin 2008/07/29] add
		g_pFXManager->Process(fTime);
		g_pEventManager->Process(fTime);

		SPCoolTimeManager::GetInstance()->Process(fTime);
		SPComboManager::GetInstance()->Process(fTime);
		break;

	case GS_LOTTERY:
		g_pNetworkManager->Process(fTime);
		g_pInputManager->Process(fTime);
		g_pGOBManager->Process(fTime);
		g_pInterfaceManager->Process(fTime);	
		g_pFXManager->Process(fTime);
		g_pEventManager->Process(fTime);

		SPCoolTimeManager::GetInstance()->Process(fTime);
		SPComboManager::GetInstance()->Process(fTime);
		break;

	case GS_PVPWAIT:		// PVP LOAD WAIT
		g_pNetworkManager->Process(fTime);
		g_pInputManager->Process(fTime);
		g_pGOBManager->Process(fTime);
		g_pInterfaceManager->Process(fTime);	
		g_pFXManager->Process(fTime);
		g_pEventManager->Process(fTime);

		SPCoolTimeManager::GetInstance()->Process(fTime);
		SPComboManager::GetInstance()->Process(fTime);
		break ;
	}
}


void SPZone::ProcessFade(float fTime)
{
	if(m_iGameState == GS_FADE_IN) {						//æ˚—Œæ˚?E
		if(m_iAlpha < MAX_FADE_ALPHA) {
			m_iAlpha += 10;
			
			////
			//if(g_pGOBManager->GetLocalPlayer()) {
			//	//int iStage = g_pGOBManager->GetLocalPlayer()->GetCurStageID();
			//	//int iGroup = g_pGOBManager->GetLocalPlayer()->GetCurMapGroupID();
			//	//g_StageManager.SetCurMapIndex(0, iGroup, iStage);
			//	//g_pGOBManager->GetLocalPlayer()->SetPos(100, 100);
			//}
			//g_StageManager.SetCurGroupIndex(0);			
		}

		if(m_iAlpha >= MAX_FADE_ALPHA) {					//øœ¿ÅEÅEæ˚—Œæ˚?E
			m_iAlpha = MAX_FADE_ALPHA;
			m_iGameState = GS_STAGE_CHANGE;
			m_NextSubGameID	= SUBGAME_LOADING;
		}
	}
	else if(m_iGameState == GS_FADE_OUT) {					//π‡æ∆¡ÅE
		if(m_iAlpha > 0) {
			m_iAlpha -= 10;
		}
		if(m_iAlpha < 1) {									//øœ¿ÅEÅEπ‡æ∆¡ÅE		
			//g_pGOBManager->OnSendEnterStageMessage((WPARAM)0, (LPARAM)0);		//∑Œµ˘»ƒ º≠πˆø° ∑ŒµÅEøœ∑ÅE∏ﬁΩ√¡ÅE¿ÅE?				
			//SPGM_EVENT_CS_STAGE_ENTER
			//g_pGOBManager->SPPerformMessage(SPGM_EVENT_CS_STAGE_ENTER);
			m_iAlpha = 0;
			m_iGameState = GS_GAME;
			if(SPCashTradeManager::GetInstance()->IsSocketFail()) {
				OnSocketFailed((WPARAM)0, (LPARAM)0);
			}
		}
	}	

	//Render(fTime);
}




void SPZone::ProcessNewFade(float fTime)
{
	if(m_iGameState == GS_FADE_IN) {						//EnDark
		if(m_iAlpha < MAX_FADE_ALPHA) {
			m_iAlpha += 10;
		}
		if(m_iAlpha >= MAX_FADE_ALPHA) {
			m_iAlpha = MAX_FADE_ALPHA ;
			m_iGameState = m_iNextState ;
			if( m_iNextState == GS_STAGE_CHANGE )	// If Stage Change -> Subgame Loading
				m_NextSubGameID	= SUBGAME_LOADING;
			m_iNextState = GS_NULL ;
			if( m_iGameState == GS_STORYVIEW || m_iGameState == GS_BOOKVIEW )
				m_iAlpha = 0 ;
		}
	}
	else if(m_iGameState == GS_FADE_OUT) {					// EnLight
		if(m_iAlpha > 0) {
			m_iAlpha -= 10;
		}
		if(m_iAlpha < 1) {
			m_iAlpha = 0;
			m_iGameState = m_iNextState;
			if( m_iGameState == GS_NULL ){
				m_iGameState = GS_GAME ;		// Default
				if(SPCashTradeManager::GetInstance()->IsSocketFail()) {
					OnSocketFailed((WPARAM)0, (LPARAM)0);
				}
			}
			m_iNextState = GS_NULL ;
		}
	}	
}


/**
	Fade ¿Ã»ƒ ¥Ÿ¿Ω State∏¶ º≥¡§«“ºˆ ¿÷µµ∑œ ¿Á±∏º∫
*/
void SPZone::SetFadeState(GAME_STATE eCurState, GAME_STATE eNextState)
{
	ErrorFLogs->Error_Log("SPZone::SetFadeState Current[%d]->[%d]",eCurState, eNextState) ;
	m_iGameState = eCurState ;
	m_iNextState = eNextState ;
	if( m_iGameState == GS_FADE_IN )
		m_iAlpha = 0 ;
	else if( m_iGameState == GS_FADE_OUT )
		m_iAlpha = MAX_FADE_ALPHA ;
}


void SPZone::Render(float fTime)
{
	if(m_iGameState == GS_STAGE_CHANGE) {
		//
		//DXUTOutputDebugString("SPZone::Render - GS_STAGE_CHANGE\n");	
		
		////		
		//g_StageManager.SetCurGroupIndex(0);			//¿”Ω√∑Œ ª—∑¡¡Ÿ πË∞Ê¿”...
		if(g_pGOBManager->GetLocalPlayer()) {
			g_pTerrainManager->Render(fTime);	
			g_pTerrainManager->RenderLayer2(fTime);	
			g_pTerrainManager->RenderLayer3(fTime);	
			g_pTerrainManager->RenderForeGround(fTime);
		}		
		//g_pFXManager->Render(fTime);
		//g_pInterfaceManager->Render(fTime);	
	}
	else if(m_iGameState == GS_FADE_IN) {
		g_pTerrainManager->Render(fTime);
		g_pTerrainManager->RenderLayer2(fTime);	
		g_pTerrainManager->RenderLayer3(fTime);	
		g_pTerrainManager->RenderForeGround(fTime); //[2005/3/25] - jinhee
		g_pFXManager->Render(fTime);		
		g_pInterfaceManager->Render(fTime);	
		//g_pEventManager->Render(fTime);				//Interface Renderø°º≠ ∑ª¥ÅE
	}
	else if(m_iGameState == GS_CASHSHOP) {
		g_pInterfaceManager->Render(fTime);	
	}
	else if(m_iGameState == GS_LOTTERY) {
		g_pInterfaceManager->Render(fTime);	
	}
	else if(m_iGameState == GS_STORYVIEW) {
		g_pInterfaceManager->Render(fTime);	
	}
	else if(m_iGameState == GS_BOOKVIEW) {
		g_pInterfaceManager->Render(fTime);	
	}
	else if (m_iGameState == GS_PVPWAIT)	{
		g_pInterfaceManager->Render(fTime);	
	}
	else {
		g_pTerrainManager->Render(fTime);	
		g_pGOBManager->Render(fTime);	

		g_pTerrainManager->RenderLayer2(fTime);	
		g_pGOBManager->RenderLayer2(fTime);	

		g_pTerrainManager->RenderLayer3(fTime);	
		g_pGOBManager->RenderLayer3(fTime);	

		g_pTerrainManager->RenderForeGround(fTime); //[2005/3/25] - jinhee

		CAutoInterface::GetInstance().Render(fTime);

		//g_pFXManager->Render(fTime);	
		g_pGOBManager->RenderFX( fTime , FXM_RENDER_LAYER_UI_BACK );

		g_pInterfaceManager->Render(fTime);		

		g_pGOBManager->RenderFX( fTime , FXM_RENDER_LAYER_UI_FRONT );

		//g_pEventManager->Render(fTime);				//Interface Renderø°º≠ ∑ª¥ÅE
	}

	if(m_pTexture && m_iAlpha > 0) {
		m_pTexture->SetColor(RGBA(255, 255, 255, m_iAlpha));
		m_pTexture->RenderTexture(&m_rtDestFade, &m_rtSrcFade);
		m_pTexture->SetColor(RGBA(255, 255, 255, 255));		
	}
}

void SPZone::RenderText()
{	
}


int SPZone::SPPerformMessage	(UINT msg, WPARAM wparam, LPARAM lParam)
{
	if(SPGM_SUG_ZONE_BEGIN <= msg && SPGM_SUG_ZONE_END > msg ) {
		return SPGMESSAGE_FUNC((SPGM)msg, wparam, lParam);
	} else if(SPGM_GOBMESSAGE_BEGIN <= msg && SPGM_GOBMESSAGE_END > msg ) {
			return SPGMESSAGE_FUNC((SPGM)msg, wparam, lParam);
	} 
	else if( SPGM_CHAR_SC_SELECT == msg )
		return SPGMESSAGE_FUNC((SPGM)msg, wparam, lParam);

	return SPSubGameManager::SPPerformMessage(msg, wparam, lParam);
}

SPGMESSAGE_MAP_BEGIN(SPZone)	
	SPGMESSAGE_COMMAND( SPGM_GOB_ENTER				, OnGOBEnter	)
	SPGMESSAGE_COMMAND( SPGM_GOB_ENTER_ID			, OnGOBEnterID	)
	SPGMESSAGE_COMMAND( SPGM_GOB_ENTER_EX			, OnGOBEnterEx	)
	SPGMESSAGE_COMMAND( SPGM_GOB_LEAVE				, OnGOBLeave	)
	SPGMESSAGE_COMMAND( SPGM_GOB_MOVE				, OnGOBMove	)
	SPGMESSAGE_COMMAND( SPGM_MESSAGE_STRING			, OnMessageString		)
	SPGMESSAGE_COMMAND( SPGM_EXIT_ZONE				, OnExitZone			)

	SPGMESSAGE_COMMAND( SPGM_EXIT_STAGE_CHANGE		, OnExitStageChange		)

	SPGMESSAGE_COMMAND( SPGM_EQUIP_DEFAULT_CHANGE	, OnPlayerEquipDefaultChange	)
	SPGMESSAGE_COMMAND( SPGM_EQUIP_CHANGE			, OnPlayerEquipChange			)	

	SPGMESSAGE_COMMAND( SPGM_VERSION_SC_CHECK		, OnVersionChecked	)
	SPGMESSAGE_COMMAND( SPGM_WORLD_SC_LEAVE			, OnLeaveWorld		)

	SPGMESSAGE_COMMAND( SPGM_CHAR_SC_SELECT			, OnCharacterSelect		)
	SPGMESSAGE_COMMAND( SPGM_CHANNEL_CHANGE			, OnChannelChange		)
	SPGMESSAGE_COMMAND( SPGM_CHANNEL_CHANGE_WAIT	, OnChannelChangeWait	)
	SPGMESSAGE_COMMAND( SPGM_CHANNEL_CHANGE_READY	, OnChannelChangeReady	)
	SPGMESSAGE_COMMAND( SPGM_GET_CHANNEL_CHANGE_TYPE , OnGetChannelChangeType	)

	SPGMESSAGE_COMMAND( SPGM_CASHSHOP_IN_REQUEST	, OnCashShopInRequest	)
	SPGMESSAGE_COMMAND( SPGM_CASHSHOP_IN_RECEIVE	, OnCashShopInReceive	)
	SPGMESSAGE_COMMAND( SPGM_CASHSHOP_INFO			, OnCashShopInfo		)
	SPGMESSAGE_COMMAND( SPGM_CASHSHOP_UPDATE		, OnCashShopUpdate		)
	SPGMESSAGE_COMMAND( SPGM_CASHSHOP_BUY			, OnCashShopBuy			)
	SPGMESSAGE_COMMAND( SPGM_CASHSHOP_BUY_GIFT		, OnCashShopGift		)
	SPGMESSAGE_COMMAND( SPGM_CASHSHOP_BUY_EXACT		, OnCashShopExact		)
	SPGMESSAGE_COMMAND( SPGM_CASHSHOP_SELL			, OnCashShopSell		)
	SPGMESSAGE_COMMAND( SPGM_CASHSHOP_COUPON		, OnCashShopCoupon		)
	SPGMESSAGE_COMMAND( SPGM_CASHSHOP_CART_LIST		, OnCashShopCartList	)
	SPGMESSAGE_COMMAND( SPGM_CASHSHOP_CART_ADD		, OnCashShopCartAdd		)
	SPGMESSAGE_COMMAND( SPGM_CASHSHOP_CART_DEL		, OnCashShopCartDel		)
	SPGMESSAGE_COMMAND( SPGM_CASHSHOP_ITEM_INIT		, OnCashShopItemInit	)
	SPGMESSAGE_COMMAND( SPGM_CASHSHOP_ITEM_UPDATE	, OnCashShopItemUpdate	)
	SPGMESSAGE_COMMAND( SPGM_CASHSHOP_OUT_READY		, OnCashShopOutReady	)
	SPGMESSAGE_COMMAND( SPGM_CASHSHOP_QB_INFO		, OnQuickBuyInfoAck		)

	SPGMESSAGE_COMMAND( SPGM_LOTTREY_IN_REQUEST		, OnLotteryInRequest	)
	SPGMESSAGE_COMMAND( SPGM_LOTTREY_IN_RECEIVE		, OnLotteryInReceive	)
	SPGMESSAGE_COMMAND( SPGM_LOTTREY_STATUS			, OnLotteryStatus		)
	SPGMESSAGE_COMMAND( SPGM_LOTTREY_UPDATE			, OnLotteryUpdate		)
	SPGMESSAGE_COMMAND( SPGM_LOTTREY_BUY			, OnLotteryBuy			)
	SPGMESSAGE_COMMAND( SPGM_LOTTREY_UNBUY			, OnLotteryUnBuy		)
	SPGMESSAGE_COMMAND( SPGM_LOTTREY_ACCEPT			, OnLotteryAcceptItem	)
	SPGMESSAGE_COMMAND( SPGM_LOTTREY_ACCEPT_SECOND	, OnLotteryAcceptItemSecond	)
	SPGMESSAGE_COMMAND( SPGM_LOTTREY_INITIAL		, OnLotteryInitial		)

	SPGMESSAGE_COMMAND( SPGM_SET_STORY_VIEW			, OnSetStoryView	)
	SPGMESSAGE_COMMAND( SPGM_SET_BOOK_VIEW			, OnSetTextView		)
	
	SPGMESSAGE_COMMAND( SPGM_GUILD_INDUN_ENTER		, OnGuildIndunEnter		)
	SPGMESSAGE_COMMAND( SPGM_INDUN_IN_REQUEST		, IndunInRequest		)

	SPGMESSAGE_COMMAND( SPGM_PVP_INDUN_ENTER		, OnSetPVPIndunIn		)
	SPGMESSAGE_COMMAND( SPGM_PVP_LOADWAIT_SHOW		, OnShowPVPLoadWait		)

	SPGMESSAGE_COMMAND( SPGM_INDUN_IN				, OnIndunIn)
	SPGMESSAGE_COMMAND( SPGM_INDUN_OUT				, OnIndunOut)
	SPGMESSAGE_COMMAND( SPGM_INDUN_FORCE_EXIT		, OnIndunForceExit)
	SPGMESSAGE_COMMAND( SPGM_INDUN_TEAM_EXIT		, OnIndunTeamExit)
	SPGMESSAGE_COMMAND( SPGM_INDUN_RESET			, OnIndunReset)

	SPGMESSAGE_COMMAND( SPGM_WORLD_SC_ENTER			, OnWorldEnter)

	SPGMESSAGE_COMMAND( SPGM_SUG_SOCKET_FAILED		, OnSocketFailed	)

	SPGMESSAGE_COMMAND( SPGM_CASHSHOP_PARCEL		, OnCashShopParcel		)
	SPGMESSAGE_COMMAND( SPGM_CASHSHOP_BEEXACT		, OnCashShopBeExact		)

SPGMESSAGE_MAP_END(SPSubGameManager)



int SPZone::OnLeaveWorld(WPARAM wparam, LPARAM lparam)
{
	if( wparam )
		m_fWaitTime = 1.0f;
	else
		m_fWaitTime = 3.0f;

	m_iGameState = GS_RELEASE_READY;
	
	g_bWarningBugleChannel = false;	// Ω¯»Î¿Æ∞»∆¡µ¿
	g_bWarningBugleCoinBegin.clear();	
	g_bWarningBugleLastOne = false;	// ∑¢ÕÍ◊Ó∫Û“ª∏ˆ¿Æ∞»
	g_bPushPropInited = false;
	
	return 1;
}

int SPZone::OnExitZone(WPARAM wparam, LPARAM lparam)
{
	switch(wparam) 
	{
	case 0:
		// Exit ServerChange
		g_pNetworkManager->GameChannel_OUT();
		g_pkIndunManager->ClearIndunInfo() ;		// if Exit Clear Indun Info
		g_pkPvpManager->ClearPvpInfo() ;			// if Exit Clear PVP Info
		m_NextSubGameID	= SUBGAME_LOBBY;
		g_pNetworkManager->SetServerID(g_pNetworkManager->GetLoginServerID());
		g_pNetworkManager->UserMoveRequest();
		break;
	case 1:
		// Exit Logout
		g_pNetworkManager->GameChannel_OUT();
		g_pkIndunManager->ClearIndunInfo() ;		// if Exit Clear Indun Info
		g_pkPvpManager->ClearPvpInfo() ;			// if Exit Clear PVP Info
		m_NextSubGameID	= SUBGAME_LOGGIN;
		//g_pNetworkManager->Disconnect2();
		OnLeaveWorld(1,0);
		break;
	case 2:
		// change channel
		m_NextSubGameID	= SUBGAME_ZONE;
		break;
	default:
		return 1;
	}

	//g_pNetworkManager->LeaveWorld();
	return 1;
}


int	SPZone::OnExitStageChange(WPARAM wparam, LPARAM lparam)
{
	m_fWaitTime = 0.0f;
	//m_iGameState = GS_STAGE_CHANGE;
	//m_NextSubGameID	= SUBGAME_LOADING;

	//m_iGameState = GS_FADE_IN;
	SetFadeState(GS_FADE_IN, GS_STAGE_CHANGE) ;
	
	g_pCameraManagr->SetCameraMode(SPCameraManager::FOCUS_FREE_CAMERA);	//[2005/11/25]

	return 1;
}


int SPZone::OnGOBEnter(WPARAM wparam, LPARAM lparam)
{
	return g_pGOBManager->SPPerformMessage(SPGM_GOB_ENTER, wparam, lparam );	
}

int SPZone::OnGOBEnterID(WPARAM wparam, LPARAM lparam)
{
	return g_pGOBManager->SPPerformMessage(SPGM_GOB_ENTER_ID, wparam, lparam );	
}

int SPZone::OnGOBEnterEx(WPARAM wparam, LPARAM lparam)
{
	return g_pGOBManager->SPPerformMessage(SPGM_GOB_ENTER_EX, wparam, lparam );	
}

int SPZone::OnGOBLeave(WPARAM wparam, LPARAM lparam)
{
	return g_pGOBManager->SPPerformMessage(SPGM_GOB_LEAVE, wparam, lparam );	
}

int SPZone::OnGOBMove(WPARAM wparam, LPARAM lparam)
{
	return g_pGOBManager->SPPerformMessage(SPGM_GOB_MOVE, wparam, lparam );	
}

int SPZone::OnMessageString(WPARAM wparam, LPARAM lparam)
{	
	//SPGM_MESSAGE_STRING¿Ã æ∆¥œ¥Ÿ.
	return g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , wparam, lparam);	
}

int SPZone::OnPlayerEquipDefaultChange(WPARAM wparam, LPARAM lparam)
{
	return g_pGOBManager->SPPerformMessage(SPGM_EQUIP_DEFAULT_CHANGE, wparam, lparam );	
}

int SPZone::OnPlayerEquipChange(WPARAM wparam, LPARAM lparam)
{
	return g_pGOBManager->SPPerformMessage(SPGM_EQUIP_CHANGE, wparam, lparam );	
}

int SPZone::OnVersionChecked(WPARAM wparam, LPARAM lparam)
{
	if(g_pNetworkManager->IsVersionCheckPassed()) 
	{				
		m_strLog = "Version Checking : PASSED\nEnter your ID and PW.";
	}
	else
	{
		m_strLog = "Version Checking : FAILED.\nPlease patch your client program.";
	}
	return 0;
}

int SPZone::OnChannelChange(WPARAM wparam, LPARAM lparam)
{
	if( g_pGOBManager->GetLocalPlayer() )
	{
		UINT64 uiAction = g_pGOBManager->GetLocalPlayer()->GetCurAction();
		if( uiAction != 0 )
		{
			D3DXCOLOR color(1.0f, 0.0f, 0.0f, 1.0f);
			g_pInterfaceManager->SPPerformMessage(SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(4000004), (LPARAM)&color);
			return 0;
		}

		//[2007/5/15]-≥≤¿« ∞≥¿ŒªÛ¡°ø° µÈæÓ∞£ ∞ÊøÏ ∞≠¡¶∑Œ ≥™ø¿∞‘ «‘
		if(g_pGOBManager->GetLocalPlayer()->GetUserShopID()) {
			g_pGOBManager->GetLocalPlayer()->SetUserShopOut();
		}
	}

	CHANNEL_INFO* pChannelInfo = (CHANNEL_INFO*)wparam;
	if( pChannelInfo == NULL )
		return 0;

	// √§≥Œ ∫Ø∞Ê«“ ∏Ò¿˚¡ÅE√§≥Œ ¡§∫∏ ¿ÅE?
	CPacket packet;
	packet.AddUINT32(GAME_CS_CHANGE_CHANNEL);
	packet.AddUINT16(g_uiWorldID);
	packet.AddUINT16(pChannelInfo->channelID);

	SENDPACKET(packet);
	m_iChangeServerID = pChannelInfo->channelID;
	g_pInterfaceManager->SPChildWndSendMessage(WIID_EXIT, SPIM_BTN_ENABLE, 0, 0);
	g_pInterfaceManager->ForceReply();
	
	if( g_pGOBManager->GetLocalPlayer() )
		g_pGOBManager->GetLocalPlayer()->TriggerEnable(false);

	g_pNetworkManager->SetIndunID(0);
	m_iChannelChangeType = 1;			// Channel Change
	return 0;
}

int SPZone::OnChannelChangeWait(WPARAM wParam, LPARAM lParam)
{
	m_bChannelWait = (bool)wParam;
	m_fChannelWaitTime = 0.0f;
	return 0;
}

int SPZone::OnChannelChangeReady(WPARAM wparam, LPARAM lparam)
{
	if( m_iChannelChangeType == 1 )
		g_pNetworkManager->SetGameServerID(m_iChangeServerID);

	g_pNetworkManager->FDS_SetChat(false) ;
	g_pNetworkManager->SetServerID(m_iChangeServerID);
	g_pNetworkManager->UserMoveRequest();
	////ÕÊº“—°‘Ò”Œœ∑Ω«…´Ω¯»Î”Œœ∑,…Ë÷√Ω«…´µƒ…Á»∫–≈œ¢
	//if ( g_pCIGWLoaderDx9 )
	//{
	//	if(g_pGOBManager->GetLocalPlayer()) 
	//	{
	//		SPPlayerStatus* pStatus = static_cast<SPPlayerStatus*>( g_pGOBManager->GetLocalPlayer()->GetStatus() );
	//		std::string strName = pStatus->GetGOBName();
	//		int iSex = static_cast<int>( pStatus->GetStatusValue( STATUS_GENDER ) );

	//		int     GameAreaID = -1;                //  ¢¥ÛÕ≥“ª”Œœ∑«¯∫≈ID£®ƒ¨»œ-1£©      // 2008.3.5 ‘ˆº””Œœ∑«¯∑˛ ˝◊÷ID–≈œ¢
	//		int     GameServerID = -1; 

	//		if ( g_pNetworkManager )
	//		{
	//			GameAreaID = g_pNetworkManager->GetWorldID();
	//			GameServerID = g_pNetworkManager->GetServerID();
	//		}

	//		//ÕÊº“—°‘Ò∑˛ŒÒ∆˜≤¢Ω¯»Î,∏ƒ±‰”√ªß–≈œ¢,ƒø«∞÷ª”–∑˛ŒÒ∆˜√˚ø…”√
	//		UserDetailInfoA UserInfo = { sizeof( UserDetailInfoA ),
	//			strName.c_str(),
	//			iSex,
	//			NULL,
	//			NULL,
	//			GameAreaID,
	//			GameServerID
	//		};
	//		g_pCIGWLoaderDx9->ChangeUserInfoA( &UserInfo );
	//	}
	//}
	return 0;
}

int	SPZone::OnGetChannelChangeType(WPARAM wparam, LPARAM lparam)
{
	int* pType = (int*)wparam;
	*pType = m_iChannelChangeType;
	return 0;
}

int SPZone::OnCharacterSelect(WPARAM wParam, LPARAM lParam)
{
	RESULTCODE result = (RESULTCODE)wParam;
	if( result == LT_NO_ERROR )
	{
		g_pGOBManager->SPPerformMessage(SPGM_CHAR_SC_SELECT, wParam, lParam );
		g_pInterfaceManager->SPChildWndSendMessage(WIID_EXIT, SPIM_BTN_ENABLE, 1, 0);
		if( g_pGOBManager->GetLocalPlayer() )
			g_pGOBManager->GetLocalPlayer()->TriggerEnable(true);

		if( m_iChannelChangeType == 1 )				// Channel Change
		{
			CPacket Packet;
			Packet.Add((UINT32)NAV_CS_ENTERSTAGE);	
			g_pNetworkManager->SPPerformMessage(NAV_CS_ENTERSTAGE, 0, (LPARAM)&Packet);
			ErrorFLogs->Error_Log("SPZone::ChannelChange => OnChanracterSelect") ;
		}
		else if( m_iChannelChangeType == 2 )		// IndunEnter
		{
			int iStage, iGroup;
			iStage = g_pGOBManager->GetLocalPlayer()->GetCurStageID();
			iGroup = g_pGOBManager->GetLocalPlayer()->GetCurMapGroupID();

			g_StageManager.SetNextStage(iStage, iGroup);
			g_pSubGameManager->SPPerformMessage(SPGM_EXIT_STAGE_CHANGE, 0);
			ErrorFLogs->Error_Log("OnChanracterSelect -> SPGM_EXIT_STAGE_CHANGE (Fade & StageChange)") ;
		}

	}

	return 0;
}


int SPZone::OnWorldEnter(WPARAM wParam, LPARAM lParam)
{
	CPacket* pkPacket = (CPacket*)lParam;
	RESULTCODE resultCode;
	pkPacket->ExtractUINT8(&resultCode);

	if( resultCode != LT_NO_ERROR )
		SPPerformMessage(SPGM_SUG_SOCKET_FAILED, 0, 0);
	else	{ 
		//// NAV_CS_ENTERSTAGE¥¬ SC_WORLD_ENTER ¿Ã»ƒø° ≥™øÕæﬂ «œ±‚∂ßπÆø° ¿Ã∞˜¿∏∑Œ ¿Ãµø
		//if( m_iChannelChangeType == 1 )				// Channel Change
		//{
		//	CPacket Packet;
		//	Packet.Add((UINT32)NAV_CS_ENTERSTAGE);	
		//	g_pNetworkManager->SPPerformMessage(NAV_CS_ENTERSTAGE, 0, (LPARAM)&Packet);
		//	ErrorFLogs->Error_Log("SPZone::OnWorldEnter -> NAV_CS_ENTERSTAGE") ;
		//}

		//if( SPLocalizeManager::GetInstance()->GetCountryType() == CT_CHINA && !g_pNetworkManager->FDS_IsShowChat() )
		//{
		//	g_pNetworkManager->FDS_QueryState() ;
		//	//g_pNetworkManager->FDS_SetChat(true) ;	-> √‚∑¬»ƒ ∫Ø∞Ê«‘
		//}
	}

	g_bWarningBugleChannel = false;	// Ω¯»Î¿Æ∞»∆¡µ¿
	g_bWarningBugleCoinBegin.clear();
	g_bWarningBugleLastOne = false;	// ∑¢ÕÍ◊Ó∫Û“ª∏ˆ¿Æ∞»

	loadBugleNotifiers();
	char szBuf[64] = {0};
	g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_GETGOBNAME, (LPARAM)szBuf);
	if (isPlayerNotified( szBuf ))
		g_bNeedBugleHighlight = false;
	else
		g_bNeedBugleHighlight = true;

	return 0;
}

// CashShop
int SPZone::OnCashShopInRequest(WPARAM wParam, LPARAM lParam)
{
	if( m_bCashShopInReq )
		return 0;

	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();
	if( pLocalPlayer == NULL )
		return 0;

	if( wParam )	// CashShop Enter
	{
		if( m_iGameState == GS_CASHSHOP || g_pInterfaceManager->IsCutIn() )
			return 0;

		if( g_pGOBManager == NULL )
			return 0;

		

		if( pLocalPlayer->IsDead() )
			return 0;

		if( pLocalPlayer->GetCurAction() == ACTION_ENCHANTTING )
		{
			D3DXCOLOR	cColor	=	D3DXCOLOR((160.0f / 255.0f), (255.0f / 255.0f), (0.0f / 255.0f), 1.0f);
			g_pInterfaceManager->SPChildWndSendMessage( WIID_SYSTEM_MESSAGE , SPIM_SYSTEM_MESSAGE_ADD , (WPARAM)g_pResourceManager->GetGlobalString( 4000002 ) , (LPARAM)&cColor );
			return 0;
		}

		int iMajor, iMinor, iHotVer;
		g_pItemCluster->GetCashVersion(iMajor, iMinor);
		iHotVer = SPCashTradeManager::GetInstance()->GetCashVerInfo().m_iHotListVer;
		CPacket kPacket(CASHSHOP_CS_IN);
		kPacket.AddUINT32(iMajor);
		kPacket.AddUINT32(iMinor);		
		kPacket.AddUINT32(iHotVer);
		(g_pNetworkManager->GetPacketHandler())->PostMsg(&kPacket);
		m_bCashShopInReq = true;
	}
	else		// CashShop Out
	{
		CPacket kPacket(CASHSHOP_CS_OUT);

		//[xialin 2008/07/30] add >>>
		int iItemCount = 0, iItemID = 0;
		iItemCount = pLocalPlayer->GetInvenArchive()->GetInventorySize(INVENTORY_WEAR) - 
						pLocalPlayer->GetInvenArchive()->GetEmptySlotCount(INVENTORY_WEAR);

		kPacket.AddUINT32(iItemCount);
		for (int i = iItemCount-1; i >= 0; i--)
		{
			SPItem * spItem = pLocalPlayer->GetInvenArchive()->GetInventoryItem(INVENTORY_WEAR, i);
			kPacket.AddUINT32(spItem->GetItemStatus()->GetItemID());
		}
		//[xialin 2008/07/30] add <<<

		(g_pNetworkManager->GetPacketHandler())->PostMsg(&kPacket);
		m_bCashShopInReq = true;
	}
	
	return 0;
}

int SPZone::OnCashShopInReceive(WPARAM wParam, LPARAM lParam)
{
	m_bCashShopInReq = false;

	CPacket* pkPacket = reinterpret_cast< CPacket* >(lParam);
	if( pkPacket == NULL )
		return 1;

	GLOBAL_STRING_ID iError;
	pkPacket->ExtractUINT32(&iError);
	if( iError )
	{
		g_pInterfaceManager->SPPerformMessage(
			SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(iError));
		return 1;
	}

	if( g_pCameraManagr )
		g_pCameraManagr->Init();

	SPPlayer* pkPlayer = NULL;
	pkPlayer = g_pGOBManager->GetLocalPlayer();

	if( wParam )
	{
		GU_ID iGUID;
		unsigned int iMinor;
		unsigned int iHotVer;

		pkPacket->ExtractUINT64(&iGUID);
		pkPacket->ExtractUINT32(&iMinor);
		pkPacket->ExtractUINT32(&iHotVer);
		g_pItemCluster->SetCashVersion(-1, iMinor);
		SPCashTradeManager::GetInstance()->SetMinorVer(iMinor);
		SPCashTradeManager::GetInstance()->SetHotVer(iHotVer);

		SITE_TYPE eSiteType = SPLocalizeManager::GetInstance()->GetSiteType();

		// If user enter Cash-shop and SiteType is GAMEPOT then set special billing URL
		if( eSiteType == SITE_GAMEPOT || eSiteType == SITE_ATGAMES ) 
		{
			UINT8 uiTokenLen = 0;
			char szTemp[32];
			pkPacket->ExtractUINT8(&uiTokenLen);
			if( uiTokenLen < 1)	uiTokenLen = 8 ;		// length can't minus (default 8)
			pkPacket->ExtractData(szTemp, uiTokenLen);
			if( g_pInterfaceManager )	
			{
				g_pInterfaceManager->SPChildWndSendMessage(WIID_CASHSHOP, SPIM_CASHSHOP_SET_URLTOKEN, 0, LPARAM(szTemp));
			}
		}
		else // 2008.07.08 dhpark √Ê¿¸¿•∆‰¿Ã¡ˆøÎ ≈∞∞™¿ª º≥¡§«œ¥¬ ∫Œ∫–¿ª 
		{
			UINT64 uiDID = 0;
			pkPacket->ExtractUINT64(&uiDID);

			char szTemp[32];
			_i64toa(uiDID,szTemp,10);

			if( g_pInterfaceManager )	
			{
				g_pInterfaceManager->SPChildWndSendMessage(WIID_CASHSHOP, SPIM_CASHSHOP_SET_URLTOKEN, 0, LPARAM(szTemp));
			}
		}

		//[xialin 2008/07/30] add >>>
		int iItemCount = 0, iItemID = 0; 
		pkPacket->ExtractUINT32((UINT32*)&iItemCount);
		pkPlayer->GetInvenArchive()->DeleteWearItem();
		for (int i=0; i < iItemCount; i++)
		{
			pkPacket->ExtractUINT32((UINT32*)&iItemID);

			SPItemAttr * pkItemAttr = NULL;
			g_pItemCluster->GetItemInfo(iItemID, pkItemAttr);
			if ( !pkItemAttr )
				continue;

			CONTAINER_ITEM	stWearItem;
			memset(&stWearItem, 0x00, sizeof(stWearItem));
			stWearItem.SlotInfo.ContainerType = INVENTORY_WEAR;
			stWearItem.SlotInfo.SlotIndex = 0;
			stWearItem.Item.iBackPrice = pkItemAttr->m_iCashPrice;
			stWearItem.Item.iItemExp = pkItemAttr->m_iPetMaxExp;
			stWearItem.Item.iItemID = iItemID;
			stWearItem.Item.iLifeTime = pkItemAttr->m_iLifeTime;
			stWearItem.Item.iOptPoint = pkItemAttr->m_iCashPoint;
			stWearItem.Item.iRare = pkItemAttr->m_eRareLimit;
			stWearItem.Item.iStackCount = pkItemAttr->m_iStackLimit;
			pkPlayer->GetInvenArchive()->ItemSetting(stWearItem, false);
		}
		//[xialin 2008/07/30] add <<<

		if( g_pInterfaceManager != NULL )
		{
			SPWindow*	pWindowTalkingRoom	=	g_pInterfaceManager->FindWindow( WIID_TALKING_ROOM );

			if( pWindowTalkingRoom != NULL )
			{
				if( pWindowTalkingRoom->IsShow() == true )
					pWindowTalkingRoom->SPSendMessage( SPIM_CLOSE );
			}
		}

		if( g_pInterfaceManager )	
			g_pInterfaceManager->SetOnlyWindowView(WIID_CASHSHOP, true, true);

		m_iGameState = GS_CASHSHOP;
		
		CPacket kPacket(CASHSHOP_CS_INFO);
		(g_pNetworkManager->GetPacketHandler())->PostMsg(&kPacket);

		g_pCheckManager->SetDBBlock(true);
		g_pGOBManager->ResetObject();
		g_pEventManager->OnCutInEnd();

		OnSetBGM((WPARAM)"DATA/BGM/CASHSHOP.mp3", 0);

		if( pkPlayer )
			pkPlayer->TriggerEnable(false);
	}
	else
	{
		if( g_pInterfaceManager )
			g_pInterfaceManager->SetOnlyWindowView(WIID_CASHSHOP, false, false);

		m_iGameState = GS_GAME;
		if(SPCashTradeManager::GetInstance()->IsSocketFail()) {
			OnSocketFailed((WPARAM)0, (LPARAM)0);
		}
		
		if( pkPlayer )
		{
			int iStage = pkPlayer->GetCurStageID();
			int iGroup = pkPlayer->GetCurMapGroupID();		
			std::string strBGM = g_StageManager.GetCurStage(iStage)->GetMapGroup(iGroup)->m_strBGMFile;
			int iSoundEffect = g_StageManager.GetCurStage(iStage)->GetMapGroup(iGroup)->m_iSoundEffectType;

			SPPerformMessage(SPGM_SUG_SETBGM , (WPARAM)strBGM.c_str());
			SPSoundBase::GetInstance()->SetRoomType(iSoundEffect);

			if( pkPlayer->IsDead() )
				pkPlayer->SetDeadAction();

			pkPlayer->TriggerEnable(true);
		}

		//////////////////////////////////////////////////////////////////////////
		// if user is in indun then indun exit
		if( g_pkIndunManager->GetIndunForceExit() )
		{
			IndunOutRequest() ;
			g_pkIndunManager->SetIndunForceExit(false) ;		// Flag off
		}
	}

	return 0;
}

int SPZone::OnCashShopInfo(WPARAM wParam, LPARAM lParam)
{
	if( g_pInterfaceManager )
		g_pInterfaceManager->SPChildWndSendMessage(WIID_CASHSHOP, SPIM_CASHSHOP_INFO, wParam, lParam);

	return 0;
}

int SPZone::OnCashShopUpdate(WPARAM wParam, LPARAM lParam)
{
	if( g_pInterfaceManager )
		g_pInterfaceManager->SPChildWndSendMessage(WIID_CASHSHOP, SPIM_CASHSHOP_UPDATE, wParam, lParam);

	return 0;
}

int SPZone::OnCashShopBuy(WPARAM wParam, LPARAM lParam)
{
	if( g_pInterfaceManager )
		g_pInterfaceManager->SPChildWndSendMessage(WIID_CASHSHOP, SPIM_CASHSHOP_BUY, wParam, lParam);

	return 0;
}

int SPZone::OnCashShopGift(WPARAM wParam, LPARAM lParam)
{
	if( g_pInterfaceManager )
	{
		//[xialin 2008/07/31] add
		SPWindowBeExact * pkWindow = (SPWindowBeExact *)g_pInterfaceManager->FindWindow(WIID_BEEXACT);
		if ( pkWindow && pkWindow->IsGifting() )
		{
			g_pInterfaceManager->SPChildWndSendMessage(WIID_BEEXACT, SPIM_BEEXACT_GIFT, wParam, lParam);
		}
		else
		{
			g_pInterfaceManager->SPChildWndSendMessage(WIID_CASHSHOP, SPIM_CASHSHOP_GIFT, wParam, lParam);
		}
	}

	return 0;
}

int SPZone::OnCashShopExact(WPARAM wParam, LPARAM lParam)
{
	if( g_pInterfaceManager )
	{
		g_pInterfaceManager->SPChildWndSendMessage(WIID_CASHSHOP, SPIM_CASHSHOP_EXACT, wParam, lParam);
	}

	return 0;
}

int SPZone::OnCashShopParcel(WPARAM wParam, LPARAM lParam)
{
	if( g_pInterfaceManager )
	{
		g_pInterfaceManager->SPChildWndSendMessage(WIID_CASHSHOP, SPIM_CASHSHOP_PARCEL, wParam, lParam);
	}

	return 0;
}

int SPZone::OnCashShopBeExact(WPARAM wParam, LPARAM lParam)
{
	if( g_pInterfaceManager )
	{
		g_pInterfaceManager->SPChildWndSendMessage(WIID_BEEXACT, SPIM_BEEXACT_MESSAGE, wParam, lParam);
	}

	return 0;
}

int SPZone::OnCashShopSell(WPARAM wParam, LPARAM lParam)
{
	if( g_pInterfaceManager )
		g_pInterfaceManager->SPChildWndSendMessage(WIID_CASHSHOP, SPIM_CASHSHOP_SELL, wParam, lParam);

	return 0;
}

int SPZone::OnCashShopCoupon(WPARAM wParam, LPARAM lParam)
{
	if( g_pInterfaceManager )
		g_pInterfaceManager->SPChildWndSendMessage(WIID_CASHSHOP, SPIM_CASHSHOP_COUPON, wParam, lParam);

	return 0;
}

int	SPZone::OnCashShopCartList(WPARAM wParam, LPARAM lParam)
{
	if( g_pInterfaceManager )
		g_pInterfaceManager->SPChildWndSendMessage(WIID_CASHSHOP, SPIM_CASHSHOP_CARTLIST, wParam, lParam);
	
	return 0;
}

int	SPZone::OnCashShopCartAdd(WPARAM wParam, LPARAM lParam)
{
	if( g_pInterfaceManager )
		g_pInterfaceManager->SPChildWndSendMessage(WIID_CASHSHOP, SPIM_CASHSHOP_CARTADD, wParam, lParam);
	
	return 0;
}

int	SPZone::OnCashShopCartDel(WPARAM wParam, LPARAM lParam)
{
	if( g_pInterfaceManager )
		g_pInterfaceManager->SPChildWndSendMessage(WIID_CASHSHOP, SPIM_CASHSHOP_CARTDEL, wParam, lParam);

	return 0;
}

int	SPZone::OnCashShopItemInit(WPARAM wparam, LPARAM lparam)
{
	CPacket* pkPacket = reinterpret_cast< CPacket* >(lparam);
	if( pkPacket == NULL )
		return 1;

	GLOBAL_STRING_ID iError;
	pkPacket->ExtractUINT32(&iError);
	if( iError )
	{
		g_pInterfaceManager->SPPerformMessage(
			SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(iError));
		return 1;
	}

	g_pItemCluster->InitMinorCashItem();
	return 0;
}

int	SPZone::OnCashShopItemUpdate(WPARAM wparam, LPARAM lparam)
{
	CPacket* pkPacket = reinterpret_cast< CPacket* >(lparam);
	if( pkPacket == NULL )
		return 1;

	GLOBAL_STRING_ID iError;
	pkPacket->ExtractUINT32(&iError);
	if( iError )
	{
		g_pInterfaceManager->SPPerformMessage(
			SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(iError));
		return 1;
	}

	GU_ID iGUID;
	unsigned short usNum, i;
	CASHSHOP_INFO tItemInfo;

	pkPacket->ExtractUINT64(&iGUID);
	pkPacket->ExtractUINT16(&usNum);
	for( i = 0; i < usNum; i++ )
	{
		pkPacket->Extract(&tItemInfo, sizeof(CASHSHOP_INFO));
		g_pItemCluster->UpdateMinorCashItem(&tItemInfo);
	}

	g_pItemCluster->UpdateItemCashPrize();

	return 0;
}

int	SPZone::OnCashShopOutReady(WPARAM wparam, LPARAM lparam)
{
	CPacket* pkPacket = reinterpret_cast< CPacket* >(lparam);
	if( pkPacket == NULL )
		return 1;

	GLOBAL_STRING_ID iError;
	pkPacket->ExtractUINT32(&iError);
	if( iError == 0 )
	{
		OnCashShopInRequest(0, 0);
		return 1;
	}

	if( g_pInterfaceManager )
		g_pInterfaceManager->SPChildWndSendMessage(WIID_CASHSHOP, SPIM_CASHSHOP_NEED_ITEM_UPDATE, 0, iError);
	
	return 1;
}

int SPZone::OnQuickBuyInfoAck(WPARAM wparam, LPARAM lparam)
{
	CPacket* pkPacket = reinterpret_cast< CPacket* >(lparam);
	if( pkPacket == NULL )
		return 1;

	GLOBAL_STRING_ID iError;
	pkPacket->ExtractUINT32(&iError);
	if( iError == 0 )
	{
		SPWindowPushProp *wnd = (SPWindowPushProp*)g_pInterfaceManager->FindWindow( WIID_PUSH_PROP );
		if (wnd)
			wnd->OnQBInfoAck( pkPacket );
		return 1;
	}

	return 0;
}

int	SPZone::OnLotteryInRequest(WPARAM wParam, LPARAM lParam)
{
	if( m_bLotteryInReq )
		return 0;

	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();
	if( pLocalPlayer == NULL )
		return 0;

	if( wParam )	// Lottery Enter
	{
		if( m_iGameState == GS_LOTTERY || g_pInterfaceManager->IsCutIn() )
			return 0;

		if( pLocalPlayer->IsDead() )
			return 0;

		if( pLocalPlayer->GetCurAction() == ACTION_ENCHANTTING )
		{
			D3DXCOLOR	cColor	=	D3DXCOLOR((160.0f / 255.0f), (255.0f / 255.0f), (0.0f / 255.0f), 1.0f);
			g_pInterfaceManager->SPChildWndSendMessage( WIID_SYSTEM_MESSAGE , SPIM_SYSTEM_MESSAGE_ADD , (WPARAM)g_pResourceManager->GetGlobalString( 4000002 ) , (LPARAM)&cColor );
			return 0;
		}

		CPacket kPacket(GAMBLE_CS_ENTER_GAMBLE);
		(g_pNetworkManager->GetPacketHandler())->PostMsg(&kPacket);
		m_bLotteryInReq = true;

		//CPacket Packet;
		//Packet.AddUINT32(0);
		//OnLotteryInReceive(1, (LPARAM)&Packet);
	}
	else		// Lottery Out
	{
		CPacket kPacket(GAMBLE_CS_LEAVER_GAMBLE);
		(g_pNetworkManager->GetPacketHandler())->PostMsg(&kPacket);
		m_bLotteryInReq = true;

		//CPacket Packet;
		//Packet.AddUINT32(0);
		//OnLotteryInReceive(0, (LPARAM)&Packet);
	}
	
	return 1;
}

int	SPZone::OnLotteryInReceive(WPARAM wParam, LPARAM lParam)
{
	m_bLotteryInReq = false;

	CPacket* pkPacket = reinterpret_cast< CPacket* >(lParam);
	if( pkPacket == NULL )
		return 1;

	GLOBAL_STRING_ID iError;
	pkPacket->ExtractUINT32(&iError);
	if( iError )
	{
		g_pInterfaceManager->SPPerformMessage(
			SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(iError));
		return 1;
	}

	if( g_pCameraManagr )
		g_pCameraManagr->Init();

	SPPlayer* pkPlayer = NULL;
	pkPlayer = g_pGOBManager->GetLocalPlayer();

	if( wParam )
	{
		m_iGameState = GS_LOTTERY;

		if( g_pInterfaceManager )
		{
			g_pInterfaceManager->SetOnlyWindowView(WIID_LOTTERY, true, true);
			g_pInterfaceManager->SPChildWndSendMessage(WIID_LOTTERY, SPIM_PURGE , NULL , NULL );
			g_pInterfaceManager->SPChildWndSendMessage(WIID_LOTTERY, SPIM_LOTTERY_REFRESH, NULL, NULL);
			g_pInterfaceManager->SPChildWndSendMessage(WIID_LOTTERY, SPIM_LOTTERY_LOAD_TIP , NULL , NULL );
		}
		
		g_pGOBManager->ResetObject();
		g_pEventManager->OnCutInEnd();

		OnSetBGM((WPARAM)"DATA\BGM\55_00.mp3", 0);

		if( pkPlayer )
			pkPlayer->TriggerEnable(false);
	}
	else
	{
		m_iGameState = GS_GAME;

		if( g_pInterfaceManager )
		{
			g_pInterfaceManager->SetOnlyWindowView(WIID_LOTTERY, false, false);
			g_pInterfaceManager->SPChildWndSendMessage(WIID_LOTTERY, SPIM_LOTTERY_SAVE_TIP , NULL , NULL );
		}

		if( pkPlayer )
		{
			int iStage = pkPlayer->GetCurStageID();
			int iGroup = pkPlayer->GetCurMapGroupID();		
			std::string strBGM = g_StageManager.GetCurStage(iStage)->GetMapGroup(iGroup)->m_strBGMFile;
			int iSoundEffect = g_StageManager.GetCurStage(iStage)->GetMapGroup(iGroup)->m_iSoundEffectType;

			SPPerformMessage(SPGM_SUG_SETBGM , (WPARAM)strBGM.c_str());
			SPSoundBase::GetInstance()->SetRoomType(iSoundEffect);

			if( pkPlayer->IsDead() )
				pkPlayer->SetDeadAction();

			pkPlayer->TriggerEnable(true);
		}
	}
	return 1;
}

int	SPZone::OnLotteryStatus(WPARAM wParam, LPARAM lParam)
{
	g_pCheckManager->SetDBBlock(false);

	if( g_pInterfaceManager )
		g_pInterfaceManager->SPChildWndSendMessage(WIID_LOTTERY, SPIM_LOTTERY_STATUS, wParam, lParam);

	return 0;
}

int	SPZone::OnLotteryUpdate(WPARAM wParam, LPARAM lParam)
{
	if( g_pInterfaceManager )
		g_pInterfaceManager->SPChildWndSendMessage(WIID_LOTTERY, SPIM_LOTTERY_UPDATE_ITEM, wParam, lParam);

	return 0;
}

int	SPZone::OnLotteryBuy(WPARAM wParam, LPARAM lParam)
{
	if( g_pInterfaceManager )
		g_pInterfaceManager->SPChildWndSendMessage(WIID_LOTTERY, SPIM_LOTTERY_BUY, wParam, lParam);

	return 0;
}

int	SPZone::OnLotteryUnBuy(WPARAM wParam, LPARAM lParam)
{
	if( g_pInterfaceManager )
		g_pInterfaceManager->SPChildWndSendMessage(WIID_LOTTERY, SPIM_LOTTERY_UNBUY, wParam, lParam);

	return 0;
}

int	SPZone::OnLotteryAcceptItem(WPARAM wParam, LPARAM lParam)
{
	if( g_pInterfaceManager )
		g_pInterfaceManager->SPChildWndSendMessage(WIID_LOTTERY, SPIM_LOTTERY_ACCEPT, wParam, lParam);

	return 0;
}

int	SPZone::OnLotteryAcceptItemSecond(WPARAM wParam, LPARAM lParam)
{
	if( g_pInterfaceManager )
		g_pInterfaceManager->SPChildWndSendMessage(WIID_LOTTERY, SPIM_LOTTERY_ACCEPTSECOND, wParam, lParam);

	return 0;
}

int	SPZone::OnLotteryInitial(WPARAM wParam, LPARAM lParam)
{
	if( g_pInterfaceManager )
		g_pInterfaceManager->SPChildWndSendMessage(WIID_LOTTERY, SPIM_LOTTERY_INITIAL, wParam, lParam);

	return 0;
}



/**
	Picture Story View
	@param WPARAM wparam	1:Set, 0:UnSet, -1:Quick Exit
*/
int SPZone::OnSetStoryView(WPARAM wparam, LPARAM lparam)
{
	bool bIn = false;
	bool bHideChat = false ;
	int iFlag = (int)wparam ;		// 1:Set, 0:UnSet, -1:Quick Exit

	if( g_pCameraManagr )
		g_pCameraManagr->Init();

	if( iFlag > 0 )		// Set
	{
		bIn = true;
		if( g_pInterfaceManager )
			g_pInterfaceManager->SPChildWndSendMessage(WIID_STORY, SPIM_STORY_SET_FACTOR, 0, lparam);

		//m_iGameState = GS_STORYVIEW;
		SetFadeState(GS_FADE_IN, GS_STORYVIEW) ;
		g_pkStoryManger->SetStoryViewID(0) ;
	}
	else					// UnSet
	{
		if( iFlag == -1 ) {
			m_iGameState = GS_GAME;			// Quick Exit
			if(SPCashTradeManager::GetInstance()->IsSocketFail()) {
				OnSocketFailed((WPARAM)0, (LPARAM)0);
			}
		}
		else
			SetFadeState(GS_FADE_OUT, GS_GAME) ;

		SPPlayer* pkPlayer = NULL;
		pkPlayer = g_pGOBManager->GetLocalPlayer();
		if( pkPlayer )
		{
			int iStage = pkPlayer->GetCurStageID();
			int iGroup = pkPlayer->GetCurMapGroupID();		
			std::string strBGM = g_StageManager.GetCurStage(iStage)->GetMapGroup(iGroup)->m_strBGMFile;
			int iSoundEffect = g_StageManager.GetCurStage(iStage)->GetMapGroup(iGroup)->m_iSoundEffectType;

			SPPerformMessage(SPGM_SUG_SETBGM , (WPARAM)strBGM.c_str());
			SPSoundBase::GetInstance()->SetRoomType(iSoundEffect);

			if( pkPlayer->IsDead() )
				pkPlayer->SetDeadAction();
		}
	}

	bHideChat = !bIn ;
	if( g_pInterfaceManager )
		g_pInterfaceManager->SetOnlyWindowView(WIID_STORY, bIn, bHideChat);

	return 1;
}

/**
	TextBook View
*/
int SPZone::OnSetTextView(WPARAM wparam, LPARAM lparam)
{
	bool bIn = false;
	int	iFlag = (int)wparam ;

	if( g_pCameraManagr )
		g_pCameraManagr->Init();

	if( iFlag > 0 )
	{
		bIn = true;
		if( g_pInterfaceManager )
			g_pInterfaceManager->SPChildWndSendMessage(WIID_TEXTBOOK, SPIM_STORY_SET_FACTOR, 0, lparam);

		//m_iGameState = GS_BOOKVIEW;
		SetFadeState(GS_FADE_IN, GS_BOOKVIEW) ;
	}
	else
	{
		if( iFlag == -1 ) {
			m_iGameState = GS_GAME;					// Quick Exit
			if(SPCashTradeManager::GetInstance()->IsSocketFail()) {
				OnSocketFailed((WPARAM)0, (LPARAM)0);
			}
		}
		else
			SetFadeState(GS_FADE_OUT, GS_GAME) ;	// Fade Out Exit

		SPPlayer* pkPlayer = NULL;
		pkPlayer = g_pGOBManager->GetLocalPlayer();
		if( pkPlayer )
		{
			int iStage = pkPlayer->GetCurStageID();
			int iGroup = pkPlayer->GetCurMapGroupID();		
			std::string strBGM = g_StageManager.GetCurStage(iStage)->GetMapGroup(iGroup)->m_strBGMFile;
			int iSoundEffect = g_StageManager.GetCurStage(iStage)->GetMapGroup(iGroup)->m_iSoundEffectType;

			SPPerformMessage(SPGM_SUG_SETBGM , (WPARAM)strBGM.c_str());
			SPSoundBase::GetInstance()->SetRoomType(iSoundEffect);

			if( pkPlayer->IsDead() )
				pkPlayer->SetDeadAction();
		}
	}

	bool bHideChat = !bIn ;
	if( g_pInterfaceManager )
		g_pInterfaceManager->SetOnlyWindowView(WIID_TEXTBOOK, bIn, bHideChat);

	return 1;
}


/**
	PVP πÊ ¿‘¿Â
	PVPManager ø°º≠ PVP_SC_IN º¬∆√ »ƒ Change_Channel¿ª ¿ß«ÿº≠ »£√‚
	@param wParam : LDT ID
	@param lParam : ServerID
 */
int SPZone::OnSetPVPIndunIn(WPARAM wparam, LPARAM lparam)
{
	UINT16 wServerID = (UINT16)lparam ;
	UINT32 uIndunID	 = (UINT32)wparam ;
	ErrorFLogs->Error_Log("SEND Change Channel World[%d]Server[%d], IndunID[%d]", g_uiWorldID, wServerID, uIndunID) ;

	m_iChangeServerID = wServerID;
	g_pInterfaceManager->SPChildWndSendMessage(WIID_EXIT, SPIM_BTN_ENABLE, 0, 0);
	SPPlayer*	pLocalPlayer = g_pGOBManager->GetLocalPlayer();
	if( pLocalPlayer )
		pLocalPlayer->TriggerEnable(false);

	// Match Sound Effect --> Sound∞° æ»≥™ø¿∏È Manager¬ ¿∏∑Œ ¿Ãµø«ÿæﬂ «‘
	//SPSoundArchive* pSoundArchive	=	NULL;
	//pLocalPlayer->SPGOBPerformMessage( SPGM_GETSOUNDARCHIVE , (LPARAM)&pSoundArchive );
	if( g_pInterfaceManager != NULL )
	{
		SOUND_TABLE_INFO* pInfo = SPSoundManager::GetInstance()->GetSoundTable( (unsigned int)PVP_MATCHOK_SOUND );
		if( pInfo != NULL )
		{
			std::string strFullName;
			if( !pInfo->strFileName[0].empty() )
			{
				strFullName = "DATA/SOUND/";
				strFullName += pInfo->strFileName[0];
				g_pInterfaceManager->RegisterSoundUnit(strFullName.c_str(), pInfo->fDelayTime[0], pInfo->bLoop[0] ) ;
			//	pSoundArchive->InsertItem( strFullName.c_str() , pInfo->fDelayTime[ 0 ] , pInfo->bLoop[ 0 ] );
			}
		}
	}

	//CPacket kSendPacket;
	//kSendPacket.AddUINT32(GAME_CS_CHANGE_CHANNEL);
	//kSendPacket.AddUINT16(g_uiWorldID);
	//kSendPacket.AddUINT16(wServerID);
	//SENDPACKET(kSendPacket);

	//////////////////////////////////////////////////////////////////////////
	// ø©±‚ø° IndunManagerø°º≠ uiIndunID∏¶ ≈Î«ÿ IndunArchive ª˝º∫ (InstanceID, LDTID ±∏∫–)
	g_pNetworkManager->SetIndunID(uIndunID);
	m_iChannelChangeType = 2;		// Enter Indun

	return 1 ;
}


/**
	PVP Indun Enter
*/
int SPZone::OnShowPVPLoadWait(WPARAM wparam, LPARAM lparam)
{
	bool bIn = false;
	int	iFlag = (int)wparam ;

	ErrorFLogs->Error_Log("OnShowPVPLoadWait [%d]", iFlag) ;

	if( g_pCameraManagr )
		g_pCameraManagr->Init();

	if( iFlag > 0 )
	{
		m_iAlpha = 0 ;
		bIn = true;
		m_iGameState = GS_PVPWAIT ;

		// Setting BGM
		SOUND_TABLE_INFO* pInfo = SPSoundManager::GetInstance()->GetSoundTable( (unsigned int)PVP_LOADWAIT_SOUND );
		if( pInfo != NULL )
		{
			std::string strFullName;
			if( !pInfo->strFileName[0].empty() )
			{
				strFullName = "DATA/SOUND/";
				strFullName += pInfo->strFileName[0];

				SPPerformMessage(SPGM_SUG_SETBGM , (WPARAM)strFullName.c_str());
				SPSoundBase::GetInstance()->SetRoomType(ECHO_GENERIC);
			}
		}
	}
	else
	{
		if( iFlag == -1 )	{
			m_iGameState = GS_GAME;					// Quick Exit
			m_iAlpha = 0 ;
			if(SPCashTradeManager::GetInstance()->IsSocketFail()) {
				OnSocketFailed((WPARAM)0, (LPARAM)0);
			}
		}
		else
			SetFadeState(GS_FADE_OUT, GS_GAME) ;	// Fade Out Exit

		SPPlayer* pkPlayer = NULL;
		pkPlayer = g_pGOBManager->GetLocalPlayer();
		if( pkPlayer )
		{
			int iStage = pkPlayer->GetCurStageID();
			int iGroup = pkPlayer->GetCurMapGroupID();		
			std::string strBGM = g_StageManager.GetCurStage(iStage)->GetMapGroup(iGroup)->m_strBGMFile;
			int iSoundEffect = g_StageManager.GetCurStage(iStage)->GetMapGroup(iGroup)->m_iSoundEffectType;

			SPPerformMessage(SPGM_SUG_SETBGM , (WPARAM)strBGM.c_str());
			SPSoundBase::GetInstance()->SetRoomType(iSoundEffect);

			if( pkPlayer->IsDead() )
				pkPlayer->SetDeadAction();
		}
	}

	ErrorFLogs->Error_Log("PVP LoadWait Show[%d] - Flag[%d]", bIn, iFlag) ;

	if( g_pInterfaceManager )
	{
		bool bHideChat = !bIn ;
		g_pInterfaceManager->SetOnlyWindowView(WIID_PVP_LOADWAIT, bIn, bHideChat);
		ErrorFLogs->Error_Log("InterfaceManager LoadWait Show") ;
	}

	return 1;
}


/**
	±ÊµÂπÊ ¿‘¿Â
*/
int	SPZone::OnGuildIndunEnter(WPARAM wparam, LPARAM lparam)
{
	SPGuildArchive* pkGuildArchive = g_pGOBManager->GetGuildArchive();
	if( pkGuildArchive == NULL )	{
		if( g_pResourceManager->GetGlobalString(27000060))		// Not Guild Member
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(27000060) );
		return 1;
	}

	int iIndunID;
	iIndunID = pkGuildArchive->GetMyGuildDungeon();
	if( iIndunID == 0 )	{
		if( g_pResourceManager->GetGlobalString(27000060))		// Not Guild Member
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(27000060) );
		return 1;
	}

	IndunInRequest(iIndunID, 0);
	return 1;
}



// Receive IndunIn
int	SPZone::OnIndunIn(WPARAM wparam, LPARAM lparam)
{
	m_bSendIndunInReq = false ;
	CPacket* pkPacket = reinterpret_cast< CPacket* >(lparam);
	if( pkPacket == NULL )
		return 1;

	GU_ID ObjectGUID;
	GLOBAL_STRING_ID iError;
	UINT32 uiIndunID;
	UINT16 wServerID;
	TCHAR szErrorMsg[256] ;

	pkPacket->ExtractUINT64(&ObjectGUID);
	pkPacket->ExtractUINT32(&iError);
	if( iError != 0 )
	{
		if( g_pResourceManager->GetGlobalString(iError) )
			g_pInterfaceManager->SPPerformMessage(SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(iError));
		else	{
			sprintf(szErrorMsg, "Instance dungeon In Error[%d]", iError) ;
			g_pInterfaceManager->SPPerformMessage(SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szErrorMsg);
		}
		return 1;
	}

	pkPacket->ExtractUINT32(&uiIndunID);
	pkPacket->ExtractUINT16(&wServerID);

	//CPacket kSendPacket;
	//kSendPacket.AddUINT32(GAME_CS_CHANGE_CHANNEL);
	//kSendPacket.AddUINT16(g_uiWorldID);
	//kSendPacket.AddUINT16(wServerID);
	//SENDPACKET(kSendPacket);

	m_iChangeServerID = wServerID;
	g_pInterfaceManager->SPChildWndSendMessage(WIID_EXIT, SPIM_BTN_ENABLE, 0, 0);
	if( g_pGOBManager->GetLocalPlayer() )
		g_pGOBManager->GetLocalPlayer()->TriggerEnable(false);

	//////////////////////////////////////////////////////////////////////////
	// ø©±‚ø° IndunManagerø°º≠ uiIndunID∏¶ ≈Î«ÿ IndunArchive ª˝º∫ (InstanceID, LDTID ±∏∫–)
	g_pkIndunManager->EnterIndun(uiIndunID) ;
	g_pNetworkManager->SetIndunID(uiIndunID);
	m_iChannelChangeType = 2;		// Enter Indun

	return 1;
}


int	SPZone::OnIndunOut(WPARAM wparam, LPARAM lparam)
{
	//CPacket* pkPacket = reinterpret_cast< CPacket* >(lparam);
	//if( pkPacket == NULL )
	//	return 1;

	//GU_ID ObjectGUID;
	//pkPacket->ExtractUINT64(&ObjectGUID);

	IndunOutRequest();
	return 1;
}

/**
	¿Œ¥¯¿‘¿Â
	MapEventø° IndunID∞° ¿÷¿∏∏È ¿Œ¥¯¿‘¿Â
	±ÊµÂπÊ¿« ∞ÊøÏ NPC∏¶ ≈Î«ÿ ¿‘¿Â
*/
int SPZone::IndunInRequest(WPARAM wparam, LPARAM lparam)
{
	SPPlayer* pkPlayer = NULL;
	INDUN_INFO indunInfo ;
	int iIndunID = (int)wparam;
	if( iIndunID < 0 )			return 0 ;

	indunInfo.Clear() ;
	if( g_pkIndunManager->FindIndunInfo(iIndunID, indunInfo) == false )
		return 0 ;
	switch( indunInfo.eType )
	{
	case IDT_PARTY:			// ∆ƒ∆º¿Œ¥¯¿∫ ∆ƒ∆ºø¯∏∏ ∞°¥…
		{
			pkPlayer = g_pGOBManager->GetLocalPlayer();
			if( !pkPlayer )						return 0 ;
			if( !pkPlayer->GetTeamActive() )	{
				if( g_pResourceManager->GetGlobalString(50000010) )
				g_pInterfaceManager->SPPerformMessage(SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(50000010));
				return 0 ;
			}
		}
		break ;
	case IDT_GUILD:		// ±ÊµÂ¿Œ¥¯¿∫ OnGuildIndunEnter() ø°º≠ √º≈©
	case IDT_SINGLE:
	case IDT_EVENT:
		break ;
	}

	if( m_bSendIndunInReq )		return 0 ;
	m_bSendIndunInReq = true ;

	CPacket kPacket(INDUN_CS_IN);
	kPacket.AddUINT32(iIndunID);
	(g_pNetworkManager->GetPacketHandler())->PostMsg(&kPacket);
	g_pkIndunManager->SetIndunLdtID(iIndunID) ;
	return 1;
}


void SPZone::IndunOutRequest()
{
	m_iChangeServerID = g_pNetworkManager->GetGameServerID();
	if( m_iChangeServerID == 0 )
		m_iChangeServerID = g_pNetworkManager->GetPreviousServerID();

	CPacket kSendPacket;
	kSendPacket.AddUINT32(GAME_CS_CHANGE_CHANNEL);
	kSendPacket.AddUINT16(g_uiWorldID);
	kSendPacket.AddUINT16(m_iChangeServerID);
	SENDPACKET(kSendPacket);

	g_pInterfaceManager->SPChildWndSendMessage(WIID_EXIT, SPIM_BTN_ENABLE, 0, 0);
	if( g_pGOBManager->GetLocalPlayer() )
		g_pGOBManager->GetLocalPlayer()->TriggerEnable(false);

	int iIndunID = 0;

	SPGuildArchive* pkGuildArchive = g_pGOBManager->GetGuildArchive();
	if( pkGuildArchive )
		iIndunID = pkGuildArchive->GetMyGuildDungeon();
	//////////////////////////////////////////////////////////////////////////
	// Indun Exit
	g_pkIndunManager->ClearIndunInfo() ;

	g_pNetworkManager->SetIndunID(iIndunID);
	m_iChannelChangeType = 2;		// Enter Indun
}








/**
	±ÊµÂ ≈ª≈¿« ∞ÊøÏ ±ÊµÂ¿Œ¥¯ ∞≠¡¶≥™∞°±‚ º≥¡§ (±ÊµÂ ≈ª≈, ∞≠≈, GM)
	# Check GuildRoom CashShop
*/
int SPZone::OnIndunForceExit(WPARAM wparam, LPARAM lparam)
{
	// ¿Œ¥¯¿Ã æ∆¥œ∏È return 
	SPIndun* pkIndun = g_pkIndunManager->GetCurrentIndun() ;
	if( pkIndun == NULL )
		return 1 ;

	if( pkIndun->GetIndunType() != INDUN_TYPE_GUILD && pkIndun->GetIndunType() != INDUN_TYPE_GUILDINDUN )
		return 1 ;

	// ƒ≥Ω¨º•¿Ã∏È, ∏ﬁΩ√¡ˆ ¥Ÿ¿ÃæÛ∑Œ±◊ & IndunManager ∞≠¡¶ ¡æ∑· º≥¡§
	if( m_iGameState == GS_CASHSHOP )
	{
		g_pkIndunManager->SetIndunForceExit(true) ;
		if( g_pInterfaceManager )
			g_pInterfaceManager->SPChildWndSendMessage(WIID_CASHSHOP, SPIM_CASHSHOP_NEED_ITEM_UPDATE, 0, 27000078);
	}
	else	// ƒ≥Ω¨º•¿Ã æ∆¥— ¿Œ¥¯¿Ã∏È πŸ∑Œ ≥™∞®
	{
		IndunOutRequest() ;
		if( g_pResourceManager->GetGlobalString(50000001) )
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(50000001) );
	}
	return 0 ;
}


/**
	∆¿ ¿Œ¥¯ ∞≠¡¶≥™∞°±‚ º≥¡§ (∆¿ ≈ª≈, ∞≠≈, «ÿ¡¶)
	«ˆ¿Á ¿Œ¥¯ ≈∏¿‘¿ª √º≈©«ÿº≠ ∆¿ ¿Œ¥¯¿Œ∞ÊøÏ ≥™∞°±‚
*/
int SPZone::OnIndunTeamExit(WPARAM wparam, LPARAM lparam)
{
	INDUN_INFO indunInfo ;
	// ¿Œ¥¯¿Ã æ∆¥œ∏È return 
	if( g_pkIndunManager->GetCurrentIndun() == NULL )
		return 1 ;

	// ¡◊¿∫ªÛ≈¬ø°º≠ ∆ƒ∆º∞° «ÿ¡¶µ«¥¬ ∞ÊøÏ INDUN_EXIT¿ª ∫∏≥ª∏È æ»µ»¥Ÿ.
	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();
	if( pLocalPlayer != NULL && pLocalPlayer->IsDead() )
		return 0;

	// «ˆ¿Á ¿Œ¥¯ ≈∏¿‘¿Ã ∆¿¿Œ¥¯¿Ã∏È ≥™∞°±‚
	int iIndunID = g_pkIndunManager->GetIndunLdtID() ;
	indunInfo.Clear() ;
	if( g_pkIndunManager->FindIndunInfo(iIndunID, indunInfo) == false )
		return 0 ;
	if( indunInfo.eType == IDT_PARTY )
	{
		IndunOutRequest() ;
		if( g_pResourceManager->GetGlobalString(50000001) )
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(50000001) );
	}
	return 0 ;
}


/**
	¿Œ¥¯ ∏Æº¬ ∞·∞˙ √≥∏Æ
*/
int SPZone::OnIndunReset(WPARAM wparam, LPARAM lparam)
{
	GU_ID				ObjectGUID;							// ∞‘¿”ø¿∫Í¡ß∆Ææ∆¿Ãµ
	UINT32				uiIndunLDTID;						// ¿Œ¥¯ LDT æ∆¿Ãµ
	GLOBAL_STRING_ID	ErrorID;							// ø°∑Øæ∆¿Ãµ
	TCHAR				szMessage[256] ;
	std::string			strIndunname ;
	CPacket* packet = (CPacket*)lparam;

	packet->ExtractUINT64(&ObjectGUID);
	packet->ExtractUINT32(&uiIndunLDTID);
	packet->ExtractUINT32(&ErrorID);
	
	if( ErrorID == 0 )
	{
		
		if( g_pkIndunManager->GetIndunName(uiIndunLDTID, strIndunname) )	{
			sprintf( szMessage, g_pResourceManager->GetGlobalString(50000006), strIndunname.c_str()) ;
			g_pInterfaceManager->SPPerformMessage(SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMessage);
		}
	}
	else
	{
		if( uiIndunLDTID > 0 )	{		// %s ¿Œ¥¯√ ±‚»≠ «“ºˆ æ¯¿Ω.
			if( g_pkIndunManager->GetIndunName(uiIndunLDTID, strIndunname) )	{
				sprintf( szMessage, g_pResourceManager->GetGlobalString(ErrorID), strIndunname.c_str()) ;
				g_pInterfaceManager->SPPerformMessage(SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMessage);
			}
		}
		else
		{
			if( g_pResourceManager->GetGlobalString(ErrorID) )
				g_pInterfaceManager->SPPerformMessage(SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(ErrorID));
		}
	}

	return 0 ;
}


void SPZone::DirectExit()
{
	if( m_bSendDirectExit == false )	{
		OnExitZone( 1 , 0 );
		//g_pNetworkManager->Disconnect2();
		g_pMainGameManager->SetExit();
		m_bSendDirectExit = true ;
	}
}

int SPZone::OnUserIn(WPARAM wParam, LPARAM lParam)
{
	CPacket* packet = (CPacket*)lParam;

	GLOBAL_STRING_ID	uiErrorID;
	PASSPORT			uiPassport;

	g_pInterfaceManager->ForceReply();

	packet->Extract(&uiErrorID);
	if( uiErrorID == 0 )
	{
		packet->Extract(&uiPassport);
		g_pNetworkManager->SetInPassport(uiPassport);

		g_pNetworkManager->EnterWorld();
	}
	else
	{
		if( uiErrorID == 80012 )		// exception
		{
			if( m_iChannelChangeRetryCount >= 2 )
			{
				SPPerformMessage(SPGM_SUG_SOCKET_FAILED, uiErrorID, 0);
				m_bChannelChangeRetry = false;
			}
			else
			{
				m_bChannelChangeRetry = true;
				m_fChannelWaitTime = 0.0f;
				g_pInterfaceManager->SetNoticeMsg(g_pResourceManager->GetGlobalString(uiErrorID), (SPWindow*)NULL);
			}
		}
		else
			SPPerformMessage(SPGM_SUG_SOCKET_FAILED, uiErrorID, 0);
	}

	return 0;
}

int SPZone::OnUserMove(WPARAM wParam, LPARAM lParam)
{
	CPacket* packet = (CPacket*)lParam;

	GLOBAL_STRING_ID	uiErrorID;
	packet->Extract(&uiErrorID);

	if( uiErrorID == 0 )
	{
		if( m_NextSubGameID	== SUBGAME_LOBBY )
		{
			g_pNetworkManager->Disconnect2();
			OnLeaveWorld(0,0);
		}
		else
		{
			g_pNetworkManager->SetExceptSocketFailed(true);
			g_pNetworkManager->Disconnect2();

			// √§≥Œ¿ÃµøΩ√ø°∏∏ √ ±‚»≠ «“∞Õ [1/26/2006 AJJIYA]
			//g_pInterfaceManager->SPChildWndSendMessage( WIID_SHORTCUT	, SPIM_PURGE , NULL , NULL );
			g_pInterfaceManager->SPChildWndSendMessage( WIID_HELP		, SPIM_PURGE , NULL , NULL );
			g_pInterfaceManager->SPChildWndSendMessage( WIID_CHAT		, SPIM_PURGE , NULL , NULL );

			g_pInterfaceManager->ForceReply();

			g_pGOBManager->PurgeAll();
			g_pInterfaceManager->PurgeAll();			
			
			if( m_iChannelChangeType == 1 )				// Channel Change
				g_pInterfaceManager->ShowWindow(WIID_CHANNEL_CHANGE, false);

			SPPerformMessage(SPGM_CHANNEL_CHANGE_WAIT, 3);
			m_iChannelChangeRetryCount = 0;
			m_bChannelChangeRetry = false;
		}
	}
	else
		SPPerformMessage(SPGM_SUG_SOCKET_FAILED, uiErrorID, 0);

	return 0;
}


int SPZone::OnSocketFailed(WPARAM wparam, LPARAM lparam)
{	
	if(g_pEventManager->GetCutInState() > CUT_IN_STATE_NULL)
		g_pEventManager->SetCutInState(CUT_IN_STATE_NULL);

	g_pInterfaceManager->SPChildWndSendMessage(WIID_EXIT, SPIM_BTN_ENABLE, 1, 0);
	return SPSubGameManager::OnSocketFailed(wparam, lparam);
}












