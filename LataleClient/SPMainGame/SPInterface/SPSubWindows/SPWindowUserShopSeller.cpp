
#include "SPCommon.h"
#include "SPUtil.h"

#include "SPGameObject.h"
#include "SPGOBManager.h"

//
#include "SPRenderModelDEF.h"
#include "SPRenderModel.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGOBModel.h"
#include "SPGOBModelUnit.h"
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
#include "SPItemCluster.h"
#include "SPSkill.h"
#include "SPSkillArchive.h"
#include "SPMotionStatus.h"
#include "SPSkillManager.h"
#include "SPPlayer.h"
#include "SPCoolTimeManager.h"
#include "SPCommandConvert.h"
#include "SPComboManager.h"

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
//#include "SPWindowEditMultiLine.h"
#include "SPWindowSlider.h"
#include "SPWindowUserShopSeller.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPItem.h"
#include "SPItemCluster.h"

#include "SPStage.h"
#include "SPStageManager.h"

#include "SPEventDEF.h"
//#include "SPGameObjectDEF.h"
//#include "SPGameObject.h"
//#include "SPGOBManager.h"
//#include "SPEvent.h"
//#include "SPEventArchive.h"

//#include "SPMailAttr.h"
//#include "SPMailArchive.h"
//#include "SPMailManager.h"
#include "SPUserShopAttr.h"
#include "SPUserShopArchive.h"
#include "SPEventManager.h"

#include "SPUserShopArchive.h"

#include "SPAbuseFilter.h"
#include "SPCheckManager.h"

#include "SPDebug.h"


SPWindowUserShopSeller::SPWindowUserShopSeller(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_USERSHOP_SELL, InstanceID, iX, iY, iCX, iCY, pParent)
, m_pTexture(NULL)
, m_pName(NULL)
{	
	Init();
}


SPWindowUserShopSeller::~SPWindowUserShopSeller()
{	
	Clean();
}


void SPWindowUserShopSeller::Init()
{
	//std::string strTextureFile = "";
	//strTextureFile = "DATA/INTERFACE/CONCEPT/UI101.PNG";
	//g_pVideo->GetTextureMgr()->LoadTexture(strTextureFile.c_str(), &m_pTexture);

	InitSubControl();

	ClearEly();
	ClearSellItem();
	
	m_iUserShopState = USERSHOP_STATE_NULL;

	m_fStartPosX = 0.0f;
	m_fStartPosY = 0.0f;

	m_iCurPage = 0;	
	//m_iMaxPage = FM_MAX_SELLITEM / MAX_USER_SHOP_LIST;
	m_iSellCount = FM_BASE_SELLITEM;
	m_iMaxPage = m_iSellCount / MAX_USER_SHOP_LIST;
	m_iSelectItem = -1;
	m_iSelectIndex = -1;
	m_iUserShopType = -1;

	std::string strTextureFile = "";
	strTextureFile.clear();
	strTextureFile = "DATA/INTERFACE/CONCEPT/UI100.PNG";
	g_pVideo->GetTextureMgr()->LoadTexture(strTextureFile.c_str(), &m_pDisable);

	SetRect(&m_rtSrcDisable, 434 , 37 , 436 , 39);
	SetRect(&m_rtMarkSrc[0], 479, 152, 511, 184);		//ICON_MARK_LOCK
	SetRect(&m_rtMarkSrc[1], 479, 185, 511, 217);		//ICON_MARK_FASHION
	SetRect(&m_rtMarkSrc[2], 479, 339, 511, 371);		//ICON_MARK_EVENT
	SetRect(&m_rtMarkSrc[3], 479, 372, 511, 404);		//ICON_MARK_FANCY
}


void SPWindowUserShopSeller::Clean()
{
	for(int i = 0; i < MAX_USER_SHOP_LIST; i++) {
		m_pItemSkin[i]->SPSendMessage(SPIM_SET_IMAGECLEAR);
		m_pItemIcon[i]->SPSendMessage(SPIM_SET_IMAGECLEAR);
	}	
	
	SAFE_RELEASE(m_pDisable);
	SPWindow::Clean();
}


void SPWindowUserShopSeller::Process(float fTime)
{	
	if( !m_bShow )
		return;
	
	//트레이드가 가능한 상황인지를 검사한다.
	if(IsTradeEnable() == false) {
		if(IsEmptyShop() == false)				//아직 물건이 남아 있는 경우에만
			SendUserShopClose();
	}
	
	SPWindow::Process(fTime);
}


void SPWindowUserShopSeller::Render(float fTime)
{
	if(m_bShow == false)
		return;
	
	SPWindow::Render(fTime);

	for(int i = 0; i < MAX_USER_SHOP_LIST; i++) {
		//Icon Disable 표현
		if(m_pDisable) {
			RECT rtDest;
			m_pItemIcon[i]->GetWindowRect(rtDest);
			if(m_bItemAble[i] == false) {		
				m_pDisable->RenderTexture(&rtDest, &m_rtSrcDisable);
			}
			if(m_iItemMark[i] > ICON_MARK_NULL) {
				m_pDisable->RenderTexture(&rtDest, &m_rtMarkSrc[m_iItemMark[i] - 1]);
			}			
		}

		//Stack Count Local 표현
		if(m_iStack[i] > 9) {
			RECT rtTenNumTarget, rtNumTarget, rtTenNumSrc, rtNumSrc;
			int iTenStack = m_iStack[i] / 10;
			int iOneStack = m_iStack[i] % 10;
			m_pItemIcon[i]->GetWindowRect(m_rtStack[i]);
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
			m_pItemIcon[i]->GetWindowRect(m_rtStack[i]);
			SetRect(&rtNumTarget, m_rtStack[i].left, m_rtStack[i].bottom - 10, m_rtStack[i].left + 9, m_rtStack[i].bottom);

			rtNumSrc.left = iOneStack * 9;
			rtNumSrc.right = rtNumSrc.left + 9;
			rtNumSrc.top = 0;
			rtNumSrc.bottom = 10;

			g_pItemCluster->GetNumTexture()->RenderTexture( &rtNumTarget , &rtNumSrc);
		}
	}
}


