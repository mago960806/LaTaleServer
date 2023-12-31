#pragma once

#include <vector>
#include <string>

using namespace std;

enum	ITEMSTATUS_ID;
class	SPTexture;

//#define ITEM_SEARCH_HEADER()	std::vector< SPItemStatus* >::iterator iter = m_vpItem.begin(); \
//								iter = std::find_if( m_vpItem.begin(), m_vpItem.end(), std::bind2nd( IsEqualItem, nItemUID ) );\
//								if(iter==m_vpItem.end())\
//								{return 0;}



//////////////////////////////////////////////////////////////////////////
// Item 에 대한 모든 정보를 가지고 있는 클래스 
class SPItemStatus;
enum ICON_MARK;
enum CASHITEM_CATEGORY;
enum ITEM_DURATION;
struct SPItemAttr;
struct SPCashItemAttr;
struct SPItemEnchant;
struct SPItemUpgrade;
struct SPItemTimeAdd;
struct SPItemSet;
struct SPItemMix;
struct SPPhantomLevelUp;
struct SPCashItemMiss;
struct SPCashItemSultlist;
struct SPCashEffectItem;

struct SPItemOption
{
	std::string			m_strName;			//	이름  [12/5/2005] 현재 이름만으로 옵션 표현.
	std::string			m_strDesc;			//	설명
	int					m_iEffect;			//	효과
	int					m_iEffectType;		//	효과 종류
	int					m_iOptionMin;		//	옵션 최소 값
	int					m_iOptionMax;		//	옵션 최대 값
	int					m_iEnchantMin;		//	인챈트 최소 값
	int					m_iEnchantMax;		//	인챈트 최대 값
	int					m_iAppType;
	int					m_iOptionGroup;
	int					m_iOptionLevel;

	SPItemOption			();
	void	SetNull			();
	virtual ~SPItemOption	();
};

enum SPITEM_UPGRADE_STATE
{
	SPIU_STATE_NULL				=	0	,
	SPIU_STATE_ITEM_UPGRADE				,
	SPIU_STATE_PET_LEVELUP				,
	SPIU_STATE_ITEM_MIX					,
};


class SPItemCluster {
public:	
	SPItemCluster();
	~SPItemCluster();

	bool		Init();
	bool		LoadLDTItem(const char* szFileName, bool bLoadPvp = false);	
	bool		LoadLDTOption();
	bool		LoadLDTCashItem();
	bool		LoadLDTEnchant();
	bool		LoadLDTUpgrade();	
	bool		LoadLDTItemSet();
	bool		LoadLDTMix();
	bool		LoadLDTCashMiss();
	bool		LoadLDTSultList();
	bool		LoadLDTCashEffect();
	
	/**
	 * 
	 @param 
	 @return
	*/
	bool		LoadLDTTimeAdd();
	bool		LoadLDTPhantomUp();//뼙緞�薨�
	//void		InsertItem(int nItemUID, ITEMSTATUS_ID eClass, int nXSrcPos, int nYSrcPos, string itemName, string itemDesc, int nStackCnt=1);
	//void		InsertItem( int , int, int, int, int, string , int nType, int nSubType, int nPosID1, int nSubPosID11=0, int nPosID2=0, int nSubPosID21=0, int nStackCnt=0);	


	int			GetItemInfo(int nItemUID, string& itemName, string& itemDesc);

	//int		GetEquipID(int nItemUID);			// equip
	//int		GetStackCount(int nItemUID);		// comsume
	//int		GetPosID(int nItemUID, int &nPosID1, int &nSubPosID1, int &nPosID2, int &nSubPosID2);
	//int 		GetPosID(int nItemUID, int &nPosID1 );

	//int			GetType( int nItemUID, int &nType);
	//int			GetSubType( int nItemUID, int &nSubType);

	SPTexture*	GetItemTexture(int iItemID, POINT& ptSrcPos);
	SPTexture*	GetItemTextureFromTextureInx(int iIconImage, int iIconIndex, POINT& ptSrcPos);
	const char*	GetItemTextureStr(int iItemID, POINT& ptSrcPos);
	const char* GetItemTextureStrFromTextureInx(int iIconImage, int iIconIndex,POINT& ptSrcPos);

	SPTexture*	GetNumTexture();

	ICON_MARK	GetItemIconMark(int iItemID);			//[2006/2/10] 아이콘 마크 정보를 가져옴

	bool		IsItemNameColor(int iItemID);			//[2006/2/13] 아이콘 이름에 특별한 컬러를 사용하는지
	D3DXCOLOR	GetItemNameColor(int iItemID);			//[2006/2/13] 아이콘 이름에 특별한 컬러를 사용하는 경우 해당 색을 가져온다 

	bool		GetItemInfo(int iItemID, SPItemAttr*& pItemAttr);

	SPCashItemAttr* GetCaseListItem(int iIndex);

	SPItemOption*	GetItemOption( UINT32 uiItemOptionID );

