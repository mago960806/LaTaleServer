#pragma once 

class SPEvent;

class SPEventArchive;

class SPEventNpcHandler;

class SPQuestManager;

class SPGiftManager;

class SPMailManager;	

class SPUserShopArchive;

class SPWayPointManager;

class SPCoreEventManager;

class SPClassChangeManager;

////
//enum CHECK_STATUS_RESULT {
//	CHECK_STATUS_RESULT_FAIL = 0,
//	CHECK_STATUS_RESULT_V1 = 1,
//	CHECK_STATUS_RESULT_V2 = 2,
//	CHECK_STATUS_RESULT_OK = 3,
//	CHECK_STATUS_RESULT_MAX = 4,
//};

enum CUT_IN_STATE;

//const int MAX_HIDE_TOP = 113;
//const int MAX_HIDE_BOTTOM = 514;

/**
* @class SPEventManager
* @brief 이벤트들의 행동에 대해 관리(동작)한다 
* @author Jinhee
* @version 00.00.01
* @date 2005.5.1
* @bug 없음
* @warning 
*/
class SPEventManager : public SPManager {
public:
	SPEventManager();
	virtual ~SPEventManager();

	virtual void Process	(float fTime);
	virtual	void Render		(float fTime);
	//virtual void RenderTextDebug();
	//virtual void RenderText();

	virtual bool Init();
	virtual void Clean();
	
	virtual int SPPerformMessage	(UINT msg, WPARAM wparam = 0, LPARAM lParam= 0);	
	
	
	/**
	 *
	 @param
	 @return
	*/
	void	SetCutIn(bool bCutIn = false);
	
	/**
	 *
	 @param
	 @return
	*/
	bool	IsCutIn();	

	/**
	 * 컷인 스테이터스 상태를 설정한다 <br>
	 @param CUT_IN_STATE iCutInState = CUT_IN_STATE_NULL	설정할 컷인 스테이터스 
	 @param , int iEventID = 0								현재 발생한 이벤트의 ID
	 @return NULL
	*/
	void	SetCutInState(CUT_IN_STATE iCutInState = CUT_IN_STATE_NULL, MAPEVENT_INFO* pEvent = NULL);
	
	/**
	 * 
	 @param 
	 @return
	*/
	CUT_IN_STATE GetCutInState();

	/**
	 *
	 @param
	 @return
	*/
	SPEventNpcHandler* GetEventHandler();
	
	/**
	 * QuestManager를 가져온다 <br> 
	 @param NONE
	 @return SPQuestManager*
	*/
	SPQuestManager* GetQuestManager();

	/**
	 * Event, Npc, QuestList, QuestAttr의 정보를 모두 지니고 있음 <br> 
	 @param 
	 @return
	*/
	SPEventArchive*	GetEventArchive();

	/**
	 * GiftManager를 가져온다 <br>
	 @param 
	 @return
	*/
	SPGiftManager* GetGiftManager();

	/**
	 * MailManager를 가져온다 <br>  
	 @param 
	 @return
	*/
	SPMailManager* GetMailManager();

	/**
	 * 
	 @param 
	 @return
	*/
	SPUserShopArchive* GetUserShopArchive();

	/**
	 * WayPoint Manager를 가져온다 <br>  
	 @param 
	 @return
	*/
	SPWayPointManager* GetWayPointManager();

	/**
	 * core Event Manager를 가져온다 <br>
	 @param 
	 @return  
	*/
	SPCoreEventManager* GetCoreEventManager();

	/**
	 * Class Change Manager를 가져온다 <br>
	 @param 
	 @return  
	*/
	SPClassChangeManager* GetClassChangeManager();

	/**
	 * 컷인을 시작한다 <br> 
	 @param NONE
	 @return NONE
	*/
	void OnCutInStart();

	/**
	 * 컷인을 종료한다 <br>
	 @param NONE
	 @return NONE
	*/
	void OnCutInEnd();

	/**
	 * 이벤트 패킷을 샌드 플래그가 설정된 경우 로딩후 호출해 초기화 해준다 <br> 
	 @param bool bSend = false
	 @return bool 
	*/
	bool SetEventPacketSendFlag(bool bSend = false)	{
		m_bSendPacket = bSend;
		return m_bSendPacket;
	}
	
	void SetEventBlock(bool bBlock) { m_bEventBlock = bBlock; }
	
	//true인 경우 이벤트들을 블럭한다 <br>
	bool IsEventBlock() { return m_bEventBlock; }

	/**
	 * 
	 @param 
	 @return
	*/
	EVENT_TYPE GetCutInEvnetType() {
		if(m_pEvent) {
			return (EVENT_TYPE)m_pEvent->eventType;
		}

		return EVENT_NULL;
	}

	/**
	 * 현재 동작중인 이벤트 정보를 가져온다 <br> 
	 @param 
	 @return
	*/
	MAPEVENT_INFO* GetCurEvent() {
		if(m_pEvent)
			return m_pEvent;

		return NULL;
	}

	void SetCoreRequireAttr(SPCoreRequireAttr* pAttr);
	SPCoreRequireAttr* GetCoreRequireAttr();
	CUT_IN_STATE GetNextCutInStateByCoreEvent();

protected:
	//SPIMESSAGE_MAP_DECLARE()

	/**
	 * 컷인모드로 들어감을 서버에 요청 <br>
	 @param int iNpcID = 0
	 @return NONE
	*/
	void SendCutInStart(UINT32 iNpcID = 0);
	
	/**
	 * 컷인모드에서 나옴을 서버에 요청 <br>
	 @param int iNpcID = 0
	 @return NONE
	*/
	void SendCutInEnd(UINT32 iNpcID = 0);


protected:
	SPEventArchive*			m_pEventArchive;		/**< Map Event Data Pool <br> */
	SPEventNpcHandler*		m_pNpcHandler;			/**< NPC Handler <br> */
	bool					m_bCutInRender;			/**< CutIn Render Flag <br> */

	SPQuestManager*			m_pQuestManager;		/**< Quest Manager <br> */
	
	MAPEVENT_INFO*			m_pEvent;				/**< 현재 동작중인 이벤트를 기억한다 <br> */

	bool					m_bSendPacket;			/**< 중복 전송을 맊기 위해서... <br> 패킷을 보낸뒤 true <br> */

	bool					m_bEventBlock;			/**< 사망시, 또는 로딩간 Event 블럭 <br> */

	SPGiftManager*			m_pGiftManager;			/**< 선물 메니져 <br> */
	SPMailManager*			m_pMailManager;			/**< Mail Box Manager <br> */
	SPUserShopArchive*		m_pUserShopArchive;		/**< User Shop Data Pool <br> */
	SPWayPointManager*		m_pWayPointManager;		/**< WayPoint Manager <br> */
	SPCoreEventManager*		m_pCoreEventManager;	/**< Core Event Manager<br> 강제 이벤트 <br> */
	SPClassChangeManager*	m_pClassChangeManager;	/**< Class Change와 Level Up 이벤트시 관리 <br> */

	SPCoreRequireAttr*		m_pCoreRequire;			/**< <br> */
};


extern SPEventManager*	g_pEventManager;


