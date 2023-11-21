
#include "SPCommon.h"
#include "SPUtil.h"
#include "SPTimer.h"
#include "LT_Error.h"
#include "SPResourceDEF.h"
#include "SPMainGameDEF.H"

#include "SPMouseCursor.h"
#include "SPManager.h"
#include "SPGOBManager.h"
#include "SPIMEPool.h"
#include "SPSubGameManager.h"
#include "SPWindowDEF.h"
#include "SPMouseCursor.h"
#include "SPLobby.h"
#include "SPTerrainDef.h"

#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"
#include "SockWnd.h"
#include "SPNetworkManager.h"

#ifdef _DEBUG
	#include "SPDebug.h"
#endif

#include "SPWindowDEF.h"
#include "SPInterfaceManager.h"
#include "SPInputManager.h"

#include "SPManager.h"
#include "SPSubGameManager.h"

#include "SPWindow.h"
#include "SPWindowLobby.h"
#include "SPWindowEdit.h"
#include "SPWindowButton.h"
#include "SPNoticeBox.h"

#include "SPResourceManager.h"
#include "SPMainGameManager.h"

#include "SPLocalizeManager.h"
#include "GlobalDefines_Share.h"


#include "SPRenderModel.h"
#include "SPFXDEF.h"
#include "SPFXModel.h"
#include "SPFXUnit.h"
#include "SPFXGroupUnit.h"
#include "SPFXArchive.h"
#include "_Error_log.h"
//
#include "SPGuildArchive.h"
//#include "SPGameObject.h"
//
//#include "SPGOBStatus.h"
//#include "SPPlayerStatus.h"
//
//#include "SPGOBManager.h"
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
using namespace std;
	


extern char g_szLobbyServerIP[];
extern int g_iLobbyServerPort;
extern bool g_bForceLobbyServer;

//------------------------------------------------------------------------------------
SPLobby::SPLobby()
: SPSubGameManager(SUBGAME_LOBBY)
, m_pkBackgroundFrame(NULL)
, m_pkBackground1(NULL)
, m_pkBackground2(NULL)
, m_pkFXArchive1(NULL)
, m_pkFXArchive2(NULL)
, m_pkFadeTexture(NULL)
, m_pMsgTimer(NULL)
//, m_pkBackCloud(NULL)
{
}

//------------------------------------------------------------------------------------
SPLobby::~SPLobby()
{
	Clean();
}

//------------------------------------------------------------------------------------
void SPLobby::Clean()
{
	SAFE_RELEASE(m_pkBackgroundFrame);
	SAFE_RELEASE(m_pkBackground1);
	SAFE_RELEASE(m_pkBackground2);
	SAFE_DELETE(m_pkFXArchive1);
	SAFE_DELETE(m_pkFXArchive2);
	SAFE_RELEASE(m_pkFadeTexture);
	SAFE_DELETE(m_pMsgTimer) ;

	m_vBackgroundSrcRgn.clear();
	m_vBackgroundDstRgn.clear();
	m_vBackFrameSrcRgn.clear();
	m_vBackFrameDstRgn.clear();

//	SAFE_RELEASE(m_pkBackCloud);		///< Delete 2007.01.02
	m_vParticleSrcRgn.clear();
	m_vSunAlpha.clear();
	m_vParticle.clear();
	
	map< int, std::vector< SPVector > >::iterator iterKey = m_mBezierControl.begin();
	while( iterKey != m_mBezierControl.end() )
	{
		iterKey->second.clear();
		++iterKey;
	}
	m_mBezierControl.clear();

	m_vWorldInfo.clear();
	m_vChannelInfo.clear();
	m_vCharacterInfo.clear();
	ZeroMemory(m_bPrologueShow, sizeof(bool) * 3);

	map< unsigned int, vector< CONTAINER_ITEM > >::iterator iterItem = m_mCharContainerItem.begin();
	while( iterItem != m_mCharContainerItem.end() )
	{
		iterItem->second.clear();
		++iterItem;
	}
	m_mCharContainerItem.clear();

	SPSubGameManager::Clean();
	ErrorFLog::Release() ;
}

//------------------------------------------------------------------------------------
bool SPLobby::Init()
{
	Clean();
	m_pMsgTimer = new SPTimer ;
	m_pMsgTimer->Clear() ;
	m_bChannelMsgClose = false ;
	
    m_IGWServerName.clear();
	m_IGWServerID = -1;

	g_pNetworkManager->FDS_SetChat(false) ;

	bool bSuccess;
	bSuccess = g_pVideo->GetTextureMgr()->LoadTexture("DATA/LOBBY/RB_FRM.PNG", &m_pkBackgroundFrame);
	if( !bSuccess ) {
		ErrorFLogs->Error_Log("[SPLobby::Init] [DATA/LOBBY/RB_FRM.PNG] LoadFail") ;
		return false;
	}

	bSuccess = g_pVideo->GetTextureMgr()->LoadTexture("DATA/LOBBY/RB_BACK_00.PNG", &m_pkBackground1);
	if( !bSuccess ) {
		ErrorFLogs->Error_Log("[SPLobby::Init] [DATA/LOBBY/RB_BACK_00.PNG] LoadFail") ;
		return false;
	}
	bSuccess = g_pVideo->GetTextureMgr()->LoadTexture("DATA/LOBBY/RB_BACK_01.PNG", &m_pkBackground2);
	if( !bSuccess ) {
		ErrorFLogs->Error_Log("[SPLobby::Init] [DATA/LOBBY/RB_BACK_01.PNG] LoadFail") ;
		return false;
	}

	///< Delete 2007.01.02
	//bSuccess = g_pVideo->GetTextureMgr()->LoadTexture("DATA/LOBBY/RB_CLOUD.PNG", &m_pkBackCloud);
	//if( !bSuccess )
	//	return false;

	bSuccess = g_pVideo->GetTextureMgr()->LoadTexture("DATA/INTERFACE/CONCEPT/UI100.PNG", &m_pkFadeTexture);
	if( !bSuccess ) {
		ErrorFLogs->Error_Log("[SPLobby::Init] [DATA/INTERFACE/CONCEPT/UI100.PNG] LoadFail") ;
		return false;
	}

	m_fWaitTime = 0.0f;

	RgnInit();
	//ParticleInit();		// Delete 2007.01.02

	// Load FX
	m_pkFXArchive1 = new SPFXArchive(NULL);		///<FX
	m_pkFXArchive2 = new SPFXArchive(NULL);		///<FX
	m_pkFXArchive1->InsertFXGroup((SPID_FX_GROUP)15063);	// Back
	m_pkFXArchive2->InsertFXGroup((SPID_FX_GROUP)15064);	// Frount

	//SetRect(&m_rcSrcFade, 128, 392, 132, 396);
	SetRect(&m_rcSrcFade, 255, 21, 257, 23);
	SetRect(&m_rcDstFade, 0, 0, 800, 600);
	m_iAlphaFade = 0;
	m_fFadeAccmulateTime = 0.0f;
	
	m_iCurServer = -1;
	m_bGameServerConnectReq = true;
	m_iConnectType = 0;
	m_iGameServerID = 0;
	m_iSelectCard = -1;

	m_bOnceSendPacket = false;
	m_bEnterTimeCheck = false;
	InitEnterTime();

	SPWindow* pkLobbyRootWindow = g_pInterfaceManager->GetLobbyWindow();
	if( pkLobbyRootWindow )
	{
		SPWindowLobby* pkWindow = (SPWindowLobby*)pkLobbyRootWindow->Find(WIID_LOBBY);
		if( pkWindow )
			pkWindow->ClearImageProcessor();
	}

	m_fGameServerConnectWaitTime = 0.0f;
	m_iGameServerConnectType = 0;
	SPSubGameManager::Init();

	if( g_ExcutionMode == EMODE_NORMAL )
	{
		if(!g_pNetworkManager->IsConnected())
		{
			m_iConnectType = 3;
			m_fWaitTime = 1.0f;
			m_iGameState = GS_WAIT;
		}
	}
	else
	{
		m_iConnectType = 1;
		m_fWaitTime = 1.0f;
		m_iGameState = GS_WAIT;
	}

	if( g_pNetworkManager->IsConnected() ) {
		ErrorFLogs->Error_Log("[SPLobby::Init] WorldList Request ") ;
		GetWorldListInfoRequest();
	}

	return true;
}

