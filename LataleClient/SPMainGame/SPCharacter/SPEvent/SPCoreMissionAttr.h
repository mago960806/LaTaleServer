#pragma once 

#include "SPQuestAttr.h"


enum CORE_MISSION_TYPE {
	CORE_MISSION_TYPE_NULL,							/**< 미션 없음 <br> */
	CORE_MISSION_TYPE_QUEST,						/**< 미션 퀘스트 <br> */
	CORE_MISSION_TYPE_ITEM,							/**< 미션 아이템 <br> */
	CORE_MISSION_TYPE_EFFECT,						/**< 미션 효과 <br> */
};


class SPCoreMissionAttr {
public:
	SPCoreMissionAttr();
	~SPCoreMissionAttr();	

	bool	CheckRequire();
	void	SetSavePos(int iIndex);

private:
	void	Clear();

public:
	int								m_iID;				/**< 해당 ID <br> */
	CORE_MISSION_TYPE				m_iType;			/**< 미션 타입 <br> */
	int 							m_iSavePos;			/**< 저장하는 경우 어떤 위치인지 <br> */
	//int							m_iQuest;
	//SPQuestItem					m_iItem[];
	//int							m_iEffect[];
	SPRequire						m_Require[MAX_CORE_EVENT_REQUIRE];	/**< 미션 실행 리콰이어 <br> */
};


