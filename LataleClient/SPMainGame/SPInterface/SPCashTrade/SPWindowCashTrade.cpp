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
#include "SPWindowCashTrade.h"

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

#include "SPRenderModelDEF.h"
#include "SPGameObjectDef.h"
#include "SPGameObject.h"
#include "SPGOBStatus.h"
#include "SPPlayerStatus.h"
#include "SPPlayerStatusModel.h"
#include "SPPlayerInvenArchive.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGOBCoordPhysics.h"
#include "SPPlayer.h"

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



SPWindowCashTrade::SPWindowCashTrade( INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent /*= NULL*/)
: SPWindow(WNDID_CASH_TRADE, InstanceID, iX, iY, iCX, iCY, pParent)
{
	Init();
}


SPWindowCashTrade::~SPWindowCashTrade()
{
	Clean();
}


void SPWindowCashTrade::Init()
{
	m_iUsePoint = 0;
	InitSubControl();
}


void SPWindowCashTrade::InitSubControl()
{
	char* CASH_SHOP1			= "DATA/INTERFACE/CONCEPT/cashshop01.png";
	char* CASH_SHOP2			= "DATA/INTERFACE/CONCEPT/cashshop02.png";
	char* CASH_SHOP3			= "DATA/INTERFACE/CONCEPT/cashshop03.png";
	char* CASH_SHOP4			= "DATA/INTERFACE/CONCEPT/cashshop04.png";
	//char* BG_IMAGE_FILE0		= "DATA/INTERFACE/CONCEPT/UI_WinBG_000.PNG";
	//char* OBJECT_IMAGE_FILE0	= "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG";
	//char* OBJECT_IMAGE_FILE1	= "DATA/INTERFACE/CONCEPT/UI_Object_001.PNG";
	//char* BUTTON_IMAGE_FILE0	= "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG";

	SPWindowStatic* pStatic		= NULL;
	SPWindowStatic* pSkin		= NULL;
	SPWindowStatic* pCenter		= NULL;
	SPWindowButton* pButton		= NULL;	

	//WIID_CASH_TRADE_TOP
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_TOP, 0, 0, 268, 8, this);
	pStatic->SetImage(CASH_SHOP2, 1, 28);
	pStatic->Show();

	//WIID_CASH_TRADE_CENTER
	int iCenterY = 8;
	pCenter = new SPWindowStatic(WIID_CASH_TRADE_CENTER, 0, 8, 268, 266, this);
	pCenter->SetImage(CASH_SHOP2, 1, 38);
	pCenter->SetSrcSize(268, 6);
	pCenter->Show();

	//WIID_CASH_TRADE_ITEMSKIN
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_ITEMSKIN, 22, 42 - iCenterY, 35, 11, pCenter);
	pStatic->SetImage(CASH_SHOP1, 443, 137);
	pStatic->Show();
	
	//WIID_CASH_TRADE_ITEM	
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_ITEM, 24, 18 - iCenterY, 32, 32, pCenter);	
	pStatic->Show();
	m_pItemIcon = pStatic;

	//WIID_CASH_TRADE_ITEMNAME
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_ITEMNAME, 69, 20 - iCenterY, 175, 12, pCenter);		
	pStatic->SetFormat(DT_LEFT | DT_VCENTER);
	//pStatic->SetMultiLine(true, 2);
	pStatic->SetFont(FONT_12_BOLD);
	pStatic->SetFontColor(RGBA(49, 76, 97, 255));
	pStatic->SetWindowText("Use Item Name");
	pStatic->Show();
	m_pItemName = pStatic;

	//WIID_CASH_TRADE_COSTSKIN
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_COSTSKIN, 70, 35 - iCenterY, 80, 14, pCenter);	
	pStatic->SetWindowType( WND_TYPE_LEFTBASE );
	pStatic->SetImageNormalHead(CASH_SHOP1, 493, 497, 495, 511);
	pStatic->SetImageNormalBody(CASH_SHOP1, 497, 497, 499, 511);
	pStatic->SetImageNormalTail(CASH_SHOP1, 501, 497, 503, 511);	
	pStatic->Show();
	
	//WIID_CASH_TRADE_COST	
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_COST, 74, 37 - iCenterY, 53, 12, pCenter);
	pStatic->SetFormat(DT_RIGHT | DT_VCENTER);
	//pStatic->SetMultiLine(true, 2);
	pStatic->SetFont(FONT_12_NORMAL);
	pStatic->SetFontColor(RGBA(255, 255, 255, 255));
	pStatic->SetWindowText("111");
	pStatic->Show();
	m_pCost = pStatic;
	
	//WIID_CASH_TRADE_COSTICON
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_COSTICON, 131, 37 - iCenterY, 15, 11, pCenter);
	pStatic->SetImage(CASH_SHOP1, 463, 113);
	pStatic->Show();
	m_pCostIcon = pStatic;

	//WIID_CASH_TRADE_CUR_CASH_TEXTIMG
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_CUR_CASH_TEXTIMG, 22, 76 - iCenterY, 96, 13, pCenter);
	pStatic->SetImage(CASH_SHOP3, 385, 185);
	pStatic->Show();

	//WIID_CASH_TRADE_CUR_CASH
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_CUR_CASH, 151, 77 - iCenterY, 46, 12, pCenter);
	pStatic->SetFormat(DT_RIGHT | DT_VCENTER);
	//pStatic->SetMultiLine(true, 2);
	pStatic->SetFont(FONT_12_NORMAL);
	pStatic->SetFontColor(RGBA(49, 76, 97, 255));
	pStatic->SetWindowText("111");
	pStatic->Show();
	m_pCurCash = pStatic;

	//WIID_CASH_TRADE_CUR_CASH_ICON
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_CUR_CASH_ICON, 201, 77 - iCenterY, 15, 11, pCenter);
	pStatic->SetImage(CASH_SHOP1, 463, 113);
	pStatic->Show();

	//WIID_CASH_TRADE_USE_CASH_TEXTIMG
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_USE_CASH_TEXTIMG, 22, 95 - iCenterY, 96, 13, pCenter);
	pStatic->SetImage(CASH_SHOP2, 353, 106);
	pStatic->Show();

	//WIID_CASH_TRADE_USE_CASH
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_USE_CASH, 151, 96 - iCenterY, 46, 12, pCenter);
	pStatic->SetFormat(DT_RIGHT | DT_VCENTER);
	//pStatic->SetMultiLine(true, 2);
	pStatic->SetFont(FONT_12_NORMAL);
	pStatic->SetFontColor(RGBA(49, 76, 97, 255));
	pStatic->SetWindowText("222");
	pStatic->Show();
	m_pUseCash = pStatic;

	//WIID_CASH_TRADE_USE_CASH_ICON
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_USE_CASH_ICON, 201, 96 - iCenterY, 15, 11, pCenter);
	pStatic->SetImage(CASH_SHOP1, 463, 113);
	pStatic->Show();

	//WIID_CASH_TRADE_USE_POINT_TEXTIMG
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_USE_POINT_TEXTIMG, 22, 114 - iCenterY, 96, 13, pCenter);
	pStatic->SetImage(CASH_SHOP2, 353, 134);
	pStatic->Show();

	//WIID_CASH_TRADE_USE_POINT
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_USE_POINT, 151, 115 - iCenterY, 46, 12, pCenter);
	pStatic->SetFormat(DT_RIGHT | DT_VCENTER);
	//pStatic->SetMultiLine(true, 2);
	pStatic->SetFont(FONT_12_NORMAL);
	pStatic->SetFontColor(RGBA(49, 76, 97, 255));
	pStatic->SetWindowText("333");
	pStatic->Show();
	m_pUsePoint = pStatic;

	//WIID_CASH_TRADE_USE_POINT_ICON
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_USE_POINT_ICON, 201, 115 - iCenterY, 15, 11, pCenter);
	pStatic->SetImage(CASH_SHOP1, 463, 125);
	pStatic->Show();

	//WIID_CASH_TRADE_USE_POINT_INC
	pButton = new SPWindowButton(WIID_CASH_TRADE_USE_POINT_INC, 225, 110 - iCenterY, 13, 9, pCenter);
	pButton->SetImage		(CASH_SHOP2, 481,	407); //일반
	pButton->SetImageHit	(CASH_SHOP2, 481,	417); //on	
	pButton->SetImagePush	(CASH_SHOP2, 481,	427); //push
	pButton->SetImageDisable(CASH_SHOP2, 481,	437); //disable
	pButton->Show();
	
	//WIID_CASH_TRADE_USE_POINT_DEC
	pButton = new SPWindowButton(WIID_CASH_TRADE_USE_POINT_DEC, 225, 122 - iCenterY, 13, 9, pCenter);
	pButton->SetImage		(CASH_SHOP2, 495,	407); //일반
	pButton->SetImageHit	(CASH_SHOP2, 495,	417); //on	
	pButton->SetImagePush	(CASH_SHOP2, 495,	427); //push
	pButton->SetImageDisable(CASH_SHOP2, 495,	437); //disable
	pButton->Show();

	//WIID_CASH_TRADE_PAY_TEXTIMG
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_PAY_TEXTIMG, 22, 149 - iCenterY, 96, 13, pCenter);
	pStatic->SetImage(CASH_SHOP2, 353, 120);
	pStatic->Show();

	//WIID_CASH_TRADE_PAY
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_PAY, 151, 150 - iCenterY, 46, 12, pCenter);
	pStatic->SetFormat(DT_RIGHT | DT_VCENTER);
	//pStatic->SetMultiLine(true, 2);
	pStatic->SetFont(FONT_12_NORMAL);
	pStatic->SetFontColor(RGBA(49, 76, 97, 255));
	pStatic->SetWindowText("444");
	pStatic->Show();
	m_pPayCash = pStatic;

	//WIID_CASH_TRADE_PAY_ICON
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_PAY_ICON, 201, 150 - iCenterY, 15, 11, pCenter);
	pStatic->SetImage(CASH_SHOP1, 463, 113);
	pStatic->Show();
	m_pPayIcon = pStatic;

	//WIID_CASH_TRADE_CHANGE_TEXTIMG
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_CHANGE_TEXTIMG, 22, 168 - iCenterY, 96, 13, pCenter);
	pStatic->SetImage(CASH_SHOP3, 241, 242);
	pStatic->Show();

	//WIID_CASH_TRADE_CHANGE
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_CHANGE, 151, 169 - iCenterY, 46, 12, pCenter);
	pStatic->SetFormat(DT_RIGHT | DT_VCENTER);
	//pStatic->SetMultiLine(true, 2);
	pStatic->SetFont(FONT_12_NORMAL);
	pStatic->SetFontColor(RGBA(49, 76, 97, 255));
	pStatic->SetWindowText("555");
	pStatic->Show();
	m_pChangeCash = pStatic;

	//WIID_CASH_TRADE_CHANGE_ICON
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_CHANGE_ICON, 201, 169 - iCenterY, 15, 11, pCenter);
	pStatic->SetImage(CASH_SHOP1, 463, 113);
	pStatic->Show();
	m_pChangeIcon = pStatic;

	//WIID_CASH_TRADE_HELP (SPIM_SET_IMAGEINDEX)
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_HELP, 13, 196 - iCenterY, 239, 45, pCenter);
	pStatic->SetImage(CASH_SHOP2, 261, 162);			//포인트 100 단위 거래
	pStatic->SetImage(CASH_SHOP3, 1, 210);				//일반적인 cash가 부족한 경우
	pStatic->SetImage(CASH_SHOP4, 1, 1);				//포인트로만 구매 하는 경우
	pStatic->Show();
	m_pHelp = pStatic;

	//WIID_CASH_TRADE_OK
	pButton = new SPWindowButton(WIID_CASH_TRADE_OK, 64, 247 - iCenterY, 66, 17, pCenter);
	pButton->SetImage		(CASH_SHOP2, 322,	280); //일반
	pButton->SetImageHit	(CASH_SHOP2, 322,	298); //on	
	pButton->SetImagePush	(CASH_SHOP2, 322,	316); //push
	pButton->SetImageDisable(CASH_SHOP2, 322,	334); //disable
	pButton->Show();

	//WIID_CASH_TRADE_CANCEL
	pButton = new SPWindowButton(WIID_CASH_TRADE_CANCEL, 141, 247 - iCenterY, 66, 17, pCenter);
	pButton->SetImage		(CASH_SHOP2, 389,	280); //일반
	pButton->SetImageHit	(CASH_SHOP2, 389,	298); //on	
	pButton->SetImagePush	(CASH_SHOP2, 389,	316); //push
	pButton->SetImageDisable(CASH_SHOP2, 389,	334); //disable
	pButton->Show();

	//WIID_CASH_TRADE_BOTTOM
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_BOTTOM, 0, 274, 268, 8, this);
	pStatic->SetImage(CASH_SHOP2, 1, 46);
	pStatic->Show();
}


