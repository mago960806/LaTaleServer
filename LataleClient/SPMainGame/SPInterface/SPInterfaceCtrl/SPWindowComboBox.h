// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-11-2    14:40 
//***************************************************************************

#pragma once

class SPWindow;
class SPWindowButton;
class SPWindowSlider;
class SPComboBoxItemBase;

class SPWindowComboBox : public SPWindow
{
public:

	SPWindowComboBox							( INSTANCE_ID InstanceID, int iX , int iY , int iCX , int iCY , SPWindow* pParent );
	~SPWindowComboBox							();

	virtual	void		Init					();
	virtual void		InitFromXML(SPWindow_Base* rec_wnd_AttrMap);
	virtual	void		Clean					();
	virtual	void		Process					( float fTime );
	virtual	void		Render					( float fTime );
	virtual void		Show					( bool bWithChild = true );
	virtual void		Hide					( bool bSendServer = true );
	virtual bool		IsCursorIn				( int iX, int iY );
	virtual void		RefreshRelationCoord	();

	void				NoRender				();

	void				PosUpdate				();

	void				SetRenderItemCountMax	( int iCount );

	void				SetListBoxTexture		( COMBOBOX_TEXTURE eTextureType , const char* strFileName , int iSrcSX , int iSrcSY , int iSrcEX , int iSrcEY );
	void				SetListBoxTexture		( const char* strFileName , int iSrcSX , int iSrcSY , int iSrcEX , int iSrcEY , int iStep = 4 );
	void				SetListBoxItemInterval	( int iInterval );
	void				SetListBoxItemMargin	( int iLeft , int iTop , int iRight , int iBottom );
	void				SetListBoxItemLimit		( int iX , int iY );
	void				SetListBoxOffset		( int iX , int iY );

	void				SetDropDownStatus		( COMBOBOX_DATA_TEXTURE_LABEL eType );
	void				SetDropDownOffset		( int iX , int iY );
	void				SetDropDownSize			( int iX , int iY );
	void				SetDropDownTexture		( COMBOBOX_DATA_TEXTURE_LABEL eTextureType , const char* strFileName , int iSrcSX , int iSrcSY );

	void				SetScroll				( BOOL bScroll );
	void				SetScrollDir			( BOOL bScrollDir );
	void				SetScrollOffset			( int iLeft , int iTop , int iRight , int iBottom );
	void				SetScrollUpSize			( int iX , int iY );
	void				SetScrollDownSize		( int iX , int iY );
	void				SetScrollSliderSize		( int iX , int iY );
	void				SetScrollUpTexture		( COMBOBOX_DATA_TEXTURE_LABEL eTextureType , const char* strFileName , int iSrcSX , int iSrcSY );
	void				SetScrollDownTexture	( COMBOBOX_DATA_TEXTURE_LABEL eTextureType , const char* strFileName , int iSrcSX , int iSrcSY );
	void				SetScrollSliderTexture	( COMBOBOX_DATA_TEXTURE_LABEL eTextureType , const char* strFileName , int iSrcSX , int iSrcSY );
	void				SetScrollSliderBGTexture( COMBOBOX_DATA_TEXTURE_LABEL eTextureType , const char* strFileName , int iSrcSX , int iSrcSY , int iSrcEX , int iSrcEY );

	void				SetShowEditBoxText		( BOOL bShowEditBoxText );
	void				SetEditBoxStatus		( COMBOBOX_DATA_TEXTURE_LABEL eType );
	void				SetEditBoxOffset		( int iX , int iY );
	void				SetEditBoxSize			( int iX , int iY );
	void				SetEditBoxItem			( int iNumber );
	void				SetEditBoxTextColor		( D3DXCOLOR stColor );
	void				SetEditBoxTexture		( COMBOBOX_DATA_TEXTURE_LABEL eTextureLabel , COMBOBOX_TEXTURE eTextureType , const char* strFileName , int iSrcSX , int iSrcSY , int iSrcEX , int iSrcEY );
	void				SetEditBoxTexture		( COMBOBOX_DATA_TEXTURE_LABEL eTextureLabel , const char* strFileName , int iSrcSX , int iSrcSY , int iSrcEX , int iSrcEY , int iStep = 4 );

	void				AddItem					( SPComboBoxItemBase* pstData );
	void				AddItemTop				( SPComboBoxItemBase* pstData );
	SPComboBoxItemBase*	GetItem					( int iNumber );
	void				DelItem					( int iNumber );
	void				DelAllItem				();
	int					GetItemCount			();

	int					GetSelectNumber			();
	void				SetSelectNumber			( int iSelectNumber );

	void				SetSelectValueApply		( BOOL bSelectValueApply );
	BOOL				GetSelectValueApply		();

