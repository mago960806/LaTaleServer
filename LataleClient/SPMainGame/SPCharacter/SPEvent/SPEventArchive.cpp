#include "SPCommon.h"
#include "SPMainGameDEF.H"

#include "SPGameObjectDef.h"
#include "SPGameObject.h"

#include "SPRenderModel.h"

#include "SPFXDEF.h"
#include "SPFXModel.h"
#include "SPFXUnit.h"
#include "SPFXGroupUnit.h"
#include "SPFXArchive.h"

#include "SPEvent.h"
#include "SPEventDEF.h"
#include "SPEventNpcHandler.h"
#include "SPEventManager.h"

#include "SPCashTradeManager.h"

#include "SPQuestAttr.h"
#include "SPQuestManager.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPEventArchive.h"
#include "SPItemCluster.h"
#include "SPDebug.h"

#include "atltime.h"
#include <algorithm>
using namespace std;

#ifdef _DEBUG
#include <iostream>			//jinhee - [2005/1/3] //테스트를 위한...
#include <fstream>
#endif


struct _IsEqualEvent : binary_function<MAPEVENT_INFO* , int, bool> {
	bool operator() (MAPEVENT_INFO* pEvent, int iItemID) const
	{
		if(pEvent->id == iItemID)
			return true;
		return false;
	}
} IsEqualEvent;


struct _IsEqualNpc : binary_function<SPNpc* , int, bool> {
	bool operator() (SPNpc* pNpc, int iItemID) const
	{
		if(pNpc->m_iNpcID == iItemID)
			return true;
		return false;
	}
} IsEqualNpc;


struct _IsEqualQuestList : binary_function<SPNpcQuestList* , int, bool> {
	bool operator() (SPNpcQuestList* pQuestList, int iItemID) const
	{
		if(pQuestList->m_iNpcID == iItemID)
			return true;
		return false;
	}
} IsEqualQuestList;


struct _IsEqualQuestInfo : binary_function<SPQuestAttr* , int, bool> {
	bool operator() (SPQuestAttr* pQuestAttr, int iItemID) const
	{
		if(pQuestAttr->m_iQuestID == iItemID)
			return true;
		return false;
	}
} IsEqualQuestInfo;


struct _IsEqualSellItem : binary_function<SPSellItem* , int, bool> {
	bool operator() (SPSellItem* pSellItem, int iItemID) const
	{
		if(pSellItem->m_iItemID == iItemID)
			return true;
		return false;
	}
} IsEqualSellItem;


struct _IsEqualCoreEventInfo : binary_function<SPCoreEventInfo*, int, bool> {
	bool operator() (SPCoreEventInfo* pEventInfo, int iItemID) const
	{
		if(pEventInfo->m_iID == iItemID)
			return true;
		return false;
	}
} IsEqualCoreEventInfo;

struct _IsEqualCutInTalk : binary_function<SPCutInTalk*, int, bool> {
	bool operator() (SPCutInTalk* pCutInTalk, int iItemID) const
	{
		if(pCutInTalk->m_iID == iItemID)
			return true;
		return false;
	}
} IsEqualCutInTalk;


SPEventArchive::SPEventArchive()
{
	Init();	
}


SPEventArchive::~SPEventArchive()
{	
	Clean();
}


void SPEventArchive::Init()
{
	m_iDeathItem = 0;
	m_iMicServer = 0;
	m_iMicChannel = 0;	
	m_iMailFee = 0;
	m_iMailStampItem = 0;
}


void SPEventArchive::Clean()
{
	m_mCoreEventReqInfo.clear();
	DeleteAllEvent();
	DeleteAllNpc();
	DeleteAllQuestList();
	DeleteAllQuestAttr();
	DeleteAllCoreEventInfo();
	DeleteAllMapInfo();
	DeleteAllEvevtPortal();
	DeleteAllCutInTalk();
}



//////////////////////////////////////////////////////////////////////////
void SPEventArchive::DeleteAllEvent()
{
	std::vector<MAPEVENT_INFO*>::iterator Iter = m_vpEventInfo.begin();
	for(;Iter != m_vpEventInfo.end(); ++Iter) {
		SAFE_DELETE(*Iter);
	}
	m_vpEventInfo.clear();
}

