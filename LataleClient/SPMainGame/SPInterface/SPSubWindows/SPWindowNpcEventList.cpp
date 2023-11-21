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

////
//#include "SPMainGameDEF.H"
//#include "SPSubGameManager.h"
//#include "SPMainGameManager.h"
//#include "SPMouseCursor.h"
//#include "SPWindowDEF.h"
//#include "SPInterfaceManager.h"
//#include "SPGOBClusterDef.h"
//#include "SPAvatarModelUnitDef.h"
//#include "SPGOBModel.h"
//#include "SPGOBModelUnit.h"
//#include "SPMonsterModelUnit.h"
//#include "SPAvatarModelUnit.h"
//#include "SPGOBCluster.h"
//#include "SPGameObject.h"
//#include "SPGOBStatus.h"
//#include "SPGOBCoordPhysics.h"
//#include "SPGOBModelUnit.h"			
//#include "SPPlayerEquipInfo.h"
//#include "SPGOBStatus.h"			
//#include "SPPlayerInvenArchive.h"	
//#include "SPPlayerStatusModel.h"	
//#include "SPPlayerEquipInfo.h"
//#include "SPItemAttr.h"
//#include "SPItemStatus.h"
//#include "SPPlayer.h"
//#include "SPGOBManager.h"

#include "SPCheckManager.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPWindowNpcEventList.h"
#include "SPWindowNpcEvent.h"

#include "SPMouseCursor.h"
#include "SPIMEPool.h"
#include "SPResourceDef.h"
#include "SPInterfaceManager.h"

#include "SPDebug.h"


SPWindowNpcEventList::SPWindowNpcEventList( INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent /*= NULL*/)
: SPWindow(WNDID_NPC_EVENTLIST, InstanceID, iX, iY, iCX, iCY, pParent)	
, m_pTextureBase (NULL)
{
	Init();
}


SPWindowNpcEventList::~SPWindowNpcEventList()
{
	Clean();
}


void SPWindowNpcEventList::Init()
{
	g_pVideo->GetTextureMgr()->LoadTexture("DATA/INTERFACE/CONCEPT/UI_WinBG_000.PNG" , &m_pTextureBase);	

	SetRect(&m_rtSrcBase[WINDOW_BASE_TOP_LEFT],		482, 482, 486, 486);
	SetRect(&m_rtSrcBase[WINDOW_BASE_TOP],			487, 482, 491, 486);	
	SetRect(&m_rtSrcBase[WINDOW_BASE_TOP_RIGHT],	492, 482, 496, 486);
	SetRect(&m_rtSrcBase[WINDOW_BASE_CENTER_LEFT],	482, 487, 486, 491);
	SetRect(&m_rtSrcBase[WINDOW_BASE_CENTER],		487, 487, 491, 491);
	SetRect(&m_rtSrcBase[WINDOW_BASE_CENTER_RIGHT],	492, 487, 496, 491);
	SetRect(&m_rtSrcBase[WINDOW_BASE_BOTTOM_LEFT],	482, 492, 486, 496);
	SetRect(&m_rtSrcBase[WINDOW_BASE_BOTTOM],		487, 492, 491, 496);
	SetRect(&m_rtSrcBase[WINDOW_BASE_BOTTOM_RIGHT],	492, 492, 496, 496);	

	SetRect(&m_rtDestBase[WINDOW_BASE_TOP_LEFT],		m_iAX, m_iAY, m_iAX + 4, m_iAY + 4);
	SetRect(&m_rtDestBase[WINDOW_BASE_TOP],				m_iAX + 4, m_iAY, m_iAX + 4 + 295, m_iAY + 4);	
	SetRect(&m_rtDestBase[WINDOW_BASE_TOP_RIGHT],		m_iAX + 299, m_iAY, m_iAX + 299 + 4, m_iAY + 4);
	SetRect(&m_rtDestBase[WINDOW_BASE_CENTER_LEFT],		m_iAX, m_iAY + 4, m_iAX + 4, m_iAY + 4 + 433);
	SetRect(&m_rtDestBase[WINDOW_BASE_CENTER],			m_iAX + 4, m_iAY + 4, m_iAX + 4 + 295, m_iAY + 4 + 433);
	SetRect(&m_rtDestBase[WINDOW_BASE_CENTER_RIGHT],	m_iAX + 299, m_iAY + 4, m_iAX + 299 + 4, m_iAY + 4 + 433);
	SetRect(&m_rtDestBase[WINDOW_BASE_BOTTOM_LEFT],		m_iAX, m_iAY + 437, m_iAX + 4, m_iAY + 437 + 4);
	SetRect(&m_rtDestBase[WINDOW_BASE_BOTTOM],			m_iAX + 4, m_iAY + 437, m_iAX + 4 + 295, m_iAY + 437 + 4);
	SetRect(&m_rtDestBase[WINDOW_BASE_BOTTOM_RIGHT],	m_iAX + 299, m_iAY + 437, m_iAX + 299 + 4, m_iAY + 437 + 4);
	
	InitSubControl();
}


