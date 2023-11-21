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
#include "SPWindowCashTradeResult.h"

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



SPWindowCashTradeResult::SPWindowCashTradeResult( INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent /*= NULL*/)
: SPWindow(WNDID_CASH_TRADE_RESULT, InstanceID, iX, iY, iCX, iCY, pParent)
{
	Init();
}


SPWindowCashTradeResult::~SPWindowCashTradeResult()
{
	Clean();
}


void SPWindowCashTradeResult::Init()
{
	InitSubControl();
}


void SPWindowCashTradeResult::InitSubControl()
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

	//WIID_CASH_TRADE_RESULT_TOP
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_RESULT_TOP, 0, 0, 268, 8, this);
	pStatic->SetImage(CASH_SHOP2, 1, 28);
	pStatic->Show();

	//WIID_CASH_TRADE_RESULT_CENTER
	int iCenterY = 8;
	pCenter = new SPWindowStatic(WIID_CASH_TRADE_RESULT_CENTER, 0, 8, 268, 210, this);
	pCenter->SetImage(CASH_SHOP2, 1, 38);
	pCenter->SetSrcSize(268, 6);
	pCenter->Show();

	//WIID_CASH_TRADE_RESULT_ITEMSKIN
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_RESULT_ITEMSKIN, 22, 42 - iCenterY, 35, 11, pCenter);
	pStatic->SetImage(CASH_SHOP1, 443, 137);
	pStatic->Show();

	//WIID_CASH_TRADE_RESULT_ITEM	
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_RESULT_ITEM, 24, 18 - iCenterY, 32, 32, pCenter);	
	pStatic->Show();
	m_pItemIcon = pStatic;

	//WIID_CASH_TRADE_RESULT_ITEMNAME
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_RESULT_ITEMNAME, 69, 20 - iCenterY, 175, 12, pCenter);		
	pStatic->SetFormat(DT_LEFT | DT_VCENTER);
	//pStatic->SetMultiLine(true, 2);
	pStatic->SetFont(FONT_12_BOLD);
	pStatic->SetFontColor(RGBA(49, 76, 97, 255));
	pStatic->SetWindowText("Use Item Name");
	pStatic->Show();
	m_pItemName = pStatic;

	//WIID_CASH_TRADE_RESULT_COSTSKIN
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_RESULT_COSTSKIN, 70, 35 - iCenterY, 80, 14, pCenter);	
	pStatic->SetWindowType( WND_TYPE_LEFTBASE );
	pStatic->SetImageNormalHead(CASH_SHOP1, 493, 497, 495, 511);
	pStatic->SetImageNormalBody(CASH_SHOP1, 497, 497, 499, 511);
	pStatic->SetImageNormalTail(CASH_SHOP1, 501, 497, 503, 511);	
	pStatic->Show();

	//WIID_CASH_TRADE_RESULT_COST	
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_RESULT_COST, 74, 37 - iCenterY, 53, 12, pCenter);
	pStatic->SetFormat(DT_RIGHT | DT_VCENTER);
	//pStatic->SetMultiLine(true, 2);
	pStatic->SetFont(FONT_12_NORMAL);
	pStatic->SetFontColor(RGBA(49, 76, 97, 255));
	pStatic->SetWindowText("111");
	pStatic->Show();
	m_pCost = pStatic;

	//WIID_CASH_TRADE_RESULT_COSTICON
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_RESULT_COSTICON, 131, 37 - iCenterY, 15, 11, pCenter);
	pStatic->SetImage(CASH_SHOP1, 463, 113);
	pStatic->Show();
	m_pCostIcon = pStatic;

	//WIID_CASH_TRADE_RESULT_PAY_TEXTIMG
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_RESULT_PAY_TEXTIMG, 22, 76 - iCenterY, 96, 13, pCenter);
	pStatic->SetImage(CASH_SHOP3, 241, 228);
	pStatic->Show();

	//WIID_CASH_TRADE_RESULT_PAY
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_RESULT_PAY, 151, 77 - iCenterY, 46, 12, pCenter);
	pStatic->SetFormat(DT_RIGHT | DT_VCENTER);
	//pStatic->SetMultiLine(true, 2);
	pStatic->SetFont(FONT_12_NORMAL);
	pStatic->SetFontColor(RGBA(70, 140, 124, 255));
	pStatic->SetWindowText("111");
	pStatic->Show();
	m_pPayCash = pStatic;

	//WIID_CASH_TRADE_RESULT_PAY_ICON
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_RESULT_PAY_ICON, 201, 77 - iCenterY, 15, 11, pCenter);
	pStatic->SetImage(CASH_SHOP1, 463, 113);
	pStatic->Show();
	m_pPayIcon = pStatic;

	//WIID_CASH_TRADE_RESULT_CHANGE_TEXTIMG
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_RESULT_CHANGE_TEXTIMG, 22, 95 - iCenterY, 96, 13, pCenter);
	pStatic->SetImage(CASH_SHOP3, 241, 242);
	pStatic->Show();

	//WIID_CASH_TRADE_RESULT_CHANGE
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_RESULT_CHANGE, 151, 96 - iCenterY, 46, 12, pCenter);
	pStatic->SetFormat(DT_RIGHT | DT_VCENTER);
	//pStatic->SetMultiLine(true, 2);
	pStatic->SetFont(FONT_12_NORMAL);
	pStatic->SetFontColor(RGBA(70, 140, 124, 255));
	pStatic->SetWindowText("555");
	pStatic->Show();
	m_pChangeCash = pStatic;

	//WIID_CASH_TRADE_RESULT_CHANGE_ICON
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_RESULT_CHANGE_ICON, 201, 96 - iCenterY, 15, 11, pCenter);
	pStatic->SetImage(CASH_SHOP1, 463, 113);
	pStatic->Show();
	m_pChangeIcon = pStatic;

	//WIID_CASH_TRADE_RESULT_HELP (SPIM_SET_IMAGEINDEX)
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_RESULT_HELP, 13, 129 - iCenterY, 239, 45, pCenter);
	pStatic->SetFormat(DT_CENTER | DT_VCENTER);
	pStatic->SetMultiLine(true, 2);
	pStatic->SetFont(FONT_12_NORMAL);
	pStatic->SetFontColor(RGBA(195, 57, 57, 255));
	pStatic->SetWindowText("This is Help Window!!!!");
	pStatic->Show();
	m_pHelp = pStatic;

	//WIID_CASH_TRADE_RESULT_OK
	pButton = new SPWindowButton(WIID_CASH_TRADE_RESULT_OK, 98, 187 - iCenterY, 66, 17, pCenter);
	pButton->SetImage		(CASH_SHOP2, 322,	280); //일반
	pButton->SetImageHit	(CASH_SHOP2, 322,	298); //on	
	pButton->SetImagePush	(CASH_SHOP2, 322,	316); //push
	pButton->SetImageDisable(CASH_SHOP2, 322,	334); //disable
	pButton->Show();	

	//WIID_CASH_TRADE_RESULT_BOTTOM
	pStatic = new SPWindowStatic(WIID_CASH_TRADE_RESULT_BOTTOM, 0, 218, 268, 8, this);
	pStatic->SetImage(CASH_SHOP2, 1, 46);
	pStatic->Show();
}


