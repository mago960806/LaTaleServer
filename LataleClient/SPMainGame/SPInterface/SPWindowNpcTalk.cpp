
#include <vector>
#include <string>

#include "SPCommon.h"
#include "SPUtil.h"

#include "SPMainGameDEF.H"
#include "SPGameObjectDEF.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"

//
#include "SPMainGameDEF.H"
#include "SPSubGameManager.h"
#include "SPMainGameManager.h"
#include "SPMouseCursor.h"
#include "SPWindowDEF.h"
#include "SPInterfaceManager.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGOBModel.h"
#include "SPGOBModelUnit.h"
#include "SPMonsterModelUnit.h"
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
#include "SPEventStatusModel.h"
#include "SPPlayerEquipInfo.h"
#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPGuildArchive.h"
#include "SPPlayer.h"
#include "SPGOBManager.h"

#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPSubGameManager.h"
#include "SPGameObject.h"
#include "SPManager.h"
#include "SPMouseCursor.h"
#include "SPIMEPool.h"
#include "SPWindowDEF.h"
#include "SPInterfaceManager.h"

#include "Packet.h"

#include "SPStage.h"						//[2005/6/10] - jinhee
#include "SPStageManager.h"

#include "SPWindow.h"
#include "SPWindowButton.h"
#include "SPWindowStatic.h"
#include "SPWindowTypeWriter.h"

#include "SPEventDEF.h"
#include "SPEventNpcHandler.h"
#include "SPCoreMissionAttr.h"
#include "SPCoreMissionArchive.h"
#include "SPCoreEventManager.h"
#include "SPClassChangeManager.h"
#include "SPEventManager.h"

#include "SPMainGameManager.h"
#include "SPStoryManager.h"
#include "SPWayPointManager.h"

#include "SPWindowNpcTalk.h"
#include "SPLocalizeManager.h"

#include "SPUtil.h"

#include "SPDebug.h"



SPWindowNpcTalk::SPWindowNpcTalk(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent /*= NULL*/)
: SPWindow(WNDID_NPCTALK, InstanceID, iX, iY, iCX, iCY, pParent)
{
	Init();
}


SPWindowNpcTalk::~SPWindowNpcTalk()
{
	Clean();
}

void SPWindowNpcTalk::Init()
{	
	m_pNpcTalk = NULL;
	m_pSavePoint = NULL;

	m_strMessage = "";
	
	InitSubControl();

	ZeroMemory(m_szMsg, 512);

	m_iTalkState = TALK_STATE_NULL;	

	for(int i = 0; i < NPC_TALK_WINDOW_MODE_MAX; i++) {
		m_pWindowSkin[i] = NULL;
	}

	std::string strTextureFile = "";
	strTextureFile = "DATA/INTERFACE/CONCEPT/CUTUI001.PNG";
	g_pVideo->GetTextureMgr()->LoadTexture(strTextureFile.c_str(), &m_pWindowSkin[NPC_TALK_WINDOW_MODE_NORMAL]);

	strTextureFile = "DATA/INTERFACE/CONCEPT/CUTUI002.PNG";
	g_pVideo->GetTextureMgr()->LoadTexture(strTextureFile.c_str(), &m_pWindowSkin[NPC_TALK_WINDOW_MODE_OUTBURST]);

	SetRect(&m_rtSkinSrc[NPC_TALK_WINDOW_MODE_NORMAL], 0, 0, 512, 311);
	SetRect(&m_rtSkinSrc[NPC_TALK_WINDOW_MODE_OUTBURST], 0, 0, 512, 311);	
	
	GetWindowRect(m_rtSkinBaseDest);	
	m_iEndHeight	= m_iSY;
	m_iEndWidth		= m_iSX;

	m_fAccumulateRender		= 0.0f;
	m_fCurDelay				= 0.01f;

	SetRect(&m_rtAttachSkinDest, m_iAX + (173 - m_iAX), m_iAY + (199 - m_iAY), (m_iAX + (173 - m_iAX)) + 180, (m_iAY + (199 - m_iAY)) + 200);
	SetRect(&m_rtAttachSkinSrc, 0, 312, 180, 512);	
	
	g_pResourceManager->GetLDTFromFileName( RES_FNAME_LDT_EVENT_ATTACH_IMAGE , &m_pAttachImageLDTFile);	
	if( m_pAttachImageLDTFile == NULL )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "----------------------------------->CORE EVENT ATTACH IMAGE LDT FILE LOAD FAIL [%s] \n" , RES_FNAME_LDT_EVENT_ATTACH_IMAGE );
#endif		
	}
	m_pAttachImage			= NULL;
	SetRect(&m_rtAttachDest, m_rtAttachSkinDest.left + 26, m_rtAttachSkinDest.top + 36, m_rtAttachSkinDest.left + 26 + 128, m_rtAttachSkinDest.top + 36 + 128);
	SetRect(&m_rtAttachSrc, 0, 0, 0, 0);
	Hide();


}


