

#include <vector>
#include <string>

#include "SPCommon.h"
#include "SPUtil.h"

#include "SPMainGameDEF.H"
#include "SPSubGameManager.h"
#include "SPMainGameManager.h"
#include "SPGameObjectDEF.h"

#include "SPGameObject.h"
#include "SPManager.h"
#include "SPMouseCursor.h"
#include "SPIMEPool.h"

#include "SPWindowDEF.h"
#include "SPSubGameManager.h"
#include "SPInterfaceManager.h"

#include "SPWindow.h"
#include "SPWindowEdit.h"
#include "SPWindowButton.h"
#include "SPWindowStatic.h"
#include "SPWindowLoginSoftKey.h"

#include <shellapi.h>
#include "SPDebug.h"

#include "SPLocalizeManager.h"
#include "GlobalDefines_Share.h"

#include "_Error_log.h"



SPWindowVKeyboard::SPWindowVKeyboard(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
{	
	Init();
}

SPWindowVKeyboard::SPWindowVKeyboard(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_LOGIN_SOFTKEY, InstanceID, iX, iY, iCX, iCY, pParent)	
{	
	Init();
}

SPWindowVKeyboard::~SPWindowVKeyboard()
{
	Clean();
}

void SPWindowVKeyboard::Init()
{
	m_RGBA = RGBA(0xFF,0xFF,0xFF,0xFF);
	m_strToolTip = "[SPWindowSoftKeyboard]";
	m_bChangeFont	= false ;
	m_bCaps			= false ;
	m_bShift		= false ;
	m_bIsUpper		= false ;

	int i ;

	//			"1", "2", "3", "4", "5", "6", "7", "8", "9", "0",
	//			"q", "w", "e", "r", "t", "y", "u", "i", "o", "p",
	//			"a", "s", "d", "f", "g", "h", "j", "k", "l",
	//			"z", "x", "c", "v", "b", "n", "m"
	//			"Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P",
	//			"A", "S", "D", "F", "G", "H", "J", "K", "L",
	//			"Z", "X", "C", "V", "B", "N", "M"
	strcpy( m_strKey, "1234567890qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM") ;


	//////////////////////////////////////////////////////////////////////////
	// Window Initialize
	SPWindowStatic* pkUpBGStatic;
	SPWindowStatic* pkLoBGStatic;
	SPWindowButton* pButton;


	//////////////////////////////////////////////////////////////////////////
	// BackGround
	pkLoBGStatic = new SPWindowStatic( WIID_LOGIN_SOFTKEY_LOWERBG, 0, 0, 254, 97, this);
	pkLoBGStatic->SetImage("DATA/LOGGIN/SoftKey.PNG", 0, 0);

	pkUpBGStatic = new SPWindowStatic( WIID_LOGIN_SOFTKEY_UPPERBG, 0, 0, 254, 97, this);
	pkUpBGStatic->SetImage("DATA/LOGGIN/SoftKey.PNG", 0, 98);

	//////////////////////////////////////////////////////////////////////////
	// Number Key 1 ~ 0
	for( i = 0 ; i < 10; i++)
	{
		pButton = new SPWindowButton( WIID_LOGIN_SOFTKEY_NUM+i*2, 24+i*19, 15, 18, 18, pkLoBGStatic);
		pButton->SetImagePush("DATA/LOGGIN/SoftKey.PNG", 255+i*19, 76);
		pButton = NULL;

		pButton = new SPWindowButton( WIID_LOGIN_SOFTKEY_NUM+i*2+1, 24+i*19, 15, 18, 18, pkUpBGStatic);
		pButton->SetImagePush("DATA/LOGGIN/SoftKey.PNG", 255+i*19, 76);
		pButton = NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	// Key Q ~ P
	for( i = 0 ; i < 10; i++)
	{
		pButton = new SPWindowButton( WIID_LOGIN_SOFTKEY_LOWERKEY+i, 30+i*19, 34, 18, 18, pkLoBGStatic);
		pButton->SetImagePush("DATA/LOGGIN/SoftKey.PNG", 255+i*19, 19);
		pButton = NULL;

		pButton = new SPWindowButton( WIID_LOGIN_SOFTKEY_UPPERKEY+i, 30+i*19, 34, 18, 18, pkUpBGStatic);
		pButton->SetImagePush("DATA/LOGGIN/SoftKey.PNG", 255+i*19, 95);
		pButton = NULL;
	}

	// Key A ~ L
	for( i = 0 ; i < 9; i++)
	{
		pButton = new SPWindowButton( WIID_LOGIN_SOFTKEY_LOWERKEY+i+10, 36+i*19, 53, 18, 18, pkLoBGStatic);
		pButton->SetImagePush("DATA/LOGGIN/SoftKey.PNG", 255+i*19, 38);
		pButton = NULL;

		pButton = new SPWindowButton( WIID_LOGIN_SOFTKEY_UPPERKEY+i+10, 36+i*19, 53, 18, 18, pkUpBGStatic);
		pButton->SetImagePush("DATA/LOGGIN/SoftKey.PNG", 255+i*19, 114);
		pButton = NULL;
	}

	// Key Z ~ M
	for( i = 0 ; i < 7; i++)
	{
		pButton = new SPWindowButton( WIID_LOGIN_SOFTKEY_LOWERKEY+i+19, 42+i*19, 72, 18, 18, pkLoBGStatic);
		pButton->SetImagePush("DATA/LOGGIN/SoftKey.PNG", 255+i*19, 57);
		pButton = NULL;

		pButton = new SPWindowButton( WIID_LOGIN_SOFTKEY_UPPERKEY+i+19, 42+i*19, 72, 18, 18, pkUpBGStatic);
		pButton->SetImagePush("DATA/LOGGIN/SoftKey.PNG", 255+i*19, 133);
		pButton = NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	// Special Key in Lower
	pButton = new SPWindowButton( WIID_LOGIN_SOFTKEY_LSHIFT, 5, 72, 36, 18, pkLoBGStatic);
	//pButton->SetImageHit("DATA/LOGGIN/SoftKey.PNG", 31, 196);
	pButton->SetImagePush("DATA/LOGGIN/SoftKey.PNG", 31, 196);
	pButton->SetImageCheck("DATA/LOGGIN/SoftKey.PNG", 31, 196);
	pButton = NULL;
	pButton = new SPWindowButton( WIID_LOGIN_SOFTKEY_RSHIFT, 213, 72, 36, 18, pkLoBGStatic);
	//pButton->SetImageHit("DATA/LOGGIN/SoftKey.PNG", 31, 196);
	pButton->SetImagePush("DATA/LOGGIN/SoftKey.PNG", 31, 196);
	pButton->SetImageCheck("DATA/LOGGIN/SoftKey.PNG", 31, 196);
	pButton = NULL;
	pButton = new SPWindowButton( WIID_LOGIN_SOFTKEY_CAPS, 5, 53, 30, 18, pkLoBGStatic);
	//pButton->SetImageHit("DATA/LOGGIN/SoftKey.PNG", 0, 196);
	pButton->SetImagePush("DATA/LOGGIN/SoftKey.PNG", 0, 196);
	pButton->SetImageCheck("DATA/LOGGIN/SoftKey.PNG", 0, 196);
	pButton = NULL;
	pButton = new SPWindowButton( WIID_LOGIN_SOFTKEY_BACK, 214, 15, 35, 18, pkLoBGStatic);
	//pButton->SetImageHit("DATA/LOGGIN/SoftKey.PNG", 68, 196);
	pButton->SetImagePush("DATA/LOGGIN/SoftKey.PNG", 68, 196);
	pButton = NULL;
	pButton = new SPWindowButton( WIID_LOGIN_SOFTKEY_ENTER, 207, 34, 42, 37, pkLoBGStatic);
	//pButton->SetImageHit("DATA/LOGGIN/SoftKey.PNG", 0, 215);
	pButton->SetImagePush("DATA/LOGGIN/SoftKey.PNG", 0, 215);
	pButton = NULL;
	pButton = new SPWindowButton( WIID_LOGIN_SOFTKEY_CLOSE, 240, 3, 9, 9, pkLoBGStatic);
	pButton->SetImagePush("DATA/LOGGIN/SoftKey.PNG", 43, 215);
	pButton = NULL;

	// Special Key in Upper
	pButton = new SPWindowButton( WIID_LOGIN_SOFTKEY_LSHIFT+1, 5, 72, 36, 18, pkUpBGStatic);
	//pButton->SetImageHit("DATA/LOGGIN/SoftKey.PNG", 31, 196);
	pButton->SetImagePush("DATA/LOGGIN/SoftKey.PNG", 31, 196);
	pButton->SetImageCheck("DATA/LOGGIN/SoftKey.PNG", 31, 196);
	pButton = NULL;
	pButton = new SPWindowButton( WIID_LOGIN_SOFTKEY_RSHIFT+1, 213, 72, 36, 18, pkUpBGStatic);
	//pButton->SetImageHit("DATA/LOGGIN/SoftKey.PNG", 31, 196);
	pButton->SetImagePush("DATA/LOGGIN/SoftKey.PNG", 31, 196);
	pButton->SetImageCheck("DATA/LOGGIN/SoftKey.PNG", 31, 196);
	pButton = NULL;
	pButton = new SPWindowButton( WIID_LOGIN_SOFTKEY_CAPS+1, 5, 53, 30, 18, pkUpBGStatic);
	//pButton->SetImageHit("DATA/LOGGIN/SoftKey.PNG", 0, 196);
	pButton->SetImagePush("DATA/LOGGIN/SoftKey.PNG", 0, 196);
	pButton->SetImageCheck("DATA/LOGGIN/SoftKey.PNG", 0, 196);
	pButton = NULL;
	pButton = new SPWindowButton( WIID_LOGIN_SOFTKEY_BACK+1, 214, 15, 35, 18, pkUpBGStatic);
	//pButton->SetImageHit("DATA/LOGGIN/SoftKey.PNG", 68, 196);
	pButton->SetImagePush("DATA/LOGGIN/SoftKey.PNG", 68, 196);
	pButton = NULL;
	pButton = new SPWindowButton( WIID_LOGIN_SOFTKEY_ENTER+1, 207, 34, 42, 37, pkUpBGStatic);
	//pButton->SetImageHit("DATA/LOGGIN/SoftKey.PNG", 0, 215);
	pButton->SetImagePush("DATA/LOGGIN/SoftKey.PNG", 0, 215);
	pButton = NULL;
	pButton = new SPWindowButton( WIID_LOGIN_SOFTKEY_CLOSE+1, 240, 3, 9, 9, pkUpBGStatic);
	pButton->SetImagePush("DATA/LOGGIN/SoftKey.PNG", 43, 215);
	pButton = NULL;
	

}

/**
	지우기
*/
void SPWindowVKeyboard::Clean()
{
	SPWindow::Clean();
}

/**
	Update
*/
void SPWindowVKeyboard::Process(float fTime)
{
	SPWindow::Process(fTime);

	// 출력 폰트를 변경한다.
	if( m_bChangeFont )
	{
		if( m_bCaps && m_bShift ) {			// 둘다 켜지면 소문자
			m_bIsUpper = false ;
		}
		else if( m_bCaps || m_bShift ) {	// 둘중 하나면 대문자
			m_bIsUpper = true ;
		}
		else {							// 둘다 꺼지면 소문자
			m_bIsUpper = false ;
		}
		AlterUpperLiterial(m_bIsUpper) ;
		m_bChangeFont = false ;

		ErrorFLogs->Error_Log("==>> Cap[%d] Shift[%d] => Upper[%d]", m_bCaps, m_bShift, m_bIsUpper) ;
	}
}


/**
	Show
*/
void SPWindowVKeyboard::Show(bool bWithChild)
{
	if( g_pSubGameManager == NULL )
		return;

	assert(g_pSubGameManager->GetSubGameID() == SUBGAME_LOGGIN);

	m_bChangeFont	= true ;
	m_bCaps			= false ;
	m_bShift		= false ;
	m_bIsUpper		= false ;
	AlterShiftKey(m_bShift) ;
	AlterCapsKey(m_bCaps);

	SPWindow::Show(bWithChild);
}




//////////////////////////////////////////////////////////////////////////
SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowVKeyboard )
	SPIMESSAGE_COMMAND(	SPIM_CURSOR_ENTER,		OnCursorEnter	)
	SPIMESSAGE_COMMAND(	SPIM_CURSOR_OUT,		OnCursorOut		)

	SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP,		WIID_LOGIN_SOFTKEY_NUM,			WIID_LOGIN_SOFTKEY_NUM+20,			OnNumKeyButtonUp	)	
	SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP,		WIID_LOGIN_SOFTKEY_UPPERKEY,	WIID_LOGIN_SOFTKEY_UPPERKEY+26,		OnUpKeyButtonUp	)	
	SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP,		WIID_LOGIN_SOFTKEY_LOWERKEY,	WIID_LOGIN_SOFTKEY_LOWERKEY+26,		OnLoKeyButtonUp	)	

	SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP,		WIID_LOGIN_SOFTKEY_LSHIFT,	WIID_LOGIN_SOFTKEY_RSHIFT+1,	OnFuncShiftUp	)	
	SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP,		WIID_LOGIN_SOFTKEY_CAPS,	WIID_LOGIN_SOFTKEY_CAPS+1,		OnFuncCapsUp	)	
	SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP,		WIID_LOGIN_SOFTKEY_LSHIFT,	WIID_LOGIN_SOFTKEY_CLOSE,		OnFuncButtonUp	)	
