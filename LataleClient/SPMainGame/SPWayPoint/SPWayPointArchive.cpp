
#include <algorithm>

#include "SPCommon.h"
#include "SPEventDef.h"
#include "SPEventManager.h"

#include "SPCheckManager.h"

#include "PacketID.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPGameObjectDEF.h"
#include "SPGameObject.h"
#include "SPGOBManager.h"

#include "SPWindowDEF.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "SPCheckManager.h"

#include "SPWayPointAttr.h"
#include "SPWayPointArchive.h"

#include "SPDebug.h"


struct _IsEqualWayPoint : binary_function<SPWayPointAttr, int, bool> {
	bool operator() (SPWayPointAttr pWayPointAttr, int iId) const
	{
		if(pWayPointAttr.m_iID == iId)
			return true;
		return false;
	}
} IsEqualWayPoint;



struct _IsEqualWayPointEvent : binary_function<SPWayPointAttr, int, bool> {
	bool operator() (SPWayPointAttr pWayPointAttr, int iEventID) const
	{
		if(pWayPointAttr.m_iMapEventID == iEventID)
			return true;
		return false;
	}
} IsEqualWayPointEvent;


SPWayPointArchive::SPWayPointArchive()
{	
	m_vpWayPoint.resize(MAX_WAY_POINT);
	ClearAllWayPoint();
	LoadLDTData();
}


SPWayPointArchive::~SPWayPointArchive()
{
	//ClearAllWayPoint();
	m_vpWayPoint.clear();
}


void SPWayPointArchive::ClearAllWayPoint()
{
	////
	std::vector<SPWayPointAttr>::iterator iter;
	iter = m_vpWayPoint.begin();
	for(; iter != m_vpWayPoint.end(); ++iter) {
		(*iter).Reset();
	}
}


bool SPWayPointArchive::LoadLDTData()
{
	SPLDTFile* pLDTFile;
	g_pResourceManager->GetLDTFromFileName(RES_FNAME_LDT_WAY_POINT, &pLDTFile);	

	if(pLDTFile == NULL) {
		return false;
	}

	int iRecordCount = pLDTFile->GetItemCount();
	int iItemID = 0;	

	LDT_Field ldtFieldGroup;
	LDT_Field ldtFieldName;
	LDT_Field ldtFieldDesc;
	LDT_Field ldtFieldType;

	LDT_Field ldtFieldRequireType;
	LDT_Field ldtFieldRequireID;
	LDT_Field ldtFieldRequireValue1;
	LDT_Field ldtFieldRequireValue2;

	LDT_Field ldtFieldItemID;
	LDT_Field ldtFieldItemCount;

	LDT_Field ldtFieldEffect;
	LDT_Field ldtFieldErr;
	LDT_Field ldtFieldStory;
	LDT_Field ldtFieldOpen;
	
	int iRequireType = 0;
	int iRequireID = 0;
	int iRequireValue1 = 0;
	int iRequireValue2 = 0;
	
	int iUseItemID = 0;
	int iUseItemCount = 0;

	int iEffect = 0;

	char szFieldRequireType[64];
	char szFieldRequireID[64];
	char szFieldRequireValue1[64];
	char szFieldRequireValue2[64];
	
	char szFieldItemID[64];
	char szFieldItemCount[64];

	char szFieldEffect[64];

	for(int i = 0; i < iRecordCount; i++) {		
		if(i >= MAX_WAY_POINT) {
			return false;
		}

		iItemID = pLDTFile->GetPrimaryKey( i );
		m_vpWayPoint.at(i).m_iID = iItemID;
		m_vpWayPoint.at(i).SetMapEventID(iItemID);

		pLDTFile->GetFieldFromLable( iItemID, "_WayPoint_Group",		ldtFieldGroup);		
		m_vpWayPoint.at(i).m_iGroup = ldtFieldGroup.uData.lValue;		

		pLDTFile->GetFieldFromLable( iItemID, "_Name",					ldtFieldName);		
		if(ldtFieldName.uData.pValue)
			m_vpWayPoint.at(i).m_strName = ldtFieldName.uData.pValue;

		pLDTFile->GetFieldFromLable( iItemID, "_Description",			ldtFieldDesc);		
		if(ldtFieldDesc.uData.pValue)
			m_vpWayPoint.at(i).m_strDesc = ldtFieldDesc.uData.pValue;

		pLDTFile->GetFieldFromLable( iItemID, "_Type",					ldtFieldType);		
		m_vpWayPoint.at(i).m_iType = (WAY_POINT_TYPE)ldtFieldType.uData.lValue;
		
		//Require
		for(int j = 0; j < MAX_WAY_POINT_RANK; j++) {
			for(int k = 0; k < MAX_WAY_POINT_REQUIRE; k++) {
				iRequireType = 0;
				iRequireID = 0;
				iRequireValue1 = 0;
				iRequireValue2 = 0;

				ZeroMemory(szFieldRequireType, 64);
				ZeroMemory(szFieldRequireID, 64);
				ZeroMemory(szFieldRequireValue1, 64);
				ZeroMemory(szFieldRequireValue2, 64); //_Rank1_Require1_Type

				wsprintf(szFieldRequireType,	"_Rank%d_Require%d_Type",		j+1, k+1);
				wsprintf(szFieldRequireID,		"_Rank%d_Require%d_ID",			j+1, k+1);
				wsprintf(szFieldRequireValue1,	"_Rank%d_Require%d_Value1",		j+1, k+1);
				wsprintf(szFieldRequireValue2,	"_Rank%d_Require%d_Value2",		j+1, k+1);

				pLDTFile->GetFieldFromLable(iItemID, szFieldRequireType,	ldtFieldRequireType);
				iRequireType = ldtFieldRequireType.uData.lValue;
				
				pLDTFile->GetFieldFromLable(iItemID, szFieldRequireID,	ldtFieldRequireID);
				iRequireID = ldtFieldRequireID.uData.lValue;

				pLDTFile->GetFieldFromLable(iItemID, szFieldRequireValue1, ldtFieldRequireValue1);
				iRequireValue1 = ldtFieldRequireValue1.uData.lValue;

				pLDTFile->GetFieldFromLable(iItemID, szFieldRequireValue2, ldtFieldRequireValue2);
				iRequireValue2 = ldtFieldRequireValue2.uData.lValue;
				
				m_vpWayPoint.at(i).m_Require[j][k].SetRequire(iRequireType, iRequireID, iRequireValue1, iRequireValue2);
			}

			iUseItemID = 0;
			iUseItemCount = 0;
			ZeroMemory(szFieldItemID, 64);
			ZeroMemory(szFieldItemCount, 64);

			wsprintf(szFieldItemID, "_Rank%d_Consumption_Item", j+1);
			wsprintf(szFieldItemCount, "_Rank%d_Consumption_ItemCount", j+1);

			pLDTFile->GetFieldFromLable(iItemID, szFieldItemID,	ldtFieldItemID);
			iUseItemID = ldtFieldItemID.uData.lValue;

			pLDTFile->GetFieldFromLable(iItemID, szFieldItemCount,	ldtFieldItemCount);
			iUseItemCount = ldtFieldItemCount.uData.lValue;

			m_vpWayPoint.at(i).m_iUseItemID[j] = iUseItemID;
			m_vpWayPoint.at(i).m_iUseItemCount[j] = iUseItemCount;
		}
		
		//Effect
		for(int j = 0; j < MAX_WAY_POINT_EFFECT; j++) {
			iEffect = 0;
			ZeroMemory(szFieldEffect, 64);
			wsprintf(szFieldEffect, "_EffectID%d", j+1);
			pLDTFile->GetFieldFromLable(iItemID, szFieldEffect,	ldtFieldEffect);
			iEffect = ldtFieldEffect.uData.lValue;

			m_vpWayPoint.at(i).m_iEffect[j] = iEffect;
		}
		
		//Error Msg
		pLDTFile->GetFieldFromLable( iItemID, "_Error_Message",			ldtFieldErr);		
		if(ldtFieldErr.uData.pValue)
			m_vpWayPoint.at(i).m_strErr = ldtFieldErr.uData.pValue;

		//StoryID 
		pLDTFile->GetFieldFromLable( iItemID, "_Story",		ldtFieldStory);		
		m_vpWayPoint.at(i).m_iStoryID = ldtFieldStory.uData.lValue;

		//Open
		pLDTFile->GetFieldFromLable( iItemID, "_OnOff",		ldtFieldOpen);
		m_vpWayPoint.at(i).m_bOpen = (bool)ldtFieldOpen.uData.lValue;
	}

	return true;
}


