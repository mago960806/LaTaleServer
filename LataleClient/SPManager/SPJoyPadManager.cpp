
//
#include "SPCommon.h"
//#include "SPManagerDef.h"
//#include "SPManager.h"
#include "windows.h"

#include "SPStream.h"

#include "SPDebug.h"
#include "SPLocalizeManager.h"

#include "SPJoyPadManager.h"


extern bool g_bWindowed;



SPJoyPadManager::SPJoyPadManager()
{	
	Init();
}


SPJoyPadManager::~SPJoyPadManager()
{
	Clean();
}


void SPJoyPadManager::Process(float fTime)
{
	//SPManager::Process(fTime);
	if(!g_pInput->IsEquipJoystick()) 		return;

	SPInputStructJoyStick* pStick = g_pInputManager->GetInputStructJoyStick();
	if(pStick == NULL)						return;
	
	if(m_bButtonSetMode)					return;

	std::vector<SPJoyPadButtonInfo*>::iterator	iter;
	int iButtonMajor = PAD_BUTTON_NO_MAX;
	int iButtonMinor = PAD_BUTTON_NO_MAX;
	
	m_bFuncCheckPush1 = false;
	m_bFuncCheckPush2 = false;
	if((m_pPadInfo->m_iFunc[BUTTON_FUNC_0] != PAD_BUTTON_NO_MAX)) {
		if(pStick->ibRgbButtons[m_pPadInfo->m_iFunc[BUTTON_FUNC_0]].iValue)
			m_bFuncCheckPush1 = true;
	}

	if((m_pPadInfo->m_iFunc[BUTTON_FUNC_1] != PAD_BUTTON_NO_MAX)) {
		if(pStick->ibRgbButtons[m_pPadInfo->m_iFunc[BUTTON_FUNC_1]].iValue)
			m_bFuncCheckPush2 = true;
	}	
	
	if(m_bFuncCheckPush1 == false && m_bFuncCheckPush2 == false) {
		iter = m_pPadInfo->m_vpButtonSetting[PAD_BUTTONTYPE_NORMAL].begin();
		for(; iter != m_pPadInfo->m_vpButtonSetting[PAD_BUTTONTYPE_NORMAL].end(); ++iter) {
			if((*iter)->m_iKey < BUTTON_KEY_MAX) {
				iButtonMajor = (*iter)->m_iButtonMajor;
				if(pStick->ibRgbButtons[iButtonMajor].iValue) {
					(*iter)->m_iInputState = INPUT_REPEAT;
					//DXUTOutputDebugString("\t * + Update Button Push[%d] (%d)\n", iButtonMajor, pStick->ibRgbButtons[iButtonMajor].iValue);
				}
				else {
					(*iter)->m_iInputState = INPUT_UNREPEAT;
					//DXUTOutputDebugString("\t * - Update Button UnPush[%d] (%d)\n", iButtonMajor, pStick->ibRgbButtons[iButtonMajor].iValue);
				}
			}
		}
	}	

	iter = m_pPadInfo->m_vpButtonSetting[PAD_BUTTONTYPE_FUNC1].begin();
	for(; iter != m_pPadInfo->m_vpButtonSetting[PAD_BUTTONTYPE_FUNC1].end(); ++iter) {
		if((*iter)->m_iKey < BUTTON_KEY_MAX) {
			iButtonMajor = (*iter)->m_iButtonMajor;
			iButtonMinor = (*iter)->m_iButtonMinor;
			if(pStick->ibRgbButtons[iButtonMajor].iValue && pStick->ibRgbButtons[iButtonMinor].iValue) {
				(*iter)->m_iInputState = INPUT_REPEAT;
			}
			else{
				(*iter)->m_iInputState = INPUT_UNREPEAT;
			}
		}
	}
	
	iter = m_pPadInfo->m_vpButtonSetting[PAD_BUTTONTYPE_FUNC2].begin();
	for(; iter != m_pPadInfo->m_vpButtonSetting[PAD_BUTTONTYPE_FUNC2].end(); ++iter) {
		if((*iter)->m_iKey < BUTTON_KEY_MAX) {
			iButtonMajor = (*iter)->m_iButtonMajor;
			iButtonMinor = (*iter)->m_iButtonMinor;
			if(pStick->ibRgbButtons[iButtonMajor].iValue && pStick->ibRgbButtons[iButtonMinor].iValue) {
				(*iter)->m_iInputState = INPUT_REPEAT;
			}
			else{
				(*iter)->m_iInputState = INPUT_UNREPEAT;
			}
		}
	}
}


