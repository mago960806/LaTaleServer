
//
#pragma once 


#define  _JOY_PAD				254

/**
 * 조이패드가 동작중인 모드 <br> 
*/
enum JOYSTICK_MODE {
	JOYSTICK_MODE_NULL				= 0,
	JOYSTICK_MODE_ANALOG			= 1,			/**< 아나로그 <br> */
	JOYSTICK_MODE_DIGITAL			= 2,			/**< 디지털 <br> */
};


/**
 * 버튼 사용 형식 구분 <br>
*/
enum PAD_BUTTONTYPE {
	PAD_BUTTONTYPE_NORMAL,							/**< 일반 버튼 <br> */
	PAD_BUTTONTYPE_FUNC1,							/**< 조합키 1 <br> */
	PAD_BUTTONTYPE_FUNC2,							/**< 조합키 2 <br> */
	PAD_BUTTONTYPE_MAX,
};


/**
 * 각 버튼 번호 <br> 
*/
enum PAD_BUTTON_NO {	
	PAD_BUTTON_NO_0							= 0,
	PAD_BUTTON_NO_1							= 1,
	PAD_BUTTON_NO_2							= 2,
	PAD_BUTTON_NO_3							= 3,
	PAD_BUTTON_NO_4							= 4,
	PAD_BUTTON_NO_5							= 5,
	PAD_BUTTON_NO_6							= 6,
	PAD_BUTTON_NO_7							= 7,
	PAD_BUTTON_NO_8							= 8,
	PAD_BUTTON_NO_9							= 9,
	PAD_BUTTON_NO_10						= 10,
	PAD_BUTTON_NO_11						= 11,	
	PAD_BUTTON_NO_MAX,
};


/**
 * 버튼별 용도 <br> 
*/
enum PAD_BUTTON_USE {
	PAD_BUTTON_USE_NULL,
	PAD_BUTTON_USE_KEY,								/**< 일반 <br> */
	PAD_BUTTON_USE_FUNC,							/**< 조합 <br> */
};


/**
 * 적용된 키에따른 구분 <br> 
*/
enum PAD_BUTTON_KEY_TYPE {
	PAD_BUTTON_KEY_TYPE_QUICK,						/**< 퀵슬롯 용 <br> */
	//PAD_BUTTON_KEY_TYPE_EMOTICON,					/**< 이모티콘 용 <br> */
	PAD_BUTTON_KEY_TYPE_INTERFACE,					/**< 인터페이스 용 <br> */
	PAD_BUTTON_KEY_TYPE_MOUSE,						/**< 마우스 용 <br> */
	PAD_BUTTON_KEY_TYPE_MAX,
};


/**
 * 대응되는 키 리스트 <br> 
*/
enum BUTTON_KEY {
	BUTTON_KEY_QUICK_START,
	BUTTON_KEY_QUICK_0			= BUTTON_KEY_QUICK_START,		//Ctrl
	BUTTON_KEY_QUICK_1,											//A
	BUTTON_KEY_QUICK_2,											//S
	BUTTON_KEY_QUICK_3,											//D
	BUTTON_KEY_QUICK_4,											//F
	BUTTON_KEY_QUICK_5,											//Alt
	BUTTON_KEY_QUICK_6,											//Shift Use With Looting 
	BUTTON_KEY_QUICK_7,											//Z
	BUTTON_KEY_QUICK_8,											//X
	BUTTON_KEY_QUICK_9,											//C
	BUTTON_KEY_QUICK_10,										//V
	BUTTON_KEY_QUICK_11,										//Space Use With Looting 
	BUTTON_KEY_QUICK_S1,										//1
	BUTTON_KEY_QUICK_S2,										//2
	BUTTON_KEY_QUICK_S3,										//3
	BUTTON_KEY_QUICK_S4,										//4
	BUTTON_KEY_QUICK_S5,										//5
	BUTTON_KEY_QUICK_S6,										//6
	BUTTON_KEY_QUICK_S7,										//7
	BUTTON_KEY_QUICK_S8,										//8
	BUTTON_KEY_QUICK_S9,										//9
	BUTTON_KEY_QUICK_S10,										//0
	BUTTON_KEY_QUICK_S11,										//-
	BUTTON_KEY_QUICK_S12,										//=
	BUTTON_KEY_QUICK_F1,										//F1
	BUTTON_KEY_QUICK_F2,										//F2

