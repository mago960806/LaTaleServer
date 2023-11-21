
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
#include "SPItemCluster.h"			//[2005/6/16] - jinhee
#include "SPWindow.h"
#include "SPWindowGrid.h"

#include "SPDebug.h"

SPWindowGrid::SPWindowGrid(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
{		
	Init();
}

SPWindowGrid::SPWindowGrid(WND_ID WndClassID, INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent, int iXNum, int iYNum, int iXGap, int iYGap,int iCellSize)
: SPWindow(WndClassID, InstanceID, iX, iY, iCX, iCY, pParent)
{	
	Init();
	AdjustCoord(iX, iY, iCX, iCY);
	init(iXNum, iYNum , iXGap, iYGap, iCellSize);
}

SPWindowGrid::~SPWindowGrid()
{
	Clean();
}

void SPWindowGrid::Init()
{	
	m_bDisable		= false;
	m_bPushed		= false;	
	//m_bPick			= false;

	m_iItemPushPosX = -1;
	m_iItemPushPosY = -1;
	m_bClickMove	= false;
	
	m_pGridTexture = NULL;
	m_pTexture  = NULL;
	std::string strTextureFile = "";
	//strTextureFile = RES_WORK_RESOURCE_PATH;
	strTextureFile = "DATA/INTERFACE/CONCEPT/UI100.PNG";
	g_pVideo->GetTextureMgr()->LoadTexture(strTextureFile.c_str(), &m_pTexture);
	
	SetRect(&m_rtSrc, 434 , 37 , 436 , 39);
	SetRect(&m_rtMarkSrc[0], 479, 152, 511, 184);		//ICON_MARK_LOCK
	SetRect(&m_rtMarkSrc[1], 479, 185, 511, 217);		//ICON_MARK_FASHION
	SetRect(&m_rtMarkSrc[2], 479, 339, 511, 371);		//ICON_MARK_EVENT
	SetRect(&m_rtMarkSrc[3], 479, 372, 511, 404);		//ICON_MARK_FANCY

	m_ptGridOffset.x = m_ptGridOffset.y = 0;
}


void SPWindowGrid::init(int iXNum, int iYNum, int iXGap, int iYGap, int iCellSize) // x,y 갯수, x,y차이(pixel)
{
	//m_ptNum.x = iXNum;
	//m_ptNum.y = iYNum;
	m_iLineSize = iXNum;
	m_iLineCount = iYNum;
	m_iShowCount = m_iLineCount;
	
	if(iXGap==0 || iYGap==0)
	{
		RECT rtWindowSize;
		GetWindowRect( rtWindowSize );
		m_ptGap.x = (rtWindowSize.right-rtWindowSize.left)/iXNum;
		m_ptGap.y = (rtWindowSize.bottom-rtWindowSize.top)/iYNum;
	}
	else
	{
		m_ptGap.x = iXGap;
		m_ptGap.y = iYGap;
	}

	m_nCellSize = (iCellSize==0)?m_ptGap.x:iCellSize;	

	RECT TarRect;
	GetWindowRect(TarRect);

	// default setting
	for( int i=0; i<iYNum; i++)
	{
		for(int j=0; j<iXNum; j++)
		{
			GRIDCELL grid;
			grid.CellClear();
			vGRID.push_back(grid);
			
			RECT rtDest;
			rtDest.left = TarRect.left + (j*m_ptGap.x);
			rtDest.top = TarRect.top + (i*m_ptGap.y);
			rtDest.right = rtDest.left + m_nCellSize;
			rtDest.bottom = rtDest.top + m_nCellSize;
			int iIndex = i * m_iLineSize + j;
			//SetRect(&m_vDestRect[iIndex], left, top, right, bottom);
			m_vDestRect.push_back(rtDest);
			m_vCoolRect.push_back(rtDest);
		}
	}
}


bool SPWindowGrid::SetGridImage(const char* szImage, int iXSrc/*=0*/, int iYSrc/*=0*/, int iSizeX/*=32*/, int iSizeY/*=32*/)
{	
	std::string strResource;

	//strResource = RES_WORK_RESOURCE_PATH;
	strResource = szImage;

	if(g_pVideo->GetTextureMgr()->LoadTexture(strResource.c_str(), &m_pGridTexture)) {
		m_rtGridSrc.left = iXSrc;
		m_rtGridSrc.top = iYSrc;
		m_rtGridSrc.right = m_rtGridSrc.left + iSizeX;
		m_rtGridSrc.bottom = m_rtGridSrc.top + iSizeY;
		return true;
	}
	return false;
}

void SPWindowGrid::SetGridImageOffset(int iOffsetX, int iOffsetY)
{
	m_ptGridOffset.x = iOffsetX;
	m_ptGridOffset.y = iOffsetY;
}


void SPWindowGrid::Clean()
{
	SAFE_RELEASE(m_pTexture);
	m_pTexture = NULL;	
	
	SPWindow::Clean();
}


void SPWindowGrid::Show()
{
	if(m_bShow == false) {
		
	}
	SPWindow::Show();
}


void SPWindowGrid::Process(float fTime)
{
	if(m_bShow){
		std::vector< GRIDCELL >::iterator iter = vGRID.begin();

		RECT TarRect;
		GetWindowRect(TarRect);
		int iIndex = -1;

		if( iter != vGRID.end() )
		{
			//for( int i=0; i<m_ptNum.y; i++)
			for( int i = 0; i < m_iLineCount; i++)
			{
				//for(int j = 0; j < m_ptNum.x; j++)
				for(int j = 0; j < m_iLineSize; j++)
				{
					////
					//if( (*iter).m_iUID==0 )
					//{
					//	++iter;
					//	continue;
					//}

					iIndex = i * m_iLineSize + j;		

					// 안좋은...
					RECT rtTarget;
					rtTarget.left = TarRect.left + (j*m_ptGap.x);
					rtTarget.top = TarRect.top + (i*m_ptGap.y);
					rtTarget.right = rtTarget.left+m_nCellSize;
					rtTarget.bottom = rtTarget.top+m_nCellSize;

					SetRect(&m_vDestRect[iIndex], rtTarget.left, rtTarget.top, rtTarget.right, rtTarget.bottom);
					SetRect(&m_vCoolRect[iIndex], rtTarget.left, rtTarget.top, rtTarget.right, rtTarget.bottom);	
				}
			}
		}
		
		CoolTimeProcess(fTime);
	}
	
	SPWindow::Process(fTime);
}


void SPWindowGrid::CoolTimeProcess(float fTime)
{	
	SPCoolTimeManager*	pCoolTimeManager	=	SPCoolTimeManager::GetInstance();	
	
	std::vector< GRIDCELL >::iterator iter = vGRID.begin();
	int iIndex = 0;
	for(; iter != vGRID.end(); ++iter, iIndex++) {
		(*iter).m_pCoolTime = NULL;
		SPItemAttr* pItemAttr = NULL;		
		int iCoolTimeID = -1;
		
		if(g_pItemCluster->GetItemInfo((*iter).m_iUID, pItemAttr) ) {
			iCoolTimeID = pItemAttr->m_iItemCoolTimeID;
		}

		if(iCoolTimeID < 1)			continue;		
		
		(*iter).m_pCoolTime = pCoolTimeManager->GetCoolTime(iCoolTimeID);
		
		if( (*iter).m_pCoolTime == NULL ) {
			(*iter).DelStatus( STATUS_UI_UNIT_COOLTIME );			
		}
		else
		{
			(*iter).AddStatus( STATUS_UI_UNIT_COOLTIME );
		}		
	}

	iter = vGRID.begin();
	int iRenderIndex = 0;
	for(; iter != vGRID.end(); ++iter, iRenderIndex++) {
		if((*iter).m_pCoolTime) {
			float fRatio = ((*iter).m_pCoolTime->fAccmulateTime / (*iter).m_pCoolTime->fCoolTime) * 100;
			int iHeight = m_vDestRect[iRenderIndex].bottom - m_vDestRect[iRenderIndex].top;
			int	iSrcY = iHeight * (fRatio / 100);
			m_vCoolRect[iRenderIndex].top = m_vDestRect[iRenderIndex].top + iSrcY;
		}
	}
}


void SPWindowGrid::Render(float fTime)
{

	g_pVideo->Flush();

	SPWindow::Render(fTime);
	
	RECT TarRect;
	GetWindowRect(TarRect);
	
	std::vector< GRIDCELL >::iterator iter = vGRID.begin();
	int iIndex = 0;	
	
	//g_pVideo->Flush();		
	
	//
	if( iter != vGRID.end() )
	{		
		//for( int i=0; i<m_ptNum.y; i++)
		//for( int i = 0; i < m_iLineCount; i++)
		for( int i = 0; i < m_iShowCount; i++)
		{
			//for(int j = 0; j < m_ptNum.x; j++)
			for(int j = 0; j < m_iLineSize; j++)
			{
				iIndex = i * m_iLineSize + j;
				
				if(m_pGridTexture) {
					if((*iter).m_iUID == 0) {
						m_pGridTexture->SetColor(RGBA(255, 255, 255, 128));
					}
					
					RECT rcDst = m_vDestRect[iIndex];
					if( m_ptGridOffset.x || m_ptGridOffset.y )
					{
						rcDst.left = m_vDestRect[iIndex].left + m_ptGridOffset.x;
						rcDst.top = m_vDestRect[iIndex].top + m_ptGridOffset.y;
						rcDst.right = rcDst.left + (m_rtGridSrc.right - m_rtGridSrc.left);
						rcDst.bottom = rcDst.top + (m_rtGridSrc.bottom - m_rtGridSrc.top);
					}
					
					m_pGridTexture->RenderTexture(&rcDst, &m_rtGridSrc);
					m_pGridTexture->SetColor(RGBA(255, 255, 255, 255));
				}

				
				//
				if( (*iter).m_iUID == 0 || (*iter).m_iStackCount == 0)
				{
					++iter;
					continue;
				}				
				
				RECT rtSrc;			
				

				rtSrc.left = (*iter).m_ptSrcPos.x ;
				rtSrc.top = (*iter).m_ptSrcPos.y ;
				rtSrc.right = rtSrc.left + m_nCellSize ;
				rtSrc.bottom = rtSrc.top + m_nCellSize ;
				
				float fR, fG, fB, fA;
				fR = fG = fB = fA = 1.0f;
				
				g_pVideo->Flush();	
				
				if((*iter).m_pImage) {					
					if((*iter).m_CellShow ==  CELL_SHOW_DISABLE) {			//팔기위해 선택한 경우
						fA = 0.6f;
					}

					//if( !(*iter).m_bEnable )
					//{
					//	fR = 0.7f;
					//	fG = 0.3f;
					//	fB = 0.3f;
					//}					
					(*iter).m_pImage->SetColor(D3DXCOLOR(fR, fG, fB, fA));
					//(*iter).m_pImage->RenderTexture(&rtTarget, &rtSrc);
					(*iter).m_pImage->RenderTexture(&m_vDestRect[iIndex], &rtSrc);
					(*iter).m_pImage->SetColor();					
				}

				g_pVideo->Flush();

				//// Stack Count
				//RECT rtNumTarget, rtNumSrc;
				//if( (*iter).m_nCellCnt >1 )
				//{
				//	rtNumTarget.left = rtTarget.right-18;
				//	rtNumTarget.top = rtTarget.bottom-10;
				//	rtNumTarget.right = rtNumTarget.left+9;
				//	rtNumTarget.bottom = rtNumTarget.top+10;
				//	
				//	rtNumSrc.left = (((*iter).m_nCellCnt)/10)*9;	
				//	rtNumSrc.right = rtNumSrc.left+9;
				//	rtNumSrc.top=0;
				//	rtNumSrc.bottom=10;

				//	if( (((*iter).m_nCellCnt)/10) > 0 )
				//	{						
				//		g_pItemCluster->GetNumTexture()->RenderTexture( &rtNumTarget , &rtNumSrc);	// 10자리
				//	}			

				//	rtNumSrc.left = (((*iter).m_nCellCnt)%10)*9;	rtNumSrc.right = rtNumSrc.left+9;
				//	rtNumTarget.left = rtNumTarget.left+9;			rtNumTarget.right = rtNumTarget.left+9;
				//	g_pItemCluster->GetNumTexture()->RenderTexture( &rtNumTarget , &rtNumSrc);	// 1자리
				//}			
				
				if( (*iter).m_iUID > 0 && (*iter).m_iStackCount > 0) {
					//[2006/2/10] 비활성 표현
					if((*iter).GetStatus(STATUS_UI_UNIT_COOLTIME) == TRUE)
					{
						if(m_pTexture) {
							m_pTexture->RenderTexture( &m_vCoolRect[iIndex] , &m_rtSrc );
							g_pVideo->Flush();
						}					
					}

					//
					if( !(*iter).m_bEnable  || (*iter).GetStatus(STATUS_UI_UNIT_DISABLE) == TRUE) {//사용할수 없는 아이콘 Diable...
						if(m_pTexture){
							g_pVideo->Flush();
							//m_pTexture->RenderTexture(&rtTarget, &m_rtSrc);
							m_pTexture->RenderTexture(&m_vDestRect[iIndex], &m_rtSrc);
						}
					}
					
					
					// Rev Stack Count
					if((*iter).m_iStackCount > 99) {
						//
					}
					else if((*iter).m_iStackCount > 9) {
						RECT rtTenNumTarget, rtNumTarget, rtTenNumSrc, rtNumSrc;
						int iTenStack = (*iter).m_iStackCount / 10;
						int iOneStack = (*iter).m_iStackCount % 10;
						//SetRect(&rtTenNumTarget, rtTarget.left, rtTarget.bottom - 10, rtTarget.left + 9, rtTarget.bottom);
						//SetRect(&rtNumTarget, rtTenNumTarget.right, rtTarget.bottom - 10, rtTenNumTarget.right + 9, rtTarget.bottom);

						SetRect(&rtTenNumTarget, m_vDestRect[iIndex].left, m_vDestRect[iIndex].bottom - 10, m_vDestRect[iIndex].left + 9, m_vDestRect[iIndex].bottom);
						SetRect(&rtNumTarget, rtTenNumTarget.right - 2, m_vDestRect[iIndex].bottom - 10, rtTenNumTarget.right + 9 - 2, m_vDestRect[iIndex].bottom);

						rtTenNumSrc.left = iTenStack * 9;
						rtTenNumSrc.right = rtTenNumSrc.left + 9;
						rtTenNumSrc.top = 0;
						rtTenNumSrc.bottom = 10;

						rtNumSrc.left = iOneStack * 9;
						rtNumSrc.right = rtNumSrc.left + 9;
						rtNumSrc.top = 0;
						rtNumSrc.bottom = 10;

						g_pItemCluster->GetNumTexture()->RenderTexture( &rtTenNumTarget , &rtTenNumSrc);
						g_pItemCluster->GetNumTexture()->RenderTexture( &rtNumTarget , &rtNumSrc);
					}
					else if((*iter).m_iStackCount > 1)	{
						int iOneStack = (*iter).m_iStackCount % 10;
						RECT rtNumTarget, rtNumSrc;
						//SetRect(&rtNumTarget, rtTarget.left, rtTarget.bottom - 10, rtTarget.left + 9, rtTarget.bottom);
						SetRect(&rtNumTarget, m_vDestRect[iIndex].left, m_vDestRect[iIndex].bottom - 10, m_vDestRect[iIndex].left + 9, m_vDestRect[iIndex].bottom);

						rtNumSrc.left = iOneStack * 9;
						rtNumSrc.right = rtNumSrc.left + 9;
						rtNumSrc.top = 0;
						rtNumSrc.bottom = 10;

						g_pItemCluster->GetNumTexture()->RenderTexture( &rtNumTarget , &rtNumSrc);
					}

					//[2006/2/10] IconMark
					if((*iter).m_iMark > ICON_MARK_NULL) {
						g_pVideo->Flush();						
						m_pTexture->RenderTexture(&m_vDestRect[iIndex], &m_rtMarkSrc[(int)( (*iter).m_iMark - 1 )] );
					}
				}
				
				g_pVideo->Flush();							//[2005/11/23]
				++iter;
			}
		}
	}
}

//bool	SPWindowGrid::SettingIconImage( SPTexture* pImage, int nItemUID, int nXSrc, int nYSrc, int nXPos, int nYPos, UINT8 nCnt)
bool SPWindowGrid::SettingIconImage( SPTexture* pImage, int iItemUID,
									int iXSrc, int iYSrc, int iXPos, int iYPos,
									UINT8 iStackCount, bool bEnable)
{
	int iIndex = (iYPos * m_iLineSize) + iXPos;
	if(iIndex < 0 || iIndex >= m_iLineSize * m_iLineCount) 
		return false;

	vGRID.at(iIndex).SetCellData(pImage, iItemUID, iXSrc, iYSrc, iStackCount, bEnable);	

	return true;
}

//bool	SPWindowGrid::SettingIconImage( SPTexture* pImage, int nItemUID, int nXSrc, int nYSrc, SLOT_INDEX nIndex, UINT8 nCnt)
bool SPWindowGrid::SettingIconImage( SPTexture* pImage, int iItemUID,
									int iXSrc, int iYSrc, SLOT_INDEX iIndex, 
									UINT8 iStackCount, bool bEnable)
{
	if(iIndex < 0 || iIndex >= m_iLineSize * m_iLineCount) 
		return false;
	
	vGRID.at(iIndex).SetCellData(pImage, iItemUID, iXSrc, iYSrc, iStackCount, bEnable);	

	return true;
}

bool	SPWindowGrid::ClearCellAll()
{
	////for( int i=0; i<m_ptNum.y; i++)
	//for( int i = 0; i < m_iLineCount; i++)
	//{
	//	//for(int j=0; j<m_ptNum.x; j++)
	//	for(int j = 0; j < m_iLineSize; j++)
	//	{
	//		ClearCell( j, i );
	//	}
	//}	
	
	int iGridSize = m_iLineCount * m_iLineSize;
	for(int i = 0; i < iGridSize ; i++) {
		ClearCell(i);
	}

	std::vector<GRIDCELL>::iterator iter = vGRID.begin();
	for(; iter != vGRID.end(); ++iter) {
		(iter)->CellClear();
	}

	Render(0.0f);

	return true;
}


bool SPWindowGrid::ClearCell( int iIndex )
{
	if(iIndex < 0 || iIndex >= m_iLineSize * m_iLineCount) 
		return false;

	vGRID.at(iIndex).CellClear();

	//if(iIndex == 0) {
	//	Render(0.0f);
	//}

	return true;
}


////bool	SPWindowGrid::ClearCell( int nXPos, int nYPos )				//[2005/5/20]
//bool SPWindowGrid::ClearCell( int iXPos, int iYPos )
//{
//	int iIndex = (iYPos * m_iLineSize) + iXPos;
//	if(iIndex < 0 || iIndex >= m_iLineSize * m_iLineCount) 
//		return false;
//
//	vGRID.at(iIndex).CellClear();
//
//	return true;
//}

//
//bool	SPWindowGrid::SetItemStack(int iIndex, UINT8 nCnt)
bool	SPWindowGrid::SetItemStack(int iIndex, UINT8 iStackCount)
{
	if(iIndex < 0 || iIndex >= static_cast<int>(vGRID.size()))
		return false;

	if( vGRID.at( iIndex ).m_iUID != 0 ) {
		vGRID.at( iIndex ).m_iStackCount = iStackCount;
	}	
	return true;
}

//
////bool	SPWindowGrid::CellSelectSell( int nXPos, int nYPos, bool bSelect /*= true*/ )		//[2005/5/20]
//bool SPWindowGrid::CellSelectSell( int iXPos, int iYPos, bool bSelect /*= true*/)
//{
//	if(bSelect) {
//		vGRID.at( (iYPos * m_iLineSize) + iXPos ).m_CellShow = CELL_SHOW_DISABLE;
//	}
//	else {
//		vGRID.at( (iYPos * m_iLineSize) + iXPos ).m_CellShow = CELL_SHOW_ENABLE;
//	}
//	return bSelect;
//}


bool	SPWindowGrid::CellSelectSell( int iIndex, bool bSelect /*= true*/)
{
	if(iIndex < 0 || iIndex >= static_cast<int>(vGRID.size()))
		return false;
	
	if(bSelect) {
		vGRID.at( iIndex ).m_CellShow = CELL_SHOW_DISABLE;
	}
	else {
		vGRID.at( iIndex ).m_CellShow = CELL_SHOW_ENABLE;
	}
	
	return bSelect;
}


bool	SPWindowGrid::CellEnable(int iIndex, bool bAble /*= true*/)
{
	if(iIndex < 0 || iIndex >= static_cast<int>(vGRID.size()))
		return false;

	if(bAble) {
		vGRID.at( iIndex ).m_bEnable = STATUS_UI_UNIT_ENABLE;
	}
	else {
		vGRID.at( iIndex ).m_bEnable = STATUS_UI_UNIT_DISABLE;
	}
	return bAble;
}


void	SPWindowGrid::SetShowLine(int iLine /*= 0*/)
{
	if(iLine == 0) {
		m_iShowCount = m_iLineCount;
	}
	else 
		m_iShowCount = iLine;
}


SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowGrid )
	SPIMESSAGE_COMMAND(	SPIM_LBUTTON_DOWN,		OnLButtonDown	)
	SPIMESSAGE_COMMAND(	SPIM_LBUTTON_UP,		OnLButtonUP		)

	SPIMESSAGE_COMMAND(	SPIM_RBUTTON_DOWN,		OnRButtonDown	)	//[2005/5/23]
	SPIMESSAGE_COMMAND(	SPIM_RBUTTON_UP,		OnRButtonUP		)

	SPIMESSAGE_COMMAND(	SPIM_LBUTTON_DBLCLICK,	OnLButtonDouble )	//[2005/10/27]

	SPIMESSAGE_COMMAND( SPIM_CURSOR_MOVE,		OnCursorMove	)
	SPIMESSAGE_COMMAND( SPIM_CURSOR_ENTER,		OnCursorEnter	)
	SPIMESSAGE_COMMAND( SPIM_CURSOR_OUT,		OnCursorOut		)

	SPIMESSAGE_COMMAND( SPIM_TOOLTIP_MOVE,		OnToolTipMove	)
	SPIMESSAGE_COMMAND( SPIM_TOOLTIP_ENTER,		OnToolTipEnter	)
	SPIMESSAGE_COMMAND( SPIM_TOOLTIP_OUT,		OnToolTipOut	)

	SPIMESSAGE_COMMAND(	SPIM_WHEEL_UP,			OnWheelUp		)	//[2005/7/6]
	SPIMESSAGE_COMMAND(	SPIM_WHEEL_DOWN,		OnWheelDown		)

	SPIMESSAGE_COMMAND( SPIM_MOVE_BEGIN	,		OnNULL			)	//  [5/24/2006] 그리드에선 마우스무브 안함.
	SPIMESSAGE_COMMAND( SPIM_MOVE_END	,		OnNULL			)
	SPIMESSAGE_COMMAND( SPIM_MOVE		,		OnNULL			)