void SPWindowCashTrade::Clean()
{
	m_pItemIcon->SPSendMessage(SPIM_SET_IMAGECLEAR);
	SPWindow::Clean();
}


void SPWindowCashTrade::Show(bool bWithChild /*= true*/)
{
	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return;
	
	SPCashItemAttr* pCashItem = SPCashTradeManager::GetInstance()->GetSelectCashItem();
	if(pCashItem == NULL || pCashItem->iCashID == 0)
		return;			
	
	m_pHelp->SPSendMessage(SPIM_SET_IMAGEINDEX, 0);

	int iCurPoint = g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValue(STATUS_MILE);	
	m_iPointTradeUnit = SPLocalizeManager::GetInstance()->GetMileageGlowUnit();	

	m_iUseCash = pCashItem->iMaxOZ;
	m_iUsePoint = 0;
	//if(pCashItem->iMaxOZ > 0 && pCashItem->iMaxMile > 0) {						//캐시와 포인트 로 구매가능		
	//	if(iCurPoint >= pCashItem->iMaxMile) {
	//		m_iUsePoint = pCashItem->iMaxMile;
	//	}
	//	else {
	//		m_iUsePoint = (iCurPoint / m_iPointTradeUnit) * m_iPointTradeUnit;			
	//	}		
	//}
	//else 
	if(pCashItem->iMaxOZ == 0 && pCashItem->iMaxMile > 0) {			//포인트로만 구매 가능		
		m_iUsePoint = pCashItem->iMaxMile;

		if(m_iUsePoint > iCurPoint) {
			if(g_pResourceManager->GetGlobalString(9001011)) {
				std::string strMsg = g_pResourceManager->GetGlobalString(9001011);
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
			}
			return;
		}

		m_pHelp->SPSendMessage(SPIM_SET_IMAGEINDEX, 2);
	}

	SPWindow::Show(bWithChild);
	
	UpdatePointControl(pCashItem);

	if(UpdateControl() == false)
		return;
}


