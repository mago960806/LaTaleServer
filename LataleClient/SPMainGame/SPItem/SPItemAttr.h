// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPItemAttr
// Comment     : 
// Creation    : metalgeni 2004-11-05 ¿ÀÀü 10:26:19
//***************************************************************************

#pragma once

#include <string>

using namespace std;

enum ITEM_RARE_LIMIT
{
	ITEM_RARE_NORMAL	= 1,	//³ë¸Ö
	ITEM_RARE_MAGIC		= 2,	//¸ÅÁ÷
	ITEM_RARE_RARE		= 3,	//·¹¾î
	ITEM_RARE_ADD1 		= 4,	//Ãß°¡1
	ITEM_RARE_ADD2		= 5,	//Ãß°¡2
};								

enum ITEM_TYPE
{
	ITEM_TYPE_NULL		= 0,
	ITEM_TYPE_FIGURE	= 1,	// Àåºñ
	ITEM_TYPE_CONSUME	= 2,	// ¼Òºñ
	ITEM_TYPE_ETC		= 3,	// ±âÅ¸
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

	IS_EQUIP_BATTLE		= 6, // ¾ÆÀÌÅÛ Á¾·ù¸¸ ÀÇ¹ÌÇß¾ú´Âµ¥ ¿µÇö¾¾°¡ À§Ä¡±îÁö ¾²µµ·Ï ÇßÀ½..
	IS_EQUIP_FASHION	= 7,
	//IS_QUEST,
};

/**
 * ¾ÆÀÌÅÛ À¯Áö½Ã°£ Å¸ÀÔ <br>
*/
enum ITEM_DURATION {
	ITEM_DURATION_NULL			= 0,	//ÀÏ¹Ý
	ITEM_DURATION_AMOUNT		= 1,	//Á¤¾× ½Ã°£Á¦
	ITEM_DURATION_QUANTITY		= 2,	//Á¤·® ½Ã°£Á¦
};



enum ITEM_SUBTYPE
{
	ITEM_SUBTYPE_NULL = 0,
	//(Àåºñ)
	ITEM_SUBTYPE_EQUIP_EQUIP	=	1,	//Àåºñ
	ITEM_SUBTYPE_EQUIP_FASHION	=	2,	//ÆÐ¼Ç
	ITEM_SUBTYPE_EQUIP_EFFECT	=	3,
	//(¼Òºñ)
	ITEM_SUBTYPE_WASTE_NORMAL	=	1, 	//ÀÏ¹Ý
	ITEM_SUBTYPE_WASTE_ENCHANT	=	2,	//ÀÎÃ¦Æ®
	ITEM_SUBTYPE_WASTE_DEFAULT	=	3,	//µðÆúÆ®
	ITEM_SUBTYPE_WASTE_MSGBOARD	=	4,	//[xialin 2008/09/11]add
	ITEM_SUBTYPE_WASTE_SEARCH	=	5,  //ÕÒÈËµÀ¾ß
	ITEM_SUBTYPE_WASTE_FOLLOW	=	6,	//¸úËæµÀ¾ß
	ITEM_SUBTYPE_WASTE_SNATCH	=	7,	//×¥ÈËµÀ¾ß

	//(Æê)
	ITEM_SUBTYPE_PET_EQUIP		=	1,	//ÆêÀåºñ
	ITEM_SUBTYPE_PET_WASTE		= 	2,	//Æê¼Òºñ
	//(ÀÌº¥Æ®)
	ITEM_SUBTYPE_EVENT_QUEST	=	1,	//ÀÌº¥Æ®Äù½ºÆ®
	ITEM_SUBTYPE_EVENT_GAMBLE	=	2,	//ÀÌº¥Æ®°×ºí
};


