// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-08-24 오전 10:26:19
//***************************************************************************

#pragma once

class SPRenderModel;

enum MODEL_VALUE_TYPE;
enum SPID_FX_MODEL;

class SPFXModel : public SPRenderModel
{
public:
	SPFXModel						();
	virtual ~SPFXModel				();

	virtual void	Init			();
	virtual void	SetNull			();
	virtual void	Destroy			();
	virtual bool	SetImage		( SPTexture** ppImage ) = 0;

	virtual LRESULT	GetMemberValue	( MODEL_VALUE_TYPE mvMsg, LPARAM lParam = 0 );

	void			SetFXModelID	( UINT uiFXModelID );
	UINT			GetFXModelID	();
	
protected:		
	
	UINT							m_ID;
	float							m_fLifeTime;
	int								m_iParentFollow;
	TEXTURE_EFFECT					m_eTextureEffect;	// AJJIYA [12/1/2005]
	float							m_fVersion;			// 모델의 버전 확인용 [10/12/2006 AJJIYA]
	SPID_FX_MODEL					m_eModelType;		// 모델 타입 확인 [11/8/2006 AJJIYA]
	int								m_iRenderLayer;		// 렌더 레이더
	
};
