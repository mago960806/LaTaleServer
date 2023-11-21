#pragma once
//--------------------------------------------------
#include <wtypes.h>
#include <vector>
//--------------------------------------------------
struct PetLevelNode
{
	int iLevel;
	int iItemID;
};
struct PetLevelData
{
	int iItemID;
	std::vector< PetLevelNode > vecNodes;
};
struct PetXPNeedData
{
	int iLevel;
	int iXPNeed;
};
//--------------------------------------------------
class SPPetLevelMgr
{
public:
	SPPetLevelMgr( void );
	~SPPetLevelMgr( void );
	static SPPetLevelMgr &getInstance( void );

	void Init( void );
	int GetRealPetClass( int iItemID, int iLevel );
	int GetXPForPet( int iLevel );
protected:
	std::vector< PetLevelData > m_vecPetLevelCfg;
	std::vector< PetXPNeedData > m_vecPetXPCfg; 
};
