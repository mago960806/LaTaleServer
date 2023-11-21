

#include "SPCommon.h"

#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPRenderModelDEF.h"

#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPItemStatusEquip.h"


SPItemStatusEquip::SPItemStatusEquip(EQUIP_ID	EquipID)
:SPItemStatus()
, m_EquipID			(EquipID)
, m_EquipClass		(EQCLASS_NULL)
, m_iEquipItemID	(0)
, m_iEquipItemColor	(0)
{	
	m_ItemStatusID = IS_EQUIP;
}

SPItemStatusEquip::~SPItemStatusEquip()
{
}

void SPItemStatusEquip::SetNull()
{	
	SPItemStatus::SetNull();

	m_EquipID		= (EQUIP_ID)		EQ_NULL;
	m_EquipClass	= (EQUIP_CLASS)		EQCLASS_NULL;
	m_iEquipItemID	= (int)				0;
	m_iEquipItemColor	= (int)			0;
}

LRESULT SPItemStatusEquip::SPSendMessage(MODEL_VALUE_TYPE mvMsg, LPARAM lParam)
{
	switch(mvMsg) 
	{
		case MV_ITEM_GETEQUIPID		:	return (LRESULT)m_EquipID; 	
		case MV_ITEM_SETEQUIPID		:	m_EquipID	= (EQUIP_ID)lParam; return 0;	
		
		case MV_ITEM_GETEQUIPITEMID	:	return (LRESULT)m_iEquipItemID; 
		case MV_ITEM_SETEQUIPITEMID :	m_iEquipItemID	= (int)lParam;	return 0;
		
		case MV_ITEM_GETCOLOR		:	return (LRESULT)m_iEquipItemColor; 
		case MV_ITEM_SETCOLOR		:	m_iEquipItemColor = (int)lParam; return 0;		

		case MV_ITEM_SETEQUIPCLASS	:   m_EquipClass = (EQUIP_CLASS)lParam; return 0; 
		case MV_ITEM_GETEQUIPCLASS	:   return m_EquipClass;

		case MV_ITEM_SETEQUIPNULL	:	SetNull();				return 0; 

		case MV_ITEM_ISNULL			:	return (m_EquipID == EQ_NULL) ? TRUE : FALSE;		
		case MV_ITEM_COPYEQUIPSTATUS :   
		{
			SPItemStatusEquip* pItemStatus = (SPItemStatusEquip*)lParam;
			{
				Copy( pItemStatus );
				return TRUE;
			}
			return FALSE;
		}
	}
	return SPItemStatus::SPSendMessage(mvMsg, lParam);
}

bool SPItemStatusEquip::Copy(SPItemStatusEquip* pSrc)
{
	SPItemStatus::Copy( pSrc );
	
	m_EquipID		= (EQUIP_ID)		pSrc->SPSendMessage(MV_ITEM_GETEQUIPID);
	m_EquipClass	= (EQUIP_CLASS)		pSrc->SPSendMessage(MV_ITEM_GETEQUIPCLASS);
	m_iEquipItemID	= (int)				pSrc->SPSendMessage(MV_ITEM_GETEQUIPITEMID);
	m_iEquipItemColor	= (int)			pSrc->SPSendMessage(MV_ITEM_GETCOLOR);	

	return true;
}









