
#include "SPCommon.h"
#include "SPUtil.h"

#include "SPGameObject.h"
#include "SPGOBManager.h"

#include "SPGameObjectDEF.h"

#include "SPGameObject.h"
#include "SPManager.h"
#include "SPMouseCursor.h"
#include "SPIMEPool.h"
#include "SPSubGameManager.h"

#include "SPWindowDEF.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "Packet.h"
#include "PacketID.h"
#include "SPNetworkManager.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPWindowEdit.h"
#include "SPWindowSlider.h"
#include "SPWindowGift.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPItemCluster.h"

#include "SPGiftAttr.h"
#include "SPGiftArchive.h"
#include "SPGiftManager.h"

#include "SPEventDEF.h"
//#include "SPGameObjectDEF.h"
//#include "SPGameObject.h"
//#include "SPGOBManager.h"
//#include "SPEvent.h"
//#include "SPEventArchive.h"
#include "SPEventManager.h"

#include "SPCheckManager.h"

#include "SPDebug.h"


SPWindowGift::SPWindowGift(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_GIFT, InstanceID, iX, iY, iCX, iCY, pParent)
{	
	Init();
}


SPWindowGift::~SPWindowGift()
{
	Clean();
}


void SPWindowGift::Init()
{
	std::string strTextureFile = "";	
	strTextureFile.clear();	
	strTextureFile = "DATA/INTERFACE/CONCEPT/UI100.PNG";
	g_pVideo->GetTextureMgr()->LoadTexture(strTextureFile.c_str(), &m_pDisable);

	m_bListRequest = false;
	m_fListDeley = 3.0f;
	m_fListAccumulate = 0.0f;

	m_iMaxPage = 0;
	m_iCurPage = 0;
	m_iStartIndex = 0;	
	m_iSelectItem = 0;

	SetRect(&m_rtSrcEly,	1, 298, 10, 308);
	SetRect(&m_rtSrcItem,	12, 298, 21, 308);	

	SetRect(&m_rtSrcDisable, 434 , 37 , 436 , 39);
	SetRect(&m_rtMarkSrc[0], 479, 152, 511, 184);		//ICON_MARK_LOCK
	SetRect(&m_rtMarkSrc[1], 479, 185, 511, 217);		//ICON_MARK_FASHION
	SetRect(&m_rtMarkSrc[2], 479, 339, 511, 371);		//ICON_MARK_EVENT
	SetRect(&m_rtMarkSrc[3], 479, 372, 511, 404);		//ICON_MARK_FANCY

	m_pPage		= NULL;
	m_pPrev		= NULL;
	m_pNext		= NULL;
	m_pReflash	= NULL;
	m_pReceive	= NULL;	
	InitSubWindow();
}