void SPWindowUserShopSeller::InitSubControl()
{
	SPWindow* pStatic = NULL;
	SPWindowButton* pButton = NULL;	
	SPWindowEdit* pEdit = NULL;
	SPWindow* pParent = NULL;
	SPWindow* pTop = NULL;
	SPWindow* pCenter = NULL;
	SPWindow* pMiddle = NULL;
	SPWindow* pBottom = NULL;

	//////////////////Top
	pTop = new SPWindowStatic(WIID_USERSHOP_SELL_TOP, 0, 0, 312, 4, this);
	pTop->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 1);
	pTop->Show();

	/////////////////Center
	pCenter = new SPWindowStatic(WIID_USERSHOP_SELL_CENTER, 0, 4, 312, 270, this);
	pCenter->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 7);
	pCenter->SetSrcSize(312, 2);
	pCenter->Show();

	//Title
	pStatic = new SPWindowStatic(WIID_USERSHOP_SELL_TITLE, 10, 6, 96, 15, pCenter);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 204, 313);
	pStatic->Show();
	pStatic = NULL;

	//Close
	m_pClose = new SPWindowButton(WIID_USERSHOP_SELL_CLOSE, 291, 10, 12, 12, pCenter);
	m_pClose->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG",			301,	321);
	m_pClose->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG",		314,	321);	//on
	m_pClose->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG",		327,	321);	//push
	m_pClose->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG",	340,	321); //disable
	//m_pPrev->SetImageCheck("DATA/INTERFACE/CONCEPT/UI010.PNG", 52, 228);	//check

	/////////////////Middle
	pMiddle = new SPWindowStatic(WIID_USERSHOP_SELL, 2, 26, 308, 221, pCenter);
	pMiddle->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 493, 486);
	pMiddle->SetSrcSize(499-493, 492-486);
	pMiddle->Show();

	//ShopName
	//pStatic = new SPWindowStatic(WIID_USERSHOP_SELL_NAME, 53, 34 - 26, 63, 11, pMiddle);
	pStatic = new SPWindowStatic(WIID_USERSHOP_SELL_NAME, 51, 8, 63, 11, pMiddle);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 275, 347);	
	pStatic->Show();
	pStatic = NULL;

	//ShopName Edit
	m_pName = new SPWindowEdit(WIID_USERSHOP_SELL_NAME_EDIT, 126, 31 - 26, 151, 17, pMiddle);
	m_pName->SetWindowType(WND_TYPE_LEFTBASE);
	m_pName->SetMargin( 2, 1);
	m_pName->SetImageNormalHead	("DATA/INTERFACE/CONCEPT/UI103.PNG", 501, 469, 503, 486);
	m_pName->SetImageNormalBody	("DATA/INTERFACE/CONCEPT/UI103.PNG", 505, 469, 507, 486);
	m_pName->SetImageNormalTail	("DATA/INTERFACE/CONCEPT/UI103.PNG", 509, 469, 511, 486);
	m_pName->SetImageFocusHead	("DATA/INTERFACE/CONCEPT/UI103.PNG", 501, 451, 503, 468);
	m_pName->SetImageFocusBody	("DATA/INTERFACE/CONCEPT/UI103.PNG", 505, 451, 507, 468);
	m_pName->SetImageFocusTail	("DATA/INTERFACE/CONCEPT/UI103.PNG", 509, 451, 511, 468);
	m_pName->SetLimitText(FM_MAX_NAME);	
	m_pName->Show();

	int iListY[MAX_USER_SHOP_LIST] = { 27, 68, 109, 150 };

	//
	for(int i = 0; i < MAX_USER_SHOP_LIST; i++) {
		//Item 존재시 추가 스킨 
		//m_pItemSkin[i] = new SPWindowStatic(WIID_USERSHOP_SELL_ITEM_ENABLE + i, 0, iListY[i], 308, 41, pMiddle);
		//m_pItemSkin[i]->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 341, 2);
		//m_pItemSkin[i]->SetSrcSize(2, 2);
		//m_pItemSkin[i]->Hide();
		
		//Skin
		pParent = new SPWindowStatic(WIID_USERSHOP_SELL_ITEM_SKIN + i, 0, iListY[i], 308, 41, pMiddle);
		pParent->Show();		
		m_pItemSkin[i] = pParent;

		//Ely
		m_pItemEly[i] = new SPWindowEdit(WIID_USERSHOP_SELL_ITEM_ELY + i, 191, 10, 76, 17, pParent);
		m_pItemEly[i]->SetWindowType(WND_TYPE_LEFTBASE);
		m_pItemEly[i]->SetMargin( 2, 1);
		m_pItemEly[i]->SetImageNormalHead	("DATA/INTERFACE/CONCEPT/UI103.PNG", 501, 469, 503, 486);
		m_pItemEly[i]->SetImageNormalBody	("DATA/INTERFACE/CONCEPT/UI103.PNG", 505, 469, 507, 486);
		m_pItemEly[i]->SetImageNormalTail	("DATA/INTERFACE/CONCEPT/UI103.PNG", 509, 469, 511, 486);
		m_pItemEly[i]->SetImageFocusHead	("DATA/INTERFACE/CONCEPT/UI103.PNG", 501, 451, 503, 468);
		m_pItemEly[i]->SetImageFocusBody	("DATA/INTERFACE/CONCEPT/UI103.PNG", 505, 451, 507, 468);
		m_pItemEly[i]->SetImageFocusTail	("DATA/INTERFACE/CONCEPT/UI103.PNG", 509, 451, 511, 468);
		m_pItemEly[i]->SetFontColor(RGBA(143, 99, 99, 255));
		m_pItemEly[i]->SetNumber();
		m_pItemEly[i]->SetMaxValue(999999999);
		m_pItemEly[i]->SetFormat(DT_RIGHT | DT_VCENTER);
		m_pItemEly[i]->Show();

		//Button
		m_pItem[i] = new SPWindowButton(WIID_USERSHOP_SELL_ITEM_BUTTON + i, 0, 0, 308, 41, pParent);		
		m_pItem[i]->Show();
		
		//Line
		pStatic = new SPWindowStatic(WIID_USERSHOP_SELL_ITEM_LINE + i, 46, 40, 251, 1, pParent);
		pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 213);

		//Item Icon Line
		m_pItemIconLine[i] = new SPWindowStatic(WIID_USERShOP_SELL_ITEM_ICONLINE + i, 10, 4, 32, 32, pParent);
		m_pItemIconLine[i]->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 203, 408);
		m_pItemIconLine[i]->Show();

		//Item Icon
		m_pItemIcon[i] = new SPWindowStatic(WIID_USERSHOP_SELL_ITEM_ICON + i, 10, 4, 32, 32, pParent);
		m_pItemIcon[i]->Show();

		//Name
		m_pItemName[i] = new SPWindowStatic(WIID_USERSHOP_SELL_ITEM_NAME + i, 51, 13, 147, 13, pParent);
		//m_pItemName[i]->SetWindowText("Test...");
		m_pItemName[i]->SetFormat(DT_LEFT | DT_VCENTER);
		m_pItemName[i]->SetFontColor(RGBA(143, 99, 99, 255));
		m_pItemName[i]->Show();

		//Name Empty
		m_pItemEmpty[i] = new SPWindowStatic(WIID_USERSHOP_SELL_ITEM_EMPTY + i, 104, 15, 36, 7, pParent);
		m_pItemEmpty[i]->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 336, 382);
		m_pItemEmpty[i]->Show();

		//Ely Icon
		m_pItemElyIcon[i] = new SPWindowStatic(WIID_USERSHOP_SELL_ITEM_ELY_ICON, 279, 19, 17, 5, pParent);
		m_pItemElyIcon[i]->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 494, 506);
		m_pItemElyIcon[i]->Show();

		//m_bItemEnable[i] = false;
		m_bItemAble[i] = true;
		m_iItemMark[i] = ICON_MARK_NULL;
		m_iStack[i] = 0;		
		m_pItemIcon[i]->GetWindowRect(m_rtStack[i]);	
	}

	//m_bItemEnable[1] = true;		//Test
	//m_bItemEnable[3] = true;

	//Prev
	m_pPrev = new SPWindowButton(WIID_USERSHOP_SELL_PREV, 122, 201, 7, 11, pMiddle);	
	m_pPrev->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG",			238,	378);
	m_pPrev->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG",		246,	378);	//on
	m_pPrev->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG",		254,	378);	//push
	m_pPrev->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG",	262,	378); //disable
	//m_pPrev->SetImageCheck("DATA/INTERFACE/CONCEPT/UI010.PNG", 52, 228);	//check
	m_pPrev->Show();	

	//Next
	m_pNext = new SPWindowButton(WIID_USERSHOP_SELL_NEXT, 185, 201, 7, 11, pMiddle);
	m_pNext->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG",			269,	378);
	m_pNext->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG",		277,	378);	//on
	m_pNext->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG",		285,	378);	//push
	m_pNext->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG",	293,	378); //disable
	//m_pPrev->SetImageCheck("DATA/INTERFACE/CONCEPT/UI010.PNG", 52, 228);	//check
	m_pNext->Show();

	//Page
	m_pPage = new SPWindowStatic(WIID_USERSHOP_SELL_PAGE, 133, 199, 48, 14, pMiddle);
	m_pPage->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG",			226,	345);
	m_pPage->SetWindowText("0/0");
	m_pPage->SetFormat(DT_CENTER | DT_VCENTER);
	m_pPage->SetFontColor(RGBA(255, 255, 255, 255));
	m_pPage->Show();

	//Ok
	m_pOK = new SPWindowButton(WIID_USERSHOP_SELL_OK, 157, 253, 68, 17, pCenter);
	m_pOK->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG",			1,	215);
	m_pOK->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG",		1,	233);	//on
	m_pOK->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG",		1,	251);	//push
	m_pOK->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG",	1,	269); //disable
	//m_pPrev->SetImageCheck("DATA/INTERFACE/CONCEPT/UI010.PNG", 52, 228);	//check
	m_pOK->Show();

	//Cancel
	m_pCancel = new SPWindowButton(WIID_USERSHOP_SELL_CANCEL, 239, 253, 68, 17, pCenter);
	m_pCancel->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG",			415,	440);
	m_pCancel->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG",		415,	458);	//on
	m_pCancel->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG",		415,	476);	//push
	m_pCancel->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG",	415,	494); //disable
	//m_pPrev->SetImageCheck("DATA/INTERFACE/CONCEPT/UI010.PNG", 52, 228);	//check
	m_pCancel->Show();

	/////////////////bottom
	pBottom  = new SPWindowStatic(WIID_USERSHOP_SELL_BOTTOM, 0, 274, 312, 4, this);
	pBottom->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 11);
	pBottom->Show();
}


