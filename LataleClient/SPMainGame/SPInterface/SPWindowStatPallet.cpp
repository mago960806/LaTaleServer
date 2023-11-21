
#include <vector>
#include <string>

#include "SPCommon.h"
#include "SPUtil.h"

#include "SPMainGameDEF.H"
#include "SPGameObjectDEF.h"

#include "SPSubGameManager.h"
#include "SPGameObject.h"
#include "SPManager.h"
#include "SPGOBManager.h"
#include "SPMouseCursor.h"
#include "SPIMEPool.h"

#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGOBModelUnit.h"
#include "SPAvatarModelUnit.h"

#include "SPStage.h"
#include "SPStageManager.h"
#include "SPTerrainAttribute.h"

#include "SPGOBStatus.h"
#include "SPPlayerStatus.h"
#include "SPPlayerStatusModel.h"
#include "SPGOBCoordPhysics.h"
#include "SPPlayer.h"

#include "SPWindowDEF.h"
#include "SPInterfaceManager.h"

#include "SPWindow.h"
#include "SPWindowButton.h"
#include "SPWindowStatic.h"
#include "SPWindowGauge.h"
#include "SPWindowDigitBoard.h"

#include "SPTitleAttr.h"
#include "SPTitleArchive.h"
#include "SPTitleRenderDEF.h"
#include "SPTitleRenderBase.h"
#include "SPTitleManager.h"

#include "SPComboBoxDEF.h"
#include "SPWindowComboBox.h"
#include "SPComboBoxItemBase.h"
#include "SPComboBoxItemTexture.h"
#include "SPComboBoxItemTitle.h"

#include "SPEventDEF.h"
#include "SPEventManager.h"
#include "SPMailManager.h"
#include "SPLocalizeManager.h"

#include "SPWindowStatPallet.h"

const char* UI_LEVEL = "DATA/INTERFACE/CONCEPT/UI100.PNG";

SPWindowStatPallet::SPWindowStatPallet(WND_ID WndClassID, INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WndClassID, InstanceID, iX, iY, iCX, iCY, pParent)	
{	
	Init();
}

SPWindowStatPallet::~SPWindowStatPallet()
{
	Clean();
}

