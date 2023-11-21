// Copyright (C) DURAGON
//***************************************************************************
// System Name : GUILD Crops
// Comment     : 
// Creation    : DURAGON 2007-5-04    11:18 
//***************************************************************************

#include "SPCommon.h"
//
//#include "SPWindowDEF.h"
//#include "SPWindow.h"
//#include "SPMouseCursor.h"
//#include "SPInterfaceManager.h"

#include <algorithm>

//#include <WinSock2.h>
//#include "Packet.h"
//#include "PacketHandler.h"
//#include "SockWnd.h"
//#include "SPNetworkManager.h"

#include "SPGameObjectDEF.h"
#include "SPGameObject.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPPlayer.h"
#include "SPGOBManager.h"
#include "SPGOBStatus.h"
#include "SPCheckManager.h"
#include "SPLDTFile.h"
#include "SPLDTManager.h"

#include "SPResourceManager.h"
#include "SPEventDEF.h"			// for close
#include "SPEventManager.h"
#include "SPLocalizeManager.h"
#include "SPAbuseFilter.h"

#include "SPUtil.h"
#include "GlobalDefines_Share.h"
#include "SPGuildLDT.h"
#include "SPGuildArchive.h"
#include "SPCropArchive.h"

//#include "SPWindow.h"
//#include "SPWindowStatic.h"
//#include "SPWindowButton.h"
//#include "SPWindowEdit.h"
//#include "SPWindowGuildEmblem.h"
#include "_Error_log.h"


// Singleton ------------------------------------------------------------------------------------
SPGuildLDT* g_pkGuildManager = NULL;
SPGuildLDT* SPGuildLDT::m_pkInstance = NULL;

SPGuildLDT* SPGuildLDT::GetInstance()
{
	if( m_pkInstance == NULL )
	{
		m_pkInstance = new SPGuildLDT;
		g_pkGuildManager = m_pkInstance;
	}

	return m_pkInstance;
}

void SPGuildLDT::Release()
{
	if( m_pkInstance )
	{
		g_pkGuildManager = NULL;
		delete m_pkInstance;
		m_pkInstance = NULL;
	}
}


//------------------------------------------------------------------------------------
SPGuildLDT::SPGuildLDT()
: m_pGuildArchive(NULL)
, m_bEnable(false)
, m_iMaxGuildLevel(0)
{
}

SPGuildLDT::~SPGuildLDT()
{
	DeleteGuildLDT() ;
	DeleteAuthorityLDT() ;

	DeleteCropLDT() ;
	DeleteBoosterLDT() ;
	DeleteEmblemLDT() ;
}


void SPGuildLDT::Init()
{
	m_mpCropInfo.clear() ;
	m_mDisplayCrop.clear() ;
	m_mDisplayCropID.clear() ;
	m_mpBoosterInfo.clear() ;
	m_mpEmblemSym.clear() ;
	m_mpEmblemBG.clear() ;
	m_mpEmblemFX.clear() ;

	LoadGuildLDT();
	LoadAuthorityLDT() ;

	LoadCropLDT() ;
	// CheckCropLDT() ;

	LoadBoosterLDT() ;
	// CheckBoosterLDT() ;

	LoadEmblemLDT() ;
	// CheckEmblemLDT() ;

	m_bEnable = true ;
}


