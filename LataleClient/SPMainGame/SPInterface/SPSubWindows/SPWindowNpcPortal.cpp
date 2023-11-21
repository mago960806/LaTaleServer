
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
#include "SPWindowSlider.h"
#include "SPWindowNpcPortal.h"

#include "SPDebug.h"



SPWindowNpcPortal::SPWindowNpcPortal( INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent/* = NULL*/)
: SPWindow(WNDID_NPC_PORTAL, InstanceID, iX, iY, iCX, iCY, pParent)	
{
	Init();
}


SPWindowNpcPortal::~SPWindowNpcPortal()
{
	Clean();
}

void SPWindowNpcPortal::Init()
{
	g_pVideo->GetTextureMgr()->LoadTexture("DATA/INTERFACE/CONCEPT/UI105.PNG" , &m_pTextureBase);	

	SetRect(&m_rtSrcBase[WINDOW_BASE_TOP_LEFT],		482, 316, 486, 320);
	SetRect(&m_rtSrcBase[WINDOW_BASE_TOP],			488, 316, 490, 320);	
	SetRect(&m_rtSrcBase[WINDOW_BASE_TOP_RIGHT],	492, 316, 496, 320);
	SetRect(&m_rtSrcBase[WINDOW_BASE_CENTER_LEFT],	482, 322, 486, 324);
	SetRect(&m_rtSrcBase[WINDOW_BASE_CENTER],		488, 322, 490, 324);
	SetRect(&m_rtSrcBase[WINDOW_BASE_CENTER_RIGHT],	492, 322, 496, 324);
	SetRect(&m_rtSrcBase[WINDOW_BASE_BOTTOM_LEFT],	482, 311, 486, 315);
	SetRect(&m_rtSrcBase[WINDOW_BASE_BOTTOM],		488, 311, 490, 315);
	SetRect(&m_rtSrcBase[WINDOW_BASE_BOTTOM_RIGHT],	492, 311, 496, 315);	

	SetRect(&m_rtDestBase[WINDOW_BASE_TOP_LEFT],		m_iAX, m_iAY, m_iAX + 4, m_iAY + 4);
	SetRect(&m_rtDestBase[WINDOW_BASE_TOP],				m_iAX + 4, m_iAY, m_iAX + 4 + 366, m_iAY + 4);	
	SetRect(&m_rtDestBase[WINDOW_BASE_TOP_RIGHT],		m_iAX + 370, m_iAY, m_iAX + 370 + 4, m_iAY + 4);
	SetRect(&m_rtDestBase[WINDOW_BASE_CENTER_LEFT],		m_iAX, m_iAY + 4, m_iAX + 4, m_iAY + 4 + 360);
	SetRect(&m_rtDestBase[WINDOW_BASE_CENTER],			m_iAX + 4, m_iAY + 4, m_iAX + 4 + 366, m_iAY + 4 + 360);
	SetRect(&m_rtDestBase[WINDOW_BASE_CENTER_RIGHT],	m_iAX + 370, m_iAY + 4, m_iAX + 370 + 4, m_iAY + 4 + 360);
	SetRect(&m_rtDestBase[WINDOW_BASE_BOTTOM_LEFT],		m_iAX, m_iAY + 364, m_iAX + 4, m_iAY + 364 + 4);
	SetRect(&m_rtDestBase[WINDOW_BASE_BOTTOM],			m_iAX + 4, m_iAY + 364, m_iAX + 4 + 366, m_iAY + 364 + 4);
	SetRect(&m_rtDestBase[WINDOW_BASE_BOTTOM_RIGHT],	m_iAX + 370, m_iAY + 364, m_iAX + 370 + 4, m_iAY + 364 + 4);

	g_pVideo->GetTextureMgr()->LoadTexture("DATA/INTERFACE/CONCEPT/UI100.PNG", &m_pDisable);	
	SetRect(&m_rtMarkSrc[0], 479, 152, 511, 184);		//ICON_MARK_LOCK
	SetRect(&m_rtMarkSrc[1], 479, 185, 511, 217);		//ICON_MARK_FASHION
	SetRect(&m_rtMarkSrc[2], 479, 339, 511, 371);		//ICON_MARK_EVENT
	SetRect(&m_rtMarkSrc[3], 479, 372, 511, 404);		//ICON_MARK_FANCY
	
	InitSubControl();
}


