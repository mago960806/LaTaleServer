
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
#include "SPWindowEditMultiLine.h"
#include "SPWindowSlider.h"
#include "SPWindowMailList.h"

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

#include "SPAbuseFilter.h"

#include "SPCheckManager.h"

#include "SPDebug.h"

SPWindowMailList::SPWindowMailList(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_MAILLIST, InstanceID, iX, iY, iCX, iCY, pParent)
, m_pDisable(NULL)
, m_pCheck(NULL)
, m_pView(NULL)
, m_pSend(NULL)
, m_pPrev(NULL)
, m_pNext(NULL)
, m_pListView(NULL)
, m_pSendView(NULL)
, m_iMailListMode(MAILLIST_MODE_VIEW)
{	
	Init();
}


SPWindowMailList::~SPWindowMailList()
{	
	Clean();
}


void SPWindowMailList::Init()
{	
	std::string strTextureFile = "";
	strTextureFile = "DATA/INTERFACE/CONCEPT/UI100.PNG";
	g_pVideo->GetTextureMgr()->LoadTexture(strTextureFile.c_str(), &m_pDisable);

	strTextureFile = "DATA/INTERFACE/CONCEPT/UI103.PNG";
	g_pVideo->GetTextureMgr()->LoadTexture(strTextureFile.c_str(), &m_pCheck);
	
	SetRect(&m_rtSrcCenter, 302, 277, 509, 278);
	SetRect(&m_rtDestCenter, m_iAX, m_iAY + 21, m_iAX + 208, m_iAY + 21 + 279);

	m_pItemStatus	= NULL;
	m_PickUpItem.Clear();
	m_iStorage		= CONTAINER_TYPE_INVALID;	
	
	//SetRect(&m_rtSrcDisable, 434 , 37 , 436 , 39);
	SetRect(&m_rtMarkSrc[0], 479, 152, 511, 184);		//ICON_MARK_LOCK
	SetRect(&m_rtMarkSrc[1], 479, 185, 511, 217);		//ICON_MARK_FASHION
	SetRect(&m_rtMarkSrc[2], 479, 339, 511, 371);		//ICON_MARK_EVENT
	SetRect(&m_rtMarkSrc[3], 479, 372, 511, 404);		//ICON_MARK_FANCY

	SetRect(&m_rtSrcCheck, 212, 46, 230, 60);			//Mail Read Check
	SetRect(&m_rtSrcNCheck, 231, 46, 249, 60);
	
	InitSubWindow();
}


void SPWindowMailList::Clean()
{	
	for(int i = 0; i < MAX_MAIL_LIST; i++) {
		m_pListCheck[i]->SPSendMessage(SPIM_SET_IMAGECLEAR);
	}	
	SAFE_RELEASE(m_pDisable);
	SAFE_RELEASE(m_pCheck);
	
	SPWindow::Clean();
}


