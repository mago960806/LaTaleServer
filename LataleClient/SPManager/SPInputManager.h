// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPInputManager
// Comment     : 
// Creation    : metalgeni 2004-08-23 오후 2:56:36
//***************************************************************************


#pragma once 

#define MAX_USEKEY			128
#define MAX_USEJSBUTTON		256

enum KEY_ACTION_MAP {
	// Action Key

	KEY_AM_LEFT				,
	KEY_AM_RIGHT			,
	KEY_AM_UP				,
	KEY_AM_DOWN				,
	KEY_AM_SWAP				,

	//////////////////////////////////////////////////////////////////////////
	// QuickSlot 중간에 끼어들면 안되요~ [7/6/2007 AJJIYA]

	KEY_AM_QUICKKEY_01_01	,
	KEY_AM_QUICKKEY_01_02	,
	KEY_AM_QUICKKEY_01_03	,
	KEY_AM_QUICKKEY_01_04	,
	KEY_AM_QUICKKEY_01_05	,
	KEY_AM_QUICKKEY_01_06	,
	KEY_AM_QUICKKEY_01_07	,
	KEY_AM_QUICKKEY_01_08	,
	KEY_AM_QUICKKEY_01_09	,
	KEY_AM_QUICKKEY_01_10	,
	KEY_AM_QUICKKEY_01_11	,
	KEY_AM_QUICKKEY_01_12	,

	KEY_AM_QUICKKEY_02_01	,
	KEY_AM_QUICKKEY_02_02	,
	KEY_AM_QUICKKEY_02_03	,
	KEY_AM_QUICKKEY_02_04	,
	KEY_AM_QUICKKEY_02_05	,
	KEY_AM_QUICKKEY_02_06	,
	KEY_AM_QUICKKEY_02_07	,
	KEY_AM_QUICKKEY_02_08	,
	KEY_AM_QUICKKEY_02_09	,
	KEY_AM_QUICKKEY_02_10	,
	KEY_AM_QUICKKEY_02_11	,
	KEY_AM_QUICKKEY_02_12	,

	KEY_AM_QUICKHOTKEY_01	,
	KEY_AM_QUICKHOTKEY_02	,

	//////////////////////////////////////////////////////////////////////////

	KEY_AM_CHAT_REPLY		,

	KEY_AM_WORLDMAP_LEFT	,
	KEY_AM_WORLDMAP_UP		,
	KEY_AM_WORLDMAP_RIGHT	,
	KEY_AM_WORLDMAP_DOWN	,
	KEY_AM_MAX				,
};


struct SPMInputKey;
struct SPInputStruct {
	SPMInputKey ikArrowUp;
	SPMInputKey ikArrowDown;
	SPMInputKey ikArrowLeft;
	SPMInputKey ikArrowRight;
	SPMInputKey ikPageUP;
	SPMInputKey ikPageDown;
	SPMInputKey ikLShift;	
	SPMInputKey ikGrave;
	SPMInputKey ikApostrophe;
	SPMInputKey ikSpaceBar;
	SPMInputKey ikCtrl;	
	SPMInputKey ikReturn;	
	SPMInputKey ikTab;	
	SPMInputKey ikEscape;
	SPMInputKey ikLAlt;
	SPMInputKey ikLCtrl;
	SPMInputKey ikA;
	SPMInputKey ikB;
	SPMInputKey ikC;
	SPMInputKey ikD;
	SPMInputKey ikE;
	SPMInputKey ikF;
	SPMInputKey ikG;
	SPMInputKey ikH;
	SPMInputKey ikI;
	SPMInputKey ikJ;
	SPMInputKey ikK;
	SPMInputKey ikL;
	SPMInputKey ikM;
	SPMInputKey ikN;
	SPMInputKey ikO;
	SPMInputKey ikP;
	SPMInputKey ikQ;
	SPMInputKey ikR;
	SPMInputKey ikS;
	SPMInputKey ikT;
	SPMInputKey ikU;
	SPMInputKey ikV;
	SPMInputKey ikW;
	SPMInputKey ikX;
	SPMInputKey ikY;
	SPMInputKey ikZ;
	SPMInputKey ikF1;
	SPMInputKey ikF2;
	SPMInputKey ikF3;
	SPMInputKey ikF4;
	SPMInputKey ikF5;
	SPMInputKey ikF6;
	SPMInputKey ikF7;
	SPMInputKey ikF8;
	SPMInputKey ikF9;
	SPMInputKey ikF10;	
	SPMInputKey ikF11;
	SPMInputKey ikF12;
	SPMInputKey ik1;
	SPMInputKey ik2;
	SPMInputKey ik3;
	SPMInputKey ik4;
	SPMInputKey ik5;
	SPMInputKey ik6;
	SPMInputKey ik7;
	SPMInputKey ik8;
	SPMInputKey ik9;
	SPMInputKey ik0;
	SPMInputKey ikMinus;
	SPMInputKey ikEquals;
	SPMInputKey ikNumPad0;
	SPMInputKey ikNumPad1;
	SPMInputKey ikNumPad2;
	SPMInputKey ikNumPad3;
	SPMInputKey ikNumPad4;
	SPMInputKey ikNumPad5;
	SPMInputKey ikNumPad6;
	SPMInputKey ikNumPad7;
	SPMInputKey ikNumPad8;
	SPMInputKey ikNumPad9;
	SPMInputKey ikPrevTrack;  /* Previous Track (DIK_CIRCUMFLEX on Japanese keyboard) */

};



