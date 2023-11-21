

#include <vector>
#include "SPCommon.h"

#include "SPMainGameDEF.H"
#include "SPFrameSkipper.h"

#include "SPGameObjectDEF.h"
#include "SPManager.h"

#include "SPGOBClusterDEF.h"
#include "SPGOBCluster.h"
#include "SPGOBManager.h"
#include "SPCameraManager.h"

#include "SPGameObject.h"

#include "SPStage.h"
#include "SPStageManager.h"
#include "SPTerrainAttribute.h"

#include "SPAvatarModelUnitDef.h"
#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPGOBStatus.h"
#include "SPPlayerStatus.h"
#include "SPPlayerStatusModel.h"
#include "SPGOBCoordPhysics.h"
#include "SPPlayer.h"

//////////////////////////////////////////////////////////////////////////
// TODO : TestTrigger전용 헤더..
#include <WinSock2.h>
#include "PacketID.h"
#include "Packet.h"
#include "SPNetworkManager.h"
#include "PacketHandler.h"

#include "SPCommandConvert.h"


//////////////////////////////////////////////////////////////////////////

#include "SPJoyPadManager.h"

#include "SPInputManager.h"


SPInputManager* g_pInputManager = NULL;

SPInputManager::SPInputManager()
: SPManager()
{
	SetMgrID(MGR_ID_INPUT);
	m_bActive = true;
	m_pJoyPadManager = NULL;
}

SPInputManager::~SPInputManager()
{
	SAFE_DELETE(m_pJoyPadManager);
	
	g_pInputManager = NULL;
}

bool SPInputManager::Init()
{
	SPManager::Init();
	m_focus = INPUT_FOCUS_NULL;
	g_pInputManager = this;	
	
	m_pJoyPadManager = new SPJoyPadManager;

	UpdateInput(); // force update
	UpdateInputJoyStick();

	bool bRet;
	 
	bRet = SetActionMap(KEY_AM_LEFT				,	SPKL_LEFT	); assert(bRet);
	bRet = SetActionMap(KEY_AM_RIGHT			,	SPKL_RIGHT	); assert(bRet);
	bRet = SetActionMap(KEY_AM_UP				,	SPKL_UP		); assert(bRet);
	bRet = SetActionMap(KEY_AM_DOWN				,	SPKL_DOWN	); assert(bRet);
	bRet = SetActionMap(KEY_AM_SWAP				,	SPKL_TAB	); assert(bRet);

	return true;
}


bool SPInputManager::SetActionMap(KEY_ACTION_MAP KeyActionMap, SPKeyList KeyList)
{
	for(int i = 0; i < MAX_USEKEY; ++i) {
		if(m_KeyBinder.ik[i].Key == KeyList) {
			m_mActionMap.insert ( std::map<KEY_ACTION_MAP, SPMInputKey*>::value_type( KeyActionMap, &(m_KeyBinder.ik[i])));
			return true;
		}
	}
	return false;
}

SPMInputKey* SPInputManager::GetActionMap(KEY_ACTION_MAP KeyActionMap)
{
	std::map<KEY_ACTION_MAP, SPMInputKey*>::iterator Iter = m_mActionMap.find( KeyActionMap );
	if(Iter != m_mActionMap.end()) {
		return (*Iter).second;
	}
	return NULL;
}

bool SPInputManager::SetActionMapJS(KEY_ACTION_MAP KeyActionMap, SPBtnList JSButtonList,int iIndex)
{
	for(int i = 0; i < MAX_USEJSBUTTON; ++i) {
		if(m_JSBinder.ib[i].iButton == JSButtonList && m_JSBinder.ib[i].iIndex == iIndex) {
			m_mActionMapJS.insert ( std::map<KEY_ACTION_MAP, SPMInputBtn*>::value_type( KeyActionMap, &(m_JSBinder.ib[i])));
			return true;
		}
	}
	return false;
}

SPMInputBtn* SPInputManager::GetActionMapJS(KEY_ACTION_MAP KeyActionMap)
{
	std::map<KEY_ACTION_MAP, SPMInputBtn*>::iterator Iter = m_mActionMapJS.find( KeyActionMap );
	if(Iter != m_mActionMapJS.end()) {
		return (*Iter).second;
	}
	return NULL;
}


