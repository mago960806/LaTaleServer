#include "SPCommon.h"
//#include "SPManagerDef.h"
//#include "SPManager.h"
#include "SPResourceDef.h"

#include "SPMainGameDEF.H"
//#include "SPSubGameManager.h"
//#include "SPMainGameManager.h"
//#include "SPMouseCursor.h"

#include "SPGameObjectDEF.h"
//#include "SPManager.h"
//#include "SPGameObjectDEF.h"
#include "SPGameObject.h"

#include "SPRenderModelDEF.h"
#include "SPRenderModel.h"

#include "SPEventDEF.h"
#include "SPEventUnit.h"
#include "SPEventModel.h"

//#include "SPEventManager.h"
//#include "PacketID.h"


SPEventUnit::SPEventUnit()
{
	SetNull();
}


SPEventUnit::SPEventUnit(SPEventModel** ppEventModel, float fX /*= 0.0f*/, float fY /*= 0.0f*/, LPARAM lParam /*= 0*/)
{
	SetNull();

	//m_pParent = NULL;
	//m_FUState = FS_INIT;
	m_pEventModel = *ppEventModel;
	//m_fPosX = fX;
	//m_fPosY = fY;

	//SetLParam(lParam);
	m_ClassID		= (MODEL_CLASS_ID)	m_pEventModel->GetMemberValue(MV_CLASSID		);
	m_iModelID		= (int)				m_pEventModel->GetMemberValue(MV_INSTANCEID	);	
	m_ppCurImage	= (SPTexture**)		m_pEventModel->GetMemberValue(MV_GETIMAGE		); // 텍스쳐한장으로 하기로 바꿨다.	
	m_BLightMap		= m_pEventModel->GetMemberValue(MV_GET_LIGHTMAP);
	//m_iFollowParent = m_pEventModel->GetMemberValue(MV_GET_FXFOLLOW_PARENT);

	switch(m_ClassID) {
	case TERRAIN_MODEL_CLASS_STATIC:
		{		
			//if(0 == (*m_ppCurImage))
			//	return ;
			//(*m_ppCurImage)->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha )); //jinhee - 04.11.02
			//(*m_ppCurImage)->RenderTexture( (int)m_fPosX - (int)m_fCamX, (int)m_fPosY - (int)m_fCamY );
			//(*m_ppCurImage)->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f ));	//jinhee - 04.11.02x		
		}
		break;

	case FX_MODEL_CLASS_FRAME_ANIMATION:
		{
			m_iNoFrame		= (int)m_pEventModel->GetMemberValue(MV_GET_MAXFRAME	);
			m_fModelDelay	= 0.0f;
		}
		break;
	
	////
	//case FX_MODEL_CLASS_PARTICLE:
	//	{
	//		m_fModelDelay		= *(float*)		m_pFXModel->GetMemberValue(MV_NEXTANIMATION	);
	//		m_fCurRotateDegree	= *(float*)		m_pFXModel->GetMemberValue(MV_GET_RADIANDEGREE);
	//		m_RGBA				= *(D3DXCOLOR*)	m_pFXModel->GetMemberValue(MV_GET_COLOR);
	//		m_fScale			= *(float*)		m_pFXModel->GetMemberValue(MV_GET_SCALE);
	//		m_RectSrc			= *(RECT*)		m_pFXModel->GetMemberValue(MV_GET_SRC_RECT); 
	//		m_RectAdjust		= *(RECT*)		m_pFXModel->GetMemberValue(MV_GET_ADJUST);
	//		m_ParticleMove		= (SPFX_PARTICLE_MOVE)m_pFXModel->GetMemberValue(MV_GET_PARTICLE_MOVE);			
	//	
	//		// 최대 그릴수 있는 수
	//		m_iMaxRenderParticle 	= m_pFXModel->GetMemberValue(MV_GET_MAXRENDERPARTICLE);	
	//
	//		// 몇개 까지 찍구 끝낼거냐 0이면 항상 m_iMaxRenderParticle로 찍는다.
	//		m_iMaxParticle			= m_pFXModel->GetMemberValue(MV_GET_MAXPARTICLE);
	//		m_iCurNOParticle		= 0;
	//
	//		//(*m_ppCurImage)->SetSrcRect( m_RectSrc );
	//
	//		SPFXParticleInstance tempInstance;
	//		for(int i=0; i<m_iMaxRenderParticle; ++i) {
	//			InitParticleInstance(tempInstance);
	//			m_vParticleInstance.push_back(tempInstance);
	//		}
	//	}
	//	break;
	default:
		assert(0);
		break;
	}

	m_fCurDelay = m_fModelDelay;
}


SPEventUnit::~SPEventUnit()
{	
	DeleteCondition();
	DeleteItem();
}


bool SPEventUnit::CreateEvent(SPEventUnit& pEvent)
{
	return false;
}