//------------------------------------------------------------------------------------
void SPLobby::RgnInit()
{
	RECT rcRgn;
	m_vBackgroundSrcRgn.clear();
	m_vBackgroundDstRgn.clear();

	// Background
	// Src rgn set
	SetRect(&rcRgn, 0, 0, 512, 512);
	m_vBackgroundSrcRgn.push_back(rcRgn);

	SetRect(&rcRgn, 512, 0, 800, 512);
	m_vBackgroundSrcRgn.push_back(rcRgn);

	SetRect(&rcRgn, 800, 0, 1024, 88);
	m_vBackgroundSrcRgn.push_back(rcRgn);

	SetRect(&rcRgn, 800, 88, 1024, 176);
	m_vBackgroundSrcRgn.push_back(rcRgn);

	SetRect(&rcRgn, 800, 176, 1024, 264);
	m_vBackgroundSrcRgn.push_back(rcRgn);

	SetRect(&rcRgn, 800, 264, 928, 352);
	m_vBackgroundSrcRgn.push_back(rcRgn);


	// Dst rgn set
	SetRect(&rcRgn, 0, 0, 512, 512);
	m_vBackgroundDstRgn.push_back(rcRgn);

	SetRect(&rcRgn, 512, 0, 800, 512);
	m_vBackgroundDstRgn.push_back(rcRgn);

	SetRect(&rcRgn, 0, 512, 224, 600);
	m_vBackgroundDstRgn.push_back(rcRgn);

	SetRect(&rcRgn, 224, 512, 448, 600);
	m_vBackgroundDstRgn.push_back(rcRgn);

	SetRect(&rcRgn, 448, 512, 672, 600);
	m_vBackgroundDstRgn.push_back(rcRgn);

	SetRect(&rcRgn, 672, 512, 800, 600);
	m_vBackgroundDstRgn.push_back(rcRgn);

	// BackFrame
	// Src rgn set
	SetRect(&rcRgn, 0, 0, 112, 112);
	m_vBackFrameSrcRgn.push_back(rcRgn);

	SetRect(&rcRgn, 144, 0, 256, 112);
	m_vBackFrameSrcRgn.push_back(rcRgn);

	SetRect(&rcRgn, 0, 144, 112, 256);
	m_vBackFrameSrcRgn.push_back(rcRgn);

	SetRect(&rcRgn, 144, 144, 256, 256);
	m_vBackFrameSrcRgn.push_back(rcRgn);

	SetRect(&rcRgn, 112, 0, 144, 16);
	m_vBackFrameSrcRgn.push_back(rcRgn);

	SetRect(&rcRgn, 240, 112, 256, 144);
	m_vBackFrameSrcRgn.push_back(rcRgn);

	SetRect(&rcRgn, 112, 240, 144, 256);
	m_vBackFrameSrcRgn.push_back(rcRgn);

	SetRect(&rcRgn, 0, 112, 16, 144);
	m_vBackFrameSrcRgn.push_back(rcRgn);

	// Dst rgn set
	SetRect(&rcRgn, 0, 0, 112, 112);
	m_vBackFrameDstRgn.push_back(rcRgn);

	SetRect(&rcRgn, 688, 0, 800, 112);
	m_vBackFrameDstRgn.push_back(rcRgn);

	SetRect(&rcRgn, 0, 488, 112, 600);
	m_vBackFrameDstRgn.push_back(rcRgn);

	SetRect(&rcRgn, 688, 488, 800, 600);
	m_vBackFrameDstRgn.push_back(rcRgn);

	SetRect(&rcRgn, 112, 0, 688, 16);
	m_vBackFrameDstRgn.push_back(rcRgn);

	SetRect(&rcRgn, 784, 112, 800, 488);
	m_vBackFrameDstRgn.push_back(rcRgn);

	SetRect(&rcRgn, 112, 584, 688, 600);
	m_vBackFrameDstRgn.push_back(rcRgn);

	SetRect(&rcRgn, 0, 112, 16, 488);
	m_vBackFrameDstRgn.push_back(rcRgn);



	vector< SPVector > vVector;

	vVector.push_back(SPVector(337.0f, 614.0f, 0.0f));
	vVector.push_back(SPVector(453.0f, 585.0f, 0.0f));
	vVector.push_back(SPVector(479.0f, 507.0f, 0.0f));
	vVector.push_back(SPVector(614.0f, 550.0f, 0.0f));
	vVector.push_back(SPVector(681.0f, 449.0f, 0.0f));
	vVector.push_back(SPVector(769.0f, 479.0f, 0.0f));
	vVector.push_back(SPVector(815.0f, 394.0f, 0.0f));

	m_mBezierControl.insert(make_pair(0, vVector));
	vVector.clear();

	vVector.push_back(SPVector(446.0f, 612.0f, 0.0f));
	vVector.push_back(SPVector(554.0f, 576.0f, 0.0f));
	vVector.push_back(SPVector(531.0f, 463.0f, 0.0f));
	vVector.push_back(SPVector(655.0f, 442.0f, 0.0f));
	vVector.push_back(SPVector(639.0f, 331.0f, 0.0f));
	vVector.push_back(SPVector(757.0f, 273.0f, 0.0f));
	vVector.push_back(SPVector(799.0f, 126.0f, 0.0f));
	vVector.push_back(SPVector(822.0f,  41.0f, 0.0f));

	m_mBezierControl.insert(make_pair(1, vVector));
	vVector.clear();

	vVector.push_back(SPVector(512.0f, 614.0f, 0.0f));
	vVector.push_back(SPVector(538.0f, 520.0f, 0.0f));
	vVector.push_back(SPVector(685.0f, 421.0f, 0.0f));
	vVector.push_back(SPVector(647.0f, 312.0f, 0.0f));
	vVector.push_back(SPVector(751.0f, 148.0f, 0.0f));
	vVector.push_back(SPVector(747.0f,  27.0f, 0.0f));
	vVector.push_back(SPVector(830.0f,  -6.0f, 0.0f));

	m_mBezierControl.insert(make_pair(2, vVector));
	vVector.clear();
}

//------------------------------------------------------------------------------------
void SPLobby::ParticleInit()
{
	PARTICLE Particle;
	SPVector* pkVector;
	int iControlNum, i, j;
	RECT rcRgn;

	m_vParticleSrcRgn.clear();
	m_vSunAlpha.clear();

	m_iCurSunFrame = 0;
	m_fSunDelayTime = 0.1f;
	m_fCloudDelayTime = 0.02f;
	m_fCloudAccmulateTime = 0.0f;

	m_fCloudRotation = 0.0f;
	m_fCloudIncRotation = 0.0005555f;

	for( i = 0; i < 20; i++ )
	{
		Particle.iResourceType = rand() % 3;
		Particle.iPathType = rand() % 3;
		Particle.bLive = true;
		Particle.fTime = -(float)(rand() % 30);
		Particle.fLiveTime = (float)(rand() % 20) + 5.0f;
		Particle.fResponeTime = 2.0f;

		map< int, std::vector< SPVector > >::iterator iterKey;

		iterKey = m_mBezierControl.find(Particle.iPathType);
		iControlNum = (int)iterKey->second.size();

		pkVector = new SPVector[iControlNum];
		for( j = 0; j < iControlNum; j++ )
			pkVector[j] = iterKey->second[j];

		Particle.kBezier.SetControlPoint(iControlNum, pkVector);
		m_vParticle.push_back(Particle);
		delete[] pkVector;
	}

	// particle RECT
	SetRect(&rcRgn, 800, 352, 928, 480);
	m_rcSunRgn = rcRgn;
	m_vSunAlpha.push_back(0.9f);
	m_vSunAlpha.push_back(0.85f);
	m_vSunAlpha.push_back(0.8f);
	m_vSunAlpha.push_back(0.85f);

	SetRect(&rcRgn, 928, 352, 992, 416);
	m_vParticleSrcRgn.push_back(rcRgn);
	SetRect(&rcRgn, 928, 416, 960, 448);
	m_vParticleSrcRgn.push_back(rcRgn);
	SetRect(&rcRgn, 960, 416, 992, 448);
	m_vParticleSrcRgn.push_back(rcRgn);

}

//------------------------------------------------------------------------------------
void SPLobby::ParticleRespone(PARTICLE* pParticle)
{
	pParticle->fTime = 0.0f;
	pParticle->bLive = true;
}

//------------------------------------------------------------------------------------
void SPLobby::InitEnterTime()
{
	m_fEnterAccmulateTime = 0.0f;
	m_fMaxEnterTime = 600.0f;
}

//------------------------------------------------------------------------------------
bool SPLobby::CheckEnterTime(float fTime)
{
	if( m_bEnterTimeCheck )
	{
		m_fEnterAccmulateTime += fTime;
		if( m_fEnterAccmulateTime > m_fMaxEnterTime )
		{
			OnCharacterBack(0, 0);
			return true;
		}
	}

	return false;
}

//------------------------------------------------------------------------------------
void SPLobby::ProcessFade(float fTime)
{
	m_iAlphaFade += 5;
	if( m_iAlphaFade > 255 )
	{
		m_iGameState = GS_RELEASE_READY;	
		m_NextSubGameID	= SUBGAME_PROLOGUE;
		m_iAlphaFade = 255;
	}
}

//------------------------------------------------------------------------------------
void SPLobby::ProcessHotKey()
{
	SPInputStruct* pkInput = g_pInputManager->GetInputStruct();
	if( pkInput->ikReturn.InputState == INPUT_PRESS && m_bGameServerConnectReq )
	{
		//if( g_pNetworkManager->CharSelectName(g_pNetworkManager->GetLoginID()) )
		//{
		//	//m_iGameState = GS_RELEASE_READY;
		//	//m_NextSubGameID	= SUBGAME_LOADING;

		//	m_bGameServerConnectReq = false;
		//}
		//else
		//	m_bGameServerConnectReq = true;
	}
}

