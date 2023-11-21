#include "SPCommon.h"
#include "SPUtil.h"

#include "PacketID.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPWindowDEF.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "SPItemAttr.h"
#include "SPItemCluster.h"

#include "SPCheckManager.h"

#include "SPHotItemInfo.h"


SPHotItemInfo::SPHotItemInfo()
: m_pCashShopAttr	(NULL)
, m_iCashShopID		(0)
, m_iToolTipLength	(0)
{	
	m_strToolTip.clear();
	//Init();

	for(int i = 0; i < MAX_CHECK_INVEN; i++) {
		m_CheckInven[i].Clear();
	}
}


SPHotItemInfo::SPHotItemInfo(CASHSHOP_HOT_INFO* pInfo)
{
	m_pCashShopAttr = NULL;
	Init(pInfo);
}


SPHotItemInfo::~SPHotItemInfo()
{		
	Clear();
}


void SPHotItemInfo::Clear()
{	
	//[xialin 2008/09/16]add
	m_pCashShopAttr = g_pItemCluster->GetMinorCashItem(m_iCashShopID);

	if(m_pCashShopAttr) {
		m_pCashShopAttr->ClearHotDesc();
	}
	m_pCashShopAttr = NULL;
}


void SPHotItemInfo::Init(CASHSHOP_HOT_INFO* pInfo)
{
	if(pInfo == NULL)
		return;
	
	for(int i = 0; i < MAX_CHECK_INVEN; i++) {
		m_CheckInven[i].Clear();
	}

	m_iCashShopID = pInfo->iShopID;
	m_pCashShopAttr = g_pItemCluster->GetMinorCashItem(m_iCashShopID);
	
	m_iToolTipLength = pInfo->iDescLength;
	m_strToolTip = pInfo->strDesc;	

	if(m_pCashShopAttr) {
		m_pCashShopAttr->SetHotDesc(pInfo->strDesc);
	}
}


SPCashItemAttr*	SPHotItemInfo::GetCashItemAttr()		
{	
	//[xialin 2008/09/16]add
	m_pCashShopAttr = g_pItemCluster->GetMinorCashItem(m_iCashShopID);
	return m_pCashShopAttr;	
}


bool SPHotItemInfo::IsShow()
{
	//[xialin 2008/09/16]add
	m_pCashShopAttr = g_pItemCluster->GetMinorCashItem(m_iCashShopID);

	if(m_pCashShopAttr && m_pCashShopAttr->bView) 
		return true;

	return false;
}