bool SPEventArchive::LoadEventLDT()
{
	SPLDTFile* pLDTFile;
	g_pResourceManager->GetLDTFromFileName(RES_FNAME_LDT_MAP_EVENT, &pLDTFile);	

	if(pLDTFile == NULL) {
		return false;
	}
	
	int iRecordCount = pLDTFile->GetItemCount();
	int iItemID = 0;
	
	LDT_Field ldtFieldName;	
	LDT_Field ldtFieldLeft;
	LDT_Field ldtFieldTop;
	LDT_Field ldtFieldRight;
	LDT_Field ldtFieldBottom;
	LDT_Field ldtFieldImageFile;	
	LDT_Field ldtFieldMessage;	
	LDT_Field ldtFieldMessageOff;	
	LDT_Field ldtFieldEventType;
	LDT_Field ldtFieldEventTypeID;	

	LDT_Field ldtFieldObjTerrain;
	LDT_Field ldtObjFlip;

	LDT_Field ldtFieldRequireType1;
	LDT_Field ldtFieldRequireID1;
	LDT_Field ldtFieldRequireVelueF1;
	LDT_Field ldtFieldRequireVelueS1;

	LDT_Field ldtFieldRequireType2;
	LDT_Field ldtFieldRequireID2;
	LDT_Field ldtFieldRequireVelueF2;
	LDT_Field ldtFieldRequireVelueS2;

	LDT_Field ldtFieldErrorMessage;

	LDT_Field ldtFieldStageID;
	LDT_Field ldtFieldMapGroupID;
	LDT_Field ldtFieldMapID;
	LDT_Field ldtFieldPosX;
	LDT_Field ldtFieldPosY;

	LDT_Field ldtFieldDesc;
	LDT_Field ldtFieldIcon;
	LDT_Field ldtFieldIconIndex;
	LDT_Field ldtFieldInsDungeon;
	LDT_Field ldtFieldFxID;

	LDT_Field ldtFieldDynamicCreate;

	MAPEVENT_INFO_EXT	stMapEventInfoExt;

	for(int i = 0; i < iRecordCount; i++) {
		MAPEVENT_INFO* pMapEvent = new MAPEVENT_INFO;
		
		iItemID = pLDTFile->GetPrimaryKey( i );
		pMapEvent->id = iItemID;

		pLDTFile->GetFieldFromLable( iItemID, "_Name",					ldtFieldName);
		pLDTFile->GetFieldFromLable( iItemID, "_RectLeft",				ldtFieldLeft);
		pLDTFile->GetFieldFromLable( iItemID, "_RectTop",				ldtFieldTop);
		pLDTFile->GetFieldFromLable( iItemID, "_RectRight",				ldtFieldRight);
		pLDTFile->GetFieldFromLable( iItemID, "_RectBottom",			ldtFieldBottom);
		pLDTFile->GetFieldFromLable( iItemID, "_ObjectImage",			ldtFieldImageFile);
		pLDTFile->GetFieldFromLable( iItemID, "_TipMessage",			ldtFieldMessage);
		pLDTFile->GetFieldFromLable( iItemID, "_EventType",				ldtFieldEventType);
		pLDTFile->GetFieldFromLable( iItemID, "_EventTypeID",			ldtFieldEventTypeID);

		pLDTFile->GetFieldFromLable( iItemID, "_Require1_Type",			ldtFieldRequireType1);
		pLDTFile->GetFieldFromLable( iItemID, "_Require1_ID",			ldtFieldRequireID1);
		pLDTFile->GetFieldFromLable( iItemID, "_Require1_Value1",		ldtFieldRequireVelueF1);
		pLDTFile->GetFieldFromLable( iItemID, "_Require1_Value2",		ldtFieldRequireVelueS1);

		pLDTFile->GetFieldFromLable( iItemID, "_Require2_Type",			ldtFieldRequireType2);
		pLDTFile->GetFieldFromLable( iItemID, "_Require2_ID",			ldtFieldRequireID2);
		pLDTFile->GetFieldFromLable( iItemID, "_Require2_Value1",		ldtFieldRequireVelueF2);
		pLDTFile->GetFieldFromLable( iItemID, "_Require2_Value2",		ldtFieldRequireVelueS2);	

		pLDTFile->GetFieldFromLable( iItemID, "_ObjectTerrain",			ldtFieldObjTerrain);
		pLDTFile->GetFieldFromLable( iItemID, "_ObjectFlip",			ldtObjFlip);

		pLDTFile->GetFieldFromLable( iItemID, "_ErrorMessage",			ldtFieldErrorMessage);
		pLDTFile->GetFieldFromLable( iItemID, "_TipMessage_off",		ldtFieldMessageOff);

		pLDTFile->GetFieldFromLable( iItemID, "_StageID",				ldtFieldStageID);
		pLDTFile->GetFieldFromLable( iItemID, "_MapGroupID",			ldtFieldMapGroupID);
		pLDTFile->GetFieldFromLable( iItemID, "_MapID",					ldtFieldMapID);
		pLDTFile->GetFieldFromLable( iItemID, "_PosX",					ldtFieldPosX);
		pLDTFile->GetFieldFromLable( iItemID, "_PosY",					ldtFieldPosY);

		pLDTFile->GetFieldFromLable( iItemID, "_Description",			ldtFieldDesc);
		pLDTFile->GetFieldFromLable( iItemID, "_Icon",					ldtFieldIcon);
		pLDTFile->GetFieldFromLable( iItemID, "_Icon_Index",			ldtFieldIconIndex);
		////[2007/5/31]-jinhee
		pLDTFile->GetFieldFromLable( iItemID, "_Ind_Num",				ldtFieldInsDungeon);
		pLDTFile->GetFieldFromLable( iItemID, "_Fx_id",					ldtFieldFxID);

		pLDTFile->GetFieldFromLable( iItemID, "_Field_Dynamic_Create",	ldtFieldDynamicCreate);

		if(ldtFieldName.uData.pValue) {
			//sprintf(pMapEvent->name, "%s", ldtFieldName.uData.pValue);
			strncpy(pMapEvent->name, ldtFieldName.uData.pValue, LEN_NAME + 1);
		}		
		
		pMapEvent->rectTop				= (float)ldtFieldTop.uData.lValue;
		pMapEvent->rectLeft				= (float)ldtFieldLeft.uData.lValue;
		pMapEvent->rectRight			= (float)ldtFieldRight.uData.lValue;
		pMapEvent->rectBottom			= (float)ldtFieldBottom.uData.lValue;

		pMapEvent->obejctImage			= (UINT32)ldtFieldImageFile.uData.lValue;
		if(ldtFieldMessage.uData.pValue) {
			wsprintf(pMapEvent->tipMessage, "%s", ldtFieldMessage.uData.pValue);
		}		
		pMapEvent->eventType			= (UINT8)ldtFieldEventType.uData.lValue;
		pMapEvent->eventTypeID			= (UINT32)ldtFieldEventTypeID.uData.lValue;	

		int iRequireType = 0;
		int iRequireID = 0;
		int iValue1 = 0;
		int iValue2 = 0;

		iRequireType	= ldtFieldRequireType1.uData.lValue;
		iRequireID		= ldtFieldRequireID1.uData.lValue;
		iValue1			= ldtFieldRequireVelueF1.uData.lValue;
		iValue2			= ldtFieldRequireVelueS1.uData.lValue;	
		pMapEvent->requireInfo[0].SetRequireInfo(iRequireType, iRequireID, iValue1, iValue2);

		iRequireType	= ldtFieldRequireType2.uData.lValue;
		iRequireID		= ldtFieldRequireID2.uData.lValue;
		iValue1			= ldtFieldRequireVelueF2.uData.lValue;
		iValue2			= ldtFieldRequireVelueS2.uData.lValue;	
		pMapEvent->requireInfo[1].SetRequireInfo(iRequireType, iRequireID, iValue1, iValue2);

		pMapEvent->iTerrainLayer		= (UINT8)ldtFieldObjTerrain.uData.lValue;
		pMapEvent->iTerrainLayer		= (UINT8)(pMapEvent->iTerrainLayer + BG_ID_LAYER_TERRAIN0 - 1);
		pMapEvent->iFacing				= (UINT8)ldtObjFlip.uData.lValue;
		if(ldtFieldErrorMessage.uData.pValue) {
			wsprintf(pMapEvent->errorMessage, "%s", ldtFieldErrorMessage.uData.pValue);
		}

		if(ldtFieldMessageOff.uData.pValue) {
			wsprintf(pMapEvent->tipMessageOff, "%s", ldtFieldMessageOff.uData.pValue);
		}

		pMapEvent->mapInfo.uiStageID	=	(UINT16)ldtFieldStageID.uData.lValue;
		pMapEvent->mapInfo.uiMapGroupID	=	(UINT16)ldtFieldMapGroupID.uData.lValue;
		pMapEvent->mapInfo.uiMapID		=	(UINT16)ldtFieldMapID.uData.lValue;
		pMapEvent->posX					=	(float)ldtFieldPosX.uData.lValue;
		pMapEvent->posY					=	(float)ldtFieldPosY.uData.lValue;
		
		////[2007/5/31]-jinhee
		pMapEvent->iInsDungeunID		=	(UINT32)ldtFieldInsDungeon.uData.lValue;
		pMapEvent->iDynamicFX			=	(UINT)ldtFieldFxID.uData.lValue;

		pMapEvent->byDynamicCreate		=	(UINT8)ldtFieldDynamicCreate.uData.lValue;

		if(ldtFieldDesc.uData.pValue)
			stMapEventInfoExt.m_strDesc		=	ldtFieldDesc.uData.pValue;
		stMapEventInfoExt.m_iIcon		=	ldtFieldIcon.uData.lValue;
		stMapEventInfoExt.m_iIconIndex	=	ldtFieldIconIndex.uData.lValue;

		m_vpEventInfo.push_back(pMapEvent);
		m_mEventInfoExt.insert( std::map< UINT32 , MAPEVENT_INFO_EXT >::value_type( pMapEvent->id , stMapEventInfoExt ) );
	}

	SAFE_RELEASE(pLDTFile);
	return true;
}

