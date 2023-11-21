
#include <vector>

//
//	2005. 4. 25.	AJJIYA
//	캐릭터 툴간 호환을 위한 헤더화일 정리
//	Start
#ifdef _LATALE_CLIENT
#include "SPCommon.h"
#include "SPGameObject.h"
#include "SPManager.h"
#else 
#include "SPToolCommon.h"
#include "SPGameMessageDEF.H"
#endif
//	End

#include "SPUtil.h"
#include "SPResourceDef.h"

#include "SPGameObjectDEF.h"

#include "SPRenderModelDEF.h"
#include "SPRenderModel.h"

#include "SPFXDEF.h"
#include "SPRenderModel.h"
#include "SPFXModel.h"

#include "SPFXModelFrameAnimation.h"

SPFXModelFrameAnimation::SPFXModelFrameAnimation()
{
	Init();	
	m_BContinue = TRUE;
}

SPFXModelFrameAnimation::~SPFXModelFrameAnimation()
{
	Destroy();
}

void SPFXModelFrameAnimation::SetNull()
{
	SPFXModel::SetNull();
	m_iNoFrame = 0;
}

void SPFXModelFrameAnimation::Init()
{	
	SPFXModel::Init();	
	m_ClassID = FX_MODEL_CLASS_FRAME_ANIMATION;	
}

void SPFXModelFrameAnimation::Destroy()
{
	SPFXModel::Destroy();
	SAFE_RELEASE(m_pFrameImage);

	std::vector<SPFXFrameData*>::iterator Iter = m_vpFrameData.begin();	
	for(;Iter != m_vpFrameData.end(); ++Iter) {
		SAFE_DELETE(*Iter);
	}
	m_vpFrameData.clear();

	SetNull();
}

bool SPFXModelFrameAnimation::SetImage(SPTexture** ppImage)
{
	m_pFrameImage = *ppImage;
	return true;
}

LRESULT SPFXModelFrameAnimation::GetMemberValue(MODEL_VALUE_TYPE mvMsg, LPARAM lParam)
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
				SPFXFrameData* pFrameData = new SPFXFrameData();
				(*pFrameData) = *((SPFXFrameData*)lParam);				
				m_vpFrameData.push_back(pFrameData);
				m_iNoFrame = (int)m_vpFrameData.size();
				return TRUE;
			}
			return FALSE;
		}

	case MV_GET_FRAMEDATA:
		{
			int	iPage	=	(int)lParam;
			if(iPage >= 0 && iPage < (int)m_vpFrameData.size()) {
				return (LRESULT)(m_vpFrameData[iPage]);
			}
			return NULL;
		}
	}

	return SPFXModel::GetMemberValue(mvMsg, lParam);
}
