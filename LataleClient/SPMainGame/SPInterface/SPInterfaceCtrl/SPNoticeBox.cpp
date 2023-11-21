
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

#include "SPInterfaceCoordManager.h"

#include "SPWindow.h"
#include "SPWindowButton.h"
#include "SPWindowStatic.h"
#include "SPWindowEdit.h"
#include "SPNoticeBox.h"

#include "SPPlayerInvenArchive.h"
#include "SPCheckManager.h"

#include "SPAbuseFilter.h"

SPNoticeBox::SPNoticeBox(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_CTRL_NOTICE, InstanceID, iX, iY, iCX, iCY, pParent)
, m_pTextureBase(NULL)
, m_pYes(NULL)
, m_pNo(NULL)
, m_pIgnore(NULL)
, m_pEdit(NULL)
{
	Init();
}


SPNoticeBox::~SPNoticeBox()
{
	SAFE_RELEASE(m_pTextureBase);
	Clean();
}


void SPNoticeBox::Init()
{
	m_strToolTip = "[SPNoticeBox]";

	m_IMDefaultAnswer	= SPIM_NOTICE_NO;
	m_GMDefaultAnswer	= SPGM_NOTICE_NO;
	m_pSenderWindow		= NULL;
	m_pSenderGOB		= NULL;

	m_fDelay			= 0.0f;
	m_fAccumulate		= 0.0f;
	m_iRemainSecond		= 0;
	m_fSecondDelay		= 1.0f;
	m_fSecondAccumulate	= 0.0f;

	m_iSelectNumber		= 0;
	m_bNoticeActive		= false;

	m_iMaxNumber		= 999;
	m_iMinNumber		= 0;

	for(int i = 0; i < WINDOW_BASE_MAX; i++) {
		SetRect(&m_rtSrcBase[i], 0, 0, 0, 0);
		SetRect(&m_rtDestBase[i], 0, 0, 0, 0);
	}

	////
	//SetRect(&m_rtSrcBase[NOTICE_BASE_TOP_LEFT],		1, 14, 5, 18);
	//SetRect(&m_rtSrcBase[NOTICE_BASE_TOP],			7, 14, 11, 18);	
	//SetRect(&m_rtSrcBase[NOTICE_BASE_TOP_RIGHT],		13, 14, 17, 18);
	//SetRect(&m_rtSrcBase[NOTICE_BASE_CENTER_LEFT],	1, 20, 5, 24);
	//SetRect(&m_rtSrcBase[NOTICE_BASE_CENTER],		7, 20, 11, 24);
	//SetRect(&m_rtSrcBase[NOTICE_BASE_CENTER_RIGHT],	13, 20, 17, 24);
	//SetRect(&m_rtSrcBase[NOTICE_BASE_BOTTOM_LEFT],	1, 26, 5, 30);
	//SetRect(&m_rtSrcBase[NOTICE_BASE_BOTTOM],		7, 26, 11, 30);
	//SetRect(&m_rtSrcBase[NOTICE_BASE_BOTTOM_RIGHT],	13, 26, 17, 30);

	SetRect(&m_rtSrcBase[WINDOW_BASE_TOP_LEFT],		34,	165, 44, 175);
	SetRect(&m_rtSrcBase[WINDOW_BASE_TOP],			45,	165, 55, 175);	
	SetRect(&m_rtSrcBase[WINDOW_BASE_TOP_RIGHT],	56,	165, 66, 175);
	SetRect(&m_rtSrcBase[WINDOW_BASE_CENTER_LEFT],	34,	176, 44, 186);
	SetRect(&m_rtSrcBase[WINDOW_BASE_CENTER],		45,	176, 55, 186);
	SetRect(&m_rtSrcBase[WINDOW_BASE_CENTER_RIGHT],	56,	176, 66, 186);
	SetRect(&m_rtSrcBase[WINDOW_BASE_BOTTOM_LEFT],	34,	187, 44, 197);
	SetRect(&m_rtSrcBase[WINDOW_BASE_BOTTOM],		45,	187, 55, 197);
	SetRect(&m_rtSrcBase[WINDOW_BASE_BOTTOM_RIGHT],	56,	187, 66, 197);
	
	//g_pVideo->GetTextureMgr()->LoadTexture("DATA/INTERFACE/CONCEPT/UI107.PNG", &m_pTextureBase);
	g_pVideo->GetTextureMgr()->GetTexture("DATA/INTERFACE/CONCEPT/UI107.PNG", &m_pTextureBase,TEX_INTERFACE);

	SPWindow* pkChildWindow;
	SPWindowButton* pkButton;
	SPWindowEdit* pkEdit;

	//pkSkinWindow = new SPWindowStatic(WIID_NOTICEBOX_SKIN_UPPER, 0, 0, 160, 4, this);
	//pkSkinWindow->SetImage("DATA/INTERFACE/CONCEPT/UI100.PNG", 1, 14);	

	//pkSkinWindow = new SPWindowStatic(WIID_NOTICEBOX_SKIN_LOWER, 0, 106, 160, 4, this);
	//pkSkinWindow->SetImage("DATA/INTERFACE/CONCEPT/UI100.PNG", 1, 24);

	//pkSkinWindow = new SPWindowStatic(WIID_NOTICEBOX_SKIN_MIDDLE, 0, 4, 160, 102, this);
	//pkSkinWindow->SetImage("DATA/INTERFACE/CONCEPT/UI100.PNG", 1, 20);
	//pkSkinWindow->SetSrcSize(160, 2);

	pkChildWindow = new SPWindowStatic(WIID_NOTICEBOX_TEXT, 10, 0, 140, 60, this);
	pkChildWindow->SetFontColor(D3DXCOLOR(0.007f, 0.1843f, 0.4588f, 1.0f));
	pkChildWindow->SetFormat(DT_VCENTER | DT_CENTER);
	//pkChildWindow->SetFormat(DT_VCENTER | DT_LEFT);
	pkChildWindow->SetMultiLine(true, 2);

	pkEdit = new SPWindowEdit(WIID_NOTICEBOX_INPUT_EDIT, 55, 63, 37, 17, this);
	pkEdit->SetWindowType(WND_TYPE_LEFTBASE);
	pkEdit->SetMargin(12, 1);
	pkEdit->SetImageNormalHead("DATA/INTERFACE/CONCEPT/UI101.PNG", 102, 316, 104, 333);
	pkEdit->SetImageNormalBody("DATA/INTERFACE/CONCEPT/UI101.PNG", 106, 316, 108, 333);
	pkEdit->SetImageNormalTail("DATA/INTERFACE/CONCEPT/UI101.PNG", 110, 316, 112, 333);
	pkEdit->SetImageFocusHead("DATA/INTERFACE/CONCEPT/UI101.PNG", 113, 316, 115, 333);
	pkEdit->SetImageFocusBody("DATA/INTERFACE/CONCEPT/UI101.PNG", 117, 316, 119, 333);
	pkEdit->SetImageFocusTail("DATA/INTERFACE/CONCEPT/UI101.PNG", 121, 316, 123, 333);
	pkEdit->SetFontColor(D3DXCOLOR(0.007f, 0.1843f, 0.4588f, 1.0f));
	pkEdit->SetFormat(DT_VCENTER | DT_LEFT);
	pkEdit->SetNumber();
	pkEdit->SetMinValue(0);
	pkEdit->SetMaxValue(999);
	m_pEdit = pkEdit;

	pkChildWindow = new SPWindowStatic(WIID_NOTICEBOX_COUNT_TEXT, 96, 66, 9, 11, this);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 14, 175);
	//pkChildWindow->SetSrcSize(11, 11);

	pkButton = new SPWindowButton(WIID_NOTICEBOX_LEFT, 39, 66, 9, 13, this);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 5, 264);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI101.PNG", 5, 278);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI101.PNG", 5, 272);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI101.PNG", 5, 306);

	pkButton = new SPWindowButton(WIID_NOTICEBOX_RIGHT, 113, 66, 9, 13, this);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI101.PNG", 16, 264);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI101.PNG", 16, 278);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI101.PNG", 16, 272);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI101.PNG", 16, 306);
	
	pkButton = new SPWindowButton(WIID_NOTICEBOX_OK, 32, 85, 60, 17, this);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI107.PNG", 1, 93);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI107.PNG", 1, 111);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI107.PNG", 1, 129);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI107.PNG", 1, 147);
	m_pYes = pkButton;

	pkButton = new SPWindowButton(WIID_NOTICEBOX_CANCEL, 94, 85, 60, 17, this);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI107.PNG", 70, 135);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI107.PNG", 70, 153);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI107.PNG", 70, 171);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI107.PNG", 70, 189);
	m_pNo = pkButton;
}


