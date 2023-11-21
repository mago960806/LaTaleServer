
#include <vector>
#include "SPCommon.h"
#include "SPUtil.h"

#include "SPResourceDef.h"

#include "SPMainGameDEF.H"

#include "SPGameObjectDEF.h"
#include "SPManager.h"

#ifndef _MAP_TOOL
#include "SPSubGameManager.h"
#endif

#include "SPRenderModelDEF.h"
#include "SPRenderModel.h"

#include "SPTerrainDEF.h"
#include "SPTerrainModel.h"
#include "SPTerrainModelStatic.h"
#include "SPTerrainModelFrameAnimation.h"
#include "SPTerrainModelCircularMotion.h"

#ifndef _MAP_TOOL
#include "SPGOBClusterDef.h"
#include "SPGOBManager.h"
#include "SPMainGameManager.h"
#endif

#include "SPCameraManager.h"
#include "SPTerrainCluster.h"
#include "SPTerrainManager.h"

#ifndef _MAP_TOOL
#include "SPInputManager.h"
#endif

#include "SPTerrainUnit.h"

#include "SPZone.h"


SPTerrainUnit::SPTerrainUnit(SPTerrainModel** ppTerrainModel, float fX, float fY, int iArg0, int iArg1, int iArg2, int iArg3,
							 float fScaleX /*= 1.0f*/,
							 float fScaleY /*= 1.0f*/ )
{
	Init();

	m_pTerrainModel = *ppTerrainModel;
	m_fPosX = fX;
	m_fPosY = fY;

	m_ClassID		= (MODEL_CLASS_ID)		m_pTerrainModel->GetMemberValue(MV_CLASSID			);
	//m_iModelID		= (int)					m_pTerrainModel->GetMemberValue(MV_INSTANCEID		);
	m_iModelID		= *(INT64*)					m_pTerrainModel->GetMemberValue(MV_INSTANCEID		);
	m_fModelDelay	= *(float*)				m_pTerrainModel->GetMemberValue(MV_NEXTANIMATION	);
	m_iNoFrame		= (int)					m_pTerrainModel->GetMemberValue(MV_GET_MAXFRAME		);
	m_ppCurImage	= (SPTexture**)			m_pTerrainModel->GetMemberValue(MV_GETINITIMAGE		);
	m_iCurAlpha		= (int)					m_pTerrainModel->GetMemberValue(MV_GET_ALPHA		);		

	if(m_ClassID == TERRAIN_MODEL_CLASS_FRAME_ANIMATION) {	
		m_fMinStart		= *(float*)m_pTerrainModel->GetMemberValue(MV_GET_MINSTART);
		m_fMaxStart		= *(float*)m_pTerrainModel->GetMemberValue(MV_GET_MAXSTART);
	}


	m_fCurDelay = m_fModelDelay;

	m_iArg0 = iArg0;
	m_iArg1	= iArg1;
	m_iArg2	= iArg2;
	m_iArg3	= iArg3;
	
	m_fScaleX = fScaleX;
	m_fScaleY = fScaleY;
}

SPTerrainUnit::~SPTerrainUnit() {}

void SPTerrainUnit::SetNull() 
{
	m_fPosX = m_fPosY = 0.f;	
	m_fAccumulateRender = m_fCurRotateDegree = 0.0f;

	m_iModelID			= 0;
	m_ClassID			= MODEL_CLASS_NULL;
	m_pTerrainModel		= NULL;
	m_iNoFrame			= 0;
	m_iCurFrame			= 0;
	m_fCurDelay			= 0.0f;
	m_fModelDelay		= 0.0f;
	m_ppCurImage		= NULL;	
	m_fCurRotateDegree	= 0.0f;

	m_fCamX				= 0;
	m_fCamY				= 0;

	m_iArg0				= 
	m_iArg1				= 
	m_iArg2				= 
	m_iArg3				= 0;

	m_fMinStart			= 
	m_fMaxStart			= 0.0f;

	m_fScaleX			= 1.0f;
	m_fScaleY			= 1.0f;

	m_bRenderCancel		= true;		// jinhee - [12/2/2004]
}

void SPTerrainUnit::Init()
{
	SetNull();
}

void SPTerrainUnit::GetAbsCoord(int& ix, int& iy)
{
	ix = (int)m_fPosX;
	iy = (int)m_fPosY;
}

//int	SPTerrainUnit::GetModelID(){
INT64	SPTerrainUnit::GetModelID(){
	return m_iModelID;
}

float SPTerrainUnit::GetPosX(){
	return m_fPosX;
}

float SPTerrainUnit::GetPosY(){
	return m_fPosY;
}

void SPTerrainUnit::SetPosX(float fPosX /*= 0.0f*/){
	m_fPosX = fPosX;
}

void SPTerrainUnit::SetPosY(float fPosY /*= 0.0f*/){
	m_fPosY = fPosY;
}

float SPTerrainUnit::GetWidth(){
	if(*m_ppCurImage == NULL) 
		return -1.0f;

	return (*m_ppCurImage)->GetLenX();
}

