
#include "SPCommon.h"
#include "SPUtil.h"
#include "SPResourceDEF.h"
#include "SPMouseCursor.h"
#include "SPManager.h"
#include "SPTerrainDEF.h"
#include "SPTerrainCluster.h"
#include "SPTerrainManager.h"

#include "SPSubGameManager.h"
#include "SPLoading.h"

#include "PacketID.h"			//[2005/12/10]
#include "Packet.h"
#include "SPNetworkManager.h"

#include "SPStage.h"
#include "SPStageManager.h"
#include "SPStageLoader.h"

#include "SPMouseCursor.h"	//  [2005/4/29]
#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPGameObjectDEF.h"
#include "SPGameObject.h"
#include "SPGOBCoordPhysics.h"
#include "SPGOBStatus.h"
#include "SPGOBClusterDef.h"
#include "SPPlayerStatusModel.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGOBManager.h"
#include "SPPlayer.h"

#include "SPEventDEF.h"
#include "SPEvent.h"
#include "SPEventManager.h"

#include "SPCashTradeManager.h"

#include "SPMOBCluster.h"
#include "SPLocalizeManager.h"

#include "SPPvpManager.h"
#include "_Error_log.h"

#ifdef _DEBUG
#include <iostream>			//jinhee - [2005/1/3] //테스트를 위한...
#include <fstream>
#endif

//std::vector<SPStage*>	g_vpStageList;
//extern SPStageManager g_StageManager;

DWORD WINAPI LoadingThread(LPVOID lParam)
{
	SPLoading* pkLoading = (SPLoading*)lParam;

	int iStartTime = timeGetTime();
	int iProcessTime;
	while( pkLoading->m_bSendKeepAlive )
	{
		iProcessTime = timeGetTime();
		g_pNetworkManager->SendProcess((float)(iProcessTime - iStartTime) / 1000.0f);
		iStartTime = iProcessTime;

		Sleep(10);
	}
	
	pkLoading->m_bThreadExit = true;
	return 0;
}

SPLoading::SPLoading()
: SPSubGameManager(SUBGAME_LOADING)
, m_pImageBack(NULL)
, m_pImageGauge(NULL)
, m_LoadState(LS_NULL)
, m_fLoadProtage(0)
, m_fProtage(0)
, m_fLastRenderDelay(0)
, m_pLDTLoadData(NULL)
, m_pLoadInfo(NULL)
{
}

SPLoading::~SPLoading() 
{
	SAFE_RELEASE(m_pImageBack);
	SAFE_RELEASE(m_pImageGauge);
	SAFE_RELEASE(m_pImageWrap);

	for(int i = 0; i < MAX_IMAGE_LIST; i++) {
		SAFE_RELEASE(m_pImageLoad[i]);
	}

	SAFE_DELETE(m_pLoadInfo);
	SAFE_RELEASE(m_pLDTLoadData);
}

bool SPLoading::Init()
{
	m_pLDTLoadData = NULL;
	bool bResult = g_pResourceManager->GetLDTFromFileName(RES_FNAME_LDT_LOAD, &m_pLDTLoadData);

	m_pLoadInfo = new SPLoadInfo;

	for(int i = 0; i < MAX_IMAGE_LIST; i++) {
		m_pImageLoad[i] = NULL;
	}

	//기본...
	g_pVideo->GetTextureMgr()->LoadTexture("DATA/LOGGIN/LOADING.PNG", &m_pImageBack);

	g_pVideo->GetTextureMgr()->LoadTexture("DATA/LOGGIN/LOADGAUGE.PNG", &m_pImageGauge);

	g_pVideo->GetTextureMgr()->LoadTexture("DATA/LOGGIN/GAUGEBAR.PNG", &m_pImageWrap);


	//gauge.png (166, 528) - 가로로 확대 (431px까지 확대 하면 100%가 됩니다)
	m_RectGauge.top	    = 528;
	m_RectGauge.bottom  = m_RectGauge.top + 32;
	m_RectGauge.left	= 166;
	m_RectGauge.right	= m_RectGauge.left;


	m_RectBack.top = m_RectBack.left = 0;
	m_RectBack.right = g_pVideo->GetScreenLenX();
	m_RectBack.bottom = g_pVideo->GetScreenLenY();

	m_RectLog.left		= 0;
	m_RectLog.right		= g_pVideo->GetScreenLenX();
	m_RectLog.top		= g_pVideo->GetScreenLenY() / 2 - 20;
	m_RectLog.bottom	= g_pVideo->GetScreenLenY() / 2 + 20;
	
	SetRect(&m_rtSrc1, 0, 0, 512, 512);
	SetRect(&m_rtSrc2, 512, 0, 800, 512);
	SetRect(&m_rtSrc3, 800, 0, 1024, 88);
	SetRect(&m_rtSrc4, 800, 88, 1024, 176);
	SetRect(&m_rtSrc5, 800, 176, 1024, 264);
	SetRect(&m_rtSrc6, 800, 264, 928, 352);

	SetRect(&m_rtDest1, 0, 0, 512, 512);
	SetRect(&m_rtDest2, 512, 0, 800, 512);
	SetRect(&m_rtDest3, 0, 512, 224, 600);
	SetRect(&m_rtDest4, 224, 512, 448, 600);
	SetRect(&m_rtDest5, 448, 512, 672, 600);
	SetRect(&m_rtDest6, 672, 512, 800, 600);

	m_fAccumulateRender = 0.0f;
	m_fCurDelay = 0.01f;					//게이지 올리는 속도 조절...

	InitLoadingThread();

	SPCashTradeManager::GetInstance()->ResetSocket();	
	return SPSubGameManager::Init();
}

