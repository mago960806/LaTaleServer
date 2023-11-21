// PacketMsg.h

#pragma once

#include <assert.h>

#ifdef USING_LATALE_CIPHER
	#ifdef _LATALE_SERVER
		#include "nProtect/CSAuth.h"
	#elif _LATALE_CLIENT
		#include "LataleCipher.h"
		//#include "nProtect/NPGameLib.h"
	#else
		#include "../LataleCipher/LataleCipher.h"
	#endif
#endif

struct CPacketMsg
{
public:
	CPacketMsg(void);

	USHORT Set_PlainText(DWORD dwSessionID,BYTE* pszData,USHORT usLen);
	USHORT Set(DWORD dwSessionID,BYTE* pszData,USHORT usLen,int iKeyValue);

	DWORD GetLastError()
	{
		DWORD dwError = 0;
#ifdef USING_LATALE_CIPHER
	#ifdef _LATALE_SERVER
		assert(false);
	#else
		//dwError = PPGetLastError();
		dwError = Cipher_GetLastError();
	#endif
#endif
		return dwError;
	}

	BYTE* GetReadPoint()
	{
		assert(m_pReadPoint);
		return m_pReadPoint;
	}

	BYTE* GetPacketBuffer()
	{
		return m_szPacketBuffer;
	}

	USHORT GetLength()
	{
		return m_usPacketLen;
	}
	int GetDataLength()
	{
		return m_iDataLen;
	}

	USHORT GetKey()
	{
		return m_usKey;
	}

	DWORD GetPacketID()
	{
		return m_dwPacketID;
	}

	void SetEncryptMode(bool bUseEncrypt)
	{
		m_bEncrypted = bUseEncrypt;
	}
	bool IsEncrypted(){return m_bEncrypted;}

	void SetMsgID(DWORD dwID){m_dwMsgID = dwID;}
	void SetCreateTime(DWORD dwTick){m_dwCreateTime = dwTick;}

	DWORD GetMsgID(){return m_dwMsgID;}
	DWORD GetCreateTime(){return m_dwCreateTime;}
	DWORD GetSessionID(){return m_dwSessionID;}

////////////////////////////////////////////////////////////////////////////
	bool GetValue(LPVOID pData,int iLen);
////////////////////////////////////////////////////////////////////////////

public:
	BYTE	m_szPacketBuffer[MAX_PACKET_SIZE+4];
	BYTE*	m_pReadPoint;

	unsigned __int16	m_usPacketLen;
	unsigned __int16 	m_usKey;
	unsigned __int32	m_dwPacketID;

	DWORD	m_dwMsgID;
	DWORD	m_dwSessionID;
	DWORD	m_dwCreateTime;

	int		m_iDataLen;

	bool	m_bEncrypted;
};

//#undef GETVALUE
//#undef GETDATA
//
//#define GETVALUE(X) if(!pPacketMsg->GetValue(X,sizeof(X))){ \
//	assert(false && "NO more Parameters!!!"); return; }
//#define GETDATA(X,LEN) if(!pPacketMsg->GetValue(X,LEN)){ \
//	assert(false && "NO more Parameters!!!"); return; }
//
