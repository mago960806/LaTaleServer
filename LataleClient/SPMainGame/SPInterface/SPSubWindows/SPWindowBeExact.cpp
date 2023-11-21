/*
* Copyright (c) 2008,上海盛趣信息技术（SDG事业部）
* All rights reserved.
* 
* 文件名称：SPWindowBeExact.cpp
* 摘    要：被索取框体实现
* 
* 作    者：夏林
* 完成日期：2008年07月30日
*/

#include "SPCommon.h"
#include "SPUtil.h"

#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowBeExact.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPWindowEdit.h"
#include "SPWindowEditMultiLine.h"

#include "SPSubGameManager.h"

#include "SPGOBManager.h"
#include "SPGameObject.h"
#include "SPPlayerInvenArchive.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPPlayer.h"

#include "SPItem.h"
#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPCheckManager.h"
#include "SPItemCluster.h"

#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"
#include "SPIMEPool.h"
#include "SPResourceManager.h"
#include "SPLocalizeManager.h"

#include "SPPlayer.h"
#include "SPGOBManager.h"


using namespace std;

//------------------------------------------------------------------------------------
SPWindowBeExact::SPWindowBeExact(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
{	
	Init();
}

//------------------------------------------------------------------------------------
SPWindowBeExact::SPWindowBeExact(INSTANCE_ID InstanceID,
	int iX, int iY, int iCX, int iCY, SPWindow* pParent)
	: SPWindow(WNDID_BEEXACT, InstanceID, iX, iY, iCX, iCY, pParent)				 
{	
	Init();
}

//------------------------------------------------------------------------------------
SPWindowBeExact::~SPWindowBeExact()
{
	Clean();
}

//------------------------------------------------------------------------------------
void SPWindowBeExact::Clean()
{
	SPWindow::Clean();
}

//------------------------------------------------------------------------------------
void SPWindowBeExact::Init()
{
	m_uiCurOZ = 0;
	m_uiCurMileage = 0;

	m_vecPackageID.clear();
	m_strRequesterName.clear();
	m_strMsg.clear();
	m_iCahsierPrice = 0;
	m_bGift = false;
	m_bBuyRecv = true;
	m_bOnceAni = false;
	m_uiBuyItem = 0;
	m_strTargetName.clear();
	m_fBuyWaitAccmulateTime = 0.0f;

	m_strGiftFriendName.clear();
	m_strGiftFriendMsg.clear();	

	m_vCashierItemAttr.clear();

	InitResult();
	InitBuyWait();

	SPWindow* pkParent = new SPWindowStatic(WIID_BEEXACT_EXACT, 0, 0, 268, 220, this);
	pkParent->Hide();

	//SetMoveable(true);

}

//------------------------------------------------------------------------------------
void SPWindowBeExact::RefreshFrame()
{
	m_iCahsierPrice = 0;

	const  int  iNum = 16;
	SPWindowStatic* pkParent, *pkChild[2];
	SPWindowButton* pkButton;
	SPWindowEdit* pkEdit;
	SPWindowEditMultiLine* pkMultiEdit;

	int iItemCount = static_cast<int>(m_vecPackageID.size());
	int iHeight = iItemCount * iNum;

	pkParent = static_cast<SPWindowStatic*>(Find(WIID_BEEXACT_EXACT));
	if ( !pkParent )
		return;

	SetRectSize(268, 220+iHeight);
	pkParent->SetRectSize(268, 220+iHeight);
	RefreshChildPos();

	pkChild[0] = static_cast<SPWindowStatic*>(pkParent->Find(WIID_BEEXACT_EXACT_FRAME));
	pkParent->CleanChild(pkChild[0]);

	pkParent = new SPWindowStatic(WIID_BEEXACT_EXACT_FRAME, 0, 0, 268, 220+iHeight, pkParent);
	pkParent->SetWindowType(WND_TYPE_TOPBASE);
	pkParent->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 28, 269, 36);
	pkParent->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 38, 269, 44);
	pkParent->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 46, 269, 54);

	char  szTitle[128];
	sprintf(szTitle, "您的好友%s向您索取礼物", m_strRequesterName.c_str());
	pkChild[0] = new SPWindowStatic(WIID_BEEXACT_EXACT_TEXT, 8, 10, 222, 11, pkParent);
	pkChild[0]->SetFont(FONT_12_NORMAL);
	pkChild[0]->SetFontColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
	pkChild[0]->SetFormat(DT_VCENTER | DT_LEFT);
	pkChild[0]->SetWindowText(szTitle);

	pkChild[0] = new SPWindowStatic(WIID_BEEXACT_EXACT_MESSAGE_GRN_BG, 2, 30, 261, 85, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 451, 121);
	pkChild[0]->SetSrcSize(2, 2);

	pkChild[1] = new SPWindowStatic(WIID_BEEXACT_EXACT_RECEIVE_BG, 89, 8, 129, 16, pkChild[0]);
	pkChild[1]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 382, 1);

	pkEdit = new SPWindowEdit(WIID_BEEXACT_EXACT_RECEIVE, 5, 3, 119, 12, pkChild[1]);
	pkEdit->SetLimitText(LEN_NAME);
	pkEdit->SetWindowText(m_strRequesterName.c_str());
	pkEdit->SetEnable(false);

	pkChild[1] = new SPWindowStatic(WIID_BEEXACT_EXACT_MESSAGE_BG, 89, 29, 129, 46, pkChild[0]);
	pkChild[1]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 382, 18);

	pkMultiEdit = new SPWindowEditMultiLine(WIID_BEEXACT_EXACT_MESSAGE, 5, 2, 119, 43, pkChild[1]);
	pkMultiEdit->SetLimitText(50);
	pkMultiEdit->SetWindowText(m_strMsg.c_str());

	pkChild[0] = new SPWindowStatic(WIID_BEEXACT_EXACT_RECEIVE_IMG, 35, 40, 24, 13, pkParent);
	pkChild[0]->SetFont(FONT_12_NORMAL);
	pkChild[0]->SetFontColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
	pkChild[0]->SetFormat(DT_VCENTER | DT_RIGHT);
	pkChild[0]->SetWindowText("好友:");
	
	pkChild[0] = new SPWindowStatic(WIID_BEEXACT_EXACT_MESSAGE_IMG, 35, 60, 24, 13, pkParent);
	pkChild[0]->SetFont(FONT_12_NORMAL);
	pkChild[0]->SetFontColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
	pkChild[0]->SetFormat(DT_VCENTER | DT_RIGHT);
	pkChild[0]->SetWindowText("消息:");

	for (int i=0; i<iItemCount; i++)
	{
		SPCashItemAttr* pkItemAttr = NULL;
		pkItemAttr = g_pItemCluster->GetMinorCashItem(m_vecPackageID[i]);
		if ( pkItemAttr )
		{
			int iPrice = pkItemAttr->iSalePrice ? pkItemAttr->iSalePrice : pkItemAttr->iPrice;

			pkButton = new SPWindowButton(WIID_BEEXACT_EXACT_CHECK+i, 15, 124 + i*iNum, 11, 11, pkParent);
			pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 324, 216);
			pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 324, 216);
			pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 324, 228);
			pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 409, 240);
			pkButton->SPSendMessage(SPIM_BTN_SELECT, true);

			pkChild[0] = new SPWindowStatic(WIID_BEEXACT_EXACT_NAME+i, 30, 124 + i*iNum, 120, 12, pkParent);
			pkChild[0]->SetFont(FONT_12_NORMAL);
			pkChild[0]->SetFontColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
			pkChild[0]->SetFormat(DT_VCENTER | DT_LEFT);
			pkChild[0]->SetWindowText(pkItemAttr->strName.c_str());
			
			pkChild[0] = new SPWindowStatic(WIID_BEEXACT_EXACT_PRICE+i, 170, 124 + i*iNum, 50, 12, pkParent);
			pkChild[0]->SetFont(FONT_12_NORMAL);
			pkChild[0]->SetFontColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
			pkChild[0]->SetFormat(DT_VCENTER | DT_RIGHT);
			pkChild[0]->SetWindowText(ConvertMoneyToString(iPrice));

			pkChild[0] = new SPWindowStatic(WIID_BEEXACT_EXACT_OZ_ICON+i, 222, 124 + i*iNum, 15, 11, pkParent);
			pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 113);

			m_iCahsierPrice += iPrice;
		}
	}

	pkChild[0] = new SPWindowStatic(WIID_BEEXACT_EXACT_PRICE_TEXT, 50, 140+iHeight, 20, 12, pkParent);
	pkChild[0]->SetFont(FONT_12_BOLD);
	pkChild[0]->SetFontColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
	pkChild[0]->SetFormat(DT_VCENTER | DT_LEFT);
	pkChild[0]->SetWindowText("合计:");
	
	pkChild[0] = new SPWindowStatic(WIID_BEEXACT_EXACT_PRICE_SUM, 120, 140+iHeight, 60, 12, pkParent);
	pkChild[0]->SetFont(FONT_12_BOLD);
	pkChild[0]->SetFontColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
	pkChild[0]->SetFormat(DT_VCENTER | DT_RIGHT);
	pkChild[0]->SetWindowText(ConvertMoneyToString(m_iCahsierPrice));

	pkChild[0] = new SPWindowStatic(WIID_BEEXACT_EXACT_OZICON_SUM, 182, 140+iHeight, 15, 11, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 113);

	pkChild[0] = new SPWindowStatic(WIID_BEEXACT_EXACT_TEXT, 100, 160+iHeight, 15, 11, pkParent);
	pkChild[0]->SetFont(FONT_12_BOLD);
	pkChild[0]->SetFontColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
	pkChild[0]->SetFormat(DT_VCENTER | DT_LEFT);
	pkChild[0]->SetWindowText("是否支付?");

	pkButton = new SPWindowButton(WIID_BEEXACT_EXACT_OK, 58, 180+iHeight, 66, 17, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 280);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 298);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 316);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 334);

	pkButton = new SPWindowButton(WIID_BEEXACT_EXACT_CANCEL, 138, 180+iHeight, 66, 17, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 389, 280);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 389, 298);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 389, 316);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 389, 334);
	
}