bool SPEventUnit::CreateEvent(int iID, EVENT_TYPE iType, string strName, int iStage, int iGroup, int iMap, 
				 int iPosX, int iPosY, int iLeft, int iTop, int iRight, int iBottom, 
				 string strImage, long lMessage, long lAction)
{	
	m_iEventID = iID;
	m_iType = iType;
	m_strName = strName;
	m_iStageID = iStage;
	m_iMapGroupID = iGroup;
	m_iMapID = iMap;
	m_iPosX = iPosX;
	m_iPosY = iPosY;

	m_iLeft = iLeft;
	m_iTop = iTop;
	m_iRight = iRight;
	m_iBottom = iBottom;

	m_strImage = strImage;
	m_lMessageID = lMessage;
	m_lActionID = lAction;
	
	//그뒤 Condition, Item Condtion 생성

	return true;
}


void SPEventUnit::SetNull() 
{
	m_iEventID = 0;
	m_iType = EVENT_NULL;

	m_strName = "";
	m_iStageID = 0;
	m_iMapGroupID = 0;
	m_iMapID = 0;
	m_iPosX = 0;
	m_iPosY = 0;

	m_iLeft = 0;
	m_iTop = 0;
	m_iRight = 0;
	m_iBottom = 0;

	m_strImage = "";
	m_lMessageID = 0;	

	m_lActionID = 0;

	DeleteCondition();
	DeleteItem();
}


void SPEventUnit::DeleteCondition() 
{
	std::vector<SPEventCondition*>::iterator iter;
	for(iter = m_vpCondition.begin(); iter != m_vpCondition.end(); ++iter)
	{
		SAFE_DELETE(*iter);
	}
	m_vpCondition.clear();
}


void SPEventUnit::DeleteItem()
{
	std::vector<SPEventItem*>::iterator iter;
	for(iter = m_vpItem.begin(); iter != m_vpItem.end(); ++iter)
	{
		SAFE_DELETE(*iter);
	}
	m_vpItem.clear();
}


bool SPEventUnit::AddCondition(int iCondition, int iValue)
{
	SPEventCondition* pCondition = new SPEventCondition;

	pCondition->SetCondition((CONDITION_TYPE)iCondition, iValue);
	
	m_vpCondition.push_back(pCondition);

	return true;
}


bool SPEventUnit::AddItem(int iItem, int iValue)
{
	SPEventItem* pItem = new SPEventItem;
	pItem->iConditionInven = (EVENT_INVEN_TYPE)iItem;
	pItem->iCheckValue1 = iValue;

	m_vpItem.push_back(pItem);

	return true;
}




//////////////////////////////////////////////////////////////////////////
void SPEventUnit::Init()
{
	SetNull();
}


void SPEventUnit::Process(float fTime)
{
	m_fAccumulateRender += fTime;
	m_fAccumulateProcess += fTime;

	if(m_fAccumulateProcess >= *(float*)m_pEventModel->GetMemberValue(MV_GET_FXLIFETIME)) {
		//m_FUState = FS_RELEASE;
		return;
	}


	if( m_fAccumulateRender > m_fCurDelay) {		// jinhee - [12/2/2004]
		m_fAccumulateRender -= m_fCurDelay;	
		
		//m_bRenderCancel = (m_fCamX > m_iPosX + (GetWidth() * GetScaleX()) || m_iPosX > (m_fCamX + 800)) ||
		//	(m_fCamY > m_iPosY + (GetHeight() * GetScaleY()) || m_iPosY > (m_fCamY + 600)) ;		

		if(m_bRenderCancel)
			return;
	}
	else{
		return;
	}

	switch(m_ClassID) {
	case TERRAIN_MODEL_CLASS_STATIC:
		{		
			//if(0 == (*m_ppCurImage))
			//	return ;			
			//(*m_ppCurImage)->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f ));
			//(*m_ppCurImage)->RenderTexture( m_iPosX - (int)m_fCamX, m_iPosY - (int)m_fCamY );
			//(*m_ppCurImage)->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f ));
		}
		break;
	case FX_MODEL_CLASS_FRAME_ANIMATION:
		{
			//ProcessFrameAnimation();
		}
		break;
	case FX_MODEL_CLASS_PARTICLE:
		{
			//ProcessParticle();
		}
		break;
	}
}


void SPEventUnit::Render(float fTime)
{
	if(*m_ppCurImage == NULL) 
		return;

	if(m_bRenderCancel)
		return;

	if(m_BLightMap) {
		g_pVideo->SetAlphaTexture(true);
	}

	m_fCamX = g_pCameraManagr->GetCameraStruct()->fOffsetX;
	m_fCamY = g_pCameraManagr->GetCameraStruct()->fOffsetY;

	g_pVideo->Flush();
	
	//렌더링 작업
	switch(m_ClassID) {
	case TERRAIN_MODEL_CLASS_STATIC:
		{		
			if(0 == (*m_ppCurImage))
				break;			
			(*m_ppCurImage)->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f ));
			(*m_ppCurImage)->RenderTexture( m_iPosX - (int)m_fCamX, m_iPosY - (int)m_fCamY );
			(*m_ppCurImage)->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f ));
		}
		break;
	case FX_MODEL_CLASS_FRAME_ANIMATION:
		{
			//ProcessFrameAnimation();
		}
		break;
	case FX_MODEL_CLASS_PARTICLE:
		{
			//ProcessParticle();
		}
		break;
	}

	if(m_BLightMap) {
		g_pVideo->SetAlphaTexture(false);
	}
}
