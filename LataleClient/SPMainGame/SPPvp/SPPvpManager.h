// Copyright (C) DURAGON
//********************************************************************
//	filename: 	SPPvpManager.h
//	created:	2007/11/19   Time:11:11
//	purpose:	
//*********************************************************************/

#pragma once 

#include "GlobalDefines_Share.h"

//---------------------------------------------------------------------
// PVP Match Type
enum PVP_MATCH_TYPE
{
	FREE_DEATH_MATCH = 1,		// 일정 Kill 까지 개인전
	FREE_TIME_MATCH ,			// 일정 시간 까지	개인전
	PARTY_DEATH_MATCH ,
	PARTY_TIME_MATCH ,
	GUILD_DEATH_MATCH ,
	GUILD_TIME_MATCH ,
} ;

//---------------------------------------------------------------------
// PVP 게임상태
enum PVP_GAME_STATE
{
	PVP_STATUS_EMPTY = 0,
	PVP_STATUS_ENTERWAIT ,			// 입장 대기
	PVP_STATUS_LOADWAIT ,			// 로딩 대기
	PVP_STATUS_GAMEPLAY ,			// 게임중
	PVP_STATUS_RESULT ,				// 결과 화면
} ;

//---------------------------------------------------------------------
// PVP Indun info
struct PVP_INFO
{
	int iPvpMatchType ;
	int	iMapInStage ;
	int iMapInGroup ;
	int	iMapOutStage ;
	int iMapOutGroup ;
	int iPvpWinValue ;
	int iPvpWarnValue ;
	std::string strIndunName ;

	void Clear()
	{
		iPvpMatchType	= 0 ;
		iMapInStage		= 0 ;
		iMapInGroup		= 0 ;
		iMapOutStage	= 0 ;
		iMapOutGroup	= 0 ;
		iPvpWinValue	= 0 ;
		iPvpWarnValue	= 0 ;
		strIndunName.clear() ;
	}
	PVP_INFO& operator = (const PVP_INFO& stPvpInfo)
	{
		iPvpMatchType	= stPvpInfo.iPvpMatchType;
		iMapInStage		= stPvpInfo.iMapInStage	;
		iMapInGroup		= stPvpInfo.iMapInGroup	;
		iMapOutStage	= stPvpInfo.iMapOutStage;
		iMapOutGroup	= stPvpInfo.iMapOutGroup;
		iPvpWinValue	= stPvpInfo.iPvpWinValue;
		iPvpWarnValue	= stPvpInfo.iPvpWarnValue;
		strIndunName	= stPvpInfo.strIndunName.c_str() ;
		return *this;
	}
	bool IsEnable()
	{
		return iPvpMatchType ? true : false ;
	}
};

bool PVPSortByRank( PVPRANK& lv, PVPRANK& rv ) ;


struct SPPvpGloryLevelAttr;
class SPPvpGloryLevelArchive;
struct SPItemRequire;
class SPPvpManager
{
public:

	static SPPvpManager* GetInstance();
	static void Release();

	bool	Init();
	void	Clear();
	void	Process(float fTime);
	void	Render();

	//bool	EnterPvpIndun(UINT32 iInstanceID, int iIndunID=0) ;
	void	SetLoadBGFileName(const char* szFileName) ;
	const char*	GetLoadBGFileName() 	{	return m_strLoadWaitBGFileName.c_str();		}
	//---------------------------------------------------------------------
	// PVP_Indun.LDT Info
	bool	FindPvpInfo(int iIndunID, PVP_INFO& indunInfo);
	void	ClearPvpInfo() ;

	//---------------------------------------------------------------------
	// Get Value
	UINT32	GetPVPScale()			{	return m_uiPVPScale;		}		// PVP 입장 등록 체크
	int		GetPvpStatus()			{	return m_iPVPGameState;		}		// PVP Game 상태

	//---------------------------------------------------------------------
	// Send Packet
	bool	SendPVPEnterWait(UINT32 uiPVPScale, int iPVPType=FREE_TIME_MATCH) ;			// PVP 입장 등록
	bool	SendPVPEnterCancel() ;			// PVP 입장 등록 취소
	bool	SendPVPExit() ;					// PVP 퇴장

	//---------------------------------------------------------------------
	// Receive Packet
	void	OnPVPScEnterRegist(void* pPacket) ;				// PVP 입장 등록 결과
	void	OnPVPScEnterCancel(void* pPacket) ;				// PVP 입장 등록 취소 결과
	void	OnPVPScMatchOK(void* pPacket) ;					// PVP Match OK
	void	OnPVPScLoadStatus(void* pPacket) ;				// PVP 로딩 대기 결과
	void	OnPVPScStart(void* pPacket) ;					// PVP 시작 -> 화면전환 및 시작
	void	OnPVPScRank(void* pPacket) ;					// PVP 중간 Ranking
	void	OnPVPScResult(void* pPacket) ;					// PVP 결과
	void	OnPVPScExit(void* pPacket) ;					// PVP Exit 결과

	//---------------------------------------------------------------------
	// Rect Util Function
	void	RectPlus(RECT& rt1, RECT& rt2, RECT& result);
	void	RectMinus(RECT& rt1, RECT& rt2, RECT& result);
	void	RectHalf(RECT& rt1, RECT& rt2, RECT& result);

	//---------------------------------------------------------------------
	// PVP Zone Check
	bool	IsPvpZone();

	//---------------------------------------------------------------------
	//
	SPPvpGloryLevelArchive* GetPvpGloryLevelArchive();
	
	/**
	 * iPvpLevel에 해당하는 SPPvpGloryLevelAttr 를 가져온다 <br>
	 * iPvpLevel이 0인경우 로컬플레이어 레벨에 맞는 SPPvpGloryLevelAttr를 가져온다 <br>
	 @param int iPvpLevel = 0			0인경우 로컬플레이어의 Pvp레벨을 사용한다
	 @return  SPPvpGloryLevelAttr*		정보가 없거나 잘못된 경우 NULL
	*/
	SPPvpGloryLevelAttr*	GetPvpGloryLevelAttr(int iPvpLevel = 0);

	/**
	* iPvpLevel에 해당하는 SPPvpGloryLevelAttr 를 가져온다 <br>
	* iPvpLevel이 0인경우 로컬플레이어 레벨에 맞는 SPPvpGloryLevelAttr를 가져온다 <br>
	@param int iPvpLevel = 0			0인경우 로컬플레이어의 Pvp레벨을 사용한다
	@return  SPPvpGloryLevelAttr*		정보가 없거나 잘못된 경우 NULL
	*/
	std::string				GetPvpGloryLevelString(int iPvpLevel = 0);


protected:
	SPPvpManager();
	virtual ~SPPvpManager();

	bool LoadLDTFile();
	

protected:
	static SPPvpManager*	m_pkInstance;

	UINT32		m_uiPVPScale ;				// PVP 등록정보
	int			m_iPVPGameState ;			// PVP 게임상태 (0:Empty, 1:로딩대기, 2:게임중, 3:결과화면)
	UINT32		m_iPvpIndunID ;				// PVP Indun Instance ID
	std::string m_strLoadWaitBGFileName ;

	std::map< int, PVP_INFO > m_mPvpData;
	PVP_INFO	m_CurPvpIndun;
	bool		m_bForceExit;

	SPPvpGloryLevelArchive*	m_pGloryLevelArchive;	/**< PVP 명예 포인트를 명예 레벨로 환산하는 테이블 <br> */
};

extern SPPvpManager* g_pkPvpManager;
