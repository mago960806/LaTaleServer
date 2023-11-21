
#include <algorithm>
#include "SPCommon.h"
#include "SPUtil.h"

#include "SPResourceDef.h"
#include "SPTexture.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"

#include "SPGOBManager.h"
#include "SPGameObject.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPPlayer.h"
#include "SPPlayerInvenArchive.h"

#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPItemStatusEquip.h"
//#include "SPItem.h"
#include "SPItemCluster.h"
#include "SPDebug.h"

#include "atlconv.h"


SPItemCluster* g_pItemCluster = NULL;

struct _IsEqualItem : binary_function<SPItemStatus*, int, bool> {
	bool operator() (SPItemStatus* item, int nItemUID) const
	{		
		return (item->SPSendMessage(MV_ITEM_GETUID) ==nItemUID);
	}
} IsEqualItem;


//////////////////////////////////////////////////////////////////////////
//
//	SPItemOption
//

SPItemOption::SPItemOption()
{
	SetNull();
}

SPItemOption::~SPItemOption()
{
}

void SPItemOption::SetNull()
{
	m_strName.clear();
	m_strDesc.clear();
	m_iEffect			=	0;
	m_iEffectType		=	0;
	m_iOptionMin		=	0;
	m_iOptionMax		=	0;
	m_iEnchantMin		=	0;
	m_iEnchantMax		=	0;
}


//////////////////////////////////////////////////////////////////////////
//
//	SPItemCluster
//

SPItemCluster::SPItemCluster()
{	
	m_pItemNumTexture = NULL;
	m_pItemTexture = NULL;

#if defined(_DEBUG)
	DXUTOutputDebugString("ItemCluster::Init()\n");
#endif

	//Init();

	g_pItemCluster = this;
}

SPItemCluster::~SPItemCluster()
{
	SAFE_RELEASE( m_pItemNumTexture );
	SAFE_RELEASE( m_pItemTexture );

	std::vector<SPItemStatus*>::iterator Iter = m_vpItem.begin();
	for(;Iter != m_vpItem.end(); ++Iter) {
		SAFE_DELETE(*Iter);
	}
	m_vpItem.clear();

	hash_map<UINT32, SPItemAttr*>::iterator hmIter = m_hmItem.begin();
	for(;hmIter != m_hmItem.end(); ++hmIter) {
		SAFE_DELETE((*hmIter).second);
	}
	m_hmItem.clear();

	hash_map<UINT32, SPTexture*>::iterator hmIter2 = m_hmIconTexture.begin();
	for(;hmIter2 != m_hmIconTexture.end(); ++hmIter2) {
		SAFE_RELEASE((*hmIter2).second);
	}
	m_hmIconTexture.clear();

	map< int, vector< SPItemEnchant > >::iterator iterEnchant = m_mItemEnchant.begin();
	for(;iterEnchant != m_mItemEnchant.end(); ++iterEnchant) {
		(*iterEnchant).second.clear();
	}
	m_mItemEnchant.clear();

	m_mItemOption.clear();
	m_vCashItem.clear();
	m_vMinorCashItem.clear();
	m_vSortCashItem.clear();
	m_mItemUpgrade.clear();
	m_mItemTime.clear();
	m_mPhantom.clear();//»ÃÓ°Éý¼¶
}


bool SPItemCluster::Init()
{
	m_strKeyName = "";
	m_vecRandItem.clear();
	m_vecCaseItem.clear();
	m_vecCurCaseList.clear();
	m_vecCaseEffect.clear();

	//g_pVideo->GetTextureMgr()->LoadTexture( 
	//	GetAddResPath( "DATA/INTERFACE/CONCEPT/UI_NUM.PNG", RES_WORK_RESOURCE_PATH) , &m_pItemNumTexture);

	//g_pVideo->GetTextureMgr()->LoadTexture( 
	//	GetAddResPath( "Data/DataFile/ItemIcon001.png", RES_WORK_RESOURCE_PATH) , &m_pItemTexture);

	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI_NUM.PNG", &m_pItemNumTexture);

	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/DATAFILE/ITEMICON001.PNG", &m_pItemTexture);

	bool bRet = true;
	bRet = LoadLDTItem(RES_FNAME_LDT_ITEM_CLUSTER1, true);
	bRet = LoadLDTItem(RES_FNAME_LDT_ITEM_CLUSTER2);
	bRet = LoadLDTItem(RES_FNAME_LDT_ITEM_CLUSTER3);

	if(bRet == false) {
		LoadLDTItem(RES_FNAME_LDT_ITEM_CLUSTER);
	}

	////
	//SPLDTFile* pLDTItem;
	//g_pResourceManager->GetLDTFromFileName(RES_FNAME_LDT_ITEM_CLUSTER, &pLDTItem);
	//if(pLDTItem == NULL) return false;

	//SPItemAttr* pItemAttr;	
	//int iItemID;	
	//LDT_Field LDTField;
	//int iNORec = pLDTItem->GetItemCount();

	//for(int iRecord = 0; iRecord < iNORec; ++iRecord ) {

	//	iItemID = pLDTItem->GetPrimaryKey( iRecord );

	//	pItemAttr = new SPItemAttr;		

	//	pLDTItem->GetField( iItemID, 0,			LDTField);			//"_Category"
	//	pItemAttr->m_iCategory			= LDTField.uData.lValue;

	//	pLDTItem->GetField( iItemID, 1,			LDTField);			//"_Num"
	//	pItemAttr->m_iNum				= LDTField.uData.lValue;

	//	pLDTItem->GetField( iItemID, 2,			LDTField);			//"_ColorID"
	//	pItemAttr->m_iColorID			= LDTField.uData.lValue;

	//	pLDTItem->GetField( iItemID, 3,			LDTField);			//"_Name"
	//	pItemAttr->m_strName			= LDTField.uData.pValue;

	//	pLDTItem->GetField( iItemID, 4,			LDTField);			//"_Description"
	//	pItemAttr->m_strDescription		= LDTField.uData.pValue;

	//	pLDTItem->GetField( iItemID, 5,			LDTField);			//"_Icon"
	//	pItemAttr->m_iIcon			= LDTField.uData.lValue;

	//	pLDTItem->GetField( iItemID, 6,			LDTField);			//"_IconIndex"
	//	pItemAttr->m_iIconIndex			= LDTField.uData.lValue;

	//	pLDTItem->GetField( iItemID, 7,			LDTField);			//"_GamePrice"
	//	pItemAttr->m_iGamePrice			= LDTField.uData.lValue;

	//	pLDTItem->GetField( iItemID, 8,			LDTField);			//"_CashPrice"
	//	pItemAttr->m_iCashPrice			= LDTField.uData.lValue;

	//	pLDTItem->GetField( iItemID, 9,			LDTField);			//"_CashCheck"
	//	pItemAttr->m_bCashCheck			= (LDTField.uData.lValue) ? true : false;
	//	
	//	//_Update_Date(10) °ÔÀÓ»ó¿¡¼­´Â »ç¿ëÇÏÁö ¾ÊÀ½

	//	pLDTItem->GetField( iItemID, 11,			LDTField);		//"_RareLimit"
	//	pItemAttr->m_eRareLimit			= (ITEM_RARE_LIMIT)LDTField.uData.lValue;

	//	pLDTItem->GetField( iItemID, 12,			LDTField);		//"_Type"
	//	pItemAttr->m_eType				= (ITEM_TYPE)LDTField.uData.lValue;

	//	pLDTItem->GetField( iItemID, 13,			LDTField);		//"_SubType"
	//	pItemAttr->m_eSubType			= (ITEM_SUBTYPE)LDTField.uData.lValue;

	//	pLDTItem->GetField( iItemID, 14,			LDTField);		//"_PosID1"
	//	pItemAttr->m_ePosID1			= (ITEM_EQUIPPOS)LDTField.uData.lValue;

	//	pLDTItem->GetField( iItemID, 15,			LDTField);		//"_SubPosID11"
	//	pItemAttr->m_eSubPosID11		= (ITEM_EQUIPPOS)LDTField.uData.lValue;

	//	pLDTItem->GetField( iItemID, 16,			LDTField);		//"_SubPosID12"
	//	pItemAttr->m_eSubPosID12		= (ITEM_EQUIPPOS)LDTField.uData.lValue;

	//	pLDTItem->GetField( iItemID, 17,			LDTField);		//"_PosID2"
	//	pItemAttr->m_ePosID2			= (ITEM_EQUIPPOS)LDTField.uData.lValue;

	//	pLDTItem->GetField( iItemID, 18,			LDTField);		//"_SubPosID21"
	//	pItemAttr->m_eSubPosID21		= (ITEM_EQUIPPOS)LDTField.uData.lValue;

	//	pLDTItem->GetField( iItemID, 19,			LDTField);		//"_SubPosID22"
	//	pItemAttr->m_eSubPosID22		= (ITEM_EQUIPPOS)LDTField.uData.lValue;

	//	pLDTItem->GetField( iItemID, 20,			LDTField);		//"_Kind"
	//	pItemAttr->m_eKind				= (ITEM_EQUIP_KIND)LDTField.uData.lValue;

	//	pLDTItem->GetField( iItemID, 21,			LDTField);		//"_StatusType1"
	//	pItemAttr->m_StatusType[0].Effect	= (ITEM_EQUIP_EFFECT)LDTField.uData.lValue;

	//	pLDTItem->GetField( iItemID, 22,			LDTField);		//"_StatusValue1"
	//	pItemAttr->m_StatusType[0].iValue	= LDTField.uData.lValue;

	//	pLDTItem->GetField( iItemID, 23,			LDTField);		//"_StatusType2"
	//	pItemAttr->m_StatusType[1].Effect		= (ITEM_EQUIP_EFFECT)LDTField.uData.lValue;

	//	pLDTItem->GetField( iItemID, 24,			LDTField);		//"_StatusValue2"
	//	pItemAttr->m_StatusType[1].iValue		= LDTField.uData.lValue;

	//	pLDTItem->GetField( iItemID, 25,			LDTField);		//"_StatusType3"
	//	pItemAttr->m_StatusType[2].Effect		= (ITEM_EQUIP_EFFECT)LDTField.uData.lValue;

	//	pLDTItem->GetField( iItemID, 26,			LDTField);		//"_StatusValue3"
	//	pItemAttr->m_StatusType[2].iValue		= LDTField.uData.lValue;

	//	pLDTItem->GetField( iItemID, 27,			LDTField);		//"_StatusType4"
	//	pItemAttr->m_StatusType[3].Effect		= (ITEM_EQUIP_EFFECT)LDTField.uData.lValue;

	//	pLDTItem->GetField( iItemID, 28,			LDTField);		//"_StatusValue4"
	//	pItemAttr->m_StatusType[3].iValue		= LDTField.uData.lValue;

	//	//	AJJIYA [11/18/2005]
	//	//	ÇÊ¿ä Á¶°Ç º¯°æ
	//	//char	szBuf[ MAX_PATH ];

	//	for ( int i = 0 ; i < MAX_ITEM_OPTION ; ++i )
	//	{
	//		//sprintf( szBuf , "_Require%d_Type"		, i + 1 );
	//		pLDTItem->GetField( iItemID , 29 + (i * 4) , LDTField );
	//		pItemAttr->m_RequireType[i].m_iType	=	LDTField.uData.lValue;

	//		//sprintf( szBuf , "_Require%d_ID"		, i + 1 );
	//		pLDTItem->GetField( iItemID , 30 + (i * 4) , LDTField );
	//		pItemAttr->m_RequireType[i].m_iID	=	LDTField.uData.lValue;

	//		//sprintf( szBuf , "_Require%d_Value1"	, i + 1 );
	//		pLDTItem->GetField( iItemID , 31 + (i * 4), LDTField );
	//		pItemAttr->m_RequireType[i].m_iValue1	=	LDTField.uData.lValue;

	//		//sprintf( szBuf , "_Require%d_Value2"	, i + 1 );
	//		pLDTItem->GetField( iItemID , 32 + (i * 4), LDTField );
	//		pItemAttr->m_RequireType[i].m_iValue2	=	LDTField.uData.lValue;
	//	}

	//	pLDTItem->GetField( iItemID, 45,			LDTField);		//"_CoolTime1"
	//	pItemAttr->m_iCoolTime1			= LDTField.uData.lValue;

	//	pLDTItem->GetField( iItemID, 46,			LDTField);		//"_CoolTime2"
	//	pItemAttr->m_iCoolTime2			= LDTField.uData.lValue;

	//	pLDTItem->GetField( iItemID, 47,			LDTField);		//"_CoolTimeApplyID1"
	//	pItemAttr->m_iCoolTimeApplyID1	= LDTField.uData.lValue;

	//	pLDTItem->GetField( iItemID, 48,			LDTField);		//"_CoolTimeApplyID2"
	//	pItemAttr->m_iCoolTimeApplyID2	= LDTField.uData.lValue;

	//	pLDTItem->GetField( iItemID, 49,			LDTField);		//"_ItemCoolTimeID"
	//	pItemAttr->m_iItemCoolTimeID	= LDTField.uData.lValue;

	//	

	//	pLDTItem->GetField( iItemID, 50,			LDTField);		//"_DurationTime"
	//	pItemAttr->m_iDurationTime		= LDTField.uData.lValue;

	//	pLDTItem->GetField( iItemID, 51,			LDTField);		//"_StackLimit"
	//	pItemAttr->m_iStackLimit		= LDTField.uData.lValue;

	//	pLDTItem->GetField( iItemID, 52,			LDTField);		//"_ItemLv"
	//	pItemAttr->m_iItemLv			= LDTField.uData.lValue;

	//	pLDTItem->GetField( iItemID, 53,			LDTField);		//"_Logging"
	//	pItemAttr->m_bLogging			= (LDTField.uData.lValue) ? true : false;

	//	pLDTItem->GetField( iItemID, 54,			LDTField);		//"_Serialized"
	//	pItemAttr->m_bSerialized		= (LDTField.uData.lValue) ? true : false;

	//	pLDTItem->GetField( iItemID, 55,			LDTField);		//"_Trade"
	//	pItemAttr->m_bTrade				= (LDTField.uData.lValue) ? true : false;

	//	pLDTItem->GetField( iItemID, 56,			LDTField);		//"_Destroy"
	//	pItemAttr->m_bDestroy			= (LDTField.uData.lValue) ? true : false;

	//	pLDTItem->GetField( iItemID, 57,			LDTField);		//"_Storage"
	//	pItemAttr->m_bStorage			= (LDTField.uData.lValue) ? true : false;

	//	pLDTItem->GetField( iItemID, 58,			LDTField);		//"_Sell"
	//	pItemAttr->m_bSell				= (LDTField.uData.lValue) ? true : false;

	//	pLDTItem->GetField( iItemID, 59,			LDTField);		//"_ItemPointMin"
	//	pItemAttr->m_iItemPointMin		= LDTField.uData.lValue;

	//	pLDTItem->GetField( iItemID, 60,			LDTField);		//"_ItemPointMax"
	//	pItemAttr->m_iItemPointMax		= LDTField.uData.lValue;

	//	pLDTItem->GetField( iItemID, 61,			LDTField);		//"_OptionID1"
	//	pItemAttr->m_iOptionID1			= LDTField.uData.lValue;

	//	pLDTItem->GetField( iItemID, 62,			LDTField);		//"_OptionID2"
	//	pItemAttr->m_iOptionID2			= LDTField.uData.lValue;

	//	pLDTItem->GetField( iItemID, 63,			LDTField);		//"_OptionID3"
	//	pItemAttr->m_iOptionID3			= LDTField.uData.lValue;

	//	pLDTItem->GetField( iItemID, 64,			LDTField);		//"_OptionID4"
	//	pItemAttr->m_iOptionID4			= LDTField.uData.lValue;

	//	pLDTItem->GetField( iItemID, 65,			LDTField);		//"_MotionTypeID1"
	//	pItemAttr->m_iMotionTypeID1		= LDTField.uData.lValue;

	//	pLDTItem->GetField( iItemID, 66,			LDTField);		//"_MotionTypeID2"
	//	pItemAttr->m_iMotionTypeID2		= LDTField.uData.lValue;

	//	//	AJJIYA [7/21/2006]
	//	//	È¿°ú ID Ãß°¡
	//	for ( i = 0 ; i < MAX_ITEM_EFFECT ; ++i )
	//	{
	//		//sprintf( szBuf , "_EffectID%d"		, i + 1 );
	//		pLDTItem->GetField( iItemID , 67 + i , LDTField );
	//		pItemAttr->m_iEffectID[ i ]	=	LDTField.uData.lValue;
	//	}

	//	pLDTItem->GetField( iItemID, 71,			LDTField);		//"_OptionType"
	//	pItemAttr->m_iOptionType			= LDTField.uData.lValue;

	//	pLDTItem->GetField( iItemID, 72,			LDTField);		//"_OptionClass"
	//	pItemAttr->m_iOptionClass			= LDTField.uData.lValue;

	//	pLDTItem->GetField( iItemID, 73,			LDTField);		//"_IconMark"
	//	pItemAttr->m_eIconMark			= (ICON_MARK)LDTField.uData.lValue;

	//	pLDTItem->GetField( iItemID, 74,			LDTField);		//"_NameColor"
	//	pItemAttr->m_bNameColor			= (LDTField.uData.lValue) ? true : false;

	//	if(pItemAttr->m_bNameColor) {
	//		pLDTItem->GetField( iItemID, 75,		LDTField);		//"_Red"
	//		pItemAttr->m_cRed			= LDTField.uData.lValue;

	//		pLDTItem->GetField( iItemID, 76,		LDTField);		//"_Green"
	//		pItemAttr->m_cGreen			= LDTField.uData.lValue;
	//		
	//		pLDTItem->GetField( iItemID, 77,		LDTField);		//"_Blue"
	//		pItemAttr->m_cBlue			= LDTField.uData.lValue;
	//	}

	//	pLDTItem->GetField( iItemID, 78,			LDTField);		//"_Time_Type"
	//	pItemAttr->m_eDuration			= (ITEM_DURATION)LDTField.uData.lValue;
	//	
	//	pLDTItem->GetField( iItemID, 79,			LDTField);		//"_Lifetime"
	//	pItemAttr->m_iLifeTime			= LDTField.uData.lValue;

	//	//_Default_Type (80)

	//	pLDTItem->GetField( iItemID, 81,			LDTField);		//"_Max_EXP"
	//	if(LDTField.uData.pValue) {
	//		pItemAttr->m_iPetMaxExp			= _atoi64(LDTField.uData.pValue);
	//	}

	//	//_Limit_EXP(82)

	//	pLDTItem->GetField( iItemID, 83,			LDTField);		//"_Pet_ClassID"
	//	pItemAttr->m_iPetClassID		= LDTField.uData.lValue;

	//	for( i = 0; i < MAX_ITEM_SKILL; i++ )
	//	{
	//		//sprintf(szBuf , "_Skill%d", i + 1 );
	//		pLDTItem->GetField(iItemID, 84 + (i * 3), LDTField);
	//		pItemAttr->m_iSkillID[i] = LDTField.uData.lValue;

	//		//sprintf(szBuf , "_SLv%d", i + 1 );
	//		pLDTItem->GetField(iItemID, 85 + (i * 3), LDTField);
	//		pItemAttr->m_iSkillLevel[i] = LDTField.uData.lValue;

	//		//sprintf(szBuf , "_Consume_SP%d", i + 1 );
	//		pLDTItem->GetField(iItemID, 86 + (i * 3), LDTField);
	//		pItemAttr->m_iConsumeSP[i] = LDTField.uData.lValue;
	//	}

	//	pLDTItem->GetField( iItemID, 96,			LDTField);		//"_Macro_Type"
	//	pItemAttr->m_iMacroType = LDTField.uData.lValue;

	//	hash_map<UINT32, SPItemAttr*>::iterator hmIterItem = m_hmItem.find(iItemID);
	//	if(hmIterItem != m_hmItem.end() && (iItemID == (*hmIterItem).first) ) {
	//		assert(0 && "Item ID Áßº¹");
	//		continue;
	//	} else {
	//		m_hmItem.insert ( hash_map<UINT32, SPItemAttr*>::value_type( iItemID, pItemAttr) );			
	//	}

	//	hash_map<UINT32, SPTexture*>::iterator hmIterTexture = m_hmIconTexture.find(pItemAttr->m_iIcon);
	//	if(hmIterTexture != m_hmIconTexture.end() && (iItemID == (*hmIterTexture).first) ) {	
	//		continue;
	//	} else {
	//		SPTexture* pTexture;
	//		string strResource; // = RES_WORK_RESOURCE_PATH;
	//		const char* pszFileName = g_pResourceManager->GetGlobalFileName( pItemAttr->m_iIcon );

	//		if( pszFileName )
	//			strResource += g_pResourceManager->GetGlobalFileName( pItemAttr->m_iIcon );

	//		g_pVideo->GetTextureMgr()->LoadTexture(strResource.c_str(), &pTexture);			
	//		m_hmIconTexture.insert( hash_map<UINT32, SPTexture*>::value_type( pItemAttr->m_iIcon, pTexture) );
	//	}
	//}
	//
	//SAFE_RELEASE(pLDTItem);	

	if( LoadLDTItemSet() == false)
		return false;
	
	if( LoadLDTOption() == false )
		return false;

	if( LoadLDTCashItem() == false )
		return false;

	UpdateItemCashPrize();	

	if( LoadLDTEnchant() == false )
		return false;

	if( LoadLDTUpgrade() == false )
		return false;

	if( LoadLDTTimeAdd() == false )
		return false;

	if( LoadLDTMix() == false )
		return false;

	if( LoadLDTCashMiss() == false )
		return false;

	if ( LoadLDTSultList() == false )
		return false;

	if ( LoadLDTCashEffect() == false )
		return false;

	return true;
}


