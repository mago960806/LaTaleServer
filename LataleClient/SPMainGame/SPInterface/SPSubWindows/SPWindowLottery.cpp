// ***************************************************************
//  SPWindowLottery   version:  1.0   ·  date: 05/30/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "SPCommon.h"
#include "SPUtil.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPWindowGrid.h"
#include "SPWindowSlider.h"
#include "SPWindowList.h"
#include "SPWindowEdit.h"
#include "SPWindowToolTip.h"

#include "SPUIUnit.h"
#include "SPUIItemUnit.h"
#include "SPUIUnitManager.h"

#include "SPResourceDef.h"
#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"

#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "SPSubGameManager.h"

#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"
#include "SockWnd.h"
#include "SPNetworkManager.h"

#include "SPItemStatus.h"
#include "SPItemAttr.h"
#include "SPManager.h"
#include "SPItem.h"
#include "SPItemCluster.h"		//[2005/6/16]

#include "SPCommandConvert.h"
#include "SPGOBStatus.h"
#include "SPGOBManager.h"
#include "SPGOBCoordPhysics.h"
#include "SPGameObject.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPPlayerInvenArchive.h"
#include "SPPlayer.h"

#include "SPRenderModelDEF.h"
#include "SPRenderModel.h"

#include "SPFXDEF.h"
#include "SPFXModel.h"
#include "SPFXUnit.h"
#include "SPFXGroupUnit.h"
#include "SPFXArchive.h"

#include "SPSoundArchive.h"
#include "SPSoundManager.h"

#include "SPWindowToolTip.h"

#include "SPCheckManager.h"
#include "SPLocalizeManager.h"
#include "GlobalDefines_Share.h"

#ifdef _DEBUG
#include "SPDebug.h"
#endif

#include "SPOptionDef.h"
#include "SPClientOptionArchive.h"

#include "SPWindowLottery.h"

#include "FontManager.h"
#include "atlconv.h"
#include <tinyxml.h>

////////////////////////////////////////////////////////////////////////////////
//
//CAniManager
//
CTicketAniManager::CTicketAniManager()
{
	m_vAni.clear();
	m_pTexture = NULL;
}

CTicketAniManager::~CTicketAniManager()
{
	m_vAni.clear();
	SAFE_RELEASE(m_pTexture);
}

bool	CTicketAniManager::Init()
{
	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI110.PNG", &m_pTexture );

	for( int y = 0 ; y < LOTTERY_GUIDE_Y_MAX ; ++y )
	{
		int iYPos			=	74 + ( y * ( 19 + 2 ) ) + ( 13 * ( y / 5 ) );
		for( int x = 0 ; x < LOTTERY_GUIDE_X_MAX ; ++x )
		{
			int iXPos		=	229 + ( x * ( 20 + 4 ) ) + ( 21 * ( x / 5 ) );
			ANI_NODE  ani;
			ani.m_rcDst.left	 = iXPos;
			ani.m_rcDst.top		 = iYPos;
			ani.m_rcDst.right	 = iXPos + 20;
			ani.m_rcDst.bottom	 = iYPos + 19;
			for ( int j=0; j<9; j++)
			{
				RECT   src;
				src.left	 = 310 + j*(18+1);
				src.top		 = 479;
				src.right	 = src.left + 18;
				src.bottom	 = src.top + 18;
				ani.m_vrcSrc.push_back(src);
			}
			m_vAni.push_back(ani);
		}
	}

	m_aniStatus = ANI_END;
	m_fStartTime = 0.0f;
	return true;
}

void	CTicketAniManager::SetAni(ANI_STATUS  status)
{ 
	if ( m_aniStatus == status )
		return;

	m_aniStatus = status;
	m_fStartTime = 0.0f;

	vector<ANI_NODE>::iterator  itAni;
	for ( itAni = m_vAni.begin(); itAni != m_vAni.end(); itAni++ )
		itAni->Stop();
}

void	CTicketAniManager::Process(float fTime)
{
	if ( m_aniStatus == ANI_END )
		return;

	m_fStartTime += fTime;
	switch ( m_aniStatus )
	{
	case ANI_1:
		Process1(fTime);
		break;
	case ANI_2:
		Process2(fTime);
		break;
	case ANI_3:
		Process3(fTime);
		break;
	case ANI_4:
		Process4(fTime);
		break;
	case ANI_5:
		Process5(fTime);
		break;
	}

	vector<ANI_NODE>::iterator  itAni;
	for ( itAni = m_vAni.begin(); itAni != m_vAni.end(); itAni++ )
		itAni->Process(fTime);
}

void	CTicketAniManager::Process1(float fTime)
{
	static  float  fSpeedTime = 0.05f;

	int iIndex = m_fStartTime / fSpeedTime;
	iIndex = iIndex % m_vAni.size();

	if ( m_vAni[iIndex].isPlay() == false )
		m_vAni[iIndex].Play(1);
}

void	CTicketAniManager::Process2(float fTime)
{
	static  float  fSpeedTime = 0.05f;
	int  iLight[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
		39, 59, 79, 99, 119, 139, 159, 179, 199, 219, 239, 259, 279, 299, 319, 339, 359, 379, 399,
		398, 397, 396, 395, 394, 393, 392, 391, 390, 389, 388, 387, 386, 385, 384, 383, 382, 381, 380,
		360, 340, 320, 300, 280, 260, 240, 220, 200, 180, 160, 140, 120, 100, 80, 60, 40, 20 };

	int iIndex = m_fStartTime / fSpeedTime;
	int	iTotal = sizeof(iLight)/sizeof(int);
	iIndex = iIndex % iTotal;

	if ( m_vAni[iLight[iIndex]].isPlay() == false )
		m_vAni[iLight[iIndex]].Play(1);
}

void	CTicketAniManager::Process3(float fTime)
{
	static  int iX = 0, iY = 0;
	static  int iXStart = 0, iYStart = 0;
	static  int iXlen = 19, iYlen = 19;

	if ( iXStart >= iXlen && iYStart >= iYlen )
	{
		iX = 0, iY = 0;
		iXStart = 0, iYStart = 0;
		iXlen = 19, iYlen = 19;
	}
	if ( iY <= iYStart )
	{
		iX++;
	}
	if ( iX <= iXStart )
	{
		iY--;
	}
	if ( iY >= iYlen )
	{
		iX--;
		iYlen--;
		iYStart++;
	}
	if ( iX >= iXlen )
	{
		iY++;
		iXlen--;
		iXStart++;
	}
	
	static  float  fSpeedTime = 0.1f;
}

void	CTicketAniManager::Process4(float fTime)
{
	static  float  fSpeedTime = 0.1f;
}

void	CTicketAniManager::Process5(float fTime)
{
	static  float  fSpeedTime = 0.1f;
}

void	CTicketAniManager::Render(float fTime)
{
	if ( m_aniStatus == ANI_END )
		return;

	vector<ANI_NODE>::iterator  itAni;
	for ( itAni = m_vAni.begin(); itAni != m_vAni.end(); itAni++ )
	{
		if ( itAni->m_iCurFrame >= 0 )
			m_pTexture->RenderTexture(&itAni->m_rcDst, &itAni->m_vrcSrc[itAni->m_iCurFrame]);
	}
}


////////////////////////////////////////////////////////////////////////////////
//
//TEXT_NODE
//
TEXT_NODE::TEXT_NODE(string str, DWORD dwColor)
{
	m_strText = str;
	m_strRenderText = "";
	m_fPosX = 0;
	m_fPosY = 0;
	m_iSpeed = 0;
	m_bPlay = false;
	m_dwTextColor = dwColor;
	m_bDelete = false;
	m_iOffset = 0;
	m_iOffsetLast = 0;
	m_iOffsetLast2 = 0;

	memset(&m_rcText, 0x00, sizeof(m_rcText));
	memset(&m_rcSrc, 0x00, sizeof(m_rcSrc));
	m_pTexture = NULL;
}

TEXT_NODE::~TEXT_NODE()
{
	SAFE_RELEASE(m_pTexture);
}

void    TEXT_NODE::Play()
{
	m_bPlay = true;

	int iWidth1 = 0, iWidth2 = 0;
	iWidth1 = m_rcSrc.right - m_rcSrc.left;
	iWidth2 = g_Font.GetLen("22");

	int iWidth = max(iWidth1, iWidth2);
	m_rcText.right -= iWidth;
	m_fPosX = m_rcText.right;
	m_fPosY = m_rcText.top;

	m_iOffset = 0;
}

void	TEXT_NODE::Process(float fTime)
{
	if ( !m_bPlay )  return;

	USES_CONVERSION;

	std::wstring  wstrText = A2W(m_strText.c_str());
	int iTextWidth = (m_rcText.right - m_fPosX) - (m_rcSrc.right - m_rcSrc.left);
	int iTextWidth2 = (m_rcText.right - m_fPosX);

	bool bOffset1 = false, bOffset2 = false;
	for ( int iOffset_end = 0; iOffset_end <= static_cast<int>(m_strText.size()); iOffset_end++ )
	{
		LPCTSTR lpszText = W2A(wstrText.substr(m_iOffset, iOffset_end).c_str());
		int istrLeght = g_Font.GetLen(lpszText);
		if ( (istrLeght >= iTextWidth && bOffset1 == false)
			|| (m_iOffsetLast == 0 && m_rcSrc.left == m_rcSrc.right) )
		{
			bOffset1 = true;
			m_iOffsetLast = iOffset_end;
		}
		if ( (istrLeght >= iTextWidth2 && bOffset2 == false) 
			|| (m_iOffsetLast2 == 0 && m_rcSrc.left == m_rcSrc.right) )
		{
			bOffset2 = true;
			m_iOffsetLast2 = iOffset_end;
		}
	}

	//
	m_fPosX -= m_iSpeed * fTime;
	if ( m_fPosX <= m_rcText.left )
	{
		m_fPosX = m_rcText.left;

		int iIconWidth = (m_rcSrc.right - m_rcSrc.left);
		int iWidth = g_Font.GetLen(W2A(wstrText.substr(m_iOffset, 1).c_str()));

		if ( m_pTexture )
		{
			m_fPosX += iIconWidth;
			SAFE_RELEASE(m_pTexture);
		}
		else
		{
			m_fPosX += iWidth;
			++m_iOffset;
		}
		m_strRenderText = W2A(wstrText.substr(m_iOffset, m_iOffsetLast2).c_str());
	}
	else
	{
		m_strRenderText = W2A(wstrText.substr(m_iOffset, m_iOffsetLast).c_str());
	}

	if ( m_strRenderText.empty() && m_pTexture == NULL )
	{
		m_bDelete = true;
	}

}

void	TEXT_NODE::Render(float fTime)
{
	if ( !m_bPlay )  return;

	float  fTextPosX = m_fPosX;

	if ( m_pTexture )
	{
		int iTexY = m_fPosY + ((m_rcText.bottom - m_rcText.top) - (m_rcSrc.bottom - m_rcSrc.top))/2;
		m_pTexture->RenderTexture(m_fPosX, iTexY, &m_rcSrc);
		fTextPosX += (m_rcSrc.right - m_rcSrc.left);
	}

	RECT   rcNotice = { fTextPosX, m_fPosY, m_rcText.right, m_rcText.bottom };
	OffsetRect( &rcNotice , 0,
			((m_rcText.bottom - m_rcText.top) - g_pVideo->GetFont(FONT_12_NORMAL)->GetHeight())/2 );

	g_pVideo->GetFont(FONT_12_NORMAL)->SetShadow(true);
	g_pVideo->GetFont(FONT_12_NORMAL)->SetColor(m_dwTextColor);
	g_pVideo->GetFont(FONT_12_NORMAL)->RenderText( m_strRenderText.c_str(), &rcNotice );

}

int     TEXT_NODE::GetLastPosX()
{
	int  iIconWidth = 0;
	if ( m_pTexture )
		iIconWidth = m_rcSrc.right - m_rcSrc.left;

	return (m_fPosX + iIconWidth + g_Font.GetLen(m_strRenderText.c_str()));
}

void    TEXT_NODE::SetPlayInfo(int iSpeed, RECT & rcText)
{
	m_iSpeed = iSpeed;
	m_rcText = rcText;
}

void    TEXT_NODE::LoadTexture(const char* szFileName, int iX, int iY, int iSX, int iSY)
{
	g_pVideo->GetTextureMgr()->LoadTexture( szFileName , &m_pTexture );
	if ( m_pTexture )
	{
		m_rcSrc.left	= iX;
		m_rcSrc.top		= iY;
		m_rcSrc.right   = iSX;
		m_rcSrc.bottom  = iSY;
	}
}

void    TEXT_NODE::LoadTexture(SPTexture * pTexture, int iX, int iY, int iSX, int iSY)
{
	if ( pTexture )
	{
		m_pTexture  = pTexture;
		m_pTexture->IncreaseRef();
		m_rcSrc.left	= iX;
		m_rcSrc.top		= iY;
		m_rcSrc.right   = iSX;
		m_rcSrc.bottom  = iSY;
	}
}


////////////////////////////////////////////////////////////////////////////////
//
//CTextScrollManager
//
CTextScrollManager::CTextScrollManager()
{
	m_iSpeed = 0;
	m_iDistance = 0;
	memset(&m_rcText, 0x00, sizeof(m_rcText));
	memset(&m_rcSrc, 0x00, sizeof(m_rcSrc));
	m_pTexture = NULL;

}

CTextScrollManager::~CTextScrollManager()
{
	SAFE_RELEASE(m_pTexture);
}

void    CTextScrollManager::LoadTexture(const char* szFileName, int iX, int iY, int iSX, int iSY)
{
	g_pVideo->GetTextureMgr()->LoadTexture( szFileName , &m_pTexture );
	m_rcSrc.left	= iX;
	m_rcSrc.top		= iY;
	m_rcSrc.right   = iSX;
	m_rcSrc.bottom  = iSY;
}

void	CTextScrollManager::AddTextNode(TEXT_NODE & text)
{
	text.SetPlayInfo(m_iSpeed, m_rcText);

	if ( m_vecTextNode.empty() )
		text.Play();

	m_vecTextNode.push_back(text);
}

void	CTextScrollManager::Process(float fTime)
{
	std::vector<TEXT_NODE>::iterator  it, it2;
	for ( it = m_vecTextNode.begin(); it != m_vecTextNode.end(); )
	{
		if ( it->isPlay() == false )
			break;

		it2 = it + 1;
		int  iLastPosX = it->GetLastPosX();
		if ( (m_rcText.right - iLastPosX) >= m_iDistance && it2 != m_vecTextNode.end() )
		{
			if ( it2->isPlay() == false )
				it2->Play();
		}

		if ( it->isDelete() )
		{
			it = m_vecTextNode.erase(it);
			continue;
		}

		it->Process(fTime);
		it++;
	}
}

void	CTextScrollManager::Render(float fTime)
{
	if ( m_pTexture )
	{
		RECT   rcSrc = { m_rcSrc.left+1, m_rcSrc.top, m_rcSrc.right-1, m_rcSrc.bottom };
		m_pTexture->RenderTexture(&m_rcText, &rcSrc);
	}

	std::vector<TEXT_NODE>::iterator  it;
	for ( it = m_vecTextNode.begin(); it != m_vecTextNode.end(); it++ )
	{
		it->Render(fTime);
	}

	if ( m_pTexture )
	{
		m_pTexture->RenderTexture(&m_rcDst[0], &m_rcSrc);
		m_pTexture->RenderTexture(&m_rcDst[1], &m_rcSrc);
	}
}

void    CTextScrollManager::SetPlayInfo(int iSpeed, int iDistance, RECT & rcText)
{
	m_iSpeed = iSpeed;
	m_iDistance = iDistance;
	m_rcText = rcText;

	int  iWidth		= m_rcSrc.right - m_rcSrc.left;
	int  iHeight	= m_rcSrc.bottom - m_rcSrc.top;

	m_rcDst[0].left		= rcText.left;
	m_rcDst[0].right	= rcText.left + iWidth;
	m_rcDst[0].top		= rcText.top;
	m_rcDst[0].bottom	= rcText.top + iHeight;

	m_rcDst[1].left		= rcText.right - iWidth;
	m_rcDst[1].right	= rcText.right;
	m_rcDst[1].top		= rcText.top;
	m_rcDst[1].bottom	= rcText.top + iHeight;
}