void SPWindowGift::InitSubWindow()
{
	SPWindow* pStatic = NULL;
	SPWindow* pCenter = NULL;
	SPWindow* pLSkin = NULL;
	SPWindow* pRSkin = NULL;
	SPWindow* pParent = NULL;
	SPWindowButton* pButton = NULL;

	pStatic = new SPWindowStatic(WIID_GIFT_TOP, 0, 0, 505, 4, this);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 1,	160);
	pStatic->Show();
	pStatic = NULL;

	pCenter = new SPWindowStatic(WIID_GIFT_CENTER, 0, 4, 505, 304, this);
	pCenter->SetSrcSize(505, 2);
	pCenter->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 1,	166);	
	pCenter->Show();
	
	pStatic = new SPWindowStatic(WIID_GIFT_BOTTOM, 0, 308, 505, 4, this);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 1,	170);
	pStatic->Show();
	pStatic = NULL;

	pStatic = new SPWindowStatic(WIID_GIFT_TITLE, 10, 7, 99, 15, this);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 402,	127);
	pStatic->Show();
	pStatic = NULL;	

	pButton = new SPWindowButton(WIID_GIFT_CLOSE, 239, 8 - 4, 12, 12, pCenter);
	pButton->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 301,	321);
	pButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG", 314,	321); //on
	pButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG", 327,	321); //push
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG", 340,	321); //disable
	pButton->Show();
	pButton = NULL;

	pStatic = new SPWindowStatic(WIID_GIFT_LINE_L_TOP, 2, 26 - 4, 256, 1, pCenter);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 24,	95);
	pStatic->SetSrcSize(4, 1);
	pStatic->Show();
	pStatic = NULL;

	pStatic = new SPWindowStatic(WIID_GIFT_LINE_L_BOTTOM, 2, 284 - 4, 256, 1, pCenter);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 24,	95);
	pStatic->SetSrcSize(4, 1);
	pStatic->Show();
	pStatic = NULL;

	pStatic = new SPWindowStatic(WIID_GIFT_LINE_R_TOP, 266, 26 - 4, 237, 1, pCenter);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 24,	95);
	pStatic->SetSrcSize(4, 1);
	pStatic->Show();
	pStatic = NULL;

	pStatic = new SPWindowStatic(WIID_GIFT_LINE_R_BOTTOM, 266, 284 - 4, 237, 1, pCenter);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 24,	95);
	pStatic->SetSrcSize(4, 1);
	pStatic->Show();
	pStatic = NULL;

	pLSkin = new SPWindowStatic(WIID_GIFT_L_SKIN, 2, 27 - 4, 256, 257, pCenter);
	pLSkin->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 13,	52);
	pLSkin->SetSrcSize(3, 3);
	pLSkin->Show();
	
	pRSkin = new SPWindowStatic(WIID_GIFT_R_SKIN, 266, 27 - 4, 237, 257, pCenter);
	pRSkin->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 13,	52);
	pRSkin->SetSrcSize(3, 3);
	pRSkin->Show();

	//Reflash
	pButton = new SPWindowButton(WIID_GIFT_REFLASH, 182, 289 - 4, 68, 17, pCenter);
	pButton->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 29,	241);
	pButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI101.PNG", 29,	260); //on
	pButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI101.PNG", 29, 279); //push
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI101.PNG", 29,	298); //disable
	pButton->Show();
	m_pReflash = pButton;
	pButton = NULL;

	//Receive
	pButton = new SPWindowButton(WIID_GIFT_RECEIVE, 427, 289 - 4, 68, 17, pCenter);
	pButton->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 98,	241);
	pButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI101.PNG", 98,	260); //on
	pButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI101.PNG", 98, 279); //push
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI101.PNG", 98,	298); //disable
	pButton->Show();
	m_pReceive = pButton;
	pButton = NULL;
	
	
	//////////////////////////////////////////////////////////////////////////	
	pStatic = new SPWindowStatic(WIID_GIFT_REWORD_IMG, 10 - 2, 31 - 27, 40, 15, pLSkin);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 437,	15);
	pStatic->Show();
	pStatic = NULL;

	pStatic = new SPWindowStatic(WIID_GIFT_LIST_IMG, 54 - 2, 31 - 27, 200, 15, pLSkin);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 210,	94);
	pStatic->Show();
	pStatic = NULL;

	//Prev
	pButton = new SPWindowButton(WIID_GIFT_PREV, 103 - 2, 268 - 27, 7, 11, pLSkin);
	pButton->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 238,	378);
	pButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG", 246,	378); //on
	pButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG", 254,	378); //push
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG", 262,	378); //disable
	pButton->Show();
	m_pPrev = pButton;
	pButton = NULL;

	//Page
	pStatic = new SPWindowStatic(WIID_GIFT_PAGE, 113 - 2, 267 - 27, 48, 14, pLSkin);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 226,	345);
	pStatic->SetFormat(DT_CENTER | DT_VCENTER);
	//pStatic->SetWindowText("12/56");
	pStatic->SetFontColor(RGBA(255, 255, 255, 255));
	pStatic->Show();
	m_pPage = pStatic;
	pStatic = NULL;

	//Next
	pButton = new SPWindowButton(WIID_GIFT_NEXT, 165 - 2, 268 - 27, 7, 11, pLSkin);
	pButton->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 269,	378);
	pButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG", 277,	378); //on
	pButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG", 285,	378); //push
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG", 293,	378); //disable
	pButton->Show();
	m_pNext = pButton;
	pButton = NULL;

	int	iButtonY[MAX_GIFT_ITEM_PAGE] = {33, 68, 103, 138};	//배경, 아이콘 공유
	int iTextY[MAX_GIFT_ITEM_PAGE] = {38, 73, 108, 143};	//텍스트 공유
	int iFocusY[MAX_GIFT_ITEM_PAGE] = {52, 87, 122, 157};
	
	for(int i = 0 ; i < MAX_GIFT_ITEM_PAGE ; i++) {
		int iY = 50 + (36 * i);
		//바닥 스킨
		pParent = new SPWindowStatic(WIID_GIFT_LIST_SKIN + i, 10 - 2, iY - 27, 244, 32, pLSkin);
		pParent->Show();

		//버튼
		pButton = new SPWindowButton(WIID_GIFT_LIST_BTN + i, 0, 0, 244, 32, pParent);		
		pButton->SetLightMap(true);
		pButton->Show();
		m_pListButton[i] = pButton;
		pButton = NULL;

		//Icon Line
		pStatic = new SPWindowStatic(WIID_GIFT_LIST_ICON_LINE + i, 4, 0, 32, 32, pParent);
		pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 203,	375);
		pStatic->Show();
		pStatic = NULL;

		//
		SPWindowStatic* pStaticWnd = NULL;
		pStaticWnd = new SPWindowStatic(WIID_GIFT_LIST_SEL + i, 0, 0, 244, 32, pParent);
		pStaticWnd->SetWindowType(WND_TYPE_LEFTBASE);
		pStaticWnd->SetImageNormalHead	("DATA/INTERFACE/CONCEPT/UI101.PNG", 486, 373, 488, 405);
		pStaticWnd->SetImageNormalBody	("DATA/INTERFACE/CONCEPT/UI101.PNG", 490, 373, 492, 405);
		pStaticWnd->SetImageNormalTail	("DATA/INTERFACE/CONCEPT/UI101.PNG", 494, 373, 496, 405);		
		pStaticWnd->Show();		
		m_pListSelect[i] = pStaticWnd;		

		//Icon
		pStatic = new SPWindowStatic(WIID_GIFT_LIST_ICON + i, 4, 0, 32, 32, pParent);
		pStatic->Show();		
		m_pListIcon[i] = pStatic;
		m_pListIcon[i]->GetWindowRect(m_rtStack[i]);
		pStatic = NULL;

		//Text
		pStatic = new SPWindowStatic(WIID_GIFT_LIST_NAME + i, 47, 10, 197, 12, pParent);
		pStatic->SetFormat(DT_CENTER | DT_VCENTER);
		//pStatic->SetWindowText("123456");
		//pStatic->SetFontColor(RGBA(255, 255, 255, 255));
		pStatic->Show();
		m_pListName[i] = pStatic;
		pStatic = NULL;

		////Ely Icon
		//pStatic = new SPWindowStatic(WIID_GIFT_LIST_ELY + i, 123, 0 + 19, 10, 11, pParent);
		//pStatic->Show();		
		//m_pListEly[i] = pStatic;
		//pStatic = NULL;

		////Item Icon
		//pStatic = new SPWindowStatic(WIID_GIFT_LIST_ITEM + i, 134, 0 + 19, 10, 11, pParent);
		//pStatic->Show();		
		//m_pListItem[i] = pStatic;
		//pStatic = NULL;	

		m_bAble[i]			= true;;
		m_iItemMark[i]		= ICON_MARK_NULL;
		m_iStackCount[i]	= 0;
	}
	

	//////////////////////////////////////////////////////////////////////////	
	//Name
	pStatic = new SPWindowStatic(WIID_GIFT_NAME, 274, 10 - 4, 220, 12, pCenter);
	pStatic->SetFormat(DT_LEFT | DT_VCENTER);
	pStatic->SetWindowText(NULL);
	pStatic->SetFont(FONT_12_BOLD);
	pStatic->SetFontColor(RGBA(143, 99, 99, 255));
	pStatic->Show();
	m_pName = pStatic;
	pStatic = NULL;

	//Text
	pStatic = new SPWindowStatic(WIID_GIFT_TEXT, 276 - 266, 42 - 27, 216, 220, pRSkin);
	pStatic->SetFormat(DT_LEFT | DT_WORDBREAK);
	pStatic->SetWindowText(NULL);
	pStatic->SetFontColor(RGBA(0, 0, 0, 255));
	pStatic->SetMultiLine(true, 5);
	pStatic->Show();
	m_pText = pStatic;
	pStatic = NULL;

	pStatic = new SPWindowStatic(WIID_GIFT_ELY_TEXT_IMG, 329 - 266, 268 - 27, 45, 11, pRSkin);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 437,	31);
	pStatic->Show();
	pStatic = NULL;

	pStatic = new SPWindowStatic(WIID_GIFT_ELY_IMG, 382 - 266, 266 - 27, 113, 14, pRSkin);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 398,	80);
	pStatic->Show();
	pStatic = NULL;

	//Ely
	pStatic = new SPWindowStatic(WIID_GIFT_ELY, 383 - 266, 268 - 27, 84, 12, pRSkin);
	//pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 398,	80);
	pStatic->SetFormat(DT_RIGHT | DT_VCENTER);
	pStatic->SetWindowText(NULL);
	pStatic->SetFontColor(RGBA(143, 99, 99, 255));
	pStatic->Show();
	m_pEly = pStatic;
	pStatic = NULL;	
}


