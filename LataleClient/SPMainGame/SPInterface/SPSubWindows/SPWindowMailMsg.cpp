
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
#include "SPWindowMailMsg.h"

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
#include "SPMailAttr.h"
#include "SPMailArchive.h"
#include "SPMailManager.h"
#include "SPEventManager.h"

#include "SPCheckManager.h"

#include "SPDebug.h"

SPWindowMailMsg::SPWindowMailMsg(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_MAILMSG, InstanceID, iX, iY, iCX, iCY, pParent)
{	
	Init();
}


SPWindowMailMsg::~SPWindowMailMsg()
{
	Clean();
}


void SPWindowMailMsg::Init()
{
	std::string strTextureFile = "";
	
	strTextureFile = "DATA/INTERFACE/CONCEPT/UI100.PNG";
	g_pVideo->GetTextureMgr()->LoadTexture(strTextureFile.c_str(), &m_pDisable);

	strTextureFile = "DATA/INTERFACE/CONCEPT/UI103.PNG";
	g_pVideo->GetTextureMgr()->LoadTexture(strTextureFile.c_str(), &m_pCheck);

	SetRect(&m_rtSrcCenter, 302, 277, 509, 278);
	SetRect(&m_rtDestCenter, m_iAX, m_iAY + 21, m_iAX + 208, m_iAY + 21 + 279);	
	
	//SetRect(&m_rtSrcDisable, 434 , 37 , 436 , 39);
	SetRect(&m_rtMarkSrc[0], 479, 152, 511, 184);		//ICON_MARK_LOCK
	SetRect(&m_rtMarkSrc[1], 479, 185, 511, 217);		//ICON_MARK_FASHION
	SetRect(&m_rtMarkSrc[2], 479, 339, 511, 371);		//ICON_MARK_EVENT
	SetRect(&m_rtMarkSrc[3], 479, 372, 511, 404);		//ICON_MARK_FANCY

	SetRect(&m_rtSrcCheck, 212, 46, 230, 60);			//Mail Read Check
	SetRect(&m_rtSrcNCheck, 231, 46, 249, 60);
	
	InitSubWindow();
}