void SPWindowNpcTalk::InitSubControl()
{
	SPWindowStatic* pParent = NULL;
	SPWindowStatic* pWindow = NULL;
	SPWindowButton* pButton = NULL;

	char* IMAGE_FILE1 = "DATA/INTERFACE/CONCEPT/CUTUI001.PNG";
	char* IMAGE_FILE2 = "DATA/INTERFACE/CONCEPT/CUTUI002.PNG";		

	//WIID_NPCTALK_NPCNAME	
	pWindow = new SPWindowStatic(WIID_NPCTALK_NPCNAME, 120, 56, 263, 22, this);
	pWindow->SetFont(FONT_14_BOLD);
	pWindow->SetFontColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	pWindow->SetFontShadowColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	pWindow->SetFontShadow(true);
	pWindow->SetFormat(DT_LEFT | DT_VCENTER);
    pWindow->SetWindowText("Npc Name...");
	pWindow->Hide();
	m_pNpcName = pWindow;
	
	//WIID_NPCTALK_LINE	
	pWindow = new SPWindowStatic(WIID_NPCTALK_LINE, 106, 82, 147, 10, this);
	pWindow->SetImage(IMAGE_FILE2, 181, 329);
	pWindow->Hide();

	//WIID_NPCTALK_CLOSE
	pButton = new SPWindowButton(WIID_NPCTALK_CLOSE , 391, 57, 34, 34, this);
	pButton->SetImage		(IMAGE_FILE1, 359,  312);
	pButton->SetImageHit	(IMAGE_FILE1, 359,  347);
	pButton->SetImagePush	(IMAGE_FILE1, 359,  382);
	pButton->SetImageDisable(IMAGE_FILE1, 359,  417);
	m_pClose = pButton;
	pButton->Show();
	
	//WIID_NPCTALK_MESSAGE
	m_pTypeWriter = new SPWindowTypeWriter(WIID_NPCTALK_MESSAGE, 112, 98, 313, 98, this);

	m_bTypeWriteEnd = false;

	//WIID_NPCTALK_SKIN
	pWindow = new SPWindowStatic(WIID_NPCTALK_SKIN, 0, 0, 512, 311, this);		
	pWindow->Hide();
	m_pParent = pWindow;
}


void SPWindowNpcTalk::Clean()
{
	SAFE_RELEASE(m_pAttachImage);
	SAFE_RELEASE(m_pAttachImageLDTFile);
	SAFE_RELEASE(m_pFocusLoseSkin);
	SAFE_RELEASE(m_pNextSkin);
	for(int i = NPC_TALK_WINDOW_MODE_NORMAL; i < NPC_TALK_WINDOW_MODE_MAX; i++) {
		SAFE_RELEASE(m_pWindowSkin[i]);
	}
	SPWindow::Clean();
}


void SPWindowNpcTalk::Show(bool bWithChild)
{	
	m_iTalkFadeType = TALK_FADE_IN;
	m_iTalkWindowMode = NPC_TALK_WINDOW_MODE_NORMAL;
	m_bFocusLose = false;
	m_bShowAttach = false;
	if(m_pAttachImage)
		SAFE_RELEASE(m_pAttachImage);
	m_bShowNext = false;	
	
	m_fFadeAlpha = 0.0f;
	m_fFadeAlphaUnit = 0.07f;
	
	m_fAccumulateRender = 0.0f;
	m_iMoveSkinHeight = 0;
	m_iMoveSkinWidth = 0;
	m_iSkinMoveX = 64;
	m_iSkinMoveY = 40;
	m_iSkinMoveUnitX = 1;
	m_iSkinMoveUnitY = 1;
	SetRect(&m_rtSkinDest, m_rtSkinBaseDest.right - m_iMoveSkinWidth, m_rtSkinBaseDest.bottom - m_iMoveSkinHeight, m_rtSkinBaseDest.right, m_rtSkinBaseDest.bottom);	
	
	return;
}


void SPWindowNpcTalk::ShowComplete()
{
	//[2008/7/17] - jinssaga 단축키 입력으로 스킵되어 들어오는 경우 여기서 리턴해준다
	if(g_pEventManager->GetCutInState() == CUT_IN_STATE_NPC_AUCTION)
		return;
	
	SPWindow::Show(true);

	//
	if(g_pEventManager->GetCutInState() != CUT_IN_STATE_NULL){
		if(g_pEventManager->GetCutInState() < CUT_IN_STATE_TALK) {
			if(g_pEventManager->GetEventHandler()->GetNpcID() == 0){		//Save Point 의 겨우
				if(g_pEventManager->GetCutInState() != CUT_IN_STATE_SAVEPOINT)	
					g_pEventManager->SetCutInState(CUT_IN_STATE_SAVEPOINT);	
				
				m_iTalkState = TALK_STATE_TALK;

				//g_pSubGameManager->SPPerformMessage(SPGM_SUG_SETBGM , (WPARAM)m_pSavePoint->m_strBGM.c_str());
			}
		}
		
		//강제이벤트인 경우
		if(g_pEventManager->GetCutInEvnetType() == EVENT_CORE) {
			m_pClose->Hide();
		}
		else {
			m_pClose->Show();
		}
	}

	m_pTypeWriter->SPSendMessage(SPIM_TYPE_START, 0,0);
	m_bTypeWriteEnd = false;
	g_pEventManager->GetEventHandler()->SetEventClick(EVENT_CLICK_NPCTALK);
}


void SPWindowNpcTalk::Hide(bool bSendServer /*= true*/)
{
	if(m_bShow) {
		SPWindow::Hide(bSendServer);

		if(m_pAttachImage)
			SAFE_RELEASE(m_pAttachImage);
	}
	
	if(m_iTalkFadeType == TALK_FADE_STOP){
		return;
	}
	
	m_iTalkFadeType = TALK_FADE_OUT;
	return;	
}



