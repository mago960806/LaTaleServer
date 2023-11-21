// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-08-24 오전 10:26:19
//***************************************************************************

#pragma once

class SPFXModel;

class SPFXModelParticle : public SPFXModel
{
public:
	SPFXModelParticle					();
	virtual ~SPFXModelParticle			();

	virtual void		SetNull			();
	virtual void		Init			();
	virtual void		Destroy			();
	virtual bool		SetImage		( SPTexture** ppImage );
	virtual LRESULT		GetMemberValue	( MODEL_VALUE_TYPE mvMsg , LPARAM lParam = 0 );

protected:

	SPTexture*							m_pFrameImage;										//	텍스처

	int									m_iFrameCount;										//	프레임 데이터 갯수
	std::vector<SPFXParticleFrameData*>	m_vpFrameData;										//	영역에 대한 프레임 데이터

	SPFX_PARTICLE_AREA_TYPE				m_eAreaType;										//	영역 종류

	SPFX_PARTICLE_DATA_TYPE				m_eDataType;										//	파티클 데이터 종류
	STD_MAP_PARTICLE_VALUE				m_mValueData[ PARTICLE_DATA_COUNT ];				//	파티클 데이터 값

	SPFX_PARTICLE_OBJECT_DATA_TYPE		m_eObjectDataType;									//	파티클 객체 데이터 종류
	STD_MAP_PARTICLE_VALUE				m_mObjectValueData[ PARTICLE_OBJECT_DATA_COUNT ];	//	파티클 객체 데이터 값

	int									m_iImageFrameCount;									//	이미지 프레임 데이터 갯수
	std::vector<SPFXFrameData*>			m_vpImageFrameData;									//	영역에 대한 이미지 프레임 데이터

	BOOL								m_bPathFollow;										//	패스에 따른 회전 자동 계산 유무

};

//////////////////////////////////////////////////////////////////////////
//
//	FX Model Version 2.1 일때 사용 하는 클래스
//

//class SPFXModelParticle21 : public SPFXModel {
//public:
//	SPFXModelParticle21();
//	SPFXModelParticle21(SPID_FX ID, float fDelay);
//	virtual ~SPFXModelParticle21();
//
//	virtual void SetNull();
//	virtual void Init();
//	virtual void Destroy();
//	virtual bool SetImage(SPTexture** ppImage);	
//	virtual LRESULT GetMemberValue(MODEL_VALUE_TYPE mvMsg, LPARAM lParam = 0);
//
//protected:
//	SPTexture*				m_pFrameImage;	
//	float					m_fRotation;			// 각 프레임의 회전값.	
//	float					m_fScale;				// 사이즈
//
//	RECT					m_RectSrc;				// 각 프레임의 텍스쳐소스.	
//	RECT					m_RectSize;				// 각 프레임의 렌더타겟의 렌더하는 영역사이즈
//
//	UINT					m_iMaxRenderParticle;	// 최대 그릴수 있는 수
//	UINT					m_iMaxParticle;			// 몇개 까지 찍구 끝낼거냐 0이면 항상 m_iMaxRenderParticle로 찍는다.
//
//	D3DXCOLOR				m_RGBA;					// 컬러
//
//
//	SPFX_PARTICLE_MOVE		m_ParticleMove;			// 어떤 파티클 종류냐
//
//
//	// VECTOR4를 인자로 사용한다..
//	// x : VelocityX
//	// y : VelocityY
//	// z : AccelX
//	// w : AccelY
//	D3DXVECTOR4				m_vec4ParticleFactor;	
//
//};