void SPWindowMailMsg::InitSubWindow()
{
	SPWindow* pStatic = NULL;
	SPWindowButton* pButton = NULL;	
	SPWindow* pParent = NULL;
	SPWindow* pTop = NULL;
	SPWindow* pCenter = NULL;
	SPWindow* pBottom = NULL;


	//Top
	pTop = new SPWindowStatic(WIID_MAILMSG_TOP, 0, 0, 350, 4, this);
	pTop->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 200);
	pTop->Show();

	//Center
	pCenter = new SPWindowStatic(WIID_MAILMSG_CENTER, 0, 4, 350, 194, this);
	pCenter->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 206);
	pCenter->SetSrcSize(350, 2);
	pCenter->Show();

	//bottom
	pBottom  = new SPWindowStatic(WIID_MAILMSG_BOTTOM, 0, 198, 350, 4, this);
	pBottom->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 210);
	pBottom->Show();

	//Read Check Icon
	m_pCheckWnd = new SPWindowStatic(WIID_MAILMSG_ICON, 8, 6, 18, 14, this);
	m_pCheckWnd->Show();
	
	//Title
	m_pTitle = new SPWindowStatic(WIID_MAILMSG_TITLE, 30, 9, 196, 12, this);
	m_pTitle->SetFontColor(RGBA(143, 99, 99, 255));
	m_pTitle->SetFont(FONT_12_BOLD);
	m_pTitle->SetFormat(DT_LEFT | DT_VCENTER);
	m_pTitle->Show();

	//Date
	m_pDate = new SPWindowStatic(WIID_MAILMSG_DATE, 230, 10, 91, 12, this);
	m_pDate->SetFontColor(RGBA(143, 99, 99, 255));
	m_pDate->SetFormat(DT_RIGHT | DT_VCENTER);
	m_pDate->Show();

	//Close
	pButton = new SPWindowButton(WIID_MAILMSG_CLOSE, 329, 9 - 4, 12, 12, pCenter);
	pButton->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 301,	321);		//normal	
	pButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG", 314,	321);	//on
	pButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG", 327,	321);	//push
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG", 340,	321); //disable
	pButton->Show();

	//Base Line
	pStatic = new SPWindowStatic(WIID_MAILMSG_BASELINE, 2, 26, 346, 1, this);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 493, 494);
	pStatic->SetSrcSize(6, 1);
	pStatic->Show();
	pStatic = NULL;	

	//Msg Base
	pParent = new SPWindowStatic(WIID_MAILMSG_BASE, 2, 27 - 4, 346, 171, pCenter);
	pParent->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 493, 477);
	pParent->SetSrcSize(6, 4);
	pParent->Show();	

	//Title Base
	//pStatic = new SPWindowStatic(WIID_MAILMSG_TITLEBASE, 63, 48 - 24 - 21 , 138, 15, pParent);
	//pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI010.PNG", 157, 262);
	//pStatic->Show();	
	

	//Sender Base
	pStatic = new SPWindowStatic(WIID_MAILMSG_SENDERBASE, 10 - 2, 34 - 27, 60, 17, pParent);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 437, 43);
	pStatic->Show();
	pStatic = NULL;

	//첨부 이미지
	pStatic = new SPWindowStatic(WIID_MAILMSG_APPENDBASE, 10 - 2, 54 - 27, 60, 30, pParent);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 156, 134);
	pStatic->Show();
	pStatic = NULL;

	//Sender
	m_pSender = new SPWindowStatic(WIID_MAILMSG_SENDER, 78 - 2, 37 - 27, 263, 12, pParent);
	m_pSender->SetFontColor(RGBA(143, 99, 99, 255));
	m_pSender->SetFormat(DT_LEFT | DT_VCENTER);
	//m_pSender->SetWindowText("보낸 사람");
	m_pSender->Show();
	pStatic = NULL;

	//MAIL_TYPE_NULL Msg
	m_pMsg = new SPWindowStatic(WIID_MAILMSG_MSG, 25 - 2, 100 - 27, 301, 66, pParent);
	m_pMsg->SetFormat(DT_LEFT | DT_TOP | DT_WORDBREAK);
	m_pMsg->SetFontColor(RGBA(0, 0, 0, 255));
	m_pMsg->SetMultiLine(true, 4);
	m_pMsg->Show();


	//*******************************************************************************
	// MAIL_TYPE_AUCTION Msg 추가사항
	//*******************************************************************************
	m_pAuctionMsg = new SPWindowStatic(WIID_MAILMSG_AUCTION_MSG, 6, 89, 338, 86, this);
	m_pAuctionMsg->SetShowEnable(false) ;
	m_pAuctionMsg->Hide() ;

	pStatic = new SPWindowStatic(WIID_MAILMSG_AUCTION_TITLE, 0, 6, 138, 12, m_pAuctionMsg);
	pStatic->SetFormat(DT_RIGHT | DT_TOP );
	pStatic->SetFont(FONT_12_NORMAL) ;
	pStatic->SetFontColor(RGBA(143, 99, 99, 255));
	//pStatic->SetWindowText("물품 : ") ;

	pStatic = new SPWindowStatic(WIID_MAILMSG_AUCTION_TITLE_ITEM, 142, 6, 176, 12, m_pAuctionMsg);
	pStatic->SetFontColor(RGBA(143, 99, 99, 255));
	//pStatic->SetWindowText("아이템 이름...") ;

	pStatic = new SPWindowStatic(WIID_MAILMSG_AUCTION_TITLE_MONEY, 214, 6, 83, 12, m_pAuctionMsg);
	pStatic->SetFormat(DT_RIGHT | DT_TOP );
	//pStatic->SetWindowText("777777777") ;

	pStatic = new SPWindowStatic(WIID_MAILMSG_AUCTION_TITLE_ICON, 301, 6, 17, 11, m_pAuctionMsg);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 157, 22);		// Ely
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 193, 22);		// Cash
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 211, 22);		// Point

	pStatic = new SPWindowStatic(WIID_MAILMSG_AUCTION_TITLE_STRING, 0, 6, 338, 12, m_pAuctionMsg);
	pStatic->SetFormat(DT_CENTER | DT_TOP );
	pStatic->SetFontColor(RGBA(143, 99, 99, 255));
	//pStatic->SetWindowText("아이템 이름...") ;

	pStatic = new SPWindowStatic(WIID_MAILMSG_AUCTION_SELL		, 0, 34, 138, 12, m_pAuctionMsg);
	pStatic->SetFontColor(RGBA(143, 99, 99, 255));
	pStatic->SetFormat(DT_RIGHT | DT_TOP );
	//pStatic->SetWindowText("등록 금액 : ") ;

	pStatic = new SPWindowStatic(WIID_MAILMSG_AUCTION_SELL_TEXT	, 214, 34, 83, 12, m_pAuctionMsg);
	pStatic->SetFontColor(RGBA(143, 99, 99, 255));
	pStatic->SetFormat(DT_RIGHT | DT_TOP );
	//pStatic->SetWindowText("999999999") ;

	pStatic = new SPWindowStatic(WIID_MAILMSG_AUCTION_SELL_ICON	, 301, 34, 17, 11, m_pAuctionMsg);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 157, 22);		// Ely
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 193, 22);		// Cash
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 211, 22);		// Point

	pStatic = new SPWindowStatic(WIID_MAILMSG_AUCTION_FEE		, 0, 49, 138, 12, m_pAuctionMsg);
	pStatic->SetFontColor(RGBA(143, 99, 99, 255));
	pStatic->SetFormat(DT_RIGHT | DT_TOP );
	// pStatic->SetWindowText("수수료 : ") ;

	pStatic = new SPWindowStatic(WIID_MAILMSG_AUCTION_FEE_TEXT	, 214, 49, 83, 12, m_pAuctionMsg);
	pStatic->SetFontColor(RGBA(197, 57, 57, 255));
	pStatic->SetFormat(DT_RIGHT | DT_TOP );
	// pStatic->SetWindowText("999999999") ;

	pStatic = new SPWindowStatic(WIID_MAILMSG_AUCTION_FEE_ICON	, 301, 49, 17, 11, m_pAuctionMsg);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 157, 22);		// Ely
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 193, 22);		// Cash
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 211, 22);		// Point

	pStatic = new SPWindowStatic(WIID_MAILMSG_AUCTION_TOTAL		, 0, 69, 138, 12, m_pAuctionMsg);
	pStatic->SetFontColor(RGBA(143, 99, 99, 255));
	pStatic->SetFormat(DT_RIGHT | DT_TOP );
	// pStatic->SetWindowText("받는금액 : ") ;

	pStatic = new SPWindowStatic(WIID_MAILMSG_AUCTION_TOTAL_TEXT, 199, 69, 98, 12, m_pAuctionMsg);
	pStatic->SetFont(FONT_12_BOLD) ;
	pStatic->SetFontColor(RGBA(143, 99, 99, 255));
	pStatic->SetFormat(DT_RIGHT | DT_TOP );
	// pStatic->SetWindowText("999999999") ;

	pStatic = new SPWindowStatic(WIID_MAILMSG_AUCTION_TOTAL_ICON, 301, 69, 17, 11, m_pAuctionMsg);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 157, 22);		// Ely
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 193, 22);		// Cash
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 211, 22);		// Point

	pStatic = new SPWindowStatic(WIID_MAILMSG_AUCTION_LINE		, 76, 64, 246, 1, m_pAuctionMsg);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_002.PNG", 22, 23);		// Line
	pStatic->SetSrcSize(6, 1) ;
	//*******************************************************************************

	//Item Button
	m_pItemButton = new SPWindowButton(WIID_MAILMSG_ITEM, 77 - 2, 53 - 27, 32, 32, pParent);
	m_pItemButton->Show();

	//Item Icon 테두리
	pStatic = new SPWindowStatic(WIID_MAILMSG_ITEMICON_LINE, 77 - 2, 53 - 27, 32, 32, pParent);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 203, 375);
	pStatic->Show();
	pStatic = NULL;

	//Item Icon
	m_pItemIcon = new SPWindowStatic(WIID_MAILMSG_ITEMICON, 77 - 2, 53 - 27, 32, 32, pParent);
	//m_pItemIcon->SetWindowText("ItemIcon");
	m_pItemIcon->Show();
	m_pItemIcon->GetWindowRect(m_rtItemStack);
	m_iItemCount = 0;
	m_iItemMark = ICON_MARK_NULL;

	//Item Name
	m_pItemName = new SPWindowStatic(WIID_MAILMSG_ITEMNAME, 117 - 2, 56 - 27, 223, 12, pParent);
	m_pItemName->SetFontColor(RGBA(143, 99, 99, 255));
	m_pItemName->SetFormat(DT_LEFT | DT_VCENTER);
	//m_pItemName->SetWindowText("ItemName");
	m_pItemName->Show();	

	//ely
	m_pEly = new SPWindowStatic(WIID_MAILMSG_ELY, 117 - 2, 71 - 27, 62, 12, pParent);
	m_pEly->SetWindowText("Ely...");
	m_pEly->SetFontColor(RGBA(143, 99, 99, 255));
	m_pEly->SetFormat(DT_LEFT | DT_VCENTER);
	m_pEly->Show();

	//ely Image
	pStatic = new SPWindowStatic(WIID_MAILMSG_ELY_IMG, 180 - 2, 75 - 27, 17, 5, pParent);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 494,	506);
	pStatic->Show();
	pStatic = NULL;

	//Recevie
	m_pReceive = new SPWindowButton(WIID_MAILMSG_RECEIVE, 200 - 2, 179 - 27, 68, 17, pParent);
	m_pReceive->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 279,	373);		//normal	
	m_pReceive->SetImageHit("DATA/INTERFACE/CONCEPT/UI101.PNG", 279,	392);	//on
	m_pReceive->SetImagePush("DATA/INTERFACE/CONCEPT/UI101.PNG", 279,	411);	//push
	m_pReceive->SetImageDisable("DATA/INTERFACE/CONCEPT/UI101.PNG", 279,	430); //disable
	m_pReceive->Show();

	//Delete
	m_pDelete = new SPWindowButton(WIID_MAILMSG_DELETE, 272 - 2, 179 - 27, 68, 17, pParent);
	m_pDelete->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 209,	373);		//normal	
	m_pDelete->SetImageHit("DATA/INTERFACE/CONCEPT/UI101.PNG", 209,	392);	//on
	m_pDelete->SetImagePush("DATA/INTERFACE/CONCEPT/UI101.PNG", 209,	411);	//push
	m_pDelete->SetImageDisable("DATA/INTERFACE/CONCEPT/UI101.PNG", 209,	430); //disable
	m_pDelete->Show();
}


