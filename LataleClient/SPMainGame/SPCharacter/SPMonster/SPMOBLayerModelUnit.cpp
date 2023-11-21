#include <algorithm>
#include <functional>
#include <string>
#include <vector>
#include <map>

// Modify by kkandori (11.9. 2004)
#ifdef _LATALE_CLIENT
#include "SPCommon.h"
#include "SPGOBClusterDEF.h"
#include "SPMonsterModelUnitDef.h"
#include "SPGOBAnimation.h"
//#include "SPGOBModel.h"
#include "SPDebug.h"

#else 
#include "SPToolCommon.h"
#include "SPGameObjectDef.h"
#include "SPGameMessageDEF.H"
#include "SPRenderModelDef.h"	
#include "SPGOBClusterDEF.h"
#include "SPAvatarModelUnitDef.h"
#include "SPMonsterModelUnitDef.h"
#include "SPGameObjectToolCustom.h"
#include "SPGOBAnimation.h"
#include "SPGOBModel.h"
#endif
// Modify End

#include "SPGOBCluster.h"
#include "SPMOBCluster.h"

#include "SPRenderModelDEF.h"
#include "SPGameObjectDef.h"
#include "SPResourceDef.h"


#ifdef _LATALE_CLIENT
#include "SPGameObject.h"
#endif

#include "SPMonsterModelUnitDef.h"
#include "SPGOBModelUnit.h"
#include "SPMOBLayerModelUnit.h"

#include "SPUtil.h"

using namespace std;

//------------------------------------------------------------------------------------
SPMOBLayerModelUnit::SPMOBLayerModelUnit(SPGameObject* pParent)
: SPGOBModelUnit(pParent, GOB_MODEL_MONSTER)
, m_strImagePath(NULL)
, m_pMOBLayerModel(NULL)
, m_eAniIndex(ANI_NULL)
, m_eLastIndex(ANI_NULL)
, m_iCurAniFrame(-1)
, m_fRenderPosX(0)
, m_fRenderPosY(0)
, m_fAccumulateAction(0.0f)
, m_fAccumulateRenderTime(0.0f)
, m_bMustQuery(false)
, m_bAnimationLoop(true)
, m_iLayerNum(0)
, m_uiNewPetClassID(0)
{	
	m_strImagePath = new TCHAR[256];
	m_strImagePath[0] = '\0';
}

//------------------------------------------------------------------------------------
SPMOBLayerModelUnit::~SPMOBLayerModelUnit()
{
	SAFE_DELETE_ARRAY(m_pMOBLayerModel);
	SAFE_DELETE_ARRAY(m_strImagePath);

	ReleaseTexture();
}

//------------------------------------------------------------------------------------
void SPMOBLayerModelUnit::Init()
{	
	m_eAniIndex = m_eLastIndex = ANI_MONSTER_STAND;
	m_iCurAniFrame = 0;

	m_fAccumulateAction = 0.0f;
	m_fAccumulateRenderTime	= 0.0f;

#if defined(_LATALE_CLIENT)

	ResetLayerModel();

#if defined(_USED_TEXTURE_TABLE)
	SetLayerTexturebyTable(RES_PATH_MONSTER);
#else
	SetLayerTexture(RES_PATH_MONSTER);
#endif

#endif
}

//------------------------------------------------------------------------------------
void SPMOBLayerModelUnit::FrameInit()
{
	m_iCurAniFrame = 0; 
	m_bMustQuery = true;
}

//------------------------------------------------------------------------------------
void SPMOBLayerModelUnit::Process(float fTime)
{
	ProcessImage(m_fRenderPosX, m_fRenderPosY, fTime);
}

//------------------------------------------------------------------------------------
void SPMOBLayerModelUnit::ProcessImage(const float fPosX, const float fPosY, float fTime)
{
	m_fAccumulateAction += fTime;
	if( m_pMOBLayerModel == NULL || m_iLayerNum == 0 )
		return;

	// PosSet
	FACING eDir;
	unsigned int uiClassID;

#if defined(_CHARTOOL)
	eDir = GetParent()->GetDirection();
	if( fTime == 0 )
		m_fAccumulateAction = 0.0f;
#else
	eDir = (FACING)GetParent()->SPGOBPerformMessage(SPGM_GETFACING);
#endif

	uiClassID = m_uiNewPetClassID;
	if (uiClassID == 0)
		uiClassID = GetParent()->GetClassID();
	if( m_bMustQuery || m_fAccumulateAction > m_pMOBLayerModel[0].FrameInfo.fDelay )
	{
		m_fAccumulateAction -= m_pMOBLayerModel[0].FrameInfo.fDelay;
		m_eLastIndex = m_eAniIndex;
		m_bMustQuery = false;

		g_pMOBCluster->QueryAnimationSequence(m_pMOBLayerModel, m_iLayerNum,
			uiClassID, m_eAniIndex, m_iCurAniFrame, m_bAnimationLoop);
		ResetTexture();
	}

#if defined(_USED_TEXTURE_TABLE)
	SetFramePosbyTable((int)fPosX, (int)fPosY, eDir);
#else
	SetFramePos((int)fPosX, (int)fPosY, eDir);
#endif

}

