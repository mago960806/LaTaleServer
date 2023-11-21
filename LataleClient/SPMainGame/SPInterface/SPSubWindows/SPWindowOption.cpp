
#include "SPCommon.h"

#include "SPUtil.h"

#include "SPManager.h"
#include "SPMouseCursor.h"
#include "SPIMEPool.h"
#include "SPSubGameManager.h"

#include "SPWindowDEF.h"
#include "SPSubGameManager.h"
#include "SPInterfaceManager.h"


#include "SPOptionDef.h"
#include "SPClientOptionArchive.h"


#include "SPWindow.h"
#include "SPWindowSlider.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"

#include "SPWindowOption.h"
#include "SPLocalizeManager.h"

#include "SPDebug.h"

extern bool g_bWindowed;



SPWindowOption::SPWindowOption(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_OPTION, InstanceID, iX, iY, iCX, iCY, pParent)
, m_pTextureBase(NULL)
, m_pCopyOption(NULL)
{	
	Init();
}

SPWindowOption::~SPWindowOption()
{
	Clean();
}

void SPWindowOption::Init()
{
	m_cusCopyServer = SSO_NULL;
	m_pCopyOption = new SPOptionStruct;
	g_pClientOption->OptionCopy(m_pCopyOption);

	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI102.PNG" , &m_pTextureBase);
	SetRect(&m_rtSrcBase[WINDOW_BASE_TOP_LEFT],		1, 1, 5, 5);
	SetRect(&m_rtSrcBase[WINDOW_BASE_TOP],			13, 1, 15, 5);	
	SetRect(&m_rtSrcBase[WINDOW_BASE_TOP_RIGHT],	257, 1, 261, 5);
	SetRect(&m_rtSrcBase[WINDOW_BASE_CENTER_LEFT],	1, 7, 5, 9);
	SetRect(&m_rtSrcBase[WINDOW_BASE_CENTER],		13, 7, 15, 9);
	SetRect(&m_rtSrcBase[WINDOW_BASE_CENTER_RIGHT],	257, 7, 261, 9);
	SetRect(&m_rtSrcBase[WINDOW_BASE_BOTTOM_LEFT],	1, 11, 5, 15);
	SetRect(&m_rtSrcBase[WINDOW_BASE_BOTTOM],		13, 11, 15, 15);
	SetRect(&m_rtSrcBase[WINDOW_BASE_BOTTOM_RIGHT],	257, 11, 261, 15);	

	SetRect(&m_rtDestBase[WINDOW_BASE_TOP_LEFT],		m_iAX, m_iAY, m_iAX + 4, m_iAY + 4);
	SetRect(&m_rtDestBase[WINDOW_BASE_TOP],				m_iAX + 4, m_iAY, m_iAX + 4 + 282, m_iAY + 4);	
	SetRect(&m_rtDestBase[WINDOW_BASE_TOP_RIGHT],		m_iAX + 286, m_iAY, m_iAX + 286 + 4, m_iAY + 4);
	SetRect(&m_rtDestBase[WINDOW_BASE_CENTER_LEFT],		m_iAX, m_iAY + 4, m_iAX + 4, m_iAY + 4 + 315);
	SetRect(&m_rtDestBase[WINDOW_BASE_CENTER],			m_iAX + 4, m_iAY + 4, m_iAX + 4 + 282, m_iAY + 4 + 315);
	SetRect(&m_rtDestBase[WINDOW_BASE_CENTER_RIGHT],	m_iAX + 286, m_iAY + 4, m_iAX + 286 + 4, m_iAY + 4 + 315);
	SetRect(&m_rtDestBase[WINDOW_BASE_BOTTOM_LEFT],		m_iAX, m_iAY + 319, m_iAX + 4, m_iAY + 319 + 4);
	SetRect(&m_rtDestBase[WINDOW_BASE_BOTTOM],			m_iAX + 4, m_iAY + 319, m_iAX + 4 + 282, m_iAY + 319 + 4);
	SetRect(&m_rtDestBase[WINDOW_BASE_BOTTOM_RIGHT],	m_iAX + 286, m_iAY + 319, m_iAX + 286 + 4, m_iAY + 319 + 4);

	m_iOptionType = OPTION_TYPE_SYSTEM;

	for(int i = 0; i < OPTION_SYSTEM_MAX; i++) {
		m_pSystemCheck[i] = NULL;
	}

	for(int i = 0; i < OPTION_COMMUNITY_MAX; i++) {
		m_pCommunityCheck[i] = NULL;
	}

	for(int i = 0; i < OPTION_BENEFIT_MAX; i++) {
		m_pBenefitCheck[i] = NULL;
	}
	
	InitSubControl();
}


