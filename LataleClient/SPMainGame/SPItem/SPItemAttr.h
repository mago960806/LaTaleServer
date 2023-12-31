// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPItemAttr
// Comment     : 
// Creation    : metalgeni 2004-11-05 오전 10:26:19
//***************************************************************************

#pragma once

#include <string>

using namespace std;

enum ITEM_RARE_LIMIT
{
	ITEM_RARE_NORMAL	= 1,	//노멀
	ITEM_RARE_MAGIC		= 2,	//매직
	ITEM_RARE_RARE		= 3,	//레어
	ITEM_RARE_ADD1 		= 4,	//추가1
	ITEM_RARE_ADD2		= 5,	//추가2
};								

enum ITEM_TYPE
{
	ITEM_TYPE_NULL		= 0,
	ITEM_TYPE_FIGURE	= 1,	// 장비
	ITEM_TYPE_CONSUME	= 2,	// 소비
	ITEM_TYPE_ETC		= 3,	// 기타
	ITEM_TYPE_EVENT		= 4,			
	ITEM_TYPE_PET		= 5,
	ITEM_TYPE_HOUSING	= 6,	
};

enum ITEMSTATUS_ID {	
	IS_NULL   = -2,
	IS_PARENT = -1,

	IS_EQUIP			= 0,	
	IS_CONSUMABLE		= 1, //IS_USE_INSTANT,
	IS_ETC				= 2,
	IS_EVENT			= 3,
	IS_PET				= 4,
	IS_HOUSING			= 5,

	IS_EQUIP_BATTLE		= 6, // 아이템 종류만 의미했었는데 영현씨가 위치까지 쓰도록 했음..
	IS_EQUIP_FASHION	= 7,
	//IS_QUEST,
};

/**
 * 아이템 유지시간 타입 <br>
*/
enum ITEM_DURATION {
	ITEM_DURATION_NULL			= 0,	//일반
	ITEM_DURATION_AMOUNT		= 1,	//정액 시간제
	ITEM_DURATION_QUANTITY		= 2,	//정량 시간제
};



enum ITEM_SUBTYPE
{
	ITEM_SUBTYPE_NULL = 0,
	//(장비)
	ITEM_SUBTYPE_EQUIP_EQUIP	=	1,	//장비
	ITEM_SUBTYPE_EQUIP_FASHION	=	2,	//패션
	ITEM_SUBTYPE_EQUIP_EFFECT	=	3,
	//(소비)
	ITEM_SUBTYPE_WASTE_NORMAL	=	1, 	//일반
	ITEM_SUBTYPE_WASTE_ENCHANT	=	2,	//인챈트
	ITEM_SUBTYPE_WASTE_DEFAULT	=	3,	//디폴트
	ITEM_SUBTYPE_WASTE_MSGBOARD	=	4,	//[xialin 2008/09/11]add
	ITEM_SUBTYPE_WASTE_SEARCH	=	5,  //冷훙돛야
	ITEM_SUBTYPE_WASTE_FOLLOW	=	6,	//뫄踞돛야
	ITEM_SUBTYPE_WASTE_SNATCH	=	7,	//婁훙돛야

	//(펫)
	ITEM_SUBTYPE_PET_EQUIP		=	1,	//펫장비
	ITEM_SUBTYPE_PET_WASTE		= 	2,	//펫소비
	//(이벤트)
	ITEM_SUBTYPE_EVENT_QUEST	=	1,	//이벤트퀘스트
	ITEM_SUBTYPE_EVENT_GAMBLE	=	2,	//이벤트겜블
};


enum ITEM_EQUIPPOS
{
	ITEM_EQUIPPOS_NULL = 0,		//	null
	//(피규어, 패션 공용)
	ITEM_EQUIPPOS_HAIRDRESS 		= 1,		//	머리장식
	ITEM_EQUIPPOS_GOGGLE			= 2,		//	안경
	ITEM_EQUIPPOS_EARING			= 3,		//	귀걸이
	ITEM_EQUIPPOS_SHIRTS			= 4,		//	상의
	ITEM_EQUIPPOS_PANTS				= 5,		//	하의
	ITEM_EQUIPPOS_CLOAK				= 6,		//	망토
	ITEM_EQUIPPOS_GLOVE				= 7,		//	장갑
	ITEM_EQUIPPOS_SHOES				= 8,		//	신발
	ITEM_EQUIPPOS_STOCKING			= 9	,		//	스타킹
	ITEM_EQUIPPOS_MAKEUP			= 10,		//	얼굴장식
	ITEM_EQUIPPOS_STONE				= 11,		//	정령석
	ITEM_EQUIPPOS_RING				= 12,		//	반지
	ITEM_EQUIPPOS_ONEHAND_WEAPON	= 13,		//	한손무기
	ITEM_EQUIPPOS_TWOHAND_WEAPON	= 13,		//	양손무기
	ITEM_EQUIPPOS_SHIELD			= 14,		//	방패
	ITEM_EQUIPPOS_ONEPIECE			= 4	,		//	한벌옷	
	ITEM_EQUIPPOS_DEFAULT_SHORTS	= 1	,		//	속옷상의
	ITEM_EQUIPPOS_DEFAULT_PANTS		= 2	,		//	속옷하의
	ITEM_EQUIPPOS_HAIR				= 3	,		//	머리카락
	ITEM_EQUIPPOS_EXPRESSION 		= 4	,		//	얼굴표정
	ITEM_EQUIPPOS_SKIN				= 5	,		//	피부색	

