#include <vector>

#include "SPCommon.h"
#include "SPUtil.h"

#include "SPGameObject.h"
#include "SPGOBManager.h"

#include "SPGameObjectDEF.h"

#include "SPGameObject.h"
#include "SPManager.h"
#include "SPMouseCursor.h"
#include "SPIMEPool.h"
#include "SPSubGameManager.h"

#include "SPWindowDEF.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPWindowEdit.h"
#include "SPWindowPcTrade.h"
#include "SPWindowPcTradeResult.h"

#include "SPStage.h"
#include "SPStageManager.h"
#include "SPTerrainAttribute.h"

#include "SPCommandConvert.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGOBCoordPhysics.h"
#include "SPPlayerCoordPhysics.h"
#include "SPPlayerInvenArchive.h"

#include "SPGOBStatus.h"
#include "SPPlayerStatus.h"
#include "SPPlayerStatusModel.h"

#include "Packet.h"
#include "PacketID.h"
#include "SPNetworkManager.h"

#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPItem.h"
#include "SPItemCluster.h"			//[2005/6/16] - jinhee
#include "SPPlayer.h"

#include "SPCheckManager.h"
#include "SPLocalizeManager.h"

#include "SPDebug.h"




SPWindowPcTradeResult::SPWindowPcTradeResult( INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent /*= NULL*/)
: SPWindow(WNDID_PC_TRADE_RESULT, InstanceID, iX, iY, iCX, iCY, pParent)
{
	Init();
}


SPWindowPcTradeResult::~SPWindowPcTradeResult()
{
	Clean();
}


void SPWindowPcTradeResult::Init()
{
	for(int i = 0; i < MAX_PC_TRADE_COUNT ; i++) {		
		m_bItemAble[i] = true;
		m_iItemMark[i] = ICON_MARK_NULL;
		m_bPcItemAble[i] = true;
		m_iPcItemMark[i] = ICON_MARK_NULL;
		m_iStack[i] = 0;
		m_iPcStack[i] = 0;
	}

	char* IMAGE_FILE0 = "DATA/INTERFACE/CONCEPT/UI100.PNG";
	g_pVideo->GetTextureMgr()->LoadTexture(IMAGE_FILE0, &m_pDisable);

	SetRect(&m_rtSrcDisable, 434 , 37 , 436 , 39);
	SetRect(&m_rtMarkSrc[0], 479, 152, 511, 184);		//ICON_MARK_LOCK
	SetRect(&m_rtMarkSrc[1], 479, 185, 511, 217);		//ICON_MARK_FASHION
	SetRect(&m_rtMarkSrc[2], 479, 339, 511, 371);		//ICON_MARK_EVENT
	SetRect(&m_rtMarkSrc[3], 479, 372, 511, 404);		//ICON_MARK_FANCY
	
	g_pVideo->GetTextureMgr()->LoadTexture("DATA/INTERFACE/CONCEPT/UI_WinBG_000.PNG", &m_pBaseSkin);
	SetRect(&m_rtBaseSrc[WINDOW_BASE_TOP_LEFT],			467, 497, 471, 501);
	SetRect(&m_rtBaseSrc[WINDOW_BASE_TOP],				472, 497, 476, 501);
	SetRect(&m_rtBaseSrc[WINDOW_BASE_TOP_RIGHT],		477, 497, 481, 501);
	SetRect(&m_rtBaseSrc[WINDOW_BASE_CENTER_LEFT],		467, 502, 471, 506);
	SetRect(&m_rtBaseSrc[WINDOW_BASE_CENTER],			472, 502, 476, 506);
	SetRect(&m_rtBaseSrc[WINDOW_BASE_CENTER_RIGHT],		477, 502, 481, 506);
	SetRect(&m_rtBaseSrc[WINDOW_BASE_BOTTOM_LEFT],		467, 507, 471, 511);
	SetRect(&m_rtBaseSrc[WINDOW_BASE_BOTTOM],			472, 507, 476, 511);
	SetRect(&m_rtBaseSrc[WINDOW_BASE_BOTTOM_RIGHT],		477, 507, 481, 511);
	
	SetWindowBase();

	InitSubControl();

	m_fWaitDeley	= 60.0f * 3.0f;
	m_fWaitAccumulate = 0.0f;
}


void SPWindowPcTradeResult::Clean()
{
	SAFE_RELEASE(m_pDisable);
	SAFE_RELEASE(m_pBaseSkin);
	SPWindow::Clean();
}


void SPWindowPcTradeResult::SetWindowBase()
{
	//
	if(SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_ALL) {
		SetRect(&m_rtBaseDest[WINDOW_BASE_TOP_LEFT],		m_iAX, m_iAY, m_iAX + 4, m_iAY + 4);
		SetRect(&m_rtBaseDest[WINDOW_BASE_TOP],				m_iAX + 4, m_iAY, m_iAX + 4 + 455, m_iAY + 4);	
		SetRect(&m_rtBaseDest[WINDOW_BASE_TOP_RIGHT],		m_iAX + 459, m_iAY, m_iAX + 459 + 4, m_iAY + 4);
		SetRect(&m_rtBaseDest[WINDOW_BASE_CENTER_LEFT],		m_iAX, m_iAY + 4, m_iAX + 4, m_iAY + 4 + 300);
		SetRect(&m_rtBaseDest[WINDOW_BASE_CENTER],			m_iAX + 4, m_iAY + 4, m_iAX + 4 + 455, m_iAY + 4 + 300);
		SetRect(&m_rtBaseDest[WINDOW_BASE_CENTER_RIGHT],	m_iAX + 459, m_iAY + 4, m_iAX + 459 + 4, m_iAY + 4 + 300);
		SetRect(&m_rtBaseDest[WINDOW_BASE_BOTTOM_LEFT],		m_iAX, m_iAY + 304, m_iAX + 4, m_iAY + 304 + 4);
		SetRect(&m_rtBaseDest[WINDOW_BASE_BOTTOM],			m_iAX + 4, m_iAY + 304, m_iAX + 4 + 455, m_iAY + 304 + 4);
		SetRect(&m_rtBaseDest[WINDOW_BASE_BOTTOM_RIGHT],	m_iAX + 459, m_iAY + 304, m_iAX + 459 + 4, m_iAY + 304 + 4);
	}
	else if(SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_CASH || 
		SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_POINT)
	{
		SetRect(&m_rtBaseDest[WINDOW_BASE_TOP_LEFT],		m_iAX, m_iAY, m_iAX + 4, m_iAY + 4);
		SetRect(&m_rtBaseDest[WINDOW_BASE_TOP],				m_iAX + 4, m_iAY, m_iAX + 4 + 455, m_iAY + 4);	
		SetRect(&m_rtBaseDest[WINDOW_BASE_TOP_RIGHT],		m_iAX + 459, m_iAY, m_iAX + 459 + 4, m_iAY + 4);
		SetRect(&m_rtBaseDest[WINDOW_BASE_CENTER_LEFT],		m_iAX, m_iAY + 4, m_iAX + 4, m_iAY + 4 + 282);
		SetRect(&m_rtBaseDest[WINDOW_BASE_CENTER],			m_iAX + 4, m_iAY + 4, m_iAX + 4 + 455, m_iAY + 4 + 282);
		SetRect(&m_rtBaseDest[WINDOW_BASE_CENTER_RIGHT],	m_iAX + 459, m_iAY + 4, m_iAX + 459 + 4, m_iAY + 4 + 282);
		SetRect(&m_rtBaseDest[WINDOW_BASE_BOTTOM_LEFT],		m_iAX, m_iAY + 286, m_iAX + 4, m_iAY + 286 + 4);
		SetRect(&m_rtBaseDest[WINDOW_BASE_BOTTOM],			m_iAX + 4, m_iAY + 286, m_iAX + 4 + 455, m_iAY + 286 + 4);
		SetRect(&m_rtBaseDest[WINDOW_BASE_BOTTOM_RIGHT],	m_iAX + 459, m_iAY + 286, m_iAX + 459 + 4, m_iAY + 286 + 4);
	}
	else {
		SetRect(&m_rtBaseDest[WINDOW_BASE_TOP_LEFT],		m_iAX, m_iAY, m_iAX + 4, m_iAY + 4);
		SetRect(&m_rtBaseDest[WINDOW_BASE_TOP],				m_iAX + 4, m_iAY, m_iAX + 4 + 455, m_iAY + 4);	
		SetRect(&m_rtBaseDest[WINDOW_BASE_TOP_RIGHT],		m_iAX + 459, m_iAY, m_iAX + 459 + 4, m_iAY + 4);
		SetRect(&m_rtBaseDest[WINDOW_BASE_CENTER_LEFT],		m_iAX, m_iAY + 4, m_iAX + 4, m_iAY + 4 + 246);
		SetRect(&m_rtBaseDest[WINDOW_BASE_CENTER],			m_iAX + 4, m_iAY + 4, m_iAX + 4 + 455, m_iAY + 4 + 246);
		SetRect(&m_rtBaseDest[WINDOW_BASE_CENTER_RIGHT],	m_iAX + 459, m_iAY + 4, m_iAX + 459 + 4, m_iAY + 4 + 246);
		SetRect(&m_rtBaseDest[WINDOW_BASE_BOTTOM_LEFT],		m_iAX, m_iAY + 250, m_iAX + 4, m_iAY + 250 + 4);
		SetRect(&m_rtBaseDest[WINDOW_BASE_BOTTOM],			m_iAX + 4, m_iAY + 250, m_iAX + 4 + 455, m_iAY + 250 + 4);
		SetRect(&m_rtBaseDest[WINDOW_BASE_BOTTOM_RIGHT],	m_iAX + 459, m_iAY + 250, m_iAX + 459 + 4, m_iAY + 250 + 4);
	}
}


