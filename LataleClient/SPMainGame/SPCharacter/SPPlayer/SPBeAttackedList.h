#pragma once 

//#define _USE_OLD_HP_BAR				255;

struct SPGOBModel;
class SPGameObject;
class SPGOBModelUnit;


enum TARGET_OBJECT_TYPE {
	TARGET_OBJECT_TYPE_MONSTER			= 0,				/**< 몬스터 <br> */
	TARGET_OBJECT_TYPE_PLAYER			= 1,				/**< 플레이어 <br> */
	TARGET_OBJECT_TYPE_BOSS				= 2,
	TARGET_OBJECT_TYPE_MAX				= 3,				/**< ??? <br> */
};


enum TARGET_OBJECT_SHOW_TYPE {
	TARGET_OBJECT_SHOW_TYPE_NORMAL		= 0,				/**< 일반 상태 <br> */
	TARGET_OBJECT_SHOW_TYPE_DEAD		= 1,				/**< 사망 상태 <br> */
	TARGET_OBJECT_SHOW_TYPE_MAX			= 2,
};


enum TARGET_LIST {
	TARGET_LIST_5,											/**< 6번 출력 <br> */
	TARGET_LIST_4,											/**< 5번 출력 <br> */
	TARGET_LIST_3,											/**< 4번 출력 <br> */
	TARGET_LIST_2,											/**< 3번 출력 <br> */
	TARGET_LIST_1,											/**< 2번 출력 <br> */
	TARGET_LIST_0,											/**< 1번 출력 <br> */
	TARGET_LIST_MAX,
};


const int MAX_VIEW_LIST			= 6;						/**< 리스트 최대 갯수 <br> */
const int MOB_ICON_WIDTH		= 32;
const int MOB_ICON_HEIGHT		= 32;


struct SPBeAttackedObject {
	TARGET_OBJECT_TYPE			m_iObjectType;				/**< 품고 있는 객체 타입 <br> */
	SPGameObject*				m_pGameObject;				/**< 링크된 객체 <br> 사용시 NULL체크 잘할것 <br> */
	TARGET_OBJECT_SHOW_TYPE		m_iShowType;				/**< 보여지는 현황 <br> */
	float						m_fDeleyTime;				/**< 다음 단계로 넘어가기 위한 대기시간 <br> */
	float						m_fAccmulateTime;			/**< 시간 계산 용 <br> */
	SPTexture*					m_pIconTexture;				/**< 아이콘 이미지 <br> */
	RECT						m_rtIcon;					/**< 아이콘 이미지 소스 영역 <br> */
	int							m_iMaxHp;					/**< 최대 HP <br> */
	int							m_iCurHp;					/**< 현재 HP <br> */
	float						m_fHpRatio;					/**< HP % <br> */	
	RECT						m_rtGageLeft;				/**< Hp Gage영역 <br> */
	RECT						m_rtGageRight;				/**< Hp Gage영역 <br> */
	std::string					m_strName;					/**< 몬스터 이름 <br> */

	SPBeAttackedObject() {
		Clear();
	}

	~SPBeAttackedObject() {
		if(m_pIconTexture) {
			SAFE_RELEASE(m_pIconTexture);
		}
		Clear();
	}
	
	void Clear();	

	/**
	 * 품을 객체를 설정 <br>
	*/
	void SetObject(SPGameObject* pObject, TARGET_OBJECT_TYPE iType);

	/**
	 * 보여지는 상황을 설정 <br> 
	*/
	void SetShowType(TARGET_OBJECT_SHOW_TYPE iShowType);
	
	/**
	 * Hp 백분율을 개산하여 타입별로 게이지 사이즈를 반환한다 <br>
	 @param 
	 @return  
	*/
	int SetHpRatio(int iMaxHp, int iCurHp);
	
	/**
	 * 게이지 사이즈와 출력 영역을 이용하여 게이지의 출력 영역을 설정한다 <br>
	 @param 
	 @return  
	*/
	void SetGageRect(int iHpWidth, RECT* p_rtMobGage);	
};


class SPBeAttackedList {
public:
	SPBeAttackedList();
	~SPBeAttackedList();

	void ClearAllList();

	void Process(float fTime);
	
	/**
	 * 다음 단계로 넘어가기 위한 체크 프로세스 <br>
	 @param 
	 @return  
	*/
	void CheckObjectProcess(float fTime);

	/**
	 * 게이지바 처리를 위한 영역 계산을 한다 <br> 
	 @param 
	 @return  
	*/
	void RenderProcess(float fTime);

