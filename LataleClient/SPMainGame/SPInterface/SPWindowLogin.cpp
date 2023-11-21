

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

#include "SPOptionDef.h"
#include "SPClientOptionArchive.h"

#include "SPWindow.h"
#include "SPWindowEdit.h"
#include "SPWindowButton.h"
#include "SPWindowStatic.h"
#include "SPWindowLogin.h"
#include "SPWindowLoginSoftKey.h"
#include "SPWindowSlider.h"
#include "SPWindowLoginIDC.h"

#include <shellapi.h>
#include "SPDebug.h"

#include "SPLocalizeManager.h"
#include "GlobalDefines_Share.h"


extern bool g_bWindowed;

#include "SPGameObject.h"
#include "SPGOBStatus.h"
#include "SPGOBClusterDef.h"
#include "SPGOBCluster.h"
#include "SPGobModelUnit.h"
#include "SPAvatarModelUnitDef.h"
#include "SPAvatarModelUnit.h"
#include "SPPlayer.h"

SPWindowLogin::SPWindowLogin(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
{	
	Init();
}

SPWindowLogin::SPWindowLogin(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_LOGIN, InstanceID, iX, iY, iCX, iCY, pParent)	
, m_pWindowEditID(NULL)
, m_pWindowEditPW(NULL)
, m_pLastFocusEdit(NULL)
, m_iShowState( LOGIN_DIALOG_NULL )
{	
	Init();
}

SPWindowLogin::~SPWindowLogin()
{
	Clean();
}

void SPWindowLogin::Init()
{
	m_RGBA = RGBA(0xFF,0xFF,0xFF,0xFF);
	m_strToolTip = "[SPWindowLogin]";
	m_bHideEnable = false;
	m_pLastFocusEdit = NULL ;
	m_iCaretStart	= 0 ;
	m_iCaretEnd		= 0 ;

	m_bSaveID = false;	
	m_bShowSoftKey = false ;
	m_strLastID = "";

	m_pLoginIDC	=	NULL;

	if( SPLocalizeManager::GetInstance()->GetSiteType() == SITE_SANDA )
	{
		//m_pLoginIDC	=	new SPWindowLoginIDC( WIID_LOGINIDC , -m_iAX , -m_iAY , 800 , 600 , this );
	}
	
	//SetRect(&m_rtLoginTitle, 0, 0, 320 , 143) ;			// 초기화: WebLogin시 Title만 출력을 위해
	//pkTitleStatic = new SPWindowStatic(WIID_LOGGIN_TITLE, 0, 0, 320, 142, this);

	SPWindowStatic* pkTitleStatic = 0;
	
	pkTitleStatic = new SPWindowStatic(WIID_LOGGIN_TITLE, 428, 114, 320, 142, this);
	pkTitleStatic->SetImage("DATA/LOGGIN/LOGIN001.PNG", 0, 0);
	pkTitleStatic->Show();
	pkTitleStatic->SetRectSize(320, 142) ;

	//SPWindowAttr*	spwndattr_Map = 0;

	/*
	if(g_UIManager->GetFlags(SPUIManager::FLAGS_SEC_UIFILE) == SPUIManager::FLAGS_LOADED)
	{
		spwndattr_Map = g_UIManager->GetWindow(WIID_LOGGIN_TITLE);

		pkTitleStatic 
			= new SPWindowStatic(WIID_LOGGIN_TITLE,
								 UI_INT(spwndattr_Map,GET_SPWINDOW_X),
								 UI_INT(spwndattr_Map,GET_SPWINDOW_Y),
								 UI_INT(spwndattr_Map,GET_SPWINDOW_CX),
								 UI_INT(spwndattr_Map,GET_SPWINDOW_CY),this);

		pkTitleStatic->SetImage( UI_STRING(spwndattr_Map,GET_SPWINDOW_STATIC_FILENAME),
								 UI_INT(spwndattr_Map,GET_SPWINDOW_STATIC_X),
			                     UI_INT(spwndattr_Map,GET_SPWINDOW_STATIC_Y));
		pkTitleStatic->Show();
		pkTitleStatic->SetRectSize(320, 142) ;
	}
	else
	{
		pkTitleStatic = new SPWindowStatic(WIID_LOGGIN_TITLE, 428, 114, 320, 142, this);
		pkTitleStatic->SetImage("DATA/LOGGIN/LOGIN001.PNG", 0, 0);
		pkTitleStatic->Show();
		pkTitleStatic->SetRectSize(320, 142) ;
		
	}
	*/

	InitLoginDialog() ;
	InitEkeyDialog() ;
	InitEkeySerialDialog() ;
}



void SPWindowLogin::InitLoginDialog()
{
	std::string strID = "";
	if(g_pClientOption) {

		g_pClientOption->LoadUserID( strID );
		if( !strID.empty() || strID.length() != 0 ) {
			m_strLastID = strID;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 실제 로그인 창
	SPWindowStatic* pkBGStatic;
	pkBGStatic = new SPWindowStatic(WIID_LOGGIN_BG, 428, 114+143, 320, 267, this);
	pkBGStatic->SetImage("DATA/LOGGIN/LOGIN001.PNG", 0, 143);
	pkBGStatic->Hide();

	SPWindowEdit* pWindow;

	pWindow = new SPWindowEdit( WIID_LOGGIN_EDIT_ID, 79, 170-143, 141, 15, pkBGStatic);
	pWindow->SetFontColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
	pWindow->SetEnableTabStop(true);
	pWindow->SetLimitText(MAX_LEN_ID_DB);	
	pWindow->SetUseIME(false);
	if(strID != "") {
		char szID[32];
		ZeroMemory(&szID, 32);
		wsprintf(szID, "%s", strID.c_str());
		pWindow->SetWindowText(szID);
	}
	pWindow->Show();
	m_pWindowEditID = pWindow ;
	pWindow = NULL ;

	pWindow = new SPWindowEdit( WIID_LOGGIN_EDIT_PW, 79, 199-143, 141, 15, pkBGStatic);
	pWindow->SetEnableTabStop(true);
	pWindow->SetFontColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
	pWindow->SetPassword( true );
	pWindow->SetLimitText(MAX_LEN_PW_DB);
	pWindow->SetUseIME(false);
	pWindow->Show();
	m_pWindowEditPW = pWindow ;
	pWindow = NULL ;


	//시작
	SPWindowButton* pButton;
	pButton = new SPWindowButton( WIID_LOGGIN_CONFIRM, 229, 168-143, 70, 49, pkBGStatic);
	pButton->Show();
	pButton->SetImageDisable("DATA/LOGGIN/LOGIN001.PNG", 321, 100);
	pButton->SetImageHit	("DATA/LOGGIN/LOGIN001.PNG", 321, 0);
	pButton->SetImagePush	("DATA/LOGGIN/LOGIN001.PNG", 321, 50);
	SetEnterFocusWindow(pButton);
	pButton = NULL;	

	//종료
	pButton = new SPWindowButton( WIID_LOGGIN_EXIT, 212, 299-143, 92, 26, pkBGStatic);
	pButton->Show();
	pButton->SetImageDisable("DATA/LOGGIN/LOGIN001.PNG", 321, 447);
	pButton->SetImageHit	("DATA/LOGGIN/LOGIN001.PNG", 321, 393);
	pButton->SetImagePush	("DATA/LOGGIN/LOGIN001.PNG", 321, 420);
	
	pButton = NULL;

	//신규가입
	pButton = new SPWindowButton( WIID_NEW_ACCOUNT, 18, 299-143, 92, 26, pkBGStatic);
	pButton->Show();
	pButton->SetImageDisable("DATA/LOGGIN/LOGIN001.PNG", 321, 366);
	pButton->SetImageHit	("DATA/LOGGIN/LOGIN001.PNG", 321, 312);
	pButton->SetImagePush	("DATA/LOGGIN/LOGIN001.PNG", 321, 339);
	pButton = NULL;
	
	//HomePage
	pButton = new SPWindowButton( WIID_HOMEPAGE, 115, 299-143, 92, 26, pkBGStatic);
	pButton->Show();
	pButton->SetImageDisable("DATA/LOGGIN/LOGIN001.PNG", 414, 366);
	pButton->SetImageHit	("DATA/LOGGIN/LOGIN001.PNG", 414, 312);
	pButton->SetImagePush	("DATA/LOGGIN/LOGIN001.PNG", 414, 339);
	pButton = NULL;

	//Password Revision
	pButton = new SPWindowButton( WIID_PASS_REVISION, 145, 230-143, 157, 26, pkBGStatic);
	pButton->Show();
	pButton->SetImageDisable("DATA/LOGGIN/LOGIN001.PNG", 321, 204);
	pButton->SetImageHit	("DATA/LOGGIN/LOGIN001.PNG", 321, 150);
	pButton->SetImagePush	("DATA/LOGGIN/LOGIN001.PNG", 321, 177);
	pButton = NULL;

	//Password Find
	pButton = new SPWindowButton( WIID_PASS_FIND, 145, 260-143, 157, 26, pkBGStatic);
	pButton->Show();
	pButton->SetImageDisable("DATA/LOGGIN/LOGIN001.PNG", 321, 285);
	pButton->SetImageHit	("DATA/LOGGIN/LOGIN001.PNG", 321, 231);
	pButton->SetImagePush	("DATA/LOGGIN/LOGIN001.PNG", 321, 258);
	pButton = NULL;

	//ID 저장
	pButton = new SPWindowButton( WIID_LOGGIN_SAVE_ID, 27, 237-143, 12, 12, pkBGStatic);	
	pButton->SetImageCheck	("DATA/LOGGIN/LOGIN001.PNG", 0, 411);
	if(strID != "") {			
		m_bSaveID = true;
		pButton->SPSendMessage(SPIM_BTN_SELECT, TRUE);
	}
	pButton->Show();
	pButton = NULL;	

	// SoftKeyboard
	if( SPLocalizeManager::GetInstance()->GetSiteType() != SITE_GAMEPOT )
	{
		pButton = new SPWindowButton( WIID_LOGGIN_SOFTKEY, 27, 267-143, 12, 12, pkBGStatic);	
		pButton->SetImageCheck	("DATA/LOGGIN/LOGIN001.PNG", 0, 411);
		m_bShowSoftKey = false;
		pButton->SPSendMessage(SPIM_BTN_SELECT, FALSE);
		pButton->Show();
		pButton = NULL;	
	}

	// IDC Select
	if( SPLocalizeManager::GetInstance()->GetSiteType() == SITE_SANDA )
	{
		pButton = new SPWindowButton( WIID_LOGGIN_IDC , 18 , 333-143 , 92 , 26 , pkBGStatic );
		pButton->Show();
		pButton->SetImageHit	("DATA/LOGGIN/LOGIN001.PNG", 414, 393);
		pButton->SetImagePush	("DATA/LOGGIN/LOGIN001.PNG", 414, 420);
		pButton->SetImageDisable("DATA/LOGGIN/LOGIN001.PNG", 414, 447);
	}
}


void SPWindowLogin::InitEkeyDialog()
{
	m_bIsEKEY = false ;

	SPWindowStatic* pkEkeyStatic;
	pkEkeyStatic = new SPWindowStatic(WIID_LOGGIN_EK_BG, 428, 114+143, 320, 267, this);
	pkEkeyStatic->SetImage("DATA/LOGGIN/LOGIN001.PNG", 0, 143);
	pkEkeyStatic->Show();

	SPWindowStatic* pkBGStatic;
	pkBGStatic = new SPWindowStatic(WIID_LOGGIN_EK_INBG, 0, 0, 320, 199, pkEkeyStatic);
	pkBGStatic->SetImage("DATA/LOGGIN/LOGIN005.PNG", 0, 0);
	pkBGStatic->Show();

	SPWindowStatic* pkStatic;
	pkStatic = new SPWindowStatic(WIID_LOGGIN_EK_CHGBG, 82, 23, 200, 19, pkBGStatic);
	pkStatic->SetImage("DATA/LOGGIN/LOGIN005.PNG", 0, 200);
	pkStatic->Show();
	pkStatic = NULL ;

	SPWindowEdit* pWindow;
	pWindow = new SPWindowEdit( WIID_LOGGIN_EK_IDKEY, 139, 51, 141, 15, pkBGStatic);
	pWindow->SetFontColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
	pWindow->SetLimitText(MAX_LEN_ID);		// Length Limit
	pWindow->SetEnableTabStop(true);
	pWindow->Show();	
	pWindow = NULL;

	pWindow = new SPWindowEdit( WIID_LOGGIN_EK_CHGKEY, 139, 25, 141, 15, pkBGStatic);
	pWindow->SetFontColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
	pWindow->SetLimitText(MAX_LEN_ID);		// Length Limit
	pWindow->SetEnableTabStop(true);
	pWindow->Show();	
	pWindow = NULL;


	//Commit
	SPWindowButton* pButton;
	pButton = new SPWindowButton( WIID_LOGIN_EK_COMMIT, 55, 143, 92, 26, pkBGStatic);
	pButton->Show();
	pButton->SetImageDisable("DATA/LOGGIN/LOGIN005.PNG", 321, 54);
	pButton->SetImageHit	("DATA/LOGGIN/LOGIN005.PNG", 321, 0);
	pButton->SetImagePush	("DATA/LOGGIN/LOGIN005.PNG", 321, 27);
	//SetEnterFocusWindow(pButton);
	pButton = NULL;	

	//Cancel
	pButton = new SPWindowButton( WIID_LOGIN_EK_CANCEL, 174, 143, 92, 26, pkBGStatic);
	pButton->Show();
	pButton->SetImageDisable("DATA/LOGGIN/LOGIN005.PNG", 321, 135);
	pButton->SetImageHit	("DATA/LOGGIN/LOGIN005.PNG", 321, 81);
	pButton->SetImagePush	("DATA/LOGGIN/LOGIN005.PNG", 321, 108);
	pButton = NULL;

}

void SPWindowLogin::InitEkeySerialDialog()
{
	SPWindowStatic* pkEkeyStatic;
	pkEkeyStatic = new SPWindowStatic(WIID_LOGGIN_SERIAL_BG, 428, 114+143, 320, 267, this);		// CopyRight 화면
	pkEkeyStatic->SetImage("DATA/LOGGIN/LOGIN001.PNG", 0, 143);
	pkEkeyStatic->Show();

	SPWindowStatic* pkBGStatic;
	pkBGStatic = new SPWindowStatic(WIID_LOGGIN_SERIAL_INBG, 0, 0, 320, 199, pkEkeyStatic);		// Real Background
	pkBGStatic->SetImage("DATA/LOGGIN/LOGIN005.PNG", 0, 248);
	pkBGStatic->Show();

	SPWindowEdit* pWindow;
	pWindow = new SPWindowEdit( WIID_LOGGIN_SERIAL_EDIT, 116, 29, 141, 15, pkBGStatic);
	pWindow->SetFontColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
	pWindow->SetEnableTabStop(true);
	pWindow->Show();	
	pWindow = NULL;

	//Commit
	SPWindowButton* pButton;
	pButton = new SPWindowButton( WIID_LOGGIN_SERIAL_COMMIT, 55, 143, 92, 26, pkBGStatic);
	pButton->Show();
	pButton->SetImageDisable("DATA/LOGGIN/LOGIN005.PNG", 321, 54);
	pButton->SetImageHit	("DATA/LOGGIN/LOGIN005.PNG", 321, 0);
	pButton->SetImagePush	("DATA/LOGGIN/LOGIN005.PNG", 321, 27);
	//SetEnterFocusWindow(pButton);
	pButton = NULL;	

	//Cancel
	pButton = new SPWindowButton( WIID_LOGGIN_SERIAL_CANCEL, 174, 143, 92, 26, pkBGStatic);
	pButton->Show();
	pButton->SetImageDisable("DATA/LOGGIN/LOGIN005.PNG", 321, 135);
	pButton->SetImageHit	("DATA/LOGGIN/LOGIN005.PNG", 321, 81);
	pButton->SetImagePush	("DATA/LOGGIN/LOGIN005.PNG", 321, 108);
	pButton = NULL;
}


void SPWindowLogin::Clean()
{
	SPWindow::Clean();
}

void SPWindowLogin::Process(float fTime)
{
	if(g_pInterfaceManager->GetFocusWindow() == NULL) {
			SPWindow* pWindow = Find(WIID_LOGGIN_EDIT_ID, true);
			if(pWindow) {
				g_pInterfaceManager->SetFocusWindow( pWindow );
				SetTabStopWindow( pWindow );
			}
	}

	if( m_bShowSoftKey )
		g_pInterfaceManager->GetSoftKeyWindow()->Process(fTime) ;

	//////////////////////////////////////////////////////////////////////////
	// GetFucus -> LastFocus Edit Window Save
	SPWindow* pkWindow = g_pInterfaceManager->GetFocusWindow() ;
	if( m_pWindowEditID == pkWindow )	{
		m_pLastFocusEdit = m_pWindowEditID ;
		g_pInterfaceManager->GetIMEPool()->GetSelW(m_iCaretStart, m_iCaretEnd) ;
	}
	if( m_pWindowEditPW == pkWindow )	{
		m_pLastFocusEdit = m_pWindowEditPW ;
		g_pInterfaceManager->GetIMEPool()->GetSelW(m_iCaretStart, m_iCaretEnd) ;
	}

	SPWindow::Process(fTime);
}

void SPWindowLogin::Render(float fTime)
{
	SPWindow::Render(fTime);

	if( m_bShowSoftKey ) {
		g_pInterfaceManager->GetSoftKeyWindow()->Render(fTime) ;
	}

	if( m_pLoginIDC != NULL )
		m_pLoginIDC->Render( fTime );
}

void SPWindowLogin::Show(bool bWithChild)
{
	if( g_pSubGameManager == NULL )
		return;

	if( g_pNetworkManager != NULL )
		g_pNetworkManager->ResetDisconnectCount();

	assert(g_pSubGameManager->GetSubGameID() == SUBGAME_LOGGIN);

	if( g_pSubGameManager->SPPerformMessage( SPGM_CHECK_WEBLOGIN ) == 0 )
	{
		SPWindow::Show(bWithChild);

		CloseEkeyDialog() ;
		CloseSerialDialog() ;
		SetEnableLoginDialog() ;
		//SetRectSize(320, 410) ;
	}
	else
	{
		SPWindow::Show(bWithChild);

		CloseLoginDialog() ;
		CloseEkeyDialog() ;
		CloseSoftKeyboard() ;
		CloseSerialDialog() ;
		//SetRectSize(320, 142) ;
	}

	if( m_bShowSoftKey == false )
	{
		CloseSoftKeyboard() ;
		g_pInterfaceManager->GetSoftKeyWindow()->Hide() ;
		m_bShowSoftKey = false ;
	}
	resetPetSkillStatus();
}


/**
	For SoftKeyboard, over-wrapping SPWindow::PerformMouseMove
	It called by InterfaceManager.
	Before call SPWindowLogin::PerformMouseMove, Check Softkeyboard!!!
*/
int SPWindowLogin::PerformMouseMove(int iX, int iY)
{
	if( m_bShowSoftKey && g_pInterfaceManager->GetSoftKeyWindow()->PerformMouseMove(iX, iY) )
	{
		return 1 ;
	}
	else
	{
		return SPWindow::PerformMouseMove(iX, iY) ;
	}
	return 0;
}


/**
	Loggin Window의 Show State
*/
LOGIN_STATE	SPWindowLogin::GetShowState()
{
	return m_iShowState ;
}
void SPWindowLogin::SetShowState(LOGIN_STATE iState)
{
	if( iState > LOGIN_DIALOG_NULL && iState < LOGIN_DIALOG_MAX )
		m_iShowState = iState ;
}

//////////////////////////////////////////////////////////////////////////
//
//	Login Dialog Sub
//
//////////////////////////////////////////////////////////////////////////

/**
	Login 창의 활성화 및 포커스
*/
bool	SPWindowLogin::SetEnableLoginDialog()
{
	SPWindow* pkWindow;
	pkWindow = Find(WIID_LOGGIN_BG);
	if( pkWindow && SPLocalizeManager::GetInstance()->OnlyWebLogin() )
	{
		// 버디에서 로그인 창이 보려지려 하면 무조건 Exit
		g_pMainGameManager->SetExit();
		return false ;
	}

	if( pkWindow )		{
		pkWindow->Show();
		pkWindow->SetEnable(true) ;
	}

	SPWindowEdit* pWindowID = (SPWindowEdit*)Find(WIID_LOGGIN_EDIT_ID, true);
	//pWindowID->SetEnableTabStop(true) ;
	SPWindowEdit* pWindowPW = (SPWindowEdit*)Find(WIID_LOGGIN_EDIT_PW, true);
	//pWindowPW->SetEnableTabStop(true) ;
	pkWindow = Find(WIID_LOGGIN_CONFIRM, true) ;
	SetEnterFocusWindow(pkWindow) ;

	if( pWindowPW ) {
		pWindowPW->SPSendMessage(SPIM_KILLFOCUS);	
		pWindowPW->SetWindowText(NULL);
	}
	if( pWindowID )	{
		pWindowID->SPSendMessage(SPIM_SETFOCUS);
		g_pInterfaceManager->SetFocusWindow( pWindowID);
		SetTabStopWindow( pWindowID );
	}
	SetShowState(LOGIN_DIALOG_NORMAL) ;
	InitTabStop() ;
	
	return true ;
}
/**
	Login창 닫기 & SoftKeyboard Close
*/
bool	SPWindowLogin::CloseLoginDialog()
{
	//SPWindowEdit* pWindowID = (SPWindowEdit*)Find(WIID_LOGGIN_EDIT_ID, true);
	//pWindowID->SetEnableTabStop(false) ;
	//SPWindowEdit* pWindowPW = (SPWindowEdit*)Find(WIID_LOGGIN_EDIT_PW, true);
	//pWindowPW->SetEnableTabStop(false) ;

	SPWindow* pkWindow;
	pkWindow = Find( WIID_LOGGIN_BG );
	if( pkWindow )	
		pkWindow->Hide();

	CloseSoftKeyboard() ;

	return true ;
}



/**
	E-Key Dialog 창이 출력되도록 한다.
	@param iEKeyType :	EKEYTYPE_WITHOUT_CHALLENGE_6 : EKey창만 6자리
						EKEYTYPE_WITHOUT_CHALLENGE_8 : ChallengeCode 출력 8자리
						EKEYTYPE_WITH_CHALLENGE_8 : EKey창만 8자리
*/
bool	SPWindowLogin::SetEnableEKeyDialog(int iEKeyType, string strChallengeCode)
{
	SPWindow* pkWindow;
	SPWindowEdit* pEditWindow;
	pkWindow = Find(WIID_LOGGIN_EK_BG);
	if( pkWindow )		{
		pkWindow->Show();
		pkWindow->SetEnable(true) ;
	}
	SPWindow* pWindowID = Find(WIID_LOGGIN_EK_IDKEY, true);
	if( pWindowID == NULL )
		return false ;
	//pWindowID->SetEnableTabStop(true) ;
	pkWindow = Find(WIID_LOGIN_EK_COMMIT, true) ;
	SetEnterFocusWindow(pkWindow) ;

	m_bIsEKEY = true ;

	switch( iEKeyType )
	{
	case EKEYTYPE_WITH_CHALLENGE_8:			// ChgCode + Key 8
		pEditWindow = (SPWindowEdit*)Find( WIID_LOGGIN_EK_CHGKEY, true );
		pEditWindow->SetWindowText(strChallengeCode.c_str()) ;

		pEditWindow = (SPWindowEdit*)pWindowID ;
		pEditWindow->SetLimitText(8) ;
		//pEditWindow->SetEnableTabStop(true) ;
		break ;

	case EKEYTYPE_WITHOUT_CHALLENGE_6 :			// Key 6
	default:
		pEditWindow = (SPWindowEdit*)pWindowID ;
		pEditWindow->SetLimitText(8) ;

		pkWindow = Find( WIID_LOGGIN_EK_CHGBG, true );
		pkWindow->Hide() ;
		pkWindow = Find( WIID_LOGGIN_EK_CHGKEY, true );
		pkWindow->Hide() ;
		break ;
	}
	
	g_pInterfaceManager->SetFocusWindow( pWindowID);
	SetTabStopWindow( pWindowID );
	SetShowState(LOGIN_DIALOG_EKEY) ;
	InitTabStop() ;

	return true ;
}

/**
	EKey창 닫기
*/
bool	SPWindowLogin::CloseEkeyDialog()
{
	SPWindow* pkWindow;
	pkWindow = Find(WIID_LOGGIN_EK_BG);
	if( pkWindow )		
		pkWindow->Hide();

	SPWindow* pWindowID = Find(WIID_LOGGIN_EK_IDKEY, true);
	//pWindowID->SetEnableTabStop(false) ;

	m_bIsEKEY = false ;
	return true ;
}


/**
	If E-key Authentication Server Error,
	then User's E-key Serial Number Input Dialog Show.
*/
bool	SPWindowLogin::SetEnableSerialDialog()
{
	SPWindow* pkWindow;
	pkWindow = Find(WIID_LOGGIN_SERIAL_BG);
	if( pkWindow )		
		pkWindow->Show();

	pkWindow = Find(WIID_LOGGIN_SERIAL_COMMIT, true) ;
	g_pInterfaceManager->SetFocusWindow( pkWindow );
	SetEnterFocusWindow(pkWindow) ;

	SPWindowEdit* pEditWindow;
	pEditWindow = (SPWindowEdit*)Find(WIID_LOGGIN_SERIAL_EDIT, true);		// Focus Edit
	pEditWindow->SetLimitText(6) ;
	//pEditWindow->SetEnableTabStop(true) ;
	g_pInterfaceManager->SetFocusWindow( pEditWindow );
	SetTabStopWindow( pEditWindow );
	SetShowState(LOGIN_DIALOG_SERIAL) ;
	InitTabStop() ;

	return true ;
}


bool	SPWindowLogin::CloseSerialDialog()
{
	SPWindow* pkWindow;
	pkWindow = Find(WIID_LOGGIN_SERIAL_BG);
	if( pkWindow )		{
		pkWindow->Hide();

		SPWindow* pkSeWindow = Find(WIID_LOGGIN_SERIAL_EDIT, true);		// Focus Edit
		//pkSeWindow->SetEnableTabStop(false) ;
	}

	return true ;
}


/**
	SoftKeyboard Close
*/
void SPWindowLogin::CloseSoftKeyboard()
{
	SPWindowButton* pButton = (SPWindowButton*)Find(WIID_LOGGIN_SOFTKEY, true);
	if( pButton == NULL )		return ;

	pButton->SPSendMessage(SPIM_BTN_SELECT, FALSE);
	g_pInterfaceManager->GetSoftKeyWindow()->Hide() ;
	m_bShowSoftKey = false ;
}

void SPWindowLogin::PressEnterKey()
{
	OnLButtonUp(0, 0) ;
}





SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowLogin )
	SPIMESSAGE_COMMAND(	SPIM_CURSOR_ENTER,		OnCursorEnter	)
	SPIMESSAGE_COMMAND(	SPIM_CURSOR_OUT,		OnCursorOut		)
	SPIMESSAGE_COMMAND(	SPIM_KEY_RETURN,		OnKeyReturn		)
	SPIMESSAGE_COMMAND( SPIM_KEY_ESCAPE,		OnEscape		)
	SPIMESSAGE_COMMAND( SPIM_LBUTTON_UP,		OnEditFocus		)	

	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_LOGGIN_CONFIRM,	OnLButtonUp		)	
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_LOGGIN_EXIT,		OnExit		)
	
	SPIMESSAGE_CONTROL( SPIM_REFRESH,			WIID_LOGGIN_EDIT_ID,	OnUpdateID)

	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_LOGGIN_SAVE_ID,	OnSaveCheck		)
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_LOGGIN_SOFTKEY,	OnSoftKeyboard		)

	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_HOMEPAGE,			OnHomePage		)	
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_NEW_ACCOUNT,		OnNewAccount	)	
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_PASS_REVISION,		OnPasswordRevision	)	
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_PASS_FIND,			OnPasswordFind	)	

	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_LOGIN_EK_COMMIT,	OnEkeyCommit	)	
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_LOGIN_EK_CANCEL,	OnEkeyCencel	)

	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_LOGGIN_SERIAL_COMMIT,	OnSerialCommit	)	
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_LOGGIN_SERIAL_CANCEL,	OnSerialCencel	)

	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_LOGGIN_IDC,			OnIDC	)

	SPIMESSAGE_COMMAND( SPIM_NOTICE_YES,		OnNoticeYes)
	
	// 기타 메시지의 경우 포커스를 셋팅한다.
	SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP,		WIID_LOGGIN_TITLE,  WIID_LOGGIN_SERIAL_CANCEL,		OnEditFocus		)	
	
