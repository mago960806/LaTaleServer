
#include "SPCommon.h"
#include "SPTexture.h"
#include "SPMonsterModelUnitDef.h"
#include "SPOptionDef.h"
#include "SPClientOptionArchive.h"
#include "SPMonsterStatusModel.h"

#include "SPGOBStatus.h"
#include "SPGameObject.h"
#include "SPGOBCoordPhysics.h"
#include "SPMonster.h"
#include "SPCommandConvert.h"
#include "SPEventDEF.h"
#include "SPUserShopAttr.h"
#include "SPUserShopArchive.h"
#include "SPEventManager.h"

#include "SPMOBCluster.h"
#include "SPAbuseFilter.h"

#include "SPBeAttackedList.h"

//------------------------------------------------------------------------------------
SPMonsterStatusModel::SPMonsterStatusModel(SPGameObject* pkMonster)
: m_pkParent(pkMonster)
, m_pkGageTexture(NULL)
, m_pkShadowTexture(NULL)
, m_iIconIndex(-1)
, m_pkTalkBalloon(NULL)
, m_fAccumulateRender( 0.0f )
{
	Init();
}

//------------------------------------------------------------------------------------
SPMonsterStatusModel::~SPMonsterStatusModel()
{
	Destroy();
}

//------------------------------------------------------------------------------------
void SPMonsterStatusModel::Destroy()
{
	SAFE_RELEASE(m_pkGageTexture);
	SAFE_RELEASE(m_pkShadowTexture);
	SAFE_RELEASE(m_pkTalkBalloon);
}

//------------------------------------------------------------------------------------
void SPMonsterStatusModel::Init()
{
	SetRect(&m_rcHPSrcRgn, 0, 0, 43, 4);
	SetRect(&m_rcHPBKSrcRgn, 0, 4, 46, 11);

	SetRect(&m_rcSPSrcRgn, 0, 15, 43, 19);
	SetRect(&m_rcSPBKSrcRgn, 0, 4, 46, 11);
	SetRect(&m_rcXPSrcRgn, 0, 19, 43, 23);
	SetRect(&m_rcXPBKSrcRgn, 0, 23, 46, 30);

	m_ptHPOffset.x = 1;
	m_ptHPOffset.y = 1;

	m_NameColor = D3DXCOLOR(1.0f, 0.392f, 0.392f, 1.0f);
	m_szName = "";
	m_uiMaxHP = 0;
	m_uiCurHP = 0;
	m_fHPRatio = 0.0f;
	m_bShadowDraw = false;
	m_bToggleName = true;
	m_bIsPet = false;

	m_fSPRatio = 0.0f;
	m_fXPRatio = 0.0f;
	m_iCurFrame = 0;
	m_bOpenShop = false;

	string strFileName = "DATA/INTERFACE/CONCEPT/MONSTER_HP_GAGE.PNG";
	g_pVideo->GetTextureMgr()->LoadTexture(strFileName.c_str(), &m_pkGageTexture);

	strFileName = "DATA/INTERFACE/CONCEPT/SHADOW.PNG";
	g_pVideo->GetTextureMgr()->LoadTexture(strFileName.c_str(), &m_pkShadowTexture);

	strFileName = "DATA/INTERFACE/CONCEPT/UI101.PNG";
	g_pVideo->GetTextureMgr()->LoadTexture(strFileName.c_str(), &m_pkTalkBalloon);
	
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
	m_colorTalkBalloonText			=	D3DCOLOR_ARGB( 255 , 255 , 255 , 255 );
	m_colorTalkBalloonID			=	D3DCOLOR_ARGB( 255 , 255 , 200 , 220 );
}

//------------------------------------------------------------------------------------
void SPMonsterStatusModel::Process(float fTime)
{
	if( !m_vTalkBalloonText.empty() )
	{
		m_fTalkBalloonAccmulateTime	+=	fTime;

		if( m_fTalkBalloonAccmulateTime > m_fTalkBalloonLimitTime )
		{
			m_fTalkBalloonAccmulateTime = 0.0f;
			SetTalkBalloon( NULL );
		}
	}

	if (m_bOpenShop)
		ProcessPetShop(fTime);
}

