// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-08-08 ¿ÀÀü 10:26:19
//***************************************************************************


#pragma once




class SPWindow;
class SPWindowButton :	public SPWindow {
public:	
	SPWindowButton(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent, WND_ID CtrlType = WNDID_CTRL_BUTTON);
	~SPWindowButton();

	virtual void Init();
	virtual void InitFromXML(SPWindow_Base* rec_wnd_AttrMap);
	virtual void Clean();	
	virtual void Process(float fTime);	
	virtual void Render(float fTime);
	virtual void RenderText();

	virtual void SetEnable(bool bEnable = true);

	void	SetImageDisable 	(SPTexture* pImage, int iXSrc=0, int iYSrc=0);
	void	SetImageHit 		(SPTexture* pImage, int iXSrc=0, int iYSrc=0);
	void	SetImagePush 		(SPTexture* pImage, int iXSrc=0, int iYSrc=0);

	bool	SetImageDisable 	(const char* szImage, int iXSrc=0, int iYSrc=0);
	bool	SetImageHit 		(const char* szImage, int iXSrc=0, int iYSrc=0);	
	bool	SetImagePush 		(const char* szImage, int iXSrc=0, int iYSrc=0);	
	bool	SetImageCheck		(const char* szImage, int iXSrc=0, int iYSrc=0);

	bool	IsChecked();

	//enum {
	//	BST_UNCHECKED	= 0,
	//	BST_PUSHED		= 1,
	//	BST_HIT			= (1<<1),
	//	BST_DISABLE		= (1<<2),
	//};

protected:
	
	SPIMESSAGE_OBJ_MAP_DECLARE()
	// {{ Message Map	
	virtual int OnCursorEnter	( WPARAM, LPARAM );
	virtual int OnCursorOut		( WPARAM, LPARAM );
	virtual int OnLButtonDown	( WPARAM, LPARAM );
	virtual int OnLButtonUP		( WPARAM, LPARAM );
	virtual int OnRButtonDown	( WPARAM, LPARAM );
	virtual int OnRButtonUP		( WPARAM, LPARAM );
	virtual int OnKeyEnter		( WPARAM, LPARAM );
	virtual int OnBtnEnable		( WPARAM wParam, LPARAM lParam);
	virtual int OnBtnSelect		( WPARAM wParam, LPARAM lParam);
	virtual int OnLButtonDBLClick( WPARAM, LPARAM );
	virtual int OnSetColor		( WPARAM wParam, LPARAM lParam );

	virtual int OnToolTipMove	( WPARAM, LPARAM );
	virtual int OnToolTipEnter	( WPARAM, LPARAM );
	virtual int OnToolTipOut	( WPARAM, LPARAM );

	virtual int OnBeginMove		( WPARAM, LPARAM );
	virtual int OnEndMove		( WPARAM, LPARAM );	
	virtual int OnMove			( WPARAM, LPARAM );
	// }}

protected:

	SPTexture* m_pImageDisable;	
	SPTexture* m_pImageHit;		
	SPTexture* m_pImagePush;
	SPTexture* m_pImageChecked;	

	POINT	m_ptDisableSrcPos;
	POINT	m_ptHitSrcPos;
	POINT	m_ptPushSrcPos;
	POINT	m_ptCheckSrcPos;

	bool	m_bDisable;
	bool	m_bPushed;
	bool	m_bChecked;


};