MAPEVENT_INFO* SPEventArchive::GetEvent(int iItemID)
{
	std::vector<MAPEVENT_INFO*>::iterator Iter = 
		std::find_if(m_vpEventInfo.begin(),
		m_vpEventInfo.end(),
		std::bind2nd(IsEqualEvent,
		iItemID));	

	if(Iter == m_vpEventInfo.end())
		return NULL;
	
	return (*Iter);
}

bool SPEventArchive::FindEvent( std::vector<MAPEVENT_INFO*>* pvDataOut , int iStage )
{
	if( pvDataOut == NULL )
		return false;
	
	pvDataOut->clear();

	std::vector<MAPEVENT_INFO*>::iterator	Iter	=	m_vpEventInfo.begin();
	MAPEVENT_INFO*	pMapEventInfo;

	while( Iter != m_vpEventInfo.end() )
	{
		pMapEventInfo	=	(*Iter);

		if( pMapEventInfo != NULL )
		{
			if( pMapEventInfo->mapInfo.uiStageID == iStage )
				pvDataOut->push_back( pMapEventInfo );
		}

		++Iter;
	}

	if( (int)pvDataOut->size() > 0 )
		return true;

	return false;
}

MAPEVENT_INFO_EXT* SPEventArchive::GetEventExt( UINT32 uiItemID )
{
	std::map< UINT32 , MAPEVENT_INFO_EXT >::iterator	mIter	=	m_mEventInfoExt.find( uiItemID );

	if( mIter == m_mEventInfoExt.end() )
		return NULL;

	return &( (*mIter).second );
}


//////////////////////////////////////////////////////////////////////////
void SPEventArchive::DeleteAllMapInfo()
{
	std::vector<MAPINFO_LDT*>::iterator Iter = m_vpMapInfoLdt.begin();
	for(;Iter != m_vpMapInfoLdt.end(); ++Iter) {
		SAFE_DELETE(*Iter);
	}
	m_vpMapInfoLdt.clear();
}
bool SPEventArchive::LoadMapInfoLDT()
{
	SPLDTFile* pLDTFile;
	g_pResourceManager->GetLDTFromFileName(RES_FNAME_LDT_MAP_INFO, &pLDTFile);	

	if(pLDTFile == NULL) {
		return false;
	}
	
	int iItemID = 0;
	int iRecordCount = pLDTFile->GetItemCount();
	
	LDT_Field ldtFieldID;
	LDT_Field ldtFieldName;	
	LDT_Field ldtFieldTimeScan;
	LDT_Field ldtFieldTimeOpen1;
	LDT_Field ldtFieldTimeOff1;
	LDT_Field ldtFieldTimeOpen2;
	LDT_Field ldtFieldTimeOff2;
	LDT_Field ldtFieldTimeOpen3;
	LDT_Field ldtFieldTimeOff3;
	LDT_Field ldtFieldTimeOpen4;
	LDT_Field ldtFieldTimeOff4;
	LDT_Field ldtFieldTimeOpen5;
	LDT_Field ldtFieldTimeOff5;

	for(int i = 0; i < iRecordCount; i++) {
		MAPINFO_LDT* pMapInfo = new MAPINFO_LDT;
		
		iItemID = pLDTFile->GetPrimaryKey( i );
		pMapInfo->m_iID = iItemID;

		pLDTFile->GetFieldFromLable( iItemID, "_Map",					ldtFieldID);
		pLDTFile->GetFieldFromLable( iItemID, "_Name",					ldtFieldName);
		pLDTFile->GetFieldFromLable( iItemID, "_TimeScan",				ldtFieldTimeScan);
		pLDTFile->GetFieldFromLable( iItemID, "_Time1_open",			ldtFieldTimeOpen1);
		pLDTFile->GetFieldFromLable( iItemID, "_Time1_off",				ldtFieldTimeOff1);
		pLDTFile->GetFieldFromLable( iItemID, "_Time2_open",			ldtFieldTimeOpen2);
		pLDTFile->GetFieldFromLable( iItemID, "_Time2_off",				ldtFieldTimeOff2);
		pLDTFile->GetFieldFromLable( iItemID, "_Time3_open",			ldtFieldTimeOpen3);
		pLDTFile->GetFieldFromLable( iItemID, "_Time3_off",				ldtFieldTimeOff3);
		pLDTFile->GetFieldFromLable( iItemID, "_Time4_open",			ldtFieldTimeOpen4);
		pLDTFile->GetFieldFromLable( iItemID, "_Time4_off",				ldtFieldTimeOff4);
		pLDTFile->GetFieldFromLable( iItemID, "_Time5_open",			ldtFieldTimeOpen5);
		pLDTFile->GetFieldFromLable( iItemID, "_Time5_off",				ldtFieldTimeOff5);

		if ( ldtFieldID.uData.pValue ) {
			pMapInfo->m_strMapID = ldtFieldID.uData.pValue;
		}

		if ( ldtFieldName.uData.pValue ) {
			pMapInfo->m_strMapName = ldtFieldName.uData.pValue;
		}

		pMapInfo->m_TimeScan  = ldtFieldTimeScan.uData.lValue;

		pMapInfo->m_TimeOpen[0] = ldtFieldTimeOpen1.uData.lValue;
		pMapInfo->m_TimeOff[0]  = ldtFieldTimeOff1.uData.lValue;
		pMapInfo->m_TimeOpen[1] = ldtFieldTimeOpen2.uData.lValue;
		pMapInfo->m_TimeOff[1]  = ldtFieldTimeOff2.uData.lValue;
		pMapInfo->m_TimeOpen[2] = ldtFieldTimeOpen3.uData.lValue;
		pMapInfo->m_TimeOff[2]  = ldtFieldTimeOff3.uData.lValue;
		pMapInfo->m_TimeOpen[3] = ldtFieldTimeOpen4.uData.lValue;
		pMapInfo->m_TimeOff[3]  = ldtFieldTimeOff4.uData.lValue;
		pMapInfo->m_TimeOpen[4] = ldtFieldTimeOpen5.uData.lValue;
		pMapInfo->m_TimeOff[4]  = ldtFieldTimeOff5.uData.lValue;

		m_vpMapInfoLdt.push_back(pMapInfo);
	}

	SAFE_RELEASE(pLDTFile);
	return true;
}

