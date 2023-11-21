
#include "SPWindowBeautyShopDataUnit.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/**
	BeautyShop Item Group
	///< 일반, 스페셜, 전문가... 등에 해당됨
*/

BeautyShopItemGroup::BeautyShopItemGroup(int _groupCode, int _groupIndex, const char* _strGroupName) 
:m_iGroupCode(_groupCode), m_iGroupIndex(_groupIndex), m_strGroupName(_strGroupName)
{
}


BeautyShopItemGroup::~BeautyShopItemGroup()	
{	
	Clean();	
}


void BeautyShopItemGroup::Clean()
{
	std::vector<BeautyShopUnit*>::iterator Iter = m_vpItemNode.begin();
	for(;Iter != m_vpItemNode.end(); ++Iter) {
		SAFE_DELETE(*Iter);
	}
	m_vpItemNode.clear();
}


//////////////////////////////////////////////////////////////////////////
///< Item 추가
bool BeautyShopItemGroup::AddItemNode(int _itemID, int _itemCode, int _layer, int _groupID, int _itemSeq, const char* _strItemName, int _needItemCode1, int _needItemCode2) 
{
	BeautyShopUnit* pInItem = new BeautyShopUnit(_itemID, _itemCode, _layer, _groupID, _itemSeq, _strItemName, _needItemCode1, _needItemCode2) ;
	if( _itemSeq < (int)m_vpItemNode.size() )
	{
		// Sequence 에 따라 정렬해 주어야 함.
		BeautyShopUnit* pItem = NULL ;
		std::vector<BeautyShopUnit*>::iterator Iter = m_vpItemNode.begin();
		for(;Iter != m_vpItemNode.end(); ++Iter) {
			pItem = (*Iter) ;
			if( pItem->m_iItemSeq > _itemSeq )
			{
				m_vpItemNode.insert(Iter, pInItem) ;
				//DXUTOutputDebugString("AdditemNode Insert[%d]-seq[%d] : [%d, %d]", i, _itemSeq, _itemID, _itemCode ) ;
				return true ;
			}
		}
		m_vpItemNode.push_back(pInItem) ;
	}
	else
		m_vpItemNode.push_back(pInItem) ;

	return true ;
}


//////////////////////////////////////////////////////////////////////////
///< ItemID로 해당 아이템을 찾는다.
BeautyShopUnit* BeautyShopItemGroup::FindItemByID(int _itemID)
{
	BeautyShopUnit* pItem = NULL ;
	std::vector<BeautyShopUnit*>::iterator Iter = m_vpItemNode.begin();
	for(;Iter != m_vpItemNode.end(); ++Iter) {
		pItem = (*Iter) ;
		if( pItem->m_iItemID == _itemID )	{
			return pItem ;
		}
	}
	return NULL ;
}


//////////////////////////////////////////////////////////////////////////
///< ItemCode로 해당 아이템을 찾는다.
BeautyShopUnit* BeautyShopItemGroup::FindItemByCode(int _itemCode)
{
	BeautyShopUnit* pItem = NULL ;
	std::vector<BeautyShopUnit*>::iterator Iter = m_vpItemNode.begin();
	for(;Iter != m_vpItemNode.end(); ++Iter) {
		pItem = (*Iter) ;
		if( pItem->m_iItemCode == _itemCode )	{
			return pItem ;
		}
	}
	return NULL ;
}


//////////////////////////////////////////////////////////////////////////
///< ItemCode로 해당 아이템을 찾는다.
int BeautyShopItemGroup::FindItemIndexByCode(int _itemCode)
{
	int iIndex = 0 ;
	std::vector<BeautyShopUnit*>::iterator Iter = m_vpItemNode.begin();
	for(;Iter != m_vpItemNode.end(); ++Iter) {
		if( (*Iter)->m_iItemCode == _itemCode )	{
			return iIndex ;
		}
		iIndex++ ;
	}
	return 0 ;
}


//////////////////////////////////////////////////////////////////////////
///< Item의 Index로 ItemNode의 포인터를 얻어온다.
const char*	BeautyShopItemGroup::GetGroupName()				
{	
	return m_strGroupName.c_str();	
}

int		BeautyShopItemGroup::GetGroupCount()					
{	
	return (int)m_vpItemNode.size();	
}

int		BeautyShopItemGroup::GetGroupCode()					
{	
	return m_iGroupCode;	
}

