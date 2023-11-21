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
#include "SPWindowEdit.h"
#include "SPWindowEditMultiLine.h"
#include "SPWindowSlider.h"
#include "SPWindowCashGift.h"

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

#include <WinSock2.h>
#include "Packet.h"
#include "PacketID.h"
#include "PacketHandler.h"
#include "SockWnd.h"
#include "SPNetworkManager.h"
#include "LT_Error.h"

#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPItem.h"
#include "SPItemCluster.h"			//[2005/6/16] - jinhee
//#include "SPPlayer.h"

#include "SPCashTradeManager.h"

#include "SPCheckManager.h"
#include "SPLocalizeManager.h"

#include "SPDebug.h"



SPWindowCashGift::SPWindowCashGift( INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent /*= NULL*/)
: SPWindow(WNDID_CASH_GIFT, InstanceID, iX, iY, iCX, iCY, pParent)
{
	Init();
}


SPWindowCashGift::~SPWindowCashGift()
{
	Clean();
}


void SPWindowCashGift::Init()
{
	InitSubControl();
}


void SPWindowCashGift::InitSubControl()
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
	SPWindowEdit*	pEdit		= NULL;
	SPWindowEditMultiLine* pMulti = NULL;

	//WIID_CASH_Gift_TOP
	pStatic = new SPWindowStatic(WIID_CASH_GIFT_TOP, 0, 0, 268, 8, this);
	pStatic->SetImage(CASH_SHOP2, 1, 28);
	pStatic->Show();

	//WIID_CASH_GIFT_CENTER
	int iCenterY = 8;
	pCenter = new SPWindowStatic(WIID_CASH_GIFT_CENTER, 0, 8, 268, 358, this);
	pCenter->SetImage(CASH_SHOP2, 1, 38);
	pCenter->SetSrcSize(268, 6);
	pCenter->Show();

	//WIID_CASH_GIFT_ITEMSKIN
	pStatic = new SPWindowStatic(WIID_CASH_GIFT_ITEMSKIN, 22, 42 - iCenterY, 35, 11, pCenter);
	pStatic->SetImage(CASH_SHOP1, 443, 137);
	pStatic->Show();

	//WIID_CASH_GIFT_ITEM	
	pStatic = new SPWindowStatic(WIID_CASH_GIFT_ITEM, 24, 18 - iCenterY, 32, 32, pCenter);	
	pStatic->Show();
	m_pItemIcon = pStatic;

	//WIID_CASH_GIFT_ITEMNAME
	pStatic = new SPWindowStatic(WIID_CASH_GIFT_ITEMNAME, 69, 20 - iCenterY, 175, 12, pCenter);		
	pStatic->SetFormat(DT_LEFT | DT_VCENTER);
	//pStatic->SetMultiLine(true, 2);
	pStatic->SetFont(FONT_12_BOLD);
	pStatic->SetFontColor(RGBA(49, 76, 97, 255));
	pStatic->SetWindowText("Use Item Name");
	pStatic->Show();
	m_pItemName = pStatic;

	//WIID_CASH_GIFT_COSTSKIN
	pStatic = new SPWindowStatic(WIID_CASH_GIFT_COSTSKIN, 70, 35 - iCenterY, 80, 14, pCenter);	
	pStatic->SetWindowType( WND_TYPE_LEFTBASE );
	pStatic->SetImageNormalHead(CASH_SHOP1, 493, 497, 495, 511);
	pStatic->SetImageNormalBody(CASH_SHOP1, 497, 497, 499, 511);
	pStatic->SetImageNormalTail(CASH_SHOP1, 501, 497, 503, 511);	
	pStatic->Show();

	//WIID_CASH_GIFT_COST	
	pStatic = new SPWindowStatic(WIID_CASH_GIFT_COST, 74, 37 - iCenterY, 53, 12, pCenter);
	pStatic->SetFormat(DT_RIGHT | DT_VCENTER);
	//pStatic->SetMultiLine(true, 2);
	pStatic->SetFont(FONT_12_NORMAL);
	pStatic->SetFontColor(RGBA(255, 255, 255, 255));
	pStatic->SetWindowText("111");
	pStatic->Show();
	m_pCost = pStatic;

	//WIID_CASH_GIFT_COSTICON
	pStatic = new SPWindowStatic(WIID_CASH_GIFT_COSTICON, 131, 37 - iCenterY, 15, 11, pCenter);
	pStatic->SetImage(CASH_SHOP1, 463, 113);
	pStatic->Show();
	m_pCostIcon = pStatic;
	
	{
	SPWindowStatic* pSendSkin = NULL; 
	int iSkinX = 2;
	int iSkinY = 66;
	//WIID_CASH_GIFT_SEND_SKIN
	pSendSkin = new SPWindowStatic(WIID_CASH_GIFT_SEND_SKIN, iSkinX, iSkinY - iCenterY, 261, 85, pCenter);
	pSendSkin->SetImage(CASH_SHOP2, 451, 121);
	pSendSkin->SetSrcSize(2, 2);
	pSendSkin->Show();	
	
	//WIID_CASH_GIFT_SEND_RECEIVER_IMG
	pStatic = new SPWindowStatic(WIID_CASH_GIFT_SEND_RECEIVER_IMG, 22 - iSkinX, 76 - iSkinY, 62, 13, pSendSkin);
	pStatic->SetImage(CASH_SHOP2, 450, 120);
	pStatic->Show();		

	//WIID_CASH_GIFT_SEND_RECEIVER_SKIN
	pStatic = new SPWindowStatic(WIID_CASH_GIFT_SEND_RECEIVER_SKIN, 91 - iSkinX, 74 - iSkinY, 129, 16, pSendSkin);
	pStatic->SetImage(CASH_SHOP2, 382, 1);
	pStatic->Show();

	//WIID_CASH_GIFT_SEND_RECEIVER
	//pEdit = new SPWindowEdit(WIID_CASH_GIFT_SEND_RECEIVER, 96 - iSkinX, 77 - iSkinY, 119, 12, pSendSkin);
	m_pReceiver = new SPWindowEdit(WIID_CASH_GIFT_SEND_RECEIVER, 96 - 91, 77 - 74, 119, 12, pStatic);
	m_pReceiver->SetWindowType(WND_TYPE_LEFTBASE);
	//m_pReceiver->SetMargin(2, 1);
	m_pReceiver->SetFontColor(RGBA(143, 99, 99, 255));
	m_pReceiver->SetFormat(DT_LEFT | DT_VCENTER);
	m_pReceiver->SetLimitText(LEN_NAME);
	m_pReceiver->Show();
	
	//WIID_CASH_GIFT_SEND_MSG_IMG
	pStatic = new SPWindowStatic(WIID_CASH_GIFT_SEND_MSG_IMG, 22 - iSkinX, 97 - iSkinY, 62, 13, pSendSkin);
	pStatic->SetImage(CASH_SHOP2, 450, 134);
	pStatic->Show();		

	//WIID_CASH_GIFT_SEND_MSG_SKIN
	pStatic = new SPWindowStatic(WIID_CASH_GIFT_SEND_MSG_SKIN, 91 - iSkinX, 95 - iSkinY, 129, 46, pSendSkin);
	pStatic->SetImage(CASH_SHOP2, 382, 18);
	pStatic->Show();

	//WIID_CASH_GIFT_SEND_MSG
	m_pMsg = new SPWindowEditMultiLine(WIID_CASH_GIFT_SEND_MSG, 96 - 91, 97 - 95, 119, 43, pStatic);
	//pMulti = new SPWindowEditMultiLine(WIID_CASH_GIFT_SEND_MSG, 96 - iSkinX, 97 - iSkinY, 119, 43, pSendSkin);
	m_pMsg->SetLimitText(MAX_MAIl_BODY_LEN);
	//pMulti->SetWindowType(WND_TYPE_LEFTBASE);
	//pMulti->SetMargin(2, 1);
	//pMulti->SetFontColor(RGBA(143, 99, 99, 255));
	//pMulti->SetFormat(DT_RIGHT | DT_VCENTER);
	m_pMsg->Show();

	//WIID_CASH_GIFT_SEND_FRIENDLIST_BTN
	pButton = new SPWindowButton(WIID_CASH_GIFT_SEND_FRIENDLIST_BTN, 224 - iSkinX, 74 - iSkinY, 35, 16, pSendSkin);
	pButton->SetImage		(CASH_SHOP2, 310,	18); //일반
	pButton->SetImageHit	(CASH_SHOP2, 346,	18); //on	
	pButton->SetImagePush	(CASH_SHOP2, 310,	35); //push
	pButton->SetImageDisable(CASH_SHOP2, 346,	35); //disable
	pButton->Show();
	}

	{
	//WIID_CASH_GIFT_FRIEND
	int iListX = 264;
	int iListY = 64;
	pStatic = new SPWindowStatic(WIID_CASH_GIFT_FRIEND, iListX, iListY - iCenterY, 185, 148, pCenter);
	pStatic->SetWindowType( WND_TYPE_TOPBASE );
	pStatic->SetImageNormalHead(CASH_SHOP2, 106, 55, 291, 63);
	pStatic->SetImageNormalBody(CASH_SHOP2, 106, 66, 291, 70);
	pStatic->SetImageNormalTail(CASH_SHOP2, 106, 73, 291, 81);	
	pStatic->Show();
	m_pFriendList = pStatic;

	SPWindowStatic* pListSkin = NULL;
	int iSkinX = 272;
	int iSkinY = 75;
	//WIID_CASH_GIFT_FRIEND_LIST
	pListSkin = new SPWindowStatic(WIID_CASH_GIFT_FRIEND_LIST, iSkinX - iListX, iSkinY - iListY, 164, 124, m_pFriendList);
	pListSkin->SetWindowType( WND_TYPE_TOPBASE );
	pListSkin->SetImageNormalHead(CASH_SHOP2, 106, 82, 270, 86);
	pListSkin->SetImageNormalBody(CASH_SHOP2, 106, 85, 270, 88);
	pListSkin->SetImageNormalTail(CASH_SHOP2, 106, 87, 270, 91);	
	pListSkin->Show();
	
	SPWindowStatic* pItemSkin = NULL;
	for(int i = 0; i < MAX_FRIEND_VIEW; i++) {
		//WIID_CASH_GIFT_FRIEND_LIST_SKIN
		pItemSkin = new SPWindowStatic(WIID_CASH_GIFT_FRIEND_LIST_SKIN + i, 273 - iSkinX, 80 + (19 * i) - iSkinY, 151, 19, pListSkin);		
		pItemSkin->Show();
		
		//WIID_CASH_GIFT_FRIEND_LIST_BTN
		//pButton = new SPWindowButton(WIID_CASH_GIFT_FRIEND_LIST_BTN + i, 273 - iSkinX, 80 + (19 * i) - iSkinY, 151, 19, pListSkin);
		pButton = new SPWindowButton(WIID_CASH_GIFT_FRIEND_LIST_BTN + i, 0, 0, 151, 19, pItemSkin);
		pButton->Show();

		//WIID_CASH_GIFT_FRIEND_LIST_SKIN
		pStatic = new SPWindowStatic(WIID_CASH_GIFT_FRIEND_LIST_SKIN + i, 0, 0, 151, 19, pItemSkin);
		pStatic->SetImage(CASH_SHOP2, 107, 65);
		pStatic->SetSrcSize(6, 6);
		pStatic->Show();		
		m_pFriendSkin[i] = pStatic;		

		//WIID_CASH_GIFT_FRIEND_LIST_TEXT
		pStatic = new SPWindowStatic(WIID_CASH_GIFT_FRIEND_LIST_TEXT + i, 12, 4, 127, 12, pItemSkin);
		//pStatic = new SPWindowStatic(WIID_CASH_GIFT_FRIEND_LIST_TEXT + i, 12, 4, 127, 12, m_pFriendSkin[i]);
		pStatic->SetFormat(DT_LEFT | DT_VCENTER);
		//pStatic->SetMultiLine(true, 2);
		pStatic->SetFont(FONT_12_NORMAL);
		pStatic->SetFontColor(RGBA(49, 76, 97, 255));
		pStatic->SetWindowText("111");
		pStatic->Show();
		m_pFriendName[i] = pStatic;
	}
	
	//WIID_CASH_GIFT_FRIEND_SCROOL_UP
	pButton = new SPWindowButton(WIID_CASH_GIFT_FRIEND_SCROOL_UP, 424 - iSkinX, 75 - iSkinY, 12, 11, pListSkin);
	pButton->SetImage		(CASH_SHOP1, 456,	389); //일반
	pButton->SetImageHit	(CASH_SHOP1, 469,	389); //on	
	pButton->SetImagePush	(CASH_SHOP1, 482,	389); //push
	pButton->SetImageDisable(CASH_SHOP1, 495,	389); //disable
	pButton->Show();
	
	//WIID_CASH_GIFT_FRIEND_SCROOL
	m_pSlider	=	new SPWindowSlider(WIID_CASH_GIFT_FRIEND_SCROOL, 424 - iSkinX, 86 - iSkinY, 12, 102, pListSkin );
	RECT rtRect;

	SetRect( &rtRect , 456 , 401 , 468 , 425 );	//	일반
	m_pSlider->SetImageHandle(CASH_SHOP1 , rtRect );

	SetRect( &rtRect , 469 , 401 , 481 , 425 );	//	마퓖E?오퉩E
	m_pSlider->SetImageHandleHit(CASH_SHOP1 , rtRect );

	SetRect( &rtRect , 482 , 401 , 494 , 425 );	//	마퓖E?클릭
	m_pSlider->SetImageHandlePush(CASH_SHOP1 , rtRect );

	SetRect( &rtRect , 495 , 401 , 507 , 425 );
	m_pSlider->SetImageHandleDisable(CASH_SHOP1 , rtRect );

	SetRect( &rtRect , 456 , 427 , 468 , 429 );	//	일반
	m_pSlider->SetImagePageNormal(CASH_SHOP1, rtRect );

	SetRect( &rtRect , 469 , 427 , 481 , 429 );	//	마퓖E?오퉩E
	m_pSlider->SetImagePageHit(CASH_SHOP1, rtRect );

	SetRect( &rtRect , 482 , 427 , 494 , 429 );	//	마퓖E?클릭
	m_pSlider->SetImagePageShadow(CASH_SHOP1, rtRect );

	SetRect( &rtRect , 495 , 427 , 507 , 429 );
	m_pSlider->SetImagePageDisable(CASH_SHOP1, rtRect );

	m_pSlider->SPSendMessage( SPIM_SLIDER_PAGESIZE , 1 );
	m_pSlider->SetCurpage( 0 );
	m_pSlider->SPSendMessage( SPIM_SET_MAXVALUE , 0 );

	//WIID_CASH_GIFT_FRIEND_SCROOL_DOWN	
	pButton = new SPWindowButton(WIID_CASH_GIFT_FRIEND_SCROOL_DOWN, 424 - iSkinX, 188 - iSkinY, 12, 11, pListSkin);
	pButton->SetImage		(CASH_SHOP1, 456,	431); //일반
	pButton->SetImageHit	(CASH_SHOP1, 469,	431); //on	
	pButton->SetImagePush	(CASH_SHOP1, 482,	431); //push
	pButton->SetImageDisable(CASH_SHOP1, 495,	431); //disable
	pButton->Show();
	}
	m_pFriendList->Hide();

	//WIID_CASH_GIFT_CUR_CASH_TEXTIMG
	pStatic = new SPWindowStatic(WIID_CASH_GIFT_CUR_CASH_TEXTIMG, 22, 158 - iCenterY, 96, 13, pCenter);
	pStatic->SetImage(CASH_SHOP3, 385, 185);
	pStatic->Show();

	//WIID_CASH_GIFT_CUR_CASH
	pStatic = new SPWindowStatic(WIID_CASH_GIFT_CUR_CASH, 151, 159 - iCenterY, 46, 12, pCenter);
	pStatic->SetFormat(DT_RIGHT | DT_VCENTER);
	//pStatic->SetMultiLine(true, 2);
	pStatic->SetFont(FONT_12_NORMAL);
	pStatic->SetFontColor(RGBA(49, 76, 97, 255));
	pStatic->SetWindowText("111");
	pStatic->Show();
	m_pCurCash = pStatic;

	//WIID_CASH_GIFT_CUR_CASH_ICON
	pStatic = new SPWindowStatic(WIID_CASH_GIFT_CUR_CASH_ICON, 201, 159 - iCenterY, 15, 11, pCenter);
	pStatic->SetImage(CASH_SHOP1, 463, 113);
	pStatic->Show();

	//WIID_CASH_GIFT_USE_CASH_TEXTIMG
	pStatic = new SPWindowStatic(WIID_CASH_GIFT_USE_CASH_TEXTIMG, 22, 177 - iCenterY, 96, 13, pCenter);
	pStatic->SetImage(CASH_SHOP2, 353, 106);
	pStatic->Show();

	//WIID_CASH_GIFT_USE_CASH
	pStatic = new SPWindowStatic(WIID_CASH_GIFT_USE_CASH, 151, 178 - iCenterY, 46, 12, pCenter);
	pStatic->SetFormat(DT_RIGHT | DT_VCENTER);
	//pStatic->SetMultiLine(true, 2);
	pStatic->SetFont(FONT_12_NORMAL);
	pStatic->SetFontColor(RGBA(49, 76, 97, 255));
	pStatic->SetWindowText("222");
	pStatic->Show();
	m_pUseCash = pStatic;

	//WIID_CASH_GIFT_USE_CASH_ICON
	pStatic = new SPWindowStatic(WIID_CASH_GIFT_USE_CASH_ICON, 201, 178 - iCenterY, 15, 11, pCenter);
	pStatic->SetImage(CASH_SHOP1, 463, 113);
	pStatic->Show();

	//WIID_CASH_GIFT_USE_POINT_TEXTIMG
	pStatic = new SPWindowStatic(WIID_CASH_GIFT_USE_POINT_TEXTIMG, 22, 196 - iCenterY, 96, 13, pCenter);
	pStatic->SetImage(CASH_SHOP2, 353, 134);
	pStatic->Show();

	//WIID_CASH_GIFT_USE_POINT
	pStatic = new SPWindowStatic(WIID_CASH_GIFT_USE_POINT, 151, 197 - iCenterY, 46, 12, pCenter);
	pStatic->SetFormat(DT_RIGHT | DT_VCENTER);
	//pStatic->SetMultiLine(true, 2);
	pStatic->SetFont(FONT_12_NORMAL);
	pStatic->SetFontColor(RGBA(49, 76, 97, 255));
	pStatic->SetWindowText("333");
	pStatic->Show();
	m_pUsePoint = pStatic;

	//WIID_CASH_GIFT_USE_POINT_ICON
	pStatic = new SPWindowStatic(WIID_CASH_GIFT_USE_POINT_ICON, 201, 197 - iCenterY, 15, 11, pCenter);
	pStatic->SetImage(CASH_SHOP1, 463, 125);
	pStatic->Show();

	//WIID_CASH_GIFT_USE_POINT_INC
	pButton = new SPWindowButton(WIID_CASH_GIFT_USE_POINT_INC, 225, 192 - iCenterY, 13, 9, pCenter);
	pButton->SetImage		(CASH_SHOP2, 481,	407); //일반
	pButton->SetImageHit	(CASH_SHOP2, 481,	417); //on	
	pButton->SetImagePush	(CASH_SHOP2, 481,	427); //push
	pButton->SetImageDisable(CASH_SHOP2, 481,	437); //disable
	pButton->Show();

	//WIID_CASH_GIFT_USE_POINT_DEC
	pButton = new SPWindowButton(WIID_CASH_GIFT_USE_POINT_DEC, 225, 204 - iCenterY, 13, 9, pCenter);
	pButton->SetImage		(CASH_SHOP2, 495,	407); //일반
	pButton->SetImageHit	(CASH_SHOP2, 495,	417); //on	
	pButton->SetImagePush	(CASH_SHOP2, 495,	427); //push
	pButton->SetImageDisable(CASH_SHOP2, 495,	437); //disable
	pButton->Show();

	//WIID_CASH_GIFT_PAY_TEXTIMG
	pStatic = new SPWindowStatic(WIID_CASH_GIFT_PAY_TEXTIMG, 22, 231 - iCenterY, 96, 13, pCenter);
	pStatic->SetImage(CASH_SHOP2, 353, 120);
	pStatic->Show();

	//WIID_CASH_GIFT_PAY
	pStatic = new SPWindowStatic(WIID_CASH_GIFT_PAY, 151, 232 - iCenterY, 46, 12, pCenter);
	pStatic->SetFormat(DT_RIGHT | DT_VCENTER);
	//pStatic->SetMultiLine(true, 2);
	pStatic->SetFont(FONT_12_NORMAL);
	pStatic->SetFontColor(RGBA(49, 76, 97, 255));
	pStatic->SetWindowText("444");
	pStatic->Show();
	m_pPayCash = pStatic;

	//WIID_CASH_GIFT_PAY_ICON
	pStatic = new SPWindowStatic(WIID_CASH_GIFT_PAY_ICON, 201, 232 - iCenterY, 15, 11, pCenter);
	pStatic->SetImage(CASH_SHOP1, 463, 113);
	pStatic->Show();
	m_pPayIcon = pStatic;

	//WIID_CASH_GIFT_CHANGE_TEXTIMG
	pStatic = new SPWindowStatic(WIID_CASH_GIFT_CHANGE_TEXTIMG, 22, 250 - iCenterY, 96, 13, pCenter);
	pStatic->SetImage(CASH_SHOP3, 241, 242);
	pStatic->Show();

	//WIID_CASH_GIFT_CHANGE
	pStatic = new SPWindowStatic(WIID_CASH_GIFT_CHANGE, 151, 251 - iCenterY, 46, 12, pCenter);
	pStatic->SetFormat(DT_RIGHT | DT_VCENTER);
	//pStatic->SetMultiLine(true, 2);
	pStatic->SetFont(FONT_12_NORMAL);
	pStatic->SetFontColor(RGBA(49, 76, 97, 255));
	pStatic->SetWindowText("555");
	pStatic->Show();
	m_pChangeCash = pStatic;

	//WIID_CASH_GIFT_CHANGE_ICON
	pStatic = new SPWindowStatic(WIID_CASH_GIFT_CHANGE_ICON, 201, 251 - iCenterY, 15, 11, pCenter);
	pStatic->SetImage(CASH_SHOP1, 463, 113);
	pStatic->Show();
	m_pChangeIcon = pStatic;

	//WIID_CASH_GIFT_HELP (SPIM_SET_IMAGEINDEX)
	pStatic = new SPWindowStatic(WIID_CASH_GIFT_HELP, 13, 278 - iCenterY, 239, 45, pCenter);	
	pStatic->SetImage(CASH_SHOP2, 261, 162);			//포인트 100 단위 거래
	pStatic->SetImage(CASH_SHOP3, 1, 210);				//일반적인 cash가 부족한 경우
	pStatic->SetImage(CASH_SHOP4, 1, 1);				//포인트로만 구매 하는 경우
	pStatic->Show();
	m_pHelp = pStatic;

	//WIID_CASH_GIFT_OK
	pButton = new SPWindowButton(WIID_CASH_GIFT_OK, 64, 329 - iCenterY, 66, 17, pCenter);
	pButton->SetImage		(CASH_SHOP2, 322,	280); //일반
	pButton->SetImageHit	(CASH_SHOP2, 322,	298); //on	
	pButton->SetImagePush	(CASH_SHOP2, 322,	316); //push
	pButton->SetImageDisable(CASH_SHOP2, 322,	334); //disable
	pButton->Show();

	//WIID_CASH_GIFT_CANCEL
	pButton = new SPWindowButton(WIID_CASH_GIFT_CANCEL, 141, 329 - iCenterY, 66, 17, pCenter);
	pButton->SetImage		(CASH_SHOP2, 389,	280); //일반
	pButton->SetImageHit	(CASH_SHOP2, 389,	298); //on	
	pButton->SetImagePush	(CASH_SHOP2, 389,	316); //push
	pButton->SetImageDisable(CASH_SHOP2, 389,	334); //disable
	pButton->Show();

	//WIID_CASH_GIFT_BOTTOM
	pStatic = new SPWindowStatic(WIID_CASH_GIFT_BOTTOM, 0, 366, 268, 8, this);
	pStatic->SetImage(CASH_SHOP2, 1, 46);
	pStatic->Show();
}