void SPWindowUserShopSeller::Show(bool bWithChild /*= true*/)
{
	//[2006/5/29] - 임시 상점개설 창 열릴때 컷인 해제
	g_pEventManager->SetCutInState(CUT_IN_STATE_RELEASE);	
	
	////
	//if(g_pGOBManager->GetLocalPlayer()) {
	//	m_fStartPosX = g_pGOBManager->GetLocalPlayer()->GetPosX();
	//	m_fStartPosY = g_pGOBManager->GetLocalPlayer()->GetPosY();
	//}

	m_iUserShopType = g_pEventManager->GetUserShopArchive()->CheckUserShopClass();
	//[2006/6/7] 현재 위치에서의 상점 개설 체크...
	if(m_iUserShopType < 1)	{
		if(g_pResourceManager->GetGlobalString(12000014)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(12000014);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}

		Hide();
		return;
	}
	
	SPUserShopAttr* pUserShopAttr = g_pEventManager->GetUserShopArchive()->FindUserShopData(m_iUserShopType);
	if(pUserShopAttr == NULL) {
		Hide();
		return;
	}	
	
	SetSellCount(pUserShopAttr->m_iPage);
	m_iCurPage = 0;

	if(m_iMaxPage > 1) {
		m_pNext->SetEnable(true);
	}
	else {
		m_pNext->SetEnable(false);
	}

	if(m_iCurPage == 0) {
		m_pPrev->SetEnable(false);
	}

	SPWindow::Show(bWithChild);		
	m_bAutoClose = false;
	m_bSendClose = false;

	for(int i = 0; i < MAX_USER_SHOP_LIST; i++) {
		m_pItemEly[i]->SetWindowText("0");
	}

	UpdateList();
}


void SPWindowUserShopSeller::Hide(bool bSendServer /*= true*/)
{
	SPPlayerInvenArchive* pInven = NULL;	

	if(g_pGOBManager->GetLocalPlayer()) {
		//[2006/5/29] 개인상점 모션인 경우 인터페이스를 못닫는다
		if(g_pGOBManager->GetLocalPlayer()->GetCurAction() & ACTION_USERSHOP) 
			return;

		pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
		pInven->ClearAllUserShopItem();
	}
	
	ClearEly();
	ClearSellItem();
	//m_iUserShopState = USERSHOP_STATE_NULL;
	m_iUserShopState = USERSHOP_STATE_CREATE;
	m_pName->SetWindowText(NULL);
	m_pName->SetEnable(true);
	m_pOK->SetEnable(true);
	m_iUserShopType = -1;
	SPWindow::Hide(bSendServer);
}


