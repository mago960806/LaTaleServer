#include <algorithm>
#include <functional>

#include "SPCommon.h"
#include "SPCommandConvert.h"
#include "SPMainGameDEF.H"

#include "SPGameObjectDef.h"
#include "SPGameObject.h"

#include "SPJoyPadManager.h"

#include "SPStage.h"
#include "SPStageManager.h"
#include "SPTerrainAttribute.h"
#include "SPGOBCoordPhysics.h"

#include "SPGOBClusterDEF.h"
#include "SPAvatarModelUnitDef.h"
#include "SPItemAttr.h"
#include "SPItemStatus.h"

#include "SPGOBStatus.h"
#include "SPPlayerStatus.h"
#include "SPPlayerStatusModel.h"
#include "SPBeAttackedList.h"
#include "SPPet.h"
#include "SPGroundPet.h"
#include "SPFlyPet.h"
#include "SPPlayer.h"

#include "SPMonsterStatus.h"
#include "SPMonsterStatusModel.h"
#include "SPMonster.h"

#include "SPLootingBag.h"

#include "SPMessageBoard.h"

#include "SPResourceDef.h"
#include "SPGOBClusterDEF.h"
#include "SPManager.h"

#include "Packet.h"
#include "SPNetworkManager.h"

#include "SPGOBCluster.h"
#include "SPMOBCluster.h"
#include "SPGOBManager.h"

#include "SPBattle.h"
#include "SPBattleCluster.h"
#include "SPBattleLoader.h"

//#include "SPItem.h"
#include "SPItemCluster.h"		//[2005/6/16]

#include "SPRenderModelDEF.h"
#include "SPRenderModel.h"

#include "SPTerrainDEF.h"
#include "SPTerrainCluster.h"
#include "SPTerrainManager.h"

#include "SPFrameSkipper.h"

#include "SPEventDEF.h"
#include "SPQuestManager.h"
#include "SPGiftManager.h"
#include "SPMailManager.h"
#include "SPUserShopAttr.h"
#include "SPUserShopArchive.h"
#include "SPWayPointManager.h"
#include "SPEventNpcHandler.h"
#include "SPCoreMissionArchive.h"
#include "SPCoreEventManager.h"
#include "SPEventManager.h"
#include "SPCheckManager.h"
#include "SPLocalizeManager.h"

#include "SPMainGameDEF.H"
#include "SPSubGameManager.h"
#include "SPMainGameManager.h"
#include "SPEvent.h"
#include "SPDebug.h"

//[2005/4/14] - jinhee for Use Interface Manager... 
#include "SPMouseCursor.h"
#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPInterfaceManager.h"

#include "SPMiniMapCtrl/SPWindowMiniMapIcon.h"

#include "SPTitleAttr.h"
#include "SPTitleArchive.h"
#include "SPTitleRenderDEF.h"
#include "SPTitleRenderBase.h"
#include "SPTitleManager.h"

#include "SPCoolTimeManager.h"
#include "SPMonsterModelUnitDef.h"

#include "SPGuildLDT.h"
#include "SPGuildArchive.h"
#include "SPCropArchive.h"

#include "SPResourceDef.h"
#include "SPResourceManager.h"
#include "PacketID.h"
#include "GlobalDefines_Share.h"

#include "SPPvpManager.h"
#include "SPAuctionManager.h"

#include "SPChatBoard.h"

#include "SPCashTradeManager.h"
#include "SPPetLevelMgr.h"

#include "SPStoryManager.h"
#include "_Error_log.h"

#include "DebugPrint.h"
#include "lgclientlog.h"

#include "SPFXDEF.h"

using namespace std;

extern SPMouseCursor* g_pMouseCursor;

SPGOBManager* g_pGOBManager = NULL;

struct _IsStandLayer : binary_function<SPGameObject*, BG_ID, bool> {
	bool operator() (SPGameObject* pGob, BG_ID LayerID) const
	{
		if(LayerID == pGob->SPGOBPerformMessage(SPGM_GETSTANDLAYER))		
			return true;
		return false;
	}
} IsStandLayer;

namespace {
	struct _IsEqualInstance : binary_function<SPGameObject*, INSTANCE_ID, bool> {
		bool operator() (SPGameObject* pGob, INSTANCE_ID InstanceID) const
		{
			if(pGob->GetInstanceID() == InstanceID)
				return true;
			return false;
		}
	} IsEqualInstance;
}

SPGOBManager::SPGOBManager()
: SPManager()
, m_pGOBCluster(NULL)
, m_pMOBCluster(NULL)
, m_pLocalPlayer(NULL)
, m_pGuildArchive(NULL)
, m_iSendPlayerInfo(0)
{
	SetMgrID(MGR_ID_GOB);
	m_LastInstance = 0;

	g_pGOBManager = this;
}

SPGOBManager::~SPGOBManager()
{	
	PurgeAll();
	Clean();
	g_pGOBManager = NULL;
}

bool SPGOBManager::Init()
{
	SPManager::Init();
	assert(!m_pGOBCluster);
	assert(!m_pMOBCluster);

	m_pGOBCluster = new SPGOBCluster(this);
	m_pMOBCluster = new SPMOBCluster(this);

	m_pGuildArchive = new SPGuildArchive() ;
	m_pGuildArchive->Init() ;

	m_LastInstance = 0;

	/////////////////////////////////////////////////////////////////////////////////////////////
	//Local Player Sample Data.
//#ifdef _ALONE
	//SetLocalPlayer(AddPlayer(450.f , 300.f));
//#endif

	// StressTest.
	//float fTerrainSX	= GetParent()->GetTerrainManager()->GetSX();
	//float fTerrainSY	= GetParent()->GetTerrainManager()->GetSY();
	//for(int i = 0; i < 100; ++i)
	//	AddPlayer((rand() % (int)(fTerrainSX-50))+5 , (rand() % (int)(fTerrainSY-50))+5);
	/////////////////////////////////////////////////////////////////////////////////////////////

	m_iCursorX = 0;
	m_iCursorY = 0;

	return true;
}

void SPGOBManager::Clean()
{
	SAFE_DELETE(m_pGOBCluster);
	SAFE_DELETE(m_pMOBCluster);
	SAFE_DELETE(m_pGuildArchive) ;

	SPBattleCluster::Release();	

	SPManager::Clean();
}


bool SPGOBManager::SetLocalPlayer(SPPlayer* pPlayer)
{
	if(m_pLocalPlayer)
		m_pLocalPlayer->SetLocalPlayer(false);

	if(pPlayer)
	{
		m_pLocalPlayer = pPlayer;
		pPlayer->SetLocalPlayer(true);
	}
	
	if(g_pGOBManager){
		g_pEventManager->SetEventPacketSendFlag();
		g_pEventManager->SetCutInState(CUT_IN_STATE_NULL);
	}

	return true;
}

SPGameObject* SPGOBManager::GetLootingBag(float fPosX, float fPosY, SPGameObject* pkExceptObject, bool bUsedDistance)
{
	SPLootingBag* pkBag, *pkRetBag = NULL;
	bool bExist = false;
	float fCompareDistance, fDistance = 0.0f;

	vector<SPGameObject*>::iterator iter = m_vpGameObjLootingBag.begin();
	while( iter != m_vpGameObjLootingBag.end() )
	{
		pkBag = (SPLootingBag*)(*iter);
		if( pkBag && pkBag->GetEnablePickup() && pkBag != pkExceptObject )
		{
			float fBagPosX = (float)pkBag->SPGOBPerformMessage(SPGM_GETPOSX);
			float fBagPosY = (float)pkBag->SPGOBPerformMessage(SPGM_GETPOSY);

			float fDistX, fDistY;
			fDistX = (fBagPosX - fPosX);
			fDistY = (fBagPosY - fPosY);

			fDistance = (fDistX * fDistX) + (fDistY * fDistY);
			if( bUsedDistance )
			{
				if( fDistance > (40 * 40) )
				{
					++iter;
					continue;
				}
			}
			
			if( !bExist )
			{
				bExist = true;
				fCompareDistance = fDistance;
				pkRetBag = pkBag;

				++iter;
				continue;
			}

			if( fDistance < fCompareDistance )
			{
				fCompareDistance = fDistance;
				pkRetBag = pkBag;
			}
		}

		++iter;
	}

	return pkRetBag;
}

SPGuildArchive* SPGOBManager::GetGuildArchive()
{	
	return m_pGuildArchive ;	
}


int SPGOBManager::GetLootingBagSize()
{
	return (int)m_vpGameObjLootingBag.size();
}

SPGameObject* SPGOBManager::GetLootingBag(int iIndex)
{
	if( iIndex < 0 || iIndex >= (int)m_vpGameObjLootingBag.size() )
		return NULL;

	return m_vpGameObjLootingBag[iIndex];
}


int SPGOBManager::GetAllMonster(std::vector< SPGameObject* >& vMOBList)
{
	vMOBList = m_vpGameObjMonster;
	return (int)m_vpGameObjMonster.size();
}

int SPGOBManager::GetAllPlayer(std::vector< SPGameObject* >& vPlayerList)
{
	vPlayerList = m_vpGameObjPlayer;
	return (int)m_vpGameObjPlayer.size();
}


void SPGOBManager::SetAllPlayerWantTeam(bool bShowByPvp /*= false*/)
{
	std::vector<SPGameObject*>::iterator Iter;

	SPPlayer* pPlayer = NULL;
	SPPlayerStatus* pPlayerStatus = NULL;
	Iter = m_vpGameObjPlayer.begin();
	for(; Iter !=  m_vpGameObjPlayer.end(); ++Iter) {
		pPlayer = NULL;
		pPlayerStatus = NULL;
		
		pPlayer = static_cast<SPPlayer*>((*Iter));
		if(pPlayer && pPlayer->GetStatus()) {
			pPlayerStatus = static_cast<SPPlayerStatus*>(pPlayer->GetStatus());
			pPlayerStatus->SetWantParty(bShowByPvp);
		}
	}
	//ErrorFLogs->Error_Log("@@@ Set AllPlayer WantTeam!!!!") ;
}


SPGameObject* SPGOBManager::Find( GU_ID guid )
{
	return Find( GetGUID2InstanceID(guid), GetGUID2ClassID(guid) );
}

SPGameObject* SPGOBManager::Find( INSTANCE_ID InstanceID, CLASS_ID ClassID )
{
	std::vector<SPGameObject*>::iterator Iter;

	switch(ClassID) {
	case CLASS_ID_NULL:		
		break;
	case CLASS_ID_AVATAR:
		{		
			Iter = std::find_if(m_vpGameObjPlayer.begin(), 	m_vpGameObjPlayer.end(), 
								bind2nd(IsEqualInstance, InstanceID) );
			if(Iter != m_vpGameObjPlayer.end())
				return (*Iter);
		}		
		break;
	case CLASS_ID_LOOTING_BAG:
		{
			Iter = std::find_if(m_vpGameObjLootingBag.begin(), 	m_vpGameObjLootingBag.end(), 
				bind2nd(IsEqualInstance, InstanceID) );
			if(Iter != m_vpGameObjLootingBag.end())
				return (*Iter);
		}
		break;
	case CLASS_ID_CHAT_BOARD:
		{
			Iter = std::find_if(m_vpGameObjChatBoard.begin(), 	m_vpGameObjChatBoard.end(), 
				bind2nd(IsEqualInstance, InstanceID) );
			if(Iter != m_vpGameObjChatBoard.end())
				return (*Iter);
		}
		break;
	case CLASS_ID_PET_SHOP_OBJECT:
		{
			Iter = std::find_if(m_vpGameObjShopPet.begin(), 	m_vpGameObjShopPet.end(), 
				bind2nd(IsEqualInstance, InstanceID) );
			if(Iter != m_vpGameObjShopPet.end())
				return (*Iter);
		}
		break;
	}

	if( ClassID >= CLASS_ID_MOB_FIRST &&
		ClassID <= CLASS_ID_MOB_LAST )
	{
		Iter = std::find_if(m_vpGameObjMonster.begin(), m_vpGameObjMonster.end(), 
								bind2nd(IsEqualInstance, InstanceID) );

		if(Iter != m_vpGameObjMonster.end())
		{
			return (*Iter);
		}

	}

	if( CLASS_ID_MAPEVENT_FIRST <= ClassID && ClassID <= CLASS_ID_MAPEVENT_LAST) {
		Iter = std::find_if(m_vpGameObjEvent.begin(), m_vpGameObjEvent.end(),
			bind2nd(IsEqualInstance, InstanceID) );

		if(Iter != m_vpGameObjEvent.end()) {
			return (*Iter);
		}
	}
	
	return NULL;
}


SPPlayer* SPGOBManager::FindPlayerByName( const char* pCharName  )
{
	std::vector<SPGameObject*>::iterator Iter;

	if( pCharName == NULL )
		return NULL ;

	SPPlayer* pPlayer = NULL ;
	Iter = m_vpGameObjPlayer.begin() ;
	while ( Iter != m_vpGameObjPlayer.end() )
	{
		pPlayer = (SPPlayer*)(*Iter) ;
		if( strcmp( pPlayer->GetGOBName(), pCharName) == 0 )
			return pPlayer ;
		++Iter ;
	}
	return NULL ;
}


void SPGOBManager::Process(float fTime)
{
	SPManager::Process(fTime);

	std::vector<SPGameObject*>::iterator Iter;

	Iter = m_vpGameObjPlayer.begin();
	for(; Iter !=  m_vpGameObjPlayer.end(); ++Iter) {
		(*Iter)->Process(fTime);
	}

	Iter = m_vpGameObjMonster.begin();
	for(; Iter !=  m_vpGameObjMonster.end(); ++Iter) {
		(*Iter)->Process(fTime);
	}

	Iter = m_vpGameObjChatBoard.begin();
	for(; Iter !=  m_vpGameObjChatBoard.end(); ++Iter) {
		(*Iter)->Process(fTime);
	}

	Iter = m_vpGameObjLootingBag.begin();
	for(; Iter !=  m_vpGameObjLootingBag.end(); ++Iter) {
		(*Iter)->Process(fTime);
	}

	Iter = m_vpGameObjEvent.begin();
	for(; Iter != m_vpGameObjEvent.end(); ++Iter) {
		(*Iter)->Process(fTime);
	}

	Iter = m_vpGameObjMessageBoard.begin();
	for(; Iter != m_vpGameObjMessageBoard.end(); ++Iter) {
		(*Iter)->Process(fTime);
	}

	Iter = m_vpGameObjShopPet.begin();
	for(; Iter != m_vpGameObjShopPet.end(); ++Iter) {
		(*Iter)->Process(fTime);
	}

	GetGuildArchive()->Process(fTime);
#ifdef _JOY_PAD
	if(SPLocalizeManager::GetInstance()->IsEnableValue(LV_JOYPAD)) {
		if(g_pVideo->IsActive() && g_pInterfaceManager && g_pInterfaceManager->IsSubGame())
			ProcessJoyPad(fTime);
	}
#endif

	GetPlayerInfo();
}

extern FLOAT  g_fFPS;
void SPGOBManager::GetPlayerInfo()
{
	static DWORD _Protime = 0;

	DWORD timen = timeGetTime();
	if(g_fFPS < 15 || timen - _Protime < 333) return;
	_Protime = timen;

	int num = m_vPlayerID.size();
	
	if( num<= 0) return;

	CPacket Packet;
	UINT32 ID;
	Packet.Add((UINT32)NAV_CS_INSIGHT_GUID);

	if(num == 1)
	{
		ID = m_vPlayerID.at(0);
		Packet.AddUINT8((UINT8)1);
		Packet.AddUINT32((UINT32)ID);
		m_vPlayerID.clear();
	}
	else
	{
		Packet.AddUINT8((UINT8)2);

		std::vector<UINT32>::iterator it = m_vPlayerID.begin();
		ID = *it;
		Packet.AddUINT32((UINT32)ID);
		it = m_vPlayerID.erase(it);
		ID = *it;
		Packet.AddUINT32((UINT32)ID);
		it = m_vPlayerID.erase(it);
	}

	g_pNetworkManager->SPPerformMessage(NAV_CS_INSIGHT_GUID, 0, (LPARAM)&Packet );
}

