
#include <assert.h>
#include <Windows.h>


#include "SPCommon.h"

#include "SPRegistry.h"
#include "SPOptionDef.h"

#include "SPManagerDEF.h"
#include "SPManager.h"

#include "LT_Error.h"
#include "Packet.h"
#include "PacketHandler.h"
#include "SPNetworkManager.h"

#include "SPSubGameManager.h"
#include "SPMainGameDEF.H"
#include "SPMainGameManager.h"
#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowOption.h"

#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "SPUtil.h"
#include "SPLocalizeManager.h"

#include "SPClientOptionArchive.h"

SPClientOptionArchive* g_pClientOption = NULL;

extern bool g_bWindowed;

SPClientOptionArchive::SPClientOptionArchive()
: SPManager()
, m_iVer(0)
, m_bHasChanged(false)
, m_usServerSideOption(SSO_NULL)
, m_bLockReceive(false)
{	
	Init();
}

SPClientOptionArchive::~SPClientOptionArchive()
{
	Clean();
}

bool SPClientOptionArchive::Init()
{
	m_strCompanyName	=	SPLocalizeManager::GetInstance()->GetRegistryCompany();

	bool bRetVal = GetMachineRegistryKey( NULL, OPTION_VER, &m_iVer, sizeof(int), NULL , m_strCompanyName.c_str() );	

	if(bRetVal == false || m_iVer != CUR_CLIENT_OPTION_VER) {			

		m_iVer = CUR_CLIENT_OPTION_VER;
		m_OptionStruct.bExp				= true;
		m_OptionStruct.bItem			= true;
		m_OptionStruct.bUseItem			= true;
		m_OptionStruct.bHelp			= true;

		m_OptionStruct.bEnableBGM		= true;
		m_OptionStruct.bEnableSE		= true;

		m_OptionStruct.iVolumeBGM		= (int)( BGM_DEFAULT * 255.0f);
		m_OptionStruct.iVolumeSE		= (int)( SE_DEFAULT * 255.0f);
		m_OptionStruct.iGammaRamp		= GAMMA_INITIAL;
		m_OptionStruct.iEffect			= EFFECT_STEP_4;

		//m_OptionStruct.bChatParty		= true;
		//m_OptionStruct.bChatFriend		= true;
		//m_OptionStruct.bChatGuild		= true;

		//m_OptionStruct.bChatPermitFriend = true;
		//m_OptionStruct.bChatPermitWhisper = true;

		m_OptionStruct.bIndicatePlayer	= true;
		m_OptionStruct.bIndicateMonster = true;
		m_OptionStruct.bIndicateNPC		= true;
		m_OptionStruct.bIndicatePet		= true;
		
		m_OptionStruct.bSpeechTeam		= true;
		m_OptionStruct.bSpeechPlayer	= true;
		m_OptionStruct.bSpeechNPC		= true;
		m_OptionStruct.bSpeechGuild		= true;

		m_OptionStruct.bHpBarSelf		= true;	
		m_OptionStruct.bHpBarParty		= true;	
		m_OptionStruct.bHpBarPlayer		= true;	
		
		m_OptionStruct.bAutoHideMouse	= true;	
		m_OptionStruct.iScreenShot		= SCREEN_SHOT_JPG;

		//m_OptionStruct.strID			= "";
		ZeroMemory(m_szLoginID, LOGIN_ID_LEN);
		ZeroMemory(m_szLoginServer, _MAX_PATH);
		m_bHasChanged = true;

	} else {
		GetMachineRegistryKey( NULL, OPTION_BIN, &m_OptionStruct, sizeof(m_OptionStruct), NULL , m_strCompanyName.c_str() );
		ZeroMemory(m_szLoginID, LOGIN_ID_LEN);
		ZeroMemory(m_szLoginServer, _MAX_PATH);

		//m_OptionStruct.bEnableSE		= true;
		//m_OptionStruct.iVolumeSE		= 255; //(int) ( SE_DEFAULT * 255.0f);
	}

	OnCheckBGM			(m_OptionStruct.bEnableBGM	? TRUE : FALSE, 0);
	OnVolumeBGM			(m_OptionStruct.iVolumeBGM	, 0);
	OnCheckSoundEffect	(m_OptionStruct.bEnableSE	? TRUE : FALSE, 0);
	OnVolumeSoundEffect	(m_OptionStruct.iVolumeSE	, 0);
	OnVolumeGammaRamp	(m_OptionStruct.iGammaRamp	, 0);
	OnDrawHideMouse		(m_OptionStruct.bAutoHideMouse ? TRUE : FALSE, 0);

	return true;
}

