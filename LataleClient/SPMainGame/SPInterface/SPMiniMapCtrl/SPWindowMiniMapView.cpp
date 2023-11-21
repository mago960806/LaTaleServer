
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

#include "SPResourceDef.h"
#include "SPWindowDEF.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowMiniMapIcon.h"
#include "SPWindowMiniMapToolTip.h"
#include "SPWindowMiniMapView.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"

//////////////////////////////////////////////////////////////////////////
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
#include "SPTeamArchive.h"

#include "SPDebug.h"
//////////////////////////////////////////////////////////////////////////



SPWindowMiniMapView::SPWindowMiniMapView(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_CTRL_STATIC, InstanceID, iX, iY, iCX, iCY, pParent)
{
	Init();
}


SPWindowMiniMapView::~SPWindowMiniMapView()
{
	Clean();
}


void SPWindowMiniMapView::Init()
{
	m_strToolTip = "[SPWindowStatic]";
	m_iFirstMapID = -1;
	m_iFirstMapID = -1;
	m_pMapGroup = NULL;
	SetRect(&m_rtDestRect, m_iAX , m_iAY, m_iAX + m_iSX - 1 , m_iAY + m_iSY );
	m_bShowToolTip	=	false;

	InitSavePoint();

	if( g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI104.PNG" , &m_pImageOutLine ) == false )
	{
		m_pImageOutLine	=	NULL;
	}

	SetRect( &m_rcSrcOutLine[ 0 ] , 488 , 423 , 489 , 498 );
	SetRect( &m_rcSrcOutLine[ 1 ] , 491 , 423 , 493 , 498 );
	SetRect( &m_rcSrcOutLine[ 2 ] , 495 , 423 , 496 , 498 );

	SetRect( &m_rcDestOutLine[ 0 ] , m_iAX				, m_iAY , m_iAX + 1			, m_iAY + m_iSY );
	SetRect( &m_rcDestOutLine[ 1 ] , m_iAX + 1			, m_iAY , m_iAX + m_iSX - 1	, m_iAY + m_iSY );
	SetRect( &m_rcDestOutLine[ 2 ] , m_iAX + m_iSX - 1	, m_iAY , m_iAX + m_iSX - 0	, m_iAY + m_iSY );
}


void SPWindowMiniMapView::Clean()
{
	SPWindow::Clean();
	
	////
	//std::vector<IMAGE_ATTR>::iterator Iter = m_vpImageAttr.begin();
	//for(; Iter != m_vpImageAttr.end(); ++Iter) {
	//	SAFE_RELEASE((*Iter).pImage);
	//}
	//m_vpImageAttr.clear();

	ResetMiniMapImageList();

	IconClean();

	CleanSavePoint();

	SAFE_RELEASE( m_pImageOutLine );
}


void SPWindowMiniMapView::ResetMiniMapImageList() 
{
	m_iFirstMapID = -1;
	m_iFirstMapID = -1;
	
	std::vector<IMAGE_ATTR>::iterator Iter = m_vpImageAttr.begin();
	for(; Iter != m_vpImageAttr.end(); ++Iter) {
		SAFE_RELEASE((*Iter).pImage);
	}
	m_vpImageAttr.clear();

	std::vector<SPTexture*>::iterator iter = m_vpImage.begin();
	for(; iter != m_vpImage.end(); ++iter ) {
		SAFE_RELEASE((*iter));
	}
	m_vpImage.clear();

	m_iFirstMapID = -1;			//렌더를 무시하기위한 설정
	m_iSecondMapID = -1;

//	IconClean();
}


void SPWindowMiniMapView::SetMiniMapImageList() 
{
	if ( g_pGOBManager->GetLocalPlayer() == NULL )
		return;

	m_iStage = g_pGOBManager->GetLocalPlayer()->GetCurStageID();
	m_iMapGroup = g_pGOBManager->GetLocalPlayer()->GetCurMapGroupID();
	
	m_pMapGroup = NULL;
	m_pMapGroup = g_StageManager.GetCurStage(m_iStage)->GetMapGroup(m_iMapGroup);
	
	m_iGroupWidth = static_cast<int>(m_pMapGroup->m_lGroupWidth);
	m_iGroupHeight = static_cast<int>(m_pMapGroup->m_lGroupHeight);
	m_iMapWidth = static_cast<int>(m_pMapGroup->m_vpMapInfo[0]->fMapWidth);
	m_iMapHeight = static_cast<int>(m_pMapGroup->m_vpMapInfo[0]->fMapHeight);

	std::vector<SPMapInfo*>::iterator iter;
	std::string strFilePath ;
	
	ResetMiniMapImageList();
	for(iter = m_pMapGroup->m_vpMapInfo.begin(); iter != m_pMapGroup->m_vpMapInfo.end(); ++iter) {
		//strFilePath = "";
		//strFilePath = RES_WORK_RESOURCE_PATH;
		//strFilePath += (*iter)->strMiniMapFile;

		strFilePath.clear();		
		strFilePath = (*iter)->strMiniMapFile;
			
		
		bool bResult = false;
		//bResult = SetImage(strFilePath.c_str());
		
		SPTexture* pImage = NULL;
		//bResult = g_pVideo->GetTextureMgr()->LoadTexture(strFilePath.c_str(), &pImage);
		bResult = g_pVideo->GetTextureMgr()->GetTexture(strFilePath.c_str(), &pImage,TEX_TERRAIN);
		m_vpImage.push_back(pImage);

#ifdef _DEBUG
		DXUTOutputDebugString("Load MiniMapImage : %s[%d]\n", strFilePath.c_str(), bResult);
#endif
	}

	GetSavePoint( m_iStage , m_iMapGroup );
}

void SPWindowMiniMapView::SetImage(SPTexture* pImage, int iSrcX, int iSrcY)
{
	IMAGE_ATTR ImageAttr;
	ImageAttr.pImage = pImage;
	ImageAttr.SrcPoint.x = iSrcX;
	ImageAttr.SrcPoint.y = iSrcY;
	m_vpImageAttr.push_back(ImageAttr);	

	OnSetImageIndex(0,0);
}


bool SPWindowMiniMapView::SetImage(const char* szImage, int iSrcX, int iSrcY)
{
	SPTexture* pImage = NULL;	
	std::string strResource;

	bool bResult = false;

	//strResource = RES_WORK_RESOURCE_PATH;
	strResource = szImage;	

#ifdef _DEBUG
	DXUTOutputDebugString("\tLoad MiniMap : %s\n", strResource.c_str());
#endif

	bResult = g_pVideo->GetTextureMgr()->LoadTexture(strResource.c_str(), &pImage);
	
	SetImage(pImage, iSrcX, iSrcY);	
	
	return bResult;
}

void SPWindowMiniMapView::Process(float fTime)
{
	SPWindow::Process(fTime);

	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return;

	m_iPosX = static_cast<int>( pLocalPlayer->GetPosX() );
	m_iPosY = static_cast<int>( pLocalPlayer->GetPosY() );

	//	현재 로컬 플레이어의 위치를 기준으로 현재의 미니맵 영역을 한번 계산해 놓자
	m_rtMiniMap = GetMiniMapRect( m_iPosX , m_iPosY );

	SetRenderProcess();

	IconProcess( fTime );
}


void SPWindowMiniMapView::SetRenderProcess() 
{
	if( m_pMapGroup == NULL )
		return;
	
	m_iFirstMapID		= 0;
	m_iSecondMapID		= 0;	

	//	맵 링크 타입에 따른 해당 맵을 가져오자.
	switch( m_pMapGroup->m_iMapLink ) 
	{
	case MAP_LINK_NULL:			//	하나의 맵일 경우
	case MAP_LINK_HORIZONTAL:	//	수평 링크일경우
		//	미니맵 영역의 좌측점 위치 기준
		m_iFirstMapID	= m_pMapGroup->FindMap( static_cast<float>( m_rtMiniMap.left )	, static_cast<float>( m_iPosY ) );
		//	미니맵 영역의 우측점 위치 기준
		m_iSecondMapID	= m_pMapGroup->FindMap( static_cast<float>( m_rtMiniMap.right )	, static_cast<float>( m_iPosY ) );
		break;
	case MAP_LINK_VERTICAL:		//	수직 링크일 경우
		//	미니맵 영역의 좌측점 위치 기준
		m_iFirstMapID	= m_pMapGroup->FindMap( static_cast<float>( m_iPosX )	, static_cast<float>( m_rtMiniMap.top ) );
		//	미니맵 영역의 우측점 위치 기준
		m_iSecondMapID	= m_pMapGroup->FindMap( static_cast<float>( m_iPosX )	, static_cast<float>( m_rtMiniMap.bottom ) );
		break;
	}

	//	첫번째 맵이 없을수 없다!@@!!!
	if( m_iFirstMapID < 0 )
		return;

	//	미니맵 좌표를 월드에서 로컬로 변경시킨다.
	RECT	rtLocalMiniMap;
	SetRect( &rtLocalMiniMap , m_rtMiniMap.left , m_rtMiniMap.top , m_rtMiniMap.right , m_rtMiniMap.bottom );

	switch( m_pMapGroup->m_iMapLink )
	{
	case MAP_LINK_NULL:			//	하나의 맵일 경우
	case MAP_LINK_HORIZONTAL:	//	수평 링크일경우
		{
			int	iOffSetWidth		=	m_iMapWidth	*	m_iFirstMapID;
			rtLocalMiniMap.left		=	m_rtMiniMap.left	-	iOffSetWidth;
			rtLocalMiniMap.right	=	m_rtMiniMap.right	-	iOffSetWidth;
		}
		break;
	case MAP_LINK_VERTICAL:		//	수직 링크일 경우
		{
			int	iOffSetHeight		=	m_iMapHeight *	m_iFirstMapID;
			rtLocalMiniMap.top		=	m_rtMiniMap.top		-	iOffSetHeight;
			rtLocalMiniMap.bottom	=	m_rtMiniMap.bottom	-	iOffSetHeight;
		}
		break;
	}

	//	맵 링크에 따라 이미지 영역을 분할한다.
	SetRect( &m_rtFirstSrc	, rtLocalMiniMap.left , rtLocalMiniMap.top , rtLocalMiniMap.right , rtLocalMiniMap.bottom );

	switch( m_pMapGroup->m_iMapLink )
	{
	case MAP_LINK_NULL:			//	하나의 맵일 경우
		m_iSecondMapID	=	-1;
		break;
	case MAP_LINK_HORIZONTAL:	//	수평 링크일경우
		if( m_rtFirstSrc.right > m_iMapWidth )
		{
			SetRect( &m_rtSecondSrc	, 0 , rtLocalMiniMap.top , m_rtFirstSrc.right - m_iMapWidth , rtLocalMiniMap.bottom );
			m_rtFirstSrc.right	=	m_iMapWidth;
		}
		else
		{
			m_iSecondMapID	=	-1;
		}
		break;
	case MAP_LINK_VERTICAL:		//	수직 링크일 경우		
		if( m_rtFirstSrc.bottom > m_iMapHeight )
		{
			SetRect( &m_rtSecondSrc	, rtLocalMiniMap.left , 0 , rtLocalMiniMap.right , m_rtFirstSrc.bottom - m_iMapHeight );
			m_rtFirstSrc.bottom	=	m_iMapHeight;
		}
		else
		{
			m_iSecondMapID	=	-1;
		}
		break;
	}

	//	이미지 영역의 로컬 좌표를 미니맵 상의 축소된 좌표로 변환한다.
	SetRect( &m_rtFirstSrc ,	m_rtFirstSrc.left	/ MINIMAP_SCALE_X , m_rtFirstSrc.top		/ MINIMAP_SCALE_Y ,
								m_rtFirstSrc.right	/ MINIMAP_SCALE_X , m_rtFirstSrc.bottom		/ MINIMAP_SCALE_Y );
	SetRect( &m_rtSecondSrc ,	m_rtSecondSrc.left	/ MINIMAP_SCALE_X , m_rtSecondSrc.top		/ MINIMAP_SCALE_Y ,
								m_rtSecondSrc.right	/ MINIMAP_SCALE_X , m_rtSecondSrc.bottom	/ MINIMAP_SCALE_Y );

	//	대상 영역을 계산한다.
	int	iFirstSizeX		=	m_rtFirstSrc.right		-	m_rtFirstSrc.left;
	int iFirstSizeY		=	m_rtFirstSrc.bottom		-	m_rtFirstSrc.top;

	//	첫번째장은 무조건 계산
	SetRect( &m_rtFirstDest	, m_iAX , m_iAY , m_iAX + iFirstSizeX	, m_iAY + iFirstSizeY );

	//	두번째장은 있을때 계산
	if( m_iSecondMapID != -1 )
	{
		int	iSecondSizeX	=	m_rtSecondSrc.right		-	m_rtSecondSrc.left;
		int iSecondSizeY	=	m_rtSecondSrc.bottom	-	m_rtSecondSrc.top;

		switch( m_pMapGroup->m_iMapLink )
		{
		case MAP_LINK_NULL:			//	하나의 맵일 경우
		case MAP_LINK_HORIZONTAL:	//	수평 링크일경우
			SetRect( &m_rtSecondDest, m_rtFirstDest.right , m_iAY , m_rtFirstDest.right + iSecondSizeX	, m_iAY + iSecondSizeY );
			break;
		case MAP_LINK_VERTICAL:		//	수직 링크일 경우
			SetRect( &m_rtSecondDest, m_iAX , m_rtFirstDest.bottom , m_iAX + iSecondSizeX	, m_rtFirstDest.bottom + iSecondSizeY );
			break;
		}
	}
}


void SPWindowMiniMapView::Render(float fTime)
{
	if( m_bShow == FALSE ) return;

	//	SPWindow::Render(fTime);		SPWindow 를 SavePoint로 렌더하기때문에.

	if( RenderMiniMap() == false )
		return;

	if( g_pGOBManager->GetLocalPlayer() == NULL ) return;

	//	IconRender내에서 ToolTip도 렌더한다.
	IconRender();

	//	SavePoint 렌더.
	RenderSavePoint( fTime );

	//	ToolTip이 있으면 렌더한다.
	if( m_bShowToolTip == true )
		m_ToolTip.Render();

}


bool SPWindowMiniMapView::RenderMiniMap() 
{
	bool	bResult	=	false;

	if(m_pMapGroup == NULL)
		return bResult;

	int	iImageCount	=	(int)m_vpImage.size();

	//	첫번째 미니맵 그리기
	if( m_iFirstMapID >= 0 && iImageCount > m_iFirstMapID )
	{
		SPTexture*	pFirstTexture	=	m_vpImage.at( m_iFirstMapID );

		if( pFirstTexture != NULL )
		{
			pFirstTexture->RenderTexture( &m_rtFirstDest , &m_rtFirstSrc );
			bResult	=	true;
		}
	}

	//	-1이 아니면 두번째 미니맵 그리기
	if( m_iSecondMapID >= 0 && iImageCount > m_iSecondMapID )
	{
		SPTexture*	pSecondTexture	=	m_vpImage.at( m_iSecondMapID );

		if( pSecondTexture != NULL )
		{
			pSecondTexture->RenderTexture( &m_rtSecondDest , &m_rtSecondSrc );
			bResult	=	true;
		}
	}

	if( bResult == true )
	{
		//	테두리 그리기
		g_pVideo->Flush();

		if( m_pImageOutLine != NULL )
		{
			for( int i = 0 ; i < MINIMAP_OUTLINE ; ++i )
			{
				m_pImageOutLine->RenderTexture( &m_rcDestOutLine[ i ] , &m_rcSrcOutLine[ i ] );
			}
		}
	}

	return bResult;
}

SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowMiniMapView )
	SPIMESSAGE_COMMAND(	SPIM_CURSOR_ENTER,			OnCursorEnter		)
	SPIMESSAGE_COMMAND(	SPIM_CURSOR_OUT,			OnCursorOut			)
	SPIMESSAGE_COMMAND(	SPIM_SET_IMAGEINDEX,		OnSetImageIndex		)
	SPIMESSAGE_COMMAND( SPIM_REFRESH_MINIMAP_GOB,	OnRefreshMiniMapGOB	)
	SPIMESSAGE_COMMAND( SPIM_REFRESH_ICON_TEAM,		OnRefreshIconTeam	)
	SPIMESSAGE_COMMAND( SPIM_PURGE,					OnPurgeAll			)
