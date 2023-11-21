
#include "SPCommon.h"

#include <algorithm>
using namespace std;

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

#include "SPUIUnit.h"
#include "SPUISkillUnit.h"
#include "SPUIUnitManager.h"
#include "SPDragnDropManager.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPWindowEdit.h"
#include "SPWindowSlider.h"
#include "SPWindowGrid.h"
#include "SPWindowGauge.h"
#include "SPWindowToolTip.h"
#include "SPWindowPet.h"
#include "SPWindowPetShop.h"
#include "SPPlayerInvenArchive.h"
#include "SPWindowUserShopSeller.h"
#include "SPWindowPetShopSeller.h"

#include "SPCommandConvert.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGOBCoordPhysics.h"
#include "SPPlayerCoordPhysics.h"
#include "SPPlayerInvenArchive.h"

#include "SPGOBStatus.h"
#include "SPPlayerStatus.h"
#include "SPPlayerStatusModel.h"

#include "SPSkill.h"
#include "SPSkillArchive.h"
#include "SPSkillManager.h"

#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPItem.h"
#include "SPItemCluster.h"			//[2005/6/16] - jinhee
#include "SPPlayer.h"

#include "SPCheckManager.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"
#include "SPAbuseFilter.h"

#include "SPUtil.h"

#include "SPPet.h"

#include "SPDebug.h"
#include "SPPetLevelMgr.h"
#include "SPPetManager.h"
#include "SPWindowPetChat.h"


struct _IsEqualPet : binary_function<SPPetTalk* , int, bool> {
	bool operator() (SPPetTalk* pPet, int iItemID) const
	{
		if(pPet->m_iPetID == iItemID)
			return true;
		return false;
	}
} IsEqualPet;


SPWindowPet::SPWindowPet(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent /*= NULL*/)
: SPWindow(WNDID_PET, InstanceID, iX, iY, iCX, iCY, pParent)	
//, m_pPetItem(NULL)
//, m_pPetFeed(NULL)
, m_pGrid(NULL)
, m_pName(NULL)
, m_pXP(NULL)
, m_pSP(NULL)
, m_pXPInfo(NULL)
, m_pSPInfo(NULL)
, m_pCurPetTalk(NULL)
, m_pkPet(NULL)
, m_pPetShop(NULL)
, m_pPetFeedIcon(NULL)
, m_iCurPetSelect( PET_PRIMARY )
, m_pBtnPrimary( NULL )
, m_pBtnSecondary( NULL )
, m_pBtnTertiary( NULL )
, m_pBtnNameEditor( NULL )
, m_pNameEditor( NULL )
{
	Init();
}


SPWindowPet::~SPWindowPet()
{
	Clean();
}


void SPWindowPet::Init()
{
	ZeroMemory(&m_szXPInfo, 16);
	ZeroMemory(&m_szSPInfo, 16);

	LoadPetTalkLDT();

	std::string strTextureFile = "";
	strTextureFile = "DATA/INTERFACE/CONCEPT/UI100.PNG";
	g_pVideo->GetTextureMgr()->LoadTexture(strTextureFile.c_str(), &m_pDisable);

	SetRect(&m_rtSrcDisable, 434 , 37 , 436 , 39);
	SetRect(&m_rtMarkSrc[0], 479, 152, 511, 184);		//ICON_MARK_LOCK
	SetRect(&m_rtMarkSrc[1], 479, 185, 511, 217);		//ICON_MARK_FASHION
	SetRect(&m_rtMarkSrc[2], 479, 339, 511, 371);		//ICON_MARK_EVENT
	SetRect(&m_rtMarkSrc[3], 479, 372, 511, 404);		//ICON_MARK_FANCY
	
	InitSubControl();
	InitSkillUI();
	LoadCashFoodCfg();
	for (int idx = 0; idx < PET_PLACE_TYPE_NUMBER; ++idx)
	{
		m_petInfo[idx].reset();
	}

	m_iModelPosX = 182;
	m_iModelPosY = 270+32;
}


void SPWindowPet::Clean()
{
	ClearAllPetTalk();
	ReleasePet();

	m_pPetFeedIcon->SPSendMessage(SPIM_SET_IMAGECLEAR);
	m_pPetFeedIcon2->SPSendMessage(SPIM_SET_IMAGECLEAR);
	SAFE_RELEASE(m_pDisable);

	SPWindow::Clean();

	m_UIUnitManager.ClearAll( TRUE );
	for (int idx = 0; idx < PET_PLACE_TYPE_NUMBER; ++idx)
	{
		m_petInfo[idx].reset();
	}
}


void SPWindowPet::Process(float fTime)
{
	if(m_bShow == false)
		return;

	SPWindow::Process(fTime);

	m_UIUnitManager.Process( fTime );

	if( m_pkPet )
	{
		m_pkPet->SetJustRenderPos(m_iAX + m_iModelPosX, m_iAY + m_iModelPosY);
		m_pkPet->Process(fTime);
	}
}


