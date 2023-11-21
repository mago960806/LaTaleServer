
#include "SPCommon.h"
#include "SPTexture.h"
#include "SPUtil.h"
#include "SPOptionDef.h"
#include "SPClientOptionArchive.h"
#include "PacketID.h"

#include "SPTitleAttr.h"
#include "SPTitleArchive.h"
#include "SPTitleRenderDEF.h"
#include "SPTitleRenderBase.h"
#include "SPTitleManager.h"

#include "SPPlayerStatusModel.h"


#include "SPGOBStatus.h"
#include "SPGameObject.h"
#include "SPGOBCoordPhysics.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPPlayerStatus.h"
#include "SPPlayer.h"

#include "SPCommandConvert.h"
#include "SPEventDEF.h"
#include "SPUserShopAttr.h"
#include "SPUserShopArchive.h"
#include "SPEventManager.h"
#include "SPAbuseFilter.h"

#include "SPRenderModelDEF.h"
#include "SPRenderModel.h"
#include "SPFXDEF.h"
#include "SPFXModel.h"
#include "SPFXUnit.h"
#include "SPFXFrameAnimationUnit.h"
#include "SPFXManager.h"
#include "SPFXDamageUnit.h"
#include "SPFXGroupUnit.h"
#include "SPFXArchive.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPGuildArchive.h"
#include "SPGuildLDT.h"
#include "SPWindowGuildEmblem.h"
#include "SPLoveManager.h"

//------------------------------------------------------------------------------------
SPPlayerStatusModel::SPPlayerStatusModel(SPPlayer* pkParent)
: m_pkParent(pkParent)
, m_pkGageTexture(NULL)
, m_pkShadowTexture(NULL)
, m_pkGuildIconTexture(NULL)
, m_pkGuildIconBGTexture(NULL)
, m_bLocal(false)
, m_bLocalPlayerTeam(false)
, m_pkGuildFxArchive(NULL) 
, m_bSetGuildFx(false)
{
	Init();
}

//------------------------------------------------------------------------------------
SPPlayerStatusModel::~SPPlayerStatusModel()
{
	Destroy();
}

//------------------------------------------------------------------------------------
void SPPlayerStatusModel::Destroy()
{
	SAFE_RELEASE(m_pkGageTexture);
	SAFE_RELEASE(m_pkShadowTexture);
	SAFE_RELEASE(m_pkTalkBalloon);
	SAFE_RELEASE(m_pkLevel);
	SAFE_RELEASE(m_pkGuildIconTexture);
	SAFE_RELEASE(m_pkGuildIconBGTexture);
	SAFE_DELETE(m_pkGuildFxArchive);

	SAFE_DELETE( m_pTiTleRender );
}

//------------------------------------------------------------------------------------
void SPPlayerStatusModel::Init()
{
	
	SetRect(&m_rcHPSrcRgn, 0, 11, 43, 15);
	SetRect(&m_rcHPBKSrcRgn, 0, 4, 46, 11);

	SetRect(&m_rcShadowSrcRgn, 0, 0, 64, 32);

	m_ptHPOffset.x = 1;
	m_ptHPOffset.y = 1;

	m_NameColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_szName = "";
	m_uiMaxHP = 0;
	m_uiCurHP = 0;
	m_fHPRatio = 0.0f;
	m_bShowDraw = false;
	m_bShowName = true;
	m_bShowHP = true;
	m_bShowShadow = true;
	m_iBeforeGuildFx = 0 ;
	m_iCenterPosX = 0 ;
	m_iCenterPosY = 0 ;
	m_iNameOffset = 155;

	m_GuildColor = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	m_strGuild = "";

	string strFileName = "DATA/INTERFACE/CONCEPT/MONSTER_HP_GAGE.PNG";
	g_pVideo->GetTextureMgr()->LoadTexture(strFileName.c_str(), &m_pkGageTexture);

	strFileName = "DATA/INTERFACE/CONCEPT/SHADOW.PNG";
	g_pVideo->GetTextureMgr()->LoadTexture(strFileName.c_str(), &m_pkShadowTexture);

	if( g_pkGuildManager->GetSymbolFilename() )	{
		strFileName = g_pkGuildManager->GetSymbolFilename() ;
		g_pVideo->GetTextureMgr()->LoadTexture(strFileName.c_str(), &m_pkGuildIconTexture);
	}

	if( g_pkGuildManager->GetEmblemBGFilename() )	{
		strFileName = g_pkGuildManager->GetEmblemBGFilename() ;
		g_pVideo->GetTextureMgr()->LoadTexture(strFileName.c_str(), &m_pkGuildIconBGTexture);
	}

	m_pTiTleRender	=	NULL;

	//	AJJIYA [8/25/2005]
	//	말풍선

	strFileName = "DATA/INTERFACE/CONCEPT/UI101.PNG";
	g_pVideo->GetTextureMgr()->LoadTexture( strFileName.c_str(), &m_pkTalkBalloon );

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
	m_fTalkBalloonLimitTime			=	6.0f;
	m_colorTalkBalloonTitle			=	D3DCOLOR_ARGB( 255 , 185 , 255 , 255 );
	m_colorTalkBalloonID			=	D3DCOLOR_ARGB( 255 , 130 , 255 , 130 );
	m_colorTalkBalloonIDOther		=	D3DCOLOR_ARGB( 255 , 255 , 255 , 255 );
	m_colorTalkBalloonText			=	D3DCOLOR_ARGB( 255 , 255 , 255 , 255 );

	m_pkGuildFxArchive			= new SPFXArchive(m_pkParent);		// GuildFX Archive
	m_bSetGuildFx				= false ;

	strFileName = "DATA/INTERFACE/CONCEPT/UI_NUM.PNG";
	g_pVideo->GetTextureMgr()->LoadTexture( strFileName.c_str(), &m_pkLevel );

	m_ptLevelTitleSize.x		=	18;
	m_ptLevelTitleSize.y		=	9;

	m_rcLevelTitleSrc.left		=	80;
	m_rcLevelTitleSrc.top		=	53;
	m_rcLevelTitleSrc.right		=	m_rcLevelTitleSrc.left	+	m_ptLevelTitleSize.x;
	m_rcLevelTitleSrc.bottom	=	m_rcLevelTitleSrc.top	+	m_ptLevelTitleSize.y;

	m_iLevelCount				=	0;

	for( int i = 0 ; i < LEVEL_COUNT ; ++i )
	{
		m_rcLevelSrc[ i ].left		=	0;
		m_rcLevelSrc[ i ].top		=	0;
		m_rcLevelSrc[ i ].right		=	0;
		m_rcLevelSrc[ i ].bottom	=	0;

		m_ptLevelPos[ i ].x			=	0;
		m_ptLevelPos[ i ].y			=	0;
	}

	SetRect(&m_rtDestShopBoard, 0, 0, 0, 0);
	SetRect(&m_rtDestShopOverLap, 0, 0, 0, 0);
	
	m_fAccumulateRender = 0.0f;
}

