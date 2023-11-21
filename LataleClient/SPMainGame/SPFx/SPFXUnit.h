// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-08-24 오전 10:26:19
//***************************************************************************

#pragma once

enum MODEL_CLASS_ID;
enum SPID_FX;

enum FXUNIT_STATE
{
	FS_INIT,
	FS_CLEAN,

	FS_WAIT,
	FS_ENABLE,
	
	FS_RELEASE_READY,
	FS_RELEASE,
};

typedef UINT FXPARAM;

const int FXPARAM_NULL			= 0;
const int FXPARAM_FACING_LEFT	= ( 1 << 0 );
const int FXPARAM_FACING_RIGHT	= ( 1 << 1 );

class SPFXUnit
{
public:
	SPFXUnit										();
	virtual	~SPFXUnit								();

	virtual void			SetNull					();
	virtual void			Init					();
	virtual void			Destory					();

	virtual bool			Process					( float fTime );
	virtual bool			Render					( float fTime );
	virtual void			RenderReset				( float fTime );

	virtual	void			SetFXModel				( SPFXModel* pFXModel );
	virtual	SPFXModel*		GetFXModel				();

	virtual	void			SetPosX					( float fPosX );
	virtual	float			GetPosX					();

	virtual	void			SetPosY					( float fPosY );
	virtual	float			GetPosY					();

	virtual	void			SetLParam				( LPARAM lParam );

	virtual	void			SetGOBID				( GU_ID tGOBID );
	virtual	GU_ID			GetGOBID				();

	virtual	FXUNIT_STATE	GetState				();

	void					SetParentPos			( float fPosX , float fPosY );

	void					SetParentFacing			( FACING eFacing );

	void					SetLoop					( BOOL bLoop );
	BOOL					GetLoop					();

	void					SetContinue				( BOOL bContinue );
	BOOL					GetContinue				();

	UINT					GetFXID					();

	void					SetLifeTime				( float fLifeTime );
	float					GetLifeTime				();

	void					SetVelocity				( float fVelocity );
	float					GetVelocity				();

	void					SetDestPosX				( float fPos );
	float					GetDestPosX				();

	void					SetDestPosY				( float fPos );
	float					GetDestPosY				();

	void					SetFollowParent			( BOOL bFollow );

	void					SetFollow				( int iFollow );
	int						GetFollow				();

protected:
//	void					InitParticleInstance	( SPFXParticleInstance& ParticleInstance );

	void					ProcessPos				( float fTime );
//	void					ProcessFrameAnimation	();
//	void					ProcessParticle			();

	bool					IsClipping				( int iPosX , int iPosY , int iSrcRectSizeX , int iSrcRectSizeY , float fScaleX , float fScaleY );

	//////////////////////////////////////////////////////////////////////////
	//
	//	변수들
	//

	float								m_fPosX;
	float								m_fPosY;

	float								m_fCamX;
	float								m_fCamY;

	float								m_fParentPosX;
	float								m_fParentPosY;

	FACING								m_eParentFacing;

	float								m_fAccumulateRender;
	float								m_fAccumulateProcess;

	// Common
	float								m_fCurRotateDegree;
	float								m_fModelDelay;
	float								m_fCurDelay;
	D3DXCOLOR							m_RGBA;
	TEXTURE_EFFECT						m_eTextureEffect;
	RECT								m_RectSrc;
	RECT								m_RectAdjust;			// 파티클일땐 사이즈이다..
	float								m_fScaleX;
	float								m_fScaleY;
	bool								m_bLRSwap;

	int									m_iModelID;				// SPFXModel instance id	
	MODEL_CLASS_ID						m_ClassID;

	FXPARAM								m_FXParam;				// 외부에서 셋팅한 값
	FXPARAM								m_FXCurUseParam;		// 내부에서 조작한 인스턴트한 값
	
	SPFXModel*							m_pFXModel;
	SPTexture**							m_ppCurImage;

	FXUNIT_STATE						m_FUState;
	int									m_iFollowParent;
	GU_ID								m_tGOBID;

	float								m_fFXMLifeTime;
	BOOL								m_bLoop;				//	반복 여부
	BOOL								m_bContinue;			//	유닛 시간에 상관없이 계속~~쭈우욱
	float								m_fUnitLifeTime;
	float								m_fUnitVelocity;
	float								m_fDestPosX;
	float								m_fDestPosY;
	BOOL								m_bScreenRender;		//	화면좌표로 찍는다.
	

};
