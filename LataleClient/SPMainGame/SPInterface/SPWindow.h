// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-08-08 오전 10:26:19
//***************************************************************************

#pragma once

#include "SPWindow_Base.h"

enum WINDOW_BASE {					//[2008/3/5]-jinhee
	WINDOW_BASE_TOP_LEFT,
	WINDOW_BASE_TOP,
	WINDOW_BASE_TOP_RIGHT,
	WINDOW_BASE_CENTER_LEFT,
	WINDOW_BASE_CENTER,
	WINDOW_BASE_CENTER_RIGHT,
	WINDOW_BASE_BOTTOM_LEFT,
	WINDOW_BASE_BOTTOM,
	WINDOW_BASE_BOTTOM_RIGHT,
	WINDOW_BASE_MAX,
};


class SPWindow {

public:
	enum REFRESHCHILD_METHOD
	{
		REFRESHCHILD_METHOD_NORMAL,
		REFRESHCHILD_METHOD_WITHOUTME
	};

	enum RENDERING_METHOD
	{
		RENDERING_METHOD_NORMAL  = 0,
		RENDERING_METHOD_FLIP_LR	,
		RENDERING_METHOD_FLIP_TB
	};

	SPWindow(WND_ID WndClassID, INSTANCE_ID InstanceID, SPWindow* pParent = NULL);
	SPWindow(WND_ID WndClassID, INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);	

	virtual ~SPWindow();

	virtual void Init();
	virtual void InitFromXML(SPWindow_Base* rec_wnd_AttrMap);		//- XML 전용로딩을 제공합니다.    [18/08/2008 W.Fairy]
	virtual void Clean();

	virtual void Process(float fTime);
	virtual void Render(float fTime);
	virtual void RenderReverse(float fTime);
			void RenderReverse(float fTime,int Reserved);			//- XML을 위한 렌더링 방식 추가.  [03/09/2008 W.Fairy]

	virtual void RenderTextDebug();
	virtual void RenderText();	

	virtual void Show(bool bWithChild = true);	
	virtual bool IsShow();	

	virtual void SetEnable(bool);
	virtual bool IsEnable();
	
	virtual void Hide(bool bSendServer = true);		//[2005/11/15] - Option Window에만 적용
	virtual void Close(); // 서브게임 체인지 할때만 쓰세요~~..

	virtual void SetWindowText(const char* pStr = NULL);
	virtual void SetToolTipStr(const char* pStr = NULL);
	virtual void SetFormat(DWORD dwFormat);	
	virtual void SetFontColor(D3DXCOLOR FontColor);	
	virtual void SetFontShadowColor(D3DXCOLOR FontColor);	

	// 기존 호환성때문에
	virtual void SetFontShadow(bool);
	virtual void SetFontEdge(bool);

	// 이걸로 쓰기 권장
	virtual void SetFontEffect(SPFONT_EFFECT FontEffect);
	virtual SPFONT_EFFECT GetFontEffect();


	void SetFont( FONT_ENUMERATE fontEnum );
	SIZE GetSizeText();	

	virtual int  PerformMouseMove		( int iX, int iY);
	virtual int  PerformMouseLDown		( int iX, int iY);
	virtual int  PerformMouseLUp		( int iX, int iY);
	virtual int  PerformMouseRDown		( int iX, int iY);
	virtual int  PerformMouseRUp		( int iX, int iY);
	virtual int  PerformMouseWheelUp	( int iDelta );
	virtual int  PerformMouseWheelDown	( int iDelta );

	INSTANCE_ID GetWidnowInstanceID();
	void SetWidnowInstanceID(INSTANCE_ID InstanceID);

	void SetWindowType			( WND_TYPE WndType );

	// Interface Message
	int	SPSendMessage			( SPIM Spim, WPARAM wParam = 0, LPARAM lParam = 0, SPWindow* pSender = NULL);
	int SPSendMessageToChild	( SPIM Spim, WPARAM wParam, LPARAM lParam, SPWindow* pSender );
	int SPSendMessageToParent	( SPIM Spim, WPARAM wParam, LPARAM lParam, SPWindow* pSender );

	// Window Message // 만약을 위해
	bool SPSendMessage( SPWindow* pSender, SPIM SpimID, WPARAM wParam, LPARAM lParam );	

	virtual void SetImage(SPTexture* pImage, int iSrcX=0, int iSrcY=0);
	virtual bool SetImage(const char* szImage, int iSrcX=0, int iSrcY=0);
	virtual void SetImagePos(int iSrcX, int iSrcY);

	void AddChild(SPWindow* pWindow);	
	bool CleanChild(SPWindow* pWindow, bool bChild = false);
	void CleanChildAll();
	
	void AdjustCoord( int iX, int iY, int iCX = -1, int iCY = -1);
	virtual void RefreshRelationCoord();
	virtual void RefreshChildPos();			//	RefreshRelationCoord 랑 동일함 단 무브 체크 안함. AJJIYA  [6/14/2006]
	void GetAbsCoord( int& iX, int& iY);
	void GetRelCoord( int& iX, int& iY);

	SPWindow*	GetWrapWindow();
	bool		IsWrapWindow();	

	void		SetMoveable(bool bMoveable);
	bool		IsMoveable();
	bool		IsMoveOn();

	SPWindow*	Find(INSTANCE_ID iID, bool bChild = false);
	void GetWindowRect( RECT& Rect );

	virtual bool IsCursorIn ( int iX, int iY );
	virtual bool IsCursorIn ();

	virtual bool IsFocus	()							{ return m_bFocus; }
	virtual void SetFocus	();
	virtual void KillFocus	();							

