
#include "SPCommon.h"

#include "SPStage.h"
#include "SPStageManager.h"
#include "SPStageLoader.h"

#include "SPTerrainDEF.h"
#include "SPTerrainCluster.h"
#include "SPTerrainManager.h"

#include "SPUtil.h"
#include "SPWindowDEF.h"

#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPWindowGrid.h"
#include "SPWindowSlider.h"
#include "SPWindowEdit.h"
#include "SPWindowEditMultiLine.h"
#include "SPTreeBoxDEF.h"
#include "SPTreeBoxItemBase.h"
#include "SPTreeBoxItemQuest.h"
#include "SPWindowTreeBox.h"
#include "SPComboBoxDEF.h"
#include "SPComboBoxItemBase.h"
#include "SPComboBoxItemTexture.h"
#include "SPWindowComboBox.h"
#include "SPWindowPageSelect.h"
#include "SPWindowCashShop.h"

#include "SPSubGameManager.h"
#include "SPGOBManager.h"
#include "SPGameObject.h"
#include "SPPlayerInvenArchive.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGOBModelUnit.h"
#include "SPPlayer.h"
#include "SPPet.h"

#include "SPWindowChat.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"
#include "SPFont.h"

#include "SPCheckManager.h"

#include <shellapi.h>
#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"

#include "SPLocalizeManager.h"
#include "GlobalDefines_Share.h"


#include "SPRefOLE.h" // for IngameWB
#include "SPIngameWebbrowser.h"

#include "SPCashTradeManager.h"

#include "SPRenderModelDEF.h"
#include "SPRenderModel.h"
#include "SPFXDEF.h"
#include "SPFXModel.h"
#include "SPFXManager.h"

#include "SPGOBCoordPhysics.h"
#include "SPPlayerCoordPhysics.h"

#include "SPSkill.h"
#include "SPSkillArchive.h"

#include "SPHotItemInfo.h"
#include "SPHotItemManager.h"
#include "SPCashTradeManager.h"

#include "atlconv.h"

#ifdef _DEBUG
	#include "DebugPrint.h"
#endif

//------------------------------------------------------------------------------------
SPWindowCashShop::SPWindowCashShop(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
, m_pkEventTexture(NULL)
, m_pkSaleArrowTexture(NULL)
, m_pkSaleOrderTexture(NULL)
, m_pkPlayer(NULL)
, m_pkBlackBackground(NULL)
, m_pkPreSelectImage(NULL)
, m_pkPet(NULL)
{	
	Init();
}

//------------------------------------------------------------------------------------
SPWindowCashShop::SPWindowCashShop(INSTANCE_ID InstanceID,
	int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_CASHSHOP, InstanceID, iX, iY, iCX, iCY, pParent)	
, m_pkEventTexture(NULL)
, m_pkSaleArrowTexture(NULL)
, m_pkSaleOrderTexture(NULL)
, m_pkPlayer(NULL)
, m_pkBlackBackground(NULL)
, m_pkPreSelectImage(NULL)
, m_pkBackground(NULL)
, m_pkPet(NULL)
, m_pkLobbyPlayer(NULL)
{	
	Init();
}

//------------------------------------------------------------------------------------
SPWindowCashShop::~SPWindowCashShop()
{
	Clean();
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::Clean()
{
	SPWindow::Clean();

	SAFE_RELEASE(m_pkEventTexture);
	SAFE_RELEASE(m_pkSaleArrowTexture);
	SAFE_RELEASE(m_pkSaleOrderTexture);
	SAFE_RELEASE(m_pkBlackBackground);
	SAFE_RELEASE(m_pkPreSelectImage);
	SAFE_RELEASE(m_pkBackground);
	SAFE_DELETE(m_pkPlayer);
	ReleasePet();
	//SAFE_DELETE(m_pkFont14);

	map< int, vector< CASHSHOP_HELP_INFO > >::iterator iter = m_mHelpInfoList.begin();
	while( iter != m_mHelpInfoList.end() )
	{
		vector< CASHSHOP_HELP_INFO >::iterator iter2 = (*iter).second.begin();
		while( iter2 != (*iter).second.end() )
		{
			(*iter2).Clear();
			++iter2;
		}
		(*iter).second.clear();
		++iter;
	}
	m_mHelpInfoList.clear();

	m_vEventAnimation.clear();
	m_vSaleArrow.clear();
	m_vSaleOrderArrow.clear();
	m_vDefaultFigureItem.clear();
	m_vEquipFigureItem.clear();
	m_vCartList.clear();
	m_vBackgroundTextList.clear();
	m_vBackgroundStarList.clear();
	m_vEquipCashItem.clear();

	m_mDefaultPageList.clear();
	ClearEffectList();

	m_vecBuyCashItemList.clear();

	m_pkLobbyPlayer = NULL;
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::Init()
{
	m_eBuyType = TYPE_BUY;
	m_vCashierItemAttr.clear();
	m_vEquipCashItem.clear();

	m_strGiftFriendName.clear();
	m_strGiftFriendMsg.clear();

	m_iMileageRatio = SPLocalizeManager::GetInstance()->GetMileageRatio();
	m_iMileageGlowUnit = SPLocalizeManager::GetInstance()->GetMileageGlowUnit();
	m_iCashierGlowUnit = SPLocalizeManager::GetInstance()->GetCashierGlowUnit();

	InitSultListView();
	InitMyInfo();
	InitStore();
	InitItemInven();
	//InitChar();
	//InitPrevView();    //[xialin 2008/07/10] 去掉购物车功能
	InitOzInven();
	InitCashierDlg();
	InitGiftDlg();
	InitExactDlg();		 //[xialin 2008/07/10] 索要对话框
	InitBuyListDlg();
	InitWearListDlg();
	InitExactListDlg();
	InitSellDlg();
	InitErrorDlg();
	InitCouponDlg();
	InitBackground();
	InitResult();
	InitBuyWait();
	InitPlayerView();    //[xialin 2008/07/10] 添加玩家预览视图

	SPWindowStatic * pkChildHelp = new SPWindowStatic(WIID_CASHSHOP_OZ_SELL_HELP, 194, 480, 247, 62, this);
	pkChildHelp->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 261, 208);
	
	m_iItemInvenCategory = -1;
	m_eStoreTabType = CST_NULL;
	m_iStoreCurPage = 0;
	m_iStoreMaxPage = 0;
	ZeroMemory(m_iStoreCategory, sizeof(m_iStoreCategory));

	m_iCashierOZ = 0;
	m_iCashierMileage = 0;
	m_vecBuyCheck.clear();
	m_iCurStartItem = 0;

	m_iOZInvenCategory = 0;
	m_iOZInvenCurPage[0] = 0;
	m_iOZInvenCurPage[1] = 0;
	m_iOZInvenMaxPage[0] = (OZ_INVENSLOT_MAX-10) / 5;
	m_iOZInvenMaxPage[1] = (OZ_INVENWEAR_MAX-10) / 5;

	m_iPreViewInvenMaxPage = OZ_INVENSLOT_MAX / 8;
	m_bPreViewSelect = false;
	m_iPreSelectSlot = -1;
	m_iErrorType = 0;

	m_pCurSultItemAttr = NULL;

	ClearSelectItem();

	m_vecBuyCashItemList.clear();

	int i;
	for( i = 0; i < 6; i++ )
	{
		m_iItemInvenMaxPage[i] = 0;
		m_iItemInvenCurPage[i] = 0;
	}

	m_iGUID = 0;
	m_szAccountID[0] = '\0';
	m_uiCurOZ = 0;
	m_uiCurMileage = 0;
	m_pkResultItemAttr = NULL;

	m_fNoticeAccmulateTime = 0.0f;
	m_fNoticeChangeTime = 0.0f;
	m_fBuyWaitAccmulateTime = 0.0f;

	m_bWebbrowerLoadingWait = false;

	g_pVideo->GetTextureMgr()->GetTexture("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", &m_pkEventTexture, TEX_INTERFACE, LEVEL_NOW);
	g_pVideo->GetTextureMgr()->GetTexture("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", &m_pkSaleArrowTexture, TEX_INTERFACE, LEVEL_NOW);
	g_pVideo->GetTextureMgr()->GetTexture("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", &m_pkSaleOrderTexture, TEX_INTERFACE, LEVEL_NOW);
	g_pVideo->GetTextureMgr()->GetTexture("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", &m_pkBlackBackground, TEX_INTERFACE, LEVEL_NOW);
	g_pVideo->GetTextureMgr()->GetTexture("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", &m_pkPreSelectImage, TEX_INTERFACE, LEVEL_NOW);
	g_pVideo->GetTextureMgr()->GetTexture("DATA/INTERFACE/CONCEPT/CASHSHOP_BACK.PNG", &m_pkBackground, TEX_INTERFACE, LEVEL_NOW);

	SetRect(&m_rcDstBlackBackground, 0, 0, 800, 600);
	SetRect(&m_rcSrcBlackBackground, 492, 448, 506, 462);

	SetRect(&m_rcDstBackground[0], 0, 0, 800, 512);
	SetRect(&m_rcDstBackground[1], 0, 512, 224, 600);
	SetRect(&m_rcDstBackground[2], 224, 512, 448, 600);
	SetRect(&m_rcDstBackground[3], 448, 512, 672, 600);
	SetRect(&m_rcDstBackground[4], 672, 512, 800, 600);

	SetRect(&m_rcSrcBackground[0], 0, 0, 800, 512);
	SetRect(&m_rcSrcBackground[1], 800, 0, 1024, 88);
	SetRect(&m_rcSrcBackground[2], 800, 88, 1024, 176);
	SetRect(&m_rcSrcBackground[3], 800, 176, 1024, 264);
	SetRect(&m_rcSrcBackground[4], 800, 264, 928, 352);

	SetRect(&m_rcPreSelect[0], 432, 16, 471, 55);
	SetRect(&m_rcPreSelect[1], 472, 16, 511, 55);
	SetRect(&m_rcPreSelect[2], 432, 56, 471, 95);
	SetRect(&m_rcPreSelect[3], 472, 56, 511, 95);

	//[xialin 2008/08/07]add >>>
	m_gpMgrTerrain = g_pTerrainManager;
	for ( int i=0; i<3; i++ )
	{
		m_pMgrTerrain[i] = new SPTerrainManager;
		m_pMgrTerrain[i]->Init();
		m_pMgrTerrain[i]->SetMgrCamera( g_pCameraManagr );
	}
	g_pTerrainManager = m_gpMgrTerrain;
	//[xialin 2008/08/07]add <<<

	m_pkPlayer = new SPPlayer;
	if( m_pkPlayer )
	{
		m_pkPlayer->TriggerEnable(true);
		m_pkPlayer->SetLobbyAvatar(LOBBY_CASH);
		m_pkPlayer->Init();
		m_pkPlayer->SetShowValue(true, false, false);
		m_pkPlayer->SetPos(125.0f, 195.0f);
	}
	LoadHelpLDTFile();
	LoadDefaultLDTFile();
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::InitMyInfo()
{
	SPWindowStatic* pkParent, * pkParent2, *pkChild[2];
	SPWindowButton* pkButton;

	pkParent = new SPWindowStatic(WIID_CASHSHOP_INFO, 233, 525, 288, 68, this);
	pkParent->SetWindowType(WND_TYPE_LEFTBASE);
	pkParent->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG",	275, 1, 276, 69);
	pkParent->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG",	277, 1, 282, 69);

	pkParent2 = new SPWindowStatic(WIID_CASHSHOP_INFO_GB2, 521, 525, 272, 68, this);
	pkParent2->SetWindowType(WND_TYPE_LEFTBASE);
	pkParent2->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG",	246, 1, 249, 69);
	pkParent2->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG",	250, 1, 252, 69);
	pkParent2->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG",	253, 1, 261, 69);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_OZ_SELL, 19, 0, 60, 56, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 390, 199);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 451, 199);
	
	if( m_iMileageRatio )
	{
		pkChild[1] = new SPWindowStatic(WIID_CASHSHOP_INFO_OZ_ICON, 85, 11, 172, 17, pkParent);
		pkChild[1]->SetWindowType(WND_TYPE_LEFTBASE);
		pkChild[1]->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 228, 416, 291, 433);
	    pkChild[1]->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG",	293, 416, 294, 433);
	    pkChild[1]->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 296, 416, 299, 433);

		pkChild[1] = new SPWindowStatic(WIID_CASHSHOP_INFO_MILEAGE_ICON, 85, 30, 172, 17, pkParent);
		pkChild[1]->SetWindowType(WND_TYPE_LEFTBASE);
		pkChild[1]->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 228, 434, 291, 451);
	    pkChild[1]->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 293, 434, 294, 451);
	    pkChild[1]->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 296, 434, 299, 451);

		pkChild[1] = new SPWindowStatic(WIID_CASHSHOP_INFO_OZ, 170, 13, 82, 12, pkParent);
		pkChild[1]->SetFont(FONT_12_BOLD);
		pkChild[1]->SetFontColor(D3DXCOLOR(0.2745f, 0.5490f, 0.4862f, 1.0f));
		pkChild[1]->SetFormat(DT_VCENTER | DT_RIGHT);

		pkChild[1] = new SPWindowStatic(WIID_CASHSHOP_INFO_MILEAGE, 170, 32, 82, 12, pkParent);
		pkChild[1]->SetFont(FONT_12_BOLD);
		pkChild[1]->SetFontColor(D3DXCOLOR(0.2745f, 0.5490f, 0.4862f, 1.0f));
		pkChild[1]->SetFormat(DT_VCENTER | DT_RIGHT);
	}
	else
	{
		pkChild[1] = new SPWindowStatic(WIID_CASHSHOP_INFO_OZ_ICON, 85, 11, 172, 17, pkParent);
		pkChild[1]->SetWindowType(WND_TYPE_LEFTBASE);
		pkChild[1]->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 228, 416, 291, 433);
	    pkChild[1]->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG",	293, 416, 294, 433);
	    pkChild[1]->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 296, 416, 299, 433);

		pkChild[1] = new SPWindowStatic(WIID_CASHSHOP_INFO_OZ, 170, 13, 82, 12, pkParent);
		pkChild[1]->SetFont(FONT_12_BOLD);
		pkChild[1]->SetFontColor(D3DXCOLOR(0.2745f, 0.5490f, 0.4862f, 1.0f));
		pkChild[1]->SetFormat(DT_VCENTER | DT_RIGHT);
	}

	if( SPLocalizeManager::GetInstance()->IsEnableValue(LV_COUPON) )
	{
		pkButton = new SPWindowButton(WIID_CASHSHOP_INFO_FILLING, 0, 2, 64, 64, pkParent2);
		pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 92);
		pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 157);
		pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 222);
		pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 287);

		pkButton = new SPWindowButton(WIID_CASHSHOP_INFO_UPDATE, 64, 2, 64, 64, pkParent2);
		pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 66, 92);
		pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 66, 157);
		pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 66, 222);
		pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 66, 287);

		pkButton = new SPWindowButton(WIID_CASHSHOP_INFO_COUPON, 128, 2, 64, 64, pkParent2);
		pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 131, 92);
		pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 131, 157);
		pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 131, 222);
		pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 131, 287);

		pkButton = new SPWindowButton(WIID_CASHSHOP_EXIT, 192, 2, 64, 64, pkParent2);
		pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 196, 92);
		pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 196, 157);
		pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 196, 222);
		pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 196, 287);
	}
	else
	{
		pkButton = new SPWindowButton(WIID_CASHSHOP_INFO_FILLING, 0, 2, 64, 64, pkParent2);
		pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 92);
		pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 157);
		pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 222);
		pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 287);

		pkButton = new SPWindowButton(WIID_CASHSHOP_INFO_UPDATE, 64, 2, 64, 64, pkParent2);
		pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 66, 92);
		pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 66, 157);
		pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 66, 222);
		pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 66, 287);
		
		pkButton = new SPWindowButton(WIID_CASHSHOP_EXIT, 192, 2, 64, 64, pkParent2);
		pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 196, 92);
		pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 196, 157);
		pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 196, 222);
		pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 196, 287);
	}

	//
	//pkChild[1] = new SPWindowStatic(WIID_CASHSHOP_INFO_CASH_SKIN, 713, 521, 90, 83, this);
	//pkChild[1]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP05.PNG", 421, 1);

}

//------------------------------------------------------------------------------------
void SPWindowCashShop::InitItemInven()
{
	SPWindowStatic* pkParent, *pkChild[2];
	SPWindowButton* pkButton;
	SPWindowGrid* pkGrid;
	SPWindowSlider* pkSlider;

	pkParent = new SPWindowStatic(WIID_CASHSHOP_ITEM_INVEN, 7, 379, 226, 214, this);
	pkParent->SetWindowType(WND_TYPE_LEFTBASE);
	pkParent->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 324, 1, 328, 215);
	pkParent->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 330, 1, 331, 215);
	pkParent->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 333, 1, 337, 215);
	
	pkButton = new SPWindowButton(WIID_CASHSHOP_ITEM_INVEN_EQUIP, 4, 4, 59, 19, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 1, 103);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 1, 123);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 1, 143);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 1, 143);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 1, 163);

	pkButton = new SPWindowButton(WIID_CASHSHOP_ITEM_INVEN_CONSUME, 62, 4, 59, 19, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 61, 103);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 61, 123);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 61, 143);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 61, 143);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 61, 163);

	pkButton = new SPWindowButton(WIID_CASHSHOP_ITEM_INVEN_ETC, 120, 4, 59, 19, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 121, 103);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 121, 123);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 121, 143);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 121, 143);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 121, 163);

	pkButton = new SPWindowButton(WIID_CASHSHOP_ITEM_INVEN_EVENT, 4, 25, 59, 19, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 181, 103);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 181, 123);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 181, 143);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 181, 143);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 181, 163);

	pkButton = new SPWindowButton(WIID_CASHSHOP_ITEM_INVEN_HOUSING, 62, 25, 59, 19, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 241, 103);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 241, 123);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 241, 143);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 241, 143);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 241, 163);

	pkButton = new SPWindowButton(WIID_CASHSHOP_ITEM_INVEN_PET, 120, 25, 59, 19, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 301, 103);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 301, 123);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 301, 143);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 301, 143);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 301, 163);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_ITEM_INVEN_WHITE_BG, 2, 46, 179, 156, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 465, 107);
	pkChild[0]->SetSrcSize(2, 2);

	pkGrid = new SPWindowGrid(WNDID_CTRL_GRID, WIID_CASHSHOP_ITEM_INVEN_SLOT,
		10, 8, 140, 140, pkChild[0], 4, 4, 36, 36, 32);
	pkGrid->SetGridImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 479, 116);
	pkGrid->ClearCellAll();

	pkButton = new SPWindowButton(WIID_CASHSHOP_ITEM_INVEN_UP, 158, 8, 12, 11, pkChild[0]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 456, 389);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 469, 389);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 482, 389);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 495, 389);

	pkButton = new SPWindowButton(WIID_CASHSHOP_ITEM_INVEN_DOWN, 158, 137, 12, 11, pkChild[0]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 456, 431);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 469, 431);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 482, 431);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 495, 431);

	pkSlider = new SPWindowSlider(WIID_CASHSHOP_ITEM_INVEN_THUMB, 158, 19, 12, 118, pkChild[0]);
	RECT rtRect;
	SetRect(&rtRect, 456, 401, 468, 425);
	pkSlider->SetImageHandle("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", rtRect);
	SetRect(&rtRect, 469, 401, 481, 425);
	pkSlider->SetImageHandleHit("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", rtRect);
	SetRect(&rtRect, 482, 401, 494, 425);
	pkSlider->SetImageHandlePush("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", rtRect);
	SetRect(&rtRect, 495, 401, 507, 425);
	pkSlider->SetImageHandleDisable("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", rtRect);

	SetRect(&rtRect, 456, 427, 468, 429);
	pkSlider->SetImagePageNormal("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", rtRect);
	SetRect(&rtRect, 469, 427, 481, 429);
	pkSlider->SetImagePageHit("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", rtRect);
	SetRect(&rtRect, 482, 427, 494, 429);
	pkSlider->SetImagePageShadow("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", rtRect);
	SetRect(&rtRect, 495, 427, 507, 429);
	pkSlider->SetImagePageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", rtRect);


	//
	//pkParent = new SPWindowStatic(WIID_CASHSHOP_ITEM_INVEN_ADDFRAME, 197, 377, 41, 222, this);
	//pkParent->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP05.PNG", 443, 1);

	pkButton = new SPWindowButton(WIID_CASHSHOP_ITEM_INVEN_ADDEQUIP, 174, 53, 48, 23, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 365, 164);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 365, 188);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 365, 212);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 365, 236);
	pkButton->SetEnable(false);

	pkButton = new SPWindowButton(WIID_CASHSHOP_ITEM_INVEN_ADDCONSUME, 174, 77, 48, 23, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 414, 164);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 414, 188);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 414, 212);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 414, 236);
	pkButton->SetEnable(false);

	pkButton = new SPWindowButton(WIID_CASHSHOP_ITEM_INVEN_ADDETC, 174, 101, 48, 23, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 463, 164);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 463, 188);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 463, 212);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 463, 236);
	pkButton->SetEnable(false);

	pkButton = new SPWindowButton(WIID_CASHSHOP_ITEM_INVEN_ADDEVENT, 174, 125, 48, 23, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 365, 260);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 365, 284);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 365, 308);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 365, 332);
	pkButton->SetEnable(false);

	pkButton = new SPWindowButton(WIID_CASHSHOP_ITEM_INVEN_ADDPET, 174, 149, 48, 23, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 414, 260);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 414, 284);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 414, 308);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 414, 332);
	pkButton->SetEnable(false);

	pkButton = new SPWindowButton(WIID_CASHSHOP_ITEM_INVEN_ADDWH, 174, 173, 48, 23, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 463, 260);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 463, 284);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 463, 308);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 463, 332);
	pkButton->SetEnable(false);

}

//------------------------------------------------------------------------------------
void SPWindowCashShop::InitOzInven()  //[xialin 2008/07/11]edit
{
	SPWindowStatic* pkParent, *pkChild[2];
	SPWindowButton* pkButton;
	SPWindowGrid* pkGrid;
	SPWindowSlider* pkSlider;
	
	pkParent = new SPWindowStatic(WIID_CASHSHOP_OZ_INVEN, 7, 255, 226, 118, this);
	pkParent->SetWindowType(WND_TYPE_LEFTBASE);
	pkParent->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 284, 1, 292, 119);
	pkParent->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 294, 1, 295, 119);
	pkParent->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 297, 1, 305, 119);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_OZ_INVEN_LINE_BG, 2, 24, 222, 8, pkParent);
	pkChild[0]->SetWindowType(WND_TYPE_LEFTBASE);
	pkChild[0]->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 306, 101, 309, 109);
	pkChild[0]->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 311, 101, 312, 109);
	pkChild[0]->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 314, 101, 317, 109);

	pkButton = new SPWindowButton(WIID_CASHSHOP_OZ_INVEN_BUYINFO, 92, 5, 68, 22, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 374, 0);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 374, 0);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 374, 23);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 374, 23);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 374, 0);

	pkButton = new SPWindowButton(WIID_CASHSHOP_OZ_INVEN_WEARINFO, 156, 5, 68, 22, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 443, 0);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 443, 0);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 443, 23);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 443, 23);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 443, 0);

	//pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_OZ_INVEN_WHITE_BG, 5, 36, 207, 78, pkParent);
	//pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 465, 107);
	//pkChild[0]->SetSrcSize(2, 2);
	
	pkGrid = new SPWindowGrid(WNDID_CTRL_GRID, WIID_CASHSHOP_OZ_INVEN_SLOT,
		14, 41, 178, 70, pkParent, 5, 2, 36, 36, 32);
	pkGrid->SetGridImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 479, 116);
	pkGrid->ClearCellAll();

	pkButton = new SPWindowButton(WIID_CASHSHOP_OZ_INVEN_LEFT, 202, 41, 12, 11, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 456, 389);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 469, 389);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 482, 389);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 495, 389);

	pkButton = new SPWindowButton(WIID_CASHSHOP_OZ_INVEN_RIGHT, 202, 98, 12, 11, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 456, 431);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 469, 431);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 482, 431);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 495, 431);

	pkSlider = new SPWindowSlider(WIID_CASHSHOP_OZ_INVEN_THUMB, 202, 52, 12, 46, pkParent);
	RECT rtRect;
	SetRect(&rtRect, 456, 401, 468, 425);
	pkSlider->SetImageHandle("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", rtRect);
	SetRect(&rtRect, 469, 401, 481, 425);
	pkSlider->SetImageHandleHit("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", rtRect);
	SetRect(&rtRect, 482, 401, 494, 425);
	pkSlider->SetImageHandlePush("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", rtRect);
	SetRect(&rtRect, 495, 401, 507, 425);
	pkSlider->SetImageHandleDisable("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", rtRect);

	SetRect(&rtRect, 456, 427, 468, 429);
	pkSlider->SetImagePageNormal("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", rtRect);
	SetRect(&rtRect, 469, 427, 481, 429);
	pkSlider->SetImagePageHit("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", rtRect);
	SetRect(&rtRect, 482, 427, 494, 429);
	pkSlider->SetImagePageShadow("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", rtRect);
	SetRect(&rtRect, 495, 427, 507, 429);
	pkSlider->SetImagePageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", rtRect);

}

//------------------------------------------------------------------------------------
void SPWindowCashShop::InitChar()
{
	SPWindowStatic* pkParent;
	SPWindowButton* pkButton;

	pkParent = new SPWindowStatic(WIID_CASHSHOP_CHAR_BG, 42, 346, 154, 20, this);
	pkParent->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 107, 104);

	pkButton = new SPWindowButton(WIID_CASHSHOP_CHAR_NORMAL, 46, 2, 16, 16, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 361, 32);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 361, 49);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 361, 66);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 361, 83);

	pkButton = new SPWindowButton(WIID_CASHSHOP_CHAR_RUN, 63, 2, 16, 16, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 378, 32);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 378, 49);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 378, 66);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 378, 83);

	pkButton = new SPWindowButton(WIID_CASHSHOP_CHAR_LADDER, 80, 2, 16, 16, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 395, 32);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 395, 49);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 395, 66);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 395, 83);

	pkButton = new SPWindowButton(WIID_CASHSHOP_CHAR_INIT, 97, 2, 52, 16, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 104);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 121);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 54, 104);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 54, 121);
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::InitStore()
{
	SPWindowStatic* pkParent, *pkChild[3];
	SPWindowButton* pkButton;
	SPWindowComboBox* pkComboBox;
	SPWindowPageSelect* pkPageSelect;
	int i;

	pkParent = new SPWindowStatic(WIID_CASHSHOP_STORE, 242, 6, 551, 514, this);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_STORE_TOP, 0, 0, 551, 48, pkParent);
	pkChild[0]->SetWindowType(WND_TYPE_LEFTBASE);
	pkChild[0]->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 306, 1, 311, 74);
	pkChild[0]->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 313, 1, 316, 74);
	pkChild[0]->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 318, 1, 323, 74);

	//pkChild[1] = new SPWindowStatic(WIID_CASHSHOP_STORE_LEFT, 0, 73, 2, 452, pkParent);
	//pkChild[1]->SetWindowType(WND_TYPE_TOPBASE);
	//pkChild[1]->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 384, 91, 386, 92);
	//pkChild[1]->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 384, 92, 386, 93);
	//pkChild[1]->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 384, 93, 386, 94);

	//pkChild[1] = new SPWindowStatic(WIID_CASHSHOP_STORE_RIGHT, 547, 73, 5, 452, pkParent);
	//pkChild[1]->SetWindowType(WND_TYPE_TOPBASE);
	//pkChild[1]->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 397, 91, 402, 92);
	//pkChild[1]->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 397, 92, 402, 93);
	//pkChild[1]->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 397, 93, 402, 94);

	pkChild[2] = new SPWindowStatic(WIID_CASHSHOP_STORE_BUTTOM, 0, 73, 551, 442, pkParent);
	pkChild[2]->SetWindowType(WND_TYPE_LEFTBASE);
	pkChild[2]->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 341, 1, 346, 442);
	pkChild[2]->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 348, 1, 351, 442);
	pkChild[2]->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 353, 1, 358, 442);

	//pkChild[2] = new SPWindowStatic(WIID_CASHSHOP_STORE_MID, 2, 73, 545, 454, pkParent);
	//pkChild[2]->SetWindowType(WND_TYPE_LEFTBASE);
	//pkChild[2]->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 508, 1, 509, 455);
	//pkChild[2]->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 509, 1, 510, 455);
	//pkChild[2]->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 510, 1, 511, 455);

#ifdef __CASHSHOP_POINT_SHOP__
	if( m_iMileageRatio )
	{
		pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_POINT, 450, 5, 74, 47, pkChild[0]);
		pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 386, 35);
		pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 352, 103);
		pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 417, 103);
		pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 417, 103);
		pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 321, 35);
	}