void SPWindowStatPallet::Init()
{	
	m_RGBA = RGBA(0xFF,0xFF,0xFF,0xFF);
	m_strToolTip = "[SPWindowStatPallet]";	
	m_bHideEnable = true;

	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI101.PNG" , &m_pImageFrameHead);
	SetRect(&m_rcFrameHeadSrc, 300, 212, 497, 233);

	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI101.PNG" , &m_pImageFrameBody);
	SetRect(&m_rcFrameBodySrc, 300, 234, 497, 236);	

	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI101.PNG" , &m_pImageFrameTail);
	SetRect(&m_rcFrameTailSrc, 300, 237, 497, 240);

	ReposOwnImage();

	RECT rcTemp;
	SPWindowGauge* pGauge;
	SPWindowStatic* pStatic;
	SPWindowDigitBoard* pBoard;



	pStatic = new SPWindowStatic(WIID_SPALLET_JOB_ICON, 3, 2, 16, 16, this);

	const char*	pTextureName;
	RECT*		pTextureSrc;

	for( int i = 0 ; i < CLASS_MAX ; ++i )
	{
		pTextureName	=	g_pInterfaceManager->GetClassTextureName( (CLASS_TYPE)i );
		pTextureSrc		=	g_pInterfaceManager->GetClassTextureSrc( (CLASS_TYPE)i );

		if( pTextureName != NULL && pTextureSrc != NULL )
			pStatic->SetImage( pTextureName , pTextureSrc->left , pTextureSrc->top );
	}

	pStatic = new SPWindowStatic(WIID_SPALLET_LEVEL_TEXT, 20, 4, 46, 12, this);
	pStatic->SetFontColor( 0xFFFFFFFF );
	pStatic->SetFormat( DT_RIGHT );

	pStatic = new SPWindowStatic(WIID_SPALLET_ID, 71, 4, 124, 12, this);
	pStatic->SetFont( FONT_12_BOLD );
	pStatic->SetFontColor( 0xFFFFFFFF );
	pStatic->SetFormat( DT_LEFT );


	pGauge = new SPWindowGauge(WIID_SPALLET_GAUGE_HP	, 10, 24, 120, 11, this);
	pGauge->SetWindowType(WND_TYPE_LEFTBASE);
	SetRect(&rcTemp, 14, 48, 15, 57); pGauge->SetImageGauge( "DATA/INTERFACE/CONCEPT/UI100.PNG", rcTemp);
	SetRect(&rcTemp, 23, 48, 25, 57); pGauge->SetImageEdge( "DATA/INTERFACE/CONCEPT/UI100.PNG",  rcTemp);
	SetRect(&rcTemp, 18, 48, 22, 57); pGauge->SetImageDeco( "DATA/INTERFACE/CONCEPT/UI100.PNG",  rcTemp);
	pGauge->SetImageFrameHead("DATA/INTERFACE/CONCEPT/UI100.PNG", 1, 47, 3, 58);
	pGauge->SetImageFrameBody("DATA/INTERFACE/CONCEPT/UI100.PNG", 6, 47, 8, 58);
	pGauge->SetImageFrameTail("DATA/INTERFACE/CONCEPT/UI100.PNG", 10, 47, 12, 58);

	pGauge->SetImageFlickerGauge("DATA/INTERFACE/CONCEPT/UI100.PNG", 434, 41, 435, 50);
	pGauge->SetImageFlickerEdge("DATA/INTERFACE/CONCEPT/UI100.PNG", 443, 41, 445, 50);
	pGauge->SetImageFlickerDeco("DATA/INTERFACE/CONCEPT/UI100.PNG", 438, 41, 442, 50);
	pGauge->SetFlicker( 0.25f );

	pGauge->Show();
	pGauge = NULL;

	pGauge = new SPWindowGauge(WIID_SPALLET_GAUGE_SP	, 10, 39, 120, 11, this);
	pGauge->SetWindowType(WND_TYPE_LEFTBASE);
	SetRect(&rcTemp, 39, 48, 40, 57); pGauge->SetImageGauge( "DATA/INTERFACE/CONCEPT/UI100.PNG", rcTemp);
	SetRect(&rcTemp, 48, 48, 50, 57); pGauge->SetImageEdge( "DATA/INTERFACE/CONCEPT/UI100.PNG",  rcTemp);
	SetRect(&rcTemp, 43, 48, 47, 57); pGauge->SetImageDeco( "DATA/INTERFACE/CONCEPT/UI100.PNG",  rcTemp);
	pGauge->SetImageFrameHead("DATA/INTERFACE/CONCEPT/UI100.PNG", 26, 47, 28, 58);
	pGauge->SetImageFrameBody("DATA/INTERFACE/CONCEPT/UI100.PNG", 31, 47, 33, 58);
	pGauge->SetImageFrameTail("DATA/INTERFACE/CONCEPT/UI100.PNG", 35, 47, 37, 58);
	pGauge->Show();
	pGauge = NULL;

	pGauge = new SPWindowGauge(WIID_SPALLET_GAUGE_XP	, 10, 54, 120, 9, this);
	pGauge->SetWindowType(WND_TYPE_LEFTBASE);
	SetRect(&rcTemp, 64, 48, 65, 55);
	pGauge->SetImageGauge( "DATA/INTERFACE/CONCEPT/UI100.PNG", rcTemp);
	SetRect(&rcTemp, 73, 48, 75, 55);
	pGauge->SetImageEdge( "DATA/INTERFACE/CONCEPT/UI100.PNG",  rcTemp);
	SetRect(&rcTemp, 68, 48, 72, 55);
	pGauge->SetImageDeco( "DATA/INTERFACE/CONCEPT/UI100.PNG",  rcTemp);
	pGauge->SetImageFrameHead("DATA/INTERFACE/CONCEPT/UI100.PNG", 51, 47, 53, 56);
	pGauge->SetImageFrameBody("DATA/INTERFACE/CONCEPT/UI100.PNG", 56, 47, 58, 56);
	pGauge->SetImageFrameTail("DATA/INTERFACE/CONCEPT/UI100.PNG", 60, 47, 62, 56);
	pGauge->Show();
	pGauge = NULL;

	pStatic = new SPWindowStatic( WIID_BACKBOARD_HP, 14, 22, 15, 7, this);
	pStatic->SetWindowType(WND_TYPE_TOPBASE);
	pStatic->SetImageNormalBody("DATA/INTERFACE/CONCEPT/UI100.PNG",	76, 47, 91, 54);
	
	pStatic = new SPWindowStatic( WIID_BACKBOARD_SP, 14, 37, 15, 7, this);
	pStatic->SetWindowType(WND_TYPE_TOPBASE);
	pStatic->SetImageNormalBody("DATA/INTERFACE/CONCEPT/UI100.PNG",	92, 47, 107, 54);

	pStatic = new SPWindowStatic( WIID_BACKBOARD_XP, 14, 52, 15, 7, this);
	pStatic->SetWindowType(WND_TYPE_TOPBASE);
	pStatic->SetImageNormalBody("DATA/INTERFACE/CONCEPT/UI100.PNG",	108, 47, 123, 54);
	
	//RECT rcSrc;
	//pBoard = new SPWindowDigitBoard( WIID_SPALLET_LV_DIGIT , 34, 34, 30 ,7, this);
	//SetRect(&rcSrc, 0,39,7,46); pBoard->SetDigit(UI_LEVEL, '0',  rcSrc);
	//SetRect(&rcSrc, 8,39,15,46); pBoard->SetDigit(UI_LEVEL, '1',  rcSrc);
	//SetRect(&rcSrc, 16,39,23,46); pBoard->SetDigit(UI_LEVEL, '2',  rcSrc);
	//SetRect(&rcSrc, 24,39,31,46); pBoard->SetDigit(UI_LEVEL, '3',  rcSrc);
	//SetRect(&rcSrc, 32,39,39,46); pBoard->SetDigit(UI_LEVEL, '4',  rcSrc);
	//SetRect(&rcSrc, 40,39,47,46); pBoard->SetDigit(UI_LEVEL, '5',  rcSrc);
	//SetRect(&rcSrc, 48,39,55,46); pBoard->SetDigit(UI_LEVEL, '6',  rcSrc);
	//SetRect(&rcSrc, 56,39,63,46); pBoard->SetDigit(UI_LEVEL, '7',  rcSrc);
	//SetRect(&rcSrc, 64,39,71,46); pBoard->SetDigit(UI_LEVEL, '8',  rcSrc);
	//SetRect(&rcSrc, 72,39,79,46); pBoard->SetDigit(UI_LEVEL, '9',  rcSrc);
	//pBoard->SetFormat(DT_LEFT);

	pBoard = new SPWindowDigitBoard( WIID_SPALLET_HP_DIGIT , 134, 26, 53 ,7, this);
	pBoard->SetFormat(DT_LEFT);

	pBoard = new SPWindowDigitBoard( WIID_SPALLET_SP_DIGIT , 134, 41, 53 , 7, this);
	pBoard->SetFormat(DT_LEFT);

	pBoard = new SPWindowDigitBoard( WIID_SPALLET_XP_DIGIT , 134, 55, 53 , 7, this);
	pBoard->SetFormat(DT_LEFT);

	pStatic = new SPWindowStatic( WIID_ELY_BACKBOARD, 173, 71, 17, 5, this);
	pStatic->SetWindowType(WND_TYPE_TOPBASE);
	pStatic->SetImageNormalBody("DATA/INTERFACE/CONCEPT/UI103.PNG",	494, 506, 511, 511);

	pBoard = new SPWindowDigitBoard( WIID_SPALLET_ELY_DIGIT , 64, 69, 105 ,7, this);
	pBoard->SetFormat(DT_RIGHT);

	//WIID_SPALLET_LETTER
	pStatic = new SPWindowStatic (WIID_SPALLET_LETTER, 10, 67, 18, 13, this);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI100.PNG",	58,  498);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI100.PNG",	1,   498);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI100.PNG",	20,  498);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI100.PNG",	39,  498);
	
	//WIID_SPALLET_LETTER_NEW
	pStatic = new SPWindowStatic (WIID_SPALLET_LETTER_NEW, 9, 68, 8, 7, this);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI100.PNG",	10,  490);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI100.PNG",	1,  490);	
	
	//WIID_STATPALLET_CASH
	pBoard = new SPWindowDigitBoard( WIID_STATPALLET_CASH , 113, 82, 56, 7, this);
	pBoard->SetFormat(DT_RIGHT);
	pBoard->Hide();

	//WIID_STATPALLET_CASH_BACK
	pStatic = new SPWindowStatic(WIID_STATPALLET_CASH_BACK, 173, 81, 13, 9, this);
	pStatic->SetWindowType(WND_TYPE_TOPBASE);
	pStatic->SetImageNormalBody("DATA/INTERFACE/CONCEPT/UI100.PNG",	214, 190, 227, 199);
	pStatic->Hide();

	//WIID_STATPALLET_POINT
	pBoard = new SPWindowDigitBoard( WIID_STATPALLET_POINT , 113, 82, 56, 7, this);
	pBoard->SetFormat(DT_RIGHT);
	pBoard->Hide();

	//WIID_STATPALLET_POINT_BACK
	pStatic = new SPWindowStatic(WIID_STATPALLET_POINT_BACK, 173, 81, 13, 9, this);
	pStatic->SetWindowType(WND_TYPE_TOPBASE);
	pStatic->SetImageNormalBody("DATA/INTERFACE/CONCEPT/UI100.PNG",	228, 190, 241, 199);
	pStatic->Hide();

	ArrangeControl();
}

