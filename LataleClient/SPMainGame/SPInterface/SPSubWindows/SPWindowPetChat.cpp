#include "SPCommon.h"
#include "SPUtil.h"
#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPWindowEdit.h"
#include "SPMouseCursor.h"
#include "SPIMEPool.h"
#include "SPAbuseFilter.h"
#include "SPInterfaceManager.h"
#include "SPGOBManager.h"
#include "SPGameObject.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGOBCoordPhysics.h"
#include "SPPlayerCoordPhysics.h"
#include "SPPlayerInvenArchive.h"
#include "SPGOBModelUnit.h"
#include "SPGOBStatus.h"
#include "SPPlayerStatus.h"
#include "SPPlayerStatusModel.h"
#include "SPGOBCoordPhysics.h"
#include "SPPlayer.h"
#include "SPPet.h"
#include "SPPetManager.h"
#include "SPResourceManager.h"
#include "SPWindowPetChat.h"
#include "tinyxml.h"
//--------------------------------------------------
void CheckBtn(SPWindowButton *pButton);
void CheckBtn(SPWindowButton *pButton, bool bCheck );
//--------------------------------------------------
SPWindowPetChatEditor::SPWindowPetChatEditor(WND_ID wndClassID, INSTANCE_ID instanceID)
: SPWindow( wndClassID, instanceID )
{
	Init();
}
SPWindowPetChatEditor::SPWindowPetChatEditor(INSTANCE_ID instanceID, 
											 int iX, int iY, int iCX, int iCY, 
											 SPWindow *pParent /* = NULL  */)
											 : SPWindow( WNDID_PETCHAT_EDITOR, instanceID, iX, iY, iCX, iCY, pParent )
{
	Init();
}
//--------------------------------------------------
SPWindowPetChatEditor::~SPWindowPetChatEditor( void )
{
	Clean();
}
//--------------------------------------------------
void SPWindowPetChatEditor::Init( void )
{
	m_pImgBGLeft = NULL;
	m_pImgBGMiddle = NULL;
	m_pImgBGRight = NULL;
	m_pImgIconTitle = NULL;
	m_pBtnClose = NULL;
	m_pBtnOK = NULL;

	m_pChatMsg_01 = NULL;
	m_pChatMsg_02 = NULL;
	m_pChatMsg_03 = NULL;

	m_strCurrPetChatCfg = "";
	m_iPetSlotIndex = PET_PRIMARY;
	m_vecstrChatMsg.clear();
	m_vecstrChatMsg.resize( 3 );

	if (!_initGUI())
		assert( false );
}
//--------------------------------------------------
void SPWindowPetChatEditor::Clean( void )
{
	m_strCurrPetChatCfg = "";
	m_pChatMsg_01->SetWindowText( "" );
	m_pChatMsg_02->SetWindowText( "" );
	m_pChatMsg_03->SetWindowText( "" );
	SPWindow::Clean();
}
//--------------------------------------------------
void SPWindowPetChatEditor::Show(bool bWithChild /* = true  */)
{
	char szBuf[64] = {0,};
	g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_GETGOBNAME, (LPARAM)szBuf);
	std::string thisUser = szBuf;
	if (thisUser != m_strCurrPetChatCfg)
		_loadXmlCfg();

	//--------------------------------------------------
	// load pet's cfg to edit box
	if (m_vecstrChatMsg.size() > m_iPetSlotIndex)
	{
		std::vector< std::string > &vecPetChat = m_vecstrChatMsg[m_iPetSlotIndex];
		if (vecPetChat.size() >= 3)
		{
			m_pChatMsg_01->SetWindowText( vecPetChat[0].c_str() );
			m_pChatMsg_02->SetWindowText( vecPetChat[1].c_str() );
			m_pChatMsg_03->SetWindowText( vecPetChat[2].c_str() );
			if (!vecPetChat[0].empty() ) CheckBtn( m_pSelectMsg_01, true );
			if (!vecPetChat[1].empty() ) CheckBtn( m_pSelectMsg_02, true );
			if (!vecPetChat[2].empty() ) CheckBtn( m_pSelectMsg_03, true );
		}
		else
		{
			m_pChatMsg_01->SetWindowText( "" );
			m_pChatMsg_02->SetWindowText( "" );
			m_pChatMsg_03->SetWindowText( "" );
			CheckBtn( m_pSelectMsg_01, false );
			CheckBtn( m_pSelectMsg_02, false );
			CheckBtn( m_pSelectMsg_03, false );
		}
	}
	//--------------------------------------------------

	SPWindow::Show( bWithChild );
}
void SPWindowPetChatEditor::Hide(bool bSendServer /* = true  */)
{
	SPWindow::Hide();
}
//--------------------------------------------------
void SPWindowPetChatEditor::Process(float fDeltaTime)
{
	if (!m_bShow)
		return;

	SPWindow::Process( fDeltaTime );
}
void SPWindowPetChatEditor::Render(float fDeltaTime)
{
	if (!m_bShow)
		return;

	SPWindow::RenderReverse( fDeltaTime );
}
//--------------------------------------------------
//--------------------------------------------------
void SetCheckButton(SPWindowButton* pButton /*= NULL*/)
{
	if(pButton){
		char* OPTION_IMAGE_FILE2 = "DATA/INTERFACE/CONCEPT/UI102.PNG";		
		pButton->SetImage		(OPTION_IMAGE_FILE2, 204, 211);
		pButton->SetImageHit	(OPTION_IMAGE_FILE2, 217, 211);
		pButton->SetImagePush	(OPTION_IMAGE_FILE2, 230, 211);
		pButton->SetImageDisable(OPTION_IMAGE_FILE2, 239, 198);
		pButton->SetImageCheck	(OPTION_IMAGE_FILE2, 230, 211);						
		pButton->Show();
	}	
}
void CheckBtn(SPWindowButton *pButton, bool bCheck )
{
	if (pButton)
		pButton->SPSendMessage( SPIM_BTN_SELECT, bCheck );
}
void CheckBtn(SPWindowButton *pButton)
{
	if (pButton)
	{
		pButton->SPSendMessage( SPIM_BTN_SELECT, !pButton->IsChecked());
	}
}
bool SPWindowPetChatEditor::_initGUI( void )
{
	m_pBtnClose = new SPWindowButton( WIID_PETCHAT_EDITOR_CLOSE, 340, 9, 12, 12, this );
	m_pBtnClose->SetImage		("DATA/INTERFACE/CONCEPT/UI102.PNG",	247,	217);
	m_pBtnClose->SetImageHit	("DATA/INTERFACE/CONCEPT/UI102.PNG",	247,	230); //on
	m_pBtnClose->SetImagePush	("DATA/INTERFACE/CONCEPT/UI102.PNG",	247,	243); //push
	m_pBtnClose->SetImageDisable("DATA/INTERFACE/CONCEPT/UI102.PNG",	247,	256); //disable
	m_pBtnClose->Show();

	m_pBtnOK = new SPWindowButton( WIID_PETCHAT_EDITOR_OK, 160, 122, 68, 17, this );
	m_pBtnOK->SetImage("DATA/INTERFACE/CONCEPT/UI104.PNG", 208, 440);
	m_pBtnOK->SetImageHit("DATA/INTERFACE/CONCEPT/UI104.PNG", 208, 458);
	m_pBtnOK->SetImagePush("DATA/INTERFACE/CONCEPT/UI104.PNG", 208, 476);
	m_pBtnOK->SetImageDisable("DATA/INTERFACE/CONCEPT/UI104.PNG", 208, 494);

	m_pChatMsg_01 = new SPWindowEdit( WIID_PETCHAT_EDITOR_CHAT_01, 47, 47, 300, 17, this );
	m_pChatMsg_01->SetFontColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
	m_pChatMsg_01->SetLimitText(50);	
	m_pChatMsg_01->Show();

	m_pChatMsg_02 = new SPWindowEdit( WIID_PETCHAT_EDITOR_CHAT_02, 47, 71, 300, 17, this );
	m_pChatMsg_02->SetFontColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
	m_pChatMsg_02->SetLimitText(50);	
	m_pChatMsg_02->Show();

	m_pChatMsg_03 = new SPWindowEdit( WIID_PETCHAT_EDITOR_CHAT_03, 47, 95, 300, 17, this );
	m_pChatMsg_03->SetFontColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
	m_pChatMsg_03->SetLimitText(50);	
	m_pChatMsg_03->Show();

	m_pSelectMsg_01 = new SPWindowButton( WIID_PETCHAT_EDITOR_SELECT_01, 27, 48, 12, 12, this );
	SetCheckButton( m_pSelectMsg_01 );

	m_pSelectMsg_02 = new SPWindowButton( WIID_PETCHAT_EDITOR_SELECT_02, 27, 72, 12, 12, this );
	SetCheckButton( m_pSelectMsg_02 );

	m_pSelectMsg_03 = new SPWindowButton( WIID_PETCHAT_EDITOR_SELECT_03, 27, 96, 12, 12, this );
	SetCheckButton( m_pSelectMsg_03 );

	SPWindowStatic	*pStatic = NULL;
	pStatic	=	new SPWindowStatic( WIID_PETCHAT_EDITOR_TXT , 8 , 27 , 122 , 16 , this );
	pStatic->SetImage( "DATA/INTERFACE/CONCEPT/UI107.PNG" , 300 , 414 );
	pStatic	=	NULL;

	pStatic	=	new SPWindowStatic( WIID_PETCHAT_EDITOR_ICONTITLE , 14 , 4 , 198 , 22 , this );
	pStatic->SetImage( "DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG" , 1 , 362);
	pStatic	=	NULL;

	m_pImgBGLeft = new SPWindowStatic( WIID_PETCHAT_EDITOR_BG_LEFT,
		0, 0, 49, 182, this );
	m_pImgBGLeft->SetImage( "DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG", 384, 118 );
	m_pImgBGLeft->Show();

	m_pImgBGMiddle = new SPWindowStatic( WIID_PETCHAT_EDITOR_BG_MIDDLE,
		49, 0, 300, 182, this );
	m_pImgBGMiddle->SetImage( "DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG", 435, 118 );
	m_pImgBGMiddle->SetSrcSize( 4, 182 );
	m_pImgBGMiddle->Show();

	m_pImgBGRight = new SPWindowStatic( WIID_PETCHAT_EDITOR_BG_RIGHT,
		349, 0, 15, 182, this );
	m_pImgBGRight->SetImage( "DATA/INTERFACE/CONCEPT/BUGLE_V2.PNG", 441, 118 );
	m_pImgBGRight->Show();

	return true;
}
static int calcValue( const char *username )
{
	if (!username)
		return -1;
	int len = strlen( username );
	int iValue = -1;
	for (int idx = 0; idx < len; ++idx)
	{
		iValue += username[idx];
	}
	return iValue;
}
bool SPWindowPetChatEditor::_loadXmlCfg( void )
{
	if (!g_pGOBManager->GetLocalPlayer())
		return false;

	char szBuf[64] = {0,};
	g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_GETGOBNAME, (LPARAM)szBuf);
	int iLocalUserCRC = calcValue( szBuf );
	m_strCurrPetChatCfg = szBuf;

	const int MAX_STRING_SIZE_CHAT = 512;
	TiXmlDocument xmlDoc;
	TiXmlElement	*pRoot = NULL;
	TiXmlElement	*pPetChat = NULL;
	m_vecstrChatMsg.clear();
	m_vecstrChatMsg.resize( 3 );
	if (!xmlDoc.LoadFile( "UserOptions.xml", TIXML_ENCODING_UTF8 ))
		return false;

	pRoot = xmlDoc.FirstChildElement();
	if (!pRoot)
		return false;

	pPetChat = pRoot->FirstChildElement( _T("PetChat") );
	if (!pPetChat)
		return false;

	TiXmlElement	*pNode = NULL;
	int		iCRCUserName = 0;
	std::string msg_01;
	std::string msg_02;
	std::string msg_03;

	pNode = pPetChat->FirstChildElement( _T("user") );
	bool bFound = false;
	while (pNode)
	{
		pNode->QueryIntAttribute( _T("value"), &iCRCUserName );
		if (iLocalUserCRC != iCRCUserName)
		{
			pNode = pNode->NextSiblingElement();
			continue;
		}

		//--------------------------------------------------
		// node PetPlace
		TiXmlElement	*pNodePetIndex = NULL;
		pNodePetIndex = pNode->FirstChildElement( "PetIndex" );
		int iPetIdx = 0;
		while (pNodePetIndex)
		{
			pNodePetIndex->QueryIntAttribute( _T("value"), &iPetIdx );
			if (iPetIdx < 0 || iPetIdx >= 3)
			{
				pNodePetIndex = pNodePetIndex->NextSiblingElement();
				continue;
			}
			const char *pMsg_01 = pNodePetIndex->Attribute( _T("msg_01") );
			const char *pMsg_02 = pNodePetIndex->Attribute( _T("msg_02") );
			const char *pMsg_03 = pNodePetIndex->Attribute( _T("msg_03") );
			if (pMsg_01) msg_01 = pMsg_01;
			if (pMsg_02) msg_02 = pMsg_02;
			if (pMsg_03) msg_03 = pMsg_03;

			if (msg_01.size() > 50) msg_01.resize( 50 );
			if (msg_02.size() > 50) msg_02.resize( 50 );
			if (msg_02.size() > 50) msg_02.resize( 50 );

			m_vecstrChatMsg[iPetIdx].push_back( msg_01 );
			m_vecstrChatMsg[iPetIdx].push_back( msg_02 );
			m_vecstrChatMsg[iPetIdx].push_back( msg_03 );

			pNodePetIndex = pNodePetIndex->NextSiblingElement();
		}
		//--------------------------------------------------

		bFound = true;
		break;
	}
	
	if (!bFound)
	{
		if (m_pChatMsg_01) m_pChatMsg_01->SetWindowText( "" );
		if (m_pChatMsg_02) m_pChatMsg_02->SetWindowText( "" );
		if (m_pChatMsg_03) m_pChatMsg_03->SetWindowText( "" );
	}

	return true;
}
bool SPWindowPetChatEditor::_saveXmlCfg( void )
{
	if (!g_pGOBManager->GetLocalPlayer())
		return false;

	char szBuf[64] = {0,};
	g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_GETGOBNAME, (LPARAM)szBuf);
	int iLocalUserCRC = calcValue( szBuf );

	TiXmlDocument	xmlDoc;
	TiXmlElement	*pRoot = NULL;
	TiXmlElement	*pPetChat = NULL;
	xmlDoc.LoadFile( "UserOptions.xml", TIXML_ENCODING_UTF8 );

	pRoot = xmlDoc.FirstChildElement();
	if (!pRoot)
	{
		pRoot = new TiXmlElement( _T("UserOptions") );
		xmlDoc.LinkEndChild( pRoot );
	}

	pPetChat = pRoot->FirstChildElement( _T("PetChat") );
	if (!pPetChat)
	{
		pPetChat = new TiXmlElement( _T("PetChat") );
		pRoot->LinkEndChild( pPetChat );
	}

	TiXmlElement	*pNode = NULL;
	TiXmlElement	*pNodePetIndex = NULL;
	int		iCRCUserName = 0;
	pNode = pPetChat->FirstChildElement( _T("user") );
	bool bFoundNode = false;
	int iCount = 0;
	const int MAX_PET_CHAT_LOG = 10;
	while (pNode)
	{
		pNode->QueryIntAttribute( _T("value"), &iCRCUserName );
		if (iLocalUserCRC != iCRCUserName)
		{
			pNode = pNode->NextSiblingElement();
			continue;
		}
		else
		{
			bFoundNode = true;
			break;
		}
	}
	
	if (bFoundNode && pNode)
	{
		pPetChat->RemoveChild( pNode );
		pNode = new TiXmlElement( _T("user") );
		if (pNode)
		{
			pNode->SetAttribute( _T("value"), iLocalUserCRC );
			int iPetIdx = 0;
			for (iPetIdx = 0; iPetIdx < (int)m_vecstrChatMsg.size(); ++iPetIdx)
			{
				std::vector< std::string > &vecPetChat = m_vecstrChatMsg[iPetIdx];
				if (vecPetChat.size() < 3)
					continue;
				pNodePetIndex = new TiXmlElement( _T("PetIndex") );
				if (pNodePetIndex)
				{
					pNodePetIndex->SetAttribute( _T("value"), iPetIdx );
					pNodePetIndex->SetAttribute( _T("msg_01"), vecPetChat.at(0).c_str() );
					pNodePetIndex->SetAttribute( _T("msg_02"), vecPetChat.at(1).c_str() );
					pNodePetIndex->SetAttribute( _T("msg_03"), vecPetChat.at(2).c_str() );
					pNode->LinkEndChild( pNodePetIndex );
					pNodePetIndex = NULL;
				}
			}
			pPetChat->LinkEndChild( pNode );
			pNode = NULL;
		}
	}
	else
	{
		if (iCount > MAX_PET_CHAT_LOG)
		{
			pPetChat->RemoveChild( pPetChat->FirstChild() );
		}
		pNode = new TiXmlElement( _T("user") );
		if (pNode)
		{
			pNode->SetAttribute( _T("value"), iLocalUserCRC );
			int iPetIdx = 0;
			for (iPetIdx = 0; iPetIdx < (int)m_vecstrChatMsg.size(); ++iPetIdx)
			{
				std::vector< std::string > &vecPetChat = m_vecstrChatMsg[iPetIdx];
				if (vecPetChat.size() < 3)
					continue;
				pNodePetIndex = new TiXmlElement( _T("PetIndex") );
				if (pNodePetIndex)
				{
					pNodePetIndex->SetAttribute( _T("value"), iPetIdx );
					pNodePetIndex->SetAttribute( _T("msg_01"), vecPetChat.at(0).c_str() );
					pNodePetIndex->SetAttribute( _T("msg_02"), vecPetChat.at(1).c_str() );
					pNodePetIndex->SetAttribute( _T("msg_03"), vecPetChat.at(2).c_str() );
					pNode->LinkEndChild( pNodePetIndex );
					pNodePetIndex = NULL;
				}
			}
			pPetChat->LinkEndChild( pNode );
			pNode = NULL;
		}
	}

	xmlDoc.SaveFile( "UserOptions.xml" );
	return true;
}
std::vector< std::string > SPWindowPetChatEditor::GetPetChatVector(PET_PLACE iPlace)
{
	static std::vector< std::string > s_vec_chat_str;
	if (m_vecstrChatMsg.size() > iPlace)
		return m_vecstrChatMsg.at( iPlace );
	else
		return s_vec_chat_str;
}
//--------------------------------------------------
SPIMESSAGE_OBJ_MAP_BEGIN(SPWindowPetChatEditor)
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP, WIID_PETCHAT_EDITOR_CLOSE, onBtnClose )
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP, WIID_PETCHAT_EDITOR_OK, onBtnOK )
	
	SPIMESSAGE_CONTROL( SPIM_SETFOCUS, WIID_PETCHAT_EDITOR_CHAT_01, onEditSetFocus )
	SPIMESSAGE_CONTROL( SPIM_SETFOCUS, WIID_PETCHAT_EDITOR_CHAT_02, onEditSetFocus )
	SPIMESSAGE_CONTROL( SPIM_SETFOCUS, WIID_PETCHAT_EDITOR_CHAT_03, onEditSetFocus )

	SPIMESSAGE_CONTROL( SPIM_KILLFOCUS, WIID_PETCHAT_EDITOR_CHAT_01, onEditKillFocus )
	SPIMESSAGE_CONTROL( SPIM_KILLFOCUS, WIID_PETCHAT_EDITOR_CHAT_02, onEditKillFocus )
	SPIMESSAGE_CONTROL( SPIM_KILLFOCUS, WIID_PETCHAT_EDITOR_CHAT_03, onEditKillFocus )

	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP, WIID_PETCHAT_EDITOR_SELECT_01, onCheckBtn_01 )
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP, WIID_PETCHAT_EDITOR_SELECT_02, onCheckBtn_02 )
	SPIMESSAGE_CONTROL( SPIM_LBUTTON_UP, WIID_PETCHAT_EDITOR_SELECT_03, onCheckBtn_03 )