//------------------------------------------------------------------------------------
void SPMonsterStatusModel::Render(float fTime)
{
	if( g_pVideo == NULL )
		return;

	SPFont* pkFont = g_pVideo->GetFont(FONT_12_NORMAL);
	if( pkFont == NULL )
		return;

	bool	bIndicateMonster	=	true;
	bool	bIndicatePet		=	true;
	bool	bSpeechNPC			=	true;

	if( g_pClientOption != NULL )
	{
		SPOptionStruct*	pOptionStruct	=	g_pClientOption->GetOptionStruct();

		if( pOptionStruct != NULL )
		{
			bIndicateMonster	=	pOptionStruct->bIndicateMonster;
			bIndicatePet		=	pOptionStruct->bIndicatePet;
			bSpeechNPC			=	pOptionStruct->bSpeechNPC;
		}
	}

	bool	bIsPet	=	false;

	if( m_pkParent != NULL )
	{
		CLASS_ID	eClassID	=	m_pkParent->GetClassID();

		//	임시임
		if( eClassID >= CLASS_ID_PET && eClassID < 0x30A32C00 )
		{
			bIndicateMonster	=	false;
			bIsPet				=	true;
		}
		else
		{
			bIndicatePet		=	false;
		}
	}

	if( m_bShowName && m_bToggleName )
	{
		if( ( bIsPet == false	&& bIndicateMonster	== true ) || 
			( bIsPet == true	&& bIndicatePet		== true )
			)
		{
			pkFont->SetShadow();
			pkFont->SetColor(m_NameColor);
			pkFont->RenderText(m_szName.c_str(), &m_rcNameRgn, DT_VCENTER | DT_CENTER );
			pkFont->SetColor();

			if( m_iIconIndex >= 0 )
			{
				RECT rcSrc;
				SPTexture* pkTexture = g_pMOBCluster->GetIconTexture(m_iIconIndex, rcSrc);
				if( pkTexture )
					pkTexture->RenderTexture(&m_rcIconDstRgn, &rcSrc);
			}
		}
	}

	if( m_pkGageTexture && (m_bShowHP || m_bIsPet) && !m_bOpenShop)
	{
		if( !m_bIsPet )
		{
//#ifdef _USE_OLD_HP_BAR
			//[2007/10/18] - 보스만 게이지 나오게 하기 위해서 임시로 풀어놓음
			m_pkGageTexture->RenderTexture(&m_rcHPRgn, &m_rcHPBKSrcRgn, 0.0f);

			int iHPWidth = (int)((float)m_rcHPSrcRgn.right * m_fHPRatio);

			RECT rcDst, rcSrc;

			rcSrc = m_rcHPSrcRgn;
			rcSrc.right = m_rcHPSrcRgn.left + iHPWidth;

			rcDst.left = m_rcHPRgn.left + m_ptHPOffset.x;
			rcDst.right = rcDst.left + iHPWidth;

			rcDst.top = m_rcHPRgn.top + m_ptHPOffset.y;
			rcDst.bottom = rcDst.top + m_rcHPSrcRgn.bottom;

			m_pkGageTexture->RenderTexture(&rcDst, &rcSrc, 0.0f);
//#endif
		}		
		else
		{
			m_pkGageTexture->RenderTexture(&m_rcSPRgn, &m_rcSPBKSrcRgn, 0.0f);

			int iWidth = (int)((float)m_rcSPSrcRgn.right * m_fSPRatio);
			int iHeight = m_rcSPSrcRgn.bottom - m_rcSPSrcRgn.top;

			RECT rcDst, rcSrc;

			rcSrc = m_rcSPSrcRgn;
			rcSrc.right = m_rcSPSrcRgn.left + iWidth;

			rcDst.left = m_rcSPRgn.left + m_ptHPOffset.x;
			rcDst.right = rcDst.left + iWidth;

			rcDst.top = m_rcSPRgn.top + m_ptHPOffset.y;
			rcDst.bottom = rcDst.top + iHeight;

			m_pkGageTexture->RenderTexture(&rcDst, &rcSrc, 0.0f);
			g_pVideo->Flush();

			m_pkGageTexture->RenderTexture(&m_rcXPRgn, &m_rcXPBKSrcRgn, 0.0f);

			iWidth = (int)((float)m_rcXPSrcRgn.right * m_fXPRatio);
			iHeight = m_rcXPSrcRgn.bottom - m_rcXPSrcRgn.top;

			rcSrc = m_rcXPSrcRgn;
			rcSrc.right = m_rcXPSrcRgn.left + iWidth;

			rcDst.left = m_rcXPRgn.left + m_ptHPOffset.x;
			rcDst.right = rcDst.left + iWidth;

			rcDst.top = m_rcXPRgn.top + m_ptHPOffset.y;
			rcDst.bottom = rcDst.top + iHeight;

			m_pkGageTexture->RenderTexture(&rcDst, &rcSrc, 0.0f);
			g_pVideo->Flush();
		}
	}

	if( m_pkShadowTexture && m_bShadowDraw && m_bShowShadow )
		m_pkShadowTexture->RenderTexture(&m_rcShadowDstRgn, &m_rcShadowSrcRgn, 0.0f);

	if (m_bOpenShop)
		RenderPetShop(fTime);

	if( m_vTalkBalloonText.empty() == FALSE && bSpeechNPC == true )
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
	}

	g_pVideo->Flush();
}