void SPGOBManager::ProcessJoyPad(float fTime)
{			
	g_pInputManager->GetJoyPadManager()->SetPadMouseMove(false);
	
	if(!g_pInput->IsEquipJoystick()) 
		return;

	SPInputStructJoyStick* pStick = g_pInputManager->GetInputStructJoyStick();
	if(pStick == NULL)	return;
	
	if(g_pInputManager->GetJoyPadManager()->GetJoyStickMode() == JOYSTICK_MODE_ANALOG){		
		//m_MouseCursor.GetMouseSize()

		if(pStick->ibLX.iValue < -100) {
			if(pStick->ibLX.iValue <= -1000){
				m_iCursorX -= 7;
			}
			//else if(pStick->ibLX.iValue <= -800){
			//	m_iCursorX -= 5;
			//}
			else if(pStick->ibLX.iValue <= -100){
				m_iCursorX -= 1;
			}

			if(m_iCursorX < 0)
				m_iCursorX = 0;

			g_pInputManager->GetJoyPadManager()->SetPadMouseMove(true);
		}	
		else if(pStick->ibLX.iValue > 100) {
			if(pStick->ibLX.iValue >= 1000){
				m_iCursorX += 7;
			}
			//else if(pStick->ibLX.iValue >= 800){
			//	m_iCursorX += 5;
			//}
			else if(pStick->ibLX.iValue >= 100){
				m_iCursorX += 1;
			}
			
			if(m_iCursorX > g_pVideo->GetScreenLenX() - 8)
				m_iCursorX = g_pVideo->GetScreenLenX() - 8;

			g_pInputManager->GetJoyPadManager()->SetPadMouseMove(true);
		}

		if(pStick->ibLY.iValue < -100) {
			if(pStick->ibLY.iValue <= -1000){
				m_iCursorY -= 7;
			}
			//else if(pStick->ibLY.iValue <= -800){
			//	m_iCursorY -= 5;
			//}
			else if(pStick->ibLY.iValue <= -100){
				m_iCursorY -= 1;
			}

			if(m_iCursorY < 0)
				m_iCursorY = 0;

			g_pInputManager->GetJoyPadManager()->SetPadMouseMove(true);
		}	
		else if(pStick->ibLY.iValue > 100) {
			if(pStick->ibLY.iValue >= 1000){
				m_iCursorY += 7;
			}
			//else if(pStick->ibLY.iValue >= 800){
			//	m_iCursorY += 5;
			//}
			else if(pStick->ibLY.iValue >= 100){
				m_iCursorY += 1;
			}

			if(m_iCursorY > g_pVideo->GetScreenLenY() - 8)
				m_iCursorY = g_pVideo->GetScreenLenY() - 8;

			g_pInputManager->GetJoyPadManager()->SetPadMouseMove(true);
		}
		
		if(g_pInputManager->GetJoyPadManager()->IsPadMouseMove()) {			
			PerformMouseMove((float)m_iCursorX, (float) m_iCursorY);
			
			POINT pt;
			RECT rect;
			::GetWindowRect(g_hWnd, &rect);
			pt.x = m_iCursorX;
			pt.y = m_iCursorY;
			pt.x += rect.left;
			pt.y += rect.top;
			pt.y += g_pInputManager->GetJoyPadManager()->GetFixHeight();
			pt.x += g_pInputManager->GetJoyPadManager()->GetFixWidth();
			::SetCursorPos(pt.x, pt.y);
		}

		std::vector<SPJoyPadButtonInfo*>* pvButton = g_pInputManager->GetJoyPadManager()->GetPadButton(PAD_BUTTON_KEY_TYPE_MOUSE);
		std::vector<SPJoyPadButtonInfo*>::iterator iter;
		iter = pvButton->begin();
		for(; iter != pvButton->end(); ++iter) {
			//iKeyIndex = (*iter)->m_iKey - BUTTON_KEY_MOUSE_START;
			if(m_bJoyPadLPush == false && (*iter)->m_iKey == BUTTON_KEY_MOUSE_L && (*iter)->m_iInputState == INPUT_REPEAT) {
				PerformMouseLDown((float)m_iCursorX, (float) m_iCursorY);
				DXUTOutputDebugString("\t * JoyPad LDownPress... Down\n");
				m_bJoyPadLPush = true;
			}

			if(m_bJoyPadLPush && (*iter)->m_iKey == BUTTON_KEY_MOUSE_L && (*iter)->m_iInputState == INPUT_UNREPEAT){
				PerformMouseLUp((float)m_iCursorX, (float) m_iCursorY);
				DXUTOutputDebugString("\t * JoyPad LDownPress... Up\n");
				m_bJoyPadLPush = false;
			}

			if(m_bJoyPadRPush == false && (*iter)->m_iKey == BUTTON_KEY_MOUSE_R && (*iter)->m_iInputState == INPUT_REPEAT) {
				PerformMouseRDown((float)m_iCursorX, (float) m_iCursorY);
				DXUTOutputDebugString("\t * JoyPad RDownPress... Down\n");
				m_bJoyPadRPush = true;
			}

			if(m_bJoyPadRPush && (*iter)->m_iKey == BUTTON_KEY_MOUSE_R && (*iter)->m_iInputState == INPUT_UNREPEAT) {
				PerformMouseRUp((float)m_iCursorX, (float) m_iCursorY);
				DXUTOutputDebugString("\t * JoyPad RDownPress... Up\n");
				m_bJoyPadRPush = false;
			}
		}		
	}
}


void SPGOBManager::Render(float fTime)
{
	SPManager::Render(fTime);	

	std::vector<SPGameObject*>::reverse_iterator RIter;

	RIter = m_vpGameObjMessageBoard.rbegin();
	for(; RIter != m_vpGameObjMessageBoard.rend(); ++RIter){
		if((*RIter)->SPGOBPerformMessage(SPGM_GETSTANDLAYER) == BG_ID_LAYER_TERRAIN0) {
			(*RIter)->Render(fTime);
		}
	}

	RIter = m_vpGameObjEvent.rbegin();
	for(; RIter != m_vpGameObjEvent.rend(); ++RIter){
		if((*RIter)->SPGOBPerformMessage(SPGM_GETSTANDLAYER) == BG_ID_LAYER_TERRAIN0) {
			(*RIter)->Render(fTime);
		}
	}

	RIter = m_vpGameObjChatBoard.rbegin();
	for(; RIter != m_vpGameObjChatBoard.rend(); ++RIter){
		if((*RIter)->SPGOBPerformMessage(SPGM_GETSTANDLAYER) == BG_ID_LAYER_TERRAIN0) {
			(*RIter)->Render(fTime);
		}
	}

	g_pVideo->Flush();

	RIter = m_vpGameObjMonster.rbegin();
	for(; RIter !=  m_vpGameObjMonster.rend(); ++RIter) {
		if((*RIter)->SPGOBPerformMessage(SPGM_GETSTANDLAYER) == BG_ID_LAYER_TERRAIN0) {
			(*RIter)->Render(fTime);
		}
	}

	RIter = m_vpGameObjPlayer.rbegin();
	for(; RIter !=  m_vpGameObjPlayer.rend(); ++RIter) {
		SPPlayer* pPlayer = static_cast<SPPlayer*>((*RIter));
		if(pPlayer) {
			if(pPlayer->GetPlayerPet() && pPlayer->GetPlayerPet()->IsPreRendering()) {
				if(pPlayer->GetPlayerPet()->GetStandLayer() == BG_ID_LAYER_TERRAIN0)
					;//pPlayer->GetPlayerPet()->Render(false);
			}
		}

		if((*RIter)->SPGOBPerformMessage(SPGM_GETSTANDLAYER) == BG_ID_LAYER_TERRAIN0) {
			(*RIter)->Render(fTime);
		}		
	}
	
	RIter = m_vpGameObjLootingBag.rbegin();
	for(; RIter !=  m_vpGameObjLootingBag.rend(); ++RIter) {
		if((*RIter)->SPGOBPerformMessage(SPGM_GETSTANDLAYER) == BG_ID_LAYER_TERRAIN0) {
			(*RIter)->Render(fTime);
		}		
	}



	g_pVideo->Flush();
}

void SPGOBManager::RenderLayer2(float fTime)
{	
	std::vector<SPGameObject*>::reverse_iterator RIter;

	RIter = m_vpGameObjMessageBoard.rbegin();
	for(; RIter != m_vpGameObjMessageBoard.rend(); ++RIter){
		if((*RIter)->SPGOBPerformMessage(SPGM_GETSTANDLAYER) == BG_ID_LAYER_TERRAIN1) {
			(*RIter)->Render(fTime);
		}
	}

	RIter = m_vpGameObjEvent.rbegin();
	for(; RIter != m_vpGameObjEvent.rend(); ++RIter){
		if((*RIter)->SPGOBPerformMessage(SPGM_GETSTANDLAYER) == BG_ID_LAYER_TERRAIN1) {
			(*RIter)->Render(fTime);
		}
	}

	RIter = m_vpGameObjChatBoard.rbegin();
	for(; RIter != m_vpGameObjChatBoard.rend(); ++RIter){
		if((*RIter)->SPGOBPerformMessage(SPGM_GETSTANDLAYER) == BG_ID_LAYER_TERRAIN1) {
			(*RIter)->Render(fTime);
		}
	}

	

	g_pVideo->Flush();

	RIter = m_vpGameObjMonster.rbegin();
	for(; RIter !=  m_vpGameObjMonster.rend(); ++RIter) {
		if((*RIter)->SPGOBPerformMessage(SPGM_GETSTANDLAYER) == BG_ID_LAYER_TERRAIN1) {
			(*RIter)->Render(fTime);
		}		
	}
	
	RIter = m_vpGameObjPlayer.rbegin();
	for(; RIter !=  m_vpGameObjPlayer.rend(); ++RIter) {
		SPPlayer* pPlayer = static_cast<SPPlayer*>((*RIter));
		if(pPlayer) {
			if(pPlayer->GetPlayerPet() && pPlayer->GetPlayerPet()->IsPreRendering()) {
				if(pPlayer->GetPlayerPet()->GetStandLayer() == BG_ID_LAYER_TERRAIN1)
					;//pPlayer->GetPlayerPet()->Render(false);
			}
		}

		if((*RIter)->SPGOBPerformMessage(SPGM_GETSTANDLAYER) == BG_ID_LAYER_TERRAIN1) {
			(*RIter)->Render(fTime);
		}		
	}	

	RIter = m_vpGameObjLootingBag.rbegin();
	for(; RIter !=  m_vpGameObjLootingBag.rend(); ++RIter) {
		if((*RIter)->SPGOBPerformMessage(SPGM_GETSTANDLAYER) == BG_ID_LAYER_TERRAIN1) {
			(*RIter)->Render(fTime);
		}		
	}

	RIter = m_vpGameObjShopPet.rbegin();
	for(; RIter !=  m_vpGameObjShopPet.rend(); ++RIter) {
		(*RIter)->Render(fTime);
	}
	if (!m_vpGameObjShopPet.empty() && !m_vpGameObjPlayer.empty())
	{
		if (m_vpGameObjPlayer[0])
			m_vpGameObjPlayer[0]->Render(fTime);
	}

	g_pVideo->Flush();
}


void SPGOBManager::RenderLayer3(float fTime){
	std::vector<SPGameObject*>::reverse_iterator RIter;

	RIter = m_vpGameObjMessageBoard.rbegin();
	for(; RIter != m_vpGameObjMessageBoard.rend(); ++RIter){
		if((*RIter)->SPGOBPerformMessage(SPGM_GETSTANDLAYER) != BG_ID_LAYER_TERRAIN0 &&
			(*RIter)->SPGOBPerformMessage(SPGM_GETSTANDLAYER) != BG_ID_LAYER_TERRAIN1) {
				(*RIter)->Render(fTime);
		}
	}

	RIter = m_vpGameObjEvent.rbegin();
	for(; RIter != m_vpGameObjEvent.rend(); ++RIter){
		if((*RIter)->SPGOBPerformMessage(SPGM_GETSTANDLAYER) != BG_ID_LAYER_TERRAIN0 &&
			(*RIter)->SPGOBPerformMessage(SPGM_GETSTANDLAYER) != BG_ID_LAYER_TERRAIN1) {
				(*RIter)->Render(fTime);
		}
	}

	RIter = m_vpGameObjChatBoard.rbegin();
	for(; RIter != m_vpGameObjChatBoard.rend(); ++RIter){
		if((*RIter)->SPGOBPerformMessage(SPGM_GETSTANDLAYER) != BG_ID_LAYER_TERRAIN0 &&
			(*RIter)->SPGOBPerformMessage(SPGM_GETSTANDLAYER) != BG_ID_LAYER_TERRAIN1) {
				(*RIter)->Render(fTime);
			}
	}

	

	g_pVideo->Flush();

	RIter = m_vpGameObjMonster.rbegin();
	for(; RIter !=  m_vpGameObjMonster.rend(); ++RIter) {
		if((*RIter)->SPGOBPerformMessage(SPGM_GETSTANDLAYER) != BG_ID_LAYER_TERRAIN0 && 
			(*RIter)->SPGOBPerformMessage(SPGM_GETSTANDLAYER) != BG_ID_LAYER_TERRAIN1) {
				(*RIter)->Render(fTime);
			}		
	}

	RIter = m_vpGameObjPlayer.rbegin();
	for(; RIter !=  m_vpGameObjPlayer.rend(); ++RIter) {
		SPPlayer* pPlayer = static_cast<SPPlayer*>((*RIter));
		if(pPlayer) {
			if(pPlayer->GetPlayerPet() && pPlayer->GetPlayerPet()->IsPreRendering()) {
				if(pPlayer->GetPlayerPet()->GetStandLayer() != BG_ID_LAYER_TERRAIN0 && 
					pPlayer->GetPlayerPet()->GetStandLayer() != BG_ID_LAYER_TERRAIN1)
				{
					;//pPlayer->GetPlayerPet()->Render(false);
				}
			}
		}

		if((*RIter)->SPGOBPerformMessage(SPGM_GETSTANDLAYER) != BG_ID_LAYER_TERRAIN0 && 
			(*RIter)->SPGOBPerformMessage(SPGM_GETSTANDLAYER) != BG_ID_LAYER_TERRAIN1) {
			(*RIter)->Render(fTime);
		}		
	}

	RIter = m_vpGameObjLootingBag.rbegin();
	for(; RIter !=  m_vpGameObjLootingBag.rend(); ++RIter) {
		if((*RIter)->SPGOBPerformMessage(SPGM_GETSTANDLAYER) != BG_ID_LAYER_TERRAIN0 && 
			(*RIter)->SPGOBPerformMessage(SPGM_GETSTANDLAYER) != BG_ID_LAYER_TERRAIN1) {
				(*RIter)->Render(fTime);
		}	
	}

	g_pVideo->Flush();

}

void SPGOBManager::RenderFX( float fTime , SP_FXM_RENDER_LAYER eRenderLayer )
{
	std::vector<SPGameObject*>::reverse_iterator RIter;

	RIter = m_vpGameObjEvent.rbegin();
	for(; RIter != m_vpGameObjEvent.rend(); ++RIter)
	{
		(*RIter)->RenderFX( fTime , eRenderLayer );
	}

	//RIter = m_vpGameObjChatBoard.rbegin();
	//for(; RIter != m_vpGameObjChatBoard.rend(); ++RIter)
	//{
	//	(*RIter)->RenderFX( fTime , eRenderLayer );
	//}

	RIter = m_vpGameObjMonster.rbegin();
	for(; RIter !=  m_vpGameObjMonster.rend(); ++RIter)
	{
		(*RIter)->RenderFX( fTime , eRenderLayer );
	}

	RIter = m_vpGameObjPlayer.rbegin();
	for(; RIter !=  m_vpGameObjPlayer.rend(); ++RIter)
	{
		(*RIter)->RenderFX( fTime , eRenderLayer );
	}

	RIter = m_vpGameObjLootingBag.rbegin();
	for(; RIter !=  m_vpGameObjLootingBag.rend(); ++RIter)
	{
		(*RIter)->RenderFX( fTime , eRenderLayer );
	}
}

// _ALONE only
SPPlayer* SPGOBManager::AddPlayer(float fX, float fY)
{
	SPPlayer* pPlayer = new SPPlayer();	
	pPlayer->SetInstanceID( ++m_LastInstance );
	pPlayer->SetPos(fX, fY);

	// AJJIYA [5/12/2005]
	WPARAM	wParam = MINIMAP_ICON_OTHERPLAYER;
//	if( bLocalPlayer )
//		wParam = 4;

	g_pInterfaceManager->SPChildWndSendMessage( WIID_MINIMAP , SPIM_REFRESH_MINIMAP_GOB , wParam , (LPARAM)pPlayer );	// AJJIYA [10/25/2005]

	m_vpGameObjPlayer.push_back(pPlayer);
	return pPlayer;
}

