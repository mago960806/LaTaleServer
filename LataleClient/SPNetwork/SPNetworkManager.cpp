#include "stdafx.h"

#include "SPCommon.h"

#include "GlobalDefines_Share.h"
#include "SPLocalizeManager.h"

#include "SPGameObjectDEF.h"
#include "SPManagerDef.h"
#include "SPManager.h"
#include "SPGOBManager.h"
#include "SPTerrainDEF.h"
#include "SPTerrainCluster.h"
#include "SPTerrainManager.h"

#include "SPGOBClusterDEF.h"
#include "SPGameObjectDEF.h"

#include "SPAvatarModelUnitDef.h"

#include "SPGameObject.h"
#include "SPItemAttr.h"
#include "SPItemStatus.h"
//#include "SPItem.h"
#include "SPItemCluster.h"				//[2005/6/16] - jinhee
#include "SPGOBStatus.h"
#include "SPPlayerStatus.h"
#include "SPPlayerStatusModel.h"
#include "SPGOBCoordPhysics.h"
#include "SPPlayer.h"

#include "SPGOBCluster.h"
#include "SPGOBModelUnit.h"

#include <WinSock2.h>
#include <process.h>

#include "Packet.h"
#include "PacketHandler.h"
#include "SockWnd.h"
#include "SPNetworkDEF.h"
#include "SPNetworkManager.h"

#include "SPSubGameManager.h"

// 캐릭터네임
#include "LataleClient.h"

#include "DebugPrint.h"

#include "LataleVersionDef.h"

#include "SPResourceDEF.h"
#include "SPResourceBase.h"

#include "lgclientlog.h"

// LDT 
//{
	#include "SPLDTFile.h"
	#include "SPLDTManager.h"
	#include "SPResourceDef.h"
	#include "SPResourceManager.h"

	#include "SPCommandDef.h"
	#include "SPClientCommandBinder.h"
//}


// Latale Cipher
//{
	#include "LataleCipherDefines.h"
	#include <Windows.h>

	#ifdef USING_LATALE_CIPHER
		#if 0
			#pragma comment( lib , "./NPGameGuard/NPGameLib" )
		#else
			#include <tchar.h>
			#include "LataleCipher.h"

			#ifndef LATALE_CIPHER_DLL
				#ifdef _DEBUG
					#pragma comment( lib , "./SPNetwork/LIB/SDCltDynCodeMTd.lib" )
					#pragma comment( lib , "./SPNetwork/LIB/LCD_3.lib" )
				#else
					#pragma comment( lib , "./SPNetwork/LIB/SDCltDynCodeMT.lib" )
					#pragma comment( lib , "./SPNetwork/LIB/LC_3.lib" )
				#endif
			#endif
		#endif
	#endif
//}

FLOAT  g_fFPS;

SPNetworkManager* g_pNetworkManager = NULL;

int g_iCurrentCipherMode = 0;

extern void packetParsing(CPacket* packet);

// Server Info(IP,PORT)

WORLD_ID g_uiWorldID = 1;

char g_szGameServerIP[MAX_PATH] = {SERVER_IPADDR};
int g_iGameServerPort = GAMESERVER_PORT;

char g_szLobbyServerIP[MAX_PATH] = {""};
int g_iLobbyServerPort = 0;
bool g_bForceLobbyServer = false;

char g_szLoginServerIP[MAX_PATH] = {""};
int g_iLoginServerPort = 0;
int g_iUserID = 0;

////////////////////////////////////////////////////////////////////////////////

SPNetworkManager::SPNetworkManager()
: SPManager()
, m_pSockWnd(NULL)
, m_pPacketHandler(NULL)
, m_bMapReady(false)
, m_bVersionCheckPassed(false)
, m_bExceptSocketFailed(false)
, m_iDisconnectCount(0)
, m_eSite(SITE_LOCAL)
, m_eCurProcessStep(STEP_NONE)
, m_eNextProcessStep(STEP_NONE)
, m_bCipherInitialized(false)
, m_bReceived_PROTOCOL_SC_START(false)
, m_uiFDS_Param1(0)
, m_uiFDS_Param2(0)
, m_bChannelEntering(false)
{
	SetMgrID(MGR_ID_NETWORK);
	g_pNetworkManager = this;		
}

SPNetworkManager::~SPNetworkManager()
{
	Exit();
	Disconnect();
	
	g_pNetworkManager = NULL;

	while(!m_qSubLogicCommandBuffer.empty())
	{
#ifdef _DEBUG
		static int i=0;
		string&  sCommand = m_qSubLogicCommandBuffer.front();
		DebugPrint("SubLogicCmdBuffer(%d) : [%s]\n",++i,sCommand.c_str());
#endif
		m_qSubLogicCommandBuffer.pop();
	}
}

bool SPNetworkManager::IsConnected()
{	
	if(!m_pSockWnd) 
		return false; 
	return m_pSockWnd->IsConnected();
}

void SPNetworkManager::Process(float fTime)
{
	SPManager::Process(fTime);	

	if(SubLogic_Loop(fTime) < 0)
	{
		// To Do...
	}

	if(m_pSockWnd && !m_pSockWnd->IsConnected()) return;

	if(m_bCipherInitialized)
	{
		switch(m_eCurProcessStep)
		{
			case STEP_CONNECT:
				{
					if(m_bReceived_PROTOCOL_SC_START)
						SetCurProcessStep(GetNextProcessStep());
				}
				break;
			case STEP_AUTH_EX:
				{
					AuthUserEx_Real(m_sProcessStepParams[0].c_str(),m_sProcessStepParams[1].c_str());
					SetCurProcessStep(STEP_LOGIN);
					SetNextProcessStep(STEP_NONE);
				}
				break;
			case STEP_AUTH_KEYLOGIN:
				{
					AuthUser_KeyLogin_Real(m_sProcessStepParams[0].c_str(),m_sProcessStepParams[1].c_str());
					SetCurProcessStep(STEP_LOGIN);
					SetNextProcessStep(STEP_NONE);
				}
				break;
			case STEP_LOGIN:
				{
					if( m_sProcessStepParams[1].empty() == false )
					{
						m_sProcessStepParams[1].assign( m_sProcessStepParams[1].size() , ' ' );
						m_sProcessStepParams[1].clear();
					}
				}
				break;
			case STEP_LOBBY:
				{
				}
				break;
			case STEP_PLAYING:
				{
				}
				break;
			default:
				break;
		}
	}

	if(m_pPacketHandler) 
	{	
		m_pPacketHandler->Parsing();			// 패킷 파싱해서 핸들하기.
		if(!m_pPacketHandler->Sending())		// 패킷 큐에 있는 모든 내용을 서버로 보낸다.
		{
			// 서버와의 연결 (X)
			return ;
		}

		KeepAliveCheck(fTime);
	}
}

void SPNetworkManager::SendProcess(float fTime)
{
	SPManager::Process(fTime);		

	if(m_pSockWnd && !m_pSockWnd->IsConnected())
		return;

	if(m_pPacketHandler) 
	{	
		if( !m_pPacketHandler->Sending() )
			return ;

		KeepAliveCheck(fTime);
	}
}

void SPNetworkManager::KeepAliveCheck(float fTime)
{
#ifndef _DEBUG
	static float fPeriodTime = KEEP_ALIVE_TIMEOUT/1000.f; 
	static float fCurTime = 0.f;

	if(fPeriodTime <= fCurTime)
	{
		CPacket packet(KEEP_ALIVE_PACKET);

		packet.AddFloat(g_fFPS);

		GetPacketHandler()->PostMsg(&packet);

		fCurTime = 0.f;
	}
	else
		fCurTime += fTime;
#endif
}

