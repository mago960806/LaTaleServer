// Copyright (C) DURAGON
//***************************************************************************
// System Name : GUILD BASE
// Comment     : 
// Creation    : DURAGON 2007-4-04    11:18 
//***************************************************************************

#include "SPCommon.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowSlider.h"
#include "SPWindowButton.h"
#include "SPWindowEdit.h"
//#include "SPWindowList.h"
#include "SPWindowFriendUser.h"
#include "SPWindowChat.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include <algorithm>

#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"
#include "SockWnd.h"
#include "SPNetworkManager.h"

#include "SPGameObjectDEF.h"
#include "SPGameObject.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPPlayer.h"
#include "SPPlayerInvenArchive.h"
#include "SPGOBManager.h"
#include "SPGOBStatus.h"
#include "SPCheckManager.h"
#include "SPLDTFile.h"
#include "SPLDTManager.h"

#include "SPItem.h"
#include "SPItemAttr.h"
#include "SPItemStatus.h"

#include "SPResourceManager.h"
#include "SPEventDEF.h"			// for close
#include "SPEventManager.h"
#include "SPEventNpcHandler.h"
#include "SPLocalizeManager.h"
#include "SPPvpManager.h"

#include "SPUtil.h"
#include "SPDebug.h"
#include "GlobalDefines_Share.h"
#include "SPGuildLDT.h"
#include "SPGuildArchive.h"
#include "SPCropArchive.h"
//#include "SPWindowGuild.h"
#include "_Error_log.h"


// SPGuildArchive*	SPGuildArchive::m_pInstance=0;




SPGuildArchive::SPGuildArchive()
: m_bBasicInfo(false)
//, m_bListInfo(false)
//, m_bShowOffline(true)
//, m_iCurPage(0)
, m_bChgSlogun(false)
, m_bChgGrade(false)
, m_bInvite(false)
, m_bExpel(false)
, m_bChgPlant(false)
, m_pCropArchive(NULL)
{
}

SPGuildArchive::~SPGuildArchive()
{
	DeleteBasicInfo() ;
	DeleteCommonInfo() ;
	//DeleteMemberList() ;

	SAFE_DELETE(m_pCropArchive) ;
	// SAFE_DELETE(m_pGuildLDT) ;
}

//
//void SPGuildArchive::Release()
//{
//	if( m_pInstance )
//	{
//		delete m_pInstance;
//		m_pInstance = NULL;
//	}
//}
//
//SPGuildArchive*	SPGuildArchive::GetInstance()
//{	
//	if(m_pInstance==0)
//		m_pInstance = new SPGuildArchive;
//	return (SPGuildArchive*)m_pInstance;
//}


void SPGuildArchive::Init()
{
	if( SPLocalizeManager::GetInstance()->IsEnableValue(LV_GUILD) )	{
		m_pCropArchive		= new SPGuildCrops ;
		m_pCropArchive->Init() ;
	}
}


void SPGuildArchive::Process(float fTime) 
{
	int iUpdate = 0 ;
	if( SPLocalizeManager::GetInstance()->IsEnableValue(LV_GUILD) )
	{
		if( GetCropArchive()->CropProcess(fTime, iUpdate) )
		{
			if( iUpdate == CROP_UPDATE_LEVEL )	{
				UpdateCropLevelImage(true) ;
			}
			g_pInterfaceManager->SPChildWndSendMessage(WIID_GUILD_INFO, SPIM_GUILD_CROP_UPDATE, CROP_UPDATE_TIME, 0);
			g_pInterfaceManager->SPChildWndSendMessage(WIID_CROPS, SPIM_GUILD_CROP_UPDATE, CROP_UPDATE_TIME, 0);
		}
	}
}



void SPGuildArchive::DeleteBasicInfo()
{
	m_bBasicInfo		= false ;	// 정보설정 여부
	m_iGuildLevel		= 0 ;		// 길드 레벨

	m_iGuildID			= 0 ;		// 길드 ID
	m_iGuildEmblem[0]	= 0 ;		// 앰블럼 ID
	m_iGuildEmblem[1]	= 0 ;		// 앰블럼 ID
	m_iGuildProperty	= 0 ;		// 길드 성향
	m_iGuildGrade		= 0 ;		// 길드내 직책 (1:길마)
	m_iGuildTotalPoint	= 0 ;		// 길드 총 경험치
	m_iNextLevelPoint	= 1 ;
	m_strGuildName.clear() ;		// 길드 이름
}

void SPGuildArchive::DeleteCommonInfo()
{
	m_strGuildSlogun.clear() ;		// 길드 슬로건
	m_strSendSlogun.clear() ;		// 길마가 Send하는 슬로건
}



/**
	길드 탈되, 강퇴되는 경우 길드 정보 제거
	1) 길드 정보창이 떠 있는 경우 Hide
	2) Player 길드 정보 제거
	3) 길드 Archive 정보 제거
*/
void SPGuildArchive::RemoveGuildInfo()
{
	if( g_pInterfaceManager->GetGuildInfoWindow() == NULL )		return ;

	if( g_pInterfaceManager->GetGuildInfoWindow()->IsShow() )	
		g_pInterfaceManager->GetGuildInfoWindow()->Hide() ;

	if( g_pInterfaceManager->IsCutIn() )		
		g_pEventManager->SetCutInState(CUT_IN_STATE_RELEASE);		

	DeleteBasicInfo() ;
	DeleteCommonInfo() ;
	//DeleteMemberList() ;
	SetCropInformation(CROP_SET_CLEAR) ;		// Destroy
}




