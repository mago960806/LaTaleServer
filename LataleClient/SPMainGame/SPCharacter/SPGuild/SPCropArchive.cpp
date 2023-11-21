// Copyright (C) DURAGON
//***************************************************************************
// System Name : GUILD Crops
// Comment     : 
// Creation    : DURAGON 2007-5-04    11:18 
//***************************************************************************

#include "SPCommon.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
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
#include "SPGOBManager.h"
#include "SPGOBStatus.h"
#include "SPCheckManager.h"
#include "SPLDTFile.h"
#include "SPLDTManager.h"

#include "SPItemCluster.h"
#include "SPItemAttr.h"

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
#include "_Error_log.h"




SPGuildCrops::SPGuildCrops()
{
}

SPGuildCrops::~SPGuildCrops()
{
}


void SPGuildCrops::Init()
{
	DestroyCrop() ;
}


void SPGuildCrops::DestroyCrop()
{
	m_InstanceID		= 0 ;
	m_ClassID			= CLASS_ID_NULL;
	m_iCropLevel		= -1 ;
	m_bComplete			= false ;
	m_iCropID			= 0 ;
	m_iUseItemID		= 0 ;
	m_iRemainTime		= 0 ;
	m_iNourishTime		= 0 ;
	m_iHastenSumTime	= 0 ;
	m_iCropCountTime	= 0 ;
	m_fProcessTime		= 0.0f ;
	m_iLimitHastenTime	= 0 ;
	m_strCropName.clear() ;

	for( int i = 0; i < 4; i++ )
	{
		m_iLevelTime[i]		= 0 ;
		m_stDisplayCrop[i].Clear() ;
	}
}


// 작물 보유 여부
bool SPGuildCrops::hasCorps()
{
	return ( m_iCropID > 0 ) ? true : false ;
}

// 작물 성장 완료 여부
bool SPGuildCrops::IsComplete()
{
	if( m_bComplete )			return true ;
	if( m_iRemainTime == 0 )	return m_bComplete ;
	return false ;
}
// 작물의 해당 레벨의 파일명
UINT32 SPGuildCrops::GetCropImageID(int iCLevel)
{
	return m_stDisplayCrop[iCLevel].iImageID ;
}
// 작물의 해당 레벨의 Dialog 파일명(GlobalRes)
UINT32 SPGuildCrops::GetCropDlgImageID(int iCLevel)
{
	return m_stDisplayCrop[iCLevel].iDlgImageID ;
}
const char* SPGuildCrops::GetCropCutinTalk(int iCLevel)
{
	return m_stDisplayCrop[iCLevel].strTalk.c_str() ;
}
UINT32 SPGuildCrops::GetCropCutinImageID(int iCLevel)
{
	return m_stDisplayCrop[iCLevel].iCutInImageID ;
}

int SPGuildCrops::GetCropLevel()
{
	return m_iCropLevel ;
}

int SPGuildCrops::CalcCropLevel()
{
	int iLevel = 0 ;
	if( hasCorps() == false )		return 0 ;
	for( int i = 0; i < 4; i++)
	{
		if( (m_iRemainTime - m_iCropCountTime) <= m_iLevelTime[i] )
			iLevel = i ;
	}
	return iLevel ;
}

const char* SPGuildCrops::GetCropName()
{
	if( m_strCropName.empty())
		return NULL ;
	return m_strCropName.c_str();
}


/**
	LevelUp이 되는 경우 작물의 InstanceID, ClassID를 통해 GOBManager에서 작물을 찾아 이미지를 변경해야 한다.
	따라서 작물의 Instance_ID & ClassID를 저장해 놓는다.
*/
void SPGuildCrops::SetCropClassID(INSTANCE_ID InstanceID/*=0*/, CLASS_ID ClassID/*=CLASS_ID_NULL*/)
{
	m_InstanceID = InstanceID ;
	m_ClassID	= ClassID ;
	ErrorFLogs->Error_Log("Set Crop InstanceID[%d], ClassID[%d]", m_InstanceID, m_ClassID) ;

	if( m_iCropLevel < 0 || m_iCropLevel >= CROP_LEVEL_MAX )	{
		ErrorFLogs->Error_Log("##CropLevelUp Fail : Level Invalid[%d]", m_iCropLevel) ;
		return ;
	}
	g_pGOBManager->UpdateEventNPC(InstanceID, ClassID, GetCropImageID(m_iCropLevel)) ;
	
}

bool SPGuildCrops::GetCropClassID(INSTANCE_ID& InstanceID, CLASS_ID& ClassID)
{
	if( InstanceID == 0 )		return false ;
	InstanceID	= m_InstanceID ;
	ClassID		= m_ClassID ;
	//ErrorFLogs->Error_Log("Get Crop InstanceID[%d], ClassID[%d]", m_InstanceID, m_ClassID) ;
	return true ;
}