void SPWindowGift::Clean()
{
	for(int i = 0; i < MAX_GIFT_ITEM_PAGE; i++) {
		m_pListIcon[i]->SPSendMessage(SPIM_SET_IMAGECLEAR);		
	}	
	SAFE_RELEASE(m_pDisable);

	SPWindow::Clean();
}


void SPWindowGift::Process(float fTime)
{	
	SPWindow::Process(fTime);

	if(m_bListRequest) {
		m_fListAccumulate += fTime;
		if( m_fListAccumulate > m_fListDeley) {
			m_fListAccumulate -= m_fListDeley;
			m_pReflash->SetEnable(true);
			m_bListRequest = false;
		}
	}
}


void SPWindowGift::Render(float fTime)
{
	if(IsShow() == false) return;
	
	SPWindow::Render(fTime);
	
	//
	for(int i = 0; i < MAX_GIFT_ITEM_PAGE; i++) {
		//Icon Disable 표현
		if(m_pDisable) {
			RECT rtDest;
			m_pListIcon[i]->GetWindowRect(rtDest);
			if(m_bAble[i] == false) {		
				m_pDisable->RenderTexture(&rtDest, &m_rtSrcDisable);
			}
			if(m_iItemMark[i] > ICON_MARK_NULL) {
				m_pDisable->RenderTexture(&rtDest, &m_rtMarkSrc[m_iItemMark[i] - 1]);
			}
		}

		//Stack Count Local 표현
		if(m_iStackCount[i] > 9) {
			RECT rtTenNumTarget, rtNumTarget, rtTenNumSrc, rtNumSrc;
			int iTenStack = m_iStackCount[i] / 10;
			int iOneStack = m_iStackCount[i] % 10;

			SetRect(&rtTenNumTarget, m_rtStack[i].left, m_rtStack[i].bottom - 10, m_rtStack[i].left + 9, m_rtStack[i].bottom);
			SetRect(&rtNumTarget, rtTenNumTarget.right - 2, m_rtStack[i].bottom - 10, rtTenNumTarget.right + 9 - 2, m_rtStack[i].bottom);

			rtTenNumSrc.left = iTenStack * 9;
			rtTenNumSrc.right = rtTenNumSrc.left + 9;
			rtTenNumSrc.top = 0;
			rtTenNumSrc.bottom = 10;

			rtNumSrc.left = iOneStack * 9;
			rtNumSrc.right = rtNumSrc.left + 9;
			rtNumSrc.top = 0;
			rtNumSrc.bottom = 10;

			g_pItemCluster->GetNumTexture()->RenderTexture( &rtTenNumTarget , &rtTenNumSrc);
			g_pItemCluster->GetNumTexture()->RenderTexture( &rtNumTarget , &rtNumSrc);
		}
		else if(m_iStackCount[i] > 1) {
			int iOneStack = m_iStackCount[i] % 10;
			RECT rtNumTarget, rtNumSrc;
			SetRect(&rtNumTarget, m_rtStack[i].left, m_rtStack[i].bottom - 10, m_rtStack[i].left + 9, m_rtStack[i].bottom);

			rtNumSrc.left = iOneStack * 9;
			rtNumSrc.right = rtNumSrc.left + 9;
			rtNumSrc.top = 0;
			rtNumSrc.bottom = 10;

			g_pItemCluster->GetNumTexture()->RenderTexture( &rtNumTarget , &rtNumSrc);
		}
	}

	//RECT rtText;
	//m_pText->GetWindowRect(rtText);
	//g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	//g_pVideo->GetSysLine()->DrawRect(rtText);
	//g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}


void SPWindowGift::Show(bool bWithChild /*= true*/)
{	
	//한번도 Gift List를 받지 않은 경우 List 요청
	if(bWithChild && g_pEventManager->GetGiftManager()->IsGiftListLoad() == false) {
		g_pEventManager->GetGiftManager()->SendGiftList();
		m_bListRequest = true;
		m_fListAccumulate = 0.0f;
		//m_pReflash->SetEnable(false);
		m_pReflash->SetEnable(true);
		m_iSelectItem = -1;
	}
	else {
		m_bListRequest = false;
		//m_bListRequest = true;
		m_fListAccumulate = 0.0f;
		m_pReflash->SetEnable(true);
		//m_pReflash->SetEnable(false);
	}
	
	SPWindow::Show(bWithChild);
	
	OnUpdateGiftList((WPARAM)0, (LPARAM)0);	
}


void SPWindowGift::UpdateList()
{	
	char szTemp[32];
	ZeroMemory(szTemp, 32);
	if(m_iMaxPage > 0) {
		wsprintf(szTemp, "%d/%d", m_iCurPage + 1, m_iMaxPage);
	}	
	else if(m_iMaxPage == 0 && m_iCurPage == 0) {
		wsprintf(szTemp, "%d/%d", m_iCurPage, m_iMaxPage);

	}
	m_pPage->SetWindowText(szTemp);
	
	int iStartIndex = m_iCurPage * MAX_GIFT_ITEM_PAGE;	
	SPGiftAttr* pGiftAttr = NULL;

	for(int i = 0; i < MAX_GIFT_ITEM_PAGE; i++, iStartIndex++) {
		//SPWindow* pkWindow = Find(WIID_GIFT_LIST_BTN + i, true);
		//if( pkWindow == NULL )
		//	continue;
		//pkWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE);
		
		m_pListSelect[i]->Hide();
		
		m_pListName[i]->SetWindowText(NULL);
		m_pListIcon[i]->SPSendMessage(SPIM_SET_IMAGECLEAR);
		//m_pListEly[i]->SPSendMessage(SPIM_SET_IMAGECLEAR);
		//m_pListItem[i]->SPSendMessage(SPIM_SET_IMAGECLEAR);
		m_bAble[i]			= true;;
		m_iItemMark[i]		= ICON_MARK_NULL;
		m_iStackCount[i]	= 0;
		
		pGiftAttr = g_pEventManager->GetGiftManager()->GetGift(iStartIndex);
		if(pGiftAttr == NULL || pGiftAttr->m_iKey == 0) 
			continue;
		
		m_pListName[i]->SetWindowText(pGiftAttr->m_strName.c_str());		
		
		if(pGiftAttr->m_iItemID != 0) {
			POINT ptSrcPos;
			SPTexture* pkTexture = g_pItemCluster->GetItemTexture(pGiftAttr->m_iItemID, ptSrcPos);
			m_pListIcon[i]->Hide();
			m_pListIcon[i]->SetImage(pkTexture, ptSrcPos.x, ptSrcPos.y);
			m_pListIcon[i]->Show();
	
			//m_pListItem[i]->Hide();
			//m_pListItem[i]->SetImage(m_pTexture, m_rtSrcItem.left, m_rtSrcItem.top);
			//m_pListItem[i]->Show();
			
			m_bAble[i] = g_pCheckManager->CheckItemStatus(pGiftAttr->m_iItemID);
			m_iItemMark[i] = g_pItemCluster->GetItemIconMark(pGiftAttr->m_iItemID);
			m_iStackCount[i] = pGiftAttr->m_iItemStack;
		}

		if(pGiftAttr->m_iEly > 0) {
			//m_pListEly[i]->Hide();
			//m_pListEly[i]->SetImage(m_pTexture, m_rtSrcEly.left, m_rtSrcEly.top);
			//m_pListEly[i]->Show();
		}
	}

	m_iSelectItem = -1;

	for(int i = 0 ; i < MAX_GIFT_ITEM_PAGE ; i++ ){		
		m_pListSelect[i]->Hide();
	}

	m_pName->SetWindowText(NULL);
	m_pText->SetWindowText(NULL);
	m_pEly->SetWindowText(NULL);
}


//////////////////////////////////////////////////////////////////////////
SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowGift )
SPIMESSAGE_COMMAND(SPIM_GIFT_LIST_UPDATE,		OnUpdateGiftList )
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_GIFT_REFLASH	, OnListReflash)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_GIFT_RECEIVE	, OnItemReceive)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_GIFT_CLOSE		, OnClose)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_GIFT_PREV		, OnPrev)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_GIFT_NEXT		, OnNext)
SPIMESSAGE_CONTROL_RANGE(SPIM_LBUTTON_UP, WIID_GIFT_LIST_BTN, WIID_GIFT_LIST_BTN + MAX_GIFT_ITEM_PAGE, OnItemClick)

SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_ENTER,	WIID_GIFT_LIST_BTN,		WIID_GIFT_LIST_BTN + MAX_GIFT_ITEM_PAGE,	OnToolTipEnter	)
SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_OUT,		WIID_GIFT_LIST_BTN,		WIID_GIFT_LIST_BTN + MAX_GIFT_ITEM_PAGE,	OnToolTipOut	)
SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_MOVE,	WIID_GIFT_LIST_BTN,		WIID_GIFT_LIST_BTN + MAX_GIFT_ITEM_PAGE,	OnToolTipMove	)
SPIMESSAGE_OBJ_MAP_END	( SPWindow	)

int SPWindowGift::OnUpdateGiftList(WPARAM wParam, LPARAM lParam)
{
	m_iSelectItem	= -1;
	m_iMaxPage		= 0;
	m_iCurPage		= 0;
	m_iStartIndex	= 0;
	int iSize = g_pEventManager->GetGiftManager()->GetGiftListSize();
	m_iMaxPage = iSize / MAX_GIFT_ITEM_PAGE;
	if(iSize % MAX_GIFT_ITEM_PAGE > 0)
		m_iMaxPage++;
	
	if(m_iMaxPage > 1) {
		m_pNext->SetEnable(true);
	}
	else /*if(m_iMaxPage == 0)*/ {
		m_pNext->SetEnable(false);
	}
	
	if(m_iCurPage == 0) {
		m_pPrev->SetEnable(false);
	}
	
    UpdateList();

	OnItemClick(WIID_GIFT_LIST_BTN, 0,0);				//[2006/6/28]

	return 1;
}