#endif

	//TAB标签
	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_HELP, 380, 5, 74, 47, pkChild[0]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 283);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 283);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 330);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 330);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 283);

	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_SEARCH, 317, 5, 74, 47, pkChild[0]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 76, 189);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 76, 189);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 76, 236);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 76, 236);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 76, 189);

	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_PACKAGE, 254, 5, 74, 47, pkChild[0]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 189);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 189);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 236);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 236);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 189);

	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_ETC, 191, 5, 74, 47, pkChild[0]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 76, 95);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 76, 95);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 76, 142);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 76, 142);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 76, 95);

	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_PET, 128, 5, 74, 47, pkChild[0]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 95);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 95);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 142);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 142);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 95);

	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_FASSION, 65, 5, 74, 47, pkChild[0]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 76, 1);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 76, 1);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 76, 48);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 76, 48);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 76, 1);

	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_EVENT, 2, 5, 74, 47, pkChild[0]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 1);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 1);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 48);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 48);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 1);

	//横条
	pkChild[1] = new SPWindowStatic(WIID_CASHSHOP_STORE_LIST_SKIN, 2, 47, 547, 26, pkChild[0]);
	pkChild[1]->SetWindowType(WND_TYPE_LEFTBASE);
	pkChild[1]->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 306, 75, 309, 101);
	pkChild[1]->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 311, 75, 312, 101);
	pkChild[1]->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 314, 75, 322, 101);

	//活动
	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_EVENT_NEW, 2, 8, 47, 12, pkChild[1]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 241, 338);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 241, 364);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 241, 351);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 241, 351);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 241, 338);

	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_EVENT_FAVORITE, 67, 8, 47, 12, pkChild[1]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 289, 338);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 289, 364);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 289, 351);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 289, 351);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 289, 338);

	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_EVENT_EVENT, 132, 8, 47, 12, pkChild[1]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 289, 377);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 289, 403);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 289, 390);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 289, 390);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 289, 377);

	//时装
	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_FASSION_DRESS, 2, 8, 47, 12, pkChild[1]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 377);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 403);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 390);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 390);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 377);

	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_FASSION_HELMET, 67, 8, 47, 12, pkChild[1]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 49, 377);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 49, 403);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 49, 390);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 49, 390);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 49, 377);

	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_FASSION_SHOES, 132, 8, 47, 12, pkChild[1]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 97, 377);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 97, 403);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 97, 390);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 97, 390);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 97, 377);

	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_FASSION_GLOVE, 197, 8, 47, 12, pkChild[1]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 145, 377);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 145, 403);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 145, 390);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 145, 390);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 145, 377);

	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_FASSION_ACCE, 262, 8, 47, 12, pkChild[1]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 193, 377);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 193, 403);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 193, 390);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 193, 390);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 193, 377);

	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_FASSION_ETC, 327, 8, 47, 12, pkChild[1]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 241, 377);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 241, 403);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 241, 390);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 241, 390);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 241, 377);

	//宠物
	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_PET_PET, 2, 8, 47, 12, pkChild[1]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 97, 299);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 97, 325);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 97, 312);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 97, 312);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 97, 299);

	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_PET_ETC, 67, 8, 47, 12, pkChild[1]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 145, 299);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 145, 325);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 145, 312);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 145, 312);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 145, 299);

	//功能
	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_ETC_FUNCTIONAL, 2, 8, 47, 12, pkChild[1]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 193, 299);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 193, 325);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 193, 312);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 193, 312);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 193, 299);

	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_ETC_BEAUTY, 67, 8, 47, 12, pkChild[1]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 241, 299);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 241, 325);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 241, 312);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 241, 312);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 214, 299);

	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_ETC_COMMUNITY, 132, 8, 47, 12, pkChild[1]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 289, 299);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 289, 325);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 289, 312);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 289, 312);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 289, 299);

	//背包
	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_PACK_FASSION, 2, 8, 47, 12, pkChild[1]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 97, 338);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 97, 364);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 97, 351);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 97, 351);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 97, 338);

	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_PACK_PET, 67, 8, 47, 12, pkChild[1]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 145, 338);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 145, 364);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 145, 364);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 145, 351);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 145, 338);

	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_PACK_ETC, 132, 8, 47, 12, pkChild[1]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 193, 338);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 193, 364);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 193, 364);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 193, 351);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 193, 338);

	//[xialin 2008/07/17] 搜索功能 >>>
	SPWindowStatic * pkStatic;
	SPWindowEdit * pkEdit;
	pkStatic = new SPWindowStatic(WIID_CASHSHOP_STORE_SEARCH_STATIC, 40, 8, 80, 12, pkChild[1]);
	pkStatic->SetFont(FONT_12_BOLD);
	pkStatic->SetFontColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	pkStatic->SetWindowText("搜索关键词:");

	pkStatic = new SPWindowStatic(WIID_CASHSHOP_STORE_SEARCH_EDIT_BG, 120, 6, 142, 16, pkChild[1]);
	pkStatic->SetWindowType(WND_TYPE_LEFTBASE);
	pkStatic->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 382, 1, 386, 17);
	pkStatic->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 390, 1, 394, 17);
	pkStatic->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 498, 1, 502, 17);

	pkEdit = new SPWindowEdit(WIID_CASHSHOP_STORE_SEARCH_EDIT, 2, 2, 138, 12, pkStatic);
	pkEdit->SetLimitText(MAX_LEN_NAME);

	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_SEARCH_SEARCH, 270, 5, 47, 17, pkChild[1]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 360, 356);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 360, 374);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 360, 392);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 360, 410);
	//[xialin 2008/07/17] 搜索功能 <<<

	//////////////////////////////////////////////////////////////////////////

#ifdef __CASHSHOP_POINT_SHOP__
	if( m_iMileageRatio )
	{
		pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_POINT_FASSION, 2, 8, 59, 19, pkChild[1]);
		pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 301, 352);
		pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 301, 372);
		pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 301, 392);
		pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 301, 392);
		pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 301, 412);

		pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_POINT_PET, 67, 8, 59, 19, pkChild[1]);
		pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 361, 352);
		pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 361, 372);
		pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 361, 392);
		pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 361, 392);
		pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 361, 412);

		pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_POINT_FUNCTIONAL, 132, 8, 59, 19, pkChild[1]);
		pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 61, 352);
		pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 61, 372);
		pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 61, 392);
		pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 61, 392);
		pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 61, 412);
	}
#endif

	//////////////////////////////////////////////////////////////////////////

	InitStorePopup(pkChild[1]);

	//pkChild[1] = new SPWindowStatic(WIID_CASHSHOP_STORE_LIST_WHITE_BG, 2, 35, 510, 374, pkChild[0]);
	//pkChild[1]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 465, 107);
	//pkChild[1]->SetSrcSize(3, 3);

	for( i = 0; i < 10; i++ )
	{
		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_STORE_ITEM + i,
			8 + (i % 2) * 229, 6 + (i / 2) * 82, 226, 79, pkChild[2]);
	}

	//增加排行榜
	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_SORT_TITLE, 468, 6, 76, 46, pkChild[2]);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP05.PNG", 229, 1);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_SORT, 468, 54, 76, 336, pkChild[2]);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP05.PNG", 435, 175);

	for ( int i=0; i<5; i++ )
	{
		pkStatic = new SPWindowStatic(WIID_CASHSHOP_SORT_ICON + i, 
			38 + 1, 3 + i*66, 32, 32, pkChild[0]);
		
		pkStatic = new SPWindowStatic(WIID_CASHSHOP_SORT_NAME + i, 
			3 + 1, 39 + i*66, 68, 12, pkChild[0]);
		pkStatic->SetFontColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		pkStatic->SetFormat(DT_VCENTER | DT_LEFT);

		pkStatic = new SPWindowStatic(WIID_CASHSHOP_SORT_PRICE + i, 
			3 + 1, 55 + i*66, 68, 12, pkChild[0]);
		pkStatic->SetFontColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
		pkStatic->SetFormat( DT_VCENTER | DT_RIGHT );
	} 

	//
	pkComboBox = new SPWindowComboBox(WIID_CASHSHOP_STORE_SORT_PRICE, 468, 397, 74/*89*/, 17, pkChild[2]);

	pkComboBox->SetDropDownOffset(64/*73*/, 7);
	pkComboBox->SetDropDownSize(9, 6);
	pkComboBox->SetDropDownTexture( COMBOBOX_DATA_TEXTURE_ENABLE	, "DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG" , 501 , 148 );
	pkComboBox->SetDropDownTexture( COMBOBOX_DATA_TEXTURE_HIT		, "DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG" , 501 , 148 );
	pkComboBox->SetDropDownTexture( COMBOBOX_DATA_TEXTURE_PUSH		, "DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG" , 501 , 155 );
	pkComboBox->SetDropDownTexture( COMBOBOX_DATA_TEXTURE_DISABLE	, "DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG" , 501 , 155 );
	pkComboBox->SetScroll(FALSE);
	pkComboBox->SetDropDownStatus(COMBOBOX_DATA_TEXTURE_ENABLE);

	pkComboBox->SetListBoxItemMargin(1, 1, 1, 1);
	pkComboBox->SetShowEditBoxText(TRUE);
	pkComboBox->SetEditBoxOffset(0, 0);
	pkComboBox->SetEditBoxSize( 78/*89*/, 17 );
	pkComboBox->SetEditBoxTextColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
	pkComboBox->SetEditBoxTexture( COMBOBOX_DATA_TEXTURE_ENABLE	, "DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG" , 494, 84, 511, 101 );
	pkComboBox->SetEditBoxTexture( COMBOBOX_DATA_TEXTURE_HIT	, "DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG" , 458, 102, 475, 119 );
	pkComboBox->SetEditBoxTexture( COMBOBOX_DATA_TEXTURE_PUSH	, "DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG" , 476, 102, 493, 119 );

	pkComboBox->SetListBoxOffset(0, 17);
	pkComboBox->SetListBoxTexture( "DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 494 , 102 , 511 , 119 );
	pkComboBox->SetRenderItemCountMax(3);

	SPComboBoxItemTexture* pkComboItem;
	pkComboItem = new SPComboBoxItemTexture;
	pkComboItem->SetSize(76/*87*/, 14);
	pkComboItem->SetText(g_pResourceManager->GetGlobalString(9000005));
	pkComboItem->SetTextColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
	pkComboItem->SetTexture(COMBOBOX_DATA_TEXTURE_HIT, "DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 499 , 92 , 500 , 93 );
	pkComboBox->AddItem(pkComboItem);

	pkComboItem = new SPComboBoxItemTexture;
	pkComboItem->SetSize(76/*87*/, 14);
	pkComboItem->SetText(g_pResourceManager->GetGlobalString(9000008));
	pkComboItem->SetTextColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
	pkComboItem->SetTexture(COMBOBOX_DATA_TEXTURE_HIT, "DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 499 , 92 , 500 , 93 );
	pkComboBox->AddItem(pkComboItem);

	pkComboItem = new SPComboBoxItemTexture;
	pkComboItem->SetSize(76/*87*/, 14);
	pkComboItem->SetText(g_pResourceManager->GetGlobalString(9000009));
	pkComboItem->SetTextColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
	pkComboItem->SetTexture(COMBOBOX_DATA_TEXTURE_HIT, "DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 499 , 92 , 500 , 93 );
	pkComboBox->AddItem(pkComboItem);

	//
	pkComboBox = new SPWindowComboBox(WIID_CASHSHOP_STORE_SORT_GENDER, 468, 416, 74/*85*/, 17, pkChild[2]);

	pkComboBox->SetDropDownOffset(64/*73*/, 7);
	pkComboBox->SetDropDownSize(9, 6);
	pkComboBox->SetDropDownTexture( COMBOBOX_DATA_TEXTURE_ENABLE	, "DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG" , 501 , 148 );
	pkComboBox->SetDropDownTexture( COMBOBOX_DATA_TEXTURE_HIT		, "DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG" , 501 , 148 );
	pkComboBox->SetDropDownTexture( COMBOBOX_DATA_TEXTURE_PUSH		, "DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG" , 501 , 155 );
	pkComboBox->SetDropDownTexture( COMBOBOX_DATA_TEXTURE_DISABLE	, "DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG" , 501 , 155 );
	pkComboBox->SetScroll(FALSE);
	pkComboBox->SetDropDownStatus(COMBOBOX_DATA_TEXTURE_ENABLE);

	pkComboBox->SetListBoxItemMargin(1, 1, 1, 1);
	pkComboBox->SetShowEditBoxText(TRUE);
	pkComboBox->SetEditBoxOffset(0, 0);
	pkComboBox->SetEditBoxSize( 78/*89*/, 17 );
	pkComboBox->SetEditBoxTextColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
	pkComboBox->SetEditBoxTexture( COMBOBOX_DATA_TEXTURE_ENABLE	, "DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG" , 494, 84, 511, 101 );
	pkComboBox->SetEditBoxTexture( COMBOBOX_DATA_TEXTURE_HIT	, "DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG" , 458, 102, 475, 119 );
	pkComboBox->SetEditBoxTexture( COMBOBOX_DATA_TEXTURE_PUSH	, "DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG" , 476, 102, 493, 119 );

	pkComboBox->SetListBoxOffset(0, 17);
	pkComboBox->SetListBoxTexture( "DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 494 , 102 , 511 , 119 );
	pkComboBox->SetRenderItemCountMax(3);

	pkComboItem = new SPComboBoxItemTexture;
	pkComboItem->SetSize(76/*87*/, 14);
	pkComboItem->SetText(g_pResourceManager->GetGlobalString(9000005));
	pkComboItem->SetTextColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
	pkComboItem->SetTexture(COMBOBOX_DATA_TEXTURE_HIT, "DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 499 , 92 , 500 , 93 );
	pkComboBox->AddItem(pkComboItem);

	pkComboItem = new SPComboBoxItemTexture;
	pkComboItem->SetSize(76/*87*/, 14);
	pkComboItem->SetText(g_pResourceManager->GetGlobalString(9000006));
	pkComboItem->SetTextColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
	pkComboItem->SetTexture(COMBOBOX_DATA_TEXTURE_HIT, "DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 499 , 92 , 500 , 93 );
	pkComboBox->AddItem(pkComboItem);

	pkComboItem = new SPComboBoxItemTexture;
	pkComboItem->SetSize(76/*87*/, 14);
	pkComboItem->SetText(g_pResourceManager->GetGlobalString(9000007));
	pkComboItem->SetTextColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
	pkComboItem->SetTexture(COMBOBOX_DATA_TEXTURE_HIT, "DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 499 , 92 , 500 , 93 );
	pkComboBox->AddItem(pkComboItem);

	//
	pkPageSelect = new SPWindowPageSelect(WIID_CASHSHOP_STORE_PAGE, /*0*/46, 420, /*510*/400, 14, pkChild[2]);
	//pkPageSelect->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 465, 107);
	//pkPageSelect->SetSrcSize(2, 2);
	
	pkPageSelect->SetPageButtonSize(PAGE_SELECT_BUNDLE_PREV, 14, 14);
	pkPageSelect->SetPageButtonImage(PAGE_SELECT_BUNDLE_PREV, "DATA/INTERFACE/CONCEPT/CASHSHOP05.PNG", 361, 123);
	pkPageSelect->SetPageButtonImageHit(PAGE_SELECT_BUNDLE_PREV, "DATA/INTERFACE/CONCEPT/CASHSHOP05.PNG", 361, 138);
	pkPageSelect->SetPageButtonImagePush(PAGE_SELECT_BUNDLE_PREV, "DATA/INTERFACE/CONCEPT/CASHSHOP05.PNG", 361, 153);
	pkPageSelect->SetPageButtonImageDisable(PAGE_SELECT_BUNDLE_PREV, "DATA/INTERFACE/CONCEPT/CASHSHOP05.PNG", 361, 168);

	pkPageSelect->SetPageButtonSize(PAGE_SELECT_PREV, 14, 14);
	pkPageSelect->SetPageButtonImage(PAGE_SELECT_PREV, "DATA/INTERFACE/CONCEPT/CASHSHOP05.PNG", 376, 123);
	pkPageSelect->SetPageButtonImageHit(PAGE_SELECT_PREV, "DATA/INTERFACE/CONCEPT/CASHSHOP05.PNG", 376, 138);
	pkPageSelect->SetPageButtonImagePush(PAGE_SELECT_PREV, "DATA/INTERFACE/CONCEPT/CASHSHOP05.PNG", 376, 153);
	pkPageSelect->SetPageButtonImageDisable(PAGE_SELECT_PREV, "DATA/INTERFACE/CONCEPT/CASHSHOP05.PNG", 376, 168);

	pkPageSelect->SetPageButtonSize(PAGE_SELECT_NEXT, 14, 14);
	pkPageSelect->SetPageButtonImage(PAGE_SELECT_NEXT, "DATA/INTERFACE/CONCEPT/CASHSHOP05.PNG", 391, 123);
	pkPageSelect->SetPageButtonImageHit(PAGE_SELECT_NEXT, "DATA/INTERFACE/CONCEPT/CASHSHOP05.PNG", 391, 138);
	pkPageSelect->SetPageButtonImagePush(PAGE_SELECT_NEXT, "DATA/INTERFACE/CONCEPT/CASHSHOP05.PNG", 391, 153);
	pkPageSelect->SetPageButtonImageDisable(PAGE_SELECT_NEXT, "DATA/INTERFACE/CONCEPT/CASHSHOP05.PNG", 391, 168);

	pkPageSelect->SetPageButtonSize(PAGE_SELECT_BUNDLE_NEXT, 14, 14);
	pkPageSelect->SetPageButtonImage(PAGE_SELECT_BUNDLE_NEXT, "DATA/INTERFACE/CONCEPT/CASHSHOP05.PNG", 406, 123);
	pkPageSelect->SetPageButtonImageHit(PAGE_SELECT_BUNDLE_NEXT, "DATA/INTERFACE/CONCEPT/CASHSHOP05.PNG", 406, 138);
	pkPageSelect->SetPageButtonImagePush(PAGE_SELECT_BUNDLE_NEXT, "DATA/INTERFACE/CONCEPT/CASHSHOP05.PNG", 406, 153);
	pkPageSelect->SetPageButtonImageDisable(PAGE_SELECT_BUNDLE_NEXT, "DATA/INTERFACE/CONCEPT/CASHSHOP05.PNG", 406, 168);

	pkPageSelect->SetNumberColor(D3DXCOLOR(0.2705f, 0.3686f, 0.4392f, 1.0f));
	pkPageSelect->SetControlInterval(12);
	pkPageSelect->SetNumberSize(14, 12);

}

//------------------------------------------------------------------------------------
void SPWindowCashShop::InitStorePopup(SPWindow* pkParent)
{
	if( pkParent == NULL )
		return;

	SPWindowStatic* pkChild[2];
	SPWindowButton* pkButton;

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_STORE_FAS_DRESS_POP, 2, 23, 104, 107, pkParent);
	pkChild[0]->SetWindowType(WND_TYPE_TOPBASE);
	pkChild[0]->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 64, 105, 72);
	pkChild[0]->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 74, 105, 76);
	pkChild[0]->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 78, 105, 86);

	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_FAS_DRESS_TOTAL, 10, 10, 81, 15, pkChild[0]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 256);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 272);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 288);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 288);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 304);

	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_FAS_DRESS_ONE, 10, 27, 81, 15, pkChild[0]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 83, 256);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 83, 272);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 83, 288);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 83, 288);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 83, 304);

	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_FAS_DRESS_BLO, 10, 44, 81, 15, pkChild[0]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 165, 256);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 165, 272);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 165, 288);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 165, 288);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 165, 304);

	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_FAS_DRESS_PAN, 10, 61, 81, 15, pkChild[0]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 247, 256);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 247, 272);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 247, 288);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 247, 288);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 247, 304);

	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_FAS_DRESS_COAT, 10, 78, 81, 15, pkChild[0]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 329, 256);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 329, 272);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 329, 288);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 329, 288);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 329, 304);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_STORE_FAS_HELMET_POP, 67, 23, 104, 107, pkParent);
	pkChild[0]->SetWindowType(WND_TYPE_TOPBASE);
	pkChild[0]->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 64, 105, 72);
	pkChild[0]->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 74, 105, 76);
	pkChild[0]->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 78, 105, 86);

	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_FAS_HELMET_TOTAL, 10, 10, 81, 15, pkChild[0]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 256);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 272);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 288);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 288);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 304);

	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_FAS_HELMET_HELM, 10, 27, 81, 15, pkChild[0]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 411, 256);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 411, 272);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 411, 288);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 411, 288);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 411, 304);

	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_FAS_HELMET_BAND, 10, 44, 81, 15, pkChild[0]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 320);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 336);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 352);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 352);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 368);

	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_FAS_HELMET_PIN, 10, 61, 81, 15, pkChild[0]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 83, 320);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 83, 336);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 83, 352);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 83, 352);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 83, 368);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_STORE_FAS_SHOES_POP, 132, 23, 104, 107, pkParent);
	pkChild[0]->SetWindowType(WND_TYPE_TOPBASE);
	pkChild[0]->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 64, 105, 72);
	pkChild[0]->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 74, 105, 76);
	pkChild[0]->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 78, 105, 86);

	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_FAS_SHOES_TOTAL, 10, 10, 81, 15, pkChild[0]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 256);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 272);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 288);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 288);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 304);

	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_FAS_SHOES_SHOES, 10, 27, 81, 15, pkChild[0]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 165, 320);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 165, 336);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 165, 352);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 165, 352);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 165, 368);

	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_FAS_SHOES_BAND, 10, 44, 81, 15, pkChild[0]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 247, 320);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 247, 336);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 247, 352);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 247, 352);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 247, 368);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_STORE_FAS_GLOVE_POP, 197, 23, 104, 107, pkParent);
	pkChild[0]->SetWindowType(WND_TYPE_TOPBASE);
	pkChild[0]->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 64, 105, 72);
	pkChild[0]->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 74, 105, 76);
	pkChild[0]->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 78, 105, 86);
	
	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_FAS_GLOVE_TOTAL, 10, 10, 81, 15, pkChild[0]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 256);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 272);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 288);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 288);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 304);

	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_FAS_GLOVE_GLOVE, 10, 27, 81, 15, pkChild[0]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 329, 320);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 329, 336);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 329, 352);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 329, 352);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 329, 368);

	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_FAS_GLOVE_BAND, 10, 44, 81, 15, pkChild[0]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 411, 320);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 411, 336);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 411, 352);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 411, 352);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 411, 368);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_STORE_FAS_ACCE_POP, 262, 23, 104, 107, pkParent);
	pkChild[0]->SetWindowType(WND_TYPE_TOPBASE);
	pkChild[0]->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 64, 105, 72);
	pkChild[0]->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 74, 105, 76);
	pkChild[0]->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 78, 105, 86);
	
	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_FAS_ACCE_TOTAL, 10, 10, 81, 15, pkChild[0]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 256);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 272);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 288);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 288);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 304);

	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_FAS_ACCE_GLASS, 10, 27, 81, 15, pkChild[0]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 384);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 400);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 416);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 416);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 432);

	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_FAS_ACCE_EARING, 10, 44, 81, 15, pkChild[0]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 83, 384);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 83, 400);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 83, 416);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 83, 416);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 83, 432);

	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_FAS_ACCE_MAKEUP, 10, 61, 81, 15, pkChild[0]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 165, 384);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 165, 400);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 165, 416);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 165, 416);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 165, 432);

	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_FAS_ACCE_STOCK, 10, 78, 81, 15, pkChild[0]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 247, 384);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 247, 400);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 247, 416);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 247, 416);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 247, 432);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_STORE_ETC_COMMU_POP, 132, 23, 104, 107, pkParent);
	pkChild[0]->SetWindowType(WND_TYPE_TOPBASE);
	pkChild[0]->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 64, 105, 72);
	pkChild[0]->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 74, 105, 76);
	pkChild[0]->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 78, 105, 86);
	
	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_ETC_COMMU_TOTAL, 10, 10, 81, 15, pkChild[0]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 256);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 272);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 288);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 288);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 304);

	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_ETC_COMMU_EMOTI, 10, 27, 81, 15, pkChild[0]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 329, 384);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 329, 400);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 329, 416);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 329, 416);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 329, 432);

	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_ETC_COMMU_EXPRE, 10, 44, 81, 15, pkChild[0]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 448);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 464);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 480);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 480);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 496);

	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_ETC_COMMU_EVENT, 10, 61, 81, 15, pkChild[0]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 83, 448);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 83, 464);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 83, 480);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 83, 480);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 83, 496);

	pkButton = new SPWindowButton(WIID_CASHSHOP_STORE_ETC_COMMU_CHAT, 10, 78, 81, 15, pkChild[0]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 411, 384);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 411, 400);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 411, 416);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 411, 416);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 411, 432);
	pkButton->SetEnable(false);
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::InitPrevView()
{
	SPWindowStatic* pkParent, *pkChild[2];
	SPWindowButton* pkButton;
	SPWindowGrid* pkGrid;

	pkParent = new SPWindowStatic(WIID_CASHSHOP_PREVIEW, 196, 88, 88, 290, this);
	pkParent->SetWindowType(WND_TYPE_TOPBASE);
	pkParent->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 184, 16, 272, 20);
	pkParent->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 184, 22, 272, 24);
	pkParent->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 184, 26, 272, 30);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_PREVIEW_TITLE, 9, 4, 70, 14, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 273, 16);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_PREVIEW_WHITE_BG, 2, 23, 84, 263, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 465, 107);
	pkChild[0]->SetSrcSize(2, 2);

	//pkButton = new SPWindowButton(WIID_CASHSHOP_PREVIEW_LEFT, 5, 8, 14, 14, pkChild[0]);
	//pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 376, 123);
	//pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 376, 138);
	//pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 376, 153);
	//pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 376, 168);

	//pkButton = new SPWindowButton(WIID_CASHSHOP_PREVIEW_RIGHT, 64, 8, 14, 14, pkChild[0]);
	//pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 391, 123);
	//pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 391, 138);
	//pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 391, 153);
	//pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 391, 168);

	//pkChild[1] = new SPWindowStatic(WIID_CASHSHOP_PREVIEW_PAGE_TEXT, 17, 9, 49, 12, pkChild[0]);
	//pkChild[1]->SetFontColor(D3DXCOLOR(0.2705f, 0.3686f, 0.4392f, 1.0f));
	//pkChild[1]->SetFormat(DT_VCENTER | DT_CENTER);

	pkGrid = new SPWindowGrid(WNDID_CTRL_GRID, WIID_CASHSHOP_PREVIEW_SLOT,
		6, 28, 73, 146, pkChild[0], 2, 4, 41, 38, 32);
	pkGrid->SetGridImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 443, 137, 35, 11);
	pkGrid->SetGridImageOffset(0, 24);
	pkGrid->ClearCellAll();

	pkChild[1] = new SPWindowStatic(WIID_CASHSHOP_PREVIEW_PRICE_BG, 8, 182, 68, 14, pkChild[0]);
	pkChild[1]->SetWindowType(WND_TYPE_LEFTBASE);
	pkChild[1]->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 493, 497, 495, 511);
	pkChild[1]->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 497, 497, 499, 511);
	pkChild[1]->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 501, 497, 503, 511);
	
	pkChild[1] = new SPWindowStatic(WIID_CASHSHOP_PREVIEW_OZ_IMAGE, 57, 184, 15, 11, pkChild[0]);
	if( m_iMileageRatio )
		pkChild[1]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 113);
	else
		pkChild[1]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 101);

	pkChild[1] = new SPWindowStatic(WIID_CASHSHOP_PREVIEW_PRICE, 9, 184, 44, 12, pkChild[0]);
	pkChild[1]->SetFontColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	pkChild[1]->SetFormat(DT_VCENTER | DT_RIGHT);

	pkButton = new SPWindowButton(WIID_CASHSHOP_PREVIEW_BUY, 8, 202, 68, 17, pkChild[0]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 1, 31);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 1, 49);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 1, 67);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 1, 85);

	pkButton = new SPWindowButton(WIID_CASHSHOP_PREVIEW_DEL, 8, 221, 68, 17, pkChild[0]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 70, 31);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 70, 49);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 70, 67);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 70, 85);

	pkButton = new SPWindowButton(WIID_CASHSHOP_PREVIEW_INIT, 8, 240, 68, 17, pkChild[0]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 139, 31);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 139, 49);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 139, 67);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 139, 85);
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::InitCashierDlg()
{
	SPWindowStatic* pkParent, *pkChild[2];
	SPWindowButton* pkButton;
	SPWindowEdit* pkEdit;

	pkParent = new SPWindowStatic(WIID_CASHSHOP_CASHIER, 266, 176, 268, m_iMileageRatio ? 286 : 248, this);
	pkParent->SetWindowType(WND_TYPE_TOPBASE);
	pkParent->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 28, 269, 36);
	pkParent->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 38, 269, 44);
	pkParent->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 46, 269, 54);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_CASHIER_ITEM_SLOT, 22, 42, 35, 11, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 443, 137);

	pkChild[1] = new SPWindowStatic(WIID_CASHSHOP_CASHIER_ITEM, 2, -24, 32, 32, pkChild[0]);
	
	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_CASHIER_NAME, 69, 20, 175, 12, pkParent);
	pkChild[0]->SetFont(FONT_12_BOLD);
	pkChild[0]->SetFontColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
	pkChild[0]->SetFormat(DT_VCENTER | DT_LEFT);
	
	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_CASHIER_PRICE_BG, 70, 35, 80, 14, pkParent);
	pkChild[0]->SetWindowType(WND_TYPE_LEFTBASE);
	pkChild[0]->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 493, 497, 495, 511);
	pkChild[0]->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 497, 497, 499, 511);
	pkChild[0]->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 501, 497, 503, 511);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_CASHIER_OZ_ICON, 131, 37, 15, 11, pkParent);
	if( m_iMileageRatio )
	{
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 113);
#ifdef __CASHSHOP_POINT_SHOP__
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 125);
#endif
	}
	else
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 101);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_CASHIER_PRICE, 74, 37, 53, 12, pkParent);
	pkChild[0]->SetFontColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	pkChild[0]->SetFormat(DT_VCENTER | DT_RIGHT);

	if( m_iMileageRatio )
	{
		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_CASHIER_MY_OZ_TEXT, 22, 76, 96, 13, pkParent);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 385, 185);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_CASHIER_USE_OZ_TEXT, 22, 95, 96, 13, pkParent);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 353, 106);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_CASHIER_USE_MILE_TEXT, 22, 114, 96, 13, pkParent);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 353, 134);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_CASHIER_USE_PRI_TEXT, 22, 149, 96, 13, pkParent);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 353, 120);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_CASHIER_AF_MY_OZ_TEXT, 22, 168, 96, 13, pkParent);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 241, 242);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_CASHIER_MY_OZ_ICON, 201, 77, 15, 11, pkParent);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 113);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_CASHIER_USE_OZ_ICON, 201, 96, 15, 11, pkParent);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 113);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_CASHIER_USE_MILE_BG, 146, 112, 74, 16, pkParent);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 307, 1);
#ifdef __CASHSHOP_POINT_SHOP__
		pkChild[0]->SetImage( (SPTexture*)NULL );
#endif

		pkEdit = new SPWindowEdit(WIID_CASHSHOP_CASHIER_USE_MILE, -15, 3, 66, 12, pkChild[0]);
		pkEdit->SetFontColor(D3DXCOLOR(0.2745f, 0.5490f, 0.4862f, 1.0f));
		pkEdit->SetFormat(DT_VCENTER | DT_RIGHT);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_CASHIER_USE_MILE_ICON, 201, 115, 15, 11, pkParent);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 125);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_CASHIER_USE_PRI_ICON, 201, 150, 15, 11, pkParent);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 113);
#ifdef __CASHSHOP_POINT_SHOP__
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 125);
#endif

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_CASHIER_AF_MY_OZ_ICON, 201, 169, 15, 11, pkParent);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 113);
#ifdef __CASHSHOP_POINT_SHOP__
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 125);
#endif

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_CASHIER_MY_OZ, 131, 77, 66, 12, pkParent);
		pkChild[0]->SetFontColor(D3DXCOLOR(0.2745f, 0.5490f, 0.4862f, 1.0f));
		pkChild[0]->SetFormat(DT_VCENTER | DT_RIGHT);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_CASHIER_USE_OZ, 131, 96, 66, 12, pkParent);
		pkChild[0]->SetFontColor(D3DXCOLOR(0.2745f, 0.5490f, 0.4862f, 1.0f));
		pkChild[0]->SetFormat(DT_VCENTER | DT_RIGHT);
		
		//pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_CASHIER_USE_MILE, 131, 115, 66, 12, pkParent);
		//pkChild[0]->SetFontColor(D3DXCOLOR(0.2745f, 0.5490f, 0.4862f, 1.0f));
		//pkChild[0]->SetFormat(DT_VCENTER | DT_RIGHT);
		
		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_CASHIER_USE_PRI, 131, 150, 66, 12, pkParent);
		pkChild[0]->SetFont(FONT_12_BOLD);
		pkChild[0]->SetFontColor(D3DXCOLOR(0.2745f, 0.5490f, 0.4862f, 1.0f));
		pkChild[0]->SetFormat(DT_VCENTER | DT_RIGHT);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_CASHIER_AF_MY_OZ, 131, 169, 66, 12, pkParent);
		pkChild[0]->SetFontColor(D3DXCOLOR(0.2745f, 0.5490f, 0.4862f, 1.0f));
		pkChild[0]->SetFormat(DT_VCENTER | DT_RIGHT);

		pkButton = new SPWindowButton(WIID_CASHSHOP_CASHIER_PLUS, 225, 110, 13, 9, pkParent);
		pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 481, 407);
		pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 481, 417);
		pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 481, 427);
		pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 481, 437);

		pkButton = new SPWindowButton(WIID_CASHSHOP_CASHIER_MINUS, 225, 122, 13, 9, pkParent);
		pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 495, 407);
		pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 495, 417);
		pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 495, 427);
		pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 495, 437);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_CASHIER_TYPE, 13, 196, 239, 45, pkParent);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 261, 162);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 210);