	void				AcceptSelectValueApply	();

	void				SetToolTipShow			( BOOL bToolTipShow );
	BOOL				GetToolTipShow			();

	void				SetListBoxRender		( BOOL bListBoxRender );
	BOOL				GetListBoxRender		();


	//////////////////////////////////////////////////////////////////////////
	//
	//	계산
	//


protected:
	
	SPIMESSAGE_OBJ_MAP_DECLARE()
	// {{ Message Map

	int					OnListBoxLButtonUp		( WPARAM wParam, LPARAM lParam );
	int					OnListBoxLButtonDown	( WPARAM wParam, LPARAM lParam );
	int					OnListBoxCursorMove		( WPARAM wParam, LPARAM lParam );
	int					OnListBoxCursorOut		( WPARAM wParam, LPARAM lParam );

	int					OnListBoxWheelUp		( WPARAM wParam, LPARAM lParam );
	int					OnListBoxWheelDown		( WPARAM wParam, LPARAM lParam );

	int					OnToolTipEnter			( WPARAM wParam, LPARAM lParam );
	int					OnToolTipOut			( WPARAM wParam, LPARAM lParam );
	int					OnToolTipMove			( WPARAM wParam, LPARAM lParam );

	int					OnDropDownButtonUp		( WPARAM wParam, LPARAM lParam );
	int					OnListBoxUpButtonUp		( WPARAM wParam, LPARAM lParam );
	int					OnListBoxDownButtonUp	( WPARAM wParam, LPARAM lParam );

	int					OnListBoxSliderPage		( WPARAM wParam, LPARAM lParam );

	// }}

	void				ProcessRenderInfo		();

	void				UpdateRenderCount		();
	void				UpdateRenderPos			();
	void				UpdateRenderPosLimit	();
	void				UpdateBGPos				();
	void				UpdateScrollPos			();
	void				UpdateComboBoxSize		();
	void				UpdateEditBoxPos		();

	int					GetScrollWidth			( COMBOBOX_SCROLL_DIR eDir );
	int					CheckListBox			( COMBOBOX_DATA_TEXTURE_LABEL eStatus , int iXPos , int iYPos );
	int					GetComboBoxPosY			();

	//////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////
	//
	//	변수들
	//

	enum COMBOBOX_INSTANCE_ID
	{
		EDITBOX_EDIT			=		1	,
		DROPDOWN_BUTTON						,
		LISTBOX_PANEL						,
		LISTBOX_BUTTON_UP					,
		LISTBOX_BUTTON_DOWN					,
		LISTBOX_BUTTON_SLIDER				,
	};

	struct CBB_TEXTURE_INFO
	{
		SPTexture*							m_pTexture;
		RECT								m_rcPos;
		RECT								m_rcSrc;
		POINT								m_ptSize;
	};

	typedef	std::vector< CBB_TEXTURE_INFO >				STD_VECTOR_CBB_TEXTURE;
	typedef	std::vector< CBB_RENDER_INFO >				STD_VECTOR_CBB_RENDER;
	typedef	std::vector< SPComboBoxItemBase* >			STD_VECTOR_CBB_DATA;

	COMBOBOX_DATA_TEXTURE_LABEL							m_eEditBoxStatus;
	STD_VECTOR_CBB_TEXTURE								m_vEditBoxBGTexture[ COMBOBOX_DATA_TEXTURE_COUNT ];
	STD_VECTOR_CBB_TEXTURE								m_vListBoxBGTexture;
	STD_VECTOR_CBB_RENDER								m_vItemRender;
	STD_VECTOR_CBB_DATA									m_vItemData;

	int													m_iItemDataRenderStart;
	int													m_iItemDataRenderCountMax;

	int													m_iItemDataInterval;
	RECT												m_rcItemDataMargin;
	POINT												m_ptItemDataLimit;

	BOOL												m_bListBoxRender;
	POINT												m_ptListBoxOffset;

	BOOL												m_bScrollUser;
	BOOL												m_bScrollData;
	BOOL												m_bScrollDir;
	RECT												m_rcScrollMargin;
	int													m_iScrollSelect;

	SPWindow*											m_pEditText;
	SPWindowButton*										m_pDropDown;
	SPWindowButton*										m_pScrollUp;
	SPWindowButton*										m_pScrollDown;
	SPWindowSlider*										m_pScrollSlider;

	BOOL												m_bShowEditBoxText;

	CBB_RENDER_INFO										m_stSelectItemRender;
	SPComboBoxItemBase*									m_pSelectItemData;

	POINT												m_ptEditBoxOffset;
	POINT												m_ptEditBoxSize;

	BOOL												m_bSelectValueApply;
	BOOL												m_bToolTipShow;

};
