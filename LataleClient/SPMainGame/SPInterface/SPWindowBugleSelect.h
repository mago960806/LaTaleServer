/**
 *	@file SPWindowBugleSelect.h
 *	@brief
		Setting windows for bugle channel.
 *	@author
		jmulrosbok, snda.
 *	@data
		2008-07-11
 */
//--------------------------------------------------
#ifndef SPWINDOW_BUGLE_SELECT_H
#define SPWINDOW_BUGLE_SELECT_H
//--------------------------------------------------
#include <string>
#include <vector>
#include "SPWindowGridEx.h"
class CUIGridEx;
using std::string;
using std::vector;
//--------------------------------------------------
#define SAFE_CALL_FUNC(obj,func)	{if(obj) (obj)->func;}
//--------------------------------------------------
#define WIID_BUGLE_SELECT_ICON			(WIID_BUGLE_SELECT + 1)		// bugle icon
#define WIID_BUGLE_SELECT_PREVIEW		(WIID_BUGLE_SELECT + 2)		// preview image

#define WIID_BUGLE_SELECT_SLIDER		(WIID_BUGLE_SELECT + 3)		// slider
#define WIID_BUGLE_SELECT_SLIDER_UP		(WIID_BUGLE_SELECT + 4)		// slider up button
#define WIID_BUGLE_SELECT_SLIDER_DOWN	(WIID_BUGLE_SELECT + 5)		// slider down button
#define WIID_BUGLE_SELECT_CLOSE			(WIID_BUGLE_SELECT + 6)		// close button

#define WIID_BUGLE_SELECT_TOP			(WIID_BUGLE_SELECT + 7)
#define WIID_BUGLE_SELECT_CENTER		(WIID_BUGLE_SELECT + 8)
#define WIID_BUGLE_SELECT_BOTTOM		(WIID_BUGLE_SELECT + 9)

#define WIID_BUGLE_SELECT_TITLE_IMAGE	(WIID_BUGLE_SELECT + 10)
#define WIID_BUGLE_SELECT_TITLE			(WIID_BUGLE_SELECT + 11)

#define WIID_BUGLE_SELECT_SKIN			(WIID_BUGLE_SELECT + 12)

#define WIID_BUGLE_SELECT_GRID			(WIID_BUGLE_SELECT + 15)

#define WIID_BUGLE_SELECT_TYPES_START	(WIID_BUGLE_SELECT + 100)	// bugle types start
#define BUGLE_TYPES_COUNT				(30)
#define WIID_BUGLE_SELECT_TYPES_END		(WIID_BUGLE_SELECT_TYPES_START + BUGLE_TYPES_COUNT)	// bugle types end

#define WIID_BUGLE_VISUAL_PREVIEW		(WIID_BUGLE_SELECT_TYPES_END + 1)
#define WIID_BUGLE_SELECT_PREVIEW_BG	(WIID_BUGLE_VISUAL_PREVIEW + 1)

#define WIID_PREVIEW_FADEIN				(WIID_BUGLE_SELECT_PREVIEW_BG + 1)
#define WIID_PREVIEW_FADEOUT			(WIID_BUGLE_SELECT_PREVIEW_BG + 2)
//--------------------------------------------------
class SPWindow;
class SPWindowSlider;
class SPWindowGrid;
class SPUIUnitManager;
class SPWindowVisualBugle;
class SPPlayer;
class SPImageProcessor;
//--------------------------------------------------
enum BugleChannelBgType
{
	BUGLE_CHANNELBG_NULL = -1,
	BUGLE_CHANNELBG_RED,
	BUGLE_CHANNELBG_BLUE,
	BUGLE_CHANNELBG_VISIBLE,
	BUGLE_CHANNELBG_WHITE,
	BUGLE_CHANNELBG_GM,

	BUGLE_CHANNELBG_ENUM_NUM
};
struct BugleInfo
{
	int		m_iItemShopID;
	string	m_strName;
	BugleChannelBgType m_channelBgType;


