// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-08-08 ¿ÀÀü 10:26:19
//***************************************************************************

#pragma once


class SPWindowSlider : public SPWindow {

public:
	SPWindowSlider(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent, WND_ID CtrlType = WNDID_CTRL_SLIDER_VERTICAL);
	virtual ~SPWindowSlider();

	virtual void Init();
	virtual void Clean();

	virtual void Process(float fTime);
	virtual void Render(float fTime);
	virtual void RefreshRelationCoord();
	
	void Refresh();
	void SetCurpage(int iPage = 0);			//[2005/7/6] - jinhee
	int	 GetCurPage();						//[2005/7/6] - jinhee
	void SetDrag( BOOL bDrag );
	void SetColor( DWORD rgba );
	DWORD GetColor();

	RECT GetHandleTarget();

	void SetImageHandle			( const char* pszImage, RECT rcSrc );
	void SetImageHandleHit		( const char* pszImage, RECT rcSrc );
	void SetImageHandleDisable	( const char* pszImage, RECT rcSrc );
	void SetImageHandlePush		( const char* pszImage, RECT rcSrc );
	void SetImagePageShadow		( const char* pszImage, RECT rcSrc );
	void SetImagePageNormal		( const char* pszImage, RECT rcSrc );
	void SetImagePageHit		( const char* pszImage, RECT rcSrc );
	void SetImagePageDisable	( const char* pszImage, RECT rcSrc );

	void SetImageHandleRect		( RECT RectEdge );
	void SetImageHandleHitRect	( RECT rcSrc );

	void SetWheelMouseUse( BOOL bUse );

	void SetEnable(bool bEnable); //  [11/8/2005]
	void SetHandlerRender(bool bRender);


protected:

	int CalcPos(int iX, int iY);
	bool CursorPageMove( bool bUp, bool bOff);

	SPIMESSAGE_OBJ_MAP_DECLARE()	
	int OnSetMaxValue	( WPARAM wParam = 0, LPARAM lParam = 0 );
	int OnSetCurValue	( WPARAM wParam = 0, LPARAM lParam = 0 );
	int OnRefresh		( WPARAM wParam = 0, LPARAM lParam = 0 );
	int OnLButtonDown	( WPARAM wParam = 0, LPARAM lParam = 0 );
	int OnLButtonUp		( WPARAM wParam = 0, LPARAM lParam = 0 );
	int OnPageUp		( WPARAM wParam = 0, LPARAM lParam = 0 );
	int OnPageDown		( WPARAM wParam = 0, LPARAM lParam = 0 );
	int OnScrollUp		( WPARAM wParam = 0, LPARAM lParam = 0 );
	int OnScrollDown	( WPARAM wParam = 0, LPARAM lParam = 0 );
	int OnWheelUp		( WPARAM wParam = 0, LPARAM lParam = 0 );
	int OnWheelDown		( WPARAM wParam = 0, LPARAM lParam = 0 );
	int OnSetPageSize	( WPARAM wParam = 0, LPARAM lParam = 0 );
	int OnCursorMove	( WPARAM wParam = 0, LPARAM lParam = 0 );
	int OnCursorEnter	( WPARAM wParam = 0, LPARAM lParam = 0 );
	int OnCursorOut		( WPARAM wParam = 0, LPARAM lParam = 0 );
	int OnKillFocus		( WPARAM wParam = 0, LPARAM lParam = 0 );
	int OnGetPage		( WPARAM wParam = 0, LPARAM lParam = 0 );
	int OnWndPosUpdate	( WPARAM wParam = 0, LPARAM lParam = 0 );

protected:
	
	SPTexture*		m_pImageHandle;
	SPTexture*		m_pImageHandleHit;
	SPTexture*		m_pImageHandleDisable;
	SPTexture*		m_pImageHandlePush;
	SPTexture*		m_pImagePageShadow;

	SPTexture*		m_pImagePageNormal	;
	SPTexture*		m_pImagePageHit		;
	SPTexture*		m_pImagePageDisable	;

	SPTexture**		m_ppImageActiveHandle	;	

	INT				m_iRangeValue;
	INT				m_iCurValue;	
	INT				m_iPageSize;
	bool			m_bPush;	

	RECT			m_DestRect;
	RECT			m_SrcRect;
	RECT			m_rcSrcHandleNormal;
	RECT			m_rcTarHandle;
	int				m_iHandleHalfSize;

	RECT			m_rcSrcHandlePush;
	RECT			m_rcSrcPageShadow;

	RECT			m_rcCalcTargetPageShadow;
	bool			m_bRenderPageShadow;

	BOOL			m_bOnlyDrag;
	RECT			m_rcSrcHandleHit;
	RECT			m_rcSrcHandleDisable;

	RECT			m_rcSrcPageNormal	;
	RECT			m_rcSrcPageHit		;	
	RECT			m_rcSrcPageDisable	;	

	RECT*			m_prcDestActive		;
	RECT*			m_prcActiveSrcHandle;
	RECT*			m_prcActiveSrcPage	;

	BOOL			m_bWheelMouseUse;	
	bool			m_bRenderHandle;

};




