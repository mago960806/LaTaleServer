
/**
	@file : SPWindowBeautyShopDataMng.cpp
	@auth : 임화용 ( duragon@actoz.com )
	@date : 2006/09/23
	@update 
		20060923 : 클래스 구성
		20060924 : 런칭

	Item ID 값
	LayerID(2) : Gender(2) : Special(1) : Serial(2) : Color(2)
	
*/
#include "SPCommon.h"

#include "SPUtil.h"
#include "SPWindowDEF.h"
#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"
#include "SPCheckManager.h"

#include "SPGOBManager.h"
#include "SPGameObject.h"
#include "SPItem.h"
#include "SPPlayerInvenArchive.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPPlayer.h"

#include "SPWindowBeautyShopDataUnit.h"
#include "SPWindowBeautyShopDataMng.h"
#include "_Error_log.h"
//#include "SPDebug.h"

BeautyShopManager::BeautyShopManager()
{
	 Create();
}


BeautyShopManager::~BeautyShopManager()
{
	Clean() ;
}

///< 생성
void BeautyShopManager::Create()
{
	for(int i = 0; i < BS_ITEM_MAX; i++)
	{
		m_pBSGroup[i] = new BeautyShopItemGroupList ;
	}
	Init() ;
}

///< 데이터 초기화
void BeautyShopManager::Init()
{
	//D3D_SetupLogFile() ;
	m_iCurGender = 0 ;
	for(int i = 0; i < 5; i++)
	{
		m_bEnabled[i] = true ;			// Default 모두 활성화
		m_bLoadFail[i] = false ;		// Default Character 로딩 실패
		m_iLayerIndex[i].Clear() ;
	}
}

///< Release
void BeautyShopManager::Clean()
{
	Init() ;
	for(int i = 0; i < BS_ITEM_MAX; i++)
	{
		SAFE_DELETE(m_pBSGroup[i]) ;
	}
}


/**
	BeautyShop용 LDT 파일을 읽어서 Data를 셋팅한다.
*/
void BeautyShopManager::LoadBeautyShopLDT()
{
	std::string strLDTFile;
	strLDTFile = "DATA/LDT/BEAUTY_SHOP.LDT";

	SPLDTFile* pkLDTFile = NULL;
	if( !g_pResourceManager->GetLDTFromFileName(strLDTFile.c_str(), &pkLDTFile) )
	{
//		DXUTOutputDebugString("BeautyShopItemData:[%s]LDT 파일을 찾지 못했습니다.", strLDTFile.c_str()) ;
		return;
	}

	int iDataNum, iFieldNum, i, j;
	long lItemID;

	LDT_Field Field;
	std::string strFileName;

	BS_IN_ITEM		bsItem ;	///< 입력할 아이템 데이터

	iDataNum = pkLDTFile->GetItemCount();
	iFieldNum = pkLDTFile->GetFieldCount();

	for( i = 0; i < iDataNum; i++ )
	{
		lItemID = pkLDTFile->GetPrimaryKey(i);
		bsItem.Clear() ;
		bsItem.itemID = (int)lItemID ;

		for( j = 0; j < iFieldNum; j++ )
		{
			pkLDTFile->GetField(lItemID, j, Field);

			if( Field.nFldTyp == fldNum )
			{
				if( Field.uData.lValue == 0 )
					continue;
			}
			else if( Field.nFldTyp == fldString )
			{
				if( (int)strlen(Field.uData.pValue) == 0 )
					continue;
			}

			//////////////////////////////////////////////////////////////////////////
			// compare String
			//if(strcmp(Field.pName, "_CategoryIndex") == 0 )
			//	bsItem.itemLayer = Field.uData.lValue ;
			//else if( strcmp(Field.pName, "_CategoryDetail") == 0 )
			//	bsItem.strCatego = Field.uData.pValue ;
			//else if( strcmp(Field.pName, "_Name") == 0 )
			//	bsItem.strDetail = Field.uData.pValue ;
			//else if( strcmp(Field.pName, "_PositionIndex") == 0 )
			//	bsItem.positionID = Field.uData.lValue ;
			//else if( strcmp(Field.pName, "_ItemID") == 0 )
			//	bsItem.itemCode = Field.uData.lValue ;
			//else if( strcmp(Field.pName, "_RequireItem1") == 0 )
			//	bsItem.needItemID1 = Field.uData.lValue ;
			//else if( strcmp(Field.pName, "_RequireItem2") == 0 )
			//	bsItem.needItemID2 = Field.uData.lValue ;

			//////////////////////////////////////////////////////////////////////////
			// compare Key
			if( j == 2 )			//_CategoryIndex
				bsItem.itemLayer = Field.uData.lValue ;
			else if( j == 3 )		//_CategoryDetail (str)
				bsItem.strCatego = Field.uData.pValue ;
			else if( j == 4 )		//_Name (str)
				bsItem.strDetail = Field.uData.pValue ;
			else if( j == 5 )		//_PositionIndex
				bsItem.positionID = Field.uData.lValue ;
			else if( j == 6 )		//_ItemID
				bsItem.itemCode = Field.uData.lValue ;
			else if( j == 7 )		//_RequireItem1
				bsItem.needItemID1 = Field.uData.lValue ;
			else if( j == 8 )		//_RequireItem2
				bsItem.needItemID2 = Field.uData.lValue ;
		}
		AddBeautyItem(bsItem) ;
	}

	SAFE_RELEASE(pkLDTFile) ;
}