void SPWindowCashGift::Clean()
{
	SPWindow::Clean();
}


void SPWindowCashGift::Show(bool bWithChild /*= true*/)
{
	SPCashItemAttr* pCashItem = SPCashTradeManager::GetInstance()->GetSelectCashItem();
	if(pCashItem == NULL || pCashItem->iCashID == 0)
		return;	

	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return;

	SPWindow::Show(bWithChild);

	m_pReceiver->SetFocus();
	
	m_iCurPage = 0;	
	m_iMaxPage = g_pGOBManager->GetLocalPlayer()->GetFriendListCount();	
	if(m_iMaxPage == 0) {
		// 캐시샵에서 친구리스트를 사용하기 위해 친구 리스트 호출[2/13/2007 AJJIYA]
		CPacket	FriendPacket;
		FriendPacket.AddUINT32( FRIEND_CS_GETLIST );
		( g_pNetworkManager->GetPacketHandler() )->PostMsg( &FriendPacket );
	}

	if(m_iMaxPage > MAX_FRIEND_VIEW)
		m_iMaxPage -= MAX_FRIEND_VIEW;
	else 
		m_iMaxPage = 0;

	m_pSlider->SetCurpage(0);
	m_pSlider->SPSendMessage(SPIM_SET_MAXVALUE , m_iMaxPage);
	
	UpdateFriendList();

	m_pFriendList->Hide();

	m_pHelp->SPSendMessage(SPIM_SET_IMAGEINDEX, 0);

	int iCurPoint = g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValue(STATUS_MILE);	
	m_iPointTradeUnit = SPLocalizeManager::GetInstance()->GetMileageGlowUnit();

	m_iUseCash = pCashItem->iMaxOZ;
	m_iUsePoint = 0;
	if(pCashItem->iMaxOZ > 0 && pCashItem->iMaxMile > 0) {						//캐시와 포인트 로 구매가능		
		if(iCurPoint >= pCashItem->iMaxMile) {
			m_iUsePoint = pCashItem->iMaxMile;
		}
		else {
			m_iUsePoint = (iCurPoint / m_iPointTradeUnit) * m_iPointTradeUnit;			
		}		
	}
	else if(pCashItem->iMaxOZ == 0 && pCashItem->iMaxMile > 0) {			//포인트로만 구매 가능		
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

	UpdatePointControl(pCashItem);

	if(UpdateControl() == false)
		return;
}


void SPWindowCashGift::UpdatePointControl(SPCashItemAttr* pCashItem)
{	
	SPWindow* pWindow = NULL;

	if(pCashItem && pCashItem->iCashID) {
		if(pCashItem->iMaxOZ > 0 && pCashItem->iMaxMile > 0) {
			pWindow = NULL;
			pWindow = Find(WIID_CASH_GIFT_USE_POINT_TEXTIMG, true);
			if(pWindow)
				pWindow->Show();

			pWindow = NULL;
			pWindow = Find(WIID_CASH_GIFT_USE_POINT, true);
			if(pWindow)
				pWindow->Show();

			pWindow = NULL;
			pWindow = Find(WIID_CASH_GIFT_USE_POINT_ICON, true);
			if(pWindow)
				pWindow->Show();

			pWindow = NULL;
			pWindow = Find(WIID_CASH_GIFT_USE_POINT_INC, true);
			if(pWindow)
				pWindow->Show();

			pWindow = NULL;
			pWindow = Find(WIID_CASH_GIFT_USE_POINT_DEC, true);
			if(pWindow)
				pWindow->Show();
		}
		else if(pCashItem->iMaxOZ == 0 && pCashItem->iMaxMile > 0) {
			pWindow = NULL;
			pWindow = Find(WIID_CASH_GIFT_USE_POINT_TEXTIMG, true);
			if(pWindow)
				pWindow->Show();

			pWindow = NULL;
			pWindow = Find(WIID_CASH_GIFT_USE_POINT, true);
			if(pWindow)
				pWindow->Show();

			pWindow = NULL;
			pWindow = Find(WIID_CASH_GIFT_USE_POINT_ICON, true);
			if(pWindow)
				pWindow->Show();

			pWindow = NULL;
			pWindow = Find(WIID_CASH_GIFT_USE_POINT_INC, true);
			if(pWindow)
				pWindow->Hide();

			pWindow = NULL;
			pWindow = Find(WIID_CASH_GIFT_USE_POINT_DEC, true);
			if(pWindow)
				pWindow->Hide();
		}	
		else {
			pWindow = NULL;
			pWindow = Find(WIID_CASH_GIFT_USE_POINT_TEXTIMG, true);
			if(pWindow)
				pWindow->Hide();

			pWindow = NULL;
			pWindow = Find(WIID_CASH_GIFT_USE_POINT, true);
			if(pWindow)
				pWindow->Hide();

			pWindow = NULL;
			pWindow = Find(WIID_CASH_GIFT_USE_POINT_ICON, true);
			if(pWindow)
				pWindow->Hide();

			pWindow = NULL;
			pWindow = Find(WIID_CASH_GIFT_USE_POINT_INC, true);
			if(pWindow)
				pWindow->Hide();

			pWindow = NULL;
			pWindow = Find(WIID_CASH_GIFT_USE_POINT_DEC, true);
			if(pWindow)
				pWindow->Hide();
		}
	}	

	if(SPLocalizeManager::GetInstance()->GetMileageRatio() == 0) {
		pWindow = NULL;
		pWindow = Find(WIID_CASH_GIFT_USE_POINT_TEXTIMG, true);
		if(pWindow)
			pWindow->Hide();

		pWindow = NULL;
		pWindow = Find(WIID_CASH_GIFT_USE_POINT, true);
		if(pWindow)
			pWindow->Hide();

		pWindow = NULL;
		pWindow = Find(WIID_CASH_GIFT_USE_POINT_ICON, true);
		if(pWindow)
			pWindow->Hide();

		pWindow = NULL;
		pWindow = Find(WIID_CASH_GIFT_USE_POINT_INC, true);
		if(pWindow)
			pWindow->Hide();

		pWindow = NULL;
		pWindow = Find(WIID_CASH_GIFT_USE_POINT_DEC, true);
		if(pWindow)
			pWindow->Hide();
	}
}


bool SPWindowCashGift::UpdateControl()
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
			else {
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


void SPWindowCashGift::UpdateFriendList()
{
	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return;
	
	SPPlayer* pPlayer = g_pGOBManager->GetLocalPlayer();		

	FRIEND_INFO* pFriendInfo;
	int iIndex = 0;
	for(int i = 0; i < MAX_FRIEND_VIEW; i++ )
	{
		m_pFriendName[i]->SetWindowText(NULL);
		iIndex = i + m_iCurPage;
		pFriendInfo = pPlayer->GetFriendListMember(iIndex);
		if( pFriendInfo == NULL )
			continue;

		m_pFriendName[i]->SetWindowText(pFriendInfo->szCharName);
	}
}


void SPWindowCashGift::Hide(bool bSendServer /*= true*/)
{
	SPWindow::Hide(bSendServer);
}


void SPWindowCashGift::Process(float fTime)
{
	SPWindow::Process(fTime);
}


void SPWindowCashGift::Render(float fTime)
{
	if(!m_bShow) 
		return;
	SPWindow::Render(fTime);
}



SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowCashGift )

SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_CASH_GIFT_OK,					OnOk		)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_CASH_GIFT_CANCEL,				OnCancel	)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_CASH_GIFT_USE_POINT_INC,		OnPointInc	)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_CASH_GIFT_USE_POINT_DEC,		OnPointDec	)
SPIMESSAGE_CONTROL(	SPIM_SETFOCUS,			WIID_CASH_GIFT_SEND_RECEIVER,		OnEditSetFocus)
SPIMESSAGE_CONTROL(	SPIM_KILLFOCUS,			WIID_CASH_GIFT_SEND_RECEIVER,		OnEditKillFocus)
SPIMESSAGE_CONTROL(	SPIM_SETFOCUS,			WIID_CASH_GIFT_SEND_MSG,			OnEditSetFocus)
SPIMESSAGE_CONTROL(	SPIM_KILLFOCUS,			WIID_CASH_GIFT_SEND_MSG,			OnEditKillFocus)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_CASH_GIFT_SEND_FRIENDLIST_BTN,	OnToggleFriendList	)

SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_CASH_GIFT_FRIEND_SCROOL_UP,	OnButtonPageUp		)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_CASH_GIFT_FRIEND_SCROOL_DOWN,	OnButtonPageDown	)

SPIMESSAGE_COMMAND( SPIM_WHEEL_UP,									OnButtonPageUp		)
SPIMESSAGE_COMMAND( SPIM_WHEEL_DOWN,								OnButtonPageDown	)
SPIMESSAGE_CONTROL( SPIM_WHEEL_UP,			WIID_CASH_GIFT_FRIEND_LIST,			OnButtonPageUp		)
SPIMESSAGE_CONTROL( SPIM_WHEEL_DOWN,		WIID_CASH_GIFT_FRIEND_LIST,			OnButtonPageDown	)

SPIMESSAGE_CONTROL( SPIM_SLIDER_UP		,	WIID_CASH_GIFT_FRIEND_SCROOL,		OnSliderPageUp	)
SPIMESSAGE_CONTROL( SPIM_SLIDER_DOWN	,	WIID_CASH_GIFT_FRIEND_SCROOL,		OnSliderPageDown)
SPIMESSAGE_CONTROL( SPIM_SLIDER_PAGEUP,		WIID_CASH_GIFT_FRIEND_SCROOL,		OnSliderPageUp	)
SPIMESSAGE_CONTROL( SPIM_SLIDER_PAGEDOWN,	WIID_CASH_GIFT_FRIEND_SCROOL,		OnSliderPageDown)
SPIMESSAGE_CONTROL( SPIM_SET_CURVALUE,		WIID_CASH_GIFT_FRIEND_SCROOL,		OnSliderPageNum	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASH_GIFT_FRIEND_SCROOL,		OnSliderButtonUp)