SPIMESSAGE_OBJ_MAP_END	( SPWindow	)

int SPWindowLogin::OnCursorEnter	( WPARAM wParam, LPARAM lParam)
{
	//m_RGBA = RGBA(0xFF,0xFF,0xFF,0xFF);
	return 1;
}

int SPWindowLogin::OnCursorOut	( WPARAM wParam, LPARAM lParam)
{
	//m_RGBA = RGBA(0xFF,0xFF,0xFF,0xFF);
	return 1;
}

int SPWindowLogin::OnLButtonDown	( WPARAM wParam, LPARAM lParam )
{	
	return 1;
}


/**
	Focus 관련 처리
*/
int SPWindowLogin::OnEditFocus	( unsigned int iID, WPARAM wParam, LPARAM lParam )
{
	return OnEditFocus(wParam, lParam);
}
int SPWindowLogin::OnEditFocus	( WPARAM wParam, LPARAM lParam )
{
	SPWindow* pFocusWnd = g_pInterfaceManager->GetFocusWindow() ;
	if( pFocusWnd->GetClassID() == WNDID_CTRL_STATIC || pFocusWnd->GetClassID() == WNDID_LOGIN)
	{
		SPWindow* pWindowID = NULL ;
		if( m_iShowState == LOGIN_DIALOG_NORMAL )
		{
			pWindowID = (SPWindow*)GetLastFocusEdit();
			g_pInterfaceManager->SetFocusWindow(pWindowID) ;
			g_pInterfaceManager->GetIMEPool()->SetSelW(GetCaretStart(), GetCaretEnd()) ;
		}
		else if (m_iShowState == LOGIN_DIALOG_EKEY )
		{
			pWindowID = Find(WIID_LOGGIN_EK_IDKEY, true);
			g_pInterfaceManager->SetFocusWindow(pWindowID) ;
		}
		else if (m_iShowState == LOGIN_DIALOG_SERIAL )
		{
			pWindowID = Find(WIID_LOGGIN_SERIAL_EDIT, true);
			g_pInterfaceManager->SetFocusWindow(pWindowID) ;
		}
	}
	
	return 1 ;
}


