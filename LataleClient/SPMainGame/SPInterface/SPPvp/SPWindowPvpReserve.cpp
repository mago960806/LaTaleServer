
#include "SPCommon.h"
#include "SPUtil.h"

#include "GlobalDefines_Share.h"

#include "SPMainGameDEF.H"
#include "SPGameObjectDEF.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPSubGameManager.h"
#include "SPGameObject.h"
#include "SPManager.h"
#include "SPMouseCursor.h"
#include "SPIMEPool.h"
#include "SPWindowDEF.h"
#include "SPInterfaceManager.h"

#include "SPEventDEF.h"
#include "SPEventManager.h"
#include "SPPvpManager.h"

#include "SPWindow.h"
#include "SPWindowButton.h"
#include "SPWindowStatic.h"
#include "SPWindowTypeWriter.h"
#include "SPWindowPvpReserve.h"

#include "SPDebug.h"


SPWindowPvpReserve::SPWindowPvpReserve(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent /*= NULL*/)
: SPWindow(WNDID_PVPRESERVE, InstanceID, iX, iY, iCX, iCY, pParent)
, m_pTextureBase(NULL)
{
	Init();
}


SPWindowPvpReserve::~SPWindowPvpReserve()
{
	Clean();
}


void SPWindowPvpReserve::Init()
{
	g_pVideo->GetTextureMgr()->LoadTexture("DATA/INTERFACE/CONCEPT/UI105.PNG" , &m_pTextureBase);	

	SetRect(&m_rtSrcBase[WINDOW_BASE_TOP_LEFT],		363, 323, 367, 327);
	SetRect(&m_rtSrcBase[WINDOW_BASE_TOP],			368, 323, 372, 327);	
	SetRect(&m_rtSrcBase[WINDOW_BASE_TOP_RIGHT],	373, 323, 377, 327);
	SetRect(&m_rtSrcBase[WINDOW_BASE_CENTER_LEFT],	363, 328, 367, 332);
	SetRect(&m_rtSrcBase[WINDOW_BASE_CENTER],		368, 328, 372, 332);
	SetRect(&m_rtSrcBase[WINDOW_BASE_CENTER_RIGHT],	373, 328, 377, 332);
	SetRect(&m_rtSrcBase[WINDOW_BASE_BOTTOM_LEFT],	363, 333, 367, 337);
	SetRect(&m_rtSrcBase[WINDOW_BASE_BOTTOM],		368, 333, 372, 337);
	SetRect(&m_rtSrcBase[WINDOW_BASE_BOTTOM_RIGHT],	373, 333, 377, 337);	

	SetRect(&m_rtDestBase[WINDOW_BASE_TOP_LEFT],		m_iAX, m_iAY, m_iAX + 4, m_iAY + 4);
	SetRect(&m_rtDestBase[WINDOW_BASE_TOP],				m_iAX + 4, m_iAY, m_iAX + 4 + 280, m_iAY + 4);	
	SetRect(&m_rtDestBase[WINDOW_BASE_TOP_RIGHT],		m_iAX + 284, m_iAY, m_iAX + 284 + 4, m_iAY + 4);
	SetRect(&m_rtDestBase[WINDOW_BASE_CENTER_LEFT],		m_iAX, m_iAY + 4, m_iAX + 4, m_iAY + 4 + 279);
	SetRect(&m_rtDestBase[WINDOW_BASE_CENTER],			m_iAX + 4, m_iAY + 4, m_iAX + 4 + 280, m_iAY + 4 + 279);
	SetRect(&m_rtDestBase[WINDOW_BASE_CENTER_RIGHT],	m_iAX + 284, m_iAY + 4, m_iAX + 284 + 4, m_iAY + 4 + 279);
	SetRect(&m_rtDestBase[WINDOW_BASE_BOTTOM_LEFT],		m_iAX, m_iAY + 283, m_iAX + 4, m_iAY + 283 + 4);
	SetRect(&m_rtDestBase[WINDOW_BASE_BOTTOM],			m_iAX + 4, m_iAY + 283, m_iAX + 4 + 280, m_iAY + 283 + 4);
	SetRect(&m_rtDestBase[WINDOW_BASE_BOTTOM_RIGHT],	m_iAX + 284, m_iAY + 283, m_iAX + 284 + 4, m_iAY + 283 + 4);

	InitSubControl();
}


