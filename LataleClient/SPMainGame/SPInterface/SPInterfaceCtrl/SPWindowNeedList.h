// Copyright (C) duragon
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : duragon 2007-08-24 오전 10:26:19
//***************************************************************************


#pragma once

//////////////////////////////////////////////////////////////////////////
#define DEFAULT_MARGIN			8			// Default Margin 8

//////////////////////////////////////////////////////////////////////////
//
struct LIST_NEEDITEM
{
	int					m_ItemID ;				// ItemID (if Item)
	int					m_ItemCount ;			// Item Count
	int					m_iRelSX ;				// 이미지 시작좌표(상대좌표)
	SPTexture*			m_pImage ;				// 렌더할 텍스처
	RECT				m_rtSrc ;				// 텍스쳐 소스 좌표
	RECT				m_rtDstImage ;			// 이미지 출력좌표

	std::string			m_strText ;				// 문자열 항목
	D3DXCOLOR			m_colText ;				// 글자색
	int					m_iStrLength ;			// 글자 길이
	RECT				m_rtDstText ;			// 텍스트 출력좌
	
	LIST_NEEDITEM()
	{
		Clear();
	}
	~LIST_NEEDITEM()
	{
		SAFE_RELEASE(m_pImage) ;
		Clear();
	}

	void	Clear()
	{
		m_ItemID	= 0 ;
		m_ItemCount = 0 ;
		m_iRelSX	= 0 ;
		m_pImage	= NULL ;
		SetRect(&m_rtSrc, 0, 0, 0, 0);
		SetRect(&m_rtDstImage, 0, 0, 0, 0);
		
		m_strText.clear() ;
		m_colText = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) ;
		m_iStrLength = 0 ;
		SetRect(&m_rtDstText, 0, 0, 0, 0) ;
	}

	bool SetImage(const char* szImage, int iSrcX, int iSrcY, int iSrcEX, int iSrcEY)
	{
		std::string strResource = szImage;

		if( strResource.empty() == true )
			return false;

		if( g_pVideo->GetTextureMgr()->LoadTexture(strResource.c_str(), &m_pImage) == false )
			return false;

		SetRect(&m_rtSrc, iSrcX, iSrcY, iSrcEX, iSrcEY) ;
		return true ;
	}
	bool SetImage(const char* szImage, RECT rtSrc)
	{
		std::string strResource = szImage;

		if( strResource.empty() == true )
			return false;

		if( g_pVideo->GetTextureMgr()->LoadTexture(strResource.c_str(), &m_pImage) == false )
			return false;

		SetRect(&m_rtSrc, rtSrc.left, rtSrc.top, rtSrc.right, rtSrc.bottom) ;
		return true ;
	}
	bool SetImage(SPTexture* pImage, RECT rtSrc)
	{
		if( pImage == NULL )
			return false;

		m_pImage = pImage ;

		SetRect(&m_rtSrc, rtSrc.left, rtSrc.top, rtSrc.right, rtSrc.bottom) ;
		return true ;
	}

	bool SetText(const char* strText)
	{
		if( strText == NULL )
			return false ;
		m_strText = strText ;
		return true ;
	}

	D3DXCOLOR SetTextColor(float fR, float fG, float fB, float fA)
	{
		m_colText = D3DXCOLOR(fR, fG, fG, fA) ;
		return m_colText ;
	}
	D3DXCOLOR SetTextColor(D3DXCOLOR dColor)
	{
		m_colText = dColor ;
		return m_colText ;
	}
};


//////////////////////////////////////////////////////////////////////////
//
class SPWindow;
class SPWindowNeedList :	public SPWindow {
public:
	SPWindowNeedList(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent);
	~SPWindowNeedList();

	virtual void Init();
	virtual void Clean();
	virtual void Process(float fTime);
	virtual void Render(float fTime);

	virtual void	RefreshRelationCoord();

	void ClearListItem() ;

	bool InsertItem( const char* pszImage, RECT rcImageSrc, const char* pszItemText , D3DXCOLOR color = D3DXCOLOR(0, 0, 0, 1.0f) ) ;
	bool InsertItem( int iItemID, int iItemCount) ;		// Default ItemAttr->m_eRareLimit Color (default Black)
	
	bool SetBGImage(const char* pszImage, RECT rcImageSrc ) ;
	void SetMargin( int iWidthMargin, int iHeightMargin ) ;

protected:
	D3DXCOLOR GetItemColor(int iLareLimit) ;
	bool PushBackItem( LIST_NEEDITEM* pListItem ) ;
	void ReposeStartPos() ;
	void ReposOwnImage() ;
	
	SPIMESSAGE_OBJ_MAP_DECLARE()
	// {{ Message Map	
	virtual int OnPurge			( WPARAM wParam , LPARAM lParam );
	// }}

protected:
	RECT		m_rcBGSrc;
	SPTexture*	m_pTextureBG ;
	SPTexture*	m_pTextureNum ;		// Item Count Texture
	RECT		m_rcNumSrc;

	bool	m_bDrawLine ;			// Guide Line Render
	int		m_iImageSX ;			// 이미지의 시작 위치
	int		m_iImageMAXWidth ;		// 이미지 영역 Width (Height는 개별)
	int		m_iTextSX ;				// Text의 시작 위치
	int		m_iTextMAXWidth ;		// Text 영역의 Width
	int		m_iWidthMargin ;		// Image와 Text의 Margin
	int		m_iHeightMargin ;

	//////////////////////////////////////////////////////////////////////////
	std::vector<LIST_NEEDITEM*>		m_vpNeedItem;

};
