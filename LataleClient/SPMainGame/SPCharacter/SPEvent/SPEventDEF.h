
#pragma once


//enum SPID_EVENT {
//	SPID_EVENT_NULL			= 0,	
//};

/**
 * Event Model Class Type 
*/
enum SPID_EVENT {
	ID_EVENT_CL_NULL			= 0,		/**< Render Image 1장 없는경우 출력 없음 <br> */
	ID_EVENT_CL_ANI				= 1000,		/**< Animation의 경우 <br> */
};


/**
 * 현재 컷인의 세부 상황 <br>
*/
enum CUT_IN_STATE {	
	CUT_IN_STATE_NULL				= 0,	/**< CutIn 이 아님 <br> */
	
	CUT_IN_STATE_SCROLL				= 1,	/**< CutIn Mode Scroll 중 <br> */
	CUT_IN_STATE_SCROLL_END			= 2,	/**< CutIn Mode Scroll 끝 <br> */
	
	CUT_IN_STATE_TALK				= 3,	/**< CutIn Mode Talk <br> */
	CUT_IN_STATE_TRADE				= 4,	/**< CutIn Mode Trade <br> */
	CUT_IN_STATE_QUEST				= 5,	/**< CutIn Mode Quest <br> */
	CUT_IN_STATE_STORAGE			= 6,	/**< CutIn Mode Storage <br> */
	CUT_IN_STATE_GIFT				= 7,	/**< CutIn Mode Gift <br> */
	CUT_IN_STATE_SAVEPOINT			= 8,	/**< CutIn Mode SavePoint <br> */
	CUT_IN_STATE_MAIL				= 9,	/**< CutIn State Mail <br> */
	CUT_IN_STATE_BEAUTY				= 10,	/**< CutIn State Beauty <br> */
	CUT_IN_STATE_GUILD_CREATE		= 11,	/**< CutIn State Guild Create<br> */
	CUT_IN_STATE_GUILD_DISSOLVE		= 12,	/**< CutIn State Guild Dissolve <br> */
	CUT_IN_STATE_GUILD_CROPS		= 13,	/**< CutIn State Guild Crops <br> */
	CUT_IN_STATE_NPC_PORTAL			= 14,	/**< CutIn State Npc Portal <br> */
	CUT_IN_STATE_NPC_CHANGE			= 15,	/**< CutIn State Npc Item Change <br> */
	CUT_IN_STATE_GUILD_LEVEL_UP		= 16,	/**< CutIn State Guild Level Up <br> */
	CUT_IN_STATE_GUILD_EMBLEM		= 17,	/**< CutIn State Guild Emblem Change <br> */
	CUT_IN_STATE_PVP_RESERVE		= 18,	/**< CutIn State Pvp Reserve <br> */
	CUT_IN_STATE_NPC_AUCTION		= 19,	/**< CutIn State Auction <br> */
	CUT_IN_STATE_NPC_REOPTION		= 20,	/**< CutIn State Item Re Option <br> */  
	CUT_IN_STATE_PARTY_PVP_RESERVE	= 21,	/**< CutIn State Party Pvp Reserve <br> */  
	CUT_IN_STATE_RANKING			= 22,	/**< CutIn State Ranking board in game <br> */  	
	CUT_IN_STATE_LOVE				= 23,	/**< CutIn State Love <br> */ 
	CUT_IN_STATE_RELEASE,
};


/**
* 대화가 일반인지 이벤트(선택문)인지 <br> 
*/
enum TALK_TYPE {
	TALK_TYPE_NULL,
	TALK_TYPE_NORMAL,							/**< 일반 대화 <br> */
	TALK_TYPE_SELECT,							/**< 선택 대화 <br> */
	TALK_TYPE_MAX,
};


/**
* 대화 윈도우가 NPC용인지 PC용인지 <br>
*/
enum NPC_TALK_WINDOW {
	NPC_TALK_WINDOW_NULL,
	NPC_TALK_WINDOW_NPC,						/**< 대화창 NPC <br> */
	NPC_TALK_WINDOW_PC,							/**< 대화창 PC <br> */
	NPC_TALK_WINDOW_MAX,
};


