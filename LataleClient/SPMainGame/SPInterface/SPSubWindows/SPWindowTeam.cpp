#include "SPCommon.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowTeam.h"

#include "SPWindowStatic.h"
#include "SPWindowButton.h"
#include "SPWindowEdit.h"
#include "SPComboBoxDEF.h"
#include "SPComboBoxItemBase.h"
#include "SPComboBoxItemTexture.h"
#include "SPWindowComboBox.h"

#include "SPStage.h"
#include "SPStageManager.h"

#include "SPGameObjectDEF.h"
#include "SPGameObject.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPPlayer.h"
#include "SPGOBManager.h"

#include <WinSock2.h>
#include "PacketID.h"
#include "Packet.h"
#include "PacketHandler.h"
#include "GlobalDefines_Share.h"

#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"
#include "SPResourceManager.h"

//------------------------------------------------------------------------------------
SPWindowTeam::SPWindowTeam(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
{
	Init();
}

//------------------------------------------------------------------------------------
SPWindowTeam::SPWindowTeam(INSTANCE_ID InstanceID,
						   int iX, int iY, int iCX, int iCY, SPWindow* pParent)
						   : SPWindow(WNDID_INVALID, InstanceID, iX, iY, iCX, iCY, pParent)
{
	Init();
}

//------------------------------------------------------------------------------------
SPWindowTeam::~SPWindowTeam()
{
	Clean();
}

//------------------------------------------------------------------------------------
void SPWindowTeam::Clean()
{
	SPWindow::Clean();
}

//------------------------------------------------------------------------------------
void SPWindowTeam::Init()
{
	SPWindowStatic* pkSkinWindow, *pkChildWindow;
	SPWindowButton* pkButton;
	SPWindowEdit* pkEdit;

	pkSkinWindow = new SPWindowStatic(WIID_TEAM_SKIN_UPPER, 0, 0, 232, 4, this);
	pkSkinWindow->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 1, 31);

	pkSkinWindow = new SPWindowStatic(WIID_TEAM_SKIN_LOWER, 0, 275, 232, 4, this);
	pkSkinWindow->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 1, 41);

	pkSkinWindow = new SPWindowStatic(WIID_TEAM_SKIN_MIDDLE, 0, 4, 232, 271, this);
	pkSkinWindow->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 1, 37);
	pkSkinWindow->SetSrcSize(232, 2);

	pkChildWindow = new SPWindowStatic(WIID_TEAM_TITLE_IMAGE, 4, 3, 33, 19, pkSkinWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 64, 347);

	pkChildWindow = new SPWindowStatic(WIID_TEAM_TITLE_TEXT, 42, 3, 62, 15, pkSkinWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 202, 367);

	pkButton = new SPWindowButton(WIID_TEAM_EXIT, 208, 5, 12, 12, pkSkinWindow);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 247, 217);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI102.PNG", 247, 230);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI102.PNG", 247, 243);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI102.PNG", 247, 256);
	
	pkChildWindow = new SPWindowStatic(WIID_TEAM_SUB_BG, 2, 22, 228, 243, pkSkinWindow);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 349, 312);
	pkChildWindow->SetSrcSize(6, 6);

	pkEdit = new SPWindowEdit(WIID_TEAM_INVITE_EDIT, 10, 8, 138, 17, pkChildWindow);
	pkEdit->SetWindowType(WND_TYPE_LEFTBASE);
	pkEdit->SetMargin(2, 1);
	pkEdit->SetImageNormalHead("DATA/INTERFACE/CONCEPT/UI102.PNG", 347, 293, 349, 310);
	pkEdit->SetImageNormalBody("DATA/INTERFACE/CONCEPT/UI102.PNG", 351, 293, 353, 310);
	pkEdit->SetImageNormalTail("DATA/INTERFACE/CONCEPT/UI102.PNG", 355, 293, 357, 310);
	pkEdit->SetImageFocusHead("DATA/INTERFACE/CONCEPT/UI102.PNG", 347, 275, 349, 292);
	pkEdit->SetImageFocusBody("DATA/INTERFACE/CONCEPT/UI102.PNG", 351, 275, 353, 292);
	pkEdit->SetImageFocusTail("DATA/INTERFACE/CONCEPT/UI102.PNG", 355, 275, 357, 292);
	pkEdit->SetFontColor(D3DXCOLOR(0.0f, 0.0f, 0.0f ,1.0f));
	pkEdit->SetLimitText(LEN_NAME);

	pkButton = new SPWindowButton(WIID_TEAM_INVITE_BTN, 152, 8, 68, 17, pkChildWindow);
	pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 1, 269);
	pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI102.PNG", 1, 287);
	pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI102.PNG", 1, 305);
	pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI102.PNG", 1, 323);

	InitPlayerSlot(pkChildWindow);
	InitComboBox(pkChildWindow);
	ClearTeamPlayer();

	m_iLootingRule = 0;
	m_strMasterName = "";
}