/**
	아이템 파일을 자료 구조에 넣기
	@param BS_IN_ITEM : Input Item struct
	@param bool : 헤어컬러, 렌즈컬러, 스킨컬러는 남녀 모두 넣음.
*/
void BeautyShopManager::AddBeautyItem(BS_IN_ITEM in_item)
{
	BS_CODE_PARSE parse_item ;
	Parsing_CharCode(in_item.itemCode, &parse_item) ;

	// 성별 체크
	if( parse_item.iGender < 1 || parse_item.iGender > 2 )	parse_item.iGender = 1 ;
	parse_item.iGender -= 1 ;		// ID -> Index
	if( in_item.itemLayer < 0 || in_item.itemLayer > BS_ITEM_MAX )
	{
		//DXUTOutputDebugString("■ERROR> BeautyShopManager::AddBeautyItem LayerIndex [%d]", in_item.itemLayer) ;
		return ;
	}


	// 카테고리 검색 
	int iAddItemLayer = in_item.itemLayer -1 ;
	switch( iAddItemLayer )
	{
	case BS_HAIR:		// 헤어
	case BS_EYE:		// 눈성형
		m_pBSGroup[iAddItemLayer]->AddItem(parse_item.iGender, iAddItemLayer, parse_item.iSpecialID, in_item.strCatego.c_str(), in_item.itemID, in_item.itemCode, in_item.positionID, in_item.strDetail.c_str(), in_item.needItemID1, in_item.needItemID2) ;
		break ;

	case BS_HAIR_C:		// 헤어 컬러 (남녀)
	case BS_EYE_C:		// 눈 컬러렌즈 (남녀)
		if( parse_item.iColor >= COLOR_SPECIAL )	parse_item.iSpecialID = 1 ;		// 스페셜
		m_pBSGroup[iAddItemLayer]->AddItem(0, iAddItemLayer, parse_item.iSpecialID, in_item.strCatego.c_str(), in_item.itemID, in_item.itemCode, in_item.positionID, in_item.strDetail.c_str(), in_item.needItemID1, in_item.needItemID2) ;
		m_pBSGroup[iAddItemLayer]->AddItem(1, iAddItemLayer, parse_item.iSpecialID, in_item.strCatego.c_str(), in_item.itemID, in_item.itemCode, in_item.positionID, in_item.strDetail.c_str(), in_item.needItemID1, in_item.needItemID2) ;
		break ;

	case BS_SKIN:		// 피부색 (남녀)
		m_pBSGroup[BS_SKIN]->AddItem(0, BS_SKIN, parse_item.iSpecialID, in_item.strCatego.c_str(), in_item.itemID, in_item.itemCode, in_item.positionID, in_item.strDetail.c_str(), in_item.needItemID1, in_item.needItemID2) ;
		m_pBSGroup[BS_SKIN]->AddItem(1, BS_SKIN, parse_item.iSpecialID, in_item.strCatego.c_str(), in_item.itemID, in_item.itemCode, in_item.positionID, in_item.strDetail.c_str(), in_item.needItemID1, in_item.needItemID2) ;
		break ;

	default:
		//DXUTOutputDebugString("■ERROR> AddBeautyItem : in_item.itemLayer ERROR[%d]\n", in_item.itemLayer -1);
		break ;
	}
}

