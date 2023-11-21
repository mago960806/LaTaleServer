// Copyright (C) DURAGON
//***************************************************************************
// System Name : GUILD CROPS
// Comment     : 
// Creation    : DURAGON 2007-4-04    11:18 
//***************************************************************************

#pragma once

enum { CROP_UPDATE_NULL, CROP_UPDATE_TIME , CROP_UPDATE_LEVEL,} ;
enum { CROP_SET_CLEAR, CROP_SET_INITIALIZE, CROP_SET_DATA, CROP_SET_NOURISH } ;

struct DISPLAY_CROP_UNIT ;
//class SPGuildLDT ;
class SPGuildCrops
{

public:
	SPGuildCrops();
	virtual ~SPGuildCrops();

	void	Init() ;
	void	DestroyCrop() ;
	bool	CropProcess(float fTime, int &iUpdate) ;

	bool	hasCorps() ;							// 작물이 있는지 여부
	bool	IsComplete() ;							// 작물 성장 완료
	
	UINT32	GetCropImageID(int iCLevel) ;			// 작물 이미지 ID (MapEvent)
	UINT32	GetCropDlgImageID(int iCLevel) ;		// 작물 이미지 ID(GlobalRes)
	UINT32	GetCropCutinImageID(int iCLevel) ;		// 작물 Cut In Image
	const char* GetCropCutinTalk(int iCLevel) ;		// Cut In Talk
	UINT32	GetUseItemForIcon()	{	return m_iUseItemID ;	}	// 작물 아이콘을 (사용 아이템)
	const char* GetCropName() ;						// 작물 이름
	int		GetCropLevel() ;						// 작물 레벨
	

	void	SetCropClassID(INSTANCE_ID InstanceID=0, CLASS_ID ClassID=CLASS_ID_NULL) ;
	bool	GetCropClassID(INSTANCE_ID& InstanceID, CLASS_ID& ClassID) ;
	void	SetCropInformation(int iCmd, UINT32 iCropID=0, UINT32 iRemainTime=0, UINT32 iNourishTime=0, UINT32 iHastenSumTime=0) ;
	void	SetCropTimes(UINT32 iRemainTime, UINT32 iNourishTime, UINT32 iHastenSumTime) ;
	void	SetGrowthComplete() ;
	void	SetCropName(const char* pName) {	m_strCropName = pName ;		}

	UINT32	GetRemainTime()			{	return m_iRemainTime - m_iCropCountTime ;	}
	UINT32	GetNourishTime()		{	return m_iNourishTime - m_iCropCountTime ;	}
	UINT32	GetHastenSumTime()		{	return m_iHastenSumTime ;	}

	bool CheckAddBoosterTime(int iType, UINT32 iAddTime) ;
	bool CheckCropLevelup() ;					// LevelUP Check (Process, USE Nourish)

private:
	void SetCropImageInfo(UINT32 iCropID) ;
	int	 CalcCropLevel() ;						// 작물 레벨 계산

private:
	INSTANCE_ID	m_InstanceID;
	CLASS_ID	m_ClassID;
	int			m_iCropLevel ;
	bool		m_bComplete ;				// 작물성장 완료
	UINT32		m_iCropID ;					// 작물 ID ( ID == 0 is Empty)
	UINT32		m_iUseItemID ;				// 사용아이템 (For Icon)
	UINT32		m_iRemainTime ;				// 성장 완료까지 남은 시간
	UINT32		m_iNourishTime ;			// 남은 비료 시간
	UINT32		m_iHastenSumTime ;			// 촉진제 누적 시간
	UINT32		m_iCropCountTime ;			// Client가 움직인 시간
	float		m_fProcessTime ;			// Process에서 계산하는 시간

	DISPLAY_CROP_UNIT	m_stDisplayCrop[4] ;
	//UINT32		m_iCropImageID[4] ;			// 레벨별 CropImageID
	//UINT32		m_iCropDlgImageID[4];		// 레벨별 Dlg용 ImageID

	UINT32		m_iLevelTime[4] ;			// 레벨업 시간
	UINT32		m_iLimitHastenTime ;		// 촉진제 제한시간
	std::string	m_strCropName ;				// 작물 이름

protected:
	
};