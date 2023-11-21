#include <algorithm>
#include <functional>
#include <string>
#include <map>

#include "SPCommon.h"

#include "SPRenderModelDEF.h"
#include "SPGameObjectDef.h"
#include "SPGameObject.h"
#include "SPGOBClusterDef.h"

#include "SPAvatarModelUnitDef.h"

#include "SPGOBModelUnit.h"
#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPItemStatusEquip.h"
#include "SPPlayerEquipInfo.h"


SPPlayerEquipInfo::SPPlayerEquipInfo(SPGameObject* pParent)
: m_pParent(pParent)
, m_pItemStatusEquipDefaultBlouse		(NULL)
, m_pItemStatusEquipDefaultPants		(NULL)
, m_pItemStatusEquipDefaultHair			(NULL)
, m_pItemStatusEquipDefaultExpression	(NULL)
, m_pItemStatusEquipDefaultSkin			(NULL)
{
	Init();	
}

SPPlayerEquipInfo::~SPPlayerEquipInfo()
{	
	SAFE_DELETE(m_pItemStatusEquipDefaultBlouse		);
	SAFE_DELETE(m_pItemStatusEquipDefaultPants		);
	SAFE_DELETE(m_pItemStatusEquipDefaultHair		);
	SAFE_DELETE(m_pItemStatusEquipDefaultExpression	);
	SAFE_DELETE(m_pItemStatusEquipDefaultSkin		);

	for(int i = EQ_FIRST; i < EQ_END; ++i) {
		SAFE_DELETE(m_pEquipItem[i]);
	}
}

void SPPlayerEquipInfo::Init()
{
	m_pItemStatusEquipDefaultBlouse		= new SPItemStatusEquip(EQ_BLOUSE);
	m_pItemStatusEquipDefaultPants		= new SPItemStatusEquip(EQ_PANTS);	
	m_pItemStatusEquipDefaultHair		= new SPItemStatusEquip(EQ_HAIR);	
	m_pItemStatusEquipDefaultExpression	= new SPItemStatusEquip(EQ_EXPRESSION);
	m_pItemStatusEquipDefaultSkin		= new SPItemStatusEquip(EQ_SKIN);

	ZeroMemory(m_pEquipItem, sizeof(m_pEquipItem));

	for(int i = EQ_FIRST; i < EQ_END; ++i) {
		m_pEquipItem[i] = new SPItemStatusEquip( (EQUIP_ID)i );
	}	
}

// 의상의 디폴트체인지값 할당.
bool SPPlayerEquipInfo::SetDefaultChange(EQUIP_ID EquipID, SPItemStatus* pItemStatus)
{
	if(IS_EQUIP != pItemStatus->SPSendMessage(MV_ITEM_GETSTATUSID)) 
		return false;

	if(EquipID != pItemStatus->SPSendMessage(MV_ITEM_GETEQUIPID)) 
		return false;

	//WILL//  여기서 Gender체크해야 한다.
	switch(EquipID) {
	case EQ_BLOUSE:
		m_pItemStatusEquipDefaultBlouse		->SPSendMessage(MV_ITEM_COPYEQUIPSTATUS, (LPARAM)pItemStatus);
		break;									
	case EQ_PANTS:								
		m_pItemStatusEquipDefaultPants		->SPSendMessage(MV_ITEM_COPYEQUIPSTATUS, (LPARAM)pItemStatus);
		break;									
	case EQ_HAIR:								
		m_pItemStatusEquipDefaultHair		->SPSendMessage(MV_ITEM_COPYEQUIPSTATUS, (LPARAM)pItemStatus);
		break;									
	case EQ_EXPRESSION:							
		m_pItemStatusEquipDefaultExpression	->SPSendMessage(MV_ITEM_COPYEQUIPSTATUS, (LPARAM)pItemStatus);
		break;									
	case EQ_SKIN:								
		m_pItemStatusEquipDefaultSkin		->SPSendMessage(MV_ITEM_COPYEQUIPSTATUS, (LPARAM)pItemStatus);
		break;
	}
	return true;
}