//------------------------------------------------------------------------------------
void SPLobby::Process(float fTime)
{
	switch( m_iGameState )
	{
	case GS_GAME:
		{	
			g_pNetworkManager->Process(fTime);
			g_pInputManager->Process(fTime);
			g_pInterfaceManager->Process(fTime);

			CheckEnterTime(fTime);
			// Channel List Message Notice Display for 2 Seconds
			if( m_pMsgTimer->CheckTimer(fTime) == true )
			{
				if( m_bChannelMsgClose == true ) {
					ShowNoticeBox(false, NOTICE_TYPE_MSG) ;			//> Close 80002
					m_pMsgTimer->Clear() ;
					m_bChannelMsgClose = false ;
				}
			}
			
#if !defined(_DEBUG)
			if( m_iConnectType == 1 )
			{
				m_fGameServerConnectWaitTime += fTime;
				if( m_fGameServerConnectWaitTime > 10.0f )
				{
					SPWindow* pkLobbyRootWindow = g_pInterfaceManager->GetLobbyWindow();
					SPWindowLobby* pkWindow = NULL;
					if( pkLobbyRootWindow )
						pkWindow = (SPWindowLobby*)pkLobbyRootWindow->Find(WIID_LOBBY);

					if( m_iGameServerConnectType == 0 )
					{
						if( pkWindow )
						{
							ShowNoticeBox(true, NOTICE_TYPE_YES, g_pResourceManager->GetGlobalString(30008));	///> Network Traffic Jam
						}
						m_iGameServerConnectType++;
					}
					else if( m_iGameServerConnectType == 1 )
					{
						if( m_fGameServerConnectWaitTime > 40.0f )
						{
							ShowNoticeBox(true, NOTICE_TYPE_YES, g_pResourceManager->GetGlobalString(30008));	///> Network Traffic Jam
							pkWindow->EnableChildWindow(1, true);

							g_pNetworkManager->Disconnect2();
							m_iGameServerConnectType++;
						}
					}
					else
					{
						if( m_fGameServerConnectWaitTime > 41.0f )
						{
							SPPerformMessage(SPGM_SUG_SOCKET_FAILED, 0, 0);
							//if(g_pNetworkManager->ConnectToServer(g_szLobbyServerIP,g_iLobbyServerPort))
							//{
							//	InitEnterTime();
							//	m_bEnterTimeCheck = true;
							//}

							m_iConnectType = 0;
						}
					}
				}
			}
#endif
		}
		break;

	case GS_RELEASE_READY:
		{
			m_fWaitTime -= fTime;
			if( m_fWaitTime <= 0 )
			{
				m_iGameState = GS_RELEASE;				
				
				SPWindow* pkLobbyRootWindow = g_pInterfaceManager->GetLobbyWindow();
				if( pkLobbyRootWindow )
				{
					SPWindowLobby* pkWindow = (SPWindowLobby*)pkLobbyRootWindow->Find(WIID_LOBBY);
					if( pkWindow )
					{
						ShowNoticeBox(false, NOTICE_TYPE_MSG) ;		//> Close 60000

						SPWindow* pkSelectButton = pkWindow->Find(WIID_LOBBY_CHAR_MENU_SELECT, true);
						if( pkSelectButton )
							pkSelectButton->SetEnable(true);
					}
				}
			} 
			else 
			{
				g_pNetworkManager->Process(fTime);
				g_pInputManager->Process(fTime);
				g_pInterfaceManager->Process(fTime);
			}
		}
		break;

	case GS_WAIT:
		{
			m_fWaitTime -= fTime;
			if( m_fWaitTime <= 0 )
			{
				SPWindow* pkLobbyRootWindow = g_pInterfaceManager->GetLobbyWindow();
				SPWindowLobby* pkWindow = NULL;
				if( pkLobbyRootWindow )
					pkWindow = (SPWindowLobby*)pkLobbyRootWindow->Find(WIID_LOBBY);

				if( m_iConnectType == 1 )
				{
					if( m_iSelectCard >= 0 && m_bPrologueShow[m_iSelectCard] )
					{
						//m_iGameState = GS_RELEASE_READY;	
						//m_NextSubGameID	= SUBGAME_PROLOGUE;
						//m_fWaitTime = 1.0f;
						m_iGameState = GS_FADE_IN;
						m_fFadeAccmulateTime = 0.0f;
						m_iAlphaFade = 0;
					}
					else
					{
						string strGameServerIP = g_pNetworkManager->GetGameServerIP();
						int iGameServerPort = g_pNetworkManager->GetGameServerPort();
						if( !g_pNetworkManager->ConnectToServer(strGameServerIP.c_str(), iGameServerPort) )
						{
							if( pkWindow )
							{
								ShowNoticeBox(true, NOTICE_TYPE_YES, g_pResourceManager->GetGlobalString(30007));	///> Connect Fail
								pkWindow->EnableChildWindow(1, true);

								m_iConnectType = 0;
							}
						}

						m_iGameState = GS_GAME;
					}
				}
				else if( m_iConnectType == 2 )		// ServerList -> Character
				{
					if( g_pNetworkManager->ConnectToServer(g_szLobbyServerIP,g_iLobbyServerPort) )
					{
						//g_pNetworkManager->UserInRequest();
					}
					else
					{
						SPPerformMessage(SPGM_SUG_SOCKET_FAILED, 0, 0);
						m_iConnectType = 0;
					}

					m_iGameState = GS_GAME;
				}
				else if( m_iConnectType == 3 )		// Character -> ServerList
				{
					pkWindow->InitLobbyType();
					pkWindow->ShowLobbyType(0);
					m_bEnterTimeCheck = false;

					// 岿靛/盲澄 急琶甫 困秦 肺变辑滚肺 立加
					if( g_pNetworkManager->ConnectToLoginServer() )
					{
						//g_pNetworkManager->UserInRequest();
					}
					else
					{
						SPPerformMessage(SPGM_SUG_SOCKET_FAILED, 0, 0);
						m_iConnectType = 0;
					}

					m_iGameState = GS_GAME;
				}
			}
		}
		break;

	case GS_FADE_IN:
		{
			g_pNetworkManager->Process(fTime);
			g_pInputManager->Process(fTime);
			g_pInterfaceManager->Process(fTime);

			m_fFadeAccmulateTime += fTime;
			if( m_fFadeAccmulateTime > 0.02f )
			{
				m_fFadeAccmulateTime -= 0.02f;
				ProcessFade(fTime);
			}
			
		}
		break;
	}

	//ProcessHotKey();
	//ProcessParticle(fTime);		///< Delete 2007.01.02
	if( m_pkFXArchive1 != NULL )
		m_pkFXArchive1->Process(fTime) ;
	if( m_pkFXArchive2 != NULL )
		m_pkFXArchive2->Process(fTime) ;

	SPSubGameManager::Process(fTime);
}

//------------------------------------------------------------------------------------
void SPLobby::ProcessParticle(float fTime)
{
	if( m_fAccumulateRenderTime > m_fSunDelayTime )
	{
		m_fAccumulateRenderTime -= m_fSunDelayTime;

		m_iCurSunFrame++;
		if( m_iCurSunFrame >= (int)m_vSunAlpha.size() )
			m_iCurSunFrame = 0;
	}

	if( m_fCloudAccmulateTime > m_fCloudDelayTime )
	{
		m_fCloudAccmulateTime -= m_fCloudDelayTime;
		m_fCloudRotation += m_fCloudIncRotation;
	}

	int iSize = (int)m_vParticle.size();
	int i;
	for( i = 0; i < iSize; i++ )
	{
		m_vParticle[i].fTime += fTime;
		if( m_vParticle[i].fTime >= 0.0f && m_vParticle[i].bLive )
		{
			float t = m_vParticle[i].fTime / m_vParticle[i].fLiveTime;
			if( t > 1.0f )
			{
				t = 1.0f;
				m_vParticle[i].bLive = false;
				m_vParticle[i].fTime = 0.0f;
			}

			m_vParticle[i].kPos = m_vParticle[i].kBezier.GetInterpolation(t);
		}
		else
		{
			if( m_vParticle[i].fTime > m_vParticle[i].fResponeTime )
			{
				ParticleRespone(&m_vParticle[i]);
			}
		}
	}
	m_fCloudAccmulateTime += fTime;
}

//------------------------------------------------------------------------------------
void SPLobby::Render(float fTime)
{
	BackgroundRender(fTime);

	g_pInterfaceManager->Render(fTime);
	FadeRender();
	SPSubGameManager::Render(fTime);
}