void SPJoyPadManager::Render(float fTime)
{
	//SPManager::Render(fTime);
}


bool SPJoyPadManager::Init()
{
	//SPManager::Init();

	for(int i = 0; i < BUTTON_KEY_MAX + 1; i++) {
		m_strKeyList[i].clear();
	}

	m_pPadInfo = new SPPadSettingInfo;
	m_bButtonSetMode = false;
	m_iSelect = JOYSTICK_SELECT_NULL;
	m_bKeyPass = false;
	m_bMouseMove = false;
	m_iFixWidth = 0;
	m_iFixHeight = 0;

	m_pPadInfo->m_iJoyStickMode = JOYSTICK_MODE_DIGITAL;

	m_pPadInfo->m_iFunc[BUTTON_FUNC_0]		= PAD_BUTTON_NO_MAX;
	m_pPadInfo->m_iFunc[BUTTON_FUNC_1]		= PAD_BUTTON_NO_MAX;
	m_pPadInfo->m_iMouse[BUTTON_MOUSE_L]	= PAD_BUTTON_NO_MAX;
	m_pPadInfo->m_iMouse[BUTTON_MOUSE_R]	= PAD_BUTTON_NO_MAX;
	
	if(Load() == false)	
		SetDefault();

	UpdateButton();		
	if(g_bWindowed) {
		bool bRet = false;
		TITLEBARINFO TitleInfo;
		TitleInfo.cbSize = sizeof(TitleInfo);
		bRet = ::GetTitleBarInfo(g_hWnd, &TitleInfo);				
		m_iFixHeight = TitleInfo.rcTitleBar.bottom - TitleInfo.rcTitleBar.top;
		m_iFixWidth = ::GetSystemMetrics(SM_CXFIXEDFRAME);
		m_iFixHeight += ::GetSystemMetrics(SM_CYFIXEDFRAME);
	}
	
	return true;
}


void SPJoyPadManager::Clean()
{		
#ifdef _JOY_PAD
	if(SPLocalizeManager::GetInstance()->IsEnableValue(LV_JOYPAD)) {
		Save();
	}
#endif
	
	SAFE_DELETE(m_pPadInfo);
	DeleteAllButton();
}


void SPJoyPadManager::Save()
{
	SPStream* pFileStream = new SPFileStream(PAD_SETTING_FILE, SPFileStream::OPEN_WRITE);
	if(pFileStream->Valid() == false) {		
		pFileStream->Release();
		DXUTOutputDebugString("PadSetting Save File Crate Fail...\n");
		return;
	}
	pFileStream->Write( PAD_SETTING_FILE, strlen(PAD_SETTING_FILE));
	pFileStream->Write( &PAD_SETTING_VER, sizeof(int));

	pFileStream->Write( &m_pPadInfo->m_iJoyStickMode, sizeof(int));

	int iSize = 0;
	int i = 0;	
	for(i = 0; i < PAD_BUTTONTYPE_MAX; i++) {
		iSize += static_cast<int>(m_pPadInfo->m_vpButtonSetting[i].size());
	}

	pFileStream->Write(&iSize , sizeof(int));

	std::vector<SPJoyPadButtonInfo*>::iterator iter;
	for(i = 0; i < PAD_BUTTONTYPE_MAX; i++) {
		iter = m_pPadInfo->m_vpButtonSetting[i].begin();
		for(; iter != m_pPadInfo->m_vpButtonSetting[i].end(); ++iter) {
			pFileStream->Write((*iter), sizeof(SPJoyPadButtonInfo));
		}
	}
	
	pFileStream->Release();

	DXUTOutputDebugString("PadSetting Save OK ...\n");
}