void SPWindowPcTradeResult::InitSubControl()
{		
	char* OBJECT_IMAGE_FILE0	= "DATA/INTERFACE/CONCEPT/UI_Object_000.PNG";
	char* OBJECT_IMAGE_FILE3	= "DATA/INTERFACE/CONCEPT/UI_Object_003.PNG";
	char* BUTTON_IMAGE_FILE0	= "DATA/INTERFACE/CONCEPT/UI_Button_000.PNG";
	
	//// Control
	SPWindowButton* pButton = NULL;
	SPWindowStatic* pStatic = NULL;
	//SPWindowStatic* pCenter = NULL;
	SPWindowStatic* pParent = NULL;
	
	////WIID_PC_TRADE_RESULT_TOP	
	//pStatic = new SPWindowStatic( WIID_PC_TRADE_RESULT_TOP,	0,	0,	4,	264, this);	
	//pStatic->SetImage(IMAGE_FILE5, 497, 247);
	//pStatic->Show();
	//pStatic = NULL;

	////WIID_PC_TRADE_RESULT_CENTER	
	//pCenter = new SPWindowStatic( WIID_PC_TRADE_RESULT_CENTER,	4,	0,	455,	264, this);	
	//pCenter->SetImage(IMAGE_FILE5, 503, 247);
	//pCenter->SetSrcSize(2, 264);
	//pCenter->Show();

	////WIID_PC_TRADE_RESULT_BOTTOM
	//pStatic = new SPWindowStatic( WIID_PC_TRADE_RESULT_BOTTOM,	459,	0,	4,	264, this);	
	//pStatic->SetImage(IMAGE_FILE5, 507, 247);
	//pStatic->Show();
	//pStatic = NULL;

	//WIID_PC_TRADE_RESULT_PC_TRADE_IMAGE
	pStatic = new SPWindowStatic( WIID_PC_TRADE_RESULT_PC_TRADE_IMAGE,	10,	10,	17,	13, this);
	pStatic->SetImage(OBJECT_IMAGE_FILE0, 277, 101);
	pStatic->Show();
	pStatic = NULL;

	//WIID_PC_TRADE_RESULT_PC_NAME
	m_pPcName = new SPWindowStatic( WIID_PC_TRADE_RESULT_PC_NAME,	33,	11,	185,	12, this);
	m_pPcName->SetFormat(DT_LEFT | DT_VCENTER);
	m_pPcName->SetFont(FONT_12_BOLD);
	m_pPcName->SetFontColor(RGBA(2, 47, 117, 255));
	m_pPcName->Show();	

	//WIID_PC_TRADE_RESULT_TRADE_IMAGE
	pStatic = new SPWindowStatic( WIID_PC_TRADE_RESULT_TRADE_IMAGE,	264,	10,	17,	13, this);	
	pStatic->SetImage(OBJECT_IMAGE_FILE0, 277, 101);
	pStatic->Show();
	pStatic = NULL;
	
	//WIID_PC_TRADE_RESULT_NAME
	pStatic = new SPWindowStatic( WIID_PC_TRADE_RESULT_NAME,	288,	9,	129,	15, this);
	pStatic->SetImage(OBJECT_IMAGE_FILE0, 91, 433);
	pStatic->Show();
	pStatic = NULL;		
	
	int iOffSet = 36;
	for(int i=0; i<MAX_PC_TRADE_COUNT; i++)
	{
		// PC_ToolTip & 아이템 올리기 이벤트
		pButton = new SPWindowButton(WIID_PC_TRADE_RESULT_PC_ITEM_BTN+i, 16, 31+(iOffSet*i), 187, 32, this);
		pButton->Show();

		// PC_Item_IconLine
		pStatic = new SPWindowStatic(WIID_PC_TRADE_RESULT_PC_ITEM_ICONLINE+i, 14, 56+(iOffSet*i), 35, 11, this);
		pStatic->SetImage(OBJECT_IMAGE_FILE0, 440, 97);
		pStatic->Show();
		
		// PC Item Icon
		m_pPcItemIcon[i] = new SPWindowStatic(WIID_PC_TRADE_RESULT_PC_ITEM_ICON+i, 16, 31+(iOffSet*i), 32, 32, this);
		m_pPcItemIcon[i]->Show();

		// PC Item Name
		m_pPcItemName[i] = new SPWindowStatic(WIID_PC_TRADE_RESULT_PC_ITEM_NAME+i, 56, 41+(iOffSet*i), 151, 12, this);
		m_pPcItemName[i]->SetWindowText("11111");
		m_pPcItemName[i]->Show();
		
		// Local PC Item BTN
		pButton = new SPWindowButton(WIID_PC_TRADE_RESULT_ITEM_BTN+i, 266, 31+(iOffSet*i), 187, 32, this);
		pButton->Show();

		// Local  PC_Item_IconLine
		pStatic = new SPWindowStatic(WIID_PC_TRADE_RESULT_ITEM_ICONLINE+i, 264, 56+(iOffSet*i), 35, 11, this);
		pStatic->SetImage(OBJECT_IMAGE_FILE0, 440, 97);
		pStatic->Show();

		// Local PC Item Icon
		m_pItemIcon[i] = new SPWindowStatic(WIID_PC_TRADE_RESULT_ITEM_ICON+i, 266, 31+(iOffSet*i), 32, 32, this);
		m_pItemIcon[i]->Show();

		// Local PC Item Name
		m_pItemName[i] = new SPWindowStatic(WIID_PC_TRADE_RESULT_ITEM_NAME+i, 306, 41+(iOffSet*i), 151, 12, this);
		m_pItemName[i]->SetWindowText("22222");
		m_pItemName[i]->Show();
	}

	//WIID_PC_TRADE_RESULT_ARROW1
	pStatic = new SPWindowStatic( WIID_PC_TRADE_RESULT_ARROW1,	211,	44,	41,	56, this);	
	pStatic->SetImage(OBJECT_IMAGE_FILE0, 338, 300);
	pStatic->Show();
	pStatic = NULL;

	//WIID_PC_TRADE_RESULT_ARROW2
	pStatic = new SPWindowStatic( WIID_PC_TRADE_RESULT_ARROW2,	211,	108,	41,	56, this);	
	pStatic->SetImage(OBJECT_IMAGE_FILE0, 338, 357);
	pStatic->Show();
	pStatic = NULL;

	//WIID_PC_TRADE_RESULT_PC_ELY_BACK
	pStatic = new SPWindowStatic(WIID_PC_TRADE_RESULT_PC_ELY_BACK, 42, 179, 129, 15, this);
	pStatic->SetWindowType( WND_TYPE_LEFTBASE );
	pStatic->SetImageNormalHead(OBJECT_IMAGE_FILE0, 380, 268, 384, 283);
	pStatic->SetImageNormalBody(OBJECT_IMAGE_FILE0, 384, 268, 388, 283);
	pStatic->SetImageNormalTail(OBJECT_IMAGE_FILE0, 388, 268, 392, 283);
	pStatic->Show();

	//WIID_PC_TRADE_RESULT_PC_ELY	
	m_pPcEly = new SPWindowStatic( WIID_PC_TRADE_RESULT_PC_ELY,	46,	180,	121,	14, this);	
	m_pPcEly->SetFormat(DT_RIGHT | DT_VCENTER);
	m_pPcEly->SetFont(FONT_12_NORMAL);
	m_pPcEly->Show();

	//WIID_PC_TRADE_RESULT_PC_ELY_IMAGE	
	pStatic = new SPWindowStatic( WIID_PC_TRADE_RESULT_PC_ELY_IMAGE,	178,	183,	17,	5, this);
	pStatic->SetImage(OBJECT_IMAGE_FILE0, 494, 111);
	pStatic->Show();
	pStatic = NULL;	

	//WIID_PC_TRADE_RESULT_ELY_BACK
	pStatic = new SPWindowStatic(WIID_PC_TRADE_RESULT_ELY_BACK, 292, 179, 129, 15, this);
	pStatic->SetWindowType( WND_TYPE_LEFTBASE );
	pStatic->SetImageNormalHead(OBJECT_IMAGE_FILE0, 380, 268, 384, 283);
	pStatic->SetImageNormalBody(OBJECT_IMAGE_FILE0, 384, 268, 388, 283);
	pStatic->SetImageNormalTail(OBJECT_IMAGE_FILE0, 388, 268, 392, 283);
	pStatic->Show();

	//WIID_PC_TRADE_RESULT_ELY
	m_pEly = new SPWindowStatic( WIID_PC_TRADE_RESULT_ELY,	296,	180,	121,	14, this);		
	m_pEly->SetFormat(DT_RIGHT | DT_VCENTER);
	m_pEly->SetFont(FONT_12_NORMAL);
	m_pEly->Show();

	//WIID_PC_TRADE_RESULT_ELY_IMAGE
	pStatic = new SPWindowStatic( WIID_PC_TRADE_RESULT_ELY_IMAGE,	428,	183,	17,	5, this);
	pStatic->SetImage(OBJECT_IMAGE_FILE0, 494, 111);	
	pStatic->Show();
	pStatic = NULL;

	//WIID_PC_TRADE_RESULT_PC_CASH_BACK
	pStatic = new SPWindowStatic(WIID_PC_TRADE_RESULT_PC_CASH_BACK, 42, 196, 129, 15, this);
	pStatic->SetWindowType( WND_TYPE_LEFTBASE );
	pStatic->SetImageNormalHead(OBJECT_IMAGE_FILE0, 380, 268, 384, 283);
	pStatic->SetImageNormalBody(OBJECT_IMAGE_FILE0, 384, 268, 388, 283);
	pStatic->SetImageNormalTail(OBJECT_IMAGE_FILE0, 388, 268, 392, 283);
	pStatic->Show();

	//WIID_PC_TRADE_RESULT_PC_CASH
	m_pPcCash = new SPWindowStatic( WIID_PC_TRADE_RESULT_PC_CASH,	296,	180,	121,	14, this);		
	m_pPcCash->SetFormat(DT_RIGHT | DT_VCENTER);
	m_pPcCash->SetFont(FONT_12_NORMAL);
	m_pPcCash->Show();
	
	//WIID_PC_TRADE_RESULT_PC_CASH_IMAGE
	pStatic = new SPWindowStatic( WIID_PC_TRADE_RESULT_PC_CASH_IMAGE,	428,	183,	15,	11, this);
	pStatic->SetImage(OBJECT_IMAGE_FILE0, 476, 97);	
	pStatic->Show();
	pStatic = NULL;

	//WIID_PC_TRADE_RESULT_CASH_BACK
	pStatic = new SPWindowStatic(WIID_PC_TRADE_RESULT_CASH_BACK, 42, 196, 129, 15, this);
	pStatic->SetWindowType( WND_TYPE_LEFTBASE );
	pStatic->SetImageNormalHead(OBJECT_IMAGE_FILE0, 380, 268, 384, 283);
	pStatic->SetImageNormalBody(OBJECT_IMAGE_FILE0, 384, 268, 388, 283);
	pStatic->SetImageNormalTail(OBJECT_IMAGE_FILE0, 388, 268, 392, 283);
	pStatic->Show();
	
	//WIID_PC_TRADE_RESULT_CASH
	m_pCash = new SPWindowStatic( WIID_PC_TRADE_RESULT_PC_CASH,	296,	180,	121,	14, this);		
	m_pCash->SetFormat(DT_RIGHT | DT_VCENTER);
	m_pCash->SetFont(FONT_12_NORMAL);
	m_pCash->Show();

	//WIID_PC_TRADE_RESULT_CASH_IMAGE
	pStatic = new SPWindowStatic( WIID_PC_TRADE_RESULT_CASH_IMAGE,	428,	183,	15,	11, this);
	pStatic->SetImage(OBJECT_IMAGE_FILE0, 476, 97);	
	pStatic->Show();
	pStatic = NULL;

	//WIID_PC_TRADE_RESULT_PC_POINT_BACK
	pStatic = new SPWindowStatic(WIID_PC_TRADE_RESULT_PC_POINT_BACK, 42, 196, 129, 15, this);
	pStatic->SetWindowType( WND_TYPE_LEFTBASE );
	pStatic->SetImageNormalHead(OBJECT_IMAGE_FILE0, 380, 268, 384, 283);
	pStatic->SetImageNormalBody(OBJECT_IMAGE_FILE0, 384, 268, 388, 283);
	pStatic->SetImageNormalTail(OBJECT_IMAGE_FILE0, 388, 268, 392, 283);
	pStatic->Show();

	//WIID_PC_TRADE_RESULT_PC_POINT
	m_pPcPoint = new SPWindowStatic( WIID_PC_TRADE_RESULT_PC_POINT,	46,	180,	121,	14, this);		
	m_pPcPoint->SetFormat(DT_RIGHT | DT_VCENTER);
	m_pPcPoint->SetFont(FONT_14_BOLD);
	m_pPcPoint->Show();
	
	//WIID_PC_TRADE_RESULT_PC_POINT_IMAGE
	pStatic = new SPWindowStatic( WIID_PC_TRADE_RESULT_PC_POINT_IMAGE,	428,	183,	15,	11, this);
	pStatic->SetImage(OBJECT_IMAGE_FILE0, 492, 97);	
	pStatic->Show();
	pStatic = NULL;

	//WIID_PC_TRADE_RESULT_POINT_BACK
	pStatic = new SPWindowStatic(WIID_PC_TRADE_RESULT_POINT_BACK, 292, 196, 129, 15, this);
	pStatic->SetWindowType( WND_TYPE_LEFTBASE );
	pStatic->SetImageNormalHead(OBJECT_IMAGE_FILE0, 380, 268, 384, 283);
	pStatic->SetImageNormalBody(OBJECT_IMAGE_FILE0, 384, 268, 388, 283);
	pStatic->SetImageNormalTail(OBJECT_IMAGE_FILE0, 388, 268, 392, 283);
	pStatic->Show();
	
	//WIID_PC_TRADE_RESULT_POINT
	m_pPoint = new SPWindowStatic( WIID_PC_TRADE_RESULT_POINT,	46,	180,	121,	14, this);		
	m_pPoint->SetFormat(DT_RIGHT | DT_VCENTER);
	m_pPoint->SetFont(FONT_14_BOLD);
	m_pPoint->Show();
	
	//WIID_PC_TRADE_RESULT_POINT_IMAGE
	pStatic = new SPWindowStatic( WIID_PC_TRADE_RESULT_POINT_IMAGE,	428,	183,	15,	11, this);
	pStatic->SetImage(OBJECT_IMAGE_FILE0, 492, 97);	
	pStatic->Show();
	pStatic = NULL;

	//WIID_PC_TRADE_RESULT_PC_FEE_TEXT
	pStatic = new SPWindowStatic( WIID_PC_TRADE_RESULT_PC_FEE_TEXT,	31,	235,	44,	14, this);
	pStatic->SetImage(OBJECT_IMAGE_FILE0, 43, 449);	
	pStatic->Show();
	pStatic = NULL;

	//WIID_PC_TRADE_RESULT_PC_FEE_BACK
	pStatic = new SPWindowStatic(WIID_PC_TRADE_RESULT_PC_FEE_BACK, 79, 233, 92, 15, this);
	pStatic->SetWindowType( WND_TYPE_LEFTBASE );
	pStatic->SetImageNormalHead(OBJECT_IMAGE_FILE0, 380, 268, 384, 283);
	pStatic->SetImageNormalBody(OBJECT_IMAGE_FILE0, 384, 268, 388, 283);
	pStatic->SetImageNormalTail(OBJECT_IMAGE_FILE0, 388, 268, 392, 283);
	pStatic->Show();
	pStatic = NULL;
	
	//WIID_PC_TRADE_RESULT_PC_FEE
	m_pPcFee = new SPWindowStatic( WIID_PC_TRADE_RESULT_PC_FEE,	83,	234,	84,	15, this);		
	m_pPcFee->SetFormat(DT_RIGHT | DT_VCENTER);
	m_pPcFee->SetFont(FONT_14_BOLD);
	m_pPcFee->Show();

	//WIID_PC_TRADE_RESULT_PC_FEE_IMAGE
	pStatic = new SPWindowStatic( WIID_PC_TRADE_RESULT_PC_FEE_IMAGE,	179,	235,	15,	11, this);
	pStatic->SetImage(OBJECT_IMAGE_FILE0, 476, 97);	
	pStatic->Show();
	pStatic = NULL;

	//WIID_PC_TRADE_RESULT_FEE_TEXT
	pStatic = new SPWindowStatic( WIID_PC_TRADE_RESULT_FEE_TEXT,	281,	235,	44,	14, this);
	pStatic->SetImage(OBJECT_IMAGE_FILE0, 43, 449);
	pStatic->Show();
	pStatic = NULL;
	
	//WIID_PC_TRADE_RESULT_FEE_BACK
	pStatic = new SPWindowStatic(WIID_PC_TRADE_RESULT_FEE_BACK, 329, 233, 92, 15, this);
	pStatic->SetWindowType( WND_TYPE_LEFTBASE );
	pStatic->SetImageNormalHead(OBJECT_IMAGE_FILE0, 380, 268, 384, 283);
	pStatic->SetImageNormalBody(OBJECT_IMAGE_FILE0, 384, 268, 388, 283);
	pStatic->SetImageNormalTail(OBJECT_IMAGE_FILE0, 388, 268, 392, 283);
	pStatic->Show();
	pStatic = NULL;

	//WIID_PC_TRADE_RESULT_FEE
	m_pFee = new SPWindowStatic( WIID_PC_TRADE_RESULT_FEE,	333,	234,	84,	15, this);		
	m_pFee->SetFormat(DT_RIGHT | DT_VCENTER);
	m_pFee->SetFont(FONT_14_BOLD);
	m_pFee->Show();
	
	//WIID_PC_TRADE_RESULT_FEE_IMAGE
	pStatic = new SPWindowStatic( WIID_PC_TRADE_RESULT_FEE_IMAGE,	429,	235,	15,	11, this);
	pStatic->SetImage(OBJECT_IMAGE_FILE0, 476, 97);	
	pStatic->Show();
	pStatic = NULL;

	//WIID_PC_TRADE_RESULT_WARNING
	pStatic = new SPWindowStatic( WIID_PC_TRADE_RESULT_WARNING,	97,	248,	267,	33, this);	
	//char* IMAGE_FILE5 = "DATA/INTERFACE/CONCEPT/UI105.PNG";
	//pStatic->SetImage(IMAGE_FILE5, 244, 1);
	pStatic->SetImage(OBJECT_IMAGE_FILE3, 1, 1);
	pStatic->Show();
	pStatic = NULL;

	//WIID_PC_TRADE_RESULT_OK		
	m_pAccept = new SPWindowButton( WIID_PC_TRADE_RESULT_OK , 159, 237, 68, 17, this);
	m_pAccept->SetImage		(BUTTON_IMAGE_FILE0, 	70,  145);
	m_pAccept->SetImageHit	(BUTTON_IMAGE_FILE0, 	70,  163);
	m_pAccept->SetImagePush	(BUTTON_IMAGE_FILE0, 	70,  181);
	m_pAccept->SetImageDisable(BUTTON_IMAGE_FILE0,	70,  199);
	m_pAccept->Show();

	//WIID_PC_TRADE_RESULT_CANCEL	
	m_pCancel = new SPWindowButton( WIID_PC_TRADE_RESULT_CANCEL , 235, 237, 68, 17, this);
	m_pCancel->SetImage		(BUTTON_IMAGE_FILE0, 	139,  145);
	m_pCancel->SetImageHit	(BUTTON_IMAGE_FILE0, 	139,  163);
	m_pCancel->SetImagePush	(BUTTON_IMAGE_FILE0, 	139,  181);
	m_pCancel->SetImageDisable(BUTTON_IMAGE_FILE0,	139,  199);
	m_pCancel->Show();
	
	m_strPcInfo.clear();
	if(g_pResourceManager->GetGlobalString(4001042))
		m_strPcInfo = g_pResourceManager->GetGlobalString(4001042);
}


