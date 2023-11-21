#ifndef PACKET_H
#define PACKET_H


#include <Windows.h>
#include <assert.h>
#include "BaseTypes.h"

#pragma once

#if 1

typedef UINT16 PACKET_LENGTH ;
typedef UINT16 PACKET_PUBKEY ;
typedef UINT32 PACKET_COMMAND;

// 패킷의 헤더부
// 해더는 어떤패킷이든 반드시 존재해야함.
typedef struct __PACKET_HEADER
{
	PACKET_LENGTH length;
	PACKET_PUBKEY pubkey;
} PACKET_HEADER;

const UINT16 PACKET_HEADER_SIZE		= sizeof(PACKET_HEADER);
const UINT16 PACKET_LENGTH_SIZE		= sizeof(PACKET_LENGTH);
const UINT16 PACKET_PUBKEY_SIZE		= sizeof(PACKET_PUBKEY);
const UINT16 PACKET_COMMAND_SIZE	= sizeof(PACKET_COMMAND);
const UINT16 PACKET_BODY_SIZE		= MAX_PACKET_SIZE - PACKET_HEADER_SIZE - PACKET_COMMAND_SIZE;

typedef struct __PACKET_BODY
{
	PACKET_COMMAND iCommand;
	UINT8 Data[PACKET_BODY_SIZE];
}PACKET_BODY;

#else
//struct PACKET_HEADER
//{
//	UINT16 length;
//	UINT16 pubkey;
//};
//#define MAX_BODY_DATA (MAX_PACKET_SIZE - sizeof(PACKET_HEADER) - sizeof(UINT32))	
//struct PACKET_BODY
//{
//	UINT32 iCommand;
//	UINT8 Data[MAX_BODY_DATA];
//};
//const UINT16 PACKET_HEADER_SIZE = sizeof(PACKET_HEADER);
#endif



class CPacket
{
protected:

#if 1
	PACKET_LENGTH m_RPoint; // Extract, >> 포인터
#else
    UINT16 m_RPoint; // Extract, >> 포인터
#endif
	// Actually, Network Stream included packet header
	union
	{
		struct  
		{
			PACKET_HEADER	m_Header;
			PACKET_BODY		m_Body;
		};

		UINT8	m_PacketData[MAX_PACKET_SIZE]; 

	};

public:

	CPacket(void);
	CPacket(PACKET_COMMAND iCommand)
	{	
		Clean();
		AddCommand(iCommand);
	}
	~CPacket(void);

#if 1
	PACKET_HEADER*			GetHead()		{ return &m_Header;}
	UINT8*					GetPacketData() { return m_PacketData;}
	UINT8*					GetBodyData()	{ return m_PacketData + PACKET_HEADER_SIZE; }

	PACKET_LENGTH			GetLength()		{ return m_Header.length; }
	PACKET_LENGTH			GetBodyLength()	{ return m_Header.length - PACKET_HEADER_SIZE; }
	PACKET_PUBKEY			GetPubKey(){return m_Header.pubkey;}
	
	void			SetLength(PACKET_LENGTH usOriginBodyLen) { m_Header.length = usOriginBodyLen + PACKET_HEADER_SIZE; }

	PACKET_COMMAND GetCommand()	
	{ 
		UpdateRPoint (PACKET_COMMAND_SIZE);
		return m_Body.iCommand; 
	}

	//[xialin 2008/08/22]add
	PACKET_COMMAND GetCmd()
	{ 
		PACKET_COMMAND  iCmd;
		UpdateRPoint (PACKET_COMMAND_SIZE);
		iCmd = m_Body.iCommand;
		m_RPoint -= PACKET_COMMAND_SIZE;
		return iCmd;
	}


	void AddCommand(PACKET_COMMAND iCommnad)	
	{ 
		memcpy(GetBodyData(),&iCommnad, PACKET_COMMAND_SIZE);
		UpdateWPoint (PACKET_COMMAND_SIZE);
	}

	void UpdateCommand(PACKET_COMMAND iCommnad)	
	{ 
		memcpy(GetBodyData(),&iCommnad, PACKET_COMMAND_SIZE);
	}

	UINT8*			GetRPoint	(){ return m_PacketData + m_RPoint; }
	UINT8*			GetWPoint	(){ return m_PacketData + m_Header.length; }
	void			UpdateRPoint(PACKET_LENGTH tSize	){ m_RPoint += tSize; }
	void			UpdateWPoint(PACKET_LENGTH tSize	){ m_Header.length += tSize; }
	void			SetRPoint	(PACKET_LENGTH tCount	){ m_RPoint = PACKET_HEADER_SIZE + tCount; } 

public:
	void Clean();
	void SetPubKey(PACKET_PUBKEY key){m_Header.pubkey=key;}
	void UpdateHead();
	void InsertHead(PACKET_HEADER *header_data);

	UINT8 CheckValidation();

#else

	//PACKET_HEADER*	GetHead()		{ return &m_Header;}
	//UINT8*			GetPacketData() { return m_PacketData;}
	//UINT8*			GetBodyData()	{ return m_PacketData + PACKET_HEADER_SIZE; }
	//UINT16			GetLength()		{ return m_Header.length; }

	//void			SetLength(UINT16 usOriginBodyLen){m_Header.length = usOriginBodyLen + PACKET_HEADER_SIZE;}
	//UINT16			GetBodyLength()	{ return m_Header.length - PACKET_HEADER_SIZE; }