/**
	정보를 받아서 데이터가 셋팅되면 Window에 알려주어야 한다.
*/
void SPGuildArchive::SetEnableBasicInfo(bool bEnable)
{
	m_bBasicInfo = bEnable ;
	g_pInterfaceManager->SPChildWndSendMessage(WIID_GUILD_INFO, SPIM_GUILD_INFO_ENABLE, 0, 0, true);
}




/**
	길드에 가입되어 있는지 여부 확인
*/
bool SPGuildArchive::GetBasicAvailable()
{
	return m_bBasicInfo ;
}

/**
	작물이 있는지 여부 확인
*/
bool SPGuildArchive::hasCorps()
{
	return m_pCropArchive->hasCorps() ;
}


/**
	@return 길드정보가 있으면 Property
	길드정보가 없으면 -1
*/
int SPGuildArchive::GetGuildProperty()
{
	return ( GetBasicAvailable() ) ? m_iGuildProperty : 0 ;
}
/**
	길드내 작위 받아오기
	@return 0 : 길드원 아님
	@return 1 ~ 6 : 길드 내 작위
*/
int SPGuildArchive::GetGuildGrade()
{
	return ( GetBasicAvailable() ) ? m_iGuildGrade : 0 ;
}
/**
	자신이 속한 길드의 레벨
	@return 0 : 길드원 아님
	@return 1 ~ 5 : 길드 레벨
*/
int SPGuildArchive::GetGuildLevel()
{
	return ( GetBasicAvailable() ) ? m_iGuildLevel : 0 ;
}
/**
	길드의 현재 총 경험치
*/
int SPGuildArchive::GetGuildTotalPoint()
{
	return ( GetBasicAvailable() ) ? m_iGuildTotalPoint : 0 ;
}
int SPGuildArchive::GetGuildPointProtege()
{
	if( !GetBasicAvailable() )	return 0 ;
	if( m_iNextLevelPoint <= 0 )	m_iNextLevelPoint = 1 ;
	float fTotalProtege = ( m_iGuildTotalPoint > 0 ) ? ((float)m_iGuildTotalPoint * 100 / m_iNextLevelPoint) : 0 ;
	int iTotalProtege = (int)fTotalProtege ;
	if( iTotalProtege > 100 )	
		return 100 ;
	if( iTotalProtege < 0 )
		return 0 ;

	return iTotalProtege ;
}
/**
	길드 총 경험치 셋팅 -> WindowGuild에서 Member 총합을 구해서 셋팅
*/
void SPGuildArchive::SetGuildTotalPoint(int iGPoint)
{
	if( iGPoint > -1 )	m_iGuildTotalPoint = iGPoint ;
}
void SPGuildArchive::UpdateGuildTotalPoint(int iGPoint)
{
	if( (m_iGuildTotalPoint + iGPoint) > 0 )	
		m_iGuildTotalPoint += iGPoint ;
}
/**
	길드 슬로건 가져오기
	@return NULL : 길드원 아님
	@return const char* : 슬로건
*/
const char* SPGuildArchive::GetGuildSlogun()
{
	return ( GetBasicAvailable() ) ? m_strGuildSlogun.c_str() : NULL ;
}
/**
	길드명 가져오기
	@return NULL : 길드원 아님
	@return const char* : 길드명
*/
const char* SPGuildArchive::GetGuildName()
{
	return ( GetBasicAvailable() ) ? m_strGuildName.c_str() : NULL ;
}
/**
	길드 엠블럼 Number 가져오기
	@param int iNo : 첫번째 or 두번째
	@return int : 엠블럼 순서 번호
*/
int SPGuildArchive::GetGuildEmblem(int iNo)
{
	return ( iNo >= 0 && iNo < 3 ) ? m_iGuildEmblem[iNo] : 0 ;
}
/**
	길드방의 인던번호 가져오기
	@param int iGLevel : 길드 레벨
	@param int iGType : 길드 타입
	@return 0 : 길드원 아님
	@return int : 길드방 인던 번호
*/
int	SPGuildArchive::GetDungeonNo(int iGLevel, int iGType)
{
	if( iGLevel < 1 || iGType < GUILD_PROPERTY_NORMAL )		
	{
		ErrorFLogs->Error_Log("Error::Guild Level[%d], Type [%d] is Invalid!!", iGLevel, iGType) ;
		return 0 ;
	}
	
	int iKey = iGLevel * 10 + iGType ;
	PGUILD_ATTR pGuildData ;
	pGuildData = g_pkGuildManager->FindGuildAttribute(iKey) ;

	if( pGuildData )
		return pGuildData->iDungeonNo ;
	else
		return 0 ;
}
/**
	내가 가입한 길드방의 인던 번호 가져오기
	@return 0 : 길드원 아님
	@return int : 길드방 인던 번호
*/
int SPGuildArchive::GetMyGuildDungeon()
{
	return ( GetBasicAvailable() ) ? GetDungeonNo(m_iGuildLevel, m_iGuildProperty) : 0 ;
}

/**
	EventID를 통해서 해당 레벨의 작물 이미지를 얻는다.
*/
bool SPGuildArchive::GetCropImageByEvent(UINT32 iEventID, int iCLevel, DISPLAY_CROP_UNIT& dpCrop )
{
	if( iCLevel < 0 || iCLevel >= CROP_LEVEL_MAX )
		iCLevel = m_iGuildLevel ;
	return g_pkGuildManager->GetCropImageByEvent(iEventID, iCLevel, dpCrop) ;
}


