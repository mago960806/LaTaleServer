
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
#include "SPIMEPool.h"
#include "SPWindowDEF.h"
#include "SPInterfaceManager.h"


#include "SPWindow.h"
#include "SPWindowChatHistory.h"


SPWindowChatHistory::SPWindowChatHistory(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
{	
	Init();
}

SPWindowChatHistory::SPWindowChatHistory(WND_ID WndClassID, INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WndClassID, InstanceID, iX, iY, iCX, iCY, pParent)	
{	
	Init();
}

SPWindowChatHistory::~SPWindowChatHistory()
{
	Clean();
}

void SPWindowChatHistory::Init()
{
	//SPWindow::Init();
	m_RGBA = RGBA(0xFF,0xFF,0xFF,0x00);
	m_strToolTip = "[SPWindowChatHistory]";
}

void SPWindowChatHistory::Clean()
{
	SPWindow::Clean();
}

void SPWindowChatHistory::Process(float fTime)
{
	SPWindow::Process(fTime);
}

void SPWindowChatHistory::Render(float fTime)
{
	SPWindow::Render(fTime);
}

SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowChatHistory )
	SPIMESSAGE_COMMAND(	SPIM_CURSOR_ENTER,		OnCursorEnter	)
	SPIMESSAGE_COMMAND(	SPIM_CURSOR_OUT,		OnCursorOut		)
	SPIMESSAGE_COMMAND(	SPIM_LBUTTON_DOWN,		OnCursorLDown	)
	SPIMESSAGE_COMMAND(	SPIM_LBUTTON_UP,		OnCursorLUp		)
SPIMESSAGE_OBJ_MAP_END	( SPWindow	)


int SPWindowChatHistory::OnCursorEnter	( WPARAM, LPARAM )
{
	m_RGBA = RGBA(0xFF,0xFF,0xFF,0xDD);
	return 1;
}

int SPWindowChatHistory::OnCursorOut	( WPARAM, LPARAM )
{
	m_RGBA = RGBA(0xFF,0xFF,0xFF,0x00);
	return 1;
}

int SPWindowChatHistory::OnCursorLDown	( WPARAM, LPARAM )
{
	//m_RGBA = RGBA(0x88,0x88,0xFF,0xFF);
	return 1;
}

int SPWindowChatHistory::OnCursorLUp	( WPARAM, LPARAM )
{
	//m_RGBA = RGBA(0xFF,0x00,0x00,0xFF);
	return 1;
}



