SPIMESSAGE_CONTROL_RANGE(SPIM_CURSOR_ENTER, WIID_CASH_GIFT_FRIEND_LIST_BTN, WIID_CASH_GIFT_FRIEND_LIST_BTN + MAX_FRIEND_VIEW, OnMouseFriendList)
SPIMESSAGE_CONTROL_RANGE(SPIM_LBUTTON_UP,	WIID_CASH_GIFT_FRIEND_LIST_BTN, WIID_CASH_GIFT_FRIEND_LIST_BTN + MAX_FRIEND_VIEW, OnSelectFriendList)

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)



int SPWindowCashGift::OnOk(WPARAM wParam, LPARAM lParam)
{
	SPCashItemAttr* pCashItem = SPCashTradeManager::GetInstance()->GetSelectCashItem();
	if(pCashItem && pCashItem->iCashID) {
		if(pCashItem->iMaxOZ > 0) {					//캐시와 포인트로 구매 가능
			if(m_iChangeCash < 0) {					//충전 페이지 연결
				SPCashTradeManager::GetInstance()->PleaseBuyMoney();
				return 1;	
			}
		}

		std::string strName = m_pReceiver->GetWindowText();
		if(strName.empty()) {
			if(g_pResourceManager->GetGlobalString(9004002)) {
				std::string strMsg = g_pResourceManager->GetGlobalString(9004002);
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
			}
			return 1;
		}

		//자신에게 보내는지 확인
		Trim(strName);
		std::string strLocalName = g_pGOBManager->GetLocalPlayer()->GetStatus()->GetGOBName();
		if(strName == strLocalName) {
			if(g_pResourceManager->GetGlobalString(9004004)) {
				std::string strMsg = g_pResourceManager->GetGlobalString(9004004);
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
			}
			return 1;
		}

		std::string strMsg = m_pMsg->GetWindowText();
		//if(strMsg.empty())
		//	return 1;

		SPCashTradeManager::GetInstance()->SendCashShopPresent(CASH_TRADE_PLACE_FIELD, pCashItem->iCashID, 
			pCashItem->iPrice, pCashItem->iSalePrice, pCashItem->iMaxOZ, pCashItem->iMaxMile,
			strName, strMsg,
			m_iUseCash, m_iUsePoint);
	}	

	return 1;
}


