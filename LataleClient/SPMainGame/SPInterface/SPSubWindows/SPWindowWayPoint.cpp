#include "SPCommon.h"
#include "SPUtil.h"

#include "SPMainGameDEF.H"
#include "SPGameObjectDEF.h"

#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPSubGameManager.h"
#include "SPGameObject.h"
#include "SPManager.h"
#include "SPMouseCursor.h"
#include "SPIMEPool.h"
#include "SPWindowDEF.h"
#include "SPInterfaceManager.h"

#include "SPWindow.h"
#include "SPWindowButton.h"
#include "SPWindowStatic.h"

#include "SPTreeBoxDEF.h"
#include "SPTreeBoxItemBase.h"
#include "SPTreeBoxItemQuest.h"
#include "SPWindowTreeBox.h"

//
#include "SPCommandConvert.h"
#include "SPMainGameDEF.H"
#include "SPSubGameManager.h"
#include "SPMainGameManager.h"
#include "SPMouseCursor.h"
#include "SPWindowDEF.h"
#include "SPWindow.h"
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
#include "SPEventStatusModel.h"
#include "SPPlayerEquipInfo.h"
#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPPlayer.h"
#include "SPGOBManager.h"

#include "SPRenderModel.h"

#include "SPFXDEF.h"
#include "SPFXModel.h"
#include "SPFXUnit.h"
#include "SPFXGroupUnit.h"
#include "SPFXArchive.h"

//#include "SPStage.h"
//#include "SPStageManager.h"
//#include "SPStageLoader.h"

#include "SPEventDEF.h"
#include "SPWayPointAttr.h"
#include "SPWayPointArchive.h"
#include "SPWayPointManager.h"
#include "SPEventManager.h"

#include "SPCheckManager.h"

//
//#include "SPMainGameDEF.H"
//#include "SPSubGameManager.h"
#include "SPMainGameManager.h"
//#include "SPMouseCursor.h"
//#include "SPWindowDEF.h"
//#include "SPInterfaceManager.h"

#include "SPWindowWayPoint.h"
#include "SPUtil.h"

#include "SPDebug.h"


SPWindowWayPoint::SPWindowWayPoint(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_WAYPOINT, InstanceID, iX, iY, iCX, iCY, pParent)
{	
	Init();
}


SPWindowWayPoint::~SPWindowWayPoint()
{	
	Clean();
}


void SPWindowWayPoint::Init()
{
	m_cStringColor		=	D3DCOLOR_ARGB( 255 , 49 , 76 , 97 );	
	m_bClickNotify		=	false;

	m_strNotSave.clear();
	if(g_pResourceManager->GetGlobalString(2100000))
		m_strNotSave = g_pResourceManager->GetGlobalString(2100000);

	m_strNotSaveInfo.clear();
	if(g_pResourceManager->GetGlobalString(2200000))
		m_strNotSaveInfo = g_pResourceManager->GetGlobalString(2200000);

	m_bMoveRequest = false;
	m_fDeley = 5.0f;
	m_fAccumulate = 0.0f;
	
	InitSubWindow();
	
}


void SPWindowWayPoint::Clean()
{
	m_vGroupItem.clear();

	SPWindow::Clean();
}