/**
	Crop Level에 따른 Image Update
*/
int	SPGuildArchive::UpdateCropLevelImage(bool bShowText)
{
	int iCropLevel = m_pCropArchive->GetCropLevel() ;
	if( iCropLevel < 0 || iCropLevel >= CROP_LEVEL_MAX )	{
		ErrorFLogs->Error_Log("##UpdateCropLevelImage Fail : Level Invalid[%d]", iCropLevel) ;
		return 0 ;
	}

	// Background Crop Update
	UpdateCropNPC(iCropLevel) ;

	// Window Crop Update
	UpdateWindowCropNPC(iCropLevel) ;
	
	// Cut-in Image & Text Update
	if(g_pEventManager->GetCutInState() == CUT_IN_STATE_GUILD_CROPS) {
		UpdateCutInNPC(iCropLevel) ;
	}

	

	return 1;
}


/**
	MapEvent에서 CropImageID에 해당하는 NPC를 UPdate 한다
*/
void SPGuildArchive::UpdateCropNPC( int iCropLevel )
{
	CLASS_ID ClassID ;
	INSTANCE_ID InstanceID ;

	m_pCropArchive->GetCropClassID(InstanceID, ClassID) ;
	if( InstanceID == 0 || ClassID == 0 ) {
		ErrorFLogs->Error_Log("##Crop InstanceID[%d], ClassID[%d] Invalid", InstanceID, ClassID) ;
		return ;
	}
	UINT32 iUpdateImageID = m_pCropArchive->GetCropImageID(iCropLevel) ;
	if( iUpdateImageID != 0 )
		g_pGOBManager->UpdateEventNPC(InstanceID, ClassID, iUpdateImageID) ;
}
/**
	Cut-in이미지를 해당 레벨로 업데이트 한다.
*/
void SPGuildArchive::UpdateCutInNPC( int iCropLevel )
{
	UINT32 iCutInImageID = m_pCropArchive->GetCropCutinImageID(iCropLevel) ;
	if( iCutInImageID != 0 )
		g_pEventManager->GetEventHandler()->SetNpcTalkImage(iCutInImageID);
	SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_SHOPTALK);
	pWindow->Show();
	pWindow->SPSendMessage(SPIM_SET_SHOPTALK_MSG, (WPARAM)m_pCropArchive->GetCropCutinTalk(iCropLevel), 0);
}
/**
	Window에 CropLevelUp을 알려서 이미지를 변경하도록 한다.
*/
void SPGuildArchive::UpdateWindowCropNPC( int iCropLevel )
{
	g_pInterfaceManager->SPChildWndSendMessage(WIID_GUILD_INFO, SPIM_GUILD_CROP_UPDATE, CROP_UPDATE_LEVEL, iCropLevel);
	g_pInterfaceManager->SPChildWndSendMessage(WIID_CROPS, SPIM_GUILD_CROP_UPDATE, CROP_UPDATE_LEVEL, iCropLevel);
}



/**
	GuildLevelUp 이후 정보 업데이트
*/
void SPGuildArchive::UpdateGuildLevel(int iLevel)
{
	m_iGuildLevel = iLevel ;

	int iKeyNext = 0 ;
	// 최고레벨이면 현재 레벨을 Limit로 설정한다.
	if( m_iGuildLevel == g_pkGuildManager->GetLimitGuildLevel() )
		iKeyNext = m_iGuildLevel * 10 + m_iGuildProperty ;
	else
		iKeyNext = (m_iGuildLevel+1) * 10 + m_iGuildProperty ;

	PGUILD_ATTR pGuildData ;
	pGuildData = g_pkGuildManager->FindGuildAttribute(iKeyNext) ;
	if( pGuildData == NULL )
		return ;

	m_iNextLevelPoint = pGuildData->iPointLimit ;
}




/**
	길드 기능별 권한이 있는지 여부 
*/
bool SPGuildArchive::GetInvitable()
{
	return m_bInvite ;		// 초대 가능
}
//bool SPGuildArchive::GetMasterChangable()
//{
//	return m_bChgMaster ;	// 권한 위임 가능
//}
bool SPGuildArchive::GetSlogunChangable()
{
	return m_bChgSlogun ;	// 슬로건 변경 가능
}
bool SPGuildArchive::GetRankChangable()
{
	return m_bChgGrade ;	// 직위 변경 가능
}
bool SPGuildArchive::GetExpelable()
{
	return m_bExpel ;	// 강퇴 가능
}
bool SPGuildArchive::GetCorpsChangable()
{
	return m_bChgPlant ;	// 작물 수정 가능 여부
}
int	SPGuildArchive::GetNourishTime()
{
	return m_pCropArchive->GetNourishTime() ;	// 작물의 남은 성장시간
}

SPGuildCrops* SPGuildArchive::GetCropArchive()
{
	return m_pCropArchive ;
}


/**
	길드에서 수정가능한 권한을 설정한다.
*/
void SPGuildArchive::SetGuildAuthority(int iGrade)
{
	// Grade별로 AuthorityLDT 값에서 권한을 읽어서 설정한다.
	if( iGrade < GUILD_GRADE_01 || iGrade >= _MAX_GUILD_GRADE )
		return ;
	m_iGuildGrade = iGrade ;
	int iAuthority = g_pkGuildManager->GetGuildAuthority(iGrade) ;
	
//	m_bChgMaster	= (iAuthority & GUILD_AUTHO_CHGMASTER )	? true : false ;
	m_bChgPlant		= (iAuthority & GUILD_AUTHO_CROPS )		? true : false ;
	m_bChgGrade		= (iAuthority & GUILD_AUTHO_GRADE )		? true : false ;
	m_bChgSlogun	= (iAuthority & GUILD_AUTHO_SLOGUN )	? true : false ;
	m_bExpel		= (iAuthority & GUILD_AUTHO_EXPEL )		? true : false ;
	m_bInvite		= (iAuthority & GUILD_AUTHO_INVITE )	? true : false ;

	ErrorFLogs->Error_Log("Set GuildAuthority Grade[%d], Plant[%d], Grade[%d], Slogun[%d], Expel[%d], Invite[%d]",
							iGrade, m_bChgPlant, m_bChgGrade, m_bChgSlogun, m_bExpel, m_bInvite) ;
}







