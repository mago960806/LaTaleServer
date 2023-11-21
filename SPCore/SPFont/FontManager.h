#pragma once

//#include "Global/include/FreeType.h"
#include "freetype.h"
#include "FixPool.h"
//#include "TextureD3D.h"
//#include "sdui.h"

#define SICON_BEGIN						0xFE				// 静态表情字体开头字节
#define DICON_BEGIN						0xFF				// 动态表情字体开头字节

#define DTF_UnderLine					0x00000020
#define DTF_Bold						0x40000000
#define DTF_BlackFrame					0x20000000
#define DTF_Center						0x00000001
#define DTF_Right						0x00000002         //[xialin 2008/08/19]add
#define DTF_Shadow						0x10000000

#define FONT_SONGTI_12_ASC_SIZE	(12 * 6 * 96 / 8)
#define FONT_SONGTI_12_FNT_SIZE	(2*12 * 126*191)
#define FONT_SONGTI_14_ASC_SIZE	(2*7*14 * 95)
#define FONT_SONGTI_14_FNT_SIZE	(2*14 * 126*191)

enum eFont_Type
{
	FONT_SONGTI          =      0,//宋体
	FONT_LISHU           =      1,//隶书
	FONT_FREETYPE_NUM    =      2,//freetype支持的字体总数

	FONT_GDI_XINSONG     =   	3,// 新宋体
	FONT_GDI_KAITI		 =		4,// 楷体
	FONT_GDI_HEITI		 =		5,// 黑体
	FONT_GDI_FANGSONG	 =		6,// 仿宋体
	FONT_GDI_SONG_XIE	 =		7,// 新宋斜体
	FONT_GDI_SONG_CU	 =		8,// 新宋粗体
	FONT_GDI_YOUYUAN	 =		10,// 幼圆
	FONT_GDI_LISHU		 =		11,// 隶书
	FONT_GDI_SHUTI		 =		12,// 舒体
	FONT_GDI_TAOTI		 =		13,// 姚体
	FONT_GDI_CAIYUN		 =		14,// 彩云
	FONT_GDI_XIHEI		 =		15,// 细黑
	FONT_GDI_XINWEI		 =		16,// 新魏
	FONT_GDI_HANGKAI	 =		17,// 行楷
	FONT_GDI_ZHONGSONG	 =		18,// 中宋

	FONT_NUM             =      19,//支持的字体总数
};


enum eFont_Size
{
	FONT_SMALL = 12,
	FONT_MIDDLE = 14,
	FONT_BIG   = 16
};

//自己定义，必须跟m_FreeType.Add加入字体库的顺序一致

#define FONT12_SIZE	(12 * 6 * 96 / 8)

class SPTexture;

class CFontManager
{
	//friend class CTextureD3D;

	struct stExpression
	{
		int	iPos;
		int iIdex;
	};

	struct stTextID
	{
		int		iFontCRC;
		int		iTextCRC;
		DWORD	dwColor;
		DWORD	dwFrameColor;  //[xialin 2008/09/06]add
		DWORD	dwBkColor;
		int		iFontSize;
		DWORD	dwFlag;

		DWORD			dwCount;
		int				iw;
		int				ih;
		int				iExpSize;
		stExpression*	pExp;
		//LPTexture		pTex;
		SPTexture*		pTex;

		stTextID():iFontCRC(0),iTextCRC(0),dwColor(-1),dwBkColor(0),iFontSize(0),dwFlag(0),
			dwCount(0),iw(0),ih(0),iExpSize(0),pExp(0),pTex(NULL),dwFrameColor(0)
		{
		}
	};

	struct stList:public stTextID
	{
		stList *Prev;
		stList *Next;

		stList():Prev(NULL),Next(NULL)
		{
		}
	};