SPIMESSAGE_OBJ_MAP_END	( SPWindow	)
//////////////////////////////////////////////////////////////////////////

int SPWindowVKeyboard::OnCursorEnter	( WPARAM, LPARAM )
{
	//m_RGBA = RGBA(0xFF,0xFF,0xFF,0xFF);
	return 1;
}

int SPWindowVKeyboard::OnCursorOut	( WPARAM, LPARAM )
{
	//m_RGBA = RGBA(0xFF,0xFF,0xFF,0xFF);
	return 1;
}


/**
	Key Button 선택시
	>> 1 ~ 0
*/
int SPWindowVKeyboard::OnNumKeyButtonUp	(unsigned int uiID, WPARAM, LPARAM )
{
	int iIndex = (uiID - WIID_LOGIN_SOFTKEY_NUM) / 2;
	if( iIndex < 0 || iIndex > 10 )
		return 0 ;

	// SEND COMMAND through SubGameManager to IME
	char szKey[2] ;
	char czKey ;
	strncpy(szKey, &m_strKey[iIndex], 1) ;
	szKey[1] = '\0' ;
	czKey = m_strKey[iIndex] ;

	if( g_pSubGameManager == NULL )		return 0;
	assert(g_pSubGameManager->GetSubGameID() == SUBGAME_LOGGIN);
	g_pSubGameManager->SPPerformMessage(SPGM_SOFTKEY_PRESS, czKey, 0);

	// Shift가 있었다면 이용후 변경
	if( m_bShift ){
		OnFuncShiftUp(WIID_LOGIN_SOFTKEY_LSHIFT, 0, 0 ) ;
	}

	return 1;
}