SPIMESSAGE_OBJ_MAP_END	( SPWindow	)


int SPWindowGrid::OnLButtonDown( WPARAM wParam, LPARAM lParam)
{
	m_bPushed = true;

	// Click 된 곳의 position을 Point로 들고 있다.
	int iX, iY;
	GetAbsCoord(iX, iY);
	int xPos = LOWORD(lParam);
	int yPos = HIWORD(lParam);

	m_iItemPushPosX = xPos;
	m_iItemPushPosY = yPos;

	int xRelPos = ( xPos - iX )/m_ptGap.x;
	int yRelPos = ( yPos - iY )/m_ptGap.y;

	int iIndex = (yRelPos * m_iLineSize) + xRelPos;

	// lparam 으로 grid의 포지션 값을 던져준다.
	//return SPSendMessageToParent(SPIM_LBUTTON_DOWN, wParam, MAKELONG( xRelPos, yRelPos ) , this);
	//return SPSendMessageToParent(SPIM_LBUTTON_DOWN, (WPARAM)iIndex, MAKELONG( xRelPos, yRelPos ) , this);
	return SPSendMessageToParent(SPIM_LBUTTON_DOWN, wParam, (LPARAM)iIndex , this);
}

int SPWindowGrid::OnLButtonUP( WPARAM wParam, LPARAM lParam)
{
	m_bPushed = false;

	//return SPSendMessageToParent(SPIM_LBUTTON_UP, wParam, lParam, this);
	
	int iX, iY;
	GetAbsCoord(iX, iY);
	int xPos = LOWORD(lParam);
	int yPos = HIWORD(lParam);

	m_iItemPushPosX = -1;
	m_iItemPushPosY = -1;

	if((xPos - iX >= m_iSX) || (yPos - iY >= m_iSY)) {
		//DXUTOutputDebugString("실 Grid Window 영역을 벗어났습니다\n");
		//아이콘 복귀하도록 한다.
		return SPSendMessageToParent(SPIM_LBUTTON_UP, wParam, -1, this);		
		//return 1;
	}

	int xRelPos = ( xPos - iX )/m_ptGap.x;
	int yRelPos = ( yPos - iY )/m_ptGap.y;	

	int iIndex = (yRelPos * m_iLineSize) + xRelPos;

	//return SPSendMessageToParent(SPIM_LBUTTON_UP, wParam, MAKELONG( xRelPos, yRelPos ), this);
	//return SPSendMessageToParent(SPIM_LBUTTON_UP, (WPARAM)iIndex, MAKELONG( xRelPos, yRelPos ), this);
	return SPSendMessageToParent(SPIM_LBUTTON_UP, wParam, (LPARAM)iIndex, this);
}


