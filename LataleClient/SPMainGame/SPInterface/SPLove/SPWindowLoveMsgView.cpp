// ***************************************************************
//  SPWindowLoveMsgView		Version:  1.0		Date: 2008/11/03
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "SPCommon.h"

#include "SPUtil.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowStatic.h"

#include "SPResourceDef.h"
#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"
#include "SPLocalizeManager.h"

#include "SPMouseCursor.h"
#include "SPIMEPool.h"
#include "SPInterfaceManager.h"

#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"
#include "SockWnd.h"
#include "SPNetworkManager.h"

#include "SPLocalizeManager.h"

#include "SPCheckManager.h"
#include "GlobalDefines_Share.h"

#include "SPAbuseFilter.h"

#include "SPLoveManager.h"

#ifdef _DEBUG
#include "SPDebug.h"
#endif

#include "SPWindowLoveMsgView.h"

SPWindowLoveMsgView::SPWindowLoveMsgView( WND_ID WndClassID , INSTANCE_ID InstanceID )
: SPWindow( WndClassID , InstanceID )
{	
	Init();	
}

SPWindowLoveMsgView::SPWindowLoveMsgView( INSTANCE_ID InstanceID , int iX , int iY , int iCX , int iCY , SPWindow* pParent /* = NULL  */ )
: SPWindow( WNDID_LOVEMSG_VIEW , InstanceID , iX , iY , iCX , iCY , pParent )
{	
	Init();	
}

SPWindowLoveMsgView::~SPWindowLoveMsgView()
{
	Clean();
}

void SPWindowLoveMsgView::Init()
{
	m_strToolTip	= "[SPWindowLoveMsgView]";

	//////////////////////////////////////////////////////////////////////////

	SPWindowStatic*			pWindowStatic		=	NULL;

	//////////////////////////////////////////////////////////////////////////

	m_bUse			=	SPLocalizeManager::GetInstance()->IsEnableValue( LV_LOVE );

	//////////////////////////////////////////////////////////////////////////

	SetImage( "DATA/INTERFACE/CONCEPT/UI_WinBG_001.PNG" , 186 , 215 );

	pWindowStatic	=	new SPWindowStatic( WIID_LOVEMSG_VIEW_CHAR_NAME , 33 , 42 , 143 , 12 , this );
	pWindowStatic->SetFontColor( RGBA( 143 , 60 , 60 , 255 ) );
	pWindowStatic->SetFormat( DT_CENTER | DT_VCENTER );
	pWindowStatic->SetFont( FONT_12_BOLD );
	pWindowStatic->SetWindowText( "테스트" );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_LOVEMSG_VIEW_MSG , 31 , 58 , 145 , 28 , this );
	pWindowStatic->SetFontColor( RGBA( 143 , 60 , 60 , 255 ) );
	pWindowStatic->SetFormat( DT_CENTER | DT_VCENTER );
	pWindowStatic->SetFont( FONT_12_NORMAL );
	pWindowStatic->SetWindowText( "가나다라마바사아자차카타파하가나다라마바사아자차카타파하" );
	pWindowStatic->SetMultiLine( true , 4 );
	pWindowStatic	=	NULL;

	m_fReMainTime	=	0.0f;
}

void SPWindowLoveMsgView::Clean()
{
	SPWindow::Clean();
}

void SPWindowLoveMsgView::Process(float fTime)
{
	if( !m_bShow )	return;

	SPWindow::Process( fTime );

	if( m_fReMainTime >= 0.0f )
		m_fReMainTime	-=	fTime;

	if( m_fReMainTime <= 0.0f )
	{
		m_fReMainTime	=	0.0f;
		TimeAction();
	}
}

//void SPWindowLoveMsgView::Render(float fTime)
//{
//	if( !m_bShow )	return;
//
//	g_pVideo->Flush();
//
//	SPWindow::Render( fTime );
//
//	g_pVideo->Flush();
//}

void SPWindowLoveMsgView::Show(bool bWithChild)
{
	return;

	//if( m_bUse == false )
	//	return;

	//SPWindow::Show( bWithChild );

	//m_fReMainTime	=	10.0f;
}

void SPWindowLoveMsgView::Hide(bool bSendServer /*= true*/)
{
	if( m_fReMainTime <= 0.0f )
		SPWindow::Hide( bSendServer );
}

SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowLoveMsgView )

	//	Common
	SPIMESSAGE_COMMAND(	SPIM_ADD_ITEM														,	OnAddItem			)

SPIMESSAGE_OBJ_MAP_END(	SPWindow	)

int SPWindowLoveMsgView::OnAddItem( WPARAM wParam, LPARAM lParam )
{
	if( m_bUse == false )
		return 1;

	LOVEMSG_VIEW_DATA*	pstLoveMsgVIewData	=	(LOVEMSG_VIEW_DATA*)lParam;

	if( pstLoveMsgVIewData == NULL )
		return 1;

	SPWindow*	pWindow	=	NULL;

	pWindow	=	Find( WIID_LOVEMSG_VIEW_CHAR_NAME , true );

	if( pWindow != NULL )
		pWindow->SetWindowText( pstLoveMsgVIewData->m_strCharID.c_str() );

	pWindow	=	Find( WIID_LOVEMSG_VIEW_MSG , true );

	if( pWindow != NULL )
		pWindow->SetWindowText( pstLoveMsgVIewData->m_strMsg.c_str() );

	SPWindow::Show();

	m_fReMainTime	=	3.0f;
	
	return 1;
}

void SPWindowLoveMsgView::TimeAction()
{
	SPWindow::Hide();
}