void SPWindowPcTradeResult::ArrangeControl()
{
	SPWindow* pStatic = NULL;
	
	int iEditStartY = 179 + 15;
	int iImgStartY = 183 + 5;

	int iEditInterval = 2;
	int iImgInterval = 7;
	
	{
		//WIID_PC_TRADE_RESULT_PC_CASH_BACK
		pStatic = NULL;
		pStatic = Find(WIID_PC_TRADE_RESULT_PC_CASH_BACK, true);
		if(pStatic) {
			pStatic->Hide();
		}		

		//WIID_PC_TRADE_RESULT_PC_CASH
		if(m_pPcCash) {
			m_pPcCash->Hide();
		}

		//WIID_PC_TRADE_RESULT_PC_CASH_IMAGE
		pStatic = NULL;
		pStatic = Find(WIID_PC_TRADE_RESULT_PC_CASH_IMAGE, true);
		if(pStatic) {
			pStatic->Hide();
		}

		//WIID_PC_TRADE_RESULT_CASH_BACK
		pStatic = NULL;
		pStatic = Find(WIID_PC_TRADE_RESULT_CASH_BACK, true);
		if(pStatic) {
			pStatic->Hide();
		}

		//WIID_PC_TRADE_RESULT_CASH
		pStatic = NULL;
		if(m_pCash) {
			m_pCash->Hide();
		}

		//WIID_PC_TRADE_RESULT_CASH_IMAGE
		pStatic = NULL;
		pStatic = Find(WIID_PC_TRADE_RESULT_CASH_IMAGE, true);
		if(pStatic) {
			pStatic->Hide();
		}
	}
	if(SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_ALL || 
		SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_CASH)
	{
		iEditStartY += iEditInterval;
		iImgStartY += iImgInterval + 3;

		//WIID_PC_TRADE_RESULT_PC_CASH_BACK
		pStatic = NULL;
		pStatic = Find(WIID_PC_TRADE_RESULT_PC_CASH_BACK, true);
		if(pStatic) {
			pStatic->AdjustCoord(42, iEditStartY, 129, 15);
			pStatic->Show();
		}		

		//WIID_PC_TRADE_RESULT_PC_CASH
		if(m_pPcCash) {
			m_pPcCash->AdjustCoord(46, iEditStartY, 121, 15);
			m_pPcCash->Show();
		}

		//WIID_PC_TRADE_RESULT_PC_CASH_IMAGE
		pStatic = NULL;
		pStatic = Find(WIID_PC_TRADE_RESULT_PC_CASH_IMAGE, true);
		if(pStatic) {
			pStatic->AdjustCoord(179, iImgStartY, 15, 11);
			pStatic->Show();
		}

		//WIID_PC_TRADE_RESULT_CASH_BACK
		pStatic = NULL;
		pStatic = Find(WIID_PC_TRADE_RESULT_CASH_BACK, true);
		if(pStatic) {
			pStatic->AdjustCoord(292, iEditStartY, 129, 15);
			pStatic->Show();
		}

		//WIID_PC_TRADE_RESULT_CASH
		if(m_pCash) {
			m_pCash->AdjustCoord(296, iEditStartY, 121, 15);
			m_pCash->Show();
		}

		//WIID_PC_TRADE_RESULT_CASH_IMAGE
		pStatic = NULL;
		pStatic = Find(WIID_PC_TRADE_RESULT_CASH_IMAGE, true);
		if(pStatic) {
			pStatic->AdjustCoord(429, iImgStartY, 15, 11);
			pStatic->Show();
		}

		iEditStartY += 15;
		iImgStartY += 11;
	}

	{
		//WIID_PC_TRADE_RESULT_PC_POINT_BACK
		pStatic = NULL;
		pStatic = Find(WIID_PC_TRADE_RESULT_PC_POINT_BACK, true);
		if(pStatic) {
			pStatic->Hide();
		}

		//WIID_PC_TRADE_RESULT_PC_POINT
		if(m_pPcPoint) {
			m_pPcPoint->Hide();
		}

		//WIID_PC_TRADE_RESULT_PC_POINT_IMAGE
		pStatic = NULL;
		pStatic = Find(WIID_PC_TRADE_RESULT_PC_POINT_IMAGE, true);
		if(pStatic) {
			pStatic->Hide();
		}

		//WIID_PC_TRADE_RESULT_POINT_BACK
		pStatic = NULL;
		pStatic = Find(WIID_PC_TRADE_RESULT_POINT_BACK, true);
		if(pStatic) {
			pStatic->Hide();
		}

		//WIID_PC_TRADE_RESULT_POINT
		if(m_pPoint) {
			m_pPoint->Hide();
		}

		//WIID_PC_TRADE_RESULT_POINT_IMAGE
		pStatic = NULL;
		pStatic = Find(WIID_PC_TRADE_RESULT_POINT_IMAGE, true);
		if(pStatic) {
			pStatic->Hide();
		}
	}
	if(SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_ALL || 
		SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_POINT)
	{
		iEditStartY += iEditInterval;
		iImgStartY += iImgInterval;

		if(SPLocalizeManager::GetInstance()->GetCashTradeType() == CASH_TRADE_POINT)
			iImgStartY += 3;

		//WIID_PC_TRADE_RESULT_PC_POINT_BACK
		pStatic = NULL;
		pStatic = Find(WIID_PC_TRADE_RESULT_PC_POINT_BACK, true);
		if(pStatic) {
			pStatic->AdjustCoord(42, iEditStartY, 129, 15);
			pStatic->Show();
		}

		//WIID_PC_TRADE_RESULT_PC_POINT
		if(m_pPcPoint) {
			m_pPcPoint->AdjustCoord(46,	iEditStartY, 121, 15);
			m_pPcPoint->Show();
		}

		//WIID_PC_TRADE_RESULT_PC_POINT_IMAGE
		pStatic = NULL;
		pStatic = Find(WIID_PC_TRADE_RESULT_PC_POINT_IMAGE, true);
		if(pStatic) {
			pStatic->AdjustCoord(179,	iImgStartY,	15,	11);
			pStatic->Show();
		}

		//WIID_PC_TRADE_RESULT_POINT_BACK
		pStatic = NULL;
		pStatic = Find(WIID_PC_TRADE_RESULT_POINT_BACK, true);
		if(pStatic) {
			pStatic->AdjustCoord(292, iEditStartY, 129, 15);
			pStatic->Show();
		}

		//WIID_PC_TRADE_RESULT_POINT
		if(m_pPoint) {
			m_pPoint->AdjustCoord(296,	iEditStartY,	121,	15);
			m_pPoint->Show();
		}

		//WIID_PC_TRADE_RESULT_POINT_IMAGE
		pStatic = NULL;
		pStatic = Find(WIID_PC_TRADE_RESULT_POINT_IMAGE, true);
		if(pStatic) {
			pStatic->AdjustCoord(429,	iImgStartY,	15,	11);
			pStatic->Show();
		}

		iEditStartY += 15;
		iImgStartY += 11;
	}


	{
		//WIID_PC_TRADE_RESULT_PC_FEE_TEXT
		pStatic = NULL;
		pStatic = Find(WIID_PC_TRADE_RESULT_PC_FEE_TEXT, true);
		if(pStatic) {
			pStatic->Hide();
		}

		//WIID_PC_TRADE_RESULT_PC_FEE_BACK
		pStatic = NULL;
		pStatic = Find(WIID_PC_TRADE_RESULT_PC_FEE_BACK, true);
		if(pStatic) {
			pStatic->Hide();
		}

		//WIID_PC_TRADE_RESULT_PC_FEE
		if(m_pPcFee) {
			m_pPcFee->Hide();
		}

		//WIID_PC_TRADE_RESULT_PC_FEE_IMAGE
		pStatic = NULL;
		pStatic = Find(WIID_PC_TRADE_RESULT_PC_FEE_IMAGE, true);
		if(pStatic) {
			pStatic->Hide();
		}

		//WIID_PC_TRADE_RESULT_FEE_TEXT
		pStatic = NULL;
		pStatic = Find(WIID_PC_TRADE_RESULT_FEE_TEXT, true);
		if(pStatic) {
			pStatic->Hide();
		}

		//WIID_PC_TRADE_RESULT_FEE_BACK
		pStatic = NULL;
		pStatic = Find(WIID_PC_TRADE_RESULT_FEE_BACK, true);
		if(pStatic) {
			pStatic->Hide();
		}

		//WIID_PC_TRADE_RESULT_FEE
		if(m_pFee) {
			m_pFee->Hide();
		}

		//WIID_PC_TRADE_RESULT_FEE_IMAGE
		pStatic = NULL;
		pStatic = Find(WIID_PC_TRADE_RESULT_FEE_IMAGE, true);
		if(pStatic) {
			pStatic->Hide();
		}
	}
	if(SPLocalizeManager::GetInstance()->GetCashTradeType() > CASH_TRADE_NULL) {
		iEditStartY += 4;
		iImgStartY += iImgInterval;
		
		//WIID_PC_TRADE_RESULT_PC_FEE_TEXT
		pStatic = NULL;
		pStatic = Find(WIID_PC_TRADE_RESULT_PC_FEE_TEXT, true);
		if(pStatic) {
			pStatic->AdjustCoord(31,	iEditStartY + 3,	44,	14);
			pStatic->Hide();
		}

		//WIID_PC_TRADE_RESULT_PC_FEE_BACK
		pStatic = NULL;
		pStatic = Find(WIID_PC_TRADE_RESULT_PC_FEE_BACK, true);
		if(pStatic) {
			pStatic->AdjustCoord(79, iEditStartY, 92, 15);
			pStatic->Hide();
		}

		//WIID_PC_TRADE_RESULT_PC_FEE
		if(m_pPcFee) {
			m_pPcFee->AdjustCoord(83,	iEditStartY,	84,	15);
			m_pPcFee->Hide();
		}

		//WIID_PC_TRADE_RESULT_PC_FEE_IMAGE
		pStatic = NULL;
		pStatic = Find(WIID_PC_TRADE_RESULT_PC_FEE_IMAGE, true);
		if(pStatic) {
			pStatic->AdjustCoord(179,	iImgStartY + 1,	15,	11);
			pStatic->Hide();
		}

		//WIID_PC_TRADE_RESULT_FEE_TEXT
		pStatic = NULL;
		pStatic = Find(WIID_PC_TRADE_RESULT_FEE_TEXT, true);
		if(pStatic) {
			pStatic->AdjustCoord(281,	iEditStartY + 3,	44,	14);
			pStatic->Hide();
		}

		//WIID_PC_TRADE_RESULT_FEE_BACK
		pStatic = NULL;
		pStatic = Find(WIID_PC_TRADE_RESULT_FEE_BACK, true);
		if(pStatic) {
			pStatic->AdjustCoord(329, iEditStartY, 92, 15);
			pStatic->Hide();
		}

		//WIID_PC_TRADE_RESULT_FEE
		if(m_pFee) {
			m_pFee->AdjustCoord(333,	iEditStartY,	84,	15);
			m_pFee->Hide();
		}

		//WIID_PC_TRADE_RESULT_FEE_IMAGE
		pStatic = NULL;
		pStatic = Find(WIID_PC_TRADE_RESULT_FEE_IMAGE, true);
		if(pStatic) {
			pStatic->AdjustCoord(429,	iImgStartY + 1,	15,	11);
			pStatic->Hide();
		}

		iEditStartY += 15;
		iImgStartY += 11;
	}	

	////WIID_PC_TRADE_RESULT_WARNING
	pStatic = NULL;
	pStatic = Find(WIID_PC_TRADE_RESULT_WARNING, true);
	if(pStatic) {
		pStatic->AdjustCoord(97,	iEditStartY + 2,	267,	33);
		pStatic->Show();
		iEditStartY += 33;
	}
	
	//WIID_PC_TRADE_RESULT_OK
	//iEditStartY += 5;
	if(m_pAccept) {
		m_pAccept->AdjustCoord(159, iEditStartY, 68, 17);
		m_pAccept->Show();
	}

	if(m_pCancel) {
		m_pCancel->AdjustCoord(235, iEditStartY, 68, 17);
		m_pCancel->Show();
	}
}