bool SPItemCluster::LoadLDTItem(const char* szFileName, bool bLoadPvp /*= false*/)
{
	SPLDTFile* pLDTItem;
	g_pResourceManager->GetLDTFromFileName(szFileName, &pLDTItem);
	if(pLDTItem == NULL) return false;

	SPLDTFile* pLDTPvpItem;
	if(bLoadPvp) {
		g_pResourceManager->GetLDTFromFileName(RES_FNAME_LDT_PVP_ITEM1, &pLDTPvpItem);
		if(pLDTPvpItem == NULL){
#ifdef _DEBUG
			DXUTOutputDebugString( "----------------------------------->PVP ITEM1 LDT FILE LOAD FAIL [%s] \n" , RES_FNAME_LDT_PVP_ITEM1);
#endif		
		}
	}
	
	SPItemAttr* pItemAttr;	
	int iItemID;	
	LDT_Field LDTField;
	int iNORec = pLDTItem->GetItemCount();

	for(int iRecord = 0; iRecord < iNORec; ++iRecord ) {

		iItemID = pLDTItem->GetPrimaryKey( iRecord );

		pItemAttr = new SPItemAttr;		

		pLDTItem->GetField( iItemID, 0,			LDTField);			//"_Category"
		pItemAttr->m_iCategory			= LDTField.uData.lValue;

		pLDTItem->GetField( iItemID, 1,			LDTField);			//"_Num"
		pItemAttr->m_iNum				= LDTField.uData.lValue;

		pLDTItem->GetField( iItemID, 2,			LDTField);			//"_ColorID"
		pItemAttr->m_iColorID			= LDTField.uData.lValue;

		pLDTItem->GetField( iItemID, 3,			LDTField);			//"_Name"
		pItemAttr->m_strName			= LDTField.uData.pValue;

		pLDTItem->GetField( iItemID, 4,			LDTField);			//"_Description"
		pItemAttr->m_strDescription		= LDTField.uData.pValue;

		pLDTItem->GetField( iItemID, 5,			LDTField);			//"_Icon"
		pItemAttr->m_iIcon			= LDTField.uData.lValue;

		pLDTItem->GetField( iItemID, 6,			LDTField);			//"_IconIndex"
		pItemAttr->m_iIconIndex			= LDTField.uData.lValue;

		pLDTItem->GetField( iItemID, 7,			LDTField);			//"_GamePrice"
		pItemAttr->m_iGamePrice			= LDTField.uData.lValue;

		pLDTItem->GetField( iItemID, 8,			LDTField);			//"_CashPrice"
		pItemAttr->m_iCashPrice			= LDTField.uData.lValue;

		pLDTItem->GetField( iItemID, 9,			LDTField);			//"_CashCheck"
		pItemAttr->m_bCashCheck			= (LDTField.uData.lValue) ? true : false;

		//_Update_Date(10) °ÔÀÓ»ó¿¡¼­´Â »ç¿ëÇÏÁö ¾ÊÀ½

		pLDTItem->GetField( iItemID, 11,			LDTField);		//"_RareLimit"
		pItemAttr->m_eRareLimit			= (ITEM_RARE_LIMIT)LDTField.uData.lValue;

		pLDTItem->GetField( iItemID, 12,			LDTField);		//"_Type"
		pItemAttr->m_eType				= (ITEM_TYPE)LDTField.uData.lValue;

		pLDTItem->GetField( iItemID, 13,			LDTField);		//"_SubType"
		pItemAttr->m_eSubType			= (ITEM_SUBTYPE)LDTField.uData.lValue;

		pLDTItem->GetField( iItemID, 14,			LDTField);		//"_PosID1"
		pItemAttr->m_ePosID1			= (ITEM_EQUIPPOS)LDTField.uData.lValue;

		pLDTItem->GetField( iItemID, 15,			LDTField);		//"_SubPosID11"
		pItemAttr->m_eSubPosID11		= (ITEM_EQUIPPOS)LDTField.uData.lValue;

		pLDTItem->GetField( iItemID, 16,			LDTField);		//"_SubPosID12"
		pItemAttr->m_eSubPosID12		= (ITEM_EQUIPPOS)LDTField.uData.lValue;

		pLDTItem->GetField( iItemID, 17,			LDTField);		//"_PosID2"
		pItemAttr->m_ePosID2			= (ITEM_EQUIPPOS)LDTField.uData.lValue;

		pLDTItem->GetField( iItemID, 18,			LDTField);		//"_SubPosID21"
		pItemAttr->m_eSubPosID21		= (ITEM_EQUIPPOS)LDTField.uData.lValue;

		pLDTItem->GetField( iItemID, 19,			LDTField);		//"_SubPosID22"
		pItemAttr->m_eSubPosID22		= (ITEM_EQUIPPOS)LDTField.uData.lValue;

		pLDTItem->GetField( iItemID, 20,			LDTField);		//"_Kind"
		pItemAttr->m_eKind				= (ITEM_EQUIP_KIND)LDTField.uData.lValue;

		pLDTItem->GetField( iItemID, 21,			LDTField);		//"_StatusType1"
		pItemAttr->m_StatusType[0].Effect	= (ITEM_EQUIP_EFFECT)LDTField.uData.lValue;
		pItemAttr->m_StatusTypePvp[0].Effect = pItemAttr->m_StatusType[0].Effect;

		pLDTItem->GetField( iItemID, 22,			LDTField);		//"_StatusValue1"
		pItemAttr->m_StatusType[0].iValue	= LDTField.uData.lValue;
		pItemAttr->m_StatusTypePvp[0].iValue = pItemAttr->m_StatusType[0].iValue;

		pLDTItem->GetField( iItemID, 23,			LDTField);		//"_StatusType2"
		pItemAttr->m_StatusType[1].Effect		= (ITEM_EQUIP_EFFECT)LDTField.uData.lValue;
		pItemAttr->m_StatusTypePvp[1].Effect = pItemAttr->m_StatusType[1].Effect;

		pLDTItem->GetField( iItemID, 24,			LDTField);		//"_StatusValue2"
		pItemAttr->m_StatusType[1].iValue		= LDTField.uData.lValue;
		pItemAttr->m_StatusTypePvp[1].iValue = pItemAttr->m_StatusType[1].iValue;

		pLDTItem->GetField( iItemID, 25,			LDTField);		//"_StatusType3"
		pItemAttr->m_StatusType[2].Effect		= (ITEM_EQUIP_EFFECT)LDTField.uData.lValue;
		pItemAttr->m_StatusTypePvp[2].Effect = pItemAttr->m_StatusType[2].Effect;

		pLDTItem->GetField( iItemID, 26,			LDTField);		//"_StatusValue3"
		pItemAttr->m_StatusType[2].iValue		= LDTField.uData.lValue;
		pItemAttr->m_StatusTypePvp[2].iValue = pItemAttr->m_StatusType[2].iValue;

		pLDTItem->GetField( iItemID, 27,			LDTField);		//"_StatusType4"
		pItemAttr->m_StatusType[3].Effect		= (ITEM_EQUIP_EFFECT)LDTField.uData.lValue;
		pItemAttr->m_StatusTypePvp[3].Effect = pItemAttr->m_StatusType[3].Effect;

		pLDTItem->GetField( iItemID, 28,			LDTField);		//"_StatusValue4"
		pItemAttr->m_StatusType[3].iValue		= LDTField.uData.lValue;
		pItemAttr->m_StatusTypePvp[2].iValue = pItemAttr->m_StatusType[2].iValue;

		//[2007/11/23] - PVP¿¡ »ç¿ëµÉ½Ã ¼Ó¼ºÀÌ ¹Ù²î´Â ¾ÆÀÌÅÛÀÎ °æ¿ì
		if(bLoadPvp && pLDTPvpItem) {
			pLDTPvpItem->GetField( iItemID, 21,			LDTField);		//"_StatusType1"
			pItemAttr->m_StatusTypePvp[0].Effect	= (ITEM_EQUIP_EFFECT)LDTField.uData.lValue;

			pLDTPvpItem->GetField( iItemID, 22,			LDTField);		//"_StatusValue1"
			pItemAttr->m_StatusTypePvp[0].iValue	= LDTField.uData.lValue;

			pLDTPvpItem->GetField( iItemID, 23,			LDTField);		//"_StatusType2"
			pItemAttr->m_StatusTypePvp[1].Effect	= (ITEM_EQUIP_EFFECT)LDTField.uData.lValue;

			pLDTPvpItem->GetField( iItemID, 24,			LDTField);		//"_StatusValue2"
			pItemAttr->m_StatusTypePvp[1].iValue	= LDTField.uData.lValue;

			pLDTPvpItem->GetField( iItemID, 25,			LDTField);		//"_StatusType3"
			pItemAttr->m_StatusTypePvp[2].Effect	= (ITEM_EQUIP_EFFECT)LDTField.uData.lValue;

			pLDTPvpItem->GetField( iItemID, 26,			LDTField);		//"_StatusValue3"
			pItemAttr->m_StatusTypePvp[2].iValue	= LDTField.uData.lValue;

			pLDTPvpItem->GetField( iItemID, 27,			LDTField);		//"_StatusType4"
			pItemAttr->m_StatusTypePvp[3].Effect	= (ITEM_EQUIP_EFFECT)LDTField.uData.lValue;

			pLDTPvpItem->GetField( iItemID, 28,			LDTField);		//"_StatusValue4"
			pItemAttr->m_StatusTypePvp[3].iValue	= LDTField.uData.lValue;
		}

		//	AJJIYA [11/18/2005]
		//	ÇÊ¿ä Á¶°Ç º¯°æ
		//char	szBuf[ MAX_PATH ];

		for ( int i = 0 ; i < MAX_ITEM_OPTION ; ++i )
		{
			//sprintf( szBuf , "_Require%d_Type"		, i + 1 );
			pLDTItem->GetField( iItemID , 29 + (i * 4) , LDTField );
			pItemAttr->m_RequireType[i].m_iType	=	LDTField.uData.lValue;

			//sprintf( szBuf , "_Require%d_ID"		, i + 1 );
			pLDTItem->GetField( iItemID , 30 + (i * 4) , LDTField );
			pItemAttr->m_RequireType[i].m_iID	=	LDTField.uData.lValue;

			//sprintf( szBuf , "_Require%d_Value1"	, i + 1 );
			pLDTItem->GetField( iItemID , 31 + (i * 4), LDTField );
			pItemAttr->m_RequireType[i].m_iValue1	=	LDTField.uData.lValue;

			//sprintf( szBuf , "_Require%d_Value2"	, i + 1 );
			pLDTItem->GetField( iItemID , 32 + (i * 4), LDTField );
			pItemAttr->m_RequireType[i].m_iValue2	=	LDTField.uData.lValue;
		}

		pLDTItem->GetField( iItemID, 45,			LDTField);		//"_CoolTime1"
		pItemAttr->m_iCoolTime1			= LDTField.uData.lValue;

		pLDTItem->GetField( iItemID, 46,			LDTField);		//"_CoolTime2"
		pItemAttr->m_iCoolTime2			= LDTField.uData.lValue;

		pLDTItem->GetField( iItemID, 47,			LDTField);		//"_CoolTimeApplyID1"
		pItemAttr->m_iCoolTimeApplyID1	= LDTField.uData.lValue;

		pLDTItem->GetField( iItemID, 48,			LDTField);		//"_CoolTimeApplyID2"
		pItemAttr->m_iCoolTimeApplyID2	= LDTField.uData.lValue;

		pLDTItem->GetField( iItemID, 49,			LDTField);		//"_ItemCoolTimeID"
		pItemAttr->m_iItemCoolTimeID	= LDTField.uData.lValue;



		pLDTItem->GetField( iItemID, 50,			LDTField);		//"_DurationTime"
		pItemAttr->m_iDurationTime		= LDTField.uData.lValue;

		pLDTItem->GetField( iItemID, 51,			LDTField);		//"_StackLimit"
		pItemAttr->m_iStackLimit		= LDTField.uData.lValue;

		pLDTItem->GetField( iItemID, 52,			LDTField);		//"_ItemLv"
		pItemAttr->m_iItemLv			= LDTField.uData.lValue;

		pLDTItem->GetField( iItemID, 53,			LDTField);		//"_Logging"
		pItemAttr->m_bLogging			= (LDTField.uData.lValue) ? true : false;

		pLDTItem->GetField( iItemID, 54,			LDTField);		//"_Serialized"
		pItemAttr->m_bSerialized		= (LDTField.uData.lValue) ? true : false;

		pLDTItem->GetField( iItemID, 55,			LDTField);		//"_Trade"
		pItemAttr->m_bTrade				= (LDTField.uData.lValue) ? true : false;

		pLDTItem->GetField( iItemID, 56,			LDTField);		//"_Destroy"
		pItemAttr->m_bDestroy			= (LDTField.uData.lValue) ? true : false;

		pLDTItem->GetField( iItemID, 57,			LDTField);		//"_Storage"
		pItemAttr->m_bStorage			= (LDTField.uData.lValue) ? true : false;

		pLDTItem->GetField( iItemID, 58,			LDTField);		//"_Sell"
		pItemAttr->m_bSell				= (LDTField.uData.lValue) ? true : false;

		pLDTItem->GetField( iItemID, 59,			LDTField);		//"_ItemPointMin"
		pItemAttr->m_iItemPointMin		= LDTField.uData.lValue;

		pLDTItem->GetField( iItemID, 60,			LDTField);		//"_ItemPointMax"
		pItemAttr->m_iItemPointMax		= LDTField.uData.lValue;

		pLDTItem->GetField( iItemID, 61,			LDTField);		//"_OptionID1"
		pItemAttr->m_iOptionID1			= LDTField.uData.lValue;

		pLDTItem->GetField( iItemID, 62,			LDTField);		//"_OptionID2"
		pItemAttr->m_iOptionID2			= LDTField.uData.lValue;

		pLDTItem->GetField( iItemID, 63,			LDTField);		//"_OptionID3"
		pItemAttr->m_iOptionID3			= LDTField.uData.lValue;

		pLDTItem->GetField( iItemID, 64,			LDTField);		//"_OptionID4"
		pItemAttr->m_iOptionID4			= LDTField.uData.lValue;

		pLDTItem->GetField( iItemID, 65,			LDTField);		//"_MotionTypeID1"
		pItemAttr->m_iMotionTypeID1		= LDTField.uData.lValue;

		pLDTItem->GetField( iItemID, 66,			LDTField);		//"_MotionTypeID2"
		pItemAttr->m_iMotionTypeID2		= LDTField.uData.lValue;

		//	AJJIYA [7/21/2006]
		//	È¿°ú ID Ãß°¡
		for ( i = 0 ; i < MAX_ITEM_EFFECT ; ++i )
		{
			//sprintf( szBuf , "_EffectID%d"		, i + 1 );
			pLDTItem->GetField( iItemID , 67 + i , LDTField );
			pItemAttr->m_iEffectID[ i ]	=	LDTField.uData.lValue;
		}

		pLDTItem->GetField( iItemID, 71,			LDTField);		//"_OptionType"
		pItemAttr->m_iOptionType			= LDTField.uData.lValue;

		pLDTItem->GetField( iItemID, 72,			LDTField);		//"_OptionClass"
		pItemAttr->m_iOptionClass			= LDTField.uData.lValue;

		pLDTItem->GetField( iItemID, 73,			LDTField);		//"_IconMark"
		pItemAttr->m_eIconMark			= (ICON_MARK)LDTField.uData.lValue;

		pLDTItem->GetField( iItemID, 74,			LDTField);		//"_NameColor"
		pItemAttr->m_bNameColor			= (LDTField.uData.lValue) ? true : false;

		if(pItemAttr->m_bNameColor) {
			pLDTItem->GetField( iItemID, 75,		LDTField);		//"_Red"
			pItemAttr->m_cRed			= LDTField.uData.lValue;

			pLDTItem->GetField( iItemID, 76,		LDTField);		//"_Green"
			pItemAttr->m_cGreen			= LDTField.uData.lValue;

			pLDTItem->GetField( iItemID, 77,		LDTField);		//"_Blue"
			pItemAttr->m_cBlue			= LDTField.uData.lValue;
		}

		pLDTItem->GetField( iItemID, 78,			LDTField);		//"_Time_Type"
		pItemAttr->m_eDuration			= (ITEM_DURATION)LDTField.uData.lValue;

		pLDTItem->GetField( iItemID, 79,			LDTField);		//"_Lifetime"
		pItemAttr->m_iLifeTime			= LDTField.uData.lValue;

		//_Default_Type (80)

		pLDTItem->GetField( iItemID, 81,			LDTField);		//"_Max_EXP"
		if(LDTField.uData.pValue) {
			pItemAttr->m_iPetMaxExp			= _atoi64(LDTField.uData.pValue);
		}

		//_Limit_EXP(82)

		pLDTItem->GetField( iItemID, 83,			LDTField);		//"_Pet_ClassID"
		pItemAttr->m_iPetClassID		= LDTField.uData.lValue;

		for( i = 0; i < MAX_ITEM_SKILL; i++ )
		{
			//sprintf(szBuf , "_Skill%d", i + 1 );
			pLDTItem->GetField(iItemID, 84 + (i * 3), LDTField);
			pItemAttr->m_iSkillID[i] = LDTField.uData.lValue;

			//sprintf(szBuf , "_SLv%d", i + 1 );
			pLDTItem->GetField(iItemID, 85 + (i * 3), LDTField);
			pItemAttr->m_iSkillLevel[i] = LDTField.uData.lValue;

			//sprintf(szBuf , "_Consume_SP%d", i + 1 );
			pLDTItem->GetField(iItemID, 86 + (i * 3), LDTField);
			pItemAttr->m_iConsumeSP[i] = LDTField.uData.lValue;
		}

		pLDTItem->GetField( iItemID, 96,			LDTField);		//"_Macro_Type"
		pItemAttr->m_iMacroType = LDTField.uData.lValue;

		pLDTItem->GetField( iItemID, 97,			LDTField);		//"SetItem"
		pItemAttr->m_iSetID = LDTField.uData.lValue;

		pLDTItem->GetField( iItemID, 98,			LDTField);		//"QuestID"
		pItemAttr->m_iQuestID = LDTField.uData.lValue;

		pLDTItem->GetField( iItemID, 99,			LDTField);		//_PvpUse
		pItemAttr->m_bPvp = static_cast<bool>(LDTField.uData.lValue);		

		pLDTItem->GetField( iItemID, 100,			LDTField);		//_CashPoint
		pItemAttr->m_iCashPoint = LDTField.uData.lValue;
		
		pLDTItem->GetField( iItemID, 101,			LDTField);		//_ItemLvValue
		pItemAttr->m_iItemLevel = LDTField.uData.lValue;

		pLDTItem->GetField( iItemID, 102,			LDTField);		//_AuctionCategory
		pItemAttr->m_iAuctionCategory = LDTField.uData.lValue;

		pLDTItem->GetField( iItemID, 103,			LDTField);		//_AuctionQuality
		pItemAttr->m_iAuctionQuality = LDTField.uData.lValue;

		pLDTItem->GetField( iItemID, 104,			LDTField);		//_ShopID
		pItemAttr->m_iShopID = LDTField.uData.lValue;

		pLDTItem->GetField( iItemID, 105,			LDTField);		//_NewPet
		pItemAttr->m_iNewPet = LDTField.uData.lValue;

		pLDTItem->GetField( iItemID, 106,			LDTField);		//_WeddingItem
		pItemAttr->m_bWedding = static_cast<bool>(LDTField.uData.lValue);

		hash_map<UINT32, SPItemAttr*>::iterator hmIterItem = m_hmItem.find(iItemID);
		if(hmIterItem != m_hmItem.end() && (iItemID == (*hmIterItem).first) ) {
			assert(0 && "Item ID Áßº¹");
			continue;
		} else {
			m_hmItem.insert ( hash_map<UINT32, SPItemAttr*>::value_type( iItemID, pItemAttr) );			
		}

		hash_map<UINT32, SPTexture*>::iterator hmIterTexture = m_hmIconTexture.find(pItemAttr->m_iIcon);
		if(hmIterTexture != m_hmIconTexture.end() && (iItemID == (*hmIterTexture).first) ) {	
			continue;
		} else {
			SPTexture* pTexture;
			string strResource; // = RES_WORK_RESOURCE_PATH;
			const char* pszFileName = g_pResourceManager->GetGlobalFileName( pItemAttr->m_iIcon );

			if( pszFileName ) 
				strResource += g_pResourceManager->GetGlobalFileName( pItemAttr->m_iIcon );

			//g_pVideo->GetTextureMgr()->LoadTexture(strResource.c_str(), &pTexture);
			g_pVideo->GetTextureMgr()->GetTexture(strResource.c_str(), &pTexture,TEX_ITEM,LEVEL_NOT);
			m_hmIconTexture.insert( hash_map<UINT32, SPTexture*>::value_type( pItemAttr->m_iIcon, pTexture) );
		}
	}

	if(bLoadPvp && pLDTPvpItem) {
		SAFE_RELEASE(pLDTPvpItem);
	}

	SAFE_RELEASE(pLDTItem);

	return true;
}