	ITEM_EQUIPPOS_PET_HEAD			= 1	,		//	머리		펫
	ITEM_EQUIPPOS_PET_BODY			= 2	,		//	몸			펫

	ITEM_EQUIPPOS_ONEHAND_WEAPON2	= 15,		//	한손무기
	ITEM_EQUIPPOS_TWOHAND_WEAPON2	= 15,		//	양손무기
	ITEM_EQUIPPOS_SHIELD2			= 16,		//	방패
};										



enum ITEM_EQUIP_KIND {
	ITEM_EQUIP_KIND_NULL			= 0,	//null
	ITEM_EQUIP_KIND_KNUCKLE			= 1,	//너클
	ITEM_EQUIP_KIND_DAGGER			= 2,	//단검
	ITEM_EQUIP_KIND_SWORD			= 3,	//검
	ITEM_EQUIP_KIND_TWOHANDSWORD	= 4,	//양손검
	ITEM_EQUIP_KIND_SABER			= 5,	//도
	ITEM_EQUIP_KIND_IDORYU			= 6,	//이도
	ITEM_EQUIP_KIND_CLUB			= 7,	//둔기
	ITEM_EQUIP_KIND_SPEAR			= 8,	//창
	ITEM_EQUIP_KIND_BOW				= 9,	//활
	ITEM_EQUIP_KIND_CROSSBOW		= 10,	//석궁
};


enum ITEM_EQUIP_EFFECT
{
	ITEM_EQUIP_EFFECT_NULL = 0,		//	null	

