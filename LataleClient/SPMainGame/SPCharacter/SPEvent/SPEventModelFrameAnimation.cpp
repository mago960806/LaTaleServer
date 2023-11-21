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
#include "SPEventModelFrameAnimation.h"



SPEventModelFrameAnimation::SPEventModelFrameAnimation(SPID_EVENT ID)
: SPEventModel(ID)
{
	Init();	
	//m_BContinue = TRUE;
}

SPEventModelFrameAnimation::~SPEventModelFrameAnimation()
{
	Destroy();
}

void SPEventModelFrameAnimation::SetNull()
{
	SPEventModel::SetNull();
	m_iNoFrame = 0;
}

void SPEventModelFrameAnimation::Init()
{	
	SPEventModel::Init();	
	m_ClassID = FX_MODEL_CLASS_FRAME_ANIMATION;	
}

void SPEventModelFrameAnimation::Destroy()
{
	SPEventModel::Destroy();
	SAFE_RELEASE(m_pFrameImage);

	std::vector<SPEventFrameData*>::iterator Iter = m_vpFrameData.begin();	
	for(;Iter != m_vpFrameData.end(); ++Iter) {
		SAFE_DELETE(*Iter);
	}
	m_vpFrameData.clear();

	SetNull();
}

bool SPEventModelFrameAnimation::SetImage(SPTexture** ppImage)
{
	m_pFrameImage = *ppImage;
	return true;
}

LRESULT SPEventModelFrameAnimation::GetMemberValue(MODEL_VALUE_TYPE mvMsg, LPARAM lParam)
{
	switch(mvMsg) {
	case MV_GET_MAXFRAME:
		return m_iNoFrame;	

	case MV_GETINITIMAGE:		
		return (LRESULT)&m_pFrameImage;

	case MV_GETIMAGE:
		// 무조건 텍스쳐한장으로 하기로 바꿨다.			
		return (LRESULT)&m_pFrameImage;

	case MV_IS_CONTINUE:
		return m_BContinue;		

	case MV_PUSHBACK_FRAMEDATA:
		{			
			if(lParam) {			
				SPEventFrameData* pFrameData = new SPEventFrameData();
				(*pFrameData) = *((SPEventFrameData*)lParam);				
				m_vpFrameData.push_back(pFrameData);
				m_iNoFrame = m_vpFrameData.size();
				return TRUE;
			}
			return FALSE;
		}

	case MV_GET_FRAMEDATA:
		{
			if(lParam >= 0 && lParam < m_vpFrameData.size()) {
				return (LRESULT)(m_vpFrameData[lParam]);
			}
			return NULL;
		}
	}

	return SPEventModel::GetMemberValue(mvMsg, lParam);
}




