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
#include "SPWindowCashTradeSelect.h"

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

#include "SPCashTradeManager.h"

#include "SPCheckManager.h"
#include "SPLocalizeManager.h"

#include "SPDebug.h"



SPWindowCashTradeSelect::SPWindowCashTradeSelect( INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent /*= NULL*/)
: SPWindow(WNDID_CASH_TRADE_SELECT, InstanceID, iX, iY, iCX, iCY, pParent)
{
	Init();
}


SPWindowCashTradeSelect::~SPWindowCashTradeSelect()
{	
	Clean();
}


void SPWindowCashTradeSelect::Init()
{
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

	SetRect(&m_rtBaseSrc[WINDOW_BASE_TOP_LEFT],			497, 467, 501, 471);
	SetRect(&m_rtBaseSrc[WINDOW_BASE_TOP],				503, 467, 505, 471);
	SetRect(&m_rtBaseSrc[WINDOW_BASE_TOP_RIGHT],		507, 467, 511, 471);
	SetRect(&m_rtBaseSrc[WINDOW_BASE_CENTER_LEFT],		497, 473, 501, 475);
	SetRect(&m_rtBaseSrc[WINDOW_BASE_CENTER],			503, 473, 505, 475);
	SetRect(&m_rtBaseSrc[WINDOW_BASE_CENTER_RIGHT],		507, 473, 511, 475);
	SetRect(&m_rtBaseSrc[WINDOW_BASE_BOTTOM_LEFT],		497, 477, 501, 481);
	SetRect(&m_rtBaseSrc[WINDOW_BASE_BOTTOM],			503, 477, 505, 481);
	SetRect(&m_rtBaseSrc[WINDOW_BASE_BOTTOM_RIGHT],		507, 477, 511, 481);

	SetWindowBase();
	
	InitSubControl();
}


void SPWindowCashTradeSelect::SetWindowBase()
{
	SetRect(&m_rtBaseDest[WINDOW_BASE_TOP_LEFT],		m_iAX, m_iAY, m_iAX + 4, m_iAY + 4);
	SetRect(&m_rtBaseDest[WINDOW_BASE_TOP],				m_iAX + 4, m_iAY, m_iAX + 4 + 156, m_iAY + 4);	
	SetRect(&m_rtBaseDest[WINDOW_BASE_TOP_RIGHT],		m_iAX + 160, m_iAY, m_iAX + 160 + 4, m_iAY + 4);
	SetRect(&m_rtBaseDest[WINDOW_BASE_CENTER_LEFT],		m_iAX, m_iAY + 4, m_iAX + 4, m_iAY + 4 + 95);
	SetRect(&m_rtBaseDest[WINDOW_BASE_CENTER],			m_iAX + 4, m_iAY + 4, m_iAX + 4 + 156, m_iAY + 4 + 95);
	SetRect(&m_rtBaseDest[WINDOW_BASE_CENTER_RIGHT],	m_iAX + 160, m_iAY + 4, m_iAX + 160 + 4, m_iAY + 4 + 95);
	SetRect(&m_rtBaseDest[WINDOW_BASE_BOTTOM_LEFT],		m_iAX, m_iAY + 99, m_iAX + 4, m_iAY + 99 + 4);
	SetRect(&m_rtBaseDest[WINDOW_BASE_BOTTOM],			m_iAX + 4, m_iAY + 99, m_iAX + 4 + 156, m_iAY + 99 + 4);
	SetRect(&m_rtBaseDest[WINDOW_BASE_BOTTOM_RIGHT],	m_iAX + 160, m_iAY + 99, m_iAX + 160 + 4, m_iAY + 99 + 4);
}