SPIMESSAGE_OBJ_MAP_END	( SPWindow	)


int SPWindowMiniMapView::OnSetImageIndex( WPARAM wParam, LPARAM lParam)
{
	int iIndex = static_cast<int>( wParam );
	if( iIndex >= 0 && iIndex < static_cast<int>(m_vpImageAttr.size() ) ) {
		m_iCurImage	= iIndex;
	}

	if(!m_vpImageAttr.empty()) {
		m_ppImageActive = &(m_vpImageAttr[m_iCurImage].pImage);
		m_iSrcX = m_vpImageAttr[m_iCurImage].SrcPoint.x;
		m_iSrcY = m_vpImageAttr[m_iCurImage].SrcPoint.y;
	}

	return 0;
}

int SPWindowMiniMapView::OnRefreshMiniMapGOB( WPARAM wParam, LPARAM lParam )
{
	//
	// AJJIYA [5/13/2005]
	// wParam이 양수면 GOB 타입을 lParam엔 GOB 포인터가 오고
	// wParam이 음수면 lParam은 Instance_ID가 온다.
	
	INSTANCE_ID		Instance_ID;
	SPGameObject*	pObj			=	NULL;
	int				iMessage		=	static_cast<int>( wParam );

	if( iMessage > 0 )
	{
		MINIMAP_ICON_TYPE	eIconType	=	static_cast<MINIMAP_ICON_TYPE>( iMessage );

		pObj	=	(SPGameObject*)( lParam );

		if( pObj == NULL )
			return 1;

		if( eIconType == MINIMAP_ICON_MYPLAYER )
			m_LocalPlayer.Create( eIconType , pObj , this );
		else
		{
			Instance_ID	=	pObj->GetInstanceID();
			SPWindowMiniMapIcon*	pIcon	=	new SPWindowMiniMapIcon;

			// 플레이어가 추가될때 팀원인걸 체크해서 추가한다. [2/23/2006 AJJIYA]
			SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

			if( pLocalPlayer != NULL )
			{
				SPTeamArchive*	pTeamArchive	=	pLocalPlayer->GetTeam();

				if( pTeamArchive != NULL )
				{
					if( pTeamArchive->GetActive() == true )
					{
						int	iTeamCount	=	pTeamArchive->GetTeamCount();
						std::vector< TEAM_MEMBER* >* pvTeamMemberList = pTeamArchive->GetTeamList();
						TEAM_MEMBER*	pTeamMember;

						for( int i = 0 ; i < iTeamCount ; i++ )
						{
							pTeamMember	=	pvTeamMemberList->at( i );

							if( pObj->GetGUID() == pTeamMember->CharGUID )
							{
								eIconType	=	MINIMAP_ICON_TEAMPLAYER;
								break;
							}
						}
					}
				}
			}

			//////////////////////////////////////////////////////////////////////////

			if( eIconType == MINIMAP_ICON_MYPLAYER || eIconType == MINIMAP_ICON_OTHERPLAYER ||
				eIconType == MINIMAP_ICON_DEATHTEAMPLAYER || eIconType == MINIMAP_ICON_TEAMPLAYER
				)
			{
				SPPlayer*	pTeamMember	=	(SPPlayer*)( pObj );

				if( pTeamMember != NULL )
				{
					bool	bTeamMember	=	false;

					//	팀멤버
					if( eIconType == MINIMAP_ICON_TEAMPLAYER )
						bTeamMember	=	true;

					pTeamMember->SetLocalPlayerTeam( bTeamMember );
				}
			}

			//////////////////////////////////////////////////////////////////////////

			pIcon->Create( eIconType , pObj , this );

			m_hmGOBIcon.insert( stdext::hash_map< INSTANCE_ID , SPWindowMiniMapIcon* >::value_type( Instance_ID , pIcon ) );
		}
	}
	else if( iMessage < 0 )
	{
		//	-3이면 다 삭제를 의미한다.
		if( iMessage == MINIMAP_ICON_DEL_ALL )
		{
			IconClean();
			return 1;
		}

		Instance_ID	=	static_cast<INSTANCE_ID>( lParam );

		stdext::hash_map< INSTANCE_ID , SPWindowMiniMapIcon* >::iterator hmIter = m_hmGOBIcon.find( Instance_ID );

		if( hmIter != m_hmGOBIcon.end() && Instance_ID == (*hmIter).first )
		{
			SAFE_DELETE((*hmIter).second);
			m_hmGOBIcon.erase( hmIter );
		}
	}

	return 1;
}