void SPWindowOption::InitSubControl()
{
	char* OPTION_IMAGE_FILE0 = "DATA/INTERFACE/CONCEPT/UI100.PNG";
	char* OPTION_IMAGE_FILE1 = "DATA/INTERFACE/CONCEPT/UI101.PNG";
	char* OPTION_IMAGE_FILE2 = "DATA/INTERFACE/CONCEPT/UI102.PNG";
	char* OPTION_IMAGE_FILE4 = "DATA/INTERFACE/CONCEPT/UI104.PNG";
	char* OPTION_IMAGE_FILE5 = "DATA/INTERFACE/CONCEPT/UI105.PNG";
	char* OPTION_IMAGE_FILE6 = "DATA/INTERFACE/CONCEPT/UI106.PNG";
	
	//// Control
	SPWindowButton* pButton = NULL;
	SPWindowStatic* pStatic = NULL;
	SPWindowStatic* pParent = NULL;
	SPWindowSlider* pSlider = NULL;

	//WIID_OPTION_TITLE_IMAGE
	pStatic = new SPWindowStatic( WIID_OPTION_TITLE_IMAGE,	11,	3,	22,	21, this);	
	pStatic->SetImage(OPTION_IMAGE_FILE2, 158, 345);
	pStatic->Show();
	pStatic = NULL;

	//WIID_OPTION_TITLE
	pStatic = new SPWindowStatic( WIID_OPTION_TITLE,	42,	7,	67,	15, this);		
	pStatic->SetImage(OPTION_IMAGE_FILE2, 181, 351);
	pStatic->Show();
	pStatic = NULL;

	//WIID_OPTION_EXIT
	pButton = new SPWindowButton( WIID_OPTION_EXIT , 271, 9, 12, 12, this);
	pButton->SetImage		(OPTION_IMAGE_FILE2, 247,  217);		
	pButton->SetImageHit	(OPTION_IMAGE_FILE2, 247,  230);
	pButton->SetImagePush	(OPTION_IMAGE_FILE2, 247,  243);
	pButton->SetImageDisable(OPTION_IMAGE_FILE2, 247,  256);
	pButton->Show();


	//WIID_OPTION_SKIN_1
	pParent = new SPWindowStatic( WIID_OPTION_SKIN_1,	2,	26,	286, 19, this);	
	pParent->SetImage(OPTION_IMAGE_FILE2, 349, 312);
	pParent->SetSrcSize(6, 6);
	pParent->Show();
	
	//WIID_OPTION_SYSTEM_BUTTON	
	pButton = new SPWindowButton( WIID_OPTION_SYSTEM_BUTTON , 26 -  2, 28 - 26, 80, 15, pParent);
	pButton->SetImage		(OPTION_IMAGE_FILE2, 1,  175);		
	pButton->SetImageHit	(OPTION_IMAGE_FILE2, 1,  191);
	pButton->SetImagePush	(OPTION_IMAGE_FILE2, 82,  175);
	pButton->SetImageDisable(OPTION_IMAGE_FILE2, 82,  191);
	pButton->SetImageCheck	(OPTION_IMAGE_FILE2, 82,  175);
	pButton->Show();
	m_pType[OPTION_TYPE_SYSTEM] = pButton;
	
	//WIID_OPTION_COMMUNITY_BUTTON
	pButton = new SPWindowButton( WIID_OPTION_COMMUNITY_BUTTON , 105 -  2, 28 - 26, 80, 15, pParent);
	pButton->SetImage		(OPTION_IMAGE_FILE1, 184,  180);		
	pButton->SetImageHit	(OPTION_IMAGE_FILE1, 184,  196);
	pButton->SetImagePush	(OPTION_IMAGE_FILE1, 265,  180);
	pButton->SetImageDisable(OPTION_IMAGE_FILE1, 265,  196);
	pButton->SetImageCheck	(OPTION_IMAGE_FILE1, 265,  180);
	pButton->Show();
	m_pType[OPTION_TYPE_COMMUNITY] = pButton;
	
	//WIID_OPTION_BENEFIT_BUTTON	
	pButton = new SPWindowButton( WIID_OPTION_BENEFIT_BUTTON , 184 -  2, 28 - 26, 80, 15, pParent);
	pButton->SetImage		(OPTION_IMAGE_FILE1, 346,  180);
	pButton->SetImageHit	(OPTION_IMAGE_FILE1, 346,  196);
	pButton->SetImagePush	(OPTION_IMAGE_FILE1, 427,  180);
	pButton->SetImageDisable(OPTION_IMAGE_FILE1, 427,  196);
	pButton->SetImageCheck	(OPTION_IMAGE_FILE1, 427,  180);
	pButton->Show();
	m_pType[OPTION_TYPE_BENEFIT] = pButton;

	pParent = NULL;

	//WIID_OPTION_LINE_1
	pStatic = new SPWindowStatic( WIID_OPTION_LINE_1,	2,	45,	286,	1, this);		
	pStatic->SetImage(OPTION_IMAGE_FILE2, 349, 329);
	pStatic->SetSrcSize(6, 1);
	pStatic->Show();
	pStatic = NULL;

	//WIID_OPTION_LINE_2
	pStatic = new SPWindowStatic( WIID_OPTION_LINE_2,	2,	292,	286,	1, this);		
	pStatic->SetImage(OPTION_IMAGE_FILE2, 349, 329);
	pStatic->SetSrcSize(6, 1);
	pStatic->Show();
	pStatic = NULL;

	//WIID_OPTION_RESET
	pButton = new SPWindowButton( WIID_OPTION_RESET , 68, 300, 68, 17, this);
	pButton->SetImage		(OPTION_IMAGE_FILE2, 139,  269);		
	pButton->SetImageHit	(OPTION_IMAGE_FILE2, 139,  287);
	pButton->SetImagePush	(OPTION_IMAGE_FILE2, 139,  305);
	pButton->SetImageDisable(OPTION_IMAGE_FILE2, 139,  323);
	pButton->Show();
	
	//WIID_OPTION_OK
	pButton = new SPWindowButton( WIID_OPTION_OK , 140, 300, 68, 17, this);
	pButton->SetImage		(OPTION_IMAGE_FILE0, 75,  468);		
	pButton->SetImageHit	(OPTION_IMAGE_FILE0, 144,  468);
	pButton->SetImagePush	(OPTION_IMAGE_FILE0, 213,  468);
	pButton->SetImageDisable(OPTION_IMAGE_FILE0, 213,  468);
	pButton->Show();
	
	//WIID_OPTION_CANCEL
	pButton = new SPWindowButton( WIID_OPTION_CANCEL , 212, 300, 68, 17, this);
	pButton->SetImage		(OPTION_IMAGE_FILE4, 197,  218);		
	pButton->SetImageHit	(OPTION_IMAGE_FILE4, 197,  236);
	pButton->SetImagePush	(OPTION_IMAGE_FILE4, 216,  291);
	pButton->SetImageDisable(OPTION_IMAGE_FILE4, 216,  309);
	pButton->Show();

	//WIID_OPTION_SKIN2
	pParent = new SPWindowStatic( WIID_OPTION_SKIN2,	2,	46,	286,	249, this);		
	pParent->SetImage(OPTION_IMAGE_FILE2, 349, 321);
	pParent->SetSrcSize(6, 6);
	pParent->Show();	

	//WIID_OPTION_SYSTEM
	m_pBase[OPTION_TYPE_SYSTEM] = new SPWindowStatic( WIID_OPTION_SYSTEM,	0,	0,	286, 249, pParent);	
	m_pBase[OPTION_TYPE_SYSTEM]->Show();
	
	////WIID_OPTION_SYSTEM_JOYPAD
	if(SPLocalizeManager::GetInstance()->IsEnableValue(LV_JOYPAD)) {
		pButton = new SPWindowButton( WIID_OPTION_SYSTEM_JOYPAD , 196 - 2, 269 - 46, 76, 17, m_pBase[OPTION_TYPE_SYSTEM]);
		pButton->SetImage		(OPTION_IMAGE_FILE6, 240,  135);		
		pButton->SetImageHit	(OPTION_IMAGE_FILE6, 240,  153);
		pButton->SetImagePush	(OPTION_IMAGE_FILE6, 240,  171);
		pButton->SetImageDisable(OPTION_IMAGE_FILE6, 240,  189);
		pButton->Show();
	}

	//WIID_OPTION_MSG_IMAGE	
	pStatic = new SPWindowStatic( WIID_OPTION_MSG_IMAGE,	18 - 2, 70 - 46, 122, 15, m_pBase[OPTION_TYPE_SYSTEM]);
	pStatic->SetImage(OPTION_IMAGE_FILE0, 1, 388);	
	pStatic->Show();
	pStatic = NULL;

	//OPTION_SYSTEM_EXP
	pButton = new SPWindowButton( WIID_OPTION_SYSTEM_CHECK + OPTION_SYSTEM_EXP , 34 -  2, 95 - 46, 12, 12, m_pBase[OPTION_TYPE_SYSTEM]);
	SetCheckButton(pButton);
	m_pSystemCheck[OPTION_SYSTEM_EXP] = pButton;
	
	//WIID_OPTION_SYSTEM_EXP_IMG
	pStatic = new SPWindowStatic( WIID_OPTION_SYSTEM_EXP_IMG,	54 - 2, 94 - 46, 77, 13, m_pBase[OPTION_TYPE_SYSTEM]);
	pStatic->SetImage(OPTION_IMAGE_FILE4, 260, 54);	
	pStatic->Show();
	pStatic = NULL;
	
	//OPTION_SYSTEM_GAIN_ITEM
	pButton = new SPWindowButton( WIID_OPTION_SYSTEM_CHECK + OPTION_SYSTEM_GAIN_ITEM , 34 -  2, 111 - 46, 12, 12, m_pBase[OPTION_TYPE_SYSTEM]);
	SetCheckButton(pButton);
	m_pSystemCheck[OPTION_SYSTEM_GAIN_ITEM] = pButton;
	
	//WIID_OPTION_SYSTEM_ITEM_IMG	
	pStatic = new SPWindowStatic( WIID_OPTION_SYSTEM_ITEM_IMG,	54 - 2, 110 - 46, 77, 13, m_pBase[OPTION_TYPE_SYSTEM]);
	pStatic->SetImage(OPTION_IMAGE_FILE4, 260, 68);	
	pStatic->Show();
	pStatic = NULL;
	
	//OPTION_SYSTEM_USE_ITEM
	pButton = new SPWindowButton( WIID_OPTION_SYSTEM_CHECK + OPTION_SYSTEM_USE_ITEM , 34 -  2, 127 - 46, 12, 12, m_pBase[OPTION_TYPE_SYSTEM]);
	SetCheckButton(pButton);
	m_pSystemCheck[OPTION_SYSTEM_USE_ITEM] = pButton;
	
	//WIID_OPTION_SYSTEM_USE_IMG
	pStatic = new SPWindowStatic( WIID_OPTION_SYSTEM_USE_IMG,	54 - 2, 126 - 46, 77, 13, m_pBase[OPTION_TYPE_SYSTEM]);
	pStatic->SetImage(OPTION_IMAGE_FILE4, 260, 82);	
	pStatic->Show();
	pStatic = NULL;
	
	//OPTION_SYSTEM_HELP
	pButton = new SPWindowButton( WIID_OPTION_SYSTEM_CHECK + OPTION_SYSTEM_HELP , 34 -  2, 143 - 46, 12, 12, m_pBase[OPTION_TYPE_SYSTEM]);
	SetCheckButton(pButton);
	m_pSystemCheck[OPTION_SYSTEM_HELP] = pButton;
	
	//WIID_OPTION_SYSTEM_HELP_IMG
	pStatic = new SPWindowStatic( WIID_OPTION_SYSTEM_HELP_IMG,	54 - 2, 142 - 46, 77, 13, m_pBase[OPTION_TYPE_SYSTEM]);
	pStatic->SetImage(OPTION_IMAGE_FILE4, 260, 96);	
	pStatic->Show();
	pStatic = NULL;
	
	//WIID_OPTION_SYSTEM_SOUND_IMG
	pStatic = new SPWindowStatic( WIID_OPTION_SYSTEM_SOUND_IMG,	150 - 2, 70 - 46, 122, 15, m_pBase[OPTION_TYPE_SYSTEM]);
	pStatic->SetImage(OPTION_IMAGE_FILE0, 1, 404);	
	pStatic->Show();
	pStatic = NULL;
	
	//WIID_OPTION_SYSTEM_SOUND_SKIN
	SPWindowStatic* pSoundSkin = new SPWindowStatic( WIID_OPTION_SYSTEM_SOUND_SKIN,	150 - 2, 85 - 46, 122, 76, m_pBase[OPTION_TYPE_SYSTEM]);
	pSoundSkin->SetWindowType(WND_TYPE_TOPBASE);
	pSoundSkin->SetImageNormalHead(OPTION_IMAGE_FILE1, 24, 175, 146, 178);
	pSoundSkin->SetImageNormalBody(OPTION_IMAGE_FILE1, 24, 180, 146, 181);
	pSoundSkin->SetImageNormalTail(OPTION_IMAGE_FILE1, 24, 183, 146, 186);
	pSoundSkin->Show();
	
	//WIID_OPTION_SYSTEM_BGM_IMG
	pStatic = new SPWindowStatic( WIID_OPTION_SYSTEM_BGM_IMG,	180 - 150, 91 - 85, 47, 11, pSoundSkin);
	pStatic->SetImage(OPTION_IMAGE_FILE2, 185, 150);	
	pStatic->Show();
	pStatic = NULL;
	
	//OPTION_SYSTEM_BGM
	pButton = new SPWindowButton( WIID_OPTION_SYSTEM_CHECK + OPTION_SYSTEM_BGM , 160 -  150, 91 - 85, 12, 12, pSoundSkin);
	SetCheckButton(pButton);
	m_pSystemCheck[OPTION_SYSTEM_BGM] = pButton;

	//WIID_OPTION_SYSTEM_BGM_SLIDER
	RECT rtRect;
	pSlider = new SPWindowSlider( WIID_OPTION_SYSTEM_BGM_SLIDER , 160 - 150, 107 - 85, 102, 13, pSoundSkin);
	pSlider->SetWindowType(WND_TYPE_LEFTBASE);
	pSlider->SetHandlerRender(true);
	SetRect( &rtRect, 1, 93, 103, 106);
	pSlider->SetImagePageNormal( OPTION_IMAGE_FILE2, rtRect);
	pSlider->SetImagePageDisable( OPTION_IMAGE_FILE2, rtRect);
	SetRect( &rtRect, 1, 107, 103, 120);
	pSlider->SetImagePageHit( OPTION_IMAGE_FILE2, rtRect);		
	SetRect( &rtRect, 204, 224, 213, 237);	
	pSlider->SetImageHandle( OPTION_IMAGE_FILE2, rtRect);
	SetRect( &rtRect, 214, 224, 223, 237);
	pSlider->SetImageHandleHit( OPTION_IMAGE_FILE2, rtRect );
	SetRect( &rtRect, 224, 224, 233, 237);
	pSlider->SetImageHandlePush( OPTION_IMAGE_FILE2, rtRect );
	SetRect( &rtRect, 234, 224, 243, 237);
	pSlider->SetImageHandleDisable( OPTION_IMAGE_FILE2, rtRect );

	pSlider->SPSendMessage( SPIM_SET_MAXVALUE ,		255	);
	pSlider->SPSendMessage( SPIM_SET_MINVALUE ,		0	);
	pSlider->SPSendMessage( SPIM_SLIDER_PAGESIZE,	50	);	
	pSlider->SPSendMessage( SPIM_SET_CURVALUE ,		0);	
	pSlider->Show();
	pSlider->SetEnable(true);
	
	//WIID_OPTION_SYSTEM_SE_IMG
	pStatic = new SPWindowStatic( WIID_OPTION_SYSTEM_SE_IMG,	180 - 150, 124 - 85, 47, 11, pSoundSkin);
	pStatic->SetImage(OPTION_IMAGE_FILE2, 132, 163);	
	pStatic->Show();
	pStatic = NULL;

	//OPTION_SYSTEM_SE
	pButton = new SPWindowButton( WIID_OPTION_SYSTEM_CHECK + OPTION_SYSTEM_SE , 160 -  150, 124 - 85, 12, 12, pSoundSkin);
	SetCheckButton(pButton);
	m_pSystemCheck[OPTION_SYSTEM_SE] = pButton;	
	
	//WIID_OPTION_SYSTEM_SE_SLIDER	
	pSlider = new SPWindowSlider( WIID_OPTION_SYSTEM_SE_SLIDER , 160 - 150, 140 - 85, 102, 13, pSoundSkin);	
	pSlider->SetWindowType(WND_TYPE_LEFTBASE);
	pSlider->SetHandlerRender(true);
	SetRect( &rtRect, 1, 93, 103, 106);
	pSlider->SetImagePageNormal( OPTION_IMAGE_FILE2, rtRect);
	pSlider->SetImagePageDisable( OPTION_IMAGE_FILE2, rtRect);
	SetRect( &rtRect, 1, 107, 103, 120);
	pSlider->SetImagePageHit( OPTION_IMAGE_FILE2, rtRect);		
	SetRect( &rtRect, 204, 224, 213, 237);	
	pSlider->SetImageHandle( OPTION_IMAGE_FILE2, rtRect);
	SetRect( &rtRect, 214, 224, 223, 237);
	pSlider->SetImageHandleHit( OPTION_IMAGE_FILE2, rtRect );
	SetRect( &rtRect, 224, 224, 233, 237);
	pSlider->SetImageHandlePush( OPTION_IMAGE_FILE2, rtRect );
	SetRect( &rtRect, 234, 224, 243, 237);
	pSlider->SetImageHandleDisable( OPTION_IMAGE_FILE2, rtRect );

	pSlider->SPSendMessage( SPIM_SET_MAXVALUE ,		255	);
	pSlider->SPSendMessage( SPIM_SET_MINVALUE ,		0	);
	pSlider->SPSendMessage( SPIM_SLIDER_PAGESIZE,	50	);	
	pSlider->SPSendMessage( SPIM_SET_CURVALUE ,		0);
	pSlider->Show();
	pSlider->SetEnable(true);

	//WIID_OPTION_SYSTEM_GRA_IMG
	pStatic = new SPWindowStatic( WIID_OPTION_SYSTEM_GRA_IMG,	150 - 2, 169 - 46, 122, 15, m_pBase[OPTION_TYPE_SYSTEM]);
	pStatic->SetImage(OPTION_IMAGE_FILE0, 1, 420);	
	pStatic->Show();
	pStatic = NULL;
	
	//WIID_OPTION_SYSTEM_GRA_SKIN
	SPWindowStatic* pGraSkin = new SPWindowStatic( WIID_OPTION_SYSTEM_GRA_SKIN,	150 - 2, 184 - 46, 122, 76, m_pBase[OPTION_TYPE_SYSTEM]);
	pGraSkin->SetWindowType(WND_TYPE_TOPBASE);
	pGraSkin->SetImageNormalHead(OPTION_IMAGE_FILE1, 24, 175, 146, 178);
	pGraSkin->SetImageNormalBody(OPTION_IMAGE_FILE1, 24, 180, 146, 181);
	pGraSkin->SetImageNormalTail(OPTION_IMAGE_FILE1, 24, 183, 146, 186);
	pGraSkin->Show();
	
	//WIID_OPTION_SYSTEM_GAMMA_IMG
	pStatic = new SPWindowStatic( WIID_OPTION_SYSTEM_GAMMA_IMG,	168 - 150, 190 - 184, 47, 11, pGraSkin);
	pStatic->SetImage(OPTION_IMAGE_FILE2, 185, 163);	
	pStatic->Show();
	pStatic = NULL;

	//WIID_OPTION_SYSTEM_EFFECT_IMG
	pStatic = new SPWindowStatic( WIID_OPTION_SYSTEM_EFFECT_IMG, 168 - 150, 223 - 184, 47, 11, pGraSkin);
	pStatic->SetImage(OPTION_IMAGE_FILE4, 260, 110);	
	pStatic->Show();
	pStatic = NULL;
	
	//WIID_OPTION_SYSTEM_GAMMA_SLIDER
	pSlider = new SPWindowSlider( WIID_OPTION_SYSTEM_GAMMA_SLIDER , 160 - 150, 206 - 184, 102, 13, pGraSkin);
	pSlider->SetWindowType(WND_TYPE_LEFTBASE);
	pSlider->SetHandlerRender(true);
	SetRect( &rtRect, 1, 93, 103, 106);
	pSlider->SetImagePageNormal( OPTION_IMAGE_FILE2, rtRect);
	pSlider->SetImagePageDisable( OPTION_IMAGE_FILE2, rtRect);
	SetRect( &rtRect, 1, 107, 103, 120);
	pSlider->SetImagePageHit( OPTION_IMAGE_FILE2, rtRect);		
	SetRect( &rtRect, 204, 224, 213, 237);	
	pSlider->SetImageHandle( OPTION_IMAGE_FILE2, rtRect);
	SetRect( &rtRect, 214, 224, 223, 237);
	pSlider->SetImageHandleHit( OPTION_IMAGE_FILE2, rtRect );
	SetRect( &rtRect, 224, 224, 233, 237);
	pSlider->SetImageHandlePush( OPTION_IMAGE_FILE2, rtRect );
	SetRect( &rtRect, 234, 224, 243, 237);
	pSlider->SetImageHandleDisable( OPTION_IMAGE_FILE2, rtRect );

	pSlider->SPSendMessage( SPIM_SET_MAXVALUE ,		255	);
	pSlider->SPSendMessage( SPIM_SET_MINVALUE ,		0	);
	pSlider->SPSendMessage( SPIM_SLIDER_PAGESIZE,	50	);	
	pSlider->SPSendMessage( SPIM_SET_CURVALUE ,		0);
	pSlider->Show();
	if(g_bWindowed == true) {
		pSlider->SetEnable(false);
	}
	else {
		pSlider->SetEnable(true);
	}
	
	//WIID_OPTION_SYSTEM_EFFECT_SLIDER
	pSlider = new SPWindowSlider( WIID_OPTION_SYSTEM_EFFECT_SLIDER , 160 - 150, 239 - 184, 102, 13, pGraSkin);
	pSlider->SetWindowType(WND_TYPE_LEFTBASE);
	pSlider->SetHandlerRender(true);
	SetRect( &rtRect, 1, 93, 103, 106);
	pSlider->SetImagePageNormal( OPTION_IMAGE_FILE2, rtRect);
	pSlider->SetImagePageDisable( OPTION_IMAGE_FILE2, rtRect);
	SetRect( &rtRect, 1, 107, 103, 120);
	pSlider->SetImagePageHit( OPTION_IMAGE_FILE2, rtRect);		
	SetRect( &rtRect, 204, 224, 213, 237);	
	pSlider->SetImageHandle( OPTION_IMAGE_FILE2, rtRect);
	SetRect( &rtRect, 214, 224, 223, 237);
	pSlider->SetImageHandleHit( OPTION_IMAGE_FILE2, rtRect );
	SetRect( &rtRect, 224, 224, 233, 237);
	pSlider->SetImageHandlePush( OPTION_IMAGE_FILE2, rtRect );
	SetRect( &rtRect, 234, 224, 243, 237);
	pSlider->SetImageHandleDisable( OPTION_IMAGE_FILE2, rtRect );

	pSlider->SPSendMessage( SPIM_SET_MAXVALUE ,		EFFECT_SETP_MAX);
	//pSlider->SPSendMessage( SPIM_SET_MINVALUE ,		0	);
	pSlider->SPSendMessage( SPIM_SLIDER_PAGESIZE,	1);		
	pSlider->SPSendMessage( SPIM_SET_CURVALUE ,		1);
	//pSlider->SetCurpage(0);
	pSlider->Show();
	pSlider->SetEnable(true);

	//WIID_OPTION_COMMUNITY
	m_pBase[OPTION_TYPE_COMMUNITY] = new SPWindowStatic( WIID_OPTION_COMMUNITY,	0,	0,	286, 249, pParent);	
	m_pBase[OPTION_TYPE_COMMUNITY]->Show();
	
	//WIID_OPTION_COM_IMG
	pStatic = new SPWindowStatic( WIID_OPTION_COM_IMG,	18 - 2,	70 - 46, 122, 15, m_pBase[OPTION_TYPE_COMMUNITY]);
	pStatic->SetImage(OPTION_IMAGE_FILE0, 1, 436);	
	pStatic->Show();
	pStatic = NULL;

	//OPTION_COMMUNITY_TRADE
	pButton = new SPWindowButton( WIID_OPTION_COM_CHECK + OPTION_COMMUNITY_TRADE , 34 -  2, 95 - 46, 12, 12, m_pBase[OPTION_TYPE_COMMUNITY]);
	SetCheckButton(pButton);
	m_pCommunityCheck[OPTION_COMMUNITY_TRADE] = pButton;

	//WIID_OPTION_COM_TRADE_IMG
	pStatic = new SPWindowStatic( WIID_OPTION_COM_TRADE_IMG,	54 - 2,	95 - 46, 47, 11, m_pBase[OPTION_TYPE_COMMUNITY]);
	pStatic->SetImage(OPTION_IMAGE_FILE2, 185, 98);	
	pStatic->Show();
	pStatic = NULL;

	//OPTION_COMMUNITY_INVATE
	pButton = new SPWindowButton( WIID_OPTION_COM_CHECK + OPTION_COMMUNITY_INVATE , 34 -  2, 111 - 46, 12, 12, m_pBase[OPTION_TYPE_COMMUNITY]);
	SetCheckButton(pButton);
	m_pCommunityCheck[OPTION_COMMUNITY_INVATE] = pButton;

	//WIID_OPTION_COM_INVITE_IMG
	pStatic = new SPWindowStatic( WIID_OPTION_COM_INVITE_IMG,	54 - 2,	111 - 46, 47, 11, m_pBase[OPTION_TYPE_COMMUNITY]);
	pStatic->SetImage(OPTION_IMAGE_FILE2, 185, 111);	
	pStatic->Show();
	pStatic = NULL;

	//OPTION_COMMUNITY_PARTY
	pButton = new SPWindowButton( WIID_OPTION_COM_CHECK + OPTION_COMMUNITY_PARTY , 34 -  2, 127 - 46, 12, 12, m_pBase[OPTION_TYPE_COMMUNITY]);
	SetCheckButton(pButton);
	m_pCommunityCheck[OPTION_COMMUNITY_PARTY] = pButton;

	//WIID_OPTION_COM_PARTY_IMG
	pStatic = new SPWindowStatic( WIID_OPTION_COM_PARTY_IMG,	54 - 2,	126 - 46, 77, 13, m_pBase[OPTION_TYPE_COMMUNITY]);
	pStatic->SetImage(OPTION_IMAGE_FILE4, 260, 124);	
	pStatic->Show();
	pStatic = NULL;

	//OPTION_COMMUNITY_FRIEND_INVATE
	pButton = new SPWindowButton( WIID_OPTION_COM_CHECK + OPTION_COMMUNITY_FRIEND_INVATE , 34 -  2, 143 - 46, 12, 12, m_pBase[OPTION_TYPE_COMMUNITY]);
	SetCheckButton(pButton);	
	m_pCommunityCheck[OPTION_COMMUNITY_FRIEND_INVATE] = pButton;

	//WIID_OPTION_COM_FRIEND_INVITE_IMG
	pStatic = new SPWindowStatic( WIID_OPTION_COM_FRIEND_INVITE_IMG,	54 - 2,	143 - 46, 77, 13, m_pBase[OPTION_TYPE_COMMUNITY]);
	pStatic->SetImage(OPTION_IMAGE_FILE4, 207, 276);	
	pStatic->Show();	
	pStatic = NULL;

	//OPTION_COMMUNITY_ALERT
	pButton = new SPWindowButton( WIID_OPTION_COM_CHECK + OPTION_COMMUNITY_ALERT , 34 -  2, 159 - 46, 12, 12, m_pBase[OPTION_TYPE_COMMUNITY]);
	SetCheckButton(pButton);
	m_pCommunityCheck[OPTION_COMMUNITY_ALERT] = pButton;

	//WIID_OPTION_COM_ALERT_IMG
	pStatic = new SPWindowStatic( WIID_OPTION_COM_ALERT_IMG,	54 - 2,	159 - 46, 47, 11, m_pBase[OPTION_TYPE_COMMUNITY]);
	pStatic->SetImage(OPTION_IMAGE_FILE2, 185, 124);	
	pStatic->Show();
	pStatic = NULL;
	
	//WIID_OPTION_COM_CHAT_IMG
	pStatic = new SPWindowStatic( WIID_OPTION_COM_CHAT_IMG,	150 - 2,	70 - 46, 122, 15, m_pBase[OPTION_TYPE_COMMUNITY]);
	pStatic->SetImage(OPTION_IMAGE_FILE0, 1, 452);	
	pStatic->Show();
	pStatic = NULL;

	//OPTION_COMMUNITY_PARTY_CHAT
	pButton = new SPWindowButton( WIID_OPTION_COM_CHECK + OPTION_COMMUNITY_PARTY_CHAT , 158 -  2, 95 - 46, 12, 12, m_pBase[OPTION_TYPE_COMMUNITY]);
	SetCheckButton(pButton);
	m_pCommunityCheck[OPTION_COMMUNITY_PARTY_CHAT] = pButton;

	//WIID_OPTION_COM_CHAT_PARTY_IMG
	pStatic = new SPWindowStatic( WIID_OPTION_COM_CHAT_PARTY_IMG,	178 - 2, 94 - 46, 77, 13, m_pBase[OPTION_TYPE_COMMUNITY]);
	pStatic->SetImage(OPTION_IMAGE_FILE4, 260, 138);	
	pStatic->Show();
	pStatic = NULL;

	//OPTION_COMMUNITY_FRIEND_CHAT
	pButton = new SPWindowButton( WIID_OPTION_COM_CHECK + OPTION_COMMUNITY_FRIEND_CHAT , 158 -  2, 111 - 46, 12, 12, m_pBase[OPTION_TYPE_COMMUNITY]);
	SetCheckButton(pButton);
	m_pCommunityCheck[OPTION_COMMUNITY_FRIEND_CHAT] = pButton;
	
	//WIID_OPTION_COM_CHAT_FRIEND_IMG
	pStatic = new SPWindowStatic( WIID_OPTION_COM_CHAT_FRIEND_IMG,	178 - 2, 110 - 46, 77, 13, m_pBase[OPTION_TYPE_COMMUNITY]);
	pStatic->SetImage(OPTION_IMAGE_FILE4, 260, 152);	
	pStatic->Show();
	pStatic = NULL;

	//OPTION_COMMUNITY_GUILD_CHAT
	pButton = new SPWindowButton( WIID_OPTION_COM_CHECK + OPTION_COMMUNITY_GUILD_CHAT , 158 -  2, 127 - 46, 12, 12, m_pBase[OPTION_TYPE_COMMUNITY]);
	SetCheckButton(pButton);
	m_pCommunityCheck[OPTION_COMMUNITY_GUILD_CHAT] = pButton;
	//m_pCommunityCheck[OPTION_COMMUNITY_GUILD_CHAT]->SetEnable(false);
	
	//WIID_OPTION_COM_CHAT_GUILD_IMG
	pStatic = new SPWindowStatic( WIID_OPTION_COM_CHAT_GUILD_IMG,	178 - 2, 126 - 46, 77, 13, m_pBase[OPTION_TYPE_COMMUNITY]);
	pStatic->SetImage(OPTION_IMAGE_FILE4, 297, 170);	
	pStatic->Show();
	pStatic = NULL;

	//WIID_OPTION_COM_CHAT_ACCEPT_IMG
	pStatic = new SPWindowStatic( WIID_OPTION_COM_CHAT_ACCEPT_IMG,	150 - 2, 169 - 46, 122, 15, m_pBase[OPTION_TYPE_COMMUNITY]);
	pStatic->SetImage(OPTION_IMAGE_FILE0, 124, 388);	
	pStatic->Show();
	pStatic = NULL;

	////OPTION_COMMUNITY_PERMIT_FRIEND
	//pButton = new SPWindowButton( WIID_OPTION_COM_CHECK + OPTION_COMMUNITY_PERMIT_FRIEND , 158 -  2, 192 - 46, 12, 12, m_pBase[OPTION_TYPE_COMMUNITY]);
	//SetCheckButton(pButton);
	//m_pCommunityCheck[OPTION_COMMUNITY_PERMIT_FRIEND] = pButton;
	
	////WIID_OPTION_COM_CHAT_ACCEPT_FRIEND_IMG
	//pStatic = new SPWindowStatic( WIID_OPTION_COM_CHAT_ACCEPT_FRIEND_IMG,	178 - 2, 191 - 46, 77, 13, m_pBase[OPTION_TYPE_COMMUNITY]);
	//pStatic->SetImage(OPTION_IMAGE_FILE4, 297, 184);	
	//pStatic->Show();
	//pStatic = NULL;

	//OPTION_COMMUNITY_PERMIT_WHISPER
	pButton = new SPWindowButton( WIID_OPTION_COM_CHECK + OPTION_COMMUNITY_PERMIT_WHISPER , 158 -  2, 192 - 46, 12, 12, m_pBase[OPTION_TYPE_COMMUNITY]);
	SetCheckButton(pButton);
	m_pCommunityCheck[OPTION_COMMUNITY_PERMIT_WHISPER] = pButton;
	
	//WIID_OPTION_COM_CHAT_ACCEPT_WHISPER_IMG
	pStatic = new SPWindowStatic( WIID_OPTION_COM_CHAT_ACCEPT_WHISPER_IMG,	178 - 2, 191 - 46, 77, 13, m_pBase[OPTION_TYPE_COMMUNITY]);
	pStatic->SetImage(OPTION_IMAGE_FILE4, 297, 198);	
	pStatic->Show();
	pStatic = NULL;

	//WIID_OPTION_BENEFIT
	m_pBase[OPTION_TYPE_BENEFIT] = new SPWindowStatic( WIID_OPTION_BENEFIT,	0,	0,	286, 249, pParent);	
	m_pBase[OPTION_TYPE_BENEFIT]->Show();	

	//WIID_OPTIOM_BEN_NAME_IMG
	pStatic = new SPWindowStatic( WIID_OPTIOM_BEN_NAME_IMG,	18 - 2,	62 - 46, 122, 15, m_pBase[OPTION_TYPE_BENEFIT]);
	pStatic->SetImage(OPTION_IMAGE_FILE0, 124, 404);	
	pStatic->Show();
	pStatic = NULL;

	//OPTION_BENEFIT_INDICATE_PLAYER
	pButton = new SPWindowButton( WIID_OPTION_BEN_CHECK + OPTION_BENEFIT_INDICATE_PLAYER , 34 -  2, 87 - 46, 12, 12, m_pBase[OPTION_TYPE_BENEFIT]);
	SetCheckButton(pButton);
	m_pBenefitCheck[OPTION_BENEFIT_INDICATE_PLAYER] = pButton;

	//WIID_OPTIOM_BEN_NAME_PLAYER_IMG	
	pStatic = new SPWindowStatic( WIID_OPTIOM_BEN_NAME_PLAYER_IMG,	54 - 2,	87 - 46, 47, 11, m_pBase[OPTION_TYPE_BENEFIT]);
	pStatic->SetImage(OPTION_IMAGE_FILE2, 132, 98);	
	pStatic->Show();
	pStatic = NULL;

	//OPTION_BENEFIT_INDICATE_MONSTER
	pButton = new SPWindowButton( WIID_OPTION_BEN_CHECK + OPTION_BENEFIT_INDICATE_MONSTER , 34 -  2, 103 - 46, 12, 12, m_pBase[OPTION_TYPE_BENEFIT]);
	SetCheckButton(pButton);
	m_pBenefitCheck[OPTION_BENEFIT_INDICATE_MONSTER] = pButton;

	//WIID_OPTIOM_BEN_NAME_MONSTER_IMG
	pStatic = new SPWindowStatic( WIID_OPTIOM_BEN_NAME_MONSTER_IMG,	54 - 2,	103 - 46, 47, 11, m_pBase[OPTION_TYPE_BENEFIT]);
	pStatic->SetImage(OPTION_IMAGE_FILE2, 132, 111);	
	pStatic->Show();
	pStatic = NULL;

	//OPTION_BENEFIT_INDICATE_NPC
	pButton = new SPWindowButton( WIID_OPTION_BEN_CHECK + OPTION_BENEFIT_INDICATE_NPC , 34 -  2, 119 - 46, 12, 12, m_pBase[OPTION_TYPE_BENEFIT]);
	SetCheckButton(pButton);
	m_pBenefitCheck[OPTION_BENEFIT_INDICATE_NPC] = pButton;
	
	//WIID_OPTIOM_BEN_NAME_NPC_IMG	
	pStatic = new SPWindowStatic( WIID_OPTIOM_BEN_NAME_NPC_IMG,	54 - 2,	119 - 46, 47, 11, m_pBase[OPTION_TYPE_BENEFIT]);
	pStatic->SetImage(OPTION_IMAGE_FILE2, 132, 124);	
	pStatic->Show();
	pStatic = NULL;

	//OPTION_BENEFIT_INDICATE_PET
	pButton = new SPWindowButton( WIID_OPTION_BEN_CHECK + OPTION_BENEFIT_INDICATE_PET , 34 -  2, 135 - 46, 12, 12, m_pBase[OPTION_TYPE_BENEFIT]);
	SetCheckButton(pButton);
	m_pBenefitCheck[OPTION_BENEFIT_INDICATE_PET] = pButton;
	
	//WIID_OPTIOM_BEN_NAME_PET_IMG	
	pStatic = new SPWindowStatic( WIID_OPTIOM_BEN_NAME_PET_IMG,	54 - 2,	134 - 46, 77, 13, m_pBase[OPTION_TYPE_BENEFIT]);
	pStatic->SetImage(OPTION_IMAGE_FILE4, 297, 212);	
	pStatic->Show();
	pStatic = NULL;

	//WIID_OPTIOM_BEN_SPEECH_IMG		
	pStatic = new SPWindowStatic( WIID_OPTIOM_BEN_SPEECH_IMG,	150 - 2, 62 - 46, 122, 15, m_pBase[OPTION_TYPE_BENEFIT]);
	pStatic->SetImage(OPTION_IMAGE_FILE0, 124, 436);	
	pStatic->Show();
	pStatic = NULL;

	//OPTION_BENEFIT_SPEECH_PLAYER
	pButton = new SPWindowButton( WIID_OPTION_BEN_CHECK + OPTION_BENEFIT_SPEECH_PLAYER , 158 -  2, 87 - 46, 12, 12, m_pBase[OPTION_TYPE_BENEFIT]);
	SetCheckButton(pButton);
	m_pBenefitCheck[OPTION_BENEFIT_SPEECH_PLAYER] = pButton;
	
	//WIID_OPTIOM_BEN_SPH_PLAYER		
	pStatic = new SPWindowStatic( WIID_OPTIOM_BEN_SPH_PLAYER,	178 - 2, 87 - 46, 47, 11, m_pBase[OPTION_TYPE_BENEFIT]);
	pStatic->SetImage(OPTION_IMAGE_FILE2, 132, 137);	
	pStatic->Show();
	pStatic = NULL;

	//OPTION_BENEFIT_SPEECH_TEAM
	pButton = new SPWindowButton( WIID_OPTION_BEN_CHECK + OPTION_BENEFIT_SPEECH_TEAM , 158 -  2, 103 - 46, 12, 12, m_pBase[OPTION_TYPE_BENEFIT]);
	SetCheckButton(pButton);
	m_pBenefitCheck[OPTION_BENEFIT_SPEECH_TEAM] = pButton;

	//WIID_OPTIOM_BEN_SPH_PARTY		
	pStatic = new SPWindowStatic( WIID_OPTIOM_BEN_SPH_PARTY,	178 - 2, 103 - 46, 47, 11, m_pBase[OPTION_TYPE_BENEFIT]);
	pStatic->SetImage(OPTION_IMAGE_FILE2, 132, 150);	
	pStatic->Show();
	pStatic = NULL;

	//OPTION_BENEFIT_SPEECH_GUILD
	pButton = new SPWindowButton( WIID_OPTION_BEN_CHECK + OPTION_BENEFIT_SPEECH_GUILD , 158 -  2, 119 - 46, 12, 12, m_pBase[OPTION_TYPE_BENEFIT]);
	SetCheckButton(pButton);
	m_pBenefitCheck[OPTION_BENEFIT_SPEECH_GUILD] = pButton;
	//m_pBenefitCheck[OPTION_BENEFIT_SPEECH_GUILD]->SetEnable(false);
	
	//WIID_OPTIOM_BEN_SPH_GUILD		
	pStatic = new SPWindowStatic( WIID_OPTIOM_BEN_SPH_GUILD,	178 - 2, 118 - 46, 77, 13, m_pBase[OPTION_TYPE_BENEFIT]);
	pStatic->SetImage(OPTION_IMAGE_FILE4, 297, 226);	
	pStatic->Show();
	pStatic = NULL;

	//OPTION_BENEFIT_SPEECH_NPC
	pButton = new SPWindowButton( WIID_OPTION_BEN_CHECK + OPTION_BENEFIT_SPEECH_NPC , 158 -  2, 135 - 46, 12, 12, m_pBase[OPTION_TYPE_BENEFIT]);
	SetCheckButton(pButton);
	m_pBenefitCheck[OPTION_BENEFIT_SPEECH_NPC] = pButton;
	
	//WIID_OPTIOM_BEN_SPH_NPC			
	pStatic = new SPWindowStatic( WIID_OPTIOM_BEN_SPH_NPC,	178 - 2, 135 - 46, 47, 11, m_pBase[OPTION_TYPE_BENEFIT]);
	pStatic->SetImage(OPTION_IMAGE_FILE2, 132, 124);	
	pStatic->Show();
	pStatic = NULL;
	
	//WIID_OPTION_BEN_CURSOR_IMG		
	pStatic = new SPWindowStatic( WIID_OPTION_BEN_CURSOR_IMG,	18 - 2, 242 - 46, 122, 15, m_pBase[OPTION_TYPE_BENEFIT]);
	pStatic->SetImage(OPTION_IMAGE_FILE0, 124, 420);	
	pStatic->Show();
	pStatic = NULL;

	//OPTION_BENEFIT_CURSOR
	pButton = new SPWindowButton( WIID_OPTION_BEN_CHECK + OPTION_BENEFIT_CURSOR , 34 -  2, 265 - 46, 12, 12, m_pBase[OPTION_TYPE_BENEFIT]);
	SetCheckButton(pButton);
	m_pBenefitCheck[OPTION_BENEFIT_CURSOR] = pButton;
	
	//WIID_OPTION_BEN_CURSOR_HIDE_IMG	
	pStatic = new SPWindowStatic( WIID_OPTION_BEN_CURSOR_HIDE_IMG,	54 - 2, 265 - 46, 47, 11, m_pBase[OPTION_TYPE_BENEFIT]);
	pStatic->SetImage(OPTION_IMAGE_FILE2, 185, 137);	
	pStatic->Show();
	pStatic = NULL;
	
	//WIID_OPTION_BEN_HP_IMG			
	pStatic = new SPWindowStatic( WIID_OPTION_BEN_HP_IMG,	150 - 2, 161 - 46, 122, 15, m_pBase[OPTION_TYPE_BENEFIT]);
	pStatic->SetImage(OPTION_IMAGE_FILE0, 124, 452);	
	pStatic->Show();
	pStatic = NULL;

	//OPTION_BENEFIT_HP_SELF
	pButton = new SPWindowButton( WIID_OPTION_BEN_CHECK + OPTION_BENEFIT_HP_SELF , 158 -  2, 184 - 46, 12, 12, m_pBase[OPTION_TYPE_BENEFIT]);
	SetCheckButton(pButton);
	m_pBenefitCheck[OPTION_BENEFIT_HP_SELF] = pButton;

	//WIID_OPTION_BEN_HP_SELF_IMG		
	pStatic = new SPWindowStatic( WIID_OPTION_BEN_HP_SELF_IMG,	178 - 2, 183 - 46, 77, 13, m_pBase[OPTION_TYPE_BENEFIT]);
	pStatic->SetImage(OPTION_IMAGE_FILE4, 219, 176);	
	pStatic->Show();
	pStatic = NULL;

	//OPTION_BENEFIT_HP_PARTY
	pButton = new SPWindowButton( WIID_OPTION_BEN_CHECK + OPTION_BENEFIT_HP_PARTY , 158 -  2, 200 - 46, 12, 12, m_pBase[OPTION_TYPE_BENEFIT]);
	SetCheckButton(pButton);
	m_pBenefitCheck[OPTION_BENEFIT_HP_PARTY] = pButton;
	
	//WIID_OPTION_BEN_HP_PARTY_IMG	
	pStatic = new SPWindowStatic( WIID_OPTION_BEN_HP_PARTY_IMG,	178 - 2, 199 - 46, 77, 13, m_pBase[OPTION_TYPE_BENEFIT]);
	pStatic->SetImage(OPTION_IMAGE_FILE4, 219, 190);	
	pStatic->Show();
	pStatic = NULL;

	//OPTION_BENEFIT_HP_PLAYER
	pButton = new SPWindowButton( WIID_OPTION_BEN_CHECK + OPTION_BENEFIT_HP_PLAYER , 158 -  2, 216 - 46, 12, 12, m_pBase[OPTION_TYPE_BENEFIT]);
	SetCheckButton(pButton);
	m_pBenefitCheck[OPTION_BENEFIT_HP_PLAYER] = pButton;
	
	//WIID_OPTION_BEN_HP_PLAYER_IMG	
	pStatic = new SPWindowStatic( WIID_OPTION_BEN_HP_PLAYER_IMG,	178 - 2, 215 - 46, 77, 13, m_pBase[OPTION_TYPE_BENEFIT]);
	pStatic->SetImage(OPTION_IMAGE_FILE4, 219, 204);	
	pStatic->Show();
	pStatic = NULL;	
	
	//WIID_OPTION_BEN_SCREEN_IMG
	pStatic = new SPWindowStatic( WIID_OPTION_BEN_SCREEN_IMG,	18 - 2, 161 - 46, 122, 15, m_pBase[OPTION_TYPE_BENEFIT]);
	pStatic->SetImage(OPTION_IMAGE_FILE5, 369, 182);
	pStatic->Show();
	pStatic = NULL;
	
	/*
	//OPTION_BENEFIT_SCR_PNG
	pButton = new SPWindowButton( WIID_OPTION_BEN_CHECK + OPTION_BENEFIT_SCR_PNG , 35 -  2, 185 - 46, 10, 10, m_pBase[OPTION_TYPE_BENEFIT]);
	SetRadioButton(pButton);
	m_pBenefitCheck[OPTION_BENEFIT_SCR_PNG] = pButton;

	//WIID_OPTION_BEN_SCREEN_PNG_IMG	
	pStatic = new SPWindowStatic( WIID_OPTION_BEN_SCREEN_PNG_IMG,	54 - 2, 184 - 46, 25, 12, m_pBase[OPTION_TYPE_BENEFIT]);
	pStatic->SetImage(OPTION_IMAGE_FILE1, 411, 95);
	pStatic->Show();
	pStatic = NULL;

	//OPTION_BENEFIT_SCR_BMP
	pButton = new SPWindowButton( WIID_OPTION_BEN_CHECK + OPTION_BENEFIT_SCR_BMP , 35 -  2, 201 - 46, 10, 10, m_pBase[OPTION_TYPE_BENEFIT]);
	SetRadioButton(pButton);
	m_pBenefitCheck[OPTION_BENEFIT_SCR_BMP] = pButton;

	//WIID_OPTION_BEN_SCREEN_BMP_IMG	
	pStatic = new SPWindowStatic( WIID_OPTION_BEN_SCREEN_BMP_IMG,	54 - 2, 200 - 46, 25, 12, m_pBase[OPTION_TYPE_BENEFIT]);
	pStatic->SetImage(OPTION_IMAGE_FILE1, 437, 95);
	pStatic->Show();
	pStatic = NULL;

	//OPTION_BENEFIT_SCR_JPG
	pButton = new SPWindowButton( WIID_OPTION_BEN_CHECK + OPTION_BENEFIT_SCR_JPG , 35 -  2, 217 - 46, 10, 10, m_pBase[OPTION_TYPE_BENEFIT]);
	SetRadioButton(pButton);
	m_pBenefitCheck[OPTION_BENEFIT_SCR_JPG] = pButton;

	//WIID_OPTION_BEN_SCREEN_JPG_IMG	
	pStatic = new SPWindowStatic( WIID_OPTION_BEN_SCREEN_JPG_IMG,	54 - 2, 216 - 46, 25, 12, m_pBase[OPTION_TYPE_BENEFIT]);
	pStatic->SetImage(OPTION_IMAGE_FILE1, 463, 95);
	pStatic->Show();
	pStatic = NULL;
	*/

	//OPTION_BENEFIT_SCR_JPG
	pButton = new SPWindowButton( WIID_OPTION_BEN_CHECK + OPTION_BENEFIT_SCR_JPG , 35 -  2, 185 - 46, 10, 10, m_pBase[OPTION_TYPE_BENEFIT]);
	SetRadioButton(pButton);
	m_pBenefitCheck[OPTION_BENEFIT_SCR_JPG] = pButton;

	//WIID_OPTION_BEN_SCREEN_JPG_IMG	
	pStatic = new SPWindowStatic( WIID_OPTION_BEN_SCREEN_JPG_IMG,	54 - 2, 184 - 46, 25, 12, m_pBase[OPTION_TYPE_BENEFIT]);
	pStatic->SetImage(OPTION_IMAGE_FILE1, 463, 95);
	pStatic->Show();
	pStatic = NULL;

	//OPTION_BENEFIT_SCR_PNG
	pButton = new SPWindowButton( WIID_OPTION_BEN_CHECK + OPTION_BENEFIT_SCR_PNG , 35 -  2, 201 - 46, 10, 10, m_pBase[OPTION_TYPE_BENEFIT]);
	SetRadioButton(pButton);
	m_pBenefitCheck[OPTION_BENEFIT_SCR_PNG] = pButton;

	//WIID_OPTION_BEN_SCREEN_PNG_IMG	
	pStatic = new SPWindowStatic( WIID_OPTION_BEN_SCREEN_PNG_IMG,	54 - 2, 200 - 46, 25, 12, m_pBase[OPTION_TYPE_BENEFIT]);
	pStatic->SetImage(OPTION_IMAGE_FILE1, 411, 95);
	pStatic->Show();
	pStatic = NULL;

	//OPTION_BENEFIT_SCR_BMP
	pButton = new SPWindowButton( WIID_OPTION_BEN_CHECK + OPTION_BENEFIT_SCR_BMP , 35 -  2, 217 - 46, 10, 10, m_pBase[OPTION_TYPE_BENEFIT]);
	SetRadioButton(pButton);
	m_pBenefitCheck[OPTION_BENEFIT_SCR_BMP] = pButton;

	//WIID_OPTION_BEN_SCREEN_BMP_IMG	
	pStatic = new SPWindowStatic( WIID_OPTION_BEN_SCREEN_BMP_IMG,	54 - 2, 216 - 46, 25, 12, m_pBase[OPTION_TYPE_BENEFIT]);
	pStatic->SetImage(OPTION_IMAGE_FILE1, 437, 95);
	pStatic->Show();
	pStatic = NULL;
}


