#include "SPCommon.h"
#include "SPUtil.h"

#include <algorithm>
#include <functional>
#include <string>
#include <vector>
#include <map>

//#include "SPGOBStatus.h"
#include "SPGOBManager.h"
//
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGameObject.h"
#include "SPPlayer.h"
#include "SPCommandConvert.h"

#include "SPItem.h"
#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPItemCluster.h"

#include "SPCheckManager.h"

#include "SPPlayerInvenArchive.h"

#include "SPResourceDef.h"
#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"
#include "SPLocalizeManager.h"

#include <WinSock2.h>
#include "PacketID.h"
#include "Packet.h"
#include "PacketHandler.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowButton.h"
#include "SPWindowStatic.h"
#include "SPWindowToolTip.h"
#include "SPMouseCursor.h"
#include "SPIMEPool.h"
#include "SPInterfaceManager.h"

#include "SPDebug.h"


#include "SPWindowItemTimeAdd.h"



SPWindowItemTimeAdd::SPWindowItemTimeAdd(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
, m_iFrame(0)
, m_pQuestion(NULL)
, m_pDisable(NULL)
, m_pItemResult(NULL)
{
	Init();
}

SPWindowItemTimeAdd::SPWindowItemTimeAdd(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent /*= NULL*/)
: SPWindow(WNDID_ITEM_TIMEADD, InstanceID, iX, iY, iCX, iCY, pParent)	
{
	Init();
}


SPWindowItemTimeAdd::~SPWindowItemTimeAdd()
{
	Clean();
}


void SPWindowItemTimeAdd::Init()
{
	m_iTimeAddStatus = TIMEADD_STATUS_FIRST;
	
	for(int i = 0; i < MAX_TIMEADD_SLOT; i++) {
		m_pItem[i] = NULL;
		m_pIcon[i] = NULL;
		m_SelectItem[i].Clear();
	}	
	m_pCursorFirst = NULL;
	m_pCursorSecond = NULL;
	m_pToolTip = NULL;
	m_pOK = NULL;
	m_pCancel = NULL;

	m_pItemResult = new SPItem(STORAGE, 0);

	std::string strTextureFile = "";
	strTextureFile = "DATA/INTERFACE/CONCEPT/UI100.PNG";
	g_pVideo->GetTextureMgr()->LoadTexture(strTextureFile.c_str(), &m_pDisable);

	SetRect(&m_rtSrcDisable, 434 , 37 , 436 , 39);
	SetRect(&m_rtMarkSrc[0], 479, 152, 511, 184);		//ICON_MARK_LOCK
	SetRect(&m_rtMarkSrc[1], 479, 185, 511, 217);		//ICON_MARK_FASHION
	SetRect(&m_rtMarkSrc[2], 479, 339, 511, 371);		//ICON_MARK_EVENT
	SetRect(&m_rtMarkSrc[3], 479, 372, 511, 404);		//ICON_MARK_FANCY
	
	CreateSubWindow();

	m_bUse	=	SPLocalizeManager::GetInstance()->IsEnableValue( LV_EXTENDEDTERMITEM );
}


void SPWindowItemTimeAdd::Clean()
{
	for(int i = 0; i < MAX_TIMEADD_SLOT; i++) {
		m_pItem[i] = NULL;
		m_SelectItem[i].Clear();
		m_pIcon[i]->SPSendMessage(SPIM_SET_IMAGECLEAR);
	}
	
	m_pToolTip->SPSendMessage(SPIM_SET_IMAGECLEAR);

	SAFE_RELEASE(m_pDisable);

	SAFE_DELETE(m_pItemResult);
	
	SPWindow::Clean();
}


void SPWindowItemTimeAdd::CreateSubWindow()
{
	SPWindowStatic* pStatic = NULL;
	SPWindowButton* pButton = NULL;
	SPWindowStatic* pCenter = NULL;
	SPWindowStatic* pSkin = NULL;

	// Top
	pStatic = new SPWindowStatic(WIID_ITEM_TIMEADD_TOP, 0, 0, 240, 4, this);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 31);
	pStatic->Show();
	pStatic = NULL;

	// Center
	pCenter = new SPWindowStatic(WIID_ITEM_TIMEADD_CENTER, 0, 4, 240, 117, this);
	pCenter->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 37);
	pCenter->SetSrcSize(240, 2);
	pCenter->Show();

	// bottom
	pStatic = new SPWindowStatic(WIID_ITEM_TIMEADD_BOTTOM, 0, 121, 240, 4, this);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 41);
	pStatic->Show();
	pStatic = NULL;

	// WIID_ITEM_TIMEADD_TITLE
	pStatic = new SPWindowStatic(WIID_ITEM_TIMEADD_TITLE, 10, 6, 200, 15, this);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 1, 408);
	pStatic->Show();
	pStatic = NULL;

	// WIID_ITEM_TIMEADD_CLOSE
	pButton = new SPWindowButton(WIID_ITEM_TIMEADD_CLOSE, 219, 9 - 4, 12, 12, pCenter);
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI103.PNG",	301,	321);
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI103.PNG",	314,	321); //on
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI103.PNG",	327,	321); //push
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG",	340,	321); //disable
	pButton->Show();
	pButton = NULL;	

    // WIID_ITEM_TIMEADD_LINE1
	pStatic = new SPWindowStatic(WIID_ITEM_TIMEADD_LINE1, 2, 26 - 4, 236, 1, pCenter);
	pStatic->SetSrcSize(6, 1);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 493, 494);
	pStatic->Show();
	pStatic = NULL;

	// WIID_ITEM_TIMEADD_LINE2
	pStatic = new SPWindowStatic(WIID_ITEM_TIMEADD_LINE2, 2, 97 - 4, 236, 1, pCenter);
	pStatic->SetSrcSize(6, 1);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 493, 494);
	pStatic->Show();
	pStatic = NULL;

	// WIID_ITEM_TIMEADD_SKIN
	pSkin = new SPWindowStatic(WIID_ITEM_TIMEADD_SKIN, 2, 27 - 4, 236, 70, pCenter);
	pSkin->SetSrcSize(6, 6);
	pSkin->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 493, 477);
	pSkin->Show();

	//WIID_ITEM_TIMEADD_QUESTION
	pStatic = new SPWindowStatic(WIID_ITEM_TIMEADD_QUESTION, 182 - 2, 44 - 27, 12, 18, pSkin);	
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 360, 321);
	pStatic->Show();
	pStatic = NULL;
	
	int iStartX[MAX_TIMEADD_SLOT] = {36, 104, 172};
	for(int i = 0; i < MAX_TIMEADD_SLOT; i++) {
		pButton = new SPWindowButton(WIID_ITEM_TIMEADD_SLOTBUTTON + i, iStartX[i] - 2, 37 - 27, 32, 32, pSkin);	
		pButton->Show();
		pButton = NULL;

		pStatic = new SPWindowStatic(WIID_ITEM_TIMEADD_SLOTLINE + i, iStartX[i] - 2, 37 - 27, 32, 32, pSkin);	
		pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 203, 408);
		pStatic->Show();
		pStatic = NULL;	

		pStatic = new SPWindowStatic(WIID_ITEM_TIMEADD_SLOTICON + i, iStartX[i] - 2, 37 - 27, 32, 32, pSkin);	
		pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 203, 408);
		pStatic->Show();
		m_pIcon[i] = pStatic;
		m_pIcon[i]->GetWindowRect(m_rtStack[i]);
		pStatic = NULL;	

		m_bItemAble[i] = true;
		m_iItemMark[i] = ICON_MARK_NULL;
		m_iStack[i] = 0;
	}

	//WIID_ITEM_TIMEADD_CURSOR1
	pStatic = new SPWindowStatic(WIID_ITEM_TIMEADD_CURSOR1, 47 - 2, 67 - 27, 10, 10, pSkin);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 354, 261);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 365, 261);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 376, 261);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 387, 261);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 398, 261);
	pStatic->Show();
	m_pCursorFirst = pStatic;
	pStatic = NULL;
	
	//WIID_ITEM_TIMEADD_CURSOR2
	pStatic = new SPWindowStatic(WIID_ITEM_TIMEADD_CURSOR2, 115 - 2, 67 - 27, 10, 10, pSkin);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 354, 261);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 365, 261);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 376, 261);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 387, 261);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 398, 261);
	pStatic->Show();
	m_pCursorSecond = pStatic;
	pStatic = NULL;

	//WIID_ITEM_TIMEADD_TOOLTIP
	pStatic = new SPWindowStatic(WIID_ITEM_TIMEADD_TOOLTIP, 16 - 2, 79 - 27, 208, 15, pSkin);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 1, 61);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 1, 77);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 1, 93);
	pStatic->Show();
	m_pToolTip = pStatic;
	pStatic = NULL;

	//WIID_ITEM_TIMEADD_PLUS
	pStatic = new SPWindowStatic(WIID_ITEM_TIMEADD_PLUS, 78 - 2, 44 - 27, 16, 16, pSkin);	
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 239, 423);
	pStatic->Show();
	pStatic = NULL;		

	//WIID_ITEM_TIMEADD_EQUAL
	pStatic = new SPWindowStatic(WIID_ITEM_TIMEADD_EQUAL, 146 - 2, 45 - 27, 16, 14, pSkin);	
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 239, 408);
	pStatic->Show();
	pStatic = NULL;

	//WIID_ITEM_TIMEADD_OK	
	m_pOK = new SPWindowButton(WIID_ITEM_TIMEADD_OK, 90, 102 - 4, 68, 17, pCenter);
	m_pOK->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG",			346,	440);
	m_pOK->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG",		346,	458);	//on
	m_pOK->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG",		346,	476);	//push
	m_pOK->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG",	346,	494); //disable
	//m_pPrev->SetImageCheck("DATA/INTERFACE/CONCEPT/UI010.PNG", 52, 228);	//check
	m_pOK->Show();

	//WIID_ITEM_TIMEADD_CANCEL
	m_pCancel = new SPWindowButton(WIID_ITEM_TIMEADD_CANCEL, 162, 102 - 4, 68, 17, pCenter);
	m_pCancel->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG",			415,	440);
	m_pCancel->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG",		415,	458);	//on
	m_pCancel->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG",		415,	476);	//push
	m_pCancel->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG",	415,	494); //disable
	//m_pPrev->SetImageCheck("DATA/INTERFACE/CONCEPT/UI010.PNG", 52, 228);	//check
	m_pCancel->Show();

}