bool SPPlayerEquipInfo::SetCustume(SPItemStatus* pItemStatus)
{
	EQUIP_ID	EquipID		= (EQUIP_ID)pItemStatus->SPSendMessage(MV_ITEM_GETEQUIPID);
	EQUIP_CLASS	EquipClass	= (EQUIP_CLASS)pItemStatus->SPSendMessage(MV_ITEM_GETEQUIPCLASS);
	assert(EquipID != EQ_INVAILD);
	assert(EquipClass != EQCLASS_INVAILD);

	// EQ_FIRST == EQ_HAIR_DRESS
	
	if(pItemStatus->SPSendMessage(MV_ITEM_GETEQUIPCLASS)/100==5)		// 한벌옷
	{
		pItemStatus->SPSendMessage(MV_ITEM_SETEQUIPID, EQ_ONE_PIECE);
		EquipID = EQ_ONE_PIECE;
	}

	switch(EquipID) {
	case EQ_BLOUSE:
	case EQ_PANTS:
	case EQ_CLOAK:
		// 원피스입혀 있을때 상의,하의를 입으면 원피스를 벗긴다.
		Unload(EQ_ONE_PIECE);		
		break;
	case EQ_BUCKLER1:
		{
			//양손무기 들었을때 방패 착용하면 양손무기 벗긴다.
			EQUIP_CLASS TempEquipClass	= (EQUIP_CLASS)(*m_pEquipItem[EQ_WEAPON1]).SPSendMessage(MV_ITEM_GETEQUIPCLASS);
			switch(TempEquipClass) {
			case EQCLASS_0802:
			case EQCLASS_0803:
			case EQCLASS_0805:
			case EQCLASS_0807:
			case EQCLASS_0811:
				break;
			default:		
				Unload(EQ_WEAPON1);
				break;
			}
		}
		break;
	}

	switch(EquipClass) {
	case EQCLASS_0804: // 양손무기
	case EQCLASS_0808:
	case EQCLASS_0809:
	case EQCLASS_0810:
	case EQCLASS_0801:
	case EQCLASS_0806:
	case EQCLASS_0812:

	//  [8/16/2007 metalgeni] 무기 카테고리 추가
	case EQCLASS_0813:
	case EQCLASS_0814:

		// 방패 들었을때 양손무기 착용하면 방패 벗긴다.
		Unload(EQ_BUCKLER1);
		break;
	}

	std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::iterator IterMapEquipPart;
	std::vector<SPEquipLayerSet>* pvEquipLayerSet = NULL;
	std::vector<SPEquipLayerSet>::iterator	IterEquipLayerSet;	

	SPEquipLayerSet* pEquipLayerSet;	// PartLayer와 ChangeType;
	SPEquipLayerPool EquipLayerPool;
	
	IterMapEquipPart = g_aEquipPart[EquipID].mvLayerEquipSet.find(EquipClass);
	if(IterMapEquipPart != g_aEquipPart[EquipID].mvLayerEquipSet.end() ) {

		m_pEquipItem[EquipID]->SPSendMessage(MV_ITEM_COPYEQUIPSTATUS, (LPARAM)pItemStatus);
		pvEquipLayerSet = &((*IterMapEquipPart).second);

		if(pvEquipLayerSet->size() != g_aEquipPart[EquipID].vPartLayer.size()) {
			assert(pvEquipLayerSet->size() == g_aEquipPart[EquipID].vPartLayer.size());
			return false;
		}

		SPGOBModelUnit* pGOBModelUnit;
		std::vector<PART_LAYER>::iterator IterPartPlayer;

		EquipLayerPool.EquipClass	= (EQUIP_CLASS)(*m_pEquipItem[EquipID]).SPSendMessage(MV_ITEM_GETEQUIPCLASS);
		EquipLayerPool.iItemID		= (*m_pEquipItem[EquipID]).SPSendMessage(MV_ITEM_GETEQUIPITEMID);
		EquipLayerPool.iItemColor	= (*m_pEquipItem[EquipID]).SPSendMessage(MV_ITEM_GETCOLOR);

		pGOBModelUnit = (SPGOBModelUnit*)(m_pParent->SPGOBPerformMessage(SPGM_GETMODEL));
		IterPartPlayer = g_aEquipPart[EquipID].vPartLayer.begin();
		IterEquipLayerSet = (*pvEquipLayerSet).begin();
		
		EQUIP_CLASS tempEquipClass = (EQUIP_CLASS)m_pEquipItem[EQ_HAIR_DRESS]->SPSendMessage(MV_ITEM_GETEQUIPCLASS);

		//윗머리를 덮는 앞머리 장식 머리장식,   윗머리를 덮는 앞,뒷 머리 장식	
		if(tempEquipClass == EQCLASS_0901 || tempEquipClass == EQCLASS_0904)
			pGOBModelUnit->SPSendMessage(MV_SET_PARTCHANGE, TRUE);
		else 
			pGOBModelUnit->SPSendMessage(MV_SET_PARTCHANGE, FALSE);

		for(;IterPartPlayer != g_aEquipPart[EquipID].vPartLayer.end(); ++IterPartPlayer, ++IterEquipLayerSet) {
			EquipLayerPool.PartLayer = (*IterPartPlayer);
			pEquipLayerSet = &(*IterEquipLayerSet);
			switch(pEquipLayerSet->ChangeType ) {
			case CTYPE_O:
				pGOBModelUnit->SPSendMessage(MV_SET_EQUIPLAYERPOOL, (WPARAM)(&EquipLayerPool));
				Restore(EquipLayerPool, EquipID);
				break;

			case CTYPE_X:
				pGOBModelUnit->SPSendMessage(MV_SET_EQUIPLAYERPOOL, (WPARAM)0,	EquipLayerPool.PartLayer);
				break;				

			case CTYPE_PC:				
				Restore(EquipLayerPool, EquipID);
				break;

			case CTYPE_DC:				
				switch(EquipID) {
				case EQ_BLOUSE:
					m_pItemStatusEquipDefaultBlouse->SPSendMessage(MV_ITEM_SETEQUIPCLASS,	(LPARAM)(EquipLayerPool.EquipClass));
					m_pItemStatusEquipDefaultBlouse->SPSendMessage(MV_ITEM_SETCOLOR,		(LPARAM)(EquipLayerPool.iItemColor));
					m_pItemStatusEquipDefaultBlouse->SPSendMessage(MV_ITEM_SETEQUIPITEMID,	(LPARAM)(EquipLayerPool.iItemID));
					m_pItemStatusEquipDefaultBlouse->SPSendMessage(MV_ITEM_SETEQUIPID,		(LPARAM)EquipID);
					break;

				case EQ_PANTS:
					m_pItemStatusEquipDefaultPants->SPSendMessage(MV_ITEM_SETEQUIPCLASS,	(LPARAM)(EquipLayerPool.EquipClass));
					m_pItemStatusEquipDefaultPants->SPSendMessage(MV_ITEM_SETCOLOR,			(LPARAM)(EquipLayerPool.iItemColor));
					m_pItemStatusEquipDefaultPants->SPSendMessage(MV_ITEM_SETEQUIPITEMID,	(LPARAM)(EquipLayerPool.iItemID));
					m_pItemStatusEquipDefaultPants->SPSendMessage(MV_ITEM_SETEQUIPID,		(LPARAM)EquipID);
					break;

				case EQ_HAIR:
					m_pItemStatusEquipDefaultHair->SPSendMessage(MV_ITEM_SETEQUIPCLASS,	(LPARAM)(EquipLayerPool.EquipClass));
					m_pItemStatusEquipDefaultHair->SPSendMessage(MV_ITEM_SETCOLOR,		(LPARAM)(EquipLayerPool.iItemColor));
					m_pItemStatusEquipDefaultHair->SPSendMessage(MV_ITEM_SETEQUIPITEMID,(LPARAM)(EquipLayerPool.iItemID));
					m_pItemStatusEquipDefaultHair->SPSendMessage(MV_ITEM_SETEQUIPID,	(LPARAM)EquipID);
					break;

				case EQ_EXPRESSION:
					m_pItemStatusEquipDefaultExpression->SPSendMessage(MV_ITEM_SETEQUIPCLASS,	(LPARAM)(EquipLayerPool.EquipClass));
					m_pItemStatusEquipDefaultExpression->SPSendMessage(MV_ITEM_SETCOLOR,		(LPARAM)(EquipLayerPool.iItemColor));
					m_pItemStatusEquipDefaultExpression->SPSendMessage(MV_ITEM_SETEQUIPITEMID,	(LPARAM)(EquipLayerPool.iItemID));
					m_pItemStatusEquipDefaultExpression->SPSendMessage(MV_ITEM_SETEQUIPID,		(LPARAM)EquipID);
					break;

				case EQ_SKIN:
					m_pItemStatusEquipDefaultSkin->SPSendMessage(MV_ITEM_SETEQUIPCLASS,	(LPARAM)(EquipLayerPool.EquipClass));
					m_pItemStatusEquipDefaultSkin->SPSendMessage(MV_ITEM_SETCOLOR,		(LPARAM)(EquipLayerPool.iItemColor));
					m_pItemStatusEquipDefaultSkin->SPSendMessage(MV_ITEM_SETEQUIPITEMID,(LPARAM)(EquipLayerPool.iItemID));
					m_pItemStatusEquipDefaultSkin->SPSendMessage(MV_ITEM_SETEQUIPID,	(LPARAM)EquipID);
					break;

				default:
					assert(0 && "Default와 관계없는 EquipStatus입니다");
					continue;
					break;
				}
				// CTYPE_DC는 디폴트값을 바꾼후 다시 체크해서 입혀야 한다.
				Restore(EquipLayerPool, EquipID); 
				break;

			case CTYPE_NC:		

				//if(EquipLayerPool.PartLayer != PL_WEAPON_IN &&
				//	EquipLayerPool.EquipClass EQCLASS_0802 
				//	EQCLASS_0803 
				//	EQCLASS_0805 
				//	EQCLASS_0807 


				Restore(EquipLayerPool, EquipID);
				break;
			}
		}

		// Cap(0901, 0904, 0906일때는 모자를 한번 더 씌워준다)
		if(EQ_HAIR==pItemStatus->SPSendMessage(MV_ITEM_GETEQUIPID) || EQ_GOGGLE==pItemStatus->SPSendMessage(MV_ITEM_GETEQUIPID))
		{
			SPItemStatus* pItem = new SPItemStatusEquip();

			// Hair Dress에 대한 ItemEquip을 가져온다.
			pItem->SPSendMessage( MV_ITEM_COPYEQUIPSTATUS, (LPARAM)m_pEquipItem[EQ_HAIR_DRESS]) ;
			LRESULT lEqClass = pItem->SPSendMessage(MV_ITEM_GETEQUIPCLASS);		

			switch( lEqClass )
			{
				case EQCLASS_0901:		// 머리모양 바뀌는 아이템
				case EQCLASS_0904:		// 머리모양 바뀌는 아이템
				case EQCLASS_0906:		// 완전히 뒤집어 쓰는 아이템
					SetCustume(pItem);
			}
			delete pItem;
		}
		return true;

	} else {
		return Unload(EquipID);
	}		
}