////////////////////////////////////////////////////////////////////////////////
//
//SPWindowLottery
//
SPWindowLottery::SPWindowLottery(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
{
	Init();
}

SPWindowLottery::SPWindowLottery(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_LOTTERY, InstanceID, iX, iY, iCX, iCY, pParent)	
{
	Init();
}

SPWindowLottery::~SPWindowLottery()
{
	Clean();
}

void SPWindowLottery::Init()
{
	m_strToolTip		=	"[SPWindowLottery]";

	m_pBGTexture = NULL;
	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI110.PNG" , &m_pBGTexture );

	m_pTurnTexture = NULL;
	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI111.PNG" , &m_pTurnTexture );

	ReposOwnImage();

	//////////////////////////////////////////////////////////////////////////

	InitWindowMessageBox();
	InitWindowTurn();
	InitWindowLottery();
	InitWindowSystemMsg();
	InitWindowItemInven();
	InitWindowChat();
	
	//////////////////////////////////////////////////////////////////////////

	InitLDTLoad();

	//////////////////////////////////////////////////////////////////////////

	InitMessageBox();

	//////////////////////////////////////////////////////////////////////////

	m_uiVersion	=	0;
	m_itemVersion = 0;
	m_usServerPoit = 0;
	m_usMyPoit	= 0;
	m_bAcceptFirst = true;

	//////////////////////////////////////////////////////////////////////////

	m_bUse	=	SPLocalizeManager::GetInstance()->IsEnableValue( LV_LOTTERY ) ;

	m_pFXArchive	=	new SPFXArchive( NULL );


	m_iItemInvenCategory = -1;
	for( int i = 0; i < 6; i++ )
	{
		m_iItemInvenMaxPage[i] = 0;
		m_iItemInvenCurPage[i] = 0;
	}

	ClearSelectItem();

	m_iChatBtnDown = 0;
	m_iSystemMsgBtnDown = 0;
	m_fDownLimitTime		=	0.1f;
	m_fDownAccmulateTime	=	0.0f;

	m_fEnterToolTipTime = 0.0f;
	m_fEnterStopTime	= 1.0f;

	m_fTurnRotation		= 0.0f;

	m_bStartTurn = false;
	m_bStopTurn = false;
	m_bTurnStoping = false;
	m_bShowGambleTip = true;
	
	m_iTurnRandom = 0;
	memset(&m_itemIDFirstGetted, 0x00, sizeof(m_itemIDFirstGetted));
	memset(&m_itemIDSecondGetted, 0x00, sizeof(m_itemIDSecondGetted));
	m_vecTurnItem.clear();

	m_vTicketBuffer.clear();
	m_vRandomBuffer.clear();
	m_vecStrName.clear();
	m_vTicketBuffer.resize(LOTTERY_GUIDE_XY_MAX);
	m_vecStrName.resize(LOTTERY_GUIDE_XY_MAX);


	m_aniStart[0].LoadTexture("DATA/INTERFACE/CONCEPT/UI110.PNG", 0, 0, 0, 0);
	m_aniStart[1].LoadTexture("DATA/INTERFACE/CONCEPT/UI110.PNG", 0, 0, 0, 0);

	RECT  rcText = {290, 45, 710, 64};
	m_textScrollManager.LoadTexture("DATA/INTERFACE/CONCEPT/UI110.PNG", 232, 31, 240, 50);
	m_textScrollManager.SetPlayInfo(50, 70, rcText);

	m_ticketAniManager.Init();
}

void SPWindowLottery::ReposOwnImage()
{
	//底色
	m_rcBGSrc[0].left	= 206;
	m_rcBGSrc[0].top	= 32;
	m_rcBGSrc[0].right	= 208;
	m_rcBGSrc[0].bottom = 34;

	//中间白色底
	m_rcBGSrc[1].left	= 201;
	m_rcBGSrc[1].top	= 32;
	m_rcBGSrc[1].right	= 203;
	m_rcBGSrc[1].bottom = 34;

	//花色背景
	m_rcBGSrc[2].left	= 365;
	m_rcBGSrc[2].top	= 1;
	m_rcBGSrc[2].right	= 483;
	m_rcBGSrc[2].bottom = 165;

	//横虚线
	m_rcBGSrc[3].left	= 234;
	m_rcBGSrc[3].top	= 66;
	m_rcBGSrc[3].right	= 350;
	m_rcBGSrc[3].bottom = 67;

	//竖虚线
	m_rcBGSrc[4].left	= 509;
	m_rcBGSrc[4].top	= 1;
	m_rcBGSrc[4].right	= 510;
	m_rcBGSrc[4].bottom = 106;

	//花色背景(WindowTurn)
	m_rcBGSrc[5].left	= 365;
	m_rcBGSrc[5].top	= 1;
	m_rcBGSrc[5].right	= 465;
	m_rcBGSrc[5].bottom = 154;

	//
	for ( int i = 0; i < 6; i++ )
		m_rcBGDst[i].clear();

	int iWidth1		=	m_rcBGSrc[2].right - m_rcBGSrc[2].left;
	int iHeight1	=	m_rcBGSrc[2].bottom - m_rcBGSrc[2].top;
	int iWidth2		=	m_rcBGSrc[3].right - m_rcBGSrc[3].left;
	int iHeight2	=	m_rcBGSrc[3].bottom - m_rcBGSrc[3].top;
	int iWidth3		=	m_rcBGSrc[4].right - m_rcBGSrc[4].left;
	int iHeight3	=	m_rcBGSrc[4].bottom - m_rcBGSrc[4].top;
	int iWidth4		=	m_rcBGSrc[5].right - m_rcBGSrc[5].left;
	int iHeight4	=	m_rcBGSrc[5].bottom - m_rcBGSrc[5].top;

	RECT  rcDst = { 0, 0, 800, 600 };
	m_rcBGDst[0].push_back( rcDst );
	RECT  rcDst2 = { 205, 35, 794, 541 };
	m_rcBGDst[1].push_back( rcDst2 );

	int  iStartX = 205, iEndX = 794;
	int  iStartY = 35,  iEndY = 541;
	for ( int x = iStartX, ix = 0; x < iEndX; x+=iWidth1, ix++ )
	{
		int iPosX	= iStartX + (ix * iWidth1);
		for ( int y = iStartY, iy = 0; y < iEndY; y+=iHeight1, iy++ )
		{
			int iPosY		= iStartY + (iy * iHeight1);
			rcDst.left		= iPosX;
			rcDst.top		= iPosY;
			rcDst.right		= iPosX + iWidth1;
			rcDst.bottom	= iPosY + iHeight1;
			m_rcBGDst[2].push_back( rcDst );
		}
	}

	for ( int x = 0; x < 4; x++ )
	{
		int iPosX	= 229 + (x * (iWidth2 + 25));
		for ( int y = 0; y < 3; y++ )
		{
			int iPosY	= 184 + (y * (iHeight2 + 117));
			rcDst.left		= iPosX;
			rcDst.top		= iPosY;
			rcDst.right		= iPosX + iWidth2;
			rcDst.bottom	= iPosY + iHeight2;
			m_rcBGDst[3].push_back( rcDst );
		}
	}

	for ( int x = 0; x < 3; x++ )
	{
		int iPosX	= 357 + (x * (iWidth3 + 140));
		for ( int y = 0; y < 4; y++ )
		{
			int iPosY	= 73 + (y * (iHeight3 + 14));
			rcDst.left		= iPosX;
			rcDst.top		= iPosY;
			rcDst.right		= iPosX + iWidth3;
			rcDst.bottom	= iPosY + iHeight3;
			m_rcBGDst[4].push_back( rcDst );
		}
	}

	//花色背景(WindowTurn)
	iStartX = 328, iEndX = 628;
	iStartY = 162,  iEndY = 467;
	for ( int x = iStartX, ix = 0; x < iEndX; x+=iWidth4, ix++ )
	{
		int iPosX	= iStartX + (ix * iWidth4);
		for ( int y = iStartY, iy = 0; y < iEndY; y+=iHeight4, iy++ )
		{
			int iPosY		= iStartY + (iy * iHeight4);
			rcDst.left		= iPosX;
			rcDst.top		= iPosY;
			rcDst.right		= iPosX + iWidth4;
			rcDst.bottom	= iPosY + iHeight4;
			m_rcBGDst[5].push_back( rcDst );
		}
	}

	//转盘
	m_rcTurnSrc.left	= 285;	//284;
	m_rcTurnSrc.top		= 2;	//1;
	m_rcTurnSrc.right	= 384;	//385;
	m_rcTurnSrc.bottom  = 101;	//102;

	m_rcTurnDst.left	= 428;	//427;
	m_rcTurnDst.top		= 393;	//392;
	m_rcTurnDst.right	= 527;	//528;
	m_rcTurnDst.bottom	= 492;	//493;

}

void SPWindowLottery::Clean()
{
	SPWindow::Clean();

	SAFE_DELETE(m_pFXArchive);

	SAFE_RELEASE( m_pBGTexture );

}

void SPWindowLottery::Show(bool bWithChild)
{
	if( m_bUse == false )
		return;

	m_iItemInvenCategory = -1;
	ZeroMemory(m_iItemInvenCurPage, sizeof(int) * 6);
	ZeroMemory(m_iItemInvenMaxPage, sizeof(int) * 6);

	ClearSelectItem();

	if(m_bShow)
		g_pInterfaceManager->RegisterSoundUnit("DATA/SOUND/SYSTEM_CLOSE_WINDOW.WAV");

	SetFocus();

	SPWindow::Show(bWithChild);

	UpdateMessageBox();
	UpdateTicket();

	HideWindowTurn();

	SetItemInvenCategory(0);

	m_iChatBtnDown = 0;
	m_iSystemMsgBtnDown = 0;

	m_aniStart[0].Start(0, 0, -400, 0);
	m_aniStart[1].Start(400, 0, 800, 0);

}

void SPWindowLottery::Hide(bool bSendServer)
{
	if(m_bShow)
		g_pInterfaceManager->RegisterSoundUnit("DATA/SOUND/SYSTEM_CLOSE_WINDOW.WAV");

	SPWindow::Hide(bSendServer);

	int	iCursorX	=	g_pInterfaceManager->GetCursorX();
	int	iCursorY	=	g_pInterfaceManager->GetCursorY();

	if( IsCursorIn( iCursorX , iCursorY ) == TRUE )
	{
		SPWindow*	pWindowToolTip	=	g_pInterfaceManager->GetToolTipWindow();

		if( pWindowToolTip != NULL )
			pWindowToolTip->SPSendMessage( SPIM_TOOLTIP_OUT , NULL , -2 );
	}

	if( m_pFXArchive )
		m_pFXArchive->Clean();

}

void SPWindowLottery::Process(float fTime)
{
	ProcessMessageBox( fTime );

	ProcessWindowTurn( fTime );

	m_textScrollManager.Process( fTime );

	//m_ticketAniManager.Process( fTime );

	if( m_bShow == FALSE ) return;

	if( m_pFXArchive != NULL )
	{
		m_pFXArchive->Process( fTime );
	}

	SPWindow::Process( fTime );

	if( m_iChatBtnDown > 0 || m_iSystemMsgBtnDown > 0 )
	{
		m_fDownAccmulateTime	+=	fTime;

		if( m_fDownAccmulateTime >= m_fDownLimitTime )
		{
			m_fDownAccmulateTime	=	0.0f;

			BtnPush( WIID_LOTTERY_SYSTEMMSG_LIST,	m_iSystemMsgBtnDown );
			BtnPush( WIID_LOTTERY_CHAT_LIST,		m_iChatBtnDown );
		}
	}

	static bool  s_bToolTipMove = false;
	if  ( m_fEnterToolTipTime != 0.0f )
	{
		m_fEnterToolTipTime += fTime;
		if ( m_fEnterToolTipTime >= m_fEnterStopTime && !s_bToolTipMove )
		{
			s_bToolTipMove = true;
			SPWindow* pkWindow = g_pInterfaceManager->GetActiveWindow();
			if( pkWindow )
				pkWindow->SPSendMessage(SPIM_TOOLTIP_MOVE, NULL, NULL);
		}
	}
	else
	{
		s_bToolTipMove = false;
	}

	//播节点特效
	static  float   s_ServerPoitFXTime = 0.0f;
	s_ServerPoitFXTime += fTime;
	if ( m_usServerPoit && s_ServerPoitFXTime >= 30.0f )
	{
		if ( m_pFXArchive->IsEmpty() )
		{
			s_ServerPoitFXTime = 0.0f;
			m_pFXArchive->Clean();
			m_pFXArchive->InsertFXGroup( (SPID_FX_GROUP)70000 );
		}
	}

}

void SPWindowLottery::Render(float fTime)
{
	if( m_bShow == FALSE ) return;

	g_pVideo->Flush();

	//背景
	if ( m_pBGTexture )
	{
		for ( int i = 0; i < 5; i++ )
		{
			for ( int j = 0; j < m_rcBGDst[i].size(); j++ )
			{
				m_pBGTexture->RenderTexture(&m_rcBGDst[i].at(j), &m_rcBGSrc[i]);
			}
		}
	}

	if( m_pMessageBox != NULL )
		m_pMessageBox->Hide();

	m_textScrollManager.Render( fTime );
	SPWindow::RenderReverse( fTime );
	//m_ticketAniManager.Render( fTime );

	//花色背景(WindowTurn)
	SPWindow *	pTurnParent = Find(WIID_LOTTERY_TURN_FRAME, true);
	if ( pTurnParent && pTurnParent->IsShow() )
	{
		if ( m_pBGTexture )
		{
			for ( int i = 0; i < m_rcBGDst[5].size(); i++ )
			{
				m_pBGTexture->RenderTexture(&m_rcBGDst[5].at(i), &m_rcBGSrc[5]);
			}
		}
		pTurnParent->Render(fTime);
	}

	//转盘动画
	if ( m_pTurnTexture && pTurnParent && pTurnParent->IsShow() )
	{
		m_pTurnTexture->RenderTexture(&m_rcTurnDst, &m_rcTurnSrc, m_fTurnRotation);
	}

	if( m_pMessageBox != NULL && m_eState != LOTTERY_STATE_NORMAL )
	{
		m_pMessageBox->Show();
		m_pMessageBox->RenderReverse( fTime );
		g_pVideo->Flush();
	}

	if( m_pFXArchive != NULL )
		m_pFXArchive->Render( fTime );

	m_aniStart[0].Render(fTime);
	m_aniStart[1].Render(fTime);

}

void SPWindowLottery::RefreshRelationCoord()
{
	SPWindow::RefreshRelationCoord();
}

SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowLottery )
	SPIMESSAGE_COMMAND(	SPIM_CURSOR_ENTER			,						OnCursorEnter		)
	SPIMESSAGE_COMMAND(	SPIM_CURSOR_OUT				,						OnCursorOut			)
	SPIMESSAGE_COMMAND(	SPIM_CURSOR_MOVE			,						OnCursorMove		)

	SPIMESSAGE_COMMAND( SPIM_LOTTERY_STATUS			,						OnLotteryStatus		)
	SPIMESSAGE_COMMAND( SPIM_LOTTERY_UPDATE_ITEM	,						OnLotteryUpdateItem	)
	SPIMESSAGE_COMMAND( SPIM_LOTTERY_BUY			,						OnLotteryBuy		)
	SPIMESSAGE_COMMAND( SPIM_LOTTERY_UNBUY			,						OnLotteryUnBuy		)
	SPIMESSAGE_COMMAND( SPIM_LOTTERY_ACCEPT			,						OnLotteryAcceptItem	)
	SPIMESSAGE_COMMAND( SPIM_LOTTERY_ACCEPTSECOND	,						OnLotteryAcceptItemSecond	)
	SPIMESSAGE_COMMAND( SPIM_LOTTERY_INITIAL		,						OnLotteryInitial	)
	SPIMESSAGE_COMMAND( SPIM_LOTTERY_BROADCAST		,						OnLotteryBroadCast	)
	SPIMESSAGE_COMMAND( SPIM_LOTTERY_SETNAME		,						OnLotterySetName	)

	SPIMESSAGE_COMMAND(	SPIM_PURGE					,						OnPurge				)
	SPIMESSAGE_COMMAND( SPIM_LOTTERY_CLEAR_DRAG		,						OnClearDrag			)
	SPIMESSAGE_COMMAND( SPIM_LOTTERY_REFRESH_ITEMINVEN,						OnReflashItemInven	)
	SPIMESSAGE_COMMAND( SPIM_LOTTERY_REFRESH		,						OnRefresh			)

	SPIMESSAGE_COMMAND( SPIM_LOTTERY_LOAD_TIP		,						OnLoadGambleTip		)
	SPIMESSAGE_COMMAND( SPIM_LOTTERY_SAVE_TIP		,						OnSaveGambleTip		)

	SPIMESSAGE_CONTROL( SPIM_WHEEL_UP		,	WIID_LOTTERY_CHAT_LIST				,	OnChatWheelUp				)			
	SPIMESSAGE_CONTROL( SPIM_WHEEL_DOWN		,	WIID_LOTTERY_CHAT_LIST				,	OnChatWheelDown				)
	SPIMESSAGE_CONTROL( SPIM_WHEEL_UP		,	WIID_LOTTERY_SYSTEMMSG_LIST			,	OnSystemMsgWheelUp			)			
	SPIMESSAGE_CONTROL( SPIM_WHEEL_DOWN		,	WIID_LOTTERY_SYSTEMMSG_LIST			,	OnSystemMsgWheelDown		)

	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP		,	WIID_LOTTERY_CHAT_UP				,	OnChatScrollUp				)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP		,	WIID_LOTTERY_CHAT_DOWN				,	OnChatScrollDown			)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP		,	WIID_LOTTERY_CHAT_BOTTOM			,	OnChatScrollLatest			)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP		,	WIID_LOTTERY_SYSTEMMSG_UP			,	OnSystemMsgScrollUp			)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP		,	WIID_LOTTERY_SYSTEMMSG_DOWN			,	OnSystemMsgScrollDown		)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP		,	WIID_LOTTERY_SYSTEMMSG_BOTTOM		,	OnSystemMsgScrollLatest		)

	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_DOWN	,	WIID_LOTTERY_CHAT_UP				,	OnChatScrollUpLBDown		)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_DOWN	,	WIID_LOTTERY_CHAT_DOWN				,	OnChatScrollDownLBDown		)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_DOWN	,	WIID_LOTTERY_SYSTEMMSG_UP			,	OnSystemMsgScrollUpLBDown	)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_DOWN	,	WIID_LOTTERY_SYSTEMMSG_DOWN			,	OnSystemMsgScrollDownLBDown	)

	SPIMESSAGE_CONTROL(	SPIM_CURSOR_OUT		,	WIID_LOTTERY_CHAT_UP				,	OnChatScrollUpCursorOut		)
	SPIMESSAGE_CONTROL(	SPIM_CURSOR_OUT		,	WIID_LOTTERY_CHAT_DOWN				,	OnChatScrollDownCursorOut	)
	SPIMESSAGE_CONTROL(	SPIM_CURSOR_OUT		,	WIID_LOTTERY_SYSTEMMSG_UP			,	OnSystemMsgScrollUpCursorOut	)
	SPIMESSAGE_CONTROL(	SPIM_CURSOR_OUT		,	WIID_LOTTERY_SYSTEMMSG_DOWN			,	OnSystemMsgScrollDownCursorOut	)


	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_LOTTERY_MESSAGE_BOX_OK		,	OnMessageOk			)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_LOTTERY_MESSAGE_BOX_YES	,	OnMessageYes		)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_LOTTERY_MESSAGE_BOX_NO		,	OnMessageNo			)

	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_LOTTERY_TURN_TIP_CHECK		,	OnTurnTipCheck		)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_LOTTERY_TURN_TIP_CLOSE		,	OnTurnTipClose		)

	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_LOTTERY_CLOSE_ICON			,	OnClose				)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_LOTTERY_REFRESH			,	OnRefresh			)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_LOTTERY_BUY_TICKET			,	OnBuy				)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_LOTTERY_RANDOM_TICKET		,	OnRandom			)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_DOWN			,	WIID_LOTTERY_TURN_CONTINUE		,	OnContinue			)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_DOWN			,	WIID_LOTTERY_TURN_ACCEPT		,	OnAccept			)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_LOTTERY_TURN_STOP			,	OnStopTurn			)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_LOTTERY_BUY_ITEM			,	OnItemBuy			)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_LOTTERY_FILLING			,	OnFilling			)

	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP				,	WIID_LOTTERY_ITEM_INVEN_UP		,	OnItemInvenUp		)
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP				,	WIID_LOTTERY_ITEM_INVEN_DOWN	,	OnItemInvenDown		)
	SPIMESSAGE_CONTROL( SPIM_SLIDER_UP				,	WIID_LOTTERY_ITEM_INVEN_THUMB	,	OnItemInvenUp		)
	SPIMESSAGE_CONTROL( SPIM_SLIDER_DOWN			,	WIID_LOTTERY_ITEM_INVEN_THUMB	,	OnItemInvenDown		)
	SPIMESSAGE_CONTROL( SPIM_SLIDER_PAGEUP			,	WIID_LOTTERY_ITEM_INVEN_THUMB	,	OnItemInvenUp		)
	SPIMESSAGE_CONTROL( SPIM_SLIDER_PAGEDOWN		,	WIID_LOTTERY_ITEM_INVEN_THUMB	,	OnItemInvenDown		)
	SPIMESSAGE_CONTROL( SPIM_SET_CURVALUE			,	WIID_LOTTERY_ITEM_INVEN_THUMB	,	OnSetItemInvenCurPage	)
	SPIMESSAGE_CONTROL( SPIM_WHEEL_UP				,	WIID_LOTTERY_ITEM_INVEN_SLOT	,	OnSetItemInvenUp		)
	SPIMESSAGE_CONTROL( SPIM_WHEEL_DOWN				,	WIID_LOTTERY_ITEM_INVEN_SLOT	,	OnSetItemInvenDown		)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_DOWN			,	WIID_LOTTERY_ITEM_INVEN_SLOT	, 	OnItemInvenLButtonDown	)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP				,	WIID_LOTTERY_ITEM_INVEN_SLOT	, 	OnItemInvenLButtonUp	)
	SPIMESSAGE_CONTROL(	SPIM_CURSOR_MOVE			,	WIID_LOTTERY_ITEM_INVEN_SLOT	, 	OnItemInvenCursorMove	)
	SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_ENTER			,	WIID_LOTTERY_ITEM_INVEN_SLOT	,	OnItemInvenToolTipEnter	)
	SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_OUT			,	WIID_LOTTERY_ITEM_INVEN_SLOT	,	OnItemInvenToolTipOut	)
	SPIMESSAGE_CONTROL(	SPIM_TOOLTIP_MOVE			,	WIID_LOTTERY_ITEM_INVEN_SLOT	,	OnItemInvenToolTipMove	)

	SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_ENTER	,	WIID_LOTTERY_TICKET_SELL ,	WIID_LOTTERY_TICKET_SELL + 399	,		OnTicketToolTipEnter	)
	SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_OUT		,	WIID_LOTTERY_TICKET_SELL ,	WIID_LOTTERY_TICKET_SELL + 399	,		OnTicketToolTipOut		)
	SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_MOVE		,	WIID_LOTTERY_TICKET_SELL ,	WIID_LOTTERY_TICKET_SELL + 399	,		OnTicketToolTipMove		)
	SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_ENTER	,	WIID_LOTTERY_TURN_LIGHT	 ,	WIID_LOTTERY_TURN_LIGHT	+ 8		,		OnTurnToolTipEnter		)
	SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_OUT		,	WIID_LOTTERY_TURN_LIGHT	 ,	WIID_LOTTERY_TURN_LIGHT	+ 8		,		OnTurnToolTipOut		)
	SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_MOVE		,	WIID_LOTTERY_TURN_LIGHT	 ,	WIID_LOTTERY_TURN_LIGHT	+ 8		,		OnTurnToolTipMove		)
	SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_ENTER	,	WIID_LOTTERY_TURN_ITEM_BG,	WIID_LOTTERY_TURN_ITEM_BG	+ 8	,		OnTurnToolTipEnter		)
	SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_OUT		,	WIID_LOTTERY_TURN_ITEM_BG,	WIID_LOTTERY_TURN_ITEM_BG	+ 8	,		OnTurnToolTipOut		)
	SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_MOVE		,	WIID_LOTTERY_TURN_ITEM_BG,	WIID_LOTTERY_TURN_ITEM_BG	+ 8	,		OnTurnToolTipMove		)
	SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_ENTER	,	WIID_LOTTERY_TURN_ITEM	 ,	WIID_LOTTERY_TURN_ITEM	+ 8		,		OnTurnToolTipEnter		)
	SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_OUT		,	WIID_LOTTERY_TURN_ITEM	 ,	WIID_LOTTERY_TURN_ITEM	+ 8		,		OnTurnToolTipOut		)
	SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_MOVE		,	WIID_LOTTERY_TURN_ITEM	 ,	WIID_LOTTERY_TURN_ITEM	+ 8		,		OnTurnToolTipMove		)
	SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_ENTER	,	WIID_LOTTERY_TURN_DISABLE,	WIID_LOTTERY_TURN_DISABLE	+ 8	,		OnTurnToolTipEnter		)
	SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_OUT		,	WIID_LOTTERY_TURN_DISABLE,	WIID_LOTTERY_TURN_DISABLE	+ 8	,		OnTurnToolTipOut		)
	SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_MOVE		,	WIID_LOTTERY_TURN_DISABLE,	WIID_LOTTERY_TURN_DISABLE	+ 8	,		OnTurnToolTipMove		)

	SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP		,	WIID_LOTTERY_TICKET		,	WIID_LOTTERY_TICKET + LOTTERY_GUIDE_X_MAX * LOTTERY_GUIDE_Y_MAX	,	OnLButtonUP			)
	SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_DBLCLICK	,	WIID_LOTTERY_TICKET		,	WIID_LOTTERY_TICKET + LOTTERY_GUIDE_X_MAX * LOTTERY_GUIDE_Y_MAX	,	OnLButtonDblClick	)

	SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP		,	WIID_LOTTERY_ITEM_INVEN_EQUIP,	WIID_LOTTERY_ITEM_INVEN_HOUSING,		OnInvenCategory)

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)

int SPWindowLottery::OnCursorEnter( WPARAM wParam, LPARAM lParam )
{
	OnCursorMove( wParam , lParam );
	return 1;
}

int SPWindowLottery::OnCursorOut( WPARAM wParam, LPARAM lParam )
{
	if( m_bShow == false )
		return 1;

	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, -2 );
	return 1;
}

int SPWindowLottery::OnCursorMove( WPARAM wParam, LPARAM lParam )
{
	if( m_bShow == false )
		return 1;

	if( IsCursorIn() == false )
		return 1;

	SPWindow*		pToolTipWindow	=	g_pInterfaceManager->GetToolTipWindow();

	if( m_eState != LOTTERY_STATE_NORMAL )
	{
		if( pToolTipWindow != NULL )
		{
			if( pToolTipWindow->IsShow() == true )
				pToolTipWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, -2 );
		}

		return 1;
	}

	return 1;
}

int SPWindowLottery::OnLotteryStatus(WPARAM wParam, LPARAM lParam)
{
	CPacket*		pPacket		=	(CPacket*)lParam;

	if( g_pCheckManager != NULL )
		g_pCheckManager->SetDBBlock(false);

	if( pPacket == NULL )
		return 1;

	GLOBAL_STRING_ID	iErrorID;
	unsigned short		usFinished;    // 是否发送完毕
	UINT64				GambleVersion;
	UINT				count;
	
	pPacket->ExtractUINT32( &iErrorID );
	pPacket->Extract( &usFinished, sizeof(unsigned short) );
	pPacket->ExtractUINT64( &m_uiVersion );
	pPacket->Extract( &m_usServerPoit, sizeof(unsigned short) );
	pPacket->ExtractUINT32( &count );

	if ( count > 0 )
	{
		t_Gamble_Status	*	pGambleStatus = new t_Gamble_Status[ count ];
		pPacket->ExtractStruct( pGambleStatus , count * sizeof( t_Gamble_Status ) );
		SetTicketData(count, pGambleStatus);

		SAFE_DELETE_ARRAY( pGambleStatus );
	}

	if ( usFinished )
	{
		UpdateTicket();
		HideMessageBox();
		
		if ( m_usServerPoit )
		{
			m_pFXArchive->Clean();
			m_pFXArchive->InsertFXGroup( (SPID_FX_GROUP)70000 );
		}
	}

	return 1;
}

int SPWindowLottery::OnLotteryUpdateItem(WPARAM wParam, LPARAM lParam)
{
	CPacket*		pPacket		=	(CPacket*)lParam;
	if( pPacket == NULL )
		return 1;

	UINT64	uiVersion			=	0;
	t_Gamble_Status				status;			//	百宝箱的状态
	char	szName[LEN_NAME+1];

	pPacket->ExtractUINT64( &uiVersion );
	pPacket->Extract(&status, sizeof(status));
	pPacket->Extract(szName, sizeof(szName));

	//if ( uiVersion != m_uiVersion)
	//{
	//	const char*		pszGlobalString	=	g_pResourceManager->GetGlobalString( 13001429 );
	//	if( pszGlobalString != NULL )
	//	{
	//		AddChatMessage(pszGlobalString);
	//	}
	//	return 1;
	//}

	SetTicketName(status.uGambleIndex, szName);

	if ( status.uGambleIndex < m_vTicketBuffer.size() )
	{
		m_vTicketBuffer.at(status.uGambleIndex) = status;
		UpdateTicket(status.uGambleIndex);

		STD_VECTOR_LOTTERY_TICKET::iterator  it =
				find_if (m_vRandomBuffer.begin(), m_vRandomBuffer.end(), FindNode(status.uGambleIndex, t_Gamble_Status, uGambleIndex, UINT));

		if ( it == m_vRandomBuffer.end() )
		{
			if ( status.nBuyStep == 0 )
			{
				m_vRandomBuffer.push_back( status );
			}
		}
		else
		{
			if ( status.nBuyStep > 0 )
			{
				m_vRandomBuffer.erase(it);
			}
		}
	}

	return 1;
}

int SPWindowLottery::OnLotteryBuy(WPARAM wParam, LPARAM lParam)
{
	CPacket*		pPacket		=	(CPacket*)lParam;

	if( g_pCheckManager != NULL )
		g_pCheckManager->SetDBBlock( false );

	if( pPacket == NULL )
		return 1;

	GLOBAL_STRING_ID	iErrorID;			//	0为没有错误，1为版本出错，客户端需要更新百宝箱
	CHARID				charID;				//	玩家的ID号
	UINT64				gambleVersion;		//	版本号
	unsigned short      usPoit;				//	是否是节点购买
	UINT8				uCount;				//	第二次物品的总数

	pPacket->ExtractUINT32( &iErrorID );

	if( iErrorID != 0 )
	{
		PrintErrorMsg( iErrorID );
		return 1;
	}

	pPacket->ExtractUINT32( &charID );
	pPacket->ExtractUINT64( &gambleVersion );
	pPacket->Extract( &m_itemIDFirstGetted, sizeof(m_itemIDFirstGetted) );
	pPacket->Extract( &usPoit, sizeof(unsigned short) );
	pPacket->ExtractUINT8(&uCount);

	if ( uCount > 0 )
	{
		m_vecTurnItem.clear();
		t_Gamble_Getted_Item *	pGambleItem = new t_Gamble_Getted_Item[uCount];
		pPacket->ExtractStruct(pGambleItem, uCount * sizeof(t_Gamble_Getted_Item));
		for ( int i=0; i<uCount; i++ )
		{
			m_vecTurnItem.push_back(pGambleItem[i]);
		}
		SAFE_DELETE_ARRAY( pGambleItem );
	}
	if ( uCount < 8 )
	{
		m_vecTurnItem.resize(8);
	}

	memset(&m_itemIDSecondGetted, 0x00, sizeof(m_itemIDSecondGetted));
	m_itemVersion = gambleVersion;
	m_usMyPoit	= usPoit;
	m_bAcceptFirst = true;

	//if ( gambleVersion != m_uiVersion )
	//{
	//	const char*		pszGlobalString	=	g_pResourceManager->GetGlobalString( 13001429 );
	//	if( pszGlobalString != NULL )
	//	{
	//		AddChatMessage(pszGlobalString);
	//	}
	//	return 1;
	//}

	ShowWindowTurn();
	ClearSelectTicket();
	HideMessageBox();
	return 1;

}

