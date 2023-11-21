#ifdef _PHANTOM_EDITOR_ENABLE
#pragma once
//--------------------------------------------------
enum WIID_PHANTOM_EDITOR_COMPONENTS
{
	WIID_PHANTOM_EDITOR______ = WIID_PHANTOM_EDITOR,
	WIID_PHANTOM_EDITOR_CLOSE,
	WIID_PHANTOM_EDITOR_R,
	WIID_PHANTOM_EDITOR_G,
	WIID_PHANTOM_EDITOR_B,
	WIID_PHANTOM_EDITOR_NUMBER,
	WIID_PHANTOM_EDITOR_DELAYTIME,
	WIID_PHANTOM_EDITOR_APPLY,
	WIID_PHANTOM_EDITOR_PREVIEW,
	WIID_PHANTOM_EDITOR_BACKGROUD,
	WIID_PHANTOM_EDITOR_FIRSTALPHA,

	WIID_PHANTOM_EDITOR_FORCEENUMMAX = WIID_PHANTOM_EDITOR + 1000
};
class SPWindow;
class SPWindowStatic;
class SPWindowEdit;
//--------------------------------------------------
class SPWindowPhantomEditor : public SPWindow
{
public:
	SPWindowPhantomEditor( WND_ID wndClassID, INSTANCE_ID instanceID );
	SPWindowPhantomEditor( INSTANCE_ID instanceID, 
		int iX, int iY, int iCX, int iCY, 
		SPWindow *pParent = NULL );
	~SPWindowPhantomEditor( void );

	virtual void Init( void );
	virtual void Clean( void );
	virtual void Show( bool bWithChild = true );
	virtual void Hide( bool bSendServer = true );
	virtual void Process( float fDeltaTime );
	virtual void Render( float fDeltaTime );
	virtual void RefreshRelationCoord( void );

protected:
	SPIMESSAGE_OBJ_MAP_DECLARE();
	int _onBtnApply( WPARAM wParam, LPARAM lParam );
	int _onBtnClose( WPARAM wParam, LPARAM lParam );
	
protected:
	bool _onApply( void );
	bool _initGUI( void );
	
protected:
	SPWindowButton	*m_pBtnApply;
	SPWindowStatic	*m_pColorPreview;
	SPWindowEdit	*m_pEditR;
	SPWindowEdit	*m_pEditG;
	SPWindowEdit	*m_pEditB;
	SPWindowEdit	*m_pEditMaxNum;
	SPWindowEdit	*m_pEditDelayTime;
	SPWindowEdit	*m_pEditFirstAlpha;
};
//--------------------------------------------------
#endif