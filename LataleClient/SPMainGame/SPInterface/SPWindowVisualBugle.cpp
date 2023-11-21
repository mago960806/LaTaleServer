#include "SPCommon.h"
#include "SPUtil.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPWindowEdit.h"
#include "SPWindowEditMultiLine.h"
#include "SPWindowSlider.h"
#include "SPWindowGrid.h"
#include "SPWindowGridEx.h"

#include "SPUIUnit.h"
#include "SPUIChatMacro.h"
#include "SPUIUnitManager.h"
#include "SPDragnDropManager.h"

#include "SPMouseCursor.h"
#include "SPIMEPool.h"
#include "SPAbuseFilter.h"
#include "SPNoticeBox.h"
#include "SPInterfaceManager.h"

#include "SPResourceDef.h"
#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"

#include "SPItemAttr.h"
#include "SPManager.h"
#include "SPItem.h"
#include "SPItemCluster.h"

#include "SPLocalizeManager.h"

#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPWindowToolTip.h"

#include "SPWindowVisualBugle.h"
#include "SPImageProcessor.h"
#include "SPWindowPresentation.h"
#include "SPWindowChat.h"
#include "SPGameObject.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPCommandConvert.h"
#include "SPPlayer.h"
#include "SPImageProcessor.h"
#include "SPWindowPresentation.h"

#include <algorithm>