int SPWindowLottery::OnLotteryUnBuy(WPARAM wParam, LPARAM lParam)
{
	CPacket*		pPacket		=	(CPacket*)lParam;

	if( pPacket == NULL )
		return 1;

	GLOBAL_STRING_ID	iErrorID;			//	errorID == 0 表示没有发生错误，只是有玩家取了购买
	UINT64				gambleVersion;		//	版本号
	UINT				uIndex;				//	百宝箱的某个位置

	pPacket->ExtractUINT32( &iErrorID );
	pPacket->ExtractUINT64( &gambleVersion );
	pPacket->ExtractUINT32( &uIndex );

	//if ( gambleVersion != m_uiVersion )
	//{
	//	const char*		pszGlobalString	=	g_pResourceManager->GetGlobalString( 13001429 );
	//	if( pszGlobalString != NULL )
	//	{
	//		AddChatMessage(pszGlobalString);
	//	}
	//	return 1;
	//}

	if ( iErrorID == 0 )
	{
		m_vTicketBuffer.at(uIndex).nBuyStep = 0;
		m_vTicketBuffer.at(uIndex).itemID = 0;
		m_vTicketBuffer.at(uIndex).uSetID = 0;
		UpdateTicket(uIndex);

		STD_VECTOR_LOTTERY_TICKET::iterator  it =
				find_if (m_vRandomBuffer.begin(), m_vRandomBuffer.end(), FindNode(uIndex, t_Gamble_Status, uGambleIndex, UINT));

		if ( it == m_vRandomBuffer.end() )
		{
			t_Gamble_Status status;
			status.uGambleIndex = uIndex;
			m_vRandomBuffer.push_back( status );
		}
	}

	return 1;
}

int SPWindowLottery::OnLotteryAcceptItem(WPARAM wParam, LPARAM lParam)
{
	CPacket*		pPacket		=	(CPacket*)lParam;

	if( g_pCheckManager != NULL )
		g_pCheckManager->SetDBBlock( false );

	if( pPacket == NULL )
		return 1;

	GLOBAL_STRING_ID		iErrorID;	//	错误信息
	UINT8					uAccept;	//	1为后面的物品为第一次获得的奖励，2为第二次供选择的物品
	t_Gamble_Getted_Item	gambleItem;	//	物品的信息

	pPacket->ExtractUINT32(&iErrorID);
	pPacket->ExtractUINT8(&uAccept);
	pPacket->Extract(&gambleItem, sizeof(gambleItem));

	if ( iErrorID != 0 )
	{
		PrintErrorMsg(iErrorID);
		return 1;
	}

	if ( uAccept == 1 )
	{
		SPItemAttr * pkItemAttr = NULL;
		g_pItemCluster->GetItemInfo(gambleItem.itemID, pkItemAttr);
		if ( pkItemAttr )
		{
			const char*		pszGlobalString	=	g_pResourceManager->GetGlobalString( 13001430 );
			if( pszGlobalString != NULL )
			{
				char    szMsg[256];
				sprintf(szMsg, pszGlobalString, pkItemAttr->m_strName.c_str());
				AddChatMessage(szMsg, gambleItem.uSetID);
			}
		}
		//PlayLotteryFx(gambleItem.uSetID);
	}
	else if ( uAccept == 2 )  //第二次抽奖
	{
		m_itemIDSecondGetted = gambleItem;
		for ( int i=0; i < m_vecTurnItem.size(); i++)
		{
			if ( m_vecTurnItem[i].itemID == m_itemIDSecondGetted.itemID )
			{
				m_iTurnRandom = i;
				break;
			}
		}
		StartTurn();
	}

	return 1;
}

int SPWindowLottery::OnLotteryAcceptItemSecond(WPARAM wParam, LPARAM lParam)
{
	CPacket*		pPacket		=	(CPacket*)lParam;
	if( pPacket == NULL )
		return 1;

	GLOBAL_STRING_ID		iErrorID;		//	错误信息
	t_Gamble_Getted_Item	gambleItem;		//	物品的信息

	pPacket->ExtractUINT32(&iErrorID);
	pPacket->Extract(&gambleItem, sizeof(gambleItem));

	if ( iErrorID != 0 )
	{
		PrintErrorMsg(iErrorID);
		return 1;
	}

	SPItemAttr * pkItemAttr = NULL;
	g_pItemCluster->GetItemInfo(gambleItem.itemID, pkItemAttr);
	if ( pkItemAttr )
	{
		const char*		pszGlobalString	=	g_pResourceManager->GetGlobalString( 13001430 );
		if( pszGlobalString != NULL )
		{
			char    szMsg[256];
			sprintf(szMsg, pszGlobalString, pkItemAttr->m_strName.c_str());
			AddChatMessage(szMsg, gambleItem.uSetID);
		}
	}

	return 1;
}

int SPWindowLottery::OnLotteryInitial(WPARAM wParam, LPARAM lParam)
{
	if( IsShow() == true )
	{
		PrintErrorMsg( 13001205 );

		m_vTicketBuffer.clear();
		m_vRandomBuffer.clear();
		m_vecStrName.clear();
		m_vTicketBuffer.resize(LOTTERY_GUIDE_XY_MAX);
		m_vecStrName.resize(LOTTERY_GUIDE_XY_MAX);

		ClearSelectTicket();

		for ( int i = 0; i < LOTTERY_GUIDE_XY_MAX; i++ )
		{
			m_vTicketBuffer.at(i).uGambleIndex = i;
			m_vRandomBuffer.push_back( m_vTicketBuffer[i] );
		}
		UpdateTicket();
	}

	CPacket*		pPacket		=	(CPacket*)lParam;
	if( pPacket == NULL )
		return 1;

	pPacket->ExtractUINT64( &m_uiVersion );
	pPacket->Extract( &m_usServerPoit, sizeof(unsigned short) );

	if ( m_usServerPoit )
	{
		m_pFXArchive->Clean();
		m_pFXArchive->InsertFXGroup( (SPID_FX_GROUP)70000 );
	}

	return 1;
}

int	SPWindowLottery::OnLotteryBroadCast(WPARAM wParam, LPARAM lParam)
{
	CPacket*		pPacket		=	(CPacket*)lParam;
	if( pPacket == NULL )
		return 1;

	char					szName[LEN_NAME + 1];	//	获得物品的玩家的名字
	t_Gamble_Getted_Item	gambleItem;				//	获得的物品的ID

	pPacket->Extract(szName, sizeof(szName));
	pPacket->Extract(&gambleItem, sizeof(gambleItem));

	SPItemAttr * pkItemAttr = NULL;
	g_pItemCluster->GetItemInfo(gambleItem.itemID, pkItemAttr);

	if ( pkItemAttr )
	{
		char    szMsg[256] = "";
		if ( gambleItem.usCount > 1 )
		{
			const char*		pszGlobalString	=	g_pResourceManager->GetGlobalString( 13001434 );
			if( pszGlobalString != NULL )
			{
				sprintf(szMsg, pszGlobalString, szName, pkItemAttr->m_strName.c_str(), gambleItem.usCount);
			}
		}
		else
		{
			const char*		pszGlobalString	=	g_pResourceManager->GetGlobalString( 13001431 );
			if( pszGlobalString != NULL )
			{
				sprintf(szMsg, pszGlobalString, szName, pkItemAttr->m_strName.c_str());
			}
		}

		if ( strlen(szMsg) > 0 )
		{
			AddSystemMessage(szMsg, gambleItem.uSetID);
			AddScrollMessage(szMsg, gambleItem.uSetID);
		}
	}

	return 1;
}

int	SPWindowLottery::OnLotterySetName(WPARAM wParam, LPARAM lParam)
{
	CPacket*		pPacket		=	(CPacket*)lParam;
	if( pPacket == NULL )
		return 1;

	UINT32	uIndex;					//	位置
	char    szName[LEN_NAME + 1];	//	名字

	pPacket->ExtractUINT32(&uIndex);
	pPacket->Extract(szName, sizeof(szName));

	SetTicketName(uIndex, szName);

}

void  SPWindowLottery::SetTicketName(int  iIndex, const char * szName)
{
	if ( iIndex < m_vecStrName.size() )
		m_vecStrName[iIndex] = szName;
}

const char *  SPWindowLottery::GetTicketName(int  iIndex)
{
	if ( iIndex < m_vecStrName.size() )
		return m_vecStrName[iIndex].c_str();

	return NULL;
}

int SPWindowLottery::OnPurge( WPARAM wParam, LPARAM lParam )
{
	m_uiVersion	=	0;
	m_itemVersion = 0;
	m_usServerPoit = 0;
	m_usMyPoit	= 0;
	m_bAcceptFirst = true;

	m_iTurnRandom = 0;
	memset(&m_itemIDFirstGetted, 0x00, sizeof(m_itemIDFirstGetted));
	memset(&m_itemIDSecondGetted, 0x00, sizeof(m_itemIDSecondGetted));
	m_vecTurnItem.clear();

	m_fEnterToolTipTime = 0.0f;

	m_vTicketBuffer.clear();
	m_vRandomBuffer.clear();
	m_vecStrName.clear();
	m_vTicketBuffer.resize(LOTTERY_GUIDE_XY_MAX);
	m_vecStrName.resize(LOTTERY_GUIDE_XY_MAX);

	ClearSelectTicket();

	StopTurn();

	SetMessageBox( LOTTERY_STATE_NORMAL , "Normal" );

	SPWindowList * pList = static_cast<SPWindowList *>(Find(WIID_LOTTERY_CHAT_LIST, true));
	if ( pList )
		pList->SPSendMessage( SPIM_PURGE );

	pList = static_cast<SPWindowList *>(Find(WIID_LOTTERY_SYSTEMMSG_LIST, true));
	if ( pList )
		pList->SPSendMessage( SPIM_PURGE );

	m_textScrollManager.Clear();
	return 1;
}

void SPWindowLottery::PlayLotteryFx(int iSetID)
{
	LOTTERY_LDT*	pLotteryLdt		=	GetPrizeLDT( iSetID );
	SPPlayer*		pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();

	if( !pLocalPlayer || !pLotteryLdt )
		return;

	m_pFXArchive->Clean();
	m_pFXArchive->InsertFXGroup( (SPID_FX_GROUP)pLotteryLdt->m_iFX1 );
	m_pFXArchive->InsertFXGroup( (SPID_FX_GROUP)pLotteryLdt->m_iFX2 );

	SPSoundArchive* pSoundArchive	=	NULL;
	pLocalPlayer->SPGOBPerformMessage( SPGM_GETSOUNDARCHIVE , (LPARAM)&pSoundArchive );

	if( pSoundArchive != NULL )
	{
		SOUND_TABLE_INFO* pInfo = SPSoundManager::GetInstance()->GetSoundTable( (unsigned int)pLotteryLdt->m_iSoundEffect );

		if( pInfo != NULL )
		{
			int i;
			std::string strFullName;

			for( i = 0; i < 3; i++ )
			{
				if( pInfo->strFileName[i].empty() )
					continue;

				strFullName = "DATA/SOUND/";
				strFullName += pInfo->strFileName[ i ];

				pSoundArchive->InsertItem( strFullName.c_str() , pInfo->fDelayTime[ i ] , pInfo->bLoop[ i ] );
			}
		}
	}

}

int SPWindowLottery::OnMessageOk( WPARAM wParam, LPARAM lParam )
{
	SetMessageBox( LOTTERY_STATE_NORMAL , "Normal" );

	return 1;
}

int SPWindowLottery::OnMessageYes( WPARAM wParam, LPARAM lParam )
{
	if( m_iSelectTicketX == -1 && m_iSelectTicketY == -1 )
	{
		PrintErrorMsg( 4000002 );
		return 1;
	}

	if( g_pCheckManager != NULL )
	{
		if( g_pCheckManager->IsDBBlock() == true )
		{
			PrintErrorMsg( 4000002 );
			return 1;
		}
	}

	if( CheckRequire() == false )
		return 1;

	UINT16 uiTicketPos	=	(UINT16)( m_iSelectTicketX + m_iSelectTicketY * LOTTERY_GUIDE_Y_MAX );

	CONTAINER_ITEM	Item;
	ZeroMemory( &Item , sizeof( CONTAINER_ITEM) );

	if( m_iRequireItem != 0 )
	{
		SPItemAttr* pItem;

		if( g_pItemCluster->GetItemInfo( m_iRequireItem , pItem ) == false )
			return 1;

		if( pItem == NULL )
			return 1;

		SPPlayer*	pLocalPlayer = g_pGOBManager->GetLocalPlayer();

		if( pLocalPlayer == NULL )
			return 1;

		SPPlayerInvenArchive*	pInvenArchive	=	pLocalPlayer->GetInvenArchive();

		if( pInvenArchive == NULL )
			return 1;

		CONTAINER_TYPE	eItemType = (CONTAINER_TYPE)( pItem->m_eType - 1 );
		SPItem* pRequireItem = pInvenArchive->GetSPItem( eItemType , m_iRequireItem );

		if( pRequireItem != NULL )
		{
			pRequireItem->ConvertContainerItem( &Item );
			++Item.SlotInfo.SlotIndex;
		}
		else
		{
			//点券购买
			memset(&Item, 0x00, sizeof(CONTAINER_ITEM));
			Item.SlotInfo.ContainerType = CONTAINER_TYPE_INVALID;
			const char*		pszGlobalString	=	g_pResourceManager->GetGlobalString( 13001432 );
			if( pszGlobalString != NULL )
			{
				AddChatMessage(pszGlobalString);
			}
		}
	}

	int	iRequireEly	=	(int)m_uiRequireEly;

	//////////////////////////////////////////////////////////////////////////

	PrintErrorMsg( 13001204 );

	//////////////////////////////////////////////////////////////////////////

	CPacket	Packet( GAMBLE_CS_BUY );
	Packet.AddUINT64( m_uiVersion );
	Packet.AddUINT32( uiTicketPos );
	Packet.AddData( &(Item.SlotInfo) , sizeof(CONTAINER_SLOT) );
	Packet.AddData( &(Item.Item.iItemNo) , sizeof(ITEMNO) );

	( g_pNetworkManager->GetPacketHandler() )->PostMsg( &Packet );

	if( g_pCheckManager != NULL )
		g_pCheckManager->SetDBBlock( true );

	return 1;
}

int SPWindowLottery::OnMessageNo( WPARAM wParam, LPARAM lParam )
{
	SetMessageBox( LOTTERY_STATE_NORMAL , "Normal" );

	return 1;
}

int SPWindowLottery::OnClose( WPARAM wParam, LPARAM lParam )
{
	if( g_pSubGameManager )
		g_pSubGameManager->SPPerformMessage(SPGM_LOTTREY_IN_REQUEST, 0, 0);

	return 1;
}

int SPWindowLottery::OnRefresh( WPARAM wParam, LPARAM lParam )
{
	if( m_eState == LOTTERY_STATE_WAIT && m_fMessageBoxAccmulateTime < m_fMessageBoxLimit )
		return 1;

	PrintErrorMsg( 13001205 );

	//////////////////////////////////////////////////////////////////////////
	if( g_pCheckManager != NULL )
	{
		if( g_pCheckManager->IsDBBlock() == true )
		{
			PrintErrorMsg( 4000002 );
			return 1;
		}
	}

	m_uiVersion	=	0;
	m_itemVersion = 0;
	m_usServerPoit = 0;
	m_usMyPoit	= 0;

	memset(&m_itemIDFirstGetted, 0x00, sizeof(m_itemIDFirstGetted));
	memset(&m_itemIDSecondGetted, 0x00, sizeof(m_itemIDSecondGetted));
	m_vecTurnItem.clear();


	m_vTicketBuffer.clear();
	m_vRandomBuffer.clear();
	m_vecStrName.clear();
	m_vTicketBuffer.resize(LOTTERY_GUIDE_XY_MAX);
	m_vecStrName.resize(LOTTERY_GUIDE_XY_MAX);

	ClearSelectTicket();

	//
	CPacket	Packet( GAMBLE_CS_GET_STATUS );
	( g_pNetworkManager->GetPacketHandler() )->PostMsg( &Packet );

	if( g_pCheckManager != NULL )
		g_pCheckManager->SetDBBlock(true);

	return 1;
}

int SPWindowLottery::OnBuy( WPARAM wParam, LPARAM lParam )
{
	return OnMessageYes( NULL , NULL );
}

int SPWindowLottery::OnRandom( WPARAM wParam, LPARAM lParam )
{
	int	iRandomCount	=	(int)m_vRandomBuffer.size();

	if( iRandomCount <= 0 )
		return 1;

	int	iAdjustValue		=	10000;
	int iRandomNumber		=	(int)GetRandom( iRandomCount * iAdjustValue - 1 ) / iAdjustValue + 1;

	if( iRandomNumber >= (int)iRandomCount )
		iRandomNumber = (int)iRandomCount - 1;

	int	iTicket	=	m_vRandomBuffer.at( iRandomNumber ).uGambleIndex;

	SetSelectTicket( WIID_LOTTERY_TICKET + ( iTicket % LOTTERY_GUIDE_XY_MAX ) );

	return 1;
}

int SPWindowLottery::OnItemBuy( WPARAM wParam, LPARAM lParam )
{
	return 1;
}

int SPWindowLottery::OnFilling( WPARAM wParam, LPARAM lParam )
{
	if ( g_pInterfaceManager )
		g_pInterfaceManager->SPChildWndSendMessage( WIID_CASHSHOP, SPIM_CASHSHOP_WEB_PAGE, NULL, NULL );

	return 1;
}

int SPWindowLottery::OnContinue( WPARAM wParam, LPARAM lParam )
{
	AcceptItem(1, false);
	return 1;
}

int SPWindowLottery::OnAccept( WPARAM wParam, LPARAM lParam )
{
	if ( m_bAcceptFirst )
		AcceptItem(1);
	else
		AcceptItem(2);

	HideWindowTurn();
	return 1;
}

int	SPWindowLottery::OnStopTurn( WPARAM wParam, LPARAM lParam )
{
	m_bTurnStoping = true;

	SPWindow * pkWindowParent = Find(WIID_LOTTERY_TURN_FRAME, true);
	if ( !pkWindowParent )
		return 1;

	SPWindowButton * pkButton = static_cast<SPWindowButton *>(pkWindowParent->Find( WIID_LOTTERY_TURN_STOP ));
	if ( pkButton )
	{
		pkButton->SetEnable(false);
	}

	return 1;
}

int SPWindowLottery::OnCursorEnter( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	return OnCursorEnter( wParam , lParam );
}

int SPWindowLottery::OnCursorOut( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	return OnCursorOut( wParam , lParam );
}

int SPWindowLottery::OnCursorMove( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	return OnCursorMove( wParam , lParam );
}

int SPWindowLottery::OnLButtonUP( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	SetSelectTicket( iID );

	return 1;
}

int SPWindowLottery::OnLButtonDblClick( unsigned int iID, WPARAM wParam , LPARAM lParam )
{
	SetSelectTicket( iID );

	OnBuy( wParam , lParam );

	return 1;
}

void SPWindowLottery::InitWindowLottery()
{
	SPWindowStatic *	pWindowParent = NULL;
	SPWindowStatic *	pWindowStatic = NULL;

	pWindowParent	=	new SPWindowStatic( WIID_LOTTERY_LOTTERY , 200, 0, 600, 600, this );

	pWindowStatic	=	new SPWindowStatic( WIID_LOTTERY_TOP , 3, 6, 593, 29, pWindowParent );
	pWindowStatic->SetWindowType(WND_TYPE_LEFTBASE);
	pWindowStatic->SetImageNormalHead("DATA/INTERFACE/CONCEPT/UI110.PNG", 197, 1, 335, 30);
	pWindowStatic->SetImageNormalBody("DATA/INTERFACE/CONCEPT/UI110.PNG", 337, 1, 342, 30);
	pWindowStatic->SetImageNormalTail("DATA/INTERFACE/CONCEPT/UI110.PNG", 344, 1, 359, 30);

	pWindowStatic	=	new SPWindowStatic( WIID_LOTTERY_LEFT , 3, 35, 2, 506, pWindowParent );
	pWindowStatic->SetWindowType(WND_TYPE_TOPBASE);
	pWindowStatic->SetImageNormalHead("DATA/INTERFACE/CONCEPT/UI110.PNG", 197, 31, 199, 32);
	pWindowStatic->SetImageNormalBody("DATA/INTERFACE/CONCEPT/UI110.PNG", 197, 32, 199, 34);
	pWindowStatic->SetImageNormalTail("DATA/INTERFACE/CONCEPT/UI110.PNG", 197, 34, 199, 35);

	pWindowStatic	=	new SPWindowStatic( WIID_LOTTERY_RIGHT , 594, 35, 2, 506, pWindowParent );
	pWindowStatic->SetWindowType(WND_TYPE_TOPBASE);
	pWindowStatic->SetImageNormalHead("DATA/INTERFACE/CONCEPT/UI110.PNG", 197, 31, 199, 32);
	pWindowStatic->SetImageNormalBody("DATA/INTERFACE/CONCEPT/UI110.PNG", 197, 32, 199, 34);
	pWindowStatic->SetImageNormalTail("DATA/INTERFACE/CONCEPT/UI110.PNG", 197, 34, 199, 35);

	pWindowStatic	=	new SPWindowStatic( WIID_LOTTERY_BOTTOM , 3, 541, 593, 56, pWindowParent );
	pWindowStatic->SetWindowType(WND_TYPE_LEFTBASE);
	pWindowStatic->SetImageNormalHead("DATA/INTERFACE/CONCEPT/UI110.PNG", 197, 36, 211, 92);
	pWindowStatic->SetImageNormalBody("DATA/INTERFACE/CONCEPT/UI110.PNG", 213, 36, 215, 92);
	pWindowStatic->SetImageNormalTail("DATA/INTERFACE/CONCEPT/UI110.PNG", 217, 36, 231, 92);

	//
	InitWindowButtons();
	InitWindowTickets();
	InitWindowEtc();

}


void SPWindowLottery::InitWindowSystemMsg()
{
	SPWindowStatic* pkParent, *pkChild;
	SPWindowButton* pkButton;
	SPWindowList*	pkList;

	pkParent = new SPWindowStatic(WIID_LOTTERY_SYSTEMMSG, 4, 5, 195, 281, this);
	pkParent->SetWindowType(WND_TYPE_TOPBASE);
	pkParent->SetImageNormalHead("DATA/INTERFACE/CONCEPT/UI110.PNG", 1, 1,  196, 23);
	pkParent->SetImageNormalBody("DATA/INTERFACE/CONCEPT/UI110.PNG", 1, 25, 196, 28);
	pkParent->SetImageNormalTail("DATA/INTERFACE/CONCEPT/UI110.PNG", 1, 30, 196, 36);

	//pkChild = new SPWindowStatic(WIID_LOTTERY_SYSTEMMSG_BG, 2, 20, 184, 248, pkParent);
	//pkChild->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 465, 107);
	//pkChild->SetSrcSize(2, 2);

	pkList = new SPWindowList( WIID_LOTTERY_SYSTEMMSG_LIST , 6 , 24 , 168 , 251 , pkParent );
	pkList->SetFontShadow(true);
	pkList->SetFontColor( D3DXCOLOR( 1.0f,  1.0f , 1.0f , 1.0f ) );
	pkList->SetMargin( 0 , 0 , 0 , 0 );
	pkList->SetMultiLine( TRUE , 2 );
	pkList->SetWheelUse( true );

	pkButton = new SPWindowButton(WIID_LOTTERY_SYSTEMMSG_UP, 176, 214, 12, 11, pkParent);
	pkButton->SetImage		("DATA/INTERFACE/CONCEPT/UI100.PNG", 460 , 38 );
	pkButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI100.PNG", 473 , 38 );
	pkButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI100.PNG", 486 , 38 );
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI100.PNG", 499 , 38 );

	pkButton = new SPWindowButton(WIID_LOTTERY_SYSTEMMSG_DOWN, 176, 227, 12, 11, pkParent);
	pkButton->SetImage		("DATA/INTERFACE/CONCEPT/UI100.PNG", 460 , 51 );
	pkButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI100.PNG", 473 , 51 );
	pkButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI100.PNG", 486 , 51 );
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI100.PNG", 499 , 51 );

	pkButton = new SPWindowButton(WIID_LOTTERY_SYSTEMMSG_BOTTOM, 176, 240, 12, 11, pkParent);
	pkButton->SetImage			("DATA/INTERFACE/CONCEPT/UI100.PNG", 460 , 64 );
	pkButton->SetImageHit		("DATA/INTERFACE/CONCEPT/UI100.PNG", 473 , 64 );
	pkButton->SetImagePush		("DATA/INTERFACE/CONCEPT/UI100.PNG", 486 , 64 );
	pkButton->SetImageDisable	("DATA/INTERFACE/CONCEPT/UI100.PNG", 499 , 64 );
	
}