/**
	Key Button 선택시
	>> a ~ z
*/
int SPWindowVKeyboard::OnLoKeyButtonUp	(unsigned int uiID, WPARAM, LPARAM )
{
	int iIndex = uiID - WIID_LOGIN_SOFTKEY_LOWERKEY;
	if( iIndex < 0 || iIndex > 26 )
		return 0 ;

	// SEND COMMAND through SubGameManager to IME
	char szKey[2] ;
	char czKey ;
	strncpy(szKey, &m_strKey[iIndex+10], 1) ;
	szKey[1] = '\0' ;
	czKey = m_strKey[iIndex+10] ;

	if( g_pSubGameManager == NULL )		return 0;
	assert(g_pSubGameManager->GetSubGameID() == SUBGAME_LOGGIN);

	// Shift가 있었다면 변경
	if( m_bShift ){
		OnFuncShiftUp(WIID_LOGIN_SOFTKEY_LSHIFT, 0, 0 ) ;
	}

	g_pSubGameManager->SPPerformMessage(SPGM_SOFTKEY_PRESS, czKey, 0);	// 숫자+Index

	return 1;
}


/**
	Key Button 선택시
	>> A ~ Z
*/
int SPWindowVKeyboard::OnUpKeyButtonUp	(unsigned int uiID, WPARAM, LPARAM )
{
	int iIndex = uiID - WIID_LOGIN_SOFTKEY_UPPERKEY;
	if( iIndex < 0 || iIndex > 26 )
		return 0 ;
	
	// SEND COMMAND through SubGameManager to IME
	char szKey[2] ;
	char czKey ;
	strncpy(szKey, &m_strKey[iIndex+10+26], 1) ;
	szKey[1] = '\0' ;
	czKey = m_strKey[iIndex+10+26] ;

	if( g_pSubGameManager == NULL )		return 0;
	assert(g_pSubGameManager->GetSubGameID() == SUBGAME_LOGGIN);
	
	// Shift가 있었다면 변경
	if( m_bShift ){
		OnFuncShiftUp(WIID_LOGIN_SOFTKEY_LSHIFT, 0, 0 ) ;
	}

	g_pSubGameManager->SPPerformMessage(SPGM_SOFTKEY_PRESS, czKey, 0);	// 숫자+소문자 + Index

	return 1;
}