void SPLoading::InitLoadingThread()
{
	DWORD dwID;
	
	m_bSendKeepAlive = true;
	m_bThreadExit = false;
	m_hThreadHandle = NULL;
	m_hThreadHandle = CreateThread(NULL, 0, LoadingThread, this, 0, &dwID);
}

/*
void SPLoading::Process(float fTime)
{
	switch(m_iGameState) {
	case GS_GAME:
		{
			if( m_bCompleteLoading )
			{
				m_iGameState = GS_RELEASE_READY;
				m_fWaitTime = 0.0f;

				if( m_hThreadHandle )
				{
					CloseHandle(m_hThreadHandle);
					m_hThreadHandle = NULL;
				}
			}
		}
		break;

	case GS_RELEASE_READY:
		{
			m_fWaitTime -= fTime;
			if(m_fWaitTime <= 0) {
				m_iGameState = GS_RELEASE;
				m_NextSubGameID	= SUBGAME_ZONE;

				if( g_pGOBManager )
					g_pGOBManager->SPPerformMessage(SPGM_EVENT_CS_STAGE_ENTER);
			}
		}
		break;
	}

	ProcessImage(fTime);
	SPSubGameManager::Process(fTime);
}
*/

void SPLoading::Process(float fTime)
{
	switch(m_iGameState) {
	case GS_GAME:
		{
			LOAD_STATE LoadStateNext = m_LoadState;	
			switch(m_LoadState) {
			case LS_NULL:
				{
					//ErrorFLogs->Error_Log("Current LoadState [LS_NULL]") ;
					//LoadStateNext = LS_BGI_LOADER;
					
					LoadStateNext = LS_STAGE_LOAD;		//jinhee - [2004/12/30] //Test
					m_LoadStateNext = LS_STAGE_LOAD;
					m_fLoadProtage = 0;
					m_fProtage = 0;
				}
				break;

			case LS_STAGE_LOAD:							//jinhee - [2004/12/30] //Test
				{	
					//ErrorFLogs->Error_Log("Current LoadState [LS_STAGE_LOAD]") ;
					SPStageLoader StageLoader;
					int iResult = 0;					
					//iResult = StageLoader.LoadStageDataText(&g_StageManager.m_vpStageList, RES_WORK_RESOURCE_PATH, RES_FNAME_STAGE);
					//iResult = StageLoader.LoadStageBinary(&g_StageManager.m_vpStageList, RES_WORK_RESOURCE_PATH, RES_FNAME_STAGE_BINARY);
					iResult = StageLoader.LoadStageBinary(&g_StageManager.m_vpStageList, "", RES_FNAME_STAGE_BINARY);					
					
					// 현재의 시작 맵, 그룹, 스테이지를 지정한다 //사용자 정보를 이용하여 설정해야 할것이다.
					//[2005/4/13] - 원래는 Loby에서 플레이어 스테이지를 받긴하는데...
					//스테이지 정보를 받는곳에서 LocalPlayer 처리가 되지 않아 문제 발생...
					//우선은 여기서 한번 세팅해주고 추후 수정되면 고칠것
					g_StageManager.SetCurMapIndex(0, m_iMapGroupID, m_iStageID);
					
					m_fLoadProtage = 10;

					////
					//ofstream file("./MapINfo.txt", std::ios::out | std::ios::app);
					//if(!file){
					//	return ;
					//}
					//file << "MAPCOUNT : " << StageLoader.GetMapCount() << endl;
					//LoadStateNext = LS_BGI_LOADER;
#if defined(_JUST_RUN)
					LoadStateNext = LS_RENDER;
					m_LoadStateNext = LS_TERRAIN_ATTR;
#else
					LoadStateNext = LS_RENDER;
					m_LoadStateNext = LS_BGI_LOADER;
#endif
					m_fAccumulateRender = 0.0f;
					m_fLastRenderDelay = fTime;
					fTime = 0.0f;
				}
				break;

			case LS_BGI_LOADER:
				{	
					//ErrorFLogs->Error_Log("Current LoadState [LS_BGI_LOADER]") ;
					//g_pTerrainManager->LoadBG(g_StageManager.GetCurStage());
					g_pTerrainManager->LoadBG(g_StageManager.GetCurStage(m_iStageID));
					
					m_fLoadProtage = 20;
					//LoadStateNext = LS_TERRAIN_PALLET;
					LoadStateNext = LS_RENDER;
					m_LoadStateNext = LS_TERRAIN_PALLET;
					m_fAccumulateRender = 0.0f;
					m_fLastRenderDelay = fTime;
					fTime = 0.0f;
				}
				break;

			case LS_TERRAIN_PALLET:
				{					
					//ErrorFLogs->Error_Log("Current LoadState [LS_TERRAIN_PALLET]") ;
					//g_pTerrainManager->SPPerformMessage(SPTM_LOADPALLET, (WPARAM)RES_FNAME_TERRAINPALLET);					
					if(g_StageManager.GetCurStage() != NULL)
					{
						if(!g_StageManager.GetCurStage()->m_vpPalette.empty()) {
							std::string filePath = "DATA/BGFORMAT/";							
							filePath += g_StageManager.GetCurStage(m_iStageID)->m_vpPalette[0] ;
							g_pTerrainManager->SPPerformMessage(SPTM_LOADPALLET, (WPARAM)filePath.c_str());
						}
					}
					
					m_fLoadProtage = 60;
					//LoadStateNext = LS_TERRAIN_ATTR;
					LoadStateNext = LS_RENDER;
					m_LoadStateNext = LS_TERRAIN_ATTR;
					m_fAccumulateRender = 0.0f;
					m_fLastRenderDelay = fTime;
					fTime = 0.0f;
				}
				break;

			case LS_TERRAIN_ATTR:
				{
					//ErrorFLogs->Error_Log("Current LoadState [LS_TERRAIN_ATTR]") ;
					SPStage* pStage = g_StageManager.GetCurStage(m_iStageID);
					if(!pStage)
						break;
					//g_pTerrainManager->SPPerformMessage(SPTM_LOADATTRIBUTE, (WPARAM)RES_FNAME_TERRAINATTRIBUTE);
					//g_pTerrainManager->SPPerformMessage(SPTM_LOADATTRIBUTE, (WPARAM)m_vpStageInfo[0]->strAttributeFile.c_str());//jinhee - [2005/1/4]	

					//g_pTerrainManager->LoadAttribute(g_StageManager.GetCurStage());
					g_pTerrainManager->LoadAttribute(pStage);
					
					m_fLoadProtage = 80;
					//LoadStateNext = LS_TERRAIN_FORM;
					LoadStateNext = LS_RENDER;

#if defined(_JUST_RUN)
					m_LoadStateNext = LS_END;
#else
					m_LoadStateNext = LS_TERRAIN_FORM;
#endif
					//[2005/4/29] Manager에 현재 스테이지 그룹의 크기를 지정해준다 

					SPMapGroup* pMapGroup = pStage->GetMapGroup(m_iMapGroupID);
					if(!pMapGroup) 
						break;
					g_pTerrainManager->SetLength(pMapGroup->m_lGroupWidth, pMapGroup->m_lGroupHeight);
					g_pInterfaceManager->SPPerformMessage(SPIM_SET_STAGE, (WPARAM)m_iStageID, (LPARAM)m_iMapGroupID);

					m_fAccumulateRender = 0.0f;
					m_fLastRenderDelay = fTime;
					fTime = 0.0f;
				}
				break;
			
			case LS_TERRAIN_FORM:
				{	
					//ErrorFLogs->Error_Log("Current LoadState [LS_TERRAIN_FORM]") ;
					//g_pTerrainManager->SPPerformMessage(SPTM_LOADFORM, (WPARAM)RES_FNAME_TERRAINFORM);	
					//g_pTerrainManager->SPPerformMessage(SPTM_LOADFORM, (WPARAM)m_vpStageInfo[0]->strFormFile.c_str());//jinhee - [2005/1/4]

					//g_pTerrainManager->LoadForm(g_StageManager.GetCurStage());
					g_pTerrainManager->LoadForm(g_StageManager.GetCurStage(m_iStageID));
					if( g_pMOBCluster )
						g_pMOBCluster->PreTextureLoading(m_iStageID);

					//m_fLoadProtage = 100;
					m_fLoadProtage = 98;
					//LoadStateNext = LS_END;
					LoadStateNext = LS_RENDER;
					//m_LoadStateNext = LS_END;
					m_LoadStateNext = LS_CASH_CHECK;
					if(!SPCashTradeManager::GetInstance()->IsSocketFail()) {
						SPCashTradeManager::GetInstance()->SendCashShopVersion();
					}
					m_fAccumulateRender = 0.0f;
					m_fLastRenderDelay = fTime;
					fTime = 0.0f;
				}
				break;	

			case LS_CASH_CHECK:				
				if(SPCashTradeManager::GetInstance()->IsMinorLoad() &&
					SPCashTradeManager::GetInstance()->IsHotListLoad()) {
					m_fLoadProtage = 100;
					LoadStateNext = LS_RENDER;
					m_LoadStateNext = LS_END;
					m_fAccumulateRender = 0.0f;
					m_fLastRenderDelay = fTime;
					fTime = 0.0f;
				}
				else {
					//OnSocketFailed((WPARAM)0, (LPARAM)0);
					g_pNetworkManager->Process(fTime);
					Render(fTime);
				}
				
				break;

			case LS_END:
				{
					////테스트 코드			//나중에 제가 지우겠습니다. 그냥 남겨주세요 - jinhee
					//#ifdef _DEBUG	
					//std::vector<SPStage*>::iterator iter;
					//ofstream file("./MapINfo.txt", std::ios::out | std::ios::app);
					//if(!file){
					//	return ;
					//}
					//file << "PATH : " << RES_WORK_RESOURCE_PATH << endl;
					//file << "FILE : " << RES_FNAME_STAGE << endl ;
					////file << "Stage Count : " << pStageLoader.GetStageCount() << endl;
					////file << "Stage Size : " << static_cast<int>(g_StageManager.m_vpStageList.size()) << endl;
					//
					//for(iter = g_StageManager.m_vpStageList.begin(); iter != g_StageManager.m_vpStageList.end(); ++iter) {
					//	file << endl << "Stage ID : " << (*iter)->m_iStageID << endl;
					//	file << "StageName : " << (*iter)->m_strStageName.c_str() << endl;
					//	file << "GroupCount : " << (*iter)->m_iGroupCount << endl;
					//	file << "SyncRegionWidth : " << (*iter)->m_iSyncRegionWidth << endl;
					//	file << "SyncRegionHeight : " << (*iter)->m_iSyncRegionHeight << endl;
					//	
					//	std::vector<std::string>::iterator iterPalette;
					//	for(iterPalette = (*iter)->m_vpPalette.begin(); iterPalette != (*iter)->m_vpPalette.end(); ++iterPalette){
					//		file << " Palette : " << (*iterPalette).c_str() << endl;
					//	}
					//
					//	std::vector<SPMapGroup*>::iterator iterMapGroup;
					//	iterMapGroup = (*iter)->m_vpMapGroup.begin();
					//	for(; iterMapGroup != (*iter)->m_vpMapGroup.end(); ++iterMapGroup){
					//		file << "\t" << "GroupID : " << (*iterMapGroup)->m_iGroupID << endl;
					//		file << "\t" << "GroupName : " << (*iterMapGroup)->m_strGroupName.c_str() << endl;
					//		file << "\t" << "MapLink : " << (*iterMapGroup)->m_iMapLink << endl;
					//		file << "\t" << "MapCount : " << (*iterMapGroup)->m_iMapCount << endl;
					//		file << "\t" << "BGID : " << (*iterMapGroup)->m_iBGID << endl;
					//		file << "\t" << "Type : " << (*iterMapGroup)->m_iType << endl;
					//		file << "\t" << "BGFile : " << (*iterMapGroup)->m_strBGFile.c_str() << endl;
					//		file << "\t" << "BGMFile : " << (*iterMapGroup)->m_strBGMFile.c_str() << endl;
					//		
					//		std::vector<SPMapInfo*>::iterator iterMap;
					//		for(iterMap = (*iterMapGroup)->m_vpMapInfo.begin(); iterMap != (*iterMapGroup)->m_vpMapInfo.end(); ++iterMap){
					//			file << "\t\t" << "MapID : " << (*iterMap)->iMapID << endl;
					//			file << "\t\t" << "MapName : " << (*iterMap)->strMapName.c_str() << endl;
					//			file << "\t\t" << "BGIndex : " << (*iterMap)->iBGIndex << endl;
					//			//file << "\t\t" << "LeftMap : " << (*iterMap)->iLeftMap << endl;
					//			//file << "\t\t" << "RightMap : " << (*iterMap)->iRightMap << endl;
					//			file << "\t\t" << "FormFile : " << (*iterMap)->strFormFile.c_str() << endl;
					//			file << "\t\t" << "AttributeFile : " << (*iterMap)->strAttributeFile.c_str() << endl;
					//			file << "\t\t" << "MiniMapFile : " << (*iterMap)->strMiniMapFile.c_str() << endl;
					//			file << "\t\t\t" << "MapRect PosLeft : " << (*iterMap)->fMapLeft << endl;
					//			file << "\t\t\t" << "MapRect PosTop : " << (*iterMap)->fMapTop << endl;
					//			file << "\t\t\t" << "MapRect PosRight : " << (*iterMap)->fMapRight << endl;
					//			file << "\t\t\t" << "MapRect PosBottom : " << (*iterMap)->fMapBottom << endl;
					//			file << "\t\t\t" << "MapRect PosWidth : " << (*iterMap)->fMapWidth << endl;
					//			file << "\t\t\t" << "MapRect PosHeight : " << (*iterMap)->fMapHeight << endl;
					//		}
					//	}
					//}
					//#endif

					//ErrorFLogs->Error_Log("Current LoadState [LS_END]") ;
					//터레인 백터에 접근하도록 하는 인덱스를 초기화 한다 
					g_pTerrainManager->GetTerrainCluster()->ResetStage();

					//[2006/3/17] - 이벤트 맵 예외처리, 채널 변경을 막는다
					SPStage* pStage = g_StageManager.GetCurStage(m_iStageID);
					if(pStage) {
						SPMapGroup* pMapGroup = pStage->GetMapGroup(m_iMapGroupID);
						if(pMapGroup && (pMapGroup->IsMapGroupType(GROUP_TYPE_EVENT) || pMapGroup->IsMapGroupType(GROUP_TYPE_INDUN))) {
							g_pInterfaceManager->SPChildWndSendMessage(WIID_EXIT, SPIM_EXIT_CHANNEL_ENABLE, 0, 0);
						}
						else {
							g_pInterfaceManager->SPChildWndSendMessage(WIID_EXIT, SPIM_EXIT_CHANNEL_ENABLE, 1, 0);
						}

						if( SPLocalizeManager::GetInstance()->IsEnableValue(LV_CASHSHOP) &&
							pMapGroup && pMapGroup->IsMapGroupType(GROUP_TYPE_CASH)) {
							g_pInterfaceManager->SPChildWndSendMessage(WIID_MENU, SPIM_SET_CASH_ENABLE, 1, 0);
						}
						else {
							g_pInterfaceManager->SPChildWndSendMessage(WIID_MENU, SPIM_SET_CASH_ENABLE, 0, 0);
						}
					}					
					
					if( g_pGOBManager->GetLocalPlayer() ) {
						g_pGOBManager->GetLocalPlayer()->SetStage(m_iStageID, m_iMapGroupID);
						g_pGOBManager->GetLocalPlayer()->GetCoordPhysics()->SetCoordPhysicsMap(m_iStageID, m_iMapGroupID);
						g_pTerrainManager->GetTerrainCluster()->SetShareLayer();
					}

					m_bSendKeepAlive = false;
					LoadStateNext = LS_THREAD_WAIT;
				}
				break;

			case LS_THREAD_WAIT:
				{
					//ErrorFLogs->Error_Log("Current LoadState [LS_THREAD_WAIT]") ;
					if( m_bThreadExit )
					{
						if( m_hThreadHandle )
						{
							CloseHandle(m_hThreadHandle);
							m_hThreadHandle = NULL;
						}

						m_iGameState = GS_RELEASE_READY;
						m_fWaitTime = 0.0f;
					}
					else
						Sleep(10);
				}
				break;

			case LS_RENDER:
				{
					//if(g_pNetworkManager) {
					//	g_pNetworkManager->Process(fTime);				//[2005/12/10]
					//}

					//렌더 타임마다
					if(m_fLastRenderDelay > 0.0f && m_fLastRenderDelay < fTime) {
						//fTime -= m_fLastRenderDelay;
						fTime = 0.0f;
					}

					m_fAccumulateRender += fTime;
					if( m_fAccumulateRender > m_fCurDelay) {
						m_fAccumulateRender -= m_fCurDelay;						
						
						if(m_fProtage < m_fLoadProtage) {
							m_fProtage++;
						}
						else {
							//ErrorFLogs->Error_Log("Current LoadState [LS_RENDER]") ;
							LoadStateNext = m_LoadStateNext;
						}
					}
					
					ProcessImage(fTime);
					Render(fTime);
					//SPSubGameManager::Process(fTime);
					m_fLastRenderDelay = fTime;
				}
				break;
			}
			m_LoadState = LoadStateNext;
		}
		break;		

	case GS_RELEASE_READY:
		{
			m_fWaitTime -= fTime;
			if(m_fWaitTime <= 0) {
				m_iGameState = GS_RELEASE;
				m_NextSubGameID	= SUBGAME_ZONE;

				if( g_pGOBManager ) {
					g_pGOBManager->SPPerformMessage(SPGM_EVENT_CS_STAGE_ENTER);
					g_pEventManager->SetEventBlock(false);
				}

				ErrorFLogs->Error_Log("Load Complete [GS_RELEASE_READY]") ;

				//[2005/12/30] - 로딩후 이벤트 관련한 팻킷을 보낼수 있도록 한다 
				//[2006/1/2] - 포탈 이벤트를 받은 경우 플래그를 풀어주도록 수정
				//if(g_pEventManager)
				//	g_pEventManager->SetEventPacketSendFlag();
			}
		}
		break;
	}	
	
	if( g_pGOBManager )
		g_pGOBManager->Process(fTime);

	ProcessImage(fTime);
	SPSubGameManager::Process(fTime);
	
}


