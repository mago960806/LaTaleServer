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

#include "SPCheckManager.h"

#include "SPUserShopAttr.h"


SPUserShopAttr::SPUserShopAttr()
{
	Clear();
}


SPUserShopAttr::~SPUserShopAttr()
{
	Clear();	
}


void SPUserShopAttr::Init()
{
	Clear();	
}


void SPUserShopAttr::Clear()
{
	m_iID								= 0;
	m_iType								= USER_SHOP_LEVEL_NULL;							
	SetRect(&m_rtOverLapRect, 0, 0, 0, 0);
	
	for(int i = 0; i < MAX_USER_SHOP_REQUIRE; i++) {
		m_Require[i].Clear();
	}
	
	m_iPage				= 1;

	m_iNameWidth		= 0;
	m_iNameHeight		= 0;
	m_NameColor			= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_bNameBold			= false;

	m_iTitleWidth		= 0;
	m_iTitleHeight		= 0;
	m_TitleColor		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_bTitleBold		= false;

	m_SignBoard[MAX_USER_SHOP_FRAME];	
	m_iMotion = 58;
}


void SPUserShopAttr::SetOverLapRect(int iLeft, int iTop, int iRight, int iBottom)
{
	SetRect(&m_rtOverLapRect, iLeft, iTop, iRight, iBottom);
}


void SPUserShopAttr::SetNameColor(int iRed, int iGreen, int iBlue, int iAlpha /*= 255*/)
{
	m_NameColor = D3DXCOLOR(iRed/255, iGreen/255, iBlue/255, iAlpha/255);
}


void SPUserShopAttr::SetTitleColor(int iRed, int iGreen, int iBlue, int iAlpha /*= 255*/)
{
	m_TitleColor = D3DXCOLOR(iRed/255, iGreen/255, iBlue/255, iAlpha/255);
}