int SPWindowMiniMapView::OnRefreshIconTeam( WPARAM wParam, LPARAM lParam )
{
	int					iMessage	=	static_cast<int>( wParam );
	GU_ID*				pGU_ID		=	(GU_ID*)lParam;
	GU_ID				tGU_ID		=	0;

	if( pGU_ID != NULL )
	{
		tGU_ID	=	*pGU_ID;
	}

	INSTANCE_ID			Instance_ID	=	GetGUID2InstanceID( tGU_ID );
	MINIMAP_ICON_TYPE	eIconType	=	MINIMAP_ICON_NULL;

	SPPlayer*	pTeamMember	=	(SPPlayer*)( g_pGOBManager->Find( tGU_ID ) );

	if( pTeamMember != NULL )
	{
		bool	bTeamMember	=	false;

		//	팀멤버
		if( iMessage == 1 )
			bTeamMember	=	true;

		pTeamMember->SetLocalPlayerTeam( bTeamMember );
	}

	stdext::hash_map< INSTANCE_ID , SPWindowMiniMapIcon* >::iterator hmIter = m_hmGOBIcon.find( Instance_ID );

	if( hmIter != m_hmGOBIcon.end() && Instance_ID == (*hmIter).first )
	{
		//	팀멤버
		if( iMessage == 1 )
			eIconType	=	MINIMAP_ICON_TEAMPLAYER;
		//	일반멤버로
		else if( iMessage == 2 )
			eIconType	=	MINIMAP_ICON_OTHERPLAYER;

		((*hmIter).second)->SetIconType( eIconType );
	}

	return 1;
}

