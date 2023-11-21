#pragma once 



enum JOYSTICK_SET_MODE {
	JOYSTICK_SET_MODE_BASE,						/**< 기본설정 페이지 <br> */
	JOYSTICK_SET_MODE_BUTTON,					/**< 버튼설정 페이지 <br> */
	JOYSTICK_SET_MODE_MAX,
};


enum JOYSTICK_BASE_HELP {
	JOYSTICK_BASE_HELP_0,
	JOYSTICK_BASE_HELP_1,
	JOYSTICK_BASE_HELP_2,
	JOYSTICK_BASE_HELP_3,
	JOYSTICK_BASE_HELP_MAX,
};


#define WIID_JOYSTICK_TITLE						(WIID_JOYSTICK	+	1)
#define WIID_JOYSTICK_LINE1						(WIID_JOYSTICK	+	2)


#define WIID_JOYSTICK_BASE						(WIID_JOYSTICK	+	100)	/**< 기본설정 바탕 <br> */
#define WIID_JOYSTICK_BASE_TEXT					(WIID_JOYSTICK	+	101)	/**< 기본설정 텍스트 이미지 <br> */

#define WIID_JOYSTICK_BASE_BTN_TEST_LEFT		(WIID_JOYSTICK	+	105)	/**< 버튼 번호확인 SKIN <br> */
#define WIID_JOYSTICK_BASE_BTN_TEST_CENTER		(WIID_JOYSTICK	+	106)
#define WIID_JOYSTICK_BASE_BTN_TEST_RIGHT		(WIID_JOYSTICK	+	107)
#define WIID_JOYSTICK_BASE_BTN_TEST_TEXT		(WIID_JOYSTICK	+	108)	/**< 버튼 번호확인 텍스트 <br> */

#define WIID_JOYSTICK_BASE_PAD_IMAGE			(WIID_JOYSTICK	+	110)	/**< 패드 이미지 <br> */
#define WIID_JOYSTICK_BASE_PAD_MODE_CHACK		(WIID_JOYSTICK	+	111)	/**< 아나로그 체크 버튼 <br> */

#define WIID_JOYSTICK_BASE_FUNCKEY_TEXT_SKIN	(WIID_JOYSTICK	+	120)	/**< 조합버튼 지정 텍스트 <br> */
#define WIID_JOYSTICK_BASE_FUNCKEY_TEXT			(WIID_JOYSTICK	+	121)
#define WIID_JOYSTICK_BASE_FUNCKEY				(WIID_JOYSTICK	+	122)	/**< 조합 A <br> */
//#define WIID_JOYSTICK_BASE_FUNCKEY_2			(WIID_JOYSTICK	+	123)	/**< 조합 B <br> */

#define WIID_JOYSTICK_BASE_MOUSEKEY_TEXT_SKIN	(WIID_JOYSTICK	+	130)	/**< 마우스 지정 텍스트 <br> */
#define WIID_JOYSTICK_BASE_MOUSEKEY_TEXT		(WIID_JOYSTICK	+	131)	
#define WIID_JOYSTICK_BASE_MOUSEKEY				(WIID_JOYSTICK	+	132)	/**< 마우스 A <br> */
//#define WIID_JOYSTICK_BASE_MOUSEKEY_2			(WIID_JOYSTICK	+	133)	/**< 마우스 B <br> */

#define WIID_JOYSTICK_BASE_HELP_SKIN_LEFT		(WIID_JOYSTICK	+	150)	/**< 도움말 영역 왼쪽 <br> */
#define WIID_JOYSTICK_BASE_HELP_SKIN_CENTER		(WIID_JOYSTICK	+	151)	/**< 도움말 영역 센터 <br> */
#define WIID_JOYSTICK_BASE_HELP_SKIN_RIGHT		(WIID_JOYSTICK	+	152)	/**< 도움말 영역 오른쪽 <br> */
#define WIID_JOYSTICK_BASE_HELP					(WIID_JOYSTICK	+	160)	/**< 도움말 <br> */

