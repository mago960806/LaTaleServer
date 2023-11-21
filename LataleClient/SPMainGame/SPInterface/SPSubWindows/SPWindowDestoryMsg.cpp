

#include <Windows.h>
#include "SPCommon.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowDestoryMsg.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPWindowEdit.h"
#include "SPWindowEditMultiLine.h"

#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "PacketID.h"
#include "SPNetworkDEF.h"
#include "SPNetworkManager.h"

#include "Packet.h"
#include "PacketHandler.h"

#include "SPGOBCluster.h"

#include "SPGameObject.h"
#include "SPItemAttr.h"
#include "SPItemCluster.h"			//[2005/6/16] - jinhee
#include "SPGOBManager.h"

#include "SPStage.h"
#include "SPStageManager.h"
#include "SPTerrainAttribute.h"

#include "SPItem.h"
#include "SPItemStatus.h"
#include "SPCommandConvert.h"
#include "SPGOBStatus.h"
#include "SPPlayerStatus.h"
#include "SPPlayerStatusModel.h"
#include "SPGOBCoordPhysics.h"

#include "SPGOBClusterDef.h"
#include "SPPlayerInvenArchive.h"
#include "SPAvatarModelUnitDef.h"
#include "SPPlayer.h"

#include "SPResourceManager.h"

using namespace std;

//------------------------------------------------------------------------------------
SPWindowDestoryMsg::SPWindowDestoryMsg(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
{	
	Init();
}

//------------------------------------------------------------------------------------
SPWindowDestoryMsg::SPWindowDestoryMsg(INSTANCE_ID InstanceID,
	int iX, int iY, int iCX, int iCY, SPWindow* pParent)
	: SPWindow(WNDID_DESTORYMSG, InstanceID, iX, iY, iCX, iCY, pParent)
							 
{	
	Init();
}

//------------------------------------------------------------------------------------
SPWindowDestoryMsg::~SPWindowDestoryMsg()
{
	Clean();
}

//------------------------------------------------------------------------------------
void SPWindowDestoryMsg::Clean()
{
	SPWindow::Clean();
}

//------------------------------------------------------------------------------------
void SPWindowDestoryMsg::Init()
{
	SPWindowStatic* pkFrame, * pkStatic;
	SPWindowButton* pkButton;

	pkFrame = new SPWindowStatic(WIID_DESTORYMSG_FRAME, 0, 150, 268, 70, this);
	pkFrame->SetWindowType(WND_TYPE_TOPBASE);
	pkFrame->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 28, 269, 36);
	pkFrame->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 38, 269, 44);
	pkFrame->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 46, 269, 54);


	pkStatic = new SPWindowStatic(WIID_DESTORYMSG_TITLE, 5, 10, 258, 12, pkFrame);
	pkStatic->SetFormat(DT_VCENTER | DT_CENTER);
	pkStatic->SetFontColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
	pkStatic->SetWindowText("是否消耗涂绘喷桶进行破坏？");

	//pkStatic = new SPWindowStatic(WIID_DESTORYMSG_TEXT, 5, 20, 258, 48, pkFrame);
	//pkStatic->SetFormat(DT_VCENTER | DT_LEFT);
	//pkStatic->SetFontColor(ARGB(255, 54, 74, 107));
	//pkStatic->SetMultiLine(true, 3);

	pkButton = new SPWindowButton(WIID_DESTORYMSG_OK, 60, 35, 66, 17, pkFrame);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 280);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 298);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 316);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 334);

	pkButton = new SPWindowButton(WIID_DESTORYMSG_CANCEL, 140, 35, 66, 17, pkFrame);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 389, 280);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 389, 298);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 389, 316);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 389, 334);
	
	m_iType = 1;

}

//------------------------------------------------------------------------------------
void SPWindowDestoryMsg::Process(float fTime)
{
	if( !m_bShow )
		return;

	SPWindow::Process(fTime);
}

//------------------------------------------------------------------------------------
void SPWindowDestoryMsg::Render(float fTime)
{
	if( !m_bShow )
		return;

	SPWindow::Render(fTime);
}

void SPWindowDestoryMsg::DestoryMsgBoard(INSTANCE_ID instanceID , CLASS_ID ClassID, int iType)
{
	if( !g_pGOBManager->GetLocalPlayer() )
		return;

	m_iType = iType;
	m_instanceID = GetGlobalID(ClassID, instanceID);

	MSG_BOARD * pMsgBoard = g_pGOBCluster->GetMsgBoardByType(m_iType);
	if ( !pMsgBoard )
		return;

	SPItem*  pItem = NULL;
	pItem = g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetInvenItem(pMsgBoard->m_destoryGUID);
	if ( !pItem )
		return;

	SPWindow * pkWindow = Find(WIID_DESTORYMSG_TITLE, true);
	if ( pkWindow )
	{
		char  szMsg[128];
		sprintf(szMsg, "是否消耗'%s'进行破坏？", pItem->GetItemAttr()->m_strName.c_str());
		pkWindow->SetWindowText(szMsg);
	}

	Show();
	return;
}

int SPWindowDestoryMsg::OnOK(WPARAM, LPARAM)
{
	MSG_BOARD * pMsgBoard = g_pGOBCluster->GetMsgBoardByType(m_iType);
	if ( !pMsgBoard )
		return 1;

	GU_ID  itemID = pMsgBoard->m_destoryGUID;

	SPItem*  pItem = NULL;
	pItem = g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetInvenItem(itemID);
	if ( pItem )
	{
		m_itemSLOT.ContainerType = pItem->GetContainerType();
		m_itemSLOT.SlotIndex = pItem->GetSlotIndex()+1;

		CPacket   packet(ITEM_CS_DEL_MSGBOARD);
		packet.Add(&itemID, sizeof(itemID));
		packet.Add(&m_itemSLOT, sizeof(m_itemSLOT));
		packet.Add(&m_instanceID, sizeof(m_instanceID));

		g_pNetworkManager->GetPacketHandler()->PostMsg(&packet);
	}
	else
	{
		string strMsg = "没有涂毁道具哦,快去商城购买吧!";
		g_pInterfaceManager->SPPerformMessage(SPIM_WNDCHAT_MESSAGE_STRING, (WPARAM)strMsg.c_str());
	}

	Hide();
	return 1;
}

int SPWindowDestoryMsg::OnCancel(WPARAM, LPARAM)
{
	Hide();
	return 1;
}


int SPWindowDestoryMsg::OnDestoryRes( WPARAM wParam, LPARAM lParam )
{
	CPacket * packet = (CPacket *)&wParam;

	GLOBAL_STRING_ID  iError = 0;
	packet->ExtractUINT32(&iError);	
	if ( iError )
	{
		string strMsg = "失败了!";
		g_pInterfaceManager->SPPerformMessage(SPIM_WNDCHAT_MESSAGE_STRING, (WPARAM)strMsg.c_str());
		return 1;
	}

	Hide();
	return 1;
}

//------------------------------------------------------------------------------------
SPIMESSAGE_OBJ_MAP_BEGIN(SPWindowDestoryMsg)

	SPIMESSAGE_COMMAND(	SPIM_MSGBOARD_DESTORY	,				OnDestoryRes	)

	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_DESTORYMSG_OK,		    OnOK	)
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_DESTORYMSG_CANCEL,		OnCancel	)

SPIMESSAGE_OBJ_MAP_END(SPWindow)