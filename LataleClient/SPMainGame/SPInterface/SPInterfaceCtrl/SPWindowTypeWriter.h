// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2006-05-25 ¿ÀÀü 10:26:19
//***************************************************************************

#pragma once


#define MAX_TYPE_LEN 512


class SPWindow;
class SPWindowTypeWriter :	public SPWindow {
public:
	SPWindowTypeWriter(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent);
	~SPWindowTypeWriter();

	virtual void Init();
	virtual void Clean();	
	virtual void Process(float fTime);	
	virtual void Render(float fTime);
	virtual void RenderText();

	virtual void SetWindowText(const char* pStr = NULL);

	int GetTextCount()		{ return (int)m_vwstrWindowText.size(); }
	int GetTextPixelHeight();

	void SetColor( DWORD rgba );
	DWORD GetColor();

	void SetImageScaleX( float fScaleX );
	void SetImageScaleY( float fScaleY );
	float GetImageScaleX();
	float GetIamgeScaleY();
	//bool SetImageBlind(const char* szImage, int iXSrc, int iYSrc, int iXLen, int iYLen);

	void SetCenterRender( bool bCenter );
	bool GetCenterRender();

protected:
	
	SPIMESSAGE_OBJ_MAP_DECLARE()
	// {{ Message Map	
	int OnCursorEnter	( WPARAM, LPARAM );
	int OnCursorOut		( WPARAM, LPARAM );
	int OnSetImageIndex ( WPARAM wParam, LPARAM lParam);
	int OnImageClear	( WPARAM wParam, LPARAM lParam);
	int OnLButtonUp		( WPARAM wParam, LPARAM lParam);		//[2005/7/11] - jinhee
	int OnWheelUp		( WPARAM wParam, LPARAM lParam);		//[2005/9/12]
	int OnWheelDown		( WPARAM wParam, LPARAM lParam);		//[2005/9/12]
	int OnSetMargin		( WPARAM wParam, LPARAM lParam);

	int OnTypeDelay		( WPARAM wParam, LPARAM lParam);
	int OnTypeClear		( WPARAM wParam, LPARAM lParam);
	int OnTypeStart		( WPARAM wParam, LPARAM lParam);
	int OnTypeEnd		( WPARAM wParam, LPARAM lParam);
	int OnTypeSkip		( WPARAM wParam, LPARAM lParam);
	int OnTypeSound		( WPARAM wParam, LPARAM lParam);

	int OnLButtonUP		( WPARAM, LPARAM );
	int OnLButtonDown	( WPARAM, LPARAM );
	// }}

protected:		

	float	m_fImageScaleX;
	float	m_fImageScaleY;

	bool	m_bCenterRender;

	bool	m_bTypeWrite;
	int		m_iCurTypeLen;
	RECT	m_rcWindowRect;
	int		m_iMargin;
	
	SPTexture* m_pImageBlind;
	RECT	m_rcBlindSrc;
	std::vector<RECT> m_vrcBlindTar;

	float	m_fTypeDelay;
	float	m_fAccmulateTime;	

	int		m_iCurTextLine;		// current line, 0 base
	int		m_iCurTextPos;		// current line base


	std::string		m_strTypeSound;

	std::wstring				m_wstrWindowText;
	std::vector<std::wstring>	m_vwstrWindowText;

};






