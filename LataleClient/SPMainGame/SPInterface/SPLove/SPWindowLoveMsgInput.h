// ***************************************************************
//  SPWindowLoveMsgInput		Version:  1.0		Date: 2008/11/03
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#ifndef SPWindowLoveMsgInput_h__
#define SPWindowLoveMsgInput_h__

#pragma once

#define WIID_LOVEMSG_INPUT_BG									0x31006100
#define WIID_LOVEMSG_INPUT_BTN_CLOSE							0x31006101
#define WIID_LOVEMSG_INPUT_CAPTION_SMG							0x31006102
#define WIID_LOVEMSG_INPUT_TIME_BG								0x31006103
#define WIID_LOVEMSG_INPUT_TIME									0x31006104
#define WIID_LOVEMSG_INPUT_BTN_OK								0x31006105
#define WIID_LOVEMSG_INPUT_BTN_CANCEL							0x31006106
#define WIID_LOVEMSG_INPUT_EDITBG_L								0x31006107
#define WIID_LOVEMSG_INPUT_EDITBG_M								0x31006108
#define WIID_LOVEMSG_INPUT_EDITBG_R								0x31006109

#define WIID_LOVEMSG_INPUT_EDIT									0x31006110

enum LOVEMSG_INPUT_GENDER
{
	LOVEMSG_INPUT_GENDER_NULL			=	0	,
	LOVEMSG_INPUT_GENDER_MAN					,
	LOVEMSG_INPUT_GENDER_WOMAN					,
};

class SPWindow;

class SPWindowLoveMsgInput : public SPWindow
{
public:
	SPWindowLoveMsgInput									( WND_ID WndClassID , INSTANCE_ID InstanceID );
	SPWindowLoveMsgInput									( INSTANCE_ID InstanceID , int iX , int iY , int iCX , int iCY , SPWindow* pParent = NULL );
	virtual ~SPWindowLoveMsgInput							();

	virtual void			Init							();
	virtual void			Clean							();

	virtual void			Process							( float fTime );
	virtual void			Render							( float fTime );

	virtual void			Show							( bool bWithChild = true );
	virtual void			Hide							( bool bSendServer = true );

	virtual void			RefreshRelationCoord			();

protected:

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()

	//	Common
	virtual		int			OnKeyReturn						( WPARAM wParam, LPARAM lParam );

	virtual		int			OnEditSetFocus					( WPARAM wParam, LPARAM lParam );
	virtual		int			OnEditKillFocus					( WPARAM wParam, LPARAM lParam );

	virtual		int			OnClose							( WPARAM wParam, LPARAM lParam );
	virtual		int			OnOk							( WPARAM wParam, LPARAM lParam );

	// }}

	void					SetGender						( LOVEMSG_INPUT_GENDER eGender );

	void					CheckGender						();

	void					TimeAction						();


	//////////////////////////////////////////////////////////////////////////
	//
	//	변수들
	//

	bool													m_bUse;

	float													m_fReMainTime;
	SPWindow*												m_pReMainTimeWindow;

	//	임시변수

	char													m_szBuf[ _MAX_PATH ];

};

#endif // SPWindowLoveMsgInput_h__