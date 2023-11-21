#include "StdAfx.h"
#include "Packet.h"

#include "PacketID.h"

CPacket::CPacket(void)
{
	Clean();
}

CPacket::~CPacket(void)
{
}

void CPacket::Clean()
{
	memset(&m_PacketData,0,MAX_PACKET_SIZE);
	m_Header.length	= PACKET_HEADER_SIZE;
	m_RPoint		= PACKET_HEADER_SIZE;
	m_Header.pubkey = 0 ;
}

void CPacket::InsertHead(PACKET_HEADER *header_data)
{
	Clean();
	m_Header=(*header_data);
}
void CPacket::UpdateHead()
{
	memcpy(m_PacketData,&m_Header,PACKET_HEADER_SIZE);
}

CPacket & CPacket::operator>> (UINT8 data)
{
	*(GetWPoint()) = data;
	UpdateWPoint (1);
	return *this;
}

CPacket & CPacket::operator>> (UINT16 data)
{
	memcpy(GetWPoint(),&data,2);
	UpdateWPoint (2);
	return *this;
}
CPacket & CPacket::operator>> (UINT32 data)
{
	memcpy(GetWPoint(),&data,4);
	UpdateWPoint (4);
	return *this;
}


CPacket & CPacket::operator>> (float data)
{
	memcpy(GetWPoint(),&data,4);
	UpdateWPoint (4);
	return *this;
}

CPacket & CPacket::operator>> (UINT64 data)
{
	memcpy(GetWPoint(),&data,8);
	UpdateWPoint (8);
	return *this;
}

CPacket & CPacket::operator>> (char *data)
{
	memcpy(GetWPoint(),(UINT8*)data,strlen(data)+1);
	UpdateWPoint ((UINT16)strlen(data)+1);
	return *this;
}

CPacket & CPacket::operator<< (UINT8 *data)
{
	*data = *(GetRPoint());
	UpdateRPoint (1);
	return *this;
}

CPacket & CPacket::operator<< (UINT16 *data)
{
	memcpy(data,GetRPoint(),2);
	UpdateRPoint (2);
	return *this;
}

CPacket & CPacket::operator<< (UINT32 *data)
{
	memcpy(data,GetRPoint(),4);
	UpdateRPoint (4);
	return *this;
}

CPacket & CPacket::operator<< (UINT64 *data)
{
	memcpy(data,GetRPoint(),8);
	UpdateRPoint (8);
	return *this;
}

CPacket & CPacket::operator<< (float *data)
{
	memcpy(data,GetRPoint(),4);
	UpdateRPoint (4);
	return *this;
}

CPacket & CPacket::operator<< (char *data)
{
	strcpy(data,(char*)GetRPoint());
	UpdateRPoint ( (UINT16)(strlen(data)+1));
	return *this;
}


void CPacket::Add(UINT8 data)
{

	*(GetWPoint()) = data;
	UpdateWPoint (1);
}

void CPacket::Add(UINT16 data)
{ 
	memcpy(GetWPoint(),&data,2);
	UpdateWPoint (2);
}

// UINT32;
void CPacket::Add(UINT32 data)
{
	memcpy(GetWPoint(),&data,4);
	UpdateWPoint (4);
}

void CPacket::Add(UINT64 data)
{
	memcpy(GetWPoint(),&data,8);
	UpdateWPoint (8);
}

void CPacket::Add(float data)
{
	memcpy(GetWPoint(),&data,4);
	UpdateWPoint (4);
}


// String
void CPacket::Add(char *data)
{
	memcpy(GetWPoint(),(UINT8*)data,strlen(data)+1);
	UpdateWPoint ((UINT16)strlen(data)+1);
}

// Structure
void CPacket::Add(void *data,UINT16 size)
{
	memcpy(GetWPoint(),(UINT8*)data,size);
	UpdateWPoint (size);
}

// UINT8
void CPacket::Extract(UINT8 *data)
{
	memcpy(data,GetRPoint(),1);
	UpdateRPoint (1);
}

// UINT16
void CPacket::Extract(UINT16 *data)
{
	memcpy(data,GetRPoint(),2);
	UpdateRPoint (2);
}

// UINT32
void CPacket::Extract(UINT32 *data)
{
	memcpy(data,GetRPoint(),4);
	UpdateRPoint (4);
}

// UINT64
void CPacket::Extract(UINT64 *data)
{
	memcpy(data,GetRPoint(),8);
	UpdateRPoint (8);
}

// float
void CPacket::Extract(float *data)
{
	memcpy(data,GetRPoint(),4);
	UpdateRPoint (4);
}

// String
void CPacket::Extract(char *dest,UINT16 iDestMaximumSize)
{
	strncpy(dest,(char*) GetRPoint(), iDestMaximumSize);
	UpdateRPoint ((UINT16) iDestMaximumSize);
}

// STRUCTURE
void CPacket::Extract(void *dest,UINT16 size)
{
	memcpy(dest,GetRPoint(),size);
	UpdateRPoint (size);
}


UINT8 CPacket::CheckValidation( )
{
	return 0;
}