//------------------------------------------------------------------------------------
void SPWindowTeam::InitPlayerSlot(SPWindow* pkParent)
{
	SPWindowStatic* pkChildWindow;
	SPWindowButton* pkButton;
	int i;

	RECT rcDstNumber[4], rcDstType[4];
	POINT ptSrcNumber[4], ptSrcType[4];

	rcDstNumber[0].left = 22;	rcDstNumber[0].top = 41;
	rcDstNumber[0].right = 9;	rcDstNumber[0].bottom = 21;

	rcDstNumber[1].left = 19;	rcDstNumber[1].top = 80;
	rcDstNumber[1].right = 15;	rcDstNumber[1].bottom = 21;

	rcDstNumber[2].left = 19;	rcDstNumber[2].top = 119;
	rcDstNumber[2].right = 16;	rcDstNumber[2].bottom = 21;

	rcDstNumber[3].left = 19;	rcDstNumber[3].top = 159;
	rcDstNumber[3].right = 16;	rcDstNumber[3].bottom = 21;

	ptSrcNumber[0].x = 254;		ptSrcNumber[0].y = 341;
	ptSrcNumber[1].x = 266;		ptSrcNumber[1].y = 341;
	ptSrcNumber[2].x = 282;		ptSrcNumber[2].y = 341;
	ptSrcNumber[3].x = 299;		ptSrcNumber[3].y = 341;

	rcDstType[0].left = 46;		rcDstType[0].top = 38;
	rcDstType[0].right = 13;	rcDstType[0].bottom = 12;

	rcDstType[1].left = 45;		rcDstType[1].top = 77;
	rcDstType[1].right = 15;	rcDstType[1].bottom = 13;

	rcDstType[2].left = 45;		rcDstType[2].top = 117;
	rcDstType[2].right = 15;	rcDstType[2].bottom = 13;

	rcDstType[3].left = 45;		rcDstType[3].top = 157;
	rcDstType[3].right = 15;	rcDstType[3].bottom = 13;

	ptSrcType[0].x = 363;		ptSrcType[0].y = 330;
	ptSrcType[1].x = 377;		ptSrcType[1].y = 329;
	ptSrcType[2].x = 377;		ptSrcType[2].y = 329;
	ptSrcType[3].x = 377;		ptSrcType[3].y = 329;

	for( i = 0; i < 4; i++ )
	{
		pkChildWindow = new SPWindowStatic(WIID_TEAM_SLOT_LEFT + i, 8, 33 + i * 40, 2, 36, pkParent);
		pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 263, 232);

		pkChildWindow = new SPWindowStatic(WIID_TEAM_SLOT_RIGHT + i, 218, 33 + i * 40, 2, 36, pkParent);
		pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 271, 232);

		pkChildWindow = new SPWindowStatic(WIID_TEAM_SLOT_MIDDLE + i, 10, 33 + i * 40, 208, 36, pkParent);
		pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 267, 232);
		pkChildWindow->SetSrcSize(2, 36);

		pkButton = new SPWindowButton(WIID_TEAM_SLOT_PLAYER_SECEDE + i, 192, 6, 11, 11, pkChildWindow);
		pkButton->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 247, 217);
		pkButton->SetImageHit("DATA/INTERFACE/CONCEPT/UI102.PNG", 247, 230);
		pkButton->SetImagePush("DATA/INTERFACE/CONCEPT/UI102.PNG", 247, 243);
		pkButton->SetImageDisable("DATA/INTERFACE/CONCEPT/UI102.PNG", 247, 256);

		pkChildWindow = new SPWindowStatic(WIID_TEAM_SLOT_NUMBER + i,
			rcDstNumber[i].left, rcDstNumber[i].top,
			rcDstNumber[i].right, rcDstNumber[i].bottom, pkParent);
		pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", ptSrcNumber[i].x, ptSrcNumber[i].y);

		pkChildWindow = new SPWindowStatic(WIID_TEAM_SLOT_PLAYER_TYPE + i,
			rcDstType[i].left, rcDstType[i].top,
			rcDstType[i].right, rcDstType[i].bottom, pkParent);
		pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", ptSrcType[i].x, ptSrcType[i].y);

		pkChildWindow = new SPWindowStatic(WIID_TEAM_SLOT_PLAYER_ID + i, 67, 38 + i * 40, 134, 12, pkParent);
		pkChildWindow->SetFontColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
		pkChildWindow->SetFormat(DT_VCENTER | DT_LEFT);

		pkChildWindow = new SPWindowStatic(WIID_TEAM_SLOT_PLAYER_POS + i, 57, 53 + i * 40, 156, 12, pkParent);
		pkChildWindow->SetFontColor(D3DXCOLOR(0.2745f, 0.5490f, 0.4862f, 1.0f));
		pkChildWindow->SetFormat(DT_VCENTER | DT_LEFT);
	}
}