void SPWindowMailList::InitSubWindow()
{
	SPWindow* pStatic = NULL;
	SPWindowButton* pButton = NULL;	
	SPWindow* pParent = NULL;
	SPWindow* pTop = NULL;
	SPWindow* pCenter = NULL;
	SPWindow* pBottom = NULL;

	//Top
	pTop = new SPWindowStatic(WIID_MAILLIST_TOP, 0, 0, 350, 4, this);
	pTop->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 185);
	pTop->Show();

	//Center
	pCenter = new SPWindowStatic(WIID_MAILLIST_CENTER, 0, 4, 350, 261, this);
	pCenter->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 191);
	pCenter->SetSrcSize(350, 2);
	pCenter->Show();

	//////////////////////////////////////////////////////////////////////////
	//Title
	pStatic = new SPWindowStatic(WIID_MAILLIST_TITLE, 10, 8 -4, 70, 15, pCenter);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 242, 31);
	pStatic->Show();
	pStatic = NULL;
	
	//Close
	pButton = new SPWindowButton(WIID_MAILLIST_CLOSE, 329, 8 - 4, 12, 12, pCenter);
	pButton->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 301,	321);		//normal
	pButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG", 314,	321);	//on
	pButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG", 327,	321);	//push
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG", 340,	321); //disable
	//pButton->SetImageCheck("DATA/INTERFACE/CONCEPT/UI103.PNG", 327, 327);	//check
	//pButton->SetLightMap(true);
	pButton->Show();

	//
	pParent = new SPWindowStatic(WIID_MAILLIST_MENU_BASE, 2, 26 - 4, 346, 19, pCenter);
	pParent->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 493, 486);
	pParent->SetSrcSize(6, 4);
	pParent->Show();
	

	// 우편 확인
	m_pView = new SPWindowButton(WIID_MAILLIST_VIEW, 10, 28 - 26, 64, 15, pParent);
	m_pView->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 256,	50);		//normal
	m_pView->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG", 256,	66);	//on
	m_pView->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG", 256,	82);	//push
	m_pView->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG", 256,	98); //disable
	m_pView->SetImageCheck("DATA/INTERFACE/CONCEPT/UI103.PNG", 256, 82);	//check
	//pButton->SetLightMap(true);
	m_pView->Show();

	// 우편 전송
	m_pSend = new SPWindowButton(WIID_MAILLIST_SEND, 73, 28 - 26, 64, 15, pParent);
	m_pSend->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 256,	114);		//normal	
	m_pSend->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG", 256,	130);	//on
	m_pSend->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG", 256,	146);	//push
	m_pSend->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG", 256,	162); //disable
	m_pSend->SetImageCheck("DATA/INTERFACE/CONCEPT/UI103.PNG", 256, 146);	//check
	//pButton->SetLightMap(true);
	m_pSend->Show();	
	
	//상단 라인
	pStatic = new SPWindowStatic(WIID_MAILLIST_UPLINE, 2, 45 - 26, 346, 1, pParent);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 493,	494);
	pStatic->SetSrcSize(6, 1);
	pStatic->Show();
	pStatic = NULL;

	pParent = NULL;



	//////////////////////////////////////////////////////////////////////////	
	//ListView
	m_pListView = new SPWindowStatic(WIID_MAILLIST_LISTVIEW, 0, 46 - 4, 355, 223, pCenter);	
	m_pListView->Show();

	//ListViewSkin
	SPWindow* pListSkin = NULL;
	pListSkin = new SPWindowStatic(WIID_MAILLIST_LIST_SKIN, 2, 46 - 46, 346, 195, m_pListView);
	pListSkin->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 493, 477);
	pListSkin->SetSrcSize(6, 4);
	pListSkin->Show();

	//List View 인경우 bottom
	pBottom  = new SPWindowStatic(WIID_MAILLIST_ITEM_BOTTOM, 0, 265 - 46, 350, 4, m_pListView);
	pBottom->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 195);
	pBottom->Show();	
	
	//// Page Base
	//m_pPageBase = new SPWindowStatic(WIID_MAILLIST_PAGEBASE, 66, 282 - 21, 76, 17, pCenter);
	//m_pPageBase->SetImage("DATA/INTERFACE/CONCEPT/UI010.PNG", 170, 292);
	//m_pPageBase->Show();

	// 이전
	m_pPrev = new SPWindowButton(WIID_MAILLIST_PREV, 141 - 2, 221 - 46, 7, 11, pListSkin);
	m_pPrev->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 238, 378);
	m_pPrev->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG", 246,	378);	//on
	m_pPrev->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG", 254,	378);	//push
	m_pPrev->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG", 262,	378); //disable
	//m_pPrev->SetImageCheck("DATA/INTERFACE/CONCEPT/UI010.PNG", 52, 228);	//check
	//pButton->SetLightMap(true);
	m_pPrev->Show();
	
	// Page	
	pStatic = new SPWindowStatic(WIID_MAILLIST_PAGE, 151 - 2, 219 - 46, 48, 14, pListSkin);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 226, 345);
	pStatic->SetFormat(DT_CENTER | DT_VCENTER);
	pStatic->SetWindowText("10 / 10");
	pStatic->SetFontColor(RGBA(255, 255, 255, 255));
	pStatic->Show();
	m_pPage = pStatic;
	pStatic = NULL;

	// 다음
	m_pNext = new SPWindowButton(WIID_MAILLIST_NEXT, 203 - 2, 221 - 46, 7, 11, pListSkin);
	m_pNext->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 269, 378);
	m_pNext->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG", 277,	378);	//on
	m_pNext->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG", 285,	378);	//push
	m_pNext->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG", 293,	378); //disable
	//m_pPrev->SetImageCheck("DATA/INTERFACE/CONCEPT/UI010.PNG", 52, 228);	//check
	//pButton->SetLightMap(true);
	m_pNext->Show();

	

	//int	iButtonY[MAX_MAIL_LIST]	= {48 - 23 - 21, 103 - 23 - 21, 158 - 23 - 21, 213 - 23 - 21};	//배경, 아이콘 공유	
	//int iTextY[MAX_MAIL_LIST]	= {38, 73, 108, 143};	//텍스트 공유
	//int iFocusY[MAX_MAIL_LIST]	= {52, 87, 122, 157};	

	for(int i = 0 ; i < MAX_MAIL_LIST ; i++) {
		int iStartY = (50 - 46) + (41 * i);
		//바닥 스킨
		pParent = new SPWindowStatic(WIID_MAILLIST_ITEM_SKIN + i, 2, iStartY, 346, 41, pListSkin);
		pParent->Show();		

		//버튼
		pButton = new SPWindowButton(WIID_MAILLIST_ITEM_BUTTON + i, 0, 0, 346, 41, pParent);		
		pButton->Show();
		m_pListButton[i] = pButton;
		pButton = NULL;

		SPWindowStatic* pStaticWnd = NULL;
		pStaticWnd = new SPWindowStatic(WIID_MAILLIST_ITEM_SEL + i, 4, 4, 334, 32, pParent);
		pStaticWnd->SetWindowType(WND_TYPE_LEFTBASE);
		pStaticWnd->SetImageNormalHead	("DATA/INTERFACE/CONCEPT/UI101.PNG", 486, 373, 488, 405);
		pStaticWnd->SetImageNormalBody	("DATA/INTERFACE/CONCEPT/UI101.PNG", 490, 373, 492, 405);
		pStaticWnd->SetImageNormalTail	("DATA/INTERFACE/CONCEPT/UI101.PNG", 494, 373, 496, 405);
		pStaticWnd->Show();
		m_pListSelect[i] = pStaticWnd;

		//Read Check
		pStatic = new SPWindowStatic(WIID_MAILLIST_ITEM_READ + i, 8, 13, 18, 14, pParent);		
		pStatic->Show();
		m_pListCheck[i] = pStatic;
		m_bRead[i] = false;
		pStatic = NULL;		

		//Sender
		pStatic = new SPWindowStatic(WIID_MAILLIST_ITEM_SENDER + i, 33, 16, 96, 12, pParent);
		pStatic->SetFormat(DT_LEFT | DT_VCENTER);
		//pStatic->SetWindowText("보낸 사람...................");
		pStatic->SetFontColor(RGBA(0, 0, 0, 255));
		pStatic->Show();
		m_pListSender[i] = pStatic;
		pStatic = NULL;

		//메일 제목
		pStatic = new SPWindowStatic(WIID_MAILLIST_ITEM_NAME + i, 135, 16, 162, 12, pParent);
		//pStatic->SetFormat(DT_LEFT | DT_VCENTER | DT_WORDBREAK);
		pStatic->SetFormat(DT_LEFT | DT_VCENTER);
		//pStatic->SetWindowText("메일 제목..................");
		pStatic->SetFontColor(RGBA(143, 99, 99, 255));
		pStatic->SetFont(FONT_12_BOLD);
		pStatic->Show();
		m_pListName[i] = pStatic;
		pStatic = NULL;

		//Icon Line
		pStatic = new SPWindowStatic(WIID_MAILLIST_ITEM_ICON + i, 308, 4, 32, 32, pParent);
		pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 203, 375);
		pStatic->Show();
		pStatic = NULL;

		//Icon
		pStatic = new SPWindowStatic(WIID_MAILLIST_ITEM_ICON + i, 308, 4, 32, 32, pParent);
		pStatic->Show();		
		m_pListIcon[i] = pStatic;
		pStatic = NULL;
		m_pListIcon[i]->GetWindowRect(m_rtStack[i]);		

		////Date
		//pStatic = new SPWindowStatic(WIID_MAILLIST_ITEM_DATE + i, 36, 36, 137, 15, pParent);
		//pStatic->SetFormat(DT_RIGHT | DT_VCENTER | DT_WORDBREAK);
		////pStatic->SetWindowText("날자.........................");
		//pStatic->SetFontColor(RGBA(0, 0, 0, 255));
		//pStatic->Show();
		//m_pListDate[i] = pStatic;
		//pStatic = NULL;

		//m_bAble[i]			= true;;
		m_iStackMark[i]		= ICON_MARK_NULL;
		m_iStackCount[i]	= 0;
	}


	//////////////////////////////////////////////////////////////////////////
	//SendView
	m_pSendView = new SPWindowStatic(WIID_MAILLIST_SENDVIEW, 0, 46 - 4, 355, 223, pCenter);
	m_pSendView->SetImage("DATA/INTERFACE/CONCEPT/UI010.PNG", 203, 1);
	m_pSendView->Hide();

	//List View 인경우 bottom
	pBottom  = new SPWindowStatic(WIID_MAILLIST_SEND_BOTTOM, 0, 265 - 46, 350, 4, m_pSendView);
	pBottom->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 195);
	pBottom->Show();

	//SendViewSkin
	SPWindow* pSendSkin = NULL;
	pSendSkin = new SPWindowStatic(WIID_MAILLIST_SEND_SKIN, 2, 46 - 46, 346, 219, m_pSendView);
	pSendSkin->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 493, 477);
	pSendSkin->SetSrcSize(6, 4);
	pSendSkin->Show();	

	//SendTitle Base
	pStatic = new SPWindowStatic(WIID_MAILLIST_SEND_TITLEBASE, 18 - 2, 58 - 46, 60, 17, pSendSkin);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 149);
	pStatic->Hide();	

	//SendTitle
	m_pTitle = new SPWindowEdit(WIID_MAILLIST_SEND_TITLE, 86 - 2, 58 - 46, 246, 17, pSendSkin);
	m_pTitle->SetWindowType(WND_TYPE_LEFTBASE);
	m_pTitle->SetMargin( 2, 1);
	m_pTitle->SetImageNormalHead	("DATA/INTERFACE/CONCEPT/UI103.PNG", 244, 91, 246, 108);
	m_pTitle->SetImageNormalBody	("DATA/INTERFACE/CONCEPT/UI103.PNG", 248, 91, 250, 108);
	m_pTitle->SetImageNormalTail	("DATA/INTERFACE/CONCEPT/UI103.PNG", 252, 91, 254, 108);
	m_pTitle->SetImageFocusHead		("DATA/INTERFACE/CONCEPT/UI103.PNG", 244, 73, 246, 90);
	m_pTitle->SetImageFocusBody		("DATA/INTERFACE/CONCEPT/UI103.PNG", 248, 73, 250, 90);
	m_pTitle->SetImageFocusTail		("DATA/INTERFACE/CONCEPT/UI103.PNG", 252, 73, 254, 90);
	m_pTitle->SetEnableTabStop(true);
	m_pTitle->SetLimitText(MAILBOX_MAX_SUBJECT_LEN - 10);
	pStatic = NULL;

	////보낸사람 -> 보낼사람
	//pStatic = new SPWindowStatic(WIID_MAILLIST_SEND_RECEIVERPLATE, 10-3, 72 - 24 - 21, 47, 11, m_pSendView);
	//pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI010.PNG", 249, 294);
	//pStatic->Hide();
	//pStatic = NULL;

	//ReceiverBase
	pStatic = new SPWindowStatic(WIID_MAILLIST_SEND_RECEIVERBASE, 18 - 2, 79 - 46, 60, 17, pSendSkin);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 167);
	pStatic->Hide();

	//Receiver
	m_pReceiver = new SPWindowEdit(WIID_MAILLIST_SEND_RECEIVER, 86 - 2, 79 - 46, 246, 17, pSendSkin);
	m_pReceiver->SetWindowType(WND_TYPE_LEFTBASE);
	m_pReceiver->SetMargin( 2, 1);
	m_pReceiver->SetImageNormalHead		("DATA/INTERFACE/CONCEPT/UI103.PNG", 244, 91, 246, 108);
	m_pReceiver->SetImageNormalBody		("DATA/INTERFACE/CONCEPT/UI103.PNG", 248, 91, 250, 108);
	m_pReceiver->SetImageNormalTail		("DATA/INTERFACE/CONCEPT/UI103.PNG", 252, 91, 254, 108);
	m_pReceiver->SetImageFocusHead		("DATA/INTERFACE/CONCEPT/UI103.PNG", 244, 73, 246, 90);
	m_pReceiver->SetImageFocusBody		("DATA/INTERFACE/CONCEPT/UI103.PNG", 248, 73, 250, 90);
	m_pReceiver->SetImageFocusTail		("DATA/INTERFACE/CONCEPT/UI103.PNG", 252, 73, 254, 90);
	m_pReceiver->SetEnableTabStop(true);
	pStatic = NULL;

	//첨부 이미지 1
	pStatic = new SPWindowStatic(WIID_MAILLIST_SEND_APPEND_1, 18 - 2 , 100 - 46, 60, 3, pSendSkin);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 145);
	pStatic->Show();

	//첨부 이미지 2
	pStatic = new SPWindowStatic(WIID_MAILLIST_SEND_APPEND_2, 18 - 2 , 103 - 46, 60, 50, pSendSkin);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 95, 134);
	pStatic->Show();

	//Item Button
	m_pItemButton = new SPWindowButton(WIID_MAILLIST_SEND_ITEM, 86 - 2 , 100 - 46, 246, 32, pSendSkin);	
	//m_pItemButton->SetWindowText("Item Button.......");
	m_pItemButton->Hide();

	pStatic = new SPWindowStatic(WIID_MAILLIST_SEND_ITEMICON_LINE, 86 - 2 , 100 - 46, 32, 32, pSendSkin);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 62, 134);
	pStatic->Show();
	pStatic = NULL;
	
	//Item Icon
	m_pItemIcon = new SPWindowStatic(WIID_MAILLIST_SEND_ITEMICON, 86 - 2 , 100 - 46, 32, 32, pSendSkin);	
	m_pItemIcon->Hide();
	m_pItemIcon->GetWindowRect(m_rtItemStack);
	m_iItemCount = 0;
	m_iItemMark = ICON_MARK_NULL;

	//Item Name
	m_pItemName = new SPWindowStatic(WIID_MAILLIST_SEND_ITEMNAME, 126 - 2, 111 - 46, 171, 12, pSendSkin);
	m_pItemName->SetFontColor(RGBA(255, 255, 255, 255));
	//m_pItemName->SetWindowText("Item Name");
	m_pItemName->Hide();	

	//ElyBase
	//pStatic = new SPWindowStatic(WIID_MAILLIST_SEND_ELYBASE, 41 - 4, 260 - 24 - 21, 138, 16, m_pSendView);
	//pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI010.PNG", 157, 245);	
	//pStatic->Hide();

	//Ely
	m_pEly = new SPWindowEdit(WIID_MAILLIST_SEND_ELY, 86 - 2, 136 - 46, 90, 17, pSendSkin);
	m_pEly->SetWindowType(WND_TYPE_LEFTBASE);
	m_pEly->SetMargin( 2, 1);
	m_pEly->SetImageNormalHead		("DATA/INTERFACE/CONCEPT/UI103.PNG", 244, 91, 246, 108);
	m_pEly->SetImageNormalBody		("DATA/INTERFACE/CONCEPT/UI103.PNG", 248, 91, 250, 108);
	m_pEly->SetImageNormalTail		("DATA/INTERFACE/CONCEPT/UI103.PNG", 252, 91, 254, 108);
	m_pEly->SetImageFocusHead		("DATA/INTERFACE/CONCEPT/UI103.PNG", 244, 73, 246, 90);
	m_pEly->SetImageFocusBody		("DATA/INTERFACE/CONCEPT/UI103.PNG", 248, 73, 250, 90);
	m_pEly->SetImageFocusTail		("DATA/INTERFACE/CONCEPT/UI103.PNG", 252, 73, 254, 90);
	m_pEly->SetNumber();
	m_pEly->SetMaxValue(999999999);
	m_pEly->SetWindowText("0");
	m_pEly->SetEnableTabStop(true);	

	pStatic = new SPWindowStatic(WIID_MAILLIST_SEND_ELY_IMG, 184 - 2, 141 - 46, 17, 5, pSendSkin);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 494, 506);
	pStatic->Show();
	pStatic = NULL;

	SPWindowStatic* pStaticWnd = NULL;
	pStaticWnd = new SPWindowStatic(WIID_MAILLIST_SEND_MSG_BASE, 18 - 2, 162 - 46, 314, 75, pSendSkin);
	pStaticWnd->SetWindowType(WND_TYPE_LEFTBASE);
	pStaticWnd->SetImageNormalHead		("DATA/INTERFACE/CONCEPT/UI103.PNG", 244, 109, 245, 183);
	pStaticWnd->SetImageNormalBody		("DATA/INTERFACE/CONCEPT/UI103.PNG", 248, 109, 250, 184);
	pStaticWnd->SetImageNormalTail		("DATA/INTERFACE/CONCEPT/UI103.PNG", 252, 109, 254, 184);
	pStaticWnd->Show();

	//Msg
	m_pMsg = new SPWindowEditMultiLine(WIID_MAILLIST_SEND_MSG, 41- 18, 166 - 162, 251, 66, pStaticWnd);
	//m_pMsg->SetLimitText(MAILBOX_MAX_MEMO_LEN - 1);
	m_pMsg->SetEnableTabStop(true);
	m_pMsg->Hide();
	m_pMsg->SetEnableEnter(true);

	//Send Button
	m_pMailSend = new SPWindowButton(WIID_MAILLIST_SEND_MAIL, 272 - 2, 246 - 46, 68, 17, pSendSkin);
	m_pMailSend->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 417,	373);		//normal	
	m_pMailSend->SetImageHit("DATA/INTERFACE/CONCEPT/UI101.PNG", 417,	392);	//on
	m_pMailSend->SetImagePush("DATA/INTERFACE/CONCEPT/UI101.PNG", 417,	411);	//push
	m_pMailSend->SetImageDisable("DATA/INTERFACE/CONCEPT/UI101.PNG", 417,	430); //disable

	//pButton = new SPWindowButton(WIID_MAILLIST_SEND_MAIL, , pSendSkin);
	//pButton->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 415,	440);		//normal	
	//pButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG", 415,	458);	//on
	//pButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG", 415,	476);	//push
	//pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG", 415,	494); //disable
}