void SPLoading::Render(float fTime)
{	
	//m_pImageBack->RenderTexture(0,0);

	//
	if(m_pImageBack) {
		m_pImageBack->RenderTexture(&m_rtDest1, &m_rtSrc1);
		m_pImageBack->RenderTexture(&m_rtDest2, &m_rtSrc2);
		m_pImageBack->RenderTexture(&m_rtDest3, &m_rtSrc3);
		m_pImageBack->RenderTexture(&m_rtDest4, &m_rtSrc4);
		m_pImageBack->RenderTexture(&m_rtDest5, &m_rtSrc5);
		m_pImageBack->RenderTexture(&m_rtDest6, &m_rtSrc6);
	}
	g_pVideo->Flush();
	

	for(int j = 0; j < m_pLoadInfo->m_iImageCount; j++) {
		if(m_pImageLoad[j] && m_pLoadInfo->m_LoadImage[j].m_strImage != "") {
			for(int i = 0; i < m_pLoadInfo->m_LoadImage[j].m_iShowCount; i++) {
				m_pImageLoad[j]->RenderTexture(&m_pLoadInfo->m_LoadImage[j].m_rtDest[i], &m_pLoadInfo->m_LoadImage[j].m_rtSrc[i]);
				g_pVideo->Flush();
			}
		}
	}	

	//g_pVideo->SetAlphaTexture(true);
	if( m_pImageGauge != NULL ) // [8/24/2006 AJJIYA]
		m_pImageGauge->RenderTexture(&m_RectGauge, NULL);
	//g_pVideo->SetAlphaTexture(false);
	g_pVideo->Flush();

	m_pImageWrap->RenderTexture(144, 512);
	g_pVideo->Flush();
	
	//g_pVideo->GetSysFont()->RenderText( "LOADING", &m_RectLog, DT_TOP | DT_CENTER );

	SPSubGameManager::Render(fTime);
}

