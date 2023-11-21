

#include "SPCommon.h"

#include "Packet.h"

#include "SPUtil.h"

#include "SPRenderModelDEF.h"
#include "SPGameObjectDef.h"
#include "SPGameObject.h"
#include "SPGOBStatus.h"

#include "SPStage.h"
#include "SPStageManager.h"
#include "SPTerrainAttribute.h"

#include "SPPlayerStatus.h"
#include "SPPlayerStatusModel.h"
#include "SPPlayerInvenArchive.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGOBCoordPhysics.h"
#include "SPPlayer.h"

#include "SPManager.h"
#include "SPGOBManager.h"

#include "SPOptionDef.h"
#include "SPClientOptionArchive.h"

#include "SPEventDEF.h"
#include "SPClassChangeManager.h"
#include "SPEventManager.h"

#include "SPMouseCursor.h"
#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowStorage.h"
#include "SPInterfaceManager.h"
#include "SPPvpManager.h"

#include "SPTitleArchive.h"

#include "SPHotItemManager.h"
#include "SPCashTradeManager.h"

#include "SPResourceManager.h"
#include "_Error_log.h"

SPPlayerStatus::SPPlayerStatus(SPGameObject* pParent)
: SPGOBStatus(pParent)
, m_uiTitleID		(0)
, m_uiFame			(0)
, m_uiFameBase		(0)
, m_Gender			(FEMALE)
, m_uiMoney			(0)
, m_uiStorageMoney	(0)
, m_cCharType		(0)
, m_iSkillPoint		(0)
, m_iGameOptions	(SSO_DEFAULT)
, m_iCash			(0)
, m_iPoint			(0)
, m_iLovePoint		(0)
, m_iLovePointMax	(0)
, m_iLoveDay		(0)
, m_iLoveState		(0)
, m_iExpandCharSlot (0)
, m_iLoveLevel		(0)
, m_iSummonMobCount	(0)
, m_iRankingLevel	(0)
, m_iRankingPvp		(0)
, m_iRankingLove	(0)
, m_iRankingGuild	(0)
{
	ZeroMemory(&m_CharExp,		sizeof(CHAR_EXP));
	ZeroMemory(&m_stItemMix,	sizeof(ITEMMIX_INFO));
	ZeroMemory(&m_stPvpStat,	sizeof(PVP_STATUS));
	
	Init();	
}

SPPlayerStatus::~SPPlayerStatus()
{
}

void SPPlayerStatus::Init()
{
	m_strName = "Dummy";
}

UINT SPPlayerStatus::GetStatusValue(STATUS_TYPE StatusType)
{
	switch(StatusType) {
	case STATUS_FAME:
		return m_uiFame 	;
	case STATUS_FAME_BASE:
		return m_uiFameBase;
	case STATUS_SKILL_POINT:
		return m_iSkillPoint;
	case STATUS_GENDER:
		return m_Gender;
	case STATUS_CLASS_TYPE:
		return m_cCharType;

	case STATUS_AMOR:						// ¹æ¾î·Â
		return m_uiArmor ;
	case STATUS_RES_EARTH:					// ÀúÇ×·Â
		return m_uiRegistEarth ;
	case STATUS_RES_WIND:					// ÀúÇ×·Â
		return m_uiRegistWind ;
	case STATUS_RES_WATER:					// ÀúÇ×·Â
		return m_uiRegistWater ;
	case STATUS_RES_FIRE:					// ÀúÇ×·Â
		return m_uiRegistFire ;
	case STATUS_REQ_LV:						// Á¦ÇÑ·¹º§½ºÅÝ
		return m_uiRequireLv ;
	case STATUS_PROB_ENCHANT:				// ÀÎÃ¾Æ® ¼º°øÈ®·üº¸Á¤.
		return m_uiProvEnchant ;
	case STATUS_CASH:						// Ä³½Ã
		return m_iCash;
	case STATUS_MILE:						// Ä³½Ã ¸¶ÀÏ¸®Áö(Æ÷ÀÎÆ®)
		return m_iPoint;
	
	case STATUS_PVP_LV:						// Pvp Lv
		return m_stPvpStat.iLv;
	case STATUS_PVP_POINT:					// Pvp Point
		return m_stPvpStat.iPoint;

	case STATUS_LOVE_LV:		return m_iLoveLevel;		// ¿¬·¡ ·¹º§ [10/27/2008 AJJIYA]
	case STATUS_LOVE_POINT:		return m_iLovePoint;		// ¿¬¾Ö Æ÷ÀÎÆ® ÇöÀç°ª [10/2/2008 AJJIYA]
	case STATUS_LOVE_POINT_MAX: return m_iLovePointMax;		// ¿¬¾Ö Æ÷ÀÎÆ® ÃÖ´ë°ª [10/2/2008 AJJIYA]
	case STATUS_LOVE_DAY:		return m_iLoveDay;			// ¿¬¾Ö ±³Á¦ ÀÏ ¼ö [10/2/2008 AJJIYA]
	case STATUS_LOVE_STATE:		return m_iLoveState;		// ¿¬¾Ö »óÅÂ ( 0 : None , 1 : ±³Á¦»óÅÂ , 2 : ÇÁ·ÎÆ÷Áî ¿Ï·á »óÅÂ , 4 : °áÈ¥»óÅÂ ) [10/2/2008 AJJIYA]
	case STATUS_EXTENDED_CHARSLOT:		return m_iExpandCharSlot;		// Ä³¸¯ÅÍ È®Àå ½½·Ô
	case STATUS_MOB_COUNT:				return m_iSummonMobCount;		// ¸ó½ºÅÍ ¼ÒÈ¯ °³Ã¼¼ö
	
	case STATUS_RANKING_LEVEL:			return m_iRankingLevel;	// ·©Å·½Ã½ºÅÛ : ·¹º§·©Å·
	case STATUS_RANKING_PVP:			return m_iRankingPvp;	// ·©Å·½Ã½ºÅÛ : PVP
	case STATUS_RANKING_LOVE:			return m_iRankingLove;	// ·©Å·½Ã½ºÅÛ : ¾ÖÁ¤µµ
	case STATUS_RANKING_GUILD:			return m_iRankingGuild;	// ·©Å·½Ã½ºÅÛ : ±æµå
	}
	return SPGOBStatus::GetStatusValue(StatusType);
}