//------------------------------------------------------------------------------------
void SPPlayerStatusModel::Process(float fTime)
{
	//[2006/6/7] - 상점 간판 렌더 관련
	if(m_pkParent->GetCurAction() & ACTION_USERSHOP) {
		if(!m_strShopName.empty()) {
			ProcessUserShop(fTime);
		}
	}	

	if( m_pkGuildFxArchive )
		m_pkGuildFxArchive->Process(fTime) ;
	
	// AJJIYA [8/25/2005]
	// 말풍선
	if( m_vTalkBalloonText.empty() == TRUE )
		return;

	m_fTalkBalloonAccmulateTime	+=	fTime;

	if( m_fTalkBalloonAccmulateTime > m_fTalkBalloonLimitTime )
		SetTalkBalloon( NULL , SCOPE_BROADCAST , INITCOLOR );
}

//------------------------------------------------------------------------------------
void SPPlayerStatusModel::Render(float fTime)
{
	if( g_pVideo == NULL )
		return;

	bool	bSpeechPlayer	=	true;
	bool	bSpeechTeam		=	true;
	bool	bSpeechGuild	=	true;
	bool	bIndicatePlayer	=	true;
	bool	bHpBarSelf		=	true;
	bool	bHpBarPlayer	=	true;
	bool	bHpBarParty		=	true;

	if( g_pClientOption != NULL )
	{
		SPOptionStruct*	pOptionStruct	=	g_pClientOption->GetOptionStruct();

		if( pOptionStruct != NULL )
		{
			bSpeechPlayer	=	pOptionStruct->bSpeechPlayer;
			bSpeechTeam		=	pOptionStruct->bSpeechTeam;
			bSpeechGuild	=	pOptionStruct->bSpeechGuild;
			bIndicatePlayer	=	pOptionStruct->bIndicatePlayer;
			bHpBarSelf		=	pOptionStruct->bHpBarSelf;
			bHpBarPlayer	=	pOptionStruct->bHpBarPlayer;
			bHpBarParty		=	pOptionStruct->bHpBarParty;
		}
	}

	if( m_bLocalPlayerTeam == true )
	{
		bHpBarPlayer	=	false;
	}
	else
	{
		bHpBarParty		=	false;
	}

	//////////////////////////////////////////////////////////////////////////

	if(m_pkParent->GetCurAction() & ACTION_USERSHOP) {
		if(g_pVideo->GetFont(FONT_12_NORMAL) != NULL)
			RenderUserShop(fTime);
	}
	else  {
		// AJJIYA [8/25/2005]
		// 말풍선
		SPFont* pkFont = g_pVideo->GetFont(FONT_12_NORMAL);		
		if( pkFont && m_bShowName )
		{
			pkFont->SetShadow(true);

			if( m_vTalkBalloonText.empty() == FALSE &&
				( ( m_eChatScope == SCOPE_AROUNDREGION	&& bSpeechPlayer == true ) || 
				( m_eChatScope == SCOPE_TEAM			&& bSpeechTeam	 == true ) ||
				( m_eChatScope == SCOPE_GUILD			&& bSpeechGuild	 == true ) )
				)
			{
				if( m_pkTalkBalloon != NULL )
				{
					m_pkTalkBalloon->SetColor();

					for( int i = 0; i < TALKBALLON_COUNT ; i++ )
						m_pkTalkBalloon->RenderTexture( &m_rcTalkBallonImageDestRgn[ i ] , &m_rcTalkBallonImageSrcRgn[ i ] , 0.0f );

					if( m_pTiTleRender != NULL )
					{
						POINT	ptTitleSize	=	m_pTiTleRender->GetSize();
						int	iDrawTitleSizeX	=	m_rcTalkBallonTitleDestRgn.right	-	m_rcTalkBallonTitleDestRgn.left;
						int	iDrawTitleSizeY	=	m_rcTalkBallonTitleDestRgn.bottom	-	m_rcTalkBallonTitleDestRgn.top;

						int	iTitleOffsetPosX	=	0;
						int	iTitleOffsetPosY	=	0;

						if( ptTitleSize.x != iDrawTitleSizeX )
							iTitleOffsetPosX	=	- ( ( ptTitleSize.x - iDrawTitleSizeX ) / 2 );

						if( ptTitleSize.y != iDrawTitleSizeY )
							iTitleOffsetPosY	=	- ( ( ptTitleSize.y - iDrawTitleSizeY ) / 2 );

						m_pTiTleRender->SetPos( m_rcTalkBallonTitleDestRgn.left + iTitleOffsetPosX , m_rcTalkBallonTitleDestRgn.top + iTitleOffsetPosY );
						m_pTiTleRender->Process( fTime );
						m_pTiTleRender->Render( fTime );
					}

					//g_pVideo->GetFont(FONT_12_NORMAL)->SetColor( m_colorTalkBalloonTitle );
					//g_pVideo->GetFont(FONT_12_NORMAL)->RenderText( "[초보모험가]" , &m_rcTalkBallonTitleDestRgn, DT_VCENTER | DT_CENTER | DT_WORDBREAK );
					//g_pVideo->GetFont(FONT_12_NORMAL)->SetColor();

					if( m_bLocal == true )
					{
						pkFont->SetColor( m_colorTalkBalloonID );
						int	ColorIndex = m_pkParent->GetStatus()->GetStatusValue( STATUS_LOVE_LV );
						if ( ColorIndex >= 1 && ColorIndex <= 10 )
						{
							D3DXCOLOR* NameColor = SPLoveManager::GetInstance()->GetD3DColor2( ColorIndex );
							if ( NameColor )
							{
								pkFont->SetColor( *NameColor );
							} 
						}
					}
					else
					{
						pkFont->SetColor( m_colorTalkBalloonIDOther );
					
					}
					pkFont->RenderText( m_szName.c_str(), &m_rcTalkBallonIDDestRgn, DT_VCENTER | DT_CENTER | DT_WORDBREAK );
					pkFont->SetColor();

					int	iStrCount	=	(int)m_vTalkBalloonText.size();

					pkFont->SetShadow(false);
					for( i = 0 ; i < iStrCount ; i++ )
					{
						pkFont->SetColor( m_colorTalkBalloonText );
						pkFont->RenderText( m_vTalkBalloonText.at( i ).c_str(), &( m_vTalkBallonTextDestRgn.at( i ) ) , DT_VCENTER | DT_CENTER );
						pkFont->SetColor();
					}
					pkFont->SetShadow(true);
				}
			}	// 말풍선 끝
			else
			{
				if( bIndicatePlayer == true )
				{
					pkFont->SetColor( m_NameColor );
					int	ColorIndex = m_pkParent->GetStatus()->GetStatusValue( STATUS_LOVE_LV );
					if ( ColorIndex >= 1 && ColorIndex <= 10 )
					{
						D3DXCOLOR* NameColor = SPLoveManager::GetInstance()->GetD3DColor2( ColorIndex );
						if ( NameColor )
						{
							pkFont->SetColor( *NameColor );
						} 
					}
					pkFont->RenderText( m_szName.c_str() , &m_rcNameRgn , DT_VCENTER | DT_CENTER );
					pkFont->SetColor();

					if( m_pTiTleRender != NULL )
					{
						m_pTiTleRender->SetPos( m_rcTalkBallonTitleDestRgn.left , m_rcTalkBallonTitleDestRgn.top );
						m_pTiTleRender->Process( fTime );
						m_pTiTleRender->Render( fTime );
					}
					g_pVideo->Flush();

					if( !m_strGuild.empty() )
					{
						pkFont->SetColor(m_NameColor);
						pkFont->RenderText(m_strGuild.c_str() , &m_rcGuildRgn , DT_VCENTER | DT_CENTER);
						pkFont->SetColor();

						if( m_pkGuildIconBGTexture )
							m_pkGuildIconBGTexture->RenderTexture(&m_rcGuildIcon , &m_rcGuildIconBGSrc);
						g_pVideo->Flush();
						if( m_pkGuildIconTexture )
							m_pkGuildIconTexture->RenderTexture(&m_rcGuildIcon , &m_rcGuildIconSrc);
						g_pVideo->Flush();
					}

					if( m_pkGuildFxArchive )			// Guild Emblem FX
						m_pkGuildFxArchive->Render(fTime) ;

					//if( m_pkLevel != NULL )
					//{
					//	m_pkLevel->SetColor();

					//	m_pkLevel->RenderTexture( &m_rcLevelTitleDest , &m_rcLevelTitleSrc );

					//	for( int i = 0 ; i < m_iLevelCount ; ++i )
					//	{
					//		m_pkLevel->RenderTexture( m_ptLevelPos[ i ].x , m_ptLevelPos[ i ].y , &m_rcLevelSrc[ i ] );
					//	}

					//	m_pkLevel->SetColor();
					//}
				}
			}
		}
	}	
	
	if( m_pkGageTexture && m_bShowHP )
	{
		if( ( m_bLocal == true	&& bHpBarSelf			== true ) ||
			( m_bLocal == false	&& m_bLocalPlayerTeam	== false	&& bHpBarPlayer	== true ) ||
			( m_bLocal == false && m_bLocalPlayerTeam	== true		&& bHpBarParty	== true )
			)
		{
			m_pkGageTexture->RenderTexture( &m_rcHPRgn , &m_rcHPBKSrcRgn , 0.0f );

			int iHPWidth = (int)((float)m_rcHPSrcRgn.right * m_fHPRatio);
			int iHPHeight = m_rcHPSrcRgn.bottom - m_rcHPSrcRgn.top;

			RECT rcDst, rcSrc;

			rcSrc = m_rcHPSrcRgn;
			rcSrc.right = m_rcHPSrcRgn.left + iHPWidth;

			rcDst.left = m_rcHPRgn.left + m_ptHPOffset.x;
			rcDst.right = rcDst.left + iHPWidth;

			rcDst.top = m_rcHPRgn.top + m_ptHPOffset.y;
			rcDst.bottom = rcDst.top + iHPHeight;

			m_pkGageTexture->RenderTexture(&rcDst, &rcSrc, 0.0f);
		}
	}

	if( m_pkShadowTexture && m_bShowDraw && m_bShowShadow )
		m_pkShadowTexture->RenderTexture(&m_rcShadowDstRgn, &m_rcShadowSrcRgn, 0.0f);

	g_pVideo->Flush();
}

