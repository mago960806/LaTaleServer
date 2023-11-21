// CPacketComposer.cpp: implementation of the CPacketComposer class.
//
//////////////////////////////////////////////////////////////////////


// <PacketChunk> ::= <len:2><key:2><packetID:4><data:len-8>
//
//			<len:2> ::= sizeof(<PacketChunk>)
//


#include "stdafx.h"
#include "CPacketComposer.h"
#include "LataleCipherDefines.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//CPacketComposer::CPacketComposer() : m_bHasData(false), m_Len(0), m_Key(DEFAULT_SECUTIRY_KEY)
//{
//}

extern bool g_bUseEncryption;

CPacketComposer::CPacketComposer(const unsigned int& packetID) : m_bHasData(false), m_Len(0),m_Key(DEFAULT_SECURITY_KEY)
{
#ifdef USING_LATALE_CIPHER
	if(g_bUseEncryption)
		m_Key = LATALE_SECURITY_KEY;
#endif

	Init(packetID);
}

CPacketComposer::~CPacketComposer()
{
}

void CPacketComposer::Init(const unsigned int& packetID)
{
	memset(m_szBuf,0,sizeof(m_szBuf));
	m_pBuffer = m_szBuf;

	m_Len = 2+2+4;

	memcpy(m_pBuffer,&m_Len,sizeof(unsigned short));	m_pBuffer += sizeof(unsigned short);
	memcpy(m_pBuffer,&m_Key,sizeof(unsigned short));	m_pBuffer += sizeof(unsigned short);
	memcpy(m_pBuffer,&packetID,sizeof(unsigned int));	m_pBuffer += sizeof(unsigned int);

	m_bHasData = true;
}

void CPacketComposer::SetKey(unsigned short usKey)
{
	m_Key = usKey;

	memcpy(m_szBuf+sizeof(unsigned short),&m_Key,sizeof(unsigned short));
}

//////////////////////////////////////////////////////////////////////

void CPacketComposer::PushBack(const LPVOID pData,const unsigned short& len)
{
	if(MAX_PACKET_SIZE < m_Len+len)
	{
		SC_ERROR(">>>>>>>>>>MAX_PACKET_SIZE < m_Len+len\n");
		assert(false && "MAX_PACKET_SIZE < m_Len+len");
		return;
	}

	memcpy(m_pBuffer,pData,len);  m_pBuffer += len;
	m_Len += len;
	memcpy(m_szBuf,&m_Len,sizeof(unsigned short));
}

void CPacketComposer::PushBack(const TCHAR* pData,const unsigned short& len)
{
	if(MAX_PACKET_SIZE < m_Len+len)
	{
		SC_ERROR(">>>>>>>>>>MAX_PACKET_SIZE < m_Len+len\n");
		assert(false && "MAX_PACKET_SIZE < m_Len+len");
		return;
	}

	memcpy(m_pBuffer,pData,len);  m_pBuffer += len;
	m_Len += len;
	memcpy(m_szBuf,&m_Len,sizeof(unsigned short));
}

void CPacketComposer::PushBack(const TCHAR& value)
{
	PushBack(&value,sizeof(value));
}

void CPacketComposer::PushBack(const short& value)
{
	PushBack((const TCHAR*)&value,sizeof(value));
}

void CPacketComposer::PushBack(const int& value)
{
	PushBack((const TCHAR*)&value,sizeof(value));
}

void CPacketComposer::PushBack(const long& value)
{
	PushBack((const TCHAR*)(const TCHAR*)&value,sizeof(value));
}

void CPacketComposer::PushBack(const float& value)
{
	PushBack((const TCHAR*)(const TCHAR*)&value,sizeof(value));
}

void CPacketComposer::PushBack(const double& value)
{
	PushBack((const TCHAR*)&value,sizeof(value));
}

void CPacketComposer::PushBack(const unsigned char& value)
{
	PushBack((const TCHAR*)&value,sizeof(value));
}

void CPacketComposer::PushBack(const unsigned short& value)
{
	PushBack((const TCHAR*)&value,sizeof(value));
}

void CPacketComposer::PushBack(const unsigned int& value)
{
	PushBack((const TCHAR*)&value,sizeof(value));
}

void CPacketComposer::PushBack(const unsigned long& value)
{
	PushBack((const TCHAR*)&value,sizeof(value));
}

void CPacketComposer::PushBack(const INT64& value)
{
	PushBack((const TCHAR*)&value,sizeof(value));
}

void CPacketComposer::PushBack(const UINT64& value)
{
	PushBack((const TCHAR*)&value,sizeof(value));
}

void CPacketComposer::PushBack(const SYSTEMTIME& value)
{
	PushBack((const TCHAR*)&value,sizeof(value));
}
/*
void CPacketComposer::PushBack(const VECTOR3& value)
{
	PushBack((const TCHAR*)&value,sizeof(value));
}

CPacketComposer& CPacketComposer::operator<<(const VECTOR3& value)
{
	PushBack(value);
	return *this;
}
*/
CPacketComposer& CPacketComposer::operator<<(const TCHAR& value)
{
	PushBack(value);
	return *this;
}

CPacketComposer& CPacketComposer::operator<<(const short& value)
{
	PushBack(value);
	return *this;
}

CPacketComposer& CPacketComposer::operator<<(const int& value)
{
	PushBack(value);
	return *this;
}

CPacketComposer& CPacketComposer::operator<<(const long& value)
{
	PushBack(value);
	return *this;
}

CPacketComposer& CPacketComposer::operator<<(const float& value)
{
	PushBack(value);
	return *this;
}

CPacketComposer& CPacketComposer::operator<<(const double& value)
{
	PushBack(value);
	return *this;
}

CPacketComposer& CPacketComposer::operator<<(const unsigned char& value)
{
	PushBack(value);
	return *this;
}

CPacketComposer& CPacketComposer::operator<<(const unsigned short& value)
{
	PushBack(value);
	return *this;
}

CPacketComposer& CPacketComposer::operator<<(const unsigned int& value)
{
	PushBack(value);
	return *this;
}
CPacketComposer& CPacketComposer::operator<<(const unsigned long& value)
{
	PushBack(value);
	return *this;
}

CPacketComposer& CPacketComposer::operator<<(const SYSTEMTIME& value)
{
	PushBack(value);
	return *this;
}

CPacketComposer& CPacketComposer::operator<<(const INT64& value)
{
	PushBack(value);
	return *this;
}

CPacketComposer& CPacketComposer::operator<<(const UINT64& value)
{
	PushBack(value);
	return *this;
}