void SPWindowWayPoint::InitSubWindow()
{
	SPWindow* pStatic = NULL;
	SPWindow* pCenter = NULL;
	SPWindow* pParent = NULL;
	SPWindowButton* pButton = NULL;

	pStatic = new SPWindowStatic(WIID_WAYPOINT_TOP, 0, 0, 247, 4, this);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI100.PNG", 125,	354);
	pStatic->Show();
	pStatic = NULL;

	pCenter = new SPWindowStatic(WIID_WAYPOINT_CENTER, 0, 4, 247, 309, this);
	pCenter->SetSrcSize(247, 2);
	pCenter->SetImage("DATA/INTERFACE/CONCEPT/UI100.PNG", 125,	360);	
	pCenter->Show();

	pStatic = new SPWindowStatic(WIID_WAYPOINT_BOTTOM, 0, 313, 247, 4, this);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI100.PNG", 125,	364);
	pStatic->Show();
	pStatic = NULL;

	pStatic = new SPWindowStatic(WIID_WAYPOINT_TITLE, 10, 7, 125, 15, this);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 1,	424);
	pStatic->Show();
	pStatic = NULL;

	pButton = new SPWindowButton(WIID_WAYPOINT_CLOSE, 226, 9 - 4, 12, 12, pCenter);
	pButton->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 247,	217);
	pButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI102.PNG", 247,	230); //on
	pButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI102.PNG", 247,	244); //push
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI102.PNG", 247,	256); //disable
	pButton->Show();
	pButton = NULL;

	pStatic = new SPWindowStatic(WIID_WAYPOINT_LINE1, 2, 26, 243, 1, this);
	pStatic->SetSrcSize(6, 1);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 349,	329);
	pStatic->Show();
	pStatic = NULL;

	pStatic = new SPWindowStatic(WIID_WAYPOINT_LINE2, 2, 225, 243, 1, this);
	pStatic->SetSrcSize(6, 1);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 349,	329);
	pStatic->Show();
	pStatic = NULL;

	pStatic = new SPWindowStatic(WIID_WAYPOINT_LINE3, 2, 289, 243, 1, this);
	pStatic->SetSrcSize(6, 1);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 349,	329);
	pStatic->Show();
	pStatic = NULL;

	//////////////////////////////////////////////////////////////////////////	Tree
	pParent = new SPWindowStatic(WIID_WAYPOINT_SKIN_TREE, 2, 27 - 4, 243, 198, pCenter);
	pParent->SetSrcSize(6, 1);
	pParent->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 349,	321);	
	pParent->Show();
	
	//Tree Contorl
	m_pTreeBox	=	new SPWindowTreeBox( WIID_WAYPOINT_TREE , 0 , 0, 243 , 198 , pParent );	
	m_pTreeBox->SetBoxTexture( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 349 , 321 , 355 , 327 , 2 );
	m_pTreeBox->SetMargin( 8 , 4 , 4 , 4 );
	m_pTreeBox->UpdateBGPos();

	m_pTreeBox->SetScrollAutoHide( false );

	m_pTreeBox->SetScrollUpSize( 12 , 11 );
	m_pTreeBox->SetScrollUpTexture( DATA_TEXTURE_ENABLE		, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 460 , 458 );
	m_pTreeBox->SetScrollUpTexture( DATA_TEXTURE_HIT			, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 473 , 458 );
	m_pTreeBox->SetScrollUpTexture( DATA_TEXTURE_PUSH			, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 486 , 458 );
	m_pTreeBox->SetScrollUpTexture( DATA_TEXTURE_DISABLE		, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 499 , 458 );

	m_pTreeBox->SetScrollDownSize( 12 , 11 );
	m_pTreeBox->SetScrollDownTexture( DATA_TEXTURE_ENABLE		, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 460 , 500 );
	m_pTreeBox->SetScrollDownTexture( DATA_TEXTURE_HIT		, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 473 , 500 );
	m_pTreeBox->SetScrollDownTexture( DATA_TEXTURE_PUSH		, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 486 , 500 );
	m_pTreeBox->SetScrollDownTexture( DATA_TEXTURE_DISABLE	, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 499 , 500 );

	m_pTreeBox->SetScrollSliderSize( 12 , 24 );
	m_pTreeBox->SetScrollSliderTexture( DATA_TEXTURE_ENABLE	, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 460 , 470 );
	m_pTreeBox->SetScrollSliderTexture( DATA_TEXTURE_HIT		, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 473 , 470 );
	m_pTreeBox->SetScrollSliderTexture( DATA_TEXTURE_PUSH		, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 486 , 470 );
	m_pTreeBox->SetScrollSliderTexture( DATA_TEXTURE_DISABLE	, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 499 , 470 );

	m_pTreeBox->SetScrollSliderBGTexture( DATA_TEXTURE_ENABLE	, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 460 , 496 , 472 , 498 );
	m_pTreeBox->SetScrollSliderBGTexture( DATA_TEXTURE_HIT	, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 473 , 496 , 485 , 498 );
	m_pTreeBox->SetScrollSliderBGTexture( DATA_TEXTURE_PUSH	, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 486 , 496 , 498 , 498 );
	m_pTreeBox->SetScrollSliderBGTexture( DATA_TEXTURE_DISABLE, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 499 , 496 , 511 , 498 );

	m_pTreeBox->SetScrollMargin( 0 , 4 , 4 , 4 );
	m_pTreeBox->UpdateScrollPos();

	m_pTreeBox->Refresh();

		
	pParent = NULL;
	////////////////////////////////////////////////////////////////////////// Desc
	pParent = new SPWindowStatic(WIID_WAYPOINT_SKIN_DESC, 2, 226 - 4, 243, 63, pCenter);
	pParent->SetSrcSize(6, 6);
	pParent->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 349,	312);
	pParent->Show();

	m_pDesc = new SPWindowStatic(WiID_WAYPOINT_DESC, 6 - 2, 235 - 226, 243, 45, pParent);
	m_pDesc->SetFormat(DT_LEFT | DT_TOP | DT_WORDBREAK);
	m_pDesc->SetMultiLine(true, 0);
	//m_pDesc->SetWindowText("Test....\nTest2...\nTest3...");
	m_pDesc->Show();
	
	pParent = NULL;


	//영상 버튼
	pButton = new SPWindowButton(WIID_WAYPOINT_IMAGE, 10, 294 - 4, 68, 17, pCenter);
	pButton->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 1,	440);
	pButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI104.PNG", 1,	458); //on
	pButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI104.PNG", 1,	476); //push
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI104.PNG", 1,	494); //disable
	pButton->Show();
	m_pImage = pButton;
	pButton = NULL;

	//위치 버튼
	pButton = new SPWindowButton(WIID_WAYPOINT_POSITION, 86, 294 - 4, 68, 17, pCenter);
	pButton->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 70,	440);
	pButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI104.PNG", 70,	458); //on
	pButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI104.PNG", 70,	476); //push
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI104.PNG", 70,	494); //disable
	pButton->Show();
	m_pPosition = pButton;
	pButton = NULL;

	//이동 버튼
	pButton = new SPWindowButton(WIID_WAYPOINT_MOVE, 162, 294 - 4, 68, 17, pCenter);
	pButton->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 139,	440);
	pButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI104.PNG", 139,	458); //on
	pButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI104.PNG", 139,	476); //push
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI104.PNG", 139,	494); //disable
	pButton->Show();
	m_pMove = pButton;
	pButton = NULL;
}