// GUILD.LDT =============================================================================
/**
	Guild LDT파일을 로딩한다
	Guild Create, GuildLevel에서 사용한다.
*/
void SPGuildLDT::LoadGuildLDT()
{
	std::string strLDTFile;
	strLDTFile = "DATA/LDT/GUILD.LDT";

	SPLDTFile* pkLDTFile = NULL;
	if( !g_pResourceManager->GetLDTFromFileName(strLDTFile.c_str(), &pkLDTFile) )
	{
		ErrorFLogs->Error_Log("Can't Find GUILD LDT[%s]", strLDTFile.c_str()) ;
		return;
	}

	int iInsertKey ;
	int iDataNum, iFieldNum, i, j;
	long lItemID;

	LDT_Field Field;
	PGUILD_ATTR	pGuildData = NULL ;

	iDataNum = pkLDTFile->GetItemCount();
	iFieldNum = pkLDTFile->GetFieldCount();
	//ErrorFLogs->Error_Log("Guild LDT Read Data[%d], Field[%d]", iDataNum, iFieldNum) ;

	for( i = 0; i < iDataNum; i++ )
	{
		iInsertKey = 0 ;
		lItemID = pkLDTFile->GetPrimaryKey(i);

		pGuildData = new GUILD_ATTR ;
		pGuildData->Clear() ;
		pGuildData->iItemID = (int)lItemID ;

		Field.Init() ;					// Property
		pkLDTFile->GetField(lItemID, 0, Field);
		if( pkLDTFile->CheckLDTField(Field) )		pGuildData->iPersonality = Field.uData.lValue ;
		Field.Init() ;					// Rank
		pkLDTFile->GetField(lItemID, 1, Field);
		if( pkLDTFile->CheckLDTField(Field) )		pGuildData->iGLevel = Field.uData.lValue ;
		m_iMaxGuildLevel = max(m_iMaxGuildLevel, pGuildData->iGLevel) ;

		iInsertKey = pGuildData->iGLevel * 10 + pGuildData->iPersonality ;

		for( j = 0; j < 5; j++ )		// Create Require	
		{
			Field.Init() ;
			pkLDTFile->GetField(lItemID, j*4+2, Field);
			if( pkLDTFile->CheckLDTField(Field) )		pGuildData->stCreateRequire[j].iRequireType = Field.uData.lValue ;
			else							continue ;
			Field.Init() ;
			pkLDTFile->GetField(lItemID, j*4+3, Field);
			if( pkLDTFile->CheckLDTField(Field) )		pGuildData->stCreateRequire[j].iRequireID = Field.uData.lValue ;
			Field.Init() ;
			pkLDTFile->GetField(lItemID, j*4+4, Field);
			if( pkLDTFile->CheckLDTField(Field) )		pGuildData->stCreateRequire[j].iRequireValue1 = Field.uData.lValue ;
			Field.Init() ;
			pkLDTFile->GetField(lItemID, j*4+5, Field);
			if( pkLDTFile->CheckLDTField(Field) )		pGuildData->stCreateRequire[j].iRequireValue2 = Field.uData.lValue ;
		}
		Field.Init() ;					// Create Money
		pkLDTFile->GetField(lItemID, 22, Field);
		if( pkLDTFile->CheckLDTField(Field) )		pGuildData->lCreateMoney = Field.uData.lValue ;
		Field.Init() ;					// Create Item 
		pkLDTFile->GetField(lItemID, 23, Field);
		if( pkLDTFile->CheckLDTField(Field) )		pGuildData->iCreateItemID = Field.uData.lValue ;
		Field.Init() ;					// Create Item Count
		pkLDTFile->GetField(lItemID, 24, Field);
		if( pkLDTFile->CheckLDTField(Field) )		pGuildData->iCreateItemCount = Field.uData.lValue ;

		//////////////////////////////////////////////////////////////////////////
		//for( j = 0; j < 5; j++ )		// Loan Require	
		//{
		//	Field.Init() ;
		//	pkLDTFile->GetField(lItemID, j*4+25, Field);
		//	if( pkLDTFile->CheckLDTField(Field) )		pGuildData->stLoanRequire[j].iRequireType = Field.uData.lValue ;
		//	else							continue ;
		//	Field.Init() ;
		//	pkLDTFile->GetField(lItemID, j*4+26, Field);
		//	if( pkLDTFile->CheckLDTField(Field) )		pGuildData->stLoanRequire[j].iRequireID = Field.uData.lValue ;
		//	Field.Init() ;
		//	pkLDTFile->GetField(lItemID, j*4+27, Field);
		//	if( pkLDTFile->CheckLDTField(Field) )		pGuildData->stLoanRequire[j].iRequireValue1 = Field.uData.lValue ;
		//	Field.Init() ;
		//	pkLDTFile->GetField(lItemID, j*4+28, Field);
		//	if( pkLDTFile->CheckLDTField(Field) )		pGuildData->stLoanRequire[j].iRequireValue2 = Field.uData.lValue ;
		//}
		//Field.Init() ;					// Loan Money
		//pkLDTFile->GetField(lItemID, 45, Field);
		//if( pkLDTFile->CheckLDTField(Field) )		pGuildData->lLoanMoney = Field.uData.lValue ;
		Field.Init() ;					// Max User 
		pkLDTFile->GetField(lItemID, 46, Field);
		if( pkLDTFile->CheckLDTField(Field) )		pGuildData->iGuildMaxNum = Field.uData.lValue ;
		Field.Init() ;					// Loan Time
		pkLDTFile->GetField(lItemID, 47, Field);
		if( pkLDTFile->CheckLDTField(Field) )		pGuildData->lLoanZoneTime = Field.uData.lValue ;
		Field.Init() ;					// Indun NO
		pkLDTFile->GetField(lItemID, 48, Field);
		if( pkLDTFile->CheckLDTField(Field) )		pGuildData->iDungeonNo = Field.uData.lValue ;

		for( j = 0; j < GUILD_STATUS_COUNT; j++ )		// STATUS Value
		{
			Field.Init() ;
			pkLDTFile->GetField(lItemID, j*2+49, Field);
			if( pkLDTFile->CheckLDTField(Field) )		pGuildData->m_dwStatusType[j] = Field.uData.lValue ;
			Field.Init() ;
			pkLDTFile->GetField(lItemID, j*2+50, Field);
			if( pkLDTFile->CheckLDTField(Field) )		pGuildData->m_iStatusValue[j] = Field.uData.lValue ;
		}
		Field.Init() ;					// GuildPoint Limit
		pkLDTFile->GetField(lItemID, 57, Field);
		if( pkLDTFile->CheckLDTField(Field) )		pGuildData->iPointLimit = Field.uData.lValue ;
		Field.Init() ;					// Guild Lv Name
		pkLDTFile->GetField(lItemID, 58, Field);
		if( pkLDTFile->CheckLDTField(Field) )		pGuildData->m_iLVName = Field.uData.lValue ;

		m_mpGuildAttr.insert( STD_MAP_GUILD_LDT::value_type( iInsertKey, pGuildData )) ;		
		// ErrorFLogs->Error_Log("GUILD_LDT Insert Key[%d] IndunNo[%d]", iInsertKey, pGuildData->iDungeonNo ) ;
	}

	SAFE_RELEASE(pkLDTFile) ;
}

void SPGuildLDT::DeleteGuildLDT()
{
	STD_MAP_GUILD_LDT::iterator	mIter	=	m_mpGuildAttr.begin();
	while ( mIter != m_mpGuildAttr.end() )	{
		SAFE_DELETE( (*mIter).second );
		++mIter;
	}
	m_mpGuildAttr.clear();
}


// GUILD_AUTHORITY.LDT =============================================================================
/**
	길드 권한 LDT 
	길드의 권한은 BitWize 형식으로 LDT 를 읽어서 셋팅한다.
	// 권한(0:작물관리, 1:직위변경, 2:슬로건변경, 3:강퇴, 4:가입권유, 5~9:예약, 10:권한최대수)
*/
void SPGuildLDT::LoadAuthorityLDT()
{
	int i ;

	for( i = 0; i < _MAX_GUILD_GRADE; i++ )
		m_iGuildAuthority[i] = 0 ;

	std::string strLDTFile;
	strLDTFile = "DATA/LDT/GUILD_AUTHORITY.LDT";
	SPLDTFile* pkLDTFile = NULL;
	if( !g_pResourceManager->GetLDTFromFileName(strLDTFile.c_str(), &pkLDTFile) )
	{
		ErrorFLogs->Error_Log("Can't Find GUILD_AUTHORITY LDT[%s]", strLDTFile.c_str()) ;
		return;
	}

	int iDataNum, iFieldNum ;
	long lItemID;
	LDT_Field Field;

	iDataNum = pkLDTFile->GetItemCount();
	iFieldNum = pkLDTFile->GetFieldCount();
	//ErrorFLogs->Error_Log("GuildAuthority LDT Read Data[%d], Field[%d]", iDataNum, iFieldNum) ;

	for( i = 0; i < iDataNum; i++ )
	{
		lItemID = pkLDTFile->GetPrimaryKey(i);

		Field.Init() ;					// Plant
		pkLDTFile->GetField(lItemID, 1, Field);
		if( Field.uData.lValue )	m_iGuildAuthority[GUILD_GRADE_01+i] += GUILD_AUTHO_CROPS ;

		Field.Init() ;					// Grade
		pkLDTFile->GetField(lItemID, 2, Field);
		if( Field.uData.lValue )	m_iGuildAuthority[GUILD_GRADE_01+i] += GUILD_AUTHO_GRADE ;

		Field.Init() ;					// Slogun
		pkLDTFile->GetField(lItemID, 3, Field);
		if( Field.uData.lValue )	m_iGuildAuthority[GUILD_GRADE_01+i] += GUILD_AUTHO_SLOGUN ;

		Field.Init() ;					// Expel
		pkLDTFile->GetField(lItemID, 4, Field);
		if( Field.uData.lValue )	m_iGuildAuthority[GUILD_GRADE_01+i] += GUILD_AUTHO_EXPEL ;

		Field.Init() ;					// Invite
		pkLDTFile->GetField(lItemID, 5, Field);
		if( Field.uData.lValue )	m_iGuildAuthority[GUILD_GRADE_01+i] += GUILD_AUTHO_INVITE ;

		//ErrorFLogs->Error_Log("GuildAuthority LDT Set GRADE[%d] Data[%d]", GUILD_GRADE_01+i, m_iGuildAuthority[GUILD_GRADE_01+i]) ;
	}
	SAFE_RELEASE(pkLDTFile) ;
}

