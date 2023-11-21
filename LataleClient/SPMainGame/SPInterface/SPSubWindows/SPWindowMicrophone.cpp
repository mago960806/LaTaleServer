
#include "SPCommon.h"

#include "SPUtil.h"

#include "SPManager.h"
#include "SPMouseCursor.h"
#include "SPIMEPool.h"
#include "SPSubGameManager.h"

#include "SPWindowDEF.h"
#include "SPSubGameManager.h"
#include "SPInterfaceManager.h"

//
#include "SPGameObject.h"
#include "SPGOBManager.h"
#include "SPRenderModelDEF.h"
#include "SPRenderModel.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGOBModel.h"
#include "SPGOBModelUnit.h"
#include "SPAvatarModelUnit.h"
#include "SPGOBCluster.h"
#include "SPGameObject.h"
#include "SPGOBStatus.h"
#include "SPGOBCoordPhysics.h"
#include "SPGOBModelUnit.h"			
#include "SPPlayerEquipInfo.h"
#include "SPGOBStatus.h"			
#include "SPPlayerInvenArchive.h"	
#include "SPPlayerStatusModel.h"	
#include "SPPlayerEquipInfo.h"
#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPItemCluster.h"
#include "SPSkill.h"
#include "SPSkillArchive.h"
#include "SPMotionStatus.h"
#include "SPSkillManager.h"
#include "SPPlayer.h"
#include "SPCoolTimeManager.h"
#include "SPCommandConvert.h"
#include "SPComboManager.h"

#include "SPWindow.h"
#include "SPWindowSlider.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPWindowEditMultiLine.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPAbuseFilter.h"
#include "SPWindowMicrophone.h"

#include "SPDebug.h"




SPWindowMicropone::SPWindowMicropone( INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent /*= NULL*/)
: SPWindow(WNDID_MICROPHONE, InstanceID, iX, iY, iCX, iCY, pParent)
{
	Init();
}


SPWindowMicropone::~SPWindowMicropone()
{
	Clean();
}


void SPWindowMicropone::Init()
{
	m_iMode = MIC_MODE_SERVER;
	InitSubControl();
}


void SPWindowMicropone::Clean()
{
	SPWindow::Clean();
}