void SPWindowOption::SetCheckButton(SPWindowButton* pButton /*= NULL*/)
{
	if(pButton){
		char* OPTION_IMAGE_FILE2 = "DATA/INTERFACE/CONCEPT/UI102.PNG";		
		pButton->SetImage		(OPTION_IMAGE_FILE2, 204, 211);
		pButton->SetImageHit	(OPTION_IMAGE_FILE2, 217, 211);
		pButton->SetImagePush	(OPTION_IMAGE_FILE2, 230, 211);
		pButton->SetImageDisable(OPTION_IMAGE_FILE2, 239, 198);
		pButton->SetImageCheck	(OPTION_IMAGE_FILE2, 230, 211);						
		pButton->Show();
	}	
}


void SPWindowOption::SetRadioButton(SPWindowButton* pButton /*= NULL*/)
{
	if(pButton){
		char* OPTION_IMAGE_FILE2 = "DATA/INTERFACE/CONCEPT/UI102.PNG";		
		pButton->SetImage		(OPTION_IMAGE_FILE2, 76, 164);
		pButton->SetImageHit	(OPTION_IMAGE_FILE2, 87, 164);
		pButton->SetImagePush	(OPTION_IMAGE_FILE2, 98, 164);
		pButton->SetImageDisable(OPTION_IMAGE_FILE2, 109, 164);
		pButton->SetImageCheck	(OPTION_IMAGE_FILE2, 98, 164);						
		pButton->Show();
	}
}