void SPWindowUserShopSeller::UpdateList()
{
	//페이지 정보 갱신	
	char szTemp[32];
	ZeroMemory(szTemp, 32);
	if(m_iMaxPage > 0) {
		wsprintf(szTemp, "%d/%d", m_iCurPage + 1, m_iMaxPage);
	}	
	else if(m_iMaxPage == 0 && m_iCurPage == 0) {
		wsprintf(szTemp, "%d/%d", m_iCurPage, m_iMaxPage);

	}
	m_pPage->SetWindowText(szTemp);
	

	int iStartIndex = m_iCurPage * MAX_USER_SHOP_LIST;

	SPPlayerInvenArchive* pInven = NULL;
	if(g_pGOBManager->GetLocalPlayer())
		pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	else 
		return;

	//실 리스트 갱신
	for(int i = 0; i < MAX_USER_SHOP_LIST; i++, iStartIndex++) {		
		////
		//if(m_bItemEnable[i]) {
		//	m_pItemSkin[i]->Hide();
		//	m_pItemSkin[i]->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 341, 2);
		//	m_pItemSkin[i]->SetSrcSize(2, 2);
		//	m_pItemSkin[i]->Hide();			
		//	m_pItemSkin[i]->Show();
		//	m_pItemEmpty[i]->Hide();
		//	m_pItemIconLine[i]->SetColor(RGBA(255, 255, 255, 128));
		//	m_pItemElyIcon[i]->SetColor(RGBA(255, 255, 255, 128));			
		//}
		//else {
		//	m_pItemSkin[i]->Hide();
		//	m_pItemSkin[i]->SPSendMessage(SPIM_SET_IMAGECLEAR);			
		//	m_pItemSkin[i]->Show();
		//	m_pItemEmpty[i]->Show();
		//	m_pItemIconLine[i]->SetColor(RGBA(255, 255, 255, 255));
		//	m_pItemElyIcon[i]->SetColor(RGBA(255, 255, 255, 255));
		//}

		//아이템 클리어
		m_pItemIcon[i]->SPSendMessage(SPIM_SET_IMAGECLEAR);
		m_pItemIcon[i]->SetWindowText(NULL);
		m_pItemName[i]->SetWindowText(NULL);
		m_bItemAble[i] = true;
		m_iItemMark[i] = ICON_MARK_NULL;
		m_iStack[i] = 0;
		
		char szTemp[64];
		ZeroMemory(szTemp, 64);
		//wsprintf(szTemp, "%I64d", m_iEly[iStartIndex]);
		sprintf(szTemp, "%I64d", m_iEly[iStartIndex]);
		//wsprintf(szTemp, "%d", m_iEly[iStartIndex]);
		m_pItemEly[i]->SetWindowText(szTemp);
		m_pItemEly[i]->SetEnable(false);

		//인터페이스 아이템 아이콘 설정
		SPSelectItem* pSelectItem = pInven->GetUserShopItem(iStartIndex);
		if(!pSelectItem->IsNull()) { //비어있지 않은 경우
			SPItem* pItem = pInven->GetInventoryItem(pSelectItem->m_iContainer, pSelectItem->m_iSlotIndex);
			if(pItem && pItem->GetItemStatus()->GetItemID() > 0) {
				//배경 인터페이스 설정
				//m_pItemSkin[i]->Hide();
				m_pItemSkin[i]->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 493, 477);
				m_pItemSkin[i]->SetSrcSize(6, 4);
				//m_pItemSkin[i]->Hide();			
				m_pItemSkin[i]->Show();
				m_pItemEmpty[i]->Hide();
				m_pItemIconLine[i]->SetColor(RGBA(255, 255, 255, 128));
				m_pItemElyIcon[i]->SetColor(RGBA(255, 255, 255, 128));
				
				
				//실제 아이템 아이콘 설정
				int iStackCount = pItem->GetItemStatusValue(MV_ITEM_GETSTACK_CNT);
				int iRearLevel = pItem->GetItemStatusValue(MV_ITEM_GET_RARELEVEL);
				POINT ptSrcPos;
				SPTexture* pTexture = g_pItemCluster->GetItemTexture(pItem->GetItemStatus()->GetItemID(), ptSrcPos);
				m_pItemIcon[i]->Hide();
				m_pItemIcon[i]->SetImage(pTexture, ptSrcPos.x, ptSrcPos.y);				
				m_pItemIcon[i]->Show();

				if(pItem->GetItemAttr()->m_bNameColor) {
					m_pItemName[i]->SetFontColor(pItem->GetItemAttr()->GetNameColor());
				}
				else {
					m_pItemName[i]->SetFontColor(g_pCheckManager->GetItemColor(iRearLevel));
				}
				m_pItemName[i]->SetWindowText(pItem->GetItemAttr()->m_strName.c_str());
				
				m_iStack[i] = iStackCount;
				m_bItemAble[i] = g_pCheckManager->CheckItemStatus(pItem->GetItemStatus()->GetItemID());
				m_iItemMark[i] = g_pItemCluster->GetItemIconMark(pItem->GetItemStatus()->GetItemID());

				//ELY Edit 입력 가능하도록 풀어준다 
				if(m_iUserShopState == USERSHOP_STATE_CREATE)
					m_pItemEly[i]->SetEnable(true);

				continue;
			}			
		}
		
		//배경 인터페이스 설정
		//m_pItemSkin[i]->Hide();
		m_pItemSkin[i]->SPSendMessage(SPIM_SET_IMAGECLEAR);			
		m_pItemSkin[i]->Show();
		if(m_strBuyer[iStartIndex].empty())
			m_pItemEmpty[i]->Show();
		else {
			m_pItemEmpty[i]->Hide();
			m_pItemName[i]->SetFontColor(RGBA(0, 0, 0, 255));
			m_pItemName[i]->SetWindowText(m_strBuyer[iStartIndex].c_str());
		}
		
		m_pItemIconLine[i]->SetColor(RGBA(255, 255, 255, 255));
		m_pItemElyIcon[i]->SetColor(RGBA(255, 255, 255, 255));
	}
}


void SPWindowUserShopSeller::CloseShop(bool bAutoClose /*= false*/)
{	
	m_bAutoClose = bAutoClose;
	
	SendUserShopClose();

	//
	//m_iUserShopState = USERSHOP_STATE_CREATE;
	m_pName->SetEnable(true);
	for(int i = 0; i < MAX_USER_SHOP_LIST; i++) {
		m_pItemEly[i]->SetEnable(true);
	}
	m_pOK->SetEnable(true);
}


void SPWindowUserShopSeller::ClearEly()
{
	for(int i = 0; i < FM_MAX_SELLITEM; i++) {
		m_iEly[i] = 0;
		m_strBuyer[i] = "";
		m_strBuyer[i].clear();
	}
}


void SPWindowUserShopSeller::ClearSellItem()
{
	std::vector<FM_SELLITEM*>::iterator iter;
	iter = m_vpSellItem.begin();
	for(; iter != m_vpSellItem.end(); ++iter) {
		delete (*iter);
	}
	m_vpSellItem.clear();
}


void SPWindowUserShopSeller::SetSellCount(int iPage)
{
	m_iSellCount = iPage * MAX_USER_SHOP_LIST;
	m_iMaxPage = m_iSellCount / MAX_USER_SHOP_LIST;
}


