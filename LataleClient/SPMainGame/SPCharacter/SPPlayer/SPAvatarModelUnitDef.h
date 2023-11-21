// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPAvatarModelUnitDef
// Comment     : 
// Creation    : metalgeni 2004-11-02 오전 10:26:19
//***************************************************************************

#pragma once

#include <map>
#include <vector>
using namespace std;

enum PART_LAYER;
enum EQUIP_CLASS {
	EQCLASS_INVAILD	= -1,
	EQCLASS_NULL	= 0,
	EQCLASS_0001	=	1,   //테스트용
	
	EQCLASS_FIRST	=   101		,
	
	EQCLASS_0101	=	101		,	//토시 없는 장갑	장갑
	EQCLASS_0102	=	102		,	//토시 있는 장갑	
	EQCLASS_0103	=	103		,	//팔찌	
	EQCLASS_0201	=	201		,	//소매 없고 망토 없는 상의 (공용)	상의
	EQCLASS_0202	=	202		,	//소매 있고 망토 없는 상의 (공용)	
	EQCLASS_0203	=	203		,	//소매 없고 망토 있는 상의 (공용)	
	EQCLASS_0204	=	204		,	//소매 있고 망토 있는 상의 (공용)	
	EQCLASS_0205	=	205		,	//소매 없고 망토 없는 상의 (남자)	
	EQCLASS_0206	=	206		,	//소매 있고 망토 없는 상의 (남자)	
	EQCLASS_0207	=	207		,	//소매 없고 망토 있는 상의 (남자)	
	EQCLASS_0208	=	208		,	//소매 있고 망토 있는 상의 (남자)	
	EQCLASS_0209	=	209		,	//소매 없고 망토 없는 상의 (여자)	
	EQCLASS_0210	=	210		,	//소매 있고 망토 없는 상의 (여자)	
	EQCLASS_0211	=	211		,	//소매 없고 망토 있는 상의 (여자)	
	EQCLASS_0212	=	212		,	//소매 있고 망토 있는 상의 (여자)	
	EQCLASS_0301	=	301		,	//바지 하의 (공용)	하의
	EQCLASS_0302	=	302		,	//바지 하의 (남자)	
	EQCLASS_0303	=	303		,	//바지 하의 (여자)	
	EQCLASS_0304	=	304		,	//치마 하의 (여자)	
	EQCLASS_0401	=	401		,	//망토	망토
	EQCLASS_0501	=	501		,	//상의 없고 소매 없고 망토 없고 바지 한벌옷 (남자 수영복)	한벌옷
	EQCLASS_0502	=	502		,	//상의 소매 없고 망토 없고 바지 한벌옷 (공용)	
	EQCLASS_0503	=	503		,	//상의 소매 있고 망토 없고 바지 한벌옷 (공용)	
	EQCLASS_0504	=	504		,	//상의 소매 없고 망토 있고 바지 한벌옷 (공용)	
	EQCLASS_0505	=	505		,	//상의 소매 있고 망토 있고 바지 한벌옷 (공용)	
	EQCLASS_0506	=	506		,	//상의 소매 없고 망토 없고 바지 한벌옷 (남자)	
	EQCLASS_0507	=	507		,	//상의 소매 있고 망토 없고 바지 한벌옷 (남자)	
	EQCLASS_0508	=	508		,	//상의 소매 없고 망토 있고 바지 한벌옷 (남자)	
	EQCLASS_0509	=	509		,	//상의 소매 있고 망토 있고 바지 한벌옷 (남자)	
	EQCLASS_0510	=	510		,	//상의 소매 없고 망토 없고 바지 한벌옷 (여자)	
	EQCLASS_0511	=	511		,	//상의 소매 있고 망토 없고 바지 한벌옷 (여자)	
	EQCLASS_0512	=	512		,	//상의 소매 없고 망토 있고 바지 한벌옷 (여자)	
	EQCLASS_0513	=	513		,	//상의 소매 있고 망토 있고 바지 한벌옷 (여자)	
	EQCLASS_0514	=	514		,	//상의 소매 없고 망토 없고 치마 한벌옷 (여자)	
	EQCLASS_0515	=	515		,	//상의 소매 있고 망토 없고 치마 한벌옷 (여자)	
	EQCLASS_0516	=	516		,	//상의 소매 없고 망토 있고 치마 한벌옷 (여자)	
	EQCLASS_0517	=	517		,	//상의 소매 있고 망토 있고 치마 한벌옷 (여자)	
	EQCLASS_0601	=	601		,	//스타킹	스타킹
	EQCLASS_0701	=	701		,	//목 없는 신발	신발
	EQCLASS_0702	=	702		,	//목 있는 신발	
	EQCLASS_0703	=	703		,	//발찌	
	EQCLASS_0801	=	801		,	//너클	무기 방패장비
	EQCLASS_0802	=	802		,	//단검	
	EQCLASS_0803	=	803		,	//검	
	EQCLASS_0804	=	804		,	//양손검	
	EQCLASS_0805	=	805		,	//도	
	EQCLASS_0806	=	806		,	//이도	
	EQCLASS_0807	=	807		,	//둔기	
	EQCLASS_0808	=	808		,	//창	
	EQCLASS_0809	=	809		,	//활	
	EQCLASS_0810	=	810		,	//석궁	
	EQCLASS_0811	=	811		,	//방패
	EQCLASS_0812	=	812		,	//지팡이
	//  [8/10/2007 metalgeni] 무기 카테고리 추가
	EQCLASS_0813	=	813		,	//기타(악기)
	EQCLASS_0814	=	814		,	//쌍권총