SPPlayer* SPGOBManager::AddPlayer(CPacket* pPacket, bool bLocalPlayer)
{	
	CHARACTER_INFO2 CharInfo2;
	LT_POSITION		PosInfo;
	STATUS_INFO		StatusInfo;	

	pPacket->ExtractStruct((LPVOID)&CharInfo2,		sizeof(CHARACTER_INFO2));
	INSTANCE_ID InstanceID = GetGUID2InstanceID(CharInfo2.BaseInfo.uiGlobalID);

	//ErrorFLogs->Error_Log("## AddPlayer[%d]", InstanceID ) ;

	if( Find(InstanceID, CLASS_ID_AVATAR) )
		return NULL;

	SPPlayer* pPlayer = new SPPlayer();	
	pPlayer->SetInstanceID(InstanceID);
	pPlayer->SetLocalPlayer(bLocalPlayer);
	pPlayer->Init();
	
	pPlayer->SPGOBPerformMessage(SPGM_SET_SC_CHARINFO,	(WPARAM)&CharInfo2);
	if( bLocalPlayer )
	{
		pPacket->ExtractStruct((LPVOID)&PosInfo, sizeof(LT_POSITION));
		pPacket->ExtractStruct((LPVOID)&StatusInfo,	sizeof(STATUS_INFO));
		
		pPlayer->SPGOBPerformMessage(SPGM_SET_SC_LPPOSITION, (WPARAM)&PosInfo);
		pPlayer->SPGOBPerformMessage(SPGM_SET_SC_STATUSINFO, (WPARAM)&StatusInfo);
		
		MAP_INFO mapInfo = {0,};
		pPacket->ExtractStruct((LPVOID)&mapInfo,sizeof(MAP_INFO));

		bool bSetStage = true;
		if( g_pSubGameManager )
		{
			int iChannelType = 0;
			g_pSubGameManager->SPPerformMessage(SPGM_GET_CHANNEL_CHANGE_TYPE, (WPARAM)&iChannelType);
			if( iChannelType == 2 )
				bSetStage = false;
		}
		
		//[2005/4/13] - jinhee
		pPlayer->SetStage(mapInfo.uiStageID, mapInfo.uiMapGroupID, bSetStage);

		// ¾ÆÀÌÅÛ Á¶ÇÕ Á¤º¸ [9/12/2007 AJJIYA]
		ITEMMIX_INFO	stItemMixInfo;
		pPacket->ExtractStruct( (LPVOID)&stItemMixInfo , sizeof( ITEMMIX_INFO ) );
		pPlayer->SPGOBPerformMessage( SPGM_SET_SC_ITEMMIX_INFO , (WPARAM)&stItemMixInfo );
		
		//[2007/11/28] - jinhee PVP_STATUS
		PVP_STATUS stPvpStat;
		ZeroMemory(&stPvpStat,			sizeof(PVP_STATUS));
		if(SPLocalizeManager::GetInstance()->IsEnableValue(LV_PVP)) {
			pPacket->ExtractStruct(&stPvpStat,	sizeof(PVP_STATUS));
			pPlayer->GetStatus()->SetStatusValue(STATUS_PVP_LV,			stPvpStat.iLv);
			pPlayer->GetStatus()->SetStatusValue(STATUS_PVP_POINT,		stPvpStat.iPoint);
			pPlayer->GetStatus()->SetStatusValueEX(STATUS_EX_PVP_GLORY,	stPvpStat.iGlory);
		}
		
		//[2008/3/12] - jinhee User Cash 
		CHARACTER_CASH_INFO	stCashInfo;
		ZeroMemory(&stCashInfo,			sizeof(CHARACTER_CASH_INFO));		
#ifdef __FIELD_CASH_TRADE__
		pPacket->ExtractStruct(&stCashInfo,	sizeof(CHARACTER_CASH_INFO));
#endif		
		pPlayer->SPGOBPerformMessage(SPGM_SET_SC_CASHINFO, (WPARAM)&stCashInfo);

		// [2008/04/25] - Duragon Auction Reset
		g_pkAuctionManager->SetAuctionReset() ;
	}
	else
	{
		// NAV_SC_INSIGHT [9/13/2007 AJJIYA]

		if( m_pLocalPlayer )
		{
			pPlayer->SetCurStageID(m_pLocalPlayer->GetCurStageID());
			pPlayer->SetCurMapGroupID(m_pLocalPlayer->GetCurMapGroupID());

			if( m_pLocalPlayer->IsTeamMember(CharInfo2.BaseInfo.uiGlobalID) )
				pPlayer->SetGOBNameColorType(1);
		}

		LPARAM lParam = (LPARAM)pPacket;
		pPlayer->SPGOBPerformMessage(SPGM_GOB_MOVE, lParam);
		pPlayer->SPGOBPerformMessage(SPGM_ITEM_SC_FIGURE_INFOS, lParam);
		pPlayer->SetEffectActivity(lParam, false);
		
		//[2006/5/23] ºê·Îµå Ä³½ºÆÃ½Ã »óÁ¡ Á¤º¸
		if(pPlayer->GetCurAction() & ACTION_USERSHOP) {
			UINT8 byType = 0;
			pPacket->ExtractUINT8(&byType);
			char szFMName[FM_MAX_NAME+1]; // »óÁ¡¸í
			ZeroMemory(szFMName, FM_MAX_NAME+1);
			pPacket->ExtractData(szFMName, FM_MAX_NAME+1);
			pPlayer->SetUserShopName(szFMName);
			pPlayer->SetUserShopType(byType);
		}

		if( SPLocalizeManager::GetInstance()->IsEnableValue(LV_GUILD) )
		{
			// ºê·ÎµåÄ³½ºÆ® ±æµåÁ¤º¸
			//typedef struct _GUILD_BROADCAST
			//{
			//	INT32	iGNum;
			//	char	szGName[GUILD_MAX_NAME+1];
			//	UINT8	arrGMark[2];
			//	UINT8	byGType;							// ±æµåÀ¯Çü(1:ÀÏ¹Ý, 2:Ãµ»ç, 3:¾Ç¸¶)
			//} GUILD_BROADCAST, *PGUILD_BROADCAST;
			GUILD_BROADCAST stGuildBroadCast;
			memset(&stGuildBroadCast, 0, sizeof(GUILD_BROADCAST));
			pPacket->ExtractStruct(&stGuildBroadCast, sizeof(GUILD_BROADCAST));
			pPlayer->SPGOBPerformMessage(SPGM_SET_GUILD_INFO, (LPARAM)&stGuildBroadCast);
		}
		
		//[2007/11/28] - jinhee PVP_STATUS
		if(SPLocalizeManager::GetInstance()->IsEnableValue(LV_PVP)) {
			UINT8 iPvpLevel = 0;
			pPacket->ExtractUINT8(&iPvpLevel);
			pPlayer->GetStatus()->SetStatusValue(STATUS_PVP_LV,			iPvpLevel);
		}

		//--------------------------------------------------
		// Pet Info - jmulro 2008/11/24
		//--------------------------------------------------
		//	UINT8		uiPetCount
		// <<repeat
		//	UINT8 slotIndex;			//	³èÎïÔÚ³èÎï×°±¸À¸ÖÐµÄÎ»ÖÃ
		//	ITEMNO itemNo;				//	³èÎïµÄÎ¨Ò»±àºÅ
		//	t_PetAttr_Info Info;		//	³èÎïµÄ»ù±¾ÊôÐÔ
		// <<repeat end
		UINT8 uiPetCount = 0U;
		t_PetAttr_Info attr;
		pPacket->ExtractUINT8( &uiPetCount );
		for (UINT8 idx = 0; idx < uiPetCount; ++idx)
		{
			pPlayer->SPGOBPerformMessage( SPGM_PET_UPDATEINFO, lParam );
		}
		//--------------------------------------------------
		if(SPLocalizeManager::GetInstance()->IsEnableValue( LV_LOVE_COLOR ) )
		{
			UINT8 uiLoveRank = 0U;
			pPacket->ExtractUINT8( &uiLoveRank );
			pPlayer->GetStatus()->SetStatusValue( STATUS_LOVE_LV , (UINT)uiLoveRank );
		}
	}

	pPlayer->SPGOBPerformMessage( SPGM_ITEM_SC_TOGGLE_USE_WEAPON_SET, (WPARAM)CharInfo2.uiWeaponSet + 1);

	// AJJIYA [5/12/2005]
	WPARAM	wParam = MINIMAP_ICON_OTHERPLAYER;
	if( bLocalPlayer )
		wParam = MINIMAP_ICON_MYPLAYER;

	g_pInterfaceManager->SPChildWndSendMessage( WIID_MINIMAP , SPIM_REFRESH_MINIMAP_GOB , wParam , (LPARAM)pPlayer );	// AJJIYA [10/25/2005]

//#ifdef _DEBUG
//	
//	DXUTOutputDebugString("ADD Player : ID[%d] XPos[%.1f] YPos[%.1f]\n",
//		(int)GetGUID2InstanceID(CharInfo2.BaseInfo.uiGlobalID),	pPlayer->GetPosX(), pPlayer->GetPosY());
//#endif

	m_vpGameObjPlayer.push_back(pPlayer);

	if( pPlayer != NULL )
	{	
		const char* pName = pPlayer->GetStatus()->GetGOBName();
		GmOutPutPlayName( pName );
	}
	return pPlayer;
}

SPMonster* SPGOBManager::AddMonster(CPacket* pPacket, CLASS_ID ClassID)
{
	MONSTER_INFO	MonsterInfo;

	LT_POSITION		PosInfo;
	//STATUS_INFO		StatusInfo;

	ZeroMemory(&MonsterInfo,	sizeof(MONSTER_INFO)	);	
	ZeroMemory(&PosInfo,		sizeof(LT_POSITION)		);

	//ErrorFLogs->Error_Log("## AddMonster[%d]", MonsterInfo.BaseInfo.uiGlobalID ) ;

	pPacket->ExtractStruct((LPVOID)&MonsterInfo,		sizeof(MONSTER_INFO));
	INSTANCE_ID InstanceID = GetGUID2InstanceID(MonsterInfo.BaseInfo.uiGlobalID);
	if( Find(InstanceID, ClassID) ) {
		return NULL;
	}

	if( g_pMOBCluster == NULL )
		return NULL;

	SPMonster* pMonster = new SPMonster(InstanceID, ClassID);
	pMonster->SetMonsterType(MonsterInfo.iType);
	pMonster->Init();
	if( m_pLocalPlayer )
	{
		pMonster->SetCurStageID(m_pLocalPlayer->GetCurStageID());
		pMonster->SetCurMapGroupID(m_pLocalPlayer->GetCurMapGroupID());

		SPGOBStatus* pkStatus = m_pLocalPlayer->GetStatus();
		if( pkStatus )
		{
			LPARAM lParam = 0;
			lParam = (((unsigned short)(pkStatus->GetStatusValue(STATUS_LV))) << 16);
			lParam |= ((unsigned short)(MonsterInfo.BaseInfo.uiLevel));
			
			pMonster->SPGOBPerformMessage(SPGM_SET_NAME_COLOR, lParam);
		}

#ifndef _USE_OLD_HP_BAR
		if(pMonster->GetIconIndex() > 3) {
			if(m_pLocalPlayer->GetBeAttackedList()) {
				m_pLocalPlayer->GetBeAttackedList()->AddBossObject(pMonster);
			}
		}
#endif
	}

	pMonster->SPGOBPerformMessage(SPGM_SET_SC_MONSTERINFO, (WPARAM)&MonsterInfo);	
	pMonster->SPGOBPerformMessage(SPGM_GOB_MOVE, (LPARAM)pPacket);

	//pMonster->SPGOBPerformMessage( SPGM_SET_SC_LPPOSITION	,	(WPARAM)&PosInfo	);

	m_vpGameObjMonster.push_back(pMonster);

	SPMOBUIInfo* pUIInfo = g_pMOBCluster->GetMOBUIInfo(ClassID);
	if( pUIInfo )
	{
		pMonster->SetShowStatus(pUIInfo->m_bShowName, pUIInfo->m_bShowHP, pUIInfo->m_bShowShadow);
		if( pUIInfo->m_iRespawnFX )
			pMonster->SPGOBPerformMessage(SPGM_SET_FX_GROUP, pUIInfo->m_iRespawnFX);

		if( !pUIInfo->m_strMOBName.empty() )
			pMonster->SetMOBName(pUIInfo->m_strMOBName.c_str());
	}
	
	return pMonster;
}

SPMonster* SPGOBManager::AddMonster(float fX, float fY, CLASS_ID ClassID)
{
	SPMonster* pkMonster = new SPMonster;
	pkMonster->SetClassID(ClassID);
	pkMonster->Init();
	pkMonster->SetPos(fX, fY);
	
	m_vpGameObjMonster.push_back(pkMonster);

#ifndef _USE_OLD_HP_BAR	
	if(m_pLocalPlayer) {
		if(pkMonster->GetIconIndex() > 3) {
			if(m_pLocalPlayer->GetBeAttackedList()) {
				m_pLocalPlayer->GetBeAttackedList()->AddBossObject(pkMonster);
			}
		}
	}	
#endif

	return pkMonster;
}

SPLootingBag* SPGOBManager::AddLootingBag(CPacket* pPacket, CLASS_ID ClassID, INSTANCE_ID InstanceID)
{
	SPLootingBag* pkLootingBag;
	LT_POSITION	PosInfo;

	GU_ID OwnerPlayerID;
	pPacket->ExtractUINT64(&OwnerPlayerID);
	pPacket->Extract(&PosInfo, sizeof(LT_POSITION));

	if( Find( InstanceID, ClassID) )
		return NULL;
	if( g_pMOBCluster == NULL )
		return NULL;

	pkLootingBag = new SPLootingBag(InstanceID, ClassID);
	pkLootingBag->Init();

	PosInfo.fAccelX = 1600.0f;		// gravity
	PosInfo.fAccelY = 30.0f;		// Maxdrop
	pkLootingBag->SPGOBPerformMessage(SPGM_SET_SC_LPPOSITION, (WPARAM)&PosInfo);
	pkLootingBag->SetOwerID(OwnerPlayerID);

	if( m_pLocalPlayer )
	{
		pkLootingBag->SetCurStageID(m_pLocalPlayer->GetCurStageID());
		pkLootingBag->SetCurMapGroupID(m_pLocalPlayer->GetCurMapGroupID());
	}

	unsigned char ucNum, ucSlot, i;
	ITEM ItemInfo;
	CONTAINER_ITEM ContainerItem;
	
	SPItemAttr* pItemAttr;
	pPacket->ExtractUINT8(&ucNum);
	for( i = 0; i < ucNum; i++ )
	{	
		pPacket->ExtractUINT8(&ucSlot);
		pPacket->Extract(&ItemInfo, sizeof(ITEM));

		g_pItemCluster->GetItemInfo(ItemInfo.iItemID, pItemAttr);
		if( pItemAttr == NULL )
			continue;

		ContainerItem.SlotInfo.SlotIndex = ucSlot;
		ContainerItem.SlotInfo.ContainerType = (CONTAINER_TYPE)(pItemAttr->m_eType - 1);
		ContainerItem.Item = ItemInfo;

		pkLootingBag->AddItem(ContainerItem);
	}
	
	m_vpGameObjLootingBag.push_back(pkLootingBag);
	pkLootingBag->SetAction(ACTION_STOP, true);
	return pkLootingBag;
}