void SPWindowPet::Render(float fTime)
{
	if(m_bShow == false)
		return;

	SPWindow::Render(fTime);

	SPItem	*&pPetFeed = m_petInfo[m_iCurPetSelect].m_pPetFeed;
	SPItem	*&pPetFeedEnergy = m_petInfo[m_iCurPetSelect].m_pPetFeedEnergy;
	//Icon Disable 표현
	if(pPetFeed && pPetFeed->GetItemStatus() && pPetFeed->GetItemStatus()->GetItemID() != 0) {
		if(m_pDisable) {
			RECT rtDest;
			m_pPetFeedIcon->GetWindowRect(rtDest);
			if(m_bItemAble == false) {		
				m_pDisable->RenderTexture(&rtDest, &m_rtSrcDisable);
			}
			if(m_iItemMark > ICON_MARK_NULL) {
				m_pDisable->RenderTexture(&rtDest, &m_rtMarkSrc[m_iItemMark - 1]);
			}			
		}

		//Stack Count Local 표현
		if(m_iStack > 9) {
			RECT rtTenNumTarget, rtNumTarget, rtTenNumSrc, rtNumSrc;
			int iTenStack = m_iStack / 10;
			int iOneStack = m_iStack % 10;
			m_pPetFeedIcon->GetWindowRect(m_rtStack);
			SetRect(&rtTenNumTarget, m_rtStack.left, m_rtStack.bottom - 10, m_rtStack.left + 9, m_rtStack.bottom);
			SetRect(&rtNumTarget, rtTenNumTarget.right - 2, m_rtStack.bottom - 10, rtTenNumTarget.right + 9 - 2, m_rtStack.bottom);

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
		else if(m_iStack > 1) {
			int iOneStack = m_iStack % 10;
			RECT rtNumTarget, rtNumSrc;
			m_pPetFeedIcon->GetWindowRect(m_rtStack);
			SetRect(&rtNumTarget, m_rtStack.left, m_rtStack.bottom - 10, m_rtStack.left + 9, m_rtStack.bottom);

			rtNumSrc.left = iOneStack * 9;
			rtNumSrc.right = rtNumSrc.left + 9;
			rtNumSrc.top = 0;
			rtNumSrc.bottom = 10;

			g_pItemCluster->GetNumTexture()->RenderTexture( &rtNumTarget , &rtNumSrc);
		}	
	}	

	if(pPetFeedEnergy && pPetFeedEnergy->GetItemStatus() && pPetFeedEnergy->GetItemStatus()->GetItemID() != 0) {
		if(m_pDisable) {
			RECT rtDest;
			m_pPetFeedIcon2->GetWindowRect(rtDest);
			if(m_bItemAble_energy == false) {		
				m_pDisable->RenderTexture(&rtDest, &m_rtSrcDisable);
			}
			if(m_iItemMark_energy > ICON_MARK_NULL) {
				m_pDisable->RenderTexture(&rtDest, &m_rtMarkSrc[m_iItemMark_energy - 1]);
			}			
		}

		//Stack Count Local 표현
		if(m_iStack_energy > 9) {
			RECT rtTenNumTarget, rtNumTarget, rtTenNumSrc, rtNumSrc;
			int iTenStack = m_iStack_energy / 10;
			int iOneStack = m_iStack_energy % 10;
			m_pPetFeedIcon2->GetWindowRect(m_rtStack_energy);
			SetRect(&rtTenNumTarget, m_rtStack_energy.left, m_rtStack_energy.bottom - 10, m_rtStack_energy.left + 9, m_rtStack_energy.bottom);
			SetRect(&rtNumTarget, rtTenNumTarget.right - 2, m_rtStack_energy.bottom - 10, rtTenNumTarget.right + 9 - 2, m_rtStack_energy.bottom);

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
		else if(m_iStack_energy > 1) {
			int iOneStack = m_iStack_energy % 10;
			RECT rtNumTarget, rtNumSrc;
			m_pPetFeedIcon2->GetWindowRect(m_rtStack_energy);
			SetRect(&rtNumTarget, m_rtStack_energy.left, m_rtStack_energy.bottom - 10, m_rtStack_energy.left + 9, m_rtStack_energy.bottom);

			rtNumSrc.left = iOneStack * 9;
			rtNumSrc.right = rtNumSrc.left + 9;
			rtNumSrc.top = 0;
			rtNumSrc.bottom = 10;

			g_pItemCluster->GetNumTexture()->RenderTexture( &rtNumTarget , &rtNumSrc);
		}	
	}	
	
	if(m_pGrid)
		m_pGrid->Render(fTime);

	g_pVideo->Flush();

	m_UIUnitManager.Render();

	if( m_pkPet )
		m_pkPet->Render(fTime);

	g_pVideo->Flush();

//#ifdef _DEBUG
//	RECT rtRect;
//	m_pGrid->GetWindowRect(rtRect);
//	g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
//	g_pVideo->GetSysLine()->DrawRect(rtRect);
//	g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
//#endif
#ifdef _DEBUG
	RECT rtRect = {0, 0, 0, 0};	
	m_pPetMsg->GetWindowRect(rtRect);
	g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	g_pVideo->GetSysLine()->DrawRect(rtRect);
	g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
#endif
}


void SPWindowPet::Show(bool bWithChild /*= true*/)
{
	OnPetSelectClick( WIID_PET_PRIMARY+m_iCurPetSelect, NULL, NULL );
	SPWindow::Show(bWithChild);
	m_pPetShop->Hide();
	m_pNameEditor->Hide();
	OnStatUpdate(PET_STAT_UPDATE_SP, m_iCurPetSelect);
	UpdatePosSkillUI();

	// Show되었을때 마우스 포지션이 스킬 아이콘 위라면 바로 툴팁을 표시한다. [11/24/2006 AJJIYA]
	int	iCursorX	=	g_pInterfaceManager->GetCursorX();
	int	iCursorY	=	g_pInterfaceManager->GetCursorY();

	OnSkillCursorEnter( 0 , NULL , MAKELONG(iCursorX, iCursorY) );
}

void SPWindowPet::Hide(bool bSendServer /*= true*/)
{
	SPWindow::Hide(bSendServer);

	// Hide되었을때 마우스 포지션이 현재 윈도우 안이라면 툴팁을 제거한다. [11/24/2006 AJJIYA]
	int	iCursorX	=	g_pInterfaceManager->GetCursorX();
	int	iCursorY	=	g_pInterfaceManager->GetCursorY();

	if( IsCursorIn( iCursorX , iCursorY ) == TRUE )
        OnSKillCursorOut( 0 , NULL , MAKELONG(iCursorX, iCursorY) );

	m_bLMouseDown	=	FALSE;
	m_lParam		=	0;
}


void SPWindowPet::RefreshRelationCoord()
{
	SPWindow::RefreshRelationCoord();
	UpdatePosSkillUI();
}


void SPWindowPet::InitSubControl()
{
	SPWindow* pStatic;
	//SPWindow* pChild;

	SPWindow* pCenter = NULL;
	SPWindow* pSkin1 = NULL;
	SPWindow* pSkin2 = NULL;
	SPWindowButton* pButton;

	pStatic = new SPWindowStatic(WIID_PET_TOP, 0, 0, 252, 4, this);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 1, 46);
	pStatic->Show();
	pStatic = NULL;

	pCenter = new SPWindowStatic(WIID_PET_CENTER, 0, 4, 252, 296+32, this);
	pCenter->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 1, 52);
	pCenter->SetSrcSize(252, 2);
	pCenter->Show();

	pStatic = new SPWindowStatic(WIID_PET_BOTTOM, 0, 300+32, 252, 4, this);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 1, 56);
	pStatic->Show();
	pStatic = NULL;

	//WIID_PET_TITLE_ICON
	pStatic = new SPWindowStatic(WIID_PET_TITLE_ICON, 8, 3, 28, 23, this);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 304, 400);
	pStatic->Show();

	//WIID_PET_TITLE
	pStatic = new SPWindowStatic(WIID_PET_TITLE, 42, 8 , 108, 15, this);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 237, 424);
	pStatic->Show();
	pStatic = NULL;

	//WIID_PET_CLOSE
	pButton = new SPWindowButton(WIID_PET_CLOSE, 231, 9 - 4, 12, 12, pCenter);
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI102.PNG",	247,	217);
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI102.PNG",	247,	230); //on
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI102.PNG",	247,	243); //push
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI102.PNG",	247,	256); //disable
	pButton->Show();
	pButton = NULL;

	//WIID_PET_SKIN_1
	pSkin1 = new SPWindowStatic(WIID_PET_SKIN_1, 2, 26 - 4 , 248, 44+15, pCenter);
	pSkin1->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 331, 18);
	pSkin1->SetSrcSize(2, 2);
	pSkin1->Show();

	//--------------------------------------------------
	// button for pet select, primary, secondary, tertiary till now.
	m_pBtnPrimary = new SPWindowButton(WIID_PET_PRIMARY, 0, 2, 64, 15, pSkin1);
	m_pBtnPrimary->SetImage		("DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG",	1,	385);
	m_pBtnPrimary->SetImageHit	("DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG",	1,	401); //on
	m_pBtnPrimary->SetImagePush	("DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG",	1,	417); //push
	m_pBtnPrimary->SetImageDisable("DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG",	1,	433); //disable
	m_pBtnPrimary->SetImageCheck("DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG",	1,	417); //push
	m_pBtnPrimary->Show();

	m_pBtnSecondary = new SPWindowButton(WIID_PET_SECONDARY, 0+64, 2, 64, 15, pSkin1);
	m_pBtnSecondary->SetImage		("DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG",	66,	385);
	m_pBtnSecondary->SetImageHit	("DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG",	66,	401); //on
	m_pBtnSecondary->SetImagePush	("DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG",	66,	417); //push
	m_pBtnSecondary->SetImageDisable("DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG",	66,	433); //disable
	m_pBtnSecondary->SetImageCheck("DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG",	66,	417); //push
	m_pBtnSecondary->Show();

	m_pBtnTertiary = new SPWindowButton(WIID_PET_TERTIARY, 0+64+64, 2, 64, 15, pSkin1);
	m_pBtnTertiary->SetImage		("DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG",	131,	385);
	m_pBtnTertiary->SetImageHit	("DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG",	131,	401); //on
	m_pBtnTertiary->SetImagePush	("DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG",	131,	417); //push
	m_pBtnTertiary->SetImageDisable("DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG",	131,	433); //disable
	m_pBtnTertiary->SetImageCheck("DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG",	131,	417); //push
	m_pBtnTertiary->Show();
	//--------------------------------------------------
	//m_pBtnNameEditor = new SPWindowButton( WIID_PET_NAME_BTN, 0+64+64+64, 2, 16, 16, pSkin1 );
	m_pBtnNameEditor = new SPWindowButton( WIID_PET_NAME_BTN, 54-2+188-16, 38-26+15, 18, 17, pSkin1 );
	m_pBtnNameEditor->SetImage		("DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG",	233, 362);
	m_pBtnNameEditor->SetImageHit	("DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG",	233,	380); //on
	m_pBtnNameEditor->SetImagePush	("DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG",	233,	398); //push
	m_pBtnNameEditor->SetImageDisable	("DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG",	233,	416); //disable
	m_pBtnNameEditor->Show();

	m_pNameEditor = new SPWindowEdit( WIID_PET_NAME_EDITOR, 54 - 2, 38 - 26+15, 188-16, 19, pSkin1 );
	m_pNameEditor->SetWindowType( WND_TYPE_LEFTBASE );
	m_pNameEditor->SetMargin( 5 , 4 );
	m_pNameEditor->SetImageNormalHead	( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 380 , 349 , 384 , 366 );
	m_pNameEditor->SetImageNormalBody	( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 384 , 349 , 388 , 366 );
	m_pNameEditor->SetImageNormalTail	( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 388 , 349 , 392 , 366 );
	m_pNameEditor->SetImageFocusHead	( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 380 , 349 , 384 , 366 );
	m_pNameEditor->SetImageFocusBody	( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 384 , 349 , 388 , 366 );
	m_pNameEditor->SetImageFocusTail	( "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , 388 , 349 , 392 , 366 );
	m_pNameEditor->SetFontColor( D3DCOLOR_ARGB( 255 , 49 , 76 , 97 ) );
	m_pNameEditor->SetFormat( DT_LEFT | DT_VCENTER );
	m_pNameEditor->SetLimitText( 16 );
	m_pNameEditor->SetMultiLine( false, 1 );
	m_pNameEditor->Hide();

	/*
	m_pNameEditor = new SPWindowEdit( WIID_PET_NAME_EDITOR, 54 - 2, 38 - 26+15, 150, 19, pSkin1 );
	m_pNameEditor->SetFontColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
	m_pNameEditor->SetLimitText(16);	
	m_pNameEditor->Hide();
	*/
	//--------------------------------------------------

	//WIID_PET_NAME
	SPWindowStatic* pStaticWnd = NULL;
	pStaticWnd = new SPWindowStatic(WIID_PET_NAME, 54 - 2, 38 - 26+15, 188-16, 19, pSkin1);
	pStaticWnd->SetWindowType(WND_TYPE_LEFTBASE);
	pStaticWnd->SetImageNormalHead	("DATA/INTERFACE/CONCEPT/UI104.PNG", 295, 34, 297, 53);
	pStaticWnd->SetImageNormalBody	("DATA/INTERFACE/CONCEPT/UI104.PNG", 298, 34, 300, 53);
	pStaticWnd->SetImageNormalTail	("DATA/INTERFACE/CONCEPT/UI104.PNG", 371, 34, 373, 53);	
	//pStaticWnd->SetFormat(DT_CENTER | DT_VCENTER | DT_WORDBREAK);	
	pStaticWnd->SetFormat(DT_CENTER | DT_VCENTER);	
	pStaticWnd->SetMultiLine(true, 2);
	pStaticWnd->SetFont(FONT_12_BOLD);
	pStaticWnd->SetFontColor(RGBA(255, 255, 255, 255));	
	//pStaticWnd->SetWindowText("WIID_PET_NAME");
	pStaticWnd->Show();
	m_pName = pStaticWnd;
	pStaticWnd = NULL;

	////WIID_PET_DESC
	//pStatic = new SPWindowStatic(WIID_PET_DESC, 69 - 2, 52 - 26, 150, 12, pSkin1);
	//pStatic->SetFormat(DT_LEFT | DT_VCENTER);
	////pStatic->SetWindowText("WIID_PET_DESC");
	//pStatic->Show();
	//m_pDesc = pStatic;
	//pStatic = NULL;

	//WIID_PET_GRID
	m_pGrid = new SPWindowGrid(WNDID_CTRL_GRID, WIID_PET_GRID, 12- 2, 32 - 26+15, 32, 32, pSkin1, 1, 1, 38, 38, 32);

	//WIID_PET_ICONLINE
	pStatic = new SPWindowStatic(WIID_PET_ICONLINE, 10 - 2, 30 - 26 + 15 , 36, 36, pSkin1);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 338, 133);
	pStatic->Show();
	pStatic = NULL;	

	//WIID_PET_LINE_1
	pStatic = new SPWindowStatic(WIID_PET_LINE_1, 2, 70 - 4+15 , 248, 1, pCenter);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 297, 35);
	pStatic->SetSrcSize(2, 1);
	pStatic->Show();
	pStatic = NULL;

	//WIID_PET_LINE_2
	pStatic = new SPWindowStatic(WIID_PET_LINE_2, 2, 294 - 4+15, 248, 1, pCenter);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 297, 35);
	pStatic->SetSrcSize(2, 1);
	pStatic->Show();
	pStatic = NULL;

	//WIID_PET_SKIN_2
	pSkin2 = new SPWindowStatic(WIID_PET_SKIN_2, 2, 71 - 4 +15, 248, 223+17, pCenter);
	pSkin2->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 379, 93);
	pSkin2->SetSrcSize(2, 2);
	pSkin2->Show();

	//WIID_PET_HPXP
	pStatic = new SPWindowStatic(WIID_PET_SPXP, 10 - 2, 79 - 71 , 33, 51, pSkin2);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG", 199, 362);
	pStatic->Show();
	pStatic = NULL;

	//WIID_PET_SP
	m_pSP = NULL;
	RECT rcTemp = {0, 0, 0, 0};
	m_pSP = new SPWindowGauge(WIID_PET_SP	, 51 - 2, 82 - 71, 120, 11, pSkin2);
	m_pSP->SetWindowType(WND_TYPE_LEFTBASE);
	SetRect(&rcTemp, 14, 48, 15, 57);
	m_pSP->SetImageGauge( "DATA/INTERFACE/CONCEPT/UI100.PNG", rcTemp);
	SetRect(&rcTemp, 23, 48, 25, 57);
	m_pSP->SetImageEdge( "DATA/INTERFACE/CONCEPT/UI100.PNG",  rcTemp);
	SetRect(&rcTemp, 18, 48, 22, 57);
	m_pSP->SetImageDeco( "DATA/INTERFACE/CONCEPT/UI100.PNG",  rcTemp);
	m_pSP->SetImageFrameHead("DATA/INTERFACE/CONCEPT/UI100.PNG", 1, 47, 3, 58);
	m_pSP->SetImageFrameBody("DATA/INTERFACE/CONCEPT/UI100.PNG", 6, 47, 8, 58);
	m_pSP->SetImageFrameTail("DATA/INTERFACE/CONCEPT/UI100.PNG", 10, 47, 12, 58);
	m_pSP->Show();

	//WIID_PET_SPINFO
	pStatic = new SPWindowStatic(WIID_PET_SPINFO, 179 - 2, 83 - 71 , 52, 12, pSkin2);
	//pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 341, 100);
	pStatic->SetFormat(DT_LEFT | DT_VCENTER);
	pStatic->SetWindowText("100 %");
	pStatic->Show();
	m_pSPInfo = pStatic;
	pStatic = NULL;

	//WIID_PET_XP	
	m_pXP = new SPWindowGauge(WIID_PET_XP	, 51 - 2, 98 - 71, 120, 9, pSkin2);	
	m_pXP->SetWindowType(WND_TYPE_LEFTBASE);
	SetRect(&rcTemp, 64, 48, 65, 55);
	m_pXP->SetImageGauge( "DATA/INTERFACE/CONCEPT/UI100.PNG", rcTemp);
	SetRect(&rcTemp, 73, 48, 75, 55);
	m_pXP->SetImageEdge( "DATA/INTERFACE/CONCEPT/UI100.PNG",  rcTemp);
	SetRect(&rcTemp, 68, 48, 72, 55);
	m_pXP->SetImageDeco( "DATA/INTERFACE/CONCEPT/UI100.PNG",  rcTemp);
	m_pXP->SetImageFrameHead("DATA/INTERFACE/CONCEPT/UI100.PNG", 51, 47, 53, 56);
	m_pXP->SetImageFrameBody("DATA/INTERFACE/CONCEPT/UI100.PNG", 56, 47, 58, 56);
	m_pXP->SetImageFrameTail("DATA/INTERFACE/CONCEPT/UI100.PNG", 60, 47, 62, 56);
	m_pXP->Show();

	//WIID_PET_XPINFO
	pStatic = new SPWindowStatic(WIID_PET_XPINFO, 179 - 2, 97 - 71 , 52, 12, pSkin2);
	//pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 341, 100);
	pStatic->SetFormat(DT_LEFT | DT_VCENTER);
	pStatic->SetWindowText("100%");
	pStatic->Show();
	m_pXPInfo = pStatic;
	pStatic = NULL;

	//--------------------------------------------------
	//WIID_PET_ENERGY	
	m_pEnergy = new SPWindowGauge(WIID_PET_ENERGY	, 51 - 2, 114 - 71, 120, 9, pSkin2);	
	m_pEnergy->SetWindowType(WND_TYPE_LEFTBASE);
	SetRect(&rcTemp, 64, 48, 65, 55);
	m_pEnergy->SetImageGauge( "DATA/INTERFACE/CONCEPT/UI100.PNG", rcTemp);
	SetRect(&rcTemp, 73, 48, 75, 55);
	m_pEnergy->SetImageEdge( "DATA/INTERFACE/CONCEPT/UI100.PNG",  rcTemp);
	SetRect(&rcTemp, 68, 48, 72, 55);
	m_pEnergy->SetImageDeco( "DATA/INTERFACE/CONCEPT/UI100.PNG",  rcTemp);
	m_pEnergy->SetImageFrameHead("DATA/INTERFACE/CONCEPT/UI100.PNG", 51, 47, 53, 56);
	m_pEnergy->SetImageFrameBody("DATA/INTERFACE/CONCEPT/UI100.PNG", 56, 47, 58, 56);
	m_pEnergy->SetImageFrameTail("DATA/INTERFACE/CONCEPT/UI100.PNG", 60, 47, 62, 56);
	m_pEnergy->Show();
	m_pEnergy->SPSendMessage(SPIM_SET_MAXVALUE, (WPARAM)100);
	m_pEnergy->SPSendMessage(SPIM_SET_CURVALUE, (WPARAM)0);
	sprintf(m_szEnergyInfo, "0 %%");
	m_pEnergy->SetWindowText(m_szEnergyInfo);

	//WIID_PET_ENERGYINFO
	pStatic = new SPWindowStatic(WIID_PET_ENERGYINFO, 179 - 2, 113 - 71 , 52, 12, pSkin2);
	//pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 341, 100);
	pStatic->SetFormat(DT_LEFT | DT_VCENTER);
	pStatic->SetWindowText("0.00%");
	pStatic->Show();
	m_pEnergyInfo = pStatic;
	pStatic = NULL;
	//--------------------------------------------------

	//WIID_PET_SKILLINFO
	pStatic = new SPWindowStatic(WIID_PET_SKILLINFO, 10 - 2, 119 - 71+17 , 101, 13, pSkin2);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 202, 396);	
	pStatic->Show();
	pStatic = NULL;

	//WIID_PET_SKILL_1
	pStatic = new SPWindowStatic(WIID_PET_SKILL_1, 19 - 2, 138 - 71+17 , 32, 32, pSkin2);	
	pStatic->Show();
	pStatic = NULL;

	//WIID_PET_SKILL_2
	pStatic = new SPWindowStatic(WIID_PET_SKILL_2, 63 - 2, 138 - 71+17 , 32, 32, pSkin2);	
	pStatic->Show();
	pStatic = NULL;

	//WIID_PET_SKILLLINE1
	pStatic = new SPWindowStatic(WIID_PET_SKILLLINE1, 17 - 2, 136 - 71+17 , 36, 36, pSkin2);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI100.PNG", 475, 115);	
	pStatic->Show();
	pStatic = NULL;
	
	//WIID_PET_SKILLLINE2
	pStatic = new SPWindowStatic(WIID_PET_SKILLLINE2, 61 - 2, 136 - 71+17 , 36, 36, pSkin2);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI100.PNG", 475, 115);	
	pStatic->Show();
	pStatic = NULL;	

	//WIID_PET_ABILITYINFO
	pStatic = new SPWindowStatic(WIID_PET_ABILITYINFO, 10 - 2, 176 - 71+17 , 101, 13, pSkin2);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 202, 410);	
	pStatic->Show();
	pStatic = NULL;	
	
	//WIID_PET_ABILITY	
	pStatic = new SPWindowStatic(WIID_PET_ABILITY, 19 - 2, 195 - 71+17 , 32, 32, pSkin2);	
	pStatic->Show();
	pStatic = NULL;

	//WIID_PET_ABILITY2
	pStatic = new SPWindowStatic(WIID_PET_ABILITY2, 63 - 2, 195 - 71+17 , 32, 32, pSkin2);	
	pStatic->Show();
	pStatic = NULL;

	//WIID_PET_ABILITYLINE
	pStatic = new SPWindowStatic(WIID_PET_ABILITYLINE, 17 - 2, 193 - 71+17 , 36, 36, pSkin2);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI100.PNG", 475, 115);	
	pStatic->Show();
	pStatic = NULL;

	//WIID_PET_ABILITYLINE2
	pStatic = new SPWindowStatic(WIID_PET_ABILITYLINE2, 61 - 2, 193 - 71+17 , 36, 36, pSkin2);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI100.PNG", 475, 115);	
	pStatic->Show();
	pStatic = NULL;

	//WIID_PET_TALK_TOP	
	pStatic = new SPWindowStatic(WIID_PET_TALK_TOP, 119 - 2, 142 - 71+17 , 123, 2, pSkin2);		
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 115, 34);	
	pStatic->Show();
	pStatic = NULL;

	//WIID_PET_TALK_CENTER
	pStatic = new SPWindowStatic(WIID_PET_TALK_CENTER, 119 - 2, 144 - 71+17 , 123, 33, pSkin2);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 115, 37);	
	pStatic->SetSrcSize(123, 1);
	pStatic->Show();
	pStatic = NULL;

	//WIID_PET_TALK_BOTTOM
	pStatic = new SPWindowStatic(WIID_PET_TALK_BOTTOM, 119 - 2, 177 - 71+17 , 123, 6, pSkin2);		
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 115, 39);	
	pStatic->Show();
	pStatic = NULL;

	//WIID_PET_TALK
	pStatic = new SPWindowStatic(WIID_PET_TALK, 121 - 2, 144 - 71+17 , 119, 33, pSkin2);
	//pStatic->SetFormat(DT_CENTER | DT_WORDBREAK);
	pStatic->SetMultiLine(true, 2);
	pStatic->SetFontColor(RGBA(0, 0, 0, 255));
	pStatic->SetWindowText("Pet Talk Message... Area");
	pStatic->Show();
	m_pPetMsg = pStatic;
	pStatic = NULL;

	//WIID_PET_EAT
	pButton = new SPWindowButton(WIID_PET_EAT, 120 - 2, 145 - 71+17 , 130, 149, pSkin2);
	pButton->Show();
	pButton = NULL;

	//WIID_PET_IMAGE_LINE
	pStatic = new SPWindowStatic(WIID_PET_IMAGE_LINE, 120 - 2, 103 - 71+17 , 130, 149, pSkin2);		
	//pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 337, 92);	
	pStatic->Show();
	pStatic = NULL;	

	////WIID_PET_FEED	
	//pButton = new SPWindowButton(WIID_PET_FEED, 10, 257 - 4, 24, 24, pCenter);
	//pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI104.PNG",	210,	61);
	//pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI104.PNG",	210,	86); //on
	//pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI104.PNG",	210,	111); //push
	//pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI104.PNG",	210,	136); //disable
	//pButton->Show();
	//pButton = NULL;	
	
	////WIID_PET_SOULREMOVE	
	//pButton = new SPWindowButton(WIID_PET_SOULREMOVE, 62, 257 - 4, 24, 24, pCenter);
	//pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI104.PNG",	260,	61);
	//pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI104.PNG",	260,	86); //on
	//pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI104.PNG",	260,	111); //push
	//pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI104.PNG",	260,	136); //disable
	//pButton->Show();
	//pButton = NULL;   

	//WIID_PET_FEED_ITEM
	pStatic = new SPWindowStatic(WIID_PET_FEED_ITEM, 10 - 2, 233 - 71+17 , 101, 13, pSkin2);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 273, 245);	
	pStatic->Show();
	pStatic = NULL;	

	//WIID_PET_FEED_ITEM_BUTTON
	pButton = new SPWindowButton(WIID_PET_FEED_ITEM_BUTTON, 17 - 2, 250 - 71+17 , 36, 36, pSkin2);
	pButton->Show();
	pButton = NULL;

	//WIID_PET_FEED_ITEM_SKIN
	pStatic = new SPWindowStatic(WIID_PET_FEED_ITEM_SKIN, 17 - 2, 250 - 71+17 , 36, 36, pSkin2);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI100.PNG", 475, 115);	
	pStatic->Show();
	pStatic = NULL;	

	//WIID_PET_FEED_ITEM_ICON	
	pStatic = new SPWindowStatic(WIID_PET_FEED_ITEM_ICON, 19 - 2, 252 - 71+17 , 32, 32, pSkin2);
	//pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 273, 245);
	pStatic->Show();
	m_pPetFeedIcon = pStatic;
	m_pPetFeedIcon->GetWindowRect(m_rtStack);
	m_bItemAble = true;
	m_iItemMark = ICON_MARK_NULL;
	m_iStack = 0;
	pStatic = NULL;		

	//--------------------------------------------------
	//WIID_PET_FEED_ITEM_BUTTON
	pButton = new SPWindowButton(WIID_PET_FEED_ITEM_BUTTON_2, 61 - 2, 250 - 71+17 , 36, 36, pSkin2);
	pButton->Show();
	pButton = NULL;

	//WIID_PET_FEED_ITEM_SKIN2
	pStatic = new SPWindowStatic(WIID_PET_FEED_ITEM_SKIN_2, 61 - 2, 250 - 71+17 , 36, 36, pSkin2);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI100.PNG", 475, 115);	
	pStatic->Show();
	pStatic = NULL;	

	//WIID_PET_FEED_ITEM_ICON	
	pStatic = new SPWindowStatic(WIID_PET_FEED_ITEM_ICON_2, 63 - 2, 252 - 71+17 , 32, 32, pSkin2);
	pStatic->Show();
	m_pPetFeedIcon2 = pStatic;
	m_pPetFeedIcon2->GetWindowRect(m_rtStack_energy);
	pStatic = NULL;	
	//--------------------------------------------------

	RECT rtRect;
	GetWindowRect(rtRect);
	m_pPetShop = new SPWindowPetShop(WIID_PETSHOP, m_iSX, 0, 240, 105, this);
}


