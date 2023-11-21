
#include "SPCommon.h"

#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPRenderModelDEF.h"


#include "SPItemAttr.h"
#include "SPItemStatus.h"

//#include "SPItem.h"
#include "SPItemCluster.h"			//[2005/6/16] - jinhee


SPItemStatus::SPItemStatus()
: m_ItemStatusID(IS_PARENT)
//m_nItemUID(0),
//m_iItemSlotIndex(0),
, m_iLifeTime(0)
, m_iStackCount(1)
, m_pItemAttr	(NULL)
, m_iItemID		(0)
, m_iRare(0)
, m_iBackPrice(0)
, m_iItemNo(0)
, m_iOptPoint(0)
, m_bTimeAvailable(true)
{	 
	m_strHotDesc.clear();
	m_strLotteryName.clear();
	ZeroMemory(m_Opt, sizeof(m_Opt));	
}

SPItemStatus::SPItemStatus(ITEMSTATUS_ID itemID)
//: m_nItemUID(0)
//m_ItemStatusID(itemID),
//m_iItemSlotIndex(0),
: m_pItemAttr (NULL)
, m_iLifeTime(0)
, m_iItemID		(0)
, m_iStackCount(1)
, m_iRare(0)
, m_iBackPrice(0)
, m_iItemNo(0)
, m_iOptPoint(0)
, m_bTimeAvailable(true)
, m_iItemExp(0)
, m_iItemSP(0)
{
	m_strHotDesc.clear();
	m_strLotteryName.clear();
	ZeroMemory(m_Opt, sizeof(m_Opt));
}


SPItemStatus::SPItemStatus( const SPItemStatus& src )
{
	*this = src;
}

SPItemStatus::~SPItemStatus()
{}


bool SPItemStatus::IsNULL()
{
	return (m_iItemID == 0);
}

int SPItemStatus::GetLifeTime()
{
	return m_iLifeTime;
}

void SPItemStatus::SetLifeTime(int iLifeTime)
{
	m_iLifeTime = iLifeTime;
	if(GetItemAttr() && GetItemAttr()->GetDurationType() != ITEM_DURATION_NULL) {
		m_bTimeAvailable = true;
	}
}


void SPItemStatus::SetExp(INT64 iExp)
{
	m_iItemExp = iExp;
}


INT64 SPItemStatus::GetExp()
{
	return m_iItemExp;
}


void SPItemStatus::SetSp(INT16 iSp)
{
	m_iItemSP = iSp;
}


INT16 SPItemStatus::GetSp()
{
	return m_iItemSP;
}


void SPItemStatus::SetTimeAvailable(bool bAvailable /*= false*/)
{
	m_bTimeAvailable = bAvailable;
}

bool SPItemStatus::IsTimeAvailable()
{
	return m_bTimeAvailable;
}


void SPItemStatus::SetHotDesc(std::string strDesc)
{
	m_strHotDesc.clear();
	m_strHotDesc = strDesc;
}

void SPItemStatus::SetLotteryName(std::string strName)
{
	m_strLotteryName.clear();
	m_strLotteryName = strName;
}


void SPItemStatus::DecreaseLifeTime(int iTime)
{
	if(m_iLifeTime > 0)
		m_iLifeTime -= iTime;
}

bool SPItemStatus::SetItemID(UINT32 iItemID)
{
	if( g_pItemCluster->GetItemInfo(iItemID, m_pItemAttr) ) {
		m_iItemID = iItemID;
		return true;
	}
	return false;
}

UINT32 SPItemStatus::GetItemID()
{
	return m_iItemID;
}

SPItemAttr*	SPItemStatus::GetItemAttr()
{
	if(m_iItemID != 0)
		return m_pItemAttr;

	return NULL;	
}

int SPItemStatus::GetRareLevel()
{
	return m_iRare; 
}

PACK_OPT* SPItemStatus::GetOption(int iOrder)
{
	if(iOrder < 0 || iOrder >= MAX_ITEM_OPT)
		return NULL;
	return &(m_Opt[iOrder]);
}

int SPItemStatus::GetOptionCount()
{
	int iCount = 0;
	for(int i = 0; i < MAX_ITEM_OPT; ++i) {
		if(m_Opt[i].iID != 0) {
			++iCount;
		}
	}
	return iCount;
}

