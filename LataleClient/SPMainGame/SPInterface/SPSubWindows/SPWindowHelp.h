// ***************************************************************
//  SPWindowHelp.h   version:  1.0   date:2006/01/13
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2006 AJJIYA - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#pragma once

class SPWindow;
class SPWindowTreeBox;

class SPWindowHelp : public SPWindow
{
public:

	SPWindowHelp								( INSTANCE_ID InstanceID, int iX , int iY , int iCX , int iCY , SPWindow* pParent );
	~SPWindowHelp								();

	virtual	void		Init					();
	virtual	void		Clean					();
	virtual	void		Process					( float fTime );
	virtual	void		Render					( float fTime );
	virtual void		Show					( bool bWithChild = true );
	virtual void		Hide					( bool bSendServer = true );
	virtual void		RefreshRelationCoord	();

	//////////////////////////////////////////////////////////////////////////
	//
	//	계산
	//


protected:

	SPIMESSAGE_OBJ_MAP_DECLARE()
	// {{ Message Map

	virtual	int					OnPurgeAll				( WPARAM wParam, LPARAM lParam );
	virtual	int					OnTreeBoxLButtonUp		( WPARAM wParam, LPARAM lParam );
	virtual	int					OnCloseLButtonUp		( WPARAM wParam, LPARAM lParam );

	// }}

	//////////////////////////////////////////////////////////////////////////

	enum HELP_TEXTURE
	{
		HELP_TEXTURE_LEFT_TOP		=	0	,
		HELP_TEXTURE_CENTER_TOP				,
		HELP_TEXTURE_RIGHT_TOP				,
		HELP_TEXTURE_LEFT_MIDDLE			,
		HELP_TEXTURE_CENTER_MIDDLE			,
		HELP_TEXTURE_RIGHT_MIDDLE			,
		HELP_TEXTURE_LEFT_BOTTOM			,
		HELP_TEXTURE_CENTER_BOTTOM			,
		HELP_TEXTURE_RIGHT_BOTTOM			,
		HELP_TEXTURE_COUNT					,
	};

	enum HELP_INSTANCE_ID
	{
		WIID_HELP_CLOSE						=	WIID_HELP	+	1				,
		WIID_HELP_TITLE															,
		WIID_HELP_TITLE_TEXT													,
		WIID_HELP_LINE_1														,
		WIID_HELP_LINE_2														,
		WIID_HELP_TREEBOX														,
	};

	//	TREEBOX에서는 +3 까지 INSTANCE_ID를 사용한다.!!!

	struct HELP_TEXTURE_INFO
	{
		SPTexture*										m_pTexture;
		RECT											m_rcPos;
		RECT											m_rcSrc;
		POINT											m_ptSize;
	};

	typedef	std::vector< HELP_TEXTURE_INFO >			STD_VECTOR_HELP_TEXTURE;

	struct HELP_CONTENT_INFO
	{
		int												m_iID;
		std::string										m_strTexture;
		POINT											m_ptTexturePos;
		POINT											m_ptTextureSize;
		POINT											m_ptTextureSrc;

		std::string										m_strText;
		POINT											m_ptTextPos;
		POINT											m_ptTextSize;
		D3DXCOLOR										m_cTextColor;
		BOOL											m_bTextBold;
		DWORD											m_dwTextAlign;
	};


	//////////////////////////////////////////////////////////////////////////
	//
	//	함수들
	//

	void				LoadLDTLeftMenu			();
	void				LoadLDTRightContents	();

	void				SetTexture				( HELP_TEXTURE eTextureType , const char* strFileName , int iSrcSX , int iSrcSY , int iSrcEX , int iSrcEY , STD_VECTOR_HELP_TEXTURE& vHelpTexture );
	void				SetBoxTexture			( const char* strFileName , int iSrcSX , int iSrcSY , int iSrcEX , int iSrcEY , int iStep , STD_VECTOR_HELP_TEXTURE& vHelpTexture );

	void				UpdateBGPos				();
	void				UpdateContentPos		();

	void				AddContent				( int iValue , HELP_CONTENT_INFO& stInfoData );
	void				SetContent				( int iValue , SPWindow* pWindow );

	//////////////////////////////////////////////////////////////////////////
	//
	//	변수들
	//

	STD_VECTOR_HELP_TEXTURE								m_vBGTexture;

	SPWindowTreeBox*									m_pTreeBox;

	STD_VECTOR_HELP_TEXTURE								m_vContentTexture;
//	HELP_TEXTURE_INFO									m_stContentGradation;

	typedef std::multimap< int , HELP_CONTENT_INFO >	STD_MAP_HELP_CONTENT;

	STD_MAP_HELP_CONTENT								m_mmContent;

	SPWindow*											m_pContent;
	SPWindow*											m_pClose;

};
