
#include <vector>
#include <string>

#include "SPCommon.h"
#include "SPUtil.h"

#include "SPMainGameDEF.H"
#include "SPGameObjectDEF.h"

#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPSubGameManager.h"
#include "SPGameObject.h"
#include "SPManager.h"
#include "SPMouseCursor.h"
#include "SPIMEPool.h"
#include "SPWindowDEF.h"
#include "SPInterfaceManager.h"

#include "SPWindow.h"
#include "SPWindowButton.h"
#include "SPWindowStatic.h"
#include "SPWindowEdit.h"

#include "Packet.h"
#include "SPNetworkManager.h"

#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPItem.h"
#include "SPItemCluster.h"			//[2005/6/16] - jinhee

#include "SPEventDEF.h"
#include "SPEventNpcHandler.h"
#include "SPEventManager.h"

#include "SPMainGameDEF.H"
#include "SPSubGameManager.h"
#include "SPMainGameManager.h"
#include "SPMouseCursor.h"
#include "SPWindowDEF.h"
#include "SPInterfaceManager.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGOBModel.h"
#include "SPGOBModelUnit.h"
#include "SPMonsterModelUnit.h"
#include "SPAvatarModelUnit.h"
#include "SPGOBCluster.h"
#include "SPGameObject.h"
#include "SPGOBStatus.h"
#include "SPGOBCoordPhysics.h"
#include "SPGOBModelUnit.h"			
#include "SPPlayerEquipInfo.h"
#include "SPGOBStatus.h"			
#include "SPPlayerInvenArchive.h"	
#include "SPPlayerStatusModel.h"	
#include "SPPlayerEquipInfo.h"
#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPPlayer.h"
#include "SPGOBManager.h"

#include "SPMainGameManager.h"

#include "SPCheckManager.h"
#include "SPLocalizeManager.h"

#include "SPWindowNpcTrade.h"

#include "SPUtil.h"

#include "SPDebug.h"

SPWindowNpcTrade::SPWindowNpcTrade(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent /*= NULL*/)
: SPWindow(WNDID_SHOPTRADE, InstanceID, iX, iY, iCX, iCY, pParent)
{
	Init();
}


SPWindowNpcTrade::~SPWindowNpcTrade()
{
	Clean();
}

void SPWindowNpcTrade::Init()
{
	m_pNpcShop = NULL;
	m_iSelectItemIndex = -1;
	m_iViewItemIndex = -1;
	m_iBuyCount = 1;
	m_iPage = 1;
	m_iMaxPage = 1;
	ZeroMemory(m_szPage, 64);
	ZeroMemory(m_szBuyCount, 64);

	m_bSend = false;

	//Load Texture Image
	std::string strTextureFile = "";
	strTextureFile = "DATA/INTERFACE/CONCEPT/UI100.PNG";
	g_pVideo->GetTextureMgr()->LoadTexture(strTextureFile.c_str(), &m_pDisable);

	SetRect(&m_rtSrcDisable, 434 , 37 , 436 , 39);
	SetRect(&m_rtMarkSrc[0], 479, 152, 511, 184);		//ICON_MARK_LOCK
	SetRect(&m_rtMarkSrc[1], 479, 185, 511, 217);		//ICON_MARK_FASHION
	SetRect(&m_rtMarkSrc[2], 479, 339, 511, 371);		//ICON_MARK_EVENT
	SetRect(&m_rtMarkSrc[3], 479, 372, 511, 404);		//ICON_MARK_FANCY

	CreateSubControl();	

	m_PickUpItem.Clear();

	//Hide();
}


