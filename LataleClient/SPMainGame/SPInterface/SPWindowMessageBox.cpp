/*
* Copyright (c) 2008,上海盛趣信息技术（SDG事业部）
* All rights reserved.
* 
* 文件名称：SPWindowMessageBox.cpp
* 摘    要：对话框管理实现
* 
* 作    者：夏林
* 完成日期：2008年07月25日
*/

#include "SPCommon.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowMessageBox.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPWindowEditMultiLine.h"

#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"


using namespace std;

//------------------------------------------------------------------------------------
SPWindowMessageBox::SPWindowMessageBox(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
{	
	Init();
}

//------------------------------------------------------------------------------------
SPWindowMessageBox::SPWindowMessageBox(INSTANCE_ID InstanceID,
	int iX, int iY, int iCX, int iCY, SPWindow* pParent)
	: SPWindow(WNDID_MESSAGEBOX, InstanceID, iX, iY, iCX, iCY, pParent)
							 
{	
	Init();
}

//------------------------------------------------------------------------------------
SPWindowMessageBox::~SPWindowMessageBox()
{
	Clean();
}

//------------------------------------------------------------------------------------
void SPWindowMessageBox::Clean()
{
	m_WndID = -1;
	m_eMsg = SPIM_NULL;
	SPWindow::Clean();
}

//------------------------------------------------------------------------------------
void SPWindowMessageBox::Init()
{
	SPWindowStatic* pkFrame, * pkStatic;
	SPWindowButton* pkButton;

	pkFrame = new SPWindowStatic(WIID_MESSAGEBOX_FRAME, 268, 240, 268, 108, this);
	pkFrame->SetWindowType(WND_TYPE_TOPBASE);
	pkFrame->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 28, 269, 36);
	pkFrame->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 38, 269, 44);
	pkFrame->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 46, 269, 54);

	pkStatic = new SPWindowStatic(WIID_MESSAGEBOX_GRN_BG, 2, 18, 261, 52, pkFrame);
	pkStatic->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 451, 121);
	pkStatic->SetSrcSize(2, 2);

	pkStatic = new SPWindowStatic(WIID_MESSAGEBOX_TITLE, 4, 4, 180, 12, pkFrame);
	pkStatic->SetFormat(DT_VCENTER | DT_LEFT);
	pkStatic->SetFontColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
	pkStatic->SetWindowText("Message");

	pkStatic = new SPWindowStatic(WIID_MESSAGEBOX_TEXT, 5, 20, 258, 48, pkFrame);
	pkStatic->SetFormat(DT_VCENTER | DT_CENTER);
	pkStatic->SetFontColor(ARGB(255, 54, 74, 107));
	pkStatic->SetMultiLine(true, 3);

	pkButton = new SPWindowButton(WIID_MESSAGEBOX_OK, 60, 78, 66, 17, pkFrame);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 280);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 298);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 316);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 334);

	pkButton = new SPWindowButton(WIID_MESSAGEBOX_CANCEL, 140, 78, 66, 17, pkFrame);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 389, 280);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 389, 298);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 389, 316);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 389, 334);
	
	m_iType = 1;
	m_WndID = -1;
	m_eMsg = SPIM_NULL;
}

//------------------------------------------------------------------------------------
void SPWindowMessageBox::Process(float fTime)
{
	if( !m_bShow )
		return;

	SPWindow::Process(fTime);
}

//------------------------------------------------------------------------------------
void SPWindowMessageBox::Render(float fTime)
{
	if( !m_bShow )
		return;

	SPWindow::Render(fTime);
}

int SPWindowMessageBox::OnOK(WPARAM, LPARAM)
{
	if ( m_WndID != -1 && m_eMsg != SPIM_NULL )
		g_pInterfaceManager->SPChildWndSendMessage(m_WndID, m_eMsg, MES_OK, m_InstanceID);

	Hide();
	g_pInterfaceManager->SetModalWindow(NULL);
	return 1;
}

int SPWindowMessageBox::OnCancel(WPARAM, LPARAM)
{
	if ( m_WndID != -1 && m_eMsg != SPIM_NULL )
		g_pInterfaceManager->SPChildWndSendMessage(m_WndID, m_eMsg, MES_CANCEL, m_InstanceID);

	Hide();
	g_pInterfaceManager->SetModalWindow(NULL);
	return 1;
}
	
INSTANCE_ID	SPWindowMessageBox::MessageBox(const char* pszMsgText, int iType, INSTANCE_ID tMsgWnd, SPIM eMsg)
{
	m_iType = iType;
	m_WndID = tMsgWnd;
	m_eMsg = eMsg;

	SPWindowStatic* pkStatic;
	SPWindowButton* pkButtonOK, * pkButtonCancel;

	pkStatic = static_cast<SPWindowStatic*>(Find(WIID_MESSAGEBOX_TEXT, true));
	if ( pkStatic )
	{
		pkStatic->SetMultiLine(true, 3);
		pkStatic->SetWindowText(pszMsgText);
	}
	
	pkButtonOK = static_cast<SPWindowButton*>(Find(WIID_MESSAGEBOX_OK, true));
	pkButtonCancel = static_cast<SPWindowButton*>(Find(WIID_MESSAGEBOX_CANCEL, true));
	if ( !pkButtonOK || !pkButtonCancel)
		return 0;

	Show();
	g_pInterfaceManager->SetModalWindow(this);

	if ( m_iType & MES_OK )
	{
		if ( m_iType & MES_CANCEL )
		{
			pkButtonOK->Show();
			pkButtonCancel->Show();
			pkButtonOK->SetWindowRelPos(60, 78);
			pkButtonCancel->SetWindowRelPos(140, 78);
		}
		else
		{
			pkButtonOK->Show();
			pkButtonCancel->Hide();
			pkButtonOK->SetWindowRelPos(101, 78);
		}
	}
	else
	{
		pkButtonOK->Hide();
		pkButtonCancel->Show();
		pkButtonCancel->SetWindowRelPos(101, 78);
	}
	
	return m_InstanceID;
}


//------------------------------------------------------------------------------------
SPIMESSAGE_OBJ_MAP_BEGIN(SPWindowMessageBox)

SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_MESSAGEBOX_OK,		    OnOK	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_MESSAGEBOX_CANCEL,		OnCancel	)

SPIMESSAGE_OBJ_MAP_END(SPWindow)