//------------------------------------------------------------------------------------
void SPLobby::BackgroundRender(float fTime)
{
	if( m_pkBackground1 == NULL ||
		m_pkBackground2 == NULL ||
		m_pkBackgroundFrame == NULL )
		return;

	int i;
	int iSize = (int)m_vBackgroundSrcRgn.size();

	/*		///< Delete 2007.01.02
	RECT rcSrc;
	float fCloudScale = 2.14844f;

	SetRect(&rcSrc, 0, 0, m_pkBackCloud->GetLenX(), m_pkBackCloud->GetLenY());
	
	m_pkBackCloud->SetSrcRect(rcSrc);
	m_pkBackCloud->SetScale(fCloudScale, fCloudScale);
	m_pkBackCloud->SetRotation(m_fCloudRotation);
	m_pkBackCloud->RenderTextureImmediateCenterBase(192, 0);
	m_pkBackCloud->SetScale();
	m_pkBackCloud->SetRotation(0.0f);
	g_pVideo->Flush();
	*/

	//////////////////////////////////////////////////////////////////////////
	// 1. Back Image
	for( i = 0; i < iSize; i++ ) {
		m_pkBackground1->RenderTexture(&m_vBackgroundDstRgn[i], &m_vBackgroundSrcRgn[i]);
	}
	g_pVideo->Flush();
	// 2. Back FX
	if( m_pkFXArchive1 != NULL )
		m_pkFXArchive1->Render(fTime) ;
	// 3. Front Image
	for( i = 0; i < iSize; i++ ) {
		m_pkBackground2->RenderTexture(&m_vBackgroundDstRgn[i], &m_vBackgroundSrcRgn[i]);
	}
	// 4. Front FX
	if( m_pkFXArchive2 != NULL )
		m_pkFXArchive2->Render(fTime) ;

	///ParticleRender();	///< Delete 2007.01.02

	// frame
	iSize = (int)m_vBackFrameSrcRgn.size();
	for( i = 0; i < iSize; i++ )
		m_pkBackgroundFrame->RenderTexture(&m_vBackFrameDstRgn[i], &m_vBackFrameSrcRgn[i]);

	g_pVideo->Flush();	
}

//------------------------------------------------------------------------------------
// NotUsed 2007.01.02
void SPLobby::ParticleRender()
{
	RECT rcDst, rcSrc;

	g_pVideo->SetAlphaTexture(true);

	// sun
	SetRect(&rcDst, 0, 0, 256, 256);
	m_pkBackground1->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_vSunAlpha[m_iCurSunFrame]));
	m_pkBackground1->RenderTexture(&rcDst, &m_rcSunRgn);
	m_pkBackground1->SetColor();

	// particle
	int iSrcWidth, iSrcHeight;
	int iSize, i;

	iSize = (int)m_vParticle.size();
	for( i = 0; i < iSize; i++ )
	{
		if( m_vParticle[i].bLive && m_vParticle[i].fTime >= 0.0f )
		{
			rcSrc = m_vParticleSrcRgn[m_vParticle[i].iResourceType];
			iSrcWidth = rcSrc.right - rcSrc.left;
			iSrcHeight = rcSrc.bottom - rcSrc.top;

			SetRect(&rcDst, (int)m_vParticle[i].kPos.m_fX, (int)m_vParticle[i].kPos.m_fY,
				(int)m_vParticle[i].kPos.m_fX + iSrcWidth, (int)m_vParticle[i].kPos.m_fY + iSrcHeight);

			m_pkBackground1->RenderTexture(&rcDst, &rcSrc);
		}
	}

	g_pVideo->SetAlphaTexture(false);
}

//------------------------------------------------------------------------------------
void SPLobby::FadeRender()
{
	if( m_pkFadeTexture && m_iAlphaFade )
	{
		m_pkFadeTexture->SetColor(RGBA(255, 255, 255, m_iAlphaFade));
		m_pkFadeTexture->RenderTexture(&m_rcDstFade, &m_rcSrcFade);
		m_pkFadeTexture->SetColor(RGBA(255, 255, 255, 255));		
	}
}

//------------------------------------------------------------------------------------
bool SPLobby::WndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	return ((g_pInterfaceManager->WndProc(message, wParam, lParam) == -1) ? true : false);
}

//------------------------------------------------------------------------------------
int SPLobby::SPPerformMessage(UINT msg, WPARAM wparam, LPARAM lParam)
{
	if(SPGM_SUG_LOGGIN_BEGIN <= msg && SPGM_SUG_LOGGIN_END > msg )
		return SPGMESSAGE_FUNC((SPGM)msg, wparam, lParam);

	return SPSubGameManager::SPPerformMessage(msg, wparam, lParam);
}

//------------------------------------------------------------------------------------
// command
int SPLobby::OnServerBack(WPARAM wParam, LPARAM lParam)
{
	m_iGameState = GS_RELEASE_READY;
	m_NextSubGameID	= SUBGAME_LOGGIN;

	g_pNetworkManager->Disconnect2();

	// BuddyBuddy狼 版快 辆丰秦具 窃.
	if( SPLocalizeManager::GetInstance()->OnlyWebLogin() )
	{
		g_pMainGameManager->SetExit();
		m_iGameState = GS_RELEASE_READY;
		m_NextSubGameID	= SUBGAME_NULL;		// 促澜悼累捞 绝档废 茄促.
	}	
	//登出游戏 
	//gai
	if ( g_pCIGWLoaderDx9 )
	{
		g_pCIGWLoaderDx9->LogoutA();
	}
	return 0;
}

//------------------------------------------------------------------------------------
int SPLobby::OnCharacterMenuSelect(WPARAM wParam, LPARAM lParam)
{
	int iSelectCard = (int)wParam;
	if( iSelectCard >= (int)m_vCharacterInfo.size() || iSelectCard < 0)
		return 0;
	
	if( _tcslen(m_vCharacterInfo[iSelectCard].BaseInfo.szName) == 0 )
		return 0;
	SPWindow* pkLobbyRootWindow = g_pInterfaceManager->GetLobbyWindow();
	SPWindowLobby* pkWindow = NULL;
	if( pkLobbyRootWindow )
	{
		pkWindow = (SPWindowLobby*)pkLobbyRootWindow->Find(WIID_LOBBY);
		if( pkWindow == NULL )
			return 0;

		ShowNoticeBox(true, NOTICE_TYPE_MSG, g_pResourceManager->GetGlobalString(60000));	///> Connecting...
		pkWindow->EnableChildWindow(1, false);
	}
	
	m_iConnectType = 1;
	m_iSelectCard = iSelectCard;
	g_pNetworkManager->SetServerID(m_iGameServerID);
	g_pNetworkManager->SetGameServerID(m_iGameServerID);
	if( m_bPrologueShow[iSelectCard] )
	{
		m_fWaitTime = 1.0f;
		m_iGameState = GS_WAIT;

		m_fGameServerConnectWaitTime = 0.0f;
		m_iGameServerConnectType = 0;

		if( g_ExcutionMode == EMODE_NORMAL )
			g_pNetworkManager->SetCharName(m_vCharacterInfo[iSelectCard].BaseInfo.szName);
	}
	else
	{
		g_pNetworkManager->UserMoveRequest();
	}
	
	return 0;
}

//------------------------------------------------------------------------------------
int SPLobby::OnCharacterMenuCreate(WPARAM wParam, LPARAM lParam)
{
	SPWindow* pkLobbyRootWindow = g_pInterfaceManager->GetLobbyWindow();
	if( pkLobbyRootWindow )
	{
		SPWindowLobby* pkWindow = (SPWindowLobby*)pkLobbyRootWindow->Find(WIID_LOBBY);
		if( pkWindow == NULL )
			return 0;

		pkWindow->ShowLobbyType(2);
	}

	return 0;
}

//------------------------------------------------------------------------------------
int SPLobby::OnCharacterMenuDelete(WPARAM wParam, LPARAM lParam)
{
	int iSelect = (int)wParam;
	//const char* pszString = (const char*)lParam;

	if( iSelect >= (int)m_vCharacterInfo.size() )
		return 0;

	CPacket kPacket;
	kPacket.AddUINT32(CHAR_CS_DELETE);
	WORLD_ID worldID = g_pNetworkManager->GetWorldID();
	kPacket.AddUINT16(worldID);
	kPacket.AddUINT32(g_pNetworkManager->GetUserID());

	CLASS_ID classID;
	CHARID charID;
	GetLocalID(m_vCharacterInfo[iSelect].BaseInfo.uiGlobalID,classID,charID);
	kPacket.AddUINT32(charID);

	DECLARE_VAR(TCHAR,szCharName,MAX_LEN_NAME);
	strncpy(szCharName, m_vCharacterInfo[iSelect].BaseInfo.szName, LEN_NAME);
	kPacket.Add(szCharName, LEN_NAME + 1);

	(g_pNetworkManager->GetPacketHandler())->PostMsg(&kPacket);
	return 0;
}

