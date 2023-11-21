// ***************************************************************
//  SPFXParticleObject   version:  1.0   ·  date: 10/12/2006
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2006 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

class SPFXParticleObject
{
public:
	SPFXParticleObject(void);
	virtual ~SPFXParticleObject(void);

	virtual void	SetNull					();
	virtual	void	Create					();
	virtual void	Destory					();
	virtual	void	Process					( float fTime );
	virtual	void	Render					();

	void			RenderOnlyLine			();

	FXUNIT_STATE	GetState				();

	void			SetCarmeraPos			( float* pfCarmeraPosX , float* pfCarmeraPosY );
	void			SetParentPos			( float fParentPosX , float fParentPosY );

	void			SetParentAlpha			( float* pfParentAlpha );
	void			SetParentGravity		( float* pfParentGravity );
	void			SetDirection			( float fDirection );
	void			SetPathFollow			( BOOL bPathFollow );
	void			SetMagnetPos			( int* piMagnetPosX , int* piMagnetPosY );

	void			SetParentRed			( float* pfParentRed );
	void			SetParentGreen			( float* pfParentGreen );
	void			SetParentBlue			( float* pfParentBlue );
	void			SetParentMagnet			( float* pfParentMagnet );

	void			SetParentNextGravity	( float* pfParentNextGravity );

	void			SetParentDirection		( bool bParentDirection );

	void			SetDefaultPos			( float fDefaultPosX , float fDefaultPosY );
	void			SetObjectValue			( SPFX_PARTICLE_OBJECT_DATA_TYPE eObjectDataType , STD_MAP_PARTICLE_VALUE*	pmParticleObjectValue );

	void			SetTexture				( SPTexture* pTexture );
	void			SetRandomStart			( bool bRandom );
	void			AddImageFrameData		( SPFXFrameData* pFrameDaa );

protected:

	void			CalValueData			( float fTime );

	void			GetCalPosByTime			( float& fOutPosX , float& fOutPosY , float fTime , float* pfGravity );
	void			CalNowDirection			( float fTime );

	void			GetCurImageFrameData	();

	FXUNIT_STATE	m_FUState;
	float			m_fAccumulateProcess;

	float*			m_pfCarmeraPosX;				//	카메라 X 위치
	float*			m_pfCarmeraPosY;				//	카메라 Y 위치

	float			m_fParentPosX;					//	부모 X 위치
	float			m_fParentPosY;					//	부모 Y 위치

	float			m_fPosX;						//	최종 계산된 X 위치
	float			m_fPosY;						//	최종 계산된 Y 위치

	float			m_fDefaultPosX;					//	생성시 X 위치
	float			m_fDefaultPosY;					//	생성시 Y 위치

	float*			m_pfParentAlpha;				//	현재 부모의 알파값
	float*			m_pfParentGravity;				//	현재 부모의 중력값
	float			m_fDirection;					//	생성시 진행 방향
	BOOL			m_bPathFollow;					//	진행방향에 따라 회전값 무시
	float			m_fPathFollowAngle;				//	진행방향에 따른 회전값 산출
	float*			m_pfParentNextGravity;			//	다음 진행 시간에 따른 중력값
	int*			m_piMagnetPosX;					//	자석 포인터 위치 X
	int*			m_piMagnetPosY;					//	자석 포인터 위치 Y

	float*			m_pfParentRed;					//	현재 부모의 빨강
	float*			m_pfParentGreen;				//	현재 부모의 녹색
	float*			m_pfParentBlue;					//	현재 부모의 파랑
	float*			m_pfParentMagnet;				//	현재 부모의 자석 포인터

	bool			m_bParentDirection;				//	현재 부모가 바라보는 방향

	float			m_fCalOut[ PARTICLE_OBJECT_DATA_COUNT ];	//	변화량 계산값

	//////////////////////////////////////////////////////////////////////////

	float			m_fStartTimeOut[ PARTICLE_OBJECT_DATA_COUNT ];	//	처음 시간
	float			m_fEndTimeOut[ PARTICLE_OBJECT_DATA_COUNT ];	//	끝 시간

	float			m_fStartValueOut[ PARTICLE_OBJECT_DATA_COUNT ];	//	처음 값
	float			m_fEndValueOut[ PARTICLE_OBJECT_DATA_COUNT ];	//	끝 값

	//////////////////////////////////////////////////////////////////////////

	STD_MAP_PARTICLE_VALUE*		m_pmParticleObjectValue[ PARTICLE_OBJECT_DATA_COUNT ];	//	변화량

//	std::vector<D3DXVECTOR2>	m_vLine1;
//	std::vector<D3DXVECTOR2>	m_vLine2;

	//////////////////////////////////////////////////////////////////////////

	SPTexture*		m_pTexture;						//	부모에서 넘겨 받는다 생성이나 릴리즈 없음!
	bool			m_bRandomStart;

	int				m_iCurFrameNumber;
	int				m_iStartFrameNumber;
	int				m_iEndFrameNumber;

	float			m_fAccumulateImageFrame;
	float			m_fCurFrameDelayTime;

	SPFXFrameData*	m_pCurFrameData;

	std::vector<SPFXFrameData*> m_vpImageFrameData;	//	영역에 대한 이미지 프레임 데이터
};