void SPWindowOption::Clean()
{	
	SAFE_RELEASE(m_pTextureBase);
	SAFE_DELETE(m_pCopyOption)
	SPWindow::Clean();
}


void SPWindowOption::Show(bool bWithChild)
{	
	SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_JOYSTICK);
	if(pWindow && pWindow->IsShow()) {
		return;
	}


	SPWindow::Show(bWithChild);
	
	m_iOptionType = OPTION_TYPE_SYSTEM;
	UpdateSubControl();
	g_pClientOption->OptionCopy(m_pCopyOption);
	UpdateData(REFRESH_CLIENTSIDE);

	m_cusCopyServer = g_pClientOption->GetServerSideOption();
	UpdateData(REFRESH_SERVERSIDE);
}


void SPWindowOption::UpdateSubControl()
{
	//
	for(int i = 0; i < OPTION_TYPE_MAX; i++) {
		if(i == m_iOptionType) {
			if(m_pBase[i])
				m_pBase[i]->Show();
			
			if(m_pType[i])
				m_pType[i]->SPSendMessage(SPIM_BTN_SELECT, TRUE);
		}
		else {
			if(m_pBase[i])
				m_pBase[i]->Hide();
			
			if(m_pType[i])
				m_pType[i]->SPSendMessage(SPIM_BTN_SELECT, FALSE);
		}
	}
}