//------------------------------------------------------------------------------------
void SPMonsterStatusModel::SetObjectRgn(RECT& rcRgn, int iCenterPosX, int iCenterPosY, bool bShadow)
{
	// Name
	SPFont* pkFont = g_pVideo->GetFont(FONT_12_NORMAL);
	if( pkFont == NULL || m_pkParent == NULL )
		return;

	int iFontWidth = pkFont->GetWidth();
	int iFontHeight = pkFont->GetHeight();
	int iLength = (int)m_szName.size();

	if( iFontWidth == 0 )
		iFontWidth = iFontHeight;

	int iNameWidth = 0;
	if( m_iIconIndex < 0 )
		iNameWidth = iFontWidth * iLength;
	else
		iNameWidth = ((iFontWidth * iLength) >> 1) + 16;
	
	m_rcNameRgn.left = iCenterPosX - (iNameWidth >> 1);
	m_rcNameRgn.right = m_rcNameRgn.left + iNameWidth;
	m_rcNameRgn.top = rcRgn.top;
	m_rcNameRgn.bottom = m_rcNameRgn.top + iFontHeight;

	if( m_iIconIndex >= 0 )
	{
		m_rcIconDstRgn.left = m_rcNameRgn.left - 16;
		m_rcIconDstRgn.top = m_rcNameRgn.top - 2;
		m_rcIconDstRgn.right = m_rcIconDstRgn.left + 16;
		m_rcIconDstRgn.bottom = m_rcIconDstRgn.top + 16;
	}

	int iWidth, iHeight;
	
	m_bShadowDraw = bShadow;
	if( bShadow )
	{
		iWidth = m_rcShadowSrcRgn.right - m_rcShadowSrcRgn.left;
		iHeight = m_rcShadowSrcRgn.bottom - m_rcShadowSrcRgn.top;

		m_rcShadowDstRgn.left = iCenterPosX - (iWidth >> 1);
		m_rcShadowDstRgn.top = iCenterPosY - (iHeight >> 1);
		m_rcShadowDstRgn.right = m_rcShadowDstRgn.left + iWidth;
		m_rcShadowDstRgn.bottom = m_rcShadowDstRgn.top + iHeight;
	}

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
	}

	// HP
	if( !m_bIsPet )
	{
		SPGOBStatus* pkGOBStatus = NULL;
		m_pkParent->SPGOBPerformMessage(SPGM_GETGOBSTATUS, (LPARAM)&pkGOBStatus);
		if( pkGOBStatus == NULL )
			return;

		SetHPValue(pkGOBStatus->GetMaxHP(), pkGOBStatus->GetCurHP());

		iWidth = m_rcHPBKSrcRgn.right - m_rcHPBKSrcRgn.left;
		iHeight = m_rcHPBKSrcRgn.bottom - m_rcHPBKSrcRgn.top;

		m_rcHPRgn.left = iCenterPosX - (iWidth >> 1);
		m_rcHPRgn.right = m_rcHPRgn.left + iWidth;
		m_rcHPRgn.top = rcRgn.bottom + 5;
		m_rcHPRgn.bottom = m_rcHPRgn.top + iHeight;
	}
	else
	{
		iWidth = m_rcSPBKSrcRgn.right - m_rcSPBKSrcRgn.left;
		iHeight = m_rcSPBKSrcRgn.bottom - m_rcSPBKSrcRgn.top;

		m_rcSPRgn.left = iCenterPosX - (iWidth >> 1);
		m_rcSPRgn.right = m_rcSPRgn.left + iWidth;
		m_rcSPRgn.top = rcRgn.bottom + 5;
		m_rcSPRgn.bottom = m_rcSPRgn.top + iHeight;

		iWidth = m_rcXPBKSrcRgn.right - m_rcXPBKSrcRgn.left;
		iHeight = m_rcXPBKSrcRgn.bottom - m_rcXPBKSrcRgn.top;

		m_rcXPRgn.left = iCenterPosX - (iWidth >> 1);
		m_rcXPRgn.right = m_rcXPRgn.left + iWidth;
		m_rcXPRgn.top = rcRgn.bottom + 10;
		m_rcXPRgn.bottom = m_rcXPRgn.top + iHeight;
	}
}