bool SPItemCluster::LoadLDTItemSet()
{
	SPLDTFile* pLDTSet;
	g_pResourceManager->GetLDTFromFileName(RES_FNAME_LDT_SETITEM_2, &pLDTSet);
	if(pLDTSet == NULL) return false;

	int iIndex = 0;	
	int iSetCount = 0;
	LDT_Field LDTField;
	int iNORec = pLDTSet->GetItemCount();
	
	SPItemSet	ItemSet;
	for(int iRecord = 0; iRecord < iNORec; ++iRecord ) {
		ItemSet.Clear();
		iIndex = 0;
		iIndex = pLDTSet->GetPrimaryKey( iRecord );
		ItemSet.m_iSetID = iIndex;
		if(ItemSet.m_iSetID == 0)
			continue;	

		pLDTSet->GetField( iIndex, 0,			LDTField);
		if(LDTField.uData.pValue) {
			ItemSet.m_strName = LDTField.uData.pValue;
		}


		ITEM_SET_OPTION_TYPE iType = ITEM_SET_OPTION_TYPE_NULL;
		int iStat = 0;
		int iValue = 0;
		for(int j = 0; j < ITEM_SET_A_MAX; j++) {
			iType = ITEM_SET_OPTION_TYPE_NULL;
			iStat = 0;
			iValue = 0;
			pLDTSet->GetField( iIndex, 1 + (j * 3),		LDTField);
			iType = (ITEM_SET_OPTION_TYPE)LDTField.uData.lValue;
			
			pLDTSet->GetField( iIndex, 2 + (j * 3),		LDTField);
			iStat = LDTField.uData.lValue;
			
			pLDTSet->GetField( iIndex, 3 + (j * 3),		LDTField);
			iValue = LDTField.uData.lValue;

			if(ItemSet.GetEffect(j))
				ItemSet.GetEffect(j)->SetEffect(iType, iStat, iValue);
		}

		pLDTSet->GetField( iIndex, 34,			LDTField);
		ItemSet.m_iMaxPieces = LDTField.uData.lValue;

		for(int j = 0; j < ITEM_SET_FUUL_OPTION; j++) {
			iType = ITEM_SET_OPTION_TYPE_NULL;
			iStat = 0;
			iValue = 0;
			
			pLDTSet->GetField( iIndex, 35 + (j * 3),		LDTField);
			iType = (ITEM_SET_OPTION_TYPE)LDTField.uData.lValue;

			pLDTSet->GetField( iIndex, 36 + (j * 3),		LDTField);
			iStat = LDTField.uData.lValue;

			pLDTSet->GetField( iIndex, 37 + (j * 3),		LDTField);
			iValue = LDTField.uData.lValue;

			if(ItemSet.GetFullEffect(j))
				ItemSet.GetFullEffect(j)->SetEffect(iType, iStat, iValue);
		}

		InsertItemSet( ItemSet );
	}

	SAFE_RELEASE(pLDTSet);
	return true;
}