void SPWindowMailMsg::Clean()
{
	m_pCheckWnd->SPSendMessage(SPIM_SET_IMAGECLEAR);
	
	SAFE_RELEASE(m_pDisable);	
	SAFE_RELEASE(m_pCheck);
	SPWindow::Clean();
}


void SPWindowMailMsg::Process(float fTime)
{
	SPWindow::Process(fTime);
}


void SPWindowMailMsg::Render(float fTime)
{
	if(IsShow() == false) return;
	
	SPWindow::Render(fTime);

	if(m_pDisable) {
		RECT rtDest;
		m_pItemIcon->GetWindowRect(rtDest);
		//if(m_bAble[i] == false) {		
		//	m_pDisable->RenderTexture(&rtDest, &m_rtSrcDisable);
		//}
		if(m_iItemMark > ICON_MARK_NULL) {
			m_pDisable->RenderTexture(&rtDest, &m_rtMarkSrc[m_iItemMark - 1]);
		}
	}

	if(m_iItemCount > 9) {
		RECT rtTenNumTarget, rtNumTarget, rtTenNumSrc, rtNumSrc;
		int iTenStack = m_iItemCount / 10;
		int iOneStack = m_iItemCount % 10;

		SetRect(&rtTenNumTarget, m_rtItemStack.left, m_rtItemStack.bottom - 10, m_rtItemStack.left + 9, m_rtItemStack.bottom);
		SetRect(&rtNumTarget, rtTenNumTarget.right - 2, m_rtItemStack.bottom - 10, rtTenNumTarget.right + 9 - 2, m_rtItemStack.bottom);

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
	else if(m_iItemCount > 1) {
		int iOneStack = m_iItemCount % 10;
		RECT rtNumTarget, rtNumSrc;
		SetRect(&rtNumTarget, m_rtItemStack.left, m_rtItemStack.bottom - 10, m_rtItemStack.left + 9, m_rtItemStack.bottom);

		rtNumSrc.left = iOneStack * 9;
		rtNumSrc.right = rtNumSrc.left + 9;
		rtNumSrc.top = 0;
		rtNumSrc.bottom = 10;

		g_pItemCluster->GetNumTexture()->RenderTexture( &rtNumTarget , &rtNumSrc);
	}
}


void SPWindowMailMsg::Show(bool bWithChild/* = true*/)
{
	SPWindow::Show(bWithChild);
}


/**
	Auction 메일의 경우 별도 처리를 하도록 한다.
	m_strMemo에 데이터를 넣는다.
	AUCTION_TYPE_등록금액_수수료_받는금액_판매아이템_판매금액
	@param SPMailAttr* pMailAttr : 내용에서
 */
void SPWindowMailMsg::SetAuctionMsg(SPMailAttr* pMailAttr)
{
	int		iNOArg = 0 ;
	int		iAuctionType = 0 ;
	INT64	iRegMoney = 0 ;
	INT64	iFeeMoney = 0 ;
	INT64	iGetMoney = 0 ;
	int		iSaleItemID ;
	INT64	iSaleMoney ;
	std::vector<std::string> vstrArg;

	int iTitleIcon = 0 ;		// 0:Ely, 1:Cash, 2:Mile
	int iMoneyIcon = 0 ;		// 0:Ely, 1:Cash, 2:Mile

	if( (iNOArg = GetSeperateString(pMailAttr->m_strMemo, vstrArg, '_') ) < 6 )
		return ;

	iAuctionType = atoi(vstrArg[0].c_str()) ;
	iRegMoney	= _atoi64(vstrArg[1].c_str()) ;
	iFeeMoney	= _atoi64(vstrArg[2].c_str()) ;
	iGetMoney	= _atoi64(vstrArg[3].c_str()) ;
	iSaleItemID = atoi(vstrArg[4].c_str()) ;
	iSaleMoney	= _atoi64(vstrArg[5].c_str()) ;

	// AuctionType 구분
	if( iAuctionType == AUCTION_TYPE_ITEM_BY_ELY )	{
		iTitleIcon = 0 ;
		iMoneyIcon = 0 ;
	}
	else if( iAuctionType == AUCTION_TYPE_CASH_BY_ELY )		{
		iTitleIcon = 1 ;
		iMoneyIcon = 0 ;
	}
	else if( iAuctionType == AUCTION_TYPE_MILEAGE_BY_ELY )	{
		iTitleIcon = 2 ;
		iMoneyIcon = 0 ;
	}
	else if( iAuctionType == AUCTION_TYPE_ELY_BY_CASH )	{
		iTitleIcon = 0 ;
		iMoneyIcon = 1 ;
	}
	else if( iAuctionType == AUCTION_TYPE_ELY_BY_MILEAGE )	{
		iTitleIcon = 0 ;
		iMoneyIcon = 2 ;
	}

	// 경매 유형 구분
	if( pMailAttr->m_iMailType == MAIL_TYPE_AUCTION_SELL )	{
		if( iAuctionType == AUCTION_TYPE_ITEM_BY_ELY )	{
			SetAuctionMsgTitle(true, iSaleItemID) ;
			SetAuctionMsgMoney(iRegMoney, iFeeMoney, iGetMoney, iMoneyIcon) ;		// 판매자(판매완료)
		}
		else	{
			SetAuctionMsgTitle(true, iSaleMoney, iTitleIcon) ;
			SetAuctionMsgMoney(iRegMoney, iFeeMoney, iGetMoney, iMoneyIcon, true) ;		// 판매자(판매완료)
		}
	}
	else if( pMailAttr->m_iMailType == MAIL_TYPE_AUCTION_BUY )	{
		if( iAuctionType == AUCTION_TYPE_ITEM_BY_ELY )	{
			SetAuctionMsgTitle(false, iSaleItemID, pMailAttr) ;
			SetAuctionMsgMoney(iRegMoney, iFeeMoney, iGetMoney, iTitleIcon) ;		// 입찰자(낙찰)
		}
		else	{
			SetAuctionMsgTitle(false, iSaleMoney, iTitleIcon) ;
			SetAuctionMsgMoney(iRegMoney, iFeeMoney, iGetMoney, iTitleIcon, true) ;		// 입찰자(낙찰)
		}
	}
	else if( pMailAttr->m_iMailType == MAIL_TYPE_AUCTION_OUTBID )	{
		SetAuctionMsgTitle((GLOBAL_STRING_ID)52000007) ;
		SetAuctionMsgMoney(iRegMoney, iFeeMoney, iGetMoney, iMoneyIcon) ;		// 이전입찰자
	}
	else if( pMailAttr->m_iMailType == MAIL_TYPE_AUCTION_CANCEL_BIDDER )	{
		SetAuctionMsgTitle((GLOBAL_STRING_ID)52000084) ;
		SetAuctionMsgMoney(iRegMoney, iFeeMoney, iGetMoney, iMoneyIcon) ;		// 입찰자
	}
	else if( pMailAttr->m_iMailType == MAIL_TYPE_AUCTION_CANCEL_SELLER )	{
		SetAuctionMsgTitle((GLOBAL_STRING_ID)52000006) ;
		SetAuctionMsgMoney(iRegMoney, iFeeMoney, iGetMoney, iTitleIcon) ;		// 판매자
	}
	else if( pMailAttr->m_iMailType == MAIL_TYPE_AUCTION_EXPIRED )	{
		SetAuctionMsgTitle((GLOBAL_STRING_ID)52000020) ;
		SetAuctionMsgMoney(iRegMoney, iFeeMoney, iGetMoney, iTitleIcon) ;		// 판매자
	}
}


/**
	내용에 실제 들어가는 등록금액, 수수료, 받는 금액 출력
	@param INT64 iValue1 : 등록금액, 1보다 작으면 Invisible
	@param INT64 iValue2 : 수수료
	@param INT64 iValue3 : 받는금액, 1보다 작으면 Invisible
	@param int iIconType : 0(Ely), 1(Cash), 2(Mili)
	@param bool bComplete : 낙찰, 판매 완료만 판매금액과 수수료를 보여준다.
 */
void SPWindowMailMsg::SetAuctionMsgMoney(INT64 iValue1, INT64 iValue2, INT64 iValue3, int iIconType, bool bComplete/*=false*/)
{
	SPWindow* pWindow = NULL ;
	// 등록금액이 없으면 보여주지 않는다.
	if( bComplete == false )	{
		pWindow = Find(WIID_MAILMSG_AUCTION_SELL, true) ;
		pWindow->Hide() ;
		pWindow = Find(WIID_MAILMSG_AUCTION_SELL_TEXT, true) ;
		pWindow->Hide() ;
		pWindow = Find(WIID_MAILMSG_AUCTION_SELL_ICON, true) ;
		pWindow->Hide() ;
		pWindow = Find(WIID_MAILMSG_AUCTION_FEE, true) ;
		pWindow->Hide() ;
		pWindow = Find(WIID_MAILMSG_AUCTION_FEE_TEXT, true) ;
		pWindow->Hide() ;
		pWindow = Find(WIID_MAILMSG_AUCTION_FEE_ICON, true) ;
		pWindow->Hide() ;
		pWindow = Find(WIID_MAILMSG_AUCTION_LINE, true) ;
		pWindow->Hide() ;
	}
	else	{
		pWindow = Find(WIID_MAILMSG_AUCTION_SELL, true) ;
		pWindow->SetWindowText(g_pResourceManager->GetGlobalString(52000057)) ;		// 등록금액
		pWindow->Show() ;
		pWindow = Find(WIID_MAILMSG_AUCTION_SELL_TEXT, true) ;
		pWindow->SetWindowText(ConvertMoneyToString(iValue1)) ;
		pWindow->Show() ;
		pWindow = Find(WIID_MAILMSG_AUCTION_SELL_ICON, true) ;
		pWindow->SPSendMessage(SPIM_SET_IMAGEINDEX, iIconType) ;
		pWindow->Show() ;
		pWindow = Find(WIID_MAILMSG_AUCTION_FEE, true) ;
		pWindow->SetWindowText(g_pResourceManager->GetGlobalString(52000025)) ;		// 수수료
		pWindow->Show() ;
		pWindow = Find(WIID_MAILMSG_AUCTION_FEE_TEXT, true) ;
		pWindow->SetWindowText(ConvertMoneyToString(iValue2)) ;
		pWindow->Show() ;
		pWindow = Find(WIID_MAILMSG_AUCTION_FEE_ICON, true) ;
		pWindow->SPSendMessage(SPIM_SET_IMAGEINDEX, iIconType) ;
		pWindow->Show() ;
		pWindow = Find(WIID_MAILMSG_AUCTION_LINE, true) ;
		pWindow->Show() ;
	}

	// 받는 금액도 없는 경우가 있다.
	if( iValue3 < 1 ) {
		pWindow = Find(WIID_MAILMSG_AUCTION_TOTAL, true) ;
		pWindow->Hide() ;
		pWindow = Find(WIID_MAILMSG_AUCTION_TOTAL_TEXT, true) ;
		pWindow->Hide() ;
		pWindow = Find(WIID_MAILMSG_AUCTION_TOTAL_ICON, true) ;
		pWindow->Hide() ;
	}
	else	{
		pWindow = Find(WIID_MAILMSG_AUCTION_TOTAL, true) ;
		pWindow->SetWindowText(g_pResourceManager->GetGlobalString(52000026)) ;		// 받는금액
		pWindow = Find(WIID_MAILMSG_AUCTION_TOTAL_TEXT, true) ;
		pWindow->SetWindowText(ConvertMoneyToString(iValue3)) ;
		pWindow = Find(WIID_MAILMSG_AUCTION_TOTAL_ICON, true) ;
		pWindow->SPSendMessage(SPIM_SET_IMAGEINDEX, iIconType) ;
	}
}


/**
	메일 내용 타이틀에 아이템을 셋팅해야 하는 경우
	판매자는 검정색으로 아이템 이름만 표시
	구매자는 아이템에 맞는 색으로 이름 표시
	@param bool bSell : true(판매), false(구매)
	@param int iItemID : 판매자의 경우 ItemID만 있음
	@param SPMailAttr* pMailAttr : 구매자의 경우 MailAttr을 통해서 색이있는 아이템 이름 출력
 */
void SPWindowMailMsg::SetAuctionMsgTitle(bool bSell, int iItemID, SPMailAttr* pMailAttr/*=NULL*/)
{
	SPWindow* pWindow = NULL ;
	pWindow = Find(WIID_MAILMSG_AUCTION_TITLE, true) ;
	if( pWindow ) {
		bSell ? pWindow->SetWindowText(g_pResourceManager->GetGlobalString(52000027)) :		// 판매물건
				pWindow->SetWindowText(g_pResourceManager->GetGlobalString(52000023)) ;		// 낙찰물건
		pWindow->SetShowEnable(true) ;
		pWindow->Show() ;
	}
	pWindow = Find( WIID_MAILMSG_AUCTION_TITLE_ITEM, true) ;
	if( pWindow )	{
		SPItemAttr* pItemAttr ;
		g_pItemCluster->GetItemInfo(iItemID, pItemAttr);
		// 낙찰된 물건의 TEXT는 모두 검정색으로 표시한다.
		pWindow->SetFontColor(RGBA(0, 0, 0, 255)) ;
		if( pItemAttr == NULL )
			pWindow->SetWindowText("") ;
		else
			pWindow->SetWindowText(pItemAttr->m_strName.c_str());
		pWindow->SetShowEnable(true) ;
		pWindow->Show() ;
	}

	pWindow = Find( WIID_MAILMSG_AUCTION_TITLE_MONEY, true) ;
	if( pWindow )	{
		pWindow->SetShowEnable(false) ;
		pWindow->Hide() ;
	}
	pWindow = Find( WIID_MAILMSG_AUCTION_TITLE_ICON, true) ;
	if( pWindow )	{
		pWindow->SetShowEnable(false) ;
		pWindow->Hide() ;
	}
	pWindow = Find( WIID_MAILMSG_AUCTION_TITLE_STRING, true) ;
	if( pWindow )	{
		pWindow->SetShowEnable(false) ;
		pWindow->Hide() ;
	}
}

/**
	게임 머니를 판매/구매하는 경우
	@param bool bSell : true(판매), false(구매)
	@param INT64 iMoney : 판매금액
	@param int iIconType : 0(Ely), 1(Cash), 2(Mili)
 */
void SPWindowMailMsg::SetAuctionMsgTitle(bool bSell, INT64 iMoney, int iIconType)
{
	SPWindow* pWindow = NULL ;
	pWindow = Find(WIID_MAILMSG_AUCTION_TITLE, true) ;
	if( pWindow )	{
		bSell ? pWindow->SetWindowText(g_pResourceManager->GetGlobalString(52000027)) :		// 판매물건
				pWindow->SetWindowText(g_pResourceManager->GetGlobalString(52000023)) ;		// 낙찰물건
		pWindow->SetShowEnable(true) ;
		pWindow->Show() ;
	}
	pWindow = Find( WIID_MAILMSG_AUCTION_TITLE_MONEY, true) ;
	if( pWindow )	{
		pWindow->SetWindowText(ConvertMoneyToString(iMoney)) ;
		pWindow->SetShowEnable(true) ;
		pWindow->Show() ;
	}
	pWindow = Find( WIID_MAILMSG_AUCTION_TITLE_ICON, true) ;
	if( pWindow )	{
		pWindow->SPSendMessage(SPIM_SET_IMAGEINDEX, iIconType) ;
		pWindow->SetShowEnable(true) ;
		pWindow->Show() ;
	}

	pWindow = Find( WIID_MAILMSG_AUCTION_TITLE_ITEM, true) ;
	if( pWindow )	{
		pWindow->SetShowEnable(false) ;
		pWindow->Hide() ;
	}
	pWindow = Find( WIID_MAILMSG_AUCTION_TITLE_STRING, true) ;
	if( pWindow )	{
		pWindow->SetShowEnable(false) ;
		pWindow->Hide() ;
	}
}


/**
	경매 취소나 상회입찰로 메시지를 출력해야 하는 경우
	@param GLOBAL_STRING_ID iMsgID : GlobalStringID
 */
void SPWindowMailMsg::SetAuctionMsgTitle(GLOBAL_STRING_ID iMsgID)
{
	SPWindow* pWindow = NULL ;
	std::string strString ;
	pWindow = Find( WIID_MAILMSG_AUCTION_TITLE_STRING, true) ;
	if( pWindow )	{
		strString = g_pResourceManager->GetGlobalString(iMsgID) ;
		pWindow->SetWindowText( strString.c_str() ) ;
		pWindow->SetShowEnable(true) ;
		pWindow->Show() ;
	}

	pWindow = Find(WIID_MAILMSG_AUCTION_TITLE, true) ;
	if( pWindow )	{
		pWindow->SetShowEnable(false) ;
		pWindow->Hide() ;
	}
	pWindow = Find( WIID_MAILMSG_AUCTION_TITLE_ITEM, true) ;
	if( pWindow )	{
		pWindow->SetShowEnable(false) ;
		pWindow->Hide() ;
	}
	pWindow = Find( WIID_MAILMSG_AUCTION_TITLE_MONEY, true) ;
	if( pWindow )	{
		pWindow->SetShowEnable(false) ;
		pWindow->Hide() ;
	}
	pWindow = Find( WIID_MAILMSG_AUCTION_TITLE_ICON, true) ;
	if( pWindow )	{
		pWindow->SetShowEnable(false) ;
		pWindow->Hide() ;
	}

}

//////////////////////////////////////////////////////////////////////////
SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowMailMsg )
SPIMESSAGE_COMMAND(SPIM_MAIL_MSG_UPDATE,		OnUpdateMailMsg )
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_MAILMSG_CLOSE		, OnClose)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_MAILMSG_RECEIVE	, OnReceiveMail)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_MAILMSG_DELETE		, OnDeleteMail)
SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_ENTER,	WIID_MAILMSG_ITEM,	OnToolTipEnter	)
SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_OUT,	WIID_MAILMSG_ITEM,	OnToolTipOut	)
SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_MOVE,	WIID_MAILMSG_ITEM,	OnToolTipMove	)
SPIMESSAGE_OBJ_MAP_END	( SPWindow	)