bool SPWindowPet::LoadPetTalkLDT()
{
	SPLDTFile* pLDTFile = NULL;
	g_pResourceManager->GetLDTFromFileName( RES_FNAME_LDT_PET_TALK , &pLDTFile);	
	if( pLDTFile == NULL )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "----------------------------------->ITEM ADD TIME LDT FILE LOAD FAIL [%s] \n" , RES_FNAME_LDT_PET_TALK );
#endif
		return false;
	}

	int				iItemID , i;
	LDT_Field		LDTField;	
	int				iRecordCount	= pLDTFile->GetItemCount();	

	for( i = 0; i < iRecordCount; ++i )
	{		
		iItemID	=	pLDTFile->GetPrimaryKey( i );
		
		if(iItemID == 0)
			break;
		
		SPPetTalk*		PetTalk = new SPPetTalk;
		PetTalk->Clear();

		pLDTFile->GetFieldFromLable( iItemID , "_ItemID" , LDTField );
		PetTalk->m_iPetID	=	LDTField.uData.lValue;

		pLDTFile->GetFieldFromLable( iItemID , "_Name" , LDTField );
		if( LDTField.uData.pValue != NULL )
		{
			if( strlen( LDTField.uData.pValue ) > 0 )
			{
				PetTalk->m_strName	=	LDTField.uData.pValue;
			}
		}

		pLDTFile->GetFieldFromLable( iItemID , "_Talk1" , LDTField );
		if( LDTField.uData.pValue != NULL )
		{
			if( strlen( LDTField.uData.pValue ) > 0 )
			{
				PetTalk->m_strSP	=	LDTField.uData.pValue;
			}
		}

		pLDTFile->GetFieldFromLable( iItemID , "_Talk2" , LDTField );
		if( LDTField.uData.pValue != NULL )
		{
			if( strlen( LDTField.uData.pValue ) > 0 )
			{
				PetTalk->m_strEXP	=	LDTField.uData.pValue;
			}
		}
		
		PetTalk->m_iTalkIndex = 0;

		pLDTFile->GetFieldFromLable( iItemID , "_Talk3" , LDTField );
		if( LDTField.uData.pValue != NULL )
		{
			if( strlen( LDTField.uData.pValue ) > 0 )
			{
				PetTalk->m_strTalk[PetTalk->m_iTalkIndex]	=	LDTField.uData.pValue;
				PetTalk->m_iTalkIndex++;
			}
		}

		pLDTFile->GetFieldFromLable( iItemID , "_Talk4" , LDTField );
		if( LDTField.uData.pValue != NULL )
		{
			if( strlen( LDTField.uData.pValue ) > 0 )
			{
				PetTalk->m_strTalk[PetTalk->m_iTalkIndex]	=	LDTField.uData.pValue;
				PetTalk->m_iTalkIndex++;
			}
		}

		pLDTFile->GetFieldFromLable( iItemID , "_Talk5" , LDTField );
		if( LDTField.uData.pValue != NULL )
		{
			if( strlen( LDTField.uData.pValue ) > 0 )
			{
				PetTalk->m_strTalk[PetTalk->m_iTalkIndex]	=	LDTField.uData.pValue;
				PetTalk->m_iTalkIndex++;
			}
		}
		
		m_vpPetTalk.push_back(PetTalk);
	}

	SAFE_RELEASE(pLDTFile);
	return true;
}


void SPWindowPet::ClearAllPetTalk()
{
	std::vector<SPPetTalk*>::iterator iter ;
	iter = m_vpPetTalk.begin();
	for(; iter != m_vpPetTalk.end(); ++iter) {
		SAFE_DELETE(*iter);
	}

	m_vpPetTalk.clear();
}


SPPetTalk* SPWindowPet::FindPetTalk(int iID)
{
	std::vector<SPPetTalk*>::iterator Iter = 
		std::find_if(m_vpPetTalk.begin(),
		m_vpPetTalk.end(),
		std::bind2nd(IsEqualPet,
		iID));	

	if(Iter == m_vpPetTalk.end())
		return NULL;

	return (*Iter);
}


void SPWindowPet::UpdatePetTalk()
{
	m_pPetMsg->SetWindowText(NULL);
	SPItem	*&pPetItem = m_petInfo[m_iCurPetSelect].m_pPetItem;

	if(pPetItem == NULL || pPetItem->GetItemStatus() == NULL || pPetItem->GetItemStatus()->GetItemID() == 0 || pPetItem->GetItemAttr() == NULL) {
		return;
	}

	if(m_pCurPetTalk == NULL || m_pCurPetTalk->m_iPetID == 0) {
		return;
	}

	// 펫이 시간제한이 걸린경우
	if(pPetItem->GetItemStatus()->IsTimeAvailable() == false) {		
		return;
	}

	if(pPetItem->GetItemStatus()->GetExp() > 0 && 
		(pPetItem->GetItemStatus()->GetExp() == pPetItem->GetItemAttr()->m_iPetMaxExp)) 
	{
		m_pPetMsg->SetWindowText(m_pCurPetTalk->m_strEXP.c_str());
		return;
	}

	if(pPetItem->GetItemStatus()->GetSp() <= PET_SP_HUNGRY) {
		m_pPetMsg->SetWindowText(m_pCurPetTalk->m_strSP.c_str());
		return;
	}

	int iRand = rand() % m_pCurPetTalk->m_iTalkIndex;
	if(!m_pCurPetTalk->m_strTalk[iRand].empty()) {
		m_pPetMsg->SetWindowText(m_pCurPetTalk->m_strTalk[iRand].c_str());
	}
}


void SPWindowPet::ClearPetFeed()
{
	m_pPetFeedIcon->SPSendMessage(SPIM_SET_IMAGECLEAR);		
	m_bItemAble = true;
	m_iItemMark = ICON_MARK_NULL;
	m_iStack = 0;
	//m_pPetFeed = NULL;	
	m_petInfo[m_iCurPetSelect].m_pPetFeed = NULL;

	SPPlayerInvenArchive* pInven = NULL;
	if(g_pGOBManager->GetLocalPlayer()) {
		pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
		pInven->ClearPetFeedItem(m_iCurPetSelect);
	}
}
void SPWindowPet::ClearPetFeedEnergy( void )
{
	m_pPetFeedIcon2->SPSendMessage(SPIM_SET_IMAGECLEAR);		
	m_bItemAble_energy = true;
	m_iItemMark_energy = ICON_MARK_NULL;
	m_iStack_energy = 0;
	//m_pPetFeed = NULL;	
	m_petInfo[m_iCurPetSelect].m_pPetFeedEnergy = NULL;

	SPPlayerInvenArchive* pInven = NULL;
	if(g_pGOBManager->GetLocalPlayer()) {
		pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
		pInven->ClearPetFeedEnergyItem(m_iCurPetSelect);
	}
}


bool SPWindowPet::UpdatePetFeed()
{
	SPPlayerInvenArchive* pInven = NULL;
	if(g_pGOBManager->GetLocalPlayer())
		pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	else 
		return false;
	
	SPItem	*&pPetFeed = m_petInfo[m_iCurPetSelect].m_pPetFeed;
	SPItem	*&pPetFeedEnergy = m_petInfo[m_iCurPetSelect].m_pPetFeedEnergy;

	//--------------------------------------------------
	// 먹이로 사용할 아이템이 있는지
	if(pPetFeed == NULL || pPetFeed->GetItemStatus() == NULL || pPetFeed->GetItemStatus()->GetItemID() == 0) {
		ClearPetFeed();
	}
	else
	{
		// 아이템 스택이 0인경우 
		if(pPetFeed && pPetFeed->GetItemStatus()) {
			if(pPetFeed->GetItemStatus()->GetItemID() == 0 || pPetFeed->GetItemStatus()->GetStackCount() == 0) {
				ClearPetFeed();
				return false;
			}
		}	
		
		POINT ptSrcPos;

		SPTexture* pTexture = g_pItemCluster->GetItemTexture(pPetFeed->GetItemStatus()->GetItemID(), ptSrcPos);

		m_pPetFeedIcon->Hide();
		m_pPetFeedIcon->SPSendMessage(SPIM_SET_IMAGECLEAR);
		m_pPetFeedIcon->SetImage(pTexture, ptSrcPos.x, ptSrcPos.y);
		m_pPetFeedIcon->Show();

		m_iStack = pPetFeed->GetItemStatusValue(MV_ITEM_GETSTACK_CNT);
		m_bItemAble = g_pCheckManager->CheckItemStatus(pPetFeed->GetItemStatus()->GetItemID());
		m_bItemAble &= pPetFeed->GetItemStatus()->IsTimeAvailable();
		m_iItemMark = g_pItemCluster->GetItemIconMark(pPetFeed->GetItemStatus()->GetItemID());
		pInven->SetPetFeedItem( m_iCurPetSelect, pPetFeed->GetContainerType(), pPetFeed->GetSlotIndex());
	}
	//--------------------------------------------------
	if(pPetFeedEnergy == NULL || pPetFeedEnergy->GetItemStatus() == NULL || pPetFeedEnergy->GetItemStatus()->GetItemID() == 0) {
		ClearPetFeedEnergy();
	}
	else
	{
		if(pPetFeedEnergy && pPetFeedEnergy->GetItemStatus()) {
			if(pPetFeedEnergy->GetItemStatus()->GetItemID() == 0 || pPetFeedEnergy->GetItemStatus()->GetStackCount() == 0) {
				ClearPetFeedEnergy();
				return false;
			}
		}		
		
		POINT ptSrcPos;
		SPTexture* pTexture = g_pItemCluster->GetItemTexture(pPetFeedEnergy->GetItemStatus()->GetItemID(), ptSrcPos);

		m_pPetFeedIcon2->Hide();
		m_pPetFeedIcon2->SPSendMessage(SPIM_SET_IMAGECLEAR);
		m_pPetFeedIcon2->SetImage(pTexture, ptSrcPos.x, ptSrcPos.y);
		m_pPetFeedIcon2->Show();

		m_iStack_energy = pPetFeedEnergy->GetItemStatusValue(MV_ITEM_GETSTACK_CNT);
		m_bItemAble_energy = g_pCheckManager->CheckItemStatus(pPetFeedEnergy->GetItemStatus()->GetItemID());
		m_bItemAble_energy &= pPetFeedEnergy->GetItemStatus()->IsTimeAvailable();
		m_iItemMark_energy = g_pItemCluster->GetItemIconMark(pPetFeedEnergy->GetItemStatus()->GetItemID());
		pInven->SetPetFeedEnergyItem( m_iCurPetSelect, pPetFeedEnergy->GetContainerType(), pPetFeedEnergy->GetSlotIndex());
	}
	//--------------------------------------------------

	return true;
}


