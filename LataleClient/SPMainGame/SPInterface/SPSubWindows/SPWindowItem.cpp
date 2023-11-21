
#include <vector>

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


#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPWindowSlider.h"
#include "SPWindowGrid.h"
#include "SPWindowToolTip.h"

#include "SPUIUnitManager.h"

#include "SPWindowEnchant.h"
#include "SPWindowItem.h"
#include "SPWindowItemUpgrade.h"
#include "SPWindowItemTimeAdd.h"
#include "SPWindowPhantomLevelUp.h"
#include "SPWindowPetLevelUp.h"
#include "SPWindowCashHotList.h"

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

#include "SPEventDEF.h"
#include "SPEventManager.h"

#include "SPUIUnit.h"				// AJJIYA [7/18/2005]
#include "SPUIItemUnit.h"			// AJJIYA [7/18/2005]
#include "SPDragnDropManager.h"		// AJJIYA [7/18/2005]

#include "SPHotItemManager.h"
#include "SPCashTradeManager.h"

#include "SPLocalizeManager.h"
#include "SPWindowPushprop.h"

#include "SPDebug.h"

SPWindowItem::SPWindowItem(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
{
	Init();
}

SPWindowItem::SPWindowItem(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_ITEM, InstanceID, iX, iY, iCX, iCY, pParent)	
{
	Init();
}

SPWindowItem::~SPWindowItem()
{
	Clean();
}

void SPWindowItem::Init()
{
	m_bShow = false;	
	
	SPWindow* pTop = NULL;
	SPWindow* pCenter = NULL;
	SPWindow* pBottom = NULL;
	SPWindow* pSkin = NULL;
	
	SPWindow* pStatic = NULL;	

	//Skin Top
	pTop = new SPWindowStatic(WIID_ITEM_TOP, 0, 0, 198, 4, this);
	pTop->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 1, 46);
	pTop->Show();

	//Skin Center
	pCenter = new SPWindowStatic(WIID_ITEM_CENTER, 0, 4, 198, 268, this);
	pCenter->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 1, 52);
	pCenter->SetSrcSize(199, 2);
	pCenter->Show();

	//Skin Bottom
	pBottom = new SPWindowStatic(WIID_ITEM_BOTTOM, 0, 272, 198, 4, this);
	pBottom->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 1, 56);
	pBottom->Show();

	//Title Image
	pStatic = new SPWindowStatic(WIID_ITEM_TITLE_IMAGE, 7, 3, 30, 23, this);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 33, 343);
	pStatic->Show();
	pStatic = NULL;

	//Title
	pStatic = new SPWindowStatic(WIID_ITEM_TITLE, 42, 7, 69, 15, this);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 69, 367);
	pStatic->Show();
	pStatic = NULL;

	//Center Skin
	pSkin = new SPWindowStatic(WIID_ITEM_SKIN, 2, 63-4, 194 - 1 , 182, pCenter);
	pSkin->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 349, 321);
	pSkin->SetSrcSize(6, 6);
	pSkin->Show();

	//Center Skin Up Line
	pStatic = new SPWindowStatic(WIID_ITEM_SKIN_UP_LINE, 2, 62-4, 194, 1, pCenter);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 349, 329);
	pStatic->SetSrcSize(6, 6);
	pStatic->Show();
	pStatic = NULL;

	//Center Skin Down Line
	pStatic = new SPWindowStatic(WIID_ITEM_SKIN_DOWN_LINE, 2, 245-4, 194, 1, pCenter);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 349, 329);
	pStatic->SetSrcSize(6, 6);
	pStatic->Show();
	pStatic = NULL;	

	//close
	SPWindowButton* pButton;	
	pButton = new SPWindowButton( WIID_ITEM_HIDE, 177, 9 - 4, 11, 11, pCenter);
	pButton->Show();
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI102.PNG", 247,	217);	
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI102.PNG", 247,	230);	
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI102.PNG", 247,	243);
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI102.PNG", 247,	256);	
	m_pHide = pButton;
	pButton = NULL;

	//Ä«Å×°úÔ® ¹öÆ° ±×?E¹Ù´Ú
	SPWindow* pGroup = NULL;
	pGroup = new SPWindowStatic(WIID_ITEM_GROUP, 2, 26 - 4, 194 - 1, 36, pCenter);
	pGroup->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 349,  312);
	pGroup->SetSrcSize(6, 6);
	pGroup->Show();
	
	//
	pButton = new SPWindowButton( WIID_ITEM_EQUIP, 4 - 2 , 28 - 26, 64, 15, pGroup);
	pButton->Show();
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI102.PNG", 1,  448);
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI102.PNG", 1,  464);	
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI102.PNG", 1,	 480);
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI102.PNG", 1,	 496);
	pButton->SetImageCheck	("DATA/INTERFACE/CONCEPT/UI102.PNG", 1,	 480);
	pButton->SPSendMessage(SPIM_BTN_SELECT, TRUE		);
	pButton = NULL;

	pButton = new SPWindowButton( WIID_ITEM_CONSUME, 67 - 2, 28 - 26, 64, 15, pGroup);
	pButton->Show();
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI102.PNG", 66,	448);
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI102.PNG", 66,	464);	
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI102.PNG", 66,	480);
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI102.PNG", 66,	496);	
	pButton->SetImageCheck	("DATA/INTERFACE/CONCEPT/UI102.PNG", 66,	480);
	pButton = NULL;

	pButton = new SPWindowButton( WIID_ITEM_ETC, 130 - 2, 28 - 26, 64, 15, pGroup);
	pButton->Show();	
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI102.PNG", 131,	448);
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI102.PNG", 131,	464);	
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI102.PNG", 131,	480);
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI102.PNG", 131,	496);	
	pButton->SetImageCheck	("DATA/INTERFACE/CONCEPT/UI102.PNG", 131,	480);
	//pButton->SetEnable(false);
	pButton = NULL;

	pButton = new SPWindowButton( WIID_ITEM_EVENT, 4 - 2 , 45 - 26, 64, 15, pGroup);
	pButton->Show();	
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI102.PNG", 196,	448);
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI102.PNG", 196,	464);	
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI102.PNG", 196,	480);
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI102.PNG", 196,	496);
	pButton->SetImageCheck	("DATA/INTERFACE/CONCEPT/UI102.PNG", 196,	480);
	//pButton->SetEnable(false);
	pButton = NULL;

	//pButton = new SPWindowButton( WIID_ITEM_PET, 130 -2 , 45 - 26, 64, 15, pGroup);
	pButton = new SPWindowButton( WIID_ITEM_PET, 67 -2 , 45 - 26, 64, 15, pGroup);
	pButton->Show();	
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI102.PNG", 326,	448);
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI102.PNG", 326,	464);	
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI102.PNG", 326,	480);
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI102.PNG", 326,	496);	
	pButton->SetImageCheck	("DATA/INTERFACE/CONCEPT/UI102.PNG", 326,	480);
	//pButton->SetEnable(false);
	pButton = NULL;

	////
	//pButton = new SPWindowButton( WIID_ITEM_HOUSING, 67 - 2, 45 - 26, 64, 15, pGroup);
	//pButton->Show();	
	//pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI102.PNG", 261,	448);
	//pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI102.PNG", 261,	464);	
	//pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI102.PNG", 261,	480);
	//pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI102.PNG", 261,	496);	
	//pButton->SetImageCheck	("DATA/INTERFACE/CONCEPT/UI102.PNG", 261,	480);
	////pButton->SetEnable(false);
	//pButton = NULL;

	pButton = new SPWindowButton( WIID_ITEM_LINE_UP, 172 - 2, 71 - 59, 12, 11, pSkin);
	pButton->Show();
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI102.PNG", 460,	458);
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI102.PNG", 473,	458);	
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI102.PNG", 486,	458);
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI102.PNG", 499,	458);	
	pButton = NULL;

	pButton = new SPWindowButton( WIID_ITEM_LINE_DOWN, 172 - 2, 208 - 59, 12, 11, pSkin);
	pButton->Show();
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI102.PNG", 460,	500);
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI102.PNG", 473,	500);	
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI102.PNG", 486,	500);
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI102.PNG", 499,	500);	
	pButton = NULL;		

	////
	//SPWindowGrid*	pGrid;
	//pGrid = new SPWindowGrid(WNDID_CTRL_GRID, WIID_ITEM_GRID, 6, 60, 136, 136, pSkin, 4, 4, 34, 34, 32);
	//pGrid->SetImage("");
	//pGrid->ClearCellAll();
	//m_pGrid = pGrid;
	//pGrid = NULL;	

	//SPWindowGrid*	pGrid;
	m_pGrid = new SPWindowGrid(WNDID_CTRL_GRID, WIID_ITEM_GRID, 14-2, 72-63, 152, 152, pSkin, 4, 4, 38, 38, 32);
	//pGrid->SetImage("");
	m_pGrid->SetGridImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 358, 382);
	m_pGrid->ClearCellAll();
	//m_pGrid = pGrid;
	//pGrid = NULL;

	m_pSlider	=	new SPWindowSlider(WIID_ITEM_SLIDER, 172 - 2, 82 - 59, 12, 126, pSkin );
	RECT rtRect;

	SetRect( &rtRect , 460 , 470 , 472 , 494 );	//	ÀÏ¹Ý
	m_pSlider->SetImageHandle( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	SetRect( &rtRect , 473 , 470 , 485 , 494 );	//	¸¶?E?¿À?E
	m_pSlider->SetImageHandleHit( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	SetRect( &rtRect , 486 , 470 , 498 , 494 );	//	¸¶?E?Å¬¸¯
	m_pSlider->SetImageHandlePush( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	SetRect( &rtRect , 499 , 470 , 511 , 494 );
	m_pSlider->SetImageHandleDisable( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	SetRect( &rtRect , 460 , 496 , 472 , 498 );	//	ÀÏ¹Ý
	m_pSlider->SetImagePageNormal( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	SetRect( &rtRect , 473 , 496 , 485 , 498 );	//	¸¶?E?¿À?E
	m_pSlider->SetImagePageHit( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	SetRect( &rtRect , 486 , 496 , 498 , 498 );	//	¸¶?E?Å¬¸¯
	m_pSlider->SetImagePageShadow( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );

	SetRect( &rtRect , 499 , 496 , 511 , 498 );
	m_pSlider->SetImagePageDisable( "DATA/INTERFACE/CONCEPT/UI102.PNG" , rtRect );


	m_pSlider->SPSendMessage( SPIM_SLIDER_PAGESIZE , 1 );
	m_pSlider->SetCurpage( 0 );
	m_pSlider->SPSendMessage( SPIM_SET_MAXVALUE , 0 );

	// AJJIYA [7/18/2005]
	m_pSelectItem	=	new SPUIItemUnit;
	m_pSelectItem->SetBGTexture( "DATA/INTERFACE/CONCEPT/UI100.PNG" );
	m_pSelectItem->SetBGRectSrc( 400 , 20 , 432  , 52 );
//	m_pSelectItem->SetBGRectSrc( 465 , 0 , 497 , 32 );
//	m_pSelectItem->SetPos( iXPos , iYPos );
	m_pSelectItem->SetSize( 32 , 32 );
	m_pSelectItem->SetDisableTexture( "DATA/INTERFACE/CONCEPT/UI100.PNG" );
	m_pSelectItem->SetDisableRectSrc( 434 , 37 , 436 , 39 );
	m_pSelectItem->SetCoolTimeTexture( "DATA/INTERFACE/CONCEPT/UI100.PNG" );
	m_pSelectItem->SetCoolTimeRectSrc( 434 , 37 , 436 , 39 );
	m_pSelectItem->AddStatus( SPUIUnit::STATUS_UI_UNIT_ENABLE );
	m_pSelectItem->AddStatus( SPUIUnit::STATUS_UI_UNIT_RENDER );
	m_pSelectItem->SetInstanceID( WIID_ITEM );

	m_bUseItemEnchant		 =	SPLocalizeManager::GetInstance()->IsEnableValue( LV_ITEMENCHANT );
	m_bUseItemUpgrade		 =	SPLocalizeManager::GetInstance()->IsEnableValue( LV_ITEMUPGRADE  );
	m_bUesItemTimeAdd		 =	SPLocalizeManager::GetInstance()->IsEnableValue( LV_EXTENDEDTERMITEM );
	m_bUseItemPetLevelUp	 =	SPLocalizeManager::GetInstance()->IsEnableValue( LV_PET );
	m_bUseItemPhantomLevelUp =  SPLocalizeManager::GetInstance()->IsEnableValue( LV_PHANTOM );
	//m_bUseItemPhantomLevelUp = true;//»ÃÓ°

	pButton = new SPWindowButton( WIID_ITEM_USERSHOP, 6, 248 - 4, 24, 24, pCenter);
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI100.PNG", 423, 385);	
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI100.PNG", 448, 385);	
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI100.PNG", 423, 410);
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI100.PNG", 448, 410);	
	m_pUserShop = pButton;

	pButton = new SPWindowButton( WIID_ITEM_ENCHANT, 32, 248 - 4, 24, 24, pCenter);
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI102.PNG", 461, 359);	
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI102.PNG", 461, 384);	
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI102.PNG", 461, 409);
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI102.PNG", 461, 434);	
	pButton->SetEnable( m_bUseItemEnchant );
	m_pEnchant = pButton;

	pButton = new SPWindowButton( WIID_ITEM_ITEMUPGRADE, 58, 248 - 4, 24, 24, pCenter);
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI102.PNG", 486, 359);	
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI102.PNG", 486, 384);	
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI102.PNG", 486, 409);
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI102.PNG", 486, 434);	
	pButton->SetEnable( m_bUseItemUpgrade );
	m_pUpGrade = pButton;

	pButton = new SPWindowButton( WIID_ITEM_TIMEADD_BUTTON, 84, 248 - 4, 24, 24, pCenter);
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI102.PNG", 333, 373);	
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI102.PNG", 333, 398);	
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI102.PNG", 333, 423);
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI102.PNG", 256, 102);	
	pButton->SetEnable( m_bUesItemTimeAdd );
	m_pTimeAdd = pButton;
	
	pButton = new SPWindowButton(WIID_ITEM_PET_LEVELUP, 110, 248 - 4, 24, 24, pCenter);
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI104.PNG",	235,	61);
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI104.PNG",	235,	86); //on
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI104.PNG",	235,	111); //push
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI104.PNG",	235,	136); //disable
	pButton->SetEnable( m_bUseItemPetLevelUp );
	m_pPetLevelUP	=	pButton;
	
	//»ÃÓ° ý¼¶°´Å?
	pButton = new SPWindowButton(WIID_ITEM_EFFECTITEM_UPGRADE, 136, 248 - 4, 24, 24, pCenter);
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/CNUI01.PNG",	77,		1);
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/CNUI01.PNG",	102,	1); //on
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/CNUI01.PNG",	127,	1); //push
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CNUI01.PNG",	152,	1); //disable
	pButton->SetEnable( m_bUseItemPhantomLevelUp );
	m_pPhantomLevelUp	=	pButton;

	pStatic = new SPWindowStatic( WIID_ITEM_ELY_BASE, 79, 227 - 4, 105, 14, pCenter);
	pStatic->SetImage		("DATA/INTERFACE/CONCEPT/UI100.PNG", 77, 497);
	pStatic = NULL;

	pStatic = new SPWindowStatic( WIID_ITEM_ELY, 81, 229 - 4, 78, 12, pCenter);
	pStatic->SetFontColor(RGBA(49, 76, 97, 255));
	pStatic->SetFormat(DT_RIGHT | DT_VCENTER);
	m_pEly = pStatic;
	pStatic = NULL;
}

void SPWindowItem::Clean()
{
	SPWindow::Clean();

	SAFE_DELETE( m_pSelectItem );
}

void SPWindowItem::Process(float fTime)
{
	if(m_pEly && g_pGOBManager->GetLocalPlayer()) {
		m_pEly->SetWindowText(ConvertMoneyToString(g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValueEX(STATUS_EX_ELY)));
	}
	SPWindow::Process(fTime);	
}

void SPWindowItem::Render(float fTime)
{
	SPWindow::Render(fTime);	
	SPWindowPushProp *wnd = (SPWindowPushProp*)g_pInterfaceManager->FindWindow( WIID_PUSH_PROP );
	if (wnd)
		wnd->SpecialRender(fTime);
}


void SPWindowItem::Show(bool bWithChild)
{
	SPWindow *wnd = g_pInterfaceManager->FindWindow( WIID_PUSH_PROP );
	if (wnd)
		wnd->Show();

	if(m_bShow == false) {
		// Á×¾ûØ­ ¸·¾Ò½À´Ï´Ù - metalgeni.
		if(g_pGOBManager->GetLocalPlayer()) {
			g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_ITEM_REFRESH_GRID , 0);
		}	
	}
	
	//m_pGrid->AdjustCoord(6, 60);
	m_pSlider->SPSendMessage( SPIM_POS_UPDATE );		//[2006/4/12] ½½¶óÀÌ´õ´Â µû·Î À§Ä¡ ÀçÁ¶Á¤À» ÇØÁà¾ß ÇÑ´Ù
	SPWindow::Show(bWithChild);	
	
	SPWindow* pkWindow = NULL;

	bool bShowHotList = false;
	//pkWindow = Find(WIID_CASH_HOTLIST, true);	
	//if(pkWindow->IsShow()) {
	//	bShowHotList = true;
	//}
	
	///
	//pkWindow = Find(WIID_ENCHANT);
	//if( pkWindow ) {
	//	pkWindow->Hide();
	//}

	//pkWindow = Find( WIID_ITEM_UPGRADE );
	//if( pkWindow ) {
	//	pkWindow->Hide();
	//}

	//pkWindow = Find( WIID_ITEM_TIMEADD );
	//if(pkWindow) {		
	//	pkWindow->Hide();
	//}

	//pkWindow = Find( WIID_PET_LEVELUP );
	//if(pkWindow) {
	//	pkWindow->Hide();
	//}

	UpdateSubWindowPos(bShowHotList);

	if(g_pEventManager->GetCutInState() == CUT_IN_STATE_NULL) {
		m_pUserShop->SetEnable(true);
		m_pEnchant->SetEnable(m_bUseItemEnchant);
		m_pUpGrade->SetEnable(m_bUseItemUpgrade);
		m_pTimeAdd->SetEnable(m_bUesItemTimeAdd);
		m_pPetLevelUP->SetEnable(m_bUseItemPetLevelUp);
		m_pPhantomLevelUp->SetEnable( m_bUseItemPhantomLevelUp );//»ÃÓ° ý¼?
	}

	if( m_pPetLevelUP != NULL )
	{
		if( m_bUseItemPetLevelUp == true )
			m_pPetLevelUP->Show();
		else
			m_pPetLevelUP->Hide();
	}

	//»ÃÓ° ý¼?
	//if( m_pPhantomLevelUp != NULL )
	//{
	//	if( m_bUseItemPhantomLevelUp == true )
	//		m_pPhantomLevelUp->Show();
	//	else
	//		m_pPhantomLevelUp->Hide();
	//}

}

void SPWindowItem::Hide(bool bSendServer)
{
	SPWindow *wnd = g_pInterfaceManager->FindWindow( WIID_PUSH_PROP );
	if (wnd)
		wnd->Hide();

	if(m_bShow) {
		g_pInterfaceManager->RegisterSoundUnit("DATA/SOUND/SYSTEM_CLOSE_WINDOW.WAV");
		//g_pInterfaceManager->ForceReply();					//[2005/10/7]
	}

	////[2008/2/1]-jinhee °³ÀÎ»óÁ¡ °ú Áßº¹ ½½·ÔÀ» ÀÌ¿ëÇÏ±â ¶§¹®¿¡ ½Ã°£¿¬Àå½Ã ÃÊ±âÈ­´Â ¿©±â¼­
	//SPWindow* pkWindow = NULL;
	//pkWindow = Find( WIID_ITEM_TIMEADD );
	//if(pkWindow && pkWindow->IsShow()) {
	//	if(g_pGOBManager->GetLocalPlayer()) {
	//		SPPlayerInvenArchive* pInven = NULL;
	//		if(g_pGOBManager->GetLocalPlayer()) {
	//			pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	//			pInven->ClearUserShopItem(0);
	//			pInven->ClearUserShopItem(1);
	//			pInven->ClearUserShopItem(2);
	//		}
	//	}
	//}
	
	SPWindow::Hide(bSendServer);
	
	OnToolTipOut(0, 0);

	//	AJJIYA [9/8/2005]
	//	Ã¢ Á¤·ÄÀ» À§ÇØ¼­!!
	g_pInterfaceManager->OrderWindowPosMove();

	m_iPrevSlotIndex	=	-1;
}


SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowItem )
	SPIMESSAGE_COMMAND(	SPIM_CURSOR_ENTER,		OnCursorEnter	)
	SPIMESSAGE_COMMAND(	SPIM_CURSOR_OUT,		OnCursorOut		)
	SPIMESSAGE_COMMAND(	SPIM_LBUTTON_UP,		OnLButtonUp		)	
	SPIMESSAGE_COMMAND(	SPIM_LBUTTON_DOWN,		OnLButtonDown	)
	//SPIMESSAGE_COMMAND(	SPIM_TOOLTIP_ENTER,		OnToolTipEnter	)
	//SPIMESSAGE_COMMAND(	SPIM_TOOLTIP_OUT,		OnToolTipOut	)
	//SPIMESSAGE_COMMAND(	SPIM_TOOLTIP_MOVE,		OnToolTipMove	)

	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP, WIID_ITEM_TOP		,	OnLButtonUp	)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP, WIID_ITEM_CENTER	,	OnLButtonUp	)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP, WIID_ITEM_BOTTOM	,	OnLButtonUp	)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP, WIID_ITEM_GROUP	,	OnLButtonUp	)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP, WIID_ITEM_SKIN	,		OnLButtonUp	)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP, WIID_ITEM_SKIN_UP_LINE	,	OnLButtonUp	)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP, WIID_ITEM_SKIN_DOWN_LINE ,	OnLButtonUp	)

	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP, WIID_ITEM_EQUIP	,	OnEquip		)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP, WIID_ITEM_CONSUME	,	OnConsumable)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP, WIID_ITEM_ETC		,	OnEtc		)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP, WIID_ITEM_EVENT	,	OnEvent		)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP, WIID_ITEM_PET		,	OnPet		)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP, WIID_ITEM_HOUSING	,	OnHousing	)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP, WIID_ITEM_HIDE		,	OnHide		)		
	
	SPIMESSAGE_CONTROL(	SPIM_CURSOR_MOVE,	WIID_ITEM_GRID, 	OnCursorMove)

	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_DOWN,	WIID_ITEM_GRID,		OnGridDown	)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,	WIID_ITEM_GRID,		OnGridUp	) //

	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_DBLCLICK,	WIID_ITEM_GRID,	OnGridDBK	) //[2005/10/27]
	
	SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_ENTER,	WIID_ITEM_GRID,		OnToolTipEnter	)
	SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_OUT,	WIID_ITEM_GRID,		OnToolTipOut	)
	SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_MOVE,	WIID_ITEM_GRID,		OnToolTipMove	)

	SPIMESSAGE_CONTROL(	SPIM_RBUTTON_UP,	WIID_ITEM_GRID		,OnUse	)	
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,	WIID_ITEM_ENCHANT		,OnItemEnchantKeyUp	)	
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,	WIID_ITEM_ITEMUPGRADE	,OnItemUpgradeKeyUp	)	
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,	WIID_ITEM_USERSHOP		,OnOpenUserShop	)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,	WIID_ITEM_TIMEADD_BUTTON	,OnItemAddTime )
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,	WIID_ITEM_PET_LEVELUP		,OnPetLevelUP )
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,    WIID_ITEM_EFFECTITEM_UPGRADE   ,OnPhantomLevelUp )//»ÃÓ° ý¼?

	//////////////////////////////////////////////////////////////////////////	
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_ITEM_LINE_UP,		OnButtonPageUp		)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_ITEM_LINE_DOWN,	OnButtonPageDown	)
	
	SPIMESSAGE_COMMAND( SPIM_WHEEL_UP,									OnButtonPageUp		)
	SPIMESSAGE_COMMAND( SPIM_WHEEL_DOWN,								OnButtonPageDown	)
	SPIMESSAGE_CONTROL( SPIM_WHEEL_UP,			WIID_ITEM_GRID,			OnButtonPageUp		)
	SPIMESSAGE_CONTROL( SPIM_WHEEL_DOWN,		WIID_ITEM_GRID,			OnButtonPageDown	)
	
	SPIMESSAGE_CONTROL( SPIM_SLIDER_UP		,	WIID_ITEM_SLIDER,		OnSliderPageUp	)
	SPIMESSAGE_CONTROL( SPIM_SLIDER_DOWN	,	WIID_ITEM_SLIDER,		OnSliderPageDown	)
	SPIMESSAGE_CONTROL( SPIM_SLIDER_PAGEUP,		WIID_ITEM_SLIDER,		OnSliderPageUp	)
	SPIMESSAGE_CONTROL( SPIM_SLIDER_PAGEDOWN,	WIID_ITEM_SLIDER,		OnSliderPageDown)
	SPIMESSAGE_CONTROL( SPIM_SET_CURVALUE,		WIID_ITEM_SLIDER,		OnSliderPageNum	)
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_ITEM_SLIDER,		OnSliderButtonUp)
	//SPIMESSAGE_CONTROL( SPIM_SET_CURVALUE,		WIID_ITEM_SLIDER,		

	//////////////////////////////////////////////////////////////////////////	
	SPIMESSAGE_COMMAND(	SPIM_ITEM_UNLOCK,		OnItemClearAllUserShopItem	)
	SPIMESSAGE_COMMAND(	SPIM_ITEM_SETTING,		OnItemSetting	)
	SPIMESSAGE_COMMAND(	SPIM_ITEM_ALLCLEAR ,	OnItemAllClear	)
	SPIMESSAGE_COMMAND(	SPIM_ITEM_CLEAR ,		OnItemClear		)
	SPIMESSAGE_COMMAND( SPIM_ITEM_SETSTACK,		OnItemSetStack	)

	SPIMESSAGE_COMMAND( SPIM_ITEM_SETDEFAULT,	OnSetDefault	) //[2005/11/27] Ã¤³Îº¯°æ½Ã ÅÇ¹öÆ° ÃÊ±âÈ­
	SPIMESSAGE_COMMAND( SPIM_ITEM_LINE,			OnSetLine		) //[2006/12/21] ÀÎº¥Åä¸® ¶óÀÎ¼ö°¡ »ý¼º ¼öº¸´Ù ÀÛÀº °æ¿ì ¼³Á¤

	SPIMESSAGE_COMMAND( SPIM_SET_EXIT_SHOW,		OnExitHide		)
	SPIMESSAGE_COMMAND( SPIM_ITEM_SUBMENU,		OnSubMenu		)

	SPIMESSAGE_COMMAND( SPIM_ITEM_SELECT,		OnItemSellSelect)	//[2005/5/20]
	SPIMESSAGE_COMMAND( SPIM_ITEM_DISABLE,		OnItemDisable)		//[2005/5/20]
	
	SPIMESSAGE_COMMAND( SPIM_ITEM_SET_MAXPAGE,	OnSetMaxPage	) //[2005/7/6]	
	SPIMESSAGE_COMMAND( SPIM_ITEM_SET_PAGE,		OnSetPage		) //[2005/7/6]		

	SPIMESSAGE_COMMAND(	SPIM_DRAGNDROP_END	,	OnDragnDropEnd	)		// AJJIYA [7/18/2005]

	SPIMESSAGE_COMMAND(	SPIM_ITEM_ENCHANT_KEYUP	,	OnItemEnchantKeyUp	)		// AJJIYA [5/22/2006]
	SPIMESSAGE_COMMAND(	SPIM_ITEM_UPGRADE_KEYUP	,	OnItemUpgradeKeyUp	)		// AJJIYA [5/22/2006]	

	SPIMESSAGE_COMMAND(	SPIM_ITEM_HOT_STATUS,	OnSetHotStatus )

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)