int SPWindowGrid::OnLButtonDouble( WPARAM wParam, LPARAM lParam ) 
{
	m_bPushed = false;

	//return SPSendMessageToParent(SPIM_LBUTTON_UP, wParam, lParam, this);

	int iX, iY;
	GetAbsCoord(iX, iY);
	int xPos = LOWORD(lParam);
	int yPos = HIWORD(lParam);

	m_iItemPushPosX = -1;
	m_iItemPushPosY = -1;

	if((xPos - iX >= m_iSX) || (yPos - iY >= m_iSY)) {
		//DXUTOutputDebugString("실 Grid Window 영역을 벗어났습니다\n");
		//아이콘 복귀하도록 한다.
		return SPSendMessageToParent(SPIM_LBUTTON_DBLCLICK, wParam, -1, this);		
		//return 1;
	}

	int xRelPos = ( xPos - iX )/m_ptGap.x;
	int yRelPos = ( yPos - iY )/m_ptGap.y;	

	int iIndex = (yRelPos * m_iLineSize) + xRelPos;

	//return SPSendMessageToParent(SPIM_LBUTTON_UP, wParam, MAKELONG( xRelPos, yRelPos ), this);
	//return SPSendMessageToParent(SPIM_LBUTTON_UP, (WPARAM)iIndex, MAKELONG( xRelPos, yRelPos ), this);
	return SPSendMessageToParent(SPIM_LBUTTON_DBLCLICK, wParam, (LPARAM)iIndex, this);
}


