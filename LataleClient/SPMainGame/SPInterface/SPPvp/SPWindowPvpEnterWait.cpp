#include "SPCommon.h"
#include "SPUtil.h"

#include "GlobalDefines_Share.h"

#include "SPMainGameDEF.H"
#include "SPGameObjectDEF.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPSubGameManager.h"
#include "SPGameObject.h"
#include "SPManager.h"
#include "SPMouseCursor.h"
#include "SPIMEPool.h"
#include "SPWindowDEF.h"
#include "SPInterfaceManager.h"

#include "SPGOBStatus.h"
#include "SPGOBClusterDef.h"
#include "SPGOBCluster.h"
#include "SPGobModelUnit.h"
#include "SPAvatarModelUnitDef.h"
#include "SPAvatarModelUnit.h"
#include "SPPlayer.h"
#include "SPGOBManager.h"

#include "SPEventDEF.h"
#include "SPEventManager.h"

#include "SPSoundArchive.h"
#include "SPSoundManager.h"

#include "SPPvpManager.h"

#include "SPWindow.h"
#include "SPWindowButton.h"
#include "SPWindowStatic.h"
#include "SPWindowTypeWriter.h"
#include "SPWindowPvpEnterWait.h"

#include "SPDebug.h"


SPWindowPvpEnterWait::SPWindowPvpEnterWait(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent /*= NULL*/)
: SPWindow(WNDID_PVPENTERWAIT, InstanceID, iX, iY, iCX, iCY, pParent)
, m_pTextureBase(NULL)
, m_pWindowMsg(NULL)
{
	Init();
}


SPWindowPvpEnterWait::~SPWindowPvpEnterWait()
{
	Clean();
}