SPJoyPadManager* SPInputManager::GetJoyPadManager()
{
	return m_pJoyPadManager;
}


void SPInputManager::SetJoyPadKeyName()
{
	if(m_pJoyPadManager)
		m_pJoyPadManager->SetKeyList();
}


void SPInputManager::Clean()
{	
	SPManager::Clean();
}

void SPInputManager::SetFocus(INPUT_FOCUS focus)
{
	m_focus = focus;
}

SPInputStruct* SPInputManager::GetInputStruct()
{
	//return &m_InputStruct;
	return &(m_KeyBinder.InputStruct);
}

SPInputStructJoyStick*		SPInputManager::GetInputStructJoyStick()
{
	return &(m_JSBinder.InputStruct);
}

void SPInputManager::Active()
{
	m_bActive = true;
}

void SPInputManager::Deactive()
{
	m_bActive = false;
	ZeroMemory(&m_KeyBinder, sizeof(m_KeyBinder));
	ZeroMemory(&m_JSBinder, sizeof(m_JSBinder));
}

bool SPInputManager::IsActive()
{
	return m_bActive;
}


void SPInputManager::Process(float fTime)
{
	SPManager::Process(fTime);	
	
	UpdateInput();
	UpdateInputJoyStick();
	
	m_pJoyPadManager->Process(fTime);
	
	// Client Input 
	CameraTrigger();
	//PlayerTrigger();
	
	// Packet Test
	TestTrigger();

}


void SPInputManager::CameraTrigger()
{
	//	AJJIYA [9/6/2005]
	//	이모티콘에서 F1 ~ F10 까지 단축키 사용함에따라 기존에 사용중인거 변경
	//	LSHIFT + 해당키!!

#if defined(_DEBUG) || defined(DEBUG)
	if( GetInputStruct()->ikLShift.InputState == INPUT_PRESS_REPEAT )
	{
		if(GetInputStruct()->ikF3.InputState == INPUT_PRESS) {
			if( m_pMgrCamera->GetCameraMode() == SPCameraManager::FOCUS_FREE_CAMERA)
				m_pMgrCamera->SetCameraMode(SPCameraManager::FOCUS_CHAR);
			else {		
				if( m_pMgrCamera->GetCameraMode() == SPCameraManager::FOCUS_CHAR)
					m_pMgrCamera->SetCameraMode(SPCameraManager::FOCUS_FREE_CAMERA);
			}		
			
			PlayerTrigger();
		}	
	}
#endif
}