	//(Pc Status. 효과 및 제한 설정에 공통으로 쓰임)	
	ITEM_EQUIP_EFFECT_CLASS					=	1 ,				//캐릭터 유형
	ITEM_EQUIP_EFFECT_GENDER				=	2 ,				//성별
	ITEM_EQUIP_EFFECT_LV					=	3 ,				//현재 레벨
	ITEM_EQUIP_EFFECT_MONEY					=	4 ,				//현재 돈
	ITEM_EQUIP_EFFECT_EXP					=	5 ,				//현재 경험치
	ITEM_EQUIP_EFFECT_HP					=	6 ,				//현재 HP
	ITEM_EQUIP_EFFECT_SP					=	7 ,				//현재 SP
	ITEM_EQUIP_EFFECT_CURR_FAME				=	8 ,				//명성 최종값
	ITEM_EQUIP_EFFECT_BASE_FAME				=	9 ,				//기본 명성
	ITEM_EQUIP_EFFECT_ADD_FAME				=	10,				//명성 + 
	ITEM_EQUIP_EFFECT_RATIO_FAME			=	11,				//명성 %
	ITEM_EQUIP_EFFECT_CURR_ATK				=	12,				//최종 공격력
	ITEM_EQUIP_EFFECT_BASE_ATK				=	13,				//기본 공격력
	ITEM_EQUIP_EFFECT_ADD_ATK				=	14,				//공격력 +
	ITEM_EQUIP_EFFECT_RATIO_ATK				=	15,				//공격력 %
	ITEM_EQUIP_EFFECT_CURR_LUK				=	16,				//최종 행운
	ITEM_EQUIP_EFFECT_BASE_LUK				=	17,				//기본 행운
	ITEM_EQUIP_EFFECT_ADD_LUK				=	18,				//행운 +
	ITEM_EQUIP_EFFECT_RATIO_LUK				=	19,				//행운 %
	ITEM_EQUIP_EFFECT_CURR_ELE				=	20,				//최종 마법력
	ITEM_EQUIP_EFFECT_BASE_ELE				=	21,				//기본 마법력
	ITEM_EQUIP_EFFECT_ADD_ELE				=	22,				//마법력 +
	ITEM_EQUIP_EFFECT_RATIO_ELE				=	23,				//마법력 %
	ITEM_EQUIP_EFFECT_CURR_DEF				=	24,				//최종 방어력
	ITEM_EQUIP_EFFECT_BASE_DEF				=	25,				//기본 방어력
	ITEM_EQUIP_EFFECT_ADD_DEF				=	26,				//방어력 +
	ITEM_EQUIP_EFFECT_RATIO_DEF				=	27,				//방어력 %
	ITEM_EQUIP_EFFECT_CURR_MHP				=	28,				//최종 최대 Hp
	ITEM_EQUIP_EFFECT_BASE_MHP				=	29,				//기본 최대 HP
	ITEM_EQUIP_EFFECT_ADD_MHP				=	30,				//최대 HP +
	ITEM_EQUIP_EFFECT_RATIO_MHP				=	31,				//최대 HP %
	ITEM_EQUIP_EFFECT_CURR_MSP				=	32,				//최종 최대 SP
	ITEM_EQUIP_EFFECT_BASE_MSP				=	33,				//기본 최대 SP
	ITEM_EQUIP_EFFECT_ADD_MSP				=	34,				//최대 SP +
	ITEM_EQUIP_EFFECT_RATIO_MSP				=	35,				//최대 SP %
	ITEM_EQUIP_EFFECT_CURR_WATER			=	36,				//최종 수속성
	ITEM_EQUIP_EFFECT_BASE_WATER			=	37,				//기본 수속성
	ITEM_EQUIP_EFFECT_ADD_WATER				=	38,				//수속성 +
	ITEM_EQUIP_EFFECT_RATIO_WATER			=	39,				//수속성 %
	ITEM_EQUIP_EFFECT_CURR_WIND				=	40,				//최종 풍속성
	ITEM_EQUIP_EFFECT_ADD_WIND				=	41,				//기본 풍속성
	ITEM_EQUIP_EFFECT_BASE_WIND				=	42,				//풍속성 +
	ITEM_EQUIP_EFFECT_RATIO_WIND			=	43,				//풍속성 %
	ITEM_EQUIP_EFFECT_CURR_FIRE				=	44,				//최종 화속성
	ITEM_EQUIP_EFFECT_BASE_FIRE				=	45,				//기본 화속성
	ITEM_EQUIP_EFFECT_ADD_FIRE				=	46,				//화속성 +
	ITEM_EQUIP_EFFECT_RATIO_FIRE			=	47,				//화속성 %
	ITEM_EQUIP_EFFECT_CURR_EARTH			=	48,				//최종 지속성
	ITEM_EQUIP_EFFECT_BASE_EARTH			=	49,				//기본 지속성
	ITEM_EQUIP_EFFECT_ADD_EARTH				=	50,				//지속성 +
	ITEM_EQUIP_EFFECT_RATIO_EARTH			=	51,				//지속성 %
	ITEM_EQUIP_EFFECT_CURR_RECV_HP			=	52,				//최종 회복 HP
	ITEM_EQUIP_EFFECT_BASE_RECV_HP			=	53,				//기본 회복 HP
	ITEM_EQUIP_EFFECT_ADD_RECV_HP			=	54,				//회복 HP +
	ITEM_EQUIP_EFFECT_RATIO_RECV_HP			=	55,				//회복 HP %
	ITEM_EQUIP_EFFECT_CURR_RECV_SP			=	56,				//최종 회복 SP
	ITEM_EQUIP_EFFECT_BASE_RECV_SP			=	57,				//기본 회복 SP
	ITEM_EQUIP_EFFECT_ADD_RECV_SP			=	58,				//회복 SP +
	ITEM_EQUIP_EFFECT_RATIO_RECV_SP			=	59,				//회복 SP %
	ITEM_EQUIP_EFFECT_CURR_HEAVY			=	60,				//최종 무기 강한 공격력
	ITEM_EQUIP_EFFECT_BASE_HEAVY			=	61,				//기본 무기 강한 공격력
	ITEM_EQUIP_EFFECT_ADD_HEAVY				=	62,				//무기 강한 공격력 +
	ITEM_EQUIP_EFFECT_RATIO_HEAVY			=	63,				//무기 강한 공격력 %
	ITEM_EQUIP_EFFECT_CURR_NORMAL			=	64,				//최종 무기 일반 공격력
	ITEM_EQUIP_EFFECT_BASE_NORMAL			=	65,				//기본 무기 일반 공격력
	ITEM_EQUIP_EFFECT_ADD_NORMAL			=	66,				//무기 일반 공격력 +
	ITEM_EQUIP_EFFECT_RATIO_NORMAL			=	67,				//무기 일반 공격력 %
	ITEM_EQUIP_EFFECT_CURR_MP_WATER			=	68,				//최종 수속성 공격력
	ITEM_EQUIP_EFFECT_BASE_MP_WATER			=	69,				//기본 수속성 공격력
	ITEM_EQUIP_EFFECT_CURR_MP_WIND			=	70,				//최종 풍속성 공격력
	ITEM_EQUIP_EFFECT_ADD_MP_WIND			=	71,				//기본 풍속성 공격력
	ITEM_EQUIP_EFFECT_CURR_MP_FIRE			=	72,				//최종 화속성 공격력
	ITEM_EQUIP_EFFECT_BASE_MP_FIRE			=	73,				//기본 화속성 공격력
	ITEM_EQUIP_EFFECT_CURR_MP_EARTH			=	74,				//최종 지속성 공격력
	ITEM_EQUIP_EFFECT_BASE_MP_EARTH			=	75,				//기본 지속성 공격력
	ITEM_EQUIP_EFFECT_ADD_MP_ELEMENTAL		=	76,				//마법 공격력 +(각 속성 공통)
	ITEM_EQUIP_EFFECT_RATIO_MP_ELEMENTAL	=	77,				//마법 공격력 %(각 속성 공통)
	ITEM_EQUIP_EFFECT_CURR_AP_HEAVY			=	78,				//최종 강한 공격력
	ITEM_EQUIP_EFFECT_BASE_AP_HEAVY			=	79,				//기본 강한 공격력
	ITEM_EQUIP_EFFECT_CURR_AP_NORMAL		=	80,				//최종 일반 공격력
	ITEM_EQUIP_EFFECT_BASE_AP_NORMAL		=	81,				//기본 일반 공격력
	ITEM_EQUIP_EFFECT_ADD_AP_ATK			=	82,				//물리 공격력 +(Heavy, Normal 공통)
	ITEM_EQUIP_EFFECT_RATIO_AP_ATK			=	83,				//물리 공격력 %(Heavy, Normal 공통)
																			
