#include "crc32.h"

CCrc32 g_CRC32;

CCrc32::CCrc32(void)
{
	Init_CRC32_Table();
}

CCrc32::~CCrc32(void)
{
}

unsigned long CCrc32::Reflect(unsigned long ref, char ch)
{
	unsigned long value(0);
	for(int i = 1; i < (ch + 1); i++)
	{
		if(ref & 1)
			value |= 1 << (ch - i);
		ref >>= 1;
	}
	return value;
}

void CCrc32::Init_CRC32_Table()
{
	unsigned long ulPolynomial = 0x058CE19A;
	for(int i = 0; i <= 0xFF; i++)
	{
		crc32_table[i]=Reflect(i, 8) << 24;
		for (int j = 0; j < 8; j++)
			crc32_table[i] = (crc32_table[i] << 1) ^ (crc32_table[i] & (1 << 31) ? ulPolynomial : 0);
		crc32_table[i] = Reflect(crc32_table[i], 32);
	}
}

int CCrc32::Get_CRC(unsigned char * csData, unsigned long dwSize)
{
	int len;
	unsigned long  crc(0xffffffff);
	unsigned char* buffer;

	len = dwSize;
	buffer = csData;
	while(len--)
	{
		crc = (crc >> 8) ^ crc32_table[(crc & 0xFF) ^ *buffer++];
	}
	return crc^0xffffffff;
}