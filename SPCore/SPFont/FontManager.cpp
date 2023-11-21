//#include "TextureD3D.h"
//#include "GraphicD3D.h"
//#include "TexManager.h"

#include <D3D9.h>
#include <d3dx9.h>
#include "FontManager.h"
#include <vector>
#include <string>
#include "DXUtil.h"
#include "sptexture.h"
#include "crc32.h"
#include "atlconv.h"

using namespace std;
//#pragma comment(lib, "FreeType.lib")

//#include "Global/Crc32.h"
//#include "Global/StringUtil.h"

#define	FONT_PERPIXEL			2		// A4R4G4B4格式的字体纹理
#define FONT_A4_MASK            0xF000

#define POOL_SIZE_16			(128 * 16 * FONT_PERPIXEL)
#define POOL_SIZE_32			(128 * 32 * FONT_PERPIXEL)
#define POOL_SIZE_64			(128 * 64 * FONT_PERPIXEL)

#define NML_POOL_SIZE			1024*1024*5

char s_Buf[1024*1024*5];
extern DWORD            g_RenderFrame;

#include "../../LataleClient/lgclientlog.h"

WORD Conv_16Color(DWORD dwColor)
{
	WORD wColor = 0;
	wColor |= (dwColor & 0xF0000000) >> 16;
	wColor |= (dwColor & 0x00F00000) >> 12;
	wColor |= (dwColor & 0x0000F000) >> 8;
	wColor |= (dwColor & 0x000000F0) >> 4;

	return wColor;
}

CFontManager g_Font;

CFontManager::CFontManager(void):m_FixPool16(POOL_SIZE_16,96),m_FixPool32(POOL_SIZE_32,32),m_FixPool64(POOL_SIZE_64,16)
{
	char path[256];

	//为GDI字体准备
	m_iLastGUIFont = FONT_SONGTI;
	m_iLastFontSize = 16;
	m_hFont = NULL;
	m_hDC = CreateCompatibleDC(NULL);
	SetTextColor(m_hDC,RGB(255,255,255));
	SetBkColor(m_hDC,0);
	SetTextAlign(m_hDC,TA_TOP);

	ZeroMemory(&m_sBmi.bmiHeader, sizeof(BITMAPINFOHEADER));
	m_sBmi.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	m_sBmi.bmiHeader.biPlanes		= 1;
	m_sBmi.bmiHeader.biBitCount		= 16;
	m_sBmi.bmiHeader.biCompression	= BI_RGB;

	for(int i = FONT_GDI_XINSONG;i < FONT_NUM;i ++)
	{
		m_sFontLib[i].height	= 18; 
		m_sFontLib[i].width		= m_sFontLib[i].height / 2;
		ZeroMemory(&m_sFontLib[i].sLogFont,sizeof(m_sFontLib[i].sLogFont));
		m_sFontLib[i].sLogFont.lfHeight			= - m_sFontLib[i].height;
		m_sFontLib[i].sLogFont.lfWeight			= FW_NORMAL;
		m_sFontLib[i].sLogFont.lfCharSet		= DEFAULT_CHARSET;
		m_sFontLib[i].sLogFont.lfPitchAndFamily	= FIXED_PITCH;
	}

	strcpy(m_sFontLib[FONT_GDI_XINSONG].sLogFont.lfFaceName,	"新宋体");
	strcpy(m_sFontLib[FONT_GDI_KAITI].sLogFont.lfFaceName,		"楷体_GB2312");
	strcpy(m_sFontLib[FONT_GDI_HEITI].sLogFont.lfFaceName,		"黑体");
	strcpy(m_sFontLib[FONT_GDI_FANGSONG].sLogFont.lfFaceName,	"仿宋_GB2312");
	strcpy(m_sFontLib[FONT_GDI_SONG_XIE].sLogFont.lfFaceName,	"新宋体");
	strcpy(m_sFontLib[FONT_GDI_SONG_CU].sLogFont.lfFaceName,	"新宋体");
	strcpy(m_sFontLib[FONT_GDI_YOUYUAN].sLogFont.lfFaceName,	"幼圆");
	strcpy(m_sFontLib[FONT_GDI_LISHU].sLogFont.lfFaceName,		"隶书");
	strcpy(m_sFontLib[FONT_GDI_SHUTI].sLogFont.lfFaceName,		"方正舒体");
	strcpy(m_sFontLib[FONT_GDI_TAOTI].sLogFont.lfFaceName,		"方正姚体");
	strcpy(m_sFontLib[FONT_GDI_CAIYUN].sLogFont.lfFaceName,		"华文彩云");
	strcpy(m_sFontLib[FONT_GDI_XIHEI].sLogFont.lfFaceName,		"华文细黑");
	strcpy(m_sFontLib[FONT_GDI_XINWEI].sLogFont.lfFaceName,		"华文新魏");
	strcpy(m_sFontLib[FONT_GDI_HANGKAI].sLogFont.lfFaceName,	"华文行楷");
	strcpy(m_sFontLib[FONT_GDI_ZHONGSONG].sLogFont.lfFaceName,	"华文中宋");

	//两种特殊字体处理
	m_sFontLib[FONT_GDI_SONG_XIE].sLogFont.lfItalic = true;
	m_sFontLib[FONT_GDI_SONG_CU].sLogFont.lfWeight	= FW_BOLD;

	//载入12像素ASCII点阵
	sprintf(path,"asc.fnt","");
	FILE *fp = fopen(path,"rb");
	if(fp)
	{
		fread(m_vAsc12,FONT_SONGTI_12_ASC_SIZE,1,fp);
		fclose(fp);
	}
	else
	{
		memset(m_vAsc12,0,FONT_SONGTI_12_ASC_SIZE);
	}

	//12像素GBK汉字字符
	sprintf(path,"gbk12.fnt","");
	fp = fopen(path,"rb");
	if(fp)
	{
		fread(m_vFnt12,FONT_SONGTI_12_FNT_SIZE,1,fp);
		fclose(fp);
	}
	else
	{
		memset(m_vFnt12,0,FONT_SONGTI_12_FNT_SIZE);
	}

	// 14像素ASCII字符
	sprintf(path,"ASC14.fnt","");
	fp = fopen(path,"rb");
	if(fp)
	{
		fread(m_vAsc14,FONT_SONGTI_14_ASC_SIZE,1,fp);
		fclose(fp);
	}
	else
	{
		memset(m_vAsc14,0,FONT_SONGTI_14_ASC_SIZE);
	}

	// 14像素汉字字符
	sprintf(path,"gbk14.fnt","");
	fp = fopen(path,"rb");
	if(fp)
	{
		fread(m_vFnt14,FONT_SONGTI_14_FNT_SIZE,1,fp);
		fclose(fp);
	}
	else
	{
		memset(m_vFnt14,0,FONT_SONGTI_14_FNT_SIZE);
	}

	m_pListHead	= NULL;
	m_pListEnd	= NULL;
	m_iCurFontType = FONT_SONGTI;

	//char path[128] = {0};

	////载入ASCII点阵
	//sprintf(path,"asc.fnt");
	//FILE *fp = fopen(path,"rb");
	//if(fp)
	//{
	//	fread(m_vAsc,FONT12_SIZE,1,fp);
	//	fclose(fp);
	//}
	//else
	//{
	//	memset(m_vAsc,0,FONT12_SIZE);
	//}

	////载入笑脸符号
	//sprintf(path,"%sfont/face.fnt",g_strDataDir);
	//fp = fopen(path,"rb");
	//if(fp)
	//{
	//	fread(&m_iFace,sizeof(WORD),1,fp);
	//	if(m_iFace > 100) //异常数据
	//	{
	//		m_iFace = 0;
	//		m_vFace = NULL;
	//		fclose(fp);
	//	}
	//	int iSize = m_iFace * 12 * 12;
	//	m_vFace = new WORD[iSize];
	//	memset(m_vFace,0,sizeof(WORD)*iSize);
	//	fread(m_vFace,sizeof(WORD),iSize,fp);
	//	fclose(fp);
	//}
	//else
	//{
	//	m_iFace = 0;
	//	m_vFace = NULL;
	//}

	//载入TrueType字体
	//m_FreeType.AddFont("simsun.ttc");
	//m_FreeType.AddFont("simli.ttf");
}


