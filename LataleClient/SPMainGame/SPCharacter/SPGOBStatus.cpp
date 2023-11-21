

#include "SPCommon.h"

#include "Packet.h"
#include "SPRenderModelDEF.h"
#include "SPGameObjectDef.h"
#include "SPGameObject.h"
#include "SPGOBStatus.h"

SPGOBStatus::SPGOBStatus(SPGameObject* pParent)
: m_pParent			(pParent)
, m_cLevel			(0)
, m_uiHitPoint		(0)
, m_uiSpiritPoint	(0)
, m_uiMaxHPBase		(0)
, m_uiMaxSPBase		(0)
{
	ZeroMemory(&m_CharSts, sizeof(CHAR_STS));	
}

SPGOBStatus::~SPGOBStatus()
{
}

void SPGOBStatus::Init()
{
}

const char* SPGOBStatus::GetGOBName()
{
	return m_strName.c_str();
}

unsigned int SPGOBStatus::GetCurHP()
{
	return m_uiHitPoint;
}

unsigned int SPGOBStatus::GetMaxHP()
{
	return m_uiMaxHitPoint;
}


UINT SPGOBStatus::GetStatusValue(STATUS_TYPE StatusType)
{
	switch(StatusType) {
	case STATUS_LV:
		return m_cLevel;			

	case STATUS_LUK_BASE:			
		return m_CharSts.Sts[STAT_LUK][VALUE_BASE];

	case STATUS_ATK_BASE:
		return m_CharSts.Sts[STAT_ATK][VALUE_BASE];

	case STATUS_DEF_BASE:
		return m_CharSts.Sts[STAT_DEF][VALUE_BASE];

	case STATUS_ELE_BASE:
		return m_CharSts.Sts[STAT_ELE][VALUE_BASE];

	case STATUS_MHP_BASE:			
		return m_uiMaxHPBase;
		
	case STATUS_MSP_BASE:
		return m_uiMaxSPBase;		

	case STATUS_LUK 		:
		return m_CharSts.Sts[STAT_LUK][VALUE_CURR];

	case STATUS_ATK  		:
		return m_CharSts.Sts[STAT_ATK][VALUE_CURR];
		
	case STATUS_DEF	 		:
		return m_CharSts.Sts[STAT_DEF][VALUE_CURR];
		
	case STATUS_ELE  		:
		return m_CharSts.Sts[STAT_ELE][VALUE_CURR];
		
	case STATUS_MHP  		:
		return m_uiMaxHitPoint;
		
	case STATUS_MSP			:
		return m_uiMaxSpiritPoint;
		
	case STATUS_HP  		:
		return m_uiHitPoint;
		
	case STATUS_SP			:
		return m_uiSpiritPoint;

	case STATUS_RECOVERY_HP:
		return m_iRecoveryHP;

	case STATUS_RECOVERY_SP:
		return m_iRecoverySP;

	case STATUS_EARTH:
		return m_CharSts.Ele[EARTH];			
		
	case STATUS_WIND:
		return m_CharSts.Ele[WIND];
		
	case STATUS_WATER:
		return m_CharSts.Ele[WATER];
		
	case STATUS_FIRE:
		return m_CharSts.Ele[FIRE];
		
	case STATUS_NORMAL_ATK:			
		return m_CharSts.Atk[ATK_NORMAL];
		
	case STATUS_HEAVY_ATK:
		return m_CharSts.Atk[ATK_HEAVY]	;
		
	}
	return 0;
}



