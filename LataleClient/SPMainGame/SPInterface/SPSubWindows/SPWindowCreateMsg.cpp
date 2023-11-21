

#include <Windows.h>
#include "SPCommon.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowCreateMsg.h"
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

#include "SPCameraManager.h"

#include "SPAbuseFilter.h"

using namespace std;

//------------------------------------------------------------------------------------
SPWindowCreateMsg::SPWindowCreateMsg(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
{	
	Init();
}

//------------------------------------------------------------------------------------
SPWindowCreateMsg::SPWindowCreateMsg(INSTANCE_ID InstanceID,
	int iX, int iY, int iCX, int iCY, SPWindow* pParent)
	: SPWindow(WNDID_CREATEMSG, InstanceID, iX, iY, iCX, iCY, pParent)
							 
{	
	Init();
	m_iType = 0;
}

//------------------------------------------------------------------------------------
SPWindowCreateMsg::~SPWindowCreateMsg()
{
	Clean();
}

//------------------------------------------------------------------------------------
void SPWindowCreateMsg::Clean()
{
	SPWindow::Clean();

}

//------------------------------------------------------------------------------------
void SPWindowCreateMsg::Init()
{
	SetMoveable(true);
}

//------------------------------------------------------------------------------------
void SPWindowCreateMsg::Process(float fTime)
{
	if( !m_bShow )
		return;

	SPWindow::Process(fTime);

	ParsingPosOK();
}

//------------------------------------------------------------------------------------
void SPWindowCreateMsg::Render(float fTime)
{
	if( !m_bShow )
		return;

	SPWindow::Render(fTime);
}

bool SPWindowCreateMsg::ParsingPosOK()
{
	bool  bPosOK = true;
	GetAbsCoord(m_iPosX, m_iPosY);

	int iPosX = m_iPosX;
	int iPosY = m_iPosY;
	if( g_pCameraManagr != NULL )
	{
		SPCameraStruct*	pCameraStruct	=	g_pCameraManagr->GetCameraStruct();
		if( pCameraStruct != NULL )
		{
			iPosX	+=	pCameraStruct->fOffsetX;
			iPosY	+=	pCameraStruct->fOffsetY;
		}
	}
	if ( iPosX < 0 || iPosY < 0 )
		bPosOK =  false;

	if( !g_pGOBManager || g_pGOBManager->CheckMsgBoardOverlap(m_iType, iPosX, iPosY) == false )
		bPosOK =  false;

	//bPosOK = true;
	SPWindow * pkWindow = Find(WIID_CREATEMSG_CHECKTEXT, true);
	if ( bPosOK )
	{
		if ( pkWindow )
		{
			pkWindow->SetFontColor(ARGB(255, 0, 0, 255));
			pkWindow->SetWindowText("OK,可以放置了!");
		}
	}
	else		
	{
		if ( pkWindow )
		{
			pkWindow->SetFontColor(ARGB(255, 255, 0, 0));
			pkWindow->SetWindowText("不能放置在此!");
		}
	}

	return bPosOK;
}

int SPWindowCreateMsg::OnCreateMsgBoard(WPARAM wParam, LPARAM lParam)
{
	m_itemSLOT.ContainerType = (CONTAINER_TYPE)wParam;
	m_itemSLOT.SlotIndex = (SLOT_INDEX)lParam+1;

	if( !g_pGOBManager->GetLocalPlayer() )
		return 1;

	SPItem*  pItem = NULL;
	pItem = g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetInventoryItem(m_itemSLOT.ContainerType, m_itemSLOT.SlotIndex-1);
	if ( !pItem )
		return 1;

	m_itmeGUID = pItem->GetItemStatus()->GetItemID();

	MSG_BOARD * pMsgBoard = g_pGOBCluster->GetMsgBoardByItme(m_itmeGUID);
	if ( !pMsgBoard )
		return 1;

	m_iType = pMsgBoard->m_iTypeID;

	RECT     rcTitle,  rcText,  rcFrame;
	rcFrame.left	= pMsgBoard->m_iImageX1;
	rcFrame.right	= pMsgBoard->m_iImageX2;
	rcFrame.top		= pMsgBoard->m_iImageY1;
	rcFrame.bottom	= pMsgBoard->m_iImageY2;
	rcText.left		= pMsgBoard->m_iTextX1;
	rcText.right	= pMsgBoard->m_iTextX2;
	rcText.top		= pMsgBoard->m_iTextY1;
	rcText.bottom	= pMsgBoard->m_iTextY2;
	rcTitle.left	= pMsgBoard->m_iTitleX;
	rcTitle.top		= pMsgBoard->m_iTitleY;


	SetRectSize(rcFrame.right-rcFrame.left, (rcFrame.bottom-rcFrame.top)+50);
	RefreshChildPos();
	CleanChildAll();

	//
	SPWindow* pkWindow = NULL;
	SPWindowStatic* pkFrame, * pkStatic[2];
	SPWindowButton* pkButton;
	SPWindowEditMultiLine *	pkMulEdit;

	pkFrame = new SPWindowStatic(WIID_CREATEMSG_FRAME, 0, 0, rcFrame.right-rcFrame.left, rcFrame.bottom-rcFrame.top, this);
	string   strTexName;
	if ( g_pResourceManager->GetGlobalFileName(pMsgBoard->m_iIcon) )
		strTexName = g_pResourceManager->GetGlobalFileName(pMsgBoard->m_iIcon);
	else
		strTexName = "DATA/INTERFACE/CONCEPT/MessageBoard1.png";
	pkFrame->SetImage(strTexName.c_str(), pMsgBoard->m_iImageX1, pMsgBoard->m_iImageY1);

	pkStatic[0] = new SPWindowStatic(WIID_CREATEMSG_TITLE, rcTitle.left, rcTitle.top, 180, 12, pkFrame);
	pkStatic[0]->SetFormat(DT_VCENTER | DT_LEFT);
	pkStatic[0]->SetFont(FONT_12_NORMAL);
	pkStatic[0]->SetFontColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	pkStatic[0]->SetWindowText(pMsgBoard->m_strName.c_str());

	pkMulEdit = new SPWindowEditMultiLine(WIID_CREATEMSG_TEXT, rcText.left, rcText.top, rcText.right-rcText.left, rcText.bottom-rcText.top, pkFrame);
	pkMulEdit->SetFormat(DT_VCENTER | DT_LEFT);
	pkMulEdit->SetFontColor(ARGB(255, 0, 0, 0));
	pkMulEdit->SetLimitText(100);
	
	pkStatic[1] = new SPWindowStatic(WIID_CREATEMSG_BUTTON_BG, 0, rcFrame.bottom-rcFrame.top, rcFrame.right-rcFrame.left, 50, this);
	pkStatic[1]->SetWindowType(WND_TYPE_LEFTBASE);
	pkStatic[1]->SetImageNormalHead("DATA/INTERFACE/CONCEPT/UI_MsgBoard.PNG", 11, 55, 13, 111);
	pkStatic[1]->SetImageNormalBody("DATA/INTERFACE/CONCEPT/UI_MsgBoard.PNG", 14, 55, 16, 111);
	pkStatic[1]->SetImageNormalTail("DATA/INTERFACE/CONCEPT/UI_MsgBoard.PNG", 226, 55, 228, 111);

	pkStatic[0] = new SPWindowStatic(WIID_CREATEMSG_CHECKTEXT, 0, 8, rcFrame.right-rcFrame.left, 12, pkStatic[1]);
	pkStatic[0]->SetFormat(DT_VCENTER | DT_CENTER);

	pkButton = new SPWindowButton(WIID_CREATEMSG_OK, (rcFrame.right-rcFrame.left)/2 - (66+10), 28, 66, 17, pkStatic[1]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 280);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 298);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 316);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 334);

	pkButton = new SPWindowButton(WIID_CREATEMSG_CANCEL, (rcFrame.right-rcFrame.left)/2 + 10, 28, 66, 17, pkStatic[1]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 389, 280);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 389, 298);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 389, 316);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 389, 334);


	ParsingPosOK();
	Show();
	return 1;
}