CFontManager::~CFontManager(void)
{
	SAFE_DELETE_ARRAY(m_vFace);
	ListClear();
}

BOOL CFontManager::SetFont(int iFont)
{
	if(iFont < 0 || iFont >= FONT_NUM)
		return FALSE;

	m_iCurFontType = iFont;
	//	m_FreeType.SetFont(m_iCurFontType);

	return TRUE;
}

CFontManager::stList* CFontManager::ListAdd(stTextID& textID)
{
	// 使用淘汰
	ListWash(textID.dwCount,400);

	stList *pList = new stList;
	if(pList)
	{
		memcpy(pList,&textID,sizeof(stTextID));

		if(m_pListHead == NULL)
			m_pListHead = m_pListEnd = pList;
		else
		{
			pList->Next			= m_pListHead;
			m_pListHead->Prev	= pList;

			m_pListHead			= pList;
		}
	}
	return pList;
}

CFontManager::stList* CFontManager::ListFind(stTextID& textID)
{
	stList* pList = m_pListHead;
	while(pList)
	{
		if(memcmp(pList,&textID,24) == 0)
			return pList;
		pList = pList->Next;
	}
	return pList;
}

void CFontManager::ListDel(stList *pList)
{
	if(pList == m_pListHead)
		m_pListHead = pList->Next;
	if(pList == m_pListEnd)
		m_pListEnd = pList->Prev;

	if(pList->Prev)
		pList->Prev->Next = pList->Next;
	if(pList->Next)
		pList->Next->Prev = pList->Prev;

	//if(pList->pTex)
	//	pList->pTex->ReleaseFontTex();

	SAFE_DELETE(pList->pTex);
	SAFE_DELETE(pList->pExp);

	SAFE_DELETE(pList);
}


void CFontManager::ListMoveHead(stList *pList)
{
	pList->dwCount = g_RenderFrame;

	if(pList == m_pListHead)
		return;

	if(pList == m_pListEnd)
		m_pListEnd = pList->Prev;

	if(pList->Prev)
		pList->Prev->Next = pList->Next;
	if(pList->Next)
		pList->Next->Prev = pList->Prev;

	pList->Prev			= NULL;
	pList->Next			= m_pListHead;
	m_pListHead->Prev	= pList;
	m_pListHead			= pList;
}

void CFontManager::ListWash(DWORD dwCount,DWORD dwDelta)
{
	if(m_pListEnd && m_pListEnd->dwCount >= 0 && (int)(dwCount - m_pListEnd->dwCount) >= (int)dwDelta)
		ListDel(m_pListEnd);
}

void CFontManager::ListClear(void)
{
	stList*	pTemp = NULL;		
	stList* pList = m_pListHead;
	while(pList)
	{
		pTemp = pList->Next;
		//		if(pList->pTex)
		//			pList->pTex->ReleaseFontTex();
		SAFE_DELETE(pList->pTex);
		SAFE_DELETE(pList->pExp);
		SAFE_DELETE(pList);
		pList = pTemp;
	}

	m_pListHead = m_pListEnd = NULL;
}

/*
BOOL CFontManager::GetWordPixel(stTextID& textID,WORD wTemp,int& iw,WORD* pwBuf,WORD wColor)
{
if(textID.iFontSize == 12 && wTemp < 0x0080)
{
int count = 9 * (wTemp - 32) * 8;
for(int h = 0;h < 12;h++)
{
for(int w = 0;w < 6;w++)
{
if(m_vAsc12[count/8] & 1<<(7 - (count % 8)))
pwBuf[h * textID.iw + iw + w] = wColor;
count++;
}
}
iw += 6;
}
else
{

BitmapInfo	*pBitmapinfo = NULL;
try
{
if(m_FreeType.GetWord(&pBitmapinfo,wTemp,textID.iFontSize) == FALSE || pBitmapinfo == NULL)
{
iw += textID.iFontSize;
return FALSE;
}
}
catch(...)
{
char tt[256];
sprintf(tt,"%d,%d,%d",wTemp,textID.iFontSize,iw);
g_Log.Log(tt);
return FALSE;
}

if(pBitmapinfo->x + pBitmapinfo->w > textID.iw)
pBitmapinfo->x = textID.iw - pBitmapinfo->w;
if(pBitmapinfo->y + pBitmapinfo->h > textID.ih)
pBitmapinfo->y = textID.ih - pBitmapinfo->h;

if(pBitmapinfo->x < 0)
pBitmapinfo->x = 0;
if(pBitmapinfo->y < 0)
pBitmapinfo->y = 0;

//对全角和半角标点符号特殊处理
if(wTemp < 0x0080 || (wTemp >= 0xFF00 && wTemp <= 0xFFEF) || (wTemp >= 0x3000 && wTemp <= 0x303F))
{
if(pBitmapinfo->h < textID.ih)
pBitmapinfo->y = textID.iFontSize - pBitmapinfo->h-1;
}

int x,y;
WORD wAlpha;
for(int h = 0;h < pBitmapinfo->h;h++)
{
for(int w = 0;w < pBitmapinfo->w;w++)
{
if(pBitmapinfo->bMono)
{
if(pBitmapinfo->pBuf[h * pBitmapinfo->pitch + w / 8] & (1<<~((w&0x00000007)|0xFFFFFFF8)))
wAlpha = 0xFFFF;
else
wAlpha = 0x0FFF;
}
else
{
wAlpha = pBitmapinfo->pBuf[h * pBitmapinfo->pitch + w];
wAlpha = (wAlpha<< 8) | 0x0FFF;
}

x = iw + pBitmapinfo->x + w;
y = pBitmapinfo->y + h;
if(x <= textID.iw && y <= textID.ih)
pwBuf[y * textID.iw + x] = wAlpha & wColor;
}
}
if(wTemp < 128)
iw += textID.iFontSize/2;
else
iw += textID.iFontSize;
}

return TRUE;
}
*/