void SPWindowLottery::InitWindowItemInven()
{
	SPWindowStatic* pkParent, *pkChild[2];
	SPWindowButton* pkButton;
	SPWindowGrid* pkGrid;
	SPWindowSlider* pkSlider;

	pkParent = new SPWindowStatic(WIID_LOTTERY_ITEM_INVEN, 4, 288, 195, 215, this);
	pkParent->SetWindowType(WND_TYPE_TOPBASE);
	pkParent->SetImageNormalHead("DATA/INTERFACE/CONCEPT/UI110.PNG", 1, 37, 196, 90);
	pkParent->SetImageNormalBody("DATA/INTERFACE/CONCEPT/UI110.PNG", 1, 92, 196, 97);
	pkParent->SetImageNormalTail("DATA/INTERFACE/CONCEPT/UI110.PNG", 1, 99, 196, 104);
	
	pkButton = new SPWindowButton(WIID_LOTTERY_ITEM_INVEN_EQUIP, 6, 11, 59, 19, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI110.PNG", 2, 120);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI110.PNG", 2, 140);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI110.PNG", 2, 160);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/UI110.PNG", 2, 160);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI110.PNG", 2, 180);

	pkButton = new SPWindowButton(WIID_LOTTERY_ITEM_INVEN_CONSUME, 64, 11, 59, 19, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI110.PNG", 62, 120);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI110.PNG", 62, 140);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI110.PNG", 62, 160);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/UI110.PNG", 62, 160);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI110.PNG", 62, 180);

	pkButton = new SPWindowButton(WIID_LOTTERY_ITEM_INVEN_ETC, 122, 11, 59, 19, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI110.PNG", 122, 120);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI110.PNG", 122, 140);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI110.PNG", 122, 160);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/UI110.PNG", 122, 160);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI110.PNG", 122, 180);

	pkButton = new SPWindowButton(WIID_LOTTERY_ITEM_INVEN_EVENT, 6, 32, 59, 19, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI110.PNG", 182, 120);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI110.PNG", 182, 140);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI110.PNG", 182, 160);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/UI110.PNG", 182, 160);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI110.PNG", 182, 180);

	//pkButton = new SPWindowButton(WIID_LOTTERY_ITEM_INVEN_HOUSING, 64, 32, 59, 16, pkParent);
	//pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI110.PNG", 242, 120);
	//pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI110.PNG", 242, 140);
	//pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI110.PNG", 242, 160);
	//pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/UI110.PNG", 242, 160);
	//pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI110.PNG", 242, 180);

	pkButton = new SPWindowButton(WIID_LOTTERY_ITEM_INVEN_PET, 64/*122*/, 32, 59, 19, pkParent);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI110.PNG", 302, 120);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI110.PNG", 302, 140);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI110.PNG", 302, 160);
	pkButton->SetImageCheck("DATA/INTERFACE/CONCEPT/UI110.PNG", 302, 160);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI110.PNG", 302, 180);

	pkChild[0] = new SPWindowStatic(WIID_LOTTERY_ITEM_INVEN_WHITE_BG, 6, 53, 183, 156, pkParent);
	pkChild[0]->SetImage("DATA/INTERFACE/CONCEPT/CASHSHOP02.PNG", 465, 107);
	pkChild[0]->SetSrcSize(2, 2);

	pkGrid = new SPWindowGrid(WNDID_CTRL_GRID, WIID_LOTTERY_ITEM_INVEN_SLOT,
		10, 8, 140, 140, pkChild[0], 4, 4, 36, 36, 32);
	pkGrid->SetGridImage("DATA/INTERFACE/CONCEPT/UI110.PNG", 242, 31);
	pkGrid->ClearCellAll();

	pkButton = new SPWindowButton(WIID_LOTTERY_ITEM_INVEN_UP, 165, 8, 12, 11, pkChild[0]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI110.PNG", 484, 200);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI110.PNG", 497, 200);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI110.PNG", 484, 254);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI110.PNG", 497, 254);

	pkButton = new SPWindowButton(WIID_LOTTERY_ITEM_INVEN_DOWN, 165, 137, 12, 11, pkChild[0]);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI110.PNG", 484, 242);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI110.PNG", 497, 242);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI110.PNG", 484, 296);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI110.PNG", 197, 296);

	pkSlider = new SPWindowSlider(WIID_LOTTERY_ITEM_INVEN_THUMB, 165, 19, 12, 118, pkChild[0]);
	RECT rtRect;
	SetRect(&rtRect, 484, 212, 496, 236);
	pkSlider->SetImageHandle("DATA/INTERFACE/CONCEPT/UI110.PNG", rtRect);
	SetRect(&rtRect, 497, 212, 509, 236);
	pkSlider->SetImageHandleHit("DATA/INTERFACE/CONCEPT/UI110.PNG", rtRect);
	SetRect(&rtRect, 484, 266, 496, 290);
	pkSlider->SetImageHandlePush("DATA/INTERFACE/CONCEPT/UI110.PNG", rtRect);
	SetRect(&rtRect, 497, 266, 509, 290);
	pkSlider->SetImageHandleDisable("DATA/INTERFACE/CONCEPT/UI110.PNG", rtRect);

	SetRect(&rtRect, 484, 238, 496, 240);
	pkSlider->SetImagePageNormal("DATA/INTERFACE/CONCEPT/UI110.PNG", rtRect);
	SetRect(&rtRect, 497, 238, 509, 240);
	pkSlider->SetImagePageHit("DATA/INTERFACE/CONCEPT/UI110.PNG", rtRect);
	SetRect(&rtRect, 484, 292, 496, 294);
	pkSlider->SetImagePageShadow("DATA/INTERFACE/CONCEPT/UI110.PNG", rtRect);
	SetRect(&rtRect, 497, 292, 509, 294);
	pkSlider->SetImagePageDisable("DATA/INTERFACE/CONCEPT/UI110.PNG", rtRect);

}

void SPWindowLottery::InitWindowChat()
{
	SPWindowStatic* pkParent;
	SPWindowButton* pkButton;
	SPWindowList*	pkList;

	pkParent = new SPWindowStatic(WIID_LOTTERY_CHAT, 6, 507, 192, 89, this);
	pkParent->SetWindowType(WND_TYPE_TOPBASE);
	pkParent->SetImageNormalHead("DATA/INTERFACE/CONCEPT/UI110.PNG", 2, 105, 194, 108);
	pkParent->SetImageNormalBody("DATA/INTERFACE/CONCEPT/UI110.PNG", 2, 110, 194, 113);
	pkParent->SetImageNormalTail("DATA/INTERFACE/CONCEPT/UI110.PNG", 2, 115, 194, 118);

	pkList = new SPWindowList( WIID_LOTTERY_CHAT_LIST , 4 , 2 , 170 , 85 , pkParent );
	pkList->SetFontShadow(true);
	pkList->SetFontColor( D3DXCOLOR( 1.0f,  1.0f , 1.0f , 1.0f ) );
	pkList->SetMargin( 0 , 0 , 0 , 0 );
	pkList->SetMultiLine( TRUE , 2 );
	pkList->SetWheelUse( true );

	pkButton = new SPWindowButton(WIID_LOTTERY_CHAT_UP, 176, 44, 12, 11, pkParent);
	pkButton->SetImage		("DATA/INTERFACE/CONCEPT/UI100.PNG", 460 , 38 );
	pkButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI100.PNG", 473 , 38 );
	pkButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI100.PNG", 486 , 38 );
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI100.PNG", 499 , 38 );

	pkButton = new SPWindowButton(WIID_LOTTERY_CHAT_DOWN, 176, 57, 12, 11, pkParent);
	pkButton->SetImage		("DATA/INTERFACE/CONCEPT/UI100.PNG", 460 , 51 );
	pkButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI100.PNG", 473 , 51 );
	pkButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI100.PNG", 486 , 51 );
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI100.PNG", 499 , 51 );

	pkButton = new SPWindowButton(WIID_LOTTERY_CHAT_BOTTOM, 176, 70, 12, 11, pkParent);
	pkButton->SetImage			("DATA/INTERFACE/CONCEPT/UI100.PNG", 460 , 64 );
	pkButton->SetImageHit		("DATA/INTERFACE/CONCEPT/UI100.PNG", 473 , 64 );
	pkButton->SetImagePush		("DATA/INTERFACE/CONCEPT/UI100.PNG", 486 , 64 );
	pkButton->SetImageDisable	("DATA/INTERFACE/CONCEPT/UI100.PNG", 499 , 64 );
}

void SPWindowLottery::InitWindowMessageBox()
{
	SPWindowButton*	pWindowButton;
	SPWindowStatic*	pWindowStatic;

	m_pMessageBox	=	new SPWindowStatic( WIID_LOTTERY_MESSAGE_BOX_BG , 0 , 0 , 800 , 600 , this );
	//m_pMessageBox->SetImage( "DATA/INTERFACE/CONCEPT/UI110.PNG" , 279, 32 );
	//m_pMessageBox->SetSrcSize( 30 , 30 );
//	m_pMessageBox->SetImage( "DATA/INTERFACE/CONCEPT/UI106.PNG" , 474 , 439 );
//	m_pMessageBox->SetSrcSize( 10 , 10 );

	int		iXPos	=	181 - 2;
	int		iYPos	=	159 - 27;

	pWindowStatic	=	new SPWindowStatic( WIID_LOTTERY_MESSAGE_BOX_TEXT , iXPos + 10 , iYPos + 68 , 209 , 47 , m_pMessageBox );
	pWindowStatic->SetFont( FONT_12_BOLD );
	pWindowStatic->SetFontColor( RGBA( 195 , 58 , 58 , 255 ) );
	pWindowStatic->SetMultiLine( true , 2 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_LOTTERY_MESSAGE_BOX_OK_TEXT , iXPos + 10 , iYPos + 29 , 209 , 82 , m_pMessageBox );
	pWindowStatic->SetFont( FONT_12_BOLD );
	pWindowStatic->SetFontColor( RGBA( 49 , 76 , 97 , 255 ) );
	pWindowStatic->SetMultiLine( true , 2 );
	pWindowStatic	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_LOTTERY_MESSAGE_BOX_OK , iXPos + 82 , iYPos + 119 , 68 , 17 , m_pMessageBox );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 75 , 468 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 144 , 468 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 213 , 468 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 213 , 486 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_LOTTERY_MESSAGE_BOX_YES , iXPos + 45 , iYPos + 119 , 68 , 17 , m_pMessageBox );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 75 , 468 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 144 , 468 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 213 , 468 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI100.PNG" , 213 , 468 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_LOTTERY_MESSAGE_BOX_NO , iXPos + 120 , iYPos + 119 , 68 , 17 , m_pMessageBox );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI104.PNG" , 197 , 218 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI104.PNG" , 197 , 236 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI104.PNG" , 216 , 291 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI104.PNG" , 216 , 309 );
	pWindowButton	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_LOTTERY_MESSAGE_BOX_TITLE , iXPos + 10 , iYPos + 10 , 104 , 11 , m_pMessageBox );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI106.PNG" , 1 , 398 );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI106.PNG" , 1 , 386 );
	pWindowStatic	=	NULL;

	//////////////////////////////////////////////////////////////////////////

	int	x , y;
	int	iXSrcEnable , iYSrcEnable;

	m_iMessageBoxTicketYX			=	31;
	m_iMessageBoxTicketYY			=	40;
	m_iMessageBoxTicketXX			=	59;
	m_iMessageBoxTicketXY			=	40;

	m_iMessageBoxBuyHelpX			=	85;
	m_iMessageBoxBuyHelpY			=	39;
	int iMessageBoxBuyHelpXSize		=	130;
	int iMessageBoxBuyHelpYSize		=	14;

	int	iMessageBoxBuyHelpXImage	=	106;
	int	iMessageBoxBuyHelpYImage	=	391;

	std::string	strMessageBoxBuyImage	=	"DATA/INTERFACE/CONCEPT/UI106.PNG";

	m_iMessageBoxBuyBGX				=	18;
	m_iMessageBoxBuyBGY				=	39;

	if( SPLocalizeManager::GetInstance()->GetSiteType() == SITE_SANDA )
	{
		m_iMessageBoxTicketYX		=	82;
		m_iMessageBoxTicketYY		=	41;
		m_iMessageBoxTicketXX		=	110;
		m_iMessageBoxTicketXY		=	41;

		m_iMessageBoxBuyHelpX		=	42;
		m_iMessageBoxBuyHelpY		=	40;
		iMessageBoxBuyHelpXSize		=	145;
		iMessageBoxBuyHelpYSize		=	14;
		iMessageBoxBuyHelpXImage	=	266;
		iMessageBoxBuyHelpYImage	=	497;

		strMessageBoxBuyImage	=	"DATA/INTERFACE/CONCEPT/UI107.PNG";

		m_iMessageBoxBuyBGX			=	69;
		m_iMessageBoxBuyBGY			=	41;
	}

	pWindowStatic	=	new SPWindowStatic( WIID_LOTTERY_MESSAGE_BOX_TICKET_Y , iXPos + m_iMessageBoxTicketYX , iYPos + m_iMessageBoxTicketYY , 14 , 12 , m_pMessageBox );

	for( y = 0 ; y < LOTTERY_GUIDE_Y_MAX ; ++y )
	{
		iXSrcEnable		=	1 + ( y * 15 );
		iYSrcEnable		=	27;

		pWindowStatic->SetImage		( "DATA/INTERFACE/CONCEPT/UI106.PNG" , iXSrcEnable , iYSrcEnable );
	}

	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_LOTTERY_MESSAGE_BOX_TICKET_X , iXPos + m_iMessageBoxTicketXX , iYPos + m_iMessageBoxTicketXY , 14 , 12 , m_pMessageBox );

	for( x = 0 ; x < LOTTERY_GUIDE_X_MAX ; ++x )
	{
		iXSrcEnable		=	1 + ( x * 15 );
		iYSrcEnable		=	1;

		pWindowStatic->SetImage		( "DATA/INTERFACE/CONCEPT/UI106.PNG" , iXSrcEnable , iYSrcEnable );
	}

	pWindowStatic	=	NULL;

	//////////////////////////////////////////////////////////////////////////

	pWindowStatic	=	new SPWindowStatic( WIID_LOTTERY_MESSAGE_BOX_BUY_HELP , iXPos + m_iMessageBoxBuyHelpX , iYPos + m_iMessageBoxBuyHelpY , iMessageBoxBuyHelpXSize , iMessageBoxBuyHelpYSize , m_pMessageBox );
	pWindowStatic->SetImage( strMessageBoxBuyImage.c_str() , iMessageBoxBuyHelpXImage , iMessageBoxBuyHelpYImage );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_LOTTERY_MESSAGE_BOX_BUY_BG , iXPos + m_iMessageBoxBuyBGX , iYPos + m_iMessageBoxBuyBGY , 67 , 14 , m_pMessageBox );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI106.PNG" , 405 , 425 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_LOTTERY_MESSAGE_BOX_BUY_BG_L , iXPos + 10 , iYPos + 29 , 2 , 35 , m_pMessageBox );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI106.PNG" , 503 , 1 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_LOTTERY_MESSAGE_BOX_BUY_BG_C , iXPos + 12 , iYPos + 29 , 205 , 35 , m_pMessageBox );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI106.PNG" , 506 , 1 );
	pWindowStatic->SetSrcSize( 2 , 35 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_LOTTERY_MESSAGE_BOX_BUY_BG_R , iXPos + 217 , iYPos + 29 , 2 , 35 , m_pMessageBox );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI106.PNG" , 509 , 1 );
	pWindowStatic	=	NULL;

	//////////////////////////////////////////////////////////////////////////

	pWindowStatic	=	new SPWindowStatic( WIID_LOTTERY_MESSAGE_BOX_BG_L_T , iXPos + 0 , iYPos + 0 , 4 , 4 , m_pMessageBox );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 482 , 301 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_LOTTERY_MESSAGE_BOX_BG_C_T , iXPos + 4 , iYPos + 0 , 221 , 4 , m_pMessageBox );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 488 , 301 );
	pWindowStatic->SetSrcSize( 2 , 4 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_LOTTERY_MESSAGE_BOX_BG_R_T , iXPos + 225 , iYPos + 0 , 4 , 4 , m_pMessageBox );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 492 , 301 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_LOTTERY_MESSAGE_BOX_BG_L_M , iXPos + 0 , iYPos + 4 , 4 , 138 , m_pMessageBox );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 482 , 307 );
	pWindowStatic->SetSrcSize( 4 , 2 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_LOTTERY_MESSAGE_BOX_BG_C_M , iXPos + 4 , iYPos + 4 , 221 , 138 , m_pMessageBox );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 488 , 307 );
	pWindowStatic->SetSrcSize( 2 , 2 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_LOTTERY_MESSAGE_BOX_BG_R_M , iXPos + 225 , iYPos + 4 , 4 , 138 , m_pMessageBox );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 492 , 307 );
	pWindowStatic->SetSrcSize( 4 , 2 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_LOTTERY_MESSAGE_BOX_BG_L_B , iXPos + 0 , iYPos + 142 , 4 , 4 , m_pMessageBox );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 482 , 311 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_LOTTERY_MESSAGE_BOX_BG_C_B , iXPos + 4 , iYPos + 142 , 221 , 4 , m_pMessageBox );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 488 , 311 );
	pWindowStatic->SetSrcSize( 2 , 4 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_LOTTERY_MESSAGE_BOX_BG_R_B , iXPos + 225 , iYPos + 142 , 4 , 4 , m_pMessageBox );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 492 , 311 );
	pWindowStatic	=	NULL;

}

void SPWindowLottery::InitWindowButtons()
{
	SPWindowButton*	pWindowButton;
	SPWindowStatic*	pWindowStatic;

	SPWindow *	pWindowParent = Find(WIID_LOTTERY_BOTTOM, true);
	if ( !pWindowParent )
		return;

	pWindowButton	=	new	SPWindowButton( WIID_LOTTERY_REFRESH , 3, 2, 76, 50, pWindowParent );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI110.PNG" , 1 , 308 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI110.PNG" , 1 , 359 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI110.PNG" , 1 , 410 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI110.PNG" , 1 , 461 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_LOTTERY_BUY_ITEM , 82, 2, 76, 50, pWindowParent );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI110.PNG" , 78 , 308 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI110.PNG" , 78 , 359 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI110.PNG" , 78 , 410 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI110.PNG" , 78 , 461 );
	pWindowButton->SetEnable(false);
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_LOTTERY_BUY_TICKET , 201, 14, 89, 25 , pWindowParent );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI110.PNG" , 1 , 202 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI110.PNG" , 1 , 228 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI110.PNG" , 1 , 254 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI110.PNG" , 1 , 280 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_LOTTERY_RANDOM_TICKET , 290, 14, 89, 25 , pWindowParent );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI110.PNG" , 90 , 202 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI110.PNG" , 90 , 228 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI110.PNG" , 90 , 254 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI110.PNG" , 90 , 280 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_LOTTERY_FILLING , 432, 2, 76, 50, pWindowParent );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI110.PNG" , 155 , 308 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI110.PNG" , 155 , 359 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI110.PNG" , 155 , 410 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI110.PNG" , 155 , 461 );
	pWindowButton	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_LOTTERY_CLOSE_ICON , 512, 2, 76, 50 , pWindowParent );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI110.PNG" , 232 , 308 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI110.PNG" , 232 , 359 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI110.PNG" , 232 , 410 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI110.PNG" , 232 , 461 );
	pWindowButton	=	NULL;

	pWindowStatic	=	new	SPWindowStatic( WIID_LOTTERY_SICE_ICON , 276, 10, 27, 32 , pWindowParent );
	pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI110.PNG" , 312 , 32 );
	pWindowStatic	=	NULL;

}

void SPWindowLottery::InitWindowTickets()
{
	SPWindowButton*	pWindowButton;
	SPWindowStatic*	pWindowStatic;

	SPWindow *	pWindowParent = Find(WIID_LOTTERY_LOTTERY, true);
	if ( !pWindowParent )
		return;

	int	x , y;
	int	iXPos , iYPos;
	int	iXSrcEnable , iYSrcEnable;
	int	iXSrcDisable , iYSrcDisable;
	int	iInstanceID;

	for( y = 0 ; y < LOTTERY_GUIDE_Y_MAX ; ++y )
	{
		iYPos			=	74 + ( y * ( 19 + 2 ) ) + ( 13 * ( y / 5 ) );

		for( x = 0 ; x < LOTTERY_GUIDE_X_MAX ; ++x )
		{
			iXPos		=	29 + ( x * ( 20 + 4 ) ) + ( 21 * ( x / 5 ) );
			iInstanceID	=	x + y * LOTTERY_GUIDE_Y_MAX;

			pWindowButton	=	new	SPWindowButton( WIID_LOTTERY_TICKET_DISABLE + iInstanceID , iXPos , iYPos , 20 , 19 , pWindowParent );
			pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI110.PNG" , 297 , 77 );
			pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI110.PNG" , 297 , 77 );
			pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI110.PNG" , 297 , 77 );
			pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI110.PNG" , 297 , 77 );
			m_pTicketDisable[ iInstanceID ]	=	pWindowButton;
			pWindowButton	=	NULL;

			pWindowButton	=	new	SPWindowButton( WIID_LOTTERY_TICKET_SELL + iInstanceID , iXPos , iYPos , 20 , 19 , pWindowParent );
			pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI110.PNG" , 234 , 97 );
			pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI110.PNG" , 234 , 97 );
			pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI110.PNG" , 234 , 97 );
			pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI110.PNG" , 234 , 97 );
			m_pTicketSell[ iInstanceID ]	=	pWindowButton;
			pWindowButton	=	NULL;

			pWindowButton	=	new	SPWindowButton( WIID_LOTTERY_TICKET + iInstanceID , iXPos , iYPos , 20 , 19 , pWindowParent );
			pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI110.PNG" , 234 , 77 );
			pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI110.PNG" , 255 , 77 );
			pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI110.PNG" , 276 , 77 );
			pWindowButton->SetImageCheck	( "DATA/INTERFACE/CONCEPT/UI110.PNG" , 276 , 77 );
			m_pTicket[ iInstanceID ]	=	pWindowButton;
			pWindowButton	=	NULL;
		}
	}

}