void SPWindowItemTimeAdd::Process(float fTime)
{
	SPWindow::Process(fTime);	
	ProcessCursor(fTime);
}


void SPWindowItemTimeAdd::ProcessCursor(float fTime)
{
	m_fCursorAccmulateTime += fTime;
	if( m_fCursorAccmulateTime > 0.05f )
	{
		if( m_iCursorDirection )
		{
			m_iFrame--;
			if( m_iFrame < 0 )
			{
				m_iFrame = 1;
				m_iCursorDirection = 0;
			}
		}
		else
		{
			m_iFrame++;
			if( m_iFrame > 4 )
			{
				m_iFrame = 3;
				m_iCursorDirection = 1;
			}
		}
		
		if(m_iTimeAddStatus == TIMEADD_STATUS_FIRST) {
			m_pCursorFirst->SPSendMessage(SPIM_SET_IMAGEINDEX, m_iFrame);
			m_pCursorSecond->Hide();
		}
		else if(m_iTimeAddStatus == TIMEADD_STATUS_SECOND) {
			m_pCursorFirst->Hide();
			m_pCursorSecond->SPSendMessage(SPIM_SET_IMAGEINDEX, m_iFrame);
		}
		else {
			m_pCursorFirst->Hide();
			m_pCursorSecond->Hide();
		}

		m_fCursorAccmulateTime = 0.0f;
	}
}


