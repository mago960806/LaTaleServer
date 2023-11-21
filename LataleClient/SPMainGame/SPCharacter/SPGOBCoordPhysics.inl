#include "SPStage.h"
#include "SPStageManager.h"

//------------------------------------------------------------------------------------
inline UINT64 SPGOBCoordPhysics::GetCurAction()
{
	return m_uiCurAction;
}

//------------------------------------------------------------------------------------
inline float SPGOBCoordPhysics::GetPosX()
{
	return m_fPosX;
}

//------------------------------------------------------------------------------------
inline float SPGOBCoordPhysics::GetPosY()
{
	return m_fPosY;
}

//------------------------------------------------------------------------------------
inline void SPGOBCoordPhysics::SetPosX(float fPosX)
{
	if( m_fPosX != fPosX )
		m_bTerrainAttributeChange = true;

	m_fPosX = fPosX;
}

//------------------------------------------------------------------------------------
inline void SPGOBCoordPhysics::SetPosY(float fPosY)
{
	if( m_fPosY != fPosY )
		m_bTerrainAttributeChange = true;

	m_fPosY = fPosY;
}

//------------------------------------------------------------------------------------
inline void SPGOBCoordPhysics::SetPos(float fPosX, float fPosY)
{
	if( m_fPosX != fPosX || m_fPosY != fPosY )
		m_bTerrainAttributeChange = true;

	m_fPosX = fPosX;
	m_fPosY = fPosY;
}

//------------------------------------------------------------------------------------
inline float SPGOBCoordPhysics::GetMaxVelocityX()
{
	return m_fMaxVelocityX;
}

//------------------------------------------------------------------------------------
inline float SPGOBCoordPhysics::GetMaxVelocityY()
{
	return m_fMaxVelocityY;
}

//------------------------------------------------------------------------------------
inline float SPGOBCoordPhysics::GetCurVelocityX()
{
	return m_fCurVelocityX;
}

//------------------------------------------------------------------------------------
inline float SPGOBCoordPhysics::GetCurVelocityY()
{
	return m_fCurVelocityY;
}

//------------------------------------------------------------------------------------
inline float SPGOBCoordPhysics::GetAccelX()
{
	return m_fAccelX;
}

//------------------------------------------------------------------------------------
inline float SPGOBCoordPhysics::GetAccelY()
{
	return m_fAccelY;
}

//------------------------------------------------------------------------------------
inline float SPGOBCoordPhysics::GetJumpSpeed()
{
	return m_fJumpSpeed;
}

//------------------------------------------------------------------------------------
inline float SPGOBCoordPhysics::GetSimulateJumpLength()
{
	return m_fSimulateJumpLength;
}

//------------------------------------------------------------------------------------
inline float SPGOBCoordPhysics::GetGravity()
{
	return m_fGravity;
}

//------------------------------------------------------------------------------------
inline float SPGOBCoordPhysics::GetMaxDropVelocity()
{
	return m_fMaxDropVelocity;
}

//------------------------------------------------------------------------------------
inline char SPGOBCoordPhysics::GetMoveVerType()
{
	return m_cMoveVerType;
}

//------------------------------------------------------------------------------------
inline void SPGOBCoordPhysics::SetMoveVerType(char cType)
{
	m_cMoveVerType = cType;
}

//------------------------------------------------------------------------------------
inline GOB_STATE_TYPE SPGOBCoordPhysics::GetCurState()
{
	return m_eCurState;
}

//------------------------------------------------------------------------------------
inline void SPGOBCoordPhysics::SetMaxVelocityX(float fVelocityX)
{
	m_fDefVelocityX = fVelocityX;
	
	if(m_pkCurStage && m_pkCurMapGroup){
		m_fMaxVelocityX = m_fDefVelocityX * m_pkCurMapGroup->m_fVelocityX;
	}
	else{
		m_fMaxVelocityX = m_fDefVelocityX;
	}	
	
	if( m_eCurState != GOB_STATE_HANGING )
	{
		if( m_fCurVelocityX < 0.0f )
			m_fCurVelocityX = -m_fMaxVelocityX;
		else if( m_fCurVelocityX > 0.0f )
			m_fCurVelocityX = m_fMaxVelocityX;
	}
}

//------------------------------------------------------------------------------------
inline void SPGOBCoordPhysics::SetMaxVelocityY(float fVelocityY)
{
	m_fMaxVelocityY = fVelocityY;
}

//------------------------------------------------------------------------------------
inline void SPGOBCoordPhysics::SetMaxVelocity(float fVelocityX, float fVelocityY)
{
	m_fMaxVelocityX = fVelocityX;
	m_fMaxVelocityY = fVelocityY;
}

