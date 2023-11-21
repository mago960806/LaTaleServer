#ifndef _SPWINDOW_GRID_EX_H
#define _SPWINDOW_GRID_EX_H
//--------------------------------------------------
#include <vector>
//--------------------------------------------------
typedef class tagUIGridCellEx
{
public:
	/// instructin&destruction
	tagUIGridCellEx( void ){};
	virtual ~tagUIGridCellEx( void ){};

	/// render cell
	virtual bool CellRender( const RECT &rtDst ) = 0;
	/// destroy cell
	virtual void Destroy( void ) = 0;
	/// reset cell
	virtual bool Reset( void ) = 0;
	/// get cell size
	virtual RECT GetSize( void ) const = 0;
} UIGridCellEx, *LPUIGridCellEx;
//--------------------------------------------------
typedef std::vector< LPUIGridCellEx > UIGridCellPtrArray;
typedef std::vector< RECT > RectArray;
class SPWindow;
//--------------------------------------------------
class CUIGridEx : public SPWindow
{
	//--------------------------------------------------
public:
	CUIGridEx( WND_ID wndClassID, INSTANCE_ID instanceID );

	/**
	 *	@param iXNum	[in]	Column count
	 *	@param iYNum	[in]	Line count
	 *	@param iCellSizeX	[in]	cell size x
	 *	@param iCellSizeY	[in]	cell size y
	 *	@param iXGap	[in]	Gap size X
	 *	@param iYGap	[in]	Gap size Y
	 */
	CUIGridEx( WND_ID wndClassID, INSTANCE_ID instanceID,
		int iX, int iY, int iCX, int iCY, 
		SPWindow *pParent,
		int iXNum, int iYNum,
		int iXGap = 0, int iYGap = 0,
		int iCellSizeX = 0, int iCellSizeY = 0);

    virtual ~CUIGridEx( void );
	virtual void Init( void );
	virtual void Clean( void );
	virtual void Show( void );
	virtual void Process( float fDeltaTime );
	virtual void Render( float fDeltaTime );
	//--------------------------------------------------
public:
	/// remove appointed cell
	bool RemoveCell( int iIndex );
	/// remove all cells
	bool RemoveAllCell( void );
	/// get appointed cell
	LPUIGridCellEx	GetCell( int iIndex );
	/// add cell
	bool AddCell( LPUIGridCellEx ptrCell );
	bool AddCell( UIGridCellPtrArray vecPtrCells );
	/// get cell count
	int GetCellCount( void ) const;
	/// get grid data lines
	int GetDataLines( void ) const;
	/// get grid size
	int GetGridLineSize( void ) const
	{
		return m_iLineSize;
	}
	int GetGridLineCount( void ) const
	{
		return m_iLineCount;
	}
	/// scorll up lines
	bool ScrollUp( int iLines = 1 );
	/// scorll down lines
	bool ScrollDown( int iLines = 1 );
	/// page up
	bool PageUp( int iPages = 1 );
	/// page down
	bool PageDown( int iPages = 1 );
	/// grid image set
	bool SetGridImage( const char *szImage, int iXSrc = 0, int iYSrc = 0,
		int iSizeX = 32, int iSizeY = 32 );
	/// grid image offset
	bool SetGridImageOffset( int iOffsetX, int iOffsetY );
	//--------------------------------------------------
protected:
	SPIMESSAGE_OBJ_MAP_DECLARE();
	virtual int OnLButtonDown( WPARAM, LPARAM );
	virtual int OnLButtonUP	( WPARAM, LPARAM );
	virtual int OnLButtonDouble( WPARAM, LPARAM );

	virtual int OnRButtonDown( WPARAM, LPARAM );
	virtual int OnRButtonUP	( WPARAM, LPARAM );

	virtual int OnCursorMove( WPARAM, LPARAM );
	virtual int OnCursorEnter( WPARAM, LPARAM );
	virtual int OnCursorOut	( WPARAM, LPARAM );

	virtual int OnToolTipMove( WPARAM, LPARAM );
	virtual int OnToolTipEnter( WPARAM, LPARAM );
	virtual int OnToolTipOut( WPARAM, LPARAM );

	virtual int OnWheelUp( WPARAM, LPARAM );
	virtual int OnWheelDown	( WPARAM, LPARAM );
	//--------------------------------------------------
protected:
	bool _initGUI( void );
	//--------------------------------------------------
protected:
	int	m_iLineSize;	//!< items each line
	int	m_iLineCount;	//!< lines
	POINT m_ptGap;		//!< gap size
	POINT m_ptCellSize;	//!< each cell size

	int m_iStartIdx;	//!< index of start item in vecPtrCells
	
	SPTexture	*m_pBgTexture;	//!< backgroud texture, if needed
	RECT		m_rtBgSrc;

	SPTexture	*m_pGridTexture;	//!< texture for grid
	RECT		m_rtGridSrc;
	POINT		m_ptGridOffset;

	UIGridCellPtrArray	m_vecPtrCells;
	RectArray m_vecDestRect;
};
#endif