bool SPItemCluster::LoadLDTMix()
{
	SPLDTFile* pLDTFile = NULL;

	g_pResourceManager->GetLDTFromFileName( RES_FNAME_LDT_ITEM_MIX , &pLDTFile);

	if( pLDTFile == NULL )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "----------------------------------->ITEM MIX LDT FILE LOAD FAIL [%s] \n" , RES_FNAME_LDT_ITEM_MIX );
#endif
		return false;
	}

	int				iItemID , i, j;
	LDT_Field		LDTField;
	int				iRecordCount	= pLDTFile->GetItemCount();
	SPItemMix		stItemMix;

	for( i = 0; i < iRecordCount; ++i )
	{
		iItemID	=	pLDTFile->GetPrimaryKey( i );

		//////////////////////////////////////////////////////////////////////////

		pLDTFile->GetField( iItemID ,  3, LDTField );	//	_Skill
		stItemMix.m_iSkillID	=	LDTField.uData.lValue;

		if( stItemMix.m_iSkillID == 0 )
			continue;

		//////////////////////////////////////////////////////////////////////////

		stItemMix.m_iItemMixID		=	iItemID;

		pLDTFile->GetField( iItemID , 0, LDTField );		//	_Name
		if( LDTField.uData.pValue != NULL )
		{
			if( strlen( LDTField.uData.pValue ) > 0 )
			{
				stItemMix.m_strName		=	LDTField.uData.pValue;
			}
		}

		pLDTFile->GetField( iItemID ,  1, LDTField );		//	_Result
		stItemMix.m_iResult	=	LDTField.uData.lValue;

		pLDTFile->GetField( iItemID ,  52, LDTField );		//	_Success_Count
		stItemMix.m_iResultCount	=	LDTField.uData.lValue;

		pLDTFile->GetField( iItemID ,  2, LDTField );		//	_When_Fails
		stItemMix.m_iWhenFails	=	LDTField.uData.lValue;

		pLDTFile->GetField( iItemID ,  53, LDTField );		//	_Fails_Count
		stItemMix.m_iWhenFailsCount	=	LDTField.uData.lValue;

		pLDTFile->GetField( iItemID ,  4, LDTField );	//	_Category
		stItemMix.m_iCategory	=	LDTField.uData.lValue;

		for( j = 0 ; j < MAX_ITEM_UPGRADE_REQUIRE ; ++j )
		{
			//sprintf( szBuf , "_Require%d_Type" , j + 1 );
			pLDTFile->GetField( iItemID , 5 + (j * 4), LDTField );
			stItemMix.m_stRequireType[ j ].m_iType		=	LDTField.uData.lValue;

			//sprintf( szBuf , "_Require%d_ID" , j + 1 );
			pLDTFile->GetField( iItemID , 6 + (j * 4) , LDTField );
			stItemMix.m_stRequireType[ j ].m_iID		=	LDTField.uData.lValue;

			//sprintf( szBuf , "_Require%d_Value1" , j + 1 );
			pLDTFile->GetField( iItemID , 7 + (j * 4) , LDTField );
			stItemMix.m_stRequireType[ j ].m_iValue1	=	LDTField.uData.lValue;

			//sprintf( szBuf , "_Require%d_Value2" , j + 1 );
			pLDTFile->GetField( iItemID , 8 + (j * 4) , LDTField );
			stItemMix.m_stRequireType[ j ].m_iValue2	=	LDTField.uData.lValue;
		}

		for( j = 0 ; j < MAX_ITEM_UPGRADE_MATERIAL ; ++j )
		{
			//sprintf( szBuf , "_Material%d" , j + 1 );
			pLDTFile->GetField( iItemID , 21 + (j * 3) , LDTField );
			stItemMix.m_stMaterial[ j ].m_iID		=	LDTField.uData.lValue;

			//sprintf( szBuf , "_Material_Count%d" , j + 1 );
			pLDTFile->GetField( iItemID , 22 + (j * 3) , LDTField );
			stItemMix.m_stMaterial[ j ].m_iCount	=	LDTField.uData.lValue;

			//sprintf( szBuf , "_Material_Rare%d" , j + 1 );
			pLDTFile->GetField( iItemID , 23 + (j * 3) , LDTField );
			stItemMix.m_stMaterial[ j ].m_eRare	=	(ITEM_UPGRADE_RARE)LDTField.uData.lValue;
		}


		pLDTFile->GetField( iItemID ,  36, LDTField );	//	_Opt_Min
		stItemMix.m_iOptMin	=	LDTField.uData.lValue;

		pLDTFile->GetField( iItemID ,  37, LDTField );	//	_Opt_Max
		stItemMix.m_iOptMax	=	LDTField.uData.lValue;

		pLDTFile->GetField( iItemID ,  38, LDTField );	//	_Success_Probability
		stItemMix.m_iSuccess_Probability	=	LDTField.uData.lValue;

		for( j = 0 ; j < MAX_ITEM_MIX_RARE_PROBABILITY ; ++j )
		{
			pLDTFile->GetField( iItemID , 39 + j , LDTField );
			stItemMix.m_iRare_Probability[ j ]	=	LDTField.uData.lValue;
		}

		pLDTFile->GetField( iItemID ,  44, LDTField );	//	_Updex_Fail_Min
		stItemMix.m_iUpDex_Fail_Min	=	LDTField.uData.lValue;

		pLDTFile->GetField( iItemID ,  45, LDTField );	//	_Updex_Fail_Max
		stItemMix.m_iUpDex_Fail_Max	=	LDTField.uData.lValue;

		pLDTFile->GetField( iItemID ,  46, LDTField );	//	_Updex_Success_Min
		stItemMix.m_iUpDex_Success_Min	=	LDTField.uData.lValue;

		pLDTFile->GetField( iItemID ,  47, LDTField );	//	_Updex_Success_Max
		stItemMix.m_iUpDex_Success_Max	=	LDTField.uData.lValue;

		pLDTFile->GetField( iItemID ,  48, LDTField );	//	_Base_Lv
		stItemMix.m_iBase_Lv	=	LDTField.uData.lValue;

		pLDTFile->GetField( iItemID ,  49, LDTField );	//	_Success_Probability_Min
		stItemMix.m_iSuccess_Probability_Min	=	LDTField.uData.lValue;

		pLDTFile->GetField( iItemID ,  50, LDTField );	//	_Success_Probability_Max
		stItemMix.m_iSuccess_Probability_Max	=	LDTField.uData.lValue;

		pLDTFile->GetField( iItemID ,  51, LDTField );	//	_Success_Probability_Revision
		stItemMix.m_iSuccess_Probability_Revision	=	LDTField.uData.lValue;

		InsertItemMix( stItemMix );
	}

	SAFE_RELEASE(pLDTFile);

	return true;
}

bool SPItemCluster::LoadLDTCashMiss()
{
	m_vecRandItem.clear();

	SPLDTFile*	pLDTItem	=	NULL;

	g_pResourceManager->GetLDTFromFileName( "DATA/LDT/cash_miss.ldt" , &pLDTItem );
	
	if( pLDTItem == NULL )
		return false;

	SPCashItemMiss	stItemMiss;
	memset(&stItemMiss, 0x00, sizeof(stItemMiss));
	LDT_Field		LDTField;
	int				iItemID;
	int				iItemCount	= pLDTItem->GetItemCount();

	for(int iRecord = 0; iRecord < iItemCount; ++iRecord )
	{
		iItemID = pLDTItem->GetPrimaryKey( iRecord );

		pLDTItem->GetField( iItemID , 1	,	LDTField );
		stItemMiss.iItemID	=	LDTField.uData.lValue;
		
		pLDTItem->GetField( iItemID , 2	,	LDTField );
		stItemMiss.iCashItemID	=	LDTField.uData.lValue;

		stItemMiss.iCategory = CI_NULL;

		m_vecRandItem.push_back(stItemMiss);
	}

	SAFE_RELEASE(pLDTItem);
	return true;
}

bool SPItemCluster::LoadLDTSultList()
{
	m_vecCaseItem.clear();

	SPLDTFile*	pLDTItem	=	NULL;

	g_pResourceManager->GetLDTFromFileName( "DATA/LDT/suit_list.ldt" , &pLDTItem );
	
	if( pLDTItem == NULL )
		return false;

	SPCashItemSultlist	stItemSult;
	memset(&stItemSult, 0x00, sizeof(stItemSult));
	LDT_Field		    LDTField;
	int				    iItemID;
	int				    iItemCount	= pLDTItem->GetItemCount();

	for(int iRecord = 0; iRecord < iItemCount; ++iRecord )
	{
		iItemID = pLDTItem->GetPrimaryKey( iRecord );

		pLDTItem->GetField( iItemID , 1	,	LDTField );
		stItemSult.iCashItem[0]	=	LDTField.uData.lValue;
		
		pLDTItem->GetField( iItemID , 2	,	LDTField );
		stItemSult.iCashItem[1]	=	LDTField.uData.lValue;

		pLDTItem->GetField( iItemID , 3	,	LDTField );
		stItemSult.iCashItem[2]	=	LDTField.uData.lValue;
		
		pLDTItem->GetField( iItemID , 4	,	LDTField );
		stItemSult.iCashItem[3]	=	LDTField.uData.lValue;

		pLDTItem->GetField( iItemID , 5	,	LDTField );
		stItemSult.iCashItem[4]	=	LDTField.uData.lValue;
		
		pLDTItem->GetField( iItemID , 6	,	LDTField );
		stItemSult.iCashItem[5]	=	LDTField.uData.lValue;

		pLDTItem->GetField( iItemID , 7	,	LDTField );
		stItemSult.iCashItem[6]	=	LDTField.uData.lValue;
		
		pLDTItem->GetField( iItemID , 8	,	LDTField );
		stItemSult.iCashItem[7]	=	LDTField.uData.lValue;

		pLDTItem->GetField( iItemID , 9	,	LDTField );
		stItemSult.iCashItem[8]	=	LDTField.uData.lValue;
		
		pLDTItem->GetField( iItemID , 10	,	LDTField );
		stItemSult.iCashItem[9]	=	LDTField.uData.lValue;

		m_vecCaseItem.push_back(stItemSult);
	}

	SAFE_RELEASE(pLDTItem);
	return true;
}

bool SPItemCluster::LoadLDTCashEffect()
{
	m_vecCaseEffect.clear();

	SPLDTFile*	pLDTItem	=	NULL;

	g_pResourceManager->GetLDTFromFileName( "DATA/LDT/cash_effect.ldt" , &pLDTItem );
	
	if( pLDTItem == NULL )
		return false;

	SPCashEffectItem	stEffect;
	LDT_Field			LDTField;
	int					iItemID;
	int					iItemCount	= pLDTItem->GetItemCount();

	for(int iRecord = 0; iRecord < iItemCount; ++iRecord )
	{
		iItemID = pLDTItem->GetPrimaryKey( iRecord );

		stEffect.iItemID = iItemID;

		pLDTItem->GetField( iItemID , 0	,	LDTField );
		stEffect.iCashNum	=	LDTField.uData.lValue;
		
		pLDTItem->GetField( iItemID , 1	,	LDTField );
		stEffect.iEffectID[0]	=	LDTField.uData.lValue;

		pLDTItem->GetField( iItemID , 2	,	LDTField );
		stEffect.iEffectID[1]	=	LDTField.uData.lValue;

		pLDTItem->GetField( iItemID , 3	,	LDTField );
		stEffect.iEffectID[2]	=	LDTField.uData.lValue;

		pLDTItem->GetField( iItemID , 4	,	LDTField );
		stEffect.iEffectID[3]	=	LDTField.uData.lValue;

		pLDTItem->GetField( iItemID , 5	,	LDTField );
		stEffect.strName	=	LDTField.uData.pValue;

		m_vecCaseEffect.push_back(stEffect);
	}

	SAFE_RELEASE(pLDTItem);
	return true;
}

bool SPItemCluster::LoadLDTOption()
{
	m_mItemOption.clear();

	SPLDTFile*	pLDTItemOption	=	NULL;

	g_pResourceManager->GetLDTFromFileName( "DATA/LDT/ITEM_OPTION.LDT" , &pLDTItemOption );
	
	if( pLDTItemOption == NULL )
		return false;

	SPItemOption	stItemOption;
	LDT_Field		LDTField;
	int				iItemID;
	int				iItemCount	= pLDTItemOption->GetItemCount();

	for(int iRecord = 0; iRecord < iItemCount; ++iRecord )
	{
		iItemID = pLDTItemOption->GetPrimaryKey( iRecord );

		stItemOption.SetNull();

		pLDTItemOption->GetField( iItemID , 0	,	LDTField );		//"_Name"
		stItemOption.m_strName		=	LDTField.uData.pValue;

		pLDTItemOption->GetField( iItemID , 1	,	LDTField );		//"_Description"
		stItemOption.m_strDesc		=	LDTField.uData.pValue;
		
		pLDTItemOption->GetField( iItemID , 2	,	LDTField );		//"_Application_Type"
		stItemOption.m_iAppType	=	LDTField.uData.lValue;

		pLDTItemOption->GetField( iItemID , 3	,	LDTField );		//"_OptionGroup"
		stItemOption.m_iOptionGroup	=	LDTField.uData.lValue;

		pLDTItemOption->GetField( iItemID , 4	,	LDTField );		//"_OptionLevel"
		stItemOption.m_iOptionLevel	=	LDTField.uData.lValue;

		pLDTItemOption->GetField( iItemID , 5	,	LDTField );		//"_Effect"
		stItemOption.m_iEffect		=	LDTField.uData.lValue;

		pLDTItemOption->GetField( iItemID , 6	,	LDTField );		//"_Effect_Type"
		stItemOption.m_iEffectType	=	LDTField.uData.lValue;

		pLDTItemOption->GetField( iItemID , 7	,	LDTField );		//"_Option_Min"
		stItemOption.m_iOptionMin	=	LDTField.uData.lValue;

		pLDTItemOption->GetField( iItemID , 8	,	LDTField );		//"_Option_Max"
		stItemOption.m_iOptionMax	=	LDTField.uData.lValue;

		pLDTItemOption->GetField( iItemID , 9	,	LDTField );		//"_Enchant_Min"
		stItemOption.m_iEnchantMin	=	LDTField.uData.lValue;

		pLDTItemOption->GetField( iItemID , 10	,	LDTField );		//"_Enchant_Max"
		stItemOption.m_iEnchantMax	=	LDTField.uData.lValue;
		

		if( GetItemOption( iItemID ) == NULL )
		{
			m_mItemOption.insert( std::map< UINT32 , SPItemOption >::value_type( iItemID , stItemOption ) );
		}
	}

	SAFE_RELEASE(pLDTItemOption);
	return true;
}