int SPWindowCashGift::OnCancel(WPARAM wParam, LPARAM lParam)
{
	Hide();
	return 1;
}


int SPWindowCashGift::OnPointInc(WPARAM wParam, LPARAM lParam)
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


int SPWindowCashGift::OnPointDec(WPARAM wParam, LPARAM lParam)
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


int	SPWindowCashGift::OnEditSetFocus(WPARAM wParam, LPARAM lParam)
{
	g_pInterfaceManager->SetKeyInput( TRUE );

	return 1;
}


int	SPWindowCashGift::OnEditKillFocus(WPARAM wParam, LPARAM lParam)
{
	g_pInterfaceManager->SetKeyInput( FALSE );

	return 1;
}


int SPWindowCashGift::OnToggleFriendList(WPARAM wParam, LPARAM lParam)
{
	if(m_bShow){
		if(m_pFriendList->IsShow()) {
			m_pFriendList->Hide();
		}
		else {
			m_pFriendList->Show();		
			
			m_iMaxPage = g_pGOBManager->GetLocalPlayer()->GetFriendListCount();	
			if(m_iMaxPage > MAX_FRIEND_VIEW)
				m_iMaxPage -= MAX_FRIEND_VIEW;
			else 
				m_iMaxPage = 0;
			
			m_iCurPage = 0;
			m_pSlider->SetCurpage(0);
			m_pSlider->SPSendMessage(SPIM_SET_MAXVALUE , m_iMaxPage);
			
			UpdateFriendList();
			for(int i = 0; i < MAX_FRIEND_VIEW; i++) {
				m_pFriendSkin[i]->Hide();
			}
		}
	}
	return 1;
}