	EQCLASS_0901	=	901		,	//윗머리를 덮는 앞머리 장식	머리장식
	EQCLASS_0902	=	902		,	//윗머리를 덮지 않는 앞머리 장식	
	EQCLASS_0903	=	903		,	//뒷머리 장식	
	EQCLASS_0904	=	904		,	//윗머리를 덮는 앞,뒷 머리 장식	
	EQCLASS_0905	=	905		,	//윗머리를 덮지 않는 앞,뒷 머리 장식	
	EQCLASS_0906	=	906		,	//머리 전체를 덮는 헬멧	
	EQCLASS_1001	=	1001	,	//안경	안경
	EQCLASS_1101	=	1101	,	//귀걸이	귀걸이
	EQCLASS_1201	=	1201	,	//얼굴 장식	얼굴 장식
	EQCLASS_1301	=	1301	,	//머리카락 (남자)	머리카락
	EQCLASS_1302	=	1302	,	//머리카락 (여자)	
	EQCLASS_1401	=	1401	,	//얼굴 표정 (남자)	얼굴 표정
	EQCLASS_1402	=	1402	,	//얼굴 표정 (여자)	
	EQCLASS_1501	=	1501	,	//피부색	피부색
	EQCLASS_1601	=	1601	,	//배경 효과	배경효과
	EQCLASS_1701	=	1701	,	//소모형, 기능성 아이템	아이템
	EQCLASS_1801	=	1801	,	//상점 매각용 아이템	
	EQCLASS_1901	=	1901	,	//이벤트 중요 아이템	
	EQCLASS_2001	=	2001	,	//하우징 아이템	

	EQCLASS_LAST	=   2002	
};

enum EQUIP_ID {
	EQ_INVAILD	= -1,
	EQ_NULL		= 0,

	EQ_FIRST	= 1,

	EQ_HAIR_DRESS	= 1,		// 머리장식 ( 모자 )
	EQ_GOGGLE		= 2,		// 안경
	EQ_EARRING		= 3,		// 귀걸이
	EQ_BLOUSE		= 4,		// 상의
	EQ_PANTS		= 5,		// 하의
	EQ_CLOAK		= 6,		// 망토
	EQ_GLOVES		= 7,		// 장갑
	EQ_SHOES		= 8,		// 신발
	EQ_STOCKINGS	= 9,		// 스타킹
	EQ_MAKEUP		= 10,		// 화장 (얼굴장식)

	EQ_STONE		= 11,		// 정령석
	EQ_RING			= 12,		// 반지

	EQ_WEAPON1		= 13,		// 무기1
	EQ_BUCKLER1		= 14,		// 방패1
	EQ_WEAPON2		= 15,		// 무기2
	EQ_BUCKLER2		= 16,		// 방패2	

	EQ_HAIR			= 17,		// 머리
	EQ_EXPRESSION	= 18,		// 표정
	EQ_SKIN			= 19,		// 피부색깔

	EQ_ONE_PIECE	= 20,		// 한벌옷

	EQ_END,

	//0	null
	//(피규어, 패션 공용)
	//1	머리장식
	//2	안경
	//3	귀걸이
	//4	상의
	//5	하의
	//6	망토
	//7	장갑
	//8	신발
	//9	스타킹
	//10	얼굴장식
	//11	정령석
	//12	반지
	//13	한손무기
	//13	양손무기
	//14	방패
	//4	한벌옷
	//(소비형 디폴트 슬롯)
	//1	속옷상의
	//2	속옷하의
	//3	머리카락
	//4	얼굴표정
	//5	피부색
	//(펫 장비 슬롯)
	//1	머리
	//2	몸


};


#ifndef _LATALE_SERVER

// 아이템 적용시 파트교환
enum CHANGE_TYPE {
	CTYPE_INVAILD	= -1,
	CTYPE_O,		// 레이어사용					(O)
	CTYPE_X,		// 레이어사용안함				(X)
	CTYPE_PC,		// 다른애니메이션파트로 교환	(Part Change)
	CTYPE_NC,		// 영향받지 않음				(No Change)
	CTYPE_DC		// 초기값으로 변경				(Default Change)
};

//[xialin 2008/07/29] add
enum LOBBY_AVATAT {
	LOBBY_NULL = 0,
	LOBBY_CASH,
	LOBBY_INFO,
};

struct SPEquipLayerPool {
	PART_LAYER	PartLayer;
	EQUIP_CLASS	EquipClass;
	int			iItemID;
	int			iItemColor;
	SPEquipLayerPool();
};


// 한번 셋팅해서 데이타로 쓸 클래스
struct SPEquipLayerSet {
	PART_LAYER	PartLayer;
	CHANGE_TYPE	ChangeType;
};

struct SPEquipPart {
	EQUIP_ID	EquipID;
	int			iNOEquipLayer;
	std::vector<PART_LAYER>	vPartLayer;
	std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >	mvLayerEquipSet;

	SPEquipPart()
	: EquipID(EQ_NULL)
	, iNOEquipLayer(0)
	{}
};

struct SPEquipRestore {
	PART_LAYER PartLayer;
	std::vector<EQUIP_ID> vRestoreEquipID;
};


extern SPEquipPart g_aEquipPart[EQ_END];
void InitEquipPart();


extern SPEquipRestore g_aEquipRestore[PL_LAST];
void InitEquipRestore();



bool IsDefaultEquip(EQUIP_ID EquipID);


#endif



















