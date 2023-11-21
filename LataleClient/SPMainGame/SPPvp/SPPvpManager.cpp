#include "SPCommon.h"
#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"

#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"
#include "SockWnd.h"
#include "SPNetworkManager.h"
#include "LT_Error.h"

#include "SPMainGameDEF.H"
#include "SPSubGameManager.h"
#include "SPMainGameManager.h"
#include "SPMouseCursor.h"
#include "SPWindowDEF.h"
#include "SPInterfaceManager.h"

#include "SPItemAttr.h"

#include "SPEventDEF.h"
#include "SPEventManager.h"
#include "SPEventNpcHandler.h"

#include "SPManagerDef.h"
#include "SPManager.h"
#include "SPGOBManager.h"
#include "SPSubGameManager.h"
#include "SPMainGameManager.h"
#include "SPGameObject.h"
#include "SPGOBStatus.h"
#include "SPPlayerStatus.h"
#include "SPGOBClusterDef.h"
#include "SPGOBCluster.h"
#include "SPGobModelUnit.h"
#include "SPAvatarModelUnitDef.h"
#include "SPAvatarModelUnit.h"
#include "SPPlayerInvenArchive.h"
#include "SPPlayer.h"

#include "SPPvpGloryLevelArchive.h"
#include "SPPvpManager.h"
#include "SPLocalizeManager.h"
#include "GlobalDefines_Share.h"

#include "_Error_log.h"

using namespace std;

/**
	Vector Sort에 사용될 Generic algorithm 비교함수
*/
bool PVPSortByRank( PVPRANK& lv, PVPRANK& rv ) {
	return lv.iRank < rv.iRank;
}

SPPvpManager* g_pkPvpManager = NULL;
SPPvpManager* SPPvpManager::m_pkInstance = NULL;
//------------------------------------------------------------------------------------
SPPvpManager* SPPvpManager::GetInstance()
{
	if( m_pkInstance == NULL )
	{
		m_pkInstance = new SPPvpManager;
		g_pkPvpManager = m_pkInstance;
	}

	return m_pkInstance;
}

//------------------------------------------------------------------------------------
void SPPvpManager::Release()
{
	if( m_pkInstance )
	{
		g_pkPvpManager = NULL;

		delete m_pkInstance;
		m_pkInstance = NULL;
	}
}

//------------------------------------------------------------------------------------
SPPvpManager::SPPvpManager()
: m_bForceExit(false)
, m_iPVPGameState(PVP_STATUS_EMPTY)
, m_uiPVPScale(0)
, m_pGloryLevelArchive(NULL)
{
}

//------------------------------------------------------------------------------------
SPPvpManager::~SPPvpManager()
{
	Clear();
}

//------------------------------------------------------------------------------------
void SPPvpManager::Clear()
{
	SAFE_DELETE(m_pGloryLevelArchive);
	m_mPvpData.clear() ;
	ClearPvpInfo() ;
}

//------------------------------------------------------------------------------------
bool SPPvpManager::Init()
{
	m_bForceExit = false ;
	m_iPVPGameState = PVP_STATUS_EMPTY ;
	m_uiPVPScale = 0 ;
	m_CurPvpIndun.Clear() ;

	LoadLDTFile() ;

	m_pGloryLevelArchive = new SPPvpGloryLevelArchive;

	return true ;
}

