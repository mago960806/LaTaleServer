
#include <string>
#include <vector>

#ifndef EMPTY_NODE
#define EMPTY_NODE	-1
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif




/**
	BeautyShop Item
	///< ItemUNIT에 해당 됨
*/
class BeautyShopUnit
{
public:
	BeautyShopUnit() {}
	BeautyShopUnit(int _itemID, int _itemCode, int _layer, int _groupID, int _itemSeq, const char* _strItemName, int _needItemCode1, int _needItemCode2) \
		:m_iItemID(_itemID), m_iItemCode(_itemCode), m_iItemLayer(_layer), m_iGroupID(_groupID), m_iItemSeq(_itemSeq), m_iNeedItemCode1(_needItemCode1), m_iNeedItemCode2(_needItemCode2), m_strItemName(_strItemName){}
	virtual ~BeautyShopUnit()  {}
	
	void SetItemData(int _itemID, int _itemCode, int _layer, int _groupID, int _itemSeq, char* _strItemName, int _needItemCode1, int _needItemCode2 = 0)
	{
		m_iItemID = _itemID ;
		m_iItemCode = _itemCode ;
		m_iItemLayer = _layer ;
		m_iGroupID = m_iGroupID;
		m_iItemSeq = _itemSeq ;
		m_iNeedItemCode1 = _needItemCode1 ;
		m_iNeedItemCode2 = _needItemCode2 ;
		m_strItemName = _strItemName ;
	}
	
	void operator=(const BeautyShopUnit& Src) {
		m_iItemID = Src.m_iItemID;
		m_iItemCode = Src.m_iItemCode;
		m_iItemLayer = Src.m_iItemLayer ;
		m_iGroupID = Src.m_iGroupID;
		m_iItemSeq = Src.m_iItemSeq;
		m_strItemName = Src.m_strItemName.c_str();
		m_iNeedItemCode1 = Src.m_iNeedItemCode1;
		m_iNeedItemCode2 = Src.m_iNeedItemCode2;
	}
	bool operator==(const BeautyShopUnit& Src) {
		return ( m_iItemCode == Src.m_iItemCode ) ? true : false ;
	}

public:
	int m_iItemID ;				///< Item ID
	int m_iItemCode ;			///< Item Code
	int m_iItemLayer;			///< Hair, Color, Eye
	int m_iGroupID ;			///< Normal, Special, Expert
	int m_iItemSeq ;			///< Item Sequence Number
	int m_iNeedItemCode1 ;	///< Require Code
	int m_iNeedItemCode2 ;	///< Require Code
	std::string	m_strItemName ;	///< Name
};



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/**
BeautyShop Item Group
///< 일반, 스페셜, 전문가... 등에 해당됨
*/
class BeautyShopItemGroup
{
public:
	BeautyShopItemGroup(int _groupCode, int _groupIndex, const char* _strGroupName);
	virtual ~BeautyShopItemGroup();

	void Clean();


	///< Item 추가
	bool AddItemNode(int _itemID, int _itemCode, int _layer, int _groupID, int _itemSeq, const char* _strItemName, int _needItemCode1, int _needItemCode2=0) ;


	///< ItemID로 해당 아이템을 찾는다.
	BeautyShopUnit* FindItemByID(int _itemID);
	///< ItemCode로 해당 아이템을 찾는다.
	BeautyShopUnit* FindItemByCode(int _itemCode);
	///< ItemCode로 해당 아이템을 찾는다.
	int FindItemIndexByCode(int _itemCode);


	///< Item의 Index로 ItemNode의 포인터를 얻어온다.
	const char*	GetGroupName();
	int		GetGroupCount();
	int		GetGroupCode();
	int		GetGroupIndex();

	BeautyShopUnit*	GetItemNode(int _index);
	const char*		GetItemName(int _index);

public:
	int				m_iGroupCode ;
	int				m_iGroupIndex ;
	std::string		m_strGroupName ;
	std::vector<BeautyShopUnit*>	m_vpItemNode;
};



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/**
BeautyShop Item GroupList 
///< 헤어스타일, 헤어 컬러 등등에 해당됨
*/
class BeautyShopItemGroupList
{
public:
	BeautyShopItemGroupList();
	virtual ~BeautyShopItemGroupList();

	virtual void Init()	;
	void Clean();


	/**
	아이템 추가 : 그룹이 없는경우 그룹을 추가해서 넣도록 한다.
	@param int _gender: 성별
	@param int _groupID : 그룹 ID
	@param int _groupName : 그룹 이름
	@param BeautyShopUnit* _pItem : 추가될 아이템 클래스
	*/
	bool AddItem(int _gender, int _layer, int _groupID, const char* _groupName, int _itemID, int _itemCode, int _itemSeq, const char* _itemName, int _needItemCode1, int _needItemCode2=0);


	/**
	아이템 찾기
	@param int _gender : 성별
	@param int _groupID : 그룹 ID
	@param int _itemCode : 아이템 코드
	*/
	BeautyShopUnit* FindItemByCode(int _gender, int _groupID, int _itemCode);
	BeautyShopUnit* FindItemByID(int _gender, int _groupID, int _itemID);	
	BeautyShopUnit* FindItemByIndex(int _gender, int _groupID, int _itemIndex);