bool SPPlayerStatus::SetStatusValue(STATUS_TYPE StatusType, UINT uiValue)
{
	bool bVal = false;
	SPPlayer* pPlayer = NULL;
	
	int iUpdateValue = 0;
	char szMsg[256];
	D3DXCOLOR color( 0.62745f , 1.0f, 0.0f, 1.0f);
	
	switch(StatusType) {
	case STATUS_FAME:
		m_uiFame 	= uiValue;		
		bVal |= true;
		break;
	case STATUS_CLASS_TYPE:
		m_cCharType = (UINT8)uiValue;		
		bVal |= true;
		
		pPlayer = static_cast<SPPlayer*>(m_pParent);
		if(pPlayer)
			pPlayer->SetPlayerClass(m_cCharType);
		
		break;
	case STATUS_FAME_BASE:
		m_uiFameBase = uiValue;
		bVal |= true;
		break;
	case STATUS_SKILL_POINT:
		m_iSkillPoint = uiValue;
		bVal |= true;
		break;
	case STATUS_AMOR:						// ¹æ¾î·Â
		m_uiArmor  = uiValue;
		bVal |= true;
		break;
	case STATUS_RES_EARTH:					// ÀúÇ×·Â
		m_uiRegistEarth  = uiValue;
		bVal |= true;
		break;
	case STATUS_RES_WIND:					// ÀúÇ×·Â
		m_uiRegistWind  = uiValue;
		bVal |= true;
		break;
	case STATUS_RES_WATER:					// ÀúÇ×·Â
		m_uiRegistWater  = uiValue;
		bVal |= true;
		break;
	case STATUS_RES_FIRE:					// ÀúÇ×·Â
		m_uiRegistFire  = uiValue;
		bVal |= true;
		break;
	case STATUS_REQ_LV:						// Á¦ÇÑ·¹º§½ºÅÝ
		m_uiRequireLv  = uiValue;
		bVal |= true;
		break;
	case STATUS_PROB_ENCHANT:				// ÀÎÃ¾Æ® ¼º°øÈ®·üº¸Á¤.
		m_uiProvEnchant  = uiValue;
		bVal |= true;
		break;
	case STATUS_CASH:						// Ä³½Ã
		if( m_pParent->IsLocalPlayer()) {
			iUpdateValue = uiValue - m_iCash;	
			if(iUpdateValue != 0 && g_pResourceManager->GetGlobalString(9001019)) {			
				ZeroMemory(szMsg, 256);
				sprintf(szMsg, g_pResourceManager->GetGlobalString(9001019), ConvertMoneyToString(uiValue));
				g_pInterfaceManager->SPChildWndSendMessage( WIID_SYSTEM_MESSAGE , SPIM_SYSTEM_MESSAGE_ADD , (WPARAM)szMsg , (LPARAM)&color );
			}
		}
		m_iCash = uiValue;
		bVal |= true;
		break;
	case STATUS_MILE:						// Ä³½Ã ¸¶ÀÏ¸®Áö(Æ÷ÀÎÆ®)
		if( m_pParent->IsLocalPlayer()) {
			iUpdateValue = uiValue - m_iPoint;	
			if(iUpdateValue != 0 && g_pResourceManager->GetGlobalString(9001018)) {			
				ZeroMemory(szMsg, 256);
				sprintf(szMsg, g_pResourceManager->GetGlobalString(9001018), ConvertMoneyToString(uiValue));
				g_pInterfaceManager->SPChildWndSendMessage( WIID_SYSTEM_MESSAGE , SPIM_SYSTEM_MESSAGE_ADD , (WPARAM)szMsg , (LPARAM)&color );
			}
		}
		m_iPoint = uiValue;
		bVal |= true;
		break;
	case STATUS_PVP_LV:						// Pvp Lv
		m_stPvpStat.iLv		= uiValue;
		bVal |= true;
		break;
	case STATUS_PVP_POINT:					// Pvp Point
		m_stPvpStat.iPoint	= uiValue;
		bVal |= true;
		break;
	case STATUS_LOVE_LV:					// ¿¬·¡ ·¹º§ [10/27/2008 AJJIYA]
		m_iLoveLevel		= uiValue;
		bVal |= true;
		break;
	case STATUS_LOVE_POINT:					// ¿¬¾Ö Æ÷ÀÎÆ® ÇöÀç°ª [10/2/2008 AJJIYA]
		SetLovePoint( uiValue , true );
		bVal |= true;
		break;
	case STATUS_LOVE_POINT_MAX:				// ¿¬¾Ö Æ÷ÀÎÆ® ÃÖ´ë°ª [10/2/2008 AJJIYA]
		m_iLovePointMax		= uiValue;
		bVal |= true;
		break;
	case STATUS_LOVE_DAY:					// ¿¬¾Ö ±³Á¦ ÀÏ ¼ö [10/2/2008 AJJIYA]
		m_iLoveDay			= uiValue;
		bVal |= true;
		break;
	case STATUS_LOVE_STATE:					// ¿¬¾Ö »óÅÂ ( 0 : None , 1 : ±³Á¦»óÅÂ , 2 : ÇÁ·ÎÆ÷Áî ¿Ï·á »óÅÂ , 4 : °áÈ¥»óÅÂ ) [10/2/2008 AJJIYA]
		m_iLoveState		= uiValue;
		bVal |= true;
		break;
	case STATUS_EXTENDED_CHARSLOT:			// Ä³¸¯ÅÍ È®Àå ½½·Ô
		m_iExpandCharSlot		= uiValue;
		bVal |= true;
		break;
	case STATUS_MOB_COUNT:					// ¸ó½ºÅÍ ¼ÒÈ¯ °³Ã¼¼ö
		m_iSummonMobCount	= uiValue ;
		//ErrorFLogs->Error_Log("SetPlayerStatus MobCount[%d]", m_iSummonMobCount) ;
		bVal |= true ;
		break ;
	case STATUS_RANKING_LEVEL:
		m_iRankingLevel		= uiValue;
		break ;
	case STATUS_RANKING_PVP:
		m_iRankingPvp		= uiValue;
		break ;
	case STATUS_RANKING_LOVE:
		m_iRankingLove		= uiValue;
		break ;
	case STATUS_RANKING_GUILD:
		m_iRankingGuild		= uiValue;
		break ;
	}

	bVal |= SPGOBStatus::SetStatusValue(StatusType, uiValue);

	if(bVal && m_pParent->IsLocalPlayer())
		g_pInterfaceManager->SPPerformMessage(SPIM_REFRESH_PLAYER_STAT);

	return bVal;	
}