//------------------------------------------------------------------------------------
bool SPPvpManager::LoadLDTFile()
{
	string strLDTFile;
	strLDTFile = "DATA/LDT/PVP_INDUN.LDT";

	SPLDTFile* pkLDTFile = NULL;
	if( !g_pResourceManager->GetLDTFromFileName(strLDTFile.c_str(), &pkLDTFile) )
		return false;

	int iDataNum, iItemID, i;
	LDT_Field ldtField;
	iDataNum = pkLDTFile->GetItemCount();

	//---------------------------------------------------------------------
	// Load and Insert Manager data structure
	PVP_INFO stPvpInfo;
	for( i = 0; i < iDataNum; i++ )
	{
		iItemID = pkLDTFile->GetPrimaryKey(i);
		stPvpInfo.Clear();
		
		//pkLDTFile->GetFieldFromLable(iItemID, "_Ind_Map_Type",		ldtField);
		ldtField.Init() ;
		pkLDTFile->GetField(iItemID, 0, ldtField);
		stPvpInfo.iPvpMatchType = ldtField.uData.lValue;

		//pkLDTFile->GetFieldFromLable(iItemID, "_Ind_Mapin_Stage",	ldtField);
		ldtField.Init() ;
		pkLDTFile->GetField(iItemID, 1, ldtField);
		stPvpInfo.iMapInStage = ldtField.uData.lValue;

		//pkLDTFile->GetFieldFromLable(iItemID, "_Ind_Mapin_Group",	ldtField);
		ldtField.Init() ;
		pkLDTFile->GetField(iItemID, 2, ldtField);
		stPvpInfo.iMapInGroup = ldtField.uData.lValue;

		//pkLDTFile->GetFieldFromLable(iItemID, "_Ind_Mapout_Stage",	ldtField);
		ldtField.Init() ;
		pkLDTFile->GetField(iItemID, 19, ldtField);
		stPvpInfo.iMapOutStage = ldtField.uData.lValue;

		//pkLDTFile->GetFieldFromLable(iItemID, "_Ind_Mapout_Group",	ldtField);
		ldtField.Init() ;
		pkLDTFile->GetField(iItemID, 20, ldtField);
		stPvpInfo.iMapOutGroup = ldtField.uData.lValue;

		//pkLDTFile->GetFieldFromLable(iItemID, "_Ind_String",	ldtField);		// 인던명
		ldtField.Init() ;
		pkLDTFile->GetField(iItemID, 23, ldtField);
		stPvpInfo.strIndunName = ldtField.uData.pValue;

		//pkLDTFile->GetFieldFromLable(iItemID, "_Pvp_Result_Num",	ldtField);	// 결과 값(?)
		ldtField.Init() ;
		pkLDTFile->GetField(iItemID, 24, ldtField);
		stPvpInfo.iPvpWinValue = ldtField.uData.lValue;

		//pkLDTFile->GetFieldFromLable(iItemID, "_Pvp_Effect_Time",	ldtField);	// 시간 이펙트 변경
		ldtField.Init() ;
		pkLDTFile->GetField(iItemID, 25, ldtField);
		stPvpInfo.iPvpWarnValue = ldtField.uData.lValue;

		m_mPvpData.insert(make_pair(iItemID, stPvpInfo));
	}

	return true;
}

//------------------------------------------------------------------------------------
void SPPvpManager::Process(float fTime)
{
}

//------------------------------------------------------------------------------------
void SPPvpManager::Render()
{
}

/**
	LoadWait window에서 사용될 배경 이미지 FileName 저장 (Only FileName)
 */
void SPPvpManager::SetLoadBGFileName(const char* szFileName)
{
	if( szFileName == NULL )
		m_strLoadWaitBGFileName = "" ;
	else
		m_strLoadWaitBGFileName = szFileName ;
}


//------------------------------------------------------------------------------------
void SPPvpManager::ClearPvpInfo()
{
	m_CurPvpIndun.Clear() ;
	m_bForceExit	= false ;
	m_iPVPGameState = PVP_STATUS_EMPTY ;
	m_iPvpIndunID	= 0 ;
	m_uiPVPScale	= 0 ;
	m_strLoadWaitBGFileName.clear() ;
}

//------------------------------------------------------------------------------------
bool SPPvpManager::FindPvpInfo(int iIndunID, PVP_INFO& indunInfo)
{
	std::map< int, PVP_INFO >::iterator mIter ;

	mIter = m_mPvpData.find(iIndunID) ;
	if( mIter != m_mPvpData.end() )
	{
		indunInfo = (*mIter).second ;
		return true ;
	}
	return false ;
}