void SPWindowOption::UpdateData(unsigned int  iRefreshOption)
{
	SPWindow* pWindow = NULL;
	
	if(iRefreshOption & REFRESH_SERVERSIDE) {
		//m_cusCopyServer = g_pClientOption->GetServerSideOption();

		m_pCommunityCheck[OPTION_COMMUNITY_TRADE]->SPSendMessage(SPIM_BTN_SELECT, (m_cusCopyServer & SSO_REQ_TRADE)? TRUE : FALSE);		
		m_pCommunityCheck[OPTION_COMMUNITY_INVATE]->SPSendMessage(SPIM_BTN_SELECT, (m_cusCopyServer & SSO_REQ_TEAM)? TRUE : FALSE);
		m_pCommunityCheck[OPTION_COMMUNITY_PARTY]->SPSendMessage(SPIM_BTN_SELECT, (m_cusCopyServer & SSO_REQ_WANTTEAM)? TRUE : FALSE);		
		m_pCommunityCheck[OPTION_COMMUNITY_FRIEND_INVATE]->SPSendMessage(SPIM_BTN_SELECT, (m_cusCopyServer & SSO_REQ_FRIEND)? TRUE : FALSE);
		m_pCommunityCheck[OPTION_COMMUNITY_ALERT]->SPSendMessage(SPIM_BTN_SELECT, (m_cusCopyServer & SSO_REQ_ALERT)? TRUE : FALSE);		
		m_pCommunityCheck[OPTION_COMMUNITY_PARTY_CHAT]->SPSendMessage(SPIM_BTN_SELECT, (m_cusCopyServer & SSO_REQ_CHATTEAM)? TRUE : FALSE);		
		m_pCommunityCheck[OPTION_COMMUNITY_FRIEND_CHAT]->SPSendMessage(SPIM_BTN_SELECT, (m_cusCopyServer & SSO_REQ_CHATFRIEND)? TRUE : FALSE);		
		m_pCommunityCheck[OPTION_COMMUNITY_GUILD_CHAT]->SPSendMessage(SPIM_BTN_SELECT, (m_cusCopyServer & SSO_REQ_CHATGUILD)? TRUE : FALSE);
		m_pCommunityCheck[OPTION_COMMUNITY_PERMIT_WHISPER]->SPSendMessage(SPIM_BTN_SELECT, (m_cusCopyServer & SSO_REQ_WHISPER)? TRUE : FALSE);

	}
	
	if(iRefreshOption & REFRESH_CLIENTSIDE) {
		//g_pClientOption->OptionCopy(m_pCopyOption);

		m_pSystemCheck[OPTION_SYSTEM_EXP]->SPSendMessage(SPIM_BTN_SELECT, m_pCopyOption->bExp);
		m_pSystemCheck[OPTION_SYSTEM_GAIN_ITEM]->SPSendMessage(SPIM_BTN_SELECT, m_pCopyOption->bItem);
		m_pSystemCheck[OPTION_SYSTEM_USE_ITEM]->SPSendMessage(SPIM_BTN_SELECT, m_pCopyOption->bUseItem);
		m_pSystemCheck[OPTION_SYSTEM_HELP]->SPSendMessage(SPIM_BTN_SELECT, m_pCopyOption->bHelp);
		m_pSystemCheck[OPTION_SYSTEM_BGM]->SPSendMessage(SPIM_BTN_SELECT, m_pCopyOption->bEnableBGM);
		m_pSystemCheck[OPTION_SYSTEM_SE]->SPSendMessage(SPIM_BTN_SELECT, m_pCopyOption->bEnableSE);

		pWindow = Find(WIID_OPTION_SYSTEM_BGM_SLIDER, true);
		if(pWindow) {
			pWindow->SPSendMessage( SPIM_SET_CURVALUE, m_pCopyOption->iVolumeBGM);
		}

		pWindow = Find(WIID_OPTION_SYSTEM_SE_SLIDER, true);
		if(pWindow) {
			pWindow->SPSendMessage( SPIM_SET_CURVALUE, m_pCopyOption->iVolumeSE);
		}

		pWindow = Find(WIID_OPTION_SYSTEM_GAMMA_SLIDER, true);
		if(pWindow) {
			pWindow->SPSendMessage( SPIM_SET_CURVALUE, m_pCopyOption->iGammaRamp);
		}

		pWindow = Find(WIID_OPTION_SYSTEM_EFFECT_SLIDER, true);
		if(pWindow) {
			pWindow->SPSendMessage( SPIM_SET_CURVALUE, m_pCopyOption->iEffect);
		}		

		m_pBenefitCheck[OPTION_BENEFIT_INDICATE_PLAYER]->SPSendMessage(SPIM_BTN_SELECT,		m_pCopyOption->bIndicatePlayer);
		m_pBenefitCheck[OPTION_BENEFIT_INDICATE_MONSTER]->SPSendMessage(SPIM_BTN_SELECT,	m_pCopyOption->bIndicateMonster);
		m_pBenefitCheck[OPTION_BENEFIT_INDICATE_NPC]->SPSendMessage(SPIM_BTN_SELECT,		m_pCopyOption->bIndicateNPC);
		m_pBenefitCheck[OPTION_BENEFIT_INDICATE_PET]->SPSendMessage(SPIM_BTN_SELECT,		m_pCopyOption->bIndicatePet);
		m_pBenefitCheck[OPTION_BENEFIT_SPEECH_PLAYER]->SPSendMessage(SPIM_BTN_SELECT,		m_pCopyOption->bSpeechPlayer);
		m_pBenefitCheck[OPTION_BENEFIT_SPEECH_TEAM]->SPSendMessage(SPIM_BTN_SELECT,			m_pCopyOption->bSpeechTeam);
		m_pBenefitCheck[OPTION_BENEFIT_SPEECH_GUILD]->SPSendMessage(SPIM_BTN_SELECT,		m_pCopyOption->bSpeechGuild);
		m_pBenefitCheck[OPTION_BENEFIT_SPEECH_NPC]->SPSendMessage(SPIM_BTN_SELECT,			m_pCopyOption->bSpeechNPC);
		m_pBenefitCheck[OPTION_BENEFIT_CURSOR]->SPSendMessage(SPIM_BTN_SELECT,				m_pCopyOption->bAutoHideMouse);
		m_pBenefitCheck[OPTION_BENEFIT_HP_SELF]->SPSendMessage(SPIM_BTN_SELECT,				m_pCopyOption->bHpBarSelf);
		m_pBenefitCheck[OPTION_BENEFIT_HP_PARTY]->SPSendMessage(SPIM_BTN_SELECT,			m_pCopyOption->bHpBarParty);
		m_pBenefitCheck[OPTION_BENEFIT_HP_PLAYER]->SPSendMessage(SPIM_BTN_SELECT,			m_pCopyOption->bHpBarPlayer);
		UpdateScreenShotFormat(m_pCopyOption);
	}
}