SPEvent* SPGOBManager::AddEvent(CPacket* pPacket, CLASS_ID ClassID, INSTANCE_ID InstanceID)
{
	//MAPEVENT_INFO EventInfo;
	EVENT_INFO EventInfo;
	//ZeroMemory(&EventInfo,		sizeof(EVENT_INFO)	);	
	//pPacket->ExtractStruct((LPVOID)&EventInfo,		sizeof(MAPEVENT_INFO));	
	pPacket->ExtractStruct((LPVOID)&EventInfo,		sizeof(EVENT_INFO));	

	//ErrorFLogs->Error_Log("## AddEvent[%d]", EventInfo.id) ;
	
	if( ClassID >= CLASS_ID_MAPEVENT_FIRST && ClassID <= CLASS_ID_MAPEVENT_LAST) 
	{
		std::vector<SPGameObject*>::iterator Iter;
		Iter = std::find_if(m_vpGameObjEvent.begin(), m_vpGameObjEvent.end(),bind2nd(IsEqualInstance, InstanceID) );
		if(Iter != m_vpGameObjEvent.end()) 
		{
			return (SPEvent*)(*Iter);
		}
	}
	
	if( g_pMOBCluster == NULL )
		return NULL;

	SPEvent* pEvent = new SPEvent(InstanceID, ClassID);
	if(pEvent)
	{
		pEvent->SPGOBPerformMessage(SPGM_EVENT_SC_INFO, (WPARAM)&EventInfo);

		EVENT_TYPE	eEventType	=	EVENT_NULL;

		// AJJIYA [5/12/2005]
		switch( pEvent->GetEventType() )
		{
		case EVENT_NPC:
		case EVENT_SAVEPOINT:
		case EVENT_PORTAL:
			eEventType	=	pEvent->GetEventType();
			break;
		case EVENT_MAILBOX:
			eEventType	=	(EVENT_TYPE)MINIMAP_ICON_MAILBOX;
			break;
		//case EVENT_CROPS:
		//	GetGuildArchive()->GetCropArchive()->SetCropClassID(InstanceID, ClassID) ;
		//	break;
		}

		if( eEventType != EVENT_NULL )
            g_pInterfaceManager->SPChildWndSendMessage( WIID_MINIMAP , SPIM_REFRESH_MINIMAP_GOB , (WPARAM)eEventType , (LPARAM)pEvent );	// AJJIYA [10/25/2005]

		m_vpGameObjEvent.push_back(pEvent);

//#ifdef _DEBUG
//		
//		DXUTOutputDebugString("AddEvent : ClassID=[%d] instanceID=[%d] vpGameObjectEvent.size()=[%d]\n", ClassID,InstanceID,static_cast<int>(m_vpGameObjEvent.size()));
//#endif
	}
	else return NULL;

	if( pEvent->GetEventType() == EVENT_CROPS )	{
		GetGuildArchive()->GetCropArchive()->SetCropClassID(InstanceID, ClassID) ;
		//ErrorFLogs->Error_Log("EVENT_CROPS EventID[%d]", EventInfo.id) ;
	}
	return pEvent;
}


/**
	Event NPCÀÇ ÀÌ¹ÌÁö¸¦ º¯°æ (Crop LevelUp)À» À§ÇÑ ÇÔ¼ö
	¿ÜºÎ¿¡¼­ Á÷Á¢ Ã³¸®ÇÒ¼ö ÀÖ¾î¼­ ÇÔ¼ö Ãß°¡ »ý·«
*/
bool SPGOBManager::UpdateEventNPC(INSTANCE_ID InstanceID, CLASS_ID ClassID, UINT32 ObjectImageID)
{
	SPEvent* pEvent = NULL ;
	pEvent = (SPEvent*)Find(InstanceID, ClassID) ;
	if( pEvent )
		pEvent->UpdateModelNpc(ObjectImageID) ;
	else 
		return false ;
	return true ;
}

bool SPGOBManager::IsChatBoardRect( RECT rcBound , bool bScreenPos )
{
	std::vector< SPGameObject* >::iterator Iter			=	m_vpGameObjChatBoard.begin();
	std::vector< SPGameObject* >::iterator IterEnd		=	m_vpGameObjChatBoard.end();
	SPChatBoard*							pChatBoard	=	NULL;

	if( g_pCameraManagr != NULL && bScreenPos == false )
	{
		SPCameraStruct*	pCameraStruct	=	g_pCameraManagr->GetCameraStruct();

		if( pCameraStruct != NULL )
		{
			rcBound.left	-=	(LONG)pCameraStruct->fOffsetX;
			rcBound.top		-=	(LONG)pCameraStruct->fOffsetY;
			rcBound.right	-=	(LONG)pCameraStruct->fOffsetX;
			rcBound.bottom	-=	(LONG)pCameraStruct->fOffsetY;
		}
	}

	while( Iter != IterEnd )
	{
		pChatBoard	=	(SPChatBoard*)(*Iter);

		if( pChatBoard != NULL )
		{
			if( pChatBoard->IsCursorIn( rcBound ) == true )
				return true;
		}

		++Iter;
	}

	return false;
}

bool SPGOBManager::IsPetShopRect( RECT rtLocal )
{
	std::vector< SPGameObject* >::iterator Iter			=	m_vpGameObjShopPet.begin();
	std::vector< SPGameObject* >::iterator IterEnd		=	m_vpGameObjShopPet.end();
	SPPet	*pPet = NULL;

	if (g_pCameraManagr != NULL)
	{
		SPCameraStruct*	pCameraStruct	=	g_pCameraManagr->GetCameraStruct();

		if( pCameraStruct != NULL )
		{
			rtLocal.left	-=	(LONG)pCameraStruct->fOffsetX;
			rtLocal.top		-=	(LONG)pCameraStruct->fOffsetY;
			rtLocal.right	-=	(LONG)pCameraStruct->fOffsetX;
			rtLocal.bottom	-=	(LONG)pCameraStruct->fOffsetY;
		}
	}

	while( Iter != IterEnd )
	{
		pPet	=	(SPPet*)(*Iter);

		if( pPet != NULL )
		{
			if( pPet->IsCursorIn( rtLocal ) == true )
				return true;
		}

		++Iter;
	}

	return false;
}


bool SPGOBManager::AddGameObject(SPGameObject* pkGameObject, int iObjectType)
{
	if( pkGameObject == NULL )
		return false;

	if( Find(pkGameObject->GetInstanceID(), pkGameObject->GetClassID()) )
		return false;

	switch( iObjectType )
	{
	case 1:
		m_vpGameObjPlayer.push_back(pkGameObject);
		break;
	case 2:
		m_vpGameObjMonster.push_back(pkGameObject);
		break;
	case 3:
		m_vpGameObjEvent.push_back(pkGameObject);
		break;
	}

	return true;
}

bool SPGOBManager::AddChatBoard( CPacket* pPacket , CLASS_ID eClassID , INSTANCE_ID eInstanceID )
{
	if( pPacket == NULL )
		return false;

	DelObject( eInstanceID , eClassID );

	//if( Find( eInstanceID , eClassID ) )
	//	return false;

	SPChatBoard*	pChatBoard	=	new SPChatBoard( eInstanceID , eClassID );

	if( pChatBoard == NULL )
		return false;

	pChatBoard->Init();

	BG_ID	eBGID	=	BG_ID_LAYER_NULL;
	UINT	iPosX	=	0;
	UINT	iPosY	=	0;

	pPacket->ExtractStruct( &eBGID , sizeof( BG_ID ));
	pPacket->ExtractStruct( &iPosX , sizeof( UINT ));
	pPacket->ExtractStruct( &iPosY , sizeof( UINT ));

	pChatBoard->SetStandLayer( (int)eBGID );
	pChatBoard->SetPosX( (float)iPosX );
	pChatBoard->SetPosY( (float)iPosY );

	CHATROOM_OBJECT_INFO	stChatRoomInfo;
	
	ZeroMemory( &stChatRoomInfo , sizeof( CHATROOM_OBJECT_INFO ) );

	pPacket->ExtractStruct( &stChatRoomInfo , sizeof( CHATROOM_OBJECT_INFO ));

	pChatBoard->SetChatRoomInfo( stChatRoomInfo );

	m_vpGameObjChatBoard.push_back( pChatBoard );

	return true;
}

bool SPGOBManager::AddMsgBoard( CPacket* pPacket , CLASS_ID eClassID , INSTANCE_ID eInstanceID )
{
	if( pPacket == NULL )
		return false;

	DelObject( eInstanceID , eClassID );

	//if( Find( eInstanceID , eClassID ) )
	//	return false;

	SPMessageBoard*	pMessageBoard	=	new SPMessageBoard( eInstanceID , eClassID );

	if( pMessageBoard == NULL )
		return false;

	pMessageBoard->Init();

	BG_ID	eBGID	=	BG_ID_LAYER_TERRAIN0;
	UINT    iStageID =  0;
	UINT    iMapGroupID =  0;
	int		iType   =   0;
	UINT	iPosX	=	0;
	UINT	iPosY	=	0;
	size_t  iMsgLen =	0;
	char    szMsg[1024] =   "";
	char    szName[LEN_NAME+1] =   "";
	time_t  uiData;
	

	pPacket->ExtractStruct( &iType , sizeof( int ));
	pPacket->ExtractStruct( &iStageID , sizeof( UINT ));
	pPacket->ExtractStruct( &iStageID , sizeof( UINT ));
	pPacket->ExtractStruct( &iPosX , sizeof( UINT ));
	pPacket->ExtractStruct( &iPosY , sizeof( UINT ));
	pPacket->ExtractStruct( szName , LEN_NAME);
	pPacket->ExtractStruct( &uiData , sizeof( uiData ));
	pPacket->ExtractStruct( &iMsgLen , sizeof( iMsgLen ));
	pPacket->ExtractStruct( szMsg , iMsgLen);

	CTime   tmDate(uiData);
	pMessageBoard->SetStandLayer( (int)eBGID );
	pMessageBoard->SetMsgType(iType);
	pMessageBoard->SetPosX( (float)iPosX );
	pMessageBoard->SetPosY( (float)iPosY );
	pMessageBoard->SetName(szName);
	pMessageBoard->SetDate(tmDate);
	pMessageBoard->SetMsg(szMsg);

	m_vpGameObjMessageBoard.push_back( pMessageBoard );

	return true;
}

//packet << GetGUID();				//	(GU_ID, unsigned __int64)
////	¼ÓÈë³èÎïÉÌµêµÄÄÚÈÝ,ÔÝÊ±¾ÍÕâÑùÉèÖÃ
//packet << m_ShopFrame.nStyleType;	//	(int)
////	ÉÌµêµÄÃû×Ö
//packet.PushBack(m_ShopFrame.szShopName, LEN_NAME);
////	ÉÌ³ÇµÄÎ»ÖÃ
//packet << m_ShopPos.m_iIndunID;		//	UINT32
//packet << m_ShopPos.m_iMapGroupID;	//	UINT
//packet << m_ShopPos.m_iStageID;		//	UINT
//packet << m_ShopPos.m_PosX;			//	INT
//packet << m_ShopPos.m_PosY;			//	INT
//packet << m_ShopPos.m_uiRegionID;	//	UINT32
////	°ÚÌ¯ÉÌµêµÄ³èÎï
//packet << m_petitemID;				//	ITEMID,UINT32
//packet << m_petLevel;				//	INT
//<<³èÎïÃû×Ö
//<<³èÎïÖ÷ÈËÃû×Ö
bool SPGOBManager::AddShopPet(CPacket* pPacket , CLASS_ID eClassID, INSTANCE_ID eInstanceID)
{
	if (!pPacket)
		return false;

	DelObject( eInstanceID, eClassID );

	GU_ID guid = GetGlobalID( eClassID, eInstanceID );
	int nStyleType = 0;
	char szShopName[LEN_NAME+1] = {0,};
	UINT32 iIndunID = 0U;
	int iMapGroupID = 0;
	int iStageID = 0;
	int iPosX = 0;
	int iPosY = 0;
	UINT32 uiRegionID = 0U;
	ITEMID petItemID = 0U;
	int petLevel = 0;
	char szPetName[LEN_NAME+1] = {0,};
	char szPlayerName[LEN_NAME+1] = {0,};

	pPacket->ExtractUINT32( (UINT*)&nStyleType );
	pPacket->ExtractData((TCHAR*)&szShopName, sizeof(TCHAR) * LEN_NAME);
	pPacket->ExtractUINT32( (UINT*)&iIndunID );
	pPacket->ExtractUINT32( (UINT*)&iMapGroupID );
	pPacket->ExtractUINT32( (UINT*)&iStageID );
	pPacket->ExtractUINT32( (UINT*)&iPosX );
	pPacket->ExtractUINT32( (UINT*)&iPosY );
	pPacket->ExtractUINT32( (UINT*)&uiRegionID );
	pPacket->ExtractUINT32( (UINT*)&petItemID );
	pPacket->ExtractUINT32( (UINT*)&petLevel );
	pPacket->ExtractData((TCHAR*)&szPetName, sizeof(TCHAR) * LEN_NAME);
	pPacket->ExtractData((TCHAR*)&szPlayerName, sizeof(TCHAR) * LEN_NAME);

	if (petItemID == 0)
		return false;

	SPPet	*pPet = NULL;
	SPItemAttr* pItemAttr = NULL;
	g_pItemCluster->GetItemInfo(petItemID, pItemAttr);
	if( pItemAttr == NULL )
		return 1;

	if( pItemAttr->m_iPetClassID )
	{
		unsigned int iRealClassID = SPPetLevelMgr::getInstance().GetRealPetClass( petItemID, petLevel );
		if (iRealClassID < (CLASS_ID_PET + 100000))
		{
			pPet = new SPGroundPet(eInstanceID, (CLASS_ID)iRealClassID, NULL);
		}
		else
		{
			pPet = new SPFlyPet(eInstanceID, (CLASS_ID)iRealClassID, NULL);
		}

		if (pItemAttr->m_iNewPet == 1)
			pPet->SetPetGeneration( true );
	}

	pPet->Init();
	pPet->SetCurStageID( iStageID );
	pPet->SetCurMapGroupID( iMapGroupID );
	pPet->SetShopStyle( nStyleType );
	pPet->SetShopGUID( guid );
	pPet->SetShopName( szShopName );
	pPet->SetHostName( szPlayerName );

	LT_POSITION Pos;
	ZeroMemory(&Pos, sizeof(LT_POSITION));
	Pos.fPosX = (float)iPosX;
	Pos.fPosY = (float)iPosY;
	Pos.bgStandLayer = BG_ID_LAYER_TERRAIN0;
	Pos.eFacing = FACING_LEFT; 

	pPet->SPGOBPerformMessage(SPGM_SET_SC_LPPOSITION, (LPARAM)&Pos);
	pPet->SetOpenShopStatus( true );
	m_vpGameObjShopPet.push_back( pPet );
    return true;
}