	ITEM_EQUIP_EFFECT_EFFECTID				= 1000	,	//// EffectID	(Effect)

//////////////////////////////////////////////////////////////////////////


			
//////////////////////////////////////////////////////////////////////////



};

enum ICON_MARK {
	ICON_MARK_NULL			= 0,
	ICON_MARK_LOCK			= 1,
	ICON_MARK_FASHION		= 2,
	ICON_MARK_EVENT			= 3,
	ICON_MARK_FANCY			= 4,
	ICON_MARK_MAX			= 5
};

enum CASHITEM_CATEGORY
{
	CI_NULL = 0,
	CI_FASSION = 1,
	CI_PET,
	CI_ETC,
	CI_HOUSING,
	CI_PVP,
	CI_PACKAGE,
	CI_POINT,

	CI_EVENT = 10,
	CI_SEARCH,     //[xialin 2008/07/17] add
};

enum CASHITEM_CATEGORY_SUB1
{
	CIS1_NULL = 0,
	CIS1_DRESS = 1,
	CIS1_HELMET,
	CIS1_SHOES,
	CIS1_GLOVE,
	CIS1_ACCESSARIES,
	CIS1_ETC,
	CIS1_PET,
	CIS1_PET_ETC,
	CIS1_FUNCTIONAL,
	CIS1_BEAUTY,
	CIS1_COMMUNITY,
	CIS1_HOUSING,
	CIS1_PVP,
	CIS1_PACKAGE_FASSION,
	CIS1_PACKAGE_PET,
	CIS1_PACKAGE_HOUSING,
	CIS1_PACKAGE_PVP,
	CIS1_PACKAGE_ETC,

	CIS1_EVENT_NEW,
	CIS1_EVENT_HOT,
	CIS1_EVENT_EVENT,
	CIS1_POINT_FASSION,
	CIS1_POINT_PET,
	CIS1_POINT_FUNCTIONAL,
};

enum CASHITEM_CATEGORY_SUB2
{
	CIS2_NULL = 0,
	CIS2_ONEPEACE = 1,
	CIS2_BLOUSE,
	CIS2_PANTS,
	CIS2_COAT,
	CIS2_HELMET,
	CIS2_HEADBAND,
	CIS2_HAIRPIN,
	CIS2_SHOES,
	CIS2_LEGBAND,
	CIS2_GLOVE,
	CIS2_ARMBAND,
	CIS2_GLASS,
	CIS2_EARING,
	CIS2_MAKEUP,
	CIS2_STOCKINGS,
	CIS2_EMOTICON,
	CIS2_EXPRESS,
	CIS2_EVENT,
	CIS2_CHATTING,
};

enum EQUIP_ID;

struct SPEffectValue {
	ITEM_EQUIP_EFFECT	Effect;
	int					iValue;
};

struct SPItemRequire {
	int					m_iType;
	int					m_iID;
	int					m_iValue1;
	int					m_iValue2;
	SPItemRequire& operator = (const SPItemRequire& indunInfo)
	{
		m_iType		= indunInfo.m_iType		;
		m_iID		= indunInfo.m_iID		;
		m_iValue1	= indunInfo.m_iValue1	;
		m_iValue2	= indunInfo.m_iValue2	;
		return *this;
	}
};


const int ITEM_SET_A_MAX				 = 11;			/**< 11가지의 셋 <br> */
const int ITEM_SET_FUUL_OPTION			 = 4;			/**< 풀셋인경우 옵션 최대치 <br> */

#define MAX_ITEM_OPTION 4
#define MAX_ITEM_EFFECT 4
#define MAX_ITEM_SKILL	4