enum ITEM_EQUIPPOS
{
	ITEM_EQUIPPOS_NULL = 0,		//	null
	//(ÇÇ±Ô¾î, ÆÐ¼Ç °ø¿ë)
	ITEM_EQUIPPOS_HAIRDRESS 		= 1,		//	¸Ó¸®Àå½Ä
	ITEM_EQUIPPOS_GOGGLE			= 2,		//	¾È°æ
	ITEM_EQUIPPOS_EARING			= 3,		//	±Í°ÉÀÌ
	ITEM_EQUIPPOS_SHIRTS			= 4,		//	»óÀÇ
	ITEM_EQUIPPOS_PANTS				= 5,		//	ÇÏÀÇ
	ITEM_EQUIPPOS_CLOAK				= 6,		//	¸ÁÅä
	ITEM_EQUIPPOS_GLOVE				= 7,		//	Àå°©
	ITEM_EQUIPPOS_SHOES				= 8,		//	½Å¹ß
	ITEM_EQUIPPOS_STOCKING			= 9	,		//	½ºÅ¸Å·
	ITEM_EQUIPPOS_MAKEUP			= 10,		//	¾ó±¼Àå½Ä
	ITEM_EQUIPPOS_STONE				= 11,		//	Á¤·É¼®
	ITEM_EQUIPPOS_RING				= 12,		//	¹ÝÁö
	ITEM_EQUIPPOS_ONEHAND_WEAPON	= 13,		//	ÇÑ¼Õ¹«±â
	ITEM_EQUIPPOS_TWOHAND_WEAPON	= 13,		//	¾ç¼Õ¹«±â
	ITEM_EQUIPPOS_SHIELD			= 14,		//	¹æÆÐ
	ITEM_EQUIPPOS_ONEPIECE			= 4	,		//	ÇÑ¹ú¿Ê	
	ITEM_EQUIPPOS_DEFAULT_SHORTS	= 1	,		//	¼Ó¿Ê»óÀÇ
	ITEM_EQUIPPOS_DEFAULT_PANTS		= 2	,		//	¼Ó¿ÊÇÏÀÇ
	ITEM_EQUIPPOS_HAIR				= 3	,		//	¸Ó¸®Ä«¶ô
	ITEM_EQUIPPOS_EXPRESSION 		= 4	,		//	¾ó±¼Ç¥Á¤
	ITEM_EQUIPPOS_SKIN				= 5	,		//	ÇÇºÎ»ö	

	ITEM_EQUIPPOS_PET_HEAD			= 1	,		//	¸Ó¸®		Æê
	ITEM_EQUIPPOS_PET_BODY			= 2	,		//	¸ö			Æê

	ITEM_EQUIPPOS_ONEHAND_WEAPON2	= 15,		//	ÇÑ¼Õ¹«±â
	ITEM_EQUIPPOS_TWOHAND_WEAPON2	= 15,		//	¾ç¼Õ¹«±â
	ITEM_EQUIPPOS_SHIELD2			= 16,		//	¹æÆÐ
};										



enum ITEM_EQUIP_KIND {
	ITEM_EQUIP_KIND_NULL			= 0,	//null
	ITEM_EQUIP_KIND_KNUCKLE			= 1,	//³ÊÅ¬
	ITEM_EQUIP_KIND_DAGGER			= 2,	//´Ü°Ë
	ITEM_EQUIP_KIND_SWORD			= 3,	//°Ë
	ITEM_EQUIP_KIND_TWOHANDSWORD	= 4,	//¾ç¼Õ°Ë
	ITEM_EQUIP_KIND_SABER			= 5,	//µµ
	ITEM_EQUIP_KIND_IDORYU			= 6,	//ÀÌµµ
	ITEM_EQUIP_KIND_CLUB			= 7,	//µÐ±â
	ITEM_EQUIP_KIND_SPEAR			= 8,	//Ã¢
	ITEM_EQUIP_KIND_BOW				= 9,	//È°
	ITEM_EQUIP_KIND_CROSSBOW		= 10,	//¼®±Ã
};


enum ITEM_EQUIP_EFFECT
{
	ITEM_EQUIP_EFFECT_NULL = 0,		//	null	