void SPClientOptionArchive::Clean()
{
	//[2005/11/14] - юс╫ц
	m_bHasChanged = true;
	
	//ClientSave();
}

////
//void SPClientOptionArchive::SetDefault()
//{		
//	m_OptionStruct.bExp				= true;
//	m_OptionStruct.bItem			= true;
//	m_OptionStruct.bUseItem			= true;
//	m_OptionStruct.bHelp			= true;
//
//	m_OptionStruct.bEnableBGM		= true;
//	m_OptionStruct.bEnableSE		= true;
//	m_OptionStruct.iVolumeBGM		= (int)( BGM_DEFAULT * 255.0f);
//	m_OptionStruct.iVolumeSE		= (int)( SE_DEFAULT * 255.0f);
//	m_OptionStruct.iGammaRamp		= GAMMA_INITIAL;
//	m_OptionStruct.iEffect			= EFFECT_STEP_4;
//
//	//m_OptionStruct.bChatParty		= true;
//	//m_OptionStruct.bChatFriend		= true;
//	//m_OptionStruct.bChatGuild		= true;
//
//	//m_OptionStruct.bChatPermitFriend = true;
//	//m_OptionStruct.bChatPermitWhisper = true;
//
//	m_OptionStruct.bIndicatePlayer	= true;
//	m_OptionStruct.bIndicateMonster = true;
//	m_OptionStruct.bIndicateNPC		= true;
//	m_OptionStruct.bIndicatePet		= true;
//	
//	m_OptionStruct.bSpeechTeam		= true;
//	m_OptionStruct.bSpeechPlayer	= true;
//	m_OptionStruct.bSpeechNPC		= true;
//	m_OptionStruct.bSpeechGuild		= true;
//	
//	m_OptionStruct.bHpBarSelf		= true;	
//	m_OptionStruct.bHpBarParty		= true;	
//	m_OptionStruct.bHpBarPlayer		= true;	
//	
//	m_OptionStruct.bAutoHideMouse	= true;	
//	m_bHasChanged = true;
//	OnCheckBGM			(m_OptionStruct.bEnableBGM	? TRUE : FALSE, 0);
//	OnVolumeBGM			(m_OptionStruct.iVolumeBGM	, 0);
//	OnCheckSoundEffect	(m_OptionStruct.bEnableSE	? TRUE : FALSE, 0);
//	OnVolumeSoundEffect	(m_OptionStruct.iVolumeSE	, 0);
//	OnVolumeGammaRamp	(m_OptionStruct.iGammaRamp	, 0);
//	OnDrawHideMouse		(m_OptionStruct.bAutoHideMouse ? TRUE : FALSE, 0);	
//
//	//m_usServerSideOption = SSO_DEFAULT;
//	//OnCommitToServer(0,0);
//}