void SPGuildLDT::DeleteAuthorityLDT()
{
	int i ;
	for( i = 0; i < _MAX_GUILD_GRADE; i++ )
		m_iGuildAuthority[i] = 0 ;
}


// CROP.LDT =============================================================================
/**
	CROP정보의 확인을 위한 CROP_INFO (Key : ItemID)
	CROP EVENT시 NPC 출력을 위한 DISPLAY_CROP (Key : EventID)
	두개의 맵을 사용한다.
*/
void SPGuildLDT::LoadCropLDT()
{
	std::string strLDTFile;
	strLDTFile = "DATA/LDT/CROP.LDT";

	SPLDTFile* pkLDTFile = NULL;
	if( !g_pResourceManager->GetLDTFromFileName(strLDTFile.c_str(), &pkLDTFile) )
	{
		ErrorFLogs->Error_Log("Can't Find CROP LDT[%s]", strLDTFile.c_str()) ;
		return;
	}

	int iDataNum, iFieldNum, i, k;
	UINT32 lItemID;
	UINT32 lConsumeItem ;
	UINT32 lEventID ;

	LDT_Field Field;
	PCROP_INFO		pCropData = NULL ;
	DISPLAY_CROP	DispData ;

	iDataNum = pkLDTFile->GetItemCount();
	iFieldNum = pkLDTFile->GetFieldCount();
	//ErrorFLogs->Error_Log("Guild CROP.LDT Read Data[%d], Field[%d]", iDataNum, iFieldNum) ;

	for( i = 0; i < iDataNum; i++ )
	{
		lItemID = pkLDTFile->GetPrimaryKey(i);
		
		pCropData = new CROP_INFO ;
		DispData.Clear() ;
		pCropData->iItemID = lItemID ;

		for( k = 0; k < CROP_REQUIRE_MAX; k++)
		{
			Field.Init() ;					// Require 1
			pkLDTFile->GetField(lItemID, 0 + k*4, Field);
			if( pkLDTFile->CheckLDTField(Field) )		pCropData->stRequire[k].iRequireType = Field.uData.lValue ;
			Field.Init() ;					// Require 1
			pkLDTFile->GetField(lItemID, 1 + k*4, Field);
			if( pkLDTFile->CheckLDTField(Field) )		pCropData->stRequire[k].iRequireID = Field.uData.lValue ;
			Field.Init() ;					// Require 1
			pkLDTFile->GetField(lItemID, 2 + k*4, Field);
			if( pkLDTFile->CheckLDTField(Field) )		pCropData->stRequire[k].iRequireValue1 = Field.uData.lValue ;
			Field.Init() ;					// Require 1
			pkLDTFile->GetField(lItemID, 3 + k*4, Field);
			if( pkLDTFile->CheckLDTField(Field) )		pCropData->stRequire[k].iRequireValue2 = Field.uData.lValue ;
		}

		Field.Init() ;					// Consume ItemID
		pkLDTFile->GetField(lItemID, 12, Field);
		pCropData->iConsumeItem = Field.uData.lValue ;
		lConsumeItem = pCropData->iConsumeItem ;
		Field.Init() ;					// Consume Item Count
		pkLDTFile->GetField(lItemID, 13, Field);

		Field.Init() ;					// EventID1
		pkLDTFile->GetField(lItemID, 14, Field);
		if( pkLDTFile->CheckLDTField(Field) )		lEventID = Field.uData.lValue ;
		Field.Init() ;					// EventID2
		pkLDTFile->GetField(lItemID, 15, Field);

		Field.Init() ;					// LevelTime1
		pkLDTFile->GetField(lItemID, 16, Field);
		if( pkLDTFile->CheckLDTField(Field) )		pCropData->iGrowTime[0] = Field.uData.lValue ;
		Field.Init() ;					// LevelTime2
		pkLDTFile->GetField(lItemID, 17, Field);
		if( pkLDTFile->CheckLDTField(Field) )		pCropData->iGrowTime[1] = Field.uData.lValue ;
		Field.Init() ;					// LevelTime3
		pkLDTFile->GetField(lItemID, 18, Field);
		if( pkLDTFile->CheckLDTField(Field) )		pCropData->iGrowTime[2] = Field.uData.lValue ;
													pCropData->iGrowTime[3] = 0 ;
		Field.Init() ;					// HastenTime
		pkLDTFile->GetField(lItemID, 19, Field);
		if( pkLDTFile->CheckLDTField(Field) )		pCropData->iHastenTime = Field.uData.lValue ;

		DispData.lItemID = lItemID ;
		for( k = 0; k < CROP_LEVEL_MAX; k++)
		{
			Field.Init() ;					// Level ImageID
			pkLDTFile->GetField(lItemID, 20 + k*3, Field);
			if( pkLDTFile->CheckLDTField(Field) )		DispData.iImageID[k] = Field.uData.lValue ;
			Field.Init() ;					// Level Talk
			pkLDTFile->GetField(lItemID, 21 + k*3, Field);
			if( pkLDTFile->CheckLDTField(Field) )		DispData.strTalk[k] = Field.uData.pValue ;
			Field.Init() ;					// Level CutIn ImageID
			pkLDTFile->GetField(lItemID, 22 + k*3, Field);
			if( pkLDTFile->CheckLDTField(Field) )		DispData.iCutInImageID[k] = Field.uData.lValue ;

			pkLDTFile->GetField(lItemID, 32 + k, Field);
			if( pkLDTFile->CheckLDTField(Field) )		DispData.iDlgImageID[k] = Field.uData.lValue ;
		}
		// 32
		m_mpCropInfo.insert( STD_MAP_CROP_INFO::value_type( lConsumeItem, pCropData ) ) ;
		m_mDisplayCropID.insert(STD_MAP_DISPLAY_CROP_ID::value_type( lItemID, lEventID ) ) ;
		m_mDisplayCrop.insert( STD_MAP_DISPLAY_CROP::value_type( lEventID, DispData ) ) ;
		//ErrorFLogs->Error_Log("DialogImageID [%d] [%d, %d, %d, %d]", DispData.lItemID, DispData.iDlgImageID[0], DispData.iDlgImageID[1], DispData.iDlgImageID[2], DispData.iDlgImageID[3] ) ;
	}

	SAFE_RELEASE(pkLDTFile) ;
}

