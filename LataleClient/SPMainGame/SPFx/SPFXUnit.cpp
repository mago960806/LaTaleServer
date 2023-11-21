// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-8-31    16:02 
//***************************************************************************

#include "SPCommon.h"
#include "SPUtil.h"

#include "SPGameObject.h"
#include "SPGOBManager.h"

#include "SPRenderModelDEF.h"
#include "SPRenderModel.h"
#include "SPFXDEF.h"
#include "SPFXModel.h"

#include "SPDebug.h"

#include "SPFXUnit.h"

const float FX_Y_ADJUST = 70.0f;	// 캐릭터 키(Height) 때문에 올림.


SPFXUnit::SPFXUnit()
{
	SetNull();
}

SPFXUnit::~SPFXUnit()
{
}

void SPFXUnit::SetNull() 
{
	m_fPosX					=	0.0f;
	m_fPosY					=	0.0f;

	m_fCamX					=	0.0f;
	m_fCamY					=	0.0f;

	m_fParentPosX			=	0.0f;
	m_fParentPosY			=	0.0f;

	m_fAccumulateRender		=	0.0f;
	m_fAccumulateProcess	=	0.0f;

	m_fCurRotateDegree		=	0.0f;
	m_fModelDelay			=	0.0f;
	m_fCurDelay				=	0.0f;
	m_RGBA					=	D3DCOLOR_ARGB( 255 , 255 , 255 , 255 );
	m_eTextureEffect		=	TEXTURE_EFFECT_NULL;
	m_RectSrc.left			=	0;
	m_RectSrc.top			=	0;
	m_RectSrc.right			=	0;
	m_RectSrc.bottom		=	0;
	m_RectAdjust.left		=	0;
	m_RectAdjust.top		=	0;
	m_RectAdjust.right		=	0;
	m_RectAdjust.bottom		=	0;
	m_fScaleX				=	0.0f;
	m_fScaleY				=	0.0f;
	m_bLRSwap				=	FALSE;

	m_iModelID				=	0;
	m_ClassID				=	MODEL_CLASS_NULL;

	m_FXParam				=	NULL;
	m_FXCurUseParam			=	NULL;

	m_pFXModel				=	NULL;
	m_ppCurImage			=	NULL;

	m_FUState				=	FS_INIT;
	m_iFollowParent			=	0;
	m_tGOBID				=	0;

	m_fFXMLifeTime			=	0.0f;
	m_bLoop					=	FALSE;
	m_bContinue				=	FALSE;
	m_fUnitLifeTime			=	0.0f;
	m_fUnitVelocity			=	0.0f;
	m_fDestPosX				=	0.0f;
	m_fDestPosY				=	0.0f;
	m_bScreenRender			=	FALSE;
}

void SPFXUnit::Init()
{
	m_FUState		= FS_INIT;

	if( m_pFXModel == NULL )
	{
		m_FUState = FS_RELEASE;
		return;
	}

	m_ClassID		= (MODEL_CLASS_ID)	m_pFXModel->GetMemberValue( MV_CLASSID );
	m_iModelID		= (int)				m_pFXModel->GetMemberValue( MV_INSTANCEID );
	m_ppCurImage	= (SPTexture**)		m_pFXModel->GetMemberValue( MV_GETIMAGE ); // 텍스쳐한장으로 하기로 바꿨다.
	m_eTextureEffect= (TEXTURE_EFFECT)	m_pFXModel->GetMemberValue( MV_GET_TEXTURE_EFFECT );
	m_fFXMLifeTime	= *(float*)			m_pFXModel->GetMemberValue( MV_GET_FXLIFETIME );
	m_iFollowParent = (int)				m_pFXModel->GetMemberValue( MV_GET_FXFOLLOW_PARENT );

	if( ( m_iFollowParent & FX_FOLLOW_LOOP ) == FX_FOLLOW_LOOP )
		m_bLoop	=	TRUE;

	if( ( m_iFollowParent & FX_FOLLOW_SCREENRENDER ) == FX_FOLLOW_SCREENRENDER )
		m_bScreenRender	=	TRUE;

	m_fCurDelay		=	m_fModelDelay;
	m_fUnitLifeTime	=	m_fFXMLifeTime;
	m_fDestPosX		=	m_fPosX;
	m_fDestPosY		=	m_fPosY;
	m_FUState		=	FS_ENABLE;
}

void SPFXUnit::Destory()
{
	SetNull();
	m_FUState	=	FS_RELEASE;
}