bool SPGOBStatus::SetStatusValue(STATUS_TYPE StatusType, UINT uiValue)
{
	switch(StatusType) {
	case STATUS_LV:
		m_cLevel= uiValue;		
		return true;			

	case STATUS_LUK_BASE:			
		m_CharSts.Sts[STAT_LUK][VALUE_BASE]= uiValue;		
		return true;

	case STATUS_ATK_BASE:
		m_CharSts.Sts[STAT_ATK][VALUE_BASE]= uiValue;		
		return true;

	case STATUS_DEF_BASE:
		m_CharSts.Sts[STAT_DEF][VALUE_BASE]= uiValue;		
		return true;

	case STATUS_ELE_BASE:
		m_CharSts.Sts[STAT_ELE][VALUE_BASE]= uiValue;		
		return true;

	case STATUS_MHP_BASE:			
		m_uiMaxHPBase = uiValue;		
		return true;

	case STATUS_MSP_BASE:
		m_uiMaxSPBase = uiValue;		
		return true;

	case STATUS_LUK 		:
		m_CharSts.Sts[STAT_LUK][VALUE_CURR]= uiValue;		
		return true;

	case STATUS_ATK  		:
		m_CharSts.Sts[STAT_ATK][VALUE_CURR]= uiValue;		
		return true;

	case STATUS_DEF	 		:
		m_CharSts.Sts[STAT_DEF][VALUE_CURR]= uiValue;		
		return true;

	case STATUS_ELE  		:
		m_CharSts.Sts[STAT_ELE][VALUE_CURR]= uiValue;		
		return true;

	case STATUS_MHP  		:
		m_uiMaxHitPoint= uiValue;		
		return true;

	case STATUS_MSP			:
		m_uiMaxSpiritPoint= uiValue;		
		return true;

	case STATUS_HP  		:
		m_uiHitPoint= uiValue;		
		if( m_uiHitPoint > m_uiMaxHitPoint )
			m_uiHitPoint = m_uiMaxHitPoint;
		return true;

	case STATUS_SP			:
		m_uiSpiritPoint= uiValue;		
		if( m_uiSpiritPoint > m_uiMaxSpiritPoint )
			m_uiSpiritPoint = m_uiMaxSpiritPoint;
		return true;

	case STATUS_RECOVERY_HP  		:
		m_iRecoveryHP = uiValue;		
		return true;

	case STATUS_RECOVERY_SP			:
		m_iRecoverySP = uiValue;
		return true;

	case STATUS_EARTH:
		m_CharSts.Ele[EARTH]= uiValue;		
		return true;

	case STATUS_WIND:
		m_CharSts.Ele[WIND]= uiValue;		
		return true;

	case STATUS_WATER:
		m_CharSts.Ele[WATER]= uiValue;		
		return true;

	case STATUS_FIRE:
		m_CharSts.Ele[FIRE]= uiValue;		
		return true;

	case STATUS_NORMAL_ATK:			
		m_CharSts.Atk[ATK_NORMAL]= uiValue;		
		return true;

	case STATUS_HEAVY_ATK:
		m_CharSts.Atk[ATK_HEAVY]	= uiValue;		
		return true;
	}
	return false;
}


UINT64 SPGOBStatus::GetStatusValueEX(STATUS_EX_TYPE	StatusType)
{
	return 0;
}

bool SPGOBStatus::SetStatusValueEX(STATUS_EX_TYPE StatusType, UINT64 uiValue)
{
	return false;
}

int SPGOBStatus::SPPerformMessage(SPGM msg, LPARAM lparam)
{
	return SPGMESSAGE_OBJ_FUNC(msg, lparam);
}


//////////////////////////////////////////////////////////////////////////
// Message Map
SPGMESSAGE_OBJ_MAP_BEGIN( SPGOBStatus ) 	
	SPGMESSAGE_OBJ_COMMAND(  SPGM_STAT_SC_CHANGE			,	OnStatChange		)
	SPGMESSAGE_OBJ_COMMAND(  SPGM_STAT_SC_CHANGE_EX			,	OnStatChangeEX		)	
SPGMESSAGE_OBJ_MAP_END_BASE()


//////////////////////////////////////////////////////////////////////////
// Message Map Function
int SPGOBStatus::OnStatChange	( LPARAM lParam )
{
	CPacket* pPacket = (CPacket*)lParam;

	UINT8			numOfStatus;
	STATUS_CHANGE	statusChanges;

	bool			bRet;

	pPacket->ExtractUINT8(&numOfStatus);
	for(int i = 0; i < numOfStatus; ++i ) {
		pPacket->ExtractStruct(&statusChanges, sizeof(STATUS_CHANGE) );
		bRet = SetStatusValue((STATUS_TYPE)statusChanges.eType, statusChanges.uiValue);
		assert(bRet && "Invalid Stat Change Type." );
	}
	return 0;
}


int SPGOBStatus::OnStatChangeEX	( LPARAM lParam )
{
	CPacket* pPacket = (CPacket*)lParam;

	UINT8				numOfStatus;
	STATUS_CHANGE_EX	statusChanges;

	pPacket->ExtractUINT8(&numOfStatus);
	for(int i = 0; i < numOfStatus; ++i ) {
		pPacket->ExtractStruct(&statusChanges, sizeof(STATUS_CHANGE_EX) );
		SetStatusValueEX((STATUS_EX_TYPE)statusChanges.eType, statusChanges.uiValue);		
	}

	return 0;
}