int SPWindowMiniMapView::OnPurgeAll( WPARAM wParam, LPARAM lParam )
{
	ResetMiniMapImageList();

	IconClean();

	return 1;
}

int SPWindowMiniMapView::OnCursorEnter( WPARAM wParam, LPARAM lParam)
{
	m_bShowToolTip	=	true;
	return SPSendMessageToParent(SPIM_CURSOR_ENTER, wParam, lParam, this);	
}


int SPWindowMiniMapView::OnCursorOut( WPARAM wParam, LPARAM lParam)
{
	m_bShowToolTip	=	false;
	return SPSendMessageToParent(SPIM_CURSOR_OUT, wParam, lParam, this);
}

void SPWindowMiniMapView::IconClean()
{
	stdext::hash_map<INSTANCE_ID, SPWindowMiniMapIcon*>::iterator hmIter = m_hmGOBIcon.begin();
	for(; hmIter != m_hmGOBIcon.end(); ++hmIter) 
	{
		SAFE_DELETE((*hmIter).second);
	}
	m_hmGOBIcon.clear();

	m_LocalPlayer.Create( MINIMAP_ICON_MYPLAYER , NULL , this );
}

void SPWindowMiniMapView::IconProcess( float fTime )
{
	stdext::hash_map<INSTANCE_ID, SPWindowMiniMapIcon*>::iterator hmIter = m_hmGOBIcon.begin();
	for(; hmIter != m_hmGOBIcon.end(); ++hmIter) 
	{
		((*hmIter).second)->Process( fTime );
	}

	m_LocalPlayer.Process( fTime );

	//	ToolTip을 표시 여부 체크
	if( m_bShowToolTip == true )
		IconRectInCheck( g_pInterfaceManager->GetCursorX() , g_pInterfaceManager->GetCursorY() );
	else
		m_ToolTip.Hide();
}