//---------------------------------------------------------------------
//
//	For Packet
//
//---------------------------------------------------------------------
//---------------------------------------------------------------------
// Send Packet
// PVP_LoacComplete => NAV_CS_ENTERSTAGE
// PVP_Exit => 
/**
	PVP 등록 전송
	// UINT8			byPVPType;							// (1:FDEATH, 2:FTIME, 3:PDEATH, 4:PTIME, 5:GDEATH, 6:GTIME)
	// UINT32			uiPVPScale;							// 몇인용인지. (1:2인용, 2:4인용, 4:6인용, 8:8인용)
 */
bool	SPPvpManager::SendPVPEnterWait(UINT32 uiPVPScale, int iPVPType/*=FREE_TIME_MATCH*/)
{
	if( !SPLocalizeManager::GetInstance()->IsEnableValue(LV_PVP) )	{
		ErrorFLogs->Error_Log("Localize PVP Disable!!!!");
		return false ;
	}
	if( uiPVPScale <= 0 ){
		ErrorFLogs->Error_Log("PVPEnterWait Scale is Zero[%d]", uiPVPScale);
		return false ;
	}

	m_uiPVPScale = uiPVPScale ;
	CPacket kSendPacket;
	kSendPacket.AddUINT32(PVP_CS_REGIST);
	kSendPacket.AddUINT8((UINT8)iPVPType);
	kSendPacket.AddUINT32(uiPVPScale);
	SENDPACKET(kSendPacket);
	return true ;
}

/**
	PVP 등록 취소 전송
	// 예정되어 있으나... 아마 없을듯.
	// UINT8			byPVPType;							// (1:FDEATH, 2:FTIME, 3:PDEATH, 4:PTIME, 5:GDEATH, 6:GTIME)
	// UINT32			uiPVPScale;							// 몇인용인지. (1:2인용, 2:4인용, 4:6인용, 8:8인용)
 */
bool	SPPvpManager::SendPVPEnterCancel()
{
	if( !SPLocalizeManager::GetInstance()->IsEnableValue(LV_PVP) )	{
		ErrorFLogs->Error_Log("Localize PVP Disable!!!!");
		return false ;
	}
	CPacket kSendPacket;
	kSendPacket.AddUINT32(PVP_CS_CANCEL);
	SENDPACKET(kSendPacket);
	return true ;
}


/**
	PVP 나가기
*/
bool	SPPvpManager::SendPVPExit()
{
	if( !SPLocalizeManager::GetInstance()->IsEnableValue(LV_PVP) )	{
		ErrorFLogs->Error_Log("Localize PVP Disable!!!!");
		return false ;
	}
	CPacket kSendPacket;
	kSendPacket.AddUINT32(PVP_CS_OUT);
	SENDPACKET(kSendPacket);
	return true ;
}




//---------------------------------------------------------------------
// Receive Packet
/**
	PVP 등록 결과
	// GU_ID			ObjectGUID;							// 게임오브젝트아이디
	// GLOBAL_STRING_ID	ErrorID;							// 에러아이디
 */