SPIMESSAGE_OBJ_MAP_END(SPWindow)
//--------------------------------------------------
int SPWindowPetChatEditor::onBtnClose(WPARAM wParam, LPARAM lParam)
{
	Hide();
	return 0;
}
int SPWindowPetChatEditor::onBtnOK(WPARAM wParam, LPARAM lParam)
{
	SPPlayer *pPlayer = NULL;
	SPPetManager	*pPetMgr = NULL;
	pPlayer = g_pGOBManager->GetLocalPlayer();
	if (!pPlayer)
		return 0;

	pPetMgr = pPlayer->GetPetMgr();
	if (!pPetMgr)
		return 0;

	SPPet *pPet = pPetMgr->GetPet( m_iPetSlotIndex );
	if (!pPet)
		return 0;

	std::vector< std::string > vecstrChatMsg;
	vecstrChatMsg.clear();
	bool bAbuse = false;
	char szBuffer[128];
	int iReplace;
	//--------------------------------------------------
	string strMsg;
	if (m_pSelectMsg_01->IsChecked())
	{
		strMsg =  m_pChatMsg_01->GetWindowText();	
		SPAbuseFilter::GetInstance()->Filter(strMsg.c_str(), szBuffer, 127, &iReplace);
		if(iReplace) bAbuse = true;
		vecstrChatMsg.push_back( strMsg );
	}

	if (m_pSelectMsg_02->IsChecked())
	{
		strMsg = m_pChatMsg_02->GetWindowText();
		SPAbuseFilter::GetInstance()->Filter(strMsg.c_str(), szBuffer, 127, &iReplace);
		if(iReplace) bAbuse = true;
		vecstrChatMsg.push_back( strMsg );
	}

	if (m_pSelectMsg_03->IsChecked())
	{
		strMsg = m_pChatMsg_03->GetWindowText();
		SPAbuseFilter::GetInstance()->Filter(strMsg.c_str(), szBuffer, 127, &iReplace);
		if(iReplace) bAbuse = true;
		vecstrChatMsg.push_back( strMsg );
	}

	if (bAbuse)
	{
		if(g_pResourceManager->GetGlobalString(14000008)) {
			std::string strMsg = g_pResourceManager->GetGlobalString(14000008);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)strMsg.c_str() );
		}
		return 1;
	}

	m_vecstrChatMsg[m_iPetSlotIndex] = vecstrChatMsg;
	pPet->SetChatMsg( m_vecstrChatMsg[m_iPetSlotIndex] );
	//--------------------------------------------------

	Hide();
	_saveXmlCfg();
	return 0;
}
int SPWindowPetChatEditor::onEditSetFocus(WPARAM wParam, LPARAM lParam)
{
	g_pInterfaceManager->SetKeyInput( TRUE );
	return 0;
}
int SPWindowPetChatEditor::onEditKillFocus(WPARAM wParam, LPARAM lParam)
{
	g_pInterfaceManager->SetKeyInput( FALSE );
	return 0;
}
int		SPWindowPetChatEditor::onCheckBtn_01( WPARAM wParam, LPARAM lParam )
{
	CheckBtn( m_pSelectMsg_01 );
	return 0;
}
int		SPWindowPetChatEditor::onCheckBtn_02( WPARAM wParam, LPARAM lParam )
{
	CheckBtn( m_pSelectMsg_02 );
	return 0;
}
int		SPWindowPetChatEditor::onCheckBtn_03( WPARAM wParam, LPARAM lParam )
{
	CheckBtn( m_pSelectMsg_03 );
	return 0;
}
//--------------------------------------------------
void SPWindowPetChatEditor::SetPetIndex(PET_PLACE iPlace)
{
	m_iPetSlotIndex = iPlace;
	// load current pet's chat cfg
	if (m_vecstrChatMsg.size() > iPlace)
	{
		std::vector< std::string > &vecPetChat = m_vecstrChatMsg[m_iPetSlotIndex];
		if (vecPetChat.size() >= 3)
		{
			m_pChatMsg_01->SetWindowText( vecPetChat[0].c_str() );
			m_pChatMsg_02->SetWindowText( vecPetChat[1].c_str() );
			m_pChatMsg_03->SetWindowText( vecPetChat[2].c_str() );
		}
	}

	return;
}
//--------------------------------------------------