int SPWindowItem::OnCursorEnter( WPARAM, LPARAM )
{
	return 1;
}

int SPWindowItem::OnCursorOut( WPARAM, LPARAM )
{
	return 1;
}

int SPWindowItem::OnLButtonDown( WPARAM, LPARAM )
{
	return 1;
}

int SPWindowItem::OnLButtonUp( WPARAM, LPARAM )
{
	if(g_pGOBManager->GetLocalPlayer())
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
	
	return 1;
}

int SPWindowItem::OnRButtonDown( WPARAM, LPARAM )
{
	return 1;
}

int SPWindowItem::OnRButtonUP( WPARAM, LPARAM )
{
	return 1;
}


int SPWindowItem::OnHide( WPARAM wParam, LPARAM lParam )		//[2005/7/8]
{
	if(g_pGOBManager->GetLocalPlayer()) {
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);		
	}	

	return SPWindow::OnHide(wParam, lParam);
}



//////////////////////////////////////////////////////////////////////////
int SPWindowItem::OnButtonPageUp( WPARAM wParam, LPARAM lParam )
{
	//if(g_pGOBManager->GetLocalPlayer())
	//	g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
	
	m_pSlider->SPSendMessage( SPIM_SLIDER_UP );
	return 1;
}


int SPWindowItem::OnButtonPageDown( WPARAM wParam, LPARAM lParam )
{
	//if(g_pGOBManager->GetLocalPlayer())
	//	g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);

	m_pSlider->SPSendMessage( SPIM_SLIDER_DOWN );
	return 1;
}


