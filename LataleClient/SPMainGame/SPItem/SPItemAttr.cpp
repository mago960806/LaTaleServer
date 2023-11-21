

#include "SPCommon.h"

#include "SPItemAttr.h"


SPItemAttr::SPItemAttr()
{
	SetNull();
}

SPItemAttr::~SPItemAttr()
{	
}


void SPItemAttr::SetNull()
{
	m_iCategory			= 0;
	m_iNum				= 0;
	m_iColorID			= 0;
	m_strName			.clear();
	m_strDescription	.clear();
	m_iIcon				= 0;
	m_iIconIndex		= 0;
	m_iGamePrice		= 0;
	m_iCashPrice		= 0;
	m_eRareLimit		= ITEM_RARE_NORMAL;
	m_eType				= ITEM_TYPE_NULL;
	m_eSubType			= ITEM_SUBTYPE_NULL;
	m_ePosID1			= ITEM_EQUIPPOS_NULL;
	m_eSubPosID11		= ITEM_EQUIPPOS_NULL;
	m_eSubPosID12		= ITEM_EQUIPPOS_NULL;
	m_ePosID2			= ITEM_EQUIPPOS_NULL;
	m_eSubPosID21		= ITEM_EQUIPPOS_NULL;
	m_eSubPosID22		= ITEM_EQUIPPOS_NULL;
	m_eKind				= ITEM_EQUIP_KIND_NULL;

	ZeroMemory(&m_StatusType, sizeof(m_StatusType));
	ZeroMemory(&m_StatusTypePvp, sizeof(m_StatusType));
	ZeroMemory(&m_RequireType, sizeof(m_RequireType));

	m_iCoolTime1		= 0;
	m_iCoolTime2		= 0;
	m_iCoolTimeApplyID1	= 0;
	m_iCoolTimeApplyID2	= 0;
	m_iItemCoolTimeID	= 0;
	m_bCashCheck		= false;
	m_iDurationTime		= 0;
	m_iStackLimit		= 0;
	m_iItemLv			= 0;
	m_bLogging			= false;
	m_bSerialized		= false;
	m_bTrade			= false;
	m_bDestroy			= false;
	m_bStorage			= false;
	m_bSell				= false;
	m_iItemPointMin		= 0;
	m_iItemPointMax		= 0;
	m_iOptionID1		= 0;
	m_iOptionID2		= 0;
	m_iOptionID3		= 0;
	m_iOptionID4		= 0;
	m_iMotionTypeID1	= 0;
	m_iMotionTypeID2	= 0;
	m_eIconMark			= ICON_MARK_NULL;
	m_bNameColor		= false;
	m_cRed				= 0;
	m_cGreen			= 0;
	m_cBlue				= 0;
	m_eDuration			= ITEM_DURATION_NULL;
	m_iLifeTime			= 0;

	m_iPetClassID		= 0;
	m_iPetMaxExp		= 0;
	
	ZeroMemory(m_iSkillID, sizeof(m_iSkillID));
	ZeroMemory(m_iSkillLevel, sizeof(m_iSkillLevel));	

	m_iSetID			= 0;
	m_iQuestID			= 0;
	m_bPvp				= false;
	m_iCashPoint		= 0;

	m_iItemLevel		= 0;
	m_iAuctionCategory	= 0;
	m_iAuctionQuality	= 0;
	m_iShopID			= 0;

	m_iCash				= 0;
	m_iPoint			= 0;
	m_iNewPet			= 0;
	m_bWedding			= false;
}


D3DXCOLOR SPItemAttr::GetNameColor()
{	
	if(m_bNameColor == false) {
		D3DXCOLOR defaultColor(1.0f, 1.0f, 1.0f, 1.0f);
		return defaultColor;
	}

	float fRed = m_cRed;
	float fGreen = m_cGreen;
	float fBlue = m_cBlue;
	D3DXCOLOR color((fRed / 255.0f), (fGreen / 255.0f), (fBlue / 255.0f), 1.0f);
	return color;
}


ITEM_DURATION SPItemAttr::GetDurationType()
{
	return m_eDuration;
}


bool SPItemAttr::GetConvertFigureItem(FIGURE_ITEM* pFigureItem)
{
	if( m_eType == ITEM_TYPE_FIGURE )
	{
		if( m_eSubType == ITEM_SUBTYPE_EQUIP_EQUIP )
			pFigureItem->SlotInfo.ContainerType = FIGURE_EQUIP;
		else if( m_eSubType == ITEM_SUBTYPE_EQUIP_FASHION )
			pFigureItem->SlotInfo.ContainerType = FIGURE_FASHION;
		else if (m_eSubType == ITEM_SUBTYPE_EQUIP_EFFECT)
			pFigureItem->SlotInfo.ContainerType = FIGURE_EFFECT;
	}
	else if( m_eType == ITEM_TYPE_CONSUME )
	{
		if( m_eSubType == ITEM_SUBTYPE_WASTE_DEFAULT )
			pFigureItem->SlotInfo.ContainerType = FIGURE_BASIC;
	}

	pFigureItem->SlotInfo.SlotIndex = m_ePosID1;
	return true;
}


void SPItemAttr::SetCash(int iCash)
{
	m_iCash = iCash;
}


void SPItemAttr::SetPoint(int iPoint)
{
	m_iPoint = iPoint;
}