void SPLoading::RenderText()
{

}

void SPLoading::ProcessImage(float fTime)
{
	//m_RectGauge.left	= 10;
	//m_RectGauge.right	= m_RectGauge.left + (431.f *  (m_fLoadProtage / 100));
	m_RectGauge.right	= m_RectGauge.left + (431.f *  (m_fProtage / 100));
}


SPLoadInfo* SPLoading::LoadLoadingDataLDT(int iItemID /*= 0*/)
{	
	if(m_iStageID == 0 && m_iMapGroupID == 0) {
		//Load Default...
		iItemID = 1;
	}
	//else if() {					//[2005/9/14] 인던 들어가면 처리할것.
	//}
	else {
		//Load...		
		iItemID = m_iStageID * 1000 + m_iMapGroupID;
	}

	assert(m_pLDTLoadData != NULL && "m_pLDTLoadData is NULL..");
	assert(m_pLoadInfo != NULL && "m_pLoadInfo is NULL..");

	int recordCount = m_pLDTLoadData->GetItemCount();
	
	std::string strBGImage = "";
	strBGImage.clear();
	
	LDT_Field ldtFieldBGImage;
	m_pLDTLoadData->GetFieldFromLable( iItemID, "_BG_Image",	ldtFieldBGImage);
	
	if(ldtFieldBGImage.uData.pValue == NULL)					//[2005/9/30]
		return NULL;
	
	strBGImage = ldtFieldBGImage.uData.pValue;	
	
	m_pLoadInfo->SetBGFile(strBGImage);

	std::string strImage = "";	
	RECT rtSrc;	
	RECT rtDest;	
	
	char szFieldImage[64];
	LDT_Field ldtFieldImage;

	char szFieldSrcLeft[64];
	char szFieldSrcTop[64];
	char szFieldSrcRight[64];
	char szFieldSrcBottom[64];

	LDT_Field ldtFieldSrcLeft;
	LDT_Field ldtFieldSrcTop;
	LDT_Field ldtFieldSrcRight;
	LDT_Field ldtFieldSrcBottom;

	char szFieldDestLeft[64];
	char szFieldDestTop[64];

	LDT_Field ldtFieldDestLeft;
	LDT_Field ldtFieldDestTop;
	
	for(int i = 0; i < MAX_IMAGE_LIST; i++) {
		ZeroMemory(szFieldImage, 64);
		wsprintf(szFieldImage, "_Image%d", i+1);
		
		strImage = "";
		strImage.clear();
		SetRect(&rtSrc, -1, -1, -1, -1);
		SetRect(&rtDest, -1, -1, -1, -1);
		
		m_pLDTLoadData->GetFieldFromLable(iItemID, szFieldImage, ldtFieldImage);
		if(ldtFieldImage.uData.pValue)
			strImage = ldtFieldImage.uData.pValue;

		if(strImage == "") {
			break;
		}
		
		m_pLoadInfo->SetImageFile(i, strImage);

		for(int j = 0; j < MAX_IMAGE_LIST ; j++) {
			ZeroMemory(szFieldSrcLeft, 64);
			wsprintf(szFieldSrcLeft, "_Input%d_Left%d", i + 1, j + 1);
			ZeroMemory(szFieldSrcTop, 64);
			wsprintf(szFieldSrcTop, "_Input%d_Top%d", i + 1, j + 1);
			ZeroMemory(szFieldSrcRight, 64);
			wsprintf(szFieldSrcRight, "_Input%d_Right%d", i + 1, j + 1);
			ZeroMemory(szFieldSrcBottom, 64);
			wsprintf(szFieldSrcBottom, "_Input%d_Bottom%d", i + 1, j + 1);

			ZeroMemory(szFieldDestLeft, 64);
			wsprintf(szFieldDestLeft, "_Output%d_PosX%d", i + 1, j + 1);
			ZeroMemory(szFieldDestTop, 64);
			wsprintf(szFieldDestTop, "_Output%d_PosY%d", i + 1, j + 1);


			m_pLDTLoadData->GetFieldFromLable(iItemID, szFieldSrcLeft, ldtFieldSrcLeft);
			rtSrc.left = ldtFieldSrcLeft.uData.lValue;
			if(rtSrc.left < 0) {
				break;
			}
			
			m_pLDTLoadData->GetFieldFromLable(iItemID, szFieldSrcTop, ldtFieldSrcTop);
			m_pLDTLoadData->GetFieldFromLable(iItemID, szFieldSrcRight, ldtFieldSrcRight);
			m_pLDTLoadData->GetFieldFromLable(iItemID, szFieldSrcBottom, ldtFieldSrcBottom);
			rtSrc.top = ldtFieldSrcTop.uData.lValue;
			rtSrc.right = ldtFieldSrcRight.uData.lValue;
			rtSrc.bottom = ldtFieldSrcBottom.uData.lValue;

			m_pLDTLoadData->GetFieldFromLable(iItemID, szFieldDestLeft, ldtFieldDestLeft);
			m_pLDTLoadData->GetFieldFromLable(iItemID, szFieldDestTop, ldtFieldDestTop);
			rtDest.left = ldtFieldDestLeft.uData.lValue;
			rtDest.top = ldtFieldDestTop.uData.lValue;

			m_pLoadInfo->SetImageRect(i, j, rtSrc, rtDest);
		}
	}
	
	return m_pLoadInfo;
}


