
#include "SPCommon.h"
#include "SPUtil.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPWindowJoyStick.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"
#include "SPLocalizeManager.h"

#include "SPJoyPadManager.h"


SPWindowJoyStick::SPWindowJoyStick( INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent /*= NULL*/)
: SPWindow(WNDID_JOYSTICK, InstanceID, iX, iY, iCX, iCY, pParent)
{
	Init();
}


SPWindowJoyStick::~SPWindowJoyStick()
{	
	Clean();
}


void SPWindowJoyStick::Init()
{	
	m_pManager = NULL;
	m_pManager = g_pInputManager->GetJoyPadManager();
	if(m_pManager == NULL) 
		return;
	
	m_pCopyInfo = new SPPadSettingInfo;	
	
	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI105.PNG" , &m_pTextureBase);
	
	SetRect(&m_rtBaseSrc[WINDOW_BASE_TOP_LEFT],		482, 316, 486, 320);
	SetRect(&m_rtBaseSrc[WINDOW_BASE_TOP],			488, 316, 490, 320);
	SetRect(&m_rtBaseSrc[WINDOW_BASE_TOP_RIGHT],		492, 316, 496, 320);
	SetRect(&m_rtBaseSrc[WINDOW_BASE_CENTER_LEFT],	482, 322, 486, 324);
	SetRect(&m_rtBaseSrc[WINDOW_BASE_CENTER],			488, 322, 490, 324);
	SetRect(&m_rtBaseSrc[WINDOW_BASE_CENTER_RIGHT],	492, 322, 496, 324);
	SetRect(&m_rtBaseSrc[WINDOW_BASE_BOTTOM_LEFT],	482, 311, 486, 315);
	SetRect(&m_rtBaseSrc[WINDOW_BASE_BOTTOM],			488, 311, 490, 315);
	SetRect(&m_rtBaseSrc[WINDOW_BASE_BOTTOM_RIGHT],	492, 311, 496, 315);

	SetRect(&m_rtBaseDest[WINDOW_BASE_TOP_LEFT],		m_iAX, m_iAY, m_iAX + 4, m_iAY + 4);
	SetRect(&m_rtBaseDest[WINDOW_BASE_TOP],			m_iAX + 4, m_iAY, m_iAX + 4 + 381, m_iAY + 4);
	SetRect(&m_rtBaseDest[WINDOW_BASE_TOP_RIGHT],		m_iAX + 385, m_iAY, m_iAX + 385 + 4, m_iAY + 4);
	SetRect(&m_rtBaseDest[WINDOW_BASE_CENTER_LEFT],	m_iAX, m_iAY + 4, m_iAX + 4, m_iAY + 4 + 436);
	SetRect(&m_rtBaseDest[WINDOW_BASE_CENTER],		m_iAX + 4, m_iAY + 4, m_iAX + 4 + 381, m_iAY + 4 + 436);
	SetRect(&m_rtBaseDest[WINDOW_BASE_CENTER_RIGHT],	m_iAX + 385, m_iAY + 4, m_iAX + 385 + 4, m_iAY + 4 + 436);
	SetRect(&m_rtBaseDest[WINDOW_BASE_BOTTOM_LEFT],	m_iAX, m_iAY + 440, m_iAX + 4, m_iAY + 440 + 4);
	SetRect(&m_rtBaseDest[WINDOW_BASE_BOTTOM],		m_iAX + 4, m_iAY + 440, m_iAX + 4 + 381, m_iAY + 440 + 4);
	SetRect(&m_rtBaseDest[WINDOW_BASE_BOTTOM_RIGHT],	m_iAX + 385, m_iAY + 440, m_iAX + 385 + 4, m_iAY + 440 + 4);
	
	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI106.PNG" , &m_pTextureButton);
	SetRect(&m_rtButtonSkinSrcNormal,			157, 299, 171, 313);
	SetRect(&m_rtButtonSkinSrcPush,				316, 1, 330, 15);	
	
	SetRect(&m_rtButtonSkinDest[PAD_BUTTON_NO_0],		m_iAX + 19, m_iAY + 62, m_iAX + 19 + 14, m_iAY + 62 + 14);
	SetRect(&m_rtButtonSkinDest[PAD_BUTTON_NO_1],		m_iAX + 34, m_iAY + 62, m_iAX + 34 + 14, m_iAY + 62 + 14);
	SetRect(&m_rtButtonSkinDest[PAD_BUTTON_NO_2],		m_iAX + 49, m_iAY + 62, m_iAX + 49 + 14, m_iAY + 62 + 14);
	SetRect(&m_rtButtonSkinDest[PAD_BUTTON_NO_3],		m_iAX + 64, m_iAY + 62, m_iAX + 64 + 14, m_iAY + 62 + 14);
	SetRect(&m_rtButtonSkinDest[PAD_BUTTON_NO_4],		m_iAX + 19, m_iAY + 77, m_iAX + 19 + 14, m_iAY + 77 + 14);
	SetRect(&m_rtButtonSkinDest[PAD_BUTTON_NO_5],		m_iAX + 34, m_iAY + 77, m_iAX + 34 + 14, m_iAY + 77 + 14);
	SetRect(&m_rtButtonSkinDest[PAD_BUTTON_NO_6],		m_iAX + 49, m_iAY + 77, m_iAX + 49 + 14, m_iAY + 77 + 14);
	SetRect(&m_rtButtonSkinDest[PAD_BUTTON_NO_7],		m_iAX + 64, m_iAY + 77, m_iAX + 64 + 14, m_iAY + 77 + 14);
	SetRect(&m_rtButtonSkinDest[PAD_BUTTON_NO_8],		m_iAX + 19, m_iAY + 92, m_iAX + 19 + 14, m_iAY + 92 + 14);
	SetRect(&m_rtButtonSkinDest[PAD_BUTTON_NO_9],		m_iAX + 34, m_iAY + 92, m_iAX + 34 + 14, m_iAY + 92 + 14);
	SetRect(&m_rtButtonSkinDest[PAD_BUTTON_NO_10],		m_iAX + 49, m_iAY + 92, m_iAX + 49 + 14, m_iAY + 92 + 14);
	SetRect(&m_rtButtonSkinDest[PAD_BUTTON_NO_11],		m_iAX + 64, m_iAY + 92, m_iAX + 64 + 14, m_iAY + 92 + 14);

	//m_rtButtonSrc[PAD_BUTTON_NO_MAX];
	SetRect(&m_rtButtonSrc[PAD_BUTTON_NO_0],			172, 301, 184, 313);
	SetRect(&m_rtButtonSrc[PAD_BUTTON_NO_1],			185, 301, 197, 313);
	SetRect(&m_rtButtonSrc[PAD_BUTTON_NO_2],			198, 301, 210, 313);
	SetRect(&m_rtButtonSrc[PAD_BUTTON_NO_3],			211, 301, 223, 313);
	SetRect(&m_rtButtonSrc[PAD_BUTTON_NO_4],			224, 301, 236, 313);
	SetRect(&m_rtButtonSrc[PAD_BUTTON_NO_5],			237, 301, 249, 313);
	SetRect(&m_rtButtonSrc[PAD_BUTTON_NO_6],			250, 301, 262, 313);
	SetRect(&m_rtButtonSrc[PAD_BUTTON_NO_7],			263, 301, 275, 313);
	SetRect(&m_rtButtonSrc[PAD_BUTTON_NO_8],			276, 301, 288, 313);
	SetRect(&m_rtButtonSrc[PAD_BUTTON_NO_9],			289, 301, 301, 313);
	SetRect(&m_rtButtonSrc[PAD_BUTTON_NO_10],			302, 301, 314, 313);
	SetRect(&m_rtButtonSrc[PAD_BUTTON_NO_11],			315, 301, 327, 313);
	SetRect(&m_rtButtonSrc[PAD_BUTTON_NO_MAX],			328, 301, 340, 313);

	//m_rtButtonDest[PAD_BUTTON_NO_MAX];
	SetRect(&m_rtButtonDest[PAD_BUTTON_NO_0],			m_rtButtonSkinDest[PAD_BUTTON_NO_0].left + 1, m_rtButtonSkinDest[PAD_BUTTON_NO_0].top + 1,	m_rtButtonDest[PAD_BUTTON_NO_0].left + 12, m_rtButtonDest[PAD_BUTTON_NO_0].top + 12);
	SetRect(&m_rtButtonDest[PAD_BUTTON_NO_1],			m_rtButtonSkinDest[PAD_BUTTON_NO_1].left + 1, m_rtButtonSkinDest[PAD_BUTTON_NO_1].top + 1,	m_rtButtonDest[PAD_BUTTON_NO_1].left + 12, m_rtButtonDest[PAD_BUTTON_NO_1].top + 12);
	SetRect(&m_rtButtonDest[PAD_BUTTON_NO_2],			m_rtButtonSkinDest[PAD_BUTTON_NO_2].left + 1, m_rtButtonSkinDest[PAD_BUTTON_NO_2].top + 1,	m_rtButtonDest[PAD_BUTTON_NO_2].left + 12, m_rtButtonDest[PAD_BUTTON_NO_2].top + 12);
	SetRect(&m_rtButtonDest[PAD_BUTTON_NO_3],			m_rtButtonSkinDest[PAD_BUTTON_NO_3].left + 1, m_rtButtonSkinDest[PAD_BUTTON_NO_3].top + 1,	m_rtButtonDest[PAD_BUTTON_NO_3].left + 12, m_rtButtonDest[PAD_BUTTON_NO_3].top + 12);
	SetRect(&m_rtButtonDest[PAD_BUTTON_NO_4],			m_rtButtonSkinDest[PAD_BUTTON_NO_4].left + 1, m_rtButtonSkinDest[PAD_BUTTON_NO_4].top + 1,	m_rtButtonDest[PAD_BUTTON_NO_4].left + 12, m_rtButtonDest[PAD_BUTTON_NO_4].top + 12);
	SetRect(&m_rtButtonDest[PAD_BUTTON_NO_5],			m_rtButtonSkinDest[PAD_BUTTON_NO_5].left + 1, m_rtButtonSkinDest[PAD_BUTTON_NO_5].top + 1,	m_rtButtonDest[PAD_BUTTON_NO_5].left + 12, m_rtButtonDest[PAD_BUTTON_NO_5].top + 12);
	SetRect(&m_rtButtonDest[PAD_BUTTON_NO_6],			m_rtButtonSkinDest[PAD_BUTTON_NO_6].left + 1, m_rtButtonSkinDest[PAD_BUTTON_NO_6].top + 1,	m_rtButtonDest[PAD_BUTTON_NO_6].left + 12, m_rtButtonDest[PAD_BUTTON_NO_6].top + 12);
	SetRect(&m_rtButtonDest[PAD_BUTTON_NO_7],			m_rtButtonSkinDest[PAD_BUTTON_NO_7].left + 1, m_rtButtonSkinDest[PAD_BUTTON_NO_7].top + 1,	m_rtButtonDest[PAD_BUTTON_NO_7].left + 12, m_rtButtonDest[PAD_BUTTON_NO_7].top + 12);
	SetRect(&m_rtButtonDest[PAD_BUTTON_NO_8],			m_rtButtonSkinDest[PAD_BUTTON_NO_8].left + 1, m_rtButtonSkinDest[PAD_BUTTON_NO_8].top + 1,	m_rtButtonDest[PAD_BUTTON_NO_8].left + 12, m_rtButtonDest[PAD_BUTTON_NO_8].top + 12);
	SetRect(&m_rtButtonDest[PAD_BUTTON_NO_9],			m_rtButtonSkinDest[PAD_BUTTON_NO_9].left + 1, m_rtButtonSkinDest[PAD_BUTTON_NO_9].top + 1,	m_rtButtonDest[PAD_BUTTON_NO_9].left + 12, m_rtButtonDest[PAD_BUTTON_NO_9].top + 12);
	SetRect(&m_rtButtonDest[PAD_BUTTON_NO_10],			m_rtButtonSkinDest[PAD_BUTTON_NO_10].left + 1, m_rtButtonSkinDest[PAD_BUTTON_NO_10].top + 1,m_rtButtonDest[PAD_BUTTON_NO_10].left + 12, m_rtButtonDest[PAD_BUTTON_NO_10].top + 12);
	SetRect(&m_rtButtonDest[PAD_BUTTON_NO_11],			m_rtButtonSkinDest[PAD_BUTTON_NO_11].left + 1, m_rtButtonSkinDest[PAD_BUTTON_NO_11].top + 1,m_rtButtonDest[PAD_BUTTON_NO_11].left + 12, m_rtButtonDest[PAD_BUTTON_NO_11].top + 12);

	SetRect(&m_rtFuncSkinDest[BUTTON_FUNC_0],			m_iAX + 352, m_iAY + 86, m_iAX + 352 + 14, m_iAY + 86 + 14);
	SetRect(&m_rtFuncSkinDest[BUTTON_FUNC_1],			m_iAX + 352, m_iAY + 111, m_iAX + 352 + 14, m_iAY + 111 + 14);

	SetRect(&m_rtFuncDest[BUTTON_FUNC_0],				m_rtFuncSkinDest[BUTTON_FUNC_0].left + 1, m_rtFuncSkinDest[BUTTON_FUNC_0].top + 1, m_rtFuncDest[BUTTON_FUNC_0].left + 12, m_rtFuncDest[BUTTON_FUNC_0].top + 12);
	SetRect(&m_rtFuncDest[BUTTON_FUNC_1],				m_rtFuncSkinDest[BUTTON_FUNC_1].left + 1, m_rtFuncSkinDest[BUTTON_FUNC_1].top + 1, m_rtFuncDest[BUTTON_FUNC_1].left + 12, m_rtFuncDest[BUTTON_FUNC_1].top + 12);

	SetRect(&m_rtMouseSkinDest[BUTTON_MOUSE_L],			m_iAX + 352, m_iAY + 167, m_iAX + 352 + 14, m_iAY + 167 + 14);
	SetRect(&m_rtMouseSkinDest[BUTTON_MOUSE_R],			m_iAX + 352, m_iAY + 192, m_iAX + 352 + 14, m_iAY + 192 + 14);

	SetRect(&m_rtMouseDest[BUTTON_MOUSE_L],				m_rtMouseSkinDest[BUTTON_MOUSE_L].left + 1, m_rtMouseSkinDest[BUTTON_MOUSE_L].top + 1, m_rtMouseDest[BUTTON_MOUSE_L].left + 12, m_rtMouseDest[BUTTON_MOUSE_L].top + 12);
	SetRect(&m_rtMouseDest[BUTTON_MOUSE_R],				m_rtMouseSkinDest[BUTTON_MOUSE_R].left + 1, m_rtMouseSkinDest[BUTTON_MOUSE_R].top + 1, m_rtMouseDest[BUTTON_MOUSE_R].left + 12, m_rtMouseDest[BUTTON_MOUSE_R].top + 12);

	SetRect(&m_rtButtonSetSkinDest[PAD_BUTTON_NO_0],		m_iAX + 47, m_iAY + 166, m_iAX + 47 + 14, m_iAY + 166 + 14);
	SetRect(&m_rtButtonSetSkinDest[PAD_BUTTON_NO_1],		m_iAX + 103, m_iAY + 166, m_iAX + 103 + 14, m_iAY + 166 + 14);
	SetRect(&m_rtButtonSetSkinDest[PAD_BUTTON_NO_2],		m_iAX + 159, m_iAY + 166, m_iAX + 159 + 14, m_iAY + 166 + 14);
	SetRect(&m_rtButtonSetSkinDest[PAD_BUTTON_NO_3],		m_iAX + 215, m_iAY + 166, m_iAX + 215 + 14, m_iAY + 166 + 14);
	SetRect(&m_rtButtonSetSkinDest[PAD_BUTTON_NO_4],		m_iAX + 271, m_iAY + 166, m_iAX + 271 + 14, m_iAY + 166 + 14);
	SetRect(&m_rtButtonSetSkinDest[PAD_BUTTON_NO_5],		m_iAX + 327, m_iAY + 166, m_iAX + 327 + 14, m_iAY + 166 + 14);
	SetRect(&m_rtButtonSetSkinDest[PAD_BUTTON_NO_6],		m_iAX + 47, m_iAY + 204, m_iAX + 47 + 14, m_iAY + 204 + 14);
	SetRect(&m_rtButtonSetSkinDest[PAD_BUTTON_NO_7],		m_iAX + 103, m_iAY + 204, m_iAX + 103 + 14, m_iAY + 204 + 14);
	SetRect(&m_rtButtonSetSkinDest[PAD_BUTTON_NO_8],		m_iAX + 159, m_iAY + 204, m_iAX + 159 + 14, m_iAY + 204 + 14);
	SetRect(&m_rtButtonSetSkinDest[PAD_BUTTON_NO_9],		m_iAX + 215, m_iAY + 204, m_iAX + 215 + 14, m_iAY + 204 + 14);
	SetRect(&m_rtButtonSetSkinDest[PAD_BUTTON_NO_10],		m_iAX + 271, m_iAY + 204, m_iAX + 271 + 14, m_iAY + 204 + 14);
	SetRect(&m_rtButtonSetSkinDest[PAD_BUTTON_NO_11],		m_iAX + 327, m_iAY + 204, m_iAX + 327 + 14, m_iAY + 204 + 14);

	SetRect(&m_rtButtonSetDest[PAD_BUTTON_NO_0],			m_rtButtonSetSkinDest[PAD_BUTTON_NO_0].left + 1, m_rtButtonSetSkinDest[PAD_BUTTON_NO_0].top + 1,	m_rtButtonSetDest[PAD_BUTTON_NO_0].left + 12, m_rtButtonSetDest[PAD_BUTTON_NO_0].top + 12);
	SetRect(&m_rtButtonSetDest[PAD_BUTTON_NO_1],			m_rtButtonSetSkinDest[PAD_BUTTON_NO_1].left + 1, m_rtButtonSetSkinDest[PAD_BUTTON_NO_1].top + 1,	m_rtButtonSetDest[PAD_BUTTON_NO_1].left + 12, m_rtButtonSetDest[PAD_BUTTON_NO_1].top + 12);
	SetRect(&m_rtButtonSetDest[PAD_BUTTON_NO_2],			m_rtButtonSetSkinDest[PAD_BUTTON_NO_2].left + 1, m_rtButtonSetSkinDest[PAD_BUTTON_NO_2].top + 1,	m_rtButtonSetDest[PAD_BUTTON_NO_2].left + 12, m_rtButtonSetDest[PAD_BUTTON_NO_2].top + 12);
	SetRect(&m_rtButtonSetDest[PAD_BUTTON_NO_3],			m_rtButtonSetSkinDest[PAD_BUTTON_NO_3].left + 1, m_rtButtonSetSkinDest[PAD_BUTTON_NO_3].top + 1,	m_rtButtonSetDest[PAD_BUTTON_NO_3].left + 12, m_rtButtonSetDest[PAD_BUTTON_NO_3].top + 12);
	SetRect(&m_rtButtonSetDest[PAD_BUTTON_NO_4],			m_rtButtonSetSkinDest[PAD_BUTTON_NO_4].left + 1, m_rtButtonSetSkinDest[PAD_BUTTON_NO_4].top + 1,	m_rtButtonSetDest[PAD_BUTTON_NO_4].left + 12, m_rtButtonSetDest[PAD_BUTTON_NO_4].top + 12);
	SetRect(&m_rtButtonSetDest[PAD_BUTTON_NO_5],			m_rtButtonSetSkinDest[PAD_BUTTON_NO_5].left + 1, m_rtButtonSetSkinDest[PAD_BUTTON_NO_5].top + 1,	m_rtButtonSetDest[PAD_BUTTON_NO_5].left + 12, m_rtButtonSetDest[PAD_BUTTON_NO_5].top + 12);
	SetRect(&m_rtButtonSetDest[PAD_BUTTON_NO_6],			m_rtButtonSetSkinDest[PAD_BUTTON_NO_6].left + 1, m_rtButtonSetSkinDest[PAD_BUTTON_NO_6].top + 1,	m_rtButtonSetDest[PAD_BUTTON_NO_6].left + 12, m_rtButtonSetDest[PAD_BUTTON_NO_6].top + 12);
	SetRect(&m_rtButtonSetDest[PAD_BUTTON_NO_7],			m_rtButtonSetSkinDest[PAD_BUTTON_NO_7].left + 1, m_rtButtonSetSkinDest[PAD_BUTTON_NO_7].top + 1,	m_rtButtonSetDest[PAD_BUTTON_NO_7].left + 12, m_rtButtonSetDest[PAD_BUTTON_NO_7].top + 12);
	SetRect(&m_rtButtonSetDest[PAD_BUTTON_NO_8],			m_rtButtonSetSkinDest[PAD_BUTTON_NO_8].left + 1, m_rtButtonSetSkinDest[PAD_BUTTON_NO_8].top + 1,	m_rtButtonSetDest[PAD_BUTTON_NO_8].left + 12, m_rtButtonSetDest[PAD_BUTTON_NO_8].top + 12);
	SetRect(&m_rtButtonSetDest[PAD_BUTTON_NO_9],			m_rtButtonSetSkinDest[PAD_BUTTON_NO_9].left + 1, m_rtButtonSetSkinDest[PAD_BUTTON_NO_9].top + 1,	m_rtButtonSetDest[PAD_BUTTON_NO_9].left + 12, m_rtButtonSetDest[PAD_BUTTON_NO_9].top + 12);
	SetRect(&m_rtButtonSetDest[PAD_BUTTON_NO_10],			m_rtButtonSetSkinDest[PAD_BUTTON_NO_10].left + 1, m_rtButtonSetSkinDest[PAD_BUTTON_NO_10].top + 1,	m_rtButtonSetDest[PAD_BUTTON_NO_10].left + 12, m_rtButtonSetDest[PAD_BUTTON_NO_10].top + 12);
	SetRect(&m_rtButtonSetDest[PAD_BUTTON_NO_11],			m_rtButtonSetSkinDest[PAD_BUTTON_NO_11].left + 1, m_rtButtonSetSkinDest[PAD_BUTTON_NO_11].top + 1,	m_rtButtonSetDest[PAD_BUTTON_NO_11].left + 12, m_rtButtonSetDest[PAD_BUTTON_NO_11].top + 12);
	
	//m_bButtonPush[PAD_BUTTON_NO_MAX];
	for(int i = 0; i < PAD_BUTTON_NO_MAX; i++) {
		m_bButtonPush[i] = false;
	}
	
	InitSubControl();

	m_iSetMode = JOYSTICK_SET_MODE_BASE;
}