int SPWindowGrid::OnRButtonDown( WPARAM wParam, LPARAM lParam )
{
	//m_bPushed = true;

	// Click 된 곳의 position을 Point로 들고 있다.
	int iX, iY;
	GetAbsCoord(iX, iY);
	int xPos = LOWORD(lParam);
	int yPos = HIWORD(lParam);

	int xRelPos = ( xPos - iX )/m_ptGap.x;
	int yRelPos = ( yPos - iY )/m_ptGap.y;

	int iIndex = (yRelPos * m_iLineSize) + xRelPos;

	//return SPSendMessageToParent(SPIM_RBUTTON_DOWN, wParam, MAKELONG( xRelPos, yRelPos ) , this);
	//return SPSendMessageToParent(SPIM_RBUTTON_DOWN, (WPARAM)iIndex, MAKELONG( xRelPos, yRelPos ) , this);
	return SPSendMessageToParent(SPIM_RBUTTON_DOWN, wParam, (LPARAM)iIndex, this);
}

int SPWindowGrid::OnRButtonUP( WPARAM wParam, LPARAM lParam )
{
	//m_bPushed = false;

	int iX, iY;
	GetAbsCoord(iX, iY);
	int xPos = LOWORD(lParam);
	int yPos = HIWORD(lParam);

	int xRelPos = ( xPos - iX )/m_ptGap.x;
	int yRelPos = ( yPos - iY )/m_ptGap.y;

	int iIndex = (yRelPos * m_iLineSize) + xRelPos;

	//return SPSendMessageToParent(SPIM_RBUTTON_UP, wParam, lParam, this);
	//return SPSendMessageToParent(SPIM_RBUTTON_UP, wParam, MAKELONG( xRelPos, yRelPos ), this);
	//return SPSendMessageToParent(SPIM_RBUTTON_UP, (WPARAM)iIndex, MAKELONG( xRelPos, yRelPos ), this);
	return SPSendMessageToParent(SPIM_RBUTTON_UP, wParam, (LPARAM)iIndex, this);
}


