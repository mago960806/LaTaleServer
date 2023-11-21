
#include "SPCommon.h"
#include "Packet.h"

#include "SPRenderModelDEF.h"
#include "SPGameObjectDef.h"
#include "SPGameObject.h"

#include "SPGOBStatus.h"
#include "SPMonsterStatus.h"


//------------------------------------------------------------------------------------
SPMonsterStatus::SPMonsterStatus(SPGameObject* pParent)
: SPGOBStatus(pParent)
{
	Init();	
}

//------------------------------------------------------------------------------------
SPMonsterStatus::~SPMonsterStatus()
{
}

//------------------------------------------------------------------------------------
void SPMonsterStatus::Init()
{
}

bool SPMonsterStatus::SetMonsterInfo(MONSTER_INFO* pInfo)
{
	if( pInfo == NULL )
		return false;

	m_strName		= pInfo->BaseInfo.szName;			
	m_cLevel		= pInfo->BaseInfo.uiLevel;
	m_uiHitPoint	= pInfo->BaseInfo.uiHP;
	m_uiSpiritPoint	= pInfo->BaseInfo.uiSP;

	m_uiMaxHPBase	= pInfo->BaseInfo.uiMHP[VALUE_BASE];
	m_uiMaxSPBase	= pInfo->BaseInfo.uiMSP[VALUE_BASE];

	m_uiMaxHitPoint		= pInfo->BaseInfo.uiMHP[VALUE_CURR];
	m_uiMaxSpiritPoint	= pInfo->BaseInfo.uiMSP[VALUE_CURR];

	m_iRecoveryHP = pInfo->BaseInfo.uiRHP;
	m_iRecoverySP = pInfo->BaseInfo.uiRSP;


	int iLength = (int)strlen(pInfo->BaseInfo.szName);
	if( iLength >= LEN_NAME )
	{
		char szBuf[32];
		sprintf(szBuf, "%s", pInfo->BaseInfo.szName);
		szBuf[LEN_NAME] = '\0';

		m_strName = szBuf;
	}
	return true;
}


//////////////////////////////////////////////////////////////////////////
// Message Map
SPGMESSAGE_OBJ_MAP_BEGIN( SPMonsterStatus ) 
	SPGMESSAGE_OBJ_COMMAND(  SPGM_SET_SC_MONSTERINFO	,	OnSetMonsterInfo	)	
SPGMESSAGE_OBJ_MAP_END (SPGOBStatus)

//////////////////////////////////////////////////////////////////////////
// Message Map Function
int SPMonsterStatus::OnSetMonsterInfo	( LPARAM lParam )
{
	MONSTER_INFO* pCharInfo = (MONSTER_INFO*)lParam;	
	SetMonsterInfo(pCharInfo);
	return 0;	
}