int SPWindowCashGift::OnMouseFriendList(unsigned int uiID, WPARAM wParam, LPARAM lParam )
{
	if(!m_bShow)
		return 1;

	//
	int iSelectItem = uiID - WIID_CASH_GIFT_FRIEND_LIST_BTN;	
	
	//int iListItem = iSelectItem + m_iCurPage;

	for(int i = 0; i < MAX_FRIEND_VIEW; i++) {
		if(i == iSelectItem)
			m_pFriendSkin[i]->Show();
		else 
			m_pFriendSkin[i]->Hide();
	}

	return 1;
}


int SPWindowCashGift::OnSelectFriendList(unsigned int uiID, WPARAM wParam, LPARAM lParam)
{
	if(!m_bShow)
		return 1;

	int iSelectItem = uiID - WIID_CASH_GIFT_FRIEND_LIST_BTN;
	
	std::string strName = m_pFriendName[iSelectItem]->GetWindowText();
	m_pReceiver->SetWindowText(strName.c_str());
	m_pFriendList->Hide();
	
	return 1;
}


int SPWindowCashGift::OnButtonPageUp( WPARAM wParam, LPARAM lParam )
{
	m_pSlider->SPSendMessage( SPIM_SLIDER_UP );
	return 1;
}


int SPWindowCashGift::OnButtonPageDown( WPARAM wParam, LPARAM lParam )
{
	m_pSlider->SPSendMessage( SPIM_SLIDER_DOWN );
	return 1;
}