void SPWindowItemTimeAdd::Render(float fTime)
{
	SPWindow::Render(fTime);

	for(int i = 0; i < MAX_TIMEADD_SLOT; i++) {
		//Icon Disable Ç¥Çö
		if(m_pDisable) {
			RECT rtDest;
			m_pIcon[i]->GetWindowRect(rtDest);
			if(m_bItemAble[i] == false) {		
				m_pDisable->RenderTexture(&rtDest, &m_rtSrcDisable);
			}
			if(m_iItemMark[i] > ICON_MARK_NULL) {
				m_pDisable->RenderTexture(&rtDest, &m_rtMarkSrc[m_iItemMark[i] - 1]);
			}			
		}

		//Stack Count Local Ç¥Çö
		if(m_iStack[i] > 9) {
			RECT rtTenNumTarget, rtNumTarget, rtTenNumSrc, rtNumSrc;
			int iTenStack = m_iStack[i] / 10;
			int iOneStack = m_iStack[i] % 10;
			m_pIcon[i]->GetWindowRect(m_rtStack[i]);
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
			m_pIcon[i]->GetWindowRect(m_rtStack[i]);
			SetRect(&rtNumTarget, m_rtStack[i].left, m_rtStack[i].bottom - 10, m_rtStack[i].left + 9, m_rtStack[i].bottom);

			rtNumSrc.left = iOneStack * 9;
			rtNumSrc.right = rtNumSrc.left + 9;
			rtNumSrc.top = 0;
			rtNumSrc.bottom = 10;

			g_pItemCluster->GetNumTexture()->RenderTexture( &rtNumTarget , &rtNumSrc);
		}
	}
}


void SPWindowItemTimeAdd::Show(bool bWithChild /*= true*/)
{
	if( m_bUse == false )
		return;

	for(int i = 0; i < MAX_TIMEADD_SLOT; i++) {
		m_pItem[i] = NULL;
		m_pIcon[i]->SPSendMessage(SPIM_SET_IMAGECLEAR);
	}	
	
	ClearAllSlotInterface();	
	m_pItemResult->SetClear();
	SetStatus(TIMEADD_STATUS_FIRST);	

	SPWindow::Show(bWithChild);
}


void SPWindowItemTimeAdd::Hide(bool bSendServer /*= true*/)
{	
	if(m_bShow) {
		for(int i = 0; i < MAX_TIMEADD_SLOT; i++) {
			m_pItem[i] = NULL;
			m_pIcon[i]->SPSendMessage(SPIM_SET_IMAGECLEAR);
		}

		ClearAllSlotInterface();		
	}
	SPWindow::Hide(bSendServer);
}


void SPWindowItemTimeAdd::SetStatus(TIMEADD_STATUS iStatus)
{	
	m_iTimeAddStatus = iStatus;
	
	switch(m_iTimeAddStatus) {
		case TIMEADD_STATUS_FIRST:
			m_pCursorFirst->Show();
			m_pCursorSecond->Hide();
			m_pToolTip->SPSendMessage(SPIM_SET_IMAGEINDEX, 0);
			m_pOK->SetEnable(false);
			break;
		case TIMEADD_STATUS_SECOND:
			m_pCursorFirst->Hide();
			m_pCursorSecond->Show();
			m_pToolTip->SPSendMessage(SPIM_SET_IMAGEINDEX, 1);
			m_pOK->SetEnable(false);
			break;
		case TIMEADD_STATUS_READY:
			m_pOK->SetEnable(true);
			m_pToolTip->SPSendMessage(SPIM_SET_IMAGEINDEX, 2);
			break;
		case TIMEADD_STATUS_RESULT:
			m_pCursorFirst->Hide();
			m_pCursorSecond->Hide();			
			m_pOK->SetEnable(false);
			break;
		default:
			m_pCursorFirst->Hide();
			m_pCursorSecond->Hide();
			m_pToolTip->SPSendMessage(SPIM_SET_IMAGECLEAR);
			m_pOK->SetEnable(false);
			break;
	}

	m_iFrame = 0;
	m_fCursorAccmulateTime = 0.0f;
	m_iCursorDirection = 0;	
}