void SPWindowCashTrade::UpdatePointControl(SPCashItemAttr* pCashItem)
{	
	SPWindow* pWindow = NULL;

	if(pCashItem && pCashItem->iCashID) {
		if(pCashItem->iMaxOZ > 0 && pCashItem->iMaxMile > 0) {
			pWindow = NULL;
			pWindow = Find(WIID_CASH_TRADE_USE_POINT_TEXTIMG, true);
			if(pWindow)
				pWindow->Show();

			pWindow = NULL;
			pWindow = Find(WIID_CASH_TRADE_USE_POINT, true);
			if(pWindow)
				pWindow->Show();

			pWindow = NULL;
			pWindow = Find(WIID_CASH_TRADE_USE_POINT_ICON, true);
			if(pWindow)
				pWindow->Show();

			pWindow = NULL;
			pWindow = Find(WIID_CASH_TRADE_USE_POINT_INC, true);
			if(pWindow)
				pWindow->Show();

			pWindow = NULL;
			pWindow = Find(WIID_CASH_TRADE_USE_POINT_DEC, true);
			if(pWindow)
				pWindow->Show();
		}
		else if(pCashItem->iMaxOZ == 0 && pCashItem->iMaxMile > 0) {
			pWindow = NULL;
			pWindow = Find(WIID_CASH_TRADE_USE_POINT_TEXTIMG, true);
			if(pWindow)
				pWindow->Show();

			pWindow = NULL;
			pWindow = Find(WIID_CASH_TRADE_USE_POINT, true);
			if(pWindow)
				pWindow->Show();

			pWindow = NULL;
			pWindow = Find(WIID_CASH_TRADE_USE_POINT_ICON, true);
			if(pWindow)
				pWindow->Show();

			pWindow = NULL;
			pWindow = Find(WIID_CASH_TRADE_USE_POINT_INC, true);
			if(pWindow)
				pWindow->Hide();

			pWindow = NULL;
			pWindow = Find(WIID_CASH_TRADE_USE_POINT_DEC, true);
			if(pWindow)
				pWindow->Hide();
		}	
		else {
			pWindow = NULL;
			pWindow = Find(WIID_CASH_TRADE_USE_POINT_TEXTIMG, true);
			if(pWindow)
				pWindow->Hide();

			pWindow = NULL;
			pWindow = Find(WIID_CASH_TRADE_USE_POINT, true);
			if(pWindow)
				pWindow->Hide();

			pWindow = NULL;
			pWindow = Find(WIID_CASH_TRADE_USE_POINT_ICON, true);
			if(pWindow)
				pWindow->Hide();

			pWindow = NULL;
			pWindow = Find(WIID_CASH_TRADE_USE_POINT_INC, true);
			if(pWindow)
				pWindow->Hide();

			pWindow = NULL;
			pWindow = Find(WIID_CASH_TRADE_USE_POINT_DEC, true);
			if(pWindow)
				pWindow->Hide();
		}
	}	

	if(SPLocalizeManager::GetInstance()->GetMileageRatio() == 0) {
		pWindow = NULL;
		pWindow = Find(WIID_CASH_TRADE_USE_POINT_TEXTIMG, true);
		if(pWindow)
			pWindow->Hide();

		pWindow = NULL;
		pWindow = Find(WIID_CASH_TRADE_USE_POINT, true);
		if(pWindow)
			pWindow->Hide();

		pWindow = NULL;
		pWindow = Find(WIID_CASH_TRADE_USE_POINT_ICON, true);
		if(pWindow)
			pWindow->Hide();

		pWindow = NULL;
		pWindow = Find(WIID_CASH_TRADE_USE_POINT_INC, true);
		if(pWindow)
			pWindow->Hide();

		pWindow = NULL;
		pWindow = Find(WIID_CASH_TRADE_USE_POINT_DEC, true);
		if(pWindow)
			pWindow->Hide();
	}
}