int SPWindowCashGift::OnSliderPageUp( WPARAM wParam, LPARAM lParam )
{
	////DXUTOutputDebugString("ItemSliderPageUp[%d]\n", (int)wParam);
	//if(g_pGOBManager->GetLocalPlayer()) {
	//	g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_ITEM_PAGE_UP , 0);

	//	//SPWindow* pWindow = Find(WIID_ITEM_GRID);
	//	//pWindow->SPSendMessage(SPIM_TOOLTIP_MOVE, 0, MAKELONG(g_pInterfaceManager->GetCursorX(), g_pInterfaceManager->GetCursorY()));
	//	m_pGrid->SPSendMessage(SPIM_TOOLTIP_MOVE, 0, MAKELONG(g_pInterfaceManager->GetCursorX(), g_pInterfaceManager->GetCursorY()));
	//}

	if(m_iCurPage > 0)
		m_iCurPage--;

	UpdateFriendList();

	return 1;
}


int SPWindowCashGift::OnSliderPageDown( WPARAM wParam, LPARAM lParam )
{
	////DXUTOutputDebugString("ItemSliderPageDown[%d]\n", (int)wParam);
	//if(g_pGOBManager->GetLocalPlayer()) {
	//	g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_ITEM_PAGE_DOWN , 0);

	//	//SPWindow* pWindow = Find(WIID_ITEM_GRID);		
	//	//pWindow->SPSendMessage(SPIM_TOOLTIP_MOVE, 0, MAKELONG(g_pInterfaceManager->GetCursorX(), g_pInterfaceManager->GetCursorY()));
	//	m_pGrid->SPSendMessage(SPIM_TOOLTIP_MOVE, 0, MAKELONG(g_pInterfaceManager->GetCursorX(), g_pInterfaceManager->GetCursorY()));
	//}

	if(m_iCurPage < m_iMaxPage)
		m_iCurPage++;

	UpdateFriendList();
	return 1;
}


int SPWindowCashGift::OnSliderPageNum( WPARAM wParam, LPARAM lParam )
{
	//DXUTOutputDebugString("OnSliderPageNum[%d]\n", (int)wParam);
	//if(g_pGOBManager->GetLocalPlayer())
	//	g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_ITEM_SETPAGE , wParam);

	if(m_iCurPage >= 0 && m_iCurPage < m_iMaxPage) {
		m_iCurPage = static_cast<int>(wParam);
		UpdateFriendList();
	}

	return 1;
}


int SPWindowCashGift::OnSliderButtonUp( WPARAM wParam, LPARAM lParam )
{
	//if(g_pGOBManager->GetLocalPlayer())
	//	g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);

	return 1;
}