int SPWindowGrid::OnWheelUp( WPARAM wParam, LPARAM lParam)
{
	return SPSendMessageToParent(SPIM_WHEEL_UP, wParam, lParam, this);
}


int SPWindowGrid::OnWheelDown( WPARAM wParam, LPARAM lParam)
{
	return SPSendMessageToParent(SPIM_WHEEL_DOWN, wParam, lParam, this);
}


int SPWindowGrid::OnCursorMove( WPARAM wParam, LPARAM lParam)
{
	// Move 된 곳의 position을 Point로 들고 있다.
	int iX, iY;
	GetAbsCoord(iX, iY);
	int iPosX = LOWORD(lParam);
	int iPosY = HIWORD(lParam);

	if(m_bPushed) {
		if(m_iItemPushPosX > -1 && m_iItemPushPosY > -1) {
			if((iPosX < m_iItemPushPosX - 3 || iPosX > m_iItemPushPosX + 3) ||
				(iPosY < m_iItemPushPosY - 3 || iPosY > m_iItemPushPosY + 3))
			{
				//DXUTOutputDebugString("SPWindowGrid Click 후 위치 이동...\n");
				m_bClickMove = true;
				return SPSendMessageToParent(SPIM_CURSOR_MOVE, (WPARAM)lParam, (LPARAM)m_bClickMove, this);
			}
		}
	}

	//int xRelPos = ( xPos - iX )/m_ptGap.x;
	//int yRelPos = ( yPos - iY )/m_ptGap.y;
	//int iIndex = (yRelPos * m_iLineSize) + xRelPos;

	// lparam 으로 grid의 포지션 값을 던져준다.
	//return SPSendMessageToParent(SPIM_CURSOR_MOVE, wParam, MAKELONG( xRelPos, yRelPos ) , this);
	//return SPSendMessageToParent(SPIM_CURSOR_MOVE, (WPARAM)iIndex, MAKELONG( xRelPos, yRelPos ) , this);
	//return SPSendMessageToParent(SPIM_CURSOR_MOVE, wParam, (LPARAM)iIndex, this);
	//return SPSendMessageToParent(SPIM_CURSOR_MOVE, (WPARAM)lParam, (LPARAM)iIndex, this);
	return SPSendMessageToParent(SPIM_CURSOR_MOVE, (WPARAM)lParam, (LPARAM)false, this);;
}