/**
	내가 속한 길드의 정보를 설정한다.
*/
bool SPGuildArchive::SetBasicInfo(long iGID, int iGEmblem1, int iGEmblem2, int iGrade, int iProperty, int iGLevel, const char* strGName, int iGPoint/*= 0*/)
{
	m_iGuildID			= iGID ;				// 길드 ID
	m_iGuildEmblem[0]	= iGEmblem1 ;			// 앰블럼 ID
	m_iGuildEmblem[1]	= iGEmblem2 ;			// 앰블럼 ID
	m_iGuildGrade		= iGrade ;				// 길드내 직책
	m_iGuildProperty	= iProperty ;			// 길드 성향
	m_strGuildName		= strGName ;			// 길드 이름
	m_iGuildPoint		= iGPoint ;				// 길드 포인트
	UpdateGuildLevel(iGLevel) ;					// 길드 레벨

	SetGuildAuthority(m_iGuildGrade) ;
	ErrorFLogs->Error_Log("SetBasicInfo: GID[%ld] EmblemID[%d, %d] Property[%d] Grade[%d] GLevel[%d] GName[%s] GPoint[%d]", m_iGuildID, m_iGuildEmblem[0], m_iGuildEmblem[1], m_iGuildProperty, m_iGuildGrade, m_iGuildLevel, m_strGuildName.c_str(), iGPoint) ;
	return ( iGID ) ? true : false ;
}

/**
	내가 속한 길드의 정보를 설정한다.
*/
bool SPGuildArchive::SetBasicInfo(long iGID, int iGEmblem1, int iGEmblem2, int iGEmblem3, int iGrade, int iProperty, int iGLevel, const char* strGName, int iGPoint/*= 0*/)
{
	m_iGuildID			= iGID ;				// 길드 ID
	m_iGuildEmblem[0]	= iGEmblem1 ;			// 앰블럼 ICON
	m_iGuildEmblem[1]	= iGEmblem2 ;			// 앰블럼 BG
	m_iGuildEmblem[2]	= iGEmblem3 ;			// 앰블럼 FX
	m_iGuildGrade		= iGrade ;				// 길드내 직책
	m_iGuildProperty	= iProperty ;			// 길드 성향
	m_strGuildName		= strGName ;			// 길드 이름
	m_iGuildPoint		= iGPoint ;				// 길드 포인트
	UpdateGuildLevel(iGLevel) ;					// 길드 레벨

	SetGuildAuthority(m_iGuildGrade) ;
	ErrorFLogs->Error_Log("SetBasicInfo: GID[%ld] EmblemID[%d, %d] Property[%d] Grade[%d] GLevel[%d] GName[%s] GPoint[%d]", m_iGuildID, m_iGuildEmblem[0], m_iGuildEmblem[1], m_iGuildProperty, m_iGuildGrade, m_iGuildLevel, m_strGuildName.c_str(), iGPoint) ;
	return ( iGID ) ? true : false ;
}

void SPGuildArchive::SetProperty(const char* strGSlogun)
{
	m_strGuildSlogun	= strGSlogun ;		// 길드 슬로건
	ErrorFLogs->Error_Log("SetProperty: Slogun[%s]", m_strGuildSlogun.c_str()) ;
}


/**
	Crop의 시간을 설정한다.
	@param int iCmd : 0(초기화), 1(셋팅), 2(시간 업데이트)
*/
void SPGuildArchive::SetCropInformation(int iCmd, int iCropID/*=0*/, int iRemainTime/*=0*/, int iNourishTime/*=0*/, int iHastenSumTime/*=0*/)
{
	m_pCropArchive->SetCropInformation(iCmd, iCropID, iRemainTime, iNourishTime, iHastenSumTime) ;
	if( iCmd == CROP_SET_DATA || iCmd == CROP_SET_INITIALIZE )	{
		UpdateCropLevelImage(false) ;
	}
	else if ( iCmd == CROP_SET_NOURISH )
	{
		if( m_pCropArchive->CheckCropLevelup() == true ) {
			UpdateCropLevelImage(true) ;
		}
	}
}

/**
	묘목 성장 완료
*/
void SPGuildArchive::SetCropGrowthComplete()
{
	m_pCropArchive->SetGrowthComplete() ;
}


/**
	Guild Master인지 여부를 확인한다.
	@param const char strName = NULL : default 자신
	@return bool : Master 여부
*/
bool SPGuildArchive::IsMaster(const char* strUserName /* = NULL*/)
{
	if( GetBasicAvailable() == false )	
		return false ;

	return ( m_iGuildGrade == GUILD_GRADE_01 ) ? true : false ;
}







