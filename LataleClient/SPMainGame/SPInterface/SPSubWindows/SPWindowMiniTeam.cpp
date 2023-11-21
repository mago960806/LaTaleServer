#include "SPCommon.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowMiniTeam.h"

#include "SPWindowStatic.h"
#include "SPWindowButton.h"

#include "SPGameObjectDEF.h"
#include "SPGameObject.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPPlayer.h"
#include "SPGOBManager.h"

using namespace std;

//------------------------------------------------------------------------------------
SPWindowMiniTeam::SPWindowMiniTeam(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
{
	Init();
}

//------------------------------------------------------------------------------------
SPWindowMiniTeam::SPWindowMiniTeam(INSTANCE_ID InstanceID,
								   int iX, int iY, int iCX, int iCY, SPWindow* pParent)
								   : SPWindow(WNDID_INVALID, InstanceID, iX, iY, iCX, iCY, pParent)
{
	Init();
}

//------------------------------------------------------------------------------------
SPWindowMiniTeam::~SPWindowMiniTeam()
{
	Clean();
}

//------------------------------------------------------------------------------------
void SPWindowMiniTeam::Clean()
{
	SPWindow::Clean();
}

//------------------------------------------------------------------------------------
void SPWindowMiniTeam::Init()
{
	SPWindowStatic* pkSkinWindow, *pkChildWindow, *pkChildWindow2;
	int i;

	m_iInterval = 34;
	m_iGageWidth = 107;

	pkSkinWindow = new SPWindowStatic(WIID_MINI_TEAM_SKIN_UPPER, 0, 0, 125, 2, this);
	pkSkinWindow->SetImage("DATA/INTERFACE/CONCEPT/UI100.PNG", 274, 20);

	pkSkinWindow = new SPWindowStatic(WIID_MINI_TEAM_SKIN_LOWER, 0, 33, 125, 2, this);
	pkSkinWindow->SetImage("DATA/INTERFACE/CONCEPT/UI100.PNG", 274, 28);

	pkSkinWindow = new SPWindowStatic(WIID_MINI_TEAM_SKIN_MIDDLE, 0, 2, 125, 31, this);
	pkSkinWindow->SetImage("DATA/INTERFACE/CONCEPT/UI100.PNG", 274, 24);
	pkSkinWindow->SetSrcSize(125, 2);
	
	for( i = 0; i < 4; i++ )
	{
		pkChildWindow = new SPWindowStatic(WIID_MINI_TEAM_HEAD + i,
			0, m_iInterval * i, 100, m_iInterval, this);

		pkChildWindow2 = new SPWindowStatic(WIID_MINI_TEAM_TYPE + i,
			3, 4, 15, 13, pkChildWindow);
		pkChildWindow2->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 362, 329);

		pkChildWindow2 = new SPWindowStatic(WIID_MINI_TEAM_NAME + i,
			17, 5, 91, 12, pkChildWindow);
		pkChildWindow2->SetFormat(DT_VCENTER | DT_LEFT);
		pkChildWindow2->SetFontColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		pkChildWindow2 = new SPWindowStatic(WIID_MINI_TEAM_HP_BG_L + i,
			8, 19, 2, 6, pkChildWindow);
		pkChildWindow2->SetImage("DATA/INTERFACE/CONCEPT/UI100.PNG", 251, 20);

		pkChildWindow2 = new SPWindowStatic(WIID_MINI_TEAM_HP_BG_R + i,
			115, 19, 2, 6, pkChildWindow);
		pkChildWindow2->SetImage("DATA/INTERFACE/CONCEPT/UI100.PNG", 259, 20);

		pkChildWindow2 = new SPWindowStatic(WIID_MINI_TEAM_HP_BG_M + i,
			10, 19, 105, 6, pkChildWindow);
		pkChildWindow2->SetImage("DATA/INTERFACE/CONCEPT/UI100.PNG", 255, 20);
		pkChildWindow2->SetSrcSize(2, 6);

		pkChildWindow2 = new SPWindowStatic(WIID_MINI_TEAM_HP_C + i,
			9, 20, 105, 6, pkChildWindow);
		pkChildWindow2->SetImage("DATA/INTERFACE/CONCEPT/UI100.PNG", 263, 20);
		pkChildWindow2->SetSrcSize(2, 4);

		pkChildWindow2 = new SPWindowStatic(WIID_MINI_TEAM_HP_R + i,
			9, 20, 1, 4, pkChildWindow);
		pkChildWindow2->SetImage("DATA/INTERFACE/CONCEPT/UI100.PNG", 267, 20);

		pkChildWindow2 = new SPWindowStatic(WIID_MINI_TEAM_SP_BG_L + i,
			8, 25, 2, 6, pkChildWindow);
		pkChildWindow2->SetImage("DATA/INTERFACE/CONCEPT/UI100.PNG", 251, 20);

		pkChildWindow2 = new SPWindowStatic(WIID_MINI_TEAM_SP_BG_R + i,
			115, 25, 2, 6, pkChildWindow);
		pkChildWindow2->SetImage("DATA/INTERFACE/CONCEPT/UI100.PNG", 259, 20);

		pkChildWindow2 = new SPWindowStatic(WIID_MINI_TEAM_SP_BG_M + i,
			10, 25, 105, 6, pkChildWindow);
		pkChildWindow2->SetImage("DATA/INTERFACE/CONCEPT/UI100.PNG", 255, 20);
		pkChildWindow2->SetSrcSize(2, 6);

		pkChildWindow2 = new SPWindowStatic(WIID_MINI_TEAM_SP_C + i,
			9, 26, 105, 6, pkChildWindow);
		pkChildWindow2->SetImage("DATA/INTERFACE/CONCEPT/UI100.PNG", 263, 25);
		pkChildWindow2->SetSrcSize(2, 4);

		pkChildWindow2 = new SPWindowStatic(WIID_MINI_TEAM_SP_R + i,
			9, 26, 1, 4, pkChildWindow);
		pkChildWindow2->SetImage("DATA/INTERFACE/CONCEPT/UI100.PNG", 267, 25);

		pkChildWindow2 = new SPWindowStatic(WIID_MINI_TEAM_OUT_RANGE + i,
			11, 20, 103, 11, pkChildWindow);
		pkChildWindow2->SetImage("DATA/INTERFACE/CONCEPT/UI100.PNG", 296, 31);
		pkChildWindow2->SetImage("DATA/INTERFACE/CONCEPT/UI100.PNG", 109, 486);

		pkChildWindow = new SPWindowStatic(WIID_MINI_TEAM_SEPARATE_LINE + i,
			1, 34 + m_iInterval * i, 123, 1, this);
		pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI100.PNG", 255, 20);
		pkChildWindow->SetSrcSize(2, 1);
	}
}