bool SPWindowUserShopSeller::CreateSellItemList()
{
	SPPlayerInvenArchive* pInven = NULL;
	if(g_pGOBManager->GetLocalPlayer())
		pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	else 
		return false;

	ClearSellItem();

	for(int i = 0; i < FM_MAX_SELLITEM; i++) {
		SPSelectItem* pSelectItem = pInven->GetUserShopItem(i);
		if(pSelectItem == NULL || pSelectItem->IsNull())
			continue;

		SPItem* pItem = pInven->GetInventoryItem(pSelectItem->m_iContainer, pSelectItem->m_iSlotIndex);
		if(pItem->IsEmptyItem())
			continue;		

		if(m_iEly[i] == 0) {
			if(g_pResourceManager->GetGlobalString(12000016)) {
				std::string strMsg = g_pResourceManager->GetGlobalString(12000016);
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
			}
			return false;
		}

		FM_SELLITEM* pSellItem = new FM_SELLITEM;
		if(InitFMSELLITEM(i, pSellItem) == false)
			continue;		

		if(SetFMSLEEITEM(pSellItem, pSelectItem, m_iEly[i]) == false)
			continue;		

		m_vpSellItem.push_back(pSellItem);
	}

	return true;
}


bool SPWindowUserShopSeller::InitFMSELLITEM(int iSlotIndex, FM_SELLITEM* pSellItem)
{
	if(pSellItem) {
		pSellItem->stFM_Item.bySlot = iSlotIndex + 1;			//1베이스
		pSellItem->stFM_Item.byStackCount = 0;
		pSellItem->stFM_Item.uiItemID = 0;
		pSellItem->stFM_Item.uiPrice = 0;
		pSellItem->stInvenSlotInfo.ContainerType = CONTAINER_TYPE_INVALID;
		pSellItem->stInvenSlotInfo.SlotIndex = -1;
		return true;
	}	
	return false;
}


bool SPWindowUserShopSeller::SetFMSLEEITEM(FM_SELLITEM* pSellItem, SPSelectItem* pSelectItem, INT64 iSellPrice)
{
	SPPlayerInvenArchive* pInven = NULL;
	if(g_pGOBManager->GetLocalPlayer())
		pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	else 
		return false;
	
	if(pSellItem == NULL || pSelectItem->IsNull())
		return false;
	
	SPItem* pItem = pInven->GetInventoryItem(pSelectItem->m_iContainer, pSelectItem->m_iSlotIndex);

	if(pItem->IsEmptyItem())
		return false;

	pSellItem->stFM_Item.byStackCount = pItem->GetItemStatus()->GetStackCount();
	pSellItem->stFM_Item.uiItemID = pItem->GetItemStatus()->GetItemID();
	pSellItem->stFM_Item.uiPrice = iSellPrice;
	pSellItem->stInvenSlotInfo.ContainerType = pSelectItem->m_iContainer;
	pSellItem->stInvenSlotInfo.SlotIndex = pSelectItem->m_iSlotIndex + 1;		//1Base
	return true;
}


FM_SELLITEM* SPWindowUserShopSeller::GetFMSELLITEM(int iSlotIndex)
{
	std::vector<FM_SELLITEM*>::iterator iter;
	iter = m_vpSellItem.begin();
	for(; iter != m_vpSellItem.end(); ++iter) {
		if((*iter)->stFM_Item.bySlot == iSlotIndex)
			return (*iter);
	}
	return NULL;
}


bool SPWindowUserShopSeller::SendUserShopOpen()
{
	if(CreateSellItemList() == false)
		return false;

	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return false;

	if(m_iUserShopType < 0 || g_pEventManager->GetUserShopArchive()->FindUserShopData(m_iUserShopType) == NULL) {
		return false;
	}
	
	std::string strShopName = m_pName->GetWindowText();
	
	if(strShopName.size() < 2) {							//상점 명이 2바이트이하일경우
		if(g_pResourceManager->GetGlobalString(12000002)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(12000002);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}
		return false;
	}
	
	//[2006/6/9] 상점명 필터링
	char szBuffer[128];
	int iReplace;
	SPAbuseFilter::GetInstance()->Filter(strShopName.c_str(), szBuffer, 127, &iReplace);
	if(iReplace) {				//검출...
		if(g_pResourceManager->GetGlobalString(12000012)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(12000012);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}
		return false;
	}	

	//char szShopName[FM_MAX_NAME + 1];
	//ZeroMemory(szShopName, FM_MAX_NAME + 1);
	//wsprintf(szShopName, "%s", strShopName.c_str());	

	DECLARE_VAR(TCHAR,szShopName,FM_MAX_NAME + 1);
	_tcsncpy(szShopName, strShopName.c_str(), FM_MAX_NAME);
	
	int iSize = static_cast<int>(m_vpSellItem.size());

	//예외처리.
	int iStage = g_pGOBManager->GetLocalPlayer()->GetCurStageID();
	int iMapGroup = g_pGOBManager->GetLocalPlayer()->GetCurMapGroupID();
	SPMapGroup* pMapGroup = g_StageManager.GetMapGroup(iMapGroup, iStage);
	
	////
	//if(!(pMapGroup && pMapGroup->IsMapGroupType(GROUP_TYPE_MARKET))) {		//프리마켓인지
	//	if(g_pResourceManager->GetGlobalString(12000009)) {
	//		std::string strMsg = g_pResourceManager->GetGlobalString(12000009);
	//		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
	//	}
	//	return false;
	//}

	//[2006/6/7] 내 클래스가 뭐뭐인 경우 맵 체크


	if(iSize < 1 || m_vpSellItem.empty())  {				//물건 없음
		if(g_pResourceManager->GetGlobalString(12000010)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(12000010);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}
		return false;
	}

	if(g_pGOBManager->CheckUserShopOverlap(m_iUserShopType) == false) {	//주위에 상점 있음
		if(g_pResourceManager->GetGlobalString(12000008)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(12000008);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}
		return false;
	}
	
	//[2006/7/7] 상점 개설 액션 상태가 아닌경우
	if(g_pGOBManager->GetLocalPlayer()) {
		UINT64 uiShopAction = ACTION_USERSHOP;
		if(g_pGOBManager->GetLocalPlayer()->GetCoordPhysics()->IsActionChange(uiShopAction) == false){
			return false;
		}
	}
	
	CPacket Packet;
	Packet.Add((UINT32)FREEMARKET_CS_OPEN);
	//UINT8 byType = 1;
	//Packet.AddUINT8(byType);
	Packet.AddUINT8(m_iUserShopType);
	Packet.AddData(szShopName, FM_MAX_NAME + 1);	
	Packet.AddUINT32(iSize);
	for(int i = 0; i < iSize; i++) {		
		Packet.AddData(m_vpSellItem[i], sizeof(FM_SELLITEM));
	}
	g_pNetworkManager->SPPerformMessage(FREEMARKET_CS_OPEN, 0, (LPARAM)&Packet);

	return true;
}