bool SPFXUnit::Process( float fTime )
{
	if(	m_FUState == FS_RELEASE || m_FUState == FS_INIT )
		return false;

	ProcessPos( fTime );

	m_fAccumulateRender		+= fTime;
	m_fAccumulateProcess	+= fTime;

	if( m_fAccumulateProcess >= m_fUnitLifeTime )
	{
		if( m_bContinue == FALSE && m_bLoop == FALSE )
		{
			m_FUState = FS_RELEASE;
			return false;
		}
	}

	SPGameObject*	pGOB	=	NULL;

	if( m_tGOBID > 0 )
		pGOB	=	g_pGOBManager->Find( m_tGOBID );

	if( m_fPosX == m_fDestPosX && m_fPosY == m_fDestPosY )
	{
		if( ( pGOB == NULL && m_tGOBID > 0 ) || m_fUnitVelocity != 0.0f )
		{
			m_FUState = FS_RELEASE;
			return false;
		}
	}

	if( m_fAccumulateProcess >= m_fFXMLifeTime )
	{
		if( m_bLoop == TRUE || m_bContinue == TRUE )
		{
// 망치질에서 캐릭터 애니메이션이랑 동기화가 안맞아서 임시로 렌더쪽 초기화 하지 않는다. 루프일때 AJJIYA [5/18/2006]
//			m_fAccumulateRender		= 0.0f;
			m_fAccumulateProcess	= 0.0f;
		}
		else
		{
			m_FUState = FS_RELEASE;
			return false;
		}
	}

	if( m_iFollowParent & FX_FOLLOW_PARENT )
	{
		m_fPosX = m_fParentPosX;
		m_fPosY = m_fParentPosY;
	}

	if( m_fAccumulateRender < m_fCurDelay )
		return false;

	m_fAccumulateRender -= m_fCurDelay;

	//switch(m_ClassID)
	//{
	//case FX_MODEL_CLASS_FRAME_ANIMATION:
	//	ProcessFrameAnimation();
	//	break;
	//case FX_MODEL_CLASS_PARTICLE:
	//	ProcessParticle();
	//	break;
	//}

	return true;
}

bool SPFXUnit::Render( float fTime )
{
	if( m_FUState == FS_RELEASE || *m_ppCurImage == NULL )
		return false;

	if( m_RGBA.a == 0.0f )
		return false;

	m_fCamX = g_pCameraManagr->GetCameraStruct()->fOffsetX;
	m_fCamY = g_pCameraManagr->GetCameraStruct()->fOffsetY;

	if( m_eTextureEffect != TEXTURE_EFFECT_NULL )
		g_pVideo->SetTextureEffect( m_eTextureEffect );

	g_pVideo->Flush();

	(*m_ppCurImage)->SetRotation( m_fCurRotateDegree );
	(*m_ppCurImage)->SetColor( m_RGBA );
	(*m_ppCurImage)->SetScale( m_fScaleX , m_fScaleY );
	(*m_ppCurImage)->SetSrcRect( m_RectSrc );

	return true;
}

void SPFXUnit::RenderReset( float fTime )
{
	if( m_eTextureEffect != TEXTURE_EFFECT_NULL )
		g_pVideo->SetTextureEffect( TEXTURE_EFFECT_NULL );

	g_pVideo->Flush();
}

void SPFXUnit::SetFXModel( SPFXModel* pFXModel )
{
	m_pFXModel	=	pFXModel;
}

SPFXModel* SPFXUnit::GetFXModel()
{
	return m_pFXModel;
}

void SPFXUnit::SetPosX( float fPosX )
{
	m_fPosX	=	fPosX;
}

float SPFXUnit::GetPosX()
{
	return m_fPosX;
}

void SPFXUnit::SetPosY( float fPosY )
{
	m_fPosY	=	fPosY;
}

float SPFXUnit::GetPosY()
{
	return m_fPosY;
}

void SPFXUnit::SetLParam( LPARAM lParam )
{
	m_FXCurUseParam = m_FXParam = (FXPARAM)lParam;
}

void SPFXUnit::SetGOBID( GU_ID tGOBID )
{
	m_tGOBID	=	tGOBID;
}

GU_ID SPFXUnit::GetGOBID()
{
	return m_tGOBID;
}

FXUNIT_STATE SPFXUnit::GetState() 
{ 
	return m_FUState;	
}

void SPFXUnit::SetParentPos( float fPosX , float fPosY )
{
	m_fParentPosX	=	fPosX;
	m_fParentPosY	=	fPosY;
}

void SPFXUnit::SetParentFacing( FACING eFacing )
{
	m_eParentFacing	=	eFacing;
}