int SPWindowGrid::OnCursorEnter( WPARAM wParam, LPARAM lParam)
{
	return SPSendMessageToParent(SPIM_CURSOR_ENTER, 0, 0, this);
}

int SPWindowGrid::OnCursorOut( WPARAM wParam, LPARAM lParam)
{
	m_bPushed = false;
	return SPSendMessageToParent(SPIM_CURSOR_OUT, 0, 0, this);
}


int SPWindowGrid::OnToolTipMove( WPARAM wParam, LPARAM lParam)
{
	// Move 된 곳의 position을 Point로 들고 있다.
	int iX, iY;
	GetAbsCoord(iX, iY);
	int xPos = LOWORD(lParam);
	int yPos = HIWORD(lParam);

	int xRelPos = ( xPos - iX )/m_ptGap.x;
	int yRelPos = ( yPos - iY )/m_ptGap.y;

	int iIndex = (yRelPos * m_iLineSize) + xRelPos;
	
	//if(xRelPos>=m_ptNum.x || yRelPos>=m_ptNum.y || xRelPos<0 || yRelPos<0)
	if(xRelPos >= m_iLineSize || yRelPos >= m_iLineCount || xRelPos < 0 || yRelPos < 0)
	{
		return 0;
	}

	// lparam 으로 grid의 포지션 값을 던져준다.
	//return SPSendMessageToParent(SPIM_TOOLTIP_MOVE, (WPARAM)iIndex, MAKELONG( xRelPos, yRelPos ) , this);
	return SPSendMessageToParent(SPIM_TOOLTIP_MOVE, wParam, (LPARAM)iIndex, this);
}