bool SPNetworkManager::Init()
{
	SetTraceLevel(1);

	m_eSite = SPLocalizeManager::GetInstance()->GetSiteType();


#ifdef USING_LATALE_CIPHER
#if 0
	DWORD dwError = InitPacketProtect(LATALE_CIPHER_KEY);		// 0(fail), 1(success)
	if(dwError == 0)
	{
		assert(_T("InitPacketProtect() failed"));
		MessageBox( NULL , _T("InitPacketProtect() failed.") , _T("Packet Encryption Library Loading...") , MB_OK );
		return false;
	}
#else

	if(m_eSite == SITE_SANDA)
	{
		g_iCurrentCipherMode = CIPHER_LIB_SNDC;
	}
	else
	{
		g_iCurrentCipherMode = LATALE_SECURITY_KEY;
	}

	if(!Cipher_Init(g_iCurrentCipherMode))
	{
		assert(_T("Cipher_Init() failed"));
		MessageBox( NULL , _T("Cipher_Init() failed.") , _T("Packet Encryption Library Loading...") , MB_OK );
		return false;
	}

#endif
#endif


	ZeroMemory(&m_UserID,	sizeof(USERID));
	ZeroMemory(&m_Passport, sizeof(PASSPORT));
	ZeroMemory(&m_InPassport, sizeof(PASSPORT));

	m_bVersionCheckPassed = false;
	m_bFDSNotice = false ;

	m_uiWorldID = 0;
	m_uiServerID = 0;
	m_uiPreviousServerID = 0;
	m_uiGameServerID = 0;

	m_sGameServerIP.clear();
	m_uiGameServerPort = 0;
	m_sPreviousGameServerIP.clear();
	m_uiPreviousGameServerPort = 0;

	m_sCharName.clear();
	
	m_strIGWLoginID.clear();
	m_strIGWPW.clear();
	m_strIGWEkey.clear();

	if(g_szLoginID[0] == '\0')
	{
		// appended by hoshim at 2005/10/14
		// 개발용 코드 : 게임 서버로 직접 접속하는 경우, 실행인자로 "/LoginID:<로긴ID>"를 설정하지 않은 경우,
		//					중복 로긴 체크용 로긴 ID로 사용됨.
		_stprintf(g_szLoginID,"%d",getpid());
	}
	m_strLoginID = g_szLoginID;

	if( g_iUserID )
		m_UserID = g_iUserID;

	if(!CreateHandler())
		return false;

	m_bSendUserMoveRequest = false;
	m_iIndunID = 0;

	ResetReceived_PROTOL_SC_START();

	if(SubLogic_Init() < 0) 
		return false;

	m_uiFDS_Param1 = 0;
	m_uiFDS_Param2 = 0;

	m_bChannelEntering = false;

	return SPManager::Init();
}

void SPNetworkManager::Exit()
{
	SubLogic_Exit();

	ClearLoginServerInfo();

#ifdef USING_LATALE_CIPHER
	Cipher_Exit();
#endif
}

void SPNetworkManager::ClearLoginServerInfo()
{
	CServerInfo* pInfo = NULL;
	vector<CServerInfo*>::iterator pos = m_vLoginServerInfo.begin();
	while(pos != m_vLoginServerInfo.end())
	{	
		pInfo = (CServerInfo*)(*pos);
		if(pInfo)
			delete pInfo;
		++pos;
	}
	m_vLoginServerInfo.clear();
}


void SPNetworkManager::Disconnect()
{
	if(m_pSockWnd)
		m_pSockWnd->Exit();

	SAFE_DELETE( m_pSockWnd );
	SAFE_DELETE( m_pPacketHandler );
	DebugPrint("SPNetworkManager::Disconnect()\n");
}

void SPNetworkManager::Disconnect2()
{
	m_bExceptSocketFailed = true;
	m_bSendUserMoveRequest = false;
	if(m_pSockWnd)
	{
		if( m_pSockWnd->IsConnected() )
			m_iDisconnectCount++;

		m_pSockWnd->Disconnect();
	}

	DebugPrint("SPNetworkManager::Disconnect2(%d)\n",WSAGetLastError());
}

void SPNetworkManager::ResetDisconnectCount()
{
	m_iDisconnectCount	=	0;
}

//Step1 : 우선 실행 인자(/LobbyServer:<ipAddress>:<port>)내용을 확인, 우선 선택
//Step2 : SERVER.LDT 파일 내용 참조
bool SPNetworkManager::LoadLoginServerInfo()
{
	ClearLoginServerInfo();

	if(g_szLoginServerIP[0] != '\0' && g_iLoginServerPort != 0)
	{
		m_vLoginServerInfo.push_back(new CServerInfo(g_szLoginServerIP,g_iLoginServerPort));
	}
	else
	{
		if(!m_vLoginServerInfo.empty())
		{
			Exit();		// m_vLoginServerInfo를 초기화하기 위해 호출.
		}

		SPLDTFile* pLDTFile;
		g_pResourceManager->GetLDTFromFileName(RES_FNAME_LDT_LOGINSERVER_INFO, &pLDTFile);	

		if(pLDTFile == NULL) return false;

		int recordCount = pLDTFile->GetItemCount();
		int fieldCount = pLDTFile->GetFieldCount();
		LDT_Field ldtField1;
		LDT_Field ldtField2;
		LDT_Field ldtField3;
		LDT_Field ldtField4;

		int		iItemID;

	#ifdef _DEBUG
		DebugPrint("=======================================================================================\n");
		DebugPrint("LoginServerInfo : ID,Name,IPAddress,Port\n");
	#endif

		for( int record = 0; record < recordCount; record++ )
		{
			iItemID = pLDTFile->GetPrimaryKey( record );

			//_ID				로긴 서버 ID	num
			//_NAME				서버 이름		txt
			//_IP				ip address		txt
			//_PORT				port #			num

			pLDTFile->GetFieldFromLable( iItemID, "_ID",	ldtField1);
			pLDTFile->GetFieldFromLable( iItemID, "_NAME",	ldtField2); 
			pLDTFile->GetFieldFromLable( iItemID, "_IP",	ldtField3); 
			pLDTFile->GetFieldFromLable( iItemID, "_PORT",	ldtField4);

			if(ldtField4.uData.lValue == 0)
				break;
	#ifdef _DEBUG
			DebugPrint("ServerID=[%d] [%s] [%s:%d]\n",ldtField1.uData.lValue,ldtField2.uData.pValue,ldtField3.uData.pValue,ldtField4.uData.lValue);
	#endif
			m_vLoginServerInfo.push_back(new CServerInfo(ldtField3.uData.pValue,ldtField4.uData.lValue));

		}
	#ifdef _DEBUG
		DebugPrint("=======================================================================================\n");
	#endif
		SAFE_RELEASE(pLDTFile);

	}
	return true;
}

bool SPNetworkManager::CreateHandler()
{
	m_sCurServerIP.clear();
	m_iCurServerPort = 0;

	m_pSockWnd = new CSockWnd();
	if(!m_pSockWnd) 
	{
		assert(false && "CSockWnd Creation failure.");
		return false;
	}

	if(!m_pSockWnd->Init(g_hWnd,g_hInst))
	{
		assert(false && "CSockWnd::Init() failure.");
		SAFE_DELETE( m_pSockWnd );
		return false;
	}

	m_pPacketHandler = new CPacketHandler(m_pSockWnd,packetParsing);
	if(m_pPacketHandler == NULL)
	{		
		SC_ERROR("NETWORK ERROR : Socket/Handle Error\n");

		SAFE_DELETE( m_pSockWnd );
		SAFE_DELETE( m_pPacketHandler );
		return false;
	}

	return true;
}

bool SPNetworkManager::ConnectToLoginServer()
{
	assert(m_pSockWnd);

	ResetReceived_PROTOL_SC_START();

	SetCurProcessStep(STEP_CONNECT);

	// 로긴 서버 목록 얻기 : 실행인자,LDT ==> m_vLoginServerInfo
	LoadLoginServerInfo();

	if(m_pSockWnd->IsConnected())
	{
		m_pSockWnd->Disconnect();
	}

	if(m_eSite == SITE_SANDA)
		m_bCipherInitialized = false;
	else
		m_bCipherInitialized = true;

	// 목록상의 로긴서버들에 접속 시도
	if(!m_pSockWnd->IsConnected())
	{
		CServerInfo* pInfo = NULL;
		vector<CServerInfo*>::const_iterator pos = m_vLoginServerInfo.begin();
		for(;pos!=m_vLoginServerInfo.end();++pos)
		{
			pInfo = (CServerInfo*)(*pos);
			if(pInfo && m_pSockWnd->Connect(pInfo->sIP.c_str(),pInfo->iPort))
			{
				m_sCurServerIP = pInfo->sIP;
				m_iCurServerPort = pInfo->iPort;
				break;
			}
		}
	}
	if(!m_pSockWnd->IsConnected())
	{
		SC_ERROR("NETWORK ERROR : Can't connect a login server.\n");
		return false;
	}

	m_bExceptSocketFailed = false;
	return true;
}

bool SPNetworkManager::ConnectToServer(const char* pszServerID,const int iServerPort)
{
	assert(pszServerID);

	ResetReceived_PROTOL_SC_START();

	SetCurProcessStep(STEP_CONNECT);

	if(m_pSockWnd->IsConnected())
	{
		m_pSockWnd->Disconnect();
	}

	Sleep(100);

	m_bExceptSocketFailed = false;

	if(m_eSite == SITE_SANDA)
		m_bCipherInitialized = false;
	else
		m_bCipherInitialized = true;

	if(m_pSockWnd->Connect(pszServerID,iServerPort) == FALSE)
	{
		return false;
	}

	return true;
}