void SPWindowCashTradeResult::Clean()
{
	SPWindow::Clean();
}


void SPWindowCashTradeResult::Show(bool bWithChild /*= true*/)
{
	SPWindow::Show(bWithChild);
	UpdateControl();
}


bool SPWindowCashTradeResult::UpdateControl()
{
	int iCurCash = g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValue(STATUS_CASH);
	int iCurPoint = g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValue(STATUS_MILE);

	SPCashItemAttr* pCashItem = NULL;
	
	SPCashTradeResultInfo* pTradeResult = NULL;

	pTradeResult = SPCashTradeManager::GetInstance()->GetCashTradeResult();
	if(pTradeResult == NULL || pTradeResult->m_iCashShopID == 0)
		return false;
	
	pCashItem = g_pItemCluster->GetMinorCashItem(pTradeResult->m_iCashShopID);
	if(pCashItem == NULL)
		return false;
	
	if(pCashItem && pCashItem->iCashID) {
		if(pTradeResult->m_strReceiver.empty()) {
			if(g_pResourceManager->GetGlobalString(9001013) != NULL) {
				std::string strMsg = g_pResourceManager->GetGlobalString(9001013);
				m_pHelp->SetWindowText(strMsg.c_str());
				//g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str());
			}
		}
		else {
			if(g_pResourceManager->GetGlobalString(9004005) != NULL) {
				char szTemp[256];
				ZeroMemory(szTemp, 256);
				sprintf(szTemp, g_pResourceManager->GetGlobalString(9004005), pTradeResult->m_strReceiver.c_str());
				m_pHelp->SetWindowText(szTemp);
			}
		}


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

		//m_pCurCash->SetWindowText(ConvertMoneyToString(iCurCash));
		//m_iUseCash = 0;
		//int iChangeCash = 0;
		//int iChangePoint = 0;		
		//if(pCashItem->iMaxOZ > 0 && pCashItem->iMaxMile > 0) {					//캐시와 포인트로 구매 가능
		//	m_iUseCash = pCashItem->iMaxOZ - m_iUsePoint;
		//	m_pUseCash->SetWindowText(ConvertMoneyToString(m_iUseCash));
		//	m_pUsePoint->SetWindowText(ConvertMoneyToString(m_iUsePoint));			
		//}
		//else if(pCashItem->iMaxOZ == 0 && pCashItem->iMaxMile > 0) {			//포인트로만 구매 가능
		//	m_pUseCash->SetWindowText(ConvertMoneyToString(pCashItem->iMaxMile));				
		//	m_pUsePoint->SetWindowText(ConvertMoneyToString(m_iUsePoint));
		//}
		//else {																	//캐시로만 구매 가능
		//	m_pUseCash->SetWindowText(ConvertMoneyToString(pCashItem->iMaxOZ));	
		//}

		if(pCashItem->iMaxOZ > 0) {
			m_pPayCash->SetWindowText(ConvertMoneyToString(pCashItem->iMaxOZ));	
			m_pPayIcon->SetImagePos(463, 113);
			
			m_pChangeCash->SetWindowText(ConvertMoneyToString(iCurCash));
			m_pChangeIcon->SetImagePos(463, 113);
		}
		else {
			m_pPayCash->SetWindowText(ConvertMoneyToString(pCashItem->iMaxMile));	
			m_pPayIcon->SetImagePos(463, 125);
			
			m_pChangeCash->SetWindowText(ConvertMoneyToString(iCurPoint));
			m_pChangeIcon->SetImagePos(463, 125);
		}

		return true;
	}	

	return false;
}


void SPWindowCashTradeResult::Hide(bool bSendServer /*= true*/)
{
	SPWindow::Hide(bSendServer);
}


void SPWindowCashTradeResult::Process(float fTime)
{
	SPWindow::Process(fTime);
}


void SPWindowCashTradeResult::Render(float fTime)
{
	if(!m_bShow) 
		return;
	
	SPWindow::Render(fTime);
}



SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowCashTradeResult )

SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_CASH_TRADE_RESULT_OK,				OnOk		)

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)



int SPWindowCashTradeResult::OnOk(WPARAM wParam, LPARAM lParam)
{
	Hide();	
	
	if(g_pGOBManager->GetLocalPlayer()){
		if(g_pGOBManager->GetLocalPlayer()->IsDead())
			g_pGOBManager->GetLocalPlayer()->SetDeadAction();
	}	

	return 1;
}