void SPWindowNpcTalk::Process(float fTime)
{	
	if(g_pEventManager->GetCutInState() > CUT_IN_STATE_NULL) {
		if(m_iTalkFadeType != TALK_FADE_STOP) {
			m_fAccumulateRender += fTime;
			if( m_fAccumulateRender > m_fCurDelay) {
				m_fAccumulateRender -= m_fCurDelay;		

				if(m_iTalkFadeType == TALK_FADE_IN) {
					m_iSkinMoveY -= m_iSkinMoveUnitY;				

					if(m_iSkinMoveY < 6)
						m_iSkinMoveY = 6;

					m_iMoveSkinHeight += m_iSkinMoveY;

				}
				else if(m_iTalkFadeType == TALK_FADE_OUT) {
					m_iSkinMoveUnitY *= 2;
					m_iSkinMoveY += m_iSkinMoveUnitY;
					if(m_iSkinMoveY > 64)
						m_iSkinMoveY = 64;

					m_iMoveSkinHeight -= m_iSkinMoveY;				
					if(m_iMoveSkinHeight < 0)
						m_iMoveSkinHeight = 0;

					m_iSkinMoveUnitX *= 2;
					m_iSkinMoveX += m_iSkinMoveUnitX;
					if(m_iSkinMoveX > 64)
						m_iSkinMoveX = 64;					

					m_iMoveSkinWidth -= m_iSkinMoveX;
					if(m_iMoveSkinWidth < 0)
						m_iMoveSkinWidth = 0;

					m_fFadeAlpha -= m_fFadeAlphaUnit;
					if(m_fFadeAlpha < 0.0f)
						m_fFadeAlpha = 0.0f;

					if(m_iMoveSkinHeight == 0 && m_iMoveSkinWidth == 0) {
						m_iTalkFadeType = TALK_FADE_STOP;
						//m_fFadeAlpha = 0.0f;
						
						
						m_iTalkWindowMode = NPC_TALK_WINDOW_MODE_NORMAL;
						m_bShowAttach = false;

						SetRect(&m_rtSkinDest, m_rtSkinBaseDest.right - m_iMoveSkinWidth, m_rtSkinBaseDest.bottom - m_iMoveSkinHeight, m_rtSkinBaseDest.right, m_rtSkinBaseDest.bottom);	
						
						Hide();
					}
				}			

				SetRect(&m_rtSkinDest, m_rtSkinBaseDest.right - m_iMoveSkinWidth, m_rtSkinBaseDest.bottom - m_iMoveSkinHeight, m_rtSkinBaseDest.right, m_rtSkinBaseDest.bottom);				
			}
		}
	}	
	
	if(IsShow() == false) {
		return;
	}

	SPWindow::Process(fTime);

	m_fAccumulateRender += fTime;
	if( m_fAccumulateRender > m_fCurDelay) {
		m_fAccumulateRender -= m_fCurDelay;	

		if(m_bShowAttach) {
			if(g_pEventManager->GetCutInState() == CUT_IN_STATE_TALK) {
				if(m_pNpcTalk && m_pNpcTalk->m_iID) {
					if(m_pNpcTalk->m_iAttachImage) {
						m_fAttachSkinAlpha += 0.09f;

						if(m_fAttachSkinAlpha >= 1.0f) {
							m_fAttachSkinAlpha = 1.0f;
							m_pNpcTalk->m_bShowAttach = true;
						}					
					}
				}
			}
		}
	}

	if(m_iVibrate != BOX_VIBRATE_NULL) {
		m_fVibrateTotalRender += fTime;
		if(m_fVibrateTotalRender > m_fVibrateTotalDelay) {
			m_fVibrateTotalRender -= m_fVibrateTotalDelay;
			//Vibrate 종료
			m_iVibrate = BOX_VIBRATE_NULL;
			SetRect(&m_rtSkinDest, m_rtSkinBaseDest.left, m_rtSkinBaseDest.top, m_rtSkinBaseDest.right, m_rtSkinBaseDest.bottom);
		}

		m_fVibrateRender += fTime;
		if(m_fVibrateRender > m_fVibrateDelay) {
			m_fVibrateRender -= m_fVibrateDelay;
			if(m_iVibrate == BOX_VIBRATE_LEFT) {
				m_iVibrate = BOX_VIBRATE_RIGHT;
				SetRect(&m_rtSkinDest, m_rtSkinBaseDest.left - 5, m_rtSkinBaseDest.top, m_rtSkinBaseDest.right - 5, m_rtSkinBaseDest.bottom);
			}
			else if(m_iVibrate == BOX_VIBRATE_RIGHT) {
				m_iVibrate = BOX_VIBRATE_LEFT;
				SetRect(&m_rtSkinDest, m_rtSkinBaseDest.left + 5, m_rtSkinBaseDest.top, m_rtSkinBaseDest.right + 5, m_rtSkinBaseDest.bottom);
			}
			else {
				m_iVibrate = BOX_VIBRATE_NULL;
				SetRect(&m_rtSkinDest, m_rtSkinBaseDest.left, m_rtSkinBaseDest.top, m_rtSkinBaseDest.right, m_rtSkinBaseDest.bottom);
			}
		}
	}

	if(m_bShowNext) {
		m_fNextRender += fTime;
		if(m_fNextRender > m_fNextDelay) {
			m_fNextRender -= m_fNextDelay;
			
			m_iNextFrame++;
			if(m_iNextFrame >= MAX_NEXT_FRAME)
				m_iNextFrame = 0;
		}
	}
}