BOOL CFontManager::GetWordPixel(stTextID& textID,WORD wTemp,int& iw,WORD* pwBuf,WORD wColor)
{
	return false;

	if(textID.iFontSize == 12 && wTemp < 0x0080)
	{
		int count = 9 * (wTemp - 32) * 8;
		for(int h = 0;h < 12;h++)
		{
			for(int w = 0;w < 6;w++)
			{
				if(m_vAsc12[count/8] & 1<<(7 - (count % 8)))
					pwBuf[h * textID.iw + iw + w] = wColor;
				count++;
			}
		}
		iw += 6;
	}
	else
	{
		BitmapInfo	*pBitmapinfo = NULL;


		//if(m_FreeType.GetWord(&pBitmapinfo,wTemp,textID.iFontSize) == FALSE || pBitmapinfo == NULL)
		//{
		//	iw += textID.iFontSize;
		//	return FALSE;
		//}

		if(pBitmapinfo->x + pBitmapinfo->w > textID.iw)
			pBitmapinfo->x = textID.iw - pBitmapinfo->w;
		if(pBitmapinfo->y + pBitmapinfo->h > textID.ih)
			pBitmapinfo->y = textID.ih - pBitmapinfo->h;

		if(pBitmapinfo->x < 0)
			pBitmapinfo->x = 0;
		if(pBitmapinfo->y < 0)
			pBitmapinfo->y = 0;

		//对全角和半角标点符号特殊处理
		if(wTemp < 0x0080 || (wTemp >= 0xFF00 && wTemp <= 0xFFEF) || (wTemp >= 0x3000 && wTemp <= 0x303F))
		{
			if(pBitmapinfo->h < textID.ih)
				pBitmapinfo->y = textID.iFontSize - pBitmapinfo->h-1;
		}

		int x,y;
		WORD wAlpha;
		for(int h = 0;h < pBitmapinfo->h;h++)
		{
			for(int w = 0;w < pBitmapinfo->w;w++)
			{
				if(pBitmapinfo->bMono)
				{
					if(pBitmapinfo->pBuf[h * pBitmapinfo->pitch + w / 8] & (1<<~((w&0x00000007)|0xFFFFFFF8)))
						wAlpha = 0xFFFF;
					else
						wAlpha = 0x0FFF;
				}
				else
				{
					wAlpha = pBitmapinfo->pBuf[h * pBitmapinfo->pitch + w];
					wAlpha = (wAlpha<<8)|0x0FFF;
				}

				x = iw + pBitmapinfo->x + w;
				y = pBitmapinfo->y + h;
				if(x <= textID.iw && y <= textID.ih)
					pwBuf[y * textID.iw + x] = wAlpha&wColor;
			}
		}
		//iw += pBitmapinfo->x + pBitmapinfo->w;
		if(wTemp < 128)
			iw += textID.iFontSize/2;
		else
			iw += textID.iFontSize;
	}

	return TRUE;
}

BOOL CFontManager::GetTextTex(stTextID& textID,LPCTSTR str)
{
	int iFont = textID.iFontCRC;

	if(textID.iFontSize > 14)
	{
		textID.iFontCRC = FONT_GDI_XINSONG;
		iFont = FONT_GDI_XINSONG;
	}

	if(iFont >= FONT_SONGTI && (textID.iFontSize == FONT_SMALL || textID.iFontSize == FONT_MIDDLE))
		return GetTextTexFNT(textID,str);
	//else if(iFont >= FONT_SONGTI && iFont < FONT_FREETYPE_NUM)
	//	return GetTextTexFreeType(textID,str);
	else if(iFont >= FONT_GDI_XINSONG && iFont < FONT_NUM)
		return GetTextTexGDI(textID,str);
	else 
		return false;

}