void SPNoticeBox::Clean()
{
	SPWindow::Clean();
}


void SPNoticeBox::Show(bool bWithChild )
{
	g_pInterfaceManager->SetModalWindow(this);
	SPWindow::Show(bWithChild);
	SetWindowType();

	SPWindow* pkWindow;
	pkWindow = Find(WIID_NOTICEBOX_INPUT_EDIT, true);
	if( pkWindow && pkWindow->IsShow())
	{
		char szTemp[32];
		sprintf(szTemp, "%d", m_iSelectNumber);

		pkWindow->SetWindowText(szTemp);		
		pkWindow->SetFocus();		
	}

}


void SPNoticeBox::Hide()
{	
	g_pInterfaceManager->SetModalWindow(NULL);
	//ForceReply();
	if(m_bShow == false) {		
		SPWindow* pkWindow = Find(WIID_NOTICEBOX_TEXT, true);
		if( pkWindow )
			pkWindow->SetWindowText(NULL);
		
		SPWindow::Hide();
		return;
	}
	
	bool bRetVal = false;
	if(m_bShow && m_strCurMessage.empty() == false && m_eNoticeType > NOTICE_TYPE_MSG) {		
		if( m_pSenderWindow ) {
			m_pSenderWindow->SPSendMessage(m_IMDefaultAnswer, m_iSelectNumber , (int)-1 );	// 강제로 닫힐경우 체크 해야되서리 [4/24/2007 AJJIYA]
			m_pSenderWindow = NULL;			
			bRetVal = true;
		}
		if( m_pSenderGOB ) {
			m_pSenderGOB->SPGOBPerformMessage(m_GMDefaultAnswer, m_iSelectNumber);
			m_pSenderGOB = NULL;			
			bRetVal = true;
		}		
		m_strCurMessage.clear();
	}
	m_bNoticeActive = false;

	SPWindow* pkWindow = Find(WIID_NOTICEBOX_TEXT, true);
	if( pkWindow )
		pkWindow->SetWindowText(NULL);

	m_fDelay = 0.0f;

	SPWindow::Hide();
}


