
#include <vector>

#include "SPCommon.h"

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
#include "SPWindowGrid.h"
#include "SPWindowStorage.h"

#include "SPStage.h"
#include "SPStageManager.h"
#include "SPTerrainAttribute.h"

#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGOBCoordPhysics.h"
#include "SPPlayerCoordPhysics.h"
#include "SPPlayerInvenArchive.h"

#include "SPGOBStatus.h"
#include "SPPlayerStatus.h"
#include "SPPlayerStatusModel.h"

#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPItem.h"
#include "SPItemCluster.h"			//[2005/6/16] - jinhee
#include "SPPlayer.h"

#include "SPEventDef.h"
#include "SPEventManager.h"

#include "SPCheckManager.h"

#include "SPUtil.h"

#include "SPDebug.h"


SPWindowStorage::SPWindowStorage(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_STORAGE, InstanceID, iX, iY, iCX, iCY, pParent)	
{	
	Init();
}

SPWindowStorage::~SPWindowStorage()
{
	Clean();
}

void SPWindowStorage::Init()
{	
	//m_iSlotCount = 4;
	//m_iViewSlotCount = DEFAULT_STORAGE_SLOT;
	m_iSlotCount = 0;
	m_iViewSlotCount = 0;
	//SetRect(&m_rtSrcSlot,		203, 408, 238, 440);	
	
	m_iMaxPage = 0;
	m_iCurPage = 0;

	//SetSlotPos();

	CreateSubWindow();
}


