
#include <vector>

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
#include "SPTerrainModelStatic.h"


SPTerrainModelStatic::SPTerrainModelStatic()
: SPTerrainModel()
, m_pStaticImage(NULL)
{
	Init();
}

//SPTerrainModelStatic::SPTerrainModelStatic(int iInstanceID, SPTexture** ppImage, int iLightMap /*= 0*/)
SPTerrainModelStatic::SPTerrainModelStatic(INT64 iInstanceID, SPTexture** ppImage, int iLightMap /*= 0*/)
: SPTerrainModel()
, m_pStaticImage(NULL)
{
	Init();
	
	m_iInstanceID = iInstanceID;
	m_pStaticImage = *ppImage;
	m_BLightMap	= static_cast<BOOL>(iLightMap);				//jinhee - 04.11.04		
}


SPTerrainModelStatic::~SPTerrainModelStatic()
{
	Destroy();
}

void SPTerrainModelStatic::SetNull()
{
	SPTerrainModel::SetNull();
}

void SPTerrainModelStatic::Init()
{
	SPTerrainModel::Init();
	SetNull();
	m_ClassID = TERRAIN_MODEL_CLASS_STATIC;
	
}

void SPTerrainModelStatic::Destroy()
{
	SPTerrainModel::Destroy();

	SAFE_RELEASE(m_pStaticImage);

	SetNull();
}

LRESULT SPTerrainModelStatic::GetMemberValue(MODEL_VALUE_TYPE mvMsg, LPARAM lParam)
{
	switch(mvMsg) {
	case MV_GETINITIMAGE:
		return (LRESULT)&m_pStaticImage;
	}
	return SPTerrainModel::GetMemberValue(mvMsg, lParam);;
}