//------------------------------------------------------------------------------------
void SPWindowBeExact::UpdateFrame()
{
	m_iCahsierPrice = 0;
	SPWindowStatic * pkChild;
	SPWindowButton * pkButton;
	int iItemCount = static_cast<int>(m_vecPackageID.size());

	for (int i=0; i<iItemCount; i++)
	{
		SPCashItemAttr* pkItemAttr = NULL;
		pkItemAttr = g_pItemCluster->GetMinorCashItem(m_vecPackageID[i]);
		if ( pkItemAttr )
		{
			pkButton = static_cast<SPWindowButton *>(Find(WIID_BEEXACT_EXACT_CHECK+i, true));
			if ( pkButton && pkButton->IsChecked() )
			{
				int iPrice = pkItemAttr->iSalePrice ? pkItemAttr->iSalePrice : pkItemAttr->iPrice;
				m_iCahsierPrice += iPrice;
			}
		}
	}
	
	pkChild = static_cast<SPWindowStatic*>(Find(WIID_BEEXACT_EXACT_PRICE_SUM, true));
	if ( pkChild )
		pkChild->SetWindowText(ConvertMoneyToString(m_iCahsierPrice));

}

//------------------------------------------------------------------------------------
void SPWindowBeExact::Process(float fTime)
{
	if( !m_bShow )
		return;

	SPWindow::Process(fTime);
	
	ProcessBuyWaitAni(fTime);
}