bool SPItemCluster::LoadLDTCashItem()
{
	m_vCashItem.clear();

	m_iCashMajorVersion = 0;
	m_iCashMinorVersion = 0;
	
	SPLDTFile* pLDTFile = NULL;
	g_pResourceManager->GetLDTFromFileName("DATA/LDT/CASH_SHOP.LDT", &pLDTFile);
	if( pLDTFile == NULL )
		return false;

	int iItemID, iSize, i, j;
	LDT_Field LDTField;
	SPCashItemAttr ItemAttr;
	//char szBuf[128];

	iSize = pLDTFile->GetItemCount();
	for( i = 0; i < iSize; i++ )
	{
		iItemID = pLDTFile->GetPrimaryKey(i);

		pLDTFile->GetField(iItemID, 0, LDTField);		//"_CategoryView"
		ItemAttr.bView = LDTField.uData.lValue ? true : false;

		pLDTFile->GetField(iItemID, 1, LDTField);		//"_CategoryIndex"
		ItemAttr.eCategory = (CASHITEM_CATEGORY)LDTField.uData.lValue;

		pLDTFile->GetField(iItemID, 2, LDTField);		//"_CategoryIndex2"
		ItemAttr.eCategorySub1 = (CASHITEM_CATEGORY_SUB1)LDTField.uData.lValue;

		pLDTFile->GetField(iItemID, 3, LDTField);		//"_CategoryIndex3"
		ItemAttr.eCategorySub2 = (CASHITEM_CATEGORY_SUB2)LDTField.uData.lValue;

		pLDTFile->GetField(iItemID, 4, LDTField);		//"_PositionIndex"
		ItemAttr.iPositionIndex = LDTField.uData.lValue;

		pLDTFile->GetField(iItemID, 5, LDTField);		//"_EventPage"
		ItemAttr.iEventPage = LDTField.uData.lValue;

		pLDTFile->GetField(iItemID, 6, LDTField);		//"_Name"
		ItemAttr.strName = LDTField.uData.pValue;

		pLDTFile->GetField(iItemID, 7, LDTField);		//"_Description"
		ItemAttr.strDescription = LDTField.uData.pValue;

		pLDTFile->GetField(iItemID, 8, LDTField);		//"_IconFile"
		ItemAttr.iIcon = LDTField.uData.lValue;

		pLDTFile->GetField(iItemID, 9, LDTField);		//"_IconIndex"
		ItemAttr.iIconIndex = LDTField.uData.lValue;

		pLDTFile->GetField(iItemID, 10, LDTField);		//"_Gender"
		ItemAttr.iGender = LDTField.uData.lValue;

		pLDTFile->GetField(iItemID, 11, LDTField);		//"_CashPrice"
		ItemAttr.iPrice = LDTField.uData.lValue;

		pLDTFile->GetField(iItemID, 12, LDTField);		//"_SalePrice"
		ItemAttr.iSalePrice = LDTField.uData.lValue;

		pLDTFile->GetField(iItemID, 13, LDTField);		//"_EventIcon"
		ItemAttr.iEventIcon = LDTField.uData.lValue;

		pLDTFile->GetField(iItemID, 14, LDTField);		//"_MaxOz"
		ItemAttr.iMaxOZ = LDTField.uData.lValue;

		pLDTFile->GetField(iItemID, 15, LDTField);		//"_MaxMile"
		ItemAttr.iMaxMile = LDTField.uData.lValue;

		for( j = 0; j < 8; j++ )
		{
			//sprintf(szBuf, "_ItemID%d", j + 1);
			pLDTFile->GetField(iItemID, 16 + (j * 4), LDTField);
			ItemAttr.ItemInfo[j].iID = LDTField.uData.lValue;

			//sprintf(szBuf, "_ItemRare%d", j + 1);
			pLDTFile->GetField(iItemID, 17 + (j * 4), LDTField);
			ItemAttr.ItemInfo[j].iRare = LDTField.uData.lValue;

			//sprintf(szBuf, "_ItemCount%d", j + 1);
			pLDTFile->GetField(iItemID, 18 + (j * 4), LDTField);
			ItemAttr.ItemInfo[j].iCount = LDTField.uData.lValue;

			//sprintf(szBuf, "_PriceRate%d", j + 1);
			pLDTFile->GetField(iItemID, 19 + (j * 4), LDTField);
			ItemAttr.ItemInfo[j].iPriceRate = LDTField.uData.lValue;
		}

		pLDTFile->GetField(iItemID, 48, LDTField);		//"_MotionID"
		ItemAttr.iMotionID = LDTField.uData.lValue;

		pLDTFile->GetField(iItemID, 49, LDTField);		//"_ClassID"
		ItemAttr.iClassID = LDTField.uData.lValue;

		for( j = 0; j < CASHITEM_REQUIRE_NUM; j++ )
		{
			pLDTFile->GetField(iItemID, 50 + (j * 4), LDTField);
			ItemAttr.ItemRequire[j].m_iType = LDTField.uData.lValue;

			pLDTFile->GetField(iItemID, 51 + (j * 4), LDTField);
			ItemAttr.ItemRequire[j].m_iID = LDTField.uData.lValue;

			pLDTFile->GetField(iItemID, 52 + (j * 4), LDTField);
			ItemAttr.ItemRequire[j].m_iValue1 = LDTField.uData.lValue;

			pLDTFile->GetField(iItemID, 53 + (j * 4), LDTField);
			ItemAttr.ItemRequire[j].m_iValue2 = LDTField.uData.lValue;
		}

		pLDTFile->GetField(iItemID, 62, LDTField);		//"_Buy_Disable"
		ItemAttr.bBuyDisable = LDTField.uData.lValue ? false : true;

		pLDTFile->GetField(iItemID, 63, LDTField);		//"_Give_Disable"
		ItemAttr.bGiftDisable = LDTField.uData.lValue ? false : true;

		ItemAttr.iCashID = iItemID;
		m_vCashItem.push_back(ItemAttr);

		//UpdateItemCashPrize(&ItemAttr);
	}

	SAFE_RELEASE(pLDTFile);
	SortCashItem();
	InitMinorCashItem();
	return true;
}

bool SPItemCluster::LoadLDTEnchant()
{
	SPLDTFile* pLDTFile = NULL;
	g_pResourceManager->GetLDTFromFileName("DATA/LDT/ITEM_ENCHANT.LDT", &pLDTFile);
	if( pLDTFile == NULL )
		return false;

	int iItemID, iSize, i, j;
	LDT_Field LDTField;
	SPItemEnchant ItemEnchant;
	//char szBuf[256];

	iSize = pLDTFile->GetItemCount();
	for( i = 0; i < iSize; i++ )
	{
		iItemID = pLDTFile->GetPrimaryKey(i);
		ItemEnchant.iEnchantKey = iItemID;
		
		pLDTFile->GetField(iItemID, 0, LDTField);		//"_Description"
		ItemEnchant.strDescription = LDTField.uData.pValue;

		pLDTFile->GetField(iItemID, 1, LDTField);		//"_Result"
		ItemEnchant.iResult = LDTField.uData.lValue;

		pLDTFile->GetField(iItemID, 2, LDTField);		//"_When_Fails"
		ItemEnchant.iWhenFailed = LDTField.uData.lValue;

		for( j = 0 ; j < 4 ; j++ )
		{
			//sprintf(szBuf, "_Require%d_Type", j + 1 );
			pLDTFile->GetField(iItemID, 3 + (j * 4), LDTField);
			ItemEnchant.RequireType[j].m_iType = LDTField.uData.lValue;

			//sprintf(szBuf, "_Require%d_ID", j + 1 );
			pLDTFile->GetField(iItemID, 4 + (j * 4), LDTField);
			ItemEnchant.RequireType[j].m_iID = LDTField.uData.lValue;

			//sprintf(szBuf, "_Require%d_Value1", j + 1 );
			pLDTFile->GetField(iItemID, 5 + (j * 4), LDTField);
			ItemEnchant.RequireType[j].m_iValue1 = LDTField.uData.lValue;

			//sprintf(szBuf, "_Require%d_Value2", j + 1 );
			pLDTFile->GetField(iItemID, 6 + (j * 4), LDTField);
			ItemEnchant.RequireType[j].m_iValue2 = LDTField.uData.lValue;
		}

		pLDTFile->GetField(iItemID, 19, LDTField);		//"_Item_OptionType"
		ItemEnchant.iItemOptionType = LDTField.uData.lValue;

		pLDTFile->GetField(iItemID, 20, LDTField);		//"_Item_OptionClass"
		ItemEnchant.iItemOptionClass = LDTField.uData.lValue;

		pLDTFile->GetField(iItemID, 21, LDTField);		//"_Item_OptionGroup"
		ItemEnchant.iItemOptionGroup = LDTField.uData.lValue;

		pLDTFile->GetField(iItemID, 22, LDTField);		//"_Item_OptionLevel"
		ItemEnchant.iItemOptionLevel = LDTField.uData.lValue;

		pLDTFile->GetField(iItemID, 23, LDTField);		//"_Item_OptionGroup_Check"
		ItemEnchant.bItemOptionCheck = (bool)LDTField.uData.lValue;

		pLDTFile->GetField(iItemID, 24, LDTField);		//"_Enchant_Material"
		ItemEnchant.iMaterialID = LDTField.uData.lValue;

		pLDTFile->GetField(iItemID, 25, LDTField);		//"_Enchant_Material_Count"
		ItemEnchant.iMaterialNum = LDTField.uData.lValue;

		pLDTFile->GetField(iItemID, 26, LDTField);		//"_Consumption_OP_Min"
		ItemEnchant.iOPMin = LDTField.uData.lValue;

		pLDTFile->GetField(iItemID, 27, LDTField);		//"_Consumption_OP_Max"
		ItemEnchant.iOPMax = LDTField.uData.lValue;

		pLDTFile->GetField(iItemID, 28, LDTField);		//"_Probability_Modifier"
		ItemEnchant.iProbabilityModifier = LDTField.uData.lValue;

		pLDTFile->GetField(iItemID, 29, LDTField);		//"_Probability_Min"
		ItemEnchant.iProbabilityMin = LDTField.uData.lValue;

		pLDTFile->GetField(iItemID, 30, LDTField);		//"_Probability_Max"
		ItemEnchant.iProbabilityMax = LDTField.uData.lValue;		

		pLDTFile->GetField(iItemID, 31, LDTField);		//"_Error"
		ItemEnchant.strErrorMsg = LDTField.uData.pValue;				
		
		InsertItemEnchant(ItemEnchant.iItemOptionType, ItemEnchant);
	}

	SAFE_RELEASE(pLDTFile);
	return true;
}

bool SPItemCluster::LoadLDTUpgrade()
{
	SPLDTFile* pLDTFile = NULL;

	g_pResourceManager->GetLDTFromFileName( RES_FNAME_LDT_ITEM_UPGRADE , &pLDTFile);

	if( pLDTFile == NULL )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "----------------------------------->ITEM UPGRADE LDT FILE LOAD FAIL [%s] \n" , RES_FNAME_LDT_ITEM_UPGRADE );
#endif
		return false;
	}

	int				iItemID , i, j;
	LDT_Field		LDTField;
	//char			szBuf[_MAX_PATH];
	int				iRecordCount	= pLDTFile->GetItemCount();
	SPItemUpgrade	ItemUpgrade;

	for( i = 0; i < iRecordCount; ++i )
	{
		iItemID	=	pLDTFile->GetPrimaryKey( i );
		ItemUpgrade.m_iLDTItemID	=	iItemID;

		pLDTFile->GetField( iItemID , 0, LDTField );		//"_Description" 
		if( LDTField.uData.pValue != NULL )
		{
			if( strlen( LDTField.uData.pValue ) > 0 )
			{
				ItemUpgrade.m_strDesc	=	LDTField.uData.pValue;
			}
		}

		pLDTFile->GetField( iItemID ,  1, LDTField );		//"_Result"
		ItemUpgrade.m_iResult	=	LDTField.uData.lValue;

		pLDTFile->GetField( iItemID ,  2, LDTField );		//"_When_Fails"

		if( LDTField.uData.lValue == 1 )
		{
			ItemUpgrade.m_eWhenFails	=	ITEM_UPGRADE_WHENFAILS_ORIGINAL;
		}
		else if( LDTField.uData.lValue == 2 )
		{
			ItemUpgrade.m_eWhenFails	=	ITEM_UPGRADE_WHENFAILS_MATERIAL;
		}
		else if( LDTField.uData.lValue == 3 )
		{
			ItemUpgrade.m_eWhenFails	=	ITEM_UPGRADE_WHENFAILS_BOTH;
		}
		else
		{
#ifdef _DEBUG
			DXUTOutputDebugString( "----------------------------------->ITEM UPGRADE LDT WHEN FAILS VALUE 0 [ KEY : %d ITEM ID : %d ITEM RARE : %d ] \n" , iItemID , ItemUpgrade.m_iItemID_Original , ItemUpgrade.m_eRare_Original );
#endif
			continue;
		}

		pLDTFile->GetField( iItemID ,  3, LDTField );	//"_Main_Item"
		ItemUpgrade.m_iItemID_Original	=	LDTField.uData.lValue;

		pLDTFile->GetField( iItemID ,  4, LDTField );	//"_Rare_Check"
		ItemUpgrade.m_eRare_Original	=	(ITEM_UPGRADE_RARE)LDTField.uData.lValue;

		

		for( j = 0 ; j < MAX_ITEM_UPGRADE_MATERIAL ; ++j )
		{
			//sprintf( szBuf , "_Material%d" , j + 1 );
			pLDTFile->GetField( iItemID , 5 + (j * 3) , LDTField );
			ItemUpgrade.m_stMaterial[ j ].m_iID		=	LDTField.uData.lValue;

			//sprintf( szBuf , "_Material_Count%d" , j + 1 );
			pLDTFile->GetField( iItemID , 6 + (j * 3) , LDTField );
			ItemUpgrade.m_stMaterial[ j ].m_iCount	=	LDTField.uData.lValue;

			//sprintf( szBuf , "_Material_Rare%d" , j + 1 );
			pLDTFile->GetField( iItemID , 7 + (j * 3) , LDTField );
			ItemUpgrade.m_stMaterial[ j ].m_eRare	=	(ITEM_UPGRADE_RARE)LDTField.uData.lValue;
		}

		for( j = 0 ; j < MAX_ITEM_UPGRADE_REQUIRE ; ++j )
		{
			//sprintf( szBuf , "_Require%d_Type" , j + 1 );
			pLDTFile->GetField( iItemID , 17 + (j * 4), LDTField );
			ItemUpgrade.m_stRequireType[ j ].m_iType	=	LDTField.uData.lValue;

			//sprintf( szBuf , "_Require%d_ID" , j + 1 );
			pLDTFile->GetField( iItemID , 18 + (j * 4) , LDTField );
			ItemUpgrade.m_stRequireType[ j ].m_iID		=	LDTField.uData.lValue;

			//sprintf( szBuf , "_Require%d_Value1" , j + 1 );
			pLDTFile->GetField( iItemID , 19 + (j * 4) , LDTField );
			ItemUpgrade.m_stRequireType[ j ].m_iValue1	=	LDTField.uData.lValue;

			//sprintf( szBuf , "_Require%d_Value2" , j + 1 );
			pLDTFile->GetField( iItemID , 20 + (j * 4) , LDTField );
			ItemUpgrade.m_stRequireType[ j ].m_iValue2	=	LDTField.uData.lValue;
		}

		//_Consumption_OP_Min(33)
		//_Consumption_OP_Max(34)

		pLDTFile->GetField( iItemID ,  35, LDTField );	//"_Probability_Modifier"
		ItemUpgrade.m_iProbabilityModifier	=	LDTField.uData.lValue;

		pLDTFile->GetField( iItemID ,  36, LDTField );	//"_Probability_Min"
		ItemUpgrade.m_iProbabilityMin	=	LDTField.uData.lValue;

		pLDTFile->GetField( iItemID ,  37, LDTField );	//"_Probability_Max"
		ItemUpgrade.m_iProbabilityMax	=	LDTField.uData.lValue;

		pLDTFile->GetField( iItemID ,  38, LDTField );	//"_Type"
		ItemUpgrade.m_eType	=	(ITEM_UPGRADE_TYPE)LDTField.uData.lValue;

		InsertItemUpgrade( ItemUpgrade );
	}

	SAFE_RELEASE(pLDTFile);
	m_eItemUpgradeState	=	SPIU_STATE_NULL;

	return true;
}
//--------------------------------------------------------------------------------------
//»ÃÓ°Éý¼¶
bool SPItemCluster::LoadLDTPhantomUp()
{
	return true;
}
//--------------------------------------------------------------------------------------
bool SPItemCluster::LoadLDTTimeAdd()
{
	SPLDTFile* pLDTFile = NULL;
	g_pResourceManager->GetLDTFromFileName( RES_FNAME_LDT_ITEM_TIMEADD , &pLDTFile);	
	if( pLDTFile == NULL )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "----------------------------------->ITEM ADD TIME LDT FILE LOAD FAIL [%s] \n" , RES_FNAME_LDT_ITEM_TIMEADD );