//------------------------------------------------------------------------------------
void SPWindowMiniTeam::Show(bool bWithChild)
{
	if( g_pGOBManager->GetLocalPlayer() &&
		!g_pGOBManager->GetLocalPlayer()->GetTeamActive() )
		return;

	m_bShow = true;
	//SPWindow::Show(bWithChild);
}

//------------------------------------------------------------------------------------
void SPWindowMiniTeam::Hide(bool bSendServer)
{
	m_bShow = false;
}

//------------------------------------------------------------------------------------
void SPWindowMiniTeam::Close()
{
	m_bShow = false;
}

//------------------------------------------------------------------------------------
void SPWindowMiniTeam::HideSkinWindow()
{
	SPWindow* pkWindow;
	int i;

	pkWindow = Find(WIID_MINI_TEAM_SKIN_UPPER, true);
	if( pkWindow )
		pkWindow->Hide();

	pkWindow = Find(WIID_MINI_TEAM_SKIN_MIDDLE, true);
	if( pkWindow )
		pkWindow->Hide();

	pkWindow = Find(WIID_MINI_TEAM_SKIN_LOWER, true);
	if( pkWindow )
		pkWindow->Hide();

	for( i = 0; i < 4; i++ )
	{
		pkWindow = Find(WIID_MINI_TEAM_HEAD + i, true);
		if( pkWindow )
			pkWindow->Hide();

		pkWindow = Find(WIID_MINI_TEAM_SEPARATE_LINE + i, true);
		if( pkWindow )
			pkWindow->Hide();
	}
}

