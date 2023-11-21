
#include <vector>
#include <string>

#include "SPCommon.h"
#include "SPUtil.h"

#include "SPMainGameDEF.H"
#include "SPGameObjectDEF.h"

#include "SPSubGameManager.h"
#include "SPGameObject.h"
#include "SPManager.h"
#include "SPMouseCursor.h"
#include "SPIMEPool.h"
#include "SPWindowDEF.h"
#include "SPInterfaceManager.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPWindow.h"
#include "SPWindowButton.h"
#include "SPWindowStatic.h"

#include "SPStage.h"
#include "SPStageManager.h"
#include "SPStageLoader.h"

#include "SPMainGameManager.h"

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
#include "SPPlayer.h"
#include "SPGOBManager.h"
//

#include "SPWindowWorldMapShortcut.h"

#include "SPWindowMiniMap.h"
#include "SPMiniMapCtrl/SPWindowMiniMapIcon.h"
#include "SPMiniMapCtrl/SPWindowMiniMapToolTip.h"
#include "SPMiniMapCtrl/SPWindowMiniMapView.h"


SPWindowMiniMap::SPWindowMiniMap(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
{	
	ResetData();
	Init();	
}

SPWindowMiniMap::SPWindowMiniMap(WND_ID WndClassID, INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WndClassID, InstanceID, iX, iY, iCX, iCY, pParent)	
{	
	ResetData();
	Init();
}

SPWindowMiniMap::~SPWindowMiniMap()
{
	Clean();
}

void SPWindowMiniMap::Init()
{	
	m_RGBA					=	D3DCOLOR_ARGB( 136 , 255 , 255 , 255 );
	m_strToolTip			=	"[SPWindowMiniMap]";

	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI100.PNG" , &m_pBGTexture );

	ReposOwnImage(); // metalgeni [5/25/2006]

	SetRect( &m_rcBGSrc[0]	, 179 , 1 , 368 , 3 );			//	배경 상단
	SetRect( &m_rcBGSrc[1]	, 179 , 5 , 368 , 7 );			//	배경 중단
	SetRect( &m_rcBGSrc[2]	, 179 , 9 , 368 , 11 );			//	배경 하단

	SPWindow* pParent;
	SPWindow* pStatic;	
	
	//기본 컨트롤 바탕
	//pParent = new SPWindowStatic(WIID_MINIMAP_BASE, 0, 0, 185, 37, this);
	//pParent->SetImage		( "DATA/INTERFACE/CONCEPT/UI001.PNG" , 164 , 425 );

	//테마 아이콘

	pStatic = new SPWindow( WNDID_CTRL_STATIC , WIID_MINIMAP_ICON, 2, 2, 32, 32, this);
//	pStatic->SetImage		( "DATA/INTERFACE/CONCEPT/UI001.PNG" , 1 , 367 );	
	pStatic = NULL;

	//맵 이름
	pStatic = new SPWindowStatic(WIID_MINIMAP_NAME, 36, 3, 131, 12, this );
	pStatic->SetFontColor( D3DCOLOR_ARGB( 255 , 255 , 255 , 255 ) );
	pStatic->SetWindowText	( m_strMapName.c_str() );
	pStatic->SetFormat( DT_CENTER | DT_TOP );
	pStatic = NULL;

	//맵 이름 볼드
	pStatic = new SPWindowStatic(WIID_MINIMAP_NAME_BOLD, 37, 3, 131, 12, this );
	pStatic->SetFontColor( D3DCOLOR_ARGB( 255 , 255 , 255 , 255 ) );
	pStatic->SetWindowText	( m_strMapName.c_str() );
	pStatic->SetFormat( DT_CENTER | DT_TOP );
	pStatic = NULL;

	//맵 번호(좌표) 배경
	pStatic = new SPWindowStatic(WIID_MINIMAP_INFO_BG, 36, 17, 151, 17, this );
	pStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 1 , 370 );
	pStatic = NULL;

	//맵 번호(좌표)
	m_pMiniMapInfo = new SPWindowStatic( WIID_MINIMAP_INFO , 36, 20, 151, 14, this );
	m_pMiniMapInfo->SetFontColor( D3DCOLOR_ARGB( 255 , 0 , 52 , 89 ) );
	m_pMiniMapInfo->SetWindowText( m_szMapInfo );
	m_pMiniMapInfo->SetFormat( DT_CENTER | DT_TOP );

	//미니맵 열고 닫기 버튼
	m_pMiniMapClose = new SPWindowButton(WIID_MINIMAP_BUTTON, 168, 3, 12, 12, this);
	m_pMiniMapClose->SetImage		( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 477 , 460 );
	m_pMiniMapClose->SetImageHit	( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 477 , 473 );
	m_pMiniMapClose->SetImagePush	( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 477 , 486 );
	m_pMiniMapClose->SetImageDisable( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 477 , 499 );

	//미니맵 View
	m_pView = new SPWindowMiniMapView(WIID_MINIMAP_VIEW, 2, 36, 185, 75, this );

	// 월드맵 단축아이콘
	pStatic	= new SPWindowWorldMapShortcut( WIID_WORLDMAP_SHORTCUT , 154 , 113 , 35 , 16 , this );
	pStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 179 , 12 );

	pParent = NULL;
}

