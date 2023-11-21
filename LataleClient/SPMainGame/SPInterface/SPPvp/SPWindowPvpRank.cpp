#include "SPCommon.h"
#include "SPUtil.h"

#include "SPMainGameDEF.H"
#include "SPGameObjectDEF.h"
#include "GlobalDefines_Share.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"
#include "SockWnd.h"
#include "SPNetworkManager.h"
#include "LT_Error.h"

#include "SPSubGameManager.h"
#include "SPGameObject.h"
#include "SPManager.h"
#include "SPMouseCursor.h"
#include "SPIMEPool.h"
#include "SPWindowDEF.h"
#include "SPInterfaceManager.h"

#include "SPGameObject.h"
#include "SPGOBStatus.h"
#include "SPGOBClusterDef.h"
#include "SPGOBCluster.h"
#include "SPGobModelUnit.h"
#include "SPAvatarModelUnitDef.h"
#include "SPAvatarModelUnit.h"
#include "SPPlayer.h"
#include "SPGOBManager.h"

#include "SPTimer.h"

#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowPvpRank.h"
#include "SPPvpManager.h"

#include "_Error_log.h"

SPWindowPvpRank::SPWindowPvpRank(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent /*= NULL*/)
: SPWindow(WNDID_PVP_RANKING, InstanceID, iX, iY, iCX, iCY, pParent)
, m_pTextureBase(NULL)
, m_pTextureBar(NULL)
, m_pTimer(NULL)
, m_iRankShowMode(0)
{
	Init();
}


SPWindowPvpRank::~SPWindowPvpRank()
{
	Clean();
}


void SPWindowPvpRank::Init()
{
	m_iUserCount	= 0 ;
	m_iMyRank		= 0 ;

	m_pTimer = new SPTimer ;
	m_pTimer->Clear() ;

	g_pVideo->GetTextureMgr()->LoadTexture("DATA/INTERFACE/CONCEPT/UI_WinBG_000.PNG" , &m_pTextureBase);	
	g_pVideo->GetTextureMgr()->LoadTexture("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG" , &m_pTextureBar);	

	// Background
	SetRect(&m_rtSrcBase[0],	497, 497, 501, 501);
	SetRect(&m_rtSrcBase[1],	502, 497, 506, 501);	
	SetRect(&m_rtSrcBase[2],	507, 497, 511, 501);
	SetRect(&m_rtSrcBase[3],	497, 502, 501, 506);
	SetRect(&m_rtSrcBase[4],	502, 502, 506, 506);
	SetRect(&m_rtSrcBase[5],	507, 502, 511, 506);
	SetRect(&m_rtSrcBase[6],	497, 507, 501, 511);
	SetRect(&m_rtSrcBase[7],	502, 507, 506, 511);
	SetRect(&m_rtSrcBase[8],	507, 507, 511, 511);	
	
	// My Bar
	SetRect(&m_rtSrcBar,	505,  1, 510, 22);	

	InitSubControl();
}


void SPWindowPvpRank::InitSubControl()
{
	SPWindowStatic* pStatic = NULL;

	for( int i = 0 ; i < PVP_MAX_ROOMUSER; i++)
	{
		int k = i%4 ;
		int j = i ? i/4 : 0 ;
		pStatic = new SPWindowStatic( WIID_PVP_RANKING_RANK + i  , 16,  8+i*24,  20,  17, this);
		pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG", 427+k*21, 29+j*19) ;
		pStatic->Hide();

		pStatic = new SPWindowStatic( WIID_PVP_RANKING_PLAYER + i , 44,  11+i*24, 120, 12, this);
		i ? pStatic->SetFont(FONT_12_NORMAL) : pStatic->SetFont(FONT_12_BOLD) ;
		pStatic->SetFontColor(RGBA(255, 255, 255, 255)) ;
		pStatic->SetFormat(DT_TOP | DT_LEFT);
		pStatic->Hide();
	}
}


