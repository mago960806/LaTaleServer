
#include "SPCommon.h"
#include "SPUtil.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "Packet.h"
#include "PacketID.h"
#include "SPNetworkManager.h"

#include "SPStage.h"
#include "SPStageManager.h"

#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPItem.h"
#include "SPItemCluster.h"

#include "SPEventDEF.h"
#include "SPEventNpcHandler.h"
#include "SPEventManager.h"

#include "Packet.h"
#include "PacketID.h"
#include "SPNetworkManager.h"

#include "SPMainGameDEF.H"
#include "SPSubGameManager.h"
#include "SPMainGameManager.h"
#include "SPMouseCursor.h"
#include "SPWindowDEF.h"
#include "SPInterfaceManager.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGOBModel.h"
#include "SPGOBModelUnit.h"
#include "SPMonsterModelUnit.h"
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
#include "SPPlayer.h"
#include "SPGOBManager.h"

#include "SPCheckManager.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPWindowNpcEvent.h"

#include "SPDebug.h"


void SPNpcEventToolTip::Clear()
{
	m_strTitle.clear();	
	m_pItem.SetItemID(0);
	m_strDesc.clear();
}


void SPNpcEventToolTip::SetTitle(std::string strTitle)
{
	if(strTitle.empty() == false) {
		m_strTitle = strTitle;
		ZeroMemory(&m_pItem, sizeof(SPItemStatus));
		m_pItem.SetItemID(0);
	}
}


SPWindowNpcEvent::SPWindowNpcEvent( INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent /*= NULL*/)
: SPWindow(WNDID_NPC_EVENT, InstanceID, iX, iY, iCX, iCY, pParent)	
{
	Init();
}


SPWindowNpcEvent::~SPWindowNpcEvent()
{
	Clean();
}


void SPWindowNpcEvent::Init()
{
	g_pVideo->GetTextureMgr()->LoadTexture("DATA/INTERFACE/CONCEPT/UI105.PNG" , &m_pTextureBase);	

	SetRect(&m_rtSrcBase[WINDOW_BASE_TOP_LEFT],		482, 346, 486, 350);
	SetRect(&m_rtSrcBase[WINDOW_BASE_TOP],			488, 346, 490, 350);	
	SetRect(&m_rtSrcBase[WINDOW_BASE_TOP_RIGHT],	492, 346, 496, 350);
	SetRect(&m_rtSrcBase[WINDOW_BASE_CENTER_LEFT],	482, 352, 486, 354);
	SetRect(&m_rtSrcBase[WINDOW_BASE_CENTER],		488, 352, 490, 354);
	SetRect(&m_rtSrcBase[WINDOW_BASE_CENTER_RIGHT],	492, 352, 496, 354);
	SetRect(&m_rtSrcBase[WINDOW_BASE_BOTTOM_LEFT],	482, 356, 486, 360);
	SetRect(&m_rtSrcBase[WINDOW_BASE_BOTTOM],		488, 356, 490, 360);
	SetRect(&m_rtSrcBase[WINDOW_BASE_BOTTOM_RIGHT],	492, 356, 496, 360);	

	SetRect(&m_rtDestBase[WINDOW_BASE_TOP_LEFT],		m_iAX, m_iAY, m_iAX + 4, m_iAY + 4);
	SetRect(&m_rtDestBase[WINDOW_BASE_TOP],				m_iAX + 4, m_iAY, m_iAX + 4 + 224, m_iAY + 4);	
	SetRect(&m_rtDestBase[WINDOW_BASE_TOP_RIGHT],		m_iAX + 228, m_iAY, m_iAX + 228 + 4, m_iAY + 4);
	SetRect(&m_rtDestBase[WINDOW_BASE_CENTER_LEFT],		m_iAX, m_iAY + 4, m_iAX + 4, m_iAY + 4 + 433);
	SetRect(&m_rtDestBase[WINDOW_BASE_CENTER],			m_iAX + 4, m_iAY + 4, m_iAX + 4 + 224, m_iAY + 4 + 433);
	SetRect(&m_rtDestBase[WINDOW_BASE_CENTER_RIGHT],	m_iAX + 228, m_iAY + 4, m_iAX + 228 + 4, m_iAY + 4 + 433);
	SetRect(&m_rtDestBase[WINDOW_BASE_BOTTOM_LEFT],		m_iAX, m_iAY + 437, m_iAX + 4, m_iAY + 437 + 4);
	SetRect(&m_rtDestBase[WINDOW_BASE_BOTTOM],			m_iAX + 4, m_iAY + 437, m_iAX + 4 + 224, m_iAY + 437 + 4);
	SetRect(&m_rtDestBase[WINDOW_BASE_BOTTOM_RIGHT],	m_iAX + 228, m_iAY + 437, m_iAX + 228 + 4, m_iAY + 437 + 4);
	
	g_pVideo->GetTextureMgr()->LoadTexture("DATA/INTERFACE/CONCEPT/UI100.PNG", &m_pDisable);

	SetRect(&m_rtSrcDisable, 434 , 37 , 436 , 39);
	SetRect(&m_rtMarkSrc[0], 479, 152, 511, 184);		//ICON_MARK_LOCK
	SetRect(&m_rtMarkSrc[1], 479, 185, 511, 217);		//ICON_MARK_FASHION
	SetRect(&m_rtMarkSrc[2], 479, 339, 511, 371);		//ICON_MARK_EVENT
	SetRect(&m_rtMarkSrc[3], 479, 372, 511, 404);		//ICON_MARK_FANCY
	
	InitSubControl();
}