void SPWindowWayPoint::Process(float fTime)
{	
	SPWindow::Process(fTime);
	
	if(m_bMoveRequest) {
		m_fAccumulate += fTime;
		if( m_fAccumulate > m_fDeley) {
			m_fAccumulate -= m_fDeley;
			m_pMove->SetEnable(true);
			m_bMoveRequest = false;
		}
	}
}


void SPWindowWayPoint::Render(float fTime)
{
	if(IsShow() == false) return;

	SPWindow::Render(fTime);	
}


void SPWindowWayPoint::Show(bool bWithChild /*= true*/)
{	
	OnUpdateWayPoint((WPARAM)0, (LPARAM)0);

	SPWindow::Show(bWithChild);
	
	m_pImage->SetEnable(false);
	m_pMove->SetEnable(false);
	m_pPosition->SetEnable(false);
}


void SPWindowWayPoint::Hide(bool bSendServer /*= true*/)
{	
	m_vGroupItem.clear();
	m_pTreeBox->DelItemAll();
	
	SPWindow::Hide(bSendServer);	
}


SPTreeBoxItemQuest* SPWindowWayPoint::CreateTreeBoxItem( bool bMenuType )
{
	SPTreeBoxItemQuest*	pQuestItem	=	new SPTreeBoxItemQuest;

	if( pQuestItem == NULL )
		return pQuestItem;

	pQuestItem->SetWindow( this );

	if( bMenuType == true )
	{
		pQuestItem->SetSize( 187 , 12 );
		pQuestItem->SetTextMargin( 20 , 3 );
	}
	else
	{
		pQuestItem->SetSize( 152 , 12 );
		pQuestItem->SetTextMargin( 40 - 12 , 3 );
	}

	for( int j = 0 ; j < DATA_STRING_COUNT ; ++j )
	{
		pQuestItem->SetTextColor( j , m_cStringColor );
	}

	if( bMenuType == true )
	{
		pQuestItem->SetQuestSize( 215 , 19 );
		pQuestItem->SetQuestOffset( 0 , 0 );

		pQuestItem->SetTexture( SPTreeBoxItemQuest::QUEST_TEXTURE_LEFT_ENABLE * 2 ,				"DATA/INTERFACE/CONCEPT/UI102.PNG" , 96 , 76 , 106 , 86 );
		pQuestItem->SetTexture( SPTreeBoxItemQuest::QUEST_TEXTURE_LEFT_ENABLE * 2 + 1 ,			"DATA/INTERFACE/CONCEPT/UI102.PNG" , 85 , 76 ,  95 , 86 );
	}
	else
	{
		pQuestItem->SetQuestSize( 203 , 19 );
		pQuestItem->SetQuestOffset( 12 , 0 );

		pQuestItem->SetTexture( SPTreeBoxItemQuest::QUEST_TEXTURE_LEFT_ENABLE * 2 ,				"DATA/INTERFACE/CONCEPT/UI102.PNG" , 119 , 94 , 131 , 110 );
		pQuestItem->SetTexture( SPTreeBoxItemQuest::QUEST_TEXTURE_LEFT_ENABLE * 2 + 1 ,			"DATA/INTERFACE/CONCEPT/UI102.PNG" , 119 , 111 , 131 , 127 );

		////
		//if( m_InstanceID == WIID_QUEST )
		//{
		//	pQuestItem->SetTexture( SPTreeBoxItemQuest::QUEST_TEXTURE_RIGHT_ENABLE * 2 ,			"DATA/INTERFACE/CONCEPT/UI102.PNG" , 62 , 121 , 74 , 134 );
		//	pQuestItem->SetTexture( SPTreeBoxItemQuest::QUEST_TEXTURE_RIGHT_ENABLE * 2 + 1 ,		"DATA/INTERFACE/CONCEPT/UI102.PNG" , 49 , 121 , 61 , 134 );

		//	pQuestItem->SetTexture( SPTreeBoxItemQuest::QUEST_TEXTURE_RIGHT_MOUSE_OVER * 2 ,		"DATA/INTERFACE/CONCEPT/UI102.PNG" , 62 , 135 , 74 , 148 );
		//	pQuestItem->SetTexture( SPTreeBoxItemQuest::QUEST_TEXTURE_RIGHT_MOUSE_OVER * 2 + 1 ,	"DATA/INTERFACE/CONCEPT/UI102.PNG" , 49 , 135 , 61 , 148 );

		//	pQuestItem->SetTexture( SPTreeBoxItemQuest::QUEST_TEXTURE_RIGHT_CLICK * 2 ,				"DATA/INTERFACE/CONCEPT/UI102.PNG" , 62 , 149 , 74 , 162 );
		//	pQuestItem->SetTexture( SPTreeBoxItemQuest::QUEST_TEXTURE_RIGHT_CLICK * 2 + 1 ,			"DATA/INTERFACE/CONCEPT/UI102.PNG" , 49 , 149 , 61 , 162 );
		//}

		pQuestItem->SetTexture( SPTreeBoxItemQuest::QUEST_TEXTURE_MOUSE_OVER * 2 ,				"DATA/INTERFACE/CONCEPT/UI100.PNG" , 252 , 368 , 455 , 384 );
	}

	//////////////////////////////////////////////////////////////////////////

	if( bMenuType == true )
	{
		pQuestItem->SetTextureOffset( SPTreeBoxItemQuest::QUEST_TEXTURE_LEFT_ENABLE * 2 ,			0 , 4 );
		pQuestItem->SetTextureOffset( SPTreeBoxItemQuest::QUEST_TEXTURE_LEFT_ENABLE * 2 + 1 ,		0 , 4 );
	}
	else
	{
		pQuestItem->SetTextureOffset( SPTreeBoxItemQuest::QUEST_TEXTURE_LEFT_ENABLE * 2 ,			19 - 12 , 1 );
		pQuestItem->SetTextureOffset( SPTreeBoxItemQuest::QUEST_TEXTURE_LEFT_ENABLE * 2 + 1 ,		19 - 12 , 1 );
		
		////
		//if( m_InstanceID == WIID_QUEST )
		//{
		//	pQuestItem->SetTextureOffset( SPTreeBoxItemQuest::QUEST_TEXTURE_RIGHT_ENABLE * 2 ,			196 - 12 , 2 );
		//	pQuestItem->SetTextureOffset( SPTreeBoxItemQuest::QUEST_TEXTURE_RIGHT_ENABLE * 2 + 1 ,		196 - 12 , 2 );

		//	pQuestItem->SetTextureOffset( SPTreeBoxItemQuest::QUEST_TEXTURE_RIGHT_MOUSE_OVER * 2 ,		196 - 12 , 2 );
		//	pQuestItem->SetTextureOffset( SPTreeBoxItemQuest::QUEST_TEXTURE_RIGHT_MOUSE_OVER * 2 + 1 ,	196 - 12 , 2 );

		//	pQuestItem->SetTextureOffset( SPTreeBoxItemQuest::QUEST_TEXTURE_RIGHT_CLICK * 2 ,			196 - 12 , 2 );
		//	pQuestItem->SetTextureOffset( SPTreeBoxItemQuest::QUEST_TEXTURE_RIGHT_CLICK * 2 + 1 ,		196 - 12 , 2 );
		//}

		pQuestItem->SetTextureOffset( SPTreeBoxItemQuest::QUEST_TEXTURE_MOUSE_OVER * 2 ,			12 - 12 , 0 );
	}

	//////////////////////////////////////////////////////////////////////////

	return pQuestItem;
}


