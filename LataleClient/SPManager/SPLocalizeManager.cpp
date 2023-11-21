#include "SPCommon.h"
#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"
#include "SPGOBClusterDef.h"

#include "SPLocalizeManager.h"
#include "GlobalDefines_Share.h"

#include "SPDebug.h"
#include "_Error_log.h"

SPLocalizeManager* SPLocalizeManager::m_pkInstance = NULL;
//------------------------------------------------------------------------------------
SPLocalizeManager* SPLocalizeManager::GetInstance()
{
	if( m_pkInstance == NULL )
		m_pkInstance = new SPLocalizeManager;

	return m_pkInstance;
}

//------------------------------------------------------------------------------------
void SPLocalizeManager::Release()
{
	if( m_pkInstance )
	{
		delete m_pkInstance;
		m_pkInstance = NULL;
	}
}

//------------------------------------------------------------------------------------
SPLocalizeManager::SPLocalizeManager() 
: m_eCountry(CT_NULL)
, m_ePublisher(SITE_LOCAL)
, m_bPrologue(false)
, m_bCashShop(false)
, m_bBeautyShop(false)
, m_bPet(false)
, m_iMileageGlowUnit(0)
, m_iCashierGlowUnit(0)
, m_bGuild(false)
, m_bLottery(false)
, m_bIndun(false)
, m_bJoyPad(false)
, m_bChatMacro(false)
, m_bItemMix(false)
, m_bGuideBook(false)
, m_bPVP(false)
, m_bChattingRoom(false)
, m_bReportShot(false)
, m_bItemEnchant(false)
, m_bItemUpgrade(false)
, m_iCashTrade(CASH_TRADE_NULL)
, m_iCashDisplay(CASH_TRADE_NULL)
, m_iCashTradeUnit(0)
, m_iPointTradeUnit(0)
, m_iCashFee(0)
, m_iPointFee(0)
,iAuctionNextBidRate(0)
,m_iAuctionShortTime(0)
,m_iAuctionMiddleTime(0)
,m_iAuctionLongTime(0)
,m_iAuctioinFeeRate(0)
, m_bExtendedTermItem(false)		// ¾ÆÀÌÅÛ±â°£¿¬Àå Disable
, m_bPetDisable(false)				// ¾Ö¿Ïµ¿¹° Disable
, m_bPhantom( false )
, m_bLove(false)
{
}

//------------------------------------------------------------------------------------
SPLocalizeManager::~SPLocalizeManager()
{
}

//------------------------------------------------------------------------------------
bool SPLocalizeManager::Init()
{
	bool bRet;	
	bRet = LoadLDTFile();
	return bRet;
}