void	SPPvpManager::OnPVPScEnterRegist(void* pPacket)
{
	if( !SPLocalizeManager::GetInstance()->IsEnableValue(LV_PVP) )	{
		ErrorFLogs->Error_Log("Localize PVP Disable!!!!");
		return ;
	}

	CPacket* pkPacket = reinterpret_cast< CPacket* >(pPacket);
	if( pkPacket == NULL )
		return ;

	if( m_iPVPGameState == PVP_STATUS_ENTERWAIT )
	{
		ErrorFLogs->Error_Log("▶ ERROR::이미 PVP 대기상태 입니다.") ;
	}

	GU_ID ObjectGUID;
	GLOBAL_STRING_ID iError;
	TCHAR szErrorMsg[256] ;

	pkPacket->ExtractUINT64(&ObjectGUID);
	pkPacket->ExtractUINT32(&iError);
	if( iError != 0 )
	{
		if( g_pResourceManager->GetGlobalString(iError) )
			g_pInterfaceManager->SPPerformMessage(SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(iError));
		else	{
			sprintf(szErrorMsg, "PVP Registry Error[%d]", iError) ;
			g_pInterfaceManager->SPPerformMessage(SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szErrorMsg);
		}
		return ;
	}

	m_iPVPGameState = PVP_STATUS_ENTERWAIT ;
	ErrorFLogs->Error_Log("◈RECV ENTER REGIST STATUS[%d]", m_iPVPGameState) ;
	// SendMessage PVP Enter Wait Window
	g_pInterfaceManager->SPChildWndSendMessage(WIID_PVPENTERWAIT, SPIM_PVP_ENTERWAIT_REGIST, 0, 0);
}

/**
	PVP 등록 취소 결과
	// GU_ID			ObjectGUID;							// 게임오브젝트아이디
	// GLOBAL_STRING_ID	ErrorID;							// 에러아이디
*/
void	SPPvpManager::OnPVPScEnterCancel(void* pPacket)
{
	if( !SPLocalizeManager::GetInstance()->IsEnableValue(LV_PVP) )	{
		ErrorFLogs->Error_Log("Localize PVP Disable!!!!");
		return ;
	}

	CPacket* pkPacket = reinterpret_cast< CPacket* >(pPacket);
	if( pkPacket == NULL )
		return ;

	if( m_iPVPGameState =! PVP_STATUS_EMPTY )
	{
		ErrorFLogs->Error_Log("▶ ERROR::PVP_EMPTY 상태가 아닙니다.[%d]", m_iPVPGameState ) ;
	}

	GU_ID ObjectGUID;
	GLOBAL_STRING_ID iError;
	TCHAR szErrorMsg[256] ;

	pkPacket->ExtractUINT64(&ObjectGUID);
	pkPacket->ExtractUINT32(&iError);
	if( iError != 0 )
	{
		if( g_pResourceManager->GetGlobalString(iError) )
			g_pInterfaceManager->SPPerformMessage(SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(iError));
		else	{
			sprintf(szErrorMsg, "PVP Registry Cancel Error[%d]", iError) ;
			g_pInterfaceManager->SPPerformMessage(SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szErrorMsg);
		}
		return ;
	}

	m_iPVPGameState = PVP_STATUS_EMPTY ;
	ClearPvpInfo() ;
	// SendMessage PVP Enter Wait Window
	g_pInterfaceManager->SPChildWndSendMessage(WIID_PVPENTERWAIT, SPIM_PVP_ENTERWAIT_CANCEL, 0, 0);
}


/**
	PVP Match OK
	// GU_ID			ObjectGUID;							// 게임오브젝트아이디
	// GLOBAL_STRING_ID	ErrorID;							// 에러아이디
	// UINT32			uiIndunID;							// PVP방 인스턴트 아이디
	// UINT16			wServerID;							// 인던 서버아이디
 */
