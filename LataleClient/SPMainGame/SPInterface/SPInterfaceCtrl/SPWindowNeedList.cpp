
#include <vector>
#include <string>
#include <tchar.h>

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
#include "SPInterfaceManager.h"

#include "SPDebug.h"
#include "_Error_log.h"

#include "SPAbuseFilter.h"

#include "SPWindow.h"
#include "SPWindowNeedList.h"

#include "SPItemAttr.h"
#include "SPItemCluster.h"


#define MAX_HISTORY_ITEM 100

SPWindowNeedList::SPWindowNeedList( INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow( WNDID_NEED_LIST, InstanceID, iX, iY, iCX, iCY, pParent )
, m_pTextureNum(NULL)
, m_pTextureBG(NULL)
{
	Init();
}

SPWindowNeedList::~SPWindowNeedList()
{
	Clean();
}

void SPWindowNeedList::Init()
{	
	m_strToolTip		= "[SPWindowNeedList]";
	m_dwAlign			= DT_LEFT;
	m_RGBA				= RGBA(0xFF,0xFF,0xFF,0x00);
	m_pTextureBG		= NULL ;
	m_bDrawLine			= false ;	// Guide Line

	m_iImageSX 			= 0 ;		// 이미지의 시작 위치
	m_iImageMAXWidth 	= 0 ;		// 이미지 영역 Width (Height는 개별)
	m_iTextSX 			= 0 ;		// Text의 시작 위치
	m_iTextMAXWidth 	= 0 ;		// Text 영역의 Width
	m_iWidthMargin 		= DEFAULT_MARGIN ;		// Image와 Text의 Margin
	m_iHeightMargin		= DEFAULT_MARGIN ;
	
	m_dwAlign = DT_LEFT | DT_VCENTER;

	g_pVideo->GetTextureMgr()->LoadTexture("DATA/INTERFACE/CONCEPT/UI_NUM.PNG", &m_pTextureNum) ;
	SetRect(&m_rcNumSrc, 0, 0, 8, 10) ;			// 0 (X += 9)
}

void SPWindowNeedList::Clean()
{
	SPWindow::Clean();

	m_iImageSX 			= 0 ;			// 이미지의 시작 위치
	m_iImageMAXWidth 	= 0 ;			// 이미지 영역 Width (Height는 개별)
	m_iTextSX 			= 0 ;			// Text의 시작 위치
	m_iTextMAXWidth 	= 0 ;			// Text 영역의 Width
	m_iWidthMargin 	= DEFAULT_MARGIN ;	// Image와 Text의 Margin
	m_iHeightMargin	= DEFAULT_MARGIN ;

	ClearListItem();

	SAFE_RELEASE(m_pTextureBG) ;
	SAFE_RELEASE(m_pTextureNum) ;
}




/**
	리스트의 각 항목 Destination 좌표 계산
	계산된 상대 좌표를 이용해서 실제 화면에 출력될 좌표를 계산한다.
*/
void SPWindowNeedList::ReposOwnImage()
{
	int iNextYPos = 0 ;
	int iItemCount = 0 ;
	int iDestX = 0 ;
	int iDestY = 0 ;
	LIST_NEEDITEM*	pCurItem = NULL ;
	std::vector<LIST_NEEDITEM*>::iterator	IterItem = m_vpNeedItem.begin() ;
	m_dwAlign = DT_LEFT | DT_VCENTER;

	while( IterItem != m_vpNeedItem.end() )
	{
		pCurItem	=	(*IterItem);

		iDestX = m_iAX + m_iImageSX + pCurItem->m_iRelSX ;
		iDestY = m_iAY + m_iHeightMargin + iNextYPos ;
		if( pCurItem != NULL )
		{
			SetRect(&(pCurItem->m_rtDstImage), iDestX, iDestY, iDestX + pCurItem->m_rtSrc.right - pCurItem->m_rtSrc.left, iDestY + pCurItem->m_rtSrc.bottom - pCurItem->m_rtSrc.top) ;
			//ErrorFLogs->Error_Log("Repose Image[%d][%d, %d, %d, %d]", iItemCount, pCurItem->m_rtDstImage.left, pCurItem->m_rtDstImage.top, pCurItem->m_rtDstImage.right, pCurItem->m_rtDstImage.bottom) ;
			iNextYPos += (m_iHeightMargin + pCurItem->m_rtSrc.bottom - pCurItem->m_rtSrc.top) ;

			// Item Text
			if( pCurItem->m_strText.empty() == false )
			{
				iDestX = m_iAX + m_iTextSX ;
				iDestY = pCurItem->m_rtDstImage.top ;
				m_pFont->SetColor( pCurItem->m_colText );
				SetRect(&(pCurItem->m_rtDstText), iDestX, iDestY, iDestX+pCurItem->m_iStrLength, pCurItem->m_rtDstImage.bottom) ;
				//ErrorFLogs->Error_Log("Repose Text[%d][%d, %d, %d, %d]", iItemCount, pCurItem->m_rtDstText.left, pCurItem->m_rtDstText.top, pCurItem->m_rtDstText.right, pCurItem->m_rtDstText.bottom) ;
			}
		}
		++iItemCount ;
		++IterItem;
	}
	m_iSY = iNextYPos ;		// 마지막 위치는 컨트롤의 세로값
}

void SPWindowNeedList::Process(float fTime)
{
	SPWindow::Process(fTime);	
}

void SPWindowNeedList::Render(float fTime)
{
	if(!m_bShow) return;

	if( g_pVideo == NULL || m_pFont == NULL )
		return;

	// Background
	RECT DestRect;
	RECT SrcRect ;
	SetRect(&DestRect, m_iAX, m_iAY, m_iAX+m_iSX, m_iAY+m_iSY) ;
	if(m_pTextureBG) {		
		g_pVideo->Flush();
		m_pTextureBG->RenderTexture( &DestRect, &m_rcBGSrc );
	}
	if( m_bDrawLine )
	{
		g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));	// Image
		g_pVideo->GetSysLine()->DrawRect(DestRect);
		g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
	
	g_pVideo->Flush();
	
	bool bShadow = m_pFont->IsShadow() ;		// Shadow Backup
	m_pFont->SetEffect( m_FontEffect );

	LIST_NEEDITEM*	pCurItem = NULL ;
	std::vector<LIST_NEEDITEM*>::iterator	IterItem = m_vpNeedItem.begin() ;

	while( IterItem != m_vpNeedItem.end() )
	{
		pCurItem	=	(*IterItem);
		if( pCurItem != NULL )
		{
			// Item 
			pCurItem->m_pImage->RenderTexture(&(pCurItem->m_rtDstImage), &(pCurItem->m_rtSrc) ) ;
			g_pVideo->Flush() ;

			// Item Count 
			if( pCurItem->m_ItemCount > 1 )
			{
				int iXPos = pCurItem->m_ItemCount ;
				SetRect(&DestRect, pCurItem->m_rtDstImage.left+16, pCurItem->m_rtDstImage.top+22, pCurItem->m_rtDstImage.left+24, pCurItem->m_rtDstImage.top+32) ;
				SetRect(&SrcRect, m_rcNumSrc.left+9*iXPos, m_rcNumSrc.top, m_rcNumSrc.right+9*iXPos, m_rcNumSrc.bottom) ;
				m_pTextureNum->RenderTexture(&DestRect, &SrcRect) ;
			}

			// Item Text
			if( pCurItem->m_strText.empty() == false )
			{
				m_pFont->SetColor( pCurItem->m_colText );
				m_pFont->RenderText( pCurItem->m_strText.c_str() , &(pCurItem->m_rtDstText) , m_dwAlign );
			}

			if( m_bDrawLine ) {
				g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));	// Image
				g_pVideo->GetSysLine()->DrawRect(pCurItem->m_rtDstImage);
				g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));	// Text
				g_pVideo->GetSysLine()->DrawRect(pCurItem->m_rtDstText);
				g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}
		
		++IterItem;
	}
	g_pVideo->Flush() ;
	m_pFont->SetShadow(bShadow) ;		// Shadow Restore
}


