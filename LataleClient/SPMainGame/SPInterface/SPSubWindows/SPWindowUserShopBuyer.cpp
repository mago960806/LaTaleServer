
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
#include "SPPet.h"
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
#include "SPWindowUserShopBuyer.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPItem.h"
#include "SPItemCluster.h"

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

#include "SPCheckManager.h"

#include "SPDebug.h"


SPWindowUserShopBuyer::SPWindowUserShopBuyer(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_USERSHOP_BUY, InstanceID, iX, iY, iCX, iCY, pParent)
{	
	Init();
}


SPWindowUserShopBuyer::~SPWindowUserShopBuyer()
{		
	Clean();	
}


void SPWindowUserShopBuyer::Init()
{
	CreateItemList();

	m_iCurPage = 0;
	m_iMaxPage = FM_MAX_SELLITEM / MAX_USER_SHOP_LIST;
	m_iSelectItem = -1;
	m_iSelectIndex = -1;

	m_pTargetPlayer = NULL;
	m_fStartPosX = 0.0f;
	m_fStartPosY = 0.0f;
	m_bPetShop = false;
	m_iShopStyle = -1;
	m_btnCallHost = NULL;

	std::string strTextureFile = "";
	strTextureFile.clear();
	strTextureFile = "DATA/INTERFACE/CONCEPT/UI100.PNG";
	g_pVideo->GetTextureMgr()->LoadTexture(strTextureFile.c_str(), &m_pDisable);
	
	SetRect(&m_rtSrcDisable, 434 , 37 , 436 , 39);
	SetRect(&m_rtMarkSrc[0], 479, 152, 511, 184);		//ICON_MARK_LOCK
	SetRect(&m_rtMarkSrc[1], 479, 185, 511, 217);		//ICON_MARK_FASHION
	SetRect(&m_rtMarkSrc[2], 479, 339, 511, 371);		//ICON_MARK_EVENT
	SetRect(&m_rtMarkSrc[3], 479, 372, 511, 404);		//ICON_MARK_FANCY

	InitSubControl();
}


void SPWindowUserShopBuyer::Clean()
{
	DeleteAllItemList();
	SAFE_RELEASE(m_pDisable);
	SPWindow::Clean();
}


void SPWindowUserShopBuyer::Show(bool bWithChild /*= true*/)
{
	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return;
	
	if (!m_bPetShop)
	{
		if(g_pGOBManager->GetLocalPlayer()->GetUserShopID()) {
			m_pTargetPlayer = (SPPlayer*)g_pGOBManager->Find(g_pGOBManager->GetLocalPlayer()->GetUserShopID());		
			m_fStartPosX = g_pGOBManager->GetLocalPlayer()->GetPosX();
			m_fStartPosY = g_pGOBManager->GetLocalPlayer()->GetPosY();

			int iUserShopType = m_pTargetPlayer->GetUserShopType();
			SPUserShopAttr* pShopAttr = g_pEventManager->GetUserShopArchive()->FindUserShopData(iUserShopType);
			m_iCurPage = 0;
			if(pShopAttr) {
				m_iMaxPage = pShopAttr->m_iPage;
			}

			if(m_iMaxPage > 1) {
				m_pNext->SetEnable(true);
			}
			else {
				m_pNext->SetEnable(false);
			}

			if(m_iCurPage == 0) {
				m_pPrev->SetEnable(false);
			}
		}
	}
	else // if (m_bPetShop)
	{
		SPUserShopAttr *pShopAttr = g_pEventManager->GetUserShopArchive()->FindUserShopData( m_iShopStyle );
		m_iCurPage = 0;
		if (pShopAttr)
			m_iMaxPage = pShopAttr->m_iPage;

		if (m_iMaxPage > 1)
			m_pNext->SetEnable( true );
		else
			m_pNext->SetEnable( false );

		if (m_iCurPage == 0)
			m_pPrev->SetEnable( false );
	}

	SPWindow::Show(bWithChild);
	
	for(int i = 0; i < MAX_USER_SHOP_LIST; i++) {
		//Select 해제
		m_pItemSkinSelect[i]->Hide();
	}	
}


void SPWindowUserShopBuyer::Hide(bool bSendServer /*= true*/)
{
	ClearAllItemList();
	m_pTitle->SetWindowText(NULL);
	SPWindow::Hide(bSendServer);
}


void SPWindowUserShopBuyer::Process(float fTime)
{
	//트레이드가 가능한 상황인지를 검사한다.
	if( !m_bShow )
		return;

	if(IsTradeEnable() == false) {
		SendUserShopOut();
	}
	
	SPWindow::Process(fTime);
}


