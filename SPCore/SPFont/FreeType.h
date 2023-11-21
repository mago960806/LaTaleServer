#pragma once

#include <windows.h>
#include <vector>

struct BitmapInfo
{
	int		x,y,w,h;
	BOOL	bMono;
	int		pitch;
	BYTE *	pBuf;
};

class CFreeType
{
public:
	CFreeType(void);
	~CFreeType(void);

	int  	GetFont(void);
	void 	SetFont(int iFont);
	BOOL	GetWord(BitmapInfo **pBI,WCHAR wChar,int iSize = 12);
	BOOL    AddFont(const char* path);

private:
	//变量定义
	std::vector<void*>  m_Fonts; //字体的数据
	int     m_iCurFont;
};