union SPKeyBinder {
	SPInputStruct	InputStruct;
	SPMInputKey		ik[MAX_USEKEY];
};


struct SPMInputBtn;
struct SPInputStructJoyStick {
	SPMInputBtn ibLX;             
	SPMInputBtn ibLY;             
	SPMInputBtn ibLZ;             
	SPMInputBtn ibLRx;            
	SPMInputBtn ibLRy;            
	SPMInputBtn ibLRz;            
	SPMInputBtn ibRglSlider[2];   
	SPMInputBtn ibRgdwPOV[4];     
	SPMInputBtn ibRgbButtons[128];
	SPMInputBtn ibLVX;            
	SPMInputBtn ibLVY;            
	SPMInputBtn ibLVZ;            
	SPMInputBtn ibLVRx;           
	SPMInputBtn ibLVRy;           
	SPMInputBtn ibLVRz;           
	SPMInputBtn ibRglVSlider[2];  
	SPMInputBtn ibLAX;            
	SPMInputBtn ibLAY;            
	SPMInputBtn ibLAZ;            
	SPMInputBtn ibLARx;           
	SPMInputBtn ibLARy;           
	SPMInputBtn ibLARz;           
	SPMInputBtn ibRglASlider[2];  
	SPMInputBtn ibLFX;            
	SPMInputBtn ibLFY;            
	SPMInputBtn ibLFZ;            
	SPMInputBtn ibLFRx;           
	SPMInputBtn ibLFRy;           
	SPMInputBtn ibLFRz;           
	SPMInputBtn ibRglFSlider[2];
};


union SPJSButtonBinder {
	SPInputStructJoyStick	InputStruct;
	SPMInputBtn				ib[MAX_USEJSBUTTON];
};



class SPManager;
class SPJoyPadManager;

class SPInputManager : public SPManager {
public:
	SPInputManager();
	virtual ~SPInputManager();

	virtual void Process(float fTime);

	virtual bool Init();
	virtual void Clean();

	void Active();
	void Deactive();
	bool IsActive();


	SPInputStruct*				GetInputStruct();
	SPInputStructJoyStick*		GetInputStructJoyStick();

	void UpdateInput();
	void UpdateInputJoyStick();

	void CameraTrigger();
	void PlayerTrigger();

	enum INPUT_FOCUS {
		INPUT_FOCUS_NULL,
		INPUT_FOCUS_CHAT,
	};

	void SetFocus(INPUT_FOCUS focus = INPUT_FOCUS_NULL);
	void TestTrigger();

	// Action Map
	bool			SetActionMap(KEY_ACTION_MAP KeyActionMap, SPKeyList KeyList);
	SPMInputKey*	GetActionMap(KEY_ACTION_MAP KeyActionMap);

	bool			SetActionMapJS(KEY_ACTION_MAP KeyActionMap, SPBtnList ButtonList,int iIndex = 0);
	SPMInputBtn*	GetActionMapJS(KEY_ACTION_MAP KeyActionMap);	

	SPJoyPadManager* GetJoyPadManager();
	void			SetJoyPadKeyName();


protected:	
	INPUT_FOCUS					m_focus;
	bool						m_bActive;

	SPKeyBinder					m_KeyBinder;
	SPJSButtonBinder			m_JSBinder;

	std::map<KEY_ACTION_MAP, SPMInputKey*> m_mActionMap;
	std::map<KEY_ACTION_MAP, SPMInputBtn*> m_mActionMapJS;	

	SPJoyPadManager*			m_pJoyPadManager;
	
};

extern SPInputManager* g_pInputManager;


