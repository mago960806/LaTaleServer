#include "SPCommon.h"
#include "SPUtil.h"
#include "SPWindowDEF.h"
#include "SPCommandConvert.h"

#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPWindowTypeWriter.h"

#include "SPWindowPrologue.h"

#include "SPRenderModelDEF.h"
#include "SPRenderModel.h"
#include "SPGOBClusterDef.h"
#include "SPGOBCoordPhysics.h"
#include "SPGOBStatus.h"
#include "SPPlayerStatusModel.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGOBModelUnit.h"
#include "SPAvatarModelUnit.h"

#include "SPGameObject.h"
#include "SPPlayer.h"
#include "SPMonster.h"

#include "SPStoryActor.h"
#include "SPStoryScene.h"
#include "SPMOBCluster.h"
#include "SPSubGameManager.h"
#include "SPPlayerActionManager.h"

#include "Packet.h"

//------------------------------------------------------------------------------------
SPWindowPrologue::SPWindowPrologue(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
, m_pkAniBgnTexture(NULL)
, m_pkLightBgnTexture(NULL)
, m_pkStoryScene(NULL)
, m_pkBlankTexture(NULL)
{
	for( int i = 0; i < 7; i++ )
		m_pkComicTexture[i] = NULL;
	
	Init();
}

//------------------------------------------------------------------------------------
SPWindowPrologue::SPWindowPrologue(INSTANCE_ID InstanceID,
	int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_PROLOGUE, InstanceID, iX, iY, iCX, iCY, pParent)
, m_pkAniBgnTexture(NULL)
, m_pkLightBgnTexture(NULL)
, m_pkStoryScene(NULL)
, m_pkBlankTexture(NULL)
{
	for( int i = 0; i < 7; i++ )
		m_pkComicTexture[i] = NULL;

	Init();
}

//------------------------------------------------------------------------------------
SPWindowPrologue::~SPWindowPrologue()
{
	Clean();
}

//------------------------------------------------------------------------------------
void SPWindowPrologue::Clean()
{
	SPWindow::Clean();
	
	SAFE_DELETE(m_pkStoryScene);
	SAFE_RELEASE(m_pkAniBgnTexture);
	SAFE_RELEASE(m_pkLightBgnTexture);
	SAFE_RELEASE(m_pkBlankTexture);
	for( int i = 0; i < 7; i++ )
	{
		SAFE_RELEASE(m_pkComicTexture[i]);
	}
}

//------------------------------------------------------------------------------------
void SPWindowPrologue::Init()
{
	int i;
	char szBuf[256];

	SPWindowStatic* pkWindow;
	SPWindowButton* pkButton;

	pkWindow = new SPWindowStatic(WIID_PROLOGUE_BUTTON_SKIN, 661, 501, 139, 88, this);
	pkWindow->SetWindowType(WND_TYPE_TOPBASE);
	pkWindow->SetImageNormalHead("DATA/PROLOGUE/UI/P_UI001.PNG", 1, 356, 140, 360);
	pkWindow->SetImageNormalBody("DATA/PROLOGUE/UI/P_UI001.PNG", 1, 360, 140, 364);
	pkWindow->SetImageNormalTail("DATA/PROLOGUE/UI/P_UI001.PNG", 1, 364, 140, 368);
	//pkWindow->SetImageNormalHead("DATA/PROLOGUE/UI/P_UI001.PNG", 1, 371, 140, 375);
	//pkWindow->SetImageNormalBody("DATA/PROLOGUE/UI/P_UI001.PNG", 1, 375, 140, 379);
	//pkWindow->SetImageNormalTail("DATA/PROLOGUE/UI/P_UI001.PNG", 1, 379, 140, 383);

	pkButton = new SPWindowButton(WIID_PROLOGUE_NEXT, 22, 11, 98, 32, pkWindow);
	pkButton->SetImage("DATA/PROLOGUE/UI/P_UI001.PNG", 314, 345);
	pkButton->SetImageHit("DATA/PROLOGUE/UI/P_UI001.PNG", 314, 378);
	pkButton->SetImagePush("DATA/PROLOGUE/UI/P_UI001.PNG", 314, 411);
	pkButton->SetImageDisable("DATA/PROLOGUE/UI/P_UI001.PNG", 314, 444);

	pkButton = new SPWindowButton(WIID_PROLOGUE_SKIP, 22, 48, 98, 32, pkWindow);
	pkButton->SetImage("DATA/PROLOGUE/UI/P_UI001.PNG", 413, 345);
	pkButton->SetImageHit("DATA/PROLOGUE/UI/P_UI001.PNG", 413, 378);
	pkButton->SetImagePush("DATA/PROLOGUE/UI/P_UI001.PNG", 413, 411);
	pkButton->SetImageDisable("DATA/PROLOGUE/UI/P_UI001.PNG", 413, 444);

	//pkButton = new SPWindowButton(WIID_PROLOGUE_NEXT, 22, 11, 98, 32, pkWindow);
	//pkButton->SetImage("DATA/PROLOGUE/UI/P_UI000.PNG", 177, 379);
	//pkButton->SetImageHit("DATA/PROLOGUE/UI/P_UI000.PNG", 177, 412);
	//pkButton->SetImagePush("DATA/PROLOGUE/UI/P_UI000.PNG", 177, 445);
	//pkButton->SetImageDisable("DATA/PROLOGUE/UI/P_UI000.PNG", 177, 478);

	//pkButton = new SPWindowButton(WIID_PROLOGUE_SKIP, 22, 48, 98, 32, pkWindow);
	//pkButton->SetImage("DATA/PROLOGUE/UI/P_UI000.PNG", 276, 379);
	//pkButton->SetImageHit("DATA/PROLOGUE/UI/P_UI000.PNG", 276, 412);
	//pkButton->SetImagePush("DATA/PROLOGUE/UI/P_UI000.PNG", 276, 445);
	//pkButton->SetImageDisable("DATA/PROLOGUE/UI/P_UI000.PNG", 276, 478);

	InitTalkBox();
	InitHelpBox();
	InitScene();

	g_pVideo->GetTextureMgr()->LoadTexture("DATA/PROLOGUE/UI/P_UI000.PNG", &m_pkAniBgnTexture);
	g_pVideo->GetTextureMgr()->LoadTexture("DATA/PROLOGUE/UI/P_UI001.PNG", &m_pkLightBgnTexture);
	for( i = 0; i < 7; i++ )
	{
		sprintf(szBuf, "DATA/PROLOGUE/UI/PROLOGUE%02d.PNG", i + 1);
		g_pVideo->GetTextureMgr()->LoadTexture(szBuf, &m_pkComicTexture[i]);
	}

	g_pVideo->GetTextureMgr()->LoadTexture("DATA/PROLOGUE/UI/P_UI001.PNG", &m_pkBlankTexture);
	m_bBlank = false;
	m_fBlankAccumlateTime = 0.0f;
	SetRect(&m_rcDstBlank, 380, 179, 426, 193);
	SetRect(&m_rcSrcBlank[0], 1, 321, 47, 335);
	SetRect(&m_rcSrcBlank[1], 1, 336, 47, 350);

	m_eStep = PLS_COMIC1_CUT;
	m_iCurComicIndex = 0;
	m_bTalkComplete = false;
	m_bMouseEventComplete = false;

	m_ptTalkTail[0].x = 202;
	m_ptTalkTail[0].y = 134;
	m_ptTalkTail[1].x = 616;
	m_ptTalkTail[1].y = 134;

	m_iCurHelpStringNo = -1;
	m_iCurTalkStringNo = -1;
	m_iCurSceneNo = -1;

	m_fWaitTime = 0.0f;
	m_fWaitAccmulateTime = 0.0f;

	OnSetStep((WPARAM)PLS_COMIC1_CUT, 0);
}

//------------------------------------------------------------------------------------
void SPWindowPrologue::InitTalkBox()
{
	SPWindowStatic* pkWindow, *pkChildWindow, *pkChildWindow2;
	SPWindowTypeWriter* pkWriter;
	char szBuf[128];

	pkWindow = new SPWindowStatic(WIID_PROLOGUE_TALK_BOX, 128, 36, 543, 189, this);
	pkWindow->SetImage("DATA/PROLOGUE/UI/P_UI000.PNG", 0, 321);

	//pkChildWindow = new SPWindowStatic(WIID_PROLOGUE_TALK_LEFT, 10, 10, 9, 128, pkWindow);
	//pkChildWindow->SetImage("DATA/PROLOGUE/UI/P_UI000.PNG", 487, 384);

	//pkChildWindow = new SPWindowStatic(WIID_PROLOGUE_TALK_MID, 19, 10, 762, 128, pkWindow);
	//pkChildWindow->SetImage("DATA/PROLOGUE/UI/P_UI000.PNG", 498, 384);
	//pkChildWindow->SetSrcSize(2, 128);

	pkWriter = new SPWindowTypeWriter(WIID_PROLOGUE_TALK_MSG, 152, 76, 342, 51, pkWindow);

	//pkChildWindow = new SPWindowStatic(WIID_PROLOGUE_TALK_RIGHT, 781, 10, 9, 128, pkWindow);
	//pkChildWindow->SetImage("DATA/PROLOGUE/UI/P_UI000.PNG", 502, 384);

	//pkChildWindow = new SPWindowStatic(WIID_PROLOGUE_TALK_TAIL, 202, 138, 40, 64, pkWindow);
	//pkChildWindow->SetImage("DATA/PROLOGUE/UI/P_UI000.PNG", 438, 384);

	pkChildWindow = new SPWindowStatic(WIID_PROLOGUE_TALK_IMAGE, 25, 24, 128, 128, pkWindow);
	for( int i = 0; i < 5; i++ )
	{
		sprintf(szBuf, "DATA/PROLOGUE/UI/FACE%02d_01.PNG", i + 1);
		pkChildWindow->SetImage(szBuf, 0, 0);
	}
	pkChildWindow2 = new SPWindowStatic(WIID_PROLOGUE_TALK_IMAGE_NAME, 156, 40, 108, 23, pkWindow);
	pkChildWindow2->SetImage("DATA/PROLOGUE/UI/P_UI001.PNG", 48, 321);		// 이리스
	pkChildWindow2->SetImage("DATA/PROLOGUE/UI/P_UI001.PNG", 205, 345);		// 카즈야
	pkChildWindow2->SetImage("DATA/PROLOGUE/UI/P_UI001.PNG", 157, 321);		// 마왕
	pkChildWindow2->SetImage("DATA/PROLOGUE/UI/P_UI001.PNG", 375, 321);		// 무웬
	pkChildWindow2->SetImage("DATA/PROLOGUE/UI/P_UI001.PNG", 266, 321);		// 레비


	pkChildWindow2 = new SPWindowStatic(WIID_PROLOGUE_TALK_LINE, 150, 64, 147, 10, pkWindow);
	pkChildWindow2->SetImage("DATA/PROLOGUE/UI/P_UI001.PNG", 48, 345);

}

//------------------------------------------------------------------------------------
void SPWindowPrologue::InitHelpBox()
{
	SPWindowStatic* pkWindow, *pkChildWindow;
	pkWindow = new SPWindowStatic(WIID_PROLOGUE_HELP_BOX, 150, 457, 238, 38, this);

	pkChildWindow = new SPWindowStatic(WIID_PROLOGUE_HELP_LEFT, 0, 0, 49, 38, pkWindow);
	pkChildWindow->SetImage("DATA/PROLOGUE/UI/P_UI001.PNG", 1, 369);

	pkChildWindow = new SPWindowStatic(WIID_PROLOGUE_HELP_MID, 49, 0, 140, 38, pkWindow);
	pkChildWindow->SetImage("DATA/PROLOGUE/UI/P_UI001.PNG", 52, 369);
	pkChildWindow->SetSrcSize(2, 38);

	pkChildWindow = new SPWindowStatic(WIID_PROLOGUE_HELP_RIGHT, 189, 0, 49, 38, pkWindow);
	pkChildWindow->SetImage("DATA/PROLOGUE/UI/P_UI001.PNG", 56, 369);

	//pkChildWindow = new SPWindowStatic(WIID_PROLOGUE_HELP_LEFT, 0, 0, 49, 38, pkWindow);
	//pkChildWindow->SetImage("DATA/PROLOGUE/UI/P_UI000.PNG", 332, 320);

	//pkChildWindow = new SPWindowStatic(WIID_PROLOGUE_HELP_MID, 49, 0, 140, 38, pkWindow);
	//pkChildWindow->SetImage("DATA/PROLOGUE/UI/P_UI000.PNG", 383, 320);
	//pkChildWindow->SetSrcSize(2, 38);

	//pkChildWindow = new SPWindowStatic(WIID_PROLOGUE_HELP_RIGHT, 189, 0, 49, 38, pkWindow);
	//pkChildWindow->SetImage("DATA/PROLOGUE/UI/P_UI000.PNG", 387, 320);

	pkChildWindow = new SPWindowStatic(WIID_PROLOGUE_HELP_MSG, 44, 0, 150, 38, pkWindow);
	pkChildWindow->SetFontColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	//pkChildWindow->SetFormat(DT_CENTER);
	pkChildWindow->SetMultiLine(true, 2);
}

//------------------------------------------------------------------------------------
void SPWindowPrologue::InitScene()
{
	m_pkStoryScene = new SPStoryScene(this);
	m_pkStoryScene->LoadDefaultLayerLDT("DATA/LDT/PROLOGUE_AVATAR.LDT");
	m_pkStoryScene->LoadSpeekLDT("DATA/LDT/PROLOGUE_MESS.LDT");

	InitActorIris();
	InitActorMonster();
}

//------------------------------------------------------------------------------------
void SPWindowPrologue::InitActorIris()
{
	if( m_pkStoryScene == NULL )
		return;

	ACTOR_ACTION ActionInfo;
	SPStoryActor* pkIris;

	pkIris = new SPStoryActor(1, CLASS_ID_AVATAR, "Iris",
		202.0f, 449.0f, 150.0f, -400.0f, true, 2, m_pkStoryScene);
	if( pkIris == NULL )
		return;

	pkIris->SetRenderOrder(0);
	ACTOR_DEFAULT_LAYER* pLayer = m_pkStoryScene->GetDefaultLayer(4, 2);
	if( pLayer )
		pkIris->ItemSetting(pLayer);

	// #1 Scene
	ZeroMemory(&ActionInfo, sizeof(ACTOR_ACTION));
	ActionInfo.eCompleteType = ACT_SEQ;
	ActionInfo.uiAction = ACTION_ATTACK_SPEEDY;
	ActionInfo.iSeqIndex = GOB_SEQ_ATTACK_NORMAL01;
	ActionInfo.m_fCompleteTime = 3.0f;
	pkIris->AddAction(1, ActionInfo);

	// #2 Scene
	ZeroMemory(&ActionInfo, sizeof(ACTOR_ACTION));
	ActionInfo.eCompleteType = ACT_POS;
	ActionInfo.uiAction = ACTION_RUN_RIGHT;
	ActionInfo.iSeqIndex = GOB_SEQ_RUN;
	ActionInfo.m_fCompleteTime = 3.0f;
	ActionInfo.m_fStrPosX = 202.0f;
	ActionInfo.m_fStrPosY = 449.0f;
	ActionInfo.m_fDstPosX = 382.0f;
	ActionInfo.m_fDstPosY = 449.0f;
	pkIris->AddAction(2, ActionInfo);

	// #3 Scene
	ZeroMemory(&ActionInfo, sizeof(ACTOR_ACTION));
	ActionInfo.eCompleteType = ACT_SEQ;
	ActionInfo.uiAction = ACTION_STOP;
	ActionInfo.iSeqIndex = GOB_SEQ_STAND;
	ActionInfo.m_fCompleteTime = 3.0f;
	pkIris->AddAction(3, ActionInfo);

	// #4 Scene
	ZeroMemory(&ActionInfo, sizeof(ACTOR_ACTION));
	ActionInfo.eCompleteType = ACT_POS;
	ActionInfo.uiAction = ACTION_RUN_LEFT;
	ActionInfo.iSeqIndex = GOB_SEQ_RUN;
	ActionInfo.m_fCompleteTime = 3.0f;
	ActionInfo.m_fStrPosX = 328.0f;
	ActionInfo.m_fStrPosY = 449.0f;
	ActionInfo.m_fDstPosX = 131.0f;
	ActionInfo.m_fDstPosY = 449.0f;
	pkIris->AddAction(4, ActionInfo);

	// #5 Scene
	ZeroMemory(&ActionInfo, sizeof(ACTOR_ACTION));
	ActionInfo.eCompleteType = ACT_SEQ;
	ActionInfo.uiAction = ACTION_STOP;
	ActionInfo.iSeqIndex = GOB_SEQ_STAND;
	ActionInfo.m_fCompleteTime = 3.0f;
	pkIris->AddAction(5, ActionInfo);

	// #6 Scene
	ZeroMemory(&ActionInfo, sizeof(ACTOR_ACTION));
	ActionInfo.eCompleteType = ACT_SEQ;
	ActionInfo.uiAction = ACTION_ATTACK_SPEEDY;
	ActionInfo.iSeqIndex = GOB_SEQ_ATTACK_NORMAL02;
	ActionInfo.m_fCompleteTime = 3.0f;
	pkIris->AddAction(6, ActionInfo);

	// #7 Scene
	ZeroMemory(&ActionInfo, sizeof(ACTOR_ACTION));
	ActionInfo.eCompleteType = ACT_TIME;
	ActionInfo.uiAction = ACTION_GUARD;
	ActionInfo.iSeqIndex = GOB_SEQ_GUARD;
	ActionInfo.m_fCompleteTime = 2.0f;
	pkIris->AddAction(7, ActionInfo);

	// #8 Scene
	ZeroMemory(&ActionInfo, sizeof(ACTOR_ACTION));
	ActionInfo.eCompleteType = ACT_SEQ;
	ActionInfo.uiAction = ACTION_BEATTACKED;
	ActionInfo.iSeqIndex = GOB_SEQ_DAMAGE_NORMAL01;
	ActionInfo.m_fCompleteTime = 3.0f;
	pkIris->AddAction(8, ActionInfo);

	// #9 Scene
	ZeroMemory(&ActionInfo, sizeof(ACTOR_ACTION));
	ActionInfo.eCompleteType = ACT_LOOP;
	ActionInfo.uiAction = ACTION_SIT_DOWN;
	ActionInfo.iSeqIndex = GOB_SEQ_DOWN;
	ActionInfo.m_fCompleteTime = 1.5f;
	pkIris->AddAction(9, ActionInfo);

	// #10 Scene
	ZeroMemory(&ActionInfo, sizeof(ACTOR_ACTION));
	ActionInfo.eCompleteType = ACT_SEQ;
	ActionInfo.uiAction = ACTION_SIT_STAND;
	ActionInfo.iSeqIndex = GOB_SEQ_DOWN_STAND;
	ActionInfo.m_fCompleteTime = 3.0f;
	pkIris->AddAction(10, ActionInfo);

	// #11 Scene
	ZeroMemory(&ActionInfo, sizeof(ACTOR_ACTION));
	ActionInfo.eCompleteType = ACT_SEQ;
	ActionInfo.uiAction = ACTION_CRITICAL_BEATTACKED;
	ActionInfo.iSeqIndex = GOB_SEQ_DAMAGE_CRITICAL;
	ActionInfo.m_fCompleteTime = 3.0f;
	pkIris->AddAction(11, ActionInfo);

	// #12 Scene
	ZeroMemory(&ActionInfo, sizeof(ACTOR_ACTION));
	ActionInfo.eCompleteType = ACT_SEQ;
	ActionInfo.uiAction = ACTION_ATTACK_SPEEDY;
	ActionInfo.iSeqIndex = GOB_SEQ_ATTACK_NORMAL03;
	ActionInfo.m_fCompleteTime = 3.0f;
	pkIris->AddAction(12, ActionInfo);
	
	m_pkStoryScene->AddActor(pkIris);
}

//------------------------------------------------------------------------------------
void SPWindowPrologue::InitActorMonster()
{
	if( m_pkStoryScene == NULL )
		return;

	ACTOR_ACTION ActionInfo;
	SPStoryActor* pkMonster;
	CLASS_ID iClassID = (CLASS_ID)811000000;

	g_pMOBCluster->PreTextureLoadingbyID(iClassID);
	pkMonster = new SPStoryActor(5, iClassID, "Monster",
		616.0f, 449.0f, 100.0f, -400.0f, false, 1, m_pkStoryScene);
	if( pkMonster == NULL )
		return;

	pkMonster->SetRenderOrder(0);
	//// #7 Scene
	//ZeroMemory(&ActionInfo, sizeof(ACTOR_ACTION));
	//ActionInfo.eCompleteType = ACT_SEQ;
	//ActionInfo.uiAction = ACTION_ATTACK_SPEEDY;
	//ActionInfo.iSeqIndex = ANI_MONSTER_SKILL01;
	//ActionInfo.m_fCompleteTime = 3.0f;
	//pkMonster->AddAction(7, ActionInfo);

	// #11 Scene
	ZeroMemory(&ActionInfo, sizeof(ACTOR_ACTION));
	ActionInfo.eCompleteType = ACT_SEQ;
	ActionInfo.uiAction = ACTION_ATTACK_SPEEDY;
	ActionInfo.iSeqIndex = ANI_MONSTER_SKILL02;
	ActionInfo.m_fCompleteTime = 3.0f;
	pkMonster->AddAction(11, ActionInfo);

	// #12 Scene
	ZeroMemory(&ActionInfo, sizeof(ACTOR_ACTION));
	ActionInfo.eCompleteType = ACT_SEQ;
	ActionInfo.uiAction = ACTION_BEATTACKED;
	ActionInfo.iSeqIndex = 0;
	ActionInfo.m_fCompleteTime = 3.0f;
	pkMonster->AddAction(12, ActionInfo);

	m_pkStoryScene->AddActor(pkMonster);
}

//------------------------------------------------------------------------------------
void SPWindowPrologue::SetWaitTime()
{
	m_fWaitAccmulateTime = 0.0f;
	m_fWaitTime = 0.0f;
	
	switch( m_iCurSceneNo )
	{
	case 1:
		m_fWaitTime = 1.0f;
		break;
	case 6:
		m_fWaitTime = 1.0f;
		break;
	//case 8:
	//	m_fWaitTime = 1.0f;
	//	break;
	case 10:
		m_fWaitTime = 1.0f;
		break;
	case 11:
		m_fWaitTime = 1.0f;
		break;
	case 12:
		m_fWaitTime = 1.0f;
		break;
	case 2:
	case 3:
	case 4:
	case 5:
	case 7:
	case 8:
	case 9:
		m_fWaitTime = 0.1f;
		break;
	}
}

//------------------------------------------------------------------------------------
void SPWindowPrologue::SetNextButton(bool bEnable)
{
	SPWindow* pkWindow;
	pkWindow = Find(WIID_PROLOGUE_NEXT, true);
	if( pkWindow == NULL )
		return;

	pkWindow->SPSendMessage(SPIM_BTN_ENABLE, (WPARAM)bEnable);
}

//------------------------------------------------------------------------------------
void SPWindowPrologue::SetSkipButton(bool bEnable)
{
	SPWindow* pkWindow;
	pkWindow = Find(WIID_PROLOGUE_SKIP, true);
	if( pkWindow == NULL )
		return;

	pkWindow->SPSendMessage(SPIM_BTN_ENABLE, (WPARAM)bEnable);
}

//------------------------------------------------------------------------------------
void SPWindowPrologue::SetTalkBoxShow(bool bShow)
{
	SPWindow* pkWindow;
	pkWindow = Find(WIID_PROLOGUE_TALK_BOX, true);
	if( pkWindow == NULL )
		return;

	if( bShow )
		pkWindow->Show();
	else
		pkWindow->Hide();
}

//------------------------------------------------------------------------------------
void SPWindowPrologue::SetTalkBox(int iNo, int iTailPos)
{
	if( m_pkStoryScene == NULL )
		return;

	m_iCurTalkStringNo = iNo;
	ACTOR_SPEAK* pSpeak = m_pkStoryScene->GetActorSpeak(iNo);
	if( pSpeak )
	{
		SPWindow* pkWindow;
		pkWindow = Find(WIID_PROLOGUE_TALK_IMAGE, true);
		if( pkWindow )
		{
			if( pSpeak->iImage > 0 && pSpeak->iImage < 6 ) {
				pkWindow->SPSendMessage(SPIM_SET_IMAGEINDEX, pSpeak->iImage - 1);
				pkWindow = Find(WIID_PROLOGUE_TALK_IMAGE_NAME, true);
				if( pSpeak->iImage > 0 && pSpeak->iImage < 6 ) {
					pkWindow->SPSendMessage(SPIM_SET_IMAGEINDEX, pSpeak->iImage - 1);
				}
			}
		}

		pkWindow = Find(WIID_PROLOGUE_TALK_MSG, true);
		if( pkWindow )
		{
			pkWindow->SetWindowText(pSpeak->strTalkMessage.c_str());
			pkWindow->SPSendMessage( SPIM_TYPE_START, 0,0 ); // Type ½AAU
			m_bTalkComplete = false;
			m_bMouseEventComplete = false;
		}

		//pkWindow = Find(WIID_PROLOGUE_TALK_TAIL, true);
		//if( pkWindow )
		//{
		//	if( iTailPos )
		//	{
		//		pkWindow->AdjustCoord(m_ptTalkTail[iTailPos - 1].x, m_ptTalkTail[iTailPos - 1].y);
		//		pkWindow->SPSendMessage(SPIM_POS_UPDATE);
		//		pkWindow->Show();
		//	}
		//	else
		//		pkWindow->Hide();
		//}
	}
}

//------------------------------------------------------------------------------------
void SPWindowPrologue::SetHelpBoxShow(bool bShow)
{
	SPWindow* pkWindow;
	pkWindow = Find(WIID_PROLOGUE_HELP_BOX, true);
	if( pkWindow == NULL )
		return;

	if( bShow )
		pkWindow->Show();
	else
		pkWindow->Hide();
}

//------------------------------------------------------------------------------------
void SPWindowPrologue::SetHelpBox(int iNo)
{
	m_iCurHelpStringNo = iNo;
	SPWindow* pkWindow;
	pkWindow = Find(WIID_PROLOGUE_HELP_MSG, true);
	if( pkWindow == NULL )
		return;

	ACTOR_SPEAK* pSpeak = m_pkStoryScene->GetActorSpeak(iNo);
	if( pSpeak )
		pkWindow->SetWindowText(pSpeak->strHelpMessage.c_str());
}

//------------------------------------------------------------------------------------
void SPWindowPrologue::SetDamageFX(const char* pszActor, int iDamage, int iCritical)
{
	if( m_pkStoryScene == NULL )
		return;

	CPacket kPacket;
	kPacket.AddUINT32(iDamage);
	kPacket.AddUINT32(iCritical);

	m_pkStoryScene->SendGOBMessage(pszActor, SPGM_SET_SC_DAMAGE_FX, (LPARAM)&kPacket);
}

//------------------------------------------------------------------------------------
void SPWindowPrologue::SetBGM()
{
	string strBGMName;
	switch( m_eStep )
	{
	case PLS_COMIC1_CUT:
	case PLS_COMIC2_CUT:
		strBGMName = "DATA/BGM/PROLOGUE1.mp3";
		break;
	case PLS_ANIMATION_CUT:
		strBGMName = "DATA/BGM/PROLOGUE2.mp3";
		break;
	}

	if( g_pSubGameManager && !strBGMName.empty() )
		g_pSubGameManager->SPPerformMessage(SPGM_SUG_SETBGM , (WPARAM)strBGMName.c_str());
}

//------------------------------------------------------------------------------------
void SPWindowPrologue::SetFacing(const char* pszActor, int iFacing)
{
	if( m_pkStoryScene == NULL )
		return;

	m_pkStoryScene->SendGOBMessage(pszActor, SPGM_SETFACING, iFacing);
}

//------------------------------------------------------------------------------------
bool SPWindowPrologue::CheckWaitTime(float fTime)
{
	if( m_fWaitTime == 0.0f )
		return false;

	m_fWaitAccmulateTime += fTime;
	if( m_fWaitAccmulateTime >= m_fWaitTime )
	{
		switch( m_iCurSceneNo )
		{
		case 1:
			SetTalkBoxShow(true);
			SetTalkBox(4, 1);
			break;
		case 2:
			m_pkStoryScene->StartScene(3);
			break;
		case 3:
			SetHelpBoxShow(true);
			SetHelpBox(7);
			break;
		case 4:
			m_pkStoryScene->StartScene(5);
			break;
		case 5:
			SetFacing("Iris", 2);
			SetTalkBoxShow(true);
			SetTalkBox(8, 1);
			break;
		case 6:
			SetTalkBoxShow(true);
			SetTalkBox(10, 1);
			break;
		case 7:
			m_pkStoryScene->StartScene(8);
			break;
		case 8:
			SetTalkBoxShow(true);
			SetTalkBox(13, 1);
			m_pkStoryScene->StartScene(9);
			break;
		//case 9:
		//	m_pkStoryScene->StartScene(10);
		//	break;
		case 10:
			SetTalkBoxShow(true);
			SetTalkBox(17, 1);
			break;
		case 11:
			SetTalkBoxShow(true);
			SetTalkBox(19, 0);
			break;
		case 12:
			SetTalkBoxShow(true);
			SetTalkBox(24, 1);
			break;
		}

		m_iCurSceneNo = -1;
		SetWaitTime();
	}

	return true;

}

//------------------------------------------------------------------------------------
bool SPWindowPrologue::CheckHelpKeyValue(SPInputStruct* pInput)
{
	if( pInput == NULL || m_iCurHelpStringNo < 0  )
		return false;
	
	switch( m_iCurHelpStringNo )
	{
	case 3:
		if( pInput->ikZ.InputState == INPUT_PRESS )
		{
			SetHelpBoxShow(false);
			m_iCurHelpStringNo = -1;

			m_pkStoryScene->StartScene(1);
		}
		break;
	case 6:
		if( m_uiAction & ACTION_RUN_RIGHT )
		{
			SetHelpBoxShow(false);
			SetTalkBoxShow(false);
			m_iCurHelpStringNo = -1;

			m_pkStoryScene->StartScene(2);
		}
		break;
	case 7:
		if( m_uiAction & ACTION_RUN_LEFT )
		{
			SetHelpBoxShow(false);
			SetTalkBoxShow(false);
			m_iCurHelpStringNo = -1;

			m_pkStoryScene->StartScene(4);
		}
		break;
	case 9:
		if( pInput->ikX.InputState == INPUT_PRESS )
		{
			SetHelpBoxShow(false);
			SetTalkBoxShow(false);
			m_iCurHelpStringNo = -1;

			m_pkStoryScene->StartScene(6);
		}
		break;
	case 12:
		if( pInput->ikArrowDown.InputState == INPUT_PRESS )
		{
			SetHelpBoxShow(false);
			SetTalkBoxShow(false);
			m_iCurHelpStringNo = -1;

			m_pkStoryScene->StartScene(7);
		}
		break;
	case 16:
		if( pInput->ikCtrl.InputState == INPUT_PRESS )
		{
			SetHelpBoxShow(false);
			SetTalkBoxShow(false);
			m_iCurHelpStringNo = -1;

			m_pkStoryScene->StartScene(10);
		}
		break;
	case 23:
		if( pInput->ikSpaceBar.InputState == INPUT_PRESS )
		{
			SetHelpBoxShow(false);
			SetTalkBoxShow(false);
			m_iCurHelpStringNo = -1;

			m_pkStoryScene->StartScene(12);
		}
		break;
	}

	return true;
}

//------------------------------------------------------------------------------------
bool SPWindowPrologue::CheckTalkKeyValue(SPInputStruct* pInput)
{
	if( pInput == NULL || m_iCurTalkStringNo < 0 || !m_bTalkComplete )
		return false;

	bool bIsPushKey = false;
	if( m_bMouseEventComplete || pInput->ikReturn.InputState == INPUT_PRESS )
		 bIsPushKey = true;

	if( !bIsPushKey )
		return false;

	m_bTalkComplete = false;
	switch( m_iCurTalkStringNo )
	{
	case 1:
		SetTalkBox(2, 1);
		break;
	case 2:
		SetTalkBoxShow(false);
		SetHelpBoxShow(true);
		SetHelpBox(3);
		m_iCurTalkStringNo = -1;
		break;
	case 4:
		SetTalkBox(5, 0);
		break;
	case 5:
		SetTalkBoxShow(false);
		SetHelpBoxShow(true);
		SetHelpBox(6);
		m_iCurTalkStringNo = -1;
		break;
	case 8:
		SetTalkBoxShow(false);
		SetHelpBoxShow(true);
		SetHelpBox(9);
		m_iCurTalkStringNo = -1;
		break;
	case 10:
		SetTalkBox(11, 2);
		break;
	case 11:
		SetTalkBoxShow(false);
		SetHelpBoxShow(true);
		SetHelpBox(12);
		m_iCurTalkStringNo = -1;
		break;
	case 13:
		SetTalkBox(14, 2);
		break;
	case 14:
		SetTalkBox(15, 0);
		break;
	case 15:
		SetTalkBoxShow(false);
		SetHelpBoxShow(true);
		SetHelpBox(16);
		m_iCurTalkStringNo = -1;
		break;
	case 17:
		SetTalkBox(18, 2);
		break;
	case 18:
		SetTalkBoxShow(false);
		SetHelpBoxShow(false);
		m_iCurTalkStringNo = -1;
		m_pkStoryScene->StartScene(11);
		break;
	case 19:
		SetTalkBox(20, 0);
		break;
	case 20:
		SetTalkBox(21, 0);
		break;
	case 21:
		SetTalkBox(22, 1);
		break;
	case 22:
		SetTalkBoxShow(false);
		SetHelpBoxShow(true);
		SetHelpBox(23);
		m_iCurTalkStringNo = -1;
		break;
	case 24:
		SetTalkBox(25, 2);
		break;
	case 25:
		SetTalkBox(26, 1);
		break;
	case 26:
		SetTalkBox(27, 2);
		break;
	case 27:
		SetTalkBoxShow(false);
		SetHelpBoxShow(true);
		SetHelpBox(28);
		m_iCurTalkStringNo = -1;
		SetNextButton(true);
		break;
	}

	m_bMouseEventComplete = false;
	return true;
}

//------------------------------------------------------------------------------------
void SPWindowPrologue::ProcessScene(float fTime)
{
	if( m_eStep != PLS_ANIMATION_CUT || m_pkStoryScene == NULL )
		return;

	m_pkStoryScene->Process(fTime);
	
	if( !CheckWaitTime(fTime) )
	{
		SPInputStruct* pInput = g_pInputManager->GetInputStruct();
		CheckHelpKeyValue(pInput);
		CheckTalkKeyValue(pInput);
	}
}

//------------------------------------------------------------------------------------
void SPWindowPrologue::ProcessBlank(float fTime)
{
	if( m_eStep != PLS_ANIMATION_CUT )
		return;

	if( m_iCurTalkStringNo < 0 || !m_bTalkComplete )
		return;

	m_fBlankAccumlateTime += fTime;
	if( m_fBlankAccumlateTime > 0.5f )
	{
		m_bBlank = !m_bBlank;
		m_fBlankAccumlateTime = 0.0f;
	}
}

//------------------------------------------------------------------------------------
void SPWindowPrologue::Process(float fTime)
{
	if( !m_bShow )
		return;

	SPWindow::Process(fTime);
	ProcessScene(fTime);
	ProcessBlank(fTime);

	SPPlayerActionManager*	pkActionManager = SPPlayerActionManager::GetInstance();
	pkActionManager->Process(fTime);
	m_uiAction = pkActionManager->GetAction();

	pkActionManager->SetAction(ACTION_STOP);
	pkActionManager->SetProcessSkip(FALSE);
}

//------------------------------------------------------------------------------------
void SPWindowPrologue::Render(float fTime)
{
	if( !m_bShow )
		return;
	
	RenderBackground();
	RenderScene();
	SPWindow::Render(fTime);
	RenderBlank();
}

//------------------------------------------------------------------------------------
void SPWindowPrologue::Show(bool bWithChild)
{
	SPWindow::Show(bWithChild);
	SetTalkBoxShow(false);
	SetHelpBoxShow(false);
}

//------------------------------------------------------------------------------------
void SPWindowPrologue::RenderBackground()
{
	switch( m_eStep )
	{
	case PLS_COMIC1_CUT:
	case PLS_COMIC2_CUT:
		{
			RECT rcDst, rcSrc;
			SetRect(&rcDst, 0, 0, 800, 600);
			SetRect(&rcSrc, 0, 0, 800, 600);

			if( m_iCurComicIndex < 7 && m_pkComicTexture[m_iCurComicIndex] )
				m_pkComicTexture[m_iCurComicIndex]->RenderTexture(&rcDst, &rcSrc);
		}
		break;
	case PLS_ANIMATION_CUT:
		{
			RECT rcDst, rcSrc;
			SetRect(&rcDst, 0, 0, 800, 600);
			SetRect(&rcSrc, 0, 0, 512, 320);

			if( m_pkAniBgnTexture )
				m_pkAniBgnTexture->RenderTexture(&rcDst, &rcSrc);
			
			if( m_pkLightBgnTexture )
			{
				g_pVideo->SetAlphaTexture(true);
				m_pkLightBgnTexture->RenderTexture(&rcDst, &rcSrc);
				g_pVideo->SetAlphaTexture(false);
			}
		}
		break;	
	case PLS_COMPLETE:
		{
			RECT rcDst, rcSrc;
			SetRect(&rcDst, 0, 0, 800, 600);
			SetRect(&rcSrc, 0, 0, 800, 600);

			if( m_iCurComicIndex == 6 && m_pkComicTexture[m_iCurComicIndex] )
				m_pkComicTexture[m_iCurComicIndex]->RenderTexture(&rcDst, &rcSrc);
		}
		break;
	}

	//g_pVideo->Flush();
}

//------------------------------------------------------------------------------------
void SPWindowPrologue::RenderScene()
{
	if( m_eStep != PLS_ANIMATION_CUT || m_pkStoryScene == NULL )
		return;

	m_pkStoryScene->Render(0.0f);
}

//------------------------------------------------------------------------------------
void SPWindowPrologue::RenderBlank()
{
	if( m_eStep != PLS_ANIMATION_CUT )
		return;
	
	if( m_iCurTalkStringNo < 0 || !m_bTalkComplete )
		return;

	if( m_bBlank )
		m_pkBlankTexture->RenderTexture(&m_rcDstBlank, &m_rcSrcBlank[1]);
	else
		m_pkBlankTexture->RenderTexture(&m_rcDstBlank, &m_rcSrcBlank[0]);
}

//------------------------------------------------------------------------------------
int SPWindowPrologue::OnSkip(WPARAM wParam, LPARAM lParam)
{
	OnSetStep((WPARAM)PLS_COMPLETE, 0);
	//OnSetStep((WPARAM)PLS_ANIMATION_CUT, 0);
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowPrologue::OnNext(WPARAM wParam, LPARAM lParam)
{
	if( m_eStep == PLS_COMPLETE )
	{
	}
	else if( m_eStep == PLS_ANIMATION_CUT )
	{
		OnSetStep((WPARAM)PLS_COMIC2_CUT, 0);
	}
	else
	{
		m_iCurComicIndex++;
		if( m_iCurComicIndex == 7 )
		{
			OnSetStep((WPARAM)PLS_COMPLETE, 0);
		}
		else if( m_iCurComicIndex == 3 )
		{
			OnSetStep((WPARAM)PLS_ANIMATION_CUT, 0);
		}
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowPrologue::OnSetStep(WPARAM wParam, LPARAM lParam)
{
	m_eStep = (PROLOGUE_STEP)wParam;
	switch( m_eStep )
	{
	case PLS_COMIC1_CUT:
		m_iCurComicIndex = 0;
		SetNextButton(true);
		SetTalkBoxShow(false);
		SetHelpBoxShow(false);
		break;
	case PLS_COMIC2_CUT:
		m_iCurComicIndex = 3;
		SetNextButton(true);
		SetTalkBoxShow(false);
		SetHelpBoxShow(false);
		break;
	case PLS_ANIMATION_CUT:
		SetNextButton(false);
		SetTalkBoxShow(true);
		SetHelpBoxShow(false);
		SetTalkBox(1, 1);
		break;
	case PLS_COMPLETE:
		m_iCurComicIndex = 6;
		SetNextButton(false);
		SetSkipButton(false);
		SetTalkBoxShow(false);
		SetHelpBoxShow(false);
		if( g_pSubGameManager )
			g_pSubGameManager->SPPerformMessage(SPGM_SUG_PROLOGUE_COMPLETE, wParam, lParam);
		break;
	}

	SetBGM();
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowPrologue::OnActorSceneComplete(WPARAM wParam, LPARAM lParam)
{
	const char* pszActorName = (const char*)wParam;
	int iSceneNo = lParam;
	
	if( pszActorName == NULL || iSceneNo == 0 )
		return 1;
	
	m_iCurSceneNo = iSceneNo;
	if( strcmp(pszActorName, "Iris") == 0 )
	{
		switch( iSceneNo )
		{
		case 1:
			SetDamageFX("Monster", 1, 0);
			SetWaitTime();
			break;
		case 6:
			SetDamageFX("Monster", 1, 0);
			SetWaitTime();
			break;
		case 8:
			SetDamageFX("Iris", 999, 0);
			SetWaitTime();
			break;
		case 11:
			SetDamageFX("Iris", 999, 0);
			SetWaitTime();
			m_pkStoryScene->SetRenderOrder("Iris", 1);
			m_pkStoryScene->SetRenderOrder("Monster", 0);
			break;
		//case 12:
		//	SetDamageFX("Monster", 9999, 0);
			//SetWaitTime();
			break;
		case 7:
		case 2:
		case 3:
		case 4:
		case 5:
		case 9:
		case 10:
			SetWaitTime();
			break;
		}
	}
	else if( strcmp(pszActorName, "Monster") == 0 )
	{
		switch( iSceneNo )
		{
		//case 7:
		//	SetDamageFX("Iris", 999, 0);
		//	m_pkStoryScene->StartScene(8);
		//	break;
		//case 11:
		//	SetDamageFX("Iris", 999, 0);
		//	m_pkStoryScene->StartScene(12);
		//	break;
		case 12:
			SetDamageFX("Monster", 9999, 0);
			SetWaitTime();
			break;
		}
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowPrologue::OnTalkMsgComplete(WPARAM wParam, LPARAM lParam)
{
	if( m_iCurTalkStringNo < 0  )
		return 1;

	m_bTalkComplete = true;
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowPrologue::OnTalkMsgLButtonUp(WPARAM wParam, LPARAM lParam)
{
	if( m_iCurTalkStringNo < 0  )
		return 1;

	if( m_bTalkComplete )
		m_bMouseEventComplete = true;
	else
	{
		SPWindow* pkWindow = Find(WIID_PROLOGUE_TALK_MSG, true);
		if( pkWindow )
			pkWindow->SPSendMessage( SPIM_TYPE_SKIP, 0,0 );
	}

	return 1;
}

//------------------------------------------------------------------------------------
SPIMESSAGE_OBJ_MAP_BEGIN(SPWindowPrologue)

SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_PROLOGUE_SKIP,		OnSkip )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_PROLOGUE_NEXT,		OnNext )
SPIMESSAGE_CONTROL( SPIM_TYPE_END,		WIID_PROLOGUE_TALK_MSG,	OnTalkMsgComplete )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_PROLOGUE_TALK_MSG,	OnTalkMsgLButtonUp )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_PROLOGUE_TALK_BOX,	OnTalkMsgLButtonUp )

SPIMESSAGE_COMMAND( SPIM_PROLOGUE_SET_STEP,		OnSetStep )
SPIMESSAGE_COMMAND( SPIM_ACTOR_SCENE_COMPLETE,	OnActorSceneComplete )

SPIMESSAGE_OBJ_MAP_END(SPWindow)