int SPLoading::LoadLoadingImage()
{
	if(m_pLoadInfo->m_strBGImage != "") {
		if(m_pImageBack)
			SAFE_RELEASE(m_pImageBack);

		if(false == g_pVideo->GetTextureMgr()->LoadTexture(m_pLoadInfo->m_strBGImage.c_str(), &m_pImageBack))
			return 0;

		if( SPLocalizeManager::GetInstance()->IsEnableValue(LV_PVP) )
		{
			// Loading 시에 배경이미지 이름을 저장하도록 한다.
			if( g_pkPvpManager->GetPvpStatus() != PVP_STATUS_EMPTY )
			{
				g_pkPvpManager->SetLoadBGFileName(m_pLoadInfo->m_strBGImage.c_str()) ;
				ErrorFLogs->Error_Log("Load Loading Image ImageBack [%s] Stage[%d] MapGroup[%d]", m_pLoadInfo->m_strBGImage.c_str(), m_iStageID, m_iMapGroupID) ;
			}
		}
	}
	else
	{
		ErrorFLogs->Error_Log("Load Loading Image ImageBack NULL!!! Stage[%d] MapGroup[%d]", m_iStageID, m_iMapGroupID) ;
	}

	for(int i = 0; i < MAX_IMAGE_LIST; i++) {
		if(m_pLoadInfo->m_LoadImage[i].m_strImage != "") {
			if(m_pImageLoad[i]) {
				SAFE_RELEASE(m_pImageLoad[i]);
			}
			
			if(false == g_pVideo->GetTextureMgr()->LoadTexture(m_pLoadInfo->m_LoadImage[i].m_strImage.c_str(), &m_pImageLoad[i]))
				return -2;	
			//ErrorFLogs->Error_Log("Load Loading Image m_LoadImage [%d][%s]", i, m_pLoadInfo->m_strBGImage.c_str()) ;
		}
	}

	return 1;
}