void SPWindowNeedList::ClearListItem()
{
	int iCount = (int)m_vpNeedItem.size() ;
	std::vector<LIST_NEEDITEM*>::iterator Iter = m_vpNeedItem.begin();
	for(; Iter != m_vpNeedItem.end(); ++Iter) {
		// SAFE_RELEASE((*Iter)->m_pImage);
		SAFE_DELETE(*Iter) ;
	}
	m_vpNeedItem.clear();
}

/**
	배경 Texture를 셋팅한다.
*/
bool SPWindowNeedList::SetBGImage(const char* pszImage, RECT rcImageSrc )
{
	if( pszImage == NULL )
		return false;

	if( g_pVideo->GetTextureMgr()->LoadTexture(pszImage, &m_pTextureBG) == false )
		return false;

	SetRect(&m_rcBGSrc, rcImageSrc.left, rcImageSrc.top, rcImageSrc.right, rcImageSrc.bottom) ;
	return true ;
}

/**
	일반 Image로 Item을 넣는경우
*/
bool SPWindowNeedList::InsertItem( const char* pszImage, RECT rcImageSrc, const char* pszItemText , D3DXCOLOR color )
{
	if( pszImage == NULL )
		return false;
	if( pszItemText == NULL )
		return false;

	LIST_NEEDITEM*	pAddListItem = new LIST_NEEDITEM;
	pAddListItem->SetImage( pszImage, rcImageSrc) ;
	pAddListItem->SetText(pszItemText) ;
	pAddListItem->SetTextColor(color.r, color.g, color.b, color.a) ;

	if( PushBackItem(pAddListItem) == false )
	{
		SAFE_DELETE(pAddListItem) ;
		return false ;
	}
	return true ;
}