//------------------------------------------------------------------------------------
void SPWindowBeExact::Render(float fTime)
{
	if( !m_bShow )
		return;

	SPWindow::Render(fTime);
	
}

//------------------------------------------------------------------------------------
int SPWindowBeExact::OnBeExact(WPARAM wParam, LPARAM lParam)
{
	if( m_bShow )
		return 1;

	m_vCashierItemAttr.clear();
	CPacket * packet = (CPacket *)lParam;

	UINT8				nPackageNUM;						// iPackageID的数量
	UINT8				iNameLen;							// szReceiverName的长度
	TCHAR				szRequesterName[LEN_NAME+1] = "";	    // 被请求玩家名字
	UINT8				iMsglen;							// szMsg长度
	TCHAR				szMsg[100] = "";					// 消息

	packet->ExtractUINT8(&nPackageNUM);
	m_vecPackageID.resize(nPackageNUM);
	for ( int i=0; i<nPackageNUM; i++)
	{
		int iItemID = 0;
		packet->ExtractUINT32((UINT32*)&iItemID);
		m_vecPackageID[i] = iItemID;
	}

	packet->ExtractUINT8(&iNameLen);
	if ( iNameLen > 0 )
		packet->ExtractData(szRequesterName, iNameLen);
	packet->ExtractUINT8(&iMsglen);
	if ( iMsglen > 0 )
		packet->ExtractData(szMsg, iMsglen);

	m_strRequesterName = szRequesterName;
	m_vCashierItemAttr.clear();
	m_strMsg = szMsg;
	m_bGift = false;

	RefreshFrame();
	Show();

	SPWindow * pkWindow = NULL;
	pkWindow = Find(WIID_BEEXACT_BUYWAIT, true);
	if ( pkWindow )
		pkWindow->Hide();

	pkWindow = Find(WIID_BEEXACT_RESULT, true);
	if ( pkWindow )
		pkWindow->Hide();

	//pkWindow = Find(WIID_BEEXACT_EXACT_FRAME, true);
	//g_pInterfaceManager->SetModalWindow(pkWindow);

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowBeExact::OnOK(WPARAM, LPARAM)
{
	SPWindow* pkWindow = Find(WIID_BEEXACT_EXACT);
	if( pkWindow == NULL )
		return 1;

	m_strGiftFriendName.clear();
	m_strGiftFriendMsg.clear();

	const char* pszString = NULL;
	SPWindow* pkChildWindow;
	pkChildWindow = pkWindow->Find(WIID_BEEXACT_EXACT_RECEIVE, true);
	if( pkChildWindow )
		pszString = pkChildWindow->GetWindowText();

	if( pszString )
	{
		m_strGiftFriendName = pszString;

		pkChildWindow = pkWindow->Find(WIID_BEEXACT_EXACT_MESSAGE, true);
		if( pkChildWindow )
			pszString = pkChildWindow->GetWindowText();

		if( pszString )
			m_strGiftFriendMsg = pszString;

		int nItemSize = static_cast<int>(m_vecPackageID.size());
		m_vCashierItemAttr.clear();
		for ( int i=0; i<nItemSize; i++ )
		{
			SPWindowButton* pkWindowButton = static_cast<SPWindowButton*>(Find(WIID_BEEXACT_EXACT_CHECK+i, true));
			if ( pkWindowButton && pkWindowButton->IsChecked() )
			{
				SPCashItemAttr* pkItemAttr = g_pItemCluster->GetMinorCashItem(m_vecPackageID[i]);
				if ( pkItemAttr )
				{
					m_vCashierItemAttr.push_back(pkItemAttr);
				}
			}
		}
	}

	pkWindow->Hide();
	//g_pInterfaceManager->SetModalWindow(NULL);

	Gift();
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowBeExact::OnCancel(WPARAM, LPARAM)
{
	Hide();
	m_bGift = false;
	//g_pInterfaceManager->SetModalWindow(NULL);
	g_pCheckManager->SetDBBlock(false);
	m_vecPackageID.clear();
	return 1;
}

int SPWindowBeExact::OnCheck(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	SPWindowButton * pkButton = static_cast<SPWindowButton *>(Find(iID, true));
	if ( pkButton )
	{
		pkButton->SPSendMessage(SPIM_BTN_SELECT, !pkButton->IsChecked());
		UpdateFrame();
	}
	return 1;
}

//------------------------------------------------------------------------------------
bool SPWindowBeExact::Gift()
{
	if( g_pCheckManager->IsDBBlock() )
		return false;

	bool bSelfGiftCheck = false, bSendPacket = false;
	if( m_vCashierItemAttr.size() > 0 && m_vCashierItemAttr.back() != NULL )
	{
		char szBuf[64];
		if( g_pGOBManager->GetLocalPlayer() )
		{
			g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_GETGOBNAME, (LPARAM)szBuf);
			if( strcmp(m_strGiftFriendName.c_str(), szBuf) == 0 )
				bSelfGiftCheck = true;
		}

		if( !bSelfGiftCheck )
		{
			int iPrice = m_vCashierItemAttr.back()->iSalePrice ? m_vCashierItemAttr.back()->iSalePrice : m_vCashierItemAttr.back()->iPrice;
			//if( m_uiCurOZ < iPrice )
			//	OnMyInfoFilling(0, 0);
			//else
			{
				m_strGiftFriendName.resize(LEN_NAME);
				m_strGiftFriendMsg.resize(MAX_MAIl_BODY_LEN);
				CPacket kPacket(CASHSHOP_CS_BUY_PRESENT);
				
				kPacket.Add(const_cast<char *>(m_strGiftFriendName.c_str()), m_strGiftFriendName.size());
				kPacket.AddUINT32(m_vCashierItemAttr.back()->iCashID);
				kPacket.AddUINT32(m_vCashierItemAttr.back()->iPrice);
				kPacket.AddUINT32(m_vCashierItemAttr.back()->iSalePrice);
				kPacket.AddUINT32(m_vCashierItemAttr.back()->iMaxOZ);
				kPacket.AddUINT32(m_vCashierItemAttr.back()->iMaxMile);
				kPacket.AddUINT32(iPrice);
				kPacket.AddUINT32(0);
				kPacket.Add(const_cast<char *>(m_strGiftFriendMsg.c_str()), m_strGiftFriendMsg.size());

				(g_pNetworkManager->GetPacketHandler())->PostMsg(&kPacket);
				g_pCheckManager->SetDBBlock(true);
				m_vCashierItemAttr.pop_back();
				m_bBuyRecv = false;
				bSendPacket = true;
				m_bGift = true;
			}
		}
	}
	
	if( bSelfGiftCheck )
	{
		Hide();
		//g_pInterfaceManager->SetModalWindow(NULL);
		string strMsg = g_pResourceManager->GetGlobalString(9004004);
		g_pInterfaceManager->SPPerformMessage(SPIM_WNDCHAT_MESSAGE_STRING, (WPARAM)strMsg.c_str());
	}
	else if( bSendPacket )
	{
		ShowBuyWaitDlg();
		return true;
	}

	m_vCashierItemAttr.clear();

	Hide();
	m_bGift = false;
	//g_pInterfaceManager->SetModalWindow(NULL);
	return false;

}


//------------------------------------------------------------------------------------
int SPWindowBeExact::OnGift(WPARAM wParam, LPARAM lParam)
{
	g_pCheckManager->SetDBBlock(false);
	m_bBuyRecv = true;
	m_bGift = false;

	CPacket* pkPacket = reinterpret_cast< CPacket* >(lParam);
	if( pkPacket == NULL )
		return 1;

	SPWindow* pkWindow = Find(WIID_BEEXACT_BUYWAIT);

	GLOBAL_STRING_ID iError;
	pkPacket->ExtractUINT32(&iError);
	if( iError )
	{
		Hide();
		//g_pInterfaceManager->SetModalWindow(NULL);
		m_vCashierItemAttr.clear();
		HideBuyWaitDlg();

		string strMsg = g_pResourceManager->GetGlobalString(iError);
		g_pInterfaceManager->SPPerformMessage(SPIM_WNDCHAT_MESSAGE_STRING, (WPARAM)strMsg.c_str());

		if( iError != 9001001 )
			return 1;
	}

	GU_ID    iGUID;
	unsigned int uiBuyItem;
	char szTargetName[LEN_NAME+1];						// 酒捞祦E钱肺 谩阑版縼E\0捞 绝绰 巩力 荐沥 [2/28/2007 AJJIYA]
	memset(szTargetName,0,sizeof(szTargetName));

	pkPacket->ExtractUINT64(&iGUID);
	pkPacket->Extract(szTargetName, sizeof(char)*LEN_NAME);

	pkPacket->ExtractUINT32(&uiBuyItem);
	pkPacket->ExtractUINT32(&m_uiCurOZ);
	pkPacket->ExtractUINT32(&m_uiCurMileage);

	m_uiBuyItem = uiBuyItem;
	m_strTargetName = szTargetName;

	if( m_bOnceAni )
	{
		HideBuyWaitDlg();
		ShowResultDlg(uiBuyItem, szTargetName);
	}

	return 1;
}


//------------------------------------------------------------------------------------
void SPWindowBeExact::InitResult()
{
	SPWindowStatic* pkParent, *pkChild[2];
	SPWindowButton* pkButton;

	pkParent = new SPWindowStatic(WIID_BEEXACT_RESULT, 0, 57, 268, 226, this);
	pkParent->SetWindowType(WND_TYPE_TOPBASE);
	pkParent->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 28, 269, 36);
	pkParent->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 38, 269, 44);
	pkParent->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 46, 269, 54);

	pkChild[0] = new SPWindowStatic(WIID_BEEXACT_RESULT_ITEM_SLOT, 22, 42, 35, 11, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 443, 137);

	pkChild[1] = new SPWindowStatic(WIID_BEEXACT_RESULT_ITEM, 2, -24, 32, 32, pkChild[0]);

	pkChild[0] = new SPWindowStatic(WIID_BEEXACT_RESULT_ITEM_NAME, 69, 20, 175, 12, pkParent);
	pkChild[0]->SetFont(FONT_12_BOLD);
	pkChild[0]->SetFontColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
	pkChild[0]->SetFormat(DT_VCENTER | DT_LEFT);

	pkChild[0] = new SPWindowStatic(WIID_BEEXACT_RESULT_PRICE_BG, 70, 35, 80, 14, pkParent);
	pkChild[0]->SetWindowType(WND_TYPE_LEFTBASE);
	pkChild[0]->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 493, 497, 495, 511);
	pkChild[0]->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 497, 497, 499, 511);
	pkChild[0]->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 501, 497, 503, 511);

	pkChild[0] = new SPWindowStatic(WIID_BEEXACT_RESULT_OZ_ICON, 131, 37, 15, 11, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 101);

	pkChild[0] = new SPWindowStatic(WIID_BEEXACT_RESULT_PRICE, 74, 37, 53, 12, pkParent);
	pkChild[0]->SetFontColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	pkChild[0]->SetFormat(DT_VCENTER | DT_RIGHT);

	pkChild[0] = new SPWindowStatic(WIID_BEEXACT_RESULT_BUY_OZ_TEXT, 22, 76, 96, 13, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 241, 228);

	pkChild[0] = new SPWindowStatic(WIID_BEEXACT_RESULT_MY_OZ_TEXT, 22, 95, 96, 13, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 241, 242);

	pkChild[0] = new SPWindowStatic(WIID_BEEXACT_RESULT_BUY_OZ, 131, 77, 66, 12, pkParent);
	pkChild[0]->SetFontColor(D3DXCOLOR(0.2745f, 0.5490f, 0.4862f, 1.0f));
	pkChild[0]->SetFormat(DT_VCENTER | DT_RIGHT);

	pkChild[0] = new SPWindowStatic(WIID_BEEXACT_RESULT_MY_OZ, 131, 96, 66, 12, pkParent);
	pkChild[0]->SetFontColor(D3DXCOLOR(0.2745f, 0.5490f, 0.4862f, 1.0f));
	pkChild[0]->SetFormat(DT_VCENTER | DT_RIGHT);

	pkChild[0] = new SPWindowStatic(WIID_BEEXACT_RESULT_BUY_OZ_ICON, 201, 77, 15, 11, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 101);

	pkChild[0] = new SPWindowStatic(WIID_BEEXACT_RESULT_MY_OZ_ICON, 201, 96, 15, 11, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 101);

	pkChild[0] = new SPWindowStatic(WIID_BEEXACT_RESULT_MSG, 13, 129, 239, 45, pkParent);
	pkChild[0]->SetFontColor(D3DXCOLOR(0.7647f, 0.2235f, 0.2235f, 1.0f));
	pkChild[0]->SetFormat(DT_VCENTER | DT_CENTER);
	pkChild[0]->SetMultiLine(true, 2);

	pkButton = new SPWindowButton(WIID_BEEXACT_RESULT_OK, 98, 187, 66, 17, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 280);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 298);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 316);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 334);

	pkParent->Hide();
}