void SPWindowLottery::InitWindowTurn()
{
	SPWindowStatic* pWindowParent;
	SPWindowStatic*	pWindowStatic, * pWindow[2];
	SPWindowButton* pButton;

	pWindow[0]	=	new SPWindowStatic( WIID_LOTTERY_TURN , 200 , 0 , 600 , 600 , this );
	pWindow[0]->SetImage( "DATA/INTERFACE/CONCEPT/UI110.PNG" , 279, 32 );
	pWindow[0]->SetSrcSize( 30 , 30 );

	pWindow[1]	=	new SPWindowStatic( WIID_LOTTERY_TURN_BACK , 128 , 79 , 301 , 388 , pWindow[0] );
	pWindow[1]->SetWindowType(WND_TYPE_TOPBASE);
	pWindow[1]->SetImageNormalHead( "DATA/INTERFACE/CONCEPT/UI110.PNG", 182, 200, 483, 287 );
	pWindow[1]->SetImageNormalBody( "DATA/INTERFACE/CONCEPT/UI110.PNG", 182, 289, 483, 293 );
	pWindow[1]->SetImageNormalTail( "DATA/INTERFACE/CONCEPT/UI110.PNG", 182, 295, 483, 299 );

	pWindowParent	=	new SPWindowStatic( WIID_LOTTERY_TURN_FRAME , 0 , 0 , 301 , 388 , pWindow[1] );

	pButton	=	new SPWindowButton( WIID_LOTTERY_TURN_ACCEPT , 34, 339, 89, 25 , pWindowParent );
	pButton->SetImage("DATA/INTERFACE/CONCEPT/UI110.PNG", 309, 360 );
	pButton->SetImageHit( "DATA/INTERFACE/CONCEPT/UI110.PNG", 309, 386 );
	pButton->SetImagePush( "DATA/INTERFACE/CONCEPT/UI110.PNG", 399, 360 );
	pButton->SetImageDisable( "DATA/INTERFACE/CONCEPT/UI110.PNG", 399, 386 );

	pButton	=	new SPWindowButton( WIID_LOTTERY_TURN_CONTINUE , 176, 339, 89, 25 , pWindowParent );
	pButton->SetImage("DATA/INTERFACE/CONCEPT/UI110.PNG", 309, 308 );
	pButton->SetImageHit( "DATA/INTERFACE/CONCEPT/UI110.PNG", 309, 334 );
	pButton->SetImagePush( "DATA/INTERFACE/CONCEPT/UI110.PNG", 399, 308 );
	pButton->SetImageDisable( "DATA/INTERFACE/CONCEPT/UI110.PNG", 399, 334 );

	pButton	=	new SPWindowButton( WIID_LOTTERY_TURN_STOP , 176, 339, 89, 25 , pWindowParent );
	pButton->SetImage("DATA/INTERFACE/CONCEPT/UI110.PNG", 309, 412 );
	pButton->SetImageHit( "DATA/INTERFACE/CONCEPT/UI110.PNG", 309, 438 );
	pButton->SetImagePush( "DATA/INTERFACE/CONCEPT/UI110.PNG", 399, 412 );
	pButton->SetImageDisable( "DATA/INTERFACE/CONCEPT/UI110.PNG", 399, 438 );

	for ( int i=0; i<9; i++ )
	{
		int iPosX = 32  + ( (i%3) * (62 + 27) );
		int iPosY = 100 + ( (i/3) * (62 + 14) );

		//
		pWindowStatic	=	new SPWindowStatic( WIID_LOTTERY_TURN_ITEM_BG + i , iPosX , iPosY , 62 , 62 , pWindowParent );
		if ( i == 0 )
			pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI111.PNG", 1, 1 );
		else if ( i == 1 )
			pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI111.PNG", 64, 1 );
		else if ( i == 2 )
			pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI111.PNG", 127, 1 );
		else if ( i == 3 )
			pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI111.PNG", 1, 64 );
		else if ( i == 4 )
			pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI111.PNG", 64, 64 );
		else if ( i == 5 )
			pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI111.PNG", 127, 64 );
		else if ( i == 6 )
			pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI111.PNG", 1, 127 );
		else if ( i == 7 )
			pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI111.PNG", 64, 127 );
		else if ( i == 8 )
			pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI111.PNG", 127, 127 );

		//
		pWindowStatic	=	new SPWindowStatic( WIID_LOTTERY_TURN_LIGHT + i , iPosX-16 , iPosY-13 , 93 , 86 , pWindowParent );
		if ( i == 4 )
			pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI111.PNG", 190, 88 );
		else
			pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI111.PNG", 190, 1 );

		//
		pWindowStatic	=	new SPWindowStatic( WIID_LOTTERY_TURN_ITEM + i , iPosX+13 , iPosY+13 , 32 , 32 , pWindowParent );

		//
		pWindowStatic	=	new SPWindowStatic( WIID_LOTTERY_TURN_LEVEL + i , iPosX+3 , iPosY+3 , 16 , 14 , pWindowParent );

		//
		pWindowStatic	=	new SPWindowStatic( WIID_LOTTERY_TURN_NUM + i , iPosX+40 , iPosY+42 , 12 , 12 , pWindowParent );
		pWindowStatic->SetFormat(DT_RIGHT | DT_VCENTER);
		pWindowStatic->SetFont(FONT_12_BOLD);
		pWindowStatic->SetFontColor(0xFFFFFFFF);
		pWindowStatic->SetFontShadow(true);

		//
		pWindowStatic	=	new SPWindowStatic( WIID_LOTTERY_TURN_DISABLE + i , iPosX , iPosY , 62 , 62 , pWindowParent );
		pWindowStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI111.PNG", 284, 103 );
	}

	//Tip
	pWindow[1]	=	new SPWindowStatic( WIID_LOTTERY_TURN_TIP , 437, 46, 152, 251 , pWindow[0] );
	pWindow[1]->SetWindowType(WND_TYPE_TOPBASE);
	pWindow[1]->SetImageNormalHead( "DATA/INTERFACE/CONCEPT/UI111.PNG", 0,  190, 152, 218 );
	pWindow[1]->SetImageNormalBody( "DATA/INTERFACE/CONCEPT/UI111.PNG", 0,  220, 152, 221 );
	pWindow[1]->SetImageNormalTail( "DATA/INTERFACE/CONCEPT/UI111.PNG", 0,  223, 152, 253 );

	const char*		pszGlobalString1	=	g_pResourceManager->GetGlobalString( 13001435 );
	const char*		pszGlobalString2	=	g_pResourceManager->GetGlobalString( 13001436 );
	//const char*		pszGlobalString3	=	g_pResourceManager->GetGlobalString( 13001437 );
	//const char*		pszGlobalString4	=	g_pResourceManager->GetGlobalString( 13001438 );

	pWindowStatic	=	new SPWindowStatic( WIID_LOTTERY_TURN_TIP_STATIC1, 10 , 42 , 135 , 48 , pWindow[1] );
	pWindowStatic->SetFormat(DT_LEFT | DT_VCENTER);
	pWindowStatic->SetMultiLine(true, 4 );
	pWindowStatic->SetFont(FONT_12_NORMAL);
	pWindowStatic->SetFontColor(0xFF000000);
	pWindowStatic->SetWindowText(pszGlobalString1);

	pWindowStatic	=	new SPWindowStatic( WIID_LOTTERY_TURN_TIP_STATIC2, 10 , 130 , 135 , 48 , pWindow[1] );
	pWindowStatic->SetFormat(DT_LEFT | DT_VCENTER);
	pWindowStatic->SetMultiLine(true, 4 );
	pWindowStatic->SetFont(FONT_12_NORMAL);
	pWindowStatic->SetFontColor(0xFF000000);
	pWindowStatic->SetWindowText(pszGlobalString2);

	//pWindowStatic	=	new SPWindowStatic( WIID_LOTTERY_TURN_TIP_STATIC3, 13 , 145 , 162 , 26 , pWindow[1] );
	//pWindowStatic->SetFormat(DT_LEFT | DT_VCENTER);
	//pWindowStatic->SetMultiLine(true, 3 );
	//pWindowStatic->SetFont(FONT_12_NORMAL);
	//pWindowStatic->SetFontColor(0xFF000000);
	//pWindowStatic->SetWindowText(pszGlobalString3);

	//pWindowStatic	=	new SPWindowStatic( WIID_LOTTERY_TURN_TIP_STATIC4, 48 , 192 , 90 , 12 , pWindow[1] );
	//pWindowStatic->SetFormat(DT_LEFT | DT_VCENTER);
	//pWindowStatic->SetFont(FONT_12_NORMAL);
	//pWindowStatic->SetFontColor(0xFF000000);
	//pWindowStatic->SetWindowText(pszGlobalString4);

	//pButton	=	new SPWindowButton( WIID_LOTTERY_TURN_TIP_CHECK , 33, 192, 12, 12 , pWindow[1] );
	//pButton->SetImage("DATA/INTERFACE/CONCEPT/UI111.PNG", 191, 176 );
	////pButton->SetImageHit( "DATA/INTERFACE/CONCEPT/UI111.PNG", 204, 176 );
	//pButton->SetImagePush( "DATA/INTERFACE/CONCEPT/UI111.PNG", 204, 176 );
	//pButton->SetImageCheck( "DATA/INTERFACE/CONCEPT/UI111.PNG", 217, 176 );
	//pButton->SetImageDisable( "DATA/INTERFACE/CONCEPT/UI111.PNG", 230, 176 );

	//pButton	=	new SPWindowButton( WIID_LOTTERY_TURN_TIP_CLOSE , 164, 11, 12, 12 , pWindow[1] );
	//pButton->SetImage("DATA/INTERFACE/CONCEPT/UI103.PNG", 301, 321);
	//pButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI103.PNG", 314, 321);
	//pButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI103.PNG", 327, 321);
	//pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI103.PNG", 340, 321);

}

void SPWindowLottery::InitWindowEtc()
{
	SPWindowStatic*	pWindowStatic;

	SPWindow *	pWindowParent = Find(WIID_LOTTERY_LOTTERY, true);
	if ( !pWindowParent )
		return;

}

void SPWindowLottery::InitLDTLoad()
{
	SPLDTFile*	pLDTFile	=	NULL;

	g_pResourceManager->GetLDTFromFileName( "DATA/LDT/prize_list.ldt" , &pLDTFile );

	if( pLDTFile == NULL )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "-----------------------------------> LDT Load Fail [ DATA/LDT/prize_list.LDT ] \n" );
#endif
		return;
	}

	int			iRecordCount	= pLDTFile->GetItemCount();
	int			iItemID;
	LOTTERY_LDT	stLottery;
	LDT_Field	ldtField;

	for( int iRecord = 0 ; iRecord < iRecordCount ; iRecord++ )
	{
		iItemID = pLDTFile->GetPrimaryKey( iRecord );
		stLottery.m_iKey = iItemID;

		pLDTFile->GetField( iItemID, 0,		ldtField );
		stLottery.m_iLevel = ldtField.uData.lValue;

		pLDTFile->GetField( iItemID, 1,		ldtField );
		stLottery.m_iItemID = ldtField.uData.lValue;

		pLDTFile->GetField( iItemID, 2,		ldtField );
		if( ldtField.uData.pValue != NULL )
			stLottery.m_strName	=	ldtField.uData.pValue;

		pLDTFile->GetField( iItemID, 3,		ldtField );
		stLottery.m_iIconFile = ldtField.uData.lValue;

		pLDTFile->GetField( iItemID, 4,		ldtField );
		stLottery.m_iIconIndex = ldtField.uData.lValue;

		pLDTFile->GetField( iItemID, 5,		ldtField );
		stLottery.m_eRareLimit = (ITEM_RARE_LIMIT)ldtField.uData.lValue;

		pLDTFile->GetField( iItemID, 6,		ldtField );
		stLottery.m_iOpt_Min = ldtField.uData.lValue;

		pLDTFile->GetField( iItemID, 7,		ldtField );
		stLottery.m_iOpt_Max = ldtField.uData.lValue;

		pLDTFile->GetField( iItemID, 8,	ldtField );
		stLottery.m_iItemCount = ldtField.uData.lValue;

		pLDTFile->GetField( iItemID, 9,	ldtField );
		stLottery.m_iOpenIconFile = ldtField.uData.lValue;

		pLDTFile->GetField( iItemID, 10,	ldtField );
		stLottery.m_iOpenIconIndex = ldtField.uData.lValue;

		pLDTFile->GetField( iItemID, 11,	ldtField );
		stLottery.m_iFX1 = ldtField.uData.lValue;

		pLDTFile->GetField( iItemID, 12,	ldtField );
		stLottery.m_iFX2 = ldtField.uData.lValue;

		pLDTFile->GetField( iItemID, 13,	ldtField );
		stLottery.m_iSoundEffect = ldtField.uData.lValue;

		AddPrizeLDT( stLottery );
	}

	SAFE_RELEASE(pLDTFile);

	//////////////////////////////////////////////////////////////////////////

	g_pResourceManager->GetLDTFromFileName( RES_FNAME_LDT_GLOBAL_SYS , &pLDTFile );

	if( pLDTFile == NULL )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "-----------------------------------> LDT Load Fail [ %s ] \n" , RES_FNAME_LDT_GLOBAL_SYS );
#endif
		return;
	}

	pLDTFile->GetField( 41 , 0 ,	ldtField );		//	_Check	Item
	m_iRequireItem	=	ldtField.uData.lValue;

	pLDTFile->GetField( 42 , 0 ,	ldtField );		//	_Check	Ely
	m_uiRequireEly	=	ldtField.uData.lValue;

	SAFE_RELEASE(pLDTFile);

}

//--------------------------------------------------
void SPWindowLottery::LoadGambleTipSetting()
{
	TiXmlDocument	xmlDoc;
	TiXmlElement *	pRoot			= NULL;
	TiXmlElement *	pLotteryNode	= NULL;
	TiXmlElement *  pChildNode		= NULL;
	if (!xmlDoc.LoadFile( "UserOptions.xml", TIXML_ENCODING_UTF8 ))
		return;

	pRoot = xmlDoc.FirstChildElement( _T("UserOptions") );
	if (!pRoot)
		return;

	pLotteryNode = pRoot->FirstChildElement( _T("Lottery") );
	if ( !pLotteryNode )
		return;

	pChildNode = pLotteryNode->FirstChildElement( _T("User") );
	if ( !pChildNode )
		return;

	int   iValue = 1;
	pChildNode->QueryIntAttribute( g_pNetworkManager->GetLoginID(), &iValue );
	m_bShowGambleTip = (bool)iValue;
}

void SPWindowLottery::SaveGambleTipSetting(bool bShowTip)
{
	m_bShowGambleTip  =  bShowTip;

	TiXmlDocument	xmlDoc;
	TiXmlElement *	pRoot			= NULL;
	TiXmlElement *	pLotteryNode	= NULL;
	TiXmlElement *  pChildNode		= NULL;
	xmlDoc.LoadFile( "UserOptions.xml", TIXML_ENCODING_UTF8 );
	
	pRoot = xmlDoc.FirstChildElement( _T("UserOptions") );
	if ( !pRoot )
	{
		pRoot = new TiXmlElement( _T("UserOptions") );
		xmlDoc.LinkEndChild( pRoot );
	}

	pLotteryNode = pRoot->FirstChildElement( _T("Lottery") );
	if ( !pLotteryNode )
	{
		pLotteryNode = new TiXmlElement( _T("Lottery") );
		pRoot->LinkEndChild( pLotteryNode );
	}
    
	pChildNode = pLotteryNode->FirstChildElement( _T("User") );
	if ( !pChildNode )
	{
		pChildNode = new TiXmlElement( _T("User") );
		pChildNode->SetAttribute( g_pNetworkManager->GetLoginID(), m_bShowGambleTip );
		pLotteryNode->LinkEndChild( pChildNode );
	}
	else
	{
		pChildNode->SetAttribute( g_pNetworkManager->GetLoginID(), (int)m_bShowGambleTip );
	}
	
	xmlDoc.SaveFile( "UserOptions.xml" );
}


int	SPWindowLottery::GetLevelIndex(int iLevel)
{
	int  iIndex = 0;
	if ( iLevel > 9 )
		iIndex = 5;
	else if ( iLevel > 6 )
		iIndex = 4;
	else if ( iLevel > 4 )
		iIndex = 3;
	else if ( iLevel > 2 )
		iIndex = 2;
	else if ( iLevel > 0 )
		iIndex = 1;

	return iIndex;
}

DWORD SPWindowLottery::GetLevelColor(int iLevelIndex)
{
	DWORD  dwColor = 0xFFFFFFFF;
	switch ( iLevelIndex )
	{
	case 1:
		dwColor = ARGB(255, 179, 163, 163);
		break;
	case 2:
		dwColor = ARGB(255, 255, 143, 147);
		break;
	case 3:
		dwColor = ARGB(255, 198, 156, 109);
		break;
	case 4:
		dwColor = ARGB(255, 255, 255, 255);
		break;
	case 5:
		dwColor = ARGB(255, 255, 214, 71);
		break;
	}

	return dwColor;
}

bool SPWindowLottery::GetTicketSellIcon( string strFileName, int iIndex, SPTexture * &pTexture, POINT& ptSrcPos )
{
	if ( iIndex <= 0 || iIndex > 576 )
		return NULL;

	ptSrcPos.x = 234 + ((20 + 1) * (iIndex-1));
	ptSrcPos.y = 97 + ((19 + 1) * (iIndex/24));

	g_pVideo->GetTextureMgr()->LoadTexture( strFileName.c_str() , &pTexture );
	return pTexture;
}


bool SPWindowLottery::GetLevelIcon( string strFileName, int iIndex, SPTexture * &pTexture, POINT& ptSrcPos)
{
	if ( iIndex <= 0 || iIndex > 1080 )
		return NULL;

	ptSrcPos.x = 310 + ((16 + 1) * (iIndex-1));
	ptSrcPos.y = 464 + ((14 + 1) * (iIndex/30));

	g_pVideo->GetTextureMgr()->LoadTexture( strFileName.c_str() , &pTexture );
	return pTexture;
}


bool SPWindowLottery::CheckRequire( bool bCheckTicket )
{
	SPPlayer*	pLocalPlayer = g_pGOBManager->GetLocalPlayer();

	const char*		pszGlobalString	=	g_pResourceManager->GetGlobalString( 4000002 );

	if( pLocalPlayer == NULL )
	{
		if( pszGlobalString != NULL )
		{
			SetMessageBoxPos( 400 , 343 );
			SetMessageBox( LOTTERY_STATE_OK , pszGlobalString );
		}

		return false;
	}

	SPGOBStatus*			pPlayerStatus	=	pLocalPlayer->GetStatus();
	SPPlayerInvenArchive*	pInvenArchive	=	pLocalPlayer->GetInvenArchive();

	if( pPlayerStatus == NULL || pInvenArchive == NULL )
	{
		if( pszGlobalString != NULL )
		{
			SetMessageBoxPos( 400 , 343 );
			SetMessageBox( LOTTERY_STATE_OK , pszGlobalString );
		}

		return false;
	}

	//	浇吩 眉农

	for( int i = INVENTORY_EQUIP ; i <= INVENTORY_HOUSING ; ++i )
	{
		if( pInvenArchive->IsFullInventory( (CONTAINER_TYPE)i ) == true )
		{
			pszGlobalString	=	g_pResourceManager->GetGlobalString( 13001011 + i );

			if( pszGlobalString != NULL )
			{
				SetMessageBoxPos( 400 , 343 );
				SetMessageBox( LOTTERY_STATE_OK , pszGlobalString );
			}

			return false;
		}
	}

	//	墨靛 锅龋 蜡瓤 眉农
	int iTicketPos	= m_iSelectTicketX + m_iSelectTicketY * LOTTERY_GUIDE_Y_MAX;
	int	iTicketCount = (int)m_vTicketBuffer.size();

	if ( bCheckTicket )
	{
		if( m_iSelectTicketX == -1 || m_iSelectTicketY == -1 || iTicketPos < 0 || iTicketPos >= 400 || iTicketCount <= iTicketPos )
		{
			pszGlobalString	=	g_pResourceManager->GetGlobalString( 13001001 );

			if( pszGlobalString != NULL )
			{
				SetMessageBoxPos( 400 , 343 );
				SetMessageBox( LOTTERY_STATE_OK , pszGlobalString );
			}

			return false;
		}
	}

	////	备概 啊瓷 酒捞袍 眉农
	if( m_iRequireItem != 0 && g_pCheckManager->CheckPCItem( m_iRequireItem , 1 , 1 ) == false )
	{
		pszGlobalString	=	g_pResourceManager->GetGlobalString( 13001002 );

		if( pszGlobalString != NULL )
		{
			SetMessageBoxPos( 400 , 343 );
			SetMessageBox( LOTTERY_STATE_OK , pszGlobalString );
		}

		return false;
	}

	//	备概 啊瓷 郡府 眉农
	UINT64	uiMoney	=	pPlayerStatus->GetStatusValueEX( STATUS_EX_ELY );

	if( uiMoney < m_uiRequireEly )
	{
		pszGlobalString	=	g_pResourceManager->GetGlobalString( 13001003 );

		if( pszGlobalString != NULL )
		{
			SetMessageBoxPos( 400 , 343 );
			SetMessageBox( LOTTERY_STATE_OK , pszGlobalString );
		}

		return false;
	}

	//	捞固 急琶等 墨靛 眉农
	if ( bCheckTicket )
	{
		int	iTicketData	=	m_vTicketBuffer.at( iTicketPos ).nBuyStep;

		if( iTicketData > 0 )
		{
			pszGlobalString	=	g_pResourceManager->GetGlobalString( 13001004 );

			if( pszGlobalString != NULL )
			{
				SetMessageBoxPos( 400 , 343 );
				SetMessageBox( LOTTERY_STATE_OK , pszGlobalString );
			}

			return false;
		}
	}

	return true;
}

void SPWindowLottery::PrintErrorMsg( GLOBAL_STRING_ID uiGlobalStringID )
{
	const char*		pszGlobalString	=	g_pResourceManager->GetGlobalString( uiGlobalStringID );
	LOTTERY_STATE	eState	=	LOTTERY_STATE_OK;

	if( uiGlobalStringID == 13001005 || uiGlobalStringID == 13001204 || uiGlobalStringID == 13001205 )
		eState	=	LOTTERY_STATE_WAIT;

	if( pszGlobalString != NULL )
	{
		SetMessageBoxPos( 400 , 343 );
		SetMessageBox( eState , pszGlobalString );
	}
}

void SPWindowLottery::InitMessageBox()
{
	SetMessageBox( LOTTERY_STATE_NORMAL , "Normal" );

	m_fMessageBoxLimit					=	2.0f;
	m_fMessageBoxAccmulateTime			=	0.0f;
	m_bMessageBoxShow					=	false;
}

void SPWindowLottery::ProcessMessageBox( float fTime )
{
	if( m_eState != LOTTERY_STATE_WAIT )
		return;

	m_fMessageBoxAccmulateTime	+=	fTime;

	if( m_fMessageBoxAccmulateTime >= m_fMessageBoxLimit ||/*&&*/ m_bMessageBoxShow == false )
	{
		InitMessageBox();
	}
}

void SPWindowLottery::HideMessageBox()
{
	m_bMessageBoxShow			=	false;
}

void SPWindowLottery::SetMessageBox( LOTTERY_STATE eState , const char* pstrMessage )
{
	if( pstrMessage == NULL )
		return;

	if( m_pMessageBox == NULL )
		m_pMessageBox	=	Find( WIID_LOTTERY_MESSAGE_BOX_BG, true );

	if( m_pMessageBox == NULL )
		return;

	SPWindow*	pWindow;

	m_eState	=	eState;

	pWindow	=	m_pMessageBox->Find( WIID_LOTTERY_MESSAGE_BOX_TEXT, true );

	if( pWindow != NULL )
		pWindow->SetWindowText( pstrMessage );

	pWindow	=	m_pMessageBox->Find( WIID_LOTTERY_MESSAGE_BOX_OK_TEXT, true );

	if( pWindow != NULL )
		pWindow->SetWindowText( pstrMessage );

	if( m_eState == LOTTERY_STATE_YESNO )
	{
		pWindow	=	m_pMessageBox->Find( WIID_LOTTERY_MESSAGE_BOX_TICKET_X, true );

		if( pWindow != NULL )
			pWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , m_iSelectTicketX );

		pWindow	=	m_pMessageBox->Find( WIID_LOTTERY_MESSAGE_BOX_TICKET_Y, true );

		if( pWindow != NULL )
			pWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , m_iSelectTicketY );
	}
	else if( m_eState == LOTTERY_STATE_WAIT )
	{
		m_fMessageBoxAccmulateTime	=	0.0f;
		m_bMessageBoxShow			=	true;
	}

	int iInstanceStart	=	WIID_LOTTERY_MESSAGE_BOX_NORMAL_START;
	int iInstanceEnd	=	WIID_LOTTERY_MESSAGE_BOX_NORMAL_END;

	for( int i = iInstanceStart ; i <= iInstanceEnd ; ++i )
	{
		pWindow	=	m_pMessageBox->Find( i, true );

		if( pWindow != NULL )
			pWindow->SetShowEnable( true );
	}

	m_pMessageBox->SetShowEnable( true );
	m_pMessageBox->Show();

	UpdateMessageBox();
}

