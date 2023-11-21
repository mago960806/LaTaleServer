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

#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPWindowCashHotList.h"

////
//#include "SPStage.h"
//#include "SPStageManager.h"
//#include "SPTerrainAttribute.h"
//
//#include "SPCommandConvert.h"
//#include "SPGOBClusterDef.h"
//#include "SPAvatarModelUnitDef.h"
//#include "SPGOBCoordPhysics.h"
//#include "SPPlayerCoordPhysics.h"
//#include "SPPlayerInvenArchive.h"
//
//#include "SPGOBStatus.h"
//#include "SPPlayerStatus.h"
//#include "SPPlayerStatusModel.h"

#include "Packet.h"
#include "PacketID.h"
#include "SPNetworkManager.h"

#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPItem.h"
#include "SPItemCluster.h"			//[2005/6/16] - jinhee
//#include "SPPlayer.h"

#include "SPHotItemInfo.h"
#include "SPHotItemManager.h"
#include "SPCashTradeManager.h"

#include "SPCheckManager.h"
#include "SPLocalizeManager.h"

#include "SPDebug.h"



SPWindowCashHotList::SPWindowCashHotList( INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent /*= NULL*/)
: SPWindow(WNDID_CASH_HOTLIST, InstanceID, iX, iY, iCX, iCY, pParent)
{
	Init();
}


SPWindowCashHotList::~SPWindowCashHotList()
{
	Clean();
}


void SPWindowCashHotList::Init()
{
	for(int i = 0; i < MAX_HOTITEM_VIEW ; i++) {		
		m_bItemAble[i] = true;
		m_iItemMark[i] = ICON_MARK_NULL;		
		m_iStack[i] = 0;
	}
	
	std::string strTextureFile = "";
	strTextureFile.clear();
	strTextureFile = "DATA/INTERFACE/CONCEPT/UI100.PNG";
	g_pVideo->GetTextureMgr()->LoadTexture(strTextureFile.c_str(), &m_pDisable);

	SetRect(&m_rtSrcDisable, 434 , 37 , 436 , 39);
	SetRect(&m_rtMarkSrc[0], 479, 152, 511, 184);		//ICON_MARK_LOCK
	SetRect(&m_rtMarkSrc[1], 479, 185, 511, 217);		//ICON_MARK_FASHION
	SetRect(&m_rtMarkSrc[2], 479, 339, 511, 371);		//ICON_MARK_EVENT
	SetRect(&m_rtMarkSrc[3], 479, 372, 511, 404);		//ICON_MARK_FANCY

	strTextureFile.clear();
	strTextureFile = "DATA/INTERFACE/CONCEPT/UI_WinBG_000.PNG";
	g_pVideo->GetTextureMgr()->LoadTexture(strTextureFile.c_str(), &m_pBaseSkin);

	SetRect(&m_rtBaseSrc[WINDOW_BASE_TOP_LEFT],			452, 452, 456, 456);
	SetRect(&m_rtBaseSrc[WINDOW_BASE_TOP],				457, 452, 461, 456);
	SetRect(&m_rtBaseSrc[WINDOW_BASE_TOP_RIGHT],		462, 452, 466, 456);
	SetRect(&m_rtBaseSrc[WINDOW_BASE_CENTER_LEFT],		452, 457, 456, 461);
	SetRect(&m_rtBaseSrc[WINDOW_BASE_CENTER],			457, 457, 461, 461);
	SetRect(&m_rtBaseSrc[WINDOW_BASE_CENTER_RIGHT],		462, 457, 466, 461);
	SetRect(&m_rtBaseSrc[WINDOW_BASE_BOTTOM_LEFT],		452, 462, 456, 466);
	SetRect(&m_rtBaseSrc[WINDOW_BASE_BOTTOM],			457, 462, 461, 466);
	SetRect(&m_rtBaseSrc[WINDOW_BASE_BOTTOM_RIGHT],		462, 462, 466, 466);

	SetWindowBase();

	InitSubControl();
}


