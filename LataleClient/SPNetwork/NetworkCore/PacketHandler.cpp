#include "stdafx.h"
#include <tchar.h>
#include <stdio.h>

#include "PacketHandler.h"
#include "LataleCipherDefines.h"

#include "DebugPrint.h"
#include "lgclientlog.h"


#if 0
	#include "./NPGameGuard/NPGameLib.h"
#else
	#include "LataleCipher.h"
#endif

void CPacketHandler::PostMsg(CPacket *tPacket)
{
#ifdef USING_LATALE_CIPHER
#if 0
	if(tPacket->GetPubKey() == CIPHER_LIB_NPROTECT)
	{
		UINT16 usEncryptLen = EncryptPacket(tPacket->GetBodyData(),tPacket->GetBodyLength());
		if(usEncryptLen == 0)
		{
			DWORD dwError = PPGetLastError();

			TCHAR szBuf[MAX_PATH];
			_stprintf(szBuf,_T("EncryptPacket() failed. ErrCode=[%d]\n"),dwError);
			OutputDebugString(szBuf);

			assert(false && "PostMsg::EncryptPacket() failed");
			return;
		}
		tPacket->SetLength(usEncryptLen);
		tPacket->SetPubKey(Cipher_GetMode());
	}
#else
	//{
		UINT16 usEncryptLen = Cipher_Encrypt(tPacket->GetBodyData(),tPacket->GetBodyLength(),0);
		if(usEncryptLen == 0)
		{
			DWORD dwError = Cipher_GetLastError();

			TCHAR szBuf[MAX_PATH];
			_stprintf(szBuf,_T("Cipher_Encrypt() failed. ErrCode=[%d]\n"),dwError);
			OutputDebugString(szBuf);

			assert(false && "PostMsg::EncryptPacket() failed");
			return;
		}
		tPacket->SetLength(usEncryptLen);
		tPacket->SetPubKey(Cipher_GetMode());
	//}
#endif
#endif
	// send Q에 보내기.
	if( m_cSocket->IsConnected() )
		m_cSendQ.AddQ(tPacket);
	else
		SendMessage(m_cSocket->GetParentWnd(), WM_SOCKET_FAIL, 0, 0);
}
void CPacketHandler::Parsing()
{
	while(m_cPacketQ.PopQ(m_cPacket))
	{
#ifdef USING_LATALE_CIPHER
#if 0
		if(m_cPacket.GetPubKey() == CIPHER_LIB_NPROTECT)
		{
			UINT16 usOriginBodyLen = DecryptPacket(m_cPacket.GetBodyData(),m_cPacket.GetBodyLength());
			if(usOriginBodyLen == 0)
			{
				DWORD dwError = PPGetLastError();

				TCHAR szBuf[MAX_PATH];
				_stprintf(szBuf,_T("DecryptPacket() failed. ErrCode=[%d]\n"),dwError);
				OutputDebugString(szBuf);
				continue;
			}

			m_cPacket.SetLength(usOriginBodyLen);
		}
#else
		if(m_cPacket.GetPubKey() != CIPHER_LIB_NONE)
		{
			UINT16 usOriginBodyLen = Cipher_Decrypt(m_cPacket.GetBodyData(),m_cPacket.GetBodyLength(),0);
			if(usOriginBodyLen == 0)
			{
				DWORD dwError = Cipher_GetLastError();

				TCHAR szBuf[MAX_PATH];
				_stprintf(szBuf,_T("Cipher_Decrypt() failed. ErrCode=[%d]\n"),dwError);
				OutputDebugString(szBuf);
				continue;
			}

			m_cPacket.SetLength(usOriginBodyLen);
		}
#endif
#endif

		// 실행
		(*m_pfProc)(&m_cPacket);

	}

}
