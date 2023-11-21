
//
//	2005. 4. 25.	AJJIYA
//	캐릭터 툴간 호환을 위한 헤더화일 정리
//	Start
#ifdef _CHARTOOL
#include "SPToolCommon.h"
#include "SPGameMessageDEF.H"
#else
#include "SPCommon.h"
#endif
//	End

#include "SPResourceDef.h"
#include "SPRenderModelDEF.h"
#include "SPRenderModel.h"

SPRenderModel::SPRenderModel()
: m_ClassID(MODEL_CLASS_NULL)
{
	Init();
}

SPRenderModel::~SPRenderModel()
{
}

void SPRenderModel::Init()
{
	SetNull();
}

void SPRenderModel::SetNull()
{	
	m_iInstanceID		= 0;	
	m_fNextAnimation	= 0.f;
	m_BLightMap			= FALSE;
}

void SPRenderModel::Destroy()
{

}

LRESULT SPRenderModel::GetMemberValue(MODEL_VALUE_TYPE mvMsg, LPARAM lParam)
{	
	switch(mvMsg) {
	case MV_CLASSID:
		return m_ClassID;

	case MV_INSTANCEID:
		//return m_iInstanceID;
		return (INT64)&m_iInstanceID;

	case MV_NEXTANIMATION:
		return (LRESULT)&m_fNextAnimation;

	case MV_SET_NEXTANIMATION:			//jinhee - 04.11.15
		m_fNextAnimation = *(float*)lParam;
		return 0;

	case MV_GET_LIGHTMAP:				//jinhee - 04.11.04
		return m_BLightMap;
	
	case MV_SET_LIGHTMAP:				//jinhee - 04.11.04
		m_BLightMap = (BOOL)lParam;
		return 0;
	}	
	return -1;
}