bool SPPlayerEquipInfo::Unload(EQUIP_ID EquipID)
{

	if(EquipID == EQ_NULL) {
		//잘못된 이큅아이디
		return false;
	}

	SPEquipLayerPool EquipLayerPool;
	bool bRetVal = false;

	EQUIP_CLASS EquipClass	= (EQUIP_CLASS)m_pEquipItem[EquipID]->SPSendMessage(MV_ITEM_GETEQUIPCLASS);
	EquipLayerPool.EquipClass	= EquipClass;
	EquipLayerPool.iItemID		= (*m_pEquipItem[EquipID]).SPSendMessage(MV_ITEM_GETEQUIPITEMID);
	EquipLayerPool.iItemColor	= (*m_pEquipItem[EquipID]).SPSendMessage(MV_ITEM_GETCOLOR);	

	m_pEquipItem[EquipID]->SPSendMessage(MV_ITEM_SETEQUIPNULL);

	std::vector<PART_LAYER>::iterator IterPartPlayer;
	std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::iterator IterMapEquipPart;
	IterMapEquipPart = g_aEquipPart[EquipID].mvLayerEquipSet.find(EquipClass);
	if(IterMapEquipPart != g_aEquipPart[EquipID].mvLayerEquipSet.end() ) {
		
		if(EquipID == EQ_ONE_PIECE) { 
			// 원피스일경우 EQ_BLOUSE와 EQ_PANTS, EQ_CLOAK를 따로 언로드
			IterPartPlayer = g_aEquipPart[EQ_BLOUSE].vPartLayer.begin();		
			for(;IterPartPlayer != g_aEquipPart[EQ_BLOUSE].vPartLayer.end(); ++IterPartPlayer) {
				EquipLayerPool.PartLayer = (*IterPartPlayer);		
				Restore(EquipLayerPool, EQ_BLOUSE);
			}

			IterPartPlayer = g_aEquipPart[EQ_PANTS].vPartLayer.begin();		
			for(;IterPartPlayer != g_aEquipPart[EQ_PANTS].vPartLayer.end(); ++IterPartPlayer) {
				EquipLayerPool.PartLayer = (*IterPartPlayer);		
				Restore(EquipLayerPool, EQ_PANTS);
			}

			IterPartPlayer = g_aEquipPart[EQ_CLOAK].vPartLayer.begin();		
			for(;IterPartPlayer != g_aEquipPart[EQ_CLOAK].vPartLayer.end(); ++IterPartPlayer) {
				EquipLayerPool.PartLayer = (*IterPartPlayer);		
				Restore(EquipLayerPool, EQ_CLOAK);
			}
		} else {
			IterPartPlayer = g_aEquipPart[EquipID].vPartLayer.begin();		
			for(;IterPartPlayer != g_aEquipPart[EquipID].vPartLayer.end(); ++IterPartPlayer) {
				EquipLayerPool.PartLayer = (*IterPartPlayer);		
				Restore(EquipLayerPool, EquipID);
			}
		}

		bRetVal = true;
	}
	return bRetVal;
}