//------------------------------------------------------------------------------------
bool SPLocalizeManager::LoadLDTFile()
{
	string strLDTFile;
	strLDTFile = "DATA/LDT/LOCALIZE.LDT";

	SPLDTFile* pkLDTFile = NULL;
	if( !g_pResourceManager->GetLDTFromFileName(strLDTFile.c_str(), &pkLDTFile) )
		return false;

	int iDataNum, iItemID, i;
	LDT_Field Field;

	iDataNum = pkLDTFile->GetItemCount();
	for( i = 0; i < iDataNum; i++ )
	{
		iItemID = pkLDTFile->GetPrimaryKey(i);		
		
		switch( iItemID )
		{
		case 1:
			pkLDTFile->GetFieldFromLable(iItemID, "_Check", Field);
			m_eCountry = (COUNTRY_TYPE)Field.uData.lValue;
			break;
		case 2:
			pkLDTFile->GetFieldFromLable(iItemID, "_Check", Field);
			m_bPrologue = Field.uData.lValue ? false : true;
			break;
		case 3:
			pkLDTFile->GetFieldFromLable(iItemID, "_Check", Field);
			m_bCashShop = Field.uData.lValue ? false : true;
			break;
		case 4:
			pkLDTFile->GetFieldFromLable(iItemID, "_Check", Field);
			m_bBeautyShop = Field.uData.lValue ? false : true;
			break;
		case 5:
			pkLDTFile->GetFieldFromLable(iItemID, "_Check", Field);
			m_bPet = Field.uData.lValue ? false : true;
			break;
		case 6:
			pkLDTFile->GetFieldFromLable(iItemID, "_Check", Field);
			m_bWayPoint = Field.uData.lValue ? false : true;
			break;

		case 7:
			pkLDTFile->GetFieldFromLable(iItemID, "_Des", Field);
			m_strHomePage = (char*) Field.uData.lValue;
			break;

		case 8:
			pkLDTFile->GetFieldFromLable(iItemID, "_Des", Field);
			m_strNewAccount = (char*) Field.uData.lValue;
			break;

		case 9:
			pkLDTFile->GetFieldFromLable(iItemID, "_Des", Field);
			m_strBillingPage = (char*) Field.uData.lValue;
			break;

		case 10:
			pkLDTFile->GetFieldFromLable(iItemID, "_Check", Field);
			m_ePublisher = (SITE_TYPE)Field.uData.lValue;
			break;

		case 11:
			pkLDTFile->GetFieldFromLable(iItemID, "_Check", Field);
			m_iMileageRatio = Field.uData.lValue;
			break;

		case 12:
			pkLDTFile->GetFieldFromLable(iItemID, "_Des", Field);
			m_strFindUserID = (char*) Field.uData.lValue;
			break;

		case 13:
			pkLDTFile->GetFieldFromLable(iItemID, "_Des", Field);
			m_strFindPasswd = (char*) Field.uData.lValue;
			break;

		case 14:
			pkLDTFile->GetFieldFromLable(iItemID, "_Check", Field);
			m_iMileageGlowUnit = Field.uData.lValue;
			break;
		
		case 15:
			pkLDTFile->GetFieldFromLable(iItemID, "_Check", Field);
			m_bCashShopCoupon = Field.uData.lValue ? false : true;
			break;

		case 16:
			pkLDTFile->GetFieldFromLable(iItemID, "_Check", Field);
			m_bCashShopGift = Field.uData.lValue ? false : true;
			break;

		case 17:
			pkLDTFile->GetFieldFromLable(iItemID, "_Check", Field);
			m_iCashierGlowUnit = Field.uData.lValue;
			break;

		case 18:		// Guild 0 is Enable, 1 is Disable
			pkLDTFile->GetFieldFromLable(iItemID, "_Check", Field);
			m_bGuild = Field.uData.lValue ? false : true ;
			break ;

		case 19:		// lottery 0 is Enable, 1 is Disable
			pkLDTFile->GetFieldFromLable(iItemID, "_Check", Field);
			m_bLottery = Field.uData.lValue ? false : true ;
			break ;

		case 20:		// Instance Dungeon
			pkLDTFile->GetFieldFromLable(iItemID, "_Check", Field);
			m_bIndun = Field.uData.lValue ? false : true ;
			break ;

		case 21:		// JoyPad
			pkLDTFile->GetFieldFromLable(iItemID, "_Check", Field);
			m_bJoyPad = Field.uData.lValue ? false : true ;
			break ;

		case 22:		// ChatMacro
			pkLDTFile->GetFieldFromLable(iItemID, "_Check", Field);
			m_bChatMacro = Field.uData.lValue ? false : true ;
			break ;

		case 23:		// ItemMix
			pkLDTFile->GetFieldFromLable(iItemID, "_Check", Field);
			m_bItemMix = Field.uData.lValue ? false : true ;
			break ;

		case 24:		// GuideBook
			pkLDTFile->GetFieldFromLable(iItemID, "_Check", Field);
			m_bGuideBook = Field.uData.lValue ? false : true ;
			break ;

		case 25:		// PVP
			pkLDTFile->GetFieldFromLable(iItemID, "_Check", Field);
			m_bPVP = Field.uData.lValue ? false : true ;
			break ;

		case 27:		// °æ¸Å »óÈ¸ÀÔÂû ±Ý¾×Áõ°¡ ºñÀ²
			pkLDTFile->GetFieldFromLable(iItemID, "_Check", Field);
			iAuctionNextBidRate = Field.uData.lValue;
			break ;

		case 28:		// °æ¸Å ½Ã°£(´Ü±â)
			pkLDTFile->GetFieldFromLable(iItemID, "_Check", Field);
			m_iAuctionShortTime = Field.uData.lValue;
			break ;

		case 29:		// °æ¸Å ½Ã°£(Áß±â)
			pkLDTFile->GetFieldFromLable(iItemID, "_Check", Field);
			m_iAuctionMiddleTime = Field.uData.lValue;
			break ;

		case 30:		// °æ¸Å ½Ã°£(ÃÖÀå±â)
			pkLDTFile->GetFieldFromLable(iItemID, "_Check", Field);
			m_iAuctionLongTime = Field.uData.lValue;
			break ;

		case 31:		// °æ¸Å ¼ö¼ö·á ºñÀ²
			pkLDTFile->GetFieldFromLable(iItemID, "_Check", Field);
			m_iAuctioinFeeRate = Field.uData.lValue;
			break ;
		
		case 32:		// Cash Trade Type
			pkLDTFile->GetFieldFromLable(iItemID, "_Check", Field);
			m_iCashTrade = (CASH_TRADE)Field.uData.lValue;
			break ;

		case 33:		// Cash Trade Display
			pkLDTFile->GetFieldFromLable(iItemID, "_Check", Field);
			m_iCashDisplay = (CASH_TRADE)Field.uData.lValue;
			break ;

		case 34:		// Cash Trade unit
			pkLDTFile->GetFieldFromLable(iItemID, "_Check", Field);
			m_iCashTradeUnit = Field.uData.lValue;
			break ;
		case 35:		// Point Trade unit
			pkLDTFile->GetFieldFromLable(iItemID, "_Check", Field);
			m_iPointTradeUnit = Field.uData.lValue;
			break ;
		case 36:		// Cash Fee
			pkLDTFile->GetFieldFromLable(iItemID, "_Check", Field);
			m_iCashFee = Field.uData.lValue;
			break ;
		case 37:		// Point Fee
			pkLDTFile->GetFieldFromLable(iItemID, "_Check", Field);
			m_iPointFee = Field.uData.lValue;
			break ;
		case 38:		// Chatting Room
			pkLDTFile->GetFieldFromLable(iItemID, "_Check", Field);
			m_bChattingRoom = Field.uData.lValue ? false : true ;
			break ;

		case 39:		// Report shot			
			pkLDTFile->GetFieldFromLable(iItemID, "_Check", Field);
			m_bReportShot = Field.uData.lValue ? false : true ;
			break;

		case 40:		// Item Enchant
			pkLDTFile->GetFieldFromLable(iItemID, "_Check", Field);
			m_bItemEnchant = Field.uData.lValue ? false : true ;
			break;

		case 41:		// Item Upgrade			
			pkLDTFile->GetFieldFromLable(iItemID, "_Check", Field);
			m_bItemUpgrade = Field.uData.lValue ? false : true ;
			break;

		case 42:		// Item ±â°£¿¬Àå
			pkLDTFile->GetFieldFromLable(iItemID, "_Check", Field);
			m_bExtendedTermItem = Field.uData.lValue ? false : true ;
			break;

		case 43:		// ¾Ö¿Ïµ¿¹° Disable
			pkLDTFile->GetFieldFromLable(iItemID, "_Check", Field);
			m_bPetDisable = Field.uData.lValue ? false : true ;
					   //»ÃÓ°Éý¼¶
		case 44:
			pkLDTFile->GetFieldFromLable(iItemID, "_Check", Field);
			m_bPhantom = Field.uData.lValue ? false : true ;
			break;

		case 45:		// ¿¬¾Ö ±â´É
			pkLDTFile->GetFieldFromLable(iItemID, "_Check", Field);
			m_bLove = Field.uData.lValue ? false : true ;
			break;
		}
	}

	SAFE_RELEASE(pkLDTFile);
#ifdef _DEBUG
//	m_ePublisher = SITE_BUDDYBUDDY ;
//	m_ePublisher = SITE_ACTOZ ;
#endif

	// ·¹Áö½ºÆ®¸® ÀúÀåÇÒ °ª »êÃâ [1/26/2007 AJJIYA]
	m_strRegistryCompany	=	"Actoz Soft";

	if( m_ePublisher == SITE_BUDDYBUDDY )
		m_strRegistryCompany	=	"BuddyGame";
	else if( m_ePublisher == SITE_ATGAMES )
		m_strRegistryCompany	=	"AtGames";

	ErrorFLogs->Error_Log("==========>SET COUNTRY [%d], SITE [%d] \n" , m_eCountry, m_ePublisher);
	return true;
}