void SPWindowItemTimeAdd::ClearAllSlotInterface()
{
	for(int i = 0; i < MAX_TIMEADD_SLOT; i++) {
		ClearSlotInterface(i);
	}
}


void SPWindowItemTimeAdd::ClearSlotInterface(int iIndex)
{
	m_pIcon[iIndex]->SPSendMessage(SPIM_SET_IMAGECLEAR);		
	m_bItemAble[iIndex] = true;
	m_iItemMark[iIndex] = ICON_MARK_NULL;
	m_iStack[iIndex] = 0;
	m_pItem[iIndex] = NULL;
	m_SelectItem[iIndex].Clear();	
	
	if(m_bShow) {
		//
		SPPlayerInvenArchive* pInven = NULL;
		if(g_pGOBManager->GetLocalPlayer()) {
			pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
			pInven->ClearBlockItem(iIndex);
		}
	}	
}


void SPWindowItemTimeAdd::SetSlotInterface(int iIndex, SPItem* pItem)
{
	POINT ptSrcPos;
	
	SPTexture* pTexture = g_pItemCluster->GetItemTexture(pItem->GetItemStatus()->GetItemID(), ptSrcPos);

	m_pIcon[iIndex]->Hide();
	m_pIcon[iIndex]->SetImage(pTexture, ptSrcPos.x, ptSrcPos.y);
	m_pIcon[iIndex]->Show();
	
	m_iStack[iIndex] = pItem->GetItemStatusValue(MV_ITEM_GETSTACK_CNT);
	m_bItemAble[iIndex] = g_pCheckManager->CheckItemStatus(pItem->GetItemStatus()->GetItemID());
	m_bItemAble[iIndex] &= pItem->GetItemStatus()->IsTimeAvailable();
	m_iItemMark[iIndex] = g_pItemCluster->GetItemIconMark(pItem->GetItemStatus()->GetItemID());	
}


bool SPWindowItemTimeAdd::SetFirstItem()
{	
	SPPlayerInvenArchive* pInven = NULL;
	if(g_pGOBManager->GetLocalPlayer())
		pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	else 
		return false;

	//»ç¸Á½Ã Á¦ÇÑ
	//if(g_pGOBManager->GetLocalPlayer()->GetCoordPhysics()->GetCurState() == GOB_STATE_DEAD) {
	//	//ResetItemControl();
	//	return false;
	//}

	//ÇÈ¾÷ »óÅÂ°¡ ¾Æ´Ñ°æ¿ì ³Ñ¾î°£´Ù
	if(pInven->m_bPickup == false) {
		//ResetItemControl();
		return false;
	}

	if(m_iTimeAddStatus != TIMEADD_STATUS_FIRST) {
		return false;
	}

	m_SelectItem[0] = pInven->GetPickupItem();
	m_pItem[0] = pInven->GetInventoryItem(m_SelectItem[0].m_iContainer, m_SelectItem[0].m_iSlotIndex);
	
	// ÀÎº¥Åä¸®¿¡¼­ ¿Â ¾ÆÀÌÅÛÀÎÁö
	if( m_SelectItem[0].GetContainerType() >= INVENTORY_HOUSING ) {
		//DXUTOutputDebugString("ItemSell Equip, Costume... Inven.. \n")	;
		if(g_pGOBManager->GetLocalPlayer())
			g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
		
		if(g_pResourceManager->GetGlobalString(13000002)){
			std::string strMsg = g_pResourceManager->GetGlobalString(13000002);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}		
		return false;
	}

	if(m_pItem[0] == NULL || m_pItem[0]->GetItemStatus() == NULL || m_pItem[0]->GetItemStatus()->GetItemID() == 0) {
		return false;
	}

	// ±â°£ Á¦ÇÑ ¾ÆÀÌÅÛÀÎÁö È®ÀÎ
	if(m_pItem[0]->GetItemStatus()->GetItemAttr()->GetDurationType() == ITEM_DURATION_NULL) {
		if(g_pResourceManager->GetGlobalString(13000001)){
			std::string strMsg = g_pResourceManager->GetGlobalString(13000001);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}
		return false;
	}
	
	// ½Ã°£Ãß°¡ ¾ÆÀÌÅÛÀÎÁö È®ÀÎ
	if(g_pItemCluster->IsItemTimeAdd(m_pItem[0]->GetItemStatus()->GetItemID()) == false) {
		if(g_pResourceManager->GetGlobalString(13000003)){
			std::string strMsg = g_pResourceManager->GetGlobalString(13000003);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}
		return false;
	}
	
	// ½Ã°£Ãß°¡ ¾ÆÀÌÅÛÀÇ ¸®Äâ Ã¼Å©
	if(CheckTimeAddItem(m_pItem[0]) == false) {
		SPItemTimeAdd* pItemAdd = g_pItemCluster->GetItemTimeAdd(m_pItem[0]->GetItemStatus()->GetItemID());
		if(pItemAdd && !pItemAdd->m_strErr.empty())	{			
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)pItemAdd->m_strErr.c_str() );
		}		
		return false;
	}

	// ÀÎÅÍÆäÀÌ½º ¼³Á¤
	SetSlotInterface(0, m_pItem[0]);
	//pInven->SetUserShopItemFake(0, m_SelectItem[0].GetContainerType(), m_SelectItem[0].GetSlotIndex());
	pInven->SetBlockItem(0, m_SelectItem[0].GetContainerType(), m_SelectItem[0].GetSlotIndex());

	//´ÙÀ½´Ü°è·Î
	SetStatus(TIMEADD_STATUS_SECOND);

	return true;
}


