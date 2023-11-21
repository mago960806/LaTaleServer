#include <vector>
#include <string>
#include "SPCommon.h"
#include "SPUtil.h"
#include "SPMainGameDEF.H"
#include "SPGameObjectDEF.h"
#include "SPSubGameManager.h"
#include "SPGameObject.h"
#include "SPManager.h"
#include "SPMouseCursor.h"
#include "SPIMEPool.h"
#include "SPResourceDef.h"
#include "SPWindowDEF.h"
#include "SPInterfaceManager.h"
#include "SPUIUnit.h"
#include "SPCoolTimeManager.h"
#include "SPItemAttr.h"
#include "SPItem.h"
#include "SPItemCluster.h"
#include "SPWindow.h"
#include "SPWindowGridEx.h"
#ifdef _DEBUG
#include "SPDebug.h"
#endif

#include <vector>
#include <string>
#include <algorithm>
using namespace std;
//--------------------------------------------------
CUIGridEx::CUIGridEx(WND_ID wndClassID, INSTANCE_ID instanceID)
: SPWindow( wndClassID, instanceID )
{
	Init();
}
CUIGridEx::CUIGridEx(WND_ID wndClassID, INSTANCE_ID instanceID, 
					 int iX, int iY, int iCX, int iCY, 
					 SPWindow *pParent, 
					 int iXNum, int iYNum, 
					 int iXGap /* = 0 */, int iYGap /* = 0 */, 
					 int iCellSizeX /* = 0 */, int iCellSizeY /* = 0 */)
					 :SPWindow(wndClassID, instanceID, iX, iY, iCX, iCY, pParent)
{
	Init();
	m_iLineSize = iXNum;
	m_iLineCount = iYNum;
	m_ptCellSize.x = iCellSizeX;
	m_ptCellSize.y = iCellSizeY;
	m_ptGap.x = iXGap;
	m_ptGap.y = iYGap;

	_initGUI();
}
CUIGridEx::~CUIGridEx( void )
{
	if (m_pBgTexture)
		SAFE_DELETE( m_pBgTexture );

	for (int idx = 0; idx < (int)m_vecPtrCells.size(); ++idx)
	{
		if (m_vecPtrCells[idx])
		{
			m_vecPtrCells[idx]->Destroy();
			m_vecPtrCells[idx] = NULL;
		}
	}

	m_vecPtrCells.clear();
	m_vecDestRect.clear();
}
//--------------------------------------------------
void CUIGridEx::Init( void )
{
	m_iLineSize = 0;
	m_iLineCount = 0;
	m_ptCellSize.x = 0;
	m_ptCellSize.y = 0;
	m_ptGap.x = 0;
	m_ptGap.y = 0;	
	m_iStartIdx = 0;
	m_pBgTexture = NULL;
	m_pGridTexture = NULL;
	m_ptGridOffset.x = 0;
	m_ptGridOffset.y = 0;

	memset( &m_rtGridSrc, 0, sizeof( RECT ) );
	memset( &m_rtBgSrc, 0, sizeof( RECT ) );

	m_vecPtrCells.clear();
	m_vecDestRect.clear();
}
//--------------------------------------------------
void CUIGridEx::Clean( void )
{
	if (m_pBgTexture)
		SAFE_DELETE( m_pBgTexture );

	RemoveAllCell();

	SPWindow::Clean();
}
//--------------------------------------------------
void CUIGridEx::Show( void )
{
	SPWindow::Show();
}
//--------------------------------------------------
void CUIGridEx::Process(float fDeltaTime)
{
	RECT targetRect;
	GetWindowRect( targetRect );

	int index = 0;
	int i = 0;
	int j = 0;

	for (i = 0; i < m_iLineCount; ++i)
	{
		for (j = 0; j < m_iLineSize; ++j)
		{
			index = i * m_iLineSize + j;
			RECT rtTarget;
			rtTarget.left = targetRect.left + (j*m_ptGap.x);
			rtTarget.top = targetRect.top + (i*m_ptGap.y);
			rtTarget.right = rtTarget.left + m_ptCellSize.x;
			rtTarget.bottom = rtTarget.top + m_ptCellSize.y;

			SetRect( &m_vecDestRect[index],
				rtTarget.left, rtTarget.top,
				rtTarget.right, rtTarget.bottom );
		}
	}

	SPWindow::Process( fDeltaTime );
}
//--------------------------------------------------
void CUIGridEx::Render(float fDeltaTime)
{
	g_pVideo->Flush();
	SPWindow::Render( fDeltaTime );

	UIGridCellPtrArray::iterator iter = m_vecPtrCells.begin();
	advance( iter, m_iStartIdx );

	RECT targetRect;
	GetWindowRect( targetRect );

	int index = 0;
	int i = 0;
	int j = 0;
	for (i = 0; i < m_iLineCount; ++i)
	{
		for (j = 0; j < m_iLineSize; ++j)
		{
            index = i * m_iLineSize + j;

            if (m_pGridTexture)
			{
				RECT rcDst = m_vecDestRect[index];
				rcDst.left = m_vecDestRect[index].left + m_ptGridOffset.x;
				rcDst.top = m_vecDestRect[index].top + m_ptGridOffset.y;
				rcDst.right = rcDst.left + (m_rtGridSrc.right - m_rtGridSrc.left);
				rcDst.bottom = rcDst.top + (m_rtGridSrc.bottom - m_rtGridSrc.top);

				m_pGridTexture->RenderTexture( &rcDst, &m_rtGridSrc );
				m_pGridTexture->SetColor( RGBA( 255, 255, 255, 255 ) );
			}

			if (iter == m_vecPtrCells.end())
				continue;

			LPUIGridCellEx &ptrCell = (*iter);
			if (!ptrCell)
				break;

			ptrCell->CellRender( m_vecDestRect[index] );
			g_pVideo->Flush();

			if (iter != m_vecPtrCells.end())
			{
				++iter;
			}
		}
	}
}
//--------------------------------------------------
//--------------------------------------------------
bool CUIGridEx::RemoveCell(int iIndex)
{
	m_vecPtrCells.erase( m_vecPtrCells.begin()+iIndex );
	return true;
}
bool CUIGridEx::RemoveAllCell( void )
{
	for (int idx = 0; idx < (int)m_vecPtrCells.size(); ++idx)
	{
		LPUIGridCellEx ptrCell = m_vecPtrCells[idx];
		if (ptrCell)
		{
			SAFE_DELETE( ptrCell );
		}
	}

	m_vecPtrCells.clear();
	return true;
}
//--------------------------------------------------
LPUIGridCellEx CUIGridEx::GetCell( int iIndex )
{
	return m_vecPtrCells.at( iIndex );
}
//--------------------------------------------------
bool CUIGridEx::AddCell( LPUIGridCellEx ptrCell )
{
	m_vecPtrCells.push_back( ptrCell );
	return true;
}
bool CUIGridEx::AddCell( UIGridCellPtrArray vecPtrCells )
{
	for (int idx = 0; idx < (int)vecPtrCells.size(); ++idx)
	{
		m_vecPtrCells.push_back( vecPtrCells[idx] );
	}
	return true;
}
//--------------------------------------------------
int CUIGridEx::GetCellCount( void ) const
{
	return (int)m_vecPtrCells.size();
}
int CUIGridEx::GetDataLines( void ) const
{
	int iLines = 0;
	iLines = (int)m_vecPtrCells.size() / m_iLineSize;
	if (m_vecPtrCells.size()%m_iLineSize != 0)
		iLines++;

	return iLines;
}
//--------------------------------------------------
bool CUIGridEx::ScrollUp( int iLines )
{
	if (iLines < 0 || m_iStartIdx <= 0)
	{
		return false;
	}

	if (m_iStartIdx <= m_iLineSize *iLines)
	{
		m_iStartIdx = 0;
		return true;
	}

	m_iStartIdx -= m_iLineSize * iLines;
	return true;
}
bool CUIGridEx::ScrollDown( int iLines )
{
	if (iLines < 0)
		return false;

	if ((m_iStartIdx + m_iLineCount*m_iLineSize) >= (int)m_vecPtrCells.size())
		return false;

	m_iStartIdx += (m_iLineSize*iLines);
	return true;
}
//--------------------------------------------------
bool CUIGridEx::PageUp( int iPages )
{
	return true;
}
bool CUIGridEx::PageDown( int iPages )
{
	return true;
}
//--------------------------------------------------
//--------------------------------------------------
int CUIGridEx::OnLButtonDown( WPARAM wParam, LPARAM lParam )
{	
	int iX, iY;
	GetAbsCoord(iX, iY);
	int xPos = LOWORD(lParam);
	int yPos = HIWORD(lParam);

	int xRelPos = ( xPos - iX )/m_ptGap.x;
	int yRelPos = ( yPos - iY )/m_ptGap.y;

	int iIndex = (yRelPos * m_iLineSize) + xRelPos + m_iStartIdx;

	return SPSendMessageToParent(SPIM_LBUTTON_DOWN, wParam, (LPARAM)iIndex , this);
}
int CUIGridEx::OnLButtonUP( WPARAM wParam, LPARAM lParam )
{	
	int iX, iY;
	GetAbsCoord(iX, iY);
	int xPos = LOWORD(lParam);
	int yPos = HIWORD(lParam);

	if((xPos - iX >= m_iSX) || (yPos - iY >= m_iSY)) {
		return SPSendMessageToParent(SPIM_LBUTTON_UP, wParam, -1, this);		
	}

	int xRelPos = ( xPos - iX )/m_ptGap.x;
	int yRelPos = ( yPos - iY )/m_ptGap.y;

	int iIndex = (yRelPos * m_iLineSize) + xRelPos + m_iStartIdx;

	return SPSendMessageToParent(SPIM_LBUTTON_UP, wParam, (LPARAM)iIndex , this);
}
int CUIGridEx::OnLButtonDouble( WPARAM wParam, LPARAM lParam )
{
	int iX, iY;
	GetAbsCoord(iX, iY);
	int xPos = LOWORD(lParam);
	int yPos = HIWORD(lParam);

	if((xPos - iX >= m_iSX) || (yPos - iY >= m_iSY)) {
		return SPSendMessageToParent(SPIM_LBUTTON_DBLCLICK, wParam, -1, this);		
	}

	int xRelPos = ( xPos - iX )/m_ptGap.x;
	int yRelPos = ( yPos - iY )/m_ptGap.y;

	int iIndex = (yRelPos * m_iLineSize) + xRelPos + m_iStartIdx;

	return SPSendMessageToParent(SPIM_LBUTTON_DBLCLICK, wParam, (LPARAM)iIndex , this);
}
//--------------------------------------------------
int CUIGridEx::OnRButtonDown( WPARAM wParam, LPARAM lParam )
{
	int iX, iY;
	GetAbsCoord(iX, iY);
	int xPos = LOWORD(lParam);
	int yPos = HIWORD(lParam);

	int xRelPos = ( xPos - iX )/m_ptGap.x;
	int yRelPos = ( yPos - iY )/m_ptGap.y;

	int iIndex = (yRelPos * m_iLineSize) + xRelPos + m_iStartIdx;

	return SPSendMessageToParent(SPIM_RBUTTON_DOWN, wParam, (LPARAM)iIndex , this);
}
int CUIGridEx::OnRButtonUP( WPARAM wParam, LPARAM lParam )
{
	int iX, iY;
	GetAbsCoord(iX, iY);
	int xPos = LOWORD(lParam);
	int yPos = HIWORD(lParam);

	int xRelPos = ( xPos - iX )/m_ptGap.x;
	int yRelPos = ( yPos - iY )/m_ptGap.y;

	int iIndex = (yRelPos * m_iLineSize) + xRelPos + m_iStartIdx;

	return SPSendMessageToParent(SPIM_RBUTTON_UP, wParam, (LPARAM)iIndex , this);
}
//--------------------------------------------------
int CUIGridEx::OnCursorMove( WPARAM wParam, LPARAM lParam )
{
	return 0;
}
int CUIGridEx::OnCursorEnter( WPARAM wParam, LPARAM lParam )
{
	return SPSendMessageToParent(SPIM_CURSOR_ENTER, 0, 0, this);
}
int CUIGridEx::OnCursorOut( WPARAM wParam, LPARAM lParam )
{
	return SPSendMessageToParent(SPIM_CURSOR_OUT, 0, 0, this);
}
//--------------------------------------------------
int CUIGridEx::OnToolTipMove( WPARAM wParam, LPARAM lParam )
{
	int iX, iY;
	GetAbsCoord(iX, iY);
	int xPos = LOWORD(lParam);
	int yPos = HIWORD(lParam);

	int xRelPos = ( xPos - iX )/m_ptGap.x;
	int yRelPos = ( yPos - iY )/m_ptGap.y;
	int iIndex = (yRelPos * m_iLineSize) + xRelPos + m_iStartIdx;

	if(xRelPos >= m_iLineSize || yRelPos >= m_iLineCount || xRelPos < 0 || yRelPos < 0)
	{
		return 0;
	}

	return SPSendMessageToParent(SPIM_TOOLTIP_MOVE, wParam, (LPARAM)iIndex, this);
}
int CUIGridEx::OnToolTipEnter( WPARAM wParam, LPARAM lParam )
{
	int iX, iY;
	GetAbsCoord(iX, iY);
	int xPos = LOWORD(lParam);
	int yPos = HIWORD(lParam);

	int xRelPos = ( xPos - iX )/m_ptGap.x;
	int yRelPos = ( yPos - iY )/m_ptGap.y;
	int iIndex = (yRelPos * m_iLineSize) + xRelPos + m_iStartIdx;

	if(xRelPos >= m_iLineSize || yRelPos >= m_iLineCount || xRelPos < 0 || yRelPos < 0)
	{
		return 0;
	}

	return SPSendMessageToParent(SPIM_TOOLTIP_ENTER, wParam, (LPARAM)iIndex, this);
}
int CUIGridEx::OnToolTipOut( WPARAM wParam, LPARAM lParam )
{
	return SPSendMessageToParent(SPIM_TOOLTIP_OUT, 0, 0, this);
}
//--------------------------------------------------
int CUIGridEx::OnWheelUp( WPARAM wParam, LPARAM lParam )
{
	return SPSendMessageToParent(SPIM_WHEEL_UP, wParam, lParam, this);
}
int CUIGridEx::OnWheelDown( WPARAM wParam, LPARAM lParam )
{
	return SPSendMessageToParent(SPIM_WHEEL_DOWN, wParam, lParam, this);
}
//--------------------------------------------------
//--------------------------------------------------
bool CUIGridEx::_initGUI( void )
{
	m_vecDestRect.reserve( m_iLineCount * m_iLineSize );
	return true;
}
//--------------------------------------------------
bool CUIGridEx::SetGridImage( const char *szImage, int iXSrc, int iYSrc,
				  int iSizeX, int iSizeY )
{
	if(g_pVideo->GetTextureMgr()->GetTexture(szImage, &m_pGridTexture, TEX_INTERFACE, LEVEL_NOW)) {
		m_rtGridSrc.left = iXSrc;
		m_rtGridSrc.top = iYSrc;
		m_rtGridSrc.right = m_rtGridSrc.left + iSizeX;
		m_rtGridSrc.bottom = m_rtGridSrc.top + iSizeY;
		return true;
	}
	return false;
}
bool CUIGridEx::SetGridImageOffset( int iOffsetX, int iOffsetY )
{
	m_ptGridOffset.x = iOffsetX;
	m_ptGridOffset.y = iOffsetY;

	return true;
}
//--------------------------------------------------
SPIMESSAGE_OBJ_MAP_BEGIN	( CUIGridEx )
	SPIMESSAGE_COMMAND(	SPIM_LBUTTON_DOWN,		OnLButtonDown	)
	SPIMESSAGE_COMMAND(	SPIM_LBUTTON_UP,		OnLButtonUP		)

	SPIMESSAGE_COMMAND(	SPIM_RBUTTON_DOWN,		OnRButtonDown	)
	SPIMESSAGE_COMMAND(	SPIM_RBUTTON_UP,		OnRButtonUP		)

	SPIMESSAGE_COMMAND(	SPIM_LBUTTON_DBLCLICK,	OnLButtonDouble )

	SPIMESSAGE_COMMAND( SPIM_CURSOR_MOVE,		OnCursorMove	)
	SPIMESSAGE_COMMAND( SPIM_CURSOR_ENTER,		OnCursorEnter	)
	SPIMESSAGE_COMMAND( SPIM_CURSOR_OUT,		OnCursorOut		)

	SPIMESSAGE_COMMAND( SPIM_TOOLTIP_MOVE,		OnToolTipMove	)
	SPIMESSAGE_COMMAND( SPIM_TOOLTIP_ENTER,		OnToolTipEnter	)
	SPIMESSAGE_COMMAND( SPIM_TOOLTIP_OUT,		OnToolTipOut	)

	SPIMESSAGE_COMMAND(	SPIM_WHEEL_UP,			OnWheelUp		)
	SPIMESSAGE_COMMAND(	SPIM_WHEEL_DOWN,		OnWheelDown		)

	SPIMESSAGE_COMMAND( SPIM_MOVE_BEGIN	,		OnNULL			)
	SPIMESSAGE_COMMAND( SPIM_MOVE_END	,		OnNULL			)
	SPIMESSAGE_COMMAND( SPIM_MOVE		,		OnNULL			)
SPIMESSAGE_OBJ_MAP_END	( SPWindow	)
//--------------------------------------------------
// end of file