void SPWindowNpcPortal::InitSubControl()
{
	char* OPTION_IMAGE_FILE0 = "DATA/INTERFACE/CONCEPT/UI100.PNG";
	char* OPTION_IMAGE_FILE1 = "DATA/INTERFACE/CONCEPT/UI101.PNG";
	char* OPTION_IMAGE_FILE2 = "DATA/INTERFACE/CONCEPT/UI102.PNG";
	char* OPTION_IMAGE_FILE4 = "DATA/INTERFACE/CONCEPT/UI104.PNG";
	char* OPTION_IMAGE_FILE5 = "DATA/INTERFACE/CONCEPT/UI105.PNG";
	char* OPTION_IMAGE_FILE6 = "DATA/INTERFACE/CONCEPT/UI106.PNG";
	
	//// Control
	SPWindowButton* pButton = NULL;
	SPWindowStatic* pStatic = NULL;
	SPWindowStatic* pParent = NULL;
	SPWindowStatic* pSkin	= NULL;
	SPWindowSlider* pSlider = NULL;
	
	//WIID_NPC_PORTAL_TITLE	
	pStatic = new SPWindowStatic( WIID_NPC_PORTAL_TITLE,	10,	5,	114,	20, this);	
	pStatic->SetImage(OPTION_IMAGE_FILE5, 207, 137);
	pStatic->Show();
	pStatic = NULL;

	//WIID_NPC_PORTAL_EXIT	
	pButton = new SPWindowButton( WIID_NPC_PORTAL_EXIT , 353, 9, 12, 12, this);
	pButton->SetImage		(OPTION_IMAGE_FILE2, 247,  217);		
	pButton->SetImageHit	(OPTION_IMAGE_FILE2, 247,  230);
	pButton->SetImagePush	(OPTION_IMAGE_FILE2, 247,  243);
	pButton->SetImageDisable(OPTION_IMAGE_FILE2, 247,  256);
	pButton->Show();
	
	//WIID_NPC_PORTAL_LINE	
	pStatic = new SPWindowStatic( WIID_NPC_PORTAL_LINE,	2,	26,	370,	1, this);		
	pStatic->SetImage(OPTION_IMAGE_FILE5, 487, 387);
	pStatic->SetSrcSize(2, 1);
	pStatic->Show();
	pStatic = NULL;

	//WIID_NPC_PORTAL_BASE	
	pSkin = new SPWindowStatic( WIID_NPC_PORTAL_BASE,	2,	27,	370,	337, this);	
	pSkin->SetImage(OPTION_IMAGE_FILE5, 488, 307);
	pSkin->SetSrcSize(2, 2);
	pSkin->Show();


	int iStartY = 40 - 27;
	for(int i = 0; i < PORTAL_PAGE; i++) {		
		//WIID_NPC_PORTAL_LIST_SKIN	
		pParent = new SPWindowStatic( WIID_NPC_PORTAL_LIST_SKIN + i,	2 - 2,	iStartY + (i * 36),	350,	36, pSkin);	
		//pParent->SetImage(OPTION_IMAGE_FILE5, 207, 137);
		pParent->Show();
		
		//WIID_NPC_PORTAL_LIST_BUTTON
		m_pButton[i] = new SPWindowButton(WIID_NPC_PORTAL_LIST_BUTTON + i, 0, 0, 350, 36, pParent);
		m_pButton[i]->Show();

		//WIID_NPC_PORTAL_LIST_SELECT
		m_pSelect[i] = new SPWindowStatic(WIID_NPC_PORTAL_LIST_SELECT + i, 0, 0, 350, 36, pParent);
		m_pSelect[i]->SetImage(OPTION_IMAGE_FILE5, 505, 152);
		m_pSelect[i]->SetSrcSize(2, 188 - 152);
		m_pSelect[i]->Show();

		//WIID_NPC_PORTAL_LIST_ICON	
		m_pDestIcon[i] = new SPWindowStatic(WIID_NPC_PORTAL_LIST_ICON + i, 16, 2, 32, 32, pParent);
		m_pDestIcon[i]->Show();
		
		//WIID_NPC_PORTAL_LIST_NAME	
		m_pName[i] = new SPWindowStatic(WIID_NPC_PORTAL_LIST_NAME + i, 55, 12, 157, 12, pParent);
		m_pName[i]->SetFormat(DT_LEFT | DT_VCENTER);
		m_pName[i]->Show();

		//WIID_NPC_PORTAL_LIST_ELY	
		m_pEly[i] = new SPWindowStatic(WIID_NPC_PORTAL_LIST_ELY + i, 219, 12, 51, 12, pParent);
		m_pEly[i]->SetFormat(DT_RIGHT | DT_VCENTER);
		m_pEly[i]->Show();
		
		//WIID_NPC_PORTAL_LIST_ELY_IMAGE
		m_pElyImage[i] = new SPWindowStatic(WIID_NPC_PORTAL_LIST_ELY_IMAGE + i, 273, 17, 17, 5, pParent);
		m_pElyImage[i]->SetImage(OPTION_IMAGE_FILE5, 208, 296);
		m_pElyImage[i]->Show();
		
		//WIID_NPC_PORTAL_LIST_ITEM_SKIN
		m_pItemSkin[i] = new SPWindowStatic(WIID_NPC_PORTAL_LIST_ITEM_SKIN + i, 298, 23, 36, 11, pParent);
		m_pItemSkin[i]->SetImage(OPTION_IMAGE_FILE5, 445, 337);
		m_pItemSkin[i]->Show();

		//WIID_NPC_PORTAL_LIST_ITEM	
		m_pItemIcon[i] = new SPWindowStatic(WIID_NPC_PORTAL_LIST_ITEM + i, 300, 1, 32, 32, pParent);
		m_pItemIcon[i]->Show();
	}

	//WIID_NPC_PORTAL_UP
	pButton = new SPWindowButton( WIID_NPC_PORTAL_UP , 352 - 2, 40 - 27, 12, 11, pSkin);
	pButton->SetImage		(OPTION_IMAGE_FILE2, 460,	458);
	pButton->SetImageHit	(OPTION_IMAGE_FILE2, 473,	458);	
	pButton->SetImagePush	(OPTION_IMAGE_FILE2, 486,	458);
	pButton->SetImageDisable(OPTION_IMAGE_FILE2, 499,	458);
	pButton->Show();
	
	//WIID_NPC_PORTAL_DOWN	
	pButton = new SPWindowButton( WIID_NPC_PORTAL_DOWN , 352 - 2, 322 - 27, 12, 11, pSkin);
	pButton->SetImage		(OPTION_IMAGE_FILE2, 460,	500);
	pButton->SetImageHit	(OPTION_IMAGE_FILE2, 473,	500);	
	pButton->SetImagePush	(OPTION_IMAGE_FILE2, 486,	500);
	pButton->SetImageDisable(OPTION_IMAGE_FILE2, 499,	500);
	pButton->Show();


	m_pSlider	=	new SPWindowSlider(WIID_NPC_PORTAL_SLIDER, 352 - 2, 51 - 27, 12, 271, pSkin);
	RECT rtRect;

	SetRect( &rtRect , 460 , 470 , 472 , 494 );	//	일반
	m_pSlider->SetImageHandle( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	SetRect( &rtRect , 473 , 470 , 485 , 494 );	//	마퓖E?오퉩E
	m_pSlider->SetImageHandleHit( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	SetRect( &rtRect , 486 , 470 , 498 , 494 );	//	마퓖E?클릭
	m_pSlider->SetImageHandlePush( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	SetRect( &rtRect , 499 , 470 , 511 , 494 );
	m_pSlider->SetImageHandleDisable( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	SetRect( &rtRect , 460 , 496 , 472 , 498 );	//	일반
	m_pSlider->SetImagePageNormal( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	SetRect( &rtRect , 473 , 496 , 485 , 498 );	//	마퓖E?오퉩E
	m_pSlider->SetImagePageHit( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	SetRect( &rtRect , 486 , 496 , 498 , 498 );	//	마퓖E?클릭
	m_pSlider->SetImagePageShadow( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	SetRect( &rtRect , 499 , 496 , 511 , 498 );
	m_pSlider->SetImagePageDisable( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );


	m_pSlider->SPSendMessage( SPIM_SLIDER_PAGESIZE , 1 );
	m_pSlider->SetCurpage( 0 );
	m_pSlider->SPSendMessage( SPIM_SET_MAXVALUE , 0 );

	//WIID_NPC_PORTAL_MOVE
	pButton = new SPWindowButton( WIID_NPC_PORTAL_MOVE , 276 - 2, 341 - 27, 76, 17, pSkin);
	pButton->SetImage		(OPTION_IMAGE_FILE6, 386,	440);
	pButton->SetImageHit	(OPTION_IMAGE_FILE6, 386,	458);	
	pButton->SetImagePush	(OPTION_IMAGE_FILE6, 386,	476);
	pButton->SetImageDisable(OPTION_IMAGE_FILE6, 386,	494);
	pButton->Show();
}


void SPWindowNpcPortal::Clean()
{
	ResetControl();	
	SAFE_RELEASE(m_pDisable);
	SAFE_RELEASE(m_pTextureBase);
	SPWindow::Clean();
}


void SPWindowNpcPortal::Process(float fTime)
{
	SPWindow::Process(fTime);
}


void SPWindowNpcPortal::Render(float fTime)
{
	if(!m_bShow)		return;

	if(m_pTextureBase) {
		for(int i = 0; i < WINDOW_BASE_MAX; i++) {
			m_pTextureBase->RenderTexture(&m_rtDestBase[i], &m_rtSrcBase[i]);
		}
	}	

	SPWindow::Render(fTime);

	for(int i = 0; i < PORTAL_PAGE; i++) {
		if(m_pDisable) {
			RECT rtRect;
			m_pItemIcon[i]->GetWindowRect(rtRect);

			//if(m_bAble[i] == false) {				//아이콘 위에 살짝 비활성화				
			//	m_pDisable->RenderTexture(&rtRect, &m_rtSrcDisable);
			//}

			if(m_iMark[i] > ICON_MARK_NULL) {				
				m_pDisable->RenderTexture(&rtRect, &m_rtMarkSrc[ (m_iMark[i] - 1) ]);
			}
		}
	}

////
//#ifdef _DEBUG
//	RECT rtRect = {0, 0, 0, 0};
//	for(int i = 0; i < PORTAL_PAGE; i++) {
//		m_pItemIcon[i]->GetWindowRect(rtRect);
//		g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
//		g_pVideo->GetSysLine()->DrawRect(rtRect);
//		g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
//	}
//#endif
}


void SPWindowNpcPortal::Show(bool bWithChild /*= true*/)
{
	SPNpcPortal* pPortal  = g_pEventManager->GetEventHandler()->GetNpcPortal();
	if(pPortal == NULL)
		return ;	
	
	ResetControl();
	
	SPWindow::Show(bWithChild);	
	
	m_iMaxPage = 0;
	m_iCurPage = 0;
	m_iSelectIndex = 0;
	m_iSelect = 0;

	if(pPortal->GetSize() < PORTAL_PAGE) {
		m_iMaxPage = 0;
	}
	else {
		m_iMaxPage = pPortal->GetSize() - PORTAL_PAGE;
	}
	
	m_pSlider->SetCurpage(m_iCurPage);
	m_pSlider->SPSendMessage(SPIM_SET_MAXVALUE , m_iMaxPage + 1);	
	
	UpdateControl();
}


void SPWindowNpcPortal::Hide(bool bSendServer /*= true*/)
{
	SPWindow::Hide(bSendServer);
}


void SPWindowNpcPortal::ResetControl()
{
	for(int i = 0; i < PORTAL_PAGE; i++) {
		m_pDestIcon[i]->Hide();
		m_pDestIcon[i]->SPSendMessage(SPIM_SET_IMAGECLEAR);
		m_pDestIcon[i]->Show();

		m_pName[i]->SetWindowText(NULL);
		m_pEly[i]->SetWindowText(NULL);

		m_pItemIcon[i]->Hide();
		m_pItemIcon[i]->SPSendMessage(SPIM_SET_IMAGECLEAR);
		m_pItemIcon[i]->Show();		

		m_pSelect[i]->Hide();

		m_iMark[i] = ICON_MARK_NULL;
	}
}


void SPWindowNpcPortal::ClearControl(int iIndex)
{
	if(iIndex < 0 || iIndex > PORTAL_PAGE)
		return;

	m_pDestIcon[iIndex]->Hide();
	m_pDestIcon[iIndex]->SPSendMessage(SPIM_SET_IMAGECLEAR);
	m_pDestIcon[iIndex]->Show();

	m_pName[iIndex]->SetWindowText(NULL);
	m_pEly[iIndex]->SetWindowText(NULL);

	m_pItemIcon[iIndex]->Hide();
	m_pItemIcon[iIndex]->SPSendMessage(SPIM_SET_IMAGECLEAR);
	m_pItemIcon[iIndex]->Show();
	
	m_pSelect[iIndex]->Hide();
	m_iMark[iIndex] = ICON_MARK_NULL;
}


void SPWindowNpcPortal::UpdateControl()
{	
	SPNpcPortal* pPortal  = g_pEventManager->GetEventHandler()->GetNpcPortal();
	if(pPortal == NULL)
		return ;	
	
	SPNpcPortalInfo* pInfo = NULL;
	
	m_iSelectIndex = m_iCurPage + m_iSelect;
	
	int iIndex = 0;
	char szTemp[128];
	for(int i = 0; i < PORTAL_PAGE; i++) {		
		ClearControl(i);
		iIndex = m_iCurPage + i;		
		pInfo = pPortal->GetInfo(iIndex);
		
		if(pInfo == NULL) {
			m_pElyImage[i]->Hide();
			m_pItemSkin[i]->Hide();
			continue;
		}
		else {
			m_pElyImage[i]->Show();
			m_pItemSkin[i]->Show();
		}

		if(i == m_iSelect) {
			m_pSelect[i]->Show();
		}
		
		SetDestIcon(pInfo, m_pDestIcon[i]);
		
		ZeroMemory(szTemp, 128);
		sprintf(szTemp, "%d. %s", iIndex + 1, pInfo->m_strName.c_str());
		m_pName[i]->SetWindowText(szTemp);
		//m_pName[i]->SetWindowText(pInfo->m_strName.c_str());			

		m_pEly[i]->SetWindowText(ConvertMoneyToString(pInfo->m_iRequireEly));

		if(pInfo->m_iRequireItem) {
			POINT ptSrcPos;
			SPTexture* pkTexture = g_pItemCluster->GetItemTexture(pInfo->m_iRequireItem, ptSrcPos);
			m_pItemIcon[i]->Hide();
			m_pItemIcon[i]->SetImage(pkTexture, ptSrcPos.x, ptSrcPos.y);
			m_pItemIcon[i]->Show();
			m_iMark[i] = g_pItemCluster->GetItemIconMark(pInfo->m_iRequireItem);
		}
	}

	//int iSelect = m_iSelectIndex - m_iCurPage;
	//m_pSelect[m_iSelect]->Show();
}


void SPWindowNpcPortal::SetDestIcon(SPNpcPortalInfo* pInfo, SPWindow* pWindow)
{
	if(pInfo == NULL)		return;

	int iResID = g_StageManager.GetMapGroup(pInfo->m_iMapGroup, pInfo->m_iStage)->m_iMiniMapResID;
	int iResIndex = g_StageManager.GetMapGroup(pInfo->m_iMapGroup, pInfo->m_iStage)->m_iMiniMapIconID;

	if( iResID <= 0 || iResIndex <= 0 )
		return;

	const char*	szFileName = g_pResourceManager->GetGlobalFileName( iResID );

	if( szFileName == NULL )
		return;

	std::string strResourceFile = szFileName;

	if( strResourceFile.empty() )
		return;

	SPTexture*	pThemeIcon	=	NULL;
	std::string strFullRes; //	=	RES_WORK_RESOURCE_PATH;
	strFullRes	=	strResourceFile;

	g_pVideo->GetTextureMgr()->LoadTexture( strFullRes.c_str() , &pThemeIcon );

	if( pThemeIcon == NULL )
		return;

	//	시작은 0이 아닌 1부터 시작하기에 1을 빼준다!!!
	iResIndex--;

	int iIconSizeX = 32;
	int	iIconSizeY = 32;

	int iIconWidth	= pThemeIcon->GetLenX() / iIconSizeX;
	int iIconYIndex = iResIndex / iIconWidth;
	int iIconXIndex = iResIndex - iIconYIndex * iIconWidth;

	if( pWindow ) {
		pWindow->Hide();
		pWindow->SetImage( strFullRes.c_str() , iIconXIndex * iIconSizeX , iIconYIndex * iIconSizeY );
		pWindow->Show();
	}
}




SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowNpcPortal )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_NPC_PORTAL_EXIT	, OnExit)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_NPC_PORTAL_MOVE	, OnMove)

SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_NPC_PORTAL_UP		, OnListUp)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_NPC_PORTAL_DOWN	, OnListDown)

SPIMESSAGE_CONTROL( SPIM_SLIDER_UP		,	WIID_NPC_PORTAL_SLIDER,		OnSliderPageUp	)
SPIMESSAGE_CONTROL( SPIM_SLIDER_DOWN	,	WIID_NPC_PORTAL_SLIDER,		OnSliderPageDown	)
SPIMESSAGE_CONTROL( SPIM_SLIDER_PAGEUP,		WIID_NPC_PORTAL_SLIDER,		OnSliderPageUp	)
SPIMESSAGE_CONTROL( SPIM_SLIDER_PAGEDOWN,	WIID_NPC_PORTAL_SLIDER,		OnSliderPageDown)
SPIMESSAGE_CONTROL( SPIM_SET_CURVALUE,		WIID_NPC_PORTAL_SLIDER,		OnSliderPageNum	)

SPIMESSAGE_CONTROL_RANGE( SPIM_WHEEL_UP,	WIID_NPC_PORTAL_LIST_BUTTON,		WIID_NPC_PORTAL_LIST_BUTTON + PORTAL_PAGE,	OnPageUp)
SPIMESSAGE_CONTROL_RANGE( SPIM_WHEEL_DOWN,	WIID_NPC_PORTAL_LIST_BUTTON,		WIID_NPC_PORTAL_LIST_BUTTON + PORTAL_PAGE,	OnPageDown)

SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP, WIID_NPC_PORTAL_LIST_BUTTON,		WIID_NPC_PORTAL_LIST_BUTTON + PORTAL_PAGE,	OnPortalClick)
SPIMESSAGE_OBJ_MAP_END	( SPWindow	)



int SPWindowNpcPortal::OnExit(WPARAM wParam, LPARAM lParam)
{
	g_pEventManager->SetCutInState(CUT_IN_STATE_RELEASE);
	return 1;
}


int SPWindowNpcPortal::OnMove(WPARAM wParam, LPARAM lParam)
{
	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return 1;
	
	SPNpcPortal* pPortal  = g_pEventManager->GetEventHandler()->GetNpcPortal();
	if(pPortal == NULL)
		return 1;

	SPNpcPortalInfo* pInfo = pPortal->GetInfo(m_iSelectIndex);
	if(pInfo == NULL)
		return 1;
	
	//제한 조건 검사
	for(int i = 0; i < MAX_NPC_PORTAL_REQUIRE; i++) {
		if(g_pCheckManager->CheckCondition(pInfo->m_Require[i].m_iRequireStat, pInfo->m_Require[i].m_iRequireStatValue,
			pInfo->m_Require[i].m_iValue1, pInfo->m_Require[i].m_iValue2) == false) 
		{
			if(!pInfo->m_strErrMsg.empty())
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)pInfo->m_strErrMsg.c_str());
			return 1;
		}
	}

	//Item 검사	
	CONTAINER_SLOT stUsingItem;
	INT64 iUsingItemNo = 0;
	ZeroMemory(&stUsingItem, sizeof(CONTAINER_SLOT));
	stUsingItem.ContainerType = CONTAINER_TYPE_INVALID;
	stUsingItem.SlotIndex = 0;
	if(pInfo->m_iRequireItem) {
		SPPlayerInvenArchive* pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
		if(pInven == NULL)
			return 1;

		SPItemAttr* pItemAttr = NULL;
		g_pItemCluster->GetItemInfo(pInfo->m_iRequireItem, pItemAttr);
		CONTAINER_TYPE iContainer = (CONTAINER_TYPE)(pItemAttr->m_eType - 1);		
		SPItem* pItem = pInven->GetSPItem(iContainer, pInfo->m_iRequireItem);
		
		//아이템 확인
		if(pItem == NULL || pItem->IsEmptyItem()) {
			if(!pInfo->m_strErrMsg.empty())
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)pInfo->m_strErrMsg.c_str());
			return 1;
		}
		
		////시간 제한 확인 -> Inven의 GetSPItem쪽에서 걸러내도록 수정해야 할듯.
		//if(pItem->GetItemStatus()->IsTimeAvailable() == false) {
		//	if(!pInfo->m_strErrMsg.empty())
		//		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)pInfo->m_strErrMsg.c_str());
		//	return 1;
		//}

		iUsingItemNo = *(ITEMNO*)pItem->GetItemStatus()->SPSendMessage( MV_ITEM_GET_ITEMNO );
		
		stUsingItem.ContainerType = pItem->GetContainerType();
		stUsingItem.SlotIndex = pItem->GetSlotIndex() + 1;
		if(stUsingItem.SlotIndex < 1)
			return 1;
	}
	
	//Ely 검사	
	UINT64 iUserEly = g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValueEX(STATUS_EX_ELY);
	if(iUserEly < pInfo->m_iRequireEly) {
		if(!pInfo->m_strErrMsg.empty())
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)pInfo->m_strErrMsg.c_str());
		return 1;
	}
	
	// int            iNpcID;			// 엔피시
	// int			  iPortalGroupID;	// 포탈 그룹의 Key ID NPCPORTAL.LDT::_ItemID
	// int			  iPotalIndex;		// 위치정보
	// int			  iPortalInfoID;    // PORTAL_DETAIL.LDT::_ItemID
	// CONTAINER_SLOT stUsingItemPos;	// 소모아이템
	// ITEMNO		  iUsingItemNo;		// 소모아이템 No
	// int			  iUsingEly;		// 소모돈

	CPacket Packet;
	Packet.Add((UINT32)NPC_CS_PORTAL);	
	Packet.Add((UINT32)pPortal->m_iNpcID);
	Packet.Add((UINT32)pPortal->m_iItemID);
	Packet.Add((UINT32)m_iSelectIndex);
	Packet.Add((UINT32)pInfo->m_iItemID);
	Packet.AddData(&stUsingItem, sizeof(CONTAINER_SLOT));
	Packet.AddUINT64(iUsingItemNo);
	Packet.Add((UINT32)pInfo->m_iRequireEly);

	//이부분 리턴값 문제 있을수 있음...
	g_pNetworkManager->SPPerformMessage(NPC_CS_PORTAL, 0, (LPARAM)&Packet);
	
	return 1;
}