bool SPWindowPet::CheckPetFeed(PET_PLACE iPlace)
{	
	SPItem	*&pPetFeed = m_petInfo[iPlace].m_pPetFeed;
	SPItem	*&pPetItem = m_petInfo[iPlace].m_pPetItem;

	SPPet *pPet = NULL;
	if (g_pGOBManager->GetLocalPlayer() && g_pGOBManager->GetLocalPlayer()->GetPetMgr())
	{
		pPet = g_pGOBManager->GetLocalPlayer()->GetPetMgr()->GetPet( iPlace );
	}
	else
		return false;


	//펫을 장비하고 있는지
	if(pPetItem == NULL || pPetItem->GetItemStatus() == NULL || pPetItem->GetItemStatus()->GetItemID() == 0 || pPetItem->GetItemAttr() == NULL) {
		return false;
	}

	// 펫이 시간제한에 걸린 경우
	if(pPetItem->GetItemStatus()->IsTimeAvailable() == false) {		
		return false;
	}

	if (pPet)
	{
		if (pPet->IsNewGenerationPet())
		{
			if (pPet->GetPetAttr()->sp >= PET_SP_HUNGRY)
				return false;
		}
		else
		{
			// 펫 SP가 50이상인 경우
			if(pPetItem->GetItemStatus()->GetSp() >= PET_SP_HUNGRY) {		
				return false;
			}
		}
	}
	else
		return false;

	// 먹이로 사용할 아이템이 있는지
	if(pPetFeed == NULL || pPetFeed->GetItemStatus() == NULL || pPetFeed->GetItemStatus()->GetItemID() == 0) {
		return false;
	}

	if(pPetFeed->GetItemStatus()->GetStackCount() < 1) {
		return false;
	}

	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return false;

	//사망시 제한
	if(g_pGOBManager->GetLocalPlayer()->GetCoordPhysics()->GetCurState() == GOB_STATE_DEAD) {		
		return false;
	}

	return true;
}
bool SPWindowPet::CheckPetFeedEnergy( PET_PLACE iPlace )
{
	SPItem	*&pPetFeedEnergy = m_petInfo[iPlace].m_pPetFeedEnergy;
	SPItem	*&pPetItem = m_petInfo[iPlace].m_pPetItem;

	if(pPetItem == NULL || pPetItem->GetItemStatus() == NULL || pPetItem->GetItemStatus()->GetItemID() == 0 || pPetItem->GetItemAttr() == NULL) {
		return false;
	}

	if(pPetItem->GetItemStatus()->IsTimeAvailable() == false) {		
		return false;
	}

	if (!g_pGOBManager->GetLocalPlayer() || !g_pGOBManager->GetLocalPlayer()->GetPetMgr())
		return false;

	SPPet	*pet = g_pGOBManager->GetLocalPlayer()->GetPetMgr()->GetPet( iPlace );
	if (!pet)
		return false;

	if (pet->GetPetAttr()->ssp >= PET_ENERGY_HUNGRY)
		return false; 

	if (!pPetFeedEnergy || pPetFeedEnergy->GetItemStatus() == NULL || pPetFeedEnergy->GetItemStatus()->GetItemID() == 0)
		return false;

	if (pPetFeedEnergy->GetItemStatus()->GetStackCount() < 1)
		return false;
	
	if (g_pGOBManager->GetLocalPlayer() == NULL)
		return false;

	if (g_pGOBManager->GetLocalPlayer()->GetCoordPhysics()->GetCurState() == GOB_STATE_DEAD)
		return false;

	return true;
}

void SPWindowPet::AutoPetEat( PET_PLACE iPlace )
{
	SPItem	*&pPetFeed = m_petInfo[iPlace].m_pPetFeed;
	SPItem	*&pPetItem = m_petInfo[iPlace].m_pPetItem;

	//펫을 장비하고 있는지
	if(pPetItem == NULL || pPetItem->GetItemStatus() == NULL || pPetItem->GetItemStatus()->GetItemID() == 0 || pPetItem->GetItemAttr() == NULL) {
		return;
	}

	// 먹이로 사용할 아이템이 있는지
	if(pPetFeed == NULL || pPetFeed->GetItemStatus() == NULL || pPetFeed->GetItemStatus()->GetItemID() == 0) {
		return;
	}

	//// 캐릭터가 서있는지?
	//if(g_pGOBManager->GetLocalPlayer()) {
	//	if((g_pGOBManager->GetLocalPlayer()->GetCurAction() != ACTION_STOP)){			
	//		return;
	//	}
	//}

	if(g_pCheckManager->IsDBBlock()) 
		return;

	// 패킷 전송
	CONTAINER_SLOT stPet;
	CONTAINER_SLOT stFeed;
	ZeroMemory(&stPet, sizeof(CONTAINER_SLOT));
	ZeroMemory(&stFeed, sizeof(CONTAINER_SLOT));

	stPet.ContainerType = pPetItem->GetContainerType();
	stPet.SlotIndex = pPetItem->GetSlotIndex() + 1;	
	stFeed.ContainerType = pPetFeed->GetContainerType();
	stFeed.SlotIndex = pPetFeed->GetSlotIndex() + 1;
	if(stPet.SlotIndex < 1 || stFeed.SlotIndex < 1) {
		return;
	}	

	//CONTAINER_SLOT		stMainItemPos;	
	// ITEMNO				iMainItemNo;
	//CONTAINER_SLOT		stMaterialItemPos;
	// ITEMNO				iMaterialItemNo;

	CPacket Packet;
	Packet.Add((UINT32)ITEM_CS_ADDSP);
	Packet.AddData(&stPet, sizeof(CONTAINER_SLOT));
	INT64 iPetItemNo = *(ITEMNO*)pPetItem->GetItemStatus()->SPSendMessage( MV_ITEM_GET_ITEMNO );	
	Packet.AddUINT64(iPetItemNo);

	Packet.AddData(&stFeed, sizeof(CONTAINER_SLOT));
	INT64 iFeedItemNo = *(ITEMNO*)pPetFeed->GetItemStatus()->SPSendMessage( MV_ITEM_GET_ITEMNO );
	Packet.AddUINT64(iFeedItemNo);	

	g_pNetworkManager->SPPerformMessage(ITEM_CS_ADDSP, 0, (LPARAM)&Packet);
}

void SPWindowPet::AutoPetEatEnergy( PET_PLACE iPlace )
{
	if (!g_pGOBManager->GetLocalPlayer() || !g_pGOBManager->GetLocalPlayer()->GetPetMgr())
		return;

	SPItem *&pPetFeedEnergy = m_petInfo[iPlace].m_pPetFeedEnergy;
	SPItem *&pPetItem = m_petInfo[iPlace].m_pPetItem;

	if (pPetItem == NULL || pPetItem->GetItemStatus() == NULL || pPetItem->GetItemStatus()->GetItemID() == 0
		||
		pPetItem->GetItemAttr() == NULL)
		return;

	if (pPetFeedEnergy == NULL || pPetFeedEnergy->GetItemStatus() == NULL || pPetFeedEnergy->GetItemStatus()->GetItemID() == 0)
		return;

	if (g_pCheckManager->IsDBBlock())
		return;

	CONTAINER_SLOT stFeed;
	ZeroMemory(&stFeed, sizeof(CONTAINER_SLOT));

	stFeed.ContainerType = pPetFeedEnergy->GetContainerType();
	stFeed.SlotIndex = pPetFeedEnergy->GetSlotIndex() + 1;
	if(stFeed.SlotIndex < 1) {
		return;
	}

	//--------------------------------------------------
	SPPet *pet = g_pGOBManager->GetLocalPlayer()->GetPetMgr()->GetPet( iPlace );
	if (!pet)
		return;

	CPacket packet;
	packet.Add((UINT32)PET_CS_ADD_SSP);
	packet.AddData(&stFeed, sizeof(CONTAINER_SLOT));
	INT64 iPetItemNo = pet->GetItemNoID();
	packet.AddUINT64( iPetItemNo );

	g_pNetworkManager->SPPerformMessage( PET_CS_ADD_SSP, 0, (LPARAM)&packet );
}


void SPWindowPet::InitSkillUI()
{
	SPUISkillUnit*		pUISkillUnit	=	NULL;
	char				szResPath[]		=	"DATA/INTERFACE/CONCEPT/UI100.PNG";

	m_UIUnitManager.SetBufferSize( MAX_ITEM_SKILL );

	for( int i = 0 ; i < MAX_ITEM_SKILL ; ++i )
	{
		pUISkillUnit	=	new SPUISkillUnit;

		pUISkillUnit->SetPos( 0 , 0 );
		pUISkillUnit->SetSize( 32 , 32 );
		pUISkillUnit->SetDisableTexture( szResPath );
		pUISkillUnit->SetDisableRectSrc( 434 , 37 , 436 , 39 );
		pUISkillUnit->SetCoolTimeTexture( szResPath );
		pUISkillUnit->SetCoolTimeRectSrc( 434 , 37 , 436 , 39 );

		pUISkillUnit->SetComboTexture( "DATA/INTERFACE/CONCEPT/Combo_Skill_ICON.png" );
		pUISkillUnit->SetComboRectSrc( 0 , 0 , 32 , 32 );

		pUISkillUnit->SetToggleTexture( "DATA/INTERFACE/CONCEPT/ui_Icon_On.png" );

		pUISkillUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_ENABLE );
		pUISkillUnit->SetInstanceID( WIID_SKILL );

		m_UIUnitManager.AddUnit( pUISkillUnit );
	}

	m_bLMouseDown	=	FALSE;
	m_lParam		=	0;
}

void SPWindowPet::UpdatePosSkillUI()
{
	SPWindow*	pWindow	=	NULL;
	SPUIUnit*	pUIUnit	=	NULL;
	RECT		rcWindowPos;

	pWindow	=	Find( WIID_PET_SKILL_1 , true );

	if( pWindow != NULL )
	{
		pWindow->GetWindowRect( rcWindowPos );

		pUIUnit	=	m_UIUnitManager.GetUnit( PET_SKILL_ONE );

		if( pUIUnit != NULL )
			pUIUnit->SetPos( rcWindowPos.left , rcWindowPos.top );
	}

	pWindow	=	Find( WIID_PET_SKILL_2 , true );

	if( pWindow != NULL )
	{
		pWindow->GetWindowRect( rcWindowPos );

		pUIUnit	=	m_UIUnitManager.GetUnit( PET_SKILL_TWO );

		if( pUIUnit != NULL )
			pUIUnit->SetPos( rcWindowPos.left , rcWindowPos.top );
	}

	pWindow	=	Find( WIID_PET_ABILITY , true );

	if( pWindow != NULL )
	{
		pWindow->GetWindowRect( rcWindowPos );

		pUIUnit	=	m_UIUnitManager.GetUnit( PET_SPECIAL_SKILL_ONE );

		if( pUIUnit != NULL )
			pUIUnit->SetPos( rcWindowPos.left , rcWindowPos.top );
	}

	pWindow = Find( WIID_PET_ABILITY2, true );
	if (pWindow)
	{
		pWindow->GetWindowRect( rcWindowPos );
		pUIUnit = m_UIUnitManager.GetUnit( PET_SPECIAL_SKILL_TWO );
		if (pUIUnit)
			pUIUnit->SetPos( rcWindowPos.left, rcWindowPos.top );
	}

	m_UIUnitManager.ReCalRect();
}

void SPWindowPet::UpdateSkillUI()
{
	SPItem	*&pPetItem = m_petInfo[m_iCurPetSelect].m_pPetItem;
	if (!g_pGOBManager->GetLocalPlayer() || !g_pGOBManager->GetLocalPlayer()->GetPetMgr())
		return;
	SPPet	*pPet = g_pGOBManager->GetLocalPlayer()->GetPetMgr()->GetPet( m_iCurPetSelect );
	SPUIUnit*	pUIUnit	=	NULL;

	for( int i = 0 ; i < MAX_ITEM_SKILL ; ++i )
	{
		pUIUnit	=	m_UIUnitManager.GetUnit( i );

		if( pUIUnit != NULL )
			pUIUnit->SetStatus( SPUIUnit::STATUS_UI_UNIT_ENABLE | SPUIUnit::STATUS_UI_UNIT_NO_DRAGNDROP );
	}

	if( pPetItem == NULL )
		return;

	SPItemAttr*		pPetItemAttr	=	pPetItem->GetItemAttr();
	SPItemStatus*	pPetItemStatus	=	pPetItem->GetItemStatus();

	if( pPetItemAttr == NULL || pPetItemStatus == NULL )
		return;

	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return;

	SPSkillArchive*	pSkillArchive	=	pLocalPlayer->GetSkillArchive();

	if( pSkillArchive == NULL )
		return;

	std::vector< int >		vSkill;
	std::vector< int >		vSpecialSkill;
	SPItemSkillActivity*	pItemSkillActivity;
	SPSkill*				pItemSkill;

	if (!pPet)
		return;

	if (!pPet->IsNewGenerationPet())
	{
		for( i = 0 ; i < MAX_ITEM_SKILL ; ++i )
		{
			if( pPetItemAttr->m_iSkillID[ i ] == 0 )
				continue;

			pItemSkillActivity	=	pSkillArchive->GetItemSkillActivity( pPetItemAttr->m_iSkillID[ i ] , pPetItem->GetContainerType() , pPetItem->GetSlotIndex() );

			if( pItemSkillActivity == NULL )
				continue;

			pItemSkill	=	pItemSkillActivity->GetSkill();

			if( pItemSkill == NULL )
				continue;

			if( pItemSkill->GetSpecialType() == SST_NULL )
				vSkill.push_back( pPetItemAttr->m_iSkillID[ i ] );
			else
			{
				if( vSpecialSkill.empty() == true )
					vSpecialSkill.push_back( pPetItemAttr->m_iSkillID[ i ] );
			}
		}
	}
	else // if (pPet->IsNewGenerationPet())
	{
		std::vector< t_Pet_Skill_To_Client > vecPetSkill = pPet->GetSkill();
		for (int idx = 0; idx < (int)vecPetSkill.size(); ++idx)
		{
			if (vecPetSkill[idx].nSkillID == 0)
				continue;

			pItemSkillActivity = pSkillArchive->GetItemSkillActivity( vecPetSkill[idx].nSkillID,
				pPetItem->GetContainerType(), pPetItem->GetSlotIndex());
			
			if (pItemSkillActivity == NULL)
				continue;

			pItemSkill	=	pItemSkillActivity->GetSkill();

			if( pItemSkill == NULL )
				continue;

			/*
			if( pItemSkill->GetSpecialType() == SST_NULL )
				vSkill.push_back( vecPetSkill[idx].nSkillID );
			else
			{
				//if( vSpecialSkill.empty() == true )
				vSpecialSkill.push_back( vecPetSkill[idx].nSkillID);
			}
			*/
			if (vSkill.size() < 2)
				vSkill.push_back( vecPetSkill[idx].nSkillID );
			else
			{
				if (vSpecialSkill.size() < 2)
					vSpecialSkill.push_back( vecPetSkill[idx].nSkillID );
			}
		}
	}

	CONTAINER_SLOT		stContainerSlot;

	stContainerSlot.ContainerType	=	pPetItem->GetContainerType();
	stContainerSlot.SlotIndex		=	pPetItem->GetSlotIndex();

	int iPetItemID	=	pPetItem->GetItemStatusValue( MV_ITEM_GETUID );

	UpdateSkillUIContents( vSkill		 , iPetItemID , stContainerSlot , PET_SKILL_ONE			);
	UpdateSkillUIContents( vSpecialSkill , iPetItemID , stContainerSlot , PET_SPECIAL_SKILL_ONE	);
}

