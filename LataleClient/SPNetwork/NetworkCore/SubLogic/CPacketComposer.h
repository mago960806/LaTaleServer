// CPacketComposer.h: interface for the CPacketComposer class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "basetypes.h"

class CPacketComposer  
{
	unsigned short m_Len;
	unsigned short m_Key;
	TCHAR* m_pBuffer;
	TCHAR m_szBuf[MAX_PACKET_SIZE];
	bool m_bHasData;

public:

	//CPacketComposer();
	CPacketComposer(const unsigned int& packetID);
	virtual ~CPacketComposer();
	
	const bool HasData() const{return m_bHasData;};
	const TCHAR* GetBuffer() const{return m_szBuf;};
	const UINT16 GetLength() const{return m_Len;};
	void Init(const unsigned int& packetID);

	void SetKey(unsigned short usKey) ;


	void PushBack(const LPVOID pData,const unsigned short& len);
	void PushBack(const TCHAR* pData,const unsigned short& len);

	void PushBack(const TCHAR& value);
	void PushBack(const short& value);
	void PushBack(const int& value);
	void PushBack(const long& value);
	void PushBack(const float& value);
	void PushBack(const double& value);
	void PushBack(const unsigned char& value);
	void PushBack(const unsigned short& value);
	void PushBack(const unsigned int& value);
	void PushBack(const unsigned long& value);
	void PushBack(const SYSTEMTIME& timeData);
	void PushBack(const INT64& value);
	void PushBack(const UINT64& value);
	//void PushBack(const VECTOR3& value);

	//CPacketComposer& operator<<(const VECTOR3& value);
	
	CPacketComposer& operator<<(const TCHAR& value);
	CPacketComposer& operator<<(const short& value);
	CPacketComposer& operator<<(const int& value);
	CPacketComposer& operator<<(const long& value);
	CPacketComposer& operator<<(const float& value);
	CPacketComposer& operator<<(const double& value);
	CPacketComposer& operator<<(const unsigned char& value);
	CPacketComposer& operator<<(const unsigned short& value);
	CPacketComposer& operator<<(const unsigned int& value);
	CPacketComposer& operator<<(const unsigned long& value);
	CPacketComposer& operator<<(const SYSTEMTIME& timeData);
	CPacketComposer& operator<<(const INT64& value);
	CPacketComposer& operator<<(const UINT64& value);
};