void SPClientOptionArchive::SetDefault(SPOptionStruct* pOption /*= NULL*/)
{
	//if(pOption == NULL && &m_OptionStruct)
	//	pOption = &m_OptionStruct;

	if(pOption) {
		pOption->bExp				= true;
		pOption->bItem			= true;
		pOption->bUseItem			= true;
		pOption->bHelp			= true;

		pOption->bEnableBGM		= true;
		pOption->bEnableSE		= true;
		pOption->iVolumeBGM		= (int)( BGM_DEFAULT * 255.0f);
		pOption->iVolumeSE		= (int)( SE_DEFAULT * 255.0f);
		pOption->iGammaRamp		= GAMMA_INITIAL;
		pOption->iEffect			= EFFECT_STEP_4;

		//pOption->bChatParty		= true;
		//pOption->bChatFriend		= true;
		//pOption->bChatGuild		= true;

		//pOption->bChatPermitFriend = true;
		//pOption->bChatPermitWhisper = true;

		pOption->bIndicatePlayer	= true;
		pOption->bIndicateMonster = true;
		pOption->bIndicateNPC		= true;
		pOption->bIndicatePet		= true;

		pOption->bSpeechTeam		= true;
		pOption->bSpeechPlayer	= true;
		pOption->bSpeechNPC		= true;
		pOption->bSpeechGuild		= true;

		pOption->bHpBarSelf		= true;	
		pOption->bHpBarParty		= true;	
		pOption->bHpBarPlayer		= true;	

		pOption->bAutoHideMouse	= true;	
		pOption->iScreenShot = SCREEN_SHOT_PNG;
		m_bHasChanged = true;		

		float fVar;
		Convert255To1( fVar, pOption->iVolumeBGM );	
		SPSoundBase::GetInstance()->UpdateBGMVolume( fVar*2 );
		Convert255To1( fVar, pOption->iVolumeSE );	
		SPSoundBase::GetInstance()->UpdateEffectVolume( fVar );
		
		if(g_bWindowed == false) {			
			g_pVideo->SetContrast( pOption->iGammaRamp );
		}
	}
}


void SPClientOptionArchive::OptionCopy(SPOptionStruct* stDest, SPOptionStruct* stSrc /*= NULL*/)
{
	if(stSrc == NULL) {
		stSrc = &m_OptionStruct;
	}

	if(stDest && stSrc) {
		stDest->bExp							=	stSrc->bExp;
		stDest->bItem							=	stSrc->bItem;
		stDest->bUseItem						=	stSrc->bUseItem;
		stDest->bHelp							=	stSrc->bHelp;

		stDest->bEnableBGM						= 	stSrc->bEnableBGM;
		stDest->bEnableSE						=	stSrc->bEnableSE;

		stDest->iVolumeBGM						=	stSrc->iVolumeBGM;
		stDest->iVolumeSE						=	stSrc->iVolumeSE;
		stDest->iGammaRamp						=	stSrc->iGammaRamp;
		stDest->iEffect							=	stSrc->iEffect;

		//stDest->bChatParty						=	stSrc->bChatParty;
		//stDest->bChatFriend						=	stSrc->bChatFriend;
		//stDest->bChatGuild						=	stSrc->bChatGuild;

		//stDest->bChatPermitFriend				=	stSrc->bChatPermitFriend;
		//stDest->bChatPermitWhisper				=	stSrc->bChatPermitWhisper;

		stDest->bIndicatePlayer					=	stSrc->bIndicatePlayer;
		stDest->bIndicateMonster				=	stSrc->bIndicateMonster;
		stDest->bIndicateNPC					=	stSrc->bIndicateNPC;
		stDest->bIndicatePet					=	stSrc->bIndicatePet;

		stDest->bSpeechPlayer					=	stSrc->bSpeechPlayer;
		stDest->bSpeechTeam						=	stSrc->bSpeechTeam;
		stDest->bSpeechNPC						=	stSrc->bSpeechNPC;
		stDest->bSpeechGuild					=	stSrc->bSpeechGuild;

		stDest->bHpBarSelf						=	stSrc->bHpBarSelf;
		stDest->bHpBarParty						=	stSrc->bHpBarParty;
		stDest->bHpBarPlayer					=	stSrc->bHpBarPlayer;

		stDest->bAutoHideMouse					=	stSrc->bAutoHideMouse;
		stDest->iScreenShot						=	stSrc->iScreenShot;
	}	
}