void SPWindowStorage::CreateSubWindow()
{
	SPWindow* pStatic;
	//SPWindow* pChild;
	
	SPWindow* pCenter = NULL;
	SPWindowButton* pButton;
	
	pStatic = new SPWindowStatic(WIID_STORAGE_TOP, 0, 0, 198, 4, this);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 61);
	pStatic->Show();
	pStatic = NULL;

	pCenter = new SPWindowStatic(WIID_STORAGE_CENTER, 0, 4, 198, 233, this);
	pCenter->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 67);
	pCenter->SetSrcSize(198, 2);
	pCenter->Show();

	pStatic = new SPWindowStatic(WIID_STORAGE_BOTTOM, 0, 237, 198, 4, this);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 71);
	pStatic->Show();
	pStatic = NULL;

	pButton = new SPWindowButton(WIID_STORAGE_CLOSE, 177, 9 - 4, 12, 12, pCenter);
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI103.PNG",	301,	321);
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI103.PNG",	314,	321); //on
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI103.PNG",	327,	321); //push
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG",	340,	321); //disable
	pButton->Show();
	pButton = NULL;

	//밝은색 스킨
	SPWindow* pMiddle = NULL;
	pMiddle = new SPWindowStatic(WIID_STORAGE_MIDDLE, 2, 26 - 4, 194, 43, pCenter);
	pMiddle->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 493, 486);
	pMiddle->SetSrcSize(6, 4);
	pMiddle->Show();

	//Title
	pStatic = new SPWindowStatic(WIID_STORAGE_TITLE, 10, 7 - 4, 57, 15, pCenter);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 1, 360);
	pStatic->Show();
	pStatic = NULL;

	//흰 스킨
	SPWindow* pSkin = NULL;
	pSkin = new SPWindowStatic(WIID_STORAGE_SKIN, 2, 70 - 4, 194, 164, pCenter);
	pSkin->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 493, 477);
	pSkin->SetSrcSize(6, 4);
	pSkin->Show();

	//흰스킨 상단 경계
	pStatic = new SPWindowStatic(WIID_STORAGE_SKIN_UP_LINE, 2, 69 - 4, 194, 1, pCenter);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 493, 494);
	pStatic->SetSrcSize(6, 1);
	pStatic->Show();
	pStatic = NULL;

	//흰스킨 하단 경계
	pStatic = new SPWindowStatic(WIID_STORAGE_SKIN_DOWN_LINE, 2, 234 - 4, 194, 1, pCenter);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 493, 494);
	pStatic->SetSrcSize(6, 1);
	pStatic->Show();
	pStatic = NULL;

	////////////////////////////////////////////////////////////////////////// Middle	
	m_pElyEdit = new SPWindowEdit( WIID_STORAGE_INPUT_ELY, 7 - 2, 48 - 26, 108, 17, pMiddle);
	m_pElyEdit->SetWindowType(WND_TYPE_LEFTBASE);
	m_pElyEdit->SetMargin( 2, 1);
	m_pElyEdit->SetImageNormalHead	("DATA/INTERFACE/CONCEPT/UI103.PNG", 501, 469, 503, 486);
	m_pElyEdit->SetImageNormalBody	("DATA/INTERFACE/CONCEPT/UI103.PNG", 505, 469, 507, 486);
	m_pElyEdit->SetImageNormalTail	("DATA/INTERFACE/CONCEPT/UI103.PNG", 509, 469, 511, 486);
	m_pElyEdit->SetImageFocusHead	("DATA/INTERFACE/CONCEPT/UI103.PNG", 501, 451, 503, 468);
	m_pElyEdit->SetImageFocusBody	("DATA/INTERFACE/CONCEPT/UI103.PNG", 505, 451, 507, 468);
	m_pElyEdit->SetImageFocusTail	("DATA/INTERFACE/CONCEPT/UI103.PNG", 509, 451, 511, 468);	
	m_pElyEdit->SetNumber();
	m_pElyEdit->SetMaxValue(9999999999999);
	//m_pElyEdit->SetFontColor(RGBA(143, 99, 99, 255));
	m_pElyEdit->SetWindowText("0");
	m_pElyEdit->SetFormat(DT_RIGHT | DT_VCENTER);	
	m_pElyEdit->SetLimitText(13);
	m_pElyEdit->Show();

	m_pElySave = new SPWindowButton(WIID_STORAGE_ELY_SAVE, 119 - 2, 49 - 26, 35, 15, pMiddle);	//보관
	m_pElySave->SetImage		("DATA/INTERFACE/CONCEPT/UI103.PNG",	131,	376);
	m_pElySave->SetImageHit		("DATA/INTERFACE/CONCEPT/UI103.PNG",	131,	392); //on
	m_pElySave->SetImagePush	("DATA/INTERFACE/CONCEPT/UI103.PNG",	131,	408); //push
	m_pElySave->SetImageDisable	("DATA/INTERFACE/CONCEPT/UI103.PNG",	131,	424); //disable
	m_pElySave->Show();

	m_pElyLoad = new SPWindowButton(WIID_STORAGE_ELY_LOAD, 153 - 2, 49 - 26, 35, 15, pMiddle);	//찾음
	m_pElyLoad->SetImage		("DATA/INTERFACE/CONCEPT/UI103.PNG",	168,	376);
	m_pElyLoad->SetImageHit		("DATA/INTERFACE/CONCEPT/UI103.PNG",	168,	392); //on
	m_pElyLoad->SetImagePush	("DATA/INTERFACE/CONCEPT/UI103.PNG",	168,	408); //push
	m_pElyLoad->SetImageDisable	("DATA/INTERFACE/CONCEPT/UI103.PNG",	168,	424); //disable
	m_pElyLoad->Show();

	pStatic = new SPWindowStatic(WIID_STORAGE_ELY_TEXT, 10 -2 , 32 - 26, 45, 11, pMiddle);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG",	292,	335);
	pStatic->Show();
	pStatic = NULL;

	m_pStorageEly = new SPWindowStatic(WIID_STORAGE_ELY, 63 - 2, 30 - 26, 102, 14, pMiddle);
	m_pStorageEly->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG",	1,	345);
	m_pStorageEly->SetSrcSize(102, 14);
	m_pStorageEly->SetFontColor(RGBA(143, 99, 99, 255));
	m_pStorageEly->SetFormat(DT_RIGHT | DT_VCENTER | DT_NOCLIP);
	m_pStorageEly->SetWindowText("11111111");	
	m_pStorageEly->Show();

	pStatic = new SPWindowStatic(WIID_STORAGE_ELY_IMG, 169 - 2, 36 - 26, 45, 11, pMiddle);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG",	494,	506);
	pStatic->Show();
	pStatic = NULL;

	//pStatic = NULL;

	////////////////////////////////////////////////////////////////////////// Skin
	pButton = new SPWindowButton(WIID_STORAGE_PAGE_UP, 172 -2 , 78 - 70 - 4, 12, 11, pSkin);		//Page Up
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI103.PNG", 460,	386);
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI103.PNG", 473,	386); //on
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI103.PNG", 486,	386); //push
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG", 499,	386); //disable
	pButton->Show();
	pButton = NULL;

	m_pSlider	=	new SPWindowSlider(WIID_STORAGE_SLIDER, 172 - 2, 89 - 70 - 4, 12, 126, pSkin );//스크롤바
	RECT rtRect;
	
	SetRect( &rtRect , 460 , 398 , 472 , 422 );	//	일반
	m_pSlider->SetImageHandle( "DATA/INTERFACE/CONCEPT/UI103.PNG" , rtRect );

	SetRect( &rtRect , 473 , 398 , 485 , 422 );	//	마우스 오버
	m_pSlider->SetImageHandleHit( "DATA/INTERFACE/CONCEPT/UI103.PNG" , rtRect );

	SetRect( &rtRect , 486 , 398 , 498 , 422 );	//	마우스 클릭
	m_pSlider->SetImageHandlePush( "DATA/INTERFACE/CONCEPT/UI103.PNG" , rtRect );

	SetRect( &rtRect , 499 , 398 , 511 , 422 );
	m_pSlider->SetImageHandleDisable( "DATA/INTERFACE/CONCEPT/UI103.PNG" , rtRect );

	SetRect( &rtRect , 460 , 423 , 472 , 427 );	//	일반
	m_pSlider->SetImagePageNormal( "DATA/INTERFACE/CONCEPT/UI103.PNG" , rtRect );

	SetRect( &rtRect , 473 , 423 , 485 , 427 );	//	마우스 오버
	m_pSlider->SetImagePageHit( "DATA/INTERFACE/CONCEPT/UI103.PNG" , rtRect );

	SetRect( &rtRect , 486 , 423 , 498 , 427 );	//	마우스 클릭
	m_pSlider->SetImagePageShadow( "DATA/INTERFACE/CONCEPT/UI103.PNG" , rtRect );

	SetRect( &rtRect , 499 , 423 , 511 , 427 );
	m_pSlider->SetImagePageDisable( "DATA/INTERFACE/CONCEPT/UI103.PNG" , rtRect );


	m_pSlider->SPSendMessage( SPIM_SLIDER_PAGESIZE , 1 );
	m_pSlider->SetCurpage( 0 );
	m_pSlider->SPSendMessage( SPIM_SET_MAXVALUE , 0 );

	pButton = new SPWindowButton(WIID_STORAGE_PAGE_DOWN, 172 - 2, 215 - 70 - 4, 12, 11, pSkin);	//Page Down
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI103.PNG", 460,	428);
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI103.PNG", 473,	428); //on
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI103.PNG", 486,	428); //push
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG", 499,	428); //disable
	pButton->Show();
	pButton = NULL;				

	m_pGrid = new SPWindowGrid(WNDID_CTRL_GRID, WIID_STORAGE_GRID, 14- 2, 79 - 70 - 4, 152, 152, pSkin, 4, 4, 38, 38, 32);
	//m_pGrid->SetImage("");	

	int iLeft = 14 - 2; 
	int iTop = 79 - 70 - 4;	
	for(int i = 0; i < MAX_STORAGE_SLOT; i++) {
		int iCol = i % LINE_SLOT;
		int iLine = i / LINE_SLOT;
		int iStartX = iLeft + (iCol * 38);
		int iStartY = iTop + (iLine * 38);
		//DXUTOutputDebugString("Storage Grid[%d] iStartX[%d] iStartY[%d]\n", i, iStartX, iStartY);
		m_pGridLine[i] = new SPWindowStatic(WIID_STORAGE_GRID_LINE + i, iStartX, iStartY, 32, 32, pSkin);
		m_pGridLine[i]->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 203,	408);
		//m_pGridLine[i]->SetSrcSize(32, 32);
		m_pGridLine[i]->Hide();
	}
}