//------------------------------------------------------------------------------------
bool SPLocalizeManager::IsEnableValue(LOCALIZE_VALUE eValue)
{
	bool bRet = false;
	switch( eValue )
	{
	case LV_PROLOGUE:
		bRet = m_bPrologue;
		break;
	case LV_CASHSHOP:
		bRet = m_bCashShop;
		break;
	case LV_BEAUTYSHOP:
		bRet = m_bBeautyShop;
		break;
	case LV_PET:
		bRet = m_bPet;
		break;
	case LV_WAYPOINT:
		bRet = m_bWayPoint;
		break;
	case LV_COUPON:
		bRet = m_bCashShopCoupon;
		break;
	case LV_GIFT:
		bRet = m_bCashShopGift;
		break;
	case LV_GUILD:
		bRet = m_bGuild ;
		break ;
	case LV_LOTTERY:
		bRet = m_bLottery ;
		break ;
	case LV_INDUN:
		bRet = m_bIndun ;
		break ;
	case LV_JOYPAD:
		bRet = m_bJoyPad ;
		break ;
	case LV_CHATMACRO:
		bRet = m_bChatMacro;
		break;
	case LV_ITEMMIX:
		bRet = m_bItemMix;
		break;
	case LV_GUIDEBOOK:
		bRet = m_bGuideBook;
		break;
	case LV_PVP:
		bRet = m_bPVP;
		break;
	case LV_CHATTINGROOM:
		bRet = m_bChattingRoom;
		break;
	case LV_REPORTSHOT:
		bRet = m_bReportShot;
		break;
	case LV_ITEMENCHANT:
		bRet = m_bItemEnchant;
		break;
	case LV_ITEMUPGRADE:
		bRet = m_bItemUpgrade;
		break;
	case LV_EXTENDEDTERMITEM:
		bRet = m_bExtendedTermItem;
		break;
	case LV_PETDISABLE:
		bRet = m_bPetDisable;
		break;
	case LV_PHANTOM:		//»ÃÓ°
		bRet = m_bPhantom;
		break;
	case LV_LOVE:
		bRet = m_bLove;
		break;
	case LV_LOVE_COLOR:
		bRet = true;
		break;
	}

	return bRet;
}