int SPWindowItem::OnSliderPageUp( WPARAM wParam, LPARAM lParam )
{
	//DXUTOutputDebugString("ItemSliderPageUp[%d]\n", (int)wParam);
	
	if(g_pGOBManager->GetLocalPlayer()) {
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_ITEM_PAGE_UP , 0);

		//SPWindow* pWindow = Find(WIID_ITEM_GRID);
		//pWindow->SPSendMessage(SPIM_TOOLTIP_MOVE, 0, MAKELONG(g_pInterfaceManager->GetCursorX(), g_pInterfaceManager->GetCursorY()));
		m_pGrid->SPSendMessage(SPIM_TOOLTIP_MOVE, 0, MAKELONG(g_pInterfaceManager->GetCursorX(), g_pInterfaceManager->GetCursorY()));
	}

	return 1;
}


int SPWindowItem::OnSliderPageDown( WPARAM wParam, LPARAM lParam )
{
	//DXUTOutputDebugString("ItemSliderPageDown[%d]\n", (int)wParam);

	if(g_pGOBManager->GetLocalPlayer()) {
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_ITEM_PAGE_DOWN , 0);
		
		//SPWindow* pWindow = Find(WIID_ITEM_GRID);		
		//pWindow->SPSendMessage(SPIM_TOOLTIP_MOVE, 0, MAKELONG(g_pInterfaceManager->GetCursorX(), g_pInterfaceManager->GetCursorY()));
		m_pGrid->SPSendMessage(SPIM_TOOLTIP_MOVE, 0, MAKELONG(g_pInterfaceManager->GetCursorX(), g_pInterfaceManager->GetCursorY()));
	}
	return 1;
}


