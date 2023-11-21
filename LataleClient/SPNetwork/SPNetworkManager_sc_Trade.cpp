
#include "SPCommon.h"
//
#include "SPGameObjectDEF.h"
#include "SPManagerDef.h"
#include "SPManager.h"
#include "SPGOBManager.h"

//
#include "SPGameObject.h"
#include "SPGOBStatus.h"
#include "SPGOBClusterDef.h"
#include "SPGOBCluster.h"
#include "SPGobModelUnit.h"
#include "SPAvatarModelUnitDef.h"
#include "SPAvatarModelUnit.h"
#include "SPPlayer.h"

#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"

#include "SPNetworkManager.h"

extern SPNetworkManager* g_pNetworkManager;


DEFINE_SC_PACKET_PROC(TRADE_SC_REQUEST)
{
	GU_ID				ObjectGUID;					// 게임오브젝트아이디
	// GU_ID			SrcCharGUID;				// 거래요청자
	// char				szSrcCharName[LEN_NAME+1];	// 거래요청자이름
	// GU_ID			DstCharGUID;				// 거래대상자
	// char				szDstCharName[LEN_NAME+1];	// 거래대상자이름
	// GOLBAL_STRING_ID	ErrorID;					// 에러아이디(0이면 에러없음)

	packet->ExtractUINT64(&ObjectGUID);
	//packet->ExtractUINT64(&SrcCharGUID);
	//packet->ExtractData(szSrcCharName, LEN_NAME+1);
	//packet->ExtractUINT64(&DstCharGUID);
	//packet->ExtractData(szDstCharName, LEN_NAME+1);
	//packet->ExtractUINT32(&ErrorID);

	WPARAM wParam = (WPARAM)(&ObjectGUID);
	LPARAM lParam = (LPARAM)packet;

	g_pGOBManager->SPPerformMessage(SPGM_TRADE_SC_REQUEST, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(TRADE_SC_RESPONSE)
{
	GU_ID				ObjectGUID;					// 게임오브젝트아이디
	// GU_ID			SrcCharGUID;				// 거래요청자
	// char				szSrcCharName[LEN_NAME+1];	// 거래요청자이름
	// GU_ID			DstCharGUID;				// 거래대상자
	// char				szDstCharName[LEN_NAME+1];	// 거래대상자이름
	// UINT8			byAccept;					// 수락/거절
	// GOLBAL_STRING_ID	ErrorID;					// 에러아이디(0이면 에러없음)

	packet->ExtractUINT64(&ObjectGUID);
	//packet->ExtractUINT64(&SrcCharGUID);
	//packet->ExtractData(szSrcCharName, LEN_NAME+1);
	//packet->ExtractUINT64(&DstCharGUID);
	//packet->ExtractData(szDstCharName, LEN_NAME+1);
	//packet->ExtractUINT8(&byAccept);
	//packet->ExtractUINT32(&ErrorID);
	
	WPARAM wParam = (WPARAM)(&ObjectGUID);
	LPARAM lParam = (LPARAM)packet;
	
	g_pGOBManager->SPPerformMessage(SPGM_TRADE_SC_RESPONSE, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(TRADE_SC_PUSHMONEY)
{
	GU_ID			ObjectGUID;		// 게임오브젝트아이디
	//GU_ID			ActCharGUID;	// 돈을 올린 아바타의 키
	//UINT64		uiMoney;		// 올린 돈

	packet->ExtractUINT64(&ObjectGUID);
	//packet->ExtractUINT64(&ActCharGUID);
	//packet->ExtractUINT64(&uiMoney);

	WPARAM wParam = (WPARAM)(&ObjectGUID);
	LPARAM lParam = (LPARAM)packet;

	g_pGOBManager->SPPerformMessage(SPGM_TRADE_SC_PUSHMONEY, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(TRADE_SC_PUSHITEM)
{
	GU_ID			ObjectGUID;		// 게임오브젝트아이디
	//GU_ID			ActCharGUID;	// 아이템을 올린 아바타키
	//TRADE_ITEM	stTradeItem;	// 아이템을 올린 유저의 컨테이너,슬롯,트레이드슬롯 정보가 들어있는 구조체
	//ITEM			stItem;			// 아이템에 대한 상세정보를 보내줘서 마우스를 아이템에 위치시키면 정보를 보여줄수있도록함

	packet->ExtractUINT64(&ObjectGUID);
	//packet->ExtractUINT64(&ActCharGUID);
	//packet->ExtractStruct(&stTradeItem, sizeof(TRADE_ITEM));
	//packet->ExtractStruct(&stItem, sizeof(ITEM));

	WPARAM wParam = (WPARAM)(&ObjectGUID);
	LPARAM lParam = (LPARAM)packet;

	g_pGOBManager->SPPerformMessage(SPGM_TRADE_SC_PUSHITEM, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(TRADE_SC_POPITEM)
{
	GU_ID			ObjectGUID;		// 게임오브젝트아이디
	//GU_ID			ActCharGUID;	// 아이템을 내린 아바타키
	//SLOT_INDEX	TradeSlotIndex;	// 트레이드슬롯

	packet->ExtractUINT64(&ObjectGUID);
	//packet->ExtractUINT64(&ActCharGUID);
	//packet->ExtractUINT8(&TradeSlotIndex);

	WPARAM wParam = (WPARAM)(&ObjectGUID);
	LPARAM lParam = (LPARAM)packet;

	g_pGOBManager->SPPerformMessage(SPGM_TRADE_SC_POPITEM, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(TRADE_SC_WAIT)
{
	GU_ID				ObjectGUID;					// 게임오브젝트아이디
	// GU_ID			ActCharGUID;				// accept를 누른 아바타키
	// UINT8			byAllWait;					// 거래대기완료여부

	packet->ExtractUINT64(&ObjectGUID);
	//packet->ExtractUINT64(&ActCharGUID);
	//packet->ExtractUINT8(&byAllWait);
	
	WPARAM wParam = (WPARAM)(&ObjectGUID);
	LPARAM lParam = (LPARAM)packet;

	g_pGOBManager->SPPerformMessage(SPGM_TRADE_SC_WAIT, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(TRADE_SC_ACCEPT)
{
	GU_ID				ObjectGUID;					// 게임오브젝트아이디
	// GU_ID			ActCharGUID;				// accept를 누른 아바타키
	// UINT8			byFinish;					// 거래완료여부
	// GOLBAL_STRING_ID	ErrorID;					// 에러아이디(0이면 에러없음)
	// GU_ID			ErrCharGUID;				// 오류제공자 아바타키

	packet->ExtractUINT64(&ObjectGUID);
	//packet->ExtractUINT64(&ActCharGUID);
	//packet->ExtractUINT8(&byFinish);
	//packet->ExtractUINT32(&ErrorID);
	//packet->ExtractUINT64(&ErrCharGUID);
	
	WPARAM wParam = (WPARAM)(&ObjectGUID);
	LPARAM lParam = (LPARAM)packet;

	g_pGOBManager->SPPerformMessage(SPGM_TRADE_SC_ACCEPT, wParam, lParam);
}

DEFINE_SC_PACKET_PROC(TRADE_SC_CANCEL)
{
	GU_ID				ObjectGUID;					// 게임오브젝트아이디
	// GU_ID			ActCharGUID;				// 거래를 취소한 아바타키

	packet->ExtractUINT64(&ObjectGUID);
	//packet->ExtractUINT64(&ActCharGUID);
	
	WPARAM wParam = (WPARAM)(&ObjectGUID);
	LPARAM lParam = (LPARAM)packet;
	g_pGOBManager->SPPerformMessage(SPGM_TRADE_SC_CANCEL, wParam, lParam);
}