//------------------------------------------------------------------------------------
void SPWindowTeam::InitComboBox(SPWindow* pkParent)
{
	SPWindowStatic* pkChildWindow;
	SPWindowComboBox* pkComboBox;

	pkChildWindow = new SPWindowStatic(WIID_TEAM_LOOTING_RULE_IMAGE, 8, 197, 79, 17, pkParent);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG",432, 307);

	pkChildWindow = new SPWindowStatic(WIID_TEAM_CHANGE_MASTER_IMAGE, 8, 218, 79, 17, pkParent);
	pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG",432, 325);

	//////////////////////////////////////////////////////////////////////////

	pkComboBox = new SPWindowComboBox(WIID_TEAM_LOOTING_RULE_COMBO, 91, 197, 129, 17, pkParent);

	pkComboBox->SetDropDownOffset( 118 , 6 );
	pkComboBox->SetDropDownSize( 9 , 6 );
	pkComboBox->SetDropDownTexture( COMBOBOX_DATA_TEXTURE_ENABLE		, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 65 , 77 );
	pkComboBox->SetDropDownTexture( COMBOBOX_DATA_TEXTURE_HIT		, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 65 , 77 );
	pkComboBox->SetDropDownTexture( COMBOBOX_DATA_TEXTURE_PUSH		, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 75 , 77 );
	pkComboBox->SetScroll(FALSE);

	pkComboBox->SetListBoxItemMargin(1, 1, 1, 1);
	pkComboBox->SetShowEditBoxText(TRUE);
	pkComboBox->SetEditBoxOffset(0, 0);
	pkComboBox->SetEditBoxSize( 129, 17 );
	pkComboBox->SetEditBoxTextColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
	pkComboBox->SetEditBoxTexture( COMBOBOX_DATA_TEXTURE_ENABLE	, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 17 , 77 , 32 , 92 );
	pkComboBox->SetEditBoxTexture( COMBOBOX_DATA_TEXTURE_HIT		, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 33 , 77 , 48 , 92 );
	pkComboBox->SetEditBoxTexture( COMBOBOX_DATA_TEXTURE_PUSH	, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 49 , 77 , 64 , 92 );

	pkComboBox->SetListBoxOffset(0, 17);
	pkComboBox->SetListBoxTexture( "DATA/INTERFACE/CONCEPT/UI102.PNG", 1 , 77 , 16 , 92 );
	pkComboBox->SetRenderItemCountMax(3);

	SPComboBoxItemTexture* pkComboItem;
	pkComboItem = new SPComboBoxItemTexture;
	pkComboItem->SetSize(127, 14);
	pkComboItem->SetText(g_pResourceManager->GetGlobalString(4002101));
	pkComboItem->SetTextColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
	pkComboItem->SetTexture( COMBOBOX_DATA_TEXTURE_HIT , "DATA/INTERFACE/CONCEPT/UI101.PNG" , 11 , 52 , 13 , 54 );
	pkComboItem->SetTextureMargin( COMBOBOX_DATA_TEXTURE_HIT		, 1 , 0 , 0 , 0 );
	pkComboBox->AddItem(pkComboItem);

	pkComboItem = new SPComboBoxItemTexture;
	pkComboItem->SetSize(127, 14);
	pkComboItem->SetText(g_pResourceManager->GetGlobalString(4002103));
	pkComboItem->SetTextColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
	pkComboItem->SetTexture( COMBOBOX_DATA_TEXTURE_HIT , "DATA/INTERFACE/CONCEPT/UI101.PNG" , 11 , 52 , 13 , 54 );
	pkComboItem->SetTextureMargin( COMBOBOX_DATA_TEXTURE_HIT		, 1 , 0 , 0 , 0 );
	pkComboBox->AddItem(pkComboItem);

	pkComboItem = new SPComboBoxItemTexture;
	pkComboItem->SetSize(127, 14);
	pkComboItem->SetText(g_pResourceManager->GetGlobalString(4002102));
	pkComboItem->SetTextColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
	pkComboItem->SetTexture( COMBOBOX_DATA_TEXTURE_HIT , "DATA/INTERFACE/CONCEPT/UI101.PNG" , 11 , 52 , 13 , 54 );
	pkComboItem->SetTextureMargin( COMBOBOX_DATA_TEXTURE_HIT		, 1 , 0 , 0 , 0 );
	pkComboBox->AddItem(pkComboItem);

	//////////////////////////////////////////////////////////////////////////

	pkComboBox = new SPWindowComboBox(WIID_TEAM_CHANGE_MASTER_COMBO , 91, 218, 129, 17, pkParent);

	pkComboBox->SetDropDownOffset( 118 , 6 );
	pkComboBox->SetDropDownSize( 9 , 6 );
	pkComboBox->SetDropDownTexture( COMBOBOX_DATA_TEXTURE_ENABLE		, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 65 , 77 );
	pkComboBox->SetDropDownTexture( COMBOBOX_DATA_TEXTURE_HIT		, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 65 , 77 );
	pkComboBox->SetDropDownTexture( COMBOBOX_DATA_TEXTURE_PUSH		, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 75 , 77 );
	pkComboBox->SetScroll(FALSE);

	pkComboBox->SetListBoxItemMargin(1, 1, 1, 1);
	pkComboBox->SetShowEditBoxText(TRUE);
	pkComboBox->SetEditBoxOffset(0, 0);
	pkComboBox->SetEditBoxSize( 129, 17 );
	pkComboBox->SetEditBoxTextColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
	pkComboBox->SetEditBoxTexture( COMBOBOX_DATA_TEXTURE_ENABLE	, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 17 , 77 , 32 , 92 );
	pkComboBox->SetEditBoxTexture( COMBOBOX_DATA_TEXTURE_HIT		, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 33 , 77 , 48 , 92 );
	pkComboBox->SetEditBoxTexture( COMBOBOX_DATA_TEXTURE_PUSH	, "DATA/INTERFACE/CONCEPT/UI102.PNG" , 49 , 77 , 64 , 92 );

	pkComboBox->SetListBoxOffset(0, 17);
	pkComboBox->SetListBoxTexture( "DATA/INTERFACE/CONCEPT/UI102.PNG", 1 , 77 , 16 , 92 );
	pkComboBox->SetRenderItemCountMax(4);

}