void SPInputManager::UpdateInput()
{
	if(m_bActive == false)
		return;
	
	if ( g_pCIGWLoaderDx9 )
	{
		if ( g_pCIGWLoaderDx9->GetFocus() == true )
		{
			return;
		}
	}
	
	if(FAILED(g_pInput->ReadImmediateData()))
		return;
	

	SPInputStruct* pInputStruct;

	pInputStruct = GetInputStruct();
	g_pInput->KeyMsg(SPKL_UP			,pInputStruct->ikArrowUp		);
	g_pInput->KeyMsg(SPKL_DOWN			,pInputStruct->ikArrowDown		);
	g_pInput->KeyMsg(SPKL_LEFT			,pInputStruct->ikArrowLeft		);
	g_pInput->KeyMsg(SPKL_RIGHT			,pInputStruct->ikArrowRight		);
										 
	g_pInput->KeyMsg(SPKL_PGUP			,pInputStruct->ikPageUP			);
	g_pInput->KeyMsg(SPKL_PGDN			,pInputStruct->ikPageDown		);	
	g_pInput->KeyMsg(SPKL_LSHIFT		,pInputStruct->ikLShift			);
	g_pInput->KeyMsg(SPKL_GRAVE			,pInputStruct->ikGrave			);
	g_pInput->KeyMsg(SPKL_APOSTROPHE	,pInputStruct->ikApostrophe		);	
	g_pInput->KeyMsg(SPKL_SPACE			,pInputStruct->ikSpaceBar		);	
	g_pInput->KeyMsg(SPKL_CTRL			,pInputStruct->ikCtrl			);	
	g_pInput->KeyMsg(SPKL_ENTER			,pInputStruct->ikReturn			);		
	g_pInput->KeyMsg(SPKL_TAB			,pInputStruct->ikTab			);
	g_pInput->KeyMsg(SPKL_ESCAPE		,pInputStruct->ikEscape			);
	g_pInput->KeyMsg(SPKL_LALT			,pInputStruct->ikLAlt			);
	g_pInput->KeyMsg(SPKL_LCTRL			,pInputStruct->ikLCtrl			);
	g_pInput->KeyMsg(SPKL_A				,pInputStruct->ikA				);
	g_pInput->KeyMsg(SPKL_B				,pInputStruct->ikB				);	
	g_pInput->KeyMsg(SPKL_C				,pInputStruct->ikC				);
	g_pInput->KeyMsg(SPKL_D				,pInputStruct->ikD				);
	g_pInput->KeyMsg(SPKL_E				,pInputStruct->ikE				);	
	g_pInput->KeyMsg(SPKL_F				,pInputStruct->ikF				);	
	g_pInput->KeyMsg(SPKL_G				,pInputStruct->ikG				);	
	g_pInput->KeyMsg(SPKL_H				,pInputStruct->ikH				);		
	g_pInput->KeyMsg(SPKL_I				,pInputStruct->ikI				);
	g_pInput->KeyMsg(SPKL_J				,pInputStruct->ikJ				);	
	g_pInput->KeyMsg(SPKL_K				,pInputStruct->ikK				);
	g_pInput->KeyMsg(SPKL_L				,pInputStruct->ikL				);
	g_pInput->KeyMsg(SPKL_M				,pInputStruct->ikM				);	
	g_pInput->KeyMsg(SPKL_N				,pInputStruct->ikN				);	
	g_pInput->KeyMsg(SPKL_O				,pInputStruct->ikO				);	
	g_pInput->KeyMsg(SPKL_P				,pInputStruct->ikP				);		
	g_pInput->KeyMsg(SPKL_Q				,pInputStruct->ikQ				);
	g_pInput->KeyMsg(SPKL_R				,pInputStruct->ikR				);	
	g_pInput->KeyMsg(SPKL_S				,pInputStruct->ikS				);
	g_pInput->KeyMsg(SPKL_T				,pInputStruct->ikT				);
	g_pInput->KeyMsg(SPKL_U				,pInputStruct->ikU				);	
	g_pInput->KeyMsg(SPKL_V				,pInputStruct->ikV				);	
	g_pInput->KeyMsg(SPKL_W				,pInputStruct->ikW				);	
	g_pInput->KeyMsg(SPKL_X				,pInputStruct->ikX				);		
	g_pInput->KeyMsg(SPKL_Y				,pInputStruct->ikY				);
	g_pInput->KeyMsg(SPKL_Z				,pInputStruct->ikZ				);

	g_pInput->KeyMsg(SPKL_F1			,pInputStruct->ikF1				);
	g_pInput->KeyMsg(SPKL_F2			,pInputStruct->ikF2				);
	g_pInput->KeyMsg(SPKL_F3			,pInputStruct->ikF3				);
	g_pInput->KeyMsg(SPKL_F4			,pInputStruct->ikF4				);
	g_pInput->KeyMsg(SPKL_F5			,pInputStruct->ikF5				);
	g_pInput->KeyMsg(SPKL_F6			,pInputStruct->ikF6				);
	g_pInput->KeyMsg(SPKL_F7			,pInputStruct->ikF7				);
	g_pInput->KeyMsg(SPKL_F8			,pInputStruct->ikF8				);
	g_pInput->KeyMsg(SPKL_F9			,pInputStruct->ikF9				);
	g_pInput->KeyMsg(SPKL_F10			,pInputStruct->ikF10			);
	g_pInput->KeyMsg(SPKL_F11			,pInputStruct->ikF11			);
	g_pInput->KeyMsg(SPKL_F12			,pInputStruct->ikF12			);

	g_pInput->KeyMsg(SPKL_1				,pInputStruct->ik1				);
	g_pInput->KeyMsg(SPKL_2				,pInputStruct->ik2				);
	g_pInput->KeyMsg(SPKL_3				,pInputStruct->ik3				);
	g_pInput->KeyMsg(SPKL_4				,pInputStruct->ik4				);
	g_pInput->KeyMsg(SPKL_5				,pInputStruct->ik5				);
	g_pInput->KeyMsg(SPKL_6				,pInputStruct->ik6				);
	g_pInput->KeyMsg(SPKL_7				,pInputStruct->ik7				);
	g_pInput->KeyMsg(SPKL_8				,pInputStruct->ik8				);
	g_pInput->KeyMsg(SPKL_9				,pInputStruct->ik9				);
	g_pInput->KeyMsg(SPKL_0				,pInputStruct->ik0				);
	g_pInput->KeyMsg(SPKL_9				,pInputStruct->ik9				);
	g_pInput->KeyMsg(SPKL_MINUS			,pInputStruct->ikMinus			);
	g_pInput->KeyMsg(SPKL_EQUALS		,pInputStruct->ikEquals			);

	g_pInput->KeyMsg(SPKL_NUMPAD0		,pInputStruct->ikNumPad0		);
	g_pInput->KeyMsg(SPKL_NUMPAD1		,pInputStruct->ikNumPad1		);
	g_pInput->KeyMsg(SPKL_NUMPAD2		,pInputStruct->ikNumPad2		);
	g_pInput->KeyMsg(SPKL_NUMPAD3		,pInputStruct->ikNumPad3		);
	g_pInput->KeyMsg(SPKL_NUMPAD4		,pInputStruct->ikNumPad4		);
	g_pInput->KeyMsg(SPKL_NUMPAD5		,pInputStruct->ikNumPad5		);
	g_pInput->KeyMsg(SPKL_NUMPAD6		,pInputStruct->ikNumPad6		);
	g_pInput->KeyMsg(SPKL_NUMPAD7		,pInputStruct->ikNumPad7		);
	g_pInput->KeyMsg(SPKL_NUMPAD8		,pInputStruct->ikNumPad8		);
	g_pInput->KeyMsg(SPKL_NUMPAD9		,pInputStruct->ikNumPad9		);
	g_pInput->KeyMsg(SPKL_PREVTRACK		,pInputStruct->ikPrevTrack		);


	if(FAILED(g_pInput->ReadImmediateDataJoyStick()))
		return;
}