////
//void SPWindowStorage::SetSlotPos()
//{
//	int iSx = m_iAX + 14;
//	int iSy = m_iAY + 79;
//	int iLine = 0;
//	int iCol = 0;
//	int iRenderX = 0;
//	int iRenderY = 0;
//	for(int i = 0 ; i < MAX_STORAGE_SLOT ; i++) {
//		iLine = i / LINE_SLOT;
//		iCol = i % LINE_SLOT;
//		iRenderX = iSx + (iCol * 34);		
//		iRenderY = iSy + (iLine * 34);
//		SetRect(&m_rtDestSlot[i], iRenderX, iRenderY, iRenderX + 32, iRenderY + 32);
//	}	
//}


void SPWindowStorage::Clean()
{
	//SAFE_RELEASE(m_pTexture);
	//SAFE_RELEASE(m_pSlotTexture);
	
	SPWindow::Clean();
}


void SPWindowStorage::Process(float fTime)
{
	SPWindow::Process(fTime);	
}


void SPWindowStorage::Render(float fTime)
{	
	if(!m_bShow) return;	
	
	for(int i = 0; i < MAX_STORAGE_SLOT; i++) {
		if(m_pGridLine[i])
			m_pGridLine[i]->Hide();
	}
	m_pGrid->Hide();
	
	SPWindow::Render(fTime);
	
	for(int i = 0; i < m_iViewSlotCount ; i++) {
		if(m_pGridLine[i]) {
			m_pGridLine[i]->Show();
			m_pGridLine[i]->Render(fTime);
		}
	}
	
	m_pGrid->Show();
	m_pGrid->Render(fTime);
}

////
//void SPWindowStorage::SlotRender()
//{
//	for(int i = 0; i < m_iViewSlotCount ; i++) {
//		if(m_pSlotTexture) {
//			if(i < MAX_STORAGE_SLOT) {
//				m_pSlotTexture->RenderTexture(&m_rtDestSlot[i], &m_rtSrcSlot);
//			}
//		}
//	}	
//	
//	return ;
//}


void SPWindowStorage::Show(bool bWithChild)
{	
	if(m_bShow == false && g_pGOBManager->GetLocalPlayer()) {
		g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->ReflashStorage();
	}

	SPWindow::Show(bWithChild);
	Refresh();
}