void SPWindowLottery::SetMessageBoxPos( int iPosX , int iPosY )
{
	if( m_pMessageBox == NULL )
		m_pMessageBox	=	Find( WIID_LOTTERY_MESSAGE_BOX_BG, true );

	if( m_pMessageBox == NULL )
		return;

	RECT	rcWindowRect;

	GetWindowRect( rcWindowRect );

	int	iXSize	=	229 / 2;
	int	iYSize	=	146 / 2;

	if( rcWindowRect.left > iPosX - iXSize )
	{
		iPosX	=	rcWindowRect.left + iXSize;
	}

	if( rcWindowRect.top > iPosY - iYSize )
	{
		iPosY	=	rcWindowRect.top + iYSize;
	}

	if( rcWindowRect.right < iPosX + iXSize )
	{
		iPosX	=	rcWindowRect.right - iXSize;
	}

	if( rcWindowRect.bottom < iPosY + iYSize )
	{
		iPosY	=	rcWindowRect.bottom - iYSize;
	}

	iPosX	-=	iXSize;
	iPosY	-=	iYSize;

	iPosX	-=	2;
	iPosY	-=	27;

	//////////////////////////////////////////////////////////////////////////

	SPWindow*	pWindow;

	pWindow	=	m_pMessageBox->Find( WIID_LOTTERY_MESSAGE_BOX_BG_L_T, true );

	if( pWindow != NULL )
		pWindow->AdjustCoord( iPosX - m_iAX , iPosY - m_iAY );

	pWindow	=	m_pMessageBox->Find( WIID_LOTTERY_MESSAGE_BOX_BG_C_T, true );

	if( pWindow != NULL )
		pWindow->AdjustCoord( iPosX + 4 - m_iAX , iPosY - m_iAY );

	pWindow	=	m_pMessageBox->Find( WIID_LOTTERY_MESSAGE_BOX_BG_R_T, true );

	if( pWindow != NULL )
		pWindow->AdjustCoord( iPosX + 225 - m_iAX , iPosY - m_iAY );

	pWindow	=	m_pMessageBox->Find( WIID_LOTTERY_MESSAGE_BOX_BG_L_M, true );

	if( pWindow != NULL )
		pWindow->AdjustCoord( iPosX - m_iAX , iPosY + 4 - m_iAY );

	pWindow	=	m_pMessageBox->Find( WIID_LOTTERY_MESSAGE_BOX_BG_C_M, true );

	if( pWindow != NULL )
		pWindow->AdjustCoord( iPosX + 4 - m_iAX , iPosY + 4 - m_iAY );

	pWindow	=	m_pMessageBox->Find( WIID_LOTTERY_MESSAGE_BOX_BG_R_M, true );

	if( pWindow != NULL )
		pWindow->AdjustCoord( iPosX + 225 - m_iAX , iPosY + 4 - m_iAY );

	pWindow	=	m_pMessageBox->Find( WIID_LOTTERY_MESSAGE_BOX_BG_L_B, true );

	if( pWindow != NULL )
		pWindow->AdjustCoord( iPosX - m_iAX , iPosY + 142 - m_iAY );

	pWindow	=	m_pMessageBox->Find( WIID_LOTTERY_MESSAGE_BOX_BG_C_B, true );

	if( pWindow != NULL )
		pWindow->AdjustCoord( iPosX + 4 - m_iAX , iPosY + 142 - m_iAY );

	pWindow	=	m_pMessageBox->Find( WIID_LOTTERY_MESSAGE_BOX_BG_R_B, true );

	if( pWindow != NULL )
		pWindow->AdjustCoord( iPosX + 225 - m_iAX , iPosY + 142 - m_iAY );

	//////////////////////////////////////////////////////////////////////////

	pWindow	=	m_pMessageBox->Find( WIID_LOTTERY_MESSAGE_BOX_BUY_HELP, true );

	if( pWindow != NULL )
		pWindow->AdjustCoord( iPosX + m_iMessageBoxBuyHelpX - m_iAX , iPosY + m_iMessageBoxBuyHelpY - m_iAY );

	pWindow	=	m_pMessageBox->Find( WIID_LOTTERY_MESSAGE_BOX_BUY_BG, true );

	if( pWindow != NULL )
		pWindow->AdjustCoord( iPosX + m_iMessageBoxBuyBGX - m_iAX , iPosY + m_iMessageBoxBuyBGY - m_iAY );

	pWindow	=	m_pMessageBox->Find( WIID_LOTTERY_MESSAGE_BOX_BUY_BG_L, true );

	if( pWindow != NULL )
		pWindow->AdjustCoord( iPosX + 10 - m_iAX , iPosY + 29 - m_iAY );

	pWindow	=	m_pMessageBox->Find( WIID_LOTTERY_MESSAGE_BOX_BUY_BG_C, true );

	if( pWindow != NULL )
		pWindow->AdjustCoord( iPosX + 12 - m_iAX , iPosY + 29 - m_iAY );

	pWindow	=	m_pMessageBox->Find( WIID_LOTTERY_MESSAGE_BOX_BUY_BG_R, true );

	if( pWindow != NULL )
		pWindow->AdjustCoord( iPosX + 217 - m_iAX , iPosY + 29 - m_iAY );

	//////////////////////////////////////////////////////////////////////////

	pWindow	=	m_pMessageBox->Find( WIID_LOTTERY_MESSAGE_BOX_TEXT, true );

	if( pWindow != NULL )
		pWindow->AdjustCoord( iPosX + 10 - m_iAX , iPosY + 68 - m_iAY );

	pWindow	=	m_pMessageBox->Find( WIID_LOTTERY_MESSAGE_BOX_OK_TEXT, true );

	if( pWindow != NULL )
		pWindow->AdjustCoord( iPosX + 10 - m_iAX , iPosY + 29 - m_iAY );

	pWindow	=	m_pMessageBox->Find( WIID_LOTTERY_MESSAGE_BOX_OK, true );

	if( pWindow != NULL )
		pWindow->AdjustCoord( iPosX + 82 - m_iAX , iPosY + 119 - m_iAY );

	pWindow	=	m_pMessageBox->Find( WIID_LOTTERY_MESSAGE_BOX_YES, true );

	if( pWindow != NULL )
		pWindow->AdjustCoord( iPosX + 45 - m_iAX , iPosY + 119 - m_iAY );

	pWindow	=	m_pMessageBox->Find( WIID_LOTTERY_MESSAGE_BOX_NO, true );

	if( pWindow != NULL )
		pWindow->AdjustCoord( iPosX + 120 - m_iAX , iPosY + 119 - m_iAY );

	pWindow	=	m_pMessageBox->Find( WIID_LOTTERY_MESSAGE_BOX_TITLE, true );

	if( pWindow != NULL )
		pWindow->AdjustCoord( iPosX + 10 - m_iAX , iPosY + 10 - m_iAY );

	//////////////////////////////////////////////////////////////////////////

	pWindow	=	m_pMessageBox->Find( WIID_LOTTERY_MESSAGE_BOX_TICKET_Y, true );

	if( pWindow != NULL )
		pWindow->AdjustCoord( iPosX + m_iMessageBoxTicketYX - m_iAX , iPosY + m_iMessageBoxTicketYY - m_iAY );

	pWindow	=	m_pMessageBox->Find( WIID_LOTTERY_MESSAGE_BOX_TICKET_X, true );

	if( pWindow != NULL )
		pWindow->AdjustCoord( iPosX + m_iMessageBoxTicketXX - m_iAX , iPosY + m_iMessageBoxTicketXY - m_iAY );

	//////////////////////////////////////////////////////////////////////////
}

void SPWindowLottery::UpdateMessageBox()
{
	if( m_pMessageBox == NULL )
		m_pMessageBox	=	Find( WIID_LOTTERY_MESSAGE_BOX_BG, true );

	if( m_pMessageBox == NULL )
		return;

	SPWindow*	pWindow;
	int			i , iInstanceStart , iInstanceEnd;

	pWindow	=	m_pMessageBox->Find( WIID_LOTTERY_MESSAGE_BOX_TITLE, true );

	if( m_eState == LOTTERY_STATE_WAIT )
	{
		if( pWindow != NULL )
			pWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , 1 );

		iInstanceStart	=	WIID_LOTTERY_MESSAGE_BOX_WAIT_START;
		iInstanceEnd	=	WIID_LOTTERY_MESSAGE_BOX_WAIT_END;
	}
	else if( m_eState == LOTTERY_STATE_OK )
	{
		if( pWindow != NULL )
			pWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , 1 );

		iInstanceStart	=	WIID_LOTTERY_MESSAGE_BOX_OK_START;
		iInstanceEnd	=	WIID_LOTTERY_MESSAGE_BOX_OK_END;
	}
	else if( m_eState == LOTTERY_STATE_YESNO )
	{
		if( pWindow != NULL )
			pWindow->SPSendMessage( SPIM_SET_IMAGEINDEX , 0 );

		iInstanceStart	=	WIID_LOTTERY_MESSAGE_BOX_YESNO_START;
		iInstanceEnd	=	WIID_LOTTERY_MESSAGE_BOX_YESNO_END;
	}
	else
	{
		iInstanceStart	=	WIID_LOTTERY_MESSAGE_BOX_NORMAL_START;
		iInstanceEnd	=	WIID_LOTTERY_MESSAGE_BOX_NORMAL_END;
	}

	for( i = iInstanceStart ; i <= iInstanceEnd ; ++i )
	{
		pWindow	=	m_pMessageBox->Find( i, true );

		if( pWindow != NULL )
		{
			pWindow->SetShowEnable( false );
			pWindow->Hide();
		}
	}

	m_pMessageBox->SetShowEnable( true );
	m_pMessageBox->Show();
}

void SPWindowLottery::ShowTurnTip()
{
	SPWindow * pkParent = Find(WIID_LOTTERY_TURN_TIP, true);
	if ( !pkParent )
		return;

	SPWindowButton * pkCheck = (SPWindowButton *)pkParent->Find(WIID_LOTTERY_TURN_TIP_CHECK);
	if ( pkCheck )
	{
		pkCheck->SPSendMessage( SPIM_BTN_SELECT, !m_bShowGambleTip );
	}

	const char*		pszGlobalString1	=	g_pResourceManager->GetGlobalString( 13001435 );
	SPWindowStatic * pkStatic = (SPWindowStatic *)pkParent->Find(WIID_LOTTERY_TURN_TIP_STATIC1);
	if ( pkStatic && pszGlobalString1 )
	{
		SPItemAttr* pkItemAttr = NULL;
		g_pItemCluster->GetItemInfo(m_itemIDFirstGetted.itemID, pkItemAttr);
		if( pkItemAttr )
		{
			char   szText[256];
			sprintf(szText, pszGlobalString1, pkItemAttr->m_strName.c_str());
			pkStatic->SetWindowText(szText);
		}
	}

	if ( 1/*m_bShowGambleTip*/ )
		pkParent->Show();
	else
		pkParent->Hide();
}

void SPWindowLottery::HideTurnTip()
{
	SPWindow * pkParent = Find(WIID_LOTTERY_TURN_TIP, true);
	if ( !pkParent )
		return;

	pkParent->Hide();
}

void SPWindowLottery::ShowWindowTurn()
{
	SPWindow *			pkParent;
	SPWindow *			pkWindowParent;
	SPWindowStatic *	pkStatic;
	SPWindowButton *    pkButton;

	pkParent = Find(WIID_LOTTERY_TURN, true);
	if ( !pkParent )
		return;

	pkParent->Show();
	pkWindowParent = pkParent->Find(WIID_LOTTERY_TURN_FRAME, true);
	if ( !pkWindowParent )
	{
		pkParent->Hide();
		return;
	}

	ITEMID   itemID = 0;
	UINT32   uSetID = 0;  
	int		 iLevel = 0;
	int      iNum	= 0;
	POINT	 ptSrcPos;

	for (int i=0; i<9; i++)
	{
		if ( i == 4 )
		{
			itemID = m_itemIDFirstGetted.itemID;
			uSetID = m_itemIDFirstGetted.uSetID;
			iNum   = m_itemIDFirstGetted.usCount;
		}
		else
		{
			int  n = i > 4 ? (i-1) : i;
			int  iLightArray[] = { 0, 1, 2, 7, 3, 6, 5, 4 };
			if ( n < m_vecTurnItem.size() )
			{
				itemID = m_vecTurnItem[iLightArray[n]].itemID;
				uSetID = m_vecTurnItem[iLightArray[n]].uSetID;
				iNum   = m_vecTurnItem[iLightArray[n]].usCount;
			}
		}
		LOTTERY_LDT*	pLotteryLdt		=	GetPrizeLDT( uSetID );
		if( pLotteryLdt )
		{
			iLevel	=	pLotteryLdt->m_iLevel;
		}

		//
		pkStatic = static_cast<SPWindowStatic *>(pkWindowParent->Find( WIID_LOTTERY_TURN_LIGHT + i, true ));
		pkStatic->Hide();

		//
		pkStatic = static_cast<SPWindowStatic *>(pkWindowParent->Find( WIID_LOTTERY_TURN_ITEM + i, true ));
		SPTexture* pkTexture = g_pItemCluster->GetItemTexture(itemID, ptSrcPos);
		pkStatic->SPSendMessage(SPIM_SET_IMAGECLEAR);
		pkStatic->SetImage(pkTexture, ptSrcPos.x, ptSrcPos.y);
		if ( pkTexture ) pkTexture->IncreaseRef();
		pkStatic->Show();

		//
		pkStatic = static_cast<SPWindowStatic *>(pkWindowParent->Find( WIID_LOTTERY_TURN_LEVEL + i, true ));
		int iLevelIndex	=	GetLevelIndex(iLevel);
		if ( iLevelIndex > 0 )
		{
			SPTexture* pkTexture = NULL;
			GetLevelIcon("DATA/INTERFACE/CONCEPT/UI110.PNG", iLevelIndex, pkTexture, ptSrcPos);
			pkStatic->SPSendMessage(SPIM_SET_IMAGECLEAR);
			pkStatic->SetImage(pkTexture, ptSrcPos.x, ptSrcPos.y);
			pkStatic->Show();
		}
		else
		{
			pkStatic->Hide();
		}

		//
		pkStatic = static_cast<SPWindowStatic *>(pkWindowParent->Find( WIID_LOTTERY_TURN_NUM + i, true ));
		if ( iNum > 1 )
		{
			char  szNum[16];
			sprintf(szNum, "%d", iNum);
			pkStatic->SetWindowText(szNum);
			pkStatic->Show();
		}
		else
		{
			pkStatic->Hide();
		}

		//
		pkStatic = static_cast<SPWindowStatic *>(pkWindowParent->Find( WIID_LOTTERY_TURN_DISABLE + i, true ));
		if ( i == 4 )
			pkStatic->Hide();
		else
			pkStatic->Show();
	}

	pkButton = static_cast<SPWindowButton *>(pkWindowParent->Find( WIID_LOTTERY_TURN_CONTINUE, true ));
	if ( pkButton )
	{
		pkButton->Show();
		pkButton->SetEnable(true);
	}

	pkButton = static_cast<SPWindowButton *>(pkWindowParent->Find( WIID_LOTTERY_TURN_STOP, true  ));
	if ( pkButton )
	{
		pkButton->Hide();
		pkButton->SetEnable(false);
	}

	pkButton = static_cast<SPWindowButton *>(pkWindowParent->Find( WIID_LOTTERY_TURN_ACCEPT, true  ));
	if ( pkButton )
	{
		pkButton->Show();
		pkButton->SetEnable(true);
	}

	m_bStopTurn		= false;
	m_bTurnStoping  = false;
	m_fTurnRotation	= 0.0f;

	ShowTurnTip();

}

void SPWindowLottery::HideWindowTurn()
{
	SPWindow * pkWindow = Find(WIID_LOTTERY_TURN, true);
	if ( pkWindow )
	{
		pkWindow->Hide();
	}
}

void SPWindowLottery::StartTurn()
{
	m_bStartTurn		= true;
	m_bStopTurn			= false;
	m_bTurnStoping		= false;
	m_iCurItemIndex		= 0;
	m_fChangeSpeed		= 0.5f;
	m_fTurnDstTime		= 0.0f;
	m_fStartDstTime		= 0.0f;
	m_fTurnRotation		= 0.0f;

	SPWindowStatic *	pkStatic;
	SPWindow *	pkWindowParent = Find(WIID_LOTTERY_TURN_FRAME, true);
	if ( !pkWindowParent )
		return;

	for (int i=0; i < 9; i++)
	{
		pkStatic = static_cast<SPWindowStatic *>(pkWindowParent->Find( WIID_LOTTERY_TURN_LIGHT + i, true ));
		if ( i == 0 )
			pkStatic->Show();
		else
			pkStatic->Hide();

		pkStatic = static_cast<SPWindowStatic *>(pkWindowParent->Find( WIID_LOTTERY_TURN_DISABLE + i, true ));
		if ( i == 4 )
			pkStatic->Show();
		else
			pkStatic->Hide();
	}

	SPWindowButton * pkButton = static_cast<SPWindowButton *>(pkWindowParent->Find( WIID_LOTTERY_TURN_CONTINUE, true  ));
	if ( pkButton )
	{
		pkButton->Hide();
		pkButton->SetEnable(false);
	}

	pkButton = static_cast<SPWindowButton *>(pkWindowParent->Find( WIID_LOTTERY_TURN_STOP, true  ));
	if ( pkButton )
	{
		pkButton->Show();
		pkButton->SetEnable(false);
	}

	pkButton = static_cast<SPWindowButton *>(pkWindowParent->Find( WIID_LOTTERY_TURN_ACCEPT, true  ));
	if ( pkButton )
	{
		pkButton->Show();
		pkButton->SetEnable(false);
	}

	//
	SPPlayer*		pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();
	if( !pLocalPlayer )
		return;

	SPSoundArchive* pSoundArchive	=	NULL;
	pLocalPlayer->SPGOBPerformMessage( SPGM_GETSOUNDARCHIVE , (LPARAM)&pSoundArchive );

	SOUND_TABLE_INFO* pInfo = SPSoundManager::GetInstance()->GetSoundTable( 186 );
	if( pInfo != NULL )
	{
		std::string strFullName;
		for( int i = 0; i < 3; i++ )
		{
			if( pInfo->strFileName[i].empty() )
				continue;

			strFullName = "DATA/SOUND/";
			strFullName += pInfo->strFileName[ i ];
			pSoundArchive->InsertItem( strFullName.c_str() , pInfo->fDelayTime[ i ] , pInfo->bLoop[ i ] );
		}
	}

}

void SPWindowLottery::StopTurn()
{
	m_bStartTurn		= false;
	m_bStopTurn			= true;
	m_bTurnStoping		= true;

	SPWindow *	pkWindowParent = Find(WIID_LOTTERY_TURN_FRAME, true);
	if ( !pkWindowParent )
		return;

	SPWindowButton * pkButton = static_cast<SPWindowButton *>(pkWindowParent->Find( WIID_LOTTERY_TURN_CONTINUE, true  ));
	if ( pkButton )
		pkButton->SetEnable(false);

	pkButton = static_cast<SPWindowButton *>(pkWindowParent->Find( WIID_LOTTERY_TURN_ACCEPT, true  ));
	if ( pkButton )
		pkButton->SetEnable(true);

	//
	SPPlayer*		pLocalPlayer	=	g_pGOBManager->GetLocalPlayer();
	if( !pLocalPlayer )
		return;

	SPSoundArchive* pSoundArchive	=	NULL;
	pLocalPlayer->SPGOBPerformMessage( SPGM_GETSOUNDARCHIVE , (LPARAM)&pSoundArchive );

	SOUND_TABLE_INFO* pInfo = SPSoundManager::GetInstance()->GetSoundTable( 186 );
	if( pInfo != NULL )
	{
		std::string strFullName;
		for( int i = 0; i < 3; i++ )
		{
			if( pInfo->strFileName[i].empty() )
				continue;

			strFullName = "DATA/SOUND/";
			strFullName += pInfo->strFileName[ i ];
			pSoundArchive->ForceRelease( strFullName.c_str() );
		}
	}

	PlayLotteryFx(m_itemIDSecondGetted.uSetID);

}

void SPWindowLottery::ProcessWindowTurn( float fTime )
{
	SPWindow *			pkWindowParent;
	SPWindowStatic *	pkStatic;

	pkWindowParent = Find(WIID_LOTTERY_TURN_FRAME, true);
	if ( !pkWindowParent || !pkWindowParent->IsShow() )
		return;

	if ( !m_bStartTurn )
	{
		pkStatic = static_cast<SPWindowStatic *>(pkWindowParent->Find( WIID_LOTTERY_TURN_LIGHT + 4, true ));
		if ( pkStatic && !m_bStopTurn )
		{
			static  float   s_fFlashTime = 0.0f;
			s_fFlashTime += fTime;
			if ( (int)(s_fFlashTime/0.4) )
			{
				s_fFlashTime = 0.0f;
				if ( pkStatic->IsShow() )
					pkStatic->Hide();
				else
					pkStatic->Show();
			}
		}
		return;
	}

	//////////////
	// 0   1   2
	// 7   4   3
	// 6   5   4
	int  iLightArray[] = { 0, 1, 2, 7, 3, 6, 5, 4 };

	//时间变量
	m_fStartDstTime += fTime;

	static float fDstTime = m_fStartDstTime;
	if ( !m_bTurnStoping )
		fDstTime = m_fStartDstTime;

	static float fStopTime = m_fStartDstTime;
	if ( !m_bStopTurn )
		fStopTime = m_fStartDstTime;

	//停止倒计时
	SPWindowButton * pkButton = NULL;
	pkButton = static_cast<SPWindowButton *>(pkWindowParent->Find( WIID_LOTTERY_TURN_STOP, true  ));
	if ( pkButton )
	{
		if ( m_fStartDstTime >= 10.0f )
		{
			OnStopTurn(NULL, NULL);
		}
		else if ( m_fStartDstTime >= 3.0f )
		{
			if ( !pkButton->IsEnable() && !m_bTurnStoping )
			{
				pkButton->SetImageDisable( "DATA/INTERFACE/CONCEPT/UI110.PNG", 399, 438 );
				pkButton->SetEnable(true);
			}
		}
		else
		{
			int  iNum = 3 - (int)m_fStartDstTime;
			if ( iNum == 3 )
				pkButton->SetImageDisable( "DATA/INTERFACE/CONCEPT/UI111.PNG", 386, 1 );
			else if ( iNum == 2 )
				pkButton->SetImageDisable( "DATA/INTERFACE/CONCEPT/UI111.PNG", 386, 27 );
			else if ( iNum == 1 )
				pkButton->SetImageDisable( "DATA/INTERFACE/CONCEPT/UI111.PNG", 386, 53 );
			pkButton->SetEnable(false);
		}
	}

	//速度计算
	if ( m_fStartDstTime <= 1.0 )
	{
		m_fChangeSpeed = 0.4f;
	}
	else if ( m_fStartDstTime <= 1.5 )
	{
		m_fChangeSpeed = 0.2f;
	}
	else if ( m_fStartDstTime <= 2.0 )
	{
		m_fChangeSpeed = 0.1f;
	}
	else if ( m_fStartDstTime <= 2.5 )
	{
		m_fChangeSpeed = 0.03f;
	}
	else if ( m_fStartDstTime <= 3.5 )
	{
		m_fChangeSpeed = 0.05f;
	}
	else if ( m_bTurnStoping && m_fStartDstTime > 3.0 )
	{
		if ( !m_bStopTurn )
			m_fChangeSpeed = 0.1f;

		if (m_fStartDstTime > fDstTime + 0.3 )
		{
			if ( !m_bStopTurn )
				m_fChangeSpeed = 0.2f;

			if ( abs(m_iTurnRandom - m_iCurItemIndex) == 4 )
			{
				m_bStopTurn	 = true;
				m_fChangeSpeed = 0.3f;
				if ( m_fStartDstTime > fStopTime + 0.6 )
				{
					m_fChangeSpeed = 0.4f;
				}
			}
		}
	}

	//转盘动画
	static float  s_fTurnDstTime = 0.0f;
	s_fTurnDstTime += fTime;
	if ( s_fTurnDstTime >= 0.05 && !m_bTurnStoping )
	{
		++m_fTurnRotation;
		s_fTurnDstTime = 0.0f;
		if ( m_fTurnRotation >= 360.0 )
			m_fTurnRotation = 0.0;
	}

	//改变转盘
	m_fTurnDstTime += fTime;
	if ( m_fTurnDstTime >= m_fChangeSpeed )
	{
		++m_iCurItemIndex;
		m_fTurnDstTime = 0.0f;
		if ( m_iCurItemIndex >= 8 )
			m_iCurItemIndex = 0;

		for (int i=0; i < 9; i++)
		{
			pkStatic = static_cast<SPWindowStatic *>(pkWindowParent->Find( WIID_LOTTERY_TURN_LIGHT + i, true ));
			if ( pkStatic )
			{
				int  n = i > 4 ? (i-1) : i;
				if ( i != 4 && iLightArray[n] == m_iCurItemIndex )
				{
					pkStatic->Show();
					if ( m_bStopTurn && m_itemIDSecondGetted.itemID == m_vecTurnItem.at(iLightArray[n]).itemID )
					{
						StopTurn();
					}
				}
				else
				{
					pkStatic->Hide();
				}
			}
		}
	}

}