/**
	Login Connect Button 
*/
int SPWindowLogin::OnLButtonUp	( WPARAM, LPARAM )
{
	//TODO:	Login Method 추가
	assert(g_pSubGameManager->GetSubGameID() == SUBGAME_LOGGIN);

	SPWindowEdit* pWindowID = (SPWindowEdit*)Find(WIID_LOGGIN_EDIT_ID, true);
	SPWindowEdit* pWindowPW = (SPWindowEdit*)Find(WIID_LOGGIN_EDIT_PW, true);		

	std::string strLoginID = pWindowID->GetWindowText();
	std::string strLoginPW = pWindowPW->GetWindowText();

	int	iResult = 0;

	if( strLoginID.length() > 0 && strLoginPW.length() > 0 )
	{
		// If Checked -> Saved ID
		SPWindowButton* pSaveButton = (SPWindowButton*)Find(WIID_LOGGIN_SAVE_ID, true);
		if(pSaveButton) {
			if(pSaveButton->IsChecked()) {
				g_pClientOption->SaveUserID(strLoginID);
			}
			else {
				g_pClientOption->SaveUserID();
			}
		}

		g_pSubGameManager->SPPerformMessage(SPGM_REQ_CONNCET_TRY, 
			WPARAM(pWindowID->GetWindowText()), 
			LPARAM(pWindowPW->GetWindowText()));

		iResult	=	1;
	}

	strLoginPW.assign( strLoginPW.size() , ' ' );
	strLoginPW.clear();
	pWindowPW->ClearStringBuffer();

	return iResult;
}