int SPLoading::SPPerformMessage	(UINT msg, WPARAM wparam, LPARAM lParam)
{
	if(SPGM_SUG_LOAD_BEGIN <= msg && SPGM_SUG_LOAD_END > msg )
		return SPGMESSAGE_FUNC((SPGM)msg, wparam, lParam);

	return SPSubGameManager::SPPerformMessage(msg, wparam, lParam);
}

int SPLoading::OnLoadSetStage(WPARAM wparam, LPARAM lparam)
{
	m_iStageID = (int)lparam;	
	return m_iStageID;
}

int SPLoading::OnLoadSetGroup(WPARAM wparam, LPARAM lparam)
{
	m_iMapGroupID = (int)lparam;
	
	//[2005/9/21] 심의버전 대비 임시 블럭...
	// Init가 호출된뒤 실행한다
	if(LoadLoadingDataLDT() != NULL) {
		//Image Load...
		if(m_pImageBack)
			SAFE_RELEASE(m_pImageBack);

		for(int i = 0; i < MAX_IMAGE_LIST; i++) {
			if(m_pImageLoad[i]) {
				SAFE_RELEASE(m_pImageLoad[i]);
			}
		}

		LoadLoadingImage();
	}

	return m_iMapGroupID;
}


int	SPLoading::OnSocketFailed(WPARAM wparam, LPARAM lparam)
{	
	SPCashTradeManager::GetInstance()->SetSocketFail();
	
	return SPSubGameManager::OnSocketFailed(wparam, lparam);
}


//////////////////////////////////////////////////////////////////////////
//
SPGMESSAGE_MAP_BEGIN(SPLoading)
SPGMESSAGE_COMMAND( SPGM_SUG_LOAD_SET_STAGE			, OnLoadSetStage	)
SPGMESSAGE_COMMAND( SPGM_SUG_LOAD_SET_GROUP			, OnLoadSetGroup	)
SPGMESSAGE_COMMAND( SPGM_SUG_SOCKET_FAILED			, OnSocketFailed	)
SPGMESSAGE_MAP_END(SPSubGameManager)













