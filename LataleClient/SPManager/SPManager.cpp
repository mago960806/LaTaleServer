

#include "SPCommon.h"
#include <vector>

//#include "SPFrameSkipper.h"
#include "SPManagerDef.h"
//#include "SPMainGameManager.h"

#include "SPManager.h"



SPManager::SPManager()
: m_ClassID(MGR_ID_INVALID)
, m_pMgrCamera(NULL)
, m_bActive(false)
, m_fAccumulateProcessTime(0.0f)
, m_fAccumulateRenderTime(0.0f)
{
}

SPManager::~SPManager()
{
}

MANAGER_ID SPManager::GetMgrID() 
{ 
	return m_ClassID; 
}

void SPManager::SetMgrID(MANAGER_ID ClassID)
{
	m_ClassID = ClassID;
}

void SPManager::Process(float fTime)
{
	m_fAccumulateProcessTime += fTime;
	m_fAccumulateRenderTime += fTime;

	if(m_fAccumulateProcessTime > ACCUMULATE_TIME_LIMIT)
		m_fAccumulateProcessTime = ACCUMULATE_TIME_LIMIT;

	if(m_fAccumulateProcessTime > ACCUMULATE_TIME_LIMIT)
		m_fAccumulateProcessTime = ACCUMULATE_TIME_LIMIT;

}

void SPManager::Render(float fTime)
{
}

bool SPManager::Init()
{	
	m_fAccumulateProcessTime = 0;
	m_fAccumulateRenderTime = 0;
	m_bActive = true;
	return true;
}

void SPManager::SetMgrCamera( SPCameraManager* pMgrCamera)
{
	m_pMgrCamera = pMgrCamera;
}

void SPManager::Clean()
{
}