void SPWindowNpcTrade::CreateSubControl()
{
	//컨트롤
	SPWindow* pStatic = NULL;
	SPWindow* pCenter = NULL;
	SPWindow* pMiddle = NULL;
	SPWindowButton* pButton;	

	pStatic = new SPWindowStatic(WIID_NPCTRADE_TOP, 0, 0, 319, 4, this);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 1,	16);
	pStatic->Show();
	pStatic = NULL;

	pCenter = new SPWindowStatic(WIID_NPCTRADE_CENTER, 0, 4, 319, 469, this);
	pCenter->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 1,	22);
	pCenter->SetSrcSize(319, 2);
	pCenter->Show();

	pStatic = new SPWindowStatic(WIID_NPCTRADE_BOTTOM, 0, 473, 319, 4, this);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 1,	26);
	pStatic->Show();
	pStatic = NULL;

	pStatic = new SPWindowStatic(WIID_NPCTRADE_TITLE, 10, 7 - 4, 57, 15, pCenter);
	pStatic->SetImage		("DATA/INTERFACE/CONCEPT/UI103.PNG", 59,	360); //일반
	pStatic->Show();
	pStatic = NULL;

	pButton = new SPWindowButton(WIID_NPCTRADE_EXIT, 298, 9 - 4, 12, 12, pCenter);
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI103.PNG", 301,	321); //일반
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI103.PNG", 314,	321); //on	
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI103.PNG", 327,	321); //push
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG", 340,	321); //disable
	pButton = NULL;

	pMiddle = new SPWindowStatic(WIID_NPCTRADE_MIDDLE, 2, 27 - 4, 315, 419, pCenter);
	pMiddle->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 493,	477);
	pMiddle->SetSrcSize(6, 6);
	pMiddle->Show();

	SPWindowStatic* pStaticWnd = NULL;
	pStaticWnd = new SPWindowStatic(WIID_NPCTRADE_LIST_IMG_BASE, 10 - 2, 34 - 27, 220, 15, pMiddle);
	pStaticWnd->SetWindowType(WND_TYPE_LEFTBASE);
	pStaticWnd->SetImageNormalHead	("DATA/INTERFACE/CONCEPT/UI103.PNG", 347, 245, 349, 260);
	pStaticWnd->SetImageNormalBody	("DATA/INTERFACE/CONCEPT/UI103.PNG", 351, 245, 353, 260);
	pStaticWnd->SetImageNormalTail	("DATA/INTERFACE/CONCEPT/UI103.PNG", 355, 245, 357, 260);
	pStaticWnd->Show();
	pStaticWnd = NULL;

	pStatic = new SPWindowStatic(WIID_NPCTRADE_LIST_IMG, 75 - 2, 36 - 27, 90, 11, pMiddle);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 355,	223);
	pStatic->Show();
	pStatic = NULL;

	pStaticWnd = new SPWindowStatic(WIID_NPCTRADE_PRICE_IMG_BASE, 232 - 2, 34 - 27, 77, 15, pMiddle);
	pStaticWnd->SetWindowType(WND_TYPE_LEFTBASE);
	pStaticWnd->SetImageNormalHead	("DATA/INTERFACE/CONCEPT/UI103.PNG", 347, 245, 349, 260);
	pStaticWnd->SetImageNormalBody	("DATA/INTERFACE/CONCEPT/UI103.PNG", 351, 245, 353, 260);
	pStaticWnd->SetImageNormalTail	("DATA/INTERFACE/CONCEPT/UI103.PNG", 355, 245, 357, 260);
	pStaticWnd->Show();
	pStaticWnd = NULL;

	pStatic = new SPWindowStatic(WIID_NPCTRADE_PRICE_IMG, 260 - 2, 36 - 27, 21, 11, pMiddle);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 424,	211);
	pStatic->Show();
	pStatic = NULL;

	
	//중단	
	//int	iSkinY[TRADE_PAGE] = {58-27, 94-27, 103-27, 166-27, 202-27, 238-27, 274-27, 310-27, 346-27, 382-27};	//배경, 아이템명, 아이콘 공유
	int iSkinY = 58 - 27;
	int iCashY[TRADE_PAGE] = {59-42, 94-77, 129-112, 164-147, 199-182, 234-217};	//가격
	for(int i = 0 ; i < TRADE_PAGE ; i++) {
		iSkinY = (58 - 27) + (36 * i);
		
		SPWindow* pSkin = NULL;

		//바닥 스킨
		pSkin = new SPWindowStatic(WIID_NPCTRADE_ITEM_SKIN + i, 2, iSkinY, 307, 32, pMiddle);		
		pSkin->Show();		

		//버튼 처리
		pButton = new SPWindowButton(WIID_NPCTRADE_ITEM_BUTTON + i, 0, 0, 319-4, 32, pSkin);		
		pButton->Show();
		m_pButton[i] = pButton;
		pButton = NULL;

		m_pSkin[i] = new SPWindowStatic(WIID_NPCTRADE_ITEM_SEL + i, 0, 0, 307, 32, pSkin);
		m_pSkin[i]->SetWindowType(WND_TYPE_LEFTBASE);
		m_pSkin[i]->SetImageNormalHead	("DATA/INTERFACE/CONCEPT/UI103.PNG", 258, 407, 260, 439);
		m_pSkin[i]->SetImageNormalBody	("DATA/INTERFACE/CONCEPT/UI103.PNG", 262, 407, 264, 439);
		m_pSkin[i]->SetImageNormalTail	("DATA/INTERFACE/CONCEPT/UI103.PNG", 266, 407, 268, 439);
		m_pSkin[i]->Show();		

		pStatic = new SPWindowStatic(WIID_NPCTRADE_ITEM_GRID + i, 10, 0, 32, 32, pSkin);
		pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 203, 408);
		pStatic->Show();
		pStatic = NULL;

		//아이템 아이콘
		//pStatic = new SPWindowStatic(WIID_NPCTRADE_ITEM_ICON + i, 6, iSkinY[i], 32, 32, pSkin);
		pStatic = new SPWindowStatic(WIID_NPCTRADE_ITEM_ICON + i, 10, 0, 32, 32, pSkin);		
		pStatic->Show();
		m_pIcon[i] = pStatic;
		m_pIcon[i]->GetWindowRect(m_rtStack[i]);
		pStatic = NULL;
		
		m_iStack[i] = 0;
		m_bAble[i] = true;
		m_iMark[i] = ICON_MARK_NULL;
	
		//이름
		//pStatic = new SPWindowStatic(WIID_NPCTRADE_ITEM_NAME + i, 40, iSkinY[i], 115, 16, pSkin);
		pStatic = new SPWindowStatic(WIID_NPCTRADE_ITEM_NAME + i, 50, 10, 178, 12, pSkin);
		pStatic->SetFormat(DT_LEFT | DT_VCENTER);
		pStatic->SetTextPeriod(true);
		//pStatic->SetWindowText("아이템명");
		pStatic->Show();
		m_pName[i] = pStatic;
		pStatic = NULL;
		
		//돈
		//pStatic = new SPWindowStatic(WIID_NPCTRADE_ITEM_CASH + i, 63, iCashY[i], 92, 16, pSkin);
		pStatic = new SPWindowStatic(WIID_NPCTRADE_ITEM_CASH + i, 230, 10, 56, 12, pSkin);
		pStatic->SetFormat(DT_RIGHT | DT_VCENTER);
		pStatic->SetWindowText("123456");
		pStatic->Show();
		//pStatic->SetFontColor(RGBA(255, 255, 255, 255));
		pStatic->SetFontColor(RGBA(0, 0, 0, 255));
		m_pCash[i] = pStatic;
		pStatic = NULL;	

		//Ely Image
		pStatic = new SPWindowStatic(WIID_NPCTRADE_ITEM_ELY_IMG + i, 290, 14, 17, 5, pSkin);
		pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 494, 506);
		pStatic->Show();
		pStatic = NULL;
	}
	

	//하단
	//페이지 감소
	pButton = new SPWindowButton(WIID_NPCTRADE_PAGEDOWN, 126 - 2 , 424 - 27, 7, 11, pMiddle);
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI103.PNG", 238,	378); //일반
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI103.PNG", 246,	378); //on	
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI103.PNG", 254,	378); //push
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG", 262,	378); //disable
	m_pPageDown = pButton;
	pButton->Show();
	pButton = NULL;

	//페이지 표시
	pStatic = new SPWindowStatic(WIID_NPCTRADE_PAGE,	136 - 2, 422 - 27, 48, 14, pMiddle);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 226,	345);
	pStatic->SetFontColor(RGBA(255, 255, 255, 255));
	pStatic->SetFormat(DT_CENTER | DT_VCENTER);
	pStatic->SetWindowText(" / ");
	m_pPageInfo = pStatic;
	pStatic->Show();
	pStatic = NULL;

	//페이지 증가
	pButton = new SPWindowButton(WIID_NPCTRADE_PAGEUP, 188 - 2 , 424 - 27, 7, 11, pMiddle);
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI103.PNG", 269,	378); //일반
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI103.PNG", 277,	378); //on	
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI103.PNG", 285,	378); //push
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG", 293,	378); //disable
	m_pPageUp = pButton;
	pButton->Show();
	pButton = NULL;

	pStatic = new SPWindowStatic(WIID_USER_ELY_TEXT_IMG, 10 - 2, 456 - 27, 34, 11, pMiddle);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 301,	378);
	pStatic->Show();
	pStatic = NULL;

	pStatic = new SPWindowStatic(WIID_USER_ELY, 50 - 2, 454 - 27, 121, 14, pMiddle);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 104, 345);
	pStatic->SetFontColor(RGBA(143, 99, 99, 255));
	pStatic->SetFormat(DT_RIGHT | DT_VCENTER);
	pStatic->Show();
	m_pUserEly = pStatic;
	pStatic = NULL;

	pStatic = new SPWindowStatic(WIID_USER_ELY_IMG, 175 - 2, 460 - 27, 17, 5, pMiddle);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 494, 506);
	pStatic->Show();
	pStatic = NULL;

	//구매 버튼
	pButton = new SPWindowButton(WIID_NPCTRADE_BUY, 241 - 2, 452 - 27, 68, 17, pMiddle);
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI103.PNG", 70,	215); //일반
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI103.PNG", 70,	233); //on	
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI103.PNG", 70,	251); //push
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG", 70,	269); //disable
	m_pBuy = pButton;
	pButton->Show();
	pButton = NULL;
}