int SPWindowMailMsg::OnUpdateMailMsg(WPARAM wParam, LPARAM lParam)
{
	int iSelectIndex = (int)wParam;

	m_pTitle->SetWindowText(NULL);
	m_pSender->SetWindowText(NULL);
	m_pMsg->SetWindowText(NULL);
	m_pEly->SetWindowText(NULL);
	m_pItemIcon->SPSendMessage(SPIM_SET_IMAGECLEAR);
	m_pItemName->SetWindowText(NULL);
	m_iItemCount = 0;
	m_iItemMark = ICON_MARK_NULL;

	SPMailAttr* pMailAttr = g_pEventManager->GetMailManager()->GetMail(iSelectIndex);
	if(pMailAttr == NULL || pMailAttr->m_pHeader->uiMailID == 0) {
		return 1;
	}

	if(pMailAttr->m_pHeader->uiReadFlag) {
		m_pCheckWnd->Hide();
		m_pCheckWnd->SetImage(m_pCheck, m_rtSrcCheck.left, m_rtSrcCheck.top);
		m_pCheckWnd->Show();
	}
	else if(pMailAttr->m_pHeader->uiMailID) {
		m_pCheckWnd->Hide();
		m_pCheckWnd->SetImage(m_pCheck, m_rtSrcNCheck.left, m_rtSrcNCheck.top);
		m_pCheckWnd->Show();
	}
	
	m_pTitle->SetWindowText(pMailAttr->m_strSubject.c_str());

	// Set Mail Memo
	if( pMailAttr->m_iMailType == MAIL_TYPE_NULL )	{
		m_pSender->SetWindowText(pMailAttr->m_pHeader->szSender);
		m_pAuctionMsg->SetShowEnable(false) ;
		m_pMsg->SetShowEnable(true) ;
		if(pMailAttr->m_iMemoLen > 0) {
			m_pMsg->SetWindowText(pMailAttr->m_strMemo.c_str());
		}
		else	{
			m_pMsg->SetWindowText("") ;
		}
	}
	else	{
		if( g_pResourceManager->GetGlobalString(52000093) )
			m_pSender->SetWindowText(g_pResourceManager->GetGlobalString(52000093));
		else
			m_pSender->SetWindowText("Auction") ;
		m_pMsg->SetShowEnable(false) ;
		m_pAuctionMsg->SetShowEnable(true) ;
		SetAuctionMsg(pMailAttr) ;
	}
	
	char szTemp[32];
	ZeroMemory(szTemp, 32);
	wsprintf(szTemp, "%d/%d/%d %d:%d", pMailAttr->m_pHeader->tmSendTime.year, pMailAttr->m_pHeader->tmSendTime.month,
		pMailAttr->m_pHeader->tmSendTime.day, pMailAttr->m_pHeader->tmSendTime.hour, pMailAttr->m_pHeader->tmSendTime.minute);
	m_pDate->SetWindowText(szTemp);

	m_pReceive->SetEnable(false);
	m_pEly->SetWindowText(ConvertMoneyToString(pMailAttr->m_pHeader->uiEly));	
	if(pMailAttr->m_pHeader->uiEly)
		m_pReceive->SetEnable(true);

	if(pMailAttr->m_pHeader->uiItemID) {
		POINT ptSrcPos;
		SPTexture* pTexture = g_pItemCluster->GetItemTexture(pMailAttr->m_pHeader->uiItemID, ptSrcPos);
		
		if(pMailAttr->m_pItem->GetItemStatus() && pMailAttr->m_pItem->GetItemAttr()) {
			//m_iMark = g_pItemCluster->GetItemIconMark(m_pItemStatus->GetItemID());		
			m_pItemIcon->Hide();
			m_pItemIcon->SetImage(pTexture, ptSrcPos.x, ptSrcPos.y);
			m_pItemIcon->Show();

			if(pMailAttr->m_pItem->GetItemAttr()->m_bNameColor) {
				m_pItemName->SetFontColor(pMailAttr->m_pItem->GetItemAttr()->GetNameColor());
			}
			else {
				m_pItemName->SetFontColor(g_pCheckManager->GetItemColor(pMailAttr->m_pItem->GetItemStatus()->GetRareLevel()));
			}
			m_pItemName->SetWindowText(pMailAttr->m_pItem->GetItemAttr()->m_strName.c_str());
			m_iItemCount = pMailAttr->m_pHeader->uiItemCount;
			m_iItemMark = g_pItemCluster->GetItemIconMark(pMailAttr->m_pHeader->uiItemID);

			m_pReceive->SetEnable(true);
		}
	}	

	return 1;
}