#ifdef __CASHSHOP_POINT_SHOP__
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 1, 1);
#endif

		pkButton = new SPWindowButton(WIID_CASHSHOP_CASHIER_OK, 64, 247, 66, 17, pkParent);
		pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 280);
		pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 298);
		pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 316);
		pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 334);

		pkButton = new SPWindowButton(WIID_CASHSHOP_CASHIER_CANCEL, 141, 247, 66, 17, pkParent);
		pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 389, 280);
		pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 389, 298);
		pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 389, 316);
		pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 389, 334);
	}
	else
	{
		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_CASHIER_MY_OZ_TEXT, 22, 76, 96, 13, pkParent);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 241, 200);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_CASHIER_USE_OZ_TEXT, 22, 95, 96, 13, pkParent);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 353, 120);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_CASHIER_AF_MY_OZ_TEXT, 22, 130, 96, 13, pkParent);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 241, 242);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_CASHIER_MY_OZ_ICON, 201, 77, 15, 11, pkParent);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 101);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_CASHIER_USE_OZ_ICON, 201, 96, 15, 11, pkParent);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 101);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_CASHIER_AF_MY_OZ_ICON, 201, 131, 15, 11, pkParent);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 101);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_CASHIER_MY_OZ, 131, 77, 66, 12, pkParent);
		pkChild[0]->SetFontColor(D3DXCOLOR(0.2745f, 0.5490f, 0.4862f, 1.0f));
		pkChild[0]->SetFormat(DT_VCENTER | DT_RIGHT);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_CASHIER_USE_OZ, 131, 96, 66, 12, pkParent);
		pkChild[0]->SetFontColor(D3DXCOLOR(0.2745f, 0.5490f, 0.4862f, 1.0f));
		pkChild[0]->SetFormat(DT_VCENTER | DT_RIGHT);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_CASHIER_AF_MY_OZ, 131, 131, 66, 12, pkParent);
		pkChild[0]->SetFontColor(D3DXCOLOR(0.2745f, 0.5490f, 0.4862f, 1.0f));
		pkChild[0]->SetFormat(DT_VCENTER | DT_RIGHT);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_CASHIER_TYPE, 13, 158, 239, 45, pkParent);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 58);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 210);

		pkButton = new SPWindowButton(WIID_CASHSHOP_CASHIER_OK, 64, 209, 66, 17, pkParent);
		pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 280);
		pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 298);
		pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 316);
		pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 334);

		pkButton = new SPWindowButton(WIID_CASHSHOP_CASHIER_CANCEL, 141, 209, 66, 17, pkParent);
		pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 389, 280);
		pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 389, 298);
		pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 389, 316);
		pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 389, 334);
	}
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::InitBuyListDlg()
{
	SPWindowStatic* pkParent, *pkChild[2];
	SPWindowButton* pkButton;
	SPWindowEdit* pkEdit;

	pkParent = new SPWindowStatic(WIID_CASHSHOP_BUYLIST, 266, 155, 268, 286, this);
	pkParent->SetWindowType(WND_TYPE_TOPBASE);
	pkParent->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 28, 269, 36);
	pkParent->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 38, 269, 44);
	pkParent->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 46, 269, 54);

	pkChild[1] = new SPWindowStatic(WIID_CASHSHOP_WEARLIST_ITEM_BG, 2, 16, 261, 128, pkParent);
	pkChild[1]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 451, 121);
	pkChild[1]->SetSrcSize(2, 2);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_BUYLIST_TIP, 2, 80, 261, 12, pkParent);
	pkChild[0]->SetFontColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	pkChild[0]->SetFormat(DT_VCENTER | DT_CENTER);
	pkChild[0]->SetWindowText("没有试穿商品可购买!");

	for ( int i=0; i<3; i++ )
	{
		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_BUYLIST_ITEMBG + i, 22, 6+i*36, 200, 28, pkChild[1]);
	}

	pkButton = new SPWindowButton(WIID_CASHSHOP_BUYLIST_UP, 230, 84, 13, 9, pkChild[1]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 481, 407);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 481, 417);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 481, 427);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 481, 437);

	pkButton = new SPWindowButton(WIID_CASHSHOP_BUYLIST_DOWN, 230, 97, 13, 9, pkChild[1]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 495, 407);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 495, 417);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 495, 427);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 495, 437);

	//
	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_BUYLIST_MY_OZ_TEXT, 22, 170, 96, 13, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 241, 200);
	
	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_BUYLIST_MY_OZ, 131, 170, 66, 12, pkParent);
	pkChild[0]->SetFontColor(D3DXCOLOR(0.2745f, 0.5490f, 0.4862f, 1.0f));
	pkChild[0]->SetFormat(DT_VCENTER | DT_RIGHT);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_BUYLIST_MY_OZ_ICON, 201, 170, 15, 11, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 101);

	//
	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_BUYLIST_USE_OZ_TEXT, 22, 188, 96, 13, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 353, 120);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_BUYLIST_USE_OZ, 131, 188, 66, 12, pkParent);
	pkChild[0]->SetFontColor(D3DXCOLOR(0.2745f, 0.5490f, 0.4862f, 1.0f));
	pkChild[0]->SetFormat(DT_VCENTER | DT_RIGHT);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_BUYLIST_USE_OZ_ICON, 201, 188, 15, 11, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 101);

	//
	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_BUYLIST_AF_MY_OZ_TEXT, 22, 206, 96, 13, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 241, 242);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_BUYLIST_AF_MY_OZ, 131, 206, 66, 12, pkParent);
	pkChild[0]->SetFontColor(D3DXCOLOR(0.2745f, 0.5490f, 0.4862f, 1.0f));
	pkChild[0]->SetFormat(DT_VCENTER | DT_RIGHT);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_BUYLIST_AF_MY_OZ_ICON, 201, 206, 15, 11, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 101);

	//
	pkButton = new SPWindowButton(WIID_CASHSHOP_BUYLIST_OK, 64, 245, 66, 17, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 280);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 298);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 316);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 334);

	pkButton = new SPWindowButton(WIID_CASHSHOP_BUYLIST_CANCEL, 141, 245, 66, 17, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 389, 280);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 389, 298);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 389, 316);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 389, 334);

}

//------------------------------------------------------------------------------------
void SPWindowCashShop::InitWearListDlg()
{
	SPWindowStatic* pkParent, *pkChild[2];
	SPWindowButton* pkButton;
	SPWindowEdit* pkEdit;

	pkParent = new SPWindowStatic(WIID_CASHSHOP_WEARLIST, 266, 150, 268, 300, this);
	pkParent->SetWindowType(WND_TYPE_TOPBASE);
	pkParent->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 28, 269, 36);
	pkParent->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 38, 269, 44);
	pkParent->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 46, 269, 54);

	pkChild[1] = new SPWindowStatic(WIID_CASHSHOP_WEARLIST_ITEM_BG, 2, 16, 261, 202, pkParent);
	pkChild[1]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 451, 121);
	pkChild[1]->SetSrcSize(2, 2);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_WEARLIST_TIP, 2, 115, 261, 12, pkParent);
	pkChild[0]->SetFontColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	pkChild[0]->SetFormat(DT_VCENTER | DT_CENTER);
	pkChild[0]->SetWindowText("没有试穿商品可察看!");

	for ( int i=0; i<5; i++ )
	{
		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_WEARLIST_ITEMBG + i , 22, 6+i*36, 200, 28, pkChild[1]);
	}

	pkButton = new SPWindowButton(WIID_CASHSHOP_WEARLIST_UP, 230, 156, 13, 9, pkChild[1]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 481, 407);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 481, 417);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 481, 427);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 481, 437);

	pkButton = new SPWindowButton(WIID_CASHSHOP_WEARLIST_DOWN, 230, 169, 13, 9, pkChild[1]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 495, 407);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 495, 417);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 495, 427);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 495, 437);

	//
	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_WEARLIST_AF_OZ_TEXT, 22, 230, 30, 13, pkParent);
	pkChild[0]->SetFontColor(D3DXCOLOR(0.2745f, 0.5490f, 0.4862f, 1.0f));
	pkChild[0]->SetFormat(DT_VCENTER | DT_RIGHT);
	pkChild[0]->SetWindowText("合计:");

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_WEARLIST_AF_COUNT, 70, 230, 60, 12, pkParent);
	pkChild[0]->SetFontColor(D3DXCOLOR(0.2745f, 0.5490f, 0.4862f, 1.0f));
	pkChild[0]->SetFormat(DT_VCENTER | DT_RIGHT);
	pkChild[0]->SetWindowText("0件");

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_WEARLIST_AF_PRICE, 150, 230, 50, 12, pkParent);
	pkChild[0]->SetFontColor(D3DXCOLOR(0.2745f, 0.5490f, 0.4862f, 1.0f));
	pkChild[0]->SetFormat(DT_VCENTER | DT_RIGHT);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_WEARLIST_AF_OZ_ICON, 203, 230, 15, 11, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 101);

	//
	pkButton = new SPWindowButton(WIID_CASHSHOP_WEARLIST_OK, 101, 261, 66, 17, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 280);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 298);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 316);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 334);

}

//------------------------------------------------------------------------------------
void SPWindowCashShop::InitExactListDlg()
{
	SPWindowStatic* pkParent, *pkChild[2];
	SPWindowButton* pkButton;
	SPWindowEdit* pkEdit;
	SPWindowEditMultiLine* pkMultiEdit;
	SPWindowTreeBox* pkTreeBox;

	pkParent = new SPWindowStatic(WIID_CASHSHOP_EXACTLIST, 266, 136, 268, 328, this);
	pkParent->SetWindowType(WND_TYPE_TOPBASE);
	pkParent->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 28, 269, 36);
	pkParent->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 38, 269, 44);
	pkParent->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 46, 269, 54);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_EXACTLIST_TIP, 2, 150, 261, 12, pkParent);
	pkChild[0]->SetFontColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	pkChild[0]->SetFormat(DT_VCENTER | DT_CENTER);
	pkChild[0]->SetWindowText("没有试穿商品可索取!");

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_EXACTLIST_MESSAGE_GRN_BG, 2, 16, 261, 85, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 451, 121);
	pkChild[0]->SetSrcSize(2, 2);

	pkChild[1] = new SPWindowStatic(WIID_CASHSHOP_EXACTLIST_RECEIVE_BG, 89, 8, 129, 16, pkChild[0]);
	pkChild[1]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 382, 1);

	pkEdit = new SPWindowEdit(WIID_CASHSHOP_EXACTLIST_RECEIVE, 5, 1, 119, 12, pkChild[1]);
	pkEdit->SetLimitText(LEN_NAME);

	pkChild[1] = new SPWindowStatic(WIID_CASHSHOP_EXACTLIST_MESSAGE_BG, 89, 29, 129, 46, pkChild[0]);
	pkChild[1]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 382, 18);

	pkMultiEdit = new SPWindowEditMultiLine(WIID_CASHSHOP_EXACTLIST_MESSAGE, 5, 2, 119, 43, pkChild[1]);
	pkMultiEdit->SetLimitText(50);

	pkButton = new SPWindowButton(WIID_CASHSHOP_EXACTLIST_FRIEND_BTN, 222, 8, 35, 16, pkChild[0]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 310, 18);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 346, 18);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 310, 35);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 346, 35);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_EXACTLIST_RECEIVE_IMG, 22, 24, 62, 13, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 449, 78);
	
	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_EXACTLIST_MESSAGE_IMG, 22, 45, 62, 13, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 450, 134);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_EXACTLIST_FRIEND, 263, 64, 185, 149, pkParent);
	pkChild[0]->SetWindowType(WND_TYPE_TOPBASE);
	pkChild[0]->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 106, 55, 291, 63);
	pkChild[0]->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 106, 66, 291, 70);
	pkChild[0]->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 106, 73, 291, 81);

	pkChild[1] = new SPWindowStatic(WIID_CASHSHOP_EXACTLIST_FRIEND_LIST_BG, 8, 11, 164, 124, pkChild[0]);
	pkChild[1]->SetWindowType(WND_TYPE_TOPBASE);
	pkChild[1]->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 106, 82, 270, 86);
	pkChild[1]->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 106, 85, 270, 88);
	pkChild[1]->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 106, 87, 270, 91);

	
	pkTreeBox =	new SPWindowTreeBox(WIID_CASHSHOP_EXACTLIST_FRIEND_LIST, 0, 0, 164, 124, pkChild[1]);
	pkTreeBox->SetMargin(13, 9, 0, 0);
	pkTreeBox->UpdateBGPos();
	pkTreeBox->SetDoubleClick(true);
	pkTreeBox->SetScrollAutoHide(true);
	pkTreeBox->SetInterval(2);

	pkTreeBox->SetScrollUpSize(12, 11);
	pkTreeBox->SetScrollUpTexture( DATA_TEXTURE_ENABLE		, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 456 , 389 );
	pkTreeBox->SetScrollUpTexture( DATA_TEXTURE_HIT			, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 469 , 389 );
	pkTreeBox->SetScrollUpTexture( DATA_TEXTURE_PUSH		, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 482 , 389 );
	pkTreeBox->SetScrollUpTexture( DATA_TEXTURE_DISABLE		, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 495 , 389 );

	pkTreeBox->SetScrollDownSize( 12 , 11 );
	pkTreeBox->SetScrollDownTexture( DATA_TEXTURE_ENABLE	, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 456 , 431 );
	pkTreeBox->SetScrollDownTexture( DATA_TEXTURE_HIT		, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 469 , 431 );
	pkTreeBox->SetScrollDownTexture( DATA_TEXTURE_PUSH		, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 482 , 431 );
	pkTreeBox->SetScrollDownTexture( DATA_TEXTURE_DISABLE	, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 495 , 431 );

	pkTreeBox->SetScrollSliderSize( 12 , 23 );
	pkTreeBox->SetScrollSliderTexture( DATA_TEXTURE_ENABLE	, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 456 , 401 );
	pkTreeBox->SetScrollSliderTexture( DATA_TEXTURE_HIT		, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 469 , 401 );
	pkTreeBox->SetScrollSliderTexture( DATA_TEXTURE_PUSH	, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 482 , 401 );
	pkTreeBox->SetScrollSliderTexture( DATA_TEXTURE_DISABLE	, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 459 , 401 );

	pkTreeBox->SetScrollSliderBGTexture( DATA_TEXTURE_ENABLE, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 456 , 427 , 468 , 429 );
	pkTreeBox->SetScrollSliderBGTexture( DATA_TEXTURE_HIT	, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 469 , 427 , 481 , 429 );
	pkTreeBox->SetScrollSliderBGTexture( DATA_TEXTURE_PUSH	, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 482 , 427 , 494 , 429 );
	pkTreeBox->SetScrollSliderBGTexture( DATA_TEXTURE_DISABLE, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 495 , 427 , 507 , 429 );

	pkTreeBox->SetScrollMargin( 0 , 0 , 0 , 0 );
	pkTreeBox->UpdateScrollPos();

	pkTreeBox->Refresh();

	//
	for ( int i=0; i<3; i++ )
	{
		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_EXACTLIST_ITEMBG + i, 22, 106+i*36, 200, 28, pkParent);
	}

	pkButton = new SPWindowButton(WIID_CASHSHOP_EXACTLIST_UP, 230, 184, 13, 9, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 481, 407);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 481, 417);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 481, 427);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 481, 437);

	pkButton = new SPWindowButton(WIID_CASHSHOP_EXACTLIST_DOWN, 230, 197, 13, 9, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 495, 407);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 495, 417);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 495, 427);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 495, 437);

	//
	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_EXACTLIST_USE_OZ_TEXT, 22, 232, 96, 13, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 353, 120);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_EXACTLIST_USE_OZ, 131, 232, 66, 12, pkParent);
	pkChild[0]->SetFontColor(D3DXCOLOR(0.2745f, 0.5490f, 0.4862f, 1.0f));
	pkChild[0]->SetFormat(DT_VCENTER | DT_RIGHT);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_EXACTLIST_USE_OZ_ICON, 201, 232, 15, 11, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 101);

	//
	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_EXACTLIST_TEXT, 88, 260, 92, 11, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 76, 461);

	pkButton = new SPWindowButton(WIID_CASHSHOP_EXACTLIST_OK, 64, 288, 66, 17, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 280);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 298);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 316);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 334);

	pkButton = new SPWindowButton(WIID_CASHSHOP_EXACTLIST_CANCEL, 141, 288, 66, 17, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 389, 280);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 389, 298);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 389, 316);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 389, 334);

}

//------------------------------------------------------------------------------------
void SPWindowCashShop::RefreshBuyListDlg()
{
	SPWindow * pkParentWindow = NULL;
	SPWindowButton * pkWindowButton = NULL;
	SPWindowStatic * pkChild[2], * pkWindowStatic = NULL;

	for (int i=0; i<3; i++ )
	{
		pkParentWindow = Find(WIID_CASHSHOP_BUYLIST_ITEMBG + i, true);
		if ( !pkParentWindow )
			return;
		pkParentWindow->CleanChildAll();

		SPCashItemAttr * pkCashItem = GetBuyListItemByIdx(m_iCurStartItem + i);
		if ( !pkCashItem )
			continue;
		
		pkWindowButton = new SPWindowButton(WIID_CASHSHOP_BUYLIST_ITEM_CHECK +i, 0, 18, 11, 11, pkParentWindow);
		pkWindowButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 324, 216);
		pkWindowButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 324, 216);
		pkWindowButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 324, 228);
		pkWindowButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 409, 240);
		pkWindowButton->SPSendMessage(SPIM_BTN_SELECT, GetBuyCheck(m_iCurStartItem + i));

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_BUYLIST_ITEM_SLOT +i, 15, 30, 35, 12, pkParentWindow);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 475, 46);

		pkChild[1] = new SPWindowStatic(WIID_CASHSHOP_BUYLIST_ITEM +i, 2, -24, 32, 32, pkChild[0]);
		POINT  pt;
		SPTexture* pkTexture = g_pItemCluster->GetItemTextureFromTextureInx(
			pkCashItem->iIcon, pkCashItem->iIconIndex, pt);
		if( pkTexture )
		{
			pkChild[1]->SetWindowType(WND_TYPE_RECT);
			pkChild[1]->SetImageRect(pkTexture, pt.x, pt.y, pt.x+32, pt.y+32);
			pkTexture->IncreaseRef();
		}
		
		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_BUYLIST_NAME +i, 59, 8, 138, 12, pkParentWindow);
		pkChild[0]->SetFont(FONT_12_BOLD);
		pkChild[0]->SetFontColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
		pkChild[0]->SetFormat(DT_VCENTER | DT_LEFT);
		pkChild[0]->SetWindowText(pkCashItem->strName.c_str());
		
		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_BUYLIST_PRICE_BG +i, 60, 23, 80, 14, pkParentWindow);
		pkChild[0]->SetWindowType(WND_TYPE_LEFTBASE);
		pkChild[0]->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 493, 497, 495, 511);
		pkChild[0]->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 497, 497, 499, 511);
		pkChild[0]->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 501, 497, 503, 511);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_BUYLIST_OZ_ICON +i, 122, 25, 15, 11, pkParentWindow);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 101);

		int iPrice = pkCashItem->iSalePrice ? pkCashItem->iSalePrice : pkCashItem->iPrice;
		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_BUYLIST_PRICE +i, 62, 25, 53, 12, pkParentWindow);
		pkChild[0]->SetFontColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		pkChild[0]->SetFormat(DT_VCENTER | DT_RIGHT);
		pkChild[0]->SetWindowText(ConvertMoneyToString(iPrice));

		pkParentWindow->Show();

	}

	int iPriceSum = 0;
	int nItemSize = static_cast<int>(m_vEquipCashItem.size());
	int nCheckSize = static_cast<int>(m_vecBuyCheck.size());
	if ( nItemSize != nCheckSize )
		return;

	for ( int i=0; i<nItemSize; i++ )
	{
		if ( m_vecBuyCheck[i] )
		{
			int iPrice = m_vEquipCashItem[i]->iSalePrice ? m_vEquipCashItem[i]->iSalePrice : m_vEquipCashItem[i]->iPrice;
			iPriceSum += iPrice;
		}
	}

	pkChild[0] = static_cast<SPWindowStatic *>(Find(WIID_CASHSHOP_BUYLIST_MY_OZ, true));
	if ( pkChild[0] )
		pkChild[0]->SetWindowText(ConvertMoneyToString(m_uiCurOZ));

	pkChild[0] = static_cast<SPWindowStatic *>(Find(WIID_CASHSHOP_BUYLIST_USE_OZ, true));
	if ( pkChild[0] )
		pkChild[0]->SetWindowText(ConvertMoneyToString(iPriceSum));

	pkChild[0] = static_cast<SPWindowStatic *>(Find(WIID_CASHSHOP_BUYLIST_AF_MY_OZ, true));
	if ( pkChild[0] )
	{
		int iMyPrice = m_uiCurOZ - iPriceSum;
		if ( iMyPrice < 0 )
		{
			char  szPrice[24];
			sprintf(szPrice, "%d", iMyPrice);
			pkChild[0]->SetWindowText(szPrice);
		}
		else
			pkChild[0]->SetWindowText(ConvertMoneyToString(iMyPrice));

	}

}

//------------------------------------------------------------------------------------
void SPWindowCashShop::RefreshWearListDlg()
{
	SPWindow * pkParentWindow = NULL;
	SPWindowButton * pkWindowButton = NULL;
	SPWindowStatic * pkChild[2], * pkWindowStatic = NULL;

	for (int i=0; i<5; i++ )
	{
		pkParentWindow = Find(WIID_CASHSHOP_WEARLIST_ITEMBG + i, true);
		if ( !pkParentWindow )
			return;
		pkParentWindow->CleanChildAll();

		SPCashItemAttr * pkCashItem = GetBuyListItemByIdx(m_iCurStartItem + i);
		if ( !pkCashItem )
			continue;

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_WEARLIST_ITEM_SLOT +i, 15, 30, 35, 12, pkParentWindow);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 475, 46);

		pkChild[1] = new SPWindowStatic(WIID_CASHSHOP_WEARLIST_ITEM +i, 2, -24, 32, 32, pkChild[0]);
		POINT  pt;
		SPTexture* pkTexture = g_pItemCluster->GetItemTextureFromTextureInx(
			pkCashItem->iIcon, pkCashItem->iIconIndex, pt);
		if( pkTexture )
		{
			pkChild[1]->SetWindowType(WND_TYPE_RECT);
			pkChild[1]->SetImageRect(pkTexture, pt.x, pt.y, pt.x+32, pt.y+32);
			pkTexture->IncreaseRef();
		}
		
		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_WEARLIST_NAME +i, 59, 8, 138, 12, pkParentWindow);
		pkChild[0]->SetFont(FONT_12_BOLD);
		pkChild[0]->SetFontColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
		pkChild[0]->SetFormat(DT_VCENTER | DT_LEFT);
		pkChild[0]->SetWindowText(pkCashItem->strName.c_str());
		
		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_WEARLIST_PRICE_BG +i, 60, 23, 80, 14, pkParentWindow);
		pkChild[0]->SetWindowType(WND_TYPE_LEFTBASE);
		pkChild[0]->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 493, 497, 495, 511);
		pkChild[0]->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 497, 497, 499, 511);
		pkChild[0]->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 501, 497, 503, 511);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_WEARLIST_OZ_ICON +i, 122, 25, 15, 11, pkParentWindow);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 101);

		int iPrice = pkCashItem->iSalePrice ? pkCashItem->iSalePrice : pkCashItem->iPrice;
		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_WEARLIST_PRICE +i, 62, 25, 53, 12, pkParentWindow);
		pkChild[0]->SetFontColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		pkChild[0]->SetFormat(DT_VCENTER | DT_RIGHT);
		pkChild[0]->SetWindowText(ConvertMoneyToString(iPrice));

		pkParentWindow->Show();
	}

	pkChild[0] = static_cast<SPWindowStatic *>(Find(WIID_CASHSHOP_WEARLIST_AF_COUNT, true));
	if ( pkChild[0] )
	{
		char   szCount[12];
		sprintf(szCount, "%d件", GetBuyListItemSize());
		pkChild[0]->SetWindowText(szCount);
	}

	int iPriceSum = 0;
	int nItemSize = static_cast<int>(m_vEquipCashItem.size());

	for ( int i=0; i<nItemSize; i++ )
	{
		int iPrice = m_vEquipCashItem[i]->iSalePrice ? m_vEquipCashItem[i]->iSalePrice : m_vEquipCashItem[i]->iPrice;
		iPriceSum += iPrice;
	}

	pkChild[0] = static_cast<SPWindowStatic *>(Find(WIID_CASHSHOP_WEARLIST_AF_PRICE, true));
	if ( pkChild[0] )
		pkChild[0]->SetWindowText(ConvertMoneyToString(iPriceSum));

}