//------------------------------------------------------------------------------------
int SPLobby::OnCharacterBack(WPARAM wParam, LPARAM lParam)
{
	ShowNoticeBox(true, NOTICE_TYPE_MSG, g_pResourceManager->GetGlobalString(80001)) ;		//> For World List

	g_pNetworkManager->SetServerID(g_pNetworkManager->GetLoginServerID());
	g_pNetworkManager->UserMoveRequest();

	m_iConnectType = 3;
	m_bEnterTimeCheck = false;

	return 0;
}

//------------------------------------------------------------------------------------
int SPLobby::OnCharacterCreateBack(WPARAM wParam, LPARAM lParam)
{
	
	SPWindow* pkLobbyRootWindow = g_pInterfaceManager->GetLobbyWindow();
	if( pkLobbyRootWindow )
	{
		SPWindowLobby* pkWindow = (SPWindowLobby*)pkLobbyRootWindow->Find(WIID_LOBBY);
		if( pkWindow == NULL )
			return 0;

		pkWindow->ShowLobbyType(1);
		GetCharacterListRequest();
	}

	return 0;
}

//------------------------------------------------------------------------------------
// Network cs
void SPLobby::GetWorldListInfoRequest()
{
	ShowNoticeBox(true, NOTICE_TYPE_MSG, g_pResourceManager->GetGlobalString(80001)) ;		//> Request World List

	CPacket kPacket;
	kPacket.AddUINT32(WORLD_CS_GETLIST);
	kPacket.AddUINT32(g_pNetworkManager->GetUserID());

	(g_pNetworkManager->GetPacketHandler())->PostMsg(&kPacket);

	if( !g_bForceLobbyServer )
	{
		g_szLobbyServerIP[0] = '\0';
		g_iLobbyServerPort = 0;
	}
}

//------------------------------------------------------------------------------------
void SPLobby::GetCharacterListRequest()
{
	if(g_pNetworkManager)
	{
		CPacket kPacket;
		kPacket.AddUINT32(CHAR_CS_GETLIST);
		kPacket.AddUINT32(g_pNetworkManager->GetUserID());
		kPacket.AddUINT16(g_pNetworkManager->GetWorldID());

		TCHAR szID[MAX_LEN_ID] = {0,};
		int nID = min(MAX_LEN_ID,_tcslen(g_pNetworkManager->GetLoginID()));
		_tcsncpy(szID,g_pNetworkManager->GetLoginID(),nID);
		kPacket.AddData(szID,MAX_LEN_ID);

		(g_pNetworkManager->GetPacketHandler())->PostMsg(&kPacket);
	}
}

//------------------------------------------------------------------------------------
int SPLobby::OnGameServerConnectRequest(WPARAM wParam, LPARAM lParam)//连接相应的服务器和频道
{
	int iServerIndex = (int)wParam;
	int iChannelIndex = (int)lParam;

	if( iServerIndex >= (int)m_vWorldInfo.size() ||
		iChannelIndex >= (int)m_vChannelInfo.size() )
		return 0;
	
	if( m_bOnceSendPacket )
		return 0;
	m_IGWServerID = m_vWorldInfo.at( iServerIndex ).wid;
	m_IGWServerName = m_vWorldInfo.at( iServerIndex ).name;
	//ChangeLocation IGW 改变房间,并可以使用快捷方式进去房间,休闲游戏中,如疯狂赛车
	//if ( g_pCIGWLoaderDx9 )
	//{	
	//	
	//	m_IGWServerID = m_vWorldInfo.at( iServerIndex ).wid;
	//	m_IGWServerName = m_vWorldInfo.at( iServerIndex ).name;
	//	
	//	UINT16 ChannelID = m_vChannelInfo.at( iChannelIndex ).channelID;
	//	TCHAR Room[20];
	//	sprintf( Room, "%d", ChannelID % 100 );
	//	
	//	TCHAR Path[20];
	//	sprintf( Path, "%d", ChannelID / 100 );
	//	
	//	TCHAR PathTitle[20];
	//	sprintf( PathTitle, "%d服", ChannelID / 100 );
	//	
	//	string RoomTitle = m_vChannelInfo.at( iChannelIndex ).name;
	//	UserLocationA UserLocation = { Path, PathTitle, Room, RoomTitle.c_str()	};
	//	g_pCIGWLoaderDx9->ChangeLocationA( &UserLocation );
	//}
	ShowNoticeBox(true, NOTICE_TYPE_MSG, g_pResourceManager->GetGlobalString(80003)) ;		//> Request Lobby Connect
	//ShowNoticeBox(true, NOTICE_TYPE_MSG, "###############################################################################################################################################################################################################################################################***************************************************************************************************************************************************************************************************************************************************************") ;		//> Request Lobby Connect

	CPacket kPacket;
	kPacket.AddUINT32(CHANNEL_CS_SELECT);
	kPacket.Add((WORLD_ID)m_vWorldInfo[iServerIndex].wid);
	kPacket.Add((SERVER_ID)m_vChannelInfo[iChannelIndex].channelID);
	kPacket.Add((USERID)g_pNetworkManager->GetUserID());
	kPacket.Add((PASSPORT)g_pNetworkManager->GetPassport());

	(g_pNetworkManager->GetPacketHandler())->PostMsg(&kPacket);
	m_iGameServerID = m_vChannelInfo[iChannelIndex].channelID;
	m_bOnceSendPacket = true;
	return 0;
}

//------------------------------------------------------------------------------------
int SPLobby::OnChannelListInfoRequest(WPARAM wParam, LPARAM lParam)
{
	if( (int)wParam >= (int)m_vWorldInfo.size() )
		return 0;

	int index = (int)wParam;
	if(index < 0 && index > (int)m_vWorldInfo.size())
	{
		assert(false && "Invalid WorldID");
		return 0;
	}

	ShowNoticeBox(true, NOTICE_TYPE_MSG, g_pResourceManager->GetGlobalString(80002)) ;		//> Request Channel List
	m_pMsgTimer->Start(0, 0.8f) ;		// Dlg Timmer
	m_bChannelMsgClose = false ;

	// 肺厚(岿靛) 辑滚 沥焊 历厘
	UINT32 worldID = m_vWorldInfo[index].wid;
	g_pNetworkManager->SetWorldID(worldID);
	g_pNetworkManager->SetLobbyServerInfo(m_vWorldInfo[index].ip,m_vWorldInfo[index].port);

	CPacket kPacket;
	kPacket.AddUINT32(WORLD_CS_CHANNELSTATUS);
	kPacket.AddUINT32(worldID);

	(g_pNetworkManager->GetPacketHandler())->PostMsg(&kPacket);
	m_iCurServer = (int)wParam;

	return 0;
}

//------------------------------------------------------------------------------------
int SPLobby::OnCharacterCreateRequest(WPARAM wParam, LPARAM lParam)
{
	CHARACTER_INFO2* pCharInfo = (CHARACTER_INFO2*)wParam;
	CHAR_FIGURE* pFiqureInfo = (CHAR_FIGURE*)lParam;

	CPacket kPacket;
	kPacket.AddUINT32(CHAR_CS_CREATE);
	WORLD_ID worldID = g_pNetworkManager->GetWorldID();
	kPacket.AddUINT16(worldID);
	kPacket.Add(pCharInfo, sizeof(CHARACTER_INFO2));
	kPacket.Add(pFiqureInfo, sizeof(CHAR_FIGURE));
	
	(g_pNetworkManager->GetPacketHandler())->PostMsg(&kPacket);
	return 0;
}

//------------------------------------------------------------------------------------
int SPLobby::OnCharacterNameCheckRequest(WPARAM wParam, LPARAM lParam)
{
	const char* pszName = (const char*)wParam;
	if( pszName && g_pNetworkManager)
	{
		TCHAR szCharName[50] = {0,};
		TCHAR szID[50] = {0,};
		strcpy(szCharName, pszName);
		strcpy(szID,g_pNetworkManager->GetLoginID());

		CPacket kPacket;
		kPacket.AddUINT32(CHAR_CS_CHECK_CHARNAME);
		WORLD_ID worldID = g_pNetworkManager->GetWorldID();
		kPacket.AddUINT16(worldID);
		kPacket.Add(szID, MAX_LEN_ID);
		kPacket.Add(szCharName, LEN_NAME);

		(g_pNetworkManager->GetPacketHandler())->PostMsg(&kPacket);
	}

	return 0;
}