void SPWindowJoyStick::InitSubControl()
{
	char* JOYSTIC_IMAGE_6 = "DATA/INTERFACE/CONCEPT/UI106.PNG";
	char* JOYSTIC_IMAGE_7 = "DATA/INTERFACE/CONCEPT/UI107.PNG";
	
	SPWindow* pStatic = NULL;
	SPWindow* pParent = NULL;
	SPWindowButton* pButton = NULL;	
	SPWindow* pBaseSet = NULL;
	SPWindow* pButtonSet = NULL;
	std::string strHelpMsg = "";
	
	//WIID_JOYSTICK_TITLE
	pStatic = new SPWindowStatic(WIID_JOYSTICK_TITLE, 7, 2, 155, 24, this);
	pStatic->SetImage(JOYSTIC_IMAGE_6, 1, 289);
	pStatic->Show();
	pStatic = NULL;

	//WIID_JOYSTICK_LINE1
	pStatic = new SPWindowStatic(WIID_JOYSTICK_LINE1, 2, 26, 385, 1, this);
	pStatic->SetImage(JOYSTIC_IMAGE_6, 340, 119);
	pStatic->SetSrcSize(3, 1);
	pStatic->Show();
	pStatic = NULL;

	//////////////////////////////////////////////////////////////////////////
	//WIID_JOYSTICK_BASE
	pBaseSet = new SPWindowStatic(WIID_JOYSTICK_BASE, 2, 27, 385, 413, this);
	pBaseSet->SetImage(JOYSTIC_IMAGE_6, 340, 122);
	pBaseSet->SetSrcSize(3, 3);
	pBaseSet->Show();
	m_pSetMode[JOYSTICK_SET_MODE_BASE] = pBaseSet;

	//WIID_JOYSTICK_BASE_TEXT
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BASE_TEXT, 15 - 2, 35 - 27, 107, 16, pBaseSet);
	pStatic->SetImage(JOYSTIC_IMAGE_6, 217, 118);
	pStatic->Show();
	pStatic = NULL;

	//WIID_JOYSTICK_BASE_BTN_TEST_LEFT
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BASE_BTN_TEST_LEFT, 15 - 2, 59 - 27, 3, 83, pBaseSet);
	pStatic->SetImage(JOYSTIC_IMAGE_6, 347, 302);
	pStatic->Show();
	pStatic = NULL;
	
	//WIID_JOYSTICK_BASE_BTN_TEST_CENTER
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BASE_BTN_TEST_CENTER, 18 - 2, 59 - 27, 61, 83, pBaseSet);
	pStatic->SetImage(JOYSTIC_IMAGE_6, 352, 302);
	pStatic->SetSrcSize(1, 385 - 302);
	pStatic->Show();
	pStatic = NULL;
	
	//WIID_JOYSTICK_BASE_BTN_TEST_RIGHT
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BASE_BTN_TEST_RIGHT, 79 - 2, 59 - 27, 3, 83, pBaseSet);
	pStatic->SetImage(JOYSTIC_IMAGE_6, 356, 302);
	pStatic->Show();
	pStatic = NULL;
	
	//WIID_JOYSTICK_BASE_BTN_TEST_TEXT
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BASE_BTN_TEST_TEXT, 17 - 2, 109 - 27, 63, 31, pBaseSet);
	pStatic->SetImage(JOYSTIC_IMAGE_6, 409, 377);
	pStatic->Show();
	pStatic = NULL;
	
	//WIID_JOYSTICK_BASE_PAD_IMAGE		
	pParent = new SPWindowStatic(WIID_JOYSTICK_BASE_PAD_IMAGE, 94 - 2, 59 - 27, 127, 151, pBaseSet);
	pParent->SetImage(JOYSTIC_IMAGE_6, 1, 137);
	pParent->Show();
	
	//WIID_JOYSTICK_BASE_PAD_MODE_CHACK	
	pButton = new SPWindowButton(WIID_JOYSTICK_BASE_PAD_MODE_CHACK, 113 -  94, 189 - 59, 12, 12, pParent);
	char* OPTION_IMAGE_FILE2 = "DATA/INTERFACE/CONCEPT/UI102.PNG";
	pButton->SetImage		(OPTION_IMAGE_FILE2, 204, 211);
	pButton->SetImageHit	(OPTION_IMAGE_FILE2, 217, 211);
	pButton->SetImagePush	(OPTION_IMAGE_FILE2, 230, 211);
	pButton->SetImageDisable(OPTION_IMAGE_FILE2, 239, 198);
	pButton->SetImageCheck	(OPTION_IMAGE_FILE2, 230, 211);						
	pButton->Show();
	m_pModeCheck = pButton;

	//WIID_JOYSTICK_BASE_FUNCKEY_TEXT_SKIN	
	SPWindowStatic* pFuncKey = new SPWindowStatic(WIID_JOYSTICK_BASE_FUNCKEY_TEXT_SKIN, 221 - 2, 59 - 27, 153, 15, pBaseSet);
	pFuncKey->SetWindowType(WND_TYPE_LEFTBASE);
	pFuncKey->SetImageNormalHead(JOYSTIC_IMAGE_6, 325, 119, 327, 134);
	pFuncKey->SetImageNormalBody(JOYSTIC_IMAGE_6, 329, 119, 331, 134);
	pFuncKey->SetImageNormalTail(JOYSTIC_IMAGE_6, 333, 119, 335, 134);	
	pFuncKey->Show();

	//WIID_JOYSTICK_BASE_FUNCKEY_TEXT
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BASE_FUNCKEY_TEXT, 223 - 221, 59 - 59, 149, 15, pFuncKey);
	pStatic->SetImage(JOYSTIC_IMAGE_6, 256, 390);
	pStatic->Show();
	pStatic = NULL;
	
	//WIID_JOYSTICK_BASE_FUNCKEY_1	
	pButton = new SPWindowButton(WIID_JOYSTICK_BASE_FUNCKEY + BUTTON_FUNC_0, 233 - 2, 81 - 27, 113, 22, pBaseSet);
	pButton->SetImage(JOYSTIC_IMAGE_6,			360,	285);
	pButton->SetImageHit(JOYSTIC_IMAGE_6,		360,	308);	//on
	pButton->SetImagePush(JOYSTIC_IMAGE_6,		360,	331);	//push
	pButton->SetImageDisable(JOYSTIC_IMAGE_6,	360,	354); //disable
	pButton->SetImageCheck(JOYSTIC_IMAGE_6,		360,	331);
	pButton->Show();
	m_pFunc[BUTTON_FUNC_0] = pButton;
	
	//WIID_JOYSTICK_BASE_FUNCKEY_2	
	pButton = new SPWindowButton(WIID_JOYSTICK_BASE_FUNCKEY + BUTTON_FUNC_1, 233 - 2, 107 - 27, 113, 22, pBaseSet);
	pButton->SetImage(JOYSTIC_IMAGE_6,			360,	193);
	pButton->SetImageHit(JOYSTIC_IMAGE_6,		360,	216);	//on
	pButton->SetImagePush(JOYSTIC_IMAGE_6,		360,	239);	//push
	pButton->SetImageDisable(JOYSTIC_IMAGE_6,	360,	262); //disable
	pButton->SetImageCheck(JOYSTIC_IMAGE_6,		360,	239);
	pButton->Show();
	m_pFunc[BUTTON_FUNC_1] = pButton;

	//WIID_JOYSTICK_BASE_MOUSEKEY_TEXT_SKIN
	SPWindowStatic* pMouseKey = new SPWindowStatic(WIID_JOYSTICK_BASE_MOUSEKEY_TEXT_SKIN, 221 - 2, 141 - 27, 153, 15, pBaseSet);
	pMouseKey->SetWindowType(WND_TYPE_LEFTBASE);
	pMouseKey->SetImageNormalHead(JOYSTIC_IMAGE_6, 327, 119, 329, 133);
	pMouseKey->SetImageNormalBody(JOYSTIC_IMAGE_6, 331, 119, 332, 133);
	pMouseKey->SetImageNormalTail(JOYSTIC_IMAGE_6, 333, 119, 335, 133);	
	pMouseKey->Show();
	
	//WIID_JOYSTICK_BASE_MOUSEKEY_TEXT	
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BASE_MOUSEKEY_TEXT, 223 - 221, 141 - 141, 149, 15, pMouseKey);
	pStatic->SetImage(JOYSTIC_IMAGE_6, 172, 286);
	pStatic->Show();
	pStatic = NULL;
	
	//WIID_JOYSTICK_BASE_MOUSEKEY_1	
	pButton = new SPWindowButton(WIID_JOYSTICK_BASE_MOUSEKEY + BUTTON_MOUSE_L, 233 - 2, 162 - 27, 113, 22, pBaseSet);
	pButton->SetImage(JOYSTIC_IMAGE_6,			360,	101);
	pButton->SetImageHit(JOYSTIC_IMAGE_6,		360,	124);	//on
	pButton->SetImagePush(JOYSTIC_IMAGE_6,		360,	147);	//push
	pButton->SetImageDisable(JOYSTIC_IMAGE_6,	360,	170); //disable
	pButton->SetImageCheck(JOYSTIC_IMAGE_6,		360,	147);
	pButton->Show();
	m_pMouse[BUTTON_MOUSE_L] = pButton;
	
	//WIID_JOYSTICK_BASE_MOUSEKEY_2	
	pButton = new SPWindowButton(WIID_JOYSTICK_BASE_MOUSEKEY + BUTTON_MOUSE_R, 233 - 2, 188 - 27, 113, 22, pBaseSet);
	pButton->SetImage(JOYSTIC_IMAGE_6,			360,	9);
	pButton->SetImageHit(JOYSTIC_IMAGE_6,		360,	32);	//on
	pButton->SetImagePush(JOYSTIC_IMAGE_6,		360,	55);	//push
	pButton->SetImageDisable(JOYSTIC_IMAGE_6,	360,	78); //disable
	pButton->SetImageCheck(JOYSTIC_IMAGE_6,		360,	55);
	pButton->Show();
	m_pMouse[BUTTON_MOUSE_R] = pButton;

	//WIID_JOYSTICK_BASE_HELP_SKIN_LEFT
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BASE_HELP_SKIN_LEFT, 16 - 2, 233 - 27, 3, 159, pBaseSet);
	pStatic->SetImage(JOYSTIC_IMAGE_6, 474, 9);	
	pStatic->Show();
	pStatic = NULL;
	
	//WIID_JOYSTICK_BASE_HELP_SKIN_CENTER
	pParent = new SPWindowStatic(WIID_JOYSTICK_BASE_BTN_TEST_LEFT, 19 - 2, 233 - 27, 352, 159, pBaseSet);
	pParent->SetImage(JOYSTIC_IMAGE_6, 479, 9);
	pParent->SetSrcSize(2, 159);
	pParent->Show();
	
	//WIID_JOYSTICK_BASE_HELP_SKIN_RIGHT
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BASE_BTN_TEST_LEFT, 371 - 2, 233 - 27, 3, 159, pBaseSet);
	pStatic->SetImage(JOYSTIC_IMAGE_6, 483, 9);
	pStatic->Show();
	pStatic = NULL;

	//WIID_JOYSTICK_BASE_HELP + JOYSTICK_BASE_HELP_0
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BASE_HELP + JOYSTICK_BASE_HELP_0, 21 - 19, 244 - 233, 350, 33, pParent);
	pStatic->SetFormat(DT_LEFT | DT_VCENTER);
	pStatic->SetMultiLine(true, 2);
	pStatic->Show();	
	strHelpMsg.clear();
	strHelpMsg = "JOYSTICK_BASE_HELP_1";
	if(g_pResourceManager->GetGlobalString(14100001)){
		strHelpMsg = g_pResourceManager->GetGlobalString(14100001);
	}
	pStatic->SetWindowText(strHelpMsg.c_str());
	m_pBaseHelp[JOYSTICK_BASE_HELP_0] = pStatic;
	pStatic = NULL;

	//WIID_JOYSTICK_BASE_HELP + JOYSTICK_BASE_HELP_1
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BASE_HELP + JOYSTICK_BASE_HELP_1, 21 - 19, 279 - 233, 350, 33, pParent);
	pStatic->SetFormat(DT_LEFT | DT_VCENTER);
	pStatic->SetMultiLine(true, 2);
	pStatic->Show();	
	strHelpMsg.clear();
	strHelpMsg = "JOYSTICK_BASE_HELP_2";
	if(g_pResourceManager->GetGlobalString(14100002)){
		strHelpMsg = g_pResourceManager->GetGlobalString(14100002);
	}
	pStatic->SetWindowText(strHelpMsg.c_str());
	m_pBaseHelp[JOYSTICK_BASE_HELP_1] = pStatic;
	pStatic = NULL;

	//WIID_JOYSTICK_BASE_HELP + JOYSTICK_BASE_HELP_2
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BASE_HELP + JOYSTICK_BASE_HELP_2, 21 - 19, 314 - 233, 350, 33, pParent);
	pStatic->SetFormat(DT_LEFT | DT_VCENTER);
	pStatic->SetMultiLine(true, 2);
	pStatic->Show();
	strHelpMsg.clear();
	strHelpMsg = "JOYSTICK_BASE_HELP_3";
	if(g_pResourceManager->GetGlobalString(14100003)){
		strHelpMsg = g_pResourceManager->GetGlobalString(14100003);
	}
	pStatic->SetWindowText(strHelpMsg.c_str());
	m_pBaseHelp[JOYSTICK_BASE_HELP_2] = pStatic;
	pStatic = NULL;

	//WIID_JOYSTICK_BASE_HELP + JOYSTICK_BASE_HELP_3
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BASE_HELP + JOYSTICK_BASE_HELP_3, 21 - 19, 349 - 233, 350, 33, pParent);
	pStatic->SetFormat(DT_LEFT | DT_VCENTER);
	pStatic->SetMultiLine(true, 2);
	pStatic->Show();
	strHelpMsg.clear();
	strHelpMsg = "JOYSTICK_BASE_HELP_4";
	if(g_pResourceManager->GetGlobalString(14100004)){
		strHelpMsg = g_pResourceManager->GetGlobalString(14100004);
	}
	pStatic->SetWindowText(strHelpMsg.c_str());
	m_pBaseHelp[JOYSTICK_BASE_HELP_3] = pStatic;
	pStatic = NULL;

	//WIID_JOYSTICK_BASE_NEXT
	pButton = new SPWindowButton(WIID_JOYSTICK_BASE_NEXT, 50 - 2, 414 - 27, 68, 17, pBaseSet);
	pButton->SetImage(JOYSTIC_IMAGE_6,			70,		314);
	pButton->SetImageHit(JOYSTIC_IMAGE_6,		70,		332);	//on
	pButton->SetImagePush(JOYSTIC_IMAGE_6,		70,		350);	//push
	pButton->SetImageDisable(JOYSTIC_IMAGE_6,	70,		368); //disable
	pButton->SetImageCheck(JOYSTIC_IMAGE_6,		70,		350);
	pButton->Show();
	m_pButtonNext = pButton;
	
	//WIID_JOYSTICK_BASE_INIT
	pButton = new SPWindowButton(WIID_JOYSTICK_BASE_INIT, 122 - 2, 414 - 27, 68, 17, pBaseSet);
	pButton->SetImage(JOYSTIC_IMAGE_6,			139,		314);
	pButton->SetImageHit(JOYSTIC_IMAGE_6,		139,		332);	//on
	pButton->SetImagePush(JOYSTIC_IMAGE_6,		139,		350);	//push
	pButton->SetImageDisable(JOYSTIC_IMAGE_6,	139,		368); //disable
	pButton->SetImageCheck(JOYSTIC_IMAGE_6,		139,		350);
	pButton->Show();
	m_pButtonReset = pButton;
	
	//WIID_JOYSTICK_BASE_OK	
	pButton = new SPWindowButton(WIID_JOYSTICK_BASE_OK, 194 - 2, 414 - 27, 68, 17, pBaseSet);
	pButton->SetImage(JOYSTIC_IMAGE_6,			208,		314);
	pButton->SetImageHit(JOYSTIC_IMAGE_6,		208,		332);	//on
	pButton->SetImagePush(JOYSTIC_IMAGE_6,		208,		350);	//push
	pButton->SetImageDisable(JOYSTIC_IMAGE_6,	208,		368); //disable
	pButton->SetImageCheck(JOYSTIC_IMAGE_6,		208,		350);
	pButton->Show();
	m_pButtonOk = pButton;
	
	//WIID_JOYSTICK_BASE_CANCEL	
	pButton = new SPWindowButton(WIID_JOYSTICK_BASE_CANCEL, 266 - 2, 414 - 27, 68, 17, pBaseSet);
	pButton->SetImage(JOYSTIC_IMAGE_6,			277,		314);
	pButton->SetImageHit(JOYSTIC_IMAGE_6,		277,		332);	//on
	pButton->SetImagePush(JOYSTIC_IMAGE_6,		277,		350);	//push
	pButton->SetImageDisable(JOYSTIC_IMAGE_6,	277,		368); //disable
	pButton->SetImageCheck(JOYSTIC_IMAGE_6,		277,		350);
	pButton->Show();
	

	//////////////////////////////////////////////////////////////////////////
	//WIID_JOYSTICK_BUTTON
	pButtonSet = new SPWindowStatic(WIID_JOYSTICK_BUTTON, 2, 27, 385, 413, this);
	pButtonSet->SetImage(JOYSTIC_IMAGE_6, 340, 122);
	pButtonSet->SetSrcSize(3, 3);
	pButtonSet->Show();
	m_pSetMode[JOYSTICK_SET_MODE_BUTTON] = pButtonSet;

	//WIID_JOYSTICK_BUTTON_TEXT
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_TEXT, 15 - 2, 35 - 27, 107, 16, pButtonSet);
	pStatic->SetImage(JOYSTIC_IMAGE_6, 1, 118);
	pStatic->Show();
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_BTN_TEST_LEFT
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_BTN_TEST_LEFT, 15 - 2, 59 - 27, 3, 83, pButtonSet);
	pStatic->SetImage(JOYSTIC_IMAGE_6, 347, 302);
	pStatic->Show();
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_BTN_TEST_CENTER
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_BTN_TEST_CENTER, 18 - 2, 59 - 27, 61, 83, pButtonSet);
	pStatic->SetImage(JOYSTIC_IMAGE_6, 352, 302);
	pStatic->SetSrcSize(1, 385 - 302);
	pStatic->Show();
	pStatic = NULL;
	
	//WIID_JOYSTICK_BUTTON_BTN_TEST_RIGHT
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_BTN_TEST_CENTER, 79 - 2, 59 - 27, 3, 83, pButtonSet);
	pStatic->SetImage(JOYSTIC_IMAGE_6, 356, 302);
	pStatic->Show();
	pStatic = NULL;
	
	//WIID_JOYSTICK_BUTTON_BTN_TEST_TEXT
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_BTN_TEST_TEXT, 17 - 2, 109 - 27, 63, 31, pButtonSet);
	pStatic->SetImage(JOYSTIC_IMAGE_6, 409, 377);
	pStatic->Show();
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_HELP1_SKIN_LEFT	
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_HELP1_SKIN_LEFT, 87 - 2, 59 - 27, 3, 83, pButtonSet);
	pStatic->SetImage(JOYSTIC_IMAGE_6, 347, 218);
	pStatic->Show();
	pStatic = NULL;
	
	//WIID_JOYSTICK_BUTTON_HELP1_SKIN_CENTER	
	pParent = new SPWindowStatic(WIID_JOYSTICK_BUTTON_HELP1_SKIN_CENTER, 90 - 2, 59 - 27, 281, 83, pButtonSet);
	pParent->SetImage(JOYSTIC_IMAGE_6, 352, 218);
	pParent->SetSrcSize(2, 83);
	pParent->Show();
	
	//WIID_JOYSTICK_BUTTON_HELP1_SKIN_RIGHT	
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_HELP1_SKIN_RIGHT, 371 - 2, 59 - 27, 3, 83, pButtonSet);
	pStatic->SetImage(JOYSTIC_IMAGE_6, 356, 218);
	pStatic->Show();
	pStatic = NULL;

	//WIID_JOYSTICK_BITTON_HELP1
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_HELP1, 89 - 90, 82 - 59, 282, 33, pParent);
	pStatic->SetFormat(DT_LEFT | DT_VCENTER);
	pStatic->SetMultiLine(true, 2);
	strHelpMsg.clear();
	strHelpMsg = "JOYSTICK_BASE_HELP_1";
	if(g_pResourceManager->GetGlobalString(14100005)){
		strHelpMsg = g_pResourceManager->GetGlobalString(14100005);
	}
	pStatic->SetWindowText(strHelpMsg.c_str());
	pStatic->Show();
	pStatic = NULL;
	
	//WIID_JOYSTICK_BUTTON_KEY_LEFT
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEY_LEFT, 15 - 2, 145 - 27, 3, 97, pButtonSet);
	pStatic->SetImage(JOYSTIC_IMAGE_6, 334, 203);
	pStatic->Show();
	pStatic = NULL;
	
	//WIID_JOYSTICK_BUTTON_KEY_CENTER
	pParent = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEY_CENTER, 18 - 2, 145 - 27, 353, 97, pButtonSet);
	pParent->SetImage(JOYSTIC_IMAGE_6, 339, 203);
	pParent->SetSrcSize(2, 97);
	pParent->Show();

	//WIID_JOYSTICK_BUTTON_KEY_RIGHT
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEY_RIGHT, 371 - 2, 145 - 27, 3, 97, pButtonSet);
	pStatic->SetImage(JOYSTIC_IMAGE_6, 343, 203);	
	pStatic->Show();
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_NORMAL
	pButton = new SPWindowButton(WIID_JOYSTICK_BUTTON_NORMAL + PAD_BUTTONTYPE_NORMAL, 18 - 18, 146 - 145, 115, 15, pParent);
	pButton->SetImage(JOYSTIC_IMAGE_6,			1,		53);
	pButton->SetImageHit(JOYSTIC_IMAGE_6,		1,		69);	//on
	pButton->SetImagePush(JOYSTIC_IMAGE_6,		1,		85);	//push
	pButton->SetImageDisable(JOYSTIC_IMAGE_6,	1,		101); //disable
	pButton->SetImageCheck(JOYSTIC_IMAGE_6,		1,		85);
	pButton->Show();
	m_pButtonMode[PAD_BUTTONTYPE_NORMAL] = pButton;

	//WIID_JOYSTICK_BUTTON_FUNC1
	pButton = new SPWindowButton(WIID_JOYSTICK_BUTTON_NORMAL + PAD_BUTTONTYPE_FUNC1, 137 - 18, 146 - 145, 115, 15, pParent);
	pButton->SetImage(JOYSTIC_IMAGE_6,			117,	53);
	pButton->SetImageHit(JOYSTIC_IMAGE_6,		117,	69);	//on
	pButton->SetImagePush(JOYSTIC_IMAGE_6,		117,	85);	//push
	pButton->SetImageDisable(JOYSTIC_IMAGE_6,	117,	101); //disable
	pButton->SetImageCheck(JOYSTIC_IMAGE_6,		117,	85);
	pButton->Show();
	m_pButtonMode[PAD_BUTTONTYPE_FUNC1] = pButton;

	//WIID_JOYSTICK_BUTTON_FUNC2
	pButton = new SPWindowButton(WIID_JOYSTICK_BUTTON_NORMAL + PAD_BUTTONTYPE_FUNC2, 256 - 18, 146 - 145, 115, 15, pParent);
	pButton->SetImage(JOYSTIC_IMAGE_6,			233,	53);
	pButton->SetImageHit(JOYSTIC_IMAGE_6,		233,	69);	//on
	pButton->SetImagePush(JOYSTIC_IMAGE_6,		233,	85);	//push
	pButton->SetImageDisable(JOYSTIC_IMAGE_6,	233,	101); //disable
	pButton->SetImageCheck(JOYSTIC_IMAGE_6,		233,	85);
	pButton->Show();
	m_pButtonMode[PAD_BUTTONTYPE_FUNC2] = pButton;

	//WIID_JOYSTICK_BUTTON_KEY
	int iPosX = 34;
	int iPosY = 182;
	for(int i = 0; i < PAD_BUTTON_NO_6; i++) {
		pButton = new SPWindowButton(WIID_JOYSTICK_BUTTON_KEY + i, iPosX - 18, iPosY - 145, 40, 18, pParent);
		SetKeyButton(pButton);
		pButton->Show();
		m_pButtonKey[i] = pButton;
		iPosX +=56;
	}

	iPosX = 34;
	iPosY = 220;
	for(int i = PAD_BUTTON_NO_6; i < PAD_BUTTON_NO_MAX; i++) {
		pButton = new SPWindowButton(WIID_JOYSTICK_BUTTON_KEY + i, iPosX - 18, iPosY - 145, 40, 18, pParent);
		SetKeyButton(pButton);
		pButton->Show();
		m_pButtonKey[i] = pButton;
		iPosX +=56;
	}
	
	//WIID_JOYSTICK_BUTTON_PREV	
	pButton = new SPWindowButton(WIID_JOYSTICK_BUTTON_PREV, 50 - 2, 414 - 27, 68, 17, pButtonSet);
	pButton->SetImage(JOYSTIC_IMAGE_6,			1,		314);
	pButton->SetImageHit(JOYSTIC_IMAGE_6,		1,		332);	//on
	pButton->SetImagePush(JOYSTIC_IMAGE_6,		1,		350);	//push
	pButton->SetImageDisable(JOYSTIC_IMAGE_6,	1,		368); //disable
	pButton->SetImageCheck(JOYSTIC_IMAGE_6,		1,		350);
	pButton->Show();
	
	//WIID_JOYSTICK_BUTTON_INIT	
	pButton = new SPWindowButton(WIID_JOYSTICK_BUTTON_INIT, 122 - 2, 414 - 27, 68, 17, pButtonSet);
	pButton->SetImage(JOYSTIC_IMAGE_6,			139,		314);
	pButton->SetImageHit(JOYSTIC_IMAGE_6,		139,		332);	//on
	pButton->SetImagePush(JOYSTIC_IMAGE_6,		139,		350);	//push
	pButton->SetImageDisable(JOYSTIC_IMAGE_6,	139,		368); //disable
	pButton->SetImageCheck(JOYSTIC_IMAGE_6,		139,		350);
	pButton->Show();
	
	//WIID_JOYSTICK_BUTTON_OK		
	pButton = new SPWindowButton(WIID_JOYSTICK_BUTTON_OK, 194 - 2, 414 - 27, 68, 17, pButtonSet);
	pButton->SetImage(JOYSTIC_IMAGE_6,			208,		314);
	pButton->SetImageHit(JOYSTIC_IMAGE_6,		208,		332);	//on
	pButton->SetImagePush(JOYSTIC_IMAGE_6,		208,		350);	//push
	pButton->SetImageDisable(JOYSTIC_IMAGE_6,	208,		368); //disable
	pButton->SetImageCheck(JOYSTIC_IMAGE_6,		208,		350);
	pButton->Show();
	
	//WIID_JOYSTICK_BUTTON_CANCEL	
	pButton = new SPWindowButton(WIID_JOYSTICK_BUTTON_CANCEL, 266 - 2, 414 - 27, 68, 17, pButtonSet);
	pButton->SetImage(JOYSTIC_IMAGE_6,			277,		314);
	pButton->SetImageHit(JOYSTIC_IMAGE_6,		277,		332);	//on
	pButton->SetImagePush(JOYSTIC_IMAGE_6,		277,		350);	//push
	pButton->SetImageDisable(JOYSTIC_IMAGE_6,	277,		368); //disable
	pButton->SetImageCheck(JOYSTIC_IMAGE_6,		277,		350);	
	pButton->Show();

	//WIID_JOYSTICK_BUTTON_INFO_TEXT_IMAGE
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_INFO_TEXT_IMAGE, 15 - 2, 247 - 27, 107, 16, pButtonSet);
	pStatic->SetImage(JOYSTIC_IMAGE_6, 109, 118);
	pStatic->Show();
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_IMAGE
	pParent = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_IMAGE, 59 - 2, 268 - 27, 264, 97, pButtonSet);
	pParent->SetImage(JOYSTIC_IMAGE_7, 1, 414);	
	pParent->Show();

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_INTERFACE_ESC, 64 - 59, 273 - 268, 23, 13, pParent);	
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;	

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY 1
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_QUICK_S1, 78 - 59, 290 - 268, 13, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY 2
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_QUICK_S2, 92 - 59, 290 - 268, 13, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY 3
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_QUICK_S3, 106 - 59, 290 - 268, 13, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY 4
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_QUICK_S4, 120 - 59, 290 - 268, 13, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY 5
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_QUICK_S5, 134 - 59, 290 - 268, 13, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY 6
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_QUICK_S6, 148 - 59, 290 - 268, 13, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY 7
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_QUICK_S7, 162 - 59, 290 - 268, 13, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY 8
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_QUICK_S8, 176 - 59, 290 - 268, 13, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY 9
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_QUICK_S9, 190 - 59, 290 - 268, 13, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY 0
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_QUICK_S10, 204 - 59, 290 - 268, 13, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY -
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_QUICK_S11, 218 - 59, 290 - 268, 13, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY ^
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_QUICK_S12, 232 - 59, 290 - 268, 13, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY F1
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_QUICK_F1, 92 - 59, 273 - 268, 13, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY F2
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_QUICK_F2, 106 - 59, 273 - 268, 13, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;


	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY Tab
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_INTERFACE_TAB, 64 - 59, 304 - 268, 20, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY Q
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_INTERFACE_Q, 85 - 59, 304 - 268, 13, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY W
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_INTERFACE_W, 99 - 59, 304 - 268, 13, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY E
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_INTERFACE_E, 113 - 59, 304 - 268, 13, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY T
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_INTERFACE_T, 140 - 59, 304 - 268, 13, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY Y
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_INTERFACE_Y, 154 - 59, 304 - 268, 13, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY U
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_INTERFACE_U, 168 - 59, 304 - 268, 13, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY I
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_INTERFACE_I, 182 - 59, 304 - 268, 13, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY O
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_INTERFACE_O, 196 - 59, 304 - 268, 13, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY P
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_INTERFACE_P, 210 - 59, 304 - 268, 13, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY A
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_QUICK_1, 92 - 59, 318 - 268, 13, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY S
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_QUICK_2, 106 - 59, 318 - 268, 13, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY D
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_QUICK_3, 120 - 59, 318 - 268, 13, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY F
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_QUICK_4, 134 - 59, 318 - 268, 13, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY G
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_INTERFACE_G, 148 - 59, 318 - 268, 13, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY H
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_INTERFACE_H, 162 - 59, 318 - 268, 13, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY K
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_INTERFACE_K, 190 - 59, 318 - 268, 13, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY L
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_INTERFACE_L, 204 - 59, 318 - 268, 13, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY SHIFT
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_QUICK_6, 64 - 59, 332 - 268, 34, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY Z
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_QUICK_7, 99 - 59, 332 - 268, 13, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY X
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_QUICK_8, 113 - 59, 332 - 268, 13, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY C
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_QUICK_9, 127 - 59, 332 - 268, 13, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY V
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_QUICK_10, 141 - 59, 332 - 268, 13, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY B
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_INTERFACE_B, 155 - 59, 332 - 268, 13, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY N
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_INTERFACE_N, 169 - 59, 332 - 268, 13, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY M
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_INTERFACE_M, 183 - 59, 332 - 268, 13, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY ALT
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_QUICK_5, 64 - 59, 346 - 268, 20, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY CTRL
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_QUICK_0, 99 - 59, 346 - 268, 20, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY SPACE
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_QUICK_11, 134 - 59, 346 - 268, 55, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY LEFT
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_MOUSE_MAX + 1, 277 - 59, 346 - 268, 13, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY DOWN
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_MOUSE_MAX + 2, 291 - 59, 346 - 268, 13, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY RIGHT
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_MOUSE_MAX + 3, 305 - 59, 346 - 268, 13, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_KEYBOARD_KEY Up
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_KEYBOARD_KEY + BUTTON_KEY_MOUSE_MAX + 4, 291 - 59, 332 - 268, 13, 13, pParent);
	pStatic->Show();
	m_vpKeyButton.push_back(pStatic);
	pStatic = NULL;

	//WIID_JOYSTICK_BUTTON_HELP2_SKIN_LEFT	
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_HELP2_SKIN_LEFT, 59 - 2, 368 - 27, 2, 35, pButtonSet);
	pStatic->SetImage(JOYSTIC_IMAGE_6, 348, 182);
	pStatic->Show();
	pStatic = NULL;
	
	//WIID_JOYSTICK_BUTTON_HELP2_SKIN_CENTER	
	pParent = new SPWindowStatic(WIID_JOYSTICK_BUTTON_HELP2_SKIN_CENTER, 61 - 2, 368 - 27, 261, 35, pButtonSet);
	pParent->SetImage(JOYSTIC_IMAGE_6, 352, 182);
	pParent->SetSrcSize(2, 35);
	pParent->Show();
	
	//WIID_JOYSTICK_BUTTON_HELP2_SKIN_RIGHT	
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_HELP2_SKIN_CENTER, 322 - 2, 368 - 27, 2, 35, pButtonSet);
	pStatic->SetImage(JOYSTIC_IMAGE_6, 356, 182);
	pStatic->Show();
	pStatic = NULL;
	
	//WIID_JOYSTICK_BUTTON_HELP2	
	pStatic = new SPWindowStatic(WIID_JOYSTICK_BUTTON_HELP2, 63 - 61, 370 - 368, 257, 31, pParent);
	pStatic->SetFormat(DT_LEFT | DT_VCENTER);
	pStatic->SetMultiLine(true, 2);
	strHelpMsg.clear();
	strHelpMsg = "JOYSTICK_BASE_HELP_2";
	if(g_pResourceManager->GetGlobalString(14100006)){
		strHelpMsg = g_pResourceManager->GetGlobalString(14100006);
	}
	pStatic->SetWindowText(strHelpMsg.c_str());
	pStatic->Show();
	pStatic = NULL;
}