void SPWindowPvpReserve::InitSubControl()
{
	char* OPTION_IMAGE_FILE3 = "DATA/INTERFACE/CONCEPT/UI103.PNG";
	char* OPTION_IMAGE_FILE5 = "DATA/INTERFACE/CONCEPT/UI105.PNG";
	char* OPTION_IMAGE_OBJ = "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG";
	char* OPTION_IMAGE_BG = "DATA/INTERFACE/CONCEPT/UI_WinBG_000.PNG";
	char* OPTION_IMAGE_BUTTON0000 = "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG";
	char* OPTION_IMAGE_BUTTON0001 = "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG";
	
	//// Control
	SPWindowButton* pButton = NULL;
	SPWindowStatic* pStatic = NULL;
	SPWindowStatic* pParent = NULL;
	SPWindowStatic* pSkin	= NULL;

	//WIID_NPC_PORTAL_EXIT	
	pButton = new SPWindowButton( WIID_PVPRESERVE_EXIT , 259, 9, 12, 12, this);
	pButton->SetImage		(OPTION_IMAGE_FILE3, 301,  321);		
	pButton->SetImageHit	(OPTION_IMAGE_FILE3, 314,  321);
	pButton->SetImagePush	(OPTION_IMAGE_FILE3, 327,  321);
	pButton->SetImageDisable(OPTION_IMAGE_FILE3, 340,  321);
	pButton->Show();

	//WIID_PVPRESERVE_TITLE
	pStatic = new SPWindowStatic( WIID_PVPRESERVE_TITLE, 10,	6,	159,	17, this);	
	pStatic->SetImage(OPTION_IMAGE_OBJ, 267, 29);
	pStatic->Show();

	//WIID_PVPRESERVE_LINE1
	pStatic = new SPWindowStatic( WIID_PVPRESERVE_LINE1, 2,	26,	284,	1, this);	
	pStatic->SetImage(OPTION_IMAGE_FILE5, 386, 324);
	pStatic->SetSrcSize(4, 1);
	pStatic->Show();

	//WIID_PVPRESERVE_SKIN
	pSkin = new SPWindowStatic( WIID_PVPRESERVE_SKIN, 2,	27,	284,	256, this);	
	pSkin->SetImage(OPTION_IMAGE_FILE5, 368, 313);
	pSkin->SetSrcSize(4, 4);
	pSkin->Show();

	//WIID_PVPRESERVE_HELP
	pStatic = new SPWindowStatic( WIID_PVPRESERVE_HELP, 8 - 2,	32 - 27,	265,	69, pSkin);	
	pStatic->SetImage(OPTION_IMAGE_OBJ, 1, 1);
	pStatic->Show();

	//WIID_PVPRESERVE_LIST_SKIN + WINDOW_BASE_TOP_LEFT
	pStatic = new SPWindowStatic( WIID_PVPRESERVE_LIST_SKIN + WINDOW_BASE_TOP_LEFT, 18 -2,	120 - 27,	2,	2, pSkin);	
	pStatic->SetImage(OPTION_IMAGE_BG, 483, 453);
	pStatic->SetSrcSize(2, 2);
	pStatic->Show();

	//WIID_PVPRESERVE_LIST_SKIN + WINDOW_BASE_TOP
	pStatic = new SPWindowStatic( WIID_PVPRESERVE_LIST_SKIN + WINDOW_BASE_TOP, 20 -2,	120 - 27,	248,	2, pSkin);	
	pStatic->SetImage(OPTION_IMAGE_BG, 488, 453);
	pStatic->SetSrcSize(2, 2);
	pStatic->Show();

	//WIID_PVPRESERVE_LIST_SKIN + WINDOW_BASE_TOP_RIGHT
	pStatic = new SPWindowStatic( WIID_PVPRESERVE_LIST_SKIN + WINDOW_BASE_TOP_RIGHT, 268 -2,	120 - 27,	2,	2, pSkin);	
	pStatic->SetImage(OPTION_IMAGE_BG, 493, 453);
	pStatic->SetSrcSize(2, 2);
	pStatic->Show();

	//WIID_PVPRESERVE_LIST_SKIN + WINDOW_BASE_CENTER_LEFT
	pStatic = new SPWindowStatic( WIID_PVPRESERVE_LIST_SKIN + WINDOW_BASE_CENTER_LEFT, 18 -2,	122 - 27,	2,	85, pSkin);	
	pStatic->SetImage(OPTION_IMAGE_BG, 483, 458);
	pStatic->SetSrcSize(2, 2);
	pStatic->Show();

	//WIID_PVPRESERVE_LIST_SKIN + WINDOW_BASE_CENTER
	pStatic = new SPWindowStatic( WIID_PVPRESERVE_LIST_SKIN + WINDOW_BASE_CENTER, 20 -2,	122 - 27,	248,	85, pSkin);	
	pStatic->SetImage(OPTION_IMAGE_BG, 488, 458);
	pStatic->SetSrcSize(2, 2);
	pParent = pStatic;
	pStatic->Show();

	//WIID_PVPRESERVE_LIST_SKIN + WINDOW_BASE_CENTER_RIGHT
	pStatic = new SPWindowStatic( WIID_PVPRESERVE_LIST_SKIN + WINDOW_BASE_CENTER_RIGHT, 268 -2,	122 - 27,	2,	85, pSkin);	
	pStatic->SetImage(OPTION_IMAGE_BG, 493, 458);
	pStatic->SetSrcSize(2, 2);
	pStatic->Show();

	//WIID_PVPRESERVE_LIST_SKIN + WINDOW_BASE_BOTTOM_LEFT
	pStatic = new SPWindowStatic( WIID_PVPRESERVE_LIST_SKIN + WINDOW_BASE_BOTTOM_LEFT, 18 -2,	207 - 27,	2,	2, pSkin);	
	pStatic->SetImage(OPTION_IMAGE_BG, 483, 463);
	pStatic->SetSrcSize(2, 2);
	pStatic->Show();

	//WIID_PVPRESERVE_LIST_SKIN + WINDOW_BASE_BOTTOM
	pStatic = new SPWindowStatic( WIID_PVPRESERVE_LIST_SKIN + WINDOW_BASE_BOTTOM, 20 -2,	207 - 27,	248,	2, pSkin);	
	pStatic->SetImage(OPTION_IMAGE_BG, 488, 463);
	pStatic->SetSrcSize(2, 2);
	pStatic->Show();

	//WIID_PVPRESERVE_LIST_SKIN + WINDOW_BASE_BOTTOM_RIGHT
	pStatic = new SPWindowStatic( WIID_PVPRESERVE_LIST_SKIN + WINDOW_BASE_BOTTOM_RIGHT, 268 -2,	207 - 27,	2,	2, pSkin);	
	pStatic->SetImage(OPTION_IMAGE_BG, 493, 463);
	pStatic->SetSrcSize(2, 2);
	pStatic->Show();

	//WIID_PVPRESERVE_LIST_CHECK + PVP_CHECK_TYPE_FFA_2
	pButton = new SPWindowButton(WIID_PVPRESERVE_LIST_CHECK + PVP_CHECK_TYPE_FFA_2 , 44 -  20, 124 - 122, 12, 12, pParent);
	SetCheckButton(pButton);
	m_pWindowCheck[PVP_CHECK_TYPE_FFA_2] = pButton;

	//WIID_PVPRESERVE_LIST_TEXT + PVP_CHECK_TYPE_FFA_2
	pStatic = new SPWindowStatic( WIID_PVPRESERVE_LIST_TEXT + PVP_CHECK_TYPE_FFA_2, 63 -20,	124 - 122,	185,	12, pParent);	
	pStatic->SetFormat(DT_LEFT | DT_VCENTER);
	pStatic->SetWindowText("PVP_CHECK_TYPE_FFA_2");
	if(g_pResourceManager->GetGlobalString(51000007))
		pStatic->SetWindowText(g_pResourceManager->GetGlobalString(51000007));
	pStatic->Show();

	//WIID_PVPRESERVE_LIST_CHECK + PVP_CHECK_TYPE_FFA_4
	pButton = new SPWindowButton(WIID_PVPRESERVE_LIST_CHECK + PVP_CHECK_TYPE_FFA_4 , 44 -  20, 140 - 122, 12, 12, pParent);
	SetCheckButton(pButton);
	m_pWindowCheck[PVP_CHECK_TYPE_FFA_4] = pButton;

	//WIID_PVPRESERVE_LIST_TEXT + PVP_CHECK_TYPE_FFA_4
	pStatic = new SPWindowStatic( WIID_PVPRESERVE_LIST_TEXT + PVP_CHECK_TYPE_FFA_4, 63 -20,	140 - 122,	185,	12, pParent);	
	pStatic->SetFormat(DT_LEFT | DT_VCENTER);
	pStatic->SetWindowText("PVP_CHECK_TYPE_FFA_4");
	if(g_pResourceManager->GetGlobalString(51000008))
		pStatic->SetWindowText(g_pResourceManager->GetGlobalString(51000008));
	pStatic->Show();

	//WIID_PVPRESERVE_LIST_CHECK + PVP_CHECK_TYPE_FFA_6
	pButton = new SPWindowButton(WIID_PVPRESERVE_LIST_CHECK + PVP_CHECK_TYPE_FFA_6 , 44 -  20, 156 - 122, 12, 12, pParent);
	SetCheckButton(pButton);
	m_pWindowCheck[PVP_CHECK_TYPE_FFA_6] = pButton;

	//WIID_PVPRESERVE_LIST_TEXT + PVP_CHECK_TYPE_FFA_6
	pStatic = new SPWindowStatic( WIID_PVPRESERVE_LIST_TEXT + PVP_CHECK_TYPE_FFA_6, 63 -20,	156 - 122,	185,	12, pParent);	
	pStatic->SetFormat(DT_LEFT | DT_VCENTER);
	pStatic->SetWindowText("PVP_CHECK_TYPE_FFA_6");
	if(g_pResourceManager->GetGlobalString(51000009))
		pStatic->SetWindowText(g_pResourceManager->GetGlobalString(51000009));
	pStatic->Show();

	//WIID_PVPRESERVE_LIST_CHECK + PVP_CHECK_TYPE_FFA_8
	pButton = new SPWindowButton(WIID_PVPRESERVE_LIST_CHECK + PVP_CHECK_TYPE_FFA_8 , 44 -  20, 172 - 122, 12, 12, pParent);
	SetCheckButton(pButton);
	m_pWindowCheck[PVP_CHECK_TYPE_FFA_8] = pButton;

	//WIID_PVPRESERVE_LIST_TEXT + PVP_CHECK_TYPE_FFA_8
	pStatic = new SPWindowStatic( WIID_PVPRESERVE_LIST_TEXT + PVP_CHECK_TYPE_FFA_8, 63 -20,	172 - 122,	185,	12, pParent);	
	pStatic->SetFormat(DT_LEFT | DT_VCENTER);
	pStatic->SetWindowText("PVP_CHECK_TYPE_FFA_8");
	if(g_pResourceManager->GetGlobalString(51000010))
		pStatic->SetWindowText(g_pResourceManager->GetGlobalString(51000010));
	pStatic->Show();

	////WIID_PVPRESERVE_LIST_CHECK + PVP_CHECK_TYPE_FFA_8
	//pButton = new SPWindowButton(WIID_PVPRESERVE_LIST_CHECK + PVP_CHECK_TYPE_FFA_8 , 44 -  20, 172 - 122, 12, 12, pParent);
	//SetCheckButton(pButton);
	//m_pWindowCheck[PVP_CHECK_TYPE_FFA_8] = pButton;
	
	//WIID_PVPRESERVE_SELECT_ALL
	pButton = new SPWindowButton( WIID_PVPRESERVE_SELECT_ALL , 44 - 20, 188 - 122, 63, 17, pParent);
	pButton->SetImage		(OPTION_IMAGE_BUTTON0001, 245,  408);
	pButton->SetImageHit	(OPTION_IMAGE_BUTTON0001, 245,  426);
	pButton->SetImagePush	(OPTION_IMAGE_BUTTON0001, 245,  444);
	pButton->SetImageDisable(OPTION_IMAGE_BUTTON0001, 245,  462);
	pButton->Show();


	//WIID_PVPRESERVE_WARNING
	pStatic = new SPWindowStatic( WIID_PVPRESERVE_WARNING, 30 -2,	221 - 27,	225,	27, pSkin);	
	pStatic->SetFormat(DT_LEFT | DT_TOP);
	pStatic->SetMultiLine(true, 2);
	pStatic->SetWindowText("WIID_PVPRESERVE_WARNING");
	if(g_pResourceManager->GetGlobalString(51000011))
		pStatic->SetWindowText(g_pResourceManager->GetGlobalString(51000011));
	pStatic->Show();

	//WIID_PVPRESERVE_OK
	pButton = new SPWindowButton( WIID_PVPRESERVE_OK , 72 - 2, 260 - 27, 68, 17, pSkin);
	pButton->SetImage		(OPTION_IMAGE_BUTTON0000, 1,  289);
	pButton->SetImageHit	(OPTION_IMAGE_BUTTON0000, 1,  307);
	pButton->SetImagePush	(OPTION_IMAGE_BUTTON0000, 1,  325);
	pButton->SetImageDisable(OPTION_IMAGE_BUTTON0000, 1,  343);
	pButton->Show();

	//WIID_PVPRESERVE_CANCEL
	pButton = new SPWindowButton( WIID_PVPRESERVE_CANCEL , 148 - 2, 260 - 27, 68, 17, pSkin);
	pButton->SetImage		(OPTION_IMAGE_FILE3, 415,  440);
	pButton->SetImageHit	(OPTION_IMAGE_FILE3, 415,  458);
	pButton->SetImagePush	(OPTION_IMAGE_FILE3, 415,  476);
	pButton->SetImageDisable(OPTION_IMAGE_FILE3, 415,  494);
	//pButton->SetImage		(OPTION_IMAGE_BUTTON0000, 1,  145);
	//pButton->SetImageHit	(OPTION_IMAGE_BUTTON0000, 1,  163);
	//pButton->SetImagePush	(OPTION_IMAGE_BUTTON0000, 1,  181);
	//pButton->SetImageDisable(OPTION_IMAGE_BUTTON0000, 1,  199);
	pButton->Show();
}