void SPWindowMiniMap::Clean()
{
	SPWindow::Clean();

	SAFE_RELEASE( m_pBGTexture );
	SAFE_RELEASE( m_pTheme );
}

void SPWindowMiniMap::ResetData() {
	m_strMapName = "";
	//m_strMapInfo = "";
	ZeroMemory(m_szMapInfo, 128);
	m_pTheme = NULL;
	m_bBuildingIn = false;
	m_bMiniMapViewStatus = true;
	//m_vpMiniMap ;
	m_iStageID = -1;
	m_iMapGroupID = -1;
	m_iMapID = -1;
	m_fPosX = -1.0f;
	m_fPosY = -1.0f;
}

void SPWindowMiniMap::UpdateMapInfo()
{
	if( g_pGOBManager->GetLocalPlayer() == NULL )
		return;
    
	m_iStageID = g_pGOBManager->GetLocalPlayer()->GetCurStageID();
	m_iMapGroupID = g_pGOBManager->GetLocalPlayer()->GetCurMapGroupID();

	SPStage* pCurStage = g_StageManager.GetCurStage(m_iStageID);

	if( pCurStage == NULL )
		return;

	SPMapGroup* pMapGroup = pCurStage->GetMapGroup( m_iMapGroupID );

	if( pMapGroup == NULL )
		return;

	m_strMapName	= pMapGroup->m_strGroupName;

	ShowThemeIcon( pMapGroup->m_iMiniMapResID , pMapGroup->m_iMiniMapIconID );

	SPWindow* pWindow;
	pWindow = Find(WIID_MINIMAP_NAME);
	if(pWindow) {
		pWindow->SetWindowText(m_strMapName.c_str());
	}

	pWindow = Find(WIID_MINIMAP_NAME_BOLD);
	if(pWindow) {
		pWindow->SetWindowText(m_strMapName.c_str());
	}

	m_pView->SetMiniMapImageList();

	//
	// AJJIYA [5/24/2005]
	// 빌딩 안이면 미니맵을 표시하지 않는다.
	// 3번에서 0번으로 번경 [6/16/2005]

	if( !pMapGroup->IsMapGroupType(GROUP_TYPE_MINIMAP) )
		m_bBuildingIn	=	true;
	else
		m_bBuildingIn	=	false;

	ShowMiniMapButton( m_bMiniMapViewStatus );
}