void SPWindowJoyStick::SetKeyButton(SPWindowButton* pButton)
{
	char* JOYSTIC_IMAGE_6 = "DATA/INTERFACE/CONCEPT/UI106.PNG";
	if(pButton) {
		pButton->SetImage(JOYSTIC_IMAGE_6,			130,		136);
		pButton->SetImageHit(JOYSTIC_IMAGE_6,		130,		155);	//on
		pButton->SetImagePush(JOYSTIC_IMAGE_6,		130,		174);	//push
		pButton->SetImageDisable(JOYSTIC_IMAGE_6,	130,		193); //disable
		pButton->SetImageCheck(JOYSTIC_IMAGE_6,		130,		174);	
	}	
}


void SPWindowJoyStick::UpdateSubControl()
{
	if(m_iSetMode == JOYSTICK_SET_MODE_BASE) {
		m_pSetMode[JOYSTICK_SET_MODE_BUTTON]->Hide();
		m_pSetMode[JOYSTICK_SET_MODE_BASE]->Show();

		if(m_pCopyInfo->m_iJoyStickMode == JOYSTICK_MODE_ANALOG){
			m_pModeCheck->SPSendMessage(SPIM_BTN_SELECT,			true);
			for(int i = 0; i < BUTTON_MOUSE_MAX; i++) {
				m_pMouse[i]->SetEnable(true);
			}
		}
		else if(m_pCopyInfo->m_iJoyStickMode == JOYSTICK_MODE_DIGITAL) {
			m_pModeCheck->SPSendMessage(SPIM_BTN_SELECT,			false);
			for(int i = 0; i < BUTTON_MOUSE_MAX; i++) {
				m_pMouse[i]->SetEnable(false);
			}
		}

		for(int i = 0; i < BUTTON_FUNC_MAX; i++) {			
			m_pFunc[i]->SPSendMessage(SPIM_BTN_SELECT,			false);
		}

		for(int i = 0; i < BUTTON_MOUSE_MAX; i++) {			
			m_pMouse[i]->SPSendMessage(SPIM_BTN_SELECT,			false);
		}

		m_pManager->SetKeyPass(false);
	}
	else if(m_iSetMode == JOYSTICK_SET_MODE_BUTTON) {
		m_pSetMode[JOYSTICK_SET_MODE_BASE]->Hide();
		m_pSetMode[JOYSTICK_SET_MODE_BUTTON]->Show();
		
		for(int i = 0; i < PAD_BUTTONTYPE_MAX; i++) {
			if(i == m_iButtonSetType) {
				m_pButtonMode[i]->SPSendMessage(SPIM_BTN_SELECT,			true);
			}
			else {
				m_pButtonMode[i]->SPSendMessage(SPIM_BTN_SELECT,			false);
			}
		}

		if(m_pCopyInfo->m_iFunc[BUTTON_FUNC_0] == PAD_BUTTON_NO_MAX) {
			m_pButtonMode[PAD_BUTTONTYPE_FUNC1]->SetEnable(false);
		}
		else {
			m_pButtonMode[PAD_BUTTONTYPE_FUNC1]->SetEnable(true);
		}

		if(m_pCopyInfo->m_iFunc[BUTTON_FUNC_1] == PAD_BUTTON_NO_MAX) {
			m_pButtonMode[PAD_BUTTONTYPE_FUNC2]->SetEnable(false);
		}
		else {
			m_pButtonMode[PAD_BUTTONTYPE_FUNC2]->SetEnable(true);
		}

		m_pManager->SetKeyPass(true);

		UpdateKeyList();
	}
}