void SPWindowCashHotList::SetWindowBase()
{
	SetRect(&m_rtBaseDest[WINDOW_BASE_TOP_LEFT],		m_iAX, m_iAY, m_iAX + 4, m_iAY + 4);
	SetRect(&m_rtBaseDest[WINDOW_BASE_TOP],				m_iAX + 4, m_iAY, m_iAX + 4 + 36, m_iAY + 4);	
	SetRect(&m_rtBaseDest[WINDOW_BASE_TOP_RIGHT],		m_iAX + 40, m_iAY, m_iAX + 40 + 4, m_iAY + 4);
	SetRect(&m_rtBaseDest[WINDOW_BASE_CENTER_LEFT],		m_iAX, m_iAY + 4, m_iAX + 4, m_iAY + 4 + 193);
	SetRect(&m_rtBaseDest[WINDOW_BASE_CENTER],			m_iAX + 4, m_iAY + 4, m_iAX + 4 + 36, m_iAY + 4 + 193);
	SetRect(&m_rtBaseDest[WINDOW_BASE_CENTER_RIGHT],	m_iAX + 40, m_iAY + 4, m_iAX + 40 + 4, m_iAY + 4 + 193);
	SetRect(&m_rtBaseDest[WINDOW_BASE_BOTTOM_LEFT],		m_iAX, m_iAY + 197, m_iAX + 4, m_iAY + 197 + 4);
	SetRect(&m_rtBaseDest[WINDOW_BASE_BOTTOM],			m_iAX + 4, m_iAY + 197, m_iAX + 4 + 36, m_iAY + 197 + 4);
	SetRect(&m_rtBaseDest[WINDOW_BASE_BOTTOM_RIGHT],	m_iAX + 40, m_iAY + 197, m_iAX + 40 + 4, m_iAY + 197 + 4);
}


void SPWindowCashHotList::InitSubControl()
{
	char* BG_IMAGE_FILE0		= "DATA/INTERFACE/CONCEPT/UI_WinBG_000.PNG";
	char* OBJECT_IMAGE_FILE0	= "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG";
	char* OBJECT_IMAGE_FILE1	= "DATA/INTERFACE/CONCEPT/UI_Object_001.PNG";
	char* BUTTON_IMAGE_FILE0	= "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG";

	SPWindowStatic* pStatic		= NULL;
	SPWindowStatic* pSkin		= NULL;
	SPWindowButton* pButton		= NULL;	
	
	//WIID_CASH_HOTLIST_LINE
	pStatic = new SPWindowStatic(WIID_CASH_HOTLIST_LINE, 0, 19, 2, 178, this);
	pStatic->SetImage(OBJECT_IMAGE_FILE0, 174, 308);
	pStatic->SetSrcSize(2, 2);
	pStatic->Show();
	
	//WIID_CASH_HOTLIST_TEXTIMG
	pStatic = new SPWindowStatic(WIID_CASH_HOTLIST_TEXTIMG, 2, 2, 40, 13, this);
	pStatic->SetImage(OBJECT_IMAGE_FILE0, 339, 286);
	pStatic->Show();

	//WIID_CASH_HOTLIST_SKIN
	int iSkinY = 17;
	pSkin = new SPWindowStatic(WIID_CASH_HOTLIST_SKIN, 0, iSkinY, 42, 180, this);
	pSkin->SetImage(OBJECT_IMAGE_FILE0, 162, 308);
	pSkin->SetSrcSize(3, 3);
	pSkin->Show();
	
	int iOffSet = 40;
	for(int i = 0; i < MAX_HOTITEM_VIEW; i++) {
		//WIID_CASH_HOTLIST_ITEMBUTTON
		pButton = new SPWindowButton(WIID_CASH_HOTLIST_ITEMBUTTON+i, 6, 29+(iOffSet*i) - iSkinY, 32, 32, pSkin);
		pButton->Show();
		
		//WIID_CASH_HOTLIST_ITEMSKIN
		pStatic = new SPWindowStatic(WIID_CASH_HOTLIST_ITEMSKIN+i, 5, 52+(iOffSet*i) - iSkinY, 35, 11, pSkin);
		pStatic->SetImage(OBJECT_IMAGE_FILE0, 440, 97);
		pStatic->Show();
		
		//WIID_CASH_HOTLIST_ITEMICON
		m_pItemIcon[i] = new SPWindowStatic(WIID_CASH_HOTLIST_ITEMICON+i, 6, 29+(iOffSet*i) - iSkinY, 32, 32, pSkin);
		m_pItemIcon[i]->Show();		
	}
}