void SPWindowNpcTrade::Clean()
{
	SAFE_RELEASE(m_pDisable);
	m_pDisable = NULL;

	SPWindow::Clean();
}


void SPWindowNpcTrade::Show(bool bWithChild)
{
	//여기로 오면 내부 상점 판매 아이템 리스트를 업데이트 한다
	m_iMaxPage = 1;
	m_iPage = 1;
	
	SPWindow::Show(bWithChild);
}


void SPWindowNpcTrade::Process(float fTime)
{	
	if(m_bShow && m_pUserEly) {
		//g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValueEX()
		if( g_pGOBManager->GetLocalPlayer() ) {
			//itoa(g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValueEX(STATUS_EX_ELY), m_szEly, 10);
			m_pUserEly->SetWindowText(ConvertMoneyToString(g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValueEX(STATUS_EX_ELY)));
		}
		else
			return;

		//m_pMessage->SetWindowText(m_szEly);		
	}
	SPWindow::Process(fTime);
}


void SPWindowNpcTrade::Render(float fTime)
{	
	if(IsShow() == false) return;	// by metalgeni 차일드 윈도우도 렌더하면 안됩니다..	

	SPWindow::Render(fTime);

	for(int i = 0; i < TRADE_PAGE; i++) {
		if(m_pDisable) {
			RECT rtRect;
			m_pIcon[i]->GetWindowRect(rtRect);
			
			if(m_bAble[i] == false) {				//아이콘 위에 살짝 비활성화				
				m_pDisable->RenderTexture(&rtRect, &m_rtSrcDisable);
			}

			if(m_iMark[i] > ICON_MARK_NULL) {				
				m_pDisable->RenderTexture(&rtRect, &m_rtMarkSrc[ (m_iMark[i] - 1) ]);
			}
		}

		//Stack Count Local 표현
		if(m_iStack[i] > 9) {
			RECT rtTenNumTarget, rtNumTarget, rtTenNumSrc, rtNumSrc;
			int iTenStack = m_iStack[i] / 10;
			int iOneStack = m_iStack[i] % 10;

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
		else if(m_iStack[i] > 1) {
			int iOneStack = m_iStack[i] % 10;
			RECT rtNumTarget, rtNumSrc;
			SetRect(&rtNumTarget, m_rtStack[i].left, m_rtStack[i].bottom - 10, m_rtStack[i].left + 9, m_rtStack[i].bottom);

			rtNumSrc.left = iOneStack * 9;
			rtNumSrc.right = rtNumSrc.left + 9;
			rtNumSrc.top = 0;
			rtNumSrc.bottom = 10;

			g_pItemCluster->GetNumTexture()->RenderTexture( &rtNumTarget , &rtNumSrc);
		}
	}

#ifdef _DEBUG
	RECT rtRect = {0, 0, 0, 0};
	for(int i = 0; i < TRADE_PAGE; i++) {
		m_pName[i]->GetWindowRect(rtRect);
		g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		g_pVideo->GetSysLine()->DrawRect(rtRect);
		g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
#endif
}


void SPWindowNpcTrade::RenderText()
{
	SPWindow::RenderText();
}


//////////////////////////////////////////////////////////////////////////
void SPWindowNpcTrade::ResetItemListControl()
{
	ZeroMemory(m_szPage, 64);
	wsprintf(m_szPage, "%d/%d", m_iPage, m_iMaxPage);
	m_pPageInfo->SetWindowText(m_szPage);

	if(m_iPage == 1) {
		m_pPageDown->SetEnable(false);
	}
	else {
		m_pPageDown->SetEnable(true);
	}

	if(m_iPage == m_iMaxPage){
		m_pPageUp->SetEnable(false);
	}
	else{
		m_pPageUp->SetEnable(true);
	}

	for(int i = 0 ; i < TRADE_PAGE ; i++ ) {
		m_pIcon[i]->Hide();
		m_pIcon[i]->SPSendMessage(SPIM_SET_IMAGECLEAR);
		m_pIcon[i]->Show();
		
		m_pName[i]->SetWindowText("");			
		m_pCash[i]->SetWindowText("");
		//m_bDisable[i] = false;
		m_bAble[i] = true;
		m_iMark[i] = ICON_MARK_NULL;
		m_iStack[i] = 0;
	}

	ResetItemButton();
}


void SPWindowNpcTrade::UpdateItemList()
{
	ResetItemListControl();

	
	m_iBuyCount = 1;			//임시
	m_iSelectItemIndex = -1;
	m_iViewItemIndex = -1;
	//UpdateBuyCount();

	//control select
	for(int i = 0 ; i < TRADE_PAGE ; i++ ){	
		m_pSkin[i]->Hide();
	}

	int iStartIndex = m_iPage -1 ;
	iStartIndex = iStartIndex * TRADE_PAGE;
	int iEndIndex = iStartIndex + TRADE_PAGE;

	
	DXUTOutputDebugString("Page[%d] iStartIndex[%d] iEndIndex[%d]\n", m_iPage, iStartIndex, iEndIndex);

	SPItemAttr* pkItemAttr = NULL;
	int iCounter = 0;
	//char szName[32];
	for(int index = iStartIndex, iCounter = 0; index < iEndIndex ; index++, iCounter++) {
		//if( g_pItemCluster->GetItemInfo(ItemInfo.Item.iItemID, pkItemAttr) ) {

		if(m_pNpcShop->m_NpcSellItem[index].m_iItemID > 0 ){
			if( g_pItemCluster->GetItemInfo(m_pNpcShop->m_NpcSellItem[index].m_iItemID, pkItemAttr) ) {

				//m_pName[iCounter]->SetFontColor(GetItemColor(m_pNpcShop->m_NpcSellItem[index].m_iRear));
				//m_pName[iCounter]->SetFontColor(g_pCheckManager->GetItemColor(m_pNpcShop->m_NpcSellItem[index].m_iRear));				
				if(g_pItemCluster->IsItemNameColor(m_pNpcShop->m_NpcSellItem[index].m_iItemID)){
					m_pName[iCounter]->SetFontColor(g_pItemCluster->GetItemNameColor(m_pNpcShop->m_NpcSellItem[index].m_iItemID));
				}
				else {
					m_pName[iCounter]->SetFontColor(g_pCheckManager->GetItemColor(m_pNpcShop->m_NpcSellItem[index].m_iRear));
				}				
				
				///[12/8/2006 jinssaga]
				//if(pkItemAttr->m_strName.length() > 24) {					
				//	ZeroMemory(szName, 32);
				//	string strSub =  pkItemAttr->m_strName.substr(0, 24);
				//	wsprintf(szName, "%s", strSub.c_str());
				//	m_pName[iCounter]->SetWindowText(szName);
				//}
				//else {
				//	m_pName[iCounter]->SetWindowText(pkItemAttr->m_strName.c_str());
				//}
				
				m_pName[iCounter]->SetWindowText(pkItemAttr->m_strName.c_str());

				int iSellFactor = m_pNpcShop->m_NpcSellItem[index].m_iSellFactor;
				int iTotalBuyPrice = GET_BUY_PRICE(pkItemAttr->m_iGamePrice, 1, iSellFactor, 100);
				iTotalBuyPrice = iTotalBuyPrice *m_pNpcShop->m_NpcSellItem[index].m_iStack;
				m_pCash[iCounter]->SetWindowText(ConvertMoneyToString(iTotalBuyPrice));

				POINT ptSrcPos;
				SPTexture* pkTexture = g_pItemCluster->GetItemTexture(m_pNpcShop->m_NpcSellItem[index].m_iItemID, ptSrcPos);
				//m_pIcon[iCounter]->SPSendMessage(SPIM_SET_IMAGECLEAR);
				m_pIcon[iCounter]->Hide();
				m_pIcon[iCounter]->SetImage(pkTexture, ptSrcPos.x, ptSrcPos.y);
				m_pIcon[iCounter]->Show();
				
				//[2005/10/10] 각 아이템별로 현 로컬플레이어가 사용할수 있는지의 여부 체크
				m_bAble[iCounter] = g_pCheckManager->CheckItemStatus(m_pNpcShop->m_NpcSellItem[index].m_iItemID);
				m_iMark[iCounter] = g_pItemCluster->GetItemIconMark(m_pNpcShop->m_NpcSellItem[index].m_iItemID);
				m_iStack[iCounter] = m_pNpcShop->m_NpcSellItem[index].m_iStack;
			}			
		}
		else {
			//m_pIcon[iCounter]->SPSendMessage(SPIM_SET_IMAGECLEAR);
			//m_pName[iCounter]->SetWindowText("");			
			//m_pCash[iCounter]->SetWindowText("");
		}
	}
}


////
//void SPWindowNpcTrade::UpdateBuyCount()
//{	
//	ZeroMemory(m_szBuyCount, 64);
//	//wsprintf(m_szBuyCount, "%d개", m_iBuyCount);
//	wsprintf(m_szBuyCount, "%d", m_iBuyCount);
//	//m_pBuyCountInfo->SetWindowText(m_szBuyCount);	
//	m_pBuyCountInfo->SetWindowFormatText(m_szBuyCount);
//
//	if(m_iSelectItemIndex < 0) {
//		m_pDownCount->SetEnable(false);
//		m_pCount->SetEnable(false);
//		m_pBuy->SetEnable(false);
//		return;
//	}
//	
//	SPItemAttr* pItemAttr = NULL;
//	int iMaxBuyCount = 1;					//아이템 스텍 제한(한 종류의 아이템이 몇개까지 중복이 되는지)
//	
//	if( g_pItemCluster->GetItemInfo(m_pNpcShop->m_NpcSellItem[m_iSelectItemIndex].m_iItemID, pItemAttr) ) {	
//		iMaxBuyCount = pItemAttr->m_iStackLimit;
//	}
//	else {
//		ResetSelectItem();
//		return;
//	}
//
//	if(pItemAttr == NULL) {				//
//		ResetSelectItem();
//		return;	
//	}
//
//	if(m_iBuyCount == 1) {
//		m_pDownCount->SetEnable(false);
//	}
//	else {
//		m_pDownCount->SetEnable(true);
//	}
//
//	if(m_iBuyCount == iMaxBuyCount || m_iBuyCount == MAX_BUY_COUNT){
//		m_pCount->SetEnable(false);
//	}
//	else{
//		m_pCount->SetEnable(true);
//	}
//
//	m_pBuy->SetEnable(true);
//
//	//m_iBuyCount 에 의한 가격 변경 사항을 출력할수 있도록 한다 
//	//int iTotalBuyPrice = m_iBuyCount * pItemAttr->m_iGamePrice;
//	int iSellFactor = m_pNpcShop->m_NpcSellItem[m_iSelectItemIndex].m_iSellFactor;
//	int iTotalBuyPrice = GET_BUY_PRICE(pItemAttr->m_iGamePrice, m_iBuyCount, iSellFactor, 100);	
//	m_pCash[m_iViewItemIndex]->SetWindowText(ConvertMoneyToString(iTotalBuyPrice));
//}	


void SPWindowNpcTrade::ResetItemButton()
{
	for(int i = 0 ; i < TRADE_PAGE ; i++ ){
		//SPWindow* pkWindow = Find(WIID_NPCTRADE_ITEM_BUTTON + i, true);
		//if( pkWindow == NULL )
		//	continue;
		//pkWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE);
		m_pSkin[i]->Hide();
	}

	m_iSelectItemIndex = -1;			//선택된 아이템 인덱스 초기화
	m_iViewItemIndex = -1;

	m_bSend = false;
}


void SPWindowNpcTrade::ResetSelectItem()
{
	//구입컨트롤 리셋화 // 함수화 시켜 예외 처리시에도 사용할수 있도록
	//m_pButton[m_iViewItemIndex]->SPSendMessage(SPIM_BTN_SELECT, FALSE);
	if(m_iViewItemIndex >= 0 && m_iViewItemIndex < TRADE_PAGE)
		m_pSkin[m_iViewItemIndex]->Hide();
	
	SPItemAttr* pItemAttr = NULL;
	if( g_pItemCluster->GetItemInfo(m_pNpcShop->m_NpcSellItem[m_iSelectItemIndex].m_iItemID, pItemAttr) ) {		
		int iSellFactor = m_pNpcShop->m_NpcSellItem[m_iSelectItemIndex].m_iSellFactor;
		int iTotalBuyPrice = GET_BUY_PRICE(pItemAttr->m_iGamePrice, 1, iSellFactor, 100);
		iTotalBuyPrice = iTotalBuyPrice *m_pNpcShop->m_NpcSellItem[m_iSelectItemIndex].m_iStack;
		m_pCash[m_iViewItemIndex]->SetWindowText(ConvertMoneyToString(iTotalBuyPrice));
	}

	m_iSelectItemIndex = -1;
	m_iViewItemIndex = -1;
	m_iBuyCount = 1;
	//UpdateBuyCount();
}


bool SPWindowNpcTrade::IsQuickItemSell()
{
	bool bSend = false;						//중복 패킷제한용... 여기 있어서는 별효과 없을듯..

	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return false;

	//사망시 제한
	if(g_pGOBManager->GetLocalPlayer()->GetCoordPhysics()->GetCurState() == GOB_STATE_DEAD) {
		return false;
	}

	//픽업 상태가 아닌경우 넘어간다
	SPPlayerInvenArchive* pInvenArchive = NULL;
	pInvenArchive = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();	

	if(pInvenArchive->m_bPickup == false) {
		DXUTOutputDebugString("No PickUp Item\n");
		return false;
	}	

	m_PickUpItem = pInvenArchive->GetPickupItem();
	
	//장비탭에서 오는 경우
	if(m_PickUpItem.GetContainerType() >= INVENTORY_HOUSING) {
		//DXUTOutputDebugString("ItemSell Equip, Costume... Inven.. \n")	;
		if(g_pGOBManager->GetLocalPlayer())
			g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);		
		return false;
	}

	//해당위치에 아이템이 없는 경우
	if(pInvenArchive->IsItemExist(m_PickUpItem.m_iSlotIndex, m_PickUpItem.m_iContainer) == false) {
		if(g_pGOBManager->GetLocalPlayer())
			g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);		
		return false;
	}
	
	SPItem* pSellSelectItem = g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetInventoryItem(m_PickUpItem.m_iContainer, m_PickUpItem.m_iSlotIndex);
	if(pSellSelectItem == NULL) {
		if(g_pGOBManager->GetLocalPlayer())
			g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);		
		return false;
	}

	SPItemStatus* pItemStatus = pSellSelectItem->GetItemStatus();
	if(pItemStatus == NULL || pItemStatus->GetItemID() == 0) {
		if(g_pGOBManager->GetLocalPlayer())
			g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);		
		return false;
	}
	
	if(m_pNpcShop == NULL) {
		if(g_pGOBManager->GetLocalPlayer())
			g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
		return false;
	}

	SPItemAttr* pItemAttr = pItemStatus->GetItemAttr();
	
	//[2007/1/22] - jinssaga   
	if(pItemAttr->m_bCashCheck) {
		std::string strMsg = "";
		if(SPLocalizeManager::GetInstance()->GetMileageRatio() == 0) {			//포인트를 사용하지 않는 경우
			if(g_pResourceManager->GetGlobalString(4003009))
				strMsg = g_pResourceManager->GetGlobalString(4003009);
		}
		else {
			if(g_pResourceManager->GetGlobalString(4003008))					//포인트를 사용
				strMsg = g_pResourceManager->GetGlobalString(4003008);
		}		
		
		if(g_pGOBManager->GetLocalPlayer())
			g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
		
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		return false;
	}
	
	//판매 불가능한 속성의 아이템의 경우 리턴	
	if(pItemAttr == NULL || pItemAttr->m_bSell == false) {
		if(g_pGOBManager->GetLocalPlayer())
			g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
		return false ;
	}

	CONTAINER_ITEM item;
	if(m_PickUpItem.m_iContainer < INVENTORY_EQUIP || m_PickUpItem.m_iContainer > INVENTORY_HOUSING)
		return false;
	if(m_PickUpItem.m_iSlotIndex < 0 || m_PickUpItem.m_iSlotIndex >= pInvenArchive->GetInventorySize(m_PickUpItem.m_iContainer))
		return false;

	item.SlotInfo.ContainerType = m_PickUpItem.m_iContainer;
	item.SlotInfo.SlotIndex = m_PickUpItem.m_iSlotIndex + 1;
	item.Item.iItemID = pItemStatus->GetItemID();
	item.Item.iStackCount = pItemStatus->SPSendMessage(MV_ITEM_GETSTACK_CNT);
	item.Item.iBackPrice = pItemStatus->SPSendMessage(MV_ITEM_GET_BACKPRICE);
	item.Item.iItemNo = *(ITEMNO*)pItemStatus->SPSendMessage(MV_ITEM_GET_ITEMNO);

	if(g_pCheckManager->IsDBBlock()) {
		
		DXUTOutputDebugString("\tSPWindowItemSell::OnSell DB Working Block\n");
		return false;
	}

	//팻킷 만들기
	CPacket Packet(NPC_SHOP_CS_SELL);
	Packet.Add((UINT32)m_pNpcShop->m_iNpcID);
	Packet.Add((UINT32)m_pNpcShop->m_iShopID);
	Packet.AddData(&item, sizeof(CONTAINER_ITEM));

	if(!bSend) {		
		g_pNetworkManager->SPPerformMessage(NPC_SHOP_CS_SELL, 0, (LPARAM)&Packet);
		bSend = true;
		//Cash Sound
		g_pInterfaceManager->RegisterSoundUnit("DATA/SOUND/SYSTEM_CASH.WAV");
	}
	
	return true;
}