void SPWindowPet::UpdateSkillUIContents( std::vector< int >& vSkillList , int iItemID , CONTAINER_SLOT stContainerSlot , int iStartPos )
{
	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();
	if (!g_pGOBManager->GetLocalPlayer() || !g_pGOBManager->GetLocalPlayer()->GetPetMgr())
		return;
	SPPet	*pPet = g_pGOBManager->GetLocalPlayer()->GetPetMgr()->GetPet( m_iCurPetSelect );

	if (!pPet)
		return;

	if( pLocalPlayer == NULL )
		return;

	SPSkillArchive*	pSkillArchive	=	pLocalPlayer->GetSkillArchive();

	if( pSkillArchive == NULL )
		return;

	int	iCount	=	(int)vSkillList.size();

	if( iCount <= 0 )
		return;

	int						iSkillID;
	SKILL_UI_INFO*			pSkillUIInfo		=	NULL;
	SPUISkillUnit*			pUISkillUnit		=	NULL;
	SPItemSkillActivity*	pItemSkillActivity	=	NULL;
	SPSkill*				pItemSkill			=	NULL;

	for( int i = 0 ; i < iCount ; ++i )
	{
		iSkillID	=	vSkillList.at( i );

		pItemSkillActivity	=	pSkillArchive->GetItemSkillActivity( iSkillID , stContainerSlot.ContainerType , stContainerSlot.SlotIndex );

		if( pItemSkillActivity == NULL )
			continue;

		pItemSkill	=	pItemSkillActivity->GetSkill();

		if( pItemSkill == NULL )
			continue;

		pUISkillUnit	=	(SPUISkillUnit*)m_UIUnitManager.GetUnit( iStartPos + i );

		if( pUISkillUnit == NULL )
			continue;

		pUISkillUnit->SetSkillID( iSkillID );
		pUISkillUnit->SetType( SIT_NULL );
		pUISkillUnit->SetItemID( iItemID );
		pUISkillUnit->SetContainerSlot( stContainerSlot );
		pUISkillUnit->SetCoolTimeID( pItemSkill->GetCoolTimeID() );
		pUISkillUnit->SetToggle( pItemSkillActivity->GetToggleOn() );

		if( pUISkillUnit->ReResourceLoad() == FALSE )
		{
			pUISkillUnit->SetStatus( SPUIUnit::STATUS_UI_UNIT_ENABLE );
			continue;
		}

		pUISkillUnit->DelStatus( SPUIUnit::STATUS_UI_UNIT_NO_DRAGNDROP );
		pUISkillUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_RENDER );

		pSkillUIInfo	=	pItemSkill->GetUIInfo();

		if( pSkillUIInfo != NULL )
		{
			if( pSkillUIInfo->bQuickSlotUse == true )
				pUISkillUnit->DelStatus( SPUIUnit::STATUS_UI_UNIT_NO_DRAGNDROP );
			else
				pUISkillUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_NO_DRAGNDROP );
		}

		SPPet *pet = g_pGOBManager->GetLocalPlayer()->GetPetMgr()->GetPet( m_iCurPetSelect );
		SPSkill *pSkill = SPSkillManager::GetInstance()->GetSkill( iSkillID );
		if (pSkill && pSkill->GetSpecialType() != SST_NULL)
		{
			bool bOn = pet->GetSkillStatus( pSkill->GetSpecialType() );
			pUISkillUnit->SetToggle( bOn );
		}

		if (!pet->IsNewGenerationPet())
		{
			if( g_pCheckManager->CheckItemSkillUseRequire( iSkillID , iItemID , stContainerSlot.ContainerType , stContainerSlot.SlotIndex , true ) == true )
			{
				pUISkillUnit->DelStatus( SPUIUnit::STATUS_UI_UNIT_DISABLE );
				pUISkillUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_ENABLE );
			}
			else
			{
				pUISkillUnit->DelStatus( SPUIUnit::STATUS_UI_UNIT_ENABLE );
				pUISkillUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_DISABLE );
			}
		}
		else
		{
			if (g_pCheckManager->CheckPetSkillUseRequire( iSkillID , iItemID , stContainerSlot.ContainerType , stContainerSlot.SlotIndex , true ) == true)
			{
				pUISkillUnit->DelStatus( SPUIUnit::STATUS_UI_UNIT_DISABLE );
				pUISkillUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_ENABLE );
			}
			else
			{
				pUISkillUnit->DelStatus( SPUIUnit::STATUS_UI_UNIT_ENABLE );
				pUISkillUnit->AddStatus( SPUIUnit::STATUS_UI_UNIT_DISABLE );
			}
			pUISkillUnit->SetType( SIT_NEWPET );
		}
	}
}

void SPWindowPet::SetPet(unsigned int uiClassID)
{
	ReleasePet();

	m_pkPet = new SPPet(1, (CLASS_ID)uiClassID, NULL);
	m_pkPet->SetJustRender(true);
	m_pkPet->Init();
	m_pkPet->SetJustRenderAnimation(ANI_MONSTER_RESERVE04, true);
}

void SPWindowPet::ReleasePet()
{
	SAFE_DELETE(m_pkPet);
}



//////////////////////////////////////////////////////////////////////////
SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowPet )
SPIMESSAGE_COMMAND(	SPIM_PET_STAT_UPDATE,	OnStatUpdate	)
SPIMESSAGE_COMMAND(	SPIM_PET_FEED_UPDATE,	OnFeedUpdate	)
SPIMESSAGE_COMMAND(	SPIM_ITEM_SETTING,		OnItemSetting	)
SPIMESSAGE_COMMAND(	SPIM_ITEM_ALLCLEAR ,	OnItemAllClear	)
SPIMESSAGE_COMMAND(	SPIM_ITEM_CLEAR ,		OnItemClear		)
SPIMESSAGE_COMMAND( SPIM_ITEM_SETSTACK,		OnItemSetStack	)
SPIMESSAGE_COMMAND( SPIM_ITEM_SELECT,		OnItemSetIconSelect)
SPIMESSAGE_COMMAND( SPIM_ITEM_DISABLE,		OnItemDisable)
SPIMESSAGE_COMMAND( SPIM_PET_SHOP_OPEN,		OnPetShop)
SPIMESSAGE_COMMAND( SPIM_NEWPET_STAT_UPDATE, onPetInfoUpdate );
SPIMESSAGE_COMMAND( SPIM_NEWPET_SHOP_OPEN,  OnNewPetShop );


SPIMESSAGE_COMMAND(	SPIM_MOVE_BEGIN,					OnBeginMove			)
SPIMESSAGE_COMMAND( SPIM_REFRESH			,			OnRefresh			)	
SPIMESSAGE_COMMAND(	SPIM_DRAGNDROP_END		,			OnDragnDropEnd		)

SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP	,	WIID_PET_SKILL_1	,	WIID_PET_ABILITY2	,	OnSkillLButtonUp	)
SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_DOWN	,	WIID_PET_SKILL_1	,	WIID_PET_ABILITY2	,	OnSkillLButtonDown	)
SPIMESSAGE_CONTROL_RANGE( SPIM_CURSOR_MOVE	,	WIID_PET_SKILL_1	,	WIID_PET_ABILITY2	,	OnSkillCursorMove	)
SPIMESSAGE_CONTROL_RANGE( SPIM_CURSOR_ENTER	,	WIID_PET_SKILL_1	,	WIID_PET_ABILITY2	,	OnSkillCursorEnter	)
SPIMESSAGE_CONTROL_RANGE( SPIM_CURSOR_OUT	,	WIID_PET_SKILL_1	,	WIID_PET_ABILITY2	,	OnSKillCursorOut	)

SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP	,	WIID_PET_PRIMARY	,	WIID_PET_TERTIARY	,	OnPetSelectClick	)

SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,	WIID_PET_EAT,			OnEat		)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,	WIID_PET_CLOSE,			OnClose		)
SPIMESSAGE_CONTROL(	SPIM_CURSOR_MOVE,	WIID_PET_GRID,			OnCursorMove)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_DOWN,	WIID_PET_GRID,			OnGridDown	)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,	WIID_PET_GRID,			OnGridUp	)

SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,	WIID_PET_FEED_ITEM_BUTTON,		OnFeed	)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,	WIID_PET_FEED_ITEM_BUTTON_2,	OnFeedEnergy )

SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,	WIID_PET_SKILL_1,		OnSkill1	)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,	WIID_PET_SKILL_2,		OnSkill2	)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,	WIID_PET_ABILITY,		OnAbility	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_PET_ABILITY2,		OnAbility2	)

SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_ENTER,	WIID_PET_GRID,			OnToolTipEnter	)
SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_OUT,	WIID_PET_GRID,			OnToolTipOut	)
SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_MOVE,	WIID_PET_GRID,			OnToolTipMove	)

SPIMESSAGE_CONTROL( SPIM_SETFOCUS, WIID_PET_NAME_EDITOR, onEditSetFocus )
SPIMESSAGE_CONTROL( SPIM_KILLFOCUS, WIID_PET_NAME_EDITOR, onEditKillFocus )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP, WIID_PET_NAME_BTN, onEditNameClick )

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)



int SPWindowPet::OnStatUpdate(WPARAM wParam, LPARAM lParam)
{
	ZeroMemory(&m_szXPInfo, 16);
	ZeroMemory(&m_szSPInfo, 16);

	PET_STAT_UPDATE iUpdate = (PET_STAT_UPDATE)wParam;

	// Pet Skill Update [12/19/2006 AJJIYA]
	if( iUpdate == PET_STAT_UPDATE_SP )
	{
		g_pInterfaceManager->SPChildWndSendMessage( WIID_QUICK_SLOT , SPIM_REFRESH , NULL , NULL );
		
		UpdatePetFeed();		
		for (int idx = 0; idx < PET_PLACE_TYPE_NUMBER; ++idx)
		{
			if(CheckPetFeed((PET_PLACE)idx)) {
				AutoPetEat((PET_PLACE)idx);
			}
			if (CheckPetFeedEnergy((PET_PLACE)idx))
			{
				AutoPetEatEnergy((PET_PLACE)idx);
			}
		}
	}

	if(m_bShow == false)
		return 1;

	if (m_iCurPetSelect != (PET_PLACE)lParam)
		return 1;
	
	SPItem	*&pPetItem = m_petInfo[m_iCurPetSelect].m_pPetItem;	
	//초기화
	if(pPetItem == NULL || pPetItem->GetItemStatus() == NULL || pPetItem->GetItemStatus()->GetItemID() == 0 || pPetItem->GetItemAttr() == NULL) {
		m_pSP->SPSendMessage(SPIM_SET_MAXVALUE, (WPARAM)ITEM_MAX_ITEMSP);
		m_pSP->SPSendMessage(SPIM_SET_CURVALUE, (WPARAM)0);		
		sprintf(m_szSPInfo, "0 %%");
		m_pSPInfo->SetWindowText(m_szSPInfo);
		
		m_pXP->SPSendMessage(SPIM_SET_MAXVALUE, (WPARAM)100);
		m_pXP->SPSendMessage(SPIM_SET_CURVALUE, (WPARAM)0);
		sprintf(m_szXPInfo, "0 %%");
		m_pXPInfo->SetWindowText(m_szXPInfo);

		m_pEnergy->SPSendMessage(SPIM_SET_MAXVALUE, (WPARAM)100);
		m_pEnergy->SPSendMessage(SPIM_SET_CURVALUE, (WPARAM)0);
		sprintf(m_szEnergyInfo, "0 %%");
		m_pEnergyInfo->SetWindowText(m_szEnergyInfo);
	}
	else
	{
		SPPlayer *pLocal = g_pGOBManager->GetLocalPlayer();
		if (pLocal->GetPetMgr()->GetPet( m_iCurPetSelect )
			&&	
			!pLocal->GetPetMgr()->GetPet( m_iCurPetSelect )->IsNewGenerationPet())
		{
			//XP, SP Gauge 설정
			m_pSP->SPSendMessage(SPIM_SET_MAXVALUE, (WPARAM)ITEM_MAX_ITEMSP);
			if(pPetItem->GetItemStatus()->GetSp() < 0) {
				m_pSP->SPSendMessage(SPIM_SET_CURVALUE, (WPARAM)0);
			}
			else if(pPetItem->GetItemStatus()->GetSp() > ITEM_MAX_ITEMSP) {
				m_pSP->SPSendMessage(SPIM_SET_CURVALUE, (WPARAM)ITEM_MAX_ITEMSP);
			}
			else {
				m_pSP->SPSendMessage(SPIM_SET_CURVALUE, (WPARAM)pPetItem->GetItemStatus()->GetSp());
			}

			float fSPPercent = ((float)pPetItem->GetItemStatus()->GetSp() / (float)ITEM_MAX_ITEMSP) * 100.0f;
			sprintf(m_szSPInfo, "%3.2f%%", fSPPercent);
			//sprintf(m_szSPInfo, "%d %%", fSPPercent);
			m_pSPInfo->SetWindowText(m_szSPInfo);

			m_pXP->SPSendMessage(SPIM_SET_MAXVALUE, (WPARAM)pPetItem->GetItemAttr()->m_iPetMaxExp);
			m_pXP->SPSendMessage(SPIM_SET_CURVALUE, (WPARAM)pPetItem->GetItemStatus()->GetExp());
			float fXPPercent = 0.0f;
			if(pPetItem->GetItemAttr()->m_iPetMaxExp > 0) {
				fXPPercent = ((float)pPetItem->GetItemStatus()->GetExp() / (float)pPetItem->GetItemAttr()->m_iPetMaxExp) * 100.0f;
			}	
			sprintf(m_szXPInfo, "%3.2f%%", fXPPercent);
			//sprintf(m_szXPInfo, "%d %%", fXPPercent);
			m_pXPInfo->SetWindowText(m_szXPInfo);

			m_pEnergy->SPSendMessage(SPIM_SET_MAXVALUE, (WPARAM)100);
			m_pEnergy->SPSendMessage(SPIM_SET_CURVALUE, (WPARAM)0);
			sprintf(m_szEnergyInfo, "0 %%");
			m_pEnergyInfo->SetWindowText(m_szEnergyInfo);
		}
	}

	UpdatePetTalk();

	// Pet Skill Update [12/19/2006 AJJIYA]
	if( iUpdate == PET_STAT_UPDATE_SP )
	{
		UpdateSkillUI();
	}

	return 1;
}


