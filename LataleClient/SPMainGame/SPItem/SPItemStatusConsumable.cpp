#include "SPCommon.h"


#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPItemStatusConsumable.h"

SPItemStatusConsumable::SPItemStatusConsumable()
//:m_nItemCnt(0)
{
	m_ItemStatusID	= IS_CONSUMABLE;
	//m_nItemCnt = 1;
}

SPItemStatusConsumable::~SPItemStatusConsumable()
{

}

LRESULT SPItemStatusConsumable::SPSendMessage(MODEL_VALUE_TYPE mvMsg, LPARAM lParam )
{
	switch(mvMsg) 
	{
		//case MV_ITEM_GETCNT:		return (LRESULT)m_nItemCnt;				return 0;
		//case MV_ITEM_GETCNT:		return (LRESULT)m_iStackCount;				return 0;
		//case MV_ITEM_SETCNT:		m_nItemCnt = (int)lParam ;				return 0;		
		//case MV_ITEM_SETCNT:		m_iStackCount = (int)lParam;			return 0;

		case MV_ITEM_SETCONSUMENULL: SetNull(); return 0;
		case MV_ITEM_COPY_CONSUME:
			{
				SPItemStatusConsumable* pItemStatus = (SPItemStatusConsumable*)lParam;
				//if(pItemStatus->m_ItemStatusID == IS_CONSUMABLE) 
				{
					Copy( pItemStatus );
					return TRUE;
				}
				return FALSE;
			}
	}

	return SPItemStatus::SPSendMessage(mvMsg, lParam);
}

void SPItemStatusConsumable::SetNull()
{
	SPItemStatus::SetNull();

	//m_nItemCnt = 0;
}

bool SPItemStatusConsumable::Copy(SPItemStatusConsumable* pSrc)
{
	SPItemStatus::Copy(pSrc);

	//m_nItemCnt			= pSrc->m_nItemCnt;

	return true;
}