void SPWindowMiniMapView::IconRender()
{
	stdext::hash_map<INSTANCE_ID, SPWindowMiniMapIcon*>::iterator hmIter = m_hmGOBIcon.begin();

	for(; hmIter != m_hmGOBIcon.end(); ++hmIter) 
	{
		((*hmIter).second)->Render();
	}

	//
	//	캐릭터를 제일 마지막에 렌더하자.
	m_LocalPlayer.Render();
}

void SPWindowMiniMapView::IconRectInCheck( int iCurX , int iCurY )
{
	m_ToolTip.Hide();

	char					strGOBName[MAX_PATH];
	SPWindowMiniMapIcon*	pIcon	=	NULL;
	stdext::hash_map<INSTANCE_ID, SPWindowMiniMapIcon*>::iterator hmIter = m_hmGOBIcon.begin();

	for(; hmIter != m_hmGOBIcon.end(); ++hmIter) 
	{
		pIcon	=	((*hmIter).second);

		if( pIcon->CheckInMouse( iCurX , iCurY ) == true )
			break;

		pIcon = NULL;
	}

	//	로컬 캐릭터를 제일 마지막에 표시하기 위해.
	if( m_LocalPlayer.CheckInMouse( iCurX , iCurY ) == true )
		pIcon	=	&m_LocalPlayer;

	if( pIcon == NULL )
		return;

	pIcon->GetGOBName( strGOBName );
	m_ToolTip.SetToolTipText( strGOBName );
	m_ToolTip.SetPosX( pIcon->GetPosX() );
	m_ToolTip.SetPosY( pIcon->GetPosY() );
	m_ToolTip.Show();
}

