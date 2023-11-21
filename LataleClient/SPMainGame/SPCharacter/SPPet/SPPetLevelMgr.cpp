#include "SPCommon.h"
#include "SPGOBClusterDef.h"
#include "SPGameObject.h"
#include "PacketID.h"
#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPItem.h"
#include "SPItemCluster.h"
#include "SPUtil.h"
#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"
#include "SPPetLevelMgr.h"
//--------------------------------------------------
SPPetLevelMgr::SPPetLevelMgr( void )
{
	Init();
}
SPPetLevelMgr::~SPPetLevelMgr( void )
{
	// nothing
}
SPPetLevelMgr &SPPetLevelMgr::getInstance( void )
{
	static SPPetLevelMgr _instance;
	return _instance;
}
//--------------------------------------------------
void SPPetLevelMgr::Init( void )
{
	//--------------------------------------------------
	m_vecPetLevelCfg.clear();
	SPLDTFile	*pFile = NULL;
	g_pResourceManager->GetLDTFromFileName( "DATA/LDT/NEWPET_CLASS.LDT", &pFile );
	if (!pFile)
		return;

	LDT_Field field_Lv;
	LDT_Field field_ID;
	PetLevelNode node;
	PetLevelData data;

	int iRecordCount = pFile->GetItemCount();
	int iItemID = 0;
	for (int idx = 0; idx < iRecordCount; ++idx)
	{
		iItemID = pFile->GetPrimaryKey( idx );
		if (iItemID == 0)
			continue;
		data.iItemID = iItemID;

		pFile->GetFieldFromLable( iItemID, "_LV_ClassID1", field_Lv );
		pFile->GetFieldFromLable( iItemID, "_Pet_ClassID1", field_ID );
		node.iItemID = field_ID.uData.lValue;
		node.iLevel = field_Lv.uData.lValue;
		data.vecNodes.push_back( node );
		
		pFile->GetFieldFromLable( iItemID, "_LV_ClassID2", field_Lv );
		pFile->GetFieldFromLable( iItemID, "_Pet_ClassID2", field_ID );
		node.iItemID = field_ID.uData.lValue;
		node.iLevel = field_Lv.uData.lValue;
		data.vecNodes.push_back( node );

		pFile->GetFieldFromLable( iItemID, "_LV_ClassID3", field_Lv );
		pFile->GetFieldFromLable( iItemID, "_Pet_ClassID3", field_ID );
		node.iItemID = field_ID.uData.lValue;
		node.iLevel = field_Lv.uData.lValue;
		data.vecNodes.push_back( node );

		m_vecPetLevelCfg.push_back( data );
		data.iItemID = 0;
		data.vecNodes.clear();
	}

	SAFE_RELEASE( pFile );
	//--------------------------------------------------
	m_vecPetXPCfg.clear();
	g_pResourceManager->GetLDTFromFileName( "DATA/LDT/PETLV.LDT", &pFile );
	if (!pFile)
		return;

	LDT_Field field;
	PetXPNeedData xpData;
	int iRecordCountXP = pFile->GetItemCount();
	int iItemIDXP = 0;
	for (int idx = 0; idx < iRecordCountXP; ++idx)
	{
		iItemIDXP = pFile->GetPrimaryKey( idx );
		if (iItemIDXP == 0)
			continue;

		pFile->GetFieldFromLable( iItemIDXP, "_PetLV", field );
		xpData.iLevel = field.uData.lValue;

		pFile->GetFieldFromLable( iItemIDXP, "_Max_EXP", field );
		xpData.iXPNeed = field.uData.lValue;

		m_vecPetXPCfg.push_back( xpData );
		xpData.iLevel = 0;
		xpData.iXPNeed = 0;
	}
	SAFE_RELEASE( pFile );
}
int SPPetLevelMgr::GetRealPetClass(int iItemID, int iLevel)
{
	PetLevelData *pData = NULL;
	for (int idx = 0; idx < (int)m_vecPetLevelCfg.size(); ++idx)
	{
		if (m_vecPetLevelCfg[idx].iItemID == iItemID)
		{
			pData = &m_vecPetLevelCfg[idx];
			break;
		}
	}

	if (!pData)
		return 0;

	PetLevelNode *pNode = NULL;
	for (int idx = (int)pData->vecNodes.size()-1; idx >= 0; --idx)
	{
		if (pData->vecNodes[idx].iLevel <= iLevel)
		{
			pNode = &(pData->vecNodes[idx]);
			break;
		}
	}
	
	if (!pNode)
		return 0;

	return pNode->iItemID;
}
//--------------------------------------------------
int SPPetLevelMgr::GetXPForPet( int iLevel )
{
	PetXPNeedData *pData = NULL;
	for (int idx = 0; idx < (int)m_vecPetXPCfg.size(); ++idx)
	{
		if (m_vecPetXPCfg[idx].iLevel == iLevel)
		{
			pData = &m_vecPetXPCfg[idx];
			break;
		}
	}

	if (!pData)
		return 10000;
	return pData->iXPNeed;
}
//--------------------------------------------------