void SPWindowStorage::Refresh()
{
	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return ;

	//itoa(g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValueEX(STATUS_EX_STORAGE_ELY), m_szStorageEly, 10);
	//m_pStorageEly->SetWindowText(m_szStorageEly);
	m_pStorageEly->SetWindowText(ConvertMoneyToString(g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValueEX(STATUS_EX_STORAGE_ELY)));

	m_pElyEdit->SetNULL();
	m_pElyEdit->SetWindowText(NULL);
}



//////////////////////////////////////////////////////////////////////////
bool SPWindowStorage::SendSaveEly(UINT64 iSaveEly)
{	
	if(g_pCheckManager->IsDBBlock()) {
		DXUTOutputDebugString("\tSPWindowStorage::SendSaveEly DB Working Block\n");
		return false;
	}
	
	CPacket Packet(STORAGE_CS_INMONEY);	
	Packet.Add((UINT64)iSaveEly);	
	g_pNetworkManager->SPPerformMessage(STORAGE_CS_INMONEY, 0, (LPARAM)&Packet);	
	
	return true;
}


bool SPWindowStorage::SendLoadEly(UINT64 iLoadEly)
{
	if(g_pCheckManager->IsDBBlock()) {

		DXUTOutputDebugString("\tSPWindowStorage::SendLoadEly DB Working Block\n");
		return false;
	}
	
	CPacket Packet(STORAGE_CS_OUTMONEY);	
	Packet.Add((UINT64)iLoadEly);	
	g_pNetworkManager->SPPerformMessage(STORAGE_CS_OUTMONEY, 0, (LPARAM)&Packet);	

	return true;
}



//////////////////////////////////////////////////////////////////////////
SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowStorage )
SPIMESSAGE_COMMAND(	SPIM_CURSOR_ENTER,		OnCursorEnter	)
SPIMESSAGE_COMMAND(	SPIM_CURSOR_OUT,		OnCursorOut		)
SPIMESSAGE_COMMAND(	SPIM_LBUTTON_DOWN,		OnCursorLDown	)
SPIMESSAGE_COMMAND(	SPIM_LBUTTON_UP,		OnCursorLUp		)
SPIMESSAGE_COMMAND(	SPIM_REFRESH,			OnRefresh		)

SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,	WIID_STORAGE_TOP,		OnCursorLUp	)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,	WIID_STORAGE_CENTER,	OnCursorLUp	)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,	WIID_STORAGE_BOTTOM,	OnCursorLUp	)

SPIMESSAGE_CONTROL( SPIM_REFRESH,		WIID_STORAGE_INPUT_ELY, OnUpdateEly )

SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,	WIID_STORAGE_ELY_SAVE,	OnSaveEly	)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,	WIID_STORAGE_ELY_LOAD,	OnLoadEly	)
SPIMESSAGE_CONTROL(	SPIM_SETFOCUS,		WIID_STORAGE_INPUT_ELY,	OnInputEly	)

SPIMESSAGE_CONTROL(	SPIM_CURSOR_MOVE,	WIID_STORAGE_GRID,		OnCursorMove)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_DOWN,	WIID_STORAGE_GRID,		OnGridDown	)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,	WIID_STORAGE_GRID,		OnGridUp	)

SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_ENTER,	WIID_STORAGE_GRID,		OnToolTipEnter	)
SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_OUT,	WIID_STORAGE_GRID,		OnToolTipOut	)
SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_MOVE,	WIID_STORAGE_GRID,		OnToolTipMove	)

SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,	WIID_STORAGE_PAGE_UP,	OnButtonPageUp		)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,	WIID_STORAGE_PAGE_DOWN,	OnButtonPageDown	)

SPIMESSAGE_COMMAND( SPIM_WHEEL_UP,								OnButtonPageUp		)
SPIMESSAGE_COMMAND( SPIM_WHEEL_DOWN,							OnButtonPageDown	)
//SPIMESSAGE_CONTROL( SPIM_WHEEL_UP,		WIID_STORAGE_TOP,		OnButtonPageUp		)
//SPIMESSAGE_CONTROL( SPIM_WHEEL_DOWN,	WIID_STORAGE_TOP,		OnButtonPageDown	)
SPIMESSAGE_CONTROL( SPIM_WHEEL_UP,		WIID_STORAGE_GRID,		OnButtonPageUp		)
SPIMESSAGE_CONTROL( SPIM_WHEEL_DOWN,	WIID_STORAGE_GRID,		OnButtonPageDown	)
//SPIMESSAGE_CONTROL( SPIM_WHEEL_UP,		WIID_STORAGE_BOTTOM,	OnButtonPageUp		)
//SPIMESSAGE_CONTROL( SPIM_WHEEL_DOWN,	WIID_STORAGE_BOTTOM,	OnButtonPageDown	)