int SPWindowPet::OnFeedUpdate(WPARAM wParam, LPARAM lParam)
{
	UpdatePetFeed();
	return 1;
}


int SPWindowPet::OnEat(WPARAM wParam, LPARAM lParam)
{	
	SPPet *pPet = NULL;
	if (!g_pGOBManager->GetLocalPlayer() || !g_pGOBManager->GetLocalPlayer()->GetPetMgr())
		return 0;

	pPet = g_pGOBManager->GetLocalPlayer()->GetPetMgr()->GetPet( m_iCurPetSelect );
	if (!pPet)
		return 0;

	SPItem	*&pPetItem = m_petInfo[m_iCurPetSelect].m_pPetItem;	
	//펫을 장비하고 있는지
	if(pPetItem == NULL || pPetItem->GetItemStatus() == NULL || pPetItem->GetItemStatus()->GetItemID() == 0 || pPetItem->GetItemAttr() == NULL) {
		return 1;
	}
	
	SPPlayerInvenArchive* pInven = NULL;
	if(g_pGOBManager->GetLocalPlayer())
		pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	else 
		return 1;

	//사망시 제한
	if(g_pGOBManager->GetLocalPlayer()->GetCoordPhysics()->GetCurState() == GOB_STATE_DEAD) {		
		return 1;
	}

	//픽업 상태가 아닌경우 넘어간다
	if(pInven->m_bPickup == false) {
		return 1;
	}	

	SPSelectItem SelectItem = pInven->GetPickupItem();
	SPItem* pItem = pInven->GetInventoryItem(SelectItem.m_iContainer, SelectItem.m_iSlotIndex);	
	
	// 먹이로 사용할 아이템이 있는지
	if(pItem == NULL || pItem->GetItemStatus() == NULL || pItem->GetItemStatus()->GetItemID() == 0) {
		return 1;
	}
	
	if (SelectItem.GetContainerType() == INVENTORY_ETC)
	{
		// 펫이 시간제한에 걸린 경우
		if(pPetItem->GetItemStatus()->IsTimeAvailable() == false) {
			if(g_pResourceManager->GetGlobalString(11002001)) {
				std::string strMsg = g_pResourceManager->GetGlobalString(11002001);
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
			}
			return 1;
		}

		// 시간제한이 걸린 경우
		if(pItem->GetItemStatus()->IsTimeAvailable() == false) {
			if(g_pResourceManager->GetGlobalString(11002003)) {
				std::string strMsg = g_pResourceManager->GetGlobalString(11002003);
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
			}
			return 1;
		}

		// 펫 SP가 90이상인 경우
		if (pPet->IsNewGenerationPet())
		{
			if(pPet->GetPetAttr()->sp >= PET_SP_REPLETION) {
				if(g_pResourceManager->GetGlobalString(11002004)) {
					std::string strMsg = g_pResourceManager->GetGlobalString(11002004);
					g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
				}
				return 1;
			}
		}
		else
		{
			if(pPetItem->GetItemStatus()->GetSp() >= PET_SP_REPLETION) {
				if(g_pResourceManager->GetGlobalString(11002004)) {
					std::string strMsg = g_pResourceManager->GetGlobalString(11002004);
					g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
				}
				return 1;
			}
		}

		// 캐릭터가 서있는지?
		if(g_pGOBManager->GetLocalPlayer()) {
			if((g_pGOBManager->GetLocalPlayer()->GetCurAction() != ACTION_STOP)){
				if(g_pResourceManager->GetGlobalString(11002005)) {
					std::string strMsg = g_pResourceManager->GetGlobalString(11002005);
					g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
				}
				return false;
			}
		}

		if(g_pCheckManager->IsDBBlock()) 
			return false;

		// 패킷 전송
		CONTAINER_SLOT stPet;
		CONTAINER_SLOT stFeed;
		ZeroMemory(&stPet, sizeof(CONTAINER_SLOT));
		ZeroMemory(&stFeed, sizeof(CONTAINER_SLOT));

		stPet.ContainerType = pPetItem->GetContainerType();
		stPet.SlotIndex = pPetItem->GetSlotIndex() + 1;	
		stFeed.ContainerType = pItem->GetContainerType();
		stFeed.SlotIndex = pItem->GetSlotIndex() + 1;
		if(stPet.SlotIndex < 1 || stFeed.SlotIndex < 1) {
			return 1;
		}	

		//CONTAINER_SLOT		stMainItemPos;	
		// ITEMNO				iMainItemNo;
		//CONTAINER_SLOT		stMaterialItemPos;
		// ITEMNO				iMaterialItemNo;

		CPacket Packet;
		Packet.Add((UINT32)ITEM_CS_ADDSP);
		Packet.AddData(&stPet, sizeof(CONTAINER_SLOT));
		INT64 iPetItemNo = *(ITEMNO*)pPetItem->GetItemStatus()->SPSendMessage( MV_ITEM_GET_ITEMNO );	
		Packet.AddUINT64(iPetItemNo);
		
		Packet.AddData(&stFeed, sizeof(CONTAINER_SLOT));
		INT64 iFeedItemNo = *(ITEMNO*)pItem->GetItemStatus()->SPSendMessage( MV_ITEM_GET_ITEMNO );
		Packet.AddUINT64(iFeedItemNo);	
		
		g_pNetworkManager->SPPerformMessage(ITEM_CS_ADDSP, 0, (LPARAM)&Packet);
		return 1;
	}
	else if (IsItemCashFood(pItem->GetItemStatus()->GetItemID()))
 	{
		if (!pPet->IsNewGenerationPet())
		{
			// not new pet
			if(g_pResourceManager->GetGlobalString(11002002)) {
				std::string strMsg = g_pResourceManager->GetGlobalString(11002002);
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
			}
			return 1;
		}

		// eat for energy
		CONTAINER_SLOT stFeed;
		ZeroMemory(&stFeed, sizeof(CONTAINER_SLOT));

		stFeed.ContainerType = SelectItem.m_iContainer;
		stFeed.SlotIndex = SelectItem.m_iSlotIndex + 1;
		if(stFeed.SlotIndex < 1) {
			return 1;
		}

		SPPet *pet = g_pGOBManager->GetLocalPlayer()->GetPetMgr()->GetPet( m_iCurPetSelect );
		if (!pet)
			return 1;

		CPacket packet;
		packet.Add((UINT32)PET_CS_ADD_SSP);
		packet.AddData(&stFeed, sizeof(CONTAINER_SLOT));
		INT64 iPetItemNo = pet->GetItemNoID();
		packet.AddUINT64( iPetItemNo );

		g_pNetworkManager->SPPerformMessage( PET_CS_ADD_SSP, 0, (LPARAM)&packet );
		return 1;
	}

	// 기타아이템이 아닌경우
	if( SelectItem.GetContainerType() != INVENTORY_ETC ) {
		if(g_pResourceManager->GetGlobalString(11002002)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(11002002);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}
		return 1;
	}
	
	return 1;
}

int SPWindowPet::OnPetShop(WPARAM wParam, LPARAM lParam)
{
	int iSkill = (int)wParam;

	if(m_pPetShop) {
		if(m_pPetShop->IsShow() == false) {			
			m_pPetShop->SetPetSlot(m_iCurPetSelect);
			m_pPetShop->Show();
			m_pPetShop->SetSkillID(iSkill);
		}
		else {
			m_pPetShop->Hide();
		}
	}
	return 1;
}

int SPWindowPet::OnNewPetShop(WPARAM wParam, LPARAM lParam)
{
	/*
	int iSkill = (int)wParam;
	
	if(m_pPetShop) {
		if(m_pPetShop->IsShow() == false) {			
			m_pPetShop->Show();
			m_pPetShop->SetSkillID(iSkill);
		}
		else {
			m_pPetShop->Hide();
		}
	}
	return 1;
	*/
	if (!g_pGOBManager->GetLocalPlayer())
		return 0;
	SPPetManager *pPetMgr = g_pGOBManager->GetLocalPlayer()->GetPetMgr();
	if (!pPetMgr)
	{
		return 0;
	}
	SPPet *pet = pPetMgr->GetPet( m_iCurPetSelect );
	if (!pet)
		return 0;

	if (pet->GetOpenShopStatus())
	{
		if (!m_petInfo[m_iCurPetSelect].m_pPetItem)
			return 0;

		CPacket Packet;
		Packet.Add((UINT32)PET_CS_CLOSE_PET_SHOP);
		Packet.AddUINT32(g_pNetworkManager->GetUserID());
		Packet.AddUINT32(g_pGOBManager->GetLocalPlayer()->GetInstanceID());
		Packet.AddUINT64(pet->GetItemNoID());
		g_pNetworkManager->SPPerformMessage(PET_CS_CLOSE_PET_SHOP, 0, (LPARAM)&Packet);
	}
	else
	{
		SPWindowPetShopSeller *pPetShopSeller = (SPWindowPetShopSeller*)g_pInterfaceManager->FindWindow( WIID_PETSHOP_SELL );
		pPetShopSeller->SetPet( pPetMgr->GetPet( m_iCurPetSelect ), m_iCurPetSelect ); 
		pPetShopSeller->Show();
	}

	return 1;
}


int SPWindowPet::OnItemSetting(WPARAM wParam, LPARAM lParam)
{
	CONTAINER_ITEM *item; // = new CONTAINER_ITEM;
	item = (CONTAINER_ITEM*)wParam;

	POINT ptSrcPos;	
	SPTexture* texture = g_pItemCluster->GetItemTexture(item->Item.iItemID, ptSrcPos); 	

	//Grid 에 대한 세팅	
	if ((int)item->SlotInfo.SlotIndex != m_iCurPetSelect)
	{
		m_iCurPetSelect = (PET_PLACE)item->SlotInfo.SlotIndex;
		reloadTabImg();
	}

	if(m_pGrid)
		m_pGrid->SettingIconImage( texture, item->Item.iItemID, ptSrcPos.x, ptSrcPos.y, (SLOT_INDEX)0, item->Item.iStackCount, (bool)lParam);

	CONTAINER_TYPE iContainer = item->SlotInfo.ContainerType;
	int iSlotIndex = item->SlotInfo.SlotIndex;

	SPPlayerInvenArchive* pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	SPItem	*&pPetItem = m_petInfo[m_iCurPetSelect].m_pPetItem;
	SPItem	*&pPetFeed = m_petInfo[m_iCurPetSelect].m_pPetFeed;
	if(pInven) {

		//--------------------------------------------------
		if (item->Item.iItemNo != m_petInfo[m_iCurPetSelect].item.Item.iItemNo)
		{
			ClearPetFeed();
			ClearPetFeedEnergy();
		}
		//--------------------------------------------------


		pPetItem = pInven->GetInventoryItem(iContainer, iSlotIndex);
		if(pPetItem && pPetItem->GetItemStatus() && pPetItem->GetItemStatus()->GetItemID() != 0) {
			if (!m_petInfo[m_iCurPetSelect].m_pPetItem)
			{
				if(pPetFeed && pPetFeed->GetItemStatus() && pPetFeed->GetItemStatus()->GetItemID() && pPetFeed->GetItemStatus()->GetStackCount() > 0){
					ClearPetFeed();
					ClearPetFeedEnergy();
				}			
			}
			if (!g_pGOBManager->GetLocalPlayer() || !g_pGOBManager->GetLocalPlayer()->GetPetMgr())
				return 0;
			SPPet *pPet = g_pGOBManager->GetLocalPlayer()->GetPetMgr()->GetPet( m_iCurPetSelect );
			if (pPet && pPet->IsNewGenerationPet())
			{
				char strPetName[512] = {0,};
				if (pPet->GetPetAttr()->szPetName[0] != '\0')
				{
					_snprintf( strPetName, LEN_NAME, "%s", pPet->GetPetAttr()->szPetName );
					_snprintf( strPetName, 511, "%s(Lv.%d)", strPetName, pPet->GetPetAttr()->lv );
				}
				else
					_snprintf( strPetName, 511, "%s(Lv.%d)", pPetItem->GetItemAttr()->m_strName.c_str(), pPet->GetPetAttr()->lv );

				m_pName->SetWindowText( strPetName );
			}
			else
				m_pName->SetWindowText( pPetItem->GetItemAttr()->m_strName.c_str() );
			m_pCurPetTalk = FindPetTalk(pPetItem->GetItemStatus()->GetItemID());

			if (pPet)
			{
				SetPet( pPet->GetClassID() );
				m_petInfo[m_iCurPetSelect].m_iPetClassID = pPet->GetClassID();
				m_petInfo[m_iCurPetSelect].item = *item;
			}
			else
			{
				SPItemAttr* pItemAttr = pPetItem->GetItemAttr();
				if( pItemAttr && pItemAttr->m_iPetClassID )
				{
					SetPet(pItemAttr->m_iPetClassID);
					m_petInfo[m_iCurPetSelect].m_iPetClassID = pItemAttr->m_iPetClassID;
					m_petInfo[m_iCurPetSelect].item = *item;
				}
			}
		}
	}

	OnStatUpdate(PET_STAT_UPDATE_SP, (int)m_iCurPetSelect);

	return 1;
}


int SPWindowPet::OnItemClear(WPARAM wParam, LPARAM lParam)
{
	int iSlotIndex = (int)0;	
	
	if(m_pGrid){
		m_pGrid->ClearCell(iSlotIndex);	
		m_pName->SetWindowText(NULL);
		//m_pDesc->SetWindowText(NULL);
	}
	
	m_petInfo[m_iCurPetSelect].m_pPetItem = NULL;
	m_petInfo[m_iCurPetSelect].m_iPetClassID = 0;
	m_pCurPetTalk = NULL;
	ClearPetFeed();
	ClearPetFeedEnergy();
	OnStatUpdate(PET_STAT_UPDATE_SP, m_iCurPetSelect);
	ReleasePet();
	m_pName->Show();
	m_pNameEditor->Hide();

	return 1;
}


