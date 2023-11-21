
#include <vector>
#include <string>

#include "SPCommon.h"
#include "SPUtil.h"

#include "SPMainGameDEF.H"
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
#include "SPWindowExit.h"
#include "SPTimer.h"

#include "SPWindowFindMessage.h"
#include "SPWindowFindMessageOne.h"
#include "SPWindowFindMessageTwo.h"
#include "SPGameObject.h"
#include "SPGOBStatus.h"
#include "SPGOBClusterDef.h"
#include "SPGOBCluster.h"
#include "SPGobModelUnit.h"
#include "SPAvatarModelUnitDef.h"
#include "SPAvatarModelUnit.h"
#include "SPPlayer.h"

//------------------------------------------------------------------------------------
SPWindowExit::SPWindowExit(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_EXIT, InstanceID, iX, iY, iCX, iCY, pParent)	
{	
	Init();
}

//------------------------------------------------------------------------------------
SPWindowExit::~SPWindowExit()
{
	Clean();
}

//------------------------------------------------------------------------------------
void SPWindowExit::Clean()
{
	SPWindow::Clean();
}

//------------------------------------------------------------------------------------
void SPWindowExit::Init()
{
	//SPWindow::Init();
	m_RGBA = RGBA(0xFF,0xFF,0xFF,0xFF);
	m_strToolTip = "[SPWindowExit]";
	m_bHideEnable = true;

	SPWindow* pkSkinWindow, *pkChildWindow;
	SPWindowButton* pkButton;

	pkSkinWindow = new SPWindowStatic(WIID_EXIT_SKIN_UPPER, 0, 0, 134, 4, this);
	pkSkinWindow->SetImage("DATA/INTERFACE/CONCEPT/UI100.PNG", 77, 211);

	pkSkinWindow = new SPWindowStatic(WIID_EXIT_SKIN_LOWER, 0, 101, 134, 4, this);
	pkSkinWindow->SetImage("DATA/INTERFACE/CONCEPT/UI100.PNG", 77, 221);

	pkSkinWindow = new SPWindowStatic(WIID_EXIT_SKIN_MIDDLE, 0, 4, 134, 97, this);
	pkSkinWindow->SetImage("DATA/INTERFACE/CONCEPT/UI100.PNG", 77, 217);
	pkSkinWindow->SetSrcSize(134, 2);

	pkChildWindow = new SPWindowStatic(WIID_EXIT_TITLE_TEXT, 10, 3, 28, 15, pkSkinWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI100.PNG", 212, 210);

	pkChildWindow = new SPWindowStatic(WIID_EXIT_ENG_TITLE_TEXT, 40, 8, 25, 9, pkSkinWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI100.PNG", 214, 200);

	pkButton = new SPWindowButton(WIID_EXIT_CLOSE, 113, 5, 12, 12, pkSkinWindow);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 247, 217);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI102.PNG", 247, 230);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI102.PNG", 247, 243);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI102.PNG", 247, 256);

	pkButton = new SPWindowButton(WIID_EXIT_SERVER_CHANGE, 19, 30, 96, 17, pkSkinWindow);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 124, 317);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI101.PNG", 221, 317);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI101.PNG", 318, 317);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI101.PNG", 415, 317);

	pkButton = new SPWindowButton(WIID_EXIT_CHANNEL_CHANGE, 19, 52, 96, 17, pkSkinWindow);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 124, 336);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI101.PNG", 221, 336);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI101.PNG", 318, 336);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI101.PNG", 415, 336);

	pkButton = new SPWindowButton(WIID_EXIT_GOTO_LOGIN, 19, 74, 96, 17, pkSkinWindow);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 124, 355);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI101.PNG", 221, 355);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI101.PNG", 318, 355);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI101.PNG", 415, 355);	
}

//------------------------------------------------------------------------------------
void SPWindowExit::Process(float fTime)
{
	SPWindow::Process(fTime);
}

//------------------------------------------------------------------------------------
void SPWindowExit::Render(float fTime)
{
	SPWindow::Render(fTime);
}

