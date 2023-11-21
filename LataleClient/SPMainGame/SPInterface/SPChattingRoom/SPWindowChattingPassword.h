// ***************************************************************
//  SPWindowChattingPassword   version:  1.0   ¡¤  date: 04/04/2008
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#ifndef SPWindowChattingPassword_h__
#define SPWindowChattingPassword_h__

#pragma once

#define WIID_CHATTING_PASSWORD_BG								(WIID_CHATTING_PASSWORD + 1)
#define WIID_CHATTING_PASSWORD_CLOSE							(WIID_CHATTING_PASSWORD + 2)
#define WIID_CHATTING_PASSWORD_SUBJECT							(WIID_CHATTING_PASSWORD + 3)
#define WIID_CHATTING_PASSWORD_EDIT								(WIID_CHATTING_PASSWORD + 4)
#define WIID_CHATTING_PASSWORD_OK								(WIID_CHATTING_PASSWORD + 5)

//////////////////////////////////////////////////////////////////////////

struct CHATTING_PASSWORD_SET_DATA
{
	std::string					m_strSubject;

	int							m_iRoomIndex;
	int							m_iRoomNumber;
	int							m_iRoomCategory;
	bool						m_bLocalSend;
};

struct CHATTING_PASSWORD_GET_DATA
{
	std::string					m_strPassword;

	int							m_iRoomIndex;
	int							m_iRoomNumber;
	int							m_iRoomCategory;
	bool						m_bLocalSend;
};

class SPWindow;

class SPWindowChattingPassword : public SPWindow
{
public:
	SPWindowChattingPassword								( WND_ID WndClassID , INSTANCE_ID InstanceID );
	SPWindowChattingPassword								( INSTANCE_ID InstanceID , int iX , int iY , int iCX , int iCY , SPWindow* pParent = NULL );
	virtual ~SPWindowChattingPassword						();

	virtual void			Init							();
	virtual void			Clean							();

	virtual void			Process							( float fTime );
//	virtual void			Render							( float fTime );

	virtual void			Show							( bool bWithChild = true );
	virtual void			Hide							( bool bSendServer = true );
//	virtual void			Close							();

//	virtual void			RefreshRelationCoord			();

protected:

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()

	//	Common
	virtual		int			OnSet							( WPARAM wParam, LPARAM lParam );
	virtual		int			OnKeyReturn						( WPARAM wParam, LPARAM lParam );
	virtual		int			OnClose							( WPARAM wParam, LPARAM lParam );
	virtual		int			OnOk							( WPARAM wParam, LPARAM lParam );
	virtual		int			OnEditSetFocus					( WPARAM wParam, LPARAM lParam );
	virtual		int			OnEditKillFocus					( WPARAM wParam, LPARAM lParam );
	// }}

protected:

	void					SendMsg							( SPIM SpimID );
	void					ClearData						();

	//////////////////////////////////////////////////////////////////////////
	//
	//	º¯¼öµé
	//

	bool									m_bUse;

	SPWindow*								m_pWindowSender;

	CHATTING_PASSWORD_SET_DATA*				m_pSetData;
	CHATTING_PASSWORD_GET_DATA				m_GetData;


};

#endif // SPWindowChattingPassword_h__