bool SPEventArchive::isMapOpenTime(UINT32 iEventID)
{
	EVENTPORTAL_INFO * pEventPortal = GetEvevtPortal(iEventID);
	if ( pEventPortal == false )
		return true;

	MAPINFO_LDT*  pMapInfo = GetMapInfo(pEventPortal->m_iStageID, pEventPortal->m_iMapGroupID);
	if ( pMapInfo == false )
		return true;

	if ( pMapInfo->m_TimeScan == 0 )
		return true;

	CTime  tmTime = CTime::GetCurrentTime();
	int  iLocalHour		= tmTime.GetHour();
	int  iLocalMinute	= tmTime.GetMinute();
	for ( int i=0; i<5; i++ )
	{
		int  iStartHour		= pMapInfo->m_TimeOpen[i]/100;
		int  iStartMinute	= pMapInfo->m_TimeOpen[i]%100;
		int  iEndHour		= pMapInfo->m_TimeOff[i]/100;
		int  iEndMinute		= pMapInfo->m_TimeOff[i]%100;

		if ( iLocalHour >= iStartHour && iLocalHour <= iEndHour
			&& (iLocalHour == iStartHour ? iLocalMinute > iStartMinute : true)
			&& (iLocalHour == iEndHour ? iLocalMinute < iEndMinute : true) )
			return true;
	}
	return false;
}

MAPINFO_LDT* SPEventArchive::GetMapInfo(int iStageID, int iMapGroupID)
{
	int iStageIndex		= 0;
	int iMapGroupIndex	= 0;

	std::vector<MAPINFO_LDT*>::iterator Iter = m_vpMapInfoLdt.begin();
	for ( ; Iter != m_vpMapInfoLdt.end(); Iter++ )
	{
		if ( sscanf((*Iter)->m_strMapID.c_str(), "%d-%d", &iStageIndex, &iMapGroupIndex) == 2
			&& iStageIndex == iStageID && iMapGroupIndex == iMapGroupID )
			return (*Iter);
	}

	return NULL;
}


//////////////////////////////////////////////////////////////////////////
void SPEventArchive::DeleteAllEvevtPortal()
{
	std::map<UINT, EVENTPORTAL_INFO*>::iterator Iter = m_mEventPortal.begin();
	for(;Iter != m_mEventPortal.end(); ++Iter) {
		SAFE_DELETE(Iter->second);
	}
	m_mEventPortal.clear();
}

bool SPEventArchive::LoadEvevtPortalLDT()
{
	SPLDTFile* pLDTFile;
	g_pResourceManager->GetLDTFromFileName(RES_FNAME_LDT_EVENT_PORTAL, &pLDTFile);	

	if(pLDTFile == NULL) {
		return false;
	}
	
	int iItemID = 0;
	int iRecordCount = pLDTFile->GetItemCount();
	
	LDT_Field ldtFieldStageID;
	LDT_Field ldtFieldMapGroupID;	
	LDT_Field ldtFieldMapID;
	LDT_Field ldtFieldPosX;
	LDT_Field ldtFieldPosY;

	for(int i = 0; i < iRecordCount; i++)
	{
		EVENTPORTAL_INFO* pEventPortal = new EVENTPORTAL_INFO;
		
		iItemID = pLDTFile->GetPrimaryKey( i );
		pEventPortal->m_iID = iItemID;

		pLDTFile->GetFieldFromLable( iItemID, "_StageID",			ldtFieldStageID);
		pLDTFile->GetFieldFromLable( iItemID, "_MapGroupID",		ldtFieldMapGroupID);
		pLDTFile->GetFieldFromLable( iItemID, "_MapID",				ldtFieldMapID);
		pLDTFile->GetFieldFromLable( iItemID, "_PosX",				ldtFieldPosX);
		pLDTFile->GetFieldFromLable( iItemID, "_PosY",				ldtFieldPosY);

		pEventPortal->m_iStageID		= ldtFieldStageID.uData.lValue;
		pEventPortal->m_iMapGroupID		= ldtFieldMapGroupID.uData.lValue;
		pEventPortal->m_iMapID			= ldtFieldMapID.uData.lValue;
		pEventPortal->m_fPosX			= (float)ldtFieldPosX.uData.lValue;
		pEventPortal->m_fPosY			= (float)ldtFieldPosY.uData.lValue;

		m_mEventPortal[pEventPortal->m_iID] = pEventPortal;
	}

	SAFE_RELEASE(pLDTFile);
	return true;
}