//------------------------------------------------------------------------------------
void SPMOBLayerModelUnit::SetFramePos(int iPosX, int iPosY, FACING eFacing)
{
	if( m_pMOBLayerModel == NULL || m_iLayerNum == 0 )
		return;

	ZeroMemory(&m_rcRenderRgn, sizeof(RECT));
	bool bRenderRgnCheck = false;

	int iResourceIndex, iIndexX, iIndexY, i;
	RECT rcDst, rcSrc;
	for( i = 0; i < m_iLayerNum; i++ )
	{
		ZeroMemory(&m_pMOBLayerModel[i].rcSrc, sizeof(RECT));
		ZeroMemory(&m_pMOBLayerModel[i].rcDst, sizeof(RECT));

		if( m_pMOBLayerModel[i].eAniIndex == ANI_NULL )
			continue;

		iResourceIndex = m_pMOBLayerModel[i].FrameInfo.iResourceIndex - 1;
		if( iResourceIndex < 0 )
			continue;

		iIndexX = iResourceIndex % m_pMOBLayerModel[i].iFrameCountX;
		iIndexY = iResourceIndex / m_pMOBLayerModel[i].iFrameCountX;

		rcSrc.left = iIndexX * m_pMOBLayerModel[i].iFrameWidth;
		rcSrc.top = iIndexY * m_pMOBLayerModel[i].iFrameHeight;
		rcSrc.right = (iIndexX + 1) * m_pMOBLayerModel[i].iFrameWidth;
		rcSrc.bottom = (iIndexY + 1) * m_pMOBLayerModel[i].iFrameHeight;

		if( eFacing == FACING_LEFT )
		{
			rcDst.left = iPosX - m_pMOBLayerModel[i].FrameInfo.iPosX;
			rcDst.right = rcDst.left + m_pMOBLayerModel[i].iFrameWidth;

			m_pMOBLayerModel[i].FrameInfo.iRotation = m_pMOBLayerModel[i].iDefaultRotation;
		}
		else
		{
			rcDst.left = iPosX + m_pMOBLayerModel[i].FrameInfo.iPosX;
			rcDst.right = rcDst.left - m_pMOBLayerModel[i].iFrameWidth;

			m_pMOBLayerModel[i].FrameInfo.iRotation = -m_pMOBLayerModel[i].iDefaultRotation;
		}

		rcDst.top = iPosY - m_pMOBLayerModel[i].FrameInfo.iPosY;
		rcDst.bottom = rcDst.top + m_pMOBLayerModel[i].iFrameHeight;

		m_pMOBLayerModel[i].rcSrc = rcSrc;
		m_pMOBLayerModel[i].rcDst = rcDst;

		if( !bRenderRgnCheck )
		{
			m_rcRenderRgn = rcDst;
			bRenderRgnCheck = true;
		}
		else
		{
			m_rcRenderRgn.left = min(m_rcRenderRgn.left, rcDst.left);
			m_rcRenderRgn.left = min(m_rcRenderRgn.left, rcDst.right);
			m_rcRenderRgn.right = max(m_rcRenderRgn.right, rcDst.left);
			m_rcRenderRgn.right = max(m_rcRenderRgn.right, rcDst.right);

			m_rcRenderRgn.top = min(m_rcRenderRgn.top, rcDst.top);
			m_rcRenderRgn.bottom = max(m_rcRenderRgn.bottom, rcDst.bottom);
		}
	}
}