void SPClientOptionArchive::SetServerSideOption(const SERVER_SIDE_OPTION iServerOption)
{
	m_usServerSideOption = iServerOption;
}


const SERVER_SIDE_OPTION SPClientOptionArchive::GetServerSideOption()
{
	return m_usServerSideOption;
}

SPOptionStruct* SPClientOptionArchive::GetOptionStruct()
{
	return &m_OptionStruct;
}


bool SPClientOptionArchive::ClientSave()
{
	//if(m_bHasChanged == false) 
	//	return true;

	//
	if( g_pMouseCursor )
		g_pMouseCursor->SetAutoHide(m_OptionStruct.bAutoHideMouse);

	SPSoundBase::GetInstance()->EnableBGM( m_OptionStruct.bEnableBGM );	
	SPSoundBase::GetInstance()->EnableEffect( m_OptionStruct.bEnableSE );	

	if(SetMachineRegistryKey(OPTION_VER, &m_iVer, sizeof(int) , m_strCompanyName.c_str() )) {
		return SetMachineRegistryKey(OPTION_BIN, &m_OptionStruct, sizeof(m_OptionStruct) , m_strCompanyName.c_str() );
	}	
	return false;
}


bool SPClientOptionArchive::SaveUserID(std::string strID /*= ""*/)
{	
	strncpy(m_szLoginID, strID.c_str(), LOGIN_ID_LEN-1);
	return SetMachineRegistryKeyString(LAST_ID, m_szLoginID, sizeof(char) * LOGIN_ID_LEN , m_strCompanyName.c_str() );
}


bool SPClientOptionArchive::LoadUserID(std::string& strID)
{
	GetMachineRegistryKeyString( NULL, LAST_ID, &m_szLoginID,	sizeof(char)*LOGIN_ID_LEN,	NULL , m_strCompanyName.c_str() );
	strID = m_szLoginID;
	return true;
	//return m_OptionStruct.strID;
}

bool SPClientOptionArchive::SaveLastLoginServer( std::string strLoginServer )
{	
	strncpy(m_szLoginServer, strLoginServer.c_str(), _MAX_PATH-1 );
	return SetMachineRegistryKeyString(LAST_LOGINSERVER, m_szLoginServer, sizeof(char) * _MAX_PATH , m_strCompanyName.c_str() );
}


bool SPClientOptionArchive::LoadLastLoginServer( std::string& strLoginServer )
{
	GetMachineRegistryKeyString( NULL, LAST_LOGINSERVER, &m_szLoginServer,	sizeof(char)*_MAX_PATH,	NULL , m_strCompanyName.c_str() );
	strLoginServer = m_szLoginServer;
	return true;
}

bool SPClientOptionArchive::IsTrade()
{
	return (m_usServerSideOption & SSO_REQ_TRADE)? true : false;
}


bool SPClientOptionArchive::IsTeam()
{
	return (m_usServerSideOption & SSO_REQ_TEAM)? true : false;
}


bool SPClientOptionArchive::IsAlert()
{
	return (m_usServerSideOption & SSO_REQ_ALERT)? true : false;
}


bool SPClientOptionArchive::IsWantTeam()
{
	return (m_usServerSideOption & SSO_REQ_WANTTEAM)? true : false;
}


bool SPClientOptionArchive::IsWhisper()
{
	return (m_usServerSideOption & SSO_REQ_WHISPER)? true : false;
}


bool SPClientOptionArchive::IsChatTeam()
{
	return (m_usServerSideOption & SSO_REQ_CHATTEAM)? true : false;
}


bool SPClientOptionArchive::IsChatFriend()
{
	return (m_usServerSideOption & SSO_REQ_CHATFRIEND)? true : false;
}