bool SPJoyPadManager::Load()
{
	char FileName[128];
	std::string strFile = PAD_SETTING_FILE;
	int iVer;

	SPStream* pFileStream = new SPFileStream(PAD_SETTING_FILE, SPFileStream::OPEN_READ);
	if(pFileStream->Valid() == false) {
		pFileStream->Release();
		return false;
	}

	pFileStream->Read( FileName, strlen(PAD_SETTING_FILE));
	FileName[strlen(PAD_SETTING_FILE)] = 0;
	if(strFile != FileName) {
		pFileStream->Release();
		return false;
	}

	pFileStream->Read( &iVer, sizeof(int));
	if(iVer != PAD_SETTING_VER) {
		pFileStream->Release();
		return false;
	}

	pFileStream->Read( &m_pPadInfo->m_iJoyStickMode, sizeof(int));

	int iSize = 0;
	int i = 0;	
	for(i = 0; i < PAD_BUTTONTYPE_MAX; i++) {
		iSize += static_cast<int>(m_pPadInfo->m_vpButtonSetting[i].size());
	}

	int iNo;
	pFileStream->Read( &iNo, sizeof(int));
	if(iNo > iSize) {
		pFileStream->Release();
		return false;
	}	

	int iType = 0;
	int iIndex = 0;
	for(int i = 0; i < iNo; i++) {
		//if(iIndex >= PAD_BUTTON_NO_MAX)	continue;
		
		SPJoyPadButtonInfo* pInfo = m_pPadInfo->m_vpButtonSetting[iType].at(iIndex);
		if(pInfo == NULL)		continue;
		
		pFileStream->Read(pInfo, sizeof(SPJoyPadButtonInfo));
		
		if(i < PAD_BUTTON_NO_MAX) {
			if(pInfo->m_iUse == PAD_BUTTON_USE_FUNC) {
				if(m_pPadInfo->m_iFunc[BUTTON_FUNC_0] == PAD_BUTTON_NO_MAX) {
					m_pPadInfo->m_iFunc[BUTTON_FUNC_0] = pInfo->m_iButtonMajor;						
					iIndex++;
					continue;
				}				
				else if(m_pPadInfo->m_iFunc[BUTTON_FUNC_1]  == PAD_BUTTON_NO_MAX) {
					m_pPadInfo->m_iFunc[BUTTON_FUNC_1] = pInfo->m_iButtonMajor;
					iIndex++;
					continue;
				}
			}

			if(pInfo->m_iKey == BUTTON_KEY_MOUSE_L) 
				m_pPadInfo->m_iMouse[BUTTON_MOUSE_L] = pInfo->m_iButtonMajor;
			if(pInfo->m_iKey == BUTTON_KEY_MOUSE_R)
				m_pPadInfo->m_iMouse[BUTTON_MOUSE_R] = pInfo->m_iButtonMajor;
		}	

		if(i == PAD_BUTTON_NO_MAX - 1 || i == (PAD_BUTTON_NO_MAX * 2 - 1)) {
			iType++;
			iIndex = 0;
		}
		else {
			iIndex++;
		}
	}	

	pFileStream->Release();
	
	return true;
}


void SPJoyPadManager::SetJoyStickMode(JOYSTICK_MODE iMode)
{
	if(m_pPadInfo)
		m_pPadInfo->m_iJoyStickMode = iMode;
}


JOYSTICK_MODE SPJoyPadManager::GetJoyStickMode()
{
	if(m_pPadInfo)
		return m_pPadInfo->m_iJoyStickMode;
	
	return JOYSTICK_MODE_NULL;
}


SPPadSettingInfo* SPJoyPadManager::GetPadSettingInfo()
{
	if(m_pPadInfo)
		return m_pPadInfo;

	return NULL;
}



std::vector<SPJoyPadButtonInfo*>* SPJoyPadManager::GetPadButton(PAD_BUTTON_KEY_TYPE iType)
{
	return &m_vpButton[iType];
}


void SPJoyPadManager::ClearAllSettingInfo()
{
	if(m_pPadInfo)
		m_pPadInfo->ClearAllButtonInfo();
}



void SPJoyPadManager::DeleteAllButton()
{	
	for(int i = 0; i < PAD_BUTTON_KEY_TYPE_MAX; i++) {
		m_vpButton[i].clear();
	}
	
	////
	//std::vector<SPJoyPadButtonInfo*>::iterator iter;	
	//for(int i = 0; i < PAD_BUTTON_KEY_TYPE_MAX; i++) {
	//	iter = m_vpButton[i].begin();
	//	for(; iter != m_vpButton[i].end(); ++iter) {
	//		delete (*iter);
	//	}
	//	m_vpButton[i].clear();
	//}
}