//------------------------------------------------------------------------------------
#if defined(_USED_TEXTURE_TABLE)
void SPMOBLayerModelUnit::SetFramePosbyTable(int iPosX, int iPosY, FACING eFacing)
{
	if( m_pMOBLayerModel == NULL || m_iLayerNum == 0 )
		return;

	ZeroMemory(&m_rcRenderRgn, sizeof(RECT));
	bool bRenderRgnCheck = false;

	int iResourceIndex, iSrcWidth, iSrcHeight, i;
	RECT rcDst, rcSrc;
	TEXTURE_TABLE_INFO* pTableInfo;
	FRAME_INFO* pFrameInfo;

	for( i = 0; i < m_iLayerNum; i++ )
	{
		ZeroMemory(&m_pMOBLayerModel[i].rcSrc, sizeof(RECT));
		ZeroMemory(&m_pMOBLayerModel[i].rcDst, sizeof(RECT));

		if( m_pMOBLayerModel[i].vpTableInfo.empty() )
			continue;

		pTableInfo = m_pMOBLayerModel[i].vpTableInfo[0];

		iResourceIndex = m_pMOBLayerModel[i].FrameInfo.iResourceIndex;
		pFrameInfo = GetResIndexRectbyTable(pTableInfo, iResourceIndex, rcSrc);

		m_pMOBLayerModel[i].pFrameInfo = pFrameInfo;
		if( pFrameInfo == NULL )
			continue;

		iSrcWidth = pFrameInfo->rcResourceRgn.right - pFrameInfo->rcResourceRgn.left;
		iSrcHeight = pFrameInfo->rcResourceRgn.bottom - pFrameInfo->rcResourceRgn.top;

		if( eFacing == FACING_LEFT )
		{
			rcDst.left = iPosX - m_pMOBLayerModel[i].FrameInfo.iPosX + pFrameInfo->iOffsetX;
			rcDst.right = rcDst.left + iSrcWidth;

			m_pMOBLayerModel[i].FrameInfo.iRotation = m_pMOBLayerModel[i].iDefaultRotation;
		}
		else
		{
			rcDst.left = iPosX + m_pMOBLayerModel[i].FrameInfo.iPosX - pFrameInfo->iOffsetX;
			rcDst.right = rcDst.left - iSrcWidth;

			m_pMOBLayerModel[i].FrameInfo.iRotation = -m_pMOBLayerModel[i].iDefaultRotation;
		}

		rcDst.top = iPosY - m_pMOBLayerModel[i].FrameInfo.iPosY + pFrameInfo->iOffsetY;
		rcDst.bottom = rcDst.top + iSrcHeight;

		m_pMOBLayerModel[i].rcSrc = rcSrc;
		m_pMOBLayerModel[i].rcDst = rcDst;

		if( !bRenderRgnCheck )
		{
			m_rcRenderRgn = rcDst;
			bRenderRgnCheck = true;
		}
		else
		{
			m_rcRenderRgn.left = min(m_rcRenderRgn.left, rcDst.left);
			m_rcRenderRgn.left = min(m_rcRenderRgn.left, rcDst.right);
			m_rcRenderRgn.right = max(m_rcRenderRgn.right, rcDst.left);
			m_rcRenderRgn.right = max(m_rcRenderRgn.right, rcDst.right);

			m_rcRenderRgn.top = min(m_rcRenderRgn.top, rcDst.top);
			m_rcRenderRgn.bottom = max(m_rcRenderRgn.bottom, rcDst.bottom);
		}
	}
}
#endif

//------------------------------------------------------------------------------------
void SPMOBLayerModelUnit::Render(float fTime)
{
	if( m_pMOBLayerModel == NULL || m_iLayerNum == 0 )
		return;
	
	int i;
	for( i = 0; i < m_iLayerNum; i++ )
	{
		if( m_pMOBLayerModel[i].bLightMap )
			g_pVideo->SetAlphaTexture(true);

		m_pMOBLayerModel[i].Render();

		if( m_pMOBLayerModel[i].bLightMap )
			g_pVideo->SetAlphaTexture(false);

		g_pVideo->Flush();
	}
}

//------------------------------------------------------------------------------------
void SPMOBLayerModelUnit::SetAccumulateAction(float fAccumulateAction)
{
	m_fAccumulateAction = fAccumulateAction;
}

//------------------------------------------------------------------------------------
void SPMOBLayerModelUnit::SetAnimationIndex(ANIMATION_INDEX AniInx, int iForce) 
{ 
	if( m_pMOBLayerModel == NULL )
		return;

	m_eAniIndex = AniInx;
	if( m_eAniIndex == m_eLastIndex && iForce == 0 )
		return;

	m_fAccumulateAction = 0.0f;
	m_fAccumulateRenderTime = 0.0f;

	m_pMOBLayerModel[0].FrameInfo.fDelay = 0.0f;
	m_eLastIndex = m_eAniIndex;
	FrameInit();
}

//------------------------------------------------------------------------------------
float SPMOBLayerModelUnit::GetAniAccumulateTime()
{
	float fAccumulateTime = 0.0f;
#if defined(_LATALE_CLIENT)
	unsigned int uiClassID = m_uiNewPetClassID;
	if (uiClassID == 0)
		uiClassID = GetParent()->GetClassID();
	if( m_pMOBLayerModel && m_iLayerNum )
		fAccumulateTime = g_pMOBCluster->GetAnimationTimeLayer(uiClassID, m_eAniIndex);
#endif
	return fAccumulateTime;
}