	/**
	 * 렌더링
	 @param 
	 @return  
	*/
	void Render(float fTime);

	/**
	* 퀵슬롯 사이즈의 크기에 따른 위치 재조정을 한다 <br> 
	@param 
	@return  
	*/
	void SetQuickSlotSize(int iSize);

	/**
	 * 객체 추가 <br>
	 @param 
	 @return  
	*/
	bool AddObject(SPGameObject* pObject, TARGET_OBJECT_TYPE iType);	
	
	/**
	 * 보스몹 추가 <br>
	 @param 
	 @return  
	*/
	bool AddBossObject(SPGameObject* pObject, TARGET_OBJECT_TYPE iType = TARGET_OBJECT_TYPE_BOSS);
	
	
	/**
	 * 객체 삭제 <br>
	 @param GU_ID iId
	 @return  
	*/
	bool DelObject(GU_ID iId);
	
	/**
	 * 객체의 죽음 상태 설정 <br> 
	 @param GU_ID iId
	 @return  
	*/
	bool SetDead(GU_ID iId);	

protected:			
	/**
	 * 출력영역을 설정한다 <br>
	 @param 
	 @return  
	*/
	void SetDestRect();
	
	/**
	 * 디버깅용 <br>
	 @param 
	 @return  
	*/
	void DebugListSize();	
	
	/**
	 * 주어진 아이디로 객체 검사 <br>
	 @param GU_ID iId
	 @return  
	*/
	SPBeAttackedObject* FindObject(GU_ID iId);	

private:
	SPBeAttackedObject*					m_pBossBeAttacked;						/**< 보스 피격 정보 <br> */
	std::vector<SPBeAttackedObject*>	m_vpBeAttackedList;						/**< 몹, 플레이어 피격 정보 리스트 <br> */

	int									m_iStartX;								/**< 렌더링 시작 영역 X <br> */
	int									m_iStartY;								/**< 렌더링 시작 영역 Y <br> */
	int									m_iQuickSlotSize;						/**< 현재 퀵슬롯의 크기 <br> */

	RECT								m_rtBossIconDest;						/**< 보스 아이콘 출력 영역 <br> */
	RECT								m_rtBossIconLineDest;					/**< 보스 아이콘 테두리 출력 영역 <br> */
	RECT								m_rtBossNameDest;						/**< 보스 이름 출력 영역 <br> */
	RECT								m_rtBossGageDestLeft;					/**< 보스 Hp Gage 베이스 출력 영역 <br> */
	RECT								m_rtBossGageDestCenter;					/**< 보스 Hp Gage 베이스 출력 영역 <br> */
	RECT								m_rtBossGageDestRight;					/**< 보스 Hp Gage 베이스 출력 영역 <br> */

	RECT								m_rtBossIconLineSrc;					/**< 보스 테두리 소스 영역 <br> */
	RECT								m_rtBossNameSrc;						/**< 보스 이름 소스 영역 <br> */
	RECT								m_rtBossGageSrcLeft;					/**< 보스 Hp 베이스 소스 <br> */
	RECT								m_rtBossGageSrcCenter;					/**< 보스 Hp 베이스 소스 <br> */
	RECT								m_rtBossGageSrcRight;					/**< 보스 Hp 베이스 소스 <br> */
	RECT								m_rtBossHpSrcLeft;						/**< 보스 Hp 소스 <br> */
	RECT								m_rtBossHpSrcRight;						/**< 보스 Hp 소스 <br> */
	
	RECT								m_rtMobIconDest[TARGET_LIST_MAX];		/**< 일반 몹 Icon 대상 <br> */
	RECT								m_rtMobGageDest[TARGET_LIST_MAX];		/**< 일반 몹 Gage 대상 <br> */
	RECT								m_rtMobHpDest[TARGET_LIST_MAX];			/**< 일반 몹 HP 대상 <br> */

	SPTexture*							m_pTexture;								/**< Hp이미지, Gage이미지 <br> */
	RECT								m_rtHpSrc;								/**< Hp이미지 소스 영역 <br> */
	RECT								m_rtGageBaseSrc;						/**< Hp게이지 바탕의 소스 영역 <br> */
	RECT								m_rtGageSrc;							/**< 일반 Hp게이지 소스 영역 <br> */	

	RECT								m_rtDeadSrc;							/**< 사망시 몹 Icon 소스 영역 <br> */
	RECT								m_rtMobSkinSrc;							/**< 일반몹 Icon 바탕 <br> */
};