void SPWindowUserShopBuyer::Render(float fTime)
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


void SPWindowUserShopBuyer::InitSubControl()
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
	pTop = new SPWindowStatic(WIID_USERSHOP_BUY_TOP, 0, 0, 312, 4, this);
	pTop->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 1);
	pTop->Show();

	/////////////////Center
	pCenter = new SPWindowStatic(WIID_USERSHOP_BUY_CENTER, 0, 4, 312, 270, this);
	pCenter->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 7);
	pCenter->SetSrcSize(312, 2);
	pCenter->Show();

	//Title
	pStatic = new SPWindowStatic(WIID_USERSHOP_BUY_TITLE, 10, 6, 96, 15, pCenter);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 204, 313);
	pStatic->Show();
	pStatic = NULL;

	//Close
	m_pClose = new SPWindowButton(WIID_USERSHOP_BUY_CLOSE, 291, 10, 12, 12, pCenter);
	m_pClose->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG",			301,	321);
	m_pClose->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG",		314,	321);	//on
	m_pClose->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG",		327,	321);	//push
	m_pClose->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG",	340,	321); //disable
	//m_pPrev->SetImageCheck("DATA/INTERFACE/CONCEPT/UI010.PNG", 52, 228);	//check

	/////////////////Middle
	pMiddle = new SPWindowStatic(WIID_USERSHOP_BUY_MIDDLE, 2, 26, 308, 221, pCenter);
	pMiddle->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 493, 486);
	pMiddle->SetSrcSize(6, 4);
	pMiddle->Show();

	//ShopName Image
	pStatic = new SPWindowStatic(WIID_USERSHOP_BUY_NAME_IMAGE, 53, 34 - 26, 63, 11, pMiddle);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 275, 347);
	pStatic->Show();
	pStatic = NULL;

	//ShopName
	SPWindowStatic* pStaticWnd = NULL;
	pStaticWnd = new SPWindowStatic(WIID_USERSHOP_BUY_NAME, 126, 31 - 26, 275+31-126, 14, pMiddle);
	pStaticWnd->SetWindowType(WND_TYPE_LEFTBASE);
	pStaticWnd->SetImageNormalHead	("DATA/INTERFACE/CONCEPT/UI103.PNG", 226, 345, 228, 359);
	pStaticWnd->SetImageNormalBody	("DATA/INTERFACE/CONCEPT/UI103.PNG", 228, 345, 230, 359);
	pStaticWnd->SetImageNormalTail	("DATA/INTERFACE/CONCEPT/UI103.PNG", 272, 345, 274, 359);
	pStaticWnd->SetFormat(DT_CENTER | DT_VCENTER);
	pStaticWnd->SetWindowText("Test..");
	pStaticWnd->SetFontColor(RGBA(255, 255, 255, 255));
	pStaticWnd->Show();
	m_pTitle = pStaticWnd;
	
	int iListY[MAX_USER_SHOP_LIST] = { 27, 68, 109, 150 };

	//
	for(int i = 0; i < MAX_USER_SHOP_LIST; i++) {
		//Skin
		pParent = new SPWindowStatic(WIID_USERSHOP_BUY_ITEM_SKIN + i, 0, iListY[i], 308, 41, pMiddle);
		pParent->Show();		
		m_pItemSkin[i] = pParent;	

		////Ely
		//m_pItemEly[i] = new SPWindowEdit(WIID_USERSHOP_BUY_ITEM_ELY + i, 191, 10, 76, 17, pParent);
		//m_pItemEly[i]->SetImageNormal("DATA/INTERFACE/CONCEPT/UI103.PNG", 455, 16);
		//m_pItemEly[i]->SetImageFocus("DATA/INTERFACE/CONCEPT/UI103.PNG", 466, 16);
		//m_pItemEly[i]->SetScaleSrc(459, 470, 2, 463, 474);
		//m_pItemEly[i]->SetFontColor(RGBA(143, 99, 99, 255));
		//m_pItemEly[i]->SetNumber();
		//m_pItemEly[i]->SetMaxValue(999999999);
		//m_pItemEly[i]->SetFormat(DT_RIGHT | DT_VCENTER);
		//m_pItemEly[i]->Show();		

		//Button
		m_pItem[i] = new SPWindowButton(WIID_USERSHOP_BUY_ITEM_BUTTON + i, 0, 0, 308, 41, pParent);		
		m_pItem[i]->Show();

		//Item 존재시 추가 스킨
		SPWindowStatic* pStaticWnd = NULL;
		pStaticWnd = new SPWindowStatic(WIID_USERSHOP_BUY_ITEM_ENABLE + i, 8, 4, 292, 32, pParent);
		pStaticWnd->SetWindowType(WND_TYPE_LEFTBASE);
		pStaticWnd->SetImageNormalHead	("DATA/INTERFACE/CONCEPT/UI103.PNG", 355, 179, 389, 211);
		pStaticWnd->SetImageNormalBody	("DATA/INTERFACE/CONCEPT/UI103.PNG", 262, 407, 264, 439);
		pStaticWnd->SetImageNormalTail	("DATA/INTERFACE/CONCEPT/UI103.PNG", 266, 407, 268, 439);		
		pStaticWnd->Hide();
		m_pItemSkinSelect[i] = pStaticWnd;

		//Ely
		m_pItemEly[i] = new SPWindowStatic(WIID_USERSHOP_BUY_ITEM_ELY + i, 201, 10, 72, 17, pParent);
		//m_pItemEly[i]->SetWindowText("0000100");
		m_pItemEly[i]->SetFormat(DT_RIGHT | DT_VCENTER);
		m_pItemEly[i]->Show();

		//Line
		//pStatic = new SPWindowStatic(WIID_USERSHOP_BUY_ITEM_LINE + i, 46, 40, 251, 1, pParent);
		//pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 213);

		//Item Icon Line
		m_pItemIconLine[i] = new SPWindowStatic(WIID_USERShOP_BUY_ITEM_ICONLINE + i, 10, 4, 32, 32, pParent);
		m_pItemIconLine[i]->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 203, 408);
		m_pItemIconLine[i]->Show();

		//Item Icon
		m_pItemIcon[i] = new SPWindowStatic(WIID_USERSHOP_BUY_ITEM_ICON + i, 10, 4, 32, 32, pParent);
		m_pItemIcon[i]->Show();

		//Name
		m_pItemName[i] = new SPWindowStatic(WIID_USERSHOP_BUY_ITEM_NAME + i, 51, 13, 147, 13, pParent);
		//m_pItemName[i]->SetWindowText("Test...");
		m_pItemName[i]->SetFontColor(RGBA(143, 99, 99, 255));
		m_pItemName[i]->Show();

		//Name Empty
		m_pItemEmpty[i] = new SPWindowStatic(WIID_USERSHOP_BUY_ITEM_EMPTY + i, 104, 15, 36, 7, pParent);
		m_pItemEmpty[i]->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 336, 382);
		m_pItemEmpty[i]->Show();

		//Ely Icon
		m_pItemElyIcon[i] = new SPWindowStatic(WIID_USERSHOP_BUY_ITEM_ELY_ICON, 279, 19, 17, 5, pParent);
		m_pItemElyIcon[i]->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 494, 506);
		m_pItemElyIcon[i]->Show();

		//m_bItemEnable[i] = false;
		m_bItemAble[i] = true;
		m_iItemMark[i] = ICON_MARK_NULL;
		m_iStack[i] = 0;		
		m_pItemIcon[i]->GetWindowRect(m_rtStack[i]);	
	}

	//Prev
	m_pPrev = new SPWindowButton(WIID_USERSHOP_BUY_PREV, 122, 201, 7, 10, pMiddle);
	m_pPrev->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG",			238,	378);
	m_pPrev->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG",		246,	378);	//on
	m_pPrev->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG",		254,	378);	//push
	m_pPrev->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG",	262,	378); //disable
	//m_pPrev->SetImageCheck("DATA/INTERFACE/CONCEPT/UI010.PNG", 52, 228);	//check
	m_pPrev->Show();	

	//Next
	m_pNext = new SPWindowButton(WIID_USERSHOP_BUY_NEXT, 185, 201, 7, 10, pMiddle);
	m_pNext->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG",			269,	378);
	m_pNext->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG",		277,	378);	//on
	m_pNext->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG",		285,	378);	//push
	m_pNext->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG",	293,	378); //disable
	//m_pPrev->SetImageCheck("DATA/INTERFACE/CONCEPT/UI010.PNG", 52, 228);	//check
	m_pNext->Show();

	//Page
	m_pPage = new SPWindowStatic(WIID_USERSHOP_BUY_PAGE, 133, 199, 48, 14, pMiddle);
	m_pPage->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG",			226,	345);
	m_pPage->SetWindowText("0/0");
	m_pPage->SetFormat(DT_CENTER | DT_VCENTER);
	m_pPage->SetFontColor(RGBA(255, 255, 255, 255));
	m_pPage->Show();

	//Ok
	m_pOK = new SPWindowButton(WIID_USERSHOP_BUY_OK, 157, 253, 68, 17, pCenter);
	m_pOK->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG",			70,	215);
	m_pOK->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG",		70,	233);	//on
	m_pOK->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG",		70,	251);	//push
	m_pOK->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG",	70,	269); //disable
	//m_pPrev->SetImageCheck("DATA/INTERFACE/CONCEPT/UI010.PNG", 52, 228);	//check
	m_pOK->Show();

	//Cancel
	m_pCancel = new SPWindowButton(WIID_USERSHOP_BUY_CANCEL, 229, 253, 68, 17, pCenter);
	m_pCancel->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG",			139,     215);
	m_pCancel->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG",		139,	 233);	//on
	m_pCancel->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG",		139,	 251);	//push
	m_pCancel->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG",	139,	 269); //disable
	//m_pPrev->SetImageCheck("DATA/INTERFACE/CONCEPT/UI010.PNG", 52, 228);	//check
	m_pCancel->Show();

	// send msg
	m_btnCallHost = new SPWindowButton( WIID_USERSHOP_BUY_CALLHOST, 78, 253, 68, 17, pCenter );
	m_btnCallHost->SetImage("DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG",			279,     362);
	m_btnCallHost->SetImageHit("DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG",		279,	 380);	//on
	m_btnCallHost->SetImagePush("DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG",		279,	 398);	//push
	m_btnCallHost->SetImageDisable("DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG",	279,	 416); //disable
	m_btnCallHost->Show();

	/////////////////bottom
	pBottom  = new SPWindowStatic(WIID_USERSHOP_BUY_BOTTOM, 0, 274, 312, 4, this);
	pBottom->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 11);
	pBottom->Show();
}


