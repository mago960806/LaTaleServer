
#include "SPCommon.h"
#include "SPUtil.h"
#include "SPOptionDef.h"
#include "SPClientOptionArchive.h"
#include "SPEventStatusModel.h"
#include "SPAbuseFilter.h"

//------------------------------------------------------------------------------------
SPEventStatusModel::SPEventStatusModel()
: m_pkTalkBalloon(NULL)
, m_pkShadowTexture(NULL)
{
	Init();
}

//------------------------------------------------------------------------------------
SPEventStatusModel::~SPEventStatusModel()
{
	Destroy();
}

//------------------------------------------------------------------------------------
void SPEventStatusModel::Destroy()
{
	SAFE_RELEASE(m_pkTalkBalloon);
	SAFE_RELEASE(m_pkShadowTexture);
}

//------------------------------------------------------------------------------------
void SPEventStatusModel::Init()
{
	m_NameColor			= D3DXCOLOR(0.858f, 0.796f, 1.0f, 1.0f);
	m_szName			= "";

	string strFileName;
	strFileName = "DATA/INTERFACE/CONCEPT/SHADOW.PNG";
	if( !g_pVideo->GetTextureMgr()->LoadTexture(strFileName.c_str(), &m_pkShadowTexture) )
		return;

	SetRect(&m_rcShadowSrcRgn, 0, 0, 64, 32);
	m_bShadowDraw = false;

	strFileName = "DATA/INTERFACE/CONCEPT/UI101.PNG";
	if( !g_pVideo->GetTextureMgr()->LoadTexture( strFileName.c_str(), &m_pkTalkBalloon ) )
		return;

	SetRect( &m_rcTalkBallonImageSrcRgn[ TALKBALLON_TOP		] ,	343,	484,	510,	490 );
	SetRect( &m_rcTalkBallonImageSrcRgn[ TALKBALLON_MIDDLE	] ,	343,	492,	510,	494 );
	SetRect( &m_rcTalkBallonImageSrcRgn[ TALKBALLON_BOTTOM	] ,	343,	496,	510,	512 );

	m_iTalkBalloonImageWidth		=	m_rcTalkBallonImageSrcRgn[ TALKBALLON_TOP ].right
										- m_rcTalkBallonImageSrcRgn[ TALKBALLON_TOP ].left;

	m_iTalkBalloonImageTopHeight	=	m_rcTalkBallonImageSrcRgn[ TALKBALLON_TOP ].bottom
										- m_rcTalkBallonImageSrcRgn[ TALKBALLON_TOP ].top;

	m_iTalkBalloonImageBottomHeight	=	m_rcTalkBallonImageSrcRgn[ TALKBALLON_BOTTOM ].bottom
										- m_rcTalkBallonImageSrcRgn[ TALKBALLON_BOTTOM ].top;

	m_iTalkBalloonTextWidth			=	m_iTalkBalloonImageWidth - 6;
	m_iTalkBalloonTextHeight		=	12;

	if( g_pVideo != NULL )
	{
		if( g_pVideo->GetFont(FONT_12_NORMAL) != NULL )
			m_iTalkBalloonTextHeight		=	(int)g_pVideo->GetFont(FONT_12_NORMAL)->GetHeight();
	}

	m_fTalkBalloonAccmulateTime		=	0.0f;
	m_fTalkBalloonLimitTime			=	5.0f;
	m_colorTalkBalloonTitle			=	D3DCOLOR_ARGB( 255 , 185 , 255 , 255 );
	m_colorTalkBalloonID			=	D3DCOLOR_ARGB( 255 , 255 , 200 , 220 );
	m_colorTalkBalloonText			=	D3DCOLOR_ARGB( 255 , 255 , 255 , 255 );

}

//------------------------------------------------------------------------------------
void SPEventStatusModel::Process(float fTime)
{
	// AJJIYA [8/25/2005]
	// 말풍선
	if( m_vTalkBalloonText.empty() == TRUE )
		return;

	m_fTalkBalloonAccmulateTime	+=	fTime;

	if( m_fTalkBalloonAccmulateTime > m_fTalkBalloonLimitTime )
		SetTalkBalloon( NULL );
}