bool SPWindowItemTimeAdd::SetSecondItem()
{
	SPPlayerInvenArchive* pInven = NULL;
	if(g_pGOBManager->GetLocalPlayer())
		pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	else 
		return false;

	//»ç¸Á½Ã Á¦ÇÑ
	//if(g_pGOBManager->GetLocalPlayer()->GetCoordPhysics()->GetCurState() == GOB_STATE_DEAD) {
	//	//ResetItemControl();
	//	return false;
	//}

	//ÇÈ¾÷ »óÅÂ°¡ ¾Æ´Ñ°æ¿ì ³Ñ¾î°£´Ù
	if(pInven->m_bPickup == false) {
		//ResetItemControl();
		return false;
	}

	if(m_iTimeAddStatus != TIMEADD_STATUS_SECOND) {
		return false;
	}

	//Ã¹¹øÂ° ½½·Ô¿¡ Á¤º¸°¡ ÀÖ´ÂÁö È®ÀÎ
	if(m_pItem[0] == NULL || m_pItem[0]->GetItemStatus() == NULL || m_pItem[0]->GetItemStatus()->GetItemID() == 0)
		return false;

	m_SelectItem[1] = pInven->GetPickupItem();
	m_pItem[1] = pInven->GetInventoryItem(m_SelectItem[1].m_iContainer, m_SelectItem[1].m_iSlotIndex);
	
	// ÀÎº¥Åä¸®¿¡¼­ ¿Â ¾ÆÀÌÅÛÀÎÁö
	if( m_SelectItem[1].GetContainerType() >= INVENTORY_HOUSING ) {
		//DXUTOutputDebugString("ItemSell Equip, Costume... Inven.. \n")	;
		if(g_pGOBManager->GetLocalPlayer())
			g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);

		if(g_pResourceManager->GetGlobalString(13000101)){
			std::string strMsg = g_pResourceManager->GetGlobalString(13000101);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}
		return false;
	}

	if(m_pItem[1] == NULL || m_pItem[1]->GetItemStatus() == NULL || m_pItem[1]->GetItemStatus()->GetItemID() == 0) {
		return false;
	}

	//Ã¹¹øÂ° ¾ÆÀÌÅÛ¿¡ °ü·ÃÇÑ ½Ã°£¿¬Àå ¾ÆÀÌÅÛÀÎÁö È®ÀÎÇÑ´Ù
	SPItemTimeAdd* pTimeAdd = g_pItemCluster->GetItemTimeAdd(m_pItem[0]->GetItemStatus()->GetItemID());
	if(pTimeAdd == NULL)
		return false;

	//ÇØ´ç Àç·á°¡ ¸Â´ÂÁö È®ÀÎ
	if(pTimeAdd->m_iMaterialID != m_pItem[1]->GetItemStatus()->GetItemID()) {
		if(g_pResourceManager->GetGlobalString(13000102)){
			std::string strMsg = g_pResourceManager->GetGlobalString(13000102);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}
		return false;
	}

	//ÇÊ¿ä·Î ÇÏ´Â °¹¼öÀÌ»óÀÎÁö È®ÀÎ
	if(pTimeAdd->m_iMaterialNum > m_pItem[1]->GetItemStatus()->GetStackCount()) {
		if(g_pResourceManager->GetGlobalString(13000103)){
			std::string strMsg = g_pResourceManager->GetGlobalString(13000103);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}
		return false;
	}

	// ÀÎÅÍÆäÀÌ½º ¼³Á¤
	SetSlotInterface(1, m_pItem[1]);						//ÉèÖÃµÚ¶þ¸ñµÄÍ¼Æ¬
	m_iStack[1] = pTimeAdd->m_iMaterialNum;					//´¢´æÐèÒªµÄ²ÄÁÏÊýÄ¿
	//pInven->SetUserShopItemFake(1, m_SelectItem[1].GetContainerType(), m_SelectItem[1].GetSlotIndex());
	pInven->SetBlockItem(1, m_SelectItem[1].GetContainerType(), m_SelectItem[1].GetSlotIndex());
	
	// °á°ú ÀÎÅÍÆäÀÌ½º ¼³Á¤
	CONTAINER_ITEM item;
	m_pItem[0]->ConvertContainerItem(&item);
	m_pItemResult->SetItemStatus(&item);
	int iRemainTime = m_pItem[0]->GetItemStatus()->GetLifeTime();
	m_pItemResult->GetItemStatus()->SetLifeTime((pTimeAdd->m_iResult * 60 * 60) + iRemainTime);
	SetSlotInterface(2, m_pItemResult);

	
	//´ÙÀ½´Ü°è·Î
	SetStatus(TIMEADD_STATUS_READY);

	return true;
}


