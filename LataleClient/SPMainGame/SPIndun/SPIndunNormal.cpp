#include "SPCommon.h"
#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"

#include "SPItemAttr.h"

#include "SPIndun.h"
#include "SPIndunManager.h"
#include "SPIndunNormal.h"

using namespace std;

//------------------------------------------------------------------------------------
SPIndunNormal::SPIndunNormal()
: SPIndun()
{
}

//------------------------------------------------------------------------------------
SPIndunNormal::SPIndunNormal(int indunID, UINT32 iInstanceID, INDUN_TYPE iType)
: SPIndun(indunID, iInstanceID, iType)
{
}

//------------------------------------------------------------------------------------
SPIndunNormal::~SPIndunNormal()
{
	Clear() ;
}

//------------------------------------------------------------------------------------
void SPIndunNormal::Clear()
{
	SPIndun::Clear() ;
}

//------------------------------------------------------------------------------------
bool SPIndunNormal::Init()
{
	SPIndun::Init() ;
	return false ;
}

//------------------------------------------------------------------------------------
void SPIndunNormal::Process(float fTime)
{
	SPIndun::Process(fTime) ;
}

//------------------------------------------------------------------------------------
void SPIndunNormal::Render()
{
	SPIndun::Render() ;
}