void SPWindowPvpEnterWait::Init()
{
	g_pVideo->GetTextureMgr()->LoadTexture("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , &m_pTextureBase);	
	
	////
	//SetRect(&m_rtSrcBase[PVPENTERWAIT_BASE_TOP_LEFT],		460, 1, 472, 13);
	//SetRect(&m_rtSrcBase[PVPENTERWAIT_BASE_TOP],			473, 1, 485, 13);	
	//SetRect(&m_rtSrcBase[PVPENTERWAIT_BASE_TOP_RIGHT],		486, 1, 498, 13);
	//SetRect(&m_rtSrcBase[PVPENTERWAIT_BASE_CENTER_LEFT],	460, 14, 472, 26);
	//SetRect(&m_rtSrcBase[PVPENTERWAIT_BASE_CENTER],			473, 14, 485, 26);
	//SetRect(&m_rtSrcBase[PVPENTERWAIT_BASE_CENTER_RIGHT],	486, 14, 498, 26);
	//SetRect(&m_rtSrcBase[PVPENTERWAIT_BASE_BOTTOM_LEFT],	460, 27, 472, 39);
	//SetRect(&m_rtSrcBase[PVPENTERWAIT_BASE_BOTTOM],			473, 27, 485, 39);
	//SetRect(&m_rtSrcBase[PVPENTERWAIT_BASE_BOTTOM_RIGHT],	486, 27, 498, 39);	

	//SetRect(&m_rtDestBase[PVPENTERWAIT_BASE_TOP_LEFT],		m_iAX, m_iAY, m_iAX + 8, m_iAY + 8);
	//SetRect(&m_rtDestBase[PVPENTERWAIT_BASE_TOP],			m_iAX + 8, m_iAY, m_iAX + 8 + 301, m_iAY + 8);	
	//SetRect(&m_rtDestBase[PVPENTERWAIT_BASE_TOP_RIGHT],		m_iAX + 309, m_iAY, m_iAX + 309 + 8, m_iAY + 8);
	//SetRect(&m_rtDestBase[PVPENTERWAIT_BASE_CENTER_LEFT],	m_iAX, m_iAY + 8, m_iAX + 8, m_iAY + 8 + 94);
	//SetRect(&m_rtDestBase[PVPENTERWAIT_BASE_CENTER],		m_iAX + 8, m_iAY + 8, m_iAX + 8 + 301, m_iAY + 8 + 94);
	//SetRect(&m_rtDestBase[PVPENTERWAIT_BASE_CENTER_RIGHT],	m_iAX + 309, m_iAY + 8, m_iAX + 309 + 8, m_iAY + 8 + 94);
	//SetRect(&m_rtDestBase[PVPENTERWAIT_BASE_BOTTOM_LEFT],	m_iAX, m_iAY + 102, m_iAX + 8, m_iAY + 102 + 8);
	//SetRect(&m_rtDestBase[PVPENTERWAIT_BASE_BOTTOM],		m_iAX + 8, m_iAY + 102, m_iAX + 8 + 301, m_iAY + 102 + 8);
	//SetRect(&m_rtDestBase[PVPENTERWAIT_BASE_BOTTOM_RIGHT],	m_iAX + 309, m_iAY + 102, m_iAX + 309 + 8, m_iAY + 102 + 8);

	m_iAniFrame		= 0;
	m_fAniRender	= 0.0f;
	m_fAniDelay		= 0.05f;

	SetRect(&m_rtAniSrc[0],				1, 71, 45, 105);
	SetRect(&m_rtAniSrc[1],				46, 71, 90, 105);
	SetRect(&m_rtAniSrc[2],				91, 71, 135, 105);
	SetRect(&m_rtAniSrc[3],				136, 71, 180, 105);
	SetRect(&m_rtAniSrc[4],				181, 71, 225, 105);
	SetRect(&m_rtAniSrc[5],				226, 71, 270, 105);

	SetRect(&m_rtAniDest,				m_iAX + 46, m_iAY + 54, m_iAX + 46 + 44, m_iAY + 54 + 34);
	
	InitSubControl();
}


void SPWindowPvpEnterWait::InitSubControl()
{
	char* OPTION_IMAGE_FILE = "DATA/INTERFACE/CONCEPT/UI_Button_001.PNG";

	//// Control
	SPWindowButton* pButton = NULL;
	SPWindowStatic* pStatic = NULL;
	SPWindowStatic* pParent = NULL;
	SPWindowStatic* pSkin	= NULL;
	
	//WIID_PVPENTERWAIT_CANCEL
	pButton = new SPWindowButton( WIID_PVPENTERWAIT_CANCEL , 153, 106, 88, 22, this);
	pButton->SetImage		(OPTION_IMAGE_FILE, 1,  421);
	pButton->SetImageHit	(OPTION_IMAGE_FILE, 1,  444);
	pButton->SetImagePush	(OPTION_IMAGE_FILE, 1,  467);
	pButton->SetImageDisable(OPTION_IMAGE_FILE, 1,  490);
	pButton->Show();

	//WIID_PVPENTERWAIT_HELP
	pStatic = new SPWindowStatic( WIID_PVPENTERWAIT_HELP, 105,	52,	228,	41, this);	
	pStatic->SetMultiLine(true, 2);
	pStatic->SetFormat(DT_LEFT | DT_VCENTER);
	pStatic->SetFont(FONT_12_BOLD);
	pStatic->SetFontColor(D3DXCOLOR(0.32f, 0.27f, 0.17f, 1.0f));
	pStatic->SetWindowText("WIID_PVPENTERWAIT_HELP");
	m_pWindowMsg = pStatic;
	pStatic->Show();
}


void SPWindowPvpEnterWait::Clean()
{
	SAFE_RELEASE(m_pTextureBase);
	SPWindow::Clean();
}


void SPWindowPvpEnterWait::Show(bool bWithChild /*= true*/)
{
	m_iAniFrame		= 0;
	m_fAniRender	= 0.0f;
	
	SPWindow::Show(bWithChild);

	if(m_pWindowMsg) {		
		if(g_pResourceManager->GetGlobalString(51000001)) {
			char szTemp[512];
			ZeroMemory(szTemp, 512);			
			
			std::string strFFA2		= "";
			std::string strFFA4		= "";
			std::string strFFA6		= "";
			std::string strFFA8		= "";
			
			UINT32 iPvpScale = g_pkPvpManager->GetPVPScale();
			if(iPvpScale & PVP_SCALE_2){
				if(g_pResourceManager->GetGlobalString(51000012))
					strFFA2 = g_pResourceManager->GetGlobalString(51000012);
			}			
			
			if(iPvpScale & PVP_SCALE_4){
				if(g_pResourceManager->GetGlobalString(51000013))
					strFFA4 = g_pResourceManager->GetGlobalString(51000013);
			}
			
			if(iPvpScale & PVP_SCALE_6){
				if(g_pResourceManager->GetGlobalString(51000014))
					strFFA6 = g_pResourceManager->GetGlobalString(51000014);
			}
			
			if(iPvpScale & PVP_SCALE_8){
				if(g_pResourceManager->GetGlobalString(51000015))
					strFFA8 = g_pResourceManager->GetGlobalString(51000015);
			}

			sprintf(szTemp, g_pResourceManager->GetGlobalString(51000001), strFFA2.c_str(), strFFA4.c_str(), strFFA6.c_str(), strFFA8.c_str());
			m_pWindowMsg->SetWindowText(szTemp);
		}
	}
}


void SPWindowPvpEnterWait::Hide(bool bSendServer /*= true*/)
{
	SPWindow::Hide(bSendServer);
}


void SPWindowPvpEnterWait::Process(float fTime)
{
	if(!m_bShow)			return;
	
	SPWindow::Process(fTime);

	m_fAniRender += fTime;
	if(m_fAniRender > m_fAniDelay) {
		m_fAniRender -= m_fAniDelay;

		m_iAniFrame++;
		if(m_iAniFrame >= MAX_WAIT_ANI_FRAME)
			m_iAniFrame = 0;
	}
}


void SPWindowPvpEnterWait::Render(float fTime)
{
	if(!m_bShow)		return;

	////
	//if(m_pTextureBase) {
	//	g_pVideo->Flush();
	//	for(int i = 0; i < PVPENTERWAIT_BASE_MAX; i++) {
	//		m_pTextureBase->RenderTexture(&m_rtDestBase[i], &m_rtSrcBase[i]);
	//		////#ifdef _DEBUG
	//		//			//RECT rtRect = {0, 0, 0, 0};			
	//		//			//m_pItemIcon[i]->GetWindowRect(rtRect);
	//		//			g_pVideo->Flush();
	//		//			g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	//		//			g_pVideo->GetSysLine()->DrawRect(m_rtDestBase[i]);
	//		//			g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	//		//#endif
	//	}
	//}
	
	SPWindow::Render(fTime);

	if(m_pTextureBase && (m_iAniFrame >=0 && m_iAniFrame <MAX_WAIT_ANI_FRAME) ) {
		m_pTextureBase->RenderTexture(&m_rtAniDest, &m_rtAniSrc[m_iAniFrame]);
	}
}


/**
	EnterWait 창이 나올때 Sound Effect
*/
//void SPWindowPvpEnterWait::WaitEffectSound(int iSoundID)
//{
//	SPSoundArchive* pSoundArchive	=	NULL;
//	SPPlayer*	pLocalPlayer = g_pGOBManager->GetLocalPlayer();
//
//	pLocalPlayer->SPGOBPerformMessage( SPGM_GETSOUNDARCHIVE , (LPARAM)&pSoundArchive );
//
//	if( pSoundArchive != NULL )
//	{
//		SOUND_TABLE_INFO* pInfo = SPSoundManager::GetInstance()->GetSoundTable( (unsigned int)iSoundID );
//
//		if( pInfo != NULL )
//		{
//			std::string strFullName;
//
//			if( !pInfo->strFileName[0].empty() )
//			{
//				strFullName = "DATA/SOUND/";
//				strFullName += pInfo->strFileName[0];
//
//				pSoundArchive->InsertItem( strFullName.c_str() , pInfo->fDelayTime[ 0 ] , pInfo->bLoop[ 0 ] );
//			}
//		}
//	}
//}



SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowPvpEnterWait )
SPIMESSAGE_COMMAND( SPIM_PVP_ENTERWAIT_REGIST,								OnPvpRegist)
SPIMESSAGE_COMMAND( SPIM_PVP_ENTERWAIT_CANCEL,								OnPvpRegistCancel)
SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP,	WIID_PVPENTERWAIT_CANCEL,			OnCancel)
SPIMESSAGE_OBJ_MAP_END	( SPWindow	)


int SPWindowPvpEnterWait::OnPvpRegist(WPARAM wParam, LPARAM lParam)
{
	SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_PVPRESERVE);
	if(pWindow) {
		pWindow->Hide();
	}
	Show();

	g_pInterfaceManager->SetFocusWindow(this) ;
	//WaitEffectSound() ;

	return 1;
}


int SPWindowPvpEnterWait::OnPvpRegistCancel(WPARAM wParam, LPARAM lParam)
{
	g_pEventManager->SetCutInState(CUT_IN_STATE_RELEASE);
	Hide();
	
	return 1;
}


int SPWindowPvpEnterWait::OnCancel(WPARAM wParam, LPARAM lParam)
{		
	//PVP 등록 패킷
	g_pkPvpManager->SendPVPEnterCancel();
	return 1;
}