	int GetSortCashItemSize(int iCategory);
	SPCashItemAttr* GetSortCashItem(int iCategory, int iIndex);
	SPCashItemAttr* GetCashItem(int iID);
	SPCashItemAttr* GetMinorCashItem(int iID);
	void SortCashItem();
	void SortCashItem(int iGender, int iPrice);
	void RandCashItem();				//[xialin 2008/07/17] add 踞샙�鉗럽╂�
	int  GetRandStoreCashCategory(int iCategory, int iIndex); //[xialin 2008/07/17] add
	void SetCashVersion(int iMajor, int iMinor);
	void GetCashVersion(int& iMajor, int& iMinor);
	void InitMinorCashItem();
	void UpdateMinorCashItem(CASHSHOP_INFO* pItemInfo);
	void InsertMinorCashItem(SPCashItemAttr& tItemAttr);
	void EraseMinorCashItem(int iID);	
	bool RefreshCaseList(SPCashItemAttr* pkItemAttr);
	/**
	 * 주어진 CashItemAttr 이 지정하는 아이템의 가격 정보를 갱신한다 <br>
	 * 카테고리가 패키지인 경우에는 갱신 하지 않는다 <br> 
	 @param SPCashItemAttr* pCashItemAttr
	 @return  
	*/
	//void UpdateItemCashPrize(SPCashItemAttr* pCashItemAttr);

	/**  
	 * ItemCluster의 모든 아이템중 CashShopID가 있는 경우 <br>
	 * 해당 CashShop 정보의 Cash, mile을 ItemAttr에 업데이트 한다 <br>
	 @param  
	 @return 
	*/  
	void UpdateItemCashPrize();

	bool InsertItemEnchant(int iKey, SPItemEnchant& ItemEnchant);
	SPItemEnchant* GetExactItemEnchant(int iOptionID, int iMaterialID, int iMaterialNum);
	SPItemEnchant* GetItemEnchant(int iOptionID, int iMaterialID, int iMaterialNum);
	bool IsItemEnchant(int iOptionID);
	bool IsItemEnchant(int iOptionID, int iMaterialID);

	bool					InsertItemUpgrade( SPItemUpgrade& ItemUpgrade );//need
	bool					IsItemUpgrade( int iItemID , int iRare );
	SPItemUpgrade*			GetItemUpgrade( int iItemID , int iRare );
	__int64					GetItemUpgradeKey( int iItemID , int iRare );
	SPITEM_UPGRADE_STATE	GetItemUpgradeState();
	void					SetItemUpgradeState( SPITEM_UPGRADE_STATE eState );

	bool			InsertItemTimeAdd(SPItemTimeAdd& ItemTimeAdd);
	bool			IsItemTimeAdd(int iItemID);			/**< 시간연장이 가능한 아이템인지 확인 <br> */
	SPItemTimeAdd*	GetItemTimeAdd(int iItemID);

	bool				InsertPhantomLevelUp( SPPhantomLevelUp& PhantomLevelUp );//꿨흙뼙緞
	bool				IsPhantomLevelUp( int iItemID );//털뙤角뤠角옵�薨떫캥촨갠읍�
	SPPhantomLevelUp*	GetPhantomLevelUp( int iItemID); //돤돕侶몸뼙緞돛야돨써뭐竟
	
	bool			InsertItemSet(SPItemSet& ItemSetB);
	bool			IsItemSet(int iSetID);
	SPItemSet*		GetItemSet(int iSetID);

	SPCashEffectItem*   GetEffectByID(int iID);
	SPCashEffectItem*   GetEffectByName(const char * pszName);


	ITEM_DURATION GetItemDuration(int iItemID);

	bool												InsertItemMix			( SPItemMix& stItemMixAdd );
	bool												IsItemMix				( int iSkillID , int iItemMixID );
	std::map< int , std::map< int , SPItemMix > >*		GetItemMixSkillID		( int iSkillID );
	std::map< int , SPItemMix >*						GetItemMixCategory		( int iSkillID , int iCategory );
	SPItemMix*											GetItemMixItemMixID		( int iSkillID , int iItemMixID );

	void            SetSearchKey(string  strKey) { m_strKeyName = strKey; }

protected:

	std::vector< SPItemStatus* >	m_vpItem;

	stdext::hash_map<UINT32, SPItemAttr*>	m_hmItem;
	stdext::hash_map<UINT32, SPTexture*>	m_hmIconTexture;

	std::map< UINT32 , SPItemOption >		m_mItemOption;

	int m_iCashMajorVersion;
	int m_iCashMinorVersion;
	std::vector< SPCashItemAttr > m_vCashItem;
	std::vector< SPCashItemAttr > m_vMinorCashItem;
	std::vector< SPCashItemAttr* > m_vSortCashItem;
	std::map< int, std::vector< SPItemEnchant > > m_mItemEnchant;

	std::map< __int64 , SPItemUpgrade >		m_mItemUpgrade;
	SPITEM_UPGRADE_STATE					m_eItemUpgradeState;

	std::map<int, SPItemTimeAdd>			m_mItemTime;
	std::map<int, SPPhantomLevelUp>			m_mPhantom;//뼙緞
	std::map<int, SPItemSet>				m_mItemSet;
	std::map< int , std::map< int , std::map< int , SPItemMix > > >		m_mmmItemMix;			//	SkillID , Category , ItemMixID

	SPTexture*			m_pItemTexture;	
	SPTexture*			m_pItemNumTexture;

	std::string                      m_strKeyName;           //[xialin 2008/07/17] 鎧乞밑숩俚
	std::vector<SPCashItemMiss>      m_vecRandItem;          //[xialin 2008/07/17] 踞샙돛야宅�鉗렀憧┗�
	std::vector<SPCashItemSultlist>  m_vecCaseItem;          //[xialin 2008/07/17] 敬陋뚤壇깊
	std::vector<SPCashItemAttr*>     m_vecCurCaseList;       //[xialin 2008/07/25] 뎠품敬陋깊
	std::vector<SPCashEffectItem>    m_vecCaseEffect;        //[xialin 2008/07/28] add

};


extern SPItemCluster* g_pItemCluster;