void SPWindowPvpReserve::SetCheckButton(SPWindowButton* pButton /*= NULL*/)
{
	if(pButton){
		char* OPTION_IMAGE_BUTTON0000 = "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG";		
		pButton->SetImage		(OPTION_IMAGE_BUTTON0000, 484, 79);
		pButton->SetImageHit	(OPTION_IMAGE_BUTTON0000, 497, 79);
		pButton->SetImagePush	(OPTION_IMAGE_BUTTON0000, 484, 92);
		pButton->SetImageCheck	(OPTION_IMAGE_BUTTON0000, 484, 92);
		pButton->Show();
	}
}


void SPWindowPvpReserve::Clean()
{
	SAFE_RELEASE(m_pTextureBase);
	SPWindow::Clean();
}


void SPWindowPvpReserve::Show(bool bWithChild /*= true*/)
{
	// Ok Button Enable
	SPWindowButton* pButton = NULL ;
	pButton = (SPWindowButton*)Find(WIID_PVPRESERVE_OK, true) ;
	if( pButton )
		pButton->SetEnable(true) ;

	SPWindow::Show(bWithChild);
	m_iPvpScale = PVP_SCALE_NONE;
	m_iPvpScale = g_pkPvpManager->GetPVPScale();
	UpdatePvpScale();

}