/**
	길드 제한 체크 ( 길드 레벨 체크 )
*/
bool SPGuildArchive::CheckGuildLevel(int iLevel, int iLevel2)
{
	if( iLevel2 <= 0 )
		return ( m_iGuildLevel >= iLevel ) ? true : false ;
	else
		return ( m_iGuildLevel >= iLevel && m_iGuildLevel <= iLevel2 ) ? true : false ;
}
/**
	길드 제한 체크 ( 길드 타입 체크 )
	길드 시스템에 의해서 변경되어지는 값 참조
	(1:중립, 2:질서, 3: 혼돈, 4: 중립질서, 5:중립혼돈, 6:질서혼돈, 7:중립질서혼돈)

*/
bool SPGuildArchive::CheckGuildType(int iType)
{
	bool bRet = false ;
	switch(iType)
	{
	case 1:			// 중립
	case 2:			// 질서
	case 3:			// 혼돈
		return ( m_iGuildProperty == iType ) ? true : false ;
		break ;
	case 4:			// 중립질서
		return ( m_iGuildProperty == GUILD_PROPERTY_NORMAL || m_iGuildProperty == GUILD_PROPERTY_ANGEL ) ? true : false ;
		break ;
	case 5:			// 중립혼돈
		return ( m_iGuildProperty == GUILD_PROPERTY_NORMAL || m_iGuildProperty == GUILD_PROPERTY_DAEMON ) ? true : false ;
		break ;
	case 6:			// 질서혼돈
		return ( m_iGuildProperty == GUILD_PROPERTY_DAEMON || m_iGuildProperty == GUILD_PROPERTY_ANGEL ) ? true : false ;
		break ;
	case 7:			// 중립질서혼돈
		return ( m_iGuildProperty != GUILD_PROPERTY_NULL ) ? true : false ;
		break ;
	}
	return false ;
}


/**
	길드 생성관련 체크
	LDT의 정보를 통해 조건에 맞는지 체크한다.
	1) 길드 가입여부 체크
	2) 아이템 보유 체크
	3) 게임 머니 체크
	@return	GUILD_CREATE_OK				: 성공
	@return ERROR_GUILD_CREATE_STAT		: 제한 1 (스텟)
	@return ERROR_GUILD_CREATE_MONEY	: 제한 2 (머니)
	@return ERROR_GUILD_CREATE_ITEM		: 제한 3 (아이템)
	@return ERROR_GUILD_CREATE_REQ4		: 제한 4
	@return ERROR_GUILD_CREATE_REQ5		: 제한 5
	@return ERROR_GUILD_CREATE_ISHAVE	: 이미 길드 가입
	@return ERROR_GUILD_CREATE_GNAME	: 길드명 오류
	@return ERROR_GUILD_CREATE_LDT		: LDT Error
*/
int SPGuildArchive::CheckGuildCreate(int iType)
{
	if( GetBasicAvailable() )	return ERROR_GUILD_CREATE_ISHAVE ;

	PGUILD_ATTR pGuildData ;
	int iKey = 10 + iType ;
	pGuildData = g_pkGuildManager->FindGuildAttribute(iKey) ;
	if( pGuildData == NULL )	return ERROR_GUILD_CREATE_LDT ;

	for( int i = 0; i < 5; i++)
	{
		if( false == g_pCheckManager->CheckCondition(pGuildData->stCreateRequire[i].iRequireType
													, pGuildData->stCreateRequire[i].iRequireID
													, pGuildData->stCreateRequire[i].iRequireValue1
													, pGuildData->stCreateRequire[i].iRequireValue2) )
		{
			return i + 1 ;
		}
	}

	return GUILD_CREATE_OK ;
}


/**
	길드 레벨업 체크
	생성 Require 체크와 동일 항목 체크 ( Master Check 는 GuildLevelUp Show에서 )
	-> Create_ItemID 를 소모하게됨 : 있으면 해당 아이템의 위치를 찾아서 TRASH_ITEM 에 채워서 보내줌
	@return ERROR_GUILD_CREATE_STAT		: 제한 1 (NULL)
	@return ERROR_GUILD_CREATE_MONEY	: 제한 2 (GuildLevel)
	@return ERROR_GUILD_CREATE_ITEM		: 제한 3 (아이템)
	@return ERROR_GUILD_CREATE_REQ4		: 제한 4 (Money)
	@return ERROR_GUILD_CREATE_REQ5		: 제한 5
	@return ERROR_GUILD_POINT			: 길드 포인트 오류
*/
int SPGuildArchive::CheckGuildLevelup(int& iRetValue)
{
	PGUILD_ATTR pGuildData ;
	int iKey = (m_iGuildLevel+1) * 10 + m_iGuildProperty ;		// Next Level Check
	pGuildData = g_pkGuildManager->FindGuildAttribute(iKey) ;
	if( pGuildData == NULL )	
		return ERROR_GUILD_CREATE_LDT ;

	for( int i = 0; i < 5; i++)
	{
		if( false == g_pCheckManager->CheckCondition(pGuildData->stCreateRequire[i].iRequireType
			, pGuildData->stCreateRequire[i].iRequireID
			, pGuildData->stCreateRequire[i].iRequireValue1
			, pGuildData->stCreateRequire[i].iRequireValue2) )
		{
			iRetValue = pGuildData->stCreateRequire[i].iRequireValue1;
			return i + 1 ;
		}
	}

	if( SendGuildLevelup(m_iGuildLevel+1, pGuildData->iCreateItemID, pGuildData->iCreateItemCount) == false )
		return ERROR_PKTSEND_FAIL ;

	return GUILD_CREATE_OK ;
}



/**
	길드 파괴 가능 체크
	@return 0 : 성공
	@return 1 : 길드원 아님
	@return 2 : 마스터 아님
*/
int SPGuildArchive::CheckGuildDestroy()
{
	if( !GetBasicAvailable() )			return 1 ;
	if( !IsMaster()	)					return 2 ;
	return 0 ;
}