int SPWindowNpcPortal::OnListUp(WPARAM wParam, LPARAM lParam)
{
	m_pSlider->SPSendMessage( SPIM_SLIDER_UP );
	return 1;
}


int SPWindowNpcPortal::OnListDown(WPARAM wParam, LPARAM lParam)
{
	m_pSlider->SPSendMessage( SPIM_SLIDER_DOWN );
	return 1;
}


int SPWindowNpcPortal::OnPageUp(unsigned int uiID, WPARAM wParam, LPARAM lParam)
{
	m_pSlider->SPSendMessage( SPIM_SLIDER_UP );
	return 1;
}


int SPWindowNpcPortal::OnPageDown(unsigned int uiID, WPARAM wParam, LPARAM lParam)
{
	m_pSlider->SPSendMessage( SPIM_SLIDER_DOWN );
	return 1;
}


int SPWindowNpcPortal::OnSliderPageUp(WPARAM wParam, LPARAM lParam)
{
	if(m_iMaxPage < 1) {
		m_iSelect--;
		if(m_iSelect < 0) {
			m_iSelect = 0;
		}

		UpdateControl();
		return 1;
	}
	
	m_iCurPage--;
	if(m_iCurPage < 0) {
		m_iCurPage = 0;
	}

	if(m_iCurPage == 0) {
		m_iSelect--;
		if(m_iSelect < 0)
			m_iSelect = 0;
	}

	UpdateControl();
	return 1;
}