void SPWindowCashTradeSelect::InitSubControl()
{
	char* BG_IMAGE_FILE0		= "DATA/INTERFACE/CONCEPT/UI_WinBG_000.PNG";
	char* OBJECT_IMAGE_FILE0	= "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG";
	char* OBJECT_IMAGE_FILE1	= "DATA/INTERFACE/CONCEPT/UI_Object_001.PNG";
	char* BUTTON_IMAGE_FILE0	= "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG";

	SPWindowStatic* pStatic		= NULL;
	SPWindowStatic* pSkin		= NULL;
	SPWindowButton* pButton		= NULL;

	// WIID_CASH_TRADE_SELECT_ITEM_NAME
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_SELECT_ITEM_NAME, 5, 6, 141, 13, this);		
	pStatic->SetFormat(DT_LEFT | DT_VCENTER);
	//pStatic->SetMultiLine(true, 2);
	pStatic->SetFont(FONT_12_NORMAL);
	pStatic->SetFontColor(RGBA(49, 76, 97, 255));
	pStatic->SetWindowText("Use Item Name");
	pStatic->Show();
	m_pItemName = pStatic;

	//WIID_CASH_TRADE_SELECT_CLOSE
	pButton = new SPWindowButton(WIID_CASH_TRADE_SELECT_CLOSE, 147, 6, 12, 12, this);
	pButton->SetImage		(BUTTON_IMAGE_FILE0, 497,	27); //일반
	pButton->SetImageHit	(BUTTON_IMAGE_FILE0, 497,	27); //on	
	pButton->SetImagePush	(BUTTON_IMAGE_FILE0, 484,	40); //push
	pButton->SetImageDisable(BUTTON_IMAGE_FILE0, 497,	40); //disable
	pButton->Show();	

	//WIID_CASH_TRADE_SELECT_MALE
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_SELECT_MALE, 15, 62, 12, 14, this);
	pStatic->SetImage(OBJECT_IMAGE_FILE0, 393, 264);
	pStatic->Show();
	m_pMaleIcon = pStatic;
	
	//WIID_CASH_TRADE_SELECT_FEMALE
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_SELECT_FEMALE, 15, 62, 12, 14, this);
	pStatic->SetImage(OBJECT_IMAGE_FILE0, 393, 279);
	pStatic->Show();
	m_pFeMaleIcon = pStatic;
	
	//WIID_CASH_TRADE_SELECT_ITEMBUTTON
	pButton = new SPWindowButton(WIID_CASH_TRADE_SELECT_ITEMBUTTON, 41, 44, 32, 32, this);	
	pButton->Show();
	
	//WIID_CASH_TRADE_SELECT_ITEMSKIN
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_SELECT_ITEMSKIN, 41, 68, 35, 11, this);
	pStatic->SetImage(OBJECT_IMAGE_FILE0, 260, 295);
	pStatic->Show();
	
	//WIID_CASH_TRADE_SELECT_ITEMICON
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_SELECT_ITEMICON, 41, 44, 32, 32, this);
	pStatic->Show();
	m_pItemIcon = pStatic;

	//WIID_CASH_TRADE_SELECT_COSTSKIN
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_SELECT_COSTSKIN, 11, 81, 65, 14, this);	
	pStatic->SetWindowType( WND_TYPE_LEFTBASE );
	pStatic->SetImageNormalHead(OBJECT_IMAGE_FILE0, 380, 253, 382, 267);
	pStatic->SetImageNormalBody(OBJECT_IMAGE_FILE0, 384, 253, 388, 267);
	pStatic->SetImageNormalTail(OBJECT_IMAGE_FILE0, 390, 253, 392, 267);	
	pStatic->Show();

	//WIID_CASH_TRADE_SELECT_COST
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_SELECT_COST, 12, 82, 43, 13, this);		
	pStatic->SetFormat(DT_RIGHT | DT_VCENTER);
	//pStatic->SetMultiLine(true, 2);
	pStatic->SetFont(FONT_12_NORMAL);
	pStatic->SetFontColor(RGBA(255, 255, 255, 255));
	pStatic->SetWindowText("111");
	pStatic->Show();
	m_pCost = pStatic;
	
	//WIID_CASH_TRADE_SELECT_COSTICON
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_SELECT_COSTICON, 57, 83, 15, 11, this);
	pStatic->SetImage(OBJECT_IMAGE_FILE0, 476, 97);
	pStatic->Show();
	m_pCostIcon = pStatic;

	//WIID_CASH_TRADE_SELECT_ITEMEVENT
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_SELECT_ITEMEVENT, 28, 18, 55, 29, this);
	pStatic->SetImage(OBJECT_IMAGE_FILE1, 1,	1);		//뉴
	pStatic->SetImage(OBJECT_IMAGE_FILE1, 113,	1);		//핫
	pStatic->SetImage(OBJECT_IMAGE_FILE1, 57,	1);		//세일	
	pStatic->SetImage(OBJECT_IMAGE_FILE1, 169,	1);		//이벤트
	pStatic->Show();
	m_pItemEvent = pStatic;
	
	//WIID_CASH_TRADE_SELECT_WISH
	pButton = new SPWindowButton(WIID_CASH_TRADE_SELECT_WISH, 85, 32, 68, 17, this);
	pButton->SetImage		(BUTTON_IMAGE_FILE0, 346,	73); //일반
	pButton->SetImageHit	(BUTTON_IMAGE_FILE0, 346,	91); //on	
	pButton->SetImagePush	(BUTTON_IMAGE_FILE0, 346,	109); //push
	pButton->SetImageDisable(BUTTON_IMAGE_FILE0, 346,	127); //disable
	pButton->Show();

	//WIID_CASH_TRADE_SELECT_GIFT
	pButton = new SPWindowButton(WIID_CASH_TRADE_SELECT_GIFT, 85, 51, 68, 17, this);
	pButton->SetImage		(BUTTON_IMAGE_FILE0, 277,	73); //일반
	pButton->SetImageHit	(BUTTON_IMAGE_FILE0, 277,	91); //on	
	pButton->SetImagePush	(BUTTON_IMAGE_FILE0, 277,	109); //push
	pButton->SetImageDisable(BUTTON_IMAGE_FILE0, 277,	127); //disable
	pButton->Show();

	//WIID_CASH_TRADE_SELECT_BUY
	pButton = new SPWindowButton(WIID_CASH_TRADE_SELECT_BUY, 85, 70, 68, 17, this);
	pButton->SetImage		(BUTTON_IMAGE_FILE0, 208,	73); //일반
	pButton->SetImageHit	(BUTTON_IMAGE_FILE0, 208,	91); //on	
	pButton->SetImagePush	(BUTTON_IMAGE_FILE0, 208,	109); //push
	pButton->SetImageDisable(BUTTON_IMAGE_FILE0, 208,	127); //disable
	pButton->Show();
}


