#pragma once

class SPGameObject;
class SPGOBModelUnit;
class SPEventStatusModel;
class SPQuestAttr;

#include "SPEventDEF.h"


const unsigned int EVENT_ID_NULL = 0;			/**< 이벤트 ID NULL 상태 <br> */


enum EVENT_QUEST_SHOW {
	EVENT_QUEST_SHOW_NULL = 0,					/**< 보여줄 Quest 관련 FX가 없음 <br> */
	EVENT_QUEST_SHOW_COMPLETE = 1,				/**< 퀘 완료가 가능한 경우 <br> */
	EVENT_QUEST_SHOW_ABLE = 2,					/**< 퀘를 방을수 있는 경우 <br> */
	EVENT_QUEST_SHOW_MAX						/**< <br> */
};


/**
* @class SPEvent
* @brief 이벤트 단위
* @author Jinhee
* @version 00.00.01
* @date 2005.5.1
* @bug 없음
* @warning 
*/
class SPEvent : public SPGameObject 
{

public:
	SPEvent();
	SPEvent(INSTANCE_ID instanceID, CLASS_ID ClassID);
	virtual ~SPEvent();

	virtual void Process(float fTime);
	virtual	void Render(float fTime);
	virtual void Init();	
	virtual void Destroy();
	virtual void RenderFX( float fTime , SP_FXM_RENDER_LAYER eRenderLayer );

	virtual int  PerformMouseMove		( int iX, int iY);
	virtual int  PerformMouseLDown		( int iX, int iY);
	virtual int  PerformMouseLUp		( int iX, int iY);
	virtual int  PerformMouseRDown		( int iX, int iY);
	virtual int  PerformMouseRUp		( int iX, int iY);
	virtual bool IsCursorIn				( int iX, int iY);
	virtual bool IsCursorIn				( RECT & rc );

	void ProcessImage(float fTime);

	FACING	GetDirection();
	
	BG_ID	GetStandLayer();				
	
	void	SetPos(float fX, float fY);

	/**
	 * Event X 축 위치를 얻어온다 <br> 
	 @param NONE
	 @return float Event Pos X
	*/
	float	GetPosX();
	
	/**
	* Event Y 축 위치를 얻어온다 <br> 
	@param NONE
	@return float Event Pos Y
	*/
	float	GetPosY();

	/**
	* Event TYPE 을 얻어온다 <br> 
	@param NONE
	@return EVENT_TYPE
	@warning EVENT_TYPE 형에 문제가 있을시 int 로 변환해서 받을수도 있다
	*/
	EVENT_TYPE GetEventType();

	/**
	 * Event Name 을 얻오온다 <br> 
	 @param NONE
	 @return string eventName
	*/
	std::string GetEventName();
	
	/**
	 * MAPEVENT_INFO struct 를 세팅해준다 <br>
	 @param MAPEVENT_INFO* pEventInfo = NULL	팻킷으로 넘어온 MAPEVENT_INFO
	 @return	bool
	*/
	//bool	SetEventInfo(MAPEVENT_INFO* pEventInfo = NULL);
	bool	SetEventInfo(EVENT_INFO* pEventInfo = NULL);
	bool	UpdateModelNpc(UINT32 ObjectImageID) ;		// NPC 이미지만 변경
	
	/**
	 * member MAPEVENT_INFO 의 내부 데이터를 초기화 한다 <br>
	 * 기타 정보들도 초기화 한다 <br> 
	 @param NONE
	 @return NONE
	*/
	void	SetNull();

	/**
	 * Window를 보여줘야 하는지 <br> 
	 @param NONE
	 @return bool	(true: show, false: hide)
	*/
	bool	IsMessageShow()		{ return m_bShowWindow; }

	/**
	 * Event Message 를 서버에 보냈는지 <br>
	 @param NONE
	 @return bool	(true: send, false: not)
	*/
	bool	IsSendPacket()		{ return m_bSendPacket; }

	/**
	* Player와 비교하여 현재 이벤트가 보여주어야 할 <br>
	* Quest Fx를 표현한다 <br>
	@param 
	@return
	*/
	EVENT_QUEST_SHOW CheckEventQuestShow();

	/**
	 * 현재 Event에 등록되어 있는 Quest관련 FX를 제거한다 <br>
	 @param NONE
	 @return NONE
	*/
	void	ClearEventFX();

protected:
	SPGMESSAGE_OBJ_MAP_DECLARE()
	
	////
	virtual int OnGetPosX	( LPARAM lParam );
	virtual int OnGetPosY	( LPARAM lParam );
	virtual int OnGetFacing(LPARAM lParam);
	virtual int OnGetGOBName( LPARAM lParam );
	virtual int OnGetStandlayer(LPARAM lParam);

	virtual int OnGetFXArchive	( LPARAM lParam );		// AJJIYA [12/16/2006 AJJIYA]	