void SPGuildLDT::CheckCropLDT()
{
	DISPLAY_CROP TempDP ;
	CROP_INFO*	pTempCrop ;
	STD_MAP_DISPLAY_CROP::iterator	mDpIter	=	m_mDisplayCrop.begin();
	STD_MAP_DISPLAY_CROP::size_type displaySize = m_mDisplayCrop.size() ;
	STD_MAP_CROP_INFO::iterator	mIter	=	m_mpCropInfo.begin();
	STD_MAP_CROP_INFO::size_type iSize = m_mpCropInfo.size() ;

	if( displaySize != iSize )
	{
		ErrorFLogs->Error_Log("Crop.LDT size is Different[%d]-[%d]!!!", displaySize, iSize) ;
		return ;
	}

	while ( mDpIter != m_mDisplayCrop.end() )
	{
		pTempCrop = (*mIter).second ;
		TempDP = (*mDpIter).second ;

		//ErrorFLogs->Error_Log("Item[%ld], Growth[%d, %d, %d, %d], Hasten[%d], Require[%d, %d, %d, %d][%d, %d, %d, %d][%d, %d, %d, %d]",
		//					pTempCrop->iItemID, pTempCrop->iGrowTime[0], pTempCrop->iGrowTime[1], pTempCrop->iGrowTime[2], pTempCrop->iGrowTime[3], pTempCrop->iHastenTime,
		//					pTempCrop->stRequire[0].iRequireID, pTempCrop->stRequire[0].iRequireType, pTempCrop->stRequire[0].iRequireValue1, pTempCrop->stRequire[0].iRequireValue2,
		//					pTempCrop->stRequire[1].iRequireID, pTempCrop->stRequire[1].iRequireType, pTempCrop->stRequire[1].iRequireValue1, pTempCrop->stRequire[1].iRequireValue2,
		//					pTempCrop->stRequire[2].iRequireID, pTempCrop->stRequire[2].iRequireType, pTempCrop->stRequire[2].iRequireValue1, pTempCrop->stRequire[2].iRequireValue2 ) ;

		//ErrorFLogs->Error_Log("Item[%ld], Image[%d, %d, %s], Image[%d, %d, %s], Image[%d, %d, %s], Image[%d, %d, %s]",
		//					TempDP.lItemID, 
		//					TempDP.iImageID[0], TempDP.iCutInImageID[0], TempDP.strTalk[0].c_str(),
		//					TempDP.iImageID[1], TempDP.iCutInImageID[1], TempDP.strTalk[1].c_str(),
		//					TempDP.iImageID[2], TempDP.iCutInImageID[2], TempDP.strTalk[2].c_str(),
		//					TempDP.iImageID[3], TempDP.iCutInImageID[3], TempDP.strTalk[3].c_str()) ;
		++mIter ;
		++mDpIter;
	}
}

void SPGuildLDT::DeleteCropLDT()
{
	STD_MAP_CROP_INFO::iterator	mIter	=	m_mpCropInfo.begin();
	STD_MAP_CROP_INFO::size_type iSize = m_mpCropInfo.size() ;
	while ( mIter != m_mpCropInfo.end() )
	{
		SAFE_DELETE( (*mIter).second );
		++mIter;
	}
	m_mpCropInfo.clear();

	//DISPLAY_CROP* pTemp ;
	//STD_MAP_DISPLAY_CROP::iterator	mDpIter	=	m_mpDisplayCrop.begin();
	//STD_MAP_DISPLAY_CROP::size_type displaySize = m_mpDisplayCrop.size() ;
	//while ( mDpIter != m_mpDisplayCrop.end() )
	//{
	//	pTemp = (*mDpIter).second ;
	//	pTemp->Clear() ;
	//	SAFE_DELETE( pTemp );
	//	++mDpIter;
	//}
	m_mDisplayCrop.clear();
	m_mDisplayCropID.clear();
}