void SPWindowNpcEventList::InitSubControl()
{
	char* BG_IMAGE_FILE0		= "DATA/INTERFACE/CONCEPT/UI_WinBG_000.PNG";
	char* OBJECT_IMAGE_FILE0	= "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG";
	char* OBJECT_IMAGE_FILE1	= "DATA/INTERFACE/CONCEPT/UI_Object_001.PNG";
	char* BUTTON_IMAGE_FILE0	= "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG";

	SPWindowStatic* pStatic		= NULL;
	SPWindowStatic* pSkin		= NULL;
	SPWindowButton* pButton		= NULL;
	
	//WIID_NPC_EVENTLIST_TITLE
	pStatic = new SPWindowStatic(WIID_NPC_EVENTLIST_TITLE,	2,	3,	110,	24, this);	
	pStatic->SetImage(OBJECT_IMAGE_FILE0, 189, 335);
	pStatic->Show();
	pStatic = NULL;
	
	////WIID_NPC_EVENTLIST_EXIT
	//pButton = new SPWindowButton(WIID_NPC_EVENTLIST_EXIT , 211, 9, 12, 12, this);
	//pButton->SetImage		(OPTION_IMAGE_FILE3, 301,  321);		
	//pButton->SetImageHit	(OPTION_IMAGE_FILE3, 314,  321);
	//pButton->SetImagePush	(OPTION_IMAGE_FILE3, 327,  321);
	//pButton->SetImageDisable(OPTION_IMAGE_FILE3, 340,  321);
	//pButton->Show();
	
	//WIID_NPC_EVENTLIST_LINE
	pStatic = new SPWindowStatic(WIID_NPC_EVENTLIST_LINE,	2,	27,	299,	1, this);	
	pStatic->SetImage(BG_IMAGE_FILE0, 503, 450);
	pStatic->SetSrcSize(2, 1);
	pStatic->Show();
	pStatic = NULL;

	//WIID_NPC_EVENTLIST_BASE
	int iSkinX = 2;
	int iSkinY = 28;
	pSkin = new SPWindowStatic(WIID_NPC_EVENTLIST_BASE,	iSkinX,	iSkinY,	299,	409, this);	
	pSkin->SetImage(BG_IMAGE_FILE0, 502, 487);
	pSkin->SetSrcSize(4, 4);
	pSkin->Show();

	//WIID_NPC_EVENTLIST_LIST_TITLE
	pStatic = new SPWindowStatic(WIID_NPC_EVENTLIST_LIST_TITLE,	18 - iSkinX, 42 - iSkinY, 148, 17, pSkin);	
	pStatic->SetImage(OBJECT_IMAGE_FILE0, 189, 360);
	pStatic->Show();
	pStatic = NULL;

	int iStartY = 72 - iSkinY;
	SPWindowStatic* pParent = NULL;
	for(int i = 0; i < EVENTLIST_PAGE; i++) {
		//WIID_NPC_EVENTLIST_LIST_SKIN
		pParent = new SPWindowStatic(WIID_NPC_EVENTLIST_LIST_SKIN + i,	12 - iSkinX,	iStartY + (i * 31), 283, 31, pSkin);
		pParent->Show();
		
		//WIID_NPC_EVENTLIST_LIST_BUTTON
		m_pButton[i] = new SPWindowButton(WIID_NPC_EVENTLIST_LIST_BUTTON + i, 0, 0, 283, 31, pParent);
		m_pButton[i]->Show();

		//WIID_NPC_EVENTLIST_LIST_SELECT		
		pStatic = new SPWindowStatic(WIID_NPC_EVENTLIST_LIST_SELECT + i, 0, 0, 283, 31, pParent);
		pStatic->SetWindowType( WND_TYPE_LEFTBASE );
		pStatic->SetImageNormalHead(OBJECT_IMAGE_FILE0, 198, 378, 208, 409);
		pStatic->SetImageNormalBody(OBJECT_IMAGE_FILE0, 204, 378, 208, 409);
		pStatic->SetImageNormalTail(OBJECT_IMAGE_FILE0, 208, 378, 218, 409);
		pStatic->Hide();
		m_pSelect[i] = pStatic;
		pStatic = NULL;
		
		//WIID_NPC_EVENTLIST_LIST_ICON
		pStatic = new SPWindowStatic(WIID_NPC_EVENTLIST_LIST_ICON + i, 8, 9, 17, 13, pParent);
		pStatic->SetImage(OBJECT_IMAGE_FILE0, 330, 112);
		pStatic->Show();
		m_pIcon[i] = pStatic;

		//WIID_NPC_EVENTLIST_LIST_NAME
		m_pName[i] = new SPWindowStatic(WIID_NPC_EVENTLIST_LIST_NAME + i, 32, 10, 237, 12, pParent);
		m_pName[i]->SetFormat(DT_LEFT | DT_VCENTER);
		m_pName[i]->SetFontColor(RGBA(0, 0, 0, 255));
		m_pName[i]->SetWindowText("Test000");
		m_pName[i]->Hide();
	}

	//WIID_NPC_EVENTLIST_PAGE_SKIN
	pStatic = new SPWindowStatic(WIID_NPC_EVENTLIST_PAGE_SKIN, 132 - iSkinX, 405 - iSkinY, 48, 14, pSkin);
	pStatic->SetWindowType( WND_TYPE_LEFTBASE );
	pStatic->SetImageNormalHead(OBJECT_IMAGE_FILE0, 393, 249, 397, 263);
	pStatic->SetImageNormalBody(OBJECT_IMAGE_FILE0, 398, 249, 397, 263);
	pStatic->SetImageNormalTail(OBJECT_IMAGE_FILE0, 402, 249, 406, 263);

	//WIID_NPC_EVENTLIST_PAGE
	m_pPage = new SPWindowStatic(WIID_NPC_EVENTLIST_PAGE,	0,	1,	48,	12, pStatic);
	m_pPage->SetFormat(DT_CENTER | DT_VCENTER);
	m_pPage->SetFontColor(RGBA(255, 255, 255, 255));
	m_pPage->SetWindowText("0 / 0");
	m_pPage->Show();
	
	//WIID_NPC_EVENTLIST_PAGE_PREV
	m_pPrev = new SPWindowButton(WIID_NPC_EVENTLIST_PAGE_PREV , 114 - iSkinX, 406 - iSkinY, 13, 13, pSkin);
	m_pPrev->SetImage		(BUTTON_IMAGE_FILE0, 484,  439);
	m_pPrev->SetImageHit	(BUTTON_IMAGE_FILE0, 484,  453);
	m_pPrev->SetImagePush	(BUTTON_IMAGE_FILE0, 484,  467);
	m_pPrev->SetImageDisable(BUTTON_IMAGE_FILE0, 484,  481);
	m_pPrev->Show();
	
	//WIID_NPC_EVENTLIST_PAGE_NEXT
	m_pNext = new SPWindowButton(WIID_NPC_EVENTLIST_PAGE_NEXT , 185 - iSkinX, 406 - iSkinY, 13, 13, pSkin);
	m_pNext->SetImage		(BUTTON_IMAGE_FILE0, 497,  439);
	m_pNext->SetImageHit	(BUTTON_IMAGE_FILE0, 497,  453);
	m_pNext->SetImagePush	(BUTTON_IMAGE_FILE0, 497,  467);
	m_pNext->SetImageDisable(BUTTON_IMAGE_FILE0, 497,  481);
	m_pNext->Show();

}