#define WIID_JOYSTICK_BASE_NEXT					(WIID_JOYSTICK	+	190)	/**< 다음 버튼 <br> */
#define WIID_JOYSTICK_BASE_INIT					(WIID_JOYSTICK	+	191)	/**< 초기화 버튼 <br> */
#define WIID_JOYSTICK_BASE_OK					(WIID_JOYSTICK	+	192)	/**< 확인 버튼 <br> */
#define WIID_JOYSTICK_BASE_CANCEL				(WIID_JOYSTICK	+	193)	/**< 취소 버튼 <br> */


#define WIID_JOYSTICK_BUTTON					(WIID_JOYSTICK	+	200)	/**< 버튼설정 바탕 <br> */
#define WIID_JOYSTICK_BUTTON_TEXT				(WIID_JOYSTICK	+	201)	/**< 버튼설정 텍스트 이미지 <br> */

#define WIID_JOYSTICK_BUTTON_BTN_TEST_LEFT		(WIID_JOYSTICK	+	205)	/**< 버튼 번호확인 SKIN <br> */
#define WIID_JOYSTICK_BUTTON_BTN_TEST_CENTER	(WIID_JOYSTICK	+	206)
#define WIID_JOYSTICK_BUTTON_BTN_TEST_RIGHT		(WIID_JOYSTICK	+	207)
#define WIID_JOYSTICK_BUTTON_BTN_TEST_TEXT		(WIID_JOYSTICK	+	208)	/**< 버튼 번호확인 텍스트 <br> */

#define WIID_JOYSTICK_BUTTON_HELP1_SKIN_LEFT	(WIID_JOYSTICK	+	210)	/**< 패드 버튼 번호확인 옆 도움말 영역 왼족 <br> */
#define WIID_JOYSTICK_BUTTON_HELP1_SKIN_CENTER	(WIID_JOYSTICK	+	211)	/**< 패드 버튼 번호확인 옆 도움말 영역 센터 <br> */
#define WIID_JOYSTICK_BUTTON_HELP1_SKIN_RIGHT	(WIID_JOYSTICK	+	212)	/**< 패드 버튼 번호확인 옆 도움말 영역 오른쪽 <br> */
#define WIID_JOYSTICK_BUTTON_HELP1				(WIID_JOYSTICK	+	213)	/**< 도움말 1 <br> */

#define WIID_JOYSTICK_BUTTON_HELP2_SKIN_LEFT	(WIID_JOYSTICK	+	215)	/**< 게임키 정보 하단의 도움말 영역 왼쪽 <br> */
#define WIID_JOYSTICK_BUTTON_HELP2_SKIN_CENTER	(WIID_JOYSTICK	+	216)	/**< 게임키 정보 하단의 도움말 영역 센터 <br> */
#define WIID_JOYSTICK_BUTTON_HELP2_SKIN_RIGHT	(WIID_JOYSTICK	+	217)	/**< 게임키 정보 하단의 도움말 영역 오른쪽 <br> */
#define WIID_JOYSTICK_BUTTON_HELP2				(WIID_JOYSTICK	+	218)	/**< 도움말 2 <br> */

#define WIID_JOYSTICK_BUTTON_KEY_LEFT			(WIID_JOYSTICK	+	220)	/**< 키 입력창 SKIN <br> */
#define WIID_JOYSTICK_BUTTON_KEY_CENTER			(WIID_JOYSTICK	+	221)
#define WIID_JOYSTICK_BUTTON_KEY_RIGHT			(WIID_JOYSTICK	+	222)

#define WIID_JOYSTICK_BUTTON_PREV				(WIID_JOYSTICK	+	225)	/**< 다음 버튼 <br> */
#define WIID_JOYSTICK_BUTTON_INIT				(WIID_JOYSTICK	+	226)	/**< 초기화 버튼 <br> */
#define WIID_JOYSTICK_BUTTON_OK					(WIID_JOYSTICK	+	227)	/**< 확인 버튼 <br> */
#define WIID_JOYSTICK_BUTTON_CANCEL				(WIID_JOYSTICK	+	228)	/**< 취소 버튼 <br> */