//------------------------------------------------------------------------------------
void SPWindowTeam::Show(bool bWithChild)
{
	SPWindow::Show(bWithChild);

	if( m_iPlayerNum == 0 )
		ClearTeamPlayer();

	SPWindow* pkWindow = Find(WIID_TEAM_INVITE_EDIT, true);
	if( pkWindow )
		pkWindow->SetWindowText(NULL);
}

//------------------------------------------------------------------------------------
void SPWindowTeam::Render(float fTime)
{
	if( !m_bShow )
		return;

	////////////////////////////////////////////////////////////////////////////
	////
	////	임시 코드
	////

	SPWindowComboBox*	pComboWindowMaster	=	(SPWindowComboBox*)Find( WIID_TEAM_CHANGE_MASTER_COMBO , true );
	SPWindowComboBox*	pComboWindowRule	=	(SPWindowComboBox*)Find( WIID_TEAM_LOOTING_RULE_COMBO , true );

	if( pComboWindowMaster != NULL )
		pComboWindowMaster->NoRender();

	if( pComboWindowRule != NULL )
		pComboWindowRule->NoRender();

	SPWindow::Render(fTime);

	if( pComboWindowMaster != NULL )
	{
		pComboWindowMaster->Show( false );
		pComboWindowMaster->Render( fTime );
	}

	if( pComboWindowRule != NULL )
	{
		pComboWindowRule->Show( false );
		pComboWindowRule->Render( fTime );
	}

}

