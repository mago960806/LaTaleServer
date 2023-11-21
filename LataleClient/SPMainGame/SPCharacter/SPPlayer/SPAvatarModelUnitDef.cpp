
#include <vector>
#include <map>
#include <hash_map>

#ifdef _LATALE_CLIENT
#include "SPCommon.h"
#else 
#include "SPToolCommon.h"
#endif

#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"

//	WEAPON-OUT			01
//	BLOUSE-ARM-OUT		02
//	GLOVE-OUT			03
//	HAND-OUT			04
//	HAND-ARM-OUT		05
//	ARM-OUT				06
//	PANTS-LEG-OUT		07
//	SHOE-OUT			08
//	FOOT-OUT			09
//	FOOT-LEG-OUT		10
//	LEG-OUT				11
//	BLOUSE				12
//	PANTS				13
//	BODY				14
//	PANTS-LEG-IN		15
//	SHOE-IN				16
//	FOOT-IN				17
//	FOOT-LEG-IN			18
//	LEG-IN				19
//	CAP-FRONT			20
//	GOGGLE				21
//	EAR					22
//	HAIR-FRONT			23
//	HAIR-REAR			24
//	FACE				25
//	MAKEUP				26
//	HEAD				27
//	CAP-REAR			28
//	BLOUSE-ARM-IN		29
//	GLOVE-IN			30
//	HAND-IN				31
//	HAND-ARM-IN			32
//	ARM-IN				33
//	WEAPON-IN			34
//	COAT				35

SPEquipPart g_aEquipPart[EQ_END];
SPEquipRestore g_aEquipRestore[PL_LAST];


SPEquipLayerPool::SPEquipLayerPool() 
:PartLayer		(PL_NULL)
,EquipClass		(EQCLASS_NULL)
,iItemID		(0)
,iItemColor		(0)
{}


bool IsDefaultEquip(EQUIP_ID EquipID)
{	
	switch(EquipID) {
	case EQ_BLOUSE:		
	case EQ_PANTS:		 
	case EQ_HAIR:		
	case EQ_EXPRESSION:	
	case EQ_SKIN:
		return true;
	}

	return false;
}