/**
	ITEMID를 통해 넣는경우
*/
bool SPWindowNeedList::InsertItem( int iItemID, int iItemCount )
{
	if( iItemID < 0 )
		return false;

	POINT ptSrcPos ;
	RECT rcImageSrc ;
	std::string strItemName ;
	SPItemAttr* pItemAttr = NULL;
	TCHAR szItemName[64] ;
	LIST_NEEDITEM*	pAddListItem = new LIST_NEEDITEM;

	if( g_pItemCluster->GetItemInfo(iItemID, pItemAttr) ) {	
		

		SPTexture* pTexture = g_pItemCluster->GetItemTextureFromTextureInx( pItemAttr->m_iIcon, pItemAttr->m_iIconIndex, ptSrcPos) ;
		SetRect(&rcImageSrc, ptSrcPos.x, ptSrcPos.y, ptSrcPos.x + 32, ptSrcPos.y + 32) ;		// Item Image Size 32*32
		pAddListItem->SetImage( pTexture, rcImageSrc) ;
		if( iItemCount > 1 ) {
			_stprintf( szItemName, _T("%s x%d"), pItemAttr->m_strName.c_str(), iItemCount) ;
			pAddListItem->SetText(szItemName) ;
		}
		else	{
			pAddListItem->SetText(pItemAttr->m_strName.c_str()) ;
		}
		pAddListItem->m_ItemID = iItemID ;
		pAddListItem->m_ItemCount = iItemCount ;
		if( pItemAttr->m_bNameColor )
			pAddListItem->SetTextColor(pItemAttr->GetNameColor()) ;
		else
			pAddListItem->SetTextColor( GetItemColor( pItemAttr->m_eRareLimit) ) ;
		
	}
	else 
	{
		SAFE_DELETE(pAddListItem) ;
		return false ;
	}

	if( PushBackItem(pAddListItem) == false)
	{
		SAFE_DELETE(pAddListItem) ;
		return false ;
	}
	return true ;
}


//1. cheap ----- 검정색 ----- RGB [0. 0. 0]
//2. normal ----- 녹색 ------ RGB [44, 143, 53]
//3. abnormal ---하늘색 ----- RGB [26, 155, 209]
//4. rare ------- 노랑색 ----- RGB [222, 146, 27]
//5. legend ---- 빨강색 ----- RGB [255, 0, 0]
D3DXCOLOR SPWindowNeedList::GetItemColor(int iLareLimit)
{
	if( iLareLimit == ITEM_RARE_NORMAL )		return D3DXCOLOR(0, 0, 0, 1.0f) ;
	else if( iLareLimit == ITEM_RARE_MAGIC )	return D3DXCOLOR( 44/255, 143/255,  53/255, 1.0f) ;
	else if( iLareLimit == ITEM_RARE_RARE )		return D3DXCOLOR( 26/255, 155/255, 209/255, 1.0f) ;
	else if( iLareLimit == ITEM_RARE_ADD1 ) 	return D3DXCOLOR(222/255, 146/255,  27/255, 1.0f) ;
	else if( iLareLimit == ITEM_RARE_ADD2 )		return D3DXCOLOR(1.0f, 0, 0, 1.0f) ;
}