//------------------------------------------------------------------------------------
void SPWindowBeExact::InitBuyWait()
{
	SPWindowStatic* pkParent, *pkChild[2];

	pkParent = new SPWindowStatic(WIID_BEEXACT_BUYWAIT, -29, 103, 325, 134, this);
	pkParent->SetWindowType(WND_TYPE_LEFTBASE);
	pkParent->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 482, 64, 492, 198);
	pkParent->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 495, 64, 498, 198);
	pkParent->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 501, 64, 511, 198);

	pkChild[0] = new SPWindowStatic(WIID_BEEXACT_BUYWAIT_ITEM, 16, 32, 32, 32, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 292, 56);

	pkChild[0] = new SPWindowStatic(WIID_BEEXACT_BUYWAIT_PLAYER, 272, 32, 30, 32, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 481, 352);

	pkChild[0] = new SPWindowStatic(WIID_BEEXACT_BUYWAIT_TEXT, 8, 83, 303, 29, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 28);

	int iY[14] = {40, 32, 28, 27, 28, 32, 37, 42, 47, 51, 52, 51, 47, 39};
	int i;

	for( i = 0; i < 14; i++ )
	{
		pkChild[0] = new SPWindowStatic(WIID_BEEXACT_BUYWAIT_ANI + i,
			50 + i * 16, iY[i], 12, 12, pkParent);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 0, 0);
	}

	pkParent->Hide();
}