void SPWindowCashTradeSelect::Clean()
{
	m_pItemIcon->SPSendMessage(SPIM_SET_IMAGECLEAR);
	
	SAFE_RELEASE(m_pDisable);
	SAFE_RELEASE(m_pBaseSkin);
	SPWindow::Clean();
}


void SPWindowCashTradeSelect::Show(bool bWithChild /*= true*/)
{	
	SPCashItemAttr* pCashItem = SPCashTradeManager::GetInstance()->GetSelectCashItem();
	if(pCashItem == NULL || pCashItem->iCashID == 0)
		return;	

	SPWindow::Show(bWithChild);	

	if(UpdateControl() == false)
		return;
}


bool SPWindowCashTradeSelect::UpdateControl()
{
	SPCashItemAttr* pCashItem = SPCashTradeManager::GetInstance()->GetSelectCashItem();
	if(pCashItem && pCashItem->iCashID) {
		m_pItemName->SetWindowText(pCashItem->strName.c_str());
		
		if(pCashItem->iMaxOZ) {
			m_pCost->SetWindowText(ConvertMoneyToString(pCashItem->iMaxOZ));
			m_pCostIcon->SetImagePos(476, 97);
		}
		else if(pCashItem->iMaxMile) {
			m_pCost->SetWindowText(ConvertMoneyToString(pCashItem->iMaxMile));
			m_pCostIcon->SetImagePos(492, 97);
		}

		if(pCashItem->iGender == 1) {
			m_pFeMaleIcon->Hide();
			
			m_pMaleIcon->AdjustCoord(15, 62, 12, 14);
			m_pMaleIcon->Show();
		}
		else if(pCashItem->iGender == 2) {			
			m_pMaleIcon->Hide();

			m_pFeMaleIcon->AdjustCoord(15, 62, 12, 14);
			m_pFeMaleIcon->Show();
		}
		else {
			m_pMaleIcon->AdjustCoord(15, 44, 12, 14);
			m_pMaleIcon->Show();

			m_pFeMaleIcon->AdjustCoord(15, 62, 12, 14);
			m_pFeMaleIcon->Show();
		}

		SPTexture* pTexture;
		POINT pt;
		m_pItemIcon->SPSendMessage(SPIM_SET_IMAGECLEAR);
		pTexture = g_pItemCluster->GetItemTextureFromTextureInx(pCashItem->iIcon, pCashItem->iIconIndex, pt);
		if( pTexture )
			m_pItemIcon->SetImage(pTexture, pt.x, pt.y);

		m_pItemEvent->Hide();
		if(pCashItem->iEventIcon) {
			m_pItemEvent->SPSendMessage(SPIM_SET_IMAGEINDEX, (pCashItem->iEventIcon - 1));
			m_pItemEvent->Show();
		}
		
		return true;
	}	
	
	return false;
}