bool SPGOBManager::DelObject( INSTANCE_ID InstanceID, CLASS_ID ClassID )
{
	std::vector<SPGameObject*>::iterator Iter;

	switch(ClassID) {
	case CLASS_ID_NULL:
	case CLASS_ID_AVATAR:
		{
			//¼ì²éÃ»ÓÐ¶ÁÈ¡µÄÈËÎïÁÐ±í
			std::vector<UINT32>::iterator it = m_vPlayerID.begin();
			for(;it != m_vPlayerID.end();++it)
			{
				if(InstanceID == *it)
				{
					m_vPlayerID.erase(it);
					break;
				}
			}
			//

			Iter = std::find_if(m_vpGameObjPlayer.begin(), 	m_vpGameObjPlayer.end(), 
				bind2nd(IsEqualInstance, InstanceID) );

			if(Iter != m_vpGameObjPlayer.end())
			{
				// AJJIYA [5/12/2005]
				g_pInterfaceManager->SPChildWndSendMessage( WIID_MINIMAP , SPIM_REFRESH_MINIMAP_GOB , (WPARAM)MINIMAP_ICON_DEL_AVATAR , (LPARAM)InstanceID );	// AJJIYA [10/25/2005]

				SAFE_DELETE( *Iter );
				m_vpGameObjPlayer.erase(Iter);
				return true;
			}
				
			if(ClassID == CLASS_ID_AVATAR)
				return false;
		}
		break;

	case CLASS_ID_LOOTING_BAG:
		{
			Iter = std::find_if(m_vpGameObjLootingBag.begin(), 	m_vpGameObjLootingBag.end(), 
				bind2nd(IsEqualInstance, InstanceID) );

			if(Iter != m_vpGameObjLootingBag.end())
			{
				if( m_pLocalPlayer )
					m_pLocalPlayer->SPGOBPerformMessage(SPGM_STAT_LOOTING_DELETE, (LPARAM)(*Iter));

				SAFE_DELETE( *Iter );
				m_vpGameObjLootingBag.erase(Iter);
				return true;
			}

			return false;
		}
		break;
	case CLASS_ID_CHAT_BOARD:
		{
			Iter = std::find_if(m_vpGameObjChatBoard.begin(), 	m_vpGameObjChatBoard.end(), bind2nd(IsEqualInstance, InstanceID) );

			if(Iter != m_vpGameObjChatBoard.end())
			{
				SAFE_DELETE( *Iter );
				m_vpGameObjChatBoard.erase( Iter );
				return true;
			}

			return false;
		}
		break;
	case CLASS_ID_MSGBOARD_OBJECT:
		{
			Iter = std::find_if(m_vpGameObjMessageBoard.begin(), 	m_vpGameObjMessageBoard.end(), bind2nd(IsEqualInstance, InstanceID) );

			if(Iter != m_vpGameObjMessageBoard.end())
			{
				SAFE_DELETE( *Iter );
				m_vpGameObjMessageBoard.erase( Iter );
				return true;
			}

			return false;
		}
		break;
	case CLASS_ID_PET_SHOP_OBJECT:
		{
			Iter = std::find_if( m_vpGameObjShopPet.begin(),
				m_vpGameObjShopPet.end(),
				bind2nd(IsEqualInstance,InstanceID));
			if (Iter != m_vpGameObjShopPet.end())
			{
				SPPet *pet = (SPPet*)(*Iter);
				SPPlayer *player = GetLocalPlayer();
				if (pet && player)
				{
					if (pet->GetShopGUID() == player->GetPetShopID())
					{
						SPWindow *pWnd = g_pInterfaceManager->FindWindow( WIID_USERSHOP_BUY );
						if (pWnd)
							pWnd->Hide();
					}
				}
				SAFE_DELETE( *Iter );
				m_vpGameObjShopPet.erase( Iter );
				return true;
			}
			return false;
		}
		break;
	}
	
	if( ClassID >= CLASS_ID_MOB_FIRST &&
		ClassID <= CLASS_ID_MOB_LAST )
	{
		Iter = std::find_if(m_vpGameObjMonster.begin(), m_vpGameObjMonster.end(), 
			bind2nd(IsEqualInstance, InstanceID) );

		if(Iter != m_vpGameObjMonster.end()) 
		{
			SAFE_DELETE( *Iter );
			m_vpGameObjMonster.erase(Iter);
			return true;
		}
	}
	
	//Event 
	if( ClassID >= CLASS_ID_MAPEVENT_FIRST && ClassID <= CLASS_ID_MAPEVENT_LAST) 
	{
		Iter = std::find_if(m_vpGameObjEvent.begin(), m_vpGameObjEvent.end(),bind2nd(IsEqualInstance, InstanceID) );
		if(Iter != m_vpGameObjEvent.end()) 
		{
			// AJJIYA [5/12/2005]
			g_pInterfaceManager->SPChildWndSendMessage( WIID_MINIMAP , SPIM_REFRESH_MINIMAP_GOB , (WPARAM)MINIMAP_ICON_DEL_EVENT, (LPARAM)InstanceID );	// AJJIYA [10/25/2005]

			SAFE_DELETE( *Iter );
			m_vpGameObjEvent.erase(Iter);
//#ifdef _DEBUG
//			
//			DXUTOutputDebugString("Delete Event : Size[%d] InstanceID=[%d]\n", static_cast<int>(m_vpGameObjEvent.size()),InstanceID);
//#endif
			return true;
		}
	}

	return false;
}

bool SPGOBManager::PurgeAll()
{
	if(m_pLocalPlayer) {
		m_pLocalPlayer->ClearBeAttackedList();
	}	
	
	m_pLocalPlayer = NULL;	// Player »èÁ¦½Ã ¾²·¹±â Ã¼Å© ¾ÈÇØ°Ô²û ¹Ì¸® NULL ¼¼ÆÃ [4/11/2007 AJJIYA]

	std::vector<SPGameObject*>::iterator Iter;

	for(Iter = m_vpGameObjPlayer.begin(); Iter != m_vpGameObjPlayer.end(); ++Iter)
		SAFE_DELETE((*Iter));
	for(Iter = m_vpGameObjMonster.begin(); Iter != m_vpGameObjMonster.end(); ++Iter)
		SAFE_DELETE((*Iter));
	for(Iter = m_vpGameObjLootingBag.begin(); Iter != m_vpGameObjLootingBag.end(); ++Iter)
		SAFE_DELETE((*Iter));
	for(Iter = m_vpGameObjEvent.begin(); Iter != m_vpGameObjEvent.end(); ++Iter)
		SAFE_DELETE((*Iter));
	for(Iter = m_vpGameObjChatBoard.begin(); Iter != m_vpGameObjChatBoard.end(); ++Iter)
		SAFE_DELETE((*Iter));
	for(Iter = m_vpGameObjMessageBoard.begin(); Iter != m_vpGameObjMessageBoard.end(); ++Iter)
		SAFE_DELETE((*Iter));
	for(Iter = m_vpGameObjShopPet.begin(); Iter != m_vpGameObjShopPet.end(); ++Iter)
		SAFE_DELETE((*Iter));

	m_vpGameObjPlayer.clear();
	m_vpGameObjMonster.clear();
	m_vpGameObjLootingBag.clear();
	m_vpGameObjEvent.clear();
	m_vpGameObjChatBoard.clear();
	m_vpGameObjMessageBoard.clear();
	m_vpGameObjShopPet.clear();

	//ÀÌ¶§ QuestManagerÀÇ Archiveµµ ÆÛÁöAll ÇØÁÖÀÚ...
	g_pEventManager->GetQuestManager()->PurgeAll();
	
	//Gift List Á¤¸®...
	g_pEventManager->GetGiftManager()->Clear();

	//Mail List Á¤¸®...
	g_pEventManager->GetMailManager()->Clear();

	//Way Point Á¤º¸ ¸®¼Â...
	g_pEventManager->GetWayPointManager()->ClearWayPoint();

	//°­Á¦ ÀÌº¥Æ® Á¤º¸ ¸®¼Â...
	g_pEventManager->GetCoreEventManager()->RestPlayerMission();

	//	¹Ì´Ï¸Ê¿¡¼­µµ ¸®¼ÂÀ» ¾Ë·ÁÁÖÀÚ~! È¤½Ã³ª~!
	g_pInterfaceManager->SPChildWndSendMessage( WIID_MINIMAP , SPIM_REFRESH_MINIMAP_GOB , (WPARAM)MINIMAP_ICON_DEL_ALL , (LPARAM)0 );	// AJJIYA [10/25/2005]

	//	Äü½½·Ô ¸®¼ÂÀ» ÇÏÀÚ!!
	g_pInterfaceManager->SPChildWndSendMessage( WIID_QUICK_SLOT , SPIM_GET_QUICK_SLOT , NULL , NULL );		// AJJIYA [10/25/2005]

	//	ÄðÅ¸ÀÓ ÇÁ·Î¼¼¼­µµ ¸®¼ÂÀ» ÇÏÀÚ!!
	SPCoolTimeManager::GetInstance()->Clear();

	//m_pLocalPlayer = NULL;
	//[2006/2/17]	DB Block Mode Ç®¾îÁÖÀÚ
	g_pCheckManager->SetDBBlock(false);

	//[2006/2/27] Event Block Mode Ç®¾îÁÖÀÚ
	g_pEventManager->SetEventBlock(false);

	//[2008/4/29] - Ä³½Ã °Å·¡ ºí·° ÇØÁ¦
	SPCashTradeManager::GetInstance()->Reset();

	// Å¸ÀÌÆ²À» ¸®¼ÂÇÏÀÚ [4/5/2006]
	SPTitleArchive*	pTitleArchive	=	SPTitleManager::GetInstance()->GetTitleArchive();

	if( pTitleArchive != NULL )
	{
		pTitleArchive->DelAllTitleData();
	}

	g_pInterfaceManager->SPChildWndSendMessage( WIID_STATUS		, SPIM_PURGE , NULL , NULL );

	return true;
}


bool SPGOBManager::ResetObject()
{	
	if(m_pLocalPlayer) {
		m_pLocalPlayer->ClearBeAttackedList();
	}

	//¸ðµç °ÔÀÓ Object »èÁ¦
	std::vector<SPGameObject*>::iterator Iter;	
	for(Iter = m_vpGameObjMonster.begin(); Iter != m_vpGameObjMonster.end(); ++Iter)
		SAFE_DELETE((*Iter));
	for(Iter = m_vpGameObjLootingBag.begin(); Iter != m_vpGameObjLootingBag.end(); ++Iter)
		SAFE_DELETE((*Iter));
	for(Iter = m_vpGameObjEvent.begin(); Iter != m_vpGameObjEvent.end(); ++Iter)
		SAFE_DELETE((*Iter));
	for(Iter = m_vpGameObjChatBoard.begin(); Iter != m_vpGameObjChatBoard.end(); ++Iter)
		SAFE_DELETE((*Iter));
	for(Iter = m_vpGameObjMessageBoard.begin(); Iter != m_vpGameObjMessageBoard.end(); ++Iter)
		SAFE_DELETE((*Iter));
	for(Iter = m_vpGameObjShopPet.begin(); Iter != m_vpGameObjShopPet.end(); ++Iter)
		SAFE_DELETE((*Iter));

	m_vpGameObjMonster.clear();
	m_vpGameObjLootingBag.clear();
	m_vpGameObjEvent.clear();
	m_vpGameObjChatBoard.clear();
	m_vpGameObjMessageBoard.clear();
	m_vpGameObjShopPet.clear();

	//Local Player¸¦ Á¦¿ÜÇÑ ¸ðµç ÇÃ·¹ÀÌ¾î »èÁ¦
	//»èÁ¦Àü ¸®½ºÆ® 0¹øÀÌ Local PlayerÀÎÁö È®ÀÎÇØÁÖ¾î¾ß ÇÑ´Ù 	
	if( !m_vpGameObjPlayer.empty() )
	{
		for(Iter = m_vpGameObjPlayer.begin() + 1; Iter != m_vpGameObjPlayer.end(); ++Iter)
		{
			SAFE_DELETE((*Iter));
		}
		
		m_vpGameObjPlayer.erase(m_vpGameObjPlayer.begin() + 1, m_vpGameObjPlayer.end());
	}
	
	
	////[2005/9/30] Æ÷Å»½Ã Äù½ºÆ®´Â MainGameManager¿¡¼­ PurgeAllÇØÁØ´Ù
	//g_pEventManager->GetQuestManager()->PurgeAll();

	//	¹Ì´Ï¸Ê¿¡¼­µµ ¸®¼ÂÀ» ¾Ë·ÁÁÖÀÚ~! È¤½Ã³ª~!
	g_pInterfaceManager->SPChildWndSendMessage( WIID_MINIMAP , SPIM_REFRESH_MINIMAP_GOB , (WPARAM)MINIMAP_ICON_DEL_ALL , (LPARAM)0 );		// AJJIYA [10/25/2005]

	//	-3Àº ÀüºÎ »èÁ¦ ±×·¡¼­ ·ÎÄÃ ÇÃ·¹ÀÌ¾î¸¦ ´Ù½Ã ¼ÂÆÃÇÑ´Ù.
	g_pInterfaceManager->SPChildWndSendMessage( WIID_MINIMAP , SPIM_REFRESH_MINIMAP_GOB , (WPARAM)MINIMAP_ICON_MYPLAYER , (LPARAM)m_pLocalPlayer );		// AJJIYA [7/3/2006]

	DXUTOutputDebugString("ResetObject : MonsterListSize[%d]\t", static_cast<int>(m_vpGameObjMonster.size()));
	DXUTOutputDebugString("LootingBagSize[%d]\t", static_cast<int>(m_vpGameObjLootingBag.size()));
	DXUTOutputDebugString("EventListSize[%d]\t", static_cast<int>(m_vpGameObjEvent.size()));
	DXUTOutputDebugString("PlayerListSize[%d]\n", static_cast<int>(m_vpGameObjPlayer.size()));
	DXUTOutputDebugString("ChatBoardSize[%d]\t", static_cast<int>(m_vpGameObjChatBoard.size()));

	return true;
}


int SPGOBManager::SPPerformMessage	(UINT msg, WPARAM wparam, LPARAM lParam)
{
	if(msg >= SPGM_GOBMESSAGE_BEGIN && msg <= SPGM_GOBMESSAGE_END) {
		
		// Game Object Message
		GU_ID guid = *(GU_ID*)wparam;
		CLASS_ID	ClassID;
		INSTANCE_ID InstanceID;
		GetLocalID(guid, ClassID, InstanceID);

		SPGameObject* pGOB = Find( InstanceID, ClassID );
		if(pGOB)
			return pGOB->SPGOBPerformMessage((SPGM)msg, lParam);
	}

	// Game Object Manager Message
	return SPGMESSAGE_FUNC((SPGM)msg, wparam, lParam);	
}


//////////////////////////////////////////////////////////////////////////
//
SPGMESSAGE_MAP_BEGIN(SPGOBManager)
	SPGMESSAGE_COMMAND( SPGM_INIT			, OnInit			)
	SPGMESSAGE_COMMAND( SPGM_PURGEALL		, OnPurgeAll		)
	//SPGMESSAGE_COMMAND( SPGM_ADDGAMEOBJ		, OnAddGameobj		)
	//SPGMESSAGE_COMMAND( SPGM_DELGAMEOBJ		, OnDelGameobj		)
	//SPGMESSAGE_COMMAND( SPGM_SETLOCALPLAYER	, OnSetLocalPlayer	)	
	SPGMESSAGE_COMMAND( SPGM_CHAR_SC_SELECT , OnAddLocalPlayer	)
	SPGMESSAGE_COMMAND( SPGM_GOB_ENTER		, OnAddGameobj		)	
	SPGMESSAGE_COMMAND( SPGM_GOB_LEAVE		, OnDelGameobj		)
	SPGMESSAGE_COMMAND( SPGM_GOB_MOVE		, OnGOBMove			)
	SPGMESSAGE_COMMAND( SPGM_GOB_ENTER_ID	, OnGOBAddObjID		)

	SPGMESSAGE_COMMAND( SPGM_EQUIP_DEFAULT_CHANGE	, OnPlayerEquipDefaultChange	)
	SPGMESSAGE_COMMAND( SPGM_EQUIP_CHANGE			, OnPlayerEquipChange			)

	SPGMESSAGE_COMMAND( SPGM_EVENT_SC_ENTER			, OnEventMessage	)
	SPGMESSAGE_COMMAND( SPGM_EVENT_CS_STAGE_ENTER	, OnSendEnterStageMessage )

	SPGMESSAGE_COMMAND( SPGM_FIGURE_INFOS_BLOCK_SC_ISBLOCKED, OnPlayerBlockExist);
	SPGMESSAGE_COMMAND( SPGM_REFRESH_NAME_COLOR, OnRefreshNameColor);
	
SPGMESSAGE_MAP_END_BASE()

int SPGOBManager::OnInit(WPARAM wparam, LPARAM lparam)
{
	return 0;
}

int SPGOBManager::OnPurgeAll(WPARAM wparam, LPARAM lparam)
{
	PurgeAll();
	return 0;
}

int	SPGOBManager::OnGOBAddObjID(WPARAM wparam, LPARAM lparam)
{
	CPacket* pPacket = (CPacket*)lparam;
	UINT8 i;
	UINT32 id;

	pPacket->ExtractUINT8(&i);
	for(;i>0; --i)
	{
		pPacket->ExtractUINT32(&id);
		m_vPlayerID.push_back(id);
	}

	return 0;
}