//------------------------------------------------------------------------------------
void SPWindowCashShop::RefreshExactListDlg()
{
	SPWindow * pkParentWindow = NULL;
	SPWindowButton * pkWindowButton = NULL;
	SPWindowStatic * pkChild[2], * pkWindowStatic = NULL;

	for (int i=0; i<3; i++ )
	{
		pkParentWindow = Find(WIID_CASHSHOP_EXACTLIST_ITEMBG + i, true);
		if ( !pkParentWindow )
			return;
		pkParentWindow->CleanChildAll();

		SPCashItemAttr * pkCashItem = GetBuyListItemByIdx(m_iCurStartItem + i);
		if ( !pkCashItem )
			continue;

		pkWindowButton = new SPWindowButton(WIID_CASHSHOP_EXACTLIST_ITEM_CHECK +i, 0, 18, 11, 11, pkParentWindow);
		pkWindowButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 324, 216);
		pkWindowButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 324, 216);
		pkWindowButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 324, 228);
		pkWindowButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 409, 240);
		pkWindowButton->SPSendMessage(SPIM_BTN_SELECT, GetBuyCheck(m_iCurStartItem + i));

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_EXACTLIST_ITEM_SLOT +i, 15, 30, 35, 12, pkParentWindow);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 475, 46);

		pkChild[1] = new SPWindowStatic(WIID_CASHSHOP_EXACTLIST_ITEM +i, 2, -24, 32, 32, pkChild[0]);
		POINT  pt;
		SPTexture* pkTexture = g_pItemCluster->GetItemTextureFromTextureInx(
			pkCashItem->iIcon, pkCashItem->iIconIndex, pt);
		if( pkTexture )
		{
			pkChild[1]->SetWindowType(WND_TYPE_RECT);
			pkChild[1]->SetImageRect(pkTexture, pt.x, pt.y, pt.x+32, pt.y+32);
			pkTexture->IncreaseRef();
		}
		
		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_EXACTLIST_NAME +i, 59, 8, 138, 12, pkParentWindow);
		pkChild[0]->SetFont(FONT_12_BOLD);
		pkChild[0]->SetFontColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
		pkChild[0]->SetFormat(DT_VCENTER | DT_LEFT);
		pkChild[0]->SetWindowText(pkCashItem->strName.c_str());
		
		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_EXACTLIST_PRICE_BG +i, 60, 23, 80, 14, pkParentWindow);
		pkChild[0]->SetWindowType(WND_TYPE_LEFTBASE);
		pkChild[0]->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 493, 497, 495, 511);
		pkChild[0]->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 497, 497, 499, 511);
		pkChild[0]->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 501, 497, 503, 511);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_EXACTLIST_OZ_ICON +i, 122, 25, 15, 11, pkParentWindow);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 101);

		int iPrice = pkCashItem->iSalePrice ? pkCashItem->iSalePrice : pkCashItem->iPrice;
		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_EXACTLIST_PRICE +i, 62, 25, 53, 12, pkParentWindow);
		pkChild[0]->SetFontColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		pkChild[0]->SetFormat(DT_VCENTER | DT_RIGHT);
		pkChild[0]->SetWindowText(ConvertMoneyToString(iPrice));

		pkParentWindow->Show();
	}

	int iPriceSum = 0;
	int nItemSize = static_cast<int>(m_vEquipCashItem.size());
	int nCheckSize = static_cast<int>(m_vecBuyCheck.size());
	if ( nItemSize != nCheckSize )
		return;

	for ( int i=0; i<nItemSize; i++ )
	{
		if ( m_vecBuyCheck[i] )
		{
			int iPrice = m_vEquipCashItem[i]->iSalePrice ? m_vEquipCashItem[i]->iSalePrice : m_vEquipCashItem[i]->iPrice;
			iPriceSum += iPrice;
		}
	}

	pkChild[0] = static_cast<SPWindowStatic *>(Find(WIID_CASHSHOP_EXACTLIST_USE_OZ, true));
	if ( pkChild[0] )
		pkChild[0]->SetWindowText(ConvertMoneyToString(iPriceSum));

}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnBuyListOK(WPARAM wParam, LPARAM lParam)
{
	SPWindow * pkParentWindow = NULL;
	
	pkParentWindow = Find(WIID_CASHSHOP_BUYLIST);
	if ( !pkParentWindow )
		return 1;

	int nItemSize = static_cast<int>(m_vEquipCashItem.size());
	int nCheckSize = static_cast<int>(m_vecBuyCheck.size());
	if ( nItemSize != nCheckSize )
		return 1;

	m_vCashierItemAttr.clear();
	for ( int i=0; i<nItemSize; i++ )
	{
		if ( m_vecBuyCheck[i] )
			m_vCashierItemAttr.push_back(m_vEquipCashItem[i]);
	}
	
	pkParentWindow->Hide();
	g_pInterfaceManager->SetModalWindow(NULL);

	m_iCashierOZ = 0;
	m_iCashierMileage = 0;
	Buy();

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnBuyListCancel(WPARAM wParam, LPARAM lParam)
{
	SPWindow * pkParentWindow = NULL;
	pkParentWindow = Find(WIID_CASHSHOP_BUYLIST);
	if ( !pkParentWindow )
		return 1;

	pkParentWindow->Hide();
	g_pInterfaceManager->SetModalWindow(NULL);
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnWearListOK(WPARAM wParam, LPARAM lParam)
{
	SPWindow * pkParentWindow = NULL;
	pkParentWindow = Find(WIID_CASHSHOP_WEARLIST);
	if ( !pkParentWindow )
		return 1;

	pkParentWindow->Hide();
	g_pInterfaceManager->SetModalWindow(NULL);
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnExactListOK(WPARAM wParam, LPARAM lParam)
{
	SPWindow * pkParentWindow = NULL;
	pkParentWindow = Find(WIID_CASHSHOP_EXACTLIST);
	if ( !pkParentWindow )
		return 1;

	int iBuyCount = GetBuyCheckCount();
	int nItemSize = static_cast<int>(m_vEquipCashItem.size());
	int nCheckSize = static_cast<int>(m_vecBuyCheck.size());
	if ( nItemSize != nCheckSize || nItemSize <= 0 || iBuyCount <= 0 )
		return 1;

	const char* pszString = NULL;
	SPWindow* pkChildWindow;
	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_EXACTLIST_RECEIVE, true);
	if( pkChildWindow )
		pszString = pkChildWindow->GetWindowText();

	bool bSelfExactCheck = false, bSendPacket = false;
	if( pszString )
	{
		char szTargetName[LEN_NAME], szComment[MAX_MAIl_BODY_LEN];
		memset(szTargetName, 0, sizeof(szTargetName));
		memset(szComment, 0, sizeof(szComment));
		strncpy(szTargetName, pszString, LEN_NAME);
		
		pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_EXACTLIST_MESSAGE, true);
		if( pkChildWindow )
			pszString = pkChildWindow->GetWindowText();

		if( pszString )
			strncpy(szComment, pszString, MAX_MAIl_BODY_LEN);

		char szBuf[64];
		if( g_pGOBManager->GetLocalPlayer() )
		{
			g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_GETGOBNAME, (LPARAM)szBuf);
			if( strcmp(szTargetName, szBuf) == 0 )      //如果是自己不处理
				bSelfExactCheck = true;
		}

		pkParentWindow->Hide();
		g_pInterfaceManager->SetModalWindow(NULL);

		if( !bSelfExactCheck )
		{
			CPacket kPacket(CASHSHOP_CS_ASK_BUY);		
			kPacket.AddUINT8(iBuyCount);
			for ( int i=0; i<nItemSize; i++ )
			{
				if ( m_vecBuyCheck[i] )
					kPacket.AddUINT32(m_vEquipCashItem[i]->iCashID);
			}
			kPacket.AddUINT8(sizeof(szComment));
			kPacket.Add(szComment, sizeof(szComment));
			kPacket.AddUINT8(sizeof(szTargetName));
			kPacket.Add(szTargetName, sizeof(szTargetName));
							
			(g_pNetworkManager->GetPacketHandler())->PostMsg(&kPacket);
			//g_pCheckManager->SetDBBlock(true);
			//m_bBuyRecv = false;
			//bSendPacket = true;

			//CPacket pkPacket;
			//pkPacket.AddUINT8(iBuyCount);
			//for ( int i=0; i<nItemSize; i++ )
			//{
			//	if ( m_vecBuyCheck[i] )
			//		pkPacket.AddUINT32(m_vEquipCashItem[i]->iCashID);
			//}
			//pkPacket.AddUINT8(sizeof(szTargetName));
			//pkPacket.Add(szTargetName, sizeof(szTargetName));
			//pkPacket.AddUINT8(sizeof(szComment));
			//pkPacket.Add(szComment, sizeof(szComment));
			//g_pInterfaceManager->SPChildWndSendMessage(WIID_BEEXACT, SPIM_BEEXACT_MESSAGE, (WPARAM)&pkPacket, 0);
		}
	}

	if( bSelfExactCheck )
	{
		SetErrorDlg(9004004);
	}
	else if( bSendPacket )
	{
		//ShowBuyWaitDlg();
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnExactListCancel(WPARAM wParam, LPARAM lParam)
{
	SPWindow * pkParentWindow = NULL;
	pkParentWindow = Find(WIID_CASHSHOP_EXACTLIST);
	if ( !pkParentWindow )
		return 1;

	pkParentWindow->Hide();
	g_pInterfaceManager->SetModalWindow(NULL);
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnBuyListUp(WPARAM wParam, LPARAM lParam)
{
	m_iCurStartItem--;
	if ( m_iCurStartItem < 0 )
		m_iCurStartItem = 0;

	RefreshBuyListDlg();
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnBuyListDown(WPARAM wParam, LPARAM lParam)
{
	m_iCurStartItem++;
	if ( m_iCurStartItem > static_cast<int>(m_vEquipCashItem.size())-3 )
		m_iCurStartItem = static_cast<int>(m_vEquipCashItem.size())-3;

	if ( m_iCurStartItem < 0 )
		m_iCurStartItem = 0;

	RefreshBuyListDlg();
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnWearListUp(WPARAM wParam, LPARAM lParam)
{
	m_iCurStartItem--;
	if ( m_iCurStartItem < 0 )
		m_iCurStartItem = 0;

	RefreshWearListDlg();
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnWearListDown(WPARAM wParam, LPARAM lParam)
{
	m_iCurStartItem++;
	if ( m_iCurStartItem > static_cast<int>(m_vEquipCashItem.size())-5 )
		m_iCurStartItem = static_cast<int>(m_vEquipCashItem.size())-5;

	if ( m_iCurStartItem < 0 )
		m_iCurStartItem = 0;

	RefreshWearListDlg();
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnExactListUp(WPARAM wParam, LPARAM lParam)
{
	m_iCurStartItem--;
	if ( m_iCurStartItem < 0 )
		m_iCurStartItem = 0;

	RefreshExactListDlg();
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnExactListDown(WPARAM wParam, LPARAM lParam)
{
	m_iCurStartItem++;
	if ( m_iCurStartItem > static_cast<int>(m_vEquipCashItem.size())-3 )
		m_iCurStartItem = static_cast<int>(m_vEquipCashItem.size())-3;

	if ( m_iCurStartItem < 0 )
		m_iCurStartItem = 0;

	RefreshExactListDlg();
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnBuyListCheck(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	int iIndex = iID - WIID_CASHSHOP_BUYLIST_ITEM_CHECK + m_iCurStartItem;
	bool  bCheck = !GetBuyCheck(iIndex);
	SetBuyCheck(iIndex, bCheck);

	SPWindowButton * pkButton = static_cast<SPWindowButton *>(Find(iID, true));
	if ( pkButton )
		pkButton->SPSendMessage(SPIM_BTN_SELECT, bCheck);

	RefreshBuyListDlg();
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnExactListCheck(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	int iIndex = iID - WIID_CASHSHOP_EXACTLIST_ITEM_CHECK + m_iCurStartItem;
	bool  bCheck = !GetBuyCheck(iIndex);
	SetBuyCheck(iIndex, bCheck);

	SPWindowButton * pkButton = static_cast<SPWindowButton *>(Find(iID, true));
	if ( pkButton )
		pkButton->SPSendMessage(SPIM_BTN_SELECT, bCheck);

	RefreshExactListDlg();
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnChangePlayerViewBack(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	SPPlayer * pkLocalPlayer = g_pGOBManager->GetLocalPlayer();
	if ( !m_pkPlayer || !pkLocalPlayer)
		return 0;

	int iIndex = iID - WIID_CASHSHOP_PLAYERVIEW_BACK1;
	for ( int i = WIID_CASHSHOP_PLAYERVIEW_BACK1; i <= WIID_CASHSHOP_PLAYERVIEW_BACK3; i++ )
	{
		SPWindowButton * pkButton = static_cast<SPWindowButton *>(Find(i, true));
		if ( pkButton )
		{
			if ( i == iID )
				pkButton->SPSendMessage(SPIM_BTN_SELECT, true);
			else
				pkButton->SPSendMessage(SPIM_BTN_SELECT, false);
		}
	}

	int iStageID = 0, iMapGroupID = 0;
	if ( iIndex == 0 )
	{
		g_pTerrainManager = m_gpMgrTerrain;   //战场用作场景1
		m_pkPlayer->SetPos(pkLocalPlayer->GetPosX(), pkLocalPlayer->GetPosY());
		iStageID = pkLocalPlayer->GetCurStageID();
		iMapGroupID = pkLocalPlayer->GetCurMapGroupID();
	}
	else if ( iIndex == 1 )
	{
		g_pTerrainManager = m_pMgrTerrain[1];
		m_pkPlayer->SetPos(10.0f, 10.0f);
		iStageID = 2;
		iMapGroupID = 2;
		return 0;
	}
	else if ( iIndex == 2 )
	{
		g_pTerrainManager = m_pMgrTerrain[2];
		m_pkPlayer->SetPos(10.0f, 10.0f);
		iStageID = 3;
		iMapGroupID = 3;
		return 0;
	}

	m_pkPlayer->SetStage(iStageID, iMapGroupID);
	g_StageManager.SetCurMapIndex(0, iMapGroupID, iStageID);
	g_pTerrainManager->GetTerrainCluster()->SetShareLayer();
	return 1;

}

//------------------------------------------------------------------------------------
void SPWindowCashShop::SetBuyCheck(int iIndex, bool bCheck)
{
	if ( iIndex >= 0 && iIndex < static_cast<int>(m_vecBuyCheck.size()) )
		m_vecBuyCheck[iIndex] = bCheck;
}

//------------------------------------------------------------------------------------
bool SPWindowCashShop::GetBuyCheck(int iIndex)
{
	if ( iIndex >= 0 && iIndex < static_cast<int>(m_vecBuyCheck.size()) )
		return m_vecBuyCheck[iIndex];
	return false;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::GetBuyCheckCount()
{
	int iCount = 0;
	for ( int i=0; i < static_cast<int>(m_vecBuyCheck.size()); i++ )
	{
		if ( m_vecBuyCheck[i] )
			iCount++;
	}

	return iCount;
}

//------------------------------------------------------------------------------------
SPCashItemAttr * SPWindowCashShop::GetBuyListItemByIdx(int iIndex)
{
	if ( iIndex >= 0 && iIndex < static_cast<int>(m_vEquipCashItem.size()) )
	{
		return m_vEquipCashItem[iIndex];
	}
	return NULL;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::GetBuyListItemSize()
{
	return static_cast<int>(m_vEquipCashItem.size());
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::InitGiftDlg()
{
	SPWindowStatic* pkParent, *pkChild[2];
	SPWindowButton* pkButton;
	SPWindowEdit* pkEdit;
	SPWindowEditMultiLine* pkMultiEdit;
	SPWindowTreeBox* pkTreeBox;

	pkParent = new SPWindowStatic(WIID_CASHSHOP_GIFT, 266, 132, 453, m_iMileageRatio ? 374 : 336, this);

	pkParent = new SPWindowStatic(WIID_CASHSHOP_GIFT_SKIN, 0, 0, 268, m_iMileageRatio ? 374 : 336, pkParent);
	pkParent->SetWindowType(WND_TYPE_TOPBASE);
	pkParent->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 28, 269, 36);
	pkParent->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 38, 269, 44);
	pkParent->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 46, 269, 54);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_GIFT_ITEM_SLOT, 22, 42, 35, 11, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 443, 137);

	pkChild[1] = new SPWindowStatic(WIID_CASHSHOP_GIFT_ITEM, 2, -24, 32, 32, pkChild[0]);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_GIFT_NAME, 69, 20, 175, 12, pkParent);
	pkChild[0]->SetFont(FONT_12_BOLD);
	pkChild[0]->SetFontColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
	pkChild[0]->SetFormat(DT_VCENTER | DT_LEFT);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_GIFT_PRICE_BG, 70, 35, 80, 14, pkParent);
	pkChild[0]->SetWindowType(WND_TYPE_LEFTBASE);
	pkChild[0]->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 493, 497, 495, 511);
	pkChild[0]->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 497, 497, 499, 511);
	pkChild[0]->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 501, 497, 503, 511);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_GIFT_OZ_ICON, 131, 37, 15, 11, pkParent);
	if( m_iMileageRatio )
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 113);
	else
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 101);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_GIFT_PRICE, 74, 37, 53, 12, pkParent);
	pkChild[0]->SetFontColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	pkChild[0]->SetFormat(DT_VCENTER | DT_RIGHT);
	
	if( m_iMileageRatio )
	{
		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_GIFT_MY_OZ_TEXT, 22, 158, 96, 13, pkParent);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 385, 185);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_GIFT_USE_OZ_TEXT, 22, 177, 96, 13, pkParent);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 353, 106);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_GIFT_USE_MILE_TEXT, 22, 196, 96, 13, pkParent);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 353, 134);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_GIFT_USE_PRI_TEXT, 22, 231, 96, 13, pkParent);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 353, 120);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_GIFT_AF_MY_OZ_TEXT, 22, 250, 96, 13, pkParent);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 241, 242);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_GIFT_MY_OZ_ICON, 201, 159, 15, 11, pkParent);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 113);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_GIFT_USE_OZ_ICON, 201, 178, 15, 11, pkParent);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 113);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_GIFT_USE_MILE_BG, 146, 194, 74, 16, pkParent);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 307, 1);

		pkEdit = new SPWindowEdit(WIID_CASHSHOP_GIFT_USE_MILE, -15, 3, 66, 12, pkChild[0]);
		pkEdit->SetFontColor(D3DXCOLOR(0.2745f, 0.5490f, 0.4862f, 1.0f));
		pkEdit->SetFormat(DT_VCENTER | DT_RIGHT);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_GIFT_USE_MILE_ICON, 201, 197, 15, 11, pkParent);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 125);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_GIFT_USE_PRICE_ICON, 201, 232, 15, 11, pkParent);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 113);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_GIFT_AF_MY_OZ_ICON, 201, 251, 15, 11, pkParent);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 113);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_GIFT_MY_OZ, 131, 159, 66, 12, pkParent);
		pkChild[0]->SetFontColor(D3DXCOLOR(0.2745f, 0.5490f, 0.4862f, 1.0f));
		pkChild[0]->SetFormat(DT_VCENTER | DT_RIGHT);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_GIFT_USE_OZ, 131, 178, 66, 12, pkParent);
		pkChild[0]->SetFontColor(D3DXCOLOR(0.2745f, 0.5490f, 0.4862f, 1.0f));
		pkChild[0]->SetFormat(DT_VCENTER | DT_RIGHT);

		//pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_GIFT_USE_MILE, 131, 197, 66, 12, pkParent);
		//pkChild[0]->SetFontColor(D3DXCOLOR(0.2745f, 0.5490f, 0.4862f, 1.0f));
		//pkChild[0]->SetFormat(DT_VCENTER | DT_RIGHT);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_GIFT_USE_PRI, 131, 232, 66, 12, pkParent);
		pkChild[0]->SetFont(FONT_12_BOLD);
		pkChild[0]->SetFontColor(D3DXCOLOR(0.2745f, 0.5490f, 0.4862f, 1.0f));
		pkChild[0]->SetFormat(DT_VCENTER | DT_RIGHT);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_GIFT_AF_MY_OZ, 131, 251, 66, 12, pkParent);
		pkChild[0]->SetFontColor(D3DXCOLOR(0.2745f, 0.5490f, 0.4862f, 1.0f));
		pkChild[0]->SetFormat(DT_VCENTER | DT_RIGHT);

		pkButton = new SPWindowButton(WIID_CASHSHOP_GIFT_PLUS, 225, 192, 13, 9, pkParent);
		pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 481, 407);
		pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 481, 417);
		pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 481, 427);
		pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 481, 437);

		pkButton = new SPWindowButton(WIID_CASHSHOP_GIFT_MINUS, 225, 204, 13, 9, pkParent);
		pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 495, 407);
		pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 495, 417);
		pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 495, 427);
		pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 495, 437);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_GIFT_TYPE, 13, 278, 239, 45, pkParent);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 261, 162);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 210);

		pkButton = new SPWindowButton(WIID_CASHSHOP_GIFT_OK, 64, 329, 66, 17, pkParent);
		pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 280);
		pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 298);
		pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 316);
		pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 334);

		pkButton = new SPWindowButton(WIID_CASHSHOP_GIFT_CANCEL, 141, 329, 66, 17, pkParent);
		pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 389, 280);
		pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 389, 298);
		pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 389, 316);
		pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 389, 334);
	}
	else
	{

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_GIFT_MY_OZ_TEXT, 22, 158, 96, 13, pkParent);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 241, 200);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_GIFT_USE_OZ_TEXT, 22, 177, 96, 13, pkParent);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 353, 120);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_GIFT_AF_MY_OZ_TEXT, 22, 212, 96, 13, pkParent);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 241, 242);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_GIFT_MY_OZ_ICON, 201, 159, 15, 11, pkParent);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 101);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_GIFT_USE_OZ_ICON, 201, 178, 15, 11, pkParent);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 101);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_GIFT_AF_MY_OZ_ICON, 201, 213, 15, 11, pkParent);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 101);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_GIFT_MY_OZ, 131, 159, 66, 12, pkParent);
		pkChild[0]->SetFontColor(D3DXCOLOR(0.2745f, 0.5490f, 0.4862f, 1.0f));
		pkChild[0]->SetFormat(DT_VCENTER | DT_RIGHT);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_GIFT_USE_OZ, 131, 178, 66, 12, pkParent);
		pkChild[0]->SetFontColor(D3DXCOLOR(0.2745f, 0.5490f, 0.4862f, 1.0f));
		pkChild[0]->SetFormat(DT_VCENTER | DT_RIGHT);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_GIFT_AF_MY_OZ, 131, 213, 66, 12, pkParent);
		pkChild[0]->SetFontColor(D3DXCOLOR(0.2745f, 0.5490f, 0.4862f, 1.0f));
		pkChild[0]->SetFormat(DT_VCENTER | DT_RIGHT);

		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_GIFT_TYPE, 13, 237, 239, 45, pkParent);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 58);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 210);

		pkButton = new SPWindowButton(WIID_CASHSHOP_GIFT_OK, 64, 291, 66, 17, pkParent);
		pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 280);
		pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 298);
		pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 316);
		pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 334);

		pkButton = new SPWindowButton(WIID_CASHSHOP_GIFT_CANCEL, 141, 291, 66, 17, pkParent);
		pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 389, 280);
		pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 389, 298);
		pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 389, 316);
		pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 389, 334);
	}

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_GIFT_MESSAGE_GRN_BG, 2, 66, 261, 85, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 451, 121);
	pkChild[0]->SetSrcSize(2, 2);

	pkChild[1] = new SPWindowStatic(WIID_CASHSHOP_GIFT_RECEIVE_BG, 89, 8, 129, 16, pkChild[0]);
	pkChild[1]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 382, 1);

	pkEdit = new SPWindowEdit(WIID_CASHSHOP_GIFT_RECEIVE, 5, 1, 119, 12, pkChild[1]);
	pkEdit->SetLimitText(LEN_NAME);

	pkChild[1] = new SPWindowStatic(WIID_CASHSHOP_GIFT_MESSAGE_BG, 89, 29, 129, 46, pkChild[0]);
	pkChild[1]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 382, 18);

	pkMultiEdit = new SPWindowEditMultiLine(WIID_CASHSHOP_GIFT_MESSAGE, 5, 2, 119, 43, pkChild[1]);
	pkMultiEdit->SetLimitText(50);

	pkButton = new SPWindowButton(WIID_CASHSHOP_GIFT_FRIEND_BTN, 222, 8, 35, 16, pkChild[0]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 310, 18);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 346, 18);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 310, 35);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 346, 35);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_GIFT_RECEIVE_IMG, 22, 76, 62, 13, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 450, 120);
	
	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_GIFT_MESSAGE_IMG, 22, 97, 62, 13, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 450, 134);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_GIFT_FRIEND, 263, 64, 185, 149, pkParent);
	pkChild[0]->SetWindowType(WND_TYPE_TOPBASE);
	pkChild[0]->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 106, 55, 291, 63);
	pkChild[0]->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 106, 66, 291, 70);
	pkChild[0]->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 106, 73, 291, 81);

	pkChild[1] = new SPWindowStatic(WIID_CASHSHOP_GIFT_FRIEND_LIST_BG, 8, 11, 164, 124, pkChild[0]);
	pkChild[1]->SetWindowType(WND_TYPE_TOPBASE);
	pkChild[1]->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 106, 82, 270, 86);
	pkChild[1]->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 106, 85, 270, 88);
	pkChild[1]->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 106, 87, 270, 91);

	
	pkTreeBox =	new SPWindowTreeBox(WIID_CASHSHOP_GIFT_FRIEND_LIST, 0, 0, 164, 124, pkChild[1]);
	pkTreeBox->SetMargin(13, 9, 0, 0);
	pkTreeBox->UpdateBGPos();
	pkTreeBox->SetDoubleClick(true);
	pkTreeBox->SetScrollAutoHide(true);
	pkTreeBox->SetInterval(2);

	pkTreeBox->SetScrollUpSize(12, 11);
	pkTreeBox->SetScrollUpTexture( DATA_TEXTURE_ENABLE		, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 456 , 389 );
	pkTreeBox->SetScrollUpTexture( DATA_TEXTURE_HIT			, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 469 , 389 );
	pkTreeBox->SetScrollUpTexture( DATA_TEXTURE_PUSH		, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 482 , 389 );
	pkTreeBox->SetScrollUpTexture( DATA_TEXTURE_DISABLE		, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 495 , 389 );

	pkTreeBox->SetScrollDownSize( 12 , 11 );
	pkTreeBox->SetScrollDownTexture( DATA_TEXTURE_ENABLE	, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 456 , 431 );
	pkTreeBox->SetScrollDownTexture( DATA_TEXTURE_HIT		, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 469 , 431 );
	pkTreeBox->SetScrollDownTexture( DATA_TEXTURE_PUSH		, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 482 , 431 );
	pkTreeBox->SetScrollDownTexture( DATA_TEXTURE_DISABLE	, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 495 , 431 );

	pkTreeBox->SetScrollSliderSize( 12 , 23 );
	pkTreeBox->SetScrollSliderTexture( DATA_TEXTURE_ENABLE	, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 456 , 401 );
	pkTreeBox->SetScrollSliderTexture( DATA_TEXTURE_HIT		, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 469 , 401 );
	pkTreeBox->SetScrollSliderTexture( DATA_TEXTURE_PUSH	, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 482 , 401 );
	pkTreeBox->SetScrollSliderTexture( DATA_TEXTURE_DISABLE	, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 459 , 401 );

	pkTreeBox->SetScrollSliderBGTexture( DATA_TEXTURE_ENABLE, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 456 , 427 , 468 , 429 );
	pkTreeBox->SetScrollSliderBGTexture( DATA_TEXTURE_HIT	, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 469 , 427 , 481 , 429 );
	pkTreeBox->SetScrollSliderBGTexture( DATA_TEXTURE_PUSH	, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 482 , 427 , 494 , 429 );
	pkTreeBox->SetScrollSliderBGTexture( DATA_TEXTURE_DISABLE, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 495 , 427 , 507 , 429 );

	pkTreeBox->SetScrollMargin( 0 , 0 , 0 , 0 );
	pkTreeBox->UpdateScrollPos();

	pkTreeBox->Refresh();

}

//------------------------------------------------------------------------------------
//[xialin 2008/07/10] 索要对话框
void SPWindowCashShop::InitExactDlg()
{
	SPWindowStatic* pkParent, *pkChild[2];
	SPWindowButton* pkButton;
	SPWindowEdit* pkEdit;
	SPWindowEditMultiLine* pkMultiEdit;
	SPWindowTreeBox* pkTreeBox;

	pkParent = new SPWindowStatic(WIID_CASHSHOP_EXACT, 280, 200, 467, 405, this);

	pkParent = new SPWindowStatic(WIID_CASHSHOP_EXACT_SKIN, 0, 0, 268, 224, pkParent);
	pkParent->SetWindowType(WND_TYPE_TOPBASE);
	pkParent->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 28, 269, 36);
	pkParent->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 38, 269, 44);
	pkParent->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 46, 269, 54);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_EXACT_ITEM_SLOT, 22, 42, 35, 11, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 443, 137);

	pkChild[1] = new SPWindowStatic(WIID_CASHSHOP_EXACT_ITEM, 2, -24, 32, 32, pkChild[0]);  //商品图标

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_EXACT_NAME, 69, 20, 175, 12, pkParent);   //商品名
	pkChild[0]->SetFont(FONT_12_BOLD);
	pkChild[0]->SetFontColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
	pkChild[0]->SetFormat(DT_VCENTER | DT_LEFT);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_EXACT_PRICE_BG, 70, 35, 80, 14, pkParent);
	pkChild[0]->SetWindowType(WND_TYPE_LEFTBASE);
	pkChild[0]->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 493, 497, 495, 511);
	pkChild[0]->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 497, 497, 499, 511);
	pkChild[0]->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 501, 497, 503, 511);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_EXACT_OZ_ICON, 131, 37, 15, 11, pkParent);  //券标
	if( m_iMileageRatio )
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 113);
	else
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 101);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_EXACT_PRICE, 74, 37, 53, 12, pkParent);  //价格
	pkChild[0]->SetFontColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	pkChild[0]->SetFormat(DT_VCENTER | DT_RIGHT);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_EXACT_TEXT, 88, 168, 92, 11, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 76, 461);

	pkButton = new SPWindowButton(WIID_CASHSHOP_EXACT_OK, 64, 190, 66, 17, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 280);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 298);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 316);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 334);

	pkButton = new SPWindowButton(WIID_CASHSHOP_EXACT_CANCEL, 141, 190, 66, 17, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 389, 280);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 389, 298);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 389, 316);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 389, 334);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_EXACT_MESSAGE_GRN_BG, 2, 66, 261, 85, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 451, 121);
	pkChild[0]->SetSrcSize(2, 2);

	pkChild[1] = new SPWindowStatic(WIID_CASHSHOP_EXACT_RECEIVE_BG, 89, 8, 129, 16, pkChild[0]);
	pkChild[1]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 382, 1);

	pkEdit = new SPWindowEdit(WIID_CASHSHOP_EXACT_RECEIVE, 5, 1, 119, 12, pkChild[1]);
	pkEdit->SetLimitText(LEN_NAME);

	pkChild[1] = new SPWindowStatic(WIID_CASHSHOP_EXACT_MESSAGE_BG, 89, 29, 129, 46, pkChild[0]);
	pkChild[1]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 382, 18);

	pkMultiEdit = new SPWindowEditMultiLine(WIID_CASHSHOP_EXACT_MESSAGE, 5, 2, 119, 43, pkChild[1]);
	pkMultiEdit->SetLimitText(50);

	pkButton = new SPWindowButton(WIID_CASHSHOP_EXACT_FRIEND_BTN, 222, 8, 35, 16, pkChild[0]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 310, 18);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 346, 18);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 310, 35);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 346, 35);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_EXACT_RECEIVE_IMG, 22, 76, 62, 13, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 449, 78);
	
	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_EXACT_MESSAGE_IMG, 22, 97, 62, 13, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 450, 134);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_EXACT_FRIEND, 263, 64, 185, 149, pkParent);
	pkChild[0]->SetWindowType(WND_TYPE_TOPBASE);
	pkChild[0]->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 106, 55, 291, 63);
	pkChild[0]->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 106, 66, 291, 70);
	pkChild[0]->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 106, 73, 291, 81);

	pkChild[1] = new SPWindowStatic(WIID_CASHSHOP_EXACT_FRIEND_LIST_BG, 8, 11, 164, 124, pkChild[0]);
	pkChild[1]->SetWindowType(WND_TYPE_TOPBASE);
	pkChild[1]->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 106, 82, 270, 86);
	pkChild[1]->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 106, 85, 270, 88);
	pkChild[1]->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 106, 87, 270, 91);

	
	pkTreeBox =	new SPWindowTreeBox(WIID_CASHSHOP_EXACT_FRIEND_LIST, 0, 0, 164, 124, pkChild[1]);
	pkTreeBox->SetMargin(13, 9, 0, 0);
	pkTreeBox->UpdateBGPos();
	pkTreeBox->SetDoubleClick(true);
	pkTreeBox->SetScrollAutoHide(true);
	pkTreeBox->SetInterval(2);

	pkTreeBox->SetScrollUpSize(12, 11);
	pkTreeBox->SetScrollUpTexture( DATA_TEXTURE_ENABLE		, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 456 , 389 );
	pkTreeBox->SetScrollUpTexture( DATA_TEXTURE_HIT			, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 469 , 389 );
	pkTreeBox->SetScrollUpTexture( DATA_TEXTURE_PUSH		, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 482 , 389 );
	pkTreeBox->SetScrollUpTexture( DATA_TEXTURE_DISABLE		, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 495 , 389 );

	pkTreeBox->SetScrollDownSize( 12 , 11 );
	pkTreeBox->SetScrollDownTexture( DATA_TEXTURE_ENABLE	, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 456 , 431 );
	pkTreeBox->SetScrollDownTexture( DATA_TEXTURE_HIT		, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 469 , 431 );
	pkTreeBox->SetScrollDownTexture( DATA_TEXTURE_PUSH		, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 482 , 431 );
	pkTreeBox->SetScrollDownTexture( DATA_TEXTURE_DISABLE	, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 495 , 431 );

	pkTreeBox->SetScrollSliderSize( 12 , 23 );
	pkTreeBox->SetScrollSliderTexture( DATA_TEXTURE_ENABLE	, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 456 , 401 );
	pkTreeBox->SetScrollSliderTexture( DATA_TEXTURE_HIT		, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 469 , 401 );
	pkTreeBox->SetScrollSliderTexture( DATA_TEXTURE_PUSH	, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 482 , 401 );
	pkTreeBox->SetScrollSliderTexture( DATA_TEXTURE_DISABLE	, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 459 , 401 );

	pkTreeBox->SetScrollSliderBGTexture( DATA_TEXTURE_ENABLE, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 456 , 427 , 468 , 429 );
	pkTreeBox->SetScrollSliderBGTexture( DATA_TEXTURE_HIT	, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 469 , 427 , 481 , 429 );
	pkTreeBox->SetScrollSliderBGTexture( DATA_TEXTURE_PUSH	, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 482 , 427 , 494 , 429 );
	pkTreeBox->SetScrollSliderBGTexture( DATA_TEXTURE_DISABLE, "DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG" , 495 , 427 , 507 , 429 );

	pkTreeBox->SetScrollMargin( 0 , 0 , 0 , 0 );
	pkTreeBox->UpdateScrollPos();

	pkTreeBox->Refresh();
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::InitErrorDlg()
{
	SPWindowStatic* pkParent, *pkChild[2];
	SPWindowButton* pkButton;

	pkParent = new SPWindowStatic(WIID_CASHSHOP_ERROR, 280, 248, 242, 106, this);
	pkParent->SetWindowType(WND_TYPE_LEFTBASE);
	pkParent->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 493, 298, 503, 404);
	pkParent->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 506, 405, 510, 511);
	pkParent->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 493, 405, 504, 511);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_ERROR_IMAGE, 17, 48, 46, 46, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 343, 209);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_ERROR_MESSAGE, 15, 13, 210, 56, pkParent);
	pkChild[0]->SetFontColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	pkChild[0]->SetMultiLine(true, 2);

	pkButton = new SPWindowButton(WIID_CASHSHOP_ERROR_OK, 159, 77, 68, 17, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 139, 138);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 139, 156);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 139, 174);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 139, 192);
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::InitCouponDlg()
{
	SPWindowStatic* pkParent, *pkChild[2];
	SPWindowButton* pkButton;
	SPWindowEdit* pkEdit;
	int i;

	pkParent = new SPWindowStatic(WIID_CASHSHOP_COUPON, 252, 245, 295, 110, this);
	pkParent->SetWindowType(WND_TYPE_TOPBASE);
	pkParent->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 1, 296, 9);
	pkParent->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 11, 296, 17);
	pkParent->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 19, 296, 27);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_COUPON_CODE_IMAGE1, 6, 6, 74, 5, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 273);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_COUPON_CODE_IMAGE2, 6, 13, 74, 8, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 353, 97);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_COUPON_TEXT, 25, 33, 239, 13, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 261, 148);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_COUPON_LOGO, 9, 46, 91, 52, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 261, 95);

	for( i = 0; i < 4; i++ )
	{
		pkChild[1] = new SPWindowStatic(WIID_CASHSHOP_COUPON_NUMBER_IMG + i,
			16 + i * 63, 7, 54, 16, pkChild[0]);
		pkChild[1]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 367, 65);

		pkEdit = new SPWindowEdit(WIID_CASHSHOP_COUPON_NUMBER + i,
			5, 3, 44, 12, pkChild[1]);
		pkEdit->SetFontColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		pkEdit->SetFormat(DT_VCENTER | DT_LEFT);
		pkEdit->SetEnableTabStop(true);
		pkEdit->SetLimitText(4);
	}

	for( i = 0; i < 3; i++ )
	{
		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_COUPON_LINE + i,
			81 + i * 63, 61, 5, 1, pkParent);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 457, 335);
	}

	pkButton = new SPWindowButton(WIID_CASHSHOP_COUPON_OK, 210, 77, 66, 17, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 280);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 298);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 316);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 334);

	pkButton = new SPWindowButton(WIID_CASHSHOP_COUPON_CANCEL, 272, 8, 10, 10, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 501, 164);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 501, 175);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 501, 186);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 501, 197);
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::InitSellDlg()
{
	SPWindowStatic* pkParent, *pkChild[2];
	SPWindowButton* pkButton;

	pkParent = new SPWindowStatic(WIID_CASHSHOP_SELL, 266, 187, 268, 207, this);
	pkParent->SetWindowType(WND_TYPE_TOPBASE);
	pkParent->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 1, 269, 9);
	pkParent->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 12, 269, 16);
	pkParent->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 19, 269, 27);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_SELL_ITEM_SLOT, 22, 42, 35, 11, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 443, 137);

	pkChild[1] = new SPWindowStatic(WIID_CASHSHOP_SELL_ITEM, 2, -24, 32, 32, pkChild[0]);
	
	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_SELL_ITEM_NAME, 69, 28, 175, 12, pkParent);
	pkChild[0]->SetFont(FONT_12_BOLD);
	pkChild[0]->SetFontColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
	pkChild[0]->SetFormat(DT_VCENTER | DT_LEFT);
	
	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_SELL_MILEAGE_TEXT, 22, 84, 96, 13, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 241, 214);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_SELL_MILEAGE_ICON, 201, 85, 15, 11, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 125);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_SELL_MILEAGE, 151, 85, 46, 12, pkParent);
	pkChild[0]->SetFontColor(D3DXCOLOR(0.2745f, 0.5490f, 0.4862f, 1.0f));
	pkChild[0]->SetFormat(DT_VCENTER | DT_RIGHT);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_SELL_COUNT_TEXT, 22, 69, 96, 13, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 385, 171);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_SELL_COUNT_ICON, 198, 68, 21, 15, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 431, 121);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_SELL_COUNT, 151, 68, 46, 12, pkParent);
	pkChild[0]->SetFontColor(D3DXCOLOR(0.2745f, 0.5490f, 0.4862f, 1.0f));
	pkChild[0]->SetFormat(DT_VCENTER | DT_RIGHT);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_SELL_INFO_TEXT, 13, 104, 239, 45, pkParent);
	pkChild[0]->SetFontColor(D3DXCOLOR(0.7647f, 0.2235f, 0.2235f, 1.0f));
	pkChild[0]->SetFormat(DT_VCENTER | DT_CENTER);
	pkChild[0]->SetMultiLine(true, 2);

	pkButton = new SPWindowButton(WIID_CASHSHOP_SELL_OK, 65, 168, 68, 17, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 138);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 156);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 174);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 192);

	pkButton = new SPWindowButton(WIID_CASHSHOP_SELL_CANCEL, 141, 168, 68, 17, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 70, 138);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 70, 156);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 70, 174);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 70, 192);
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::InitResult()
{
	SPWindowStatic* pkParent, *pkChild[2];
	SPWindowButton* pkButton;

	pkParent = new SPWindowStatic(WIID_CASHSHOP_RESULT, 266, 187, 268, 226, this);
	pkParent->SetWindowType(WND_TYPE_TOPBASE);
	pkParent->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 28, 269, 36);
	pkParent->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 38, 269, 44);
	pkParent->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 1, 46, 269, 54);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_RESULT_ITEM_SLOT, 22, 42, 35, 11, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 443, 137);

	pkChild[1] = new SPWindowStatic(WIID_CASHSHOP_RESULT_ITEM, 2, -24, 32, 32, pkChild[0]);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_RESULT_ITEM_NAME, 69, 20, 175, 12, pkParent);
	pkChild[0]->SetFont(FONT_12_BOLD);
	pkChild[0]->SetFontColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
	pkChild[0]->SetFormat(DT_VCENTER | DT_LEFT);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_RESULT_PRICE_BG, 70, 35, 80, 14, pkParent);
	pkChild[0]->SetWindowType(WND_TYPE_LEFTBASE);
	pkChild[0]->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 493, 497, 495, 511);
	pkChild[0]->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 497, 497, 499, 511);
	pkChild[0]->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 501, 497, 503, 511);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_RESULT_OZ_ICON, 131, 37, 15, 11, pkParent);
	if( m_iMileageRatio )
	{
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 113);
#ifdef __CASHSHOP_POINT_SHOP__
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 125);
#endif
	}
	else
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 101);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_RESULT_PRICE, 74, 37, 53, 12, pkParent);
	pkChild[0]->SetFontColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	pkChild[0]->SetFormat(DT_VCENTER | DT_RIGHT);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_RESULT_BUY_OZ_TEXT, 22, 76, 96, 13, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 241, 228);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_RESULT_MY_OZ_TEXT, 22, 95, 96, 13, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 241, 242);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_RESULT_BUY_OZ, 131, 77, 66, 12, pkParent);
	pkChild[0]->SetFontColor(D3DXCOLOR(0.2745f, 0.5490f, 0.4862f, 1.0f));
	pkChild[0]->SetFormat(DT_VCENTER | DT_RIGHT);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_RESULT_MY_OZ, 131, 96, 66, 12, pkParent);
	pkChild[0]->SetFontColor(D3DXCOLOR(0.2745f, 0.5490f, 0.4862f, 1.0f));
	pkChild[0]->SetFormat(DT_VCENTER | DT_RIGHT);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_RESULT_BUY_OZ_ICON, 201, 77, 15, 11, pkParent);
	if( m_iMileageRatio )
	{
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 113);
#ifdef __CASHSHOP_POINT_SHOP__
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 125);
#endif
	}
	else
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 101);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_RESULT_MY_OZ_ICON, 201, 96, 15, 11, pkParent);
	if( m_iMileageRatio )
	{
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 113);
#ifdef __CASHSHOP_POINT_SHOP__
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 125);
#endif
	}
	else
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP01.PNG", 463, 101);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_RESULT_MSG, 13, 129, 239, 45, pkParent);
	pkChild[0]->SetFontColor(D3DXCOLOR(0.7647f, 0.2235f, 0.2235f, 1.0f));
	pkChild[0]->SetFormat(DT_VCENTER | DT_CENTER);
	pkChild[0]->SetMultiLine(true, 2);

	pkButton = new SPWindowButton(WIID_CASHSHOP_RESULT_OK, 98, 187, 66, 17, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 280);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 298);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 316);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 322, 334);
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::InitBuyWait()
{
	SPWindowStatic* pkParent, *pkChild[2];

	pkParent = new SPWindowStatic(WIID_CASHSHOP_BUYWAIT, 237, 233, 325, 134, this);
	pkParent->SetWindowType(WND_TYPE_LEFTBASE);
	pkParent->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 482, 64, 492, 198);
	pkParent->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 495, 64, 498, 198);
	pkParent->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 501, 64, 511, 198);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_BUYWAIT_ITEM, 16, 32, 32, 32, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 292, 56);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_BUYWAIT_PLAYER, 272, 32, 30, 32, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 481, 352);

	pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_BUYWAIT_TEXT, 8, 83, 303, 29, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP03.PNG", 1, 28);

	int iY[14] = {40, 32, 28, 27, 28, 32, 37, 42, 47, 51, 52, 51, 47, 39};
	int i;

	for( i = 0; i < 14; i++ )
	{
		pkChild[0] = new SPWindowStatic(WIID_CASHSHOP_BUYWAIT_ANI + i,
			50 + i * 16, iY[i], 12, 12, pkParent);
		pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 0, 0);
	}
}