void	SPPvpManager::OnPVPScMatchOK(void* pPacket)
{
	if( !SPLocalizeManager::GetInstance()->IsEnableValue(LV_PVP) )	{
		ErrorFLogs->Error_Log("Localize PVP Disable!!!!");
		return ;
	}

	CPacket* pkPacket = reinterpret_cast< CPacket* >(pPacket);
	if( pkPacket == NULL )
		return ;

	GU_ID				ObjectGUID;
	GLOBAL_STRING_ID	ErrorID;
	UINT32				uiIndunID;
	UINT16				wServerID ;
	PVP_INFO			stPVPInfo ;

	pkPacket->ExtractUINT64(&ObjectGUID);
	pkPacket->ExtractUINT32(&ErrorID);
	pkPacket->ExtractUINT32(&uiIndunID);
	pkPacket->ExtractUINT16(&wServerID);

	if( ErrorID != 0 )
	{
		if( g_pResourceManager->GetGlobalString(ErrorID) )
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(ErrorID) );
		else
		{
			TCHAR szError[64] ;
			wsprintf(szError, "PVP_SC_IN Error[%d]", ErrorID) ;
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szError );
		}
		g_pInterfaceManager->SPChildWndSendMessage(WIID_PVPENTERWAIT, SPIM_PVP_ENTERWAIT_CANCEL, 0, 0);
		ClearPvpInfo() ;
		return ;
	}

	m_iPvpIndunID = uiIndunID ;

	// Send To Zone -> Channel Change 
	if(g_pSubGameManager)
	{
		m_iPVPGameState = PVP_STATUS_LOADWAIT;
		ErrorFLogs->Error_Log("◈RECV PVP MatchOK STATUS[%d]", m_iPVPGameState) ;
		g_pSubGameManager->SPPerformMessage(SPGM_PVP_INDUN_ENTER, uiIndunID, wServerID);
	}
}



/**
	PVP LoadWait 사용자 상태 정보
	// GU_ID			ObjectGUID;							// 게임오브젝트아이디
	// INT32			iMemberCnt;							// 대기자수
	// PVPLOADSTATUS	stPVPLoadStatus;					// 대기자의 PVP로드상태
	// ...
 */
void	SPPvpManager::OnPVPScLoadStatus(void* pPacket)
{
	if( !SPLocalizeManager::GetInstance()->IsEnableValue(LV_PVP) )	{
		ErrorFLogs->Error_Log("Localize PVP Disable!!!!");
		return ;
	}

	if( g_pSubGameManager->GetSubGameID() != SUBGAME_ZONE )
	{
		ErrorFLogs->Error_Log("▶ ERROR::GubGame이 SUBGAME_ZONE 아닙니다.[%d]", g_pSubGameManager->GetSubGameID() ) ;
	}

	m_iPVPGameState = PVP_STATUS_LOADWAIT ;
	ErrorFLogs->Error_Log("◈RECV PVP LoadStatus STATUS[%d]", m_iPVPGameState) ;
	// Update PVP Load Wait Window
	g_pInterfaceManager->SPChildWndSendMessage(WIID_PVP_LOADWAIT, SPIM_PVP_LOADWAIT_UPDATE, 0, (LPARAM)pPacket);
}


/**
	PVP Start!!!
	// GU_ID			ObjectGUID;							// 게임오브젝트아이디
	// UINT32			uiLDTID;							// PVP LDT 아이디
 */
void	SPPvpManager::OnPVPScStart(void* pPacket)
{
	if( !SPLocalizeManager::GetInstance()->IsEnableValue(LV_PVP) )	{
		ErrorFLogs->Error_Log("Localize PVP Disable!!!!");
		return ;
	}

	CPacket* pkPacket = reinterpret_cast< CPacket* >(pPacket);
	if( pkPacket == NULL )
		return ;

	GU_ID ObjectGUID;
	UINT32 uiLdtID;
	PVP_INFO stPVPInfo ;

	if( g_pEventManager->IsEventBlock() )
		g_pEventManager->SetEventBlock(false);

	pkPacket->ExtractUINT64(&ObjectGUID);
	pkPacket->ExtractUINT32(&uiLdtID);
	
	// 이미 게임을 시작하기 전에 PVP_SC_IN에서 셋팅 되어 있음
	if( FindPvpInfo(uiLdtID, stPVPInfo) )
	{
		m_CurPvpIndun = stPVPInfo ;
	}
	else
	{
		ErrorFLogs->Error_Log("Can't Find PVP_Indun.LDT ID[%d]", uiLdtID) ;
		return ;
	}

	m_iPVPGameState = PVP_STATUS_GAMEPLAY ;
	ErrorFLogs->Error_Log("◈RECV PVP Start STATUS[%d]", m_iPVPGameState) ;
	
	// WantTeam Force Enable
	g_pGOBManager->SetAllPlayerWantTeam(true) ;
	// Hide PVP LoadWait Quickly(-1)
	g_pSubGameManager->SPPerformMessage(SPGM_PVP_LOADWAIT_SHOW, -1) ;		
	// Show Game Windows & Sound & Effect
	if( stPVPInfo.iPvpMatchType == FREE_TIME_MATCH )
	{
		g_pInterfaceManager->SPChildWndSendMessage(WIID_PVP_TIMMER, SPIM_PVP_SET_TIMELIMIT, stPVPInfo.iPvpWinValue, stPVPInfo.iPvpWarnValue) ;
	}
	else
	{
		ErrorFLogs->Error_Log("PVP MatchType Not FREE [%d]", stPVPInfo.iPvpMatchType) ;
	}
	// Show Effect & Sound 
	// ...
}