//------------------------------------------------------------------------------------
// SITE_BUDDYBUDDY & SITE_ATGAMES is Only Web Login
bool SPLocalizeManager::OnlyWebLogin()
{
	return ( m_ePublisher == SITE_BUDDYBUDDY || m_ePublisher == SITE_ATGAMES ) ? true : false ;
}


COUNTRY_TYPE SPLocalizeManager::GetCountryType()
{
	return m_eCountry;
}

SITE_TYPE	SPLocalizeManager::GetSiteType()
{
	return m_ePublisher ;
}

const char* SPLocalizeManager::GetHomePage()
{
	return m_strHomePage.c_str();
}

const char* SPLocalizeManager::GetNewAccount()
{
	return m_strNewAccount.c_str();
}

const char* SPLocalizeManager::GetBillingPage()
{
	return m_strBillingPage.c_str();
}

int SPLocalizeManager::GetMileageRatio()
{
	return m_iMileageRatio;
}

int SPLocalizeManager::GetMileageGlowUnit()
{
	return m_iMileageGlowUnit;
}

int SPLocalizeManager::GetCashierGlowUnit()
{
	return m_iCashierGlowUnit;
}

const char* SPLocalizeManager::GetHomeFindUserID()
{
	return m_strFindUserID.c_str();
}

const char* SPLocalizeManager::GetHomeFindPasswd()
{
	return m_strFindPasswd.c_str();
}

const char* SPLocalizeManager::GetRegistryCompany()
{
	return m_strRegistryCompany.c_str();
}


CASH_TRADE	SPLocalizeManager::GetCashTradeType()
{
	return m_iCashTrade;
}


CASH_TRADE	SPLocalizeManager::GetCashTradeDisplay()
{
	return m_iCashDisplay;
}

//////////////////////////////////////////////////////////////////////////
// °æ¸Å °ü·Ã
int SPLocalizeManager::GetAuctionNextBidRate()
{
	return iAuctionNextBidRate;
}

int SPLocalizeManager::GetAuctionTimeShort()
{
	return m_iAuctionShortTime;
}

int SPLocalizeManager::GetAuctionTimeMiddle()
{
	return m_iAuctionMiddleTime;
}

int SPLocalizeManager::GetAuctionTimeLong()
{
	return m_iAuctionLongTime;
}

int SPLocalizeManager::GetAuctionFeeRate()
{
	return m_iAuctioinFeeRate;
}

void SPLocalizeManager::SetCashTradeType(int iType)
{
	if(iType == CASH_TRADE_ALL) {
		m_iCashDisplay		= static_cast<CASH_TRADE>(iType);
		m_iCashTrade		= static_cast<CASH_TRADE>(iType);
	}
	else if(iType == CASH_TRADE_CASH) {
		m_iCashDisplay		= static_cast<CASH_TRADE>(iType);
		m_iCashTrade		= static_cast<CASH_TRADE>(iType);
	}
	else if(iType == CASH_TRADE_POINT) {
		m_iCashDisplay		= static_cast<CASH_TRADE>(iType);
		m_iCashTrade		= static_cast<CASH_TRADE>(iType);
	}
	else {
		m_iCashDisplay		= CASH_TRADE_NULL;
		m_iCashTrade		= CASH_TRADE_NULL;
	}
}


int	SPLocalizeManager::GetCashTradeUnit()
{
	if(m_iCashTrade == CASH_TRADE_CASH || m_iCashTrade == CASH_TRADE_ALL)
		return m_iCashTradeUnit;

	return 0;
}


int	SPLocalizeManager::GetPointTradeUnit()
{
	if(m_iCashTrade == CASH_TRADE_POINT || m_iCashTrade == CASH_TRADE_ALL)
		return m_iPointTradeUnit;

	return 0;
}


int	SPLocalizeManager::GetCashFee()
{
	if(m_iCashTrade == CASH_TRADE_CASH || m_iCashTrade == CASH_TRADE_ALL)
		return m_iCashFee;

	return 0;
}


int	SPLocalizeManager::GetPointFee()
{
	if(m_iCashTrade == CASH_TRADE_POINT || m_iCashTrade == CASH_TRADE_ALL)
		return m_iPointFee;

	return 0;
}

