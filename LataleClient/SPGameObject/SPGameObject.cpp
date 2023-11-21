

#include "SPCommon.h"

#include "SPGameObjectDef.h"
#include "SPGameObject.h"

#include "SPGOBClusterDEF.h"
#include "SPGOBCluster.h"

#include "SPFXDEF.h"

SPGameObject::SPGameObject()
: m_ClassID			(CLASS_ID_INVALID) 
, m_InstanceID		(0)
, m_bEnable			(true)
, m_bClipping(false)
{}

SPGameObject::SPGameObject(INSTANCE_ID instanceID, CLASS_ID ClassID)
: m_InstanceID(instanceID)
, m_ClassID(ClassID)
, m_bClipping(false)
{
}

SPGameObject::~SPGameObject()
{	
}

void SPGameObject::Init()
{
	m_fAccumulateRenderTime = 0.f;
	m_fAccumulateProcessTime = 0.f;	
}


void SPGameObject::SetClassID(CLASS_ID ClassID)
{ 
	m_ClassID = ClassID; 
}

void SPGameObject::SetInstanceID( INSTANCE_ID InstanceID )
{
	m_InstanceID = InstanceID;
}

CLASS_ID SPGameObject::GetClassID()		
{ 
	return m_ClassID;	
}

INSTANCE_ID SPGameObject::GetInstanceID()	
{ 
	return  m_InstanceID;	
}

GU_ID SPGameObject::GetGUID()
{ 
	return GetGlobalID(m_ClassID, m_InstanceID); 
}

bool SPGameObject::IsLocalPlayer() 
{ 
	return false; 
}

void SPGameObject::RenderFX( float fTime , SP_FXM_RENDER_LAYER eRenderLayer )
{
}

bool SPGameObject::IsShowName()
{
	return true;
}

bool SPGameObject::IsShowHP()
{
	return true;
}

bool SPGameObject::IsClipping()
{
	return m_bClipping;
}

void SPGameObject::Process(float fTime)
{
	m_fAccumulateProcessTime += fTime;
	m_fAccumulateRenderTime += fTime;

	if(m_fAccumulateProcessTime > ACCUMULATE_TIME_LIMIT)
		m_fAccumulateProcessTime = ACCUMULATE_TIME_LIMIT;

	if(m_fAccumulateProcessTime > ACCUMULATE_TIME_LIMIT)
		m_fAccumulateProcessTime = ACCUMULATE_TIME_LIMIT;
}

void SPGameObject::Render(float fTime)
{}

int SPGameObject::SPGOBPerformMessage(SPGM msg, LPARAM lparam)
{
	return SPGMESSAGE_OBJ_FUNC(msg, lparam);
}

SPGMESSAGE_OBJ_MAP_BEGIN( SPGameObject ) 
SPGMESSAGE_OBJ_MAP_END_BASE() 


//////////////////////////////////////////////////////////////////////////
int SPGameObject::PerformMouseMove(int iX, int iY)
{
	return 0;
}


int SPGameObject::PerformMouseLDown(int iX, int iY)
{
	return 0;
}


int SPGameObject::PerformMouseLUp(int iX, int iY)
{
	return 0;
}


int SPGameObject::PerformMouseRDown(int iX, int iY)
{
	return 0;
}


int SPGameObject::PerformMouseRUp(int iX, int iY)
{
	return 0;
}


bool SPGameObject::IsCursorIn( int iX, int iY)
{
	return 0;
}




