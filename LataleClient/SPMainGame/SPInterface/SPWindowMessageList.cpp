#include "SPCommon.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"

#include "SPCommon.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowSlider.h"
#include "SPWindowButton.h"
#include "SPWindowEdit.h"
//#include "SPWindowList.h"
#include "SPWindowFriendUser.h"
#include "SPWindowChat.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include <algorithm>

#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"
#include "SockWnd.h"
#include "SPNetworkManager.h"

#include "SPGameObjectDEF.h"
#include "SPGameObject.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPPlayer.h"
#include "SPGOBManager.h"
#include "SPGOBStatus.h"

#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"
#include "SPResourceManager.h"

#include "SPResourceManager.h"
#include "SPPvpManager.h"

#include "SPUtil.h"
#include "SPDebug.h"

#include "SPEventDEF.h"
#include "SPEventManager.h"

#include "SPSubGameManager.h"
#include "SPMainGameManager.h"
#include "SPAbuseFilter.h"

#include "SPWindowFriend.h"

#include "SPWindowMessageList.h"

using namespace std;

//------------------------------------------------------------------------------------
SPWindowMessageList::SPWindowMessageList(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
{	
	Init();
}

//------------------------------------------------------------------------------------
SPWindowMessageList::SPWindowMessageList(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_MESSAGELIST, InstanceID, iX, iY, iCX, iCY, pParent)							 
{	
	Init();
}

//------------------------------------------------------------------------------------
SPWindowMessageList::~SPWindowMessageList()
{
	Clean();
}

//------------------------------------------------------------------------------------
void SPWindowMessageList::Clean()
{
	SPWindow::Clean();

	m_mMsgList.clear();
}

//------------------------------------------------------------------------------------
void SPWindowMessageList::Init()
{
	m_mMsgList.clear();
}

//------------------------------------------------------------------------------------
void SPWindowMessageList::Process(float fTime)
{
	if( m_bShow == false ) return;

	SPWindow::Process(fTime);
}

//------------------------------------------------------------------------------------
void SPWindowMessageList::Render(float fTime)
{
	if( m_bShow == false ) return;
	
	SPWindow::Render(fTime);
}

//------------------------------------------------------------------------------------
void SPWindowMessageList::Show(bool bWithChild)
{
	SPWindow::Show( bWithChild );
}

//------------------------------------------------------------------------------------
int SPWindowMessageList::OnClose( WPARAM wParam, LPARAM lParam )
{
	Hide();
	return 1;
}


//------------------------------------------------------------------------------------
SPIMESSAGE_OBJ_MAP_BEGIN(SPWindowMessageList)


SPIMESSAGE_OBJ_MAP_END(SPWindow)