UINT64 SPPlayerStatus::GetStatusValueEX(STATUS_EX_TYPE	StatusType)
{
	switch(StatusType) {	
	case STATUS_EX_EXP:
		return m_CharExp.uiCurExp;
	case STATUS_EX_ELY:
		return m_uiMoney;
	case STATUS_EX_EXP_MAX:
		return m_CharExp.uiMaxExp;
	case STATUS_EX_EXP_MIN:
		return m_CharExp.uiMinExp;
	case STATUS_EX_STORAGE_ELY:					//[2005/7/20]
		return m_uiStorageMoney;
	case STATUS_EX_PVP_GLORY:
		return m_stPvpStat.iGlory;
	}
	return SPGOBStatus::GetStatusValueEX(StatusType);
}

bool SPPlayerStatus::SetStatusValueEX(STATUS_EX_TYPE StatusType, UINT64 uiValue)
{
	if( m_pParent->IsLocalPlayer()) {
		if(StatusType == STATUS_EX_EXP || StatusType == STATUS_EX_ELY) {
			if(g_pClientOption && g_pClientOption->GetOptionStruct()) {				
				char szMsg[256];
				ZeroMemory(szMsg, 256);
				INT64 iTempExp = 0;
				INT64 iTempEly = 0;
				D3DXCOLOR color( 0.62745f , 1.0f, 0.0f, 1.0f);

				if(StatusType == STATUS_EX_EXP && g_pClientOption->GetOptionStruct()->bExp) {
					iTempExp = uiValue - m_CharExp.uiCurExp;
					if(iTempExp > 0 && g_pResourceManager->GetGlobalString(5017008)) {						
						//sprintf(szMsg, g_pResourceManager->GetGlobalString(5017008), iTempExp);
						sprintf(szMsg, g_pResourceManager->GetGlobalString(5017008), ConvertMoneyToString(iTempExp));
//						g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMsg);
						g_pInterfaceManager->SPChildWndSendMessage( WIID_SYSTEM_MESSAGE , SPIM_SYSTEM_MESSAGE_ADD , (WPARAM)szMsg , (LPARAM)&color );
					}					
				}

				if(StatusType == STATUS_EX_ELY && g_pClientOption->GetOptionStruct()->bItem) {
					iTempEly = uiValue - m_uiMoney;
					if(iTempEly > 0 && g_pResourceManager->GetGlobalString(5017010)) {
						//sprintf(szMsg, g_pResourceManager->GetGlobalString(5017010), iTempEly);
						sprintf(szMsg, g_pResourceManager->GetGlobalString(5017010), ConvertMoneyToString(iTempEly));
//						g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMsg);
						g_pInterfaceManager->SPChildWndSendMessage( WIID_SYSTEM_MESSAGE , SPIM_SYSTEM_MESSAGE_ADD , (WPARAM)szMsg , (LPARAM)&color );
					}					
				}
			}
		}
	}

	bool bVal = false;
	
	switch(StatusType) {	
	case STATUS_EX_EXP:		
		m_CharExp.uiCurExp = uiValue;
		bVal |= true;
		break;
	case STATUS_EX_ELY:
		m_uiMoney = uiValue;		
		bVal |= true;
		break;
	case STATUS_EX_EXP_MAX:
		m_CharExp.uiMaxExp  = uiValue;
		bVal |= true;
		break;
	case STATUS_EX_EXP_MIN:
		m_CharExp.uiMinExp  = uiValue;
		bVal |= true;
		break;
	case STATUS_EX_STORAGE_ELY:					//[2005/7/20]
		m_uiStorageMoney = uiValue;
		if( m_pParent->IsLocalPlayer())
			g_pInterfaceManager->GetStorageWindow()->SPSendMessage(SPIM_STORAGE_REFRESH);
		bVal |= true;
		break;
	case STATUS_EX_PVP_GLORY:
		m_stPvpStat.iGlory = uiValue;
		bVal |= true;
		break;
	}
	bVal |= SPGOBStatus::SetStatusValueEX(StatusType, uiValue);

	if(bVal && m_pParent->IsLocalPlayer())
		g_pInterfaceManager->SPPerformMessage(SPIM_REFRESH_PLAYER_STAT);

	return bVal;
}