void SPWindowPvpReserve::Hide(bool bSendServer /*= true*/)
{
	SPWindow::Hide(bSendServer);
}


void SPWindowPvpReserve::Process(float fTime)
{
	SPWindow::Process(fTime);
}


void SPWindowPvpReserve::Render(float fTime)
{
	if(!m_bShow)		return;
	
	if(m_pTextureBase) {
		for(int i = 0; i < WINDOW_BASE_MAX; i++) {
			m_pTextureBase->RenderTexture(&m_rtDestBase[i], &m_rtSrcBase[i]);

////#ifdef _DEBUG
//			//RECT rtRect = {0, 0, 0, 0};			
//			//m_pItemIcon[i]->GetWindowRect(rtRect);
//			g_pVideo->Flush();
//			g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
//			g_pVideo->GetSysLine()->DrawRect(m_rtDestBase[i]);
//			g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
//#endif
		}		
	}
	
	SPWindow::Render(fTime);

}


void SPWindowPvpReserve::UpdatePvpScale()
{	
	m_pWindowCheck[PVP_CHECK_TYPE_FFA_2]->SPSendMessage(SPIM_BTN_SELECT, (m_iPvpScale & PVP_SCALE_2)? TRUE : FALSE);
	m_pWindowCheck[PVP_CHECK_TYPE_FFA_4]->SPSendMessage(SPIM_BTN_SELECT, (m_iPvpScale & PVP_SCALE_4)? TRUE : FALSE);
	m_pWindowCheck[PVP_CHECK_TYPE_FFA_6]->SPSendMessage(SPIM_BTN_SELECT, (m_iPvpScale & PVP_SCALE_6)? TRUE : FALSE);
	m_pWindowCheck[PVP_CHECK_TYPE_FFA_8]->SPSendMessage(SPIM_BTN_SELECT, (m_iPvpScale & PVP_SCALE_8)? TRUE : FALSE);
}


SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowPvpReserve )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_PVPRESERVE_EXIT,			OnExit)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_PVPRESERVE_OK,				OnOK)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_PVPRESERVE_CANCEL,			OnCancel)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_PVPRESERVE_SELECT_ALL,		OnSelectAll)

SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP,	WIID_PVPRESERVE_LIST_CHECK,		WIID_PVPRESERVE_LIST_CHECK + PVP_CHECK_TYPE_MAX,	OnSelect)
SPIMESSAGE_OBJ_MAP_END	( SPWindow	)


int SPWindowPvpReserve::OnExit(WPARAM wParam, LPARAM lParam)
{
	if(g_pkPvpManager->GetPvpStatus() == PVP_STATUS_EMPTY) {
		g_pEventManager->SetCutInState(CUT_IN_STATE_RELEASE);	
		Hide();
	}
	return 1;
}


int SPWindowPvpReserve::OnOK(WPARAM wParam, LPARAM lParam)
{
	////
	//Hide();
	//
	//SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_PVPENTERWAIT);
	//if(pWindow) {
	//	pWindow->Show();
	//}

	if( g_pkPvpManager->SendPVPEnterWait(m_iPvpScale) )
	{
		// If Send packet then Button Disable
		SPWindowButton* pButton = NULL ;
		pButton = (SPWindowButton*)Find(WIID_PVPRESERVE_OK, true) ;
		if( pButton )
			pButton->SetEnable(false) ;
	}

	return 1;
}