void SPWindowUserShopBuyer::CreateItemList()
{
	for(int i = 0; i < FM_MAX_SELLITEM ; i++) {
		SPUserSellItem* pSellItem = new SPUserSellItem(i);
		m_vpSellItem.push_back(pSellItem);
	}
	m_vpViewItem.resize( FM_MAX_SELLITEM );
}


void SPWindowUserShopBuyer::DeleteAllItemList()
{
	std::vector<SPUserSellItem*>::iterator iter;
	iter = m_vpSellItem.begin();
	for(; iter != m_vpSellItem.end(); ++iter) {
		delete (*iter);
	}
	m_vpSellItem.clear();
	m_vpViewItem.clear();
}


void SPWindowUserShopBuyer::ClearAllItemList()
{
	std::vector<SPUserSellItem*>::iterator iter;
	iter = m_vpSellItem.begin();
	for(; iter != m_vpSellItem.end(); ++iter) {
		(*iter)->Clear();
	}
}


bool SPWindowUserShopBuyer::IsTradeEnable()
{
	if (m_bPetShop)
		return true;

	if(g_pGOBManager->GetLocalPlayer()) {
		// 타겟 플레이어 가져오기...
		if(g_pGOBManager->GetLocalPlayer()->GetUserShopID()) {
			m_pTargetPlayer = (SPPlayer*)g_pGOBManager->Find(g_pGOBManager->GetLocalPlayer()->GetUserShopID());
			if( m_pTargetPlayer == NULL ) {
				//OnHide(0, 0);
				return false;
			}

			if(m_pTargetPlayer->GetCurAction() & ACTION_USERSHOP) {
				;
			}
			else {
				return false;
			}
		}

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


bool SPWindowUserShopBuyer::SendUserShopOut()
{
	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return false;
	
	////
	//GU_ID uiSellUser = g_pGOBManager->GetLocalPlayer()->GetUserShopID();	
	//CPacket Packet;
	//Packet.Add((UINT32)FREEMARKET_CS_OUT);
	//Packet.AddUINT64(uiSellUser);
	//g_pNetworkManager->SPPerformMessage(FREEMARKET_CS_OUT, 0, (LPARAM)&Packet);

	//[2006/7/13] - 인터페이스가 보여지고 있는 상황에서만 팻킷을 보낸다
	if(m_bShow)
	{
		if (m_bPetShop)
		{
			g_pGOBManager->GetLocalPlayer()->SendPetShopOut();
		}
		else
		{
			g_pGOBManager->GetLocalPlayer()->SendUserShopOut();
		}
	}
	
	return true;
}


void SPWindowUserShopBuyer::UpdateList()
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
	
	m_iSelectIndex = -1;					//셀렉트 초기화

	for(int i = 0; i < MAX_USER_SHOP_LIST; i++, iStartIndex++) {		
		//아이템 클리어
		m_pItemIcon[i]->SPSendMessage(SPIM_SET_IMAGECLEAR);
		m_pItemIcon[i]->SetWindowText(NULL);
		m_pItemName[i]->SetWindowText(NULL);
		m_bItemAble[i] = true;
		m_iItemMark[i] = ICON_MARK_NULL;
		m_iStack[i] = 0;
		m_pItemEly[i]->SetWindowText("0");
		
		//배경 인터페이스 설정
		//m_pItemSkin[i]->Hide();
		m_pItemSkin[i]->SPSendMessage(SPIM_SET_IMAGECLEAR);			
		m_pItemSkin[i]->Show();
		m_pItemEmpty[i]->Show();
		m_pItemIconLine[i]->SetColor(RGBA(255, 255, 255, 255));
		m_pItemElyIcon[i]->SetColor(RGBA(255, 255, 255, 255));

		//Select 해제
		m_pItemSkinSelect[i]->Hide();

		if(m_vpSellItem.at(iStartIndex)->m_SellInfo.uiItemID == 0)
			continue;		

		//인터페이스 아이템 아이콘 설정
		//SPSelectItem* pSelectItem = pInven->GetUserShopItem(iStartIndex);
		//if(!pSelectItem->IsNull()) { //비어있지 않은 경우
		//SPItem* pItem = pInven->GetInventoryItem(pSelectItem->m_iContainer, pSelectItem->m_iSlotIndex);

		SPItem* pItem = m_vpSellItem.at(iStartIndex)->m_pItem;
		if(pItem && pItem->GetItemStatus()->GetItemID() > 0) {
			//배경 인터페이스 설정
			m_pItemSkin[i]->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 493, 477);
			m_pItemSkin[i]->SetSrcSize(6, 4);
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
			//if(m_iUserShopState == USERSHOP_STATE_CREATE)
			//	m_pItemEly[i]->SetEnable(true);

			//char szTemp[64];
			//ZeroMemory(szTemp, 64);
			//wsprintf(szTemp, "%I64d", m_vpSellItem.at(iStartIndex)->m_SellInfo.uiPrice);
			m_pItemEly[i]->SetWindowText(ConvertMoneyToString(m_vpSellItem.at(iStartIndex)->m_SellInfo.uiPrice));
			m_pItemEly[i]->SetEnable(false);

			//Select 해제
			m_pItemSkinSelect[i]->Hide();

			continue;
		}			
		//}

		////배경 인터페이스 설정
		////m_pItemSkin[i]->Hide();
		//m_pItemSkin[i]->SPSendMessage(SPIM_SET_IMAGECLEAR);			
		//m_pItemSkin[i]->Show();
		//m_pItemEmpty[i]->Show();
		//m_pItemIconLine[i]->SetColor(RGBA(255, 255, 255, 255));
		//m_pItemElyIcon[i]->SetColor(RGBA(255, 255, 255, 255));
	}
}


SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowUserShopBuyer )
SPIMESSAGE_COMMAND(SPIM_USERSHOP_BUY_IN		,	OnShopIn	)
SPIMESSAGE_COMMAND(SPIM_USERSHOP_BUY_OUT	,	OnShopOut	)
SPIMESSAGE_COMMAND(SPIM_USERSHOP_BUY_BUY	,	OnShopBuy	)
SPIMESSAGE_COMMAND(SPIM_PETSHOP_BUY_IN		,	OnShopIn	)
SPIMESSAGE_COMMAND(SPIM_PETSHOP_BUY_OUT		,	OnShopOut	)
SPIMESSAGE_COMMAND(SPIM_PETSHOP_BUY_BUY		,	OnShopBuy	)
SPIMESSAGE_COMMAND(SPIM_USERSHOP_SELL_SETCLOSE, OnShopSetClose)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP	,WIID_USERSHOP_BUY_CLOSE		,OnClose)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP	,WIID_USERSHOP_BUY_OK			,OnOk)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP	,WIID_USERSHOP_BUY_CANCEL		,OnCancel)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP	,WIID_USERSHOP_BUY_PREV			,OnPrev)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP	,WIID_USERSHOP_BUY_NEXT			,OnNext)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP	,WIID_USERSHOP_BUY_CALLHOST		,OnCallHost)
SPIMESSAGE_CONTROL_RANGE(SPIM_LBUTTON_UP,	WIID_USERSHOP_BUY_ITEM_BUTTON, WIID_USERSHOP_BUY_ITEM_BUTTON + MAX_USER_SHOP_LIST,	OnItemClickUp)
SPIMESSAGE_CONTROL_RANGE(SPIM_LBUTTON_DOWN,	WIID_USERSHOP_BUY_ITEM_BUTTON, WIID_USERSHOP_BUY_ITEM_BUTTON + MAX_USER_SHOP_LIST,	OnItemClickDown)
SPIMESSAGE_CONTROL_RANGE(SPIM_TOOLTIP_ENTER,	WIID_USERSHOP_BUY_ITEM_BUTTON,	WIID_USERSHOP_BUY_ITEM_BUTTON + MAX_USER_SHOP_LIST,	OnToolTipEnter)
SPIMESSAGE_CONTROL_RANGE(SPIM_TOOLTIP_OUT,		WIID_USERSHOP_BUY_ITEM_BUTTON,	WIID_USERSHOP_BUY_ITEM_BUTTON + MAX_USER_SHOP_LIST,	OnToolTipOut)
SPIMESSAGE_CONTROL_RANGE(SPIM_TOOLTIP_MOVE,	WIID_USERSHOP_BUY_ITEM_BUTTON,		WIID_USERSHOP_BUY_ITEM_BUTTON + MAX_USER_SHOP_LIST,	OnToolTipMove)
SPIMESSAGE_OBJ_MAP_END	( SPWindow	)