BOOL CFontManager::GetTextTexFNT(stTextID& textID,LPCTSTR str)
{
	BYTE c1,c2;
	int len = (int)strlen(str);

	textID.dwCount	= g_RenderFrame;
	textID.iw		= GetLen(str,NULL,textID.iFontSize) + 1;//+ 2;
	textID.ih		= textID.iFontSize + 1;
	textID.iExpSize	= 0;

	int    cw       = textID.iFontSize / 2;   //单个字符的宽度
	int    ch       = textID.iFontSize;       //单个字符的高度
	int  clen		= cw * ch;                //ASCII码字符所占字节数
	int  chzlen		= ((cw*2+7) / 8) * ch;    //汉字所占字节数

	//for(int i = 0;i < len;i++)
	//{
	//	c1 = (BYTE)str[i];
	//	c2 = (BYTE)str[i + 1];
	//	if(c1 == DICON_BEGIN && c2 < 200)
	//	{
	//		textID.iExpSize++;
	//		i++;
	//	}
	//	else if(c1 >= 0x81 && c1 <= 0xFE && c2 >= 0x40 && c2 <= 0xFE)
	//	{
	//		i++;
	//	}
	//}

	if(textID.iw * 64 * FONT_PERPIXEL > NML_POOL_SIZE)
		return FALSE;

	memset(s_Buf,0,textID.iw * 64 * FONT_PERPIXEL);
	WORD *pwBuf = (WORD *)s_Buf;

	//if(textID.iExpSize > 0)
	//{
	//	textID.pExp = new stExpression[textID.iExpSize];
	//	memset(textID.pExp,0,sizeof(stExpression) * textID.iExpSize);
	//}

	// 填充A4R4G4B4像素数据
	int		iw = 1,iCount = 0;
	WORD	wColor = (WORD)(0xF000|((textID.dwColor & 0x00F00000)>>12)|((textID.dwColor & 0x0000F000)>>8)|((textID.dwColor & 0x000000F0)>>4));
	for(int i = 0;i < len;i++)
	{
		c1 = (BYTE)str[i];
		c2 = (BYTE)str[i + 1];
		//if(c1 == DICON_BEGIN && c2 < 200) //大的图标
		//{
		//	textID.pExp[iCount].iPos	= iw;
		//	textID.pExp[iCount].iIdex	= c2;
		//	iCount++;
		//	iw += 16;
		//	i++;
		//}
		//else if(c1== 0xF8 && c2>=0xA1 && c2<=0xFE) //笑脸符号
		//{
		//	if(m_vFace != NULL && m_iFace > 0)
		//	{
		//		c2 -= 0xA1;
		//		if( c2 >= m_iFace)
		//			c2 = m_iFace-1;

		//		WORD* pwFace = &m_vFace[c2*144];

		//		for(int h = 0;h < 12;h++)
		//		{
		//			for(int w = 0;w < 12;w++)
		//			{
		//				if((*pwFace) & 0xF000)
		//					pwBuf[h * textID.iw + iw + w] = *pwFace;
		//				pwFace++;
		//			}
		//		}
		//	}
		//	iw += 12;
		//	i++;
		//}
		//else 
		if( c1>=0x81 && c2>=0x40 && c1<=0xFE && c2<=0xFE )// 中文
		{
			BYTE* pChar = &m_vFnt12[((c1-129)*191+(c2-64))*chzlen];
			if(textID.iFontSize == FONT_MIDDLE)
				pChar = &m_vFnt14[((c1-129)*191+(c2-64))*chzlen];

			for(int y = 1;y < ch;y++)
			{
				int j = y*textID.iw + 1 + i*cw;

				for(int ii = 0;ii< 8;ii++)
				{
					BYTE cMask = (0x80 >> ii);
					if(*pChar & cMask)
						pwBuf[j+ ii]=wColor;
				}
				pChar++;

				for(int ii = 8;ii< cw * 2;ii++)
				{
					BYTE cMask = (0x80 >> (ii - 8));
					if(*pChar & cMask)
						pwBuf[j+ ii]=wColor;
				}
				pChar++;
			}

			iw += textID.iFontSize;
			i++;
		}
		else// ASCII
		{		
			if( c1<32 || c1>126 ) c1 = 32;

			if(textID.iFontSize == FONT_SMALL)
			{
				int count = 9 * (c1 - 32) * 8;
				for(int h = 0;h < 12;h++)
				{
					for(int w = 0;w < 6;w++)
					{
						if(m_vAsc12[count/8] & 1<<(7 - (count % 8)))
							pwBuf[h * textID.iw + iw + w] = wColor;
						count++;
					}
				}
			}
			else
			{
				WORD* pAsc = &m_vAsc14[(c1-32)*clen];

				// 英文字符，每个点都是16bit为A4R4G4B4格式，可以直接memcpy
				for(int h = 1;h < ch;h++) 
				{
					int j = h*textID.iw + 1 + i*cw;
					for(int w = 0;w < cw;w++)
					{
						if(pAsc[w])
							pwBuf[j+w]=wColor;
					}
					pAsc += cw;
				}
			}

			iw += cw;
		}
	}

	//// 下划线
	//if(textID.dwFlag & DTF_UnderLine)
	//{
	//	for(int i = 0;i < textID.iw;i++)
	//		pwBuf[(textID.ih - 1) * textID.iw + i] = wColor;
	//}

	// 粗体
	if(textID.dwFlag & DTF_Bold)
	{
		WORD temp[2048];
		for(int j = 0;j < textID.ih;j++)
		{
			memcpy(temp,&pwBuf[j * textID.iw],textID.iw * 2);
			for(int i = 1;i < textID.iw;i++)
			{
				if(!(pwBuf[j * textID.iw + i] & 0xF000) && (pwBuf[j * textID.iw + i - 1] & 0xF000))
					temp[i] = pwBuf[j * textID.iw + i - 1];
			}
			memcpy(&pwBuf[j * textID.iw],temp,textID.iw * 2);
		}
	}

	// 黑边框
	if(textID.dwFlag & DTF_BlackFrame)
	{
		//[xialin 2008/09/06]edit
		//WORD wFrameColor = 0xF000;
		WORD wFrameColor = (WORD)(((textID.dwFrameColor & 0xF0000000)>>16)|((textID.dwFrameColor & 0x00F00000)>>12)|((textID.dwFrameColor & 0x0000F000)>>8)|((textID.dwFrameColor & 0x000000F0)>>4));
		for(int j = 0;j < textID.ih;j++)
		{
			for(int i = 0;i < textID.iw;i++)
			{
				if((pwBuf[j * textID.iw + i] & 0xF000) && (pwBuf[j * textID.iw + i] | 0xF000) == wColor)
				{
					if(j > 0 && !(pwBuf[(j - 1) * textID.iw + i] & 0xF000))
						pwBuf[(j - 1) * textID.iw + i] = wFrameColor;

					if(i > 0 && !(pwBuf[j * textID.iw + (i - 1)] & 0xF000))
						pwBuf[j * textID.iw + (i - 1)] = wFrameColor;

					if(i < (textID.iw - 1) && !(pwBuf[j * textID.iw + (i + 1)] & 0xF000))
						pwBuf[j * textID.iw + (i + 1)] = wFrameColor;

					if(j < (textID.ih - 1) && !(pwBuf[(j + 1) * textID.iw + i] & 0xF000))
						pwBuf[(j + 1) * textID.iw + i] = wFrameColor;
				}
			}
		}
	}

	//// 背景色
	//if(textID.dwBkColor)
	//{
	//	WORD wBackColor = (WORD)(((textID.dwBkColor & 0xF0000000)>>16)|((textID.dwBkColor & 0x00F00000)>>12)|((textID.dwBkColor & 0x0000F000)>>8)|((textID.dwBkColor & 0x000000F0)>>4));
	//	for(int j = 0;j < textID.ih;j++)
	//	{
	//		for(int i = 0;i < textID.iw;i++)
	//		{
	//			if(!(pwBuf[j * textID.iw + i] & 0xF000))
	//				pwBuf[j * textID.iw + i] = wBackColor;
	//		}
	//	}
	//}

	textID.pTex = new SPTexture;
	if(!textID.pTex)
	{
		SAFE_DELETE(textID.pExp);
		return FALSE;
	}

	if(textID.pTex->CreateFontTexture(textID.iw,textID.ih,s_Buf) == FALSE)
	{
		SAFE_DELETE(textID.pTex);
		SAFE_DELETE(textID.pExp);
		return FALSE;
	}

	return TRUE;

	/*
	BYTE c1,c2;
	int len = (int)strlen(str);

	textID.dwCount	= g_RenderFrame;
	textID.iw		= GetLen(str,NULL,textID.iFontSize) + 2;
	textID.ih		= textID.iFontSize;
	textID.iExpSize	= 0;

	for(int i = 0;i < len;i++)
	{
	c1 = (BYTE)str[i];
	c2 = (BYTE)str[i + 1];
	if(c1 == DICON_BEGIN && c2 < 200)
	{
	textID.iExpSize++;
	i++;
	}
	else if(c1 >= 0x81 && c1 <= 0xFE && c2 >= 0x40 && c2 <= 0xFE)
	{
	i++;
	}
	}

	if(textID.iw * 64 * FONT_PERPIXEL > NML_POOL_SIZE)
	return FALSE;

	memset(s_Buf,0,textID.iw * 64 * FONT_PERPIXEL);
	WORD *pwBuf = (WORD *)s_Buf;

	if(textID.iExpSize > 0)
	{
	textID.pExp = new stExpression[textID.iExpSize];
	memset(textID.pExp,0,sizeof(stExpression) * textID.iExpSize);
	}

	// 填充A8R8G8B8像素数据
	WORD	wTemp;
	int		iw = 0,iCount = 0;
	WORD	wColor = Conv_16Color(textID.dwColor) | FONT_A4_MASK;

	for(int i = 0;i < len;i++)
	{
	c1 = (BYTE)str[i];
	c2 = (BYTE)str[i + 1];
	//if(c1 == DICON_BEGIN && c2 < 200) //大的图标
	//{
	//	textID.pExp[iCount].iPos	= iw;
	//	textID.pExp[iCount].iIdex	= c2;
	//	iCount++;
	//	iw += 16;
	//	i++;
	//}
	//else if(c1== 0xF8 && c2>=0xA1 && c2<=0xFE) //笑脸符号
	//{
	//	if(m_vFace != NULL && m_iFace > 0)
	//	{
	//		c2 -= 0xA1;
	//		if( c2 >= m_iFace)
	//			c2 = m_iFace-1;

	//		WORD* pwFace = &m_vFace[c2*144];

	//		for(int h = 0;h < 12;h++)
	//		{
	//			for(int w = 0;w < 12;w++)
	//			{
	//				if((*pwFace) & FONT_A4_MASK)
	//					pwBuf[h * textID.iw + iw + w] = *pwFace;
	//				pwFace++;
	//			}
	//		}
	//	}
	//	iw += 12;
	//	i++;
	//}
	if(c1 >= 0x81 && c1 <= 0xFE && c2 >= 0x40 && c2 <= 0xFE)
	{
	MultiByteToWideChar(CP_ACP,0,str + i,2,(LPWSTR)&wTemp,1);
	GetWordPixel(textID,wTemp,iw,pwBuf,wColor);
	i++;
	}
	else
	{
	if(c1 > 32 && c1 < 128)
	{
	MultiByteToWideChar(CP_ACP,0,str + i,1,(LPWSTR)&wTemp,1);
	GetWordPixel(textID,wTemp,iw,pwBuf,wColor);
	}
	else
	{
	iw += textID.iFontSize / 2;
	}
	}
	}

	// 下划线
	if(textID.dwFlag & DTF_UnderLine)
	{
	for(int i = 0;i < textID.iw;i++)
	pwBuf[(textID.ih - 1) * textID.iw + i] = wColor;
	}

	// 粗体
	if(textID.dwFlag & DTF_Bold)
	{
	WORD temp[4096];
	for(int j = 0;j < textID.ih;j++)
	{
	memcpy(temp,&pwBuf[j * textID.iw],textID.iw * FONT_PERPIXEL);
	for(int i = 1;i < textID.iw;i++)
	{
	if(!(pwBuf[j * textID.iw + i] & FONT_A4_MASK) && (pwBuf[j * textID.iw + i - 1] & FONT_A4_MASK))
	temp[i] = pwBuf[j * textID.iw + i - 1];
	}
	memcpy(&pwBuf[j * textID.iw],temp,textID.iw * FONT_PERPIXEL);
	}
	}

	// 黑边框
	if(textID.dwFlag & DTF_BlackFrame)
	{
	WORD wFrameColor = FONT_A4_MASK;
	for(int j = 0;j < textID.ih;j++)
	{
	for(int i = 0;i < textID.iw;i++)
	{
	if((pwBuf[j * textID.iw + i] & FONT_A4_MASK) && (pwBuf[j * textID.iw + i] | FONT_A4_MASK) == wColor)
	{
	if(j > 0 && !(pwBuf[(j - 1) * textID.iw + i] & FONT_A4_MASK))
	pwBuf[(j - 1) * textID.iw + i] = wFrameColor;

	if(i > 0 && !(pwBuf[j * textID.iw + (i - 1)] & FONT_A4_MASK))
	pwBuf[j * textID.iw + (i - 1)] = wFrameColor;

	if(i < (textID.iw - 1) && !(pwBuf[j * textID.iw + (i + 1)] & FONT_A4_MASK))
	pwBuf[j * textID.iw + (i + 1)] = wFrameColor;

	if(j < (textID.ih - 1) && !(pwBuf[(j + 1) * textID.iw + i] & FONT_A4_MASK))
	pwBuf[(j + 1) * textID.iw + i] = wFrameColor;
	}
	}
	}
	}

	// 背景色
	if(textID.dwBkColor)
	{
	WORD wBackColor = Conv_16Color(textID.dwBkColor);

	for(int j = 0;j < textID.ih;j++)
	{
	for(int i = 0;i < textID.iw;i++)
	{
	if(!(pwBuf[j * textID.iw + i] & FONT_A4_MASK))
	pwBuf[j * textID.iw + i] = wBackColor;
	}
	}
	}

	textID.pTex = new SPTexture;
	if(!textID.pTex)
	{
	SAFE_DELETE(textID.pExp);
	return FALSE;
	}

	if(textID.pTex->CreateFontTexture(textID.iw,textID.ih,s_Buf) == FALSE)
	{
	SAFE_DELETE(textID.pTex);
	SAFE_DELETE(textID.pExp);
	return FALSE;
	}

	return TRUE;

	*/
}
BOOL CFontManager::GetTextTexFreeType(stTextID& textID,LPCTSTR str)
{
	BYTE c1,c2;
	int len = (int)strlen(str);

	textID.dwCount	= g_RenderFrame;
	textID.iw		= GetLen(str,NULL,textID.iFontSize) + 1;//+ 2;
	textID.ih		= textID.iFontSize;
	textID.iExpSize	= 0;

	for(int i = 0;i < len;i++)
	{
		c1 = (BYTE)str[i];
		c2 = (BYTE)str[i + 1];
		if(c1 == DICON_BEGIN && c2 < 200)
		{
			textID.iExpSize++;
			i++;
		}
		else if(c1 >= 0x81 && c1 <= 0xFE && c2 >= 0x40 && c2 <= 0xFE)
		{
			i++;
		}
	}

	if(textID.iw * 64 * FONT_PERPIXEL > NML_POOL_SIZE)
		return FALSE;

	memset(s_Buf,0,textID.iw * 64 * FONT_PERPIXEL);
	WORD *pwBuf = (WORD *)s_Buf;

	if(textID.iExpSize > 0)
	{
		textID.pExp = new stExpression[textID.iExpSize];
		memset(textID.pExp,0,sizeof(stExpression) * textID.iExpSize);
	}

	// 填充A4R4G4B4像素数据
	WORD	wTemp;
	int		iw = 1,iCount = 0;
	WORD	wColor = (WORD)(0xF000|((textID.dwColor & 0x00F00000)>>12)|((textID.dwColor & 0x0000F000)>>8)|((textID.dwColor & 0x000000F0)>>4));
	for(int i = 0;i < len;i++)
	{
		c1 = (BYTE)str[i];
		c2 = (BYTE)str[i + 1];
		if(c1 == DICON_BEGIN && c2 < 200) //大的图标
		{
			textID.pExp[iCount].iPos	= iw;
			textID.pExp[iCount].iIdex	= c2;
			iCount++;
			iw += 16;
			i++;
		}
		else if(c1== 0xF8 && c2>=0xA1 && c2<=0xFE) //笑脸符号
		{
			if(m_vFace != NULL && m_iFace > 0)
			{
				c2 -= 0xA1;
				if( c2 >= m_iFace)
					c2 = m_iFace-1;

				WORD* pwFace = &m_vFace[c2*144];

				for(int h = 0;h < 12;h++)
				{
					for(int w = 0;w < 12;w++)
					{
						if((*pwFace) & 0xF000)
							pwBuf[h * textID.iw + iw + w] = *pwFace;
						pwFace++;
					}
				}
			}
			iw += 12;
			i++;
		}
		else if(c1 >= 0x81 && c1 <= 0xFE && c2 >= 0x40 && c2 <= 0xFE)
		{
			MultiByteToWideChar(CP_ACP,0,str + i,2,(LPWSTR)&wTemp,1);
			GetWordPixel(textID,wTemp,iw,pwBuf,wColor);
			i++;
		}
		else
		{
			if(c1 > 32 && c1 < 128)
			{
				MultiByteToWideChar(CP_ACP,0,str + i,1,(LPWSTR)&wTemp,1);
				GetWordPixel(textID,wTemp,iw,pwBuf,wColor);
			}
			else
			{
				iw += textID.iFontSize / 2;
			}
		}
	}

	// 下划线
	if(textID.dwFlag & DTF_UnderLine)
	{
		for(int i = 0;i < textID.iw;i++)
			pwBuf[(textID.ih - 1) * textID.iw + i] = wColor;
	}

	// 粗体
	if(textID.dwFlag & DTF_Bold)
	{
		WORD temp[2048];
		for(int j = 0;j < textID.ih;j++)
		{
			memcpy(temp,&pwBuf[j * textID.iw],textID.iw * 2);
			for(int i = 1;i < textID.iw;i++)
			{
				if(!(pwBuf[j * textID.iw + i] & 0xF000) && (pwBuf[j * textID.iw + i - 1] & 0xF000))
					temp[i] = pwBuf[j * textID.iw + i - 1];
			}
			memcpy(&pwBuf[j * textID.iw],temp,textID.iw * 2);
		}
	}

	// 黑边框
	if(textID.dwFlag & DTF_BlackFrame)
	{
		WORD wFrameColor = 0xF000;
		for(int j = 0;j < textID.ih;j++)
		{
			for(int i = 0;i < textID.iw;i++)
			{
				if((pwBuf[j * textID.iw + i] & 0xF000) && (pwBuf[j * textID.iw + i] | 0xF000) == wColor)
				{
					if(j > 0 && !(pwBuf[(j - 1) * textID.iw + i] & 0xF000))
						pwBuf[(j - 1) * textID.iw + i] = wFrameColor;

					if(i > 0 && !(pwBuf[j * textID.iw + (i - 1)] & 0xF000))
						pwBuf[j * textID.iw + (i - 1)] = wFrameColor;

					if(i < (textID.iw - 1) && !(pwBuf[j * textID.iw + (i + 1)] & 0xF000))
						pwBuf[j * textID.iw + (i + 1)] = wFrameColor;

					if(j < (textID.ih - 1) && !(pwBuf[(j + 1) * textID.iw + i] & 0xF000))
						pwBuf[(j + 1) * textID.iw + i] = wFrameColor;
				}
			}
		}
	}

	// 背景色
	if(textID.dwBkColor)
	{
		WORD wBackColor = (WORD)(((textID.dwBkColor & 0xF0000000)>>16)|((textID.dwBkColor & 0x00F00000)>>12)|((textID.dwBkColor & 0x0000F000)>>8)|((textID.dwBkColor & 0x000000F0)>>4));
		for(int j = 0;j < textID.ih;j++)
		{
			for(int i = 0;i < textID.iw;i++)
			{
				if(!(pwBuf[j * textID.iw + i] & 0xF000))
					pwBuf[j * textID.iw + i] = wBackColor;
			}
		}
	}

	textID.pTex = new SPTexture;
	if(!textID.pTex)
	{
		SAFE_DELETE(textID.pExp);
		return FALSE;
	}

	if(textID.pTex->CreateFontTexture(textID.iw,textID.ih,s_Buf) == FALSE)
	{
		SAFE_DELETE(textID.pTex);
		SAFE_DELETE(textID.pExp);
		return FALSE;
	}

	return TRUE;
}