void SPWindowNpcTalk::Render(float fTime)
{
	if(g_pEventManager->GetCutInState() > CUT_IN_STATE_NULL) {
		if(m_pWindowSkin[NPC_TALK_WINDOW_MODE_NORMAL] == NULL || 
			m_pWindowSkin[NPC_TALK_WINDOW_MODE_OUTBURST] == NULL)
			return;		
		
		if(m_iTalkFadeType != TALK_FADE_STOP) {
			g_pVideo->Flush();
			if(m_iTalkWindowMode == NPC_TALK_WINDOW_MODE_NORMAL && m_pWindowSkin[NPC_TALK_WINDOW_MODE_NORMAL]) {				
				m_pWindowSkin[NPC_TALK_WINDOW_MODE_NORMAL]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fFadeAlpha));
				m_pWindowSkin[NPC_TALK_WINDOW_MODE_NORMAL]->RenderTexture(&m_rtSkinDest, &m_rtSkinSrc[NPC_TALK_WINDOW_MODE_NORMAL]);
				m_pWindowSkin[NPC_TALK_WINDOW_MODE_NORMAL]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else if(m_iTalkWindowMode == NPC_TALK_WINDOW_MODE_OUTBURST && m_pWindowSkin[NPC_TALK_WINDOW_MODE_OUTBURST]) {
				m_pWindowSkin[NPC_TALK_WINDOW_MODE_OUTBURST]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fFadeAlpha));
				m_pWindowSkin[NPC_TALK_WINDOW_MODE_OUTBURST]->RenderTexture(&m_rtSkinDest, &m_rtSkinSrc[NPC_TALK_WINDOW_MODE_OUTBURST]);
				m_pWindowSkin[NPC_TALK_WINDOW_MODE_OUTBURST]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}

		if(IsShow() == false) {
			return;
		}

		g_pVideo->Flush();

		if(m_iTalkWindowMode == NPC_TALK_WINDOW_MODE_NORMAL && m_pWindowSkin[NPC_TALK_WINDOW_MODE_NORMAL]) {
			m_pWindowSkin[NPC_TALK_WINDOW_MODE_NORMAL]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fFadeAlpha));
			m_pWindowSkin[NPC_TALK_WINDOW_MODE_NORMAL]->RenderTexture(&m_rtSkinDest, &m_rtSkinSrc[NPC_TALK_WINDOW_MODE_NORMAL]);
			m_pWindowSkin[NPC_TALK_WINDOW_MODE_NORMAL]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else if(m_iTalkWindowMode == NPC_TALK_WINDOW_MODE_OUTBURST && m_pWindowSkin[NPC_TALK_WINDOW_MODE_OUTBURST]) {
			m_pWindowSkin[NPC_TALK_WINDOW_MODE_OUTBURST]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fFadeAlpha));
			m_pWindowSkin[NPC_TALK_WINDOW_MODE_OUTBURST]->RenderTexture(&m_rtSkinDest, &m_rtSkinSrc[NPC_TALK_WINDOW_MODE_OUTBURST]);
			m_pWindowSkin[NPC_TALK_WINDOW_MODE_OUTBURST]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}

		if(m_iTalkFadeType == TALK_FADE_STOP) {
			g_pVideo->Flush();
			SPWindow::Render(fTime);

			if(m_bShowNext && m_pNextSkin){
				m_pNextSkin->RenderTexture(&m_rtNextSkinDest, &m_rtNextSkinSrc[m_iNextFrame]);
			}

			if(m_bShowAttach && m_pAttachImage) {
				if(m_pWindowSkin[NPC_TALK_WINDOW_MODE_OUTBURST]) {


//#ifdef _DEBUG					
//					g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
//					g_pVideo->GetSysLine()->DrawRect(m_rtAttachDest);
//					g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
//#endif
				}
			}			

			if(m_bFocusLose && m_pFocusLoseSkin){
				g_pVideo->Flush();
				m_pFocusLoseSkin->RenderTexture(&m_rtSkinDest, &m_rtFocusLoseSkinSrc[m_iTalkWindowMode]);
			}

#ifdef _DEBUG
			RECT rtRect = {0, 0, 0, 0};
			if(m_pTypeWriter && m_pTypeWriter->IsShow()) {
				//m_pTypeWriter->GetWindowRect(rtRect);
				m_pParent->GetWindowRect(rtRect);
				g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
				g_pVideo->GetSysLine()->DrawRect(rtRect);
				g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

				m_pTypeWriter->GetWindowRect(rtRect);
				g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
				g_pVideo->GetSysLine()->DrawRect(rtRect);
				g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

				m_pNpcName->GetWindowRect(rtRect);
				g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
				g_pVideo->GetSysLine()->DrawRect(rtRect);
				g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
#endif
		}
	}	
}


//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowNpcTalk )

SPIMESSAGE_COMMAND( SPIM_SET_NPCDATA,				OnSetNpcData)
SPIMESSAGE_COMMAND( SPIM_SET_SAVEPOINT,				OnSetSavePoint)
SPIMESSAGE_COMMAND(	SPIM_SET_NPCTALK,				OnSetNpcTalk)
SPIMESSAGE_COMMAND(	SPIM_SET_HIDE_START,			OnHideStart)
SPIMESSAGE_COMMAND(	SPIM_SET_CUTIN_CLICK,			OnTypeWriteClick)
SPIMESSAGE_COMMAND(	SPIM_SET_NPCTALK_BOXTYPE,		OnSetBoxType)


SPIMESSAGE_CONTROL( SPIM_TYPE_END,		WIID_NPCTALK_MESSAGE	, OnTypeWriteEnd )

SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_NPCTALK_SKIN		, OnTypeWriteClick )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_NPCTALK_NPCNAME	, OnTypeWriteClick )
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_NPCTALK_MESSAGE	, OnTypeWriteClick )

SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_NPCTALK_CLOSE		, OnNpcTalkClose)

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)


int SPWindowNpcTalk::OnSetNpcData( WPARAM wParam, LPARAM lParam )
{
	//Show();									//이 메시지를 받아야 WindowNpcTalk 창이 보인다 	
	m_iTalkIndex = 0;	
	m_bShowAttach = false;	

	int iCounter = 0;
	std::string strMenu;

	if(g_pEventManager->GetCutInState() != CUT_IN_STATE_NULL) {
		m_strMessage = g_pEventManager->GetEventHandler()->GetNpcData()->m_strMessage;
		m_pNpcName->SetWindowText(g_pEventManager->GetEventHandler()->GetNpcData()->m_strNpcName.c_str());

		ZeroMemory(m_szMsg, 512);		
		if(g_pGOBManager->GetLocalPlayer()) {
			sprintf(m_szMsg, m_strMessage.c_str(), g_pGOBManager->GetLocalPlayer()->GetStatus()->GetGOBName());
			m_pTypeWriter->SetWindowText(m_szMsg);			
		}
		else {
			m_pTypeWriter->SetWindowText(m_strMessage.c_str());			
		}		
	}

	//bool bReload true - Npc메뉴 상태로 돌아온 경우 (ESC키에 의함)
	bool bReload = (bool)wParam;
	if(bReload) {
		m_iTalkFadeType = TALK_FADE_IN;
		m_iTalkWindowMode = NPC_TALK_WINDOW_MODE_NORMAL;
		m_bShowAttach = false;
		m_bFocusLose = false;

		m_iVibrate = BOX_VIBRATE_NULL;
		SetRect(&m_rtSkinDest, m_rtSkinBaseDest.left, m_rtSkinBaseDest.top, m_rtSkinBaseDest.right, m_rtSkinBaseDest.bottom);
		
		m_pTypeWriter->SPSendMessage(SPIM_TYPE_START, 0,0);
		m_bTypeWriteEnd = false;		
		
		SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_PCTALK);
		if(pWindow) {
			pWindow->SPSendMessage(SPIM_SET_NPCDATA, (WPARAM)0, (LPARAM)0);
			
			if(pWindow->IsShow())
				pWindow->SPSendMessage(SPIM_SET_HIDE_START, (WPARAM)0, (LPARAM)0);
		}		
	}
	return 1;
}


int SPWindowNpcTalk::OnSetSavePoint( WPARAM wParam, LPARAM lParam )
{
	int iType = (int)wParam;
	m_bFocusLose = false;

	if(iType == 0) {
		m_iTalkIndex = 0;

		m_pSavePoint = g_pEventManager->GetEventHandler()->GetSavePointData();

		m_pNpcName->SetWindowText(m_pSavePoint->m_MsgInfo[m_iTalkIndex].m_strNpcName.c_str());

		//		
		m_strMessage = m_pSavePoint->m_MsgInfo[m_iTalkIndex].m_strMessage;	
		ZeroMemory(m_szMsg, 512);		
		if(g_pGOBManager->GetLocalPlayer()) {
			sprintf(m_szMsg, m_strMessage.c_str(), g_pGOBManager->GetLocalPlayer()->GetStatus()->GetGOBName());
			m_pTypeWriter->SetWindowText(m_szMsg);
		}
		else {
			m_pTypeWriter->SetWindowText(m_strMessage.c_str());
		}		

		m_iTalkState = TALK_STATE_TALK;
	}	
	else if(iType == 1) {		//YES를 선택한 경우
		if(m_pSavePoint && m_pSavePoint->m_iEventID) {
			m_pNpcName->SetWindowText(m_pSavePoint->m_MsgInfo[SAVE_POINT_YES].m_strNpcName.c_str());

			m_strMessage = m_pSavePoint->m_MsgInfo[SAVE_POINT_YES].m_strMessage;
			ZeroMemory(m_szMsg, 512);		
			if(g_pGOBManager->GetLocalPlayer()) {
				sprintf(m_szMsg, m_strMessage.c_str(), g_pGOBManager->GetLocalPlayer()->GetStatus()->GetGOBName());
				m_pTypeWriter->SetWindowText(m_szMsg);
			}
			else {
				m_pTypeWriter->SetWindowText(m_strMessage.c_str());
			}
			
			g_pEventManager->GetEventHandler()->SetNpcBGM(m_pSavePoint->m_MsgInfo[SAVE_POINT_YES].m_strBGM);

			m_pTypeWriter->SPSendMessage(SPIM_TYPE_START, 0,0);
			m_bTypeWriteEnd = false;			
			m_bShowNext = false;
		}
	}
	else if(iType == 2) {		//No를 선택한 경우
		if(m_pSavePoint && m_pSavePoint->m_iEventID) {
			m_pNpcName->SetWindowText(m_pSavePoint->m_MsgInfo[SAVE_POINT_NO].m_strNpcName.c_str());
			m_strMessage = m_pSavePoint->m_MsgInfo[SAVE_POINT_NO].m_strMessage;
			ZeroMemory(m_szMsg, 512);		
			if(g_pGOBManager->GetLocalPlayer()) {
				sprintf(m_szMsg, m_strMessage.c_str(), g_pGOBManager->GetLocalPlayer()->GetStatus()->GetGOBName());
				m_pTypeWriter->SetWindowText(m_szMsg);
			}
			else {
				m_pTypeWriter->SetWindowText(m_strMessage.c_str());
			}
			
			g_pEventManager->GetEventHandler()->SetNpcBGM(m_pSavePoint->m_MsgInfo[SAVE_POINT_NO].m_strBGM);

			m_pTypeWriter->SPSendMessage(SPIM_TYPE_START, 0,0);
			m_bTypeWriteEnd = false;
			m_bShowNext = false;
		}
	}
	else {
		g_pEventManager->SetCutInState(CUT_IN_STATE_RELEASE);
	}

	return 1;
}


