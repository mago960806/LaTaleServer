// ***************************************************************
//  SPWindowFindMessageTwo   version:  1.0   ¡¤  date: 31/08/2008
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2008.8.31 - All Rights Reserved
// ***************************************************************
// liuyang
// ***************************************************************

#pragma once

#define WIID_FIND_MESSAGE_TWO_MINIMIZE					(WIID_FIND_MESSAGE_TWO + 10)
#define WIID_FIND_MESSAGE_TWO_MINIMIZE_BG				(WIID_FIND_MESSAGE_TWO + 11)
#define WIID_FIND_MESSAGE_TWO_MINIMIZE_TO_MAXIMIZE		(WIID_FIND_MESSAGE_TWO + 12)

#define WIID_FIND_MESSAGE_TWO_MAXIMIZE					(WIID_FIND_MESSAGE_TWO + 20)
#define WIID_FIND_MESSAGE_TWO_MAXIMIZE_TITLE_LEFT		(WIID_FIND_MESSAGE_TWO + 21)
#define WIID_FIND_MESSAGE_TWO_MAXIMIZE_TITLE_CENTER		(WIID_FIND_MESSAGE_TWO + 22)
#define WIID_FIND_MESSAGE_TWO_MAXIMIZE_TITLE_RIGHT		(WIID_FIND_MESSAGE_TWO + 23)
#define WIID_FIND_MESSAGE_TWO_MAXIMIZE_TITLE_IMG		(WIID_FIND_MESSAGE_TWO + 24)

#define WIID_FIND_MESSAGE_TWO_MAXIMIZE_WINDOW_TOP		(WIID_FIND_MESSAGE_TWO + 25)
#define WIID_FIND_MESSAGE_TWO_MAXIMIZE_WINDOW_BOTTOM	(WIID_FIND_MESSAGE_TWO + 26)

#define WIID_FIND_MESSAGE_TWO_MAXIMIZE_TO_MINIMIZE		(WIID_FIND_MESSAGE_TWO + 31)

#define WIID_FIND_MESSAGE_TWO_TEXT_BOX					(WIID_FIND_MESSAGE_TWO + 32)

#define FIND_MESSAGE_TWO_BG_COUNT					2

class SPWindow;
class SPWindowButton;
class SPWindowEditMultiLine;
class SPTimer;

class SPWindowFindMessageTwo : public SPWindow
{
public:

	SPWindowFindMessageTwo						( WND_ID WndClassID , INSTANCE_ID InstanceID );
	SPWindowFindMessageTwo						( INSTANCE_ID InstanceID , int iX , int iY , int iCX , int iCY , SPWindow* pParent = NULL );
	virtual ~SPWindowFindMessageTwo			();

	virtual void	Init					();
	virtual void	Clean					();

	virtual void	Process					( float fTime );
	virtual void	Render					( float fTime );

	virtual void	Show					( bool bWithChild = true );

	void			SetMinimized			( bool bMinimized );
	bool			GetMinimized			();

	int				setFindName				( std::string Name );
	std::string		getFindName				();
	
	int				SendSearchName			();

	void			setID					( int iID );
	int				getID					();

	void            setSendMessage			( bool isSend );
	
	static void		SetToStart				();

	SPWindowEditMultiLine* getEditMultiLine(); 

	static int						timer;
	static int						ShowTime;

	SPTimer*		getTimer				();
protected:

	//Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()

	virtual int		OnMininize				( WPARAM wParam, LPARAM lParam );
	virtual int		OnMaximize				( WPARAM wParam, LPARAM lParam );


	virtual int		OnPurge					( WPARAM wParam, LPARAM lParam );


protected:

	void			UpdateWindow			( bool bMinimized );

	SPWindowEditMultiLine*			m_pEditMultiLine;
	bool							m_bMinimized;
	
	std::string						m_strFindName;

	int								m_iInitPosX;
	int								m_iInitPosY;
	
	int								m_iID;
	int								m_fLeavingTime;
	bool							m_bSendMessage;
	SPTimer*						m_pFindTimer;

};