//------------------------------------------------------------------------------------
//[xialin 2008/07/10] 添加玩家预览视图
void SPWindowCashShop::InitPlayerView()
{
	SPWindowStatic* pkParent, * pkStatic;
	SPWindowButton* pkButton;

	pkParent = new SPWindowStatic(WIID_CASHSHOP_PLAYERVIEW, 7, 6, 226, 243, this);
	pkParent->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP05.PNG", 1, 1);

	//pkStatic = new SPWindowStatic(WIID_CASHSHOP_PLAYERVIEW_TITLE, 6, 9, 32, 13, pkParent);
	//pkStatic->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 133, 1);

	pkStatic = new SPWindowStatic(WIID_CASHSHOP_PLAYERVIEW_BG, 2, 2, 238, 215, pkParent);
	pkStatic->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP05.PNG", 1, 245);

	//pkButton = new SPWindowButton(WIID_CASHSHOP_PLAYERVIEW_BACK1, 122, 5, 38, 22, pkParent);
	//pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 395, 46);
	//pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 395, 46);
	//pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 395, 69);
	//pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 395, 69);
	//pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 395, 46);

	//pkButton = new SPWindowButton(WIID_CASHSHOP_PLAYERVIEW_BACK2, 154, 5, 38, 22, pkParent);
	//pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 434, 46);
	//pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 434, 46);
	//pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 434, 69);
	//pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 434, 69);
	//pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 434, 46);

	//pkButton = new SPWindowButton(WIID_CASHSHOP_PLAYERVIEW_BACK3, 187, 5, 38, 22, pkParent);
	//pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 473, 46);
	//pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 473, 46);
	//pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 473, 69);
	//pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 473, 69);
	//pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 473, 46);

	pkButton = new SPWindowButton(WIID_CASHSHOP_PLAYERVIEW_BUY, 9, 222, 51, 17, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 408, 356);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 408, 374);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 408, 392);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 4008, 410);

	pkButton = new SPWindowButton(WIID_CASHSHOP_PLAYERVIEW_WEARINFO, 61, 222, 51, 17, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 460, 356);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 460, 374);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 460, 392);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 460, 410);

	pkButton = new SPWindowButton(WIID_CASHSHOP_PLAYERVIEW_EXACT, 113, 222, 51, 17, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 408, 428);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 408, 446);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 408, 464);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 408, 482);

	pkButton = new SPWindowButton(WIID_CASHSHOP_PLAYERVIEW_INIT, 165, 222, 51, 17, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 460, 428);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 460, 446);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 460, 464);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/CASHSHOP04.PNG", 460, 482);

}

void SPWindowCashShop::InitSultListView()
{
	SPWindowStatic * pkParent, * pkStatic;

	pkParent = new SPWindowStatic(WIID_CASHSHOP_SULT, 244, 79, 547, 441, this);
	pkParent->SetWindowType(WND_TYPE_LEFTBASE);
	pkParent->SetImageNormalHead("DATA/INTERFACE/CONCEPT/CASHSHOP05.PNG", 306, 1, 309, 441);
	pkParent->SetImageNormalBody("DATA/INTERFACE/CONCEPT/CASHSHOP05.PNG", 311, 1, 312, 441);
	pkParent->SetImageNormalTail("DATA/INTERFACE/CONCEPT/CASHSHOP05.PNG", 314, 1, 317, 441);

	pkStatic = new SPWindowStatic(WIID_CASHSHOP_SULT_ITEM, 0, 0 , 229, 79, pkParent);

	pkParent = new SPWindowStatic(WIID_CASHSHOP_SULT_ITEM_BACK, 238, 0, 229, 400, pkParent);
	for( int i = 1; i < 6; i++ )
	{
		pkStatic = new SPWindowStatic(WIID_CASHSHOP_SULT_ITEM + i,
			6, 12+(i-1)*82, 226, 79, pkParent);
	}
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::InitBackground()
{
	m_vBackgroundTextList.clear();
	m_vBackgroundStarList.clear();

	BACKGROUND_ANIMAITON BackAniInfo;
	int i, j;
	int iLineCount[15] = {2, 3, 4, 6, 7, 9, 11, 11, 11, 9, 7, 6, 4, 2, 1};
	float fSpeed, fStrPosX, fStrPosY, fEndPosX, fEndPosY;
	RECT rcTextSrc;

	fSpeed = 40.0f;
	fStrPosX = 100.0f;
	fStrPosY = -100.0f;
	fEndPosX = -100.0f;
	fEndPosY = 100.0f;

	if( m_iMileageRatio )
		SetRect(&rcTextSrc, 801, 353, 875, 409);
	else
		SetRect(&rcTextSrc, 801, 353, 908, 441);

	for( i = 0; i < 15; i++ )
	{
		for( j = 0; j < iLineCount[i]; j++ )
		{
			BackAniInfo.vStrPos = SPVector(fStrPosX, fStrPosY, 0.0f);
			BackAniInfo.vEndPos = SPVector(fEndPosX, fEndPosY, 0.0f);
			BackAniInfo.fSpeed = fSpeed;
			BackAniInfo.fRotate = 0.0f;//D3DXToRadian(45.0f);
			BackAniInfo.rcSrc = rcTextSrc;
			
			BackAniInfo.vCurPos.m_fX = fStrPosX - (fStrPosX - fEndPosX) * ((float)j / (float)iLineCount[i]);
			BackAniInfo.vCurPos.m_fY = fStrPosY + (fEndPosY - fStrPosY) * ((float)j / (float)iLineCount[i]);

			m_vBackgroundTextList.push_back(BackAniInfo);
		}

		if( fStrPosX <= 900.f && fStrPosY < 0.0f )
		{
			fStrPosX += 100.0f;
			if( fStrPosX == 1000.0f )
			{
				fStrPosX = 900.0f;
				fStrPosY = 0.0f;
			}
		}
		else
			fStrPosY += 100.0f;

		if( fEndPosY <= 700.0f && fEndPosX < 0.0f )
		{
			fEndPosY += 100.0f;
			if( fEndPosY == 800.0f )
			{
				fEndPosY = 700.0f;
				fEndPosX = 0.0f;
			}
		}
		else
			fEndPosX += 100.0f;
	}

	fSpeed = 20.0f;
	fStrPosX = 50.0f;
	fStrPosY = -50.0f;
	fEndPosX = -50.0f;
	fEndPosY = 50.0f;
	
	for( i = 0; i < 10; i++ )
	{
		for( j = 0; j < 2;j++ )
		{
			BackAniInfo.vStrPos = SPVector(fStrPosX, fStrPosY, 0.0f);
			BackAniInfo.vEndPos = SPVector(fStrPosX, fEndPosY, 0.0f);
			BackAniInfo.fSpeed = fSpeed;
			BackAniInfo.fRotate = 0.0f;//D3DXToRadian(45.0f);
			SetRect(&BackAniInfo.rcSrc, 930, 266, 947, 284);

			BackAniInfo.vCurPos.m_fX = fStrPosX - (fStrPosX - fEndPosX) * ((float)j / 2.0f);
			BackAniInfo.vCurPos.m_fY = fStrPosY + (fEndPosY - fStrPosY) * ((float)j / 2.0f);

			m_vBackgroundStarList.push_back(BackAniInfo);
		}

		if( fStrPosX <= 850.f && fStrPosY < 0.0f )
		{
			fStrPosX += 200.0f;
			if( fStrPosX == 1050.0f )
			{
				fStrPosX = 850.0f;
				fStrPosY = 0.0f;
			}
		}
		else
			fStrPosY += 200.0f;

		if( fEndPosY <= 650.0f && fEndPosX < 0.0f )
		{
			fEndPosY += 200.0f;
			if( fEndPosY == 850.0f )
			{
				fEndPosY = 650.0f;
				fEndPosX = 0.0f;
			}
		}
		else
			fEndPosX += 200.0f;
	}

	fSpeed = 60.0f;
	fStrPosX = 20.0f;
	fStrPosY = -20.0f;
	fEndPosX = -20.0f;
	fEndPosY = 20.0f;

	for( i = 0; i < 10; i++ )
	{
		for( j = 0; j < 2; j++ )
		{
			BackAniInfo.vStrPos = SPVector(fStrPosX, fStrPosY, 0.0f);
			BackAniInfo.vEndPos = SPVector(fStrPosX, fEndPosY, 0.0f);
			BackAniInfo.fSpeed = fSpeed;
			BackAniInfo.fRotate = 0.0f;//D3DXToRadian(45.0f);
			SetRect(&BackAniInfo.rcSrc, 949, 266, 957, 275);

			BackAniInfo.vCurPos.m_fX = fStrPosX - (fStrPosX - fEndPosX) * ((float)j / 2.0f);
			BackAniInfo.vCurPos.m_fY = fStrPosY + (fEndPosY - fStrPosY) * ((float)j / 2.0f);

			m_vBackgroundStarList.push_back(BackAniInfo);
		}

		if( fStrPosX <= 820.f && fStrPosY < 0.0f )
		{
			fStrPosX += 200.0f;
			if( fStrPosX == 1020.0f )
			{
				fStrPosX = 820.0f;
				fStrPosY = 0.0f;
			}
		}
		else
			fStrPosY += 200.0f;

		if( fEndPosY <= 620.0f && fEndPosX < 0.0f )
		{
			fEndPosY += 200.0f;
			if( fEndPosY == 820.0f )
			{
				fEndPosY = 620.0f;
				fEndPosX = 0.0f;
			}
		}
		else
			fEndPosX += 200.0f;
	}
}

//------------------------------------------------------------------------------------
bool SPWindowCashShop::LoadHelpLDTFile()
{
	SPLDTFile* pkLDTFile = NULL;
	if( !g_pResourceManager->GetLDTFromFileName("DATA/LDT/CASHSHOP_HELP.LDT", &pkLDTFile) )
		return false;

	int iDataNum, iItemID, iPageNo, iFontSize, i;
	LDT_Field LFD_Value;
	CASHSHOP_HELP_INFO HelpInfo;

	iDataNum = pkLDTFile->GetItemCount();
	for( i = 0; i < iDataNum; i++ )
	{
		iItemID = pkLDTFile->GetPrimaryKey(i);

		pkLDTFile->GetFieldFromLable(iItemID, "_Page",			LFD_Value);
		iPageNo = LFD_Value.uData.lValue;

		if( iPageNo == 0 )
			break;

		pkLDTFile->GetFieldFromLable(iItemID, "_Image",			LFD_Value);
		HelpInfo.pkTexture = NULL;
		if( LFD_Value.uData.pValue )
			g_pVideo->GetTextureMgr()->GetTexture(LFD_Value.uData.pValue, &HelpInfo.pkTexture, TEX_INTERFACE, LEVEL_NOW);

		pkLDTFile->GetFieldFromLable(iItemID, "_Left",			LFD_Value);
		HelpInfo.rcImageSrc.left = LFD_Value.uData.lValue;

		pkLDTFile->GetFieldFromLable(iItemID, "_Top",			LFD_Value);
		HelpInfo.rcImageSrc.top = LFD_Value.uData.lValue;

		pkLDTFile->GetFieldFromLable(iItemID, "_Right",			LFD_Value);
		HelpInfo.rcImageSrc.right = LFD_Value.uData.lValue;

		pkLDTFile->GetFieldFromLable(iItemID, "_Bottom",		LFD_Value);
		HelpInfo.rcImageSrc.bottom = LFD_Value.uData.lValue;
		
		pkLDTFile->GetFieldFromLable(iItemID, "_Image_PosX",	LFD_Value);
		HelpInfo.rcImageDst.left = LFD_Value.uData.lValue;

		pkLDTFile->GetFieldFromLable(iItemID, "_Image_PosY",	LFD_Value);
		HelpInfo.rcImageDst.top = LFD_Value.uData.lValue;

		pkLDTFile->GetFieldFromLable(iItemID, "_Image_Width",	LFD_Value);
		HelpInfo.rcImageDst.right = HelpInfo.rcImageDst.left + LFD_Value.uData.lValue;

		pkLDTFile->GetFieldFromLable(iItemID, "_Image_Height",	LFD_Value);
		HelpInfo.rcImageDst.bottom = HelpInfo.rcImageDst.top + LFD_Value.uData.lValue;

		pkLDTFile->GetFieldFromLable(iItemID, "_Text_PosX",		LFD_Value);
		HelpInfo.rcTextDst.left = LFD_Value.uData.lValue;

		pkLDTFile->GetFieldFromLable(iItemID, "_Text_PosY",		LFD_Value);
		HelpInfo.rcTextDst.top = LFD_Value.uData.lValue;

		pkLDTFile->GetFieldFromLable(iItemID, "_Text_Width",	LFD_Value);
		HelpInfo.rcTextDst.right = HelpInfo.rcTextDst.left + LFD_Value.uData.lValue;

		pkLDTFile->GetFieldFromLable(iItemID, "_Text_Height",	LFD_Value);
		HelpInfo.rcTextDst.bottom = HelpInfo.rcTextDst.top + LFD_Value.uData.lValue;

		pkLDTFile->GetFieldFromLable(iItemID, "_Text_Size",	LFD_Value);
		iFontSize = LFD_Value.uData.lValue;

		HelpInfo.pFont = NULL;
		if( iFontSize == 12 )
			HelpInfo.pFont = g_pVideo->GetFont(FONT_12_NORMAL);
		else if( iFontSize == 14 )
			HelpInfo.pFont = g_pVideo->GetFont(FONT_14_NORMAL);

		HelpInfo.Color.a = 1.0f;
		pkLDTFile->GetFieldFromLable(iItemID, "_Red",			LFD_Value);
		HelpInfo.Color.r = (float)LFD_Value.uData.lValue / 255.0f;

		pkLDTFile->GetFieldFromLable(iItemID, "_Green",			LFD_Value);
		HelpInfo.Color.g = (float)LFD_Value.uData.lValue / 255.0f;

		pkLDTFile->GetFieldFromLable(iItemID, "_Blue",			LFD_Value);
		HelpInfo.Color.b = (float)LFD_Value.uData.lValue / 255.0f;

		HelpInfo.strText = "";
		pkLDTFile->GetFieldFromLable(iItemID, "_Text",			LFD_Value);
		if( LFD_Value.uData.pValue )
			HelpInfo.strText = LFD_Value.uData.pValue;

		InsertHelpInfo(iPageNo, &HelpInfo);
	}
	
	return true;
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::InsertHelpInfo(int iPageNo, CASHSHOP_HELP_INFO* pHelpInfo)
{
	vector< CASHSHOP_HELP_INFO >* pvPageHelpList;
	pvPageHelpList = GetHelpInfo(iPageNo);
	if( pvPageHelpList )
	{
		(*pvPageHelpList).push_back(*pHelpInfo);
		return;
	}

	vector< CASHSHOP_HELP_INFO > vTempList;
	vTempList.push_back(*pHelpInfo);
	m_mHelpInfoList.insert(make_pair(iPageNo, vTempList));
	vTempList.clear();
}

//------------------------------------------------------------------------------------
vector< CASHSHOP_HELP_INFO >* SPWindowCashShop::GetHelpInfo(int iPageNo)
{
	map< int, vector< CASHSHOP_HELP_INFO > >::iterator iter = m_mHelpInfoList.find(iPageNo);
	if( iter == m_mHelpInfoList.end() )
		return NULL;

	return &((*iter).second);
}

//------------------------------------------------------------------------------------
bool SPWindowCashShop::LoadDefaultLDTFile()
{
	SPLDTFile* pkLDTFile = NULL;
	if( !g_pResourceManager->GetLDTFromFileName("DATA/LDT/CASHSHOP_DEFAULT.LDT", &pkLDTFile) )
		return false;

	int iDataNum, iItemID, iIndex, iPage, i;
	LDT_Field LFD_Value;
	
	m_mDefaultPageList.clear();
	iDataNum = pkLDTFile->GetItemCount();
	for( i = 0; i < iDataNum; i++ )
	{
		iItemID = pkLDTFile->GetPrimaryKey(i);

		pkLDTFile->GetFieldFromLable(iItemID, "_CategoryIndex",	LFD_Value);
		iIndex = LFD_Value.uData.lValue;

		pkLDTFile->GetFieldFromLable(iItemID, "_DefaultPage",	LFD_Value);
		iPage = LFD_Value.uData.lValue;

		switch( iIndex )
		{
		case CI_FASSION:
			iPage -= CIS1_DRESS;
			break;
		case CI_PET:
			iPage -= CIS1_PET;
			break;
		case CI_ETC:
			iPage -= CIS1_FUNCTIONAL;
			break;
		case CI_HOUSING:
			iPage -= CIS1_HOUSING;
			break;
		case CI_PVP:
			iPage -= CIS1_PVP;
			break;
		case CI_PACKAGE:
			iPage -= CIS1_PACKAGE_FASSION;
			break;
#ifndef __CASHSHOP_POINT_SHOP__
		case 7:
			iIndex = CI_EVENT;
			iPage -= 19;
#else
		case CI_EVENT:
			iPage -= CIS1_EVENT_NEW;
			break;
		case CI_POINT:
			iPage -= CIS1_POINT_FASSION;
			break;
#endif
		}

		m_mDefaultPageList.insert(make_pair(iIndex, iPage));
	}

	return true;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::GetDefaultPage(int iIndex)
{
	int iRet = 0;
	map< int, int >::iterator iter = m_mDefaultPageList.find(iIndex);

	if( iter != m_mDefaultPageList.end() )
		iRet = (*iter).second;

	return iRet;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::SetSliderMaxPage(int iType, int iPage)
{
	SPWindow* pkWindow = NULL;
	switch( iType )
	{
	case 1:
		pkWindow = Find(WIID_CASHSHOP_ITEM_INVEN_THUMB, true);
		break;
	case 2:
		pkWindow = Find(WIID_CASHSHOP_OZ_INVEN_THUMB, true);
		break;
	}

	if( pkWindow == NULL )
		return 1;

	pkWindow->SPSendMessage(SPIM_SET_MAXVALUE, iPage);
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::SetSliderPage(int iType, int iPage)
{
	SPWindow* pkWindow = NULL;
	switch( iType )
	{
	case 1:
		pkWindow = Find(WIID_CASHSHOP_ITEM_INVEN_THUMB, true);
		break;
	case 2:
		pkWindow = Find(WIID_CASHSHOP_OZ_INVEN_THUMB, true);
		break;
	}

	if( pkWindow == NULL )
		return 1;

	pkWindow->SPSendMessage(SPIM_SET_CURVALUE, iPage);
	return 1;
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::SetSelectSlot(int iSlotIndex, bool bPrevView)
{
	if( m_iPreSelectSlot != iSlotIndex || m_bPreViewSelect != bPrevView )
	{
		RECT rc;
		if( bPrevView )
		{
			rc.left = 20 + (iSlotIndex % 5) * 36;
			rc.top = 133 + (iSlotIndex / 5) * 36;
		}
		else
		{
			rc.left = 20 + (iSlotIndex % 5) * 36;
			rc.top = 292 + (iSlotIndex / 5) * 36;
		}

		rc.right = rc.left + 36;
		rc.bottom = rc.top + 36;

		m_bPreViewSelect = bPrevView;
		m_iPreSelectSlot = iSlotIndex;
		

		ZeroMemory(&m_PreSelectAni, sizeof(EVENT_ANIMATION));
		m_PreSelectAni.fDelayTime = 0.1f;
		m_PreSelectAni.iTotalFrame = 4;
		m_PreSelectAni.rcDst = rc;
	}
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::ClearSelectItem()
{
	if( m_iSelectSlot >= 0 && m_pkSelectItem )
	{
		unsigned int uiWndID = 0;
		switch( m_iSelectInven )
		{
		case 0:
			uiWndID = WIID_CASHSHOP_ITEM_INVEN_SLOT;
			break;
		case 1:
			uiWndID = WIID_CASHSHOP_OZ_INVEN_SLOT;
			break;
		}
		if( uiWndID > 0 )
		{
			SPWindowGrid* pkGridWindow = static_cast< SPWindowGrid* >(Find(uiWndID, true));
			if( pkGridWindow )
				pkGridWindow->CellSelectSell(m_iSelectSlot, false);
		}
	}

	m_iSelectSlot = -1;
	m_iSelectInven = -1;
	m_pkSelectItem = NULL;

	g_pInterfaceManager->ClearMouse();

	//if( m_pSellItem == NULL )
	//	ClearOZSellInfo();
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::SetCategoryDiable()
{
	SPWindow* pkParent, *pkChild;
	int iFilter, iSize, i, j;

#ifndef __CASHSHOP_POINT_SHOP__
	int iCategorySize1 = 5;
	int iCategorySize2[5] = {3, 6, 2, 3, 3};	//	菩记 辑宏皋春 5->6 函版 扁鸥眠啊  [11/27/2007 AJJIYA]
	int iFilter1[5] = {CI_EVENT, CI_FASSION, CI_PET, CI_ETC, CI_PACKAGE};
	int iFilter2[5] = {1, CIS1_DRESS, CIS1_PET, CIS1_FUNCTIONAL, CIS1_PACKAGE_FASSION};

	int iCategory1WndID[5] = {
			WIID_CASHSHOP_STORE_EVENT,
			WIID_CASHSHOP_STORE_FASSION,
			WIID_CASHSHOP_STORE_PET,
			WIID_CASHSHOP_STORE_ETC,
			WIID_CASHSHOP_STORE_PACKAGE,
	};

	int iCategory2WndID[5] = {
			WIID_CASHSHOP_STORE_EVENT_NEW,
			WIID_CASHSHOP_STORE_FASSION_DRESS,
			WIID_CASHSHOP_STORE_PET_PET,
			WIID_CASHSHOP_STORE_ETC_FUNCTIONAL,
			WIID_CASHSHOP_STORE_PACK_FASSION
	};

#else
	int iCategorySize1 = 6;
	int iCategorySize2[6] = {3, 6, 2, 3, 3, 3};
	int iFilter1[6] = {CI_EVENT, CI_FASSION, CI_PET, CI_ETC, CI_PACKAGE, CI_POINT};
	int iFilter2[6] = {1, CIS1_DRESS, CIS1_PET, CIS1_FUNCTIONAL, CIS1_PACKAGE_FASSION, CIS1_POINT_FASSION};

	int iCategory1WndID[6] = {
			WIID_CASHSHOP_STORE_EVENT,
			WIID_CASHSHOP_STORE_FASSION,
			WIID_CASHSHOP_STORE_PET,
			WIID_CASHSHOP_STORE_ETC,
			WIID_CASHSHOP_STORE_PACKAGE,
			WIID_CASHSHOP_STORE_POINT
	};

	int iCategory2WndID[6] = {
			WIID_CASHSHOP_STORE_EVENT_NEW,
			WIID_CASHSHOP_STORE_FASSION_DRESS,
			WIID_CASHSHOP_STORE_PET_PET,
			WIID_CASHSHOP_STORE_ETC_FUNCTIONAL,
			WIID_CASHSHOP_STORE_PACK_FASSION,
			WIID_CASHSHOP_STORE_POINT_FASSION
	};
#endif

	for( i = 0; i < iCategorySize1; i++ )
	{
		pkParent = Find(iCategory1WndID[i], true);
		if( pkParent )
		{
			iFilter = (iFilter1[i] << 16);
			iSize = g_pItemCluster->GetSortCashItemSize(iFilter);
			if( iSize )
			{
				pkParent->SetEnable(true);
				for( j = 0; j < iCategorySize2[i]; j++ )
				{
					pkChild = Find(iCategory2WndID[i] + j, true);
					if( pkChild == NULL )
						continue;

					iFilter = (iFilter1[i] << 16);
					if( i == 4 && j == 2 )
						iFilter |= (CIS1_PACKAGE_ETC << 8);
					else
						iFilter |= ((j + iFilter2[i]) << 8);

					iSize = g_pItemCluster->GetSortCashItemSize(iFilter);
					if( iSize )
						pkChild->SetEnable(true);
					else
						pkChild->SetEnable(false);
				}
			}
			else
				pkParent->SetEnable(false);
		}
	}
}

//------------------------------------------------------------------------------------
bool SPWindowCashShop::IsBuyRequireSuccess(SPCashItemAttr* pkItemAttr)
{
	if( g_pCheckManager == NULL || pkItemAttr == NULL )
		return false;

	int i;
	for( i = 0; i < CASHITEM_REQUIRE_NUM; i++ )
	{
		if( pkItemAttr->ItemRequire[i].m_iType )
		{
			if( !g_pCheckManager->CheckCondition(pkItemAttr->ItemRequire[i].m_iType, pkItemAttr->ItemRequire[i].m_iID,
				pkItemAttr->ItemRequire[i].m_iValue1, pkItemAttr->ItemRequire[i].m_iValue2) )
				return false;
		}
	}

	return true;
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::Show(bool bWithChild)
{
	SPWindow::Show(bWithChild);

	//[xialin 2008/08/05] add >>>
	SPPlayer * pkLocalPlayer = g_pGOBManager->GetLocalPlayer();
	if ( m_pkPlayer && pkLocalPlayer )
	{
		OnChangePlayerViewBack(WIID_CASHSHOP_PLAYERVIEW_BACK1, 0 ,0);

		CHARACTER_INFO2   Info;
		memset(&Info, 0x00, sizeof(Info));
		Info.MovingStatus.fDropSpeedNow =  pkLocalPlayer->GetCoordPhysics()->GetGravity();
		Info.MovingStatus.fDropSpeedMax = pkLocalPlayer->GetCoordPhysics()->GetMaxDropVelocity();
		Info.MovingStatus.fMaxVelocityX = pkLocalPlayer->GetCoordPhysics()->GetMaxVelocityX();
		Info.MovingStatus.fJumpSpeed = pkLocalPlayer->GetCoordPhysics()->GetJumpSpeed();
		Info.MovingStatus.fUpDownVelocity = pkLocalPlayer->GetCoordPhysics()->GetMaxUpDownVelocity();
		Info.MovingStatus.fHangingVelocity = pkLocalPlayer->GetCoordPhysics()->GetMaxHangingVelocity();

		m_pkPlayer->GetCoordPhysics()->SetGravity(Info.MovingStatus.fDropSpeedNow);
		m_pkPlayer->GetCoordPhysics()->SetMaxDropVelocity(Info.MovingStatus.fDropSpeedMax);
		m_pkPlayer->GetCoordPhysics()->SetMaxVelocityX(120.0f/*Info.MovingStatus.fMaxVelocityX*/);
		m_pkPlayer->GetCoordPhysics()->SetJumpSpeed(Info.MovingStatus.fJumpSpeed);
		m_pkPlayer->GetCoordPhysics()->SetMaxUpDownVelocity(Info.MovingStatus.fUpDownVelocity);
		m_pkPlayer->GetCoordPhysics()->SetMaxHangingVelocity(Info.MovingStatus.fHangingVelocity);

		m_pkPlayer->GetSkillArchive()->Clear();
		int iSkillCount = pkLocalPlayer->GetSkillArchive()->GetSkillCount();
		SPSkillActivity* pSkillActivity = pkLocalPlayer->GetSkillArchive()->GetFirstSkillActivity();
		for ( int i=0; i < iSkillCount; i++ )
		{
			if ( pSkillActivity ) 
				m_pkPlayer->GetSkillArchive()->AddSkill(i, pSkillActivity);
			pSkillActivity = pkLocalPlayer->GetSkillArchive()->GetNextSkillActivity();
		}

		m_pkPlayer->SetControlAction(pkLocalPlayer->GetControl());
		m_pkPlayer->SetAction(pkLocalPlayer->GetControl(), true);
		m_pkPlayer->SPGOBPerformMessage(SPGM_SET_PET, -1);	
	}
	//[xialin 2008/08/05] add <<<


	//
	m_eBuyType = TYPE_BUY;
	m_iItemInvenCategory = -1;
	m_eStoreTabType = CST_NULL;
	ZeroMemory(m_iStoreCategory, sizeof(m_iStoreCategory));
	ZeroMemory(m_iItemInvenCurPage, sizeof(int) * 6);
	ZeroMemory(m_iItemInvenMaxPage, sizeof(int) * 6);

	m_strGiftFriendName.clear();
	m_strGiftFriendMsg.clear();

	m_vecBuyCashItemList.clear();

	m_iGUID = 0;
	m_szAccountID[0] = '\0';
	m_uiCurOZ = 0;
	m_uiCurMileage = 0;
	ClearPreViewSelect();

	m_iOZInvenCurPage[0] = 0;
	m_iOZInvenCurPage[1] = 0;
	SetOZInvenCategory(0);
	m_vCartList.clear();

	m_fNoticeAccmulateTime = 0.0f;
	m_fNoticeChangeTime = 0.0f;

	m_fSellHelpAccumulate = 0.0f;
	m_iSellHelpAlpha = 255;

	m_bWebbrowerLoadingWait = false;
	m_iErrorType = 0;

	m_pCurSultItemAttr = NULL;

	ZeroMemory(m_iSortType, sizeof(m_iSortType));

	SPWindowComboBox* pkComboBox;
	pkComboBox = (SPWindowComboBox*)Find(WIID_CASHSHOP_STORE_SORT_GENDER, true);
	if( pkComboBox )
		pkComboBox->SetEditBoxItem(0);

	pkComboBox = (SPWindowComboBox*)Find(WIID_CASHSHOP_STORE_SORT_PRICE, true);
	if( pkComboBox )
		pkComboBox->SetEditBoxItem(0);

	if ( g_pItemCluster )
		g_pItemCluster->SetSearchKey("");

	UpdateItemList();

	ClearSelectItem();
	
	UpdateMyInfo();
	ClearOZInven();
	UpdateOZInven();

	SetItemInvenCategory(0);
	
	SetStoreTabType(CST_EVENT);
	HideStorePopupMenu(0);

	ClearPreViewInven();
	ResetPlayerLayer();
	//InitPlayerAnimation();

	ClearEffectList();
	SetCategoryDiable();

	SPWindow* pkWindow = Find(WIID_CASHSHOP_STORE_SEARCH_EDIT, true);
	if ( pkWindow )
		pkWindow->SetWindowText("");

	pkWindow = Find(WIID_CASHSHOP_BUYLIST);
	if( pkWindow )
		pkWindow->Hide();

	pkWindow = Find(WIID_CASHSHOP_WEARLIST);
	if( pkWindow )
		pkWindow->Hide();

	pkWindow = Find(WIID_CASHSHOP_EXACTLIST);
	if( pkWindow )
		pkWindow->Hide();

	pkWindow = Find(WIID_CASHSHOP_SULT);
	if( pkWindow )
		pkWindow->Hide();
	
	pkWindow = Find(WIID_CASHSHOP_CASHIER);
	if( pkWindow )
		pkWindow->Hide();

	pkWindow = Find(WIID_CASHSHOP_GIFT);
	if( pkWindow )
		pkWindow->Hide();

	pkWindow = Find(WIID_CASHSHOP_EXACT);
	if( pkWindow )
		pkWindow->Hide();

	pkWindow = Find(WIID_CASHSHOP_ERROR);
	if( pkWindow )
		pkWindow->Hide();

	pkWindow = Find(WIID_CASHSHOP_COUPON);
	if( pkWindow )
		pkWindow->Hide();

	pkWindow = Find(WIID_CASHSHOP_SELL);
	if( pkWindow )
		pkWindow->Hide();

	pkWindow = Find(WIID_CASHSHOP_RESULT);
	if( pkWindow )
		pkWindow->Hide();

	pkWindow = Find(WIID_CASHSHOP_BUYWAIT);
	if( pkWindow )
		pkWindow->Hide();

	pkWindow = Find(WIID_CASHSHOP_OZ_SELL_HELP);
	if( pkWindow )
		pkWindow->SetColor(RGBA(255, 255, 255, 255));

	m_vCashierItemAttr.clear();
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::PerformMouseMove(int iX, int iY)
{
	SPWindow::PerformMouseMove(iX, iY);

	SPWindow * pkWindow = NULL;
	pkWindow = Find(WIID_CASHSHOP_STORE_SORT_PRICE, true);
	if( pkWindow )
	{
		if ( !pkWindow->PerformMouseMove(iX, iY) )
		{
			pkWindow = Find(WIID_CASHSHOP_STORE_SORT_GENDER, true);
			if( pkWindow )
				pkWindow->PerformMouseMove(iX, iY);
		}
	}

	return 1;
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::UpdateMyInfo()
{
	if( g_pGOBManager == NULL || g_pGOBManager->GetLocalPlayer() == NULL )
		return;

	SPWindow* pkWindow;	

	pkWindow = Find(WIID_CASHSHOP_INFO_OZ, true);
	if( pkWindow )
		pkWindow->SetWindowText(ConvertMoneyToString(m_uiCurOZ));

	pkWindow =Find(WIID_CASHSHOP_INFO_MILEAGE, true);
	if( pkWindow )
		pkWindow->SetWindowText(ConvertMoneyToString(m_uiCurMileage));

	if( m_pkPlayer )
	{
		char szBuf[64];
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_GETGOBNAME, (LPARAM)szBuf);
		m_pkPlayer->SetGOBName(szBuf);
	}
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::SetErrorDlg(int iError, bool bShowButton)
{
	if( (g_pSubGameManager == NULL) || g_pSubGameManager->GetGameState() != GS_CASHSHOP )
		return;

	SPWindow* pkParentWindow, *pkChildWindow;
	pkParentWindow = Find(WIID_CASHSHOP_ERROR);
	if( pkParentWindow == NULL )
		return;

	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_ERROR_MESSAGE);
	if( pkChildWindow )
		pkChildWindow->SetWindowText(g_pResourceManager->GetGlobalString(iError));

	pkParentWindow->Show();
	if( !bShowButton )
	{
		pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_ERROR_OK);
		if( pkChildWindow )
			pkChildWindow->Hide();
	}
	g_pInterfaceManager->SetModalWindow(pkParentWindow);
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::SetErrorDlg(const char* pszMsg, bool bShowButton)
{
	if( (g_pSubGameManager == NULL) || g_pSubGameManager->GetGameState() != GS_CASHSHOP )
		return;

	SPWindow* pkParentWindow, *pkChildWindow;
	pkParentWindow = Find(WIID_CASHSHOP_ERROR);
	if( pkParentWindow == NULL )
		return;

	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_ERROR_MESSAGE);
	if( pkChildWindow )
		pkChildWindow->SetWindowText(pszMsg);

	pkParentWindow->Show();
	if( !bShowButton )
	{
		pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_ERROR_OK);
		if( pkChildWindow )
			pkChildWindow->Hide();
	}
	g_pInterfaceManager->SetModalWindow(pkParentWindow);
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::ShowCouponDlg()
{
	if( (g_pSubGameManager == NULL) || g_pSubGameManager->GetGameState() != GS_CASHSHOP )
		return;

	SPWindow* pkParentWindow, *pkChildWindow;
	pkParentWindow = Find(WIID_CASHSHOP_COUPON);
	if( pkParentWindow == NULL )
		return;

	for( int i = 0; i < 4; i++ )
	{
		pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_COUPON_NUMBER + i, true);
		if( pkChildWindow )
			pkChildWindow->SetWindowText(NULL);
	}

	pkParentWindow->Show();
	g_pInterfaceManager->SetModalWindow(pkParentWindow);
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::ShowResultDlg(CASHSHOP_BUY_TYPE BuyType, int iItemID, const char* pszTargetName)
{
	m_pkResultItemAttr = g_pItemCluster->GetMinorCashItem(iItemID);
	if( m_pkResultItemAttr == NULL )
		return;

	SPWindow* pkParentWindow, *pkChildWindow;
	pkParentWindow = Find(WIID_CASHSHOP_RESULT);
	if( pkParentWindow == NULL )
		return;

	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_RESULT_ITEM, true);
	if( pkChildWindow )
	{
		pkChildWindow->SPSendMessage(SPIM_SET_IMAGECLEAR);

		SPTexture* pkTexture;
		POINT pt;
		pkTexture = g_pItemCluster->GetItemTextureFromTextureInx(
			m_pkResultItemAttr->iIcon, m_pkResultItemAttr->iIconIndex, pt);
		if( pkTexture )
			pkChildWindow->SetImage(pkTexture, pt.x, pt.y);
	}

	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_RESULT_ITEM_NAME, true);
	if( pkChildWindow )
		pkChildWindow->SetWindowText(m_pkResultItemAttr->strName.c_str());

	int iPrice = m_pkResultItemAttr->iPrice;
	if( m_pkResultItemAttr->iSalePrice )
		iPrice = m_pkResultItemAttr->iSalePrice;
	//CaluCashierGlow(iPrice);
	//int iNum10 = (iPrice / 10) % 10;
	//if( iNum10 )
	//	iPrice += 100 - (iNum10 * 10);

	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_RESULT_PRICE, true);
	if( pkChildWindow )
		pkChildWindow->SetWindowText(ConvertMoneyToString(iPrice));

	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_RESULT_BUY_OZ, true);
	if( pkChildWindow )
		pkChildWindow->SetWindowText(ConvertMoneyToString(iPrice));

	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_RESULT_MY_OZ, true);
	if( pkChildWindow )
		pkChildWindow->SetWindowText(ConvertMoneyToString(m_uiCurOZ));

	//////////////////////////////////////////////////////////////////////////

#ifdef __CASHSHOP_POINT_SHOP__
	if( m_iMileageRatio )
	{
		pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_RESULT_OZ_ICON, true);
		if( pkChildWindow )
		{
			if( m_pkResultItemAttr->iMaxOZ > 0 )
				pkChildWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , 0 );
			else
				pkChildWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , 1 );
		}

		pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_RESULT_BUY_OZ_ICON, true);
		if( pkChildWindow )
		{
			if( m_pkResultItemAttr->iMaxOZ > 0 )
				pkChildWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , 0 );
			else
				pkChildWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , 1 );
		}

		pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_RESULT_MY_OZ_ICON, true);
		if( pkChildWindow )
		{
			if( m_pkResultItemAttr->iMaxOZ > 0 )
				pkChildWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , 0 );
			else
				pkChildWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , 1 );
		}

		pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_RESULT_MY_OZ, true);
		if( pkChildWindow )
			pkChildWindow->SetWindowText(ConvertMoneyToString(m_uiCurMileage));

	}
