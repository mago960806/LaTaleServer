#pragma once

#include "SPQuestAttr.h"

const int MAX_USER_SHOP_REQUIRE = 4;					/**< 최대 요구사항 <br> */
const int MAX_USER_SHOP_FRAME = 4;						/**< 최대 애니메이션 정보 <br> */

enum USER_SHOP_LEVEL {
	USER_SHOP_LEVEL_NULL	= 0,
	USER_SHOP_LEVEL_1		= 1,
	USER_SHOP_LEVEL_2		= 2,
	USER_SHOP_LEVEL_3		= 3,
	USER_SHOP_LEVEL_4		= 4,
	USER_SHOP_LEVEL_5		= 5,
	USER_SHOP_LEVEL_MAX		= 6,
};

struct SPSignBoard {
	std::string		m_strSource;						/**< 소스 파일 <br> */
	RECT			m_rtSrc;							/**< 소스 영역 <br> */
	POINT			m_ptDest;							/**< 출력 영역 <br> */
	POINT			m_ptNameDest;						/**< 상점 주인 이름 영역 <br> */
	POINT			m_ptTitleDest;						/**< 상점 명 영역 <br> */
	float			m_fDelay;							/**< 딜레이 <br> */
	
	SPTexture*		m_pTexture;							/**< 소스 텍스쳐 <br> */	
	int				m_iWidth;							/**< 이미지 넓이 <br> */
	int				m_iHeight;							/**< 이미지 높이 <br> */

	SPSignBoard() {
		m_pTexture = NULL; 
		Clear();
	}

	~SPSignBoard() {
		SAFE_RELEASE(m_pTexture);
		Clear();
	}

	void Clear() {
		m_strSource = "";
		m_strSource.clear();
		SetRect(&m_rtSrc, 0, 0, 0, 0);
		m_ptDest.x = 0;
		m_ptDest.y = 0;
		m_ptNameDest.x = 0;
		m_ptNameDest.y = 0;
		m_ptTitleDest.x = 0;
		m_ptTitleDest.y = 0;
		m_fDelay = 0.0f;

		m_iWidth = 0;
		m_iHeight = 0;
	}

	void SetSrcFile(std::string strSrcFile) {
		m_strSource = "";
		m_strSource.clear();
		m_strSource = strSrcFile;
		
		std::string strTextureFile = "";
		strTextureFile.clear();
		strTextureFile = "DATA/INTERFACE/CONCEPT/" + m_strSource;
		//g_pVideo->GetTextureMgr()->LoadTexture(strTextureFile.c_str(), &m_pTexture);
		g_pVideo->GetTextureMgr()->GetTexture(strTextureFile.c_str(), &m_pTexture,TEX_INTERFACE);
		
	}

	void SetSrcRect(int iLeft, int iTop, int iRight, int iBottom) {
		SetRect(&m_rtSrc, iLeft, iTop, iRight, iBottom);
		m_iWidth = iRight - iLeft ;
		m_iHeight = iBottom - iTop;
	}

	void SetDest(int iX, int iY) {
		m_ptDest.x = iX;
		m_ptDest.y = iY; 
	}

	void SetNameDeat(int iX, int iY) {
		m_ptNameDest.x = iX;
		m_ptNameDest.y = iY;
	}

	void SetTitleDest(int iX, int iY) {
		m_ptTitleDest.x = iX;
		m_ptTitleDest.y = iY;
	}
};



class SPUserShopAttr {
public:
	SPUserShopAttr();
	~SPUserShopAttr();

	void SetOverLapRect(int iLeft, int iTop, int iRight, int iBottom);
	void SetNameColor(int iRed, int iGreen, int iBlue, int iAlpha = 255);
	void SetTitleColor(int iRed, int iGreen, int iBlue, int iAlpha = 255);
	

protected:
	void Init();
	void Clear();

public:
	int				m_iID;								/**< LDT ID <br> */
	int				m_iType;							/**< 개인상점 레벨 <br> */
	RECT			m_rtOverLapRect;					/**< 중복 체크 영역 <br> */
	SPRequire		m_Require[MAX_USER_SHOP_REQUIRE];	/**< 개인상점 레벨 체크용 요구사항 <br> */
	int				m_iPage;							/**< 레벨별 개인상점 페이지 <br> 페이지 * 4 <br> */	
	
	int				m_iNameWidth;						/**< 상점 주인 이름 <br> */
	int				m_iNameHeight;						/**< 상점 주인 이름 <br> */	
	D3DXCOLOR		m_NameColor;						/**< 상점 주인 이름 <br> */	
	bool			m_bNameBold;						/**< 상점 주인 이름 <br> */	

	int				m_iTitleWidth;						/**< 상점 이름 <br> */
	int				m_iTitleHeight;						/**< 상점 이름 <br> */
	D3DXCOLOR		m_TitleColor;						/**< 상점 이름 <br> */
	bool			m_bTitleBold;						/**< 상점 이름 <br> */

	SPSignBoard		m_SignBoard[MAX_USER_SHOP_FRAME];	/**< 에니메이션? 프레임 정보 <br> */		
	int				m_iMaxFrame;						/**< 에니메이션 최대 프레임 정보 <br> */

	int				m_iMotion;							//!< shop motion	
};


