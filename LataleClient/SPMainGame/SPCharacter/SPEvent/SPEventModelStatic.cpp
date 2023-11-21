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
#include "SPEventModelStatic.h"


SPEventModelStatic::SPEventModelStatic(SPID_EVENT ID)
: SPEventModel(ID)
, m_pStaticImage(NULL)
{
	Init();
}

SPEventModelStatic::SPEventModelStatic(SPID_EVENT ID, int iInstanceID, SPTexture** ppImage, int iLightMap /*= 0*/)
: SPEventModel(ID)
, m_pStaticImage(NULL)
{
	Init();

	m_iInstanceID = iInstanceID;
	m_pStaticImage = *ppImage;
	m_BLightMap	= static_cast<BOOL>(iLightMap);				//jinhee - 04.11.04		
}


SPEventModelStatic::~SPEventModelStatic()
{
	Destroy();
}

void SPEventModelStatic::SetNull()
{
	SPEventModel::SetNull();
}

void SPEventModelStatic::Init()
{
	SPEventModel::Init();
	SetNull();
	m_ClassID = TERRAIN_MODEL_CLASS_STATIC;

}

void SPEventModelStatic::Destroy()
{
	SPEventModel::Destroy();

	SAFE_RELEASE(m_pStaticImage);

	SetNull();
}

LRESULT SPEventModelStatic::GetMemberValue(MODEL_VALUE_TYPE mvMsg, LPARAM lParam)
{
	switch(mvMsg) {
	case MV_GETINITIMAGE:
		return (LRESULT)&m_pStaticImage;
	}
	return SPEventModel::GetMemberValue(mvMsg, lParam);;
}