void SPWindowPcTradeResult::Process(float fTime)
{
	if(!m_bShow) 
		return;	
	
	SPWindow::Process(fTime);

	//트레이드가 가능한 상황인지를 검사한다.
	SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_PC_TRADE);
	if(pWindow == NULL) 
		return;

	SPWindowPcTrade* pTradeWindow = static_cast<SPWindowPcTrade*>(pWindow);

	if(pTradeWindow == NULL)
		return;

	if(pTradeWindow->IsTradeEnable() == false) {
		pTradeWindow->SendTradeCancel();
		//OnHide(w0, 0);
	}
	
	m_fWaitAccumulate += fTime;
	if( m_fWaitAccumulate > m_fWaitDeley) {
		m_fWaitAccumulate -= m_fWaitDeley;
		pTradeWindow->SendTradeCancel();
	}
}


void SPWindowPcTradeResult::Render(float fTime)
{
	if(!m_bShow) 
		return;

	g_pVideo->Flush();
	
	int i = 0;
	if(m_pBaseSkin) {
		for(i = WINDOW_BASE_TOP_LEFT; i < WINDOW_BASE_MAX; i++) {
			m_pBaseSkin->RenderTexture(&m_rtBaseDest[i], &m_rtBaseSrc[i]);
		}
	}

	SPWindow::Render(fTime);

	for(i = 0; i < MAX_PC_TRADE_COUNT; i++) {
		//Icon Disable 표현
		if(m_pDisable) {
			RECT rtDest;
			m_pItemIcon[i]->GetWindowRect(rtDest);
			if(m_bItemAble[i] == false) {		
				m_pDisable->RenderTexture(&rtDest, &m_rtSrcDisable);
			}
			if(m_iItemMark[i] > ICON_MARK_NULL) {
				m_pDisable->RenderTexture(&rtDest, &m_rtMarkSrc[m_iItemMark[i] - 1]);
			}

			RECT rtPcDest;
			m_pPcItemIcon[i]->GetWindowRect(rtPcDest);
			if(m_bPcItemAble[i] == false) {				
				m_pDisable->RenderTexture(&rtPcDest, &m_rtSrcDisable);
			}
			if(m_iPcItemMark[i] > ICON_MARK_NULL) {
				m_pDisable->RenderTexture(&rtPcDest, &m_rtMarkSrc[m_iPcItemMark[i] - 1]);
			}
		}

		//Stack Count Local 표현
		if(m_iStack[i] > 9) {
			RECT rtTenNumTarget, rtNumTarget, rtTenNumSrc, rtNumSrc;
			int iTenStack = m_iStack[i] / 10;
			int iOneStack = m_iStack[i] % 10;

			m_pItemIcon[i]->GetWindowRect(m_rtStack[i]);
			SetRect(&rtTenNumTarget, m_rtStack[i].left, m_rtStack[i].bottom - 10, m_rtStack[i].left + 9, m_rtStack[i].bottom);
			SetRect(&rtNumTarget, rtTenNumTarget.right - 2, m_rtStack[i].bottom - 10, rtTenNumTarget.right + 9 - 2, m_rtStack[i].bottom);

			rtTenNumSrc.left = iTenStack * 9;
			rtTenNumSrc.right = rtTenNumSrc.left + 9;
			rtTenNumSrc.top = 0;
			rtTenNumSrc.bottom = 10;

			rtNumSrc.left = iOneStack * 9;
			rtNumSrc.right = rtNumSrc.left + 9;
			rtNumSrc.top = 0;
			rtNumSrc.bottom = 10;

			g_pItemCluster->GetNumTexture()->RenderTexture( &rtTenNumTarget , &rtTenNumSrc);
			g_pItemCluster->GetNumTexture()->RenderTexture( &rtNumTarget , &rtNumSrc);
		}
		else if(m_iStack[i] > 1) {
			int iOneStack = m_iStack[i] % 10;
			m_pItemIcon[i]->GetWindowRect(m_rtStack[i]);
			RECT rtNumTarget, rtNumSrc;
			SetRect(&rtNumTarget, m_rtStack[i].left, m_rtStack[i].bottom - 10, m_rtStack[i].left + 9, m_rtStack[i].bottom);

			rtNumSrc.left = iOneStack * 9;
			rtNumSrc.right = rtNumSrc.left + 9;
			rtNumSrc.top = 0;
			rtNumSrc.bottom = 10;

			g_pItemCluster->GetNumTexture()->RenderTexture( &rtNumTarget , &rtNumSrc);
		}	

		////Stack Count 상대 PC 표현
		if(m_iPcStack[i] > 9) {
			RECT rtTenNumTarget, rtNumTarget, rtTenNumSrc, rtNumSrc;
			int iTenStack = m_iPcStack[i] / 10;
			int iOneStack = m_iPcStack[i] % 10;

			m_pPcItemIcon[i]->GetWindowRect(m_rtPcStack[i]);
			SetRect(&rtTenNumTarget, m_rtPcStack[i].left, m_rtPcStack[i].bottom - 10, m_rtPcStack[i].left + 9, m_rtPcStack[i].bottom);
			SetRect(&rtNumTarget, rtTenNumTarget.right - 2, m_rtPcStack[i].bottom - 10, rtTenNumTarget.right + 9 - 2, m_rtPcStack[i].bottom);

			rtTenNumSrc.left = iTenStack * 9;
			rtTenNumSrc.right = rtTenNumSrc.left + 9;
			rtTenNumSrc.top = 0;
			rtTenNumSrc.bottom = 10;

			rtNumSrc.left = iOneStack * 9;
			rtNumSrc.right = rtNumSrc.left + 9;
			rtNumSrc.top = 0;
			rtNumSrc.bottom = 10;

			g_pItemCluster->GetNumTexture()->RenderTexture( &rtTenNumTarget , &rtTenNumSrc);
			g_pItemCluster->GetNumTexture()->RenderTexture( &rtNumTarget , &rtNumSrc);
		}
		else if(m_iPcStack[i] > 1) {
			int iOneStack = m_iPcStack[i] % 10;
			RECT rtNumTarget, rtNumSrc;
			m_pPcItemIcon[i]->GetWindowRect(m_rtPcStack[i]);
			SetRect(&rtNumTarget, m_rtPcStack[i].left, m_rtPcStack[i].bottom - 10, m_rtPcStack[i].left + 9, m_rtPcStack[i].bottom);

			rtNumSrc.left = iOneStack * 9;
			rtNumSrc.right = rtNumSrc.left + 9;
			rtNumSrc.top = 0;
			rtNumSrc.bottom = 10;

			g_pItemCluster->GetNumTexture()->RenderTexture( &rtNumTarget , &rtNumSrc);
		}
	}

////
//#ifdef _DEBUG	
//	RECT rtRect;	
//	//SPWindow* pwindow = Find(WIID_PC_TRADE_RESULT_WARNING, true);
//	//if(pwindow) {
//	//	g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
//	//	pwindow->GetWindowRect(rtRect);
//	//	g_pVideo->GetSysLine()->DrawRect(rtRect);
//	//	g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
//	//}
//
//	//WIID_PC_TRADE_RESULT_FEE_IMAGE
//	SPWindow* pStatic = Find(WIID_PC_TRADE_RESULT_FEE_IMAGE, true);
//	if(pStatic) {
//		g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
//		pStatic->GetWindowRect(rtRect);
//		g_pVideo->GetSysLine()->DrawRect(rtRect);
//		g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
//	}
//	
//	if(m_pAccept) {
//		g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
//		m_pAccept->GetWindowRect(rtRect);
//		g_pVideo->GetSysLine()->DrawRect(rtRect);
//		g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
//	}
//
//	if(m_pCancel) {
//		g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
//		m_pCancel->GetWindowRect(rtRect);
//		g_pVideo->GetSysLine()->DrawRect(rtRect);
//		g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
//	}
//
//#endif
}