	//(Pc Status. È¿°ú ¹× Á¦ÇÑ ¼³Á¤¿¡ °øÅëÀ¸·Î ¾²ÀÓ)	
	ITEM_EQUIP_EFFECT_CLASS					=	1 ,				//Ä³¸¯ÅÍ À¯Çü
	ITEM_EQUIP_EFFECT_GENDER				=	2 ,				//¼ºº°
	ITEM_EQUIP_EFFECT_LV					=	3 ,				//ÇöÀç ·¹º§
	ITEM_EQUIP_EFFECT_MONEY					=	4 ,				//ÇöÀç µ·
	ITEM_EQUIP_EFFECT_EXP					=	5 ,				//ÇöÀç °æÇèÄ¡
	ITEM_EQUIP_EFFECT_HP					=	6 ,				//ÇöÀç HP
	ITEM_EQUIP_EFFECT_SP					=	7 ,				//ÇöÀç SP
	ITEM_EQUIP_EFFECT_CURR_FAME				=	8 ,				//¸í¼º ÃÖÁ¾°ª
	ITEM_EQUIP_EFFECT_BASE_FAME				=	9 ,				//±âº» ¸í¼º
	ITEM_EQUIP_EFFECT_ADD_FAME				=	10,				//¸í¼º + 
	ITEM_EQUIP_EFFECT_RATIO_FAME			=	11,				//¸í¼º %
	ITEM_EQUIP_EFFECT_CURR_ATK				=	12,				//ÃÖÁ¾ °ø°Ý·Â
	ITEM_EQUIP_EFFECT_BASE_ATK				=	13,				//±âº» °ø°Ý·Â
	ITEM_EQUIP_EFFECT_ADD_ATK				=	14,				//°ø°Ý·Â +
	ITEM_EQUIP_EFFECT_RATIO_ATK				=	15,				//°ø°Ý·Â %
	ITEM_EQUIP_EFFECT_CURR_LUK				=	16,				//ÃÖÁ¾ Çà¿î
	ITEM_EQUIP_EFFECT_BASE_LUK				=	17,				//±âº» Çà¿î
	ITEM_EQUIP_EFFECT_ADD_LUK				=	18,				//Çà¿î +
	ITEM_EQUIP_EFFECT_RATIO_LUK				=	19,				//Çà¿î %
	ITEM_EQUIP_EFFECT_CURR_ELE				=	20,				//ÃÖÁ¾ ¸¶¹ý·Â
	ITEM_EQUIP_EFFECT_BASE_ELE				=	21,				//±âº» ¸¶¹ý·Â
	ITEM_EQUIP_EFFECT_ADD_ELE				=	22,				//¸¶¹ý·Â +
	ITEM_EQUIP_EFFECT_RATIO_ELE				=	23,				//¸¶¹ý·Â %
	ITEM_EQUIP_EFFECT_CURR_DEF				=	24,				//ÃÖÁ¾ ¹æ¾î·Â
	ITEM_EQUIP_EFFECT_BASE_DEF				=	25,				//±âº» ¹æ¾î·Â
	ITEM_EQUIP_EFFECT_ADD_DEF				=	26,				//¹æ¾î·Â +
	ITEM_EQUIP_EFFECT_RATIO_DEF				=	27,				//¹æ¾î·Â %
	ITEM_EQUIP_EFFECT_CURR_MHP				=	28,				//ÃÖÁ¾ ÃÖ´ë Hp
	ITEM_EQUIP_EFFECT_BASE_MHP				=	29,				//±âº» ÃÖ´ë HP
	ITEM_EQUIP_EFFECT_ADD_MHP				=	30,				//ÃÖ´ë HP +
	ITEM_EQUIP_EFFECT_RATIO_MHP				=	31,				//ÃÖ´ë HP %
	ITEM_EQUIP_EFFECT_CURR_MSP				=	32,				//ÃÖÁ¾ ÃÖ´ë SP
	ITEM_EQUIP_EFFECT_BASE_MSP				=	33,				//±âº» ÃÖ´ë SP
	ITEM_EQUIP_EFFECT_ADD_MSP				=	34,				//ÃÖ´ë SP +
	ITEM_EQUIP_EFFECT_RATIO_MSP				=	35,				//ÃÖ´ë SP %
	ITEM_EQUIP_EFFECT_CURR_WATER			=	36,				//ÃÖÁ¾ ¼ö¼Ó¼º
	ITEM_EQUIP_EFFECT_BASE_WATER			=	37,				//±âº» ¼ö¼Ó¼º
	ITEM_EQUIP_EFFECT_ADD_WATER				=	38,				//¼ö¼Ó¼º +
	ITEM_EQUIP_EFFECT_RATIO_WATER			=	39,				//¼ö¼Ó¼º %
	ITEM_EQUIP_EFFECT_CURR_WIND				=	40,				//ÃÖÁ¾ Ç³¼Ó¼º
	ITEM_EQUIP_EFFECT_ADD_WIND				=	41,				//±âº» Ç³¼Ó¼º
	ITEM_EQUIP_EFFECT_BASE_WIND				=	42,				//Ç³¼Ó¼º +
	ITEM_EQUIP_EFFECT_RATIO_WIND			=	43,				//Ç³¼Ó¼º %
	ITEM_EQUIP_EFFECT_CURR_FIRE				=	44,				//ÃÖÁ¾ È­¼Ó¼º
	ITEM_EQUIP_EFFECT_BASE_FIRE				=	45,				//±âº» È­¼Ó¼º
	ITEM_EQUIP_EFFECT_ADD_FIRE				=	46,				//È­¼Ó¼º +
	ITEM_EQUIP_EFFECT_RATIO_FIRE			=	47,				//È­¼Ó¼º %
	ITEM_EQUIP_EFFECT_CURR_EARTH			=	48,				//ÃÖÁ¾ Áö¼Ó¼º
	ITEM_EQUIP_EFFECT_BASE_EARTH			=	49,				//±âº» Áö¼Ó¼º
	ITEM_EQUIP_EFFECT_ADD_EARTH				=	50,				//Áö¼Ó¼º +
	ITEM_EQUIP_EFFECT_RATIO_EARTH			=	51,				//Áö¼Ó¼º %
	ITEM_EQUIP_EFFECT_CURR_RECV_HP			=	52,				//ÃÖÁ¾ È¸º¹ HP
	ITEM_EQUIP_EFFECT_BASE_RECV_HP			=	53,				//±âº» È¸º¹ HP
	ITEM_EQUIP_EFFECT_ADD_RECV_HP			=	54,				//È¸º¹ HP +
	ITEM_EQUIP_EFFECT_RATIO_RECV_HP			=	55,				//È¸º¹ HP %
	ITEM_EQUIP_EFFECT_CURR_RECV_SP			=	56,				//ÃÖÁ¾ È¸º¹ SP
	ITEM_EQUIP_EFFECT_BASE_RECV_SP			=	57,				//±âº» È¸º¹ SP
	ITEM_EQUIP_EFFECT_ADD_RECV_SP			=	58,				//È¸º¹ SP +
	ITEM_EQUIP_EFFECT_RATIO_RECV_SP			=	59,				//È¸º¹ SP %
	ITEM_EQUIP_EFFECT_CURR_HEAVY			=	60,				//ÃÖÁ¾ ¹«±â °­ÇÑ °ø°Ý·Â
	ITEM_EQUIP_EFFECT_BASE_HEAVY			=	61,				//±âº» ¹«±â °­ÇÑ °ø°Ý·Â
	ITEM_EQUIP_EFFECT_ADD_HEAVY				=	62,				//¹«±â °­ÇÑ °ø°Ý·Â +
	ITEM_EQUIP_EFFECT_RATIO_HEAVY			=	63,				//¹«±â °­ÇÑ °ø°Ý·Â %
	ITEM_EQUIP_EFFECT_CURR_NORMAL			=	64,				//ÃÖÁ¾ ¹«±â ÀÏ¹Ý °ø°Ý·Â
	ITEM_EQUIP_EFFECT_BASE_NORMAL			=	65,				//±âº» ¹«±â ÀÏ¹Ý °ø°Ý·Â
	ITEM_EQUIP_EFFECT_ADD_NORMAL			=	66,				//¹«±â ÀÏ¹Ý °ø°Ý·Â +
	ITEM_EQUIP_EFFECT_RATIO_NORMAL			=	67,				//¹«±â ÀÏ¹Ý °ø°Ý·Â %
	ITEM_EQUIP_EFFECT_CURR_MP_WATER			=	68,				//ÃÖÁ¾ ¼ö¼Ó¼º °ø°Ý·Â
	ITEM_EQUIP_EFFECT_BASE_MP_WATER			=	69,				//±âº» ¼ö¼Ó¼º °ø°Ý·Â
	ITEM_EQUIP_EFFECT_CURR_MP_WIND			=	70,				//ÃÖÁ¾ Ç³¼Ó¼º °ø°Ý·Â
	ITEM_EQUIP_EFFECT_ADD_MP_WIND			=	71,				//±âº» Ç³¼Ó¼º °ø°Ý·Â
	ITEM_EQUIP_EFFECT_CURR_MP_FIRE			=	72,				//ÃÖÁ¾ È­¼Ó¼º °ø°Ý·Â
	ITEM_EQUIP_EFFECT_BASE_MP_FIRE			=	73,				//±âº» È­¼Ó¼º °ø°Ý·Â
	ITEM_EQUIP_EFFECT_CURR_MP_EARTH			=	74,				//ÃÖÁ¾ Áö¼Ó¼º °ø°Ý·Â
	ITEM_EQUIP_EFFECT_BASE_MP_EARTH			=	75,				//±âº» Áö¼Ó¼º °ø°Ý·Â
	ITEM_EQUIP_EFFECT_ADD_MP_ELEMENTAL		=	76,				//¸¶¹ý °ø°Ý·Â +(°¢ ¼Ó¼º °øÅë)
	ITEM_EQUIP_EFFECT_RATIO_MP_ELEMENTAL	=	77,				//¸¶¹ý °ø°Ý·Â %(°¢ ¼Ó¼º °øÅë)
	ITEM_EQUIP_EFFECT_CURR_AP_HEAVY			=	78,				//ÃÖÁ¾ °­ÇÑ °ø°Ý·Â
	ITEM_EQUIP_EFFECT_BASE_AP_HEAVY			=	79,				//±âº» °­ÇÑ °ø°Ý·Â
	ITEM_EQUIP_EFFECT_CURR_AP_NORMAL		=	80,				//ÃÖÁ¾ ÀÏ¹Ý °ø°Ý·Â
	ITEM_EQUIP_EFFECT_BASE_AP_NORMAL		=	81,				//±âº» ÀÏ¹Ý °ø°Ý·Â
	ITEM_EQUIP_EFFECT_ADD_AP_ATK			=	82,				//¹°¸® °ø°Ý·Â +(Heavy, Normal °øÅë)
	ITEM_EQUIP_EFFECT_RATIO_AP_ATK			=	83,				//¹°¸® °ø°Ý·Â %(Heavy, Normal °øÅë)
																			
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


const int ITEM_SET_A_MAX				 = 11;			/**< 11°¡ÁöÀÇ ¼Â <br> */
const int ITEM_SET_FUUL_OPTION			 = 4;			/**< Ç®¼ÂÀÎ°æ¿ì ¿É¼Ç ÃÖ´ëÄ¡ <br> */

#define MAX_ITEM_OPTION 4
#define MAX_ITEM_EFFECT 4
#define MAX_ITEM_SKILL	4


struct SPItemAttr {
	// m_iCategory, m_iNum, m_iColorID °¡ ÇÕÃÄÀú Item ID¸¦ ¸¸µç´Ù
	int							m_iCategory			;		//Ä«Å×°í¸®
	int							m_iNum				;		//?
	int							m_iColorID			;		//ÄÃ·¯
	
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
	SPEffectValue				m_StatusTypePvp [MAX_ITEM_OPTION];	//PVP¿¡ »ç¿ëµÇ´Â ¾ÆÀÌÅÛ½ºÅÈ
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
	int							m_iEffectID[ MAX_ITEM_EFFECT ];	// È¿°ú ID  AJJIYA [7/21/2006]
	int							m_iOptionType		;
	int							m_iOptionClass		;
	