struct SPItemAttr {
	// m_iCategory, m_iNum, m_iColorID 가 합쳐저 Item ID를 만든다
	int							m_iCategory			;		//카테고리
	int							m_iNum				;		//?
	int							m_iColorID			;		//컬러
	
	string						m_strName			;
	string						m_strDescription	;
	int							m_iIcon				;
	int							m_iIconIndex		;
	int							m_iGamePrice		;
	int							m_iCashPrice		;
	ITEM_RARE_LIMIT				m_eRareLimit		;
	ITEM_TYPE					m_eType				;
	ITEM_SUBTYPE				m_eSubType			;
	ITEM_EQUIPPOS				m_ePosID1			;
	ITEM_EQUIPPOS				m_eSubPosID11		;
	ITEM_EQUIPPOS				m_eSubPosID12		;
	ITEM_EQUIPPOS				m_ePosID2			;
	ITEM_EQUIPPOS				m_eSubPosID21		;
	ITEM_EQUIPPOS				m_eSubPosID22		;
	ITEM_EQUIP_KIND				m_eKind				;

	SPEffectValue				m_StatusType	[MAX_ITEM_OPTION];
	SPEffectValue				m_StatusTypePvp [MAX_ITEM_OPTION];	//PVP에 사용되는 아이템스탯
	SPItemRequire				m_RequireType	[MAX_ITEM_OPTION];
	//ITEM_EQUIP_EFFECT			m_eStatusType1		;
	//int							m_iStatusValue1		;
	//ITEM_EQUIP_EFFECT			m_eStatusType2      ;   
	//int							m_iStatusValue2     ;   
	//ITEM_EQUIP_EFFECT			m_eStatusType3      ;   
	//int							m_iStatusValue3     ;   
	//ITEM_EQUIP_EFFECT			m_eStatusType4      ;   
	//int							m_iStatusValue4     ;   
	//ITEM_EQUIP_EFFECT			m_eRequireType1		;
	//int							m_iRequireValue1    ;   
	//ITEM_EQUIP_EFFECT			m_eRequireType2     ;   
	//int							m_iRequireValue2    ;   
	//ITEM_EQUIP_EFFECT			m_eRequireType3     ;   
	//int							m_iRequireValue3    ;   
	//ITEM_EQUIP_EFFECT			m_eRequireType4     ;   
	//int							m_iRequireValue4    ;   

	int							m_iCoolTime1		;
	int							m_iCoolTime2		;
	int							m_iCoolTimeApplyID1	;
	int							m_iCoolTimeApplyID2	;
	int							m_iItemCoolTimeID	;
	bool						m_bCashCheck		;
	int							m_iDurationTime		;
	int							m_iStackLimit		;
	int							m_iItemLv			;
	bool						m_bLogging			;
	bool						m_bSerialized		;
	bool						m_bTrade			;
	bool						m_bDestroy			;
	bool						m_bStorage			;
	bool						m_bSell				;
	int							m_iItemPointMin		;
	int							m_iItemPointMax		;
	int							m_iOptionID1		;
	int							m_iOptionID2		;
	int							m_iOptionID3		;
	int							m_iOptionID4		;
	int							m_iMotionTypeID1	;
	int							m_iMotionTypeID2	;
	int							m_iEffectID[ MAX_ITEM_EFFECT ];	// 효과 ID  AJJIYA [7/21/2006]
	int							m_iOptionType		;
	int							m_iOptionClass		;
	
	ICON_MARK 					m_eIconMark			;	// 아이콘에 추가 표시

	bool						m_bNameColor		;	// 우선순위 이름 컬러
	byte						m_cRed				;
	byte						m_cGreen			;
	byte						m_cBlue				;

	ITEM_DURATION				m_eDuration			;	// 시간 아이템 유지 타입
	int							m_iLifeTime			;	// 정량제아이템의 시간

	int							m_iMacroType;

	unsigned int				m_iPetClassID;
	INT64						m_iPetMaxExp;
	int							m_iSkillID[MAX_ITEM_SKILL];
	int							m_iSkillLevel[MAX_ITEM_SKILL];
	int							m_iConsumeSP[MAX_ITEM_SKILL];

	int							m_iSetID;						/**< 아이템의 Set ID <br> */
	int							m_iQuestID;						// 아이템의 퀘스트 ID 이벤트일때만 사용 [10/2/2007 AJJIYA]
	bool						m_bPvp;							/**< Pvp에서의 사용 가능 여부 필드 <br> */
	int							m_iCashPoint;					//캐시포인트

	int							m_iItemLevel;					/**< 아이템 레벨 <br> */
	int							m_iAuctionCategory;				/**< 경매 구분 <br> */
	int							m_iAuctionQuality;				/**< 경매 품질 <br> */
	int							m_iShopID;						/**< 캐시샵 ID <br> */

	int							m_iCash;						/**< 캐시 거래 가 <br> */
	int							m_iPoint;						/**< 포인트 거래 가 <br> */