//------------------------------------------------------------------------------------
// Network sc
int SPLobby::OnGetWorldListInfo(WPARAM wParam, LPARAM lParam)
{
	CPacket* pkPacket = (CPacket*)lParam;
	m_vWorldInfo.clear();
	
	unsigned char ucNum;
	int i;
	WORLD_INFO WorldInfo;

	pkPacket->Extract(&ucNum);
	for( i = 0; i < ucNum; i++ )
	{
		pkPacket->Extract(&WorldInfo, sizeof(WORLD_INFO));
		m_vWorldInfo.push_back(WorldInfo);
	}

	ShowNoticeBox(false, NOTICE_TYPE_MSG) ;			//> Close 80001

	//// sort
	//for( i = 0; i < ucNum; i++ )
	//{
	//	for( j = i + 1; j < ucNum; j++ )
	//	{
	//		bool bChange = false;
	//		if( m_vWorldInfo[i].nCharCount < m_vWorldInfo[j].nCharCount )
	//			bChange = true;
	//		else if( m_vWorldInfo[i].status < m_vWorldInfo[j].status ) 
	//			bChange = true;

	//		if( bChange )
	//		{
	//			TempInfo = m_vWorldInfo[j];
	//			m_vWorldInfo[j] = m_vWorldInfo[i];
	//			m_vWorldInfo[i] = TempInfo;
	//		}
	//	}
	//}
	
	SPWindow* pkLobbyRootWindow = g_pInterfaceManager->GetLobbyWindow();
	if( pkLobbyRootWindow )
	{
		SPWindowLobby* pkWindow = (SPWindowLobby*)pkLobbyRootWindow->Find(WIID_LOBBY);
		if( pkWindow == NULL )
			return 0;

		pkWindow->SetServerList(m_vWorldInfo);
	}

	return 0;
}

//------------------------------------------------------------------------------------
int SPLobby::OnGetChannelListInfo(WPARAM wParam, LPARAM lParam)
{
	CPacket* pkPacket = (CPacket*)lParam;
	m_vChannelInfo.clear();

	unsigned char ucNum;
	int i, j;
	CHANNEL_INFO ChannelInfo, TempInfo;

	pkPacket->Extract(&ucNum);
	for( i = 0; i < ucNum; i++ )
	{
		pkPacket->Extract(&ChannelInfo, sizeof(CHANNEL_INFO));
		m_vChannelInfo.push_back(ChannelInfo);
	}

	if( m_pMsgTimer->IsExpired() == true ) {
		ShowNoticeBox(false, NOTICE_TYPE_MSG) ;			//> Close 80002
		m_pMsgTimer->Clear() ;
	}
	else
		m_bChannelMsgClose = true ;

	// sort
	for( i = 0; i < ucNum; i++ )
	{
		for( j = i + 1; j < ucNum; j++ )
		{
			bool bChange = false;
			if( m_vChannelInfo[i].channelID > m_vChannelInfo[j].channelID )
				bChange = true;

			if( bChange )
			{
				TempInfo = m_vChannelInfo[j];
				m_vChannelInfo[j] = m_vChannelInfo[i];
				m_vChannelInfo[i] = TempInfo;
			}
		}
	}

	SPWindow* pkLobbyRootWindow = g_pInterfaceManager->GetLobbyWindow();
	if( pkLobbyRootWindow )
	{
		SPWindowLobby* pkWindow = (SPWindowLobby*)pkLobbyRootWindow->Find(WIID_LOBBY);
		if( pkWindow == NULL )
			return 0;

		if( m_iCurServer >= 0 )
			pkWindow->SetChannelList(&m_vWorldInfo[m_iCurServer], m_vChannelInfo);
	}

	return 0;
}


//------------------------------------------------------------------------------------
// CHANNEL_SC_SELECT(GameServerIP,port) ==> disconnect(loginServer) ==> connect(LobbyServerIP,port) ==> CHAR_CS_GETLIST
int SPLobby::OnChannelSelect(WPARAM wParam, LPARAM lParam)
{
	CPacket* pkPacket = (CPacket*)lParam;

	RESULTCODE result;
	pkPacket->Extract(&result);

	SPWindow* pkLobbyRootWindow = g_pInterfaceManager->GetLobbyWindow();
	SPWindowLobby* pkWindow = (SPWindowLobby*)pkLobbyRootWindow->Find(WIID_LOBBY);
	
	if( result == LT_NO_ERROR )
	{
		// 盲澄 立加 沥焊(ip,port)甫 历厘. 瞒饶 霸烙 辑滚肺 立加且 锭 荤侩凳.
		//{
			TCHAR szGameServerIP[LEN_IP+1] = {0,};
			UINT16 port = 0;

			pkPacket->Extract(&szGameServerIP, sizeof(TCHAR)*LEN_IP);
			pkPacket->Extract(&port);

			g_pNetworkManager->SetGameServerInfo(szGameServerIP,port);
		//}


		// 某腐磐 包府甫 困秦 肺厚辑滚肺 立加. 快急鉴困 : 角青牢磊 > 促款罐篮 肺厚辑滚 沥焊
		if( !g_bForceLobbyServer )
		{
			if(g_szLobbyServerIP[0] == '\0')
			{
				strcpy(g_szLobbyServerIP,g_pNetworkManager->GetLobbyServerIP().c_str());
				g_iLobbyServerPort = g_pNetworkManager->GetLobbyServerPort();
			}
		}

		g_pNetworkManager->SetServerID(m_vWorldInfo[m_iCurServer].usLobbySID);
		g_pNetworkManager->UserMoveRequest();
		m_iConnectType = 2;
	}
	else
	{
		switch(result)
		{
		case LT_ERROR_USERFULL:
			ShowNoticeBox(true, NOTICE_TYPE_YES, g_pResourceManager->GetGlobalString(50001)) ;	///> Server FULL
			break;
		default:
			ShowNoticeBox(true, NOTICE_TYPE_YES, g_pResourceManager->GetGlobalString(70000)) ;	///> Unknown Error
			break;
		}
		m_bOnceSendPacket = false;
	}

	return 0;
}

//------------------------------------------------------------------------------------
// 昏力 抗沥 2005/5/2 by hoshim
int SPLobby::OnGameServerConnect(WPARAM wParam, LPARAM lParam)
{
	//CPacket* pkPacket = (CPacket*)lParam;
	//
	//RESULTCODE result;
	//pkPacket->Extract(&result);

	//if( result == LT_NO_ERROR )
	//{
	//	SPWindow* pkLobbyRootWindow = g_pInterfaceManager->GetLobbyWindow();
	//	if( pkLobbyRootWindow )
	//	{
	//		SPWindowLobby* pkWindow = (SPWindowLobby*)pkLobbyRootWindow->Find(WIID_LOBBY);
	//		if( pkWindow == NULL )
	//			return 0;

	//		pkWindow->ShowLobbyType(1);
	//		GetCharacterListRequest();
	//	}
	//}
	//else
	//{
	//}

	return 0;
}

//------------------------------------------------------------------------------------
int SPLobby::OnGetCharacterListInfo(WPARAM wParam, LPARAM lParam)
{
	m_vCharacterInfo.clear();
	ZeroMemory(m_bPrologueShow, sizeof(bool) * 3);
	map< unsigned int, vector< CONTAINER_ITEM > >::iterator iterItem = m_mCharContainerItem.begin();
	while( iterItem != m_mCharContainerItem.end() )
	{
		iterItem->second.clear();
		++iterItem;
	}
	m_mCharContainerItem.clear();

	ShowNoticeBox(false, NOTICE_TYPE_MSG) ;			//> Close 80003

	CPacket* pkPacket = (CPacket*)lParam;
	
	RESULTCODE result;
	pkPacket->Extract(&result);
	
	if( result == LT_NO_ERROR )
	{
		unsigned char ucNum;
		CHARACTER_INFO2 CharInfo;
		pkPacket->Extract(&ucNum);

		int i, j, k;
		unsigned short usItemNum;
		CONTAINER_ITEM ItemInfo;

		vector< CONTAINER_ITEM > vContainerItem, vEquipItem, vFashionItem, vBasicItem, vEffectItem;
		for( i = 0; i < ucNum; i++ )
		{
			pkPacket->Extract(&CharInfo, sizeof(CHARACTER_INFO2));
			pkPacket->Extract(&usItemNum);

			for( j = 0; j < usItemNum; j++ )
			{
				pkPacket->Extract(&ItemInfo, sizeof(CONTAINER_ITEM));
				if( ItemInfo.SlotInfo.ContainerType == FIGURE_EQUIP )
					vEquipItem.push_back(ItemInfo);
				else if( ItemInfo.SlotInfo.ContainerType == FIGURE_FASHION )
					vFashionItem.push_back(ItemInfo);
				else if (ItemInfo.SlotInfo.ContainerType == FIGURE_EFFECT)
					vEffectItem.push_back( ItemInfo );	
				else
					vBasicItem.push_back(ItemInfo);
			}

			// 2006.05.11 dhpark 某腐积己饶 弥檬柳涝咯何
			UINT8 byNewbi = 0;
			pkPacket->Extract(&byNewbi);
			if( byNewbi && i < 3 )
				m_bPrologueShow[i] = true;

			for( k = 0; k < (int)vBasicItem.size(); k++ )
				vContainerItem.push_back(vBasicItem[k]);
			for( k = 0; k < (int)vEquipItem.size(); k++ )
				vContainerItem.push_back(vEquipItem[k]);
			for( k = 0; k < (int)vFashionItem.size(); k++ )
				vContainerItem.push_back(vFashionItem[k]);
			for( k = 0; k < (int)vEffectItem.size(); ++k)
				vContainerItem.push_back(vEffectItem[k]);

			m_vCharacterInfo.push_back(CharInfo);
			m_mCharContainerItem.insert(make_pair(i, vContainerItem));

			vContainerItem.clear();
			vEquipItem.clear();
			vFashionItem.clear();
			vBasicItem.clear();
		}

		SPWindow* pkLobbyRootWindow = g_pInterfaceManager->GetLobbyWindow();
		if( pkLobbyRootWindow )
		{
			SPWindowLobby* pkWindow = (SPWindowLobby*)pkLobbyRootWindow->Find(WIID_LOBBY);
			if( pkWindow == NULL )
				return 0;

			pkWindow->SetCharacterList(m_vCharacterInfo, m_mCharContainerItem);
		}
	}
	else
	{
	}

	return 0;
}