// BOOSTER.LDT =============================================================================
/**
	ItemID 값으로 비료타입과 적용시간을 알아올수 있도록 
	ItemID를 Key로 사용하는 Map구조를 사용한다.
*/
void SPGuildLDT::LoadBoosterLDT()
{
	std::string strLDTFile;
	strLDTFile = "DATA/LDT/BOOSTER.LDT";

	SPLDTFile* pkLDTFile = NULL;
	if( !g_pResourceManager->GetLDTFromFileName(strLDTFile.c_str(), &pkLDTFile) )
	{
		ErrorFLogs->Error_Log("Can't Find BOOSTER LDT[%s]", strLDTFile.c_str()) ;
		return;
	}

	int iDataNum, iFieldNum, i;
	long lItemID;
	UINT32 iKeyID ;

	LDT_Field Field;
	PBOOSTER_INFO		pBoosterData = NULL ;

	iDataNum = pkLDTFile->GetItemCount();
	iFieldNum = pkLDTFile->GetFieldCount();
	//ErrorFLogs->Error_Log("Guild CROP.LDT Read Data[%d], Field[%d]", iDataNum, iFieldNum) ;

	for( i = 0; i < iDataNum; i++ )
	{
		iKeyID = 0 ;
		lItemID = pkLDTFile->GetPrimaryKey(i);

		pBoosterData = new BOOSTER_INFO ;

		pBoosterData->lItemID = lItemID ;
		Field.Init() ;					// Consume ItemID
		pkLDTFile->GetField(lItemID, 0, Field);
		if( pkLDTFile->CheckLDTField(Field) )		pBoosterData->iConsumeItemID = Field.uData.lValue ;

		Field.Init() ;					// Type
		pkLDTFile->GetField(lItemID, 1, Field);
		if( pkLDTFile->CheckLDTField(Field) )		pBoosterData->iType = Field.uData.lValue ;
		
		Field.Init() ;					// Calc Time
		pkLDTFile->GetField(lItemID, 2, Field);
		if( pkLDTFile->CheckLDTField(Field) )		pBoosterData->iCalcTime = Field.uData.lValue ;

		iKeyID = pBoosterData->iConsumeItemID ;
		m_mpBoosterInfo.insert( STD_MAP_BOOSTER_INFO::value_type( iKeyID, pBoosterData ) ) ;
		//ErrorFLogs->Error_Log("BoosterItem Key[%d] Item[%d], ConsumeItem[%d], Type[%d], Time[%d]", iKeyID, pBoosterData->lItemID, pBoosterData->iConsumeItemID, pBoosterData->iType, pBoosterData->iCalcTime ) ;
	}

	SAFE_RELEASE(pkLDTFile) ;
}


void SPGuildLDT::CheckBoosterLDT()
{
	PBOOSTER_INFO	pTemp ;
	STD_MAP_BOOSTER_INFO::iterator	mIter	=	m_mpBoosterInfo.begin();
	while ( mIter != m_mpBoosterInfo.end() )	{
		pTemp = (*mIter).second ;
		// Add Here Check Condition
		//ErrorFLogs->Error_Log("Booster Item[%ld], consume[%d], type[%d], Time[%d] ", pTemp->lItemID, pTemp->iConsumeItemID, pTemp->iType, pTemp->iCalcTime) ;
		++mIter;
	}
}


void SPGuildLDT::DeleteBoosterLDT()
{
	STD_MAP_BOOSTER_INFO::iterator	mIter	=	m_mpBoosterInfo.begin();
	while ( mIter != m_mpBoosterInfo.end() )
	{
		SAFE_DELETE( (*mIter).second );
		++mIter;
	}
	m_mpBoosterInfo.clear();
}

// GUILD_CI.LDT =============================================================================
void SPGuildLDT::LoadEmblemLDT()
{
	std::string strLDTFile;
	strLDTFile = "DATA/LDT/GUILD_CI.LDT";

	SPLDTFile* pkLDTFile = NULL;
	if( !g_pResourceManager->GetLDTFromFileName(strLDTFile.c_str(), &pkLDTFile) )
	{
		ErrorFLogs->Error_Log("Can't Find Guild_CI LDT[%s]", strLDTFile.c_str()) ;
		return;
	}

	int iDataNum, iFieldNum, i;
	long lItemID;
	UINT32 iKeyID ;
	int iSortID ;

	LDT_Field Field;
	PEMBLEM_INFO		pEmblemData = NULL ;

	iDataNum = pkLDTFile->GetItemCount();
	iFieldNum = pkLDTFile->GetFieldCount();

	for( i = 0; i < iDataNum; i++ )
	{
		//iKeyID = i ;
		lItemID = pkLDTFile->GetPrimaryKey(i);

		pEmblemData = new EMBLEM_INFO ;
		pEmblemData->Clear();

		pEmblemData->iCIItemID = lItemID ;			// Item ID

		Field.Init() ;						
		pkLDTFile->GetField(lItemID, 0, Field);		// CI_Type
		if( pkLDTFile->CheckLDTField(Field) )		pEmblemData->iCIType = Field.uData.lValue ;

		for( int k = 0; k < EMBLEM_REQUIRE_MAX; k++)
		{
			Field.Init() ;
			pkLDTFile->GetField(lItemID, 1 + k*4, Field);	// Require Type
			if( pkLDTFile->CheckLDTField(Field) )		pEmblemData->stRequire[k].iRequireType = Field.uData.lValue ;
			Field.Init() ;
			pkLDTFile->GetField(lItemID, 2 + k*4, Field);	// Require ID
			if( pkLDTFile->CheckLDTField(Field) )		pEmblemData->stRequire[k].iRequireID = Field.uData.lValue ;
			Field.Init() ;
			pkLDTFile->GetField(lItemID, 3 + k*4, Field);	// Require Value1
			if( pkLDTFile->CheckLDTField(Field) )		pEmblemData->stRequire[k].iRequireValue1 = Field.uData.lValue ;
			Field.Init() ;
			pkLDTFile->GetField(lItemID, 4 + k*4, Field);	// Require Value2
			if( pkLDTFile->CheckLDTField(Field) )		pEmblemData->stRequire[k].iRequireValue2 = Field.uData.lValue ;
		}

		Field.Init() ;
		pkLDTFile->GetField(lItemID, 13, Field);	// Consume Ely
		if( pkLDTFile->CheckLDTField(Field) )		pEmblemData->iConsumeEly = Field.uData.lValue ;

		Field.Init() ;
		pkLDTFile->GetField(lItemID, 14, Field);	// Consume ItemID
		if( pkLDTFile->CheckLDTField(Field) )		pEmblemData->iConsumeItem = Field.uData.lValue ;

		Field.Init() ;
		pkLDTFile->GetField(lItemID, 15, Field);	// Consume Item Count
		if( pkLDTFile->CheckLDTField(Field) )		pEmblemData->iConsumeItemCount = Field.uData.lValue ;

		Field.Init() ;
		pkLDTFile->GetField(lItemID, 16, Field);	// Icon Resource ID
		if( pkLDTFile->CheckLDTField(Field) )		pEmblemData->iResIconID = Field.uData.lValue ;

		Field.Init() ;
		pkLDTFile->GetField(lItemID, 17, Field);	// Icon Resource Index
		if( pkLDTFile->CheckLDTField(Field) )		pEmblemData->iResIconIndex = Field.uData.lValue ;

		Field.Init() ;
		pkLDTFile->GetField(lItemID, 18, Field);	// Icon FX ID
		if( pkLDTFile->CheckLDTField(Field) )		pEmblemData->iResFxID = Field.uData.lValue ;

		Field.Init() ;
		pkLDTFile->GetField(lItemID, 19, Field);	// Icon Min Level
		if( pkLDTFile->CheckLDTField(Field) )		pEmblemData->iMinLevel = Field.uData.lValue ;

		Field.Init() ;
		pkLDTFile->GetField(lItemID, 20, Field);	// Icon Max Level
		if( pkLDTFile->CheckLDTField(Field) )		pEmblemData->iMaxLevel = Field.uData.lValue ;

		Field.Init() ;
		pkLDTFile->GetField(lItemID, 21, Field);	// Icon Sort
		if( pkLDTFile->CheckLDTField(Field) )		pEmblemData->iSortIndex = Field.uData.lValue ;
		--pEmblemData->iSortIndex ;
		iSortID = pEmblemData->iSortIndex;

		if( iSortID < 0 )
		{
			ErrorFLogs->Error_Log("## ERROR GUILD_CI.LDT itemID[%d] SortID ERROR!!!", lItemID) ;
			continue ;
		}
		// Insert Map each Type
		// Sort ID is base 1 & IconIndex is base 0
		if( pEmblemData->iCIType == 1 )	{
			iKeyID = pEmblemData->iResIconIndex ;
			m_mpEmblemSym.insert( STD_MAP_EMBLEM_SYMBOL::value_type( iKeyID, pEmblemData ) ) ;
			m_mpSortSym.insert( STD_MAP_EMBLEM_SORT_SYMBOL::value_type( iSortID, pEmblemData ) ) ;
		}
		else if( pEmblemData->iCIType == 2 )	{
			iKeyID = pEmblemData->iResIconIndex ;
			m_mpEmblemBG.insert( STD_MAP_EMBLEM_BG::value_type( iKeyID, pEmblemData ) ) ;
			m_mpSortBG.insert( STD_MAP_EMBLEM_SORT_BG::value_type( iSortID, pEmblemData ) ) ;
		}
		else	{
			iKeyID = pEmblemData->iResIconIndex ;
			m_mpEmblemFX.insert( STD_MAP_EMBLEM_FX::value_type( iKeyID, pEmblemData ) ) ;
			m_mpSortFX.insert( STD_MAP_EMBLEM_SORT_FX::value_type( iSortID, pEmblemData ) ) ;
		}

	}

	SAFE_RELEASE(pkLDTFile) ;
}


