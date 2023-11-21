// ***************************************************************
//  SPFXModel   version:  1.0   ·  date: 11/08/2006
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2006 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

//
//	2005. 4. 25.	AJJIYA
//	캐릭터 툴간 호환을 위한 헤더화일 정리
//	Start
#ifdef _LATALE_CLIENT
#include "SPCommon.h"	
#include "SPManager.h"
#else 
#include "SPToolCommon.h"
#include "SPGameMessageDEF.H"
#endif
//	End

#include "SPResourceDef.h"

#include "SPGameObjectDEF.h"

#include "SPRenderModelDEF.h"
#include "SPRenderModel.h"

#include "SPFXDEF.h"
#include "SPRenderModel.h"
#include "SPFXModel.h"


SPFXModel::SPFXModel()
: SPRenderModel()
{
	Init();
}

SPFXModel::~SPFXModel()
{
}

void SPFXModel::Init()
{	
	SetNull();
}

void SPFXModel::SetNull()
{	
	SPRenderModel::SetNull();

	m_ID				=	0;
	m_fLifeTime			=	0.0f;
	m_iParentFollow		=	FX_FOLLOW_NULL;
	m_eTextureEffect	=	TEXTURE_EFFECT_NULL;
	m_fVersion			=	0.0f;
	m_eModelType		=	IDFXM_NULL;
	m_iRenderLayer		=	0;
}

void SPFXModel::Destroy()
{
}

void SPFXModel::SetFXModelID( UINT uiFXModelID )
{
	m_ID	=	uiFXModelID;
}

UINT SPFXModel::GetFXModelID()
{
	return m_ID;
}

LRESULT SPFXModel::GetMemberValue(MODEL_VALUE_TYPE mvMsg, LPARAM lParam)
{
	switch(mvMsg) {
	case MV_GET_FXLIFETIME:
		return (LRESULT)&m_fLifeTime;
	case MV_SET_FXLIFETIME:
		m_fLifeTime = *(float*)lParam;
		break;
	case MV_GET_FXFOLLOW_PARENT:
		return (LRESULT)m_iParentFollow;
	case MV_SET_FXFOLLOW_PARENT:
		m_iParentFollow = (int)lParam;
		break;
	case MV_GET_TEXTURE_EFFECT:
		return (LRESULT)m_eTextureEffect;
	case MV_SET_TEXTURE_EFFECT:
		m_eTextureEffect = (TEXTURE_EFFECT)lParam;
		break;
	case MV_GET_FXMODEL_VERSION:
		return (LRESULT)&m_fVersion;
	case MV_SET_FXMODEL_VERSION:
		m_fVersion = *(float*)lParam;
		break;
	case MV_GET_FXMODEL_TYPE:
		return (LRESULT)m_eModelType;
	case MV_SET_FXMODEL_TYPE:
		m_eModelType = (SPID_FX_MODEL)lParam;
		break;
	case MV_GET_FX_RENDER_LAYER:
		return (LRESULT)m_iRenderLayer;
	case MV_SET_FX_RENDER_LAYER:
		m_iRenderLayer = (int)lParam;
		break;
	}

	return SPRenderModel::GetMemberValue(mvMsg, lParam);
}