/**
	PVP Rank Show
	// GU_ID			ObjectGUID;							// 게임오브젝트아이디
	// UINT32			iMemberCnt;							// 대기자수
	// PVPRANK			stPVPRank;							// 대기자의 PVP순위정보

*/
void	SPPvpManager::OnPVPScRank(void* pPacket)
{
	if( !SPLocalizeManager::GetInstance()->IsEnableValue(LV_PVP) )	{
		ErrorFLogs->Error_Log("Localize PVP Disable!!!!");
		return ;
	}

	if( g_pSubGameManager->GetSubGameID() != SUBGAME_ZONE )
	{
		ErrorFLogs->Error_Log("▶ ERROR::GubGame이 SUBGAME_ZONE 아닙니다.[%d]", g_pSubGameManager->GetSubGameID() ) ;
	}

	ErrorFLogs->Error_Log("◈RECV PVP Ranking STATUS[%d]", m_iPVPGameState) ;
	// Update PVP Load Wait Window
	g_pInterfaceManager->SPChildWndSendMessage(WIID_PVP_RANKING, SPIM_PVP_SET_RANKING, 0, (LPARAM)pPacket);
}



/**
	PVP Result Show
	// GU_ID			ObjectGUID;							// 게임오브젝트아이디
	// UINT32			iMemberCnt;							// 대기자수
	// PVPRANK			stPVPRank;							// 대기자의 PVP순위정보
*/
void	SPPvpManager::OnPVPScResult(void* pPacket)
{
	if( !SPLocalizeManager::GetInstance()->IsEnableValue(LV_PVP) )	{
		ErrorFLogs->Error_Log("Localize PVP Disable!!!!");
		return ;
	}

	if( g_pSubGameManager->GetSubGameID() != SUBGAME_ZONE )
	{
		ErrorFLogs->Error_Log("▶ ERROR::GubGame이 SUBGAME_ZONE 아닙니다.[%d]", g_pSubGameManager->GetSubGameID() ) ;
	}

	// 게임이 종료되면 동작을 막는다.
	//if( g_pEventManager->IsEventBlock() == false )
	//	g_pEventManager->SetEventBlock(true);

	m_iPVPGameState = PVP_STATUS_RESULT ;
	ErrorFLogs->Error_Log("◈RECV PVP Result STATUS[%d]", m_iPVPGameState) ;

	// WantTeam Force Disable
	g_pGOBManager->SetAllPlayerWantTeam(false) ;

	// Update PVP Load Wait Window
	g_pInterfaceManager->ShowWindow(WIID_PVP_TIMMER, SP_HIDE, SP_TOGGLE_OFF, true);			// Hide Timer
	g_pInterfaceManager->ShowWindow(WIID_PVP_RANKING, SP_HIDE, SP_TOGGLE_OFF, true);		// Hide Ranking

	g_pInterfaceManager->SPChildWndSendMessage(WIID_PVP_RESULT, SPIM_PVP_SET_RESULT, 0, (LPARAM)pPacket);
}