void SPFXUnit::SetLoop( BOOL bLoop )
{
	m_bLoop	=	bLoop;
}

BOOL SPFXUnit::GetLoop()
{
	return m_bLoop;
}

void SPFXUnit::SetContinue( BOOL bContinue )
{
	m_bContinue	=	bContinue;

	SetLoop( bContinue );
}

BOOL SPFXUnit::GetContinue()
{
	return m_bContinue;
}

UINT SPFXUnit::GetFXID()
{
	return m_pFXModel->GetFXModelID();
}

void SPFXUnit::SetLifeTime( float fLifeTime )
{
	m_fUnitLifeTime	=	fLifeTime;
}

float SPFXUnit::GetLifeTime()
{
	return m_fUnitLifeTime;
}

void SPFXUnit::SetVelocity( float fVelocity )
{
	m_fUnitVelocity	=	fVelocity;
}

float SPFXUnit::GetVelocity()
{
	return m_fUnitVelocity;
}

void SPFXUnit::SetDestPosX( float fPos )
{
	m_fDestPosX	=	fPos;
}

float SPFXUnit::GetDestPosX()
{
	return m_fDestPosX;
}

void SPFXUnit::SetDestPosY( float fPos )
{
	m_fDestPosY	=	fPos;
}

float SPFXUnit::GetDestPosY()
{
	return m_fDestPosY;
}

void SPFXUnit::SetFollowParent( BOOL bFollow )
{
	if( bFollow == TRUE )
		m_iFollowParent	=	FX_FOLLOW_BOTH;
	else
		m_iFollowParent	=	0;
}

void SPFXUnit::SetFollow( int iFollow )
{
	m_iFollowParent	=	iFollow;
}

int SPFXUnit::GetFollow()
{
	return m_iFollowParent;
}

//void SPFXUnit::InitParticleInstance(SPFXParticleInstance& ParticleInstance)
//{
//	SPGameObject*	pGOB	=	g_pGOBManager->Find( m_tGOBID );
//
//	if( m_iFollowParent & FX_FOLLOW_FACINGPARENT && pGOB != NULL )
//	{
//		FACING Dir = (FACING)pGOB->SPGOBPerformMessage( SPGM_GETFACING );
//		if(Dir == FACING_LEFT)
//		{
//			m_FXCurUseParam = FXPARAM_FACING_LEFT;
//		} else if( Dir == FACING_RIGHT )
//		{
//			m_FXCurUseParam = FXPARAM_FACING_RIGHT;
//		}
//	} else {
//		switch(m_FXParam)
//		{
//		case FACING_LEFT:
//			m_FXCurUseParam = FXPARAM_FACING_LEFT;
//			break;
//		case FACING_RIGHT:
//			m_FXCurUseParam = FXPARAM_FACING_RIGHT;
//			break;		
//		}
//	}
//
//	switch(m_ParticleMove)
//	{
//	case PARTICLE_SPREAD_BACK:
//		{
//			ParticleInstance.vec2Pos.x = (float)( (rand() % (m_RectAdjust.right/4)*2) - m_RectAdjust.right / 4 );
//			ParticleInstance.vec2Pos.y = (float)( (rand() % (m_RectAdjust.bottom/4)*2) - m_RectAdjust.bottom / 4 );
//
//			switch(m_FXCurUseParam)
//			{
//			case FXPARAM_FACING_LEFT:
//				{
//					ParticleInstance.vec2Pos.x = -abs(ParticleInstance.vec2Pos.x);
//				}
//				break;
//			case FXPARAM_FACING_RIGHT:
//				{
//					ParticleInstance.vec2Pos.x = +abs(ParticleInstance.vec2Pos.x);
//				}
//				break;
//			}
//			D3DXVec2Normalize( &ParticleInstance.vec2Velocity, &ParticleInstance.vec2Pos);
//			ParticleInstance.vec2Velocity *= 2.5f;
//		}
//		break;
//	case PARTICLE_SPREAD:
//		{
//			ParticleInstance.vec2Pos.x = (float)( (rand() % (m_RectAdjust.right/4)*2) - m_RectAdjust.right/4 );
//			ParticleInstance.vec2Pos.y = (float)( (rand() % (m_RectAdjust.bottom/4)*2) - m_RectAdjust.bottom/4 );
//
//			D3DXVec2Normalize( &ParticleInstance.vec2Velocity, &ParticleInstance.vec2Pos);
//			ParticleInstance.vec2Velocity *= 2.5f;
//		}
//		break;
//
//	case PARTICLE_CHARGE:
//		{
//			ParticleInstance.vec2Pos.x = (float)( (rand() % (m_RectAdjust.right*2)) - m_RectAdjust.right );
//			ParticleInstance.vec2Pos.y = (float)( (rand() % (m_RectAdjust.bottom*2)) - m_RectAdjust.bottom );
//
//			D3DXVec2Normalize( &ParticleInstance.vec2Velocity, &ParticleInstance.vec2Pos);
//			ParticleInstance.vec2Velocity *= -2.5f;
//		}
//		break;
//
//	case PARTICLE_DROP:
//		{
//		}
//		break;
//	}
//
//	// TODO: Initialize tempInstance.vec2Accel
//	// 가속도
//}