bool SPWindowCashTrade::UpdateControl()
{
	int iCurCash = g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValue(STATUS_CASH);
	int iCurPoint = g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValue(STATUS_MILE);
	
	
	SPCashItemAttr* pCashItem = SPCashTradeManager::GetInstance()->GetSelectCashItem();
	if(pCashItem && pCashItem->iCashID) {
		m_pItemName->SetWindowText(pCashItem->strName.c_str());

		if(pCashItem->iMaxOZ) {
			m_pCost->SetWindowText(ConvertMoneyToString(pCashItem->iMaxOZ));
			m_pCostIcon->SetImagePos(463, 113);
		}
		else if(pCashItem->iMaxMile) {
			m_pCost->SetWindowText(ConvertMoneyToString(pCashItem->iMaxMile));
			m_pCostIcon->SetImagePos(463, 125);
		}

		SPTexture* pTexture;
		POINT pt;
		m_pItemIcon->SPSendMessage(SPIM_SET_IMAGECLEAR);
		pTexture = g_pItemCluster->GetItemTextureFromTextureInx(pCashItem->iIcon, pCashItem->iIconIndex, pt);
		if( pTexture )
			m_pItemIcon->SetImage(pTexture, pt.x, pt.y);				
		
		m_pCurCash->SetWindowText(ConvertMoneyToString(iCurCash));		
		
		//m_iUseCash = 0;
		int iChangeCash = 0;
		int iChangePoint = 0;		
		if(pCashItem->iMaxOZ > 0 && pCashItem->iMaxMile > 0) {					//캐시와 포인트로 구매 가능
			m_iUseCash = pCashItem->iMaxOZ - m_iUsePoint;
			m_pUseCash->SetWindowText(ConvertMoneyToString(m_iUseCash));
			m_pUsePoint->SetWindowText(ConvertMoneyToString(m_iUsePoint));
		}
		else if(pCashItem->iMaxOZ == 0 && pCashItem->iMaxMile > 0) {			//포인트로만 구매 가능
			m_pUseCash->SetWindowText(ConvertMoneyToString(pCashItem->iMaxOZ));
			m_pUsePoint->SetWindowText(ConvertMoneyToString(m_iUsePoint));
		}
		else {																	//캐시로만 구매 가능			
			m_pUseCash->SetWindowText(ConvertMoneyToString(pCashItem->iMaxOZ));	
		}

		if(pCashItem->iMaxOZ > 0) {
			m_pPayCash->SetWindowText(ConvertMoneyToString(pCashItem->iMaxOZ));	
			m_pPayIcon->SetImagePos(463, 113);
			
			m_iChangeCash = iCurCash - (pCashItem->iMaxOZ - m_iUsePoint);
			if(m_iChangeCash < 0) {
				iChangeCash = 0;
				m_pHelp->SPSendMessage(SPIM_SET_IMAGEINDEX, 1);
			}
			else{ 
				iChangeCash = m_iChangeCash;
			}
			m_pChangeCash->SetWindowText(ConvertMoneyToString(iChangeCash));
			m_pChangeIcon->SetImagePos(463, 113);
		}
		else {
			m_pPayCash->SetWindowText(ConvertMoneyToString(pCashItem->iMaxMile));	
			m_pPayIcon->SetImagePos(463, 125);
			
			iChangePoint = iCurPoint - m_iUsePoint;
			m_pChangeCash->SetWindowText(ConvertMoneyToString(iChangePoint));
			m_pChangeIcon->SetImagePos(463, 125);
		}

		return true;
	}	

	return false;
}