int SPItemStatus::GetStatusCount()
{
	int iCount = 0;
	for(int i = 0; i < MAX_ITEM_OPT; ++i) {
		if(m_pItemAttr->m_StatusType[i].Effect != ITEM_EQUIP_EFFECT_NULL) {
			++iCount;
		}
	}
	return iCount;
}

int SPItemStatus::GetStackCount()
{
	return m_iStackCount;
}

LRESULT SPItemStatus::SPSendMessage(MODEL_VALUE_TYPE mvMsg, LPARAM lParam)
{
	switch(mvMsg) 
	{
		case MV_ITEM_GETSTATUSID:	return (LRESULT)m_ItemStatusID;
		case MV_ITEM_SETSTATUSID:	m_ItemStatusID = (ITEMSTATUS_ID)lParam; return 0;
		
		case MV_ITEM_GETUID:		return (LRESULT)m_iItemID;
		case MV_ITEM_SETUID:		m_iItemID = (int)lParam;				return 0;		

		case MV_ITEM_GETINDEX:		return (LRESULT)m_iItemSlotIndex;
		case MV_ITEM_SETINDEX:		m_iItemSlotIndex = (int)lParam;			return 0;		

		case MV_ITEM_GETSTACK_CNT:	return (LRESULT)m_iStackCount;
		case MV_ITEM_SETSTACK_CNT:  m_iStackCount = (int)lParam; return 0;

		case MV_ITEM_GET_RARELEVEL: return (LRESULT)m_iRare;
		case MV_ITEM_SET_RARELEVEL: m_iRare = (int)lParam; return 0;

		case MV_ITEM_GET_OPTION:	return (LRESULT)m_Opt;
		case MV_ITEM_SET_OPTION:	CopyMemory(m_Opt, (PACK_OPT*)lParam, sizeof(m_Opt)); return 0;		

		case MV_ITEM_GET_BACKPRICE:	return (LRESULT)m_iBackPrice;
		case MV_ITEM_SET_BACKPRICE:	m_iBackPrice = (int)lParam;			return 0;		

		case MV_ITEM_GET_ITEMNO:	return (LRESULT)&m_iItemNo;
		case MV_ITEM_SET_ITEMNO:	m_iItemNo = *((ITEMNO*)lParam);		return 0;

		case MV_ITEM_GET_OPTPOINT:	return (LRESULT)m_iOptPoint;
		case MV_ITEM_SET_OPTPOINT:	m_iOptPoint = (int)lParam;			return 0;		
	
		case MV_ITEM_GET_LIFETIME:  return (LRESULT)m_iLifeTime;
		case MV_ITEM_SET_LIFETIME:  m_iLifeTime = (int)lParam;			return 0;
	}
	return -1;
}

void SPItemStatus::SetNull()
{
	m_ItemStatusID = IS_NULL ;

	m_iItemSlotIndex = 0;
	m_iItemID = 0 ;

	//m_nSrcPosX = m_nSrcPosY = 0;
	m_iStackCount = 1;
	m_iItemNo = 0;
	m_iItemExp = 0;
	m_iItemSP = 0;
}

bool SPItemStatus::Copy(SPItemStatus* pSrc)
{
	m_iItemID			= pSrc->m_iItemID;	
	m_pItemAttr			= pSrc->m_pItemAttr;

	m_ItemStatusID 		= pSrc->m_ItemStatusID 	;
						
	m_iItemSlotIndex	= pSrc->m_iItemSlotIndex;
						
	m_iStackCount		= pSrc->m_iStackCount	;
	m_iLifeTime			= pSrc->m_iLifeTime		;
	m_iBackPrice		= pSrc->m_iBackPrice;
	m_iItemNo			= pSrc->m_iItemNo;
	m_iOptPoint			= pSrc->m_iOptPoint;

	m_iRare				= pSrc->m_iRare;
	memcpy(&m_Opt, &(pSrc->m_Opt), sizeof(m_Opt));
	
	m_bTimeAvailable	= pSrc->m_bTimeAvailable;
	m_iItemExp			= pSrc->m_iItemExp;
	m_iItemSP			= pSrc->m_iItemSP;
	return true;
}






