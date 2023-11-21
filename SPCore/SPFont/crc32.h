
#pragma once

class CCrc32
{
private:
	unsigned long crc32_table[256];

	void			Init_CRC32_Table();
	unsigned long	Reflect(unsigned long ref, char ch);
public:
	CCrc32(void);
	~CCrc32(void);

	int	Get_CRC(unsigned char * csData, unsigned long dwSize);
};

extern CCrc32 g_CRC32;