void SPGuildLDT::CheckEmblemLDT()
{
	PEMBLEM_INFO	pTemp = NULL ;
	STD_MAP_EMBLEM_SYMBOL::iterator	mIterSym	=	m_mpEmblemSym.begin();
	while ( mIterSym != m_mpEmblemSym.end() )	{
		pTemp = (*mIterSym).second ;
		// Add Here Check Condition
		++mIterSym;
	}

	STD_MAP_EMBLEM_BG::iterator	mIterBG	=	m_mpEmblemBG.begin();
	while ( mIterBG != m_mpEmblemBG.end() )	{
		pTemp = (*mIterBG).second ;
		// Add Here Check Condition
		++mIterBG;
	}

	STD_MAP_EMBLEM_FX::iterator	mIterFX	=	m_mpEmblemFX.begin();
	while ( mIterFX != m_mpEmblemFX.end() )	{
		pTemp = (*mIterFX).second ;
		// Add Here Check Condition
		++mIterFX;
	}
}


void SPGuildLDT::DeleteEmblemLDT()
{
	STD_MAP_EMBLEM_SYMBOL::iterator	mIterSym	=	m_mpEmblemSym.begin();
	while ( mIterSym != m_mpEmblemSym.end() )	{
		SAFE_DELETE( (*mIterSym).second );
		++mIterSym;
	}
	m_mpEmblemSym.clear();
	m_mpSortSym.clear();

	STD_MAP_EMBLEM_BG::iterator	mIterBG	=	m_mpEmblemBG.begin();
	while ( mIterBG != m_mpEmblemBG.end() )	{
		SAFE_DELETE( (*mIterBG).second );
		++mIterBG;
	}
	m_mpEmblemBG.clear();
	m_mpSortBG.clear() ;

	STD_MAP_EMBLEM_FX::iterator	mIterFX	=	m_mpEmblemFX.begin();
	while ( mIterFX != m_mpEmblemFX.end() )	{
		SAFE_DELETE( (*mIterFX).second );
		++mIterFX;
	}
	m_mpEmblemFX.clear();
	m_mpSortFX.clear();
}

//////////////////////////////////////////////////////////////////////////
//
//	Find Functions for GUILD DATA
//
/**
	현재 Crop Level에 맞는 Display_CROP_UNIT을 만들어서 전달한다.
*/
bool SPGuildLDT::GetCropImageByEvent(UINT32 iEventID, int iCLevel, DISPLAY_CROP_UNIT& dpCrop )
{
	STD_MAP_DISPLAY_CROP::iterator	mDpIter	;
	DISPLAY_CROP TempDP ;

	if( iCLevel < 0 || iCLevel >= CROP_LEVEL_MAX )
	{
		return false ;
	}
	mDpIter = m_mDisplayCrop.find( iEventID );
	if( mDpIter != m_mDisplayCrop.end() )
	{
		TempDP = (*mDpIter).second ;
		dpCrop.iImageID			= TempDP.iImageID[iCLevel] ;
		dpCrop.iCutInImageID	= TempDP.iCutInImageID[iCLevel] ;
		dpCrop.iDlgImageID		= TempDP.iDlgImageID[iCLevel] ;
		dpCrop.strTalk			= TempDP.strTalk[iCLevel] ;
		return true ;
	}
	return false ;
}