bool SPWindowUserShopSeller::SendUserShopClose()
{	
	//이미 클로즈를 한번 보냈거나 판매 상태가 아닌경우
	if(m_bSendClose || m_iUserShopState != USERSHOP_STATE_SELL)
		return false;

	CPacket Packet;
	Packet.Add((UINT32)FREEMARKET_CS_CLOSE);
	g_pNetworkManager->SPPerformMessage(FREEMARKET_CS_CLOSE, 0, (LPARAM)&Packet);

	m_bSendClose = true;
	return true;
}


bool SPWindowUserShopSeller::IsTradeEnable()
{
	if(g_pGOBManager->GetLocalPlayer()) {
		// 타겟 플레이어 가져오기...		
		//m_pTargetPlayer = (SPPlayer*)g_pGOBManager->Find(m_uiPlayerGUID);
		//if( m_pTargetPlayer == NULL ) {
		//	//OnHide(0, 0);
		//	return false;
		//}

		if(g_pGOBManager->GetLocalPlayer()->IsDead()) {
			return false;
		}

		float fCurPosX = g_pGOBManager->GetLocalPlayer()->GetPosX();
		float fCurPosY = g_pGOBManager->GetLocalPlayer()->GetPosY();	

		if( (fCurPosX < m_fStartPosX - 30 || m_fStartPosX + 30 < fCurPosX) ||
			(fCurPosY < m_fStartPosY - 30 || m_fStartPosY + 30 < fCurPosY))
		{
			return false;
		}
		
		return true;
	}	

	return false;
}


bool SPWindowUserShopSeller::IsEmptyShop()
{
	std::vector<FM_SELLITEM*>::iterator iter;
	iter = m_vpSellItem.begin();
	for(; iter != m_vpSellItem.end(); ++iter) {
		if((*iter)->stFM_Item.uiItemID)
			return false;
	}
	return true;
}




SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowUserShopSeller )
SPIMESSAGE_COMMAND(SPIM_USERSHOP_SELL_OPEN	,	OnShopOpen	)
SPIMESSAGE_COMMAND(SPIM_USERSHOP_SELL_CLOSE	,	OnShopClose )
SPIMESSAGE_COMMAND(SPIM_USERSHOP_SELL_BUY	,	OnShopBuy	)
SPIMESSAGE_COMMAND(SPIM_USERSHOP_SELL_SETCLOSE, OnShopSetClose)

SPIMESSAGE_CONTROL(	SPIM_SETFOCUS	,WIID_USERSHOP_SELL_NAME_EDIT		,OnEditSetFocus)
SPIMESSAGE_CONTROL(	SPIM_KILLFOCUS	,WIID_USERSHOP_SELL_NAME_EDIT		,OnEditKillFocus)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP	,WIID_USERSHOP_SELL_CLOSE			,OnClose)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP	,WIID_USERSHOP_SELL_OK				,OnOk)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP	,WIID_USERSHOP_SELL_CANCEL			,OnCancel)
SPIMESSAGE_CONTROL_RANGE(SPIM_SETFOCUS,		WIID_USERSHOP_SELL_ITEM_ELY,	WIID_USERSHOP_SELL_ITEM_ELY + MAX_USER_SHOP_LIST,	OnEditSetFocusRange	)
SPIMESSAGE_CONTROL_RANGE(SPIM_KILLFOCUS,	WIID_USERSHOP_SELL_ITEM_ELY,	WIID_USERSHOP_SELL_ITEM_ELY + MAX_USER_SHOP_LIST,	OnEditKillFocusRange)
SPIMESSAGE_CONTROL_RANGE(SPIM_LBUTTON_UP,	WIID_USERSHOP_SELL_ITEM_BUTTON, WIID_USERSHOP_SELL_ITEM_BUTTON + MAX_USER_SHOP_LIST, OnItemClickUp)
SPIMESSAGE_CONTROL_RANGE(SPIM_LBUTTON_DOWN,	WIID_USERSHOP_SELL_ITEM_BUTTON, WIID_USERSHOP_SELL_ITEM_BUTTON + MAX_USER_SHOP_LIST, OnItemClickDown)
SPIMESSAGE_CONTROL_RANGE(SPIM_TOOLTIP_ENTER,	WIID_USERSHOP_SELL_ITEM_BUTTON,	WIID_USERSHOP_SELL_ITEM_BUTTON + MAX_USER_SHOP_LIST,	OnToolTipEnter)
SPIMESSAGE_CONTROL_RANGE(SPIM_TOOLTIP_OUT,		WIID_USERSHOP_SELL_ITEM_BUTTON,	WIID_USERSHOP_SELL_ITEM_BUTTON + MAX_USER_SHOP_LIST,	OnToolTipOut)
SPIMESSAGE_CONTROL_RANGE(SPIM_TOOLTIP_MOVE,	WIID_USERSHOP_SELL_ITEM_BUTTON,		WIID_USERSHOP_SELL_ITEM_BUTTON + MAX_USER_SHOP_LIST,	OnToolTipMove)
SPIMESSAGE_CONTROL_RANGE(SPIM_REFRESH,			WIID_USERSHOP_SELL_ITEM_ELY,	WIID_USERSHOP_SELL_ITEM_ELY + MAX_USER_SHOP_LIST,		OnUpdateEly)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP	,WIID_USERSHOP_SELL_PREV			,OnPrev)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP	,WIID_USERSHOP_SELL_NEXT			,OnNext)
SPIMESSAGE_OBJ_MAP_END	( SPWindow	)


int SPWindowUserShopSeller::OnShopOpen(WPARAM wParam, LPARAM lParam)
{
	bool bOpen = static_cast<bool>(wParam);
	
	if(bOpen) {
		m_iUserShopState = USERSHOP_STATE_SELL;
		if(g_pResourceManager->GetGlobalString(12000001)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(12000001);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}
		
		//
		if(g_pGOBManager->GetLocalPlayer()) {
			m_fStartPosX = g_pGOBManager->GetLocalPlayer()->GetPosX();
			m_fStartPosY = g_pGOBManager->GetLocalPlayer()->GetPosY();
		}

		m_bSendClose = false;
	}
	else {
		m_iUserShopState = USERSHOP_STATE_CREATE;
		m_pOK->SetEnable(true);
	}

	return 1;
}


int SPWindowUserShopSeller::OnShopClose(WPARAM wParam, LPARAM lParam)
{
	bool bClose = static_cast<bool>(wParam);
	if(bClose) {
		m_iUserShopState = USERSHOP_STATE_CREATE;

		for(int i = 0; i < MAX_USER_SHOP_LIST; i++) {
			m_pItemEly[i]->SetEnable(false);
		}

		if(m_bAutoClose == false)
			Hide();
	}
	else {
		//에러...
	}
	return 1;
}