void SPWindowLottery::SetSelectTicket( unsigned int iID )
{
	int	iCurPos = iID - WIID_LOTTERY_TICKET;

	if( iCurPos < 0 )
		return;

	int	iYPos	=	iCurPos	/	LOTTERY_GUIDE_Y_MAX;
	int	iXPos	=	iCurPos	-	( iYPos * LOTTERY_GUIDE_Y_MAX );

	SetSelectTicket( iXPos , iYPos );
}

void SPWindowLottery::SetSelectTicket( int iXPos , int iYPos )
{
	SPWindow*	pWindowGuideX;
	SPWindow*	pWindowGuideY;
	SPWindow*	pWindowBuy;
	int			iTicketPos;

	//////////////////////////////////////////////////////////////////////////

	iTicketPos	=	m_iSelectTicketX + m_iSelectTicketY * LOTTERY_GUIDE_Y_MAX;

	if( iTicketPos >= 0 && iTicketPos < LOTTERY_GUIDE_XY_MAX )
	{
		if( m_pTicket[ iTicketPos ] != NULL )
		{
			m_pTicket[ iTicketPos ]->SPSendMessage( SPIM_BTN_SELECT , FALSE );
		}
	}

	//////////////////////////////////////////////////////////////////////////

	m_iSelectTicketX	=	iXPos;
	m_iSelectTicketY	=	iYPos;

	//////////////////////////////////////////////////////////////////////////

	iTicketPos	=	m_iSelectTicketX + m_iSelectTicketY * LOTTERY_GUIDE_Y_MAX;

	if( iTicketPos >= 0 && iTicketPos < LOTTERY_GUIDE_XY_MAX )
	{
		if( m_pTicket[ iTicketPos ] != NULL )
		{
			m_pTicket[ iTicketPos ]->SPSendMessage( SPIM_BTN_SELECT , TRUE );
		}
	}


	pWindowBuy		=	Find( WIID_LOTTERY_BUY_TICKET, true );

	if( pWindowBuy != NULL )
	{
		pWindowBuy->SetEnable( true );
	}
}

void SPWindowLottery::ClearSelectTicket()
{
	SPWindow*	pWindowGuideX;
	SPWindow*	pWindowGuideY;
	SPWindow*	pWindowBuy;
	int			iTicketPos;

	iTicketPos	=	m_iSelectTicketX + m_iSelectTicketY * LOTTERY_GUIDE_Y_MAX;

	if( iTicketPos >= 0 && iTicketPos < LOTTERY_GUIDE_XY_MAX )
	{
		if( m_pTicket[ iTicketPos ] != NULL )
		{
			m_pTicket[ iTicketPos ]->SPSendMessage( SPIM_BTN_SELECT , FALSE );
		}
	}

	//////////////////////////////////////////////////////////////////////////

	m_iSelectTicketX	=	-1;
	m_iSelectTicketY	=	-1;

	//////////////////////////////////////////////////////////////////////////

	pWindowBuy		=	Find( WIID_LOTTERY_BUY_TICKET, true );

	if( pWindowBuy != NULL )
	{
		pWindowBuy->SetEnable( false );
	}
}

void SPWindowLottery::SetTicketData( UINT16 uiTicketCount, t_Gamble_Status	status[])
{
	for ( int i = 0; i < uiTicketCount; i++ )
	{
		if ( status[i].uGambleIndex >= 0 && status[i].uGambleIndex < m_vTicketBuffer.size() )
		{
			m_vTicketBuffer.at(status[i].uGambleIndex) = status[i];
			if ( status[i].nBuyStep == 0 )
				m_vRandomBuffer.push_back( status[i] );
		}
	}

}

void SPWindowLottery::UpdateTicket()
{
	int	iTicketCount	=	(int)m_vTicketBuffer.size();
	int	i = 0;

	int	iTicketData , iTicketPos;
	for( i = 0 ; i < LOTTERY_GUIDE_XY_MAX ; ++i )
	{
		iTicketPos	=	i;

		if( iTicketPos >= iTicketCount )
			break;

		iTicketData		=	m_vTicketBuffer.at( iTicketPos ).nBuyStep;
		UINT32  uSetID  =	m_vTicketBuffer.at( iTicketPos ).uSetID;

		if( m_pTicket[ i ] != NULL && m_pTicketSell[ i ] != NULL && m_pTicketDisable[ i ] != NULL )
		{
			if( iTicketData == 0 )
			{
				m_pTicket[ i ]->Show();
				m_pTicketSell[ i ]->Hide();
				m_pTicketDisable[ i ]->Hide();
			}
			else
			{
				m_pTicket[ i ]->Hide();
				m_pTicketSell[ i ]->Show();
				m_pTicketDisable[ i ]->Hide();

				LOTTERY_LDT*	pLotteryLdt		=	GetPrizeLDT( uSetID );
				if( pLotteryLdt )
				{
					POINT	 ptSrcPos;
					SPTexture * pTexture = NULL;
					int iLevelIndex = GetLevelIndex(pLotteryLdt->m_iLevel);
					GetTicketSellIcon("DATA/INTERFACE/CONCEPT/UI110.PNG", iLevelIndex, pTexture, ptSrcPos);

					if ( pTexture )
					{
						((SPWindowButton * )m_pTicketSell[ i ])->SetImage		( pTexture , ptSrcPos.x , ptSrcPos.y );
						((SPWindowButton * )m_pTicketSell[ i ])->SetImageHit	( pTexture , ptSrcPos.x , ptSrcPos.y );
						((SPWindowButton * )m_pTicketSell[ i ])->SetImagePush	( pTexture , ptSrcPos.x , ptSrcPos.y );
						((SPWindowButton * )m_pTicketSell[ i ])->SetImageDisable( pTexture , ptSrcPos.x , ptSrcPos.y );
						pTexture->IncreaseRef();
						pTexture->IncreaseRef();
						pTexture->IncreaseRef();
					}
				}
			}
		}
	}

	for( ; i < LOTTERY_GUIDE_XY_MAX ; ++i )
	{
		if( m_pTicket[ i ] != NULL && m_pTicketSell[ i ] != NULL && m_pTicketDisable[ i ] != NULL )
		{
			m_pTicket[ i ]->Hide();
			m_pTicketSell[ i ]->Hide();
			m_pTicketDisable[ i ]->Show();
		}
	}
}

void SPWindowLottery::UpdateTicket( int iIndex )
{
	if( iIndex < 0 || iIndex >= LOTTERY_GUIDE_XY_MAX || iIndex >= m_vTicketBuffer.size() )
		return;

	int iTicketData		=	m_vTicketBuffer.at( iIndex ).nBuyStep;
	UINT32  uSetID		=	m_vTicketBuffer.at( iIndex ).uSetID;

	if( m_pTicket[ iIndex ] != NULL && m_pTicketSell[ iIndex ] != NULL && m_pTicketDisable[ iIndex ] != NULL )
	{
		if( iTicketData == 0 )
		{
			m_pTicket[ iIndex ]->Show();
			m_pTicketSell[ iIndex ]->Hide();
			m_pTicketDisable[ iIndex ]->Hide();
		}
		else
		{
			m_pTicket[ iIndex ]->Hide();
			m_pTicketSell[ iIndex ]->Show();
			m_pTicketDisable[ iIndex ]->Hide();

			LOTTERY_LDT*	pLotteryLdt		=	GetPrizeLDT( uSetID );
			if( pLotteryLdt )
			{
				POINT	 ptSrcPos;
				SPTexture * pTexture = NULL;
				int iLevelIndex = GetLevelIndex(pLotteryLdt->m_iLevel);
				GetTicketSellIcon("DATA/INTERFACE/CONCEPT/UI110.PNG", iLevelIndex, pTexture, ptSrcPos);

				if ( pTexture )
				{
					((SPWindowButton * )m_pTicketSell[ iIndex ])->SetImage		( pTexture , ptSrcPos.x , ptSrcPos.y );
					((SPWindowButton * )m_pTicketSell[ iIndex ])->SetImageHit	( pTexture , ptSrcPos.x , ptSrcPos.y );
					((SPWindowButton * )m_pTicketSell[ iIndex ])->SetImagePush	( pTexture , ptSrcPos.x , ptSrcPos.y );
					((SPWindowButton * )m_pTicketSell[ iIndex ])->SetImageDisable( pTexture , ptSrcPos.x , ptSrcPos.y );
					pTexture->IncreaseRef();
					pTexture->IncreaseRef();
					pTexture->IncreaseRef();
				}
			}
		}
	}

}

int SPWindowLottery::AcceptItem( int iIndex, bool bAccept )
{
	if ( iIndex == 1 )   //第一次奖品处理
	{
		if( g_pCheckManager != NULL )
		{
			if( g_pCheckManager->IsDBBlock() == true )
			{
				PrintErrorMsg( 4000002 );
				return 1;
			}
		}

		if( bAccept == false && CheckRequire(false) == false )
			return 1;

		CONTAINER_ITEM	Item;
		ZeroMemory( &Item , sizeof( CONTAINER_ITEM) );

		if( m_iRequireItem != 0 )
		{
			SPItemAttr* pItem;

			if( g_pItemCluster->GetItemInfo( m_iRequireItem , pItem ) == false )
				return 1;

			if( pItem == NULL )
				return 1;

			SPPlayer*	pLocalPlayer = g_pGOBManager->GetLocalPlayer();

			if( pLocalPlayer == NULL )
				return 1;

			SPPlayerInvenArchive*	pInvenArchive	=	pLocalPlayer->GetInvenArchive();

			if( pInvenArchive == NULL )
				return 1;

			CONTAINER_TYPE	eItemType = (CONTAINER_TYPE)( pItem->m_eType - 1 );
			SPItem* pRequireItem = pInvenArchive->GetSPItem( eItemType , m_iRequireItem );

			if( pRequireItem != NULL )
			{
				pRequireItem->ConvertContainerItem( &Item );
				++Item.SlotInfo.SlotIndex;
			}
			else
			{
				//点券购买
				memset(&Item, 0x00, sizeof(CONTAINER_ITEM));
				Item.SlotInfo.ContainerType = CONTAINER_TYPE_INVALID;
			}
		}
		int accept = bAccept ? 0 : 1;

		CPacket	Packet( GAMBLE_CS_ACCEPT_ITEM );
		Packet.AddUINT64( m_itemVersion );
		Packet.AddUINT8 ( accept );
		Packet.AddData( &m_itemIDFirstGetted, sizeof(m_itemIDFirstGetted) );
		Packet.AddData( &(Item.SlotInfo) , sizeof(CONTAINER_SLOT) );
		Packet.AddData( &(Item.Item.iItemNo) , sizeof(ITEMNO) );
		Packet.AddData( &m_usMyPoit , sizeof(m_usMyPoit) );
		( g_pNetworkManager->GetPacketHandler() )->PostMsg( &Packet );

		if( bAccept == false )
			m_bAcceptFirst = false;

		if( g_pCheckManager != NULL )
			g_pCheckManager->SetDBBlock( true );

	}
	else if ( iIndex == 2 )    //第二次奖品处理
	{
		CPacket	Packet( GAMBLE_CS_ACCEPT_SECOND_ITEM );
		Packet.AddUINT32( m_itemIDSecondGetted.itemID );
		( g_pNetworkManager->GetPacketHandler() )->PostMsg( &Packet );
	}

}

void SPWindowLottery::AddPrizeLDT( LOTTERY_LDT& stPrize )
{
	m_mPrize.insert( STD_MAP_LOTTERY_LDT::value_type( stPrize.m_iItemID , stPrize ) );
}

LOTTERY_LDT* SPWindowLottery::GetPrizeLDT( int iSetID )
{
	LOTTERY_LDT*					pLotteryLdt	=	NULL;
	STD_MAP_LOTTERY_LDT::iterator	mIter		=	m_mPrize.find( iSetID );

	if( mIter != m_mPrize.end() )
		pLotteryLdt	=	&((*mIter).second);

	return pLotteryLdt;
}

void SPWindowLottery::AddChatMessage(string strMsg, ITEMID iSetID)
{
	if( strMsg.empty() )
		return;

	DWORD  dwColor = 0xFFFFFFFF;

	LOTTERY_LDT*	pLotteryLdt		=	GetPrizeLDT( iSetID );
	if ( pLotteryLdt )
	{
		dwColor = GetLevelColor(GetLevelIndex(pLotteryLdt->m_iLevel));
	}

	SPWindowList * pList = static_cast<SPWindowList *>(Find(WIID_LOTTERY_CHAT_LIST, true));
	if ( pList )
	{
		LIST_ITEM	stAddListItem;

		stAddListItem.m_strText		=	strMsg;
		stAddListItem.m_stTextColor	=	dwColor;

		pList->InsertItem(&stAddListItem);
	}
}

void SPWindowLottery::AddSystemMessage(string strMsg, ITEMID iSetID)
{
	if( strMsg.empty() )
		return;

	POINT  ptScr = { 0, 0 };
	DWORD   dwColor = 0xFFFFFFFF;
	SPTexture * pTexture = NULL;

	LOTTERY_LDT*	pLotteryLdt		=	GetPrizeLDT( iSetID );
	if ( pLotteryLdt )
	{
		int iLevelIndex = GetLevelIndex(pLotteryLdt->m_iLevel);
		dwColor = GetLevelColor(iLevelIndex);
		GetLevelIcon("DATA/INTERFACE/CONCEPT/UI110.PNG", iLevelIndex, pTexture, ptScr);
	}

	SPWindowList * pList = static_cast<SPWindowList *>(Find(WIID_LOTTERY_SYSTEMMSG_LIST, true));
	if ( pList )
	{
		LIST_ITEM	stAddListItem;

		stAddListItem.m_strText		=	strMsg;
		stAddListItem.m_stTextColor	=	dwColor;

		if ( pTexture )
		{
			pTexture->IncreaseRef();

			LIST_ICON   stAddListIcon;
			stAddListIcon.m_pImage	=	pTexture;
			stAddListIcon.m_rcSrc.left		=	ptScr.x;
			stAddListIcon.m_rcSrc.top		=	ptScr.y;
			stAddListIcon.m_rcSrc.right		=	ptScr.x+16;
			stAddListIcon.m_rcSrc.bottom	=	ptScr.y+14;
			stAddListItem.m_vIconImage.push_back(stAddListIcon);
		}

		pList->InsertItem(&stAddListItem);
	}
}

void SPWindowLottery::AddScrollMessage(string strMsg, ITEMID iSetID)
{
	POINT  ptScr = { 0, 0 };
	DWORD   dwColor = 0xFFFFFFFF;
	SPTexture * pTexture = NULL;

	LOTTERY_LDT*	pLotteryLdt		=	GetPrizeLDT( iSetID );
	if ( pLotteryLdt )
	{
		int iLevelIndex = GetLevelIndex(pLotteryLdt->m_iLevel);
		dwColor = GetLevelColor(iLevelIndex);
		GetLevelIcon("DATA/INTERFACE/CONCEPT/UI110.PNG", iLevelIndex, pTexture, ptScr);
	}
	
	TEXT_NODE    text(strMsg, dwColor);
	text.LoadTexture(pTexture, ptScr.x, ptScr.y, ptScr.x+16, ptScr.y+14 );
	m_textScrollManager.AddTextNode(text);
}

//------------------------------------------------------------------------------------
void SPWindowLottery::ClearSelectItem()
{
	if( m_iSelectSlot >= 0 && m_pkSelectItem )
	{
		SPWindowGrid* pkGridWindow = static_cast< SPWindowGrid* >(Find(WIID_LOTTERY_ITEM_INVEN_SLOT, true));
		if( pkGridWindow )
			pkGridWindow->CellSelectSell(m_iSelectSlot, false);
	}

	m_iSelectSlot = -1;
	m_pkSelectItem = NULL;

	g_pInterfaceManager->ClearMouse();

}

//------------------------------------------------------------------------------------
SPItem* SPWindowLottery::GetItemInvenItem(int iSlotIndex)
{
	if( g_pGOBManager->GetLocalPlayer() == NULL )
		return NULL;

	SPPlayerInvenArchive* pkInvenArchive = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	if( pkInvenArchive == NULL )
		return NULL;

	int iSize, iStartIndex;
	iStartIndex = m_iItemInvenCurPage[m_iItemInvenCategory] * INVENTORY_LINE;
	iSize = pkInvenArchive->GetInventorySize((CONTAINER_TYPE)m_iItemInvenCategory);

	if( (iStartIndex + iSlotIndex) >= iSize )
		return NULL;

	 return pkInvenArchive->GetInventoryItem((CONTAINER_TYPE)m_iItemInvenCategory, iStartIndex + iSlotIndex);
}

//------------------------------------------------------------------------------------
void SPWindowLottery::EmptyItemInvenSlot()
{
	SPPlayerInvenArchive* pkInvenArchive = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	if( pkInvenArchive == NULL )
		return;

	if( pkInvenArchive->IsFullInventory((CONTAINER_TYPE)m_iItemInvenCategory) )
		return;

	SPItem* pkItem;
	int iSize, i;
	iSize = pkInvenArchive->GetInventorySize((CONTAINER_TYPE)m_iItemInvenCategory);
	for( i = 0; i < iSize; i++ )
	{
		if( i >= iSize )
			break;

		pkItem = pkInvenArchive->GetInventoryItem((CONTAINER_TYPE)m_iItemInvenCategory, i);
		if( pkItem == NULL )
		{
			int iIndex = (i / INVENTORY_LINE) - 3;
			if( iIndex > 0 )
				m_iItemInvenCurPage[m_iItemInvenCategory] = iIndex;
				
			return;
		}
	}
}

//------------------------------------------------------------------------------------
void SPWindowLottery::RefreshItemInven()
{
	if( g_pGOBManager->GetLocalPlayer() == NULL )
		return;
	
	SPWindowGrid* pkWindowGrid = static_cast<SPWindowGrid*>(Find(WIID_LOTTERY_ITEM_INVEN_SLOT, true));
	if( pkWindowGrid == NULL )
		return;

	pkWindowGrid->ClearCellAll();

	SPPlayerInvenArchive* pkInvenArchive = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	if( pkInvenArchive == NULL )
		return;

	int iLineSize;
	if( m_iItemInvenCategory == 4 )
	{
		int iSize = pkInvenArchive->GetInventorySize(INVENTORY_PET);
		iLineSize = iSize / INVENTORY_LINE;
		if( iLineSize > 4 )
			iLineSize = 4;	
	}
	else
		iLineSize = 4;

	pkWindowGrid->SetShowLine(iLineSize);
	
	int iSize, iStartIndex, i;
	SPItem* pkItem;

	iStartIndex = m_iItemInvenCurPage[m_iItemInvenCategory] * INVENTORY_LINE;
	iSize = pkInvenArchive->GetInventorySize((CONTAINER_TYPE)m_iItemInvenCategory);
	for( i = 0; i < VIEW_SLOT; i++ )
	{
		if( (iStartIndex + i) >= iSize )
			break;

		pkItem = pkInvenArchive->GetInventoryItem((CONTAINER_TYPE)m_iItemInvenCategory, iStartIndex + i);
		if( pkItem == NULL )
			continue;

		CONTAINER_ITEM Item;
		pkItem->ConvertContainerItem(&Item);

		bool bEnable = g_pCheckManager->CheckItemStatus(Item.Item.iItemID);	
		Item.SlotInfo.SlotIndex = Item.SlotInfo.SlotIndex - (m_iItemInvenCurPage[m_iItemInvenCategory] * INVENTORY_LINE);
		
		POINT ptSrcPos;	
		SPTexture* pkTexture = g_pItemCluster->GetItemTexture(Item.Item.iItemID, ptSrcPos); 	

		pkWindowGrid->SettingIconImage(pkTexture, Item.Item.iItemID,
			ptSrcPos.x, ptSrcPos.y, Item.SlotInfo.SlotIndex, Item.Item.iStackCount, bEnable);
	}

	int iMaxInvenPage = pkInvenArchive->GetMaxInvenPage(m_iItemInvenCategory);
	if( m_iItemInvenMaxPage[m_iItemInvenCategory] != iMaxInvenPage )
		m_iItemInvenMaxPage[m_iItemInvenCategory] = iMaxInvenPage;

	SetSliderMaxPage(1, iMaxInvenPage + 1);
	SetSliderPage(1, m_iItemInvenCurPage[m_iItemInvenCategory]);
}