/**
	ListItem을 Vector에 PushBack
	1) ImageWidth 및 TextWidth 계산
	2) ImageSX, TextSX 계산
	3) pListItem의 Height 계산 (이미지의 가운데 정렬)
	## Image는 Text보다 높이가 높아야 한다. (Image가 없으면 DummyImage를 꼭 넣어야 한다.)
*/
bool SPWindowNeedList::PushBackItem( LIST_NEEDITEM* pListItem )
{
	int iWindowWidth = m_iSX ;
	int iImageWidth = 0 ;
	SIZE sizeString = {0, 0};
	TCHAR wszItemName[64] ;
	
	if( g_pVideo == NULL || m_pFont == NULL )
		return false ;
	if( pListItem == NULL )
		return false ;
	
	_tcscpy( wszItemName, pListItem->m_strText.c_str() );
	sizeString = m_pFont->GetSize( wszItemName );
	iImageWidth = pListItem->m_rtSrc.right - pListItem->m_rtSrc.left ;
	m_iImageMAXWidth = max(iImageWidth, m_iImageMAXWidth) ;
	m_iTextMAXWidth = max( sizeString.cx, m_iTextMAXWidth) ;
	if( iImageWidth == m_iImageMAXWidth )		// 새로 들어온 이미지가 크다 -> 기존 목록 수정
	{
		m_iImageSX	= ( iWindowWidth / 2 ) - ( (iImageWidth + m_iWidthMargin + m_iTextMAXWidth) / 2 ) ;
		m_iTextSX	= m_iImageSX + m_iImageMAXWidth + m_iWidthMargin ;
		pListItem->m_iRelSX	= 0 ;
		ReposeStartPos() ;
	}
	else
	{
		pListItem->m_iRelSX	= ( m_iImageMAXWidth / 2 ) - ( iImageWidth / 2 ) ;
	}
	
	pListItem->m_iStrLength = sizeString.cx + m_iWidthMargin ;

	m_vpNeedItem.push_back(pListItem) ;
	
	ReposOwnImage() ;
	SPSendMessageToParent(SPIM_LIST_RESIZE, 0, m_iSY, this);	// Resize를 부모에 알림
	return true;
}


/**
	이미지의 시작좌표로부터의 상대좌표의 변경이 필요한 경우
	Vector의 모든 항목에 적용한다.
*/
void SPWindowNeedList::ReposeStartPos()
{
	int iImageWidth = 0 ;
	std::vector<LIST_NEEDITEM*>::iterator Iter = m_vpNeedItem.begin();
	for(; Iter != m_vpNeedItem.end(); ++Iter) {
		iImageWidth = (*Iter)->m_rtSrc.right - (*Iter)->m_rtSrc.left ;
		(*Iter)->m_iRelSX	= m_iImageMAXWidth / 2 - iImageWidth / 2 ;
	}
}

/**
	리스트의 가로여백(WidthMargin), 세로 여백HeightMargin) 설정
*/
void SPWindowNeedList::SetMargin( int iWidthMargin, int iHeightMargin )
{
	if( iWidthMargin < 0 || iHeightMargin < 0 )
		return ;

	m_iWidthMargin	= iWidthMargin ;
	m_iHeightMargin = iHeightMargin ;
}


void SPWindowNeedList::RefreshRelationCoord()
{
	SPWindow::RefreshRelationCoord();
	ReposOwnImage();
}


SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowNeedList )
	SPIMESSAGE_COMMAND(	SPIM_PURGE			,	OnPurge			)
SPIMESSAGE_OBJ_MAP_END	( SPWindow	)




int SPWindowNeedList::OnPurge	( WPARAM wParam, LPARAM lParam)
{
	ClearListItem();
	return SPSendMessageToParent(SPIM_PURGE, wParam, lParam, this);
}