/**
	Shift Key Pressed
*/
int SPWindowVKeyboard::OnFuncShiftUp	(unsigned int uiID, WPARAM, LPARAM )
{
	m_bShift ^= true ;
	AlterShiftKey(m_bShift) ;
	g_pSubGameManager->SPPerformMessage(SPGM_SOFTKEY_PRESS, 0, 0);	// For Focus
	
	return 1 ;
}

/**
	Caps Key Pressed
*/
int SPWindowVKeyboard::OnFuncCapsUp	(unsigned int uiID, WPARAM, LPARAM )
{
	m_bCaps ^= true ;
	AlterCapsKey(m_bCaps) ;
	g_pSubGameManager->SPPerformMessage(SPGM_SOFTKEY_PRESS, 0, 0);	// For Focus
	
	return 1 ;
}


/**
	Function Button 선택시
	>> BACK, ENTER, Close
*/
int SPWindowVKeyboard::OnFuncButtonUp	(unsigned int uiID, WPARAM, LPARAM )
{
	//TODO:	Function 동작
	switch( uiID )
	{
		case WIID_LOGIN_SOFTKEY_BACK:
		case WIID_LOGIN_SOFTKEY_BACK+1:
			// IME쪽에 Back Message
			if( g_pSubGameManager == NULL )		return 0;
			assert(g_pSubGameManager->GetSubGameID() == SUBGAME_LOGGIN);
			g_pSubGameManager->SPPerformMessage(SPGM_SOFTKEY_PRESSBACK);
			break ;

		case WIID_LOGIN_SOFTKEY_ENTER:
		case WIID_LOGIN_SOFTKEY_ENTER+1:
			// IME쪽에 Enter Message
			if( g_pSubGameManager == NULL )		return 0;
			assert(g_pSubGameManager->GetSubGameID() == SUBGAME_LOGGIN);
			g_pSubGameManager->SPPerformMessage(SPGM_SOFTKEY_PRESSENTER);
			break ;

		case WIID_LOGIN_SOFTKEY_CLOSE:
		case WIID_LOGIN_SOFTKEY_CLOSE+1:
			if( g_pSubGameManager == NULL )		return 0;
			assert(g_pSubGameManager->GetSubGameID() == SUBGAME_LOGGIN);
			g_pSubGameManager->SPPerformMessage(SPGM_SOFTKEY_PRESSCLOSE);
			// 화면 닫기
			break ;
		default:
			// 여긴 들어오면 안됨.
			ErrorFLogs->Error_Log("SoftKey FunctionButton Error!!") ;
			break ;
	}

	return 1;
}