bool SPNoticeBox::Notice( const char* szMsg, SPWindow* pSender, NOTICE_TYPE NoticeType,
						 int iShowDelay, /*= 0*/
						 DWORD dwAlign /* = DT_CENTER | DT_VCENTER  */,
						SPIM IMDefaultAnswer /* = SPIM_NOTICE_YES */ )
{	
	//[12/15/2008 jinssaga]
	//ofstream file("./DeadActionInfo.txt", std::ios::out | std::ios::app);
	//if(!file){
	//	return false;
	//}	
	//file << "-----SPNoticeBox::Notice() Window Msg[" << szMsg << "] ShowTime[" << iShowDelay << "]" << endl;
	
	if(m_bShow)
		ForceReply();

	m_eNoticeType = NoticeType;
	m_pSenderGOB = NULL;
	m_pSenderWindow = pSender;	
	m_fDelay	= 0.0f;
	
	m_iSelectNumber = 0;	
	if(m_eNoticeType == NOTICE_TYPE_YESNO) {				//[2005/9/13]
		m_iMaxNumber = 0;
		m_iSelectNumber = 0;		
	}
	m_bNoticeActive = true;

	if(szMsg == NULL) {
		m_pSenderWindow->SPSendMessage(m_IMDefaultAnswer, m_iSelectNumber , (int)-1 );	// 강제로 닫힐경우 체크 해야되서리 [4/24/2007 AJJIYA]
		m_pSenderWindow = NULL;
		return false;
	}

	m_strCurMessage = szMsg;
	if(m_strCurMessage.empty()) {
		m_pSenderWindow->SPSendMessage(m_IMDefaultAnswer, m_iSelectNumber , (int)-1 );	// 강제로 닫힐경우 체크 해야되서리 [4/24/2007 AJJIYA]
		m_pSenderWindow = NULL;
		return false;
	}
	
	ResizeWindow();

	SPWindow* pkWindow = Find(WIID_NOTICEBOX_TEXT, true);
	if( pkWindow )
	{
		pkWindow->SetWindowText(m_strCurMessage.c_str());
		pkWindow->SetFormat( dwAlign );
	}

	if(iShowDelay) {
		m_fDelay			= iShowDelay * 1.0f;
		m_fAccumulate		= 0.0f;
		m_iRemainSecond		= iShowDelay;
		m_fSecondAccumulate = 0.0f;
		m_IMDefaultAnswer	= IMDefaultAnswer;		
		
		if(pkWindow) {
			char szTemp[256];
			ZeroMemory(szTemp, 256);
			sprintf(szTemp, m_strCurMessage.c_str(), m_iRemainSecond);
			pkWindow->SetWindowText(szTemp);
		}
	}
	
	g_pInterfaceManager->RegisterSoundUnit("DATA/SOUND/SYSTEM_ATTENTION_WINDOW.WAV");	
	Show();

	return m_bNoticeActive;
}


bool SPNoticeBox::Notice( const char* szMsg, SPGameObject* pSender, NOTICE_TYPE NoticeType,
						 int iShowDelay, /*= 0,*/
						 DWORD dwAlign /* = DT_CENTER | DT_VCENTER  */,
						 SPGM GMDefaultAnswer /* = SPGM_NOTICE_YES */ )
{	
	//[12/15/2008 jinssaga]
	//ofstream file("./DeadActionInfo.txt", std::ios::out | std::ios::app);
	//if(!file){
	//	return false;
	//}	
	//file << "-----SPNoticeBox::Notice() Object Msg[" << szMsg << "] ShowTime[" << iShowDelay << "]" << endl;
	
	if(m_bShow)
		ForceReply();

	m_eNoticeType = NoticeType;
	m_pSenderGOB = pSender;	
	m_pSenderWindow = NULL;
	m_fDelay	= 0.0f;

	//m_iSelectNumber = 0;	
	m_iSelectNumber = m_iMaxNumber;				//[2005/7/25] - jinhee //�...
	if(m_eNoticeType == NOTICE_TYPE_YESNO) {			//[2005/9/13]
		m_iMaxNumber = 0;
		m_iSelectNumber = 0;		
	}

	m_bNoticeActive = true;

	if(szMsg == NULL) {
		m_pSenderGOB->SPGOBPerformMessage(m_GMDefaultAnswer, m_iSelectNumber);
		m_pSenderGOB = NULL;
		return false;
	}

	m_strCurMessage = szMsg;
	if(m_strCurMessage.empty()) {
		m_pSenderGOB->SPGOBPerformMessage(m_GMDefaultAnswer, m_iSelectNumber);
		m_pSenderGOB = NULL;
		return false;
	}

	//char szTemp[512];
	//ZeroMemory(&szTemp, 512);
	//wsprintf(szTemp, "WWWWWWWWwWWWWWWWWWWWWWWWw\nWWWWWWWWWWWWWWWWWwwwwwwwwwwwwwwwwwwwwwwww-End");
	//m_strCurMessage += szTemp;

	ResizeWindow();

	SPWindow* pkWindow = Find(WIID_NOTICEBOX_TEXT, true);
	if( pkWindow )
	{
		pkWindow->SetWindowText(m_strCurMessage.c_str());
		pkWindow->SetFormat( dwAlign );
	}
	
	if(iShowDelay) {
		m_fDelay			= iShowDelay * 1.0f;
		m_fAccumulate		= 0.0f;
		m_iRemainSecond		= iShowDelay;
		m_fSecondAccumulate = 0.0f;
		m_GMDefaultAnswer	= GMDefaultAnswer;		
		
		if(pkWindow) {
			char szTemp[256];
			ZeroMemory(szTemp, 256);
			sprintf(szTemp, m_strCurMessage.c_str(), m_iRemainSecond);
			pkWindow->SetWindowText(szTemp);
		}
	}

	g_pInterfaceManager->RegisterSoundUnit("DATA/SOUND/SYSTEM_ATTENTION_WINDOW.WAV");	
	Show();

	return m_bNoticeActive;
}


