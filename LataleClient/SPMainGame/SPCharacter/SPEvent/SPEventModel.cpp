#include "SPCommon.h"
//#include "SPManagerDef.h"
//#include "SPManager.h"
#include "SPResourceDef.h"

#include "SPMainGameDEF.H"
//#include "SPSubGameManager.h"
//#include "SPMainGameManager.h"
//#include "SPMouseCursor.h"

#include "SPGameObjectDEF.h"
#include "SPManager.h"

#include "SPRenderModelDEF.h"
#include "SPRenderModel.h"

#include "SPEventDEF.h"
//#include "SPEventManager.h"
#include "SPEventModel.h"


SPEventModel::SPEventModel(SPID_EVENT ID)
: SPRenderModel()
, m_ID(ID)
, m_fLifeTime(0.0f)
//, m_iParentFollow(FXFOLLOW_PARENT)
{
	Init();
}

SPEventModel::~SPEventModel()
{
}

void SPEventModel::Init()
{	
	SetNull();	
	m_BLightMap = TRUE;
}

void SPEventModel::SetNull()
{	
	SPRenderModel::SetNull();

}

void SPEventModel::Destroy()
{

}

SPID_EVENT SPEventModel::GetClassID() 
{ 
	return m_ID; 
}


LRESULT SPEventModel::GetMemberValue(MODEL_VALUE_TYPE mvMsg, LPARAM lParam)
{
	switch(mvMsg) {
	////
	//case MV_GET_FXLIFETIME:
	//	return (LRESULT)&m_fLifeTime;
	//case MV_SET_FXLIFETIME:
	//	m_fLifeTime = *(float*)lParam;
	//	break;
	//case MV_GET_FXFOLLOW_PARENT:
	//	return (LRESULT)m_iParentFollow;
	//case MV_SET_FXFOLLOW_PARENT:
	//	m_iParentFollow = lParam;
	//	break;
	}

	return SPRenderModel::GetMemberValue(mvMsg, lParam);
}