void SPInputManager::UpdateInputJoyStick()
{
	if(FAILED(g_pInput->ReadImmediateDataJoyStick())) {		
		return;
	}

	// Todo: bind used button here
	SPInputStructJoyStick* pInputStruct;
	pInputStruct = GetInputStructJoyStick();

    g_pInput->JSButtonMsg(SPBL_LX,				pInputStruct->ibLX);	
	g_pInput->JSButtonMsg(SPBL_LY,				pInputStruct->ibLY);
	g_pInput->JSButtonMsg(SPBL_LZ,				pInputStruct->ibLZ);
	g_pInput->JSButtonMsg(SPBL_LRZ,				pInputStruct->ibLRz);
	g_pInput->JSButtonMsg(SPBL_RGDWPOV,			pInputStruct->ibRgdwPOV[0]);
	//g_pInput->JSButtonMsg(SPBL_RGDWPOV,			pInputStruct->ibRgdwPOV[1], 1);
	
	for(int i = 0; i < 12; i++) {
		g_pInput->JSButtonMsg(SPBL_RGBBUTTONS,		pInputStruct->ibRgbButtons[i], i);
	}	
}



void SPInputManager::PlayerTrigger()
{
	SPPlayer* pPlayer = g_pGOBManager->GetLocalPlayer();
	if( m_pMgrCamera->GetCameraMode() == SPCameraManager::FOCUS_CHAR) {		
		if(pPlayer) {
			pPlayer->TriggerEnable(true);
		}
	} else {
		if(pPlayer) {
			pPlayer->TriggerEnable(false);
		}		
	}
}

