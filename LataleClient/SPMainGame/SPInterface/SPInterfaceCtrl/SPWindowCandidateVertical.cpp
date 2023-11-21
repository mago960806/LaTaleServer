
#include <vector>
#include <string>

#include "SPCommon.h"
#include "SPUtil.h"

#include "SPMainGameDEF.H"
#include "SPGameObjectDEF.h"

#include "SPSubGameManager.h"
#include "SPGameObject.h"
#include "SPManager.h"
#include "SPGOBManager.h"
#include "SPMouseCursor.h"
#include "SPIMEPool.h"


#include "SPWindowDEF.h"
#include "SPInterfaceManager.h"

#include "SPWindow.h"
#include "SPWindowStatic.h"

#include "SPWindowCandidateVertical.h"

#include "SPAbuseFilter.h"

#define CANDIV_MAX_ITEM 9



SPWindowCandidateVertical::SPWindowCandidateVertical(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_COMMON_CANDVERTICAL, InstanceID, iX, iY, iCX, iCY, pParent)	
{	
	Init();
}

SPWindowCandidateVertical::~SPWindowCandidateVertical()
{
	Clean();
}

void SPWindowCandidateVertical::Init()
{	
	m_RGBA = RGBA(0xFF,0xFF,0xFF,0xFF);
	m_strToolTip = "[SPWindowCandidateVertical]";	
	m_bHideEnable = true;

	//g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI104.PNG" , &m_pImageFrameHead);
	//SetRect(&m_rcFrameHeadSrc, 1, 31, 114, 35);

	//g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI104.PNG" , &m_pImageFrameBody);
	//SetRect(&m_rcFrameBodySrc, 1, 37, 114, 39);	

	//g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI104.PNG" , &m_pImageFrameTail);
	//SetRect(&m_rcFrameTailSrc, 1, 41, 114, 45);

	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI104.PNG" , &m_pImageFrameCursor);
	SetRect(&m_rcFrameCursorSrc, 295, 34, 373, 53);


//////////////////////////////////////////////////////////////////////////
	//SPTexture*		m_paImageFrame[9];
	//RECT			m_rcaFrameSrc[9];
	//RECT			m_rcaFrameCalc[9];


	// Minimum length (Pixel) == 82
	m_iMaxCandLength = 82;	
	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI104.PNG" , &(m_paImageFrame[0]));
	SetRect(&(m_rcaFrameSrc[0]), 1, 31, 26, 35);
	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI104.PNG" , &(m_paImageFrame[1]));
	SetRect(&(m_rcaFrameSrc[1]), 28, 31, 30, 35);
	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI104.PNG" , &(m_paImageFrame[2]));
	SetRect(&(m_rcaFrameSrc[2]), 32, 31, 36, 35);
	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI104.PNG" , &(m_paImageFrame[3]));
	SetRect(&(m_rcaFrameSrc[3]), 1, 37, 26, 39);
	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI104.PNG" , &(m_paImageFrame[4]));
	SetRect(&(m_rcaFrameSrc[4]), 28, 37, 30, 39);
	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI104.PNG" , &(m_paImageFrame[5]));
	SetRect(&(m_rcaFrameSrc[5]), 32, 37, 36, 39);
	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI104.PNG" , &(m_paImageFrame[6]));
	SetRect(&(m_rcaFrameSrc[6]), 1, 41, 26, 45);
	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI104.PNG" , &(m_paImageFrame[7]));
	SetRect(&(m_rcaFrameSrc[7]), 28, 41, 30, 45);
	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI104.PNG" , &(m_paImageFrame[8]));
	SetRect(&(m_rcaFrameSrc[8]), 32, 41, 36, 45);



//////////////////////////////////////////////////////////////////////////
	m_iCursorPos = 0;
	m_aiCursorY[0] = 4;
	m_aiCursorY[1] = 25;
	m_aiCursorY[2] = 46;
	m_aiCursorY[3] = 67;
	m_aiCursorY[4] = 88;
	m_aiCursorY[5] = 109;
	m_aiCursorY[6] = 130;
	m_aiCursorY[7] = 151;
	m_aiCursorY[8] = 172;

	ReposOwnImage();

	RECT rcTemp;	
	SPWindowStatic* pStatic;

	pStatic = new SPWindowStatic( WIID_STC_INDEX1	, 10, 9, 7, 12, this);		pStatic->SetFormat(DT_LEFT);
	pStatic = new SPWindowStatic( WIID_STC_INDEX2	, 10, 30, 7, 12, this);		pStatic->SetFormat(DT_LEFT);
	pStatic = new SPWindowStatic( WIID_STC_INDEX3	, 10, 51, 7, 12, this);		pStatic->SetFormat(DT_LEFT);
	pStatic = new SPWindowStatic( WIID_STC_INDEX4	, 10, 72, 7, 12, this);		pStatic->SetFormat(DT_LEFT);
	pStatic = new SPWindowStatic( WIID_STC_INDEX5	, 10, 93, 7, 12, this);		pStatic->SetFormat(DT_LEFT);
	pStatic = new SPWindowStatic( WIID_STC_INDEX6	, 10, 114, 7, 12, this);	pStatic->SetFormat(DT_LEFT);
	pStatic = new SPWindowStatic( WIID_STC_INDEX7	, 10, 135, 7, 12, this);	pStatic->SetFormat(DT_LEFT);
	pStatic = new SPWindowStatic( WIID_STC_INDEX8	, 10, 156, 7, 12, this);	pStatic->SetFormat(DT_LEFT);
	pStatic = new SPWindowStatic( WIID_STC_INDEX9	, 10, 177, 7, 12, this);	pStatic->SetFormat(DT_LEFT);
	pStatic = new SPWindowStatic( WIID_STC_TEXT1	, 33, 6,	ITEM_SX, ITEM_SY, this);	pStatic->SetFormat(DT_LEFT);
	pStatic = new SPWindowStatic( WIID_STC_TEXT2	, 33, 27,	ITEM_SX, ITEM_SY, this);	pStatic->SetFormat(DT_LEFT);
	pStatic = new SPWindowStatic( WIID_STC_TEXT3	, 33, 48,	ITEM_SX, ITEM_SY, this);	pStatic->SetFormat(DT_LEFT);
	pStatic = new SPWindowStatic( WIID_STC_TEXT4	, 33, 69,	ITEM_SX, ITEM_SY, this);	pStatic->SetFormat(DT_LEFT);
	pStatic = new SPWindowStatic( WIID_STC_TEXT5	, 33, 90,	ITEM_SX, ITEM_SY, this);	pStatic->SetFormat(DT_LEFT);
	pStatic = new SPWindowStatic( WIID_STC_TEXT6	, 33, 111,	ITEM_SX, ITEM_SY, this);	pStatic->SetFormat(DT_LEFT);
	pStatic = new SPWindowStatic( WIID_STC_TEXT7	, 33, 132,	ITEM_SX, ITEM_SY, this);	pStatic->SetFormat(DT_LEFT);
	pStatic = new SPWindowStatic( WIID_STC_TEXT8	, 33, 153,	ITEM_SX, ITEM_SY, this);	pStatic->SetFormat(DT_LEFT);
	pStatic = new SPWindowStatic( WIID_STC_TEXT9	, 33, 174,	ITEM_SX, ITEM_SY, this);	pStatic->SetFormat(DT_LEFT);

	

	pStatic = new SPWindowStatic( WIID_STC_PAGE	, 53, 197,	50, 12, this);



}