void SPWindowCashHotList::Clean()
{
	for(int i = 0; i < MAX_HOTITEM_VIEW; i++) {
		m_pItemIcon[i]->SPSendMessage(SPIM_SET_IMAGECLEAR);
	}
	
	SAFE_RELEASE(m_pBaseSkin);
	SAFE_RELEASE(m_pDisable);
	SPWindow::Clean();
}


void SPWindowCashHotList::Show(bool bWithChild /*= true*/)
{
	SPHotItemManager* pHotManager = SPCashTradeManager::GetInstance()->GetHotItemManager();
	if(pHotManager == NULL)
		return;

	if(!pHotManager->IsShow())
		return;
	
	SPWindow::Show(bWithChild);
	UpdateControl();
}


void SPWindowCashHotList::Hide(bool bSendServer /*= true*/)
{
	SPWindow::Hide(bSendServer);
}


void SPWindowCashHotList::Process(float fTime)
{
	if(!m_bShow)
		return;

	SetWindowBase();
	
	SPWindow::Process(fTime);
}


void SPWindowCashHotList::Render(float fTime)
{
	if(!m_bShow)
		return;

	g_pVideo->Flush();

	if(m_pBaseSkin) {
		for(int i = 0; i < WINDOW_BASE_MAX; i++) {
			m_pBaseSkin->RenderTexture(&m_rtBaseDest[i], &m_rtBaseSrc[i]);
		}
	}

	g_pVideo->Flush();
	SPWindow::Render(fTime);

	for(int i = 0; i < MAX_HOTITEM_VIEW; i++) {
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
			m_pItemIcon[i]->GetWindowRect(m_rtStack[i]);
			RECT rtNumTarget, rtNumSrc;
			SetRect(&rtNumTarget, m_rtStack[i].left, m_rtStack[i].bottom - 10, m_rtStack[i].left + 9, m_rtStack[i].bottom);

			rtNumSrc.left = iOneStack * 9;
			rtNumSrc.right = rtNumSrc.left + 9;
			rtNumSrc.top = 0;
			rtNumSrc.bottom = 10;

			g_pItemCluster->GetNumTexture()->RenderTexture( &rtNumTarget , &rtNumSrc);
		}
	}

	g_pVideo->Flush();
}


void SPWindowCashHotList::UpdateControl()
{
	SPHotItemManager* pHotManager = SPCashTradeManager::GetInstance()->GetHotItemManager();
	if(pHotManager == NULL)
		return;	
	
	int iSize = pHotManager->GetHotItemSize();
	if(iSize > MAX_HOTITEM_VIEW)
		return;

	SPHotItemInfo* pHotItemInfo = NULL;
	SPCashItemAttr* pCashItemAttr = NULL;
	for(int i = 0; i < MAX_HOTITEM_VIEW; i++) {
		pHotItemInfo = NULL;
		pCashItemAttr = NULL;
		m_pItemIcon[i]->SPSendMessage(SPIM_SET_IMAGECLEAR);
		
		pHotItemInfo = pHotManager->GetHotItem(i);
		if(pHotItemInfo && pHotItemInfo->IsShow()) {
			SPTexture* pTexture;
			POINT pt;
			pTexture = g_pItemCluster->GetItemTextureFromTextureInx(
				pHotItemInfo->GetCashItemAttr()->iIcon, pHotItemInfo->GetCashItemAttr()->iIconIndex, pt);
			if( pTexture )
				m_pItemIcon[i]->SetImage(pTexture, pt.x, pt.y);
		}
	}
}


SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowCashHotList )

SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_CASH_HOTLIST_CLOSE,				OnClose	)

SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_ENTER,	WIID_CASH_HOTLIST_ITEMBUTTON,		WIID_CASH_HOTLIST_ITEMBUTTON + MAX_HOTITEM_VIEW,	OnToolTipEnter)
SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_OUT,		WIID_CASH_HOTLIST_ITEMBUTTON,		WIID_CASH_HOTLIST_ITEMBUTTON + MAX_HOTITEM_VIEW,	OnToolTipOut)
SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_MOVE,	WIID_CASH_HOTLIST_ITEMBUTTON,		WIID_CASH_HOTLIST_ITEMBUTTON + MAX_HOTITEM_VIEW,	OnToolTipMove)

SPIMESSAGE_CONTROL_RANGE( SPIM_RBUTTON_UP,		WIID_CASH_HOTLIST_ITEMBUTTON,		WIID_CASH_HOTLIST_ITEMBUTTON + MAX_HOTITEM_VIEW,	OnItemSelect)

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)


int SPWindowCashHotList::OnClose(WPARAM wParam, LPARAM lParam)
{
	Hide();
	return 1;
}


int SPWindowCashHotList::OnToolTipMove(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	int iSelectNum = iUiID - WIID_CASH_HOTLIST_ITEMBUTTON;	

	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	if( pWindow == NULL )
		return 1;
	
	SPHotItemInfo* pHotItemInfo = SPCashTradeManager::GetInstance()->GetHotItemManager()->GetHotItem(iSelectNum);
	if(pHotItemInfo && pHotItemInfo->GetCashItemAttr()) {
		if( pHotItemInfo->GetCashItemAttr()->eCategory == CI_PACKAGE )
			pWindow->SPSendMessage(SPIM_TOOLTIP_ENTER, (WPARAM)pHotItemInfo->GetCashItemAttr(), -5);
		else
		{
			int	iCount = 0;

			if( pHotItemInfo->GetCashItemAttr()->eCategory == CI_POINT )
			{
				for( int i = 0 ; i < CASHITEMINFO_COUNT ; ++i )
				{
					if( iCount > 1 )
						break;

					if( pHotItemInfo->GetCashItemAttr()->ItemInfo[ i ].iID > 0 )
						++iCount;
				}

			}

			if( iCount > 1 )
			{
				pWindow->SPSendMessage(SPIM_TOOLTIP_MOVE, (WPARAM)pHotItemInfo->GetCashItemAttr(), -5);
			}
			else
			{
				SPItemAttr* pkItemAttr = NULL;
				g_pItemCluster->GetItemInfo(pHotItemInfo->GetCashItemAttr()->ItemInfo[0].iID, pkItemAttr);
				if( pkItemAttr == NULL )
					return 1;

				SPItemStatus kItemStatus;
				kItemStatus.SetItemID(pHotItemInfo->GetCashItemAttr()->ItemInfo[0].iID);
				kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPID, pkItemAttr->m_ePosID1);
				kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPCLASS, pkItemAttr->m_iCategory);
				kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPITEMID, pkItemAttr->m_iNum);
				kItemStatus.SPSendMessage(MV_ITEM_SETCOLOR,	pkItemAttr->m_iColorID);
				kItemStatus.SPSendMessage(MV_ITEM_SET_RARELEVEL, pkItemAttr->m_eRareLimit);

				if(pkItemAttr->m_eDuration != ITEM_DURATION_NULL) {
					kItemStatus.SetLifeTime(pkItemAttr->m_iLifeTime * 3600);
				}

				if(!pHotItemInfo->GetCashItemAttr()->strHotDesc.empty()) {
					kItemStatus.SetHotDesc(pHotItemInfo->GetCashItemAttr()->strHotDesc);
				}

				pWindow->SPSendMessage(SPIM_TOOLTIP_ENTER, (WPARAM)&kItemStatus, lParam);
			}
		}
		pWindow->Show();
	}
	else {
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
	}	

	return 1;
}


