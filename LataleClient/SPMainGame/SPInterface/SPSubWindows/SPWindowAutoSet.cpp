
#include "SPCommon.h"
#include "SPUtil.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowAutoSet.h"

#include "SPWindowListDEF.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPWindowEdit.h"
#include "SPWindowSlider.h"

#include <WinSock2.h>
#include "PacketID.h"
#include "Packet.h"
#include "PacketHandler.h"

#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"
#include "SPResourceManager.h"

#include "SPRsManager.h"
#include "SPLocalizeManager.h"

#include "_Error_log.h"

//------------------------------------------------------------------------------------
SPWindowAutoSet::SPWindowAutoSet(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
{
	Init();
}

//------------------------------------------------------------------------------------
SPWindowAutoSet::SPWindowAutoSet(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_AUTOSET, InstanceID, iX, iY, iCX, iCY, pParent)
{
	Init();
}

//------------------------------------------------------------------------------------
SPWindowAutoSet::~SPWindowAutoSet()
{
	Clean();
}

//------------------------------------------------------------------------------------
void SPWindowAutoSet::Clean()
{
	SPWindow::Clean();
}

//------------------------------------------------------------------------------------
void SPWindowAutoSet::Init()
{
	SPWindow* pStatic = NULL;
	SPWindow* pParent = NULL;
	SPWindowButton* pButton = NULL;

	pParent = new SPWindowStatic(WIID_AUTO_FRAME, 0, 0, 591, 359, this);

	pStatic = new SPWindowStatic(WIID_AUTO_PIC, 0, 0, 591, 359, pParent);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI112.PNG", 0, 0);

	pButton = new SPWindowButton(WIID_AUTO_EXIT, 570, 10, 12, 12, pStatic);
	pButton->SetImage		("DATA/INTERFACE/CONCEPT/UI102.PNG", 247,  217);
	pButton->SetImageHit	("DATA/INTERFACE/CONCEPT/UI102.PNG", 247,  230);
	pButton->SetImagePush	("DATA/INTERFACE/CONCEPT/UI102.PNG", 247,  244);
	pButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI102.PNG", 247,  256);

}

//------------------------------------------------------------------------------------
void SPWindowAutoSet::Process(float fTime)
{
	SPWindow::Process(fTime);
}

//------------------------------------------------------------------------------------
void SPWindowAutoSet::Render(float fTime)
{
	if(IsShow() == false)
		return;

	SPWindow::Render(fTime);
}

//------------------------------------------------------------------------------------
int SPWindowAutoSet::OnExitClick( WPARAM wParam, LPARAM lParam )
{
	Hide();
	return 1;
}

//------------------------------------------------------------------------------------
SPIMESSAGE_OBJ_MAP_BEGIN(SPWindowAutoSet)

SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,	WIID_AUTO_EXIT,				OnExitClick			)

SPIMESSAGE_OBJ_MAP_END(SPWindow)