int SPWindowItem::OnSliderPageNum( WPARAM wParam, LPARAM lParam )
{
	//DXUTOutputDebugString("OnSliderPageNum[%d]\n", (int)wParam);
	if(g_pGOBManager->GetLocalPlayer())
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_ITEM_SETPAGE , wParam);
	return 1;
}


int SPWindowItem::OnSliderButtonUp( WPARAM wParam, LPARAM lParam )
{
	if(g_pGOBManager->GetLocalPlayer())
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);

	return 1;
}



//////////////////////////////////////////////////////////////////////////
int SPWindowItem::OnCursorMove(WPARAM wParam, LPARAM lParam)
{		
	if(g_pGOBManager->GetLocalPlayer())	// metalgeni
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_ITEM_CURSOR_MOVE , (LPARAM)lParam);

	CheckDragNDrop();
	
	return 1;
}


int SPWindowItem::OnToolTipEnter( WPARAM wParam, LPARAM lParam) 
{
	// by metalgeni
	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return 0;

	CONTAINER_TYPE iContainerType = g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetCurrentItemPage();
	
	int iSlotIndex = (int)lParam;
	iSlotIndex += (g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetInventoryCurPage(iContainerType) * INVENTORY_LINE);

	if( m_iPrevSlotIndex == iSlotIndex )
		return 1;

	m_iPrevSlotIndex	=	iSlotIndex;

	int iPickUpContainer = g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetPickupItem().GetContainerType();
	int iPickUpSlotIndex = g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetPickupItem().GetSlotIndex();
	
	//Pick Up Item?E°°Àº ¾ÆÀÌÅÛÀÎ °æ?E
	if(g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->m_bPickup && iContainerType == iPickUpContainer && iSlotIndex == iPickUpSlotIndex) {
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
		m_iPrevSlotIndex	=	-1;
		return 1;
	}

	if(g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetInventoryItem(iContainerType, iSlotIndex) == NULL) {
		return 1;
	}

	SPItemStatus* pItemStatus = g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetInventoryItem(iContainerType, iSlotIndex)->GetItemStatus();	

	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	if( pWindow != NULL )
		pWindow->SPSendMessage( SPIM_TOOLTIP_ENTER, (WPARAM)pItemStatus, SPWindowToolTip::TOOLTIP_SHOW_TYPE_ITEM );

	return 1;
}