	/*
	BUTTON_KEY_EMOTICON_START,
	BUTTON_KEY_EMOTICON_0		= BUTTON_KEY_EMOTICON_START,	//F1
	BUTTON_KEY_EMOTICON_1,										//F2
	BUTTON_KEY_EMOTICON_2,										//F3
	BUTTON_KEY_EMOTICON_3,										//F4
	BUTTON_KEY_EMOTICON_4,										//F5
	BUTTON_KEY_EMOTICON_5,										//F6
	BUTTON_KEY_EMOTICON_6,										//F7
	BUTTON_KEY_EMOTICON_7,										//F8
	BUTTON_KEY_EMOTICON_8,										//F9
	BUTTON_KEY_EMOTICON_9,										//F10
	BUTTON_KEY_EMOTICON_10,										//F11
	BUTTON_KEY_EMOTICON_11,										//F12
	*/

	BUTTON_KEY_INTERFACE_START,
	BUTTON_KEY_INTERFACE_I		= BUTTON_KEY_INTERFACE_START,	//I Inven
	BUTTON_KEY_INTERFACE_E,										//E Equip
	BUTTON_KEY_INTERFACE_T,										//T State
	BUTTON_KEY_INTERFACE_K,										//K Skill
	BUTTON_KEY_INTERFACE_B,										//B Friend
	BUTTON_KEY_INTERFACE_Q,										//Q Quest
	BUTTON_KEY_INTERFACE_O,										//O Option
	BUTTON_KEY_INTERFACE_P,										//P Team
	BUTTON_KEY_INTERFACE_H,										//H Help
	BUTTON_KEY_INTERFACE_W,										//W World Map
	BUTTON_KEY_INTERFACE_M,										//M	Map Stage
	BUTTON_KEY_INTERFACE_N,										//N Enchant
	BUTTON_KEY_INTERFACE_U,										//U Upgrad
	BUTTON_KEY_INTERFACE_Y,										//Y Way Point
	BUTTON_KEY_INTERFACE_L,										//L Pet
	BUTTON_KEY_INTERFACE_G,										//G Guild
	BUTTON_KEY_INTERFACE_TAB,									//Tab
	//BUTTON_KEY_INTERFACE_SHIFT,
	BUTTON_KEY_INTERFACE_ENTER,									//Enter
	//BUTTON_KEY_INTERFACE_SPACE,
	BUTTON_KEY_INTERFACE_ESC,									//Esc

	BUTTON_KEY_MOUSE_START,
	BUTTON_KEY_MOUSE_L			= BUTTON_KEY_MOUSE_START,
	BUTTON_KEY_MOUSE_R,	
	BUTTON_KEY_MOUSE_MAX,
	BUTTON_KEY_MAX				= BUTTON_KEY_MOUSE_MAX,
};