int SPWindowUserShopSeller::OnShopBuy(WPARAM wParam, LPARAM lParam)
{
	int iSellSlot = (int)wParam;	

	//if(iSellSlot < 0 || iSellSlot >= static_cast<int>(m_vpSellItem.size()))
	//	return 1;	

	if(iSellSlot < 0)
		return 1;

	//SlotIndex로 파인드
	FM_SELLITEM* pSellItem = GetFMSELLITEM(iSellSlot + 1);		// 1베이스 보정
	if(pSellItem == NULL)
		return 1;	
	
	if(InitFMSELLITEM(iSellSlot, pSellItem) == false){
	}

	m_iEly[iSellSlot] = 0;
	m_strBuyer[iSellSlot] = (char*)lParam;

	UpdateList();

	//아이템 판메 메시지
	char szMsg[256];
	ZeroMemory(szMsg, 256);	
	if(g_pResourceManager->GetGlobalString(12000006)) {
		sprintf(szMsg, g_pResourceManager->GetGlobalString(12000006), m_strBuyer[iSellSlot].c_str());
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMsg /*, (LPARAM)&color*/);
	}

	
	bool bEmpty = IsEmptyShop();
	if(bEmpty) {		//물건이 다 팔린 경우 자동으로 클로즈
		CloseShop(bEmpty);
	}
	return 1;
}


int SPWindowUserShopSeller::OnShopSetClose(WPARAM wParam, LPARAM lParam)
{
	if(m_bShow ==  false)
		return 1;
	
	m_bAutoClose = false;
	if(m_iUserShopState == USERSHOP_STATE_SELL) {
		CloseShop();		
	}
	else if(m_iUserShopState == USERSHOP_STATE_CREATE_REQUEST) {
		return 1;
	}
	else {
		Hide();
	}
	
	return 2;
}


int	SPWindowUserShopSeller::OnEditSetFocus(WPARAM wParam, LPARAM lParam)
{
	g_pInterfaceManager->SetKeyInput( TRUE );

	return 1;
}


int	SPWindowUserShopSeller::OnEditKillFocus(WPARAM wParam, LPARAM lParam)
{
	g_pInterfaceManager->SetKeyInput( FALSE );

	return 1;
}


int SPWindowUserShopSeller::OnClose(WPARAM wParam, LPARAM lParam)
{	
	m_bAutoClose = false;
	if(m_iUserShopState == USERSHOP_STATE_SELL) {
		CloseShop();		
	}
	else if(m_iUserShopState == USERSHOP_STATE_CREATE_REQUEST) {
		return 1;
	}
	else {
		Hide();
	}

	return 1;
}