void SPWindowJoyStick::UpdateKeyList()
{
	std::vector<SPJoyPadButtonInfo*>::iterator iter;
	iter = m_pCopyInfo->m_vpButtonSetting[m_iButtonSetType].begin();
	int iIndex = 0;
	for(; iter != m_pCopyInfo->m_vpButtonSetting[m_iButtonSetType].end(); ++iter, iIndex++) {		
		m_pButtonKey[iIndex]->SetWindowText(NULL);
		
		if(iIndex == m_pCopyInfo->m_iFunc[BUTTON_FUNC_0] || iIndex == m_pCopyInfo->m_iFunc[BUTTON_FUNC_1] ||
			iIndex == m_pCopyInfo->m_iMouse[BUTTON_MOUSE_L] || iIndex == m_pCopyInfo->m_iMouse[BUTTON_MOUSE_R]) 
		{
			m_pButtonKey[iIndex]->SetEnable(false);
			continue;
		}
		else {
			m_pButtonKey[iIndex]->SetEnable(true);
		}
		
		if(m_iButtonSetType == PAD_BUTTONTYPE_NORMAL) {			
			if(g_pInputManager->GetJoyPadManager()->GetKeyName((*iter)->m_iKey))
				m_pButtonKey[iIndex]->SetWindowText(g_pInputManager->GetJoyPadManager()->GetKeyName((*iter)->m_iKey));
		}
		else {
			if(g_pInputManager->GetJoyPadManager()->GetKeyName((*iter)->m_iKey))
				m_pButtonKey[iIndex]->SetWindowText(g_pInputManager->GetJoyPadManager()->GetKeyName((*iter)->m_iKey));
		}
	}
}