void SPWindowNpcEventList::Clean()
{
	SAFE_RELEASE(m_pTextureBase);
	SPWindow::Clean();
}


void SPWindowNpcEventList::Process(float fTime)
{
	SPWindow::Process(fTime);
}


void SPWindowNpcEventList::Render(float fTime)
{
	if(!m_bShow) return;

	if(m_pTextureBase) {
		g_pVideo->Flush();
		for(int i = 0; i < WINDOW_BASE_MAX; i++) {
			m_pTextureBase->RenderTexture(&m_rtDestBase[i], &m_rtSrcBase[i]);
		}
	}	
	
	SPWindow::Render(fTime);
}


void SPWindowNpcEventList::Show(bool bWithChild /*= true*/)
{	
	SPNpcEventList* pEventList = g_pEventManager->GetEventHandler()->GetNpcEventList();
	
	if(pEventList == NULL)
		return;
	
	int iListSize = pEventList->GetSize();
	if(iListSize % EVENTLIST_PAGE) {
		m_iMaxPage		= (iListSize / EVENTLIST_PAGE) + 1;
	}
	else {
		m_iMaxPage		= iListSize / EVENTLIST_PAGE;
	} 
	
	m_iCurPage		= 1;
	m_iSelect		= 0;
	pEventList->SetSelectReset();
	m_iSelectIndex	= 0;
	
	SPWindow::Show(bWithChild);
	
	ResetSubControl();

	UpdateEventList();
}