////
//static std::string g_strKeyList[BUTTON_KEY_MAX + 1] = {
//	"Ctrl",					//BUTTON_KEY_QUICK_0			
//	"A",					//BUTTON_KEY_QUICK_1,
//	"S",					//BUTTON_KEY_QUICK_2,
//	"D",					//BUTTON_KEY_QUICK_3,
//	"F",					//BUTTON_KEY_QUICK_4,
//	"Alt",					//BUTTON_KEY_QUICK_5,
//	"Shift",				//BUTTON_KEY_QUICK_6,		//Use With Looting
//	"Z",					//BUTTON_KEY_QUICK_7,
//	"X",					//BUTTON_KEY_QUICK_8,
//	"C",					//BUTTON_KEY_QUICK_9,
//	"V",					//BUTTON_KEY_QUICK_10,
//	"Space",				//BUTTON_KEY_QUICK_11,		//Use With Looting
//	"1",					//BUTTON_KEY_QUICK_S1
//	"2",					//BUTTON_KEY_QUICK_S2
//	"3",					//BUTTON_KEY_QUICK_S3
//	"4",					//BUTTON_KEY_QUICK_S4
//	"5",					//BUTTON_KEY_QUICK_S5,
//	"6",					//BUTTON_KEY_QUICK_S6,
//	"7",					//BUTTON_KEY_QUICK_S7,
//	"8",					//BUTTON_KEY_QUICK_S8,
//	"9",					//BUTTON_KEY_QUICK_S9,
//	"10",					//BUTTON_KEY_QUICK_S10,
//	"-",					//BUTTON_KEY_QUICK_S11,
//	"=",					//BUTTON_KEY_QUICK_S12,
//	"F1",					//BUTTON_KEY_QUICK_F1,
//	"F2",					//BUTTON_KEY_QUICK_F2,
//	
//	/*
//	"F1",					//BUTTON_KEY_EMOTICON_0		
//	"F2",					//BUTTON_KEY_EMOTICON_1,
//	"F3",					//BUTTON_KEY_EMOTICON_2,
//	"F4",					//BUTTON_KEY_EMOTICON_3,
//	"F5",					//BUTTON_KEY_EMOTICON_4,
//	"F6",					//BUTTON_KEY_EMOTICON_5,
//	"F7",					//BUTTON_KEY_EMOTICON_6,
//	"F8",					//BUTTON_KEY_EMOTICON_7,
//	"F9",					//BUTTON_KEY_EMOTICON_8,
//	"F10",					//BUTTON_KEY_EMOTICON_9,
//	"F11",					//BUTTON_KEY_EMOTICON_10,
//	"F12",					//BUTTON_KEY_EMOTICON_11,	
//	*/
//
//	"I",					//BUTTON_KEY_INTERFACE_I		
//	"E",					//BUTTON_KEY_INTERFACE_E,		
//	"T",					//BUTTON_KEY_INTERFACE_T,		
//	"K",					//BUTTON_KEY_INTERFACE_K,		
//	"B",					//BUTTON_KEY_INTERFACE_B,		
//	"Q",					//BUTTON_KEY_INTERFACE_Q,		
//	"O",					//BUTTON_KEY_INTERFACE_O,		
//	"P",					//BUTTON_KEY_INTERFACE_P,		
//	"H",					//BUTTON_KEY_INTERFACE_H,		
//	"W",					//BUTTON_KEY_INTERFACE_W,		
//	"M",					//BUTTON_KEY_INTERFACE_M,		
//	"N",					//BUTTON_KEY_INTERFACE_N,		
//	"U",					//BUTTON_KEY_INTERFACE_U,		
//	"Y",					//BUTTON_KEY_INTERFACE_Y,		
//	"L",					//BUTTON_KEY_INTERFACE_L,		
//	"G",					//BUTTON_KEY_INTERFACE_G,		
//	"Tab",					//BUTTON_KEY_INTERFACE_TAB,
//	//"Shift",				//BUTTON_KEY_INTERFACE_SHIFT,
//	"Enter",				//BUTTON_KEY_INTERFACE_ENTER,
//	//"Space",				//BUTTON_KEY_INTERFACE_SPACE,
//	"Esc",					//BUTTON_KEY_INTERFACE_ESC,	
//	
//	"MouseL",				//BUTTON_KEY_MOUSE_L		
//	"MouseR",				//BUTTON_KEY_MOUSE_R,	
//	"NULL",					//BUTTON_KEY_MAX			
//};							/**< 대응되는 키 이름 <br> */


/**
 * 조합키 구분용 <br>
*/
enum BUTTON_FUNC {
	BUTTON_FUNC_0,					/**< 조합키 1 <br> */
	BUTTON_FUNC_1,					/**< 조합키 2 <br> */
	BUTTON_FUNC_MAX,
};


/**
 * 마우스 구분용 <br>
*/
enum BUTTON_MOUSE {
	BUTTON_MOUSE_L,					/**< 마우스 L 버튼 <br> */
	BUTTON_MOUSE_R,					/**< 마우스 R 버튼 <br> */
	BUTTON_MOUSE_MAX,
};


/**
 * 현재 인터페이스에서 변경하고자 선택한 키 <br>
*/
enum JOYSTICK_SELECT {
	JOYSTICK_SELECT_NULL,
	JOYSTICK_SELECT_FUNC1,								//조합1
	JOYSTICK_SELECT_FUNC2,								//조합2
	JOYSTICK_SELECT_MOUSEL,								//마우스 L
	JOYSTICK_SELECT_MOUSER,								//마우스 R

