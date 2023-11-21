// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-6-28    14:43 
//***************************************************************************

#include "SPCommon.h"
#include "SPUtil.h"
#include "SPGameObjectDEF.h"
#include "SPResourceManager.h"
#include <string>
#include "SPCoolTimeManager.h"
#include "SPUIUnit.h"

SPUIUnit::SPUIUnit()
{
	Init();
}

SPUIUnit::~SPUIUnit()
{
	Clear();
}

//////////////////////////////////////////////////////////////////////////
//
//	기본인터페이스
//

void SPUIUnit::Init()
{
	m_dwStatus					=	STATUS_UI_UNIT_NONE;

	m_pTexture					=	NULL;
	m_pDisableTexture			=	NULL;
	m_pCoolTimeTexture			=	NULL;

	m_ptPos.x					=	0;
	m_ptPos.y					=	0;

	m_ptSize.x					=	0;
	m_ptSize.y					=	0;
	
	m_rtSrc.left				=	0;
	m_rtSrc.top					=	0;
	m_rtSrc.right				=	0;
	m_rtSrc.bottom				=	0;

	m_rtDisableSrc.left			=	0;
	m_rtDisableSrc.top			=	0;
	m_rtDisableSrc.right		=	0;
	m_rtDisableSrc.bottom		=	0;

	m_rtDisableTarget.left		=	0;
	m_rtDisableTarget.top		=	0;
	m_rtDisableTarget.right		=	0;
	m_rtDisableTarget.bottom	=	0;

	m_rtCoolTimeSrc.left		=	0;
	m_rtCoolTimeSrc.top			=	0;
	m_rtCoolTimeSrc.right		=	0;
	m_rtCoolTimeSrc.bottom		=	0;

	m_rtCoolTimeTarget.left		=	0;
	m_rtCoolTimeTarget.top		=	0;
	m_rtCoolTimeTarget.right	=	0;
	m_rtCoolTimeTarget.bottom	=	0;

	m_cRGBA.r					=	1.0f;
	m_cRGBA.g					=	1.0f;
	m_cRGBA.b					=	1.0f;
	m_cRGBA.a					=	1.0f;

	m_cDisableRGBA.r			=	1.0f;
	m_cDisableRGBA.g			=	1.0f;
	m_cDisableRGBA.b			=	1.0f;
	m_cDisableRGBA.a			=	1.0f;

	m_iCoolTimeID				=	-1;
}

void SPUIUnit::Clear()
{
	SAFE_RELEASE( m_pTexture );
	SAFE_RELEASE( m_pDisableTexture );
	SAFE_RELEASE( m_pCoolTimeTexture );
}

void SPUIUnit::Process( float fTime )
{
	m_cRGBA.a		=	1.0f;

	if( GetStatus( STATUS_UI_UNIT_DRAGNDROP ) == TRUE )
	{
		m_cRGBA.a	=	0.5f;
	}

	if( GetStatus( STATUS_UI_UNIT_DISABLE ) == TRUE )
	{
		DisablePorcess();
	}

	if( m_iCoolTimeID == - 1 )
		DelStatus( STATUS_UI_UNIT_COOLTIME );
	else
		CoolTimeProcess( fTime );

}

void SPUIUnit::Render()
{
	if( IsTexture() == FALSE || IsRender() == FALSE || g_pVideo == NULL )
		return;

	m_pTexture->SetColor( m_cRGBA );
	m_pTexture->RenderTexture( m_ptPos.x , m_ptPos.y , &m_rtSrc );
	g_pVideo->Flush();

	if( GetStatus( STATUS_UI_UNIT_DISABLE ) == TRUE && IsDisableTexture() == TRUE )
	{
		m_pDisableTexture->SetColor( m_cDisableRGBA );
		m_pDisableTexture->RenderTexture( &m_rtDisableTarget , &m_rtDisableSrc );
		g_pVideo->Flush();
	}

	if( GetStatus( STATUS_UI_UNIT_COOLTIME ) == TRUE && IsCoolTimeTexture() == TRUE )
	{
		m_pCoolTimeTexture->SetColor();
		m_pCoolTimeTexture->RenderTexture( &m_rtCoolTimeTarget , &m_rtCoolTimeSrc );
		g_pVideo->Flush();
	}

	m_pTexture->SetColor();
}

BOOL SPUIUnit::Use()
{
	return GetStatus( STATUS_UI_UNIT_ENABLE );
}