void SPWindowPcTradeResult::Show(bool bWithChild /*= true*/)
{
	if(g_pCheckManager->CheckPlayerState() != PLAYER_STATE_TRADE)
		return;
	
	ClearControl();

	SPWindow::Show(bWithChild);	

	ArrangeControl();
	RefreshRelationCoord();

	g_pInterfaceManager->SetModalWindow(this);
	g_pInterfaceManager->SetFocusWindow(this);

	m_pAccept->SetEnable(true);
	m_pCancel->SetEnable(true);	

	if(!UpdateControl())
		return;
	
	m_fWaitAccumulate = 0.0f;
}


void SPWindowPcTradeResult::Hide()
{	
	g_pInterfaceManager->SetFocusWindow(NULL);
	g_pInterfaceManager->SetModalWindow(NULL);	
	SPWindow::Hide();	
}


void SPWindowPcTradeResult::ClearControl()
{
	for(int i = 0; i < MAX_PC_TRADE_COUNT; i++) {
		m_pPcItemIcon[i]->SPSendMessage(SPIM_SET_IMAGECLEAR);
		m_pPcItemIcon[i]->SetWindowText(NULL);
		m_pPcItemName[i]->SetWindowText(NULL); 
		m_iPcStack[i] = 0;
		m_bPcItemAble[i] = true;
		m_iPcItemMark[i] = ICON_MARK_NULL;
		
		m_pItemIcon[i]->SPSendMessage(SPIM_SET_IMAGECLEAR);
		m_pItemIcon[i]->SetWindowText(NULL);
		m_pItemName[i]->SetWindowText(NULL); 
		m_iStack[i] = 0;
		m_bItemAble[i] = true;
		m_iItemMark[i] = ICON_MARK_NULL;		
	}
}