void SPWindowNpcEvent::InitSubControl()
{
	char* OPTION_IMAGE_FILE0 = "DATA/INTERFACE/CONCEPT/UI100.PNG";
	char* OPTION_IMAGE_FILE1 = "DATA/INTERFACE/CONCEPT/UI101.PNG";
	char* OPTION_IMAGE_FILE2 = "DATA/INTERFACE/CONCEPT/UI102.PNG";
	char* OPTION_IMAGE_FILE3 = "DATA/INTERFACE/CONCEPT/UI103.PNG";
	char* OPTION_IMAGE_FILE4 = "DATA/INTERFACE/CONCEPT/UI104.PNG";
	char* OPTION_IMAGE_FILE5 = "DATA/INTERFACE/CONCEPT/UI105.PNG";
	char* OPTION_IMAGE_FILE6 = "DATA/INTERFACE/CONCEPT/UI106.PNG";
	char* OPTION_IMAGE_FILE7 = "DATA/INTERFACE/CONCEPT/UI107.PNG";

	//// Control
	SPWindowButton* pButton = NULL;
	SPWindowStatic* pStatic = NULL;
	SPWindowStatic* pParent = NULL;
	SPWindowStatic* pSkin	= NULL;
	
	//WIID_NPC_EVENT_TITLE
	pStatic = new SPWindowStatic(WIID_NPC_EVENT_TITLE,	7,	9,	196,	14, this);	
	//pStatic->SetImage(OPTION_IMAGE_FILE7, 266, 479);
	pStatic->SetFont(FONT_12_BOLD);
	pStatic->SetFormat(DT_LEFT | DT_VCENTER);
	pStatic->SetFontColor(RGBA(255, 255, 255, 255));
	pStatic->SetFontShadowColor(RGBA(143, 99, 99, 255));
	pStatic->SetFontShadow(true) ;
	pStatic->SetWindowText("Test000");
	pStatic->Show();
	m_pTitle = pStatic;
	pStatic = NULL;

	//WIID_NPC_EVENT_EXIT
	pButton = new SPWindowButton(WIID_NPC_EVENT_EXIT , 211, 9, 12, 12, this);
	pButton->SetImage		(OPTION_IMAGE_FILE3, 301,  321);		
	pButton->SetImageHit	(OPTION_IMAGE_FILE3, 314,  321);
	pButton->SetImagePush	(OPTION_IMAGE_FILE3, 327,  321);
	pButton->SetImageDisable(OPTION_IMAGE_FILE3, 340,  321);
	pButton->Show();

	//WIID_NPC_EVENT_LINE

	//WIID_NPC_EVENT_BASE	
	pSkin = new SPWindowStatic(WIID_NPC_EVENT_BASE,	2,	27,	228,	253, this);	
	pSkin->SetImage(OPTION_IMAGE_FILE5, 488, 337);
	pSkin->SetSrcSize(2, 2);
	pSkin->Show();

	//WIID_NPC_EVENT_LIST_TITLE_SKIN	
	pParent = new SPWindowStatic(WIID_NPC_EVENT_LIST_TITLE_SKIN,	11 - 2,	34 - 27,	210,	20, pSkin);	
	pParent->SetWindowType( WND_TYPE_LEFTBASE );
	pParent->SetImageNormalHead(OPTION_IMAGE_FILE7, 501, 472, 503, 492);
	pParent->SetImageNormalBody(OPTION_IMAGE_FILE7, 505, 472, 507, 492);
	pParent->SetImageNormalTail(OPTION_IMAGE_FILE7, 509, 472, 511, 492);
	pParent->Show();
	
	//WIID_NPC_EVENT_LIST_TITLE		
	pStatic = new SPWindowStatic(WIID_NPC_EVENT_LIST_TITLE,	43 - 11,	38 - 34,	148,	13, pParent);	
	pStatic->SetImage(OPTION_IMAGE_FILE7, 266, 465);
	pStatic->Show();
	pStatic = NULL;
	pParent = NULL;
	
	int iStartY = 60 - 27;
	for(int i = 0; i < EVENT_PAGE; i++) {
		//WIID_NPC_EVENT_LIST_SKIN
		pParent = new SPWindowStatic(WIID_NPC_EVENT_LIST_SKIN + i,	21 - 2,	iStartY + (i * 34),	190, 32, pSkin);
		pParent->Show();
		
		//WIID_NPC_EVENT_LIST_BUTTON
		m_pButton[i] = new SPWindowButton(WIID_NPC_EVENT_LIST_BUTTON + i, 0, 0, 278, 36, pParent);
		m_pButton[i]->Show();

		//WIID_NPC_EVENT_LIST_SELECT
		m_pSelect[i] = new SPWindowStatic(WIID_NPC_EVENT_LIST_SELECT + i, 0, 0, 278, 32, pParent);
		m_pSelect[i]->SetImage(OPTION_IMAGE_FILE3, 321, 146);
		m_pSelect[i]->Show();

		//WIID_NPC_EVENT_LIST_ICONSKIN
		m_pItemSkin[i] = new SPWindowStatic(WIID_NPC_EVENT_LIST_ICONSKIN + i, 23 - 21, 0, 32, 32, pParent);
		m_pItemSkin[i]->SetImage(OPTION_IMAGE_FILE3, 203, 375);
		m_pItemSkin[i]->Show();
		
		//WIID_NPC_EVENT_LIST_ICON
		m_pItemIcon[i] = new SPWindowStatic(WIID_NPC_EVENT_LIST_ICON + i, 23-21, 0, 32, 32, pParent);
		m_pItemIcon[i]->Show();
		m_pItemIcon[i]->GetWindowRect(m_rtRewordStack[i]);		

		//WIID_NPC_EVENT_LIST_NAME
		m_pName[i] = new SPWindowStatic(WIID_NPC_EVENT_LIST_NAME + i, 61 - 21, 11, 140, 13, pParent);
		m_pName[i]->SetWindowText("Test000");
		m_pName[i]->Show();		
	}

	//WIID_NPC_EVENT_PAGE_SKIN
	m_pPage = new SPWindowStatic(WIID_NPC_EVENT_PAGE_SKIN,	92 - 2,	264 - 27,	48,	14, pSkin);	
	m_pPage->SetImage(OPTION_IMAGE_FILE3, 226, 345);
	m_pPage->SetFormat(DT_CENTER | DT_VCENTER);
	m_pPage->SetFontColor(RGBA(255, 255, 255, 255));
	m_pPage->SetWindowText("0 / 0");
	m_pPage->Show();

	//WIID_NPC_EVENT_PAGE_PREV
	m_pPrev = new SPWindowButton(WIID_NPC_EVENT_PAGE_PREV , 82 - 2, 266 - 27, 7, 11, pSkin);
	m_pPrev->SetImage		(OPTION_IMAGE_FILE3, 238,  378);
	m_pPrev->SetImageHit	(OPTION_IMAGE_FILE3, 246,  378);
	m_pPrev->SetImagePush	(OPTION_IMAGE_FILE3, 254,  378);
	m_pPrev->SetImageDisable(OPTION_IMAGE_FILE3, 262,  378);
	m_pPrev->Show();
	
	//WIID_NPC_EVENT_PAGE_NEXT
	m_pNext = new SPWindowButton(WIID_NPC_EVENT_PAGE_NEXT , 144 - 2, 266 - 27, 7, 11, pSkin);
	m_pNext->SetImage		(OPTION_IMAGE_FILE3, 269,  378);
	m_pNext->SetImageHit	(OPTION_IMAGE_FILE3, 277,  378);
	m_pNext->SetImagePush	(OPTION_IMAGE_FILE3, 285,  378);
	m_pNext->SetImageDisable(OPTION_IMAGE_FILE3, 293,  378);
	m_pNext->Show();

	//WIID_NPC_EVENT_ARROW
	pStatic = new SPWindowStatic(WIID_NPC_EVENT_ARROW,	23 - 2,	263 - 27,	33,	17, pSkin);	
	pStatic->SetImage(OPTION_IMAGE_FILE7, 380, 479);
	pStatic->Show();
	pStatic = NULL;

	////////////////////////////////////////////////////////////////////////// 하단
	pSkin = NULL;

	//WIID_NPC_EVENT_REQ_ITEM_SKIN
	pSkin = new SPWindowStatic(WIID_NPC_EVENT_REQ_ITEM_SKIN,	7,	285,	218,	102, this);	
	pSkin->SetWindowType( WND_TYPE_LEFTBASE );
	pSkin->SetImageNormalHead(OPTION_IMAGE_FILE7, 501, 61, 503, 163);
	pSkin->SetImageNormalBody(OPTION_IMAGE_FILE7, 505, 61, 507, 163);
	pSkin->SetImageNormalTail(OPTION_IMAGE_FILE7, 509, 61, 511, 163);
	pSkin->Show();

	//WIID_NPC_EVENT_REQ_ITEM_TEXT_SKIN
	pParent = new SPWindowStatic(WIID_NPC_EVENT_REQ_ITEM_TEXT_SKIN,	10 - 7,	288 - 285,	212,	18, pSkin);	
	pParent->SetWindowType( WND_TYPE_LEFTBASE );
	pParent->SetImageNormalHead(OPTION_IMAGE_FILE7, 501, 493, 503, 511);
	pParent->SetImageNormalBody(OPTION_IMAGE_FILE7, 505, 493, 507, 511);
	pParent->SetImageNormalTail(OPTION_IMAGE_FILE7, 509, 493, 511, 511);
	pParent->Show();

	//WIID_NPC_EVENT_REQ_ITEM_TEXT
	pStatic = new SPWindowStatic(WIID_NPC_EVENT_REQ_ITEM_TEXT,	57 - 10,	290 - 288,	122,	15, pParent);	
	pStatic->SetImage(OPTION_IMAGE_FILE7, 266, 449);
	pStatic->Show();
	pStatic = NULL;
	pParent = NULL;

	//WIID_NPC_EVENT_REQ_ITEM_ELY_IMG_L
	pStatic = new SPWindowStatic(WIID_NPC_EVENT_REQ_ITEM_ELY_IMG_L,	30 - 7,	347 - 285,	33,	14, pSkin);	
	pStatic->SetImage(OPTION_IMAGE_FILE7, 467, 497);
	pStatic->Show();
	pStatic = NULL;

	//WIID_NPC_EVENT_REQ_ITEM_ELY_SKIN
	pParent = new SPWindowStatic(WIID_NPC_EVENT_REQ_ITEM_ELY_SKIN,	67 - 7,	345 - 285,	154,	18, pSkin);	
	pParent->SetWindowType( WND_TYPE_LEFTBASE );
	pParent->SetImageNormalHead(OPTION_IMAGE_FILE7, 501, 453, 503, 471);
	pParent->SetImageNormalBody(OPTION_IMAGE_FILE7, 505, 453, 507, 471);
	pParent->SetImageNormalTail(OPTION_IMAGE_FILE7, 509, 453, 511, 471);
	pParent->Show();
	
	//WIID_NPC_EVENT_REQ_ITEM_ELY
	m_pReqEly = new SPWindowStatic(WIID_NPC_EVENT_REQ_ITEM_ELY,	71 - 67,	348 - 345,	116,	13, pParent);	
	m_pReqEly->SetFormat(DT_RIGHT | DT_VCENTER);
	m_pReqEly->SetFontColor(RGBA(144, 99, 99, 255));
	m_pReqEly->SetWindowText("Ely Area");
	m_pReqEly->Show();	

	//WIID_NPC_EVENT_REQ_ITEM_ELY_IMG_R
	pStatic = new SPWindowStatic(WIID_NPC_EVENT_REQ_ITEM_ELY_IMG_R,	191 - 67,	355 - 345,	17,	5, pParent);	
	pStatic->SetImage(OPTION_IMAGE_FILE7, 454, 418);
	pStatic->Show();
	pStatic = NULL;
	pParent = NULL;

	//WIID_NPC_EVENT_REQ_ITEM_PVP_IMG_L
	pStatic = new SPWindowStatic(WIID_NPC_EVENT_REQ_ITEM_PVP_IMG_L,	9 - 7,	368 - 285,	54,	14, pSkin);	
	pStatic->SetImage(OPTION_IMAGE_FILE7, 412, 497);
	pStatic->Show();
	pStatic = NULL;
	
	//WIID_NPC_EVENT_REQ_ITEM_PVP_SKIN
	pParent = new SPWindowStatic(WIID_NPC_EVENT_REQ_ITEM_PVP_SKIN,	67 - 7,	366 - 285,	154,	18, pSkin);	
	pParent->SetWindowType( WND_TYPE_LEFTBASE );
	pParent->SetImageNormalHead(OPTION_IMAGE_FILE7, 501, 453, 503, 471);
	pParent->SetImageNormalBody(OPTION_IMAGE_FILE7, 505, 453, 507, 471);
	pParent->SetImageNormalTail(OPTION_IMAGE_FILE7, 509, 453, 511, 471);
	pParent->Show();
	
	//WIID_NPC_EVENT_REQ_ITEM_PVP
	m_pReqPvpPoint = new SPWindowStatic(WIID_NPC_EVENT_REQ_ITEM_PVP,	71 - 67,	369 - 366,	116,	13, pParent);	
	m_pReqPvpPoint->SetFormat(DT_RIGHT | DT_VCENTER);
	m_pReqPvpPoint->SetFontColor(RGBA(144, 99, 99, 255));
	m_pReqPvpPoint->SetWindowText("PvpPoint Area");
	m_pReqPvpPoint->Show();
	
	//WIID_NPC_EVENT_REQ_ITEM_PVP_IMG_R
	pStatic = new SPWindowStatic(WIID_NPC_EVENT_REQ_ITEM_PVP_IMG_R,	191 - 67,	376 - 366,	27,	5, pParent);
	pStatic->SetImage(OPTION_IMAGE_FILE7, 142, 37);
	pStatic->Show();
	pStatic = NULL;
	pParent = NULL;

	int iStartX[EVENT_REQ_ITEM] = {17, 72, 128, 183};
	for(int i = 0; i < EVENT_REQ_ITEM; i++) {
		//WIID_NPC_EVENT_REQ_ITEM_ICONSKIN
		pParent = new SPWindowStatic(WIID_NPC_EVENT_REQ_ITEM_ELY_SKIN + i,	iStartX[i] - 7,	311 - 285,	32,	32, pSkin);	
		pParent->SetImage(OPTION_IMAGE_FILE3, 203, 375);
		pParent->Show();		

		//WIID_NPC_EVENT_REQ_ITEM_ICONBUTTON
		m_pReqItemButton[i] = new SPWindowButton(WIID_NPC_EVENT_REQ_ITEM_ICONBUTTON + i, 0, 0, 32, 32, pParent);
		m_pReqItemButton[i]->Show();

		//WIID_NPC_EVENT_REQ_ITEM_ICON
		m_pReqItemIcon[i] = new SPWindowStatic(WIID_NPC_EVENT_REQ_ITEM_ICON + i, 0, 0, 32, 32, pParent);
		m_pReqItemIcon[i]->Show();
		m_pReqItemIcon[i]->GetWindowRect(m_rtReqStack[i]);
	}	

	//WIID_NPC_EVENT_REQ_ITEM_PLUS1
	pStatic = new SPWindowStatic(WIID_NPC_EVENT_REQ_ITEM_PLUS1,	53 - 7,	319 - 285,	16,	16, pSkin);	
	pStatic->SetImage(OPTION_IMAGE_FILE3, 239, 423);
	pStatic->Show();
	pStatic = NULL;
	
	//WIID_NPC_EVENT_REQ_ITEM_PLUS2
	pStatic = new SPWindowStatic(WIID_NPC_EVENT_REQ_ITEM_PLUS2,	108 - 7,	319 - 285,	16,	16, pSkin);	
	pStatic->SetImage(OPTION_IMAGE_FILE3, 239, 423);
	pStatic->Show();
	pStatic = NULL;

	//WIID_NPC_EVENT_REQ_ITEM_PLUS3
	pStatic = new SPWindowStatic(WIID_NPC_EVENT_REQ_ITEM_PLUS3,	163 - 7,	319 - 285,	16,	16, pSkin);	
	pStatic->SetImage(OPTION_IMAGE_FILE3, 239, 423);
	pStatic->Show();
	pStatic = NULL;
	
	pSkin = NULL;

	//WIID_NPC_EVENT_TIP_SKIN
	pSkin = new SPWindowStatic(WIID_NPC_EVENT_TIP_SKIN,	7,	392,	218,	15, this);	
	pSkin->SetWindowType( WND_TYPE_LEFTBASE );
	pSkin->SetImageNormalHead(OPTION_IMAGE_FILE7, 501, 393, 503, 408);
	pSkin->SetImageNormalBody(OPTION_IMAGE_FILE7, 505, 393, 507, 408);
	pSkin->SetImageNormalTail(OPTION_IMAGE_FILE7, 509, 393, 511, 408);
	pSkin->Show();

	//WIID_NPC_EVENT_TIP
	m_pTip = new SPWindowStatic(WIID_NPC_EVENT_TIP,	8 - 7,	393-392,	216,	12, pSkin);	
	m_pTip->SetFormat(DT_CENTER | DT_VCENTER);
	m_pTip->SetWindowText("Tip Message Area..");
	m_pTip->Show();

	//WIID_NPC_EVENT_RECEIVE
	m_pRecevie = new SPWindowButton(WIID_NPC_EVENT_RECEIVE , 82, 415, 68, 17, this);
	m_pRecevie->SetImage		(OPTION_IMAGE_FILE7, 1,  342);		
	m_pRecevie->SetImageHit	(OPTION_IMAGE_FILE7, 1,  360);
	m_pRecevie->SetImagePush	(OPTION_IMAGE_FILE7, 1,  378);
	m_pRecevie->SetImageDisable(OPTION_IMAGE_FILE7, 1,  396);
	m_pRecevie->Show();
}