BOOL SPUIUnit::MouseOver( int iX , int iY )
{
	if( IsRender() == FALSE )
		return FALSE;

	if( IsRectIn( iX , iY ) == FALSE )
		return FALSE;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//
//	InstanceID 인터페이스
//

void SPUIUnit::SetInstanceID( INSTANCE_ID iID )
{
	m_iInstanceID	=	iID;
}

INSTANCE_ID SPUIUnit::GetInstanceID()
{
	return m_iInstanceID;
}

//////////////////////////////////////////////////////////////////////////
//
//	Status 인터페이스
//

void SPUIUnit::SetStatus( DWORD dwStatus )
{
	m_dwStatus	=	dwStatus;
}

void SPUIUnit::AddStatus( DWORD dwStatus )
{
	if( ( dwStatus & STATUS_UI_UNIT_MASK ) == STATUS_UI_UNIT_NONE )
	{
		m_dwStatus	=	m_dwStatus & STATUS_UI_UNIT_MASK | dwStatus;
	}
	else
	{
		m_dwStatus	|=	dwStatus;
	}
}

void SPUIUnit::DelStatus( DWORD dwStatus )
{
	if( GetStatus( dwStatus ) == TRUE )
		m_dwStatus	^=	dwStatus;
}

BOOL SPUIUnit::GetStatus( DWORD dwStatus )
{
	return ( ( m_dwStatus & dwStatus ) == dwStatus );
}

DWORD SPUIUnit::GetStatus()
{
	return m_dwStatus;
}

//////////////////////////////////////////////////////////////////////////
//
//	리소스 세팅 인터페이스
//

BOOL SPUIUnit::SetResource( int iResID , int iResIndex )
{
	if( iResID <= 0 || iResIndex <= 0 )
		return FALSE;

	const char*	szFileName	=	g_pResourceManager->GetGlobalFileName( iResID );

	if( szFileName == NULL )
		return FALSE;

	std::string strResourceFile = szFileName;

	if( strResourceFile.empty() )
		return FALSE;

	if( SetTexture( strResourceFile.c_str() ) == FALSE )
		return FALSE;

	//	시작은 0이 아닌 1부터 시작하기에 1을 빼준다!!!
	iResIndex--;

	int iIconSizeX = 32;
	int	iIconSizeY = 32;

	int iIconWidth	= m_pTexture->GetLenX() / iIconSizeX;
	int iIconYIndex = iResIndex / iIconWidth;
	int iIconXIndex = iResIndex - iIconYIndex * iIconWidth;

	RECT	rtSrc;

	rtSrc.left		=	iIconXIndex * iIconSizeX;
	rtSrc.top		=	iIconYIndex * iIconSizeY;
	rtSrc.right		=	rtSrc.left	+ iIconSizeX;
	rtSrc.bottom	=	rtSrc.top	+ iIconSizeY;

	SetRectSrc( rtSrc );

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//
//	텍스쳐 렌더 영역 인터페이스
//

void SPUIUnit::SetRectSrc( RECT rtSrc )
{
	m_rtSrc		=	rtSrc;
	m_ptSize.x	=	m_rtSrc.right	-	m_rtSrc.left;
	m_ptSize.y	=	m_rtSrc.bottom	-	m_rtSrc.top;
}

void SPUIUnit::SetRectSrc( int ixLeft , int iyTop , int ixRight , int iyBottom )
{
	SetRect( &m_rtSrc , ixLeft , iyTop , ixRight , iyBottom );
	m_ptSize.x	=	m_rtSrc.right	-	m_rtSrc.left;
	m_ptSize.y	=	m_rtSrc.bottom	-	m_rtSrc.top;
}

RECT SPUIUnit::GetRectSrc()
{
	return m_rtSrc;
}

//////////////////////////////////////////////////////////////////////////
//
//	텍스쳐 렌더 위치 인터페이스
//

void SPUIUnit::SetPos( POINT ptPos )
{
	m_ptPos	=	ptPos;
}

void SPUIUnit::SetPos( LONG lX , LONG lY )
{
	m_ptPos.x	=	lX;
	m_ptPos.y	=	lY;
}

POINT SPUIUnit::GetPos()
{
	return m_ptPos;
}

//////////////////////////////////////////////////////////////////////////
//
//	텍스쳐 사이즈 인터페이스
//

void SPUIUnit::SetSize( POINT ptSize )
{
	m_ptSize	=	ptSize;
}

void SPUIUnit::SetSize( LONG lX , LONG lY )
{
	m_ptSize.x	=	lX;
	m_ptSize.y	=	lY;
}

POINT SPUIUnit::GetSize()
{
	return m_ptSize;
}

//////////////////////////////////////////////////////////////////////////
//
//	렌더 텍스쳐 인터페이스
//

void SPUIUnit::SetTexture( SPTexture* pTexture )
{
	if( pTexture != NULL )
		SetTexture( pTexture->GetResName() );
	else
		m_pTexture	=	pTexture;
}

BOOL SPUIUnit::SetTexture( const char* pstrTexture )
{
	if( m_pTexture != NULL )
	{
		std::string	strTextureName;

		GetUpperString3( strTextureName , pstrTexture );

		if( strTextureName.compare( m_pTexture->GetResName() ) == 0 )
			return TRUE;
	}

	SAFE_RELEASE( m_pTexture );

	return g_pVideo->GetTextureMgr()->LoadTexture( pstrTexture , &m_pTexture );
}

SPTexture* SPUIUnit::GetTexture()
{
	return m_pTexture;
}

//////////////////////////////////////////////////////////////////////////
//
//	비활성화 텍스쳐 렌더 영역 인터페이스
//

void SPUIUnit::SetDisableRectSrc( RECT rtSrc )
{
	m_rtDisableSrc		=	rtSrc;
}

void SPUIUnit::SetDisableRectSrc( int ixLeft , int iyTop , int ixRight , int iyBottom )
{
	SetRect( &m_rtDisableSrc , ixLeft , iyTop , ixRight , iyBottom );
}

RECT SPUIUnit::GetDisableRectSrc()
{
	return m_rtDisableSrc;
}

//////////////////////////////////////////////////////////////////////////
//
//	비활성화 텍스쳐 인터페이스
//

void SPUIUnit::SetDisableTexture( SPTexture* pTexture )
{
	if( pTexture != NULL )
		SetDisableTexture( pTexture->GetResName() );
	else
		m_pDisableTexture	=	pTexture;
}

BOOL SPUIUnit::SetDisableTexture( const char* pstrTexture )
{
	SAFE_RELEASE( m_pDisableTexture );

	return g_pVideo->GetTextureMgr()->LoadTexture( pstrTexture , &m_pDisableTexture );
}

SPTexture* SPUIUnit::GetDisableTexture()
{
	return m_pDisableTexture;
}

//////////////////////////////////////////////////////////////////////////
//
//	비활성화 컬러 인터페이스
//

void SPUIUnit::SetDisableColor( D3DXCOLOR cColor )
{
	m_cDisableRGBA	=	cColor;
}

D3DXCOLOR SPUIUnit::GetDisableColor()
{
	return m_cDisableRGBA;
}

//////////////////////////////////////////////////////////////////////////
//
//	자기 자신 복사 인터페이스
//

SPUIUnit* SPUIUnit::ClonePtr()
{
	return new SPUIUnit;
}

BOOL SPUIUnit::Copy( SPUIUnit** ppDestUIUnit )
{
	if( ppDestUIUnit == NULL || *ppDestUIUnit == NULL )
		return FALSE;

	POINT	ptPos	=	(*ppDestUIUnit)->GetPos();

	SPTexture*	pTexture			=	(*ppDestUIUnit)->GetTexture();
	SPTexture*	pDisableTexture		=	(*ppDestUIUnit)->GetDisableTexture();
	SPTexture*	pCoolTimeTexture	=	(*ppDestUIUnit)->GetCoolTimeTexture();

	SAFE_RELEASE( pTexture );
	SAFE_RELEASE( pDisableTexture );
	SAFE_RELEASE( pCoolTimeTexture );

	//	텍스처를 NULL로 세팅 한다.
	(*ppDestUIUnit)->SetTexture( pTexture );
	(*ppDestUIUnit)->SetDisableTexture( pDisableTexture );
	(*ppDestUIUnit)->SetCoolTimeTexture( pCoolTimeTexture );

	// 데이터를 카피하자. [4/27/2006]
	(*ppDestUIUnit)->SetInstanceID( GetInstanceID() );
	(*ppDestUIUnit)->SetStatus( GetStatus() );

	(*ppDestUIUnit)->SetTexture( GetTexture() );
	(*ppDestUIUnit)->SetRectSrc( GetRectSrc() );
	(*ppDestUIUnit)->SetSize( GetSize() );
	(*ppDestUIUnit)->SetPos( ptPos );

	(*ppDestUIUnit)->SetDisableTexture( GetDisableTexture() );
	(*ppDestUIUnit)->SetDisableRectSrc( GetDisableRectSrc() );

	(*ppDestUIUnit)->SetCoolTimeID( GetCoolTimeID() );
	(*ppDestUIUnit)->SetCoolTimeTexture( GetCoolTimeTexture() );
	(*ppDestUIUnit)->SetCoolTimeRectSrc( GetCoolTimeRectSrc() );

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//
//	아이디로 리소스 재 로드
//

BOOL SPUIUnit::ReResourceLoad()
{
	char			szResPath[]		=	"DATA/INTERFACE/CONCEPT/UI100.PNG";

	SetTexture( szResPath );
	SetRectSrc( 400 , 20 , 432 , 52 );

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//
//	CoolTime ID 인터페이스
//

void SPUIUnit::SetCoolTimeID( int iID )
{
	m_iCoolTimeID	=	iID;
}

int SPUIUnit::GetCoolTimeID()
{
	return m_iCoolTimeID;
}

//////////////////////////////////////////////////////////////////////////
//
//	CoolTime 텍스처 인터페이스
//

void SPUIUnit::SetCoolTimeTexture( SPTexture* pTexture )
{
	if( pTexture != NULL )
		SetCoolTimeTexture( pTexture->GetResName() );
	else
		m_pCoolTimeTexture	=	pTexture;
}

BOOL SPUIUnit::SetCoolTimeTexture( const char* pstrTexture )
{
	SAFE_RELEASE( m_pCoolTimeTexture );

	return g_pVideo->GetTextureMgr()->LoadTexture( pstrTexture , &m_pCoolTimeTexture );
}

SPTexture* SPUIUnit::GetCoolTimeTexture()
{
	return m_pCoolTimeTexture;
}

//////////////////////////////////////////////////////////////////////////
//
//	CoolTime 텍스처 렌더 영역 인터페이스
//

void SPUIUnit::SetCoolTimeRectSrc( RECT rtSrc )
{
	m_rtCoolTimeSrc		=	rtSrc;
}

void SPUIUnit::SetCoolTimeRectSrc( int ixLeft , int iyTop , int ixRight , int iyBottom )
{
	SetRect( &m_rtCoolTimeSrc , ixLeft , iyTop , ixRight , iyBottom );
}

RECT SPUIUnit::GetCoolTimeRectSrc()
{
	return m_rtCoolTimeSrc;
}

//////////////////////////////////////////////////////////////////////////
//
//	기타 인터페이스
//

BOOL SPUIUnit::IsRender()
{
	return GetStatus( STATUS_UI_UNIT_RENDER );
}

BOOL SPUIUnit::IsTexture()
{
	if( m_pTexture == NULL )
		return FALSE;

	return TRUE;
}

BOOL SPUIUnit::IsDisableTexture()
{
	if( m_pDisableTexture == NULL )
		return FALSE;

	return TRUE;
}

BOOL SPUIUnit::IsCoolTimeTexture()
{
	if( m_pCoolTimeTexture == NULL )
		return FALSE;

	return TRUE;
}

BOOL SPUIUnit::IsRectIn( int iX , int iY )
{
	int	iEX	=	m_ptPos.x	+	m_ptSize.x;
	int iEY	=	m_ptPos.y	+	m_ptSize.y;

	if( iX >= m_ptPos.x && iY >= m_ptPos.y && iX <= iEX && iY <= iEY )
		return TRUE;

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
//
//	프로세서 인터페이스
//

void SPUIUnit::DisablePorcess()
{
	SetRect( &m_rtDisableTarget , m_ptPos.x , m_ptPos.y , m_ptPos.x + m_ptSize.x , m_ptPos.y + m_ptSize.y );
}

void SPUIUnit::CoolTimeProcess( float fTime )
{
	SPCoolTimeManager*	pCoolTimeManager	=	SPCoolTimeManager::GetInstance();
	COOLTIME_INFO*		pCoolTimeInfo		=	pCoolTimeManager->GetCoolTime( m_iCoolTimeID );

	if( pCoolTimeInfo == NULL )
		DelStatus( STATUS_UI_UNIT_COOLTIME );
	else
	{
		AddStatus( STATUS_UI_UNIT_COOLTIME );

		float	fRatio	=	pCoolTimeInfo->fAccmulateTime	/	pCoolTimeInfo->fCoolTime;
		int		iSrcY	=	m_ptSize.y	-	(int)( m_ptSize.y	*	fRatio );

		m_rtCoolTimeTarget.right	=	m_ptPos.x	+	m_ptSize.x;
		m_rtCoolTimeTarget.bottom	=	m_ptPos.y	+	m_ptSize.y;
		m_rtCoolTimeTarget.left		=	m_ptPos.x;
		m_rtCoolTimeTarget.top		=	m_rtCoolTimeTarget.bottom	-	iSrcY;
	}
}