void SPWindowCashTrade::Hide(bool bSendServer /*= true*/)
{
	SPWindow::Hide(bSendServer);
}


void SPWindowCashTrade::Process(float fTime)
{
	SPWindow::Process(fTime);
}


void SPWindowCashTrade::Render(float fTime)
{
	if(!m_bShow) 
		return;
	
	SPWindow::Render(fTime);
}



SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowCashTrade )

SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_CASH_TRADE_OK,				OnOk		)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_CASH_TRADE_CANCEL,			OnCancel	)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_CASH_TRADE_USE_POINT_INC,	OnPointInc	)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_CASH_TRADE_USE_POINT_DEC,	OnPointDec	)

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)


int SPWindowCashTrade::OnOk(WPARAM wParam, LPARAM lParam)
{	
	SPCashItemAttr* pCashItem = SPCashTradeManager::GetInstance()->GetSelectCashItem();
	if(pCashItem && pCashItem->iCashID) {
		if(pCashItem->iMaxOZ > 0) {					//캐시와 포인트로 구매 가능
			if(m_iChangeCash < 0) {					//충전 페이지 연결
				SPCashTradeManager::GetInstance()->PleaseBuyMoney();
				return 1;	
			}
		}		
		
		SPCashTradeManager::GetInstance()->SendCashShopBuy(CASH_TRADE_PLACE_FIELD, pCashItem->iCashID, 
			pCashItem->iPrice, pCashItem->iSalePrice, pCashItem->iMaxOZ, pCashItem->iMaxMile,
			m_iUseCash, m_iUsePoint);
	}	

	return 1;
}