EVENTPORTAL_INFO* SPEventArchive::GetEvevtPortal(UINT32 iEventID)
{
	std::map<UINT, EVENTPORTAL_INFO*>::iterator Iter = m_mEventPortal.find(iEventID);
	if ( Iter != m_mEventPortal.end() ) 
		return Iter->second;
	return NULL;
}


//////////////////////////////////////////////////////////////////////////
void SPEventArchive::DeleteAllNpc()
{
	std::vector<SPNpc*>::iterator Iter = m_vpNpcInfo.begin();
	for(;Iter != m_vpNpcInfo.end(); ++Iter) {
		SAFE_DELETE(*Iter);
	}
	m_vpNpcInfo.clear();
}


bool SPEventArchive::LoadNpcLDT()
{
	SPLDTFile* pLDTFile;
	g_pResourceManager->GetLDTFromFileName(RES_FNAME_LDT_NPC, &pLDTFile);	

	if(pLDTFile == NULL) {
		return false;
	}

	int iRecordCount = pLDTFile->GetItemCount();
	int iItemID = 0;
	
	LDT_Field ldtFieldCutImage;
	LDT_Field ldtFieldName;
	LDT_Field ldtFieldMessage;	

	LDT_Field ldtEventType1;
	LDT_Field ldtTypeID1;
	LDT_Field ldtTypeMenu1;
	LDT_Field ldtEventType2;
	LDT_Field ldtTypeID2;
	LDT_Field ldtTypeMenu2;
	LDT_Field ldtEventType3;
	LDT_Field ldtTypeID3;
	LDT_Field ldtTypeMenu3;
	LDT_Field ldtEventType4;
	LDT_Field ldtTypeID4;
	LDT_Field ldtTypeMenu4;

	for(int i = 0; i < iRecordCount; i++) {
		SPNpc* pNpc = new SPNpc;
		iItemID = pLDTFile->GetPrimaryKey( i );

		pLDTFile->GetFieldFromLable( iItemID, "_CutInImage",	ldtFieldCutImage);
		pLDTFile->GetFieldFromLable( iItemID, "_Npcname",		ldtFieldName); 
		pLDTFile->GetFieldFromLable( iItemID, "_Message",		ldtFieldMessage); 		

		pLDTFile->GetFieldFromLable( iItemID, "_EventType1",	ldtEventType1); 
		pLDTFile->GetFieldFromLable( iItemID, "_TypeID1",		ldtTypeID1); 
		pLDTFile->GetFieldFromLable( iItemID, "_Text1",			ldtTypeMenu1); 

		pLDTFile->GetFieldFromLable( iItemID, "_EventType2",	ldtEventType2); 
		pLDTFile->GetFieldFromLable( iItemID, "_TypeID2",		ldtTypeID2); 
		pLDTFile->GetFieldFromLable( iItemID, "_Text2",			ldtTypeMenu2); 

		pLDTFile->GetFieldFromLable( iItemID, "_EventType3",	ldtEventType3); 	
		pLDTFile->GetFieldFromLable( iItemID, "_TypeID3",		ldtTypeID3); 
		pLDTFile->GetFieldFromLable( iItemID, "_Text3",			ldtTypeMenu3); 

		pLDTFile->GetFieldFromLable( iItemID, "_EventType4",	ldtEventType4); 
		pLDTFile->GetFieldFromLable( iItemID, "_TypeID4",		ldtTypeID4);
		pLDTFile->GetFieldFromLable( iItemID, "_Text4",			ldtTypeMenu4); 

		int iCutImage = ldtFieldCutImage.uData.lValue;
		
		std::string strName;
		strName.clear();
		if(ldtFieldName.uData.pValue) {
			strName = ldtFieldName.uData.pValue;
		}
		
		std::string strMessage;
		strMessage.clear();
		if(ldtFieldMessage.uData.pValue) {
			strMessage = ldtFieldMessage.uData.pValue;
		}

		int iType1 = ldtEventType1.uData.lValue;
		int iTypeID1 = ldtTypeID1.uData.lValue;
		std::string strMenu1;
		strMenu1.clear();
		if(ldtTypeMenu1.uData.pValue) {
			strMenu1 = ldtTypeMenu1.uData.pValue;
		}		

		int iType2 = ldtEventType2.uData.lValue;
		int iTypeID2 = ldtTypeID2.uData.lValue;
		std::string strMenu2;
		strMenu2.clear();
		if(ldtTypeMenu2.uData.pValue) {
			strMenu2 = ldtTypeMenu2.uData.pValue;
		}		

		int iType3 = ldtEventType3.uData.lValue;
		int iTypeID3 = ldtTypeID3.uData.lValue;
		std::string strMenu3;
		strMenu3.clear();
		if(ldtTypeMenu3.uData.pValue) {
			strMenu3 = ldtTypeMenu3.uData.pValue;
		}		

		int iType4 = ldtEventType4.uData.lValue;
		int iTypeID4 = ldtTypeID4.uData.lValue;
		std::string strMenu4;
		strMenu4.clear();
		if(ldtTypeMenu4.uData.pValue) {
			strMenu4 = ldtTypeMenu4.uData.pValue;
		}		

		pNpc->SetNpc(iItemID, iCutImage, 
			iType1, iTypeID1, strMenu1,
			iType2, iTypeID2, strMenu2,
			iType3, iTypeID3, strMenu3,
			iType4, iTypeID4, strMenu4,
			strName,
			strMessage);

		m_vpNpcInfo.push_back(pNpc);
	}

	SAFE_RELEASE(pLDTFile);
	return true;
}