bool SPClientOptionArchive::IsChatGuild()
{
	return (m_usServerSideOption & SSO_REQ_CHATGUILD)? true : false;
}


int	SPClientOptionArchive::SPPerformMessage	(UINT msg, WPARAM wparam, LPARAM lParam)
{
	return SPOAMESSAGE_FUNC( msg, wparam, lParam);	
}



SPOAMESSAGE_MAP_BEGIN(SPClientOptionArchive)
	SPOAMESSAGE_COMMAND(SPOAM_COMMIT_TO_SERVER		, OnCommitToServer		)

	////
	//SPOAMESSAGE_COMMAND(SPOAM_NAME_PLAYER		, OnDrawNamePlayer		)
	//SPOAMESSAGE_COMMAND(SPOAM_NAME_MONSTER		, OnDrawNameMonster		)
	//SPOAMESSAGE_COMMAND(SPOAM_NAME_NPC			, OnDrawNameNpc			)
	//SPOAMESSAGE_COMMAND(SPOAM_SPEECH_PLAYER		, OnDrawSpeechPlayer	)
	//SPOAMESSAGE_COMMAND(SPOAM_SPEECH_TEAM		, OnDrawSpeechTeam		)
	//SPOAMESSAGE_COMMAND(SPOAM_SPEECH_NPC		, OnDrawSpeechNpc		)
	//SPOAMESSAGE_COMMAND(SPOAM_COMM_TRADE		, OnDrawCommTrade		)
	//SPOAMESSAGE_COMMAND(SPOAM_COMM_INVITE		, OnDrawCommInvite		)
	//SPOAMESSAGE_COMMAND(SPOAM_COMM_ALERT		, OnDrawCommAlert		)	
	
	SPOAMESSAGE_COMMAND(SPOAM_HIDE_MOUSE		, OnDrawHideMouse		)
	SPOAMESSAGE_COMMAND(SPOAM_BTN_BGM			, OnCheckBGM			)
	SPOAMESSAGE_COMMAND(SPOAM_SLD_BGM			, OnVolumeBGM			)
	SPOAMESSAGE_COMMAND(SPOAM_BTN_SOUNDEFFECT	, OnCheckSoundEffect	)
	SPOAMESSAGE_COMMAND(SPOAM_SLD_SOUNDEFFECT	, OnVolumeSoundEffect	)
	SPOAMESSAGE_COMMAND(SPOAM_SLD_GAMMARAMP		, OnVolumeGammaRamp		)
SPOAMESSAGE_MAP_END_BASE()

////
//int SPClientOptionArchive::OnDrawNamePlayer		(WPARAM wParam, LPARAM lparam)
//{
//	m_OptionStruct.bIndicatePlayer = (wParam ? true : false);
//	// TODO : Event Process Here
//
//	return 1;
//}

////
//int SPClientOptionArchive::OnDrawNameMonster	(WPARAM wParam, LPARAM lparam)
//{
//	m_OptionStruct.bIndicateMonster = (wParam ? true : false);
//	// TODO : Event Process Here
//
//	return 1;
//}

////
//int SPClientOptionArchive::OnDrawNameNpc		(WPARAM wParam, LPARAM lparam)
//{
//	m_OptionStruct.bIndicateNPC = (wParam ? true : false);
//	// TODO : Event Process Here
//
//	return 1;
//}

////
//int SPClientOptionArchive::OnDrawSpeechPlayer	(WPARAM wParam, LPARAM lparam)
//{
//	m_OptionStruct.bSpeechPlayer = (wParam ? true : false);
//	// TODO : Event Process Here
//
//	return 1;
//}

////
//int SPClientOptionArchive::OnDrawSpeechTeam		(WPARAM wParam, LPARAM lparam)
//{
//	m_OptionStruct.bSpeechTeam = (wParam ? true : false);
//	// TODO : Event Process Here
//
//	return 1;
//}

