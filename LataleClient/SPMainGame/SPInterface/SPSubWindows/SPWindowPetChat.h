#pragma once
#include <vector>
#include <string>
//--------------------------------------------------
enum WIID_PETCHAT_EDITOR_COMPONENTS
{
	WIID_PETCHAT_EDITOR_____ = WIID_PETCHAT_EDITOR,
	WIID_PETCHAT_EDITOR_BG_LEFT,
	WIID_PETCHAT_EDITOR_BG_MIDDLE,
	WIID_PETCHAT_EDITOR_BG_RIGHT,
	WIID_PETCHAT_EDITOR_ICONTITLE,
	WIID_PETCHAT_EDITOR_CLOSE,
	WIID_PETCHAT_EDITOR_TXT,
	WIID_PETCHAT_EDITOR_CHAT_01,
	WIID_PETCHAT_EDITOR_CHAT_02,
	WIID_PETCHAT_EDITOR_CHAT_03,
	WIID_PETCHAT_EDITOR_SELECT_01,
	WIID_PETCHAT_EDITOR_SELECT_02,
	WIID_PETCHAT_EDITOR_SELECT_03,
	WIID_PETCHAT_EDITOR_OK,

	WIID_PETCHAT_EDITOR_FORCEENUMMAX = WIID_PETCHAT_EDITOR + 0x999
};
//--------------------------------------------------
class SPWindow;
class SPWindowStatic;
class SPWindowEdit;
class SPPet;
//--------------------------------------------------
class SPWindowPetChatEditor : public SPWindow
{
public:
	SPWindowPetChatEditor( WND_ID wndClassID, INSTANCE_ID instanceID );
	SPWindowPetChatEditor( INSTANCE_ID instanceID, 
		int iX, int iY, int iCX, int iCY, 
		SPWindow *pParent = NULL );
	~SPWindowPetChatEditor( void );

	virtual void Init( void );
	virtual void Clean( void );
	virtual void Show( bool bWithChild = true );
	virtual void Hide( bool bSendServer = true );
	virtual void Process( float fDeltaTime );
	virtual void Render( float fDeltaTime );

	void SetPetIndex( PET_PLACE iPlace );
	bool	_loadXmlCfg( void );
	bool	_saveXmlCfg( void );
	std::vector< std::string >	GetPetChatVector( PET_PLACE iPlace );

protected:
	SPIMESSAGE_OBJ_MAP_DECLARE();
	int		onBtnClose( WPARAM wParam, LPARAM lParam );
	int		onBtnOK( WPARAM wParam, LPARAM lParam );
	int		onEditSetFocus( WPARAM wParam, LPARAM lParam );
	int		onEditKillFocus( WPARAM wParam, LPARAM lParam );
	int		onCheckBtn_01( WPARAM wParam, LPARAM lParam );
	int		onCheckBtn_02( WPARAM wParam, LPARAM lParam );
	int		onCheckBtn_03( WPARAM wParam, LPARAM lParam );

protected:
	bool	_initGUI( void );

protected:
	SPWindowStatic		*m_pImgBGLeft;
	SPWindowStatic		*m_pImgBGMiddle;
	SPWindowStatic		*m_pImgBGRight;
	SPWindowStatic		*m_pImgIconTitle;
	SPWindowButton		*m_pBtnClose;
	SPWindowButton		*m_pBtnOK;

	SPWindowEdit		*m_pChatMsg_01;
	SPWindowEdit		*m_pChatMsg_02;
	SPWindowEdit		*m_pChatMsg_03;

	SPWindowButton		*m_pSelectMsg_01;
	SPWindowButton		*m_pSelectMsg_02;
	SPWindowButton		*m_pSelectMsg_03;

	std::vector< std::vector<std::string> >	m_vecstrChatMsg;
	std::string m_strCurrPetChatCfg;
	PET_PLACE			m_iPetSlotIndex;
};
//--------------------------------------------------
// end of file