int SPWindowNpcTalk::OnSetNpcTalk(WPARAM wParam, LPARAM lParam)
{
	//if(g_pEventManager->GetCutInState() != CUT_IN_STATE_NULL) {
		m_pNpcTalk = g_pEventManager->GetEventHandler()->GetNpcTalkData();
		m_bShowAttach = false;
		m_bFocusLose = false;
		m_bShowNext = false;
		if(m_pNpcTalk == NULL)
			return 1;
		if(m_pNpcTalk->m_iID == 0)
			return 1;
		
		NPC_TALK_WINDOW_MODE iMode = m_pNpcTalk->m_iBoxType;
		//g_pEventManager->GetEventHandler()->SetNpcTalkWindowMode(iMode);
		OnSetBoxType((WPARAM)iMode, (LPARAM)0);

		m_pNpcName->SetWindowText(m_pNpcTalk->m_strName.c_str());

		m_strMessage = m_pNpcTalk->m_strMessage;
		ZeroMemory(m_szMsg, 512);		
		if(g_pGOBManager->GetLocalPlayer()) {
			sprintf(m_szMsg, m_strMessage.c_str(), g_pGOBManager->GetLocalPlayer()->GetStatus()->GetGOBName());
			m_pTypeWriter->SetWindowText(m_szMsg);			
		}
		else {
			m_pTypeWriter->SetWindowText(m_strMessage.c_str());			
		}
		
		g_pEventManager->GetEventHandler()->SetNpcBGM(m_pNpcTalk->m_strBGM);		

		m_pTypeWriter->SPSendMessage(SPIM_TYPE_START, 0,0);
		m_bTypeWriteEnd = false;
		
		////
		//if(m_iTalkFadeType == TALK_FADE_STOP) {
		//	if(m_pNpcTalk->m_bCompel) {
		//		m_pClose->Hide();
		//	}
		//	else {
		//		m_pClose->Show();
		//	}
		//}

		g_pEventManager->GetEventHandler()->SetEventClick(EVENT_CLICK_NPCTALK);
	//}
	return 1;
}


int SPWindowNpcTalk::OnHideStart(WPARAM wParam, LPARAM lParam)
{
	m_pTypeWriter->Hide();
	
	if(m_iTalkFadeType != TALK_FADE_IN){
		if(!m_bShow)
			return 1;
	}


	return 1;
}


int SPWindowNpcTalk::OnTypeWriteEnd(WPARAM wParam, LPARAM lParam)
{
	if(m_bTypeWriteEnd)
		return 1;
	
	m_bTypeWriteEnd = true;
	DXUTOutputDebugString("TypeWrite String Print End...\n");	

	if(g_pEventManager->GetCutInState() != CUT_IN_STATE_NULL) {		
		if(g_pEventManager->GetCutInState() == CUT_IN_STATE_SCROLL_END) {
			SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_PCTALK);
			if( pWindow ) {
				if(!pWindow->IsShow()) 
					pWindow->Show();
			}
			//[2007/10/9] - Npc처음 메뉴로 넘어갈때 비활성화 요청시 주석 해제
			//m_bFocusLose = true;
		}

		//다음으로 넘어가라는 신호가 나와야 한다 
		if(g_pEventManager->GetCutInState() == CUT_IN_STATE_TALK) {			
			if(m_pNpcTalk&& m_pNpcTalk->m_iBoxPos == NPC_TALK_WINDOW_NPC) {
				if(m_pNpcTalk->m_iMissionID) {
					m_bShowNext = true;
				}
				else if(m_pNpcTalk->m_NpcEventType[0].m_iEventType == NPC_EVENT_TYPE_TALK) {				
					int iTalkID = m_pNpcTalk->m_NpcEventType[0].m_iTypeID;
					if(iTalkID) {
						m_bShowNext = true;
					}
				}
			}
		}
		else if(g_pEventManager->GetCutInState() == CUT_IN_STATE_SAVEPOINT){
			if(m_pSavePoint && m_iTalkIndex <= m_pSavePoint->m_iMsgSize)
				m_bShowNext = true;
		}
	}

	return 1;
}