//------------------------------------------------------------------------------------
void SPMonsterStatusModel::SetObjectName(const char* pszName, D3DXCOLOR* pColor)
{
	m_szName = pszName;
	if( pColor )
		m_NameColor = *pColor;
}

//------------------------------------------------------------------------------------
void SPMonsterStatusModel::SetMonterType(MONSTER_INDEX eType)
{
	switch( eType )
	{
	case MON_TINY:
		SetRect(&m_rcShadowSrcRgn, 0, 0, 64, 32);
		break;
	case MON_SMALL:
		SetRect(&m_rcShadowSrcRgn, 64, 0, 128, 32);
		break;
	case MON_MED:
		SetRect(&m_rcShadowSrcRgn, 0, 32, 128, 64);
		break;
	case MON_BIG:
		SetRect(&m_rcShadowSrcRgn, 0, 64, 128, 96);
		break;
	}
}

//------------------------------------------------------------------------------------
void SPMonsterStatusModel::SetHPValue(unsigned int uiMaxHP, unsigned int uiCurHP)
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
void SPMonsterStatusModel::SetShowValue(bool bName, bool bHP, bool bShadow)
{
	m_bShowName = bName;
	m_bShowHP = bHP;
	m_bShowShadow = bShadow;
}

//------------------------------------------------------------------------------------
void SPMonsterStatusModel::SetNameColor(LPARAM lParam)
{
	unsigned short usPlayerLV, usMobLV;
	usPlayerLV = (unsigned short)((lParam & 0xFFFF0000) >> 16);
	usMobLV = (unsigned short)(lParam & 0x0000FFFF);

	float fDiff = (float)(usPlayerLV - usMobLV);
	if( fDiff >= (float)usPlayerLV * 0.1f + 6.0f )
		m_NameColor = D3DXCOLOR(0.8039f, 0.8039f, 0.8039f, 1.0f);
	else if( fDiff >= (float)usPlayerLV * 0.07f + 4.0f )
		m_NameColor = D3DXCOLOR(0.3607f, 1.0f, 0.0f, 1.0f);
	else if( fDiff >= (float)usPlayerLV * 0.04f + 2.0f )
		m_NameColor = D3DXCOLOR(0.7607f, 1.0f, 0.0f, 1.0f);
	else if( fDiff >= -(float)usPlayerLV * 0.04f - 2.0f )
		m_NameColor = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
	else if( fDiff >= -(float)usPlayerLV * 0.07f - 4.0f )
		m_NameColor = D3DXCOLOR(1.0f, 0.7450f, 0.0f, 1.0f);
	else if( fDiff >= -(float)usPlayerLV * 0.1f - 6.0f )
		m_NameColor = D3DXCOLOR(1.0f, 0.4470f, 0.0f, 1.0f);
	else
		m_NameColor = D3DXCOLOR(1.0f, 0.0235f, 0.0f, 1.0f);
}

//------------------------------------------------------------------------------------
void SPMonsterStatusModel::SetToggleName(bool bEnable)
{
	m_bToggleName = bEnable;
}

//------------------------------------------------------------------------------------
void SPMonsterStatusModel::SetIconIndex(int iIndex)
{
	m_iIconIndex = iIndex;
}