//------------------------------------------------------------------------------------
RECT SPMOBLayerModelUnit::GetRenderRgn()
{
	return m_rcRenderRgn;
}

//------------------------------------------------------------------------------------
float SPMOBLayerModelUnit::GetRenderPosX()
{
	return m_fRenderPosX;
}

//------------------------------------------------------------------------------------
float SPMOBLayerModelUnit::GetRenderPosY()
{
	return m_fRenderPosY;
}

//------------------------------------------------------------------------------------
#if defined(_CHARTOOL)
SPMOBLayerModel* SPMOBLayerModelUnit::GetMOBLayerModel(int iLayer)
{
	if( m_pMOBLayerModel == NULL || iLayer >= m_iLayerNum )
		return NULL;

	return &m_pMOBLayerModel[iLayer];
}

//------------------------------------------------------------------------------------
void SPMOBLayerModelUnit::SetCurAniFrame(int iFrame)
{
	if( m_pMOBLayerModel == NULL )
		return;

	m_iCurAniFrame = iFrame;
	unsigned int uiClassID = m_uiNewPetClassID;
	if (uiClassID == 0)
		uiClassID = GetParent()->GetClassID();
	g_pMOBCluster->QueryAnimationSequence(m_pMOBLayerModel, m_iLayerNum,
		uiClassID, m_eAniIndex, iFrame, m_bAnimationLoop);
	ResetTexture();
}

#endif

//------------------------------------------------------------------------------------
void SPMOBLayerModelUnit::SetAnimationLoop(bool bLoop)
{
	m_bAnimationLoop = bLoop;
}

//------------------------------------------------------------------------------------
void SPMOBLayerModelUnit::SetLayerTexture(const char* pszImagePath)
{
	ReleaseTexture();
	unsigned int uiClassID = m_uiNewPetClassID;
	if (uiClassID == 0)
		uiClassID = m_pParent->GetClassID();
	m_iLayerNum = g_pMOBCluster->GetMOBLayerSize(uiClassID);

	SPTexture* pkTexture;
	SPMOBLayerInfo* pLayerInfo;
	char szFullName[MAX_PATH];
	int i;

	for( i = 0; i < m_iLayerNum; i++ )
	{
		pLayerInfo = g_pMOBCluster->GetMOBLayerInfo(uiClassID, i);
		if( pLayerInfo == NULL )
			continue;

		if( pLayerInfo->strImageName.empty() )
			continue;

		sprintf(szFullName, "%s%s", pszImagePath, pLayerInfo->strImageName.c_str());
		//if( !g_pVideo->GetTextureMgr()->LoadTexture(szFullName, &pkTexture) )
		if( !g_pVideo->GetTextureMgr()->GetTexture(szFullName, &pkTexture,TEX_MON,LEVEL_NOW) )
			continue;

		m_mTextureList.insert(make_pair(i, pkTexture));
	}
	ResetTexture();
}

//------------------------------------------------------------------------------------
#if defined(_USED_TEXTURE_TABLE)
void SPMOBLayerModelUnit::SetLayerTexturebyTable(const char* pszImagePath)
{
	ReleaseTexture();
	unsigned int uiClassID = m_uiNewPetClassID;
	if (uiClassID == 0)
		uiClassID = m_pParent->GetClassID();
	m_iLayerNum = g_pMOBCluster->GetMOBLayerSize(uiClassID);

	SPTexture* pkTexture;
	SPMOBLayerInfo* pLayerInfo;
	TEXTURE_TABLE_INFO* pTableInfo;
	
	char szFullName[MAX_PATH];
	int i;

	for( i = 0; i < m_iLayerNum; i++ )
	{
		pLayerInfo = g_pMOBCluster->GetMOBLayerInfo(uiClassID, i);
		if( pLayerInfo == NULL )
			continue;

		if( pLayerInfo->strImageName.empty() )
			continue;

		pTableInfo = g_pMOBCluster->GetTextureTable(pLayerInfo->strImageName);
		if( pTableInfo == NULL )
			continue;

		sprintf(szFullName, "%s%s", pszImagePath, pTableInfo->strTextureName.c_str());
		//if( !g_pVideo->GetTextureMgr()->LoadTexture(szFullName, &pkTexture) )
		if( !g_pVideo->GetTextureMgr()->GetTexture(szFullName, &pkTexture,TEX_MON,LEVEL_NOW) )
			continue;

		m_mTextureList.insert(make_pair(i, pkTexture));
	}
	ResetTexture();
}
#endif