//------------------------------------------------------------------------------------
int SPWindowExit::OnConfirmExit( WPARAM, LPARAM )
{
	g_pSubGameManager->SPPerformMessage(SPGM_EXIT_ZONE, 1);	
    OnEnableButton(0, 0);
	OnClearFindMessage();
	resetPetSkillStatus();
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowExit::OnConfirmServerChange( WPARAM, LPARAM )
{
	g_pSubGameManager->SPPerformMessage(SPGM_EXIT_ZONE, 0);	
	OnEnableButton(0, 0);
	resetPetSkillStatus();
	OnClearFindMessage();
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowExit::OnConfirmChannelChange( WPARAM, LPARAM )
{
	g_pInterfaceManager->ShowWindow(WIID_CHANNEL_CHANGE, true);
	OnHide(0, 0);
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowExit::OnKeyReturn	( WPARAM, LPARAM )
{	
	if(IsShow() && m_pWindowEnterFocus) {
		assert(m_pWindowEnterFocus->GetClassID() == WNDID_CTRL_BUTTON);
		m_pWindowEnterFocus->SPSendMessage(SPIM_LBUTTON_UP);
		return 1;
	}
	return -1;	
}

//------------------------------------------------------------------------------------
int SPWindowExit::OnEnableButton(WPARAM wParam, LPARAM lParam)
{
	bool bEnable = false;
	if( wParam )
		bEnable = true;

	SPWindow* pkWindow;
	pkWindow = Find(WIID_EXIT_GOTO_LOGIN, true);
	if( pkWindow )
		pkWindow->SetEnable(bEnable);

	pkWindow = Find(WIID_EXIT_SERVER_CHANGE, true);
	if( pkWindow )
		pkWindow->SetEnable(bEnable);

	pkWindow = Find(WIID_EXIT_CHANNEL_CHANGE, true);
	if( pkWindow )
		pkWindow->SetEnable(bEnable);

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowExit::OnChannelEnable(WPARAM wParam, LPARAM lParam)
{
	bool bEnable = false;
	if( wParam )
		bEnable = true;

	SPWindow* pkWindow;
	pkWindow = Find(WIID_EXIT_CHANNEL_CHANGE, true);
	if( pkWindow )
		pkWindow->SetEnable(bEnable);

	return 1;
}

//------------------------------------------------------------------------------------
SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowExit )

SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_EXIT_GOTO_LOGIN		,		OnConfirmExit			)	
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_EXIT_SERVER_CHANGE		,		OnConfirmServerChange	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_EXIT_CHANNEL_CHANGE	,		OnConfirmChannelChange	)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,		WIID_EXIT_CLOSE				,		OnHide					)

SPIMESSAGE_COMMAND(	SPIM_KEY_RETURN,			OnKeyReturn		)
SPIMESSAGE_COMMAND(	SPIM_BTN_ENABLE,			OnEnableButton	)
SPIMESSAGE_COMMAND(	SPIM_EXIT_CHANNEL_ENABLE,	OnChannelEnable	)

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)

int SPWindowExit::OnClearFindMessage()
{

	SPWindowFindMessage* pFindOne = static_cast<SPWindowFindMessage*>( g_pInterfaceManager->FindWindow( WIID_FIND_MESSAGE ) );

	SPWindowFindMessageOne* pFindTwo = static_cast<SPWindowFindMessageOne*>( g_pInterfaceManager->FindWindow( WIID_FIND_MESSAGE_ONE ) );

	SPWindowFindMessageTwo* pFindThree = static_cast<SPWindowFindMessageTwo*>( g_pInterfaceManager->FindWindow( WIID_FIND_MESSAGE_TWO ) );

	if ( pFindOne != NULL && pFindTwo != NULL && pFindThree != NULL )
	{
			//需要更新,隐藏所有的框
			pFindOne->Hide();
			pFindOne->getTimer()->SetExpired();
			pFindOne->setSendMessage( false );

			pFindTwo->Hide();
			pFindTwo->getTimer()->SetExpired();
			pFindTwo->setSendMessage( false );

			pFindThree->Hide();
			pFindThree->getTimer()->SetExpired();
			pFindThree->setSendMessage( false );
	}

	return 1;
}