	int							m_iNewPet;						//!< 1 for new pet, 0 fo old pet

	bool						m_bWedding;						/**< 결혼 아이템인지 <br> true인 경우 결혼 아이템 <br> */  

	SPItemAttr();	
	void	SetNull();
	virtual ~SPItemAttr();
	
	/**
	 * 우선순위 아이템 이름 컬러를 가져온다 <br> 
	 @param NONE
	 @return D3DXCOLOR
	*/
	D3DXCOLOR GetNameColor();
	
	/**
	 * 
	 @param 
	 @return
	*/
	ITEM_DURATION GetDurationType();	
	
	bool GetConvertFigureItem(FIGURE_ITEM* pFigureItem);

	void SetCash(int iCash);
	void SetPoint(int iPoint);
};

struct SPCashItemInfo
{
	int iID;
	int iRare;
	int iCount;
	int iPriceRate;

	SPCashItemInfo()
	{
		iID = 0;
		iRare = 0;
		iCount = 0;
		iPriceRate = 0;
	}
};

#define CASHITEMINFO_COUNT		8
#define CASHITEM_REQUIRE_NUM	3

struct SPCashItemAttr
{
	CASHITEM_CATEGORY eCategory;
	CASHITEM_CATEGORY_SUB1 eCategorySub1;
	CASHITEM_CATEGORY_SUB2 eCategorySub2;

	bool bView;
	int iCashID;
	int iPositionIndex;
	int iEventPage;
	int iIcon;
	int iIconIndex;
	int iPrice;
	int iSalePrice;
	int iEventIcon;
	int iMaxOZ;
	int iMaxMile;
	int iGender;
	int iMotionID;
	int iClassID;
	bool bBuyDisable;
	bool bGiftDisable;

	SPCashItemInfo ItemInfo[ CASHITEMINFO_COUNT ];
	SPItemRequire ItemRequire[CASHITEM_REQUIRE_NUM];
	
	string strName;
	string strDescription;
	string strHotDesc;

	SPCashItemAttr()
	{
		eCategory = CI_NULL;
		eCategorySub1 = CIS1_NULL;
		eCategorySub2 = CIS2_NULL;
		iCashID = 0;
		iPositionIndex = 0;
		iEventPage = 0;
		iIcon = 0;
		iIconIndex = 0;
		iPrice = 0;
		iSalePrice = 0;
		iEventIcon = 0;
		iMaxOZ = 0;
		iMaxMile = 0;
		iGender = 0;
		iMotionID = 0;
		iClassID = 0;
		bView = true;
		bBuyDisable = false;
		bGiftDisable = false;

		strName.clear();
		strDescription.clear();
		strHotDesc.clear();
	}

	void ClearHotDesc() {
		strHotDesc.clear();
	}

	void SetHotDesc(string strDesc) {
		strHotDesc.clear();
		strHotDesc = strDesc;
	}

	void UpdateCashPrize();
};

struct SPItemEnchant
{
	int iEnchantKey;
	int iItemOptionType;
	int iItemOptionClass;
	int iItemOptionGroup;
	int iItemOptionLevel;
	bool bItemOptionCheck;

	int iMaterialID;
	int iMaterialNum;

	int iOPMin;
	int iOPMax;
	int iProbabilityModifier;
	int iProbabilityMin;
	int iProbabilityMax;

	int iResult;
	int iWhenFailed;
	string strDescription;
	string strErrorMsg;

	SPItemRequire RequireType[4];
};

struct SPItemEnchantResult
{
	void* pItem;					// SPItem
	SPItemEnchant* pItemEnchant;
};


struct SPItemTimeAdd
{
	int m_iItemID;								//메인 Key (대상 Item ID)
	string m_strDesc;							//설명
	int m_iResult;								//아이템 시간 추가 성공 시에 추가되는 시간 값을 정의. 단위는 시간값(hour)
	
	int m_iMaterialID;							//재료 아이템 ID
	int m_iMaterialNum;							//재료 갯수
	SPItemRequire m_Require[MAX_ITEM_OPTION];	//Require
	string m_strErr;							//에러 메시지

	SPItemTimeAdd() {
		Clear();
	}

	~SPItemTimeAdd() {
	}

	void Clear() {		
		m_iItemID			= 0;
		m_iResult			= 0;
		m_iMaterialID		= 0;
		m_iMaterialNum		= 0;

		for(int i = 0; i < MAX_ITEM_OPTION; i++) {
			m_Require[i].m_iID = 0;
			m_Require[i].m_iType = 0;
			m_Require[i].m_iValue1 = 0;
			m_Require[i].m_iValue2 = 0;
		}

		m_strDesc.clear();
		m_strErr.clear();
	}
};
//뼙緞�薨�
struct SPPhantomLevelUp
{
	
};
//////////////////////////////////////////////////////////////////////////
//
//	ITEM UPGRADE
//	AJJIYA [5/19/2006]
//