int SPGOBManager::OnAddGameobj(WPARAM wparam, LPARAM lparam)
{
	CPacket* pPacket = (CPacket*)lparam;	

	GU_ID guid = *(GU_ID*)wparam;
	CLASS_ID ClassID = GetGUID2ClassID(guid);
	INSTANCE_ID InstanceID  = GetGUID2InstanceID(guid); 

	if( ClassID == CLASS_ID_AVATAR )
	{
		AddPlayer(pPacket);
		// ¾Æ¹ÙÅ¸°¡ ¾ÖµåµÉ¶§ ¿Ö ¹Ì´Ï¸ÊÀ»¼¼ÆÃÇÒ±î? AJJIYA [8/2/2005]
		//if(GetLocalPlayer()) {
		//	int iStage = GetLocalPlayer()->GetCurStageID();
		//	int iGroup = GetLocalPlayer()->GetCurMapGroupID();
		//	g_pInterfaceManager->SPPerformMessage(SPIM_SET_STAGE, (WPARAM)iStage, (LPARAM)iGroup);
		//	g_pMainGameManager->GetEventManager()->LoadEventLDT(iStage, iGroup);
		//	GetLocalPlayer()->GetEventArchive()->LoadEventLDT(iStage, iGroup);
		//}
	}
	else if( ClassID == CLASS_ID_LOOTING_BAG )
		AddLootingBag(pPacket, ClassID, InstanceID);
	else if( ClassID >= CLASS_ID_MOB_FIRST && ClassID <= CLASS_ID_MOB_LAST )
		AddMonster(pPacket, (CLASS_ID)ClassID);
	else if( ClassID >= CLASS_ID_MAPEVENT_FIRST && ClassID <= CLASS_ID_MAPEVENT_LAST )
		AddEvent(pPacket, (CLASS_ID)ClassID, (INSTANCE_ID)InstanceID);
	else if( ClassID == CLASS_ID_CHAT_BOARD )
		AddChatBoard( pPacket , ClassID , InstanceID );
	else if( ClassID == CLASS_ID_MSGBOARD_OBJECT )
		AddMsgBoard( pPacket , ClassID , InstanceID );
	else if (ClassID == CLASS_ID_PET_SHOP_OBJECT)
		AddShopPet( pPacket, ClassID, InstanceID );
	else
	{
		DXUTOutputDebugString("Invalid Object Create : classID[%d]\n", (int)ClassID);
		assert(0 && "Invalid Object Enter");
	}
	

//#ifdef _DEBUG
//	DXUTOutputDebugString("OnAddGameobj : ClassID=[%d] instanceID=[%d]\n",ClassID,InstanceID);
//#endif

	return 0;	
}

int SPGOBManager::OnDelGameobj(WPARAM wparam, LPARAM lparam)
{
	//CPacket* pPacket = (CPacket*)lparam;	

	GU_ID guid = *(GU_ID*)wparam;
	CLASS_ID ClassID;
	INSTANCE_ID InstanceID;
	GetLocalID( guid, ClassID, InstanceID);

//#ifdef _DEBUG
//	DXUTOutputDebugString("OnDelGameObj : ClassID=[%d] instanceID=[%d]\n",ClassID,InstanceID);
//#endif

	if(m_pLocalPlayer) {
		m_pLocalPlayer->DeleteBeAttackedObject(guid);
	}

	return DelObject( InstanceID, ClassID ) ? 0 : -1;
}

int SPGOBManager::OnAddLocalPlayer	(WPARAM wparam, LPARAM lparam)
{
	PurgeAll();

	CPacket* pPacket = (CPacket*)lparam;
	SPPlayer* pPlayer = AddPlayer(pPacket,   true);
	if(pPlayer)
		SetLocalPlayer(pPlayer); 

	return 0;
}


int SPGOBManager::OnAddEnterPlayerEx(WPARAM wparam, LPARAM lparam)
{
	//WILL
	assert(0);
	CPacket* pPacket = (CPacket*)lparam;
	SPPlayer* pPlayer = AddPlayer(pPacket);
	if(pPlayer)
	{
		pPlayer->SPGOBPerformMessage( SPGM_EQUIP_DEFAULT_CHANGE, lparam);
		pPlayer->SPGOBPerformMessage( SPGM_EQUIP_CHANGE, lparam);
	}
	
	return 0;
}

int SPGOBManager::OnGOBMove(WPARAM wparam, LPARAM lparam)
{
	CPacket* pPacket = (CPacket*)lparam;

	CLASS_ID classID;
	CHARID	charID;
	GU_ID guid = 0;
	pPacket->ExtractUINT64(&guid);
	GetLocalID(guid,classID,charID);

	SPGameObject* pPlayer = Find(charID, classID);

	if(pPlayer) {
		return pPlayer->SPGOBPerformMessage( SPGM_GOB_MOVE, lparam);
	} else {
		return -1;
	}	
}

int	SPGOBManager::OnPlayerEquipDefaultChange	(WPARAM wparam, LPARAM lparam)
{
	CPacket* pPacket = (CPacket*)lparam;

	CHARID	charID;
	pPacket->Extract(&charID);

	SPGameObject* pPlayer = Find(charID, CLASS_ID_AVATAR);
	if(pPlayer) {
		return pPlayer->SPGOBPerformMessage( SPGM_EQUIP_DEFAULT_CHANGE, lparam);
	} else {
		return -1;
	}
}

int	SPGOBManager::OnPlayerEquipChange			(WPARAM wparam, LPARAM lparam)
{
	CPacket* pPacket = (CPacket*)lparam;

	CHARID	charID;
	pPacket->Extract(&charID);

	SPGameObject* pPlayer = Find(charID, CLASS_ID_AVATAR);
	if(pPlayer) {
		return pPlayer->SPGOBPerformMessage( SPGM_EQUIP_CHANGE, lparam);
	} else {
		return -1;
	}
}


int SPGOBManager::OnEventMessage(WPARAM wparam, LPARAM lparam)
{
	//ÇØ´ç ¸Þ½ÃÁö´Â ·ÎÄÃ ÇÃ·¹ÀÌ¾îÇÑÅ×¸¸ ¿Â´Ù.
	int iStageID = GetLocalPlayer()->GetCurStageID();
	int iGroupID = GetLocalPlayer()->GetCurMapGroupID();
	SPMapGroup* pMapGroup = NULL;
	
	CPacket* pPacket = (CPacket*)lparam;
	UINT8 eventType;
	UINT32 uiEventID;
	MAP_INFO mapInfo;
	float posX;
	float posY;
	pPacket->ExtractUINT8(&eventType);
	switch(eventType) {
		case EVENT_NPC:
			break;
		case EVENT_SAVEPOINT:
			pPacket->ExtractUINT32(&uiEventID);
#ifdef _DEBUG
			char szTemp[256];
			wsprintf(szTemp, "EVENT_SAVEPOINT OK\n");
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szTemp );					
#endif
			//Check New WayPoint
			if( SPLocalizeManager::GetInstance()->IsEnableValue(LV_WAYPOINT) ) {
				if( g_pEventManager->GetWayPointManager()->SaveNewWayPoint(uiEventID) == true )
				{
					// Only new WayPoint & has StoryViewID -> Show StoryView
					if( g_pkStoryManger->GetStoryViewID() > 0 )	{
						g_pEventManager->SetCutInState(CUT_IN_STATE_NULL) ;
						g_pSubGameManager->SPPerformMessage(SPGM_SET_STORY_VIEW, 1, g_pkStoryManger->GetStoryViewID());
					}
				}
			}
			break;

		case EVENT_PORTAL:
			pPacket->ExtractUINT32(&uiEventID);				//ºÎÈ°½Ã Æ÷Å»¸Þ½ÃÁö EVENT_ID ´Â Save Point IDÀÓ
			pPacket->Extract(&mapInfo,sizeof(MAP_INFO));
			pPacket->Extract(&posX);
			pPacket->Extract(&posY);

			ExecutePotalEvent(mapInfo.uiStageID, mapInfo.uiMapGroupID, posX, posY);
			break;
		case EVENT_QUEST:
#ifdef _DEBUG
			ZeroMemory(szTemp, 256);
			wsprintf(szTemp, "EVENT_QUEST OK\n");
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szTemp );
#endif
			break;
		default:
			//EVENT_NULL ¹× ±×¿Ü...
			break;
	}
	
	g_pInterfaceManager->SPPerformMessage( SPIM_SET_MESSAGE_SHOW, (WPARAM)false );
	return -1;
}


void SPGOBManager::ClearEventQuestFx()
{
	std::vector<SPGameObject*>::iterator iter;
	iter = m_vpGameObjEvent.begin();
	SPEvent* pEvent = NULL;
	for(; iter != m_vpGameObjEvent.end(); ++iter) {
		pEvent = (SPEvent*)(*iter);
		pEvent->ClearEventFX();
	}
}


void SPGOBManager::CheckEventQuestFx()
{
	std::vector<SPGameObject*>::iterator iter;
	iter = m_vpGameObjEvent.begin();
	SPEvent* pEvent = NULL;
	for(; iter != m_vpGameObjEvent.end(); ++iter) {
		pEvent = (SPEvent*)(*iter);
		pEvent->CheckEventQuestShow();
	}
}


bool SPGOBManager::ExecutePotalEvent(int iStage, int iGroup, float fPosX /*= 0.0f*/, float fPosY /*= 0.0f*/)
{
	assert(iStage > -1 && "ExecutePotalEvent Á¸ÀçÇÏÁö ¾Ê´Â Stage\n");

	assert(iGroup > -1 && "ExecutePotalEvent Á¸ÀçÇÏÁö ¾Ê´Â MapGroup\n");
	
	int iCurStageID = GetLocalPlayer()->GetCurStageID();
	int iCurGroupID = GetLocalPlayer()->GetCurMapGroupID();
	SPMapGroup* pMapGroup = NULL;

	g_pEventManager->SetCutInState(CUT_IN_STATE_NULL);					//[2005/10/7]
	g_pEventManager->SetEventPacketSendFlag();							//[2006/1/2]	
	g_pEventManager->SetEventBlock(true);								//[2006/2/22]

	//[2006/12/26] ÇÃ·¹ÀÌ¾î°¡ Æ®·¹ÀÌµå »óÅÂÀÎÃ¤·Î Æ÷Å»ÇÏ´Â °æ¿ì ¿¹¿ÜÃ³¸®
	if(g_pGOBManager->GetLocalPlayer()->GetTradeState() == TRADE_STATE_TRADE) {
		SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_PC_TRADE);
		if(pWindow) {
			pWindow->SPSendMessage(SPIM_TRADE_ITEM_CANCEL, (WPARAM)0, (LPARAM)0);
		}
	}	
	
	//ÀÌ ÀÌÈÄ¿¡... Local PlayerÀÇ StageÁ¤º¸¸¦ ºñ±³ÇÏ¿© Loading À» °áÁ¤ÇØÁÖ°í			
	if(iCurStageID != iStage) {		//Stage º¯°æµÈ °æ¿ì
		DXUTOutputDebugString("EVENT_POTAL : [%d] Stage Change [%d]\n", iCurStageID, iStage);

		g_StageManager.SetNextStage(iStage, iGroup);					//[2005/10/31] - ´ÙÀ½ °¡¾ßÇÒ ½ºÅ×ÀÌÁö Á¤º¸¸¦ ÀÓ½Ã ±â¾ï½ÃÅ²´Ù 
		
		g_pSubGameManager->SPPerformMessage(SPGM_EXIT_STAGE_CHANGE, 0);		//ÀÌ ÀÌÈÄ MainGameManager¿¡¼­ ·ÎµùÀ¸·Î ³Ñ¾î°£´Ù
		//g_pMainGameManager->ChangeSubGame(SUBGAME_LOADING, false);		//·ÎµùÀ¸·Î ³Ñ¾î°¨ (³»ºÎ¿¡¼­ ¸ðµç Object¸¦ »èÁ¦ ÇÑ´Ù)
		//¹Ì´Ï¸Ê ·ÎµùÀº ·ÎµùºÎºÐ¿¡¼­ ÇØÁØ´Ù
		g_pGOBManager->GetLocalPlayer()->SetPetResetPos(true);
	}
	else if(iCurStageID == iStage &&  iCurGroupID != iGroup) {//MapGroup º¯°æµÈ °æ¿ì
		DXUTOutputDebugString("EVENT_POTAL : [%d] Group Change [%d]\n", iCurGroupID, iGroup);
		
		ResetObject();
		GetLocalPlayer()->SetStage(iStage, iGroup);	//À½...
		g_pTerrainManager->GetTerrainCluster()->SetShareLayer();			//[2005/11/8]
		g_pInterfaceManager->SPPerformMessage(SPIM_SET_STAGE, (WPARAM)iStage, (LPARAM)iGroup);		
		
		long lGroupWidth = 0;
		long lGroupHeight = 0;
		std::string strBGM = "";
		int iSoundEffect = 0;
		if(g_StageManager.GetCurStage(iStage)->GetMapGroup(iGroup) != NULL) {
			lGroupWidth = g_StageManager.GetCurStage(iStage)->GetMapGroup(iGroup)->m_lGroupWidth;
			lGroupHeight = g_StageManager.GetCurStage(iStage)->GetMapGroup(iGroup)->m_lGroupHeight;
			strBGM = g_StageManager.GetCurStage(iStage)->GetMapGroup(iGroup)->m_strBGMFile;
			iSoundEffect = g_StageManager.GetCurStage(iStage)->GetMapGroup(iGroup)->m_iSoundEffectType;
		}
		g_pTerrainManager->SetLength(lGroupWidth, lGroupHeight); //ÇØ´ç MapGroupÅ©±â¸¦ Àç¼³Á¤ÇØÁØ´Ù //Stageº¯°æÀº ·ÎµùÇÏ¸é¼­ÀÐÀ¸¹Ç·Î ¹«°üÇÏ´Ù

		//[2006/3/17] - ÀÌº¥Æ® ¸Ê ¿¹¿ÜÃ³¸® Ã¤³ÎÀÌµ¿ ¹öÆ°À» ¸·´Â´Ù
		SPStage* pStage = g_StageManager.GetCurStage(iStage);
		if(pStage) {
			SPMapGroup* pMapGroup = pStage->GetMapGroup(iGroup);
			if(pMapGroup && (pMapGroup->IsMapGroupType(GROUP_TYPE_EVENT) || pMapGroup->IsMapGroupType(GROUP_TYPE_INDUN))) {
				g_pInterfaceManager->SPChildWndSendMessage(WIID_EXIT, SPIM_EXIT_CHANNEL_ENABLE, 0, 0);
			}
			else {
				g_pInterfaceManager->SPChildWndSendMessage(WIID_EXIT, SPIM_EXIT_CHANNEL_ENABLE, 1, 0);
			}

			if(SPLocalizeManager::GetInstance()->IsEnableValue(LV_CASHSHOP) && 
				pMapGroup && pMapGroup->IsMapGroupType(GROUP_TYPE_CASH)) {
				g_pInterfaceManager->SPChildWndSendMessage(WIID_MENU, SPIM_SET_CASH_ENABLE, 1, 0);
			}
			else {
				g_pInterfaceManager->SPChildWndSendMessage(WIID_MENU, SPIM_SET_CASH_ENABLE, 0, 0);
			}
		}

		//[2005/6/10] - BGM Ãâ·Â
		if(strBGM != ""){
			g_pSubGameManager->SPPerformMessage(SPGM_SUG_SETBGM , (WPARAM)strBGM.c_str());
			//
			//DXUTOutputDebugString("GobManager Play BGM[%d][%d] : %s\n", iStage, iGroup, strBGM.c_str());			
		}
		SPSoundBase::GetInstance()->SetRoomType(iSoundEffect);
		//DXUTOutputDebugString("GobManager SetRoomType[%d]\n", iSoundEffect);

		OnSendEnterStageMessage((WPARAM)0, (LPARAM)0);		//·ÎµùÈÄ ¼­¹ö¿¡ ·Îµù ¿Ï·á ¸Þ½ÃÁö Àü¼Û		
		g_pEventManager->SetEventBlock(false);
	}
	else {	//Pos¸¸ º¯°æµÈ °æ¿ì
		OnSendEnterStageMessage((WPARAM)0, (LPARAM)0);		//·ÎµùÈÄ ¼­¹ö¿¡ ·Îµù ¿Ï·á ¸Þ½ÃÁö Àü¼Û
		g_pEventManager->SetEventBlock(false);
	}

	//PlayerÀÇ PositionÀ» Àç ¼³Á¤ÇØÁÖ¾î¾ß ÇÑ´Ù.
	DXUTOutputDebugString("...X[%d] Y[%d] PosChange X[%d] Y[%d]\n", (int)GetLocalPlayer()->GetPosX(), (int)GetLocalPlayer()->GetPosY(),
		(int)fPosX, (int)fPosY);
	
	//GetLocalPlayer()->GetCoordPhysics()->SetCoordPhysicsMap(iStage, iGroup);
	GetLocalPlayer()->SetPos(fPosX, fPosY);

	return true;
}

