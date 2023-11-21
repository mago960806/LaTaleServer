

#include "SPCommon.h"
#include "SPResourceDef.h"

#include "SPGameObjectDEF.h"
#include "SPManager.h"

#include "SPRenderModelDEF.h"
#include "SPRenderModel.h"

#include "SPTerrainDEF.h"
#include "SPTerrainCluster.h"
#include "SPTerrainManager.h"
#include "SPTerrainModel.h"




SPTerrainModel::SPTerrainModel()
: SPRenderModel()
{
	Init();
}

SPTerrainModel::~SPTerrainModel()
{
}

void SPTerrainModel::Init()
{
	SetNull();
}

void SPTerrainModel::SetNull()
{	
	m_iInstanceID		= 0;	
	m_fNextAnimation	= 0.f;
	m_BLightMap			= FALSE;
	m_strFile			= "";
	
	SPRenderModel::SetNull();
}

void SPTerrainModel::Destroy()
{

}


LRESULT SPTerrainModel::GetMemberValue(MODEL_VALUE_TYPE mvMsg, LPARAM lParam)
{
	switch(mvMsg) {
	case MV_GET_FILENAME:
		return (LRESULT)&m_strFile;		
	case MV_SET_FILENAME:
		m_strFile = (char*)lParam;
		//TRACE("m_strFile[%s]\n", m_strFile.c_str());
		return 0;
	}
	return SPRenderModel::GetMemberValue(mvMsg, lParam);
}