	JOYSTICK_SELECT_KEY_0,								//일반 키
	JOYSTICK_SELECT_KEY_1,
	JOYSTICK_SELECT_KEY_2,
	JOYSTICK_SELECT_KEY_3,
	JOYSTICK_SELECT_KEY_4,
	JOYSTICK_SELECT_KEY_5,
	JOYSTICK_SELECT_KEY_6,
	JOYSTICK_SELECT_KEY_7,
	JOYSTICK_SELECT_KEY_8,
	JOYSTICK_SELECT_KEY_9,
	JOYSTICK_SELECT_KEY_10,
	JOYSTICK_SELECT_KEY_11,

	JOYSTICK_SELECT_FUNC1_0,							//조합 키 1
	JOYSTICK_SELECT_FUNC1_1,
	JOYSTICK_SELECT_FUNC1_2,
	JOYSTICK_SELECT_FUNC1_3,
	JOYSTICK_SELECT_FUNC1_4,
	JOYSTICK_SELECT_FUNC1_5,
	JOYSTICK_SELECT_FUNC1_6,
	JOYSTICK_SELECT_FUNC1_7,
	JOYSTICK_SELECT_FUNC1_8,
	JOYSTICK_SELECT_FUNC1_9,
	JOYSTICK_SELECT_FUNC1_10,
	JOYSTICK_SELECT_FUNC1_11,

	JOYSTICK_SELECT_FUNC2_0,							//조합 키 2
	JOYSTICK_SELECT_FUNC2_1,
	JOYSTICK_SELECT_FUNC2_2,
	JOYSTICK_SELECT_FUNC2_3,
	JOYSTICK_SELECT_FUNC2_4,
	JOYSTICK_SELECT_FUNC2_5,
	JOYSTICK_SELECT_FUNC2_6,
	JOYSTICK_SELECT_FUNC2_7,
	JOYSTICK_SELECT_FUNC2_8,
	JOYSTICK_SELECT_FUNC2_9,
	JOYSTICK_SELECT_FUNC2_10,
	JOYSTICK_SELECT_FUNC2_11,
};


static char*	PAD_SETTING_FILE		= "PADSETTING.DAT";			/**< 설정을 저장할 파일 <br> */

/**
 * 1 - 기본
 * 2 - 조합버튼 변경
 * 3 - Push 상태 체크 추가
*/
static int		PAD_SETTING_VER			= 3;						/**< 저장 버전 <br> */


/**
 * 버튼 정보 <br>
*/
struct SPJoyPadButtonInfo {
	PAD_BUTTON_NO		m_iButtonMajor;				/**< 선 입력 키 <br> */
	PAD_BUTTON_USE		m_iUse;						/**< 사용 용도 <br> */
	PAD_BUTTON_NO		m_iButtonMinor;				/**< 후 입력 키 <br> */
	
	BUTTON_KEY			m_iKey;						/**< 대응 키 <br> */
	int					m_iInputState;				/**< 입력 상황 <br> */
	bool				m_bPush;					/**< 버튼이 계속 눌리고 있는지 판별시 사용 <br> 외부에서 설정 <br> */

	//
	SPJoyPadButtonInfo() {
		Clear();
	}

	~SPJoyPadButtonInfo() {

	}

	void Clear() {
		m_iButtonMajor	= PAD_BUTTON_NO_MAX;
		m_iUse			= PAD_BUTTON_USE_NULL;		
		m_iButtonMinor	= PAD_BUTTON_NO_MAX;
		m_iKey			= BUTTON_KEY_MAX;
		m_iInputState	= INPUT_UNPRESS;
		m_bPush			= false;
	}

	void SetInfo(PAD_BUTTON_NO iMajor, PAD_BUTTON_USE iUse, BUTTON_KEY iAction, PAD_BUTTON_NO iMinor = PAD_BUTTON_NO_MAX) {
		m_iButtonMajor	= iMajor;
		m_iUse			= iUse;		
		m_iButtonMinor	= iMinor;
		m_iKey			= iAction;
		m_iInputState	= INPUT_UNPRESS;
		m_bPush			= false;
	}

