// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPGOBManager
// Comment     : 
// Creation    : metalgeni 2004-08-23 오후 2:56:36
//***************************************************************************


#pragma once 

class SPGameObject;
class SPPlayer;
class SPMonster;
class SPLootingBag;
class SPGOBCluster;
class SPMOBCluster;
class SPBattleCluster;
class SPEvent;					//[2005/4/25] - jinhee
class SPGuildArchive;
//class SPMouseCursor;			//[2005/5/6] - jinhee
class SPChatBoard;				//  [4/10/2008 AJJIYA]
enum SP_FXM_RENDER_LAYER;

typedef int ( *PNameOutPut )( const char* playName );

class SPGOBManager : public SPManager {
public:

	SPGOBManager();
	virtual ~SPGOBManager();

	virtual void	Process(float fTime);
	void			ProcessJoyPad(float fTime);			//[2007/5/28]-jinhee
	virtual	void	Render(float fTime);
	void			RenderLayer2(float fTime);
	void			RenderLayer3(float fTime);			//jinhee - [2005/1/24]
	void			RenderFX( float fTime , SP_FXM_RENDER_LAYER eRenderLayer );

	virtual bool	Init();
	virtual void	Clean();	
	virtual bool	PurgeAll();			// Purge Instant Data for Subgame Change
	virtual int		SPPerformMessage	(UINT msg, WPARAM wparam = 0, LPARAM lParam = 0);

	SPGameObject*	Find( INSTANCE_ID InstanceID, CLASS_ID ClassID = CLASS_ID_NULL);
	SPGameObject*	Find( GU_ID guid );
	SPPlayer*		FindPlayerByName( const char* pCharName  ) ;

	SPPlayer*		AddPlayer(float fX, float fY);	
	SPPlayer*		AddPlayer(CPacket* pPacket, bool bLocalPlayer = false);
	SPMonster*		AddMonster(CPacket* pPacket, CLASS_ID ClassID);
	SPMonster*		AddMonster(float fX, float fY, CLASS_ID ClassID);
	SPLootingBag*	AddLootingBag(CPacket* pPacket, CLASS_ID ClassID, INSTANCE_ID InstanceID);
	SPEvent*		AddEvent(CPacket* pPacket, CLASS_ID ClassID, INSTANCE_ID InstanceID);	//[2005/4/25] - jinhee
	bool			AddGameObject(SPGameObject* pkGameObject, int iObjectType);
	bool			AddChatBoard( CPacket* pPacket , CLASS_ID eClassID, INSTANCE_ID eInstanceID );	//  [4/10/2008 AJJIYA]
	bool			AddMsgBoard( CPacket* pPacket , CLASS_ID eClassID, INSTANCE_ID eInstanceID );	// {xialin 2008/09/10]add
	bool			AddShopPet( CPacket* pPacket , CLASS_ID eClassID, INSTANCE_ID eInstanceID );

	bool			DelObject( INSTANCE_ID InstanceID, CLASS_ID ClassID);

	bool			ResetObject();		//Local Player이외의 모든 오브젝트를 삭제

	SPPlayer*		GetLocalPlayer()	
	{ 	
		return m_pLocalPlayer;
	}

	SPGuildArchive* GetGuildArchive() ;	

	SPGOBCluster*	GetGOBCluster()		{ return m_pGOBCluster; }
	bool			SetLocalPlayer(SPPlayer* pPlayer);

	SPGameObject* GetLootingBag(float fPosX, float fPosY, SPGameObject* pkExceptObject = NULL, bool bUsedDistance = true);

	int GetLootingBagSize();
	SPGameObject* GetLootingBag(int iIndex);

	int GetAllMonster(std::vector< SPGameObject* >& vMOBList);
	int GetAllPlayer(std::vector< SPGameObject* >& vPlayerList);
	
	/**
	 * 모든 플레이어 일괄로 파티구함 표현 적용 <br>
	 @param bool bShowByPvp = false				true인 경우 모두 표시 적용 false 인경우 Option에 의해 적용
	 @return  
	*/
	void SetAllPlayerWantTeam(bool bShowByPvp = false);

	/**
	* EventList의 Object내의 Quest관련 FX를 지워준다 <br>
	@param NONE
	@return NONE
	*/
	void ClearEventQuestFx();

	/**
	 * EventList의 Object내의 Quest관련 Fx를 다시 설정하게 한다 <br>
	 @param NONE
	 @return NONE
	*/
	void CheckEventQuestFx();
	
	/**
	 * ExecutePotalEvent <br>
	 * OnEventMessage() 시 이벤트가 포탈인경우 실행한다 <br>
	 @param int iStage
	 @param int iGroup
	 @param float fPosX = 0.0f
	 @param float fPosY = 0.0f
	 @return bool 
	*/
	bool ExecutePotalEvent(int iStage, int iGroup, float fPosX = 0.0f, float fPosY = 0.0f);	

	/**
	* 상점 개설시 주위에 중복되는 상점이 있는지 확인한다 <br>
	* 주위에 대화방이 있는지도 확인한다 <br> 
	@param int iShopType = 0 개설하려는 개인상점 종류
	@return bool (true인 경우 중복 없음)
	*/
	bool CheckUserShopOverlap(int iShopType = 0);