bool SPWindowWayPoint::UpdateGroup()
{
	SPTreeBoxItemQuest*	pQuestItem = NULL;
	TREEBOX_ITEM*	pTreeBoxItem = NULL;

	std::vector<std::string>* vGroupName = g_pEventManager->GetWayPointManager()->GetGroupNameList();
	std::vector<std::string>::iterator iter;

	iter = vGroupName->begin();
	int iIndex = 10000;
	for(; iter != vGroupName->end(); ++iter, iIndex++) {
		pQuestItem	=	CreateTreeBoxItem( true );

		if( pQuestItem == NULL )
			return false;
		
		pQuestItem->SetText((*iter).c_str());
		pQuestItem->SetReturnValue(iIndex);

		//TREEBOX_ITEM*	pTreeBoxItem	=	m_pTreeBox->InsertItem( NULL , pQuestItem , iCategory - 1 );
		//pTreeBoxItem	=	m_pTreeBox->InsertItem( NULL , pQuestItem , iIndex );

		pTreeBoxItem = m_pTreeBox->AddItem( 0, pQuestItem );
		m_vGroupItem.push_back(pTreeBoxItem);
	}

	return true;
}


bool SPWindowWayPoint::UpdateWayPoint()
{
	std::string strCategory;

	std::vector<SPWayPointAttr>* vpWayPointList = g_pEventManager->GetWayPointManager()->GetWayPointArchive()->GetWayPointList();
	std::vector<SPWayPointAttr>::iterator iter;
	int iGroupSize = g_pEventManager->GetWayPointManager()->GetGroupSize();

	//SPWindowTreeBox*	pTreeBox	=	(SPWindowTreeBox*)( Find( WIID_WAYPOINT_TREE ) );
	SPTreeBoxItemQuest*	pQuestItem = NULL;
	TREEBOX_ITEM*	pParentItem = NULL;
	TREEBOX_ITEM*	pTreeBoxItem = NULL;
	int iIndex = 0;

	for(iter = vpWayPointList->begin(); iter != vpWayPointList->end(); ++iter, iIndex++) {		
		if((*iter).m_iID == 0) {
			continue;
		}

		if((*iter).m_iGroup - 1 < 0 || (*iter).m_iGroup - 1 >= iGroupSize) {			
			continue;
		}

		pParentItem = m_vGroupItem.at((*iter).m_iGroup - 1);	//Group ID 1베이스, 데이터 0베이스
		if(pParentItem == NULL) {
			continue;
		}
		
		pQuestItem	=	CreateTreeBoxItem( false );
		if(pQuestItem == NULL){
			continue;
		}		
		
		if((*iter).IsSave()) {									//저장된 WayPoint
			pQuestItem->SetText((*iter).m_strName.c_str());
		}
		else {
			//std::string strName = "";
			//strName = (*iter).m_strName;
			//strName += m_strNotSaveInfo;
			//pQuestItem->SetText(strName.c_str());
			
			D3DXCOLOR color((173 / 255.0f), (183 / 255.0f), (192 / 255.0f), 1.0f);
			
			pQuestItem->SetTextColor(DATA_STRING_DISABLE,		color);
			pQuestItem->SetTextColor(DATA_STRING_ENABLE,		color);
			pQuestItem->SetTextColor(DATA_STRING_HIT,			color);
			pQuestItem->SetTextColor(DATA_STRING_PUSH,			color);
			pQuestItem->SetTextColor(DATA_STRING_SELECT,		color);

			pQuestItem->SetTextColor(DATA_STRING_DISABLE * 2,		color);
			pQuestItem->SetTextColor(DATA_STRING_ENABLE * 2,		color);
			pQuestItem->SetTextColor(DATA_STRING_HIT * 2,			color);
			pQuestItem->SetTextColor(DATA_STRING_PUSH * 2,			color);
			pQuestItem->SetTextColor(DATA_STRING_SELECT * 2,		color);
			
			pQuestItem->SetText((*iter).m_strName.c_str());
		}		
		
		if((*iter).m_iType == WAY_POINT_TYPE_TOMB) {
			pQuestItem->SetQuestComplete(true);
		}
	
		pQuestItem->SetReturnValue( (*iter).m_iID );
		//pQuestItem->SetReturnValue(iIndex);

		//TREEBOX_ITEM*	pTreeBoxItem	=	m_pTreeBox->InsertItem( NULL , pQuestItem , iCategory - 1 );
		//pTreeBoxItem	=	m_pTreeBox->InsertItem( pParentItem , pQuestItem , (*iter).m_iID );

		m_pTreeBox->AddItem( pParentItem, pQuestItem );		
	}
	
	return true;
}