/**
	현재 Crop Level에 맞는 Display_CROP_UNIT을 만들어서 전달한다.
	1) CropID -> Find EventID
	2) EventID -> Find DISPLAY_CROP_UNIT
*/
bool SPGuildLDT::GetCropImageByCrop(UINT32 iCropID, int iCLevel, DISPLAY_CROP_UNIT& dpCrop )
{
	STD_MAP_DISPLAY_CROP_ID::iterator	mIter;
	UINT32	iFindEventID ;
	STD_MAP_DISPLAY_CROP::iterator	mDpIter ;
	DISPLAY_CROP TempDP ;

	// 1) CropID -> Find EventID
	mIter = m_mDisplayCropID.find(iCropID) ;
	if( mIter == m_mDisplayCropID.end() )
	{
		return false ;
	}
	iFindEventID = (*mIter).second ;

	// 2) EventID -> Find DISPLAY_CROP_UNIT
	if( iCLevel < 0 || iCLevel >= CROP_LEVEL_MAX )
	{
		return false ;
	}
	mDpIter = m_mDisplayCrop.find( iFindEventID );
	if( mDpIter != m_mDisplayCrop.end() )
	{
		TempDP = (*mDpIter).second ;
		dpCrop.iImageID			= TempDP.iImageID[iCLevel] ;
		dpCrop.iCutInImageID	= TempDP.iCutInImageID[iCLevel] ;
		dpCrop.iDlgImageID		= TempDP.iDlgImageID[iCLevel] ;
		dpCrop.strTalk			= TempDP.strTalk[iCLevel] ;
		return true ;
	}
	return false ;
}

/**
	LDT에서 읽은 Guild Grade별 Authority를 반환 (Authority.LDT)
*/
int SPGuildLDT::GetGuildAuthority(int iGrade)
{
	if(iGrade > 0 && iGrade < GUILD_GRADE_MAX)
	{
		return m_iGuildAuthority[iGrade] ;
	}
	return 0 ;
}


/**
	Guild Require 항목을 찾아서 반환 (Guild.LDT)
*/
PGUILD_ATTR SPGuildLDT::FindGuildAttribute(int iKey)
{
	STD_MAP_GUILD_LDT::iterator	mIter	=	m_mpGuildAttr.find( iKey );
	if( mIter == m_mpGuildAttr.end() )		
	{
		ErrorFLogs->Error_Log("##Error::Can't Find Guild Key[%d]!!", iKey) ;
		return NULL ;
	}
	return (*mIter).second ;
}


UINT32	SPGuildLDT::FindGuildCreateItem(int iKey)
{
	PGUILD_ATTR pData = NULL ;
	STD_MAP_GUILD_LDT::iterator	mIter	=	m_mpGuildAttr.find( iKey );
	if( mIter == m_mpGuildAttr.end() )		
	{
		ErrorFLogs->Error_Log("##Error::Can't Find Guild Key[%d]!!", iKey) ;
		return 0 ;
	}
	pData = (*mIter).second ;
	return pData->iCreateItemID ;
}




/**
	묘목 심을때 사용된 아이템을 Crop.LDT에서 찾아서 해당 Crop ItemID를 알려준다.
*/
bool SPGuildLDT::FindCropIDbyConsumeID(UINT32 iConsumeItemID, UINT32& iItemID)
{
	CROP_INFO*	pTempCrop ;
	STD_MAP_CROP_INFO::iterator	mIter	=	m_mpCropInfo.find(iConsumeItemID);

	if( mIter != m_mpCropInfo.end() )
	{
		pTempCrop = (*mIter).second ;
		iItemID = pTempCrop->iItemID ;
		ErrorFLogs->Error_Log("Find CropID[%d] by ConsumeID[%d]", iItemID, iConsumeItemID) ;
		return true ;
	}
	ErrorFLogs->Error_Log("##Can't Find CropID by ConsumeID[%d]", iConsumeItemID) ;
	return false ;
}

/**
*/
bool SPGuildLDT::FindCropInfobyConsumeID(UINT32 iConsumeItemID, CROP_INFO* &pCrop)
{
	STD_MAP_CROP_INFO::iterator	mIter	=	m_mpCropInfo.find(iConsumeItemID);

	if( mIter != m_mpCropInfo.end() )
	{
		pCrop = (*mIter).second ;
		ErrorFLogs->Error_Log("Find CropInfo by ConsumeID[%d]", iConsumeItemID) ;
		return true ;
	}
	ErrorFLogs->Error_Log("##Can't Find CropInfo by ConsumeID[%d]", iConsumeItemID) ;
	return false ;
}


/**
*/
bool SPGuildLDT::FindCropInfobyCropID(UINT32 iCropID, CROP_INFO& pCrop)
{
	CROP_INFO*	pTempCrop ;
	STD_MAP_CROP_INFO::iterator	mIter	=	m_mpCropInfo.begin();

	while( mIter != m_mpCropInfo.end() )
	{
		pTempCrop = (*mIter).second ;
		if( pTempCrop->iItemID == iCropID )
		{
			pCrop = (*pTempCrop) ;
			//ErrorFLogs->Error_Log("Find CropInfo by ItemID[%d]", iCropID) ;
			return true ;
		}
		++mIter ;
	}
	ErrorFLogs->Error_Log("##Can't Find CropInfo by ItemID[%d]", iCropID) ;
	return false ;
}


/**
	비료에 사용된 아이템을 Booster.LDT에서 찾아서 해당 Booster ItemID를 알려준다.
*/
bool SPGuildLDT::FindBoosterIDbyItem(UINT32 iConsumeItemID, UINT32& iItemID)
{
	PBOOSTER_INFO	pTempBooster ;
	STD_MAP_BOOSTER_INFO::iterator	mIter	=	m_mpBoosterInfo.find(iConsumeItemID);

	if( mIter != m_mpBoosterInfo.end() )
	{
		pTempBooster = (*mIter).second ;
		iItemID = pTempBooster->lItemID ;
		ErrorFLogs->Error_Log("Find BoosterID[%d] by ConsumeID[%d]", iItemID, iConsumeItemID) ;
		return true ;
	}
	ErrorFLogs->Error_Log("##Can't Find BoosterID by ConsumeID[%d]", iConsumeItemID) ;
	return false ;
}