// 使用GDI函数来显示文字，效率比较差，但是可以使用任意的windows字体
// 虽然理论上用GDI字体速度应该比用点阵字库慢，但是实际测试发现速度差别很小，这说明Cache结构做的是成功的，在极端情况下，速度能提高九倍左右!
BOOL CFontManager::GetTextTexGDI(stTextID& textID,LPCTSTR str)
{

	try
	{
		int i,j,y,x,w,h;
		WORD *pBuf, *pBits;
		WORD wColor,wBackColor;
		HBITMAP hBmp;
		RECT rc;
		bool bNewBuf;

		int iLen = (int)strlen(str);

		textID.dwCount	= g_RenderFrame;
		textID.iExpSize	= 0;


		// 当前字体是否改变，如果改变要产生新的字体
		if(textID.iFontCRC != m_iLastGUIFont || textID.iFontSize != m_iLastFontSize)
		{
			if(m_hFont) DeleteObject((HGDIOBJ)m_hFont);
			m_sFontLib[textID.iFontCRC].sLogFont.lfHeight = -textID.iFontSize;
			m_hFont = CreateFontIndirect( &(m_sFontLib[textID.iFontCRC].sLogFont) );
			m_iLastGUIFont = textID.iFontCRC;
			m_iLastFontSize = textID.iFontSize;
			if(m_hFont == NULL)
				return GetTextTex(textID,str);
		}
		SelectObject(m_hDC,m_hFont);


		//当前字体为非固定长度，必须用GDI函数来得到此行文字的宽度
		SIZE sSize;
		GetTextExtentPoint(m_hDC,str,iLen,&sSize);
		w = sSize.cx;
		w = min(w,1024);
		if( w % 2 ) w++;
		h	= textID.iFontSize;
		wColor		= (WORD)( ((textID.dwColor>>16) & 0xF000) | ((textID.dwColor>>12) & 0x0F00) | ((textID.dwColor>>8) & 0x00F0) | ((textID.dwColor>>4) & 0x0000F) );
		wBackColor	= (WORD)( ((textID.dwBkColor>>16) & 0xF000) | ((textID.dwBkColor>>12) & 0x0F00) | ((textID.dwBkColor>>8) & 0x00F0) | ((textID.dwBkColor>>4) & 0x0000F) );

		int iNeedBufLen = (w + 2) * h * 2;
		if(iNeedBufLen <= NML_POOL_SIZE)
		{
			memset(s_Buf,0,iNeedBufLen);
			pBuf = (WORD *)s_Buf;
			bNewBuf = false;
		}
		else
		{
			pBuf = new WORD[(w+2)*h];
			if(!pBuf)
				return FALSE;

			bNewBuf = true;
		}

		// 创建HBITMAP
		m_sBmi.bmiHeader.biWidth	= w;
		m_sBmi.bmiHeader.biHeight	= h;
		hBmp = CreateDIBSection(m_hDC, &m_sBmi, DIB_RGB_COLORS,(void **)&pBits, NULL, 0);
		SelectObject(m_hDC,hBmp);

		// 输出文字
		rc.left	  = 0;
		rc.top	  = 0;
		rc.right  = w;
		rc.bottom = h;
		FillRect(m_hDC,&rc,(HBRUSH)GetStockObject(BLACK_BRUSH));
		TextOut(m_hDC,0,0,str,iLen);

		// 多出来的边框清零
		w += 2;
		h += 2;
		j = (h-1) * w;
		for(x = 0;x < w; x++)
		{
			pBuf[x]		= wBackColor;
			pBuf[j+x]	= wBackColor;
		}
		for(y = 0;y < h;y ++)
		{
			pBuf[y*w]		= wBackColor;
			pBuf[y*w+(w-1)]	= wBackColor;
		}
		w-=2;
		h-=2;

		// 把文字从HBITMAP复制到CTexture中
		for(y=0;y<h;y++) 
		{
			i = (h-y) * w;
			for(x=0;x<w;x++)
			{
				if( pBits[i+x] ) 
					pBuf[(y+1)*(w+2)+(x+1)] = wColor; 
				else 
					pBuf[(y+1)*(w+2)+(x+1)] = wBackColor;
			}
		}
		w += 2;
		h += 2;

		// 加边框
		if(textID.dwFlag & DTF_BlackFrame)
		{
			//[xialin 2008/09/06]edit
			//WORD wFrameColor = 0xF000;
			WORD wFrameColor = (WORD)(((textID.dwFrameColor & 0xF0000000)>>16)|((textID.dwFrameColor & 0x00F00000)>>12)|((textID.dwFrameColor & 0x0000F000)>>8)|((textID.dwFrameColor & 0x000000F0)>>4));
			for(y=1;y<(h-1);y++)
			{
				j = y*w;
				for(x=1;x<(w-1);x++)
				{
					if( pBuf[j+x] == wColor )
					{
						if( pBuf[j+x-w] == wBackColor )
							pBuf[j+x-w] = wFrameColor;
						if( pBuf[j+x+w] == wBackColor )
							pBuf[j+x+w] = wFrameColor;
						if( pBuf[j+x-1] == wBackColor )
							pBuf[j+x-1] = wFrameColor;
						if( pBuf[j+x+1] == wBackColor )
							pBuf[j+x+1] = wFrameColor;
					}
				}
			}
		}


		textID.pTex = new SPTexture;
		if(!textID.pTex)
		{
			SAFE_DELETE(textID.pExp);
			return FALSE;
		}

		if(textID.pTex->CreateFontTexture(w,h,s_Buf) == FALSE)
		{
			SAFE_DELETE(textID.pTex);
			SAFE_DELETE(textID.pExp);
			return FALSE;
		}

		DeleteObject(hBmp);
		if(bNewBuf) 
			SAFE_DELETE_ARRAY(pBuf);

	}
	catch(...)
	{
		return true;
	}
	return true;

}