int SPWindowItem::OnToolTipOut	( WPARAM wParam, LPARAM lParam) 
{
	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
	
	m_iPrevSlotIndex	=	-1;

	return 1;
}


int SPWindowItem::OnToolTipMove	( WPARAM wParam, LPARAM lParam) 
{

	if(g_pGOBManager->GetLocalPlayer() == NULL ) // by metalgeni
		return 1;
 

	CONTAINER_TYPE iContainerType = g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetCurrentItemPage();
	
	int iSlotIndex = (int)lParam;
	iSlotIndex += (g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetInventoryCurPage(iContainerType) * INVENTORY_LINE);

	if( m_iPrevSlotIndex == iSlotIndex )
		return 1;

	m_iPrevSlotIndex	=	iSlotIndex;

	int iPickUpContainer = g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetPickupItem().GetContainerType();
	int iPickUpSlotIndex = g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetPickupItem().GetSlotIndex();
	
	//Pick Up Item?E°°Àº ¾ÆÀÌÅÛÀÎ °æ?E
	if(g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->m_bPickup && iContainerType == iPickUpContainer && iSlotIndex == iPickUpSlotIndex) {
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
		m_iPrevSlotIndex	=	-1;
		return 1;
	}

	if(g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetInventoryItem(iContainerType, iSlotIndex) == NULL) {		
		return 1;
	}
	
	SPItemStatus* pItemStatus = g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetInventoryItem(iContainerType, iSlotIndex)->GetItemStatus();

	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	if( pWindow != NULL )
		pWindow->SPSendMessage( SPIM_TOOLTIP_MOVE, (WPARAM)pItemStatus, SPWindowToolTip::TOOLTIP_SHOW_TYPE_ITEM );

	return 1;
}


