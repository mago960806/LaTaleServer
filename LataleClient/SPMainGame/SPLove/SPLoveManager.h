// ***************************************************************
//  SPLoveManager		Version:  1.0		Date: 2008/09/25
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#ifndef SPLoveManager_h__
#define SPLoveManager_h__

#pragma once

enum LOVE_LDT_REQUIRE
{
	LOVE_LDT_REQUIRE_NULL		=	0				,
	LOVE_LDT_REQUIRE_FRIENDSHIP_CONSUME_ELY			,	//	133	[결혼]_(교제)_Ely 소비 금액
	LOVE_LDT_REQUIRE_FRIENDSHIP_CONSUME_ITEM		,	//	14	[결혼]_(교제)_교제 아이템 ID
	LOVE_LDT_REQUIRE_PROPOSE_COMSUME_ELY			,	//	105	[결혼]_(프러포즈)_ 프러포즈 소비 금액
	LOVE_LDT_REQUIRE_PROPOSE_COMSUME_ITEM			,	//	18	[결혼]_(프러포즈)_ 프러포즈 아이템
	LOVE_LDT_REQUIRE_PROPOSE_POSSIBLE_DISTANCE		,	//	19	[결혼]_(프러포즈)_프러포즈 가능 픽셀 단위 거리
	LOVE_LDT_REQUIRE_PROPOSE_POSSIBLE_LOVEPOINT		,	//	20	[결혼]_(프러포즈)_프러포즈 가능 애정도
	LOVE_LDT_REQUIRE_PARTING_CONSUME_ELY			,	//	26	[결혼]_(이별)_이별 시 소비되는 Ely
	LOVE_LDT_REQUIRE_PARTING_PENALTY_TIME			,	//	27	[결혼]_(이별)_기간 페널티 초 단위 시간
	LOVE_LDT_REQUIRE_COMMON_MESSAGEBOX_REMAIN_TIME	,	//	28	[결혼]_(공통)_팝업 메시지 창 대기 초 단위 시간
	LOVE_LDT_REQUIRE_WEDDING_POSSIBLE_DISTANCE		,	//	29	[결혼]_(결혼식)_결혼식 거리 체크
	LOVE_LDT_REQUIRE_WEDDING_SHOUT_REMAIN_TIME		,	//	30	[결혼]_(결혼식)_외치기 입력 초단위 대기 시간
	LOVE_LDT_REQUIRE_WEDDING_EQUIP_SET_EFFECT		,	//	24	[결혼]_(결혼식)_웨딩 아이템 착용 체크 세트 이펙트아이디

	LOVE_LDT_REQUIRE_MAX							,
};


const int	RECALL_EFFECT_ID						= 100003;	/**< 연인 소환 Effect ID <br> */  


struct LOVE_LEVEL_DATA 
{
	int												m_iLevel;
	std::string										m_strTitle;
};

typedef std::map< int , LOVE_LEVEL_DATA >			STD_MAP_LOVE_LEVEL_DATA;

class SPPlayer;

class SPLoveManager
{
private:
	SPLoveManager									();

	static	SPLoveManager*							m_pLoveManagerInstance;

public:
	virtual ~SPLoveManager							();

	static	SPLoveManager*							GetInstance				(void);
	static	void									DelInstance				(void);

	//	초기화
	void											Init					();
	void											ClearVariables			();

	int												GetRequire				( LOVE_LDT_REQUIRE eChatLDTRequire );

	LOVE_LEVEL_DATA*								GetLevelData			( int iLevel );

	void											SetLoveInfo				( WEDDING_INFO& stLoveInfo );
	WEDDING_INFO*									GetLoveInfo				();
	void											ClearLoveInfo			();
	int												GetCurPartingTime		();

	void											SetLifePartnerGUID		( GU_ID eLifePartner );
	GU_ID											GetLifePartnerGUID		();
	const char*										GetLifePartnerString	();

	SPPlayer*										GetLifePartner			();

	void											SetCutIn				( bool bCutIn );


	/**  
	 * NPC 선택지에서 결혼식 요청 <br>
	 @param  
	 @return 
	*/  
	bool											SetWeddingRequest		();
	
	/**  
	 * WEDDING_CS_REQUEST <br>
	 * 러브 관련 신청 패킷을 서버에 보낸다. <br>
	 @param  WEDDING_TYPE iType
	 @param  int iEly
	 @param  CONTAINER_SLOT SlotInfo
	 @param  std::string strName
	 @return bool
	*/  
	bool											SendWeddingRequest		(WEDDING_TYPE iType, int iEly, CONTAINER_SLOT SlotInfo, std::string strName);

	/**  
	 * WEDDING_SC_REQUEST <br>
	 * 러브 관련 신청에 관련 결과를 처리한다. <br>	 
	 * 실제 신청에 관한 응답은 Response에서 처리한다 <br>
	 @param  CPacket* pPacket
	 @return bool	 
	*/  
	bool											OnWeddingRequest		(CPacket* pPacket);

	/**  
	 * WEDDING_SC_REQUEST_RECV <br>
	 * 러브 관련 신청을 받음 <br>
	 @param  
	 @return 
	*/  
	// SPWIndowLove 인터페이스 처리함 [11/10/2008 AJJIYA]
//	bool											OnWeddingRequestRecv	(CPacket* pPacket);