//------------------------------------------------------------------------------------
int SPLobby::OnCharacterNameCheck(WPARAM wParam, LPARAM lParam)
{
	CPacket* pkPacket = (CPacket*)lParam;

	RESULTCODE result;
	pkPacket->Extract(&result);

	SPWindow* pkLobbyRootWindow = g_pInterfaceManager->GetLobbyWindow();
	if( pkLobbyRootWindow == NULL )
		return 0;

	SPWindowLobby* pkWindow = (SPWindowLobby*)pkLobbyRootWindow->Find(WIID_LOBBY);
	if( pkWindow == NULL )
		return 0;

	bool bSuccess;
	if( result == LT_NO_ERROR )
		bSuccess = true;
	else
		bSuccess = false;
	
	pkWindow->SetCharacterNameCheck(bSuccess);
	return 0;
}

//------------------------------------------------------------------------------------
int SPLobby::OnCharacterCreate(WPARAM wParam, LPARAM lParam)
{
	CPacket* pkPacket = (CPacket*)lParam;
	RESULTCODE result;
	pkPacket->Extract(&result);

	SPWindow* pkLobbyRootWindow = g_pInterfaceManager->GetLobbyWindow();
	SPWindowLobby* pkWindow = (SPWindowLobby*)pkLobbyRootWindow->Find(WIID_LOBBY);

	ShowNoticeBox(false, NOTICE_TYPE_MSG) ;			//> Close 40000

	if( result == LT_NO_ERROR )
	{
		//pkWindow->SetInfomationDialog("", false);
		OnCharacterCreateBack(0, 0);
	}
	else
	{
		switch( result )
		{
		case LT_ERROR_CHARNAME_RESERVED:
			ShowNoticeBox(true, NOTICE_TYPE_YES, g_pResourceManager->GetGlobalString(40002)) ;		///> Invalid Name
			break;
		case LT_ERROR_CHARNAME_USED:	// 捞固 荤侩吝牢 捞抚涝聪促.
			ShowNoticeBox(true, NOTICE_TYPE_YES, g_pResourceManager->GetGlobalString(40002)) ;		///> Already Used Name
			break;
		case LT_ERROR_LEN_CHARNAME:
			ShowNoticeBox(true, NOTICE_TYPE_YES, g_pResourceManager->GetGlobalString(40002)) ;		///> Character Name Length Invalid
			break;
		case LT_ERROR_INVALID_CHARACTER:
			ShowNoticeBox(true, NOTICE_TYPE_YES, g_pResourceManager->GetGlobalString(40002)) ;		///> Character Name Illegal
			break;
		default:
			ShowNoticeBox(true, NOTICE_TYPE_YES, g_pResourceManager->GetGlobalString(70000)) ;		///> Unknown Error
			break;
		}
	}
	
	return 0;
}

//------------------------------------------------------------------------------------
int SPLobby::OnCharacterDelete(WPARAM wParam, LPARAM lParam)
{
	CPacket* pkPacket = (CPacket*)lParam;

	GLOBAL_STRING_ID iError; 
	pkPacket->Extract(&iError);

	SPWindow* pkLobbyRootWindow = g_pInterfaceManager->GetLobbyWindow();
	SPWindowLobby* pkWindow = (SPWindowLobby*)pkLobbyRootWindow->Find(WIID_LOBBY);

	

	if( iError == 0 )
	{
		ShowNoticeBox(false, NOTICE_TYPE_MSG);			//> Close 60010
	}
	else
	{
		string strError;
		const char* pszString = g_pResourceManager->GetGlobalString(iError);
		if( pszString )
			strError = pszString;
		
		if( strError.empty() )
			ShowNoticeBox(true, NOTICE_TYPE_YES, g_pResourceManager->GetGlobalString(70000)) ;

		ShowNoticeBox(true, NOTICE_TYPE_YES, pszString) ;
		return 0;
	}
		
	GetCharacterListRequest();
	return 0;
}

//------------------------------------------------------------------------------------
int SPLobby::OnCharacterSelect(WPARAM wParam, LPARAM lParam)//选择人物进入游戏
{
	RESULTCODE result = (RESULTCODE)wParam;
	if( result == LT_NO_ERROR )
		g_pGOBManager->SPPerformMessage(SPGM_CHAR_SC_SELECT, wParam, lParam );
	else
	{
		SPWindow* pkLobbyRootWindow = g_pInterfaceManager->GetLobbyWindow();
		if( pkLobbyRootWindow )
		{
			SPWindowLobby* pkWindow = (SPWindowLobby*)pkLobbyRootWindow->Find(WIID_LOBBY);
			if( pkWindow == NULL )
				return 0;

			ShowNoticeBox(true, NOTICE_TYPE_YES, g_pResourceManager->GetGlobalString(30007));		///> Connect Fail
			pkWindow->EnableChildWindow(1, true);
		}
	}

	return 0;
}

//------------------------------------------------------------------------------------
int SPLobby::OnWorldEnter(WPARAM wParam, LPARAM lParam)//进入游戏世界
{
	CPacket* pkPacket = (CPacket*)lParam;
	RESULTCODE resultCode;
	pkPacket->ExtractUINT8(&resultCode);

	m_fGameServerConnectWaitTime = 0.0f;
	m_iConnectType = 0;
	if( resultCode == LT_NO_ERROR )
	{
		m_iGameState = GS_RELEASE_READY;	
		m_NextSubGameID	= SUBGAME_LOADING;
		m_fWaitTime = 2.0f;

		//g_pNetworkManager->GameChannel_IN();
		g_pNetworkManager->SetChannelEnteringFlag(true);
		if ( g_pCIGWLoaderDx9 )	{
			
			if(g_pGOBManager->GetLocalPlayer()) {
		//玩家选择游戏角色进入游戏,设置角色的社群信息
			
				SPPlayerStatus* pStatus = static_cast<SPPlayerStatus*>( g_pGOBManager->GetLocalPlayer()->GetStatus() );
				std::string strName = pStatus->GetGOBName();
				int iSex = static_cast<int>( pStatus->GetStatusValue( STATUS_GENDER ) );
				int iClassType = pStatus->GetStatusValue( STATUS_CLASS_TYPE );
				std::string Occupation = "";
				if ( g_pResourceManager->GetGlobalString( iClassType + 20000 ) )
				{
					Occupation = g_pResourceManager->GetGlobalString( iClassType + 20000 );
				}
				int iLevel = pStatus->GetLevel();

				std::string GuildName = "";
				int GuildGrade = 0;
				if ( g_pGOBManager->GetGuildArchive() )
				{
					if ( g_pGOBManager->GetGuildArchive()->GetGuildName() )
					{
						GuildName = g_pGOBManager->GetGuildArchive()->GetGuildName();
					}

					if ( g_pGOBManager->GetGuildArchive()->GetGuildName() )
					{
						GuildGrade = g_pGOBManager->GetGuildArchive()->GetGuildGrade();
					}
				}
				TCHAR szGuildGrade[10];
				sprintf( szGuildGrade, "%d", GuildGrade );

				UserCharacterInfoA CharInfo = { strName.c_str(),
					iSex, 
					Occupation.c_str(),
					iLevel, 
					GuildName.c_str(),
					szGuildGrade	
				};
				int     GameAreaID = -1;                // 盛大统一游戏区号ID（默认-1）      // 2008.3.5 增加游戏区服数字ID信息
				int     GameServerID = -1; 
				if ( g_pNetworkManager )
				{
					GameAreaID = g_pNetworkManager->GetLoginServerID();
					GameServerID = g_pNetworkManager->GetWorldID();
				}
			
				TCHAR szp[128];
				::ZeroMemory( szp, sizeof(szp) );
				sprintf( szp, "%d,%d,%d,%d",
							g_pNetworkManager->GetWorldID(),
											g_pNetworkManager->GetServerID(), 
											g_pNetworkManager->GetGameServerID(),
											g_pNetworkManager->GetPreviousServerID());
				//DXUTOutputDebugString( szp );
				//玩家选择服务器并进入,改变用户信息,目前只有服务器名可用
				UserDetailInfoA UserInfo = { sizeof( UserDetailInfoA ),
					strName.c_str(),
					iSex,
					NULL,
					m_IGWServerName.c_str(),
					GameAreaID,
					GameServerID
				};
				g_pCIGWLoaderDx9->ChangeUserInfoA( &UserInfo );

				g_pCIGWLoaderDx9->SetCommunityInfoA( &CharInfo );
			}
		}
	}
	else
	{
		switch( resultCode )
		{
		case LT_ERROR_DUPLICATED:
			ShowNoticeBox(true, NOTICE_TYPE_YES, g_pResourceManager->GetGlobalString(30003));		///> Duplicate User (Already Using)
			break;
		case LT_ERROR_USERFULL:
			ShowNoticeBox(true, NOTICE_TYPE_YES, g_pResourceManager->GetGlobalString(50001));		///> Server Full
			break;
		default:
			ShowNoticeBox(true, NOTICE_TYPE_YES, g_pResourceManager->GetGlobalString(70000));		///> Unknown Error
			break;

		}

		SPWindow* pkLobbyRootWindow = g_pInterfaceManager->GetLobbyWindow();
		SPWindowLobby* pkWindow = (SPWindowLobby*)pkLobbyRootWindow->Find(WIID_LOBBY);

		if( pkWindow )
		{
			pkWindow->EnableChildWindow(1, true);
		}
	}

	return 0;
}