void SPWindowJoyStick::Show(bool bWithChild /*= true*/)
{
	m_pManager = g_pInputManager->GetJoyPadManager();
	if(m_pManager == NULL) 
		return;	
	
	SPWindow::Show(bWithChild);

	if(!g_pInput->IsEquipJoystick()) {
		m_pButtonNext->SetEnable(false);
		m_pButtonOk->SetEnable(false);
		m_pButtonReset->SetEnable(false);
	}
	else {
		m_pButtonNext->SetEnable(true);
		m_pButtonOk->SetEnable(true);
		m_pButtonReset->SetEnable(true);
	}

	SPPadSettingInfo* pSrcInfo = m_pManager->GetPadSettingInfo();
	if(pSrcInfo && m_pCopyInfo) {
		m_pCopyInfo->CopySettingInfo(pSrcInfo);
	}

	m_pManager->SetSelectKey(JOYSTICK_SELECT_NULL);
	m_iButtonSetNo = PAD_BUTTON_NO_MAX;
	m_iButtonSetType = PAD_BUTTONTYPE_NORMAL;
	m_pManager->SetButtonSetMode(true);

	m_iSetMode = JOYSTICK_SET_MODE_BASE;
	UpdateSubControl();
}


void SPWindowJoyStick::Hide(bool bSendServer /*= true*/)
{
	m_pManager->SetButtonSetMode(false);
	m_pManager->SetKeyPass(false);
	SPWindow::Hide(bSendServer);
}


void SPWindowJoyStick::Clean()
{	
	SAFE_DELETE(m_pCopyInfo);
	SAFE_RELEASE(m_pTextureBase);
	SAFE_RELEASE(m_pTextureButton);
	SPWindow::Clean();
	m_vpKeyButton.clear();
}