void SPWindowCashTradeSelect::Hide(bool bSendServer /*= true*/)
{
	SPWindow::Hide(bSendServer);
}


void SPWindowCashTradeSelect::Process(float fTime)
{
	if(!m_bShow)
		return;
	
	SetWindowBase();
	
	SPWindow::Process(fTime);
}


void SPWindowCashTradeSelect::Render(float fTime)
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

////
//#ifdef _DEBUG
//	SPWindow* pWindow = Find(WIID_CASH_TRADE_SELECT_ITEM_NAME, true);	
//	if(pWindow) {
//		g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
//		RECT rtRect;
//		pWindow->GetWindowRect(rtRect);
//		g_pVideo->GetSysLine()->DrawRect(rtRect);
//		g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
//	}
//#endif
}



SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowCashTradeSelect )

SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_CASH_TRADE_SELECT_CLOSE,				OnClose	)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_CASH_TRADE_SELECT_WISH,				OnWish	)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_CASH_TRADE_SELECT_GIFT,				OnGift	)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_CASH_TRADE_SELECT_BUY,					OnBuy	)

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)


int SPWindowCashTradeSelect::OnClose(WPARAM wParam, LPARAM lParam)
{
	Hide();
	return 1;
}


int SPWindowCashTradeSelect::OnWish(WPARAM wParam, LPARAM lParam)
{	
	////
	//SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_CASH_TRADE_RESULT);
	//if(pWindow) {
	//	pWindow->Show();
	//}
	//g_pInterfaceManager->ShowWindow(WIID_CASH_TRADE_RESULT, SP_NONE, SP_TOGGLE_ON, true);	

	SPCashItemAttr* pCashItem = SPCashTradeManager::GetInstance()->GetSelectCashItem();
	if(pCashItem == NULL || pCashItem->iCashID == 0)
		return 1;	

	SPCashTradeManager::GetInstance()->SendWishAdd(CASH_TRADE_PLACE_FIELD, pCashItem->iCashID);
	
	return 1;
}


int SPWindowCashTradeSelect::OnGift(WPARAM wParam, LPARAM lParam)
{
	//g_pInterfaceManager->ShowWindow(WIID_CASH_GIFT, SP_NONE, SP_TOGGLE_ON, true);
	
	SPCashTradeManager::GetInstance()->SendCashShopPrice(CASH_TRADE_TYPE_GIFT);
	
	return 1;
}


int SPWindowCashTradeSelect::OnBuy(WPARAM wParam, LPARAM lParam)
{
	//SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_CASH_TRADE);
	//if(pWindow) {
	//	pWindow->Show();
	//}
	//g_pInterfaceManager->ShowWindow(WIID_CASH_TRADE, SP_NONE, SP_TOGGLE_ON, true);

	SPCashTradeManager::GetInstance()->SendCashShopPrice(CASH_TRADE_TYPE_BUY);
	
	return 1;
}



