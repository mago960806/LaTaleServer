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
// Item ¿¡ ´ëÇÑ ¸ðµç Á¤º¸¸¦ °¡Áö°í ÀÖ´Â Å¬·¡½º 
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
	std::string			m_strName;			//	ÀÌ¸§  [12/5/2005] ÇöÀç ÀÌ¸§¸¸À¸·Î ¿É¼Ç Ç¥Çö.
	std::string			m_strDesc;			//	¼³¸í
	int					m_iEffect;			//	È¿°ú
	int					m_iEffectType;		//	È¿°ú Á¾·ù
	int					m_iOptionMin;		//	¿É¼Ç ÃÖ¼Ò °ª
	int					m_iOptionMax;		//	¿É¼Ç ÃÖ´ë °ª
	int					m_iEnchantMin;		//	ÀÎÃ¦Æ® ÃÖ¼Ò °ª
	int					m_iEnchantMax;		//	ÀÎÃ¦Æ® ÃÖ´ë °ª
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
	bool		LoadLDTPhantomUp();//»ÃÓ°Éý¼¶
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

	ICON_MARK	GetItemIconMark(int iItemID);			//[2006/2/10] ¾ÆÀÌÄÜ ¸¶Å© Á¤º¸¸¦ °¡Á®¿È

	bool		IsItemNameColor(int iItemID);			//[2006/2/13] ¾ÆÀÌÄÜ ÀÌ¸§¿¡ Æ¯º°ÇÑ ÄÃ·¯¸¦ »ç¿ëÇÏ´ÂÁö
	D3DXCOLOR	GetItemNameColor(int iItemID);			//[2006/2/13] ¾ÆÀÌÄÜ ÀÌ¸§¿¡ Æ¯º°ÇÑ ÄÃ·¯¸¦ »ç¿ëÇÏ´Â °æ¿ì ÇØ´ç »öÀ» °¡Á®¿Â´Ù 

	bool		GetItemInfo(int iItemID, SPItemAttr*& pItemAttr);

	SPCashItemAttr* GetCaseListItem(int iIndex);

	SPItemOption*	GetItemOption( UINT32 uiItemOptionID );

	int GetSortCashItemSize(int iCategory);
	SPCashItemAttr* GetSortCashItem(int iCategory, int iIndex);
	SPCashItemAttr* GetCashItem(int iID);
	SPCashItemAttr* GetMinorCashItem(int iID);
	void SortCashItem();
	void SortCashItem(int iGender, int iPrice);
	void RandCashItem();				//[xialin 2008/07/17] add Ëæ»úÉÌÆ·´¦Àí
	int  GetRandStoreCashCategory(int iCategory, int iIndex); //[xialin 2008/07/17] add
	void SetCashVersion(int iMajor, int iMinor);
	void GetCashVersion(int& iMajor, int& iMinor);
	void InitMinorCashItem();
	void UpdateMinorCashItem(CASHSHOP_INFO* pItemInfo);
	void InsertMinorCashItem(SPCashItemAttr& tItemAttr);
	void EraseMinorCashItem(int iID);	
	bool RefreshCaseList(SPCashItemAttr* pkItemAttr);
	/**
	 * ÁÖ¾îÁø CashItemAttr ÀÌ ÁöÁ¤ÇÏ´Â ¾ÆÀÌÅÛÀÇ °¡°Ý Á¤º¸¸¦ °»½ÅÇÑ´Ù <br>
	 * Ä«Å×°í¸®°¡ ÆÐÅ°ÁöÀÎ °æ¿ì¿¡´Â °»½Å ÇÏÁö ¾Ê´Â´Ù <br> 
	 @param SPCashItemAttr* pCashItemAttr
	 @return  
	*/
	//void UpdateItemCashPrize(SPCashItemAttr* pCashItemAttr);

	/**  
	 * ItemClusterÀÇ ¸ðµç ¾ÆÀÌÅÛÁß CashShopID°¡ ÀÖ´Â °æ¿ì <br>
	 * ÇØ´ç CashShop Á¤º¸ÀÇ Cash, mileÀ» ItemAttr¿¡ ¾÷µ¥ÀÌÆ® ÇÑ´Ù <br>
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
	bool			IsItemTimeAdd(int iItemID);			/**< ½Ã°£¿¬ÀåÀÌ °¡´ÉÇÑ ¾ÆÀÌÅÛÀÎÁö È®ÀÎ <br> */
	SPItemTimeAdd*	GetItemTimeAdd(int iItemID);

	bool				InsertPhantomLevelUp( SPPhantomLevelUp& PhantomLevelUp );//²åÈë»ÃÓ°
	bool				IsPhantomLevelUp( int iItemID );//ÅÐ¶ÏÊÇ·ñÊÇ¿ÉÉý¼¶µÄ»ÃÓ°µÀ¾ß
	SPPhantomLevelUp*	GetPhantomLevelUp( int iItemID); //µÃµ½Õâ¸ö»ÃÓ°µÀ¾ßµÄ½á¹¹Ìå
	
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
	std::map<int, SPPhantomLevelUp>			m_mPhantom;//»ÃÓ°
	std::map<int, SPItemSet>				m_mItemSet;
	std::map< int , std::map< int , std::map< int , SPItemMix > > >		m_mmmItemMix;			//	SkillID , Category , ItemMixID

	SPTexture*			m_pItemTexture;	
	SPTexture*			m_pItemNumTexture;

	std::string                      m_strKeyName;           //[xialin 2008/07/17] ËÑË÷¹Ø¼ü×Ö
	std::vector<SPCashItemMiss>      m_vecRandItem;          //[xialin 2008/07/17] Ëæ»úµÀ¾ßÓëÉÌÆ·¶ÔÓ¦±í
	std::vector<SPCashItemSultlist>  m_vecCaseItem;          //[xialin 2008/07/17] Ì××°¶ÔÓ¦±í
	std::vector<SPCashItemAttr*>     m_vecCurCaseList;       //[xialin 2008/07/25] µ±Ç°Ì××°±í
	std::vector<SPCashEffectItem>    m_vecCaseEffect;        //[xialin 2008/07/28] add

};


extern SPItemCluster* g_pItemCluster;