//////////////////////////////////////////////////////////////////////////
SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowNpcTrade )

SPIMESSAGE_COMMAND( SPIM_SET_NPCSHOP_SHOW,		OnUpdateShopData )

SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_NPCTRADE_EXIT		, OnClose)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_NPCTRADE_PAGEDOWN	, OnPageDown)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_NPCTRADE_PAGEUP	, OnPageUp)

//SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_NPCTRADE_COUNT		, OnCount)		//상황에 따라 LBUTTON_DOWN으로 바뀌어야 할지도..
//SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_NPCTRADE_DOWNCOUNT	, OnCountDown)
//SPIMESSAGE_CONTROL( SPIM_REFRESH,		WIID_NPCTRADE_COUNTER	, OnUpdateCount)
//SPIMESSAGE_CONTROL( SPIM_SETFOCUS,	WIID_NPCTRADE_COUNTER	, OnInputFocus)
//SPIMESSAGE_COMMAND( SPIM_WHEEL_UP,								OnCount)
//SPIMESSAGE_COMMAND( SPIM_WHEEL_DOWN,							OnCountDown)
//SPIMESSAGE_CONTROL( SPIM_WHEEL_UP,	WIID_NPCTRADE_COUNTER,	OnCount)
//SPIMESSAGE_CONTROL( SPIM_WHEEL_DOWN,	WIID_NPCTRADE_COUNTER,	OnCountDown)

SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_NPCTRADE_BUY		, OnBuyItem)	//실제 구매

SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP, WIID_NPCTRADE_ITEM_BUTTON,		WIID_NPCTRADE_ITEM_BUTTON + TRADE_PAGE,	OnLBuyItemClick)
SPIMESSAGE_CONTROL_RANGE( SPIM_RBUTTON_UP, WIID_NPCTRADE_ITEM_BUTTON,		WIID_NPCTRADE_ITEM_BUTTON + TRADE_PAGE,	OnRBuyItemClick)
SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_ENTER,	WIID_NPCTRADE_ITEM_BUTTON,		WIID_NPCTRADE_ITEM_BUTTON + TRADE_PAGE,	OnToolTipEnter)
SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_OUT,		WIID_NPCTRADE_ITEM_BUTTON,		WIID_NPCTRADE_ITEM_BUTTON + TRADE_PAGE,	OnToolTipOut)
SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_MOVE,	WIID_NPCTRADE_ITEM_BUTTON,		WIID_NPCTRADE_ITEM_BUTTON + TRADE_PAGE,	OnToolTipMove)

SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_NPCTRADE_TOP		, OnLButtonUp)		//아이템아이콘 리커버
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_NPCTRADE_CENTER	, OnLButtonUp)		//아이템아이콘 리커버
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_NPCTRADE_BOTTOM	, OnLButtonUp)		//아이템아이콘 리커버
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_NPCTRADE_TITLE		, OnLButtonUp)		//아이템아이콘 리커버
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_NPCTRADE_MIDDLE	, OnLButtonUp)		//아이템아이콘 리커버
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_NPCTRADE_LIST_IMG_BASE , OnLButtonUp)		//아이템아이콘 리커버
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_NPCTRADE_LIST_IMG	, OnLButtonUp)		//아이템아이콘 리커버
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_NPCTRADE_PRICE_IMG_BASE, OnLButtonUp)		//아이템아이콘 리커버
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_NPCTRADE_PRICE_IMG		, OnLButtonUp)		//아이템아이콘 리커버
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_NPCTRADE_PAGEUP	, OnLButtonUp)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_NPCTRADE_PAGE		, OnLButtonUp)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_NPCTRADE_PAGEDOWN	, OnLButtonUp)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_USER_ELY_TEXT_IMG	, OnLButtonUp)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_USER_ELY			, OnLButtonUp)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_USER_ELY_IMG		, OnLButtonUp)

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)