void SPFXUnit::ProcessPos( float fTime )
{
	float	fPosX	=	m_fPosX	-	m_fDestPosX;

	SPGameObject*	pGOB	=	NULL;

	if( m_tGOBID > 0 )
		pGOB	=	g_pGOBManager->Find( m_tGOBID );

	if( fPosX == 0.0f )
	{
		if( pGOB != NULL )
			m_fDestPosX	=	(float)pGOB->SPGOBPerformMessage( SPGM_GETPOSX );
		else if( m_tGOBID <= 0 )
			m_fDestPosX	=	m_fParentPosX;
	}
	else if( fPosX > 0.0f )
	{
		if( m_fUnitVelocity != 0.0f )
			m_FXCurUseParam	=	FXPARAM_FACING_LEFT;

		m_fPosX	-=	m_fUnitVelocity * fTime;

		if( m_fPosX < m_fDestPosX )
			m_fPosX = m_fDestPosX;
	}
	else if( fPosX < 0.0f )
	{
		if( m_fUnitVelocity != 0.0f )
			m_FXCurUseParam	=	FXPARAM_FACING_RIGHT;

		m_fPosX	+=	m_fUnitVelocity * fTime;

		if( m_fPosX > m_fDestPosX )
			m_fPosX	= m_fDestPosX;
	}

	if( m_iFollowParent & FX_FOLLOW_FACINGPARENT )
	{
		if( m_FXCurUseParam == FXPARAM_FACING_LEFT )
			m_RectAdjust.left = -m_RectAdjust.left;

		FACING Dir	=	m_eParentFacing;

		if( pGOB != NULL )
			Dir = (FACING)pGOB->SPGOBPerformMessage( SPGM_GETFACING );

		if(Dir == FACING_LEFT)
			m_FXCurUseParam = FXPARAM_FACING_LEFT;
		else if( Dir == FACING_RIGHT )
			m_FXCurUseParam = FXPARAM_FACING_RIGHT;

		if( m_FXCurUseParam == FXPARAM_FACING_LEFT )
			m_RectAdjust.left = -m_RectAdjust.left;
	}

	float	fPosY	=	m_fPosY	-	m_fDestPosY;

	if( fPosY == 0.0f )
	{
		if( pGOB != NULL )
			m_fDestPosY	=	(float)pGOB->SPGOBPerformMessage( SPGM_GETPOSY );
		else if( m_tGOBID <= 0 )
			m_fDestPosY	=	m_fParentPosY;
	}
	else if( fPosY > 0.0f )
	{
		m_fPosY	-=	m_fUnitVelocity * fTime;

		if( m_fPosY < m_fDestPosY )
			m_fPosY = m_fDestPosY;
	}
	else if( fPosY < 0.0f )
	{
		m_fPosY	+=	m_fUnitVelocity * fTime;

		if( m_fPosY > m_fDestPosY )
			m_fPosY	= m_fDestPosY;
	}
}

bool SPFXUnit::IsClipping( int iPosX , int iPosY , int iSrcRectSizeX , int iSrcRectSizeY , float fScaleX , float fScaleY )
{
	POINT	ptClippingOffset;

	ptClippingOffset.x	=	(int)( (float)iSrcRectSizeX * fScaleX * 0.5f );
	ptClippingOffset.y	=	(int)( (float)iSrcRectSizeY * fScaleY * 0.5f );

	RECT	rcClipping;

	rcClipping.left				=	iPosX	-	ptClippingOffset.x;
	rcClipping.top				=	iPosY	-	ptClippingOffset.y;
	rcClipping.right			=	iPosX	+	ptClippingOffset.x;
	rcClipping.bottom			=	iPosY	+	ptClippingOffset.y;

	int		iVideoSizeX			=	g_pVideo->GetScreenLenX();
	int		iVideoSizeY			=	g_pVideo->GetScreenLenY();

	if( ( rcClipping.left < 0 && rcClipping.right < 0 ) || ( rcClipping.left > iVideoSizeX && rcClipping.right > iVideoSizeX ) ||
		( rcClipping.top < 0 && rcClipping.bottom < 0 ) || ( rcClipping.top > iVideoSizeY && rcClipping.bottom > iVideoSizeY ) )
	{
		return true;
	}

	return false;
}