SPIMESSAGE_CONTROL( SPIM_SLIDER_UP,			WIID_STORAGE_SLIDER,	OnSliderPageUp	)
SPIMESSAGE_CONTROL( SPIM_SLIDER_DOWN,		WIID_STORAGE_SLIDER,	OnSliderPageDown)
SPIMESSAGE_CONTROL( SPIM_SLIDER_PAGEUP,		WIID_STORAGE_SLIDER,	OnSliderPageUp	)
SPIMESSAGE_CONTROL( SPIM_SLIDER_PAGEDOWN,	WIID_STORAGE_SLIDER,	OnSliderPageDown)
SPIMESSAGE_CONTROL( SPIM_SET_CURVALUE,		WIID_STORAGE_SLIDER,	OnSliderPageNum	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_STORAGE_SLIDER,	OnSliderButtonUp)

SPIMESSAGE_COMMAND( SPIM_STORAGE_INITSLOT,	InitSlot		)
SPIMESSAGE_COMMAND( SPIM_STORAGE_SETSLOT,	SetExtendSlot	)

SPIMESSAGE_COMMAND(	SPIM_ITEM_SETTING,		OnItemSetting	)
SPIMESSAGE_COMMAND(	SPIM_ITEM_ALLCLEAR ,	OnItemAllClear	)
SPIMESSAGE_COMMAND(	SPIM_ITEM_CLEAR ,		OnItemClear		)
SPIMESSAGE_COMMAND( SPIM_ITEM_SETSTACK,		OnItemSetStack	)
SPIMESSAGE_COMMAND( SPIM_ITEM_SELECT,		OnItemSetIconSelect)
SPIMESSAGE_COMMAND( SPIM_ITEM_DISABLE,		OnItemDisable)

SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,	WIID_STORAGE_ELY,		OnCursorLUp	)

SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,	WIID_STORAGE_CLOSE,		OnClose		)

SPIMESSAGE_COMMAND( SPIM_STORAGE_REFRESH,	OnRefresh )

//SPIMESSAGE_COMMAND()

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)


int SPWindowStorage::OnCursorEnter	( WPARAM, LPARAM )
{
	return 1;
}


int SPWindowStorage::OnCursorOut	( WPARAM, LPARAM )
{
	return 1;
}


int SPWindowStorage::OnCursorLDown	( WPARAM, LPARAM )
{
	return 1;
}


int SPWindowStorage::OnCursorLUp	( WPARAM, LPARAM )
{
	if(g_pGOBManager->GetLocalPlayer())
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);

	return 1;
}


int SPWindowStorage::OnRefresh	( WPARAM wParam, LPARAM lParam)
{		
	Refresh();
	return 1;
}


int	SPWindowStorage::OnButtonPageUp(WPARAM wParam, LPARAM lParam)
{
	m_pSlider->SPSendMessage( SPIM_SLIDER_UP );
	return 1;
}


int	SPWindowStorage::OnButtonPageDown(WPARAM wParam, LPARAM lParam)
{
	m_pSlider->SPSendMessage( SPIM_SLIDER_DOWN );
	return 1;
}


int	SPWindowStorage::OnSliderPageUp(WPARAM wParam, LPARAM lParam)
{	
	m_iCurPage--;
	if(m_iCurPage < 0)
		m_iCurPage = 0;

	m_iViewSlotCount = m_iSlotCount - (m_iCurPage * LINE_SLOT);
	if(m_iViewSlotCount > MAX_STORAGE_SLOT)
		m_iViewSlotCount = MAX_STORAGE_SLOT;

	if(g_pGOBManager->GetLocalPlayer()) {
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_STORAGE_PAGE_UP , 0);

		//SPWindow* pWindow = Find(WIID_STORAGE_GRID);
		//pWindow->SPSendMessage(SPIM_TOOLTIP_MOVE, 0, MAKELONG(g_pInterfaceManager->GetCursorX(), g_pInterfaceManager->GetCursorY()));
		m_pGrid->SPSendMessage(SPIM_TOOLTIP_MOVE, 0, MAKELONG(g_pInterfaceManager->GetCursorX(), g_pInterfaceManager->GetCursorY()));
	}
	
	return 1;
}


int	SPWindowStorage::OnSliderPageDown(WPARAM wParam, LPARAM lParam)
{
	m_iCurPage++;
	if(m_iCurPage > m_iMaxPage)
		m_iCurPage = m_iMaxPage;

	m_iViewSlotCount = m_iSlotCount - (m_iCurPage * LINE_SLOT);
	if(m_iViewSlotCount > MAX_STORAGE_SLOT)
		m_iViewSlotCount = MAX_STORAGE_SLOT;

	if(g_pGOBManager->GetLocalPlayer()) {
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_STORAGE_PAGE_DOWN , 0);

		//SPWindow* pWindow = Find(WIID_STORAGE_GRID);
		//pWindow->SPSendMessage(SPIM_TOOLTIP_MOVE, 0, MAKELONG(g_pInterfaceManager->GetCursorX(), g_pInterfaceManager->GetCursorY()));
		m_pGrid->SPSendMessage(SPIM_TOOLTIP_MOVE, 0, MAKELONG(g_pInterfaceManager->GetCursorX(), g_pInterfaceManager->GetCursorY()));
	}

	return 1;
}