int SPWindowNpcTrade::OnUpdateShopData( WPARAM wParam, LPARAM lParam )
{
	//상점데이터를 가져온다
	m_pNpcShop = g_pEventManager->GetEventHandler()->GetNpcShopData();
	
	m_iPage = 1;
	m_iMaxPage = m_pNpcShop->m_iPageCount;
	
	//m_iBuyCount = 1;			//임시
	
	//컨트롤 관련 업데이트를 한다 	
	UpdateItemList();

	return 1;
}


int SPWindowNpcTrade::OnClose( WPARAM wParam, LPARAM lParam )
{
	g_pEventManager->SetCutInState(CUT_IN_STATE_RELEASE);

	
	DXUTOutputDebugString("SPWindowNpcTrade::OnClose::CUT_IN_STATE_NULL\n");

	//g_pInterfaceManager->GetItemSellWindow()->SPSendMessage(SPIM_SET_ITEMSELL_RESET);
	
	return 1;
}


int SPWindowNpcTrade::OnPageDown( WPARAM wParam, LPARAM lParam )
{
	if(g_pGOBManager->GetLocalPlayer())
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
	
	if( m_iPage > 1  && m_iPage <= m_iMaxPage ) {
		m_iPage--;

		UpdateItemList();
	}

	return 1;
}


int SPWindowNpcTrade::OnPageUp( WPARAM wParam, LPARAM lParam )
{
	if(g_pGOBManager->GetLocalPlayer())
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
	
	if( m_iPage > 0 && m_iPage < m_iMaxPage ) {
		m_iPage++;
		
		UpdateItemList();
	}
	
	return 1;
}


int SPWindowNpcTrade::OnCount( WPARAM wParam, LPARAM lParam )
{
	//현재 선택된 아이템의 유효성을 먼저 체크 
	if(m_iSelectItemIndex < 0 || m_iSelectItemIndex > m_pNpcShop->m_iItemSize)
		return 1;
	
	SPItemAttr* pItemAttr = NULL;
	if( g_pItemCluster->GetItemInfo(m_pNpcShop->m_NpcSellItem[m_iSelectItemIndex].m_iItemID, pItemAttr) ) {
		if(0 < m_iBuyCount && m_iBuyCount < pItemAttr->m_iStackLimit) {
			m_iBuyCount++;

			//UpdateBuyCount();
		}
	}

	return 1;
}


int SPWindowNpcTrade::OnCountDown( WPARAM wParam, LPARAM lParam )
{
	//현재 선택된 아이템의 유효성을 먼저 체크 	
	if(m_iSelectItemIndex < 0 || m_iSelectItemIndex > m_pNpcShop->m_iItemSize)
		return 1;
	
	SPItemAttr* pItemAttr = NULL;
	if( g_pItemCluster->GetItemInfo(m_pNpcShop->m_NpcSellItem[m_iSelectItemIndex].m_iItemID, pItemAttr) ) {
		//if(1 < m_iBuyCount && m_iBuyCount <= MAX_BUY_COUNT ) {
		if(1 < m_iBuyCount && m_iBuyCount <= pItemAttr->m_iStackLimit ) {
			m_iBuyCount--;

			//UpdateBuyCount();
		}
	}

	return 1;
}


int SPWindowNpcTrade::OnUpdateCount(WPARAM wParam, LPARAM lParam)
{
	//현재 선택된 아이템의 유효성을 먼저 체크 	
	if(m_iSelectItemIndex < 0 || m_iSelectItemIndex > m_pNpcShop->m_iItemSize) {
		m_iBuyCount = 1;
		//m_pBuyCountInfo->KillFocus();
		return 1;
	}
	
	//int iTradeCount = atoi(m_pBuyCountInfo->GetWindowText());
	int iTradeCount = 0;

	SPItemAttr* pItemAttr = NULL;
	if( g_pItemCluster->GetItemInfo(m_pNpcShop->m_NpcSellItem[m_iSelectItemIndex].m_iItemID, pItemAttr) ) {		
		if(iTradeCount > 0 && iTradeCount <= pItemAttr->m_iStackLimit) {
			//카운터 업데이트
			m_iBuyCount = iTradeCount;
			
		}
		else {			
			if(iTradeCount < 1) {
				m_iBuyCount = 0;
			}
			else if(iTradeCount > pItemAttr->m_iStackLimit) {
				m_iBuyCount = pItemAttr->m_iStackLimit;
			}		
		}
		//UpdateBuyCount();
	}
	
	return 1;
}


int SPWindowNpcTrade::OnInputFocus(WPARAM wParam, LPARAM lParam)
{
	//현재 선택된 아이템의 유효성을 먼저 체크 	
	if(m_iSelectItemIndex < 0 || m_iSelectItemIndex > m_pNpcShop->m_iItemSize) {
		//m_pBuyCountInfo->KillFocus();
		return 1;
	}

	//아이템정보가 없거나 스택이 1이하인 경우
	SPItemAttr* pItemAttr;
	g_pItemCluster->GetItemInfo(m_pNpcShop->m_NpcSellItem[m_iSelectItemIndex].m_iItemID, pItemAttr);
	if( pItemAttr ) {
		if(pItemAttr->m_iStackLimit < 2) {
			//m_pBuyCountInfo->KillFocus();					//스택이 1이상인 아이템만 포커스를 준다
			return 1;
		}
	}
	
	return 1;
}