//void SPFXUnit::ProcessFrameAnimation()
//{
//	SPFXFrameData* pFrameData = (SPFXFrameData*)m_pFXModel->GetMemberValue( MV_GET_FRAMEDATA , m_iCurFrame );
//
//	if(pFrameData)
//	{
//		m_RGBA				= pFrameData->FrameColor;
//		m_fCurDelay			= pFrameData->fFrameTime;
//		m_RectSrc			= pFrameData->RectSrc;
//		m_fScaleX			= pFrameData->fFrameScaleX;
//		m_fScaleY			= pFrameData->fFrameScaleY;
//		m_fCurRotateDegree	= pFrameData->fFrameDegree;
//		m_RectAdjust		= pFrameData->RectAdjust;
//		m_bLRSwap			= pFrameData->bLRSwap;
//
//		if( m_iFollowParent == 0 )
//			m_FXCurUseParam	= m_FXParam;
//
//		if( m_FXCurUseParam == FXPARAM_NULL )
//			m_FXCurUseParam = m_FXParam;
//
//		if( m_FXCurUseParam == FXPARAM_FACING_LEFT )
//			m_RectAdjust.left = -m_RectAdjust.left;
//	
//	} else {
//		assert(0 && "SPFXUnit - FrameData not found");
//	}
//
//	if( ++m_iCurFrame >= m_iNoFrame )
//	{
//		if( m_bLoop == TRUE )
//		{
//			m_iCurFrame = 0;
//
//			SPGameObject*	pGOB	=	g_pGOBManager->Find( m_tGOBID );
//
//			if( pGOB != NULL )
//			{
//				int iPosX	=	pGOB->SPGOBPerformMessage( SPGM_GETPOSX );
//				int iPosY	=	pGOB->SPGOBPerformMessage( SPGM_GETPOSY );
//				m_fPosX	=	static_cast<float>( iPosX );
//				m_fPosY	=	static_cast<float>( iPosY );
//			}
//		}
//		else
//			m_iCurFrame = m_iNoFrame - 1;
//	}
//}

//void SPFXUnit::ProcessParticle()
//{
//	m_fCurRotateDegree	+=	*(float*)m_pFXModel->GetMemberValue(MV_GET_RADIANDEGREE);	
//
//	switch(m_ParticleMove) {
//	case PARTICLE_CHARGE:
//		{
//			std::vector<SPFXParticleInstance>::iterator Iter = m_vParticleInstance.begin();
//			for(;Iter != m_vParticleInstance.end(); ++Iter) {
//				(*Iter).vec2Pos += (*Iter).vec2Velocity;
//
//				if((*Iter).vec2Velocity.x < 0.0f && (*Iter).vec2Pos.x < 0.0f)
//					InitParticleInstance(*Iter);
//
//				if((*Iter).vec2Velocity.y < 0.0f && (*Iter).vec2Pos.y < 0.0f)
//					InitParticleInstance(*Iter);
//
//				if((*Iter).vec2Velocity.x > 0.0f && (*Iter).vec2Pos.x > 0.0f)
//					InitParticleInstance(*Iter);
//
//				if((*Iter).vec2Velocity.y > 0.0f && (*Iter).vec2Pos.y > 0.0f)
//					InitParticleInstance(*Iter);
//			}
//		}
//		break;
//
//	case PARTICLE_SPREAD_BACK:
//	case PARTICLE_SPREAD:
//		{
//			std::vector<SPFXParticleInstance>::iterator Iter = m_vParticleInstance.begin();
//			for(;Iter != m_vParticleInstance.end(); ++Iter) {
//				(*Iter).vec2Pos += (*Iter).vec2Velocity;
//				if((*Iter).vec2Pos.x < m_RectAdjust.left || (*Iter).vec2Pos.x > m_RectAdjust.right) {
//					InitParticleInstance(*Iter);
//				}
//				if((*Iter).vec2Pos.y < m_RectAdjust.top || (*Iter).vec2Pos.y > m_RectAdjust.bottom) {
//					InitParticleInstance(*Iter);
//				}
//			}
//		}
//		break;
//
//	case PARTICLE_DROP:
//		{
//		}
//		break;
//
//	}
//}