void SPWindowMicropone::InitSubControl()
{
	char* IMAGE_FILE0 = "DATA/INTERFACE/CONCEPT/UI100.PNG";
	char* IMAGE_FILE2 = "DATA/INTERFACE/CONCEPT/UI102.PNG";
	char* IMAGE_FILE4 = "DATA/INTERFACE/CONCEPT/UI104.PNG";
	char* IMAGE_FILE5 = "DATA/INTERFACE/CONCEPT/UI105.PNG";

	//// Control
	SPWindowButton* pButton = NULL;
	SPWindowStatic* pStatic = NULL;
	SPWindowStatic* pCenter = NULL;
	SPWindowStatic* pParent = NULL;

	//WIID_MICROPHONE_TOP	
	pStatic = new SPWindowStatic( WIID_MICROPHONE_TOP,	0,	0,	4,	150, this);	
	pStatic->SetImage(IMAGE_FILE5, 482, 361);
	pStatic->Show();
	pStatic = NULL;

	//WIID_MICROPHONE_CENTER	
	pCenter = new SPWindowStatic( WIID_MICROPHONE_CENTER,	4,	0,	229,	150, this);	
	pCenter->SetImage(IMAGE_FILE5, 488, 361);
	pCenter->SetSrcSize(2, 150);
	pCenter->Show();

	//WIID_MICROPHONE_BOTTOM
	pStatic = new SPWindowStatic( WIID_MICROPHONE_BOTTOM,	233,	0,	4,	150, this);	
	pStatic->SetImage(IMAGE_FILE5, 492, 361);
	pStatic->Show();
	pStatic = NULL;

	////WIID_MICROPHONE_CLOSE		
	//pButton = new SPWindowButton( WIID_MICROPHONE_CLOSE , 216 - 4, 9, 12, 12, pCenter);
	//pButton->SetImage		(IMAGE_FILE2, 247,  217);
	//pButton->SetImageHit	(IMAGE_FILE2, 247,  230);
	//pButton->SetImagePush	(IMAGE_FILE2, 247,  243);
	//pButton->SetImageDisable(IMAGE_FILE2, 247,  256);
	//pButton->Show();

	//WIID_MICROPHONE_OK		
	m_pOk = new SPWindowButton( WIID_MICROPHONE_OK , 87 - 4, 118, 68, 17, pCenter);
	m_pOk->SetImage		(IMAGE_FILE0, 75,  468);
	m_pOk->SetImageHit	(IMAGE_FILE0, 144,  468);
	m_pOk->SetImagePush	(IMAGE_FILE0, 213,  468);
	m_pOk->SetImageDisable(IMAGE_FILE0, 213,  486);
	m_pOk->Show();

	//WIID_MICROPHONE_CANCEL	
	m_pCancel = new SPWindowButton( WIID_MICROPHONE_CANCEL , 159 - 4, 118, 68, 17, pCenter);
	m_pCancel->SetImage		(IMAGE_FILE4, 197,  218);
	m_pCancel->SetImageHit	(IMAGE_FILE4, 197,  236);
	m_pCancel->SetImagePush	(IMAGE_FILE4, 216,  291);
	m_pCancel->SetImageDisable(IMAGE_FILE4, 216,  309);
	m_pCancel->Show();

	//WIID_MICROPHONE_TITLE
	pStatic = new SPWindowStatic( WIID_MICROPHONE_TITLE,	10,	3,	167,	21, this);	
	pStatic->SetImage(IMAGE_FILE5, 244, 35);
	pStatic->Show();
	pStatic = NULL;	

	//WIID_MICROPHONE_SERVER_HELP	
	m_pHelp[MIC_MODE_SERVER] = new SPWindowStatic( WIID_MICROPHONE_SERVER_HELP,	9 - 4,	34,	226,	13, pCenter);
	m_pHelp[MIC_MODE_SERVER]->SetImage(IMAGE_FILE5, 1, 31);
	m_pHelp[MIC_MODE_SERVER]->SetSrcSize(226, 13);
	m_pHelp[MIC_MODE_SERVER]->Show();
	
	//WIID_MICROPHONE_CHANNEL_HELP
	m_pHelp[MIC_MODE_CHANNEL] = new SPWindowStatic( WIID_MICROPHONE_CHANNEL_HELP,	9 - 4,	34,	226,	13, pCenter);
	m_pHelp[MIC_MODE_CHANNEL]->SetImage(IMAGE_FILE5, 1, 17);
	m_pHelp[MIC_MODE_SERVER]->SetSrcSize(226, 13);
	m_pHelp[MIC_MODE_CHANNEL]->Show();

	//WIID_MICROPHONE_SEND_MSG_BASE
	SPWindowStatic* pStaticWnd = NULL;
	pStaticWnd = new SPWindowStatic(WIID_MICROPHONE_SEND_MSG_BASE, 10 - 4, 54, 217, 56, pCenter);
	pStaticWnd->SetWindowType(WND_TYPE_LEFTBASE);
	pStaticWnd->SetImageNormalHead		(IMAGE_FILE5, 413, 35, 417, 91);
	pStaticWnd->SetImageNormalBody		(IMAGE_FILE5, 419, 35, 421, 91);
	pStaticWnd->SetImageNormalTail		(IMAGE_FILE5, 423, 35, 427, 91);
	pStaticWnd->Show();

	//WIID_MICROPHONE_SEND_MSG
	m_pMsg = new SPWindowEditMultiLine(WIID_MICROPHONE_SEND_MSG, 17 - 10, 62 - 54, 203, 40, pStaticWnd);
	m_pMsg->SetLimitText(LEN_CASHITEM_NORMAL_MSG - 1);
	m_pMsg->SetEnableTabStop(true);
	m_pMsg->Show();
}


void SPWindowMicropone::Process(float fTime)
{
	if(!m_bShow)
		return;
	
	SPWindow::Process(fTime);
}