int SPWindowPvpReserve::OnCancel(WPARAM wParam, LPARAM lParam)
{		
	if(g_pkPvpManager->GetPvpStatus() == PVP_STATUS_EMPTY) {
		g_pEventManager->SetCutInState(CUT_IN_STATE_RELEASE);	
		Hide();
	}
	
	return 1;
}


int SPWindowPvpReserve::OnSelectAll(WPARAM wParam, LPARAM lParam)
{
	m_iPvpScale |= PVP_SCALE_2;
	m_iPvpScale |= PVP_SCALE_4;
	m_iPvpScale |= PVP_SCALE_6;
	m_iPvpScale |= PVP_SCALE_8;
	UpdatePvpScale();
	return 1;
}


int SPWindowPvpReserve::OnSelect(unsigned int uiID, WPARAM wParam, LPARAM lParam)
{
	int iSelect = uiID - WIID_PVPRESERVE_LIST_CHECK;	
	DXUTOutputDebugString("PVP Select %d\n", iSelect);

	switch(iSelect) {
		case PVP_CHECK_TYPE_FFA_2:
			(m_iPvpScale & PVP_SCALE_2)?	m_iPvpScale &= ~PVP_SCALE_2 : m_iPvpScale |= PVP_SCALE_2;
			m_pWindowCheck[PVP_CHECK_TYPE_FFA_2]->SPSendMessage(SPIM_BTN_SELECT, (m_iPvpScale & PVP_SCALE_2)? TRUE : FALSE);
			break;
		case PVP_CHECK_TYPE_FFA_4:
			(m_iPvpScale & PVP_SCALE_4)?	m_iPvpScale &= ~PVP_SCALE_4 : m_iPvpScale |= PVP_SCALE_4;
			m_pWindowCheck[PVP_CHECK_TYPE_FFA_4]->SPSendMessage(SPIM_BTN_SELECT, (m_iPvpScale & PVP_SCALE_4)? TRUE : FALSE);
			break;
		case PVP_CHECK_TYPE_FFA_6:
			(m_iPvpScale & PVP_SCALE_6)?	m_iPvpScale &= ~PVP_SCALE_6 : m_iPvpScale |= PVP_SCALE_6;
			m_pWindowCheck[PVP_CHECK_TYPE_FFA_6]->SPSendMessage(SPIM_BTN_SELECT, (m_iPvpScale & PVP_SCALE_6)? TRUE : FALSE);
			break;
		case PVP_CHECK_TYPE_FFA_8:
			(m_iPvpScale & PVP_SCALE_8)?	m_iPvpScale &= ~PVP_SCALE_8 : m_iPvpScale |= PVP_SCALE_8;
			m_pWindowCheck[PVP_CHECK_TYPE_FFA_8]->SPSendMessage(SPIM_BTN_SELECT, (m_iPvpScale & PVP_SCALE_8)? TRUE : FALSE);
			break;
		default:
			break;
	}

	return 1;
}