void SPWindowNpcEvent::Clean()
{	
	ResetSubControl();
	SAFE_RELEASE(m_pDisable);
	SAFE_RELEASE(m_pTextureBase);
	SPWindow::Clean();
}


void SPWindowNpcEvent::Process(float fTime)
{
	SPWindow::Process(fTime);
}


void SPWindowNpcEvent::Render(float fTime)
{
	if(!m_bShow) return;
	
	if(m_pTextureBase) {
		for(int i = 0; i < WINDOW_BASE_MAX; i++) {
			m_pTextureBase->RenderTexture(&m_rtDestBase[i], &m_rtSrcBase[i]);
		}
	}	

	SPWindow::Render(fTime);

	g_pVideo->Flush();
	
	int i = 0;
	for(i = 0; i < EVENT_PAGE; i++) {
		if(m_pDisable) {
			RECT rtRect;
			m_pItemIcon[i]->GetWindowRect(rtRect);

			if(m_bRewordAble[i] == false) {				//아이콘 위에 살짝 비활성화				
				m_pDisable->RenderTexture(&rtRect, &m_rtSrcDisable);
			}

			if(m_iRewordMark[i] > ICON_MARK_NULL) {				
				m_pDisable->RenderTexture(&rtRect, &m_rtMarkSrc[ (m_iRewordMark[i] - 1) ]);
			}
		}

		//Stack Count Local 표현
		if(m_iRewordStack[i] > 9) {
			RECT rtTenNumTarget, rtNumTarget, rtTenNumSrc, rtNumSrc;
			int iTenStack = m_iRewordStack[i] / 10;
			int iOneStack = m_iRewordStack[i] % 10;

			SetRect(&rtTenNumTarget, m_rtRewordStack[i].left, m_rtRewordStack[i].bottom - 10, m_rtRewordStack[i].left + 9, m_rtRewordStack[i].bottom);
			SetRect(&rtNumTarget, rtTenNumTarget.right - 2, m_rtRewordStack[i].bottom - 10, rtTenNumTarget.right + 9 - 2, m_rtRewordStack[i].bottom);

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
		else if(m_iRewordStack[i] > 1) {
			int iOneStack = m_iRewordStack[i] % 10;
			RECT rtNumTarget, rtNumSrc;
			SetRect(&rtNumTarget, m_rtRewordStack[i].left, m_rtRewordStack[i].bottom - 10, m_rtRewordStack[i].left + 9, m_rtRewordStack[i].bottom);

			rtNumSrc.left = iOneStack * 9;
			rtNumSrc.right = rtNumSrc.left + 9;
			rtNumSrc.top = 0;
			rtNumSrc.bottom = 10;

			g_pItemCluster->GetNumTexture()->RenderTexture( &rtNumTarget , &rtNumSrc);
		}
	}
	
	g_pVideo->Flush();
	
	for(i = 0; i < EVENT_REQ_ITEM; i++) {
		if(m_pDisable) {
			RECT rtRect;
			m_pReqItemIcon[i]->GetWindowRect(rtRect);

			if(m_bReqAble[i] == false) {				//아이콘 위에 살짝 비활성화				
				m_pDisable->RenderTexture(&rtRect, &m_rtSrcDisable);
			}

			if(m_iReqMark[i] > ICON_MARK_NULL) {				
				m_pDisable->RenderTexture(&rtRect, &m_rtMarkSrc[ (m_iReqMark[i] - 1) ]);
			}
		}

		//Stack Count Local 표현
		if(m_iReqStack[i] > 9) {
			RECT rtTenNumTarget, rtNumTarget, rtTenNumSrc, rtNumSrc;
			int iTenStack = m_iReqStack[i] / 10;
			int iOneStack = m_iReqStack[i] % 10;

			SetRect(&rtTenNumTarget, m_rtReqStack[i].left, m_rtReqStack[i].bottom - 10, m_rtReqStack[i].left + 9, m_rtReqStack[i].bottom);
			SetRect(&rtNumTarget, rtTenNumTarget.right - 2, m_rtReqStack[i].bottom - 10, rtTenNumTarget.right + 9 - 2, m_rtReqStack[i].bottom);

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
		else if(m_iReqStack[i] > 1) {
			int iOneStack = m_iReqStack[i] % 10;
			RECT rtNumTarget, rtNumSrc;
			SetRect(&rtNumTarget, m_rtReqStack[i].left, m_rtReqStack[i].bottom - 10, m_rtReqStack[i].left + 9, m_rtReqStack[i].bottom);

			rtNumSrc.left = iOneStack * 9;
			rtNumSrc.right = rtNumSrc.left + 9;
			rtNumSrc.top = 0;
			rtNumSrc.bottom = 10;

			g_pItemCluster->GetNumTexture()->RenderTexture( &rtNumTarget , &rtNumSrc);
		}
	}
}


void SPWindowNpcEvent::Show(bool bWithChild /*= true*/)
{
	SPNpcEvent* pNpcEvent = g_pEventManager->GetEventHandler()->GetNpcEvent();
	if(pNpcEvent == NULL)
		return;

	SPWindow::Show(bWithChild);

	ResetSubControl();
	
	m_pTitle->SetWindowText(pNpcEvent->m_strName.c_str());

	//m_iMaxPage		= (pNpcEvent->GetSize() / EVENT_PAGE) + 1;
	int iListSize = pNpcEvent->GetSize();
	if(iListSize % EVENT_PAGE) {
		m_iMaxPage		= (iListSize / EVENT_PAGE) + 1;
	}
	else {
		m_iMaxPage		= iListSize / EVENT_PAGE;
	} 
	m_iCurPage		= 1;
	m_iSelect		= 0;
	m_iSelectIndex	= 0;
	
	m_iTotalReqEly	= 0;
	m_iTotalReqPvpPoint = 0;

	UpdateRewordList();
	
	int iStringID = 0;
	if(pNpcEvent->m_iRewordType == NPC_EVENT_REWARD_TYPE_SELECT) {
		m_pSelect[m_iSelect]->Show();
		iStringID = 3006003;
	}
	else if(pNpcEvent->m_iRewordType == NPC_EVENT_REWARD_TYPE_RENDOM) {
		iStringID = 3006002;
	}
	else if(pNpcEvent->m_iRewordType == NPC_EVENT_REWARD_TYPE_FIX) {
		iStringID = 3006004;
	}

	m_pTip->SetWindowText(NULL);
	if(g_pResourceManager->GetGlobalString(iStringID) != NULL) {
		std::string strMsg = g_pResourceManager->GetGlobalString(iStringID);
		//g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str());
		m_pTip->SetWindowText(strMsg.c_str());
	}
	
	UpdateRequireList();
	
}


void SPWindowNpcEvent::Hide(bool bSendServer /*= true*/)
{
	SPWindow::Hide(bSendServer);
}


void SPWindowNpcEvent::ResetSubControl()
{
	for(int i = 0; i < EVENT_PAGE; i++) {
		ClearRewordList(i);
	}

	m_pPage->SetWindowText(NULL);
	m_pReqEly->SetWindowText(NULL);
	
	ResetRequrieList();
}


void SPWindowNpcEvent::ClearRewordList(int iIndex)
{
	if(iIndex < 0 || iIndex >= EVENT_PAGE)
		return;
	
	m_pSelect[iIndex]->Hide();
	m_pName[iIndex]->SetWindowText(NULL);

	m_pItemIcon[iIndex]->Hide();
	m_pItemIcon[iIndex]->SPSendMessage(SPIM_SET_IMAGECLEAR);
	m_pItemIcon[iIndex]->Show();

	m_iRewordStack[iIndex] = 0;
	m_bRewordAble[iIndex] = true;
	m_iRewordMark[iIndex] = ICON_MARK_NULL;
}


void SPWindowNpcEvent::UpdateRewordList()
{
	SPNpcEvent* pNpcEvent = g_pEventManager->GetEventHandler()->GetNpcEvent();
	if(pNpcEvent == NULL)
		return;
	
	ZeroMemory(m_szPage, 12);
	sprintf(m_szPage, "%d / %d", m_iCurPage, m_iMaxPage);
	m_pPage->SetWindowText(m_szPage);
	if(m_iCurPage == 1) {
		m_pPrev->SetEnable(false);
	}
	else {
		m_pPrev->SetEnable(true);
	}

	if(m_iCurPage == m_iMaxPage){
		m_pNext->SetEnable(false);
	}
	else{
		m_pNext->SetEnable(true);
	}
	
	SPNpcEventInfo* pEventInfo = NULL;
	for(int i = 0; i < EVENT_PAGE; i++) {
		int iIndex = i + ((m_iCurPage - 1) * EVENT_PAGE);
		pEventInfo = pNpcEvent->GetEventInfo(iIndex);		
		
		ClearRewordList(i);

		if(pEventInfo == NULL)
			continue;

		if(pNpcEvent->m_iRewordType == NPC_EVENT_REWARD_TYPE_SELECT) {
			if(m_iSelect == i) {
				m_pSelect[i]->Show();
			}	
		}
		m_pSelect[i]->Hide();

		if(pEventInfo->m_RewordItem.m_iItemID) {
			SPItemAttr* pItemAttr = NULL;
			if(g_pItemCluster->GetItemInfo(pEventInfo->m_RewordItem.m_iItemID, pItemAttr)) {
				if(pItemAttr) {
					if(pItemAttr->m_bNameColor) {
						m_pName[i]->SetFontColor(pItemAttr->GetNameColor());
					}
					else {
						m_pName[i]->SetFontColor(g_pCheckManager->GetItemColor(pEventInfo->m_RewordItem.m_iRear));
					}
					m_pName[i]->SetWindowText(pItemAttr->m_strName.c_str());

					m_iRewordStack[i] = pEventInfo->m_RewordItem.m_iStack;
					m_iRewordMark[i] = g_pItemCluster->GetItemIconMark(pEventInfo->m_RewordItem.m_iItemID);
					m_bRewordAble[i] = g_pCheckManager->CheckItemStatus(pEventInfo->m_RewordItem.m_iItemID);
				}
			}
		}
		else {
			m_pName[i]->SetWindowText(pEventInfo->m_strTitleName.c_str());			
		}

		SetRewordListIcon(pEventInfo, i);
	}
}


bool SPWindowNpcEvent::SetRewordListIcon(SPNpcEventInfo* pInfo, int iListIndex)
{
	if(pInfo == NULL)
		return false;
	
	if(iListIndex < 0 || iListIndex >= EVENT_PAGE) 
		return false;
	
	if(pInfo->m_RewordItem.m_iItemID) {
		POINT ptSrcPos;
		SPTexture* pTexture = g_pItemCluster->GetItemTexture(pInfo->m_RewordItem.m_iItemID, ptSrcPos);

		if(m_pItemIcon[iListIndex]) {
			m_pItemIcon[iListIndex]->Hide();
			m_pItemIcon[iListIndex]->SetImage(pTexture, ptSrcPos.x, ptSrcPos.y);
			m_pItemIcon[iListIndex]->Show();
		}
		return true;
	}
	else {
		int iResID = pInfo->m_iIcon;
		int iResIndex = pInfo->m_iIconIndex;

		if( iResID <= 0 || iResIndex <= 0 )
			return false;

		const char*	szFileName = g_pResourceManager->GetGlobalFileName( iResID );

		if( szFileName == NULL )
			return false;

		std::string strResourceFile = szFileName;

		if( strResourceFile.empty() )
			return false;

		SPTexture*	pThemeIcon	=	NULL;
		std::string strFullRes; //	=	RES_WORK_RESOURCE_PATH;
		strFullRes	=	strResourceFile;

		g_pVideo->GetTextureMgr()->LoadTexture( strFullRes.c_str() , &pThemeIcon );

		if( pThemeIcon == NULL )
			return false;

		//	시작은 0이 아닌 1부터 시작하기에 1을 빼준다!!!
		iResIndex--;

		int iIconSizeX = 32;
		int	iIconSizeY = 32;

		int iIconWidth	= pThemeIcon->GetLenX() / iIconSizeX;
		int iIconYIndex = iResIndex / iIconWidth;
		int iIconXIndex = iResIndex - iIconYIndex * iIconWidth;

		if(m_pItemIcon[iListIndex]) {
			m_pItemIcon[iListIndex]->Hide();
			m_pItemIcon[iListIndex]->SetImage(strFullRes.c_str() , iIconXIndex * iIconSizeX , iIconYIndex * iIconSizeY);
			m_pItemIcon[iListIndex]->Show();
		}

		return true;
	}

	return true;
}


void SPWindowNpcEvent::ResetRequrieList()
{
	m_pReqEly->SetWindowText(NULL);
	
	m_iReqItemCount = 0;
	for(int i = 0; i < EVENT_REQ_ITEM; i++) {
		ClearRequireList(i);
	}
}


void SPWindowNpcEvent::ClearRequireList(int iIndex)
{
	if(iIndex < 0 || iIndex >= EVENT_REQ_ITEM)
		return;

	m_pReqItemIcon[iIndex]->Hide();
	m_pReqItemIcon[iIndex]->SPSendMessage(SPIM_SET_IMAGECLEAR);
	m_pReqItemIcon[iIndex]->Show();

	m_iReqStack[iIndex] = 0;
	m_bReqAble[iIndex] = true;
	m_iReqMark[iIndex] = ICON_MARK_NULL;

	ZeroMemory(&m_ReqItem[iIndex], sizeof(TRASH_ITEM));
	m_ReqItem[iIndex].iContainerSlot.ContainerType = CONTAINER_TYPE_INVALID;
	m_iReqItemNo[iIndex] = 0;
	m_ReqToolTipItem[iIndex].Clear();
}


void SPWindowNpcEvent::UpdateRequireList()
{
	SPNpcEvent* pNpcEvent = g_pEventManager->GetEventHandler()->GetNpcEvent();
	if(pNpcEvent == NULL)
		return;
	
	ResetRequrieList();

	if(pNpcEvent->m_iRewordType < NPC_EVENT_REWARD_TYPE_SELECT || pNpcEvent->m_iRewordType > NPC_EVENT_REWARD_TYPE_FIX)
		return;

	int iReqIndex = 0;
	m_iTotalReqEly = 0;
	m_iTotalReqPvpPoint = 0;

	m_iTotalReqEly = pNpcEvent->m_iRequireEly;
	m_iTotalReqPvpPoint = pNpcEvent->m_iRequirePvpPoint;
	
	if(pNpcEvent->m_RequireItem[0].m_iItemID) {
		SetRequestListIcon(&pNpcEvent->m_RequireItem[0], iReqIndex);		
		iReqIndex++;
	}
	
	if(pNpcEvent->m_RequireItem[1].m_iItemID) {
		SetRequestListIcon(&pNpcEvent->m_RequireItem[1], iReqIndex);
		iReqIndex++;
	}

	if(pNpcEvent->m_iRewordType == NPC_EVENT_REWARD_TYPE_SELECT) {
		m_iSelectIndex = m_iSelect + ((m_iCurPage - 1) * EVENT_PAGE);
		SPNpcEventInfo* pInfo = pNpcEvent->GetEventInfo(m_iSelectIndex);
		if(pInfo == NULL)
			return;

		m_iTotalReqEly += pInfo->m_iRequireEly;
		m_iTotalReqPvpPoint += pInfo->m_iRequirePvpPoint;

		if(pInfo->m_RequireItem[0].m_iItemID) {
			SetRequestListIcon(&pInfo->m_RequireItem[0], iReqIndex);
			iReqIndex++;
		}

		if(pInfo->m_RequireItem[1].m_iItemID) {
			SetRequestListIcon(&pInfo->m_RequireItem[1], iReqIndex);
			iReqIndex++;
		}
	}

	m_pReqEly->SetWindowText(ConvertMoneyToString(m_iTotalReqEly));
	m_pReqPvpPoint->SetWindowText(ConvertMoneyToString(m_iTotalReqPvpPoint));

	////
	//if(CheckRequire() == false) {
	//	m_pRecevie->SetEnable(false);
	//}
	//else {
	//	m_pRecevie->SetEnable(true);
	//}
}



bool SPWindowNpcEvent::SetRequestListIcon(SPQuestItem* pItemInfo, int iListIndex)
{
	if(pItemInfo == NULL) 
		return false;

	if(pItemInfo->m_iItemID == 0)
		return false;

	if(iListIndex < 0 || iListIndex >= EVENT_REQ_ITEM)
		return false;

	POINT ptSrcPos;
	SPTexture* pTexture = g_pItemCluster->GetItemTexture(pItemInfo->m_iItemID, ptSrcPos);

	if(m_pReqItemIcon[iListIndex]) {
		m_pReqItemIcon[iListIndex]->Hide();
		m_pReqItemIcon[iListIndex]->SetImage(pTexture, ptSrcPos.x, ptSrcPos.y);
		m_pReqItemIcon[iListIndex]->Show();
	}

	m_iReqStack[iListIndex] = pItemInfo->m_iStack;
	m_iReqMark[iListIndex] = g_pItemCluster->GetItemIconMark(pItemInfo->m_iItemID);
	//m_bReqAble[iListIndex] = g_pCheckManager->CheckItemStatus(pItemInfo->m_iItemID);
	
	m_ReqToolTipItem[iListIndex] = static_cast<SPQuestItem>(*pItemInfo);
	
	return true;
}


bool SPWindowNpcEvent::CheckRequire()
{
	SPNpcEvent* pNpcEvent = g_pEventManager->GetEventHandler()->GetNpcEvent();
	if(pNpcEvent == NULL)
		return false;

	if(pNpcEvent->m_iRewordType < NPC_EVENT_REWARD_TYPE_SELECT || pNpcEvent->m_iRewordType > NPC_EVENT_REWARD_TYPE_FIX)
		return false;
	
	int i = 0;
	bool bReturn = true;
	for(i = 0; i < MAX_NPC_EVENT_REQUIRE; i++) {
		pNpcEvent->m_Require[i].m_bOK = g_pCheckManager->CheckCondition(pNpcEvent->m_Require[i].m_iRequireStat, 
			pNpcEvent->m_Require[i].m_iRequireStatValue, pNpcEvent->m_Require[i].m_iValue1, pNpcEvent->m_Require[i].m_iValue2);		
		bReturn &= pNpcEvent->m_Require[i].m_bOK;		
	}

	if(pNpcEvent->m_iRewordType == NPC_EVENT_REWARD_TYPE_SELECT) {
		m_iSelectIndex = m_iSelect + ((m_iCurPage - 1) * EVENT_PAGE);
		SPNpcEventInfo* pInfo = pNpcEvent->GetEventInfo(m_iSelectIndex);
		if(pInfo == NULL) 
			return false;

		for(i = 0; i < MAX_NPC_EVENT_INFO_REQUIRE; i++) {
			pInfo->m_Require[i].m_bOK = g_pCheckManager->CheckCondition(pInfo->m_Require[i].m_iRequireStat, 
				pInfo->m_Require[i].m_iRequireStatValue, pInfo->m_Require[i].m_iValue1, pInfo->m_Require[i].m_iValue2);

			bReturn &= pInfo->m_Require[i].m_bOK;
		}

		if(bReturn == false && !pInfo->m_strErr.empty())
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)pInfo->m_strErr.c_str());
	}
	else if(pNpcEvent->m_iRewordType == NPC_EVENT_REWARD_TYPE_RENDOM || pNpcEvent->m_iRewordType == NPC_EVENT_REWARD_TYPE_FIX){
		if(bReturn == false && !pNpcEvent->m_strErr.empty())
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)pNpcEvent->m_strErr.c_str());
	}

	return bReturn;
}