//////////////////////////////////////////////////////////////////////////
SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowWayPoint )
SPIMESSAGE_COMMAND(SPIM_WAYPOINT_UPDATE,		OnUpdateWayPoint)
SPIMESSAGE_COMMAND(SPIM_WAYPOINT_MOVERESULT,	OnMoveResult)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_WAYPOINT_CLOSE		, OnClose)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_WAYPOINT_IMAGE		, OnImage)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_WAYPOINT_POSITION	, OnPosition)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_WAYPOINT_MOVE		, OnMovement)
SPIMESSAGE_CONTROL(SPIM_LBUTTON_UP,	WIID_WAYPOINT_TREE		, OnTreeBoxLButtonUp)
SPIMESSAGE_OBJ_MAP_END	( SPWindow	)


int SPWindowWayPoint::OnUpdateWayPoint(WPARAM wParam, LPARAM lParam)
{
	DXUTOutputDebugString("SPWindowWayPoint::OnUpdateWayPoint\n");	

	UpdateGroup();
	UpdateWayPoint();

	m_pTreeBox->Refresh();

	return 1;
}

int SPWindowWayPoint::OnClose(WPARAM wParam, LPARAM lParam)
{	
	DXUTOutputDebugString("SPWindowWayPoint::OnClose\n");
	Hide();
	return 1;
}


