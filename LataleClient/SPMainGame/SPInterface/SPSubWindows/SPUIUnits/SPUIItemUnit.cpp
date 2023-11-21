// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-7-15    18:33 
//***************************************************************************

#include "SPCommon.h"
#include "SPUIUnit.h"

#include "SPItemCluster.h"
#include "SPPlayerInvenArchive.h"
#include "SPGOBManager.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGameObject.h"
#include "SPPlayer.h"
#include "SPItemAttr.h"

#include "SPUIItemUnit.h"

SPUIItemUnit::SPUIItemUnit()
{
	Init();
}

SPUIItemUnit::~SPUIItemUnit()
{
	Clear();
}

//////////////////////////////////////////////////////////////////////////
//
//	기본 인터페이스
//

void SPUIItemUnit::Init()
{
	SPUIUnit::Init();

	m_iItemID			=	0;
	m_eItemType			=	CONTAINER_TYPE_INVALID;

	m_pBGTexture		=	NULL;

	m_rtBGSrc.left		=	0;
	m_rtBGSrc.top		=	0;
	m_rtBGSrc.right		=	0;
	m_rtBGSrc.bottom	=	0;

	m_iCount			=	0;

	m_iCountLength		=	0;

	for( int i = 0 ; i < ITEM_LENGTH_MAX ; i++ )
	{
		m_rtCountSrc[ i ].left		=	0;
		m_rtCountSrc[ i ].top		=	0;
		m_rtCountSrc[ i ].right		=	0;
		m_rtCountSrc[ i ].bottom	=	0;

		m_ptCountPos[ i ].x			=	0;
		m_ptCountPos[ i ].y			=	0;
	}

	m_rtIconSrc.left	=	0;
	m_rtIconSrc.top		=	0;
	m_rtIconSrc.right	=	0;
	m_rtIconSrc.bottom	=	0;

	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI100.PNG" , &m_pIconTexture );

	m_bDuration			=	FALSE;
}

void SPUIItemUnit::Clear()
{
	SPUIUnit::Clear();

	SAFE_RELEASE( m_pBGTexture );
	SAFE_RELEASE( m_pIconTexture );
}

void SPUIItemUnit::Process( float fTime )
{
	SPUIUnit::Process( fTime );
}

void SPUIItemUnit::Render()
{
	if( IsTexture() == FALSE || IsRender() == FALSE || g_pVideo == NULL )
		return;

	g_pVideo->Flush();

	if( m_pBGTexture )
	{
		m_pBGTexture->RenderTexture( m_ptPos.x , m_ptPos.y , &m_rtBGSrc );
		g_pVideo->Flush();
	}

	SPUIUnit::Render();

	if( m_pIconTexture != NULL )
	{
		m_pIconTexture->RenderTexture( m_ptPos.x , m_ptPos.y , &m_rtIconSrc );
		g_pVideo->Flush();
	}

	RenderCount();
}

BOOL SPUIItemUnit::Use()
{
	if( SPUIUnit::Use() == FALSE )
		return FALSE;

	if( g_pGOBManager->GetLocalPlayer() == NULL )
		return FALSE;

	SPPlayerInvenArchive*	pInvenArchive	=	g_pGOBManager->GetLocalPlayer()->GetInvenArchive();

	if( pInvenArchive == NULL )
		return FALSE;

	return	pInvenArchive->ItemQuickUse( m_eItemType , m_iItemID );
}

//////////////////////////////////////////////////////////////////////////
//
//	아이템 아이디 인터페이스
//

void SPUIItemUnit::SetItemID( int iItemID )
{
	m_iItemID	=	iItemID;
}

int SPUIItemUnit::GetItemID()
{
	return m_iItemID;
}

//////////////////////////////////////////////////////////////////////////
//
//	아이템 타입 인터페이스
//

void SPUIItemUnit::SetItemType( CONTAINER_TYPE eType )
{
	m_eItemType	=	eType;
}

CONTAINER_TYPE SPUIItemUnit::GetItemType()
{
	return m_eItemType;
}

//////////////////////////////////////////////////////////////////////////
//
//	자기 자신 복사 인터페이스
//