float SPTerrainUnit::GetHeight(){
	if(*m_ppCurImage == NULL) 
		return -1.0f;

	return (*m_ppCurImage)->GetLenY();
}

void SPTerrainUnit::SetScale(float fScaleX /*= 1.0f*/, float fScaleY /*= 1.0f*/){
	m_fScaleX = fScaleX;
	m_fScaleY = fScaleY;
}

float SPTerrainUnit::GetScaleX(){
	return m_fScaleX;
}

float SPTerrainUnit::GetScaleY(){
	return m_fScaleY;
}


SPTerrainModel* SPTerrainUnit::GetTerrainModel()
{
	return m_pTerrainModel;
}


MODEL_CLASS_ID	SPTerrainUnit::GetClassID()
{
	return m_ClassID;
}


void SPTerrainUnit::Process(float fTime)
{
	m_fAccumulateRender += fTime;
	if( m_fAccumulateRender > 6.0f )
		m_fAccumulateRender = 6.0f;
	
#ifdef _MAP_TOOL
	m_fCamX = g_pMainGameManager->GetCameraStruct()->fOffsetX;
	m_fCamY = g_pMainGameManager->GetCameraStruct()->fOffsetY;
#else 
	m_fCamX = g_pCameraManagr->GetCameraStruct()->fOffsetX;
	m_fCamY = g_pCameraManagr->GetCameraStruct()->fOffsetY;	
#endif

	int iScreenX = g_pVideo->GetScreenLenX();
	int iScreenY = g_pVideo->GetScreenLenY();	
	
	//[xialin 2008/08/06]add >>>
	if ( g_pSubGameManager->GetGameState() == GS_CASHSHOP )
	{
		m_fCamX += CASHSHOP_OFFSETCAMX;
		m_fCamY += CASHSHOP_OFFSETCAMY;
	}
	//[xialin 2008/08/06]add <<<

	if(TERRAIN_MODEL_CLASS_CIRCULAR_MOTION == m_ClassID){
		//시계바늘의 경우 중점이 가운데가 아니기 때문에 width의 + - 위치를 모두 계산해주어야 한다.
		//우선은 회전인 경우 모두 랜더 하고 추후 회전으로 인한 성능 감소시 
		//위 컨셉으로 수정할것
		m_bRenderCancel = (m_fCamX > m_fPosX + (GetWidth() * GetScaleX()) || m_fPosX - (GetWidth() * GetScaleX()) > (m_fCamX + iScreenX)) ||
			(m_fCamY > m_fPosY + (GetHeight() * GetScaleY()) || m_fPosY - (GetHeight() * GetScaleY()) > (m_fCamY + iScreenY)) ;			
	}
	else if(TERRAIN_MODEL_CLASS_FRAME_ANIMATION == m_ClassID) {		//[2005/10/27]			
		//[2006/3/27]
		if(GetScaleX() <= -0.01f) {			//X 축 역상인 경우
			m_bRenderCancel = (m_fCamX > m_fPosX - (GetWidth() * GetScaleX()) + (GetWidth() * GetScaleX()) || m_fPosX + (GetWidth() * GetScaleX()) > (m_fCamX + iScreenX)) ||
				(m_fCamY > m_fPosY + (GetHeight() * GetScaleY()) || m_fPosY > (m_fCamY + iScreenY)) ;
		}
		else {	
			m_bRenderCancel = ( (m_fCamX - 32)  > m_fPosX + (GetWidth() * GetScaleX()) || m_fPosX > (m_fCamX + iScreenX + 32)) ||
				(m_fCamY - 32 > m_fPosY + (GetHeight() * GetScaleY()) || m_fPosY > (m_fCamY + iScreenY +32)) ;
		}
		
		//m_bRenderCancel = ( (m_fCamX - 32)  > m_fPosX + (GetWidth() * GetScaleX()) || m_fPosX > (m_fCamX + 832)) ||
		//	(m_fCamY - 32 > m_fPosY + (GetHeight() * GetScaleY()) || m_fPosY > (m_fCamY + 632)) ;
	}
	else{
		//if(GetScaleX() <= -1.0) {
		if(GetScaleX() <= -0.01f) {			//X 축 역상인 경우
			m_bRenderCancel = (m_fCamX > m_fPosX - (GetWidth() * GetScaleX()) + (GetWidth() * GetScaleX()) || m_fPosX + (GetWidth() * GetScaleX()) > (m_fCamX + iScreenX)) ||
				(m_fCamY > m_fPosY + (GetHeight() * GetScaleY()) || m_fPosY > (m_fCamY + iScreenY)) ;
		}
		else {
			m_bRenderCancel = (m_fCamX  > m_fPosX + (GetWidth() * GetScaleX()) || m_fPosX > (m_fCamX + iScreenX)) ||
				(m_fCamY > m_fPosY + (GetHeight() * GetScaleY()) || m_fPosY > (m_fCamY + iScreenY)) ;
		}
		
		//m_bRenderCancel = (m_fCamX > m_fPosX + (GetWidth() * GetScaleX()) || m_fPosX > (m_fCamX + 800)) ||
		//	(m_fCamY > m_fPosY + (GetHeight() * GetScaleY()) || m_fPosY > (m_fCamY + 600)) ;
	}		

	if(m_bRenderCancel && TERRAIN_MODEL_CLASS_STATIC == m_ClassID)
		return;	

	switch(m_ClassID) {
	case MODEL_CLASS_NULL:
	case TERRAIN_MODEL_CLASS_STATIC:	
		return;

	case TERRAIN_MODEL_CLASS_FRAME_ANIMATION:
		{
			// jinhee - [12/2/2004]
			if( m_fAccumulateRender > m_fCurDelay) {
				m_fAccumulateRender -= m_fCurDelay;				

				m_ppCurImage	= (SPTexture**)	m_pTerrainModel->GetMemberValue(MV_GETIMAGE, m_iCurFrame);
				m_iCurAlpha		= (int)			m_pTerrainModel->GetMemberValue(MV_GET_ALPHA, m_iCurFrame);

				//m_fCurDelay = 0.1f;
				++m_iCurFrame;
				if(m_iCurFrame >= m_iNoFrame) {
					m_iCurFrame = 0;
					if(m_fMaxStart == 0 ) {
						m_fCurDelay = *(float*)m_pTerrainModel->GetMemberValue(MV_NEXTANIMATION);
					} else {	
						if(m_fMaxStart == m_fMinStart ) {
							m_fCurDelay = m_fMaxStart;
						} else {						
							int iRandVal = rand() % (int)(m_fMaxStart*100) + (m_fMinStart*100);
							m_fCurDelay = (float)iRandVal/100;
						}
					}
				} 				
				else {
					m_fCurDelay = *(float*)m_pTerrainModel->GetMemberValue(MV_NEXTANIMATION);
				}
			}
			
		}
		break;

	case TERRAIN_MODEL_CLASS_CIRCULAR_MOTION:
		{
			if( m_fAccumulateRender > m_fCurDelay) {
				m_fAccumulateRender -= m_fCurDelay;

				m_fCurRotateDegree += (*(float*) m_pTerrainModel->GetMemberValue(MV_GET_RADIANDEGREE) * D3DX_PI);
				if(m_fCurRotateDegree > (2.0f*D3DX_PI)) {
					m_fCurRotateDegree -= (2.0f*D3DX_PI);
				}
			}
		}
		break;
	}
	
}