// 取得当前字体下字符串的宽度
int CFontManager::GetLen(LPCTSTR p0,LPCTSTR p1,int iFontSize)
{
	BYTE c1,c2;
	WORD wTemp = 0;
	BitmapInfo	*pBitmapinfo = NULL;

	try
	{
		int i = 0;
		while(*p0)
		{
			if(p1 && p0 >= p1)
				break;

			c1 = (BYTE)*p0;
			c2 = (BYTE)*(p0 + 1);

			if(c1==0xF8 && c2>=0xA1 && c2<=0xFE )
			{
				i += 12;
				p0++;
			}
			else if(c1 >= 0x81 && c1 <= 0xFE && c2 >= 0x40 && c2 <= 0xFE)
			{
				i += iFontSize;
				p0++;
			}
			else 
			{
				i += iFontSize / 2;
			}
			p0++;
		}

		return i;
	}
	catch(...)
	{
		return 0;
	}
}

// 一般的文字绘制
void CFontManager::DrawText(RECT* pRect,LPCTSTR str,DWORD dwColor, DWORD dwFrameColor, int iFontSize,DWORD dwFlag,DWORD dwBkColor)
{
	//	if(g_Gfx.IsNoDraw() || !str || strlen(str) == 0 || iFontSize > 64 || iFontSize % 2)
	//		return;

	USES_CONVERSION;

	//[xialin 2008/09/05] add
	int  iTextTop, iTop = pRect->top;
	std::wstring  wstrText = A2W(str);
	std::vector<wstring>  wstrTextList;
	wstrTextList.push_back(wstrText);


	int xx = pRect->left;

	if(!str || strlen(str) == 0 || iFontSize > 64 || iFontSize % 2)
		return;

	if(dwFlag & DTF_Center)
	{
		//x -= GetLen(str,NULL,iFontSize) / 2;
		xx += (pRect->right - pRect->left - GetLen(str,NULL,iFontSize))/2;
	}
	else if (dwFlag & DTF_Right)  //[xialin 2008/08/19]add
	{
		xx += (pRect->right - pRect->left - GetLen(str,NULL,iFontSize));
	}
	//[xialin 2008/09/05] 支持换行 >>>
	if (dwFlag & DT_WORDBREAK)
	{
		wstrTextList.clear();
		int  iOffset = 0, isize = 0;
		int  iOffset_old = iOffset;

		for ( iOffset=0; iOffset < static_cast<int>(wstrText.size()); iOffset++ )
		{
			isize = iOffset - iOffset_old;
			LPCTSTR strText = W2A(wstrText.substr(iOffset_old, isize).c_str());
			int istrLeght = GetLen(strText,NULL,iFontSize);
			if ( istrLeght >= (pRect->right - pRect->left) )
			{
				wstrTextList.push_back(wstrText.substr(iOffset_old, isize));
				iOffset_old = iOffset;
			}
		}
		isize = iOffset - iOffset_old;
		if ( isize > 0 ) 
		{
			wstrTextList.push_back(wstrText.substr(iOffset_old, isize));
		}
	}
	if ( wstrTextList.size() > 1 )
	{
		xx = pRect->left;
	}


	for ( int i=0; i < static_cast<int>(wstrTextList.size()); i++ )
	{
		LPCTSTR strText = W2A(wstrTextList[i].c_str());
		iTextTop = iTop + (i * (iFontSize + 2));

		if( i == wstrTextList.size()-1 && (dwFlag & DTF_Center) )
		{
			xx = pRect->left;
			xx += (pRect->right - pRect->left - GetLen(strText,NULL,iFontSize))/2;
		}

		stTextID text;

		text.iFontCRC	= m_iCurFontType;
		text.iTextCRC	= g_CRC32.Get_CRC((BYTE *)strText,(ULONG)strlen(strText));
		text.dwColor	= dwColor;
		text.iFontSize	= iFontSize;
		text.dwFlag		= dwFlag;
		text.dwBkColor	= dwBkColor;
		text.dwFrameColor = dwFrameColor;

		stList *pList = ListFind(text);
		if(pList)
			ListMoveHead(pList);
		else if(GetTextTex(text,strText))
			pList = ListAdd(text);

		if(pList && pList->pTex)
		{							
			//dwColor |= 0x00FFFFFF;

			//if(dwFlag & DTF_Shadow)//带阴影
			//{
			//	pList->pTex->SetColor(0xFF000000);
			//	pList->pTex->RenderTexture(xx,pRect->top+1);
			//	pList->pTex->SetColor(0xFFFFFFFF);
			//}

			//		g_Gfx.DrawTextureNL(x,y,pList->pTex);

			pList->pTex->RenderTexture(xx, iTextTop);

			//		if(pList->iExpSize > 0 && pList->pExp)
			//		{
			//			for(int i = 0; i < pList->iExpSize; i++)
			//			{
			////				LPTexture pTex = g_TexMgr.GetTex(PACKAGE_CURSOR,100 + pList->pExp[i].iIdex);
			////				g_Gfx.DrawTextureNL(x + pList->pExp[i].iPos + 1,y + (iFontSize - 16) / 2,pTex,dwColor);
			//			}
			//		}
		}
	}
	//[xialin 2008/09/05] 支持换行 <<<
}