void SPWindowMailList::Process(float fTime)
{
	SPWindow::Process(fTime);
}


void SPWindowMailList::Render(float fTime)
{
	if(IsShow() == false) return;	
	
	SPWindow::Render(fTime);

	//
	if(m_iMailListMode == MAILLIST_MODE_VIEW) {
		for(int i = 0; i < MAX_MAIL_LIST; i++) {
			////Icon Disable 표현
			if(m_pDisable) {
				RECT rtDest;
				m_pListIcon[i]->GetWindowRect(rtDest);
				//if(m_bAble[i] == false) {		
				//	m_pDisable->RenderTexture(&rtDest, &m_rtSrcDisable);
				//}
				if(m_iStackMark[i] > ICON_MARK_NULL) {
					m_pDisable->RenderTexture(&rtDest, &m_rtMarkSrc[m_iStackMark[i] - 1]);
				}
			}

			//Stack Count Local 표현
			if(m_iStackCount[i] > 9) {
				RECT rtTenNumTarget, rtNumTarget, rtTenNumSrc, rtNumSrc;
				int iTenStack = m_iStackCount[i] / 10;
				int iOneStack = m_iStackCount[i] % 10;

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
			else if(m_iStackCount[i] > 1) {
				int iOneStack = m_iStackCount[i] % 10;
				RECT rtNumTarget, rtNumSrc;
				SetRect(&rtNumTarget, m_rtStack[i].left, m_rtStack[i].bottom - 10, m_rtStack[i].left + 9, m_rtStack[i].bottom);

				rtNumSrc.left = iOneStack * 9;
				rtNumSrc.right = rtNumSrc.left + 9;
				rtNumSrc.top = 0;
				rtNumSrc.bottom = 10;

				g_pItemCluster->GetNumTexture()->RenderTexture( &rtNumTarget , &rtNumSrc);
			}
		}
	}
	else if(m_iMailListMode == MAILLIST_MODE_SEND) {
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
	g_pVideo->Flush();
}


void SPWindowMailList::Show(bool bWithChild/* = true*/)
{
	SPWindow::Show(bWithChild);

	if(m_bShow) {
		m_iMailListMode = MAILLIST_MODE_VIEW;		
		SetModeButton();
		m_iCurPage = 0;

		if(g_pEventManager->GetMailManager()->IsMailListLoad() == false) {
			//g_pEventManager->GetMailManager()->SendGetMailList();
			//m_bListRequest = true;
			//m_fListAccumulate = 0.0f;
			//m_pReflash->SetEnable(true);
			m_iSelectItem = -1;			
		}
	}

	//g_pEventManager->GetMailManager()->SetNewMail();		//이시점에서 새로운 메일을 읽은것으로 처리	

	OnUpdateMailList((WPARAM)0, (LPARAM)0);
}


void SPWindowMailList::Hide(bool bSendServer /*= true*/)
{
	ResetItemControl();
	
	m_pTitle->SetWindowText(NULL);
	m_pReceiver->SetWindowText(NULL);
	m_pMsg->SetWindowText(NULL);
	m_pEly->SetWindowText("0");
	g_pInterfaceManager->GetIMEPool()->SetFocus( false );

	SPWindow::Hide(bSendServer);
}


void SPWindowMailList::SetModeButton()
{
	if(m_iMailListMode == MAILLIST_MODE_VIEW) {
		m_pView->SPSendMessage(SPIM_BTN_SELECT, true);
		m_pSend->SPSendMessage(SPIM_BTN_SELECT, false);

		//인벤토리
		ResetItemControl();
		g_pInterfaceManager->ShowWindow(WIID_ITEM,		SP_HIDE);
		
		m_pSendView->Hide();
		m_pMailSend->Hide();
		m_pListView->Show();
		//m_pPageBase->Show();
	}
	else if(m_iMailListMode == MAILLIST_MODE_SEND) {
		m_pView->SPSendMessage(SPIM_BTN_SELECT, false);
		m_pSend->SPSendMessage(SPIM_BTN_SELECT, true);
		
		m_pListView->Hide();
		//m_pPageBase->Hide();
		m_pSendView->Show();
		m_pMailSend->Show();

		//m_pTitle->SPSendMessage( SPIM_SETFOCUS );
		g_pInterfaceManager->SetFocusWindow( m_pTitle );
		GetWrapWindow()->SetTabStopWindow( m_pTitle );

		//메일 확인창
		g_pInterfaceManager->ShowWindow(WIID_MAILMSG, SP_HIDE);		
		
		//인벤토리
		g_pInterfaceManager->GetInvenWindow()->AdjustCoord(362+44, 9+63);
		g_pInterfaceManager->ShowWindow(WIID_ITEM,		SP_SHOW);
		g_pInterfaceManager->SPPerformMessage(SPIM_SET_EXIT_SHOW, (WPARAM)WIID_ITEM, (LPARAM)false);
	}
	else {
		//Error
	}
}


void SPWindowMailList::ClearSend()
{
	if(g_pResourceManager->GetGlobalString(5007016)) {		//[%s]님에게 우편을 발송하였습니다.
		char szMsg[256];
		ZeroMemory(szMsg, 256);
		sprintf(szMsg, g_pResourceManager->GetGlobalString(5007016), m_pReceiver->GetWindowText());	
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMsg);
	}	
	
	m_pTitle->SetWindowText(NULL);
	m_pReceiver->SetWindowText(NULL);
	m_pMsg->SetWindowText(NULL);
	m_pEly->SetWindowText("0");
	
	m_pItemName->SetWindowText(NULL);
	m_pItemIcon->SPSendMessage(SPIM_SET_IMAGECLEAR);	
	ResetItemControl();
	SPSelectItem pNullSelectItem;
	pNullSelectItem.SetSelectItem(CONTAINER_TYPE_INVALID, -1);
	g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->SetSellSelectItem(pNullSelectItem);
	
}


void SPWindowMailList::UpdateList()
{
	char szTemp[32];
	ZeroMemory(szTemp, 32);
	if(m_iMaxPage > 0) {
		wsprintf(szTemp, "%d/%d", m_iCurPage + 1, m_iMaxPage);
	}	
	else if(m_iMaxPage == 0 && m_iCurPage == 0) {
		wsprintf(szTemp, "%d/%d", m_iCurPage, m_iMaxPage);

	}
	m_pPage->SetWindowText(szTemp);

	int iStartIndex = m_iCurPage * MAX_MAIL_LIST;	
	
	SPMailAttr* pMailAttr = NULL;
	
	//
	for(int i = 0; i < MAX_MAIL_LIST; i++, iStartIndex++) {		
		//SPWindow* pkWindow = Find(WIID_MAILLIST_ITEM_BUTTON + i, true);
		//if( pkWindow == NULL )
		//	continue;
		//pkWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE);
		
		m_pListSelect[i]->Hide();


		m_pListName[i]->SetWindowText(NULL);
		m_pListIcon[i]->SPSendMessage(SPIM_SET_IMAGECLEAR);
		m_pListSender[i]->SetWindowText(NULL);
		//m_pListDate[i]->SetWindowText(NULL);
		m_pListCheck[i]->SPSendMessage(SPIM_SET_IMAGECLEAR);
		
		//m_bAble[i]			= true;;
		
		m_iStackMark[i]		= ICON_MARK_NULL;
		m_iStackCount[i]	= 0;
		m_bRead[i]			= false;

		pMailAttr = g_pEventManager->GetMailManager()->GetMail(iStartIndex);
		if(pMailAttr == NULL || pMailAttr->m_pHeader->uiMailID == 0) 
			continue;

		if(pMailAttr->m_strSubject.size() > TITLE_LENGTH) {
			char szName[64];
			string strSub = pMailAttr->m_strSubject.substr(0, TITLE_LENGTH);
			wsprintf(szName, "%s", strSub.c_str());
			m_pListName[i]->SetWindowText(szName);
		}
		else {
			m_pListName[i]->SetWindowText(pMailAttr->m_strSubject.c_str());
		}
		

		if(pMailAttr->m_pHeader->uiItemID != 0) {
			if(pMailAttr->m_pHeader->uiItemID) {
				POINT ptSrcPos;
				SPTexture* pkTexture = g_pItemCluster->GetItemTexture(pMailAttr->m_pHeader->uiItemID, ptSrcPos);
				m_pListIcon[i]->Hide();
				m_pListIcon[i]->SetImage(pkTexture, ptSrcPos.x, ptSrcPos.y);
				m_pListIcon[i]->Show();
				
				m_iStackMark[i] = g_pItemCluster->GetItemIconMark(pMailAttr->m_pHeader->uiItemID);
				m_iStackCount[i] = pMailAttr->m_pHeader->uiItemCount;
			}		

			//m_bAble[i] = g_pCheckManager->CheckItemStatus(pGiftAttr->m_iItemID);
			//m_iItemMark[i] = g_pItemCluster->GetItemIconMark(pGiftAttr->m_iItemID);
			//m_iStackCount[i] = pGiftAttr->m_iItemStack;
		}

		if(pMailAttr->m_pHeader->uiReadFlag) {
			m_pListCheck[i]->Hide();
			m_pListCheck[i]->SetImage(m_pCheck, m_rtSrcCheck.left, m_rtSrcCheck.top);
			m_pListCheck[i]->Show();
		}
		else if(pMailAttr->m_pHeader->uiMailID) {
			m_pListCheck[i]->Hide();
			m_pListCheck[i]->SetImage(m_pCheck, m_rtSrcNCheck.left, m_rtSrcNCheck.top);
			m_pListCheck[i]->Show();
		}

		// 경매장 메일의 경우 Sender를 경매장으로..
		if( pMailAttr->m_iMailType == MAIL_TYPE_NULL )	{
			m_pListSender[i]->SetWindowText(pMailAttr->m_pHeader->szSender);
		}
		else {
			if( g_pResourceManager->GetGlobalString(52000093) )
				m_pListSender[i]->SetWindowText(g_pResourceManager->GetGlobalString(52000093));
			else
				m_pListSender[i]->SetWindowText("Auction") ;
		}
		
		//ZeroMemory(szTemp, 32);
		//wsprintf(szTemp, "%d/%d/%d %d:%d", pMailAttr->m_pHeader->tmSendTime.year, pMailAttr->m_pHeader->tmSendTime.month,
		//	pMailAttr->m_pHeader->tmSendTime.day, pMailAttr->m_pHeader->tmSendTime.hour, pMailAttr->m_pHeader->tmSendTime.minute);
		//m_pListDate[i]->SetWindowText(szTemp);

	}

	m_iSelectItem = -1;	
}


void SPWindowMailList::ResetItemControl(bool bRestore /*= true*/)
{
	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return;

	//만일 이전의 값이 있다면 인벤콘트롤에서 활성화를 시켜주고 팔기위해 선택한 아이템 정보를 초기화 한다
	if(m_pItemStatus != NULL) {
		if(bRestore) {
			g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->SetSellSelectItem(m_PickUpItem, false);
		}
	}
	m_pItemStatus = NULL;
	RestoreInven();			//same as... m_pSellSelectItem = NULL


	m_pItemIcon->SPSendMessage(SPIM_SET_IMAGECLEAR);
	m_iItemCount = 0;
	m_iItemMark = ICON_MARK_NULL;
	m_pItemName->SetWindowText("");

	//m_bAble = true;
	//m_iMark = ICON_MARK_NULL;

	//m_iSellCount = 1;							//카운터 관련 컨트롤 초기화
	//m_pCount->SetEnable(false);
	//m_pDownCount->SetEnable(false);	
	//m_pSell->SetEnable(false);


	//ZeroMemory(m_szSellCount, 64);				//갯수 표시 조정	
	//wsprintf(m_szSellCount, "%d", m_iSellCount);
	//m_pCounter->SetWindowText(m_szSellCount);

	m_iItemCount = 0;

	//m_pCash->SetWindowText("");	

	//[2005/12/12] - 기존의 툴팁도 삭제해준다
	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, 0);
	pWindow->Show();
}