//------------------------------------------------------------------------------------
void SPPlayerStatusModel::SetObjectRgn(RECT& rcRgn, int iCenterPosX, int iCenterPosY, bool bShadow)
{
	// Name
	SPFont* pkFont = g_pVideo->GetFont(FONT_12_NORMAL);

	if( pkFont == NULL )
		return;

	int iFontWidth	= pkFont->GetWidth();
	int iFontHeight	= (int)pkFont->GetHeight();
	m_iCenterPosX = iCenterPosX ;
	m_iCenterPosY = iCenterPosY ;

	// AJJIYA [8/25/2005]
	// 말풍선
	if( m_vTalkBalloonText.empty() == FALSE )
	{
		int		iTalkFontHeight	=	iFontHeight + 1;
		int		iStrCount		=	(int)m_vTalkBalloonText.size();
		int		iLineCount		=	iStrCount + 1;

		for( int i = 0 ; i < TALKBALLON_COUNT ; i++ )
		{
			m_rcTalkBallonImageDestRgn[ i ].left	=	iCenterPosX - ( m_iTalkBalloonImageWidth >> 1 );
			m_rcTalkBallonImageDestRgn[ i ].right	=	m_rcTalkBallonImageDestRgn[ i ].left + m_iTalkBalloonImageWidth;
		}

		int	iTitleHeight	=	0;

		if( m_pTiTleRender != NULL )
		{
			iTitleHeight	+=	m_pTiTleRender->GetSize().y;
		}

		m_rcTalkBallonImageDestRgn[ TALKBALLON_BOTTOM ].bottom	=	iCenterPosY - 132;	//	말풍선 OFFSET
		m_rcTalkBallonImageDestRgn[ TALKBALLON_BOTTOM ].top		=	m_rcTalkBallonImageDestRgn[ TALKBALLON_BOTTOM ].bottom - m_iTalkBalloonImageBottomHeight;

		m_rcTalkBallonImageDestRgn[ TALKBALLON_MIDDLE ].bottom	=	m_rcTalkBallonImageDestRgn[ TALKBALLON_BOTTOM ].top;
		m_rcTalkBallonImageDestRgn[ TALKBALLON_MIDDLE ].top		=	m_rcTalkBallonImageDestRgn[ TALKBALLON_MIDDLE ].bottom - iLineCount * iTalkFontHeight - iTitleHeight;

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

		m_rcTalkBallonTitleDestRgn.left		= m_rcTalkBallonIDDestRgn.left;
		m_rcTalkBallonTitleDestRgn.right	= m_rcTalkBallonIDDestRgn.right;
		m_rcTalkBallonTitleDestRgn.top		= m_rcTalkBallonImageDestRgn[ TALKBALLON_TOP ].bottom;
		m_rcTalkBallonTitleDestRgn.bottom	= m_rcTalkBallonIDDestRgn.top;

	}	// 말풍선 끝

	// 이름
	SIZE csSize = pkFont->GetSize(m_szName.c_str());
	m_rcNameRgn.left	= iCenterPosX - (csSize.cx >> 1);
	m_rcNameRgn.right	= m_rcNameRgn.left + csSize.cx;
	m_rcNameRgn.top		= iCenterPosY - m_iNameOffset;			//	캐릭터 이름 OFFSET
	m_rcNameRgn.bottom	= m_rcNameRgn.top + csSize.cy;

	if( m_vTalkBalloonText.empty() == TRUE && m_pTiTleRender != NULL )
	{
		m_rcTalkBallonTitleDestRgn.left		=	iCenterPosX	-	( m_pTiTleRender->GetSize().x >> 1 );
		m_rcTalkBallonTitleDestRgn.right	=	m_rcTalkBallonTitleDestRgn.left	+	m_pTiTleRender->GetSize().x;
		m_rcTalkBallonTitleDestRgn.top		=	m_rcNameRgn.top - m_pTiTleRender->GetSize().y;
		m_rcTalkBallonTitleDestRgn.bottom	=	m_rcTalkBallonTitleDestRgn.top + m_pTiTleRender->GetSize().y;
	}

	// 길드명이 없어도 계산을 하도록 한다.
	// 길드를 셋팅할때 좌표를 가져올수 있도록 하기 위해서 임.
	// 길드명이 없으면 캐릭터 명으로 계산
	if( !m_strGuild.empty() )
	{
		csSize = pkFont->GetSize(m_strGuild.c_str());
		m_rcGuildRgn.left	= iCenterPosX - (csSize.cx >> 1);
		m_rcGuildRgn.right	= m_rcGuildRgn.left + csSize.cx;
		m_rcGuildRgn.top	= iCenterPosY - 141;
		m_rcGuildRgn.bottom	= m_rcGuildRgn.top + csSize.cy;

		m_rcGuildIcon.left = (m_rcGuildRgn.left > m_rcNameRgn.left) ? m_rcNameRgn.left : m_rcGuildRgn.left;
		m_rcGuildIcon.left -= 28;
		m_rcGuildIcon.top = iCenterPosY - 155;
		m_rcGuildIcon.right = m_rcGuildIcon.left + 24;
		m_rcGuildIcon.bottom = m_rcGuildIcon.top + 24;

		// Guild FX Setting
		if( m_bSetGuildFx )
		{
			EMBLEM_INFO* pEmblem = NULL ;
			g_pkGuildManager->FindEmblemFX(m_iBeforeGuildFx, pEmblem) ;
			if( pEmblem ) {
				m_pkGuildFxArchive->Clean() ;
				m_pkGuildFxArchive->SetNULL() ;

				m_pkGuildFxArchive->SetOffsetPos(m_rcGuildIcon.left+12 - iCenterPosX, m_rcGuildIcon.top+12 - iCenterPosY) ;
				m_pkGuildFxArchive->InsertFXGroup( (SPID_FX_GROUP)pEmblem->iResFxID ) ;
			}
			m_bSetGuildFx = false ;
		}
	}


	// HP
	if( m_pkParent == NULL )
		return;

	SPGOBStatus* pkGOBStatus = NULL;
	m_pkParent->SPGOBPerformMessage(SPGM_GETGOBSTATUS, (LPARAM)&pkGOBStatus);
	if( pkGOBStatus == NULL )
		return;

	SetHPValue(pkGOBStatus->GetMaxHP(), pkGOBStatus->GetCurHP());

	int iWidth	= m_rcHPBKSrcRgn.right	- m_rcHPBKSrcRgn.left;
	int iHeight	= m_rcHPBKSrcRgn.bottom	- m_rcHPBKSrcRgn.top;

	m_rcHPRgn.left		= iCenterPosX - ( iWidth >> 1 );
	m_rcHPRgn.right		= m_rcHPRgn.left + iWidth;
	m_rcHPRgn.top		= iCenterPosY + 10;
	m_rcHPRgn.bottom	= m_rcHPRgn.top + iHeight;

	m_bShowDraw = bShadow;
	if( bShadow )
	{
		iWidth	= m_rcShadowSrcRgn.right - m_rcShadowSrcRgn.left;
		iHeight	= m_rcShadowSrcRgn.bottom - m_rcShadowSrcRgn.top;

		m_rcShadowDstRgn.left	= iCenterPosX - (iWidth >> 1);
		m_rcShadowDstRgn.top	= iCenterPosY - (iHeight >> 1);
		m_rcShadowDstRgn.right	= m_rcShadowDstRgn.left + iWidth;
		m_rcShadowDstRgn.bottom	= m_rcShadowDstRgn.top + iHeight;
	}			

//	UpdateDrawLevel( iCenterPosX , m_rcNameRgn.bottom );
}