bool SPWindowPcTradeResult::UpdateControl()
{	
	SPSelectItem* pTradeSelectItem = NULL;
	SPItem* pTradeItem =  NULL;
	int iStackCount = 0;
	int iRearLevel = 0;
	POINT ptSrcPos;
	SPTexture* pTexture = NULL;	

	for(int iIndex = 0; iIndex < MAX_PC_TRADE_COUNT; iIndex++) {
		if(!SetUserItem(iIndex))
			continue;
		
		if(!SetPcItem(iIndex))
			continue;
	}
	
	SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_PC_TRADE);
	if(pWindow == NULL) 
		return false;

	SPWindowPcTrade* pTradeWindow = static_cast<SPWindowPcTrade*>(pWindow);

	if(pTradeWindow == NULL)
		return false;

    char szTemp[128];
	ZeroMemory(szTemp, 128);
	sprintf(szTemp, m_strPcInfo.c_str(), pTradeWindow->GetPcName());	
	m_pPcName->SetWindowText(szTemp);	
	
	m_pPcEly->SetWindowText(ConvertMoneyToString(pTradeWindow->GetPcEly()));
	m_pEly->SetWindowText(ConvertMoneyToString(pTradeWindow->GetEly()));

	int iPcCash = pTradeWindow->GetPcCash();
	int iCash = pTradeWindow->GetCash();
	m_pPcCash->SetWindowText(ConvertMoneyToString(iPcCash));
	m_pCash->SetWindowText(ConvertMoneyToString(iCash));

	int iPcPoint = pTradeWindow->GetPcPoint();
	int iPoint = pTradeWindow->GetPoint();
	m_pPcPoint->SetWindowText(ConvertMoneyToString(iPcPoint));
	m_pPoint->SetWindowText(ConvertMoneyToString(iPoint));

	int iPcFee = pTradeWindow->GetPcFee();
	int iFee = pTradeWindow->GetFee();
	
	if(iPcFee) {
		//WIID_PC_TRADE_RESULT_PC_FEE_TEXT
		SPWindow* pStatic = NULL;
		pStatic = Find(WIID_PC_TRADE_RESULT_PC_FEE_TEXT, true);
		if(pStatic) {
			pStatic->Show();
		}

		//WIID_PC_TRADE_RESULT_PC_FEE_BACK
		pStatic = NULL;
		pStatic = Find(WIID_PC_TRADE_RESULT_PC_FEE_BACK, true);
		if(pStatic) {
			pStatic->Show();
		}

		//WIID_PC_TRADE_RESULT_PC_FEE
		if(m_pPcFee) {
			m_pPcFee->SetWindowText(ConvertMoneyToString(iPcFee));
			m_pPcFee->Show();
		}

		//WIID_PC_TRADE_RESULT_PC_FEE_IMAGE
		pStatic = NULL;
		pStatic = Find(WIID_PC_TRADE_RESULT_PC_FEE_IMAGE, true);		
		if(pStatic) {
			if(iPcCash && iPcFee) {
				pStatic->SetImagePos(476, 97);
			}

			if(iPcPoint && iPcFee) {
				pStatic->SetImagePos(492, 97);
			}
			pStatic->Show();
		}
	}
	
	if(iFee) {
		//WIID_PC_TRADE_RESULT_FEE_TEXT
		SPWindow* pStatic = NULL;
		pStatic = Find(WIID_PC_TRADE_RESULT_FEE_TEXT, true);
		if(pStatic) {
			pStatic->Show();
		}

		//WIID_PC_TRADE_RESULT_FEE_BACK
		pStatic = NULL;
		pStatic = Find(WIID_PC_TRADE_RESULT_FEE_BACK, true);
		if(pStatic) {
			pStatic->Show();
		}

		//WIID_PC_TRADE_RESULT_FEE
		if(m_pFee) {			
			m_pFee->SetWindowText(ConvertMoneyToString(iFee));
			m_pFee->Show();
		}

		//WIID_PC_TRADE_RESULT_FEE_IMAGE
		pStatic = NULL;
		pStatic = Find(WIID_PC_TRADE_RESULT_FEE_IMAGE, true);
		if(pStatic) {
			if(iCash && iFee) {
				pStatic->SetImagePos(476, 97);
			}

			if(iPoint && iFee) {
				pStatic->SetImagePos(492, 97);
			}
			pStatic->Show();
		}		
	}	
	
	return true;
}