bool SPWindowNpcEvent::CheckRequireItem()
{
	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return false;
	
	SPPlayerInvenArchive* pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	if(pInven == NULL)
		return false;
	
	SPNpcEvent* pNpcEvent = g_pEventManager->GetEventHandler()->GetNpcEvent();
	if(pNpcEvent == NULL)
		return false;

	if(pNpcEvent->m_iRewordType < NPC_EVENT_REWARD_TYPE_SELECT || pNpcEvent->m_iRewordType > NPC_EVENT_REWARD_TYPE_FIX)
		return false;	
	
	m_iReqItemCount = 0;
	bool bReturn = true;
	SPItemAttr* pItemAttr = NULL;
	if(pNpcEvent->m_RequireItem[0].m_iItemID) {		
		pItemAttr = NULL;
		g_pItemCluster->GetItemInfo(pNpcEvent->m_RequireItem[0].m_iItemID, pItemAttr);
		CONTAINER_TYPE iContainer = (CONTAINER_TYPE)(pItemAttr->m_eType - 1);		
		
		if(pInven->IsBiggerStack(iContainer, pNpcEvent->m_RequireItem[0].m_iItemID, pNpcEvent->m_RequireItem[0].m_iStack)) {
			SPItem* pItem = pInven->GetSPItem(iContainer, pNpcEvent->m_RequireItem[0].m_iItemID, pNpcEvent->m_RequireItem[0].m_iStack);

			//아이템 확인
			if(pItem == NULL || pItem->IsEmptyItem()) {
				if(!pNpcEvent->m_strErr.empty())
					g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)pNpcEvent->m_strErr.c_str());
				bReturn &= false;
			}
			else {
				m_ReqItem[m_iReqItemCount].iContainerSlot.ContainerType = iContainer;
				m_ReqItem[m_iReqItemCount].iContainerSlot.SlotIndex		= pItem->GetSlotIndex() + 1;
				m_ReqItem[m_iReqItemCount].iCount						= pNpcEvent->m_RequireItem[0].m_iStack;
				m_iReqItemNo[m_iReqItemCount] = *(ITEMNO*)pItem->GetItemStatus()->SPSendMessage( MV_ITEM_GET_ITEMNO );
				m_iReqItemCount++;
			}
		}
		else {
			if(!pNpcEvent->m_strErr.empty())
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)pNpcEvent->m_strErr.c_str());
			bReturn &= false;
			return bReturn;
		}
	}

	if(pNpcEvent->m_RequireItem[1].m_iItemID) {
		pItemAttr = NULL;
		g_pItemCluster->GetItemInfo(pNpcEvent->m_RequireItem[1].m_iItemID, pItemAttr);
		CONTAINER_TYPE iContainer = (CONTAINER_TYPE)(pItemAttr->m_eType - 1);		
		
		if(pInven->IsBiggerStack(iContainer, pNpcEvent->m_RequireItem[1].m_iItemID, pNpcEvent->m_RequireItem[1].m_iStack)) {
			SPItem* pItem = pInven->GetSPItem(iContainer, pNpcEvent->m_RequireItem[1].m_iItemID, pNpcEvent->m_RequireItem[1].m_iStack);

			//아이템 확인
			if(pItem == NULL || pItem->IsEmptyItem()) {
				if(!pNpcEvent->m_strErr.empty())
					g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)pNpcEvent->m_strErr.c_str());
				bReturn &= false;
			}
			else {
				m_ReqItem[m_iReqItemCount].iContainerSlot.ContainerType = iContainer;
				m_ReqItem[m_iReqItemCount].iContainerSlot.SlotIndex		= pItem->GetSlotIndex() + 1;
				m_ReqItem[m_iReqItemCount].iCount						= pNpcEvent->m_RequireItem[1].m_iStack;
				m_iReqItemNo[m_iReqItemCount] = *(ITEMNO*)pItem->GetItemStatus()->SPSendMessage( MV_ITEM_GET_ITEMNO );
				m_iReqItemCount++;
			}
		}
		else {
			if(!pNpcEvent->m_strErr.empty())
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)pNpcEvent->m_strErr.c_str());
			bReturn &= false;
			return bReturn;
		}
	}

	if(pNpcEvent->m_iRewordType == NPC_EVENT_REWARD_TYPE_SELECT) {
		m_iSelectIndex = m_iSelect + ((m_iCurPage - 1) * EVENT_PAGE);
		SPNpcEventInfo* pInfo = pNpcEvent->GetEventInfo(m_iSelectIndex);
		if(pInfo == NULL)
			return false;

		if(pInfo->m_RequireItem[0].m_iItemID) {
			pItemAttr = NULL;
			g_pItemCluster->GetItemInfo(pInfo->m_RequireItem[0].m_iItemID, pItemAttr);
			CONTAINER_TYPE iContainer = (CONTAINER_TYPE)(pItemAttr->m_eType - 1);		

			if(pInven->IsBiggerStack(iContainer, pInfo->m_RequireItem[0].m_iItemID, pInfo->m_RequireItem[0].m_iStack)) {
				SPItem* pItem = pInven->GetSPItem(iContainer, pInfo->m_RequireItem[0].m_iItemID, pInfo->m_RequireItem[0].m_iStack);

				//아이템 확인
				if(pItem == NULL || pItem->IsEmptyItem()) {
					if(!pInfo->m_strErr.empty())
						g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)pInfo->m_strErr.c_str());
					bReturn &= false;
				}
				else {
					m_ReqItem[m_iReqItemCount].iContainerSlot.ContainerType = iContainer;
					m_ReqItem[m_iReqItemCount].iContainerSlot.SlotIndex		= pItem->GetSlotIndex() + 1;
					m_ReqItem[m_iReqItemCount].iCount						= pInfo->m_RequireItem[0].m_iStack;
					m_iReqItemNo[m_iReqItemCount] = *(ITEMNO*)pItem->GetItemStatus()->SPSendMessage( MV_ITEM_GET_ITEMNO );
					m_iReqItemCount++;
				}
			}
			else {
				if(!pInfo->m_strErr.empty())
					g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)pInfo->m_strErr.c_str());
				bReturn &= false;
				return bReturn;
			}
		}

		if(pInfo->m_RequireItem[1].m_iItemID) {
			pItemAttr = NULL;
			g_pItemCluster->GetItemInfo(pInfo->m_RequireItem[1].m_iItemID, pItemAttr);
			CONTAINER_TYPE iContainer = (CONTAINER_TYPE)(pItemAttr->m_eType - 1);		
			
			if(pInven->IsBiggerStack(iContainer, pInfo->m_RequireItem[1].m_iItemID, pInfo->m_RequireItem[1].m_iStack)) {
				SPItem* pItem = pInven->GetSPItem(iContainer, pInfo->m_RequireItem[1].m_iItemID, pInfo->m_RequireItem[1].m_iStack);

				//아이템 확인
				if(pItem == NULL || pItem->IsEmptyItem()) {
					if(!pInfo->m_strErr.empty())
						g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)pInfo->m_strErr.c_str());
					bReturn &= false;
				}
				else {
					m_ReqItem[m_iReqItemCount].iContainerSlot.ContainerType = iContainer;
					m_ReqItem[m_iReqItemCount].iContainerSlot.SlotIndex		= pItem->GetSlotIndex() + 1;
					m_ReqItem[m_iReqItemCount].iCount						= pInfo->m_RequireItem[1].m_iStack;
					m_iReqItemNo[m_iReqItemCount] = *(ITEMNO*)pItem->GetItemStatus()->SPSendMessage( MV_ITEM_GET_ITEMNO );
					m_iReqItemCount++;
				}
			}
			else {
				if(!pInfo->m_strErr.empty())
					g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)pInfo->m_strErr.c_str());
				bReturn &= false;
				return bReturn;
			}
		}

		UINT64 iUserEly = g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValueEX(STATUS_EX_ELY);
		if(iUserEly < m_iTotalReqEly) {
			if(!pInfo->m_strErr.empty())
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)pInfo->m_strErr.c_str());
			bReturn &= false;
			return bReturn;
		}

		//
		UINT32 iUserPvpPoint = g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValue(STATUS_PVP_POINT);
		if(iUserPvpPoint < m_iTotalReqPvpPoint) {
			if(!pInfo->m_strErr.empty())
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)pInfo->m_strErr.c_str());
			bReturn &= false;
			return bReturn;
		}
	}
	else {
		UINT64 iUserEly = g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValueEX(STATUS_EX_ELY);
		if(iUserEly < m_iTotalReqEly) {
			if(!pNpcEvent->m_strErr.empty())
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)pNpcEvent->m_strErr.c_str());
			bReturn &= false;
			return bReturn;
		}

		//
		UINT32 iUserPvpPoint = g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValue(STATUS_PVP_POINT);
		if(iUserPvpPoint < m_iTotalReqPvpPoint) {
			if(!pNpcEvent->m_strErr.empty())
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)pNpcEvent->m_strErr.c_str());
			bReturn &= false;
			return bReturn;
		}
	}	

	return bReturn;
}