//------------------------------------------------------------------------------------
void SPWindowBeExact::InitBuyWaitAni(SPWindow* pkParentWindow)
{
	SPWindow* pkWindow;
	if( pkParentWindow == NULL )
		return;

	int i;
	for( i = 0; i < 14; i++ )
	{
		m_BuyAni[i].fAccmulateTime = 0.0f - (i * 0.06f);
		m_BuyAni[i].fDelayTime = 0.05f;
		m_BuyAni[i].iCurFrame = 0;
		m_BuyAni[i].iTotalFrame = -1;
		m_BuyAni[i].iIncDir = 1;

		pkWindow = pkParentWindow->Find(WIID_BEEXACT_BUYWAIT_ANI + i, true);
		if( pkWindow )
			pkWindow->Hide();
	}
}


//------------------------------------------------------------------------------------
void SPWindowBeExact::ProcessBuyWaitAni(float fTime)
{
	SPWindow* pkParentWindow, *pkChildWindow;
	pkParentWindow = Find(WIID_BEEXACT_BUYWAIT);
	if( pkParentWindow == NULL || !pkParentWindow->IsShow() )
		return;

	int iSrcX[5] = {343, 356, 369, 356, 369};
	int iSrcY[5] = {82, 82, 82, 52, 52};

	int i;
	bool bHide;
	for( i = 0; i < 14; i++ )
	{
		if( m_BuyAni[i].iTotalFrame >= 0 )
			continue;

		pkChildWindow = pkParentWindow->Find(WIID_BEEXACT_BUYWAIT_ANI + i, true);
		if( pkChildWindow == NULL )
			continue;

		bHide = (m_BuyAni[i].fAccmulateTime <= 0.0f) ? true : false;
		m_BuyAni[i].fAccmulateTime += fTime;

		if( bHide && m_BuyAni[i].fAccmulateTime >= 0.0f )
			pkChildWindow->Show();

		if( m_BuyAni[i].fAccmulateTime > m_BuyAni[i].fDelayTime )
		{
			m_BuyAni[i].fAccmulateTime -= m_BuyAni[i].fDelayTime;
			m_BuyAni[i].iCurFrame += m_BuyAni[i].iIncDir;

			if( m_BuyAni[i].iCurFrame > 4 )
			{
				m_BuyAni[i].iCurFrame = 3;
				m_BuyAni[i].iIncDir = -1;
			}
			else if( m_BuyAni[i].iCurFrame < 0 )
			{
				m_BuyAni[i].iTotalFrame = 0;
				m_BuyAni[i].iCurFrame = 0;
				pkChildWindow->Hide();

				if( i == 13 )
				{
					if( m_bBuyRecv )
					{
						HideBuyWaitDlg();
						ShowResultDlg(m_uiBuyItem, m_strTargetName.c_str());
						return;
					}
					else
					{
						InitBuyWaitAni(pkParentWindow);
						m_bOnceAni = true;
					}
				}
			}
		}

		pkChildWindow->SetImagePos(iSrcX[m_BuyAni[i].iCurFrame], iSrcY[m_BuyAni[i].iCurFrame]);
	}

	m_fBuyWaitAccmulateTime += fTime;
	if( m_fBuyWaitAccmulateTime > 180.0f )	// 3min
	{
		Hide();
		//g_pInterfaceManager->SetModalWindow(NULL);
		HideBuyWaitDlg();
		m_fBuyWaitAccmulateTime = 0.0f;

		string strMsg = g_pResourceManager->GetGlobalString(9000010);
		g_pInterfaceManager->SPPerformMessage(SPIM_WNDCHAT_MESSAGE_STRING, (WPARAM)strMsg.c_str());
		
	}
}