#endif
		return false;
	}

	int				iItemID , i, j;
	LDT_Field		LDTField;
	//char			szBuf[_MAX_PATH];
	int				iRecordCount	= pLDTFile->GetItemCount();
	SPItemTimeAdd	ItemTimeAdd;

	for( i = 0; i < iRecordCount; ++i )
	{
		iItemID	=	pLDTFile->GetPrimaryKey( i );
		ItemTimeAdd.m_iItemID	=	iItemID;

		pLDTFile->GetField( iItemID , 0, LDTField );		//"_Description" 
		if( LDTField.uData.pValue != NULL )
		{
			if( strlen( LDTField.uData.pValue ) > 0 )
			{
				ItemTimeAdd.m_strDesc	=	LDTField.uData.pValue;
			}
		}		

		pLDTFile->GetField( iItemID ,  1, LDTField );		//"_Result"
		ItemTimeAdd.m_iResult	=	LDTField.uData.lValue;

		pLDTFile->GetField( iItemID ,  2, LDTField );		//"_Material" 
		ItemTimeAdd.m_iMaterialID	=	LDTField.uData.lValue;

		pLDTFile->GetField( iItemID ,  3, LDTField );		//"_Material_Count"
		ItemTimeAdd.m_iMaterialNum	=	LDTField.uData.lValue;

		for( j = 0 ; j < MAX_ITEM_UPGRADE_REQUIRE ; ++j )
		{
			//sprintf( szBuf , "_Require%d_Type" , j + 1 );
			pLDTFile->GetField( iItemID , 4 + (j * 4) , LDTField );
			ItemTimeAdd.m_Require[ j ].m_iType	=	LDTField.uData.lValue;

			//sprintf( szBuf , "_Require%d_ID" , j + 1 );
			pLDTFile->GetField( iItemID , 5 + (j * 4) , LDTField );
			ItemTimeAdd.m_Require[ j ].m_iID		=	LDTField.uData.lValue;

			//sprintf( szBuf , "_Require%d_Value1" , j + 1 );
			pLDTFile->GetField( iItemID , 6 + (j * 4) , LDTField );
			ItemTimeAdd.m_Require[ j ].m_iValue1	=	LDTField.uData.lValue;

			//sprintf( szBuf , "_Require%d_Value2" , j + 1 );
			pLDTFile->GetField( iItemID , 7 + (j * 4) , LDTField );
			ItemTimeAdd.m_Require[ j ].m_iValue2	=	LDTField.uData.lValue;
		}	

		pLDTFile->GetField( iItemID ,  20, LDTField );		//"_Error_Message"
		if( LDTField.uData.pValue != NULL )
		{
			if( strlen( LDTField.uData.pValue ) > 0 )
			{
				ItemTimeAdd.m_strErr	=	LDTField.uData.pValue;
			}
		}

		InsertItemTimeAdd( ItemTimeAdd );
	}

	SAFE_RELEASE(pLDTFile);
	return true;
}

bool SPItemCluster::GetItemInfo(int iItemID, SPItemAttr*& pItemAttr)
{
	hash_map<UINT32, SPItemAttr*>::iterator Iter = m_hmItem.find(iItemID);
	if(Iter != m_hmItem.end() && (iItemID == (*Iter).first) ) {
		pItemAttr = (*Iter).second;
		return true;
	} 
	pItemAttr = NULL;
	return  false;
}


SPTexture* SPItemCluster::GetNumTexture()
{ 	
	return m_pItemNumTexture;
}

SPTexture* SPItemCluster::GetItemTexture(int iItemID, POINT& ptSrcPos)
{
	SPItemAttr* pItemAttr;
	hash_map<UINT32, SPItemAttr*>::iterator hmIterItem = m_hmItem.find(iItemID);
	if(hmIterItem != m_hmItem.end() && (iItemID == (*hmIterItem).first) ) {
		pItemAttr = (*hmIterItem).second;		
		return GetItemTextureFromTextureInx( pItemAttr->m_iIcon, pItemAttr->m_iIconIndex, ptSrcPos);
	} else {
		return NULL;
	}
}

SPTexture* SPItemCluster::GetItemTextureFromTextureInx(int iIconImage, int iIconIndex, POINT& ptSrcPos)
{	
	SPTexture* pTexture;
	hash_map<UINT32, SPTexture*>::iterator hmIterIcon = m_hmIconTexture.find(iIconImage);
	if(hmIterIcon != m_hmIconTexture.end() && (iIconImage == (*hmIterIcon).first) ) {	
		pTexture = 	(*hmIterIcon).second;
		ptSrcPos.x = (iIconIndex-1)%16*32;
		ptSrcPos.y = (iIconIndex-1)/16*32;
		return pTexture;
	} else {
		return NULL;
	}
}

const char*	SPItemCluster::GetItemTextureStr(int iItemID, POINT& ptSrcPos)
{
	SPItemAttr* pItemAttr;
	hash_map<UINT32, SPItemAttr*>::iterator hmIterItem = m_hmItem.find(iItemID);
	if(hmIterItem != m_hmItem.end() && (iItemID == (*hmIterItem).first) ) {
		pItemAttr = (*hmIterItem).second;		
		return GetItemTextureStrFromTextureInx( pItemAttr->m_iIcon, pItemAttr->m_iIconIndex, ptSrcPos);
	} else {
		return NULL;
	}
}

const char* SPItemCluster::GetItemTextureStrFromTextureInx(int iIconImage, int iIconIndex,POINT& ptSrcPos)
{
	ptSrcPos.x = (iIconIndex-1)%16*32;
	ptSrcPos.y = (iIconIndex-1)/16*32;

	return g_pResourceManager->GetGlobalFileName( iIconImage );
}

ICON_MARK SPItemCluster::GetItemIconMark(int iItemID)
{
	SPItemAttr* pItemAttr;
	hash_map<UINT32, SPItemAttr*>::iterator Iter = m_hmItem.find(iItemID);
	if(Iter != m_hmItem.end() && (iItemID == (*Iter).first) ) {
		pItemAttr = (*Iter).second;
		return pItemAttr->m_eIconMark;
	} 
	
	return  ICON_MARK_NULL;
}


bool SPItemCluster::IsItemNameColor(int iItemID)
{
	SPItemAttr* pItemAttr;
	hash_map<UINT32, SPItemAttr*>::iterator Iter = m_hmItem.find(iItemID);
	if(Iter != m_hmItem.end() && (iItemID == (*Iter).first) ) {
		pItemAttr = (*Iter).second;
		return pItemAttr->m_bNameColor;
	} 

	return  false;
}


D3DXCOLOR SPItemCluster::GetItemNameColor(int iItemID)
{
	SPItemAttr* pItemAttr;
	D3DXCOLOR defaultColor(1.0f, 1.0f, 1.0f, 1.0f);
	
	hash_map<UINT32, SPItemAttr*>::iterator Iter = m_hmItem.find(iItemID);
	if(Iter != m_hmItem.end() && (iItemID == (*Iter).first) ) {
		pItemAttr = (*Iter).second;
		if(pItemAttr->m_bNameColor == false) {
			return defaultColor;
		}
		
		float fRed = pItemAttr->m_cRed;
		float fGreen = pItemAttr->m_cGreen;
		float fBlue = pItemAttr->m_cBlue;
		D3DXCOLOR color((fRed / 255.0f), (fGreen / 255.0f), (fBlue / 255.0f), 1.0f);
		return color;
	}

	return defaultColor;
}



//int		SPItemCluster::GetItemInfo(int nItemUID, string& itemName, string& itemDesc)
//{
//	std::vector< SPItemStatus* >::iterator iter = m_vpItem.begin(); 
//	iter = std::find_if( m_vpItem.begin(), m_vpItem.end(), std::bind2nd( IsEqualItem, nItemUID ) );
//	if(iter==m_vpItem.end())
//	{
//		return 0;
//	}
//
//	itemName = (*iter)->GetItemName();
//	itemDesc = (*iter)->GetItemDesc();
//}
//
//int		SPItemCluster::GetStackCount(int nItemUID)
//{
//	ITEM_SEARCH_HEADER()
//
//	return (*iter)->SPSendMessage(MV_ITEM_GETSTACK_CNT);
//}
//
//int		SPItemCluster::GetEquipID(int nItemUID)
//{
//	ITEM_SEARCH_HEADER()
//
//	return (*iter)->SPSendMessage(MV_ITEM_GETEQUIPID);
//}
//
//int 	SPItemCluster::GetPosID(int nItemUID, int &nPosID1, int &nSubPosID1, int &nPosID2, int &nSubPosID2)
//{
//	ITEM_SEARCH_HEADER()
//	
//	nPosID1		= (*iter)->SPSendMessage(MV_ITEM_GETPOSID1);
//	nSubPosID1	= (*iter)->SPSendMessage(MV_ITEM_GETSUBPOSID1);
//	nPosID2		= (*iter)->SPSendMessage(MV_ITEM_GETPOSID2);
//	nSubPosID2	= (*iter)->SPSendMessage(MV_ITEM_GETSUBPOSID2);
//	return nPosID1;
//}
//
//int 	SPItemCluster::GetPosID(int nItemUID, int &nPosID1 )
//{
//	ITEM_SEARCH_HEADER()
//
//	nPosID1		= (*iter)->SPSendMessage(MV_ITEM_GETPOSID1);
//	return nPosID1;
//}
//
//
//int		SPItemCluster::GetType( int nItemUID, int &nType)
//{
//	ITEM_SEARCH_HEADER()
//
//	nType	= (*iter)->SPSendMessage(MV_ITEM_GETTYPE);
//	return nType;
//}
//int		SPItemCluster::GetSubType( int nItemUID, int &nSubType)
//{
//	ITEM_SEARCH_HEADER()
//
//	nSubType= (*iter)->SPSendMessage(MV_ITEM_GETSUBTYPE);
//	return nSubType;
//}

SPItemOption* SPItemCluster::GetItemOption( UINT32 uiItemOptionID )
{
	std::map< UINT32 , SPItemOption >::iterator	mIter	=	m_mItemOption.find( uiItemOptionID );

	if( mIter != m_mItemOption.end() && (*mIter).first == uiItemOptionID )
		return	&( (*mIter).second );

	return NULL;
}

int SPItemCluster::GetSortCashItemSize(int iCategory)
{
	int iCategory1, iCategory2, iCategory3, iSize;

	iCategory1 = ((iCategory & 0x00FF0000) >> 16);
	iCategory2 = ((iCategory & 0x0000FF00) >> 8);
	iCategory3 = (iCategory & 0x000000FF);

	iSize = 0;
	vector< SPCashItemAttr* >::iterator iter = m_vSortCashItem.begin();
	while( iter != m_vSortCashItem.end() )
	{
		if( !(*iter)->bView )
		{
			iter++;
			continue;
		}

		//[xialin 2008/07/25] add Ëæ»úµÀ¾ß >>>
		if ( iCategory1 != CI_SEARCH )
		{
			std::vector<SPCashItemMiss>::iterator  itItem =
				find_if(m_vecRandItem.begin(), m_vecRandItem.end(), 
					FindNode((*iter)->iCashID, SPCashItemMiss, iCashItemID, int));
			if ( itItem !=  m_vecRandItem.end() )
			{
				SPPlayerInvenArchive* pkInvenArchive = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
				if( !pkInvenArchive )
				{
					++iter; 
					continue;
				}

				SPItem* pkItem = pkInvenArchive->GetInvenItem(itItem->iItemID);
				if( !pkItem )
				{
					++iter; 
					continue;
				}

				if ( itItem->iCategory == iCategory1 )
				{
					if( iCategory2 )
					{
						if( itItem->iCategorySub1 == iCategory2 )
						{
							if( iCategory3 )
							{
								if( itItem->iCategorySub2 == iCategory3 )
									iSize++;
							}
							else
								iSize++;
						}
					}
					else
						iSize++;
				}
				++iter; 
				continue;
			}
		}
		//[xialin 2008/07/25] add Ëæ»úµÀ¾ß <<<

		if( iCategory1 == CI_EVENT )
		{
			if( (*iter)->iEventPage )
			{
				if( iCategory2 )
				{
					if( (*iter)->iEventPage == iCategory2 )
						iSize++;
				}
				else
					iSize++;
			}
		}
		else if ( iCategory1 == CI_SEARCH )//[xialin 2008/07/17] add
		{
			std::vector<SPCashItemMiss>::iterator  itItem = 
				find_if(m_vecRandItem.begin(), m_vecRandItem.end(), 
					FindNode((*iter)->iCashID, SPCashItemMiss, iCashItemID, int));
			if ( itItem == m_vecRandItem.end() )
			{
				if (!m_strKeyName.empty())
				{
					USES_CONVERSION;

					WCHAR  wszName[128], wszKeyName[128];
					wcscpy(wszName, A2W((*iter)->strName.c_str()));
					wcscpy(wszKeyName, A2W(m_strKeyName.c_str()));

					if ( wcsstr(wszName, wszKeyName) )
					{
						iSize++;
					}
				}
			}
		}
		else
		{
			if( (*iter)->eCategory == iCategory1 )
			{
				if( iCategory2 )
				{
					if( (*iter)->eCategorySub1 == iCategory2 )
					{
						if( iCategory3 )
						{
							if( (*iter)->eCategorySub2 == iCategory3 )
								iSize++;
						}
						else
							iSize++;
					}
				}
				else
					iSize++;
			}
		}

		++iter;
	}

	return iSize;
}