bool SPWindowNpcEvent::CheckRewordInvenSlot()
{
	int iRequireSlot[FIGURE_EQUIP] = {0, 0, 0, 0, 0, 0};		//Inventroy별 size 6
	
	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return false;

	SPPlayerInvenArchive* pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	if(pInven == NULL)
		return false;

	SPNpcEvent* pNpcEvent = g_pEventManager->GetEventHandler()->GetNpcEvent();
	if(pNpcEvent == NULL)
		return false;

	if(pNpcEvent->m_iRewordType < NPC_EVENT_REWARD_TYPE_SELECT || pNpcEvent->m_iRewordType > NPC_EVENT_REWARD_TYPE_FIX)
		return false;

	if(pNpcEvent->m_iRewordType == NPC_EVENT_REWARD_TYPE_SELECT) {
		m_iSelectIndex = m_iSelect + ((m_iCurPage - 1) * EVENT_PAGE);
		SPNpcEventInfo* pInfo = pNpcEvent->GetEventInfo(m_iSelectIndex);
		if(pInfo == NULL)
			return false;		
		
		if(pInfo->m_RewordItem.m_iItemID) {
			if(g_pCheckManager->CheckEnableSlot(pInfo->m_RewordItem.m_iItemID) == false) {
				CONTAINER_TYPE iContainer = CONTAINER_TYPE_INVALID;
				SPItemAttr* pItem = NULL;
				g_pItemCluster->GetItemInfo(pInfo->m_RewordItem.m_iItemID, pItem);
				if(pItem == NULL)
					return false;

				iContainer = (CONTAINER_TYPE)(pItem->m_eType - 1);
				SetInvenFullMsg(iContainer);
				return false;
			}
		}		
		return true;
	}
	else if(pNpcEvent->m_iRewordType == NPC_EVENT_REWARD_TYPE_RENDOM) {
		int i = 0;		
		
		for(i = 0; i < FIGURE_EQUIP; i++) {
			if(false == g_pCheckManager->CheckRequireSlot(i, 1)) {
				//i번째 인벤토리 공간 부족
				SetInvenFullMsg(i);
				return false;
			}
		}
		return true;
	}
	else if(pNpcEvent->m_iRewordType == NPC_EVENT_REWARD_TYPE_FIX) {
		int i = 0;		
		
		//각 컨테이너별로 필요한 슬롯수를 기푳E
		for(i = 0; i < pNpcEvent->GetSize(); i++) {
			SPNpcEventInfo* pInfo = pNpcEvent->GetEventInfo(i);
			if(pInfo == NULL || pInfo->m_RewordItem.m_iItemID == 0)
				continue;
			
			CONTAINER_TYPE iContainer = CONTAINER_TYPE_INVALID;
			SPItemAttr* pItem = NULL;
			g_pItemCluster->GetItemInfo(pInfo->m_RewordItem.m_iItemID, pItem);
			if(pItem == NULL)
				continue;
			iContainer = (CONTAINER_TYPE)(pItem->m_eType - 1);
			iRequireSlot[iContainer]++;
		}

		//컨테이너 별로 비엉復는 슬롯수를 계퍊E磯?		
		for(i = 0; i < FIGURE_EQUIP; i++) {
			if(iRequireSlot[i] == 0) 
				continue;

			if(false == g_pCheckManager->CheckRequireSlot(i, iRequireSlot[i])) {
				//i번째 인벤토리 공간 부족
				SetInvenFullMsg(i);
				return false;
			}
		}

		//스텍을 염두해 계퍊E磯?
		for(i = 0; i < pNpcEvent->GetSize(); i++) {
			SPNpcEventInfo* pInfo = pNpcEvent->GetEventInfo(i);
			if(pInfo == NULL || pInfo->m_RewordItem.m_iItemID == 0)
				continue;
			
			if(pInfo->m_RewordItem.m_iItemID > 0) {
				if(false == g_pCheckManager->CheckEnableSlot(pInfo->m_RewordItem.m_iItemID, pInfo->m_RewordItem.m_iStack)) {
					//i번째 인벤토리 공간 부족
					CONTAINER_TYPE iContainer = CONTAINER_TYPE_INVALID;
					SPItemAttr* pItem = NULL;
					g_pItemCluster->GetItemInfo(pInfo->m_RewordItem.m_iItemID, pItem);
					if(pItem == NULL)
						return false;

					iContainer = (CONTAINER_TYPE)(pItem->m_eType - 1);
					SetInvenFullMsg(iContainer);					
					return false;
				}
			}
		}
		return true;
	}	
	
	return true;
}