//------------------------------------------------------------------------------------
inline void SPGOBCoordPhysics::SetMaxUpDownVelocity(float fVelocity)
{
	m_fDefUpDownVelocity = fVelocity;
	
	if(m_pkCurStage && m_pkCurMapGroup){
		m_fMaxUpDownVelocity = m_fDefUpDownVelocity * m_pkCurMapGroup->m_fUpDownVelocity;
	}
	else {
		m_fMaxUpDownVelocity = m_fDefUpDownVelocity;
	}	
	
	if( m_eCurState == GOB_STATE_UP )
		m_fCurVelocityY = -m_fMaxUpDownVelocity;
	else if( m_eCurState == GOB_STATE_DOWN )
		m_fCurVelocityY = m_fMaxUpDownVelocity;
}

//------------------------------------------------------------------------------------
inline void SPGOBCoordPhysics::SetMaxHangingVelocity(float fVelocity)
{
	m_fDefHangingVelocity = fVelocity;
	
	if(m_pkCurStage && m_pkCurMapGroup){
		m_fMaxHangingVelocity = m_fDefHangingVelocity * m_pkCurMapGroup->m_fHangingVelocity;
	}
	else {
		m_fMaxHangingVelocity = m_fDefHangingVelocity;
	}	
	
	if( m_eCurState == GOB_STATE_HANGING )
	{
		if( m_fCurVelocityX < 0.0f )
			m_fCurVelocityX = -m_fMaxHangingVelocity;
		else if( m_fCurVelocityX > 0.0f )
			m_fCurVelocityX = m_fMaxHangingVelocity;
	}
}

//------------------------------------------------------------------------------------
inline float SPGOBCoordPhysics::GetMaxUpDownVelocity(void)
{
	return m_fMaxUpDownVelocity;
}

//------------------------------------------------------------------------------------
inline float SPGOBCoordPhysics::GetMaxHangingVelocity(void)
{
	return m_fMaxHangingVelocity;
}

//------------------------------------------------------------------------------------
inline void SPGOBCoordPhysics::SetCurVelocityX(float fVelocityX)
{
	m_fCurVelocityX = fVelocityX;
}

//------------------------------------------------------------------------------------
inline void SPGOBCoordPhysics::SetCurVelocityY(float fVelocityY)
{
	m_fCurVelocityY = fVelocityY;
}

//------------------------------------------------------------------------------------
inline void SPGOBCoordPhysics::SetCurVelocity(float fVelocityX, float fVelocityY)
{
	m_fCurVelocityX = fVelocityX;
	m_fCurVelocityY = fVelocityY;
}

//------------------------------------------------------------------------------------
inline void SPGOBCoordPhysics::SetAccelX(float fAccelX)
{
	m_fAccelX = fAccelX;
}

//------------------------------------------------------------------------------------
inline void SPGOBCoordPhysics::SetAccelY(float fAccelY)
{
	m_fAccelY = fAccelY;
}

//------------------------------------------------------------------------------------
inline void SPGOBCoordPhysics::SetAccel(float fAccelX, float fAccelY)
{
	m_fAccelX = fAccelX;
	m_fAccelY = fAccelY;
}

//------------------------------------------------------------------------------------
inline void SPGOBCoordPhysics::SetJumpSpeed(float fJumpSpeed)
{	
	//if( fabs(m_fJumpSpeed - fJumpSpeed) < 0.001f )
	//	return;

	//int iIndex = -1;
	//if( fabs(fJumpSpeed + 800.0f) < 0.001f )
	//	iIndex = 0;
	//else if( fabs(fJumpSpeed + 100.0f) < 0.001f )
	//	iIndex = 1;

	//if( fJumpSpeed < -3000.0f )
	//	fJumpSpeed = -3000.0f;

	//m_fJumpSpeed = fJumpSpeed;
	m_fDefJumpSpeed = fJumpSpeed;
	if(m_pkCurStage && m_pkCurMapGroup){
		m_fJumpSpeed = m_fDefJumpSpeed * m_pkCurMapGroup->m_fJumpSpeed;
	}
	else{
		m_fJumpSpeed = m_fDefJumpSpeed;
	}

	int iIndex = -1;
	if( fabs(m_fJumpSpeed + 800.0f) < 0.001f )
		iIndex = 0;
	else if( fabs(m_fJumpSpeed + 100.0f) < 0.001f )
		iIndex = 1;
	
	if( m_fJumpSpeed < -3000.0f )
		m_fJumpSpeed = -3000.0f;

	if( iIndex >= 0 )
	{
		m_fSimulateJumpLength = g_fSimulateJumpLength[iIndex];		
		m_fMaxJumpPosTime = g_fMaxJumpPosTime[iIndex];
		return;
	}

	// simulate
	float fVelocityY, fElapsedTime;
	
	m_fSimulateJumpLength = 0.0f;
	m_fMaxJumpPosTime = 0.0f;
	fElapsedTime = 0.016f;
	fVelocityY = m_fJumpSpeed;

	if( m_fGravity == 0.0f )
		return;

	while( 1 )
	{
		fVelocityY += m_fGravity * fElapsedTime;
		m_fSimulateJumpLength += fVelocityY * fElapsedTime;
		m_fMaxJumpPosTime += fElapsedTime;
		if( fVelocityY >= 0.0f )
			break;
	}
}