SPNpc* SPEventArchive::GetNpc(int iItemID)
{
	std::vector<SPNpc*>::iterator Iter = 
		std::find_if(m_vpNpcInfo.begin(),
		m_vpNpcInfo.end(),
		std::bind2nd(IsEqualNpc,
		iItemID));

	if(Iter == m_vpNpcInfo.end())
		return NULL;

	return (*Iter);
}



//////////////////////////////////////////////////////////////////////////
void SPEventArchive::DeleteAllQuestList()
{
	std::vector<SPNpcQuestList*>::iterator Iter = m_vpQuestListInfo.begin();
	for(;Iter != m_vpQuestListInfo.end(); ++Iter) {
		SAFE_DELETE(*Iter);
	}
	m_vpQuestListInfo.clear();
}


bool SPEventArchive::LoadQuestListLDT()
{
	SPLDTFile* pLDTFile;
	g_pResourceManager->GetLDTFromFileName(RES_FNAME_LDT_NPCQUEST, &pLDTFile);	

	if(pLDTFile == NULL) {
		return false;
	}

	int iRecordCount = pLDTFile->GetItemCount();
	int iItemID = 0;

	LDT_Field ldtFieldCutIn;
	LDT_Field ldtFieldMsg;		
	LDT_Field ldtFieldQuestID;
	LDT_Field ldtFieldTitle;

	int iImage = 0;
	std::string strMsg = "";
	int iQuestID = 0;
	std::string strTitle = "";

	for(int i = 0; i < iRecordCount; i++) {
		SPNpcQuestList* pQuestList = new SPNpcQuestList;
		iItemID = pLDTFile->GetPrimaryKey( i );

		pLDTFile->GetFieldFromLable(iItemID, "_CutInImage",	ldtFieldCutIn);
		iImage = ldtFieldCutIn.uData.lValue;

		pLDTFile->GetFieldFromLable(iItemID, "_Message",	ldtFieldMsg);
		if(ldtFieldMsg.uData.pValue){
			strMsg = ldtFieldMsg.uData.pValue;
		}		

		pLDTFile->GetFieldFromLable(iItemID, "_Name",	ldtFieldTitle);
		if(ldtFieldTitle.uData.pValue) {
			strTitle = ldtFieldTitle.uData.pValue;
		}		

		//pQuestList->SetNpcQuest(m_iNpcID, iImage, strMsg, strTitle);
		pQuestList->SetNpcQuest(iItemID, iImage, strMsg, strTitle);

		char szFieldQuestID[64];	
		for(int j = 0; j < MAX_QUEST_LIST; j++) {
			ZeroMemory(szFieldQuestID, 64);
			wsprintf(szFieldQuestID, "_QuestID%d", j+1);		
			pLDTFile->GetFieldFromLable(iItemID, szFieldQuestID,	ldtFieldQuestID);
			iQuestID = ldtFieldQuestID.uData.lValue;

			if(iQuestID == 0)
				break;

			pQuestList->m_iQuestID[j] = iQuestID;
			pQuestList->m_iQuestCount++;
		}

		m_vpQuestListInfo.push_back(pQuestList);
	}				
	
	SAFE_RELEASE(pLDTFile);
	return true;
}


SPNpcQuestList*	SPEventArchive::GetQuestList(int iItemID)
{
	std::vector<SPNpcQuestList*>::iterator Iter = 
		std::find_if(m_vpQuestListInfo.begin(),
		m_vpQuestListInfo.end(),
		std::bind2nd(IsEqualQuestList,
		iItemID));

	// 찾은게 널인햨E확인!!! [1/26/2006 AJJIYA]
	if( Iter == m_vpQuestListInfo.end() )
		return NULL;

	return (*Iter);
}




//////////////////////////////////////////////////////////////////////////
void SPEventArchive::DeleteAllQuestAttr()
{
	std::vector<SPQuestAttr*>::iterator Iter = m_vpQuestAttrInfo.begin();
	for(;Iter != m_vpQuestAttrInfo.end(); ++Iter) {
		SAFE_DELETE(*Iter);
	}
	m_vpQuestAttrInfo.clear();
}


bool SPEventArchive::LoadQuestAttrLDT()
{
	SPLDTFile* pLDTFile;
	g_pResourceManager->GetLDTFromFileName(RES_FNAME_LDT_QUEST, &pLDTFile);	

	if(pLDTFile == NULL) {
		return false;
	}

	int iRecordCount = pLDTFile->GetItemCount();
	int iItemID = 0;
	
	for(int i = 0; i < iRecordCount; i++) {
		SPQuestAttr* pQuestAttr = new SPQuestAttr;
		iItemID = pLDTFile->GetPrimaryKey( i );

		g_pEventManager->GetQuestManager()->LoadLDTFile(iItemID, pQuestAttr);

		m_vpQuestAttrInfo.push_back(pQuestAttr);
	}
	
	SAFE_RELEASE(pLDTFile);
	return true;
}


SPQuestAttr* SPEventArchive::GetQuestAttr(int iItemID)
{
	std::vector<SPQuestAttr*>::iterator Iter = 
		std::find_if(m_vpQuestAttrInfo.begin(),
		m_vpQuestAttrInfo.end(),
		std::bind2nd(IsEqualQuestInfo,
		iItemID));	

	if(Iter == m_vpQuestAttrInfo.end())
		return NULL;

	return (*Iter);
}


bool SPEventArchive::LoadCoreEventInfoLDT()
{
	SPLDTFile* pLDTFile = NULL;
	g_pResourceManager->GetLDTFromFileName( RES_FNAME_LDT_EVENT_CORE_START , &pLDTFile);	
	if( pLDTFile == NULL )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "----------------------------------->CORE EVENT START LDT FILE LOAD FAIL [%s] \n" , RES_FNAME_LDT_EVENT_CORE_START );