bool SPPlayerStatus::SetStatusInfo(STATUS_INFO* pStatusInfo)
{
	CopyMemory(&m_CharExp, &(pStatusInfo->ExpInfo), sizeof(CHAR_EXP));
	CopyMemory(&m_CharSts, &(pStatusInfo->StsInfo), sizeof(CHAR_STS));
	//m_iRecoveryHP = pStatusInfo->iCurrRHP;
	//m_iRecoverySP = pStatusInfo->iCurrRSP;
	m_iSkillPoint = pStatusInfo->iSkillFreePoint;
	m_uiArmor = pStatusInfo->iAmor ;
	m_uiRegistEarth = pStatusInfo->iResEarth ;
	m_uiRegistWind = pStatusInfo->iResWind ;
	m_uiRegistWater = pStatusInfo->iResWater ;
	m_uiRegistFire = pStatusInfo->iResFire ;
	m_uiRequireLv = pStatusInfo->iReqLv ;
	m_uiProvEnchant = pStatusInfo->iProbEnchat ;

	if( m_pParent->IsLocalPlayer())
		g_pInterfaceManager->SPPerformMessage(SPIM_REFRESH_PLAYER_STAT);
	return true;
}

bool SPPlayerStatus::SetCharInfo(CHARACTER_INFO2* pInfo)
{
	if( pInfo == NULL )
		return false;

	m_strName		= pInfo->BaseInfo.szName;		
	m_uiTitleID		= pInfo->uiTitleID;
	m_cLevel		= pInfo->BaseInfo.uiLevel;		
	m_cCharType		= pInfo->uiClassType;	
	SPPlayer* pPlayer = static_cast<SPPlayer*>(m_pParent);
	if(pPlayer)
		pPlayer->SetPlayerClass(m_cCharType);	
	m_Gender		= (GENDER)pInfo->uiSex;
	m_uiMoney		= pInfo->uiMoney;
	m_uiStorageMoney = pInfo->uiStorageMoney;

	m_uiHitPoint	= pInfo->BaseInfo.uiHP;
	m_uiSpiritPoint	= pInfo->BaseInfo.uiSP;

	m_uiMaxHPBase	= pInfo->BaseInfo.uiMHP[VALUE_BASE];
	m_uiMaxSPBase	= pInfo->BaseInfo.uiMSP[VALUE_BASE];

	m_uiMaxHitPoint		= pInfo->BaseInfo.uiMHP[VALUE_CURR];
	m_uiMaxSpiritPoint	= pInfo->BaseInfo.uiMSP[VALUE_CURR];

	m_iRecoveryHP = pInfo->BaseInfo.uiRHP;
	m_iRecoverySP = pInfo->BaseInfo.uiRSP;


	m_uiFameBase = pInfo->Fame[VALUE_BASE];
	m_uiFame = pInfo->Fame[VALUE_CURR];	
	
	m_iGameOptions = pInfo->iGameOptions;
	UpdateOption();

	SetWantParty( g_pkPvpManager->IsPvpZone() );

	int iLength = (int)strlen(pInfo->BaseInfo.szName);
	if( iLength >= LEN_NAME )
	{
		char szBuf[32];
		sprintf(szBuf, "%s", pInfo->BaseInfo.szName);
		szBuf[LEN_NAME] = '\0';
		m_strName = szBuf;
	}

	if( m_pParent->IsLocalPlayer())
		g_pInterfaceManager->SPPerformMessage(SPIM_REFRESH_PLAYER_STAT);

	// AJJIYA [4/3/2006]
	if( m_pParent != NULL )
	{
		SPPlayer*	pPlayer	=	(SPPlayer*)m_pParent;

		if( pPlayer != NULL )
		{ 
			SPTitleData	stTitleData;

			stTitleData.m_uiTitleID	=	m_uiTitleID;

			pPlayer->SetTitle( stTitleData );
		}
	}

	return true;
}