//------------------------------------------------------------------------------------
void SPWindowBeExact::ShowResultDlg(int iItemID, const char* pszTargetName)
{
	SPCashItemAttr * pkResultItemAttr = g_pItemCluster->GetMinorCashItem(iItemID);
	if( pkResultItemAttr == NULL )
		return;

	SPWindow* pkParentWindow, *pkChildWindow;
	pkParentWindow = Find(WIID_BEEXACT_RESULT);
	if( pkParentWindow == NULL )
		return;

	pkChildWindow = pkParentWindow->Find(WIID_BEEXACT_RESULT_ITEM, true);
	if( pkChildWindow )
	{
		pkChildWindow->SPSendMessage(SPIM_SET_IMAGECLEAR);

		SPTexture* pkTexture;
		POINT pt;
		pkTexture = g_pItemCluster->GetItemTextureFromTextureInx(
			pkResultItemAttr->iIcon, pkResultItemAttr->iIconIndex, pt);
		if( pkTexture )
			pkChildWindow->SetImage(pkTexture, pt.x, pt.y);
	}

	pkChildWindow = pkParentWindow->Find(WIID_BEEXACT_RESULT_ITEM_NAME, true);
	if( pkChildWindow )
		pkChildWindow->SetWindowText(pkResultItemAttr->strName.c_str());

	int iPrice = pkResultItemAttr->iPrice;
	if( pkResultItemAttr->iSalePrice )
		iPrice = pkResultItemAttr->iSalePrice;

	pkChildWindow = pkParentWindow->Find(WIID_BEEXACT_RESULT_PRICE, true);
	if( pkChildWindow )
		pkChildWindow->SetWindowText(ConvertMoneyToString(iPrice));

	pkChildWindow = pkParentWindow->Find(WIID_BEEXACT_RESULT_BUY_OZ, true);
	if( pkChildWindow )
		pkChildWindow->SetWindowText(ConvertMoneyToString(iPrice));

	pkChildWindow = pkParentWindow->Find(WIID_BEEXACT_RESULT_MY_OZ, true);
	if( pkChildWindow )
		pkChildWindow->SetWindowText(ConvertMoneyToString(m_uiCurOZ));

	string strMsg;
	if( pszTargetName )
	{
		char szTemp[256];
		wsprintf(szTemp, g_pResourceManager->GetGlobalString(9004005), pszTargetName);
		strMsg = szTemp;
	}

	pkChildWindow = pkParentWindow->Find(WIID_BEEXACT_RESULT_MSG, true);
	if( pkChildWindow )
		pkChildWindow->SetWindowText(strMsg.c_str());

	pkParentWindow->Show();
	//g_pInterfaceManager->SetModalWindow(pkParentWindow);
}

