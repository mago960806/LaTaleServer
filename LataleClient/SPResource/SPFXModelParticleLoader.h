// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-5-9    13:51 
//***************************************************************************

#pragma once

#include <string>

class SPFXModel;
class SPFXModelBaseLoader;
class SPStream;

struct SPFXStreamDefaultData;

class SPFXModelParticleLoader : public SPFXModelBaseLoader
{
private:

	// {{ WORD CASE MAP
	DECLARE_WORDCASE_MAP ( OnAreaType				);		//	파티클 영역 타입
	DECLARE_WORDCASE_MAP ( OnPathRotation			);		//	패스에 따른 회전을 자동으로 하는 유무
	DECLARE_WORDCASE_MAP ( OnFrameDataType			);		//	프레임 데이터 타입에 따른 세팅 준비
	DECLARE_WORDCASE_MAP ( OnValueDataType			);		//	값 데이터 타입에 따른 세팅 준비
	DECLARE_WORDCASE_MAP ( OnObjectValueDataType	);		//	오브젝트 값데이터 타입에 따른 세팅 준비
	DECLARE_WORDCASE_MAP ( OnBlockBegin				);		//	블럭 시작
	DECLARE_WORDCASE_MAP ( OnTime					);		//	프레임당 지속시간
	DECLARE_WORDCASE_MAP ( OnScaleX					);		//	프레임당 X 크기 값
	DECLARE_WORDCASE_MAP ( OnScaleY					);		//	프레임당 Y 크기 값
	DECLARE_WORDCASE_MAP ( OnRadianDegree			);		//	프레임당 회전 각도
	DECLARE_WORDCASE_MAP ( OnLRSwap					);		//	프레임당 좌우 플립 값
	DECLARE_WORDCASE_MAP ( OnColor					);		//	프레임당 컬러 값
	DECLARE_WORDCASE_MAP ( OnSrcRect				);		//	프레임당 이미지 사각형
	DECLARE_WORDCASE_MAP ( OnAdjustRect				);		//	프레임당 렌더할때 보정하는 사각형
	DECLARE_WORDCASE_MAP ( OnAreaRect				);		//	프레임당 영역
	DECLARE_WORDCASE_MAP ( OnCreateTime				);		//	프레임당 생성되는 시간
	DECLARE_WORDCASE_MAP ( OnCreateCount			);		//	프레임당 생성되는 갯수
	DECLARE_WORDCASE_MAP ( OnMagnetPos				);		//	자석 위치
	DECLARE_WORDCASE_MAP ( OnFirstKeyTime			);		//	MAP의 첫번째 키 Time
	DECLARE_WORDCASE_MAP ( OnSecondKeyValue			);		//	MAP의 두번째 값 Value
	DECLARE_WORDCASE_MAP ( OnBlockEnd				);		//	블럭 끝
	//

	// }}

	int								m_iDeep;				//	블럭을 체크하기 위한 값
	SPFXParticleFrameData			m_ParticleFrameData;	//	프레임 파이클 데이터
	SPFXFrameData					m_FrameData;			//	프레임 이미지 데이터
	SPFX_PARTICLE_FRAME_DATA_TYPE	m_eFrameDataType;		//	프레임 데이터 타입
	SPFX_PARTICLE_DATA_TYPE			m_eValueDataType;		//	값 데이터 타입
	SPFX_PARTICLE_OBJECT_DATA_TYPE	m_eObjectValueDataType;	//	오브젝트 값 데이터 타입
	float							m_fFirstKeyTime;		//	임시로 보관 하는 키 값
	float							m_fSecondKeyValue;		//	임시로 보관 하는 키 값
	STD_MAP_PARTICLE_VALUE			m_mValueData;			//	임시로 보관 하는 데이터
	

public:
	SPFXModelParticleLoader(void);
	virtual ~SPFXModelParticleLoader(void);

	virtual	bool	LoadASCII	( SPFXModel* pModel , SPStream* pFile );
	virtual	bool	LoadBinary	( SPFXModel* pModel , SPStream* pFile , SPFXStreamDefaultData& DefaultData , float fVersion );
};

//////////////////////////////////////////////////////////////////////////
//	FXM 모델 2.1 일때 파티클
//////////////////////////////////////////////////////////////////////////
//
//class SPFXModelParticleLoader21 : public SPFXModelBaseLoader
//{
//private:
//
//	// {{ WORD CASE MAP
//	DECLARE_WORDCASE_MAP ( OnDelay					);		//	Dealy값 읽기
//	DECLARE_WORDCASE_MAP ( OnMaxRenderParticle		);		//	최대 렌더되는 파티클수
//	DECLARE_WORDCASE_MAP ( OnMaxParticle			);		//	최대 파티클수
//	DECLARE_WORDCASE_MAP ( OnRadianDegree			);		//	회전 값
//	DECLARE_WORDCASE_MAP ( OnScale					);		//	크기 값
//	DECLARE_WORDCASE_MAP ( OnParticleMove			);		//	파티클 이동시의 옵션
//	DECLARE_WORDCASE_MAP ( OnColor					);		//	컬러 읽기
//	DECLARE_WORDCASE_MAP ( OnSrcRect				);		//	이미지의 사각형
//	DECLARE_WORDCASE_MAP ( OnAdjustRect				);		//	이미지를 렌더할때 보정되는 사각형
//	// }}
//
//	SPID_FX			m_eSPID_FX;								//	FX모델 ID
//
//public:
//	SPFXModelParticleLoader21(void);
//	virtual ~SPFXModelParticleLoader21(void);
//
//	virtual	bool	LoadASCII	( SPFXModel** ppModel , SPStream* pFile , SPID_FX eSPID_FX );
//	virtual	bool	LoadBinary	( SPFXModel** ppModel , SPStream* pFile	, SPID_FX eSPID_FX , SPFXStreamDefaultData& DefaultData , float fVersion );
//};
