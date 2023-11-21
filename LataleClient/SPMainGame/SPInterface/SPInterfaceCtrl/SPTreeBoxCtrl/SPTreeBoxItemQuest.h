/********************************************************************
    Copyright (C) 2006 AJJIYA - All Rights Reserved
	created:	2006/06/23
	created:	2006:6:23:   15:59
	filename: 	SPTreeBoxItemQuest.h
	
	purpose:	
*********************************************************************/

#pragma once

class SPWindow;

class SPTreeBoxItemBase;

class SPTreeBoxItemQuest : public SPTreeBoxItemBase
{
public:
	SPTreeBoxItemQuest								();
	virtual	~SPTreeBoxItemQuest						();

	virtual	void					Init			();
	virtual	void					Clear			();

	virtual	void					Process			( float fTime );
	virtual	void					Render			( float fTime );

	virtual	int						GetNextPosX		();
	virtual	int						GetNextPosY		();

	virtual	bool					IsCursorIn		( int iX , int iY );

	virtual	void					SetPos			( int iX , int iY );
	virtual	RECT					GetPosRect		();

	virtual	void					SetStatus		( TREEBOX_DATA_STATUS_LABEL eStatus );
	virtual	void					AddStatus		( DWORD dwStatus );
	virtual	void					DelStatus		( DWORD dwStatus );

	enum TREEBOX_QUEST_TEXTURE_LABEL
	{
		QUEST_TEXTURE_LEFT_ENABLE		=	0							,
		QUEST_TEXTURE_RIGHT_ENABLE										,
		QUEST_TEXTURE_RIGHT_MOUSE_OVER									,
		QUEST_TEXTURE_RIGHT_CLICK										,
		QUEST_TEXTURE_MOUSE_OVER										,
		QUEST_TEXTURE_END												,
		QUEST_TEXTURE_COUNT				=	QUEST_TEXTURE_END * 2 - 1	,
	};

	BOOL							SetTexture			( TREEBOX_QUEST_TEXTURE_LABEL eLabel , const char* strFileName , int iSrcSX , int iSrcSY , int iSrcEX , int iSrcEY );
	BOOL							SetTexture			( int iLabel , const char* strFileName , int iSrcSX , int iSrcSY , int iSrcEX , int iSrcEY );
	void							SetTextureOffset	( TREEBOX_QUEST_TEXTURE_LABEL eLabel , int iX , int iY );
	void							SetTextureOffset	( int iLabel , int iX , int iY );

	void							SetQuestSize		( int iXSize , int iYSize );
	void							SetQuestOffset		( int iX , int iY );

	void							SetQuestComplete	( bool bQuestComplete );
	void							SetQuestMiniWindow	( bool bQuestMiniWindow );

	bool							GetQuestComplete	();
	bool							GetQuestMiniWindow	();

	void							SetWindow			( SPWindow* pWindow );

protected:

	SPTexture*						m_pTexture			[ QUEST_TEXTURE_COUNT ];
	RECT							m_rcTextureSrc		[ QUEST_TEXTURE_COUNT ];
	RECT							m_rcTexturePos		[ QUEST_TEXTURE_COUNT ];
	POINT							m_ptTextureSize		[ QUEST_TEXTURE_COUNT ];
	POINT							m_ptTextureOffset	[ QUEST_TEXTURE_COUNT ];

	POINT							m_ptCtrlPos;
	POINT							m_ptCtrlSize;
	POINT							m_ptCtrlOffset;

	bool							m_bRightMouseOver;

	bool							m_bQuestComplete;
	bool							m_bQuestMiniWindow;

	SPWindow*						m_pParentWindow;

};