int SPWindowUserShopBuyer::OnShopIn(WPARAM wParam, LPARAM lParam)
{
	CPacket* packet = (CPacket*)wParam;
	if( packet == NULL )
		return 1;

	std::string strMessage = (char*)(lParam);
	
	UINT32			iCnt;			// 판매아이템 개수
	FM_VIEWITEM		stFM_ViewItem;	// 판매아이템 정보

	packet->ExtractUINT32(&iCnt);
	for(int i = 0; i < iCnt; i++) {
		packet->ExtractStruct(&stFM_ViewItem, sizeof(FM_VIEWITEM));
		stFM_ViewItem.stFM_Item.bySlot--;
		if(stFM_ViewItem.stFM_Item.bySlot < 0 || stFM_ViewItem.stFM_Item.bySlot >= static_cast<int>(m_vpSellItem.size()))
			continue;
		
		m_vpSellItem.at(stFM_ViewItem.stFM_Item.bySlot)->SetSellItem(&stFM_ViewItem);
		m_vpViewItem.at(stFM_ViewItem.stFM_Item.bySlot) = stFM_ViewItem;
	}

	Show();
	UpdateList();	
	m_pTitle->SetWindowText(strMessage.c_str());

	return 1;
}


int SPWindowUserShopBuyer::OnShopOut(WPARAM wParam, LPARAM lParam)
{
	Hide();
	return 1;
}