	struct SFontLib 
	{
		int	width,height;  // 单位宽度和高度，这里的宽度指一个汉字的宽度
		LOGFONT	sLogFont;  // 字体属性结构
	};	

private:
	stList* ListAdd(stTextID& textID);
	stList* ListFind(stTextID& textID);
	void ListDel(stList *pList);
	void ListMoveHead(stList *pList);
	void ListWash(DWORD dwCount,DWORD dwDelta);
	void ListClear(void);

	BOOL GetTextTex(stTextID& textID,LPCTSTR str);
	BOOL GetTextTexGDI(stTextID& textID,LPCTSTR str);
	BOOL GetTextTexFreeType(stTextID& textID,LPCTSTR str);
	BOOL GetTextTexFNT(stTextID& textID,LPCTSTR str);
	BOOL GetWordPixel(stTextID& textID,WORD wTemp,int& iw,WORD* pwBuf,WORD wColor);

protected:
	BYTE		m_vAsc12[FONT_SONGTI_12_ASC_SIZE];
	WORD		m_vAsc14[FONT_SONGTI_14_ASC_SIZE];
	BYTE		m_vFnt12[FONT_SONGTI_12_FNT_SIZE];
	BYTE		m_vFnt14[FONT_SONGTI_14_FNT_SIZE];

	WORD*       m_vFace;
	int         m_iFace;
	int         m_iCurFontType;
	//CFreeType	m_FreeType;

	stList*		m_pListHead;
	stList*		m_pListEnd;

	CFixPool	m_FixPool16;
	CFixPool	m_FixPool32;
	CFixPool	m_FixPool64;

	// GDI 字体
	HDC         m_hDC;			// 窗口DC
	HFONT       m_hFont;		// 字体句柄
	BITMAPINFO  m_sBmi;			// 字体图块结构
	int			m_iLastGUIFont;	// 上一次使用的GDI字体
	int         m_iLastFontSize;// 上一次使用的GDI字体的大小
	SFontLib    m_sFontLib[FONT_NUM];

public:
	CFontManager(void);
	~CFontManager(void);

	// 设置字体类型,宋体:FONT_SONGTI, eFont_Type类型之一
	BOOL	SetFont(int iFont = FONT_SONGTI);

	// 取得字符串像素长度
	int		GetLen(LPCTSTR p0,LPCTSTR p1 = NULL,int iFontSize = 12);

	//[xialin 2008/08/19]add-->dwFrameColor 支持彩色边框
	// 绘制文字，其中dwFlag可以是(DTF_UnderLine|DTF_Bold|DTF_BlackFrame)的任意组合,dwBkColor表示文字的背景色
	void	DrawText(RECT* pRect,LPCTSTR str,DWORD dwColor, DWORD dwFrameColor, int iFontSize = 12,DWORD dwFlag = 0,DWORD dwBkColor = 0);

	// 绘制文字，其中dwFlag可以是(DTF_UnderLine|DTF_Bold|DTF_BlackFrame)的任意组合,dwBkColor表示文字的背景色,可以拉伸，旋转
	void    DrawTextFX(int x,int y,LPCTSTR str,DWORD dwColor,int iFontSize = 12,DWORD dwFlag = 0,float fScaleX = 1.0,float fScaleY = 1.0,float fRotY = 0,DWORD dwBackColor = 0);

	// 绘制部分文字
	void	DrawTextPart(int x,int y,LPCTSTR str,DWORD dwColor,int iOffH,int iFontSize = 12,DWORD dwFlag = 0,DWORD dwBkColor = 0);

	// 绘制滚动文字
	// iOff：像素偏移
	// iDisArea：区域间的像素间隔
	// iDisLine：每行间的象素间隔
	int		DrawTextScroll(int x,int y,LPCTSTR str,DWORD dwColor,int iw,int ih,UINT iOff = 0,int iDisArea = 16,int iDisLine = 0,int iFontSize = 12,DWORD dwFlag = 0,DWORD dwBkColor = 0);
};

extern CFontManager g_Font;