void SPWindowJoyStick::Process(float fTime)
{	
	if(!m_bShow)		return;

	SetRect(&m_rtBaseDest[WINDOW_BASE_TOP_LEFT],		m_iAX, m_iAY, m_iAX + 4, m_iAY + 4);
	SetRect(&m_rtBaseDest[WINDOW_BASE_TOP],			m_iAX + 4, m_iAY, m_iAX + 4 + 381, m_iAY + 4);
	SetRect(&m_rtBaseDest[WINDOW_BASE_TOP_RIGHT],		m_iAX + 385, m_iAY, m_iAX + 385 + 4, m_iAY + 4);
	SetRect(&m_rtBaseDest[WINDOW_BASE_CENTER_LEFT],	m_iAX, m_iAY + 4, m_iAX + 4, m_iAY + 4 + 436);
	SetRect(&m_rtBaseDest[WINDOW_BASE_CENTER],		m_iAX + 4, m_iAY + 4, m_iAX + 4 + 381, m_iAY + 4 + 436);
	SetRect(&m_rtBaseDest[WINDOW_BASE_CENTER_RIGHT],	m_iAX + 385, m_iAY + 4, m_iAX + 385 + 4, m_iAY + 4 + 436);
	SetRect(&m_rtBaseDest[WINDOW_BASE_BOTTOM_LEFT],	m_iAX, m_iAY + 440, m_iAX + 4, m_iAY + 440 + 4);
	SetRect(&m_rtBaseDest[WINDOW_BASE_BOTTOM],		m_iAX + 4, m_iAY + 440, m_iAX + 4 + 381, m_iAY + 440 + 4);
	SetRect(&m_rtBaseDest[WINDOW_BASE_BOTTOM_RIGHT],	m_iAX + 385, m_iAY + 440, m_iAX + 385 + 4, m_iAY + 440 + 4);

	SetRect(&m_rtButtonSkinDest[PAD_BUTTON_NO_0],		m_iAX + 19, m_iAY + 62, m_iAX + 19 + 14, m_iAY + 62 + 14);
	SetRect(&m_rtButtonSkinDest[PAD_BUTTON_NO_1],		m_iAX + 34, m_iAY + 62, m_iAX + 34 + 14, m_iAY + 62 + 14);
	SetRect(&m_rtButtonSkinDest[PAD_BUTTON_NO_2],		m_iAX + 49, m_iAY + 62, m_iAX + 49 + 14, m_iAY + 62 + 14);
	SetRect(&m_rtButtonSkinDest[PAD_BUTTON_NO_3],		m_iAX + 64, m_iAY + 62, m_iAX + 64 + 14, m_iAY + 62 + 14);
	SetRect(&m_rtButtonSkinDest[PAD_BUTTON_NO_4],		m_iAX + 19, m_iAY + 77, m_iAX + 19 + 14, m_iAY + 77 + 14);
	SetRect(&m_rtButtonSkinDest[PAD_BUTTON_NO_5],		m_iAX + 34, m_iAY + 77, m_iAX + 34 + 14, m_iAY + 77 + 14);
	SetRect(&m_rtButtonSkinDest[PAD_BUTTON_NO_6],		m_iAX + 49, m_iAY + 77, m_iAX + 49 + 14, m_iAY + 77 + 14);
	SetRect(&m_rtButtonSkinDest[PAD_BUTTON_NO_7],		m_iAX + 64, m_iAY + 77, m_iAX + 64 + 14, m_iAY + 77 + 14);
	SetRect(&m_rtButtonSkinDest[PAD_BUTTON_NO_8],		m_iAX + 19, m_iAY + 92, m_iAX + 19 + 14, m_iAY + 92 + 14);
	SetRect(&m_rtButtonSkinDest[PAD_BUTTON_NO_9],		m_iAX + 34, m_iAY + 92, m_iAX + 34 + 14, m_iAY + 92 + 14);
	SetRect(&m_rtButtonSkinDest[PAD_BUTTON_NO_10],		m_iAX + 49, m_iAY + 92, m_iAX + 49 + 14, m_iAY + 92 + 14);
	SetRect(&m_rtButtonSkinDest[PAD_BUTTON_NO_11],		m_iAX + 64, m_iAY + 92, m_iAX + 64 + 14, m_iAY + 92 + 14);
	
	SetRect(&m_rtButtonDest[PAD_BUTTON_NO_0],			m_rtButtonSkinDest[PAD_BUTTON_NO_0].left + 1, m_rtButtonSkinDest[PAD_BUTTON_NO_0].top + 1,	m_rtButtonDest[PAD_BUTTON_NO_0].left + 12, m_rtButtonDest[PAD_BUTTON_NO_0].top + 12);
	SetRect(&m_rtButtonDest[PAD_BUTTON_NO_1],			m_rtButtonSkinDest[PAD_BUTTON_NO_1].left + 1, m_rtButtonSkinDest[PAD_BUTTON_NO_1].top + 1,	m_rtButtonDest[PAD_BUTTON_NO_1].left + 12, m_rtButtonDest[PAD_BUTTON_NO_1].top + 12);
	SetRect(&m_rtButtonDest[PAD_BUTTON_NO_2],			m_rtButtonSkinDest[PAD_BUTTON_NO_2].left + 1, m_rtButtonSkinDest[PAD_BUTTON_NO_2].top + 1,	m_rtButtonDest[PAD_BUTTON_NO_2].left + 12, m_rtButtonDest[PAD_BUTTON_NO_2].top + 12);
	SetRect(&m_rtButtonDest[PAD_BUTTON_NO_3],			m_rtButtonSkinDest[PAD_BUTTON_NO_3].left + 1, m_rtButtonSkinDest[PAD_BUTTON_NO_3].top + 1,	m_rtButtonDest[PAD_BUTTON_NO_3].left + 12, m_rtButtonDest[PAD_BUTTON_NO_3].top + 12);
	SetRect(&m_rtButtonDest[PAD_BUTTON_NO_4],			m_rtButtonSkinDest[PAD_BUTTON_NO_4].left + 1, m_rtButtonSkinDest[PAD_BUTTON_NO_4].top + 1,	m_rtButtonDest[PAD_BUTTON_NO_4].left + 12, m_rtButtonDest[PAD_BUTTON_NO_4].top + 12);
	SetRect(&m_rtButtonDest[PAD_BUTTON_NO_5],			m_rtButtonSkinDest[PAD_BUTTON_NO_5].left + 1, m_rtButtonSkinDest[PAD_BUTTON_NO_5].top + 1,	m_rtButtonDest[PAD_BUTTON_NO_5].left + 12, m_rtButtonDest[PAD_BUTTON_NO_5].top + 12);
	SetRect(&m_rtButtonDest[PAD_BUTTON_NO_6],			m_rtButtonSkinDest[PAD_BUTTON_NO_6].left + 1, m_rtButtonSkinDest[PAD_BUTTON_NO_6].top + 1,	m_rtButtonDest[PAD_BUTTON_NO_6].left + 12, m_rtButtonDest[PAD_BUTTON_NO_6].top + 12);
	SetRect(&m_rtButtonDest[PAD_BUTTON_NO_7],			m_rtButtonSkinDest[PAD_BUTTON_NO_7].left + 1, m_rtButtonSkinDest[PAD_BUTTON_NO_7].top + 1,	m_rtButtonDest[PAD_BUTTON_NO_7].left + 12, m_rtButtonDest[PAD_BUTTON_NO_7].top + 12);
	SetRect(&m_rtButtonDest[PAD_BUTTON_NO_8],			m_rtButtonSkinDest[PAD_BUTTON_NO_8].left + 1, m_rtButtonSkinDest[PAD_BUTTON_NO_8].top + 1,	m_rtButtonDest[PAD_BUTTON_NO_8].left + 12, m_rtButtonDest[PAD_BUTTON_NO_8].top + 12);
	SetRect(&m_rtButtonDest[PAD_BUTTON_NO_9],			m_rtButtonSkinDest[PAD_BUTTON_NO_9].left + 1, m_rtButtonSkinDest[PAD_BUTTON_NO_9].top + 1,	m_rtButtonDest[PAD_BUTTON_NO_9].left + 12, m_rtButtonDest[PAD_BUTTON_NO_9].top + 12);
	SetRect(&m_rtButtonDest[PAD_BUTTON_NO_10],			m_rtButtonSkinDest[PAD_BUTTON_NO_10].left + 1, m_rtButtonSkinDest[PAD_BUTTON_NO_10].top + 1,m_rtButtonDest[PAD_BUTTON_NO_10].left + 12, m_rtButtonDest[PAD_BUTTON_NO_10].top + 12);
	SetRect(&m_rtButtonDest[PAD_BUTTON_NO_11],			m_rtButtonSkinDest[PAD_BUTTON_NO_11].left + 1, m_rtButtonSkinDest[PAD_BUTTON_NO_11].top + 1,m_rtButtonDest[PAD_BUTTON_NO_11].left + 12, m_rtButtonDest[PAD_BUTTON_NO_11].top + 12);

	PadCheckProcess(fTime);

	if(m_iSetMode == JOYSTICK_SET_MODE_BASE) {
		SetRect(&m_rtFuncSkinDest[BUTTON_FUNC_0],			m_iAX + 352, m_iAY + 86, m_iAX + 352 + 14, m_iAY + 86 + 14);
		SetRect(&m_rtFuncSkinDest[BUTTON_FUNC_1],			m_iAX + 352, m_iAY + 111, m_iAX + 352 + 14, m_iAY + 111 + 14);

		SetRect(&m_rtFuncDest[BUTTON_FUNC_0],				m_rtFuncSkinDest[BUTTON_FUNC_0].left + 1, m_rtFuncSkinDest[BUTTON_FUNC_0].top + 1, m_rtFuncDest[BUTTON_FUNC_0].left + 12, m_rtFuncDest[BUTTON_FUNC_0].top + 12);
		SetRect(&m_rtFuncDest[BUTTON_FUNC_1],				m_rtFuncSkinDest[BUTTON_FUNC_1].left + 1, m_rtFuncSkinDest[BUTTON_FUNC_1].top + 1, m_rtFuncDest[BUTTON_FUNC_1].left + 12, m_rtFuncDest[BUTTON_FUNC_1].top + 12);

		SetRect(&m_rtMouseSkinDest[BUTTON_MOUSE_L],			m_iAX + 352, m_iAY + 167, m_iAX + 352 + 14, m_iAY + 167 + 14);
		SetRect(&m_rtMouseSkinDest[BUTTON_MOUSE_R],			m_iAX + 352, m_iAY + 192, m_iAX + 352 + 14, m_iAY + 192 + 14);

		SetRect(&m_rtMouseDest[BUTTON_MOUSE_L],				m_rtMouseSkinDest[BUTTON_MOUSE_L].left + 1, m_rtMouseSkinDest[BUTTON_MOUSE_L].top + 1, m_rtMouseDest[BUTTON_MOUSE_L].left + 12, m_rtMouseDest[BUTTON_MOUSE_L].top + 12);
		SetRect(&m_rtMouseDest[BUTTON_MOUSE_R],				m_rtMouseSkinDest[BUTTON_MOUSE_R].left + 1, m_rtMouseSkinDest[BUTTON_MOUSE_R].top + 1, m_rtMouseDest[BUTTON_MOUSE_R].left + 12, m_rtMouseDest[BUTTON_MOUSE_R].top + 12);
	}
	else if(m_iSetMode == JOYSTICK_SET_MODE_BUTTON) {
		SetRect(&m_rtButtonSetSkinDest[PAD_BUTTON_NO_0],		m_iAX + 47, m_iAY + 166, m_iAX + 47 + 14, m_iAY + 166 + 14);
		SetRect(&m_rtButtonSetSkinDest[PAD_BUTTON_NO_1],		m_iAX + 103, m_iAY + 166, m_iAX + 103 + 14, m_iAY + 166 + 14);
		SetRect(&m_rtButtonSetSkinDest[PAD_BUTTON_NO_2],		m_iAX + 159, m_iAY + 166, m_iAX + 159 + 14, m_iAY + 166 + 14);
		SetRect(&m_rtButtonSetSkinDest[PAD_BUTTON_NO_3],		m_iAX + 215, m_iAY + 166, m_iAX + 215 + 14, m_iAY + 166 + 14);
		SetRect(&m_rtButtonSetSkinDest[PAD_BUTTON_NO_4],		m_iAX + 271, m_iAY + 166, m_iAX + 271 + 14, m_iAY + 166 + 14);
		SetRect(&m_rtButtonSetSkinDest[PAD_BUTTON_NO_5],		m_iAX + 327, m_iAY + 166, m_iAX + 327 + 14, m_iAY + 166 + 14);
		SetRect(&m_rtButtonSetSkinDest[PAD_BUTTON_NO_6],		m_iAX + 47, m_iAY + 204, m_iAX + 47 + 14, m_iAY + 204 + 14);
		SetRect(&m_rtButtonSetSkinDest[PAD_BUTTON_NO_7],		m_iAX + 103, m_iAY + 204, m_iAX + 103 + 14, m_iAY + 204 + 14);
		SetRect(&m_rtButtonSetSkinDest[PAD_BUTTON_NO_8],		m_iAX + 159, m_iAY + 204, m_iAX + 159 + 14, m_iAY + 204 + 14);
		SetRect(&m_rtButtonSetSkinDest[PAD_BUTTON_NO_9],		m_iAX + 215, m_iAY + 204, m_iAX + 215 + 14, m_iAY + 204 + 14);
		SetRect(&m_rtButtonSetSkinDest[PAD_BUTTON_NO_10],		m_iAX + 271, m_iAY + 204, m_iAX + 271 + 14, m_iAY + 204 + 14);
		SetRect(&m_rtButtonSetSkinDest[PAD_BUTTON_NO_11],		m_iAX + 327, m_iAY + 204, m_iAX + 327 + 14, m_iAY + 204 + 14);		

		SetRect(&m_rtButtonSetDest[PAD_BUTTON_NO_0],			m_rtButtonSetSkinDest[PAD_BUTTON_NO_0].left + 1, m_rtButtonSetSkinDest[PAD_BUTTON_NO_0].top + 1,	m_rtButtonSetDest[PAD_BUTTON_NO_0].left + 12, m_rtButtonSetDest[PAD_BUTTON_NO_0].top + 12);
		SetRect(&m_rtButtonSetDest[PAD_BUTTON_NO_1],			m_rtButtonSetSkinDest[PAD_BUTTON_NO_1].left + 1, m_rtButtonSetSkinDest[PAD_BUTTON_NO_1].top + 1,	m_rtButtonSetDest[PAD_BUTTON_NO_1].left + 12, m_rtButtonSetDest[PAD_BUTTON_NO_1].top + 12);
		SetRect(&m_rtButtonSetDest[PAD_BUTTON_NO_2],			m_rtButtonSetSkinDest[PAD_BUTTON_NO_2].left + 1, m_rtButtonSetSkinDest[PAD_BUTTON_NO_2].top + 1,	m_rtButtonSetDest[PAD_BUTTON_NO_2].left + 12, m_rtButtonSetDest[PAD_BUTTON_NO_2].top + 12);
		SetRect(&m_rtButtonSetDest[PAD_BUTTON_NO_3],			m_rtButtonSetSkinDest[PAD_BUTTON_NO_3].left + 1, m_rtButtonSetSkinDest[PAD_BUTTON_NO_3].top + 1,	m_rtButtonSetDest[PAD_BUTTON_NO_3].left + 12, m_rtButtonSetDest[PAD_BUTTON_NO_3].top + 12);
		SetRect(&m_rtButtonSetDest[PAD_BUTTON_NO_4],			m_rtButtonSetSkinDest[PAD_BUTTON_NO_4].left + 1, m_rtButtonSetSkinDest[PAD_BUTTON_NO_4].top + 1,	m_rtButtonSetDest[PAD_BUTTON_NO_4].left + 12, m_rtButtonSetDest[PAD_BUTTON_NO_4].top + 12);
		SetRect(&m_rtButtonSetDest[PAD_BUTTON_NO_5],			m_rtButtonSetSkinDest[PAD_BUTTON_NO_5].left + 1, m_rtButtonSetSkinDest[PAD_BUTTON_NO_5].top + 1,	m_rtButtonSetDest[PAD_BUTTON_NO_5].left + 12, m_rtButtonSetDest[PAD_BUTTON_NO_5].top + 12);
		SetRect(&m_rtButtonSetDest[PAD_BUTTON_NO_6],			m_rtButtonSetSkinDest[PAD_BUTTON_NO_6].left + 1, m_rtButtonSetSkinDest[PAD_BUTTON_NO_6].top + 1,	m_rtButtonSetDest[PAD_BUTTON_NO_6].left + 12, m_rtButtonSetDest[PAD_BUTTON_NO_6].top + 12);
		SetRect(&m_rtButtonSetDest[PAD_BUTTON_NO_7],			m_rtButtonSetSkinDest[PAD_BUTTON_NO_7].left + 1, m_rtButtonSetSkinDest[PAD_BUTTON_NO_7].top + 1,	m_rtButtonSetDest[PAD_BUTTON_NO_7].left + 12, m_rtButtonSetDest[PAD_BUTTON_NO_7].top + 12);
		SetRect(&m_rtButtonSetDest[PAD_BUTTON_NO_8],			m_rtButtonSetSkinDest[PAD_BUTTON_NO_8].left + 1, m_rtButtonSetSkinDest[PAD_BUTTON_NO_8].top + 1,	m_rtButtonSetDest[PAD_BUTTON_NO_8].left + 12, m_rtButtonSetDest[PAD_BUTTON_NO_8].top + 12);
		SetRect(&m_rtButtonSetDest[PAD_BUTTON_NO_9],			m_rtButtonSetSkinDest[PAD_BUTTON_NO_9].left + 1, m_rtButtonSetSkinDest[PAD_BUTTON_NO_9].top + 1,	m_rtButtonSetDest[PAD_BUTTON_NO_9].left + 12, m_rtButtonSetDest[PAD_BUTTON_NO_9].top + 12);
		SetRect(&m_rtButtonSetDest[PAD_BUTTON_NO_10],			m_rtButtonSetSkinDest[PAD_BUTTON_NO_10].left + 1, m_rtButtonSetSkinDest[PAD_BUTTON_NO_10].top + 1,	m_rtButtonSetDest[PAD_BUTTON_NO_10].left + 12, m_rtButtonSetDest[PAD_BUTTON_NO_10].top + 12);
		SetRect(&m_rtButtonSetDest[PAD_BUTTON_NO_11],			m_rtButtonSetSkinDest[PAD_BUTTON_NO_11].left + 1, m_rtButtonSetSkinDest[PAD_BUTTON_NO_11].top + 1,	m_rtButtonSetDest[PAD_BUTTON_NO_11].left + 12, m_rtButtonSetDest[PAD_BUTTON_NO_11].top + 12);

		KeyCheckProcess(fTime);
	}
	
	SPWindow::Process(fTime);
}