int SPWindowUserShopBuyer::OnShopBuy(WPARAM wParam, LPARAM lParam)
{
	int iSellSlot = (int)wParam;

	if(iSellSlot < 0 || iSellSlot >= static_cast<int>(m_vpSellItem.size()))
		return 1;

	std::string strSeller = "";
	if(lParam) {
		strSeller = (char*)lParam;
	}	

	m_vpSellItem.at(iSellSlot)->Clear();

	int iShowIndex = iSellSlot % MAX_USER_SHOP_LIST;

	m_bItemAble[iShowIndex] = true;
	m_iStack[iShowIndex] = 0;		
	m_iItemMark[iShowIndex] = ICON_MARK_NULL;

	//아이템 구입 메시지
	if(lParam) {
		char szMsg[256];
		ZeroMemory(szMsg, 256);	
		if(g_pResourceManager->GetGlobalString(12000007)) {
			sprintf(szMsg, g_pResourceManager->GetGlobalString(12000007), strSeller.c_str());
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMsg /*, (LPARAM)&color*/);
		}
	}	

	UpdateList();
	return 1;
}

int SPWindowUserShopBuyer::OnShopSetClose(WPARAM wParam, LPARAM lParam)
{
	if(m_bShow ==  false)
		return 1;
	
	SendUserShopOut();

	return 2;
}


int SPWindowUserShopBuyer::OnClose(WPARAM wParam, LPARAM lParam)
{	
	SendUserShopOut();
	return 1;
}


int SPWindowUserShopBuyer::OnPrev(WPARAM wParam, LPARAM lParam)
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


int SPWindowUserShopBuyer::OnNext(WPARAM wParam, LPARAM lParam)
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