void SPWindowMailList::RestoreInven()
{
	//인벤토리 리스트에서 리스토어 하도록 
	if(m_pItemStatus == NULL)			return;

	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return;

	//DXUTOutputDebugString("ItemSell Restore ItemStatus ID[%d]\n", m_pItemStatus->GetItemID());

	SPItem* pSellSelctItem = g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetInventoryItem(m_PickUpItem.m_iContainer, m_PickUpItem.m_iSlotIndex);	
	if(m_PickUpItem.m_iContainer > CONTAINER_TYPE_INVALID && 
		m_PickUpItem.m_iContainer < FIGURE_EQUIP)
	{
		CONTAINER_ITEM _item;
		pSellSelctItem->ConvertContainerItem((CONTAINER_ITEM*)&_item);
		g_pInterfaceManager->SPPerformMessage( SPIM_ITEM_SETTING, (WPARAM)&_item, 0);
	}
	g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->SetSellSelectItem(m_PickUpItem);
}



//////////////////////////////////////////////////////////////////////////
SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowMailList )
SPIMESSAGE_COMMAND(SPIM_MAIL_LIST_UPDATE,			OnUpdateMailList )
SPIMESSAGE_COMMAND(SPIM_MAIL_LIST_SENDCLEAR,		OnClearSend )
SPIMESSAGE_COMMAND(SPIM_MAIL_LIST_SENDITEM_RESET,	OnResetSendItem )
SPIMESSAGE_COMMAND(SPIM_MAIL_LIST_CHECK_UPDATE,		OnReadCheck )
SPIMESSAGE_COMMAND(SPIM_KEY_TAB,					OnTab )
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_MAILLIST_SEND		, OnMailSend)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_MAILLIST_VIEW		, OnMailView)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_MAILLIST_PREV		, OnPrev)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_MAILLIST_NEXT		, OnNext)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_MAILLIST_CLOSE		, OnClose)
SPIMESSAGE_CONTROL_RANGE(SPIM_LBUTTON_UP, WIID_MAILLIST_ITEM_BUTTON, WIID_MAILLIST_ITEM_BUTTON + MAX_MAIL_LIST, OnItemClick)
SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_ENTER,	WIID_MAILLIST_ITEM_BUTTON,		WIID_MAILLIST_ITEM_BUTTON + MAX_MAIL_LIST,	OnToolTipEnter	)
SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_OUT,		WIID_MAILLIST_ITEM_BUTTON,		WIID_MAILLIST_ITEM_BUTTON + MAX_MAIL_LIST,	OnToolTipOut	)
SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_MOVE,	WIID_MAILLIST_ITEM_BUTTON,		WIID_MAILLIST_ITEM_BUTTON + MAX_MAIL_LIST,	OnToolTipMove	)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_MAILLIST_SEND_ITEM	, OnButtonUpItem)
SPIMESSAGE_CONTROL(SPIM_TOOLTIP_MOVE,	WIID_MAILLIST_SEND_ITEM,		OnSendItemToolTipMove)
SPIMESSAGE_CONTROL(SPIM_TOOLTIP_ENTER,	WIID_MAILLIST_SEND_ITEM,		OnSendItemToolTipEnter)
SPIMESSAGE_CONTROL(SPIM_TOOLTIP_OUT,	WIID_MAILLIST_SEND_ITEM,		OnSendItemToolTipOut)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_MAILLIST_SEND_MAIL	, OnSend)
SPIMESSAGE_CONTROL(SPIM_REFRESH,	WIID_MAILLIST_SEND_ELY	, OnUpdateEly)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_MAILLIST_TOP		, OnLButtonUp)		//아이템아이콘 리커버
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_MAILLIST_MENU_BASE , OnLButtonUp)		//아이템아이콘 리커버
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_MAILLIST_SEND_SKIN , OnLButtonUp)		//아이템아이콘 리커버
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP, WIID_MAILLIST_SEND_TITLEBASE , OnLButtonUp)	//아이템아이콘 리커버
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP, WIID_MAILLIST_SEND_RECEIVERBASE , OnLButtonUp)//아이템아이콘 리커버
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP, WIID_MAILLIST_SEND_APPEND_1 , OnLButtonUp)	//아이템아이콘 리커버
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP, WIID_MAILLIST_SEND_APPEND_2 , OnLButtonUp)	//아이템아이콘 리커버
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_MAILLIST_ITEM_BOTTOM	, OnLButtonUp)	//아이템아이콘 리커버
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_MAILLIST_SEND_BOTTOM	, OnLButtonUp)	//아이템아이콘 리커버
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_MAILLIST_SENDVIEW	, OnLButtonUp)		//아이템아이콘 리커버
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_MAILLIST_CENTER	, OnLButtonUp)		//아이템아이콘 리커버
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_MAILLIST_SEND_ELY	, OnLButtonUp)		//아이템아이콘 리커버
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_MAILLIST_SEND_TITLE, OnLButtonUp)		//아이템아이콘 리커버
//SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_MAILLIST_SEND_RECEIVERPLATE, OnLButtonUp)	//아이템아이콘 리커버
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_MAILLIST_SEND_RECEIVER, OnLButtonUp)	//아이템아이콘 리커버
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_MAILLIST_SEND_MSG	, OnLButtonUp)		//아이템아이콘 리커버
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_MAILLIST_SEND_MSG_BASE, OnLButtonUp)	//아이템아이콘 리커버
SPIMESSAGE_OBJ_MAP_END	( SPWindow	)