void SPWindowOption::UpdateCanCel()
{
	SPOptionStruct* pOption = g_pClientOption->GetOptionStruct();

	float fVar;
	Convert255To1( fVar, pOption->iVolumeBGM );	
	SPSoundBase::GetInstance()->UpdateBGMVolume( fVar*2 );

	Convert255To1( fVar, pOption->iVolumeSE );	
	SPSoundBase::GetInstance()->UpdateEffectVolume( fVar );

	if(g_bWindowed == false) {		
		g_pVideo->SetContrast( pOption->iGammaRamp );
	}

}


void SPWindowOption::UpdateScreenShotFormat(SPOptionStruct* pOption /*= NULL*/)
{
	if(pOption == NULL)
		return;

	for(int i = OPTION_BENEFIT_SCR_JPG; i < OPTION_BENEFIT_MAX; i++) {
		if(pOption->iScreenShot + OPTION_BENEFIT_SCR_JPG == i)
			m_pBenefitCheck[i]->SPSendMessage(SPIM_BTN_SELECT, true);
		else
			m_pBenefitCheck[i]->SPSendMessage(SPIM_BTN_SELECT, false);
	}
}


void SPWindowOption::Hide(bool bSendServer /*= true*/)
{
	////[2005/11/15] 檬扁拳 窍搁辑 辑滚 可记档 檬扁拳 矫难滚覆 OnHide肺 快急捞悼
	//if(bSendServer) {
	//	g_pClientOption->SPPerformMessage( SPOAM_COMMIT_TO_SERVER );
	//}	
	
	SPWindow::Hide();
}