int SPWindowNpcPortal::OnSliderPageDown(WPARAM wParam, LPARAM lParam)
{
	if(m_iMaxPage < 1) {
		SPNpcPortal* pPortal  = g_pEventManager->GetEventHandler()->GetNpcPortal();
		if(pPortal == NULL)
			return 1;

		m_iSelect++;
		if(m_iSelect >= pPortal->GetSize()) {
			int iSize = m_iSelect - pPortal->GetSize();
			m_iSelect  -= (iSize + 1);
		}
		
		UpdateControl();
		return 1;
	}
	
	m_iCurPage++;
	if(m_iCurPage == m_iMaxPage) {
		m_iCurPage = m_iMaxPage;
		UpdateControl();
		return 1;
	}

	if(m_iCurPage >= m_iMaxPage) {
		m_iCurPage = m_iMaxPage;
		SPNpcPortal* pPortal  = g_pEventManager->GetEventHandler()->GetNpcPortal();
		if(pPortal == NULL)
			return 1;
		
		m_iSelect++;
		if(m_iSelect >= PORTAL_PAGE) {
			m_iSelect = PORTAL_PAGE - 1;
		}
	}
	UpdateControl();
	return 1;
}


int SPWindowNpcPortal::OnSliderPageNum(WPARAM wParam, LPARAM lParam)
{
	if(m_iMaxPage < 1)
		return 1;
	
	return 1;
}


int SPWindowNpcPortal::OnPortalClick( unsigned int uiID, WPARAM wParam, LPARAM lParam )
{
	m_iSelect = uiID - WIID_NPC_PORTAL_LIST_BUTTON;	
	DXUTOutputDebugString("Portal Select %d\n", m_iSelect);
	
	
	SPNpcPortal* pPortal  = g_pEventManager->GetEventHandler()->GetNpcPortal();
	if(pPortal == NULL)
		return 1;
	
	m_iSelectIndex = m_iCurPage + m_iSelect;

	if(m_iSelectIndex < 0)
		return 1;

	if(pPortal->GetSize() <= m_iSelectIndex) {
		int iSize = m_iSelectIndex - pPortal->GetSize();		
		m_iSelectIndex -= (iSize + 1);
		m_iSelect  -= (iSize + 1);
	}

	UpdateControl();

	return 1;
}