////
//int SPClientOptionArchive::OnDrawSpeechNpc		(WPARAM wParam, LPARAM lparam)
//{
//	m_OptionStruct.bSpeechNPC = (wParam ? true : false);
//	// TODO : Event Process Here
//	return 1;
//}

////
//int SPClientOptionArchive::OnDrawCommTrade		(WPARAM wParam, LPARAM lparam)
//{
//	if(wParam) {
//		m_usServerSideOption |= SSO_REQ_TRADE;
//	} else {
//		m_usServerSideOption &= ~SSO_REQ_TRADE;
//	}
//	return 1;
//}

////
//int SPClientOptionArchive::OnDrawCommInvite		(WPARAM wParam, LPARAM lparam)
//{
//	if(wParam) {
//		m_usServerSideOption |= SSO_REQ_TEAM;
//	} else {
//		m_usServerSideOption &= ~SSO_REQ_TEAM;
//	}
//	return 1;
//}

////
//int SPClientOptionArchive::OnDrawCommAlert		(WPARAM wParam, LPARAM lparam)
//{
//	if(wParam) {
//		m_usServerSideOption |= SSO_REQ_ALERT;
//	} else {
//		m_usServerSideOption &= ~SSO_REQ_ALERT;
//	}
//	return 1;
//}


int SPClientOptionArchive::OnCommitToServer		(WPARAM wParam, LPARAM lparam)
{	
	//TODO : Commit process Here
	CPacket	Packet;
	Packet.AddUINT32( GAME_CS_SETOPTION );
	//Packet.AddUINT16( m_usServerSideOption );
	Packet.AddUINT32( m_usServerSideOption );
	( g_pNetworkManager->GetPacketHandler() )->PostMsg( &Packet );
	return 1;
}


int SPClientOptionArchive::OnDrawHideMouse		(WPARAM wParam, LPARAM lparam)
{
	m_OptionStruct.bAutoHideMouse = (wParam ? true : false);
	// TODO : Event Process Here
	
	if( g_pMouseCursor )
		g_pMouseCursor->SetAutoHide(m_OptionStruct.bAutoHideMouse);
	return 1;
}


int SPClientOptionArchive::OnCheckBGM			(WPARAM wParam, LPARAM lparam)
{
	m_OptionStruct.bEnableBGM = (wParam ? true : false);	
	SPSoundBase::GetInstance()->EnableBGM( m_OptionStruct.bEnableBGM );	
	return 1;
}


int SPClientOptionArchive::OnVolumeBGM			(WPARAM wParam, LPARAM lparam)
{
	m_OptionStruct.iVolumeBGM = (int)wParam;

	float fVar;
	Convert255To1( fVar, m_OptionStruct.iVolumeBGM );	
	SPSoundBase::GetInstance()->UpdateBGMVolume( fVar*2 );
	return 1;
}


int SPClientOptionArchive::OnCheckSoundEffect	(WPARAM wParam, LPARAM lparam)
{
	m_OptionStruct.bEnableSE = (wParam ? true : false);
	SPSoundBase::GetInstance()->EnableEffect( m_OptionStruct.bEnableSE );	
	return 1;
}


int SPClientOptionArchive::OnVolumeSoundEffect	(WPARAM wParam, LPARAM lparam)
{
	m_OptionStruct.iVolumeSE = (int)wParam;

	float fVar;
	Convert255To1( fVar, m_OptionStruct.iVolumeSE );
	//SPSoundBase::GetInstance()->UpdateBGMVolume( fVar );
	SPSoundBase::GetInstance()->UpdateEffectVolume( fVar );
	return 1;
}


int SPClientOptionArchive::OnVolumeGammaRamp	(WPARAM wParam, LPARAM lparam)
{
	m_OptionStruct.iGammaRamp = (int)wParam;
	g_pVideo->SetContrast( m_OptionStruct.iGammaRamp );
	return 1;
}