void SPNoticeBox::ResizeWindow()
{
	if(m_strCurMessage.empty())
		return;

	int iScreenWidth = g_pVideo->GetScreenLenX();
	int iScreenHeight = g_pVideo->GetScreenLenY();

	int iFontY = 0;
	SIZE szRet = {0, 0};

	SPWindow* pWindow  = Find(WIID_NOTICEBOX_TEXT, true);
	if(pWindow) {
		iFontY = (int)m_pFont->GetHeight();
		szRet = m_pFont->GetSize( m_strCurMessage.c_str() );
	}
	
	//Width 
	int iStartX = 0;
	int iStartY = 0;
	int iWidth = 0;
	int iHeight = (szRet.cy + 2) * 2 ;
		
	iStartX = (iScreenWidth / 2) - 100;
	iWidth = 200;	

/*
	if(szRet.cx > 200 && szRet.cx < 320) {
		iStartX = (iScreenWidth / 2) - (szRet.cx / 2);
		iWidth = szRet.cx;		
	}
	else if(szRet.cx > 320) {
		iStartX = (iScreenWidth / 2) - (320 / 2);
		iWidth = 320;
		//이쪽은 짤라내야 하니..
	//	iHeight = (szRet.cy + 2) * ((szRet.cx / 320) + 1);
	}

	//int iCount = 0;
	//for(int i = 0; i < m_strCurMessage.length(); i++) {
	//	if( m_strCurMessage[i] == '\n') {
	//		iCount++;
	//	}
	//}
*/

	//////////////////////////////////////////////////////////////////////////
	// \n 구분하여 계산하는 부분 [8/24/2007 AJJIYA]
	char	szLineFeedBuf[MAX_STR_LENGTH];
	SPAbuseFilter::GetInstance()->ReplaceLineFeed( m_strCurMessage.c_str() , szLineFeedBuf , MAX_STR_LENGTH );

	std::string	strTestMessage	=	szLineFeedBuf;
	std::string	strTestLineMesage;

	int		iCount			=	0;
	int		iCurPos			=	0;
	int		iFindPos		=	0;
	SIZE	szMultiRet		=	{0, 0};
	int		iMessageLength	=	(int)strTestMessage.length();
	bool	bLastLine		=	false;

	while ( iCurPos != std::string::npos )
	{				
		iCurPos = (int)strTestMessage.find( '\n' , iFindPos );

		if( bLastLine == false && iCurPos == std::string::npos )
		{
			iCurPos		=	iMessageLength;
			bLastLine	=	true;
		}

		if( iFindPos >= 0 && iCurPos >= 0 && iFindPos <= iMessageLength && iCurPos <= iMessageLength )
		{
			strTestLineMesage	=	strTestMessage.substr( iFindPos , iCurPos - iFindPos );
			szMultiRet = m_pFont->GetSize( strTestLineMesage.c_str() );

			if( szMultiRet.cx > 320 )
			{
				iHeight	+=	(szRet.cy + 2) * (szMultiRet.cx / 320);
				szMultiRet.cx	=	320;
			}
			else if( szMultiRet.cx < 200 )
			{
				szMultiRet.cx	=	200;
			}

			iStartX = max( iStartX , (iScreenWidth / 2) - (szMultiRet.cx / 2) );
			iWidth	= max( iWidth , szMultiRet.cx );
		}

		if( bLastLine == true )
			break;

		if( iCurPos != std::string::npos )
		{
			++iCount;
			iFindPos	=	iCurPos	+	1;
		}
	}

	//////////////////////////////////////////////////////////////////////////

	iHeight = iHeight + ((szRet.cy + 2) * iCount);
	iHeight += 10;

	iStartX = iStartX - 10;	
	
	//Window Resize
	POINT ptStart = {0, 0};
	int iTotalWidth = iWidth + 10;
	int iTotalHeight = 0;
	if(m_eNoticeType == NOTICE_TYPE_MSG) {
		iTotalHeight = iHeight + 24;
	}
	else {
		iTotalHeight = iHeight + 20 + 23;
	}
	//int iTotalHeight = iHeight + 20 + 23;
	
	SPInterfaceCoordManager::GetInstance()->GetWindowPT(WIID_NOTICEBOX, ptStart);
	iStartY = ptStart.y;
	this->AdjustCoord(iStartX, iStartY, iTotalWidth, iTotalHeight);	

	SetRect(&m_rtDestBase[WINDOW_BASE_TOP_LEFT],		iStartX, iStartY, iStartX + 10, iStartY + 10);
	SetRect(&m_rtDestBase[WINDOW_BASE_TOP],				iStartX + 10, iStartY, iStartX + 10 + iTotalWidth -20, iStartY + 10);
	SetRect(&m_rtDestBase[WINDOW_BASE_TOP_RIGHT],		m_rtDestBase[WINDOW_BASE_TOP].right, iStartY, m_rtDestBase[WINDOW_BASE_TOP].right + 10, iStartY + 10);	

	SetRect(&m_rtDestBase[WINDOW_BASE_CENTER_LEFT],		iStartX, m_rtDestBase[WINDOW_BASE_TOP_LEFT].bottom, iStartX + 10, m_rtDestBase[WINDOW_BASE_TOP_LEFT].bottom + iTotalHeight - 20);
	SetRect(&m_rtDestBase[WINDOW_BASE_CENTER],			iStartX + 10, m_rtDestBase[WINDOW_BASE_TOP_LEFT].bottom, iStartX + 10 + iTotalWidth -20, m_rtDestBase[WINDOW_BASE_TOP_LEFT].bottom + iTotalHeight - 20);
	SetRect(&m_rtDestBase[WINDOW_BASE_CENTER_RIGHT],	m_rtDestBase[WINDOW_BASE_CENTER].right, m_rtDestBase[WINDOW_BASE_TOP_LEFT].bottom, m_rtDestBase[WINDOW_BASE_CENTER].right + 10, m_rtDestBase[WINDOW_BASE_TOP_LEFT].bottom + iTotalHeight - 20);
	
	SetRect(&m_rtDestBase[WINDOW_BASE_BOTTOM_LEFT],		iStartX, m_rtDestBase[WINDOW_BASE_CENTER].bottom, iStartX + 10, m_rtDestBase[WINDOW_BASE_CENTER].bottom + 10);
	SetRect(&m_rtDestBase[WINDOW_BASE_BOTTOM],			iStartX + 10, m_rtDestBase[WINDOW_BASE_CENTER].bottom, iStartX + 10 + iTotalWidth -20, m_rtDestBase[WINDOW_BASE_CENTER].bottom + 10);
	SetRect(&m_rtDestBase[WINDOW_BASE_BOTTOM_RIGHT],	m_rtDestBase[WINDOW_BASE_BOTTOM].right, m_rtDestBase[WINDOW_BASE_CENTER].bottom, m_rtDestBase[WINDOW_BASE_BOTTOM].right + 10, m_rtDestBase[WINDOW_BASE_CENTER].bottom + 10);	

	ArrangeControl();
	ArrangeButton();
}