//------------------------------------------------------------------------------------
int  SPMonsterStatusModel::GetIconIndex()
{
	return m_iIconIndex;
}

//------------------------------------------------------------------------------------
void SPMonsterStatusModel::SetTalkBalloon(const char* pszString)
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

//------------------------------------------------------------------------------------
void SPMonsterStatusModel::SetPet(bool bPet)
{
	m_bIsPet = bPet;
}

//------------------------------------------------------------------------------------
void SPMonsterStatusModel::SetPetGageRgn(int iCurSP, int iMaxSP, INT64 iCurXP, INT64 iMaxXP)
{
	m_fSPRatio = 0.0f;
	if( iMaxSP != 0 )
		m_fSPRatio = (float)iCurSP / (float)iMaxSP;

	m_fXPRatio = 0.0f;
	if( iMaxXP != 0 )
		m_fXPRatio = (double)iCurXP / (double)iMaxXP;
}

//--------------------------------------------------
void SPMonsterStatusModel::ProcessPetShop(float fTime)
{
	if (m_strShopName.empty() || m_iShopType < 0)
		return;

	SPUserShopAttr	*pShopData = g_pEventManager->GetUserShopArchive()->FindUserShopData( m_iShopType );
	if (!pShopData)
		return;

	m_fAccumulateRender += fTime;
	if (m_fAccumulateRender > pShopData->m_SignBoard[m_iCurFrame].m_fDelay)
	{
		m_fAccumulateRender -= pShopData->m_SignBoard[m_iCurFrame].m_fDelay;
		m_iCurFrame++;
		if (m_iCurFrame >= pShopData->m_iMaxFrame)
			m_iCurFrame = 0;
	}
}

//--------------------------------------------------
void SPMonsterStatusModel::RenderPetShop(float fTime)
{
	if (!m_strShopName.empty())
	{
		SPUserShopAttr	*pShopData = g_pEventManager->GetUserShopArchive()->FindUserShopData( m_iShopType );
		if (!pShopData)
			return;

		if (m_iCurFrame < 0 || m_iCurFrame >= pShopData->m_iMaxFrame)
			return;

		if (pShopData->m_SignBoard[m_iCurFrame].m_pTexture)
			pShopData->m_SignBoard[m_iCurFrame].m_pTexture->RenderTexture( &m_rtDestShopBoard, &pShopData->m_SignBoard[m_iCurFrame].m_rtSrc );

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

//--------------------------------------------------
void SPMonsterStatusModel::SetPetShopRgn(RECT &rcRgn, int iCenterPosX, int iCenterPosY)
{
	if(m_strShopName.empty() || m_iShopType < 0)
		return;

	SPUserShopAttr* pShopData = g_pEventManager->GetUserShopArchive()->FindUserShopData(m_iShopType);
	if(pShopData == NULL)
		return;

	if(m_iCurFrame < 0 || m_iCurFrame >= pShopData->m_iMaxFrame)
		return;

	SPFont* pkFont = g_pVideo->GetFont(FONT_12_NORMAL);

	if( pkFont == NULL )
		return;

	int iFontWidth	= pkFont->GetWidth();
	int iFontHeight	= (int)pkFont->GetHeight();	

	m_rtDestShopBoard.left = iCenterPosX + pShopData->m_SignBoard[m_iCurFrame].m_ptDest.x;
	m_rtDestShopBoard.top = iCenterPosY + pShopData->m_SignBoard[m_iCurFrame].m_ptDest.y;
	m_rtDestShopBoard.right = m_rtDestShopBoard.left + pShopData->m_SignBoard[m_iCurFrame].m_iWidth;
	m_rtDestShopBoard.bottom = m_rtDestShopBoard.top + pShopData->m_SignBoard[m_iCurFrame].m_iHeight;

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

//--------------------------------------------------
RECT* SPMonsterStatusModel::GetPetShopBoardRect( void )
{
	if(m_strShopName.empty() || m_iShopType < 0)
		return NULL;

	SPUserShopAttr* pShopData = g_pEventManager->GetUserShopArchive()->FindUserShopData(m_iShopType);
	if(pShopData == NULL)
		return NULL;

	if(m_iCurFrame < 0 || m_iCurFrame >= pShopData->m_iMaxFrame)
		return NULL;

	return &m_rtDestShopBoard;
}