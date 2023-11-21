// PacketProcDefines.h

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CSessionContext;
class CIOCPThread2;
class CServerMainThread;

typedef void (*LPPACKET_PROC)(LPVOID pThis,CSessionContext* pSessionContext,CPacketMsg* pPacketMsg);

//#define REGISTER_PACKET_PROC(X) GetPacketProcManager(LPPACKET_PROC)->Register(##X,(LPPACKET_PROC)CServerMainThread::Proc_##X)
#define REGISTER_PACKET_PROC(X) g_ProcMgr.Register(##X,(LPPACKET_PROC)CServerMainThread::Proc_##X)
#define DECLARE_PACKET_PROC(X) static void Proc_##X(CServerMainThread* pThis,CSessionContext* pSessionContext,CPacketMsg* pPacketMsg)

#ifdef _DEBUG
	// 일반적인 용도
	#define DEFINE_PACKET_PROC(X) \
		void CServerMainThread::Proc_##X(CServerMainThread* pThis,CSessionContext* pSessionContext,CPacketMsg* pPacketMsg) { \
		CFunctionDebugger funcDebugger(#X); \
		UNREFERENCED_PARAMETER(pThis); \
		UNREFERENCED_PARAMETER(pSessionContext); \
		UNREFERENCED_PARAMETER(pPacketMsg); \
		DebugPrint("PacketID=[%s(%d)] : Len=[%d]\n",#X,pPacketMsg->GetPacketID(),pPacketMsg->GetLength());

	// 주기적으로 수신되는 패킷인데 디버그 메세지가 불필요한 경우에 사용
	#define DEFINE_PACKET_PROC_NO_DEBUGPRINT(X) \
		void CServerMainThread::Proc_##X(CServerMainThread* pThis,CSessionContext* pSessionContext,CPacketMsg* pPacketMsg) { \
		UNREFERENCED_PARAMETER(pThis); \
		UNREFERENCED_PARAMETER(pSessionContext); \
		UNREFERENCED_PARAMETER(pPacketMsg); 
#else
	#define DEFINE_PACKET_PROC(X) \
		void CServerMainThread::Proc_##X(CServerMainThread* pThis,CSessionContext* pSessionContext,CPacketMsg* pPacketMsg) { \
		UNREFERENCED_PARAMETER(pThis); \
		UNREFERENCED_PARAMETER(pSessionContext); \
		UNREFERENCED_PARAMETER(pPacketMsg);

	#define DEFINE_PACKET_PROC_NO_DEBUGPRINT(X)	DEFINE_PACKET_PROC(X)
#endif

#define DEFINE_PACKET_END()		}	

#define DECLARE_PACKET_PROC2(X) static void Proc_##X(LPVOID pThis,CSessionContext* pSessionContext,CPacketMsg* pPacketMsg)
#define REGISTER_RSPACKET_PROC(X) g_ProcMgr.Register(##X,(LPPACKET_PROC)Proc_##X)

#ifdef _DEBUG
	#define DEFINE_PACKET_PROC3(X) \
		void Proc_##X(LPVOID pThis, CSessionContext* pSessionContext,CPacketMsg* pPacketMsg){ \
			DebugPrint("PacketID=[%s(%d)] : Len=[%d]\n",#X,pPacketMsg->GetPacketID(),pPacketMsg->GetLength()); \
			UNREFERENCED_PARAMETER(pThis); \
			UNREFERENCED_PARAMETER(pSessionContext); 
#else
	#define DEFINE_PACKET_PROC3(X) \
		void Proc_##X(LPVOID pThis, CSessionContext* pSessionContext,CPacketMsg* pPacketMsg) { \
			UNREFERENCED_PARAMETER(pThis); \
			UNREFERENCED_PARAMETER(pSessionContext); 
#endif

//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
	#define GETVALUE(X) if(!pPacketMsg->GetValue((LPVOID)&X,sizeof(X))){ \
		assert(false && "NO more Parameters!!!"); return; }
	#define GETDATA(X,LEN) if(!pPacketMsg->GetValue((LPVOID)X,LEN)){ \
		assert(false && "NO more Parameters!!!"); return; }
#else
	#define GETVALUE(X) if(!pPacketMsg->GetValue((LPVOID)&X,sizeof(X))){ \
		assert(false && "NO more Parameters!!!"); \
		SC_ERROR("NO more Parameters!!!\n"); return; }
	#define GETDATA(X,LEN) if(!pPacketMsg->GetValue((LPVOID)X,LEN)){ \
		assert(false && "NO more Parameters!!!"); \
		SC_ERROR("NO more Parameters!!!\n"); return; }
#endif