CHAR_EXP* SPPlayerStatus::GetCHAR_EXP()		
{ 
	return &m_CharExp;	
}

void SPPlayerStatus::SetTitleID( UINT32 uiTitleID )
{
	m_uiTitleID	=	uiTitleID;
}

UINT32 SPPlayerStatus::GetTitleID()
{
	return m_uiTitleID;
};


int	SPPlayerStatus::GetGameOption()
{
	return m_iGameOptions;
}


bool SPPlayerStatus::IsTrade()
{
	return (m_iGameOptions & SSO_REQ_TRADE)? true : false;
}


bool SPPlayerStatus::IsTeam()
{
#ifdef _DEBUG
	bool bTeam = false;
	(m_iGameOptions & SSO_REQ_TEAM)? bTeam = true : bTeam = false;
	return bTeam;
#else
	return (m_iGameOptions & SSO_REQ_TEAM)? true : false;
#endif
}


bool SPPlayerStatus::IsAlert()
{
#ifdef _DEBUG
	bool bAlert = false;
	(m_iGameOptions & SSO_REQ_ALERT)? bAlert = true : bAlert = false;
	return bAlert;
#else
	return (m_iGameOptions & SSO_REQ_ALERT)? true : false;
#endif
}


bool SPPlayerStatus::IsWantTeamHide()
{
#ifdef _DEBUG
	bool bWantTeamHide = false;
	(m_iGameOptions & SSO_REQ_WANTTEAM)? bWantTeamHide = true : bWantTeamHide = false;
	return bWantTeamHide;
#else
	return (m_iGameOptions & SSO_REQ_WANTTEAM)? true : false;
#endif
}

void SPPlayerStatus::SetItemMix( ITEMMIX_INFO& stItemMixInfo , bool bShowChatMessage )
{
	if( m_stItemMix.iCurExp >= 0 && bShowChatMessage == true )
	{
		char	szBuf[ MAX_PATH ]	=	{};
		const char*	pszGlobalString	=	g_pResourceManager->GetGlobalString( 16010021 );

		if( pszGlobalString != NULL )
		{
			int	iAddExp	=	stItemMixInfo.iCurExp - m_stItemMix.iCurExp;

			if( iAddExp > 0 )
			{
				sprintf( szBuf , pszGlobalString , iAddExp );
				D3DXCOLOR color(1.0f, 0.0f, 0.0f, 1.0f);
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szBuf , (LPARAM)&color );
			}
		}
	}

	memcpy( &m_stItemMix, &stItemMixInfo, sizeof(ITEMMIX_INFO) );
}

ITEMMIX_INFO* SPPlayerStatus::GetItemMix()
{
	return &m_stItemMix;
}