int SPWindowLogin::OnKeyReturn	( WPARAM, LPARAM )
{
	//m_RGBA = RGBA(0xFF,0x00,0x00,0xFF);

	if( m_pLoginIDC != NULL )
		if( m_pLoginIDC->SPSendMessage( SPIM_KEY_RETURN ) > 0 )
			return 1;

	if(g_pInterfaceManager->GetIMEPool()->IsComposition() == true)
		return 1;

	if( m_pWindowEnterFocus ) {
		assert(m_pWindowEnterFocus->GetClassID() == WNDID_CTRL_BUTTON);
		if( m_pWindowEnterFocus->IsShow() == true && g_pInterfaceManager->GetModalWindow() == NULL )
			m_pWindowEnterFocus->SPSendMessage(SPIM_LBUTTON_UP);
	}
	return 1;
}

int SPWindowLogin::OnEscape	( WPARAM, LPARAM )
{
	if( m_pLoginIDC != NULL )
		if( m_pLoginIDC->SPSendMessage( SPIM_KEY_ESCAPE ) > 0 )
			return 1;

	return 0;
}

int SPWindowLogin::OnExit	( WPARAM, LPARAM )
{	
	g_pMainGameManager->SetExit();
	return 1;
}

/**
	Notice Box에서 Yes 선택
	-> Exit 조건이면 종료되도록 해야함.
*/
int SPWindowLogin::OnNoticeYes( WPARAM wParam, LPARAM lParam)
{
	assert(g_pSubGameManager->GetSubGameID() == SUBGAME_LOGGIN);
	// If Reserve Exit
	if( g_pSubGameManager->SPPerformMessage( SPGM_CHECK_RESERVEEXIT ) == 1 )
	{
		g_pMainGameManager->SetExit();
	}
	// If Buddy & Login Notice Show -> Exit
	if( SPLocalizeManager::GetInstance()->OnlyWebLogin() )
	{
		g_pMainGameManager->SetExit();
	}

	// If TimeOut then Move LoginDialog
	if( g_pSubGameManager->SPPerformMessage( SPGM_CHECK_TIMEOUT ) == 1 )
	{
		CloseEkeyDialog() ;
		CloseSerialDialog() ;
		SetEnableLoginDialog() ;
		g_pSubGameManager->SPPerformMessage( SPGM_CHECK_TIMEOUT, 1, 0 ) ;		// TimeOutFlag Init

		//SetRectSize(320, 410) ;
	}
	return 1;
}