#endif
		return false;
	}

	LDT_Field LDTField;
	int	iItemID = 0;
	int i = 0;
	int j = 0;
	
	int				iRecordCount	= pLDTFile->GetItemCount();
	SPCoreEventInfo*	pCoreEventInfo = NULL;

	for( i = 0; i < iRecordCount; ++i )
	{
		pCoreEventInfo = new SPCoreEventInfo;
		
		iItemID	=	pLDTFile->GetPrimaryKey( i );
		pCoreEventInfo->m_iID = iItemID;

		for(j = 0; j < MAX_CORE_REQUIRE; j++) {
			pLDTFile->GetField( iItemID , j, LDTField );				//_Inventory1
			pCoreEventInfo->m_iRequireID[j]	= LDTField.uData.lValue;
		}

		m_vpCoreEventInfo.push_back(pCoreEventInfo);
	}

	SAFE_RELEASE(pLDTFile);
	return true;
}


SPCoreEventInfo* SPEventArchive::GetCorEventInfo(int iItemID)
{
	std::vector<SPCoreEventInfo*>::iterator Iter = 
		std::find_if(m_vpCoreEventInfo.begin(),
		m_vpCoreEventInfo.end(),
		std::bind2nd(IsEqualCoreEventInfo,
		iItemID));
	
	if(Iter == m_vpCoreEventInfo.end())
		return NULL;
	
	return (*Iter);	
}


void SPEventArchive::DeleteAllCoreEventInfo()
{
	std::vector<SPCoreEventInfo*>::iterator Iter = m_vpCoreEventInfo.begin();
	for(;Iter != m_vpCoreEventInfo.end(); ++Iter) {
		SAFE_DELETE(*Iter);
	}
	m_vpCoreEventInfo.clear();
}


bool SPEventArchive::LoadCoreRequireAttrLDT()
{
	SPLDTFile* pLDTFile = NULL;
	g_pResourceManager->GetLDTFromFileName( RES_FNAME_LDT_EVENT_CORE_REQUIRE , &pLDTFile);	
	if( pLDTFile == NULL )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "----------------------------------->CORE EVENT REQUIRE LDT FILE LOAD FAIL [%s] \n" , RES_FNAME_LDT_EVENT_CORE_REQUIRE );
#endif
		return false;
	}
	
	LDT_Field LDTField;
	int	iItemID = 0;
	int i = 0;
	int j = 0;
	
	int				iRecordCount	= pLDTFile->GetItemCount();
	SPCoreRequireAttr	RequireAttr;

	for( i = 0; i < iRecordCount; ++i )
	{
		RequireAttr.Clear();
		
		iItemID	=	pLDTFile->GetPrimaryKey( i );
		
		RequireAttr.m_iID	=	iItemID;		
		
		pLDTFile->GetField( iItemID , 0, LDTField );				//_Inventory1
		RequireAttr.m_InvenReq[0].m_iContainerType	= (CONTAINER_TYPE)(LDTField.uData.lValue - 1);

		pLDTFile->GetField( iItemID , 1, LDTField );				//_InventoryCount1
		RequireAttr.m_InvenReq[0].m_iSize			= LDTField.uData.lValue;

		pLDTFile->GetField( iItemID , 2, LDTField );				//_Inventory1
		RequireAttr.m_InvenReq[1].m_iContainerType	= (CONTAINER_TYPE)(LDTField.uData.lValue - 1);

		pLDTFile->GetField( iItemID , 3, LDTField );				//_InventoryCount1
		RequireAttr.m_InvenReq[1].m_iSize			= LDTField.uData.lValue;

		pLDTFile->GetField( iItemID , 4, LDTField );				//_QuestCount
		RequireAttr.m_iReqQuestSize					= LDTField.uData.lValue;

		
		int iStat = 0;
		int iStatValue = 0;
		int iValue1 = 0;
		int iValue2 = 0;
		for(j = 0; j < MAX_CORE_EVENT_REQUIRE; j++) {
			iStat = 0;
			iStatValue = 0;
			iValue1 = 0;
			iValue2 = 0;
			
			pLDTFile->GetField( iItemID , 5 + (j * 4), LDTField );				//_Require1_Type
			iStat = LDTField.uData.lValue;

			pLDTFile->GetField( iItemID , 6 + (j * 4), LDTField );				//_Require1_ID
			iStatValue = LDTField.uData.lValue;
			
			pLDTFile->GetField( iItemID , 7 + (j * 4), LDTField );				//_Require1_Value1
			iValue1 = LDTField.uData.lValue;
			
			pLDTFile->GetField( iItemID , 8 + (j * 4), LDTField );				//_Require1_Value2
			iValue2 = LDTField.uData.lValue;

			RequireAttr.m_Require[j].SetRequireInfo(iStat, iStatValue, iValue1, iValue2);
		}

		pLDTFile->GetField( iItemID , 37, LDTField );				//_EventType
		RequireAttr.m_iEventType = (NPC_EVENT_TYPE)LDTField.uData.lValue;
		
		pLDTFile->GetField( iItemID , 38, LDTField );				//_EventType
		RequireAttr.m_iEventID = LDTField.uData.lValue;

		InsertCoreRequireAttr(RequireAttr);
	}	

	SAFE_RELEASE(pLDTFile);
	return true;
}


SPCoreRequireAttr*	SPEventArchive::GetCoreRequireAttr(int iItemID)
{
	std::map<int, SPCoreRequireAttr>::iterator iter = m_mCoreEventReqInfo.find(iItemID);

	if(iter != m_mCoreEventReqInfo.end())
		return &((*iter).second);

	return NULL;
}


bool SPEventArchive::InsertCoreRequireAttr(SPCoreRequireAttr& RequireAttr)
{
	if(GetCoreRequireAttr(RequireAttr.m_iID))
		return false;

	m_mCoreEventReqInfo.insert(std::map<int, SPCoreRequireAttr>::value_type(RequireAttr.m_iID, RequireAttr));	
	return true;
}


void SPEventArchive::DeleteAllCutInTalk()
{
	std::vector<SPCutInTalk*>::iterator			iter = m_vpCutInTalk.begin();
	for(; iter != m_vpCutInTalk.end(); ++iter) {
		SAFE_DELETE((*iter));
	}
	m_vpCutInTalk.clear();
}


bool SPEventArchive::LoadCutInTalkLDT()
{
	SPLDTFile* pLDTFile		= NULL;
	g_pResourceManager->GetLDTFromFileName( RES_FNAME_LDT_CUTIN_TALK , &pLDTFile);	
	if( pLDTFile == NULL )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "----------------------------------->NPC GUILD LDT FILE LOAD FAIL [%s] \n" , RES_FNAME_LDT_NPCGUILD );