void SPPlayerStatus::SetLovePoint( int iLovePoint , bool bShowMsg )
{
	if( bShowMsg == true )
	{
		if( m_pParent != NULL && g_pInterfaceManager != NULL && g_pResourceManager != NULL )
		{
			if( m_pParent->IsLocalPlayer() == true )
			{
				int iUpdateValue				=	iLovePoint	-	m_iLovePoint;
				const char* pstrGlobalString	=	g_pResourceManager->GetGlobalString( 5208135 );

				if( iUpdateValue > 0 && pstrGlobalString != NULL )
				{
					char szMsg[256];
					ZeroMemory( szMsg , 256 );
					//sprintf( szMsg , pstrGlobalString , ConvertMoneyToString( iUpdateValue ) );
					sprintf( szMsg , pstrGlobalString , iUpdateValue );
					g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMsg );
				}
			}
		}
	}

	m_iLovePoint	=	iLovePoint;
}

bool SPPlayerStatus::SetWantParty(bool bPvp /*= false*/)
{
	if(m_pParent == NULL)
		return false;

	//ErrorFLogs->Error_Log("¡ÚSetWantParty[%d] - Current [%d]", bPvp, IsWantTeamHide()) ;
	
	bool bShowWantTeam = false;

	if(bPvp) {
		bShowWantTeam = true;
	}
	else { 
		(IsWantTeamHide())? bShowWantTeam = false : bShowWantTeam = true;
	}	

	//ErrorFLogs->Error_Log("  Change[%d] - Current[%d] Change[%d]", bPvp, IsWantTeamHide(), bShowWantTeam) ;

	SPPlayer* pPlayer = static_cast<SPPlayer*>(m_pParent);
	pPlayer->SetWantTeam(bShowWantTeam, m_cCharType, m_cLevel);

	return bShowWantTeam;
}



//////////////////////////////////////////////////////////////////////////
// Message Map
SPGMESSAGE_OBJ_MAP_BEGIN( SPPlayerStatus ) 	
	SPGMESSAGE_OBJ_COMMAND(  SPGM_STAT_SC_LEVEL_UP			,	OnStatLevelUp		)	
	SPGMESSAGE_OBJ_COMMAND(  SPGM_SET_SC_STATUSINFO			,	OnSetStatusInfo 	)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_SET_SC_EFFECT_STATUS		,	OnSetEffectStatusInfo 	)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_SET_SC_CHARINFO			,	OnSetCharInfo		)	
	SPGMESSAGE_OBJ_COMMAND(	 SPGM_STORAGE_SC_IN_MONENY		,	OnStorageInMoney	)
	SPGMESSAGE_OBJ_COMMAND(	 SPGM_STORAGE_SC_OUT_MONENY		,	OnStorageOutMoney	)
	SPGMESSAGE_OBJ_COMMAND(	 SPGM_STAT_SC_SETOPTION			,	OnSetOptions		)
	SPGMESSAGE_OBJ_COMMAND(	 SPGM_SET_SC_CASHINFO			,	OnSetCashInfo		)
SPGMESSAGE_OBJ_MAP_END (SPGOBStatus)


//////////////////////////////////////////////////////////////////////////
// Message Map Function
int SPPlayerStatus::OnStatLevelUp	( LPARAM lParam )
{
	CPacket* pPacket = (CPacket*)lParam;
	UINT8			numOfStatus;
	STATUS_CHANGE	statusChanges;

	STATUS_TYPE		eStatusType;
	int				iStatusOldValue		=	0;
	const char*		szGlobalString		=	NULL;
	char			szBuf[ MAX_PATH ]	=	{};
	std::string		strLevelUpMessage;

	pPacket->ExtractUINT64(&(m_CharExp.uiMinExp));
	pPacket->ExtractUINT64(&(m_CharExp.uiMaxExp));

	pPacket->ExtractUINT8(&numOfStatus);

	strLevelUpMessage.clear();

	for(int i = 0; i < numOfStatus; ++i ) {
		pPacket->ExtractStruct(&statusChanges, sizeof(STATUS_CHANGE) );

		eStatusType	=	(STATUS_TYPE)statusChanges.eType;

		if( statusChanges.eType >= STATUS_LUK_BASE && statusChanges.eType <= STATUS_ELE_BASE )
		{
			iStatusOldValue = GetStatusValue( eStatusType );
			iStatusOldValue = statusChanges.uiValue - iStatusOldValue;

			if( iStatusOldValue > 0 )
			{
				szGlobalString = g_pResourceManager->GetGlobalString( 4005001 + statusChanges.eType - STATUS_LUK_BASE );

				if( szGlobalString != NULL )
				{
					sprintf( szBuf , szGlobalString , iStatusOldValue );
					strLevelUpMessage	+=	szBuf;
				}
			}
		}
		SetStatusValue(eStatusType, statusChanges.uiValue);
		//½ÇÉ«Éý¼¶
		if ( g_pCIGWLoaderDx9 )
		{
			g_pCIGWLoaderDx9->OnLevelUp( statusChanges.uiValue );
		}
	}

	if( strLevelUpMessage.empty() == false )
	{
		D3DXCOLOR	stColor	=	g_pResourceManager->GetGlobalColor( 8 );
		szGlobalString = g_pResourceManager->GetGlobalString( 4005005 );

		if( szGlobalString != NULL )
		{
			strLevelUpMessage	+=	szGlobalString;
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strLevelUpMessage.c_str() , (LPARAM)&stColor );
		}
	}