int SPWindowUserShopBuyer::OnOk(WPARAM wParam, LPARAM lParam)
{
	if (!m_bPetShop)
	{
		//실제 구매작업
		// GU_ID			FMGUID;			// 상점주인의 게임오브젝트아이디
		// FM_ITEM			stFM_Item;		// 사고싶은 아이템 정보
		if(g_pGOBManager->GetLocalPlayer() == NULL)
			return 1;

		GU_ID uiSellUser = g_pGOBManager->GetLocalPlayer()->GetUserShopID();
		SPPlayer* pPlayer = (SPPlayer*)g_pGOBManager->Find(uiSellUser);
		if(pPlayer == NULL)
			return 1;
		
		if(m_iSelectIndex < 0 || m_iSelectIndex >= static_cast<int>(m_vpSellItem.size()))
			return 1;	

		SPUserSellItem* pSellItem = m_vpSellItem.at(m_iSelectIndex);
		if(pSellItem == NULL)
			return 1;

		if(pSellItem->m_SellInfo.uiItemID == 0)
			return 1;

		if(pSellItem->m_pItem->GetItemStatus()->GetItemID() == 0)
			return 1;

		//물건 값이 내 보유금액보다 비싼경우
		if(pSellItem->m_SellInfo.uiPrice > g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValueEX(STATUS_EX_ELY))
		{
			if(g_pResourceManager->GetGlobalString(12000011)) { //ID는 수정할것
				std::string strMsg = g_pResourceManager->GetGlobalString(12000011);
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
			}
			return 1;
		}

		SPPlayerInvenArchive* pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
		if(pInven->IsFullInventory((CONTAINER_TYPE)(pSellItem->m_pItem->GetItemAttr()->m_eType - 1))) {
			DXUTOutputDebugString("SPWindowUserShopBuyer ItemBuy FullInventory\n");
			return 1;
		}

		pSellItem->m_SellInfo.bySlot++;

		CPacket Packet;
		Packet.Add((UINT32)FREEMARKET_CS_BUY);
		Packet.AddUINT64(uiSellUser);
		Packet.AddData(&pSellItem->m_SellInfo, sizeof(FM_ITEM));
		g_pNetworkManager->SPPerformMessage(FREEMARKET_CS_BUY, 0, (LPARAM)&Packet);

		pSellItem->m_SellInfo.bySlot--;
	}
	else // if (m_bPetShop)
	{
		if(g_pGOBManager->GetLocalPlayer() == NULL)
			return 1;

		if(m_iSelectIndex < 0 || m_iSelectIndex >= static_cast<int>(m_vpSellItem.size()))
			return 1;	

		SPUserSellItem* pSellItem = m_vpSellItem.at(m_iSelectIndex);
		if(pSellItem == NULL)
			return 1;

		if(pSellItem->m_SellInfo.uiItemID == 0)
			return 1;

		if(pSellItem->m_pItem->GetItemStatus()->GetItemID() == 0)
			return 1;

		//물건 값이 내 보유금액보다 비싼경우
		if(pSellItem->m_SellInfo.uiPrice > g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValueEX(STATUS_EX_ELY))
		{
			if(g_pResourceManager->GetGlobalString(12000011)) {
				std::string strMsg = g_pResourceManager->GetGlobalString(12000011);
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
			}
			return 1;
		}

		SPPlayerInvenArchive* pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
		if(pInven->IsFullInventory((CONTAINER_TYPE)(pSellItem->m_pItem->GetItemAttr()->m_eType - 1))) {
			DXUTOutputDebugString("SPWindowUserShopBuyer ItemBuy FullInventory\n");
			return 1;
		}

		pSellItem->m_SellInfo.bySlot++;

		CPacket Packet;
		Packet.Add((UINT32)PET_CS_BUY_ITEM);
		Packet.AddUINT64(m_uiShopGUID);
		Packet.AddUINT8((UINT8)m_iSelectIndex+1);
		//Packet.AddData(&pSellItem->m_SellInfo, sizeof(FM_ITEM));
		Packet.AddData( &m_vpViewItem.at(m_iSelectIndex), sizeof(FM_VIEWITEM) );
		g_pNetworkManager->SPPerformMessage(PET_CS_BUY_ITEM, 0, (LPARAM)&Packet);

		pSellItem->m_SellInfo.bySlot--;
	}

	return 1;
}


int SPWindowUserShopBuyer::OnCancel(WPARAM wParam, LPARAM lParam)
{
	SendUserShopOut();
	return 1;
}

int SPWindowUserShopBuyer::OnCallHost(WPARAM wParam, LPARAM lParam)
{
	SPPet *pet = (SPPet*)g_pGOBManager->Find( m_uiShopGUID );
	if (pet)
	{
		std::string strHostName = pet->GetHostName();
		if (!strHostName.empty())
			return g_pInterfaceManager->SPChildWndSendMessage( WIID_CHAT , SPIM_WNDCHAT_SET_WHISPER , (WPARAM)strHostName.c_str() , lParam );
	}
			return 0;
}