	bool CopyInfo(SPJoyPadButtonInfo* pSrc) {
		if(pSrc) {
			m_iButtonMajor	= pSrc->m_iButtonMajor;
			m_iUse			= pSrc->m_iUse;
			m_iButtonMinor	= pSrc->m_iButtonMinor;
			m_iKey			= pSrc->m_iKey;
			m_iInputState	= INPUT_UNPRESS;
			m_bPush			= false;

			return true;
		}
		return false;
	}	
};


/**
 * 버튼 정보를 포함한 조이패드 설정 정보 <br>
 * Manager와 Interface에 각각 한개씩 생성 관리 <br>
*/
class SPPadSettingInfo {
public:
	JOYSTICK_MODE			m_iJoyStickMode;

	/**
	* 버튼 별 키 설정 상태및 신호 업데이트 용 정보 36개 <br> 임시 저장용 <br>
	*/
	std::vector<SPJoyPadButtonInfo*>	m_vpButtonSetting[PAD_BUTTONTYPE_MAX];

	PAD_BUTTON_NO			m_iFunc[BUTTON_FUNC_MAX];			/**< 조합키로 설정된 키를 기억 <br> 임시 저장용 <br> */
	PAD_BUTTON_NO			m_iMouse[BUTTON_MOUSE_MAX];			/**< 마우스로 설정된 키를 기억 <br> 임시 저장용 <br>*/	


public:		
	SPPadSettingInfo() {
		int i = 0;
		SPJoyPadButtonInfo* pButtonInfo = NULL;

		for(i = 0; i < PAD_BUTTONTYPE_MAX; i++) {
			for(int j = 0; j < PAD_BUTTON_NO_MAX; j++) {
				pButtonInfo = new SPJoyPadButtonInfo;
				m_vpButtonSetting[i].push_back(pButtonInfo);
			}
		}		
		Clear();
	};

	~SPPadSettingInfo() {
		std::vector<SPJoyPadButtonInfo*>::iterator iter;

		for(int i = 0; i < PAD_BUTTONTYPE_MAX; i++) {
			iter = m_vpButtonSetting[i].begin();
			for(; iter != m_vpButtonSetting[i].end(); ++iter) {
				delete (*iter);
			}
			m_vpButtonSetting[i].clear();
		}
	}
	
	void Clear() {
		m_iJoyStickMode = JOYSTICK_MODE_DIGITAL;
		for(int i = 0; i < BUTTON_FUNC_MAX; i++) {
			m_iFunc[i] = PAD_BUTTON_NO_MAX;			
		}

		for(int i = 0; i < BUTTON_MOUSE_MAX; i++) {
			m_iMouse[i] = PAD_BUTTON_NO_MAX;			
		}

	}

	/**
	 * 모든 버튼 설정을 초기화 <br>
	 @param 
	 @return  
	*/
	void ClearAllButtonInfo() {
		std::vector<SPJoyPadButtonInfo*>::iterator iter;
		for(int i = 0; i < PAD_BUTTONTYPE_MAX; i++) {
			iter = m_vpButtonSetting[i].begin();
			for(; iter != m_vpButtonSetting[i].end(); ++iter) {
				(*iter)->Clear();
			}		
		}
	}
	
	/**
	 * 주어진 SPPadSettingInfo로 자신을 복사 <br>
	 @param  SPPadSettingInfo* pSrcInfo
	 @return  bool
	*/
	bool CopySettingInfo(SPPadSettingInfo* pSrcInfo) {
		if(pSrcInfo == NULL) 
			return false;
		
		m_iJoyStickMode = pSrcInfo->m_iJoyStickMode;
		for(int i = 0; i < BUTTON_FUNC_MAX; i++) {
			m_iFunc[i] = pSrcInfo->m_iFunc[i];
		}

		for(int i = 0; i < BUTTON_MOUSE_MAX; i++) {
			m_iMouse[i] = pSrcInfo->m_iMouse[i];
		}

		bool bRet = false;
		std::vector<SPJoyPadButtonInfo*>::iterator iter;
		std::vector<SPJoyPadButtonInfo*>::iterator iterSrc;
		for(int i = 0; i < PAD_BUTTONTYPE_MAX; i++) {
			iter = m_vpButtonSetting[i].begin();
			iterSrc = pSrcInfo->m_vpButtonSetting[i].begin();
			for(; iter != m_vpButtonSetting[i].end(); ++iter, ++iterSrc) {
				bRet &= (*iter)->CopyInfo((*iterSrc));
			}
		}
		return bRet;
	}
	