int SPWindowGift::OnListReflash(WPARAM wParam, LPARAM lParam)
{	
	m_bListRequest = true;
	m_fListAccumulate = 0.0f;
	m_pReflash->SetEnable(false);
	g_pEventManager->GetGiftManager()->SendGiftList();
	m_iSelectItem = -1;
	return 1;
}


int SPWindowGift::OnClose(WPARAM wParam, LPARAM lParam)
{
	g_pEventManager->SetCutInState(CUT_IN_STATE_RELEASE);
	return 1;
}


int SPWindowGift::OnPrev(WPARAM wParam, LPARAM lParam)
{
	m_iCurPage--;
	
	if(m_iCurPage < 1) {		
		m_iCurPage = 0;
		m_pPrev->SetEnable(false);
		//UpdateList();
		//return 1;
	}

	if(m_iMaxPage > 1) {
		m_pNext->SetEnable(true);
	}

	UpdateList();	
	
	return 1;
}


int SPWindowGift::OnNext(WPARAM wParam, LPARAM lParam)
{
	m_iCurPage++;

	if(m_iCurPage + 1 >= m_iMaxPage) {
		//m_iCurPage--;
		m_pNext->SetEnable(false);
		//UpdateList();
		//return 1;
	}

	if(m_iCurPage > 0) {
		m_pPrev->SetEnable(true);
	}
	
	UpdateList();
	return 1;
}


