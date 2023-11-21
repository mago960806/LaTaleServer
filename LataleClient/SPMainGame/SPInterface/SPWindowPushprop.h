#pragma once

// QB is the ab. of quick buy
#define WIID_ITEM_QB					(WIID_PUSH_PROP+ 210)
#define WIID_ITEM_QB_END				(WIID_PUSH_PROP+ 310)

#define QB_SLOT_NUMBER		(6)
enum WIID_ITEM_QB_COMPONENTS
{
	WIID_ITEM_QB_HEADER = WIID_ITEM_QB,
	WIID_ITEM_QB_BODY,
	WIID_ITEM_QB_BOTTOM,
	WIID_ITEM_QB_SLOT_START,
	WIID_ITEM_QB_SLOT_END = WIID_ITEM_QB_SLOT_START + QB_SLOT_NUMBER - 1,
	WIID_ITEM_QB_SLOT_BG_START,
	WIID_ITEM_QB_SLOT_BG_END = WIID_ITEM_QB_SLOT_BG_START + QB_SLOT_NUMBER - 1,
	WIID_ITEM_QB_ROLLUP,
	WIID_ITEM_QB_ROLLDOWN,

	WIID_ITEM_QB_FORCEENUMMAX = WIID_ITEM_QB_END 
};
//--------------------------------------------------
class SPWindow;
class SPWindowStatic;
class SPWindowButton;
class SPWindowPushProp : public SPWindow
{
public:
	SPWindowPushProp( WND_ID wndClassID, INSTANCE_ID instanceID );
	SPWindowPushProp( INSTANCE_ID instanceID, 
		int iX, int iY, int iCX, int iCY, 
		SPWindow *pParent = NULL );
	virtual ~SPWindowPushProp( void );

	virtual void Init( void );
	virtual void Clean( void );
	virtual void Show( bool bWithChild = true );
	virtual void Hide( bool bSendServer = true );
	virtual void Process( float fDeltaTime );
	virtual void Render( float fDeltaTime );
	virtual void RenderReverse( float fDeltaTime );
	virtual void SpecialRender( float fDeltaTime );
	virtual void RefreshChildPos( void );

public:
	void OnQBInfoAck( CPacket* packet );

protected:
	//--------------------------------------------------
	SPIMESSAGE_OBJ_MAP_DECLARE();
	
	bool initQB_GUI( void );
	int	onQB_RollUp( WPARAM wParam, LPARAM lParam );
	int	onQB_RollDown( WPARAM wParam, LPARAM lParam );

	int onQB_TollTipEnter( int iID, WPARAM wParam, LPARAM lParam );
	int onQB_TollTipOut( int iID, WPARAM wParam, LPARAM lParam );
	int onQB_TollTipMove( int iID, WPARAM wParam, LPARAM lParam );

	int	onQB_SlotLBtnUp( int iID, WPARAM wParam, LPARAM lParam );
	//--------------------------------------------------

	//--------------------------------------------------
	SPWindowStatic	*m_ptrQB_Header;
	SPWindowStatic	*m_ptrQB_Body;
	SPWindowStatic	*m_ptrQB_Bottom;
	SPWindowStatic	*m_ptrQB_SLOT_BG[QB_SLOT_NUMBER];
	SPWindowStatic	*m_ptrQB_SLOT[QB_SLOT_NUMBER];
	SPWindowButton	*m_ptrQB_RollUp;
	SPWindowButton	*m_ptrQB_RollDown;
	static vector< t_Quick_Buy_Info > m_vecQBInfo;
	//--------------------------------------------------
};