//------------------------------------------------------------------------------------
void SPPlayerStatusModel::SetObjectName(const char* pszName)
{
	m_szName = pszName;
}

//------------------------------------------------------------------------------------
void SPPlayerStatusModel::SetShowValue(bool bName, bool bHP, bool bShadow)
{
	m_bShowName = bName;
	m_bShowHP = bHP;
	m_bShowShadow = bShadow;
}

//------------------------------------------------------------------------------------
void SPPlayerStatusModel::SetHPValue(unsigned int uiMaxHP, unsigned int uiCurHP)
{
	m_uiMaxHP = uiMaxHP;
	m_uiCurHP = uiCurHP;

	if( uiMaxHP == 0 )
	{
		m_fHPRatio = 0.0f;
		return;
	}
	 
	m_fHPRatio = (float)m_uiCurHP / (float)m_uiMaxHP;
}

//------------------------------------------------------------------------------------
void SPPlayerStatusModel::SetNameColorType(int iType)
{
	switch( iType )
	{
	case 0:			// Local
		m_NameColor = D3DXCOLOR(0.5f, 1.0f, 0.5f, 1.0f);
		break;
	
	case 1:			// Team
		m_NameColor = D3DXCOLOR(0.5607f, 0.7490f, 0.9294f, 1.0f);
		break;

	default:
		m_NameColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		break;
	}
}

