// PacketMsg.h

#include "StdAfx.h"
#include "PacketMsg.h"

#ifdef _LATALE_SERVER
	#include "../LataleCipher/LataleCipher.h"
#endif

CPacketMsg::CPacketMsg(void) : m_bEncrypted(false)
{
	memset(m_szPacketBuffer,0,sizeof(m_szPacketBuffer));
	m_pReadPoint = NULL;
	m_dwPacketID = m_usKey = m_usPacketLen = 0;

	m_dwMsgID = 0;
	m_dwSessionID = 0;
	m_dwCreateTime = 0;

	m_iDataLen = 0;
}

// Plain Text 패킷 내용을 수신하여 packetMsg 내부 데이터에 설정
USHORT CPacketMsg::Set_PlainText(DWORD dwSessionID,BYTE* pszData,USHORT usLen)
{
	if(pszData == NULL || usLen < 1)
	{
		SC_ERROR("Invalid Parameters.\n");
		assert(false && "Invalid Parameters.");
		return 0;
	}

	m_dwSessionID = dwSessionID;

	usLen = min(usLen,MAX_PACKET_SIZE);
	memcpy(m_szPacketBuffer,pszData,usLen);

	m_pReadPoint = m_szPacketBuffer;

	SetEncryptMode(false);

	// <PacketChunk> ::= <len:2><key:2><packetID:4><data:len-8>
	//
	//			<len:2> ::= sizeof(<PacketChunk>)
	//

	memcpy(&m_usPacketLen,m_pReadPoint,2);	m_pReadPoint += 2;
	memcpy(&m_usKey,m_pReadPoint,2);		m_pReadPoint += 2;
	memcpy(&m_dwPacketID,m_pReadPoint,4);	m_pReadPoint += 4;

	assert(usLen == m_usPacketLen);

	m_iDataLen = max(0,m_usPacketLen-8);

	return m_usPacketLen;
}


USHORT CPacketMsg::Set(DWORD dwSessionID,BYTE* pszData,USHORT usLen,int iKeyValue)
{
	m_dwSessionID = dwSessionID;

	usLen = min(usLen,MAX_PACKET_SIZE);
	memcpy(m_szPacketBuffer,pszData,usLen);

	m_pReadPoint = m_szPacketBuffer;

	// <PacketChunk> ::= <len:2><key:2><packetID:4><data:len-8>
	//
	//			<len:2> ::= sizeof(<PacketChunk>)
	//

	memcpy(&m_usPacketLen,m_pReadPoint,2);	m_pReadPoint += 2;
	memcpy(&m_usKey,m_pReadPoint,2);		m_pReadPoint += 2;
	USHORT usOriginLen = usLen;

	m_usPacketLen = usOriginLen;
	
#ifdef USING_LATALE_CIPHER
	
	#define PLAIN_TEXT	0
	#define OLD_MAGIC_KEY	77
	
		if(m_usKey != PLAIN_TEXT && m_usKey != OLD_MAGIC_KEY)
		{
			if(IsEncrypted())
				usOriginLen = Cipher_Decrypt(m_pReadPoint,usLen-4,iKeyValue);
		}
#endif

	if(usOriginLen != 0)
	{
		if(usOriginLen >= 4)
		{
			memcpy(&m_dwPacketID,m_pReadPoint,4);		m_pReadPoint += 4;

			if(IsEncrypted())
				m_usPacketLen = usOriginLen+4;

			memcpy(m_szPacketBuffer,&m_usPacketLen,2);
		}
		else
		{
			assert(usOriginLen >= 4);
			return 0;
		}
	}
	else	
	{
#ifdef USING_LATALE_CIPHER
		DWORD dwError = Cipher_GetLastError();
		ErrorLog("CPacketMsg::Set::Decrypt() failed. ErrorCode=[%d]\n",dwError);
		assert(false && "usOriginLen == 0");
#endif
	}

	m_iDataLen = max(0,m_usPacketLen-8);

	return usOriginLen;
}

////////////////////////////////////////////////////////////////////////////

bool CPacketMsg::GetValue(LPVOID pData,int iLen)
{
	if(m_iDataLen - iLen < 0)
	{
		return false;
	}
	memcpy(pData,GetReadPoint(),iLen);
	m_iDataLen -= iLen;
	m_pReadPoint += iLen;
	return true;
}