/**
* NPC 대화 형식 <br>
*/
enum NPC_TALK_WINDOW_MODE {
	NPC_TALK_WINDOW_MODE_NULL,
	NPC_TALK_WINDOW_MODE_NORMAL,				/**< Npc 일반 대화 <br> */
	NPC_TALK_WINDOW_MODE_OUTBURST,				/**< Npc 격노(?) 대화 <br> */
	NPC_TALK_WINDOW_MODE_MAX,
};


/**
 * NPC 대화중 효과 <br> 
*/
enum NPC_TALK_EFFECT {
	NPC_TALK_EFFECT_NULL,						/**< 효과 없음 <br> */
	NPC_TALK_EFFECT_SCROLL,						/**< 효과 스크롤 <br> */
	NPC_TALK_EFFECT_MAX,
};


/**
 * 현재 Npc Talk 윈도우가 어떤 모드로 열려 있는지 <br> 
 * 사용하지는 않고 있음 <br> 
*/
enum NPC_TALK_USE {
	NPC_TALK_USE_NULL,							/**< <br> */
	NPC_TALK_USE_START,							/**< <br> */
	NPC_TALK_USE_TALK,							/**< <br> */
	NPC_TALK_USE_SAVEPOINT,						/**< <br> */
	NPC_TALK_USE_MAX,
};


////
///**
//* 현재 NPC 의 EVENT 세부 사항 <br> 
//*/
//enum NPC_EVENT_TYPE {
//	NPC_EVENT_TYPE_NULL				= 0,/**< 아무것도 아님 <br> */
//	NPC_EVENT_TYPE_TALK				= 1,/**< 대화 <br> */
//	NPC_EVENT_TYPE_TRADE			= 2,/**< 상점 <br> */
//	NPC_EVENT_TYPE_STORAGE			= 3,/**< 창고 <br> */
//	NPC_EVENT_TYPE_QUEST			= 4,/**< Quest <br> */	
//};


const int MAX_NPC_EVENT_TYPE		= 4;/**< NPC가 지닐수 있는 최대 EVENT_TYPE의 갯수 <br> */
const int MAX_NPC_MESSAGE			= 4;/**< NPC가 지닐수 있는 최대 Message의 갯수 <br> */


typedef struct _EVENT_TYPE {
	NPC_EVENT_TYPE m_iEventType;	
	int m_iTypeID;
	std::string m_strMemu;

	_EVENT_TYPE() {
		m_iEventType = NPC_EVENT_TYPE_NULL;
		m_iTypeID = 0;
		m_strMemu = "";
		m_strMemu.clear();
	}

	void SetEventType(NPC_EVENT_TYPE iType, int iTypeID, std::string strMenu) {		
		m_iEventType = iType;
		m_iTypeID = iTypeID;
		m_strMemu = strMenu;
	}
} EventType ;

/**
 * Event Type
*/
//enum EVENT_TYPE {
//	EVENT_TYPE_NULL		= 0,			/**< 안내판 용도 <br> */
//	EVENT_TYPE_NPC		= 1,			/**< NPC <br> */
//	EVENT_TYPE_SAVE		= 2,			/**< Save Point <br> */
//	EVENT_TYPE_PORTAL	= 3,			/**< Potal <br> */
//};


/**
 * Condition Type
*/
enum CONDITION_TYPE {
	CONDITION_TYPE_NULL			= 0,
	CONDITION_TYPE_TYPE			= 1,
	CONDITION_TYPE_GENDER		= 2,
};


/**
 * Event Condition Check Stuct <br> 
 * 실제 영역 체크후 후속 체크를 하는 경우<br>
 * 해당조건의 정보를 담고 있는 구조체임 <br>
*/
struct SPEventCondition {
	CONDITION_TYPE iCondition;			/**< 조건 타입 <br> */
	int iCheckValue1;					/**< 제한 값 1 <br> */
	int iCheckValue2;					/**< 제한 값 2 <br> 기획에 없지만 임의로 추가함 <br>*/

	SPEventCondition() {
		iCondition = CONDITION_TYPE_NULL;
		iCheckValue1 = -1;
		iCheckValue2 = -1;
	}

	//void SetCondition(CONDITION_TYPE iCon, int iVal) {
	//	iCondition = iCon;
	//	iCheckVelue1 = iVal;
	//}