void SPWindowNpcEvent::SetInvenFullMsg(int i)
{
	CONTAINER_TYPE iContainer = (CONTAINER_TYPE)i;	
	int iStringID = 0;
	switch(iContainer) {
		case INVENTORY_EQUIP:
			iStringID = 13001011;
			break;
		case INVENTORY_CONSUME:
			iStringID = 13001012;
			break;
		case INVENTORY_ETC:
			iStringID = 13001013;
			break;
		case INVENTORY_EVENT:
			iStringID = 13001014;
			break;
		case INVENTORY_PET:
			iStringID = 13001015;
			break;
		case INVENTORY_HOUSING:
			iStringID = 13001016;
			break;
		default:
			return;
			break;
	}
	
	if(g_pResourceManager->GetGlobalString(iStringID) != NULL) {
		std::string strMsg = g_pResourceManager->GetGlobalString(iStringID);
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str());
	}

	return;
}



SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowNpcEvent )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_NPC_EVENT_EXIT,				OnExit)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_NPC_EVENT_PAGE_PREV,			OnPrev)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_NPC_EVENT_PAGE_NEXT,			OnNext)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_NPC_EVENT_RECEIVE,				OnReceive)
SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP,		WIID_NPC_EVENT_LIST_BUTTON,		WIID_NPC_EVENT_LIST_BUTTON + EVENT_PAGE,	OnItemClick)
SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_MOVE,	WIID_NPC_EVENT_LIST_BUTTON,		WIID_NPC_EVENT_LIST_BUTTON + EVENT_PAGE,	OnRewordToolTipMove)
SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_ENTER,	WIID_NPC_EVENT_LIST_BUTTON,		WIID_NPC_EVENT_LIST_BUTTON + EVENT_PAGE,	OnRewordToolTipEnter)
SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_OUT,		WIID_NPC_EVENT_LIST_BUTTON,		WIID_NPC_EVENT_LIST_BUTTON + EVENT_PAGE,	OnRewordToolTipOut)
SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_MOVE,	WIID_NPC_EVENT_REQ_ITEM_ICONBUTTON,		WIID_NPC_EVENT_REQ_ITEM_ICONBUTTON + EVENT_REQ_ITEM,	OnReqToolTipMove)
SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_ENTER,	WIID_NPC_EVENT_REQ_ITEM_ICONBUTTON,		WIID_NPC_EVENT_REQ_ITEM_ICONBUTTON + EVENT_REQ_ITEM,	OnReqToolTipEnter)
SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_OUT,		WIID_NPC_EVENT_REQ_ITEM_ICONBUTTON,		WIID_NPC_EVENT_REQ_ITEM_ICONBUTTON + EVENT_REQ_ITEM,	OnReqToolTipOut)
SPIMESSAGE_OBJ_MAP_END	( SPWindow	)


