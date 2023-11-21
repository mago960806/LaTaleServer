// SPNetworkManager_SubLogic.cpp

#include "SPCommon.h"

#include <WinSock2.h>
#include "LT_Error.h"
#include "./NetworkCore/SubLogic/CPacketComposer.h"

#include "SPNetworkManager.h"


//#define USING_SUBLOGIC 

extern FLOAT  g_fFPS;

#if defined(USING_SUBLOGIC)
	#include "SubLogic/Connector4SubLogic.h"

	string g_sSubLogic_IP = "hoshim";
	USHORT g_usSublogic_Port = 10000;
	bool g_bInit = false;

	CConnector4SubLogic& GetConnector()
	{
		static CConnector4SubLogic s_Conector;
		return s_Conector;
	}
#endif

int	SPNetworkManager::SubLogic_Init()
{
#ifdef USING_SUBLOGIC
	g_bInit = GetConnector().Init();
	if(g_bInit)
	{
		GetConnector().SetServerInfo((TCHAR*)g_sSubLogic_IP.c_str(),g_usSublogic_Port);
		GetConnector().SetEncryptMode(true);
	}
#endif
	return 0;
}

void SPNetworkManager::SubLogic_Exit()
{
#if defined(USING_SUBLOGIC)
	if(g_bInit)
	{
		GetConnector().Exit();
	}
#endif
}

int SPNetworkManager::SubLogic_Loop(float fElapsedTime)
{
#if defined(USING_SUBLOGIC)
	if(g_bInit)
	{
		//static bool bConnectTried = false; 
		//if(!bConnectTried)
		//{
		//	GetConnector().Connect((TCHAR*)g_sSubLogic_IP.c_str(),g_usSublogic_Port);
		//	bConnectTried = true;
		//	return 0;
		//}

		////{
		//	static float fPeriodTime = /*KEEP_ALIVE_TIMEOUT*/3000.f/1000.f; 
		//	static float fCurTime = 0.f;

		//	if(fPeriodTime <= fElapsedTime)
		//	{
		//		CPacketComposer packet(KEEP_ALIVE_PACKET);
		//		packet << g_fFPS;

		//		GetConnector().SendBufferEx(packet.GetBuffer(),packet.GetLength());

		//		fCurTime = 0.f;
		//	}
		//	else
		//		fCurTime += fElapsedTime;
		////}

		GetConnector().CheckEvent(0);

		if(GetConnector().IsConnected())
		{
			GetConnector().SendFromQueue();
		}

	}
#endif
	return 0;
}


//////////////////////

static bool g_bConnectTried = false; 

void Connect(char* pszArg)
{
#if defined(USING_SUBLOGIC)

	TCHAR szIP[100];
	TCHAR szPort[50];

	TCHAR* pstr = (TCHAR*)pszArg;

	pstr = strtok(pstr," ");
	if(!pstr)
	{
		return;
	}
	string sIP(pstr);

	pstr = strtok(NULL," ");
	if(!pstr)
	{
		return;
	}
	USHORT usPort = atoi(pstr);

	if(!g_bConnectTried)
	{
		GetConnector().Connect((TCHAR*)sIP.c_str(),usPort);
		g_bConnectTried = true;
		return;
	}
#endif
}

void Disconnect()
{
#if defined(USING_SUBLOGIC)
	GetConnector().Close();
	g_bConnectTried = false;
#endif
}

void SendMsg(char* pszArg)
{
#if defined(USING_SUBLOGIC)

	UINT8 displayTime = 60;
	UINT8 optionFlags = 0;
	UINT16 uiLen = _tcslen(pszArg);

	if(uiLen < 1)
		return;
	else
		uiLen++;		// include NULL

	CPacketComposer packet(MSG_CS_NOTICE);
	packet << displayTime << optionFlags;
	packet << uiLen;
	packet.PushBack(pszArg,uiLen);

	GetConnector().SendBufferEx(packet.GetBuffer(),packet.GetLength());
#endif
}