bool SPWindowPcTradeResult::SetUserItem(int iIndex)
{
	SPPlayerInvenArchive* pInven = NULL;
	if(g_pGOBManager->GetLocalPlayer())
		pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	else 
		return false;	
	
	SPSelectItem* pTradeSelectItem = NULL;
	SPItem* pTradeItem =  NULL;
	int iStackCount = 0;
	int iRearLevel = 0;
	POINT ptSrcPos;
	SPTexture* pTexture = NULL;	

	pTradeSelectItem = pInven->GetTradeItem(iIndex);
	pTradeItem = pInven->GetInventoryItem(pTradeSelectItem->m_iContainer, pTradeSelectItem->m_iSlotIndex);
	
	if(pTradeItem && !pTradeItem->IsEmptyItem()) {
		iStackCount = pTradeItem->GetItemStatusValue(MV_ITEM_GETSTACK_CNT);
		iRearLevel = pTradeItem->GetItemStatusValue(MV_ITEM_GET_RARELEVEL);

		pTexture = g_pItemCluster->GetItemTexture(pTradeItem->GetItemStatus()->GetItemID(), ptSrcPos);
		//m_pIcon[iCounter]->SPSendMessage(SPIM_SET_IMAGECLEAR);
		m_pItemIcon[iIndex]->Hide();
		m_pItemIcon[iIndex]->SetImage(pTexture, ptSrcPos.x, ptSrcPos.y);
		m_pItemIcon[iIndex]->Show();

		if(pTradeItem->GetItemAttr()->m_bNameColor) {
			m_pItemName[iIndex]->SetFontColor(pTradeItem->GetItemAttr()->GetNameColor());
		}
		else {
			m_pItemName[iIndex]->SetFontColor(g_pCheckManager->GetItemColor(iRearLevel));
		}

		//[2006/7/12] 긴 아이템 명 잘라내기
		if(pTradeItem->GetItemAttr()->m_strName.size() > 24) {
			char szTemp[32];
			ZeroMemory(szTemp, 32);
			string strSub = pTradeItem->GetItemAttr()->m_strName.substr(0, 24);
			wsprintf(szTemp, "%s", strSub.c_str());		
			m_pItemName[iIndex]->SetWindowText(szTemp);
		}
		else { 
			m_pItemName[iIndex]->SetWindowText(pTradeItem->GetItemAttr()->m_strName.c_str());
		}

		m_iStack[iIndex] = iStackCount;

		//[2005/10/10] 현재 올려놓은 아이템이 내가 사용할수 있는 아이템인지 확인한다 
		m_bItemAble[iIndex] = g_pCheckManager->CheckItemStatus(pTradeItem->GetItemStatus()->GetItemID());
		m_iItemMark[iIndex] = g_pItemCluster->GetItemIconMark(pTradeItem->GetItemStatus()->GetItemID());
	}
	else {
		m_pItemIcon[iIndex]->SPSendMessage(SPIM_SET_IMAGECLEAR);
		m_pItemIcon[iIndex]->SetWindowText(NULL);
		m_pItemName[iIndex]->SetWindowText(NULL); 
		m_iStack[iIndex] = 0;
		m_bItemAble[iIndex] = true;
		m_iItemMark[iIndex] = ICON_MARK_NULL;
	}

	return true;
}


bool SPWindowPcTradeResult::SetPcItem(int iIndex)
{
	SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_PC_TRADE);
	if(pWindow == NULL) 
		return false;

	SPWindowPcTrade* pTradeWindow = static_cast<SPWindowPcTrade*>(pWindow);

	if(pTradeWindow == NULL)
		return false;	
	
	SPItem* pTradeItem =  NULL;
	int iStackCount = 0;
	int iRearLevel = 0;
	POINT ptSrcPos;
	SPTexture* pTexture = NULL;

	pTradeItem = pTradeWindow->GetPcItem(iIndex);
	if(pTradeItem && !pTradeItem->IsEmptyItem()) {
		iStackCount = pTradeItem->GetItemStatusValue(MV_ITEM_GETSTACK_CNT);
		iRearLevel = pTradeItem->GetItemStatusValue(MV_ITEM_GET_RARELEVEL);

		pTexture = g_pItemCluster->GetItemTexture(pTradeItem->GetItemStatus()->GetItemID(), ptSrcPos);
		//m_pIcon[iCounter]->SPSendMessage(SPIM_SET_IMAGECLEAR);
		m_pPcItemIcon[iIndex]->Hide();
		m_pPcItemIcon[iIndex]->SetImage(pTexture, ptSrcPos.x, ptSrcPos.y);
		m_pPcItemIcon[iIndex]->Show();

		if(pTradeItem->GetItemAttr()->m_bNameColor) {
			m_pPcItemName[iIndex]->SetFontColor(pTradeItem->GetItemAttr()->GetNameColor());
		}
		else {
			m_pPcItemName[iIndex]->SetFontColor(g_pCheckManager->GetItemColor(iRearLevel));
		}

		//[2006/7/12] 긴 아이템 명 잘라내기
		if(pTradeItem->GetItemAttr()->m_strName.size() > 24) {
			char szTemp[32];
			ZeroMemory(szTemp, 32);
			string strSub = pTradeItem->GetItemAttr()->m_strName.substr(0, 24);
			wsprintf(szTemp, "%s", strSub.c_str());		
			m_pPcItemName[iIndex]->SetWindowText(szTemp);
		}
		else { 
			m_pPcItemName[iIndex]->SetWindowText(pTradeItem->GetItemAttr()->m_strName.c_str());
		}

		m_iPcStack[iIndex] = iStackCount;

		//[2005/10/10] 현재 올려놓은 아이템이 내가 사용할수 있는 아이템인지 확인한다 
		m_bPcItemAble[iIndex] = g_pCheckManager->CheckItemStatus(pTradeItem->GetItemStatus()->GetItemID());
		m_iPcItemMark[iIndex] = g_pItemCluster->GetItemIconMark(pTradeItem->GetItemStatus()->GetItemID());
	}
	else {
		m_pPcItemIcon[iIndex]->SPSendMessage(SPIM_SET_IMAGECLEAR);
		m_pPcItemIcon[iIndex]->SetWindowText(NULL);
		m_pPcItemName[iIndex]->SetWindowText(NULL); 
		m_iPcStack[iIndex] = 0;
		m_bPcItemAble[iIndex] = true;
		m_iPcItemMark[iIndex] = ICON_MARK_NULL;
	}

	return true;
}




SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowPcTradeResult )
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP	,	WIID_PC_TRADE_RESULT_OK					,	OnOk				)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP	,	WIID_PC_TRADE_RESULT_CANCEL				,	OnCancel			)

SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_ENTER,	WIID_PC_TRADE_RESULT_ITEM_BTN,		WIID_PC_TRADE_RESULT_ITEM_BTN + MAX_PC_TRADE_COUNT,	OnToolTipEnter)
SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_OUT,		WIID_PC_TRADE_RESULT_ITEM_BTN,		WIID_PC_TRADE_RESULT_ITEM_BTN + MAX_PC_TRADE_COUNT,	OnToolTipOut)
SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_MOVE,	WIID_PC_TRADE_RESULT_ITEM_BTN,		WIID_PC_TRADE_RESULT_ITEM_BTN + MAX_PC_TRADE_COUNT,	OnToolTipMove)

SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_ENTER,	WIID_PC_TRADE_RESULT_PC_ITEM_BTN,	WIID_PC_TRADE_RESULT_PC_ITEM_BTN + MAX_PC_TRADE_COUNT,		OnPCToolTipEnter)
SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_OUT,		WIID_PC_TRADE_RESULT_PC_ITEM_BTN,	WIID_PC_TRADE_RESULT_PC_ITEM_BTN + MAX_PC_TRADE_COUNT,		OnPCToolTipOut)
SPIMESSAGE_CONTROL_RANGE( SPIM_TOOLTIP_MOVE,	WIID_PC_TRADE_RESULT_PC_ITEM_BTN,	WIID_PC_TRADE_RESULT_PC_ITEM_BTN + MAX_PC_TRADE_COUNT,		OnPCToolTipMove)
SPIMESSAGE_OBJ_MAP_END	( SPWindow	)


int SPWindowPcTradeResult::OnOk(WPARAM wParam, LPARAM lparam)
{
	SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_PC_TRADE);
	if(pWindow == NULL) 
		return false;

	SPWindowPcTrade* pTradeWindow = static_cast<SPWindowPcTrade*>(pWindow);
	
	pTradeWindow->SendTradeAccept();

	m_pAccept->SetEnable(false);
	//m_pCancel->SetEnable(false);
	
	return 1;
}


int SPWindowPcTradeResult::OnCancel(WPARAM wParam, LPARAM lparam)
{
	SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_PC_TRADE);
	if(pWindow == NULL) 
		return false;

	SPWindowPcTrade* pTradeWindow = static_cast<SPWindowPcTrade*>(pWindow);

	if(pTradeWindow == NULL)
		return false;

	pTradeWindow->SendTradeCancel();

	m_pAccept->SetEnable(false);
	m_pCancel->SetEnable(false);
	
	//Hide();
	return 1;
}


int SPWindowPcTradeResult::OnToolTipMove(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	int iSelectNum = iUiID - WIID_PC_TRADE_RESULT_ITEM_BTN;
	
	SPPlayerInvenArchive* pInven = NULL;
	if(g_pGOBManager->GetLocalPlayer())
		pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	else 
		return false;

	SPSelectItem* pSelectItem = pInven->GetTradeItem(iSelectNum);

	if(pSelectItem->m_iContainer < INVENTORY_EQUIP || pSelectItem->m_iContainer > INVENTORY_HOUSING) {
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
		return 0;
	}

	if(iSelectNum < 0 || iSelectNum >= pInven->GetInventorySize(pSelectItem->m_iContainer)) {
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
		return 0;
	}

	if(pInven->IsItemExist(pSelectItem->m_iSlotIndex, pSelectItem->m_iContainer) == false) {
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
		return 0;
	}

	SPItem* pItem = pInven->GetInventoryItem(pSelectItem->m_iContainer, pSelectItem->m_iSlotIndex);

	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage( SPIM_TOOLTIP_MOVE, (WPARAM)pItem->GetItemStatus(), lParam);
	pWindow->Show();

	return 1;
}


int SPWindowPcTradeResult::OnToolTipEnter(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	int iSelectNum = iUiID - WIID_PC_TRADE_RESULT_ITEM_BTN;

	SPPlayerInvenArchive* pInven = NULL;
	if(g_pGOBManager->GetLocalPlayer())
		pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	else 
		return false;

	SPSelectItem* pSelectItem = pInven->GetTradeItem(iSelectNum);

	if(pSelectItem->m_iContainer < INVENTORY_EQUIP || pSelectItem->m_iContainer > INVENTORY_HOUSING) {
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
		return 0;
	}

	if(iSelectNum < 0 || iSelectNum >= pInven->GetInventorySize(pSelectItem->m_iContainer)) {
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
		return 0;
	}

	if(pInven->IsItemExist(pSelectItem->m_iSlotIndex, pSelectItem->m_iContainer) == false) {
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
		return 0;
	}

	SPItem* pItem = pInven->GetInventoryItem(pSelectItem->m_iContainer, pSelectItem->m_iSlotIndex);

	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage( SPIM_TOOLTIP_MOVE, (WPARAM)pItem->GetItemStatus(), lParam);
	pWindow->Show();
	
	return 1;
}


int SPWindowPcTradeResult::OnToolTipOut(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
	pWindow->Show();
	return 1;
}


int SPWindowPcTradeResult::OnPCToolTipMove(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	int iSelectNum = iUiID - WIID_PC_TRADE_RESULT_PC_ITEM_BTN;
	
	SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_PC_TRADE);
	if(pWindow == NULL) 
		return 1;

	SPWindowPcTrade* pTradeWindow = static_cast<SPWindowPcTrade*>(pWindow);

	if(pTradeWindow == NULL)
		return 1;	

	SPItem* pItem  = pTradeWindow->GetPcItem(iSelectNum);

	if(pItem == NULL || pItem->IsEmptyItem()) {
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
		return 1;
	}	

	SPWindow* pToolTip = g_pInterfaceManager->GetToolTipWindow();
	pToolTip->SPSendMessage( SPIM_TOOLTIP_MOVE, (WPARAM)pItem->GetItemStatus(), lParam);
	pToolTip->Show();
	return 1;
}


int SPWindowPcTradeResult::OnPCToolTipEnter(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	int iSelectNum = iUiID - WIID_PC_TRADE_RESULT_PC_ITEM_BTN;

	SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_PC_TRADE);
	if(pWindow == NULL) 
		return 1;

	SPWindowPcTrade* pTradeWindow = static_cast<SPWindowPcTrade*>(pWindow);

	if(pTradeWindow == NULL)
		return 1;	

	SPItem* pItem  = pTradeWindow->GetPcItem(iSelectNum);

	if(pItem == NULL || pItem->IsEmptyItem()) {
		SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
		pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
		return 1;
	}	

	SPWindow* pToolTip = g_pInterfaceManager->GetToolTipWindow();
	pToolTip->SPSendMessage( SPIM_TOOLTIP_MOVE, (WPARAM)pItem->GetItemStatus(), lParam);
	pToolTip->Show();
	return 1;
}


int SPWindowPcTradeResult::OnPCToolTipOut(unsigned int iUiID, WPARAM wParam, LPARAM lParam)
{
	SPWindow* pWindow = g_pInterfaceManager->GetToolTipWindow();
	pWindow->SPSendMessage( SPIM_TOOLTIP_OUT, NULL, lParam);
	pWindow->Show();
	return 1;
}