void SPWindowMiniMap::UpdatePos()
{
	if( g_pGOBManager->GetLocalPlayer() == NULL )
		return;

	m_fPosX = g_pGOBManager->GetLocalPlayer()->GetPosX();
	m_fPosY = g_pGOBManager->GetLocalPlayer()->GetPosY();

	SPStage*	pStage	=	g_StageManager.GetCurStage( m_iStageID );

	if( pStage == NULL )
		return;

	SPMapGroup*	pMapGroup = pStage->GetMapGroup( m_iMapGroupID );

	if( pMapGroup == NULL )
		return;

	m_iMapID		=	pMapGroup->FindMap( m_fPosX , m_fPosY );
	m_iMapIDMax		=	pMapGroup->m_iMapCount;
	m_iGroupHeight	=	static_cast<int>( pMapGroup->m_lGroupHeight );

	// 미니맵 높이 표시 방법 변경 유팀장님 요청건 [9/15/2006 AJJIYA]
//	wsprintf(m_szMapInfo, "%d/%d (%d,%d)", m_iMapID + 1 , m_iMapIDMax , (int)m_fPosX, ( m_iGroupHeight - (int)m_fPosY ) );
	wsprintf(m_szMapInfo, "%d/%d (%d,%d)", m_iMapID + 1 , m_iMapIDMax , (int)m_fPosX, (int)m_fPosY );

	m_pMiniMapInfo->SetWindowText( m_szMapInfo );
}

void SPWindowMiniMap::ShowMiniMapButton( bool bStatus )
{
	if( m_pView == NULL || m_pMiniMapClose == NULL )
		return;

	if( bStatus == true && m_bBuildingIn == false )
		m_pView->Show();
	else
		m_pView->Hide();

	if( m_bBuildingIn == false )
		m_pMiniMapClose->Show();
	else
		m_pMiniMapClose->Hide();

	SPWindowWorldMapShortcut*	pWindowWorldMap	=	(SPWindowWorldMapShortcut*)Find( WIID_WORLDMAP_SHORTCUT );

	if( pWindowWorldMap != NULL )
	{
		if( m_pView->IsShow() == true )
		{
			pWindowWorldMap->AdjustCoord( 154 , 113 );
			SetRectSize( 189 , 129 );
		}
		else
		{
			pWindowWorldMap->AdjustCoord( 154 , 36 );
			SetRectSize( 189 , 52 );
		}

		pWindowWorldMap->UpdatePos();
		ReposOwnImage();
	}
}

void SPWindowMiniMap::ShowThemeIcon( int iResID , int iResIndex )
{
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
    
	SPWindow*	pWindow = Find( WIID_MINIMAP_ICON );
	if( pWindow )
		pWindow->SetImage( strFullRes.c_str() , iIconXIndex * iIconSizeX , iIconYIndex * iIconSizeY );
}

void SPWindowMiniMap::Process(float fTime)
{
	if( g_pGOBManager->GetLocalPlayer() == NULL )
		return;

	UpdatePos();
	SPWindow::Process(fTime);
}


void SPWindowMiniMap::Render(float fTime)
{
	if( !m_bShow )
		return;

	g_pVideo->Flush();
	if( m_pBGTexture )
	{
		for( int i = 0 ; i < MINIMAP_BG_COUNT ; i++ )
		{
			m_pBGTexture->RenderTexture( &m_rcBGDest[i] , &m_rcBGSrc[i] );
		}

		g_pVideo->Flush();
	}

	SPWindow::Render(fTime);
}

void SPWindowMiniMap::Show(bool bWithChild)
{
	SPWindow::Show(bWithChild);
	
	ShowMiniMapButton( m_bMiniMapViewStatus );
}

SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowMiniMap )
	SPIMESSAGE_COMMAND(	SPIM_CURSOR_ENTER,			OnCursorEnter	)
	SPIMESSAGE_COMMAND(	SPIM_CURSOR_OUT,			OnCursorOut		)
	SPIMESSAGE_COMMAND(	SPIM_LBUTTON_DOWN,			OnCursorLDown	)
	SPIMESSAGE_COMMAND(	SPIM_LBUTTON_UP,			OnCursorLUp		)
	
	SPIMESSAGE_COMMAND( SPIM_SET_STAGE,				OnSetStageID	)
	SPIMESSAGE_COMMAND( SPIM_REFRESH_MINIMAP_GOB,	OnRefreshMiniMapGOB	)
	SPIMESSAGE_COMMAND( SPIM_REFRESH_ICON_TEAM,		OnRefreshIconTeam	)

	SPIMESSAGE_COMMAND( SPIM_PURGE,					OnPurgeAll		)

	SPIMESSAGE_COMMAND( SPIM_WORLDMAP_WORLD,		OnWorldMap		)
	SPIMESSAGE_COMMAND( SPIM_WORLDMAP_STAGE,		OnStageMap		)

	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_MINIMAP_BUTTON,	OnMiniMapButton		)

	//SPIMESSAGE_COMMAND( SPIM_SET_CAMERA_POS,	OnSetCameraPos	)
	//SPIMESSAGE_COMMAND( SPIM_SET_LOCAL_PLAYER_POS, OnSetPlayerPos)