/**
	캐릭터 코드 파싱
	@param int : 캐릭터의 ItemID
	@param BS_CODE_PARSE : 분리된 데이터 Structure
*/
int BeautyShopManager::Parsing_CharCode(int iCharCode, BS_CODE_PARSE* _p)
{
	if( iCharCode < 1 )		return -1;

	_p->iCode			=  iCharCode ;
	_p->iLayerID		=  iCharCode / 10000000 ;
	_p->iGender			= (iCharCode % 1000000)?(iCharCode % 1000000 / 100000):(0) ;
	_p->iSpecialID		= (iCharCode % 100000)?(iCharCode % 100000 / 10000):(0) ;
	_p->iSerialID		= (iCharCode % 10000)?(iCharCode % 10000 / 100):(0) ;
	_p->iColor			= iCharCode % 100 ;
	_p->iCodeNC			= _p->iCode - _p->iColor ;	// 컬러를 제외한 코드값

	return 0 ;
}


/**
	테스트용 초기화
*/
void BeautyShopManager::SetTestCharSet(int _gender)
{
	m_iCurGender = _gender ;

	for(int i = 0; i < BS_ITEM_MAX; i++)
	{
		m_iLayerIndex[i].Clear() ;
		m_bEnabled[i] = true ;
	}

}

/**
	내가 갖추고 있는 Layer가 현재 판매중이 아닐경우
	Default로 셋팅을 해줘야 한다.
*/
void BeautyShopManager::SetDefaultIndexSet(int _itemLayer)
{
	m_bLoadFail[_itemLayer] = true ;
	m_iLayerIndex[_itemLayer].InitCategory(0) ;
	m_iLayerIndex[_itemLayer].InitDetail(0) ;
}


/**
	캐릭터의 itemCode 를 받아서 해당 아이템의 Index를 셋팅한다.
	///< 현재 입고있는 장비대로 BeautyShop 창을 설정하기 위해 사용
	@param bool bChange : 캐릭터에 적용여부
*/
int BeautyShopManager::SetDefaultCharSet(int _ItemCode, int _gender, bool bChanged)
{
	BS_CODE_PARSE psItem ;
	BeautyShopUnit* pBSItem = NULL ;
	BeautyShopUnit* pBSItemCol = NULL ;
	int		iColorSpecial ;
	int		iItemLayer ;

	m_iCurGender = _gender ;
	Parsing_CharCode(_ItemCode, &psItem) ;

	if( psItem.iLayerID == 13 )			iItemLayer = BS_HAIR ;
	else if( psItem.iLayerID == 14 )	iItemLayer = BS_EYE ;
	else								iItemLayer = BS_SKIN ;

	if( iItemLayer == BS_SKIN )
	{
		m_bLoadFail[iItemLayer] = false ;
		if( m_pBSGroup[iItemLayer]->GetGroupListCount(_gender) <= psItem.iSpecialID )
		{
			SetDefaultIndexSet(BS_SKIN) ;
			return EMPTY_NODE ;
		}
		pBSItem = m_pBSGroup[BS_SKIN]->FindItemByCode(_gender, psItem.iSpecialID, _ItemCode) ;
		if(pBSItem == NULL)
		{
			//DXUTOutputDebugString("■ERROR> SetDefaultCharSet : FindItemByCode ERROR code[%d]\n", _ItemCode);
			SetDefaultIndexSet(BS_SKIN) ;
			return EMPTY_NODE ;
		}
		m_iLayerIndex[BS_SKIN].InitCategory(pBSItem->m_iGroupID) ;
		m_iLayerIndex[BS_SKIN].InitDetail(pBSItem->m_iItemSeq-1) ;
	}
	else
	{
		m_bLoadFail[iItemLayer] = false ;
		m_bLoadFail[iItemLayer+1] = false ;
		if( m_pBSGroup[iItemLayer]->GetGroupListCount(_gender) <= psItem.iSpecialID )
		{
			SetDefaultIndexSet(iItemLayer) ;
			SetDefaultIndexSet(iItemLayer+1) ;
			return EMPTY_NODE ;
		}
		pBSItem =  ( psItem.iSpecialID < TYPE_EXPERT) ? \
			m_pBSGroup[iItemLayer]->FindItemByCode(_gender, psItem.iSpecialID, psItem.iCodeNC) : \
			m_pBSGroup[iItemLayer]->FindItemByCode(_gender, psItem.iSpecialID, _ItemCode) ;
		if(pBSItem == NULL)
		{
			//DXUTOutputDebugString("■ERROR> SetDefaultCharSet : FindItemByCode BS_HAIR ERROR code[%d]\n", psItem.iCodeNC);
			SetDefaultIndexSet(iItemLayer) ;
			SetDefaultIndexSet(iItemLayer+1) ;
			return EMPTY_NODE ;
		}
		if( bChanged ) {
			m_iLayerIndex[iItemLayer].InitCategory(pBSItem->m_iGroupID) ;
			m_iLayerIndex[iItemLayer].InitDetail(pBSItem->m_iItemSeq-1) ;
		}
		else
			m_iLayerIndex[iItemLayer].PreChange(pBSItem->m_iGroupID, pBSItem->m_iItemSeq-1) ;

		// Color
		m_bEnabled[iItemLayer+1] = ( m_iLayerIndex[iItemLayer].m_iCurCategoIndex < TYPE_EXPERT) ? true : false ;
		iColorSpecial = (psItem.iColor >= COLOR_SPECIAL )? 1 : 0 ;
		pBSItemCol = m_pBSGroup[iItemLayer+1]->FindItemByCode(_gender, iColorSpecial, psItem.iColor) ;
		if(pBSItemCol == NULL)
		{
			m_iLayerIndex[iItemLayer+1].InitCategory(0) ;
			m_iLayerIndex[iItemLayer+1].InitDetail(0) ;
			return EMPTY_NODE ;
		}

		if( bChanged ) {
			m_iLayerIndex[iItemLayer+1].InitCategory(pBSItemCol->m_iGroupID) ;
			m_iLayerIndex[iItemLayer+1].InitDetail(pBSItemCol->m_iItemSeq-1) ;
		}
		else
			m_iLayerIndex[iItemLayer+1].PreChange(pBSItemCol->m_iGroupID, pBSItemCol->m_iItemSeq-1) ;
	}

	return 0 ;
}