int	SPWindowStorage::OnSliderPageNum(WPARAM wParam, LPARAM lParam)
{	
	int iPage = (int)wParam;
	DXUTOutputDebugString("StoragePage[%d]\n", iPage);

	if(iPage < 0)
		iPage = 0;

	if(iPage > m_iMaxPage)
		iPage = m_iMaxPage;

	m_iCurPage = iPage;

	m_iViewSlotCount = m_iSlotCount - (m_iCurPage * LINE_SLOT);
	if(m_iViewSlotCount > MAX_STORAGE_SLOT)
		m_iViewSlotCount = MAX_STORAGE_SLOT;
	
	if(g_pGOBManager->GetLocalPlayer())
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_STORAGE_SETPAGE , wParam);
	
	return 1;
}


int SPWindowStorage::OnSliderButtonUp(WPARAM wParam, LPARAM lParam)
{
	if(g_pGOBManager->GetLocalPlayer())
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
	return 1;
}


int SPWindowStorage::OnClose( WPARAM wParam, LPARAM lParam )
{
	if(g_pGOBManager->GetLocalPlayer())
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);

	g_pEventManager->SetCutInState(CUT_IN_STATE_RELEASE);


	DXUTOutputDebugString("SPWindowStorage::OnClose::CUT_IN_STATE_NULL\n");
	return 1;
}


int SPWindowStorage::InitSlot(WPARAM wParam, LPARAM lParam)
{
	m_iSlotCount = DEFAULT_STORAGE_SLOT;
	m_iViewSlotCount = m_iSlotCount;
	m_iMaxPage = 0;
	m_iCurPage = 0;
	
	if(m_iSlotCount > MAX_STORAGE_SLOT) {
		m_iViewSlotCount = MAX_STORAGE_SLOT;

		m_iMaxPage = (m_iSlotCount - MAX_STORAGE_SLOT) / LINE_SLOT;
		if( ((m_iSlotCount - MAX_STORAGE_SLOT) % LINE_SLOT) > 0 )
			m_iMaxPage++;
	}		

	m_pSlider->SPSendMessage(SPIM_SET_MAXVALUE, (WPARAM)m_iMaxPage + 1);
	m_pSlider->SPSendMessage(SPIM_SET_CURVALUE, (WPARAM)m_iCurPage);
	return 1;
}


int SPWindowStorage::SetExtendSlot(WPARAM wParam, LPARAM lParam)
{
	int iExtendCount = (int)wParam;	
	//int iPage = 0;

	m_iMaxPage = 0;

	m_iSlotCount = m_iSlotCount + iExtendCount;
	m_iViewSlotCount = m_iSlotCount;
	
	//if(iExtendCount > MAX_STORAGE_SLOT) {
	if(m_iSlotCount > MAX_STORAGE_SLOT) {
		m_iViewSlotCount = MAX_STORAGE_SLOT;

		m_iMaxPage = (m_iSlotCount - MAX_STORAGE_SLOT) / LINE_SLOT;
		if( ((m_iSlotCount - MAX_STORAGE_SLOT) % LINE_SLOT) > 0 )
			m_iMaxPage++;
	}		

	m_pSlider->SPSendMessage(SPIM_SET_MAXVALUE, (WPARAM)m_iMaxPage + 1);
	m_pSlider->SPSendMessage(SPIM_SET_CURVALUE, (WPARAM)m_iCurPage);
	return 1;
}


int SPWindowStorage::OnUpdateEly(WPARAM wParam, LPARAM lParam)
{
	INT64 iEly = atoi(m_pElyEdit->GetWindowText());
	INT64 iUserEly = 0;
	INT64 iUserStorageEly = 0;
	INT64 iMaxEly = 0;
	
	if(g_pGOBManager->GetLocalPlayer()) {
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
		iUserEly = g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValueEX(STATUS_EX_ELY);
		iUserStorageEly = g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValueEX(STATUS_EX_STORAGE_ELY);
	}

	if(iUserEly >= iUserStorageEly){
		iMaxEly = iUserEly;
	}
	else {
		iMaxEly = iUserStorageEly;
	}

	if(iEly < 0 || iEly > iMaxEly) {
		iEly = iUserEly;
		char szTemp[32];
		ZeroMemory(szTemp, 32);
		//wsprintf(szTemp, "%I64d", iEly);
		sprintf(szTemp, "%I64d", iEly);
		//wsprintf(szTemp, "%d", iEly);
		m_pElyEdit->SetWindowText(szTemp);
	}

	return 1;
}