void SPWindowCandidateVertical::Clean()
{
	SPWindow::Clean();
}

void SPWindowCandidateVertical::Process(float fTime)
{
	SPWindow::Process(fTime);
}

void SPWindowCandidateVertical::Render(float fTime)
{
	if(g_pInterfaceManager->GetIMEPool()->IsVerticalCand() == false) {
		return;
	}

	if( !m_bShow )
		return;

	//if(m_pImageFrameHead) 
	//	m_pImageFrameHead->RenderTexture(&m_rcCalcFrameHead, &m_rcFrameHeadSrc);

	//if(m_pImageFrameBody) 
	//	m_pImageFrameBody->RenderTexture(&m_rcCalcFrameBody, &m_rcFrameBodySrc);		

	//if(m_pImageFrameTail) 
	//	m_pImageFrameTail->RenderTexture(&m_rcCalcFrameTail, &m_rcFrameTailSrc);


	for(int i = 0; i < NO_FRAME; ++i) {
		if(m_paImageFrame[i]) {
			m_paImageFrame[i]->RenderTexture(&(m_rcaFrameCalc[i]), &(m_rcaFrameSrc[i]));
		}
	}

	if(m_pImageFrameCursor)
		m_pImageFrameCursor->RenderTexture(&m_rcCalcFrameCursor, &m_rcFrameCursorSrc);

	SPWindow::Render(fTime);
}