//------------------------------------------------------------------------------------
void SPPlayerStatusModel::SetLocal(bool bLocal)
{
	m_bLocal = bLocal;
	if( m_bLocal )
		SetNameColorType(0);
}

//------------------------------------------------------------------------------------
void SPPlayerStatusModel::SetTalkBalloon( const char* pszString , BROADCAST_SCOPE eScope , D3DXCOLOR color )
{
	m_fTalkBalloonAccmulateTime	=	0.0f;
	m_eChatScope	=	eScope;

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

	g_pVideo->GetFont(FONT_12_NORMAL)->ConvertMultiLineW(pszString, rcDest, m_vTalkBalloonText);
	
	m_colorTalkBalloonText	=	color;

	int	iTalkStringLength	= (int)strlen(pszString);
	if( iTalkStringLength > 50 )
		m_fTalkBalloonLimitTime	=	8.0f;
	else
		m_fTalkBalloonLimitTime	=	6.0f;

}

//------------------------------------------------------------------------------------
void SPPlayerStatusModel::UpdateDrawLevel( int iCenterPosX , int iStartPosY )
{
	if( m_pkParent == NULL )
		return;

	SPGOBStatus*	pGOBStatus	=	m_pkParent->GetStatus();

	if( pGOBStatus == NULL )
		return;

	int	iValue	=	pGOBStatus->GetStatusValue( STATUS_LV );

	if( iValue <= 0 )
		return;

	if( iValue > LEVEL_MAX )
		iValue = LEVEL_MAX;

	int	iLevelTotalWidth		=	m_ptLevelTitleSize.x	+	m_iLevelCount	*	LEVEL_X_STEP;
	int	iLevelStartPosX			=	iCenterPosX	-	iLevelTotalWidth / 2;

	m_rcLevelTitleDest.left		=	iLevelStartPosX;
	m_rcLevelTitleDest.right	=	m_rcLevelTitleDest.left	+	m_ptLevelTitleSize.x;

	m_rcLevelTitleDest.top		=	iStartPosY;
	m_rcLevelTitleDest.bottom	=	m_rcLevelTitleDest.top	+	m_ptLevelTitleSize.y;

	iLevelStartPosX				=	m_rcLevelTitleDest.right;

	char		szBuf[_MAX_PATH]	=	{};
	std::string	strBufOneChiper;

	itoa( iValue , szBuf , 10 );

	int			iOneChiper;
	m_iLevelCount		=	(int)strlen( szBuf );

	for( int i = 0 ; i < m_iLevelCount ; i++ )
	{
		strBufOneChiper	=	szBuf[i];
		iOneChiper		=	atoi( strBufOneChiper.c_str() );

		m_rcLevelSrc[ i ].left		= iOneChiper				*	LEVEL_X_STEP	+	LEVEL_X_START;
		m_rcLevelSrc[ i ].right		= m_rcLevelSrc[ i ].left	+	LEVEL_X_STEP;
		m_rcLevelSrc[ i ].top		= LEVEL_Y_START;
		m_rcLevelSrc[ i ].bottom	= m_rcLevelSrc[ i ].top		+	LEVEL_Y_STEP;

		m_ptLevelPos[ i ].x	=	iLevelStartPosX	+	LEVEL_X_STEP	*	i;
		m_ptLevelPos[ i ].y	=	iStartPosY;

		if( i != 0 )
			m_ptLevelPos[ i ].x	-=	( 1 * i );
	}
}