void SPNoticeBox::ArrangeControl()
{
	RECT rtBase = {0, 0, 0, 0};
	this->GetWindowRect(rtBase);

	int iWidth = rtBase.right - rtBase.left;
	int iHeight = rtBase.bottom - rtBase.top;
	SPWindow* pWindow = NULL;

	pWindow = Find(WIID_NOTICEBOX_TEXT, true);
	if(pWindow) {
		if(m_eNoticeType == NOTICE_TYPE_MSG) {
			pWindow->AdjustCoord(5, 5, iWidth - 10, iHeight - 12);
		}
		else {
			pWindow->AdjustCoord(5, 5, iWidth - 10, iHeight - 10 - (20 + 23));
		}
	}

	RECT rtText = {0, 0, 0, 0};
	pWindow->GetWindowRect(rtText);

	int iStartX = 0 + (iWidth / 2 - 44);
	int iStartY = 5 + (rtText.bottom - rtText.top) + 5;
	int iEndX = 0;	

	if(m_eNoticeType == NOTICE_TYPE_YESNOSLIDER) {
		pWindow = Find(WIID_NOTICEBOX_LEFT, true);
		if(pWindow) {
			pWindow->AdjustCoord(iStartX, iStartY + 3, 9, 13);
		}
		iEndX = iStartX + 9 + 7;
		
		pWindow = Find(WIID_NOTICEBOX_INPUT_EDIT, true);
		if(pWindow) {
			pWindow->AdjustCoord(iEndX, iStartY, 37, 17);
			SPWindowEdit* pkEdit = (SPWindowEdit*)pWindow;
			pkEdit->SetWindowType(WND_TYPE_LEFTBASE);
			pkEdit->SetMargin(12, 1);
			pkEdit->SetImageNormalHead("DATA/INTERFACE/CONCEPT/UI101.PNG", 102, 316, 104, 333);
			pkEdit->SetImageNormalBody("DATA/INTERFACE/CONCEPT/UI101.PNG", 106, 316, 108, 333);
			pkEdit->SetImageNormalTail("DATA/INTERFACE/CONCEPT/UI101.PNG", 110, 316, 112, 333);
			pkEdit->SetImageFocusHead("DATA/INTERFACE/CONCEPT/UI101.PNG", 113, 316, 115, 333);
			pkEdit->SetImageFocusBody("DATA/INTERFACE/CONCEPT/UI101.PNG", 117, 316, 119, 333);
			pkEdit->SetImageFocusTail("DATA/INTERFACE/CONCEPT/UI101.PNG", 121, 316, 123, 333);
			pkEdit->SetNumber();
			pkEdit->SetMaxValue(m_iMaxNumber);
			pkEdit->SetMinValue(m_iMinNumber);			
		}
		iEndX = iEndX + 37 + 4;
		
		pWindow = Find(WIID_NOTICEBOX_COUNT_TEXT, true);		
		if(pWindow) {
			pWindow->AdjustCoord(iEndX, iStartY + 3, 9, 11);
		}
		iEndX = iEndX + 9 + 4;

		pWindow = Find(WIID_NOTICEBOX_RIGHT, true);
		if(pWindow) {
			pWindow->AdjustCoord(iEndX, iStartY + 3, 9, 13);
		}		
		
	}
}