int SPWindowGrid::OnToolTipEnter( WPARAM wParam, LPARAM lParam)
{
	// Move 된 곳의 position을 Point로 들고 있다.
	int iX, iY;
	GetAbsCoord(iX, iY);
	int xPos = LOWORD(lParam);
	int yPos = HIWORD(lParam);

	int xRelPos = ( xPos - iX )/m_ptGap.x;
	int yRelPos = ( yPos - iY )/m_ptGap.y;

	int iIndex = (yRelPos * m_iLineSize) + xRelPos;

	//if(xRelPos>=m_ptNum.x || yRelPos>=m_ptNum.y || xRelPos<0 || yRelPos<0)
	if(xRelPos >= m_iLineSize || yRelPos >= m_iLineCount || xRelPos < 0 || yRelPos < 0)
	{
		return 0;
	}

	// lparam 으로 grid의 포지션 값을 던져준다.
	//return SPSendMessageToParent(SPIM_TOOLTIP_ENTER, (WPARAM)iIndex, MAKELONG( xRelPos, yRelPos ) , this);
	return SPSendMessageToParent(SPIM_TOOLTIP_ENTER, wParam, (LPARAM)iIndex, this);
}


int SPWindowGrid::OnToolTipOut( WPARAM wParam, LPARAM lParam)
{
	return SPSendMessageToParent(SPIM_TOOLTIP_OUT, 0, 0, this);
}