int SPWindowLogin::OnUpdateID(WPARAM wParam, LPARAM lParam)
{
	SPWindowButton* pSaveButton = (SPWindowButton*)Find(WIID_LOGGIN_SAVE_ID, true);	
	SPWindowEdit* pEidtID = (SPWindowEdit*)Find(WIID_LOGGIN_EDIT_ID, true);

	std::string strID = pEidtID->GetWindowText();
	
	if(m_bSaveID) {		
		if(m_strLastID != strID) {					//이픸E문자열에서 변경된 경퓖E저픸E횁E?
			pSaveButton->SPSendMessage(SPIM_BTN_SELECT, (WPARAM)FALSE);
			m_bSaveID = false;
			
			//아이탛E저장을 횁E老磯?
			//g_pClientOption->SaveUserID();
		}		
	}
	return 1;
}

int SPWindowLogin::OnSaveCheck(WPARAM wParam, LPARAM lParam)
{
	SPWindowButton* pSaveButton = (SPWindowButton*)Find(WIID_LOGGIN_SAVE_ID, true);
	//bool bCheck = pSaveButton	
	
	if(m_bSaveID){
		pSaveButton->SPSendMessage(SPIM_BTN_SELECT, (WPARAM)FALSE);
		m_bSaveID = false;
		
		//아이탛E저장을 횁E老磯?
		//g_pClientOption->SaveUserID();
	}
	else {
		//만일 에디터에 문자열값이 없다툈E리턴
		SPWindowEdit* pEidt = (SPWindowEdit*)Find(WIID_LOGGIN_EDIT_ID, true);
		std::string strID = pEidt->GetWindowText();
		if(strID == "") {
			return 1;
		}
		
		pSaveButton->SPSendMessage(SPIM_BTN_SELECT, (WPARAM)TRUE);
		m_bSaveID = true;

		//아이디를 옵션 정보로 보내준다
		//g_pClientOption->SaveUserID(strID);

		m_strLastID = strID;
	}

	return 1;
}


