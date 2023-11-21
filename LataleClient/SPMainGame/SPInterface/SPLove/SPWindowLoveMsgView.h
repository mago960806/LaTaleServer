// ***************************************************************
//  SPWindowLoveMsgView		Version:  1.0		Date: 2008/11/03
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#ifndef SPWindowLoveMsgView_h__
#define SPWindowLoveMsgView_h__

#pragma once

#define WIID_LOVEMSG_VIEW_CHAR_NAME							(WIID_LOVEMSG_VIEW + 1)
#define WIID_LOVEMSG_VIEW_MSG								(WIID_LOVEMSG_VIEW + 2)

struct LOVEMSG_VIEW_DATA
{
	std::string			m_strCharID;
	std::string			m_strMsg;
};

class SPWindow;

class SPWindowLoveMsgView : public SPWindow
{
public:
	SPWindowLoveMsgView										( WND_ID WndClassID , INSTANCE_ID InstanceID );
	SPWindowLoveMsgView										( INSTANCE_ID InstanceID , int iX , int iY , int iCX , int iCY , SPWindow* pParent = NULL );
	virtual ~SPWindowLoveMsgView							();

	virtual void			Init							();
	virtual void			Clean							();

	virtual void			Process							( float fTime );
//	virtual void			Render							( float fTime );

	virtual void			Show							( bool bWithChild = true );
	virtual void			Hide							( bool bSendServer = true );

protected:

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()

	//	Common
	virtual int				OnAddItem						( WPARAM wParam, LPARAM lParam );

	// }}

	void					TimeAction						();


	//////////////////////////////////////////////////////////////////////////
	//
	//	변수들
	//

	bool													m_bUse;

	float													m_fReMainTime;

	//	임시변수

	char													m_szBuf[ _MAX_PATH ];

};

#endif // SPWindowLoveMsgView_h__