int SPWindowNpcEvent::OnExit(WPARAM wParam, LPARAM lParam)
{
	g_pEventManager->SetCutInState(CUT_IN_STATE_RELEASE);
	return 1;
}


int SPWindowNpcEvent::OnPrev(WPARAM wParam, LPARAM lParam)
{
	if( m_iCurPage > 1  && m_iCurPage <= m_iMaxPage ) {
		m_iCurPage--;

		UpdateRewordList();
	}
	
	return 1;
}


int SPWindowNpcEvent::OnNext(WPARAM wParam, LPARAM lParam)
{
	if( m_iCurPage > 0 && m_iCurPage < m_iMaxPage ) {
		m_iCurPage++;

		UpdateRewordList();
	}

	return 1;
}


int SPWindowNpcEvent::OnReceive(WPARAM wParam, LPARAM lParam)
{
	SPPlayerInvenArchive* pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	if(pInven == NULL)
		return 1;

	SPNpcEvent* pNpcEvent = g_pEventManager->GetEventHandler()->GetNpcEvent();
	if(pNpcEvent == NULL)
		return 1;	

	UpdateRequireList();	
	
	if(CheckRequire() == false)
		return 1;

	if(CheckRequireItem() == false)
		return 1;

	if(CheckRewordInvenSlot() == false)
		return 1;

	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return 1;	

	if(g_pCheckManager->IsDBBlock())
		return 1;
	
	g_pCheckManager->SetDBBlock(true);
	//Packet
	// NPC 보상 시스템 요청
	//NPC_CS_REWARD,
		// int            iNpcID;			// 엔피시
		// int			  iRewardGroupID;		  
		// UINT8		  iRewardIndex;	   // 랜덤 및 모두 보상일경우는의미없음
		// int			  iNumOfUsingItem;
		// TRASH_ITEM stReqItem;	// 소모아이템
		// ITEMNO	  iReqItemNo;		// 소모아이템 No
		// int			  iUsingEly;		// 소모돈	

	CPacket Packet;
	Packet.Add((UINT32)NPC_CS_REWARD);	
	Packet.Add((UINT32)pNpcEvent->m_iNpcID);
	Packet.Add((UINT32)pNpcEvent->m_iItemID);
	Packet.AddUINT8((UINT8)m_iSelectIndex);
	Packet.AddUINT8(m_iReqItemCount);
	for(int i = 0; i < m_iReqItemCount; i++) {
		if(m_ReqItem[i].iContainerSlot.ContainerType < INVENTORY_EQUIP || m_ReqItem[i].iContainerSlot.ContainerType > INVENTORY_HOUSING)
			return 1;

		if(m_iReqItemNo[i] == 0)
			return 1;
		
		Packet.AddData(&m_ReqItem[i], sizeof(TRASH_ITEM));
		Packet.AddUINT64(m_iReqItemNo[i]);
	}
	Packet.Add((UINT32)m_iTotalReqEly);	
	Packet.Add((UINT32)m_iTotalReqPvpPoint);

	//이부분 리턴값 문제 있을수 있음...
	g_pNetworkManager->SPPerformMessage(NPC_CS_REWARD, 0, (LPARAM)&Packet);
	
	return 1;
}


int SPWindowNpcEvent::OnItemClick(unsigned int uiID, WPARAM wParam, LPARAM lParam)
{
	SPNpcEvent* pNpcEvent = g_pEventManager->GetEventHandler()->GetNpcEvent();
	if(pNpcEvent == NULL) 
		return 1;

	if(pNpcEvent->m_iRewordType != NPC_EVENT_REWARD_TYPE_SELECT)
		return 1;
	
	m_iSelect = uiID - WIID_NPC_EVENT_LIST_BUTTON;	
	DXUTOutputDebugString("Event Select %d\n", m_iSelect);	

	int iIndex = m_iSelect + ((m_iCurPage - 1) * EVENT_PAGE);
	if(iIndex >= pNpcEvent->GetSize())
		return 1;
	
	for(int i = 0; i < EVENT_PAGE; i++) {
		if(i == m_iSelect) {
			m_pSelect[i]->Show();
		}
		else
			m_pSelect[i]->Hide();
	}	
	
	//필요아이템 Ely리스트 업데이트
	UpdateRequireList();

	return 1;
}