SPUIUnit* SPUIItemUnit::ClonePtr()
{
	return new SPUIItemUnit;
}

BOOL SPUIItemUnit::Copy( SPUIUnit** ppDestUIUnit )
{
	SPUIItemUnit*	pUIItemUnit	=	(SPUIItemUnit*)(*ppDestUIUnit);

	if( pUIItemUnit == NULL )
		return FALSE;

	// m_pIconTexture는 생성자에서 생성하기에 따로 값을 복사하지 않는다. [4/27/2006]

	SPTexture*		pBGTexture		=	pUIItemUnit->GetBGTexture();

	SAFE_RELEASE( pBGTexture );

	pUIItemUnit->SetBGTexture( pBGTexture );

	if( SPUIUnit::Copy( ppDestUIUnit ) == FALSE )
		return FALSE;

	pUIItemUnit->SetItemID( GetItemID() );
	pUIItemUnit->SetItemType( GetItemType() );

	pUIItemUnit->SetBGTexture( GetBGTexture() );
	pUIItemUnit->SetBGRectSrc( GetBGRectSrc() );

	pUIItemUnit->SetCount( GetCount() );

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//
//	아이디로 리소스 재 로드
//

BOOL SPUIItemUnit::ReResourceLoad()
{
	POINT		ptSrc;
	SPTexture*	pTexture = g_pItemCluster->GetItemTexture( m_iItemID , ptSrc );

	SPItemAttr* pItem;

	if( g_pItemCluster->GetItemInfo( m_iItemID , pItem ) == true )
		SetItemType( (CONTAINER_TYPE)(pItem->m_eType - 1) );

	if( pTexture == NULL )
		return FALSE;

	SetTexture( pTexture );

	m_rtSrc.left	=	ptSrc.x;
	m_rtSrc.top		=	ptSrc.y;
	m_rtSrc.right	=	ptSrc.x	+	m_ptSize.x;
	m_rtSrc.bottom	=	ptSrc.y	+	m_ptSize.y;

	switch( g_pItemCluster->GetItemIconMark( m_iItemID ) )
	{
		case ICON_MARK_LOCK:		SetRect( &m_rtIconSrc	, 479 , 152 , 511 , 184 );		break;
		case ICON_MARK_FASHION:		SetRect( &m_rtIconSrc	, 479 , 185 , 511 , 217 );		break;
		case ICON_MARK_EVENT:		SetRect( &m_rtIconSrc	, 479 , 339 , 511 , 371 );	    break;
		case ICON_MARK_FANCY:		SetRect( &m_rtIconSrc	, 479 , 372 , 511 , 404 );	    break;
		default:					SetRect( &m_rtIconSrc	, 0 , 0 , 0 , 0 );			    break;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//
//	아이템 백그라운드 텍스처 인터페이스
//

void SPUIItemUnit::SetBGTexture( SPTexture* pTexture )
{
	if( pTexture != NULL )
		SetBGTexture( pTexture->GetResName() );
	else
		m_pBGTexture	=	pTexture;
}

BOOL SPUIItemUnit::SetBGTexture( const char* pstrTexture )
{
	SAFE_RELEASE( m_pBGTexture );

	return g_pVideo->GetTextureMgr()->LoadTexture( pstrTexture , &m_pBGTexture );
}

SPTexture* SPUIItemUnit::GetBGTexture()
{
	return m_pBGTexture;
}

//////////////////////////////////////////////////////////////////////////
//
//	아이템 백그라운드 텍스처 영역 인터페이스
//

void SPUIItemUnit::SetBGRectSrc( RECT rtSrc )
{
	m_rtBGSrc		=	rtSrc;
}

void SPUIItemUnit::SetBGRectSrc( int ixLeft , int iyTop , int ixRight , int iyBottom )
{
	SetRect( &m_rtBGSrc , ixLeft , iyTop , ixRight , iyBottom );
}

RECT SPUIItemUnit::GetBGRectSrc()
{
	return m_rtBGSrc;
}

//////////////////////////////////////////////////////////////////////////
//
//	아이템 갯수 체크 인터페이스
//

void SPUIItemUnit::CheckCount()
{
	SPPlayer*	pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( pLocalPlayer == NULL )
		return;

	SPPlayerInvenArchive*	pInvenArchive	=	pLocalPlayer->GetInvenArchive();

	if( pInvenArchive == NULL )
		return;

	if( pInvenArchive->CheckPCStatus( m_iItemID , true ) == false )
		AddStatus( SPUIUnit::STATUS_UI_UNIT_DISABLE );
	else
		AddStatus( SPUIUnit::STATUS_UI_UNIT_ENABLE );

	SetCount( pInvenArchive->GetTotalStackCount( m_eItemType , m_iItemID, false ) );

	if( m_iCount <= 0 )
		AddStatus( SPUIUnit::STATUS_UI_UNIT_DISABLE );
}

void SPUIItemUnit::SetCount( int iCount )
{
	m_iCount	=	iCount;

	ProcessCount();
}

int SPUIItemUnit::GetCount()
{
	return m_iCount;
}

//////////////////////////////////////////////////////////////////////////
//
//	아이템 아이콘 텍스처 인터페이스
//

void SPUIItemUnit::SetIconTexture( SPTexture* pTexture )
{
	if( pTexture != NULL )
		SetIconTexture( pTexture->GetResName() );
	else
		m_pBGTexture	=	pTexture;
}

BOOL SPUIItemUnit::SetIconTexture( const char* pstrTexture )
{
	SAFE_RELEASE( m_pBGTexture );

	return g_pVideo->GetTextureMgr()->LoadTexture( pstrTexture , &m_pBGTexture );
}

SPTexture* SPUIItemUnit::GetIconTexture()
{
	return m_pBGTexture;
}

//////////////////////////////////////////////////////////////////////////
//
//	아이템 갯수 프로세서 인터페이스
//

void SPUIItemUnit::ProcessCount()
{
	if( m_iCount <= 0 )
	{
		m_iCountLength	=	0;
		return;
	}

	if( m_iCount > ITEM_COUNT_MAX )
		m_iCount = ITEM_COUNT_MAX;

	char		szBuf[_MAX_PATH]	=	{};
	std::string	strBufOneChiper;

	sprintf( szBuf , "%d" , m_iCount );

	int			iOneChiper;
	m_iCountLength		=	(int)strlen( szBuf );

	for( int i = 0 ; i < m_iCountLength ; ++i )
	{
		strBufOneChiper	=	szBuf[ i ];
		iOneChiper		=	atoi( strBufOneChiper.c_str() );

		m_rtCountSrc[ i ].left		= iOneChiper				*	9;
		m_rtCountSrc[ i ].right		= m_rtCountSrc[ i ].left	+	9;
		m_rtCountSrc[ i ].top		= 0;
		m_rtCountSrc[ i ].bottom	= 10;

		m_ptCountPos[ i ].x	=	m_ptPos.x	+	( m_rtCountSrc[ i ].right	-	m_rtCountSrc[ i ].left) * i;
		m_ptCountPos[ i ].y	=	m_ptPos.y	+	m_ptSize.y	-	( m_rtCountSrc[ i ].bottom	-	m_rtCountSrc[ i ].top );

		if( i != 0 )
			m_ptCountPos[ i ].x	-=	( 2 * i );
	}
}

//////////////////////////////////////////////////////////////////////////
//
//	아이템 기간 표시 유무
//

void SPUIItemUnit::SetDuration( BOOL bDuration )
{
	m_bDuration	=	bDuration;
}

BOOL SPUIItemUnit::GetDuration()
{
	return	m_bDuration;
}

//////////////////////////////////////////////////////////////////////////
//
//	아이템 갯수 렌더 인터페이스
//

void SPUIItemUnit::RenderCount()
{
	if( m_iCountLength <= 0 || g_pVideo == NULL || g_pItemCluster == NULL )
		return;

	SPTexture*	pNumTexture	=	g_pItemCluster->GetNumTexture();

	if( pNumTexture == NULL )
		return;

	g_pVideo->Flush();

	for( int i = 0 ; i < m_iCountLength ; i++ )
		pNumTexture->RenderTexture( m_ptCountPos[ i ].x , m_ptCountPos[ i ].y , &m_rtCountSrc[ i ] );
}