int SPWindowMailMsg::OnClose(WPARAM wParam, LPARAM lParam)
{
	Hide();
	return 1;
}


int SPWindowMailMsg::OnToolTipEnter(WPARAM wParam, LPARAM lParam)
{
	SPMailAttr* pMailAttr = g_pEventManager->GetMailManager()->GetSelectMail();
	if(pMailAttr == NULL || pMailAttr->m_pHeader->uiMailID == 0) {
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
		return 1;
	}

	if(pMailAttr->m_pHeader->uiItemID) {
		if(pMailAttr->m_pItem->GetItemStatus() && pMailAttr->m_pItem->GetItemAttr()) {			
			if(pMailAttr->m_pItem->GetItemAttr()->m_eDuration != ITEM_DURATION_NULL) {
				pMailAttr->m_pItem->GetItemStatus()->SetLifeTime(pMailAttr->m_pItem->GetItemAttr()->m_iLifeTime * 3600);
			}
			
			SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
			pWindow->SPSendMessage( SPIM_TOOLTIP_ENTER, (WPARAM)pMailAttr->m_pItem->GetItemStatus(), lParam);
			pWindow->Show();
		}
	}	

	return 1;
}


int SPWindowMailMsg::OnToolTipOut(WPARAM wParam, LPARAM lParam)
{
	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
	pWindow->Show();

	return 1;
}