void SPNoticeBox::ArrangeButton()
{
	RECT rtBase = {0, 0, 0, 0};
	this->GetWindowRect(rtBase);

	int iWidth = rtBase.right - rtBase.left;
	int iHeight = rtBase.bottom - rtBase.top;
	SPWindow* pWindow = NULL;

	int iStartX = 0;
	int iStartY = iHeight - 5 - 17;
	int iEndX = 0;

	switch(m_eNoticeType) {
		case NOTICE_TYPE_MSG:
			return;
		case NOTICE_TYPE_YES:
			{				
				//pWindow = Find(WIID_NOTICEBOX_CANCEL);
				//if(pWindow)
				//	pWindow->Hide();
				
				iStartX = iWidth - 7 - 60;				
				pWindow = Find(WIID_NOTICEBOX_OK);
				if(pWindow)
					pWindow->AdjustCoord(iStartX, iStartY, 60, 17);
			}
			break;
		case NOTICE_TYPE_YESNOIGNORE:
			{
			}
			break;
		case NOTICE_TYPE_YESNO:
		case NOTICE_TYPE_YESNOSLIDER:
		default:
			{				
				iStartX = iWidth - 7 - 60;
				pWindow = Find(WIID_NOTICEBOX_CANCEL);
				if(pWindow) {
					pWindow->AdjustCoord(iStartX, iStartY, 60, 17);
					//if(!pWindow->IsShow())
					//	pWindow->Show();
				}

				iStartX = iStartX - 7 - 60;
				pWindow = Find(WIID_NOTICEBOX_OK);
				if(pWindow)
					pWindow->AdjustCoord(iStartX, iStartY, 60, 17);
			}
			break;
	}
}


void SPNoticeBox::SetWindowType()
{
	bool bBtnShow = false;
	if( m_eNoticeType == NOTICE_TYPE_YESNOSLIDER )
		bBtnShow = true;

	SPWindow* pkWindow;
	int i;
	for( i = WIID_NOTICEBOX_LEFT; i <= WIID_NOTICEBOX_COUNT_TEXT; i++ )
	{
		pkWindow = Find(i, true);
		if( pkWindow )
		{
			if( bBtnShow )
				pkWindow->Show();
			else
				pkWindow->Hide();
		}
	}

	switch(m_eNoticeType) {
		case NOTICE_TYPE_MSG:
			if(m_pYes)
				m_pYes->Hide();
			
			if(m_pNo)
			m_pNo->Hide();
			break;
		case NOTICE_TYPE_YES:
			if(m_pYes)
				m_pYes->Show();
			if(m_pNo)
				m_pNo->Hide();
			break;
		case NOTICE_TYPE_YESNOIGNORE:
			//break;
		case NOTICE_TYPE_YESNO:
		case NOTICE_TYPE_YESNOSLIDER:
		default:
			if(m_pYes)
				m_pYes->Show();
			
			if(m_pNo)
				m_pNo->Show();
			break;		
	}
}

void SPNoticeBox::UpdateInputString(const char* pszString)
{
	SPWindow* pkWindow = Find(WIID_NOTICEBOX_INPUT_EDIT, true);
	if( pkWindow ) {
		pkWindow->SetWindowText(pszString);
		g_pInterfaceManager->GetIMEPool()->SetSel(-1, -1); // 셀렉트 해제
	}	
}