/**
	Category String을 가져온다.
	@param bool isCurrend : 현재 선택된 값을 가져올 것인지 여부
	@param int _layer : Index
	@param int _gender : Index
	@param int _catego : Index
*/
const char* BeautyShopManager::GetCategoryString(bool isCurrend, int _layer, int _gender, int _catego)				
{
	if( 0 > _catego || 0 > m_iLayerIndex[_layer].m_iCurCategoIndex)
		return m_pBSGroup[_layer]->GetGroupName(_gender, 0 );		///< Default 일반
		
	const char* pCategoryString = (isCurrend) ? \
		m_pBSGroup[_layer]->GetGroupName(m_iCurGender, m_iLayerIndex[_layer].m_iCurCategoIndex) : \
		m_pBSGroup[_layer]->GetGroupName(_gender, _catego);	

	return ( pCategoryString == NULL ) ? g_pResourceManager->GetGlobalString(9007024) : pCategoryString ;
}


/**
	Detail String을 가져온다.
	@param bool isCurrend : 현재 선택된 값을 가져올 것인지 여부
	@param int _layer : Index
	@param int _gender : Index
	@param int _catego : Index
	@param int _detail : Index
*/
const char* BeautyShopManager::GetDetailString(bool isCurrend, int _layer, int _gender, int _catego, int _detail)	
{	
	if( 0 > _detail || 0 > m_iLayerIndex[_layer].m_iCurDetailIndex || m_bLoadFail[_layer] == true )
		return g_pResourceManager->GetGlobalString(9007024) ;		///< Default GlobalString
	const char* pDetailString = (isCurrend) ? \
		m_pBSGroup[_layer]->GetItemNodeName(m_iCurGender, m_iLayerIndex[_layer].m_iCurCategoIndex, m_iLayerIndex[_layer].m_iCurDetailIndex) : \
		m_pBSGroup[_layer]->GetItemNodeName(_gender, _catego, _detail) ;	
	return ( pDetailString == NULL ) ? g_pResourceManager->GetGlobalString(9007024) : pDetailString ;
}