int SPWindowMailList::OnUpdateMailList(WPARAM wParam, LPARAM lParam)
{
	m_iSelectItem	= -1;
	m_iMaxPage		= 0;
	//m_iCurPage		= 0;
	m_iStartIndex	= 0;
	int iSize = g_pEventManager->GetMailManager()->GetMailListSize();
	m_iMaxPage = iSize / MAX_MAIL_LIST;
	if(iSize % MAX_MAIL_LIST > 0)
		m_iMaxPage++;

	if(m_iMaxPage >= MAX_MAIL_PAGE)
		m_iMaxPage = MAX_MAIL_PAGE;

	if(m_iCurPage == m_iMaxPage && m_iCurPage > 0)
		m_iCurPage--;

	if(m_iMaxPage > 1) {
		m_pNext->SetEnable(true);
	}
	else /*if(m_iMaxPage == 0)*/ {
		m_pNext->SetEnable(false);
	}

	if(m_iCurPage == 0) {
		m_pPrev->SetEnable(false);
	}

	UpdateList();
	return 1;
}


int SPWindowMailList::OnClearSend(WPARAM wParam, LPARAM lParam)
{
	ClearSend();
	return 1;
}


int	SPWindowMailList::OnResetSendItem(WPARAM wParam, LPARAM lParam)
{
	ResetItemControl();
	return 1;
}


int SPWindowMailList::OnReadCheck(WPARAM wParam, LPARAM lParam)
{
	if(m_iSelectItem >= 0 && m_iSelectItem < MAX_MAIL_LIST)
	{
		m_pListCheck[m_iSelectItem]->Hide();
		m_pListCheck[m_iSelectItem]->SPSendMessage(SPIM_SET_IMAGECLEAR);
		m_pListCheck[m_iSelectItem]->SetImage(m_pCheck, m_rtSrcCheck.left, m_rtSrcCheck.top);
		m_pListCheck[m_iSelectItem]->Show();
	}
	return 1;
}


int SPWindowMailList::OnTab(WPARAM wParam, LPARAM lParam)
{
	//if( m_pWhisperEdit->IsShow() == true && m_pWhisperEdit->IsFocus() == true )
	//	g_pInterfaceManager->SetFocusWindow( m_pMsgEdit );

	//if(!wParam) {
	//	this->NextTabStop();
	//} else {
	//	this->PrevTabStop();
	//}
	return SPWindow::OnTab(wParam, lParam);
	
}


int SPWindowMailList::OnMailView(WPARAM wParam, LPARAM lParam)
{
	if(g_pGOBManager->GetLocalPlayer())
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);

	m_iMailListMode = MAILLIST_MODE_VIEW;
	SetModeButton();

	for(int i = 0; i < MAX_MAIL_LIST; i++) {
		m_pListSelect[i]->Hide();
	}

	return 1;
}


int SPWindowMailList::OnMailSend(WPARAM wParam, LPARAM lParam)
{
	if(g_pGOBManager->GetLocalPlayer())
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
	
	m_iMailListMode = MAILLIST_MODE_SEND;
	SetModeButton();
	return 1;
}


int SPWindowMailList::OnPrev(WPARAM wParam, LPARAM lParam)
{
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
	
	//메일 확인창
	g_pInterfaceManager->ShowWindow(WIID_MAILMSG, SP_HIDE);	

	return 1;
}


int SPWindowMailList::OnNext(WPARAM wParam, LPARAM lParam)
{
	m_iCurPage++;

	if(m_iCurPage + 1 >= m_iMaxPage) {
		//m_iCurPage--;
		m_pNext->SetEnable(false);
		//UpdateList();
		//return 1;
	}

	if(m_iCurPage + 1 >= MAX_MAIL_PAGE) {		//10페이지 제한
		m_pNext->SetEnable(false);
	}

	if(m_iCurPage > 0) {
		m_pPrev->SetEnable(true);
	}

	UpdateList();

	//메일 확인창
	g_pInterfaceManager->ShowWindow(WIID_MAILMSG, SP_HIDE);	

	return 1;
}


