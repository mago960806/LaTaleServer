// ***************************************************************
//  SPFXParticleUnit   version:  1.0   ·  date: 09/29/2006
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2006 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#pragma once

class SPFXUnit;
class SPFXParticleObject;

class SPFXParticleUnit : public SPFXUnit
{
protected:
	int										m_iCurFrameNum;
	int										m_iMaxFrameCount;

	float									m_fAccumulateCreateTime;
	float									m_fCreateTime;				// 발생 시간
	int										m_iCreateCount;				// 발생 개수
	SPFX_PARTICLE_AREA_TYPE					m_eAreaType;				// 영역 타입
	BOOL									m_bPathFollow;				// 패스에 따른 회전 산출 유무
	int										m_iMagnetPosX;				// 자석 포인터 위치 X
	int										m_iMagnetPosY;				// 자석 포인터 위치 Y

	float									m_fCalOut[ PARTICLE_DATA_COUNT ];
	float									m_fCalOutNextGravity;

	float									m_fStartTimeOut[ PARTICLE_DATA_COUNT ];		//	처음 시간
	float									m_fEndTimeOut[ PARTICLE_DATA_COUNT ];		//	끝 시간

	float									m_fStartValueOut[ PARTICLE_DATA_COUNT ];	//	처음 값
	float									m_fEndValueOut[ PARTICLE_DATA_COUNT ];		//	끝 값

	float									m_fStartTimeOutNextGravity;					//	처음 다음 중력 시간
	float									m_fEndTimeOutNextGravity;					//	끝 다음 중력 시간

	float									m_fStartValueOutNextGravity;				//	처음 다음 중력 값
	float									m_fEndValueOutNextGravity;					//	끝 다음 중력 값

	STD_VECTOR_SPFXPARTICLEOBJECT			m_vpParticleObject;


	void			CalFrameData			();
	void			CalValueData			( float fTime );

	void			RandomCreatePos			( float& fOutPosX , float& fOutPosY );
	void			RandomCreatePosEllipse	( float& fOutPosX , float& fOutPosY , float fRandomAngle );
	void			RandomCreatePosArea		( float& fOutPosX , float& fOutPosY , float fRandomAngle );

	void			SettingObject			( SPFXParticleObject& stObject );
	void			CreateObject			();
	void			ProcessObject			( float fTime );
	void			RenderObject			();

	float			GetRandomAngle			();


public:
	SPFXParticleUnit						(void);
	virtual ~SPFXParticleUnit				(void);

	virtual void	SetNull					();
	virtual	void	Init					();
	virtual	bool	Process					( float fTime );
	virtual	bool	Render					( float fTime );

	void			RenderOnlyLine			();
};

//////////////////////////////////////////////////////////////////////////
/*
class SPFXParticleUnit21 : public SPFXUnit
{
public:
	SPFXParticleUnit21					(void);
	virtual ~SPFXParticleUnit21			(void);

	virtual void	SetNull				();
	virtual	void	Init				();
	virtual	bool	Process				( float fTime );
	virtual	bool	Render				( float fTime );

protected:

	void			InitParticleInstance( SPFXParticleInstance& ParticleInstance );

	//////////////////////////////////////////////////////////////////////////

	UINT								m_iMaxRenderParticle;	// 최대 그릴수 있는 수
	UINT								m_iMaxParticle;			// 몇개 까지 찍구 끝낼거냐 0이면 항상 m_iMaxRenderParticle로 찍는다.
	UINT								m_iCurNOParticle;
	std::vector<SPFXParticleInstance>	m_vParticleInstance;
	SPFX_PARTICLE_MOVE					m_ParticleMove;


};
*/