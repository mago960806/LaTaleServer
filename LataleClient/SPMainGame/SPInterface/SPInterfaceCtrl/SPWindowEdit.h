// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2005-01-07 오전 10:26:19
//***************************************************************************



#pragma once


class SPWindow;
class SPWindowEdit : public SPWindow
{
public :	
	SPWindowEdit( INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);
	~SPWindowEdit();

	// default
	virtual void Process( float fTime );
	virtual void Render	( float fTime );
	virtual void Hide(bool bSendServer = true);		//[2005/11/15] - Option Window에만 적용
	virtual void RefreshRelationCoord();	

	// focus
	void SetFocus();
	void KillFocus();
	void SetPassword( bool bPassword = true ) { m_bPassword = bPassword; }

	// text
	virtual void SetWindowText(const char* pStr);
	void SetLimitText( int nLength );
	void SetNULL();
	void SetWindowFormatText( const WCHAR* szText, ... );	
	virtual const char* GetWindowText();

	// 숫자모드
	void SetNumber( bool bNumber = true ) { m_bNumber = bNumber; }
	void SetMinValue( INT64 iwMinValue )	{ m_iwMinValue = iwMinValue; }	
	void SetMaxValue( INT64 iwMaxValue )	{ m_iwMaxValue = iwMaxValue; }	
	void SetUseIME(bool bEnable)		{ m_bEnableIME = bEnable; }

	bool SetImageNormalHead	(const char* szImage, int iXSrc, int iYSrc, int iX2Src, int iY2Src);
	bool SetImageNormalBody	(const char* szImage, int iXSrc, int iYSrc, int iX2Src, int iY2Src);
	bool SetImageNormalTail	(const char* szImage, int iXSrc, int iYSrc, int iX2Src, int iY2Src);

	bool SetImageFocusHead	(const char* szImage, int iXSrc, int iYSrc, int iX2Src, int iY2Src);
	bool SetImageFocusBody	(const char* szImage, int iXSrc, int iYSrc, int iX2Src, int iY2Src);
	bool SetImageFocusTail	(const char* szImage, int iXSrc, int iYSrc, int iX2Src, int iY2Src);

	bool SetImageDeco (const char* szImage, int iXSrc, int iYSrc, int iX2Src, int iY2Src);
	
	void SetMargin(int iX = 0, int iY = 0);

	//void SetScaleSrc(int iCenterX, int iFocusCenterX, int iCenterWidth, int iRightX, int iFocusRightX);	

	void ClearStringBuffer();
	
	
private:
	void ReposOwnImage();
	bool ProcessString(OUT WCHAR* pwszBuf = NULL);

protected :

	SPIMESSAGE_OBJ_MAP_DECLARE()
	int OnSelFocus		( WPARAM wParam, LPARAM lParam);
	int OnKillFocus		( WPARAM wParam, LPARAM lParam);
	int OnCursorLDown	( WPARAM wParam, LPARAM lParam);
	int OnSetCaretPos	( WPARAM wParam, LPARAM lParam);

	int OnWheelUp		( WPARAM wParam, LPARAM lParam);		//[2005/12/20]
	int OnWheelDown		( WPARAM wParam, LPARAM lParam);		//[2005/12/20]
	int OnKeyDown		( WPARAM wParam, LPARAM lParam);		//  [12/27/2005]
	int OnCursorOut		( WPARAM wParam, LPARAM lParam);
	int OnCursorMove	( WPARAM wParam, LPARAM lParam);
	int OnKeyReturn		( WPARAM wParam, LPARAM lParam);


protected:

	std::wstring	m_wstrWindowTextBuf;
	std::wstring    m_wstrWindowText;
	CHAR			m_szWindowText[256];
	
	bool			m_bPassword;	// 암호모드
	bool			m_bNumber;		// 숫자모드

	INT64			m_iwMinValue;	// 숫자 모드일때 가질 수 있는 최소의 수
	INT64			m_iwMaxValue;	// 숫자 모드일때 가질 수 있는 최대의 수

	int				m_iLimitText;
	
	bool			m_bTwinkleCursor;
	bool			m_bSelect;
	float 			m_fPrevTime;

	SIZE			m_sizeCaretLen;
	RECT			m_rcWindowRect;
	RECT			m_rcCaretRect;
	RECT			m_rcCompositionCaret;

	SPTexture*		m_pNormalHead;
	SPTexture*		m_pNormalBody;
	SPTexture*		m_pNormalTail;
	SPTexture*		m_pFocusHead;	
	SPTexture*		m_pFocusBody;	
	SPTexture*		m_pFocusTail;
	SPTexture*		m_pImageDeco;

	RECT			m_rcCalcDestHead;		
	RECT			m_rcCalcDestBody;		
	RECT			m_rcCalcDestTail;		
	RECT			m_rcCalcDestDeco;
	
	RECT			m_rcSrcHead;
	RECT			m_rcSrcBody;
	RECT			m_rcSrcTail;

	RECT			m_rcSrcHeadFocus;
	RECT			m_rcSrcBodyFocus;
	RECT			m_rcSrcTailFocus;

	RECT			m_rcSrcDeco;

	RECT			m_rcTextRect;
	int				m_iMarginX;
	int				m_iMarginY;

	bool			m_bEnableIME;
	

};