int SPWindowNpcTrade::OnLBuyItemClick( unsigned int uiID, WPARAM wParam, LPARAM lParam )	//구매할 아이템 클릭 (컨트롤 클릭)
{
	//[2005/12/19] - 퀵 판매용 만일 현재 아이템을 들고 있는 상황이라면... 처리
	if(IsQuickItemSell())
		return 1;
	
	//기존의 선택되어 있는 부분을 리셋한다 
	ResetSelectItem();						//[2005/10/10]
	
	int iSelect = uiID - WIID_NPCTRADE_ITEM_BUTTON;	
	assert(iSelect < TRADE_PAGE && "NPC Shop Item List Constrol의 인터페이스 인덱스 에러");	
	
	//이전에 원래 있던 Counter정보를 리셋해준다 
	ResetItemButton();	

	//컨트롤 Select전에 실제 판매 아이템 리스트와 비교하여 사이즈보다 큰경우 그냥 반환한다 
	
	m_iSelectItemIndex = iSelect + ((m_iPage - 1) * TRADE_PAGE);	
	m_iViewItemIndex = iSelect;
	if(m_iSelectItemIndex >= m_pNpcShop->m_iItemSize) {
		DXUTOutputDebugString("FAIL : m_iSelectItemIndex [%d] - ItemSize[%d] \n", m_iSelectItemIndex, m_pNpcShop->m_iItemSize);
		
		m_iSelectItemIndex = -1;
		m_iViewItemIndex = -1;
		
		return 1;
	}
	DXUTOutputDebugString("OK : m_iSelectItemIndex [%d] ViewIndex[%d]- ItemSize[%d]\n", m_iSelectItemIndex, m_iViewItemIndex, m_pNpcShop->m_iItemSize);	

	//control select
	for(int i = 0 ; i < TRADE_PAGE ; i++ ){
		if(i == m_iViewItemIndex) {
			m_pSkin[i]->Show();						
		}
		else {
			m_pSkin[i]->Hide();
		}

	}	
	
	//아이템 카운터를 업데이트 한다 	
	m_iBuyCount = 1;
	//이거 이후에 컨트롤 가격정보도 수정해주어야 한다 (List 전체가 수정되어야 할듯...)	
	
	//UpdateBuyCount();

	SPItemAttr* pItemAttr;
	g_pItemCluster->GetItemInfo(m_pNpcShop->m_NpcSellItem[m_iSelectItemIndex].m_iItemID, pItemAttr);
	if( pItemAttr ) {
		if(pItemAttr->m_iStackLimit > 1) {
			//m_pBuyCountInfo->SetFocus();					//스택이 1이상인 아이템만 포커스를 준다
		}
	}	
	
	return 1;
}


int SPWindowNpcTrade::OnRBuyItemClick( unsigned int uiID, WPARAM wParam, LPARAM lParam )
{
	//[2005/12/19] - 퀵 판매용 만일 현재 아이템을 들고 있는 상황이라면... 처리
	if(IsQuickItemSell())
		return 1;

	//기존의 선택되어 있는 부분을 리셋한다 
	ResetSelectItem();						//[2005/10/10]

	int iSelect = uiID - WIID_NPCTRADE_ITEM_BUTTON;	
	assert(iSelect < TRADE_PAGE && "NPC Shop Item List Constrol의 인터페이스 인덱스 에러");	

	//이전에 원래 있던 Counter정보를 리셋해준다 
	ResetItemButton();	

	//컨트롤 Select전에 실제 판매 아이템 리스트와 비교하여 사이즈보다 큰경우 그냥 반환한다 
	
	m_iSelectItemIndex = iSelect + ((m_iPage - 1) * TRADE_PAGE);	
	m_iViewItemIndex = iSelect;
	if(m_iSelectItemIndex >= m_pNpcShop->m_iItemSize) {
		DXUTOutputDebugString("FAIL : m_iSelectItemIndex [%d] - ItemSize[%d] \n", m_iSelectItemIndex, m_pNpcShop->m_iItemSize);

		m_iSelectItemIndex = -1;
		m_iViewItemIndex = -1;

		return 1;
	}
	DXUTOutputDebugString("OK : m_iSelectItemIndex [%d] ViewIndex[%d]- ItemSize[%d]\n", m_iSelectItemIndex, m_iViewItemIndex, m_pNpcShop->m_iItemSize);	

	//control select
	for(int i = 0 ; i < TRADE_PAGE ; i++ ){
		if(i == m_iViewItemIndex) {
			m_pSkin[i]->Show();						
		}
		else {
			m_pSkin[i]->Hide();
		}

	}

	OnBuyItem((WPARAM)0, (LPARAM)0);
	
	return 1;
}