	void SetCondition(CONDITION_TYPE iCon, int iVal1, int iVal2 = 0) {
		iCondition = iCon;
		iCheckValue1 = iVal1;
		iCheckValue2 = iVal2;
	}
};


//enum ITEM_TYPE;
//#include "SPItemAttr.h"
//#include "SPItemStatus.h"

enum EVENT_INVEN_TYPE {
	EVENT_INVEN_TYPE_NULL = 0,			/**< 인벤 NULL <br> */
	EVENT_INVEN_TYPE_EQUIP = 1,			/**< 장비 인벤 <br> */
	EVENT_INVEN_TYPE_USE = 2,				/**< 사용 인벤 <br> */
	EVENT_INVEN_TYPE_ETC = 3,				/**< 기타 인벤 <br> */
	EVENT_INVEN_TYPE_EVENT = 4,			/**< 이벤트 인벤 <br> */
	EVENT_INVEN_TYPE_PET = 5,				/**< 펫 인벤 <br> */
	EVENT_INVEN_TYPE_HOUSING = 6,			/**< 하우징 인벤 <br> */
};

/**
 * Item Condition Check Struct <br>
 * 아이템 조건을 체크를 위한 정보를 가지는 구조체임 <br> 
*/
struct SPEventItem {
	//ITEM_TYPE iConditionItem;			/**< 조건 아이템 <br> */
	EVENT_INVEN_TYPE iConditionInven;
	int iCheckValue1;					/**< 제한 값 1 <br> */

	SPEventItem() {
		//iConditionItem = ITEM_TYPE_NULL;
		iConditionInven = EVENT_INVEN_TYPE_NULL;
		iCheckValue1 = -1;
	}

	void SetEventItem( EVENT_INVEN_TYPE iItem, int iVal) {
		iConditionInven = iItem;
		iCheckValue1 = iVal;
	}
};



struct SPEventFrameData {
	FLOAT		fFrameTime;			// 각 프레임의 Delay.
	FLOAT		fFrameDegree;		// 각 프레임의 회전값.
	FLOAT		fFrameScale;		// 각 프레임의 Scale.
	RECT		RectSrc;			// 각 프레임의 텍스쳐소스.
	RECT		RectAdjust;			// 각 프레임의 렌더타겟의 위치보정값 top, left만 씀.
	D3DXCOLOR	FrameColor;			// 각 프레임의 컬러.
	bool		bLRSwap;			// 좌우측 뒤집기

	SPEventFrameData() 
		: fFrameTime(0.0f)
		, fFrameDegree(0.0f)
		, fFrameScale(0.0f)
		, FrameColor(1.0f, 1.0f, 1.0f, 1.0f)
		, bLRSwap(false)
	{
		ZeroMemory(&RectSrc,		sizeof(RECT));
		ZeroMemory(&RectAdjust,		sizeof(RECT));
	}

	void SetNull()
	{
		fFrameTime		=	(0.0f);
		fFrameDegree	=	(0.0f);
		fFrameScale		=	(0.0f);
		FrameColor		=	D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		bLRSwap			=	false;
		ZeroMemory(&RectSrc,		sizeof(RECT));
		ZeroMemory(&RectAdjust,		sizeof(RECT));
	}

	void operator=(const SPEventFrameData Src)
	{
		fFrameTime		= Src.fFrameTime;		
		fFrameDegree	= Src.fFrameDegree;	
		fFrameScale		= Src.fFrameScale;	
		RectSrc			= Src.RectSrc;		
		RectAdjust		= Src.RectAdjust;		
		FrameColor		= Src.FrameColor;
		bLRSwap			= Src.bLRSwap;
	}
};


/**
* Npc가 상점에 판매할 아이템 구조
*/
typedef struct _SPSellItem {
	int m_iItemID;									/**< Item ID <br> */
	int m_iRear;									/**< Rear <br> */
	int m_iStack;									/**< 판매되는 갯수 <br> */
	int m_iSellFactor;								/**< NPC가 유저에게 팔때 가격에 영향을 주는 인자 <br> */

	_SPSellItem(){
		Clear();
	}

	~_SPSellItem(){
		Clear();
	}

	void Clear(){
		m_iItemID		= 0;
		m_iRear			= 0;
		m_iStack		= 0;
		m_iSellFactor	= 0;
	}

	void SetSellItem(int iItemID, int iRear, int iStack, int iSellFactor) {
		Clear();
		m_iItemID		= iItemID;
		m_iRear			= iRear;
		m_iStack		= iStack;
		m_iSellFactor	= iSellFactor;
	}
} SPSellItem;