bool SPNoticeBox::ForceReply()
{
	bool bRetVal = false;
	if(m_bShow && m_strCurMessage.empty() == false && m_eNoticeType > NOTICE_TYPE_MSG) {
		if( m_pSenderWindow ) {
			m_pSenderWindow->SPSendMessage(m_IMDefaultAnswer, m_iSelectNumber , (int)-1 );	// 강제로 닫힐경우 체크 해야되서리 [4/24/2007 AJJIYA]
			m_pSenderWindow = NULL;			
			bRetVal = true;
		}
		if( m_pSenderGOB ) {
			m_pSenderGOB->SPGOBPerformMessage(m_GMDefaultAnswer, m_iSelectNumber);
			m_pSenderGOB = NULL;			
			bRetVal = true;
		}		
		m_strCurMessage.clear();
	}
	m_bNoticeActive = false;

	SPWindow* pkWindow = Find(WIID_NOTICEBOX_TEXT, true);
	if( pkWindow )
		pkWindow->SetWindowText(NULL);

	Hide();
	return bRetVal;
}

bool SPNoticeBox::Reply(SPIM SpimYesNo)
{
	SPIM	IMDefaultAnswer = SPIM_NOTICE_NO;
	SPGM	GMDefaultAnswer	= SPGM_NOTICE_NO;

	switch(SpimYesNo) {
	case SPIM_NOTICE_YES:
		{
			IMDefaultAnswer = SPIM_NOTICE_YES;
			GMDefaultAnswer = SPGM_NOTICE_YES;			
		}
		break;

	case SPIM_NOTICE_NO:
		{
			IMDefaultAnswer = SPIM_NOTICE_NO;
			GMDefaultAnswer = SPGM_NOTICE_NO;
		}
		break;
	}

	if( m_pSenderWindow ) {
		m_pSenderWindow->SPSendMessage(IMDefaultAnswer, m_iSelectNumber);
		//m_pSenderWindow = NULL;					
	}

	if( m_pSenderGOB ) {
		m_pSenderGOB->SPGOBPerformMessage(GMDefaultAnswer, m_iSelectNumber);
		//m_pSenderGOB = NULL;				
	}		
	
	////
	//m_strCurMessage.clear();
	//SPWindow* pkWindow = Find(WIID_NOTICEBOX_TEXT, true);
	//if( pkWindow )
	//	pkWindow->SetWindowText(NULL);

	//m_bNoticeActive = false;	

	return true;

}


void SPNoticeBox::Process(float fTime)
{
	if(!m_bShow) return;
	
	if(m_fDelay != 0.0f && m_bNoticeActive) {	
		m_fAccumulate += fTime;
		if(m_fAccumulate > m_fDelay) {
			
			if( m_pSenderWindow ) {
				m_pSenderWindow->SPSendMessage(m_IMDefaultAnswer, 0 , (int)-1 );	// 강제로 닫힐경우 체크 해야되서리 [4/24/2007 AJJIYA]
				m_pSenderWindow = NULL;
			}
			if( m_pSenderGOB ) {
				m_pSenderGOB->SPGOBPerformMessage(m_GMDefaultAnswer, 0);
				m_pSenderGOB = NULL;
			}

			ForceReply();
			m_fAccumulate -= m_fDelay;
			m_fDelay = 0.0f;
		}

		m_fSecondAccumulate += fTime;
		if(m_fSecondAccumulate > m_fSecondDelay) {
			m_fSecondAccumulate -= m_fSecondDelay;
			m_iRemainSecond--;
			if(m_iRemainSecond < 1)
				m_iRemainSecond = 0;
			
			SPWindow* pWindow = Find(WIID_NOTICEBOX_TEXT, true);
			if(pWindow) {
				char szTemp[256];
				ZeroMemory(szTemp, 256);
				sprintf(szTemp, m_strCurMessage.c_str(), m_iRemainSecond);
				pWindow->SetWindowText(szTemp);
			}
		}
	}
	SPWindow::Process(fTime);
}


void SPNoticeBox::Render(float fTime)
{
	if(!m_bShow) return;	

	if(m_bNoticeActive == false)
		return;

	g_pVideo->Flush();
	
	if(m_pTextureBase) {
		for(int i = 0; i < WINDOW_BASE_MAX; i++) {
			m_pTextureBase->RenderTexture(&m_rtDestBase[i], &m_rtSrcBase[i]);
		}
	}

	SPWindow::Render(fTime);

#ifdef _DEBUG
	////
	//RECT rtRect = {0, 0, 0, 0};
	//SPWindow* pWindow = NULL;
	//
	//g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	////this->GetWindowRect(rtRect);
	////g_pVideo->GetSysLine()->DrawRect(rtRect);
	//
	//pWindow = Find(WIID_NOTICEBOX_TEXT, this);
	//if(pWindow) {
	//	pWindow->GetWindowRect(rtRect);
	//	g_pVideo->GetSysLine()->DrawRect(rtRect);
	//}
	//
	////for(int i = 0; i < NOTICE_BASE_MAX; i++) {
	////	g_pVideo->GetSysLine()->DrawRect(m_rtDestBase[i]);
	////}

	///*
	//pWindow = Find(WIID_NOTICEBOX_SKIN_MIDDLE, this);
	//if(pWindow) {
	//	pWindow->GetWindowRect(rtRect);
	//	g_pVideo->GetSysLine()->DrawRect(rtRect);
	//}

	//pWindow = Find(WIID_NOTICEBOX_SKIN_LOWER, this);
	//if(pWindow) {
	//	pWindow->GetWindowRect(rtRect);
	//	g_pVideo->GetSysLine()->DrawRect(rtRect);
	//}
	//*/
	//g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
#endif
}