int SPWindowNpcTrade::OnBuyItem( WPARAM wParam, LPARAM lParam )
{
	//////////////////////////////////////////////////////////////////////////실제 아이템 구매 처리
	if(m_pNpcShop == NULL)		return 1;			//예외처리
	if(m_iSelectItemIndex < 0)	return 1;

	SPItemAttr* pItemAttr = NULL;
	
	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return 1;

	//현재 Local Player가 물건 구매를 할수 있는 상황인지 확인한다
	if(g_pGOBManager->GetLocalPlayer()->GetCoordPhysics()->GetCurState() == GOB_STATE_DEAD) {
		ResetSelectItem();
		return 1;
	}
	
	//실제 구매 작업	
	if( g_pItemCluster->GetItemInfo(m_pNpcShop->m_NpcSellItem[m_iSelectItemIndex].m_iItemID, pItemAttr) ) {					
		CONTAINER_TYPE iContainerType = (CONTAINER_TYPE)(pItemAttr->m_eType - 1);
		int iItemID = m_pNpcShop->m_NpcSellItem[m_iSelectItemIndex].m_iItemID;
		int iStack = m_pNpcShop->m_NpcSellItem[m_iSelectItemIndex].m_iStack;

		//인벤토리가 Full인지 부터 확인한다
		if(g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->IsFullInventory(iContainerType)) {
			if(false == g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->IsEnableSpace(iContainerType, iItemID, iStack))
			{	
				//만일 스택카운트가 다 차지 않은 아이템을 구입하는 경우 빠져 나가도록 한다		
				ResetSelectItem();

				if(g_pResourceManager->GetGlobalString(4003102)) {
					std::string strMsg = g_pResourceManager->GetGlobalString(4003102);	//인벤토리 공간이 부족하여 아이템을 구매할 수 없습니다.
					g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
				}

				return 1;
			}
		}		
		
		if(false == g_pCheckManager->CheckEnableSlot(m_pNpcShop->m_NpcSellItem[m_iSelectItemIndex].m_iItemID, m_pNpcShop->m_NpcSellItem[m_iSelectItemIndex].m_iStack)) {
			ResetSelectItem();
			return 1;
		}
		
		//플레이어의 돈이 구입할 금액보다 부족한 경우 처리
		int iUserEly = g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValueEX(STATUS_EX_ELY);
		//int iTotalBuyPrice = m_iBuyCount * pItemAttr->m_iGamePrice;
		int iSellFactor = m_pNpcShop->m_NpcSellItem[m_iSelectItemIndex].m_iSellFactor;
		int iTotalBuyPrice = GET_BUY_PRICE(pItemAttr->m_iGamePrice, m_iBuyCount, iSellFactor, 100);
		iTotalBuyPrice = iTotalBuyPrice * m_pNpcShop->m_NpcSellItem[m_iSelectItemIndex].m_iStack;
		
		if(iTotalBuyPrice > iUserEly) {
			ResetSelectItem();

			if(g_pResourceManager->GetGlobalString(4003101)) {
				std::string strMsg = g_pResourceManager->GetGlobalString(4003101);	//Ely가 부족하여 아이템을 구매할 수 없습니다.
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
			}

			return 1;
		}

		//iMaxBuyCount = pItemAttr->m_iStackLimit;

		if(g_pCheckManager->IsDBBlock()) {
			
			DXUTOutputDebugString("\tSPWindowNpcTrade::OnBuyItem DB Working Block\n");
			return 1;
		}

		CPacket Packet(NPC_SHOP_CS_BUY);
		Packet.Add((UINT32)m_pNpcShop->m_iNpcID);
		Packet.Add((UINT32)m_pNpcShop->m_iShopID);
		Packet.Add((UINT8)m_iSelectItemIndex);
		
		ITEM item = {0,};		
		item.iItemID = m_pNpcShop->m_NpcSellItem[m_iSelectItemIndex].m_iItemID;
		item.iStackCount = m_pNpcShop->m_NpcSellItem[m_iSelectItemIndex].m_iStack;		
		Packet.AddData(&item, sizeof(ITEM));

		if(!m_bSend) {
			//이부분 리턴값 문제 있을수 있음...
			//return g_pNetworkManager->SPPerformMessage(NPC_SHOP_CS_SELL, 0, (LPARAM)&Packet);	
			g_pNetworkManager->SPPerformMessage(NPC_SHOP_CS_BUY, 0, (LPARAM)&Packet);	

			m_bSend = true;				

			//구입컨트롤 리셋화
			ResetSelectItem();

			//팔기위해 등록한 아이템을 리셋하도록 한다
			//g_pInterfaceManager->GetItemSellWindow()->SPSendMessage(SPIM_SET_ITEMSELL_RESET);

			//m_pBuy->SetEnable(false);

			g_pCheckManager->SetDBBlock(true);

			//Cash Sound
			g_pInterfaceManager->RegisterSoundUnit("DATA/SOUND/SYSTEM_CASH.WAV");
		}
	}

	return 1;
}


int SPWindowNpcTrade::OnToolTipMove(unsigned int uiID, WPARAM wParam, LPARAM lParam)
{
	int iCurIndex = uiID - WIID_NPCTRADE_ITEM_BUTTON;	
	iCurIndex = iCurIndex + ((m_iPage - 1) * TRADE_PAGE);	

	unsigned int uiUniqueID;
	int iEquipClass, iItemID, iItemColor;

	if(m_pNpcShop == NULL || m_pNpcShop->m_NpcSellItem[iCurIndex].m_iItemID == 0)	return 1;

	//uiUniqueID = m_ContainerItem[iCurIndex].Item.iItemID;
	uiUniqueID = m_pNpcShop->m_NpcSellItem[iCurIndex].m_iItemID;	
	iItemColor = (m_pNpcShop->m_NpcSellItem[iCurIndex].m_iItemID) % 100;
	iEquipClass = (m_pNpcShop->m_NpcSellItem[iCurIndex].m_iItemID) / 100000;
	iItemID = ((m_pNpcShop->m_NpcSellItem[iCurIndex].m_iItemID) % 100000 - iItemColor) / 100;	

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
		kItemStatus.SPSendMessage(MV_ITEM_SET_RARELEVEL, m_pNpcShop->m_NpcSellItem[iCurIndex].m_iRear);

		if(pItemAttr->m_eDuration != ITEM_DURATION_NULL) {
			kItemStatus.SetLifeTime(pItemAttr->m_iLifeTime * 3600);
		}

		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage(SPIM_TOOLTIP_MOVE, (WPARAM)&kItemStatus, lParam);
		pWindow->Show();
	}

	return 1;
}


int SPWindowNpcTrade::OnToolTipEnter(unsigned int uiID, WPARAM wParam, LPARAM lParam)
{	
	int iCurIndex = uiID - WIID_NPCTRADE_ITEM_BUTTON;	
	iCurIndex = iCurIndex + ((m_iPage - 1) * TRADE_PAGE);

	unsigned int uiUniqueID;
	int iEquipClass, iItemID, iItemColor;
	
	if(m_pNpcShop == NULL || m_pNpcShop->m_NpcSellItem[iCurIndex].m_iItemID == 0)	return 1;	
	
	uiUniqueID = m_pNpcShop->m_NpcSellItem[iCurIndex].m_iItemID;	
	iItemColor = (m_pNpcShop->m_NpcSellItem[iCurIndex].m_iItemID) % 100;
	iEquipClass = (m_pNpcShop->m_NpcSellItem[iCurIndex].m_iItemID) / 100000;
	iItemID = ((m_pNpcShop->m_NpcSellItem[iCurIndex].m_iItemID) % 100000 - iItemColor) / 100;	

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
		kItemStatus.SPSendMessage(MV_ITEM_SET_RARELEVEL, m_pNpcShop->m_NpcSellItem[iCurIndex].m_iRear);

		if(pItemAttr->m_eDuration != ITEM_DURATION_NULL) {
			kItemStatus.SetLifeTime(pItemAttr->m_iLifeTime * 3600);
		}

		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage(SPIM_TOOLTIP_ENTER, (WPARAM)&kItemStatus, lParam);
		pWindow->Show();
	}

	return 1;
}


int SPWindowNpcTrade::OnToolTipOut(unsigned int uiID, WPARAM wParam, LPARAM lParam)
{
	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
	pWindow->Show();

	return 1;
}


int SPWindowNpcTrade::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{
	if(g_pGOBManager->GetLocalPlayer())
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
	return 1;
}


