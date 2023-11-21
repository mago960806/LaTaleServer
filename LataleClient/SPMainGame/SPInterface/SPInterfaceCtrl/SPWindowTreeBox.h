// ***************************************************************
//  SPWindowTreeBox.h   version:  1.0   date:2006/01/13
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2006 AJJIYA - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#pragma once

class SPWindow;
class SPWindowButton;
class SPWindowSlider;
class SPTreeBoxItemBase;

class SPWindowTreeBox : public SPWindow
{
public:

	SPWindowTreeBox								( INSTANCE_ID InstanceID, int iX , int iY , int iCX , int iCY , SPWindow* pParent );
	~SPWindowTreeBox							();

	virtual	void		Init					();
	virtual	void		Clean					();
	virtual	void		Process					( float fTime );
	virtual	void		Render					( float fTime );
	virtual void		Show					( bool bWithChild = true );
	virtual void		Hide					( bool bSendServer = true );
	virtual void		RefreshRelationCoord	();

	//////////////////////////////////////////////////////////////////////////
	//
	//	함수들
	//

	void				SetTexture				( TREEBOX_TEXTURE eTextureType , const char* strFileName , int iSrcSX , int iSrcSY , int iSrcEX , int iSrcEY );
	void				SetBoxTexture			( const char* strFileName , int iSrcSX , int iSrcSY , int iSrcEX , int iSrcEY , int iStep );

	void				UpdateBGPos				();
	void				UpdateScrollPos			();

	bool				IsEmpty					();
	TREEBOX_ITEM*		AddItem					( TREEBOX_ITEM* pParent , SPTreeBoxItemBase* pItemBase );
	TREEBOX_ITEM*		InsertItem				( TREEBOX_ITEM* pParent , SPTreeBoxItemBase* pItemBase , int iPos );
	TREEBOX_ITEM*		GetSelectItem			();
	void				SetSelectItem			( TREEBOX_ITEM* pSelectItem );
	bool				DelItem					( TREEBOX_ITEM** ppItem , bool bParent = false );
	void				DelItemAll				();

	bool				FindItem				( TREEBOX_ITEM* pItem , TREEBOX_ITEM* pStartItem = NULL );

	TREEBOX_ITEM*		GetRootItem				();

	void				Refresh					();

	void				SetMargin				( int iLeft , int iTop , int iRight , int iBottom );
	void				SetInterval				( int iInterval );

	void				SetScrollEnable			( bool bEnable );
	void				SetScrollAutoHide		( bool bAutoHide );
	void				SetScrollMargin			( int iLeft , int iTop , int iRight , int iBottom );
	void				SetScrollUpSize			( int iX , int iY );
	void				SetScrollDownSize		( int iX , int iY );
	void				SetScrollSliderSize		( int iX , int iY );
	void				SetScrollUpTexture		( TREEBOX_DATA_TEXTURE_LABEL eTextureType , const char* strFileName , int iSrcSX , int iSrcSY );
	void				SetScrollDownTexture	( TREEBOX_DATA_TEXTURE_LABEL eTextureType , const char* strFileName , int iSrcSX , int iSrcSY );
	void				SetScrollSliderTexture	( TREEBOX_DATA_TEXTURE_LABEL eTextureType , const char* strFileName , int iSrcSX , int iSrcSY );
	void				SetScrollSliderBGTexture( TREEBOX_DATA_TEXTURE_LABEL eTextureType , const char* strFileName , int iSrcSX , int iSrcSY , int iSrcEX , int iSrcEY );

	int					GetTreeBoxWidth			( int iSizeX );
	int					GetScrollWidth			();

	void				SetDoubleClick			( bool bDoubleClick );

	void				Reset					();

	int					GetItemCount			( bool bRootStart , TREEBOX_ITEM* pItem = NULL );

protected:
	
	SPIMESSAGE_OBJ_MAP_DECLARE()
	// {{ Message Map

	int					OnLButtonUp				( WPARAM wParam, LPARAM lParam );
	int					OnDblLButtonUp			( WPARAM wParam, LPARAM lParam );
	int					OnLButtonDown			( WPARAM wParam, LPARAM lParam );
	int					OnCursorMove			( WPARAM wParam, LPARAM lParam );
	int					OnCursorOut				( WPARAM wParam, LPARAM lParam );
	int					OnWheelUp				( WPARAM wParam, LPARAM lParam );
	int					OnWheelDown				( WPARAM wParam, LPARAM lParam );


	int					OnSliderPage			( WPARAM wParam, LPARAM lParam );
	int					OnSliderCursorEnter		( WPARAM wParam, LPARAM lParam );



	// }}

	int					ProcessItem				( int iCurPos , int iPosX , int iPosY , float fTime );
	TREEBOX_ITEM*		FindRenderItem			( LPARAM lParam );
	void				FindViewItem			( int& iOutCurPos , TREEBOX_ITEM* pStartItem , TREEBOX_ITEM* pCurItem );

	int					CalRenderItemDistance	();

	void				AdjustRenderItemDistance( int iCurPos );

	void				ResetItem				( TREEBOX_ITEM* pStartItem );
	bool				CreateRootItem			();

	//////////////////////////////////////////////////////////////////////////

	enum TREEBOX_INSTANCE_ID
	{
		TREEBOX_BUTTON_UP		=	1		,
		TREEBOX_BUTTON_DOWN					,
		TREEBOX_BUTTON_SLIDER				,
	};

	struct TREEBOX_TEXTURE_INFO
	{
		SPTexture*										m_pTexture;
		RECT											m_rcPos;
		RECT											m_rcSrc;
		POINT											m_ptSize;
	};

	//////////////////////////////////////////////////////////////////////////
	//
	//	변수들
	//

	typedef	std::vector< TREEBOX_TEXTURE_INFO >			STD_VECTOR_TREEBOX_TEXTURE;

	STD_VECTOR_TREEBOX_TEXTURE							m_vBGTexture;

	typedef	std::vector< TREEBOX_ITEM* >				STD_VECTOR_TREEBOX_ITEM;

	STD_VECTOR_TREEBOX_ITEM								m_vpRenderItem;
	STD_VECTOR_TREEBOX_ITEM								m_vpViewItem;


	TREEBOX_ITEM*										m_pRootItem;
	TREEBOX_ITEM*										m_pStartItem;

	TREEBOX_ITEM*										m_pSelectItem;
	TREEBOX_ITEM*										m_pMouseOver;

	bool												m_bUpdate;

	RECT												m_rcMargin;
	int													m_iInterval;

	bool												m_bScrollEnable;
	bool												m_bAutoHide;
	SPWindowButton*										m_pScrollUp;
	SPWindowButton*										m_pScrollDown;
	SPWindowSlider*										m_pScrollSlider;
	RECT												m_rcScrollMargin;

	bool												m_bLButtonDown;

	int													m_iScrollCurPos;

	bool												m_bDoubleClick;

};