int SPWindowLogin::OnSoftKeyboard(WPARAM wParam, LPARAM lParam)
{
	// SoftKeyboard Window에 Show 메시지를 보낸다.
	SPWindowButton* pButton = (SPWindowButton*)Find(WIID_LOGGIN_SOFTKEY, true);
	if( pButton == NULL )		return 0 ;

	if( m_bShowSoftKey )
	{
		// Softkey Close
		pButton->SPSendMessage(SPIM_BTN_SELECT, FALSE);
		m_bShowSoftKey = false ;
		g_pInterfaceManager->GetSoftKeyWindow()->Hide() ;
	}
	else
	{
		// SoftKey Show
		pButton->SPSendMessage(SPIM_BTN_SELECT, TRUE);
		m_bShowSoftKey = true ;
		g_pInterfaceManager->GetSoftKeyWindow()->SetShowEnable(true) ;
		g_pInterfaceManager->GetSoftKeyWindow()->Show() ;
	}
	return 1;
}




int SPWindowLogin::OnHomePage(WPARAM wParam, LPARAM lParam)
{
	SHELLEXECUTEINFO sei;
	ZeroMemory(&sei, sizeof(SHELLEXECUTEINFO));
	sei.cbSize = sizeof(SHELLEXECUTEINFO);

	sei.lpFile = __TEXT("explorer");
	sei.nShow = SW_SHOWMAXIMIZED;

	//sei.lpParameters = "http://www.latale.com";
	sei.lpParameters = SPLocalizeManager::GetInstance()->GetHomePage();
	if( strlen(sei.lpParameters) == 0 )
		return 0;	

	sei.lpVerb = __TEXT("open");
	ShellExecuteEx(&sei);
	if( g_bWindowed == false )
		SendMessage(g_hWnd, WM_ACTIVATE, 0, 0) ;
	ShowWindow(g_hWnd, SW_MINIMIZE);

	return 1;
}