void InitEquipPart()
{
	static bool bInit = false;
	if(bInit == true) return;
	
	std::vector<SPEquipLayerSet> vEquipLayerSet;
	SPEquipLayerSet TempEquipLayerSet;

	// GLOVES
	g_aEquipPart[EQ_GLOVES].EquipID = EQ_GLOVES;
	g_aEquipPart[EQ_GLOVES].iNOEquipLayer = 4;
	g_aEquipPart[EQ_GLOVES].vPartLayer.push_back(PL_GLOVE_OUT		);
	g_aEquipPart[EQ_GLOVES].vPartLayer.push_back(PL_HAND_ARM_OUT	);
	g_aEquipPart[EQ_GLOVES].vPartLayer.push_back(PL_GLOVE_IN		);
	g_aEquipPart[EQ_GLOVES].vPartLayer.push_back(PL_HAND_ARM_IN		);

	TempEquipLayerSet.PartLayer = PL_GLOVE_OUT		;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_HAND_ARM_OUT	;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_GLOVE_IN		;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_HAND_ARM_IN	;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	g_aEquipPart[EQ_GLOVES].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0101, vEquipLayerSet));
	

	vEquipLayerSet.clear();

	TempEquipLayerSet.PartLayer = PL_GLOVE_OUT		;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_HAND_ARM_OUT	;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_GLOVE_IN		;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_HAND_ARM_IN	;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	g_aEquipPart[EQ_GLOVES].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0102, vEquipLayerSet));


	vEquipLayerSet.clear();

	TempEquipLayerSet.PartLayer = PL_GLOVE_OUT		;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_HAND_ARM_OUT	;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_GLOVE_IN		;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_HAND_ARM_IN	;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	g_aEquipPart[EQ_GLOVES].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0103, vEquipLayerSet));


	//JACKET
	g_aEquipPart[EQ_BLOUSE].EquipID = EQ_BLOUSE;
	g_aEquipPart[EQ_BLOUSE].iNOEquipLayer = 4;
	g_aEquipPart[EQ_BLOUSE].vPartLayer.push_back(PL_BLOUSE_ARM_OUT	);
	g_aEquipPart[EQ_BLOUSE].vPartLayer.push_back(PL_BLOUSE			);
	g_aEquipPart[EQ_BLOUSE].vPartLayer.push_back(PL_BLOUSE_ARM_IN	);
	//g_aEquipPart[EQ_BLOUSE].vPartLayer.push_back(PL_COAT			);

	vEquipLayerSet.clear();

	TempEquipLayerSet.PartLayer = PL_BLOUSE_ARM_OUT	;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_BLOUSE			;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_BLOUSE_ARM_IN	;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	/*TempEquipLayerSet.PartLayer = PL_COAT			;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);*/

	g_aEquipPart[EQ_BLOUSE].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0201, vEquipLayerSet));
	g_aEquipPart[EQ_BLOUSE].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0205, vEquipLayerSet));
	g_aEquipPart[EQ_BLOUSE].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0209, vEquipLayerSet));

	vEquipLayerSet.clear();

	TempEquipLayerSet.PartLayer = PL_BLOUSE_ARM_OUT	;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_BLOUSE			;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_BLOUSE_ARM_IN	;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	/*TempEquipLayerSet.PartLayer = PL_COAT			;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);*/

	g_aEquipPart[EQ_BLOUSE].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0202, vEquipLayerSet));
	g_aEquipPart[EQ_BLOUSE].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0206, vEquipLayerSet));
	g_aEquipPart[EQ_BLOUSE].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0210, vEquipLayerSet));

	vEquipLayerSet.clear();

	TempEquipLayerSet.PartLayer = PL_BLOUSE_ARM_OUT	;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_BLOUSE			;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_BLOUSE_ARM_IN	;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	/*TempEquipLayerSet.PartLayer = PL_COAT			;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);*/

	g_aEquipPart[EQ_BLOUSE].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0203, vEquipLayerSet));
	g_aEquipPart[EQ_BLOUSE].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0207, vEquipLayerSet));
	g_aEquipPart[EQ_BLOUSE].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0211, vEquipLayerSet));

	TempEquipLayerSet.PartLayer = PL_BLOUSE_ARM_OUT	;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_BLOUSE			;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_BLOUSE_ARM_IN	;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	/*TempEquipLayerSet.PartLayer = PL_COAT			;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);*/

	g_aEquipPart[EQ_BLOUSE].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0204, vEquipLayerSet));
	g_aEquipPart[EQ_BLOUSE].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0208, vEquipLayerSet));
	g_aEquipPart[EQ_BLOUSE].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0212, vEquipLayerSet));

	
	//PANTS
	g_aEquipPart[EQ_PANTS].EquipID = EQ_PANTS;
	g_aEquipPart[EQ_PANTS].iNOEquipLayer = 3;
	g_aEquipPart[EQ_PANTS].vPartLayer.push_back(PL_PANTS_LEG_OUT	);
	g_aEquipPart[EQ_PANTS].vPartLayer.push_back(PL_PANTS			);
	g_aEquipPart[EQ_PANTS].vPartLayer.push_back(PL_PANTS_LEG_IN		);	

	vEquipLayerSet.clear();

	TempEquipLayerSet.PartLayer = PL_PANTS_LEG_OUT	;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_PANTS			;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_PANTS_LEG_IN	;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	g_aEquipPart[EQ_PANTS].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0301, vEquipLayerSet));
	g_aEquipPart[EQ_PANTS].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0302, vEquipLayerSet));
	g_aEquipPart[EQ_PANTS].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0303, vEquipLayerSet));

	vEquipLayerSet.clear();

	TempEquipLayerSet.PartLayer = PL_PANTS_LEG_OUT	;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_PANTS			;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_PANTS_LEG_IN	;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	g_aEquipPart[EQ_PANTS].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0304, vEquipLayerSet));

	//CLOAK
	g_aEquipPart[EQ_CLOAK].EquipID = EQ_CLOAK;
	g_aEquipPart[EQ_CLOAK].iNOEquipLayer = 1;
	g_aEquipPart[EQ_CLOAK].vPartLayer.push_back(PL_COAT	);

	vEquipLayerSet.clear();

	TempEquipLayerSet.PartLayer = PL_COAT	;
	TempEquipLayerSet.ChangeType = CTYPE_O	;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	g_aEquipPart[EQ_CLOAK].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0401, vEquipLayerSet));



	//EQ_ONE_PIECE
	g_aEquipPart[EQ_ONE_PIECE].EquipID = EQ_ONE_PIECE;
	g_aEquipPart[EQ_ONE_PIECE].iNOEquipLayer = 7;
	g_aEquipPart[EQ_ONE_PIECE].vPartLayer.push_back(PL_BLOUSE_ARM_OUT	);
	g_aEquipPart[EQ_ONE_PIECE].vPartLayer.push_back(PL_PANTS_LEG_OUT	);
	g_aEquipPart[EQ_ONE_PIECE].vPartLayer.push_back(PL_BLOUSE			);
	g_aEquipPart[EQ_ONE_PIECE].vPartLayer.push_back(PL_PANTS			);
	g_aEquipPart[EQ_ONE_PIECE].vPartLayer.push_back(PL_PANTS_LEG_IN		);
	g_aEquipPart[EQ_ONE_PIECE].vPartLayer.push_back(PL_BLOUSE_ARM_IN	);
	g_aEquipPart[EQ_ONE_PIECE].vPartLayer.push_back(PL_COAT				);

	vEquipLayerSet.clear();

	TempEquipLayerSet.PartLayer = PL_BLOUSE_ARM_OUT	;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_PANTS_LEG_OUT	;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_BLOUSE			;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_PANTS			;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_PANTS_LEG_IN	;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_BLOUSE_ARM_IN	;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_COAT			;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	g_aEquipPart[EQ_ONE_PIECE].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0501, vEquipLayerSet));

	vEquipLayerSet.clear();

	TempEquipLayerSet.PartLayer = PL_BLOUSE_ARM_OUT	;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_PANTS_LEG_OUT	;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_BLOUSE			;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_PANTS			;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_PANTS_LEG_IN	;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_BLOUSE_ARM_IN	;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_COAT			;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	g_aEquipPart[EQ_ONE_PIECE].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0502, vEquipLayerSet));
	g_aEquipPart[EQ_ONE_PIECE].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0506, vEquipLayerSet));
	g_aEquipPart[EQ_ONE_PIECE].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0510, vEquipLayerSet));

	vEquipLayerSet.clear();

	TempEquipLayerSet.PartLayer = PL_BLOUSE_ARM_OUT	;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_PANTS_LEG_OUT	;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_BLOUSE			;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_PANTS			;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_PANTS_LEG_IN	;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_BLOUSE_ARM_IN	;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_COAT			;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	g_aEquipPart[EQ_ONE_PIECE].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0503, vEquipLayerSet));
	g_aEquipPart[EQ_ONE_PIECE].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0507, vEquipLayerSet));
	g_aEquipPart[EQ_ONE_PIECE].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0511, vEquipLayerSet));

	vEquipLayerSet.clear();

	TempEquipLayerSet.PartLayer = PL_BLOUSE_ARM_OUT	;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_PANTS_LEG_OUT	;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_BLOUSE			;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_PANTS			;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_PANTS_LEG_IN	;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_BLOUSE_ARM_IN	;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_COAT			;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	g_aEquipPart[EQ_ONE_PIECE].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0504, vEquipLayerSet));
	g_aEquipPart[EQ_ONE_PIECE].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0508, vEquipLayerSet));
	g_aEquipPart[EQ_ONE_PIECE].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0512, vEquipLayerSet));


	vEquipLayerSet.clear();

	TempEquipLayerSet.PartLayer = PL_BLOUSE_ARM_OUT	;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_PANTS_LEG_OUT	;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_BLOUSE			;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_PANTS			;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_PANTS_LEG_IN	;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_BLOUSE_ARM_IN	;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_COAT			;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	g_aEquipPart[EQ_ONE_PIECE].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0505, vEquipLayerSet));
	g_aEquipPart[EQ_ONE_PIECE].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0509, vEquipLayerSet));
	g_aEquipPart[EQ_ONE_PIECE].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0513, vEquipLayerSet));


	vEquipLayerSet.clear();

	TempEquipLayerSet.PartLayer = PL_BLOUSE_ARM_OUT	;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_PANTS_LEG_OUT	;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_BLOUSE			;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_PANTS			;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_PANTS_LEG_IN	;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_BLOUSE_ARM_IN	;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_COAT			;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	g_aEquipPart[EQ_ONE_PIECE].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0514, vEquipLayerSet));

	vEquipLayerSet.clear();

	TempEquipLayerSet.PartLayer = PL_BLOUSE_ARM_OUT	;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_PANTS_LEG_OUT	;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_BLOUSE			;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_PANTS			;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_PANTS_LEG_IN	;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_BLOUSE_ARM_IN	;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_COAT			;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	g_aEquipPart[EQ_ONE_PIECE].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0515, vEquipLayerSet));


	vEquipLayerSet.clear();

	TempEquipLayerSet.PartLayer = PL_BLOUSE_ARM_OUT	;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_PANTS_LEG_OUT	;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_BLOUSE			;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_PANTS			;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_PANTS_LEG_IN	;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_BLOUSE_ARM_IN	;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_COAT			;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	g_aEquipPart[EQ_ONE_PIECE].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0516, vEquipLayerSet));

	vEquipLayerSet.clear();

	TempEquipLayerSet.PartLayer = PL_BLOUSE_ARM_OUT	;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_PANTS_LEG_OUT	;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_BLOUSE			;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_PANTS			;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_PANTS_LEG_IN	;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_BLOUSE_ARM_IN	;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_COAT			;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	g_aEquipPart[EQ_ONE_PIECE].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0517, vEquipLayerSet));


	

	// EQ_STOCKINGS
	g_aEquipPart[EQ_STOCKINGS].EquipID = EQ_STOCKINGS;
	g_aEquipPart[EQ_STOCKINGS].iNOEquipLayer = 4;
	g_aEquipPart[EQ_STOCKINGS].vPartLayer.push_back(PL_FOOT_OUT		);
	g_aEquipPart[EQ_STOCKINGS].vPartLayer.push_back(PL_LEG_OUT		);
	g_aEquipPart[EQ_STOCKINGS].vPartLayer.push_back(PL_FOOT_IN		);
	g_aEquipPart[EQ_STOCKINGS].vPartLayer.push_back(PL_LEG_IN		);

	vEquipLayerSet.clear();

	TempEquipLayerSet.PartLayer = PL_FOOT_OUT		;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_LEG_OUT	;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_FOOT_IN		;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_LEG_IN	;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	g_aEquipPart[EQ_STOCKINGS].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0601, vEquipLayerSet));


	// EQ_SHOES
	g_aEquipPart[EQ_SHOES].EquipID = EQ_SHOES;
	g_aEquipPart[EQ_SHOES].iNOEquipLayer = 4;
	g_aEquipPart[EQ_SHOES].vPartLayer.push_back(PL_SHOE_OUT		);
	g_aEquipPart[EQ_SHOES].vPartLayer.push_back(PL_FOOT_LEG_OUT	);
	g_aEquipPart[EQ_SHOES].vPartLayer.push_back(PL_SHOE_IN		);
	g_aEquipPart[EQ_SHOES].vPartLayer.push_back(PL_FOOT_LEG_IN	);

	vEquipLayerSet.clear();

	TempEquipLayerSet.PartLayer = PL_SHOE_OUT		;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_FOOT_LEG_OUT		;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_SHOE_IN		;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_FOOT_LEG_IN	;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	g_aEquipPart[EQ_SHOES].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0701, vEquipLayerSet));

	vEquipLayerSet.clear();

	TempEquipLayerSet.PartLayer = PL_SHOE_OUT		;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_FOOT_LEG_OUT	;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_SHOE_IN		;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_FOOT_LEG_IN	;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	g_aEquipPart[EQ_SHOES].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0702, vEquipLayerSet));

	vEquipLayerSet.clear();

	TempEquipLayerSet.PartLayer = PL_SHOE_OUT		;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_FOOT_LEG_OUT	;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_SHOE_IN		;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_FOOT_LEG_IN	;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	g_aEquipPart[EQ_SHOES].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0703, vEquipLayerSet));



	// EQ_WEAPON1
	g_aEquipPart[EQ_WEAPON1].EquipID = EQ_WEAPON1;
	g_aEquipPart[EQ_WEAPON1].iNOEquipLayer = 2;
	g_aEquipPart[EQ_WEAPON1].vPartLayer.push_back(PL_WEAPON_OUT		);
	g_aEquipPart[EQ_WEAPON1].vPartLayer.push_back(PL_WEAPON_IN		);

	vEquipLayerSet.clear();

	TempEquipLayerSet.PartLayer = PL_WEAPON_OUT		;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_WEAPON_IN		;
	TempEquipLayerSet.ChangeType = CTYPE_NC			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	g_aEquipPart[EQ_WEAPON1].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0802, vEquipLayerSet) );
	g_aEquipPart[EQ_WEAPON1].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0803, vEquipLayerSet) );
	g_aEquipPart[EQ_WEAPON1].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0805, vEquipLayerSet) );
	g_aEquipPart[EQ_WEAPON1].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0807, vEquipLayerSet) );


	vEquipLayerSet.clear();

	TempEquipLayerSet.PartLayer = PL_WEAPON_OUT		;
	TempEquipLayerSet.ChangeType = CTYPE_NC			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_WEAPON_IN		;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	g_aEquipPart[EQ_WEAPON1].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0811, vEquipLayerSet) );


	vEquipLayerSet.clear();

	TempEquipLayerSet.PartLayer = PL_WEAPON_OUT		;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_WEAPON_IN		;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	g_aEquipPart[EQ_WEAPON1].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0804, vEquipLayerSet) );
	g_aEquipPart[EQ_WEAPON1].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0808, vEquipLayerSet) );
	g_aEquipPart[EQ_WEAPON1].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0809, vEquipLayerSet) );
	g_aEquipPart[EQ_WEAPON1].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0810, vEquipLayerSet) );
	g_aEquipPart[EQ_WEAPON1].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0812, vEquipLayerSet) );
	//  [8/16/2007 metalgeni] 무기 카테고리 추가
	g_aEquipPart[EQ_WEAPON1].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0813, vEquipLayerSet) );


	vEquipLayerSet.clear();

	TempEquipLayerSet.PartLayer = PL_WEAPON_OUT		;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_WEAPON_IN		;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	g_aEquipPart[EQ_WEAPON1].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0801, vEquipLayerSet) );
	g_aEquipPart[EQ_WEAPON1].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0806, vEquipLayerSet) );
	//  [8/16/2007 metalgeni] 무기 카테고리 추가
	g_aEquipPart[EQ_WEAPON1].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0814, vEquipLayerSet) );


	// EQ_WEAPON2 -- 안쓴다.
	g_aEquipPart[EQ_WEAPON2].EquipID = EQ_WEAPON2;
	g_aEquipPart[EQ_WEAPON2].iNOEquipLayer = 0;


	//BUCKLER
	g_aEquipPart[EQ_BUCKLER1].EquipID = EQ_BUCKLER1;
	g_aEquipPart[EQ_BUCKLER1].iNOEquipLayer = 2;
	g_aEquipPart[EQ_BUCKLER1].vPartLayer.push_back(PL_WEAPON_OUT	);
	g_aEquipPart[EQ_BUCKLER1].vPartLayer.push_back(PL_WEAPON_IN		);

	vEquipLayerSet.clear();

	TempEquipLayerSet.PartLayer = PL_WEAPON_OUT	;
	TempEquipLayerSet.ChangeType = CTYPE_NC	;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_WEAPON_OUT	;
	TempEquipLayerSet.ChangeType = CTYPE_O	;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	g_aEquipPart[EQ_BUCKLER1].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0811, vEquipLayerSet));


	//vEquipLayerSet.clear();

	//TempEquipLayerSet.PartLayer = PL_WEAPON_OUT		;
	//TempEquipLayerSet.ChangeType = CTYPE_O			;
	//vEquipLayerSet.push_back(TempEquipLayerSet);

	//TempEquipLayerSet.PartLayer = PL_WEAPON_IN		;
	//TempEquipLayerSet.ChangeType = CTYPE_O			;
	//vEquipLayerSet.push_back(TempEquipLayerSet);

	//g_aEquipPart[EQ_BUCKLER1].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0801, vEquipLayerSet) );
	//g_aEquipPart[EQ_BUCKLER1].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0806, vEquipLayerSet) );



	// EQ_HAIR_DRESS
	g_aEquipPart[EQ_HAIR_DRESS].EquipID = EQ_HAIR_DRESS;
	g_aEquipPart[EQ_HAIR_DRESS].iNOEquipLayer = 7;
	g_aEquipPart[EQ_HAIR_DRESS].vPartLayer.push_back(PL_CAP_FRONT	);
	g_aEquipPart[EQ_HAIR_DRESS].vPartLayer.push_back(PL_GOGGLE	);
	g_aEquipPart[EQ_HAIR_DRESS].vPartLayer.push_back(PL_EAR);
	g_aEquipPart[EQ_HAIR_DRESS].vPartLayer.push_back(PL_HAIR_FRONT	);
	g_aEquipPart[EQ_HAIR_DRESS].vPartLayer.push_back(PL_HAIR_REAR	);
	g_aEquipPart[EQ_HAIR_DRESS].vPartLayer.push_back(PL_MAKEUP	);
	g_aEquipPart[EQ_HAIR_DRESS].vPartLayer.push_back(PL_CAP_REAR	);


	vEquipLayerSet.clear();

	TempEquipLayerSet.PartLayer = PL_CAP_FRONT		;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_GOGGLE			;
	TempEquipLayerSet.ChangeType = CTYPE_NC			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_EAR			;
	TempEquipLayerSet.ChangeType = CTYPE_NC			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_HAIR_FRONT		;
	TempEquipLayerSet.ChangeType = CTYPE_PC			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_HAIR_REAR		;
	TempEquipLayerSet.ChangeType = CTYPE_NC			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_MAKEUP			;
	TempEquipLayerSet.ChangeType = CTYPE_NC			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_CAP_REAR		;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	g_aEquipPart[EQ_HAIR_DRESS].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0901, vEquipLayerSet) );

	vEquipLayerSet.clear();

	TempEquipLayerSet.PartLayer = PL_CAP_FRONT		;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_GOGGLE			;
	TempEquipLayerSet.ChangeType = CTYPE_NC			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_EAR			;
	TempEquipLayerSet.ChangeType = CTYPE_NC			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_HAIR_FRONT		;
	TempEquipLayerSet.ChangeType = CTYPE_NC			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_HAIR_REAR		;
	TempEquipLayerSet.ChangeType = CTYPE_NC			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_MAKEUP			;
	TempEquipLayerSet.ChangeType = CTYPE_NC			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_CAP_REAR		;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	g_aEquipPart[EQ_HAIR_DRESS].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0902, vEquipLayerSet) );

	vEquipLayerSet.clear();

	TempEquipLayerSet.PartLayer = PL_CAP_FRONT		;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_GOGGLE			;
	TempEquipLayerSet.ChangeType = CTYPE_NC			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_EAR			;
	TempEquipLayerSet.ChangeType = CTYPE_NC			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_HAIR_FRONT		;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_HAIR_REAR		;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_MAKEUP			;
	TempEquipLayerSet.ChangeType = CTYPE_NC			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_CAP_REAR		;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	g_aEquipPart[EQ_HAIR_DRESS].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0903, vEquipLayerSet) );

	vEquipLayerSet.clear();

	TempEquipLayerSet.PartLayer = PL_CAP_FRONT		;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_GOGGLE			;
	TempEquipLayerSet.ChangeType = CTYPE_NC			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_EAR			;
	TempEquipLayerSet.ChangeType = CTYPE_NC			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_HAIR_FRONT		;
	TempEquipLayerSet.ChangeType = CTYPE_PC			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_HAIR_REAR		;
	TempEquipLayerSet.ChangeType = CTYPE_NC			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_MAKEUP			;
	TempEquipLayerSet.ChangeType = CTYPE_NC			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_CAP_REAR		;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	g_aEquipPart[EQ_HAIR_DRESS].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0904, vEquipLayerSet) );


	vEquipLayerSet.clear();

	TempEquipLayerSet.PartLayer = PL_CAP_FRONT		;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_GOGGLE			;
	TempEquipLayerSet.ChangeType = CTYPE_NC			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_EAR			;
	TempEquipLayerSet.ChangeType = CTYPE_NC			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_HAIR_FRONT		;
	TempEquipLayerSet.ChangeType = CTYPE_NC			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_HAIR_REAR		;
	TempEquipLayerSet.ChangeType = CTYPE_NC			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_MAKEUP			;
	TempEquipLayerSet.ChangeType = CTYPE_NC			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_CAP_REAR		;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	g_aEquipPart[EQ_HAIR_DRESS].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0905, vEquipLayerSet) );

	vEquipLayerSet.clear();

	TempEquipLayerSet.PartLayer = PL_CAP_FRONT		;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_GOGGLE			;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_EAR			;
	//TempEquipLayerSet.ChangeType = CTYPE_X			;
	TempEquipLayerSet.ChangeType = CTYPE_O			; //  [5/20/2008 metalgeni] 하이바에서 귀걸이 보임으로 고침
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_HAIR_FRONT		;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_HAIR_REAR		;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_MAKEUP			;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_CAP_REAR		;
	TempEquipLayerSet.ChangeType = CTYPE_X			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	g_aEquipPart[EQ_HAIR_DRESS].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_0906, vEquipLayerSet) );


	
	// EQ_GOGGLE
	g_aEquipPart[EQ_GOGGLE].EquipID = EQ_GOGGLE;
	g_aEquipPart[EQ_GOGGLE].iNOEquipLayer = 1;
	g_aEquipPart[EQ_GOGGLE].vPartLayer.push_back(PL_GOGGLE	);

	vEquipLayerSet.clear();

	TempEquipLayerSet.PartLayer = PL_GOGGLE			;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	g_aEquipPart[EQ_GOGGLE].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_1001, vEquipLayerSet) );


	// EQ_EARRING
	g_aEquipPart[EQ_EARRING].EquipID = EQ_EARRING;
	g_aEquipPart[EQ_EARRING].iNOEquipLayer = 1;
	g_aEquipPart[EQ_EARRING].vPartLayer.push_back(PL_EAR	);

	vEquipLayerSet.clear();

	TempEquipLayerSet.PartLayer = PL_EAR			;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	g_aEquipPart[EQ_EARRING].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_1101, vEquipLayerSet) );


	// EQ_MAKEUP
	g_aEquipPart[EQ_MAKEUP].EquipID = EQ_MAKEUP;
	g_aEquipPart[EQ_MAKEUP].iNOEquipLayer = 1;
	g_aEquipPart[EQ_MAKEUP].vPartLayer.push_back(PL_MAKEUP	);

	vEquipLayerSet.clear();

	TempEquipLayerSet.PartLayer = PL_MAKEUP			;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	g_aEquipPart[EQ_MAKEUP].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_1201, vEquipLayerSet) );

	// EQ_HAIR
	g_aEquipPart[EQ_HAIR].EquipID = EQ_HAIR;
	g_aEquipPart[EQ_HAIR].iNOEquipLayer = 2;
	g_aEquipPart[EQ_HAIR].vPartLayer.push_back(PL_HAIR_FRONT	);
	g_aEquipPart[EQ_HAIR].vPartLayer.push_back(PL_HAIR_REAR		);

	vEquipLayerSet.clear();

	TempEquipLayerSet.PartLayer = PL_HAIR_FRONT		;
	TempEquipLayerSet.ChangeType = CTYPE_DC			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_HAIR_REAR		;
	TempEquipLayerSet.ChangeType = CTYPE_DC			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	g_aEquipPart[EQ_HAIR].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_1301, vEquipLayerSet) );
	g_aEquipPart[EQ_HAIR].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_1302, vEquipLayerSet) );


	// EQ_EXPRESSION
	g_aEquipPart[EQ_EXPRESSION].EquipID = EQ_EXPRESSION;
	g_aEquipPart[EQ_EXPRESSION].iNOEquipLayer = 1;
	g_aEquipPart[EQ_EXPRESSION].vPartLayer.push_back(PL_FACE	);

	vEquipLayerSet.clear();

	TempEquipLayerSet.PartLayer = PL_FACE			;
	TempEquipLayerSet.ChangeType = CTYPE_O			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	g_aEquipPart[EQ_EXPRESSION].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_1401, vEquipLayerSet) );
	g_aEquipPart[EQ_EXPRESSION].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_1402, vEquipLayerSet) );

	// EQ_SKIN
	g_aEquipPart[EQ_SKIN].EquipID = EQ_SKIN;
	g_aEquipPart[EQ_SKIN].iNOEquipLayer = 10;
	g_aEquipPart[EQ_SKIN].vPartLayer.push_back(PL_HAND_OUT	);
	g_aEquipPart[EQ_SKIN].vPartLayer.push_back(PL_ARM_OUT	);
	g_aEquipPart[EQ_SKIN].vPartLayer.push_back(PL_FOOT_OUT	);
	g_aEquipPart[EQ_SKIN].vPartLayer.push_back(PL_LEG_OUT	);
	g_aEquipPart[EQ_SKIN].vPartLayer.push_back(PL_BODY		);
	g_aEquipPart[EQ_SKIN].vPartLayer.push_back(PL_FOOT_IN	);
	g_aEquipPart[EQ_SKIN].vPartLayer.push_back(PL_LEG_IN	);
	g_aEquipPart[EQ_SKIN].vPartLayer.push_back(PL_HEAD		);
	g_aEquipPart[EQ_SKIN].vPartLayer.push_back(PL_HAND_IN	);
	g_aEquipPart[EQ_SKIN].vPartLayer.push_back(PL_ARM_IN	);

	vEquipLayerSet.clear();

	TempEquipLayerSet.PartLayer = PL_HAND_OUT	;
	TempEquipLayerSet.ChangeType = CTYPE_DC		;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_ARM_OUT	;
	TempEquipLayerSet.ChangeType = CTYPE_DC		;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_FOOT_OUT	;
	TempEquipLayerSet.ChangeType = CTYPE_DC		;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_LEG_OUT	;
	TempEquipLayerSet.ChangeType = CTYPE_DC		;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_BODY		;
	TempEquipLayerSet.ChangeType = CTYPE_DC			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_FOOT_IN	;
	TempEquipLayerSet.ChangeType = CTYPE_DC			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_LEG_IN		;
	TempEquipLayerSet.ChangeType = CTYPE_DC			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_HEAD		;
	TempEquipLayerSet.ChangeType = CTYPE_DC			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_HAND_IN	;
	TempEquipLayerSet.ChangeType = CTYPE_DC			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	TempEquipLayerSet.PartLayer = PL_ARM_IN		;
	TempEquipLayerSet.ChangeType = CTYPE_DC			;
	vEquipLayerSet.push_back(TempEquipLayerSet);

	g_aEquipPart[EQ_SKIN].mvLayerEquipSet.insert( std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::value_type(EQCLASS_1501, vEquipLayerSet) );

	bInit = true;
}