int SPWindowUserShopSeller::OnPrev(WPARAM wParam, LPARAM lParam)
{
	DXUTOutputDebugString("OnPrev\n");
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


int SPWindowUserShopSeller::OnNext(WPARAM wParam, LPARAM lParam)
{
	DXUTOutputDebugString("OnNext\n");
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


int SPWindowUserShopSeller::OnOk(WPARAM wParam, LPARAM lParam)
{
	DXUTOutputDebugString("SPWindowUserShopSeller::OnOk\n");

	if(m_iUserShopState > USERSHOP_STATE_CREATE) {
		return 1;
	}

	m_iUserShopType = g_pEventManager->GetUserShopArchive()->CheckUserShopClass();
	//[2006/6/7] 버튼 클릭시 현재 위치에서의 상점 개설 체크...
	if(m_iUserShopType < 1)	{
		if(g_pResourceManager->GetGlobalString(12000013)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(12000013);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}
		return 1;
	}
	
	if(SendUserShopOpen()) {
		m_iUserShopState = USERSHOP_STATE_CREATE_REQUEST;
		
		m_pName->SetEnable(false);
		for(int i = 0; i < MAX_USER_SHOP_LIST; i++) {
			m_pItemEly[i]->SetEnable(false);
		}
		m_pOK->SetEnable(false);
	}

	return 1;
}


int SPWindowUserShopSeller::OnCancel(WPARAM wParam, LPARAM lParam)
{
	DXUTOutputDebugString("SPWindowUserShopSeller::OnCancel\n");
	m_bAutoClose = false;	
	if(m_iUserShopState == USERSHOP_STATE_SELL) {
		CloseShop();		
	}
	else if(m_iUserShopState == USERSHOP_STATE_CREATE_REQUEST) {
		return 1;
	}
	else {
		Hide();
	}
	
	return 1;
}


int SPWindowUserShopSeller::ItemClear(WPARAM wParam, LPARAM lParam)
{
	int iIndex = (int)wParam;

	if(iIndex < 0 || iIndex >= FM_MAX_SELLITEM)
		return -1;	

	SPPlayerInvenArchive* pInven = NULL;
	if(g_pGOBManager->GetLocalPlayer())
		pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	else 
		return false;

	if(pInven->ClearUserShopItem(iIndex)) {
		m_iEly[iIndex] = 0;
		UpdateList();
	}	

	//m_pItemIcon[iIndex]->SPSendMessage(SPIM_SET_IMAGECLEAR);
	//m_pItemIcon[iIndex]->SetWindowText(NULL);
	//m_pItemText[iIndex]->SetWindowText(NULL);
	//m_bItemAble[iIndex] = true;
	//m_iItemMark[iIndex] = ICON_MARK_NULL;
	//m_iStack[iIndex] = 0;

	////트레이드 아이템(돈포함)이 없는경우
	//m_pAccept->SetEnable(false);
	//m_pAccept->SPSendMessage(SPIM_BTN_SELECT, false);

	return 1;
}


int	SPWindowUserShopSeller::OnEditSetFocusRange(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	g_pInterfaceManager->SetKeyInput( TRUE );

	return 1;
}


int	SPWindowUserShopSeller::OnEditKillFocusRange(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	g_pInterfaceManager->SetKeyInput( FALSE );

	return 1;
}


int SPWindowUserShopSeller::OnItemClickUp(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	m_iSelectItem = iUiID - WIID_USERSHOP_SELL_ITEM_BUTTON;	
	assert(m_iSelectItem < MAX_USER_SHOP_LIST && "User Shop List 인덱스 에러");	

	m_iSelectIndex = m_iCurPage * MAX_USER_SHOP_LIST + m_iSelectItem;

	if(m_iSelectIndex < 0 || m_iSelectIndex >= m_iSellCount)
		return 1;

	SPPlayerInvenArchive* pInven = NULL;
	if(g_pGOBManager->GetLocalPlayer())
		pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	else 
		return 1;

	// 상점 개설 중비중 상태가 아니면 무시
	if(m_iUserShopState != USERSHOP_STATE_CREATE) {
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
		return 1;
	}

	SPSelectItem* pCurItem = g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetUserShopItem(m_iSelectIndex);		
	
	if(pInven->GetPickupItem().IsNull()) {
		return 1;
	}	

	if(pCurItem->IsNull() == false && g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->IsItemExist(pCurItem->m_iSlotIndex, pCurItem->m_iContainer)) {
		//if(pInven->GetPickupItem().IsNull()) {
		//SendPopItem(iSelectNum);			//메시지 아이템 내림 
		ItemClear(m_iSelectIndex, 0);
		//}
		//else {
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
		//}	

		//[2006/3/13]
		//m_bChanged = true;
		//m_fChangedAccumulate = 0.0f;
		//m_pAccept->SetEnable(false);
		//ItemClear(iSelectNum, 0);		
		return 1;
	}

	////
	//if(g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->SetTradeItem(iSelectNum) == false) {		
	//	if(g_pGOBManager->GetLocalPlayer())
	//		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
	//	return -1;
	//}
	//else {		//거래창에 아이콘 셋팅		
	//	//SetItemIcon(iSelectNum);
	//}

	//if(g_pGOBManager->GetLocalPlayer())
	//	g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);

	//if(IsTradeEnable()) {
		if(pInven->GetPickupItem().IsNull() == false) {
			//[2005/9/2] 장비창에서 교환창으로 내려가는것을 맊는다...
			if(pInven->GetPickupItem().m_iContainer > INVENTORY_HOUSING) {
				g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
				return 1;
			}

			//[2005/10/13] 아이템이 교환 불가인 경우 내려가는것을 맊는다...
			if(pInven->IsTradeEnable() == false) {				//픽업되어 있는 아이템을 체크한다 
				g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
				return 1;
			}			

			//SendPushItem(m_iSelectIndex);							//메시지 전송

			//[2006/3/13]
			//m_bChanged = true;
			//m_fChangedAccumulate = 0.0f;
			//m_pAccept->SetEnable(false);

			//아이템 등록
			if(pInven->IsItemExist(pInven->GetPickupItem().m_iSlotIndex, pInven->GetPickupItem().m_iContainer)) {
				if(pInven->SetUserShopItem(m_iSelectIndex, pInven->GetPickupItem().m_iContainer, pInven->GetPickupItem().m_iSlotIndex))		//SetTradeItem 에서 RecoverIcon 된다
				{					
					UpdateList();
				}
			}
		}
	//}

	return 1;

	
	DXUTOutputDebugString("User Shop Select[%d] Index[%d]\n", m_iSelectItem, m_iSelectIndex);
	return 1;
}


int SPWindowUserShopSeller::OnItemClickDown(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	m_iSelectItem = iUiID - WIID_USERSHOP_SELL_ITEM_BUTTON;	
	assert(m_iSelectItem < MAX_USER_SHOP_LIST && "User Shop List 인덱스 에러");	

	m_iSelectIndex = m_iCurPage * MAX_USER_SHOP_LIST + m_iSelectItem;
	
	DXUTOutputDebugString("User Shop DeSelect[%d] Index[%d]\n", m_iSelectItem, m_iSelectIndex);
	return 1;
}


int SPWindowUserShopSeller::OnUpdateEly(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	int iSelectItem = iUiID - WIID_USERSHOP_SELL_ITEM_ELY;	
	assert(iSelectItem < MAX_USER_SHOP_LIST && "User Shop List 인덱스 에러");	
	int iSelectIndex = m_iCurPage * MAX_USER_SHOP_LIST + iSelectItem;
	
	INT64 iEditEly = _atoi64(m_pItemEly[iSelectItem]->GetWindowText());

	if(iEditEly < 0 || iEditEly > 999999999) {
		m_pItemEly[iSelectItem]->SetWindowText("999999999");
		iEditEly = 999999999;
	}
	
	m_iEly[iSelectIndex] = iEditEly;

	return 1;
}


int SPWindowUserShopSeller::OnToolTipMove(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	m_iSelectItem = iUiID - WIID_USERSHOP_SELL_ITEM_BUTTON;	
	m_iSelectIndex = m_iCurPage * MAX_USER_SHOP_LIST + m_iSelectItem;
	
	SPPlayerInvenArchive* pInven = NULL;
	if(g_pGOBManager->GetLocalPlayer())
		pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	else 
		return 0;

	SPSelectItem* pSelectItem = pInven->GetUserShopItem(m_iSelectIndex);
	if(pSelectItem == NULL)
		return 0;

	if(pSelectItem->m_iContainer < INVENTORY_EQUIP || pSelectItem->m_iContainer > INVENTORY_HOUSING) {
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
		return 0;
	}

	if(m_iSelectIndex < 0 || m_iSelectIndex >= pInven->GetInventorySize(pSelectItem->m_iContainer)) {
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
		return 0;
	}

	if(pInven->IsItemExist(pSelectItem->m_iSlotIndex, pSelectItem->m_iContainer) == false) {
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
		return 0;
	}

	SPItem* pItem = pInven->GetInventoryItem(pSelectItem->m_iContainer, pSelectItem->m_iSlotIndex);

	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage( SPIM_TOOLTIP_MOVE, (WPARAM)pItem->GetItemStatus(), lParam);
	pWindow->Show();
	return 1;
}


int SPWindowUserShopSeller::OnToolTipEnter(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	m_iSelectItem = iUiID - WIID_USERSHOP_SELL_ITEM_BUTTON;	
	m_iSelectIndex = m_iCurPage * MAX_USER_SHOP_LIST + m_iSelectItem;

	SPPlayerInvenArchive* pInven = NULL;
	if(g_pGOBManager->GetLocalPlayer())
		pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	else 
		return 0;

	SPSelectItem* pSelectItem = pInven->GetUserShopItem(m_iSelectIndex);
	if(pSelectItem == NULL)
		return 0;

	if(pSelectItem->m_iContainer < INVENTORY_EQUIP || pSelectItem->m_iContainer > INVENTORY_HOUSING) {
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
		return 0;
	}

	if(m_iSelectIndex < 0 || m_iSelectIndex >= pInven->GetInventorySize(pSelectItem->m_iContainer)) {
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
		return 0;
	}

	if(pInven->IsItemExist(pSelectItem->m_iSlotIndex, pSelectItem->m_iContainer) == false) {
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
		return 0;
	}

	SPItem* pItem = pInven->GetInventoryItem(pSelectItem->m_iContainer, pSelectItem->m_iSlotIndex);

	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage( SPIM_TOOLTIP_MOVE, (WPARAM)pItem->GetItemStatus(), lParam);
	pWindow->Show();

	return 1;
}


int SPWindowUserShopSeller::OnToolTipOut(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
	pWindow->Show();
	return 1;
}