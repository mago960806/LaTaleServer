
#include <vector>
#include <string>

#include "SPCommon.h"
#include "SPUtil.h"

#include "SPMainGameDEF.H"
#include "SPGameObjectDEF.h"

#include "SPSubGameManager.h"
#include "SPGameObject.h"
#include "SPManager.h"
#include "SPMouseCursor.h"

#include "SPWindowDEF.h"
#include "SPIMEPool.h"
#include "SPInterfaceManager.h"


#include "SPWindow.h"
#include "SPWindowChatConsole.h"


SPWindowChatConsole::SPWindowChatConsole(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
{	
	Init();
}

SPWindowChatConsole::SPWindowChatConsole(WND_ID WndClassID, INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WndClassID, InstanceID, iX, iY, iCX, iCY, pParent)	
{	
	Init();
}

SPWindowChatConsole::~SPWindowChatConsole()
{
	Clean();
}

void SPWindowChatConsole::Init()
{	
	m_RGBA = RGBA(0xFF,0xFF,0xFF,0xFF);
	m_strToolTip = "[SPWindowChatConsole]";



}

void SPWindowChatConsole::Clean()
{
	SPWindow::Clean();
}

void SPWindowChatConsole::Process(float fTime)
{
	SPWindow::Process(fTime);
}

void SPWindowChatConsole::Render(float fTime)
{
	SPWindow::Render(fTime);
}

SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowChatConsole )
	SPIMESSAGE_COMMAND(	SPIM_CURSOR_ENTER,		OnCursorEnter	)
	SPIMESSAGE_COMMAND(	SPIM_CURSOR_OUT,		OnCursorOut		)
	SPIMESSAGE_COMMAND(	SPIM_LBUTTON_DOWN,		OnCursorLDown	)
	SPIMESSAGE_COMMAND(	SPIM_LBUTTON_UP,		OnCursorLUp		)
SPIMESSAGE_OBJ_MAP_END	( SPWindow	)


int SPWindowChatConsole::OnCursorEnter	( WPARAM, LPARAM )
{
	m_RGBA = RGBA(0xFF,0xFF,0xFF,0xDD);
	return 1;
}

int SPWindowChatConsole::OnCursorOut	( WPARAM, LPARAM )
{
	m_RGBA = RGBA(0xFF,0xFF,0xFF,0xFF);
	return 1;
}

int SPWindowChatConsole::OnCursorLDown	( WPARAM, LPARAM )
{
	//m_RGBA = RGBA(0x88,0x88,0xFF,0xFF);
	return 1;
}

int SPWindowChatConsole::OnCursorLUp	( WPARAM, LPARAM )
{
	//m_RGBA = RGBA(0xFF,0x00,0x00,0xFF);
	return 1;
}



















