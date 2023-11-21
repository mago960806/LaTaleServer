
#include <vector>
#include <string>	//jinhee

#include "SPCommon.h"
#include "SPUtil.h"
#include "SPResourceDef.h"

#include "SPGameObjectDEF.h"
#include "SPManager.h"

#include "SPRenderModelDEF.h"
#include "SPRenderModel.h"

#include "SPTerrainDEF.h"
#include "SPTerrainCluster.h"
#include "SPTerrainManager.h"
#include "SPTerrainModel.h"
#include "SPTerrainModelFrameAnimation.h"


SPTerrainModelFrameAnimation::SPTerrainModelFrameAnimation()
: SPTerrainModel()
{
	Init();
}


//SPTerrainModelFrameAnimation::SPTerrainModelFrameAnimation(int iInstance, float fDelay, float fMinStart, float fMaxStart, BOOL BLightMap, int iSync)
SPTerrainModelFrameAnimation::SPTerrainModelFrameAnimation(INT64 iInstance, float fDelay, float fMinStart, float fMaxStart, BOOL BLightMap, int iSync)
: SPTerrainModel()
{
	Init();

	m_iInstanceID = iInstance;
	m_fNextAnimation = fDelay;

	m_fMinStart = fMinStart;
	m_fMaxStart = fMaxStart;
	
	m_BLightMap = BLightMap;
	m_iSyncTile = iSync;
}

SPTerrainModelFrameAnimation::~SPTerrainModelFrameAnimation()
{
	Destroy();
}

void SPTerrainModelFrameAnimation::SetNull()
{
	SPTerrainModel::SetNull();
	m_iNoFrame = 0;
}

void SPTerrainModelFrameAnimation::Init()
{
	SPTerrainModel::Init();
	SetNull();
	m_ClassID = TERRAIN_MODEL_CLASS_FRAME_ANIMATION;	
}

void SPTerrainModelFrameAnimation::Destroy()
{
	SPTerrainModel::Destroy();

	std::vector<SPTexture*>::iterator Iter = m_vpFrameImage.begin();
	for(;m_vpFrameImage.end() != Iter; ++Iter) {
		SAFE_RELEASE((*Iter));
	}

	SetNull();
}

bool SPTerrainModelFrameAnimation::SetImage(SPTexture** ppImage, int iAlpha)
{
	m_vpFrameImage.push_back(*ppImage);
	m_viFrameAlpha.push_back(iAlpha);
	m_iNoFrame = m_vpFrameImage.size();

	return true;
}

LRESULT SPTerrainModelFrameAnimation::GetMemberValue(MODEL_VALUE_TYPE mvMsg, LPARAM lParam)
{
	switch(mvMsg) {
	case MV_GET_MAXSTART:
		return (LRESULT)&m_fMaxStart;

	case MV_SET_MAXSTART:				//jinhee - 04.11.15
		m_fMaxStart = *(float*)lParam;
		return 0;

	case MV_GET_MINSTART:
		return (LRESULT)&m_fMinStart;

	case MV_SET_MINSTART:				//jinhee - 04.11.15
		m_fMinStart = *(float*)lParam;
		return 0;

	case MV_GET_MAXFRAME:
		return m_iNoFrame;	

	case MV_GETINITIMAGE:
		{
			//SPTexture* pImage = &(*(m_vpFrameImage.begin()));		
			return (LRESULT) &(*(m_vpFrameImage.begin()));
		}

	case MV_GETIMAGE:
		{
			//SPTexture* pImage = (*(m_vpFrameImage.begin() + lParam));		
			//return (LRESULT) &pImage;
			if(lParam >= m_vpFrameImage.size())
				return (LRESULT) &(*(m_vpFrameImage.begin()));
			else
				return (LRESULT) &(*(m_vpFrameImage.begin() + lParam));
		}

	case MV_GET_ALPHA:	
		return m_viFrameAlpha[lParam];

	case MV_SET_ALPHA:			//jinhee - 04.11.15
		//파라미터를 구조체 스타일로 받아야 할듯...
		//m_viFrameAlpha[lParam] = 255;		
		return 0;

	case MV_SET_FILELIST:		//jinhee - 04.11.11
		{
			std::string strFile = (char*)lParam;
			m_vstrFrameFile.push_back(strFile);
			//TRACE("Member Add [%s]\n", strFile.c_str());
		}
		return 0;

	case MV_GET_FILELIST:		//jinhee - 04.11.11
		return (LRESULT)&m_vstrFrameFile;

	case MV_SET_FRAMEGROUP:		//[2005/11/2] - 추후 그릅화에 사용한다 
		m_iSyncTile = (int)lParam;
		return 0;
	case MV_GET_FRAMEGROUP:		//[2005/11/2] - 추후 그룹화에 사용한다
		return (LRESULT)m_iSyncTile;
		return 0;
	}		

	return SPTerrainModel::GetMemberValue(mvMsg, lParam);;
}