int SPWindowGift::OnItemClick(unsigned int uiID, WPARAM wParam, LPARAM lParam)
{
	m_iSelectItem = uiID - WIID_GIFT_LIST_BTN;	
	assert(m_iSelectItem < MAX_GIFT_ITEM_PAGE && "Gift List 인덱스 에러");	

	DXUTOutputDebugString("Gift Select[%d]\n", m_iSelectItem);

	//control select
	for(int i = 0 ; i < MAX_GIFT_ITEM_PAGE ; i++ ){
		//SPWindow* pkWindow = Find(WIID_GIFT_LIST_BTN + i, true);
		//if( pkWindow == NULL )
		//	continue;

		//if( uiID == (WIID_GIFT_LIST_BTN + i) )
		//	pkWindow->SPSendMessage(SPIM_BTN_SELECT, TRUE);
		//else
		//	pkWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE);

		if(m_iSelectItem == i) {
			m_pListSelect[i]->Show();
		}
		else {
			m_pListSelect[i]->Hide();
		}
	}

	int iSelectIndex = (m_iCurPage * MAX_GIFT_ITEM_PAGE) + m_iSelectItem;
	SPGiftAttr* pGiftAttr = g_pEventManager->GetGiftManager()->GetGift(iSelectIndex);
	if(pGiftAttr == NULL || pGiftAttr->m_iKey == 0){
		m_iSelectItem = -1;
		m_pName->SetWindowText(NULL);
		m_pText->SetWindowText(NULL);
		m_pEly->SetWindowText(NULL);
		return 1;
	}

	m_pName->SetWindowText(pGiftAttr->m_strName.c_str());
	m_pText->SetWindowText(pGiftAttr->m_strDesc.c_str());
	
	//char szEly[32];
	//ZeroMemory(szEly, 32);
	//wsprintf(szEly, "%d", pGiftAttr->m_iEly);
	m_pEly->SetWindowText(ConvertMoneyToString(pGiftAttr->m_iEly));
	return 1;
}