SPCashItemAttr* SPItemCluster::GetSortCashItem(int iCategory, int iIndex)
{
	int iCategory1, iCategory2, iCategory3, iCount;

	iCategory1 = ((iCategory & 0x00FF0000) >> 16);
	iCategory2 = ((iCategory & 0x0000FF00) >> 8);
	iCategory3 = (iCategory & 0x000000FF);

	iCount = 0;
	vector< SPCashItemAttr* >::iterator iter = m_vSortCashItem.begin();
	while( iter != m_vSortCashItem.end() )
	{
		if( !(*iter)->bView )
		{
			iter++;
			continue;
		}

		//[xialin 2008/07/25] add Ëæ»úµÀ¾ß >>>
		if ( iCategory1 != CI_SEARCH )
		{
			std::vector<SPCashItemMiss>::iterator  itItem = 
				find_if(m_vecRandItem.begin(), m_vecRandItem.end(), 
					FindNode((*iter)->iCashID, SPCashItemMiss, iCashItemID, int));
			if ( itItem !=  m_vecRandItem.end() )
			{
				SPPlayerInvenArchive* pkInvenArchive = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
				if( !pkInvenArchive )
				{
					++iter; 
					continue;
				}

				SPItem* pkItem = pkInvenArchive->GetInvenItem(itItem->iItemID);
				if( !pkItem )
				{
					++iter; 
					continue;
				}

				if ( itItem->iCategory == iCategory1 )
				{
					if( iCategory2 )
					{
						if( itItem->iCategorySub1 == iCategory2 )
						{
							if( iCategory3 )
							{
								if( itItem->iCategorySub2 == iCategory3 )
								{
									if( iCount == iIndex )
										return (*iter);
									++iCount;
								}
							}
							else
							{
								if( iCount == iIndex )
									return (*iter);
								++iCount;
							}
						}
					}
					else
					{
						if( iCount == iIndex )
							return (*iter);
						++iCount;
					}
				}
				++iter; 
				continue;
			}
		}
		//[xialin 2008/07/25] add Ëæ»úµÀ¾ß <<<

		if( iCategory1 == CI_EVENT )
		{
			if( (*iter)->iEventPage )
			{
				if( iCategory2 )
				{
					if( (*iter)->iEventPage == iCategory2 )
					{
						if( iCount == iIndex )
							return (*iter);

						++iCount;
					}
				}
				else
				{
					if( iCount == iIndex )
						return (*iter);

					++iCount;
				}
			}
		}
		else if ( iCategory1 == CI_SEARCH )//[xialin 2008/07/17] add
		{
			std::vector<SPCashItemMiss>::iterator  itItem = 
					find_if(m_vecRandItem.begin(), m_vecRandItem.end(), 
						FindNode((*iter)->iCashID, SPCashItemMiss, iCashItemID, int));
			if ( itItem == m_vecRandItem.end() )
			{
				if (!m_strKeyName.empty())
				{
					USES_CONVERSION;

					WCHAR  wszName[128], wszKeyName[128];
					wcscpy(wszName, A2W((*iter)->strName.c_str()));
					wcscpy(wszKeyName, A2W(m_strKeyName.c_str()));

					if ( wcsstr(wszName, wszKeyName) )
					{
						if ( iCount == iIndex )
							return (*iter);
						iCount++;
					}
				}
			}
		}
		else
		{
			if( (*iter)->eCategory == iCategory1 )
			{
				if( iCategory2 )
				{
					if( (*iter)->eCategorySub1 == iCategory2 )
					{
						if( iCategory3 )
						{
							if( (*iter)->eCategorySub2 == iCategory3 )
							{
								if( iCount == iIndex )
									return (*iter);

								++iCount;
							}
						}
						else
						{
							if( iCount == iIndex )
								return (*iter);

							++iCount;
						}
					}
				}
				else
				{
					if( iCount == iIndex )
						return (*iter);

					++iCount;
				}
			}
		}

		++iter;
	}

	return NULL;
}

SPCashItemAttr* SPItemCluster::GetCashItem(int iID)
{
	vector< SPCashItemAttr >::iterator iter = m_vCashItem.begin();
	while( iter != m_vCashItem.end() )
	{
		if( (*iter).iCashID == iID )
			return &(*iter);

		++iter;
	}

	return NULL;
}

SPCashItemAttr* SPItemCluster::GetMinorCashItem(int iID)
{
	vector< SPCashItemAttr >::iterator iter = m_vMinorCashItem.begin();
	while( iter != m_vMinorCashItem.end() )
	{
		if( (*iter).iCashID == iID )
			return &(*iter);

		++iter;
	}

	return NULL;
}

void SPItemCluster::SortCashItem()
{
	vector< SPCashItemAttr >::iterator iter1, iter2;
	SPCashItemAttr ItemTemp;

	iter1 = m_vCashItem.begin();
	while( iter1 != m_vCashItem.end() )
	{
		iter2 = iter1;
		++iter2;
		while( iter2 != m_vCashItem.end() )
		{
			if( (*iter1).iPositionIndex > (*iter2).iPositionIndex )
			{
				ItemTemp = (*iter1);
				(*iter1) = (*iter2);
				(*iter2) = ItemTemp;
			}

			++iter2;
		}

		++iter1;
	}
}

void SPItemCluster::SortCashItem(int iGender, int iPrice)
{
	vector< SPCashItemAttr >::iterator iterCash;

	m_vSortCashItem.clear();
	if( iGender )
	{
		iterCash = m_vMinorCashItem.begin();
		while( iterCash != m_vMinorCashItem.end() )
		{
			if( (*iterCash).iGender == iGender || (*iterCash).iGender == 0 )
				m_vSortCashItem.push_back(&(*iterCash));

			++iterCash;
		}
	}
	else
	{
		iterCash = m_vMinorCashItem.begin();
		while( iterCash != m_vMinorCashItem.end() )
		{
			m_vSortCashItem.push_back(&(*iterCash));
			++iterCash;
		}
	}

	if( iPrice == 0 )
		return;

	vector< SPCashItemAttr* >::iterator iterSort1, iterSort2;
	SPCashItemAttr* pItemTemp;

	iterSort1 = m_vSortCashItem.begin();
	while( iterSort1 != m_vSortCashItem.end() )
	{
		iterSort2 = iterSort1;
		++iterSort2;
		while( iterSort2 != m_vSortCashItem.end() )
		{
			bool bChange = false;
			if( iPrice == 1 )
			{
				if( (*iterSort1)->iPrice < (*iterSort2)->iPrice )
					bChange = true;
			}
			else
			{
				if( (*iterSort1)->iPrice > (*iterSort2)->iPrice )
					bChange = true;
			}
				
			if( bChange )
			{
				pItemTemp = (*iterSort1);
				(*iterSort1) = (*iterSort2);
				(*iterSort2) = pItemTemp;
			}

			++iterSort2;
		}

		++iterSort1;
	}
}


void SPItemCluster::RandCashItem()
{
	std::vector< SPCashItemAttr* >::iterator  itSortItem;
	int iMaxPos = m_vSortCashItem.size();
	std::vector<SPCashItemMiss>::iterator  itItem = m_vecRandItem.begin();
	for ( ; itItem != m_vecRandItem.end(); itItem++ )
	{
		itItem->iCategory     = GetRandStoreCashCategory(0, 0);
		itItem->iCategorySub1 = GetRandStoreCashCategory(itItem->iCategory, 1);
		itItem->iCategorySub2 = GetRandStoreCashCategory(itItem->iCategorySub1, 2);

		itSortItem = find_if(m_vSortCashItem.begin(), m_vSortCashItem.end(), 
						FindNode(itItem->iCashItemID, SPCashItemAttr, iCashID, int));
		if(itSortItem != m_vSortCashItem.end() )
		{
			time_t	tms;
			time(&tms);
			srand( (unsigned int)tms );
			int iPos = rand()%iMaxPos;

			SPCashItemAttr* pTempCash = (*itSortItem);
			(*itSortItem) = m_vSortCashItem[iPos];
			m_vSortCashItem[iPos] = pTempCash;
		}
	}
}

//------------------------------------------------------------------------------------
int SPItemCluster::GetRandStoreCashCategory(int iCategory, int iIndex)
{
	int iRet = 0;
	std::vector<int>  vecItem;
	std::map<int, vector<int> >   mapRandCategorySub1;
	std::map<int, vector<int> >   mapRandCategorySub2;
	mapRandCategorySub1.clear();
	mapRandCategorySub2.clear();

	//Ò»¼¶²Ëµ¥
	int  iRandCategory[5] = { CI_FASSION, CI_PET, CI_ETC, CI_PACKAGE, CI_EVENT };

	//¶þ¼¶²Ëµ¥
	vecItem.clear();
	vecItem.push_back(CIS1_DRESS);
	vecItem.push_back(CIS1_HELMET);
	vecItem.push_back(CIS1_SHOES);
	vecItem.push_back(CIS1_GLOVE);
	vecItem.push_back(CIS1_ACCESSARIES);
	vecItem.push_back(CIS1_ETC);
	mapRandCategorySub1[CI_FASSION] = vecItem;

	vecItem.clear();
	vecItem.push_back(CIS1_PET);
	vecItem.push_back(CIS1_PET_ETC);
	mapRandCategorySub1[CI_PET] = vecItem;

	vecItem.clear();
	vecItem.push_back(CIS1_FUNCTIONAL);
	vecItem.push_back(CIS1_BEAUTY);
	vecItem.push_back(CIS1_COMMUNITY);
	mapRandCategorySub1[CI_ETC] = vecItem;

	//vecItem.clear();
	//vecItem.push_back(CIS1_PACKAGE_FASSION);
	//vecItem.push_back(CIS1_PACKAGE_PET);
	//vecItem.push_back(CIS1_PACKAGE_HOUSING);
	//vecItem.push_back(CIS1_PACKAGE_PVP);
	//vecItem.push_back(CIS1_PACKAGE_ETC);
	//mapRandCategorySub1[CI_PACKAGE] = vecItem;

	vecItem.clear();
	vecItem.push_back(CIS1_EVENT_NEW);
	vecItem.push_back(CIS1_EVENT_HOT);
	vecItem.push_back(CIS1_EVENT_EVENT);
	mapRandCategorySub1[CI_EVENT] = vecItem;

	//Èý¼¶²Ëµ¥
	vecItem.clear();
	vecItem.push_back(CIS2_ONEPEACE);
	vecItem.push_back(CIS2_BLOUSE);
	vecItem.push_back(CIS2_PANTS);
	vecItem.push_back(CIS2_COAT);
	mapRandCategorySub2[CIS1_DRESS] = vecItem;

	vecItem.clear();
	vecItem.push_back(CIS2_HELMET);
	vecItem.push_back(CIS2_HEADBAND);
	vecItem.push_back(CIS2_HAIRPIN);
	mapRandCategorySub2[CIS1_HELMET] = vecItem;

	vecItem.clear();
	vecItem.push_back(CIS2_SHOES);
	vecItem.push_back(CIS2_LEGBAND);
	mapRandCategorySub2[CIS1_SHOES] = vecItem;

	vecItem.clear();
	vecItem.push_back(CIS2_GLOVE);
	vecItem.push_back(CIS2_ARMBAND);
	mapRandCategorySub2[CIS1_GLOVE] = vecItem;

	vecItem.clear();
	vecItem.push_back(CIS2_GLASS);
	vecItem.push_back(CIS2_EARING);
	vecItem.push_back(CIS2_MAKEUP);
	vecItem.push_back(CIS2_STOCKINGS);
	mapRandCategorySub2[CIS1_ACCESSARIES] = vecItem;

	vecItem.clear();
	vecItem.push_back(CIS2_EMOTICON);
	vecItem.push_back(CIS2_EXPRESS);
	vecItem.push_back(CIS2_EVENT);
	vecItem.push_back(CIS2_CHATTING);
	mapRandCategorySub2[CIS1_COMMUNITY] = vecItem;

	//
	time_t	tms;
	time(&tms);
	srand( (unsigned int)tms );

	if ( iIndex == 0 )
	{
		iRet = iRandCategory[rand()%(sizeof(iRandCategory)/sizeof(int))];
	}
	else if( iIndex == 1 )
	{
		std::map<int, vector<int> >::iterator  it = mapRandCategorySub1.find(iCategory);
		if ( it != mapRandCategorySub1.end() )
		{
			iRet = it->second[rand()%it->second.size()];
		}
	}
	else if( iIndex == 2 )
	{
		std::map<int, vector<int> >::iterator  it = mapRandCategorySub2.find(iCategory);
		if ( it != mapRandCategorySub2.end() )
		{
			iRet = it->second[rand()%it->second.size()];
		}
	}

	return iRet;
}

void SPItemCluster::InitMinorCashItem()
{
	m_vMinorCashItem.clear();
	m_vMinorCashItem = m_vCashItem;
}

void SPItemCluster::UpdateMinorCashItem(CASHSHOP_INFO* pItemInfo)
{
	if( pItemInfo == NULL )
		return;

	SPCashItemAttr* pkItemAttr = GetMinorCashItem(pItemInfo->iPackageID);
	if( pkItemAttr == NULL )
		return;

	SPCashItemAttr tMinorItemAttr = *pkItemAttr;

	tMinorItemAttr.bView = pItemInfo->bEnable;
	tMinorItemAttr.iEventPage = pItemInfo->iEventPage;
	tMinorItemAttr.iEventIcon = pItemInfo->iEventIcon;
	tMinorItemAttr.iPrice = pItemInfo->iCashPrice;
	tMinorItemAttr.iSalePrice = pItemInfo->iSalePrice;
	tMinorItemAttr.iMaxOZ = pItemInfo->iMaxOz;
	tMinorItemAttr.iMaxMile = pItemInfo->iMaxMileage;
	tMinorItemAttr.iPositionIndex = pItemInfo->iPositionIndex;

	//UpdateItemCashPrize(&tMinorItemAttr);

	EraseMinorCashItem(tMinorItemAttr.iCashID);
	InsertMinorCashItem(tMinorItemAttr);
}

void SPItemCluster::InsertMinorCashItem(SPCashItemAttr& tItemAttr)
{
	SPCashItemAttr* pkExistItem = GetMinorCashItem(tItemAttr.iCashID);
	if( pkExistItem != NULL )
		return;

	vector< SPCashItemAttr >::iterator iter = m_vMinorCashItem.begin();
	while( iter != m_vMinorCashItem.end() )
	{
		if( tItemAttr.iPositionIndex <= (*iter).iPositionIndex )
		{
			m_vMinorCashItem.insert(iter, tItemAttr);
			return;
		}

		++iter;
	}

	m_vMinorCashItem.push_back(tItemAttr);
}

void SPItemCluster::EraseMinorCashItem(int iID)
{
	vector< SPCashItemAttr >::iterator iter = m_vMinorCashItem.begin();
	while( iter != m_vMinorCashItem.end() )
	{
		if( (*iter).iCashID == iID )
		{
			m_vMinorCashItem.erase(iter);
			return;
		}

		++iter;
	}
}

////
//void SPItemCluster::UpdateItemCashPrize(SPCashItemAttr* pCashItemAttr)
//{
//	if(pCashItemAttr == NULL || pCashItemAttr->iCashID == 0)
//		return;
//
//	if(pCashItemAttr->eCategory > CI_NULL && pCashItemAttr->eCategory != CI_PACKAGE) {
//		if(pCashItemAttr->eCategory == CI_POINT) {
//			if(pCashItemAttr->ItemInfo[0].iID > 0 && pCashItemAttr->ItemInfo[1].iID > 0)	//ÀÌ °æ¿ì ÆÐÅ°Áö
//				return;	
//		}
//		
//		SPItemAttr* pItemAttr = NULL;
//		if(GetItemInfo(pCashItemAttr->ItemInfo[0].iID, pItemAttr)) {
//			if(pItemAttr) {
//				pItemAttr->m_iCash = pCashItemAttr->iMaxOZ;
//				pItemAttr->m_iPoint = pCashItemAttr->iMaxMile;
//			}
//		}
//	}
//}


