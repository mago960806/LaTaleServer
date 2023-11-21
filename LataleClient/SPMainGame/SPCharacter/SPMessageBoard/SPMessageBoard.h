// ***************************************************************
//  SPChatBoard   version:  1.0   ¡¤  date: 04/10/2008
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#ifndef SPMseeageBoard_h__
#define SPMseeageBoard_h__

#pragma once
#include "atltime.h"


class SPGameObject;

class SPMessageBoard : public SPGameObject
{
public:
	SPMessageBoard			();
	SPMessageBoard			( INSTANCE_ID instanceID , CLASS_ID ClassID );
	virtual ~SPMessageBoard	();

	virtual void				Init			();
	virtual void				Destroy			();
	virtual void				Process			( float fTime );
	virtual void				Render			( float fTime );

	virtual int					PerformMouseMove( int iX , int iY );
	virtual int					PerformMouseLUp	( int iX , int iY );
	virtual int					PerformMouseLDBLClick	( int iX , int iY );
	virtual bool				IsCursorIn		( int iX , int iY );
	virtual bool				IsCursorIn		( RECT & rc );

	void						SetPosX			( int iPosX );
	void						SetPosY			( int iPosY );

	int							GetPosX			();
	int							GetPosY			();

	void						SetStandLayer	( int iLayer );

	int							GetStandLayer	();

	void     SetMsgType(int  iType) { m_iType = iType; }
	void     SetName(const char * szName) { m_strName = szName; }
	void     SetMsg(const char * szMsg) { m_strMsg = szMsg; }
	void     SetDate(const CTime& tmDate) { m_tmDate = tmDate; }

protected:

	SPGMESSAGE_OBJ_MAP_DECLARE()

	virtual int					OnGetStandlayer	( LPARAM lParam );

	//////////////////////////////////////////////////////////////////////////
	int                         m_iType;
	string                      m_strName;
	string                      m_strMsg;
	CTime						m_tmDate;

	float						m_fCamX;
	float						m_fCamY;
	int							m_iScreenX;
	int							m_iScreenY;

	int							m_iPosX;
	int							m_iPosY;

	SPTexture*					m_pTexture;
	RECT						m_rcTextureSrc;
	RECT						m_rcTitleDest;
	RECT						m_rcTextDest;

	int							m_iStandLayer;

};

#endif // SPMessageBoard_h__