	//[xialin 2008/09/11]add
	bool CheckMsgBoardOverlap(int iMsgType, int iPosX, int iPosY);

	/**
	 * 주위에 영역이 중복되는 상점이 존재하는지 확인한다 <br> 
	 @param RECT rtLocal 체크 할 Local Rect (로컬 플레이어의 개인상점 크기별로 차이남)
	 @return  bool (true인 경우 중복 없음)
	*/
	bool IsUserShopRect(RECT rtLocal);


	//[xialin 2008/09/11]add
	bool IsTerrainBack( RECT rcLocal );

	//[xialin 2008/09/11]add
	bool IsMsgBoardRect( RECT rcBound , bool bScreenPos );

	//[xialin 2008/09/11]add
	bool IsEventRect( RECT rcBound , bool bScreenPos );

	/**
		GuildCrop의 LevelUp시 Npc의 Image를 변경해주는 함수
	*/
	bool UpdateEventNPC(INSTANCE_ID InstanceID, CLASS_ID ClassID, UINT32 ObjectImageID) ;

	bool IsChatBoardRect( RECT rcBound , bool bScreenPos );	// 필드의 채팅보드 영역 검사 [5/23/2008 AJJIYA]
	bool IsPetShopRect(RECT rtLocal);

	virtual int	WndProc(UINT message, WPARAM wParam, LPARAM lParam);
	int PerformMouseMove		( int iX, int iY);
	int PerformMouseLDown		( int iX, int iY);
	int PerformMouseLUp			( int iX, int iY);
	int PerformMouseRDown		( int iX, int iY);
	int PerformMouseRUp			( int iX, int iY);
	int PerformMouseLDBLClick	( int iX, int iY);
	int PerformMouseRDBLClick	( int iX, int iY);	

protected:
	bool SendPlayerBlockExist(std::string strCharName = "");
	void GetPlayerInfo();

	SPGMESSAGE_MAP_DECLARE()
	int OnInit				(WPARAM wparam, LPARAM lparam);
	int OnPurgeAll			(WPARAM wparam, LPARAM lparam);
	int OnAddGameobj		(WPARAM wparam, LPARAM lparam);
	int OnDelGameobj		(WPARAM wparam, LPARAM lparam);
	int OnSetLocalPlayer	(WPARAM wparam, LPARAM lparam);
	int	OnGOBAddObjID		(WPARAM wparam, LPARAM lparam);

	int OnAddLocalPlayer	(WPARAM wparam, LPARAM lparam);	

	int OnAddEnterPlayer	(WPARAM wparam, LPARAM lparam);
	int OnAddEnterPlayerEx	(WPARAM wparam, LPARAM lparam);
	int OnAddLeavePlayer	(WPARAM wparam, LPARAM lparam);
	int OnGOBMove			(WPARAM wparam, LPARAM lparam);	

	int	OnPlayerEquipDefaultChange	(WPARAM wparam, LPARAM lparam);
	int	OnPlayerEquipChange			(WPARAM wparam, LPARAM lparam);
	
	int OnEventMessage		(WPARAM wparam, LPARAM lparam);			//Server 쪽에서 Event 응답 메시지가 온 경우 처리
	int OnSendEnterStageMessage		(WPARAM wparam, LPARAM lparam); //Server응답후 클라이언트 로딩 완료 메시지 보냄

	int OnPlayerBlockExist (WPARAM wParam, LPARAM lParam);	//[2005/9/2] 내가 다른 유저에의해 블록이 되었는지의 여부 판단...
	int OnRefreshNameColor(WPARAM wParam, LPARAM lParam);
	
private:

	std::vector<SPGameObject*> m_vpGameObjPlayer	;	
	std::vector<SPGameObject*> m_vpGameObjMonster	;	
	std::vector<SPGameObject*> m_vpGameObjEvent		;	//[2005/4/25] - jinhee
	std::vector<SPGameObject*> m_vpGameObjLootingBag;
	std::vector<SPGameObject*> m_vpGameObjChatBoard;	//  [4/10/2008 AJJIYA]
	std::vector<SPGameObject*> m_vpGameObjMessageBoard;	//  [xialin 2008/09/10]add
	std::vector<SPGameObject*> m_vpGameObjShopPet;

	std::vector<UINT32>        m_vPlayerID; //쏵흙끝쒼鯤소 ID

	INSTANCE_ID			m_LastInstance;	
	SPGOBCluster*		m_pGOBCluster;
	SPMOBCluster*		m_pMOBCluster;
	SPGuildArchive*		m_pGuildArchive ;

	SPBattleCluster*	m_pBattleCluster;	

	SPPlayer*			m_pLocalPlayer;

	GU_ID				m_iSendPlayerInfo;				//[2005/9/2] - 플레이어 정보보기 패킷을 요청했는지?

	int					m_iCursorX;
	int					m_iCursorY;
	bool				m_bJoyPadLPush;			/**< <br> */
	bool				m_bJoyPadRPush;			/**< <br> */
private:

	int					GmOutPutPlayName( const char* pName );
};


extern SPGOBManager* g_pGOBManager;