void SPWindowJoyStick::PadCheckProcess(float fTime)
{
	if(m_bShow == false)
		return;

	if(!g_pInput->IsEquipJoystick()) 
		return;

	SPInputStructJoyStick* pStick = g_pInputManager->GetInputStructJoyStick();
	if(pStick == NULL)	return;

	for(int i = 0; i < PAD_BUTTON_NO_MAX; i++) {
		if(pStick->ibRgbButtons[i].iValue) {
			m_bButtonPush[i] = true;
			if(m_pManager->GetSelectKey() != JOYSTICK_SELECT_NULL) {
				SetSpecialKey((PAD_BUTTON_NO)i);
			}
		}
		else {
			m_bButtonPush[i] = false;
		}
	}
}


bool SPWindowJoyStick::SetSpecialKey(PAD_BUTTON_NO iButton)
{
	if(m_iSetMode != JOYSTICK_SET_MODE_BASE)
		return false;

	switch(m_pManager->GetSelectKey()) {
		case JOYSTICK_SELECT_FUNC1:
			//SetNormalKeyInfo(4, PAD_BUTTON_NO_4, PAD_BUTTON_USE_FUNC, BUTTON_KEY_MAX);
			m_pCopyInfo->m_iFunc[BUTTON_FUNC_0] = (PAD_BUTTON_NO)iButton;
			m_pCopyInfo->SetNormalInfo((PAD_BUTTON_NO)iButton, PAD_BUTTON_USE_FUNC, BUTTON_KEY_MAX);
			m_pFunc[BUTTON_FUNC_0]->SPSendMessage(SPIM_BTN_SELECT,			false);
			m_pManager->SetSelectKey(JOYSTICK_SELECT_NULL);
			break;
		case JOYSTICK_SELECT_FUNC2:
			m_pCopyInfo->m_iFunc[BUTTON_FUNC_1] = (PAD_BUTTON_NO)iButton;
			m_pCopyInfo->SetNormalInfo((PAD_BUTTON_NO)iButton, PAD_BUTTON_USE_FUNC, BUTTON_KEY_MAX);
			m_pFunc[BUTTON_FUNC_1]->SPSendMessage(SPIM_BTN_SELECT,			false);
			m_pManager->SetSelectKey(JOYSTICK_SELECT_NULL);
			break;
		case JOYSTICK_SELECT_MOUSEL:
			m_pCopyInfo->m_iMouse[BUTTON_MOUSE_L] = (PAD_BUTTON_NO)iButton;
			m_pCopyInfo->SetNormalInfo((PAD_BUTTON_NO)iButton, PAD_BUTTON_USE_KEY, BUTTON_KEY_MOUSE_L);
			m_pMouse[BUTTON_MOUSE_L]->SPSendMessage(SPIM_BTN_SELECT,			false);
			m_pManager->SetSelectKey(JOYSTICK_SELECT_NULL);
			break;
		case JOYSTICK_SELECT_MOUSER:
			m_pCopyInfo->m_iMouse[BUTTON_MOUSE_R] = (PAD_BUTTON_NO)iButton;
			m_pCopyInfo->SetNormalInfo((PAD_BUTTON_NO)iButton, PAD_BUTTON_USE_KEY, BUTTON_KEY_MOUSE_R);					
			m_pMouse[BUTTON_MOUSE_R]->SPSendMessage(SPIM_BTN_SELECT,			false);
			m_pManager->SetSelectKey(JOYSTICK_SELECT_NULL);
			break;				
		default:
			break;
	}

	return true;
}


void SPWindowJoyStick::KeyCheckProcess(float fTime)
{
	if(m_bShow == false)
		return;

	if(m_pManager->GetSelectKey() == JOYSTICK_SELECT_NULL)
		return;

	if(m_iButtonSetNo == PAD_BUTTON_NO_MAX)
		return;	
	
	SPInputStruct* pInputStruct = g_pInputManager->GetInputStruct();

	BUTTON_KEY iKey = BUTTON_KEY_MAX;

	if(pInputStruct->ikLCtrl.InputState) {		//Ctrl
		iKey = BUTTON_KEY_QUICK_0;
	}
	if(pInputStruct->ikA.InputState) {			//A
		iKey = BUTTON_KEY_QUICK_1;
	}
	if(pInputStruct->ikS.InputState) {			//S
		iKey = BUTTON_KEY_QUICK_2;
	}
	if(pInputStruct->ikD.InputState) {			//D
		iKey = BUTTON_KEY_QUICK_3;
	}
	if(pInputStruct->ikF.InputState) {			//F
		iKey = BUTTON_KEY_QUICK_4;
	}
	if(pInputStruct->ikLAlt.InputState) {		//Alt
		iKey = BUTTON_KEY_QUICK_5;
	}	
	if(pInputStruct->ikLShift.InputState) {		//Shift
		iKey = BUTTON_KEY_QUICK_6;
	}
	if(pInputStruct->ikZ.InputState) {			//Z
		iKey = BUTTON_KEY_QUICK_7;
	}
	if(pInputStruct->ikX.InputState) {			//X
		iKey = BUTTON_KEY_QUICK_8;
	}
	if(pInputStruct->ikC.InputState) {			//C
		iKey = BUTTON_KEY_QUICK_9;
	}
	if(pInputStruct->ikV.InputState) {			//V
		iKey = BUTTON_KEY_QUICK_10;
	}
	if(pInputStruct->ikSpaceBar.InputState) {	//Space
		iKey = BUTTON_KEY_QUICK_11;
	}
	if(pInputStruct->ik1.InputState) {			//1
		iKey = BUTTON_KEY_QUICK_S1;
	}
	if(pInputStruct->ik2.InputState) {			//2
		iKey = BUTTON_KEY_QUICK_S2;
	}
	if(pInputStruct->ik3.InputState) {			//3
		iKey = BUTTON_KEY_QUICK_S3;
	}
	if(pInputStruct->ik4.InputState) {			//4
		iKey = BUTTON_KEY_QUICK_S4;
	}
	if(pInputStruct->ik5.InputState) {			//5
		iKey = BUTTON_KEY_QUICK_S5;
	}
	if(pInputStruct->ik6.InputState) {			//6
		iKey = BUTTON_KEY_QUICK_S6;
	}
	if(pInputStruct->ik7.InputState) {			//7
		iKey = BUTTON_KEY_QUICK_S7;
	}
	if(pInputStruct->ik8.InputState) {			//8
		iKey = BUTTON_KEY_QUICK_S8;
	}
	if(pInputStruct->ik9.InputState) {			//9
		iKey = BUTTON_KEY_QUICK_S9;
	}
	if(pInputStruct->ik0.InputState) {			//10
		iKey = BUTTON_KEY_QUICK_S10;
	}
	if(pInputStruct->ikMinus.InputState) {		//11
		iKey = BUTTON_KEY_QUICK_S11;
	}
	
	if(pInputStruct->ikPrevTrack.InputState) {		//12
		if(SPLocalizeManager::GetInstance()->GetCountryType() == CT_JAPAN)
			iKey = BUTTON_KEY_QUICK_S12;	
	}
	
	if(pInputStruct->ikEquals.InputState) {		//12
		if(SPLocalizeManager::GetInstance()->GetCountryType() != CT_JAPAN)
			iKey = BUTTON_KEY_QUICK_S12;
	}

	if(pInputStruct->ikF1.InputState) {			//F1
		iKey = BUTTON_KEY_QUICK_F1;
	}
	if(pInputStruct->ikF2.InputState) {			//F2
		iKey = BUTTON_KEY_QUICK_F2;
	}	    
	if(pInputStruct->ikI.InputState) {			//I Inven
		iKey = BUTTON_KEY_INTERFACE_I;
	}
	if(pInputStruct->ikE.InputState) {			//E Equip
		iKey = BUTTON_KEY_INTERFACE_E;
	}
	if(pInputStruct->ikT.InputState) {			//T State
		iKey = BUTTON_KEY_INTERFACE_T;
	}
	if(pInputStruct->ikK.InputState) {			//K Skill
		iKey = BUTTON_KEY_INTERFACE_K;
	}
	if(pInputStruct->ikB.InputState) {			//B Friend
		iKey = BUTTON_KEY_INTERFACE_B;
	}
	if(pInputStruct->ikQ.InputState) {			//Q Quest
		iKey = BUTTON_KEY_INTERFACE_Q;
	}
	if(pInputStruct->ikO.InputState) {			//O Option
		iKey = BUTTON_KEY_INTERFACE_O;
	}
	if(pInputStruct->ikP.InputState) {			//P Team
		iKey = BUTTON_KEY_INTERFACE_P;
	}
	if(pInputStruct->ikH.InputState) {			//H Help
		iKey = BUTTON_KEY_INTERFACE_H;
	}
	if(pInputStruct->ikW.InputState) {			//W World Map
		iKey = BUTTON_KEY_INTERFACE_W;
	}
	if(pInputStruct->ikM.InputState) {			//M	Map Stage
		iKey = BUTTON_KEY_INTERFACE_M;
	}
	if(pInputStruct->ikN.InputState) {			//N Enchant
		iKey = BUTTON_KEY_INTERFACE_N;
	}
	if(pInputStruct->ikU.InputState) {			//U Upgrad
		iKey = BUTTON_KEY_INTERFACE_U;
	}
	if(pInputStruct->ikY.InputState) {			//Y Way Point
		iKey = BUTTON_KEY_INTERFACE_Y;
	}
	if(pInputStruct->ikL.InputState) {			//L Pet
		iKey = BUTTON_KEY_INTERFACE_L;
	}
	if(pInputStruct->ikG.InputState) {			//G Guild
		iKey = BUTTON_KEY_INTERFACE_G;
	}
	if(pInputStruct->ikTab.InputState) {		//Tab
		iKey = BUTTON_KEY_INTERFACE_TAB;
	}
	if(pInputStruct->ikReturn.InputState) {		//Enter
		iKey = BUTTON_KEY_INTERFACE_ENTER;
	}
	if(pInputStruct->ikEscape.InputState) {		//Esc
		iKey = BUTTON_KEY_INTERFACE_ESC;
	}
	
	if(iKey == BUTTON_KEY_MAX)
		return;	
	
	m_pCopyInfo->SetClearKey(iKey);
	switch(m_iButtonSetType) {
		case PAD_BUTTONTYPE_NORMAL:
			m_pCopyInfo->SetNormalInfo(m_iButtonSetNo, PAD_BUTTON_USE_KEY, iKey);
			break;
		case PAD_BUTTONTYPE_FUNC1:
			m_pCopyInfo->SetFunc1KeyInfo(m_iButtonSetNo, iKey);
			m_pCopyInfo->SetFuncKey(PAD_BUTTONTYPE_FUNC1);
			break;
		case PAD_BUTTONTYPE_FUNC2:
			m_pCopyInfo->SetFunc2KeyInfo(m_iButtonSetNo, iKey);
			m_pCopyInfo->SetFuncKey(PAD_BUTTONTYPE_FUNC2);
			break;
	}

	m_pButtonKey[m_iButtonSetNo]->SPSendMessage(SPIM_BTN_SELECT,			false);
	m_pManager->SetSelectKey(JOYSTICK_SELECT_NULL);
	m_iButtonSetNo = PAD_BUTTON_NO_MAX;	
	UpdateKeyList();
}