void SPWindowStatPallet::Clean()
{
	SPWindow::Clean();
}

void SPWindowStatPallet::Process(float fTime)
{
	SPWindow::Process(fTime);
}

void SPWindowStatPallet::Render(float fTime)
{
	if( !m_bShow )
		return;

	if(m_pImageFrameHead) 
		m_pImageFrameHead->RenderTexture(&m_rcCalcFrameHead, &m_rcFrameHeadSrc);

	if(m_pImageFrameBody) 
		m_pImageFrameBody->RenderTexture(&m_rcCalcFrameBody, &m_rcFrameBodySrc);		

	if(m_pImageFrameTail) 
		m_pImageFrameTail->RenderTexture(&m_rcCalcFrameTail, &m_rcFrameTailSrc);


	SPWindow::Render(fTime);

	////
	//#ifdef _DEBUG
	//	g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));	
	//	RECT rtRect;	
	//	SPWindow* pWindow = Find(WIID_SPALLET_LETTER, this);
	//	if(pWindow) {
	//		pWindow->GetWindowRect(rtRect);
	//		g_pVideo->GetSysLine()->DrawRect(rtRect);
	//	}		
	//	g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	//#endif
}

void SPWindowStatPallet::ReposOwnImage()
{
	if(SPLocalizeManager::GetInstance()->GetCashTradeDisplay() == CASH_TRADE_NULL)	{
		SetRect(&m_rcCalcFrameHead, m_iAX, m_iAY, m_iAX+197, m_iAY+21);
		SetRect(&m_rcCalcFrameBody, m_iAX, m_iAY+21, m_iAX+197, m_iAY+21 +60);
		SetRect(&m_rcCalcFrameTail, m_iAX, m_iAY+81, m_iAX+197, m_iAY+81 +3);

		////[2008/3/20] - jinhee
		//SetRect(&m_rcCalcFrameHead, m_iAX, m_iAY, m_iAX+197, m_iAY+21);
		//SetRect(&m_rcCalcFrameBody, m_iAX, m_iAY+21, m_iAX+197, m_iAY+21 +66);
		//SetRect(&m_rcCalcFrameTail, m_iAX, m_iAY+87, m_iAX+197, m_iAY+87 +3);
	}
	else {
		SetRect(&m_rcCalcFrameHead, m_iAX, m_iAY, m_iAX+197, m_iAY+21);
		SetRect(&m_rcCalcFrameBody, m_iAX, m_iAY+21, m_iAX+197, m_iAY+21 +72);
		SetRect(&m_rcCalcFrameTail, m_iAX, m_iAY+93, m_iAX+197, m_iAY+93 +3);
	}
}