// 버전 확인 
bool SPNetworkManager::CheckVersion(UINT16 uiVersionMajor,UINT16 uiVersionMinor)
{
	//if(!m_pSockWnd->IsConnected())
	//{
	//	if(!ConnectToLoginServer())	
	//	{
	//		return false;
	//	}
	//}

	////UINT16 uiVersionMajor = VER_MAJOR;
	////UINT16 uiVersionMinor = VER_MINOR;
	//UINT32 uiOption = 0;

	//CPacket packet;
	//packet.AddUINT32(PROTOCOL_CS_VERSION_CHECK);
	//packet.AddUINT16(uiVersionMajor);
	//packet.AddUINT16(uiVersionMinor);
	//packet.AddUINT32(uiOption);

	//GetPacketHandler()->PostMsg(&packet);

	return true;
}


bool SPNetworkManager::AuthUser_KeyLogin(const char* pszKeyID, const char* pszStartTime)
{
	if(!ConnectToLoginServer())	
	{
		return false;
	}

	FDS_SetNotice(false) ;
	SetNextProcessStep(STEP_AUTH_KEYLOGIN);
	SetProcessStepParams(pszKeyID,pszStartTime);

	return true;
}

// 로긴 서버에 인증 요청
bool SPNetworkManager::AuthUser_KeyLogin_Real(const char* pszKeyID, const char* pszStartTime)
{
	// Login
	TCHAR szKeyID[MAX_PATH] = {0,};
	TCHAR szStartTime[MAX_PATH] = {0,};

	UINT8 lenKeyID = min(MAX_PATH,(int)_tcslen(pszKeyID)+1);
	UINT8 lenStartTime = min(MAX_PATH,(int)_tcslen(pszStartTime)+1);
	_tcsncpy(szKeyID,pszKeyID,lenKeyID);
	_tcsncpy(szStartTime,pszStartTime,lenStartTime);
	m_strLoginID = "";

	CPacket packet;
	packet.AddUINT32(AUTH_CS_KEYLOGIN);
	packet.AddUINT8(lenKeyID);
	packet.AddData(szKeyID,lenKeyID);
	packet.AddUINT8(lenStartTime);
	packet.AddData(szStartTime,lenStartTime);
	packet.AddUINT16((UINT16)VER_MAJOR);
	packet.AddUINT16((UINT16)VER_MINOR);
	packet.AddUINT32((UINT32)0);

	AuthUser_AppendResVersionInfo(packet);

	GetPacketHandler()->PostMsg(&packet);

	if( m_sProcessStepParams[1].empty() == false )
	{
		m_sProcessStepParams[1].assign( m_sProcessStepParams[1].size() , ' ' );
		m_sProcessStepParams[1].clear();
	}

	return true;
}

bool SPNetworkManager::AuthUserEx(const char* pszParam0,const char* pszParam1)
{
	if(!ConnectToLoginServer())	
	{
		return false;
	}

	FDS_SetNotice(false) ;
	SetNextProcessStep(STEP_AUTH_EX);
	SetProcessStepParams(pszParam0,pszParam1);

	return true;
}

// param0(ID), param1(PW or AuthCode)
bool SPNetworkManager::AuthUserEx_Real(const char* pszParam0,const char* pszParam1)
{
	UINT8 lenParam[2] = {0,};

	lenParam[0] = _tcslen(pszParam0);		// ID
	lenParam[1] = _tcslen(pszParam1);		// PW or AUTHCODE

	CPacket packet;

	packet.AddUINT32(AUTH_CS_LOGIN_EX);
	packet.AddUINT32((UINT32)0);
	packet.AddUINT16((UINT16)VER_MAJOR);
	packet.AddUINT16((UINT16)VER_MINOR);

	// ID,PW
	if(lenParam[0] < 1) return false;
	packet.AddUINT8(lenParam[0]+1);
	packet.AddData(pszParam0,lenParam[0]+1);		// ID

	if(lenParam[1] < 1) return false;
	packet.AddUINT8(lenParam[1]+1);
	packet.AddData(pszParam1,lenParam[1]+1);		// PW or AUTHCODE

	m_strLoginID = pszParam0;
	
	m_strIGWLoginID = pszParam0;
	m_strIGWPW		= pszParam1;

	AuthUser_AppendResVersionInfo(packet);

	GetPacketHandler()->PostMsg(&packet);

	if( m_sProcessStepParams[1].empty() == false )
	{
		m_sProcessStepParams[1].assign( m_sProcessStepParams[1].size() , ' ' );
		m_sProcessStepParams[1].clear();
	}

#ifdef _DEBUG
	DebugPrint("AUTH_CS_LOGIN_EX : Server=[%s:%d] ID=[%s] PW=[%s]\n",m_sCurServerIP.c_str(),m_iCurServerPort,GetLoginID(),pszParam0);
#endif	

	return true;
}

void SPNetworkManager::AuthUser_AppendResVersionInfo(CPacket& packet)
{
	UINT8	uiCount = 0;
	UINT32	uiVersion = 0;
	UINT8	uiFileNameLen = 0;

	// SPF 리소스 정보 추출 [7/4/2007 AJJIYA]
	SPResourceBase*	pResourceManager = SPResourceBase::GetInstance();

	if( pResourceManager != NULL )
	{
		uiCount = pResourceManager->GetNoSPF();
		packet.AddUINT8(uiCount);
		if(uiCount > 0)
		{
			F_SPF_VERSION	stSPFVersion	=	0;
			const char*		pszSPFFileName	=	NULL;

			// 1 base index
			for( int i = 1 ; i <= uiCount ; ++i )
			{
				uiVersion		= pResourceManager->GetSPFVersionByInx( i );
				pszSPFFileName	= pResourceManager->GetSPFNameByInx( i );
				uiFileNameLen	= _tcslen(pszSPFFileName) +1;

				packet.AddUINT32(uiVersion);
				packet.AddUINT8(uiFileNameLen);
				packet.AddData(pszSPFFileName,uiFileNameLen);
			}
		}
	}
}


bool SPNetworkManager::AuthUser_EKeyInfo(const char* pszChallenge,const char* pszEKey)
{
	UINT8 lenChallenge = _tcslen(pszChallenge);		
	UINT8 lenEKey= _tcslen(pszEKey);		

	if(lenChallenge > 0)
		lenChallenge++;				// include NULL
	if(lenEKey > 0)
		lenEKey++;
	else return false;
	
	m_strIGWEkey = pszEKey;

	CPacket packet;
	packet.AddUINT32(AUTH_CS_EKEY);

	// CHALLENGE,E-KEY
	packet.AddUINT8(lenChallenge);
	if(lenChallenge > 0)
		packet.AddData(pszChallenge,lenChallenge);		
	packet.AddUINT8(lenEKey);
	packet.AddData(pszEKey,lenEKey);	

	GetPacketHandler()->PostMsg(&packet);

	return true;
}


bool SPNetworkManager::AuthUser_EKeySN(const const char* pszEKeySN)
{
	int lenEKeySN= _tcslen(pszEKeySN);		
	if(lenEKeySN != 6)
	{
#ifdef _DEBUG
		DebugPrint("AuthUser_EKeySN : EKeySN=[%s] is invalid size.\n",pszEKeySN);
#endif			
		return false;
	}

	CPacket packet;
	packet.AddUINT32(AUTH_CS_EKEY_SN);
	packet.AddData(pszEKeySN,7);			// include NULL	
	GetPacketHandler()->PostMsg(&packet);

	return true;
}

bool SPNetworkManager::EnterWorld()
{
	if( !m_pSockWnd->IsConnected() )
		return false;

	CPacket packet;
	packet.AddUINT32(WORLD_CS_ENTER);
	packet.AddUINT32(GetUserID());
	packet.AddUINT32(GetPassport());
	packet.AddUINT16(g_uiWorldID);

	string sCharName = GetCharName();
	TCHAR szCharName[40] = {0,};
	int iNameLen = min(LEN_NAME,(int)sCharName.length());
	_tcsncpy(szCharName,sCharName.c_str(),iNameLen);
	packet.AddData(szCharName,LEN_NAME);

	TCHAR szID[40] = {0,};
	int nID = min(MAX_LEN_ID,_tcslen(g_pNetworkManager->GetLoginID()));
	_tcsncpy(szID,g_pNetworkManager->GetLoginID(),nID);
	packet.AddData(szID,MAX_LEN_ID);
	packet.AddUINT32(m_iIndunID);

	GetPacketHandler()->PostMsg(&packet);
	m_bExceptSocketFailed = false;
	return true;
}

// 게임 서버에서 logout할 경우 전송
void SPNetworkManager::LeaveWorld()
{
	CPacket packet;
	packet.AddUINT32(WORLD_CS_LEAVE);
	packet.AddUINT32(GetUserID());
	packet.AddUINT32(GetPassport());

	GetPacketHandler()->PostMsg(&packet);
}