//------------------------------------------------------------------------------------
void SPWindowTeam::SetTeamPlayer(int iIndex, TEAM_MEMBER* pMemberInfo)
{
	SPWindow* pkWindow;

	pkWindow = Find(WIID_TEAM_SLOT_PLAYER_ID + iIndex, true);
	if( pkWindow )
		pkWindow->SetWindowText(pMemberInfo->szCharName);

	pkWindow = Find(WIID_TEAM_SLOT_PLAYER_POS + iIndex, true);
	if( pkWindow )
	{
		SPStage* pkCurStage = g_StageManager.GetCurStage(pMemberInfo->usStageID);
		if( pkCurStage )
		{
			SPMapGroup* pkMapGroup = pkCurStage->GetMapGroup(pMemberInfo->usMapGroupID);
			if( pkMapGroup )
			{
				string strPos = "- ";
				strPos += pkMapGroup->m_strGroupName;
				pkWindow->SetWindowText(strPos.c_str());
			}
		}
#if !defined(_CHANNEL_TEAM_VERSION_)
		UINT16	uiServerID	=	(UINT16)g_pNetworkManager->GetServerID();
		if( pMemberInfo->wServerID != uiServerID )
		{
			UINT16	uiChannel	=	pMemberInfo->wServerID % 100;

			int	iItemID	=	4002201;

			if( uiChannel > INDUN_PVP_CHANNEL_MAX )
				iItemID	=	4002203;
			else if( uiChannel > INDUN_CHANNEL_MAX )
				iItemID	=	4002202;

			const char*	pszGlobalString	=	g_pResourceManager->GetGlobalString( iItemID );

			if( pszGlobalString != NULL )
			{
				char	szBuf[ _MAX_PATH ];
				sprintf( szBuf , pszGlobalString , uiChannel );
				pkWindow->SetWindowText( szBuf );
			}
		}
#endif
	}

	pkWindow = Find(WIID_TEAM_SLOT_PLAYER_SECEDE + iIndex, true);
	if( pkWindow )
	{
		bool bButtonEnable = false;
		if( g_pGOBManager->GetLocalPlayer() )
		{
			if( g_pGOBManager->GetLocalPlayer()->GetGUID() == pMemberInfo->CharGUID ||
				g_pGOBManager->GetLocalPlayer()->IsMaster() )
				bButtonEnable = true;
		}

		pkWindow->Show();
		pkWindow->SetEnable(bButtonEnable);
	}

	pkWindow = Find(WIID_TEAM_SLOT_PLAYER_TYPE + iIndex, true);
	if( pkWindow )
		pkWindow->Show();

	if( pMemberInfo->byGrade )
		m_strMasterName = pMemberInfo->szCharName;

	SPWindowComboBox* pkComboBox = (SPWindowComboBox*)Find(WIID_TEAM_CHANGE_MASTER_COMBO, true);
	if( pkComboBox )
	{
		SPComboBoxItemTexture* pkComboItem;
		pkComboItem = new SPComboBoxItemTexture;
		pkComboItem->SetSize(127, 14);
		pkComboItem->SetText(pMemberInfo->szCharName);
		pkComboItem->SetTextColor(D3DXCOLOR(0.1921f, 0.2980f, 0.3803f, 1.0f));
		pkComboItem->SetTexture( COMBOBOX_DATA_TEXTURE_HIT , "DATA/INTERFACE/CONCEPT/UI101.PNG" , 11 , 52 , 13 , 54 );
		pkComboItem->SetTextureMargin( COMBOBOX_DATA_TEXTURE_HIT		, 1 , 0 , 0 , 0 );
		pkComboBox->AddItem(pkComboItem);
	}
}