	//UINT16			GetPubKey(){return m_Header.pubkey;}

	//UINT32			GetCommand()	
	//{ 
	//	UpdateRPoint (4);
	//	return m_Body.iCommand; 
	//}

	//void 			AddCommand(UINT32 iCommnad)	
	//{ 
	//	memcpy(GetBodyData(),&iCommnad, 4);
	//	UpdateWPoint (4);
	//}

	//void 			UpdateCommand(UINT32 iCommnad)	
	//{ 
	//	memcpy(GetBodyData(),&iCommnad, 4);
	//}

	//UINT8*			GetRPoint()		{ return m_PacketData + m_RPoint; }
	//UINT8*			GetWPoint()		{ return m_PacketData + m_Header.length; }
	//void			UpdateRPoint(UINT16 tSize){ m_RPoint += tSize; }
	//void			UpdateWPoint(UINT16 tSize){ m_Header.length += tSize; }
	//void			SetRPoint(UINT16 tCount){ m_RPoint = PACKET_HEADER_SIZE + tCount; } 
	//


	//void Clean();

	//void InsertHead(PACKET_HEADER *header_data);
	//
	//// delete by hoshim at 2005/7/5 : 코드상에 버그(?) 로 사용 배제.  차후 논의 필요
	////void InsertBody(UINT8 *data);
	//
	//void SetPubKey(UINT16 key){m_Header.pubkey=key;} // Encrypt key
	//void UpdateHead();
	//	
	//UINT8 CheckValidation();
#endif

	//-----------------------------------------------------------------------------//
	// 개발자가 불편하더라도 명확함을 추구하는게 좋지 않을까는 개인적인 생각
	// 논의후 비추부분을 빼서 아예 강제적으로 사용하지 못하도록 하는것도 괜찮을듯..
    //-----------------------------------------------------------------------------//

	void AddUINT8(UINT8 data) 
	{	
		*(GetWPoint()) = data;
		UpdateWPoint (1);;
	}
	void AddUINT16(UINT16 data)
	{	
		memcpy(GetWPoint(),&data,2);
		UpdateWPoint (2);;
	}
	void AddUINT32(UINT32 data)
	{	
		memcpy(GetWPoint(),&data,4);
		UpdateWPoint (4);;
	}
	void AddUINT64(UINT64 data)
	{	
		memcpy(GetWPoint(),&data,8);
		UpdateWPoint (8);;
	}
	void AddFloat(float data)
	{	
		memcpy(GetWPoint(),&data,4);
		UpdateWPoint (4);
	}
	//void AddString(const char *data)
	//{	
	//	memcpy(GetWPoint(),(UINT8*)data,strlen(data)+1);
	//	UpdateWPoint ((UINT16)strlen(data)+1);
	//}
	void AddData(const void *data, UINT16 size)
	{	
		memcpy(GetWPoint(),(UINT8*)data,size);
		UpdateWPoint (size);
	}

	void ExtractUINT8(UINT8 *data)
	{
		memcpy(data,GetRPoint(),1);
		UpdateRPoint (1);
	}
	void ExtractUINT16(UINT16 *data)
	{
		memcpy(data,GetRPoint(),2);
		UpdateRPoint (2);
	}
	void ExtractUINT32(UINT32 *data)
	{
		memcpy(data,GetRPoint(),4);
		UpdateRPoint (4);
	}
	void ExtractUINT64(UINT64 *data)
	{
		memcpy(data,GetRPoint(),8);
		UpdateRPoint (8);
	}
	void ExtractFloat(float *data)
	{
		memcpy(data,GetRPoint(),4);
		UpdateRPoint (4);
	}

	//void ExtractString(char *dest, UINT16 iDestMaximumSize) // 최대 스트링 
	//{
	//	strncpy(dest,(char*) GetRPoint(), iDestMaximumSize);
	//	UpdateRPoint ((UINT16) (strlen(dest)+1));
	//}

	void ExtractData(TCHAR* pszDest, UINT16 nLen) 
	{
		memcpy(pszDest,GetRPoint(), nLen);
		UpdateRPoint ((UINT16)nLen);
	}

	void ExtractStruct(void *dest, UINT16 size)
	{
		memcpy(dest,GetRPoint(),size);
		UpdateRPoint (size);
	}


	//-----------------------------------------------------------------------------//
	//-----------------------------------------------------------------------------//

	// Append 
	void Add(UINT8 data);
	void Add(UINT16 data);
	void Add(UINT32 data);
	void Add(UINT64 data);
	void Add(float data);
	void Add(char *data);
	void Add(void *data, UINT16 size);

	
	void Extract(UINT8 *data);
	void Extract(UINT16 *data);
	void Extract(UINT32 *data);
	void Extract(UINT64 *data);
	void Extract(float *data);
	void Extract(char *dest, UINT16 iDestMaximumSize); // 최대 스트링 
    void Extract(void *dest, UINT16 size);

	// operator REdefine
	CPacket & operator>> (UINT8 data);
	CPacket & operator>> (UINT16 data);
	CPacket & operator>> (UINT32 data);
	CPacket & operator>> (UINT64 data);
	CPacket & operator>> (float data);
	CPacket & operator>> (char *data);


	CPacket & operator<< (UINT8 *data);
	CPacket & operator<< (UINT16 *data);
	CPacket & operator<< (UINT32 *data);
	CPacket & operator<< (UINT64 *data);
	CPacket & operator<< (float *data);
	CPacket & operator<< (char *data);


	
};

#endif