/**
	Crop의 Data를 Update한다.
	@param int iCmd : 0(클리어), 1, 초기화, 2(셋팅), 3(시간 업데이트)
*/
void SPGuildCrops::SetCropInformation(int iCmd, UINT32 iCropID/*=0*/, UINT32 iRemainTime/*=0*/, UINT32 iNourishTime/*=0*/, UINT32 iHastenSumTime/*=0*/)
{
	CROP_INFO stCrop ;
	switch( iCmd )
	{
	case CROP_SET_INITIALIZE:		// Initialize (Create)
		m_iCropID = iCropID ;
		if( g_pkGuildManager->FindCropInfobyCropID(iCropID, stCrop) )	{
			SetCropTimes( stCrop.iGrowTime[0] , 0, 0) ;
			SetCropImageInfo(m_iCropID) ;
			g_pInterfaceManager->SPChildWndSendMessage(WIID_GUILD_INFO, SPIM_GUILD_CROP_SET, 0, 0);
		}
		else	{
			ErrorFLogs->Error_Log("##Can't Find CropInfo:: CropID[%d]", m_iCropID) ;
			g_pInterfaceManager->GetGuildInfoWindow()->Hide() ;
		}
		break ;

	case CROP_SET_DATA:			// Set (Receive Server Info)
		if( iCropID == 0 )		{
			ErrorFLogs->Error_Log("##UpdateCropInfomation:: CropID is ZERO [%d][%d]", m_iCropID, iCropID) ;
			break ;
		}
		m_iCropID = iCropID ;
		SetCropTimes( iRemainTime, iNourishTime, iHastenSumTime) ;
		SetCropImageInfo(m_iCropID) ;
		break ;

	case CROP_SET_NOURISH:			// Update (비료, 촉진제 사용)
		SetCropTimes( iRemainTime, iNourishTime, iHastenSumTime) ;
		break ;

	case CROP_SET_CLEAR:			// Clear
	default:
		m_iCropID = 0 ;
		m_iUseItemID = 0 ;
		m_bComplete = false ;
		SetCropTimes( 0, 0, 0) ;
		DestroyCrop() ;
		break ;
	}
}

void SPGuildCrops::SetCropTimes(UINT32 iRemainTime, UINT32 iNourishTime, UINT32 iHastenSumTime)
{
	m_iRemainTime		= iRemainTime ;
	m_iNourishTime		= iNourishTime ;
	m_iHastenSumTime	= iHastenSumTime ;
	m_iCropCountTime	= 0 ;
	m_fProcessTime		= 0.0f ;
}

/**
	작물 성장이 완료된 경우
*/
void SPGuildCrops::SetGrowthComplete()
{
	m_bComplete = true ;
	SetCropTimes(0, 0, 0) ;
}



/**
	CropID에 해당하는 Crop 정보 및 Image를 설정한다.
*/
void SPGuildCrops::SetCropImageInfo(UINT32 iCropID) 
{
	SPItemAttr* pItemAttr = NULL ;
	DISPLAY_CROP_UNIT	dpCrop ;
	CROP_INFO			CropInfo ;
	if( g_pkGuildManager->IsEnable() )
	{
		m_iCropLevel = CalcCropLevel() ;
		if( m_iCropLevel == CROP_LEVEL_MAX -1 )		SetGrowthComplete() ;
		if( g_pkGuildManager->FindCropInfobyCropID(iCropID, CropInfo) == true )
		{
			m_iLimitHastenTime = CropInfo.iHastenTime ;
			m_iUseItemID = CropInfo.iConsumeItem ;
			if( g_pItemCluster->GetItemInfo(m_iUseItemID, pItemAttr) )
				m_strCropName = pItemAttr->m_strName ;
			for( int i = 0; i < 4; i++ )
			{
				m_iLevelTime[i] = CropInfo.iGrowTime[i] ;
				if( g_pkGuildManager->GetCropImageByCrop(iCropID, i, dpCrop ) == true )
				{
					m_stDisplayCrop[i].iImageID		= dpCrop.iImageID ;
					m_stDisplayCrop[i].iCutInImageID= dpCrop.iCutInImageID ;
					m_stDisplayCrop[i].iDlgImageID	= dpCrop.iDlgImageID ;
					m_stDisplayCrop[i].strTalk		= dpCrop.strTalk ;
					//ErrorFLogs->Error_Log("CropImage Info Set Image[%d] CuiIn[%d], Dlg[%d] Talk[%s]", m_stDisplayCrop[i].iImageID, m_stDisplayCrop[i].iCutInImageID, 
					//																				  m_stDisplayCrop[i].iDlgImageID	, m_stDisplayCrop[i].strTalk.c_str()) ;
				}
			}
		}
	}
}

/**
	작물 성장 시간을 보여주기 위한 Process Timer
	실제 창이 보여지고 있는 경우에만 돌아간다.
*/
bool SPGuildCrops::CropProcess(float fTime, int &iUpdate)
{
	if( !hasCorps() )			return false;		// 작물 없음
	if( m_iNourishTime == 0 )	return false;		// 비료시간 없음

	if( m_iNourishTime > m_iCropCountTime )
	{
		m_fProcessTime += fTime ;
		if( m_fProcessTime > 1.0f ){
			++m_iCropCountTime ;
			m_fProcessTime -= 1.0f ;
			if( CheckCropLevelup() )
				iUpdate = CROP_UPDATE_LEVEL ;
			else
				iUpdate = CROP_UPDATE_TIME ;
			return true ;
		}
	}
	return false ;
}


/**
	LevelUp이 되었는지 체크한다.
*/
bool SPGuildCrops::CheckCropLevelup()
{
	if( m_iCropLevel < 0 )	return false ;
	if( m_iCropLevel != CalcCropLevel() )	{
		m_iCropLevel = CalcCropLevel() ;
		return true ;
	}
	return false ;
}



/**
	비료 사용시간 제한 체크
	비료를 사용할수 있는 시간이 있으면 OK
	Booster는 비료의 시간이 남아있으면 무조건 OK
*/
bool SPGuildCrops::CheckAddBoosterTime(int iType, UINT32 iAddTime)
{
	if( iType == BOOSTER_TYPE_MANURE )
		return (m_iRemainTime - m_iNourishTime) > 0 ? true : false ;
	else if( m_iNourishTime > 0 )
		return ( (m_iLimitHastenTime - m_iCropCountTime) > 0 ) ? true : false ;
	return false ;
}