int SPWindowCashHotList::OnToolTipEnter(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	int iSelectNum = iUiID - WIID_CASH_HOTLIST_ITEMBUTTON;

	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	if( pWindow == NULL )
		return 1;

	SPHotItemInfo* pHotItemInfo = SPCashTradeManager::GetInstance()->GetHotItemManager()->GetHotItem(iSelectNum);
	if(pHotItemInfo && pHotItemInfo->GetCashItemAttr()) {
		if( pHotItemInfo->GetCashItemAttr()->eCategory == CI_PACKAGE )
			pWindow->SPSendMessage(SPIM_TOOLTIP_ENTER, (WPARAM)pHotItemInfo->GetCashItemAttr(), -5);
		else
		{
			int	iCount = 0;

			if( pHotItemInfo->GetCashItemAttr()->eCategory == CI_POINT )
			{
				for( int i = 0 ; i < CASHITEMINFO_COUNT ; ++i )
				{
					if( iCount > 1 )
						break;

					if( pHotItemInfo->GetCashItemAttr()->ItemInfo[ i ].iID > 0 )
						++iCount;
				}

			}

			if( iCount > 1 )
			{
				pWindow->SPSendMessage(SPIM_TOOLTIP_MOVE, (WPARAM)pHotItemInfo->GetCashItemAttr(), -5);
			}
			else
			{
				SPItemAttr* pkItemAttr = NULL;
				g_pItemCluster->GetItemInfo(pHotItemInfo->GetCashItemAttr()->ItemInfo[0].iID, pkItemAttr);
				if( pkItemAttr == NULL )
					return 1;

				SPItemStatus kItemStatus;
				kItemStatus.SetItemID(pHotItemInfo->GetCashItemAttr()->ItemInfo[0].iID);
				kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPID, pkItemAttr->m_ePosID1);
				kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPCLASS, pkItemAttr->m_iCategory);
				kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPITEMID, pkItemAttr->m_iNum);
				kItemStatus.SPSendMessage(MV_ITEM_SETCOLOR,	pkItemAttr->m_iColorID);
				kItemStatus.SPSendMessage(MV_ITEM_SET_RARELEVEL, pkItemAttr->m_eRareLimit);

				if(pkItemAttr->m_eDuration != ITEM_DURATION_NULL) {
					kItemStatus.SetLifeTime(pkItemAttr->m_iLifeTime * 3600);
				}

				if(!pHotItemInfo->GetCashItemAttr()->strHotDesc.empty()) {
					kItemStatus.SetHotDesc(pHotItemInfo->GetCashItemAttr()->strHotDesc);
				}

				pWindow->SPSendMessage(SPIM_TOOLTIP_ENTER, (WPARAM)&kItemStatus, lParam);
			}
		}
		pWindow->Show();
	}
	else {
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
	}

	return 1;
}


int SPWindowCashHotList::OnToolTipOut(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
	pWindow->Show();
	return 1;
}


int SPWindowCashHotList::OnItemSelect(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	int iSelectNum = iUiID - WIID_CASH_HOTLIST_ITEMBUTTON;	

	SPHotItemInfo* pHotItemInfo = SPCashTradeManager::GetInstance()->GetHotItemManager()->GetHotItem(iSelectNum);
	if(pHotItemInfo && pHotItemInfo->GetCashItemAttr()) {
		if(SPCashTradeManager::GetInstance()->SetSelectCashItem(pHotItemInfo->m_iCashShopID)) {
			return 1;
			////
			//SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_CASH_TRADE_SELECT);
			//if(pWindow) {
			//	pWindow->Show();
			//}
		}
	}

	return 1;
}





