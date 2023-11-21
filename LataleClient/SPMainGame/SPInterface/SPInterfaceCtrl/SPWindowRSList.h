// Copyright (C) DURAGON
//********************************************************************
//	filename: 	SPWindowRSList.h
//	created:	2007/9/21   Time:15:36
//	purpose:	
//
//  1. RS 계열의 함수는 URL을 포함할수 있도록 RETURNVALUR 값을 사용하고 있다.
//		RSList에서는 GetReturnValue() 를 통해서 받을 수있도로 셋팅 되어 있다.
//	2. 또다른 특징은 CursorEnter시 폰트 컬러가 바뀌거나 배경 ImgaeBar가 나오도록 설정 가능하다
//		FontColor는 SetSelectFontcolor() 호출 이후 넣어지는 함수
//		또는 InsertItem( RSLIST_ITEM* pListItem )를 통해 넣을때 직접 셋팅해서 넣으면 된다.
//	3. Select ImageBar Setting은 SetSelectBar() 를 호출하면 자동으로 셋팅된다.
//		
//*********************************************************************/


#pragma once

#include "SPWindowListDEF.h"

struct RSLIST_ITEM
{
	bool						m_bUseFontColor ;		//	CursorOver 폰트효과 사용여부
	std::string					m_strText;				//	문자열
	D3DXCOLOR					m_TextColor;			//	랜더링 컬러
	D3DXCOLOR					m_DefTextColor;			//	Default 컬러
	D3DXCOLOR					m_RvsTextColor;			//	Mouse Over 컬러
	LIST_RETURNVALUE			m_stReturnValue;		//	돌려줄 값
	RECT						m_rcDrawPos;			//	전체 영역

	void	Clear()
	{
		m_bUseFontColor = false ;
		m_strText.clear();
		m_TextColor		= D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );
		m_DefTextColor	= D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );
		m_RvsTextColor	= D3DXCOLOR( 0 , 0 , 0 , 1.0f );

		m_rcDrawPos.left	=	0;
		m_rcDrawPos.top		=	0;
		m_rcDrawPos.right	=	0;
		m_rcDrawPos.bottom	=	0;
	}

	RSLIST_ITEM()
	{
		Clear();
	}
	RSLIST_ITEM& operator = (const RSLIST_ITEM& Src)
	{
		m_bUseFontColor = Src.m_bUseFontColor ;
		m_strText		= Src.m_strText ;
		m_TextColor		= Src.m_TextColor;
		m_RvsTextColor	= Src.m_RvsTextColor;
		m_stReturnValue = Src.m_stReturnValue ;

		m_rcDrawPos.left	=	Src.m_rcDrawPos.left;
		m_rcDrawPos.top		=	Src.m_rcDrawPos.top;
		m_rcDrawPos.right	=	Src.m_rcDrawPos.right;
		m_rcDrawPos.bottom	=	Src.m_rcDrawPos.bottom;
		return *this;
	}
};

//////////////////////////////////////////////////////////////////////////

class SPWindow;
class SPWindowRSList :	public SPWindow {
public:
	SPWindowRSList(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent);
	~SPWindowRSList();

	virtual void Init();
	virtual void Clean();
	virtual void Process(float fTime);
	virtual void Render(float fTime);
	virtual void Show(bool bWithChild = true);
	virtual void Refresh();

	void	SetSelectFontcolor(bool bSet)			{	m_bUseFontColor = bSet ;	}
	void	SetSelectBar	(const char* pszImage, RECT rcImageSrc, int iRelPosX, int iRelPosY) ;
	bool	InsertItem		( const char* pszListItem , D3DXCOLOR color, D3DXCOLOR Rvscolor );
	bool	InsertItem		( RSLIST_ITEM* pListItem );
	void	PushBackItem	( RSLIST_ITEM& stListItem );
	void	ClearListItem	();

	LIST_RETURNVALUE*	GetReturnValue( int iXPos , int iYPos );
	RSLIST_ITEM* GetMouseOverItem() ;

	int		ScrollList		( int iSubSpim = SUB_SPIM_SCROLL_LATEST );

	void	SetMargin		( int iLeft , int iTop , int iRight , int iBottom );
	bool	GetLatest		();
	bool	IsAddString		();
	void	SetWheelUse		( bool bUse );
	void	SetNextLine		( bool bUse );
	int		GetTotalLine	()				{	return m_iTotalLineCount ;		}


protected:

	SPIMESSAGE_OBJ_MAP_DECLARE()
		// {{ Message Map	
	virtual int OnCursorEnter	( WPARAM, LPARAM );
	virtual int OnCursorMove	( WPARAM, LPARAM );
	virtual int OnCursorOut		( WPARAM, LPARAM );
	virtual int OnLButtonDown	( WPARAM, LPARAM );
	virtual int OnLButtonUP		( WPARAM, LPARAM );	
	virtual int OnAddItem		( WPARAM wParam , LPARAM lParam);
	virtual int OnScroll		( WPARAM wParam , LPARAM lParam );
	virtual int OnPurge			( WPARAM wParam , LPARAM lParam );
	virtual int OnWheelUp		( WPARAM wParam , LPARAM lParam );
	virtual int OnWheelDown		( WPARAM wParam , LPARAM lParam );
	// }}

protected:

	int					m_iBeginLine;
	int					m_iPrintLineCount;
	int					m_iTotalLineCount;
	RSLIST_ITEM*		m_pEnterItem ;

	bool				m_bUseFontColor ;	// SelectFontColor 사용여부
	bool				m_bUseSelectBar ;	// SelectBar 사용여부
	SPTexture*			m_pBarTexture ;
	RECT				m_rcSelectBarSrc;	// Bar의 소스좌표
	int					m_rcRelPosX;		// Bar의 출력 상대좌표
	int					m_rcRelPosY;		// Bar의 출력 상대좌표
	RECT				m_rcMargin;			// 리스트 사이 간격

	bool				m_bLatest;
	bool				m_bAddString;
	bool				m_bWheelUse;
	bool				m_bNextLine;
	bool				m_bFontShadow ;

	//////////////////////////////////////////////////////////////////////////

	std::vector<RSLIST_ITEM>		m_vListItem;

};