//------------------------------------------------------------------------------------
void SPWindowMiniTeam::ShowTeamWindow(int iIndex, TEAM_MEMBER* pMemberInfo)
{
	SPWindow *pkHeadWindow, *pkChildWindow;

	pkHeadWindow = Find(WIID_MINI_TEAM_HEAD + iIndex);
	if( pkHeadWindow == NULL )
		return;

	pkHeadWindow->Show();

	pkChildWindow = pkHeadWindow->Find(WIID_MINI_TEAM_NAME + iIndex);
	if( pkChildWindow )
		pkChildWindow->SetWindowText(pMemberInfo->szCharName);

	pkChildWindow = pkHeadWindow->Find(WIID_MINI_TEAM_TYPE + iIndex);
	if( pkChildWindow )
	{
		pkChildWindow->SPSendMessage(SPIM_SET_IMAGECLEAR);
		if( pMemberInfo->byGrade )
			pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 362, 329);
		else
			pkChildWindow->SetImage("DATA/INTERFACE/CONCEPT/UI102.PNG", 377, 329);
	}
}

//------------------------------------------------------------------------------------
// Message
int SPWindowMiniTeam::OnJoin(WPARAM wParam, LPARAM lParam)
{
	vector< TEAM_MEMBER* >* pTeamList = (vector< TEAM_MEMBER* >*)wParam;
	GU_ID* pLocalID = (GU_ID*)lParam;

	HideSkinWindow();
	if( pTeamList == NULL || (*pLocalID) == 0 )
		return 1;

	int iCount = 0;
	vector< TEAM_MEMBER* >::iterator iter = (*pTeamList).begin();
	while( iter != (*pTeamList).end() )
	{
		//if( (*pLocalID) == (*iter)->CharGUID )
		//{
		//	++iter;
		//	continue;
		//}

		ShowTeamWindow(iCount, *iter);

		++iter;
		++iCount;
	}

	SPWindow *pkChildWindow;
	int iSize = (int)(*pTeamList).size();

	if( iSize )
	{
		pkChildWindow = Find(WIID_MINI_TEAM_SKIN_UPPER, true);
		if( pkChildWindow )
		{
			if( !pkChildWindow->IsShow() )
				pkChildWindow->Show();
		}

		pkChildWindow = Find(WIID_MINI_TEAM_SKIN_MIDDLE, true);
		if( pkChildWindow )
		{
			pkChildWindow->AdjustCoord(0, 2, 125, m_iInterval * iSize);
			pkChildWindow->SPSendMessage(SPIM_POS_UPDATE);
			if( !pkChildWindow->IsShow() )
				pkChildWindow->Show();
		}

		pkChildWindow = Find(WIID_MINI_TEAM_SKIN_LOWER, true);
		if( pkChildWindow )
		{
			pkChildWindow->AdjustCoord(0, 2 + m_iInterval * iSize, 125, 2);
			pkChildWindow->SPSendMessage(SPIM_POS_UPDATE);
			if( !pkChildWindow->IsShow() )
				pkChildWindow->Show();
		}

		int i;
		for( i = 1; i < iSize; i++ )
		{
			pkChildWindow = Find(WIID_MINI_TEAM_SEPARATE_LINE + (i - 1), true);
			if( pkChildWindow )
				pkChildWindow->Show();
		}
	}

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowMiniTeam::OnSecede(WPARAM wParam, LPARAM lParam)
{
	OnJoin(wParam, lParam);
	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowMiniTeam::OnUpdate(WPARAM wParam, LPARAM lParam)
{
	/*
	TEAM_MEMBER* pMemberInfo = (TEAM_MEMBER*)wParam;
	if( pMemberInfo == NULL )
		return 1;

	SPWindow* pkWindow;
	int i;

	for( i = 0; i < 3; i++ )
	{
		pkWindow = Find(WIID_MINI_TEAM_CHAR_NAME + i, true);
		if( pkWindow == NULL )
			continue;

		if( pkWindow->GetWindowText() == NULL )
			continue;

		if( strcmp(pkWindow->GetWindowText(), pMemberInfo->szCharName) != 0 )
			continue;

		pkWindow = Find(WIID_MINI_TEAM_LEVEL + i, true);
		if( pkWindow )
		{
			char szBuf[64];
			sprintf(szBuf, "%d", pMemberInfo->sLv);
			pkWindow->SetWindowText(szBuf);
		}

		break;
	}
	*/

	return 1;
}

//------------------------------------------------------------------------------------
int SPWindowMiniTeam::OnUpdateStatus(WPARAM wParam, LPARAM lParam)
{
	STATUS_INFO* pStatusInfo = (STATUS_INFO*)wParam;

	SPWindow* pkWindow;
	int i, iWidth;

	for( i = 0; i < 4; i++ )
	{
		pkWindow = Find(WIID_MINI_TEAM_NAME + i, true);
		if( pkWindow == NULL )
			continue;

		if( strcmp(pkWindow->GetWindowText(), pStatusInfo->szCharName) != 0 )
			continue;

		SPWindow* pkWindowHP_C, *pkWindowHP_R;
		SPWindow* pkWindowSP_C, *pkWindowSP_R;
		SPWindow* pkWindowOutRange, *pkWindowBack;

		pkWindowHP_C = Find(WIID_MINI_TEAM_HP_C + i, true);
		if( pkWindowHP_C == NULL )
			break;

		pkWindowHP_R = Find(WIID_MINI_TEAM_HP_R + i, true);
		if( pkWindowHP_R == NULL )
			break;

		pkWindowSP_C = Find(WIID_MINI_TEAM_SP_C + i, true);
		if( pkWindowSP_C == NULL )
			break;

		pkWindowSP_R = Find(WIID_MINI_TEAM_SP_R + i, true);
		if( pkWindowSP_R == NULL )
			break;

		pkWindowOutRange = Find(WIID_MINI_TEAM_OUT_RANGE + i, true);
		if( pkWindowOutRange == NULL )
			break;

		if( pStatusInfo->bOutofRange )
		{
			if( pkWindowHP_C->IsShow() )
				pkWindowHP_C->Hide();
			if( pkWindowHP_R->IsShow() )
				pkWindowHP_R->Hide();
			if( pkWindowSP_C->IsShow() )
				pkWindowSP_C->Hide();
			if( pkWindowSP_R->IsShow() )
				pkWindowSP_R->Hide();

			pkWindowBack = Find(WIID_MINI_TEAM_HP_BG_L + i, true);
			if( pkWindowBack )
				pkWindowBack->Hide();

			pkWindowBack = Find(WIID_MINI_TEAM_HP_BG_M + i, true);
			if( pkWindowBack )
				pkWindowBack->Hide();

			pkWindowBack = Find(WIID_MINI_TEAM_HP_BG_R + i, true);
			if( pkWindowBack )
				pkWindowBack->Hide();

			pkWindowBack = Find(WIID_MINI_TEAM_SP_BG_L + i, true);
			if( pkWindowBack )
				pkWindowBack->Hide();

			pkWindowBack = Find(WIID_MINI_TEAM_SP_BG_M + i, true);
			if( pkWindowBack )
				pkWindowBack->Hide();

			pkWindowBack = Find(WIID_MINI_TEAM_SP_BG_R + i, true);
			if( pkWindowBack )
				pkWindowBack->Hide();

#if !defined(_CHANNEL_TEAM_VERSION_)
			if( pStatusInfo->bDifferentChannel == true )
				pkWindowOutRange->SPSendMessage( SPIM_SET_IMAGEINDEX , 1 );
			else
				pkWindowOutRange->SPSendMessage( SPIM_SET_IMAGEINDEX , 0 );
#endif

			if( !pkWindowOutRange->IsShow() )
				pkWindowOutRange->Show();
		}
		else
		{
			float fRatio;

			pkWindowHP_C->Show();
			pkWindowHP_R->Show();
			if( pStatusInfo->iMaxHP <= 0.0f )
				fRatio = 1.0f;
			else
				fRatio = (float)pStatusInfo->iCurHP / (float)pStatusInfo->iMaxHP;
			
			iWidth = (int)((float)m_iGageWidth * fRatio);
			if( iWidth < 2 )
			{
				pkWindowHP_C->AdjustCoord(9, 20, 0, 4);
				if( iWidth < 1 )
					pkWindowHP_R->AdjustCoord(9, 20, 0, 4);
				else
					pkWindowHP_R->AdjustCoord(9, 20, 1, 4);
			}
			else
			{
				pkWindowHP_C->AdjustCoord(9, 20, iWidth - 1, 4);
				pkWindowHP_R->AdjustCoord(9 + (iWidth - 1), 20, 1, 4);
			}

			pkWindowSP_C->Show();
			pkWindowSP_R->Show();
			if( pStatusInfo->iMaxSP <= 0.0f )
				fRatio = 1.0f;
			else
				fRatio = (float)pStatusInfo->iCurSP / (float)pStatusInfo->iMaxSP;

			iWidth = (int)((float)m_iGageWidth * fRatio);
			if( iWidth < 2 )
			{
				pkWindowSP_C->AdjustCoord(9, 26, 0, 4);
				if( iWidth < 1 )
					pkWindowSP_R->AdjustCoord(9, 26, 0, 4);
				else
					pkWindowSP_R->AdjustCoord(9, 26, 1, 4);
			}
			else
			{
				pkWindowSP_C->AdjustCoord(9, 26, iWidth - 1, 4);
				pkWindowSP_R->AdjustCoord(9 + (iWidth - 1), 26, 1, 4);
			}

			pkWindowBack = Find(WIID_MINI_TEAM_HP_BG_L + i, true);
			if( pkWindowBack )
				pkWindowBack->Show();

			pkWindowBack = Find(WIID_MINI_TEAM_HP_BG_M + i, true);
			if( pkWindowBack )
				pkWindowBack->Show();

			pkWindowBack = Find(WIID_MINI_TEAM_HP_BG_R + i, true);
			if( pkWindowBack )
				pkWindowBack->Show();

			pkWindowBack = Find(WIID_MINI_TEAM_SP_BG_L + i, true);
			if( pkWindowBack )
				pkWindowBack->Show();

			pkWindowBack = Find(WIID_MINI_TEAM_SP_BG_M + i, true);
			if( pkWindowBack )
				pkWindowBack->Show();

			pkWindowBack = Find(WIID_MINI_TEAM_SP_BG_R + i, true);
			if( pkWindowBack )
				pkWindowBack->Show();
			
			if( pkWindowOutRange->IsShow() )
				pkWindowOutRange->Hide();
		}

		break;
	}

	return 1;
}


//------------------------------------------------------------------------------------
SPIMESSAGE_OBJ_MAP_BEGIN(SPWindowMiniTeam)

SPIMESSAGE_COMMAND(	SPIM_TEAM_JOIN,				OnJoin		)
SPIMESSAGE_COMMAND(	SPIM_TEAM_SECEDE,			OnSecede	)
SPIMESSAGE_COMMAND(	SPIM_TEAM_UPDATE,			OnUpdate	)
SPIMESSAGE_COMMAND(	SPIM_TEAM_UPDATE_STATUS,	OnUpdateStatus	)

SPIMESSAGE_OBJ_MAP_END(SPWindow)