enum ITEM_UPGRADE_WHENFAILS
{
	ITEM_UPGRADE_WHENFAILS_NULL			=	0x0000			,	//	오류값
	ITEM_UPGRADE_WHENFAILS_ORIGINAL		=	0x0001			,	//	원본 파괴
	ITEM_UPGRADE_WHENFAILS_MATERIAL		=	0x0002			,	//	재료 파괴
	ITEM_UPGRADE_WHENFAILS_BOTH			=	0x000F			,	//	모두 파괴
};

enum ITEM_UPGRADE_RARE
{
	ITEM_UPGRADE_RARE_CHEAP				=	1				,	//	LDT 값 1
	ITEM_UPGRADE_RARE_NORMAL								,	//	LDT 값 2
	ITEM_UPGRADE_RARE_ABNOMAL								,	//	LDT 값 3
	ITEM_UPGRADE_RARE_RARE									,	//	LDT 값 4
	ITEM_UPGRADE_RARE_LEGEND								,	//	LDT 값 5
};

enum ITEM_UPGRADE_TYPE
{
	ITEM_UPGRADE_TYPE_NULL				=	0				,
	ITEM_UPGRADE_TYPE_WEAPON								,	//	장비
	ITEM_UPGRADE_TYPE_PET									,	//	펫
};

struct SPItemUpgradeMaterial
{
	int									m_iID;				//	재료 아이템 아이디
	int									m_iCount;			//	재료 아이템 갯수
	ITEM_UPGRADE_RARE					m_eRare;			//	재료 아이템 레어도
};

#define ITEM_UPGRADE_KEY_SECTION		100					//	아이템 아이디와 레어도를 분리하는 값
#define MAX_ITEM_UPGRADE_MATERIAL		4
#define MAX_ITEM_UPGRADE_REQUIRE		4

struct SPItemUpgrade
{
	int									m_iLDTItemID;			//	ITEM_UPGRADE.LDT의 ItemID = No.
	std::string							m_strDesc;				//	설명
	int									m_iResult;				//	성공시 업그레이드 되는 아이템 ID
	ITEM_UPGRADE_WHENFAILS				m_eWhenFails;			//	실패 했을때의 해야 하는 작업
	int									m_iItemID_Original;		//	원본 아이템 ID
	ITEM_UPGRADE_RARE					m_eRare_Original;		//	원본 아이템의 레어도
	int									m_iProbabilityModifier;	//	확률 보정
	int									m_iProbabilityMin;		//	확률 최소 값
	int									m_iProbabilityMax;		//	확률 최대 값
	ITEM_UPGRADE_TYPE					m_eType;				//	아이템 타입

	SPItemUpgradeMaterial				m_stMaterial	[ MAX_ITEM_UPGRADE_MATERIAL ];	//	재료 ( 아이디 , 갯수 , 레어도 )
	SPItemRequire						m_stRequireType	[ MAX_ITEM_UPGRADE_REQUIRE ];	//	필요 조건
};


enum ITEM_SET_OPTION_TYPE {
	ITEM_SET_OPTION_TYPE_NULL,
	ITEM_SET_OPTION_TYPE_STAT,				/**< 효과가 Stat으로 붙는 경우 <br> SPEffectValue 로 사용 <br> */
	ITEM_SET_OPTION_TYPE_EFFECT,			/**< 효과가 Effect로 붙는 경우 <br> int m_iEffectID 로 사용 <br> */
	ITEM_SET_OPTION_TYPE_MAX,
};


/**
* Item Set  에 사용될 효과 <br>    
*/
struct SPSetEffect {
	ITEM_SET_OPTION_TYPE	m_iType;			/**< ITEM_SET_OPTION_TYPE 참고 <br> */
	SPEffectValue			m_stEffect;			/**< m_iType이 ITEM_SET_OPTION_TYPE_STAT 인 경우 <br> */
	int						m_iEffectID;		/**< m_iType이 ITEM_SET_OPTION_TYPE_EFFECT 인 경우 <br> */

	SPSetEffect() {
		Clear();
	}

	~SPSetEffect() {
		Clear();
	}

	void Clear() {
		m_iType			= ITEM_SET_OPTION_TYPE_NULL;
		ZeroMemory(&m_stEffect, sizeof(SPEffectValue));
		m_iEffectID		= 0;
	}

	void SetEffect(ITEM_SET_OPTION_TYPE iType, int iStat, int iValue = 0) {		
		if(iType == ITEM_SET_OPTION_TYPE_STAT) {
			m_iType = iType;
			m_stEffect.Effect = (ITEM_EQUIP_EFFECT)iStat;
			m_stEffect.iValue = iValue;
			m_iEffectID = 0;
		}
		else if(iType == ITEM_SET_OPTION_TYPE_EFFECT) {
			m_iType = iType;
			ZeroMemory(&m_stEffect, sizeof(SPEffectValue));
			m_iEffectID = iStat;
		}
	}