#include "SPQuestAttr.h"

const int MAX_NPC_PORTAL_REQUIRE = 2;							/**< Npc를 이용한 포탈 정보에 사용되는 조건체크 갯수 <br> */

/**
 * Npc 를 이용한 포탈에 사용하는 포탈 개개의 정보 <br>
*/
typedef struct _SPNPCPortalInfo {
	int					m_iItemID;								/**< Item Id <br> */
	std::string			m_strName;								/**< Name <br> */
	int					m_iDestID;								/**< 목적지 ID <br> */
	int					m_iStage;								/**< 목적지의 Stage <br> */
	int					m_iMapGroup;							/**< 목적지의 mapGroup <br> */
	int					m_iMapIndex;							/**< 목적지의 MapIndex <br> */
	SPRequire			m_Require[MAX_NPC_PORTAL_REQUIRE];		/**< 체그 조건 <br> */
	int					m_iRequireItem;							/**< 체크 Item <br> */
	UINT64				m_iRequireEly;							/**< 체크 Ely <br> */
	int					m_iFxID;								/**< FX ID <br> */
	float				m_fDelay;								/**< Fx 딜레이 <br> */
	std::string			m_strErrMsg;							/**< Error <br> */

	_SPNPCPortalInfo() {
		Clear();
	}

	~_SPNPCPortalInfo() {
		Clear();
	}

	void Clear() {
		m_iItemID				= 0;
		m_strName.clear();
		m_iDestID				= 0;
		m_iStage				= 0;	
		m_iMapGroup				= 0;
		m_iMapIndex				= 0;
		m_iRequireItem			= 0;
		m_iRequireEly			= 0;
		m_iFxID					= 0;
		m_fDelay				= 0.0f;
		m_strErrMsg.clear();

		for(int i = 0; i < MAX_NPC_PORTAL_REQUIRE; i++) {
			m_Require[i].Clear();
		}
	}

} SPNpcPortalInfo;


const int MAX_NPC_EVENT_INFO_REQUIRE = 2;
const int MAX_NPC_EVENT_REQUIRE_ITEM = 4;

typedef struct _SPNpcEventInfo {
	int					m_iID;							/**< Key ID <br> */
	
	SPQuestItem			m_RewordItem;					/**< 보상으로 들어가는 아이템의 정보 <br> */	
	std::string			m_strTitleName;					/**< 보상의 이름 <br> */	
	int					m_iIcon;						/**< 아이템 ID 가 없는 경우 사용할 아이콘 <br> */
	int					m_iIconIndex;					/**< 아이템 ID 가 없는 경우 사용할 아이콘<br> */
	UINT64				m_iEly;							/**< 보상 엘리 <br> */	
	
	std::string			m_strDesc;						/**< 설명 <br> */
	
	SPRequire			m_Require[MAX_NPC_EVENT_INFO_REQUIRE];			/**< 제한 조건 <br> */
	SPQuestItem			m_RequireItem[MAX_NPC_EVENT_REQUIRE_ITEM];		/**< 필요 아이템 <br> */

	UINT64				m_iRequireEly;									/**< 필요 엘리 <br> */
	int					m_iRequirePvpPoint;								/**< 필요 PVP 포인트 <br> */

	std::string			m_strErr;						/**< 제한조건이나 필요 아이템 또는 엘리 부족시 메시지 <br> */

	_SPNpcEventInfo() {
		Clear();
	}

	~_SPNpcEventInfo() {
		Clear();
	}

	void Clear() {
		m_iID					= 0;
		
		m_RewordItem.Clear();
		m_strTitleName.clear();
		m_iIcon					= 0;
		m_iIconIndex			= 0;
		m_iEly					= 0;

		m_strDesc.clear();
		
		int i = 0;
		for(i = 0; i < MAX_NPC_EVENT_INFO_REQUIRE; i++) {
			m_Require[i].Clear();
		}

		for(i = 0; i < MAX_NPC_EVENT_REQUIRE_ITEM; i++) {
			m_RequireItem[i].Clear();
		}
		
		m_iRequireEly			= 0;
		m_iRequirePvpPoint		= 0;
		m_strErr.clear();
	}

} SPNpcEventInfo;