void SPJoyPadManager::SetDefault(SPPadSettingInfo* pInfo /*= NULL*/)
{	
	bool bRet = true;

	if(m_pPadInfo == NULL)
		return;
	
	if(pInfo == NULL && m_pPadInfo)
		pInfo = m_pPadInfo;
	
	pInfo->Clear();
	pInfo->ClearAllButtonInfo();

	pInfo->m_iJoyStickMode = JOYSTICK_MODE_DIGITAL;

	bRet &= pInfo->SetNormalInfo(PAD_BUTTON_NO_0, PAD_BUTTON_USE_KEY, BUTTON_KEY_QUICK_0);
	bRet &= pInfo->SetNormalInfo(PAD_BUTTON_NO_1, PAD_BUTTON_USE_KEY, BUTTON_KEY_QUICK_1);
	bRet &= pInfo->SetNormalInfo(PAD_BUTTON_NO_2, PAD_BUTTON_USE_KEY, BUTTON_KEY_QUICK_2);
	bRet &= pInfo->SetNormalInfo(PAD_BUTTON_NO_3, PAD_BUTTON_USE_KEY, BUTTON_KEY_QUICK_3);
	bRet &= pInfo->SetNormalInfo(PAD_BUTTON_NO_4, PAD_BUTTON_USE_KEY, BUTTON_KEY_QUICK_4);
	bRet &= pInfo->SetNormalInfo(PAD_BUTTON_NO_5, PAD_BUTTON_USE_KEY, BUTTON_KEY_QUICK_5);
	bRet &= pInfo->SetNormalInfo(PAD_BUTTON_NO_6, PAD_BUTTON_USE_KEY, BUTTON_KEY_QUICK_6);
	bRet &= pInfo->SetNormalInfo(PAD_BUTTON_NO_7, PAD_BUTTON_USE_KEY, BUTTON_KEY_QUICK_7);
	bRet &= pInfo->SetNormalInfo(PAD_BUTTON_NO_8, PAD_BUTTON_USE_KEY, BUTTON_KEY_QUICK_8);
	bRet &= pInfo->SetNormalInfo(PAD_BUTTON_NO_9, PAD_BUTTON_USE_KEY, BUTTON_KEY_QUICK_9);
	bRet &= pInfo->SetNormalInfo(PAD_BUTTON_NO_10, PAD_BUTTON_USE_KEY, BUTTON_KEY_QUICK_10);
	bRet &= pInfo->SetNormalInfo(PAD_BUTTON_NO_11, PAD_BUTTON_USE_KEY, BUTTON_KEY_QUICK_11);		
}