#ifndef _CLASS_CHANGE_VER
	GU_ID guid = m_pParent->GetGUID();
	g_pGOBManager->SPPerformMessage(SPGM_SET_LEVEL_UP_FX, (WPARAM)&guid);	
#else
	SPLevelUpEffectAttr* pLevelUpEffect = g_pEventManager->GetClassChangeManager()->GetLevelUpEffectAttr(m_cLevel, m_cCharType);
	if(pLevelUpEffect) {
		SPPlayer* pPlayer = static_cast<SPPlayer*>(m_pParent);
		if(pPlayer) {
			if(pLevelUpEffect->m_iFx) {
				pPlayer->SPGOBPerformMessage(SPGM_SET_FX_GROUP, pLevelUpEffect->m_iFx);
			}

			if(pLevelUpEffect->m_iSoundEffect) {
				pPlayer->SetActionSound(pLevelUpEffect->m_iSoundEffect, true);
			}

			if(pPlayer->IsLocalPlayer()) {
				if(!pLevelUpEffect->m_strMsg.empty()) {
					float fRed = float(pLevelUpEffect->m_iRed) / 255.0f;
					float fGreen  = float(pLevelUpEffect->m_iGreen) / 255.0f;
					float fBlue  = float(pLevelUpEffect->m_iBlue) / 255.0f;
					D3DXCOLOR color(fRed, fGreen, fBlue, 1.0f);
					g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING, (WPARAM)pLevelUpEffect->m_strMsg.c_str(), (LPARAM)&color);
				}

				if(pLevelUpEffect->m_iGuideBook) {
					//GuideBook Ã³¸®
					SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_GUIDE_BOOK);
					if(pWindow){
						pWindow->SPSendMessage(SPIM_GUIDE_BOOK_VIEW, (WPARAM)pLevelUpEffect->m_iGuideBook);
					}
				}
			}
		}		
	}
#endif

//	SetStatusValue((STATUS_TYPE)statusChanges.eType, statusChanges.uiValue); ¿¡¼­ SPIM_REFRESH_PLAYER_STAT¸¦ ³¯·ÁÁØ´Ù.	 AJJIYA [8/1/2005]
//	if( m_pParent->IsLocalPlayer())
//		g_pInterfaceManager->SPPerformMessage(SPIM_REFRESH_PLAYER_STAT);

	//[2005/9/21] ½ºÅÈ º¯°æ½Ã¸¶´Ù »ç¿ë°¡´É ¾ÆÀÌÅÛ ¼³Á¤ º¯°æÀ» À§ÇÔ...
	if( m_pParent->IsLocalPlayer()) {	
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_ITEM_REFRESH_GRID);
		g_pGOBManager->SPPerformMessage(SPGM_REFRESH_NAME_COLOR);
		g_pInterfaceManager->SPChildWndSendMessage( WIID_GREENERGUIDE, SPIM_ITEM_LEVEL, (WPARAM)NULL, (LPARAM)NULL );
	}

	//[2007/2/27]
	SetWantParty( g_pkPvpManager->IsPvpZone() );

	return 0;
}


int SPPlayerStatus::OnSetStatusInfo ( LPARAM lParam )
{
	STATUS_INFO* pStatusInfo = (STATUS_INFO*)lParam;
	SetStatusInfo(pStatusInfo);

//	SetStatusInfo(pStatusInfo); ¿¡¼­ SPIM_REFRESH_PLAYER_STAT¸¦ ³¯·ÁÁØ´Ù.	 AJJIYA [8/1/2005]
//	if( m_pParent->IsLocalPlayer())
//		g_pInterfaceManager->SPPerformMessage(SPIM_REFRESH_PLAYER_STAT);
	return 0;
}


int SPPlayerStatus::OnSetEffectStatusInfo(LPARAM lParam)
{
	EFFECT_STATUS_INFO* pEffectStatus = (EFFECT_STATUS_INFO*)lParam;

	m_cLevel = pEffectStatus->uiLevel;

	m_uiHitPoint	= pEffectStatus->uiHP;
	m_uiSpiritPoint	= pEffectStatus->uiSP;

	m_uiMaxHPBase	= pEffectStatus->uiMHP[VALUE_BASE];
	m_uiMaxSPBase	= pEffectStatus->uiMSP[VALUE_BASE];

	m_uiMaxHitPoint		= pEffectStatus->uiMHP[VALUE_CURR];
	m_uiMaxSpiritPoint	= pEffectStatus->uiMSP[VALUE_CURR];

	m_uiFameBase = pEffectStatus->Fame[VALUE_BASE];
	m_uiFame = pEffectStatus->Fame[VALUE_CURR];

	m_iRecoveryHP = pEffectStatus->uiRHP;
	m_iRecoverySP = pEffectStatus->uiRSP;

	SetStatusInfo(&pEffectStatus->Status);

	//if( m_pParent->IsLocalPlayer())
	//	g_pInterfaceManager->SPPerformMessage(SPIM_REFRESH_PLAYER_STAT);

	return 0;
}