	/**
	 * 일반 키 설정을 한다 <br>
	 @param PAD_BUTTON_NO iMajor	대응 버튼이며 리스트상의 위치이기도 하다 
	 @param PAD_BUTTON_USE iUse		사용 타입
	 @param BUTTON_KEY iKey			적용 키
	 @return  bool
	*/
	bool SetNormalInfo(PAD_BUTTON_NO iMajor, PAD_BUTTON_USE iUse, BUTTON_KEY iKey){		
		if(iMajor < 0 || iMajor >= static_cast<int>(m_vpButtonSetting[PAD_BUTTONTYPE_NORMAL].size()))
			return false;
		SPJoyPadButtonInfo* pInfo = m_vpButtonSetting[PAD_BUTTONTYPE_NORMAL].at(iMajor);

		if(pInfo) {
			pInfo->m_iButtonMajor	= iMajor;
			pInfo->m_iUse			= iUse;
			pInfo->m_iKey			= iKey;
			pInfo->m_iInputState	= INPUT_UNPRESS;			
			return true;
		}

		return false;
	}
	
	/**
	 * 조합키 1 에 관한 키 설정을 한다 <br>
	 * 조합키 는 기존에 설정된 키를 사용한다 <br>
	 @param PAD_BUTTON_NO iMinor	대응 버튼 이며 리스트상의 위치이기도 하다 
	 @param BUTTON_KEY iKey			적용 키
	 @return  bool
	*/
	bool SetFunc1KeyInfo(PAD_BUTTON_NO iMinor, BUTTON_KEY iKey) {
		if(iMinor < 0 || iMinor >= static_cast<int>(m_vpButtonSetting[PAD_BUTTONTYPE_FUNC1].size()))
			return false;

		SPJoyPadButtonInfo* pInfo = m_vpButtonSetting[PAD_BUTTONTYPE_FUNC1].at(iMinor);
		if(pInfo) {
			if(iMinor == m_iFunc[BUTTON_FUNC_0] || iMinor == m_iFunc[BUTTON_FUNC_1] || 
				iMinor == m_iMouse[BUTTON_MOUSE_L] || iMinor == m_iMouse[BUTTON_MOUSE_R])
			{
				return false;
			}

			pInfo->m_iButtonMajor	= m_iFunc[BUTTON_FUNC_0];
			pInfo->m_iButtonMinor	= iMinor;
			pInfo->m_iKey			= iKey;
			pInfo->m_iInputState	= INPUT_UNPRESS;
		}

		return true;
	}
	
	/**
	* 조합키 2 에 관한 키 설정을 한다 <br>
	* 조합키 는 기존에 설정된 키를 사용한다 <br>
	@param PAD_BUTTON_NO iMinor	대응 버튼 이며 리스트상의 위치이기도 하다 
	@param BUTTON_KEY iKey			적용 키
	@return  bool
	*/
	bool SetFunc2KeyInfo(PAD_BUTTON_NO iMinor, BUTTON_KEY iKey) {
		if(iMinor < 0 || iMinor >= static_cast<int>(m_vpButtonSetting[PAD_BUTTONTYPE_FUNC2].size()))
			return false;

		SPJoyPadButtonInfo* pInfo = m_vpButtonSetting[PAD_BUTTONTYPE_FUNC2].at(iMinor);
		if(pInfo) {
			if(iMinor == m_iFunc[BUTTON_FUNC_0] || iMinor == m_iFunc[BUTTON_FUNC_1] || 
				iMinor == m_iMouse[BUTTON_MOUSE_L] || iMinor == m_iMouse[BUTTON_MOUSE_R])
			{
				return false;
			}

			pInfo->m_iButtonMajor	= m_iFunc[BUTTON_FUNC_1];
			pInfo->m_iButtonMinor	= iMinor;
			pInfo->m_iKey			= iKey;
			pInfo->m_iInputState	= INPUT_UNPRESS;
		}

		return true;
	}
	
