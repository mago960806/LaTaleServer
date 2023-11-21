// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-08-08 오전 10:26:19
//***************************************************************************
/*
// 2007.08.22 WND_TYPE_NINEBASE 추가 [duragon]
## 사용법
	m_pLevelTooltip	=	new SPWindowStatic( WIID_GUILD_STUS_LEVEL_TOOLTIP , 157 , 213 , 207 , 256 , m_pGuildStatusWnd );
	m_pLevelTooltip->SetWindowType(WND_TYPE_NINEBASE) ;
	m_pLevelTooltip->SetImageNine( "DATA/INTERFACE/CONCEPT/UI105.PNG", 482, 301, 4, 1) ;	// SrcX, SrcY, Width, Margin
## 유의 사항
	WND_TYPE_LEFTBASE, WND_TYPE_TOPBASE 에서는 사용 불가 -> 각각의 이미지의 크기를 갖고 있기 때문에
	배경으로 사용시 Static이 배경 전체를 가리기 때문에 하위 Window는 Child로 제작 되어야 함. -> Find시 유의
*/



#pragma once

class SPWindow;
class SPWindowStatic :	public SPWindow {
public:
	SPWindowStatic(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent);
	~SPWindowStatic();

	virtual void Init();
	virtual void InitFromXML(SPWindow_Base* rec_wnd_AttrMap);
	virtual void Clean();	
	virtual void Process(float fTime);	
	virtual void Render(float fTime);
	virtual void RenderText();

	virtual void SetWindowText(const char* pStr = NULL);
	virtual bool SetImage(const char* szImage, int iSrcX=0, int iSrcY=0);
	virtual void SetImage(SPTexture* pImage, int iSrcX= 0, int iSrcY=0);	
	virtual void SetImagePos(int iSrcX, int iSrcY);
	virtual void RefreshRelationCoord();

	virtual void SetSrcSize(int isx, int isy);

	int GetTextCount()		{ return (int)m_vWindowTextList.size(); }
	int GetTextPixelHeight();

	void SetColor( DWORD rgba );
	DWORD GetColor();

	void SetImageScaleX( float fScaleX );
	void SetImageScaleY( float fScaleY );
	float GetImageScaleX();
	float GetIamgeScaleY();

	void SetCenterRender( bool bCenter );
	bool GetCenterRender();

	// [6/12/2006] WND_TYPE_TOPBASE, WND_TYPE_LEFTBASE
	void ReposOwnImage();
	bool SetImageRect(SPTexture * pTexture, int iXSrc, int iYSrc, int iX2Src, int iY2Src); //[xialin 2008/07/08]add
	bool SetImageNormalHead	(const char* szImage, int iXSrc, int iYSrc, int iX2Src, int iY2Src);
	bool SetImageNormalBody	(const char* szImage, int iXSrc, int iYSrc, int iX2Src, int iY2Src);
	bool SetImageNormalTail	(const char* szImage, int iXSrc, int iYSrc, int iX2Src, int iY2Src);

	// [2008/08/13]Duragon : 9방향 이미지 Static 사용 설정 (WND_TYPE_NINEBASE)
	// iWidth 이미지 크기(가로,세로 동일), 마진값
	bool SetImageNine(const char* szImage, int iXSrc, int iYSrc, int iWidth, int iMargin);
	bool SetSourceRect(int iXSrc, int iYSrc, int iWidthSrc, int iHeightSrc, int iMargin) ;

	void SetTextAutoWidthMin( int iTextAutoWidthMin );		//	글자수에 따른 자동 너비 계산 최소값 [6/20/2008 AJJIYA]
	void SetTextAutoWidthMax( int iTextAutoWidthMax );		//	글자수에 따른 자동 너비 계산 최대값 [6/20/2008 AJJIYA]
	void CalTextAutoWidth();

	void DeleteLastTexture(); 
protected:
	
	SPIMESSAGE_OBJ_MAP_DECLARE()
	// {{ Message Map	
	int OnCursorEnter	( WPARAM, LPARAM );
	int OnCursorOut		( WPARAM, LPARAM );
	int OnSetImageIndex ( WPARAM wParam, LPARAM lParam);
	int OnImageClear	( WPARAM wParam, LPARAM lParam);
	int OnLButtonUp		( WPARAM wParam, LPARAM lParam);		//[2005/7/11] - jinhee
	int OnRButtonUp		( WPARAM wParam, LPARAM lParam);		//[2008/4/24]-jinhee
	int OnWheelUp		( WPARAM wParam, LPARAM lParam);		//[2005/9/12]
	int OnWheelDown		( WPARAM wParam, LPARAM lParam);		//[2005/9/12]
	// }}

protected:

	struct IMAGE_ATTR {
		SPTexture*	pImage;
		POINT		SrcPoint;
	};
	std::vector<IMAGE_ATTR> m_vpImageAttr;

	int m_iCurImage;

	std::vector< std::string > m_vWindowTextList;

	float	m_fImageScaleX;
	float	m_fImageScaleY;

	bool	m_bCenterRender;

	int		m_iSrcWidth ;
	int		m_iSrcHeight ;
	int		m_iSrcMargin ;

	SPTexture*		m_pNormalHead;
	SPTexture*		m_pNormalBody;
	SPTexture*		m_pNormalTail;

	RECT			m_rcCalcDestHead[3];		
	RECT			m_rcCalcDestBody[3];		
	RECT			m_rcCalcDestTail[3];

	RECT			m_rcSrcHead[3];
	RECT			m_rcSrcBody[3];
	RECT			m_rcSrcTail[3];

	//RECT			m_rcSrcHeadFocus;
	//RECT			m_rcSrcBodyFocus;
	//RECT			m_rcSrcTailFocus;

	int				m_iTextAutoWidthMin;
	int				m_iTextAutoWidthMax;
};