	string	m_strIconFilename;
	int		m_iIconX;
	int		m_iIconY;
	int		m_iIconSizeX;
	int		m_iIconSizeY;
	
	int		m_iColorR;
	int		m_iColorG;
	int		m_iColorB;

	string	m_strBgFilename;
	int		m_iBgX;
	int		m_iBgY;
	int		m_iBgSizeX;
	int		m_iBgSizeY;

	string	m_strBgHeader;
	int		m_iBgHeaderX;
	int		m_iBgHeaderY;
	int		m_iBgHeaderSizeX;
	int		m_iBgHeaderSizeY;

	string	m_strBgTail;
	int		m_iBgTailX;
	int		m_iBgTailY;
	int		m_iBgTailSizeX;
	int		m_iBgTailSizeY;

	string	m_strPreviewFilename;
	int		m_iPreviewX;
	int		m_iPreviewY;
	int		m_iPreviewSizeX;
	int		m_iPreviewSizeY;

	string	m_strVisualFilename;
	int		m_iVisualX;
	int		m_iVisualY;
	int		m_iVisualSizeX;
	int		m_iVisualSizeY;

	int		m_iVisualColorR;
	int		m_iVisualColorG;
	int		m_iVisualColorB;

	int		m_iVisualTextX;
	int		m_iVisualTextY;
	int		m_iVisualTextSizeX;
	int		m_iVisualTextSizeY;

	string	m_strPreviewText;

	BugleInfo( void )
	{
		reset();
	}
	void reset( void )
	{
		m_iItemShopID = -1;
		m_strName = "«Î…Ë÷√";
		m_channelBgType = BUGLE_CHANNELBG_NULL;

		m_strIconFilename = "";
		m_iIconX = 0;
		m_iIconY = 0;
		m_iIconSizeX = 0;
		m_iIconSizeY = 0;

		m_iColorR = 0;
		m_iColorG = 0;
		m_iColorB = 0;

		m_strBgFilename = "";
		m_iBgX = 0;
		m_iBgY = 0;
		m_iBgSizeX = 0;
		m_iBgSizeY = 0;

		m_strPreviewFilename = "";
		m_iPreviewX = 0;
		m_iPreviewY = 0;
		m_iPreviewSizeX = 0;
		m_iPreviewSizeY = 0;

		m_strBgHeader = "";
		m_iBgHeaderX = 0;
		m_iBgHeaderY = 0;
		m_iBgHeaderSizeX = 0;
		m_iBgHeaderSizeY = 0;

		m_strBgTail = "";
		m_iBgTailX = 0;
		m_iBgTailY = 0;
		m_iBgTailSizeX = 0;
		m_iBgTailSizeY = 0;

		m_strVisualFilename = "";
		m_iVisualX = 0;
		m_iVisualY = 0;
		m_iVisualSizeX = 0;
		m_iVisualSizeY = 0;

		m_iVisualColorR = 0;
		m_iVisualColorG = 0;
		m_iVisualColorB = 0;

		m_iVisualTextX = 0;
		m_iVisualTextY = 0;
		m_iVisualTextSizeX = 0;
		m_iVisualTextSizeY = 0;

		m_strPreviewText.clear();
	}
};
//--------------------------------------------------
class BugleItemCell : public UIGridCellEx
{
public:
	BugleItemCell( void );
	BugleItemCell( const char *szImage, const RECT &rtSrc, int iBugleShopID );
	virtual ~BugleItemCell( void );
	virtual bool CellRender( const RECT &rtDst );
	virtual void Destroy( void );
	virtual bool Reset( void );
	virtual RECT GetSize( void ) const;

public:
	void SetTexture( const char *szImage, const RECT &rtSrc );

protected:
	SPTexture	*m_ptrTexture;
	RECT		m_rtSrc;
	int			m_iBugleShopID;
};
//--------------------------------------------------
struct PreviewAnim
{
	static const float SHOWTIMELIMIT;
	SPImageProcessor	*m_pFadeIn;
	SPImageProcessor	*m_pFadeOut;
	bool m_bShowed;
	float	m_fAccuTime;
	PreviewAnim( void )
	{
		m_pFadeIn = NULL;
		m_pFadeOut = NULL;
		m_bShowed = false;
		m_fAccuTime = 0.0f;
	}
	void Release( void );
	~PreviewAnim( void );
};
//--------------------------------------------------
class SPWindowBugleSelect : public SPWindow
{
	friend class SPWindowBugleMenu;
public:
	SPWindowBugleSelect( WND_ID wndClassID, INSTANCE_ID instanceID );
	SPWindowBugleSelect( INSTANCE_ID instanceID, 
		int iX, int iY, int iCX, int iCY, 
		SPWindow *pParent = NULL );
	virtual ~SPWindowBugleSelect( void );