int		BeautyShopItemGroup::GetGroupIndex()					
{	
	return m_iGroupIndex;	
}

//////////////////////////////////////////////////////////////////////////
//
//	Sequence를 이용한 Indexing
//	Sequence는 무조건 이어져야 한다.
//
BeautyShopUnit*	BeautyShopItemGroup::GetItemNode(int _index)	
{	
	return (_index > EMPTY_NODE && _index < (int)m_vpItemNode.size())?m_vpItemNode.at(_index) : NULL ;	
}

const char*		BeautyShopItemGroup::GetItemName(int _index)	
{	
	return (_index > EMPTY_NODE && _index < (int)m_vpItemNode.size())?m_vpItemNode.at(_index)->m_strItemName.c_str() : NULL ;	
}
// Index Search -> Find Search 수정
// SEQ를 사용하므로 그대로 Index 사용
//BeautyShopUnit*	BeautyShopItemGroup::GetItemNode(int _index)	
//{	
//	int iSize = (int)m_vpItemNode.size() ;
//	if( iSize < 1 )		return NULL ;
//	for( int i = 0; i < iSize; i++ )
//	{
//		int iCode = m_vpItemNode.at(i)->m_iItemCode ;
//		if(iCode % 100 == _index)
//			return m_vpItemNode.at(i) ;
//	}
//	return NULL ;	
//}
//
//const char*		BeautyShopItemGroup::GetItemName(int _index)	
//{	
//	int iSize = (int)m_vpItemNode.size() ;
//	if( iSize < 1 )		return NULL ;
//	for( int i = 0; i < iSize; i++ )
//	{
//		int iCode = m_vpItemNode.at(i)->m_iItemCode ;
//		if( iCode % 100 == _index)
//			return m_vpItemNode.at(i)->m_strItemName.c_str() ;
//	}
//	return NULL ;	
//}




//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/**
	BeautyShop Item GroupList 
	///< 헤어스타일, 헤어 컬러 등등에 해당됨
*/

BeautyShopItemGroupList::BeautyShopItemGroupList()	
{	
	Init();	
}


BeautyShopItemGroupList::~BeautyShopItemGroupList()	
{	
	Clean();	
}


void BeautyShopItemGroupList::Init()	
{	
	m_vpItemGroup[0].clear();
	m_vpItemGroup[1].clear();
}


void BeautyShopItemGroupList::Clean()
{
	for(int i = 0; i < 2; i++)
	{
		std::vector<BeautyShopItemGroup*>::iterator Iter = m_vpItemGroup[i].begin();
		for(;Iter != m_vpItemGroup[i].end(); ++Iter) {
			(*Iter)->Clean() ;
			SAFE_DELETE(*Iter);
		}
		m_vpItemGroup[i].clear();
	}
}


/**
	아이템 추가 : 그룹이 없는경우 그룹을 추가해서 넣도록 한다.
	@param int _gender: 성별
	@param int _groupID : 그룹 ID
	@param int _groupName : 그룹 이름
	@param BeautyShopUnit* _pItem : 추가될 아이템 클래스
*/
bool BeautyShopItemGroupList::AddItem(int _gender, int _layer, int _groupID, const char* _groupName, int _itemID, int _itemCode, int _itemSeq, const char* _itemName, int _needItemCode1, int _needItemCode2)
{
	int _groupIdx ;

	if( _groupID < (int)m_vpItemGroup[_gender].size() )
	{
		if( (_groupIdx = GetGroupIndex(_gender, _groupID)) < 0 ) 
			return false ;	
		return m_vpItemGroup[_gender].at(_groupIdx)->AddItemNode(_itemID, _itemCode, _layer, _groupID, _itemSeq, _itemName, _needItemCode1, _needItemCode2) ;
	}
	else
	{
		_groupIdx = (int)m_vpItemGroup[_gender].size() ;
		BeautyShopItemGroup* pGroup = new BeautyShopItemGroup(_groupID, _groupIdx, _groupName) ;
		m_vpItemGroup[_gender].push_back(pGroup) ;
		return m_vpItemGroup[_gender].at(_groupIdx)->AddItemNode(_itemID, _itemCode, _layer, _groupID, _itemSeq, _itemName, _needItemCode1, _needItemCode2) ;
	}
}