#endif

	//////////////////////////////////////////////////////////////////////////

	string strMsg;
	m_eBuyType = BuyType;
	switch( m_eBuyType )
	{
	case TYPE_BUY:
		if( m_iMileageRatio )
			strMsg = g_pResourceManager->GetGlobalString(9001007);
		else
			strMsg = g_pResourceManager->GetGlobalString(9001005);
		break;
	case TYPE_GIFT:
		if( pszTargetName )
		{
			char szTemp[256];
			wsprintf(szTemp, g_pResourceManager->GetGlobalString(9004005), pszTargetName);
			strMsg = szTemp;
		}
		break;
	case TYPE_EXACT:
		if( pszTargetName )
		{
			char szTemp[256];
			wsprintf(szTemp, g_pResourceManager->GetGlobalString(9004005), pszTargetName);
			strMsg = szTemp;
		}
		break;
	}

	pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_RESULT_MSG, true);
	if( pkChildWindow )
		pkChildWindow->SetWindowText(strMsg.c_str());

	pkParentWindow->Show();
	g_pInterfaceManager->SetModalWindow(pkParentWindow);
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::ShowBuyWaitDlg()
{
	SPWindow* pkParentWindow;
	pkParentWindow = Find(WIID_CASHSHOP_BUYWAIT);
	if( pkParentWindow == NULL )
		return;

	m_bOnceAni = false;
	InitBuyWaitAni(pkParentWindow);
	m_fBuyWaitAccmulateTime = 0.0f;
	pkParentWindow->Show();
	g_pInterfaceManager->SetModalWindow(pkParentWindow);
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::HideBuyWaitDlg()
{
	SPWindow* pkParentWindow;
	pkParentWindow = Find(WIID_CASHSHOP_BUYWAIT);
	if( pkParentWindow == NULL )
		return;

	pkParentWindow->Hide();
	g_pInterfaceManager->SetModalWindow(NULL);
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::CaluCashierGlow(int& iPrice)
{
	if( m_iCashierGlowUnit == 0 )
		return;

	if( iPrice < m_iCashierGlowUnit )
	{
		iPrice = m_iCashierGlowUnit;
		return;
	}

	int iGlowUnit;
	iGlowUnit = iPrice % m_iCashierGlowUnit;
	if( iGlowUnit )
		iPrice += m_iCashierGlowUnit - iGlowUnit;
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::InitBuyWaitAni(SPWindow* pkParentWindow)
{
	SPWindow* pkWindow;
	if( pkParentWindow == NULL )
		return;

	int i;
	for( i = 0; i < 14; i++ )
	{
		m_BuyAni[i].fAccmulateTime = 0.0f - (i * 0.06f);
		m_BuyAni[i].fDelayTime = 0.05f;
		m_BuyAni[i].iCurFrame = 0;
		m_BuyAni[i].iTotalFrame = -1;
		m_BuyAni[i].iIncDir = 1;

		pkWindow = pkParentWindow->Find(WIID_CASHSHOP_BUYWAIT_ANI + i, true);
		if( pkWindow )
			pkWindow->Hide();
	}
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::Process(float fTime)
{
	if( !m_bShow )
		return;

	SPWindow::Process(fTime);

	vector< EVENT_ANIMATION >::iterator iter = m_vEventAnimation.begin();
	while( iter != m_vEventAnimation.end() )
	{
		(*iter).fAccmulateTime += fTime;
		if( (*iter).fAccmulateTime > (*iter).fDelayTime )
		{
			(*iter).fAccmulateTime = 0.0f;
			(*iter).iCurFrame++;

			if( (*iter).iCurFrame >= (*iter).iTotalFrame )
				(*iter).iCurFrame = 0;
		}
		++iter;
	}


	if( m_iPreSelectSlot >= 0 )
	{
		m_PreSelectAni.fAccmulateTime += fTime;
		if( m_PreSelectAni.fAccmulateTime > m_PreSelectAni.fDelayTime )
		{
			m_PreSelectAni.fAccmulateTime = 0.0f;
			m_PreSelectAni.iCurFrame++;
			if( m_PreSelectAni.iCurFrame >= m_PreSelectAni.iTotalFrame )
				m_PreSelectAni.iCurFrame = 0;
		}
	}

	if( m_pkPlayer )
		m_pkPlayer->Process(fTime);

	//if( m_pkPet )
	//	m_pkPet->Process(fTime);

	ProcessBackground(fTime);
	ProcessHotKey(fTime);
	ProcessNoticeRandom(fTime);
	ProcessAni(fTime);
	ProcessBuyWaitAni(fTime);
	ProcessSellHelp(fTime);	
	ProcessWebbrowser(fTime);
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::ProcessBackground(float fTime)
{
	vector< BACKGROUND_ANIMAITON >::iterator iter;
	BACKGROUND_ANIMAITON* pAniInfo;
	float fDiff;

	iter = m_vBackgroundTextList.begin();
	while( iter != m_vBackgroundTextList.end() )
	{
		pAniInfo = &(*iter);
		pAniInfo->vCurPos.m_fX -= (pAniInfo->fSpeed * fTime);
		pAniInfo->vCurPos.m_fY += (pAniInfo->fSpeed * fTime);
		if( pAniInfo->vCurPos.m_fX < pAniInfo->vEndPos.m_fX )
		{
			fDiff = pAniInfo->vEndPos.m_fX - pAniInfo->vCurPos.m_fX;
			pAniInfo->vCurPos.m_fX = pAniInfo->vStrPos.m_fX - fDiff;
			pAniInfo->vCurPos.m_fY = pAniInfo->vStrPos.m_fY + fDiff;
		}

		if( pAniInfo->vCurPos.m_fY > pAniInfo->vEndPos.m_fY )
		{
			fDiff = pAniInfo->vCurPos.m_fY - pAniInfo->vEndPos.m_fY;
			pAniInfo->vCurPos.m_fX = pAniInfo->vStrPos.m_fX - fDiff;
			pAniInfo->vCurPos.m_fY = pAniInfo->vStrPos.m_fY + fDiff;
		}

		++iter;
	}

	iter = m_vBackgroundStarList.begin();
	while( iter != m_vBackgroundStarList.end() )
	{
		pAniInfo = &(*iter);
		pAniInfo->vCurPos.m_fX -= (pAniInfo->fSpeed * fTime);
		pAniInfo->vCurPos.m_fY += (pAniInfo->fSpeed * fTime);
		if( pAniInfo->vCurPos.m_fX < pAniInfo->vEndPos.m_fX )
		{
			fDiff = pAniInfo->vEndPos.m_fX - pAniInfo->vCurPos.m_fX;
			pAniInfo->vCurPos.m_fX = pAniInfo->vStrPos.m_fX - fDiff;
			pAniInfo->vCurPos.m_fY = pAniInfo->vStrPos.m_fY + fDiff;
		}

		if( pAniInfo->vCurPos.m_fY > pAniInfo->vEndPos.m_fY )
		{
			fDiff = pAniInfo->vCurPos.m_fY - pAniInfo->vEndPos.m_fY;
			pAniInfo->vCurPos.m_fX = pAniInfo->vStrPos.m_fX - fDiff;
			pAniInfo->vCurPos.m_fY = pAniInfo->vStrPos.m_fY + fDiff;
		}

		pAniInfo->fRotate += (pAniInfo->fSpeed * fTime * 0.5f);
		++iter;
	}
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::ProcessHotKey(float fTime)
{
	if( g_pInputManager == NULL || g_pInterfaceManager == NULL )
		return;

	SPInputStruct* pInputStruct = g_pInputManager->GetInputStruct();
	if( pInputStruct == NULL )
		return;

	if( pInputStruct->ikEscape.InputState == INPUT_PRESS )
	{
		if( g_pInterfaceManager->GetModalWindow() )
		{
			g_pInterfaceManager->GetModalWindow()->Hide();
			m_vCashierItemAttr.clear();

			g_pInterfaceManager->SetModalWindow(NULL);
		}

		if( m_iErrorType == 1 )
			OnExit(0, 0);
	}
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::ProcessNoticeRandom(float fTime)
{
	SPWindow* pkWindow = Find(WIID_CASHSHOP_NOTICE);
	if( pkWindow == NULL )
		return;

	m_fNoticeAccmulateTime += fTime;
	if( m_fNoticeAccmulateTime > m_fNoticeChangeTime )
	{
		STD_VECTOR_CHAT_ADVERTISEMENT* pChatAdvertise;
		g_pInterfaceManager->SPChildWndSendMessage(WIID_CHAT, SPIM_WNDCHAT_GET_NOTICE,
			(WPARAM)(&pChatAdvertise), MAKELPARAM(0, 99));

		if( pChatAdvertise == NULL )
			return;
		
		int iIndex, iSize;
		iSize = (int)(*pChatAdvertise).size();
		if( iSize == 0 )
			return;

		iIndex = rand() % iSize;
		CHAT_ADVERTISEMENT NoticeInfo;
		NoticeInfo = (*pChatAdvertise).at(iIndex);

		pkWindow->SetFontColor(NoticeInfo.m_cColor);
		pkWindow->SetWindowText(NoticeInfo.m_strMessage.c_str());

		m_fNoticeChangeTime = NoticeInfo.m_fLimitTime / 10.0f;
		m_fNoticeAccmulateTime = 0.0f;
	}
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::ProcessAni(float fTime)
{
	if( m_fAniMaxTime == 0.0f )
		return;

	m_fAniAccmulateTime += fTime;
	if( m_fAniAccmulateTime > m_fAniMaxTime )
	{
		m_fAniAccmulateTime = 0.0f;
		m_fAniMaxTime = 0.0f;

		SetAniSeq(m_uiBeforeAniIndex);
	}
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::ProcessBuyWaitAni(float fTime)
{
	SPWindow* pkParentWindow, *pkChildWindow;
	pkParentWindow = Find(WIID_CASHSHOP_BUYWAIT);
	if( pkParentWindow == NULL || !pkParentWindow->IsShow() )
		return;

	int iSrcX[5] = {343, 356, 369, 356, 369};
	int iSrcY[5] = {82, 82, 82, 52, 52};

	int i;
	bool bHide;
	for( i = 0; i < 14; i++ )
	{
		if( m_BuyAni[i].iTotalFrame >= 0 )
			continue;

		pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_BUYWAIT_ANI + i, true);
		if( pkChildWindow == NULL )
			continue;

		bHide = (m_BuyAni[i].fAccmulateTime <= 0.0f) ? true : false;
		m_BuyAni[i].fAccmulateTime += fTime;

		if( bHide && m_BuyAni[i].fAccmulateTime >= 0.0f )
			pkChildWindow->Show();

		if( m_BuyAni[i].fAccmulateTime > m_BuyAni[i].fDelayTime )
		{
			m_BuyAni[i].fAccmulateTime -= m_BuyAni[i].fDelayTime;
			m_BuyAni[i].iCurFrame += m_BuyAni[i].iIncDir;

			if( m_BuyAni[i].iCurFrame > 4 )
			{
				m_BuyAni[i].iCurFrame = 3;
				m_BuyAni[i].iIncDir = -1;
			}
			else if( m_BuyAni[i].iCurFrame < 0 )
			{
				m_BuyAni[i].iTotalFrame = 0;
				m_BuyAni[i].iCurFrame = 0;
				pkChildWindow->Hide();

				if( i == 13 )
				{
					if( m_bBuyRecv )
					{
						HideBuyWaitDlg();
						ShowResultDlg(m_eBuyType, m_uiBuyItem, m_strTargetName.c_str());
						return;
					}
					else
					{
						InitBuyWaitAni(pkParentWindow);
						m_bOnceAni = true;
					}
				}
			}
		}

		pkChildWindow->SetImagePos(iSrcX[m_BuyAni[i].iCurFrame], iSrcY[m_BuyAni[i].iCurFrame]);
	}

	m_fBuyWaitAccmulateTime += fTime;
	if( m_fBuyWaitAccmulateTime > 180.0f )	// 3min
	{
		HideBuyWaitDlg();
		SetErrorDlg(g_pResourceManager->GetGlobalString(9000010));
		m_fBuyWaitAccmulateTime = 0.0f;
	}
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::ProcessSellHelp(float fTime)
{
	SPWindow* pkParentWindow;
	pkParentWindow = Find(WIID_CASHSHOP_OZ_SELL_HELP);
	if( pkParentWindow == NULL || !pkParentWindow->IsShow() )
		return;

	m_fSellHelpAccumulate += fTime;
	if( m_fSellHelpAccumulate > 5.0f )
	{
		m_iSellHelpAlpha = (255 - (int)((m_fSellHelpAccumulate - 5.0f) * 255.0f));
		if( m_iSellHelpAlpha <= 0 )
		{
			m_iSellHelpAlpha = 0;
			pkParentWindow->Hide();
		}
		else
			pkParentWindow->SetColor(RGBA(255, 255, 255, m_iSellHelpAlpha));
	}
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::ProcessWebbrowser(float fTime)
{
	if(g_pIngameWebBrowser == NULL)
		return;

	if( !m_bWebbrowerLoadingWait )
		return;

	if( g_pIngameWebBrowser->IsLoadOK() )
	{
		OnErrorOK(0, 0);
		m_bWebbrowerLoadingWait = false;
	}
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::Render(float fTime)
{
	if( !m_bShow )
		return;

	//[xialin 2008/07/29] add >>>
	g_pTerrainManager->Render(fTime);	
	if( m_pkPlayer->SPGOBPerformMessage(SPGM_GETSTANDLAYER) == BG_ID_LAYER_TERRAIN0 )
		m_pkPlayer->Render(fTime);

	g_pTerrainManager->RenderLayer2(fTime);
	if( m_pkPlayer->SPGOBPerformMessage(SPGM_GETSTANDLAYER) == BG_ID_LAYER_TERRAIN1 )
		m_pkPlayer->Render(fTime);

	g_pTerrainManager->RenderLayer3(fTime);	
	if( m_pkPlayer->SPGOBPerformMessage(SPGM_GETSTANDLAYER) != BG_ID_LAYER_TERRAIN0 
		&& m_pkPlayer->SPGOBPerformMessage(SPGM_GETSTANDLAYER) != BG_ID_LAYER_TERRAIN1 )
		m_pkPlayer->Render(fTime);

	g_pTerrainManager->RenderForeGround(fTime);
	g_pFXManager->Render(fTime);
	//[xialin 2008/07/29] add <<<

	//if( m_pkPet )
	//	m_pkPet->Render(fTime);

	RenderBackground();
	SPWindow::Render(fTime);
	
	if( m_pkEventTexture )
	{
		vector< EVENT_ANIMATION >::iterator iter = m_vEventAnimation.begin();
		while( iter != m_vEventAnimation.end() )
		{
			m_pkEventTexture->RenderTexture(&(*iter).rcDst, &(*iter).rcSrc[(*iter).iCurFrame]);
			++iter;
		}
	}

	if( m_pkSaleArrowTexture )
	{
		RECT rcSrc;
		SetRect(&rcSrc, 271, 89, 338, 94);
		vector< RECT >::iterator iter = m_vSaleArrow.begin();
		while( iter != m_vSaleArrow.end() )
		{
			m_pkSaleArrowTexture->RenderTexture(&(*iter), &rcSrc);
			++iter;
		}
	}

	if( m_pkSaleOrderTexture )
	{
		RECT rcSrc[3];
		SetRect(&rcSrc[0], 493, 443, 509, 459);
		SetRect(&rcSrc[1], 493, 460, 509, 476);
		SetRect(&rcSrc[2], 493, 477, 509, 493);
		vector< RECT >::iterator iter = m_vSaleOrderArrow.begin();
		int iCount = 0;
		while( iter != m_vSaleOrderArrow.end() )
		{
			m_pkSaleOrderTexture->RenderTexture(&(*iter), &rcSrc[iCount]);
			++iter;
			++iCount;

			if( iCount >= 3 )
				break;
		}
	}

	if( m_iPreSelectSlot >= 0 && m_pkPreSelectImage )
		m_pkPreSelectImage->RenderTexture(&m_PreSelectAni.rcDst, &m_rcPreSelect[m_PreSelectAni.iCurFrame]);

	//[xialin 2008/07/29] 因为遮挡关系 , 重绘 >>>
	for (int i=WIID_CASHSHOP_PLAYERVIEW_BACK1; i<=WIID_CASHSHOP_PLAYERVIEW_BACK3; i++)
	{
		SPWindowButton * pkButton = static_cast<SPWindowButton *>(Find(i, true));
		if ( pkButton && pkButton->IsChecked() )
			pkButton->Render(fTime);
	}

	for (int i=WIID_CASHSHOP_OZ_INVEN_BUYINFO; i<=WIID_CASHSHOP_OZ_INVEN_WEARINFO; i++)
	{
		SPWindowButton * pkButton = static_cast<SPWindowButton *>(Find(i, true));
		if ( pkButton && pkButton->IsChecked() )
			pkButton->Render(fTime);
	}

	for (int i=WIID_CASHSHOP_STORE_EVENT; i<=WIID_CASHSHOP_STORE_HELP; i++)
	{
		SPWindowButton * pkButton = static_cast<SPWindowButton *>(Find(i, true));
		if ( pkButton && pkButton->IsChecked() )
			pkButton->Render(fTime);
	}

	SPWindow * pkWindow = NULL;
	pkWindow = Find(WIID_CASHSHOP_STORE_SORT_PRICE, true);
	if( pkWindow )
		pkWindow->Render(fTime);
	
	pkWindow = Find(WIID_CASHSHOP_SULT);
	if( pkWindow )
		pkWindow->Render(fTime);
	//[xialin 2008/07/29] 因为遮挡关系 , 重绘 <<<
	
	if( m_bHelpTextDraw )
		RenderHelpInfo();
	
	RenderPopup();
	if( g_pInterfaceManager->GetModalWindow() )
	{
		if( m_pkBlackBackground )
		{
			m_pkBlackBackground->RenderTexture(
				&m_rcDstBlackBackground, &m_rcSrcBlackBackground);

			g_pVideo->Flush();
		}

		g_pInterfaceManager->GetModalWindow()->Render(fTime);
	}
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::RenderPopup()
{
	SPWindow* pkParent, *pkWindow;

	pkParent = Find(WIID_CASHSHOP_STORE, true);
	if( pkParent == NULL )
		return;
	
	int i = WIID_CASHSHOP_STORE_FAS_DRESS_POP;
	for( ; i <= WIID_CASHSHOP_STORE_ETC_COMMU_POP; i++ )
	{
		pkWindow = pkParent->Find(i, true);
		if( pkWindow == NULL )
			continue;

		if( pkWindow->IsShow() )
		{
			pkWindow->Render(0.0f);
			return;
		}
	}
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::RenderHelpInfo()
{
	vector< CASHSHOP_HELP_INFO >* pvHelpInfo = GetHelpInfo(m_iStoreCurPage + 1);
	if( pvHelpInfo == NULL )
		return;

	vector< CASHSHOP_HELP_INFO >::iterator iter = (*pvHelpInfo).begin();
	while( iter != (*pvHelpInfo).end() )
	{
		if( !(*iter).strText.empty() && (*iter).pFont )
		{
			(*iter).pFont->SetColor((*iter).Color);
			(*iter).pFont->SetShadow(false);
			(*iter).pFont->RenderText((*iter).strText.c_str(), &(*iter).rcTextDst);
			(*iter).pFont->SetShadow();
			(*iter).pFont->SetColor();
		}

		if( (*iter).pkTexture &&
			(*iter).rcImageSrc.left >= 0 && (*iter).rcImageSrc.top >= 0 &&
			(*iter).rcImageSrc.right >= 0 && (*iter).rcImageSrc.bottom >= 0 )
		{
			(*iter).pkTexture->RenderTexture(&(*iter).rcImageDst, &(*iter).rcImageSrc);
		}

		++iter;
	}
}

//------------------------------------------------------------------------------------
void SPWindowCashShop::RenderBackground()
{

	if( m_pkBackground == NULL )
		return;

	for( int i = 0; i < 5; i++ )
	{
		m_pkBackground->RenderTexture(
			&m_rcDstBackground[i], &m_rcSrcBackground[i]);
	}

	//int i, iWidth, iHeight;
	//RECT rcDst;
	//vector< BACKGROUND_ANIMAITON >::iterator iter;

	//iter = m_vBackgroundTextList.begin();
	//while( iter != m_vBackgroundTextList.end() )
	//{
	//	iWidth = (*iter).rcSrc.right - (*iter).rcSrc.left;
	//	iHeight = (*iter).rcSrc.bottom - (*iter).rcSrc.top;

	//	rcDst.left = (long)((*iter).vCurPos.m_fX);
	//	rcDst.top = (long)((*iter).vCurPos.m_fY);
	//	rcDst.right = rcDst.left + iWidth;
	//	rcDst.bottom = rcDst.top + iHeight;
	//	
	//	m_pkBackground->RenderTexture(&rcDst, &(*iter).rcSrc, (*iter).fRotate);
	//	++iter;
	//}

	//iter = m_vBackgroundStarList.begin();
	//while( iter != m_vBackgroundStarList.end() )
	//{
	//	iWidth = (*iter).rcSrc.right - (*iter).rcSrc.left;
	//	iHeight = (*iter).rcSrc.bottom - (*iter).rcSrc.top;

	//	rcDst.left = (long)((*iter).vCurPos.m_fX);
	//	rcDst.top = (long)((*iter).vCurPos.m_fY);
	//	rcDst.right = rcDst.left + iWidth;
	//	rcDst.bottom = rcDst.top + iHeight;

	//	m_pkBackground->RenderTexture(&rcDst, &(*iter).rcSrc, (*iter).fRotate);
	//	++iter;
	//}
}



//------------------------------------------------------------------------------------
// Message
int SPWindowCashShop::OnLButtonUpCheck(WPARAM wParam, LPARAM lParam)
{
	bool bHidePopup = true;
	int iID = wParam;

	SPWindow* pkWindow = NULL;
	switch( iID )
	{
	case WIID_CASHSHOP_STORE_FASSION_DRESS:
		pkWindow = Find(WIID_CASHSHOP_STORE_FAS_DRESS_POP, true);
		break;
	case WIID_CASHSHOP_STORE_FASSION_HELMET:
		pkWindow = Find(WIID_CASHSHOP_STORE_FAS_HELMET_POP, true);
		break;
	case WIID_CASHSHOP_STORE_FASSION_SHOES:
		pkWindow = Find(WIID_CASHSHOP_STORE_FAS_SHOES_POP, true);
		break;
	case WIID_CASHSHOP_STORE_FASSION_GLOVE:
		pkWindow = Find(WIID_CASHSHOP_STORE_FAS_GLOVE_POP, true);
		break;
	case WIID_CASHSHOP_STORE_FASSION_ACCE:
		pkWindow = Find(WIID_CASHSHOP_STORE_FAS_ACCE_POP, true);
		break;
	case WIID_CASHSHOP_STORE_ETC_COMMUNITY:
		pkWindow = Find(WIID_CASHSHOP_STORE_ETC_COMMU_POP, true);
		break;
	}
	
	if( pkWindow && pkWindow->IsShow() )
		bHidePopup = false;

	if( bHidePopup )
		HideStorePopupMenu(0);

	if( iID != WIID_CASHSHOP_OZ_INVEN_SLOT && iID != WIID_CASHSHOP_PREVIEW_SLOT &&
		iID != WIID_CASHSHOP_PREVIEW_BUY && iID != WIID_CASHSHOP_PREVIEW_DEL )
		SetSelectSlot(-1, false);

	SPWindowComboBox* pkCombo;
	if( iID < WIID_CASHSHOP_STORE_SORT_GENDER || iID > WIID_CASHSHOP_STORE_SORT_GENDER + 9 )
	{
		pkCombo = (SPWindowComboBox*)Find(WIID_CASHSHOP_STORE_SORT_GENDER, true);
		if( pkCombo )
		{
			pkCombo->SetListBoxRender(FALSE);
			pkCombo->SetEditBoxStatus(COMBOBOX_DATA_TEXTURE_ENABLE);
		}
	}

	if( iID < WIID_CASHSHOP_STORE_SORT_PRICE || iID > WIID_CASHSHOP_STORE_SORT_PRICE + 9 )
	{
		pkCombo = (SPWindowComboBox*)Find(WIID_CASHSHOP_STORE_SORT_PRICE, true);
		if( pkCombo )
		{
			pkCombo->SetListBoxRender(FALSE);
			pkCombo->SetEditBoxStatus(COMBOBOX_DATA_TEXTURE_ENABLE);
		}
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnExit(WPARAM wParam, LPARAM lParam)
{
	if( g_pSubGameManager )
		g_pSubGameManager->SPPerformMessage(SPGM_CASHSHOP_IN_REQUEST, 0, 0);

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnErrorOK(WPARAM wParam, LPARAM lParam)
{
	if( m_iErrorType == 1 )
	{
		OnExit(0, 0);
		return 1;
	}

	SPWindow* pkWindow = Find(WIID_CASHSHOP_ERROR);
	if( pkWindow )
		pkWindow->Hide();

	g_pInterfaceManager->SetModalWindow(NULL);
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnMyInfoFilling(WPARAM wParam, LPARAM lParam)
{
	char szBillURL[256];

	//TEST
	//wsprintf(szBillURL, "%s/login.ashx?userid=%s&token=%s",	"http://bill.latale.jp",
	//	"actoz0001", "azt7gciy");
	//g_pIngameWebBrowser->CreateNewInstance();
	//if( g_pIngameWebBrowser->Navigate(szBillURL) )
	//{
	//	SetErrorDlg(9009001, false);
	//	m_bWebbrowerLoadingWait = true;
	//}
	//else
	//	SetErrorDlg(9009002);

	//return 1;

	if(g_pIngameWebBrowser == NULL)
		return 1;
	
	SITE_TYPE eSiteType = SPLocalizeManager::GetInstance()->GetSiteType();

	//COUNTRY_TYPE eCountryType = SPLocalizeManager::GetInstance()->GetCountryType();

	switch(eSiteType) {
	case SITE_GAMEPOT: 
		{
			if( m_strURLToken.empty() )
			{
				SetErrorDlg(9009003);
				return 1;
			}

			// if JP then "http://bill.latale.jp/login.ashx?userid=test01&token=AbcdEFgH"
			wsprintf(szBillURL, "%s/login.ashx?userid=%s&token=%s",	SPLocalizeManager::GetInstance()->GetBillingPage(),
				g_pNetworkManager->GetLoginID(), m_strURLToken.c_str());

			g_pIngameWebBrowser->CreateNewInstance();
			if( g_pIngameWebBrowser->Navigate(szBillURL) )
			{
				SetErrorDlg(9009001, false);
				m_bWebbrowerLoadingWait = true;
			}
			else
				SetErrorDlg(9009002);
		}
		break;

	case SITE_ATGAMES:
		{
			if( m_strURLToken.empty() )
			{
				SetErrorDlg(9009003);
				return 1;
			}

			// if JP then "http://bill.latale.jp/login.ashx?userid=test01&token=AbcdEFgH"
			// http://bill.latale.jp/payment/atgames/index.aspx			
			wsprintf(szBillURL, "%s/payment/atgames/index.aspx?userid=%s&token=%s",	SPLocalizeManager::GetInstance()->GetBillingPage(),
				g_pNetworkManager->GetLoginID(), m_strURLToken.c_str());

			g_pIngameWebBrowser->CreateNewInstance();
			if( g_pIngameWebBrowser->Navigate(szBillURL) )
			{
				SetErrorDlg(9009001, false);
				m_bWebbrowerLoadingWait = true;
			}
			else
				SetErrorDlg(9009002);
		}
		break;

	case SITE_SANDA:
		{
			wsprintf(szBillURL, "%s",	SPLocalizeManager::GetInstance()->GetBillingPage());

			g_pIngameWebBrowser->CreateNewInstance();
			if( g_pIngameWebBrowser->Navigate(szBillURL) )
			{
				SetErrorDlg(9009001, false);
				m_bWebbrowerLoadingWait = true;
			}
			else
				SetErrorDlg(9009002);
		}
		break;

	default:
		{
			SHELLEXECUTEINFO sei;
			ZeroMemory(&sei, sizeof(SHELLEXECUTEINFO));
			sei.cbSize = sizeof(SHELLEXECUTEINFO);

			sei.lpFile = __TEXT("explorer");
			sei.nShow = SW_SHOWMAXIMIZED;
			sei.lpParameters = SPLocalizeManager::GetInstance()->GetBillingPage();
			if( strlen(sei.lpParameters) == 0 )
				return 0;

			//wsprintf(szBillURL, "%s", SPLocalizeManager::GetInstance()->GetBillingPage());

			ShellExecuteEx(&sei);
			ShowWindow(g_hWnd, SW_MINIMIZE);
		}
		break;

	};

//	if( eSiteType == SITE_GAMEPOT || eSiteType == SITE_ATGAMES ) 
//	{
//		if( m_strURLToken.empty() )
//		{
//			SetErrorDlg(9009003);
//			return 1;
//		}
//
//		//if(g_pIngameWebBrowser->IsPreloaded()) {
//		//	g_pIngameWebBrowser->ShowPreload();
//		//	return 1;
//		//}
//
//		// if JP then "http://bill.latale.jp/login.ashx?userid=test01&token=AbcdEFgH"
//		wsprintf(szBillURL, "%s/login.ashx?userid=%s&token=%s",	SPLocalizeManager::GetInstance()->GetBillingPage(),
//			g_pNetworkManager->GetLoginID(), m_strURLToken.c_str());
//		
//		g_pIngameWebBrowser->CreateNewInstance();
//		if( g_pIngameWebBrowser->Navigate(szBillURL) )
//		{
//			SetErrorDlg(9009001, false);
//			m_bWebbrowerLoadingWait = true;
//		}
//		else
//			SetErrorDlg(9009002);
//	}
////	else if( eSiteType == SITE_ACTOZ )
////	{
////		if( m_strURLToken.empty() )
////		{
////			SetErrorDlg(9009003);
////			return 1;
////		}
////
////		if(g_pIngameWebBrowser->IsPreloaded()) {
////			g_pIngameWebBrowser->ShowPreload();
////			return 1;
////		}
////
////		//wsprintf(szBillURL, "https://cash.actoz.com/GameCash/Login/Login.asp?GameServiceID=1&BKey=1234");
////
////		wsprintf(szBillURL, "%s/GameCash/Login/Login.asp?GameServiceID=1&BKey=%s",	
////			SPLocalizeManager::GetInstance()->GetBillingPage(),
////			//"https://cash.actoz.com",  // LDT郴侩 荐沥秦具窃
////			m_strURLToken.c_str());
////
////#ifdef _DEBUG
////		DebugPrint("%s\n",szBillURL);
////#endif
////
////		g_pIngameWebBrowser->CreateNewInstance();
////		if( g_pIngameWebBrowser->Navigate(szBillURL) )
////		{
////			SetErrorDlg(9009001, false);
////			m_bWebbrowerLoadingWait = true;
////		}
////		else
////			SetErrorDlg(9009002);
////	}
//	else
//	{
//		SHELLEXECUTEINFO sei;
//		ZeroMemory(&sei, sizeof(SHELLEXECUTEINFO));
//		sei.cbSize = sizeof(SHELLEXECUTEINFO);
//
//		sei.lpFile = __TEXT("explorer");
//		sei.nShow = SW_SHOWMAXIMIZED;
//		sei.lpParameters = SPLocalizeManager::GetInstance()->GetBillingPage();
//		if( strlen(sei.lpParameters) == 0 )
//			return 0;
//
//		//wsprintf(szBillURL, "%s", SPLocalizeManager::GetInstance()->GetBillingPage());
//
//		ShellExecuteEx(&sei);
//		ShowWindow(g_hWnd, SW_MINIMIZE);
//	}
//	
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnMyInfoUpdate(WPARAM wParam, LPARAM lParam)
{
	if( g_pNetworkManager == NULL || g_pNetworkManager->GetPacketHandler() == NULL )
		return 1;

	CPacket kPacket(CASHSHOP_CS_CASH);
	(g_pNetworkManager->GetPacketHandler())->PostMsg(&kPacket);

	g_pCheckManager->SetDBBlock(true);
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnMyInfoCoupon(WPARAM wParam, LPARAM lParam)
{
	ShowCouponDlg();
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnCouponOK(WPARAM wParam, LPARAM lParam)
{
	g_pInterfaceManager->SetModalWindow(NULL);

	SPWindow* pkParentWindow, *pkChildWindow;;
	pkParentWindow = Find(WIID_CASHSHOP_COUPON);
	if( pkParentWindow )
	{
		std::string strCouponNumber;
		for( int i = 0; i < 4; i++ )
		{
			pkChildWindow = pkParentWindow->Find(WIID_CASHSHOP_COUPON_NUMBER + i, true);
			if( pkChildWindow )
				strCouponNumber += pkChildWindow->GetWindowText();
		}

		if( strCouponNumber.size() != COUPON_NUM )
			SetErrorDlg(9002008);
		else
		{
			char szBuf[COUPON_NUM + 1];
			strcpy(szBuf, strCouponNumber.c_str());
			CPacket kPacket(CASHSHOP_CS_COUPON);
			kPacket.AddUINT8(COUPON_NUM + 1);
			kPacket.Add(szBuf, sizeof(char) * (COUPON_NUM + 1));

			(g_pNetworkManager->GetPacketHandler())->PostMsg(&kPacket);
			g_pCheckManager->SetDBBlock(true);
		}

		pkParentWindow->Hide();
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnCouponCancel(WPARAM wParam, LPARAM lParam)
{
	SPWindow* pkWindow = Find(WIID_CASHSHOP_COUPON);
	if( pkWindow )
		pkWindow->Hide();

	g_pInterfaceManager->SetModalWindow(NULL);
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnResultOK(WPARAM wParam, LPARAM lParam)
{
	SPWindow* pkWindow = Find(WIID_CASHSHOP_RESULT);
	if( pkWindow )
		pkWindow->Hide();

	g_pInterfaceManager->SetModalWindow(NULL);

	switch( m_eBuyType )
	{
	case TYPE_BUY:
		Buy();
		break;
	case TYPE_GIFT:
		Gift();
		break;
	case TYPE_EXACT:
		break;
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnClearDrag(WPARAM wParam, LPARAM lParam)
{
	ClearSelectItem();
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnReflashItemInven(WPARAM wParam, LPARAM lParam)
{
	RefreshItemInven();
	UpdateOZInven();
	SetSelectSlot(-1, false);
	return 1;
}



//------------------------------------------------------------------------------------
int SPWindowCashShop::OnInfo(WPARAM wParam, LPARAM lParam)
{
	g_pCheckManager->SetDBBlock(false);
	CPacket* pkPacket = reinterpret_cast< CPacket* >(lParam);
	if( pkPacket == NULL )
		return 1;

	GLOBAL_STRING_ID iError;
	pkPacket->ExtractUINT32(&iError);
	if( iError )
	{
		SetErrorDlg(iError);
		return 1;
	}

	ZeroMemory(m_szAccountID,sizeof(m_szAccountID));
	strcpy(m_szAccountID, g_pNetworkManager->GetLoginID());

	pkPacket->ExtractUINT64(&m_iGUID);
	pkPacket->ExtractUINT32(&m_uiCurOZ);
	pkPacket->ExtractUINT32(&m_uiCurMileage);
	UpdateMyInfo();

	unsigned short usItemCount, i;
	CONTAINER_ITEM Item;

	if( g_pGOBManager->GetLocalPlayer() == NULL )
		return 1;

	SPPlayerInvenArchive* pkInvenArchive = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	if( pkInvenArchive == NULL )
		return 1;

	ClearOZInven();
	pkInvenArchive->ClearInventory(INVENTORY_CASH);
	pkPacket->ExtractUINT16(&usItemCount);
	for( i = 0; i < usItemCount; i++ )
	{
		pkPacket->Extract(&Item, sizeof(CONTAINER_ITEM));
		Item.SlotInfo.SlotIndex -= 1;
		pkInvenArchive->ItemSetting(Item, false);
	}
	UpdateOZInven();

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnUpdate(WPARAM wParam, LPARAM lParam)
{
	g_pCheckManager->SetDBBlock(false);

	CPacket* pkPacket = reinterpret_cast< CPacket* >(lParam);
	if( pkPacket == NULL )
		return 1;

	GLOBAL_STRING_ID iError;
	pkPacket->ExtractUINT32(&iError);
	if( iError )
	{
		SetErrorDlg(iError);
		return 1;
	}

	pkPacket->ExtractUINT64(&m_iGUID);
	pkPacket->ExtractUINT32(&m_uiCurOZ);
	pkPacket->ExtractUINT32(&m_uiCurMileage);
	UpdateMyInfo();

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnBuy(WPARAM wParam, LPARAM lParam)
{
	g_pCheckManager->SetDBBlock(false);

	CPacket* pkPacket = reinterpret_cast< CPacket* >(lParam);
	if( pkPacket == NULL )
		return 1;

	GLOBAL_STRING_ID iError;
	pkPacket->ExtractUINT32(&iError);
	if( iError )
	{
		HideBuyWaitDlg();
		SetErrorDlg(iError);
		if( iError != 9001001 )
			return 1;
		m_vCashierItemAttr.clear();
	}

	unsigned int uiBuyItem;
	unsigned short usNum, i;
	CONTAINER_ITEM Item;

	pkPacket->ExtractUINT64(&m_iGUID);
	pkPacket->ExtractUINT32(&uiBuyItem);
	pkPacket->ExtractUINT32(&m_uiCurOZ);
	pkPacket->ExtractUINT32(&m_uiCurMileage);
	UpdateMyInfo();

	m_uiBuyItem = uiBuyItem;
	if( g_pGOBManager->GetLocalPlayer() == NULL )
		return 1;

	SPPlayerInvenArchive* pkInvenArchive = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	if( pkInvenArchive == NULL )
		return 1;

	int iSlotIndex = -1;
	pkPacket->ExtractUINT16(&usNum);
	for( i = 0; i < usNum; i++ )
	{
		pkPacket->Extract(&Item, sizeof(CONTAINER_ITEM));
		Item.SlotInfo.SlotIndex -= 1;
		pkInvenArchive->ItemSetting(Item, false);

		iSlotIndex = Item.SlotInfo.SlotIndex;
	}

	if( iSlotIndex >= 0 )
		m_iOZInvenCurPage[m_iOZInvenCategory] = iSlotIndex / 10;
	
	SetOZInvenPageText();
	UpdateOZInven();

	m_bBuyRecv = true;
	m_eBuyType = TYPE_BUY;
	
	if( m_bOnceAni )
	{
		ShowResultDlg(TYPE_BUY, uiBuyItem);
	}
	return 1;
}

//------------------------------------------------------------------------------------
bool SPWindowCashShop::Buy()
{
	if( g_pCheckManager->IsDBBlock() )
	{
		m_vCashierItemAttr.clear();
		return false;
	}

	if( g_pGOBManager->GetLocalPlayer() == NULL )
	{
		m_vCashierItemAttr.clear();
		return false;
	}

	SPPlayerInvenArchive* pkInvenArchive = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	if( pkInvenArchive == NULL )
	{
		m_vCashierItemAttr.clear();
		return false;
	}

	if( m_vCashierItemAttr.size() > 0 && m_vCashierItemAttr.back() != NULL )
	{
		if( !IsBuyRequireSuccess(m_vCashierItemAttr.back()) )
		{
			SetErrorDlg(9001008);
			m_vCashierItemAttr.clear();
			return false;
		}

		int iPrice = m_vCashierItemAttr.back()->iSalePrice ? m_vCashierItemAttr.back()->iSalePrice : m_vCashierItemAttr.back()->iPrice;

		bool bSendPack = false;
		int iEmptySize = pkInvenArchive->GetEmptySlotCount(INVENTORY_CASH);
		if( iEmptySize >= static_cast<int>(m_vCashierItemAttr.size()) )
		{
			int iCurOZAndMileage = 0;
			if ( m_iCashierOZ == 0 && m_iCashierMileage == 0 )
			{
				iCurOZAndMileage = m_uiCurOZ;
			}
			else
			{
				iCurOZAndMileage = m_uiCurOZ + m_uiCurMileage;
			}

			if( iCurOZAndMileage < iPrice )
			{
				OnMyInfoFilling(0, 0);
			}
			else if( m_vCashierItemAttr.back() )
			{			
				int iCashierPrice = 0, iCashierMileage = 0;
				if ( m_iCashierOZ == 0 && m_iCashierMileage == 0 )
				{
					iCashierPrice = iPrice;
					iCashierMileage = 0;
				}
				else
				{
					iCashierPrice = m_iCashierOZ;
					iCashierMileage = m_iCashierMileage;
				}

				if( SPCashTradeManager::GetInstance()->SendCashShopBuy(CASH_TRADE_PLACE_SHOP, 
					m_vCashierItemAttr.back()->iCashID, m_vCashierItemAttr.back()->iPrice, 
					m_vCashierItemAttr.back()->iSalePrice, m_vCashierItemAttr.back()->iMaxOZ, m_vCashierItemAttr.back()->iMaxMile, 
					iCashierPrice, iCashierMileage) == true )
				{
					m_vecBuyCashItemList.push_back(m_vCashierItemAttr.back());

					m_vCashierItemAttr.pop_back();
					m_bBuyRecv = false;
					bSendPack = true;
				}

				//CPacket kPacket(CASHSHOP_CS_BUY);

				//kPacket.AddUINT32(m_vCashierItemAttr.back()->iCashID);
				//kPacket.AddUINT32(m_vCashierItemAttr.back()->iPrice);
				//kPacket.AddUINT32(m_vCashierItemAttr.back()->iSalePrice);
				//kPacket.AddUINT32(m_vCashierItemAttr.back()->iMaxOZ);
				//kPacket.AddUINT32(m_vCashierItemAttr.back()->iMaxMile);

				//if ( m_iCashierOZ == 0 && m_iCashierMileage == 0 )
				//{
				//	kPacket.AddUINT32(iPrice);
				//	kPacket.AddUINT32(0);
				//}
				//else
				//{
				//	kPacket.AddUINT32(m_iCashierOZ);
				//	kPacket.AddUINT32(m_iCashierMileage);
				//}

				//(g_pNetworkManager->GetPacketHandler())->PostMsg(&kPacket);
				//g_pCheckManager->SetDBBlock(true);
				//m_vCashierItemAttr.pop_back();
				//m_bBuyRecv = false;
				//bSendPack = true;
			}
		}
		else
		{
			if( m_iMileageRatio )
				SetErrorDlg(9001003);
			else
				SetErrorDlg(9001006);
			m_vCashierItemAttr.clear();
		}

		if ( bSendPack )
		{
			ShowBuyWaitDlg();
			return true;
		}
	}

	m_vCashierItemAttr.clear();
	return false;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnGift(WPARAM wParam, LPARAM lParam)
{
	g_pCheckManager->SetDBBlock(false);
	m_bBuyRecv = true;
	m_eBuyType = TYPE_GIFT;

	CPacket* pkPacket = reinterpret_cast< CPacket* >(lParam);
	if( pkPacket == NULL )
		return 1;

	SPWindow* pkWindow = Find(WIID_CASHSHOP_BUYWAIT);

	GLOBAL_STRING_ID iError;
	pkPacket->ExtractUINT32(&iError);
	if( iError )
	{
		HideBuyWaitDlg();
		SetErrorDlg(iError);
		if( iError != 9001001 )
			return 1;
	}

	unsigned int uiBuyItem;
	char szTargetName[LEN_NAME+1];						// 酒捞祦E钱肺 谩阑版縼E\0捞 绝绰 巩力 荐沥 [2/28/2007 AJJIYA]
	memset(szTargetName,0,sizeof(szTargetName));

	pkPacket->ExtractUINT64(&m_iGUID);
	pkPacket->Extract(szTargetName, sizeof(char)*LEN_NAME);

	pkPacket->ExtractUINT32(&uiBuyItem);
	pkPacket->ExtractUINT32(&m_uiCurOZ);
	pkPacket->ExtractUINT32(&m_uiCurMileage);
	UpdateMyInfo();

	m_uiBuyItem = uiBuyItem;
	m_strTargetName = szTargetName;
	
	if( m_bOnceAni )
	{
		HideBuyWaitDlg();
		ShowResultDlg(TYPE_GIFT, uiBuyItem, szTargetName);
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnExact(WPARAM wParam, LPARAM lParam)
{
	g_pCheckManager->SetDBBlock(false);
	m_bBuyRecv = true;
	m_eBuyType = TYPE_EXACT;

	CPacket* pkPacket = reinterpret_cast< CPacket* >(lParam);
	if( pkPacket == NULL )
		return 1;

	GLOBAL_STRING_ID iError;
	pkPacket->ExtractUINT32(&iError);
	if( iError )
	{
		HideBuyWaitDlg();
		SetErrorDlg(iError);
		if( iError != 9001001 )
			return 1;
	}

	UINT8				nPackageNum;				// iPackageID的数量
	int					iPackageID[MAX_LEN_ID_DB];	// CASH_SHOP.LDT::ItemID
	UINT8				nNameLen;					// szSender的长度
	TCHAR				szSenderName[MAX_LEN_NAME];	// 发送者名字
	memset(iPackageID, 0x00, sizeof(iPackageID));
	memset(szSenderName, 0x00, sizeof(szSenderName));

	pkPacket->ExtractUINT8(&nPackageNum);
	pkPacket->Extract(iPackageID, nPackageNum);
	pkPacket->ExtractUINT8(&nNameLen);
	pkPacket->Extract(szSenderName, nNameLen);

	//if( m_bOnceAni )
	{
		HideBuyWaitDlg();
		
		char  szMsg[128] = "";
		sprintf(szMsg, "向好友%s发出了索取礼物消息!", szSenderName);
		g_pInterfaceManager->MessageBox(szMsg);
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnParcel(WPARAM wParam, LPARAM lParam)
{
	int					iSeqID;
	GLOBAL_STRING_ID	iErrorMsg;

	CPacket* pkPacket = reinterpret_cast< CPacket* >(lParam);
	if( pkPacket == NULL )
		return 1;

	pkPacket->ExtractUINT32((UINT32*)&iSeqID);
	pkPacket->ExtractUINT32(&iErrorMsg);
	if(iErrorMsg)
	{
		if(g_pResourceManager->GetGlobalString(iErrorMsg)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(iErrorMsg);
			SetErrorDlg( strMsg.c_str() );
		}
		return 1;
	}

	RefreshItemInven();
	g_pInterfaceManager->MessageBox("操作成功!");

}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnSell(WPARAM wParam, LPARAM lParam)
{
	g_pCheckManager->SetDBBlock(false);

	CPacket* pkPacket = reinterpret_cast< CPacket* >(lParam);
	if( pkPacket == NULL )
		return 1;

	GLOBAL_STRING_ID iError;
	pkPacket->ExtractUINT32(&iError);
	if( iError )
	{
		SetErrorDlg(iError);
		return 1;
	}

	unsigned int	uiCurMileage	=	0;
	TRASH_ITEM		TrashItem;
	UINT8			iSkipUpdate		=	0;

	pkPacket->ExtractUINT64(&m_iGUID);
	pkPacket->ExtractUINT32(&uiCurMileage);
	pkPacket->Extract(&TrashItem, sizeof(TRASH_ITEM));
	pkPacket->ExtractUINT8(&iSkipUpdate);

	// 儡绊甫诀单捞飘 且瘤咯何 0: 诀单捞飘 1: 诀单捞飘救窃
	if( iSkipUpdate == 0 )
		m_uiCurMileage	=	uiCurMileage;

	UpdateMyInfo();

	if( g_pGOBManager->GetLocalPlayer() == NULL )
		return 1;

	SPPlayerInvenArchive* pkInvenArchive = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	if( pkInvenArchive == NULL )
		return 1;

	TrashItem.iContainerSlot.SlotIndex -= 1;
	pkInvenArchive->TrashItem(TrashItem);
	UpdateOZInven();
	RefreshItemInven(); 
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnCoupon(WPARAM wParam, LPARAM lParam)
{
	g_pCheckManager->SetDBBlock(false);

	CPacket* pkPacket = reinterpret_cast< CPacket* >(lParam);
	if( pkPacket == NULL )
		return 1;

	GLOBAL_STRING_ID iError;
	pkPacket->ExtractUINT32(&iError);

	char szError[128];
	switch( iError )
	{
	case 9002003:	// iAddMilage甫 裙垫窍继嚼聪促.
		{
			unsigned int uiAddMilage = 0;
			unsigned int uiRemainMilage = 0;
			pkPacket->ExtractUINT32(&uiAddMilage);
			pkPacket->ExtractUINT32(&uiRemainMilage);
			wsprintf(szError, g_pResourceManager->GetGlobalString(iError), uiAddMilage);

			m_uiCurMileage = uiRemainMilage;
			UpdateMyInfo();
		}
		break;
	case 9002010:	// iOz甫 裙垫窍继嚼聪促.
		{
			unsigned int uiAddOz = 0;
			unsigned int uiRemainOz = 0;
			pkPacket->ExtractUINT32(&uiAddOz);
			pkPacket->ExtractUINT32(&uiRemainOz);
			wsprintf(szError, g_pResourceManager->GetGlobalString(iError), uiAddOz);

			m_uiCurOZ = uiRemainOz;
			UpdateMyInfo();
		}
		break;
	case 9002004:
	default:
		{
			wsprintf(szError, g_pResourceManager->GetGlobalString(iError));
		}
		break;
	}

	SetErrorDlg(szError);
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnCartList(WPARAM wParam, LPARAM lParam)
{
	g_pCheckManager->SetDBBlock(false);

	CPacket* pkPacket = reinterpret_cast< CPacket* >(lParam);
	if( pkPacket == NULL )
		return 1;

	unsigned int iNum, iID, i;
	pkPacket->ExtractUINT32(&iNum);
	for( i = 0; i < iNum; i++ )
	{
		pkPacket->ExtractUINT32(&iID);
		m_vCartList.push_back(iID);
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnCartAdd(WPARAM wParam, LPARAM lParam)
{
	g_pCheckManager->SetDBBlock(false);

	CPacket* pkPacket = reinterpret_cast< CPacket* >(lParam);
	if( pkPacket == NULL )
		return 1;

	if( (g_pSubGameManager == NULL) || (g_pSubGameManager->GetGameState() != GS_CASHSHOP) )
		return 1;

	GLOBAL_STRING_ID iError, iMessage;
	pkPacket->ExtractUINT32(&iError);
	iMessage = iError;

	if( iMessage == 0 )
		iMessage = 9005002;

	SetErrorDlg(iMessage);
	if( iError )
		return 1;

	unsigned int iID;
	pkPacket->ExtractUINT32(&iID);
	m_vCartList.push_back(iID);

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnCartDel(WPARAM wParam, LPARAM lParam)
{
	g_pCheckManager->SetDBBlock(false);

	CPacket* pkPacket = reinterpret_cast< CPacket* >(lParam);
	if( pkPacket == NULL )
		return 1;

	GLOBAL_STRING_ID iError;
	pkPacket->ExtractUINT32(&iError);
	if( iError )
	{
		SetErrorDlg(iError);
		return 1;
	}

	unsigned int iID;
	pkPacket->ExtractUINT32(&iID);

	vector< unsigned int >::iterator iter= m_vCartList.begin();
	while( iter != m_vCartList.end() )
	{
		if( (*iter) == iID )
		{
			m_vCartList.erase(iter);
			break;
		}
		++iter;
	}

	RefreshStore();
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnHidePopupMenu(WPARAM wParam, LPARAM lParam)
{
	HideStorePopupMenu(0);
	return 1;
}


//------------------------------------------------------------------------------------
int SPWindowCashShop::OnSetFillingURL(WPARAM wParam, LPARAM lParam)
{
	m_strURLToken = (char*)lParam ;
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnNeedItemUpdate(WPARAM wParam, LPARAM lParam)
{
	int iStringID = lParam;
	SetErrorDlg(iStringID);
	m_iErrorType = 1;
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnGetPlayer(WPARAM wParam, LPARAM lParam)
{
	memcpy((void *)wParam, &m_pkPlayer, sizeof(m_pkPlayer));
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnSwapTerrainMgr(WPARAM wParam, LPARAM lParam)
{
	if ( (bool)wParam == true )
	{
		g_pTerrainManager = m_gpMgrTerrain;
		SPPlayer * pkLocalPlayer = g_pGOBManager->GetLocalPlayer();
		if ( pkLocalPlayer )
		{
			int iStageID = pkLocalPlayer->GetCurStageID();
			int iMapGroupID = pkLocalPlayer->GetCurMapGroupID();

			pkLocalPlayer->SetStage(iStageID, iMapGroupID);
			g_StageManager.SetCurMapIndex(0, iMapGroupID, iStageID);
			pkLocalPlayer->GetCoordPhysics()->SetCoordPhysicsMap(iStageID, iMapGroupID);
			g_pTerrainManager->GetTerrainCluster()->SetShareLayer();
		}
	}
	else
	{
		static  bool  bLoadTerrain = true/*false*/;
		if ( !bLoadTerrain )
		{
			bLoadTerrain = true;
			for ( int i=0; i<3; i++ )
			{
				//
				int iStageID =0, iMapGroupID = 0;
				if ( i == 0 )
				{
					iStageID = 1;  iMapGroupID = 1;
				}
				else if ( i == 1 )
				{
					iStageID = 2;  iMapGroupID = 2;
				}
				else if ( i == 2 )
				{
					iStageID = 3;  iMapGroupID = 3;
				}

				m_pMgrTerrain[i]->LoadBG(g_StageManager.GetCurStage(iStageID));
				if( g_StageManager.GetCurStage(iStageID) != NULL )
				{
					if( !g_StageManager.GetCurStage(iStageID)->m_vpPalette.empty() )
					{
						std::string filePath = "DATA/BGFORMAT/";
						filePath += g_StageManager.GetCurStage(iStageID)->m_vpPalette[0];
						m_pMgrTerrain[i]->SPPerformMessage(SPTM_LOADPALLET, (WPARAM)filePath.c_str());
					}
				}

				SPStage* pStage = g_StageManager.GetCurStage(iStageID);
				if( !pStage )
					continue;
				m_pMgrTerrain[i]->LoadAttribute(pStage);

				SPMapGroup* pMapGroup = pStage->GetMapGroup(iMapGroupID);
				if( !pMapGroup )
					continue;
				m_pMgrTerrain[i]->SetLength(pMapGroup->m_lGroupWidth, pMapGroup->m_lGroupHeight);
				m_pMgrTerrain[i]->LoadForm(g_StageManager.GetCurStage(iStageID));
				m_pMgrTerrain[i]->GetTerrainCluster()->ResetStage();
			}
		}
	}
	return 1;
}
//------------------------------------------------------------------------------------

int SPWindowCashShop::OnUpdateCashSort( WPARAM wParam, LPARAM lParam )
{
	string unit = "点券";
	SPWindowStatic* pkStatic;
	for ( int i = 0; i < 5; i++ )
	{
		SPHotItemManager* pHotManager = SPCashTradeManager::GetInstance()->GetHotItemManager();
		if(pHotManager == NULL)
			return 0;

		if ( i >= pHotManager->GetHotItemSize() )
			return 0;

		SPCashItemAttr* hotItemAttr = pHotManager->GetHotItem( i )->GetCashItemAttr();

		if ( NULL == hotItemAttr ) 
		{
			continue;
		}
		POINT  pt;
		SPTexture* pkTexture = g_pItemCluster->GetItemTextureFromTextureInx(
			hotItemAttr->iIcon, hotItemAttr->iIconIndex, pt);
		if( NULL == pkTexture )
		{
			continue;
		}
		pkStatic =  static_cast<SPWindowStatic *>( Find( WIID_CASHSHOP_SORT_ICON + i , true  ) );
		if( NULL == pkStatic )
		{
			continue;
		}

		if( pkTexture )
		{
			pkStatic->SetWindowType(WND_TYPE_RECT);
			pkStatic->SetImageRect(pkTexture, pt.x, pt.y, pt.x+32, pt.y+32);
			pkTexture->IncreaseRef();
		}
		
		pkStatic =  static_cast<SPWindowStatic *>( Find( WIID_CASHSHOP_SORT_NAME + i , true  ) );
		if( NULL == pkStatic )
		{
			continue;
		}

		USES_CONVERSION;

		string strName = hotItemAttr->strName;
		wstring  wstr = A2W(strName.c_str());
		if ( strName.length() > 10 )
		{
			wstr = wstr.substr(0, 5);
			wstr += L"..";
		}
		strName = W2A(wstr.c_str());
		pkStatic->SetWindowText( strName.c_str() );

		pkStatic =  static_cast<SPWindowStatic *>( Find( WIID_CASHSHOP_SORT_PRICE + i , true  ) );
		if( NULL == pkStatic )
		{
			continue;
		}
		int iPrice = hotItemAttr->iSalePrice ? hotItemAttr->iSalePrice : hotItemAttr->iPrice;
		string price = ConvertMoneyToString( iPrice );
		pkStatic->SetWindowText( ( price + unit ).c_str() );
	}
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowCashShop::OnCheckCart(WPARAM wParam, LPARAM lParam)
{
	int iCheckShopID = static_cast<int>(wParam);

	//(int&)lParam = 0;
	*((int*)lParam) = 0;

	if((int)m_vCartList.size() >= CASHSHOP_MAX_WISH) {
		*((int*)lParam) = 9005003;
		return 1;
	}	

	vector< unsigned int >::iterator iter = m_vCartList.begin();
	while(iter != m_vCartList.end()) {
		if((*iter) == iCheckShopID) {
			*((int*)lParam) = 9005001;
			return 1;
		}
		++iter;
	}
	
	return 1;
}


bool SPWindowCashShop::IsBuyCashItemList(SPCashItemAttr * pCashItemAttr)
{
	std::vector<SPCashItemAttr*>::iterator  it = find(m_vecBuyCashItemList.begin(),  
		m_vecBuyCashItemList.end(), pCashItemAttr);
	if ( it != m_vecBuyCashItemList.end() )
	{
		return true;
	}

	return false;
}

//------------------------------------------------------------------------------------
SPIMESSAGE_OBJ_MAP_BEGIN(SPWindowCashShop)

SPIMESSAGE_CHECK( SPIM_LBUTTON_UP,		OnLButtonUpCheck)

SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_EXIT,		OnExit )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_ERROR_OK,	OnErrorOK )

SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_INFO_FILLING,		OnMyInfoFilling )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_INFO_UPDATE,		OnMyInfoUpdate )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_INFO_COUPON,		OnMyInfoCoupon )

SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_ITEM_INVEN_UP,	OnItemInvenUp )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_ITEM_INVEN_DOWN,	OnItemInvenDown )
SPIMESSAGE_CONTROL( SPIM_SLIDER_UP,			WIID_CASHSHOP_ITEM_INVEN_THUMB,	OnItemInvenUp )
SPIMESSAGE_CONTROL( SPIM_SLIDER_DOWN,		WIID_CASHSHOP_ITEM_INVEN_THUMB,	OnItemInvenDown )
SPIMESSAGE_CONTROL( SPIM_SLIDER_PAGEUP,		WIID_CASHSHOP_ITEM_INVEN_THUMB,	OnItemInvenUp )
SPIMESSAGE_CONTROL( SPIM_SLIDER_PAGEDOWN,	WIID_CASHSHOP_ITEM_INVEN_THUMB,	OnItemInvenDown )
SPIMESSAGE_CONTROL( SPIM_SET_CURVALUE,		WIID_CASHSHOP_ITEM_INVEN_THUMB,	OnSetItemInvenCurPage )
SPIMESSAGE_CONTROL( SPIM_WHEEL_UP,			WIID_CASHSHOP_ITEM_INVEN_SLOT,	OnSetItemInvenUp )
SPIMESSAGE_CONTROL( SPIM_WHEEL_DOWN,		WIID_CASHSHOP_ITEM_INVEN_SLOT,	OnSetItemInvenDown )
SPIMESSAGE_CONTROL(	SPIM_CURSOR_MOVE,		WIID_CASHSHOP_ITEM_INVEN_SLOT, 	OnItemInvenCursorMove )
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_DOWN,		WIID_CASHSHOP_ITEM_INVEN_SLOT, 	OnItemInvenLButtonDown )
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_CASHSHOP_ITEM_INVEN_SLOT, 	OnItemInvenLButtonUp )
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_CASHSHOP_ITEM_INVEN_ADDEQUIP, 		OnAddParcel )
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_CASHSHOP_ITEM_INVEN_ADDCONSUME, 	OnAddConsume )
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_CASHSHOP_ITEM_INVEN_ADDETC, 	OnAddETC )
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_CASHSHOP_ITEM_INVEN_ADDEVENT, 	OnAddEvent )
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_CASHSHOP_ITEM_INVEN_ADDPET, 	OnAddPET )
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_CASHSHOP_ITEM_INVEN_ADDWH, 	OnAddWH )

//SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_CASHSHOP_OZ_INVEN_LEFT, 		OnOZInvenLeftPage )
//SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_CASHSHOP_OZ_INVEN_RIGHT, 		OnOZInvenRightPage )
SPIMESSAGE_CONTROL(	SPIM_CURSOR_MOVE,		WIID_CASHSHOP_OZ_INVEN_SLOT, 		OnOZInvenCursorMove )
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_DOWN,		WIID_CASHSHOP_OZ_INVEN_SLOT, 		OnOZInvenLButtonDown )
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_CASHSHOP_OZ_INVEN_SLOT, 		OnOZInvenLButtonUp )
SPIMESSAGE_CONTROL(	SPIM_CURSOR_ENTER,		WIID_CASHSHOP_OZ_SELL, 				OnOZSellCursorEnter )
SPIMESSAGE_CONTROL(	SPIM_CURSOR_OUT,		WIID_CASHSHOP_OZ_SELL, 				OnOZSellCursorOut )
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_CASHSHOP_OZ_SELL, 				OnOZSellLButtonUp )
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_CASHSHOP_OZ_SELL_HELP, 		OnOZSellHelpLButtonUp )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_OZ_INVEN_LEFT,	OnOZInvenUp )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_OZ_INVEN_RIGHT,	OnOZInvenDown )
SPIMESSAGE_CONTROL( SPIM_SLIDER_UP,			WIID_CASHSHOP_OZ_INVEN_THUMB,	OnOZInvenUp )
SPIMESSAGE_CONTROL( SPIM_SLIDER_DOWN,		WIID_CASHSHOP_OZ_INVEN_THUMB,	OnOZInvenDown )
SPIMESSAGE_CONTROL( SPIM_SLIDER_PAGEUP,		WIID_CASHSHOP_OZ_INVEN_THUMB,	OnOZInvenUp )
SPIMESSAGE_CONTROL( SPIM_SLIDER_PAGEDOWN,	WIID_CASHSHOP_OZ_INVEN_THUMB,	OnOZInvenDown )
SPIMESSAGE_CONTROL( SPIM_SET_CURVALUE,		WIID_CASHSHOP_OZ_INVEN_THUMB,	OnSetOZInvenCurPage )

SPIMESSAGE_CONTROL( SPIM_PAGE,					WIID_CASHSHOP_STORE_PAGE,			OnStorePage )
SPIMESSAGE_CONTROL( SPIM_COMBOBOX_CHANGE_VALUE, WIID_CASHSHOP_STORE_SORT_GENDER,	OnChangeSortGender)
SPIMESSAGE_CONTROL( SPIM_COMBOBOX_CHANGE_VALUE, WIID_CASHSHOP_STORE_SORT_PRICE,		OnChangeSortPrice)

SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_CASHIER_PLUS,			OnCashierPlus )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_CASHIER_MINUS,		OnCashierMinus )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_CASHIER_OK,			OnCashierOK )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_CASHIER_CANCEL,		OnCashierCancel )
SPIMESSAGE_CONTROL( SPIM_KILLFOCUS,			WIID_CASHSHOP_CASHIER_USE_MILE,		OnCashierMileageChange )
SPIMESSAGE_CONTROL( SPIM_KEY_RETURN,		WIID_CASHSHOP_CASHIER_USE_MILE,		OnCashierMileageChange )

SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_GIFT_PLUS,			OnGiftPlus )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_GIFT_MINUS,			OnGiftMinus )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_GIFT_OK,				OnGiftOK )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_GIFT_CANCEL,			OnGiftCancel )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_GIFT_FRIEND_BTN,		OnGiftFriend )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_GIFT_FRIEND_LIST,		OnGiftFriendListLButton )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_DBLCLICK,	WIID_CASHSHOP_GIFT_FRIEND_LIST,		OnGiftFriendListDBLButton )
SPIMESSAGE_CONTROL( SPIM_KILLFOCUS,			WIID_CASHSHOP_GIFT_USE_MILE,		OnGiftMileageChange )
SPIMESSAGE_CONTROL( SPIM_KEY_RETURN,		WIID_CASHSHOP_GIFT_USE_MILE,		OnGiftMileageChange )

SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_SELL_OK,				OnSellOK )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_SELL_CANCEL,			OnSellCancel )

SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_COUPON_OK,			OnCouponOK )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_COUPON_CANCEL,		OnCouponCancel )

SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_RESULT_OK,			OnResultOK )

SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_CASHSHOP_PREVIEW_LEFT, 		OnPreViewInvenLeftPage )
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_CASHSHOP_PREVIEW_RIGHT, 		OnPreViewInvenRightPage )
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_CASHSHOP_PREVIEW_SLOT, 		OnPreViewInvenLButtonDown )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_PREVIEW_DEL,			OnSetPreViewInvenDel )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_PREVIEW_INIT,			OnSetPreViewInvenInit )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_PREVIEW_BUY,			OnSetPreViewInvenBuy )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_CHAR_NORMAL,			OnSetPreViewInvenNormal )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_CHAR_RUN,				OnSetPreViewInvenRun )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_CHAR_LADDER,			OnSetPreViewInvenLadder )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_CHAR_INIT,			OnSetPreViewInvenCharInit )

SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_PLAYERVIEW_INIT,		OnSetPreViewInvenCharInit )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_PLAYERVIEW_BUY,		OnSetPlayerViewBuy )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_PLAYERVIEW_EXACT,		OnSetPlayerViewExact )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_PLAYERVIEW_WEARINFO,	OnSetPlayerViewWearInfo )

SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_EXACT_OK,				OnExactOK )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_EXACT_CANCEL,			OnExactCancel )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_EXACT_FRIEND_BTN,		OnExactFriend )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_EXACT_FRIEND_LIST,	OnExactFriendListLButton )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_DBLCLICK,	WIID_CASHSHOP_EXACT_FRIEND_LIST,	OnExactFriendListDBLButton )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_EXACTLIST_FRIEND_BTN,		OnExactListFriend )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_EXACTLIST_FRIEND_LIST,	OnExactListFriendListLButton )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_DBLCLICK,	WIID_CASHSHOP_EXACTLIST_FRIEND_LIST,	OnExactListFriendListDBLButton )

SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_CASHSHOP_STORE_SEARCH_SEARCH,	OnSearch )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_BUYLIST_OK,			OnBuyListOK)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_BUYLIST_CANCEL,		OnBuyListCancel)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_WEARLIST_OK,			OnWearListOK)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_EXACTLIST_OK,			OnExactListOK)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_EXACTLIST_CANCEL,		OnExactListCancel)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_BUYLIST_UP,			OnBuyListUp)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_BUYLIST_DOWN, 		OnBuyListDown)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_WEARLIST_UP,			OnWearListUp)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_WEARLIST_DOWN,		OnWearListDown)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_EXACTLIST_UP,			OnExactListUp)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_CASHSHOP_EXACTLIST_DOWN,		OnExactListDown)

SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_CASHSHOP_SULT_ITEM_EXIT,	    OnExitSultView )

SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_ENTER,		WIID_CASHSHOP_ITEM_INVEN_SLOT,		OnItemInvenToolTipEnter	)
SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_OUT,		WIID_CASHSHOP_ITEM_INVEN_SLOT,		OnItemInvenToolTipOut	)
SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_MOVE,		WIID_CASHSHOP_ITEM_INVEN_SLOT,		OnItemInvenToolTipMove	)
SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_ENTER,		WIID_CASHSHOP_OZ_INVEN_SLOT,		OnOZInvenToolTipEnter	)
SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_OUT,		WIID_CASHSHOP_OZ_INVEN_SLOT,		OnOZInvenToolTipOut	)
SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_MOVE,		WIID_CASHSHOP_OZ_INVEN_SLOT,		OnOZInvenToolTipMove	)
SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_ENTER,		WIID_CASHSHOP_PREVIEW_SLOT,			OnPreViewToolTipEnter	)
SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_OUT,		WIID_CASHSHOP_PREVIEW_SLOT,			OnPreViewToolTipOut	)
SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_MOVE,		WIID_CASHSHOP_PREVIEW_SLOT,			OnPreViewToolTipMove	)
SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_ENTER,		WIID_CASHSHOP_CASHIER_ITEM,			OnCashierToolTipEnter	)
SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_OUT,		WIID_CASHSHOP_CASHIER_ITEM,			OnCashierToolTipOut	)
SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_MOVE,		WIID_CASHSHOP_CASHIER_ITEM,			OnCashierToolTipMove	)
SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_ENTER,		WIID_CASHSHOP_GIFT_ITEM,			OnCashierToolTipEnter	)
SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_OUT,		WIID_CASHSHOP_GIFT_ITEM,			OnCashierToolTipOut	)
SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_MOVE,		WIID_CASHSHOP_GIFT_ITEM,			OnCashierToolTipMove	)
SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_ENTER,		WIID_CASHSHOP_ITEM_INVEN_SLOT,		OnItemInvenToolTipEnter	)
SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_OUT,		WIID_CASHSHOP_ITEM_INVEN_SLOT,		OnItemInvenToolTipOut	)
SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_MOVE,		WIID_CASHSHOP_ITEM_INVEN_SLOT,		OnItemInvenToolTipMove	)
SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_ENTER,		WIID_CASHSHOP_SELL_ITEM,			OnSellToolTipEnter	)
SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_OUT,		WIID_CASHSHOP_SELL_ITEM,			OnSellToolTipOut	)
SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_MOVE,		WIID_CASHSHOP_SELL_ITEM,			OnSellToolTipMove	)
SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_ENTER,		WIID_CASHSHOP_RESULT_ITEM,			OnResultToolTipEnter	)
SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_OUT,		WIID_CASHSHOP_RESULT_ITEM,			OnResultToolTipOut	)
SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_MOVE,		WIID_CASHSHOP_RESULT_ITEM,			OnResultToolTipMove	)
SPIMESSAGE_CONTROL_RANGE(SPIM_TOOLTIP_ENTER,	WIID_CASHSHOP_STORE_ITEM_IMAGE, WIID_CASHSHOP_STORE_ITEM_IMAGE + 9,	OnStoreToolTipEnter	)
SPIMESSAGE_CONTROL_RANGE(SPIM_TOOLTIP_OUT,		WIID_CASHSHOP_STORE_ITEM_IMAGE, WIID_CASHSHOP_STORE_ITEM_IMAGE + 9,	OnStoreToolTipOut	)
SPIMESSAGE_CONTROL_RANGE(SPIM_TOOLTIP_MOVE,		WIID_CASHSHOP_STORE_ITEM_IMAGE, WIID_CASHSHOP_STORE_ITEM_IMAGE + 9,	OnStoreToolTipMove	)
SPIMESSAGE_CONTROL_RANGE(SPIM_TOOLTIP_ENTER,	WIID_CASHSHOP_SULT_ITEM_IMAGE,	WIID_CASHSHOP_SULT_ITEM_IMAGE + 5,	OnStoreToolTipEnter)
SPIMESSAGE_CONTROL_RANGE(SPIM_TOOLTIP_OUT,		WIID_CASHSHOP_SULT_ITEM_IMAGE,	WIID_CASHSHOP_SULT_ITEM_IMAGE + 5,	OnStoreToolTipOut)
SPIMESSAGE_CONTROL_RANGE(SPIM_TOOLTIP_MOVE,		WIID_CASHSHOP_SULT_ITEM_IMAGE,	WIID_CASHSHOP_SULT_ITEM_IMAGE + 5,	OnStoreToolTipMove)
SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP,		WIID_CASHSHOP_OZ_INVEN_BUYINFO,	WIID_CASHSHOP_OZ_INVEN_WEARINFO,	OnOZInvenCategory)

SPIMESSAGE_CONTROL_RANGE(SPIM_TOOLTIP_ENTER,	WIID_CASHSHOP_SORT_ICON, WIID_CASHSHOP_SORT_ICON + 4,	OnCashSortToolTipEnter	)
SPIMESSAGE_CONTROL_RANGE(SPIM_TOOLTIP_OUT,		WIID_CASHSHOP_SORT_ICON, WIID_CASHSHOP_SORT_ICON + 4,	OnCashSortToolTipOut	)
SPIMESSAGE_CONTROL_RANGE(SPIM_TOOLTIP_MOVE,		WIID_CASHSHOP_SORT_ICON, WIID_CASHSHOP_SORT_ICON + 4,	OnCashSortToolTipMove	)

SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP,		WIID_CASHSHOP_ITEM_INVEN_EQUIP,			WIID_CASHSHOP_ITEM_INVEN_HOUSING,		OnInvenCategory)
SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP,		WIID_CASHSHOP_STORE_EVENT,				WIID_CASHSHOP_STORE_POINT,				OnStoreTabType)
SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP,		WIID_CASHSHOP_STORE_EVENT_NEW,			WIID_CASHSHOP_STORE_POINT_FUNCTIONAL,	OnStoreCategory1)
SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP,		WIID_CASHSHOP_STORE_FAS_DRESS_TOTAL,	WIID_CASHSHOP_STORE_ETC_COMMU_CHAT,		OnStoreCategory2)
SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP,		WIID_CASHSHOP_SULT_ITEM_SLOT_IMAGE,		WIID_CASHSHOP_SULT_ITEM_IMAGE + 5,		OnStoreLButtonDown)
SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP,		WIID_CASHSHOP_STORE_ITEM_SLOT_IMAGE,	WIID_CASHSHOP_STORE_ITEM_IMAGE + 9,		OnStoreLButtonDown)
SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_DBLCLICK,WIID_CASHSHOP_STORE_ITEM_SLOT_IMAGE,	WIID_CASHSHOP_STORE_ITEM_IMAGE + 9,		OnStoreLButtonDBL)
SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP,		WIID_CASHSHOP_STORE_ITEM_BUY,			WIID_CASHSHOP_STORE_ITEM_BUY + 9,		OnStoreBuy)
SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP,		WIID_CASHSHOP_STORE_ITEM_GIFT,			WIID_CASHSHOP_STORE_ITEM_GIFT + 9,		OnStoreGift)
SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP,		WIID_CASHSHOP_STORE_ITEM_EXACT,			WIID_CASHSHOP_STORE_ITEM_EXACT + 9,		OnStoreExact)
SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP,		WIID_CASHSHOP_STORE_ITEM_SKIN_W,	    WIID_CASHSHOP_STORE_ITEM_SKIN_W + 9,    OnStoreSultItem )
//SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP,		WIID_CASHSHOP_STORE_ITEM_CART,			WIID_CASHSHOP_STORE_ITEM_CART + 7,		OnStoreCart)
//SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP,		WIID_CASHSHOP_STORE_ITEM_CART_DEL,		WIID_CASHSHOP_STORE_ITEM_CART_DEL + 9,	OnStoreCartDel)
SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP,		WIID_CASHSHOP_SULT_ITEM_BUY,			WIID_CASHSHOP_SULT_ITEM_BUY + 5,		OnStoreBuy)
SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP,		WIID_CASHSHOP_SULT_ITEM_GIFT,			WIID_CASHSHOP_SULT_ITEM_GIFT + 5,		OnStoreGift)
SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP,		WIID_CASHSHOP_SULT_ITEM_EXACT,			WIID_CASHSHOP_SULT_ITEM_EXACT + 5,		OnStoreExact)
SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP,		WIID_CASHSHOP_BUYLIST_ITEM_CHECK,		WIID_CASHSHOP_BUYLIST_ITEM_CHECK + 5,		OnBuyListCheck)
SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP,		WIID_CASHSHOP_EXACTLIST_ITEM_CHECK,		WIID_CASHSHOP_EXACTLIST_ITEM_CHECK + 3,		OnExactListCheck)
SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP,		WIID_CASHSHOP_PLAYERVIEW_BACK1,			WIID_CASHSHOP_PLAYERVIEW_BACK3,				OnChangePlayerViewBack)
SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_DBLCLICK,WIID_CASHSHOP_SORT_ICON,				WIID_CASHSHOP_SORT_ICON + 4,				OnCashSortBuy )

SPIMESSAGE_COMMAND( SPIM_CASHSHOP_CLEAR_DRAG,			OnClearDrag )
SPIMESSAGE_COMMAND( SPIM_CASHSHOP_REFRESH_ITEMINVEN,	OnReflashItemInven )
SPIMESSAGE_COMMAND( SPIM_CASHSHOP_INFO,					OnInfo )
SPIMESSAGE_COMMAND( SPIM_CASHSHOP_UPDATE,				OnUpdate )
SPIMESSAGE_COMMAND( SPIM_CASHSHOP_BUY,					OnBuy )
SPIMESSAGE_COMMAND( SPIM_CASHSHOP_GIFT,					OnGift )
SPIMESSAGE_COMMAND( SPIM_CASHSHOP_EXACT,				OnExact )
SPIMESSAGE_COMMAND( SPIM_CASHSHOP_SELL,					OnSell )
SPIMESSAGE_COMMAND( SPIM_CASHSHOP_COUPON,				OnCoupon )
SPIMESSAGE_COMMAND( SPIM_CASHSHOP_CARTLIST,				OnCartList )
SPIMESSAGE_COMMAND( SPIM_CASHSHOP_CARTADD,				OnCartAdd )
SPIMESSAGE_COMMAND( SPIM_CASHSHOP_CARTDEL,				OnCartDel )
SPIMESSAGE_COMMAND( SPIM_CASHSHOP_HIDE_POPUP_MENU,		OnHidePopupMenu )
SPIMESSAGE_COMMAND( SPIM_CASHSHOP_SET_URLTOKEN,			OnSetFillingURL )
SPIMESSAGE_COMMAND( SPIM_CASHSHOP_NEED_ITEM_UPDATE,		OnNeedItemUpdate )
SPIMESSAGE_COMMAND(	SPIM_CASHSHOP_MSG_ADDPARCEL, 		OnAddParcel2 )
SPIMESSAGE_COMMAND(	SPIM_CASHSHOP_MSG_ADDCONSUME, 		OnAddConsume2 )
SPIMESSAGE_COMMAND(	SPIM_CASHSHOP_MSG_ADDETC, 			OnAddETC2 )
SPIMESSAGE_COMMAND(	SPIM_CASHSHOP_MSG_ADDEVENT, 		OnAddEvent2 )
SPIMESSAGE_COMMAND(	SPIM_CASHSHOP_MSG_ADDPET, 			OnAddPET2 )
SPIMESSAGE_COMMAND(	SPIM_CASHSHOP_MSG_ADDWH, 			OnAddWH2 )
SPIMESSAGE_COMMAND( SPIM_CASHSHOP_PARCEL,				OnParcel )
SPIMESSAGE_COMMAND( SPIM_CASHSHOP_GETPLAYER,            OnGetPlayer )
SPIMESSAGE_COMMAND( SPIM_CASHSHOP_SWAPTERMGR,           OnSwapTerrainMgr )
SPIMESSAGE_COMMAND( SPIM_CASHSHOP_CASHSORT,				OnUpdateCashSort )

SPIMESSAGE_COMMAND( SPIM_CASHSHOP_WEB_PAGE,				OnMyInfoFilling )
SPIMESSAGE_COMMAND( SPIM_CASHSHOP_CART_CHECK,			OnCheckCart )


SPIMESSAGE_OBJ_MAP_END(SPWindow)