RECT SPWindowMiniMapView::GetMiniMapRect( int iLocalPlayerPosX , int iLocalPlayerPosY )
{
	RECT	rtMap ={0,}, rtReturn;
	int		iRealImgSizeX = m_iSX * MINIMAP_SCALE_X;
	int		iRealImgSizeY = m_iSY * MINIMAP_SCALE_Y;
	int		iWorldMiniMapX , iWorldMiniMapY;

	if(!m_pMapGroup)
		return rtMap;

	rtMap.left		=	0;
	rtMap.top		=	0;
	rtMap.right		=	static_cast<int>( m_pMapGroup->m_lGroupWidth ) - iRealImgSizeX;
	rtMap.bottom	=	static_cast<int>( m_pMapGroup->m_lGroupHeight ) - iRealImgSizeY;

	iWorldMiniMapX	=	iLocalPlayerPosX - iRealImgSizeX / 2;
	iWorldMiniMapY	=	iLocalPlayerPosY - iRealImgSizeY / 2;

	if( iWorldMiniMapX < rtMap.left )
		iWorldMiniMapX = rtMap.left;

	if( iWorldMiniMapX > rtMap.right )
		iWorldMiniMapX = rtMap.right;

	if( iWorldMiniMapY < rtMap.top )
		iWorldMiniMapY = rtMap.top;

	if( iWorldMiniMapY > rtMap.bottom )
		iWorldMiniMapY = rtMap.bottom;

	SetRect( &rtReturn , iWorldMiniMapX , iWorldMiniMapY , iWorldMiniMapX + iRealImgSizeX , iWorldMiniMapY + iRealImgSizeY );

	return rtReturn;
}