int SPWindowMailList::OnItemClick(unsigned int uiID, WPARAM wParam, LPARAM lParam)
{
	m_iSelectItem = uiID - WIID_MAILLIST_ITEM_BUTTON;	
	assert(m_iSelectItem < MAX_MAIL_LIST && "Mail List 인덱스 에러");	

	DXUTOutputDebugString("Mail Select[%d]\n", m_iSelectItem);

	//control select
	for(int i = 0 ; i < MAX_MAIL_LIST ; i++ ){
		//SPWindow* pkWindow = Find(WIID_MAILLIST_ITEM_BUTTON + i, true);
		//if( pkWindow == NULL )
		//	continue;

		//if( uiID == (WIID_MAILLIST_ITEM_BUTTON + i) )
		//	pkWindow->SPSendMessage(SPIM_BTN_SELECT, TRUE);
		//else
		//	pkWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE);

		if(m_iSelectItem == i) 
			m_pListSelect[i]->Show();
		else
			m_pListSelect[i]->Hide();
	}

	int iSelectIndex = (m_iCurPage * MAX_MAIL_LIST) + m_iSelectItem;
	
	//SPGiftAttr* pGiftAttr = g_pEventManager->GetGiftManager()->GetGift(iSelectIndex);
	//if(pGiftAttr == NULL || pGiftAttr->m_iKey == 0){
	//	m_iSelectItem = -1;
	//	m_pName->SetWindowText(NULL);
	//	m_pText->SetWindowText(NULL);
	//	m_pEly->SetWindowText(NULL);
	//	return 1;
	//}

	//m_pName->SetWindowText(pGiftAttr->m_strName.c_str());
	//m_pText->SetWindowText(pGiftAttr->m_strDesc.c_str());

	//char szEly[32];
	//ZeroMemory(szEly, 32);
	//wsprintf(szEly, "%d", pGiftAttr->m_iEly);
	//m_pEly->SetWindowText(szEly);

	//본문 수신 메시지를 보냄
	SPMailAttr* pMailAttr = g_pEventManager->GetMailManager()->GetMail(iSelectIndex);
	if(pMailAttr == NULL || pMailAttr->m_pHeader->uiMailID == 0) 
		return 1;
	
	if(pMailAttr && pMailAttr->IsSendBody() == false) {
		g_pEventManager->GetMailManager()->SendGetMailBody(iSelectIndex);
	}
	else {												//이미 바디를 받은 경우
		g_pEventManager->GetMailManager()->SetSelectMail(iSelectIndex);
		
		// Msg Window Update
		g_pInterfaceManager->ShowWindow(WIID_MAILMSG, SP_SHOW);	
		SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_MAILMSG);
		if(pWindow) {
			pWindow->SPSendMessage(SPIM_MAIL_MSG_UPDATE, (WPARAM)iSelectIndex, (LPARAM)0);
		}
	}	

	return 1;
}


int SPWindowMailList::OnToolTipMove(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	int iCurIndex = iUiID - WIID_MAILLIST_ITEM_BUTTON;	
	iCurIndex = (m_iCurPage * MAX_MAIL_LIST) + iCurIndex;

	//iCurIndex += (m_iCurPage * 4);

	unsigned int uiUniqueID;
	int iEquipClass, iItemID, iItemColor;

	SPMailAttr* pMailAttr = g_pEventManager->GetMailManager()->GetMail(iCurIndex);
	if(pMailAttr == NULL || pMailAttr->m_pHeader->uiMailID == 0 || pMailAttr->m_pHeader->uiItemID == 0) {
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);		
		return 1;
	}
	
	//uiUniqueID = m_ContainerItem[iCurIndex].Item.iItemID;
	uiUniqueID = pMailAttr->m_pHeader->uiItemID;	
	iItemColor = pMailAttr->m_pHeader->uiItemID % 100;
	iEquipClass = pMailAttr->m_pHeader->uiItemID / 100000;
	iItemID = (pMailAttr->m_pHeader->uiItemID % 100000 - iItemColor) / 100;	

	//SPItemStatus kItemStatus;
	//kItemStatus.SetItemID(uiUniqueID);

	//SPItemAttr* pItemAttr;
	//g_pItemCluster->GetItemInfo(uiUniqueID, pItemAttr);
	if( pMailAttr->m_pItem && pMailAttr->m_pItem->GetItemStatus() && pMailAttr->m_pItem->GetItemAttr())
	{
		//kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPID, pItemAttr->m_ePosID1);
		//kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPCLASS, iEquipClass);
		//kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPITEMID, iItemID);
		//kItemStatus.SPSendMessage(MV_ITEM_SETCOLOR,	iItemColor);
		//kItemStatus.SPSendMessage(MV_ITEM_SET_RARELEVEL, pGiftAttr->m_iItemRare);

		if(pMailAttr->m_pItem->GetItemAttr()->m_eDuration != ITEM_DURATION_NULL) {
			pMailAttr->m_pItem->GetItemStatus()->SetLifeTime(pMailAttr->m_pItem->GetItemAttr()->m_iLifeTime * 3600);
		}

		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage(SPIM_TOOLTIP_MOVE, (WPARAM)pMailAttr->m_pItem->GetItemStatus(), lParam);
		pWindow->Show();
	}

	return 1;
}


int SPWindowMailList::OnToolTipEnter(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	int iCurIndex = iUiID - WIID_MAILLIST_ITEM_BUTTON;	
	iCurIndex = (m_iCurPage * MAX_MAIL_LIST) + iCurIndex;

	//iCurIndex += (m_iCurPage * 4);

	unsigned int uiUniqueID;
	int iEquipClass, iItemID, iItemColor;

	SPMailAttr* pMailAttr = g_pEventManager->GetMailManager()->GetMail(iCurIndex);
	if(pMailAttr == NULL || pMailAttr->m_pHeader->uiMailID == 0 || pMailAttr->m_pHeader->uiItemID == 0) {
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);		
		return 1;
	}

	uiUniqueID = pMailAttr->m_pHeader->uiItemID;	
	iItemColor = pMailAttr->m_pHeader->uiItemID % 100;
	iEquipClass = pMailAttr->m_pHeader->uiItemID / 100000;
	iItemID = (pMailAttr->m_pHeader->uiItemID % 100000 - iItemColor) / 100;	

	//SPItemStatus kItemStatus;
	//kItemStatus.SetItemID(uiUniqueID);

	//SPItemAttr* pItemAttr;
	//g_pItemCluster->GetItemInfo(uiUniqueID, pItemAttr);
	if( pMailAttr->m_pItem && pMailAttr->m_pItem->GetItemStatus() && pMailAttr->m_pItem->GetItemAttr() )
	{
		//kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPID, pItemAttr->m_ePosID1);
		//kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPCLASS, iEquipClass);
		//kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPITEMID, iItemID);
		//kItemStatus.SPSendMessage(MV_ITEM_SETCOLOR,	iItemColor);
		//kItemStatus.SPSendMessage(MV_ITEM_SET_RARELEVEL, pGiftAttr->m_iItemRare);

		if(pMailAttr->m_pItem->GetItemAttr()->m_eDuration != ITEM_DURATION_NULL) {
			pMailAttr->m_pItem->GetItemStatus()->SetLifeTime(pMailAttr->m_pItem->GetItemAttr()->m_iLifeTime * 3600);
		}

		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage(SPIM_TOOLTIP_MOVE, (WPARAM)pMailAttr->m_pItem->GetItemStatus(), lParam);
		pWindow->Show();
	}

	return 1;
}


int SPWindowMailList::OnToolTipOut(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
	pWindow->Show();
	return 1;
}