/**
	PVP Exit!!!
	// GU_ID			ObjectGUID;							// 게임오브젝트아이디
*/
void	SPPvpManager::OnPVPScExit(void* pPacket)
{
	if( !SPLocalizeManager::GetInstance()->IsEnableValue(LV_PVP) )	{
		ErrorFLogs->Error_Log("Localize PVP Disable!!!!");
		return ;
	}

	if( m_iPVPGameState != PVP_STATUS_RESULT )
	{
		ErrorFLogs->Error_Log("Recv PVP Exit But Status is NOT PVP_STATUS_RESULT [%d]", m_iPVPGameState ) ;
	}
	ErrorFLogs->Error_Log("Recv PVP Exit [%d]", m_iPVPGameState ) ;
	m_iPVPGameState = PVP_STATUS_EMPTY ;
	ClearPvpInfo() ;

	// Indun Out -> change channel
	g_pSubGameManager->SPPerformMessage(SPGM_INDUN_OUT, 0, 0);
}


//---------------------------------------------------------------------
// 임시로 넣음... 추후 이동
/**
	두 RECT를 더한 RECT 계산
*/
void SPPvpManager::RectPlus(RECT& rt1, RECT& rt2, RECT& result)
{
	result.left		= rt1.left + rt2.left ;
	result.top		= rt1.top + rt2.top ;
	result.right	= rt1.right + rt2.right ;
	result.bottom	= rt1.bottom + rt2.bottom ;
}

/**
	두 RECT를 뺀 RECT 계산
*/
void SPPvpManager::RectMinus(RECT& rt1, RECT& rt2, RECT& result)
{
	result.left		= rt1.left - rt2.left ;
	result.top		= rt1.top - rt2.top ;
	result.right	= rt1.right - rt2.right ;
	result.bottom	= rt1.bottom - rt2.bottom ;
}

/**
	두 RECT의 평균값
*/
void SPPvpManager::RectHalf(RECT& rt1, RECT& rt2, RECT& result)
{
	result.left		= (rt1.left + rt2.left) / 2 ;
	result.top		= (rt1.top + rt2.top) / 2 ;
	result.right	= (rt1.right + rt2.right) / 2 ;
	result.bottom	= (rt1.bottom + rt2.bottom) / 2 ;
}

//---------------------------------------------------------------------
// PVP Zone Check
bool SPPvpManager::IsPvpZone()
{
	if( m_iPVPGameState == PVP_STATUS_EMPTY || m_iPVPGameState == PVP_STATUS_ENTERWAIT )
		return false;

	return true;
}

//---------------------------------------------------------------------
SPPvpGloryLevelArchive* SPPvpManager::GetPvpGloryLevelArchive()
{
	return m_pGloryLevelArchive;
}


SPPvpGloryLevelAttr* SPPvpManager::GetPvpGloryLevelAttr(int iPvpLevel /*= 0*/)
{
	if(m_pGloryLevelArchive) {
		if(iPvpLevel) {
			return m_pGloryLevelArchive->GetPvpGloryLevel(iPvpLevel);
		}
		else {
			//LocalPlayer
			if(g_pGOBManager->GetLocalPlayer()) {
				int iLevel = g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValue(STATUS_PVP_LV);
				return m_pGloryLevelArchive->GetPvpGloryLevel(iLevel);
			}
		}
	}
	
	return NULL;
}


std::string	SPPvpManager::GetPvpGloryLevelString(int iPvpLevel /*= 0*/)
{
	if(m_pGloryLevelArchive) {
		if(iPvpLevel) {
			return m_pGloryLevelArchive->GetPvpGloryLevelString(iPvpLevel);
		}
		else {
			//LocalPlayer
			if(g_pGOBManager->GetLocalPlayer()) {
				int iLevel = g_pGOBManager->GetLocalPlayer()->GetStatus()->GetStatusValue(STATUS_PVP_LV);
				return m_pGloryLevelArchive->GetPvpGloryLevelString(iLevel);
			}
		}
	}	
	return "";
}



