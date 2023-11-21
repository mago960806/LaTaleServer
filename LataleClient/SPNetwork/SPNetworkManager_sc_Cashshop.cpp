
#include "SPCommon.h"
#include "SPManagerDef.h"
#include "SPSubGameManager.h"
#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"

#include "SPNetworkManager.h"
#include "SPCashTradeManager.h"

DEFINE_SC_PACKET_PROC(CASHSHOP_SC_IN)
{
	WPARAM wParam = 1;
	LPARAM lParam = (LPARAM)packet;

	if(g_pSubGameManager)
		g_pSubGameManager->SPPerformMessage(SPGM_CASHSHOP_IN_RECEIVE, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(CASHSHOP_SC_OUT)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if(g_pSubGameManager)
		g_pSubGameManager->SPPerformMessage(SPGM_CASHSHOP_IN_RECEIVE, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(CASHSHOP_SC_INFO)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if(g_pSubGameManager)
		g_pSubGameManager->SPPerformMessage(SPGM_CASHSHOP_INFO, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(CASHSHOP_SC_CASH)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if(g_pSubGameManager)
		g_pSubGameManager->SPPerformMessage(SPGM_CASHSHOP_UPDATE, wParam, lParam);
}


DEFINE_SC_PACKET_PROC(CASHSHOP_SC_BUY)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	//if(g_pSubGameManager)
	//	g_pSubGameManager->SPPerformMessage(SPGM_CASHSHOP_BUY, wParam, lParam);

	SPCashTradeManager::GetInstance()->OnCashShopBuy(packet);
}

DEFINE_SC_PACKET_PROC(CASHSHOP_SC_BUY_PRESENT)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	//if(g_pSubGameManager)
	//	g_pSubGameManager->SPPerformMessage(SPGM_CASHSHOP_BUY_GIFT, wParam, lParam);

	SPCashTradeManager::GetInstance()->OnCashShopGift(packet);
}

DEFINE_SC_PACKET_PROC(CASHSHOP_SC_SELL)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if(g_pSubGameManager)
		g_pSubGameManager->SPPerformMessage(SPGM_CASHSHOP_SELL, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(CASHSHOP_SC_COUPON)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if(g_pSubGameManager)
		g_pSubGameManager->SPPerformMessage(SPGM_CASHSHOP_COUPON, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(CASHSHOP_SC_WISH_LIST)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if(g_pSubGameManager)
		g_pSubGameManager->SPPerformMessage(SPGM_CASHSHOP_CART_LIST, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(CASHSHOP_SC_WISH_ADD)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	//if(g_pSubGameManager)
	//	g_pSubGameManager->SPPerformMessage(SPGM_CASHSHOP_CART_ADD, wParam, lParam);
	
	SPCashTradeManager::GetInstance()->OnCashWishAdd(packet);
}

DEFINE_SC_PACKET_PROC(CASHSHOP_SC_WISH_DEL)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if(g_pSubGameManager)
		g_pSubGameManager->SPPerformMessage(SPGM_CASHSHOP_CART_DEL, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(CASHSHOP_SC_MINOR_INIT)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if(g_pSubGameManager)
		g_pSubGameManager->SPPerformMessage(SPGM_CASHSHOP_ITEM_INIT, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(CASHSHOP_SC_MINOR_INFO)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	//if(g_pSubGameManager)
	//	g_pSubGameManager->SPPerformMessage(SPGM_CASHSHOP_ITEM_UPDATE, wParam, lParam);

	SPCashTradeManager::GetInstance()->OnCashShopItemUpdate(packet);
}

DEFINE_SC_PACKET_PROC(CASHSHOP_SC_OUT_READY)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if(g_pSubGameManager)
		g_pSubGameManager->SPPerformMessage(SPGM_CASHSHOP_OUT_READY, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(CASHSHOP_SC_VERSION)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;
	
	SPCashTradeManager::GetInstance()->OnCashShopVersion(packet);
}

DEFINE_SC_PACKET_PROC(CASHSHOP_SC_PRICE)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;	

	SPCashTradeManager::GetInstance()->OnCashShopPrice(packet);
}

DEFINE_SC_PACKET_PROC(CASHSHOP_SC_HOT_INFO)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	SPCashTradeManager::GetInstance()->OnCashShopHotItemUpdate(packet);
}


DEFINE_SC_PACKET_PROC(CASHSHOP_SC_HOT_STATUS)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;	

	SPCashTradeManager::GetInstance()->OnSetHotItemStatus(packet);
}


//[xialin 2008/07/30]add
DEFINE_SC_PACKET_PROC( CASHSHOP_SC_ASK_BUY			)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if(g_pSubGameManager)
		g_pSubGameManager->SPPerformMessage(SPGM_CASHSHOP_BUY_EXACT, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(CASHSHOP_SC_EFFECT)
{

	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if(g_pSubGameManager)
		g_pSubGameManager->SPPerformMessage(SPGM_CASHSHOP_PARCEL, wParam, lParam);

}

//[xialin 2008/07/30]add
DEFINE_SC_PACKET_PROC( CASHSHOP_SC_ASK_BUY_RECV	)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;

	if(g_pSubGameManager)
		g_pSubGameManager->SPPerformMessage(SPGM_CASHSHOP_BEEXACT, wParam, lParam);

}

DEFINE_SC_PACKET_PROC(CASHSHOP_SC_GET_QUICK_BUY_INFO)
{
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)packet;	

	if(g_pSubGameManager)
		g_pSubGameManager->SPPerformMessage(SPGM_CASHSHOP_QB_INFO, wParam, lParam);

}