int SPWindowMailList::OnSend(WPARAM wParam, LPARAM lParam)
{

	DXUTOutputDebugString("OnSend Click\n");
	
	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return 1;	
	
	if(g_pGOBManager->GetLocalPlayer())
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
	
	//MAILBOX_CS_SEND,
	// MAIL_HEADER_ATTACHMENT	mailHeaderInfo[];		// 메일헤더 정보
	// UINT8					uiSubjectLen;			// 제목
	// TCHAR					szSubect[];
	// UINT8					uiMemoLen;				// 본문
	// TCHAR					szMemo[];
	// TCHAR					szReceiver[LEN_NAME+1];	// 수신자	

	MAIL_HEADER_ATTACHMENT stMailHeader;
	ZeroMemory(&stMailHeader, sizeof(MAIL_HEADER_ATTACHMENT));
	std::string strSubject = "";
	std::string strReceiver = "";
	std::string strMemo = "";
	
	strSubject = m_pTitle->GetWindowText();
	//if(strSubject.size() > TITLE_LENGTH) {			//[2006/4/27] 음...
	//	strSubject = strSubject.substr(0, TITLE_LENGTH);
	//}

	UINT8 iSubjectLen = 0;
	if(strSubject != "") {
		iSubjectLen = static_cast<UINT8>(strSubject.size()) + 1;
	}
	else {						//제목이 없는 경우
		std::string strMsg = g_pResourceManager->GetGlobalString(5007007);
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		return 1;
	}

	//[2006/6/13] - 메일 제목 필터링
	char szBuffer[128];
	int iReplace;
	SPAbuseFilter::GetInstance()->Filter(strSubject.c_str(), szBuffer, 127, &iReplace);
	if(iReplace) {				//검출...
		if(g_pResourceManager->GetGlobalString(5007013)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(5007013);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}
		return 1;
	}
	
	strReceiver = m_pReceiver->GetWindowText();	
	if(strReceiver == "") {		//받는사람 이름이 없는 경우
		std::string strMsg = g_pResourceManager->GetGlobalString(5007010);
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		return 1;
	}	

#ifndef _DEBUG
	Trim(strReceiver);				//[2006/5/2] 앞뒤 공백 문자열 제거
	std::string strLocalName = g_pGOBManager->GetLocalPlayer()->GetStatus()->GetGOBName();
	if(strReceiver == strLocalName) {
		std::string strMsg = g_pResourceManager->GetGlobalString(5007012);
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		return 1;
	}
#endif	
	
	strMemo = m_pMsg->GetWindowText();
	UINT8 iMemoLen = 0;
	if(strMemo != "") {
		 iMemoLen = static_cast<UINT8>(strMemo.size()) + 1;

		 //[2006/6/13] - 메일 내용 필터링
		 char szBuffer[128];
		 int iReplace;
		 SPAbuseFilter::GetInstance()->Filter(strMemo.c_str(), szBuffer, 127, &iReplace);
		 if(iReplace) {				//검출...
			 if(g_pResourceManager->GetGlobalString(5007013)) {
				 std::string strMsg = g_pResourceManager->GetGlobalString(5007013);
				 g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
			 }
			 return 1;
		 }
	}	

	//예외처리 및 보정
	//{
////
//#if 0
	//if(strReceiver.size() > LEN_NAME) {
	//	DXUTOutputDebugString("strReceiver.size() > LEN_NAME");
	//	return -1;
	//}

	if(iSubjectLen > MAILBOX_MAX_SUBJECT_LEN) {
		DXUTOutputDebugString("iSubjectLen >= MAILBOX_MAX_SUBJECT_LEN");
		return -1;
	}

	if(iMemoLen > MAILBOX_MAX_MEMO_LEN + 1)	{			//메모 크기에 + 1을 강제적으로 앞에서 함
		DXUTOutputDebugString("iMemoLen >= MAILBOX_MAX_MEMO_LEN");
		return -1;
	}
//#else
	DECLARE_VAR(TCHAR,szReceiver,MAX_LEN_NAME);
	DECLARE_VAR(TCHAR,szSubject,MAILBOX_MAX_SUBJECT_LEN);
	DECLARE_VAR(TCHAR,szMemo,MAILBOX_MAX_MEMO_LEN);

	_tcsncpy(szReceiver,strReceiver.c_str(),LEN_NAME);
	_tcsncpy(szSubject,strSubject.c_str(),MAILBOX_MAX_SUBJECT_LEN);
	_tcsncpy(szMemo,strMemo.c_str(),MAILBOX_MAX_MEMO_LEN);

	iSubjectLen = BETWEEN(0,iSubjectLen,MAILBOX_MAX_SUBJECT_LEN);
	iMemoLen = BETWEEN(0,iMemoLen,MAILBOX_MAX_MEMO_LEN);
//#endif
	//}

	//MAIL_HEADER_ATTACHMENT
	if(g_pGOBManager->GetLocalPlayer()) {
		wsprintf(stMailHeader.szSender, "%s", g_pGOBManager->GetLocalPlayer()->GetStatus()->GetGOBName());
	}

	stMailHeader.uiItemID = 0;
	stMailHeader.uiItemNo = 0;
	stMailHeader.uiItemCount = 0;
	stMailHeader.uiContainerType = -1;
	stMailHeader.uiContainerSlot = 0;

	//[2006/4/18]	
	if(m_pItemStatus) {
		//픽업 아이템이 있는 경우
		if(g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->m_bPickup == true) {			
			return 1;
		}

		//사망시 제한
		if(g_pGOBManager->GetLocalPlayer()->GetCoordPhysics()->GetCurState() == GOB_STATE_DEAD) {
			DXUTOutputDebugString("Item Sell Error 0-1 Player Dead\n");
			return 1;
		}

		if(m_pItemStatus == NULL) {
			DXUTOutputDebugString("Item Sell Error 1 m_pItemStatus NULL\n");
			return  1;	
		}

		if(m_pItemStatus->GetItemID() == 0) {
			DXUTOutputDebugString("Item Sell Error 2 m_pItemStatus ID 0\n");
			return  1;	
		}	

		SPItemAttr* pItemAttr = m_pItemStatus->GetItemAttr();
		if(pItemAttr == NULL) {
			DXUTOutputDebugString("Item Sell Error 4 pItemAttr NULL\n");
			return 1;
		}
		
		if(pItemAttr->m_bTrade == false) {		
			//OnItemSellReset((WPARAM)0, (LPARAM)0);					//[2005/11/1] 판매 등록 해제
			return 1 ;
		}
		
		if(m_PickUpItem.m_iContainer < INVENTORY_EQUIP || m_PickUpItem.m_iContainer > INVENTORY_HOUSING)
			return 1;
		if(m_PickUpItem.m_iSlotIndex < 0 || m_PickUpItem.m_iSlotIndex >= g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetInventorySize(m_PickUpItem.m_iContainer))
			return 1;

		//CONTAINER_ITEM item;
		//item.SlotInfo.ContainerType = m_PickUpItem.m_iContainer;
		//item.SlotInfo.SlotIndex = m_PickUpItem.m_iSlotIndex + 1;
		//item.Item.iItemID = m_pItemStatus->GetItemID();
		//item.Item.iStackCount = m_iSellCount;

		//if(g_pCheckManager->IsDBBlock()) {
		//	
		//	DXUTOutputDebugString("\tSPWindowItemSell::OnSell DB Working Block\n");
		//	return 1;
		//}

		stMailHeader.uiItemID = m_pItemStatus->GetItemID();
		stMailHeader.uiItemNo = *(ITEMNO*)m_pItemStatus->SPSendMessage(MV_ITEM_GET_ITEMNO);
		stMailHeader.uiItemCount = m_pItemStatus->SPSendMessage(MV_ITEM_GETSTACK_CNT);;
		stMailHeader.uiContainerType = m_PickUpItem.m_iContainer;
		stMailHeader.uiContainerSlot = m_PickUpItem.m_iSlotIndex + 1;
	}

	stMailHeader.uiEly = _atoi64(m_pEly->GetWindowText());

	//메일에 내용이 없는 경우
	if(strMemo == "" && stMailHeader.uiEly == 0 && stMailHeader.uiItemID == 0) {
		std::string strMsg = g_pResourceManager->GetGlobalString(5007005);
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		return 1;
	}
	
	//보내는돈 + 수수료 가 현재 금액보다 많은 경우
	UINT64 iCurMoney = g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValueEX(STATUS_EX_ELY);
	int iFee = g_pCheckManager->GetMailFee();
	if(MAILBOX_SEND_FEE + stMailHeader.uiEly > iCurMoney) {
		if(g_pResourceManager->GetGlobalString(5007011)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(5007011);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}
		return 1;
	}
	
	// 우표 아이템 체크
	CONTAINER_SLOT Slot;
	Slot.ContainerType = CONTAINER_TYPE_INVALID;
	Slot.SlotIndex = 0;	
	INT64 iItemNo = 0;
	int iMailStampItemID = g_pCheckManager->GetMailStampItemID();
	if(iMailStampItemID) {
		SPPlayerInvenArchive* pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
		if(pInven == NULL) {
			return 1;
		}

		SPItemAttr* pItemAttr = NULL;
		if(g_pItemCluster->GetItemInfo(iMailStampItemID, pItemAttr) == false) {
			return 1;
		}

		if(pItemAttr == NULL) {
			return 1;
		}

		CONTAINER_TYPE iContainer = (CONTAINER_TYPE)(pItemAttr->m_eType - 1);

		SPItem* pItem = pInven->GetSPItem(iContainer, iMailStampItemID);
		if(pItem == NULL) {
			if(g_pResourceManager->GetGlobalString(5007015)) {
				std::string strMsg = g_pResourceManager->GetGlobalString(5007015); //우편을 보내기 위해선 XX 아이템이 필요합니다.
				char szTemp[256];
				ZeroMemory(szTemp, 256);
				sprintf(szTemp, strMsg.c_str(), pItemAttr->m_strName.c_str());
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szTemp);
			}
			return 1;
		}
		
		Slot.ContainerType = pItem->GetContainerType();
		Slot.SlotIndex = pItem->GetSlotIndex() + 1;
		iItemNo = *(ITEMNO*)pItem->GetItemStatus()->SPSendMessage( MV_ITEM_GET_ITEMNO );
	}

	if(g_pCheckManager->IsDBBlock()) {
		DXUTOutputDebugString("\tSPWindowMailList::OnSend DB Working Block\n");
		return 1;
	}

	if(g_pEventManager->GetCurEvent() == NULL)
		return 1;
	
	CPacket Packet;
	Packet.Add((UINT32)MAILBOX_CS_SEND);
	Packet.AddData(&stMailHeader, sizeof(MAIL_HEADER_ATTACHMENT));
	Packet.AddUINT8(iSubjectLen);
	Packet.AddData(strSubject.c_str(), iSubjectLen);
	Packet.AddUINT8(iMemoLen);
	Packet.AddData(strMemo.c_str(), iMemoLen);
	Packet.AddData(szReceiver, LEN_NAME+1);
	Packet.AddUINT32(g_pEventManager->GetCurEvent()->eventTypeID);
	Packet.AddData(&Slot, sizeof(CONTAINER_SLOT));
	Packet.AddUINT64(iItemNo);
	g_pNetworkManager->SPPerformMessage(MAILBOX_CS_SEND, 0, (LPARAM)&Packet);	
	
	g_pCheckManager->SetDBBlock(true);
	DXUTOutputDebugString(" - OnMailSend\n");
	return 1;
}