#define WIID_JOYSTICK_BUTTON_NORMAL				(WIID_JOYSTICK	+	230)	/**< 탭 단일키, 조합1, 조합2 <br> */

#define WIID_JOYSTICK_BUTTON_KEY				(WIID_JOYSTICK	+	250)	/**< 키 입력을 받는 버튼 <br> 12개 */

#define WIID_JOYSTICK_BUTTON_INFO_TEXT_IMAGE	(WIID_JOYSTICK	+	300)	/**< 게임키 정보 텍스트 <br> */
#define WIID_JOYSTICK_BUTTON_KEYBOARD_IMAGE		(WIID_JOYSTICK	+	301)	/**< 키보드 이미지 <br> */
#define WIID_JOYSTICK_BUTTON_KEYBOARD_KEY		(WIID_JOYSTICK	+	310)	/**< 키보드 각각의 키 <br> */

const int MAX_FUNC_BUTTON				= 2;
const int MAX_MOUSE_BUTTON				= 2;

#include "SPJoyPadManager.h"
class SPWindow;
class SPWindowButton;

class SPWindowJoyStick : public SPWindow 
{
public:
	SPWindowJoyStick(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);
	virtual ~SPWindowJoyStick();

	virtual void Init();
	virtual void Clean();

	virtual void Process(float fTime);
	virtual void Render(float fTime);



protected:
	virtual void Show(bool bWithChild = true);
	virtual void Hide(bool bSendServer = true);		

	/**
	* 내부 컨트롤 생성 <br>
	@param 
	@return
	*/
	void InitSubControl();
	
	/**
	 * 12개의 키 입력 버튼 셋팅용 <br> 
	 @param 
	 @return  
	*/
	inline void SetKeyButton(SPWindowButton* pButton);

	/**
	 * JOYSTICK_SET_MODE 별로 인터페이스 갱신
	 @param 
	 @return  
	*/
	void UpdateSubControl();

	/**
	 * 버튼설정에 설정된 키 정보를 업데이트 해준다 <br> 
	 @param 
	 @return  
	*/
	void UpdateKeyList();

	/**
	* 패드 입력신호를 체크한다 <br>
	@param 
	@return  
	*/
	void PadCheckProcess(float fTime);
	
	/**
	 * 조합키와 마우스 키를 셋팅한다 <br> 
	 @param PAD_BUTTON_NO iButton	선택된 버튼
	 @return  bool
	*/
	bool SetSpecialKey(PAD_BUTTON_NO iButton);	

	/**
	 * 키보드 입력을 체크 한다 <br> 
	 @param 
	 @return  
	*/
	void KeyCheckProcess(float fTime);

protected:
	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()
	int OnNext				(WPARAM wParam, LPARAM lparam);
	int OnReset				(WPARAM wParam, LPARAM lparam);
	int OnOk				(WPARAM wParam, LPARAM lparam);
	int OnCancel			(WPARAM wParam, LPARAM lparam);
	int OnPrev				(WPARAM wParam, LPARAM lparam);
	int OnPadMode			(WPARAM wParam, LPARAM lparam);							/**< 아나로그 체크 박스 <br> */
	int OnSelectFunc		(unsigned int iID, WPARAM wParam, LPARAM lparam);		/**< 조합키 1, 2 <br> */
	int OnSelectMouse		(unsigned int iID, WPARAM wParam, LPARAM lparam);		/**< Mouse L, R <br> */
	int OnSelectKeyType		(unsigned int iID, WPARAM wParam, LPARAM lparam);		/**< 단일키 입력, 조합키 입력 1, 조합키 입력 2 <br> */
	int OnSelectKey			(unsigned int iID, WPARAM wParam, LPARAM lparam);		/**< 12개의 키 <br> */

private:
	SPTexture*				m_pTextureBase;							/**< 윈도우 바탕용 리소스 <br> */
	RECT					m_rtBaseSrc[WINDOW_BASE_MAX];
	RECT					m_rtBaseDest[WINDOW_BASE_MAX];