/**
	카테고리 Left 클릭
	@param unsigned int : Layer Index
	@return bool : 변경되었는지 여부
*/
bool BeautyShopManager::OnChangeCategoryLeft(unsigned int _layer)
{
	int iChangeIndex = 0 ;

	if( m_bEnabled[_layer] == false )		// 비활성화 되어 있다면 스킵
		return false ;

	iChangeIndex = m_iLayerIndex[_layer].m_iCurCategoIndex ;
	iChangeIndex--;
	if( iChangeIndex < 0 )
	{
		// 변경 사항 없음.
		return false ;
	}
	else
	{
		m_bLoadFail[_layer] = false ;
		m_iLayerIndex[_layer].m_iCurCategoIndex = iChangeIndex ;
		m_iLayerIndex[_layer].m_iCurDetailIndex = 0 ;	// 카테고리가 바뀌면 Detail 초기화
		if( m_iLayerIndex[_layer].m_iCurCategoIndex < TYPE_EXPERT )
		{
			if(_layer == BS_HAIR || _layer == BS_EYE) 
				m_bEnabled[_layer+1] = true ;		// 활성화
		}
	}

	return true ;
}


/**
	카테고리 Right 클릭
	@param unsigned int : Layer Index
	@return bool : 변경되었는지 여부
*/
bool BeautyShopManager::OnChangeCategoryRight(unsigned int _layer)
{
	int iChangeIndex = 0 ;
	int iTotalNum ;

	if( m_bEnabled[_layer] == false )		// 비활성화 되어 있다면 스킵
		return false ;

	// Group Category의 Count를 가져온다.
	iTotalNum = m_pBSGroup[_layer]->GetGroupListCount(m_iCurGender);
	iChangeIndex = m_iLayerIndex[_layer].m_iCurCategoIndex ;

	iChangeIndex++ ;
	if( iChangeIndex > iTotalNum-1 )
	{
		return false ;
	}
	else
	{
		m_bLoadFail[_layer] = false ;
		m_iLayerIndex[_layer].m_iCurCategoIndex = iChangeIndex ;
		m_iLayerIndex[_layer].m_iCurDetailIndex = 0 ;	// 카테고리가 바뀌면 Detail 초기화

		if( m_iLayerIndex[_layer].m_iCurCategoIndex > TYPE_SPECIAL ) {
			if(_layer == BS_HAIR || _layer == BS_EYE) 
				m_bEnabled[_layer+1] = false ;		// 비활성화
		}
	}

	return true ;
}


/**
	세부내용 Left 클릭
	@param unsigned int : Layer Index
	@return bool : 변경되었는지 여부
*/
bool BeautyShopManager::OnChangeDetailLeft(unsigned int _layer)
{
	int iChangeIndex = 0 ;

	if( m_bEnabled[_layer] == false )		// 비활성화 되어 있다면 스킵
		return false ;

	iChangeIndex = m_iLayerIndex[_layer].m_iCurDetailIndex ;
	iChangeIndex--;
	if( iChangeIndex < 0 )
	{
		//DXUTOutputDebugString("DetailLeft == Not Change");
		return false ;
	}
	else
	{
		m_bLoadFail[_layer] = false ;
		m_iLayerIndex[_layer].m_iCurDetailIndex = iChangeIndex ;
	}

	return true ;
}


/**
	세부내용 Right 클릭
	@param unsigned int : Layer Index
	@return bool : 변경되었는지 여부
*/

bool BeautyShopManager::OnChangeDetailRight(unsigned int _layer)
{
	int iChangeIndex = 0 ;
	int iTotalNum ;

	if( m_bEnabled[_layer] == false )		// 비활성화 되어 있다면 스킵
		return false ;

	iTotalNum = m_pBSGroup[_layer]->GetGroupCount(m_iCurGender, m_iLayerIndex[_layer].m_iCurCategoIndex);
	iChangeIndex = m_iLayerIndex[_layer].m_iCurDetailIndex ;
	iChangeIndex++;
	if( iChangeIndex > iTotalNum-1 )
	{
		//DXUTOutputDebugString("DetailRight == Not Change ");
		return false ;
	}
	else
	{
		m_bLoadFail[_layer] = false ;
		m_iLayerIndex[_layer].m_iCurDetailIndex = iChangeIndex ;
	}

	return true ;
}