int SPWindowLogin::OnNewAccount(WPARAM wParam, LPARAM lParam)
{
	SHELLEXECUTEINFO sei;
	ZeroMemory(&sei, sizeof(SHELLEXECUTEINFO));
	sei.cbSize = sizeof(SHELLEXECUTEINFO);

	sei.lpFile = __TEXT("explorer");
	sei.nShow = SW_SHOWMAXIMIZED;

	//sei.lpParameters = "http://www.latale.com";
	sei.lpParameters = SPLocalizeManager::GetInstance()->GetNewAccount();
	if( strlen(sei.lpParameters) == 0 )
		return 0;	

	sei.lpVerb = __TEXT("open");
	ShellExecuteEx(&sei);
	if( g_bWindowed == false )
		SendMessage(g_hWnd, WM_ACTIVATE, 0, 0) ;
	ShowWindow(g_hWnd, SW_MINIMIZE);

	return 1;
}


int SPWindowLogin::OnPasswordRevision(WPARAM wParam, LPARAM lParam)
{
	
	SHELLEXECUTEINFO sei;
	ZeroMemory(&sei, sizeof(SHELLEXECUTEINFO));
	sei.cbSize = sizeof(SHELLEXECUTEINFO);

	sei.lpFile = __TEXT("explorer");
	sei.nShow = SW_SHOWMAXIMIZED;

	// LocalizeManager에 비밀번호 수정 페이지 추가 (Actoz "ID찾기")
	sei.lpParameters = SPLocalizeManager::GetInstance()->GetHomeFindUserID();
	if( strlen(sei.lpParameters) == 0 )
		return 0;	

	sei.lpVerb = __TEXT("open");
	ShellExecuteEx(&sei);
	if( g_bWindowed == false )
		SendMessage(g_hWnd, WM_ACTIVATE, 0, 0) ;
	ShowWindow(g_hWnd, SW_MINIMIZE);

	return 1;
}