int SPWindowPet::OnItemAllClear(WPARAM wParam, LPARAM lParam)
{
	if(m_pGrid)
		m_pGrid->ClearCellAll() ;
	
	m_pName->SetWindowText(NULL);
	//m_pDesc->SetWindowText(NULL);

	for (int idx = 0; idx < PET_PLACE_TYPE_NUMBER; ++idx)
	{
		m_iCurPetSelect = (PET_PLACE)idx;
		m_petInfo[idx].reset();
		m_pCurPetTalk = NULL;
		ClearPetFeed();
		ClearPetFeedEnergy();
		OnStatUpdate(0, m_iCurPetSelect);		// 여기서는 지울때만 호출하기때문에 QuickSlot에서 업데이트를 하지 않는다. [4/11/2007 AJJIYA]
	}
	ReleasePet();
	m_pName->Show();
	m_pNameEditor->Hide();
	
	return 1;
}


int SPWindowPet::OnItemSetStack(WPARAM wParam, LPARAM lParam)
{
	int iIndex = (int)wParam /*- 1*/;
	short iStackCount = (short)lParam;	
	
	if(m_pGrid)
		m_pGrid->SetItemStack(iIndex, iStackCount);
	return 1;
}


int SPWindowPet::OnClose(WPARAM wParam, LPARAM lParam)
{
	Hide();
	return 1;
}


int SPWindowPet::OnItemSetIconSelect(WPARAM wParam, LPARAM lParam)
{
	int iSlotIndex = static_cast<int>(wParam);
	bool bSelect = static_cast<int>(lParam);

	if(m_pGrid)
		m_pGrid->CellSelectSell(iSlotIndex, bSelect);

	return 1;
}


int SPWindowPet::OnItemDisable(WPARAM wParam, LPARAM lParam)
{
	int iSlotIndex = static_cast<int>(wParam);
	bool bAble = static_cast<bool>(lParam);

	if(m_pGrid)
		m_pGrid->CellEnable(iSlotIndex, bAble);

	return 1;
}

int SPWindowPet::OnBeginMove( WPARAM wParam, LPARAM lParam)
{
	int				iXPos		=	g_pInterfaceManager->GetCursorX();
	int				iYPos		=	g_pInterfaceManager->GetCursorY();
	SPUIUnit*		pUIUnit		=	m_UIUnitManager.GetUnitMouseOver( iXPos , iYPos );

	if( pUIUnit == NULL )
	{
		return SPWindow::OnBeginMove(wParam, lParam);
	}

	return 0;
}

int SPWindowPet::OnCursorMove(WPARAM wParam, LPARAM lParam)
{
	if(g_pGOBManager->GetLocalPlayer())
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_PET_CURSOR_MOVE , (LPARAM)lParam);	
	
	return 1;
}


int SPWindowPet::OnGridDown(WPARAM wParam, LPARAM lParam)
{
	int iSlotIndex = (int)m_iCurPetSelect;
	
	if(iSlotIndex != 0)  {
		if(g_pGOBManager->GetLocalPlayer())
			g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
		//return 1;
	}

	if(g_pGOBManager->GetLocalPlayer())	// metalgeni
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_PET_CLICK_DOWN , (LPARAM)iSlotIndex);

	return 1;
}


int SPWindowPet::OnGridUp(WPARAM wParam, LPARAM lParam)
{
	int iSlotIndex = (int)lParam;
	
	if(iSlotIndex != 0)  {
		if(g_pGOBManager->GetLocalPlayer())
			g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
		return 1;
	}

	if(g_pGOBManager->GetLocalPlayer())	// metalgeni
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_PET_CLICK_UP , (LPARAM)m_iCurPetSelect);
	return 1;
}


int SPWindowPet::OnSkill1( WPARAM wParam, LPARAM lParam )
{

	return 1;
}


int SPWindowPet::OnSkill2( WPARAM wParam, LPARAM lParam )
{

	return 1;
}


int SPWindowPet::OnAbility( WPARAM wParam, LPARAM lParam )
{

	return 1;
}

int SPWindowPet::OnAbility2(WPARAM wParam, LPARAM lParam)
{
	return 1;
}


int SPWindowPet::OnFeed(WPARAM wParam, LPARAM lParam)
{
	SPItem	*&pPetItem = m_petInfo[m_iCurPetSelect].m_pPetItem;
	SPItem	*&pPetFeed = m_petInfo[m_iCurPetSelect].m_pPetFeed;
	//펫을 장비하고 있는지
	if(pPetItem == NULL || pPetItem->GetItemStatus() == NULL || pPetItem->GetItemStatus()->GetItemID() == 0 || pPetItem->GetItemAttr() == NULL) {
		return 1;
	}

	SPPlayerInvenArchive* pInven = NULL;
	if(g_pGOBManager->GetLocalPlayer())
		pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	else 
		return 1;

	//사망시 제한
	if(g_pGOBManager->GetLocalPlayer()->GetCoordPhysics()->GetCurState() == GOB_STATE_DEAD) {		
		return 1;
	}

	////현재 자동 먹이로 등록한 아이템이 있는 경우
	//if(m_pPetItem->IsEmptyItem() == false) {
	//	ClearPetFeed();
	//}

	//픽업 상태가 아닌경우 등록된 아이템이 있으면 초기화
	if(pInven->m_bPickup == false && pPetItem->IsEmptyItem() == false) {
		ClearPetFeed();
		return 1;
	}

	SPSelectItem SelectItem = pInven->GetPickupItem();
	
	// 기타아이템이 아닌경우
	if( SelectItem.GetContainerType() != INVENTORY_ETC ) {
		if(g_pResourceManager->GetGlobalString(11002002)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(11002002);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}
		return 1;
	}

	// 펫이 시간제한에 걸린 경우
	if(pPetItem->GetItemStatus()->IsTimeAvailable() == false) {
		if(g_pResourceManager->GetGlobalString(11002001)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(11002001);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}
		return 1;
	}

	//기존에 장비하고 있는 펫 먹이가 있는 경우
	if(pPetFeed && pPetFeed->GetItemStatus() && pPetFeed->GetItemStatus()->GetItemID() && pPetFeed->GetItemStatus()->GetStackCount() > 0){
		ClearPetFeed();
	}
	
	pPetFeed = pInven->GetInventoryItem(SelectItem.m_iContainer, SelectItem.m_iSlotIndex);
	
	// 먹이로 사용할 아이템이 있는지
	if(pPetFeed == NULL || pPetFeed->GetItemStatus() == NULL || pPetFeed->GetItemStatus()->GetItemID() == 0) {
		return 1;
	}

	// 시간제한이 걸린 경우
	if(pPetFeed->GetItemStatus()->IsTimeAvailable() == false) {
		if(g_pResourceManager->GetGlobalString(11002003)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(11002003);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}
		return 1;
	}
	
	UpdatePetFeed();
	
	if(CheckPetFeed(m_iCurPetSelect)) {
		AutoPetEat(m_iCurPetSelect);
	}

	return 1;
}

int SPWindowPet::OnFeedEnergy(WPARAM wParam, LPARAM lParam)
{
	SPPet *pPet = NULL;
	if (g_pGOBManager->GetLocalPlayer() && g_pGOBManager->GetLocalPlayer()->GetPetMgr())
	{
		pPet = g_pGOBManager->GetLocalPlayer()->GetPetMgr()->GetPet( m_iCurPetSelect );
	}
	else
	{
		return 0;
	}

	if (!pPet || !pPet->IsNewGenerationPet())
	{
		if(g_pResourceManager->GetGlobalString(11002002)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(11002002);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}
		return 0;
	}

	SPItem	*&pPetItem = m_petInfo[m_iCurPetSelect].m_pPetItem;
	SPItem	*&pPetFeed = m_petInfo[m_iCurPetSelect].m_pPetFeedEnergy;
	if(pPetItem == NULL || pPetItem->GetItemStatus() == NULL || pPetItem->GetItemStatus()->GetItemID() == 0 || pPetItem->GetItemAttr() == NULL) {
		return 1;
	}

	SPPlayerInvenArchive* pInven = NULL;
	if(g_pGOBManager->GetLocalPlayer())
		pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	else 
		return 1;

	if(g_pGOBManager->GetLocalPlayer()->GetCoordPhysics()->GetCurState() == GOB_STATE_DEAD) {		
		return 1;
	}

	if(pInven->m_bPickup == false && pPetItem->IsEmptyItem() == false) {
		ClearPetFeedEnergy();
		return 1;
	}

	SPSelectItem SelectItem = pInven->GetPickupItem();

	/*if( SelectItem.GetContainerType() != INVENTORY_ETC ) {
		if(g_pResourceManager->GetGlobalString(11002002)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(11002002);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}
		return 1;
	}*/

	if(pPetItem->GetItemStatus()->IsTimeAvailable() == false) {
		if(g_pResourceManager->GetGlobalString(11002001)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(11002001);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}
		return 1;
	}

	if(pPetFeed && pPetFeed->GetItemStatus() && pPetFeed->GetItemStatus()->GetItemID() && pPetFeed->GetItemStatus()->GetStackCount() > 0){
		ClearPetFeedEnergy();
	}

	pPetFeed = pInven->GetInventoryItem(SelectItem.m_iContainer, SelectItem.m_iSlotIndex);

	if(pPetFeed == NULL || pPetFeed->GetItemStatus() == NULL || pPetFeed->GetItemStatus()->GetItemID() == 0) {
		return 1;
	}

	if (!IsItemCashFood(pPetFeed->GetItemStatus()->GetItemID()))
	{
		if(g_pResourceManager->GetGlobalString(11002002)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(11002002);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}
		pPetFeed = NULL;
		return 1;
	}

	if(pPetFeed->GetItemStatus()->IsTimeAvailable() == false) {
		if(g_pResourceManager->GetGlobalString(11002003)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(11002003);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}
		return 1;
	}

	UpdatePetFeed();

	if(CheckPetFeedEnergy(m_iCurPetSelect)) {
		AutoPetEatEnergy(m_iCurPetSelect);
	}

	return 1;
}

int SPWindowPet::OnRefresh( WPARAM wParam, LPARAM lParam )
{
	UpdateSkillUI();
	return 1;

	//ReloadPetInfo();
}


int SPWindowPet::OnDragnDropEnd( WPARAM wParam, LPARAM lParam )
{
	m_bLMouseDown	=	FALSE;
	m_lParam		=	0;

	g_pInterfaceManager->RegisterSoundUnit( "DATA/SOUND/SYSTEM_ICON_PUT_DOWN.WAV" );

	return 1;
}


int SPWindowPet::OnSkillLButtonUp( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	if( m_bLMouseDown == TRUE )
	{
		int			iXPos	=	LOWORD(lParam);
		int			iYPos	=	HIWORD(lParam);
		SPUIUnit*	pUIUnit	=	m_UIUnitManager.GetUnitMouseOver( iXPos , iYPos );
		if ( pUIUnit == NULL )
			return 0;

		SPUISkillUnit *pSkillUnit = (SPUISkillUnit*)pUIUnit;
		int nSkillID = pSkillUnit->GetSkillID();
		SPSkill *pSkill = SPSkillManager::GetInstance()->GetSkill( nSkillID );
		SKILL_SPECIAL_TYPE eSpecialType = SST_NULL;
		eSpecialType = pSkill->GetSpecialType();
		if (g_pGOBManager->GetLocalPlayer() == NULL)
			return 0;
		SPPetManager *pPetMgr = g_pGOBManager->GetLocalPlayer()->GetPetMgr();
		if (!pPetMgr)
			return 0;
		SPPet *pet = pPetMgr->GetPet( m_iCurPetSelect );
		if (pet && pSkill && pSkill->GetSpecialType() != SST_NULL)
		{
			eSpecialType = pSkill->GetSpecialType();
			if (eSpecialType == SST_ITEM_SELL)
			{
				OnPetShop( (WPARAM)nSkillID, (LPARAM)0 );
			}
			else if (eSpecialType == SST_PET_SHOP)
			{
				OnNewPetShop( (WPARAM)nSkillID, (LPARAM)0 );
			}
			else if (eSpecialType == SST_PET_CHAT)
			{
				pet->SwitchSkillStatus( eSpecialType );
				SPWindowPetChatEditor *pWnd = (SPWindowPetChatEditor*)g_pInterfaceManager->FindWindow( WIID_PETCHAT_EDITOR );
				if (!pWnd)
					return 0;
				if (pet->GetSkillStatus( eSpecialType))
				{
					pWnd->SetPetIndex( m_iCurPetSelect );
					pWnd->Show();
				}
				else
				{
					pWnd->Hide();
				}
			}
			else
			{
				pet->SwitchSkillStatus( eSpecialType );
			}

			UpdateSkillUI();
		}
		else
		{
			if( pUIUnit != NULL )
				pUIUnit->Use();
		}

		//--------------------------------------------------
		//// 흔벎角별뺐세콘,컸척뎠별뺐槨역폘珂,槨늪UIUNIT속흙...
		//SPUISkillUnit *pSkillUnit = (SPUISkillUnit*)pUIUnit;
		//int	nSkillID = pSkillUnit->GetSkillID();
		//SPSkill *pSkill = SPSkillManager::GetInstance()->GetSkill( nSkillID );
		//if (pSkill)
		//{
		//	if (pSkill->GetSpecialType() == SST_PET_CHAT)
		//	{
		//		SPPet *pet = g_pGOBManager->GetLocalPlayer()->GetPetMgr()->GetPet( m_iCurPetSelect );
		//		if (pet)
		//		{
		//			if (pet->GetAutoChat())
		//			{
		//				pSkillUnit->SetToggle( false );
		//				pet->SetAutoChat( false );
		//				pet->SetSpecialSkill( SST_PET_CHAT, nSkillID, false, true);
		//			}
		//			else
		//			{
		//				pSkillUnit->SetToggle( true );
		//				pet->SetAutoChat( true );
		//				pet->SetSpecialSkill( SST_PET_CHAT, nSkillID, true, true );
		//			}
		//		}
		//	}
		//}
		//--------------------------------------------------
	}

	OnDragnDropEnd( wParam , lParam );


	return 1;
}


int SPWindowPet::OnSkillLButtonDown( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	int	iXPos		=	LOWORD(lParam);
	int	iYPos		=	HIWORD(lParam);
	int	iUnitNumber	=	m_UIUnitManager.GetNumberMouseOver( iXPos ,iYPos );

	if( iUnitNumber >= 0 )
	{
		m_bLMouseDown	=	TRUE;
		m_lParam		=	lParam;
	}

	return 1;
}