SPIMESSAGE_OBJ_MAP_END	( SPWindow	)


int SPWindowMiniMap::OnCursorEnter	( WPARAM, LPARAM )
{
	m_RGBA = D3DCOLOR_ARGB( 255 , 255 , 255 , 255 );
	return 1;
}


int SPWindowMiniMap::OnCursorOut	( WPARAM, LPARAM )
{
	m_RGBA = D3DCOLOR_ARGB( 136 , 255 , 255 , 255 );
	return 1;
}


int SPWindowMiniMap::OnCursorLDown	( WPARAM, LPARAM )
{
	//m_RGBA = RGBA(0x88,0x88,0xFF,0xFF);
	return 1;
}


int SPWindowMiniMap::OnCursorLUp	( WPARAM, LPARAM )
{
	//m_RGBA = RGBA(0xFF,0x00,0x00,0xFF);
	return 1;
}


//////////////////////////////////////////////////////////////////////////
int SPWindowMiniMap::OnSetStageID	( WPARAM wParam, LPARAM lParam) {	
	int iStage = (int)wParam;
	int iMapGroup = (int)lParam;
	
	UpdateMapInfo();

	return 1;
}

int SPWindowMiniMap::OnRefreshMiniMapGOB( WPARAM wParam, LPARAM lParam )
{
	if( m_pView != NULL )
		m_pView->SPSendMessage(SPIM_REFRESH_MINIMAP_GOB, wParam, lParam);

	return 1;
}

int SPWindowMiniMap::OnRefreshIconTeam( WPARAM wParam, LPARAM lParam )
{
	if( m_pView != NULL )
		m_pView->SPSendMessage(SPIM_REFRESH_ICON_TEAM, wParam, lParam);

	return 1;
}

int SPWindowMiniMap::OnPurgeAll( WPARAM wParam, LPARAM lParam )
{
	m_pView->SPSendMessage( SPIM_PURGE );

	return 1;
}

int SPWindowMiniMap::OnWorldMap( WPARAM wParam, LPARAM lParam )
{
	g_pInterfaceManager->SPChildWndSendMessage( WIID_WORLDMAP , SPIM_WORLDMAP_WORLD , NULL , NULL ); 
	return 1;
}

int SPWindowMiniMap::OnStageMap( WPARAM wParam, LPARAM lParam )
{
	g_pInterfaceManager->SPChildWndSendMessage( WIID_WORLDMAP , SPIM_WORLDMAP_STAGE , NULL , NULL ); 
	return 1;
}

int SPWindowMiniMap::OnMiniMapButton( WPARAM, LPARAM )
{
	if( m_bBuildingIn == true || m_pView == NULL || m_pMiniMapClose == NULL )
		return 1;

	m_bMiniMapViewStatus	=	!m_bMiniMapViewStatus;
	ShowMiniMapButton( m_bMiniMapViewStatus );

	return 1;
}

void SPWindowMiniMap::ReposOwnImage()
{
	SetRect( &m_rcBGDest[0]	, m_iAX					, m_iAY					, m_iAX + 189			, m_iAY + 2 );							//	배경 상단
	SetRect( &m_rcBGDest[1]	, m_rcBGDest[0].left	, m_rcBGDest[0].bottom	, m_rcBGDest[0].right	, m_rcBGDest[0].bottom + m_iSY - 20 );	//	배경 중단
	SetRect( &m_rcBGDest[2]	, m_rcBGDest[1].left	, m_rcBGDest[1].bottom	, m_rcBGDest[1].right	, m_rcBGDest[1].bottom + 2 );			//	배경 하단
}