int SPWindowLogin::OnPasswordFind(WPARAM wParam, LPARAM lParam)
{
	SHELLEXECUTEINFO sei;
	ZeroMemory(&sei, sizeof(SHELLEXECUTEINFO));
	sei.cbSize = sizeof(SHELLEXECUTEINFO);

	sei.lpFile = __TEXT("explorer");
	sei.nShow = SW_SHOWMAXIMIZED;

	// LocalizeManager에 비밀번호 찾기
	sei.lpParameters = SPLocalizeManager::GetInstance()->GetHomeFindPasswd();
	if( strlen(sei.lpParameters) == 0 )
		return 0;	

	sei.lpVerb = __TEXT("open");
	ShellExecuteEx(&sei);

	if( g_bWindowed == false )
		SendMessage(g_hWnd, WM_ACTIVATE, 0, 0) ;
	ShowWindow(g_hWnd, SW_MINIMIZE);

	return 1;
}



//////////////////////////////////////////////////////////////////////////
// Ekey Dialog Button

int SPWindowLogin::OnEkeyCommit(WPARAM wParam, LPARAM lParam)
{
	SPWindowEdit*	pEkeyWindow;
	SPWindowEdit*	pchgkeyWindow;
	pEkeyWindow = (SPWindowEdit*)Find( WIID_LOGGIN_EK_IDKEY, true );
	if( pEkeyWindow == NULL )		return 0 ;

	pchgkeyWindow = (SPWindowEdit*)Find( WIID_LOGGIN_EK_CHGKEY, true );

	std::string strLoginEkey = pEkeyWindow->GetWindowText();
	if( strLoginEkey.length() > 5 ) {
		g_pSubGameManager->SPPerformMessage( SPGM_REQ_EKEY_CONNCET_TRY, 
			WPARAM(pchgkeyWindow->GetWindowText()),			
			LPARAM(pEkeyWindow->GetWindowText())	) ;
	}
	else
	{
		// Ekey ReInput Message....
	}

	pEkeyWindow->SetWindowText("") ;	// 전송후 초기화

	return 1;
}

int SPWindowLogin::OnEkeyCencel(WPARAM wParam, LPARAM lParam)
{
	SPWindowEdit*	pEditWindow;
	pEditWindow = (SPWindowEdit*)Find( WIID_LOGGIN_EK_IDKEY, true );
	pEditWindow->SetWindowText("") ;	// 초기화

	g_pNetworkManager->Disconnect2();

	CloseEkeyDialog() ;
	SetEnableLoginDialog() ;

	return 1;
}

//////////////////////////////////////////////////////////////////////////
// Serial Dialog Button
int SPWindowLogin::OnSerialCommit(WPARAM wParam, LPARAM lParam)
{
	SPWindowEdit*	pEditWindow;
	pEditWindow = (SPWindowEdit*)Find( WIID_LOGGIN_SERIAL_EDIT, true );
	if( pEditWindow == NULL )		return 0 ;

	std::string strLoginSerial = pEditWindow->GetWindowText();

	if( strLoginSerial.length() > 5 || strLoginSerial != "") {
		g_pSubGameManager->SPPerformMessage( SPGM_REQ_SERIAL_CONNCET_TRY, 
			WPARAM(pEditWindow->GetWindowText())) ;
	}
	else
	{
		// Ekey ReInput Message....
	}

	pEditWindow->SetWindowText("") ;	// 전송후 초기화

	return 1;
}

int SPWindowLogin::OnSerialCencel(WPARAM wParam, LPARAM lParam)
{
	SPWindowEdit*	pEditWindow;
	pEditWindow = (SPWindowEdit*)Find( WIID_LOGGIN_SERIAL_EDIT, true );
	pEditWindow->SetWindowText("") ;	// 초기화

	g_pNetworkManager->Disconnect2();

	CloseSerialDialog() ;
	SetEnableLoginDialog() ;

	return 1;
}

//////////////////////////////////////////////////////////////////////////
//
//	IDC Select
//

int SPWindowLogin::OnIDC( WPARAM wParam , LPARAM lParam )
{
	if( m_pLoginIDC != NULL )
		m_pLoginIDC->SPSendMessage( SPIM_LOGIN_IDC_SHOW );

	return 1;
}