void SPWindowMiniMapView::InitSavePoint()
{
	LoadSavePoint();

	m_iMaxLine			=	181;
	m_iSaveLineOffSetX	=	4;
	m_iSaveLineOffSetY	=	3;
	m_iSavePointSizeX	=	3;

	m_pSaveLine		=	new	SPWindowStatic( m_InstanceID + 1 , m_iSaveLineOffSetX , m_iSaveLineOffSetY , m_iMaxLine , 6 , this );
	m_pSaveLine->SetImage( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 251 , 13 );

	m_pSavePlayer	=	new	SPWindowStatic( m_InstanceID + 2 , 0 , 0 , m_iSavePointSizeX * 2 , 6 , this );
	m_pSavePlayer->SetImage( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 429 , 1 );

	m_pSavePoint	=	new	SPWindowStatic( m_InstanceID + 3 , 0 , 0 , m_iSavePointSizeX * 2 , 6 , this );
	m_pSavePoint->SetImage( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 437 , 1 );

}

void SPWindowMiniMapView::LoadSavePoint()
{
	SPLDTFile*	pLDTFile	=	NULL;

	g_pResourceManager->GetLDTFromFileName( RES_FNAME_LDT_MAP_EVENT , &pLDTFile );

	if( pLDTFile == NULL )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "----------------------------------->LDT Load Fail : [%s] \n" , RES_FNAME_LDT_MAP_EVENT );
#endif
		return;
	}

	LDT_Field ldtFieldEventType;
	LDT_Field ldtFieldStageID;
	LDT_Field ldtFieldMapGroupID;
	LDT_Field ldtFieldMapID;
	LDT_Field ldtFieldPosX;