int SPWindowNpcTalk::OnTypeWriteClick(WPARAM wParam, LPARAM lParam)
{	
 	DXUTOutputDebugString("TypeWrite Click... \n");
	
	if(m_bTypeWriteEnd){
		if(g_pEventManager->GetCutInState() == CUT_IN_STATE_TALK && m_pNpcTalk) {
			if(m_pNpcTalk->m_bShowAttach == false && m_pNpcTalk->m_iAttachImage) {
				//AttachImage Show
				bool bRet = LoadAttachImage(m_pNpcTalk->m_iAttachImage);				
				if(m_bShowAttach == false && bRet) {
					m_bShowAttach = true;
					m_fAttachSkinAlpha = 0.18f;
					m_fAccumulateRender = 0.0f;
				}
				return 1;
			}
			
			if(m_pNpcTalk->m_iTalkType == TALK_TYPE_SELECT)
				return 1;

			if(m_pNpcTalk->m_iStoryID) {
				g_pEventManager->SetCutInState(CUT_IN_STATE_RELEASE);
				g_pSubGameManager->SPPerformMessage(SPGM_SET_STORY_VIEW, 1, m_pNpcTalk->m_iStoryID);
				return 1;
			}
			
			if(m_pNpcTalk->m_iClassChangeID) {				
				g_pEventManager->GetClassChangeManager()->SendClassChangeInfo(m_pNpcTalk->m_iClassChangeID);
				g_pEventManager->SetCutInState(CUT_IN_STATE_RELEASE);
				return 1;
			}
			
			//현재 미션이 있는 경우 
			if(m_pNpcTalk->m_iMissionID) {
				SPCoreEventManager* pCoreEventManager = g_pEventManager->GetCoreEventManager();
				SPCoreMissionAttr* pMissionAttr = pCoreEventManager->GetMissionArchive()->GetCoreMissionAttr(m_pNpcTalk->m_iMissionID);
				if(pMissionAttr && pMissionAttr->m_iID) {
					if(pMissionAttr->CheckRequire()) {
						////
						//switch(pMissionAttr->m_iType) {
						//	case CORE_MISSION_TYPE_QUEST:
						//		break;
						//	case CORE_MISSION_TYPE_ITEM:
						//		break;
						//	case CORE_MISSION_TYPE_EFFECT:								
						//		break;
						//	default:
						//		break;
						//}
						
						g_pEventManager->SetCutInState(CUT_IN_STATE_RELEASE);
						pCoreEventManager->SendCoreEventRaise(m_pNpcTalk->m_iMissionID);
					}
					else {
						g_pEventManager->SetCutInState(CUT_IN_STATE_RELEASE);
					}
				}				

				return 1;
			}

			if(m_pNpcTalk->m_NpcEventType[0].m_iEventType == NPC_EVENT_TYPE_TALK) {				
				int iTalkID = m_pNpcTalk->m_NpcEventType[0].m_iTypeID;
				if(iTalkID) {
					if(m_pNpcTalk->m_bShowAttach == true) {
						m_bShowAttach = false;
						m_fAttachSkinAlpha = 0.0f;
						SetNpcTalkData(iTalkID);
					}
					else {
						return 1;
					}
				}
				else {
					g_pEventManager->SetCutInState(CUT_IN_STATE_RELEASE);
				}
			}
			////[2007/10/7] - 대화 마지막인경우
			//else if(m_pNpcTalk->m_iTalkType == TALK_TYPE_NORMAL || m_pNpcTalk->m_iTalkType == TALK_TYPE_NULL)
			//{
			//	if(m_pNpcTalk->m_NpcEventType[0].m_iEventType == NPC_EVENT_TYPE_NULL &&
			//		m_pNpcTalk->m_NpcEventType[0].m_iTypeID == 0)
			//	{
			//		g_pEventManager->SetCutInState(CUT_IN_STATE_RELEASE);
			//	}
			//}		
		}
		else if(g_pEventManager->GetCutInState() == CUT_IN_STATE_SAVEPOINT) {
			if(g_pEventManager->GetEventHandler()->GetNpcID() == 0 && m_pSavePoint && m_pSavePoint->m_iEventID) {
				if(m_iTalkIndex < m_pSavePoint->m_iMsgSize) {
					m_iTalkIndex++;
					SetSavePointTalk(m_iTalkIndex);
				}
				else if(m_iTalkIndex == m_pSavePoint->m_iMsgSize){
                    m_iTalkIndex++;
					SetSavePointTalk(m_iTalkIndex);
				}
				else {
					//
				}
			}
		}
	}
	else {
		if(m_pTypeWriter) 
			m_pTypeWriter->SPSendMessage( SPIM_TYPE_SKIP, 0,0 );
	}	

	return 1;
}


int SPWindowNpcTalk::OnNpcTalkClose( WPARAM wParam, LPARAM lParam )
{
	if(g_pGOBManager->GetLocalPlayer() == NULL)
		return 1;
	
	if(g_pEventManager->GetEventHandler()->GetNpcID() == 0) {
		int iStage = g_pGOBManager->GetLocalPlayer()->GetCurStageID();
		int iGroup = g_pGOBManager->GetLocalPlayer()->GetCurMapGroupID();		
		std::string strBGM = g_StageManager.GetCurStage(iStage)->GetMapGroup(iGroup)->m_strBGMFile;
		int iSoundEffect = g_StageManager.GetCurStage(iStage)->GetMapGroup(iGroup)->m_iSoundEffectType;
		
		if(strBGM != ""){
			g_pSubGameManager->SPPerformMessage(SPGM_SUG_SETBGM , (WPARAM)strBGM.c_str());
			SPSoundBase::GetInstance()->SetRoomType(iSoundEffect);
		}
	}
	
	g_pEventManager->SetCutInState(CUT_IN_STATE_RELEASE);	
	
	DXUTOutputDebugString("CUT_IN_STATE_NULL\n");	

	Hide();
	return 1;
}


int SPWindowNpcTalk::OnSetBoxType(WPARAM wParam, LPARAM lParam)
{
	m_iTalkWindowMode = (NPC_TALK_WINDOW_MODE)wParam;
	
	if(m_iTalkWindowMode == NPC_TALK_WINDOW_MODE_NORMAL){
		m_iVibrate = BOX_VIBRATE_NULL;
		SetRect(&m_rtSkinDest, m_rtSkinBaseDest.left, m_rtSkinBaseDest.top, m_rtSkinBaseDest.right, m_rtSkinBaseDest.bottom);
	}
	else if(m_iTalkWindowMode == NPC_TALK_WINDOW_MODE_OUTBURST){
		m_iVibrate = BOX_VIBRATE_LEFT;
		m_fVibrateRender		= 0.0f;
		m_fVibrateDelay			= 0.01f;
		m_fVibrateTotalRender	= 0.0f;
		m_fVibrateTotalDelay	= 1.0f;
	}	
	
	return 1;
}