int SPWindowWayPoint::OnImage(WPARAM wParam, LPARAM lParam)
{
	DXUTOutputDebugString("SPWindowWayPoint::OnImage\n");
	TREEBOX_ITEM*	pSelectItem = m_pTreeBox->GetSelectItem();

	if(pSelectItem == NULL)
		return 1;

	SPTreeBoxItemQuest*	pItem		=	(SPTreeBoxItemQuest*)( pSelectItem->m_pItemBase );

	if( pItem == NULL )
		return 1;

	int	iWayPointID	=	pItem->GetReturnValue();

	if(iWayPointID > 9999) {
		return 1;
	}

	SPWayPointAttr* pWayPoint = g_pEventManager->GetWayPointManager()->FindWayPointByID(iWayPointID);
	if(pWayPoint == NULL) {
		return 1;
	}

	//[2007/5/7] - 해당 스토리 ID가 있는 경우
	if(pWayPoint->m_iStoryID) {
		if( g_pSubGameManager )
			g_pSubGameManager->SPPerformMessage(SPGM_SET_STORY_VIEW, 1, pWayPoint->m_iStoryID);
	}

	return 1;
}


int SPWindowWayPoint::OnPosition(WPARAM wParam, LPARAM lParam)
{
	DXUTOutputDebugString("SPWindowWayPoint::OnPosition\n");
	TREEBOX_ITEM*	pSelectItem = m_pTreeBox->GetSelectItem();

	if(pSelectItem == NULL)
		return 1;

	SPTreeBoxItemQuest*	pItem		=	(SPTreeBoxItemQuest*)( pSelectItem->m_pItemBase );

	if( pItem == NULL )
		return 1;

	int	iWayPointID	=	pItem->GetReturnValue();

	if(iWayPointID > 9999) {
		return 1;
	}

	SPWayPointAttr* pWayPoint = g_pEventManager->GetWayPointManager()->FindWayPointByID(iWayPointID);
	if(pWayPoint == NULL) {
		return 1;
	}

	// 월드맵에 해당 정보를 표시한다. [9/6/2006 AJJIYA]
	if(pWayPoint->m_bOpen)
		g_pInterfaceManager->SPChildWndSendMessage( WIID_WORLDMAP , SPIM_WORLDMAP_STAGE , (WPARAM)pWayPoint , NULL );

	return 1;
}