//------------------------------------------------------------------------------------
inline void SPGOBCoordPhysics::SetGravity(float fGravity)
{
	m_fDefGravity = fGravity;

	if(m_pkCurStage && m_pkCurMapGroup){
		m_fGravity = m_fDefGravity * m_pkCurMapGroup->m_fGravity;
	}
	else {
		m_fGravity = m_fDefGravity;
	}	
}

//------------------------------------------------------------------------------------
inline void SPGOBCoordPhysics::SetMaxDropVelocity(float fVelocity)
{
	m_fDefDropVelocity = fVelocity;

	if(m_pkCurStage && m_pkCurMapGroup){
		m_fMaxDropVelocity = m_fDefDropVelocity * m_pkCurMapGroup->m_fMaxDropSpeed;
	}
	else {
		m_fMaxDropVelocity = m_fDefDropVelocity;
	}	
}

//------------------------------------------------------------------------------------
inline void SPGOBCoordPhysics::SetDirection(const FACING eFace)
{
	m_eFacing = eFace;
}

//------------------------------------------------------------------------------------
inline FACING SPGOBCoordPhysics::GetDirection()
{
	return m_eFacing;
}

//------------------------------------------------------------------------------------
inline void SPGOBCoordPhysics::SetStandLayer(const BG_ID eLayer)
{
	m_eStandLayer = eLayer;
}

//------------------------------------------------------------------------------------
inline BG_ID SPGOBCoordPhysics::GetStandLayer()
{
	return m_eStandLayer;
}

//------------------------------------------------------------------------------------
inline void SPGOBCoordPhysics::StateChange(GOB_STATE_TYPE eType)
{
	m_eCurState = eType;
}

//------------------------------------------------------------------------------------
inline void SPGOBCoordPhysics::SetActionTime(float fTime)
{
	m_fActionTime = fTime;
}

//------------------------------------------------------------------------------------
inline float SPGOBCoordPhysics::GetActionTime()
{
	return m_fActionTime;
}

//------------------------------------------------------------------------------------
inline int SPGOBCoordPhysics::GetCurStageID()
{
	return m_iCurStageID;
}

//------------------------------------------------------------------------------------
inline int SPGOBCoordPhysics::GetCurMapGroupID()
{
	return m_iCurMapGroupID;
}

//------------------------------------------------------------------------------------
inline void SPGOBCoordPhysics::SetKnockBack(KNOCKBACK_INFO& Info)
{
	m_KnockBackInfo.bKnockBack = Info.bKnockBack;
	m_KnockBackInfo.bDelay = Info.bDelay;
	m_KnockBackInfo.fKnockBackTime = Info.fKnockBackTime;
	m_KnockBackInfo.fDelayTime = Info.fDelayTime;
	
	m_KnockBackInfo.fKnockBackVelocityX = Info.fKnockBackVelocityX;
	m_KnockBackInfo.fKnockBackVelocityY = Info.fKnockBackVelocityY;

	//if( Info.fKnockBackVelocityY )
	//{
	//	m_fCurVelocityY = Info.fKnockBackVelocityY;
	//	m_fAccelY = m_fGravity;
	//}

	m_KnockBackInfo.fKnockBackAccmulateTime = 0.0f;
	if( m_KnockBackInfo.bKnockBack )
		ZeroMemory(&m_ActivityVelocityInfo, sizeof(ACTIVE_VEL_INFO));
}

//------------------------------------------------------------------------------------
inline KNOCKBACK_INFO* SPGOBCoordPhysics::GetKnockBack()
{
	return &m_KnockBackInfo;
}

//------------------------------------------------------------------------------------
inline void SPGOBCoordPhysics::SetActicityVelocity(ACTIVE_VEL_INFO& Info)
{
	if( !m_KnockBackInfo.bKnockBack )
	{
		m_ActivityVelocityInfo.bEnable = Info.bEnable;
		m_ActivityVelocityInfo.bDelay = Info.bDelay;
		m_ActivityVelocityInfo.fDelayTime = Info.fDelayTime;
		m_ActivityVelocityInfo.fTime = Info.fTime;
		m_ActivityVelocityInfo.fVelocityX = Info.fVelocityX;
		m_ActivityVelocityInfo.fVelocityY = Info.fVelocityY;

		m_ActivityVelocityInfo.fAccmulateTime = 0.0f;
	}
}

//------------------------------------------------------------------------------------
inline void SPGOBCoordPhysics::AddAction(UINT64& uiAction, UINT64 uiAddAction)
{
	uiAction |= uiAddAction;
}

//------------------------------------------------------------------------------------
inline void SPGOBCoordPhysics::SubAction(UINT64& uiAction, UINT64 uiSubAction)
{
	uiAction &= ~uiSubAction;
}
//------------------------------------------------------------------------------------
inline void SPGOBCoordPhysics::SetIgnoreStage(bool bIgnore)
{
	m_bIgnoreStage = bIgnore;
}