/**  
 * NPC 의 작은 대화 상자에 사용될 이미지와 메시지 <br> 
 * NPC_GUILD 와 유사한 정보 <br>
 * 현재는 CUT_IN_STATE_NPC_REOPTION 인 상황에서만 사용 <br>
*/  
struct SPCutInTalk {
	int					m_iID;						/**< ID <br> */  
	int					m_iImageID;					/**< CutIn Image <br> */  
	std::string			m_strMsg;					/**< CutIn Shop Message <br> */ 

	SPCutInTalk() {
		Clear();
	}

	~SPCutInTalk() {
		Clear();
	}

	void Clear() {
		m_iID				= 0;
		m_iImageID			= 0;
		m_strMsg.clear();
	}	
};


const int MAX_CORE_REQUIRE				= 10;		/**< 한 영역에서 체크할 최대 Event Require의 갯수 <br> */
const int MAX_CORE_EVENT_REQINVEN		= 2;		/**< 최대 인벤 체크용 갯수 <br> */
const int MAX_CORE_EVENT_REQUIRE		= 8;		/**< 최대 Requrie 체크용 갯수 <br> */


/**
 * 강제 이벤트 실행 리콰 체크 ID 리스트 <br> 
 * ID를 참조하여 SPCoreRequireAttr 정보를 읽어내야 함 <br> 
*/
struct SPCoreEventInfo {
	int				m_iID;							/**< 해당 Info의 키 <br> */
	int				m_iRequireID[MAX_CORE_REQUIRE];	/**< RequireAttr 을 가르키는 ID <br> */

	SPCoreEventInfo() {
		Clear();
	}

	~SPCoreEventInfo() {
		Clear();
	}

	void Clear() {
		m_iID				= 0;
		for(int i = 0; i < MAX_CORE_REQUIRE; i++) {
			m_iRequireID[i]		= 0;
		}		
	}
};


/**
 * 인벤 체크용 정보 <br>
*/
struct SPReqInvenSize {
	CONTAINER_TYPE	m_iContainerType;				/**< 체크할 인벤토리 타입 <br> */
	int				m_iSize;						/**< 체크할 비어있어야할 공간 <br> */

	SPReqInvenSize() {
		Clear();
	}

	~SPReqInvenSize() {
		Clear();
	}

	void Clear() {
		m_iContainerType	= CONTAINER_TYPE_INVALID;
		m_iSize				= 0;
	}

	void SetReqInven(CONTAINER_TYPE iType, int iSize) {
		m_iContainerType	= iType;
		m_iSize				= iSize;
	}
};


typedef struct _SPCoreRequireAttr {
	int				m_iID;										/**< 해당 ID <br> */						
	SPReqInvenSize	m_InvenReq[MAX_CORE_EVENT_REQINVEN];		/**< 인벤 체크용 정보 <br> */
	int				m_iReqQuestSize;							/**< 퀘스트 체크용 정보 <br> */
	REQUIRE_INFO	m_Require[MAX_CORE_EVENT_REQUIRE];			/**< 일반 Require체크용 <br> */

	NPC_EVENT_TYPE	m_iEventType;								/**< 연결되는 이벤트 타입 <br> */
    int				m_iEventID;									/**< 연결되는 이벤트 ID <br> */
	
	int				m_iRank;

	_SPCoreRequireAttr() {
		Clear();
	}

	~_SPCoreRequireAttr() {
		Clear();
	}

	void Clear() {
		m_iID				= 0;
		int i = 0;
		for(i = 0; i < MAX_CORE_EVENT_REQINVEN; i++) {
			m_InvenReq[i].Clear();
		}
		
		m_iReqQuestSize		= 0;
		
		for(i = 0; i < MAX_CORE_EVENT_REQUIRE; i++) {
			m_Require[i].Clear();
		}		

		m_iEventType		= NPC_EVENT_TYPE_NULL;
		m_iEventID			= 0;
		m_iRank				= 0;
	}


} SPCoreRequireAttr;