	void SetMultiLine(bool bEnable, int iInterval);

	SPWindow*	GetParent()						{ return m_pParent;		}
	SPWindow*	GetTabStopWindow()				{ return m_pWindowTabStop;		}	

	// WrapWindow가 Child를 같고있다.
	// TabStop을 지원하는 윈도우 인지..
	bool IsTabStopWindow()						{ return m_bTabStop; }
	bool SetEnableTabStop(bool bTabStop = true);
	void SetTabStopWindow(SPWindow* pWindow)	{ m_pWindowTabStop = pWindow;	}
	void ClearTabStopWindow() ;
	SPWindow* NextTabStop();
	SPWindow* PrevTabStop();

	// Enter 눌릴때 현재 눌려야 할 버튼.
	void		SetEnterFocusWindow(SPWindow* pWindow);
	SPWindow*	GetEnterFocusWindow();

	INSTANCE_ID		GetInstanceID()				{ return m_InstanceID;	}
	WND_ID			GetClassID()				{ return m_WndClassID;	}
	WND_STATE		GetState()					{ return m_WindowState;	}

	virtual const char*		GetWindowText()		{ return m_strWindowText.c_str();	}	// AJJIYA [8/24/2005]

	void SetRectSize( int isx,	int isy );
	void SetAbsCoord( int ix,	int iy );
	virtual void SetSrcSize(int isx, int isy);
	void SetLightMap(bool bEnable)				{ m_bLightMap = bEnable; }

	void SetHideEnable(bool bEnable)			{ m_bHideEnable = bEnable; }
	void SetShowEnable(bool bEnable, bool bUpdate = true) ;
	bool IsShowEnable()							{	return m_bShowEnable;	}
	void SetColor(DWORD dwColor)				{ m_RGBA = dwColor; }

	void SetTextPeriod(bool bPeriod)			{ m_bTextPeriod = bPeriod; }
	void SetNullWindow(int rec_flg)				{ m_flg_NullWindow = rec_flg; }
	int IsNullWindow()							{ return m_flg_NullWindow; }

	virtual void RefreshChildPos(REFRESHCHILD_METHOD rec_Method);						//- W.Fairy [09/29/2008] : 툴 사용을 위해 추가된 함수.
	virtual void SetRenderingMethod(RENDERING_METHOD rec_Method,bool rec_Flags);		//- W.Fairy [09/29/2008] : 툴 사용을 위해 추가된 함수.

	void SetWindowRelPos( int ix,	int iy );   //[xialin 2008/07/25]add

protected:
	
	void SetRelCoord( int ix,	int iy );
	bool SwapFocusWrap(SPWindow* pWindow);

	void InitTabStop();
	bool InsertTabStop(SPWindow* pWindow);
	bool EraseTabStop(SPWindow* pWindow);


	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()
	virtual int OnNULL			( WPARAM, LPARAM );	
	virtual int OnCursorEnter	( WPARAM, LPARAM );
	virtual int OnCursorOut		( WPARAM, LPARAM );
	virtual int OnTab			( WPARAM, LPARAM );
	virtual int OnHide			( WPARAM, LPARAM );
	virtual int OnSetFocus		( WPARAM, LPARAM );
	virtual int OnBeginMove		( WPARAM, LPARAM );
	virtual int OnEndMove		( WPARAM, LPARAM );	
	virtual int OnMove			( WPARAM, LPARAM );
	// }}

protected:

	SPTexture**		m_ppImageActive;
	SPTexture*		m_pImageNormal;
	DWORD			m_RGBA;
	FONT_SIZE		m_FontSize;

	std::string		m_strWindowText;
	std::string		m_strDebugText;

	bool			m_bEnableToolTip;
	std::string		m_strToolTip;
	
	bool			m_bShow;
	bool			m_bEnable;
	bool			m_bTwinkle;
	bool			m_bFocus;
	bool			m_bTabStop;

	D3DXCOLOR		m_FontColor	;	
	D3DXCOLOR		m_FontShadowColor	;	
	DWORD			m_dwAlign	;
	//bool			m_bFontShadow;
	SPFONT_EFFECT	m_FontEffect;

	int				m_iAX;	//절대좌표
	int				m_iAY;	
	int				m_iRX;	//상대좌표
	int				m_iRY;
	int				m_iSX;	//사이즈
	int				m_iSY;
	int				m_iSrcX, m_iSrcY;	// Texture Source의 좌표
	int				m_iNormalSrcX, m_iNormalSrcY;	// Texture Source의 좌표	

	int				m_iSrcSX;
	int				m_iSrcSY;
	
	SPWindow*		m_pParent;
	SPWindow*		m_pWindowTabStop;
	SPWindow*		m_pWindowEnterFocus;

	std::vector<SPWindow*>	m_vpChildWindow;
	std::vector<SPWindow*>	m_vpWindowTabStop;

	WND_ID			m_WndClassID;
	INSTANCE_ID		m_InstanceID;
	WND_STATE		m_WindowState;

	bool			m_bHideEnable;
	bool			m_bShowEnable;
	bool			m_bMultiLine;
	int				m_iLineInterval;
	bool			m_bLightMap;

	bool			m_bMoveable;
	bool			m_bMove;
	POINT			m_ptMoveStart;	

	WND_TYPE		m_WndType;
	SPFont*			m_pFont;

	bool			m_bTextPeriod;
	int				m_flg_NullWindow;
	
	bool			m_flg_Flip_LR;      //- W.Fairy :  임시 플립 플래그. 코어에서 지원되면
	bool			m_flg_Flip_TB;      //            삭제 요망 합니다.

};