void InitEquipRestore()
{
	static bool bInit = false;
	if(bInit == true) return;

	for(int i = (int)PL_FIRST; i < (int)PL_LAST; ++i ) {	
		g_aEquipRestore[i].PartLayer = (PART_LAYER)i;
	}
	g_aEquipRestore[PL_WEAPON_OUT		].vRestoreEquipID.push_back(EQ_WEAPON1);

	g_aEquipRestore[PL_BLOUSE_ARM_OUT	].vRestoreEquipID.push_back(EQ_BLOUSE);
	g_aEquipRestore[PL_BLOUSE_ARM_OUT	].vRestoreEquipID.push_back(EQ_ONE_PIECE);	 

	g_aEquipRestore[PL_GLOVE_OUT		].vRestoreEquipID.push_back(EQ_GLOVES);

	g_aEquipRestore[PL_HAND_OUT			].vRestoreEquipID.push_back(EQ_SKIN);
	g_aEquipRestore[PL_HAND_ARM_OUT		].vRestoreEquipID.push_back(EQ_GLOVES);
	g_aEquipRestore[PL_ARM_OUT			].vRestoreEquipID.push_back(EQ_SKIN);

	g_aEquipRestore[PL_PANTS_LEG_OUT	].vRestoreEquipID.push_back(EQ_PANTS);
	g_aEquipRestore[PL_PANTS_LEG_OUT	].vRestoreEquipID.push_back(EQ_ONE_PIECE);

	g_aEquipRestore[PL_SHOE_OUT			].vRestoreEquipID.push_back(EQ_SHOES);

	g_aEquipRestore[PL_FOOT_OUT			].vRestoreEquipID.push_back(EQ_SKIN);
	g_aEquipRestore[PL_FOOT_OUT			].vRestoreEquipID.push_back(EQ_STOCKINGS);

	g_aEquipRestore[PL_FOOT_LEG_OUT		].vRestoreEquipID.push_back(EQ_SHOES);

	g_aEquipRestore[PL_LEG_OUT			].vRestoreEquipID.push_back(EQ_SKIN);
	g_aEquipRestore[PL_LEG_OUT			].vRestoreEquipID.push_back(EQ_STOCKINGS);

	g_aEquipRestore[PL_BLOUSE			].vRestoreEquipID.push_back(EQ_BLOUSE);
	g_aEquipRestore[PL_BLOUSE			].vRestoreEquipID.push_back(EQ_ONE_PIECE);

	g_aEquipRestore[PL_PANTS			].vRestoreEquipID.push_back(EQ_PANTS);
	g_aEquipRestore[PL_PANTS			].vRestoreEquipID.push_back(EQ_ONE_PIECE);

	g_aEquipRestore[PL_BODY				].vRestoreEquipID.push_back(EQ_SKIN);

	g_aEquipRestore[PL_PANTS_LEG_IN		].vRestoreEquipID.push_back(EQ_PANTS);
	g_aEquipRestore[PL_PANTS_LEG_IN		].vRestoreEquipID.push_back(EQ_ONE_PIECE);

	g_aEquipRestore[PL_SHOE_IN			].vRestoreEquipID.push_back(EQ_SHOES);

	g_aEquipRestore[PL_FOOT_IN			].vRestoreEquipID.push_back(EQ_SKIN);
	g_aEquipRestore[PL_FOOT_IN			].vRestoreEquipID.push_back(EQ_STOCKINGS);

	g_aEquipRestore[PL_FOOT_LEG_IN		].vRestoreEquipID.push_back(EQ_SHOES);

	g_aEquipRestore[PL_LEG_IN			].vRestoreEquipID.push_back(EQ_SKIN);
	g_aEquipRestore[PL_LEG_IN			].vRestoreEquipID.push_back(EQ_STOCKINGS);

	g_aEquipRestore[PL_CAP_FRONT		].vRestoreEquipID.push_back(EQ_HAIR_DRESS);

	g_aEquipRestore[PL_GOGGLE			].vRestoreEquipID.push_back(EQ_HAIR_DRESS);
	g_aEquipRestore[PL_GOGGLE			].vRestoreEquipID.push_back(EQ_GOGGLE);

	g_aEquipRestore[PL_EAR				].vRestoreEquipID.push_back(EQ_HAIR_DRESS);
	g_aEquipRestore[PL_EAR				].vRestoreEquipID.push_back(EQ_EARRING);

	g_aEquipRestore[PL_HAIR_FRONT		].vRestoreEquipID.push_back(EQ_HAIR);
	g_aEquipRestore[PL_HAIR_FRONT		].vRestoreEquipID.push_back(EQ_HAIR_DRESS);

	g_aEquipRestore[PL_HAIR_REAR		].vRestoreEquipID.push_back(EQ_HAIR);
	g_aEquipRestore[PL_HAIR_REAR		].vRestoreEquipID.push_back(EQ_HAIR_DRESS);

	g_aEquipRestore[PL_FACE				].vRestoreEquipID.push_back(EQ_EXPRESSION);

	g_aEquipRestore[PL_MAKEUP			].vRestoreEquipID.push_back(EQ_MAKEUP);
	g_aEquipRestore[PL_MAKEUP			].vRestoreEquipID.push_back(EQ_HAIR_DRESS);

	g_aEquipRestore[PL_HEAD				].vRestoreEquipID.push_back(EQ_SKIN);

	g_aEquipRestore[PL_CAP_REAR			].vRestoreEquipID.push_back(EQ_HAIR_DRESS);

	g_aEquipRestore[PL_BLOUSE_ARM_IN	].vRestoreEquipID.push_back(EQ_BLOUSE);
	g_aEquipRestore[PL_BLOUSE_ARM_IN	].vRestoreEquipID.push_back(EQ_ONE_PIECE);

	g_aEquipRestore[PL_GLOVE_IN			].vRestoreEquipID.push_back(EQ_GLOVES);

	g_aEquipRestore[PL_HAND_IN			].vRestoreEquipID.push_back(EQ_SKIN);

	g_aEquipRestore[PL_HAND_ARM_IN		].vRestoreEquipID.push_back(EQ_GLOVES);

	g_aEquipRestore[PL_ARM_IN			].vRestoreEquipID.push_back(EQ_SKIN);

	g_aEquipRestore[PL_WEAPON_IN		].vRestoreEquipID.push_back(EQ_WEAPON1);
	g_aEquipRestore[PL_WEAPON_IN		].vRestoreEquipID.push_back(EQ_BUCKLER1);

	g_aEquipRestore[PL_COAT				].vRestoreEquipID.push_back(EQ_BLOUSE);
	g_aEquipRestore[PL_COAT				].vRestoreEquipID.push_back(EQ_CLOAK);
	g_aEquipRestore[PL_COAT				].vRestoreEquipID.push_back(EQ_ONE_PIECE);

	bInit = true;

}

