//////////////////////////////////////////////////////////////////////////


/**
	WindowText를 대문자로 바꿔준다.
*/
void SPWindowVKeyboard::AlterUpperLiterial(bool bUpper)
{
	SPWindowStatic* pkUpWindow = (SPWindowStatic*)Find(WIID_LOGIN_SOFTKEY_UPPERBG, true);
	SPWindowStatic* pkLoWindow = (SPWindowStatic*)Find(WIID_LOGIN_SOFTKEY_LOWERBG, true);
	// 배경을 대, 소문자로 구분해서 배경을 바꿔주는게 좋을듯.
	if( bUpper )
	{
		pkLoWindow->Hide() ;
		pkUpWindow->Show() ;
	}
	else
	{
		pkUpWindow->Hide() ;
		pkLoWindow->Show() ;
	}
}


/**
	Caps Key 상태 변경
*/
void SPWindowVKeyboard::AlterCapsKey(bool bToogle)
{
	m_bChangeFont = true ;

	SPWindowButton* pUpWindow = (SPWindowButton*)Find(WIID_LOGIN_SOFTKEY_CAPS+1, true);
	SPWindowButton* pLoWindow = (SPWindowButton*)Find(WIID_LOGIN_SOFTKEY_CAPS, true);
	if( pUpWindow == NULL )		return ;

	if( bToogle ) {
		pLoWindow->SPSendMessage(SPIM_BTN_SELECT, (WPARAM)TRUE);
		pUpWindow->SPSendMessage(SPIM_BTN_SELECT, (WPARAM)TRUE);
	}
	else {
		pLoWindow->SPSendMessage(SPIM_BTN_SELECT, (WPARAM)FALSE);
		pUpWindow->SPSendMessage(SPIM_BTN_SELECT, (WPARAM)FALSE);
	}
}