void SPJoyPadManager::SetKeyList()
{
	m_strKeyList[BUTTON_KEY_QUICK_0]			= "Ctrl";
	m_strKeyList[BUTTON_KEY_QUICK_1]			= "A";
	m_strKeyList[BUTTON_KEY_QUICK_2]			= "S";
	m_strKeyList[BUTTON_KEY_QUICK_3]			= "D";
	m_strKeyList[BUTTON_KEY_QUICK_4]			= "F";
	m_strKeyList[BUTTON_KEY_QUICK_5]			= "Alt";
	m_strKeyList[BUTTON_KEY_QUICK_6]			= "Shift";
	m_strKeyList[BUTTON_KEY_QUICK_7]			= "Z";
	m_strKeyList[BUTTON_KEY_QUICK_8]			= "X";	
	m_strKeyList[BUTTON_KEY_QUICK_9]			= "C";	
	m_strKeyList[BUTTON_KEY_QUICK_10]			= "V";	
	m_strKeyList[BUTTON_KEY_QUICK_11]			= "Space";
	m_strKeyList[BUTTON_KEY_QUICK_S1]			= "1";	
	m_strKeyList[BUTTON_KEY_QUICK_S2]			= "2";	
	m_strKeyList[BUTTON_KEY_QUICK_S3]			= "3";	
	m_strKeyList[BUTTON_KEY_QUICK_S4]			= "4";	
	m_strKeyList[BUTTON_KEY_QUICK_S5]			= "5";	
	m_strKeyList[BUTTON_KEY_QUICK_S6]			= "6";	
	m_strKeyList[BUTTON_KEY_QUICK_S7]			= "7";	
	m_strKeyList[BUTTON_KEY_QUICK_S8]			= "8";	
	m_strKeyList[BUTTON_KEY_QUICK_S9]			= "9";	
	m_strKeyList[BUTTON_KEY_QUICK_S10]			= "0";	
	m_strKeyList[BUTTON_KEY_QUICK_S11]			= "-";	
	
	if(SPLocalizeManager::GetInstance()->GetCountryType() == CT_JAPAN)	
		m_strKeyList[BUTTON_KEY_QUICK_S12]		= "^";
	else
		m_strKeyList[BUTTON_KEY_QUICK_S12]		= "=";
		
	
	m_strKeyList[BUTTON_KEY_QUICK_F1]		= "F1";	
	m_strKeyList[BUTTON_KEY_QUICK_F2]		= "F2";

	m_strKeyList[BUTTON_KEY_INTERFACE_I]		= "I";	
	m_strKeyList[BUTTON_KEY_INTERFACE_E]		= "E";	
	m_strKeyList[BUTTON_KEY_INTERFACE_T]		= "T";	
	m_strKeyList[BUTTON_KEY_INTERFACE_K]		= "K";	
	m_strKeyList[BUTTON_KEY_INTERFACE_B]		= "B";	
	m_strKeyList[BUTTON_KEY_INTERFACE_Q]		= "Q";	
	m_strKeyList[BUTTON_KEY_INTERFACE_O]		= "O";	
	m_strKeyList[BUTTON_KEY_INTERFACE_P]		= "P";	
	m_strKeyList[BUTTON_KEY_INTERFACE_H]		= "H";	
	m_strKeyList[BUTTON_KEY_INTERFACE_W]		= "W";	
	m_strKeyList[BUTTON_KEY_INTERFACE_M]		= "M";	
	m_strKeyList[BUTTON_KEY_INTERFACE_N]		= "N";	
	m_strKeyList[BUTTON_KEY_INTERFACE_U]		= "U";	
	m_strKeyList[BUTTON_KEY_INTERFACE_Y]		= "Y";	
	m_strKeyList[BUTTON_KEY_INTERFACE_L]		= "L";	
	m_strKeyList[BUTTON_KEY_INTERFACE_G]		= "G";	
	m_strKeyList[BUTTON_KEY_INTERFACE_TAB]		= "Tab";	
	m_strKeyList[BUTTON_KEY_INTERFACE_ENTER]	= "Enter";
	m_strKeyList[BUTTON_KEY_INTERFACE_ESC]		= "Esc";	

	m_strKeyList[BUTTON_KEY_MOUSE_L]			= "MouseL";	
	m_strKeyList[BUTTON_KEY_MOUSE_R]			= "MouseR";	
	m_strKeyList[BUTTON_KEY_MAX]				= " ";
}


const char* SPJoyPadManager::GetKeyName(BUTTON_KEY iKey)
{
	if(iKey < BUTTON_KEY_QUICK_START || iKey > BUTTON_KEY_MAX) 
		return NULL;
	
	if(m_strKeyList[iKey].empty())
		return NULL;

	return m_strKeyList[iKey].c_str();
}


void SPJoyPadManager::UpdateButton()
{
	DeleteAllButton();
	
	int i = 0;
	std::vector<SPJoyPadButtonInfo*>::iterator IterSetting;

	for(i = 0; i < PAD_BUTTONTYPE_MAX; i++) {
		IterSetting = m_pPadInfo->m_vpButtonSetting[i].begin();
		for(; IterSetting != m_pPadInfo->m_vpButtonSetting[i].end(); ++IterSetting) {
			if((*IterSetting)->m_iKey >= BUTTON_KEY_QUICK_START && (*IterSetting)->m_iKey < BUTTON_KEY_INTERFACE_START) {
				m_vpButton[PAD_BUTTON_KEY_TYPE_QUICK].push_back((*IterSetting));
			}
			else if((*IterSetting)->m_iKey >= BUTTON_KEY_INTERFACE_START && (*IterSetting)->m_iKey < BUTTON_KEY_MOUSE_START) {
				m_vpButton[PAD_BUTTON_KEY_TYPE_INTERFACE].push_back((*IterSetting));
			}
			else if((*IterSetting)->m_iKey >= BUTTON_KEY_MOUSE_START && (*IterSetting)->m_iKey < BUTTON_KEY_MAX) {
				m_vpButton[PAD_BUTTON_KEY_TYPE_MOUSE].push_back((*IterSetting));
			}
			else {				
				DXUTOutputDebugString("\t * Can't Update Button [%d]\n", (*IterSetting)->m_iButtonMajor);
			}
		}
	}
}