	ICON_MARK 					m_eIconMark			;	// ¾ÆÀÌÄÜ¿¡ Ãß°¡ Ç¥½Ã

	bool						m_bNameColor		;	// ¿ì¼±¼øÀ§ ÀÌ¸§ ÄÃ·¯
	byte						m_cRed				;
	byte						m_cGreen			;
	byte						m_cBlue				;

	ITEM_DURATION				m_eDuration			;	// ½Ã°£ ¾ÆÀÌÅÛ À¯Áö Å¸ÀÔ
	int							m_iLifeTime			;	// Á¤·®Á¦¾ÆÀÌÅÛÀÇ ½Ã°£

	int							m_iMacroType;

	unsigned int				m_iPetClassID;
	INT64						m_iPetMaxExp;
	int							m_iSkillID[MAX_ITEM_SKILL];
	int							m_iSkillLevel[MAX_ITEM_SKILL];
	int							m_iConsumeSP[MAX_ITEM_SKILL];

	int							m_iSetID;						/**< ¾ÆÀÌÅÛÀÇ Set ID <br> */
	int							m_iQuestID;						// ¾ÆÀÌÅÛÀÇ Äù½ºÆ® ID ÀÌº¥Æ®ÀÏ¶§¸¸ »ç¿ë [10/2/2007 AJJIYA]
	bool						m_bPvp;							/**< Pvp¿¡¼­ÀÇ »ç¿ë °¡´É ¿©ºÎ ÇÊµå <br> */
	int							m_iCashPoint;					//Ä³½ÃÆ÷ÀÎÆ®