void SPNetworkManager::GameChannel_IN()
{
	UINT32 uiReservedParam = 0;

	CPacket packet;
	packet.AddUINT32(CHANNEL_CS_IN);
	packet.AddUINT32(GetUserID());
	packet.AddUINT32(uiReservedParam);

	GetPacketHandler()->PostMsg(&packet);
}

void SPNetworkManager::GameChannel_OUT()
{
	UINT32 uiReservedParam = 0;

	CPacket packet;
	packet.AddUINT32(CHANNEL_CS_OUT);
	packet.AddUINT32(GetUserID());
	packet.AddUINT32(uiReservedParam);

	GetPacketHandler()->PostMsg(&packet);
}

void SPNetworkManager::UserInRequest()
{
	CPacket packet;
	packet.AddUINT32(USER_CS_IN);
	packet.AddUINT32(GetUserID());
	packet.AddUINT32(GetInPassport());


#if 1

	if(m_pSockWnd)
	{
		SOCKET sock;
		struct sockaddr_in* pPeer;
		struct sockaddr peerName;
		DECLARE_VAR(TCHAR,szIP,LEN_IP);

		int nameLen = sizeof(struct sockaddr);
		sock = m_pSockWnd->GetSocket();

		getpeername(sock,&peerName,&nameLen);
		pPeer = (struct sockaddr_in*)&peerName;
		_stprintf(szIP,"%d.%d.%d.%d",pPeer->sin_addr.S_un.S_un_b.s_b1
			,pPeer->sin_addr.S_un.S_un_b.s_b2,pPeer->sin_addr.S_un.S_un_b.s_b3
			,pPeer->sin_addr.S_un.S_un_b.s_b4);
		packet.AddData(szIP,LEN_IP+1);
	}
#endif

	GetPacketHandler()->PostMsg(&packet);
#if defined(_DEBUG)
	OutputDebugString("Network UserIn Req\n");
#endif
}

void SPNetworkManager::UserMoveRequest()
{	
	if( m_bSendUserMoveRequest )
		return;

	CPacket packet;
	packet.AddUINT32(USER_CS_MOVE);
	packet.AddUINT16(m_uiServerID);

	m_bSendUserMoveRequest = true;
	GetPacketHandler()->PostMsg(&packet);
}

// 월드 목록 요청
bool SPNetworkManager::RequestWorldStatus()
{
	CPacket packet;
	packet.AddUINT32(WORLD_CS_GETLIST);

	GetPacketHandler()->PostMsg(&packet);

	return true;
}

int SPNetworkManager::WndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(!isActive()) return 0;

	switch(message)
	{
	case WM_SOCKET_DISCONNECT:
		{
			OutputDebugString("WM_SOCKET_DISCONNECT\n");

#if defined(_DEBUG)
			char szBuf[128];
			sprintf(szBuf, "DisconnectCount : %d\n", m_iDisconnectCount);
			OutputDebugString(szBuf);
#endif
			if( m_iDisconnectCount > 0 )
			{
				m_iDisconnectCount--;
				return 0;
			}

			if( m_pSockWnd->IsConnected() )
			{
				if( g_pSubGameManager && !m_bExceptSocketFailed )
				{
					g_pSubGameManager->SPPerformMessage(SPGM_SUG_SOCKET_FAILED, 0, 0);
					m_bSendUserMoveRequest = false;
				}
			}
		}
		break;

	case WM_SOCKET_FAIL: // 비정상 적인 소켓에 쓰기나 RECV를 시도한 경우
		{
			//OutputDebugString("WM_SOCKET_FAIL\n");
			//g_pSubGameManager->SPPerformMessage(SPGM_SUG_SOCKET_FAILED, 0, 0);
			m_bSendUserMoveRequest = false;
		}
		break;
	}
	return 0;	
}


int SPNetworkManager::SPPerformMessage	(UINT msg, WPARAM wparam, LPARAM lParam )	
{
	return SPNMESSAGE_FUNC(msg, wparam, lParam);
}


DEFINE_SC_PACKET_PROC(ECHO_XX_PACKET)
{
	OutputDebugString("ECHO_XX_PACKET\n");
}


