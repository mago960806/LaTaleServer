
#pragma once 

#include <string>
#include <vector>

/**
	BeautyShopData Manager Class
	///< HairStyle, HairColor, 
	///< 실제 사용자 Interface를 제공한다.
*/

enum BS_ITEM_STYLE
{
	BS_HAIR = 0,
	BS_HAIR_C,
	BS_EYE,
	BS_EYE_C,
	BS_SKIN,
	BS_ITEM_MAX,
};

enum{
	TYPE_BASIC = 0,
	TYPE_SPECIAL,
	TYPE_EXPERT,
	COLOR_SPECIAL = 50,
};

enum BS_VALID_CHECK {
	CHECK_OK = 0,
	ERROR_CANT_ITEMINFO = -1 ,
	ERROR_HASNT_COUPON	= -2 ,
	ERROR_SAME_CHANGE	= -3 ,
	WARNING_EXPERT_CHANGE  = -4,
	ERROR_COLOR_CHANGE	= -5 ,
	ERROR_ITEM_LIMIT	= -6 ,
} ;

struct BS_CODE_PARSE
{
	int iCode ;			// 아이템 ID
	int iLayerID ;			// 레이어 구분
	int iGender;			// 성별
	int iSpecialID ;		// 스페셜 코드
	int iSerialID ;			// 일렬번호
	int iColor;				// 컬러값
	int iCodeNC;			// 컬러를 제외한 코드값

	BS_CODE_PARSE(){
		Clear();
	}
	void Clear(){
		iCode = 0 ;
		iLayerID = 0 ;
		iGender = 0 ;
		iSpecialID = 0 ;
		iSerialID = 0 ;
		iColor = 0 ;
		iCodeNC = 0 ;
	}
};

struct BS_IN_ITEM {
	int itemLayer;
	int itemID ;
	int itemCode ;
	int needItemID1 ;
	int needItemID2 ;
	int positionID ;		///< Sequence No
	std::string strCatego ;
	std::string strDetail ;

	BS_IN_ITEM() {
		Clear() ;
	}
	void insert(int nlay, int iid, int icode, int nid01, int nid02, int pos, char* strC, char* strD) {
		itemLayer = nlay ;
		itemID = iid ;
		itemCode = icode ;
		needItemID1 = nid01 ;
		needItemID2 = nid02 ;
		positionID = pos ;
		strCatego = strC ;
		strDetail = strD ;
	}
	void Clear()	{
		itemLayer = 0 ;
		itemID = 0;
		itemCode = 0;
		needItemID1 = 0;
		needItemID2 = 0;
		positionID = 0;
		strCatego.clear() ;
		strDetail.clear() ;
	}
};


struct BS_INDEX_MNG
{
	int		m_iCurCategoIndex ;
	int		m_iCurDetailIndex ;
	int		m_iPreCategoIndex ;
	int		m_iPreDetailIndex ;
	BS_INDEX_MNG()
	{
		Clear() ;
	}
	void Clear()
	{
		m_iCurCategoIndex = 0;
		m_iCurDetailIndex = 0;
		m_iPreCategoIndex = 0;
		m_iPreDetailIndex = 0;
	}
	void InitCategory(int _catIndex)
	{
		m_iPreCategoIndex = _catIndex;
		m_iCurCategoIndex = _catIndex ;
	}
	void InitDetail(int _detailIndex)
	{
		m_iPreDetailIndex = _detailIndex;
		m_iCurDetailIndex = _detailIndex ;
		if(m_iPreDetailIndex < 0)	 m_iPreDetailIndex = 0 ;
	}
	void PreChange(int _catIndex, int _detailIndex)
	{
		m_iPreCategoIndex = _catIndex ;
		m_iPreDetailIndex = _detailIndex ;
	}
	void SetCategory(int _catIndex)
	{
		m_iPreCategoIndex = m_iCurCategoIndex;
		m_iCurCategoIndex = _catIndex ;
	}
	void SetDetail(int _detailIndex)
	{
		m_iPreDetailIndex = m_iCurDetailIndex;
		m_iCurDetailIndex = _detailIndex ;
	}
};



class BeautyShopUnit;
class BeautyShopItemGroup ;
class BeautyShopItemGroupList;

class BeautyShopManager
{
public:
	BeautyShopManager() ;
	~BeautyShopManager() ;

	void Create() ;		///< 생성
	void Init() ;		///< 초기화
	void Clean() ;		//< 메모리 해제

	void LoadBeautyShopLDT() ;									///< BeautyShop.LDT파일 읽기
	void AddBeautyItem(BS_IN_ITEM in_item) ;					///< LDT Data -> List에 추가
	int Parsing_CharCode(int iCharCode, BS_CODE_PARSE* _p) ;	///< 아이템 정보 분리

	void SetTestCharSet(int _gender) ;						///< 사용자의 기본 캐릭터 설정에 맞춰 초기화
	int SetDefaultCharSet(int _ItemCode, int _gender, bool bChanged = true) ;	///< 캐릭터의 셋팅대로 Data Setting
	void SetDefaultIndexSet(int _itemLayer);									///< 없는 아이템 Default Index Setting

	bool OnChangeCategoryLeft(unsigned int);	///< 카테고리 Left
	bool OnChangeCategoryRight(unsigned int);	///< 카테고리 Right
	bool OnChangeDetailLeft(unsigned int);		///< 세부내용 Left
	bool OnChangeDetailRight(unsigned int);		///< 세부내용 Right

	const char* GetCategoryString(bool isCurrend, int _layer, int _gender = 0, int _catego = 0) ;
	const char* GetDetailString(bool isCurrend, int _layer, int _gender = 0, int _catego = 0, int _detail = 0) ;

	bool GetCategoryEnable(int _layer);
	int GetCategoryValue(int _layer)		{ return m_iLayerIndex[_layer].m_iCurCategoIndex ; }
	int GetDetailValue(int _layer)			{ return m_iLayerIndex[_layer].m_iCurDetailIndex ; }
	BeautyShopUnit* GetSelectItem(int _layer);

	// Group count & Item Count
	int GetLayerCount(int _gender, int _layer, int _categ = -1) ;
	int GetHairStyleCount(int _gender, int _categ = -1);
	int GetHairColorCount(int _gender, int _categ = -1);
	int GetEyeStyleCount(int _gender, int _categ = -1);
	int GetEyecolorCount(int _gender, int _categ = -1);
	int GetSkinCount(int _gender, int _categ = -1);

	// Item Valid Check
	int CommitItemValidCheck(int _layer, bool bChecked, CONTAINER_SLOT& _sendSlot1, CONTAINER_SLOT& _sendSlot2);
	int GetExpectItemCode(int _layer);
	bool VerifyBeautyShopLDT() ;

private:
	int		m_iCurGender ;								///< 현재 성별
	bool	m_bEnabled[BS_ITEM_MAX];					///< 활성화 여부
	bool	m_bLoadFail[BS_ITEM_MAX] ;					///< 기본 로딩 불가 여부
	BS_INDEX_MNG m_iLayerIndex[BS_ITEM_MAX] ;			///< 이전선택, 현재선택 Index
	BeautyShopItemGroupList* m_pBSGroup[BS_ITEM_MAX] ;	///< 아이템 그룹 리스트
};