//------------------------------------------------------------------------------------
void SPWindowTeam::ClearTeamPlayer()
{
	int i;
	SPWindow* pkWindow;

	for( i = 0; i < 4; i++ )
	{
		pkWindow = Find(WIID_TEAM_SLOT_PLAYER_ID + i, true);
		if( pkWindow )
			pkWindow->SetWindowText(NULL);

		pkWindow = Find(WIID_TEAM_SLOT_PLAYER_POS + i, true);
		if( pkWindow )
			pkWindow->SetWindowText(NULL);

		pkWindow = Find(WIID_TEAM_SLOT_PLAYER_SECEDE + i, true);
		if( pkWindow )
		{
			pkWindow->Hide();
			pkWindow->SetEnable(false);
		}

		pkWindow = Find(WIID_TEAM_SLOT_PLAYER_TYPE + i, true);
		if( pkWindow )
			pkWindow->Hide();
	}

	SPWindowComboBox* pkComboBox = (SPWindowComboBox*)Find(WIID_TEAM_CHANGE_MASTER_COMBO, true);
	if( pkComboBox )
		pkComboBox->DelAllItem();

	m_iLootingRule = 0;
	m_strMasterName = "";
	m_iPlayerNum = 0;

	pkComboBox = (SPWindowComboBox*)Find(WIID_TEAM_LOOTING_RULE_COMBO, true);
	if( pkComboBox )
		pkComboBox->SetSelectNumber( m_iLootingRule );
}