//------------------------------------------------------------------------------------
void SPWindowBeExact::ShowBuyWaitDlg()
{
	SPWindow* pkParentWindow;
	pkParentWindow = Find(WIID_BEEXACT_BUYWAIT);
	if( pkParentWindow == NULL )
		return;

	m_bOnceAni = false;
	InitBuyWaitAni(pkParentWindow);
	m_fBuyWaitAccmulateTime = 0.0f;
	pkParentWindow->Show();
	//g_pInterfaceManager->SetModalWindow(pkParentWindow);
}

//------------------------------------------------------------------------------------
void SPWindowBeExact::HideBuyWaitDlg()
{
	SPWindow* pkParentWindow;
	pkParentWindow = Find(WIID_BEEXACT_BUYWAIT);
	if( pkParentWindow == NULL )
		return;

	pkParentWindow->Hide();
	//g_pInterfaceManager->SetModalWindow(NULL);
}

//------------------------------------------------------------------------------------
int SPWindowBeExact::OnResultOK(WPARAM wParam, LPARAM lParam)
{
	SPWindow* pkWindow = Find(WIID_BEEXACT_RESULT);
	if( pkWindow )
		pkWindow->Hide();

	//g_pInterfaceManager->SetModalWindow(NULL);

	Gift();
	return 1;
}