std::vector<SPWayPointAttr>* SPWayPointArchive::GetWayPointList()
{
	return &m_vpWayPoint;
}


SPWayPointAttr* SPWayPointArchive::GetWayPoint(int iIndex)
{
	if(iIndex < 0 || iIndex > static_cast<int>(m_vpWayPoint.size()))
		return NULL;

	return &m_vpWayPoint.at(iIndex);
}


SPWayPointAttr* SPWayPointArchive::FindWayPointByID(int iID)
{
	std::vector<SPWayPointAttr>::iterator iter;
	iter = std::find_if(m_vpWayPoint.begin(), m_vpWayPoint.end(),
		std::bind2nd(IsEqualWayPoint, iID));

	if(iter != m_vpWayPoint.end()) {
		return &(*iter);
	}
	else {
		return NULL;
	}
}


SPWayPointAttr* SPWayPointArchive::FindWayPointByMapEventID(int iMapEventID)
{
	std::vector<SPWayPointAttr>::iterator iter;
	iter = std::find_if(m_vpWayPoint.begin(), m_vpWayPoint.end(),
		std::bind2nd(IsEqualWayPointEvent, iMapEventID));

	if(iter != m_vpWayPoint.end()) {
		return &(*iter);
	}
	else {
		return NULL;
	}
}


void SPWayPointArchive::PrintSaveInfo()
{
	std::vector<SPWayPointAttr>::iterator iter;
	
	int iIndex = 0;
	DXUTOutputDebugString("-------- WayPoint Save Info -----------\n");
	for(iter = m_vpWayPoint.begin(); iter != m_vpWayPoint.end(); ++iter, iIndex++) {
		DXUTOutputDebugString("%d ", (*iter).IsSave());
	}
	DXUTOutputDebugString("\n---------------------------------------\n");
}