//////////////////////////////////////////////////////////////////////////
// Button Event
int SPWindowItem::OnEquip( WPARAM, LPARAM lParam) 
{
	//// PickUp Item Ã³¸®
	//if(g_pGOBManager->GetLocalPlayer()){
	//	if( g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetPickupItem().GetContainerType() > CONTAINER_TYPE_INVALID
	//		&& g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetPickupItem().GetContainerType() < FIGURE_EQUIP) 
	//	{
	//		if(g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetPickupItem().GetContainerType() != INVENTORY_EQUIP)
	//			return 0;
	//	}
	//}

	SPWindow* pWindow;
	pWindow = Find(WIID_ITEM_EQUIP		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, TRUE		);
	pWindow = Find(WIID_ITEM_CONSUME	, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
	pWindow = Find(WIID_ITEM_ETC		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
	pWindow = Find(WIID_ITEM_EVENT		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
	pWindow = Find(WIID_ITEM_PET		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
	pWindow = Find(WIID_ITEM_HOUSING	, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);	

	if(m_pGrid)
		m_pGrid->SetShowLine();

	if(g_pGOBManager->GetLocalPlayer())	// metalgeni
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_ITEM_CLICK_EQUIP , lParam);
	return 1;
}

int SPWindowItem::OnConsumable( WPARAM, LPARAM lParam )
{
	//// PickUp Item Ã³¸®
	//if(g_pGOBManager->GetLocalPlayer()){
	//	if( g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetPickupItem().GetContainerType() > CONTAINER_TYPE_INVALID
	//		&& g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetPickupItem().GetContainerType() < FIGURE_EQUIP) 
	//	{
	//		if(g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetPickupItem().GetContainerType() != INVENTORY_CONSUME)
	//			return 0;
	//	}
	//}
	
	SPWindow* pWindow;
	pWindow = Find(WIID_ITEM_EQUIP		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
	pWindow = Find(WIID_ITEM_CONSUME	, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, TRUE		);
	pWindow = Find(WIID_ITEM_ETC		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
	pWindow = Find(WIID_ITEM_EVENT		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
	pWindow = Find(WIID_ITEM_PET		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
	pWindow = Find(WIID_ITEM_HOUSING	, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);

	if(m_pGrid)
		m_pGrid->SetShowLine();

	if(g_pGOBManager->GetLocalPlayer())	// metalgeni
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_ITEM_CLICK_CONSUME , lParam);
	return 1;
}

int SPWindowItem::OnEtc( WPARAM, LPARAM lParam ) 
{
	//// PickUp Item Ã³¸®
	//if(g_pGOBManager->GetLocalPlayer()){
	//	if( g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetPickupItem().GetContainerType() > CONTAINER_TYPE_INVALID
	//		&& g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetPickupItem().GetContainerType() < FIGURE_EQUIP) 
	//	{
	//		if(g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetPickupItem().GetContainerType() != INVENTORY_ETC)
	//			return 0;
	//	}
	//}

	SPWindow* pWindow;
	pWindow = Find(WIID_ITEM_EQUIP		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
	pWindow = Find(WIID_ITEM_CONSUME	, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
	pWindow = Find(WIID_ITEM_ETC		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, TRUE		);
	pWindow = Find(WIID_ITEM_EVENT		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
	pWindow = Find(WIID_ITEM_PET		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
	pWindow = Find(WIID_ITEM_HOUSING	, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);	

	if(m_pGrid)
		m_pGrid->SetShowLine();

	if(g_pGOBManager->GetLocalPlayer())	// metalgeni
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_ITEM_CLICK_ETC , lParam);
	return 1;
}

int SPWindowItem::OnEvent( WPARAM, LPARAM lParam )
{
	//// PickUp Item Ã³¸®
	//if(g_pGOBManager->GetLocalPlayer()){
	//	if( g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetPickupItem().GetContainerType() > CONTAINER_TYPE_INVALID
	//		&& g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetPickupItem().GetContainerType() < FIGURE_EQUIP) 
	//	{
	//		if(g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetPickupItem().GetContainerType() != INVENTORY_EVENT)
	//			return 0;
	//	}
	//}
	
	SPWindow* pWindow;
	pWindow = Find(WIID_ITEM_EQUIP		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
	pWindow = Find(WIID_ITEM_CONSUME	, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
	pWindow = Find(WIID_ITEM_ETC		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
	pWindow = Find(WIID_ITEM_EVENT		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, TRUE		);
	pWindow = Find(WIID_ITEM_PET		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
	pWindow = Find(WIID_ITEM_HOUSING	, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);	

	if(m_pGrid)
		m_pGrid->SetShowLine();

	if(g_pGOBManager->GetLocalPlayer())	// metalgeni
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_ITEM_CLICK_EVENT , lParam);
	return 1;
}

int SPWindowItem::OnPet( WPARAM, LPARAM lParam )
{
	//// PickUp Item Ã³¸®
	//if(g_pGOBManager->GetLocalPlayer()){
	//	if( g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetPickupItem().GetContainerType() > CONTAINER_TYPE_INVALID
	//		&& g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetPickupItem().GetContainerType() < FIGURE_EQUIP) 
	//	{
	//		if(g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetPickupItem().GetContainerType() != INVENTORY_PET)
	//			return 0;
	//	}
	//}

	SPWindow* pWindow;
	pWindow = Find(WIID_ITEM_EQUIP		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
	pWindow = Find(WIID_ITEM_CONSUME	, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
	pWindow = Find(WIID_ITEM_ETC		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
	pWindow = Find(WIID_ITEM_EVENT		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
	pWindow = Find(WIID_ITEM_PET		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, TRUE		);
	pWindow = Find(WIID_ITEM_HOUSING	, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);	

	if(m_pGrid) {
		SPPlayerInvenArchive*	pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
		if(pInven == NULL)
			return 1;

		if(pInven->GetInventorySize(INVENTORY_PET) < VIEW_SLOT) {
			int iLine = pInven->GetInventorySize(INVENTORY_PET) / INVENTORY_LINE;
			m_pGrid->SetShowLine(iLine);
		}
		else {
			m_pGrid->SetShowLine();
		}
	}

	if(g_pGOBManager->GetLocalPlayer())	// metalgeni
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_ITEM_CLICK_PET , lParam);
	return 1;
}

int SPWindowItem::OnHousing( WPARAM, LPARAM lParam )
{
	////
	//if(g_pGOBManager->GetLocalPlayer()){
	//	if( g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetPickupItem().GetContainerType() > CONTAINER_TYPE_INVALID
	//		&& g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetPickupItem().GetContainerType() < FIGURE_EQUIP) 
	//	{
	//		if(g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->GetPickupItem().GetContainerType() != INVENTORY_HOUSING)
	//			return 0;
	//	}
	//}
	
	SPWindow* pWindow;
	pWindow = Find(WIID_ITEM_EQUIP		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
	pWindow = Find(WIID_ITEM_CONSUME	, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
	pWindow = Find(WIID_ITEM_ETC		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
	pWindow = Find(WIID_ITEM_EVENT		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
	pWindow = Find(WIID_ITEM_PET		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
	pWindow = Find(WIID_ITEM_HOUSING	, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, TRUE		);	

	if(m_pGrid)
		m_pGrid->SetShowLine();

	if(g_pGOBManager->GetLocalPlayer())	// metalgeni
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_ITEM_CLICK_HOUSING , lParam);
	return 1;
}


int SPWindowItem::OnSetDefault(WPARAM wParam, LPARAM lParam)
{
	CONTAINER_TYPE iType = (CONTAINER_TYPE)(wParam);
	
	SPWindow* pWindow;
	switch(iType) {		
		case INVENTORY_CONSUME:
			pWindow = Find(WIID_ITEM_EQUIP		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
			pWindow = Find(WIID_ITEM_CONSUME	, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, TRUE		);
			pWindow = Find(WIID_ITEM_ETC		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
			pWindow = Find(WIID_ITEM_EVENT		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
			pWindow = Find(WIID_ITEM_PET		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
			pWindow = Find(WIID_ITEM_HOUSING	, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
			break;
		case INVENTORY_ETC:
			pWindow = Find(WIID_ITEM_EQUIP		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
			pWindow = Find(WIID_ITEM_CONSUME	, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
			pWindow = Find(WIID_ITEM_ETC		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, TRUE		);
			pWindow = Find(WIID_ITEM_EVENT		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
			pWindow = Find(WIID_ITEM_PET		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
			pWindow = Find(WIID_ITEM_HOUSING	, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
			break;
		case INVENTORY_EVENT:
			pWindow = Find(WIID_ITEM_EQUIP		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
			pWindow = Find(WIID_ITEM_CONSUME	, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
			pWindow = Find(WIID_ITEM_ETC		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
			pWindow = Find(WIID_ITEM_EVENT		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, TRUE		);
			pWindow = Find(WIID_ITEM_PET		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
			pWindow = Find(WIID_ITEM_HOUSING	, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
			break;
		case INVENTORY_PET:
			pWindow = Find(WIID_ITEM_EQUIP		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
			pWindow = Find(WIID_ITEM_CONSUME	, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
			pWindow = Find(WIID_ITEM_ETC		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
			pWindow = Find(WIID_ITEM_EVENT		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
			pWindow = Find(WIID_ITEM_PET		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, TRUE		);
			pWindow = Find(WIID_ITEM_HOUSING	, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
			break;
		case INVENTORY_HOUSING:			
			pWindow = Find(WIID_ITEM_EQUIP		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
			pWindow = Find(WIID_ITEM_CONSUME	, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
			pWindow = Find(WIID_ITEM_ETC		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
			pWindow = Find(WIID_ITEM_EVENT		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
			pWindow = Find(WIID_ITEM_PET		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
			pWindow = Find(WIID_ITEM_HOUSING	, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, TRUE		);
			break;
		case INVENTORY_EQUIP:			
		default:
			pWindow = Find(WIID_ITEM_EQUIP		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, TRUE		);
			pWindow = Find(WIID_ITEM_CONSUME	, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
			pWindow = Find(WIID_ITEM_ETC		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
			pWindow = Find(WIID_ITEM_EVENT		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
			pWindow = Find(WIID_ITEM_PET		, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
			pWindow = Find(WIID_ITEM_HOUSING	, true); if(pWindow) pWindow->SPSendMessage(SPIM_BTN_SELECT, FALSE	);
			break;
	}

	if(g_pGOBManager->GetLocalPlayer() == NULL) {
		m_pGrid->SetShowLine();
		return 1;
	}

	if(iType == INVENTORY_PET) {
		SPPlayerInvenArchive*	pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
		if(pInven == NULL)
			return 1;

		if(pInven->GetInventorySize(iType) < VIEW_SLOT) {
			int iLine = pInven->GetInventorySize(iType) / INVENTORY_LINE;
			m_pGrid->SetShowLine(iLine);
		}
		else {
			m_pGrid->SetShowLine();
		}
	}
	else {
		m_pGrid->SetShowLine();
	}	
	
	return 1;
}


int SPWindowItem::OnSetLine(WPARAM wParam, LPARAM lParam)
{
	CONTAINER_TYPE iType = (CONTAINER_TYPE)(wParam);

	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return 1;

	if(iType == INVENTORY_PET) {
		SPPlayerInvenArchive*	pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
		if(pInven == NULL)
			return 1;

		if(pInven->GetInventorySize(iType) < VIEW_SLOT) {
			int iLine = pInven->GetInventorySize(iType) / INVENTORY_LINE;
			m_pGrid->SetShowLine(iLine);
		}
		else {
			m_pGrid->SetShowLine();
		}
	}
	else {
		m_pGrid->SetShowLine();
	}

	return 1;
}


//////////////////////////////////////////////////////////////////////////
//int SPWindowItem::OnGrid( WPARAM wParam, LPARAM lParam)
//{
//	// ItemÀ» Å¬¸¯ÇßÀ»°æ?E( µé¾ûÛÃ¸®´Â °æ?E ³»¸®´Â °æ?E)
//	if(g_pGOBManager->GetLocalPlayer())	// metalgeni
//		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_ITEM_CLICK_GRID , lParam);
//
//	return 1;
//}


int SPWindowItem::OnGridDown( WPARAM wParam, LPARAM lParam)
{
	if(g_pGOBManager->GetLocalPlayer())	// metalgeni
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_ITEM_GRID_CLICK_DOWN , lParam);
	
	return 1;
}


int SPWindowItem::OnGridUp( WPARAM wParam, LPARAM lParam)
{
	if((int)lParam < 0) {
		if(g_pGOBManager->GetLocalPlayer())
			g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
	}


	if(g_pGOBManager->GetLocalPlayer())	// metalgeni
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_ITEM_GRID_CLICK_UP , lParam);

	return 1;
}


int SPWindowItem::OnGridDBK( WPARAM wParam, LPARAM lParam)
{
	if((int)lParam < 0) {
		return 1;
	}


	if(g_pGOBManager->GetLocalPlayer())	// metalgeni
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_ITEM_GRID_DBK , lParam);
	
	return 1;
}


int SPWindowItem::OnItemClearAllUserShopItem( WPARAM wParam, LPARAM lParam )
{
	SPWindow*		pWindow	=	NULL;
	INSTANCE_ID		iID		=	(INSTANCE_ID)wParam;
	std::vector< SPWindow* >	vActionWindow;

	//////////////////////////////////////////////////////////////////////////

	vActionWindow.reserve( 10 );
	
	vActionWindow.push_back( Find( WIID_ENCHANT ) );
	vActionWindow.push_back( Find( WIID_ITEM_UPGRADE ) );
	vActionWindow.push_back( Find( WIID_ITEM_TIMEADD ) );
	vActionWindow.push_back( Find( WIID_PET_LEVELUP ) );
	vActionWindow.push_back( Find( WIID_EFFECTITEM_UPGRADE ) );//»ÃÓ° ý¼?
	vActionWindow.push_back( g_pInterfaceManager->FindWindow( WIID_ITEM_MIX ) );
	vActionWindow.push_back( g_pInterfaceManager->FindWindow( WIID_USERSHOP_SELL ) );
	

	std::vector< SPWindow* >::iterator	Iter	=	vActionWindow.begin();
	std::vector< SPWindow* >::iterator	IterEnd	=	vActionWindow.end();

	while( Iter != IterEnd )
	{
		pWindow = (SPWindow*)(*Iter);

		if( pWindow != NULL )
		{
			if( pWindow->IsShow() == true )
			{
				if( pWindow->GetInstanceID() == WIID_ENCHANT )
				{
					pWindow->SPSendMessage( SPIM_ITEM_ENCHANT_CANCEL );
				}
				
				////
				//if(pWindow->GetInstanceID() == WIID_ITEM_TIMEADD) {
				//	if(pWindow && pWindow->IsShow()) {
				//		if(g_pGOBManager->GetLocalPlayer()) {
				//			SPPlayerInvenArchive* pInven = NULL;
				//			if(g_pGOBManager->GetLocalPlayer()) {
				//				pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
				//				pInven->ClearUserShopItem(0);
				//				pInven->ClearUserShopItem(1);
				//				pInven->ClearUserShopItem(2);
				//			}
				//		}
				//	}
				//}
				pWindow->Hide();
			}
			else
			{
				if( pWindow->GetInstanceID() == iID )
				{
					pWindow->Show();
				}
			}
		}

		++Iter;
	}

	//////////////////////////////////////////////////////////////////////////

	return 1;
}

int SPWindowItem::OnItemSetting( WPARAM wParam, LPARAM lParam)
{
	CONTAINER_ITEM *item; // = new CONTAINER_ITEM;
	item = (CONTAINER_ITEM*)wParam;

	POINT ptSrcPos;	
	SPTexture* texture = g_pItemCluster->GetItemTexture(item->Item.iItemID, ptSrcPos); 	
	
	//Grid ¿¡ ?E?¼¼ÆÃ
	SPWindowGrid *pWindow = (SPWindowGrid*)(GetWrapWindow()->GetParent()->Find(WIID_ITEM_GRID, true));
	//pWindow->SettingIconImage( texture, item->Item.iItemID, ptSrcPos.x, ptSrcPos.y, item->SlotInfo.SlotIndex-1, item->Item.iStackCount );
	pWindow->SettingIconImage( texture, item->Item.iItemID, ptSrcPos.x, ptSrcPos.y, item->SlotInfo.SlotIndex, item->Item.iStackCount, (bool)lParam);
	return 1;
}


int SPWindowItem::OnItemDisable	( WPARAM wParam, LPARAM lParam)
{
	int iIndex = (int)wParam;
	bool bAble = (bool)lParam;

	SPWindowGrid *pWindow = (SPWindowGrid*)(GetWrapWindow()->GetParent()->Find(WIID_ITEM_GRID, true));
	pWindow->CellEnable(iIndex, bAble);
	return 1;
}


int SPWindowItem::OnItemAllClear( WPARAM wParam, LPARAM lParam)
{		
	SPWindowGrid *pWindow = (SPWindowGrid*)(GetWrapWindow()->GetParent()->Find(WIID_ITEM_GRID, true));
	pWindow->ClearCellAll() ;

	return 1;
}

int SPWindowItem::OnItemClear( WPARAM wParam, LPARAM lParam)
{
	//int xPos = LOWORD(lParam);
	//int yPos = HIWORD(lParam);
	int iSlotIndex = (int)lParam;

	SPWindowGrid *pWindow = (SPWindowGrid*)(GetWrapWindow()->GetParent()->Find(WIID_ITEM_GRID, true));
	
	//pWindow->ClearCell( xPos, yPos ) ;	
	pWindow->ClearCell(iSlotIndex);

	return 1;
}

int SPWindowItem::OnItemSetStack( WPARAM wParam, LPARAM lParam)
{
	//wParam slot index
	int iIndex = (int)wParam /*- 1*/;
	short iStackCount = (short)lParam;

	SPWindowGrid *pWindow = (SPWindowGrid*)(GetWrapWindow()->GetParent()->Find(WIID_ITEM_GRID, true));
	pWindow->SetItemStack(iIndex, iStackCount);
	
	return 1;
}

int SPWindowItem::OnExitHide( WPARAM wParam, LPARAM lParam)
{
	bool bShow = (bool)lParam;

	if(bShow) {
		m_pHide->Show();
		m_pUserShop->SetEnable(true);
		m_pEnchant->SetEnable(m_bUseItemEnchant);
		m_pUpGrade->SetEnable(m_bUseItemUpgrade);
		m_pTimeAdd->SetEnable(m_bUesItemTimeAdd);
		m_pPetLevelUP->SetEnable(m_bUseItemPetLevelUp);
		m_pPhantomLevelUp->SetEnable( m_bUseItemPhantomLevelUp );//»ÃÓ° ý¼?
	}
	else {
		m_pHide->Hide();
		m_pUserShop->SetEnable(false);
		m_pUpGrade->SetEnable(false);
		m_pEnchant->SetEnable(false);
		m_pTimeAdd->SetEnable(false);
		m_pPetLevelUP->SetEnable(false);
		m_pPhantomLevelUp->SetEnable( false );//»ÃÓ° ý¼?
	}

	return 1;
}


int SPWindowItem::OnSubMenu( WPARAM wParam, LPARAM lParam)
{
	bool bShow = (bool)lParam;

	if(bShow) {
		m_pUserShop->SetEnable(true);
		m_pEnchant->SetEnable(m_bUseItemEnchant);
		m_pUpGrade->SetEnable(m_bUseItemUpgrade);
		m_pTimeAdd->SetEnable(m_bUesItemTimeAdd);
		m_pPetLevelUP->SetEnable(m_bUseItemPetLevelUp);
		m_pPhantomLevelUp->SetEnable( m_bUseItemPhantomLevelUp ); //»ÃÓ° ý¼?
	}
	else {
		m_pUserShop->SetEnable(false);
		m_pUpGrade->SetEnable(false);
		m_pEnchant->SetEnable(false);
		m_pTimeAdd->SetEnable(false);
		m_pPetLevelUP->SetEnable(false);
		m_pPhantomLevelUp->SetEnable( false ); //»ÃÓ° ý¼?
	}

	return 1;
}


int SPWindowItem::OnItemSellSelect( WPARAM wParam, LPARAM lParam)
{
	//int iPosX = LOWORD(wParam);
	//int iPosY = HIWORD(wParam);
	int iSlotIndex = (int)wParam;
	bool bSelect = (bool)lParam;

	SPWindowGrid *pWindow = (SPWindowGrid*)(GetWrapWindow()->GetParent()->Find(WIID_ITEM_GRID, true));	
	
	//pWindow->ClearCell( xPos, yPos ) ;
	//pWindow->CellSelectSell( iIndex );
	//pWindow->CellSelectSell( iPosX, iPosY, bSelect );
	pWindow->CellSelectSell(iSlotIndex, bSelect);

	return 1;
}


int SPWindowItem::OnSetMaxPage( WPARAM wParam, LPARAM lParam)
{
	CONTAINER_TYPE iType = (CONTAINER_TYPE)wParam;
	int iPage = (int)lParam;
	//DXUTOutputDebugString("SPWindowItem::OnSetMaxPage Container[%d] Page[%d]\n", iType, iPage);
	m_pSlider->SPSendMessage(SPIM_SET_MAXVALUE, (WPARAM)iPage);
	return 1;
}


int SPWindowItem::OnSetPage( WPARAM wParam, LPARAM lParam)
{
	int iCurPage = (int)lParam;
	//DXUTOutputDebugString("SPWindowItem::OnSetPage CurPage[%d]\n", iCurPage);
	m_pSlider->SPSendMessage(SPIM_SET_CURVALUE, (WPARAM)iCurPage);	
	return 1;
}


int SPWindowItem::OnUse( WPARAM wParam, LPARAM lParam)
{
	if(g_pGOBManager->GetLocalPlayer())	// metalgeni
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage( SPGM_ITEM_RCLICK_GRID , lParam);

	return 1;
}

int SPWindowItem::OnDragnDropEnd( WPARAM wParam, LPARAM lParam )
{
	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return 1;

	int	iMessage	=	(int)wParam;

	if( iMessage != -1 || pLocalPlayer->GetInvenArchive()->m_bPickup == true )
		pLocalPlayer->SPGOBPerformMessage( SPGM_INVEN_RECOVER_ICON );

	return 1;
}

int SPWindowItem::OnItemEnchantKeyUp( WPARAM wParam, LPARAM lParam )
{
	if( IsShow() == false )
		Show();

	OnItemClearAllUserShopItem( (WPARAM)WIID_ENCHANT , NULL );

	return 1;
}

int SPWindowItem::OnItemUpgradeKeyUp( WPARAM wParam, LPARAM lParam )
{
	if( IsShow() == false )
		Show();

	OnItemClearAllUserShopItem( (WPARAM)WIID_ITEM_UPGRADE , NULL );

	return 1;
}


int SPWindowItem::OnOpenUserShop( WPARAM wParam, LPARAM lParam )
{	
	if( IsShow() == false )
		return 1;

	////[2008/2/1]-jinhee °³ÀÎ»óÁ¡ °ú Áßº¹ ½½·ÔÀ» ÀÌ¿ëÇÏ±â ¶§¹®¿¡ ½Ã°£¿¬Àå½Ã ÃÊ±âÈ­´Â ¿©±â¼­
	//SPWindow* pkWindow = NULL;
	//pkWindow = Find( WIID_ITEM_TIMEADD );
	//if(pkWindow && pkWindow->IsShow()) {
	//	if(g_pGOBManager->GetLocalPlayer()) {
	//		SPPlayerInvenArchive* pInven = NULL;
	//		if(g_pGOBManager->GetLocalPlayer()) {
	//			pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	//			pInven->ClearUserShopItem(0);
	//			pInven->ClearUserShopItem(1);
	//			pInven->ClearUserShopItem(2);
	//		}
	//	}
	//}

	OnItemClearAllUserShopItem( (WPARAM)WIID_USERSHOP_SELL , NULL );

	return 1;
}


int SPWindowItem::OnItemAddTime( WPARAM wParam, LPARAM lParam )
{
	if( IsShow() == false )
		return 1;

	OnItemClearAllUserShopItem( (WPARAM)WIID_ITEM_TIMEADD , NULL );

	return 1;
}

int SPWindowItem::OnPetLevelUP( WPARAM wParam, LPARAM lParam )
{
	if( IsShow() == false )
		return 1;

	if( SPLocalizeManager::GetInstance()->IsEnableValue(LV_PET) == false )
		return 1;

	OnItemClearAllUserShopItem( (WPARAM)WIID_PET_LEVELUP , NULL );

	return 1;
}

int SPWindowItem::OnPhantomLevelUp( WPARAM wParam, LPARAM lParam )
{
	if( IsShow() == false )
	{
		return 1;
	}
	OnItemClearAllUserShopItem( (WPARAM)WIID_EFFECTITEM_UPGRADE, NULL );

	return 1;
}


int SPWindowItem::OnSetHotStatus(WPARAM wParam, LPARAM lParam)
{
	bool bShow = SPCashTradeManager::GetInstance()->GetHotItemManager()->IsShow();
	
	SPWindow* pWindow = Find(WIID_CASH_HOTLIST);
	bool bShowHotList = false;
	if(pWindow) {
		if(bShow) {
			if(m_bShow && !pWindow->IsShow()) {
				pWindow->Show();
				bShowHotList = true;
			}
		}
		else {
			if(m_bShow && pWindow->IsShow()) {
				pWindow->Hide();
			}
		}
	}

	return 1;
}


void SPWindowItem::UpdateSubWindowPos(bool bShowHotList /*= false*/)
{
	SPWindow* pWindow = NULL;
	
	//new SPWindowEnchant(WIID_ENCHANT, m_iSX, 0, 240, 114, this);
	//new SPWindowItemUpgrade( WIID_ITEM_UPGRADE , m_iSX , 0 , 192 , 212 , this );
	//new SPWindowItemTimeAdd( WIID_ITEM_TIMEADD, m_iSX, 0, 240, 125, this);
	//new SPWindowPetLevelUp( WIID_PET_LEVELUP , m_iSX , 0 , 240 , 125 , this );
	//new SPWindowCashHotList(WIID_CASH_HOTLIST, m_iSX, 46, 44, 210, this);

	int iHotListWidth = 44;

	pWindow = Find(WIID_ENCHANT);
	if( pWindow ) {
		pWindow->Hide();
		if(bShowHotList){
			pWindow->AdjustCoord(m_iSX + iHotListWidth, 46, 240, 114);
		}
		else {
			pWindow->AdjustCoord(m_iSX, 46, 240, 114);
		}	
	}

	pWindow = Find( WIID_ITEM_UPGRADE );
	if( pWindow ) {
		pWindow->Hide();
		if(bShowHotList){
			pWindow->AdjustCoord(m_iSX + iHotListWidth , 46 , 192 , 212);
		}
		else {
			pWindow->AdjustCoord(m_iSX , 46 , 192 , 212);
		}		
	}

	pWindow = Find( WIID_ITEM_TIMEADD );
	if(pWindow) {
		pWindow->Hide();
		if(bShowHotList){
			pWindow->AdjustCoord(m_iSX + iHotListWidth, 46, 240, 125);
		}
		else {
			pWindow->AdjustCoord(m_iSX, 46, 240, 125);
		}		
	}

	pWindow = Find( WIID_PET_LEVELUP );
	if(pWindow) {
		pWindow->Hide();
		if(bShowHotList){
			pWindow->AdjustCoord(m_iSX + iHotListWidth, 46, 240, 125);
		}
		else {
			pWindow->AdjustCoord(m_iSX, 46, 240, 125);
		}		
	}
	
	pWindow = Find( WIID_EFFECTITEM_UPGRADE );
	if(pWindow) {
		pWindow->Hide();
		if(bShowHotList){
			pWindow->AdjustCoord(m_iSX + iHotListWidth, 46, 240, 125);
		}
		else {
			pWindow->AdjustCoord(m_iSX, 46, 240, 125);
		}		
	}
}


void SPWindowItem::CheckDragNDrop()
{
	if( g_pGOBManager->GetLocalPlayer() == NULL )
		return;

	if( g_pInterfaceManager->GetNoticeBox()->IsShow() == TRUE )
		return;

	SPPlayerInvenArchive*	pInvenArchive		=	g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	SPDragnDropManager*		pDragnDropManager	=	SPDragnDropManager::GetInstance();

	if( pDragnDropManager->IsDragging() == TRUE )
		return;

	if( pInvenArchive != NULL )
	{
		int	iSelectedItemNum	=	pInvenArchive->GetPickUpItemID();

		if( iSelectedItemNum != 0 )
		{
			SPSelectItem	SelectItem	=	pInvenArchive->GetPickupItem();
			SPItem*			pItem		=	pInvenArchive->GetInventoryItem( SelectItem.GetContainerType() , SelectItem.GetSlotIndex() );

			if( pItem != NULL )
			{
				SPItemAttr*		pItemAttr	=	pItem->GetItemAttr();

				if( pItemAttr != NULL )
				{
					if( pItemAttr->m_eType == ITEM_TYPE_CONSUME )
					{
						m_pSelectItem->SetItemID( iSelectedItemNum );
						m_pSelectItem->SetItemType( pItem->GetContainerType() );
						m_pSelectItem->SetCoolTimeID( pItemAttr->m_iItemCoolTimeID );
						m_pSelectItem->ReResourceLoad();
						pDragnDropManager->Set( this , (SPUIUnit*)m_pSelectItem );
					}
				}
			}
		}
	}
}
//--------------------------------------------------
void SPWindowItem::RefreshRelationCoord( void )
{
	SPWindow *pushPropWnd = g_pInterfaceManager->FindWindow( WIID_PUSH_PROP );
	if (pushPropWnd)
	{
		int x = 0;
		int y = 0;
		GetAbsCoord( x, y );
		pushPropWnd->SetAbsCoord( x - 42, y );
		pushPropWnd->RefreshChildPos();
	}
	SPWindow::RefreshRelationCoord();
}
//--------------------------------------------------