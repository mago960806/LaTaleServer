
#include <string>

#include "SPEffect.h"
//------------------------------------------------------------------------------------
SPEffect::SPEffect()
{
	Init();
}

//------------------------------------------------------------------------------------
SPEffect::SPEffect(int iID, EFFECT_TYPE eType, float fApplyTime, float fApplyTimeElv)
: m_iID(iID)
, m_eType(eType)
, m_fApplyTime(fApplyTime)
, m_fApplyTimeElv(fApplyTimeElv)
{
}

//------------------------------------------------------------------------------------
SPEffect::~SPEffect()
{
}

//------------------------------------------------------------------------------------
void SPEffect::Init()
{
	m_iID = 0;
	m_eType = ET_NULL;
	m_fApplyTime = 0.0f;
	m_fApplyTimeElv = 0.0f;

	m_iTransparency = 0;
}