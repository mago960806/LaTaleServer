
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

#include "SPWindow.h"
#include "SPWindowDigitBoard.h"

inline int RectWidth( RECT &rc )	{ return ((rc).right - (rc).left ); }
inline int RectHeight( RECT &rc )	{ return ((rc).bottom - (rc).top ); }

SPWindowDigitBoard::SPWindowDigitBoard(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_CTRL_DIGITBOARD, InstanceID, iX, iY, iCX, iCY, pParent)
{
	Init();
}

SPWindowDigitBoard::~SPWindowDigitBoard()
{
	Clean();
}

void SPWindowDigitBoard::Init()
{	
	ImageDigitboard idb;
	if(g_pVideo->GetTextureMgr()->LoadTexture("DATA/INTERFACE/CONCEPT/UI100.PNG", &(idb.pImage)) == true) {
		SetRect( &(idb.rcSrc),  153,380,158,387);
		m_mImageList.insert(std::map< char, ImageDigitboard >::value_type ('0', idb));
	}	
	if(g_pVideo->GetTextureMgr()->LoadTexture("DATA/INTERFACE/CONCEPT/UI100.PNG", &(idb.pImage)) == true) {
		SetRect( &(idb.rcSrc),159,380,164,387);
		m_mImageList.insert(std::map< char, ImageDigitboard >::value_type ('1', idb));
	}
	if(g_pVideo->GetTextureMgr()->LoadTexture("DATA/INTERFACE/CONCEPT/UI100.PNG", &(idb.pImage)) == true) {
		SetRect( &(idb.rcSrc),165,380,170,387);
		m_mImageList.insert(std::map< char, ImageDigitboard >::value_type ('2', idb));
	}	
	if(g_pVideo->GetTextureMgr()->LoadTexture("DATA/INTERFACE/CONCEPT/UI100.PNG", &(idb.pImage)) == true) {
		SetRect( &(idb.rcSrc),171,380,176,387);
		m_mImageList.insert(std::map< char, ImageDigitboard >::value_type ('3', idb));
	}
	if(g_pVideo->GetTextureMgr()->LoadTexture("DATA/INTERFACE/CONCEPT/UI100.PNG", &(idb.pImage)) == true) {
		SetRect( &(idb.rcSrc),177,380,182,387);
		m_mImageList.insert(std::map< char, ImageDigitboard >::value_type ('4', idb));
	}
	if(g_pVideo->GetTextureMgr()->LoadTexture("DATA/INTERFACE/CONCEPT/UI100.PNG", &(idb.pImage)) == true) {
		SetRect( &(idb.rcSrc),183,380,188,387);
		m_mImageList.insert(std::map< char, ImageDigitboard >::value_type ('5', idb));
	}
	if(g_pVideo->GetTextureMgr()->LoadTexture("DATA/INTERFACE/CONCEPT/UI100.PNG", &(idb.pImage)) == true) {
		SetRect( &(idb.rcSrc),189,380,194,387);
		m_mImageList.insert(std::map< char, ImageDigitboard >::value_type ('6', idb));
	}
	if(g_pVideo->GetTextureMgr()->LoadTexture("DATA/INTERFACE/CONCEPT/UI100.PNG", &(idb.pImage)) == true) {
		SetRect( &(idb.rcSrc),195,380,200,387);
		m_mImageList.insert(std::map< char, ImageDigitboard >::value_type ('7', idb));
	}
	if(g_pVideo->GetTextureMgr()->LoadTexture("DATA/INTERFACE/CONCEPT/UI100.PNG", &(idb.pImage)) == true) {
		SetRect( &(idb.rcSrc),201,380,206,387);
		m_mImageList.insert(std::map< char, ImageDigitboard >::value_type ('8', idb));
	}
	if(g_pVideo->GetTextureMgr()->LoadTexture("DATA/INTERFACE/CONCEPT/UI100.PNG", &(idb.pImage)) == true) {
		SetRect( &(idb.rcSrc),207,380,212,387);
		m_mImageList.insert(std::map< char, ImageDigitboard >::value_type ('9', idb));
	}
	if(g_pVideo->GetTextureMgr()->LoadTexture("DATA/INTERFACE/CONCEPT/UI100.PNG", &(idb.pImage)) == true) {
		SetRect( &(idb.rcSrc),213,380,218,387);
		m_mImageList.insert(std::map< char, ImageDigitboard >::value_type ('%', idb));		
	}
	if(g_pVideo->GetTextureMgr()->LoadTexture("DATA/INTERFACE/CONCEPT/UI100.PNG", &(idb.pImage)) == true) {
		SetRect( &(idb.rcSrc),219,380,224,387);
		m_mImageList.insert(std::map< char, ImageDigitboard >::value_type ('/', idb));		
	}
	if(g_pVideo->GetTextureMgr()->LoadTexture("DATA/INTERFACE/CONCEPT/UI100.PNG", &(idb.pImage)) == true) {
		SetRect( &(idb.rcSrc),276,385,281,392);
		m_mImageList.insert(std::map< char, ImageDigitboard >::value_type ('.', idb));		
	}
	if(g_pVideo->GetTextureMgr()->LoadTexture("DATA/INTERFACE/CONCEPT/UI100.PNG", &(idb.pImage)) == true) { //[2008/3/4]-jinhee
		SetRect( &(idb.rcSrc),225,380,230,387);
		m_mImageList.insert(std::map< char, ImageDigitboard >::value_type (',', idb));		
	}


	m_strWindowText = "0";
	Refresh();
}