//------------------------------------------------------------------------------------
void SPMOBLayerModelUnit::ResetLayerModel()
{
	SAFE_DELETE_ARRAY(m_pMOBLayerModel);

	unsigned int uiClassID = m_uiNewPetClassID;
	if (uiClassID == 0)
		uiClassID = m_pParent->GetClassID();
	m_iLayerNum = g_pMOBCluster->GetMOBLayerSize(uiClassID);
	if( m_iLayerNum )
		m_pMOBLayerModel = new SPMOBLayerModel[m_iLayerNum];
}

//------------------------------------------------------------------------------------
void SPMOBLayerModelUnit::ResetTexture()
{
	if( m_pMOBLayerModel == NULL || m_iLayerNum == 0 )
		return;

	int i;
	for( i = 0; i < m_iLayerNum; i++ )
	{
		if( m_pMOBLayerModel[i].iLayerNo < 0 )
			continue;

		m_pMOBLayerModel[i].pkTexture = GetTexture(m_pMOBLayerModel[i].iLayerNo);

#if defined(_USED_TEXTURE_TABLE)
		SPMOBLayerInfo* pLayerInfo;
		TEXTURE_TABLE_INFO* pTableInfo;

		m_pMOBLayerModel[i].vpTableInfo.clear();
		unsigned int uiClassID = m_uiNewPetClassID;
		if (uiClassID == 0)
			uiClassID = GetParent()->GetClassID();
		pLayerInfo = g_pMOBCluster->GetMOBLayerInfo(uiClassID, m_pMOBLayerModel[i].iLayerNo);
		if( pLayerInfo == NULL )
			continue;

		pTableInfo = g_pMOBCluster->GetTextureTable(pLayerInfo->strImageName);
		if( pTableInfo )
			m_pMOBLayerModel[i].vpTableInfo.push_back(pTableInfo);
#endif
	}
}

//------------------------------------------------------------------------------------
SPTexture* SPMOBLayerModelUnit::GetTexture(int iLayerNo)
{
	map< int, SPTexture* >::iterator iter = m_mTextureList.find(iLayerNo);
	if( iter == m_mTextureList.end() )
		return NULL;

	return (*iter).second;
}

//------------------------------------------------------------------------------------
void SPMOBLayerModelUnit::ReleaseTexture()
{
	map< int, SPTexture* >::iterator iter = m_mTextureList.begin();
	while( iter != m_mTextureList.end() )
	{
		if( (*iter).second )
			((*iter).second)->Release();
		
		++iter;
	}
	m_mTextureList.clear();
}

//------------------------------------------------------------------------------------
LRESULT SPMOBLayerModelUnit::SPSendMessage(MODEL_VALUE_TYPE mvMsg, WPARAM wParam, LPARAM lParam)
{
	switch( mvMsg )
	{
	case MV_FRAMEINIT:				
		FrameInit();						
		return 0;

	case MV_SET_ACCUMUL_ACTION:	
		float fAccTime;
		DataConvert<WPARAM,float>(wParam, fAccTime);		// Data Convert!!!
		SetAccumulateAction	(fAccTime);
		return 0;

	case MV_SET_ANIMATION_INDEX:
		SetAnimationIndex((ANIMATION_INDEX)wParam, (int)lParam);
		return 0;

	case MV_SET_ANI_LOOP:
		SetAnimationLoop((bool)wParam);
		return 0;

	case MV_GET_ANIMATION_INDEX:
		return (LRESULT)m_eAniIndex;

	case MV_GET_CURR_FRAME:
		return (LRESULT)m_iCurAniFrame;

	case MV_GET_ANI_ACCUMUL_TIME:
		{
			float fTime = GetAniAccumulateTime();
			*((float*)wParam) = fTime;
		}
		return 0;

	case MV_SET_RENDERPOSX:
		{
			DataConvert<WPARAM,float>(wParam, m_fRenderPosX);
		}
		return 0;

	case MV_SET_RENDERPOSY:
		{
			DataConvert<WPARAM,float>(wParam, m_fRenderPosY);
		}
		return 0;

	case MV_SET_RESET_LAYER:
		ResetLayerModel();
		return 0;

	case MV_GET_RENDER_RGN:
		{
			*((RECT*)wParam) = GetRenderRgn();
		}
		return 0;
	}

	return SPGOBModelUnit::SPSendMessage(mvMsg, wParam, lParam);
}

void SPMOBLayerModelUnit::SetNewPet(unsigned int ClassID)
{
	m_uiNewPetClassID = ClassID;
	Init();
}