	/**
	 * 패킷으로 온 MAPEVENT_INFO를 SetEventInfo를 SetEventInfo()로 넘김 <br>
	 @param LPARAM lParam
	 @return int
	*/
	virtual int OnSetEventInfo(LPARAM lParam);

protected:		
	/**
	 * Event 영역에 라인을 그려준다 <br> 
	 * _DEBUG 출력 <br>
	 @param NONE
	 @return NONE
	*/	
	void RenderEventLine();

	/**
	 * Player Postion 정보를 이용하여 이벤트 영역에 위치했는지 체크한다 <br>
	 @param NONE
	 @return unsgined int EventID
	*/
	unsigned int CheckPlayerEvent();
	
	/**
	 *
	 @param 
	 @return  
	*/
	bool LoadEventCore(int iItemID);
	

	/**
	 * Client상에서 Event Data로딩... <br>
	 @param int iItemID
	 @return bool
	*/
	bool LoadEventLDT(int iItemID);

	/**
	 * NpcLdt에서 필요한 현재 Npc가 지니고 있는 questlist를 만들어 내도록 한다 <br> 
	 @param 
	 @return
	*/
	int LoadNpcLDT(int iItemID);

	/**
	 * NpcQuest List를 얻어와 퀘스트 리스트를 추가하고 <br>
	 * EVENT_QUEST_SHOW플래그 설정 작업을 한다 <br>
	 @param 
	 @return
	*/
	int LoadNpcQuestList(int iItemID);

	/**
	 * NpcQuest List에 퀘스트를 추가한다 <br>
	 @param 
	 @return
	*/
	bool AddQuestList(int iQuestID);	

	/**
	* Event Message 를 서버에 보낸다 <br>
	@param NONE
	@return bool
	*/
	bool	SendEventPacket();

	/**
	 * Local Player의 Status 조건을 체크한다 <br>
	 * 이 함수는 현재 비어 있고 외부에서 체크 할수 있도록 해야 할듯하다 <br>
	 @param NONE
	 @return bool
	*/
	bool CheckRequire();

	/**
	 * 강제 이벤트 Require조건을 체크한다 <br>
	 @param 
	 @return  
	*/
	bool CheckCoreRequire();

	/**
	 * 강제 이벤트 Require체크시 인벤토리의 빈 공간을 체크해준다 <br>
	 @param 
	 @return  
	*/
	bool IsEnableInven(SPCoreRequireAttr* pCoreReq);
	
	/**
	 * 강제 이벤트 Require체크시 플레이어의 수락 가능한 퀘스트 빈공간을 체크해준다 <br>
	 @param 
	 @return  
	*/
	bool IsEnableQuestSize(SPCoreRequireAttr* pCoreReq);

	/**
	 * ...<br> 
	 @param 
	 @return
	*/
	void ChangeAnimation();

	/**
	 * 모든 QuestList를 삭제한다 <br>
	 @param 
	 @return
	*/
	void DeleteAllQuest();

	/**
		Dynimic Event 의 경우 DynimicFx를 설정해야 할 때가 있다.
	*/
	void SetDynimicFX() ;

private:
	// Action
	UINT64 m_uiCurAction;
	UINT64 m_uiLastAction;

	float m_fPlayerX;						/**< Local Player Pos X <br> */
	float m_fPlayerY;						/**< Local Player Pos Y <br> */

	float m_fCamX;							/**< Camera Pos X <br> */
	float m_fCamY;							/**< Camera Pos Y <br> */

	float m_fMouseX;						/**< Mouse Pos X <br> */
	float m_fMouseY;						/**< Mouse Pos Y <br> */
	int	m_iLClick;							/**< Mouse Left Button Click <br> */
	
	SPFXArchive*			m_pkFXArchive;	// AJJIYA [11/28/2005]

	MAPEVENT_INFO	m_MapEvent;				/**< Map Event Struct <br> */
	RECT			m_rtEvent;				/**< Event Area Rect <br> */
	bool			m_bShowWindow;			/**< Window Show Flag <br> */
	bool			m_bSendPacket;			/**< Event Send Packet <br> */
	bool			m_bRunCoreEvent;		/**< Core Event Running Flag <br> */

	FACING			m_eFacing;
	float			m_fAnimationTime;
	
	SPGOBModelUnit*	m_pkModelUnit;			/**< Render Model <br> */
	SPEventStatusModel*	m_pkStatusModel;
	
	int		m_iQuestListID;
	std::vector<SPQuestAttr*> m_vpQuest;	/**< Event가 Npc이고 Quest를 지닌 경우 지니고 있는 QuestList <br> */
	EVENT_QUEST_SHOW m_iEventQuestShow;		/**< Evnet가 Npc이고 Quest를 지닌 경우 FX 표시용 <br> */
	
	std::vector<SPCoreRequireAttr*> m_vpCoreRequire;
};