void SPPlayerStatusModel::SetTitle( SPTitleData& stTitleData )
{
	SAFE_DELETE( m_pTiTleRender );

	m_pTiTleRender	=	SPTitleManager::GetInstance()->GetCreteTitleBase( stTitleData.m_uiTitleID , false );
}

SPTitleRenderBase* SPPlayerStatusModel::GetTitle()
{
	return m_pTiTleRender;
}

void SPPlayerStatusModel::SetLocalPlayerTeam( bool bLocalPlayerTeam )
{
	m_bLocalPlayerTeam	=	bLocalPlayerTeam;
}

bool SPPlayerStatusModel::GetLocalPlayerTeam()
{
	return m_bLocalPlayerTeam;
}

void SPPlayerStatusModel::SetUserShopName(std::string strName)
{
	m_strShopName.clear();
	m_strShopName = strName;
	
	m_fAccumulateRender = 0.0f;
	m_iCurFrame = 0;
}

std::string SPPlayerStatusModel::GetUserShopName()
{
	return m_strShopName;
}

void SPPlayerStatusModel::SetGuildInfo(const char* pszGuildName, int iIcon, int iIconBG, int iFxID)
{
	TCHAR szGuildName[64] ;
	m_strGuild = "";

	if( pszGuildName != NULL )
	{
		sprintf( szGuildName, "[%s]", pszGuildName) ;
		m_strGuild = szGuildName ;
	}
	
//#ifdef __ADD_GUILD_MARK
	// New
	m_rcGuildIconBGSrc.left = iIconBG ? (iIconBG%MAX_ICON_HORIZEN)*EMBLEM_ICON_SIZE : 0;
	m_rcGuildIconBGSrc.top	= iIconBG ? (iIconBG/MAX_ICON_HORIZEN)*EMBLEM_ICON_SIZE : 0;
	m_rcGuildIconBGSrc.right = m_rcGuildIconBGSrc.left + EMBLEM_ICON_SIZE;
	m_rcGuildIconBGSrc.bottom = m_rcGuildIconBGSrc.top + EMBLEM_ICON_SIZE;

	m_rcGuildIconSrc.left = iIcon ? (iIcon%MAX_ICON_HORIZEN)*EMBLEM_ICON_SIZE : 0;
	m_rcGuildIconSrc.top  = iIcon ? (iIcon/MAX_ICON_HORIZEN)*EMBLEM_ICON_SIZE : 0;
	m_rcGuildIconSrc.right = m_rcGuildIconSrc.left + 24;
	m_rcGuildIconSrc.bottom = m_rcGuildIconSrc.top + 24;

	if( iFxID > 0 )
	{
		m_iBeforeGuildFx = iFxID ;
		m_bSetGuildFx = true ;
	}
	else if( m_iBeforeGuildFx > 0 )
	{
		EMBLEM_INFO* pEmblem = NULL ;
		g_pkGuildManager->FindEmblemFX(m_iBeforeGuildFx, pEmblem) ;
		m_pkGuildFxArchive->DeleteFXGroup((SPID_FX_GROUP)pEmblem->iResFxID) ;
		m_iBeforeGuildFx = 0 ;
	}
	
//#else
	//int iRow, iCol ;
	//iRow = iIconBG / 5;
	//iCol = iIconBG % 5;
	//m_rcGuildIconBGSrc.left = 51 + iRow*25;
	//m_rcGuildIconBGSrc.top = 75 + iCol*25;
	//m_rcGuildIconBGSrc.right = m_rcGuildIconBGSrc.left + 24;
	//m_rcGuildIconBGSrc.bottom = m_rcGuildIconBGSrc.top + 24;

	//iRow = iIcon / 5;
	//iCol = iIcon % 5;
	//m_rcGuildIconSrc.left = 1 + iRow*25;
	//m_rcGuildIconSrc.top = 75 + iCol*25;
	//m_rcGuildIconSrc.right = m_rcGuildIconSrc.left + 24;
	//m_rcGuildIconSrc.bottom = m_rcGuildIconSrc.top + 24;
//#endif

	
}