//------------------------------------------------------------------------------------
void SPEventStatusModel::Render(float fTime)
{
	if( m_szName.empty() )
		return;

	if( g_pVideo == NULL )
		return;

	SPFont* pkFont = g_pVideo->GetFont(FONT_12_NORMAL);
	if( pkFont == NULL )
		return;

	if( m_vTalkBalloonText.empty() == FALSE && g_pClientOption->GetOptionStruct()->bSpeechNPC == true )
	{
		if( m_pkTalkBalloon != NULL )
		{
			m_pkTalkBalloon->SetColor();

			for( int i = 0; i < TALKBALLON_COUNT ; i++ )
				m_pkTalkBalloon->RenderTexture( &m_rcTalkBallonImageDestRgn[ i ] , &m_rcTalkBallonImageSrcRgn[ i ] , 0.0f );

			pkFont->SetColor( m_colorTalkBalloonID );
			pkFont->RenderText( m_szName.c_str(), &m_rcTalkBallonIDDestRgn, DT_VCENTER | DT_CENTER | DT_WORDBREAK );
			pkFont->SetColor();

			int	iStrCount	=	(int)m_vTalkBalloonText.size();
			int iRgnCount	=	(int)m_vTalkBallonTextDestRgn.size();

			pkFont->SetShadow(false);
			for( i = 0 ; i < iStrCount ; i++ )
			{
				pkFont->SetColor( m_colorTalkBalloonText );

				if( i < iRgnCount )
					pkFont->RenderText( m_vTalkBalloonText.at( i ).c_str(), &( m_vTalkBallonTextDestRgn.at( i ) ) , DT_VCENTER | DT_CENTER );

				pkFont->SetColor();
			}
			pkFont->SetShadow(true);
		}
	}	// 말풍선 끝
	else
	{
		if( g_pClientOption->GetOptionStruct()->bIndicateNPC == true )
		{
			pkFont->SetShadow();
			pkFont->SetColor( m_NameColor );			
			pkFont->RenderText( m_szName.c_str() , &m_rcNameRgn , DT_VCENTER | DT_CENTER );
			pkFont->SetColor();
		}
	}

	if( m_pkShadowTexture && m_bShadowDraw )
		m_pkShadowTexture->RenderTexture(&m_rcShadowDstRgn, &m_rcShadowSrcRgn, 0.0f);

	g_pVideo->Flush();
}