void SPWindowPvpRank::ReposOwnImage()
{
	SetRect(&m_rtDestBase[0],	m_iAX ,			m_iAY ,					m_iAX +   4,	m_iAY + 4);
	SetRect(&m_rtDestBase[1],	m_iAX + 4,		m_iAY ,					m_iAX + 160,	m_iAY + 4);	
	SetRect(&m_rtDestBase[2],	m_iAX + 160,	m_iAY ,					m_iAX + 164,	m_iAY + 4);
	SetRect(&m_rtDestBase[3],	m_iAX ,			m_iAY + 4,				m_iAX +   4,	m_iAY + 5+m_iUserCount*24);
	SetRect(&m_rtDestBase[4],	m_iAX + 4,		m_iAY + 4,				m_iAX + 160,	m_iAY + 5+m_iUserCount*24);
	SetRect(&m_rtDestBase[5],	m_iAX + 160,	m_iAY + 4,				m_iAX + 164,	m_iAY + 5+m_iUserCount*24);
	SetRect(&m_rtDestBase[6],	m_iAX ,			m_rtDestBase[3].bottom,	m_iAX +   4,	m_iAY + 9+m_iUserCount*24);
	SetRect(&m_rtDestBase[7],	m_iAX + 4,		m_rtDestBase[4].bottom,	m_iAX + 160,	m_iAY + 9+m_iUserCount*24);
	SetRect(&m_rtDestBase[8],	m_iAX + 160,	m_rtDestBase[5].bottom,	m_iAX + 164,	m_iAY + 9+m_iUserCount*24);

	// My Rank
	if( m_iMyRank > 0)
		SetRect(&m_rtDstBar,	m_iAX, m_iAY - 18 + m_iMyRank*24, m_iAX + 164, m_iAY + 3 + m_iMyRank*24);
}


void SPWindowPvpRank::Clean()
{
	m_iMyRank = 0 ;
	m_iRankShowMode = RANK_MODE_NULL ;
	m_fMoveAccumulate = 0 ;
	SAFE_RELEASE(m_pTextureBase);
	SAFE_RELEASE(m_pTextureBar);
	SAFE_DELETE( m_pTimer ) ;
	SPWindow::Clean();
}


void SPWindowPvpRank::Show(bool bWithChild /*= true*/)
{
	// Set Timer until 5 Seconds
	// m_pTimer->Start(1, PVP_RANK_SHOW_TIME) ;

	SPWindow::Show(bWithChild);
}


void SPWindowPvpRank::Hide(bool bSendServer /*= true*/)
{
	SPWindow::Hide(bSendServer);
}


void SPWindowPvpRank::Process(float fTime)
{
	if(!m_bShow)			return;


	if( m_pTimer->CheckTimer(fTime) )
	{
		if( m_pTimer->IsExpired() )
		{
			m_iRankShowMode = RANK_MODE_OUT ;
			m_fMoveAccumulate = 0 ;
			m_pTimer->Clear() ;
		}
	}

	ProcessMove(fTime) ;
	
	SPWindow::Process(fTime);
}


/**
	Window Move-IN or Move-OUT
	Original Position = 636, 129 ;
	Move distance = 800 - 636 = 164 ;
 */
void SPWindowPvpRank::ProcessMove(float fTime)
{
	int iMovePosX ;
	if( m_iRankShowMode == RANK_MODE_IN )
	{
		m_fMoveAccumulate += fTime ;
		if( m_fMoveAccumulate > PVP_RANK_MOVE_TIME )
		{
			m_iRankShowMode = RANK_MODE_SHOW ;
			m_fMoveAccumulate = 0 ;
			this->AdjustCoord(636, 129) ;
			ReposOwnImage() ;
			m_pTimer->Start(1, PVP_RANK_SHOW_TIME) ;
			return ;
		}
		iMovePosX = (int)(800 - (164 * m_fMoveAccumulate)/PVP_RANK_MOVE_TIME ) ;
		this->AdjustCoord(iMovePosX, 129) ;
		ReposOwnImage() ;
	}
	else if( m_iRankShowMode == RANK_MODE_OUT )
	{
		m_fMoveAccumulate += fTime ;
		if( m_fMoveAccumulate > PVP_RANK_MOVE_TIME )
		{
			m_iRankShowMode = RANK_MODE_NULL ;
			m_fMoveAccumulate = 0 ;
			Hide() ;
		}
		iMovePosX = (int)(636 + (164 * m_fMoveAccumulate)/PVP_RANK_MOVE_TIME ) ;
		this->AdjustCoord(iMovePosX, 129) ;
		ReposOwnImage() ;
	}
}