void SPWindowJoyStick::Render(float fTime)
{
	if(!m_bShow)		return;

	if(m_pTextureBase) {
		g_pVideo->Flush();
		for(int i = 0; i < WINDOW_BASE_MAX; i++) {
			m_pTextureBase->RenderTexture(&m_rtBaseDest[i], &m_rtBaseSrc[i]);
		}
	}

	SPWindow::Render(fTime);

	if(m_pTextureButton) {
		for(int i = 0; i < PAD_BUTTON_NO_MAX; i++) {
			g_pVideo->Flush();

			if(m_bButtonPush[i])
				m_pTextureButton->RenderTexture(&m_rtButtonSkinDest[i], &m_rtButtonSkinSrcPush);
			else 
				m_pTextureButton->RenderTexture(&m_rtButtonSkinDest[i], &m_rtButtonSkinSrcNormal);			
		
			m_pTextureButton->RenderTexture(&m_rtButtonDest[i], &m_rtButtonSrc[i]);
		}
	}

	if(m_iSetMode == JOYSTICK_SET_MODE_BASE) {
		for(int i = 0; i < BUTTON_FUNC_MAX; i++) {
			m_pTextureButton->RenderTexture(&m_rtFuncSkinDest[i], &m_rtButtonSkinSrcNormal);
			m_pTextureButton->RenderTexture(&m_rtFuncDest[i],	&m_rtButtonSrc[m_pCopyInfo->m_iFunc[i]]);
		}

		for(int i = 0; i < BUTTON_MOUSE_MAX; i++) {
			m_pTextureButton->RenderTexture(&m_rtMouseSkinDest[i], &m_rtButtonSkinSrcNormal);
			m_pTextureButton->RenderTexture(&m_rtMouseDest[i],	&m_rtButtonSrc[m_pCopyInfo->m_iMouse[i]]);
		}
	}
	else if(m_iSetMode == JOYSTICK_SET_MODE_BUTTON) {
		for(int i = 0; i < PAD_BUTTON_NO_MAX; i++) {
			if(m_bButtonPush[i])
				m_pTextureButton->RenderTexture(&m_rtButtonSetSkinDest[i], &m_rtButtonSkinSrcPush);
			else 
				m_pTextureButton->RenderTexture(&m_rtButtonSetSkinDest[i], &m_rtButtonSkinSrcNormal);			

			m_pTextureButton->RenderTexture(&m_rtButtonSetDest[i], &m_rtButtonSrc[i]);
		}

#ifdef _DEBUG
		std::vector<SPWindow*>::iterator iter = m_vpKeyButton.begin();
		RECT rtRect;
		for(; iter != m_vpKeyButton.end(); ++iter) {
			(*iter)->GetWindowRect(rtRect);
			g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			g_pVideo->GetSysLine()->DrawRect(rtRect);
			g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
#endif
	}
}




SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowJoyStick )
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP	,	WIID_JOYSTICK_BASE_NEXT				,	OnNext				)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP	,	WIID_JOYSTICK_BASE_INIT				,	OnReset				)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP	,	WIID_JOYSTICK_BASE_OK				,	OnOk				)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP	,	WIID_JOYSTICK_BASE_CANCEL			,	OnCancel			)

SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP	,	WIID_JOYSTICK_BUTTON_PREV			,	OnPrev				)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP	,	WIID_JOYSTICK_BUTTON_INIT			,	OnReset				)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP	,	WIID_JOYSTICK_BUTTON_OK				,	OnOk				)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP	,	WIID_JOYSTICK_BUTTON_CANCEL			,	OnCancel			)

SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP	,	WIID_JOYSTICK_BASE_PAD_MODE_CHACK	,	OnPadMode			)
SPIMESSAGE_CONTROL_RANGE(SPIM_LBUTTON_UP, WIID_JOYSTICK_BASE_FUNCKEY,	WIID_JOYSTICK_BASE_FUNCKEY + BUTTON_FUNC_MAX,	OnSelectFunc)
SPIMESSAGE_CONTROL_RANGE(SPIM_LBUTTON_UP, WIID_JOYSTICK_BASE_MOUSEKEY,	WIID_JOYSTICK_BASE_MOUSEKEY + BUTTON_MOUSE_MAX,	OnSelectMouse)
SPIMESSAGE_CONTROL_RANGE(SPIM_LBUTTON_UP, WIID_JOYSTICK_BUTTON_NORMAL,	WIID_JOYSTICK_BUTTON_NORMAL + PAD_BUTTONTYPE_MAX,	OnSelectKeyType)
SPIMESSAGE_CONTROL_RANGE(SPIM_LBUTTON_UP, WIID_JOYSTICK_BUTTON_KEY,		WIID_JOYSTICK_BUTTON_KEY + PAD_BUTTON_NO_MAX,	OnSelectKey)
SPIMESSAGE_OBJ_MAP_END	( SPWindow	)



int SPWindowJoyStick::OnNext(WPARAM wParam, LPARAM lparam)
{
	m_iSetMode = JOYSTICK_SET_MODE_BUTTON;
	UpdateSubControl();
	return 1;
}


int SPWindowJoyStick::OnReset(WPARAM wParam, LPARAM lparam)
{	
	m_pManager->SetDefault(m_pCopyInfo);
	
	//SPPadSettingInfo* pSrcInfo = m_pManager->GetPadSettingInfo();
	//if(pSrcInfo && m_pCopyInfo) {
	//	m_pCopyInfo->CopySettingInfo(pSrcInfo);
	//}
	
	UpdateSubControl();
	
	for(int i = 0; i < PAD_BUTTON_NO_MAX; i++) {
		m_pButtonKey[i]->SPSendMessage(SPIM_BTN_SELECT,			false);
	}

	m_pManager->UpdateButton();
	m_pManager->Save();

	return 1;
}


int SPWindowJoyStick::OnOk(WPARAM wParam, LPARAM lparam)
{
	SPPadSettingInfo* pSrcInfo = m_pManager->GetPadSettingInfo();
	if(pSrcInfo && m_pCopyInfo) {
		pSrcInfo->CopySettingInfo(m_pCopyInfo);
	}

	//UpdateSubControl();
	m_pManager->UpdateButton();
	m_pManager->Save();
	Hide();

	return 1;
}


int SPWindowJoyStick::OnCancel(WPARAM wParam, LPARAM lparam)
{
	SPPadSettingInfo* pSrcInfo = m_pManager->GetPadSettingInfo();
	if(pSrcInfo && m_pCopyInfo) {
		m_pCopyInfo->CopySettingInfo(pSrcInfo);
	}	
	UpdateSubControl();

	Hide();
	return 1;
}


int SPWindowJoyStick::OnPrev(WPARAM wParam, LPARAM lparam)
{
	m_iSetMode = JOYSTICK_SET_MODE_BASE;
	UpdateSubControl();
	return 1;
}


int SPWindowJoyStick::OnPadMode(WPARAM wParam, LPARAM lparam)
{
	if(m_pCopyInfo->m_iJoyStickMode == JOYSTICK_MODE_ANALOG){
		m_pCopyInfo->m_iJoyStickMode = JOYSTICK_MODE_DIGITAL;
		m_pModeCheck->SPSendMessage(SPIM_BTN_SELECT,			false);
		for(int i = 0; i < BUTTON_MOUSE_MAX; i++) {
			m_pMouse[i]->SetEnable(false);
			m_pCopyInfo->m_iMouse[i] = PAD_BUTTON_NO_MAX;
		}
	}
	else if(m_pCopyInfo->m_iJoyStickMode == JOYSTICK_MODE_DIGITAL) {
		m_pCopyInfo->m_iJoyStickMode = JOYSTICK_MODE_ANALOG;
		m_pModeCheck->SPSendMessage(SPIM_BTN_SELECT,			true);
	}

	UpdateSubControl();
	
	return 1;
}


int SPWindowJoyStick::OnSelectFunc(unsigned int iID, WPARAM wParam, LPARAM lparam)
{
	int iSelect = iID - WIID_JOYSTICK_BASE_FUNCKEY;	
	if(m_iSetMode != JOYSTICK_SET_MODE_BASE) 
		return 1;	
	
	for(int i = 0; i < BUTTON_FUNC_MAX; i++) {
		if(i == iSelect) {
			m_pFunc[i]->SPSendMessage(SPIM_BTN_SELECT,			true);
			m_pCopyInfo->m_iFunc[i] = PAD_BUTTON_NO_MAX;
			
			if(i == BUTTON_FUNC_0) {
				m_pCopyInfo->SetFuncKey(PAD_BUTTONTYPE_FUNC1);
				m_pManager->SetSelectKey(JOYSTICK_SELECT_FUNC1);
			}
			else if(i == BUTTON_FUNC_1){
				m_pCopyInfo->SetFuncKey(PAD_BUTTONTYPE_FUNC2);
				m_pManager->SetSelectKey(JOYSTICK_SELECT_FUNC2);
			}
		}
		else {
			m_pFunc[i]->SPSendMessage(SPIM_BTN_SELECT,			false);
		}
	}

	for(int i = 0; i < BUTTON_MOUSE_MAX; i++) {
		m_pMouse[i]->SPSendMessage(SPIM_BTN_SELECT,			false);
	}
	
	//m_pCopyInfo->ClearAllButtonInfo();
	
	return 1;
}


int SPWindowJoyStick::OnSelectMouse(unsigned int iID, WPARAM wParam, LPARAM lparam)
{
	int iSelect = iID - WIID_JOYSTICK_BASE_MOUSEKEY;
	if(m_iSetMode != JOYSTICK_SET_MODE_BASE) 
		return 1;

	if(m_pCopyInfo->m_iJoyStickMode != JOYSTICK_MODE_ANALOG)
		return 1;
	
	for(int i = 0; i < BUTTON_FUNC_MAX; i++) {
		m_pFunc[i]->SPSendMessage(SPIM_BTN_SELECT,			false);
	}

	for(int i = 0; i < BUTTON_MOUSE_MAX; i++) {
		if(i == iSelect) {
			m_pMouse[i]->SPSendMessage(SPIM_BTN_SELECT,			true);
			m_pCopyInfo->m_iMouse[i] = PAD_BUTTON_NO_MAX;
			if(i == BUTTON_MOUSE_L) {
				m_pManager->SetSelectKey(JOYSTICK_SELECT_MOUSEL);
			}
			else if(i == BUTTON_MOUSE_R){
				m_pManager->SetSelectKey(JOYSTICK_SELECT_MOUSER);
			}
		}
		else 
			m_pMouse[i]->SPSendMessage(SPIM_BTN_SELECT,			false);
	}

	return 1;
}


int SPWindowJoyStick::OnSelectKeyType(unsigned int iID, WPARAM wParam, LPARAM lparam)
{
	int iSelect = iID - WIID_JOYSTICK_BUTTON_NORMAL;
	
	if(m_iSetMode != JOYSTICK_SET_MODE_BUTTON) 
		return 1;	

	for(int i = 0; i < PAD_BUTTONTYPE_MAX; i++) {
		if(i == iSelect) {
			m_iButtonSetType = (PAD_BUTTONTYPE)iSelect;
			m_pButtonMode[i]->SPSendMessage(SPIM_BTN_SELECT,			true);
		}
		else {
			m_pButtonMode[i]->SPSendMessage(SPIM_BTN_SELECT,			false);
		}		
	}

	for(int i = 0; i < PAD_BUTTON_NO_MAX; i++) {
		m_pButtonKey[i]->SPSendMessage(SPIM_BTN_SELECT,			false);
	}
	m_pManager->SetSelectKey(JOYSTICK_SELECT_NULL);

	UpdateKeyList();

	return 1;
}


int SPWindowJoyStick::OnSelectKey(unsigned int iID, WPARAM wParam, LPARAM lparam)
{
	int iSelect = iID - WIID_JOYSTICK_BUTTON_KEY;

	if(m_iSetMode != JOYSTICK_SET_MODE_BUTTON) 
		return 1;	

	for(int i = 0; i < PAD_BUTTON_NO_MAX; i++) {
		if(i == iSelect) {
			m_iButtonSetNo = (PAD_BUTTON_NO)iSelect;
			switch(m_iButtonSetType) {
				case PAD_BUTTONTYPE_NORMAL:
					m_pManager->SetSelectKey((JOYSTICK_SELECT)(i + JOYSTICK_SELECT_KEY_0));
					break;
				case PAD_BUTTONTYPE_FUNC1:
					m_pManager->SetSelectKey((JOYSTICK_SELECT)(i + JOYSTICK_SELECT_FUNC1_0));
					break;
				case PAD_BUTTONTYPE_FUNC2:
					m_pManager->SetSelectKey((JOYSTICK_SELECT)(i + JOYSTICK_SELECT_FUNC2_0));
					break;
			}
			
			m_pButtonKey[i]->SPSendMessage(SPIM_BTN_SELECT,			true);
			//m_pButtonKey[i]->SetWindowText(NULL);
			//m_pCopyInfo->GetButtonInfo(m_iButtonSetType, m_iButtonSetNo)->Clear();
		}
		else {
			m_pButtonKey[i]->SPSendMessage(SPIM_BTN_SELECT,			false);
		}
	}

	UpdateKeyList();	

	return 1;
}