///////////////////////////////////////////////////////////////////////////////////
// Instant test function
void packetParsing(CPacket* packet)
{
	int iPacketID ;
	// 서버로부터 패킷 수신
	//switch (packet->GetCommand())

	iPacketID = packet->GetCommand();

	switch (iPacketID)
	{
		// Echo
		CASE_SC_PACKET_PROC(ECHO_XX_PACKET);

		CASE_SC_PACKET_PROC(PROTOCOL_SC_START_REQ);
		CASE_SC_PACKET_PROC(PROTOCOL_SC_START);
		CASE_SC_PACKET_PROC(PROTOCOL_SC_LOGINSERVERINFO);

		CASE_SC_PACKET_PROC(PROTOCOL_SC_CHECK_CLIENT_VALIDATION_REQ);

		CASE_SC_PACKET_PROC(PROTOCOL_SC_START_DYNCODE_1);
		CASE_SC_PACKET_PROC(PROTOCOL_SC_START_DYNCODE_2);

		CASE_SC_PACKET_PROC(SYSTEM_SC_ERROR);

		// authentication
		CASE_SC_PACKET_PROC(AUTH_SC_LOGIN);
		CASE_SC_PACKET_PROC(AUTH_SC_LOGIN_EX);
		CASE_SC_PACKET_PROC(AUTH_SC_EKEY);
		
		CASE_SC_PACKET_PROC(USER_SC_IN);
		CASE_SC_PACKET_PROC(USER_SC_MOVE);

		// World/Channel
		CASE_SC_PACKET_PROC(WORLD_SC_GETLIST);
		CASE_SC_PACKET_PROC(WORLD_SC_SELECT);
		CASE_SC_PACKET_PROC(WORLD_SC_CHANNELSTATUS);		// 로긴/로비창에서
		CASE_SC_PACKET_PROC(WORLD_SC_CHANNELSTATUS2);		// 채널 변경 과정중 채널 목록 요청의 결과
		CASE_SC_PACKET_PROC(CHANNEL_SC_SELECT);
		CASE_SC_PACKET_PROC(WORLD_SC_ENTER);
		CASE_SC_PACKET_PROC(WORLD_SC_LEAVE);

		CASE_SC_PACKET_PROC(STAT_SC_CHANGE);
		CASE_SC_PACKET_PROC(STAT_SC_CHANGE_EX);
		CASE_SC_PACKET_PROC(STAT_SC_LEVEL_UP);
		CASE_SC_PACKET_PROC(WORLDENV_SC_DATETIME);
		CASE_SC_PACKET_PROC(WORLDENV_SC_WEATHER);

		CASE_SC_PACKET_PROC(GAME_SC_CHANGE_CHANNEL);
		CASE_SC_PACKET_PROC(GAME_SC_START);
		
		CASE_SC_PACKET_PROC(GAME_SC_GETOPTION);
		CASE_SC_PACKET_PROC(GAME_SC_SETOPTION);

		// Character
		CASE_SC_PACKET_PROC(CHAR_SC_GETLIST);
		CASE_SC_PACKET_PROC(CHAR_SC_CHECK_CHARNAME);
		CASE_SC_PACKET_PROC(CHAR_SC_CREATE);
		CASE_SC_PACKET_PROC(CHAR_SC_DELETE);
		CASE_SC_PACKET_PROC(CHAR_SC_SELECT);
		CASE_SC_PACKET_PROC(CHAR_SC_CHARINFO);
		CASE_SC_PACKET_PROC(CHAR_SC_ACTION);
		CASE_SC_PACKET_PROC(CHAR_SC_STATUSINFO);
		CASE_SC_PACKET_PROC(CHAR_SC_QUICK_SLOT);
		CASE_SC_PACKET_PROC(CHAR_SC_MOVING_STATUS_INFO);
		CASE_SC_PACKET_PROC(CHAR_SC_FACING);
		CASE_SC_PACKET_PROC(CHAR_SC_EFFECT_STATUS_INFO);

		CASE_SC_PACKET_PROC(BATTLE_SC_ATTACKED);
		CASE_SC_PACKET_PROC(BATTLE_SC_HEALING);

		// Inventory Container Slot Extend 
		CASE_SC_PACKET_PROC(CHAR_SC_CONTAINER_INFOS);

		// Navigation
		CASE_SC_PACKET_PROC(NAV_SC_INSIGHT);
		//CASE_SC_PACKET_PROC(NAV_SC_INSIGHT_EX);
		CASE_SC_PACKET_PROC(NAV_SC_OUTOFSIGHT);
		CASE_SC_PACKET_PROC(ACTION_SC_COMMAND);			// MOVE/STOP/JUMP/ATTACK
		CASE_SC_PACKET_PROC(ACTION_SC_KNOCKBACK);
		CASE_SC_PACKET_PROC(NAV_SC_INSIGHT_GUID);

		CASE_SC_PACKET_PROC(EVENT_SC_RAISE);

		CASE_SC_PACKET_PROC(EVENTMISSION_SC_LIST);
		CASE_SC_PACKET_PROC(EVENTMISSION_SC_RAISE);
		CASE_SC_PACKET_PROC(CHAR_SC_CHANGE_CLASS);

		CASE_SC_PACKET_PROC(CHAT_SC_MSG);
		CASE_SC_PACKET_PROC(CHAT_SC_MSG_MONEY);
		CASE_SC_PACKET_PROC(CHAT_SC_MSG_PCINFO);
		CASE_SC_PACKET_PROC(MONEY_SC_CHAT);
		CASE_SC_PACKET_PROC(CHAR_SC_GM_STATUS);
		CASE_SC_PACKET_PROC(MSG_SC_NOTICE);				// 공지 메세지(서버 점검,...)

		// Item		
		CASE_SC_PACKET_PROC( ITEM_SC_CONTAINER_INFO		);
		CASE_SC_PACKET_PROC( ITEM_SC_CONTAINER_INFOS	);
		CASE_SC_PACKET_PROC( ITEM_SC_LOOTING			);
		CASE_SC_PACKET_PROC( ITEM_SC_CHANGE_SLOT		);
		CASE_SC_PACKET_PROC( ITEM_SC_TRASH				);
		CASE_SC_PACKET_PROC( ITEM_SC_ALARM				);
		CASE_SC_PACKET_PROC( ITEM_SC_TOGGLE_USE_WEAPON_SET );
		CASE_SC_PACKET_PROC( ITEM_SC_DIVIDE				);
		CASE_SC_PACKET_PROC( ITEM_SC_USING				);
		CASE_SC_PACKET_PROC( NPC_SHOP_SC_BUY			);
		CASE_SC_PACKET_PROC( ITEM_SC_ENCHANT			);
		CASE_SC_PACKET_PROC( ITEM_SC_UPGRADE			);
		CASE_SC_PACKET_PROC( ITEM_SC_DESTROY_BY_TIME	);
		CASE_SC_PACKET_PROC( ITEM_SC_EXPIRE				);
		CASE_SC_PACKET_PROC( ITEM_SC_ADDTIME			);
		CASE_SC_PACKET_PROC( ITEM_SC_ITEMEXP			);
		CASE_SC_PACKET_PROC( ITEM_SC_ADDSP				);
		CASE_SC_PACKET_PROC( ITEM_SC_CHAT				);	//[2007/3/28]
		CASE_SC_PACKET_PROC( ITEM_SC_WHERE				);	//[ 2008.9.3 ]
		CASE_SC_PACKET_PROC( ITEM_SC_TRACK				);	//[ 2008.10.10 ]
		CASE_SC_PACKET_PROC( ITEM_SC_WHERE_COMMAMD_INFOS); // [2008.9.4]
		CASE_SC_PACKET_PROC( ITEM_SC_MIX				);	// 아이템 조합 [9/12/2007 AJJIYA]
		CASE_SC_PACKET_PROC( CHAR_SC_ITEMMIX_INFO		);	// 아이템조합 정보 [9/12/2007 AJJIYA]
		CASE_SC_PACKET_PROC( ITEM_SC_EFFECTUPGRADE		);

		// Another PC Equip
		CASE_SC_PACKET_PROC( ITEM_SC_FIGURE_INFOS_FOR_SINGLE );


		// Storage
		CASE_SC_PACKET_PROC( STORAGE_SC_INMONEY			);
		CASE_SC_PACKET_PROC( STORAGE_SC_OUTMONEY		);

		// Gift
		CASE_SC_PACKET_PROC( GIFT_SC_LIST				);
		CASE_SC_PACKET_PROC( GIFT_SC_GET				);

		// Trade
		CASE_SC_PACKET_PROC( TRADE_SC_REQUEST			);
		CASE_SC_PACKET_PROC( TRADE_SC_RESPONSE			);
		CASE_SC_PACKET_PROC( TRADE_SC_PUSHMONEY			);
		CASE_SC_PACKET_PROC( TRADE_SC_PUSHITEM			);
		CASE_SC_PACKET_PROC( TRADE_SC_POPITEM			);
		CASE_SC_PACKET_PROC( TRADE_SC_WAIT				);
		CASE_SC_PACKET_PROC( TRADE_SC_ACCEPT			);
		CASE_SC_PACKET_PROC( TRADE_SC_CANCEL			);

		// Quest
		CASE_SC_PACKET_PROC( MISSION_SC_LIST			);
		CASE_SC_PACKET_PROC( MISSION_SC_UPDATE			);
		CASE_SC_PACKET_PROC( MISSION_SC_ACTION			);
		CASE_SC_PACKET_PROC( QUEST_SC_LIST				);
		CASE_SC_PACKET_PROC( QUEST_SC_ACCEPT			);
		CASE_SC_PACKET_PROC( QUEST_SC_CANCEL			);
		CASE_SC_PACKET_PROC( QUEST_SC_FINISH			);
		CASE_SC_PACKET_PROC( QUEST_SC_SHARE				);
		CASE_SC_PACKET_PROC( QUEST_SC_SHARE_RECV		);

		// Team
		CASE_SC_PACKET_PROC( TEAM_SC_REQUEST			);
		CASE_SC_PACKET_PROC( TEAM_SC_JOIN				);
		CASE_SC_PACKET_PROC( TEAM_SC_SECEDE				);
		CASE_SC_PACKET_PROC( TEAM_SC_EXPEL				);
		CASE_SC_PACKET_PROC( TEAM_SC_CHANGECAPTAIN		);
		CASE_SC_PACKET_PROC( TEAM_SC_CHANGELOOTINGRULE	);
		CASE_SC_PACKET_PROC( TEAM_SC_UPDATEMEMBER		);
		CASE_SC_PACKET_PROC( TEAM_SC_INFO				);

		// Npc
		CASE_SC_PACKET_PROC( CUTIN_SC_OPEN				);
		CASE_SC_PACKET_PROC( CUTIN_SC_CLOSE				);
		CASE_SC_PACKET_PROC( NPC_SC_PORTAL				);
		CASE_SC_PACKET_PROC( NPC_SC_REWARD				);

		// LOOTING
		CASE_SC_PACKET_PROC( ITEM_SC_LOOTING_LIST		);

		// SKILL
		CASE_SC_PACKET_PROC( SKILL_SC_INFOS		);
		CASE_SC_PACKET_PROC( SKILL_SC_INIT		);
		CASE_SC_PACKET_PROC( SKILL_SC_USING		);
		CASE_SC_PACKET_PROC( ITEMSKILL_SC_USING	);
		CASE_SC_PACKET_PROC( SKILL_SC_LVUP		);
		CASE_SC_PACKET_PROC( SKILL_SC_LEARN		);
		CASE_SC_PACKET_PROC( SKILL_SC_INIT_ITEMMIX		);
		CASE_SC_PACKET_PROC( SKILL_SC_INIT_SELECTIVE	);
		CASE_SC_PACKET_PROC( EFFECT_SC_INFOS	);
		CASE_SC_PACKET_PROC( EFFECT_SC_DEL	);
		// Broadcast
		CASE_SC_PACKET_PROC( ITEM_SC_FIGURE_INFOS		);
		CASE_SC_PACKET_PROC( ITEM_SC_FIGURE_ON			);
		CASE_SC_PACKET_PROC( ITEM_SC_FIGURE_OFF			);

		// Quick Slot AJJIYA [7/14/2005]
		//CASE_SC_PACKET_PROC( CHAR_SC_QUICK_SLOT	);

		// Friend/Block list
		CASE_SC_PACKET_PROC( FRIEND_SC_GETLIST);
		CASE_SC_PACKET_PROC( FRIEND_SC_ADD);
		CASE_SC_PACKET_PROC( FRIEND_SC_ADD_RECV);
		CASE_SC_PACKET_PROC( FRIEND_SC_ACCEPT);
		CASE_SC_PACKET_PROC( FRIEND_SC_DELETE);
		CASE_SC_PACKET_PROC( FRIEND_SC_REFRESH_STATUS);
		CASE_SC_PACKET_PROC( BLOCK_SC_GETLIST);
		CASE_SC_PACKET_PROC( BLOCK_SC_ADD);
		CASE_SC_PACKET_PROC( BLOCK_SC_DELETE);

		CASE_SC_PACKET_PROC( BLOCK_SC_ISBLOCKED);

		// Title
		CASE_SC_PACKET_PROC( TITLE_SC_LIST			);
		CASE_SC_PACKET_PROC( TITLE_SC_INSERT		);
		CASE_SC_PACKET_PROC( TITLE_SC_CHANGE		);

		// CashShop
		CASE_SC_PACKET_PROC( CASHSHOP_SC_IN );
		CASE_SC_PACKET_PROC( CASHSHOP_SC_OUT );
		CASE_SC_PACKET_PROC( CASHSHOP_SC_INFO );
		CASE_SC_PACKET_PROC( CASHSHOP_SC_CASH );
		CASE_SC_PACKET_PROC( CASHSHOP_SC_BUY );
		CASE_SC_PACKET_PROC( CASHSHOP_SC_BUY_PRESENT );
		CASE_SC_PACKET_PROC( CASHSHOP_SC_SELL );
		CASE_SC_PACKET_PROC( CASHSHOP_SC_COUPON );
		CASE_SC_PACKET_PROC( CASHSHOP_SC_WISH_LIST);
		CASE_SC_PACKET_PROC( CASHSHOP_SC_WISH_ADD);
		CASE_SC_PACKET_PROC( CASHSHOP_SC_WISH_DEL);
		CASE_SC_PACKET_PROC( CASHSHOP_SC_MINOR_INIT );
		CASE_SC_PACKET_PROC( CASHSHOP_SC_MINOR_INFO );
		CASE_SC_PACKET_PROC( CASHSHOP_SC_OUT_READY );
		CASE_SC_PACKET_PROC( CASHSHOP_SC_VERSION );
		CASE_SC_PACKET_PROC( CASHSHOP_SC_PRICE );
		CASE_SC_PACKET_PROC( CASHSHOP_SC_HOT_INFO );
		CASE_SC_PACKET_PROC( CASHSHOP_SC_HOT_STATUS );
		CASE_SC_PACKET_PROC( CASHSHOP_SC_GET_QUICK_BUY_INFO );


		// Mail Box
		CASE_SC_PACKET_PROC( MAILBOX_SC_GETLIST	);	
		CASE_SC_PACKET_PROC( MAILBOX_SC_SEND		);	
		CASE_SC_PACKET_PROC( MAILBOX_SC_DELETE	);	
		CASE_SC_PACKET_PROC( MAILBOX_SC_GETBODY	);	
		CASE_SC_PACKET_PROC( MAILBOX_SC_MOVEATTACHMENT);
		CASE_SC_PACKET_PROC( MAILBOX_SC_NEWMAIL);

		// FreeMarket
		CASE_SC_PACKET_PROC( FREEMARKET_SC_OPEN			);
		CASE_SC_PACKET_PROC( FREEMARKET_SC_CLOSE		);
		CASE_SC_PACKET_PROC( FREEMARKET_SC_IN			);
		CASE_SC_PACKET_PROC( FREEMARKET_SC_OUT			);
		CASE_SC_PACKET_PROC( FREEMARKET_SC_BUY			);

		// WayPoint
		CASE_SC_PACKET_PROC( WAYPOINT_SC_LIST			);
		CASE_SC_PACKET_PROC( WAYPOINT_SC_FIND			);
		CASE_SC_PACKET_PROC( WAYPOINT_SC_MOVE			);

		// BeautyShop
		CASE_SC_PACKET_PROC( ITEM_SC_BASIC_FIGURE_CHANGE	);

		// Guild
		CASE_SC_PACKET_PROC( GUILD_SC_PCGUILD			);
		CASE_SC_PACKET_PROC( GUILD_SC_GUILD_COMMON		);
		CASE_SC_PACKET_PROC( GUILD_SC_MEMBERLIST		);
		CASE_SC_PACKET_PROC( GUILD_SC_CREATE			);
		CASE_SC_PACKET_PROC( GUILD_SC_DESTROY			);
		CASE_SC_PACKET_PROC( GUILD_SC_UPDATE_SLOGAN		);
		CASE_SC_PACKET_PROC( GUILD_SC_REQUEST			);
		CASE_SC_PACKET_PROC( GUILD_SC_JOIN				);
		CASE_SC_PACKET_PROC( GUILD_SC_SECEDE			);
		CASE_SC_PACKET_PROC( GUILD_SC_EXPEL				);
		CASE_SC_PACKET_PROC( GUILD_SC_EXPEL_ME			);
		CASE_SC_PACKET_PROC( GUILD_SC_CHANGEGRADE		);
		CASE_SC_PACKET_PROC( GUILD_SC_CHANGEGRADE_ME	);
		CASE_SC_PACKET_PROC( GUILD_SC_MSG_MEMBER		);
		CASE_SC_PACKET_PROC( GUILD_SC_MSG_GUILD			);
		CASE_SC_PACKET_PROC( GUILD_SC_CHANGEMASTER		);
		CASE_SC_PACKET_PROC( GUILD_SC_CHANGEMASTER_ME	);
		CASE_SC_PACKET_PROC( GUILD_SC_PLANT				);
		CASE_SC_PACKET_PROC( GUILD_SC_GRUB				);
		CASE_SC_PACKET_PROC( GUILD_SC_NOURISH			);
		CASE_SC_PACKET_PROC( GUILD_SC_GROWTHCROP		);
		CASE_SC_PACKET_PROC( GUILD_SC_RANKUP			);
		CASE_SC_PACKET_PROC( GUILD_SC_CHANGEMARK		);

		CASE_SC_PACKET_PROC( NOTI_SC_ONLINE_STATUS_FRIEND );
		CASE_SC_PACKET_PROC( SUBLOGIC_SC_COMMAND );

		// Indun
		CASE_SC_PACKET_PROC( INDUN_SC_IN				);
		CASE_SC_PACKET_PROC( INDUN_SC_OUT				);
		CASE_SC_PACKET_PROC( INDUN_SC_INIT				);

		// Lottery
		CASE_SC_PACKET_PROC( LOTTERY_SC_START			);
		CASE_SC_PACKET_PROC( LOTTERY_SC_UPDATE			);
		CASE_SC_PACKET_PROC( LOTTERY_SC_CHOICE			);

		// Gamble
		CASE_SC_PACKET_PROC( GAMBLE_SC_REQ				);

		CASE_SC_PACKET_PROC( FDS_SC_NOTICE );

		// PVP
		CASE_SC_PACKET_PROC( PVP_SC_REGIST				);
		CASE_SC_PACKET_PROC( PVP_SC_CANCEL				);
		CASE_SC_PACKET_PROC( PVP_SC_IN					);
		CASE_SC_PACKET_PROC( PVP_SC_LOADSTATUS			);
		CASE_SC_PACKET_PROC( PVP_SC_START				);
		CASE_SC_PACKET_PROC( PVP_SC_OUT					);
		CASE_SC_PACKET_PROC( PVP_SC_RANK				);
		CASE_SC_PACKET_PROC( PVP_SC_FINISH				);

		// 서버시간/정보
		CASE_SC_PACKET_PROC( SERVERTIME_SC_INFO			);

		// ChattingRoom
		CASE_SC_PACKET_PROC( CHATROOM_SC_CREATE			);
		CASE_SC_PACKET_PROC( CHATROOM_SC_ROOMINFO_CHECK	);
		CASE_SC_PACKET_PROC( CHATROOM_SC_JOIN			);
		CASE_SC_PACKET_PROC( CHATROOM_SC_MODIFY			);
		CASE_SC_PACKET_PROC( CHATROOM_SC_OUT			);
		CASE_SC_PACKET_PROC( CHATROOM_SC_FORCEOUT		);
		CASE_SC_PACKET_PROC( CHATROOM_SC_CHANGEMASTER	);
		CASE_SC_PACKET_PROC( CHATROOM_SC_BROADCAST_UNITED_MESSAGE	);
		CASE_SC_PACKET_PROC( CHATROOM_SC_MEMBERLIST		);
		CASE_SC_PACKET_PROC( CHATROOM_SC_ADVERT			);
		CASE_SC_PACKET_PROC( CHATROOM_SC_ROOMLIST		);
		CASE_SC_PACKET_PROC( CHATROOM_SC_USERLIST		);
		CASE_SC_PACKET_PROC( CHATROOM_SC_INVITE_REQ		);
		CASE_SC_PACKET_PROC( CHATROOM_SC_INVITE_ANS		);
		CASE_SC_PACKET_PROC( CHATROOM_SC_OTHERPC_INFO	);
		CASE_SC_PACKET_PROC( CHATROOM_SC_GET_MYSTYLE	);

		// AUCTION
		CASE_SC_PACKET_PROC( AUCTION_SC_OPEN			);
		CASE_SC_PACKET_PROC( AUCTION_SC_CREATE			);
		CASE_SC_PACKET_PROC( AUCTION_SC_CANCEL			);
		CASE_SC_PACKET_PROC( AUCTION_SC_FINISH			);
		CASE_SC_PACKET_PROC( AUCTION_SC_FINISH_SELL		);
		CASE_SC_PACKET_PROC( AUCTION_SC_BID				);
		CASE_SC_PACKET_PROC( AUCTION_SC_BUYOUT			);
		CASE_SC_PACKET_PROC( AUCTION_SC_SEARCH_INFO		);
		CASE_SC_PACKET_PROC( AUCTION_SC_INFO_4BIDDER	);
		CASE_SC_PACKET_PROC( AUCTION_SC_BIDLIST			);
		CASE_SC_PACKET_PROC( AUCTION_SC_BIDDEL			);		
		CASE_SC_PACKET_PROC( AUCTION_SC_INFO_4SELLER	);
		CASE_SC_PACKET_PROC( AUCTION_SC_SELLLIST		);		

		// 보안툴 검증
		CASE_SC_PACKET_PROC( SECURITY_SC_CHECK			);
		CASE_SC_PACKET_PROC( SECURITY_SC_INFO			);

		//	CoolTime
		CASE_SC_PACKET_PROC( COOLTIME_SC_INFOS			);

		CASE_SC_PACKET_PROC( CASHSHOP_SC_ASK_BUY		);
		CASE_SC_PACKET_PROC( CASHSHOP_SC_ASK_BUY_RECV	);
		CASE_SC_PACKET_PROC( CASHSHOP_SC_EFFECT			);

		// greener guide
		CASE_SC_PACKET_PROC( GREENERGUIDE_SC_EVENT );
		CASE_SC_PACKET_PROC( GREENERGUIDE_SC_FETECHAWARD );

		CASE_SC_PACKET_PROC( GAMBLE_SC_GET_STATUS		);
		CASE_SC_PACKET_PROC( GAMBLE_SC_UPDATE			);
		CASE_SC_PACKET_PROC( GAMBLE_SC_BUY				);
		CASE_SC_PACKET_PROC( GAMBLE_SC_UN_BUY			);
		CASE_SC_PACKET_PROC( GAMBLE_SC_ACCEPT_ITEM		);
		CASE_SC_PACKET_PROC( GAMBLE_SC_ACCEPT_SECOND_ITEM	);
		CASE_SC_PACKET_PROC( GAMBLE_SC_ENTER_GAMBLE		);
		CASE_SC_PACKET_PROC( GAMBLE_SC_LEAVER_GAMBLE	);
		CASE_SC_PACKET_PROC( GAMBLE_SC_INITIAL			);
		CASE_SC_PACKET_PROC( GAMBLE_SC_BROADCAST_GETTED_ITEM	);
		CASE_SC_PACKET_PROC( GAMBLE_SC_GET_NAME	);

		CASE_SC_PACKET_PROC( PET_SC_GET_INFO );
		CASE_SC_PACKET_PROC( PET_SC_OPEN_PET_SHOP );
		CASE_SC_PACKET_PROC( PET_SC_GET_PET_SHOP_INFO );
		CASE_SC_PACKET_PROC( PET_SC_ENTER_PET_SHOP );
		CASE_SC_PACKET_PROC( PET_SC_LEAVE_PET_SHOP );
		CASE_SC_PACKET_PROC( PET_SC_UPDATE_PET_SHOP_ITEM );
		CASE_SC_PACKET_PROC( PET_SC_BUY_ITEM );
		CASE_SC_PACKET_PROC( PET_SC_CLOSE_PET_SHOP );
		CASE_SC_PACKET_PROC( PET_SC_LEARN_SKILL );
		CASE_SC_PACKET_PROC( PET_SC_DISCARD_SKILL );
		CASE_SC_PACKET_PROC( PET_SC_PET_ATTR_CHANGE );
		CASE_SC_PACKET_PROC( PET_SC_PET_TAKE_OFF );
		CASE_SC_PACKET_PROC( PET_SC_ADD_SSP );
		CASE_SC_PACKET_PROC( PET_SC_CHANGE_NAME );
		CASE_SC_PACKET_PROC( PET_SC_SHOUT );
		CASE_SC_PACKET_PROC( PET_SC_PET_SHOP_CLOSED );

		//	Love [11/3/2008 AJJIYA]
		CASE_SC_PACKET_PROC( WEDDING_SC_INFO			);
		CASE_SC_PACKET_PROC( WEDDING_SC_REQUEST			);
		CASE_SC_PACKET_PROC( WEDDING_SC_REQUEST_RECV	);
		CASE_SC_PACKET_PROC( WEDDING_SC_RESPONSE		);
		CASE_SC_PACKET_PROC( WEDDING_SC_WEDDING_CANCEL	);
		CASE_SC_PACKET_PROC( WEDDING_SC_MOTION_STARTEND	);
		CASE_SC_PACKET_PROC( WEDDING_SC_UPDATEMEMBER	);
		CASE_SC_PACKET_PROC( WEDDING_SC_PARTING			);
		CASE_SC_PACKET_PROC( WEDDING_SC_SKILL_USING		);
		CASE_SC_PACKET_PROC( WEDDING_SC_SUMMON_REQUEST	);
		CASE_SC_PACKET_PROC( WEDDING_SC_SUMMON_REQUEST_ERR);
		CASE_SC_PACKET_PROC( WEDDING_SC_SUMMON_RESPONSE	);
		CASE_SC_PACKET_PROC( WEDDING_SC_SUMMON_RESPONSE_RECV	);

		default:
		{
			//g_pGOBManager->SPPerformMessage(packet->GetCommand(), 0, (LPARAM)packet);
		}
		break;
	}

}