void SPWindowStatPallet::ArrangeControl()
{
	SPWindow* pStatic = NULL;
	
	{
		//WIID_SPALLET_LETTER
		pStatic = NULL;
		pStatic = Find(WIID_SPALLET_LETTER, this);
		if(pStatic) {			
			pStatic->Hide();
		}	

		//WIID_SPALLET_LETTER_NEW
		pStatic = NULL;
		pStatic = Find(WIID_SPALLET_LETTER_NEW, this);
		if(pStatic) {			
			pStatic->Hide();
		}

		//WIID_STATPALLET_CASH
		pStatic = NULL;
		pStatic = Find(WIID_STATPALLET_CASH, this);
		if(pStatic) {			
			pStatic->Hide();
		}

		//WIID_STATPALLET_CASH_BACK
		pStatic = NULL;
		pStatic = Find(WIID_STATPALLET_CASH_BACK, this);
		if(pStatic) {			
			pStatic->Hide();
		}

		//WIID_STATPALLET_POINT
		pStatic = NULL;
		pStatic = Find(WIID_STATPALLET_POINT, this);
		if(pStatic) {			
			pStatic->Hide();
		}

		//WIID_STATPALLET_POINT_BACK
		pStatic = NULL;
		pStatic = Find(WIID_STATPALLET_POINT_BACK, this);
		if(pStatic) {			
			pStatic->Hide();
		}
	}
	if(SPLocalizeManager::GetInstance()->GetCashTradeDisplay() == CASH_TRADE_ALL)
	{
		//WIID_SPALLET_LETTER
		pStatic = NULL;
		pStatic = Find(WIID_SPALLET_LETTER, this);
		if(pStatic) {			
			pStatic->AdjustCoord(10, 79, 18, 13);
			pStatic->Show();
		}	

		//WIID_SPALLET_LETTER_NEW
		pStatic = NULL;
		pStatic = Find(WIID_SPALLET_LETTER_NEW, this);
		if(pStatic) {			
			pStatic->AdjustCoord(10, 80, 8, 7);
			pStatic->Show();
		}

		//WIID_STATPALLET_CASH
		pStatic = NULL;
		pStatic = Find(WIID_STATPALLET_CASH, this);
		if(pStatic) {			
			pStatic->AdjustCoord(36, 82, 56, 7);
			pStatic->Show();
		}

		//WIID_STATPALLET_CASH_BACK
		pStatic = NULL;
		pStatic = Find(WIID_STATPALLET_CASH_BACK, this);
		if(pStatic) {			
			pStatic->AdjustCoord(96, 81, 13, 9);
			pStatic->Show();
		}

		//WIID_STATPALLET_POINT
		pStatic = NULL;
		pStatic = Find(WIID_STATPALLET_POINT, this);
		if(pStatic) {			
			pStatic->AdjustCoord(113, 82, 56, 7);
			pStatic->Show();
		}

		//WIID_STATPALLET_POINT_BACK
		pStatic = NULL;
		pStatic = Find(WIID_STATPALLET_POINT_BACK, this);
		if(pStatic) {			
			pStatic->AdjustCoord(173, 81, 13, 9);
			pStatic->Show();
		}
	}	
	else if(SPLocalizeManager::GetInstance()->GetCashTradeDisplay() == CASH_TRADE_CASH) {
		//WIID_SPALLET_LETTER
		pStatic = NULL;
		pStatic = Find(WIID_SPALLET_LETTER, this);
		if(pStatic) {			
			pStatic->AdjustCoord(10, 79, 18, 13);
			pStatic->Show();
		}	

		//WIID_SPALLET_LETTER_NEW
		pStatic = NULL;
		pStatic = Find(WIID_SPALLET_LETTER_NEW, this);
		if(pStatic) {			
			pStatic->AdjustCoord(10, 80, 8, 7);
			pStatic->Show();
		}		

		//WIID_STATPALLET_CASH
		pStatic = NULL;
		pStatic = Find(WIID_STATPALLET_CASH, this);
		if(pStatic) {			
			pStatic->AdjustCoord(113, 82, 56, 7);
			pStatic->Show();
		}

		//WIID_STATPALLET_CASH_BACK
		pStatic = NULL;
		pStatic = Find(WIID_STATPALLET_CASH_BACK, this);
		if(pStatic) {			
			pStatic->AdjustCoord(173, 81, 13, 9);
			pStatic->Show();
		}

		//WIID_STATPALLET_POINT
		pStatic = NULL;
		pStatic = Find(WIID_STATPALLET_POINT, this);
		if(pStatic) {			
			pStatic->Hide();
		}

		//WIID_STATPALLET_POINT_BACK
		pStatic = NULL;
		pStatic = Find(WIID_STATPALLET_POINT_BACK, this);
		if(pStatic) {			
			pStatic->Hide();
		}
	}
	else if(SPLocalizeManager::GetInstance()->GetCashTradeDisplay() == CASH_TRADE_POINT) {
		//WIID_SPALLET_LETTER
		pStatic = NULL;
		pStatic = Find(WIID_SPALLET_LETTER, this);
		if(pStatic) {			
			pStatic->AdjustCoord(10, 79, 18, 13);
			pStatic->Show();
		}	

		//WIID_SPALLET_LETTER_NEW
		pStatic = NULL;
		pStatic = Find(WIID_SPALLET_LETTER_NEW, this);
		if(pStatic) {			
			pStatic->AdjustCoord(10, 80, 8, 7);
			pStatic->Show();
		}

		//WIID_STATPALLET_CASH
		pStatic = NULL;
		pStatic = Find(WIID_STATPALLET_CASH, this);
		if(pStatic) {
			pStatic->Hide();
		}

		//WIID_STATPALLET_CASH_BACK
		pStatic = NULL;
		pStatic = Find(WIID_STATPALLET_CASH_BACK, this);
		if(pStatic) {
			pStatic->Hide();
		}

		//WIID_STATPALLET_POINT
		pStatic = NULL;
		pStatic = Find(WIID_STATPALLET_POINT, this);
		if(pStatic) {
			pStatic->AdjustCoord(113, 82, 56, 7);
			pStatic->Show();
		}

		//WIID_STATPALLET_POINT_BACK
		pStatic = NULL;
		pStatic = Find(WIID_STATPALLET_POINT_BACK, this);
		if(pStatic) {
			pStatic->AdjustCoord(173, 81, 13, 9);
			pStatic->Show();
		}
	}
	else {
		//WIID_SPALLET_LETTER
		pStatic = NULL;
		pStatic = Find(WIID_SPALLET_LETTER, this);
		if(pStatic) {			
			pStatic->AdjustCoord(10, 67, 18, 13);
			pStatic->Show();
		}	

		//WIID_SPALLET_LETTER_NEW
		pStatic = NULL;
		pStatic = Find(WIID_SPALLET_LETTER_NEW, this);
		if(pStatic) {			
			pStatic->AdjustCoord(9, 68, 8, 7);
			pStatic->Show();
		}
	}
}