bool SPWindowItemTimeAdd::CheckTimeAddItem(SPItem* pItem)
{
	if(pItem == NULL || pItem->GetItemStatus() == NULL || pItem->GetItemStatus()->GetItemID() == 0)
		return false;

	if(g_pItemCluster->IsItemTimeAdd(pItem->GetItemStatus()->GetItemID()) == false) 
		return false;

	SPItemTimeAdd* pTimeAdd = g_pItemCluster->GetItemTimeAdd(pItem->GetItemStatus()->GetItemID());
	if(pTimeAdd == NULL)
		return false;

	for( int i = 0 ; i < MAX_ITEM_OPTION ; i++ )
	{
		if( g_pCheckManager->CheckCondition( pTimeAdd->m_Require[i].m_iType , pTimeAdd->m_Require[i].m_iID , pTimeAdd->m_Require[i].m_iValue1 , pTimeAdd->m_Require[i].m_iValue2 ) == false )
			return false;
	}

	return true;
}


bool SPWindowItemTimeAdd::SendAddTime()
{
	//ITEM_CS_ADDTIME,
	// int				iItemAddTimeID;				// ITEM_ADD_TIME.LDT::PrimaryKey
	//----------------------------------------------------------
	// CONTAINER_TYPE	iMainContainerType;			// ¸ÞÀÎ
	// UINT8			iMainContainerSlot;			// ¸ÞÀÎ
	// ITEMNO			iMainItemNo;				// ¸ÞÀÎ

	// CONTAINER_TYPE	iMaterialContainerType;		// Àç·á ¾ÆÀÌÅÛ À§Ä¡  
	// UINT8			iMaterialContainerSlot;		// Àç·á ¾ÆÀÌÅÛ À§Ä¡
	// ITEMNO			iMaterialItemNo;			// Àç·á ¾ÆÀÌÅÛ ¹øÈ£
	// UINT8			iMaterialItemCount;			// Àç·á ¾ÆÀÌÅÛ ¿ä±¸°¹¼ö.  -- ÂüÁ¶¿ë
	//-------------------------------------------------------------

	if(m_pItem[0] == NULL || m_pItem[0]->GetItemStatus() == NULL || m_pItem[0]->GetItemStatus()->GetItemID() == 0) {
		return false;
	}

	if(m_pItem[1] == NULL || m_pItem[1]->GetItemStatus() == NULL || m_pItem[1]->GetItemStatus()->GetItemID() == 0) {
		return false;
	}

	CPacket Packet;
	Packet.Add((UINT32)ITEM_CS_ADDTIME);
	//UINT8 byType = 1;
	//Packet.AddUINT8(byType);
	Packet.AddUINT32(m_pItem[0]->GetItemStatus()->GetItemID());
	Packet.AddUINT32(m_SelectItem[0].GetContainerType());
	Packet.AddUINT8(m_SelectItem[0].GetSlotIndex() + 1);
	Packet.AddUINT64(*(ITEMNO*)m_pItem[0]->GetItemStatus()->SPSendMessage( MV_ITEM_GET_ITEMNO ));

	Packet.AddUINT32(m_SelectItem[1].GetContainerType());
	Packet.AddUINT8(m_SelectItem[1].GetSlotIndex() + 1);
	Packet.AddUINT64(*(ITEMNO*)m_pItem[1]->GetItemStatus()->SPSendMessage( MV_ITEM_GET_ITEMNO ));
	Packet.AddUINT8(m_pItem[1]->GetItemStatus()->GetStackCount());	
	
	g_pNetworkManager->SPPerformMessage(ITEM_CS_ADDTIME, 0, (LPARAM)&Packet);
	return true;
}




SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowItemTimeAdd )
SPIMESSAGE_COMMAND(SPIM_ITEM_TIMEADD_RESULT	,	OnResult	)

SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_ITEM_TIMEADD_TOP		, OnLButtonUp)		//¾ÆÀÌÅÛ¾ÆÀÌÄÜ ¸®Ä¿¹ö
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_ITEM_TIMEADD_CENTER	, OnLButtonUp)		//¾ÆÀÌÅÛ¾ÆÀÌÄÜ ¸®Ä¿¹ö
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_ITEM_TIMEADD_BOTTOM	, OnLButtonUp)		//¾ÆÀÌÅÛ¾ÆÀÌÄÜ ¸®Ä¿¹ö
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_ITEM_TIMEADD_TITLE		, OnLButtonUp)		//¾ÆÀÌÅÛ¾ÆÀÌÄÜ ¸®Ä¿¹ö
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_ITEM_TIMEADD_SKIN		, OnLButtonUp)		//¾ÆÀÌÅÛ¾ÆÀÌÄÜ ¸®Ä¿¹ö
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_ITEM_TIMEADD_LINE1		, OnLButtonUp)		//¾ÆÀÌÅÛ¾ÆÀÌÄÜ ¸®Ä¿¹ö
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_ITEM_TIMEADD_LINE2		, OnLButtonUp)		//¾ÆÀÌÅÛ¾ÆÀÌÄÜ ¸®Ä¿¹ö
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_ITEM_TIMEADD_CURSOR1	, OnLButtonUp)		//¾ÆÀÌÅÛ¾ÆÀÌÄÜ ¸®Ä¿¹ö
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_ITEM_TIMEADD_CURSOR2	, OnLButtonUp)		//¾ÆÀÌÅÛ¾ÆÀÌÄÜ ¸®Ä¿¹ö
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_ITEM_TIMEADD_TOOLTIP	, OnLButtonUp)		//¾ÆÀÌÅÛ¾ÆÀÌÄÜ ¸®Ä¿¹ö
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_ITEM_TIMEADD_PLUS		, OnLButtonUp)		//¾ÆÀÌÅÛ¾ÆÀÌÄÜ ¸®Ä¿¹ö
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_ITEM_TIMEADD_EQUAL		, OnLButtonUp)		//¾ÆÀÌÅÛ¾ÆÀÌÄÜ ¸®Ä¿¹ö

SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP	,WIID_ITEM_TIMEADD_CLOSE	,		OnClose		)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP	,WIID_ITEM_TIMEADD_OK	,			OnOK		)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP	,WIID_ITEM_TIMEADD_CANCEL	,		OnCancel	)

SPIMESSAGE_CONTROL_RANGE(SPIM_LBUTTON_UP,	WIID_ITEM_TIMEADD_SLOTBUTTON, WIID_ITEM_TIMEADD_SLOTBUTTON + MAX_TIMEADD_SLOT, OnItemClick)
SPIMESSAGE_CONTROL_RANGE(SPIM_TOOLTIP_ENTER,	WIID_ITEM_TIMEADD_SLOTBUTTON,	WIID_ITEM_TIMEADD_SLOTBUTTON + MAX_TIMEADD_SLOT,	OnToolTipEnter)
SPIMESSAGE_CONTROL_RANGE(SPIM_TOOLTIP_OUT,		WIID_ITEM_TIMEADD_SLOTBUTTON,	WIID_ITEM_TIMEADD_SLOTBUTTON + MAX_TIMEADD_SLOT,	OnToolTipOut)
SPIMESSAGE_CONTROL_RANGE(SPIM_TOOLTIP_MOVE,	WIID_ITEM_TIMEADD_SLOTBUTTON,		WIID_ITEM_TIMEADD_SLOTBUTTON + MAX_TIMEADD_SLOT,	OnToolTipMove)
SPIMESSAGE_OBJ_MAP_END	( SPWindow	)


int SPWindowItemTimeAdd::OnResult(WPARAM wParam, LPARAM lParam)
{
	bool bResult = (bool)wParam;
	SPPlayerInvenArchive* pInven = NULL;
	
	if(bResult) {
		SetSlotInterface(0, m_pItem[0]);		
		ClearSlotInterface(1);		
		ClearSlotInterface(2);
		//if(g_pGOBManager->GetLocalPlayer()) {
		//	pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
		//	pInven->ClearUserShopItem(1);
		//	pInven->ClearUserShopItem(2);
		//}
		SetStatus(TIMEADD_STATUS_SECOND);
	}
	else {
		ClearSlotInterface(0);
		ClearSlotInterface(1);
		ClearSlotInterface(2);
		//if(g_pGOBManager->GetLocalPlayer()) {
		//	pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
		//	pInven->ClearUserShopItem(0);
		//	pInven->ClearUserShopItem(1);
		//	pInven->ClearUserShopItem(2);
		//}
		SetStatus(TIMEADD_STATUS_FIRST);
	}

	m_pItemResult->SetClear();
	return 1;
}


int SPWindowItemTimeAdd::OnClose(WPARAM wParam, LPARAM lParam)
{
	if(g_pGOBManager->GetLocalPlayer())
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);	

	////
	//SPPlayerInvenArchive* pInven = NULL;
	//if(g_pGOBManager->GetLocalPlayer()) {
	//	pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	//	pInven->ClearUserShopItem(0);
	//	pInven->ClearUserShopItem(1);
	//	pInven->ClearUserShopItem(2);
	//}
	
	Hide();
	return 1;
}


int SPWindowItemTimeAdd::OnOK(WPARAM wParam, LPARAM lParam)
{
	if(g_pGOBManager->GetLocalPlayer())
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
	
	if(m_iTimeAddStatus != TIMEADD_STATUS_READY) 
		return 1;
	
	if(SendAddTime() == false) 
	{
	}
	return 1;
}