int SPWindowStorage::OnSaveEly(WPARAM wParam, LPARAM lParam)
{	
	if(g_pGOBManager->GetLocalPlayer())
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
	
	INT64 iSaveEly = 0;
	INT64 iUserEly = 0;
	
	iSaveEly = _atoi64(m_pElyEdit->GetWindowText());	

	iUserEly = g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValueEX(STATUS_EX_ELY);

	if(iSaveEly > iUserEly || iSaveEly < 1) {
		m_pElyEdit->SetNULL();
		m_pElyEdit->SetWindowText(NULL);		
		m_pElySave->SetEnable(false);
		return 0;
	}

	DXUTOutputDebugString("OnSaveEly [%d]\n", iSaveEly);
	
	SendSaveEly(iSaveEly);
	return 1;
}


int SPWindowStorage::OnLoadEly(WPARAM wParam, LPARAM lParam)
{
	if(g_pGOBManager->GetLocalPlayer())
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);

	INT64 iLoadEly = 0;
	INT64 iStorageEly = 0;
	
	iLoadEly = _atoi64(m_pElyEdit->GetWindowText());	
	iStorageEly = g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValueEX(STATUS_EX_STORAGE_ELY);
	
	if(iLoadEly > iStorageEly || iLoadEly < 1) {
		m_pElyEdit->SetNULL();
		m_pElyEdit->SetWindowText(NULL);		
		m_pElyLoad->SetEnable(false);
		return 0;
	}
	
	DXUTOutputDebugString("OnLoadEly [%d]\n", iLoadEly);

	SendLoadEly(iLoadEly);
	return 1;
}


int	SPWindowStorage::OnInputEly(WPARAM wParam, LPARAM lParam)
{		
	if(g_pGOBManager->GetLocalPlayer())
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);

	m_pElySave->SetEnable(true);
	m_pElyLoad->SetEnable(true);
	return 1;
}


int SPWindowStorage::OnCursorMove(WPARAM wParam, LPARAM lParam)
{	
	//
	if(g_pGOBManager->GetLocalPlayer())	// metalgeni
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_STORAGE_CURSOR_MOVE , (LPARAM)lParam);	

	return 1;
}


int SPWindowStorage::OnGridDown(WPARAM wParam, LPARAM lParam)
{
	int iSlotIndex = (int)lParam;
	if(iSlotIndex < 0 || iSlotIndex >= m_iViewSlotCount || iSlotIndex >= MAX_STORAGE_SLOT) {
		//범위 오류
		iSlotIndex = -1;
		if(g_pGOBManager->GetLocalPlayer())
			g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
	}
	
	//
	if(g_pGOBManager->GetLocalPlayer())	// metalgeni
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_STORAGE_CLICK_DOWN , (LPARAM)iSlotIndex);	

	//DXUTOutputDebugString("SPWindowStorage OnGridDown[%d]\n", iSlotIndex);

	return 1;
}


int SPWindowStorage::OnGridUp(WPARAM wParam, LPARAM lParam)
{
	int iSlotIndex = (int)lParam;
	if(iSlotIndex < 0 || iSlotIndex >= m_iViewSlotCount || iSlotIndex >= MAX_STORAGE_SLOT) {
		//범위 오류
		int iSlotIndex = -1;
		if(g_pGOBManager->GetLocalPlayer()) {
			g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
		}
		return 1;
	}
	
	////
	//if((int)lParam < 0) {
	//	if(g_pGOBManager->GetLocalPlayer())
	//		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
	//}

	if(g_pGOBManager->GetLocalPlayer())	// metalgeni
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_STORAGE_CLICK_UP , (LPARAM)iSlotIndex);

	//DXUTOutputDebugString("SPWindowStorage OnGridUp[%d]\n", iSlotIndex);
	
	return 1;
}


int SPWindowStorage::OnItemSetting(WPARAM wParam, LPARAM lParam)
{
	CONTAINER_ITEM *item; // = new CONTAINER_ITEM;
	item = (CONTAINER_ITEM*)wParam;

	POINT ptSrcPos;	
	SPTexture* texture = g_pItemCluster->GetItemTexture(item->Item.iItemID, ptSrcPos); 	

	//Grid 에 대한 세팅
	SPWindowGrid *pWindow = (SPWindowGrid*)(GetWrapWindow()->GetParent()->Find(WIID_STORAGE_GRID, true));
	//pWindow->SettingIconImage( texture, item->Item.iItemID, ptSrcPos.x, ptSrcPos.y, item->SlotInfo.SlotIndex-1, item->Item.iStackCount );
	if(pWindow)
		pWindow->SettingIconImage( texture, item->Item.iItemID, ptSrcPos.x, ptSrcPos.y, item->SlotInfo.SlotIndex, item->Item.iStackCount, (bool)lParam);
	return 1;
}


