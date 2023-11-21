// ***************************************************************
//  SPChatBoard   version:  1.0   ¡¤  date: 04/10/2008
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#ifndef SPChatBoard_h__
#define SPChatBoard_h__

#pragma once

enum CHATBOARD_TEXTURE
{
	CHATBOARD_TEXTURE_BG		=	0	,
	CHATBOARD_TEXTURE_OBJECT			,
	CHATBOARD_TEXTURE_LOCK				,
	CHATBOARD_TEXTURE_TYPE				,
	CHATBOARD_TEXTURE_NO				,
	CHATBOARD_TEXTURE_NO_100			,
	CHATBOARD_TEXTURE_NO_10				,
	CHATBOARD_TEXTURE_NO_1				,
	CHATBOARD_TEXTURE_FULL				,
	CHATBOARD_TEXTURE_MAX				,
};

enum CHATBOARD_LOCK
{
	CHATBOARD_LOCK_NO			=	0	,
	CHATBOARD_LOCK_YES					,
	CHATBOARD_LOCK_MAX					,
};

enum CHATBOARD_TYPE
{
	CHATBOARD_TYPE_NORMAL		=	0	,
	CHATBOARD_TYPE_MARKET				,
	CHATBOARD_TYPE_WANTED				,
	CHATBOARD_TYPE_MEETING				,
	CHATBOARD_TYPE_MAX					,
};

enum CHATBOARD_NO
{
	CHATBOARD_NO_0				=	0	,
	CHATBOARD_NO_1						,
	CHATBOARD_NO_2						,
	CHATBOARD_NO_3						,
	CHATBOARD_NO_4						,
	CHATBOARD_NO_5						,
	CHATBOARD_NO_6						,
	CHATBOARD_NO_7						,
	CHATBOARD_NO_8						,
	CHATBOARD_NO_9						,
	CHATBOARD_NO_MAX					,
};

enum CHATBOARD_FULL
{
	CHATBOARD_FULL_NO			=	0	,
	CHATBOARD_FULL_YES					,
	CHATBOARD_FULL_MAX					,
};

class SPGameObject;

class SPChatBoard : public SPGameObject
{
public:
	SPChatBoard									();
	SPChatBoard									( INSTANCE_ID instanceID , CLASS_ID ClassID );
	virtual ~SPChatBoard						();

	virtual void				Init			();
	virtual void				Destroy			();
	virtual void				Process			( float fTime );
	virtual void				Render			( float fTime );

	virtual int					PerformMouseMove( int iX , int iY );
	virtual int					PerformMouseLUp	( int iX , int iY );
	virtual bool				IsCursorIn		( int iX , int iY );

	void						SetPosX			( int iPosX );
	void						SetPosY			( int iPosY );

	int							GetPosX			();
	int							GetPosY			();

	void						SetStandLayer	( int iLayer );

	int							GetStandLayer	();

	void						SetChatRoomInfo	( CHATROOM_OBJECT_INFO& stChatRoomInfo );

	bool						IsCursorIn		( RECT rcBound );

protected:

	SPGMESSAGE_OBJ_MAP_DECLARE()

	virtual int					OnGetStandlayer	( LPARAM lParam );

	//////////////////////////////////////////////////////////////////////////

	float						m_fCamX;
	float						m_fCamY;

//	SPGOBCoordPhysics*			m_pCoordPhysics;

	CHATROOM_OBJECT_INFO		m_stChatRoomInfo;

	SPTexture*					m_pTexture			[ CHATBOARD_TEXTURE_MAX ];
	RECT						m_rcDest			[ CHATBOARD_TEXTURE_MAX ];
	RECT						m_rcSrc				[ CHATBOARD_TEXTURE_MAX ];

	RECT						m_rcDestOriginal	[ CHATBOARD_TEXTURE_MAX ];
	RECT						m_rcSrcType			[ CHATBOARD_TYPE_MAX ];
	RECT						m_rcSrcNo			[ CHATBOARD_NO_MAX ];
	RECT						m_rcSrcLock			[ CHATBOARD_LOCK_MAX ];
	RECT						m_rcSrcFull			[ CHATBOARD_FULL_MAX ];

	int							m_iTempIndex;

	SPFont*						m_pFont;

	RECT						m_rcBound;
	RECT						m_rcBoundOriginal;

	RECT						m_rcBoundClick;
	RECT						m_rcBoundClickOriginal;

	RECT						m_rcTextDestOriginal;

	std::vector< RECT >			m_vTextDest;
	std::vector< RECT >			m_vTextDestOriginal;
	std::vector< std::string >	m_vTextBuf;

	int							m_iPosX;
	int							m_iPosY;

	int							m_iPosXOriginal;
	int							m_iPosYOriginal;

	int							m_iScreenX;
	int							m_iScreenY;

	int							m_iStandLayer;



};

#endif // SPChatBoard_h__