	/**  
	 * WEDDING_CS_RESPONSE <br>
	 * 러브 관련 신청에 대한 응답을 한다 <br>
	 * 러브 관련 응답 패킷을 서버에 보낸다 <br>
	 @param  WEDDING_TYPE iType
	 @param  bool bAccept						0인 경우 거절 1인경우 수락
	 @param  std::string strName				요청자 이름
	 @return 
	*/ 
	// SPWIndowLove 인터페이스 처리함 [11/10/2008 AJJIYA]
//	bool											SendWeddingResponse		(WEDDING_TYPE iType, bool bAccept, std::string strName);

	/**  
	 *  WEDDING_SC_RESPONSE <br>
	 * 러브 관련 요청에 대한 응답을 처리한다 <br>
	 @param  
	 @return 
	*/  
	// SPWIndowLove 인터페이스 처리함 [11/10/2008 AJJIYA]
//	bool											OnWeddingResponse		(CPacket* pPacket);
	

	/**  
	 * 이별 시 Notice에 필요한 메시지를 만들어 이별할것인지 물어본다 <br>
	 @param  
	 @return 
	*/  
	bool											GetWeddingDivorceMsgIndex	();

	/**  
	 * 이별시 Notice에서 Yes를 선택한 경우 이별 작업을 실행한다 <br>
	 @param  
	 @return 
	*/  
	bool											SetWeddingDivorce		();	

	/**  
	 * WEDDING_SC_PARTING <br>
	 * 이별 신청에 대한 응답을 받는다 <br>
	 @param  
	 @return 
	*/  
	bool											OnWeddingDivorce		(CPacket* pPacket);

	/**  
	* WEDDING_SC_WEDDING_CANCEL <br>
	* 웨딩 상태라면 모든 웨딩상태를 취소시킨다. <br>
	@param  
	@return 
	*/  

	void											OnWeddingCancel			( CPacket* pPacket );

	/**  
	* WEDDING_SC_MOTION_STARTEND <br>
	* 강제 모션 상태 시작시 키보드 입력 처리를 한다. <br>
	@param  
	@return 
	*/  

	void											OnMotionStartEnd		( CPacket* pPacket );

	/**  
	 * 로컬 플레이어에게 추가되는 이팩트 중에 배우자 소환 이 들어왔는지 확인 <br>
	 * 해당 소환이 들어온 경우 소환 받을수 있는지 확인하여 소환 여부를 묻는 notice를 뛰운다 <br>
	 @param  
	 @return 
	*/  
	//bool											CheckRecallEffect		( int iEffectID );

	/**  
	 * 소환 실행 요청 <br>
	 * 팝업 메시지로 소환에 응할것인지 확인 할수 있도록 한다 <br>
	 @param  
	 @return 
	*/  
	bool											OnRequestRecall			( CPacket* pPacket );

	/**  
	* 소환 시전자에게 소환이 실패 했는지 알림 <br>
	@param  
	@return 
	*/  
	bool											OnRequestRecallErr		( CPacket* pPacket );

	/**  
	 * 소환 여부에 관한 응답을 알림 <br>
	 @param  
	 @return 
	*/  
	bool											SendAcceptRecall		( bool bAccept );
	
	/**  
	 * 소환 여부 응답 알림의 결과 <br> 
	 * 소환 대상자의 의사 응답 <br>
	 @param  
	 @return 
	*/  
	bool											OnAcceptRecall			( CPacket* pPacket );

	/**  
	 * 소환 여부 응답 알림의 결과 <br> 
	 * 소환 스킬 시전자에게 소환 대상자의 의사 응답 <br>
	 @param  
	 @return 
	*/  
	bool											OnResultRecall			( CPacket* pPacket );

	D3DXCOLOR										GetD3DColor				( int ColorIndex );
	D3DXCOLOR*										GetD3DColor2			( int ColorIndex );
	
	void											CleanColor2();

	void											LoadColorNameLDT		();
protected:

	void											LoadGlobalSysLDT		();
	void											LoadLDT					();

	void											AddLevelData			( LOVE_LEVEL_DATA& stLoveLevelData );

	/**  
	* WEDDING_CS_PARTING 용 임시 <br>  
	* 교제 또는 결혼 상태에서 갈라서기 신청을 한다 <br>
	@param  
	@return 
	*/  
	bool											SendWeddingDivorce		(int iEly);

	/**  
	 * 배우자 소환시 내가 소환 받을수 있는 상황인지 체크 한다 <br>
	 @param  
	 @return 
	*/  
	//bool											IsEnableRecall			();

	//////////////////////////////////////////////////////////////////////////
	//
	//	변수들
	//

	int												m_iRequireLDT[ LOVE_LDT_REQUIRE_MAX ];

	STD_MAP_LOVE_LEVEL_DATA							m_mLoveLevelData;

	WEDDING_INFO									m_stLoveInfo;
	GU_ID											m_eLifePartner;

	double											m_iAbsoluteTime;
	enum { ColorNameNum = 11 };
	D3DXCOLOR										m_ColorName[ ColorNameNum ];

	D3DXCOLOR*										m_ColorNameTwo[ ColorNameNum ];

};

#endif // SPLoveManager_h__