void SPWindowOption::Process(float fTime)
{
	if(m_bShow == false)		return;
	
	SetRect(&m_rtDestBase[WINDOW_BASE_TOP_LEFT],		m_iAX, m_iAY, m_iAX + 4, m_iAY + 4);
	SetRect(&m_rtDestBase[WINDOW_BASE_TOP],				m_iAX + 4, m_iAY, m_iAX + 4 + 282, m_iAY + 4);	
	SetRect(&m_rtDestBase[WINDOW_BASE_TOP_RIGHT],		m_iAX + 286, m_iAY, m_iAX + 286 + 4, m_iAY + 4);
	SetRect(&m_rtDestBase[WINDOW_BASE_CENTER_LEFT],		m_iAX, m_iAY + 4, m_iAX + 4, m_iAY + 4 + 315);
	SetRect(&m_rtDestBase[WINDOW_BASE_CENTER],			m_iAX + 4, m_iAY + 4, m_iAX + 4 + 282, m_iAY + 4 + 315);
	SetRect(&m_rtDestBase[WINDOW_BASE_CENTER_RIGHT],	m_iAX + 286, m_iAY + 4, m_iAX + 286 + 4, m_iAY + 4 + 315);
	SetRect(&m_rtDestBase[WINDOW_BASE_BOTTOM_LEFT],		m_iAX, m_iAY + 319, m_iAX + 4, m_iAY + 319 + 4);
	SetRect(&m_rtDestBase[WINDOW_BASE_BOTTOM],			m_iAX + 4, m_iAY + 319, m_iAX + 4 + 282, m_iAY + 319 + 4);
	SetRect(&m_rtDestBase[WINDOW_BASE_BOTTOM_RIGHT],	m_iAX + 286, m_iAY + 319, m_iAX + 286 + 4, m_iAY + 319 + 4);	
	
	SPWindow::Process(fTime);
}

void SPWindowOption::Render(float fTime)
{
	if(m_bShow == false)		return;	

	if(m_pTextureBase) {
		g_pVideo->Flush();
		
		for(int i = 0; i < WINDOW_BASE_MAX; i++) {
			m_pTextureBase->RenderTexture(&m_rtDestBase[i], &m_rtSrcBase[i]);
		}		
	}	

	SPWindow::Render(fTime);

#ifdef _DEBUG
	////
	//g_pVideo->Flush();
	//g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));	
	//for(int i = 0; i < OPTION_BASE_MAX; i++) {
	//	g_pVideo->GetSysLine()->DrawRect(m_rtDestBase[i]);
	//}
	//g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
#endif
}



SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowOption )
	SPIMESSAGE_COMMAND( SPIM_REFRESH,		OnReceiveOption )
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP	,	WIID_OPTION_EXIT				,	OnExit				)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP	,	WIID_OPTION_RESET				,	OnReset				)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP	,	WIID_OPTION_OK					,	OnOk				)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP	,	WIID_OPTION_CANCEL				,	OnCancel			)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP	,	WIID_OPTION_SYSTEM_JOYPAD		,	OnJoyPad			)

	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP	,	WIID_OPTION_SYSTEM_BUTTON		,	OnSystem			)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP	,	WIID_OPTION_COMMUNITY_BUTTON	,	OnCommunity			)
	SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP	,	WIID_OPTION_BENEFIT_BUTTON		,	OnBenefit			)
	
	SPIMESSAGE_CONTROL_RANGE(SPIM_LBUTTON_UP, WIID_OPTION_SYSTEM_CHECK, WIID_OPTION_SYSTEM_CHECK + OPTION_SYSTEM_MAX,	OnCheckSystem)
	SPIMESSAGE_CONTROL_RANGE(SPIM_LBUTTON_UP, WIID_OPTION_COM_CHECK,	WIID_OPTION_COM_CHECK + OPTION_COMMUNITY_MAX,	OnCheckCommunity)
	SPIMESSAGE_CONTROL_RANGE(SPIM_LBUTTON_UP, WIID_OPTION_BEN_CHECK,	WIID_OPTION_BEN_CHECK + OPTION_BENEFIT_MAX,		OnCheckBenefit)	

	SPIMESSAGE_CONTROL(	SPIM_REFRESH	,	WIID_OPTION_SYSTEM_BGM_SLIDER		,	OnBGM)
	SPIMESSAGE_CONTROL(	SPIM_REFRESH	,	WIID_OPTION_SYSTEM_SE_SLIDER		,	OnSoundEffect)
	SPIMESSAGE_CONTROL(	SPIM_REFRESH	,	WIID_OPTION_SYSTEM_GAMMA_SLIDER		,	OnGammaRamp)
	SPIMESSAGE_CONTROL(	SPIM_REFRESH	,	WIID_OPTION_SYSTEM_EFFECT_SLIDER	,	OnEffect)
SPIMESSAGE_OBJ_MAP_END	( SPWindow	)		


int SPWindowOption::OnReceiveOption		(WPARAM wParam, LPARAM lparam)
{
	m_cusCopyServer = g_pClientOption->GetServerSideOption();
	UpdateData(wParam);
	return 1;
}

int SPWindowOption::OnExit(WPARAM wParam, LPARAM lparam)
{
	UpdateCanCel();
	Hide();
	return 1;
}


int SPWindowOption::OnReset(WPARAM wParam, LPARAM lparam)
{	
	g_pClientOption->SetDefault(m_pCopyOption);
	UpdateData(REFRESH_CLIENTSIDE);
	
	if(m_cusCopyServer != SSO_DEFAULT) {
		m_cusCopyServer = SSO_DEFAULT;
	}
	UpdateData(REFRESH_SERVERSIDE);	
	
	return 1;
}


int SPWindowOption::OnOk(WPARAM wParam, LPARAM lparam)
{
	SPOptionStruct* pSrcOption = NULL;
	pSrcOption = g_pClientOption->GetOptionStruct();	

	SERVER_SIDE_OPTION pSrcSvrOption = g_pClientOption->GetServerSideOption();
	
	//if(bSendServer) {
	if(pSrcSvrOption != m_cusCopyServer) {
		g_pClientOption->SetServerSideOption(m_cusCopyServer);
		g_pClientOption->SPPerformMessage( SPOAM_COMMIT_TO_SERVER );
	}	
	//}
	
	g_pClientOption->OptionCopy(pSrcOption, m_pCopyOption);
	UpdateData(REFRESH_CLIENTSIDE);
	g_pClientOption->ClientSave();

	Hide();
	
	return 1;
}


int SPWindowOption::OnCancel(WPARAM wParam, LPARAM lparam)
{
	UpdateCanCel();
	Hide();
	return 1;
}


int SPWindowOption::OnJoyPad(WPARAM wParam, LPARAM lparam)
{
	if(m_iOptionType != OPTION_TYPE_SYSTEM) 
		return 1;

	if(SPLocalizeManager::GetInstance()->IsEnableValue(LV_JOYPAD)) {
		SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_JOYSTICK);
		if(pWindow && pWindow->IsShow() == false){
			Hide();
			pWindow->Show();
		}
	}
	return 1;
}


int SPWindowOption::OnSystem(WPARAM wParam, LPARAM lparam)
{
	m_iOptionType = OPTION_TYPE_SYSTEM;
	UpdateSubControl();
	return 1;
}


int SPWindowOption::OnCommunity(WPARAM wParam, LPARAM lparam)
{
	m_iOptionType = OPTION_TYPE_COMMUNITY;
	UpdateSubControl();
	return 1;
}


int SPWindowOption::OnBenefit(WPARAM wParam, LPARAM lparam)
{
	m_iOptionType = OPTION_TYPE_BENEFIT;
	UpdateSubControl();
	return 1;
}


int SPWindowOption::OnCheckSystem(unsigned int iID, WPARAM wParam, LPARAM lparam)
{
	int iSelect = iID - WIID_OPTION_SYSTEM_CHECK;
	DXUTOutputDebugString("	- CheckSystem[%d] \n", iSelect);	
	
	if(m_iOptionType == OPTION_TYPE_SYSTEM) {		
		switch(iSelect) {
			case OPTION_SYSTEM_EXP:
				(m_pCopyOption->bExp)?			m_pCopyOption->bExp = false : m_pCopyOption->bExp = true;
				m_pSystemCheck[OPTION_SYSTEM_EXP]->SPSendMessage(SPIM_BTN_SELECT,			m_pCopyOption->bExp);
				break;
			case OPTION_SYSTEM_GAIN_ITEM:
				(m_pCopyOption->bItem)?			m_pCopyOption->bItem = false : m_pCopyOption->bItem = true;
				m_pSystemCheck[OPTION_SYSTEM_GAIN_ITEM]->SPSendMessage(SPIM_BTN_SELECT,		m_pCopyOption->bItem);
				break;
			case OPTION_SYSTEM_USE_ITEM:
				(m_pCopyOption->bUseItem)?		m_pCopyOption->bUseItem = false : m_pCopyOption->bUseItem = true;
				m_pSystemCheck[OPTION_SYSTEM_USE_ITEM]->SPSendMessage(SPIM_BTN_SELECT,		m_pCopyOption->bUseItem);
				break;
			case OPTION_SYSTEM_HELP:
				(m_pCopyOption->bHelp)?			m_pCopyOption->bHelp = false : m_pCopyOption->bHelp = true;
				m_pSystemCheck[OPTION_SYSTEM_HELP]->SPSendMessage(SPIM_BTN_SELECT,			m_pCopyOption->bHelp);
				break;
			case OPTION_SYSTEM_BGM:
				(m_pCopyOption->bEnableBGM)?	m_pCopyOption->bEnableBGM = false : m_pCopyOption->bEnableBGM = true;
				m_pSystemCheck[OPTION_SYSTEM_BGM]->SPSendMessage(SPIM_BTN_SELECT,			m_pCopyOption->bEnableBGM);
				break;
			case OPTION_SYSTEM_SE:
				(m_pCopyOption->bEnableSE)?		m_pCopyOption->bEnableSE = false : m_pCopyOption->bEnableSE = true;
				m_pSystemCheck[OPTION_SYSTEM_SE]->SPSendMessage(SPIM_BTN_SELECT,			m_pCopyOption->bEnableSE);
				break;
			default:
				break;
		}		
	}

	return 1;
}


