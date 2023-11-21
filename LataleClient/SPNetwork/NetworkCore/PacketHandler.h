#ifndef PACKET_HANDLER_H
#define PACKET_HANDLER_H

#include <Windows.h>
#include "BaseTypes.h"
#include "Packet.h"
#include "Queue.h"
#include "SockWnd.h"

enum PACKET_HANDLER_ERR
{
	HANDLE_ESTABLISHED, // SUCCESS 
	HANDLE_SOCKWND_FAIL,
	HANDLE_SOCKCONN_FAIL,
};

typedef void (*func_ptr) (CPacket *);

class CPacketHandler
{
private:
#if 1
	CQueue<CPacket> m_cPacketQ;
	CQueue<CPacket> m_cSendQ; 
#else
	CMemoryPoolQueue<CPacket> m_cPacketQ;
	CMemoryPoolQueue<CPacket> m_cSendQ; 
#endif

	CPacket m_cPacket;
	CSockWnd *m_cSocket;
	func_ptr m_pfProc;


public :


	bool Sending()
	{
		if(!m_cSocket->IsConnected())
			return false;
		return m_cSocket->SendPacketQ();
	}

	void PostMsg(CPacket *tPacket);
	void Parsing();

	PACKET_HANDLER_ERR GetError()
	{
		if(!m_cSocket->IsValidWindow()) return HANDLE_SOCKWND_FAIL;
		if(!m_cSocket->IsConnected()) return HANDLE_SOCKCONN_FAIL;
		
		return HANDLE_ESTABLISHED;
	}

	CPacketHandler()
	{
	}
	CPacketHandler(CSockWnd *tSockWnd, func_ptr pFunction)
	{
		if(tSockWnd == NULL) puts("Error");

		m_cSocket = tSockWnd;
		m_pfProc = pFunction;
		m_cPacketQ.reset();
		m_cSendQ.reset();
		tSockWnd->SetQueue(&m_cPacketQ,&m_cSendQ);
	}
	~CPacketHandler()
	{
	}
};

#endif