bool SPPlayerEquipInfo::Restore(SPEquipLayerPool EquipLayerPool, EQUIP_ID EquipID)
{
	assert(EquipLayerPool.PartLayer == g_aEquipRestore[EquipLayerPool.PartLayer].PartLayer);
	SPGOBModelUnit* pGOBModelUnit = (SPGOBModelUnit*)(m_pParent->SPGOBPerformMessage(SPGM_GETMODEL));
	SPEquipLayerPool	EquipLayerPoolRestore;
	bool				bRetVal = false;

	// EquipID를 벗었을때 현재 레이어와 관계있는 Item을 찾아 Restore한다.

	EQUIP_ID EquipIDIter;

	std::vector<EQUIP_ID>::reverse_iterator RIter = g_aEquipRestore[EquipLayerPool.PartLayer].vRestoreEquipID.rbegin();	
	for(;RIter != g_aEquipRestore[EquipLayerPool.PartLayer].vRestoreEquipID.rend(); ++RIter) {						
		//DXUTOutputDebugString("RIter = %d", *RIter);
		EquipIDIter = *RIter;		

		EquipLayerPoolRestore.EquipClass = (EQUIP_CLASS)m_pEquipItem[*RIter]->SPSendMessage(MV_ITEM_GETEQUIPCLASS);
		EquipLayerPoolRestore.iItemColor = m_pEquipItem[*RIter]->SPSendMessage(MV_ITEM_GETCOLOR);
		EquipLayerPoolRestore.iItemID	 = m_pEquipItem[*RIter]->SPSendMessage(MV_ITEM_GETEQUIPITEMID);
		EquipLayerPoolRestore.PartLayer	 = EquipLayerPool.PartLayer;
		
		if(IsDefaultEquip(EquipIDIter)) {
			//////////////////////////////////////////////////////////////////////////
			//  치마속에 바지입은 버그 수정.. CTYPE_X 일때 갈아입다 false 나는 버그..
			std::map<EQUIP_CLASS, std::vector<SPEquipLayerSet> >::iterator IterMapEquipPart;
			std::vector<SPEquipLayerSet>* pvEquipLayerSet = NULL;
			std::vector<SPEquipLayerSet>::iterator	IterEquipLayerSet;	

			SPEquipLayerSet* pEquipLayerSet;	// PartLayer와 ChangeType;		

			IterMapEquipPart = g_aEquipPart[EquipIDIter].mvLayerEquipSet.find(EquipLayerPoolRestore.EquipClass);
			if(IterMapEquipPart != g_aEquipPart[EquipIDIter].mvLayerEquipSet.end() ) {			
				pvEquipLayerSet = &((*IterMapEquipPart).second);
				
				std::vector<PART_LAYER>::iterator IterPartPlayer;
				
				IterPartPlayer = g_aEquipPart[EquipIDIter].vPartLayer.begin();
				IterEquipLayerSet = (*pvEquipLayerSet).begin();

				for(;IterPartPlayer != g_aEquipPart[EquipIDIter].vPartLayer.end(); ++IterPartPlayer, ++IterEquipLayerSet) {

					pEquipLayerSet = &(*IterEquipLayerSet);
					switch(pEquipLayerSet->ChangeType ) {
					default:	
						bRetVal = (bool)pGOBModelUnit->SPSendMessage(MV_SET_EQUIPLAYERPOOL, (WPARAM)(&EquipLayerPoolRestore));	
						break;

					case CTYPE_X:						
						bRetVal = true;
						break;

					}
				}
			}
			//////////////////////////////////////////////////////////////////////////	
		} else {
			bRetVal = (bool)pGOBModelUnit->SPSendMessage(MV_SET_EQUIPLAYERPOOL, (WPARAM)(&EquipLayerPoolRestore));
		}		

		if(bRetVal == true) {
			break;
		}							
	}

	EQUIP_CLASS tempEquipClass = (EQUIP_CLASS)m_pEquipItem[EQ_HAIR_DRESS]->SPSendMessage(MV_ITEM_GETEQUIPCLASS);
	if(tempEquipClass == EQCLASS_0901 || tempEquipClass == EQCLASS_0904)
		pGOBModelUnit->SPSendMessage(MV_SET_PARTCHANGE, TRUE);
	else 
		pGOBModelUnit->SPSendMessage(MV_SET_PARTCHANGE, FALSE);

	//관계있는 Item이 없을때 Default값으로 Restore한다.
	if( bRetVal == false) {
		switch(EquipID) {
		case EQ_BLOUSE:
			EquipLayerPoolRestore.EquipClass = (EQUIP_CLASS)m_pItemStatusEquipDefaultBlouse->SPSendMessage(MV_ITEM_GETEQUIPCLASS);
			EquipLayerPoolRestore.iItemColor = m_pItemStatusEquipDefaultBlouse->SPSendMessage(MV_ITEM_GETCOLOR);
			EquipLayerPoolRestore.iItemID	 = m_pItemStatusEquipDefaultBlouse->SPSendMessage(MV_ITEM_GETEQUIPITEMID);
			EquipLayerPoolRestore.PartLayer	 = EquipLayerPool.PartLayer;
			pGOBModelUnit->SPSendMessage(MV_SET_EQUIPLAYERPOOL, (WPARAM)(&EquipLayerPoolRestore));							
			break;

		case EQ_PANTS:
			EquipLayerPoolRestore.EquipClass = (EQUIP_CLASS)m_pItemStatusEquipDefaultPants->SPSendMessage(MV_ITEM_GETEQUIPCLASS);
			EquipLayerPoolRestore.iItemColor = m_pItemStatusEquipDefaultPants->SPSendMessage(MV_ITEM_GETCOLOR);
			EquipLayerPoolRestore.iItemID	 = m_pItemStatusEquipDefaultPants->SPSendMessage(MV_ITEM_GETEQUIPITEMID);
			EquipLayerPoolRestore.PartLayer	 = EquipLayerPool.PartLayer;
			pGOBModelUnit->SPSendMessage(MV_SET_EQUIPLAYERPOOL, (WPARAM)(&EquipLayerPoolRestore));							
			break;

		case EQ_HAIR:
			EquipLayerPoolRestore.EquipClass = (EQUIP_CLASS)m_pItemStatusEquipDefaultHair->SPSendMessage(MV_ITEM_GETEQUIPCLASS);
			EquipLayerPoolRestore.iItemColor = m_pItemStatusEquipDefaultHair->SPSendMessage(MV_ITEM_GETCOLOR);
			EquipLayerPoolRestore.iItemID	 = m_pItemStatusEquipDefaultHair->SPSendMessage(MV_ITEM_GETEQUIPITEMID);
			EquipLayerPoolRestore.PartLayer	 = EquipLayerPool.PartLayer;
			pGOBModelUnit->SPSendMessage(MV_SET_EQUIPLAYERPOOL, (WPARAM)(&EquipLayerPoolRestore));							
			break;

		case EQ_EXPRESSION:
			EquipLayerPoolRestore.EquipClass = (EQUIP_CLASS)m_pItemStatusEquipDefaultExpression->SPSendMessage(MV_ITEM_GETEQUIPCLASS);
			EquipLayerPoolRestore.iItemColor = m_pItemStatusEquipDefaultExpression->SPSendMessage(MV_ITEM_GETCOLOR);
			EquipLayerPoolRestore.iItemID	 = m_pItemStatusEquipDefaultExpression->SPSendMessage(MV_ITEM_GETEQUIPITEMID);
			EquipLayerPoolRestore.PartLayer	 = EquipLayerPool.PartLayer;
			pGOBModelUnit->SPSendMessage(MV_SET_EQUIPLAYERPOOL, (WPARAM)(&EquipLayerPoolRestore));							
			break;

		case EQ_SKIN:
			EquipLayerPoolRestore.EquipClass = (EQUIP_CLASS)m_pItemStatusEquipDefaultSkin->SPSendMessage(MV_ITEM_GETEQUIPCLASS);
			EquipLayerPoolRestore.iItemColor = m_pItemStatusEquipDefaultSkin->SPSendMessage(MV_ITEM_GETCOLOR);
			EquipLayerPoolRestore.iItemID	 = m_pItemStatusEquipDefaultSkin->SPSendMessage(MV_ITEM_GETEQUIPITEMID);
			EquipLayerPoolRestore.PartLayer	 = EquipLayerPool.PartLayer;
			pGOBModelUnit->SPSendMessage(MV_SET_EQUIPLAYERPOOL, (WPARAM)(&EquipLayerPoolRestore));							
			break;

		default:
			// default가 아니면 Restore를 안해도 된다.
			return false;
			break;
		}
	}
	return true;
}

SPItemStatus* SPPlayerEquipInfo::GetItemStatus(EQUIP_ID eID)
{
	return m_pEquipItem[eID];
}