//////////////////////////////////////////////////////////////////////////
//
//	Packet
//
//////////////////////////////////////////////////////////////////////////
bool SPGuildArchive::SendGuildCreate(int iType, int iEmblem1, int iEmblem2, const char* strGName)
{
	UINT8 byData ;
	UINT8 arrGMark[2] ;
	CPacket packet;
	
	byData		= (UINT8) iType ;
	arrGMark[0] = (UINT8) iEmblem1 ;
	arrGMark[1] = (UINT8) iEmblem2 ;

	packet.AddUINT32( GUILD_CS_CREATE );	
	packet.AddData( strGName, GUILD_MAX_NAME+1);
	packet.AddUINT8( byData ) ;
	packet.AddData( arrGMark, sizeof(BYTE) * 2);

	byData = 1 ;		
	packet.AddUINT8( byData ) ;			// 길드 랭크
	SENDPACKET(packet) ;

	return true ;
}

/**
	길드 파괴를 위한 패킷을 서버에 보낸다.
	1) 길드원이 마스터 이외에 다른사람이 있으면 실패 -> 클라이언트에 리스트 없으면 확인불가
*/
bool SPGuildArchive::SendGuildDestroy()
{
	CPacket packet;
	packet.AddUINT32( GUILD_CS_DESTROY );	

	SENDPACKET(packet) ;
	return true ;
}


/**
	길드 초대 패킷 보내기
*/
bool SPGuildArchive::SendGuildInvite(const char* szCharName)
{
	if( strlen(szCharName) < MIN_LEN_NAME )		
		return false ;

	// If PVP Zone then Block
	if( g_pkPvpManager->IsPvpZone() )
	{
		char	szMsg[256];
		if( g_pResourceManager->GetGlobalString(51000021) )
			wsprintf(szMsg, g_pResourceManager->GetGlobalString(51000021));
		else
			wsprintf(szMsg, "[Guild]You can't use it in PVP zone!!");
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szMsg);
		return false ;
	}

	// [2007.09.06]for China Request : 2Byte Guild can't invite other user
	if( SPLocalizeManager::GetInstance()->GetCountryType() == CT_CHINA )
	{
		if( m_strGuildName.length() < MIN_LEN_GUILDNAME )
		{
			if( g_pResourceManager->GetGlobalString(27000107) )
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(27000107) );
			return false ;
		}
	}
	if( GetBasicAvailable() == false)	
	{
		if( g_pResourceManager->GetGlobalString(27000006) )
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(27000006) );
		return false ;
	}
	if( GetInvitable() == false )
	{
		if( g_pResourceManager->GetGlobalString(27000006) )
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(27000006) );
		return false ;
	}
	
	CPacket packet;
	packet.AddUINT32( GUILD_CS_REQUEST );	
	packet.AddData( szCharName, LEN_NAME+1);

	SENDPACKET(packet) ;
	return true ;
}

/**
	길드 탈퇴 ( LocalPlayer에서 메시지 창을 통해 확인후 패킷 전송)
*/
bool SPGuildArchive::SendGuildLeave()
{
	if( GetBasicAvailable() == false)	
	{
		if( g_pResourceManager->GetGlobalString(27000006) )
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(27000006) );
		return false ;
	}

	g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_GUILD_SECEDE_ME, 0);
	return true ;
}

// GUILD_CS_RANKUP,
// UINT8			byNewGRank;							// New길드랭크
// TRASH_ITEM		stConsumeItem;						// 소모되는아이템
bool SPGuildArchive::SendGuildLevelup(int iNewLevel, int iItemID, int iItemCount)
{
	CPacket		packet;
	TRASH_ITEM	stConsumeItem;

	SPPlayerInvenArchive* pkInvenArchive = g_pGOBManager->GetLocalPlayer()->GetInvenArchive() ;
	if(pkInvenArchive == NULL)
		return false ;

	if( iItemID > 0 )	{
		SPItem* pSearchItem = pkInvenArchive->GetSPItem(INVENTORY_EVENT, iItemID ) ;
		if( pSearchItem == NULL || pSearchItem->GetSlotIndex() < 0 ) {
			return false ;
		}
		stConsumeItem.iContainerSlot.ContainerType = pSearchItem->GetContainerType() ;
		stConsumeItem.iContainerSlot.SlotIndex = pSearchItem->GetSlotIndex() + 1 ;
		stConsumeItem.iCount = iItemCount ;
	}
	else
	{
		stConsumeItem.iContainerSlot.ContainerType = CONTAINER_TYPE_INVALID ;
		stConsumeItem.iContainerSlot.SlotIndex = 0 ;
		stConsumeItem.iCount = 0 ;
	}

	packet.AddUINT32( GUILD_CS_RANKUP );
	packet.AddUINT8( iNewLevel );
	packet.AddData( &stConsumeItem, sizeof(TRASH_ITEM));

	SENDPACKET(packet) ;
	return true ;
}