	int							m_iItemLevel;					/**< ¾ÆÀÌÅÛ ·¹º§ <br> */
	int							m_iAuctionCategory;				/**< °æ¸Å ±¸ºÐ <br> */
	int							m_iAuctionQuality;				/**< °æ¸Å Ç°Áú <br> */
	int							m_iShopID;						/**< Ä³½Ã¼¥ ID <br> */

	int							m_iCash;						/**< Ä³½Ã °Å·¡ °¡ <br> */
	int							m_iPoint;						/**< Æ÷ÀÎÆ® °Å·¡ °¡ <br> */

	int							m_iNewPet;						//!< 1 for new pet, 0 fo old pet

	bool						m_bWedding;						/**< °áÈ¥ ¾ÆÀÌÅÛÀÎÁö <br> trueÀÎ °æ¿ì °áÈ¥ ¾ÆÀÌÅÛ <br> */  

	SPItemAttr();	
	void	SetNull();
	virtual ~SPItemAttr();
	
	/**
	 * ¿ì¼±¼øÀ§ ¾ÆÀÌÅÛ ÀÌ¸§ ÄÃ·¯¸¦ °¡Á®¿Â´Ù <br> 
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
	int m_iItemID;								//¸ÞÀÎ Key (´ë»ó Item ID)
	string m_strDesc;							//¼³¸í
	int m_iResult;								//¾ÆÀÌÅÛ ½Ã°£ Ãß°¡ ¼º°ø ½Ã¿¡ Ãß°¡µÇ´Â ½Ã°£ °ªÀ» Á¤ÀÇ. ´ÜÀ§´Â ½Ã°£°ª(hour)
	
	int m_iMaterialID;							//Àç·á ¾ÆÀÌÅÛ ID
	int m_iMaterialNum;							//Àç·á °¹¼ö
	SPItemRequire m_Require[MAX_ITEM_OPTION];	//Require
	string m_strErr;							//¿¡·¯ ¸Þ½ÃÁö

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
//»ÃÓ°Éý¼¶
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
	ITEM_UPGRADE_WHENFAILS_NULL			=	0x0000			,	//	¿À·ù°ª
	ITEM_UPGRADE_WHENFAILS_ORIGINAL		=	0x0001			,	//	¿øº» ÆÄ±«
	ITEM_UPGRADE_WHENFAILS_MATERIAL		=	0x0002			,	//	Àç·á ÆÄ±«
	ITEM_UPGRADE_WHENFAILS_BOTH			=	0x000F			,	//	¸ðµÎ ÆÄ±«
};

enum ITEM_UPGRADE_RARE
{
	ITEM_UPGRADE_RARE_CHEAP				=	1				,	//	LDT °ª 1
	ITEM_UPGRADE_RARE_NORMAL								,	//	LDT °ª 2
	ITEM_UPGRADE_RARE_ABNOMAL								,	//	LDT °ª 3
	ITEM_UPGRADE_RARE_RARE									,	//	LDT °ª 4
	ITEM_UPGRADE_RARE_LEGEND								,	//	LDT °ª 5
};

enum ITEM_UPGRADE_TYPE
{
	ITEM_UPGRADE_TYPE_NULL				=	0				,
	ITEM_UPGRADE_TYPE_WEAPON								,	//	Àåºñ
	ITEM_UPGRADE_TYPE_PET									,	//	Æê
};

struct SPItemUpgradeMaterial
{
	int									m_iID;				//	Àç·á ¾ÆÀÌÅÛ ¾ÆÀÌµð
	int									m_iCount;			//	Àç·á ¾ÆÀÌÅÛ °¹¼ö
	ITEM_UPGRADE_RARE					m_eRare;			//	Àç·á ¾ÆÀÌÅÛ ·¹¾îµµ
};

#define ITEM_UPGRADE_KEY_SECTION		100					//	¾ÆÀÌÅÛ ¾ÆÀÌµð¿Í ·¹¾îµµ¸¦ ºÐ¸®ÇÏ´Â °ª
#define MAX_ITEM_UPGRADE_MATERIAL		4
#define MAX_ITEM_UPGRADE_REQUIRE		4

struct SPItemUpgrade
{
	int									m_iLDTItemID;			//	ITEM_UPGRADE.LDTÀÇ ItemID = No.
	std::string							m_strDesc;				//	¼³¸í
	int									m_iResult;				//	¼º°ø½Ã ¾÷±×·¹ÀÌµå µÇ´Â ¾ÆÀÌÅÛ ID
	ITEM_UPGRADE_WHENFAILS				m_eWhenFails;			//	½ÇÆÐ ÇßÀ»¶§ÀÇ ÇØ¾ß ÇÏ´Â ÀÛ¾÷
	int									m_iItemID_Original;		//	¿øº» ¾ÆÀÌÅÛ ID
	ITEM_UPGRADE_RARE					m_eRare_Original;		//	¿øº» ¾ÆÀÌÅÛÀÇ ·¹¾îµµ
	int									m_iProbabilityModifier;	//	È®·ü º¸Á¤
	int									m_iProbabilityMin;		//	È®·ü ÃÖ¼Ò °ª
	int									m_iProbabilityMax;		//	È®·ü ÃÖ´ë °ª
	ITEM_UPGRADE_TYPE					m_eType;				//	¾ÆÀÌÅÛ Å¸ÀÔ

	SPItemUpgradeMaterial				m_stMaterial	[ MAX_ITEM_UPGRADE_MATERIAL ];	//	Àç·á ( ¾ÆÀÌµð , °¹¼ö , ·¹¾îµµ )
	SPItemRequire						m_stRequireType	[ MAX_ITEM_UPGRADE_REQUIRE ];	//	ÇÊ¿ä Á¶°Ç
};


enum ITEM_SET_OPTION_TYPE {
	ITEM_SET_OPTION_TYPE_NULL,
	ITEM_SET_OPTION_TYPE_STAT,				/**< È¿°ú°¡ StatÀ¸·Î ºÙ´Â °æ¿ì <br> SPEffectValue ·Î »ç¿ë <br> */
	ITEM_SET_OPTION_TYPE_EFFECT,			/**< È¿°ú°¡ Effect·Î ºÙ´Â °æ¿ì <br> int m_iEffectID ·Î »ç¿ë <br> */
	ITEM_SET_OPTION_TYPE_MAX,
};


/**
* Item Set  ¿¡ »ç¿ëµÉ È¿°ú <br>    
*/
struct SPSetEffect {
	ITEM_SET_OPTION_TYPE	m_iType;			/**< ITEM_SET_OPTION_TYPE Âü°í <br> */
	SPEffectValue			m_stEffect;			/**< m_iTypeÀÌ ITEM_SET_OPTION_TYPE_STAT ÀÎ °æ¿ì <br> */
	int						m_iEffectID;		/**< m_iTypeÀÌ ITEM_SET_OPTION_TYPE_EFFECT ÀÎ °æ¿ì <br> */

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
	int					m_iMaxPieces;					/**< SetÀÇ ¸Æ½º <br> */
	int					m_iCurrentPieces;				/**< ÇöÀç Âø¿ëÇÏ°í ÀÖ´Â °¹¼ö <br> GetSetItemEffectB() ÇÔ¼ö »ç¿ë½Ã ¼³Á¤ <br> */
	bool				m_bFull;						/**< ÇöÀç Âø¿ë °¹¼ö°¡ ¸Æ½º¿Í °°ÀºÁö¸¦ ÀúÀå <br> */
	SPSetEffect			m_stSetEffect[ITEM_SET_A_MAX];	/**< °¹¼öº° Àû¿ë È¿°ú ³»¿ë <br> */
	SPSetEffect			m_stFullSetEffect[ITEM_SET_FUUL_OPTION]; /**< Ç®¼Â½Ã È¿°ú ³»¿ë <br> */


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
	std::string							m_strName;												//	ÀÌ¸§
	int									m_iResult;												//	°á°ú ¾ÆÀÌÅÛ ¾ÆÀÌµð
	int									m_iResultCount;											//	°á°ú ¾ÆÀÌÅÛ °¹¼ö
	int									m_iWhenFails;											//	½ÇÆÐ ÇßÀ»¶§ »ý¼ºµÇ´Â ¾ÆÀÌÅÛ
	int									m_iWhenFailsCount;										//	½ÇÆÐ ¾ÆÀÌÅÛ °¹¼ö
	int									m_iSkillID;												//	¸µÅ©µÈ ½ºÅ³ ¾ÆÀÌµð
	int									m_iCategory;											//	Ä«Å×°í¸®
	SPItemRequire						m_stRequireType	[ MAX_ITEM_MIX_REQUIRE ];				//	ÇÊ¿ä Á¶°Ç 4°³
	SPItemUpgradeMaterial				m_stMaterial	[ MAX_ITEM_MIX_MATERIAL ];				//	Àç·á ( ¾ÆÀÌµð , °¹¼ö , ·¹¾îµµ )