void SPWindowNpcEventList::Hide(bool bSendServer /*= true*/)
{
	SPWindow::Hide(bSendServer);
}


void SPWindowNpcEventList::ResetSubControl()
{
	for(int i = 0; i < EVENTLIST_PAGE; i++) {		
		m_pIcon[i]->Hide();
		m_pSelect[i]->Hide();
		m_pName[i]->SetWindowText(NULL);
		m_pName[i]->SetFontColor(RGBA(0, 0, 0, 255));		
	}
}


void SPWindowNpcEventList::UpdateEventList()
{
	SPNpcEventList* pEventList = g_pEventManager->GetEventHandler()->GetNpcEventList();
	if(pEventList == NULL)
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

	ResetSubControl();
	
	SPNpcEvent* pNpcEvent = NULL;
	for(int i = 0; i < EVENTLIST_PAGE; i++) {		
		int iIndex = i + ((m_iCurPage - 1) * EVENTLIST_PAGE);
		pNpcEvent = NULL;
		pNpcEvent = pEventList->GetNpcEvent(iIndex);
		if(pNpcEvent == NULL)
			continue;
		
		if(pNpcEvent->m_iItemID) {
			m_pName[i]->SetWindowText(pNpcEvent->m_strName.c_str());
			m_pIcon[i]->Show();
		}

		if(i == m_iSelect) {
			m_pSelect[i]->Show();
			m_pName[i]->SetFontColor(RGBA(255, 255, 255, 255));
		}
		else {
			m_pSelect[i]->Hide();
			m_pName[i]->SetFontColor(RGBA(0, 0, 0, 255));
		}		
	}

	m_iSelectIndex = ((m_iCurPage - 1) * EVENTLIST_PAGE) + m_iSelect;	
	if(m_iSelectIndex < 0 || m_iSelectIndex >= pEventList->GetSize()) {
		m_iSelectIndex = 0;
	}	
	
	pEventList->SetSelect(m_iSelectIndex);
	SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_NPC_EVENT);
	if(pWindow){
		pWindow->Show();
	}
}


void SPWindowNpcEventList::UpdateControl()
{

}


SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowNpcEventList )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_NPC_EVENTLIST_PAGE_PREV,			OnPrev)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_NPC_EVENTLIST_PAGE_NEXT,			OnNext)
SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP,		WIID_NPC_EVENTLIST_LIST_BUTTON,		WIID_NPC_EVENTLIST_LIST_BUTTON + EVENTLIST_PAGE,	OnItemClick)
SPIMESSAGE_OBJ_MAP_END	( SPWindow	)


int SPWindowNpcEventList::OnPrev(WPARAM wParam, LPARAM lParam)
{
	if( m_iCurPage > 1  && m_iCurPage <= m_iMaxPage ) {
		m_iCurPage--;

		UpdateEventList();
	}
	return 1;
}


int SPWindowNpcEventList::OnNext(WPARAM wParam, LPARAM lParam)
{
	if( m_iCurPage > 0 && m_iCurPage < m_iMaxPage ) {
		m_iCurPage++;

		UpdateEventList();
	}
	
	return 1;
}


int SPWindowNpcEventList::OnItemClick(unsigned int uiID, WPARAM wParam, LPARAM lParam)
{
	m_iSelect = uiID - WIID_NPC_EVENTLIST_LIST_BUTTON;	
	DXUTOutputDebugString("EventList Select %d\n", m_iSelect);	
	
	int iIndex = m_iSelect + ((m_iCurPage - 1) * EVENTLIST_PAGE);	
	SPNpcEventList* pEventList = g_pEventManager->GetEventHandler()->GetNpcEventList();
	if(pEventList == NULL)
		return 1;
	
	if(iIndex >= pEventList->GetSize())
		return 1;

	////
	//for(int i = 0; i < EVENTLIST_PAGE; i++) {
	//	if(i == m_iSelect) {
	//		m_pSelect[i]->Show();
	//		m_pName[i]->SetFontColor(RGBA(255, 255, 255, 255));
	//	}
	//	else {
	//		m_pSelect[i]->Hide();
	//		m_pName[i]->SetFontColor(RGBA(0, 0, 0, 255));
	//	}
	//}

	UpdateEventList();
	
	return 1;
}


