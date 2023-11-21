#include "LT_Error.h"
#include "SPCommon.h"
#include "SPMainGameDEF.H"

#include "SPManager.h"
#include "SPClientCommandBinder.h"
#include "SPSubGameManager.h"
#include "SPGOBManager.h"
#include "SPResourceManager.h"
#include "SPWindowDEF.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

SPSubGameManager*	g_pSubGameManager	= NULL;
EXECUTION_MODE		g_ExcutionMode		= EMODE_NULL;

SPSubGameManager::SPSubGameManager(SUBGAME_ID SubGameID)
: SPManager()
, m_SubGameID		(SubGameID)
, m_NextSubGameID	(SUBGAME_NULL)
, m_iGameState		(GS_INIT)
, m_fWaitTime		(0.0f)
, m_pCommandBinder (NULL)
{
	SetMgrID(MGR_ID_SUBGAME);
	g_pSubGameManager = this;
	m_pCommandBinder = new SPClientCommandBinder;
}

SPSubGameManager::~SPSubGameManager() 
{
	SAFE_DELETE(m_pCommandBinder);
}

bool SPSubGameManager::Init()
{
	m_iGameState = GS_GAME;	

	return SPManager::Init();
}

void SPSubGameManager::Clean()
{
	//SPSoundBase::GetInstance()->SetMainBGM(NULL);
	SPManager::Clean();
}

bool SPSubGameManager::WndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	return false;
}

int	SPSubGameManager::SPPerformMessage(UINT msg, WPARAM wparam, LPARAM lParam)
{
	if(SPGM_SUG_COMMON_BEGIN <= msg && SPGM_SUG_COMMON_END > msg ) {
		return SPGMESSAGE_FUNC((SPGM)msg, wparam, lParam);
	}
	return -1;
}

void SPSubGameManager::Render(float fTime)
{
	SPManager::Render(fTime);
}

void SPSubGameManager::Process(float fTime)
{
	SPManager::Process(fTime);
}

void SPSubGameManager::RenderText()
{
}

bool SPSubGameManager::ClientCommandExcute(const char* pszCommand)
{
	m_pCommandBinder->ClientCommandExecute(pszCommand);
	return true;
}

bool SPSubGameManager::ServerCommandExcute(const char* pszCommand)
{
	m_pCommandBinder->ServerCommandExecute(pszCommand);
	return true;
}



SPGMESSAGE_MAP_BEGIN(SPSubGameManager)	
	SPGMESSAGE_COMMAND( SPGM_CHAR_SC_SELECT			, OnCharSCSelect	)
	SPGMESSAGE_COMMAND( SPGM_SUG_SETBGM				, OnSetBGM			)
	SPGMESSAGE_COMMAND( SPGM_SUG_SOCKET_FAILED		, OnSocketFailed	)
	//SPGMESSAGE_COMMAND( SPGM_WORLD_SC_ENTER			, OnWorldEnter		)
	SPGMESSAGE_COMMAND( SPGM_SUG_SC_USER_IN			, OnUserIn	)
	SPGMESSAGE_COMMAND( SPGM_SUG_SC_USER_MOVE		, OnUserMove	)
	
SPGMESSAGE_MAP_END_BASE() // SPManager엔 할필요 없다.

//int SPSubGameManager::OnWorldEnter(WPARAM wparam, LPARAM lparam)
//{
//	//하위 처리용 버쳘함수
//	assert(0 && "하위 처리용 버쳘함수");
//	return -1;
//}


//////////////////////////////////////////////////////////////////////////
// OnCharSCSelect - SPLoggin->SPLobby->SPSubGameManager로 옮겨왔습니다..
//					서브게임여러군데서 쓸거 같아서..
int SPSubGameManager::OnCharSCSelect(WPARAM wparam, LPARAM lparam)
{
	/*
	* wparam = resultcode;	
	* lParam = (LPARAM)packet;
	*/	

	if(wparam != LT_NO_ERROR) {
		m_strLog = "Character Select FAIL.... Restart please.";
		return 1;
	} 

	m_strLog = "Character Select Success.";

	g_pGOBManager->SPPerformMessage(SPGM_CHAR_SC_SELECT, wparam, lparam );

	m_iGameState	= GS_RELEASE_READY;	
	m_fWaitTime = 0.0f;

	return 0;
}

int SPSubGameManager::OnSetBGM(WPARAM wparam, LPARAM lparam)
{
	//SPSoundBase::GetInstance()->SetMainBGM("../TestData/BackToTheFuture.mp3");
	SPSoundBase::GetInstance()->SetMainBGM((const char*)wparam);
	return 0;
}

int SPSubGameManager::OnSocketFailed(WPARAM wparam, LPARAM lparam)
{
	// wparam 이 0으로 넘어올 경우 글로벌 스트링을 찾지못한다. 0은 스레기값 [12/27/2007 AJJIYA]
	int iGlobalString = (int)wparam;

	if( iGlobalString <= 0 )
		iGlobalString	=	4000010;

	const char* pszErrorMsg = g_pResourceManager->GetGlobalString( iGlobalString );

	if( pszErrorMsg == NULL )
		pszErrorMsg = g_pResourceManager->GetGlobalString(4000010);
	else if( strlen( pszErrorMsg ) <= 0 )
		pszErrorMsg = g_pResourceManager->GetGlobalString(4000010);

	if( pszErrorMsg == NULL )
		return 1;

	int iType = 1;
	g_pNetworkManager->Disconnect2();
	g_pInterfaceManager->SPPerformMessage(SPIM_ERROR_SET_MSG, (WPARAM)pszErrorMsg, iType);
	return 0;
}

int SPSubGameManager::OnUserIn(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

int SPSubGameManager::OnUserMove(WPARAM wParam, LPARAM lParam)
{
	return 0;
}