int SPWindowBeExact::OnEditSetFocus( WPARAM wParam, LPARAM lParam )
{
	g_pInterfaceManager->SetKeyInput( TRUE );

	return 1;
}

int SPWindowBeExact::OnEditKillFocus( WPARAM wParam, LPARAM lParam )
{
	g_pInterfaceManager->SetKeyInput( FALSE );

	return 1;
}

//------------------------------------------------------------------------------------
SPIMESSAGE_OBJ_MAP_BEGIN(SPWindowBeExact)

SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_BEEXACT_EXACT_OK,		    OnOK	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_BEEXACT_EXACT_CANCEL,		OnCancel	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_BEEXACT_RESULT_OK,			OnResultOK	)

SPIMESSAGE_CONTROL(	SPIM_SETFOCUS	,	WIID_BEEXACT_EXACT_MESSAGE		,	OnEditSetFocus		)
SPIMESSAGE_CONTROL(	SPIM_KILLFOCUS	,	WIID_BEEXACT_EXACT_MESSAGE		,	OnEditKillFocus		)

SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP,		WIID_BEEXACT_EXACT_CHECK,			WIID_BEEXACT_EXACT_CHECK + 20,		OnCheck)

SPIMESSAGE_COMMAND(	SPIM_BEEXACT_MESSAGE, 		OnBeExact   )
SPIMESSAGE_COMMAND( SPIM_BEEXACT_GIFT,			OnGift    )

SPIMESSAGE_OBJ_MAP_END(SPWindow)