//bool SPPlayerStatusModel::SetUserShop(int iType)
//{
//	//if(m_pkParent->GetUserShopType() != iType)
//	//	return ;
//
//	SPUserShopAttr* pShopData = g_pEventManager->GetUserShopArchive()->FindUserShopData(iType);
//	if(pShopData == NULL)
//		return false;
//
//
//
//	return true;
//}


void SPPlayerStatusModel::ProcessUserShop(float fTime)
{
	if(m_strShopName.empty() || m_pkParent->GetUserShopType() < 0)
		return;

	SPUserShopAttr* pShopData = g_pEventManager->GetUserShopArchive()->FindUserShopData(m_pkParent->GetUserShopType());
	if(pShopData == NULL)
		return;
	
	m_fAccumulateRender += fTime;			
	//if( m_fAccumulateRender > m_fCurDelay) {
	//	m_fAccumulateRender -= m_fCurDelay;
	//}
	if( m_fAccumulateRender > pShopData->m_SignBoard[m_iCurFrame].m_fDelay) {
		m_fAccumulateRender -= pShopData->m_SignBoard[m_iCurFrame].m_fDelay;
		
		m_iCurFrame++;
		if(m_iCurFrame >= pShopData->m_iMaxFrame)
			m_iCurFrame = 0;

		//if(m_iCurFrame >= pShopData->m_iMaxFrame) {
		//	m_iCurFrame = 0;
		//}
		//else {
		//	m_iCurFrame++;
		//}
	}
}


void SPPlayerStatusModel::RenderUserShop(float fTime)
{
	if(!m_strShopName.empty()) {
		SPUserShopAttr* pShopData = g_pEventManager->GetUserShopArchive()->FindUserShopData(m_pkParent->GetUserShopType());
		if(pShopData == NULL)
			return;

		if(m_iCurFrame < 0 || m_iCurFrame >= pShopData->m_iMaxFrame)
			return;

		if(pShopData->m_SignBoard[m_iCurFrame].m_pTexture) {
			pShopData->m_SignBoard[m_iCurFrame].m_pTexture->RenderTexture(&m_rtDestShopBoard, &pShopData->m_SignBoard[m_iCurFrame].m_rtSrc);
		}

#ifdef _DEBUG
		//g_pVideo->Renderrect
		//g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		//g_pVideo->GetSysLine()->DrawRect(m_rtDestShopBoard);
		//g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
		g_pVideo->GetSysLine()->DrawRect(m_rtDestShopName);
		g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
		g_pVideo->GetSysLine()->DrawRect(m_rtDestShopTitle);
		g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));		
		
		g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		g_pVideo->GetSysLine()->DrawRect(m_rtDestShopOverLap);
		g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