	/**
	 * 주어진 위치의 버튼 정보를 가져온다 <br>
	 @param PAD_BUTTONTYPE iType
	 @param PAD_BUTTON_NO iIndex
	 @return  SPJoyPadButtonInfo*
	*/
	SPJoyPadButtonInfo* GetButtonInfo(PAD_BUTTONTYPE iType, PAD_BUTTON_NO iIndex) {		
		if(iIndex < 0 || iIndex >= static_cast<int>(m_vpButtonSetting[iType].size()))
			return NULL;

		return m_vpButtonSetting[iType].at(iIndex);
	}
	
	/**
	 * 주어진 키로 셋팅된 모든 키를 검색해 적용 키를 초기화 한다 <br>
	 * 중복을 맊기위한 용도 <br>
	 @param BUTTON_KEY iKey
	 @return  int 
	*/
	int SetClearKey(BUTTON_KEY iKey) {
		int iCount = 0;
		std::vector<SPJoyPadButtonInfo*>::iterator iter;
		for(int i = 0; i < PAD_BUTTONTYPE_MAX; i++) {
			iter = m_vpButtonSetting[i].begin();
			for(; iter != m_vpButtonSetting[i].end(); ++iter) {
				if((*iter)->m_iKey == iKey) {
					(*iter)->m_iKey = BUTTON_KEY_MAX;
					iCount++;
				}
			}
		}

		return iCount;
	}

	/**
	 *
	 @param 
	 @return  
	*/
	bool SetFuncKey(PAD_BUTTONTYPE iType) {
		std::vector<SPJoyPadButtonInfo*>::iterator iter;
		if(iType == PAD_BUTTONTYPE_FUNC1) {
			iter = m_vpButtonSetting[iType].begin();
			for(; iter != m_vpButtonSetting[iType].end(); ++iter) {
				(*iter)->m_iButtonMajor = m_iFunc[BUTTON_FUNC_0];
			}

			return true;
		}
		else if(iType == PAD_BUTTONTYPE_FUNC2) {
			iter = m_vpButtonSetting[iType].begin();
			for(; iter != m_vpButtonSetting[iType].end(); ++iter) {
				(*iter)->m_iButtonMajor = m_iFunc[BUTTON_FUNC_1];
			}
			return true;
		}
		
		return false;
	}
};



//class SPManager;
class SPInputManager;

class SPJoyPadManager {
public:
	SPJoyPadManager();
	~SPJoyPadManager();

	void Process(float fTime);
	void Render(float fTime);		

	/**
	* 설정된 버튼 정보를 저장한다 <br>
	@param 
	@return  
	*/
	void Save();
	
	/**
	* 버튼 정보를 초기화한다 <br>
	@param SPPadSettingInfo* 초기화 하고자 하는 setting info NULL로 들어가면 맴버를 초기화
	@return  
	*/
	void SetDefault(SPPadSettingInfo* pInfo = NULL);

	/**
	*
	@param 
	@return  
	*/
	void SetKeyList();
	
	/**
	 *
	 @param 
	 @return  
	*/
	const char* GetKeyName(BUTTON_KEY iKey);

	/**
	* ButtonSetting에 세팅된 정보를 Button에 연결한다 <br> 
	@param 
	@return  
	*/
	void UpdateButton();

	/**
	 * 조이패드의 아나로그/디지털 모드를 설정 <br>
	 @param JOYSTICK_MODE iMode
	 @return  
	*/
	void SetJoyStickMode(JOYSTICK_MODE iMode);
	
	/**
	 * 현재 조이패드 모드를 확인 <br>
	 @param 
	 @return   JOYSTICK_MODE
	*/
	JOYSTICK_MODE GetJoyStickMode();

	/**
	 * 조이패드 셋팅 정보를 가져온다 <br>
	 @param 
	 @return  SPPadSettingInfo*
	*/
	SPPadSettingInfo* GetPadSettingInfo();