int	SPWindowMailList::OnButtonUpItem(WPARAM wParam, LPARAM lParam)
{
	//아이템을 셋팅하기 전에 이전의 선택된 아이템의 비활성화를 풀어준다
	UINT32 iPrevItemID = 0;
	int iPrevSlotIndex = 0;

	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return 1;

	//사망시 제한
	if(g_pGOBManager->GetLocalPlayer()->GetCoordPhysics()->GetCurState() == GOB_STATE_DEAD) {
		ResetItemControl();
		return 1;
	}

	//픽업 상태가 아닌경우 넘어간다
	if(g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->m_bPickup == false) {
		ResetItemControl();
		return 1;
	}

	if(m_pItemStatus) {
		//초기화전 이전 Item에 대한 정보만 잠깐 보관한다
		iPrevItemID = m_pItemStatus->GetItemID();
		iPrevSlotIndex = m_pItemStatus->GetItemSlotIndex();
	}
	//판매 윈도우 환경을 초기화 한다 

	ResetItemControl();

	//실제 받은 아이템 셋팅	
	if(g_pGOBManager->GetLocalPlayer()) {
		SPPlayerInvenArchive* pInvenArchive = NULL;							//[2005/5/25]
		pInvenArchive = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();				

		m_PickUpItem = g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetPickupItem();
		SPItem* pSellSelectItem = g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetInventoryItem(m_PickUpItem.m_iContainer, m_PickUpItem.m_iSlotIndex);				
		m_pItemStatus = pSellSelectItem->GetItemStatus();

		if( m_PickUpItem.GetContainerType() >= INVENTORY_HOUSING ) {
			//DXUTOutputDebugString("ItemSell Equip, Costume... Inven.. \n")	;
			if(g_pGOBManager->GetLocalPlayer())
				g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);

			ResetItemControl(false);
			return 1;
		}

		//
		//Cash Item인 경우
		if(m_pItemStatus->GetItemAttr()->m_bCashCheck) {
			if(g_pGOBManager->GetLocalPlayer())
				g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);

			ResetItemControl(false);
			return 1;
		}
		
		//트레이드가 안되는 아이템
		if(m_pItemStatus->GetItemAttr()->m_bTrade == false) {
			if(g_pGOBManager->GetLocalPlayer())
				g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);

			ResetItemControl(false);
			return 1;
		}		

		//시간 제한이 걸린 아이템
		if(m_pItemStatus->GetItemAttr()->GetDurationType() > ITEM_DURATION_NULL) {
			if(g_pGOBManager->GetLocalPlayer())
				g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);

			ResetItemControl(false);
			return 1;
		}

		//DXUTOutputDebugString("ItemSell Get ItemStatus ID[%d]\n", m_pItemStatus->GetItemID());
	}

	if(m_pItemStatus == NULL)	{			//아이템이 없는 경우
		DXUTOutputDebugString("ItemSell item NULL \n")	;
		return 1;
	}

	//받은 아이템이 판매가 가능한 아이템인지 확인한다 
	//m_bSend = false;	

	//아이템 셋팅
	if(m_pItemStatus != NULL) {
		m_iStorage = g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetCurrentItemPage();

		POINT ptSrcPos;
		SPTexture* pTexture = g_pItemCluster->GetItemTexture(m_pItemStatus->GetItemID(), ptSrcPos);
		//m_iMark = g_pItemCluster->GetItemIconMark(m_pItemStatus->GetItemID());		
		m_pItemIcon->Hide();
		m_pItemIcon->SetImage(pTexture, ptSrcPos.x, ptSrcPos.y);
		m_pItemIcon->Show();

		//m_pName->SetFontColor(GetItemColor(m_pItemStatus->GetRareLevel()));
		if(m_pItemStatus->GetItemAttr()->m_bNameColor) {
			m_pItemName->SetFontColor(m_pItemStatus->GetItemAttr()->GetNameColor());
		}
		else {
			m_pItemName->SetFontColor(g_pCheckManager->GetItemColor(m_pItemStatus->GetRareLevel()));
		}

		m_pItemName->SetWindowText(m_pItemStatus->GetItemAttr()->m_strName.c_str());
		m_iItemCount = m_pItemStatus->SPSendMessage(MV_ITEM_GETSTACK_CNT);	
		m_iItemMark = g_pItemCluster->GetItemIconMark(m_pItemStatus->GetItemID());

		//제목갱신
		std::string strTitle = m_pTitle->GetWindowText();
		if(strTitle == "") {
			m_pTitle->SetWindowText(NULL);
			if(m_iItemCount > 1) {
				char szTitle[100];
				ZeroMemory(szTitle, 100);
				//wsprintf(szTitle, "%s (%d)", m_pItemStatus->GetItemAttr()->m_strName.c_str(), m_iItemCount);
				wsprintf(szTitle, "%s", m_pItemStatus->GetItemAttr()->m_strName.c_str());
				m_pTitle->SetWindowText(szTitle);
			}
			else {		
				m_pTitle->SetWindowText(m_pItemStatus->GetItemAttr()->m_strName.c_str());
			}
		}		

		//SPNpcShop* pNpcShop = g_pEventManager->GetEventHandler()->GetNpcShopData();
		SPItemAttr* pItemAttr = m_pItemStatus->GetItemAttr();
		//int iPrice = GET_SELL_PRICE(pItemAttr->m_iGamePrice, m_iSellCount, pNpcShop->m_iBuyFactor, 100);
		//m_pCash->SetWindowText(ConvertMoneyToString(iPrice));

		//[2005/10/11]	현재 올린 아이템이 내가 사용할수있는 아이템인지?
		//m_bAble = g_pCheckManager->CheckItemStatus(m_pItemStatus->GetItemID());		

		//커서에 올라가있는 아이템을 삭제한뒤 인벤토리를 갱신한다		
		//인벤토리 리스트에서 리스토어 하고 슬롯 비활성화
		RestoreInven();

		//커서 이미지 삭제
		if(g_pGOBManager->GetLocalPlayer())
			g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);

		g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->SetSellSelectItem(m_PickUpItem);		

		//DXUTOutputDebugString("Item Sell SendMessage OK\n");
		//if(m_iSellCount > 1)							//스택갯수가 1이상인 경우에만
		//	m_pCounter->SetFocus();
	}		

	return 1;
}


int SPWindowMailList::OnSendItemToolTipEnter(WPARAM wParam, LPARAM lParam)
{
	if(m_pItemStatus == NULL || m_pItemStatus->GetItemID() == 0) {
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
		return 0;
	}	

	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage( SPIM_TOOLTIP_ENTER, (WPARAM)m_pItemStatus, lParam);
	pWindow->Show();

	return 1;
}


int SPWindowMailList::OnSendItemToolTipOut(WPARAM wParam, LPARAM lParam)
{
	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
	pWindow->Show();

	return 1;
}


int SPWindowMailList::OnSendItemToolTipMove(WPARAM wParam, LPARAM lParam)
{
	if(m_pItemStatus == NULL || m_pItemStatus->GetItemID() == 0) {
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
		return 0;
	}	

	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage( SPIM_TOOLTIP_MOVE, (WPARAM)m_pItemStatus, lParam);
	pWindow->Show();
	return 1;
}



int SPWindowMailList::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{
	if(g_pGOBManager->GetLocalPlayer())
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
	return 1;
}


int SPWindowMailList::OnClose(WPARAM wParam, LPARAM lParam)
{
	g_pEventManager->SetCutInState(CUT_IN_STATE_RELEASE);

	DXUTOutputDebugString("SPWindowMailList::OnClose::CUT_IN_STATE_NULL\n");
	
	return 1;
}


int SPWindowMailList::OnUpdateEly(WPARAM wParam, LPARAM lParam)
{
	INT64 iEly = atoi(m_pEly->GetWindowText());
	INT64 iUserEly = 0;

	if(g_pGOBManager->GetLocalPlayer()) {
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
		iUserEly = g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValueEX(STATUS_EX_ELY);
	}

	if(iEly < 0 || iEly > iUserEly) {
		iEly = iUserEly;
		char szTemp[32];
		ZeroMemory(szTemp, 32);
		//wsprintf(szTemp, "%I64d", iEly);
		sprintf(szTemp, "%I64d", iEly);
		//wsprintf(szTemp, "%d", iEly);
		m_pEly->SetWindowText(szTemp);
	}

	return 1;
}