/**
	아이템의 제한사항을 확인해서 이상이 없는지 체크한다.
	@param unsigned int : Layer Index
	@param CONTAINER_SLOT& : 체크후 서버에 보낼 Container_slot
	@param CONTAINER_SLOT& : 체크후 서버에 보낼 Container_slot
	@return int ERROR_CODE : 
*/
int BeautyShopManager::CommitItemValidCheck(int _layer, bool bChecked, CONTAINER_SLOT& _sendSlot1, CONTAINER_SLOT& _sendSlot2)
{
	BeautyShopUnit* selectItem = NULL ;
	SPItem* pSearchItem[2] = {NULL,} ;
	int iExpectCode = 0 ;

	//////////////////////////////////////////////////////////////////////////
	// 선택된 레이어의 아이템을 가져온다.
	selectItem =  GetSelectItem(_layer) ;
	if( selectItem == NULL )
	{
		//DXUTOutputDebugString("■ERROR> Can't Get Item Information") ;
		return ERROR_CANT_ITEMINFO ;
	}

	//////////////////////////////////////////////////////////////////////////
	// 전문가에서 일반으로 컬러의 변경은 안된다.
	if( _layer == BS_HAIR_C || _layer == BS_EYE_C)
	{
		// 스타일이 전문가에서 일반으로 바뀌고, 색상변경은 안된다.
		if( _layer == BS_HAIR_C && m_iLayerIndex[BS_HAIR].m_iPreCategoIndex > TYPE_SPECIAL && m_iLayerIndex[BS_HAIR].m_iCurCategoIndex < TYPE_EXPERT ||
			_layer == BS_EYE_C && m_iLayerIndex[BS_EYE].m_iPreCategoIndex > TYPE_SPECIAL && m_iLayerIndex[BS_EYE].m_iCurCategoIndex < TYPE_EXPERT ) 
		{
				return ERROR_COLOR_CHANGE ;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 아이템을 가지고 있는지 먼저 확인해야 한다. (NeedItem Check)
	SPPlayerInvenArchive* pkInvenArchive = g_pGOBManager->GetLocalPlayer()->GetInvenArchive() ;
	if(pkInvenArchive == NULL)
	return 0 ;

	if( selectItem->m_iNeedItemCode1 > 0 )
	{
		pSearchItem[0] = pkInvenArchive->GetSPItem(INVENTORY_EVENT, selectItem->m_iNeedItemCode1 ) ;
		if( pSearchItem[0] == NULL || pSearchItem[0]->GetSlotIndex() < 0 ) {
			return ERROR_HASNT_COUPON ;
		}
		_sendSlot1.ContainerType = pSearchItem[0]->GetContainerType() ;
		_sendSlot1.SlotIndex = pSearchItem[0]->GetSlotIndex() + 1 ;	// Server는 1Base
		if( selectItem->m_iNeedItemCode2 > 0 )
		{
			pSearchItem[1] = pkInvenArchive->GetSPItem(INVENTORY_EVENT, selectItem->m_iNeedItemCode2 ) ;
			if( pSearchItem[1] == NULL || pSearchItem[1]->GetSlotIndex() < 0 ) {
				return ERROR_HASNT_COUPON ;
			}
			_sendSlot2.ContainerType = pSearchItem[1]->GetContainerType() ;
			_sendSlot2.SlotIndex = pSearchItem[1]->GetSlotIndex() + 1;
		}
		else {
			_sendSlot2.ContainerType = CONTAINER_TYPE_INVALID ;
			_sendSlot2.SlotIndex = 0 ;
		}
	}
	else
	{
		_sendSlot1.ContainerType = CONTAINER_TYPE_INVALID ;
		_sendSlot1.SlotIndex = 0 ;
		_sendSlot2.ContainerType = CONTAINER_TYPE_INVALID ;
		_sendSlot2.SlotIndex = 0 ;
	}

	//////////////////////////////////////////////////////////////////////////
	// 아이템 제한사항이 걸리는지 확인한다.
	iExpectCode = GetExpectItemCode(_layer) ;
	if(g_pCheckManager->CheckItemStatus(iExpectCode) == false) {
		return ERROR_ITEM_LIMIT ;
	}

	//////////////////////////////////////////////////////////////////////////
	// 전문가에서 일반으로 바뀌면 선택창을 띄운다.
	if( (_layer == BS_HAIR || _layer == BS_EYE) && bChecked == false )
	{
		if( m_iLayerIndex[_layer].m_iPreCategoIndex > TYPE_SPECIAL && m_iLayerIndex[_layer].m_iCurCategoIndex < TYPE_EXPERT ) {
			return WARNING_EXPERT_CHANGE ;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// 입고 있는지 확인 - 이미 입고 있으면 구입할수 없다.
	if( m_iLayerIndex[_layer].m_iPreCategoIndex == m_iLayerIndex[_layer].m_iCurCategoIndex &&
		m_iLayerIndex[_layer].m_iPreDetailIndex == m_iLayerIndex[_layer].m_iCurDetailIndex)
		return ERROR_SAME_CHANGE ;
	
	return iExpectCode ;
}

			
/**
	해당 Layer의 Expect Item Code를 리턴
	@param int _layer : 레이어
	@return int : Expect Item code
*/
int BeautyShopManager::GetExpectItemCode(int _layer)
{
	int iExpectCode = 0 ;
	int iPreCode = 0 ;
	if( m_bEnabled[_layer] == false ) {
		//DXUTOutputDebugString("GetExpectItemCode =====> DISABLE\n") ;
		return EMPTY_NODE ;
	}

	if( _layer == BS_HAIR_C || _layer == BS_EYE_C) {
		// 변경될 코드값 (이전 스타일 + 현재 컬러 )
		if( m_iLayerIndex[_layer-1].m_iPreCategoIndex > TYPE_SPECIAL )
			return EMPTY_NODE ;
		if( m_pBSGroup[_layer-1]->GetItemNode(m_iCurGender, m_iLayerIndex[_layer-1].m_iPreCategoIndex, m_iLayerIndex[_layer-1].m_iPreDetailIndex) )
			iPreCode = m_pBSGroup[_layer-1]->GetItemNode(m_iCurGender, m_iLayerIndex[_layer-1].m_iPreCategoIndex, m_iLayerIndex[_layer-1].m_iPreDetailIndex)->m_iItemCode ;
		iExpectCode = iPreCode + GetSelectItem(_layer)->m_iItemCode ;
	}
	else if( _layer == BS_SKIN ) {
		iExpectCode = GetSelectItem(_layer)->m_iItemCode ;
	}
	else
	{
		if( m_iLayerIndex[_layer].m_iCurCategoIndex < TYPE_EXPERT )
		{
			if( m_iLayerIndex[_layer].m_iPreCategoIndex > TYPE_SPECIAL ) {		// 전문가 -> 일반 : 현재 스타일 + 전문가 컬러
				if( m_pBSGroup[_layer]->GetItemNode(m_iCurGender, m_iLayerIndex[_layer].m_iPreCategoIndex, m_iLayerIndex[_layer].m_iPreDetailIndex) )
					iPreCode = (m_pBSGroup[_layer]->GetItemNode(m_iCurGender, m_iLayerIndex[_layer].m_iPreCategoIndex, m_iLayerIndex[_layer].m_iPreDetailIndex)->m_iItemCode) % 100 ;
				iExpectCode =  GetSelectItem(_layer)->m_iItemCode + iPreCode ;
				//DXUTOutputDebugString("★전문가에서 변경됨 : [%d] = %d + %d\n", iExpectCode, GetSelectItem(_layer)->m_iItemCode , iPreCode) ;
			}
			else {				// 일반 -> 일반 : 현재 스타일 + 이전 컬러
				if( m_pBSGroup[_layer+1]->GetItemNode(m_iCurGender, m_iLayerIndex[_layer+1].m_iPreCategoIndex, m_iLayerIndex[_layer+1].m_iPreDetailIndex) )
					iPreCode = m_pBSGroup[_layer+1]->GetItemNode(m_iCurGender, m_iLayerIndex[_layer+1].m_iPreCategoIndex, m_iLayerIndex[_layer+1].m_iPreDetailIndex)->m_iItemCode ;
				iExpectCode =  GetSelectItem(_layer)->m_iItemCode + iPreCode ;
			}
		}
		else	
		{
			// 일반 -> 전문가 : 전문가 코드
			// 스페셜 -> 전문가 : 전문가 코드
			iExpectCode =  GetSelectItem(_layer)->m_iItemCode ;
		}
	}
	return iExpectCode ;
}



/**
	해당 layer가 활성화 상태인지 리턴
	@param int _layer : 레이어
	@return bool : Enable여부
*/
bool BeautyShopManager::GetCategoryEnable(int _layer)
{
	return m_bEnabled[_layer] ;	
}


/**
	해당 Layer의 현재 선택된 ItemUnit 내용을 리턴한다.
	@param int _layer : 레이어
	@return BeautyShopUnit* : Unit Pointer
*/
BeautyShopUnit* BeautyShopManager::GetSelectItem(int _layer)
{
	return (m_pBSGroup[_layer]->GetItemNode(m_iCurGender, m_iLayerIndex[_layer].m_iCurCategoIndex, m_iLayerIndex[_layer].m_iCurDetailIndex )) ;
}


/**
	해당 gender & category의 카운트를 돌려준다.
	@param int _gender : 성별
	@param int _categ ; 카테고리
	@return int : 카운트
*/
int BeautyShopManager::GetHairStyleCount(int _gender, int _categ)	
{	
	return (_categ < 0) ? \
		m_pBSGroup[BS_HAIR]->GetGroupListCount(_gender) : \
		m_pBSGroup[BS_HAIR]->GetGroupCount(_gender, _categ) ;	
}

int BeautyShopManager::GetHairColorCount(int _gender, int _categ)  
{
	return (_categ < 0) ? \
		m_pBSGroup[BS_HAIR_C]->GetGroupListCount(_gender) : \
		m_pBSGroup[BS_HAIR_C]->GetGroupCount(_gender, _categ) ;	
}

int BeautyShopManager::GetEyeStyleCount(int _gender, int _categ)	
{
	return (_categ < 0) ? \
		m_pBSGroup[BS_EYE]->GetGroupListCount(_gender): \
		m_pBSGroup[BS_EYE]->GetGroupCount(_gender, _categ) ;	
}

int BeautyShopManager::GetEyecolorCount(int _gender, int _categ)	
{
	return (_categ < 0) ? \
		m_pBSGroup[BS_EYE_C]->GetGroupListCount(_gender) : \
		m_pBSGroup[BS_EYE_C]->GetGroupCount(_gender, _categ) ;	
}

int BeautyShopManager::GetSkinCount(int _gender, int _categ)		
{
	return (_categ < 0) ? \
		m_pBSGroup[BS_SKIN]->GetGroupListCount(_gender) : \
		m_pBSGroup[BS_SKIN]->GetGroupCount(_gender, _categ) ;	
}


int BeautyShopManager::GetLayerCount(int _gender, int _layer, int _categ)
{
	switch(_layer)
	{
	case BS_HAIR:
		return GetHairStyleCount(_gender, _categ) ;
		break ;
	case BS_HAIR_C:
		return GetHairColorCount(_gender, _categ) ;
		break ;
	case BS_EYE:
		return GetEyeStyleCount(_gender, _categ) ;
		break ;
	case BS_EYE_C:
		return GetEyecolorCount(_gender, _categ) ;
		break ;
	case BS_SKIN:
		return GetSkinCount(_gender, _categ) ;
		break ;
	}
	return -1 ;
}

/**
	BeautyShop LDT Data를 검증하는 루틴
	# 중요 : Sequence 값이 Serial해야 한다.
	1) Data 가 NULL인지 여부 체크
	2) Sequence값이 Serial인지 체크
*/
bool BeautyShopManager::VerifyBeautyShopLDT()
{
	int iGender ;
	int iCatgCount ;
	int iItemCount ;
	BeautyShopUnit* pItem = NULL ;

	// 성별
	for( int i = 0; i < 2; i++)
	{
		iGender = i ;

		//	Layer (헤어, 헤어컬러, 눈, 눈컬러, 스킨)
		for( int iLayer = BS_HAIR; iLayer < BS_ITEM_MAX; iLayer++)
		{
			// 카테고리 (일반, 스페셜, 전문가...)
			iCatgCount = GetLayerCount(iGender, iLayer) ;
			for( int iCatg = 0; iCatg < iCatgCount; iCatg++ )
			{
				// 세부 아이템
				iItemCount = GetLayerCount(iGender, iLayer, iCatg) ;
				for( int k = 0; k < iItemCount; k++ )
				{
					pItem = m_pBSGroup[iLayer]->GetItemNode(iGender, iCatg, k ) ;
					if( pItem == NULL )
					{
						ErrorFLogs->Error_Log("◈ERROR:Gender[%d], Layer[%d], Categ[%d/%d], Detail[%d/%d] is NULL", iGender, iLayer, iCatg, iCatgCount, k, iItemCount ) ;
						continue ;
					}
					if( pItem->m_iItemSeq != k+1 )
					{
						ErrorFLogs->Error_Log( "◈ERROR:Item[%d] Name[%s] Sequence is Not Serial[%d:%d/%d]", pItem->m_iItemCode, pItem->m_strItemName.c_str(), pItem->m_iItemSeq, k+1, iItemCount) ;
						continue ;
					}
					//ErrorFLogs->Error_Log( ">> PASS:Item[%d] Name[%s] Sequence is [%d:%d/%d]", pItem->m_iItemCode, pItem->m_strItemName.c_str(), pItem->m_iItemSeq, k+1, iItemCount) ;
				}
			}
		}
	}
	return true ;
}