int SPWindowMailMsg::OnToolTipMove(WPARAM wParam, LPARAM lParam)
{
	SPMailAttr* pMailAttr = g_pEventManager->GetMailManager()->GetSelectMail();
	if(pMailAttr == NULL || pMailAttr->m_pHeader->uiMailID == 0) {
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
		return 1;
	}

	if(pMailAttr->m_pHeader->uiItemID) {
		if(pMailAttr->m_pItem->GetItemStatus() && pMailAttr->m_pItem->GetItemAttr()) {
			if(pMailAttr->m_pItem->GetItemAttr()->m_eDuration != ITEM_DURATION_NULL) {
				pMailAttr->m_pItem->GetItemStatus()->SetLifeTime(pMailAttr->m_pItem->GetItemAttr()->m_iLifeTime * 3600);
			}

			SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
			pWindow->SPSendMessage( SPIM_TOOLTIP_ENTER, (WPARAM)pMailAttr->m_pItem->GetItemStatus(), lParam);
			pWindow->Show();
		}
	}

	return 1;
}


int SPWindowMailMsg::OnReceiveMail(WPARAM wParam, LPARAM lParam)
{
	SPMailAttr* pMailAttr = g_pEventManager->GetMailManager()->GetSelectMail();
	if(pMailAttr == NULL || pMailAttr->m_pHeader->uiMailID == 0) {
		return 1;
	}

	//메일에 받을것이 남겨져 있는 경우
	if(pMailAttr->m_pHeader->uiItemID || pMailAttr->m_pHeader->uiEly > 0) {
		if(pMailAttr->m_pHeader->uiItemID) {				//아이템을 넣을수 있는 공간이 있는지 확인
			if(false == g_pCheckManager->CheckEnableSlot(pMailAttr->m_pHeader->uiItemID, pMailAttr->m_pHeader->uiItemCount)) {
				std::string strMsg = g_pResourceManager->GetGlobalString(5007009);
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
				return 1;
			}
		}		
		g_pEventManager->GetMailManager()->SendReceiveBody();
	}
	
	return 1;
}


int SPWindowMailMsg::OnDeleteMail(WPARAM wParam, LPARAM lParam)
{
	SPMailAttr* pMailAttr = g_pEventManager->GetMailManager()->GetSelectMail();
	if(pMailAttr == NULL || pMailAttr->m_pHeader->uiMailID == 0) {
		return 1;
	}
	
	//메일에 받을것이 남겨져 있는 경우
	if(pMailAttr->m_pHeader->uiItemID || pMailAttr->m_pHeader->uiEly > 0) {		
		std::string strMsg = g_pResourceManager->GetGlobalString(5007008);
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		return 1;
	}

	g_pEventManager->GetMailManager()->SendDeleteMail();

	return 1;
}
