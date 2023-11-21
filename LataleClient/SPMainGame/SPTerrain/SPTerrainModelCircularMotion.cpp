

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
#include "SPTerrainModelCircularMotion.h"


SPTerrainModelCircularMotion::SPTerrainModelCircularMotion()
: SPTerrainModel()
, m_pStaticImage(NULL)
{
	Init();	
}

//SPTerrainModelCircularMotion::SPTerrainModelCircularMotion(int iInstance, float fDelay, float fRadian, SPTexture** ppImage, BOOL BLightMap)
SPTerrainModelCircularMotion::SPTerrainModelCircularMotion(INT64 iInstance, float fDelay, float fRadian, SPTexture** ppImage, BOOL BLightMap)
: SPTerrainModel()
, m_pStaticImage(NULL)
{
	Init();
	
	m_iInstanceID = iInstance;
	m_fNextAnimation = fDelay;
	m_fRotateDegree = fRadian;
	m_pStaticImage = *ppImage;
	m_BLightMap	= BLightMap;
}

SPTerrainModelCircularMotion::~SPTerrainModelCircularMotion()
{
	Destroy();
}

void SPTerrainModelCircularMotion::SetNull()
{
	SPTerrainModel::SetNull();
}

void SPTerrainModelCircularMotion::Init()
{
	SPTerrainModel::Init();
	SetNull();
	m_ClassID = TERRAIN_MODEL_CLASS_CIRCULAR_MOTION;
}

void SPTerrainModelCircularMotion::Destroy()
{
	SPTerrainModel::Destroy();

	SAFE_RELEASE(m_pStaticImage);

	SetNull();
}


LRESULT SPTerrainModelCircularMotion::GetMemberValue(MODEL_VALUE_TYPE mvMsg, LPARAM lParam)
{	
	switch(mvMsg) {
	case MV_GET_RADIANDEGREE:
		return (LRESULT) &m_fRotateDegree;
	case MV_SET_RADIANDEGREE:				//jinhee - 04.11.15
		m_fRotateDegree = *(float*)lParam;
		return 0;
	case MV_GETINITIMAGE:
		return (LRESULT) &m_pStaticImage;
	}

	return SPTerrainModel::GetMemberValue(mvMsg, lParam);
}