int SPWindowPet::OnSkillCursorMove( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	if( IsShow() == FALSE )
		return 1;

	int			iXPos			=	LOWORD(lParam);
	int			iYPos			=	HIWORD(lParam);
	int			iXPosOld		=	LOWORD(m_lParam);
	int			iYPosOld		=	HIWORD(m_lParam);
	int			iUnitNumber		=	m_UIUnitManager.GetNumberMouseOver( iXPosOld , iYPosOld );
	SPUIUnit*	pUIUnit			=	m_UIUnitManager.GetUnit( iUnitNumber );
	BOOL		bIsDragging		=	SPDragnDropManager::GetInstance()->IsDragging();

	if( pUIUnit == NULL || bIsDragging == TRUE )
		return 1;

	int	iXStep	=	abs( iXPos - iXPosOld );
	int iYStep	=	abs( iYPos - iYPosOld );

	if( bIsDragging == FALSE )
	{
		if( m_bLMouseDown == TRUE && ( iXStep > 5 || iYStep > 5 ) )
		{
			if( SPDragnDropManager::GetInstance()->Set( this , pUIUnit ) == FALSE )
			{
				m_bLMouseDown	=	FALSE;
				m_lParam		=	0;
			}
		}
	}

	return 1;
}


int SPWindowPet::OnSkillCursorEnter( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	int				iXPos			=	LOWORD(lParam);
	int				iYPos			=	HIWORD(lParam);
	SPUIUnit*		pUIUnit			=	m_UIUnitManager.GetUnitMouseOver( iXPos , iYPos );
	SPWindow*		pWindow			=	g_pInterfaceManager->GetToolTipWindow();

	if( pUIUnit == NULL )
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT , NULL , SPWindowToolTip::TOOLTIP_SHOW_TYPE_UIUNIT );
	else
		pWindow->SPSendMessage( SPIM_TOOLTIP_ENTER , (WPARAM)pUIUnit , SPWindowToolTip::TOOLTIP_SHOW_TYPE_UIUNIT );

	return 1;
}


int SPWindowPet::OnSKillCursorOut( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	if( SPDragnDropManager::GetInstance()->IsDragging() == FALSE )
	{
		m_bLMouseDown	=	FALSE;
		m_lParam		=	0;
	}

	SPWindow*	pWindow =	g_pInterfaceManager->GetToolTipWindow();

	if( pWindow != NULL )
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT , NULL , SPWindowToolTip::TOOLTIP_SHOW_TYPE_UIUNIT );

	return 1;
}


int SPWindowPet::OnToolTipEnter(WPARAM wParam, LPARAM lParam)
{
	CONTAINER_TYPE iContainerType = FIGURE_PET;
	int iSlotIndex = (int)m_iCurPetSelect;

	//iSlotIndex += (m_iCurPage * STORAGE_LINE);

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


int SPWindowPet::OnToolTipOut(WPARAM wParam, LPARAM lParam)
{
	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
	pWindow->Show();

	return 1;
}


int SPWindowPet::OnToolTipMove(WPARAM wParam, LPARAM lParam)
{
	CONTAINER_TYPE iContainerType = FIGURE_PET;
	int iSlotIndex = (int)m_iCurPetSelect;	

	//iSlotIndex += (m_iCurPage * STORAGE_LINE);

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
void setSelected( SPWindowButton *btn, bool bSelect )
{
	if (btn)
		btn->SPSendMessage( SPIM_BTN_SELECT, bSelect );
}
int SPWindowPet::OnPetSelectClick(unsigned int iID, WPARAM wParam , LPARAM lParam)
{
	int petIndex = iID - WIID_PET_PRIMARY;
	m_iCurPetSelect = (PET_PLACE)petIndex;
	reloadTabImg();
	ReloadPetInfo();
	return 1;
}

void SPWindowPet::reloadTabImg(void)
{
	setSelected( m_pBtnPrimary, false );
	setSelected( m_pBtnSecondary, false );
	setSelected( m_pBtnTertiary, false );

	switch (m_iCurPetSelect)
	{
	case PET_PRIMARY:
		setSelected( m_pBtnPrimary, true );
		break;
	case PET_SECONDARY:
		setSelected( m_pBtnSecondary, true );
		break;
	case PET_TERTIARY:
		setSelected( m_pBtnTertiary, true );
		break;
	default:
		setSelected( m_pBtnPrimary, true );
		break;
	}

	m_pNameEditor->Hide();
	m_pName->Show();
}
void SPWindowPet::ReloadPetInfo( void )
{
	if (!g_pGOBManager->GetLocalPlayer() || !g_pGOBManager->GetLocalPlayer()->GetPetMgr())
		return;

	if (m_petInfo[m_iCurPetSelect].m_iPetClassID > 0)
	{
		OnItemSetting( (WPARAM)&(m_petInfo[m_iCurPetSelect].item), (LPARAM)true ); 
		UpdateSkillUI();
		if (g_pGOBManager->GetLocalPlayer()->GetPetMgr()->GetPet(m_iCurPetSelect))
		{
			if (g_pGOBManager->GetLocalPlayer()->GetPetMgr()->GetPet(m_iCurPetSelect)->IsNewGenerationPet())
			{
				onPetInfoUpdate( m_iCurPetSelect, (LPARAM)(g_pGOBManager->GetLocalPlayer()->GetPetMgr()->GetPet(m_iCurPetSelect)->GetPetAttr()));
			}
		}
	}
	else
	{
		//OnItemAllClear( NULL, NULL );
		OnItemClear(NULL,NULL);
		UpdateSkillUI();
	}
}
bool SPWindowPet::LoadCashFoodCfg( void )
{
	const char *strFileName = "DATA/LDT/PET_FOOD.LDT";
	SPLDTFile	*pFile = NULL;
	g_pResourceManager->GetLDTFromFileName( strFileName, &pFile );
	if (!pFile)
		return false;

	m_vecCashFood.clear();
	LDT_Field field;
	int iRecordCount = pFile->GetItemCount();
	int id = 0;
	for (int idx = 0; idx < iRecordCount; ++idx)
	{
		id = pFile->GetPrimaryKey( idx );
		if (id == 0)
			continue;

		pFile->GetFieldFromLable( id, "_ItemID", field );
		m_vecCashFood.push_back( field.uData.lValue );
	}
	return true;
}
bool SPWindowPet::IsItemCashFood( int itemID )
{
	std::vector< int >::iterator iter = 
		find( m_vecCashFood.begin(), m_vecCashFood.end(), itemID );
	if (iter != m_vecCashFood.end())
		return true;
	else
		return false;
}
//--------------------------------------------------
int SPWindowPet::onEditSetFocus(WPARAM wParam, LPARAM lParam)
{
	g_pInterfaceManager->SetKeyInput( TRUE );
	return 0;
}
int SPWindowPet::onEditKillFocus(WPARAM wParam, LPARAM lParam)
{
	g_pInterfaceManager->SetKeyInput( FALSE );
	return 0;
}
int SPWindowPet::onEditNameClick(WPARAM wParam, LPARAM lParam)
{
	assert( m_pBtnNameEditor && m_pNameEditor );
	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();
	if( pLocalPlayer == NULL )
		return 0;

	SPPet *pet = pLocalPlayer->GetPetMgr()->GetPet( m_iCurPetSelect );
	if (!pet)
		return 0;

	if (m_pNameEditor->IsShow())
	{
		//--------------------------------------------------
		// if new name is blank or equal to old name, just hide
		//const char *szNewName = m_pNameEditor->GetWindowText();
		char szNewName[LEN_NAME+1] = {0,};
		_snprintf( szNewName, LEN_NAME, "%s", m_pNameEditor->GetWindowText() );
		char szOldName[LEN_NAME+1] = {0,};
		if (pet->GetPetAttr())
			memcpy( szOldName, pet->GetPetAttr()->szPetName, LEN_NAME );
		if (!szNewName || szNewName[0] == '\0')
		{
			m_pNameEditor->Hide();
			m_pName->Show();
			return 0;
		}
		if (strcmp( szNewName, szOldName ) == 0)
		{
			m_pNameEditor->Hide();
			m_pName->Show();
			return 0;
		}
		//--------------------------------------------------

		//--------------------------------------------------
		// send msg to server, modify pet's name
		//--------------------------------------------------
		SPPlayerInvenArchive*	pInvenArchive	=	pLocalPlayer->GetInvenArchive();
		if( pInvenArchive == NULL )
			return false;

		SPItem*	pRequireItem	=	pInvenArchive->GetSPItem( INVENTORY_CONSUME , 180130001 );
		if (!pRequireItem)
		{
			// say something
			if(g_pResourceManager->GetGlobalString(13001445)) {
				std::string strMsg = g_pResourceManager->GetGlobalString(13001445);
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
			}
			return 1;
		}

		char szBuffer[128];
		int iReplace;
		SPAbuseFilter::GetInstance()->Filter(szNewName, szBuffer, 127, &iReplace);
		if(iReplace) {
			if(g_pResourceManager->GetGlobalString(12000012)) {
				std::string strMsg = g_pResourceManager->GetGlobalString(13001443);
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
			}
			return 1;
		}	

		CONTAINER_SLOT Slot;
		Slot.ContainerType = pRequireItem->GetContainerType();
		Slot.SlotIndex = pRequireItem->GetSlotIndex() + 1;

		CPacket packet;
		packet.AddUINT32((UINT32)PET_CS_CHANGE_NAME);
		packet.AddData( &Slot, sizeof(Slot) );
		packet.AddUINT64((UINT64)pet->GetItemNoID());
		packet.AddData( szNewName, LEN_NAME ); 
		g_pNetworkManager->SPPerformMessage( PET_CS_CHANGE_NAME, (WPARAM)0, (LPARAM)&packet );

		m_pNameEditor->Hide();
		m_pName->Show();
	}
	else
	{
		m_pName->Hide();
		m_pNameEditor->Show();
	}

	return 0;
}
int SPWindowPet::onPetInfoUpdate( WPARAM wParam, LPARAM lParam )
{
	PET_PLACE iPlace = (PET_PLACE)wParam;
	for (int idx = 0; idx < PET_PLACE_TYPE_NUMBER; ++idx)
	{
		if (CheckPetFeed((PET_PLACE)idx))
			AutoPetEat((PET_PLACE)idx);
		if (CheckPetFeedEnergy((PET_PLACE)idx))
			AutoPetEatEnergy((PET_PLACE)idx);
	}

	if (!lParam)
		return 0;
	t_PetAttr_Info	&info = *(t_PetAttr_Info*)lParam;
	if (iPlace != m_iCurPetSelect)
		return 0;

	//--------------------------------------------------
	// update pet's info, as name, lv, sp, xp, energy etc.
	//--------------------------------------------------
	SPItem *pItem = m_petInfo[m_iCurPetSelect].m_pPetFeed;
	if (pItem && pItem->GetItemStatus() && pItem->GetItemStatus()->GetStackCount() < 1)
		m_pPetFeedIcon->SPSendMessage(SPIM_SET_IMAGECLEAR);

	SPItem *pItemEnergy = m_petInfo[m_iCurPetSelect].m_pPetFeedEnergy;
	if (pItemEnergy && pItemEnergy->GetItemStatus() && pItemEnergy->GetItemStatus()->GetStackCount() < 1)
		m_pPetFeedIcon2->SPSendMessage(SPIM_SET_IMAGECLEAR);

	// name
	char strPetName[512] = {0,};
	if (info.szPetName[0] != '\0')
	{
		_snprintf( strPetName, LEN_NAME, "%s", info.szPetName );
		_snprintf( strPetName, 511, "%s(Lv.%d)", strPetName, info.lv );
		m_pName->SetWindowText( strPetName );
	}
	else // if pet's name is null
	{
		SPItem	*&pPetItem = m_petInfo[m_iCurPetSelect].m_pPetItem;
		if (pPetItem)
		{
			_snprintf( strPetName, 511, "%s(Lv.%d)", pPetItem->GetItemAttr()->m_strName.c_str(), info.lv );
			m_pName->SetWindowText( strPetName );
		}
	}

	//sp xp ssp
	//--------------------------------------------------
	m_pSP->SPSendMessage( SPIM_SET_MAXVALUE, (WPARAM)ITEM_MAX_ITEMSP );
	if (info.sp < 0)
		m_pSP->SPSendMessage( SPIM_SET_CURVALUE, (WPARAM)0);
	else if (info.sp > ITEM_MAX_ITEMSP)
		m_pSP->SPSendMessage( SPIM_SET_CURVALUE, (WPARAM)ITEM_MAX_ITEMSP );
	else
		m_pSP->SPSendMessage( SPIM_SET_CURVALUE, (WPARAM)info.sp );

	float fSPPercent = ((float)info.sp/ (float)ITEM_MAX_ITEMSP) * 100.0f;
	sprintf(m_szSPInfo, "%3.2f%%", fSPPercent);
	m_pSPInfo->SetWindowText(m_szSPInfo);

	//--------------------------------------------------
	int MAX_XP = SPPetLevelMgr::getInstance().GetXPForPet( info.lv );
	float fXPPercent = 0.0f;
	if (MAX_XP == 0)
	{
		m_pXP->SPSendMessage( SPIM_SET_MAXVALUE, (WPARAM)1);
		m_pXP->SPSendMessage( SPIM_SET_CURVALUE, (WPARAM)1);
		fXPPercent = 100.0f;
	}
	else
	{
		m_pXP->SPSendMessage( SPIM_SET_MAXVALUE, (WPARAM)MAX_XP );
		m_pXP->SPSendMessage( SPIM_SET_CURVALUE, (WPARAM)info.exp );
		if (info.exp > 0)
		{
			fXPPercent = ((float)info.exp / (float)MAX_XP) * 100.0f;
		}
	}
	_snprintf( m_szXPInfo, 15, "%3.2f%%", fXPPercent );
	m_pXPInfo->SetWindowText( m_szXPInfo );

	//--------------------------------------------------
	const int MAX_PET_ENERGY = 10000;
	m_pEnergy->SPSendMessage( SPIM_SET_MAXVALUE, (WPARAM)MAX_PET_ENERGY );
	if (info.ssp < 0)
		m_pEnergy->SPSendMessage( SPIM_SET_CURVALUE, (WPARAM)0 );
	else if (info.ssp > MAX_PET_ENERGY)
		m_pEnergy->SPSendMessage( SPIM_SET_CURVALUE, (WPARAM)MAX_PET_ENERGY );
	else
		m_pEnergy->SPSendMessage( SPIM_SET_CURVALUE, (WPARAM)info.ssp );

	float fEnergyPercent = ((float)info.ssp / (float)MAX_PET_ENERGY) * 100.0f;
	_snprintf( m_szEnergyInfo, 15, "%3.2f%%", fEnergyPercent );
	m_pEnergyInfo->SetWindowText( m_szEnergyInfo );
	//--------------------------------------------------
	UpdateSkillUI();
	//--------------------------------------------------

	return 1;
}
//--------------------------------------------------