SPNMESSAGE_MAP_BEGIN( SPNetworkManager )							

	//  서버로 패킷 전송 (__CS__)
	//SPNMESSAGE_COMMAND_EX (AUTH_CS_LOGIN)

	SPNMESSAGE_COMMAND_EX (USER_CS_IN)
	SPNMESSAGE_COMMAND_EX (USER_CS_MOVE)

	//SPNMESSAGE_COMMAND_EX (WORLD_CS_SELECT)		
	SPNMESSAGE_COMMAND_EX (CHANNEL_CS_SELECT)		
	SPNMESSAGE_COMMAND_EX (WORLD_CS_ENTER)		
	SPNMESSAGE_COMMAND_EX (WORLD_CS_LEAVE)		

	SPNMESSAGE_COMMAND_EX (GAME_CS_START)		

	SPNMESSAGE_COMMAND_EX (CHAR_CS_GETLIST)		
	SPNMESSAGE_COMMAND_EX (CHAR_CS_CREATE)		
	SPNMESSAGE_COMMAND_EX (CHAR_CS_DELETE)		
	SPNMESSAGE_COMMAND_EX (CHAR_CS_SELECT)		
	SPNMESSAGE_COMMAND_EX (CHAR_CS_CHARINFO)		
	SPNMESSAGE_COMMAND_EX (CHAR_CS_ACTION)		

	SPNMESSAGE_COMMAND_EX (ACTION_CS_COMMAND)		// MOVE/STOP/JUMP/ATTACK
	SPNMESSAGE_COMMAND_EX (SKILL_CS_USING)
	SPNMESSAGE_COMMAND_EX (ITEMSKILL_CS_SELL)		//[2006/12/1]

	SPNMESSAGE_COMMAND_EX (SKILL_CS_LVUP)			// AJJIYA [7/14/2005]
	
	SPNMESSAGE_COMMAND_EX (CHAR_CS_REVIVAL)			//[2006/1/19] jinhee
	SPNMESSAGE_COMMAND_EX (CHAR_CS_PVP_REVIVAL);	//[2007/11/28]-jinhee

	SPNMESSAGE_COMMAND_EX(CHAT_CS_MSG);
	SPNMESSAGE_COMMAND_EX(MONEY_CS_CHAT);

	SPNMESSAGE_COMMAND_EX(EVENT_CS_RAISE);
	SPNMESSAGE_COMMAND_EX(NAV_CS_ENTERSTAGE);

	SPNMESSAGE_COMMAND_EX(EVENTMISSION_CS_RAISE);	//2007/10/4]-jinhee
	SPNMESSAGE_COMMAND_EX(CHAR_CS_CHANGE_CLASS);	//[2007/11/13]-jinhee

	// system message

	SPNMESSAGE_COMMAND_EX( SERVERTIME_CS_INFO	);

	// Item
	SPNMESSAGE_COMMAND_EX( ITEM_CS_LOOTING		);	
	SPNMESSAGE_COMMAND_EX( ITEM_CS_CHANGE_SLOT	);
	SPNMESSAGE_COMMAND_EX( ITEM_CS_TRASH		);	
	SPNMESSAGE_COMMAND_EX( ITEM_CS_USING		);	//[2005/5/23]
	SPNMESSAGE_COMMAND_EX( ITEM_CS_DIVIDE		);	//[2005/9/7]
	SPNMESSAGE_COMMAND_EX( ITEM_CS_CHAT			);	//[2007/3/28]

	// Search
	SPNMESSAGE_COMMAND_EX( ITEM_CS_WHERE );			//[2008/9/3]
	// Follow and Snatch
	SPNMESSAGE_COMMAND_EX( ITEM_CS_TRACK );			//[2008/10/10]
	// Item Time ADD
	SPNMESSAGE_COMMAND_EX(ITEM_CS_ADDTIME		);	//[2006/11/16]

	// Item Add Sp
	SPNMESSAGE_COMMAND_EX(ITEM_CS_ADDSP			);	//[2006/11/23]
	
	// Storage
	SPNMESSAGE_COMMAND_EX( STORAGE_CS_INMONEY	);	//[2005/7/21]
	SPNMESSAGE_COMMAND_EX( STORAGE_CS_OUTMONEY	);	//[2005/7/21]

	// Gift
	SPNMESSAGE_COMMAND_EX( GIFT_CS_LIST			);	//[2006/3/22]
	SPNMESSAGE_COMMAND_EX( GIFT_CS_GET			);	//[2006/3/22]

	// Mail Box
	SPNMESSAGE_COMMAND_EX( MAILBOX_CS_GETLIST	);	//[2006/4/13]
	SPNMESSAGE_COMMAND_EX( MAILBOX_CS_SEND		);	//[2006/4/13]
	SPNMESSAGE_COMMAND_EX( MAILBOX_CS_DELETE	);	//[2006/4/13]
	SPNMESSAGE_COMMAND_EX( MAILBOX_CS_GETBODY	);	//[2006/4/13]
	SPNMESSAGE_COMMAND_EX( MAILBOX_CS_MOVEATTACHMENT);	//[2006/4/13]
    

	// PC TRADE
	SPNMESSAGE_COMMAND_EX( TRADE_CS_REQUEST		);	//[2005/7/27]
	SPNMESSAGE_COMMAND_EX( TRADE_CS_RESPONSE	);	//[2005/7/27]
	SPNMESSAGE_COMMAND_EX( TRADE_CS_PUSHMONEY	);	//[2005/7/27]
	SPNMESSAGE_COMMAND_EX( TRADE_CS_PUSHITEM	);	//[2005/7/27]
	SPNMESSAGE_COMMAND_EX( TRADE_CS_POPITEM		);	//[2005/7/27]
	SPNMESSAGE_COMMAND_EX( TRADE_CS_WAIT		);	//[2007/3/22]
	SPNMESSAGE_COMMAND_EX( TRADE_CS_ACCEPT		);	//[2005/7/27]
	SPNMESSAGE_COMMAND_EX( TRADE_CS_CANCEL		);	//[2005/7/27]


	// Free Market
	SPNMESSAGE_COMMAND_EX(FREEMARKET_CS_OPEN	);	//[2006/5/16]
	SPNMESSAGE_COMMAND_EX(FREEMARKET_CS_CLOSE	);	//[2006/5/16]
	SPNMESSAGE_COMMAND_EX(FREEMARKET_CS_IN		);	//[2006/5/16]
	SPNMESSAGE_COMMAND_EX(FREEMARKET_CS_OUT		);	//[2006/5/16]
	SPNMESSAGE_COMMAND_EX(FREEMARKET_CS_BUY		);	//[2006/5/16]

	// Way Point
	SPNMESSAGE_COMMAND_EX(WAYPOINT_CS_FIND		);	//[2006/9/4]
	SPNMESSAGE_COMMAND_EX(WAYPOINT_CS_MOVE		);	//[2006/9/4]


	// Another PC Info Request
	SPNMESSAGE_COMMAND_EX( BLOCK_CS_ISBLOCKED		);	//[2005/9/2]

	// Another PC Equip Item
	SPNMESSAGE_COMMAND_EX( ITEM_CS_FIGURE_INFOS_FOR_SINGLE );
	
	// NPC TRADE
	SPNMESSAGE_COMMAND_EX( NPC_SHOP_CS_SELL		);
	SPNMESSAGE_COMMAND_EX( NPC_SHOP_CS_BUY		);	

	SPNMESSAGE_COMMAND_EX( ITEM_CS_TOGGLE_USE_WEAPON_SET );	

	SPNMESSAGE_COMMAND_EX ( CHAR_CS_SET_QUICK_SLOT )			// AJJIYA [7/19/2005]

	// QUEST
	SPNMESSAGE_COMMAND_EX( MISSION_CS_ACTION);	//[2005/9/27]
	SPNMESSAGE_COMMAND_EX( QUEST_CS_ACCEPT	);	//[2005/9/27]
	SPNMESSAGE_COMMAND_EX( QUEST_CS_CANCEL	);	//[2005/9/27]
	SPNMESSAGE_COMMAND_EX( QUEST_CS_FINISH	);	//[2005/9/27]

	// TEAM
	SPNMESSAGE_COMMAND_EX( TEAM_CS_REQUEST				);	// 2005.11.14 dhpark
	SPNMESSAGE_COMMAND_EX( TEAM_CS_JOIN					);	// 2005.11.14 dhpark
	SPNMESSAGE_COMMAND_EX( TEAM_CS_SECEDE				);	// 2005.11.14 dhpark
	SPNMESSAGE_COMMAND_EX( TEAM_CS_EXPEL				);	// 2005.11.14 dhpark
	SPNMESSAGE_COMMAND_EX( TEAM_CS_CHANGECAPTAIN		);	// 2005.11.14 dhpark
	SPNMESSAGE_COMMAND_EX( TEAM_CS_CHANGELOOTINGRULE	);	// 2005.11.14 dhpark
		
	// Npc
	SPNMESSAGE_COMMAND_EX( CUTIN_CS_OPEN		);	// 2005.11.01 dhpark 상태제어를 위해서 추가합니다.
	SPNMESSAGE_COMMAND_EX( CUTIN_CS_CLOSE		);	// 2005.11.01 dhpark 상태제어를 위해서 추가합니다.

	// Npc Portal 
	SPNMESSAGE_COMMAND_EX( NPC_CS_PORTAL		);

	// Npc Event
	SPNMESSAGE_COMMAND_EX( NPC_CS_REWARD		);

	// Title
	SPNMESSAGE_COMMAND_EX( TITLE_CS_CHANGE	);	// 2006.04.04 dhpark

	// Field Cash Trade 
	SPNMESSAGE_COMMAND_EX( CASHSHOP_CS_VERSION	);	//[2008/4/14] - jinhee
	SPNMESSAGE_COMMAND_EX( CASHSHOP_CS_PRICE	);
	SPNMESSAGE_COMMAND_EX( CASHSHOP_CS_BUY		);
	SPNMESSAGE_COMMAND_EX( CASHSHOP_CS_BUY_PRESENT	);
	SPNMESSAGE_COMMAND_EX( CASHSHOP_CS_WISH_ADD	);

	// quick buy
	SPNMESSAGE_COMMAND_EX( CASHSHOP_CS_GET_QUICK_BUY_INFO );
	SPNMESSAGE_COMMAND_EX( CASHSHOP_CS_QUICK_BUY_BUY_ITEM );

	// effect item upgrade
	SPNMESSAGE_COMMAND_EX( ITEM_CS_EFFECTUPGRADE );
	SPNMESSAGE_COMMAND_EX( NAV_CS_INSIGHT_GUID);

	// greener guide
	SPNMESSAGE_COMMAND_EX( GREENERGUIDE_CS_FETCHAWARD );

	// pet shop
	SPNMESSAGE_COMMAND_EX( PET_CS_OPEN_PET_SHOP );
	SPNMESSAGE_COMMAND_EX( PET_CS_GET_PET_SHOP_INFO );
	SPNMESSAGE_COMMAND_EX( PET_CS_ENTER_PET_SHOP );
	SPNMESSAGE_COMMAND_EX( PET_CS_LEAVE_PET_SHOP );
	SPNMESSAGE_COMMAND_EX( PET_CS_BUY_ITEM );
	SPNMESSAGE_COMMAND_EX( PET_CS_CLOSE_PET_SHOP );
	SPNMESSAGE_COMMAND_EX( PET_CS_LEARN_SKILL );
	SPNMESSAGE_COMMAND_EX( PET_CS_DISCARD_SKILL );
	SPNMESSAGE_COMMAND_EX( PET_CS_ADD_SSP );
	SPNMESSAGE_COMMAND_EX( PET_CS_CHANGE_NAME );
	SPNMESSAGE_COMMAND_EX( PET_CS_SHOUT );


//	SPNMESSAGE_COMMAND_RANGE_EX(CHAR_CS_FUNC,CHAR_CS_GETLIST,CHAR_CS_ACTION);

SPNMESSAGE_MAP_END_BASE() 