void SPWindowCandidateVertical::ReposOwnImage()
{
	SetRect(&m_rcCalcFrameHead, m_iAX, m_iAY, m_iAX+113, m_iAY+4);
	SetRect(&m_rcCalcFrameBody, m_iAX, m_iAY+4, m_iAX+113, m_iAY+4 +207);
	SetRect(&m_rcCalcFrameTail, m_iAX, m_iAY+211, m_iAX+113, m_iAY+4 +207+4);


	SetRect(&(m_rcaFrameCalc[0]), m_iAX, m_iAY, m_iAX+25, m_iAY+4);
	SetRect(&(m_rcaFrameCalc[3]), m_iAX, m_iAY+4, m_iAX+25, m_iAY+4+207);
	SetRect(&(m_rcaFrameCalc[6]), m_iAX, m_iAY+211, m_iAX+25, m_iAY+4+207+4);

	SetRect(&(m_rcaFrameCalc[1]), m_rcaFrameCalc[0].right, m_rcaFrameCalc[0].top, (m_rcaFrameCalc[0]).right+m_iMaxCandLength+5, m_rcaFrameCalc[0].bottom);
	SetRect(&(m_rcaFrameCalc[4]), m_rcaFrameCalc[3].right, m_rcaFrameCalc[3].top, (m_rcaFrameCalc[3]).right+m_iMaxCandLength+5, m_rcaFrameCalc[3].bottom);
	SetRect(&(m_rcaFrameCalc[7]), m_rcaFrameCalc[6].right, m_rcaFrameCalc[6].top, (m_rcaFrameCalc[6]).right+m_iMaxCandLength+5, m_rcaFrameCalc[6].bottom);

	SetRect(&(m_rcaFrameCalc[2]), m_rcaFrameCalc[1].right, m_rcaFrameCalc[1].top, (m_rcaFrameCalc[1]).right+4, m_rcaFrameCalc[1].bottom);
	SetRect(&(m_rcaFrameCalc[5]), m_rcaFrameCalc[4].right, m_rcaFrameCalc[4].top, (m_rcaFrameCalc[4]).right+4, m_rcaFrameCalc[4].bottom);
	SetRect(&(m_rcaFrameCalc[8]), m_rcaFrameCalc[7].right, m_rcaFrameCalc[7].top, (m_rcaFrameCalc[7]).right+4, m_rcaFrameCalc[7].bottom);

	SPWindow* pWindow;
	for (int i = WIID_STC_TEXT1; i <= WIID_STC_TEXT9; ++i) {
		pWindow = Find(i);
		if(pWindow) {
			pWindow->SetRectSize( m_iMaxCandLength + 5, ITEM_SY);
		}
	}


	m_rcCalcFrameCursor.left = m_iAX+26;
	m_rcCalcFrameCursor.top = m_iAY + m_aiCursorY[m_iCursorPos];
	//m_rcCalcFrameCursor.right = m_rcCalcFrameCursor.left + 78;
	m_rcCalcFrameCursor.right = m_rcCalcFrameCursor.left + m_iMaxCandLength;
	m_rcCalcFrameCursor.bottom = m_rcCalcFrameCursor.top + 19;
}


void SPWindowCandidateVertical::Refresh()
{
	if(g_pInterfaceManager->GetIMEPool()->IsVerticalCand() == false) {
		return;
	}

	const SPIMEPool::SPCandList* pCandList = g_pInterfaceManager->GetIMEPool()->GetCandList();

	SPWindow*	pWindowInx;
	SPWindow*	pWindowText;
	char		szTemp[10];	
	char		szCand[512];

	// Minimum length (Pixel) == 82
	m_iMaxCandLength = 82;
	SIZE tSize;
	if(pCandList->bShowWindow && pCandList->dwCount != 0 ) {
		for(int i = 0; i < CANDIV_MAX_ITEM; ++i) {
			if(i < pCandList->dwCount) {			
				pWindowInx = Find(i + WIID_STC_INDEX1);
				if(pWindowInx) {
					itoa ( i + 1, szTemp , 10);
					pWindowInx->SetWindowText( szTemp );
				}
				pWindowText = Find(i + WIID_STC_TEXT1 );
				if(pWindowText) {				
					SPAbuseFilter::GetInstance()->ConvertWideToAnsiString(szCand, pCandList->awszCandidate[i]);	
					pWindowText->SetWindowText( szCand );

					//tSize = m_pFont->GetSize(szCand);
					tSize = m_pFont->GetSizeW(pCandList->awszCandidate[i]);
					if( tSize.cx + 20 > m_iMaxCandLength)
						m_iMaxCandLength = tSize.cx + 20;
				}
			} else {
				pWindowInx = Find(i + WIID_STC_INDEX1);
				if(pWindowInx) 
					pWindowInx->SetWindowText(NULL);
				pWindowText = Find(i + WIID_STC_TEXT1 );
				if(pWindowText)
					pWindowText->SetWindowText(NULL);
			}
		}

		if(pWindowText = Find(WIID_STC_PAGE)) {
			sprintf(szCand, "%d/%d", pCandList->dwSelection + 1, pCandList->dwCount); 
			pWindowText->SetWindowText(szCand);
		}		

		if(pCandList->dwCount != 0) {
			m_iCursorPos = pCandList->dwSelection;
		} else {
			m_iCursorPos = 0;
		}


		ReposOwnImage();
		RefreshChildPos();
		Show();

	} else {
		Hide();
	}

}

SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowCandidateVertical )
SPIMESSAGE_COMMAND(	SPIM_REFRESH,			OnRefresh		)
SPIMESSAGE_OBJ_MAP_END	( SPWindow	)


int SPWindowCandidateVertical::OnRefresh	( WPARAM wParam, LPARAM lParam)
{		
	Refresh();
	return 1;
}




