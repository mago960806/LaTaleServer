#include "SPCommon.h"
#include "SPUtil.h"

#include "PacketID.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPEventDEF.h"

#include "SPCoreMissionAttr.h"
#include "SPCoreMissionArchive.h"

#include "SPCheckManager.h"

#include "SPDebug.h"



SPCoreMissionArchive::SPCoreMissionArchive()
{
	Clear();
}


SPCoreMissionArchive::~SPCoreMissionArchive()
{
	Clear();
}


void SPCoreMissionArchive::Clear()
{
	DeleteAllCoreMission();
}


void SPCoreMissionArchive::DeleteAllCoreMission()
{
	std::map<int, SPCoreMissionAttr*>::iterator iter = m_mCoreMission.begin();
	for(; iter != m_mCoreMission.end(); ++iter){
		SAFE_DELETE((*iter).second);
	}

	m_mCoreMission.clear();
}


bool SPCoreMissionArchive::LoadCoreMissionLDT()
{
	SPLDTFile* pLDTFile		= NULL;
	g_pResourceManager->GetLDTFromFileName( RES_FNAME_LDT_EVENT_CORE_MISSION , &pLDTFile);	
	if( pLDTFile == NULL )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "----------------------------------->CORE EVENT REQUIRE LDT FILE LOAD FAIL [%s] \n" , RES_FNAME_LDT_EVENT_CORE_MISSION );
#endif
		return false;
	}

	DeleteAllCoreMission();

	LDT_Field LDTField;
	int	iItemID = 0;
	int i = 0;
	int j = 0;

	int				iRecordCount	= pLDTFile->GetItemCount();
	SPCoreMissionAttr*	pMissionAttr = NULL;
	int iStat = 0;
	int iStatValue = 0;
	int iValue1 = 0;
	int	iValue2 = 0;

	for( i = 0; i < iRecordCount; ++i )
	{
		pMissionAttr = new SPCoreMissionAttr;
		
		iItemID	=	pLDTFile->GetPrimaryKey( i );

		pMissionAttr->m_iID	=	iItemID;		

		pLDTFile->GetField( iItemID , 0, LDTField );				//_MissionType
		pMissionAttr->m_iType	= (CORE_MISSION_TYPE)LDTField.uData.lValue;

		for(j = 0; j < MAX_CORE_EVENT_REQUIRE; j++) {
			iStat = 0;
			iStatValue = 0;
			iValue1 = 0;
			iValue2 = 0;

			pLDTFile->GetField( iItemID , 12 + (j*4), LDTField );				//_Require1_Type
			iStat = LDTField.uData.lValue;

			pLDTFile->GetField( iItemID , 13 + (j*4), LDTField );				//_Require1_ID
			iStatValue = LDTField.uData.lValue;

			pLDTFile->GetField( iItemID , 14 + (j*4), LDTField );				//_Require1_Value1
			iValue1 = LDTField.uData.lValue;

			pLDTFile->GetField( iItemID , 15 + (j*4), LDTField );				//_Require1_Value2
			iValue2 = LDTField.uData.lValue;
			
			pMissionAttr->m_Require[j].SetRequire(iStat, iStatValue, iValue1, iValue2);
		}

		pLDTFile->GetField( iItemID , 44, LDTField );
		pMissionAttr->SetSavePos(LDTField.uData.lValue);			//내부에서 1베이스를 0베이스로 변환해준다

		if(GetCoreMissionAttr(pMissionAttr->m_iID) == NULL) {
			m_mCoreMission.insert(std::map<int, SPCoreMissionAttr*>::value_type(pMissionAttr->m_iID, pMissionAttr));	
		}
		else {
			SAFE_DELETE(pMissionAttr);
		}
	}
	
	return true;
}


SPCoreMissionAttr*	SPCoreMissionArchive::GetCoreMissionAttr(int iItemID)
{
	std::map<int, SPCoreMissionAttr*>::iterator iter = m_mCoreMission.find(iItemID);

	if(iter != m_mCoreMission.end())
		return ((*iter).second);

	return NULL;
}


SPCoreMissionAttr*	SPCoreMissionArchive::FindMissionAttr(int iSavePos)
{
	std::map<int, SPCoreMissionAttr*>::iterator iter = m_mCoreMission.begin();
	for(;iter != m_mCoreMission.end(); ++iter) {
		if((*iter).second->m_iSavePos < 0)
			continue;

		if((*iter).second->m_iSavePos == iSavePos)
			return (*iter).second;
	}

	return NULL;
}


