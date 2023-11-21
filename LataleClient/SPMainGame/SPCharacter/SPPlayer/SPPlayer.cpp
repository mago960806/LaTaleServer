
#include "SPCommon.h"
#include "SPDebug.h"
#include "SPMainGameDEF.H"
#include "SPGameObjectDef.h"
#include "SPGameObject.h"

#include "SPRenderModelDEF.h"
#include "SPRenderModel.h"

#include "SPGOBClusterDEF.h"
#include "SPGOBCluster.h"
#include "SPGOBModelUnit.h"
#include "SPAvatarModelUnit.h"

#include "SPManager.h"
#include "SPItem.h"
#include "SPItemCluster.h"		//[2005/6/16]
#include "SPGOBManager.h"
#include "SPCameraManager.h"

#include "SPTerrainDEF.h"
#include "SPTerrainCluster.h"
#include "SPTerrainManager.h"
#include "SPInputManager.h"

#include "SPFXDEF.h"
#include "SPFXModel.h"
#include "SPFXUnit.h"
#include "SPFXFrameAnimationUnit.h"
#include "SPFXManager.h"
#include "SPFXDamageUnit.h"
#include "SPFXGroupUnit.h"
#include "SPFXArchive.h"

#include "SPSoundArchive.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPMouseCursor.h"
#include "SPWindowButton.h"
#include "SPWindowEdit.h"
#include "SPNoticeBox.h"
#include "SPInterfaceManager.h"

#include "SPAvatarModelUnitDef.h"
//#include "SPItem.h"
#include "SPItemCluster.h"			//[2005/6/16] - jinhee
#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPItemStatusEquip.h"
#include "SPPlayerEquipInfo.h"
#include "SPGOBCoordPhysics.h"
#include "SPPlayerCoordPhysics.h"
#include "SPGOBStatus.h"
#include "SPPlayerStatus.h"
#include "SPPlayerStatusModel.h"
#include "SPCommandConvert.h"

#include "SPBattle.h"
#include "SPBattleCluster.h"

#include "SPTitleArchive.h"

#include "SPUtil.h"
#include "SPPlayerInvenArchive.h"
#include "SPPlayerEquipDefine.h" // TEST Equip
#include "SPPlayer.h"

#include "SPStage.h"			//[2005/4/13] - jinhee
#include "SPStageManager.h"
#include "SPStageLoader.h"

#include "SPLootingBag.h"

#include "SPEventDEF.h"			//[2005/5/24]
#include "SPEventNpcHandler.h"
#include "SPQuestManager.h"
#include "SPEventManager.h"

#include "SPOptionDef.h"

#include <WinSock2.h>
#include "PacketID.h"
#include "GlobalDefines_Share.h"
#include "Packet.h"
#include "PacketHandler.h"

#include "SPSoundManager.h"
#include "SPSkill.h"
#include "SPSkillArchive.h"
#include "SPMotionStatus.h"
#include "SPSkillManager.h"

#include "SPEffect.h"
#include "SPEffectArchive.h"
#include "SPEffectManager.h"
#include "SPCoolTimeManager.h"
#include "SPComboManager.h"

#include "SPSubGameManager.h"
#include "SPMainGameManager.h"

#include "SPPvpManager.h"

#include "SPCheckManager.h"
#include "SPResourceManager.h"
#include "SPLocalizeManager.h"

#include "SPCashTradeManager.h"

#include "SPPet.h"
#include "SPGroundPet.h"
#include "SPFlyPet.h"

//[2005/4/21] - jinhee
//#include "SPSubGameManager.h"
//#include "SPMainGameManager.h"
//#include "SPGameObjectDEF.h"
//#include "SPEventDEF.h"
//#include "SPEventModel.h"
//#include "SPEventModelStatic.h"
//#include "SPEventModelFrameAnimation.h"
//#include "SPEventUnit.h"
//#include "SPEventManager.h"
//#include "SPEventArchive.h"

#include "SPResourceDef.h"

#include "SPPlayerActionManager.h"		// AJJIYA [7/14/2005]
#include "SPPlayerLayerLoad.h"
#include "SPTeamArchive.h"

#include "SPGuildArchive.h"
#include "SPWindowMiniTeam.h"

#include "SPLoveManager.h"

#include "SPBeAttackedList.h"

#include "SPZone.h"

#include "SPWindowStatic.h"
#include "SPWindowBugleSelect.h"
#include "RTREffect/RTREffectManager.h"
#include "RTREffect/SPPhantom.h"
#include "SPPetManager.h"
#include "SPWindowUserShopBuyer.h"
#include "SPPetLevelMgr.h"
#include "SPWindowPetChat.h"

#include "AutoInterface.h"


const float ACCUMULAT_REAR_EFFECT_TICK = 0.1f;
bool g_bWarningBugleChannel = false;	// ½øÈëÀ®°ÈÆÁµÀ
hash_map<int,int> g_bWarningBugleCoinBegin;	// ¿ªÊ¼µã¾íÀ®°È
bool g_bWarningBugleLastOne = false;	// ·¢Íê×îºóÒ»¸öÀ®°È
bool g_bNeedBugleHighlight = false;		// ÊÇ·ñÐèÒª½øÐÐÀ®°ÈÆµµÀÏû·ÑÒýµ¼
bool g_bPushPropInited = false;			// ÊÇ·ñÒÑ¾­µÃµ½ÁËÍÆËÍÉÌÆ·ÁÐ±í
vector<int> g_vecNotifiers;				// ÒÑ¾­Òýµ¼¹ýµÄÍæ¼ÒÁÐ±í
int iPetChatInterval = 10;				// interval of pet chat
//--------------------------------------------------
// ´¦ÀíÀ®°ÈÆµµÀµÄÌáÊ¾¼ÇÂ¼
//--------------------------------------------------
#include <tinyxml.h>
#include <vector>
#include <algorithm>
using namespace std;
//--------------------------------------------------
bool loadBugleNotifiers( void )
{
	g_vecNotifiers.clear();

	TiXmlDocument	xmlDoc;
	TiXmlElement	*pRoot = NULL;
	TiXmlElement	*pNotifyNode = NULL;
	if (!xmlDoc.LoadFile( "UserOptions.xml", TIXML_ENCODING_UTF8 ))
		return false;

	pRoot = xmlDoc.FirstChildElement();
	if (!pRoot)
		return false;

	pNotifyNode = pRoot->FirstChildElement( _T("Notifiers") );
	if (!pNotifyNode)
		return false;

	TiXmlElement	*pNode = NULL;
	int iValue = -1;
	pNode = pNotifyNode->FirstChildElement( _T("user") );
	while (pNode)
	{
		pNode->QueryIntAttribute( _T("value"), &iValue);
		g_vecNotifiers.push_back( iValue );	
		pNode = pNode->NextSiblingElement();
	}

	TiXmlElement *pPetChatNode = NULL;
	pPetChatNode= pRoot->FirstChildElement( _T("PetChat") );
	if (pPetChatNode)
	{
		pPetChatNode->QueryIntAttribute( _T("Interval"), &iPetChatInterval );
	}
	
	return true;
}
int calcValue( const char *username )
{
	if (!username)
		return -1;
	int len = strlen( username );
	int iValue = -1;
	for (int idx = 0; idx < len; ++idx)
	{
		iValue += username[idx];
	}
	return iValue;
}
bool isPlayerNotified( const char *username )
{
	int iValue = calcValue( username );
	vector<int>::iterator iter = 
		find( g_vecNotifiers.begin(), g_vecNotifiers.end(), iValue );
	if (iter != g_vecNotifiers.end())
		return true;
	else
		return false;
}
bool addNotifier( const char *username )
{
	int iValue = calcValue( username );
	if (-1 == iValue)
		return false;

	TiXmlDocument	xmlDoc;
	TiXmlElement	*pRoot = NULL;
	TiXmlElement	*pNotifyNode = NULL;
	xmlDoc.LoadFile( "UserOptions.xml", TIXML_ENCODING_UTF8 );

	pRoot = xmlDoc.FirstChildElement();
	if (!pRoot)
	{
		pRoot = new TiXmlElement( _T("UserOptions") );
		xmlDoc.LinkEndChild( pRoot );
	}

	pNotifyNode = pRoot->FirstChildElement( _T("Notifiers") );
	if (!pNotifyNode)
	{
		pNotifyNode = new TiXmlElement( _T("Notifiers") );
		pRoot->LinkEndChild( pNotifyNode );
	}

	TiXmlElement	*pNode = new TiXmlElement( _T("user") );
	if (pNode)
	{
		pNode->SetAttribute( _T("value"), iValue );
		pNotifyNode->LinkEndChild( pNode );
		pNode = NULL;
	}

	xmlDoc.SaveFile( "UserOptions.xml" );
	return true;
}
//--------------------------------------------------
//--------------------------------------------------

SPPlayer::SPPlayer()
: SPGameObject()
, m_pGOBModelUnit(NULL)
, m_pPlayerStatus(NULL)
, m_bControlEnable(false)
, m_eCurState(GOB_STATE_NULL)
, m_eLastState(GOB_STATE_NULL)
, m_eCurSeqIndex(GOB_SEQ_STAND)
, m_uiCurAction(ACTION_STOP)
, m_uiLastAction(ACTION_STOP)
, m_uiControlAction(ACTION_NULL)
, m_iSendAction(-1)
, m_pPlayerEquipInfo(NULL)
, m_pkCoordPhysics(NULL)
, m_bLocalPlayer(false)
, m_eLobbyAvatar(LOBBY_NULL)
, m_bStatusRender(true)
, m_pFXArchive(NULL)
, m_pPlayerStatusModel(NULL)
, m_pkCurLootingBag(NULL)
, m_pkPetCurLootingBag(NULL)
, m_pPlayerInventory(NULL)
//, m_pEventArchive(NULL)
//, m_bSendEventPacket(true)
, m_pSoundArchive(NULL)
, m_pkSkillArchive(NULL)
, m_pkRevSkillActivity(NULL)
, m_pkRevSkill(NULL)
, m_pkEffectArchive(NULL)
, m_pkTeamArchive(NULL)
//, m_pkPet(NULL)
, m_iTrade(TRADE_STATE_NULL)
, m_uiCheckIntervalPlayer(0)
, m_uiUserShopPlayer(0)
, m_iUserShopType(0)
, m_bLootingAll(false)
, m_iLootingIndex(0)
, m_iSavePoint(SAVEPOINT_TYPE_MAX)
, m_pPartyFindTexture(NULL)
, m_pClassTexture(NULL)
, m_pBeAttackedList(NULL)
, m_bIsGM( false )
, m_pkRTREffectMgr( NULL )
, m_bTransformMode( false )
, m_pPetMgr( NULL )
, m_fAccumulatePetChat( 0.0f )
, m_iAvatarOffsetX( 0 )
, m_iAvatarOffsetY( 0 )
, m_bWeaponVisible( true )
, m_bTumbleCloudMode( false )
{					  
	//Init();
	m_colorPhantom.a = 1.0f;
	m_colorPhantom.b = 1.0f;
	m_colorPhantom.r = 1.0f;
	m_colorPhantom.g = 1.0f;
}

//--------------------------------------------------
// save pet's skill status
struct _Pet_Skill_Status
{
	_Pet_Skill_Status(void)
	{
		reset();
	}
	void reset( void )
	{
		itemNo = 0;
		memset( status, 0, SST_NUM * sizeof(int) );
	}
	ITEMNO	itemNo;
	int		status[SST_NUM];
};
static _Pet_Skill_Status g_s_PetSkillStatus[PET_PLACE_TYPE_NUMBER];
void resetPetSkillStatus( void )
{
	for (int idx = 0; idx < PET_PLACE_TYPE_NUMBER; ++idx)
	{
		g_s_PetSkillStatus[idx].reset();
	}
}
void _savePetSkillStatus( SPPetManager *petMgr )
{
	if (!petMgr)
		return;

	for (int idx = 0; idx < PET_PLACE_TYPE_NUMBER; ++idx)
	{
		g_s_PetSkillStatus[idx].reset();
		SPPet *pet = petMgr->GetPet( (PET_PLACE)idx );
		if (pet && pet->GetItemNoID() != 0)
		{
			g_s_PetSkillStatus[idx].itemNo = pet->GetItemNoID();
			vector< t_Pet_Skill_To_Client > &vecSkills = pet->GetSkill();
			for (int i = 0; i < (int)vecSkills.size(); ++i)
			{
				t_Pet_Skill_To_Client &skill = vecSkills[i];
				SPSkill *pkSkill = SPSkillManager::GetInstance()->GetSkill( skill.nSkillID );
				SKILL_SPECIAL_TYPE eSpecialType = SST_NULL;
				if (pkSkill)
					eSpecialType = pkSkill->GetSpecialType();
				if (eSpecialType != SST_NULL)
				{
					if (pet->IsToggleOnSpecialSkill( eSpecialType ))
					{
						g_s_PetSkillStatus[idx].status[eSpecialType] = 1;
					}
				}
			}
		}
	}
}
bool isSkillNeedRestore( PET_PLACE iPlace, ITEMNO itemNo, SKILL_SPECIAL_TYPE eSkillType )
{
	if (iPlace <= PET_PLACE_NULL || iPlace >= PET_PLACE_TYPE_NUMBER)
		return false;

	if (g_s_PetSkillStatus[iPlace].itemNo != itemNo)
		return false;

	if (g_s_PetSkillStatus[iPlace].status[eSkillType] == 1)
		return true;
	else
		return false;
		
}
//--------------------------------------------------

SPPlayer::~SPPlayer()
{
#if defined(_LAYER_THREAD_LOAD)
	
	while( !AllUnUsedThread() )
		Sleep(10);
#endif

	if( (m_bLocalPlayer || m_eLobbyAvatar == LOBBY_CASH) && m_pkTeamArchive )
	{
		m_pkTeamArchive->Clear();

		GU_ID iLocalID = GetGUID();
		g_pInterfaceManager->SPPerformMessage(
			SPIM_TEAM_SECEDE, (WPARAM)(m_pkTeamArchive->GetTeamList()), (LPARAM)&iLocalID);

		g_pInterfaceManager->SPPerformMessage(SPIM_TEAM_ICON_UPDATE, 2, 0);
	}

	//--------------------------------------------------
	// save pet's skill status
	if (m_bLocalPlayer)
	{
		_savePetSkillStatus(m_pPetMgr);
	}
	//--------------------------------------------------

	SAFE_DELETE(m_pGOBModelUnit);
	SAFE_DELETE(m_pPlayerStatus);
	SAFE_DELETE(m_pkCoordPhysics);
	SAFE_DELETE(m_pPlayerEquipInfo);	
	SAFE_DELETE(m_pFXArchive);
	SAFE_DELETE(m_pPlayerInventory);
	SAFE_DELETE(m_pPlayerStatusModel);
	SAFE_DELETE(m_pkSkillArchive);
	SAFE_DELETE(m_pkEffectArchive);
	SAFE_DELETE(m_pkTeamArchive);
	SAFE_DELETE(m_ptrPhantomAnim);
	SAFE_DELETE(m_pkRTREffectMgr);
//	SAFE_DELETE(m_pkGuildArchive) ;
	for (int idx = 0; idx < (int)PET_PLACE_TYPE_NUMBER; ++idx)
		ReleasePet((PET_PLACE)idx);
	SAFE_DELETE( m_pPetMgr );
	SAFE_RELEASE(m_pClassTexture);
	SAFE_RELEASE(m_pPartyFindTexture);

	SAFE_DELETE(m_pBeAttackedList);

	//InstanceID ÃÊ±âÈ­ ÇÏÁö ¾ÊÀº Player°¡ Á¸ÀçÇÏ±â ¶§¹®¿¡ GU_ID¸¸ ³Ñ°ÜÁà¼­ ÀÌ¹Ì Áö¿öÁø°Ç ½ºÅµÇÑ´Ù.
	unsigned __int64 ui64Guid = (unsigned __int64)GetGUID();
	SPSoundBase::GetInstance()->FreeArchive(ui64Guid);
	m_pSoundArchive = NULL;	

	if(m_bLocalPlayer) {
		g_pGOBManager->SetLocalPlayer(NULL);
	}

	m_vFigureItemList.clear();

	DelFriendListAll();
}

void SPPlayer::Init()
{
	SPGameObject::Init();

	SetClassID( CLASS_ID_AVATAR );

	m_pPlayerEquipInfo		= new SPPlayerEquipInfo(this);	
	m_pGOBModelUnit			= new SPAvatarModelUnit(this);
	m_pPlayerStatus			= new SPPlayerStatus(this);
	m_pkCoordPhysics		= new SPPlayerCoordPhysics(this);	
	m_pFXArchive			= new SPFXArchive(this);
	m_pPlayerStatusModel	= new SPPlayerStatusModel(this);
	m_pkSkillArchive		= new SPSkillArchive(this);
	m_pkEffectArchive		= new SPEffectArchive(this);
	m_ptrPhantomAnim		= new PhantomAnim();
	m_pkRTREffectMgr		= new RTREffectManager(this);
	m_pPetMgr				= new SPPetManager();
	//m_pkGuildArchive		= new SPGuildArchive ;

	if( m_bLocalPlayer )
	{
		m_pPlayerInventory		= new SPPlayerInvenArchive(this);
		m_pkTeamArchive			= new SPTeamArchive;
	}

	// SPSoundArchiveÀÇ °ü¸®Å°´Â GU_IDÀÌ´Ù.
	if( m_eLobbyAvatar == LOBBY_NULL )
	{
		unsigned __int64 ui64Guid = (unsigned __int64)GetGUID();
		SPSoundBase::GetInstance()->AllocateArchive(ui64Guid, &m_pSoundArchive );
	}
	else
		m_pGOBModelUnit->SPSendMessage(MV_SET_ALLLAYER_PROCESS, 1);
	
	//m_pEventArchive			= new SPEventArchive(this);

	m_pkCoordPhysics->Init();

	m_fCamX = 0.f;
	m_fCamY = 0.f;

	m_iWeaponItemSeq = 1;
	m_iWeaponitemKind = 1;
	m_iActiveWeapon = 1;

	m_bBuckler = false;
	m_bClipping = false;

	m_RunControl.fKeyTermAccmulateTime = 0.0f;
	m_RunControl.fKeyPushAccmulateTime = 0.0f;
	m_RunControl.fKeyTermTime = 0.2f;
	m_RunControl.iDirection = 0;

	// Accumulate Time for Exceptional Animation. 
	// Sequence Time for Exceptional Animation. 
	// Sequence Image for Exceptional Animation. 

	m_pPlayerStatus->SetGender(FEMALE);

	// ÃÊ±â ÀåºñµéÀ» initÇÑ´Ù.
	//for(int i=0; i<EQ_END; i++)
	//{
	//	m_nBattleEquipInfo[i] = m_nFashionEquipInfo[i] = m_nBasicEquipInfo[i] = 0;
	//}
	ZeroMemory(m_nBattleEquipInfo,	sizeof(m_nBattleEquipInfo));
	ZeroMemory(m_nFashionEquipInfo, sizeof(m_nFashionEquipInfo));
	ZeroMemory(m_nBasicEquipInfo,	sizeof(m_nBasicEquipInfo));

	m_pkCurLootingBag = NULL;
	m_pkPetCurLootingBag = NULL;
	SetState(GOB_STATE_STAND);
	//SetAction(ACTION_MOVE_DROP);

	m_bActiveSkill = false;
	m_fSkillEffectDelay = 0.0f;
	m_fSkillAccmulateTime = 0.0f;
	m_iSendSkillID = -1;
	m_iBeforeSkillID = 0;
	m_fSendAccmulateTime = 0.0f;

	m_bLootingAll = false;
	m_iLootingIndex = 0;
	m_fAccumulateLooting = 0.0f;
	m_fCurLootingDelay = 0.1f;

	m_fAccmulatePetCheckLooting = 0.0f;
	m_fAccumulatePetLooting = 0.0f;
	m_fPetLootingDelay = 0.1f;

	m_fAccumulatePetItemUsed = 0.0f;
	m_fPetItemUsedDelay = 1.0f;

	m_fAccumulateItem = 0.0f;
	m_fCurItemDelay = 60.0f;			//1ºÐ¿¡ ÇÑ¹ø¾¿ ÀÌº¥Æ® ½ÇÇà

	m_fFigureAccmulateTime = 0.0f;
	m_fFigureDelayTime = 0.0f;
	m_bActiveWeaponChange = false;

	m_bFacingSend = false;
	m_bWeaponSwapping = false;
	m_uiSendAttackIndex = 0;

	//m_bPetResetPos = false;
	m_bPetResetPos = true;				//[2008/7/7] - ÃÊ±âÈ­½Ã PetÀ§Ä¡¸¦ ÇÃ·¹ÀÌ¾î¿Í µ¿ÀÏÇÑ À§Ä¡·Î ¼¼ÆÃÇÏ°Ô ÇÔ

	std::string strFileName = "DATA/INTERFACE/CONCEPT/UI101.PNG";
	g_pVideo->GetTextureMgr()->LoadTexture( strFileName.c_str(), &m_pPartyFindTexture);
	
	//strFileName = "DATA/INTERFACE/CONCEPT/UI102.PNG";
	strFileName = "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG";
	g_pVideo->GetTextureMgr()->LoadTexture( strFileName.c_str(), &m_pClassTexture );

	m_bRenderPartyFind = false;
	m_iClass = CLASS_A;
	ZeroMemory(m_szLevel, 4);

	SetRect(&m_rtDestPartyFindTail,		0, 0, 0, 0);
	SetRect(&m_rtDestPartyFindTop,		0, 0, 0, 0);
	SetRect(&m_rtDestPartyFindCenter,	0, 0, 0, 0);
	SetRect(&m_rtDestPartyFindBottom,	0, 0, 0, 0);	
	SetRect(&m_rtDestPartyFindLvImg,	0, 0, 0, 0);
	SetRect(&m_rtDestPartyFindLv,		0, 0, 0, 0);
	SetRect(&m_rtDestPartyFindClass,	0, 0, 0, 0);	

	SetRect(&m_rtSrcPartyFindTop,		430, 143, 495, 147);
	SetRect(&m_rtSrcPartyFindCenter,	430, 148, 495, 151);
	SetRect(&m_rtSrcPartyFindBottom,	430, 152, 495, 159);
	SetRect(&m_rtSrcPartyFindTail,		402, 143, 429, 159);
	SetRect(&m_rtSrcPartyFindLvImg,		496, 143, 510, 148);

	SetRect(&m_rtSrcClass[CLASS_NULL],	0, 0, 0, 0);
	////
	//SetRect(&m_rtSrcClass[CLASS_A],		395, 324, 411, 340);		// °ø°ÝÇü
	//SetRect(&m_rtSrcClass[CLASS_B],		412, 324, 428, 340);		// ¹æ¾îÇü
	//SetRect(&m_rtSrcClass[CLASS_C],		395, 307, 411, 323);		// ¸¶¹ýÇü
	//SetRect(&m_rtSrcClass[CLASS_D],		412, 307, 428, 323);		// Á¤ÂûÇü
	//SetRect(&m_rtSrcClass[CLASS_E],		412, 307, 428, 323);		// ¿ö¸®¾î
	//SetRect(&m_rtSrcClass[CLASS_F],		412, 307, 428, 323);		// ±¤Àü»ç
	//SetRect(&m_rtSrcClass[CLASS_G],		412, 307, 428, 323);		// ¼º±â»ç
	//SetRect(&m_rtSrcClass[CLASS_H],		412, 307, 428, 323);		// ¸ùÅ©
	//SetRect(&m_rtSrcClass[CLASS_I],		412, 307, 428, 323);		// ÅÛÇÃ·¯
	//SetRect(&m_rtSrcClass[CLASS_J],		412, 307, 428, 323);		// ¾ÆÆ¼½ºÆ®
	//SetRect(&m_rtSrcClass[CLASS_K],		412, 307, 428, 323);		// Æ®·¹Á®ÇåÅÍ
	//SetRect(&m_rtSrcClass[CLASS_L],		412, 307, 428, 323);		// °Ç³Ê
	SetRect(&m_rtSrcClass[CLASS_A],		161, 141, 177, 157);		// °ø°ÝÇü
	SetRect(&m_rtSrcClass[CLASS_B],		178, 141, 194, 157);		// ¹æ¾îÇü
	SetRect(&m_rtSrcClass[CLASS_C],		195, 141, 211, 157);		// ¸¶¹ýÇü
	SetRect(&m_rtSrcClass[CLASS_D],		212, 141, 228, 157);		// Á¤ÂûÇü
	SetRect(&m_rtSrcClass[CLASS_E],		229, 141, 245, 157);		// ¿ö¸®¾î
	SetRect(&m_rtSrcClass[CLASS_F],		246, 141, 262, 157);		// ±¤Àü»ç
	SetRect(&m_rtSrcClass[CLASS_G],		161, 158, 177, 174);		// ¼º±â»ç
	SetRect(&m_rtSrcClass[CLASS_H],		178, 158, 194, 174);		// ¸ùÅ©
	SetRect(&m_rtSrcClass[CLASS_I],		195, 158, 211, 174);		// ÅÛÇÃ·¯
	SetRect(&m_rtSrcClass[CLASS_J],		212, 158, 228, 174);		// ¾ÆÆ¼½ºÆ®
	SetRect(&m_rtSrcClass[CLASS_K],		229, 158, 245, 174);		// Æ®·¹Á®ÇåÅÍ
	SetRect(&m_rtSrcClass[CLASS_L],		246, 158, 262, 174);		// °Ç³Ê

	m_bCriticalEffect				= false;
	m_fTotalCriticalAccumulate		= 0.0f;
	m_fTotalCriticalDelay			= 1.0f;
	m_fCriticalAccumulate			= 0.0f;
	m_fCriticalDelay				= 0.01f;

	m_fRemainProccessTime           = 0.f;

#if defined(_LAYER_THREAD_LOAD)
	ZeroMemory(m_bUseThread, sizeof(bool) * 4);
#endif
}

void SPPlayer::TriggerEnable(bool bEnable)
{ 
	m_bControlEnable = bEnable; 	
}

SPGOBCoordPhysics* SPPlayer::GetCoordPhysics()	
{ 
	return 	m_pkCoordPhysics;	
}

SPGOBStatus* SPPlayer::GetStatus()			
{ 
	return	m_pPlayerStatus;	
}

SPPlayerInvenArchive* SPPlayer::GetInvenArchive()	
{ 
	return	m_pPlayerInventory;	
}

SPGOBModelUnit* SPPlayer::GetModelUnit()
{
	return m_pGOBModelUnit;
}

SPPlayerEquipInfo* SPPlayer::GetEquipInfo()
{
	return m_pPlayerEquipInfo;
}


void SPPlayer::SetFirstAction()
{
	m_pkCoordPhysics->SetFirstAction();
}

void SPPlayer::InitSendAction()
{
	m_iSendAction = -1;
	m_iSendSkillID = -1;

	m_fSendAccmulateTime = 0.0f;
}

bool SPPlayer::IsWeaponChangeEnable()
{
	GOB_STATE_TYPE eType;
	eType = m_pkCoordPhysics->GetCurState();

	if( eType == GOB_STATE_ATTACK_SPEEDY ||
		eType == GOB_STATE_ATTACK_MIGHTY ||
		eType == GOB_STATE_SKILL )
		return false;

	return true;
}

int SPPlayer::GetEquipItem(EQUIP_ID eID)
{
	int iSlotIndex = eID - 1;
	if( iSlotIndex < 0 )
		return 0;

	if( eID == EQ_WEAPON1 || eID == EQ_BUCKLER1 )
	{
		if( m_iActiveWeapon == 2 )
			iSlotIndex += 2;
	}

	SPItemAttr* pItemAttr;
	int iKind = 0;

	SPItem* pkItem = NULL;
	if( m_bLocalPlayer && m_pPlayerInventory )
	{
		pkItem = m_pPlayerInventory->GetInventoryItem(FIGURE_EQUIP, iSlotIndex);		
		if( pkItem )
		{
			bool bItemAvailable = pkItem->GetItemStatus()->IsTimeAvailable(); //[2006/10/31] ¾ÆÀÌÅÛÀÇ ½Ã°£À¯È¿ °Ë»ç
			pItemAttr = pkItem->GetItemAttr();
			if( pItemAttr && bItemAvailable)
				iKind = pItemAttr->m_eKind;
		}
	}
	else
	{
		if( iSlotIndex >= 0 && iSlotIndex < EQ_END )
		{
			g_pItemCluster->GetItemInfo(m_nBattleEquipInfo[iSlotIndex], pItemAttr);
			if( pItemAttr )
				iKind = pItemAttr->m_eKind;
		}
	}

	return iKind;
}

void SPPlayer::SetState(GOB_STATE_TYPE state, bool bForce)
{
	if( !bForce && m_eCurState == state)
		return;

	m_eLastState = state;
	m_eCurState = state;

	unsigned int uiBeforeAniIndex;
	uiBeforeAniIndex = GetGOBSeqKey(m_iWeaponItemSeq, m_eCurSeqIndex, m_iClass);
	
	m_eCurSeqIndex = GOB_SEQ_NULL;
	m_pGOBModelUnit->SPSendMessage(MV_SET_ACCUMUL_ACTION, 0);
	
	switch( m_eCurState )
	{
	case GOB_STATE_DROP:
		m_eCurSeqIndex = GOB_SEQ_FALL;
		break;

	case GOB_STATE_LAND:
		m_eCurSeqIndex = GOB_SEQ_LAND;
		break;

	case GOB_STATE_STAND:
		m_eCurSeqIndex = GOB_SEQ_STAND;
		break;

	case GOB_STATE_VERTICAL_STAND:
		if( m_pkCoordPhysics->GetMoveVerType() == 0 )
			m_eCurSeqIndex = GOB_SEQ_LADDER_STOP;
		else
			m_eCurSeqIndex = GOB_SEQ_VERROPE_STOP;
		break;

	case GOB_STATE_HANGING_STAND:
		m_eCurSeqIndex = GOB_SEQ_HANGING_STOP;
		break;

	case GOB_STATE_WALK:
		m_eCurSeqIndex = GOB_SEQ_WALK;
		break;

	case GOB_STATE_RUN:
		m_eCurSeqIndex = GOB_SEQ_RUN;
		break;

	case GOB_STATE_RUN_STOP:
		m_eCurSeqIndex = GOB_SEQ_RUN_STOP;
		break;

	case GOB_STATE_UP:
		if( m_pkCoordPhysics->GetMoveVerType() == 0 )
			m_eCurSeqIndex = GOB_SEQ_LADDER_MOVE_UP;
		else
			m_eCurSeqIndex = GOB_SEQ_VERROPE_MOVE_UP;
		break;

	case GOB_STATE_DOWN:
		if( m_pkCoordPhysics->GetMoveVerType() == 0 )
			m_eCurSeqIndex = GOB_SEQ_LADDER_MOVE_DOWN;
		else
			m_eCurSeqIndex = GOB_SEQ_VERROPE_MOVE_DOWN;
		break;

	case GOB_STATE_HANGING:
		m_eCurSeqIndex = GOB_SEQ_HANGING_MOVE;
		break;

	case GOB_STATE_JUMP:
		m_eCurSeqIndex = GOB_SEQ_JUMP;
		break;

	case GOB_STATE_ATTACK_SPEEDY:
		if( m_eAttackRandIndex == GOB_SEQ_JUMP_ATTACK_NORMAL01 ||
			m_eAttackRandIndex == GOB_SEQ_JUMP_ATTACK_HEAVY01 )
		{
			if( m_uiLastAction == ACTION_STOP )
				m_eAttackRandIndex = GOB_SEQ_ATTACK_NORMAL01;
		}

		m_eCurSeqIndex = m_eAttackRandIndex;
		break;

	case GOB_STATE_ATTACK_MIGHTY:
		if( m_eAttackRandIndex == GOB_SEQ_JUMP_ATTACK_NORMAL01 ||
			m_eAttackRandIndex == GOB_SEQ_JUMP_ATTACK_HEAVY01 )
		{
			if( m_uiLastAction == ACTION_STOP )
				m_eAttackRandIndex = GOB_SEQ_ATTACK_HEAVY01;
		}

		m_eCurSeqIndex = m_eAttackRandIndex;
		break;
	
	case GOB_STATE_ATTACKED:
		m_eCurSeqIndex = GOB_SEQ_DAMAGE_NORMAL01;
		break;

	case GOB_STATE_CRITICAL_ATTACKED:
		m_eCurSeqIndex = GOB_SEQ_DAMAGE_CRITICAL;
		break;

	case GOB_STATE_DEAD:
		m_eCurSeqIndex = GOB_SEQ_DEAD;
		break;

	case GOB_STATE_LOOTING:
		m_eCurSeqIndex = GOB_SEQ_LOOTING;
		break;

	case GOB_STATE_LOOTING_STAND:
		m_eCurSeqIndex = GOB_SEQ_LOOTING_STAND;
		break;

	case GOB_STATE_SIT_DOWN:
		m_eCurSeqIndex = GOB_SEQ_SIT_DOWN;
		break;

	case GOB_STATE_SIT_WAIT:
		m_eCurSeqIndex = GOB_SEQ_SIT_WAIT;
		break;

	case GOB_STATE_SIT_STAND:
		m_eCurSeqIndex = GOB_SEQ_SIT_STAND;
		break;

	case GOB_STATE_GUARD:
		m_eCurSeqIndex = GOB_SEQ_GUARD;
		break;

	case GOB_STATE_GUARD_DAMAGE:
		m_eCurSeqIndex = GOB_SEQ_GUARD_DAMAGE;
		break;

	case GOB_STATE_ATTACKED_DOWN:
		m_eCurSeqIndex = GOB_SEQ_DOWN;
		break;
	
	case GOB_STATE_ATTACKED_DOWN_STAND:
		m_eCurSeqIndex = GOB_SEQ_DOWN_STAND;
		break;

	case GOB_STATE_ENCHANTTING:
		m_eCurSeqIndex = GOB_SEQ_ENCHANTTING;
		break;

	case GOB_STATE_ENCHANT_RESULT:
		m_eCurSeqIndex = m_eAttackRandIndex;
		break;

	case GOB_STATE_USERSHOP:
		//m_eCurSeqIndex = GOB_SEQ_USERSHOP01;
		m_eCurSeqIndex = m_eAttackRandIndex;
		break;

	default:
		m_eCurSeqIndex = GOB_SEQ_STAND;
		break;
	}

	SetActionStatus(uiBeforeAniIndex);
}

void SPPlayer::SetAnimationLoop()
{
	bool bLoop = true;
	switch( m_eCurState )
	{
	case GOB_STATE_ATTACK_SPEEDY:
	case GOB_STATE_ATTACK_MIGHTY:
	case GOB_STATE_ATTACKED:
	case GOB_STATE_CRITICAL_ATTACKED:
	case GOB_STATE_DEAD:
	case GOB_STATE_LOOTING:
	case GOB_STATE_LOOTING_STAND:
	case GOB_STATE_SIT_DOWN:
	case GOB_STATE_SIT_STAND:
	case GOB_STATE_RUN_STOP:
	case GOB_STATE_GUARD:
	case GOB_STATE_GUARD_DAMAGE:
	case GOB_STATE_ATTACKED_DOWN:
	case GOB_STATE_ATTACKED_DOWN_STAND:
	case GOB_STATE_LAND:
	case GOB_STATE_JUMP:
	case GOB_STATE_DROP:
	case GOB_STATE_ENCHANT_RESULT:
		bLoop = false;
		break;
	}

	m_pGOBModelUnit->SPSendMessage(MV_SET_ANI_LOOP, bLoop);
}

void SPPlayer::SetActionSound(unsigned int uiKeyValue, bool bSkill, bool bInsert)
{
	SOUND_TABLE_INFO* pInfo;

	if( !m_pSoundArchive )
		return;

	if( bSkill )
		pInfo = SPSoundManager::GetInstance()->GetSoundTable(uiKeyValue);
	else
		pInfo = SPSoundManager::GetInstance()->GetGOBSoundTable(uiKeyValue);

	if( pInfo == NULL )
		return;

	int i;
	string strFullName;
	
	for( i = 0; i < 3; i++ )
	{
		if( pInfo->strFileName[i].empty() )
			continue;

		strFullName = "DATA/SOUND/";
		strFullName += pInfo->strFileName[i];

		if( bInsert )
			m_pSoundArchive->InsertItem(strFullName.c_str(), pInfo->fDelayTime[i], pInfo->bLoop[i]);
		else
			m_pSoundArchive->ForceRelease(strFullName.c_str());
	}
}

bool SPPlayer::SetAction(UINT64 uiAction, bool bForce)
{
	if( !bForce && m_uiCurAction == uiAction)
		return false;

	m_uiLastAction = m_uiCurAction;
	m_uiCurAction = uiAction;
	
	m_pkCoordPhysics->SetAction(uiAction);

	//[2006/7/20] ¾×¼Ç ¹ß»ý½Ã Äù½ºÆ®ÁøÇàÁßÀÎ°ÍÀÎÁö È®ÀÎÇÑ´Ù 
	if(m_bLocalPlayer) {
		g_pEventManager->GetQuestManager()->CheckMissionAction(m_uiCurAction);
	}

	if( m_uiCurAction == ACTION_STOP )
 		SetState(GOB_STATE_STAND, bForce);
	else if( m_uiCurAction == ACTION_VERTICAL_STOP )
		SetState(GOB_STATE_VERTICAL_STAND, bForce);
	else if( m_uiCurAction == ACTION_HANGING_STOP )
		SetState(GOB_STATE_HANGING_STAND, bForce);
	else if( m_uiCurAction & ACTION_RUN_STOP )
		SetState(GOB_STATE_RUN_STOP, bForce);
	else if( m_uiCurAction & ACTION_MOVE_LAND )
		SetState(GOB_STATE_LAND, bForce);
	else if( m_uiCurAction & ACTION_DEAD ) {
		SetState(GOB_STATE_DEAD, bForce);
		if(m_bLocalPlayer)
			SetDeadAction();
	}
	else
	{
		if( m_uiCurAction & ACTION_MOVE_LEFT ||
			m_uiCurAction & ACTION_MOVE_RIGHT )
			SetState(GOB_STATE_WALK, bForce);

		if( m_uiCurAction == ACTION_RUN_LEFT ||
			m_uiCurAction == ACTION_RUN_RIGHT )
			SetState(GOB_STATE_RUN, bForce);
		
		if( m_uiCurAction & ACTION_MOVE_HANGING_LEFT ||
			m_uiCurAction & ACTION_MOVE_HANGING_RIGHT )
			SetState(GOB_STATE_HANGING, bForce);

		if( m_uiCurAction & ACTION_MOVE_UP )
			SetState(GOB_STATE_UP, bForce);
		else if( m_uiCurAction & ACTION_MOVE_DOWN )
			SetState(GOB_STATE_DOWN, bForce);

		if( m_uiCurAction & ACTION_MOVE_JUMP )
			SetState(GOB_STATE_JUMP, bForce);
		else if( m_uiCurAction & ACTION_MOVE_DROP )
			SetState(GOB_STATE_DROP, bForce);
		
		if( m_uiCurAction & ACTION_ATTACK_SPEEDY )
			SetState(GOB_STATE_ATTACK_SPEEDY, bForce);
		else if( m_uiCurAction & ACTION_ATTACK_MIGHTY )
			SetState(GOB_STATE_ATTACK_MIGHTY, bForce);

		if( m_uiCurAction & ACTION_BEATTACKED )
			SetState(GOB_STATE_ATTACKED, bForce);
		else if( m_uiCurAction & ACTION_CRITICAL_BEATTACKED )
		{
			SetState(GOB_STATE_CRITICAL_ATTACKED, bForce);
			if( m_bLocalPlayer == true )
				g_pInterfaceManager->ShowWindow( WIID_WORLDMAP ,SP_HIDE );	// ¿ùµå¸ÊÀ» ´Ý¾ÆÁØ´Ù. [4/18/2006]
		}
		else if( m_uiCurAction & ACTION_BEATTACKED_DOWN )
		{
			SetState(GOB_STATE_ATTACKED_DOWN, bForce);
			if( m_bLocalPlayer == true )
				g_pInterfaceManager->ShowWindow( WIID_WORLDMAP ,SP_HIDE );	// ¿ùµå¸ÊÀ» ´Ý¾ÆÁØ´Ù. [4/18/2006]
		}
		else if( m_uiCurAction & ACTION_BEATTACKED_DOWN_STAND )
			SetState(GOB_STATE_ATTACKED_DOWN_STAND, bForce);

		if( m_uiCurAction & ACTION_LOOTING )
		{
			SetState(GOB_STATE_LOOTING, bForce);
			
			if( m_pkCurLootingBag && m_bLocalPlayer )
			{
				g_pInterfaceManager->SPPerformMessage(
					SPIM_LOOTING_ITEM_SET, (WPARAM)(m_pkCurLootingBag->GetItemList()), (LPARAM)m_pPlayerInventory);
				g_pInterfaceManager->ShowWindow(WIID_LOOTING, SP_SHOW);
			}
		}
		else if( m_uiCurAction & ACTION_LOOTING_STAND )
			SetState(GOB_STATE_LOOTING_STAND, bForce);

		if( m_uiCurAction & ACTION_SIT_DOWN )
			SetState(GOB_STATE_SIT_DOWN, bForce);
		else if( m_uiCurAction & ACTION_SIT_WAIT )
			SetState(GOB_STATE_SIT_WAIT, bForce);
		else if( m_uiCurAction & ACTION_SIT_STAND )
			SetState(GOB_STATE_SIT_STAND, bForce);

		if( m_uiCurAction & ACTION_GUARD )
			SetState(GOB_STATE_GUARD, bForce);
		else if( m_uiCurAction & ACTION_GUARD_DAMAGE )
			SetState(GOB_STATE_GUARD_DAMAGE, bForce);

		if( m_bLocalPlayer )
		{
			if( !((m_uiCurAction & ACTION_LOOTING) || (m_uiCurAction & ACTION_LOOTING_WAIT)) )
			{
				m_pkCurLootingBag = NULL;
				g_pInterfaceManager->ShowWindow(WIID_LOOTING, SP_HIDE);
			}			
		}

		if( m_uiCurAction & ACTION_ENCHANTTING )
			SetState(GOB_STATE_ENCHANTTING, bForce);
		else if( m_uiCurAction & ACTION_ENCHANT_RESULT )
			SetState(GOB_STATE_ENCHANT_RESULT, bForce);
		
		if( m_uiCurAction & ACTION_USERSHOP )
			SetState(GOB_STATE_USERSHOP, bForce);

		m_iSendSkillID = -1;
		if( m_uiCurAction & ACTION_SKILL )
			SetState(GOB_STATE_SKILL, bForce);
	}	
	
	float fTime;
	m_pGOBModelUnit->SPSendMessage(MV_GET_ANI_ACCUMUL_TIME, (WPARAM)&fTime);
	m_pkCoordPhysics->SetActionTime(fTime);

	return true;
}

void SPPlayer::SetActionStatus(unsigned int uiBeforeAniIndex)
{
	m_pFXArchive->DeleteFXGroup( uiBeforeAniIndex );
	SetActionSound(uiBeforeAniIndex, false, false);

	if( m_iBeforeSkillID )
	{
		SPSkill* pkSkill = SPSkillManager::GetInstance()->GetSkill(m_iBeforeSkillID);
		if( pkSkill )
		{
			SKILL_MOTION* pMotion = pkSkill->GetMotion(m_bBuckler);
			if( pMotion )
			{
				m_pFXArchive->DeleteFXGroup((SPID_FX_GROUP)pMotion->iFXKeyIndex);
				SetActionSound(pMotion->iSoundKeyIndex, true, false);
			}
		}
		
		m_iBeforeSkillID = 0;
	}
	
	
	unsigned int uiAniIndex = 0;
	if( m_eCurState != GOB_STATE_SKILL )
	{
		uiAniIndex = GetGOBSeqKey(m_iWeaponItemSeq, m_eCurSeqIndex, m_iClass);
		m_pGOBModelUnit->SPSendMessage(MV_SET_ANIMATION_INDEX, uiAniIndex);

		SetAnimationLoop();
		SetActionSound(uiAniIndex);

		//if( m_eCurSeqIndex == GOB_STATE_RUN )
		//	m_pFXArchive->SetContinue( TRUE );


		m_pFXArchive->InsertFXGroup(uiAniIndex);			// ÇöÀç ½ÃÄö½º »óÅÂ¿¡ ¸Â´Â FX ±×·ì È£Ãâ!!! AJJIYA [5/16/2005]


		//if( uiBeforeAniIndex )
		//	m_pkSkillArchive->RemoveUsedMotion(uiBeforeAniIndex);

		SPMotionStatus* pkStatus = SPSkillManager::GetInstance()->GetMotionStatus(uiAniIndex);
		if( pkStatus )
		{
			//SKILL_APPLY_EFFECT* pApplyEffect = pkStatus->GetApplyEffect();
			//if( pApplyEffect->eTargetType == STT_ENEMY )
			//{
			//	SPMotionUsed* pkMotionUsed = new SPMotionUsed(this, pkStatus, uiAniIndex);
			//	if( !m_pkSkillArchive->UsedMotion(pkMotionUsed) )
			//		delete pkMotionUsed;
			//}

			SKILL_ACTIVITY_VELOCITY* pActivityVel = pkStatus->GetActivityVelocity();
			if( pActivityVel->fVelocityX || pActivityVel->fVelocityY )
			{
				ACTIVE_VEL_INFO Info;
				ZeroMemory(&Info, sizeof(ACTIVE_VEL_INFO));

				Info.bEnable = true;
				Info.fVelocityY = pActivityVel->fVelocityY;
				Info.fTime = pActivityVel->fTime;

				if( m_pkCoordPhysics->GetDirection() == FACING_LEFT )
					Info.fVelocityX = -pActivityVel->fVelocityX;
				else
					Info.fVelocityX = pActivityVel->fVelocityX;

				Info.bDelay = true;
				Info.fDelayTime = pActivityVel->fDelayTime;

				m_pkCoordPhysics->SetActicityVelocity(Info);
			}

			if( m_bLocalPlayer )
			{
				SKILL_COMBO* pCombo = pkStatus->GetComboInfo();
				SPComboManager::GetInstance()->Clear();
				if( pCombo->iApplyComboID )
					SPComboManager::GetInstance()->AddCombo(pCombo->iApplyComboID, *pCombo);
			}
		}
	}
	else
	{
		if( m_pkRevSkill )
		{
			SKILL_MOTION* pMotion = m_pkRevSkill->GetMotion(m_bBuckler);
			if( pMotion )
			{ 
				m_pGOBModelUnit->SPSendMessage(MV_SET_ANIMATION_INDEX, pMotion->iSeqKeyIndex);
				m_pFXArchive->InsertFXGroup((SPID_FX_GROUP)pMotion->iFXKeyIndex);
				m_pGOBModelUnit->SPSendMessage(MV_SET_ANI_LOOP, false);
				SetActionSound(pMotion->iSoundKeyIndex, true);
			}

			//SPSkillUsed* pkSkillUsed = new SPSkillUsed(this, m_pkRevSkill);
			//if( !m_pkSkillArchive->UsedSkill(pkSkillUsed) )
			//	delete pkSkillUsed;

			if( m_bLocalPlayer )
			{
				int iSkillLevel = m_pkRevSkillActivity->GetLevel();
				int iCoolTimeSize = m_pkRevSkill->GetCoolTimeSize();
				SKILL_COOLTIME* pCoolTime;

				for( int i = 0; i < iCoolTimeSize; i++ )
				{
					pCoolTime = m_pkRevSkill->GetCoolTime(i);
					if( pCoolTime == NULL )
						continue;

					float fCoolTime = pCoolTime->fTime + pCoolTime->fTimeSlv * iSkillLevel;
					ApplyCoolTime(pCoolTime->iApplyID, fCoolTime , 0.0f );
				}

				SKILL_COMBO* pCombo = m_pkRevSkill->GetCombo();
				SPComboManager::GetInstance()->Clear();

				if( pCombo->iApplyComboID )
					SPComboManager::GetInstance()->AddCombo(pCombo->iApplyComboID, *pCombo);

				if( (m_uiLastAction & ACTION_ATTACK_SPEEDY) ||
					(m_uiLastAction & ACTION_ATTACK_MIGHTY) ||
					(m_uiLastAction & ACTION_SKILL) )
				{
//					m_pFXArchive->InsertFXGroup((SPID_FX_GROUP)5002);
				}
			}

			SKILL_ACTIVITY_VELOCITY* pActivityVel = m_pkRevSkill->GetActivityVelocity();
			if( pActivityVel->fVelocityX || pActivityVel->fVelocityY )
			{
				ACTIVE_VEL_INFO Info;
				ZeroMemory(&Info, sizeof(ACTIVE_VEL_INFO));

				Info.bEnable = true;
				Info.fVelocityY = pActivityVel->fVelocityY;
				Info.fTime = pActivityVel->fTime;

				if( m_pkCoordPhysics->GetDirection() == FACING_LEFT )
					Info.fVelocityX = -pActivityVel->fVelocityX;
				else
					Info.fVelocityX = pActivityVel->fVelocityX;

				Info.bDelay = true;
				Info.fDelayTime = pActivityVel->fDelayTime;

				m_pkCoordPhysics->SetActicityVelocity(Info);
			}

			m_iBeforeSkillID = m_pkRevSkill->GetID();
		}
		
	}	
}

void SPPlayer::SetWeaponSeqItem()
{
	int iSeqNumber = 1;
	m_iWeaponitemKind = 0;
	m_bBuckler = false;

	SPItemStatus* pkStatus = m_pPlayerEquipInfo->GetItemStatus(EQ_BUCKLER1);
	if( pkStatus && pkStatus->GetItemAttr() )
		m_bBuckler = true;

	SPItemAttr* pItemAttr = NULL;
	pkStatus = m_pPlayerEquipInfo->GetItemStatus(EQ_WEAPON1);
	if( pkStatus && (pItemAttr = pkStatus->GetItemAttr()) )
	{
		if( m_bBuckler )
			iSeqNumber = pItemAttr->m_iMotionTypeID2;
		else
			iSeqNumber = pItemAttr->m_iMotionTypeID1;

		m_iWeaponitemKind = pItemAttr->m_eKind;
	}
	else if( m_bBuckler )
		iSeqNumber = 2;

	if( m_iWeaponItemSeq != iSeqNumber )
	{
		//	AJJIYA [12/7/2005]
		//	¹«±â°¡ ¹Ù²î±âÀü ÀÌÀü ½ÃÄö½º °ª ÀúÀå.
		unsigned int	uiBeforeAniIndex	=	GetGOBSeqKey(m_iWeaponItemSeq, m_eCurSeqIndex, m_iClass);

		m_iWeaponItemSeq = iSeqNumber;

		//	AJJIYA [12/7/2005]
		//	¹«±â°¡ ¹Ù²ïÈÄ ½ÃÄö½º °ª°ú ºñ±³ÇØ¼­ ´Ù¸£¸é »èÁ¦ÇÑ´Ù.
		unsigned int	uiAfterAniIndex	=	GetGOBSeqKey(m_iWeaponItemSeq, m_eCurSeqIndex, m_iClass);

		if( uiBeforeAniIndex != uiAfterAniIndex )
			m_pFXArchive->DeleteFXGroup( uiBeforeAniIndex );

		SetState(m_eCurState, true);
	}
}

void SPPlayer::EquipChange_CHAR_EQUIPMENT(CHAR_EQUIPMENT CharEquipment, bool bDefault)
{
	SPItemStatusEquip ItemStatusEquip;	

	ItemStatusEquip.SetItemID(CharEquipment.uiItemUniqueID);
	ItemStatusEquip.SPSendMessage(MV_ITEM_SETEQUIPID,		CharEquipment.iEquipID);
	ItemStatusEquip.SPSendMessage(MV_ITEM_SETEQUIPCLASS,	CharEquipment.iEquipClass);
	ItemStatusEquip.SPSendMessage(MV_ITEM_SETEQUIPITEMID,	CharEquipment.iiItemID);
	ItemStatusEquip.SPSendMessage(MV_ITEM_SETCOLOR,			CharEquipment.iiItemColor);

	if(bDefault) {
		// LDT ¿¡¼­ µðÆúÆ®½½·Ô¹øÈ£´Â Æ÷Áö¼Ç°ú ´Ù¸£´Ù..
		switch(CharEquipment.iEquipID) {
		case 1: CharEquipment.iEquipID = EQ_BLOUSE;		break;
		case 2: CharEquipment.iEquipID = EQ_PANTS;		break;
		case 3: CharEquipment.iEquipID = EQ_HAIR;		break;
		case 4: CharEquipment.iEquipID = EQ_EXPRESSION;	break;
		case 5: CharEquipment.iEquipID = EQ_SKIN;		break;
		}

		ItemStatusEquip.SPSendMessage(MV_ITEM_SETEQUIPID,		CharEquipment.iEquipID);
		m_pPlayerEquipInfo->SetDefaultChange( (EQUIP_ID)(CharEquipment.iEquipID), &ItemStatusEquip);
	}
	m_pPlayerEquipInfo->SetCustume(&ItemStatusEquip);

	//[xialin 2008/09/19]edit
	if( m_eLobbyAvatar == LOBBY_NULL || m_eLobbyAvatar == LOBBY_CASH )
		SetWeaponSeqItem();
}



void SPPlayer::Process(float fTime)
{
	if( g_pMainGameManager->GetSubGameID() == SUBGAME_LOADING )
	{
		if( m_pkEffectArchive )
			m_pkEffectArchive->Process(fTime);
		return;
	}

	SPGameObject::Process(fTime);
	if( m_bLocalPlayer || m_eLobbyAvatar == LOBBY_CASH ) //[xialin 2008/07/14]edit
	{
		SetControl(fTime);
		if( m_iSendAction > 0 || m_iSendSkillID > 0 )
		{
			m_fSendAccmulateTime += fTime;
			if( m_fSendAccmulateTime > 2.0f )
				InitSendAction();
		}

		////EventCheck Test -> Process ¿¡¼­ ÁÖ±âÀûÀ¸·Î °è¼Ó Ã¼Å©ÇØÁÖ¾î¾ß ÇÒµíÇÔ..
		//int iEvent = -1;		
		//iEvent = m_pEventArchive->CheckEventIndexArea(GetPosX(), GetPosY());
		//if(iEvent > -1) {			
		//	if( ((m_uiControlAction & ACTION_MOVE_UP) == ACTION_MOVE_UP) && m_bSendEventPacket){
		//		//ÆÐÅ¶À» º¸³¾¼ö ÀÖÀ½...
		//		SPEventUnit* pEvent = m_pEventArchive->GetEventUnit(iEvent);
		//		DXUTOutputDebugString(" - Send Event[%d] - Type[%d] ID[%d]\n", iEvent, pEvent->m_iType, pEvent->m_iEventID);
		//		SendEventPacket(pEvent);
		//		m_bSendEventPacket = false;
		//	}	
		//}
		//else {
		//	m_bSendEventPacket = true;			//ÀÌ °ÍÀº ¼­¹öÀÇ Event ¸®ÇÃ¶óÀÌ·Î ÇÑ´Ù 
		//}

		if( m_bControlEnable )
			ParsingControlAction();	

		if( m_pkTeamArchive && m_pkTeamArchive->GetActive() )
			UpdateTeamStatus();

#ifndef _USE_OLD_HP_BAR
		if(m_pBeAttackedList)
			m_pBeAttackedList->Process(fTime);
#endif
	}
	else
	{
		if( !m_vFigureItemList.empty() )
		{
			m_fFigureAccmulateTime += fTime;
			if( m_fFigureAccmulateTime > m_fFigureDelayTime )
				ChangeFigureItem();
		}
	}

	if (m_pkRTREffectMgr)
		m_pkRTREffectMgr->Process(fTime);

	ProcessImage(fTime);
	
	if( m_pkCoordPhysics )
	{
		if (m_fRemainProccessTime > 0.001f || m_fRemainProccessTime < 0.0f)
		{
			m_fRemainProccessTime	= 0;
		}
		m_fRemainProccessTime	+= fTime;
		if (m_fRemainProccessTime > 1.0f)
		{
			// Ê±¼ä
			m_pkCoordPhysics->Process(m_fRemainProccessTime);
			m_fRemainProccessTime		= 0;
		}

		while (m_fRemainProccessTime > 0.001f)
		{
			m_fRemainProccessTime	-= 0.001f;
			m_pkCoordPhysics->Process(0.001f);
		}
	}

	if(m_pFXArchive)
		m_pFXArchive->Process(fTime);

	if(m_pSoundArchive) {
 		SPAvatarModelUnit* pkAvatarUnit = (SPAvatarModelUnit*)m_pGOBModelUnit;
		POINT pos;
		//float fX = ((pkAvatarUnit->GetRenderPosX()) - ((float)(g_pVideo->GetScreenLenX()) / 2.0f) + 0.0f) / 3.5f;
		//float fY = ((pkAvatarUnit->GetRenderPosY()) - ((float)(g_pVideo->GetScreenLenY()) / 2.0f) + 0.0f) / 3.5f; 

		float fX = ((pkAvatarUnit->GetRenderPosX()) - ((float)(g_pVideo->GetScreenLenX()) / 2.0f) + 0.0f) / 5.5f;
		float fY = ((pkAvatarUnit->GetRenderPosY()) - ((float)(g_pVideo->GetScreenLenY()) / 2.0f) + 0.0f) / 5.5f; 

		pos.x = (long)fX;
		pos.y = (long)fY;
		m_pSoundArchive->Process(fTime, pos);
	}

	if( m_pkSkillArchive )
		m_pkSkillArchive->Process(fTime);

	if( m_pkEffectArchive )
		m_pkEffectArchive->Process(fTime);
	
	if((m_bLocalPlayer || m_eLobbyAvatar == LOBBY_CASH)) {
		ProcessLooting(fTime);
		ProcessPetLooting(fTime);
		ProcessPetItemUsed(fTime);
		ProcessItemDuration(fTime);
		ProcessPetChat(fTime);
		//ProcessCriticalEffect(fTime);					//[2007/11/5] - Å©¸®Æ¼ÄÃ Å¸°Ý Å×½ºÆ®¿ë
		
		if(m_iTrade == TRADE_STATE_RESPONSE) {			//³»°¡ ÀÀ´äÇØ¾ß ÇÒ »óÈ²¿¡¼­...
			if(this->IsDead() /*|| g_pEventManager->GetCutInState() > CUT_IN_STATE_NULL*/) {
				g_pInterfaceManager->ForceReply();
				//m_pPlayerInventory->SendTradeResponse(false);
				m_iTrade = TRADE_STATE_NULL;			//°­Á¦·Î ¸ðµå ¹Ù²Þ...
				DXUTOutputDebugString("³»°¡ Á×¾ú°Å³ª ÄÆÀÎ¸ðµå·Î µé¾î°¡ Ãë¼Ò\n");
			}

			if(CheckPcInterval(m_uiCheckIntervalPlayer) == false) {			//»ó´ë¿Í ³ªÀÇ °Å¸®°¡ Æ®·¹ÀÌµå°¡ ºÒ°¡´ÉÇÏ°Ô µÈ °æ¿ì
				g_pInterfaceManager->ForceReply();
				//m_pPlayerInventory->SendTradeResponse(false);
				m_iTrade = TRADE_STATE_NULL;			//°­Á¦·Î ¸ðµå ¹Ù²Þ...
				DXUTOutputDebugString("À¯Àú°£ °Å¸® ¹®Á¦·Î Ãë¼Ò...\n");
			}
			
		}
		else if(m_uiUserShopPlayer) {										//[2006/5/22] °³ÀÎ»óÁ¡¿¡ ÀÔÀåÇÑ °æ¿ì
			g_pInterfaceManager->ForceReply();
			if(CheckPcInterval(m_uiUserShopPlayer) == false) {
				DXUTOutputDebugString("À¯Àú°£ °Å¸® ¹®Á¦·Î °³ÀÎ»óÁ¡ ÀÔÀå Ãë¼Ò...\n");
				SendUserShopOut();
				g_pInterfaceManager->GetStatPalletWindow()->SPSendMessage(SPIM_STAT_BUTTON_ENABLE, (WPARAM)true, (LPARAM)false);
			}
		}
		else if(GetCurAction() & ACTION_USERSHOP) {							//[2006/5/29] ³»°¡ °³ÀÎ»óÁ¡ÀÎ °æ¿ì
			g_pInterfaceManager->ForceReply();
		}
		else if(SPCashTradeManager::GetInstance()->GetCashTradeState() == CASH_TRADE_STATE_WAIT) {	//[2008/4/25] - cash °Å·¡ ´ë±âÁß				
			if(SPCashTradeManager::GetInstance()->GetCashTradePlace() == CASH_TRADE_PLACE_FIELD)
				g_pInterfaceManager->ForceReply();
		}
	}

	if( m_pPlayerStatusModel )
		m_pPlayerStatusModel->Process( fTime );

	//if( m_pkPet )
	//{
	//	m_pkPet->Process(fTime);

	//	//[xialin 2008/08/13]add
	//	if ( m_eLobbyAvatar == LOBBY_CASH )
	//	{
	//		m_pkPet->SetPetGageRgn(100, 100, 100, 100);
	//	}
	//	else if( m_bLocalPlayer )
	//	{
	//		SPItem* pPetItem = m_pPlayerInventory->GetInventoryItem(FIGURE_PET, 0);
	//		if( pPetItem )
	//		{
	//			SPItemStatus* pkItemStatus = pPetItem->GetItemStatus();
	//			if( pkItemStatus )
	//			{
	//				SPItemAttr* pkItemAttr = pkItemStatus->GetItemAttr();
	//				if( pkItemAttr )
	//				{
	//					m_pkPet->SetPetGageRgn(pkItemStatus->GetSp(), 100,
	//						pkItemStatus->GetExp(), pkItemAttr->m_iPetMaxExp);
	//				}
	//			}
	//		}
	//	}
	//}

	//--------------------------------------------------
	if (m_pPetMgr)
	{
		m_pPetMgr->Process( fTime );
		SPPet *pet = NULL;
		for (int idx = 0; idx < PET_PLACE_TYPE_NUMBER; ++idx)
		{
			pet = m_pPetMgr->GetPet( (PET_PLACE)idx );
			if (pet)
			{
				if (m_pPlayerInventory)
				{
					SPItem *pPetItem = m_pPlayerInventory->GetInventoryItem( FIGURE_PET, idx );
					if (pPetItem)
					{
						SPItemStatus *pkItemStatus = pPetItem->GetItemStatus();
						if (pkItemStatus)
						{
							SPItemAttr *pkItemAttr = pkItemStatus->GetItemAttr();
							if (pkItemAttr)
								pet->SetPetGageRgn(pkItemStatus->GetSp(), 100,
									pkItemStatus->GetExp(), pkItemAttr->m_iPetMaxExp);
						}
					}
				}
			}
		}
	}
	//--------------------------------------------------
}


int	SPPlayer::ProcessLooting(float fTime)			//µô·¹ÀÌ Å¸ÀÓ ÁÙ°Í
{
	m_fAccumulateLooting += fTime;
	if( m_fAccumulateLooting > m_fCurLootingDelay) {
		m_fAccumulateLooting -= m_fCurLootingDelay;	
	
		if(m_bLootingAll == false)
			return 0;
		
		if(g_pCheckManager->IsDBBlock()) {
			//
			//DXUTOutputDebugString("\tSPPlayer::ProcessLooting DB Working Block\n");
			return 0;
		}	

		if( m_pkCurLootingBag == NULL ) {
			m_bLootingAll = false;
			return 0;		
		}

		//LOOTING_ITEM LootingInfo;
		unsigned char ucNum;

		GU_ID iGlobalID = m_pkCurLootingBag->GetGUID();

		//[2005/11/4] ÁÖÀÇ ÇöÀç »óÅÂ¿¡¼­´Â ¸ð³»±â°¡ »ý±è... ¹Ù·Î Á¢±ÙÇÏÁö ¸»°í ¸É¹ö¿¡ Ä«ÇÇ ÇÏ´Â ¹æ½ÄÀ» »ç¿ëÇØ¾ß ÇÒÁöµµ...
		vector< CONTAINER_ITEM >* pvItemList = m_pkCurLootingBag->GetItemList();
		ucNum = (unsigned char)(*pvItemList).size();
		if( ucNum == 0 ) {
			m_bLootingAll = false;
			return 0;
		}

		//if(m_iLootingIndex >= ucNum) {
		//	m_bLootingAll = false;
		//	return 0;
		//}
		
		CONTAINER_ITEM pItem = pvItemList->at(m_iLootingIndex);				//ÁÖÀÇ ¾ðÁ¦³ª m_iLootingIndex = 0 	

		OnLootingItemGainReq((LPARAM)(&pItem));
		DXUTOutputDebugString(" - Send Looting Item [%d]\n", m_iLootingIndex);
		//m_iLootingIndex++;
	}
	
	return 0;
}

int SPPlayer::ProcessPetLooting(float fTime)
{
	//if( m_pkPet == NULL || !m_pkPet->IsToggleOnSpecialSkill(SST_AUTO_LOOTING) || IsDead() )
		//return 0;
	SPPet *LootingPet = NULL;
	if (m_pPetMgr == NULL || !m_pPetMgr->IsToggleOnSpecialSkill( SST_AUTO_LOOTING, &LootingPet ) || IsDead())
		return 0;

	if( m_pkPetCurLootingBag == NULL )
	{
		m_fAccmulatePetCheckLooting += fTime;
		if( m_fAccmulatePetCheckLooting > 1.0f )
		{	
			CheckPetLootingBag();
			m_fAccmulatePetCheckLooting = 0.0f;
		}

		return 0;
	}

	m_fAccumulatePetLooting += fTime;
	if( m_fAccumulatePetLooting < m_fPetLootingDelay )
		return 0;

	m_fAccumulatePetLooting = 0.0f;

	if( g_pCheckManager->IsDBBlock() || g_pCheckManager->CheckPlayerState() != PLAYER_STATE_NULL )
		return 0;

	CONTAINER_ITEM* pItem = NULL;
	pItem = GetCanLootingItem(m_pkPetCurLootingBag);
	if( pItem == NULL )
	{
		m_pkPetCurLootingBag = NULL;
		return 0;
	}
	
	if( OnPetLootingItemGainReq((LPARAM)pItem) )
		if (LootingPet)
			LootingPet->SetLootingFX();

	return 0;
}

int	SPPlayer::ProcessPetItemUsed(float fTime)
{
	//if( m_pkPet == NULL || !m_pkPet->IsToggleOnSpecialSkill(SST_AUTO_ITEM_USED) || IsDead() )
	//	return 0;
	if (m_pPetMgr == NULL || !m_pPetMgr->IsToggleOnSpecialSkill( SST_AUTO_ITEM_USED ) || IsDead())
		return 0;

	m_fAccumulatePetItemUsed += fTime;
	if( m_fAccumulatePetItemUsed < m_fPetItemUsedDelay )
		return 0;

	m_fAccumulatePetItemUsed = 0.0f;

	unsigned int uiMaxHP, uiCurHP, uiMaxSP, uiCurSP;
	uiMaxHP = m_pPlayerStatus->GetMaxHP();
	uiCurHP = m_pPlayerStatus->GetCurHP();
	uiMaxSP = m_pPlayerStatus->GetStatusValue(STATUS_MSP);
	uiCurSP = m_pPlayerStatus->GetStatusValue(STATUS_SP);

	if( g_pCheckManager->CheckPlayerState() != PLAYER_STATE_NULL )
		return 0;

	if( uiMaxHP == 0 || uiMaxSP == 0 )
		return 0;

	int i, iSize, iItemID;
	SPItem* pkItem;
	SPItemAttr* pItemAttr;

	if( ((float)uiCurHP / (float)uiMaxHP) <= 0.25f )
	{
		iSize = m_pPlayerInventory->GetInventorySize(INVENTORY_CONSUME);
		for( i = 0; i < iSize; i++ )
		{
			pkItem = m_pPlayerInventory->GetInventoryItem(INVENTORY_CONSUME, i);
			if( pkItem == NULL )
				continue;

			iItemID = pkItem->GetItemStatusValue(MV_ITEM_GETUID);
			if( iItemID == 0 )
				continue;

			if( !g_pItemCluster->GetItemInfo(iItemID, pItemAttr) )
				continue;

			if( pItemAttr->m_iMacroType != 1 )
				continue;
			
			if( m_pPlayerInventory->ItemQuickUse(INVENTORY_CONSUME, iItemID) )
				return 0;
		}
	}

	if( ((float)uiCurSP / (float)uiMaxSP) <= 0.25f )
	{
		iSize = m_pPlayerInventory->GetInventorySize(INVENTORY_CONSUME);
		for( i = 0; i < iSize; i++ )
		{
			pkItem = m_pPlayerInventory->GetInventoryItem(INVENTORY_CONSUME, i);
			if( pkItem == NULL )
				continue;

			iItemID = pkItem->GetItemStatusValue(MV_ITEM_GETUID);
			if( iItemID == 0 )
				continue;

			if( !g_pItemCluster->GetItemInfo(iItemID, pItemAttr) )
				continue;

			if( pItemAttr->m_iMacroType != 2 )
				continue;

			if( m_pPlayerInventory->ItemQuickUse(INVENTORY_CONSUME, iItemID) )
				return 0;
		}
	}

	return 0;
}

int SPPlayer::ProcessPetChat(float fTime)
{
	SPPet *pPet = NULL;
	if (IsDead())
	{
		return 0;
	}

	m_fAccumulatePetChat += fTime;
	if (m_fAccumulatePetChat < iPetChatInterval)
		return 0;

	m_fAccumulatePetChat = 0.0f;

	for (int idx = 0; idx < PET_PLACE_TYPE_NUMBER; ++idx)
	{
		pPet = m_pPetMgr->GetPet( (PET_PLACE)idx );
		if (!pPet)
			continue;

		if (pPet->GetSkillStatus( SST_PET_CHAT ))
		{
		    pPet->SendChat();
		}
		pPet = NULL;
	}

	return 0;
}


int	SPPlayer::ProcessItemDuration(float fTime)
{
	m_fAccumulateItem += fTime;
	if( m_fAccumulateItem > m_fCurItemDelay) {
		m_fAccumulateItem -= m_fCurItemDelay;	
		if(m_pPlayerInventory)
			m_pPlayerInventory->DecreaseLifeTime();
	}

	return 0;
}


void SPPlayer::ProcessImage(float fTime)
{
	if( m_pGOBModelUnit == NULL || m_pkCoordPhysics == NULL )
		return;

	m_fCamX = g_pCameraManagr->GetCameraStruct()->fOffsetX;
	m_fCamY = g_pCameraManagr->GetCameraStruct()->fOffsetY;

	//[xialin 2008/08/06]add
	if ( m_eLobbyAvatar == LOBBY_CASH )
	{
		m_fCamX += CASHSHOP_OFFSETCAMX;
		m_fCamY += CASHSHOP_OFFSETCAMY;
	}
	else if( m_eLobbyAvatar != LOBBY_NULL )
	{
		m_fCamX = 0.0f;
		m_fCamY = 0.0f;
	}

	m_RectPosition.left		= (LONG)(m_pkCoordPhysics->GetPosX()-m_fCamX - 5);
	m_RectPosition.right	= (LONG)(m_pkCoordPhysics->GetPosX()-m_fCamX + 5);
	m_RectPosition.top		= (LONG)(m_pkCoordPhysics->GetPosY()-m_fCamY - 5);
	m_RectPosition.bottom	= (LONG)(m_pkCoordPhysics->GetPosY()-m_fCamY + 5);

	/*
	m_RectName.top			= m_pPlayerCoordPhysics->GetPosY() - m_fCamY - 144.f;
	m_RectName.right		= m_RectName.left + 127;	
	m_RectName.bottom		= m_RectName.top + 14;
	*/

	float	fParsingX, fParsingY;
	WPARAM	wParsingX, wParsingY; 
	fParsingX = m_pkCoordPhysics->GetPosX()-m_fCamX-m_iAvatarOffsetX;
	fParsingY = m_pkCoordPhysics->GetPosY()-m_fCamY-m_iAvatarOffsetY;
	DataConvert<float,WPARAM>(fParsingX, wParsingX);
	DataConvert<float,WPARAM>(fParsingY, wParsingY);
	m_pGOBModelUnit->SPSendMessage(MV_SET_RENDERPOSX, wParsingX );
	m_pGOBModelUnit->SPSendMessage(MV_SET_RENDERPOSY, wParsingY );
	
	m_bClipping = false;
	if( fParsingX < -400 || fParsingX > 1200 )
		m_bClipping = true;
	else if( fParsingY < -300 || fParsingY > 900 )
		m_bClipping = true;
	
	m_pGOBModelUnit->Process(fTime);

	SPAvatarModelUnit* pkAvatarUnit = (SPAvatarModelUnit*)m_pGOBModelUnit;

	bool bShadow = true;
	GOB_STATE_TYPE eType = m_pkCoordPhysics->GetCurState();
	if( m_pkCoordPhysics->GetCurVelocityY() ||
		eType == GOB_STATE_DROP ||
		eType == GOB_STATE_VERTICAL_STAND ||
		eType == GOB_STATE_HANGING_STAND ||
		eType == GOB_STATE_HANGING )
		bShadow = false;
	
	if( m_pPlayerStatusModel )
	{
		m_pPlayerStatusModel->SetObjectRgn(pkAvatarUnit->GetRenderRgn(),
			(int)pkAvatarUnit->GetRenderPosX(), (int)pkAvatarUnit->GetRenderPosY(), bShadow);

		//»óÁ¡ÀÎ °æ¿ì Æ÷Áö¼Ç ¾÷µ¥ÀÌÆ® ÀÛ¾÷
		if(GetCurAction() & ACTION_USERSHOP) {
			m_pPlayerStatusModel->SetUserShopRgn(pkAvatarUnit->GetRenderRgn(),
				(int)pkAvatarUnit->GetRenderPosX(), (int)pkAvatarUnit->GetRenderPosY());
		}

		//[2007/2/27]
		if(m_bRenderPartyFind) {
			SetPartyFindRenderDest((int)pkAvatarUnit->GetRenderPosX(), (int)pkAvatarUnit->GetRenderPosY());
		}
		
		////ÆÄÆ¼±¸ÇÔ Ç¥½Ã
		//SPPlayerStatus* pPlayerStatus = static_cast<SPPlayerStatus*>(m_pPlayerStatus);
		//if(pPlayerStatus && pPlayerStatus->GetGameOption() & SSO_REQ_TEAM) {
		//	int i = 0;
		//}
	}
}


void SPPlayer::Render(float fTime)
{
	SPGameObject::Render(fTime);

	bool bPetRender = false;
	//if( m_pkPet )
	//{
	//	//[xialin 2008/08/13]add
	//	if ( m_eLobbyAvatar == LOBBY_CASH )
	//	{
	//		bPetRender = true;
	//	}
	//	else
	//	{
	//		if( m_pkPet->IsPreRendering() ) {
	//			//m_pkPet->Render(fTime);
	//			;
	//		}
	//		else
	//			bPetRender = true;
	//	}
	//}

	if (m_pPetMgr)
	{
		if (LOBBY_CASH == m_eLobbyAvatar)
			m_pPetMgr->JudgePreRender( true );
		else
			m_pPetMgr->JudgePreRender();
	}

	RenderFX( fTime , FXM_RENDER_LAYER_OBJECT_BACK );

	//////////////////////////////////////////////////////////////////////////
	// Ä³¸¯ÅÍ
	if( !m_bClipping )
	{
		if( m_bStatusRender && m_pPlayerStatusModel )
			m_pPlayerStatusModel->Render(fTime);

		if (m_pkRTREffectMgr)
			m_pkRTREffectMgr->Render(fTime);

		if( m_vFigureItemList.empty() && m_pGOBModelUnit)
		{
			if (!m_bTransformMode)
				m_pGOBModelUnit->Render(fTime);
			else if (LOBBY_CASH == m_eLobbyAvatar)
				m_pGOBModelUnit->Render(fTime);
		}

		if(m_bRenderPartyFind)
			RenderPartyFind(fTime);
	}

#ifndef _USE_OLD_HP_BAR
	if((m_bLocalPlayer || m_eLobbyAvatar == LOBBY_CASH) && m_pBeAttackedList) {
		m_pBeAttackedList->Render(fTime);
	}	
#endif

	RenderFX( fTime , FXM_RENDER_LAYER_OBJECT_FRONT );
	//if( m_pFXArchive )
	//	m_pFXArchive->Render(fTime);
	
#if defined(_DEBUG)
	if( g_pMainGameManager->GetRenderDebugInfo() )
	{
		// Position Rectangle
		g_pVideo->GetSysLine()->DrawRect(m_RectPosition);

		AttackBoxRender();
	}
#endif
	
	//if( bPetRender )
	//	m_pkPet->Render(fTime);
	if (m_pPetMgr)
		m_pPetMgr->Render( fTime );

}

void SPPlayer::AttackBoxRender()
{
	SPCharacterAttack* pkAttack = NULL;
	SPCharacterBeAttacked* pkBeAttacked = NULL;

	g_pBattleCluster->GetAttack(m_pGOBModelUnit->SPSendMessage(MV_GET_ANIMATION_INDEX), &pkAttack);
	
	int i;
	RECT rcAttack, rcBeAttack;
	if( pkAttack )
	{
		for( i = 0; i < (int)pkAttack->m_AttackInfo.size(); i++ )
		{
			int iWidth = pkAttack->m_AttackInfo.at(i).m_rtRange.right - pkAttack->m_AttackInfo.at(i).m_rtRange.left;
			if( m_pkCoordPhysics->GetDirection() == FACING_RIGHT )
			{
				rcAttack.left = (LONG)(m_pkCoordPhysics->GetPosX() - m_fCamX
					+ pkAttack->m_AttackInfo.at(i).m_rtRange.left);
				rcAttack.right = (LONG)(m_pkCoordPhysics->GetPosX() - m_fCamX
					+ pkAttack->m_AttackInfo.at(i).m_rtRange.right);
			}
			else
			{
				rcAttack.left = (LONG)(m_pkCoordPhysics->GetPosX() - m_fCamX
					-pkAttack->m_AttackInfo.at(i).m_rtRange.left - iWidth);
				rcAttack.right = (LONG)(rcAttack.left + iWidth);
			}

			rcAttack.top = (LONG)(m_pkCoordPhysics->GetPosY() - m_fCamY
				+ pkAttack->m_AttackInfo.at(i).m_rtRange.top);
			rcAttack.bottom	= (LONG)(m_pkCoordPhysics->GetPosY() - m_fCamY
				+ pkAttack->m_AttackInfo.at(i).m_rtRange.bottom);

			g_pVideo->GetSysLine()->DrawRect(rcAttack);
		}
	}
	
	g_pBattleCluster->GetAttack(m_pGOBModelUnit->SPSendMessage(MV_GET_ANIMATION_INDEX), &pkBeAttacked);

	if( pkBeAttacked )
	{
		int nFrame = (int)m_pGOBModelUnit->SPSendMessage(MV_GET_CURR_FRAME) + 1;

		vector< APCBEATTACKEDINFO >::iterator iter = pkBeAttacked->m_BeAttackInfo.begin();
		while( iter!= pkBeAttacked->m_BeAttackInfo.end() )
		{
			if( nFrame <= (*iter).m_nIndex )
			{
				if( m_pkCoordPhysics->GetDirection()==FACING_RIGHT )
				{
					rcBeAttack.left	= (LONG)(m_pkCoordPhysics->GetPosX() - m_fCamX
						+ (*iter).m_rtRange.left);
					rcBeAttack.right = (LONG)(m_pkCoordPhysics->GetPosX() - m_fCamX
						+(*iter).m_rtRange.right);
				}
				else
				{
					rcBeAttack.left	= (LONG)(m_pkCoordPhysics->GetPosX() - m_fCamX
						- (*iter).m_rtRange.right);
					rcBeAttack.right = (LONG)(m_pkCoordPhysics->GetPosX() - m_fCamX
						- (*iter).m_rtRange.left);
				}

				rcBeAttack.top = (LONG)(m_pkCoordPhysics->GetPosY() - m_fCamY
					+ (*iter).m_rtRange.top);
				rcBeAttack.bottom = (LONG)(m_pkCoordPhysics->GetPosY() - m_fCamY
					+ (*iter).m_rtRange.bottom);

				g_pVideo->GetSysLine()->DrawRect(rcBeAttack);
				break;
			}
			
			++iter;
		}		
	}
}

bool SPPlayer::CheckStatus(UINT64& uiAction)
{
	if( uiAction & ACTION_SKILL )
		return CheckSkillStatus(uiAction);

	int RequireValue = 0;
	unsigned int uiAniIndex = 0;
	if( (uiAction & ACTION_RUN_LEFT) ||
		(uiAction & ACTION_RUN_RIGHT) )
		uiAniIndex = GetGOBSeqKey(m_iWeaponItemSeq, GOB_SEQ_RUN, m_iClass);

	if( uiAction & ACTION_GUARD )
		uiAniIndex = GetGOBSeqKey(m_iWeaponItemSeq, GOB_SEQ_GUARD, m_iClass);

	if( (uiAction & ACTION_ATTACK_SPEEDY) ||
		(uiAction & ACTION_ATTACK_MIGHTY) )
	{
		unsigned int uiCurAniIndex = GetGOBSeqKey(m_iWeaponItemSeq, m_eCurSeqIndex, m_iClass);
		int iAttackType = 1;
		if( uiAction & ACTION_ATTACK_MIGHTY )
			iAttackType = 2;

		m_uiSendAttackIndex = SPSkillManager::GetInstance()->GetNextAttackComboNo(uiCurAniIndex, iAttackType);
		if( m_uiSendAttackIndex == 0 )
		{
			if( iAttackType == 1 )
			{
				if( m_uiCurAction & ACTION_MOVE_JUMP ||
					m_uiCurAction & ACTION_MOVE_DROP )
					m_uiSendAttackIndex = GOB_SEQ_JUMP_ATTACK_NORMAL01;
				else
					m_uiSendAttackIndex = GOB_SEQ_ATTACK_NORMAL01;
			}
			else
			{
				if( m_uiCurAction & ACTION_MOVE_JUMP ||
					m_uiCurAction & ACTION_MOVE_DROP )
					m_uiSendAttackIndex = GOB_SEQ_JUMP_ATTACK_HEAVY01;
				else
					m_uiSendAttackIndex = GOB_SEQ_ATTACK_HEAVY01;
			}
		}
		
		uiAniIndex = GetGOBSeqKey(m_iWeaponItemSeq, m_uiSendAttackIndex, m_iClass);
	}

	SPMotionStatus* pkStatus = SPSkillManager::GetInstance()->GetMotionStatus(uiAniIndex);
	if( pkStatus == NULL )
		return true;

	SKILL_COMBO* pComboInfo = pkStatus->GetComboInfo();
	if( (m_uiCurAction & ACTION_ATTACK_SPEEDY) ||
		(m_uiCurAction & ACTION_ATTACK_MIGHTY) )
	{
		if( !SPComboManager::GetInstance()->CanConnection(pComboInfo->iSelfComboID) )
			return false;
	}

	MOTION_STATUS_STAT* pStat = pkStatus->GetRequireStatList(0);
	if( pStat )
	{
		if( !g_pCheckManager->CheckCondition(pStat->eType, pStat->iID, pStat->iValue, pStat->iValue2) )
		{
			if( uiAction & ACTION_RUN_LEFT )
			{
				uiAction &= ~ACTION_RUN_LEFT;
				uiAction |= ACTION_MOVE_LEFT;
			}
			else if( uiAction & ACTION_RUN_RIGHT )
			{
				uiAction &= ~ACTION_RUN_RIGHT;
				uiAction |= ACTION_MOVE_RIGHT;
			}

			if( uiAction & ACTION_GUARD )
				uiAction &= ~ACTION_GUARD;
		}
	}

	if( uiAction == m_uiCurAction &&
		!(uiAction & ACTION_ATTACK_SPEEDY) &&
		!(uiAction & ACTION_ATTACK_MIGHTY) )
		return false;

	for( int i = 0 ; i < pkStatus->GetRequireStatListCount() ; ++i )
	{
		pStat = pkStatus->GetRequireStatList( i );

		if( pStat != NULL )
		{
			if( !g_pCheckManager->CheckCondition(pStat->eType, pStat->iID, pStat->iValue, pStat->iValue2) )
				return false;
		}
	}

	return true;
}

int SPPlayer::GetAniIndex(UINT64 uiAction)
{
	if( uiAction & ACTION_SKILL )
		return -1;

	int RequireValue = 0;
	unsigned int uiAniIndex = 0;
	if( (uiAction & ACTION_RUN_LEFT) ||
		(uiAction & ACTION_RUN_RIGHT) )
		uiAniIndex = GetGOBSeqKey(m_iWeaponItemSeq, GOB_SEQ_RUN, m_iClass);

	if( uiAction & ACTION_GUARD )
		uiAniIndex = GetGOBSeqKey(m_iWeaponItemSeq, GOB_SEQ_GUARD, m_iClass);

	if( (uiAction & ACTION_ATTACK_SPEEDY) ||
		(uiAction & ACTION_ATTACK_MIGHTY) )
	{
		unsigned int uiCurAniIndex = GetGOBSeqKey(m_iWeaponItemSeq, m_eCurSeqIndex, m_iClass);
		int iAttackType = 1;
		if( uiAction & ACTION_ATTACK_MIGHTY )
			iAttackType = 2;

		UINT uiSendAttackIndex = SPSkillManager::GetInstance()->GetNextAttackComboNo(uiCurAniIndex, iAttackType);
		if( uiSendAttackIndex == 0 )
		{
			if( iAttackType == 1 )
			{
				if( m_uiCurAction & ACTION_MOVE_JUMP ||
					m_uiCurAction & ACTION_MOVE_DROP )
					uiSendAttackIndex = GOB_SEQ_JUMP_ATTACK_NORMAL01;
				else
					uiSendAttackIndex = GOB_SEQ_ATTACK_NORMAL01;
			}
			else
			{
				if( m_uiCurAction & ACTION_MOVE_JUMP ||
					m_uiCurAction & ACTION_MOVE_DROP )
					uiSendAttackIndex = GOB_SEQ_JUMP_ATTACK_HEAVY01;
				else
					uiSendAttackIndex = GOB_SEQ_ATTACK_HEAVY01;
			}
		}
		
		uiAniIndex = GetGOBSeqKey(m_iWeaponItemSeq, uiSendAttackIndex, m_iClass);
	}

	return uiAniIndex;
}

bool SPPlayer::CheckSkillStatus(UINT64& uiAction)
{
	uiAction &= ~ACTION_SKILL;

	if( g_pCheckManager->CheckSkillUseRequire( m_iRequestSkillID ) == false )
		return false;

	uiAction |= ACTION_SKILL;
	return true;
}

void SPPlayer::ParsingControlAction()
{
	if( m_pkCoordPhysics->GetCurState() == GOB_STATE_DEAD )
		return;

	unsigned char ucFacing = m_pkCoordPhysics->IsFacingChange(m_uiControlAction);
	if( ucFacing > 0 && !m_bFacingSend )
	{
		//[xialin 2008/07/29] edit >>>
		if ( m_eLobbyAvatar == LOBBY_CASH )
		{
			OnSetFacing((LPARAM)ucFacing);
		}
		else
		{
			CPacket pkPacket;
			pkPacket.AddUINT32(CHAR_CS_FACING);
			pkPacket.AddUINT8(ucFacing);
			(((SPNetworkManager*)g_pNetworkManager)->GetPacketHandler())->PostMsg(&pkPacket);
			
			m_bFacingSend = true;
		}
		return;
	}
	
	if( !m_pkCoordPhysics->IsActionChange(m_uiControlAction) )
		return;

	if( (m_uiControlAction & ACTION_SKILL) && (m_uiCurAction & ACTION_SKILL) )
	{

	}
	else
	{
		if( !(m_uiControlAction & ACTION_ATTACK_SPEEDY) &&
			!(m_uiControlAction & ACTION_ATTACK_MIGHTY) )
		{
			if( m_uiControlAction == m_uiCurAction )
				return;
		}
	}

	if( m_bWeaponSwapping  )
		return;

	if( !CheckStatus(m_uiControlAction) )
		return;

	if( (m_uiControlAction & ACTION_LOOTING) && !CheckLootingBag() )
		return;

	GOB_STATE_TYPE eType = m_pkCoordPhysics->GetCurState();
	if( eType == GOB_STATE_LOOTING_WAIT ||
		eType == GOB_STATE_LOOTING )
	{
		m_pkCurLootingBag = NULL;
		g_pInterfaceManager->ShowWindow(WIID_LOOTING, SP_HIDE);
	}

	if( eType == GOB_STATE_ENCHANTTING )
	{
		// AJJIYA [5/19/2006]
		// ¾ÆÀÌÅÛ ¾÷±×·¹ÀÌµå¿Í ÀÎÃ¦Æ®´Â ÅäÅ¬ Ã¢Çü½ÄÀ¸·Î ¿­·ÁÁø Ã¢¸¸ ÀÛµ¿ÇÏ°Ô ÇØ¾ß ÇÑ´Ù.
		// ±×·¡¼­ ¸ÕÀú ¾ÆÀÌÅÛ ¾÷±×·¹ÀÌµå ÂÊÀ» Ã¼Å©ÇÑ´Ù.
		// ¾ÆÀÌÅÛ Á¶ÇÕµµ Ãß°¡ µÇ¾ú´Ù.

		if( g_pInterfaceManager->SPChildWndSendMessage( WIID_ITEM_UPGRADE , SPIM_ITEM_UPGRADE_CANCEL , 1 , 0 , true ) == 0 )
		{
			if( g_pInterfaceManager->SPChildWndSendMessage( WIID_ITEM_MIX , SPIM_ITEM_MIX_CANCEL , 1 , 0 , true ) == 0 )
			{
				g_pInterfaceManager->SPChildWndSendMessage(WIID_ENCHANT, SPIM_ITEM_ENCHANT_CANCEL, 0, 0, true);

				D3DXCOLOR color(1.0f, 0.0f, 0.0f, 1.0f);
				g_pInterfaceManager->SPPerformMessage(SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(10000017), (LPARAM)&color );
			}
		}
	}

	if ( m_eLobbyAvatar == LOBBY_CASH ) //[xialin 2008/08/05]add
	{
		SetAction(m_uiControlAction);
	}
	SendActionPacket();
}

float	SPPlayer::GetPosX()					
{ 
	if ( !m_pkCoordPhysics )
		return 0;

	return m_pkCoordPhysics->GetPosX(); 
}

float	SPPlayer::GetPosY()					
{
	if ( !m_pkCoordPhysics )
		return 0;

	return m_pkCoordPhysics->GetPosY(); 
}

FACING	SPPlayer::GetDirection()
{ 
	return m_pkCoordPhysics->GetDirection(); 
}

BG_ID	SPPlayer::GetStandLayer()
{ 
	return m_pkCoordPhysics->GetStandLayer(); 
}

void	SPPlayer::SetPos(float fX, float fY)	
{ 
	m_pkCoordPhysics->SetPos(fX, fY);
	if (m_pPetMgr)
		m_pPetMgr->SetPos( fX, fY );
	//if( m_pkPet )
		//m_pkPet->SetPos(fX, fY);
}

bool SPPlayer::IsLocalPlayer()
{
	return m_bLocalPlayer;
}

void SPPlayer::RenderFX( float fTime , SP_FXM_RENDER_LAYER eRenderLayer )
{
	if( m_pFXArchive != NULL )
		m_pFXArchive->Render( fTime , eRenderLayer );
}

void SPPlayer::SetLocalPlayer(bool bLocalPlayer)
{
	m_bLocalPlayer = bLocalPlayer;
	if( m_pPlayerStatusModel )
		m_pPlayerStatusModel->SetLocal(bLocalPlayer);

	//·ÎÄÃ ÇÃ·¹ÀÌ¾îÀÇ °æ¿ì ÀÚ½Å¸¸ÀÇ ÀÎº¥Åä¸® °ø°£À» »ý¼ºÇÏµµ·Ï ÇÑ´Ù 
	if(m_bLocalPlayer && m_pPlayerInventory) {
		m_pPlayerInventory->CreateLocalInventory();
		
#ifndef _USE_OLD_HP_BAR
		if(m_pBeAttackedList == NULL)
			m_pBeAttackedList = new SPBeAttackedList;
#endif
	}

	if (bLocalPlayer)
	{
		SPWindowPetChatEditor *pWnd = (SPWindowPetChatEditor *)g_pInterfaceManager->FindWindow( WIID_PETCHAT_EDITOR );
		if (pWnd)
			pWnd->_loadXmlCfg();
	}

	//¼­¹ö½ÌÅ© È®ÀÎ¿ë.
	/*
	if(g_LocalPlayerMoveDisable) {	
		if(m_bLocalPlayer ) {
			m_pkCoordPhysics->SetStop(true);
		} else {
			m_pkCoordPhysics->SetStop(false);
		}
	}
	*/
}

bool SPPlayer::CheckLootingBag()
{
	SPLootingBag* pkBag; 
	pkBag = (SPLootingBag*)g_pGOBManager->GetLootingBag(GetPosX(), GetPosY());
	if( pkBag == NULL )
		return false;

	m_pkCurLootingBag = pkBag;
	return true;
}

bool SPPlayer::CheckPetLootingBag()
{
	SPLootingBag* pkBag = NULL;
	int i, iSize;

	iSize = g_pGOBManager->GetLootingBagSize();
	for( i = 0; i < iSize; i++ )
	{
		pkBag = (SPLootingBag*)g_pGOBManager->GetLootingBag(i);
		if( pkBag == NULL )
			break;

		if( GetCanLootingItem(pkBag) )
		{
			m_pkPetCurLootingBag = pkBag;
			return true;
		}
	}

	return false;
}

CONTAINER_ITEM* SPPlayer::GetCanLootingItem(SPLootingBag* pkBag)
{
	CONTAINER_ITEM* pItem = NULL;
	if( m_pPlayerInventory == NULL || pkBag == NULL )
		return NULL;

	int i, iSize;
	SPItemAttr* pkItemAttr = NULL;
	bool bStackEnabel = false;

	vector< CONTAINER_ITEM >* pvItemList = pkBag->GetItemList();
	iSize = (int)(*pvItemList).size();
	for( i = 0; i < iSize; i++ )
	{
		pItem = &(*pvItemList)[i];
		if( !g_pItemCluster->GetItemInfo(pItem->Item.iItemID, pkItemAttr) )
			continue;

		if( m_pPlayerInventory->IsFullInventory((CONTAINER_TYPE)(pkItemAttr->m_eType - 1)) )
		{
			bStackEnabel = m_pPlayerInventory->IsEnableSpace((CONTAINER_TYPE)(pkItemAttr->m_eType - 1), pItem->Item.iItemID);
			if( bStackEnabel == false )
				continue;
		}

		return pItem;
	}
	
	return NULL;
}

bool SPPlayer::IsDead()
{
	int iHP = m_pPlayerStatus->GetStatusValue(STATUS_HP);
	if( iHP < 1 || m_pkCoordPhysics->GetCurState() == GOB_STATE_DEAD )
		return true;

	return false;
}

void SPPlayer::ApplyCoolTime( int iID , float fCoolTime , float fAccmulateTime )
{
	COOLTIME_INFO CoolTimeInfo;
	
	CoolTimeInfo.fAccmulateTime	= fAccmulateTime;
	CoolTimeInfo.fCoolTime		= fCoolTime;

	SPCoolTimeManager::GetInstance()->AddCoolTime(iID, CoolTimeInfo);
}

void SPPlayer::SetTradeState(TRADE_STATE iTrade, UINT64 uiID /*= 0*/)
{	
	//³»°¡ ¿äÃ»¹ÞÀº »óÅÂ¿¡¼­ °Å·¡°¡ ÀÌ·ç¾îÁø °æ¿ì
	if(m_bLocalPlayer && m_iTrade == TRADE_STATE_RESPONSE && iTrade == TRADE_STATE_TRADE) {
		if(m_uiCheckIntervalPlayer != uiID) {
			g_pInterfaceManager->ForceReply();
			m_pPlayerInventory->SendTradeResponse(false);
			DXUTOutputDebugString("³»°¡ ´Ù¸¥ »ç¶÷°ú °Å·¡·Î ÀÎÇÑ Ãë¼Ò?\n");
		}
	}
	
	m_iTrade = iTrade;
	
	if(m_iTrade == TRADE_STATE_RESPONSE) {
		if(uiID != 0)
			m_uiCheckIntervalPlayer = uiID;
	}
	else if(m_iTrade == TRADE_STATE_TRADE) {
		//g_pInterfaceManager->GetStatPalletWindow()->SPSendMessage(SPIM_STAT_BUTTON_ENABLE, (WPARAM)false, (LPARAM)false);
		SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_MENU);
		if(pWindow) {
			pWindow->SPSendMessage(SPIM_MENU_POPUP, WPARAM(0), LPARAM(0));
			pWindow->SetEnable(false);
		}
		
		//[2006/2/14] - ·çÆÃÁßÀÎ°æ¿ì???
		if(m_pkCurLootingBag) {
			m_pkCurLootingBag = NULL;
			g_pInterfaceManager->ShowWindow(WIID_LOOTING, SP_HIDE);
			OnLootingExit(0);
		}		
	}
	else if(m_iTrade == TRADE_STATE_NULL) {		
		//g_pInterfaceManager->GetStatPalletWindow()->SPSendMessage(SPIM_STAT_BUTTON_ENABLE, (WPARAM)true, (LPARAM)false);

		SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_MENU);
		if(pWindow) {
			pWindow->SetEnable(true);
		}

		SPStage* pStage = g_StageManager.GetCurStage(GetCurStageID());

		if(pStage) {
			SPMapGroup* pMapGroup = pStage->GetMapGroup(GetCurMapGroupID());
				
			if(SPLocalizeManager::GetInstance()->IsEnableValue(LV_CASHSHOP) && 
				pMapGroup && pMapGroup->IsMapGroupType(GROUP_TYPE_CASH)) 
			{
					g_pInterfaceManager->SPChildWndSendMessage(WIID_MENU, SPIM_SET_CASH_ENABLE, 1, 0);
			}
			else
			{
				g_pInterfaceManager->SPChildWndSendMessage(WIID_MENU, SPIM_SET_CASH_ENABLE, 0, 0);
			}
		
			//[2006/3/17] - ÀÌº¥Æ® ¸Ê ¿¹¿ÜÃ³¸® Ã¤³ÎÀÌµ¿ ¹öÆ°À» ¸·´Â´Ù
			if(pMapGroup && (pMapGroup->IsMapGroupType(GROUP_TYPE_EVENT) || pMapGroup->IsMapGroupType(GROUP_TYPE_INDUN))) {
				g_pInterfaceManager->SPChildWndSendMessage(WIID_EXIT, SPIM_EXIT_CHANNEL_ENABLE, 0, 0);
			}
			else {
				g_pInterfaceManager->SPChildWndSendMessage(WIID_EXIT, SPIM_EXIT_CHANNEL_ENABLE, 1, 0);
			}
		}		
	}
	
	m_uiCheckIntervalPlayer = uiID;
}

TRADE_STATE SPPlayer::GetTradeState()
{
	return m_iTrade;
}

//bool SPPlayer::IsPcTradeEnable(/*int iStage, int iMapGroup, float fPosX, float fPosY*/)
bool SPPlayer::CheckPcInterval(GU_ID uiGuid)
{
	//if(m_uiCheckIntervalPlayer != 0){
	//	return false;
	//}

	SPPlayer* pPlayer = NULL;
	
	pPlayer = (SPPlayer*)g_pGOBManager->Find(uiGuid);
	if( pPlayer != NULL ) {	
		//°°Àº ¸Ê¿¡ Á¸ÀçÇÏ´ÂÁö ?
		if(this->GetCurStageID() != pPlayer->GetCurStageID() ||
			this->GetCurMapGroupID() != pPlayer->GetCurMapGroupID())
		{
			return false;
		}

		//Æ®·¹ÀÌµå°¡ °¡´ÉÇÑ °Å¸®¿¡ ÀÖ´ÂÁö?
		if(g_pGOBManager->GetLocalPlayer()) {
			float fLocalX = this->GetPosX();
			float fLocalY = this->GetPosY();
			float fPcX = pPlayer->GetPosX();
			float fPcY = pPlayer->GetPosY();

			if((fPcX > fLocalX - (CHECK_INTERVAL_X - 20) && fPcX < fLocalX + (CHECK_INTERVAL_X - 20)) && 
				(fPcY > fLocalY - (CHECK_INTERVAL_Y - 20) && fPcY < fLocalY + (CHECK_INTERVAL_Y - 20)))
			{
				return true;
			}
		}	
	}

	return false;
}


INT64	SPPlayer::GetCheckStatusValue(CHECK_STATUS iStatus)
{
	int iValue;	
	if((!m_bLocalPlayer && m_eLobbyAvatar != LOBBY_CASH))			return -1;
	switch(iStatus) {
	case CHECK_STATUS_CLASS:
		return m_pPlayerStatus->GetStatusValue(STATUS_CLASS_TYPE);
		break;
	case CHECK_STATUS_GENDER:
		return m_pPlayerStatus->GetStatusValue(STATUS_GENDER);
		break;
	case CHECK_STATUS_LV:
		return m_pPlayerStatus->GetStatusValue(STATUS_LV);
		break;
	case CHECK_STATUS_MONEY:
		return m_pPlayerStatus->GetStatusValueEX(STATUS_EX_ELY);
		break;
	case CHECK_STATUS_EXP:
		return m_pPlayerStatus->GetStatusValueEX(STATUS_EX_EXP);
		break;
	case CHECK_STATUS_HP:
		return m_pPlayerStatus->GetStatusValue(STATUS_HP);
		break;
	case CHECK_STATUS_SP:
		return m_pPlayerStatus->GetStatusValue(STATUS_SP);
		break;
	case CHECK_STATUS_CURR_FAME:
		return m_pPlayerStatus->GetStatusValue(STATUS_FAME);
		break;
	case CHECK_STATUS_BASE_FAME:
		return m_pPlayerStatus->GetStatusValue(STATUS_FAME_BASE);
		break;
	case CHECK_STATUS_CURR_ATK:
		return m_pPlayerStatus->GetStatusValue(STATUS_ATK);
		break;
	case CHECK_STATUS_BASE_ATK:
		return m_pPlayerStatus->GetStatusValue(STATUS_ATK_BASE);
		break;
	case CHECK_STATUS_CURR_LUK:
		return m_pPlayerStatus->GetStatusValue(STATUS_LUK);
		break;
	case CHECK_STATUS_BASE_LUK:
		return m_pPlayerStatus->GetStatusValue(STATUS_LUK_BASE);
		break;
	case CHECK_STATUS_CURR_ELE:
		return m_pPlayerStatus->GetStatusValue(STATUS_ELE);
		break;
	case CHECK_STATUS_BASE_ELE:
		return m_pPlayerStatus->GetStatusValue(STATUS_ELE_BASE);
		break;	
	case CHECK_STATUS_CURR_DEF:
		return m_pPlayerStatus->GetStatusValue(STATUS_DEF);
		break;
	case CHECK_STATUS_BASE_DEF:
		return m_pPlayerStatus->GetStatusValue(STATUS_DEF_BASE);
		break;
	case CHECK_STATUS_CURR_MHP:
		return m_pPlayerStatus->GetStatusValue(STATUS_MHP);
		break;
	case CHECK_STATUS_BASE_MHP:
		return m_pPlayerStatus->GetStatusValue(STATUS_MHP_BASE);
		break;
	case CHECK_STATUS_CURR_MSP:
		return m_pPlayerStatus->GetStatusValue(STATUS_MSP);
		break;
	case CHECK_STATUS_BASE_MSP:
		return m_pPlayerStatus->GetStatusValue(STATUS_MSP_BASE);
		break;
	case CHECK_STATUS_CURR_WATER:
		return m_pPlayerStatus->GetStatusValue(STATUS_WATER);
		break;	
	case CHECK_STATUS_CURR_WIND:
		return m_pPlayerStatus->GetStatusValue(STATUS_WIND);
		break;
	case CHECK_STATUS_CURR_FIRE:
		return m_pPlayerStatus->GetStatusValue(STATUS_FIRE);
		break;	
	case CHECK_STATUS_CURR_EARTH:
		return m_pPlayerStatus->GetStatusValue(STATUS_EARTH);
		break;
	case CHECK_STATUS_SKILL_POINT:
		return m_pPlayerStatus->GetStatusValue(STATUS_SKILL_POINT);
		break;
	case CHECK_STATUS_REQUIRE_LV:
		return m_pPlayerStatus->GetStatusValue(STATUS_REQ_LV);
	case CHECK_STATUS_GUILD_LV:
		return g_pGOBManager->GetGuildArchive()->GetGuildGrade() ;
	
	case CHECK_STATUS_EQUIPMENT_SLOT:
		iValue = m_pPlayerInventory->GetInventorySize(INVENTORY_EQUIP);
		iValue = (iValue - DEFAULT_INVENTORY_SLOT) / 4;
		return iValue;
	case CHECK_STATUS_CONSUMPTION_SLOT:
		iValue = m_pPlayerInventory->GetInventorySize(INVENTORY_CONSUME);
		iValue = (iValue - DEFAULT_INVENTORY_SLOT) / 4;
		return iValue;
	case CHECK_STATUS_ETC_SLOT:
		iValue = m_pPlayerInventory->GetInventorySize(INVENTORY_ETC);
		iValue = (iValue - DEFAULT_INVENTORY_SLOT) / 4;
		return iValue;
	case CHECK_STATUS_EVENT_SLOT:
		iValue = m_pPlayerInventory->GetInventorySize(INVENTORY_EVENT);
		iValue = (iValue - DEFAULT_INVENTORY_SLOT) / 4;
		return iValue;
	case CHECK_STATUS_HOUSING_SLOT:
		iValue = m_pPlayerInventory->GetInventorySize(INVENTORY_HOUSING);
		iValue = (iValue - DEFAULT_INVENTORY_SLOT) / 4;
		return iValue;
	case CHECK_STATUS_PET_SLOT:
		iValue = m_pPlayerInventory->GetInventorySize(INVENTORY_PET);
		iValue = (iValue - DEFAULT_INVENTORY_SLOT) / 4;
		return iValue;
	case CHECK_STATUS_STORAGE_SLOT:		
		iValue = m_pPlayerInventory->GetInventorySize(STORAGE);
		iValue = (iValue - DEFAULT_STORAGE_SIZE) / 4;
		return iValue;

	case CHECK_STATUS_ITEMMIX_EXP:
		{
			SPPlayerStatus*	pPlayerStatus	=	(SPPlayerStatus*)m_pPlayerStatus;

			if( pPlayerStatus == NULL )
				return -1;

			ITEMMIX_INFO*	pItemMixInfo	=	pPlayerStatus->GetItemMix();

			if( pItemMixInfo == NULL )
				return -1;

			return pItemMixInfo->iCurExp;
		}
	case CHECK_STATUS_ITEMMIX_LV:
		{
			SPPlayerStatus*	pPlayerStatus	=	(SPPlayerStatus*)m_pPlayerStatus;

			if( pPlayerStatus == NULL )
				return -1;

			ITEMMIX_INFO*	pItemMixInfo	=	pPlayerStatus->GetItemMix();

			if( pItemMixInfo == NULL )
				return -1;

			return pItemMixInfo->iLv;
		}
	case CHECK_STATUS_ITEMMIX_RARE:
		{
			SPPlayerStatus*	pPlayerStatus	=	(SPPlayerStatus*)m_pPlayerStatus;

			if( pPlayerStatus == NULL )
				return -1;

			ITEMMIX_INFO*	pItemMixInfo	=	pPlayerStatus->GetItemMix();

			if( pItemMixInfo == NULL )
				return -1;

			return pItemMixInfo->iRareProb;
		}
	case CHECK_STATUS_ITEMMIX_SUCCESS:
		{
			SPPlayerStatus*	pPlayerStatus	=	(SPPlayerStatus*)m_pPlayerStatus;

			if( pPlayerStatus == NULL )
				return -1;

			ITEMMIX_INFO*	pItemMixInfo	=	pPlayerStatus->GetItemMix();

			if( pItemMixInfo == NULL )
				return -1;

			return pItemMixInfo->iSuccessProb;
		}
	case CHECK_STATUS_ITEMMIX_OPT:
		{
			SPPlayerStatus*	pPlayerStatus	=	(SPPlayerStatus*)m_pPlayerStatus;

			if( pPlayerStatus == NULL )
				return -1;

			ITEMMIX_INFO*	pItemMixInfo	=	pPlayerStatus->GetItemMix();

			if( pItemMixInfo == NULL )
				return -1;

			return pItemMixInfo->iOptProb;
		}

	case CHECK_STATUS_PVP_LV:
		return m_pPlayerStatus->GetStatusValue(STATUS_PVP_LV);
		break;
	case CHECK_STATUS_PVP_POINT:
		return m_pPlayerStatus->GetStatusValue(STATUS_PVP_POINT);
		break;
	case CHECK_STATUS_LOVE_POINT:		return m_pPlayerStatus->GetStatusValue( STATUS_LOVE_POINT );
	case CHECK_STATUS_LOVE_POINT_MAX:	return m_pPlayerStatus->GetStatusValue( STATUS_LOVE_POINT_MAX );
	case CHECK_STATUS_LOVE_DAY:			return m_pPlayerStatus->GetStatusValue( STATUS_LOVE_DAY );
	case CHECK_STATUS_LOVE_STATE:		return m_pPlayerStatus->GetStatusValue( STATUS_LOVE_STATE );
	case CHECK_STATUS_EXPAND_SLOT:		return m_pPlayerStatus->GetStatusValue( STATUS_EXTENDED_CHARSLOT);
	case CHECK_STATUS_LOVE_LEVEL:		return m_pPlayerStatus->GetStatusValue( STATUS_LOVE_LV );
	case CHECK_STATUS_MOB_RECALL_CNT:	return m_pPlayerStatus->GetStatusValue( STATUS_MOB_COUNT );

	case CHECK_STATUS_RANKING_LEVEL:	return m_pPlayerStatus->GetStatusValue( STATUS_RANKING_LEVEL );
	case CHECK_STATUS_RANKING_PVP:		return m_pPlayerStatus->GetStatusValue( STATUS_RANKING_PVP );	
	case CHECK_STATUS_RANKING_LOVE:		return m_pPlayerStatus->GetStatusValue( STATUS_RANKING_LOVE );
	case CHECK_STATUS_RANKING_GUILD:	return m_pPlayerStatus->GetStatusValue( STATUS_RANKING_GUILD );
	
	
	default	:
		return -1;
	}

	return -1;
}

void SPPlayer::InitFigureInfo()
{
	ZeroMemory(m_nBattleEquipInfo, sizeof(m_nBattleEquipInfo));
	ZeroMemory(m_nFashionEquipInfo, sizeof(m_nBattleEquipInfo));
	ZeroMemory(m_nBasicEquipInfo, sizeof(m_nBattleEquipInfo));

	if( m_pPlayerEquipInfo == NULL )
		return;

	for( int i = 0; i < EQ_END; i++ )
	{
		m_pPlayerEquipInfo->Unload((EQUIP_ID)i);
	}
}


//////////////////////////////////////////////////////////////////////////
//[2005/4/13] - jinhee Stage Management
void SPPlayer::SetStage(int iStage, int iMapGroup,bool bSetStageManager) {
	SetCurStageID(iStage, bSetStageManager);
	SetCurMapGroupID(iMapGroup, bSetStageManager);
}

int	SPPlayer::GetCurStageID() {
	return m_pkCoordPhysics->GetCurStageID();
}

void SPPlayer::SetCurStageID(int iStageID, bool bSetStageManager) {
	m_pkCoordPhysics->SetCurStageID(iStageID);
	if( (IsLocalPlayer() || m_eLobbyAvatar == LOBBY_CASH) && bSetStageManager) {
		g_StageManager.SetCurStageIndex(iStageID);
	}

	//if( m_pkPet )
		//m_pkPet->SetCurStageID(iStageID);
	if (m_pPetMgr)
		m_pPetMgr->SetCurStageID( iStageID );
}

int	SPPlayer::GetCurMapGroupID() {
	return m_pkCoordPhysics->GetCurMapGroupID();
}

void SPPlayer::SetCurMapGroupID(int iMapGroupID, bool bSetStageManager) {
	m_pkCoordPhysics->SetCurMapGroupID(iMapGroupID);
	if( (IsLocalPlayer() || m_eLobbyAvatar == LOBBY_CASH) && bSetStageManager) {
		g_StageManager.SetCurGroupIndex(iMapGroupID);
	}

	//if( m_pkPet )
	//	m_pkPet->SetCurMapGroupID(iMapGroupID);
	if (m_pPetMgr)
		m_pPetMgr->SetCurMapGroupID( iMapGroupID );
}

bool SPPlayer::CheckRun(float fElapsedTime, int iDirection)
{
	if( m_RunControl.iDirection )
	{
		if( m_RunControl.iDirection == iDirection )
		{
			m_RunControl.fKeyPushAccmulateTime += fElapsedTime;
			if( m_RunControl.fKeyPushAccmulateTime >= 0.3f )
				return false;
		}
	}
	else
		m_RunControl.fKeyPushAccmulateTime = 0.0f;

	if( iDirection == 0 && m_RunControl.iDirection != 0 )
	{
		m_RunControl.fKeyTermAccmulateTime += fElapsedTime;
		if( m_RunControl.fKeyTermAccmulateTime >= m_RunControl.fKeyTermTime )
			m_RunControl.iDirection = 0;

		return false;
	}

	if( m_RunControl.iDirection != iDirection )
	{
		m_RunControl.iDirection = iDirection;
		m_RunControl.fKeyTermAccmulateTime = 0.0f;

		return false;
	}

	if( m_RunControl.fKeyTermAccmulateTime > 0.001f &&
		m_RunControl.fKeyTermAccmulateTime < m_RunControl.fKeyTermTime )
		return true;

	return false;
}

void SPPlayer::ChangeFigureItem()
{
	vector< FIGURE_CHANGE_INFO >::iterator iter = m_vFigureItemList.begin();
	if( iter == m_vFigureItemList.end() )
		return;

	if( (*iter).Item.SlotInfo.ContainerType == FIGURE_PET )
	{
		int iItemID = (*iter).Item.iItemID;
		if( (*iter).bOff )
			iItemID = -1;
		
		//OnSetPet(iItemID);
		PET_PLACE iPlace = (PET_PLACE)((*iter).Item.SlotInfo.SlotIndex - 1);
		OnSetMultiPet( iItemID, iPlace );
	}
	else
		ItemSetting((*iter).Item, (*iter).bOff);

	m_vFigureItemList.erase(iter);
	m_fFigureAccmulateTime = 0.0f;
	
	if( m_bActiveWeaponChange && m_vFigureItemList.empty() )
	{
		unsigned int uiBeforeAniIndex, uiAfterAniIndex;
		uiBeforeAniIndex = GetGOBSeqKey(m_iWeaponItemSeq, m_eCurSeqIndex, m_iClass);
		WeaponChange();
		uiAfterAniIndex	= GetGOBSeqKey(m_iWeaponItemSeq, m_eCurSeqIndex, m_iClass);

		if( uiBeforeAniIndex != uiAfterAniIndex )
			m_pFXArchive->DeleteFXGroup( uiBeforeAniIndex );

		RefreshEquipItem();
	}
}


void SPPlayer::SetDeadAction()
{	
	if(m_bLocalPlayer) {				
		std::string strMsg = "";

		//[2007/11/28] - PVP»óÈ²ÀÇ »ç¸ÁÀÎ °æ¿ì
		if(SPLocalizeManager::GetInstance()->IsEnableValue(LV_PVP)) {
			if(g_pkPvpManager->GetPvpStatus() == PVP_STATUS_RESULT) {
				return;
			}
			else if(g_pkPvpManager->GetPvpStatus() == PVP_STATUS_GAMEPLAY) {
				if(g_pResourceManager->GetGlobalString(51000002)) {
					strMsg = g_pResourceManager->GetGlobalString(51000002);
					g_pCheckManager->SetNoticeUse(NOTICE_USE_PLAYER_DEAD_PVP);
					g_pInterfaceManager->SetNotice(strMsg.c_str(), static_cast<SPGameObject*>(this), NOTICE_TYPE_MSG, PVP_REVIVAL_DELAY, SPGM_NOTICE_YES);
					return;
				}
			}
		}

		//Ä³½Ã °Å·¡ ÀÀ´ä ´ë±âÁß »ç¸Á½Ã ³Ñ¾î°£´Ù 
		//Ä³½Ã °Å·¡ ¿Ï·á ¸Þ½ÃÁö°¡ ¿Â °æ¿ì »ç¸Á½Ã Ã³¸®¸¦ ´Ù½Ã ÇØÁØ´Ù 
		if(SPCashTradeManager::GetInstance()->GetCashTradeState() == CASH_TRADE_STATE_WAIT) {
			return;
		}

		if(g_pResourceManager->GetGlobalString(4004001)) {
			strMsg = g_pResourceManager->GetGlobalString(4004001);
		}		
		g_pInterfaceManager->SetNotice(strMsg.c_str(), this);
		g_pCheckManager->SetNoticeUse(NOTICE_USE_PLAYER_DEAD);
		m_iSavePoint = SAVEPOINT_TYPE_MAX;
	}

	return;
}

void SPPlayer::SetShowValue(bool bName, bool bHP, bool bShadow)
{
	if( m_pPlayerStatusModel == NULL )
		return;

	m_pPlayerStatusModel->SetShowValue(bName, bHP, bShadow);
}

void SPPlayer::SetGOBName(const char* pszName)
{
	if( pszName == NULL || m_pPlayerStatusModel == NULL )
		return;

	m_pPlayerStatusModel->SetObjectName(pszName);
}

void SPPlayer::SetGOBNameColorType(int iType)
{
	if( m_pPlayerStatusModel == NULL )
		return;

	m_pPlayerStatusModel->SetNameColorType(iType);
}

const char* SPPlayer::GetGOBName()
{
	if( m_pPlayerStatus )
		return m_pPlayerStatus->GetGOBName() ;
	return NULL ;
}

void SPPlayer::SelectRevivalPortal(SAVEPOINT_TYPE iType)
{
	if(m_bLocalPlayer == false)
		return;

	if(iType == SAVEPOINT_TYPE_TOWN || iType == SAVEPOINT_TYPE_SPOT) {
		m_iSavePoint = iType;
		//g_pInterfaceManager->GetNoticeBox()->Hide();
		int iLevel = m_pPlayerStatus->GetLevel();

		if((iLevel > 10) && g_pCheckManager->CheckDeathItem()) {
			std::string strMsg = "NO Death Handicap... !!";
			if(g_pResourceManager->GetGlobalString(4004011)){
				strMsg.clear();
				strMsg = g_pResourceManager->GetGlobalString(4004011);
			}
			g_pInterfaceManager->SetNotice(strMsg.c_str(), this);
			g_pCheckManager->SetNoticeUse(NOTICE_USE_PLAYER_DEAD_REDUCE);
		}
		else {
			////
			//if(IsDead()) {
			//	CPacket Packet;
			//	Packet.Add((UINT32)CHAR_CS_REVIVAL);
			//	Packet.AddUINT32((UINT32)iType);
			//	g_pNetworkManager->SPPerformMessage(CHAR_CS_REVIVAL, 0, (LPARAM)&Packet );
			//}
			SendRevivalPortal(m_iSavePoint);
		}
	}
	return;
}


bool SPPlayer::SendRevivalPortal(SAVEPOINT_TYPE iType, SPItem* pItem /*= NULL*/)
{
	if(m_bLocalPlayer == false)
		return false;

	if(iType == SAVEPOINT_TYPE_TOWN || iType == SAVEPOINT_TYPE_SPOT) {
		TRASH_ITEM TrashItem;
		ZeroMemory(&TrashItem, sizeof(TRADE_ITEM));
		ITEMNO iItemNo = 0;
		
		if(pItem && pItem->IsEmptyItem() == false) {							//»ç¸ÁÆÐ³ÎÆ¼ °¨¼Ò ¾ÆÀÌÅÛÀÌ ÀÖ°í »ç¿ëÇÏ´Â °æ¿ì
			TrashItem.iContainerSlot.ContainerType = pItem->GetContainerType();
			TrashItem.iContainerSlot.SlotIndex = pItem->GetSlotIndex() + 1;
			TrashItem.iCount = 1;
			iItemNo = *(ITEMNO*)pItem->GetItemStatus()->SPSendMessage(MV_ITEM_GET_ITEMNO);
			
			CPacket Packet;
			Packet.Add((UINT32)CHAR_CS_REVIVAL);
			Packet.AddUINT32((UINT32)iType);
			Packet.AddUINT64((UINT64)iItemNo);
			Packet.AddData(&TrashItem, sizeof(TRADE_ITEM));			
			g_pNetworkManager->SPPerformMessage(CHAR_CS_REVIVAL, 0, (LPARAM)&Packet );	
		}
		else {																	//ÀÏ¹ÝÀûÀÎ °æ¿ì
			CPacket Packet;
			Packet.Add((UINT32)CHAR_CS_REVIVAL);
			Packet.AddUINT32((UINT32)iType);
			Packet.AddUINT64((UINT64)iItemNo);
			Packet.AddData(&TrashItem, sizeof(TRADE_ITEM));			
			g_pNetworkManager->SPPerformMessage(CHAR_CS_REVIVAL, 0, (LPARAM)&Packet );	
		}	
	}
	
	return false;
}


void SPPlayer::SetControl(float fTime)
{
	assert( (IsLocalPlayer() || m_eLobbyAvatar == LOBBY_CASH) );
	if( (!IsLocalPlayer() && m_eLobbyAvatar != LOBBY_CASH) || !m_bControlEnable )
		return;

	//CutIn½Ã Local Player Control Á¦ÇÑ
	if(g_pEventManager->IsCutIn()){		
		m_uiControlAction = ACTION_STOP;
		return;
	}

	//[2005/8/1] User°£ Trade½Ã Control Á¦ÇÑ
	if(this->m_iTrade == TRADE_STATE_TRADE) {
		m_uiControlAction = ACTION_STOP;
		return;
	}

	//[2006/5/30] ³»°¡ »óÁ¡ ±¸¸ÅÀÚÀÎ °æ¿ì Control Á¦ÇÑ ÆÇ¸ÅÁßÀÎ °æ¿ì ÇÇÁ÷½º¿¡¼­ Ã³¸®
	if(m_uiUserShopPlayer) {
		m_uiControlAction = ACTION_STOP;
		return;
	}

	//[2007/12/05] PVP_STATUS_RESULT¿¡¼­ Stop Ã³¸®
	if( g_pkPvpManager )	{
		if( g_pkPvpManager->GetPvpStatus() == PVP_STATUS_RESULT ) {
			m_uiControlAction = ACTION_STOP;
			return ;
		}
	}

	if(SPCashTradeManager::GetInstance()->GetCashTradeState() == CASH_TRADE_STATE_WAIT) {	//[2008/4/25] - cash °Å·¡ ´ë±âÁß
		m_uiControlAction = ACTION_STOP;
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//	Quick Slot »ç¿ëÀ» À§ÇØ¼­ »©ÀÚ!!
	//												AJJIYA [7/14/2005]

	SPPlayerActionManager*	pPlayerActionManager	=	SPPlayerActionManager::GetInstance();

	pPlayerActionManager->Process( fTime );

	m_uiControlAction	=	pPlayerActionManager->GetAction();
	m_iRequestSkillID	=	pPlayerActionManager->GetSkillID(m_ItemSkillContainer);

	pPlayerActionManager->SetAction( ACTION_STOP );
	pPlayerActionManager->SetProcessSkip( FALSE );
}

int SPPlayer::SendActionPacket()
{
	if( (!m_bLocalPlayer && m_eLobbyAvatar != LOBBY_CASH) ) {
		assert(0);
		return -1;
	}

	bool bSendPacket = true;
	if( m_iSendAction >= 0 )
		bSendPacket = false;
	
	if( m_uiControlAction & ACTION_SKILL )
	{
		if( (m_iSendSkillID >= 0) || (m_iSendSkillID == m_iRequestSkillID) )
			bSendPacket = false;
	}

	if( bSendPacket )
	{	
		m_iSendAction = m_uiControlAction;

		g_pMouseCursor->SetType((SPMouseCursor::MOUSE_TYPE)0);
		if( m_uiControlAction & ACTION_SKILL )
		{
			if( SendSkillPacket() >= 0 )
			{
				m_pkRevSkillActivity = NULL;
				m_iSendSkillID = m_iRequestSkillID;
			}
			else
				InitSendAction();
		}
		else
		{	
			//[xialin 2008/07/29] edit >>>
			if ( m_eLobbyAvatar == LOBBY_CASH )
			{
				CPacket Packet;
				LT_POSITION		pos;
				memset(&pos, 0x00, sizeof(pos));
				m_pkCoordPhysics->GetLTPosition(&pos);
				Packet.Add(m_uiControlAction);
				Packet.Add(m_uiSendAttackIndex);
				Packet.Add(&pos, sizeof(pos));
				OnSetMoveCommand((LPARAM)&Packet);
			}
			else
			{
				if ( CAutoInterface::GetInstance().isRecord() )
					CAutoInterface::GetInstance().RecordAction(m_uiControlAction);

				CPacket Packet;
				Packet.Add((UINT32)ACTION_CS_COMMAND);
				Packet.Add(m_uiControlAction);
				Packet.Add(m_uiSendAttackIndex);
				return g_pNetworkManager->SPPerformMessage(ACTION_CS_COMMAND, 0, (LPARAM)&Packet);
			}
			//[xialin 2008/07/29] edit <<<
		}			
	}
	return -1;
}

int SPPlayer::SendSkillPacket()
{
	SPSkillActivity* pkSkillActivity = NULL;
	if( m_ItemSkillContainer.ContainerType == CONTAINER_TYPE_INVALID )
		pkSkillActivity = m_pkSkillArchive->GetSkillActivity(m_iRequestSkillID);
	else
	{
		pkSkillActivity = m_pkSkillArchive->GetItemSkillActivity(m_iRequestSkillID,
			m_ItemSkillContainer.ContainerType, m_ItemSkillContainer.SlotIndex);
	}
		
	if( pkSkillActivity == NULL )
		return -1;

	SPSkill* pkSkill = pkSkillActivity->GetSkill();
	if( pkSkill == NULL )
		return -1;

	if( pkSkill->GetSpecialType() == SST_ITEM_SELL )
	{
		//if( m_pkPet && m_pkPet->IsEnableSpecialSkill(SST_ITEM_SELL) )
			//g_pInterfaceManager->SPChildWndSendMessage(WIID_PET, SPIM_PET_SHOP_OPEN, m_iRequestSkillID, 0);
		if ( m_pPetMgr && m_pPetMgr->IsEnableSpecialSkill( SST_ITEM_SELL ))
			g_pInterfaceManager->SPChildWndSendMessage( WIID_PET, SPIM_PET_SHOP_OPEN, m_iRequestSkillID, 0 );

		return -1;
	}

	if (pkSkill->GetSpecialType() == SST_PET_SHOP)
	{
		if (m_pPetMgr && m_pPetMgr->IsEnableSpecialSkill( SST_PET_SHOP ))
			g_pInterfaceManager->SPChildWndSendMessage( WIID_PET, SPIM_NEWPET_SHOP_OPEN, m_iRequestSkillID, 0 );
	}

	if (pkSkill->GetSpecialType() == SST_PET_CHAT)
	{
		SPPet *pet = NULL;
		pet = m_pPetMgr->GetPet( (PET_PLACE)m_ItemSkillContainer.SlotIndex );

		if (pet && pet->IsToggleOnSpecialSkill( SST_PET_CHAT ))
		{
			SPWindow *pWnd = g_pInterfaceManager->FindWindow( WIID_PETCHAT_EDITOR );
			if (pWnd)
				pWnd->Show();
		}
	}


	CPacket Packet;
	if( pkSkill->GetType() == ST_TOGGLE && pkSkillActivity->GetToggleOn() )
	{
		Packet.Add((UINT32)SKILL_CS_OFFTOGGLE);
		Packet.Add((UINT32)m_iRequestSkillID);
	}
	else
	{
		unsigned char ucFacing = m_pkCoordPhysics->GetDirection();
		if( m_uiControlAction & ACTION_MOVE_LEFT || m_uiControlAction & ACTION_RUN_LEFT )
			ucFacing = FACING_LEFT;
		if( m_uiControlAction & ACTION_MOVE_RIGHT || m_uiControlAction & ACTION_RUN_RIGHT )
			ucFacing = FACING_RIGHT;
		if( m_ItemSkillContainer.ContainerType == CONTAINER_TYPE_INVALID )
		{
			//[xialin 2008/07/29] edit >>>
			if ( m_eLobbyAvatar == LOBBY_CASH )
			{
				LT_POSITION		pos;
				memset(&pos, 0x00, sizeof(pos));
				m_pkCoordPhysics->GetLTPosition(&pos);

				Packet.Add((UINT32)m_iRequestSkillID);
				Packet.Add(&pos, sizeof(pos));
				OnSetSkillUsing((LPARAM)&Packet);
				return 0;
			}
			else
			{
				Packet.Add((UINT32)SKILL_CS_USING);
				Packet.Add((UINT32)m_iRequestSkillID);
				Packet.Add(ucFacing);
			}
			//[xialin 2008/07/29] edit <<<
		}
		else
		{
			CONTAINER_SLOT ContainerSlot;

			ContainerSlot.ContainerType = m_ItemSkillContainer.ContainerType;
			ContainerSlot.SlotIndex = m_ItemSkillContainer.SlotIndex + 1;

			//[xialin 2008/07/29] edit >>>
			if ( m_eLobbyAvatar == LOBBY_CASH )
			{
				LT_POSITION		pos;
				memset(&pos, 0x00, sizeof(pos));
				m_pkCoordPhysics->GetLTPosition(&pos);
				ITEM_SP			stItemSP;
				stItemSP.stSlotInfo = ContainerSlot;
				stItemSP.sItemSP = 0;

				Packet.Add(&stItemSP, sizeof(stItemSP));
				Packet.Add((UINT32)m_iRequestSkillID);
				Packet.Add(&pos, sizeof(pos));
				OnSetItemSkillUsing((LPARAM)&Packet);
				return 0;
			}
			else
			{
				Packet.Add((UINT32)ITEMSKILL_CS_USING);
				Packet.Add(&ContainerSlot, sizeof(CONTAINER_SLOT));
				Packet.Add((UINT32)m_iRequestSkillID);
				Packet.Add(ucFacing);
			}
			//[xialin 2008/07/29] edit <<<
			
		}
	}

	return g_pNetworkManager->SPPerformMessage(SKILL_CS_USING, 0, (LPARAM)&Packet);
}

bool SPPlayer::IsSkillActivity(unsigned int uiSkillID)
{
	SPSkillActivity* pkSkillActivity = m_pkSkillArchive->GetSkillActivity(uiSkillID);
	if( pkSkillActivity == NULL )
		return false;

	bool bRet = pkSkillActivity->GetPossibleUsed();
	return bRet;
}

//int SPPlayer::SendEventPacket(SPEventUnit* pEvent) 
//{
//	if(pEvent == NULL)			return -1;
//	
//	//if(bSendPacket) {	
//		//UINT uiCommand = 0;		
//		//command_convert::Encode(m_uiCurAction, ~m_uiCurAction, uiCommand);
//		CPacket Packet;
//		Packet.Add((UINT32)EVENT_CS_RAISE);
//		Packet.Add((UINT8)pEvent->m_iType);
//		Packet.Add((UINT32)pEvent->m_iEventID);
//		return g_pNetworkManager->SPPerformMessage(EVENT_CS_RAISE, 0, (LPARAM)&Packet);
//	//}
//	
//	return 1;
//}

void SPPlayer::InsertTeamMember(TEAM_MEMBER& tMember)
{
	if( m_pkTeamArchive == NULL )
		return;

	m_pkTeamArchive->InsertTeamPlayer(tMember);

#if !defined(_CHANNEL_TEAM_VERSION_)
	if( (UINT16)g_pNetworkManager->GetServerID() != tMember.wServerID )
		return;
#endif

	SPPlayer* pkMemberChar;
	pkMemberChar = (SPPlayer*)g_pGOBManager->Find(tMember.CharGUID);
	if( pkMemberChar && !pkMemberChar->m_bLocalPlayer )
	{
		pkMemberChar->m_pPlayerStatusModel->SetNameColorType(1);
	}
}

void SPPlayer::DeleteTeamMember(GU_ID iID)
{
	if( m_pkTeamArchive == NULL )
		return;

	m_pkTeamArchive->EraseTeamPlayer(iID);
	SPPlayer* pkMemberChar;
	pkMemberChar = (SPPlayer*)g_pGOBManager->Find(iID);
	if( pkMemberChar && !pkMemberChar->m_bLocalPlayer )
	{
		pkMemberChar->m_pPlayerStatusModel->SetNameColorType(2);
	}
}

void SPPlayer::DeleteTeamMember( INT32 iCharDBKey )
{
	if( m_pkTeamArchive == NULL )
		return;

#if !defined(_CHANNEL_TEAM_VERSION_)
	UINT16			wServerID	=	0;
	GU_ID			iID			=	0;
	TEAM_MEMBER*	pTeamMember	=	m_pkTeamArchive->GetCharInfo( iCharDBKey );

	if( pTeamMember != NULL )
	{
		wServerID	=	pTeamMember->wServerID;
		iID			=	pTeamMember->CharGUID;
	}

	m_pkTeamArchive->EraseTeamPlayer( iCharDBKey );

	if( (UINT16)g_pNetworkManager->GetServerID() != wServerID )
		return;

	SPPlayer* pkMemberChar;
	pkMemberChar = (SPPlayer*)g_pGOBManager->Find(iID);
	if( pkMemberChar && !pkMemberChar->m_bLocalPlayer )
	{
		pkMemberChar->m_pPlayerStatusModel->SetNameColorType(2);
	}
#endif
}

void SPPlayer::ClearTeamMember()
{
	if( m_pkTeamArchive == NULL )
		return;

	SPPlayer* pkMemberChar;
	vector< TEAM_MEMBER* >* pvTeamMemberList = m_pkTeamArchive->GetTeamList();
	vector< TEAM_MEMBER* >::iterator iter = (*pvTeamMemberList).begin();
	while( iter != (*pvTeamMemberList).end() )
	{
		pkMemberChar = (SPPlayer*)g_pGOBManager->Find((*iter)->CharGUID);
		if( pkMemberChar && !pkMemberChar->m_bLocalPlayer )
		{
			pkMemberChar->m_pPlayerStatusModel->SetNameColorType(2);
		}

		++iter;
	}

	m_pkTeamArchive->Clear();
}

void SPPlayer::UpdateTeamStatus()
{
	vector< TEAM_MEMBER* >* pvTeamMemberList = m_pkTeamArchive->GetTeamList();
	vector< TEAM_MEMBER* >::iterator iter = (*pvTeamMemberList).begin();
	
	SPWindowMiniTeam::STATUS_INFO StatusInfo;
	SPPlayer* pkMemberChar;
	SPGOBStatus* pkGOBStatus;

	while( iter != (*pvTeamMemberList).end() )
	{
		//if( (*iter)->CharGUID != GetGUID() )
		{
			strcpy(StatusInfo.szCharName, (*iter)->szCharName);

			pkMemberChar = (SPPlayer*)g_pGOBManager->Find((*iter)->CharGUID);
			if( pkMemberChar )
			{
				pkGOBStatus = pkMemberChar->GetStatus();
				
				StatusInfo.iMaxHP = pkGOBStatus->GetStatusValue(STATUS_MHP);
				StatusInfo.iCurHP = pkGOBStatus->GetStatusValue(STATUS_HP);
				StatusInfo.iMaxSP = pkGOBStatus->GetStatusValue(STATUS_MSP);
				StatusInfo.iCurSP = pkGOBStatus->GetStatusValue(STATUS_SP);
				StatusInfo.bOutofRange = false;
			}
			else
				StatusInfo.bOutofRange = true;

#if !defined(_CHANNEL_TEAM_VERSION_)
			StatusInfo.iServerID			= (int)(*iter)->wServerID;

			if( (int)g_pNetworkManager->GetServerID() != StatusInfo.iServerID )
			{
				StatusInfo.bDifferentChannel	= true;
				StatusInfo.bOutofRange			= true;
			}
			else
			{
				StatusInfo.bDifferentChannel	= false;
			}
#endif

			g_pInterfaceManager->SPPerformMessage( SPIM_TEAM_UPDATE_STATUS , (WPARAM)&StatusInfo , 0 );
		}
		++iter;
	}
}
	
bool SPPlayer::IsTeamMember(GU_ID iID)
{
	if( m_pkTeamArchive == NULL )
		return false;

#if !defined(_CHANNEL_TEAM_VERSION_)
	return m_pkTeamArchive->IsMember( iID , (UINT16)g_pNetworkManager->GetServerID() );
#else
	return m_pkTeamArchive->IsMember(iID);
#endif
}


void SPPlayer::ClearBeAttackedList()
{
	if(m_bLocalPlayer && m_pBeAttackedList) {
		m_pBeAttackedList->ClearAllList();
	}
}


void SPPlayer::DeleteBeAttackedObject(GU_ID iId)
{
	if(m_bLocalPlayer && m_pBeAttackedList) {
		m_pBeAttackedList->DelObject(iId);
	}
}


void SPPlayer::SetQuickSlotSize(int iSize)
{
	if(m_bLocalPlayer && m_pBeAttackedList) {
		m_pBeAttackedList->SetQuickSlotSize(iSize);
	}
}


void SPPlayer::SetEffectActivity(LPARAM lParam, bool bTargetFX)
{
	CPacket* pPacket = (CPacket*)lParam;

	unsigned short iNumofEffect ;
	pPacket->ExtractUINT16(&iNumofEffect);

	SPEffectManager* pkEffectManager = SPEffectManager::GetInstance();
	SPEffectActivity* pkEffectActivity;
	SPEffect* pkEffect;

	bool bCaster = false;
	for( int i = 0; i < iNumofEffect ; i++ )
	{
		EFFECT_DATA  EffectData;
		pPacket->ExtractStruct(&EffectData, sizeof(EFFECT_DATA));

		pkEffect = SPEffectManager::GetInstance()->GetEffect(EffectData.iEffectID);
		if( pkEffect == NULL )
			continue;

		bCaster = false;
		if( GetGUID() == EffectData.iCasterID )
			bCaster = true;

		pkEffectActivity = new SPEffectActivity(pkEffect, EffectData.iEffectLv, (float)EffectData.iRemainTime, bCaster);
		m_pkEffectArchive->AddEffect(EffectData.iInstanceID, pkEffectActivity, bTargetFX);

		//////////////////////////////////////////////////////////////////////////
		//	AJJIYA [11/21/2005]
		//	ÀÌÆåÆ® ºÎÂø FX Ãß°¡
		m_pFXArchive->InsertFXGroup( (SPID_FX_GROUP)pkEffect->GetAttachFXGroup() );
		//////////////////////////////////////////////////////////////////////////

		//EFFECT_MISSILE* pMissileInfo = pkEffect->GetMissileInfo();
		//if( pMissileInfo && pMissileInfo->eType == EMT_MISSILE )
		//{
		//	
		//}


		//#if defined(_DEBUG)
		//		DXUTOutputDebugString("SetEffectID:%d\n", EffectData.iEffectID);
		//#endif

		if( m_bLocalPlayer )
		{
			if( pkEffect->GetDescription() )
			{
				D3DXCOLOR color(0.5294f, 0.7176f, 1.0f, 1.0f);
				g_pInterfaceManager->SPPerformMessage(SPIM_WNDCHAT_MESSAGE_STRING, (WPARAM)pkEffect->GetDescription(), (LPARAM)&color);
			}

			m_pkSkillArchive->SetToggleSkill(EffectData.iEffectID, true);
		}
	}

	// ÀÌÆåÆ®°¡ µé¾î¿Ã¶§ ¾÷µ¥ÀÌÆ®¸¦ ÇØÁà¾ß ÇÑ´Ù. [7/6/2006]
	if( m_bLocalPlayer == true )
	{
		UpdatePetSpecialSkillToggle();
		g_pInterfaceManager->SPChildWndSendMessage( WIID_QUICK_SLOT , SPIM_REFRESH , NULL , NULL );
		g_pInterfaceManager->SPChildWndSendMessage( WIID_SKILL , SPIM_REFRESH , NULL , NULL );
		g_pInterfaceManager->SPChildWndSendMessage( WIID_PET , SPIM_REFRESH , NULL , NULL );
	}
}


//////////////////////////////////////////////////////////////////////////
// SPGOBManager, SPGameObject³¢¸®¸¸ ºÒ·Á¾ß ÇÑ´Ù.
SPGMESSAGE_OBJ_MAP_BEGIN( SPPlayer )
	SPGMESSAGE_OBJ_COMMAND(  SPGM_GETMODEL				,	OnGetModel				)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_GETFACING				,	OnGetFacing				)

	SPGMESSAGE_OBJ_COMMAND(  SPGM_GETPOSX				,	OnGetPosX				)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_GETPOSY				,	OnGetPosY				)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_GETGOBNAME			,	OnGetGOBName			)

	SPGMESSAGE_OBJ_COMMAND(  SPGM_GETMODELUNIT			,	OnGetModelUnit			)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_GETCOORDPHYSICS		,	OnGetCoordPhysics		)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_GETEFFECTARCHIVE		,	OnGetEffectArchive		)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_GETFXARCHIVE			,	OnGetFXArchive			)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_GETSOUNDARCHIVE		,	OnGetSoundArchive		)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_GETGOBSTATUS			,	OnGetGOBStatus			)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_GETWEAPONVISIBLE		,	OnGetWeaponVisible		)
	
	SPGMESSAGE_OBJ_COMMAND(  SPGM_GETSTANDLAYER			,	OnGetStandlayer			)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_GETVELOSITY			,	OnGetVelosity			)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_GETCURANIINDEX		,	OnGetCurAniIndex		)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_GETCURANIFRAME		,	OnGetCurAniFrame		)
	SPGMESSAGE_OBJ_COMMAND(	 SPGM_ISDEAD				,	OnIsDead				)
	SPGMESSAGE_OBJ_COMMAND(	 SPGM_GETTYPE				,	OnGetType				)
	SPGMESSAGE_OBJ_COMMAND(	 SPGM_GETACTION				,	OnGetAction				)
	
	SPGMESSAGE_OBJ_COMMAND(  SPGM_SET_SC_LPPOSITION		,	OnSetLPPosotion			)	
	SPGMESSAGE_OBJ_COMMAND(  SPGM_GOB_MOVE				,	OnSetMoveCommand		)	
	SPGMESSAGE_OBJ_COMMAND(  SPGM_SETFACING				,	OnSetFacing				)	
	SPGMESSAGE_OBJ_COMMAND(  SPGM_SETACTION				,	OnSetAction				)	
	SPGMESSAGE_OBJ_COMMAND(  SPGM_SETSTATUSRENDER		,	OnSetStatusRender		)	
	SPGMESSAGE_OBJ_COMMAND(  SPGM_SETGOBSEQINDEX		,	OnSetGOBSeqIndex		)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_SETIGNORESTAGE		,	OnSetIgnoreStage		)
	
	SPGMESSAGE_OBJ_COMMAND(  SPGM_COMMAND_ACTION		,	OnSetActionCommand		)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_COMMAND_ACTION_COMPLETE,	OnSetActionCompleteCommand )

	SPGMESSAGE_OBJ_COMMAND(  SPGM_EQUIP_DEFAULT_CHANGE	,	OnSetEquipDefaultChange )
	SPGMESSAGE_OBJ_COMMAND(  SPGM_EQUIP_CHANGE			,	OnSetEquipChange		)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_EQUIP_UNLOAD			,	OnSetEquipUnload		)

	SPGMESSAGE_OBJ_COMMAND(  SPGM_SET_SC_DAMAGE_FX		,	OnSetDamageFX			)	
	SPGMESSAGE_OBJ_COMMAND(  SPGM_SET_SC_HEALING_FX		,	OnSetHealingFX			)	
	SPGMESSAGE_OBJ_COMMAND(  SPGM_SET_LEVEL_UP_FX		,	OnLevelUpFX				)	
	SPGMESSAGE_OBJ_COMMAND(  SPGM_SET_FX_GROUP			,	OnFXGroup				)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_SET_FX_GROUP_CLEAR	,	OnFXGroupClear			)

	SPGMESSAGE_OBJ_COMMAND(  SPGM_SET_KNOCKBACK			,	OnSetKnockBack			)

	SPGMESSAGE_OBJ_COMMAND(  SPGM_ITEM_SC_FIGURE_INFOS	,	OnSetEquipInit_Broadcast )
	SPGMESSAGE_OBJ_COMMAND(  SPGM_ITEM_SC_FIGURE_ON		,	OnSetEquipOn_Broadcast )
	SPGMESSAGE_OBJ_COMMAND(  SPGM_ITEM_SC_FIGURE_OFF	,	OnSetEquipOff_Broadcast )
	SPGMESSAGE_OBJ_COMMAND(  SPGM_ITEM_SC_TOGGLE_USE_WEAPON_SET	,	OnSetWeaponChange )
	SPGMESSAGE_OBJ_COMMAND(	 SPGM_ITEM_SC_CONTAINER_INFOS,	OnSetContainerInfos		)

	SPGMESSAGE_OBJ_COMMAND(  SPGM_STAT_CS_LOOTING_ITEM_GAIN		,	OnLootingItemGainReq )
	SPGMESSAGE_OBJ_COMMAND(  SPGM_STAT_CS_LOOTING_ITEM_ALL_GAIN	,	OnLootingItemAllGainReq )
	SPGMESSAGE_OBJ_COMMAND(  SPGM_STAT_SC_LOOTING_ITEM			,	OnLootingItemGain )
	SPGMESSAGE_OBJ_COMMAND(  SPGM_STAT_LOOTING_EXIT				,	OnLootingExit )
	SPGMESSAGE_OBJ_COMMAND(  SPGM_STAT_LOOTING_DELETE			,	OnLootingDelete )

	SPGMESSAGE_OBJ_COMMAND( SPGM_SET_SKILL_INIT,			OnSetSkillInit )
	SPGMESSAGE_OBJ_COMMAND( SPGM_SET_SKILL_INIT_SELECTIVE,	OnSetSkillInitSelective )
	SPGMESSAGE_OBJ_COMMAND( SPGM_SET_SKILL_LIST,			OnSetSkillList)
	SPGMESSAGE_OBJ_COMMAND( SPGM_SET_SKILL_USING,			OnSetSkillUsing)
	SPGMESSAGE_OBJ_COMMAND( SPGM_SET_ITEM_SKILL_USING,		OnSetItemSkillUsing)
	SPGMESSAGE_OBJ_COMMAND( SPGM_SET_LOVE_SKILL_USING,		OnSetLoveSkillUsing)
	SPGMESSAGE_OBJ_COMMAND( SPGM_SET_EFFECT_ACTIVITY,		OnSetEffectActivity)
	SPGMESSAGE_OBJ_COMMAND( SPGM_SET_EFFECT_DELETE,			OnSetEffectDelete)
	SPGMESSAGE_OBJ_COMMAND( SPGM_SET_EFFECT_FXSOUND,		OnEffectFXSound)
	SPGMESSAGE_OBJ_COMMAND( SPGM_SET_EFFECT_ALL_CLEAR,		OnEffectAllClear)
	SPGMESSAGE_OBJ_COMMAND( SPGM_SET_CASTER_EFFECT_FXSOUND,		OnCasterEffectFXSound)
	SPGMESSAGE_OBJ_COMMAND( SPGM_SET_TARGET_EFFECT_FXSOUND,		OnTargetEffectFXSound)
	SPGMESSAGE_OBJ_COMMAND( SPGM_SET_PLAY_SOUND,			OnPlaySound)
	SPGMESSAGE_OBJ_COMMAND( SPGM_ISPLAYER,					OnIsPlayer)
	SPGMESSAGE_OBJ_COMMAND( SPGM_SET_TRANSPARENCY,			OnSetTransparency)
	SPGMESSAGE_OBJ_COMMAND( SPGM_DEL_TRANSPARENCY,			OnDelTransparency)

	SPGMESSAGE_OBJ_COMMAND( SPGM_NOTICE_YES,				OnYes)
	SPGMESSAGE_OBJ_COMMAND( SPGM_NOTICE_NO,					OnNo)

	SPGMESSAGE_OBJ_COMMAND( SPGM_GUILD_INVITE,				OnGuildInviteReceive)
	SPGMESSAGE_OBJ_COMMAND( SPGM_GUILD_SECEDE_ME,			OnGuildSecedeMe)
	
	SPGMESSAGE_OBJ_COMMAND( SPGM_TEAM_INVITE_REQ,			OnTeamInviteRequest)
	SPGMESSAGE_OBJ_COMMAND( SPGM_TEAM_INVITE,				OnTeamInviteReceive)
	SPGMESSAGE_OBJ_COMMAND( SPGM_TEAM_JOIN,					OnTeamJoinReceive)
	SPGMESSAGE_OBJ_COMMAND( SPGM_TEAM_SECEDE_REQ,			OnTeamSecedeRequest)
	SPGMESSAGE_OBJ_COMMAND( SPGM_TEAM_SECEDE,				OnTeamSecedeReceive)
	SPGMESSAGE_OBJ_COMMAND( SPGM_TEAM_EXPEL,				OnTeamExpelReceive)
	SPGMESSAGE_OBJ_COMMAND( SPGM_TEAM_CHANGE_MASTER_REQ,	OnTeamChangeMasterRequest)
	SPGMESSAGE_OBJ_COMMAND( SPGM_TEAM_CHANGE_MASTER,		OnTeamChangeMasterReceive)
	SPGMESSAGE_OBJ_COMMAND( SPGM_TEAM_CHANGE_LOOTING_REQ,	OnTeamChangeLootingRequest)
	SPGMESSAGE_OBJ_COMMAND( SPGM_TEAM_CHANGE_LOOTING,		OnTeamChangeLootingReceive)
	SPGMESSAGE_OBJ_COMMAND( SPGM_TEAM_UPDATE_MEMBER,		OnTeamUpdateMemberReceive)
	SPGMESSAGE_OBJ_COMMAND( SPGM_TEAM_CHANNEL_MOVE,			OnTeamChannelMove)
	
	SPGMESSAGE_OBJ_COMMAND( SPGM_TITLE_CHANGE,				OnTitleChange)
	SPGMESSAGE_OBJ_COMMAND( SPGM_SET_COOLTIME,				OnSetCoolTime)

	SPGMESSAGE_OBJ_COMMAND( SPGM_SET_CONTAINER_INFO,		OnSetContainerInfo)
	SPGMESSAGE_OBJ_COMMAND( SPGM_ITEM_SC_ENCHANT,			OnItemEnchantReceive)
	SPGMESSAGE_OBJ_COMMAND( SPGM_ITEM_SC_UPGRADE,			OnItemUpgradeReceive)
	SPGMESSAGE_OBJ_COMMAND( SPGM_ITEM_SC_EFFECTUPGRADE,		OnItemEffectUpgradeReceive)
	
	SPGMESSAGE_OBJ_COMMAND( SPGM_USERSHOP_SC_OPEN	,		OnUserShopOpen	)
	SPGMESSAGE_OBJ_COMMAND( SPGM_USERSHOP_SC_CLOSE	,		OnUserShopClose	)
	SPGMESSAGE_OBJ_COMMAND( SPGM_USERSHOP_SC_IN		,		OnUserShopIn	)
	SPGMESSAGE_OBJ_COMMAND( SPGM_USERSHOP_SC_OUT	,		OnUserShopOut	)
	SPGMESSAGE_OBJ_COMMAND( SPGM_USERSHOP_SC_BUY	,		OnUserShopBuy	)

	SPGMESSAGE_OBJ_COMMAND( SPGM_PETSHOP_SC_OPEN,			OnPetShopOpen	)
	SPGMESSAGE_OBJ_COMMAND( SPGM_PETSHOP_SC_CLOSE,			OnPetShopClose	)
	SPGMESSAGE_OBJ_COMMAND( SPGM_PETSHOP_SC_IN		,		OnPetShopIn	)
	SPGMESSAGE_OBJ_COMMAND( SPGM_PETSHOP_SC_OUT	,			OnPetShopOut	)
	SPGMESSAGE_OBJ_COMMAND( SPGM_PETSHOP_SC_BUY	,			OnPetShopBuy	)


	SPGMESSAGE_OBJ_COMMAND( SPGM_SET_PET			,		OnSetPet	)

	SPGMESSAGE_OBJ_COMMAND( SPGM_ITEM_SKILL_INSERT	,		OnItemSkillInsert)
	SPGMESSAGE_OBJ_COMMAND( SPGM_ITEM_SKILL_DELETE	,		OnItemSkillDelete)

	SPGMESSAGE_OBJ_COMMAND( SPGM_ITEM_RTREFFECT_INSERT	,	OnItemRtrEffectInsert)
	SPGMESSAGE_OBJ_COMMAND( SPGM_ITEM_RTREFFECT_DELETE	,	OnItemRtrEffectDelete)

	SPGMESSAGE_OBJ_COMMAND( SPGM_PET_SKILL_INSERT	,		OnPetSkillInsert)
	SPGMESSAGE_OBJ_COMMAND( SPGM_PET_SKILL_DELETE	,		OnPetSkillDelete)

	SPGMESSAGE_OBJ_COMMAND( SPGM_SET_GUILD_INFO	,			OnSetGuildInfo)
	SPGMESSAGE_OBJ_COMMAND( SPGM_PET_SETINFO,				SetPetInfo)
	SPGMESSAGE_OBJ_COMMAND( SPGM_PET_UPDATEINFO,			UpdatePetInfo)
	SPGMESSAGE_OBJ_COMMAND( SPGM_PETSHOP_SC_UPDATEITEM,	OnPetShopItemUpdate)
	SPGMESSAGE_OBJ_COMMAND( SPGM_PET_TAKEOFF,				TakeOffPet)
	SPGMESSAGE_OBJ_COMMAND( SPGM_PET_ADD_SSP,				OnPetAddSSP)
	SPGMESSAGE_OBJ_COMMAND( SPGM_PET_CHANGE_NAME,			OnPetChangeName)
	SPGMESSAGE_OBJ_COMMAND( SPGM_PET_SHOUT,					OnPetShout)
	SPGMESSAGE_OBJ_COMMAND( SPGM_PET_LEARN_SKILL,			OnPetLearnSkill)
	SPGMESSAGE_OBJ_COMMAND( SPGM_PET_SHOP_FORCE_CLOSED,		OnPetShopForceClosed)

	SPGMESSAGE_OBJ_COMMAND_RANGE ( SPGM_ITEM_MSG_BEGIN, SPGM_ITEM_MSG_END, OnItemMessage )
	SPGMESSAGE_OBJ_COMMAND_RANGE ( SPGM_STAT_MSG_BEGIN, SPGM_STAT_MSG_END, OnStatMessage )

	//SPGMESSAGE_OBJ_COMMAND( SPGM_EVENT_SC_ENTER		,	OnEventMessage )//[2005/4/25]
	
SPGMESSAGE_OBJ_MAP_END( SPGameObject )



int SPPlayer::OnGetPosX	( LPARAM lParam )
{
	//
	// AJJIYA [5/12/2005]
	// DataConvert°¡ °ªÀÌ ÀÌ»óÇÏ°Ô µÈ´Ù...
/*	FLOAT fX = m_pkCoordPhysics->GetPosX();
	int iRetVal;
	DataConvert<float,int>(fX, iRetVal);
	return iRetVal;
*/
	return static_cast<int>( m_pkCoordPhysics->GetPosX() );
}

int SPPlayer::OnGetPosY	( LPARAM lParam )
{
	//
	// AJJIYA [5/12/2005]
	// DataConvert°¡ °ªÀÌ ÀÌ»óÇÏ°Ô µÈ´Ù...
/*	FLOAT fY = m_pkCoordPhysics->GetPosY();

	int iRetVal;
	DataConvert<float,int>(fY, iRetVal);
	return iRetVal;

	//return (int)  &fY;
*/
	return static_cast<int>( m_pkCoordPhysics->GetPosY() );
}

int SPPlayer::OnGetGOBName( LPARAM lParam )
{
	char* pStrName = (char*)lParam;

	strcpy( pStrName , GetStatus()->GetGOBName() );

	return 0;
}

int SPPlayer::OnGetModelUnit(LPARAM lParam)
{
	SPGOBModelUnit** ppModelUnit = (SPGOBModelUnit**)lParam;

	*ppModelUnit = m_pGOBModelUnit;
	return 0;
}

int SPPlayer::OnGetCoordPhysics(LPARAM lParam)
{
	SPGOBCoordPhysics** ppCoordPhysics = (SPGOBCoordPhysics**)lParam;

	*ppCoordPhysics = m_pkCoordPhysics;
	return 0;
}

int SPPlayer::OnGetEffectArchive(LPARAM lParam)
{
	SPEffectArchive** ppEffectArchive = (SPEffectArchive**)lParam;

	*ppEffectArchive = m_pkEffectArchive;
	return 0;
}

int SPPlayer::OnGetFXArchive(LPARAM lParam)
{
	SPFXArchive** ppFXArchive = (SPFXArchive**)lParam;

	*ppFXArchive = m_pFXArchive;
	return 0;
}

int SPPlayer::OnGetSoundArchive(LPARAM lParam)
{
	SPSoundArchive** ppSoundArchive = (SPSoundArchive**)lParam;

	*ppSoundArchive = m_pSoundArchive;
	return 0;
}

int SPPlayer::OnGetGOBStatus(LPARAM lParam)
{
	SPGOBStatus** ppGOBStatus = (SPGOBStatus**)lParam;

	*ppGOBStatus = m_pPlayerStatus;
	return 0;
}

int SPPlayer::OnGetWeaponVisible(LPARAM lParam)
{
	return m_bWeaponVisible ? 1 : 0;
}
		
int SPPlayer::OnSetEquipDefaultChange	( LPARAM lParam )
{	
	CONTAINER_ITEM* item = (CONTAINER_ITEM*)lParam;	
	
	if (item->SlotInfo.ContainerType == FIGURE_EFFECT)
		return 0;

	SPItemAttr* pItemAttr = NULL;
	if( !g_pItemCluster->GetItemInfo(item->Item.iItemID, pItemAttr) )
		return 0;

	CHAR_EQUIPMENT CharEquipMent;
	//CharEquipMent.iiItemColor = (item->Item.iItemID)%100;
	//CharEquipMent.iEquipClass = (item->Item.iItemID)/100000;
	//CharEquipMent.iiItemID = ((item->Item.iItemID)%100000-CharEquipMent.iiItemColor)/100;	
	//CharEquipMent.uiItemUniqueID = item->Item.iItemID;

	CharEquipMent.iiItemColor = pItemAttr->m_iColorID;
	CharEquipMent.iEquipClass = pItemAttr->m_iCategory;
	CharEquipMent.iiItemID = pItemAttr->m_iNum;
	CharEquipMent.uiItemUniqueID = item->Item.iItemID;
	
	// Item Pool ¿¡¼­ ÀÌ³ðÀÌ µé¾î°¥ À§Ä¡(EQUIP_ID)¸¦ °¡Á®¿Â´Ù.
	int nPosID=0;
	//g_pItemCluster->GetPosID(item->Item.iItemID, nPosID);

	if(pItemAttr) {	
		nPosID = (int)pItemAttr->m_ePosID1;
		CharEquipMent.iEquipID = nPosID;		
		EquipChange_CHAR_EQUIPMENT(CharEquipMent, true);
	} 
	
	return 0;
}

int SPPlayer::OnSetEquipChange			( LPARAM lParam )
{
	CONTAINER_ITEM* item = (CONTAINER_ITEM*)lParam;	

	SPItemAttr* pItemAttr = NULL;
	if( !g_pItemCluster->GetItemInfo(item->Item.iItemID, pItemAttr) )
		return 0;

	//if (m_pkRTREffectMgr)
	//	m_pkRTREffectMgr->AddItemRTREffect( item->Item.iItemID );
	
	if (item->SlotInfo.ContainerType == FIGURE_EFFECT)
		return 0;

	CHAR_EQUIPMENT CharEquipMent;
	CharEquipMent.iiItemColor = pItemAttr->m_iColorID;
	CharEquipMent.iEquipClass = pItemAttr->m_iCategory;
	CharEquipMent.iiItemID = pItemAttr->m_iNum;
	CharEquipMent.uiItemUniqueID = item->Item.iItemID;

	//CHAR_EQUIPMENT CharEquipMent;
	//CharEquipMent.iiItemColor = (item->Item.iItemID)%100;
	//CharEquipMent.iEquipClass = (item->Item.iItemID)/100000;
	//CharEquipMent.iiItemID = ((item->Item.iItemID)%100000-CharEquipMent.iiItemColor)/100;	
	//CharEquipMent.uiItemUniqueID = item->Item.iItemID;

	// Item Pool ¿¡¼­ ÀÌ³ðÀÌ µé¾î°¥ À§Ä¡(EQUIP_ID)¸¦ °¡Á®¿Â´Ù.
	int nPosID=0;
	//g_pItemCluster->GetPosID(item->Item.iItemID, nPosID);
	if(pItemAttr) {	
		nPosID = (int)pItemAttr->m_ePosID1;
		CharEquipMent.iEquipID = nPosID;
		EquipChange_CHAR_EQUIPMENT(CharEquipMent);
	}

	return 0;
}

int SPPlayer::OnSetEquipInit_Broadcast( LPARAM lParam )
{
	if(IsLocalPlayer()) return 0;
	CPacket* pPacket = (CPacket*)lParam;	

	m_vFigureItemList.clear();
	FIGURE_CHANGE_INFO FigureChangeInfo;

	UINT16	nNum;
	pPacket->ExtractUINT16(&nNum);

	for(int i=0; i<nNum; i++)
	{
		FIGURE_ITEM item;
		pPacket->ExtractStruct(&item, sizeof(FIGURE_ITEM));
		assert(item.iItemID!=0);

#if defined(_LAYER_THREAD_LOAD)
		m_FigureItem[i] = item;
#else
		FigureChangeInfo.Item = item;
		FigureChangeInfo.bOff = false;
		//--------------------------------------------------
		// if pet, just show
		if (FigureChangeInfo.Item.SlotInfo.ContainerType == FIGURE_PET)
		{
			int iItemID = FigureChangeInfo.Item.iItemID;
			if( FigureChangeInfo.bOff )
				iItemID = -1;

			PET_PLACE iPlace = (PET_PLACE)(FigureChangeInfo.Item.SlotInfo.SlotIndex - 1);
			OnSetMultiPet( iItemID, iPlace );
		}
		else
		{
			m_vFigureItemList.push_back(FigureChangeInfo);
			//ItemSetting(item);
			OnItemRtrEffectInsert( item.iItemID );
		}
#endif
	}

#if defined(_LAYER_THREAD_LOAD)
	m_iEquipInitNum = nNum;

	DWORD dwID;
	HANDLE hHandle;

	m_bUseThread[0] = true;
	hHandle = CreateThread(NULL, 0, EquipInitThread, this, 0, &dwID);
	CloseHandle(hHandle);
#else
	//WeaponChange();
	if( nNum )
	{
		m_fFigureDelayTime = 0.5f / (float)nNum;
		float fDivide = 100.0f / m_fFigureDelayTime;
		m_fFigureAccmulateTime = (float)(rand() % 100) / fDivide;
	}

	m_bActiveWeaponChange = true;
#endif
	
	return 0;
}

int SPPlayer::OnSetEquipOn_Broadcast( LPARAM lParam )
{
	if(IsLocalPlayer()) return 0;
	CPacket* pPacket = (CPacket*)lParam;

	FIGURE_ITEM item;
	pPacket->ExtractStruct(&item, sizeof(FIGURE_ITEM));

	if( m_vFigureItemList.size() )
	{
		FIGURE_CHANGE_INFO FigureChangeInfo;
		FigureChangeInfo.Item = item;
		FigureChangeInfo.bOff = false;
		m_vFigureItemList.push_back(FigureChangeInfo);
		m_fFigureAccmulateTime = 0.0f;
	}
	else
	{
		if( item.SlotInfo.ContainerType == FIGURE_PET )
		{
			PET_PLACE iPlace = (PET_PLACE)(item.SlotInfo.SlotIndex - 1);
			OnSetMultiPet( item.iItemID, (PET_PLACE)iPlace );
		}
		else
		{
			ItemSetting(item);
			if( item.SlotInfo.ContainerType == FIGURE_BASIC )
				RefreshEquipItem();
		}
	}

	return 0;
}
int SPPlayer::OnSetEquipOff_Broadcast( LPARAM lParam )
{
	if(IsLocalPlayer()) return 0;
	CPacket* pPacket = (CPacket*)lParam;

	FIGURE_ITEM item;
	pPacket->ExtractStruct(&item, sizeof(FIGURE_ITEM));
	
	if( m_vFigureItemList.size() )
	{
		FIGURE_CHANGE_INFO FigureChangeInfo;
		FigureChangeInfo.Item = item;
		FigureChangeInfo.bOff = true;
		m_vFigureItemList.push_back(FigureChangeInfo);
		m_fFigureAccmulateTime = 0.0f;
	}
	else
	{
		if( item.SlotInfo.ContainerType == FIGURE_PET )
		{
			PET_PLACE iPlace = (PET_PLACE)(item.SlotInfo.SlotIndex - 1);
			OnSetMultiPet( -1, (PET_PLACE)iPlace );
		}
		else
			ItemSetting(item, true);
	}

	return 0;
}

int SPPlayer::OnSetWeaponChange(LPARAM lParam)
{	
	int iRet = 0;
	m_iActiveWeapon = lParam;

	if( m_bLocalPlayer )
		m_bWeaponSwapping = false;

#if defined(_LAYER_THREAD_LOAD)
	
	DWORD dwID;
	HANDLE hHandle;

	m_bUseThread[3] = true;
	hHandle = CreateThread(NULL, 0, WeaponChangeThread, this, 0, &dwID);
	CloseHandle(hHandle);

#else

	if( IsLocalPlayer() )
	{
		//m_iActiveWeapon = lParam;
		iRet = m_pPlayerInventory->OnSendMessage( SPGM_ITEM_SC_USE_WEAPON_SET,  m_iActiveWeapon );
		iRet = m_pPlayerInventory->OnSendMessage( SPGM_ITEM_SC_USE_WEAPON_CHANGE,  m_iActiveWeapon );
		//iRet = m_pPlayerInventory->OnSendMessage( SPGM_ITEM_SC_TOGGLE_USE_WEAPON_SET, /*(WPARAM)0,*/ lParam );
	}
	else
	{
		//WeaponChange();
		if( m_vFigureItemList.empty() )
			WeaponChange();
		else
		{
			m_bActiveWeaponChange = true;
			return iRet;
		}
	}

#endif

	return iRet;
}

int SPPlayer::OnSetContainerInfos(LPARAM lParam)
{
	int iRet = 0;
	iRet = m_pPlayerInventory->OnSendMessage( SPGM_ITEM_SC_CONTAINER_INFOS, /*(WPARAM)0,*/ lParam );

	DXUTOutputDebugString("SPPlayer::OnSetContainerInfos ActiveWeapon[%d] - Inven[%d]\n", m_iActiveWeapon, m_pPlayerInventory->GetActiveWeaponSlot());
	
	//[2005/7/12] ¿ø·¡
	m_pPlayerInventory->OnSendMessage(SPGM_ITEM_SC_USE_WEAPON_SET, /*(WPARAM)0,*/ m_iActiveWeapon);	
	
	//[2005/7/12] Æ÷Å»ÅºµÚ ¿þÆù·£´õ¸µ º¯°æÇÏ´Â°Í ÀÓ½Ã·Î ¢À½
	//m_pPlayerInventory->OnSendMessage(SPGM_ITEM_SC_USE_WEAPON_SET, /*(WPARAM)0,*/ m_pPlayerInventory->GetActiveWeaponSlot());
	
	m_pPlayerInventory->OnSendMessage(SPGM_ITEM_SC_USE_WEAPON_CHANGE, /*(WPARAM)0,*/ 0);

	return iRet;
}	


int SPPlayer::OnSetEquipUnload( LPARAM lParam )
{
	m_pPlayerEquipInfo->Unload( (EQUIP_ID)lParam);
	SetWeaponSeqItem();
	return 0;
}


//////////////////////////////////////////////////////////////////////////


int	SPPlayer::OnStatMessage	( SPGM msg, LPARAM lParam )
{
	int iRet = m_pPlayerStatus->SPPerformMessage(msg, lParam);
	
	if( msg == SPGM_SET_SC_CHARINFO )
	{
		m_pPlayerStatusModel->SetObjectName(m_pPlayerStatus->GetGOBName());

		CHARACTER_INFO2* pInfo = (CHARACTER_INFO2*)lParam;
		if( pInfo && m_pkCoordPhysics )
		{
			m_pkCoordPhysics->SetGravity(pInfo->MovingStatus.fDropSpeedNow);
			m_pkCoordPhysics->SetMaxDropVelocity(pInfo->MovingStatus.fDropSpeedMax);
			m_pkCoordPhysics->SetMaxVelocityX(pInfo->MovingStatus.fMaxVelocityX);
			m_pkCoordPhysics->SetJumpSpeed(pInfo->MovingStatus.fJumpSpeed);
			m_pkCoordPhysics->SetMaxUpDownVelocity(pInfo->MovingStatus.fUpDownVelocity);
			m_pkCoordPhysics->SetMaxHangingVelocity(pInfo->MovingStatus.fHangingVelocity);
		}
	}
	else if( msg == SPGM_SET_SC_MOVING_STATUS )
	{
		MOVING_STATUS* pStatus = (MOVING_STATUS*)lParam;
		if( pStatus && m_pkCoordPhysics )
		{
			m_pkCoordPhysics->SetGravity(pStatus->fDropSpeedNow);
			m_pkCoordPhysics->SetMaxDropVelocity(pStatus->fDropSpeedMax);
			m_pkCoordPhysics->SetMaxVelocityX(pStatus->fMaxVelocityX);
			m_pkCoordPhysics->SetJumpSpeed(pStatus->fJumpSpeed);
			m_pkCoordPhysics->SetMaxUpDownVelocity(pStatus->fUpDownVelocity);
			m_pkCoordPhysics->SetMaxHangingVelocity(pStatus->fHangingVelocity);
		}
	}
	else if( msg == SPGM_SET_SC_EFFECT_STATUS )
	{
		EFFECT_STATUS_INFO* pEffectStatus = (EFFECT_STATUS_INFO*)lParam;
		if( pEffectStatus && m_pkCoordPhysics )
		{
			m_pkCoordPhysics->SetGravity(pEffectStatus->MovingStatus.fDropSpeedNow);
			m_pkCoordPhysics->SetMaxDropVelocity(pEffectStatus->MovingStatus.fDropSpeedMax);
			m_pkCoordPhysics->SetMaxVelocityX(pEffectStatus->MovingStatus.fMaxVelocityX);
			m_pkCoordPhysics->SetJumpSpeed(pEffectStatus->MovingStatus.fJumpSpeed);
			m_pkCoordPhysics->SetMaxUpDownVelocity(pEffectStatus->MovingStatus.fUpDownVelocity);
			m_pkCoordPhysics->SetMaxHangingVelocity(pEffectStatus->MovingStatus.fHangingVelocity);
		}
	}
	else if( msg == SPGM_SET_SC_ITEMMIX_INFO )
	{
		ITEMMIX_INFO*		pItemMixInfo	=	(ITEMMIX_INFO*)lParam;
		SPPlayerStatus*		pPlayeerStatus	=	(SPPlayerStatus*)m_pPlayerStatus;

		if( pItemMixInfo != NULL && pPlayeerStatus != NULL )
		{
			pPlayeerStatus->SetItemMix( *pItemMixInfo , false );
		}
	}
	else if (SPGM_STAT_SC_ISGM == msg)
	{
		if (!lParam)
			return iRet;

		int iGM = 0;
		((CPacket*)lParam)->Extract( &iGM, sizeof( int ) );

		if (0 == iGM)
			m_bIsGM = false;
		else if (iGM > 0)
			m_bIsGM = true;
		else
			m_bIsGM = false;

		loadBugleNotifiers();
		char szBuf[64] = {0};
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_GETGOBNAME, (LPARAM)szBuf);
		if (isPlayerNotified( szBuf ))
			g_bNeedBugleHighlight = false;
		else
			g_bNeedBugleHighlight = true;
	}

	return iRet;
}

int SPPlayer::OnSetLPPosotion		( LPARAM lParam )
{
	LT_POSITION* pPosInfo = (LT_POSITION*)lParam;

	m_pkCoordPhysics->SetPos(pPosInfo->fPosX, pPosInfo->fPosY);	

	m_pkCoordPhysics->SetDirection((FACING)pPosInfo->eFacing);
	m_pkCoordPhysics->SetStandLayer((BG_ID)pPosInfo->bgStandLayer);

	m_pkCoordPhysics->SetAccel(pPosInfo->fAccelX, pPosInfo->fAccelY);
	m_pkCoordPhysics->SetCurVelocity(pPosInfo->fVelocityX, pPosInfo->fVelocityY);

	KNOCKBACK_INFO Info;
	Info.bKnockBack = pPosInfo->bKnockBack;
	Info.bDelay = pPosInfo->bDelay;
	Info.fKnockBackTime = pPosInfo->fKnockTime;
	Info.fDelayTime = pPosInfo->fDelayTime;
	Info.fKnockBackVelocityX = pPosInfo->fKnockVelocityX;
	Info.fKnockBackVelocityY = pPosInfo->fKnockVelocityY;

	m_pkCoordPhysics->SetKnockBack(Info);

	return 0;
}

int SPPlayer::OnGetModel		( LPARAM lParam )
{
	return (int)(m_pGOBModelUnit);
}

int SPPlayer::OnGetFacing		( LPARAM lParam )
{
	return m_pkCoordPhysics->GetDirection();
}

int SPPlayer::OnGetStandlayer	( LPARAM lParam )
{	
	return m_pkCoordPhysics->GetStandLayer();
}

int SPPlayer::OnGetVelosity	( LPARAM lParam )
{	
	//return (int)m_pPlayerCoordPhysics->GetVelocityProtage();
	return 0;
}

int SPPlayer::OnGetCurAniIndex( LPARAM lParam )
{
	if( m_pGOBModelUnit == NULL )
		return -1;

	return m_pGOBModelUnit->SPSendMessage(MV_GET_ANIMATION_INDEX);
}

int SPPlayer::OnGetCurAniFrame( LPARAM lParam )
{
	if( m_pGOBModelUnit == NULL )
		return -1;

	return m_pGOBModelUnit->SPSendMessage(MV_GET_CURR_FRAME);
}

int SPPlayer::OnIsDead(LPARAM lParam)
{
	if( IsDead() )
		return 1;

	return 0;
}


int SPPlayer::OnSetActionCommand(LPARAM lParam)
{
	UINT64 uiTempAction = m_uiCurAction;
	UINT64 uiAction = *((UINT64*)lParam);
	uiTempAction |= uiAction;
	SetAction(uiTempAction);
	return 1;
}

int SPPlayer::OnSetActionCompleteCommand(LPARAM lParam)
{
	UINT64 uTempAction = m_uiCurAction;
	UINT64 uiAction = *((UINT64*)lParam);
	uTempAction &= ~uiAction;
	
	if( uiAction != 0xFFFFFFFFFFFFFFFF )
	{
		if( uiAction & ACTION_BEATTACKED_DOWN )
		{
			if( IsDead() )
				uTempAction = ACTION_DEAD;
			else
				uTempAction = ACTION_BEATTACKED_DOWN_STAND;
		}
		
		if( uiAction & ACTION_GUARD_DAMAGE )
			uTempAction = ACTION_GUARD;
	}

	SetAction(uTempAction);
	return 1;
}


int SPPlayer::OnSetMoveCommand	( LPARAM lParam )
{	
	CPacket* pPacket = (CPacket*)lParam;

	// Broadcast command 
	unsigned __int64 uiCommandBuf, uiTemp;
	unsigned int uiRandSeqIndex;
	pPacket->Extract(&uiCommandBuf);
	pPacket->Extract(&uiRandSeqIndex);
	
	m_eAttackRandIndex = (GOB_SEQ_INDEX)uiRandSeqIndex;

	uiTemp = uiCommandBuf;
	m_pkCoordPhysics->IsActionChange(uiTemp);
	SetAction(uiCommandBuf, true);

	if( (m_bLocalPlayer || m_eLobbyAvatar == LOBBY_CASH) )
		InitSendAction();

	
	//
	//m_eCurSeqIndex = (GOB_SEQ_INDEX)uiSeqIndex;

	// Broadcast position
	LT_POSITION		PosInfo;		
	ZeroMemory(&PosInfo,	sizeof(LT_POSITION)		);		
	pPacket->ExtractStruct((LPVOID)&PosInfo,		sizeof(LT_POSITION));		
	this->SPGOBPerformMessage( SPGM_SET_SC_LPPOSITION	,	(WPARAM)&PosInfo	);	

	if( (m_bLocalPlayer || m_eLobbyAvatar == LOBBY_CASH) && m_pPetMgr && m_bPetResetPos )
	{
		m_pPetMgr->SetRePos(GetCurStageID(), GetCurMapGroupID(), GetPosX(), GetPosY());
		m_bPetResetPos = false;
	}

//#if defined(_DEBUG)
//	if( m_bLocalPlayer )
//		DXUTOutputDebugString("\t ------ Rev: m_uiCurAction[%d]\n", m_uiCurAction);
//#endif

	return 0;
}

int SPPlayer::OnSetAction(LPARAM lParam)
{
	UINT64 uiAction = *((UINT64*)lParam);
	SetAction(uiAction, true);
	return 0;
}

int SPPlayer::OnSetStatusRender(LPARAM lParam)
{
	m_bStatusRender = lParam ? true:false;
	return 0;
}

int SPPlayer::OnSetGOBSeqIndex(LPARAM lParam)
{
	m_eAttackRandIndex = (GOB_SEQ_INDEX)lParam;
	return 0;
}

int SPPlayer::OnSetIgnoreStage(LPARAM lParam)
{
	bool bIgnore = lParam ? true : false;
	if( m_pkCoordPhysics )
		m_pkCoordPhysics->SetIgnoreStage(bIgnore);

	return 0;
}

int SPPlayer::OnSetFacing(LPARAM lParam)
{
	FACING eFacing = FACING_LEFT;
	if( lParam > 1 )
		eFacing = FACING_RIGHT;

	m_pkCoordPhysics->SetDirection(eFacing);
	m_bFacingSend = false;
	return 0;
}

int SPPlayer::OnSetKnockBack(LPARAM lParam)
{
	CPacket* pPacket = (CPacket*)lParam;

	LT_KNOCKBACK KnockBackInfo;		
	pPacket->ExtractStruct((LPVOID)&KnockBackInfo, sizeof(LT_KNOCKBACK));
	
	KNOCKBACK_INFO Info;
	Info.bKnockBack = KnockBackInfo.bKnockBack;
	Info.bDelay = KnockBackInfo.bDelay;
	Info.fDelayTime = KnockBackInfo.fDelayTime;
	Info.fKnockBackTime = KnockBackInfo.fKnockTime;
	Info.fKnockBackVelocityX = KnockBackInfo.fKnockVelocityX;
	Info.fKnockBackVelocityY = KnockBackInfo.fKnockVelocityY;

	m_pkCoordPhysics->SetKnockBack(Info);
	return 0;
}

int SPPlayer::OnItemMessage( SPGM msg, /*WPARAM wParam,*/ LPARAM lParam ) 
{
	if(m_pPlayerInventory) {							//[12/2/2005] - jinhee //ÀÌ·±°Ô ¸î¸î ´õ ÀÖÀ¸¸®¶ó ÆÇ´ÜµÊ...
		return m_pPlayerInventory->OnSendMessage( msg, lParam );
	}	

	return 1;
}

//////////////////////////////////////////////////////////////////////////
// FX
int SPPlayer::OnSetDamageFX( LPARAM lParam )
{
	// attackType, FACING, point
	CPacket* pPacket = (CPacket* )lParam;
	int	iDamage, iMode, iType;
	
	pPacket->ExtractUINT32((UINT*)&iDamage);
	pPacket->ExtractUINT32((UINT*)&iMode);
	pPacket->ExtractUINT32((UINT*)&iType);

	InsertDamageFX( FDT_BEATTACK , iDamage , iMode , iType );

	if( m_bLocalPlayer == true )
	{
		// µ¥¹ÌÁö°¡ ¶ß¸é ¿ùµå¸Ê¿¡ È¿°ú¸¦ ÁØ´Ù. AJJIYA [4/18/2006]
		g_pInterfaceManager->SPChildWndSendMessage( WIID_WORLDMAP , SPIM_REFRESH , (WPARAM)iMode , NULL );
	}

	GU_ID	iAttackGuid	=	0;
	pPacket->ExtractUINT64( &iAttackGuid );

#ifndef _USE_OLD_HP_BAR
	////
	//if(!m_bLocalPlayer && g_pGOBManager->GetLocalPlayer()) {
	//	SPBeAttackedList* pBeAttackedList = g_pGOBManager->GetLocalPlayer()->GetBeAttackedList();
	//	if(pBeAttackedList)	{
	//		if(g_pGOBManager->GetLocalPlayer()->GetGUID() == iAttackGuid) {
	//			g_pGOBManager->GetLocalPlayer()->GetBeAttackedList()->AddObject(this, TARGET_OBJECT_TYPE_PLAYER);
	//		}
	//	}		
	//}
#endif

	UINT8	uiHitComboCount	=	0;
	pPacket->ExtractUINT8( &uiHitComboCount );

	if( g_pGOBManager != NULL )
	{
		SPPlayer*	pLocalPlayer = g_pGOBManager->GetLocalPlayer();

		if( pLocalPlayer != NULL )
		{
			if( pLocalPlayer->GetGUID() == iAttackGuid )
			{
				SPFXArchive* pLocalFXArchive	=	NULL;

				pLocalPlayer->SPGOBPerformMessage( SPGM_GETFXARCHIVE , (LPARAM)&pLocalFXArchive );

				if( pLocalFXArchive != NULL )
				{
					if( iDamage > 0 && ( iMode == 0 || iMode == 1 ) &&
						( iType == 10001 || iType == 10002 || iType == 20001 || iType == 20002 || iType == 20003 || iType == 20004 || iType == 20005 ) &&
						uiHitComboCount > 1 )
					{
						InsertHitComboFX( uiHitComboCount , pLocalFXArchive );
					}
				}
			}
		}
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
//	Healing FX

int SPPlayer::OnSetHealingFX( LPARAM lParam )
{
	CPacket* pPacket = (CPacket* )lParam;
	int	iDamage, iMode, iType;

	pPacket->ExtractUINT32((UINT*)&iDamage);
	pPacket->ExtractUINT32((UINT*)&iMode);
	pPacket->ExtractUINT32((UINT*)&iType);

	InsertDamageFX( FDT_HEALING , iDamage , iMode , iType );
	
	return 0;
}

int SPPlayer::OnLevelUpFX( LPARAM lParam )
{
//	m_pFXArchive->SetLPARAM( m_pkCoordPhysics->GetDirection() );
	m_pFXArchive->InsertFXGroup( IDFXGR_LEVEL_UP );

	SetActionSound( 11 , true );

	return 0;
}

int SPPlayer::OnFXGroup	( LPARAM lParam )
{
	//m_pFXArchive->SetLPARAM( m_pkCoordPhysics->GetDirection() );
	m_pFXArchive->InsertFXGroup( (SPID_FX_GROUP) lParam );

	/*
	int iDamage = (int)lParam;
	POINT	ptGOBBox;
	ptGOBBox.x	=	0;
	ptGOBBox.y	=	100;

	for( int i = 0 ; i < iDamage ; i++ )
	{
		m_pFXArchive->SetDamage( rand() % 99 );
		m_pFXArchive->SetGOBBox( ptGOBBox );
		m_pFXArchive->SetAttack( TRUE );
		m_pFXArchive->SetFXGroup( IDFXGR_DAMAGE );
		m_pFXArchive->InsertFXGroup( IDFXGR_DAMAGE );

		m_pFXArchive->InsertFXGroup( (SPID_FX_GROUP)9999 );
	}
	*/

	return 0;
}

int SPPlayer::OnFXGroupClear( LPARAM lParam )
{
	int	iFXGroup	=	(int)lParam;

	if( iFXGroup == 0 )
		m_pFXArchive->Clean();
	else
		m_pFXArchive->DeleteFXGroup( (UINT)iFXGroup );

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// Inventory

int SPPlayer::ItemSetting(FIGURE_ITEM item, bool bOff)
{
	SPItemAttr* pItemAttr = NULL;
	if( !g_pItemCluster->GetItemInfo(item.iItemID, pItemAttr) )
		return 0;

	CHAR_EQUIPMENT CharEquipMent;
	CharEquipMent.iiItemColor = pItemAttr->m_iColorID;
	CharEquipMent.iEquipClass = pItemAttr->m_iCategory;
	CharEquipMent.iiItemID = pItemAttr->m_iNum;
	CharEquipMent.uiItemUniqueID = item.iItemID;

	//CHAR_EQUIPMENT CharEquipMent;
	//CharEquipMent.iiItemColor = (item.iItemID)%100;
	//CharEquipMent.iEquipClass = (item.iItemID)/100000;
	//CharEquipMent.iiItemID = ((item.iItemID)%100000-CharEquipMent.iiItemColor)/100;
	//CharEquipMent.uiItemUniqueID = item.iItemID;
	int nPosID=0;

	//g_pItemCluster->GetPosID(item.iItemID, nPosID);

	if(pItemAttr == NULL) {
		return 0;
	}


	nPosID = (int)pItemAttr->m_ePosID1;

	CharEquipMent.iEquipID = nPosID;	

	int iOnItemID = item.iItemID;
	int iOffItemID = 0;
	switch(item.SlotInfo.ContainerType)
	{
		case FIGURE_EQUIP:
		{
			iOffItemID = m_nBattleEquipInfo[item.SlotInfo.SlotIndex-1];
			m_nBattleEquipInfo[item.SlotInfo.SlotIndex-1] = (bOff)?0:item.iItemID;			
			break;
		}
		case FIGURE_FASHION:
		{
			iOffItemID = m_nFashionEquipInfo[item.SlotInfo.SlotIndex-1];
			m_nFashionEquipInfo[item.SlotInfo.SlotIndex-1] = (bOff)?0:item.iItemID;
			break;
		}
		case FIGURE_BASIC:
		{
			iOffItemID = m_nBasicEquipInfo[item.SlotInfo.SlotIndex-1];
			m_nBasicEquipInfo[item.SlotInfo.SlotIndex-1] = (bOff)?0:item.iItemID;			
			break;
		}
		case FIGURE_EFFECT:
		{
			iOffItemID = m_nEffectEquipInfo[item.SlotInfo.SlotIndex-1];
			m_nEffectEquipInfo[item.SlotInfo.SlotIndex-1] = (bOff)?0:item.iItemID;
			break;
		}
	}

	//--------------------------------------------------
	// rtr effect process
	if (bOff)
	{
		if (iOffItemID != 0)
			OnItemRtrEffectDelete( iOffItemID );
	}
	else
	{
		if (iOffItemID != 0)
		{
			OnItemRtrEffectDelete( iOffItemID );
		}

		OnItemRtrEffectInsert( iOnItemID );
	}
	if (item.SlotInfo.ContainerType == FIGURE_EFFECT)
		return 1;
	//--------------------------------------------------

	if(!bOff)
	{
		//if(item.SlotInfo.ContainerType==FIGURE_EQUIP && item.SlotInfo.SlotIndex>=EQ_WEAPON2)
		//	return 0;

		if( item.SlotInfo.ContainerType==FIGURE_EQUIP )
		{
			if( m_iActiveWeapon == 1 )
			{
				if( item.SlotInfo.SlotIndex == EQ_WEAPON2 ||
					item.SlotInfo.SlotIndex == EQ_BUCKLER2 )
					return 0;
			}
			else
			{
				if( item.SlotInfo.SlotIndex == EQ_WEAPON1 ||
					item.SlotInfo.SlotIndex == EQ_BUCKLER1 )
					return 0;
			}
		}


		// battleÀ» ÀÔÈ÷°í, fashion¿¡ ÀÖÀ»¶§´Â »ý·«.
		if( item.SlotInfo.ContainerType==FIGURE_EQUIP )
		{
			if( m_nFashionEquipInfo[item.SlotInfo.SlotIndex-1] != 0 )
				return 0;
			
			if( item.SlotInfo.SlotIndex == EQ_PANTS || item.SlotInfo.SlotIndex == EQ_CLOAK )
			{
				if( m_nFashionEquipInfo[EQ_BLOUSE - 1] )
					return 0;
			}
		}

		EquipChange_CHAR_EQUIPMENT(CharEquipMent, (item.SlotInfo.ContainerType==FIGURE_BASIC)?true:false);
	}
	else			
	{
		//if(item.SlotInfo.ContainerType==FIGURE_EQUIP && item.SlotInfo.SlotIndex>=EQ_WEAPON2)
		//	return 0;

		if( item.SlotInfo.ContainerType==FIGURE_EQUIP )
		{
			if( m_iActiveWeapon == 1 )
			{
				if( item.SlotInfo.SlotIndex == EQ_WEAPON2 ||
					item.SlotInfo.SlotIndex == EQ_BUCKLER2 )
					return 0;
			}
			else
			{
				if( item.SlotInfo.SlotIndex == EQ_WEAPON1 ||
					item.SlotInfo.SlotIndex == EQ_BUCKLER1 )
					return 0;
			}
		}
			
		if( CharEquipMent.iEquipClass / 100 == 5 )
			CharEquipMent.iEquipID = EQ_ONE_PIECE;

		// battleÀ» ÀÔÈ÷°í, fashion¿¡ ÀÖÀ»¶§´Â »ý·«.
		if( item.SlotInfo.ContainerType==FIGURE_EQUIP )
		{
			if( m_nFashionEquipInfo[item.SlotInfo.SlotIndex-1] != 0 )
				return 0;

			if( item.SlotInfo.SlotIndex == EQ_PANTS || item.SlotInfo.SlotIndex == EQ_CLOAK )
			{
				if( m_nFashionEquipInfo[EQ_BLOUSE - 1] )
					return 0;
			}
		}

		m_pPlayerEquipInfo->Unload((EQUIP_ID)CharEquipMent.iEquipID);
		if( item.SlotInfo.ContainerType==FIGURE_EQUIP )
			SetWeaponSeqItem();

		// fashionÀ» ¹þ±â°í, battle¿¡ ÀÖÀ»¶§´Â battle¿¡ ÀÖ´Â°É ÀÔÇôÁØ´Ù.
		if( item.SlotInfo.ContainerType==FIGURE_FASHION )
		{
			int iSlotIndex[3];
			ZeroMemory(&iSlotIndex, sizeof(int) * 3);
			
			iSlotIndex[0] = m_nBattleEquipInfo[item.SlotInfo.SlotIndex-1];
			if( CharEquipMent.iEquipID == EQ_ONE_PIECE )
			{
				iSlotIndex[1] = m_nBattleEquipInfo[EQ_PANTS - 1];
				iSlotIndex[2] = m_nBattleEquipInfo[EQ_CLOAK - 1];

				for( int i = 0; i < 3; i++ )
				{
					if( iSlotIndex[i] == 0 )
					{
						m_pPlayerEquipInfo->Unload((EQUIP_ID)(i + EQ_BLOUSE));
						continue;
					}

					pItemAttr = NULL;
					if( !g_pItemCluster->GetItemInfo(iSlotIndex[i], pItemAttr) )
						return 0;

					CharEquipMent.iiItemColor = pItemAttr->m_iColorID;
					CharEquipMent.iEquipClass = pItemAttr->m_iCategory;
					CharEquipMent.iiItemID = pItemAttr->m_iNum;
					CharEquipMent.uiItemUniqueID = iSlotIndex[i];
					CharEquipMent.iEquipID = pItemAttr->m_ePosID1;

					EquipChange_CHAR_EQUIPMENT(CharEquipMent);

				}
			}
			else
			{
				pItemAttr = NULL;
				if( !g_pItemCluster->GetItemInfo(iSlotIndex[0], pItemAttr) )
					return 0;

				CharEquipMent.iiItemColor = pItemAttr->m_iColorID;
				CharEquipMent.iEquipClass = pItemAttr->m_iCategory;
				CharEquipMent.iiItemID = pItemAttr->m_iNum;
				CharEquipMent.uiItemUniqueID = iSlotIndex[0];
				CharEquipMent.iEquipID = pItemAttr->m_ePosID1;

				EquipChange_CHAR_EQUIPMENT(CharEquipMent);
			}
		}
	}

	return 0;
}

void	SPPlayer::WeaponChange()
{
	//int tmp;
	int iWeaponItemID, iBucklerItemID;
	
	if( m_iActiveWeapon == 1 )
	{
		iWeaponItemID = m_nBattleEquipInfo[EQ_WEAPON1-1];
		iBucklerItemID = m_nBattleEquipInfo[EQ_BUCKLER1-1];
	}
	else
	{
		iWeaponItemID = m_nBattleEquipInfo[EQ_WEAPON2-1];
		iBucklerItemID = m_nBattleEquipInfo[EQ_BUCKLER2-1];
	}
	
	CHAR_EQUIPMENT CharEquipMent;
	if(iWeaponItemID!=0)		
	{
		SPItemAttr* pItemAttr = NULL;
		g_pItemCluster->GetItemInfo(iWeaponItemID, pItemAttr);
		if( pItemAttr )
		{
			CharEquipMent.iiItemColor = pItemAttr->m_iColorID;
			CharEquipMent.iEquipClass = pItemAttr->m_iCategory;
			CharEquipMent.iiItemID = pItemAttr->m_iNum;
			CharEquipMent.iEquipID = EQ_WEAPON1;
			CharEquipMent.uiItemUniqueID = iWeaponItemID;

			//CharEquipMent.iiItemColor = iWeaponItemID%100;
			//CharEquipMent.iEquipClass = iWeaponItemID/100000;
			//CharEquipMent.iiItemID = (iWeaponItemID%100000-CharEquipMent.iiItemColor)/100;
			//CharEquipMent.iEquipID = EQ_WEAPON1;
			//CharEquipMent.uiItemUniqueID = iWeaponItemID;

			EquipChange_CHAR_EQUIPMENT(CharEquipMent);
		}
	}
	else
	{
		OnSetEquipUnload((LPARAM)EQ_WEAPON1);
		//m_pPlayerEquipInfo->Unload(EQ_WEAPON1);
	}

	if(iBucklerItemID!=0)		
	{
		SPItemAttr* pItemAttr = NULL;
		g_pItemCluster->GetItemInfo(iBucklerItemID, pItemAttr);
		if( pItemAttr )
		{
			CharEquipMent.iiItemColor = pItemAttr->m_iColorID;
			CharEquipMent.iEquipClass = pItemAttr->m_iCategory;
			CharEquipMent.iiItemID = pItemAttr->m_iNum;
			CharEquipMent.iEquipID = EQ_BUCKLER1;
			CharEquipMent.uiItemUniqueID = iBucklerItemID;

			//CharEquipMent.iiItemColor = iBucklerItemID%100;
			//CharEquipMent.iEquipClass = iBucklerItemID/100000;
			//CharEquipMent.iiItemID = (iBucklerItemID%100000-CharEquipMent.iiItemColor)/100;
			//CharEquipMent.iEquipID = EQ_BUCKLER1;
			//CharEquipMent.uiItemUniqueID = iBucklerItemID;

			EquipChange_CHAR_EQUIPMENT(CharEquipMent);
		}
	}
	else
	{
		OnSetEquipUnload((LPARAM)EQ_BUCKLER1);
		//m_pPlayerEquipInfo->Unload(EQ_BUCKLER1);
	}

	m_bActiveWeaponChange = false;
}

int SPPlayer::OnLootingItemGainReq(LPARAM lParam)
{
	CONTAINER_ITEM* pItem = (CONTAINER_ITEM*)lParam;
	if( m_pkCurLootingBag == NULL || pItem == NULL ) {
		m_bLootingAll = false;
		return 0;
	}
	
	if( m_pkCurLootingBag->IsValidItem(pItem) )
	{
		SPItemAttr* pkItemAttr = NULL;
		if( !g_pItemCluster->GetItemInfo(pItem->Item.iItemID, pkItemAttr) )
			return 0;

		if( m_pPlayerInventory->IsFullInventory((CONTAINER_TYPE)(pkItemAttr->m_eType - 1)) ){
			bool bStackEnabel = false;
			
			//¾ÆÀÌÅÛ ½ºÅÃÀ» Á¶»ç
  			bStackEnabel = m_pPlayerInventory->IsEnableSpace((CONTAINER_TYPE)(pkItemAttr->m_eType - 1), pItem->Item.iItemID);
			if(bStackEnabel == false)
				return 0;
			//return 0;
		}
		
		GU_ID iGlobalID = m_pkCurLootingBag->GetGUID();

		LOOTING_ITEM LootingInfo;
		LootingInfo.iSlotIndex = pItem->SlotInfo.SlotIndex;
		LootingInfo.iItemID = pItem->Item.iItemID;

		if(g_pCheckManager->IsDBBlock()) {				//ÀÌ ¾Õ¿¡¼­µµ ¢±â´Â ÇÑ´Ù..
			
			DXUTOutputDebugString("\tSPPlayer::OnLootingItemGainReq DB Working Block\n");
			return 0;
		}

		//unsigned char ucNum = 1;
		CPacket kPacket;
		kPacket.AddUINT32(ITEM_CS_LOOTING);
		kPacket.AddUINT64(iGlobalID);
		//kPacket.AddUINT8(ucNum);
		kPacket.Add(&LootingInfo, sizeof(LOOTING_ITEM));

		(((SPNetworkManager*)g_pNetworkManager)->GetPacketHandler())->PostMsg(&kPacket);

		g_pCheckManager->SetDBBlock(true);			//[2005/11/8] - ÆÖÅ¶´Ü¿¡¼­ ÇØÁ¦...
		
		DXUTOutputDebugString("Request : OnLootingItemGainReq [%d]\n", LootingInfo.iSlotIndex);
	}

	return 0;
}

int SPPlayer::OnPetLootingItemGainReq(LPARAM lParam)
{
	int iRet = 0;
	CONTAINER_ITEM* pItem = (CONTAINER_ITEM*)lParam;
	if( m_pkPetCurLootingBag == NULL || pItem == NULL )
		return iRet;

	if( m_pkPetCurLootingBag->IsValidItem(pItem) )
	{
		SPItemAttr* pkItemAttr = NULL;
		if( !g_pItemCluster->GetItemInfo(pItem->Item.iItemID, pkItemAttr) )
			return iRet;

		if( m_pPlayerInventory->IsFullInventory((CONTAINER_TYPE)(pkItemAttr->m_eType - 1)) )
		{
			bool bStackEnabel = false;
			bStackEnabel = m_pPlayerInventory->IsEnableSpace((CONTAINER_TYPE)(pkItemAttr->m_eType - 1), pItem->Item.iItemID);
			if(bStackEnabel == false)
				return iRet;
		}

		GU_ID iGlobalID = m_pkPetCurLootingBag->GetGUID();

		LOOTING_ITEM LootingInfo;
		LootingInfo.iSlotIndex = pItem->SlotInfo.SlotIndex;
		LootingInfo.iItemID = pItem->Item.iItemID;

		CPacket kPacket;
		kPacket.AddUINT32(ITEM_CS_LOOTING);
		kPacket.AddUINT64(iGlobalID);
		kPacket.Add(&LootingInfo, sizeof(LOOTING_ITEM));

		(((SPNetworkManager*)g_pNetworkManager)->GetPacketHandler())->PostMsg(&kPacket);
		g_pCheckManager->SetDBBlock(true);
		iRet = 1;
	}
	
	return iRet;
}

int SPPlayer::OnLootingItemAllGainReq(LPARAM lParam)
{
	if( m_pkCurLootingBag == NULL ) {
		m_bLootingAll = false;
		return 0;		
	}

	//LOOTING_ITEM LootingInfo;
	unsigned char ucNum;

	GU_ID iGlobalID = m_pkCurLootingBag->GetGUID();

	vector< CONTAINER_ITEM >* pvItemList = m_pkCurLootingBag->GetItemList();
	ucNum = (unsigned char)(*pvItemList).size();
	if( ucNum == 0 )
		return 0;

	m_bLootingAll = true;
	m_iLootingIndex = 0;

	////
	//CPacket kPacket;
	//kPacket.AddUINT32(ITEM_CS_LOOTING);
	//kPacket.AddUINT64(iGlobalID);
	//kPacket.AddUINT8(ucNum);		

	//vector< CONTAINER_ITEM >::iterator iter = (*pvItemList).begin();
	//while( iter != (*pvItemList).end() )
	//{
	//	LootingInfo.iSlotIndex = (*iter).SlotInfo.SlotIndex;
	//	LootingInfo.iItemID = (*iter).Item.iItemID;

	//	kPacket.Add(&LootingInfo, sizeof(LOOTING_ITEM));
	//	++iter;
	//}
	//(((SPNetworkManager*)g_pNetworkManager)->GetPacketHandler())->PostMsg(&kPacket);	
	
	return 0;
}

int SPPlayer::OnLootingItemGain(LPARAM lParam)
{
	g_pCheckManager->SetDBBlock(false);				//[2005/11/8] - DBºí·° ÇØÁ¦

	SPLootingBag* pkBag = (SPLootingBag*)lParam;
	if( pkBag == NULL )
		return 0;
	
	if( m_pkCurLootingBag == pkBag )
	{
		g_pInterfaceManager->SPPerformMessage(
			SPIM_LOOTING_ITEM_DELETE, (WPARAM)(pkBag->GetItemList()), (LPARAM)m_pPlayerInventory);
	}

	return 0;
}

int SPPlayer::OnLootingExit(LPARAM lParam)
{
	m_uiControlAction = ACTION_LOOTING_STAND;
	m_pkCurLootingBag = NULL;
	ParsingControlAction();

	return 0;
}

int SPPlayer::OnLootingDelete(LPARAM lParam)
{
	SPLootingBag* pkBag = (SPLootingBag*)lParam;

	if( m_pkCurLootingBag && m_pkCurLootingBag == pkBag )
		OnLootingExit(0);

	if( m_pkPetCurLootingBag && m_pkPetCurLootingBag == pkBag )
		m_pkPetCurLootingBag = NULL;

	return 0;
}

int SPPlayer::OnSetSkillInit( LPARAM lParam )
{
	CPacket* pPacket = (CPacket* )lParam;

	if( pPacket == NULL )
		return 0;

	short iSkillFreePoint	=	0;

	pPacket->Extract( &iSkillFreePoint , sizeof( iSkillFreePoint ) );

	SetSkillInitData( SSC_SKILL , iSkillFreePoint , true );

	return 0;
}

int SPPlayer::OnSetSkillInitSelective( LPARAM lParam )
{
	CPacket* pPacket = (CPacket* )lParam;

	if( pPacket == NULL )
		return 0;

	UINT32			iMaskingFlag	=	0;

	pPacket->ExtractUINT32( &iMaskingFlag );

	SetSkillInitData( iMaskingFlag , -1 , true );

	return 0;
}

int SPPlayer::OnSetSkillList(LPARAM lParam)
{
	CPacket* pPacket = (CPacket* )lParam;
	CHAR_SKILL SkillInfo;
	unsigned short usCount, i;
	SPSkill* pkSkill;

	pPacket->ExtractUINT16(&usCount);
	for( i = 0; i < usCount; i++ )
	{
		pPacket->ExtractStruct(&SkillInfo, sizeof(CHAR_SKILL));
		
		pkSkill = SPSkillManager::GetInstance()->GetSkill(SkillInfo.iSkillID);
		if( pkSkill )
			m_pkSkillArchive->AddSkill(SkillInfo.iSkillNo, pkSkill, SkillInfo.iSkillLv, SkillInfo.iSkillCurrLv);
	}

	// ½ºÅ³ ½Àµæ½Ã ½ºÅ³À©µµ¿ì·Î ¸Þ¼¼Áö Åëº¸ AJJIYA [7/20/2005]
	g_pInterfaceManager->SPChildWndSendMessage( WIID_SKILL , SPIM_SKILL_UPDATE , NULL , NULL );	// AJJIYA [10/25/2005]
	return 0;
}

int SPPlayer::OnSetSkillUsing(LPARAM lParam)
{
	CPacket* pPacket = (CPacket*)lParam;
	unsigned int uiSkillID;
	
	pPacket->Extract(&uiSkillID);

	if( m_bLocalPlayer )
	{
		InitSendAction();
		m_pkRevSkillActivity = m_pkSkillArchive->GetSkillActivity(uiSkillID);
		if( m_pkRevSkillActivity == NULL )
			return 0;
	}

	m_pkRevSkill = SPSkillManager::GetInstance()->GetSkill(uiSkillID);
	if( m_pkRevSkill == NULL )
		return 0;

	LT_POSITION	PosInfo;		
	ZeroMemory(&PosInfo, sizeof(LT_POSITION));
	pPacket->ExtractStruct((LPVOID)&PosInfo, sizeof(LT_POSITION));

	m_pkCoordPhysics->SetDirection((FACING)PosInfo.eFacing);
	SetAction(ACTION_SKILL, true);
	this->SPGOBPerformMessage(SPGM_SET_SC_LPPOSITION, (WPARAM)&PosInfo);

	return 0;
}

int SPPlayer::OnSetItemSkillUsing(LPARAM lParam)
{
	CPacket* pPacket = (CPacket*)lParam;
	
	ITEM_SP	ItemSP;
	unsigned int uiSkillID;
	ZeroMemory(&ItemSP, sizeof(ITEM_SP));

	pPacket->Extract(&ItemSP, sizeof(ITEM_SP));
	ItemSP.stSlotInfo.SlotIndex-- ;					//¼­¹ö 1 º£ÀÌ½º, Å¬¶óÀÌ¾ðÆ® 0º£ÀÌ½º
	pPacket->Extract(&uiSkillID);

	m_pkRevSkill = SPSkillManager::GetInstance()->GetSkill(uiSkillID);
	if( m_pkRevSkill == NULL )
		return 0;

	if( m_bLocalPlayer )
	{
		InitSendAction();
		
		m_pkRevSkillActivity = m_pkSkillArchive->GetItemSkillActivity(
			uiSkillID, ItemSP.stSlotInfo.ContainerType, ItemSP.stSlotInfo.SlotIndex);
		if( m_pkRevSkillActivity == NULL )
			return 0;

		if(m_pPlayerInventory){				//[12/18/2006 jinssaga]
			if(ItemSP.stSlotInfo.SlotIndex < 0){
				return 0;
			}

			if(m_pPlayerInventory->IsItemExist(ItemSP.stSlotInfo.SlotIndex, ItemSP.stSlotInfo.ContainerType) == false) {
				return 0;
			}

			SPItem* pItem = m_pPlayerInventory->GetInventoryItem(ItemSP.stSlotInfo.ContainerType, ItemSP.stSlotInfo.SlotIndex);
			if(pItem == NULL || pItem->GetItemStatus() == NULL || pItem->GetItemStatus()->GetItemID() == 0) {
				return 0;
			}

			pItem->GetItemStatus()->SetSp(ItemSP.sItemSP);

			if(ItemSP.stSlotInfo.ContainerType == FIGURE_PET)
			{
				if (m_pPetMgr)
				{
					SPPet *pPet = m_pPetMgr->GetPet( (PET_PLACE)(ItemSP.stSlotInfo.SlotIndex) );
					if (pPet)
						pPet->SwitchSkillStatus( m_pkRevSkill->GetSpecialType() );
					g_pInterfaceManager->GetPetWindow()->SPSendMessage(SPIM_PET_STAT_UPDATE, (WPARAM)1, (LPARAM)ItemSP.stSlotInfo.SlotIndex);		
				}
			}
		}
	}

	if( m_pkRevSkill->GetPetActionSeq() && m_pPetMgr)
	{
		m_pPetMgr->SetSkillAnimation((PET_PLACE)(ItemSP.stSlotInfo.SlotIndex),(ANIMATION_INDEX)(m_pkRevSkill->GetPetActionSeq() - 1));
		m_pPetMgr->SetAction((PET_PLACE)(ItemSP.stSlotInfo.SlotIndex),ACTION_SKILL, true);
	}

	LT_POSITION	PosInfo;		
	ZeroMemory(&PosInfo, sizeof(LT_POSITION));
	pPacket->ExtractStruct((LPVOID)&PosInfo, sizeof(LT_POSITION));

	m_pkCoordPhysics->SetDirection((FACING)PosInfo.eFacing);
	SetAction(ACTION_SKILL, true);
	this->SPGOBPerformMessage(SPGM_SET_SC_LPPOSITION, (WPARAM)&PosInfo);
	
	return 0;
}

int SPPlayer::OnSetLoveSkillUsing(LPARAM lParam)
{
	CPacket* pPacket = (CPacket*)lParam;
	unsigned int uiSkillID;

	pPacket->Extract(&uiSkillID);

	if( m_bLocalPlayer )
	{
		InitSendAction();
		m_pkRevSkillActivity = m_pkSkillArchive->GetLoveSkillActivity( uiSkillID );
		if( m_pkRevSkillActivity == NULL )
			return 0;
	}

	m_pkRevSkill = SPSkillManager::GetInstance()->GetSkill(uiSkillID);
	if( m_pkRevSkill == NULL )
		return 0;

	LT_POSITION	PosInfo;		
	ZeroMemory(&PosInfo, sizeof(LT_POSITION));
	pPacket->ExtractStruct((LPVOID)&PosInfo, sizeof(LT_POSITION));

	m_pkCoordPhysics->SetDirection((FACING)PosInfo.eFacing);
	SetAction(ACTION_SKILL, true);
	this->SPGOBPerformMessage(SPGM_SET_SC_LPPOSITION, (WPARAM)&PosInfo);

	return 0;
}

int SPPlayer::OnSetEffectActivity(LPARAM lParam)
{
	SetEffectActivity(lParam);
	return 0;
}

int SPPlayer::OnSetEffectDelete(LPARAM lParam)
{
	CPacket* pPacket = (CPacket*)lParam;

	unsigned short usNum, i;
	unsigned int uiInstanceID;
	pPacket->ExtractUINT16(&usNum);	

	SPEffectActivity* pkEffectActivity;
	SPEffect* pkEffect;
	
	for( i = 0; i < usNum; i++ )
	{
		pPacket->ExtractUINT32(&uiInstanceID);

		//////////////////////////////////////////////////////////////////////////
		//	AJJIYA [11/21/2005]
		//	ÀÌÆåÆ® ºÎÂø FX »èÁ¦
		pkEffectActivity	=	m_pkEffectArchive->GetUniqueKeyActivity( uiInstanceID );
		if( pkEffectActivity != NULL )
		{
			pkEffect	=	pkEffectActivity->GetEffect();
			m_pFXArchive->DeleteFXGroup( pkEffect->GetAttachFXGroup() );

			if( m_bLocalPlayer )
			{
				m_pkSkillArchive->SetToggleSkill(pkEffect->GetID(), false);
			}
		}
		//////////////////////////////////////////////////////////////////////////

		m_pkEffectArchive->RemoveEffect(uiInstanceID);

//#if defined(_DEBUG)
//		DXUTOutputDebugString("Delete EffectID:%d\n", uiInstanceID);
//#endif
	}

	// ÀÌÆåÆ®°¡ µé¾î¿Ã¶§ ¾÷µ¥ÀÌÆ®¸¦ ÇØÁà¾ß ÇÑ´Ù. [7/6/2006]
	if( m_bLocalPlayer == true )
	{
		UpdatePetSpecialSkillToggle();
		g_pInterfaceManager->SPChildWndSendMessage( WIID_QUICK_SLOT , SPIM_REFRESH , NULL , NULL );
		g_pInterfaceManager->SPChildWndSendMessage( WIID_SKILL , SPIM_REFRESH , NULL , NULL );
		g_pInterfaceManager->SPChildWndSendMessage( WIID_PET , SPIM_REFRESH , NULL , NULL );
	}

	return 0;
}

int SPPlayer::OnEffectAllClear(LPARAM lParam)
{
	if( m_pkEffectArchive )
		m_pkEffectArchive->Clear();

	if( m_pFXArchive )
		m_pFXArchive->Clean();

	if( m_pSoundArchive )
		m_pSoundArchive->Clean();

	return 0;
}


//////////////////////////////////////////////////////////////////////////
bool SPPlayer::IsCursorIn(int iX, int iY)
{	
	//int fMouseX = m_fCamX + g_pInput->GetMousePosX();
	//int fMouseY = m_fCamY + g_pInput->GetMousePosY();
	int fMouseX = m_fCamX + iX;
	int fMouseY = m_fCamY + iY;

	if(m_bLocalPlayer)									//ÀÚ±â ÀÚ½ÅÀº Å¬¸¯°ú ¿¬°üÀÌ ¾ø´Ù.
		return false;

	//if(m_iTrade == TRADE_STATE_TRADE)		//³»°¡ °Å·¡Áß¿¡´Â ´Ù¸¥ Ä³¸¯ÅÍ¸¦ Å¬¸¯ÇÒ¼ö ¾ø´Ù.
	//	return false;

	if(g_pEventManager->GetCutInState() > CUT_IN_STATE_NULL)//³»°¡ ÄÆÀÎÁß¿¡´Â ´Ù¸¥ Ä³¸¯ÅÍ¸¦ Å¬¸¯ÇÒ¼ö ¾ø´Ù
		return false;
	
	if((m_pkCoordPhysics->GetPosX() - 18 < fMouseX && m_pkCoordPhysics->GetPosX() + 18 > fMouseX) &&
		(m_pkCoordPhysics->GetPosY() - 110 < fMouseY && m_pkCoordPhysics->GetPosY() > fMouseY))
	{
		return true;
	}
	else if(GetCurAction() & ACTION_USERSHOP) {		
		RECT* pRECT = m_pPlayerStatusModel->GetUserShopBoardRect();						

		int iMouseX = g_pInput->GetMousePosX();
		int iMouseY = g_pInput->GetMousePosY();
		if(pRECT && ((pRECT->left < iMouseX && pRECT->right > iMouseX) && 
			(pRECT->top < iMouseY && pRECT->bottom > iMouseY)))
		//if(pRECT && ((pRECT->left < g_pInput->GetMousePosX() && pRECT->right > g_pInput->GetMousePosX()) && 
		//	(pRECT->top < g_pInput->GetMousePosY() && pRECT->bottom > g_pInput->GetMousePosY())) ) 		
		{
			return true;
		}
	}
	
	return false;
}


int SPPlayer::PerformMouseMove(int iX, int iY)
{
	if(IsCursorIn(iX, iY)) {
		return 1;
	}

	return 0;
}


int SPPlayer::PerformMouseLDown(int iX, int iY)
{
	return 0;
}


int SPPlayer::PerformMouseLUp(int iX, int iY)
{	
	if(IsCursorIn(iX, iY)) {
		//std::string strName = this->GetStatus()->GetGOBName();		
		
		DXUTOutputDebugString("Player LClick uID[%d] Name[]\n", this->GetGUID()/*, this->GetStatus()->GetGOBName()*/);
		return 1;
	}
	return 0;
}


int SPPlayer::PerformMouseRDown(int iX, int iY)
{
	return 0;
}


int SPPlayer::PerformMouseRUp(int iX, int iY)
{
	if(IsCursorIn(iX, iY)) {
		//std::string strName = this->GetStatus()->GetGOBName();		
		
		DXUTOutputDebugString("Player RClick uID[%d] Name[]\n", this->GetGUID()/*, this->GetStatus()->GetGOBName()*/);
		return 1;
	}
	return 0;
}



//////////////////////////////////////////////////////////////////////////
////Event
//int SPPlayer::OnEventMessage( LPARAM lParam )
//{
//	if(m_bLocalPlayer){
//		if((CPacket*)lParam == NULL) return -1;
//		
//		//LT_POSITION* pPosInfo = (LT_POSITION*)lParam;		
//
//		CPacket* pPacket = (CPacket*)lParam;
//
//		/*
//		UINT8 eventType;
//		UINT32 uiEventID;
//		MAP_INFO mapInfo;
//		float posX;
//		float posY;
//		packet->ExtractUINT8(&uiEventID);
//		packet->ExtractUINT8(&mapInfo,sizeof(MAP_INFO));
//		packet->ExtractUINT8(&posX);
//		packet->ExtractUINT8(&posY);
//		*/
//
//		UINT8 eventType;
//		UINT32 uiEventID;
//		MAP_INFO mapInfo;
//		float posX;
//		float posY;
//		pPacket->ExtractUINT8(&eventType);
//		pPacket->ExtractUINT32(&uiEventID);
//		pPacket->Extract(&mapInfo,sizeof(MAP_INFO));
//		pPacket->Extract(&posX);
//		pPacket->Extract(&posY);
//	}
//
//	return -1;
//}


// Send
//int	SPPlayer::SendReqLooting(LOOTING_ITEM item)
//{
//	return m_pPlayerInventory->SendReqLooting( item );	
//}

//int	SPPlayer::SendReqChangeSlot(CHANGE_SLOT item)
//{
//	return m_pPlayerInventory->SendReqChangeSlot( item );	
//
//}
//int	SPPlayer::SendReqTrash(TRASH_ITEM item)
//{
//	return m_pPlayerInventory->SendReqTrash( item );
//}
//


int SPPlayer::OnYes(LPARAM lParam) {
	if(m_bLocalPlayer)
	{
		if( g_pCheckManager->GetNoticeUse() == NOTICE_USE_PARTY_INVITE )
		{
			char szCharName[LEN_NAME + 1];
			strcpy(szCharName, m_pkTeamArchive->GetActCharName());
			if( strlen(szCharName) == 0 )
				return 1;

			CPacket kPacket(TEAM_CS_JOIN);
			kPacket.Add(szCharName, (LEN_NAME + 1) * sizeof(char));

			g_pNetworkManager->GetPacketHandler()->PostMsg(&kPacket);
		}		
		else if(g_pCheckManager->GetNoticeUse() == NOTICE_USE_PLAYER_DEAD) {
			SelectRevivalPortal(SAVEPOINT_TYPE_TOWN);
		}
		else if(g_pCheckManager->GetNoticeUse() == NOTICE_USE_PLAYER_DEAD_REDUCE){
			int iDeathItemID = g_pCheckManager->GetDeathItemID();			
			SPItemAttr* pItemAttr = NULL;
			if(g_pItemCluster->GetItemInfo(iDeathItemID, pItemAttr) == false) {
				SendRevivalPortal(m_iSavePoint);
				return 1;
			}

			if(pItemAttr == NULL) {
				SendRevivalPortal(m_iSavePoint);
				return 1;
			}

			CONTAINER_TYPE iContainer = (CONTAINER_TYPE)(pItemAttr->m_eType - 1);

			SPItem* pItem = m_pPlayerInventory->GetSPItem(iContainer, iDeathItemID);
			if(pItem == NULL) {
				SendRevivalPortal(m_iSavePoint);
				return 1;
			}

			SendRevivalPortal(m_iSavePoint, pItem);
		}
		else if(g_pCheckManager->GetNoticeUse() == NOTICE_USE_PLAYER_DEAD_PVP) {
			CPacket Packet;
			Packet.Add((UINT32)CHAR_CS_PVP_REVIVAL);
			g_pNetworkManager->SPPerformMessage(CHAR_CS_PVP_REVIVAL, 0, (LPARAM)&Packet );	
		}
		else if (g_pCheckManager->GetNoticeUse() == NOTICE_USE_GUILD_INVITE )
		{
			char szCharName[LEN_NAME + 1];
			strcpy(szCharName, g_pGOBManager->GetGuildArchive()->GetInviteName());
			if( strlen(szCharName) == 0 )
				return 1;
			CPacket kPacket(GUILD_CS_JOIN);
			kPacket.Add(szCharName, (LEN_NAME + 1) * sizeof(char));

			g_pNetworkManager->GetPacketHandler()->PostMsg(&kPacket);
		}
		else if( g_pCheckManager->GetNoticeUse() == NOTICE_USE_GUILD_EXCEDE )
		{
			CPacket kPacket(GUILD_CS_SECEDE);
			g_pNetworkManager->GetPacketHandler()->PostMsg(&kPacket);
		}
		else if(g_pCheckManager->GetNoticeUse() == NOTICE_USE_LOVE_SUMMON) {
			SPLoveManager* pLoveManager = SPLoveManager::GetInstance();
			if(pLoveManager) {
				pLoveManager->SendAcceptRecall(true);
			}
		}
		else if(g_pCheckManager->GetNoticeUse() == NOTICE_USE_LOVE_DIVORCE) {
			SPLoveManager* pLoveManager = SPLoveManager::GetInstance();
			if(pLoveManager) {
				pLoveManager->SetWeddingDivorce();
			}
			g_pEventManager->SetCutInState(CUT_IN_STATE_RELEASE);
		}
		else
			return m_pPlayerInventory->OnSendMessage(SPGM_NOTICE_YES, lParam);
	}

	return 1;
}

int SPPlayer::OnNo(LPARAM lParam) {
	if(m_bLocalPlayer)
	{
		if(g_pCheckManager->GetNoticeUse() == NOTICE_USE_PLAYER_DEAD) {
			SelectRevivalPortal(SAVEPOINT_TYPE_SPOT);
		}
		else if(g_pCheckManager->GetNoticeUse() == NOTICE_USE_PLAYER_DEAD_REDUCE) {
			SendRevivalPortal(m_iSavePoint);
		}
		else if(g_pCheckManager->GetNoticeUse() == NOTICE_USE_PLAYER_DEAD_PVP) {
			//CPacket Packet;
			//Packet.Add((UINT32)CHAR_CS_PVP_REVIVAL);
			//g_pNetworkManager->SPPerformMessage(CHAR_CS_PVP_REVIVAL, 0, (LPARAM)&Packet );
			;
		}
		else if( g_pCheckManager->GetNoticeUse() != NOTICE_USE_PARTY_INVITE )
		{
			return m_pPlayerInventory->OnSendMessage(SPGM_NOTICE_NO, lParam);
		}
		else if(g_pCheckManager->GetNoticeUse() == NOTICE_USE_LOVE_SUMMON) {
			SPLoveManager* pLoveManager = SPLoveManager::GetInstance();
			if(pLoveManager) {
				pLoveManager->SendAcceptRecall(false);
			}
		}
		else if(g_pCheckManager->GetNoticeUse() == NOTICE_USE_LOVE_DIVORCE) {
			g_pEventManager->SetCutInState(CUT_IN_STATE_RELEASE);
		}
		else {
			return m_pPlayerInventory->OnSendMessage(SPGM_NOTICE_NO, lParam);
		}
	}

	return 1;
}


int SPPlayer::OnSetContainerInfo(LPARAM lParam)
{
	CPacket* pPacket = (CPacket*)lParam;
	UINT8			iNumOfInfo;		// º¯°æµÉ Á¤º¸¼¼Æ® °¹¼ö
	
	CONTAINER_SLOT_INFO tInfo ;

	//UINT32	iContainerType;	// ÄÁÅ×ÀÌ³Ê Å¸ÀÔ.		(º¯°æµÉ Á¤º¸¼¼Æ®)
	//UINT8			iTotalSlot;		// ÃÑ°¹¼ö (=  ±âº» + OptionSlotTot )
	//UINT8			iOptionSlotTot;	// ¿É¼Ç ½½·ÔÀÇ ÃÑ°¹¼ö.(º¯°æµÉ Á¤º¸¼¼Æ®)	

	pPacket->ExtractUINT8(&iNumOfInfo);
	
	for(int i = 0 ; i < iNumOfInfo ;i++) 
	{

		pPacket->ExtractStruct(&tInfo, sizeof(CONTAINER_SLOT_INFO));
		//pPacket->ExtractUINT32(&iContainerType);
		//pPacket->ExtractUINT8(&iTotalSlot);
		//pPacket->ExtractUINT8(&iOptionSlotTot);
		
		//m_pPlayerInventory->SetExtendSlot((CONTAINER_TYPE)iContainerType, iTotalSlot, iOptionSlotTot);

		m_pPlayerInventory->SetExtendSlot((CONTAINER_TYPE)tInfo.ContainerType, tInfo.iContainerSlot, tInfo.iOptionalSlot);
	}
	return 1;
}

int SPPlayer::OnEffectFXSound(LPARAM lParam)
{
	FX_SOUND_INFO* pFXSoundInfo = (FX_SOUND_INFO*)lParam;
	SPEffect* pkEffect;
	int i, iEffectID;
	float fPosX, fPosY;
	FACING eFacing;

	fPosX = GetPosX();
	fPosY = GetPosY();
	eFacing = m_pkCoordPhysics->GetDirection();
	
	SPFXArchive* pkFXArchive;
	EFFECT_MISSILE* pMissileInfo;
	EFFECT_FX_SOUND* pEffectFXSound;
	for( i = 0; i < 4; i++ )
	{
		if( iEffectID = pFXSoundInfo->pApplyEffect->iTargetEffect[i] )
		{
			pkEffect = SPEffectManager::GetInstance()->GetEffect(iEffectID);
			if( pkEffect == NULL )
				continue;

			pMissileInfo = pkEffect->GetMissileInfo();
			pEffectFXSound = pkEffect->GetFXSoundInfo();

			vector< SPGameObject* >::iterator iter = pFXSoundInfo->vTargetList.begin();
			while( iter != pFXSoundInfo->vTargetList.end() )
			{
				if( pMissileInfo->eType == EMT_MISSILE )
					m_pFXArchive->AddNextTargetGOBID((*iter)->GetGUID());
				else
				{
					(*iter)->SPGOBPerformMessage(SPGM_GETFXARCHIVE, (LPARAM)&pkFXArchive);
					pkFXArchive->SetTargetGOBID( (*iter)->GetGUID() );
					pkFXArchive->InsertFXGroup((SPID_FX_GROUP)pEffectFXSound->iTargetFX);

					if( pEffectFXSound->iTargetSound )
						(*iter)->SPGOBPerformMessage(SPGM_SET_PLAY_SOUND, (LPARAM)pEffectFXSound->iTargetSound);
				}

				++iter;
			}

			if( pMissileInfo->eType == EMT_MISSILE )
			{
				if( pFXSoundInfo->vTargetList.size() )
					m_pFXArchive->AddNextFXGroupID((SPID_FX_GROUP)pEffectFXSound->iTargetFX);

				m_pFXArchive->SetSrcPosX(fPosX);
				m_pFXArchive->SetSrcPosY(fPosY);
				m_pFXArchive->SetVelocity(pMissileInfo->iSpeed);
				m_pFXArchive->SetLoop(TRUE);
//				m_pFXArchive->SetLPARAM((LPARAM)eFacing);

				if( pFXSoundInfo->pkMissileTarget )
					m_pFXArchive->SetTargetGOBID(pFXSoundInfo->pkMissileTarget->GetGUID());
				else
				{
					if( eFacing == FACING_LEFT )
						m_pFXArchive->SetDestPosX(fPosX - 500.0f);
					else
						m_pFXArchive->SetDestPosX(fPosX + 500.0f);

					m_pFXArchive->SetDestPosY(fPosY);
				}

				m_pFXArchive->InsertFXGroup((SPID_FX_GROUP)pMissileInfo->iTargetFX);
			}
		}
	}

//#if defined(_DEBUG)
//	DXUTOutputDebugString("TargetNum: %d\n", pFXSoundInfo->vTargetList.size());
//#endif
	
	return 1;
}

int SPPlayer::OnCasterEffectFXSound(LPARAM lParam)
{
	EFFECT_FX_SOUND* pFXSound = (EFFECT_FX_SOUND*)lParam;
	if( pFXSound == NULL )
		return 0;

	if( pFXSound->iCasterFX )
	{
		if( m_pFXArchive )
		{
			m_pFXArchive->InsertFXGroup((SPID_FX_GROUP)pFXSound->iCasterFX);
		}
	}

	if( pFXSound->iCasterSound )
		SetActionSound(pFXSound->iCasterSound, true);

	return 0;
}

int SPPlayer::OnTargetEffectFXSound(LPARAM lParam)
{
	EFFECT_FX_SOUND* pFXSound = (EFFECT_FX_SOUND*)lParam;
	if( pFXSound == NULL )
		return 0;

	if( pFXSound->iTargetFX )
	{
		if( m_pFXArchive )
		{
			m_pFXArchive->InsertFXGroup((SPID_FX_GROUP)pFXSound->iTargetFX);
		}
	}

	if( pFXSound->iTargetSound )
		SetActionSound(pFXSound->iTargetSound, true);

	return 0;
}

int SPPlayer::OnPlaySound(LPARAM lParam)
{
	SetActionSound(lParam, true);
	return 0;
}

int SPPlayer::OnIsPlayer(LPARAM lParam)
{
	return 1;
}

int SPPlayer::OnSetTransparency(LPARAM lParam)
{
	if( m_pGOBModelUnit )
		m_pGOBModelUnit->SPSendMessage(MV_SET_TRANSPARENCY, 1);

	return 0;
}

int SPPlayer::OnDelTransparency(LPARAM lParam)
{
	if( m_pGOBModelUnit )
		m_pGOBModelUnit->SPSendMessage(MV_SET_TRANSPARENCY, 0);

	return 0;
}

int SPPlayer::OnGetType(LPARAM lParam)
{
	return 0;
}

int SPPlayer::OnGetAction(LPARAM lParam)
{
	*((UINT64*)lParam) = m_uiCurAction;
	return 0;
}


int SPPlayer::OnGuildInviteReceive(LPARAM lParam)
{
	if( !m_bLocalPlayer )
		return 1;

	CPacket* pkPacket = (CPacket*)lParam;

	GU_ID	lObjectID ;
	char	szGuildName[GUILD_MAX_NAME+1];
	char	szCharName[LEN_NAME+1];
	char	szMsg[256];
	GLOBAL_STRING_ID ErrorID;

	pkPacket->ExtractUINT64(&lObjectID);
	pkPacket->ExtractStruct(szGuildName, sizeof(char)*(GUILD_MAX_NAME+1));
	pkPacket->ExtractStruct(szCharName, sizeof(char)*(LEN_NAME+1));
	pkPacket->ExtractUINT32(&ErrorID);

	if( ErrorID != 0 )
	{
		if( g_pResourceManager->GetGlobalString(ErrorID) )
			wsprintf(szMsg, g_pResourceManager->GetGlobalString(ErrorID));
		else
			wsprintf(szMsg, "[Guild]Can't Invite %s", szCharName);
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMsg);
	}
	else
	{
		wsprintf(szMsg, g_pResourceManager->GetGlobalString(27000031), szGuildName, szCharName);

		g_pInterfaceManager->SetNotice(szMsg, this);
		g_pCheckManager->SetNoticeUse(NOTICE_USE_GUILD_INVITE);
		g_pGOBManager->GetGuildArchive()->SetInviteName(szCharName) ;
	}

	return 1;
}


int SPPlayer::OnGuildSecedeMe(LPARAM lParam)
{
	if( !m_bLocalPlayer )
		return 1;

	char	szMsg[256];

	wsprintf(szMsg, g_pResourceManager->GetGlobalString(27000062) );
	g_pInterfaceManager->SetNotice(szMsg, this);
	g_pCheckManager->SetNoticeUse(NOTICE_USE_GUILD_EXCEDE);

	return 1;
}



int SPPlayer::OnTeamInviteRequest(LPARAM lParam)
{
	if( !m_bLocalPlayer )
		return 1;

	// If PVP Zone then Block
	if( g_pkPvpManager->IsPvpZone() )
	{
		char	szMsg[256];
		if( g_pResourceManager->GetGlobalString(51000021) )
			wsprintf(szMsg, g_pResourceManager->GetGlobalString(51000021));
		else
			wsprintf(szMsg, "[Team]You can't use it in PVP zone!!");
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMsg);
		return 1 ;
	}

	if( m_pkTeamArchive->GetTeamCount() >= 4 )
		return 1;

	const char* pszCharName = (const char*)lParam;
	if( pszCharName == NULL )
		return 1;

	int iLength = strlen(pszCharName);
	if( iLength == 0 || iLength > LEN_NAME )
		return 1;

	if( strcmp(m_pPlayerStatus->GetGOBName(), pszCharName) == 0 )
		return 1;

	CPacket kPacket(TEAM_CS_REQUEST);

	char szBuf[LEN_NAME + 1];
	strcpy(szBuf, pszCharName);
	kPacket.Add(szBuf, (LEN_NAME + 1) * sizeof(char));

	g_pNetworkManager->GetPacketHandler()->PostMsg(&kPacket);
	return 1;
}

int SPPlayer::OnTeamInviteReceive(LPARAM lParam)
{
	if( !m_bLocalPlayer )
		return 1;

	CPacket* pkPacket = (CPacket*)lParam;

	char szCharName[LEN_NAME+1];
	GLOBAL_STRING_ID ErrorID;

	pkPacket->ExtractStruct(szCharName, sizeof(char)*(LEN_NAME+1));
	pkPacket->ExtractUINT32(&ErrorID);

	if( ErrorID != 0 )
	{
		char szMsg[512];
		if( ErrorID == 4000003 )
			wsprintf(szMsg, g_pResourceManager->GetGlobalString(ErrorID));
		else
			wsprintf(szMsg, g_pResourceManager->GetGlobalString(ErrorID), szCharName);
		
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMsg);
	}
	else
	{
		char szMsg[128];
		wsprintf(szMsg, g_pResourceManager->GetGlobalString(4002000), szCharName);

		g_pInterfaceManager->SetNotice(szMsg, this);
		g_pCheckManager->SetNoticeUse(NOTICE_USE_PARTY_INVITE);
		m_pkTeamArchive->SetActCharName(szCharName);
	}

	return 1;
}

int SPPlayer::OnTeamJoinReceive(LPARAM lParam)
{
	if( !m_bLocalPlayer )
		return 1;

	CPacket* pkPacket = (CPacket*)lParam;

	char szActCharName[LEN_NAME+1], szRcvCharName[LEN_NAME+1], szMsg[512], szLootingMsg[512];
	GU_ID RcvCharGUID;
	GLOBAL_STRING_ID ErrorID;
	UINT8 byLootingRule;
	int	iCnt, i;
	TEAM_MEMBER	MemberInfo;

	pkPacket->ExtractStruct(szActCharName, sizeof(char)*(LEN_NAME+1));
	pkPacket->ExtractUINT64(&RcvCharGUID);
	pkPacket->ExtractStruct(szRcvCharName, sizeof(char)*(LEN_NAME+1));
	pkPacket->ExtractUINT32(&ErrorID);

	if( ErrorID != 0 )
	{
		szMsg[0] = '\0';
		if( ErrorID == 5013000 )
			wsprintf(szMsg, g_pResourceManager->GetGlobalString(ErrorID), szActCharName);
		else if( ErrorID == 4002004 )
			wsprintf(szMsg, g_pResourceManager->GetGlobalString(ErrorID), szActCharName);
		
		g_pInterfaceManager->SPPerformMessage(SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMsg);
	} 
	else
	{
		pkPacket->ExtractUINT8(&byLootingRule);
		pkPacket->ExtractUINT32((UINT32*)&iCnt);

		ClearTeamMember();
		for( i = 0; i < iCnt; i++ )
		{
			pkPacket->ExtractStruct(&MemberInfo, sizeof(TEAM_MEMBER));
			InsertTeamMember(MemberInfo);

#if !defined(_CHANNEL_TEAM_VERSION_)
			if( (UINT16)g_pNetworkManager->GetServerID() != MemberInfo.wServerID )
				continue;
#endif

			if( MemberInfo.byGrade )
			{
				if( GetGUID() == MemberInfo.CharGUID )
					m_pkTeamArchive->SetMaster(true);
			}

			// Á¶ÀÎµÇ´Â ÆÀ¸â¹ö ¹Ì´Ï¸Ê ¾ÆÀÌÄÜ º¯°æ [2/15/2006 AJJIYA]
			// ÀÚ±â ÀÚ½ÅÀº º¯°æÇÏÁö ¾Ê´Â´Ù.
			if( GetGUID() != MemberInfo.CharGUID )
				g_pInterfaceManager->SPChildWndSendMessage( WIID_MINIMAP , SPIM_REFRESH_ICON_TEAM , 1 , (LPARAM)&MemberInfo.CharGUID );
		}

		switch( byLootingRule )
		{
		case 0:
			wsprintf(szLootingMsg, g_pResourceManager->GetGlobalString(4002008));
			break;
		case 1:
			wsprintf(szLootingMsg, g_pResourceManager->GetGlobalString(4002010));
			break;
		case 2:
			wsprintf(szLootingMsg, g_pResourceManager->GetGlobalString(4002009));
			break;
		default:
			szLootingMsg[0] = '\0';
			break;
		}

		if( iCnt )
		{
			g_pInterfaceManager->ShowWindow(WIID_MINI_TEAM, true);
			
			GU_ID iLocalID = GetGUID();
			g_pInterfaceManager->SPPerformMessage(
				SPIM_TEAM_JOIN, (WPARAM)(m_pkTeamArchive->GetTeamList()), (LPARAM)&iLocalID);

			int iPalletTeamIcon = 0;
			if( m_pkTeamArchive->IsMaster() )
				iPalletTeamIcon = 1;

			g_pInterfaceManager->SPPerformMessage(SPIM_TEAM_ICON_UPDATE, iPalletTeamIcon, 0);
			g_pInterfaceManager->SPPerformMessage(SPIM_TEAM_UPDATE_LOOTING_RULE, byLootingRule, iPalletTeamIcon);
			
			szMsg[0] = '\0';
			if( iCnt == 2 )
			{
				if( m_pkTeamArchive->IsMaster() )
					wsprintf(szMsg, g_pResourceManager->GetGlobalString(4002002), szActCharName);
				else
					wsprintf(szMsg, g_pResourceManager->GetGlobalString(4002003), szActCharName);

				g_pInterfaceManager->SPPerformMessage(SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMsg);
				g_pInterfaceManager->SPPerformMessage(SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szLootingMsg);
			}
			else
			{
				if( GetGUID() == RcvCharGUID )
					wsprintf(szMsg, g_pResourceManager->GetGlobalString(4002003), szActCharName);
				else
					wsprintf(szMsg, g_pResourceManager->GetGlobalString(4002002), szRcvCharName);

				g_pInterfaceManager->SPPerformMessage(SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMsg);
				if( GetGUID() == RcvCharGUID )
					g_pInterfaceManager->SPPerformMessage(SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szLootingMsg);
			}
		}
	}

	return 1;
}

int SPPlayer::OnTeamSecedeRequest(LPARAM lParam)
{
	CPacket kPacket;
	if( IsMaster() )
	{
		const char* pszCharName = (const char*)lParam;
		GU_ID CharID = m_pkTeamArchive->GetCharGUID(pszCharName);

		if( GetGUID() == CharID )
			kPacket.AddUINT32(TEAM_CS_SECEDE);
		else
		{
			kPacket.AddUINT32(TEAM_CS_EXPEL);

#if !defined(_CHANNEL_TEAM_VERSION_)
			INT32 iCharDBKey	=	m_pkTeamArchive->GetCharDBKey( pszCharName );
			kPacket.AddUINT32( iCharDBKey );
#else
			kPacket.AddUINT64(CharID);
#endif
		}
	}
	else
		kPacket.AddUINT32(TEAM_CS_SECEDE);
	
	g_pNetworkManager->GetPacketHandler()->PostMsg(&kPacket);
	return 1;
}

int SPPlayer::OnTeamSecedeReceive(LPARAM lParam)
{
	CPacket* pkPacket = (CPacket*)lParam;

	GU_ID CharGUID	=	0;

#if !defined(_CHANNEL_TEAM_VERSION_)
	INT32 iCharDBKey;

	pkPacket->ExtractUINT32((UINT32*)&iCharDBKey);

	TEAM_MEMBER*	pTeamInfo	=	m_pkTeamArchive->GetCharInfo( iCharDBKey );

	if( pTeamInfo != NULL )
	{
		if( (UINT16)g_pNetworkManager->GetServerID() == pTeamInfo->wServerID )
			CharGUID	=	pTeamInfo->CharGUID;
	}
#else
	pkPacket->ExtractUINT64(&CharGUID);
#endif

	char szMsg[512];
	szMsg[0] = '\0';
	if( GetGUID() == CharGUID )
	{
		// ³»°¡ ÆÀÀ» Å»Åð ÇßÀ»°æ¿ì [2/15/2006 AJJIYA]
		// ¸É¹ö¹Ì´Ï¸Ê ¾ÆÀÌÄÜÀ» ¿ø·¡´ë·Î µ¹¸°´Ù.
		std::vector< TEAM_MEMBER* >* pvTeamMemberList = m_pkTeamArchive->GetTeamList();

		if( pvTeamMemberList != NULL )
		{
			TEAM_MEMBER*	pTeamMember;

			vector< TEAM_MEMBER* >::iterator iter = pvTeamMemberList->begin();
			while( iter != pvTeamMemberList->end() )
			{
				pTeamMember	=	(TEAM_MEMBER*)(*iter);

				if( pTeamMember != NULL )
				{
#if !defined(_CHANNEL_TEAM_VERSION_)
					if( (UINT16)g_pNetworkManager->GetServerID() == pTeamMember->wServerID )
					{
						++iter;
						continue;
					}
#endif
					if( GetGUID() != pTeamMember->CharGUID )
						g_pInterfaceManager->SPChildWndSendMessage( WIID_MINIMAP , SPIM_REFRESH_ICON_TEAM , 2 , (LPARAM)&pTeamMember->CharGUID );
				}

				++iter;
			}
		}
		//

		wsprintf(szMsg, g_pResourceManager->GetGlobalString(4002006));
		ClearTeamMember();
	}
	else
	{
		// ´Ù¸¥ »ç¶÷ÀÌ Å»Åð¸¦ ÇßÀ»°æ¿ì [2/15/2006 AJJIYA]
		// ¸É¹ö ¹Ì´Ï¸Ê ¾ÆÀÌÄÜÀ» ¿ø·¡´ë·Î µ¹¸°´Ù.
		if( GetGUID() != CharGUID )
			g_pInterfaceManager->SPChildWndSendMessage( WIID_MINIMAP , SPIM_REFRESH_ICON_TEAM , 2 , (LPARAM)&CharGUID );
		//

#if !defined(_CHANNEL_TEAM_VERSION_)
		if( m_pkTeamArchive->GetCharName( iCharDBKey ) )
			wsprintf(szMsg, g_pResourceManager->GetGlobalString(4002005), m_pkTeamArchive->GetCharName( iCharDBKey ));

		DeleteTeamMember( iCharDBKey );
#else
		if( m_pkTeamArchive->GetCharName(CharGUID) )
			wsprintf(szMsg, g_pResourceManager->GetGlobalString(4002005), m_pkTeamArchive->GetCharName(CharGUID));

		DeleteTeamMember(CharGUID);
#endif

		if( m_pkTeamArchive->GetTeamCount() <= 1 )
			ClearTeamMember();
	}

	GU_ID iLocalID = GetGUID();
	g_pInterfaceManager->SPPerformMessage( SPIM_TEAM_SECEDE, (WPARAM)(m_pkTeamArchive->GetTeamList()), (LPARAM)&iLocalID);

	if( !m_pkTeamArchive->GetActive() )
	{
		g_pInterfaceManager->SPPerformMessage(SPIM_TEAM_ICON_UPDATE, 2, 0);
		g_pSubGameManager->SPPerformMessage(SPGM_INDUN_TEAM_EXIT, 0, 0);
	}

	g_pInterfaceManager->SPPerformMessage(SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMsg);

	return 1;
}

int SPPlayer::OnTeamExpelReceive(LPARAM lParam)
{
	CPacket* pkPacket = (CPacket*)lParam;

	GU_ID CharGUID	=	0;

#if !defined(_CHANNEL_TEAM_VERSION_)
	INT32 iCharDBKey;

	pkPacket->ExtractUINT32((UINT32*)&iCharDBKey);

	TEAM_MEMBER*	pTeamInfo	=	m_pkTeamArchive->GetCharInfo( iCharDBKey );

	if( pTeamInfo != NULL )
	{
		if( (UINT16)g_pNetworkManager->GetServerID() == pTeamInfo->wServerID )
			CharGUID	=	pTeamInfo->CharGUID;
	}
#else
	pkPacket->ExtractUINT64(&CharGUID);
#endif

	char szMsg[512];
	szMsg[0] = '\0';
	if( GetGUID() == CharGUID )
	{
		// ³»°¡ °­Åð¸¦ ´çÇßÀ» °æ¿ì [2/15/2006 AJJIYA]
		// ¸É¹ö ¹Ì´Ï¸Ê ¾ÆÀÌÄÜÀ» ¿ø·¡´ë·Î µ¹¸°´Ù.
		std::vector< TEAM_MEMBER* >* pvTeamMemberList = m_pkTeamArchive->GetTeamList();

		if( pvTeamMemberList != NULL )
		{
			TEAM_MEMBER*	pTeamMember;

			vector< TEAM_MEMBER* >::iterator iter = pvTeamMemberList->begin();
			while( iter != pvTeamMemberList->end() )
			{
				pTeamMember	=	(TEAM_MEMBER*)(*iter);

				if( pTeamMember != NULL )
				{
#if !defined(_CHANNEL_TEAM_VERSION_)
					if( (UINT16)g_pNetworkManager->GetServerID() == pTeamMember->wServerID )
					{
						++iter;
						continue;
					}
#endif

					if( GetGUID() != pTeamMember->CharGUID )
						g_pInterfaceManager->SPChildWndSendMessage( WIID_MINIMAP , SPIM_REFRESH_ICON_TEAM , 2 , (LPARAM)&pTeamMember->CharGUID );
				}

				++iter;
			}
		}
		//

		ClearTeamMember();
		wsprintf(szMsg, g_pResourceManager->GetGlobalString(4002006));
	}
	else
	{
		// ´Ù¸¥ »ç¶÷ÀÌ °­Åð ´çÇßÀ»°æ¿ì [2/15/2006 AJJIYA]
		// ¸É¹ö ¹Ì´Ï¸Ê ¾ÆÀÌÄÜÀ» ¿ø·¡´ë·Î µ¹¸°´Ù.
		if( GetGUID() != CharGUID )
			g_pInterfaceManager->SPChildWndSendMessage( WIID_MINIMAP , SPIM_REFRESH_ICON_TEAM , 2 , (LPARAM)&CharGUID );
		//

#if !defined(_CHANNEL_TEAM_VERSION_)
		if( m_pkTeamArchive->GetCharName( iCharDBKey ) )
			wsprintf(szMsg, g_pResourceManager->GetGlobalString(4002005), m_pkTeamArchive->GetCharName( iCharDBKey ));

		DeleteTeamMember( iCharDBKey );
#else
		if( m_pkTeamArchive->GetCharName(CharGUID) )
			wsprintf(szMsg, g_pResourceManager->GetGlobalString(4002005), m_pkTeamArchive->GetCharName(CharGUID));

		DeleteTeamMember(CharGUID);
#endif

		if( m_pkTeamArchive->GetTeamCount() <= 1 )
			ClearTeamMember();
	}

	GU_ID iLocalID = GetGUID();
	g_pInterfaceManager->SPPerformMessage( SPIM_TEAM_SECEDE, (WPARAM)(m_pkTeamArchive->GetTeamList()), (LPARAM)&iLocalID);

	if( !m_pkTeamArchive->GetActive() )
	{
		g_pInterfaceManager->SPPerformMessage(SPIM_TEAM_ICON_UPDATE, 2, 0);
		g_pSubGameManager->SPPerformMessage(SPGM_INDUN_TEAM_EXIT, 0, 0);
	}

	g_pInterfaceManager->SPPerformMessage(SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMsg);

	return 1;
}

int SPPlayer::OnTeamChangeMasterRequest(LPARAM lParam)
{
	const char* pszCharName = (const char*)lParam;

#if !defined(_CHANNEL_TEAM_VERSION_)
	INT32	iCharDBKey	=	m_pkTeamArchive->GetCharDBKey( pszCharName );

	if( iCharDBKey != 0 )
	{
		CPacket kPacket;
		kPacket.AddUINT32(TEAM_CS_CHANGECAPTAIN);
		kPacket.AddUINT32(iCharDBKey);
		g_pNetworkManager->GetPacketHandler()->PostMsg(&kPacket);
	}
#else
	GU_ID MasterID = m_pkTeamArchive->GetCharGUID(pszCharName);

	if( MasterID != 0 )
	{
		CPacket kPacket;
		kPacket.AddUINT32(TEAM_CS_CHANGECAPTAIN);
		kPacket.AddUINT64(MasterID);
		g_pNetworkManager->GetPacketHandler()->PostMsg(&kPacket);
	}
#endif

	return 1;
}

int SPPlayer::OnTeamChangeMasterReceive(LPARAM lParam)
{
	CPacket* pkPacket = (CPacket*)lParam;

	GU_ID CharGUID = 0;

#if !defined(_CHANNEL_TEAM_VERSION_)
	INT32 iCharDBKey;

	pkPacket->ExtractUINT32((UINT32*)&iCharDBKey);

	TEAM_MEMBER*	pTeamInfo	=	m_pkTeamArchive->GetCharInfo( iCharDBKey );

	if( pTeamInfo != NULL )
	{
		if( (UINT16)g_pNetworkManager->GetServerID() == pTeamInfo->wServerID )
			CharGUID	=	pTeamInfo->CharGUID;
	}

	m_pkTeamArchive->ChangeMaster( iCharDBKey );

#else
	pkPacket->ExtractUINT64(&CharGUID);

	m_pkTeamArchive->ChangeMaster(CharGUID);
#endif

	GU_ID iLocalID = GetGUID();
	int iPalletTeamIcon = 0;

	if( GetGUID() == CharGUID )
	{
		m_pkTeamArchive->SetMaster(true);
		iPalletTeamIcon = 1;
	}
	else
	{
		m_pkTeamArchive->SetMaster(false);
	}
	
	g_pInterfaceManager->SPPerformMessage(SPIM_TEAM_JOIN, (WPARAM)(m_pkTeamArchive->GetTeamList()), (LPARAM)&iLocalID);
	g_pInterfaceManager->SPPerformMessage(SPIM_TEAM_ICON_UPDATE, iPalletTeamIcon, 0);

	char szMsg[512];

#if !defined(_CHANNEL_TEAM_VERSION_)
	wsprintf(szMsg, g_pResourceManager->GetGlobalString(4002007), m_pkTeamArchive->GetCharName(iCharDBKey));
#else
	wsprintf(szMsg, g_pResourceManager->GetGlobalString(4002007), m_pkTeamArchive->GetCharName(CharGUID));
#endif
	g_pInterfaceManager->SPPerformMessage(SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMsg);

	return 1;
}

int SPPlayer::OnTeamChangeLootingRequest(LPARAM lParam)
{
	if( !m_pkTeamArchive->GetActive() )
		return 1;

	unsigned char ucLootingRule = lParam;

	CPacket kPacket;
	kPacket.AddUINT32(TEAM_CS_CHANGELOOTINGRULE);
	kPacket.AddUINT8(ucLootingRule);
	g_pNetworkManager->GetPacketHandler()->PostMsg(&kPacket);

	return 1;
}

int SPPlayer::OnTeamChangeLootingReceive(LPARAM lParam)
{
	CPacket* pkPacket = (CPacket*)lParam;

	UINT8 cLootingRule;
	pkPacket->ExtractUINT8(&cLootingRule);

	int iMaster = 0;
	if( m_pkTeamArchive->IsMaster() )
		iMaster = 1;

	g_pInterfaceManager->SPPerformMessage(SPIM_TEAM_UPDATE_LOOTING_RULE, cLootingRule, iMaster);
	
	char szMsg[512];
	switch( cLootingRule )
	{
	case 0:
		wsprintf(szMsg, g_pResourceManager->GetGlobalString(4002008));
		break;
	case 1:
		wsprintf(szMsg, g_pResourceManager->GetGlobalString(4002010));
		break;
	case 2:
		wsprintf(szMsg, g_pResourceManager->GetGlobalString(4002009));
		break;
	default:
		szMsg[0] = '\0';
		break;
	}

	g_pInterfaceManager->SPPerformMessage(SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMsg);
	return 1;
}

int SPPlayer::OnTeamUpdateMemberReceive(LPARAM lParam)
{
	CPacket* pkPacket = (CPacket*)lParam;

	TEAM_MEMBER MemberInfo;
	pkPacket->ExtractStruct(&MemberInfo, sizeof(TEAM_MEMBER));

	m_pkTeamArchive->UpdateTeamPlayer(MemberInfo);
	g_pInterfaceManager->SPPerformMessage(SPIM_TEAM_UPDATE, (WPARAM)&MemberInfo);

	return 1;
}

int SPPlayer::OnTeamChannelMove(LPARAM lParam)
{
	if( !m_bLocalPlayer )
		return 1;

	CPacket* pkPacket = (CPacket*)lParam;

	UINT8 byLootingRule;
	int	iCnt, i;
	TEAM_MEMBER	MemberInfo;

	pkPacket->ExtractUINT8(&byLootingRule);
	pkPacket->ExtractUINT32((UINT32*)&iCnt);

	ClearTeamMember();

	for( i = 0; i < iCnt; i++ )
	{
		pkPacket->ExtractStruct(&MemberInfo, sizeof(TEAM_MEMBER));
		InsertTeamMember(MemberInfo);

#if !defined(_CHANNEL_TEAM_VERSION_)
		if( (UINT16)g_pNetworkManager->GetServerID() != MemberInfo.wServerID )
			continue;
#endif

		if( MemberInfo.byGrade )
		{
			if( GetGUID() == MemberInfo.CharGUID )
				m_pkTeamArchive->SetMaster(true);
		}

		// Á¶ÀÎµÇ´Â ÆÀ¸â¹ö ¹Ì´Ï¸Ê ¾ÆÀÌÄÜ º¯°æ [2/15/2006 AJJIYA]
		// ÀÚ±â ÀÚ½ÅÀº º¯°æÇÏÁö ¾Ê´Â´Ù.
		if( GetGUID() != MemberInfo.CharGUID )
			g_pInterfaceManager->SPChildWndSendMessage( WIID_MINIMAP , SPIM_REFRESH_ICON_TEAM , 1 , (LPARAM)&MemberInfo.CharGUID );
	}

	if( iCnt )
	{
		g_pInterfaceManager->ShowWindow(WIID_MINI_TEAM, true);

		GU_ID iLocalID = GetGUID();
		g_pInterfaceManager->SPPerformMessage( SPIM_TEAM_JOIN, (WPARAM)(m_pkTeamArchive->GetTeamList()), (LPARAM)&iLocalID);

		int iPalletTeamIcon = 0;
		if( m_pkTeamArchive->IsMaster() )
			iPalletTeamIcon = 1;

		g_pInterfaceManager->SPPerformMessage(SPIM_TEAM_ICON_UPDATE, iPalletTeamIcon, 0);
		g_pInterfaceManager->SPPerformMessage(SPIM_TEAM_UPDATE_LOOTING_RULE, byLootingRule, iPalletTeamIcon);
	}

	return 1;

}

int SPPlayer::OnTitleChange( LPARAM lParam )
{
	SPTitleData*	pTitleData	=	(SPTitleData*)lParam;

	if( pTitleData != NULL )
	{
		SetTitle( *pTitleData );
	}

	return 1;
}

int SPPlayer::OnSetCoolTime( LPARAM lParam )
{
	CPacket* pPacket = (CPacket*)lParam;

	if( pPacket == NULL )
		return 1;

	UINT16			iNumOfCoolTime;
	COOLTIME_DATA	stCoolTime;

	pPacket->ExtractUINT16( &iNumOfCoolTime );

	for( UINT16 i = 0 ; i < iNumOfCoolTime ; ++i )
	{
		pPacket->ExtractStruct( &stCoolTime , sizeof( COOLTIME_DATA ) );

		if( stCoolTime.iCoolTimeID > 0 && stCoolTime.iApplyCoolTime > 0 )
		{
			ApplyCoolTime( stCoolTime.iCoolTimeID , (float)stCoolTime.iMaxCoolTime , (float)stCoolTime.iMaxCoolTime - (float)stCoolTime.iApplyCoolTime );
		}
	}

	if( m_bLocalPlayer == true && iNumOfCoolTime > 0 )
	{
		UpdatePetSpecialSkillToggle();
		g_pInterfaceManager->SPChildWndSendMessage( WIID_QUICK_SLOT , SPIM_REFRESH , NULL , NULL );
		g_pInterfaceManager->SPChildWndSendMessage( WIID_SKILL , SPIM_REFRESH , NULL , NULL );
		g_pInterfaceManager->SPChildWndSendMessage( WIID_PET , SPIM_REFRESH , NULL , NULL );
	}

	return 1;
}

int SPPlayer::OnItemEnchantReceive(LPARAM lParam)
{
	CPacket* pkPacket = (CPacket*)lParam;
	if( pkPacket == NULL )
		return 1;

	unsigned char ucEnchantBehavior;
	GLOBAL_STRING_ID iError;
	pkPacket->ExtractUINT8(&ucEnchantBehavior);
	pkPacket->ExtractUINT32(&iError);
	
	if( g_pInterfaceManager )
		g_pInterfaceManager->SPChildWndSendMessage(WIID_ENCHANT, SPIM_ITEM_ENCHANT_RESULT, iError, ucEnchantBehavior, true);

	return 1;
}

// ¾ÆÀÌÅÛ ¾÷±×·¹ÀÌµå AJJIYA [5/23/2006]
int SPPlayer::OnItemUpgradeReceive(LPARAM lParam)
{
	CPacket* pkPacket = (CPacket*)lParam;
	if( pkPacket == NULL )
		return 1;

	unsigned char ucItemUpgradeBehavior;
	GLOBAL_STRING_ID iError;
	pkPacket->ExtractUINT8(&ucItemUpgradeBehavior);
	pkPacket->ExtractUINT32(&iError);

	if( g_pInterfaceManager == NULL || g_pItemCluster == NULL )
		return 1;

	INSTANCE_ID	eSendMessageID	=	0;

	if( g_pItemCluster->GetItemUpgradeState() == SPIU_STATE_PET_LEVELUP )
	{
		eSendMessageID	=	WIID_PET_LEVELUP;
	}
	else if( g_pItemCluster->GetItemUpgradeState() == SPIU_STATE_ITEM_UPGRADE )
	{
		eSendMessageID	=	WIID_ITEM_UPGRADE;
	}

	g_pInterfaceManager->SPChildWndSendMessage( eSendMessageID , SPIM_ITEM_UPGRADE_RESULT , iError, ucItemUpgradeBehavior, true);
	return 1;
}

int SPPlayer::OnItemEffectUpgradeReceive(LPARAM lParam)
{
	CPacket* pkPacket = (CPacket*)lParam;
	if( pkPacket == NULL )
		return 1;

	unsigned char ucItemUpgradeBehavior;
	GLOBAL_STRING_ID iError;
	pkPacket->ExtractUINT8(&ucItemUpgradeBehavior);
	pkPacket->ExtractUINT32(&iError);

	if( g_pInterfaceManager == NULL || g_pItemCluster == NULL )
		return 1;

	g_pInterfaceManager->SPChildWndSendMessage( WIID_EFFECTITEM_UPGRADE , SPIM_ITEM_UPGRADE_RESULT , iError, ucItemUpgradeBehavior, true);
}

//[2006/5/22] °³ÀÎ»óÁ¡
bool SPPlayer::SendUserShopIn(GU_ID iGuid)
{
	//[2006/7/13] ·ÎÄÃ ÇÃ·¹ÀÌ¾î »óÅÂ Ã¼Å©
	if(m_bLocalPlayer) {
		if(GetCurAction() & ACTION_USERSHOP) {	//³»°¡ °³¼³ÀÚ
			return false;
		}

		if(m_uiUserShopPlayer != NULL) {		//ÀÌ¹Ì °³¼³µÈ °³ÀÎ»óÁ¡¿¡ µé¾î°£ °æ¿ì
			return false;
		}

		if(g_pEventManager->GetCutInState() != CUT_IN_STATE_NULL) { //³»°¡ ÄÆÀÎÁßÀÎ°æ¿ì
			return false;
		}

		if(m_iTrade == TRADE_STATE_TRADE) {			//³»°¡ °Å·¡ÁßÀÎ °æ¿ì
			return false;
		}
		else if(m_iTrade == TRADE_STATE_RESPONSE) {	//³»°¡ °Å·¡ ¿äÃ»À» ¹ÞÀº °æ¿ì
			g_pInterfaceManager->ForceReply();		//¿äÃ» °ÅºÎ
			m_iTrade = TRADE_STATE_NULL;
		}		
	}
	else {
		return false;
	}		

	if(g_pGOBManager->Find(iGuid)) {
		if(IsLocalPlayer()) {
			if(GetGUID() == iGuid)			//³ª¿Í »ó´ë¹æÀÌ µ¿ÀÏÇÑ GUIDÀÎ °æ¿ì
				return false;
		}

		if(CheckPcInterval(iGuid) == false) {		//»ó´ë¿Í ³ªÀÇ °Å¸®°¡ Æ®·¹ÀÌµå°¡ ºÒ°¡´ÉÇÏ°Ô µÈ °æ¿ì
			if(g_pResourceManager->GetGlobalString(12000015)) {
				std::string strMsg = g_pResourceManager->GetGlobalString(12000015);
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
			}
			return false;
		}

		m_uiUserShopPlayer = iGuid;
		
		CPacket Packet;
		Packet.Add((UINT32)FREEMARKET_CS_IN);		
		Packet.AddUINT64(m_uiUserShopPlayer);
		g_pNetworkManager->SPPerformMessage(FREEMARKET_CS_IN, 0, (LPARAM)&Packet);

		DXUTOutputDebugString("\t * Send FREEMARKET_CS_IN\n");

		return true;
	}

	return false;
}

bool SPPlayer::SendPetShopIn(GU_ID iGuid)
{
	//CPacket packet;
	//packet.Add((UINT32)PET_CS_ENTER_PET_SHOP);
	//packet.AddUINT64(iGuid);
	//g_pNetworkManager->SPPerformMessage(PET_CS_ENTER_PET_SHOP, 0, (LPARAM)&packet );
	
	CPacket packet_get_info;
	packet_get_info.Add((UINT32)PET_CS_GET_PET_SHOP_INFO);
	packet_get_info.AddUINT64(iGuid);
	g_pNetworkManager->SPPerformMessage(PET_CS_GET_PET_SHOP_INFO, 0, (LPARAM)&packet_get_info );
	m_uiPetShopPlayer = iGuid;
	return true;
}

bool SPPlayer::SendUserShopOut()
{
	if(m_bLocalPlayer) {
		//GU_ID uiSellUser = g_pGOBManager->GetLocalPlayer()->GetUserShopID();
		if(m_uiUserShopPlayer == 0)
			return false;

		DXUTOutputDebugString("\t * FREEMARKET_CS_OUT\n");

		CPacket Packet;
		Packet.Add((UINT32)FREEMARKET_CS_OUT);
		Packet.AddUINT64(m_uiUserShopPlayer);
		g_pNetworkManager->SPPerformMessage(FREEMARKET_CS_OUT, 0, (LPARAM)&Packet);

		m_uiUserShopPlayer = 0;			//[2006/5/25] °Å·¡Ã¼Å©¶§¹®¿¡ Ãß°¡(ÀÓ½Ã)
		OnUserShopOut(0);		
	}	
	return false;
}

bool SPPlayer::SendPetShopOut( void )
{
	CPacket packet;
	packet.Add((UINT32)PET_CS_LEAVE_PET_SHOP);
	packet.AddUINT64(m_uiPetShopPlayer);
	g_pNetworkManager->SPPerformMessage(PET_CS_LEAVE_PET_SHOP, 0, (LPARAM)&packet);

	return true;
}

GU_ID SPPlayer::GetUserShopID()
{
	return m_uiUserShopPlayer;
}

GU_ID SPPlayer::GetPetShopID( void )
{
	return m_uiPetShopPlayer;
}

void SPPlayer::SetUserShopName(std::string strShopName)
{
	m_pPlayerStatusModel->SetUserShopName(strShopName);
}

void SPPlayer::SetPetShopName(std::string strShopName)
{
	return;
}

std::string SPPlayer::GetUserShopName()
{
	return m_pPlayerStatusModel->GetUserShopName();
}

std::string SPPlayer::GetPetShopName( void )
{
	return std::string("");
}

void SPPlayer::SetUserShopType(int iType)
{
	m_iUserShopType = iType;
}

void SPPlayer::SetPetShopType(int iType)
{
	m_iPetShopType = iType;
}

int SPPlayer::GetUserShopType()
{
	return m_iUserShopType;
}
int SPPlayer::GetPetShopType( void )
{
	return m_iPetShopType;
}


void SPPlayer::SetUserShopOut()
{
	//ÀÌ °á°ú´Â ³»°¡ ³²ÀÇ °³ÀÎ»óÁ¡¿¡¼­ ³ª°¡´Â °æ¿ì¸¸ ÀÖÀ½
	m_uiUserShopPlayer = 0;
	SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_USERSHOP_BUY);
	if(pWindow) {
		//pWindow->Show();
		pWindow->SPSendMessage(SPIM_USERSHOP_BUY_OUT, (WPARAM)0, (LPARAM)0);
		g_pInterfaceManager->GetStatPalletWindow()->SPSendMessage(SPIM_STAT_BUTTON_ENABLE, (WPARAM)true, (LPARAM)false);

		//[2006/7/12] - ÇÏ´Ü ¸Þ´º È°¼º
		pWindow = g_pInterfaceManager->FindWindow(WIID_MENU);
		if(pWindow) {
			pWindow->SetEnable(true);
		}
	}
}

void SPPlayer::SetPetShopOut( void )
{
	m_uiPetShopPlayer = 0;
	SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_USERSHOP_BUY);
	if(pWindow) {
		pWindow->SPSendMessage(SPIM_USERSHOP_BUY_OUT, (WPARAM)0, (LPARAM)0);
		g_pInterfaceManager->GetStatPalletWindow()->SPSendMessage(SPIM_STAT_BUTTON_ENABLE, (WPARAM)true, (LPARAM)false);

		pWindow = g_pInterfaceManager->FindWindow(WIID_MENU);
		if(pWindow) {
			pWindow->SetEnable(true);
		}
	}
}


void SPPlayer::RefreshEquipItem()
{
	if( m_bLocalPlayer )
		return;

	int i;
	FIGURE_ITEM FigureItem;

	for( i = 0; i < EQ_END; i++ )
	{
		if( m_nBattleEquipInfo[i] )
		{
			FigureItem.iItemID = m_nBattleEquipInfo[i];
			FigureItem.SlotInfo.ContainerType = FIGURE_EQUIP;
			FigureItem.SlotInfo.SlotIndex = i + 1;

			ItemSetting(FigureItem);
		}
	}

	for( i = 0; i < EQ_END; i++ )
	{
		if( m_nFashionEquipInfo[i] )
		{
			FigureItem.iItemID = m_nFashionEquipInfo[i];
			FigureItem.SlotInfo.ContainerType = FIGURE_FASHION;
			FigureItem.SlotInfo.SlotIndex = i + 1;

			ItemSetting(FigureItem);
		}
	}
}


int SPPlayer::OnUserShopOpen(LPARAM lParam)
{
	CPacket* packet = (CPacket*)lParam;
	if( packet == NULL )
		return 1;

	GU_ID				ObjectGUID;					// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
	GLOBAL_STRING_ID	ErrorID;					// ¿¡·¯¾ÆÀÌµð(0ÀÌ¸é ¿¡·¯¾øÀ½)
	UINT8				byType = 0;					// »óÁ¡Å¸ÀÔ
	char				szFMName[FM_MAX_NAME+1];	// »óÁ¡¸í

	packet->ExtractUINT64(&ObjectGUID);
	packet->ExtractUINT32(&ErrorID);

	bool bOpen = false;

	if(ErrorID) {
		if(g_pResourceManager->GetGlobalString(ErrorID)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(ErrorID);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}		
		//return 1;
	}
	else {
		packet->ExtractUINT8(&byType);
		packet->ExtractData(szFMName, FM_MAX_NAME+1);
		bOpen = true;
		SetUserShopName(szFMName);
	}	
	
	SPWindow* pWindow = NULL;
	if(GetGUID() == ObjectGUID) {
		if(IsLocalPlayer()) {								//³»°¡ °³¼³ÀÚ			
			g_pInterfaceManager->GetStatPalletWindow()->SPSendMessage(SPIM_STAT_BUTTON_ENABLE, (WPARAM)false, (LPARAM)false);
			
			//±âÅ¸ ÀÎÅÍÆäÀÌ½º ´ÝÀ½
			g_pInterfaceManager->ShowWindow(WIID_ITEM		, 	SP_HIDE);	
			g_pInterfaceManager->ShowWindow(WIID_EQUIP		, 	SP_HIDE);
			g_pInterfaceManager->ShowWindow(WIID_SKILL		, 	SP_HIDE);
			g_pInterfaceManager->ShowWindow(WIID_STATUS		, 	SP_HIDE);
			g_pInterfaceManager->ShowWindow(WIID_QUEST		, 	SP_HIDE);
			g_pInterfaceManager->ShowWindow(WIID_TEAM		, 	SP_HIDE);
			//g_pInterfaceManager->ShowWindow(WIID_MENU		, 	SP_HIDE);
			g_pInterfaceManager->ShowWindow(WIID_PC_INFO	, 	SP_HIDE);
			g_pInterfaceManager->ShowWindow(WIID_CHANNEL_CHANGE		, 	SP_HIDE);
			g_pInterfaceManager->ShowWindow(WIID_FRIEND		, 	SP_HIDE);
			g_pInterfaceManager->ShowWindow(WIID_OPTION		,	SP_HIDE);
			g_pInterfaceManager->ShowWindow(WIID_HELP		,	SP_HIDE);
			g_pInterfaceManager->ShowWindow(WIID_WORLDMAP	,	SP_HIDE);
			g_pInterfaceManager->ShowWindow(WIID_WAYPOINT	,	SP_HIDE);
			g_pInterfaceManager->ShowWindow(WIID_MICROPHONE	,	SP_HIDE);
			g_pInterfaceManager->ShowWindow( WIID_FRIEND_FIND,	SP_HIDE ); //[liuyang, 2008.8.29]
			g_pInterfaceManager->ShowWindow( WIID_FRIEND_FOLLOW,	SP_HIDE );//[liuyang, 2008.10.10]
			g_pInterfaceManager->ShowWindow( WIID_FRIEND_SNATCH,	SP_HIDE );//[liuyang, 2008.10.10]
			g_pInterfaceManager->ShowWindow(WIID_EXIT		,	SP_HIDE);
			
			pWindow = g_pInterfaceManager->FindWindow(WIID_USERSHOP_SELL);
			if(pWindow) {
				pWindow->SPSendMessage(SPIM_USERSHOP_SELL_OPEN, (WPARAM)bOpen, (LPARAM)0);
			}
			
			//[2006/7/12] - ÇÏ´Ü ¸Þ´º ºñÈ°¼º
			pWindow = g_pInterfaceManager->FindWindow(WIID_MENU);
			if(pWindow) {
				pWindow->SPSendMessage(SPIM_MENU_POPUP, WPARAM(0), LPARAM(0));
				pWindow->SetEnable(false);
			}
		}	
		else {												//Å¸ÀÎÀÌ °³¼³ÀÚ			
		}
		SetUserShopType(byType);
	}
	
	return 1;
}


int SPPlayer::OnUserShopClose(LPARAM lParam)
{
	CPacket* packet = (CPacket*)lParam;
	if( packet == NULL )
		return 1;

	GU_ID				ObjectGUID;					// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
	GLOBAL_STRING_ID	ErrorID;					// ¿¡·¯¾ÆÀÌµð(0ÀÌ¸é ¿¡·¯¾øÀ½)
	
	packet->ExtractUINT64(&ObjectGUID);		
	packet->ExtractUINT32(&ErrorID);
	
	bool bClose = false;
	if(ErrorID) {
		if(g_pResourceManager->GetGlobalString(ErrorID)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(ErrorID);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}		
		//return 1;		
	}
	else {
		bClose = true;
	}	

	SPWindow* pWindow = NULL;
	if(GetGUID() == ObjectGUID) {
		if(IsLocalPlayer()) {						//³»°¡ °³¼³ÀÚ
			pWindow = g_pInterfaceManager->FindWindow(WIID_USERSHOP_SELL);
			if(pWindow) {
				pWindow->SPSendMessage(SPIM_USERSHOP_SELL_CLOSE, (WPARAM)bClose, (LPARAM)0);
			}

			//[2006/7/12] - ÇÏ´Ü ¸Þ´º È°¼º
			pWindow = g_pInterfaceManager->FindWindow(WIID_MENU);
			if(pWindow) {
				pWindow->SetEnable(true);
			}
		}
		else {										//´Ù¸¥ÀÌ°¡ °³¼³ÀÚ			
			if(g_pGOBManager->GetLocalPlayer() && g_pGOBManager->GetLocalPlayer()->GetUserShopID()) {
				if(g_pGOBManager->GetLocalPlayer()->GetUserShopID() == ObjectGUID)
					g_pGOBManager->GetLocalPlayer()->SendUserShopOut();
				//g_pGOBManager->GetLocalPlayer()->OnUserShopOut(0);
			}
			//m_uiUserShopPlayer = 0;
			//pWindow = g_pInterfaceManager->FindWindow(WIID_USERSHOP_BUY);
			//if(pWindow) {
			//	pWindow->SPSendMessage(SPIM_USERSHOP_BUY_OUT, (WPARAM)0, (LPARAM)0);
			//}			
		}
		SetUserShopType(0);
		g_pInterfaceManager->GetStatPalletWindow()->SPSendMessage(SPIM_STAT_BUTTON_ENABLE, (WPARAM)true, (LPARAM)false);
	}
	else {
	}
	
	return 1;
}


int SPPlayer::OnUserShopIn(LPARAM lParam)
{
	CPacket* packet = (CPacket*)lParam;
	if( packet == NULL )
		return 1;

	GLOBAL_STRING_ID	ErrorID;					// ¿¡·¯¾ÆÀÌµð(0ÀÌ¸é ¿¡·¯¾øÀ½)	

	//
	packet->ExtractUINT32(&ErrorID);

	if(ErrorID) {
		if(g_pResourceManager->GetGlobalString(ErrorID)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(ErrorID);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}
		
		m_uiUserShopPlayer = 0;

		return 1;
	}

	//ÀÌ °á°ú´Â ³»°¡ ³²ÀÇ °³ÀÎ»óÁ¡¿¡ µé¾î°¡´Â °æ¿ì¸¸ ÀÖÀ½
	SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_USERSHOP_BUY);
	if(pWindow) {
		SPPlayer* pShopOpener = (SPPlayer*)g_pGOBManager->Find(m_uiUserShopPlayer);
		if(pShopOpener) {
			g_pInterfaceManager->GetStatPalletWindow()->SPSendMessage(SPIM_STAT_BUTTON_ENABLE, (WPARAM)false, (LPARAM)false);			

			//±âÅ¸ ÀÎÅÍÆäÀÌ½º ´ÝÀ½
			g_pInterfaceManager->ShowWindow(WIID_ITEM		, 	SP_HIDE);	
			g_pInterfaceManager->ShowWindow(WIID_EQUIP		, 	SP_HIDE);
			g_pInterfaceManager->ShowWindow(WIID_SKILL		, 	SP_HIDE);
			g_pInterfaceManager->ShowWindow(WIID_STATUS		, 	SP_HIDE);
			g_pInterfaceManager->ShowWindow(WIID_QUEST		, 	SP_HIDE);
			g_pInterfaceManager->ShowWindow(WIID_TEAM		, 	SP_HIDE);
			//g_pInterfaceManager->ShowWindow(WIID_MENU		, 	SP_HIDE);
			g_pInterfaceManager->ShowWindow(WIID_PC_INFO	, 	SP_HIDE);
			g_pInterfaceManager->ShowWindow(WIID_CHANNEL_CHANGE		, 	SP_HIDE);
			g_pInterfaceManager->ShowWindow(WIID_FRIEND		, 	SP_HIDE);
			g_pInterfaceManager->ShowWindow(WIID_OPTION		,	SP_HIDE);
			g_pInterfaceManager->ShowWindow(WIID_HELP		,	SP_HIDE);
			g_pInterfaceManager->ShowWindow(WIID_WORLDMAP	,	SP_HIDE);
			g_pInterfaceManager->ShowWindow(WIID_WAYPOINT	,	SP_HIDE);
			g_pInterfaceManager->ShowWindow(WIID_MICROPHONE	,	SP_HIDE);
			g_pInterfaceManager->ShowWindow( WIID_FRIEND_FIND, SP_HIDE );    //[liuyang, 2008.8.29]
			g_pInterfaceManager->ShowWindow( WIID_FRIEND_FOLLOW,	SP_HIDE );//[liuyang, 2008.10.10]
			g_pInterfaceManager->ShowWindow( WIID_FRIEND_SNATCH,	SP_HIDE );//[liuyang, 2008.10.10]

			pWindow->SPSendMessage(SPIM_USERSHOP_BUY_IN, (WPARAM)packet, (LPARAM)pShopOpener->GetUserShopName().c_str());
			SPWindowUserShopBuyer* pBuyer = (SPWindowUserShopBuyer*)pWindow;
			pBuyer->SetPetShop( false );


			//[2006/7/12] - ÇÏ´Ü ¸Þ´º ºñÈ°¼º
			pWindow = g_pInterfaceManager->FindWindow(WIID_MENU);
			if(pWindow) {
				pWindow->SPSendMessage(SPIM_MENU_POPUP, WPARAM(0), LPARAM(0));
				pWindow->SetEnable(false);
			}
		}		
	}

	return 1;
}


int SPPlayer::OnUserShopOut(LPARAM lParam)
{	
	SetUserShopOut();
	return 1;
}


int SPPlayer::OnUserShopBuy(LPARAM lParam)
{
	CPacket* packet = (CPacket*)lParam;
	if( packet == NULL )
		return 1;

	GU_ID				ObjectGUID;					// °ÔÀÓ¿ÀºêÁ§Æ®¾ÆÀÌµð
	GLOBAL_STRING_ID	ErrorID;					// ¿¡·¯¾ÆÀÌµð(0ÀÌ¸é ¿¡·¯¾øÀ½)
	FM_ITEM				stFM_Item;					// ÆÇ¸ÅµÈ ¾ÆÀÌÅÛ Á¤º¸

	packet->ExtractUINT64(&ObjectGUID);		
	packet->ExtractUINT32(&ErrorID);

	if(ErrorID) {
		if(g_pResourceManager->GetGlobalString(ErrorID)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(ErrorID);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}		
		return 1;
	}

	packet->ExtractStruct(&stFM_Item, sizeof(FM_ITEM));

	stFM_Item.bySlot--;	//0º£ÀÌ½º º¸Á¤;;;

	if(GetGUID() == ObjectGUID) {					//±¸¸Å ´ç»çÀÚ
		if(IsLocalPlayer()) {
			SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_USERSHOP_BUY);
			if(pWindow) {
				//pWindow->Show();
				SPPlayer* pPlayer = (SPPlayer*)g_pGOBManager->Find(m_uiUserShopPlayer);
				char szName[32];
				ZeroMemory(szName, 32);
				if(pPlayer){
					pPlayer->OnGetGOBName((LPARAM)szName);
				}
				pWindow->SPSendMessage(SPIM_USERSHOP_BUY_BUY, (WPARAM)stFM_Item.bySlot, (LPARAM)szName);
			}
		}
		else {
			if(g_pGOBManager->GetLocalPlayer()) {
				if(g_pGOBManager->GetLocalPlayer()->GetCurAction() & ACTION_USERSHOP) {	//°³¼³ÀÚ
					SPPlayer* pPlayer = (SPPlayer*)g_pGOBManager->Find(ObjectGUID);
					char szName[32];
					ZeroMemory(szName, 32);
					if(pPlayer){
						pPlayer->OnGetGOBName((LPARAM)szName);
					}

					SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_USERSHOP_SELL);
					if(pWindow) {
						pWindow->SPSendMessage(SPIM_USERSHOP_SELL_BUY, (WPARAM)stFM_Item.bySlot, (LPARAM)szName);
					}
				}
				else {									//»óÁ¡¿¡ µé¾î°¡ÀÖ´ø ´Ù¸¥ À¯Àú°¡ ¹°°ÇÀ» ±¸ÀÔÇÑ°æ¿ì
					if(g_pGOBManager->GetLocalPlayer()->GetGUID()  != ObjectGUID) {
						SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_USERSHOP_BUY);
						if(pWindow) {
							//pWindow->Show();
							pWindow->SPSendMessage(SPIM_USERSHOP_BUY_BUY, (WPARAM)stFM_Item.bySlot, (LPARAM)0);
						}
					}					
				}
			}			
		}
	}
	else {											//±¸¸ÅÀÚ ÀÌ¿ÜÀÇ À¯Àú
		//if(IsLocalPlayer()) {			
		//}
	}
	
	return 1;
}

int SPPlayer::OnPetShopOpen(LPARAM lParam)
{
	CPacket	*packet = (CPacket*)lParam;
	if (!packet)
		return 1;

	GLOBAL_STRING_ID	errorID;
	packet->ExtractUINT32( &errorID );
	if (errorID)
	{
		if(g_pResourceManager->GetGlobalString(errorID)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(errorID);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}
		SPWindow *pWindow = g_pInterfaceManager->FindWindow( WIID_PETSHOP_SELL );
		if (pWindow)
		{
			pWindow->SPSendMessage( SPIM_PETSHOP_SELL_OPEN, (WPARAM)false, (LPARAM)0);
		}
		return 1;
	}

	SPWindow *pWindow = g_pInterfaceManager->FindWindow( WIID_PETSHOP_SELL );
	if (pWindow)
	{
		pWindow->SPSendMessage( SPIM_PETSHOP_SELL_OPEN, (WPARAM)true, (LPARAM)0);
	}

	return 0;
}
int SPPlayer::OnPetShopClose(LPARAM lParam)
{
	CPacket	*packet = (CPacket*)lParam;
	if (!packet)
		return 1;

	GLOBAL_STRING_ID	errorID;
	packet->ExtractUINT32( &errorID );
	if (errorID)
	{
		if(g_pResourceManager->GetGlobalString(errorID)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(errorID);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}
		return 1;
	}
	return 1;
}
int SPPlayer::OnPetShopIn(LPARAM lParam)
{
	CPacket* packet = (CPacket*)lParam;
	if( packet == NULL )
		return 1;

	GLOBAL_STRING_ID	ErrorID;					// ¿¡·¯¾ÆÀÌµð(0ÀÌ¸é ¿¡·¯¾øÀ½)	
	char shopname[LEN_NAME+1] ={0,};

	packet->ExtractUINT32(&ErrorID);
	if(ErrorID) {
		if(g_pResourceManager->GetGlobalString(ErrorID)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(ErrorID);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}

		m_uiPetShopPlayer = 0;

		return 1;
	}

	packet->ExtractData( (TCHAR*)&shopname, LEN_NAME );
	SPWindow *pWnd = g_pInterfaceManager->FindWindow( WIID_USERSHOP_BUY );
	if (pWnd)
	{
		pWnd->SPSendMessage( SPIM_PETSHOP_BUY_IN, (WPARAM)packet, (LPARAM)shopname );
		SPWindowUserShopBuyer* pBuyer = (SPWindowUserShopBuyer*)pWnd;
		pBuyer->SetPetShop( true );
		pBuyer->SetShopGUID( m_uiPetShopPlayer );
	}

	return 1;
}
int SPPlayer::OnPetShopOut(LPARAM lParam)
{
	SetPetShopOut();
	return 1;
}
int SPPlayer::OnPetShopBuy(LPARAM lParam)
{
	CPacket	*packet = (CPacket*)lParam;
	if (!packet)
		return 1;

	GLOBAL_STRING_ID	errorID;
	packet->ExtractUINT32( &errorID );
	if (errorID)
	{
		if(g_pResourceManager->GetGlobalString(errorID)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(errorID);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}
		return 1;
	}
	return 1;
}
int SPPlayer::OnPetShopItemUpdate(LPARAM lParam)
{
	CPacket *packet = (CPacket*)lParam;
	if (!packet)
		return 1;

	GU_ID shopguid = 0;
	UINT8 slotIndex = 0U;
	packet->ExtractUINT64( &shopguid );
	packet->ExtractUINT8( &slotIndex );
	slotIndex--;

	if (m_uiPetShopPlayer == shopguid)
	{
		SPWindow *pWnd = g_pInterfaceManager->FindWindow( WIID_USERSHOP_BUY );
		if (pWnd && pWnd->IsShow())
		{
			pWnd->SPSendMessage( SPIM_USERSHOP_BUY_BUY, (WPARAM)(int)slotIndex, (LPARAM)NULL );
		}
	}

	return 0;
}

int SPPlayer::OnSetPet(LPARAM lParam)
{
	if( lParam < 0 )
		ReleasePet();
	else
	{
		SPItemAttr* pItemAttr = NULL;
		g_pItemCluster->GetItemInfo(lParam, pItemAttr);
		if( pItemAttr == NULL )
			return 1;

		if( pItemAttr->m_iPetClassID )
		{
			SPPet *pet = SetPet(pItemAttr->m_iPetClassID);
			if( pet )
			{
				D3DXCOLOR color(0.5f, 1.0f, 0.5f, 1.0f);
				pet->SetName(pItemAttr->m_strName.c_str(), &color);
			}
		}
	}

	return 1;
}

int SPPlayer::OnPetSkillInsert(LPARAM lParam)
{
	PET_SKILL_INFO *petSkillInfo = (PET_SKILL_INFO*)lParam;
	if (!petSkillInfo)
		return 0;

	std::vector< t_Pet_Skill_To_Client > &vecSkills = petSkillInfo->vecSkills;
	SPItemAttr *pItemAttr = NULL;
	UINT32 itemID = 0;
	SPItem	*pItem = NULL;
	if (petSkillInfo->pItem && petSkillInfo->pItem->GetItemStatus())
	{
		itemID = petSkillInfo->pItem->GetItemStatus()->GetItemID();
		pItem = petSkillInfo->pItem;
	}
	else
		return 0;

	if (!g_pItemCluster->GetItemInfo( itemID, pItemAttr ))
		return 0;

	int i;
	SPSkill* pkSkill;
	SPItemSkillActivity* pkItemSkillActivity;
	SKILL_SPECIAL_TYPE eSpcialType;
	for (int idx = 0; idx < (int)vecSkills.size(); ++idx)
	{
		if (vecSkills[idx].nSkillID == 0)
			continue;

		pkSkill = SPSkillManager::GetInstance()->GetSkill( vecSkills[idx].nSkillID );
		if (pkSkill == NULL)
			continue;

		pkItemSkillActivity = m_pkSkillArchive->GetItemSkillActivity( vecSkills[idx].nSkillID,
			(int)pItem->GetContainerType(), (int)pItem->GetSlotIndex() );

		if (pkItemSkillActivity != NULL)
			continue;

		pkItemSkillActivity = new SPItemSkillActivity( pkSkill, vecSkills[idx].nLevel, 0,
			itemID, (int)pItem->GetContainerType(), (int)pItem->GetSlotIndex());

		if (pkSkill->GetType() == ST_TOGGLE)
		{
			if( m_pkEffectArchive->IsExistEffectActivity(pkSkill->GetToggleEffectID()) )
				pkItemSkillActivity->SetToggle(true);
			else
				pkItemSkillActivity->SetToggle(false);
		}

		m_pkSkillArchive->AddSkill(pkItemSkillActivity);

		eSpcialType = pkSkill->GetSpecialType();
		if( eSpcialType != SST_NULL )
		{
			SPPet *pet = m_pPetMgr->GetPet( (PET_PLACE)pItem->GetSlotIndex() );
			if( pet )
			{
				pet->SetSpecialSkill(eSpcialType, vecSkills[idx].nSkillID, true, false);
				//pet->SetSpecialSkill(eSpcialType, vecSkills[idx].nSkillID, pkItemSkillActivity->GetToggleOn(), true);
				pet->SetSkillStatus( eSpcialType, false );
			}
		}

		g_pInterfaceManager->SPChildWndSendMessage( WIID_QUICK_SLOT , SPIM_QUICK_SLOT_UPDATE , -1 , vecSkills[idx].nSkillID );
	}

	g_pInterfaceManager->SPChildWndSendMessage( WIID_SKILL , SPIM_SKILL_UPDATE , -1 , -1 );	// AJJIYA [10/25/2005]
	g_pInterfaceManager->SPChildWndSendMessage( WIID_PET , SPIM_REFRESH , NULL , NULL );
	return 1;
}

int SPPlayer::OnPetSkillDelete(LPARAM lParam)
{
	PET_SKILL_INFO *petSkillInfo = (PET_SKILL_INFO*)lParam;
	if (!petSkillInfo)
		return 0;

	std::vector< t_Pet_Skill_To_Client > &vecSkills = petSkillInfo->vecSkills;
	SPItemAttr *pItemAttr = NULL;
	UINT32 itemID = 0;
	SPItem	*pItem = NULL;
	if (petSkillInfo->pItem && petSkillInfo->pItem->GetItemStatus())
	{
		itemID = petSkillInfo->pItem->GetItemStatus()->GetItemID();
		pItem = petSkillInfo->pItem;
	}
	else
		return 0;

	if (!g_pItemCluster->GetItemInfo( itemID, pItemAttr ))
		return 0;

	for (int idx = 0; idx < (int)vecSkills.size(); ++idx)
	{
		if (vecSkills[idx].nSkillID == 0)
			continue;

		m_pkSkillArchive->RemoveSkill( vecSkills[idx].nSkillID, itemID,
			(int)pItem->GetContainerType(), (int)pItem->GetSlotIndex());
		g_pInterfaceManager->SPChildWndSendMessage( WIID_QUICK_SLOT, SPIM_QUICK_SLOT_UPDATE, -1, vecSkills[idx].nSkillID );
	}

	g_pInterfaceManager->SPChildWndSendMessage( WIID_QUICK_SLOT, SPIM_SKILL_UPDATE, -1, -1 );
	g_pInterfaceManager->SPChildWndSendMessage( WIID_PET , SPIM_REFRESH , NULL , NULL );
	return 1;
}

int SPPlayer::SetPetInfo( LPARAM lParam )
{
	CPacket	*packet = (CPacket*)lParam;
	if (!packet)
		return 0;

	//  PET_SC_GET_INFO
	//--------------------------------------------------
	//	UINT8 slotIndex;			//	³èÎïÔÚ³èÎï×°±¸À¸ÖÐµÄÎ»ÖÃ
	//	ITEMNO itemNo;				//	³èÎïµÄÎ¨Ò»±àºÅ
	//	t_PetAttr_Info Info;		//	³èÎïµÄ»ù±¾ÊôÐÔ
	//	int	   nSkillCount;			//	³èÎï¼¼ÄÜ×ÜÊý
	//	t_Pet_Skill_To_Client petSkill[nSkillCount];	//	³èÎï¼¼ÄÜ

	UINT8	slotIndex;
	ITEMNO	itemNo;
	t_PetAttr_Info info;
	packet->ExtractUINT8( &slotIndex );
	packet->ExtractUINT64( (UINT64*)&itemNo );
	packet->ExtractStruct( &info, sizeof(t_PetAttr_Info) );

	UINT32 nSkillCount = 0;
	std::vector< t_Pet_Skill_To_Client > vecPetSkill;
	packet->ExtractUINT32( &nSkillCount );
	if (nSkillCount > 0)
	{
		for (int idx = 0; idx < nSkillCount; ++idx)
		{
			t_Pet_Skill_To_Client petskill;
			packet->ExtractStruct( &petskill, sizeof(t_Pet_Skill_To_Client) );
			vecPetSkill.push_back( petskill );
		}
	}

	if (slotIndex <= 0 || slotIndex > 3)
		return 0;

	PET_PLACE iPlace = (PET_PLACE)--slotIndex;
	SPPet *pet = m_pPetMgr->GetPet( (PET_PLACE)iPlace );
	if (pet)
	{
		//--------------------------------------------------
		bool bOldPetGround = pet->GetClassID() < (CLASS_ID_PET + 100000);
		int iNewPetClass = SPPetLevelMgr::getInstance().GetRealPetClass( pet->GetItemID(), info.lv );
		if (iNewPetClass >= CLASS_ID_PET)
		{
			bool bNewPetGround = iNewPetClass < (CLASS_ID_PET + 100000);
			if (bOldPetGround != bNewPetGround)
			{
				pet = SetPet( iNewPetClass, (PET_PLACE)slotIndex, pet->GetItemID() ); 
			}
		}
		//--------------------------------------------------
		pet->SetItemNoID( itemNo );
		pet->SetPetAttr( &info );
		pet->SetSkill( vecPetSkill );

		//--------------------------------------------------
		// RESTORE TOGGLE SKILL
		for (int idx = 0; idx < (int)vecPetSkill.size(); ++idx)
		{
			t_Pet_Skill_To_Client &skill = vecPetSkill[idx];
			SPSkill *pkSkill = SPSkillManager::GetInstance()->GetSkill( skill.nSkillID );
			SKILL_SPECIAL_TYPE eSpecialType = SST_NULL;
			if (pkSkill)
				eSpecialType = pkSkill->GetSpecialType();
			if (eSpecialType != SST_NULL)
			{
				if (isSkillNeedRestore( (PET_PLACE)slotIndex, itemNo, eSpecialType ))
				{
					pet->SetSkillStatus( eSpecialType, true );	
				}
			}
		}
		//--------------------------------------------------
		// restore pet chat settings
		SPWindowPetChatEditor *pWndPetChat = (SPWindowPetChatEditor*)g_pInterfaceManager->FindWindow( WIID_PETCHAT_EDITOR );
		if (pWndPetChat)
		{
			std::vector< std::string > &vecChat = pWndPetChat->GetPetChatVector( (PET_PLACE)slotIndex );
			pet->SetChatMsg( vecChat );
		}
		//--------------------------------------------------

		SPWindow *pWnd = g_pInterfaceManager->FindWindow( WIID_PET );
		if (pWnd)
			pWnd->SPSendMessage( SPIM_NEWPET_STAT_UPDATE, (WPARAM)iPlace, (LPARAM)&info );

		if (info.uPetStatus != 1)
		{
			pet->SetShopUncloseError( true );
		}
		//--------------------------------------------------
		if (info.uPetStatus == 2 
			||	
			info.uPetStatus == 3)
		{
			pet->SetSkillStatus( SST_PET_SHOP, true );
		}
		else
		{
			pet->SetSkillStatus( SST_PET_SHOP, false );
		}
		//--------------------------------------------------
	}
	return 0;
}

int SPPlayer::OnPetLearnSkill(LPARAM lParam)
{
	//	GLOBAL_STRING_ID			//	iErrorID;		
	//	ITEMNO	itemNo;				//	ÄÄ¸ö³èÎïÑ§µÄ
	//	int		nSkillID;			//	Ñ§Ï°µÄ¼¼ÄÜ
	//	int		nBaseLevel;			//	Ñ§Ï°¼¼ÄÜµÄµÈ¼¶
	CPacket	*packet = (CPacket*)lParam;
	if (!packet)
		return 1;

	GLOBAL_STRING_ID	errorID;
	packet->ExtractUINT32( &errorID );
	if (errorID)
	{
		if(g_pResourceManager->GetGlobalString(errorID)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(errorID);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
			g_pInterfaceManager->SPChildWndSendMessage( WIID_PET_LEVELUP , SPIM_ITEM_UPGRADE_RESULT , 0, 0, true);
		}
		return 1;
	}	

	ITEMNO itemNo = 0;
	int		nSkillID = 0;
	int		nSkillLevel = 0;
	packet->ExtractUINT64( (UINT64*)(&itemNo) );
	packet->ExtractUINT32( (UINT32*)(&nSkillID) );
	packet->ExtractUINT32( (UINT32*)(&nSkillLevel) );

	SPPet *pet = m_pPetMgr->GetPetFromITEMNO( itemNo );
	if (!pet)
		return 0;

	t_Pet_Skill_To_Client skillinfo;
	skillinfo.nLevel = nSkillLevel;
	skillinfo.nSkillID = nSkillID;
	pet->AddSkill( skillinfo ); 
	g_pInterfaceManager->SPChildWndSendMessage( WIID_PET_LEVELUP , SPIM_ITEM_UPGRADE_RESULT , 0, 0, true);

	return 0;
}

int SPPlayer::UpdatePetInfo( LPARAM lParam )
{
	//PET_SC_PET_ATTR_CHANGE,			//	³èÎïµÄÊôÐÔ¸Ä±ä£¬ÐèÒª¹ã²¥¸ø¸½½üµÄÍæ¼Ò
	//	CHARID  charID;				//	Íæ¼ÒµÄID
	//	UINT8 slotIndex;			//	³èÎïÔÚ³èÎï×°±¸À¸ÖÐµÄÎ»ÖÃ
	//	ITEMNO itemNo;				//	³èÎïµÄÎ¨Ò»±àºÅ
	//	t_PetAttr_Info Info;		//	³èÎïµÄ»ù±¾ÊôÐÔ
	CPacket	*packet = (CPacket*)lParam;
	if (!packet)
		return 0;

	UINT8	slotIndex = 0U;
	ITEMNO	itemNo = 0;
	t_PetAttr_Info	info;
	packet->ExtractUINT8( &slotIndex );
	packet->ExtractUINT64( (UINT64*)&itemNo );
	packet->ExtractStruct( &info, sizeof(t_PetAttr_Info) );

	if (slotIndex <= PET_PRIMARY || slotIndex > PET_PLACE_TYPE_NUMBER)
		return 0;

	SPPet *pet = m_pPetMgr->GetPet( (PET_PLACE)--slotIndex );
	if (pet && pet->IsNewGenerationPet())
	{
		//--------------------------------------------------
		// Èç¹û³èÎïµÄ»ù±¾ÊôÐÔ£¬·ÉÐÐ»òµØÃæ³è¸Ä±ä£¬ÔòÖØÐÂ½¨Á¢Ò»¸ö³èÎï
		if (info.lv != pet->GetPetAttr()->lv)
		{
			bool bOldPetGround = pet->GetClassID() < (CLASS_ID_PET + 100000);
			int iNewPetClass = SPPetLevelMgr::getInstance().GetRealPetClass( pet->GetItemID(), info.lv );
			if (iNewPetClass >= CLASS_ID_PET)
			{
				bool bNewPetGround = iNewPetClass < (CLASS_ID_PET + 100000);
				if (bOldPetGround != bNewPetGround)
				{
					vector< t_Pet_Skill_To_Client > vecSkill = pet->GetSkill();
					int iItemID = pet->GetItemID();
					int iItemNoID = pet->GetItemNoID();
					pet = SetPet( iNewPetClass, (PET_PLACE)slotIndex, iItemID ); 
					pet->SetPetAttr( &info );
					pet->SetSkill( vecSkill );
					pet->SetItemNoID( iItemNoID );
					pet->SetPetGeneration(true);
				}
			}
		}
		//--------------------------------------------------
		if (info.uPetStatus != 1
			&&
			pet->GetPetAttr()->uPetStatus == 1)
		{
			pet->SetSkillStatus( SST_PET_SHOP, false );
			g_pInterfaceManager->SPChildWndSendMessage( WIID_PET, SPIM_REFRESH, 0, 0 );
		}

		if (info.sp != pet->GetPetAttr()->sp
			||
			info.ssp != pet->GetPetAttr()->ssp)
		{
			pet->SetPetAttr( &info );
			g_pInterfaceManager->SPChildWndSendMessage( WIID_QUICK_SLOT , SPIM_REFRESH , NULL , NULL );
		}
		else
		{
			pet->SetPetAttr( &info );
		}

		if (m_bLocalPlayer)
		{
			SPWindow *pWnd = g_pInterfaceManager->FindWindow( WIID_PET );
			if (pWnd)
				pWnd->SPSendMessage( SPIM_NEWPET_STAT_UPDATE, (WPARAM)slotIndex, (LPARAM)&info );
		}
	}
	return 0;
}

int SPPlayer::TakeOffPet( LPARAM lParam )
{
	//	PET_SC_PET_TAKE_OFF,			//	Ð¶ÔØ³èÎïÊ±¹ã²¥
	//	CHARID	charID;
	//	UINT8	slotIndex;
	//	ITEMNO	itemNo;
	return 0;
}

void SPPlayer::OnSetMultiPet( int itemID, PET_PLACE place )
{
	if (itemID <= 0)
		ReleasePet( place );
	else
	{
		SPItemAttr *pItemAttr = NULL;
		g_pItemCluster->GetItemInfo( itemID, pItemAttr );
		if (!pItemAttr)
			return;

		if (pItemAttr->m_iPetClassID)
		{
			SPPet *pet = SetPet( pItemAttr->m_iPetClassID, place, itemID );
			if (pet)
			{
				D3DXCOLOR color(0.5f, 1.0f, 0.5f, 1.0f);
				pet->SetName(pItemAttr->m_strName.c_str(), &color);
				if (pItemAttr->m_iNewPet == 1)
					pet->SetPetGeneration( true );
			}
		}
	}
}

int SPPlayer::OnItemSkillInsert(LPARAM lParam)
{
	CONTAINER_ITEM* pItem = (CONTAINER_ITEM*)lParam;
	if( pItem == NULL )
		return 1;

	SPItemAttr* pItemAttr = NULL;
	if( !g_pItemCluster->GetItemInfo(pItem->Item.iItemID, pItemAttr) )
		return 1;

	OnItemRtrEffectInsert( pItem->Item.iItemID );

	bool bPet = false;
	if( pItem->SlotInfo.ContainerType == FIGURE_PET )
	{
		//OnSetPet(pItem->Item.iItemID);
		OnSetMultiPet( pItem->Item.iItemID, (PET_PLACE)pItem->SlotInfo.SlotIndex );
		bPet = true;
	}

	int i;
	SPSkill* pkSkill;
	SPItemSkillActivity* pkItemSkillActivity;
	SKILL_SPECIAL_TYPE eSpcialType;

	for( i = 0; i < MAX_ITEM_SKILL; i++ )
	{
		if( pItemAttr->m_iSkillID[i] == 0 )
			continue;

		pkSkill = SPSkillManager::GetInstance()->GetSkill(pItemAttr->m_iSkillID[i]);
		if( pkSkill == NULL )
			continue;

		pkItemSkillActivity = m_pkSkillArchive->GetItemSkillActivity( pItemAttr->m_iSkillID[ i ] , (int)pItem->SlotInfo.ContainerType, (int)pItem->SlotInfo.SlotIndex );

		if( pkItemSkillActivity != NULL )
			continue;
//			m_pkSkillArchive->RemoveSkill( pItemAttr->m_iSkillID[ i ] , pItem->Item.iItemID , (int)pItem->SlotInfo.ContainerType, (int)pItem->SlotInfo.SlotIndex );

		pkItemSkillActivity = new SPItemSkillActivity(pkSkill, pItemAttr->m_iSkillLevel[i], 0,
			pItem->Item.iItemID, (int)pItem->SlotInfo.ContainerType, pItem->SlotInfo.SlotIndex);
		
		if( pkSkill->GetType() == ST_TOGGLE )
		{
			if( m_pkEffectArchive->IsExistEffectActivity(pkSkill->GetToggleEffectID()) )
				pkItemSkillActivity->SetToggle(true);
			else
				pkItemSkillActivity->SetToggle(false);
		}

		m_pkSkillArchive->AddSkill(pkItemSkillActivity);

		eSpcialType = pkSkill->GetSpecialType();
		if( eSpcialType != SST_NULL )
		{
			SPPet *pet = m_pPetMgr->GetPet( (PET_PLACE)pItem->SlotInfo.SlotIndex );
			if( bPet && pet )
			{
				pet->SetSpecialSkill(eSpcialType, pItemAttr->m_iSkillID[i], true, false);
				//pet->SetSpecialSkill(eSpcialType, pItemAttr->m_iSkillID[i], pkItemSkillActivity->GetToggleOn(), true);
				pet->SetSkillStatus( eSpcialType, false );
			}
		}

		g_pInterfaceManager->SPChildWndSendMessage( WIID_QUICK_SLOT , SPIM_QUICK_SLOT_UPDATE , -1 , pItemAttr->m_iSkillID[ i ] );
	}

	g_pInterfaceManager->SPChildWndSendMessage( WIID_SKILL , SPIM_SKILL_UPDATE , -1 , -1 );	// AJJIYA [10/25/2005]

	return 1;
}

int SPPlayer::OnItemSkillDelete(LPARAM lParam)
{
	CONTAINER_ITEM* pItem = (CONTAINER_ITEM*)lParam;
	if( pItem == NULL )
		return 1;

	SPItemAttr* pItemAttr = NULL;
	if( !g_pItemCluster->GetItemInfo(pItem->Item.iItemID, pItemAttr) )
		return 1;

	OnItemRtrEffectDelete( pItem->Item.iItemID );

	if( pItem->SlotInfo.ContainerType == FIGURE_PET )
		OnSetMultiPet( -1, (PET_PLACE)pItem->SlotInfo.SlotIndex );

	int i;
	for( i = 0; i < MAX_ITEM_SKILL; i++ )
	{
		if( pItemAttr->m_iSkillID[i] == 0 )
			continue;

		m_pkSkillArchive->RemoveSkill(pItemAttr->m_iSkillID[i], pItem->Item.iItemID,
			(int)pItem->SlotInfo.ContainerType, pItem->SlotInfo.SlotIndex);

		g_pInterfaceManager->SPChildWndSendMessage( WIID_QUICK_SLOT , SPIM_QUICK_SLOT_UPDATE , -1 , pItemAttr->m_iSkillID[ i ] );
	}

	g_pInterfaceManager->SPChildWndSendMessage( WIID_SKILL , SPIM_SKILL_UPDATE , -1 , -1 );	// AJJIYA [10/25/2005]

	return 1;
}

int SPPlayer::OnItemRtrEffectInsert(LPARAM lParam)
{
	if (m_pkRTREffectMgr)
		m_pkRTREffectMgr->AddItemRTREffect( (int)lParam );

	return 1;
}
int SPPlayer::OnItemRtrEffectDelete(LPARAM lParam)
{
	if (m_pkRTREffectMgr)
		m_pkRTREffectMgr->DelItemRTREffect( (int)lParam );

	return 1;
}

int SPPlayer::OnSetGuildInfo(LPARAM lParam)
{
	GUILD_BROADCAST* pGuildInfo = (GUILD_BROADCAST*)lParam;

	if( m_pPlayerStatusModel )
	{
		if( pGuildInfo == NULL || SPLocalizeManager::GetInstance()->IsEnableValue(LV_GUILD) == false || pGuildInfo->iGNum < 1 )
		{
			m_pPlayerStatusModel->SetGuildInfo(NULL, 0, 0);
		}
		else
		{
			m_pPlayerStatusModel->SetGuildInfo(pGuildInfo->szGName,	pGuildInfo->arrGMark[0], pGuildInfo->arrGMark[1], pGuildInfo->arrGMark[2]);
		}
	}

	return 1;
}


//	AJJIYA [8/25/2005]
//	¸»Ç³¼±
void SPPlayer::SetTalkBalloon( const char* pszString , BROADCAST_SCOPE eScope , D3DXCOLOR color )
{
	if( m_pPlayerStatusModel != NULL )
		m_pPlayerStatusModel->SetTalkBalloon( pszString , eScope , color );
}

void SPPlayer::SetTitle( SPTitleData& stTitleData )
{
	if( m_pPlayerStatusModel != NULL )
		m_pPlayerStatusModel->SetTitle( stTitleData );

	if( m_pPlayerStatus != NULL )
	{
		SPPlayerStatus*	pPlayerStatus	=	(SPPlayerStatus*)m_pPlayerStatus;

		if( pPlayerStatus != NULL )
			pPlayerStatus->SetTitleID( stTitleData.m_uiTitleID );
	}
}

SPTitleRenderBase* SPPlayer::GetTitle()
{
	if( m_pPlayerStatusModel != NULL )
		return m_pPlayerStatusModel->GetTitle();

	return NULL;
}

UINT64 SPPlayer::GetCurAction()
{
	return m_uiCurAction;
}

bool SPPlayer::IsMaster()
{
	bool bRet = false;
	if( m_pkTeamArchive )
		bRet = m_pkTeamArchive->IsMaster();

	return bRet;
}

bool SPPlayer::GetTeamActive()
{
	bool	bTeamActive	=	false;

	if( m_pkTeamArchive != NULL )
		bTeamActive	=	m_pkTeamArchive->GetActive();

	return bTeamActive;
}

SPTeamArchive* SPPlayer::GetTeam()
{
	return m_pkTeamArchive;
}

//////////////////////////////////////////////////////////////////////////
//
//	InsertDamageFx
//

void SPPlayer::InsertDamageFX( FXUNIT_DAMAGE_TYPE eDamageType , int iDamage , int iMode , int iType )
{
	//
	// AJJIYA [5/23/2005]
	// µ¥¹ÌÁö ¼ýÀÚ ¶ß´Â FX
	POINT	ptGOBBox;
	SPCharacterBeAttacked*	pkBeAttacked = NULL;

	g_pBattleCluster->GetAttack(m_pGOBModelUnit->SPSendMessage(MV_GET_ANIMATION_INDEX), &pkBeAttacked);

	ptGOBBox.x	=	0;
	ptGOBBox.y	=	0;		//	±âº» Ä³¸¯ÅÍ ³ôÀÌ

	//	Ä³¸¯ÅÍ°£ ³ôÀÌ º¸Á¤¿ë!!
	if( pkBeAttacked )
	{
		int nFrame		= (int)m_pGOBModelUnit->SPSendMessage(MV_GET_CURR_FRAME);
		int nMaxFrame	= (int)pkBeAttacked->m_BeAttackInfo.size();

		if( nFrame >= nMaxFrame )
			nFrame	=	nMaxFrame - 1;

		APCBEATTACKEDINFO	beAttackedInfo	=	pkBeAttacked->m_BeAttackInfo[ nFrame ];

		if( nFrame <= beAttackedInfo.m_nIndex )
			ptGOBBox.y	=	abs( beAttackedInfo.m_rtRange.bottom	-	beAttackedInfo.m_rtRange.top );
	}

	if( iMode == 1 )	// Å©¸®Æ¼ÄÃ °ø°Ý [6/27/2006]
	{
		if( iDamage > 0 )
		{
			m_pFXArchive->SetGOBBox( ptGOBBox );
			m_pFXArchive->SetCritical( true );
			m_pFXArchive->InsertFXGroup( IDFXGR_CRITICAL );
		}
	}
	else if( iMode == 2 )	// ¹Ì½º °ø°Ý [6/27/2006]
	{
		m_pFXArchive->InsertFXGroup( IDFXGR_DAMAGE_MISS );
	}
	else if( iMode == 3 )	// Immune °ø°Ý [6/27/2006]
	{
		m_pFXArchive->InsertFXGroup( IDFXGR_DAMAGE_IMMUNE );
	}

	if( iType == FDET_POSION || iType == FDET_BLEEDING || iType == FDET_CURSE )
	{
		eDamageType	=	FDT_POSION;
	}

	if( iDamage > 0 )		// ÀÏ¹Ý °ø°Ý [6/27/2006]
	{
		m_pFXArchive->SetDamage( iDamage );
		m_pFXArchive->SetGOBBox( ptGOBBox );
		m_pFXArchive->SetDamageType( eDamageType );
		m_pFXArchive->InsertFXGroup( IDFXGR_DAMAGE );
	}

	if( ( iMode == 0 || iMode == 1 ) && iDamage <= 0 )
	{
		m_pFXArchive->InsertFXGroup( IDFXGR_DAMAGE_MISS );
	}
}

void SPPlayer::InsertHitComboFX( UINT8 uiHitComboCount , SPFXArchive* pLocalFXArchive )
{
	if( pLocalFXArchive == NULL )
		return;

	//////////////////////////////////////////////////////////////////////////
	//	¼ýÀÚ °ü·Ã
	//////////////////////////////////////////////////////////////////////////

	UINT	uiFXGHitNumber		=	5007;

	if( pLocalFXArchive->IsFXGroup( uiFXGHitNumber ) == true )
		pLocalFXArchive->DeleteFXGroup( uiFXGHitNumber );

	pLocalFXArchive->SetDamage( (int)uiHitComboCount );
	pLocalFXArchive->InsertFXGroup( (SPID_FX_GROUP)uiFXGHitNumber );

	//////////////////////////////////////////////////////////////////////////
	//	ÄÞº¸ ½ÃÀÛ °ü·Ã
	//////////////////////////////////////////////////////////////////////////

	UINT	uiFXGHitComboStart	=	5005;
	UINT	uiFXGHitComboIng	=	5006;
	UINT	uiFXGHitComboEnd	=	5014;

	bool	bIsFXGHitComboStart	=	pLocalFXArchive->IsFXGroup( uiFXGHitComboStart , uiFXGHitComboStart );
	bool	bIsFXGHitComboIng	=	pLocalFXArchive->IsFXGroup( uiFXGHitComboStart , uiFXGHitComboIng );

    if( bIsFXGHitComboStart == false && bIsFXGHitComboIng == false )
	{
		pLocalFXArchive->AddNextFXGroupID( uiFXGHitComboIng );
		pLocalFXArchive->AddNextFXGroupID( uiFXGHitComboEnd );

		pLocalFXArchive->InsertFXGroup( (SPID_FX_GROUP)uiFXGHitComboStart );
	}
	else if( bIsFXGHitComboStart == false && bIsFXGHitComboIng == true )
	{
		pLocalFXArchive->DeleteFXGroup( uiFXGHitComboIng );
		pLocalFXArchive->AddNextFXGroupID( uiFXGHitComboEnd );

		pLocalFXArchive->InsertFXGroup( (SPID_FX_GROUP)uiFXGHitComboIng );
	}

	//////////////////////////////////////////////////////////////////////////
	//	ºÎ°¡ ±ÛÀÚ °ü·Ã
	//////////////////////////////////////////////////////////////////////////

	UINT	uiFXGHitComboAdd	=	0;
	UINT	uiSoundEffect		=	0;

	switch( uiHitComboCount )
	{
		case 10:	uiFXGHitComboAdd	=	5008;	uiSoundEffect	=	783;	break;
		case 30:	uiFXGHitComboAdd	=	5009;	uiSoundEffect	=	784;	break;
		case 50:	uiFXGHitComboAdd	=	5010;	uiSoundEffect	=	785;	break;
		case 70:	uiFXGHitComboAdd	=	5011;	uiSoundEffect	=	786;	break;
		case 90:	uiFXGHitComboAdd	=	5012;	uiSoundEffect	=	787;	break;
		case 99:	uiFXGHitComboAdd	=	5013;	uiSoundEffect	=	0;		break;
	}

	if( uiFXGHitComboAdd > 0 )
	{
		pLocalFXArchive->InsertFXGroup( (SPID_FX_GROUP)uiFXGHitComboAdd );

		if( uiSoundEffect > 0 )
			SetActionSound( uiSoundEffect , true );
	}
}

SPPet* SPPlayer::SetPet(unsigned int uiClassID, PET_PLACE place, int iItemID)
{
	SPPet *pet = NULL;
	ReleasePet( place );
	if( uiClassID == 0 )
		return NULL;

	float fJumpSpeed = 0.0f;
	if( uiClassID < (CLASS_ID_PET + 100000) )
	{
		pet = new SPGroundPet(GetInstanceID(), (CLASS_ID)uiClassID, this);
		fJumpSpeed = PET_JUMP_VELOCITY;
	}
	else
	{
		pet = new SPFlyPet(GetInstanceID(), (CLASS_ID)uiClassID, this);
		fJumpSpeed = -PET_WALK_VELOCITY;
	}

	pet->SetPetPlace( place );
	pet->Init();
	pet->SetGravity(m_pkCoordPhysics->GetGravity(), m_pkCoordPhysics->GetMaxDropVelocity());
	pet->SetJumpSpeed(fJumpSpeed);
	pet->SetItemID( iItemID );
	//--------------------------------------------------
	SPItemAttr *pItemAttr = NULL;
	g_pItemCluster->GetItemInfo( iItemID, pItemAttr );
	if (pItemAttr)
	{
		D3DXCOLOR color(0.5f, 1.0f, 0.5f, 1.0f);
		pet->SetName(pItemAttr->m_strName.c_str(), &color);
		if (pItemAttr->m_iNewPet == 1)
			pet->SetPetGeneration( true );
	}
	//--------------------------------------------------

	if( m_bLocalPlayer )
		pet->SetLocal(true);
	
	m_fAccumulatePetLooting = 0.0f;
	m_fAccmulatePetCheckLooting = 0.0f;
	m_fAccumulatePetItemUsed = 0.0f; 

	LT_POSITION Pos;
	ZeroMemory(&Pos, sizeof(LT_POSITION));

	Pos.fPosX = GetPosX();
	Pos.fPosY = GetPosY();
	Pos.bgStandLayer = GetStandLayer();
	Pos.eFacing = GetDirection();

	pet->SetCurStageID(GetCurStageID());
	pet->SetCurMapGroupID(GetCurMapGroupID());

	pet->SPGOBPerformMessage(SPGM_SET_SC_LPPOSITION, (LPARAM)&Pos);
	if (m_pPetMgr)
		m_pPetMgr->SetPet( place, pet );
	return pet;
}

void SPPlayer::ReleasePet(PET_PLACE place)
{
	//SAFE_DELETE(m_pkPet);
	if (m_pPetMgr)
		m_pPetMgr->DelPet( place );

	m_pkPetCurLootingBag = NULL;
}

void SPPlayer::UpdatePetSpecialSkillToggle()
{
	/*
	for (int idx = 0; idx < (int)PET_PLACE_TYPE_NUMBER; ++idx)
	{
		SPPet *pet = m_pPetMgr->GetPet( (PET_PLACE)idx );
		if( pet == NULL )
			return;

		std::vector<SKILL_SPECIAL_TYPE> eSpecialType = pet->GetSpecialSkillType();
		std::vector<int> iSkillID = pet->GetSpecialSkillID();
		assert( eSpecialType.size() == iSkillID.size() );
		for (int count = 0; count < (int)eSpecialType.size(); ++count)
		{
			if( eSpecialType[count] == SST_NULL || iSkillID[count] == 0 )
				continue;

			SPSkillActivity* pkSkillActivity = m_pkSkillArchive->GetItemSkillActivity(
				iSkillID[count], FIGURE_PET, idx);
			if( pkSkillActivity == NULL )
				continue;

			pet->SetSpecialSkill(eSpecialType[count], iSkillID[count], pkSkillActivity->GetToggleOn(), true);
		}
	}
	*/
}


bool SPPlayer::SetWantTeam(bool bShow, int iClass, int iLevel)
{
	m_bRenderPartyFind = bShow;
	m_iClass = iClass;
	ZeroMemory(m_szLevel, 4);
	sprintf(m_szLevel, "%d", iLevel);	
	return true;
}


void SPPlayer::SetPartyFindRenderDest(int iCenterX, int iCenterY)
{	
	if(m_bRenderPartyFind) {
		int iStartX = iCenterX + 4;
		//int iStartY = iCenterPosY - 350;
		int iStartY = iCenterY - 140;		
		SetRect(&m_rtDestPartyFindTop,		iStartX, iStartY + 16, iStartX + 65, iStartY + 16 + 4);
		SetRect(&m_rtDestPartyFindCenter,	iStartX, iStartY + 20, iStartX + 65, iStartY + 20 + 13);
		SetRect(&m_rtDestPartyFindBottom,	iStartX, iStartY + 33, iStartX + 65, iStartY + 33 + 7);
		SetRect(&m_rtDestPartyFindTail,		iStartX + 36, iStartY, iStartX + 36 + 27, iStartY + 16);
		
		SetRect(&m_rtDestPartyFindLvImg,	iStartX + 24, iStartY + 26, iStartX + 24 + 14, iStartY + 26 + 5);
		SetRect(&m_rtDestPartyFindLv,		iStartX + 41, iStartY + 21, iStartX + 41 + 21, iStartY + 21 + 12);
		SetRect(&m_rtDestPartyFindClass,	iStartX + 3, iStartY + 18, iStartX + 3 + 16, iStartY + 18 + 16);		
	}
}


void SPPlayer::RenderPartyFind(float fTime)
{	
	if(m_pPartyFindTexture) {
		m_pPartyFindTexture->RenderTexture(&m_rtDestPartyFindTail, &m_rtSrcPartyFindTail);
		m_pPartyFindTexture->RenderTexture(&m_rtDestPartyFindTop, &m_rtSrcPartyFindTop);
		m_pPartyFindTexture->RenderTexture(&m_rtDestPartyFindCenter, &m_rtSrcPartyFindCenter);
		m_pPartyFindTexture->RenderTexture(&m_rtDestPartyFindBottom, &m_rtSrcPartyFindBottom);
		m_pPartyFindTexture->RenderTexture(&m_rtDestPartyFindLvImg, &m_rtSrcPartyFindLvImg);
		g_pVideo->Flush();
	}

//#ifdef _DEBUG
//	RECT rtRect = {0, 0, 0, 0};	
//	g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
//	g_pVideo->GetSysLine()->DrawRect(m_rtDestPartyFindTail);
//	g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
//#endif

	if(m_pClassTexture) {
		m_pClassTexture->RenderTexture(&m_rtDestPartyFindClass, &m_rtSrcClass[m_iClass]);

		g_pVideo->Flush();
		
		SPFont* pFont = g_pVideo->GetFont(FONT_12_NORMAL);
		if(pFont) {
			pFont->SetShadow(false);
			pFont->SetColor( RGBA(0, 0, 0, 255) );
			pFont->RenderText( m_szLevel , &m_rtDestPartyFindLv,  DT_LEFT | DT_VCENTER | DT_WORDBREAK );			
			pFont->SetColor();
			pFont->SetShadow(true);
		}			
	}	
}


void SPPlayer::SetCriticalEffect(bool bCritical /*= true*/)
{
	if(m_bLocalPlayer == false)
		return;
	
	if(m_bCriticalEffect)
		return;
	
	m_bCriticalEffect				= true;
	m_fTotalCriticalAccumulate		= 0.0f;
	m_fTotalCriticalDelay			= 0.5f;
	m_fCriticalAccumulate			= 0.0f;
	m_fCriticalDelay				= 0.01f;
	m_bCamMoveLeft					= true;
	m_fLastCamX						= g_pCameraManagr->GetCameraStruct()->fOffsetX;
	m_fLastCamY						= g_pCameraManagr->GetCameraStruct()->fOffsetY;
	DXUTOutputDebugString("SetCriticalEffect\n");
}


void SPPlayer::SetPlayerClass(unsigned char iClass)
{
	m_iClass = iClass;
}


unsigned char SPPlayer::GetPlayerClass()
{	
	////
	//if(m_pPlayerStatus) {
	//	SPPlayerStatus* pPlayerStatus = (SPPlayerStatus*)m_pPlayerStatus;
	//	return pPlayerStatus->GetStatusValue(STATUS_CLASS_TYPE);
	//}
	//return CLASS_NULL;

	return m_iClass;
}


void SPPlayer::ProcessCriticalEffect(float fTime)
{
	if(m_bLocalPlayer == false)
		return;

	if(m_bCriticalEffect == false)
		return;
	
	m_fTotalCriticalAccumulate += fTime;
	if(m_fTotalCriticalAccumulate > m_fTotalCriticalDelay){
		m_fTotalCriticalAccumulate -= m_fTotalCriticalDelay;
		//Vibrate Á¾·á
		m_bCriticalEffect = false;
		g_pCameraManagr->GetCameraStruct()->fOffsetX = m_fLastCamX;
		g_pCameraManagr->GetCameraStruct()->fOffsetY = m_fLastCamY;
	}

	if(m_bCriticalEffect) {
		m_fCriticalAccumulate += fTime;
		if(m_fCriticalAccumulate > m_fCriticalDelay) {
			m_fCriticalAccumulate -= m_fCriticalDelay;
			if(m_bCamMoveLeft) {
				g_pCameraManagr->GetCameraStruct()->fOffsetX -= 5;
				m_bCamMoveLeft = false;
			}
			else {
				g_pCameraManagr->GetCameraStruct()->fOffsetX += 5;
				m_bCamMoveLeft = true;
			}
		}
	}	
}


void SPPlayer::RenderCriticalEffect(float fTime)
{
	return;
}


//////////////////////////////////////////////////////////////////////////
//
// Friend List Inteface [1/26/2007 AJJIYA]
//

bool SPPlayer::AddFriendList( FRIEND_INFO* pFriendInfo )
{
	if( pFriendInfo == NULL )
		return false;

	std::string	strFriendName;

	strFriendName.clear();
	GetUpperString3( strFriendName , pFriendInfo->szCharName );

	DelFriendList( strFriendName.c_str() );

	m_mpFriendList.insert( std::map< std::string , FRIEND_INFO* >::value_type( strFriendName , pFriendInfo ) );
	return true;
}

bool SPPlayer::DelFriendList( const char* pszName )
{
	if( pszName == NULL )
		return false;

	std::string					strFriendName;

	strFriendName.clear();
	GetUpperString3( strFriendName , pszName );

	std::map< std::string , FRIEND_INFO* >::iterator	mIter	=	m_mpFriendList.find( strFriendName );

	if( mIter != m_mpFriendList.end() )
	{
		SAFE_DELETE( (*mIter).second );
		m_mpFriendList.erase( mIter );
		return true;
	}

	return false;
}

bool SPPlayer::DelFriendListAll()
{
	std::map< std::string , FRIEND_INFO* >::iterator	mIter	=	m_mpFriendList.begin();

	while ( mIter != m_mpFriendList.end() )
	{
		SAFE_DELETE( (*mIter).second );
		++mIter;
	}

	m_mpFriendList.clear();
	return true;
}

int SPPlayer::GetFriendListCount()
{
	return (int)m_mpFriendList.size();
}

FRIEND_INFO* SPPlayer::GetFriendListMember( int iPos )
{
	std::map< std::string , FRIEND_INFO* >::iterator	mIter	=	m_mpFriendList.begin();

	for( int i = 0 ; i < iPos ; ++i )
	{
		++mIter;
	}

	if( mIter != m_mpFriendList.end() )
	{
		return (*mIter).second;
	}

	return NULL;
}

FRIEND_INFO* SPPlayer::GetFriendListMember( const char* pszName )
{
	std::string					strFriendName;

	strFriendName.clear();
	GetUpperString3( strFriendName , pszName );

	std::map< std::string , FRIEND_INFO* >::iterator	mIter	=	m_mpFriendList.find( strFriendName );

	if( mIter != m_mpFriendList.end() )
	{
		return (*mIter).second;
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////

void SPPlayer::SetLocalPlayerTeam( bool bLocalPlayerTeam )
{
	if( m_pPlayerStatusModel != NULL )
		m_pPlayerStatusModel->SetLocalPlayerTeam( bLocalPlayerTeam );
}

bool SPPlayer::GetLocalPlayerTeam()
{
	bool	bLocalPlayerTeam	=	false;

	if( m_pPlayerStatusModel != NULL )
		bLocalPlayerTeam = m_pPlayerStatusModel->GetLocalPlayerTeam();

	return bLocalPlayerTeam;
}

//////////////////////////////////////////////////////////////////////////

void SPPlayer::SetSkillInitData( UINT32 uiSaveCheck , int iSkillPoint , bool bUIRefresh )
{
	SPGOBStatus*	pPlayerStatus	=	GetStatus();

	if( pPlayerStatus != NULL && iSkillPoint >= 0 )
		pPlayerStatus->SetStatusValue( STATUS_SKILL_POINT , iSkillPoint );

	//	½ºÅ³ ÃÊ±âÈ­
	if( m_pkSkillArchive != NULL )
		m_pkSkillArchive->ReInitSkill( uiSaveCheck );

	if( bUIRefresh == true &&g_pInterfaceManager != NULL )
	{
		// ½ºÅ³ º¯°æ½Ã ½ºÅ³ À©µµ¿ì·Î ¸Þ¼¼Áö Åëº¸ AJJIYA [7/28/2005]
		g_pInterfaceManager->SPChildWndSendMessage( WIID_SKILL , SPIM_SKILL_UPDATE , -1 , -1 );	// AJJIYA [10/25/2005]

		// ½ºÅ³ º¯°æ½Ã Äü½½·Ô À©µµ¿ì·Î ¸Þ¼¼Áö Åëº¸ AJJIYA [7/28/2005]
		g_pInterfaceManager->SPChildWndSendMessage( WIID_QUICK_SLOT , SPIM_QUICK_SLOT_UPDATE , -1 , -1 );	// AJJIYA [10/25/2005]

		// ½ºÅ³ º¯°æ½Ã ÀÌ¸ðÆ¼ÄÜ À©µµ¿ì·Î ¸Þ¼¼Áö Åëº¸ AJJIYA [8/30/2005]
		g_pInterfaceManager->SPChildWndSendMessage( WIID_QUICK_SLOT , SPIM_UPDATE_EMOTICON , -1 , -1 );		// AJJIYA [10/25/2005]
	}
}

D3DXCOLOR SPPlayer::getPhantomColor( void ) const
{
	return m_colorPhantom;	
}

bool SPPlayer::setPhantomColor(D3DXCOLOR color)
{
	m_colorPhantom = color;
	return true;
}

SPPet	*SPPlayer::GetPlayerPet( void )
{
	if (m_pPetMgr)
		return m_pPetMgr->GetPrimaryPet();
	else
		return NULL;
}

int SPPlayer::OnPetAddSSP(LPARAM lParam)
{
	CPacket	*packet = (CPacket*)lParam;
	if (!packet)
		return 1;

	GLOBAL_STRING_ID	errorID;
	packet->ExtractUINT32( &errorID );
	if (errorID)
	{
		if(g_pResourceManager->GetGlobalString(errorID)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(errorID);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}
		return 1;
	}
	return 1;
}

int SPPlayer::OnPetChangeName(LPARAM lParam)
{
	CPacket	*packet = (CPacket*)lParam;
	if (!packet)
		return 1;

	GLOBAL_STRING_ID	errorID;
	packet->ExtractUINT32( &errorID );
	if (errorID)
	{
		if(g_pResourceManager->GetGlobalString(errorID)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(errorID);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}
		return 1;
	}
	return 1;
}

int SPPlayer::OnPetShout(LPARAM lParam)
{
	CPacket	*packet = (CPacket*)lParam;
	if (!packet)
		return 1;

	GLOBAL_STRING_ID	errorID;
	packet->ExtractUINT32( &errorID );
	// msg
	UINT8 slotIndex = 0;
	packet->ExtractUINT8( &slotIndex );
	if (slotIndex ==0)
		slotIndex = PET_PLACE_TYPE_NUMBER;
	else
		--slotIndex;

	if (errorID)
	{
		if(g_pResourceManager->GetGlobalString(errorID)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(errorID);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}
		SPPet *pet = m_pPetMgr->GetPet( (PET_PLACE)slotIndex );
		if (pet)
		{
			pet->SetAutoChat( false );
			pet->SetSkillStatus( SST_PET_CHAT, false );
			g_pInterfaceManager->SPChildWndSendMessage( WIID_PET , SPIM_REFRESH , NULL , NULL );
		}
		return 1;
	}

	char strMsg[51] = {0,};
	packet->ExtractData( (TCHAR*)(&strMsg), 50 );
	SPPet *pet = m_pPetMgr->GetPet( (PET_PLACE)slotIndex );
	if (pet)
	{
		pet->SetTalkBalloon( strMsg );
	}

	return 1;
}

int SPPlayer::OnPetShopForceClosed(LPARAM lParam)
{
	CPacket *packet = (CPacket*)lParam;
	if (!packet)
		return 0;

	ITEMNO itemNo = 0;
	packet->ExtractUINT64( (UINT64*)&itemNo );
	SPPet *pet = m_pPetMgr->GetPetFromITEMNO( itemNo );
	if (pet)
	{
		char buf[LEN_NAME+1] = {0,};
		_snprintf( buf, LEN_NAME, "%s", pet->GetPetAttr()->szPetName );
		char bufMsg[512] = {0,};
		//_snprintf( bufMsg, 511, "ÄúµÄ³èÎï[%s]¿ªÉèµÄÉÌµêÃ»ÓÐÕý³£¹Ø±Õ¡£ÇëÖ÷¶¯¹Ø±ÕÉÌµêÒÔÊÕ»Ø³èÎï¡£", buf );
		_snprintf( bufMsg, 511, "[ÏµÍ³]ÄúµÄ³èÎï[%s]Ëù¿ªÉèÉÌµêÎïÆ·ÒÑÊÛóÀ»òÐèÒª²¹³äÄÜÁ¿£¬Çë¹Ø±ÕÉÌµêÒÔÊÕ»Ø³èÎï£¬½áËãÊÕÈë¡£", buf );
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)bufMsg );
	}

	return 0;
}
