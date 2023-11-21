
#include <vector>
#include <string>

#include "SPCommon.h"
#include "SPUtil.h"

#include "SPMainGameDEF.H"
#include "SPGameObjectDEF.h"

#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPSubGameManager.h"
#include "SPGameObject.h"
#include "SPManager.h"
#include "SPMouseCursor.h"
#include "SPIMEPool.h"
#include "SPWindowDEF.h"
#include "SPInterfaceManager.h"

#include "SPWindow.h"
#include "SPWindowButton.h"
#include "SPWindowStatic.h"

#include "SPStage.h"
#include "SPStageManager.h"
#include "SPStageLoader.h"

//
//#include "SPMainGameDEF.H"
//#include "SPSubGameManager.h"
#include "SPMainGameManager.h"
//#include "SPMouseCursor.h"
//#include "SPWindowDEF.h"
//#include "SPInterfaceManager.h"

#include "SPWindowMessage.h"


SPWindowMessage::SPWindowMessage(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent /*= NULL*/)
: SPWindow(WNDID_MESSAGE, InstanceID, iX, iY, iCX, iCY, pParent)
{
	Init();
}


SPWindowMessage::~SPWindowMessage()
{
	Clean();
}

void SPWindowMessage::Init()
{
	m_strMessage = "";
	m_strInfo = "";	

	SPWindow* pStatic = NULL;
	SPWindow* pCenter = NULL;

	pStatic = new SPWindowStatic(WIID_MESSAGE_TOP, 0, 0, 254, 5, this);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 257,	63);
	pStatic->Show();
	pStatic = NULL;

	pCenter = new SPWindowStatic(WIID_MESSAGE_CENTER, 0, 5, 254, 83, this);
	pCenter->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 257,	70);
	pCenter->SetSrcSize(511-257, 2);
	pCenter->Show();

	pStatic = new SPWindowStatic(WIID_MESSAGE_BOTTOM, 0, 88, 254, 5, this);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 257,	74);
	pStatic->Show();
	pStatic = NULL;
	
	
	//메시지 영역	
	pStatic = new SPWindowStatic(WIID_MESSAGE_MESSAGE, 15, 3 - 5, 225, 69, pCenter);
	pStatic->SetFont(FONT_14_BOLD);
	pStatic->SetFontColor(RGBA(255, 235, 166, 255));
	pStatic->SetFormat( DT_CENTER | DT_VCENTER | DT_WORDBREAK );
	pStatic->SetMultiLine(true, 5);
	pStatic->Show();
	m_pMessage = pStatic;
	pStatic = NULL;
	
	pStatic = new SPWindowStatic(WIID_MESSAGE_INFO, 30, 75 - 5, 214, 12, pCenter);
	pStatic->SetFont(FONT_12_BOLD);
	pStatic->SetFontColor(RGBA(255, 255, 255, 255));
	pStatic->SetFormat( DT_RIGHT | DT_VCENTER | DT_WORDBREAK );
	pStatic->Show();
	m_pInfo = pStatic;
	pStatic = NULL;

	Hide();
}


void SPWindowMessage::Clean()
{
	SPWindow::Clean();	
}


void SPWindowMessage::SetFocus()
{
	// 이윈도우에선 포커스 먹으면 안된돠~~ 
}

void SPWindowMessage::Process(float fTime)
{
	SPWindow::Process(fTime);
}


void SPWindowMessage::Render(float fTime)
{
	if(!m_bShow)
		return;
	
	SPWindow::Render(fTime);
	g_pVideo->Flush();
}


void SPWindowMessage::RenderText()
{
	SPWindow::RenderText();
}


//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowMessage )
	//SPIMESSAGE_COMMAND( SPIM_SET_CAMERA_POS,	OnSetCameraPos	)
	//SPIMESSAGE_COMMAND( SPIM_SET_LOCAL_PLAYER_POS, OnSetPlayerPos)

	SPIMESSAGE_COMMAND( SPIM_SET_MESSAGE_SHOW,	OnMessageShow	)
	SPIMESSAGE_COMMAND( SPIM_SET_MESSAGE_TEXT,	OnMessageTextSet)
SPIMESSAGE_OBJ_MAP_END	( SPWindow	)


int SPWindowMessage::OnMessageShow( WPARAM wParam, LPARAM lParam )
{
	//if(m_bShow)
	//	m_bShow = false;
	//else 
	//	m_bShow = true;

	m_bShow = (bool)wParam;
	
	if(!m_bShow) {
		Hide();
		m_strMessage.clear();
		//m_strInfo.clear();
	} else {
		Show();	
	}
		
	return 1;
}


int SPWindowMessage::OnMessageTextSet( WPARAM wParam, LPARAM lParam )
{
	//unsigned int iMessageStringID = (unsigned int)wParam;			//Tip Message
	//unsigned int iMessageInfoID = (unsigned int)lParam;			//EventType에 따른 Message (포탈인 경우 이동, 세이브 포인트의 경우 작동)
	int iEventType = (EVENT_TYPE)lParam;
	
	m_strMessage.clear();
	m_strMessage = "";
	
	////
	//if(iEventType == MAX_EVENT) {
	//	m_strMessage = (char*)wParam;
	//}
	//else {
	//	if(iMessageStringID != 0) {
	//		if(g_pResourceManager->GetGlobalString(iMessageStringID) != NULL)
	//			m_strMessage = g_pResourceManager->GetGlobalString(iMessageStringID);
	//		else 
	//			m_strMessage = "Global String Index Error";				//Global LDT 에서 읽을수 있도록 할것
	//	}
	//}	

	m_strMessage = (char*)wParam;
	
	////
	//if(iMessageStringID != 0) {
	//	if(g_pResourceManager->GetGlobalString(iMessageStringID) != NULL)
	//		m_strMessage = g_pResourceManager->GetGlobalString(iMessageStringID);
	//	else 
	//		m_strMessage = "Global String Index Error";				//Global LDT 에서 읽을수 있도록 할것
	//}
	m_pMessage->SetWindowText(m_strMessage.c_str());
	
	m_strInfo.clear();
	m_strInfo = "";
	if(iEventType == EVENT_SAVEPOINT || iEventType == EVENT_PORTAL || iEventType == EVENT_NPC || iEventType == 5) {
		m_strInfo = g_pResourceManager->GetGlobalString(3000000);		
	}
	m_pInfo->SetWindowText(m_strInfo.c_str());
	
	return 1;
}