// GUILD_CS_CHANGEMARK,
// UINT32			uiGMarkID;							// 길드마크LDT아이디
// TRASH_ITEM		stConsumeItem;						// 소모되는아이템
bool SPGuildArchive::SendGuildEmblemChange(int iEmblemID, int iItemID, int iItemCount)
{
	CPacket		packet;
	TRASH_ITEM	stConsumeItem;

	SPPlayerInvenArchive* pkInvenArchive = g_pGOBManager->GetLocalPlayer()->GetInvenArchive() ;
	if(pkInvenArchive == NULL)
		return false ;

	if( iItemID > 0 )	{
		SPItem* pSearchItem = pkInvenArchive->GetSPItem(INVENTORY_EVENT, iItemID ) ;
		if( pSearchItem == NULL || pSearchItem->GetSlotIndex() < 0 ) {
			return false ;
		}
		stConsumeItem.iContainerSlot.ContainerType = pSearchItem->GetContainerType() ;
		stConsumeItem.iContainerSlot.SlotIndex = pSearchItem->GetSlotIndex() + 1 ;
		stConsumeItem.iCount = iItemCount ;
	}
	else
	{
		stConsumeItem.iContainerSlot.ContainerType = CONTAINER_TYPE_INVALID ;
		stConsumeItem.iContainerSlot.SlotIndex = 0 ;
		stConsumeItem.iCount = 0 ;
	}

	packet.AddUINT32( GUILD_CS_CHANGEMARK );
	packet.AddUINT32( iEmblemID );
	packet.AddData( &stConsumeItem, sizeof(TRASH_ITEM));

	SENDPACKET(packet) ;
	return true ;
}


/**
	길드 추가 정보 요청 패킷 (길드 레벨, 슬로건)
*/
bool SPGuildArchive::SendGuildCommonInfo()
{
	DeleteCommonInfo() ;

	CPacket packet;
	packet.AddUINT32( GUILD_CS_GUILD_COMMON );	

	SENDPACKET(packet) ;
	return true ;
}






//////////////////////////////////////////////////////////////////////////
//
//	Crop
//
//////////////////////////////////////////////////////////////////////////

/**
	[묘목 심기] 패킷
*/
bool SPGuildArchive::SendCropRegist(UINT32 iCropID, TRASH_ITEM stConsumeItem)
{
	CPacket packet;
	packet.AddUINT32( GUILD_CS_PLANT );	
	packet.AddUINT32( iCropID );
	packet.AddData(&stConsumeItem, sizeof(TRASH_ITEM));
	(((SPNetworkManager*)g_pNetworkManager)->GetPacketHandler())->PostMsg(&packet);
	return true ;
}


/**
	[비료/촉진제 주기]
*/
bool SPGuildArchive::SendCropNourish(UINT32 iBoosterID, TRASH_ITEM stConsumeItem)
{
	CPacket packet;
	packet.AddUINT32( GUILD_CS_NOURISH );	
	packet.AddUINT32( iBoosterID );
	packet.AddData(&stConsumeItem, sizeof(TRASH_ITEM));
	(((SPNetworkManager*)g_pNetworkManager)->GetPacketHandler())->PostMsg(&packet);
	return true ;
}

/**
	[길드 묘목 제거]
*/
bool SPGuildArchive::SendCropEliminate()
{
	CPacket packet;
	packet.AddUINT32( GUILD_CS_GRUB );	
	(((SPNetworkManager*)g_pNetworkManager)->GetPacketHandler())->PostMsg(&packet);
	return true ;
}



/**
	묘목 심기
	return int 0 : 성공
	return int 1 : CropItem 을 찾지 못함 (다른 아이템)
	return int 2 : Require 1
	return int 3 : Require 2
	return int 4 : Require 3
*/
int SPGuildArchive::UseCropItem(SPItem* pItem)
{
	TRASH_ITEM stConsumeItem ;
	CROP_INFO* pCrop = NULL ;
	if( g_pkGuildManager->FindCropInfobyConsumeID(pItem->GetItemStatus()->GetItemID(), pCrop) )
	{
		stConsumeItem.iContainerSlot.ContainerType = pItem->GetContainerType() ;
		stConsumeItem.iContainerSlot.SlotIndex = pItem->GetSlotIndex() + 1 ;	// Server는 1Base
		stConsumeItem.iCount = 1 ;
		// Require Check
		for( int i = 0; i < CROP_REQUIRE_MAX; i++)
		{
			if( false == g_pCheckManager->CheckCondition(pCrop->stRequire[i].iRequireType
				, pCrop->stRequire[i].iRequireID
				, pCrop->stRequire[i].iRequireValue1
				, pCrop->stRequire[i].iRequireValue2) )
			{
				return i+2 ;
			}
		}
		SendCropRegist(pCrop->iItemID, stConsumeItem) ;
		return 0 ;
	}
	return 1 ;
}