/**
	비료에 사용된 아이템을 Booster.LDT에서 찾아서 해당 Booster를 알려준다.
*/
bool SPGuildLDT::FindBoosterInfobyItem(UINT32 iConsumeItemID, BOOSTER_INFO* &pBooster)
{
	STD_MAP_BOOSTER_INFO::iterator	mIter	=	m_mpBoosterInfo.find(iConsumeItemID);

	if( mIter != m_mpBoosterInfo.end() )
	{
		pBooster = (*mIter).second ;
		ErrorFLogs->Error_Log("Find BoosterInfo[%d] by ConsumeID[%d]", pBooster->lItemID, iConsumeItemID) ;
		return true ;
	}
	ErrorFLogs->Error_Log("##Can't Find BoosterInfo by ConsumeID[%d]", iConsumeItemID) ;
	return false ;
}


// Emblem ============================================================
/**
	Emblem Index를 이용해서 해당 Emblem 정보를 찾는다.
*/
bool SPGuildLDT::FindEmblemInfo(int iLayer, int iIndex, EMBLEM_INFO* &emblemInfo)
{
	if( iLayer == LAYER_GUILD_SYM )
		return FindEmblemSymbol(iIndex, emblemInfo) ;
	else if( iLayer == LAYER_GUILD_BG )
		return FindEmblemBG(iIndex, emblemInfo) ;
	else if( iLayer == LAYER_GUILD_FX )
		return FindEmblemFX(iIndex, emblemInfo) ;
	return false ;
}



bool SPGuildLDT::FindEmblemSymbol(int iIndex, EMBLEM_INFO* &emblemInfo)
{
	STD_MAP_EMBLEM_SYMBOL::iterator	mIter	=	m_mpEmblemSym.find(iIndex);
	if( mIter != m_mpEmblemSym.end() )	{
		emblemInfo = (*mIter).second ;
		return true ;
	}
	return false ;
}

bool SPGuildLDT::FindEmblemBG(int iIndex, EMBLEM_INFO* &emblemInfo)
{
	STD_MAP_EMBLEM_BG::iterator	mIter	=	m_mpEmblemBG.find(iIndex);
	if( mIter != m_mpEmblemBG.end() )	{
		emblemInfo = (*mIter).second ;
		return true ;
	}
	return false ;
}

bool SPGuildLDT::FindEmblemFX(int iIndex, EMBLEM_INFO* &emblemInfo)
{
	STD_MAP_EMBLEM_FX::iterator	mIter	=	m_mpEmblemFX.find(iIndex);
	if( mIter != m_mpEmblemFX.end() )	{
		emblemInfo = (*mIter).second ;
		return true ;
	}
	return false ;
}


/**
	정렬된 Emblem List를 찾을때 사용한다
 */
bool SPGuildLDT::FindSortEmblemInfo(int iLayer, int iIndex, EMBLEM_INFO* &emblemInfo)
{
	if( iLayer == LAYER_GUILD_SYM )
		return FindSortSymbol(iIndex, emblemInfo) ;
	else if( iLayer == LAYER_GUILD_BG )
		return FindSortBG(iIndex, emblemInfo) ;
	else if( iLayer == LAYER_GUILD_FX )
		return FindSortFX(iIndex, emblemInfo) ;
	return false ;
}



bool SPGuildLDT::FindSortSymbol(int iViewIndex, EMBLEM_INFO* &emblemInfo)
{
	STD_MAP_EMBLEM_SORT_SYMBOL::iterator	mIter	=	m_mpSortSym.find(iViewIndex);
	if( mIter != m_mpSortSym.end() )	{
		emblemInfo = (*mIter).second ;
		return true ;
	}
	return false ;
}

bool SPGuildLDT::FindSortBG(int iViewIndex, EMBLEM_INFO* &emblemInfo)
{
	STD_MAP_EMBLEM_SORT_BG::iterator	mIter	=	m_mpSortBG.find(iViewIndex);
	if( mIter != m_mpSortBG.end() )	{
		emblemInfo = (*mIter).second ;
		return true ;
	}
	return false ;
}

bool SPGuildLDT::FindSortFX(int iViewIndex, EMBLEM_INFO* &emblemInfo)
{
	STD_MAP_EMBLEM_SORT_FX::iterator	mIter	=	m_mpSortFX.find(iViewIndex);
	if( mIter != m_mpSortFX.end() )	{
		emblemInfo = (*mIter).second ;
		return true ;
	}
	return false ;
}


/**
	각 Emblem Layer 별 Data 크기를 알려준다.
*/
int SPGuildLDT::GetEmblemCount(int iLayer)
{
	if( iLayer == LAYER_GUILD_SYM )
		return (int)m_mpEmblemSym.size() ;
	else if( iLayer == LAYER_GUILD_BG )
		return (int)m_mpEmblemBG.size() ;
	else if( iLayer == LAYER_GUILD_FX )
		return (int)m_mpEmblemFX.size() ;
	return 0 ;
}


/**
	Icon FileName 얻어오기
	PlayerStatusModel 에서 Icon 출력을 위해
*/
const char* SPGuildLDT::GetSymbolFilename()
{
	EMBLEM_INFO* pEmblemInfo = NULL ;
	STD_MAP_EMBLEM_FX::iterator	mIter	=	m_mpEmblemSym.begin() ;
	if( mIter != m_mpEmblemSym.end() )	{
		pEmblemInfo = (*mIter).second ;
		return g_pResourceManager->GetGlobalFileName(pEmblemInfo->iResIconID) ;
	}
	return NULL ;
}

const char* SPGuildLDT::GetEmblemBGFilename()
{
	EMBLEM_INFO* pEmblemInfo = NULL ;
	STD_MAP_EMBLEM_FX::iterator	mIter	=	m_mpEmblemBG.begin();
	if( mIter != m_mpEmblemBG.end() )	{
		pEmblemInfo = (*mIter).second ;
		return g_pResourceManager->GetGlobalFileName(pEmblemInfo->iResIconID) ;
	}
	return NULL ;
}

UINT32 SPGuildLDT::GetEmblemFxID(int iFxIndex)
{
	EMBLEM_INFO* pEmblemInfo = NULL ;
	STD_MAP_EMBLEM_FX::iterator	mIter	=	m_mpEmblemFX.find(iFxIndex);
	if( mIter != m_mpEmblemFX.end() )	{
		pEmblemInfo = (*mIter).second ;
		return pEmblemInfo->iResFxID ;
	}
	return 0 ;
}