int SPWindowGift::OnItemReceive(WPARAM wParam, LPARAM lParam)
{	
	if(m_iSelectItem < 0 || m_iSelectItem >= MAX_GIFT_ITEM_PAGE) {
		DXUTOutputDebugString("OnItemReceive Item Select Error");
		return 1;
	}
	
	int iSelectIndex = (m_iCurPage * MAX_GIFT_ITEM_PAGE) + m_iSelectItem;

	g_pEventManager->GetGiftManager()->SendGetGiftItem(iSelectIndex);

	return 1;
}


int SPWindowGift::OnToolTipMove(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	int iCurIndex = iUiID - WIID_GIFT_LIST_BTN;	
	iCurIndex = (m_iCurPage * MAX_GIFT_ITEM_PAGE) + iCurIndex;

	//iCurIndex += (m_iCurPage * 4);

	unsigned int uiUniqueID;
	int iEquipClass, iItemID, iItemColor;

	SPGiftAttr* pGiftAttr = g_pEventManager->GetGiftManager()->GetGift(iCurIndex);
	if(pGiftAttr == NULL || pGiftAttr->m_iKey == 0 || pGiftAttr->m_iItemID == 0) {
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);		
		return 1;
	}

	//if(m_pNpcShop == NULL || m_pNpcShop->m_NpcSellItem[iCurIndex].m_iItemID == 0)	return 1;

	//uiUniqueID = m_ContainerItem[iCurIndex].Item.iItemID;
	uiUniqueID = pGiftAttr->m_iItemID;	
	iItemColor = pGiftAttr->m_iItemID % 100;
	iEquipClass = pGiftAttr->m_iItemID / 100000;
	iItemID = (pGiftAttr->m_iItemID % 100000 - iItemColor) / 100;	

	SPItemStatus kItemStatus;
	kItemStatus.SetItemID(uiUniqueID);

	SPItemAttr* pItemAttr;
	g_pItemCluster->GetItemInfo(uiUniqueID, pItemAttr);
	if( pItemAttr )
	{
		kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPID, pItemAttr->m_ePosID1);
		kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPCLASS, iEquipClass);
		kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPITEMID, iItemID);
		kItemStatus.SPSendMessage(MV_ITEM_SETCOLOR,	iItemColor);
		kItemStatus.SPSendMessage(MV_ITEM_SET_RARELEVEL, pGiftAttr->m_iItemRare);
		if(pItemAttr->m_eDuration != ITEM_DURATION_NULL) {
			kItemStatus.SetLifeTime(pItemAttr->m_iLifeTime * 3600);
		}

		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage(SPIM_TOOLTIP_MOVE, (WPARAM)&kItemStatus, lParam);
		pWindow->Show();
	}

	return 1;
}


