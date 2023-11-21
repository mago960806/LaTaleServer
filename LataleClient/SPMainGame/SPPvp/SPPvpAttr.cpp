#include "SPCommon.h"
#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"

#include "SPItemAttr.h"

#include "SPPvpAttr.h"
#include "SPPvpManager.h"

using namespace std;

//------------------------------------------------------------------------------------
SPPvpAttr::SPPvpAttr()
: m_iPvpLdtID(PVP_EMPTY)
, m_iInstanceID(0)
, m_iMatchType(0)
{
}

//------------------------------------------------------------------------------------
SPPvpAttr::SPPvpAttr(int indunID, UINT32 iInstanceID, int iType)
: m_iPvpLdtID(indunID)
, m_iInstanceID(iInstanceID)
, m_iMatchType(iType)
{
}

//------------------------------------------------------------------------------------
SPPvpAttr::~SPPvpAttr()
{
	Clear();
}

//------------------------------------------------------------------------------------
void SPPvpAttr::Clear()
{
	m_iPvpLdtID		= PVP_EMPTY ;
	m_iMatchType	= 0 ;
	m_iInstanceID	= 0 ;
}

//------------------------------------------------------------------------------------
bool SPPvpAttr::Init()
{
	return false ;
}

//------------------------------------------------------------------------------------
void SPPvpAttr::Process(float fTime)
{
}

//------------------------------------------------------------------------------------
void SPPvpAttr::Render()
{

}