	ITEM_SET_OPTION_TYPE GetType() {
		return m_iType;
	}
};

/**
 * Item Set <br>
*/
struct SPItemSet {
	int					m_iSetID;						/**< Set ID <br> */
	std::string			m_strName;						/**< Set Name <br> */
	int					m_iMaxPieces;					/**< Set의 맥스 <br> */
	int					m_iCurrentPieces;				/**< 현재 착용하고 있는 갯수 <br> GetSetItemEffectB() 함수 사용시 설정 <br> */
	bool				m_bFull;						/**< 현재 착용 갯수가 맥스와 같은지를 저장 <br> */
	SPSetEffect			m_stSetEffect[ITEM_SET_A_MAX];	/**< 갯수별 적용 효과 내용 <br> */
	SPSetEffect			m_stFullSetEffect[ITEM_SET_FUUL_OPTION]; /**< 풀셋시 효과 내용 <br> */


	SPItemSet() {
		Clear();
	}

	~SPItemSet() {
		Clear();
	}

	void Clear() {
		m_iSetID		= 0;
		m_strName.clear();
		m_iMaxPieces	= 0;
		m_iCurrentPieces = 0;
		m_bFull			= false;
		
		int i = 0;
		for(i = 0; i < ITEM_SET_A_MAX; i++) {
			m_stSetEffect[i].Clear();
		}
		
		for(i = 0; i < ITEM_SET_FUUL_OPTION; i++) {
			m_stFullSetEffect[i].Clear();
		}
	}

	SPSetEffect*	GetEffect(int iIndex) {
		if(iIndex < 0 || iIndex >= ITEM_SET_A_MAX)
			return NULL;

		return &m_stSetEffect[iIndex];
	}

	SPSetEffect*	GetFullEffect(int iIndex) {
		if(iIndex < 0 || iIndex >= ITEM_SET_FUUL_OPTION)
			return NULL;

		return &m_stFullSetEffect[iIndex];
	}

	void	SetCurrentPieces(int iTotalCount) {
		m_iCurrentPieces = iTotalCount;
		m_bFull = false;
		if(m_iCurrentPieces == m_iMaxPieces)
			m_bFull = true;
	}
};

#define MAX_ITEM_MIX_MATERIAL				5
#define MAX_ITEM_MIX_REQUIRE				4
#define MAX_ITEM_MIX_RARE_PROBABILITY		5

/**
* Item Mixt <br>
*/
struct SPItemMix
{
	int									m_iItemMixID;											//	Ldt ItemID = No.
	std::string							m_strName;												//	이름
	int									m_iResult;												//	결과 아이템 아이디
	int									m_iResultCount;											//	결과 아이템 갯수
	int									m_iWhenFails;											//	실패 했을때 생성되는 아이템
	int									m_iWhenFailsCount;										//	실패 아이템 갯수
	int									m_iSkillID;												//	링크된 스킬 아이디
	int									m_iCategory;											//	카테고리
	SPItemRequire						m_stRequireType	[ MAX_ITEM_MIX_REQUIRE ];				//	필요 조건 4개
	SPItemUpgradeMaterial				m_stMaterial	[ MAX_ITEM_MIX_MATERIAL ];				//	재료 ( 아이디 , 갯수 , 레어도 )

	int									m_iOptMin;												//	내구도 최소 값 보정
	int									m_iOptMax;												//	내구도 최대 값 보정
	int									m_iSuccess_Probability;									//	성공 확률 보정
	int									m_iRare_Probability[ MAX_ITEM_MIX_RARE_PROBABILITY ];	//	옵션 발생 확률
	int									m_iUpDex_Success_Min;									//	숙련도 성공 최소 값
	int									m_iUpDex_Success_Max;									//	숙련도 성공 최대 값
	int									m_iUpDex_Fail_Min;										//	숙련도 실패 최소 값
	int									m_iUpDex_Fail_Max;										//	숙련도 실패 최대 값
	int									m_iBase_Lv;												//	베이스 레벨
	int									m_iSuccess_Probability_Min;								//	최소 성공 확률
	int									m_iSuccess_Probability_Max;								//	최대 성공 확률
	int									m_iSuccess_Probability_Revision;						//	성공 확률 보정
};

struct SPCashItemMiss
{
	int   iItemID;
	int   iCashItemID;
	int   iCategory;
	int   iCategorySub1;
	int   iCategorySub2;
};

struct SPCashItemSultlist
{
	int   iCashItem[10];
};

struct SPCashEffectItem
{
	int     iItemID;
	int     iCashNum;
	int     iEffectID[4];
	string  strName;

	SPCashEffectItem()
	{
		iItemID = 0;
		iCashNum = 0;
		for (int i=0; i<4; i++)
			iEffectID[i] = 0;
		strName = "";
	}
};