//------------------------------------------------------------------------------------
// Message
int SPWindowTeam::OnExit(WPARAM wParam, LPARAM lParam)
{
	Hide();
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowTeam::OnPlayerInvite(WPARAM wParam, LPARAM lParam)
{
	SPWindow* pkWindow;

	pkWindow = Find(WIID_TEAM_INVITE_EDIT, true);
	if( pkWindow )
	{
		SPPlayer* pkLocalPlayer;
		if( pkLocalPlayer = g_pGOBManager->GetLocalPlayer() )
			pkLocalPlayer->SPGOBPerformMessage(SPGM_TEAM_INVITE_REQ, (LPARAM)pkWindow->GetWindowText());

		pkWindow->SetWindowText(NULL);
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowTeam::OnInviteEditSetFocus(WPARAM wParam, LPARAM lParam)
{
	g_pInterfaceManager->SetKeyInput(TRUE);
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowTeam::OnInviteEditKillFocus(WPARAM wParam, LPARAM lParam)
{
	g_pInterfaceManager->SetKeyInput(FALSE);
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowTeam::OnJoin(WPARAM wParam, LPARAM lParam)
{
	vector< TEAM_MEMBER* >* pTeamList = (vector< TEAM_MEMBER* >*)wParam;
	GU_ID* pLocalID = (GU_ID*)lParam;

	if( pTeamList == NULL || (*pLocalID) == 0 )
		return 1;

	int iOldLootingRule = m_iLootingRule;

	ClearTeamPlayer();

	m_iLootingRule = iOldLootingRule;

	vector< TEAM_MEMBER* >::iterator iter = (*pTeamList).begin();
	int iCount = 0;
	while( iter != (*pTeamList).end() )
	{
		SetTeamPlayer(iCount, *iter);

		++iter;
		++iCount;
		++m_iPlayerNum;
	}

	bool bInvite = true;
	if( iCount >= 4 )
		bInvite = false;

	SPWindow* pkWindow = Find(WIID_TEAM_INVITE_EDIT, true);
	if( pkWindow )
		pkWindow->SetEnable(bInvite);

	pkWindow = Find(WIID_TEAM_INVITE_BTN, true);
	if( pkWindow )
		pkWindow->SetEnable(bInvite);

	SPWindowComboBox* pkLootingComboBox, *pkMasterComboBox;

	pkLootingComboBox = (SPWindowComboBox*)Find(WIID_TEAM_LOOTING_RULE_COMBO, true);
	pkMasterComboBox = (SPWindowComboBox*)Find(WIID_TEAM_CHANGE_MASTER_COMBO, true);
	if( pkLootingComboBox && pkMasterComboBox )
	{
		if( iCount )
			pkMasterComboBox->SetEditBoxItem(0);

		if( iCount == 0 || g_pGOBManager->GetLocalPlayer()->IsMaster() )
		{
			pkLootingComboBox->SetDropDownStatus(COMBOBOX_DATA_TEXTURE_ENABLE);
			pkMasterComboBox->SetDropDownStatus(COMBOBOX_DATA_TEXTURE_ENABLE);
		}
		else
		{
			pkLootingComboBox->SetDropDownStatus(COMBOBOX_DATA_TEXTURE_DISABLE);
			pkMasterComboBox->SetDropDownStatus(COMBOBOX_DATA_TEXTURE_DISABLE);
		}

		pkLootingComboBox->SetSelectNumber( m_iLootingRule );
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowTeam::OnSecede(WPARAM wParam, LPARAM lParam)
{
	OnJoin(wParam, lParam);
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowTeam::OnUpdate(WPARAM wParam, LPARAM lParam)
{
	TEAM_MEMBER* pMemberInfo = (TEAM_MEMBER*)wParam;
	if( pMemberInfo == NULL )
		return 1;

	SPWindow* pkWindow;
	int i;

	for( i = 0; i < 4; i++ )
	{
		pkWindow = Find(WIID_TEAM_SLOT_PLAYER_ID + i, true);
		if( pkWindow == NULL )
			continue;

		if( pkWindow->GetWindowText() == NULL )
			break;

		if( strcmp(pkWindow->GetWindowText(), pMemberInfo->szCharName) != 0 )
			continue;

		pkWindow = Find(WIID_TEAM_SLOT_PLAYER_POS + i, true);
		if( pkWindow )
		{
			SPStage* pkCurStage = g_StageManager.GetCurStage(pMemberInfo->usStageID);
			if( pkCurStage )
			{
				SPMapGroup* pkMapGroup = pkCurStage->GetMapGroup(pMemberInfo->usMapGroupID);
				if( pkMapGroup )
				{
					string strPos = "- ";
					strPos += pkMapGroup->m_strGroupName;
					pkWindow->SetWindowText(strPos.c_str());
				}
			}
#if !defined(_CHANNEL_TEAM_VERSION_)
			UINT16	uiServerID	=	(UINT16)g_pNetworkManager->GetServerID();
			if( pMemberInfo->wServerID != uiServerID )
			{
				UINT16	uiChannel	=	pMemberInfo->wServerID % 100;

				int	iItemID	=	4002201;

				if( uiChannel > INDUN_PVP_CHANNEL_MAX )
					iItemID	=	4002203;
				else if( uiChannel > INDUN_CHANNEL_MAX )
					iItemID	=	4002202;

				const char*	pszGlobalString	=	g_pResourceManager->GetGlobalString( iItemID );

				if( pszGlobalString != NULL )
				{
					char	szBuf[ _MAX_PATH ];
					sprintf( szBuf , pszGlobalString , uiChannel );
					pkWindow->SetWindowText( szBuf );
				}
			}
#endif
		}

		break;
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowTeam::OnUpdateLootingRule(WPARAM wParam, LPARAM lParam)
{
	SPWindowComboBox* pkComboBox = (SPWindowComboBox*)Find(WIID_TEAM_LOOTING_RULE_COMBO, true);
	if( pkComboBox == NULL )
		return 1;

	pkComboBox->SetEditBoxItem(wParam);
	if( lParam )
		pkComboBox->SetDropDownStatus(COMBOBOX_DATA_TEXTURE_ENABLE);
	else
		pkComboBox->SetDropDownStatus(COMBOBOX_DATA_TEXTURE_DISABLE);

	m_iLootingRule = wParam;
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowTeam::OnSecedeReq(unsigned int uiID, WPARAM, LPARAM)
{
	int iIndex = uiID - WIID_TEAM_SLOT_PLAYER_SECEDE;

	SPWindow* pkWindow = Find(WIID_TEAM_SLOT_PLAYER_ID + iIndex, true);
	if( pkWindow && g_pGOBManager->GetLocalPlayer() )
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_TEAM_SECEDE_REQ, (LPARAM)pkWindow->GetWindowText());

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowTeam::OnChangeLootingRuleReq(WPARAM wParam, LPARAM lParam)
{
	SPWindowComboBox* pkComboBox = (SPWindowComboBox*)Find(WIID_TEAM_LOOTING_RULE_COMBO, true);
	if( pkComboBox == NULL )
		return 1;

	int iChangeRule = pkComboBox->GetSelectNumber();
	if( m_iLootingRule != iChangeRule )
		g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_TEAM_CHANGE_LOOTING_REQ, iChangeRule);

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowTeam::OnChangeMasterReq(WPARAM wParam, LPARAM lParam)
{
	SPWindowComboBox* pkComboBox = (SPWindowComboBox*)Find(WIID_TEAM_CHANGE_MASTER_COMBO, true);
	if( pkComboBox == NULL )
		return 1;

	int iChangeMaster = pkComboBox->GetSelectNumber();
	if( iChangeMaster != 0 )
	{
		SPComboBoxItemBase* pkItem = pkComboBox->GetItem(iChangeMaster);
		if( pkItem )
		{
			g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(
				SPGM_TEAM_CHANGE_MASTER_REQ, (LPARAM)pkItem->GetText());
		}
	}

	return 1;
}

//------------------------------------------------------------------------------------
SPIMESSAGE_OBJ_MAP_BEGIN(SPWindowTeam)

SPIMESSAGE_COMMAND(	SPIM_TEAM_JOIN,					OnJoin		)
SPIMESSAGE_COMMAND(	SPIM_TEAM_SECEDE,				OnSecede	)
SPIMESSAGE_COMMAND(	SPIM_TEAM_UPDATE,				OnUpdate	)
SPIMESSAGE_COMMAND(	SPIM_TEAM_UPDATE_LOOTING_RULE,	OnUpdateLootingRule	)

SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,	WIID_TEAM_EXIT,				OnExit			)
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,	WIID_TEAM_INVITE_BTN,		OnPlayerInvite	)

SPIMESSAGE_CONTROL( SPIM_SETFOCUS,	WIID_TEAM_INVITE_EDIT,		OnInviteEditSetFocus)
SPIMESSAGE_CONTROL( SPIM_KILLFOCUS, WIID_TEAM_INVITE_EDIT,		OnInviteEditKillFocus)

SPIMESSAGE_CONTROL( SPIM_COMBOBOX_CHANGE_VALUE, WIID_TEAM_LOOTING_RULE_COMBO,		OnChangeLootingRuleReq)
SPIMESSAGE_CONTROL( SPIM_COMBOBOX_CHANGE_VALUE, WIID_TEAM_CHANGE_MASTER_COMBO,		OnChangeMasterReq)

SPIMESSAGE_CONTROL_RANGE( SPIM_LBUTTON_UP, WIID_TEAM_SLOT_PLAYER_SECEDE,	WIID_TEAM_SLOT_PLAYER_SECEDE + 3, OnSecedeReq)

SPIMESSAGE_OBJ_MAP_END(SPWindow)