int SPWindowWayPoint::OnMovement(WPARAM wParam, LPARAM lParam)
{	
	DXUTOutputDebugString("SPWindowWayPoint::OnMovement\n");	

	//Player 상태 검사
	if(g_pGOBManager->GetLocalPlayer()) {
		// 트레이드 중인 경우
		if(g_pGOBManager->GetLocalPlayer()->GetTradeState() == TRADE_STATE_TRADE) {
			return 1;
		}

		// 개인상점을 개설한 경우
		if(g_pGOBManager->GetLocalPlayer()->GetCurAction() & ACTION_USERSHOP) {
			return 1;
		}
	}
	else {
		return 1;
	}

	TREEBOX_ITEM*	pSelectItem = m_pTreeBox->GetSelectItem();

	if(pSelectItem == NULL)
		return 1;

	SPTreeBoxItemQuest*	pItem		=	(SPTreeBoxItemQuest*)( pSelectItem->m_pItemBase );

	if( pItem == NULL )
		return 1;

	int	iWayPointID	=	pItem->GetReturnValue();

	if(iWayPointID > 9999) {
		return 1;
	}

	SPWayPointAttr* pWayPoint = g_pEventManager->GetWayPointManager()->FindWayPointByID(iWayPointID);
	if(pWayPoint == NULL) {
		return 1;
	}

	//리콰체크
	WAY_POINT_RANK iRank = pWayPoint->CheckRequire();
	if(iRank == WAY_POINT_RANK_NULL) {					//요구조건에 맞는게 없음
		if(pWayPoint->m_strErr.empty() == false) {
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)pWayPoint->m_strErr.c_str() );
		}		
		return 1;
	}

	//인벤토리 체크	
	if(pWayPoint->m_iUseItemID[iRank] && pWayPoint->m_iUseItemCount[iRank]) {		//해당 랭크에 필요아이템이 있는경우
		if(g_pCheckManager->CheckBiggerSlot(pWayPoint->m_iUseItemID[iRank], pWayPoint->m_iUseItemCount[iRank]) == false) {
			if(g_pCheckManager->CheckBiggerSlot5(pWayPoint->m_iUseItemID[iRank], pWayPoint->m_iUseItemCount[iRank]) == false) {
				if(pWayPoint->m_strErr.empty() == false) {
					g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)pWayPoint->m_strErr.c_str() );
				}	
				return 1;
			}
		}
	}	

	//포탈 요청
	g_pEventManager->GetWayPointManager()->SendMove(pWayPoint->m_iID, iRank);
	
	m_bMoveRequest = true;
	m_fAccumulate = 0.0f;	
	m_pMove->SetEnable(false);

	return 1;
}


