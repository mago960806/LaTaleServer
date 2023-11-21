
#include "SPCommon.h"
#include "SPUtil.h"
#include "LT_Error.h"
#include "SPResourceDEF.h"
#include "SPMainGameDEF.H"
#include "SPWindowDEF.h"

#include "SPManager.h"
#include "SPSubGameManager.h"
#include "SPPrologue.h"

#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "SPRenderModelDEF.h"
#include "SPRenderModel.h"
#include "SPFXDEF.h"
#include "SPFXModel.h"
#include "SPGOBManager.h"
#include "SPFXManager.h"

#include "Packet.h"

//------------------------------------------------------------------------------------
SPPrologue::SPPrologue()
: SPSubGameManager(SUBGAME_PROLOGUE)
, m_pkFadeTexture(NULL)
{
}

//------------------------------------------------------------------------------------
SPPrologue::~SPPrologue()
{
	Clean();
}

//------------------------------------------------------------------------------------
void SPPrologue::Clean()
{
	SAFE_RELEASE(m_pkFadeTexture);
	SPSubGameManager::Clean();
}

//------------------------------------------------------------------------------------
bool SPPrologue::Init()
{
	SPSubGameManager::Init();

	g_pFXManager->LoadExtraFXLDT(
		"DATA/LDT/PROLOGUE_FX_UNIT.LDT", "DATA/LDT/PROLOGUE_FX_GROUP.LDT", RES_FNAME_FX);

	g_pVideo->GetTextureMgr()->LoadTexture("DATA/INTERFACE/CONCEPT/UI100.PNG", &m_pkFadeTexture);
	//SetRect(&m_rcSrcFade, 128, 392, 132, 396);
	SetRect(&m_rcSrcFade, 255, 21, 257, 23);
	SetRect(&m_rcDstFade, 0, 0, 800, 600);
	m_iAlphaFade = 255;
	m_fFadeAccmulateTime = 0.0f;
	m_iGameState = GS_FADE_OUT;
	
	g_pSubGameManager->SPPerformMessage(SPGM_SUG_SETBGM , (WPARAM)"DATA/BGM/PROLOGUE1.mp3");
	return true;
}

//------------------------------------------------------------------------------------
void SPPrologue::ProcessFade(float fTime)
{
	if( m_iGameState == GS_FADE_IN )
	{
		m_iAlphaFade += 5;
		if( m_iAlphaFade > 255 )
		{
			m_iGameState = GS_RELEASE_READY;
			m_NextSubGameID	= SUBGAME_LOADING;
			m_fWaitTime = 0.1f;

			m_iAlphaFade = 255;
		}
	}
	else
	{
		m_iAlphaFade -= 5;
		if( m_iAlphaFade < 0 )
		{
			m_iGameState = GS_GAME;	
			m_iAlphaFade = 0;
		}
	}
}

//------------------------------------------------------------------------------------
void SPPrologue::Process(float fTime)
{
	switch(m_iGameState)
	{
	case GS_GAME:
		g_pNetworkManager->Process(fTime);
		g_pInputManager->Process(fTime);
		//g_pCameraManagr->Process(fTime);
		g_pInterfaceManager->Process(fTime);
		g_pFXManager->Process(fTime);
		break;
	case GS_RELEASE_READY:
		m_fWaitTime -= fTime;
		if( m_fWaitTime <= 0 )
			m_iGameState = GS_RELEASE;				
		else 
		{
			g_pNetworkManager->Process(fTime);
			g_pInputManager->Process(fTime);
			g_pInterfaceManager->Process(fTime);
		}
		break;
	case GS_WAIT:
		m_fWaitTime -= fTime;
		if( m_fWaitTime <= 0 )
		{
			string strGameServerIP = g_pNetworkManager->GetGameServerIP();
			int iGameServerPort = g_pNetworkManager->GetGameServerPort();
			if( !g_pNetworkManager->ConnectToServer(strGameServerIP.c_str(), iGameServerPort) )
				SPPerformMessage(SPGM_SUG_SOCKET_FAILED);

			m_iGameState = GS_GAME;
		}
		break;

	case GS_FADE_IN:
	case GS_FADE_OUT:
		g_pNetworkManager->Process(fTime);
		g_pInputManager->Process(fTime);
		//g_pCameraManagr->Process(fTime);
		g_pInterfaceManager->Process(fTime);
		g_pFXManager->Process(fTime);
		
		m_fFadeAccmulateTime += fTime;
		if( m_fFadeAccmulateTime > 0.02f )
		{
			m_fFadeAccmulateTime -= 0.02f;
			ProcessFade(fTime);
		}

		break;
	}

	SPSubGameManager::Process(fTime);
}