	virtual void Init( void );
	virtual void Clean( void );
	virtual void Show( bool bWithChild = true );
	virtual void Hide( bool bSendServer = true );
	virtual void Process( float fDeltaTime );
	virtual void Render( float fDeltaTime );
	virtual void RefreshRelationCoord( void );

private:
	void _reposOwnImage( void );
	void buildLocalAvatar();
protected:
	SPIMESSAGE_OBJ_MAP_DECLARE()

	/// mouse click, preview bugle
	int _onBuglePreview( WPARAM wParam , LPARAM lParam );
	/// mouse double click, select bugle
	int	_onBugleSelect( WPARAM wParam , LPARAM lParam );

	/// tip
	virtual int _onToolTipEnter( WPARAM wParam, LPARAM lParam );
	virtual int _onToolTipOut( WPARAM wParam, LPARAM lParam );
	virtual int _onToolTipMove( WPARAM wParam, LPARAM lParam );
	
	int	_onBugleItemsPageUp( WPARAM wParam, LPARAM lParam );
	int _onBugleItemsPageDown( WPARAM wParam, LPARAM lParam );
	int _onSliderUp( WPARAM wParam, LPARAM lParam );
	int _onSliderDown( WPARAM wParam, LPARAM lParam );
	int _onSlider( WPARAM wParam, LPARAM lParam );
	int	_onSliderPageNum( WPARAM wParam, LPARAM lParam );
	
	int _onClose( WPARAM wParam, LPARAM lParam );

protected:
	/// try to load bugle type ldt file
	static bool _loadBugleLDT( bool bGM );
	/// initialize grid cell information
	bool _initGrid( void );
	/// initialize gui
	bool _initGUI( void );

protected:
	SPWindowStatic	*m_pImgBugleIcon;
	SPWindowStatic	*m_pImgBuglePreview;
	SPWindowStatic	*m_pImgBuglePreviewBG;
	SPWindowSlider	*m_pSlider;
	SPWindowStatic	*m_pTop;
	CUIGridEx*		m_pGrid;
	SPWindowButton	*m_pbtnClose;
	SPPlayer		*m_ptrPlayer;
	SPWindowVisualBugle	*m_wndVisualBuglePreview;
	
	std::vector< PreviewAnim > m_vecPreviewAnims;
	int	m_FadeInBugleIdx;
	int m_FadeOutBugleIdx;

public:
	static BugleInfo GetBugleInfo( const int itemID );
	static vector< BugleInfo > m_vecBugleInfo;
	static vector< BugleInfo > m_vecGMBugleInfo;
	static int		m_iCurBugleInfoIdx;		//!< current used bugle info.
	bool ReloadBugleLDT( bool bGM );
}; // end of class SPWindowBugleSelect
//--------------------------------------------------
#endif //ifndef SPWINDOW_BUGLE_SELECT_H
//--------------------------------------------------
// end of file