int SPWindowCreateMsg::OnOK(WPARAM wParam, LPARAM lParam)
{
	if ( !ParsingPosOK() )
	{
		string strMsg = "不能放置在此!";
		g_pInterfaceManager->SPPerformMessage(SPIM_WNDCHAT_MESSAGE_STRING, (WPARAM)strMsg.c_str());
		return 1;
	}

	SPWindow * pWindowMsg = Find(WIID_CREATEMSG_TEXT, true);
	if ( pWindowMsg )
	{
		int iPosX=0, iPosY=0;
		pWindowMsg->GetAbsCoord(iPosX, iPosY);
		char *  lpszMsg = const_cast<char *>(pWindowMsg->GetWindowText());
		if ( lpszMsg && strlen(lpszMsg) > 0 )
		{
			char szBuffer[1025];
			SPAbuseFilter::GetInstance()->Filter( lpszMsg , szBuffer , 1024 );
			lpszMsg	=	szBuffer;

			int iPosX = m_iPosX;
			int iPosY = m_iPosY;
			UINT8	iLen = (UINT8)strlen(lpszMsg);

			if( g_pCameraManagr != NULL )
			{
				SPCameraStruct*	pCameraStruct	=	g_pCameraManagr->GetCameraStruct();

				if( pCameraStruct != NULL )
				{
					iPosX	+=	pCameraStruct->fOffsetX;
					iPosY	+=	pCameraStruct->fOffsetY;
				}
			}

			CPacket   packet(ITEM_CS_MSGBOARD);
			packet.Add(&m_itmeGUID, sizeof(m_itmeGUID));
			packet.Add(&m_itemSLOT, sizeof(m_itemSLOT));
			packet.Add(&iPosX, sizeof(iPosX));
			packet.Add(&iPosY, sizeof(iPosY));
			packet.Add(&m_iType, sizeof(m_iType));
			packet.Add(&iLen, sizeof(iLen));
			packet.Add(lpszMsg, iLen);

			g_pNetworkManager->GetPacketHandler()->PostMsg(&packet);
		}
		else
		{
			string strMsg = "留言不能为空!";
			g_pInterfaceManager->SPPerformMessage(SPIM_WNDCHAT_MESSAGE_STRING, (WPARAM)strMsg.c_str());
			return 1;
		}
	}

	Hide();
	return 1;
}