	JOYSTICK_SET_MODE		m_iSetMode;								/**< 설정 모드 <br> */
	SPWindow*				m_pSetMode[JOYSTICK_SET_MODE_MAX];		/**< 각 모드 별 바탕 윈도우 <br> */

	SPTexture*				m_pTextureButton;						/**< 버튼 표현용 리소스 <br> */
	RECT					m_rtButtonSkinSrcNormal;				/**< 패드버튼 번호 밑 스킨 소스 일반 <br> */
	RECT					m_rtButtonSkinSrcPush;					/**< 패드버튼 번호 밑 스킨 소스 입력 <br> */
	RECT					m_rtButtonSkinDest[PAD_BUTTON_NO_MAX];	/**< 패드버튼 번호 밑 스킨 대상 <br> */
	RECT					m_rtButtonSrc[PAD_BUTTON_NO_MAX + 1];	/**< 패드버튼 번호 소스 <br> */
	RECT					m_rtButtonDest[PAD_BUTTON_NO_MAX];		/**< 패드버튼 번호 대상 <br> */
	bool					m_bButtonPush[PAD_BUTTON_NO_MAX];		/**< 패드버튼 번호 입력 상태 <br> */

	RECT					m_rtFuncSkinDest[BUTTON_FUNC_MAX];		/**< 조합키 스킨 대상 <br> */
	RECT					m_rtFuncDest[BUTTON_FUNC_MAX];			/**< 조합키 패드버튼 대상 <br> */

	RECT					m_rtMouseSkinDest[BUTTON_MOUSE_MAX];	/**< 마우스 대상 <br> */
	RECT					m_rtMouseDest[BUTTON_MOUSE_MAX];		/**< 마우스 패드버튼 대상 <br> */

	SPWindowButton*			m_pModeCheck;							/**< 아날로그, 디지털 체크 버튼 <br> */
	SPWindowButton*			m_pFunc[BUTTON_FUNC_MAX];				/**< 조합버튼 지정 체크 버튼 <br> */
	SPWindowButton*			m_pMouse[BUTTON_MOUSE_MAX];				/**< 마우스버튼 지정 체크 버튼 <br> */
	//JOYSTICK_SELECT			m_iSelect;								/**< 설정 대상 <br> */
	SPWindow*				m_pBaseHelp[JOYSTICK_BASE_HELP_MAX];	/**< <br> */
	
	SPWindowButton*			m_pButtonMode[PAD_BUTTONTYPE_MAX];		/**< 단일키, 조합1,2 선택 탭 버튼 <br> */
	PAD_BUTTONTYPE			m_iButtonSetType;
	SPWindowButton*			m_pButtonKey[PAD_BUTTON_NO_MAX];		/**< 키입력을 받고, 기억하고, 보여줄 버튼 <br> */
	PAD_BUTTON_NO			m_iButtonSetNo;

	RECT					m_rtButtonSetSkinDest[PAD_BUTTON_NO_MAX];/**< 단일키, 조합1,2 에 각 버튼 위치 스킨 대상 <br> */
	RECT					m_rtButtonSetDest[PAD_BUTTON_NO_MAX];	/**< 단일키, 조합1,2 에 각 버튼 대상 <br> */
	
	SPPadSettingInfo*		m_pCopyInfo;							/**< 에디트용 패드 설정 정보 <br> */	
	SPJoyPadManager*		m_pManager;								/**< SPJoyPadManager* <br> */	
	
	SPWindowButton*			m_pButtonNext;							/**< 다음 버튼 <br> */
	SPWindowButton*			m_pButtonOk;							/**< 확인 버튼 <br> */
	SPWindowButton*			m_pButtonReset;							/**< 초기화 버튼 <br> */
	std::vector<SPWindow*>	m_vpKeyButton;							/**< 게임키 정보에 사용되는 키들 <br> */
};