#endif			
		if(pShopData->m_bNameBold) {
			g_pVideo->GetFont(FONT_12_BOLD)->SetColor( pShopData->m_NameColor );
			g_pVideo->GetFont(FONT_12_BOLD)->SetShadow(false);		
			g_pVideo->GetFont(FONT_12_BOLD)->RenderText( m_szName.c_str() , &m_rtDestShopName , DT_VCENTER | DT_CENTER );
			g_pVideo->GetFont(FONT_12_BOLD)->SetColor();
			g_pVideo->GetFont(FONT_12_BOLD)->SetShadow(true);
		}
		else {
			g_pVideo->GetFont(FONT_12_NORMAL)->SetColor( pShopData->m_NameColor );
			g_pVideo->GetFont(FONT_12_NORMAL)->SetShadow(false);		
			g_pVideo->GetFont(FONT_12_NORMAL)->RenderText( m_szName.c_str() , &m_rtDestShopName , DT_VCENTER | DT_CENTER );
			g_pVideo->GetFont(FONT_12_NORMAL)->SetColor();
			g_pVideo->GetFont(FONT_12_NORMAL)->SetShadow(true);
		}	
		
		if(pShopData->m_bTitleBold){
			g_pVideo->GetFont(FONT_12_BOLD)->SetColor( pShopData->m_TitleColor );
			g_pVideo->GetFont(FONT_12_BOLD)->SetShadow(false);
			g_pVideo->GetFont(FONT_12_BOLD)->RenderText( m_strShopName.c_str(), &m_rtDestShopTitle, DT_VCENTER | DT_LEFT | DT_WORDBREAK );
			g_pVideo->GetFont(FONT_12_BOLD)->SetColor();
			g_pVideo->GetFont(FONT_12_BOLD)->SetShadow(true);
		}
		else {
			g_pVideo->GetFont(FONT_12_NORMAL)->SetColor( pShopData->m_TitleColor );
			g_pVideo->GetFont(FONT_12_NORMAL)->SetShadow(false);
			g_pVideo->GetFont(FONT_12_NORMAL)->RenderText( m_strShopName.c_str(), &m_rtDestShopTitle, DT_VCENTER | DT_LEFT | DT_WORDBREAK );
			g_pVideo->GetFont(FONT_12_NORMAL)->SetColor();
			g_pVideo->GetFont(FONT_12_NORMAL)->SetShadow(true);
		}
		
		g_pVideo->Flush();
	}
}


void SPPlayerStatusModel::SetUserShopRgn(RECT& rcRgn, int iCenterPosX, int iCenterPosY)
{
	if(m_strShopName.empty() || m_pkParent->GetUserShopType() < 0)
		return;

	SPUserShopAttr* pShopData = g_pEventManager->GetUserShopArchive()->FindUserShopData(m_pkParent->GetUserShopType());
	if(pShopData == NULL)
		return;

	if(m_iCurFrame < 0 || m_iCurFrame >= pShopData->m_iMaxFrame)
		return;

	// Name
	SPFont* pkFont = g_pVideo->GetFont(FONT_12_NORMAL);

	if( pkFont == NULL )
		return;

	int iFontWidth	= pkFont->GetWidth();
	int iFontHeight	= (int)pkFont->GetHeight();	
	
	//m_iCurFrame = 0;
	//m_fAccumulateRender = 0.0f;

	m_rtDestShopBoard.left = iCenterPosX + pShopData->m_SignBoard[m_iCurFrame].m_ptDest.x;
	m_rtDestShopBoard.top = iCenterPosY + pShopData->m_SignBoard[m_iCurFrame].m_ptDest.y;
	m_rtDestShopBoard.right = m_rtDestShopBoard.left + pShopData->m_SignBoard[m_iCurFrame].m_iWidth;
	m_rtDestShopBoard.bottom = m_rtDestShopBoard.top + pShopData->m_SignBoard[m_iCurFrame].m_iHeight;
	//m_rtDestShopBoard.right = m_rtDestShopBoard.left + 200;
	//m_rtDestShopBoard.bottom = m_rtDestShopBoard.top + 50;

	m_rtDestShopOverLap.left = iCenterPosX + pShopData->m_rtOverLapRect.left;
	m_rtDestShopOverLap.top = iCenterPosY + pShopData->m_rtOverLapRect.top;
	m_rtDestShopOverLap.right = iCenterPosX + pShopData->m_rtOverLapRect.right;
	m_rtDestShopOverLap.bottom = iCenterPosY + pShopData->m_rtOverLapRect.bottom;
	
	m_rtDestShopName.left = iCenterPosX + pShopData->m_SignBoard[m_iCurFrame].m_ptNameDest.x;
	m_rtDestShopName.top = iCenterPosY + pShopData->m_SignBoard[m_iCurFrame].m_ptNameDest.y;
	m_rtDestShopName.right = m_rtDestShopName.left + pShopData->m_iNameWidth;
	m_rtDestShopName.bottom = m_rtDestShopName.top + pShopData->m_iNameHeight;

	m_rtDestShopTitle.left = iCenterPosX + pShopData->m_SignBoard[m_iCurFrame].m_ptTitleDest.x;
	m_rtDestShopTitle.top = iCenterPosY + pShopData->m_SignBoard[m_iCurFrame].m_ptTitleDest.y;
	m_rtDestShopTitle.right = m_rtDestShopTitle.left + pShopData->m_iTitleWidth;
	m_rtDestShopTitle.bottom = m_rtDestShopTitle.top + pShopData->m_iTitleHeight;
}


RECT* SPPlayerStatusModel::GetUserShopBoardRect()
{
	if(m_strShopName.empty() || m_pkParent->GetUserShopType() < 0)
		return NULL;

	SPUserShopAttr* pShopData = g_pEventManager->GetUserShopArchive()->FindUserShopData(m_pkParent->GetUserShopType());
	if(pShopData == NULL)
		return NULL;

	if(m_iCurFrame < 0 || m_iCurFrame >= pShopData->m_iMaxFrame)
		return NULL;

	return &m_rtDestShopBoard;
}