void SPWindowDigitBoard::Clean()
{
	std::map<char, ImageDigitboard>::iterator mIter = m_mImageList.begin();	
	for(; mIter != m_mImageList.end() ; ++mIter) {
		SAFE_RELEASE( ((*mIter).second).pImage );
	}
	m_mImageList.clear();
	SPWindow::Clean();
}

bool SPWindowDigitBoard::SetDigit(const char* pszImage, char cDigit, RECT rcSrc)
{
	ImageDigitboard idb;
	std::map< char, ImageDigitboard >::iterator mIter;
	if(g_pVideo->GetTextureMgr()->LoadTexture(pszImage, &(idb.pImage)) == true) {
		idb.rcSrc = rcSrc;
		mIter = m_mImageList.find(cDigit);
		if(mIter != m_mImageList.end()) {
			m_mImageList.erase(mIter);
		}
		m_mImageList.insert(std::map< char, ImageDigitboard >::value_type (cDigit, idb));
		return true;
	}
	return false;
}

void SPWindowDigitBoard::RefreshRelationCoord()
{
	SPWindow::RefreshRelationCoord();
	ReposOwnImage();
}

void SPWindowDigitBoard::SetWindowText(const char* pStr)
{
	m_strWindowText = pStr;
	Refresh();
}

void SPWindowDigitBoard::Process(float fTime)
{
	SPWindow::Process(fTime);
}

void SPWindowDigitBoard::Render(float fTime)
{
	if(!m_bShow) return;
	SPWindow::Render(fTime);	

	ResultInstance* pResultInstance;
	RECT		rcTemp;
	std::vector<ResultInstance>::iterator Iter = m_vResult.begin();
	for(;m_vResult.end() != Iter; ++Iter) {
		pResultInstance = &(*Iter);		
		rcTemp = pResultInstance->rcTar;
		OffsetRect(&rcTemp, m_iAX, m_iAY);
		pResultInstance->pImageDigit->pImage->RenderTexture(&rcTemp, &(pResultInstance->pImageDigit->rcSrc));		
	}
}

void SPWindowDigitBoard::RenderText()
{
}

void SPWindowDigitBoard::ReposOwnImage()
{
}

void SPWindowDigitBoard::Refresh()
{
	if(m_strWindowText.length() == 0) {
		return;
	}

	RECT rcWindow;	
	ResultInstance resultInstance;
	std::map< char, ImageDigitboard >::iterator mIter;
	int iX1;
	int iLength;

	m_vResult.clear();
	GetWindowRect(rcWindow);

	if( m_dwAlign == DT_LEFT ) {
		iX1 = 0;
		std::string::iterator strIter = m_strWindowText.begin();
		for(; strIter != m_strWindowText.end(); ++strIter) {
			mIter = m_mImageList.find(*strIter);
			if( mIter != m_mImageList.end() ) {
				resultInstance.pImageDigit = &((*mIter).second);
				iLength = RectWidth(resultInstance.pImageDigit->rcSrc);
				//SetRect( &(resultInstance.rcTar), iX1, rcWindow.top, iX1 + iLength, rcWindow.bottom);
				SetRect( &(resultInstance.rcTar), iX1, 0, iX1 + iLength, RectHeight(rcWindow));
				iX1 += (iLength+1);
				m_vResult.push_back( resultInstance );
			}
		}
	} else {
		iX1 = RectWidth(rcWindow);	
		std::string::reverse_iterator strRIter = m_strWindowText.rbegin();
		for(; strRIter != m_strWindowText.rend(); ++strRIter) {
			mIter = m_mImageList.find(*strRIter);
			if( mIter != m_mImageList.end() ) {
				resultInstance.pImageDigit = &((*mIter).second);
				iLength = RectWidth(resultInstance.pImageDigit->rcSrc);
				SetRect( &(resultInstance.rcTar), iX1 - iLength, 0, iX1, RectHeight(rcWindow));
				iX1 -= (iLength+1);
				m_vResult.push_back( resultInstance );
			}
		}
	}

}




//////////////////////////////////////////////////////////////////////////


SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowDigitBoard )
	SPIMESSAGE_COMMAND(	SPIM_CURSOR_ENTER,		OnCursorEnter	)
	SPIMESSAGE_COMMAND(	SPIM_CURSOR_OUT,		OnCursorOut		)	
	SPIMESSAGE_COMMAND( SPIM_WHEEL_UP,			OnWheelUp		)
	SPIMESSAGE_COMMAND( SPIM_WHEEL_DOWN,		OnWheelDown		)

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)

int SPWindowDigitBoard::OnCursorEnter( WPARAM wParam, LPARAM lParam)
{	
	return SPSendMessageToParent(SPIM_CURSOR_ENTER, wParam, lParam, this);	
}

int SPWindowDigitBoard::OnCursorOut( WPARAM wParam, LPARAM lParam)
{
	return SPSendMessageToParent(SPIM_CURSOR_OUT, wParam, lParam, this);
}

int SPWindowDigitBoard::OnWheelUp( WPARAM wParam, LPARAM lParam)
{
	return SPSendMessageToParent(SPIM_WHEEL_UP, wParam, lParam, this);
}

int SPWindowDigitBoard::OnWheelDown( WPARAM wParam, LPARAM lParam)
{
	return SPSendMessageToParent(SPIM_WHEEL_DOWN, wParam, lParam, this);
}