/**
	비료나 촉진제 아이템이 Drag&Drop 되는 경우 처리
	@param iType  1:비료, 2:촉진제
	@return 0 : 성공
	@return 1 : 다른 아이템
	@return 2 : 타입이 다름 (비료 != 촉진제)
	@return 3 : Timeless
*/
int SPGuildArchive::UseNourishItem(int iUseType, SPItem* pItem)
{
	TRASH_ITEM stConsumeItem ;
	BOOSTER_INFO* pUseBooster = NULL ;
	if( g_pkGuildManager->FindBoosterInfobyItem(pItem->GetItemStatus()->GetItemID(), pUseBooster) )
	{
		if( iUseType != pUseBooster->iType )
			return 2 ;
		if( m_pCropArchive->CheckAddBoosterTime(pUseBooster->iType, pUseBooster->iCalcTime) == true )
		{
			stConsumeItem.iContainerSlot.ContainerType = pItem->GetContainerType() ;
			stConsumeItem.iContainerSlot.SlotIndex = pItem->GetSlotIndex() + 1 ;	// Server는 1Base
			stConsumeItem.iCount = 1 ;

			SendCropNourish(pUseBooster->lItemID, stConsumeItem) ;
			return 0 ;
		}
		return 3 ;
	}
	return 1 ;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Update
//
//////////////////////////////////////////////////////////////////////////
bool SPGuildArchive::ChangeSlogun(int iMode, const char* strSlogun)
{
	if( iMode == 0 )	// Send
	{
		m_strSendSlogun = strSlogun ;
		CPacket packet;
		packet.AddUINT32( GUILD_CS_UPDATE_SLOGAN );	
		packet.AddData( strSlogun, GUILD_MAX_SLOGAN+1);
		(((SPNetworkManager*)g_pNetworkManager)->GetPacketHandler())->PostMsg(&packet);
	}
	else				// Recv
	{
		m_strGuildSlogun = m_strSendSlogun ;
		m_strSendSlogun.clear() ;
		//g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(27000024) );
		g_pInterfaceManager->SPChildWndSendMessage(WIID_GUILD_INFO, SPIM_GUILD_REFRESH_COMMON, 0, 0);
	}
	return true ;
}


/**
	초대에 사용될 사용자 이름 관련
*/
bool SPGuildArchive::SetInviteName(const char* szRequestName)
{
	// 3바이트에서 2바이트로 변경. 일본 요청사항 [8/20/2007 AJJIYA]
	if( strlen(szRequestName) < MIN_LEN_NAME)		return false ;
	m_strRequestName = szRequestName ;
	return true ;
}
void SPGuildArchive::ClearInviteName()
{
	m_strRequestName.clear() ;
}
const char* SPGuildArchive::GetInviteName()
{
	return m_strRequestName.c_str() ;
}

/**
	Guild 내부 Grade를 업데이트
*/
bool SPGuildArchive::UpdateMyGrade(int iGrade)
{
	if( iGrade < GUILD_GRADE_01 || iGrade > _MAX_GUILD_GRADE-1 )
		return false ;
	m_iGuildGrade = iGrade ;
	SetGuildAuthority(m_iGuildGrade) ;
	//소痢뒈貫맣긴
	if ( g_pCIGWLoaderDx9 )
	{
		TCHAR GuildGradeNew[10];
		sprintf( GuildGradeNew, "%d", m_iGuildGrade );
		g_pCIGWLoaderDx9->OnCommunityEventA( ceSetGuildTitle, GuildGradeNew );
	}
	return true ;
}


/**
	GuildEmblem 업데이트
*/
bool SPGuildArchive::UpdateMyEmblem(int iGMark1, int iGMark2, int iGMark3)
{
	int iChangeLayer = 0 ;
	GUILD_BROADCAST		stGuildBroad ;
	if( m_iGuildEmblem[0] != iGMark1 )	{
		iChangeLayer = 0 ;
		m_iGuildEmblem[0] = iGMark1 ;
	}
	if( m_iGuildEmblem[1] != iGMark2 )	{
		iChangeLayer = 1 ;
		m_iGuildEmblem[1] = iGMark2 ;
	}
	if( m_iGuildEmblem[2] != iGMark3 )	{
		iChangeLayer = 2 ;
		m_iGuildEmblem[2] = iGMark3 ;
	}

	stGuildBroad.arrGMark[0] = m_iGuildEmblem[0] ;
	stGuildBroad.arrGMark[1] = m_iGuildEmblem[1] ;
	stGuildBroad.arrGMark[2] = m_iGuildEmblem[2] ;
	stGuildBroad.byGRank = (UINT8)GetGuildLevel() ;
	stGuildBroad.byGType = (UINT8)GetGuildProperty() ;
	stGuildBroad.iGNum = m_iGuildID ;
	strcpy(stGuildBroad.szGName, GetGuildName()) ;

	g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_SET_GUILD_INFO, (LPARAM)&stGuildBroad);
	g_pInterfaceManager->SPChildWndSendMessage(WIID_GUILD_EMBLEM, SPIM_GUILD_EM_CHANGE_RESULT, iChangeLayer, 0);
	return true ;
}



//////////////////////////////////////////////////////////////////////////
//
//	Packet Receive Process
//	
//////////////////////////////////////////////////////////////////////////
/**
	PPCGUILD : BROADCAST 와 COMMON이 함께 온다.
	GameConnect, GuildCreate, GuildJoin시에 받음.
*/
void SPGuildArchive::SetGuildBasicInfo(PPCGUILD pPCGuild)
{
	if( SPLocalizeManager::GetInstance()->IsEnableValue(LV_GUILD) )	{
		SetEnableBasicInfo( SetBasicInfo(pPCGuild->iGNum, pPCGuild->arrGMark[0], pPCGuild->arrGMark[1], pPCGuild->arrGMark[2], pPCGuild->byGrade, pPCGuild->byGType, pPCGuild->byGRank, pPCGuild->szGName, pPCGuild->iPoint) ) ;
		SetCropInformation(CROP_SET_DATA, pPCGuild->uiCropID, pPCGuild->uiRemainTime, pPCGuild->uiNourishTime, pPCGuild->uiHastenSumTime ) ;
	}
}


void SPGuildArchive::SetGuildCommonInfo(PGUILD_COMMON pPCGuild)
{
	if( SPLocalizeManager::GetInstance()->IsEnableValue(LV_GUILD) )	{
		SetProperty(pPCGuild->szGSlogan) ;
		if( g_pInterfaceManager->GetGuildInfoWindow()->IsShow() )
			g_pInterfaceManager->SPChildWndSendMessage(WIID_GUILD_INFO, SPIM_GUILD_REFRESH_COMMON, 0, 0);
	}
	SetCropInformation(CROP_SET_DATA, pPCGuild->uiCropID, pPCGuild->uiRemainTime, pPCGuild->uiNourishTime, pPCGuild->uiHastenSumTime ) ;	// Set
}