	int									m_iOptMin;												//	³»±¸µµ ÃÖ¼Ò °ª º¸Á¤
	int									m_iOptMax;												//	³»±¸µµ ÃÖ´ë °ª º¸Á¤
	int									m_iSuccess_Probability;									//	¼º°ø È®·ü º¸Á¤
	int									m_iRare_Probability[ MAX_ITEM_MIX_RARE_PROBABILITY ];	//	¿É¼Ç ¹ß»ý È®·ü
	int									m_iUpDex_Success_Min;									//	¼÷·Ãµµ ¼º°ø ÃÖ¼Ò °ª
	int									m_iUpDex_Success_Max;									//	¼÷·Ãµµ ¼º°ø ÃÖ´ë °ª
	int									m_iUpDex_Fail_Min;										//	¼÷·Ãµµ ½ÇÆÐ ÃÖ¼Ò °ª
	int									m_iUpDex_Fail_Max;										//	¼÷·Ãµµ ½ÇÆÐ ÃÖ´ë °ª
	int									m_iBase_Lv;												//	º£ÀÌ½º ·¹º§
	int									m_iSuccess_Probability_Min;								//	ÃÖ¼Ò ¼º°ø È®·ü
	int									m_iSuccess_Probability_Max;								//	ÃÖ´ë ¼º°ø È®·ü
	int									m_iSuccess_Probability_Revision;						//	¼º°ø È®·ü º¸Á¤
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