int SPWindowOption::OnCheckCommunity(unsigned int iID, WPARAM wParam, LPARAM lparam)
{
	int iSelect = iID - WIID_OPTION_COM_CHECK;
	DXUTOutputDebugString("	- CheckCommunity[%d] \n", iSelect);	
	
	if(m_iOptionType == OPTION_TYPE_COMMUNITY) {
		switch(iSelect) {
			case OPTION_COMMUNITY_TRADE:
				(m_cusCopyServer & SSO_REQ_TRADE)?	m_cusCopyServer &= ~SSO_REQ_TRADE : m_cusCopyServer |= SSO_REQ_TRADE;
				m_pCommunityCheck[OPTION_COMMUNITY_TRADE]->SPSendMessage(SPIM_BTN_SELECT, (m_cusCopyServer & SSO_REQ_TRADE)? TRUE : FALSE);
				break;
			case OPTION_COMMUNITY_INVATE:
				(m_cusCopyServer & SSO_REQ_TEAM)?	m_cusCopyServer &= ~SSO_REQ_TEAM : m_cusCopyServer |= SSO_REQ_TEAM;
				m_pCommunityCheck[OPTION_COMMUNITY_INVATE]->SPSendMessage(SPIM_BTN_SELECT, (m_cusCopyServer & SSO_REQ_TEAM)? TRUE : FALSE);
				break;
			case OPTION_COMMUNITY_PARTY:
				(m_cusCopyServer & SSO_REQ_WANTTEAM)?	m_cusCopyServer &= ~SSO_REQ_WANTTEAM : m_cusCopyServer |= SSO_REQ_WANTTEAM;
				m_pCommunityCheck[OPTION_COMMUNITY_PARTY]->SPSendMessage(SPIM_BTN_SELECT, (m_cusCopyServer & SSO_REQ_WANTTEAM)? TRUE : FALSE);
				break;
			case OPTION_COMMUNITY_FRIEND_INVATE:
				(m_cusCopyServer & SSO_REQ_FRIEND)?	m_cusCopyServer &= ~SSO_REQ_FRIEND : m_cusCopyServer |= SSO_REQ_FRIEND;
				m_pCommunityCheck[OPTION_COMMUNITY_FRIEND_INVATE]->SPSendMessage(SPIM_BTN_SELECT, (m_cusCopyServer & SSO_REQ_FRIEND)? TRUE : FALSE);
				break;
			case OPTION_COMMUNITY_ALERT:
				(m_cusCopyServer & SSO_REQ_ALERT)?	m_cusCopyServer &= ~SSO_REQ_ALERT : m_cusCopyServer |= SSO_REQ_ALERT;
				m_pCommunityCheck[OPTION_COMMUNITY_ALERT]->SPSendMessage(SPIM_BTN_SELECT, (m_cusCopyServer & SSO_REQ_ALERT)? TRUE : FALSE);
				break;
			case OPTION_COMMUNITY_PARTY_CHAT:
				(m_cusCopyServer & SSO_REQ_CHATTEAM)?	m_cusCopyServer &= ~SSO_REQ_CHATTEAM : m_cusCopyServer |= SSO_REQ_CHATTEAM;
				m_pCommunityCheck[OPTION_COMMUNITY_PARTY_CHAT]->SPSendMessage(SPIM_BTN_SELECT, (m_cusCopyServer & SSO_REQ_CHATTEAM)? TRUE : FALSE);
				break;
			case OPTION_COMMUNITY_FRIEND_CHAT:
				(m_cusCopyServer & SSO_REQ_CHATFRIEND)?	m_cusCopyServer &= ~SSO_REQ_CHATFRIEND : m_cusCopyServer |= SSO_REQ_CHATFRIEND;
				m_pCommunityCheck[OPTION_COMMUNITY_FRIEND_CHAT]->SPSendMessage(SPIM_BTN_SELECT, (m_cusCopyServer & SSO_REQ_CHATFRIEND)? TRUE : FALSE);
				break;
			case OPTION_COMMUNITY_GUILD_CHAT:
				(m_cusCopyServer & SSO_REQ_CHATGUILD)?	m_cusCopyServer &= ~SSO_REQ_CHATGUILD : m_cusCopyServer |= SSO_REQ_CHATGUILD;
				m_pCommunityCheck[OPTION_COMMUNITY_GUILD_CHAT]->SPSendMessage(SPIM_BTN_SELECT, (m_cusCopyServer & SSO_REQ_CHATGUILD)? TRUE : FALSE);
				break;
			//case OPTION_COMMUNITY_PERMIT_FRIEND:
			//	(m_pCopyOption->bChatPermitFriend)?			m_pCopyOption->bChatPermitFriend = false : m_pCopyOption->bChatPermitFriend = true;
			//	m_pCommunityCheck[OPTION_COMMUNITY_PERMIT_FRIEND]->SPSendMessage(SPIM_BTN_SELECT,	m_pCopyOption->bChatPermitFriend);
			//	break;
			case OPTION_COMMUNITY_PERMIT_WHISPER:
				(m_cusCopyServer & SSO_REQ_WHISPER)?	m_cusCopyServer &= ~SSO_REQ_WHISPER : m_cusCopyServer |= SSO_REQ_WHISPER;
				m_pCommunityCheck[OPTION_COMMUNITY_PERMIT_WHISPER]->SPSendMessage(SPIM_BTN_SELECT, (m_cusCopyServer & SSO_REQ_WHISPER)? TRUE : FALSE);
				break;
			default:
				break;
		}
	}
	return 1;
}


int SPWindowOption::OnCheckBenefit(unsigned int iID, WPARAM wParam, LPARAM lparam)
{
	int iSelect = iID - WIID_OPTION_BEN_CHECK;
	DXUTOutputDebugString("	- CheckBenefit[%d] \n", iSelect);

	if(m_iOptionType == OPTION_TYPE_BENEFIT) {
		switch(iSelect) {
			case OPTION_BENEFIT_INDICATE_PLAYER:
				(m_pCopyOption->bIndicatePlayer)?					m_pCopyOption->bIndicatePlayer = false : m_pCopyOption->bIndicatePlayer = true;
				m_pBenefitCheck[OPTION_BENEFIT_INDICATE_PLAYER]->SPSendMessage(SPIM_BTN_SELECT,		m_pCopyOption->bIndicatePlayer);
				break;
			case OPTION_BENEFIT_INDICATE_MONSTER:
				(m_pCopyOption->bIndicateMonster)?					m_pCopyOption->bIndicateMonster = false : m_pCopyOption->bIndicateMonster = true;
				m_pBenefitCheck[OPTION_BENEFIT_INDICATE_MONSTER]->SPSendMessage(SPIM_BTN_SELECT,	m_pCopyOption->bIndicateMonster);
				break;
			case OPTION_BENEFIT_INDICATE_NPC:
				(m_pCopyOption->bIndicateNPC)?						m_pCopyOption->bIndicateNPC = false : m_pCopyOption->bIndicateNPC = true;
				m_pBenefitCheck[OPTION_BENEFIT_INDICATE_NPC]->SPSendMessage(SPIM_BTN_SELECT,		m_pCopyOption->bIndicateNPC);
				break;
			case OPTION_BENEFIT_INDICATE_PET:
				(m_pCopyOption->bIndicatePet)?						m_pCopyOption->bIndicatePet = false : m_pCopyOption->bIndicatePet = true;
				m_pBenefitCheck[OPTION_BENEFIT_INDICATE_PET]->SPSendMessage(SPIM_BTN_SELECT,		m_pCopyOption->bIndicatePet);
				break;
			case OPTION_BENEFIT_SPEECH_PLAYER:
				(m_pCopyOption->bSpeechPlayer)?						m_pCopyOption->bSpeechPlayer = false : m_pCopyOption->bSpeechPlayer = true;
				m_pBenefitCheck[OPTION_BENEFIT_SPEECH_PLAYER]->SPSendMessage(SPIM_BTN_SELECT,		m_pCopyOption->bSpeechPlayer);
				break;
			case OPTION_BENEFIT_SPEECH_TEAM:
				(m_pCopyOption->bSpeechTeam)?						m_pCopyOption->bSpeechTeam = false : m_pCopyOption->bSpeechTeam = true;
				m_pBenefitCheck[OPTION_BENEFIT_SPEECH_TEAM]->SPSendMessage(SPIM_BTN_SELECT,			m_pCopyOption->bSpeechTeam);
				break;
			case OPTION_BENEFIT_SPEECH_GUILD:
				(m_pCopyOption->bSpeechGuild)?						m_pCopyOption->bSpeechGuild = false : m_pCopyOption->bSpeechGuild = true;
				m_pBenefitCheck[OPTION_BENEFIT_SPEECH_GUILD]->SPSendMessage(SPIM_BTN_SELECT,		m_pCopyOption->bSpeechGuild);
				break;
			case OPTION_BENEFIT_SPEECH_NPC:
				(m_pCopyOption->bSpeechNPC)?						m_pCopyOption->bSpeechNPC = false : m_pCopyOption->bSpeechNPC = true;
				m_pBenefitCheck[OPTION_BENEFIT_SPEECH_NPC]->SPSendMessage(SPIM_BTN_SELECT,			m_pCopyOption->bSpeechNPC);
				break;
			case OPTION_BENEFIT_CURSOR:
				(m_pCopyOption->bAutoHideMouse)?					m_pCopyOption->bAutoHideMouse = false : m_pCopyOption->bAutoHideMouse = true;
				m_pBenefitCheck[OPTION_BENEFIT_CURSOR]->SPSendMessage(SPIM_BTN_SELECT,				m_pCopyOption->bAutoHideMouse);
				break;
			case OPTION_BENEFIT_HP_SELF:
				(m_pCopyOption->bHpBarSelf)?						m_pCopyOption->bHpBarSelf = false : m_pCopyOption->bHpBarSelf = true;
				m_pBenefitCheck[OPTION_BENEFIT_HP_SELF]->SPSendMessage(SPIM_BTN_SELECT,				m_pCopyOption->bHpBarSelf);
				break;
			case OPTION_BENEFIT_HP_PARTY:
				(m_pCopyOption->bHpBarParty)?						m_pCopyOption->bHpBarParty = false : m_pCopyOption->bHpBarParty = true;
				m_pBenefitCheck[OPTION_BENEFIT_HP_PARTY]->SPSendMessage(SPIM_BTN_SELECT,			m_pCopyOption->bHpBarParty);
				break;
			case OPTION_BENEFIT_HP_PLAYER:
				(m_pCopyOption->bHpBarPlayer)?						m_pCopyOption->bHpBarPlayer = false : m_pCopyOption->bHpBarPlayer = true;
				m_pBenefitCheck[OPTION_BENEFIT_HP_PLAYER]->SPSendMessage(SPIM_BTN_SELECT,			m_pCopyOption->bHpBarPlayer);
				break;
			case OPTION_BENEFIT_SCR_JPG:
				m_pCopyOption->iScreenShot = SCREEN_SHOT_JPG;
				UpdateScreenShotFormat(m_pCopyOption);
				break;
			case OPTION_BENEFIT_SCR_PNG:
				m_pCopyOption->iScreenShot = SCREEN_SHOT_PNG;
				UpdateScreenShotFormat(m_pCopyOption);
				break;
			case OPTION_BENEFIT_SCR_BMP:
				m_pCopyOption->iScreenShot = SCREEN_SHOT_BMP;
				UpdateScreenShotFormat(m_pCopyOption);
				break;			
			default:
				break;
		}
	}
	return 1;
}


int SPWindowOption::OnBGM				(WPARAM wParam, LPARAM lparam)	
{	
	if(m_bShow) {
		m_pCopyOption->iVolumeBGM = (int)wParam;
		DXUTOutputDebugString("	- BGM Volume [%d] \n", m_pCopyOption->iVolumeBGM);

		float fVar;
		Convert255To1( fVar, m_pCopyOption->iVolumeBGM );	
		SPSoundBase::GetInstance()->UpdateBGMVolume( fVar*2 );
		//暂时关闭
		if ( g_pCIGWLoaderDx9 )
		{// 通知音乐设置相关的变化
			g_pCIGWLoaderDx9->NodifyAudioChanged();
		}
	}
	
	return 1;
}


int SPWindowOption::OnSoundEffect		(WPARAM wParam, LPARAM lparam)	
{
	if(m_bShow) {
		m_pCopyOption->iVolumeSE = (int)wParam;	
		DXUTOutputDebugString("	- BGM Volume [%d] \n", m_pCopyOption->iVolumeSE);

		float fVar;
		Convert255To1( fVar, m_pCopyOption->iVolumeSE );	
		SPSoundBase::GetInstance()->UpdateEffectVolume( fVar );
		//暂时关闭
		if ( g_pCIGWLoaderDx9 )
		{// 通知音乐设置相关的变化
			g_pCIGWLoaderDx9->NodifyAudioChanged();
		}
	}	
	return 1;
}


int SPWindowOption::OnGammaRamp			(WPARAM wParam, LPARAM lparam)	
{
	if(m_bShow && g_bWindowed == false) {
		m_pCopyOption->iGammaRamp = (int)wParam;
		///DXUTOutputDebugString("	*** SPWindowOption::OnGammaRamp Current[%d] New\n", g_pVideo->GetContrast(), m_pCopyOption->iGammaRamp);
		g_pVideo->SetContrast( m_pCopyOption->iGammaRamp );
	}	
	return 1;
}


int SPWindowOption::OnEffect			(WPARAM wParam, LPARAM lparam)
{
	m_pCopyOption->iEffect = (int)wParam;	
	return 1;
}