void SPTerrainUnit::Render(float fTime, float fAlpha /*= 1.0f*/)
{
	if(*m_ppCurImage == NULL) 
		return;	
	
	if(m_bRenderCancel)
		return;

	BOOL BLightMap = m_pTerrainModel->GetMemberValue(MV_GET_LIGHTMAP);

	if(BLightMap) {
		g_pVideo->SetAlphaTexture(TRUE);
	}

	(*m_ppCurImage)->SetScale(m_fScaleX, m_fScaleY);	//jinhee - [12/7/2004]

	switch(m_ClassID) {
	case TERRAIN_MODEL_CLASS_STATIC:
		{
			if(0 == (*m_ppCurImage))
				return ;
			
			(*m_ppCurImage)->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha )); //jinhee - 04.11.02
			(*m_ppCurImage)->RenderTexture( (int)m_fPosX - (int)m_fCamX, (int)m_fPosY - (int)m_fCamY );
			//(*m_ppCurImage)->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f ));	//jinhee - 04.11.02x
		}
		break;

	case TERRAIN_MODEL_CLASS_FRAME_ANIMATION:
		{
			float fAlpha = ((float)m_iCurAlpha)/255.f;
			(*m_ppCurImage)->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha ));
			(*m_ppCurImage)->RenderTexture((int)m_fPosX - (int)m_fCamX, (int)m_fPosY - (int)m_fCamY);
			//(*m_ppCurImage)->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f ));	//jinhee - 04.11.02x
		}
		break;

	case TERRAIN_MODEL_CLASS_CIRCULAR_MOTION:
		{
			RECT	Src, Dest;
			Src.left = 0;
			Src.top = 0;
			Src.right = (*m_ppCurImage)->GetLenX();
			Src.bottom = (*m_ppCurImage)->GetLenY();

			Dest.left = (int)m_fPosX - (int)m_fCamX;
			Dest.top = (int)m_fPosY - (int)m_fCamY;
			Dest.right = Dest.left + (*m_ppCurImage)->GetLenX();
			Dest.bottom = Dest.top + (*m_ppCurImage)->GetLenY();

			(*m_ppCurImage)->SetColor( D3DXCOLOR(((float)m_iArg0)/255, ((float)m_iArg1)/255, ((float)m_iArg2)/255, ((float)m_iArg3)/255));	
			(*m_ppCurImage)->RenderTexture(&Dest, NULL, m_fCurRotateDegree );
			//(*m_ppCurImage)->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f ));	//jinhee - 04.11.02x

		}
		break;
	}

	(*m_ppCurImage)->SetScale(1.0f, 1.0f);		//jinhee - [12/7/2004]

	(*m_ppCurImage)->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f ));	//[2005/11/2]

	if(BLightMap) {
		g_pVideo->SetAlphaTexture(FALSE);
	}


}