//	LDT_Field ldtFieldPosY;

	int				iRecordCount	= pLDTFile->GetItemCount();
	int				iItemID;
	SAVE_POINT		SavePoint;

	for( int iRecord = 0 ; iRecord < iRecordCount ; iRecord++ )
	{
		iItemID = pLDTFile->GetPrimaryKey( iRecord );

		pLDTFile->GetFieldFromLable( iItemID, "_EventType",				ldtFieldEventType		);

		if( ldtFieldEventType.uData.lValue != EVENT_SAVEPOINT )		//	SAVE_POINT만 읽는다.
			continue;

		pLDTFile->GetFieldFromLable( iItemID, "_StageID",				ldtFieldStageID			); 
		pLDTFile->GetFieldFromLable( iItemID, "_MapGroupID",			ldtFieldMapGroupID		); 
		pLDTFile->GetFieldFromLable( iItemID, "_MapID",					ldtFieldMapID			);
		pLDTFile->GetFieldFromLable( iItemID, "_PosX",					ldtFieldPosX			);
//		pLDTFile->GetFieldFromLable( iItemID, "_PosY",					ldtFieldPosY			);

		memset( &SavePoint, 0 , sizeof(SAVE_POINT) );

		SavePoint.uiStageID		=	(UINT16)ldtFieldStageID.uData.lValue;
		SavePoint.uiMapGroupID	=	(UINT16)ldtFieldMapGroupID.uData.lValue;
		SavePoint.uiMapID		=	(UINT16)ldtFieldMapID.uData.lValue;
		SavePoint.fPosX			=	(float)ldtFieldPosX.uData.lValue;
//		SavePoint.fPosY			=	(float)ldtFieldPosY.uData.lValue;
		SavePoint.bCal			=	FALSE;

		AddSavePoint( SavePoint );
	}

	SAFE_RELEASE( pLDTFile );
}

void SPWindowMiniMapView::AddSavePoint( SAVE_POINT& SavePointInfo )
{
	GU_ID	KeyGU_ID	=	GetGlobalID( (CLASS_ID)SavePointInfo.uiStageID , (INSTANCE_ID)SavePointInfo.uiMapGroupID );

	m_mmSavePoints.insert( STD_MULTIMAP_SAVEPOINT::value_type( KeyGU_ID , SavePointInfo ) );
}

void SPWindowMiniMapView::CleanSavePoint()
{
	m_mmSavePoints.clear();
}

void SPWindowMiniMapView::GetSavePoint( int iStageID , int iMapGroupID )
{
	m_vSavePoints.clear();
	GU_ID	SavePointKey	=	GetGlobalID( (CLASS_ID)iStageID , (INSTANCE_ID)iMapGroupID );
	int		iMaxPosX		=	m_iMaxLine - m_iSavePointSizeX;
	float	fPosX;

	STD_MULTIMAP_SAVEPOINT::iterator	mIter	=	m_mmSavePoints.find( SavePointKey );

	while( mIter != m_mmSavePoints.end() )
	{
		if( ((*mIter).first) == SavePointKey )
		{
			if( ((*mIter).second).bCal == FALSE )
			{
				if( ((*mIter).second).uiStageID == iStageID && ((*mIter).second).uiMapGroupID == iMapGroupID )
				{
					fPosX	=	((*mIter).second).fPosX	+	m_iMapWidth * ((*mIter).second).uiMapID;
					fPosX	=	(float)( (float)m_iMaxLine * ( fPosX / (float)m_iGroupWidth ) + m_iSaveLineOffSetX );

					if( (int)fPosX >= iMaxPosX )
						fPosX	=	(float)( iMaxPosX + m_iSaveLineOffSetX );

					((*mIter).second).fPosX	=	fPosX;
					((*mIter).second).bCal	=	TRUE;
				}

			}

			m_vSavePoints.push_back( &( (*mIter).second ) );
			mIter++;
		}
		else
			break;
	}
}

void SPWindowMiniMapView::RenderSavePoint( float fTime )
{
	m_pSaveLine->Render( fTime );

	STD_VECTOR_SAVEPOINT::iterator	Iter	=	m_vSavePoints.begin();

	while( Iter != m_vSavePoints.end() )
	{
		m_pSavePoint->AdjustCoord( (int)(*Iter)->fPosX , m_iSaveLineOffSetY );
		m_pSavePoint->Render( fTime );
		Iter++;
	}

	int	iPosX		=	(int)( (float)m_iMaxLine * ( (float)m_iPosX / (float)m_iGroupWidth ) + m_iSaveLineOffSetX );
	int	iMaxPosX	=	m_iMaxLine - m_iSavePointSizeX - m_iSavePointSizeX + m_iSaveLineOffSetX;

	if( iPosX >= iMaxPosX )
		iPosX = iMaxPosX;

	m_pSavePlayer->AdjustCoord( iPosX , m_iSaveLineOffSetY );
	m_pSavePlayer->Render( fTime );
}