	/**
	 * 주어진 타입의 m_vpButton 을 가져온다 
	 @param PAD_BUTTON_KEY_TYPE iType
	 @return  std::vector<SPJoyPadButtonInfo*>*
	*/
	std::vector<SPJoyPadButtonInfo*>* GetPadButton(PAD_BUTTON_KEY_TYPE iType);

	/**
	 * 현재 키셋팅중 임을 적용 <br> 
	 @param 
	 @return  
	*/
	void SetButtonSetMode(bool bMode) {	m_bButtonSetMode = bMode; }
	
	/**
	 * 현재 키셋팅중인지 확인 <br> 
	 @param 
	 @return  
	*/
	bool IsButtonSetMode()	{ return m_bButtonSetMode; }		
	
	/**
	 * 주어진 키를 편집중인 키로 설정 <br> 
	 @param 
	 @return  
	*/
	inline void SetSelectKey(JOYSTICK_SELECT iSelect)	{ m_iSelect = iSelect; }
	
	/**
	 * 편집중인 키가 있는지 확안 <br>
	 @param 
	 @return  
	*/
	inline JOYSTICK_SELECT GetSelectKey() { return m_iSelect; }
	
	/**
	 * 키 편집중 다른 키 입력을 막기위해 설정 <br>
	 @param 
	 @return  
	*/
	inline void SetKeyPass(bool bPass)		{ m_bKeyPass = bPass; }
	
	/**
	 * 현재 버튼설정을 하고 있는지 확인 <br>
	 @param 
	 @return  
	*/
	inline bool IsKeyPass()					{ return m_bKeyPass; }
	
	/**
	 * 패드를 이용하여 마우스 포인터를 이동하고 있는지 설정 <br>
	 @param 
	 @return  
	*/
	inline void SetPadMouseMove(bool bMove)	{ m_bMouseMove = bMove; }
	
	/**
	 * 패드를 이용하여 마우스 포인터를 이동하고 있는지 확인 <br>
	 @param 
	 @return  
	*/
	inline bool IsPadMouseMove()			{ return m_bMouseMove; }
	
	/**
	 * 패드를 이용하여 마우스 포인터를 이동하는 경우 보정치 <br>
	 @param 
	 @return  
	*/
	inline int	GetFixHeight()				{ return m_iFixHeight; }

	/**
	 * 패드를 이용하여 마우스 포인터를 이동하는 경우 보정치 <br>
	 @param 
	 @return  
	*/
	inline int	GetFixWidth()				{ return m_iFixWidth; }

protected:
	bool Init();
	void Clean();	
	
	/**
	 * 패드 설정을 로드 <br>
	 @param 
	 @return  
	*/
	bool Load();

	/**
	 * 버튼별 셋팅 상태 초기화 <br> 
	 @param 
	 @return  
	*/
	void ClearAllSettingInfo();	
	
	
	/**
	 * 모든 키 기능별 상태 확인용 인포 삭제 <br> 
	 @param 
	 @return  
	*/
	void DeleteAllButton();			
	

private:	
	/**
	 * 키 기능별 상태 확인용 정보 <br> 실 데이터는 없는 링크 포인터 모음 <br> 
	*/
	std::vector<SPJoyPadButtonInfo*>	m_vpButton[PAD_BUTTON_KEY_TYPE_MAX];		

	SPPadSettingInfo*		m_pPadInfo;				/**< 조이패드 설정 정보를 기억 <br> */

	bool					m_bButtonSetMode;		/**< 현재 버튼 세팅 중인지 <br> */
	JOYSTICK_SELECT			m_iSelect;				/**< 현재 편집을 위해 선택한 버튼 <br> */
	bool					m_bKeyPass;				/**< 현재 버튼설정을 하고 있는지 <br> */
	bool					m_bMouseMove;			/**< 현재 마우스 Move를 패드로 하고 있는지 <br> */
	
	int						m_iFixWidth;			/**< 프레임 넓이 <br> */
	int						m_iFixHeight;			/**< 프레임 높이 <br> */
	
	bool					m_bFuncCheckPush1;		/**< 조합키 1 눌려있는지 확인 <br> */
	bool					m_bFuncCheckPush2;		/**< 조합키 2 눌려있는지 확인 <br> */

	std::string				m_strKeyList[BUTTON_KEY_MAX + 1];
};