/**
	아이템 찾기
	@param int _gender : 성별
	@param int _groupID : 그룹 ID
	@param int _itemCode : 아이템 코드
*/
BeautyShopUnit* BeautyShopItemGroupList::FindItemByCode(int _gender, int _groupID, int _itemCode)	
{	
	//int _groupIdx ;
	//if( (_groupIdx = GetGroupIndex(_gender, _groupID)) < 0 ) 
	//	return NULL ;
	return m_vpItemGroup[_gender].at(_groupID)->FindItemByCode(_itemCode) ; 
}

BeautyShopUnit* BeautyShopItemGroupList::FindItemByID(int _gender, int _groupID, int _itemID)		
{	
	//int _groupIdx ;
	//if( (_groupIdx = GetGroupIndex(_gender, _groupID)) < 0 ) 
	//	return NULL ;
	return m_vpItemGroup[_gender].at(_groupID)->FindItemByID(_itemID) ; 
}

BeautyShopUnit* BeautyShopItemGroupList::FindItemByIndex(int _gender, int _groupID, int _itemIndex)	
{	
	//int _groupIdx ;
	//if( (_groupIdx = GetGroupIndex(_gender, _groupID)) < 0 ) 
	//	return NULL ;
	return m_vpItemGroup[_gender].at(_groupID)->GetItemNode(_itemIndex) ; 
}

//////////////////////////////////////////////////////////////////////////
///< 아이템 코드로 아이템의 Index를 리턴
int BeautyShopItemGroupList::FindItemIndexByCode(int _gender, int _groupID, int _itemCode)			
{	
	//int _groupIdx ;
	//if( (_groupIdx = GetGroupIndex(_gender, _groupID)) < 0 ) 
	//	return -1 ;
	return m_vpItemGroup[_gender].at(_groupID)->FindItemIndexByCode(_itemCode) ; 
}

BeautyShopUnit*	BeautyShopItemGroupList::GetItemNode(int _gender, int _groupID, int _detail)			
{	
	//int _groupIdx ;
	//if( (_groupIdx = GetGroupIndex(_gender, _groupID)) < 0 ) 
	//	return NULL ;
	return m_vpItemGroup[_gender].at(_groupID)->GetItemNode(_detail) ;	
}

const char*	BeautyShopItemGroupList::GetItemNodeName(int _gender, int _groupID, int _detail)			
{	
	//int _groupIdx ;
	//if( (_groupIdx = GetGroupIndex(_gender, _groupID)) < 0 ) 
	//	return NULL ;
	return  m_vpItemGroup[_gender].at(_groupID)->GetItemName(_detail) ;

}

//////////////////////////////////////////////////////////////////////////
///< Group 관련
const char*	BeautyShopItemGroupList::GetGroupName(int _gender, int _groupID)			
{	
	//int _groupIdx ;
	//if( (_groupIdx = GetGroupIndex(_gender, _groupID)) < 0 ) 
	//	return NULL ;
	return m_vpItemGroup[_gender].at(_groupID)->GetGroupName();	
}

int			BeautyShopItemGroupList::GetGroupCount(int _gender, int _groupID)		
{	
	//int _groupIdx ;
	//if( (_groupIdx = GetGroupIndex(_gender, _groupID)) < 0 ) 
	//	return -1 ;
	return m_vpItemGroup[_gender].at(_groupID)->GetGroupCount();	
}

int			BeautyShopItemGroupList::GetGroupCode(int _gender, int _groupID)			
{	
	//int _groupIdx ;
	//if( (_groupIdx = GetGroupIndex(_gender, _groupID)) < 0 ) 
	//	return -1 ;
	return m_vpItemGroup[_gender].at(_groupID)->GetGroupCode();	
}

//////////////////////////////////////////////////////////////////////////
///< GroupList 관련
int		BeautyShopItemGroupList::GetGroupListCount(int _gender)			
{	
	return (int)m_vpItemGroup[_gender].size();	
}

int		BeautyShopItemGroupList::GetGroupIndex(int _gender, int _groupID)				
{	
	std::vector<BeautyShopItemGroup*>::iterator Iter = m_vpItemGroup[_gender].begin();
	for(;Iter != m_vpItemGroup[_gender].end(); ++Iter) {
		if( (*Iter)->GetGroupCode() == _groupID )
			return (*Iter)->GetGroupIndex() ;
	}
	return -1 ; 
}