//------------------------------------------------------------------------------------
int SPLobby::OnUserIn(WPARAM wParam, LPARAM lParam)//场景的切换
{
	CPacket* packet = (CPacket*)lParam;

	GLOBAL_STRING_ID	uiErrorID;
	PASSPORT			uiPassport;

	packet->Extract(&uiErrorID);
	if( uiErrorID == 0 )
	{
		packet->Extract(&uiPassport);
		g_pNetworkManager->SetInPassport(uiPassport);

		switch( m_iConnectType )
		{
		case 1:
			{
				if( g_ExcutionMode == EMODE_NORMAL )
					g_pNetworkManager->SetCharName(m_vCharacterInfo[m_iSelectCard].BaseInfo.szName);

				g_pNetworkManager->SetIndunID(0);
				g_pNetworkManager->EnterWorld();
			}
			break;

		case 2:
			{
				SPWindow* pkLobbyRootWindow = g_pInterfaceManager->GetLobbyWindow();
				SPWindowLobby* pkWindow = NULL;
				if( pkLobbyRootWindow )
					pkWindow = (SPWindowLobby*)pkLobbyRootWindow->Find(WIID_LOBBY);

				pkWindow->ShowLobbyType(1);
				GetCharacterListRequest();

				InitEnterTime();
				m_bEnterTimeCheck = true;
			}
			break;

		case 3:
			{
				ErrorFLogs->Error_Log("[SPLobby::OnUserIn] WorldList Request ") ;
				GetWorldListInfoRequest();
			}
			break;
		}
	}
	else
	{
		ShowNoticeBox(false, NOTICE_TYPE_MSG) ;
		SPPerformMessage(SPGM_SUG_SOCKET_FAILED, uiErrorID, 0);
	}

	m_bOnceSendPacket = false;
	return 0;
}

//------------------------------------------------------------------------------------
int SPLobby::OnUserMove(WPARAM wParam, LPARAM lParam)//场景的切换
{
	CPacket* packet = (CPacket*)lParam;

	GLOBAL_STRING_ID	uiErrorID;
	packet->Extract(&uiErrorID);

	if( uiErrorID == 0 )
	{
		switch( m_iConnectType )
		{
		case 1:
			g_pNetworkManager->Disconnect2();
			m_fWaitTime = 3.0f;
			m_iGameState = GS_WAIT;

			m_fGameServerConnectWaitTime = 0.0f;
			m_iGameServerConnectType = 0;
			break;
		
		case 2:
			g_pNetworkManager->Disconnect2();
			m_fWaitTime = 3.0f;
			m_iGameState = GS_WAIT;
			break;

		case 3:
			g_pNetworkManager->Disconnect2();
			m_fWaitTime = 3.0f;
			m_iGameState = GS_WAIT;
			break;
		}
		
	}
	else
	{
		ShowNoticeBox(false, NOTICE_TYPE_MSG) ;
		SPPerformMessage(SPGM_SUG_SOCKET_FAILED, uiErrorID, 0);
	}

	return 0;
}

/**
	For Send ServerList to SPWindowLobby
	@param wParam : Current Top Scroll Index
*/
int SPLobby::OnWorldListScroll(WPARAM wParam, LPARAM lParam)
{
	int iPage = (int)wParam ;

	SPWindow* pkLobbyRootWindow = g_pInterfaceManager->GetLobbyWindow();
	if( pkLobbyRootWindow )
	{
		SPWindowLobby* pkWindow = (SPWindowLobby*)pkLobbyRootWindow->Find(WIID_LOBBY);
		if( pkWindow == NULL )
			return 0;

		pkWindow->UpdateServerList(iPage, m_vWorldInfo) ;
	}
	return 1 ;
}

//------------------------------------------------------------------------------------
// Network Message
SPGMESSAGE_MAP_BEGIN(SPLobby)

SPGMESSAGE_COMMAND(SPGM_SERVER_BACK				,	OnServerBack);
SPGMESSAGE_COMMAND(SPGM_CHAR_MENU_SELECT		,	OnCharacterMenuSelect);
SPGMESSAGE_COMMAND(SPGM_CHAR_MENU_CREATE		,	OnCharacterMenuCreate);
SPGMESSAGE_COMMAND(SPGM_CHAR_MENU_DELETE		,	OnCharacterMenuDelete);
SPGMESSAGE_COMMAND(SPGM_CHAR_BACK				,	OnCharacterBack);

SPGMESSAGE_COMMAND(SPGM_CHAR_CREATE_NAME_CHECK_REQ	,	OnCharacterNameCheckRequest);
SPGMESSAGE_COMMAND(SPGM_CHAR_CREATE_REQ				,	OnCharacterCreateRequest);
SPGMESSAGE_COMMAND(SPGM_CHAR_CREATE_BACK			,	OnCharacterCreateBack);

SPGMESSAGE_COMMAND(SPGM_GAMESERVER_CONNECT_REQ	,	OnGameServerConnectRequest);	// Lobby Server Connect
SPGMESSAGE_COMMAND(SPGM_CHANNEL_LIST_REQ		,	OnChannelListInfoRequest);		// Channel List Request

SPGMESSAGE_COMMAND(SPGM_WORLD_SC_GETLIST		,	OnGetWorldListInfo);		// WorldList
SPGMESSAGE_COMMAND(SPGM_SERVER_LIST_SCROLL		,	OnWorldListScroll);		// WorldList Scroll -> m_vWorldInfo
SPGMESSAGE_COMMAND(SPGM_WORLD_SC_CHANNELSTATUS	,	OnGetChannelListInfo);		// Channel Status
//SPGMESSAGE_COMMAND(SPGM_CHANNEL_SC_SELECT		,	OnGameServerConnect);
SPGMESSAGE_COMMAND(SPGM_CHANNEL_SC_SELECT		,	OnChannelSelect);			// Channel Select
SPGMESSAGE_COMMAND(SPGM_CHAR_SC_GETLIST			,	OnGetCharacterListInfo);
SPGMESSAGE_COMMAND(SPGM_CHAR_SC_CHECK_CHARNAME	,	OnCharacterNameCheck);
SPGMESSAGE_COMMAND(SPGM_CHAR_SC_CREATE			,	OnCharacterCreate);
SPGMESSAGE_COMMAND(SPGM_CHAR_SC_DELETE			,	OnCharacterDelete);
SPGMESSAGE_COMMAND(SPGM_CHAR_SC_SELECT			,	OnCharacterSelect);

SPGMESSAGE_COMMAND(SPGM_WORLD_SC_ENTER			,	OnWorldEnter);

SPGMESSAGE_MAP_END(SPSubGameManager)


/**
	Modal Notice Box Display
	@param bool bShow : be Show (true, false)
	@param NOTICE_TYPE : define SPNotice.h
	@param TCHAR* szMsg : For Display Text
*/
bool SPLobby::ShowNoticeBox(bool bShow, NOTICE_TYPE iType, const TCHAR* szMsg)
{
	//////////////////////////////////////////////////////////////////////////
	// New Version
	SPWindow* pkLobbyRootWindow = g_pInterfaceManager->GetLobbyWindow();
	SPWindowLobby* pkWindow = (SPWindowLobby*)pkLobbyRootWindow->Find(WIID_LOBBY);

	return pkWindow->SetInfomationDialog(bShow, iType, szMsg) ;
}

//
//void SPLobby::PrintLogs(const TCHAR* fmt, ...)
//{
//	try
//	{
//		char msg[512];
//		va_list va_ptr;
//
//		va_start(va_ptr, fmt);
//		vsprintf(msg, fmt, va_ptr);
//		va_end(va_ptr);
//
//		ErrorFLogs->Error_Log(msg) ;
//	}catch (...) 
//	{
//	}
//}