int SPWindowCashTrade::OnCancel(WPARAM wParam, LPARAM lParam)
{
	Hide();
	return 1;
}


int SPWindowCashTrade::OnPointInc(WPARAM wParam, LPARAM lParam)
{	
	int iCurCash = g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValue(STATUS_CASH);
	int iCurPoint = g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValue(STATUS_MILE);	

	int iMaxPoint = (iCurPoint / m_iPointTradeUnit) * m_iPointTradeUnit;

	SPCashItemAttr* pCashItem = SPCashTradeManager::GetInstance()->GetSelectCashItem();
	if(pCashItem && pCashItem->iCashID) {
		if(pCashItem->iMaxOZ > 0 && pCashItem->iMaxMile > 0) {
			if(m_iUsePoint < iMaxPoint && m_iUsePoint < pCashItem->iMaxMile)
				m_iUsePoint += (m_iPointTradeUnit);

			UpdateControl();
		}
	}
	
	return 1;
}


int SPWindowCashTrade::OnPointDec(WPARAM wParam, LPARAM lParam)
{
	int iCurCash = g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValue(STATUS_CASH);
	int iCurPoint = g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValue(STATUS_MILE);

	SPCashItemAttr* pCashItem = SPCashTradeManager::GetInstance()->GetSelectCashItem();
	if(pCashItem && pCashItem->iCashID) {
		if(pCashItem->iMaxOZ > 0 && pCashItem->iMaxMile > 0) {
			if(m_iUsePoint >= m_iPointTradeUnit)
				m_iUsePoint -= (m_iPointTradeUnit);

			UpdateControl();
		}
	}
	
	return 1;
}