int SPWindowNpcEvent::OnRewordToolTipMove(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	int i = iID - WIID_NPC_EVENT_LIST_BUTTON;
	
	SPNpcEvent* pNpcEvent = g_pEventManager->GetEventHandler()->GetNpcEvent();
	if(pNpcEvent == NULL) 
		return 1;

	int iIndex = i + ((m_iCurPage - 1) * EVENT_PAGE);
	if(iIndex >= pNpcEvent->GetSize())
		return 1;

	SPNpcEventInfo* pInfo = pNpcEvent->GetEventInfo(iIndex);
	if(pInfo == NULL)
		return 1;

	if(pInfo->m_strTitleName.empty() == false) {
		SPNpcEventToolTip m_EventToolTip;
		m_EventToolTip.SetTitle(pInfo->m_strTitleName);
		m_EventToolTip.m_strDesc = pInfo->m_strDesc;

		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage(SPIM_TOOLTIP_ENTER, (WPARAM)&m_EventToolTip, (LPARAM)-12);
		pWindow->Show();
		return 1;
	}

	if(pInfo->m_RewordItem.m_iItemID == 0)
		return 1;

	if(pInfo->m_RewordItem.m_iItemID) {
		unsigned int uiUniqueID;
		int iEquipClass, iItemID, iItemColor;	
		uiUniqueID = pInfo->m_RewordItem.m_iItemID;	
		iItemColor = (pInfo->m_RewordItem.m_iItemID) % 100;
		iEquipClass = (pInfo->m_RewordItem.m_iItemID) / 100000;
		iItemID = ((pInfo->m_RewordItem.m_iItemID) % 100000 - iItemColor) / 100;	

		SPNpcEventToolTip m_EventToolTip;
		m_EventToolTip.m_pItem.SetItemID(uiUniqueID);
		
		SPItemAttr* pItemAttr;
		g_pItemCluster->GetItemInfo(uiUniqueID, pItemAttr);
		
		if( pItemAttr )
		{
			m_EventToolTip.m_pItem.SPSendMessage(MV_ITEM_SETEQUIPID, pItemAttr->m_ePosID1);
			m_EventToolTip.m_pItem.SPSendMessage(MV_ITEM_SETEQUIPCLASS, iEquipClass);
			m_EventToolTip.m_pItem.SPSendMessage(MV_ITEM_SETEQUIPITEMID, iItemID);
			m_EventToolTip.m_pItem.SPSendMessage(MV_ITEM_SETCOLOR,	iItemColor);
			m_EventToolTip.m_pItem.SPSendMessage(MV_ITEM_SET_RARELEVEL, pInfo->m_RewordItem.m_iRear);

			if(pItemAttr->m_eDuration != ITEM_DURATION_NULL) {
				m_EventToolTip.m_pItem.SetLifeTime(pItemAttr->m_iLifeTime * 3600);
			}

			m_EventToolTip.m_strDesc = pInfo->m_strDesc;

			SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
			pWindow->SPSendMessage(SPIM_TOOLTIP_ENTER, (WPARAM)&m_EventToolTip, (LPARAM)-12);
			pWindow->Show();
		}
	}		

	return 1;
}


int SPWindowNpcEvent::OnRewordToolTipEnter(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	int i = iID - WIID_NPC_EVENT_LIST_BUTTON;

	SPNpcEvent* pNpcEvent = g_pEventManager->GetEventHandler()->GetNpcEvent();
	if(pNpcEvent == NULL) 
		return 1;

	int iIndex = i + ((m_iCurPage - 1) * EVENT_PAGE);
	if(iIndex >= pNpcEvent->GetSize())
		return 1;

	SPNpcEventInfo* pInfo = pNpcEvent->GetEventInfo(iIndex);
	if(pInfo == NULL)
		return 1;

	if(pInfo->m_strTitleName.empty() == false) {
		SPNpcEventToolTip m_EventToolTip;
		m_EventToolTip.SetTitle(pInfo->m_strTitleName);
		m_EventToolTip.m_strDesc = pInfo->m_strDesc;

		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage(SPIM_TOOLTIP_ENTER, (WPARAM)&m_EventToolTip, (LPARAM)-12);
		pWindow->Show();
		return 1;
	}

	if(pInfo->m_RewordItem.m_iItemID == 0)
		return 1;

	if(pInfo->m_RewordItem.m_iItemID) {
		unsigned int uiUniqueID;
		int iEquipClass, iItemID, iItemColor;	
		uiUniqueID = pInfo->m_RewordItem.m_iItemID;	
		iItemColor = (pInfo->m_RewordItem.m_iItemID) % 100;
		iEquipClass = (pInfo->m_RewordItem.m_iItemID) / 100000;
		iItemID = ((pInfo->m_RewordItem.m_iItemID) % 100000 - iItemColor) / 100;	

		SPNpcEventToolTip m_EventToolTip;
		m_EventToolTip.m_pItem.SetItemID(uiUniqueID);

		SPItemAttr* pItemAttr;
		g_pItemCluster->GetItemInfo(uiUniqueID, pItemAttr);

		if( pItemAttr )
		{
			m_EventToolTip.m_pItem.SPSendMessage(MV_ITEM_SETEQUIPID, pItemAttr->m_ePosID1);
			m_EventToolTip.m_pItem.SPSendMessage(MV_ITEM_SETEQUIPCLASS, iEquipClass);
			m_EventToolTip.m_pItem.SPSendMessage(MV_ITEM_SETEQUIPITEMID, iItemID);
			m_EventToolTip.m_pItem.SPSendMessage(MV_ITEM_SETCOLOR,	iItemColor);
			m_EventToolTip.m_pItem.SPSendMessage(MV_ITEM_SET_RARELEVEL, pInfo->m_RewordItem.m_iRear);

			if(pItemAttr->m_eDuration != ITEM_DURATION_NULL) {
				m_EventToolTip.m_pItem.SetLifeTime(pItemAttr->m_iLifeTime * 3600);
			}

			m_EventToolTip.m_strDesc = pInfo->m_strDesc;

			SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
			pWindow->SPSendMessage(SPIM_TOOLTIP_ENTER, (WPARAM)&m_EventToolTip, (LPARAM)-12);
			pWindow->Show();
		}
	}		

	return 1;
}


int SPWindowNpcEvent::OnRewordToolTipOut(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	int iIndex = iID - WIID_NPC_EVENT_LIST_BUTTON;
	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
	pWindow->Show();
	return 1;
}


int SPWindowNpcEvent::OnReqToolTipMove(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	int iIndex = iID - WIID_NPC_EVENT_REQ_ITEM_ICONBUTTON;
	
	if(m_ReqToolTipItem[iIndex].m_iItemID == 0) {
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
		pWindow->Show();
		return 1;
	}
	
	unsigned int uiUniqueID;
	int iEquipClass, iItemID, iItemColor;	
	uiUniqueID = m_ReqToolTipItem[iIndex].m_iItemID;	
	iItemColor = (m_ReqToolTipItem[iIndex].m_iItemID) % 100;
	iEquipClass = (m_ReqToolTipItem[iIndex].m_iItemID) / 100000;
	iItemID = ((m_ReqToolTipItem[iIndex].m_iItemID) % 100000 - iItemColor) / 100;	

	SPItemStatus kItemStatus;
	kItemStatus.SetItemID(uiUniqueID);

	SPItemAttr* pItemAttr;
	g_pItemCluster->GetItemInfo(uiUniqueID, pItemAttr);
	if( pItemAttr )
	{
		kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPID, pItemAttr->m_ePosID1);
		kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPCLASS, iEquipClass);
		kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPITEMID, iItemID);
		kItemStatus.SPSendMessage(MV_ITEM_SETCOLOR,	iItemColor);
		kItemStatus.SPSendMessage(MV_ITEM_SET_RARELEVEL, m_ReqToolTipItem[iIndex].m_iRear);

		if(pItemAttr->m_eDuration != ITEM_DURATION_NULL) {
			kItemStatus.SetLifeTime(pItemAttr->m_iLifeTime * 3600);
		}

		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage(SPIM_TOOLTIP_ENTER, (WPARAM)&kItemStatus, lParam);
		pWindow->Show();
	}

	return 1;
}


int SPWindowNpcEvent::OnReqToolTipEnter(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	int iIndex = iID - WIID_NPC_EVENT_REQ_ITEM_ICONBUTTON;
	if(m_ReqToolTipItem[iIndex].m_iItemID == 0) {
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
		pWindow->Show();
		return 1;
	}

	unsigned int uiUniqueID;
	int iEquipClass, iItemID, iItemColor;	
	uiUniqueID = m_ReqToolTipItem[iIndex].m_iItemID;	
	iItemColor = (m_ReqToolTipItem[iIndex].m_iItemID) % 100;
	iEquipClass = (m_ReqToolTipItem[iIndex].m_iItemID) / 100000;
	iItemID = ((m_ReqToolTipItem[iIndex].m_iItemID) % 100000 - iItemColor) / 100;	

	SPItemStatus kItemStatus;
	kItemStatus.SetItemID(uiUniqueID);

	SPItemAttr* pItemAttr;
	g_pItemCluster->GetItemInfo(uiUniqueID, pItemAttr);
	if( pItemAttr )
	{
		kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPID, pItemAttr->m_ePosID1);
		kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPCLASS, iEquipClass);
		kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPITEMID, iItemID);
		kItemStatus.SPSendMessage(MV_ITEM_SETCOLOR,	iItemColor);
		kItemStatus.SPSendMessage(MV_ITEM_SET_RARELEVEL, m_ReqToolTipItem[iIndex].m_iRear);

		if(pItemAttr->m_eDuration != ITEM_DURATION_NULL) {
			kItemStatus.SetLifeTime(pItemAttr->m_iLifeTime * 3600);
		}

		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage(SPIM_TOOLTIP_ENTER, (WPARAM)&kItemStatus, lParam);
		pWindow->Show();
	}
	return 1;
}


int SPWindowNpcEvent::OnReqToolTipOut(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	int iIndex = iID - WIID_NPC_EVENT_REQ_ITEM_ICONBUTTON;
	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
	pWindow->Show();
	return 1;
}