//------------------------------------------------------------------------------------
void SPEventStatusModel::SetObjectRgn(RECT& rcRgn, int iCenterPosX, int iCenterPosY, bool bShadow)
{
	// Name
	SPFont* pkFont = g_pVideo->GetFont(FONT_12_NORMAL);
	if( pkFont == NULL )
		return;
	
	int iFontWidth	= pkFont->GetWidth();
	int iFontHeight	= (int)pkFont->GetHeight();

	

	// 이름
	int iLength = (int)m_szName.size();

	if( iFontWidth == 0 )
		iFontWidth = iFontHeight;

	int iNameWidth = iFontWidth * iLength;

	m_rcNameRgn.left	= iCenterPosX - (iNameWidth >> 1);
	m_rcNameRgn.right	= m_rcNameRgn.left + iNameWidth;
	//m_rcNameRgn.top		= iCenterPosY - 155;			//	캐릭터 이름 OFFSET
	m_rcNameRgn.top		= rcRgn.top - iFontHeight;			//	MapEvent Size정보로 높이 설정
	m_rcNameRgn.bottom	= m_rcNameRgn.top + iFontHeight;

	// 그림자
	m_bShadowDraw = bShadow;
	if( bShadow )
	{
		int iWidth = m_rcShadowSrcRgn.right - m_rcShadowSrcRgn.left;
		int iHeight = m_rcShadowSrcRgn.bottom - m_rcShadowSrcRgn.top;

		m_rcShadowDstRgn.left = iCenterPosX - (iWidth >> 1);
		m_rcShadowDstRgn.top = iCenterPosY - (iHeight >> 1);
		m_rcShadowDstRgn.right = m_rcShadowDstRgn.left + iWidth;
		m_rcShadowDstRgn.bottom = m_rcShadowDstRgn.top + iHeight;
	}

	// AJJIYA [8/25/2005]
	// 말풍선

	if( m_vTalkBalloonText.empty() == FALSE )
	{
		int		iTalkFontHeight	=	iFontHeight + 1;
		int		iStrCount		=	(int)m_vTalkBalloonText.size();
		int		iLineCount		=	iStrCount + 1;		//	타이틀 사용 안함

		for( int i = 0 ; i < TALKBALLON_COUNT ; i++ )
		{
			m_rcTalkBallonImageDestRgn[ i ].left	=	iCenterPosX - ( m_iTalkBalloonImageWidth >> 1 );
			m_rcTalkBallonImageDestRgn[ i ].right	=	m_rcTalkBallonImageDestRgn[ i ].left + m_iTalkBalloonImageWidth;
		}

		m_rcTalkBallonImageDestRgn[ TALKBALLON_BOTTOM ].bottom	=	m_rcNameRgn.top;
		m_rcTalkBallonImageDestRgn[ TALKBALLON_BOTTOM ].top		=	m_rcTalkBallonImageDestRgn[ TALKBALLON_BOTTOM ].bottom - m_iTalkBalloonImageBottomHeight;

		m_rcTalkBallonImageDestRgn[ TALKBALLON_MIDDLE ].bottom	=	m_rcTalkBallonImageDestRgn[ TALKBALLON_BOTTOM ].top;
		m_rcTalkBallonImageDestRgn[ TALKBALLON_MIDDLE ].top		=	m_rcTalkBallonImageDestRgn[ TALKBALLON_MIDDLE ].bottom - iLineCount * iTalkFontHeight;

		m_rcTalkBallonImageDestRgn[ TALKBALLON_TOP ].bottom		=	m_rcTalkBallonImageDestRgn[ TALKBALLON_MIDDLE ].top;
		m_rcTalkBallonImageDestRgn[ TALKBALLON_TOP ].top		=	m_rcTalkBallonImageDestRgn[ TALKBALLON_TOP ].bottom - m_iTalkBalloonImageTopHeight;


		m_vTalkBallonTextDestRgn.clear();

		RECT	rcTalkBallonTextDestRgn;

		rcTalkBallonTextDestRgn.left	= iCenterPosX - ( m_iTalkBalloonTextWidth >> 1 );
		rcTalkBallonTextDestRgn.right	= rcTalkBallonTextDestRgn.left + m_iTalkBalloonTextWidth;
		rcTalkBallonTextDestRgn.top		= m_rcTalkBallonImageDestRgn[ TALKBALLON_MIDDLE ].bottom;
		rcTalkBallonTextDestRgn.bottom	= m_rcTalkBallonImageDestRgn[ TALKBALLON_MIDDLE ].bottom + iTalkFontHeight;

		for( i = 0 ; i < iStrCount ; i++ )
		{
			rcTalkBallonTextDestRgn.top		-= iTalkFontHeight;
			rcTalkBallonTextDestRgn.bottom	-= iTalkFontHeight;

			m_vTalkBallonTextDestRgn.insert( m_vTalkBallonTextDestRgn.begin() , rcTalkBallonTextDestRgn );
		}

		m_rcTalkBallonIDDestRgn.left		= rcTalkBallonTextDestRgn.left;
		m_rcTalkBallonIDDestRgn.right		= rcTalkBallonTextDestRgn.right;
		m_rcTalkBallonIDDestRgn.top			= rcTalkBallonTextDestRgn.top - iTalkFontHeight;
		m_rcTalkBallonIDDestRgn.bottom		= rcTalkBallonTextDestRgn.top;

		//m_rcTalkBallonTitleDestRgn.left		= m_rcTalkBallonIDDestRgn.left;
		//m_rcTalkBallonTitleDestRgn.right	= m_rcTalkBallonIDDestRgn.right;
		//m_rcTalkBallonTitleDestRgn.top		= m_rcTalkBallonImageDestRgn[ TALKBALLON_TOP ].bottom;
		//m_rcTalkBallonTitleDestRgn.bottom	= m_rcTalkBallonIDDestRgn.top;

	}	// 말풍선 끝
}

//------------------------------------------------------------------------------------
void SPEventStatusModel::SetObjectName(const char* pszName)
{
	m_szName = pszName;
}

//------------------------------------------------------------------------------------
void SPEventStatusModel::SetTalkBalloon(const char* pszString)
{
	m_fTalkBalloonAccmulateTime	=	0.0f;

	if( pszString == NULL )
	{
		m_vTalkBalloonText.clear();
		return;
	}

	if( g_pVideo == NULL )
		return;

	if( g_pVideo->GetFont(FONT_12_NORMAL) == NULL )
		return;

	RECT	rcDest;

	SetRect( &rcDest , 0 , 0 , m_iTalkBalloonTextWidth , m_iTalkBalloonTextHeight );

	m_vTalkBalloonText.clear();

	char szLineFeedBuf[MAX_PATH];
	SPAbuseFilter::GetInstance()->ReplaceLineFeed(pszString, szLineFeedBuf, MAX_PATH);
	g_pVideo->GetFont(FONT_12_NORMAL)->ConvertMultiLineW(szLineFeedBuf, rcDest, m_vTalkBalloonText);

	int	iTalkStringLength	= (int)strlen(szLineFeedBuf);
	if( iTalkStringLength > 50 )
		m_fTalkBalloonLimitTime	=	8.0f;
	else
		m_fTalkBalloonLimitTime	=	6.0f;

}