//------------------------------------------------------------------------------------
void SPPrologue::FadeRender()
{
	if( m_pkFadeTexture && m_iAlphaFade )
	{
		m_pkFadeTexture->SetColor(RGBA(255, 255, 255, m_iAlphaFade));
		m_pkFadeTexture->RenderTexture(&m_rcDstFade, &m_rcSrcFade);
		m_pkFadeTexture->SetColor(RGBA(255, 255, 255, 255));		
	}
}

//------------------------------------------------------------------------------------
void SPPrologue::Render(float fTime)
{
	g_pFXManager->Render(fTime);
	g_pInterfaceManager->Render(fTime);
	FadeRender();
	SPSubGameManager::Render(fTime);
}

//------------------------------------------------------------------------------------
bool SPPrologue::WndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	return ((g_pInterfaceManager->WndProc(message, wParam, lParam) == -1) ? true : false);
}

//------------------------------------------------------------------------------------
int SPPrologue::SPPerformMessage(UINT msg, WPARAM wparam, LPARAM lParam)
{
	if(SPGM_SUG_PROLOGUE_BEGIN <= msg && SPGM_SUG_PROLOGUE_END > msg )
		return SPGMESSAGE_FUNC((SPGM)msg, wparam, lParam);
	else if( SPGM_CHAR_SC_SELECT == msg )
		return SPGMESSAGE_FUNC((SPGM)msg, wparam, lParam);

	return SPSubGameManager::SPPerformMessage(msg, wparam, lParam);
}

//------------------------------------------------------------------------------------
// Message
int SPPrologue::OnPrologueComplete(WPARAM wParam, LPARAM lParam)
{
	g_pGOBManager->PurgeAll();
	g_pFXManager->ClearExtraFX();
	g_pNetworkManager->UserMoveRequest();
	return 0;
}

//------------------------------------------------------------------------------------
int SPPrologue::OnWorldEnter(WPARAM wParam, LPARAM lParam)
{
	CPacket* pkPacket = (CPacket*)lParam;
	RESULTCODE resultCode;
	pkPacket->ExtractUINT8(&resultCode);
	
	if( resultCode == LT_NO_ERROR )
	{
		m_iGameState = GS_FADE_IN;
		m_iAlphaFade = 0;
		m_fFadeAccmulateTime = 0.0f;

		//g_pNetworkManager->GameChannel_IN();
		g_pNetworkManager->SetChannelEnteringFlag(true);
	}
	else
		SPPerformMessage(SPGM_SUG_SOCKET_FAILED, 0, 0);

	return 0;
}

//------------------------------------------------------------------------------------
int SPPrologue::OnCharacterSelect(WPARAM wParam, LPARAM lParam)
{
	RESULTCODE result = (RESULTCODE)wParam;
	if( result == LT_NO_ERROR )
		g_pGOBManager->SPPerformMessage(SPGM_CHAR_SC_SELECT, wParam, lParam);
	else
		SPPerformMessage(SPGM_SUG_SOCKET_FAILED, 0, 0);
	
	return 0;
}

//------------------------------------------------------------------------------------
int SPPrologue::OnUserIn(WPARAM wParam, LPARAM lParam)
{
	CPacket* packet = (CPacket*)lParam;

	GLOBAL_STRING_ID	uiErrorID;
	packet->Extract(&uiErrorID);
	if( uiErrorID == 0 )
	{
		g_pNetworkManager->SetIndunID(0);
		g_pNetworkManager->EnterWorld();
	}
	else
		SPPerformMessage(SPGM_SUG_SOCKET_FAILED, uiErrorID, 0);
	
	return 0;
}

//------------------------------------------------------------------------------------
int SPPrologue::OnUserMove(WPARAM wParam, LPARAM lParam)
{
	CPacket* packet = (CPacket*)lParam;

	GLOBAL_STRING_ID	uiErrorID;
	packet->Extract(&uiErrorID);

	if( uiErrorID == 0 )
	{
		g_pNetworkManager->Disconnect2();
		m_fWaitTime = 2.0f;
		m_iGameState = GS_WAIT;
	}
	else
		SPPerformMessage(SPGM_SUG_SOCKET_FAILED, uiErrorID, 0);

	return 0;
}

//------------------------------------------------------------------------------------
SPGMESSAGE_MAP_BEGIN(SPPrologue)

SPGMESSAGE_COMMAND(SPGM_SUG_PROLOGUE_COMPLETE	,	OnPrologueComplete)
SPGMESSAGE_COMMAND(SPGM_WORLD_SC_ENTER			,	OnWorldEnter)
SPGMESSAGE_COMMAND(SPGM_CHAR_SC_SELECT			,	OnCharacterSelect)

SPGMESSAGE_MAP_END(SPSubGameManager)