void SPInputManager::TestTrigger()
{
	if(!g_pGOBManager->GetLocalPlayer()) return;

	//	AJJIYA [9/6/2005]
	//	이모티콘에서 F1 ~ F10 까지 단축키 사용함에따라 기존에 사용중인거 변경
	//	LSHIFT + 해당키!!

#if defined(_DEBUG) || defined(DEBUG)
	if( GetInputStruct()->ikLShift.InputState == INPUT_PRESS_REPEAT )
	{
		if(GetInputStruct()->ikF7.InputState == INPUT_PRESS) 
		{
			//////////////////////////////////////////////////////////////////////////
			// TODO : 테스트 전용 코드
			CPacket packet;
			packet.AddUINT32(CHEAT_CS_SETPOSITION);

			TCHAR	charName[LEN_NAME] = {0,};
			LT_POSITION		pos = {0,};

			strcpy(charName, g_pGOBManager->GetLocalPlayer()->GetStatus()->GetGOBName());
			packet.AddData(charName, LEN_NAME);

			pos.fPosX = 100.f;
			pos.fPosY = 100.f;

			packet.AddData(&pos, sizeof(LT_POSITION));
			(((SPNetworkManager*)g_pNetworkManager)->GetPacketHandler())->PostMsg(&packet);

			//////////////////////////////////////////////////////////////////////////
		} else if(GetInputStruct()->ikF8.InputState == INPUT_PRESS) {

			CPacket packet;
			packet.AddUINT32(CHEAT_CS_SETPOSITION);

			TCHAR	charName[LEN_NAME] = {0,};
			LT_POSITION		pos = {0,};

			strcpy(charName, g_pGOBManager->GetLocalPlayer()->GetStatus()->GetGOBName());
			packet.AddData(charName, LEN_NAME);
			
			pos.fPosX = 2500.f;
			pos.fPosY = 100.f;
			pos.eFacing = FACING_RIGHT;

			packet.AddData(&pos, sizeof(LT_POSITION));
			(((SPNetworkManager*)g_pNetworkManager)->GetPacketHandler())->PostMsg(&packet);

		} else if(GetInputStruct()->ikF9.InputState == INPUT_PRESS) {

			CPacket packet;
			packet.AddUINT32(CHEAT_CS_SETPOSITION);

			TCHAR	charName[LEN_NAME] = {0,};
			LT_POSITION		pos = {0,};

			strcpy(charName, g_pGOBManager->GetLocalPlayer()->GetStatus()->GetGOBName());
			packet.AddData(charName, LEN_NAME);

			pos.fPosX = 7000.f;
			pos.fPosY = 100.f;
			pos.eFacing = FACING_RIGHT;

			packet.AddData(&pos, sizeof(LT_POSITION));
			(((SPNetworkManager*)g_pNetworkManager)->GetPacketHandler())->PostMsg(&packet);

		}
		else if(GetInputStruct()->ik1.InputState == INPUT_PRESS) {

			CPacket packet;
			packet.AddUINT32(CASHSHOP_CS_COUPON);

			UINT8 len;
			TCHAR szCouponCode[50] = "1300120165947490";		// Milage Coupon

			len = _tcslen(szCouponCode)+1;

			packet.AddUINT8(len);
			packet.AddData(szCouponCode, len);

			(((SPNetworkManager*)g_pNetworkManager)->GetPacketHandler())->PostMsg(&packet);

		}
		else if(GetInputStruct()->ik2.InputState == INPUT_PRESS) {

			CPacket packet;
			packet.AddUINT32(CASHSHOP_CS_COUPON);

			UINT8 len;
			TCHAR szCouponCode[50] = "1400143678983028";		// Item Coupon

			len = _tcslen(szCouponCode)+1;

			packet.AddUINT8(len);
			packet.AddData(szCouponCode, len);

			(((SPNetworkManager*)g_pNetworkManager)->GetPacketHandler())->PostMsg(&packet);

		}
	}
#endif
}
