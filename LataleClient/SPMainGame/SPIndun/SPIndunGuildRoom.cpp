#include "SPCommon.h"
#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"

#include "SPItemAttr.h"

#include "SPIndun.h"
#include "SPIndunManager.h"
#include "SPIndunGuildRoom.h"

using namespace std;

//------------------------------------------------------------------------------------
SPIndunGuildRoom::SPIndunGuildRoom()
: SPIndun()
{
}

//------------------------------------------------------------------------------------
SPIndunGuildRoom::SPIndunGuildRoom(int indunID, UINT32 iInstanceID, INDUN_TYPE iType)
: SPIndun(indunID, iInstanceID, iType)
{
}

//------------------------------------------------------------------------------------
SPIndunGuildRoom::~SPIndunGuildRoom()
{
	Clear();
}

//------------------------------------------------------------------------------------
void SPIndunGuildRoom::Clear()
{
	SPIndun::Clear() ;
}

//------------------------------------------------------------------------------------
bool SPIndunGuildRoom::Init()
{
	SPIndun::Init() ;
	return false ;
}

//------------------------------------------------------------------------------------
void SPIndunGuildRoom::Process(float fTime)
{
	SPIndun::Process(fTime) ;
}

//------------------------------------------------------------------------------------
void SPIndunGuildRoom::Render()
{
	SPIndun::Render() ;
}