bool SPGOBManager::CheckMsgBoardOverlap(int iMsgType, int iPosX, int iPosY)
{
	if(GetLocalPlayer() == NULL)
		return false;

	if(iMsgType == 0)
		return false;

	MSG_BOARD * pMsgBoard = g_pGOBCluster->GetMsgBoardByType(iMsgType);
	if ( !pMsgBoard )
		return false;

	float fLocalLeftX = iPosX;
	float fLocalRightX = iPosX + (pMsgBoard->m_iImageX2 - pMsgBoard->m_iImageX1);
	float fLocalTopY = iPosY;
	float fLocalBottomY = iPosY + (pMsgBoard->m_iImageY2 - pMsgBoard->m_iImageY1);

	RECT rtLocal;
	SetRect(&rtLocal, fLocalLeftX, fLocalTopY, fLocalRightX, fLocalBottomY);

	if ( GetLocalPlayer()->GetPosX() >= (fLocalLeftX+400)
		 || GetLocalPlayer()->GetPosX() <= (fLocalLeftX-400)
		 || GetLocalPlayer()->GetPosY() >= (fLocalTopY+400)
		 || GetLocalPlayer()->GetPosY() <= (fLocalTopY-400) )
	{
		return false;
	}

	if( IsChatBoardRect(rtLocal, false) 
		|| IsMsgBoardRect(rtLocal, false) 
		|| IsEventRect(rtLocal, false) )
	{
		return false;
	}
	
	if ( IsTerrainBack(rtLocal) )
	{
		return false;
	}

	return true;
}

bool SPGOBManager::CheckUserShopOverlap(int iShopType /*= 0*/)
{
	if(GetLocalPlayer() == NULL)
		return false;

	if(iShopType == 0)
		return false;
	GetLocalPlayer()->SetUserShopType(iShopType);

	SPUserShopAttr* pLocalShopData = g_pEventManager->GetUserShopArchive()->FindUserShopData(iShopType); //³» ¼¥ Á¤º¸	
	if(pLocalShopData == NULL)
		return false;

	int	iStage = GetLocalPlayer()->GetCurStageID();
	int iMapGroup = GetLocalPlayer()->GetCurMapGroupID();
	float fLocalX = GetLocalPlayer()->GetPosX();
	float fLocalY = GetLocalPlayer()->GetPosY();

	float fLocalLeftX = fLocalX + pLocalShopData->m_rtOverLapRect.left;
	float fLocalRightX = fLocalX + pLocalShopData->m_rtOverLapRect.right;	
	float fLocalTopY = fLocalY + pLocalShopData->m_rtOverLapRect.top;
	float fLocalBottomY = fLocalY + pLocalShopData->m_rtOverLapRect.bottom;

	RECT rtLocal;
	SetRect(&rtLocal, fLocalLeftX, fLocalTopY, fLocalRightX, fLocalBottomY);
	
	if(IsUserShopRect(rtLocal) && IsChatBoardRect( rtLocal , false ) == false && !IsPetShopRect(rtLocal))
		return true;
	
	/*
	std::vector<SPGameObject*>::iterator iter;
	iter = m_vpGameObjPlayer.begin();
	//g_pGOBManager->GetLocalPlayer()->GetCurAction() & ACTION_USERSHOP
	for(; iter != m_vpGameObjPlayer.end(); ++iter) {
		SPPlayer* pPlayer = (SPPlayer*)(*iter);
		if(pPlayer->GetCurAction() & ACTION_USERSHOP) {
			int iPlayerStage = pPlayer->GetCurStageID();
			int iPlayerMapGroup = pPlayer->GetCurMapGroupID();
			float fPlayerX = pPlayer->GetPosX();
			float fPlayerY = pPlayer->GetPosY();
			SPUserShopAttr* pPcShopData = g_pEventManager->GetUserShopArchive()->FindUserShopData(pPlayer->GetUserShopType());		//´Ù¸¥ÀÌÀÇ ¼¥ Á¤º¸¿ë
			if(pPcShopData == NULL)
				continue;

			if(iStage != iPlayerStage || iMapGroup != iPlayerMapGroup)
				continue;
			
			//if((fPlayerX > fLocalX - FM_OTHER_DISTANCE_X && fPlayerX < fLocalX + FM_OTHER_DISTANCE_X) && 
			//	(fPlayerY > fLocalY - FM_OTHER_DISTANCE_Y && fPlayerY < fLocalY + FM_OTHER_DISTANCE_Y))
			//{
			//	return false;
			//}	

			//
			float fLocalLeftX = fLocalX + pLocalShopData->m_rtOverLapRect.left;
			float fLocalRightX = fLocalX + pLocalShopData->m_rtOverLapRect.right;
			float fPlayerLeftX = fPlayerX + pPcShopData->m_rtOverLapRect.left;
			float fPlayerRightX = fPlayerX + pPcShopData->m_rtOverLapRect.right;

			float fLocalTopY = fLocalY + pLocalShopData->m_rtOverLapRect.top;
			float fLocalBottomY = fLocalY + pLocalShopData->m_rtOverLapRect.bottom;
			float fPlayerTopY = fPlayerY + pPcShopData->m_rtOverLapRect.top;
			float fPlayerBottomY = fPlayerY + pPcShopData->m_rtOverLapRect.bottom;
			
			bool bCheckInInsideLeftX = (fLocalLeftX <= fPlayerLeftX && fPlayerLeftX <= fLocalRightX);
			bool bCheckInInsideRightX = (fLocalLeftX <= fPlayerRightX && fPlayerRightX <= fLocalRightX);
			bool bCheckInInsideTopY = (fLocalTopY <= fPlayerTopY && fPlayerTopY <= fLocalBottomY);
			bool bCheckInInsideBottomY = (fLocalTopY <= fPlayerBottomY && fPlayerBottomY <= fLocalBottomY);

			if((bCheckInInsideLeftX || bCheckInInsideRightX) && (bCheckInInsideTopY || bCheckInInsideBottomY)) {
				return false;
			}
			
			////
			//bool bCheckSide = (fPlayerX + pPcShopData->m_rtOverLapRect.right > fLocalX + pLocalShopData->m_rtOverLapRect.left) || 
			//					(fPlayerX + pPcShopData->m_rtOverLapRect.left  < fLocalX + pLocalShopData->m_rtOverLapRect.right);

			//bool bCheckUpDown = (fPlayerY + pPcShopData->m_rtOverLapRect.bottom > fLocalY + pLocalShopData->m_rtOverLapRect.top) ||
			//					(fPlayerY + pPcShopData->m_rtOverLapRect.top < fLocalY + pLocalShopData->m_rtOverLapRect.bottom);
			//
			//if(bCheckSide && bCheckUpDown)  //Áßº¹ÀÌ µÇ´Â °æ¿ì?
			//	return false;
		}
	}
	*/

	return false;
}

bool SPGOBManager::IsMsgBoardRect( RECT rcBound , bool bScreenPos )
{
	std::vector< SPGameObject* >::iterator Iter			=	m_vpGameObjMessageBoard.begin();
	std::vector< SPGameObject* >::iterator IterEnd		=	m_vpGameObjMessageBoard.end();
	SPMessageBoard *					pMsgBoard		=	NULL;

	if( g_pCameraManagr != NULL && bScreenPos == false )
	{
		SPCameraStruct*	pCameraStruct	=	g_pCameraManagr->GetCameraStruct();

		if( pCameraStruct != NULL )
		{
			rcBound.left	-=	(LONG)pCameraStruct->fOffsetX;
			rcBound.top		-=	(LONG)pCameraStruct->fOffsetY;
			rcBound.right	-=	(LONG)pCameraStruct->fOffsetX;
			rcBound.bottom	-=	(LONG)pCameraStruct->fOffsetY;
		}
	}

	while( Iter != IterEnd )
	{
		pMsgBoard	=	(SPMessageBoard*)(*Iter);

		if( pMsgBoard != NULL )
		{
			if( pMsgBoard->IsCursorIn(rcBound) )
				return true;
		}

		++Iter;
	}

	return false;
}

bool SPGOBManager::IsEventRect( RECT rcBound , bool bScreenPos )
{
	std::vector< SPGameObject* >::iterator Iter			=	m_vpGameObjEvent.begin();
	std::vector< SPGameObject* >::iterator IterEnd		=	m_vpGameObjEvent.end();
	SPEvent *					pEvent		=	NULL;

	while( Iter != IterEnd )
	{
		pEvent	=	(SPEvent*)(*Iter);

		if( pEvent != NULL )
		{
			if( pEvent->IsCursorIn(rcBound) )
				return true;
		}

		++Iter;
	}

	return false;
}


bool SPGOBManager::IsTerrainBack( RECT rcLocal )
{
	if ( !g_pTerrainManager )
		return false;

	SPTerrainCluster * pTerrainCluster = g_pTerrainManager->GetTerrainCluster();
	if ( !pTerrainCluster )
		return false;

	POINT  checkPos[3] = {	{rcLocal.left+20,	rcLocal.top+20},
							{rcLocal.left + (rcLocal.right-rcLocal.left)/2,	rcLocal.top + (rcLocal.bottom - rcLocal.top)/2},
							{rcLocal.right-20,	rcLocal.bottom-20},
	};

	for ( int n=0; n < 3; n++ )
	{
		if ( pTerrainCluster->IsTerrainLayer2Back(checkPos[n].x, checkPos[n].y)
			 ||	pTerrainCluster->IsTerrainLayer3Back(checkPos[n].x, checkPos[n].y) )
		{
			return true;
		}
	}


	//SPMapGroup* pMapGroup = g_StageManager.GetMapGroup();
	//if( pMapGroup == NULL )
	//	return false;

	//std::vector<std::vector<SPRotateScrollImage*> > * pBackRotatePicList = g_pTerrainManager->GetBackRotatePicList();

	//if( !pBackRotatePicList || pBackRotatePicList->empty() ) 
	//	return false;

	//if( pMapGroup->m_iBGID < 0 || pMapGroup->m_iBGID >= (int)pBackRotatePicList->size() )
	//	return false;

	//std::vector<SPRotateScrollImage*> pScrollImageList = (*pBackRotatePicList)[pMapGroup->m_iBGID];
	//std::vector<SPRotateScrollImage*>::iterator Iter = pScrollImageList.begin();
	//
	//for(int i; Iter != pScrollImageList.end() && (*Iter)->PicID != BG_ID_LAYER_FOREGROUND0; ++Iter) 
	//{
	//	if( (*Iter) == NULL || (*Iter)->PicID <= BG_ID_LAYER_TERRAIN0 )
	//		continue;

	//	for( i = -1; i < (*Iter)->iResultRotateX; ++i) 
	//	{
	//		if(0 == (*Iter)->pImage)
	//			continue;

	//		int iX = (*Iter)->fResultX + ((i + 1) * (*Iter)->fRotateStepX);
	//		int iY = (*Iter)->fResultY;
	//		int iSX = iX + (*Iter)->pImage->GetLenX() * (*Iter)->fScaleX;
	//		int iSY = iY + (*Iter)->pImage->GetLenY() * (*Iter)->fScaleY;

	//		for ( int n=0; n < 3; n++ )
	//		{
	//			if ( checkPos[n].x >= iX && checkPos[n].x <= iSX
	//				 && checkPos[n].y >= iY && checkPos[n].y <= iSY )
	//				 return true;
	//		}
	//	}

	//	for( i = 0; i < (*Iter)->iResultRotateY; ++i) 
	//	{
	//		if(0 == (*Iter)->pImage)
	//			continue;

	//		int iX = (*Iter)->fResultX;
	//		int iY = (*Iter)->fResultY + ((i+1) * (*Iter)->fRotateStepY);
	//		int iSX = iX + (*Iter)->pImage->GetLenX() * (*Iter)->fScaleX;
	//		int iSY = iY + (*Iter)->pImage->GetLenY() * (*Iter)->fScaleY;

	//		for ( int n=0; n < 3; n++ )
	//		{
	//			if ( checkPos[n].x >= iX && checkPos[n].x <= iSX
	//				 && checkPos[n].y >= iY && checkPos[n].y <= iSY )
	//				 return true;
	//		}
	//										
	//	}
	//}

	return false;
}

bool SPGOBManager::IsUserShopRect(RECT rtLocal)
{
	if(GetLocalPlayer() == NULL)
		return false;
	
	float fLocalLeftX = rtLocal.left;
	float fLocalRightX = rtLocal.right;
	float fLocalTopY = rtLocal.top;
	float fLocalBottomY = rtLocal.bottom;

	int	iStage = GetLocalPlayer()->GetCurStageID();
	int iMapGroup = GetLocalPlayer()->GetCurMapGroupID();
	
	std::vector<SPGameObject*>::iterator iter;

	iter = m_vpGameObjPlayer.begin();
	for(; iter != m_vpGameObjPlayer.end(); ++iter) {
		SPPlayer* pPlayer = (SPPlayer*)(*iter);
		if(pPlayer->GetCurAction() & ACTION_USERSHOP) {
			int iPlayerStage = pPlayer->GetCurStageID();
			int iPlayerMapGroup = pPlayer->GetCurMapGroupID();
			float fPlayerX = pPlayer->GetPosX();
			float fPlayerY = pPlayer->GetPosY();
			SPUserShopAttr* pPcShopData = g_pEventManager->GetUserShopArchive()->FindUserShopData(pPlayer->GetUserShopType());		//´Ù¸¥ÀÌÀÇ ¼¥ Á¤º¸¿ë
			if(pPcShopData == NULL)
				continue;

			if(iStage != iPlayerStage || iMapGroup != iPlayerMapGroup)
				continue;			
			
			float fPlayerLeftX = fPlayerX + pPcShopData->m_rtOverLapRect.left;
			float fPlayerRightX = fPlayerX + pPcShopData->m_rtOverLapRect.right;			
			float fPlayerTopY = fPlayerY + pPcShopData->m_rtOverLapRect.top;
			float fPlayerBottomY = fPlayerY + pPcShopData->m_rtOverLapRect.bottom;

			bool bCheckInInsideLeftX = (fLocalLeftX <= fPlayerLeftX && fPlayerLeftX <= fLocalRightX);
			bool bCheckInInsideRightX = (fLocalLeftX <= fPlayerRightX && fPlayerRightX <= fLocalRightX);
			bool bCheckInInsideTopY = (fLocalTopY <= fPlayerTopY && fPlayerTopY <= fLocalBottomY);
			bool bCheckInInsideBottomY = (fLocalTopY <= fPlayerBottomY && fPlayerBottomY <= fLocalBottomY);

			if((bCheckInInsideLeftX || bCheckInInsideRightX) && (bCheckInInsideTopY || bCheckInInsideBottomY)) {
				return false;
			}			
		}
	}

	return true;
}


int SPGOBManager::OnSendEnterStageMessage(WPARAM wparam, LPARAM lparam)
{
	//DXUTOutputDebugString("Send Enter Stage Message\n");
	
	CPacket Packet;
	Packet.Add((UINT32)NAV_CS_ENTERSTAGE);	

	//ÀÌºÎºÐ ¸®ÅÏ°ª ¹®Á¦ ÀÖÀ»¼ö ÀÖÀ½...
	//ErrorFLogs->Error_Log("SPGOBManager::OnSendEnterStageMessage -> NAV_CS_ENTERSTAGE") ;
	return g_pNetworkManager->SPPerformMessage(NAV_CS_ENTERSTAGE, 0, (LPARAM)&Packet);	

	return -1;
}


int SPGOBManager::OnPlayerBlockExist (WPARAM wParam, LPARAM lParam)
{
	//wParam ÀÌ ·ÎÄÃ Player¿ÍÀÇ GU_ID¿Í µ¿ÀÏÇØ¾ß ÇÑ´Ù... 
	GU_ID iLocalID = *(GU_ID*)wParam;
	UINT32 iItemID = *(UINT32*)lParam;

	if(m_pLocalPlayer && m_pLocalPlayer->GetGUID() != iLocalID) {
		m_iSendPlayerInfo = 0;
		return -1;
	}
	
	if(iItemID == 0) {
		SPPlayer* pPlayer = (SPPlayer*)g_pGOBManager->Find(m_iSendPlayerInfo);

		if(pPlayer) {		
			g_pInterfaceManager->SPPerformMessage(SPIM_SET_PLAYER_INFO, (WPARAM)&m_iSendPlayerInfo);
			g_pInterfaceManager->ShowWindow(WIID_PC_INFO, false);
			g_pInterfaceManager->ShowWindow(WIID_PC_INFO, true);
		}
		else {			
			if(g_pResourceManager->GetGlobalString(5018001)){				//ÇØ´çÇÃ·¹ÀÌ¾î¸¦ Ã£À»¼ö ¾ø½À´Ï´Ù
				std::string strMsg = g_pResourceManager->GetGlobalString(5018001);
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
			}
		}
	}
	else {				//¹«¾ð°¡¿¡ ÀÇÇØ °ÅºÎµÊ...
		if(g_pResourceManager->GetGlobalString(5016006)){					//Á¤º¸º¸±â°¡ °ÅºÎ µÇ¾ú½À´Ï´Ù
			std::string strMsg = g_pResourceManager->GetGlobalString(5016009);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}
	}

	m_iSendPlayerInfo = 0;

	return -1;
}