int SPWindowGift::OnToolTipEnter(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	int iCurIndex = iUiID - WIID_GIFT_LIST_BTN;	
	iCurIndex = (m_iCurPage * MAX_GIFT_ITEM_PAGE) + iCurIndex;

	//iCurIndex += (m_iCurPage * 4);

	unsigned int uiUniqueID;
	int iEquipClass, iItemID, iItemColor;

	SPGiftAttr* pGiftAttr = g_pEventManager->GetGiftManager()->GetGift(iCurIndex);
	if(pGiftAttr == NULL || pGiftAttr->m_iKey == 0 || pGiftAttr->m_iItemID == 0) {
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);		
		return 1;
	}
	
	uiUniqueID = pGiftAttr->m_iItemID;	
	iItemColor = pGiftAttr->m_iItemID % 100;
	iEquipClass = pGiftAttr->m_iItemID / 100000;
	iItemID = (pGiftAttr->m_iItemID % 100000 - iItemColor) / 100;	

	SPItemStatus kItemStatus;
	kItemStatus.SetItemID(uiUniqueID);

	SPItemAttr* pItemAttr;
	g_pItemCluster->GetItemInfo(uiUniqueID, pItemAttr);
	if( pItemAttr )
	{
		kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPID, pItemAttr->m_ePosID1);
		kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPCLASS, iEquipClass);
		kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPITEMID, iItemID);
		kItemStatus.SPSendMessage(MV_ITEM_SETCOLOR,	iItemColor);
		kItemStatus.SPSendMessage(MV_ITEM_SET_RARELEVEL, pGiftAttr->m_iItemRare);
		if(pItemAttr->m_eDuration != ITEM_DURATION_NULL) {
			kItemStatus.SetLifeTime(pItemAttr->m_iLifeTime * 3600);
		}

		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage(SPIM_TOOLTIP_MOVE, (WPARAM)&kItemStatus, lParam);
		pWindow->Show();
	}

	return 1;
}


int SPWindowGift::OnToolTipOut(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
	pWindow->Show();
	return 1;
}