/**
	Shift Key 상태변경
*/
void SPWindowVKeyboard::AlterShiftKey(bool bToogle)
{
	m_bChangeFont = true ;

	SPWindowButton* pLSUWindow = (SPWindowButton*)Find(WIID_LOGIN_SOFTKEY_LSHIFT+1, true);		// Upper
	SPWindowButton* pRSUWindow = (SPWindowButton*)Find(WIID_LOGIN_SOFTKEY_RSHIFT+1, true);
	SPWindowButton* pLSLWindow = (SPWindowButton*)Find(WIID_LOGIN_SOFTKEY_LSHIFT, true);		// Lower
	SPWindowButton* pRSLWindow = (SPWindowButton*)Find(WIID_LOGIN_SOFTKEY_RSHIFT, true);
	if( pLSUWindow == NULL )		return ;

	if( bToogle ) {
		pLSUWindow->SPSendMessage(SPIM_BTN_SELECT, (WPARAM)TRUE);
		pRSUWindow->SPSendMessage(SPIM_BTN_SELECT, (WPARAM)TRUE);
		pLSLWindow->SPSendMessage(SPIM_BTN_SELECT, (WPARAM)TRUE);
		pRSLWindow->SPSendMessage(SPIM_BTN_SELECT, (WPARAM)TRUE);
	}
	else {
		pLSUWindow->SPSendMessage(SPIM_BTN_SELECT, (WPARAM)FALSE);
		pRSUWindow->SPSendMessage(SPIM_BTN_SELECT, (WPARAM)FALSE);
		pLSLWindow->SPSendMessage(SPIM_BTN_SELECT, (WPARAM)FALSE);
		pRSLWindow->SPSendMessage(SPIM_BTN_SELECT, (WPARAM)FALSE);
	}
}


void SPWindowVKeyboard::PrintLogs(const TCHAR* fmt, ...)
{
	//#ifdef _DEBUG
	//	try
	//	{
	//		char msg[512];
	//		va_list va_ptr;
	//
	//		va_start(va_ptr, fmt);
	//		vsprintf(msg, fmt, va_ptr);
	//		va_end(va_ptr);
	//
	//		Error_Files(msg) ;
	//	}catch (...) 
	//	{
	//	}
	//#endif
}