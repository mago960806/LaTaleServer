// CSockWnd.h: interface for the CSockWnd class.
//
//////////////////////////////////////////////////////////////////////
#ifndef WINSOCKAAA_H
#define WINSOCKAAA_H

#pragma once
#include <Windows.h>
#include <stdio.h>
#include "BaseTypes.h"
#include "Packet.h"
#include "Queue.h"

typedef CPacket _MGPACKET;

#define WM_SOCKET_MESSAGE		(WM_USER + 101)
#define WM_SOCKET_PACKET		(WM_USER + 102)
#define WM_SOCKET_CONNECT		(WM_USER + 103)
#define WM_SOCKET_DISCONNECT	(WM_USER + 104) // Shutdown / remote 의비정상 종료에 의하여 닫힘이벤트가 올라오는 경우
#define WM_SOCKET_FAIL			(WM_USER + 105) // 비정상적인 소켓에 recv/write 를 시도하는 경우

#define HEADER_MODE	0
#define BODY_MODE	1
#define MAX_STREAM_SIZE 8192

///////////////////////////////////////////////////////////////////////////////

class CSockWnd  
{
private:
	HWND	m_hParentWnd;
	HWND	m_hWnd;

	SOCKET	m_Socket;
	BOOL	m_bConnect;

	char	m_iPacketMode;
	int		m_iPacketSize;
	CPacket m_cPacket;


	static	CSockWnd *m_pSockWnd;
	UINT8	m_Stream[MAX_STREAM_SIZE];
	UINT16 m_iReadStreamCount;
	
#if 1
	CQueue<CPacket> *m_RecvQ;
	CQueue<CPacket> *m_SendQ;
#else
	CMemoryPoolQueue<CPacket> *m_RecvQ;
	CMemoryPoolQueue<CPacket> *m_SendQ;
#endif
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if(m_pSockWnd)
		{
			return m_pSockWnd->WndProcSock(hWnd, message, wParam, lParam);
		}
		else 
			return 0;
    }
	
	void RecvInit()
	{
		m_iPacketMode = HEADER_MODE;
		m_iPacketSize = 0;
		m_cPacket.Clean();
	}
	
public:

	bool Init(HWND hParentWnd, HINSTANCE hParentInst);
	void Exit();

	BOOL Create(HWND hWnd, HINSTANCE hInst);
	BOOL Connect(LPCTSTR szIPAddress, UINT iPort);
	BOOL Disconnect();

	void ForGracefulDisconnect();
#if 1
	void SetQueue(CQueue<CPacket> *PlQueue,CQueue<CPacket> *cSendQ);
#else
	void SetQueue(CMemoryPoolQueue<CPacket> *PlQueue, CMemoryPoolQueue<CPacket> *cSendQ);
#endif

	BOOL RecvPacket();
	bool MakePacket(const int iMaxSize);
	int RecvStream(const int iMaxSize);
	int	GetPacket(_MGPACKET **pPacket);
	bool SendPacketQ();
	HWND GetParentWnd(){	return m_hParentWnd; }

	void PrintBuff(unsigned char* buf, int len );

	LRESULT WndProcSock(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	SOCKET GetSocket(){return m_Socket;};

	BOOL IsValidWindow() 
	{ 
		if (m_pSockWnd)		return TRUE;
		return FALSE;
	}

	BOOL IsConnected() 
	{ 
		if (m_pSockWnd)		return m_bConnect;
		return FALSE;
	}

public:

	CSockWnd();
	virtual ~CSockWnd();

};

#endif