SPIMESSAGE_OBJ_MAP_BEGIN	( SPNoticeBox )
SPIMESSAGE_COMMAND(	SPIM_SET_MAXVALUE,		OnMaxValue )
SPIMESSAGE_COMMAND(	SPIM_SET_MINVALUE,		OnMinValue )

SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_NOTICEBOX_LEFT,		OnScrollLeft	)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_NOTICEBOX_RIGHT,		OnScrollRight	)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_NOTICEBOX_OK	,		OnYes			)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_NOTICEBOX_CANCEL,		OnNo			)
SPIMESSAGE_CONTROL( SPIM_REFRESH,			WIID_NOTICEBOX_INPUT_EDIT,	OnUpdateCount)

SPIMESSAGE_CONTROL( SPIM_WHEEL_UP,			WIID_NOTICEBOX_TEXT,		OnScrollRight)
SPIMESSAGE_CONTROL( SPIM_WHEEL_DOWN,		WIID_NOTICEBOX_TEXT,		OnScrollLeft)
SPIMESSAGE_CONTROL( SPIM_WHEEL_UP,			WIID_NOTICEBOX_INPUT_EDIT,	OnScrollRight)
SPIMESSAGE_CONTROL( SPIM_WHEEL_DOWN,		WIID_NOTICEBOX_INPUT_EDIT,	OnScrollLeft)

SPIMESSAGE_COMMAND(	SPIM_KEY_RETURN,		OnKeyReturn )
SPIMESSAGE_CONTROL(	SPIM_KEY_RETURN,		WIID_NOTICEBOX_INPUT_EDIT,	OnKeyReturn )

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)

int SPNoticeBox::OnKeyReturn( WPARAM wParam, LPARAM lParam)
{
	if(g_pCheckManager->GetNoticeUse() == NOTICE_USE_PLAYER_DEAD || 
		g_pCheckManager->GetNoticeUse() == NOTICE_USE_PLAYER_DEAD_REDUCE ||
		g_pCheckManager->GetNoticeUse() == NOTICE_USE_PLAYER_DEAD_PVP)
	{
		return -1;
	}
	
	return OnYes(0,0);
}


int SPNoticeBox::OnScrollLeft	( WPARAM wParam, LPARAM lParam)
{
	--m_iSelectNumber;
	if(m_iMinNumber > m_iSelectNumber)
		m_iSelectNumber = m_iMinNumber;

	char szTemp[32];
	ZeroMemory(szTemp, 32);
	sprintf(szTemp, "%d", m_iSelectNumber);
	UpdateInputString(szTemp);
	
	return 1;
}

int SPNoticeBox::OnScrollRight	( WPARAM wParam, LPARAM lParam)
{
	++m_iSelectNumber;
	if(m_iMaxNumber < m_iSelectNumber)
		m_iSelectNumber = m_iMaxNumber;

	char szTemp[32];
	ZeroMemory(szTemp, 32);
	sprintf(szTemp, "%d", m_iSelectNumber);
	UpdateInputString(szTemp);
	return 1;
}

int SPNoticeBox::OnUpdateCount(WPARAM wParam, LPARAM lParam)
{
	//SPWindow* pkWindow;
	//pkWindow = Find(WIID_NOTICEBOX_INPUT_EDIT, true);
	//if( pkWindow == NULL )
	//	return 1;
	if(m_pEdit == NULL)
		return 1;

	//int iCount = atoi(pkWindow->GetWindowText());
	int iCount = atoi(m_pEdit->GetWindowText());
	if(iCount > 0 && iCount <= m_iMaxNumber)	{
		m_iSelectNumber = iCount;		
	}
	else {
		if(iCount < 1) {
			m_iSelectNumber = 0;
		}
		else if(iCount > m_iMaxNumber) {
			m_iSelectNumber = m_iMaxNumber;
		}
	}

	char szTemp[32];
	ZeroMemory(szTemp, 32);
	sprintf(szTemp, "%d", m_iSelectNumber);
	UpdateInputString(szTemp);

	return 1;
}

int SPNoticeBox::OnYes			( WPARAM wParam, LPARAM lParam)
{
	Hide();
	Reply(SPIM_NOTICE_YES);	
	return 1;
}

int SPNoticeBox::OnNo			( WPARAM wParam, LPARAM lParam)
{
	Hide();
	Reply(SPIM_NOTICE_NO);	
	return 1;
}

int SPNoticeBox::OnMaxValue ( WPARAM wParam, LPARAM lParam)
{
	m_iMaxNumber = (int)wParam;
	return 1;
}

int SPNoticeBox::OnMinValue ( WPARAM wParam, LPARAM lParam)
{
	m_iMinNumber = (int)wParam;
	return 1;
}