#endif
		return false;
	}

	DeleteAllCutInTalk();	

	int	iItemID = 0;
	int i = 0;

	int iCutInImageFieldID	= pLDTFile->GetFieldNum("_CutInImage");
	int iMessageFieldID		= pLDTFile->GetFieldNum("_Message");
	LDT_Field ldtFieldImage;
	LDT_Field ldtFieldMessage;

	int			iRecordCount = pLDTFile->GetItemCount();
	SPCutInTalk*	pCutInTalk = NULL;
	for( i = 0; i < iRecordCount; ++i )
	{
		pCutInTalk = new SPCutInTalk;

		iItemID	=	pLDTFile->GetPrimaryKey( i );

		pCutInTalk->m_iID = iItemID;

		pLDTFile->GetField(iItemID, iCutInImageFieldID,		ldtFieldImage);		//"_CutInImage"
		pLDTFile->GetField(iItemID, iMessageFieldID,		ldtFieldMessage);	//"_Message"

		int iImage = ldtFieldImage.uData.lValue;	
		std::string iMessage = "";
		if(ldtFieldMessage.uData.pValue)
			iMessage = ldtFieldMessage.uData.pValue;

		pCutInTalk->m_iImageID = iImage;
		pCutInTalk->m_strMsg = iMessage;

		m_vpCutInTalk.push_back(pCutInTalk);
	}

	SAFE_RELEASE(pLDTFile);
	return true;
}


SPCutInTalk* SPEventArchive::GetCutInTalk(int iItemID)
{
	std::vector<SPCutInTalk*>::iterator Iter = 
		std::find_if(m_vpCutInTalk.begin(),
		m_vpCutInTalk.end(),
		std::bind2nd(IsEqualCutInTalk,
		iItemID));

	if(Iter == m_vpCutInTalk.end())
		return NULL;

	return (*Iter);	
}

//////////////////////////////////////////////////////////////////////////
////
//void SPEventArchive::DeleteAllSellItem()
//{
//	std::vector<SPSellItem*>::iterator iter;
//	iter = m_vpSellItem.begin();
//	for(; iter != m_vpSellItem.end(); ++iter) {
//		delete (*iter);
//	}
//
//	m_vpSellItem.clear();
//}
//
//
//bool SPEventArchive::LoadSellItemLDT()
//{
//	SPLDTFile* pLDTFile;
//	g_pResourceManager->GetLDTFromFileName(RES_FNAME_LDT_NPCTRADE_DETAIL, &pLDTFile);	
//
//	if(pLDTFile == NULL) {
//		return false;
//	}
//
//	int iRecordCount = pLDTFile->GetItemCount();
//	int iItemID = 0;
//
//	LDT_Field ldtFieldItemID;
//	LDT_Field ldtFieldRear;
//	LDT_Field ldtFieldStack;
//	LDT_Field ldtFieldSellFactor;
//	
//	for(int i = 0 ; i < iRecordCount ; i++) {
//		iItemID = pLDTFile->GetPrimaryKey( i );
//		if(iItemID == 0)
//			break;
//
//		SPSellItem* pSellItem = new SPSellItem;
//
//		pLDTFile->GetFieldFromLable( iItemID, "_ItemID",		ldtFieldItemID);
//		pLDTFile->GetFieldFromLable( iItemID, "_ItemRare",		ldtFieldRear);
//		pLDTFile->GetFieldFromLable( iItemID, "_ItemCount",		ldtFieldStack);
//		pLDTFile->GetFieldFromLable( iItemID, "_SellFactor",	ldtFieldSellFactor);
//
//		int iSellItemID = ldtFieldItemID.uData.lValue;
//		int iRear = ldtFieldRear.uData.lValue;
//		int iStack = ldtFieldStack.uData.lValue;
//		int iSellFactor = ldtFieldSellFactor.uData.lValue;
//
//		if(iSellItemID == 0)		break;
//
//		//m_pNpcShop->SetNpcShopSellItem(i, iItemID, iRear, iSellFactor);		
//		
//		pSellItem->SetSellItem(iItemID, iRear, iSellFactor);
//		m_vpSellItem.push_back(pSellItem);
//	}
//
//	return true;
//}
//
//
//SPSellItem*	SPEventArchive::GetSellItem(int iItemID)
//{
//	std::vector<SPSellItem*>::iterator Iter = 
//		std::find_if(m_vpSellItem.begin(),
//		m_vpSellItem.end(),
//		std::bind2nd(IsEqualSellItem,
//		iItemID));	
//
//	if(Iter == m_vpSellItem.end())
//		return NULL;
//
//	return (*Iter);
//}


bool SPEventArchive::LoadGlobalSysLDT()
{
	SPLDTFile* pLDTFile;
	g_pResourceManager->GetLDTFromFileName(RES_FNAME_LDT_GLOBAL_SYS, &pLDTFile);	

	if(pLDTFile == NULL) {
		return false;
	}

	int iRecordCount = pLDTFile->GetItemCount();
	int iItemID = 0;

	LDT_Field ldtField;

	pLDTFile->GetFieldFromLable(1, "_Check",	ldtField);
	m_iDeathItem = ldtField.uData.lValue;	

	pLDTFile->GetFieldFromLable(5, "_Check",	ldtField);
	m_iMicServer = ldtField.uData.lValue;	

	pLDTFile->GetFieldFromLable(6, "_Check",	ldtField);
	m_iMicChannel = ldtField.uData.lValue;	

	pLDTFile->GetFieldFromLable(12, "_Check",	ldtField);
	m_iMailFee = ldtField.uData.lValue;	

	pLDTFile->GetFieldFromLable(11, "_Check",	ldtField);
	m_iMailStampItem = ldtField.uData.lValue;	

	int iCashMajor;	
	pLDTFile->GetFieldFromLable(21, "_Check",	ldtField);
	iCashMajor = ldtField.uData.lValue;
	
	g_pItemCluster->SetCashVersion(iCashMajor, -1);
	
	SAFE_RELEASE(pLDTFile);
	return true;
}