int SPWindowWayPoint::OnTreeBoxLButtonUp(WPARAM wParam, LPARAM lParam)
{
	TREEBOX_ITEM*	pSelectQuestItem	=	(TREEBOX_ITEM*)wParam;

	if( pSelectQuestItem == NULL )
		return 1;

	SPTreeBoxItemQuest*	pQuestItem		=	(SPTreeBoxItemQuest*)( pSelectQuestItem->m_pItemBase );

	if( pQuestItem == NULL )
		return 1;

	int	iWayPointID	=	pQuestItem->GetReturnValue();

	if(iWayPointID > 9999) {					//부모 노드에서 이벤트가 발생한 경우
		m_pDesc->SetWindowText(NULL);
		m_pImage->SetEnable(false);
		m_pPosition->SetEnable(false);
		m_pMove->SetEnable(false);
		return 1;
	}

	SPWayPointAttr* pWayPoint = g_pEventManager->GetWayPointManager()->FindWayPointByID(iWayPointID);

	if(pWayPoint == NULL) {
		m_pDesc->SetWindowText(NULL);
		m_pImage->SetEnable(false);
		m_pPosition->SetEnable(false);
		m_pMove->SetEnable(false);
		return 1;
	}

	if(pWayPoint->IsSave()) {
		m_pDesc->SetWindowText(pWayPoint->m_strDesc.c_str());
		if(pWayPoint->m_iStoryID)
			m_pImage->SetEnable(true);
		else
			m_pImage->SetEnable(false);
		
		if(pWayPoint->m_bOpen)
			m_pPosition->SetEnable(true);
		else
			m_pPosition->SetEnable(false);
		
		//자동으로 안풀리도록 막음
		if(m_bMoveRequest == false)
			m_pMove->SetEnable(true);
	}	
	else {
		m_pDesc->SetWindowText(m_strNotSave.c_str());
		m_pImage->SetEnable(false);
		
		if(pWayPoint->m_bOpen)
			m_pPosition->SetEnable(true);
		else
			m_pPosition->SetEnable(false);
		
		m_pMove->SetEnable(false);
	}
	
	return 1;
}


int SPWindowWayPoint::OnMoveResult(WPARAM wParam, LPARAM lParam)
{	
	//자동으로 안풀리도록 막음
	if(m_bMoveRequest == false)
		m_pMove->SetEnable(true);
	return 1;
}