#include <SPXTrace.h>
//--------------------------------------------------
const float VB_FADETIME = 0.4f;
//--------------------------------------------------
UINT64 VisualBugleInfo::iCount = 0;
void VisualBugleInfo::reset( void )
{
	m_strMsg.clear();
	m_bPreview = false;
	m_bShowed = true;
	m_iIndex = 0;
}
//--------------------------------------------------
SPWindowVisualBugle::SPWindowVisualBugle(WND_ID wndClassID, INSTANCE_ID instanceID)
: SPWindow( wndClassID, instanceID )
, m_pPlayer( NULL)
, m_strMsg( "" )
, m_bPreview( false )
, m_pImgBG( NULL )
, m_pkImageProcessor( NULL )
, m_status( VB_WAIT )
, m_pStrMsg( NULL )
{
	Init();
}
SPWindowVisualBugle::SPWindowVisualBugle(INSTANCE_ID instanceID, 
										 int iX, int iY, int iCX, int iCY, 
										 SPWindow *pParent /* = NULL  */)
										 : SPWindow( WNDID_BUGLE_SELECT, instanceID, iX, iY, iCX, iCY, pParent )
										 , m_pPlayer( NULL)
										 , m_strMsg( "" )
										 , m_bPreview( false )
										 , m_pImgBG( NULL )
										 , m_pkImageProcessor( NULL )
										 , m_status( VB_WAIT )
										 , m_pStrMsg( NULL )
{
	Init();
}
//--------------------------------------------------
SPWindowVisualBugle::~SPWindowVisualBugle( void )
{
	Clean();
}
//--------------------------------------------------
void SPWindowVisualBugle::Init( void )
{
	m_strToolTip = "[SPWindowVisualBugle]";

	m_bugleInfo.reset();

	m_fBGMovieTime = 0.3f;
	m_fAvatarDelay = VB_FADETIME;
	m_fMsgDelay = 0.4f;

	m_pPlayer = new SPPlayer;
	m_pPlayer->SetLobbyAvatar(LOBBY_INFO);
	m_pPlayer->Init();
	m_pPlayer->SetShowValue(false, false, false);
	// Charactor
	m_pPlayer->SetPlayerClass(CLASS_A);
	m_pPlayer->SetAction(ACTION_NULL, true);
	m_pPlayer->SetPos(770.0f, 296.0f);

	if (false == _initGUI())
	{
		assert( false && "GUI init failed." );
	}
}
//--------------------------------------------------
void SPWindowVisualBugle::Clean( void )
{
	if (m_pPlayer)
	{
		m_pPlayer = NULL;
	}
	SPWindow::Clean();
}
//--------------------------------------------------
void SPWindowVisualBugle::Show(bool bWithChild /* = true  */)
{
	SPWindow::Show( bWithChild );
}
void SPWindowVisualBugle::Hide(bool bSendServer /* = true  */)
{
	if (m_curBugle.m_bPreview)
	{
		if (!m_vecBugleQueue.empty())
		{
			for_each( m_vecBugleQueue.begin(),
				m_vecBugleQueue.end(),
				mem_fun_ref( &VisualBugleInfo::reset ) );
		}
		m_curBugle.reset();
		m_status = VB_WAIT;
	}

	SPWindow::Hide();
}
//--------------------------------------------------
void SPWindowVisualBugle::Process(float fDeltaTime)
{
	if (!m_vecBugleQueue.empty() && !m_bShow)
		Show();
	else if (m_vecBugleQueue.empty() && m_bShow)
		Hide();

	if (!m_bShow)
		return;	

	showBugle();
	m_fAccuTime += fDeltaTime;


	if (m_pkImageProcessor)
		m_pkImageProcessor->Process( fDeltaTime );

	if (m_curBugle.m_pPlayer)
		m_curBugle.m_pPlayer->Process( fDeltaTime );

	SPWindow::Process( fDeltaTime );
}
void SPWindowVisualBugle::Render(float fDeltaTime)
{	
	if (!m_bShow)
		return;

	if (m_pkImageProcessor)
		m_pkImageProcessor->Render();

	if (!m_curBugle.m_bPreview)
	{/* 正式的喇叭 */
		if (m_fAccuTime > VB_FADETIME && VB_SHOW == m_status)
		{
			if (m_curBugle.m_pPlayer)
				m_curBugle.m_pPlayer->Render(fDeltaTime);
			m_pStrMsg->Show();
		}
		else
		{
			m_pStrMsg->Hide();
		}

		if (m_fAccuTime > m_fShowTime + VB_FADETIME)
		{
			m_curBugle.m_bShowed = true;
			m_pStrMsg->Hide();
			m_status = VB_WAIT;
		}
		else if (m_fAccuTime > m_fShowTime && m_status != VB_FADEOUT)
		{
			m_status = VB_PREFADEOUT;
			m_pStrMsg->Hide();
		}
	}
	else
	{/* 预览的喇叭 */
		if (m_fAccuTime > VB_FADETIME && VB_SHOW == m_status)
		{
			if (m_curBugle.m_pPlayer)
				m_curBugle.m_pPlayer->Render(fDeltaTime);
			m_pStrMsg->Show();
		}

		if (m_fAccuTime > m_fShowTime + VB_FADETIME && VB_SHOW != m_status)
		{
			m_curBugle.m_bShowed = true;
			m_pStrMsg->Hide();
			m_status = VB_WAIT;
		}

		SPWindow *wndSelect = g_pInterfaceManager->FindWindow( WIID_BUGLE_SELECT );
		if (wndSelect)
			wndSelect->Render(fDeltaTime);
	}

	SPWindow::Render( fDeltaTime );
}
void SPWindowVisualBugle::RefreshRelationCoord( void )
{
	SPWindow::RefreshRelationCoord();
}
//--------------------------------------------------
bool 
SPWindowVisualBugle::ShowVisualBugle(VisualBuglePlayerInfo &playerInfo, 
									 const std::string &strMsg, 
									 const BugleInfo &bugleInfo, 
									 bool bPreview)
{
	VisualBugleInfo visualInfo;
	visualInfo.m_vbPlayerInfo = playerInfo;
	visualInfo.m_strMsg = strMsg;
	visualInfo.m_info = bugleInfo;
	visualInfo.m_bPreview = bPreview;
	visualInfo.m_bShowed = false;

	m_vecBugleQueue.push_back( visualInfo );
	Show();
	return true;
}
bool 
SPWindowVisualBugle::ShowVisualBugleImme(SPPlayer *pPlayer, 
										 const std::string &strMsg, 
										 const BugleInfo &bugleInfo, 
										 bool bPreview)
{
	if (!m_vecBugleQueue.empty())
	{
		if (m_vecBugleQueue.front().m_iIndex == m_curBugle.m_iIndex)
			if (VB_FADEOUT != m_status)
				m_status = VB_PREFADEOUT;
	}

	if (m_vecBugleQueue.size() > 1)
	{
		for_each( m_vecBugleQueue.begin()++,
			m_vecBugleQueue.end(),
			mem_fun_ref( &VisualBugleInfo::reset ) );

		m_vecBugleQueue.resize( 1 );
	}

	VisualBugleInfo visualInfo;
	visualInfo.m_pPlayer = pPlayer;
	visualInfo.m_strMsg = bugleInfo.m_strPreviewText;
	visualInfo.m_info = bugleInfo;
	visualInfo.m_bPreview = bPreview;
	visualInfo.m_bShowed = false;
	visualInfo.m_bAutoDeletePlayer = false;	// preview mode, don't delete player automatic.

	m_vecBugleQueue.push_back( visualInfo );
	return true;
}
bool
SPWindowVisualBugle::UnshowImme( void )
{
	if (!m_vecBugleQueue.empty())
	{
		if (m_vecBugleQueue.front().m_iIndex == m_curBugle.m_iIndex)
			if (VB_FADEOUT != m_status)
				m_status = VB_PREFADEOUT;
	}
	
	return true;
}
//--------------------------------------------------
bool
SPWindowVisualBugle::showBugle( void )
{
	if (VB_PREFADEOUT == m_status)
	{
		float fStrPosX, fStrPosY, fDstPosX, fDstPosY;
		RECT rcSrc;

		if (!m_curBugle.m_bPreview)
		{/* 正式的喇叭 */
			fStrPosX = 800.0f - m_curBugle.m_info.m_iVisualSizeX;
			fStrPosY = 114.0f;
			fDstPosX = 800.0f;
			fDstPosY = 114.0f;
		}
		else
		{/* 预览的喇叭 */
			fStrPosX = 277.0f;		
			fStrPosY = 388.0f;
			fDstPosX = 277.0f - 50.0f;
			fDstPosY = 388.0f;
		}

		SetRect(&rcSrc, 
			m_curBugle.m_info.m_iVisualX,
			m_curBugle.m_info.m_iVisualY,
			m_curBugle.m_info.m_iVisualSizeX + m_curBugle.m_info.m_iVisualX,
			m_curBugle.m_info.m_iVisualSizeY + m_curBugle.m_info.m_iVisualY );
		if (m_pkImageProcessor)
		{
			SAFE_DELETE( m_pkImageProcessor );
		}

		m_pkImageProcessor = new SPWindowPresentation( WIID_VISUAL_BUGLE_BG,
			m_curBugle.m_info.m_strVisualFilename.c_str(),
			rcSrc, fStrPosX, fStrPosY, fDstPosX, fDstPosY, VB_FADETIME, false);
		m_status = VB_FADEOUT;
		m_pStrMsg->Hide();
		if (m_curBugle.m_bPreview)
		{
			m_fShowTime = m_fAccuTime;
		}
	}

	if ((m_fAccuTime > 6.0f || m_curBugle.m_bShowed) && !m_vecBugleQueue.empty())
	{
		if (m_vecBugleQueue.front().m_iIndex == m_curBugle.m_iIndex)
			m_vecBugleQueue.erase( m_vecBugleQueue.begin() );

		m_curBugle.reset();
		m_pStrMsg->SetWindowText( "" );
		m_status = VB_WAIT;
		if (m_pkImageProcessor)
			SAFE_DELETE( m_pkImageProcessor );


		if (!m_vecBugleQueue.empty())
		{
			m_curBugle = *m_vecBugleQueue.begin();
			float fStrPosX, fStrPosY, fDstPosX, fDstPosY;
			RECT rcSrc;

			if (!m_curBugle.m_bPreview)
			{/* 正式的喇叭 */
				fStrPosX = 800.0f;		
				fStrPosY = 114.0f;
				fDstPosX = fStrPosX - m_curBugle.m_info.m_iVisualSizeX;
				fDstPosY = 114.0f;

				//--------------------------------------------------
				// local bugle avatar setting
				m_curBugle.m_pPlayer = m_pPlayer;
				m_curBugle.m_pPlayer->InitFigureInfo();
				for( int idx = 0; idx < m_curBugle.m_vbPlayerInfo.usItemNum; idx++ )
				{
					FIGURE_ITEM &ItemInfo = m_curBugle.m_vbPlayerInfo.vItems[idx];
					m_pPlayer->ItemSetting( ItemInfo );
				}
				//--------------------------------------------------
			}
			else
			{/* 预览的喇叭 */
				fStrPosX = 277.0f - 50.0f;		
				fStrPosY = 388.0f;
				fDstPosX = 277.0f;
				fDstPosY = 388.0f;
			}

			SetRect(&rcSrc, 
				m_curBugle.m_info.m_iVisualX,
				m_curBugle.m_info.m_iVisualY,
				m_curBugle.m_info.m_iVisualSizeX + m_curBugle.m_info.m_iVisualX,
				m_curBugle.m_info.m_iVisualSizeY + m_curBugle.m_info.m_iVisualY );
			if (m_pkImageProcessor)
			{
				SAFE_DELETE( m_pkImageProcessor );
			}

			m_pkImageProcessor = new SPWindowPresentation( WIID_VISUAL_BUGLE_BG,
				m_curBugle.m_info.m_strVisualFilename.c_str(),
				rcSrc, fStrPosX, fStrPosY, fDstPosX, fDstPosY, VB_FADETIME);

			m_pStrMsg->AdjustCoord( m_curBugle.m_info.m_iVisualTextX,
				m_curBugle.m_info.m_iVisualTextY,
				m_curBugle.m_info.m_iVisualTextSizeX,
				m_curBugle.m_info.m_iVisualTextSizeY );
			RefreshChildPos();
			m_pStrMsg->SetWindowText( m_curBugle.m_strMsg.c_str() );
			D3DXCOLOR color = 
				D3DCOLOR_ARGB( 255,	
				m_curBugle.m_info.m_iVisualColorR, 
				m_curBugle.m_info.m_iVisualColorG, 
				m_curBugle.m_info.m_iVisualColorB );
			m_pStrMsg->SetFontColor( color );
			m_pStrMsg->SetFormat(DT_TOP | DT_LEFT);

			m_fShowTime = 3.0f + VB_FADETIME;
			m_fAccuTime = 0.0f;
			m_status = VB_SHOW;
			Show();
			m_pStrMsg->Hide();
		}
		else
		{
			Hide();
		}
	}
	return true;
}
//--------------------------------------------------
bool
SPWindowVisualBugle::_initGUI( void )
{
	m_pStrMsg = new SPWindowStatic( WIID_VISUAL_BUGLE_MSG,
			45, 97, 103, 71, this );
	m_pStrMsg->SetMultiLine( true ,1 );
	//m_pStrMsg->SetMoveable( true );
	return true;
}
//--------------------------------------------------
bool
SPWindowVisualBugle::IsQueueReadyForNew( void )
{
	if (m_vecBugleQueue.empty()
		||
		(1 == (int)m_vecBugleQueue.size() && VB_FADEOUT == m_status))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//--------------------------------------------------
// end of file