int SPWindowItemTimeAdd::OnCancel(WPARAM wParam, LPARAM lParam)
{
	/*
	if(g_pGOBManager->GetLocalPlayer()) 
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
	
	SPPlayerInvenArchive* pInven = NULL;
	if(g_pGOBManager->GetLocalPlayer()) {
		pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
		pInven->ClearUserShopItem(0);
		pInven->ClearUserShopItem(1);
		pInven->ClearUserShopItem(2);
	}

	Hide();
	*/

	OnClose(wParam, lParam);
	return 1;
}


int SPWindowItemTimeAdd::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{
	if(g_pGOBManager->GetLocalPlayer())
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
	return 1;
}


int SPWindowItemTimeAdd::OnItemClick(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	int iSelectIndex = iUiID - WIID_ITEM_TIMEADD_SLOTBUTTON;	

	if(iSelectIndex == 0) {
		//ÀÌ¹Ì ¾ÆÀÌÅÛÀÌ ÀÖ´Â °æ¿ì
		if(m_pItem[0] && m_pItem[0]->GetItemStatus() && m_pItem[0]->GetItemStatus()->GetItemID()) {
			ClearSlotInterface(0);
			ClearSlotInterface(1);
			ClearSlotInterface(2);
			SetStatus(TIMEADD_STATUS_FIRST);
			
			////
			//SPPlayerInvenArchive* pInven = NULL;
			//if(g_pGOBManager->GetLocalPlayer()) {
			//	pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
			//	pInven->ClearUserShopItem(0);
			//	pInven->ClearUserShopItem(1);
			//	pInven->ClearUserShopItem(2);
			//	g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
			//}			
			return 1;
		}

		if(SetFirstItem() == false) {
			g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
			return 1;
		}
	}
	else if(iSelectIndex == 1) {
		//ÀÌ¹Ì ¾ÆÀÌÅÛÀÌ ÀÖ´Â °æ¿ì
		if(m_pItem[1] && m_pItem[1]->GetItemStatus() && m_pItem[1]->GetItemStatus()->GetItemID()) {
			ClearSlotInterface(1);
			ClearSlotInterface(2);
			SetStatus(TIMEADD_STATUS_SECOND);
			////
			//SPPlayerInvenArchive* pInven = NULL;
			//if(g_pGOBManager->GetLocalPlayer()) {
			//	pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();				
			//	pInven->ClearUserShopItem(1);
			//	pInven->ClearUserShopItem(2);
			//	g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
			//}			
			return 1;
		}

		if(SetSecondItem() == false) {
			g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
			return 1;
		}
	}
	else {
		if(g_pGOBManager->GetLocalPlayer())
			g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
	}

	return 1;
}


int SPWindowItemTimeAdd::OnToolTipMove(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	int iSelectIndex = iUiID - WIID_ITEM_TIMEADD_SLOTBUTTON;	
	SPWindow* pWindow = NULL;
	//SPItem* pItem = pInven->GetInventoryItem(pSelectItem->m_iContainer, pSelectItem->m_iSlotIndex);
	if(iSelectIndex == 2) {
		if(m_pItemResult && m_pItemResult->GetItemStatus() && m_pItemResult->GetItemStatus()->GetItemID()) {
			pWindow = g_pInterfaceManager->GetToolTipWindow();
			pWindow->SPSendMessage( SPIM_TOOLTIP_MOVE, (WPARAM)m_pItemResult->GetItemStatus(), lParam);
		}
	}
	else {
		if(m_pItem[iSelectIndex] && m_pItem[iSelectIndex]->GetItemStatus() && m_pItem[iSelectIndex]->GetItemStatus()->GetItemID()) {
			pWindow = g_pInterfaceManager->GetToolTipWindow();
			pWindow->SPSendMessage( SPIM_TOOLTIP_MOVE, (WPARAM)m_pItem[iSelectIndex]->GetItemStatus(), lParam);
		}
	}
	
	if(pWindow)
		pWindow->Show();

	return 1;
}


int SPWindowItemTimeAdd::OnToolTipEnter(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	int iSelectIndex = iUiID - WIID_ITEM_TIMEADD_SLOTBUTTON;	
	SPWindow* pWindow = NULL;
	//SPItem* pItem = pInven->GetInventoryItem(pSelectItem->m_iContainer, pSelectItem->m_iSlotIndex);
	if(iSelectIndex == 2) {
		if(m_pItemResult && m_pItemResult->GetItemStatus() && m_pItemResult->GetItemStatus()->GetItemID()) {
			pWindow = g_pInterfaceManager->GetToolTipWindow();
			pWindow->SPSendMessage( SPIM_TOOLTIP_MOVE, (WPARAM)m_pItemResult->GetItemStatus(), lParam);
		}
	}
	else {
		if(m_pItem[iSelectIndex] && m_pItem[iSelectIndex]->GetItemStatus() && m_pItem[iSelectIndex]->GetItemStatus()->GetItemID()) {
			pWindow = g_pInterfaceManager->GetToolTipWindow();
			pWindow->SPSendMessage( SPIM_TOOLTIP_MOVE, (WPARAM)m_pItem[iSelectIndex]->GetItemStatus(), lParam);
		}
	}

	if(pWindow)
		pWindow->Show();

	return 1;
}


int SPWindowItemTimeAdd::OnToolTipOut(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
	pWindow->Show();
	return 1;
}