void SPWindowNpcTalk::SetNpcTalkData(int iTalkIndex /*= 0*/)
{
	m_pNpcTalk = g_pEventManager->GetEventHandler()->LoadNpcTalkDataLDT(iTalkIndex);
	m_bShowAttach = false;
	m_bShowNext = false;

	if(m_pNpcTalk == NULL)
		return;
	if(m_pNpcTalk->m_iID == 0)
		return;
	
	if(g_pEventManager->GetCutInState() != CUT_IN_STATE_TALK)
		g_pEventManager->SetCutInState(CUT_IN_STATE_TALK);	
	
	m_iTalkIndex = 0;
	
	SetNpcTalk();
}


void SPWindowNpcTalk::SetNpcTalk()
{	
	if(m_pNpcTalk->m_iBoxPos == NPC_TALK_WINDOW_NPC) {
		OnSetNpcTalk((WPARAM)0, (LPARAM)0);
		m_bFocusLose = false;
		SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_PCTALK);
		if(pWindow && pWindow->IsShow()) {
			pWindow->SPSendMessage(SPIM_SET_NPCTALK_FOCUSLOSE, (WPARAM)true, (LPARAM)0);
		}
	}	
	else if(m_pNpcTalk->m_iBoxPos == NPC_TALK_WINDOW_PC) {
		SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_PCTALK);
		if(pWindow) {
			pWindow->SPSendMessage(SPIM_SET_NPCTALK);
			if(!pWindow->IsShow())
				pWindow->Show();

			m_bFocusLose = true;
		}		
	}

}


void SPWindowNpcTalk::SetSavePointTalk(int iTalkIndex /*= 0*/)
{
	if( m_pSavePoint == NULL )
		return;

	//ClearString();

	//
	if(iTalkIndex > m_pSavePoint->m_iMsgSize) {						//대화 마지막후 PC선택으로 넘김
        SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_PCTALK);
		if(pWindow) {
			pWindow->SPSendMessage(SPIM_SET_SAVEPOINT, (WPARAM)0, (LPARAM)0);
			if(!pWindow->IsShow())
				pWindow->Show();
		}
		m_bFocusLose = true;
		return;
	}
	else if(iTalkIndex == m_pSavePoint->m_iMsgSize) {				//대화 마지막		
		m_pNpcName->SetWindowText(m_pSavePoint->m_MsgInfo[SAVE_POINT_SELECT].m_strNpcName.c_str());
		m_strMessage = m_pSavePoint->m_MsgInfo[SAVE_POINT_SELECT].m_strMessage;
		ZeroMemory(m_szMsg, 512);		
		if(g_pGOBManager->GetLocalPlayer()) {
			sprintf(m_szMsg, m_strMessage.c_str(), g_pGOBManager->GetLocalPlayer()->GetStatus()->GetGOBName());
			m_pTypeWriter->SetWindowText(m_szMsg);
		}
		else {
			m_pTypeWriter->SetWindowText(m_strMessage.c_str());
		}

		g_pEventManager->GetEventHandler()->SetNpcBGM(m_pSavePoint->m_MsgInfo[SAVE_POINT_SELECT].m_strBGM);

		//이미지 로드
		g_pEventManager->GetEventHandler()->SetNpcTalkImage(m_pSavePoint->m_MsgInfo[SAVE_POINT_SELECT].m_iCutInImage);
	}	
	else {
		m_pNpcName->SetWindowText(m_pSavePoint->m_MsgInfo[m_iTalkIndex].m_strNpcName.c_str());
		m_strMessage = m_pSavePoint->m_MsgInfo[m_iTalkIndex].m_strMessage; //->m_strMessage[m_iTalkIndex];

		ZeroMemory(m_szMsg, 512);		
		if(g_pGOBManager->GetLocalPlayer()) {
			sprintf(m_szMsg, m_strMessage.c_str(), g_pGOBManager->GetLocalPlayer()->GetStatus()->GetGOBName());
			m_pTypeWriter->SetWindowText(m_szMsg);
		}
		else {
			m_pTypeWriter->SetWindowText(m_strMessage.c_str());
		}

		g_pEventManager->GetEventHandler()->SetNpcBGM(m_pSavePoint->m_MsgInfo[m_iTalkIndex].m_strBGM);

		//이미지 로드
		g_pEventManager->GetEventHandler()->SetNpcTalkImage(m_pSavePoint->m_MsgInfo[m_iTalkIndex].m_iCutInImage);

		m_iTalkState = TALK_STATE_TALK;		
	}

}


bool SPWindowNpcTalk::LoadAttachImage(int iImageIndex /*= 0*/)
{
	if( m_pAttachImageLDTFile == NULL )
		return false;
	
	LDT_Field LDTField;
	int	iItemID = iImageIndex;
	std::string strImagePath = "";
	
	bool bRet = true;

	bRet = m_pAttachImageLDTFile->GetField( iItemID , 0, LDTField );				//_Image
	if(bRet == false)
		return bRet;	
	
	if(LDTField.uData.pValue){
		strImagePath = LDTField.uData.pValue;
	}
	else 
		return false;


	
	return true;
}