	///< 아이템 코드로 아이템의 Index를 리턴
	int FindItemIndexByCode(int _gender, int _groupID, int _itemCode);		


	BeautyShopUnit*	GetItemNode(int _gender, int _groupID, int _detail);
	const char*	GetItemNodeName(int _gender, int _groupID, int _detail);			


	///< Group 관련
	const char*	GetGroupName(int _gender, int _groupID);
	int			GetGroupCount(int _gender, int _groupID);
	int			GetGroupCode(int _gender, int _groupID);			

	///< GroupList 관련
	int		GetGroupListCount(int _gender);
	int		GetGroupIndex(int _gender, int _groupID);

private:
	std::vector<BeautyShopItemGroup*>	m_vpItemGroup[2];
};





////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
///**
//	BeautyShop Item Group
//	///< 일반, 스페셜, 전문가... 등에 해당됨
//*/
//class BeautyShopItemGroup
//{
//public:
//	BeautyShopItemGroup(int _groupCode, int _groupIndex, const char* _strGroupName) \
//		:m_iGroupCode(_groupCode), m_iGroupIndex(_groupIndex), m_strGroupName(_strGroupName){}
//	virtual ~BeautyShopItemGroup()	{	Clean();	}
//
//	void Clean()
//	{
//		std::vector<BeautyShopUnit*>::iterator Iter = m_vpItemNode.begin();
//		for(;Iter != m_vpItemNode.end(); ++Iter) {
//				SAFE_DELETE(*Iter);
//			}
//		m_vpItemNode.clear();
//	}
//
//	///< Item 추가
//	bool AddItemNode(int _itemID, int _itemCode, int _layer, int _groupID, int _itemSeq, const char* _strItemName, int _needItemCode1, int _needItemCode2=0) 
//	{
//		BeautyShopUnit* pInItem = new BeautyShopUnit(_itemID, _itemCode, _layer, _groupID, _itemSeq, _strItemName, _needItemCode1, _needItemCode2) ;
//		if( _itemSeq < (int)m_vpItemNode.size() )
//		{
//			// Sequence 에 따라 정렬해 주어야 함.
//			BeautyShopUnit* pItem = NULL ;
//			std::vector<BeautyShopUnit*>::iterator Iter = m_vpItemNode.begin();
//			for(;Iter != m_vpItemNode.end(); ++Iter) {
//				pItem = (*Iter) ;
//				if( pItem->m_iItemSeq > _itemSeq )
//				{
//					m_vpItemNode.insert(Iter, pInItem) ;
//					//DXUTOutputDebugString("AdditemNode Insert[%d]-seq[%d] : [%d, %d]", i, _itemSeq, _itemID, _itemCode ) ;
//					return true ;
//				}
//			}
//			m_vpItemNode.push_back(pInItem) ;
//		}
//		else
//			m_vpItemNode.push_back(pInItem) ;
//			
//		return true ;
//	}
//
//
//	///< ItemID로 해당 아이템을 찾는다.
//	BeautyShopUnit* FindItemByID(int _itemID)
//	{
//		BeautyShopUnit* pItem = NULL ;
//		std::vector<BeautyShopUnit*>::iterator Iter = m_vpItemNode.begin();
//		for(;Iter != m_vpItemNode.end(); ++Iter) {
//			pItem = (*Iter) ;
//			if( pItem->m_iItemID == _itemID )	{
//				return pItem ;
//			}
//		}
//		return NULL ;
//	}
//
//	///< ItemCode로 해당 아이템을 찾는다.
//	BeautyShopUnit* FindItemByCode(int _itemCode)
//	{
//		BeautyShopUnit* pItem = NULL ;
//		std::vector<BeautyShopUnit*>::iterator Iter = m_vpItemNode.begin();
//		for(;Iter != m_vpItemNode.end(); ++Iter) {
//			pItem = (*Iter) ;
//			if( pItem->m_iItemCode == _itemCode )	{
//				return pItem ;
//			}
//		}
//		return NULL ;
//	}
//
//	///< ItemCode로 해당 아이템을 찾는다.
//	int FindItemIndexByCode(int _itemCode)
//	{
//		int iIndex = 0 ;
//		std::vector<BeautyShopUnit*>::iterator Iter = m_vpItemNode.begin();
//		for(;Iter != m_vpItemNode.end(); ++Iter) {
//			if( (*Iter)->m_iItemCode == _itemCode )	{
//				return iIndex ;
//			}
//			iIndex++ ;
//		}
//		return 0 ;
//	}
//	
//
//	///< Item의 Index로 ItemNode의 포인터를 얻어온다.
//	const char*	GetGroupName()				{	return m_strGroupName.c_str();	}
//	int		GetGroupCount()					{	return (int)m_vpItemNode.size();	}
//	int		GetGroupCode()					{	return m_iGroupCode;	}
//
//	BeautyShopUnit*	GetItemNode(int _index)	{	return (_index > EMPTY_NODE && _index < (int)m_vpItemNode.size())?m_vpItemNode.at(_index) : NULL ;	}
//	const char*		GetItemName(int _index)	{	return (_index > EMPTY_NODE && _index < (int)m_vpItemNode.size())?m_vpItemNode.at(_index)->m_strItemName.c_str() : NULL ;	}
//
//public:
//	int				m_iGroupCode ;
//	int				m_iGroupIndex ;
//	std::string		m_strGroupName ;
//	std::vector<BeautyShopUnit*>	m_vpItemNode;
//};
//
//
//
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
///**
//	BeautyShop Item GroupList 
//	///< 헤어스타일, 헤어 컬러 등등에 해당됨
//*/
//class BeautyShopItemGroupList
//{
//public:
//	BeautyShopItemGroupList()			{	Init();	}
//	virtual ~BeautyShopItemGroupList()	{	Clean();	}
//
//	virtual void Init()	
//	{	
//		m_vpItemGroup[0].clear();
//		m_vpItemGroup[1].clear();
//	}
//	void Clean()
//	{
//		for(int i = 0; i < 2; i++)
//		{
//			std::vector<BeautyShopItemGroup*>::iterator Iter = m_vpItemGroup[i].begin();
//			for(;Iter != m_vpItemGroup[i].end(); ++Iter) {
//				(*Iter)->Clean() ;
//				SAFE_DELETE(*Iter);
//			}
//			m_vpItemGroup[i].clear();
//		}
//	}
//
//	/**
//		아이템 추가 : 그룹이 없는경우 그룹을 추가해서 넣도록 한다.
//		@param int _gender: 성별
//		@param int _groupID : 그룹 ID
//		@param int _groupName : 그룹 이름
//		@param BeautyShopUnit* _pItem : 추가될 아이템 클래스
//	*/
//	bool AddItem(int _gender, int _layer, int _groupID, const char* _groupName, int _itemID, int _itemCode, int _itemSeq, const char* _itemName, int _needItemCode1, int _needItemCode2=0)
//	{
//		if( _groupID < (int)m_vpItemGroup[_gender].size() )
//		{
//			return m_vpItemGroup[_gender].at(_groupID)->AddItemNode(_itemID, _itemCode, _layer, _groupID, _itemSeq, _itemName, _needItemCode1, _needItemCode2) ;
//		}
//		else
//		{
//			BeautyShopItemGroup* pGroup = new BeautyShopItemGroup(_groupID, (int)m_vpItemGroup[_gender].size(), _groupName) ;
//			m_vpItemGroup[_gender].push_back(pGroup) ;
//			return m_vpItemGroup[_gender].at(_groupID)->AddItemNode(_itemID, _itemCode, _layer, _groupID, _itemSeq, _itemName, _needItemCode1, _needItemCode2) ;
//		}
//	}
//	
//
//	/**
//		아이템 찾기
//		@param int _gender : 성별
//		@param int _groupID : 그룹 ID
//		@param int _itemCode : 아이템 코드
//	*/
//	BeautyShopUnit* FindItemByCode(int _gender, int _groupID, int _itemCode)	{	return m_vpItemGroup[_gender].at(_groupID)->FindItemByCode(_itemCode) ; }
//	BeautyShopUnit* FindItemByID(int _gender, int _groupID, int _itemID)		{	return m_vpItemGroup[_gender].at(_groupID)->FindItemByID(_itemID) ; }
//	BeautyShopUnit* FindItemByIndex(int _gender, int _groupID, int _itemIndex)	{	return m_vpItemGroup[_gender].at(_groupID)->GetItemNode(_itemIndex) ; }
//	///< 아이템 코드로 아이템의 Index를 리턴
//	int FindItemIndexByCode(int _gender, int _groupID, int _itemCode)			{	return m_vpItemGroup[_gender].at(_groupID)->FindItemIndexByCode(_itemCode) ; }
//	BeautyShopUnit*	GetItemNode(int _gender, int _groupID, int _detail)			{	return m_vpItemGroup[_gender].at(_groupID)->GetItemNode(_detail) ;	}
//	const char*	GetItemNodeName(int _gender, int _groupID, int _detail)			{	return m_vpItemGroup[_gender].at(_groupID)->GetItemName(_detail) ;	}
//	
//	///< Group 관련
//	const char*	GetGroupName(int _gender, int _groupID)			{	return m_vpItemGroup[_gender].at(_groupID)->GetGroupName();	}
//	int			GetGroupCount(int _gender, int _groupID)		{	return m_vpItemGroup[_gender].at(_groupID)->GetGroupCount();	}
//	int			GetGroupCode(int _gender, int _groupID)			{	return m_vpItemGroup[_gender].at(_groupID)->GetGroupCode();	}
//
//	///< GroupList 관련
//	int		GetGroupListCount(int _gender)			{	return (int)m_vpItemGroup[_gender].size();	}
//private:
//	std::vector<BeautyShopItemGroup*>	m_vpItemGroup[2];
//};
//