int SPWindowCreateMsg::OnCancel(WPARAM wParam, LPARAM lParam)
{
	Hide();
	return 1;
}

int SPWindowCreateMsg::OnBeginMove	( WPARAM wParam, LPARAM lParam)
{
	ParsingPosOK();
	return SPWindow::OnBeginMove(wParam, lParam);
}

int SPWindowCreateMsg::OnEndMove		( WPARAM wParam, LPARAM lParam)
{
	ParsingPosOK();
	return SPWindow::OnEndMove(wParam, lParam);
}

int SPWindowCreateMsg::OnMove			( WPARAM wParam, LPARAM lParam)
{
	ParsingPosOK();
	return SPWindow::OnMove(wParam, lParam);
}


int SPWindowCreateMsg::OnCreateRes( WPARAM wParam, LPARAM lParam )
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

int SPWindowCreateMsg::OnEditSetFocus( WPARAM wParam, LPARAM lParam )
{
	g_pInterfaceManager->SetKeyInput( TRUE );

	return 1;
}

int SPWindowCreateMsg::OnEditKillFocus( WPARAM wParam, LPARAM lParam )
{
	g_pInterfaceManager->SetKeyInput( FALSE );

	return 1;
}

//------------------------------------------------------------------------------------
SPIMESSAGE_OBJ_MAP_BEGIN(SPWindowCreateMsg)

	SPIMESSAGE_COMMAND(	SPIM_MOVE_BEGIN			,				OnBeginMove		)
	SPIMESSAGE_COMMAND(	SPIM_MOVE_END			,				OnEndMove		)
	SPIMESSAGE_COMMAND(	SPIM_MOVE				,				OnMove			)
	SPIMESSAGE_COMMAND(	SPIM_MSGBOARD_CREATE_OPEN,				OnCreateMsgBoard)
	SPIMESSAGE_COMMAND(	SPIM_MSGBOARD_CREATE	,				OnCreateRes		)

	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_CREATEMSG_OK,		    OnOK	)
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_CREATEMSG_CANCEL,		OnCancel	)

	SPIMESSAGE_CONTROL(	SPIM_SETFOCUS	,	WIID_CREATEMSG_TEXT		,	OnEditSetFocus		)
	SPIMESSAGE_CONTROL(	SPIM_KILLFOCUS	,	WIID_CREATEMSG_TEXT		,	OnEditKillFocus		)

SPIMESSAGE_OBJ_MAP_END(SPWindow)