int SPGOBManager::OnSetLocalPlayer	(WPARAM wparam, LPARAM lparam)
{
	//WILL
	assert(0);
	return -1;
}

//////////////////////////////////////////////////////////////////////////
// Control Event 
int SPGOBManager::WndProc(UINT message, WPARAM wParam, LPARAM lParam)
{	
	if(!isActive()) return 0;

	switch(message)
	{
	case WM_LBUTTONDOWN:
		{
			float fX = (float)LOWORD(lParam);
			float fY = (float)HIWORD(lParam);
			return PerformMouseLDown(fX, fY);
		}		
		break;

	case WM_RBUTTONDOWN:		
		{
			float fX = (float)LOWORD(lParam);
			float fY = (float)HIWORD(lParam);
			return PerformMouseRDown(fX, fY);
		}
		break;

	case WM_LBUTTONUP:		
		{		
			float fX = (float)LOWORD(lParam);
			float fY = (float)HIWORD(lParam);
			return PerformMouseLUp(fX, fY);
		}
		break;

	case WM_RBUTTONUP:
		{
			float fX = (float)LOWORD(lParam);
			float fY = (float)HIWORD(lParam);
			return PerformMouseRUp(fX, fY);
		}		
		break;

	case WM_MOUSEMOVE:
		{
			float fX = (float)LOWORD(lParam);
			float fY = (float)HIWORD(lParam);
			if(!g_pInputManager->GetJoyPadManager()->IsPadMouseMove())
				PerformMouseMove(fX, fY);
			return 0;
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			float fX = (float)LOWORD(lParam);
			float fY = (float)HIWORD(lParam);
			PerformMouseLDBLClick(fX, fY);
			return 1;
		}
		break;

	case WM_RBUTTONDBLCLK:
		{
			float fX = (float)LOWORD(lParam);
			float fY = (float)HIWORD(lParam);
			PerformMouseRDBLClick(fX, fY);
			return 1;
		}
		break;

		//case WM_MOUSEWHEEL:
		//	{
		//	}
		//	break;

	case WM_KEYDOWN:
		{
		}
		break;

	}
	return 0;
}


int SPGOBManager::PerformMouseMove(int iX, int iY)
{
	m_iCursorX = iX;
	m_iCursorY = iY;
	SPWindow*	pWorldMapWindow	=	g_pInterfaceManager->FindWindow( WIID_WORLDMAP );

	std::vector<SPGameObject*>::iterator Iter;
	for(Iter = m_vpGameObjEvent.begin(); Iter != m_vpGameObjEvent.end(); ++Iter) {
		
		if((*Iter)->PerformMouseMove(iX, iY) > 0) {
			if(g_pEventManager->GetCutInState() == CUT_IN_STATE_NULL) {

				// ¿ùµå¸Ê¿¡¼­ ¸¶¿ì½º Ä¿¼­¸¦ »ç¿ëÇÑ´Ù. [4/10/2006]
				if( pWorldMapWindow != NULL )
				{
					if( pWorldMapWindow->IsShow() == false )
					{
						g_pMouseCursor->SetType((SPMouseCursor::MOUSE_TYPE)5);
					}
				}

				//g_pMouseCursor->SetType(SPMouseCursor::MOUSE_TYPE_NPC_TALK);
			}			
			return true;
		}
	}
	
	for(Iter = m_vpGameObjPlayer.begin(); Iter != m_vpGameObjPlayer.end(); ++Iter) {

		if((*Iter)->PerformMouseMove(iX, iY) > 0) {
			if(g_pEventManager->GetCutInState() == CUT_IN_STATE_NULL) {

				// ¿ùµå¸Ê¿¡¼­ ¸¶¿ì½º Ä¿¼­¸¦ »ç¿ëÇÑ´Ù. [4/10/2006]
				if( pWorldMapWindow != NULL )
				{
					if( pWorldMapWindow->IsShow() == false )
					{
						g_pMouseCursor->SetType((SPMouseCursor::MOUSE_TYPE)6);
					}
				}
				//g_pMouseCursor->SetType(SPMouseCursor::MOUSE_TYPE_PC_TALK);
			}
			return true;
		}
	}

	for(Iter = m_vpGameObjChatBoard.begin(); Iter != m_vpGameObjChatBoard.end(); ++Iter) {

		if((*Iter)->PerformMouseMove(iX, iY) > 0) {
			if(g_pEventManager->GetCutInState() == CUT_IN_STATE_NULL) {

				// ¿ùµå¸Ê¿¡¼­ ¸¶¿ì½º Ä¿¼­¸¦ »ç¿ëÇÑ´Ù. [4/10/2006]
				if( pWorldMapWindow != NULL )
				{
					if( pWorldMapWindow->IsShow() == false )
					{
						g_pMouseCursor->SetType((SPMouseCursor::MOUSE_TYPE)7);
					}
				}

				//g_pMouseCursor->SetType(SPMouseCursor::MOUSE_TYPE_NPC_TALK);
			}			
			return true;
		}
	}

	// pet shop mouse move event.
	for(Iter = m_vpGameObjShopPet.begin(); Iter != m_vpGameObjShopPet.end(); ++Iter) {
		if((*Iter)->PerformMouseMove(iX, iY) > 0) {		
			if(g_pEventManager->GetCutInState() == CUT_IN_STATE_NULL) {
				if( pWorldMapWindow != NULL )
				{
					if( pWorldMapWindow->IsShow() == false )
					{
						g_pMouseCursor->SetType((SPMouseCursor::MOUSE_TYPE)7);
					}
				}
			}		
			return true;
		}
	}

	for(Iter = m_vpGameObjMessageBoard.begin(); Iter != m_vpGameObjMessageBoard.end(); ++Iter) {
		if((*Iter)->PerformMouseMove(iX, iY) > 0) {		
			if(g_pEventManager->GetCutInState() == CUT_IN_STATE_NULL) {

				// ¿ùµå¸Ê¿¡¼­ ¸¶¿ì½º Ä¿¼­¸¦ »ç¿ëÇÑ´Ù. [4/10/2006]
				if( pWorldMapWindow != NULL )
				{
					if( pWorldMapWindow->IsShow() == false )
					{
						g_pMouseCursor->SetType((SPMouseCursor::MOUSE_TYPE)7);
					}
				}

				//g_pMouseCursor->SetType(SPMouseCursor::MOUSE_TYPE_NPC_TALK);
			}		
			return true;
		}
	}

	//* ¿©±â¼­ true·Î ³ª°¡´õ¶óµµ »óÀ§¿¡¼­ false ·Î ¸®ÅÏÇÏ°Ô µÇ¾îÀÖÀ½
	// ¿ùµå¸Ê¿¡¼­ ¸¶¿ì½º Ä¿¼­¸¦ »ç¿ëÇÑ´Ù. [4/10/2006]
	if( pWorldMapWindow != NULL )
	{
		if( pWorldMapWindow->IsShow() == false )
		{
			g_pMouseCursor->SetType();
		}
	}

	return false;
}


int SPGOBManager::PerformMouseLDBLClick( int iX, int iY)
{
	std::vector<SPGameObject*>::iterator Iter;

	for(Iter = m_vpGameObjChatBoard.begin(); Iter != m_vpGameObjChatBoard.end(); ++Iter)
	{		
		if((*Iter)->PerformMouseLUp(iX, iY) > 0)
			return true;
	}

	for(Iter = m_vpGameObjMessageBoard.begin(); Iter != m_vpGameObjMessageBoard.end(); ++Iter)
	{	
		SPMessageBoard * pMessageBoard = (SPMessageBoard *)(*Iter);
		if(pMessageBoard->PerformMouseLDBLClick(iX, iY) > 0)
			return true;
	}

	return false;	
}


int SPGOBManager::PerformMouseRDBLClick( int iX, int iY)
{	
	return false;
}


int SPGOBManager::PerformMouseLDown(int iX, int iY)
{	
	return false;
}


int SPGOBManager::PerformMouseLUp(int iX, int iY)
{	
	m_iCursorX = iX;
	m_iCursorY = iY;
	
	//
	if(g_pEventManager->GetCutInState() > CUT_IN_STATE_NULL &&
		g_pEventManager->GetCutInState() < CUT_IN_STATE_RELEASE)
	{
		g_pEventManager->GetEventHandler()->PerformMouseLUp(iX, iY);
	}
	
	std::vector<SPGameObject*>::iterator Iter;
	for(Iter = m_vpGameObjEvent.begin(); Iter != m_vpGameObjEvent.end(); ++Iter) {		
		//SAFE_DELETE((*Iter));
		//if( (*Iter)->IsShow() && (*Iter)->PerformMouseMove(iX, iY))
		//	return 1;

		if((*Iter)->PerformMouseLUp(iX, iY) > 0)
			return true;
	}

	for(Iter = m_vpGameObjPlayer.begin(); Iter != m_vpGameObjPlayer.end(); ++Iter) {
		if((*Iter)->PerformMouseLUp(iX, iY) > 0) {
			GU_ID iID = (*Iter)->GetGUID();
			SPPlayer* pPlayer = (SPPlayer*)g_pGOBManager->Find(iID);

			if(GetLocalPlayer() && (iID != GetLocalPlayer()->GetGUID())) {
				if(pPlayer->GetCurAction() & ACTION_USERSHOP ) {
					//DXUTOutputDebugString("»óÁ¡ ÀÎ ÆÐÅ¶ ¿äÃ»...\n");					
					//pPlayer->SendUserShopIn(iID);
					return GetLocalPlayer()->SendUserShopIn(iID);		//¿äÃ»Àº ³» ÀÚ½Å
				}
			}

			return true;
		}
	}

	//--------------------------------------------------
	// added by jmulro, 2008/11/10, pet shop click event
	for(Iter = m_vpGameObjShopPet.begin(); Iter != m_vpGameObjShopPet.end(); ++Iter) {
		if((*Iter)->PerformMouseLUp(iX, iY) > 0) {
			SPPet *pet = (SPPet*)(*Iter);
			if (pet)
			{
				GU_ID iID = pet->GetShopGUID();
				return GetLocalPlayer()->SendPetShopIn(iID);
			}
			return true;
		}
	}
	//--------------------------------------------------

	//[xialin 2008/09/11]add
	for(Iter = m_vpGameObjMessageBoard.begin(); Iter != m_vpGameObjMessageBoard.end(); ++Iter) {
		if((*Iter)->PerformMouseLUp(iX, iY) > 0) {
			return true;
		}
	}


	return false;
}


int SPGOBManager::PerformMouseRDown(int iX, int iY)
{
	return false;	
}


int SPGOBManager::PerformMouseRUp(int iX, int iY)
{
	m_iCursorX = iX;
	m_iCursorY = iY;
	//³»°¡ °Å·¡ÁßÀÎ °æ¿ì »ó´ë¹æÀÇ Á¤º¸Ã¢À» ¿­¼ö ¾ø´Ù.
	if(GetLocalPlayer()){
		if(GetLocalPlayer()->GetTradeState() == TRADE_STATE_TRADE)
			return false;
	}
	
	std::vector<SPGameObject*>::iterator Iter;
	for(Iter = m_vpGameObjPlayer.begin(); Iter != m_vpGameObjPlayer.end(); ++Iter) {
		//SAFE_DELETE((*Iter));
		//if( (*Iter)->IsShow() && (*Iter)->PerformMouseMove(iX, iY))
		//	return 1;

		if((*Iter)->PerformMouseRUp(iX, iY) > 0)
		{
			if(m_iSendPlayerInfo == 0 ) {		//ÇÃ·¹ÀÌ¾î Á¤º¸º¸±â ¿äÃ»À» ÇÏÁö ¾ÊÀº »óÅÂ¶ó¸é
				//¿©±â¼­ Á¤º¸º¸±â ¿äÃ» ÆÖÅ¶À» º¸³½´Ù...
				
				GU_ID iID = (*Iter)->GetGUID();
				SPPlayer* pPlayer = (SPPlayer*)g_pGOBManager->Find(iID);
				
				if(pPlayer) {
					SPPlayerStatus* pPlayerStatus = (SPPlayerStatus*)pPlayer->GetStatus();
					
					////[2007/2/26] - »ó´ë¹æÀÌ ¿É¼Ç »óÅÂº¸±â °ÅºÎÁß
					//if(pPlayerStatus && pPlayerStatus->IsAlert() == false) {
					//	char szMsg[256];
					//	ZeroMemory(szMsg, 256);
					//	if(g_pResourceManager->GetGlobalString(5017002)) {
					//		sprintf(szMsg, g_pResourceManager->GetGlobalString(5017002), pPlayerStatus->GetGOBName());
					//		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMsg);
					//	}
					//	return true;
					//}

					// If PVP Zone then Block
					if( g_pkPvpManager->IsPvpZone() )
					{
						char	szMsg[256];
						if( g_pResourceManager->GetGlobalString(51000021) )
							wsprintf(szMsg, g_pResourceManager->GetGlobalString(51000021));
						else
							wsprintf(szMsg, "[GOBINFO]You can't use it in PVP zone!!");
						g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMsg);
						return false ;
					}

					char szName[64];
					ZeroMemory(szName, 64);
					pPlayer->SPGOBPerformMessage(SPGM_GETGOBNAME, (LPARAM)szName);
					if(SendPlayerBlockExist(szName))
						m_iSendPlayerInfo =	iID;
				}				
			}
			
			////
			//GU_ID iID = (*Iter)->GetGUID();
			//g_pInterfaceManager->SPPerformMessage(SPIM_SET_PLAYER_INFO, (WPARAM)&iID);
			//g_pInterfaceManager->ShowWindow(WIID_PC_INFO, false);
			//g_pInterfaceManager->ShowWindow(WIID_PC_INFO, true);			
			
			return true;
		}
	}
	return false;
}


bool SPGOBManager::SendPlayerBlockExist(std::string strCharName /*= ""*/)
{
	CPacket Packet(BLOCK_CS_ISBLOCKED);

	//UINT8 len = min(_tcslen(szFriendName),LEN_NAME);
	UINT8 len = static_cast<UINT8>(strCharName.size());
	Packet.AddUINT8(len);
	
	Packet.AddData(strCharName.c_str(),len);
	
	g_pNetworkManager->SPPerformMessage(BLOCK_CS_ISBLOCKED, 0, (LPARAM)&Packet);
	
	return true;
}

int SPGOBManager::OnRefreshNameColor(WPARAM wParam, LPARAM lParam)
{
	if( m_pLocalPlayer == NULL )
		return 0;

	SPGOBStatus* pkPlayerStatus, *pkMonsterStatus;
	pkPlayerStatus = m_pLocalPlayer->GetStatus();
	if( pkPlayerStatus == NULL )
		return 0;
	
	unsigned short usPlayerLevel = (unsigned short)pkPlayerStatus->GetStatusValue(STATUS_LV);
	if( usPlayerLevel == 0 )
		return 0;

	//float fRatio;
	vector<SPGameObject*>::iterator iter = m_vpGameObjMonster.begin();
	while( iter != m_vpGameObjMonster.end() )
	{
		(*iter)->SPGOBPerformMessage(SPGM_GETGOBSTATUS, (LPARAM)&pkMonsterStatus);
		if( pkMonsterStatus )
		{
			LPARAM lParam = 0;
			lParam = (usPlayerLevel << 16);
			lParam |= ((unsigned short)(pkMonsterStatus->GetStatusValue(STATUS_LV)));

			(*iter)->SPGOBPerformMessage(SPGM_SET_NAME_COLOR, lParam);
		}

		++iter;
	}

	return 0;
}

int	SPGOBManager::GmOutPutPlayName( const char* pName )
{
	if ( NULL == pName )
	{
		return 1;
	}
	HINSTANCE hinst;
	hinst = ::LoadLibrary("GmOutPut.dll");

	if( NULL == hinst )
	{
		return 1;
	}
	PNameOutPut output = (PNameOutPut)::GetProcAddress( hinst, "PNameOutPut" );	
	if( NULL == output )
	{	
		::FreeLibrary( hinst );
		return 1;
	}
	output( pName );
	::FreeLibrary( hinst );
	return 1;
}