void SPWindowPvpRank::Render(float fTime)
{
	if(!m_bShow)		return;

	if(m_pTextureBase) {
		g_pVideo->Flush();
		for(int i = 0; i < PVP_RANKING_BG; i++) {
			m_pTextureBase->RenderTexture(&m_rtDestBase[i], &m_rtSrcBase[i]);
		}
	}
	
	SPWindow::Render(fTime);

	if( m_pTextureBar && m_iMyRank )
		m_pTextureBar->RenderTexture(&m_rtDstBar, &m_rtSrcBar);		// My Bar
	
}



SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowPvpRank )

SPIMESSAGE_COMMAND(SPIM_PVP_SET_RANKING		, OnSetRankData	)

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)


/**
	@param int iUserCount : 사용자수
	@param const char* pUserNameList : 사용자 ID 목록 (ID : ID :.....)
	// GU_ID			ObjectGUID;							// 게임오브젝트아이디
	// UINT32			iMemberCnt;							// 대기자수
	// PVPRANK			stPVPRank;							// 대기자의 PVP순위정보
*/
int SPWindowPvpRank::OnSetRankData( WPARAM wParam, LPARAM lParam )
{
	CPacket* pkPacket = reinterpret_cast< CPacket* >(lParam);
	if( pkPacket == NULL )
		return 0;

	int i ;
	GU_ID ObjectGUID;
	UINT32 iMemberCount = 0;
	PVPRANK stPVPRank ;
	SPWindowStatic* pStaticName = NULL ;
	SPWindowStatic* pStaticReady = NULL ;
	std::vector<PVPRANK> vecRank ;
	vecRank.reserve(8) ;

	TCHAR szPlayerName[MAX_LEN_NAME] ;
	memset(szPlayerName, 0, MAX_LEN_NAME) ;
	g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_GETGOBNAME, (LPARAM)szPlayerName);

	// Rank Clear
	m_iMyRank = 0 ;

	pkPacket->ExtractUINT64(&ObjectGUID);
	pkPacket->ExtractUINT32(&iMemberCount);
	//ErrorFLogs->Error_Log("RECV PVP RANK ===========================>>") ;
	//ErrorFLogs->Error_Log("OnSetRankData Members[%d]", iMemberCount) ;
	m_iUserCount = iMemberCount ;
	if( m_iUserCount <= 0 )
	{
		ErrorFLogs->Error_Log("OnSetRankData Members[%d] then Hide", iMemberCount) ;
		return 0 ;
	}

	for( i = 0; i < iMemberCount; i ++)
	{
		pkPacket->ExtractStruct(&stPVPRank, sizeof(PVPRANK));
		vecRank.push_back(stPVPRank) ;
	}
	sort( vecRank.begin(), vecRank.end(), PVPSortByRank) ;
	for( i = 0; i < iMemberCount; i ++)
	{
		if( strcmp(vecRank[i].szCharName, szPlayerName) == 0 )
		{
			m_iMyRank = i + 1;
			//ErrorFLogs->Error_Log("My PVP Rank Members[%s][%d]", vecRank[i].szCharName, m_iMyRank) ;
		}
	}
	
	// Setting Item
	SPWindowStatic* pWindowIcon = NULL ;
	SPWindowStatic* pWindowID = NULL ;
	for( int i = 0; i < PVP_MAX_ROOMUSER; i ++)
	{
		pWindowID = (SPWindowStatic*)Find(WIID_PVP_RANKING_PLAYER+i, true) ;
		pWindowIcon = (SPWindowStatic*)Find(WIID_PVP_RANKING_RANK+i, true) ;
		if( i < m_iUserCount )
		{
			if( pWindowID )	{
				pWindowID->SetShowEnable(true) ;
				pWindowID->SetWindowText(vecRank[i].szCharName) ;
			}
			if( pWindowIcon )	{
				pWindowIcon->SetShowEnable(true) ;
			}
		}
		else
		{
			if( pWindowID )	{
				pWindowID->SetShowEnable(false) ;
				pWindowID->Hide() ;
			}
			if( pWindowIcon )	{
				pWindowIcon->SetShowEnable(false) ;
				pWindowIcon->Hide() ;
			}
		}
	}
	m_iRankShowMode = RANK_MODE_IN ;
	m_fMoveAccumulate = 0 ;
	ReposOwnImage() ;
	Show() ;
	return 1; 
}