int SPWindowUserShopBuyer::OnItemClickUp(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	m_iSelectItem = iUiID - WIID_USERSHOP_BUY_ITEM_BUTTON;	
	assert(m_iSelectItem < MAX_USER_SHOP_LIST && "User Shop List 인덱스 에러");	
	m_iSelectIndex = m_iCurPage * MAX_USER_SHOP_LIST + m_iSelectItem;

	for(int i = 0 ; i < MAX_USER_SHOP_LIST ; i++ ){
		if(m_iSelectItem == i){
			m_pItemSkinSelect[i]->Show();
		}
		else {
			m_pItemSkinSelect[i]->Hide();
		}
	}	
	
	DXUTOutputDebugString("User Shop Select[%d] Index[%d]\n", m_iSelectItem, m_iSelectIndex);

	return 1;
}


int	SPWindowUserShopBuyer::OnItemClickDown(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	return 1;
}


int SPWindowUserShopBuyer::OnToolTipEnter(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	int iSelectItem = iUiID - WIID_USERSHOP_BUY_ITEM_BUTTON;	
	int iSelectIndex = m_iCurPage * MAX_USER_SHOP_LIST + iSelectItem;

	//SPPlayerInvenArchive* pInven = NULL;
	//if(g_pGOBManager->GetLocalPlayer())
	//	pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	//else 
	//	return false;

	if(iSelectIndex < 0 || iSelectIndex >= static_cast<int>(m_vpSellItem.size()))
		return 1;
	
	SPUserSellItem* pSellItem = m_vpSellItem.at(iSelectIndex);
	if(pSellItem == NULL)
		return 1;

	//if(pSellItem->m_iContainer < INVENTORY_EQUIP || pSelectItem->m_iContainer > INVENTORY_HOUSING) {
	//	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	//	pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
	//	return 0;
	//}

	//if(iSelectIndex < 0 || m_iSelectIndex >= pInven->GetInventorySize(pSelectItem->m_iContainer)) {
	//	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	//	pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
	//	return 0;
	//}

	//if(pInven->IsItemExist(pSelectItem->m_iSlotIndex, pSelectItem->m_iContainer) == false) {
	//	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	//	pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
	//	return 0;
	//}

	//SPItem* pItem = pInven->GetInventoryItem(pSelectItem->m_iContainer, pSelectItem->m_iSlotIndex);

	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage( SPIM_TOOLTIP_MOVE, (WPARAM)pSellItem->m_pItem->GetItemStatus(), lParam);
	pWindow->Show();

	return 1;
}


int SPWindowUserShopBuyer::OnToolTipMove(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	int iSelectItem = iUiID - WIID_USERSHOP_BUY_ITEM_BUTTON;	
	int iSelectIndex = m_iCurPage * MAX_USER_SHOP_LIST + iSelectItem;

	//SPPlayerInvenArchive* pInven = NULL;
	//if(g_pGOBManager->GetLocalPlayer())
	//	pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	//else 
	//	return false;

	if(iSelectIndex < 0 || iSelectIndex >= static_cast<int>(m_vpSellItem.size()))
		return 1;

	SPUserSellItem* pSellItem = m_vpSellItem.at(iSelectIndex);
	if(pSellItem == NULL)
		return 1;

	//if(pSellItem->m_iContainer < INVENTORY_EQUIP || pSelectItem->m_iContainer > INVENTORY_HOUSING) {
	//	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	//	pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
	//	return 0;
	//}

	//if(iSelectIndex < 0 || m_iSelectIndex >= pInven->GetInventorySize(pSelectItem->m_iContainer)) {
	//	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	//	pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
	//	return 0;
	//}

	//if(pInven->IsItemExist(pSelectItem->m_iSlotIndex, pSelectItem->m_iContainer) == false) {
	//	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	//	pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
	//	return 0;
	//}

	//SPItem* pItem = pInven->GetInventoryItem(pSelectItem->m_iContainer, pSelectItem->m_iSlotIndex);

	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage( SPIM_TOOLTIP_MOVE, (WPARAM)pSellItem->m_pItem->GetItemStatus(), lParam);
	pWindow->Show();
	return 1;
}


int SPWindowUserShopBuyer::OnToolTipOut(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
	pWindow->Show();
	return 1;
}

void SPWindowUserShopBuyer::SetPetShop(bool bPetShop)
{
	if (m_bPetShop != bPetShop)
	{
		m_bPetShop = bPetShop;
		SPWindowStatic	*pTitle = (SPWindowStatic*)Find( WIID_USERSHOP_BUY_TITLE, true );
		if (!pTitle)
			return;

		pTitle->SPSendMessage(SPIM_SET_IMAGECLEAR);

		if (m_bPetShop)
		{
			pTitle->SetImage("DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG", 254, 218);
		}
		else
		{
			pTitle->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 204, 313);
		}
	}
}