// 绘制部分文字
void CFontManager::DrawTextPart(int x,int y,LPCTSTR str,DWORD dwColor,int iOffH,int iFontSize,DWORD dwFlag,DWORD dwBkColor)
{
	//	if(g_Gfx.IsNoDraw() || !str || strlen(str) == 0 || iFontSize > 64 || iFontSize % 2)
	//		return;

	if(dwFlag & DTF_Center)
	{
		x -= GetLen(str,NULL,iFontSize) / 2;
	}

	stTextID text;

	text.iFontCRC	= m_iCurFontType;
	text.iTextCRC	= g_CRC32.Get_CRC((BYTE *)str,(ULONG)strlen(str));
	text.dwColor	= dwColor;
	text.iFontSize	= iFontSize;
	text.dwFlag		= dwFlag;
	text.dwBkColor	= dwBkColor;

	stList *pList = ListFind(text);
	if(pList)
		ListMoveHead(pList);
	else if(GetTextTex(text,str))
		pList = ListAdd(text);

	if(pList && pList->pTex)
	{
		RECT	rc;
		int		iMaxH = (iFontSize < 16) ? 16 : iFontSize;

		rc.left		= 0;
		rc.right	= pList->iw;

		if(iOffH >= iMaxH || iOffH <= -iMaxH)
			iOffH = 0;

		if(iOffH >= 0)
		{
			rc.top		= iOffH + (iFontSize - iMaxH) / 2;
			rc.bottom	= pList->ih;
		}
		else
		{
			rc.top		= 0;
			rc.bottom	= iMaxH + iOffH + (iFontSize - iMaxH) / 2;
		}

		if(rc.top < 0)
			rc.top = 0;
		if(rc.bottom > pList->ih)
			rc.bottom = pList->ih;

		dwColor |= 0x00FFFFFF;

		//		g_Gfx.DrawTextureFX(x,y,pList->pTex,dwColor,RM_ALPHA,&rc);
		if(pList->iExpSize > 0 && pList->pExp)
		{
			rc.left		= 0;
			rc.right	= 16;
			if(iOffH >= 0)
			{
				rc.top		= iOffH + (16 - iMaxH) / 2;
				rc.bottom	= 16;
			}
			else
			{
				rc.top		= 0;
				rc.bottom	= iMaxH + iOffH + (16 - iMaxH) / 2;
			}

			if(rc.top < 0)
				rc.top = 0;
			if(rc.bottom > 16)
				rc.bottom = 16;

			for(int i = 0; i < pList->iExpSize; i++)
			{
				//				LPTexture pTex = g_TexMgr.GetTex(PACKAGE_CURSOR,100 + pList->pExp[i].iIdex);
				//				g_Gfx.DrawTextureFX(x + pList->pExp[i].iPos + 1,y + (iFontSize - 16) / 2,pTex,dwColor,RM_ALPHA,&rc);
			}
		}
	}
}
// 绘制文字，其中dwFlag可以是(DTF_UnderLine|DTF_Bold|DTF_BlackFrame)的任意组合,dwBkColor表示文字的背景色,可以拉伸，旋转
void CFontManager::DrawTextFX(int x,int y,LPCTSTR str,DWORD dwColor,int iFontSize,DWORD dwFlag,float fScaleX,float fScaleY,float fRotY,DWORD dwBackColor)
{
	//	if(g_Gfx.IsNoDraw() || !str || strlen(str) == 0 || iFontSize > 64 || iFontSize % 2)
	//		return;

	if(dwFlag & DTF_Center)
	{
		x -= GetLen(str,NULL,iFontSize) / 2;
	}

	stTextID text;

	text.iFontCRC	= m_iCurFontType;
	text.iTextCRC	= g_CRC32.Get_CRC((BYTE *)str,(ULONG)strlen(str));
	text.dwColor	= dwColor;
	text.iFontSize	= iFontSize;
	text.dwFlag		= dwFlag;
	text.dwBkColor	= dwBackColor;

	stList *pList = ListFind(text);
	if(pList)
		ListMoveHead(pList);
	else if(GetTextTex(text,str))
		pList = ListAdd(text);

	if(pList && pList->pTex)
	{
		dwColor |= 0x00FFFFFF;

		if(dwFlag & DTF_Shadow)//带阴影
		{
			//			g_Gfx.DrawTextureFX(x+1,y+1,pList->pTex,0xFF000000,RM_ALPHA,NULL,&POS(fScaleX,fScaleY),0.0f,fRotY,0.0f);
		}

		//		g_Gfx.DrawTextureFX(x,y,pList->pTex,dwColor,RM_ALPHA,NULL,&POS(fScaleX,fScaleY),0.0F,fRotY,0.0f);

		if(pList->iExpSize > 0 && pList->pExp)
		{
			for(int i = 0; i < pList->iExpSize; i++)
			{
				//				LPTexture pTex = g_TexMgr.GetTex(PACKAGE_CURSOR,100 + pList->pExp[i].iIdex);
				//				g_Gfx.DrawTextureFX(x + pList->pExp[i].iPos + 1,y + (iFontSize - 16) / 2,pTex,dwColor,
				//					RM_ALPHA,NULL,&POS(fScaleX,fScaleY),0.0f,fRotY,0.0f);
			}
		}
	}
}
// 绘制滚动文字
// iOff：像素偏移
// iDisArea：区域间的像素间隔
// iDisLine：每行间的象素间隔
int CFontManager::DrawTextScroll(int x,int y,LPCTSTR str,DWORD dwColor,int iw,int ih,UINT iOff,int iDisArea,int iDisLine,int iFontSize,DWORD dwFlag,DWORD dwBkColor)
{
	//	if(g_Gfx.IsNoDraw() || !str || iw == 0 || ih <= 0)
	//		return 0;

	int iRet = 0; 

	std::vector<std::string>	vStr;

	// 分解出每一行
	int		i,j;
	int		w = 0;
	const char*	p	= NULL;
	const char*	p0	= str;
	const char*	p1	= str;

	while(true)
	{
		p = CharNext(p1);
		i = (int)(p - p1);

		// 是否是字符串结束
		if(i > 2 || i <= 0)
			goto ADDTEXT;

		// 是否是硬换行
		if(i == 1 && (*p1 == 0x0D || *p1 == 0x0A))
		{
			if(*p1 == 0x0D && *(p1 + 1) == 0x0A)
				p++;
			goto ADDTEXT;
		}

		if(i == 2 && (BYTE)(*p1) == DICON_BEGIN)		// 表情动画字符特殊
			j = w + i * 8;
		else
			j = w + i * iFontSize / 2;

		// 是否需要软换行
		if(j > iw)
		{
			p		= p1;
			goto ADDTEXT;
		}
		else
			w = j; 

		p1	= p;
		continue;

ADDTEXT:
		if(p1 > p0)
			vStr.push_back(std::string(p0,p1 - p0));

		if(i > 2 || i <= 0)		// 字符串结束
			break;

		p1		= p;
		p0		= p;
		w		= 0;
	}

	// 绘制每一行
	int	iMaxH	= (iFontSize < 16) ? 16 : iFontSize;
	int iLineH	= iMaxH + iDisLine;
	int	h		= (int)vStr.size() * iLineH;			// 文字的实际高度

	if(h <= ih)				// 不需要滚动
	{
		for(UINT c = 0; c < vStr.size(); c++)
			;//DrawText(x,y + c * iLineH,vStr[c].c_str(),dwColor,iFontSize,dwFlag,dwBkColor);
	}
	else					// 需要滚动
	{
		int	idx,oy,off = iOff % (h + iDisArea);		// 相对偏移

		if(off >= h)
		{
			idx = 0;
			oy	= h + iDisArea - off;
		}
		else
		{
			idx	= off / iLineH;
			oy  = -(off % iLineH);
		}

		while(true)
		{
			if((oy + iLineH) > ih)		// 底部的一行
			{
				if((oy + iMaxH) > ih)
					DrawTextPart(x,y + oy,vStr[idx].c_str(),dwColor,ih - oy - iMaxH,iFontSize,dwFlag,dwBkColor);
				else
					DrawTextPart(x,y + oy,vStr[idx].c_str(),dwColor,0,iFontSize,dwFlag,dwBkColor);
				break;
			}
			else if(oy <= 0)					// 顶端的一行
			{
				if((off % iLineH) < iMaxH)
					DrawTextPart(x,y + oy,vStr[idx].c_str(),dwColor,off % iLineH,iFontSize,dwFlag,dwBkColor);
			}
			else								// 中间的一行
				DrawTextPart(x,y + oy,vStr[idx].c_str(),dwColor,0,iFontSize,dwFlag,dwBkColor);

			idx++;
			oy += iLineH;
			if(idx >= (int)vStr.size())
			{
				oy += iDisArea;
				idx = 0;
			}

			if(oy >= ih)
				break;
		}
		if(idx == 0)
			iRet = 0;
		else
			iRet = off;
	}

	return iRet;
}