int SPWindowStorage::OnItemClear(WPARAM wParam, LPARAM lParam)
{
	int iSlotIndex = (int)lParam;

	SPWindowGrid *pWindow = (SPWindowGrid*)(GetWrapWindow()->GetParent()->Find(WIID_STORAGE_GRID, true));
	if(pWindow)
		pWindow->ClearCell(iSlotIndex);	
	return 1;
}


int SPWindowStorage::OnItemAllClear(WPARAM wParam, LPARAM lParam)
{
	SPWindowGrid *pWindow = (SPWindowGrid*)(GetWrapWindow()->GetParent()->Find(WIID_STORAGE_GRID, true));
	if(pWindow)
		pWindow->ClearCellAll() ;	
	return 1;
}


int SPWindowStorage::OnItemSetStack(WPARAM wParam, LPARAM lParam)
{
	int iIndex = (int)wParam /*- 1*/;
	short iStackCount = (short)lParam;

	SPWindowGrid *pWindow = (SPWindowGrid*)(GetWrapWindow()->GetParent()->Find(WIID_STORAGE_GRID, true));
	if(pWindow)
		pWindow->SetItemStack(iIndex, iStackCount);
	
	return 1;
}


int SPWindowStorage::OnItemSetIconSelect(WPARAM wParam, LPARAM lParam)
{
	int iSlotIndex = (int)wParam;
	bool bSelect = (bool)lParam;

	SPWindowGrid *pWindow = (SPWindowGrid*)(GetWrapWindow()->GetParent()->Find(WIID_STORAGE_GRID, true));		
	
	if(pWindow)
		pWindow->CellSelectSell(iSlotIndex, bSelect);

	return 1;
}


int SPWindowStorage::OnItemDisable(WPARAM wParam, LPARAM lParam)
{
	int iSlotIndex = (int)wParam;
	bool bAble = (bool)lParam;

	SPWindowGrid *pWindow = (SPWindowGrid*)(GetWrapWindow()->GetParent()->Find(WIID_STORAGE_GRID, true));

	if(pWindow)
		pWindow->CellEnable(iSlotIndex, bAble);
	
	return 1;
}


int SPWindowStorage::OnToolTipEnter(WPARAM wParam, LPARAM lParam)
{
	CONTAINER_TYPE iContainerType = STORAGE;
	int iSlotIndex = (int)lParam;

	iSlotIndex += (m_iCurPage * STORAGE_LINE);

	//Pick Up Item과 같은 아이템인 경우
	if(iContainerType == g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetPickupItem().GetContainerType() &&
		iSlotIndex == g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetPickupItem().GetSlotIndex()) 
	{
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
		return 0;
	}

	//
	//DXUTOutputDebugString("SPWindowStorage::OnToolTipEnter Container[%d] SlotIndex[%d]\n", iContainerType, iSlotIndex);

	if(g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetInventoryItem(iContainerType, iSlotIndex) == NULL) {
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
		return 0;
	}

	SPItemStatus* pItemStatus = g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetInventoryItem(iContainerType, iSlotIndex)->GetItemStatus();
	if(pItemStatus->GetItemID() == 0) {
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
		return 0;
	}	

	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage( SPIM_TOOLTIP_ENTER, (WPARAM)pItemStatus, lParam);
	pWindow->Show();

	return 1;
}


int SPWindowStorage::OnToolTipOut(WPARAM wParam, LPARAM lParam)
{
	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
	pWindow->Show();
	
	return 1;
}


int SPWindowStorage::OnToolTipMove(WPARAM wParam, LPARAM lParam)
{
	CONTAINER_TYPE iContainerType = STORAGE;
	int iSlotIndex = (int)lParam;	

	iSlotIndex += (m_iCurPage * STORAGE_LINE);

	//Pick Up Item과 같은 아이템인 경우
	if(iContainerType == g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetPickupItem().GetContainerType() &&
		iSlotIndex == g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetPickupItem().GetSlotIndex()) 
	{
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
		return 0;
	}

	//
	//DXUTOutputDebugString("SPWindowItem::OnToolTipMove Cotainer[%d] SlotIndex[%d]\n", iContainerType, iSlotIndex);

	//SPItemStatus* pItemStatus = g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetGridItemStatusInven(iXPos, iYPos);
	if(g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetInventoryItem(iContainerType, iSlotIndex) == NULL) {
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
		return 0;
	}

	SPItemStatus* pItemStatus = g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetInventoryItem(iContainerType, iSlotIndex)->GetItemStatus();
	if(pItemStatus->GetItemID() == 0) {
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
		return 0;
	}	

	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage( SPIM_TOOLTIP_MOVE, (WPARAM)pItemStatus, lParam);
	pWindow->Show();

	return 1;
}