void SPWindowStatPallet::Show(bool bWithChild /*= true*/)
{
	SPWindow::Show(bWithChild);
	ReposOwnImage();
	ArrangeControl();
}


void SPWindowStatPallet::Refresh(int iRefreshCate)
{
	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return; 

	SPGOBStatus* pPlayerStatus	= g_pGOBManager->GetLocalPlayer()->GetStatus();

	char szTempData[255];
	std::string strTemp;
	int iValue, iValue2;
	SPWindow* pWindow;	

	
	//1	공격형
	//2	방어형
	//3	마법형
	//4	탐색형
	UINT uiClassType = pPlayerStatus->GetStatusValue(STATUS_CLASS_TYPE);
	pWindow = Find(WIID_SPALLET_JOB_ICON	,false );
	if( pWindow ) 		
		pWindow->SPSendMessage(SPIM_SET_IMAGEINDEX, uiClassType);

	pWindow = Find(WIID_SPALLET_LEVEL_TEXT		,false );
	iValue = pPlayerStatus->GetStatusValue(STATUS_LV); 
	sprintf(szTempData, "Lv %d.", iValue);
	if(pWindow) pWindow->SetWindowText(szTempData);	


	pWindow = Find(WIID_SPALLET_ID, false );
	if(pWindow) pWindow->SetWindowText( pPlayerStatus->GetGOBName() );


	pWindow = Find(WIID_SPALLET_GAUGE_XP	);
	if(pWindow) {
		pWindow->SPSendMessage(SPIM_SET_MAXVALUE	,	
			pPlayerStatus->GetCHAR_EXP()->uiMaxExp - pPlayerStatus->GetCHAR_EXP()->uiMinExp);
		pWindow->SPSendMessage(SPIM_SET_CURVALUE	,	
			pPlayerStatus->GetCHAR_EXP()->uiCurExp - pPlayerStatus->GetCHAR_EXP()->uiMinExp);
	}

	pWindow = Find(WIID_SPALLET_GAUGE_HP	);
	if(pWindow) {
		pWindow->SPSendMessage(SPIM_SET_CURVALUE,	pPlayerStatus->GetStatusValue(STATUS_HP)	);
		pWindow->SPSendMessage(SPIM_SET_MAXVALUE,	pPlayerStatus->GetStatusValue(STATUS_MHP)	);
	}
	pWindow = Find(WIID_SPALLET_GAUGE_SP	);
	if(pWindow) {
		pWindow->SPSendMessage(SPIM_SET_CURVALUE,	pPlayerStatus->GetStatusValue(STATUS_SP)	);
		pWindow->SPSendMessage(SPIM_SET_MAXVALUE,	pPlayerStatus->GetStatusValue(STATUS_MSP)	);	
	}	

	
	//pWindow = Find(WIID_SPALLET_LV_DIGIT);
	//if(pWindow) {
	//	iValue = pPlayerStatus->GetStatusValue(STATUS_LV); 					
	//	strTemp.clear();
	//	sprintf(szTempData, "%d", iValue);
	//	strTemp += szTempData;
	//	pWindow->SetWindowText( strTemp.c_str());
	//}

	pWindow = Find(WIID_SPALLET_HP_DIGIT);
	if(pWindow) {
		iValue = pPlayerStatus->GetStatusValue(STATUS_HP); 			
		iValue2 = pPlayerStatus->GetStatusValue(STATUS_MHP);			
		strTemp.clear();
		sprintf(szTempData, "%d/", iValue);
		strTemp = szTempData;
		sprintf(szTempData, "%d", iValue2);
		strTemp += szTempData;
		pWindow->SetWindowText( strTemp.c_str());
	}

	pWindow = Find(WIID_SPALLET_SP_DIGIT);
	if(pWindow) {
		iValue = pPlayerStatus->GetStatusValue(STATUS_SP); 			
		iValue2 = pPlayerStatus->GetStatusValue(STATUS_MSP);			
		strTemp.clear();
		sprintf(szTempData, "%d/", iValue);
		strTemp = szTempData;
		sprintf(szTempData, "%d", iValue2);
		strTemp += szTempData;
		pWindow->SetWindowText( strTemp.c_str());
	}

	pWindow = Find(WIID_SPALLET_XP_DIGIT);
	if(pWindow) {
		float fPercent;
		if((pPlayerStatus->GetCHAR_EXP()->uiCurExp - pPlayerStatus->GetCHAR_EXP()->uiMinExp) == 0) {		
			fPercent = 0.0f;
		} else if( (pPlayerStatus->GetCHAR_EXP()->uiMaxExp - pPlayerStatus->GetCHAR_EXP()->uiMinExp) == 
					(pPlayerStatus->GetCHAR_EXP()->uiCurExp - pPlayerStatus->GetCHAR_EXP()->uiMinExp)) {
			fPercent = 1.0f;
		} else {
			fPercent = 
				((float)(pPlayerStatus->GetCHAR_EXP()->uiCurExp - pPlayerStatus->GetCHAR_EXP()->uiMinExp) / 
				(pPlayerStatus->GetCHAR_EXP()->uiMaxExp - pPlayerStatus->GetCHAR_EXP()->uiMinExp) * 100.0f);	
		}
		sprintf(szTempData, "%3.2f%%", fPercent);
		pWindow->SetWindowText( szTempData );
		
	}

	pWindow = Find(WIID_SPALLET_ELY_DIGIT	);
	if(pWindow) {	
		pWindow->SetWindowText(ConvertMoneyToString(pPlayerStatus->GetStatusValueEX(STATUS_EX_ELY))); //[2006/4/11]
	}


	pWindow = Find(WIID_SPALLET_LETTER	,false );
	if( pWindow )  {
		MAIL_VOLUME mailVolume = g_pEventManager->GetMailManager()->GetMailVolume();
		int iIndex = 0;
		switch(mailVolume) {
		default:
		case MAIL_VOLUME_NULL :			iIndex = 0; break;
		case MAIL_VOLUME_SMALL :		iIndex = 1; break;
		case MAIL_VOLUME_MIDDLE:		iIndex = 2; break;
		case MAIL_VOLUME_LARGE :		iIndex = 2; break;
		case MAIL_VOLUME_MAX	:		iIndex = 3; break;			
		}
		pWindow->SPSendMessage(SPIM_SET_IMAGEINDEX, iIndex);
	}

	pWindow = Find(WIID_SPALLET_LETTER_NEW	,false );
	if( pWindow ) {
		if(g_pEventManager->GetMailManager()->IsNewMail()) {
			pWindow->SPSendMessage(SPIM_SET_IMAGEINDEX, 1);
		} else {
			pWindow->SPSendMessage(SPIM_SET_IMAGEINDEX, 0);
		}
	}	

	pWindow = Find(WIID_STATPALLET_CASH, true);
	if(pWindow) {
		pWindow->SetWindowText(ConvertMoneyToString(pPlayerStatus->GetStatusValue(STATUS_CASH)));
	}

	pWindow = Find(WIID_STATPALLET_POINT, true);
	if(pWindow) {
		pWindow->SetWindowText(ConvertMoneyToString(pPlayerStatus->GetStatusValue(STATUS_MILE)));
	}
}

SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowStatPallet )
SPIMESSAGE_COMMAND(	SPIM_REFRESH,			OnRefresh		)
SPIMESSAGE_OBJ_MAP_END	( SPWindow	)


int SPWindowStatPallet::OnRefresh	( WPARAM wParam, LPARAM lParam)
{		
	Refresh(0);
	return 1;
}

