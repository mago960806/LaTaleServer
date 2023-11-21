#include "SPCommon.h"
#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"

#include "SPItemAttr.h"

#include "SPIndun.h"
#include "SPIndunManager.h"

using namespace std;

//------------------------------------------------------------------------------------
SPIndun::SPIndun()
: m_iIndunLdtID(INDUN_EMPTY)
, m_iInstanceID(0)
, m_iType(IDT_NULL)
{
}

//------------------------------------------------------------------------------------
SPIndun::SPIndun(int indunID, UINT32 iInstanceID, INDUN_TYPE iType)
: m_iIndunLdtID(indunID)
, m_iInstanceID(iInstanceID)
, m_iType(iType)
{
}

//------------------------------------------------------------------------------------
SPIndun::~SPIndun()
{
	Clear();
}

//------------------------------------------------------------------------------------
void SPIndun::Clear()
{
	m_iIndunLdtID	= INDUN_EMPTY ;
	m_iType			= IDT_NULL ;
	m_iInstanceID	= 0 ;
}

//------------------------------------------------------------------------------------
bool SPIndun::Init()
{
	return false ;
}

//------------------------------------------------------------------------------------
void SPIndun::Process(float fTime)
{
}

//------------------------------------------------------------------------------------
void SPIndun::Render()
{

}