int SPPlayerStatus::OnSetCharInfo	( LPARAM lParam )
{
	CHARACTER_INFO2* pCharInfo = (CHARACTER_INFO2*)lParam;	
	SetCharInfo(pCharInfo);

//	SetCharInfo(pCharInfo); ¿¡¼­ SPIM_REFRESH_PLAYER_STAT¸¦ ³¯·ÁÁØ´Ù.	 AJJIYA [8/1/2005]
//	if( m_pParent->IsLocalPlayer())
//		g_pInterfaceManager->SPPerformMessage(SPIM_REFRESH_PLAYER_STAT);
	return 0;	
}

//Storage
int SPPlayerStatus::OnStorageInMoney(LPARAM lParam)
{
	CPacket* pPacket = (CPacket*)lParam;
	UINT64 uiMoney = 0, uiStorageMoney = 0;
	pPacket->ExtractUINT64(&uiMoney);
	pPacket->ExtractUINT64(&uiStorageMoney);

	////
	//if(g_pGOBManager->GetLocalPlayer()) {
	//	g_pGOBManager->GetLocalPlayer()->GetStatus()->SetStatusValueEX(STATUS_EX_ELY, uiMoney);
	//	g_pGOBManager->GetLocalPlayer()->GetStatus()->SetStatusValueEX(STATUS_EX_STORAGE_ELY, uiStorageMoney);
	//}	
	if(m_pParent->IsLocalPlayer()) {
		this->SetStatusValueEX(STATUS_EX_ELY, uiMoney);
		this->SetStatusValueEX(STATUS_EX_STORAGE_ELY, uiStorageMoney);
	}
	
	return 0;
}

int SPPlayerStatus::OnStorageOutMoney(LPARAM lParam)
{
	CPacket* pPacket = (CPacket*)lParam;
	UINT64 uiMoney = 0, uiStorageMoney = 0;
	pPacket->ExtractUINT64(&uiMoney);
	pPacket->ExtractUINT64(&uiStorageMoney);

	//if(g_pGOBManager->GetLocalPlayer()) {
	//	g_pGOBManager->GetLocalPlayer()->GetStatus()->SetStatusValueEX(STATUS_EX_ELY, uiMoney);
	//	g_pGOBManager->GetLocalPlayer()->GetStatus()->SetStatusValueEX(STATUS_EX_STORAGE_ELY, uiStorageMoney);
	//}
	if(m_pParent->IsLocalPlayer()) {
		this->SetStatusValueEX(STATUS_EX_ELY, uiMoney);
		this->SetStatusValueEX(STATUS_EX_STORAGE_ELY, uiStorageMoney);
	}

	return 0;
}


int SPPlayerStatus::OnSetOptions(LPARAM lParam)
{
	m_iGameOptions = (INT32)lParam;
	UpdateOption();

	if( !g_pkPvpManager->IsPvpZone() )
		SetWantParty();

	return 0;
}


int SPPlayerStatus::OnSetCashInfo(LPARAM lParam)
{
	CHARACTER_CASH_INFO* pCashInfo = (CHARACTER_CASH_INFO*)lParam;
	m_iCash = pCashInfo->iCash;
	m_iPoint = pCashInfo->iMile;
	
	if(pCashInfo->uiHotCashShopMode) {
		SPCashTradeManager::GetInstance()->GetHotItemManager()->SetShowFlag(true);
	}
	else {
		SPCashTradeManager::GetInstance()->GetHotItemManager()->SetShowFlag(false);
	}
	return 0;
}

void SPPlayerStatus::UpdateOption()
{
	if( m_pParent == NULL )
		return;

	if( m_pParent->IsLocalPlayer() == false )
		return;

	if( g_pClientOption != NULL )
		g_pClientOption->SetServerSideOption( m_iGameOptions );

	SPWindow* pWindow = NULL;
	
	pWindow = g_pInterfaceManager->FindWindow( WIID_OPTION );
	if( pWindow )
		pWindow->SPSendMessage(	SPIM_REFRESH , REFRESH_SERVERSIDE );

	// ChattingRoom [4/14/2008 AJJIYA]
	if( g_pInterfaceManager != NULL )
		g_pInterfaceManager->SPChildWndSendMessage( WIID_CHATTING_ROOM , SPIM_CHATTINGROOM_OPTION , NULL , NULL );
}