//------------------------------------------------------------------------------------
int SPWindowLottery::SetSliderMaxPage(int iType, int iPage)
{
	SPWindow* pkWindow = NULL;
	switch( iType )
	{
	case 1:
		pkWindow = Find(WIID_LOTTERY_ITEM_INVEN_THUMB, true);
		break;
	case 2:
		//pkWindow = Find(WIID_CASHSHOP_OZ_INVEN_THUMB, true);
		break;
	}

	if( pkWindow == NULL )
		return 1;

	pkWindow->SPSendMessage(SPIM_SET_MAXVALUE, iPage);
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLottery::SetSliderPage(int iType, int iPage)
{
	SPWindow* pkWindow = NULL;
	switch( iType )
	{
	case 1:
		pkWindow = Find(WIID_LOTTERY_ITEM_INVEN_THUMB, true);
		break;
	case 2:
		//pkWindow = Find(WIID_CASHSHOP_OZ_INVEN_THUMB, true);
		break;
	}

	if( pkWindow == NULL )
		return 1;

	pkWindow->SPSendMessage(SPIM_SET_CURVALUE, iPage);
	return 1;
}

void SPWindowLottery::BtnPush( int iBtn, int iBtnDown )
{
	DWORD   dwCtrlID = iBtn;

	SPWindowList * pList = static_cast<SPWindowList *>(Find(dwCtrlID, true));
	if ( pList )
	{
		switch( iBtnDown )
		{
		case 1:
			pList->SPSendMessage (SPIM_SCROLL , SUB_SPIM_SCROLL_UP );
			break;
		case 2:
			pList->SPSendMessage( SPIM_SCROLL , SUB_SPIM_SCROLL_DOWN );
			break;
		}
	}
	
}

//------------------------------------------------------------------------------------
void SPWindowLottery::SetItemInvenCategory(int iType)
{
	SPWindow* pkWindow, *pkChildWindow;

	pkWindow = Find(WIID_LOTTERY_ITEM_INVEN, true);
	if( pkWindow == NULL )
		return;

	int i = WIID_LOTTERY_ITEM_INVEN_EQUIP;
	for( ; i <= WIID_LOTTERY_ITEM_INVEN_HOUSING; i++ )
	{
		BOOL bCheck = FALSE;
		if( iType == (i - WIID_LOTTERY_ITEM_INVEN_EQUIP) )
			bCheck = TRUE;

		pkChildWindow = pkWindow->Find(i, true);
		if( pkChildWindow )
			pkChildWindow->SPSendMessage(SPIM_BTN_SELECT, bCheck);
	}

	if( m_iItemInvenCategory != iType )
	{
		m_iItemInvenCategory = iType;
		RefreshItemInven();
	}
}

//------------------------------------------------------------------------------------
// Message
int SPWindowLottery::OnItemInvenUp(WPARAM wParam, LPARAM lParam)
{
	m_iItemInvenCurPage[m_iItemInvenCategory]--;
	if( m_iItemInvenCurPage[m_iItemInvenCategory] < 0 )
		 m_iItemInvenCurPage[m_iItemInvenCategory] = 0;
	else
		RefreshItemInven();

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLottery::OnItemInvenDown(WPARAM wParam, LPARAM lParam)
{
	m_iItemInvenCurPage[m_iItemInvenCategory]++;
	if( m_iItemInvenCurPage[m_iItemInvenCategory] > m_iItemInvenMaxPage[m_iItemInvenCategory] )
		m_iItemInvenCurPage[m_iItemInvenCategory] = m_iItemInvenMaxPage[m_iItemInvenCategory];
	else
		RefreshItemInven();

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLottery::OnSetItemInvenUp(WPARAM wParam, LPARAM lParam)
{
	SPWindow* pkWindow = Find(WIID_LOTTERY_ITEM_INVEN_THUMB, true);
	if( pkWindow )
		pkWindow->SPSendMessage(pkWindow, SPIM_WHEEL_UP, wParam, lParam);

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLottery::OnSetItemInvenDown(WPARAM wParam, LPARAM lParam)
{
	SPWindow* pkWindow = Find(WIID_LOTTERY_ITEM_INVEN_THUMB, true);
	if( pkWindow )
		pkWindow->SPSendMessage(pkWindow, SPIM_WHEEL_DOWN, wParam, lParam);

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLottery::OnSetItemInvenCurPage(WPARAM wParam, LPARAM lParam)
{
	if( m_iItemInvenCurPage[m_iItemInvenCategory] != wParam )
	{
		m_iItemInvenCurPage[m_iItemInvenCategory] = (int)wParam;
		RefreshItemInven();
	}
	
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLottery::OnInvenCategory(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	SetItemInvenCategory(iID - WIID_LOTTERY_ITEM_INVEN_EQUIP);
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLottery::OnItemInvenLButtonDown(WPARAM wParam, LPARAM lParam)
{
	m_iSelectSlot = (int)lParam;
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLottery::OnItemInvenLButtonUp(WPARAM wParam, LPARAM lParam)
{
	if( m_iSelectSlot < 0 || g_pItemCluster == NULL )
		return 1;

	int iDstSlotIndex = (int)lParam;
	if( iDstSlotIndex < 0 )
		return 1;

	CHANGE_SLOT ChangeSlot;
	if( m_pkSelectItem )
	{
		if( g_pCheckManager->IsDBBlock() )
			return 1;
		
		int iStartIndex = m_iItemInvenCurPage[m_iItemInvenCategory] * INVENTORY_LINE;
		if( (m_pkSelectItem->GetSlotIndex() != iStartIndex + iDstSlotIndex) || 
			(m_pkSelectItem->GetContainerType() != m_iItemInvenCategory) )
		{
			ChangeSlot.SrcSlotInfo.ContainerType = m_pkSelectItem->GetContainerType();
			ChangeSlot.SrcSlotInfo.SlotIndex = m_pkSelectItem->GetSlotIndex() + 1;
			ChangeSlot.iCount = m_pkSelectItem->GetItemStatusValue(MV_ITEM_GETSTACK_CNT);
			
			ChangeSlot.DestSlotInfo.ContainerType = (CONTAINER_TYPE)m_iItemInvenCategory;
			ChangeSlot.DestSlotInfo.SlotIndex = iStartIndex + iDstSlotIndex + 1;

			CPacket kPacket;
			kPacket.Add((UINT32)ITEM_CS_CHANGE_SLOT);
			kPacket.AddData(&ChangeSlot, sizeof(CHANGE_SLOT));	
			(g_pNetworkManager->GetPacketHandler())->PostMsg(&kPacket);
		}
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLottery::OnItemInvenCursorMove(WPARAM wParam, LPARAM lParam)
{
	if( m_pkSelectItem == NULL && m_iSelectSlot >= 0 )
	{
		m_pkSelectItem = GetItemInvenItem(m_iSelectSlot);
		if( m_pkSelectItem )
		{
			POINT ptSrcPos;
			SPTexture* pkTexture = g_pItemCluster->GetItemTexture( m_pkSelectItem->GetItemStatusValue(MV_ITEM_GETUID), ptSrcPos);
			g_pInterfaceManager->SetMousePickup(pkTexture, ptSrcPos.x, ptSrcPos.y);

			SPWindowGrid* pkGridWindow =
				static_cast< SPWindowGrid* >(Find(WIID_LOTTERY_ITEM_INVEN_SLOT, true));
			if( pkGridWindow )
				pkGridWindow->CellSelectSell(m_iSelectSlot, true);
		}
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLottery::OnItemInvenToolTipEnter(WPARAM wParam, LPARAM lParam)
{
	if( m_pkSelectItem )
	{
		SPItemAttr* pItemAttr = m_pkSelectItem->GetItemAttr();
		if( pItemAttr )
		{
			int iType = pItemAttr->m_eType;
			if( iType != 0 && (iType - 1) != m_iItemInvenCategory )
			{
				SetItemInvenCategory(iType - 1);
				EmptyItemInvenSlot();
				RefreshItemInven();
			}
		}
	}

	int iSlotIndex = (int)lParam;
	SPItem* pkItem = GetItemInvenItem(iSlotIndex);
	if( pkItem == NULL )
		return 1;

	SPItemStatus* pkItemStatus = pkItem->GetItemStatus();
	if( pkItemStatus == NULL )
		return 1;
	
	SPWindow* pkWindow = g_pInterfaceManager->GetToolTipWindow();
	if( pkWindow == NULL )
		return 1;

	pkWindow->SPSendMessage(SPIM_TOOLTIP_ENTER, (WPARAM)pkItemStatus, lParam);
	pkWindow->Show();

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLottery::OnItemInvenToolTipOut(WPARAM wParam, LPARAM lParam)
{
	SPWindow* pkWindow = g_pInterfaceManager->GetToolTipWindow();
	pkWindow->SPSendMessage(SPIM_TOOLTIP_OUT, NULL, lParam);
	pkWindow->Show();

	return 1;
}
//------------------------------------------------------------------------------------
int SPWindowLottery::OnItemInvenToolTipMove(WPARAM wParam, LPARAM lParam)
{
	int iSlotIndex = (int)lParam;
	SPItem* pkItem = GetItemInvenItem(iSlotIndex);
	if( pkItem == NULL )
		return 1;

	SPItemStatus* pkItemStatus = pkItem->GetItemStatus();
	if( pkItemStatus == NULL )
		return 1;

	SPWindow* pkWindow = g_pInterfaceManager->GetToolTipWindow();
	if( pkWindow == NULL )
		return 1;

	pkWindow->SPSendMessage(SPIM_TOOLTIP_MOVE, (WPARAM)pkItemStatus, lParam);
	pkWindow->Show();

	return 1;
}


//------------------------------------------------------------------------------------
int SPWindowLottery::OnTicketToolTipEnter(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	int iSlotIndex = iID - WIID_LOTTERY_TICKET_SELL;
	if ( iSlotIndex < 0 || iSlotIndex >= m_vTicketBuffer.size() )
		return 1;

	// 更新名字
	const char *  lpszName = GetTicketName(iSlotIndex);
	if ( !lpszName || strlen(lpszName) <= 0 )
	{
		CPacket  packet(GAMBLE_CS_GET_NAME);
		packet.AddUINT64(m_uiVersion);
		packet.AddUINT32(iSlotIndex);
		g_pNetworkManager->GetPacketHandler()->PostMsg( &packet );
	}

	if ( m_fEnterToolTipTime == 0.0f )
		m_fEnterToolTipTime = 0.0001f;
	if ( m_fEnterToolTipTime < m_fEnterStopTime )
		return 1;

	ITEMID   itemID = m_vTicketBuffer.at(iSlotIndex).itemID;
	if ( itemID == 0 )
		return 1;

	SPItemAttr* pkItemAttr = NULL;
	g_pItemCluster->GetItemInfo(itemID, pkItemAttr);
	if( pkItemAttr == NULL )
		return 1;

	//ITEM	ItemInfo;

	SPItemStatus kItemStatus;
	kItemStatus.SetItemID(itemID);
	kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPID,		pkItemAttr->m_ePosID1);
	kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPCLASS,	pkItemAttr->m_iCategory);
	kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPITEMID,	pkItemAttr->m_iNum);
	kItemStatus.SPSendMessage(MV_ITEM_SETCOLOR,			pkItemAttr->m_iColorID);
	kItemStatus.SPSendMessage(MV_ITEM_SET_RARELEVEL,	pkItemAttr->m_eRareLimit);
	//kItemStatus.SPSendMessage(MV_ITEM_SET_OPTION,		(LPARAM)ItemInfo.Opt );

	if(pkItemAttr->m_eDuration != ITEM_DURATION_NULL) {
		kItemStatus.SetLifeTime(pkItemAttr->m_iLifeTime * 3600);
	}

	const char *  lpszTicketName = GetTicketName(iSlotIndex);
	if ( lpszTicketName && strlen(lpszTicketName) > 0 )
	{
		const char*		pszGlobalString	=	g_pResourceManager->GetGlobalString( 13001433 );
		if( pszGlobalString != NULL )
		{
			char  szName[32];
			sprintf(szName, pszGlobalString, lpszTicketName);
			kItemStatus.SetLotteryName(szName);
		}
	}
	
	SPWindow* pkWindow = g_pInterfaceManager->GetToolTipWindow();
	if( pkWindow == NULL )
		return 1;

	pkWindow->SPSendMessage(SPIM_TOOLTIP_ENTER, (WPARAM)&kItemStatus, lParam);
	pkWindow->Show();

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLottery::OnTicketToolTipOut(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	m_fEnterToolTipTime = 0.0f;

	SPWindow* pkWindow = g_pInterfaceManager->GetToolTipWindow();
	pkWindow->SPSendMessage(SPIM_TOOLTIP_OUT, NULL, lParam);
	pkWindow->Show();

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLottery::OnTicketToolTipMove(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	if ( m_fEnterToolTipTime < m_fEnterStopTime )
		return 1;

	int iSlotIndex = iID - WIID_LOTTERY_TICKET_SELL;
	if ( iSlotIndex < 0 || iSlotIndex >= m_vTicketBuffer.size() )
		return 1;

	ITEMID   itemID = m_vTicketBuffer.at(iSlotIndex).itemID;
	if ( itemID == 0 )
		return 1;

	SPItemAttr* pkItemAttr = NULL;
	g_pItemCluster->GetItemInfo(itemID, pkItemAttr);
	if( pkItemAttr == NULL )
		return 1;

	SPItemStatus kItemStatus;
	kItemStatus.SetItemID(itemID);
	kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPID,		pkItemAttr->m_ePosID1);
	kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPCLASS,	pkItemAttr->m_iCategory);
	kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPITEMID,	pkItemAttr->m_iNum);
	kItemStatus.SPSendMessage(MV_ITEM_SETCOLOR,			pkItemAttr->m_iColorID);
	kItemStatus.SPSendMessage(MV_ITEM_SET_RARELEVEL,	pkItemAttr->m_eRareLimit);

	if(pkItemAttr->m_eDuration != ITEM_DURATION_NULL) {
		kItemStatus.SetLifeTime(pkItemAttr->m_iLifeTime * 3600);
	}

	const char *  lpszTicketName = GetTicketName(iSlotIndex);
	if ( lpszTicketName && strlen(lpszTicketName) > 0 )
	{
		const char*		pszGlobalString	=	g_pResourceManager->GetGlobalString( 13001433 );
		if( pszGlobalString != NULL )
		{
			char  szName[32];
			sprintf(szName, pszGlobalString, lpszTicketName);
			kItemStatus.SetLotteryName(szName);
		}
	}
	
	SPWindow* pkWindow = g_pInterfaceManager->GetToolTipWindow();
	if( pkWindow == NULL )
		return 1;

	pkWindow->SPSendMessage(SPIM_TOOLTIP_MOVE, (WPARAM)&kItemStatus, lParam);
	pkWindow->Show();

	return 1;
}


//------------------------------------------------------------------------------------
int SPWindowLottery::OnTurnToolTipEnter(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	int iSlotIndex = 0;
	if ( iID >= WIID_LOTTERY_TURN_DISABLE )
	{
		iSlotIndex = iID - WIID_LOTTERY_TURN_DISABLE;
	}
	else if ( iID >= WIID_LOTTERY_TURN_ITEM )
	{
		iSlotIndex = iID - WIID_LOTTERY_TURN_ITEM;
	}
	else if ( iID >= WIID_LOTTERY_TURN_ITEM_BG )
	{
		iSlotIndex = iID - WIID_LOTTERY_TURN_ITEM_BG;
	}
	else if ( iID >= WIID_LOTTERY_TURN_LIGHT )
	{
		iSlotIndex = iID - WIID_LOTTERY_TURN_LIGHT;
	}
	
	if ( iSlotIndex < 0 || iSlotIndex >= 9 )
		return 1;

	//////////////
	// 0   1   2
	// 7   4   3
	// 6   5   4
	int  iLightArray[] = { 0, 1, 2, 7, 4, 3, 6, 5, 4 };

	ITEMID   itemID = 0;
	if ( iSlotIndex == 4 )
		itemID = m_itemIDFirstGetted.itemID;
	else
		itemID = m_vecTurnItem.at(iLightArray[iSlotIndex]).itemID;
	if ( itemID == 0 )
		return 1;

	SPItemAttr* pkItemAttr = NULL;
	g_pItemCluster->GetItemInfo(itemID, pkItemAttr);
	if( pkItemAttr == NULL )
		return 1;

	SPItemStatus kItemStatus;
	kItemStatus.SetItemID(itemID);
	kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPID,		pkItemAttr->m_ePosID1);
	kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPCLASS,	pkItemAttr->m_iCategory);
	kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPITEMID,	pkItemAttr->m_iNum);
	kItemStatus.SPSendMessage(MV_ITEM_SETCOLOR,			pkItemAttr->m_iColorID);
	kItemStatus.SPSendMessage(MV_ITEM_SET_RARELEVEL,	pkItemAttr->m_eRareLimit);

	if(pkItemAttr->m_eDuration != ITEM_DURATION_NULL) {
		kItemStatus.SetLifeTime(pkItemAttr->m_iLifeTime * 3600);
	}
	
	SPWindow* pkWindow = g_pInterfaceManager->GetToolTipWindow();
	if( pkWindow == NULL )
		return 1;

	pkWindow->SPSendMessage(SPIM_TOOLTIP_ENTER, (WPARAM)&kItemStatus, lParam);
	pkWindow->Show();

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLottery::OnTurnToolTipOut(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	SPWindow* pkWindow = g_pInterfaceManager->GetToolTipWindow();
	pkWindow->SPSendMessage(SPIM_TOOLTIP_OUT, NULL, lParam);
	pkWindow->Show();

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLottery::OnTurnToolTipMove(unsigned int iID, WPARAM wParam, LPARAM lParam)
{
	int iSlotIndex = 0;
	if ( iID >= WIID_LOTTERY_TURN_DISABLE )
	{
		iSlotIndex = iID - WIID_LOTTERY_TURN_DISABLE;
	}
	else if ( iID >= WIID_LOTTERY_TURN_ITEM )
	{
		iSlotIndex = iID - WIID_LOTTERY_TURN_ITEM;
	}
	else if ( iID >= WIID_LOTTERY_TURN_ITEM_BG )
	{
		iSlotIndex = iID - WIID_LOTTERY_TURN_ITEM_BG;
	}
	else if ( iID >= WIID_LOTTERY_TURN_LIGHT )
	{
		iSlotIndex = iID - WIID_LOTTERY_TURN_LIGHT;
	}

	if ( iSlotIndex < 0 || iSlotIndex >= 9 )
		return 1;

	//////////////
	// 0   1   2
	// 7   4   3
	// 6   5   4
	int  iLightArray[] = { 0, 1, 2, 7, 4, 3, 6, 5, 4 };

	ITEMID   itemID = 0;
	if ( iSlotIndex == 4 )
		itemID = m_itemIDFirstGetted.itemID;
	else
		itemID = m_vecTurnItem.at(iLightArray[iSlotIndex]).itemID;
	if ( itemID == 0 )
		return 1;

	SPItemAttr* pkItemAttr = NULL;
	g_pItemCluster->GetItemInfo(itemID, pkItemAttr);
	if( pkItemAttr == NULL )
		return 1;

	SPItemStatus kItemStatus;
	kItemStatus.SetItemID(itemID);
	kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPID,		pkItemAttr->m_ePosID1);
	kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPCLASS,	pkItemAttr->m_iCategory);
	kItemStatus.SPSendMessage(MV_ITEM_SETEQUIPITEMID,	pkItemAttr->m_iNum);
	kItemStatus.SPSendMessage(MV_ITEM_SETCOLOR,			pkItemAttr->m_iColorID);
	kItemStatus.SPSendMessage(MV_ITEM_SET_RARELEVEL,	pkItemAttr->m_eRareLimit);

	if(pkItemAttr->m_eDuration != ITEM_DURATION_NULL) {
		kItemStatus.SetLifeTime(pkItemAttr->m_iLifeTime * 3600);
	}
	
	SPWindow* pkWindow = g_pInterfaceManager->GetToolTipWindow();
	if( pkWindow == NULL )
		return 1;

	pkWindow->SPSendMessage(SPIM_TOOLTIP_MOVE, (WPARAM)&kItemStatus, lParam);
	pkWindow->Show();

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLottery::OnReflashItemInven(WPARAM wParam, LPARAM lParam)
{
	RefreshItemInven();
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLottery::OnClearDrag(WPARAM wParam, LPARAM lParam)
{
	ClearSelectItem();
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLottery::OnChatScrollUp( WPARAM wParam, LPARAM lParam)
{
	SPWindowList * pList = static_cast<SPWindowList *>(Find(WIID_LOTTERY_CHAT_LIST, true));
	if ( pList )
	{
		pList->SPSendMessage (SPIM_SCROLL , SUB_SPIM_SCROLL_UP );
	}
	m_iChatBtnDown = 0;

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLottery::OnChatScrollDown( WPARAM wParam, LPARAM lParam)
{
	SPWindowList * pList = static_cast<SPWindowList *>(Find(WIID_LOTTERY_CHAT_LIST, true));
	if ( pList )
	{
		pList->SPSendMessage( SPIM_SCROLL , SUB_SPIM_SCROLL_DOWN );
	}
	m_iChatBtnDown = 0;

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLottery::OnChatScrollLatest( WPARAM wParam, LPARAM lParam)
{
	SPWindowList * pList = static_cast<SPWindowList *>(Find(WIID_LOTTERY_CHAT_LIST, true));
	if ( pList )
	{
		pList->SPSendMessage( SPIM_SCROLL , SUB_SPIM_SCROLL_LATEST );
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLottery::OnSystemMsgScrollUp( WPARAM wParam, LPARAM lParam)
{
	SPWindowList * pList = static_cast<SPWindowList *>(Find(WIID_LOTTERY_SYSTEMMSG_LIST, true));
	if ( pList )
	{
		pList->SPSendMessage (SPIM_SCROLL , SUB_SPIM_SCROLL_UP );
	}
	m_iSystemMsgBtnDown = 0;

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLottery::OnSystemMsgScrollDown( WPARAM wParam, LPARAM lParam)
{
	SPWindowList * pList = static_cast<SPWindowList *>(Find(WIID_LOTTERY_SYSTEMMSG_LIST, true));
	if ( pList )
	{
		pList->SPSendMessage( SPIM_SCROLL , SUB_SPIM_SCROLL_DOWN );
	}
	m_iSystemMsgBtnDown = 0;

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLottery::OnSystemMsgScrollLatest( WPARAM wParam, LPARAM lParam)
{
	SPWindowList * pList = static_cast<SPWindowList *>(Find(WIID_LOTTERY_SYSTEMMSG_LIST, true));
	if ( pList )
	{
		pList->SPSendMessage( SPIM_SCROLL , SUB_SPIM_SCROLL_LATEST );
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLottery::OnChatWheelUp( WPARAM wParam, LPARAM lParam )
{
	OnChatScrollUp( NULL , NULL );

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLottery::OnChatWheelDown( WPARAM wParam, LPARAM lParam )
{
	OnChatScrollDown( NULL , NULL );

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLottery::OnSystemMsgWheelUp( WPARAM wParam, LPARAM lParam )
{
	OnSystemMsgScrollUp( NULL , NULL );

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLottery::OnSystemMsgWheelDown( WPARAM wParam, LPARAM lParam )
{
	OnSystemMsgScrollDown( NULL , NULL );

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLottery::OnLoadGambleTip( WPARAM wParam, LPARAM lParam )
{
	LoadGambleTipSetting();
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLottery::OnSaveGambleTip( WPARAM wParam, LPARAM lParam )
{
	SaveGambleTipSetting( m_bShowGambleTip );
	return 1;
}


//------------------------------------------------------------------------------------
int SPWindowLottery::OnTurnTipCheck( WPARAM wParam, LPARAM lParam )
{
	SPWindow * pkParent = Find(WIID_LOTTERY_TURN_TIP, true);
	if ( !pkParent )
		return 1;

	SPWindowButton * pkCheck = (SPWindowButton *)pkParent->Find(WIID_LOTTERY_TURN_TIP_CHECK);
	if ( pkCheck )
	{
		m_bShowGambleTip = !m_bShowGambleTip;
		pkCheck->SPSendMessage( SPIM_BTN_SELECT, !m_bShowGambleTip );
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLottery::OnTurnTipClose( WPARAM wParam, LPARAM lParam )
{
	HideTurnTip();
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLottery::OnChatScrollUpLBDown( WPARAM wParam, LPARAM lParam )
{
	m_iChatBtnDown	=	1;

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLottery::OnChatScrollDownLBDown( WPARAM wParam, LPARAM lParam )
{
	m_iChatBtnDown	=	2;

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLottery::OnChatScrollUpCursorOut( WPARAM wParam, LPARAM lParam )
{
	m_iChatBtnDown	=	0;

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLottery::OnChatScrollDownCursorOut( WPARAM wParam, LPARAM lParam )
{
	m_iChatBtnDown	=	0;

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLottery::OnSystemMsgScrollUpLBDown( WPARAM wParam, LPARAM lParam )
{
	m_iSystemMsgBtnDown	=	1;

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLottery::OnSystemMsgScrollDownLBDown( WPARAM wParam, LPARAM lParam )
{
	m_iSystemMsgBtnDown	=	2;

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLottery::OnSystemMsgScrollUpCursorOut( WPARAM wParam, LPARAM lParam )
{
	m_iSystemMsgBtnDown	=	0;

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowLottery::OnSystemMsgScrollDownCursorOut( WPARAM wParam, LPARAM lParam )
{
	m_iSystemMsgBtnDown	=	0;

	return 1;
}