void SPItemCluster::UpdateItemCashPrize()
{
	stdext::hash_map<UINT32, SPItemAttr*>::iterator iter = m_hmItem.begin();
	SPItemAttr* pItemAttr = NULL;
	SPCashItemAttr* pCashItemAttr = NULL;
	for(; iter != m_hmItem.end(); ++iter) {
		pItemAttr = NULL;
		pCashItemAttr = NULL;
		
		pItemAttr = (*iter).second;		
		if(pItemAttr->m_iShopID) {
			pCashItemAttr = GetMinorCashItem(pItemAttr->m_iShopID);
			if(pCashItemAttr == NULL || pCashItemAttr->iCashID == 0)
				continue;

			if(pCashItemAttr->eCategory > CI_NULL && pCashItemAttr->eCategory != CI_PACKAGE) {
				if(pCashItemAttr->eCategory == CI_POINT) {
					if(pCashItemAttr->ItemInfo[0].iID > 0 && pCashItemAttr->ItemInfo[1].iID > 0)	//ÀÌ °æ¿ì ÆÐÅ°Áö
						continue;
				}

				pItemAttr->m_iCash = pCashItemAttr->iMaxOZ;
				pItemAttr->m_iPoint = pCashItemAttr->iMaxMile;
			}
		}
	}
}


void SPItemCluster::SetCashVersion(int iMajor, int iMinor)
{
	if( iMajor >= 0 )
		m_iCashMajorVersion = iMajor;
	if( iMinor >= 0 )
		m_iCashMinorVersion = iMinor;
}


void SPItemCluster::GetCashVersion(int& iMajor, int& iMinor)
{
	iMajor = m_iCashMajorVersion;
	iMinor = m_iCashMinorVersion;
}


bool SPItemCluster::InsertItemEnchant(int iKey, SPItemEnchant& ItemEnchant)
{
	map< int, vector< SPItemEnchant > >::iterator iterKey;
	iterKey = m_mItemEnchant.find(iKey);
	
	if( iterKey == m_mItemEnchant.end() )
	{
		vector< SPItemEnchant > vItemList;
		vItemList.push_back(ItemEnchant);

		m_mItemEnchant.insert(make_pair(iKey, vItemList));
		vItemList.clear();
	}
	else
		(*iterKey).second.push_back(ItemEnchant);

	return true;
}

SPItemEnchant* SPItemCluster::GetExactItemEnchant(int iOptionID, int iMaterialID, int iMaterialNum)
{
	map< int, vector< SPItemEnchant > >::iterator iterKey;
	iterKey = m_mItemEnchant.find(iOptionID);

	if( iterKey == m_mItemEnchant.end() )
		return NULL;

	vector< SPItemEnchant >::iterator iterItemEnchant;
	iterItemEnchant = (*iterKey).second.begin();
	while( iterItemEnchant != (*iterKey).second.end() )
	{
		if( (*iterItemEnchant).iMaterialID == iMaterialID &&
			(*iterItemEnchant).iMaterialNum == iMaterialNum )
			return &(*iterItemEnchant);

		++iterItemEnchant;
	}

	return NULL;
}

SPItemEnchant* SPItemCluster::GetItemEnchant(int iOptionID, int iMaterialID, int iMaterialNum)
{
	SPItemEnchant* pItemEnchant = GetExactItemEnchant(iOptionID, iMaterialID, iMaterialNum);
	if( pItemEnchant == NULL )
	{
		map< int, vector< SPItemEnchant > >::iterator iterKey;
		iterKey = m_mItemEnchant.find(iOptionID);

		if( iterKey == m_mItemEnchant.end() )
			return NULL;

		int iNearCount = 0;
		vector< SPItemEnchant >::iterator iterItemEnchant;
		iterItemEnchant = (*iterKey).second.begin();
		while( iterItemEnchant != (*iterKey).second.end() )
		{
			if( (*iterItemEnchant).iMaterialID == iMaterialID )
			{
				if( (*iterItemEnchant).iMaterialNum < iMaterialNum &&
					(*iterItemEnchant).iMaterialNum > iNearCount )
				{
					pItemEnchant = &(*iterItemEnchant);
					iNearCount = (*iterItemEnchant).iMaterialNum;
				}
			}
			++iterItemEnchant;
		}
	}

	return pItemEnchant;
}

bool SPItemCluster::IsItemEnchant(int iOptionID)
{
	map< int, vector< SPItemEnchant > >::iterator iterKey;
	iterKey = m_mItemEnchant.find(iOptionID);

	if( iterKey == m_mItemEnchant.end() )
		return false;

	return true;
}	

bool SPItemCluster::IsItemEnchant(int iOptionID, int iMaterialID)
{
	map< int, vector< SPItemEnchant > >::iterator iterKey;
	iterKey = m_mItemEnchant.find(iOptionID);

	if( iterKey == m_mItemEnchant.end() )
		return false;

	vector< SPItemEnchant >::iterator iterItemEnchant;
	iterItemEnchant = (*iterKey).second.begin();
	while( iterItemEnchant != (*iterKey).second.end() )
	{
		if( (*iterItemEnchant).iMaterialID == iMaterialID )
			return true;

		++iterItemEnchant;
	}

	return false;
}

bool SPItemCluster::InsertItemUpgrade( SPItemUpgrade& ItemUpgrade )
{
	__int64	i64Key = GetItemUpgradeKey( ItemUpgrade.m_iItemID_Original , ItemUpgrade.m_eRare_Original );

	std::map< __int64, SPItemUpgrade >::iterator mIterKey = m_mItemUpgrade.find( i64Key );

	if( mIterKey != m_mItemUpgrade.end() )
	{
#ifdef _DEBUG
		if( ItemUpgrade.m_iItemID_Original > 0 )
			DXUTOutputDebugString( "----------------------------------->ITEM UPGRADE ID Overlapping [%d] \n" , ItemUpgrade.m_iItemID_Original );
#endif
		return false;
	}

	m_mItemUpgrade.insert( std::map< __int64 , SPItemUpgrade >::value_type( i64Key , ItemUpgrade ) );
	return true;
}

bool SPItemCluster::IsItemUpgrade( int iItemID , int iRare )
{
	std::map< __int64, SPItemUpgrade >::iterator mIterKey = m_mItemUpgrade.find( GetItemUpgradeKey( iItemID , iRare ) );

	if( mIterKey == m_mItemUpgrade.end() )
		return false;

	return true;
}

SPItemUpgrade* SPItemCluster::GetItemUpgrade( int iItemID , int iRare )
{
	std::map< __int64, SPItemUpgrade >::iterator mIterKey = m_mItemUpgrade.find( GetItemUpgradeKey( iItemID , iRare ) );

	if( mIterKey == m_mItemUpgrade.end() )
		return NULL;

	return &((*mIterKey).second);
}

__int64 SPItemCluster::GetItemUpgradeKey( int iItemID , int iRare )
{
	return __int64(iItemID)	* __int64(ITEM_UPGRADE_KEY_SECTION) + __int64(iRare);
}

SPITEM_UPGRADE_STATE SPItemCluster::GetItemUpgradeState()
{
	return	m_eItemUpgradeState;
}

void SPItemCluster::SetItemUpgradeState( SPITEM_UPGRADE_STATE eState )
{
	m_eItemUpgradeState	=	eState;
}

bool SPItemCluster::InsertItemTimeAdd(SPItemTimeAdd& ItemTimeAdd)
{	
	if(GetItemTimeAdd(ItemTimeAdd.m_iItemID))
		return false;

	m_mItemTime.insert(std::map<int, SPItemTimeAdd>::value_type(ItemTimeAdd.m_iItemID, ItemTimeAdd));	
	return true;
}


bool SPItemCluster::IsItemTimeAdd(int iItemID)
{
	if(GetItemTimeAdd(iItemID))
		return true;

	return false;
}


SPItemTimeAdd*	SPItemCluster::GetItemTimeAdd(int iItemID)
{
	std::map<int, SPItemTimeAdd>::iterator iter = m_mItemTime.find(iItemID);
	
	if(iter != m_mItemTime.end())
		return &((*iter).second);

	return NULL;
}
//------------------------------------------------------------------------------------
//»ÃÓ°Éý¼¶
bool SPItemCluster::InsertPhantomLevelUp( SPPhantomLevelUp& PhantomLevelUp )
{
	return true;
}

bool SPItemCluster::IsPhantomLevelUp( int iItemID )
{
	return true;
}

SPPhantomLevelUp* SPItemCluster::GetPhantomLevelUp( int iItemID )
{
		SPPhantomLevelUp* plu;
		return plu;
}
//------------------------------------------------------------------------------------

bool SPItemCluster::InsertItemSet(SPItemSet& ItemSet)
{
	if(GetItemSet(ItemSet.m_iSetID))
		return false;

	m_mItemSet.insert(std::map<int, SPItemSet>::value_type(ItemSet.m_iSetID, ItemSet));	
	return true;
}


bool SPItemCluster::IsItemSet(int iSetID)
{
	if(GetItemSet(iSetID))
		return true;

	return false;
}


SPItemSet*	SPItemCluster::GetItemSet(int iSetID)
{
	std::map<int, SPItemSet>::iterator iter = m_mItemSet.find(iSetID);

	if(iter != m_mItemSet.end())
		return &((*iter).second);

	return NULL;
}


//---------------------------------------------------------------------------------------
bool SPItemCluster::RefreshCaseList(SPCashItemAttr* pkItemAttr)
{
	if ( !pkItemAttr )
		return false;

	m_vecCurCaseList.clear();
	std::vector<SPCashItemSultlist>::iterator  itCaseItem = m_vecCaseItem.begin();
	for ( ; itCaseItem != m_vecCaseItem.end(); itCaseItem++ )
	{
		bool bFind = false;
		for (int i=0; i<10; i++ )
		{
			if ( itCaseItem->iCashItem[i] == pkItemAttr->iCashID )
			{
				bFind = true;
				break;
			}
		}
		if ( bFind )
			break;
	}

	if ( itCaseItem == m_vecCaseItem.end() )
		return false;

	for (int i=0; i<10; i++ )
	{
		SPCashItemAttr* pkCashItem = GetMinorCashItem(itCaseItem->iCashItem[i]);
		if ( pkCashItem && pkCashItem->iCashID != pkItemAttr->iCashID )
			m_vecCurCaseList.push_back(pkCashItem);
	}
	return true;
}

SPCashItemAttr* SPItemCluster::GetCaseListItem(int iIndex)
{
	if ( iIndex < 0 || iIndex >= static_cast<int>(m_vecCurCaseList.size()) )
		return NULL;
	return m_vecCurCaseList[iIndex];
}

SPCashEffectItem*  SPItemCluster::GetEffectByID(int iID)
{
	std::vector<SPCashEffectItem>::iterator it =
		find_if(m_vecCaseEffect.begin(), m_vecCaseEffect.end(), FindNode(iID, SPCashEffectItem, iItemID, int));
	if (it != m_vecCaseEffect.end() )
		return &(*it);

	return NULL;
}

SPCashEffectItem*  SPItemCluster::GetEffectByName(const char * pszName)
{
	std::vector<SPCashEffectItem>::iterator it =
		find_if(m_vecCaseEffect.begin(), m_vecCaseEffect.end(), FindNode(string(pszName), SPCashEffectItem, strName, string));
	if (it != m_vecCaseEffect.end() )
		return &(*it);

	return NULL;
}


ITEM_DURATION SPItemCluster::GetItemDuration(int iItemID)
{
	hash_map<UINT32, SPItemAttr*>::iterator iterKey = m_hmItem.find(iItemID);
	if( iterKey == m_hmItem.end() )
		return ITEM_DURATION_NULL;

	return ((*iterKey).second)->m_eDuration;
}

bool SPItemCluster::InsertItemMix( SPItemMix& stItemMixAdd )
{
	std::map< int , std::map< int , SPItemMix > >*	pmmItemMix	=	GetItemMixSkillID( stItemMixAdd.m_iSkillID );

	if( pmmItemMix == NULL )
	{
		std::map< int , SPItemMix >	mItemMix;
		std::map< int , std::map< int , SPItemMix > >	mmItemMix;

		mItemMix.insert( std::make_pair( stItemMixAdd.m_iItemMixID , stItemMixAdd ) );
		mmItemMix.insert( std::make_pair( stItemMixAdd.m_iCategory , mItemMix ) );
		m_mmmItemMix.insert( std::make_pair( stItemMixAdd.m_iSkillID , mmItemMix ) );
	}
	else
	{
		std::map< int , SPItemMix >*	pmItemMix	=	GetItemMixCategory( stItemMixAdd.m_iSkillID , stItemMixAdd.m_iCategory );

		if( pmItemMix == NULL )
		{
			std::map< int , SPItemMix >	mItemMix;

			mItemMix.insert( std::make_pair( stItemMixAdd.m_iItemMixID , stItemMixAdd ) );
			pmmItemMix->insert( std::make_pair( stItemMixAdd.m_iCategory , mItemMix ) );
		}
		else
		{
			pmItemMix->insert( std::make_pair( stItemMixAdd.m_iItemMixID , stItemMixAdd ) );
			return true;
		}
	}

	return false;
}

bool SPItemCluster::IsItemMix(int iSkillID , int iItemMixID )
{
	if( GetItemMixItemMixID( iSkillID , iItemMixID ) == NULL )
		return false;

	return true;
}

std::map< int , std::map< int , SPItemMix > >* SPItemCluster::GetItemMixSkillID( int iSkillID )
{
	std::map< int , std::map< int , std::map< int , SPItemMix > > >::iterator	mmmIter	=	m_mmmItemMix.find( iSkillID );

	if( mmmIter == m_mmmItemMix.end() )
		return NULL;

	return &((*mmmIter).second);
}

std::map< int , SPItemMix >* SPItemCluster::GetItemMixCategory( int iSkillID , int iCategory )
{
	std::map< int , std::map< int , SPItemMix > >*	pmmItemMix	=	GetItemMixSkillID( iSkillID );

	if( pmmItemMix == NULL )
		return NULL;

	std::map< int , std::map< int , SPItemMix > >::iterator	mmIter	=	pmmItemMix->find( iCategory );

	if( mmIter == pmmItemMix->end() )
		return NULL;

	return &((*mmIter).second);
}

SPItemMix* SPItemCluster::GetItemMixItemMixID( int iSkillID , int iItemMixID )
{
	std::map< int , std::map< int , SPItemMix > >*	pmmItemMix	=	GetItemMixSkillID( iSkillID );

	if( pmmItemMix == NULL )
		return NULL;

	std::map< int , std::map< int , SPItemMix > >::iterator	mmIter		=	pmmItemMix->begin();
	std::map< int , std::map< int , SPItemMix > >::iterator	mmIterEnd	=	pmmItemMix->end();

	std::map< int , SPItemMix >::iterator	mIter;
	std::map< int , SPItemMix >*			pmItemMix	=	NULL;

	while( mmIter != mmIterEnd )
	{
		pmItemMix	=	&((*mmIter).second);

		if( pmItemMix != NULL )
		{
			mIter	=	pmItemMix->find( iItemMixID );

			if( mIter != pmItemMix->end() )
			{
				return &((*mIter).second);
			}
		}

		++mmIter;
	}

	return NULL;
}