void SPWindowMicropone::Render(float fTime)
{
	if(!m_bShow)
		return;
	
	SPWindow::Render(fTime);
	
	////
	//RECT rtRect;
	//m_pHelp[MIC_MODE_SERVER]->GetWindowRect(rtRect);
	//g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	//g_pVideo->GetSysLine()->DrawRect(rtRect);
	//g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}


void SPWindowMicropone::Show(bool bWithChild /*= true*/)
{
	SPWindow::Show(bWithChild);
	
	m_pHelp[MIC_MODE_SERVER]->Hide();
	m_pHelp[MIC_MODE_CHANNEL]->Hide();
	
	switch(m_iMode) {
		case MIC_MODE_SERVER:			
			m_pHelp[MIC_MODE_SERVER]->Show();
			break;
		case MIC_MODE_CHANNEL:
			m_pHelp[MIC_MODE_CHANNEL]->Show();
			break;
		case MIC_MODE_ITEM:
			m_pHelp[MIC_MODE_SERVER]->Show();
			break;
		default:
			break;	 
	}

	m_pMsg->SetWindowText(NULL);
	//m_pMsg->SetFocus();	

	m_pOk->SetEnable(true);
	m_pCancel->SetEnable(true);
}


void SPWindowMicropone::Hide(bool bSendServer /*= true*/)
{
	m_pMsg->SetWindowText(NULL);

	if(g_pGOBManager->GetLocalPlayer()) {
		SPPlayerInvenArchive* pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();

		if(pInven) {
			pInven->ClearMicItem();
		}
	}

	OnEditKillFocusRange(0,0);

	SPWindow::Hide(bSendServer);
}



SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowMicropone )
SPIMESSAGE_COMMAND( SPIM_MICROPHONE_MODE,		OnMode )
SPIMESSAGE_COMMAND( SPIM_MICROPHONE_RESULT,		OnResult)
//SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP	,	WIID_MICROPHONE_CLOSE				,	OnClose				)
//SPIMESSAGE_CONTROL( SPIM_SETFOCUS,		WIID_MICROPHONE_SEND_MSG_BASE		,	OnEditFocus			)
SPIMESSAGE_CONTROL( SPIM_SETFOCUS,		WIID_MICROPHONE_SEND_MSG			,	OnEditSetFocusRange	)
SPIMESSAGE_CONTROL( SPIM_KILLFOCUS,		WIID_MICROPHONE_SEND_MSG			,	OnEditKillFocusRange)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP	,	WIID_MICROPHONE_OK					,	OnOk				)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP	,	WIID_MICROPHONE_CANCEL				,	OnCancel			)
SPIMESSAGE_OBJ_MAP_END	( SPWindow	)



int SPWindowMicropone::OnMode(WPARAM wParam, LPARAM lparam)
{
	m_iMode = static_cast<MIC_MODE>(wParam);
	return 1;
}


int SPWindowMicropone::OnResult(WPARAM wParam, LPARAM lparam)
{	
	bool bResult = static_cast<bool>(wParam);
	Hide();
	return 1;
}


int	SPWindowMicropone::OnEditSetFocusRange(WPARAM wParam, LPARAM lParam)
{
	g_pInterfaceManager->SetKeyInput(TRUE);
	return 1;
}


int	SPWindowMicropone::OnEditKillFocusRange(WPARAM wParam, LPARAM lParam)
{
	g_pInterfaceManager->SetKeyInput(FALSE);
	return 1;
}

////
//int SPWindowMicropone::OnClose(WPARAM wParam, LPARAM lparam)
//{
//	Hide();
//	return 1;
//}


int SPWindowMicropone::OnOk(WPARAM wParam, LPARAM lparam)
{
	std::string strMsg = m_pMsg->GetWindowText();

	if(strMsg.empty())
		return 1;

	int iMsgLen = static_cast<int>(strMsg.size());
	
	if(iMsgLen <= 0 || iMsgLen > LEN_CASHITEM_NORMAL_MSG)
		return 1;

	char szBuffer[128];
	int iReplace;
	SPAbuseFilter::GetInstance()->Filter(strMsg.c_str(), szBuffer, 127, &iReplace);
	if(iReplace) {				//°ËÃâ...
		if(g_pResourceManager->GetGlobalString(5007014)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(5007014);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}
		return 1;
	}

	if(g_pGOBManager->GetLocalPlayer()) {
		SPPlayerInvenArchive* pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();

		if(pInven) {
			if (MIC_MODE_ITEM == m_iMode)
			{
				if (pInven->SendBugle(strMsg) == false)
					return 1;
			}
			else
			{
				if(pInven->SendMicItemUseMsg(strMsg) == false)
					return 1;
			}
		}
	}

	m_pOk->SetEnable(false);
	m_pCancel->SetEnable(false);
	
	return 1;
}


int SPWindowMicropone::OnCancel(WPARAM wParam, LPARAM lparam)
{
	Hide();
	return 1;
}


