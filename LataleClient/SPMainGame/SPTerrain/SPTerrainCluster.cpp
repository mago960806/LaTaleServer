
#include <assert.h>

#include "SPCommon.h"
#include "SPUtil.h"
#include "SPResourceDef.h"

#include "SPGameObjectDEF.h"
#include "SPUtil.h"

#include "SPManager.h"

#include "SPTerrainDEF.h"
#include "SPTerrainAttribute.h"
#include "SPTerrainCluster.h"
#include "SPTerrainManager.h"

#include "SPRenderModelDEF.h"
#include "SPRenderModel.h"

#include "SPTerrainModel.h"
#include "SPTerrainModelStatic.h"
#include "SPTerrainModelFrameAnimation.h"
#include "SPTerrainModelCircularMotion.h"

#include "dxutil.h"
#include "SPTerrainPalletLoader.h"
#include "SPTerrainFormLoader.h"
#include "SPTerrainAttributeLoader.h"

#include "SPTerrainUnit.h"

#include "SPStage.h"
#include "SPStageManager.h"

#include "SPDebug.h"

//extern SPStageManager g_StageManager;

SPTerrainCluster::SPTerrainCluster()
: m_pParent(NULL)
, m_fCameraOffsetX(0)
, m_fCameraOffsetY(0)
, m_pAttrImageLayer1(NULL)
, m_pAttrImageLayer2(NULL)
, m_pTerrainAttribute(NULL)
, m_iFirstMapID(-1)
, m_iSecondMapID(-1)
, m_bShowSecondLayer(false)
{
}

SPTerrainCluster::~SPTerrainCluster()
{
	Clean();
	
	//[2005/5/31] Global Palette 삭제
	std::vector<SPTerrainModel*>::iterator Iter;	
	Iter = m_vpTerrainGlobalModel.begin();
	for(;Iter != m_vpTerrainGlobalModel.end(); ++Iter) {	
		(*Iter)->Destroy();
		SAFE_DELETE(*Iter);
	}
	m_vpTerrainGlobalModel.clear();
}


void SPTerrainCluster::Clean()
{
	SAFE_RELEASE(m_pAttrImageLayer1);
	SAFE_RELEASE(m_pAttrImageLayer2);

	std::vector<SPTerrainModel*>::iterator Iter;
	Iter = m_vpTerrainLocalModel.begin();

	for(;Iter != m_vpTerrainLocalModel.end(); ++Iter) {	
		(*Iter)->Destroy();
		SAFE_DELETE(*Iter);
	}
	m_vpTerrainLocalModel.clear();

	////[2005/5/31]
	//Iter = m_vpTerrainGlobalModel.begin();
	//for(;Iter != m_vpTerrainGlobalModel.end(); ++Iter) {	
	//	(*Iter)->Destroy();
	//	SAFE_DELETE(*Iter);
	//}
	//m_vpTerrainGlobalModel.clear();

	DeleteAllTerrainLayerList();
	DeleteAllAttributeLayerList();

	m_vpShareTerrainLayer1.clear();
	m_vpShareTerrainLayer2.clear();
	m_vpShareTerrainLayer3.clear();

	SAFE_DELETE(m_pTerrainAttribute);

}


bool SPTerrainCluster::DeleteAllTerrainLayerList() 
{
	std::vector<std::vector<SPTerrainUnit*> >::iterator iterLayer;
	
	iterLayer = m_vpTerrainLayer1List.begin();
	for(; iterLayer != m_vpTerrainLayer1List.end(); ++iterLayer) {
		std::vector<SPTerrainUnit*>::iterator iterUnit;
		iterUnit = (*iterLayer).begin();
		for(; iterUnit != (*iterLayer).end(); ++iterUnit) {
			SAFE_DELETE(*iterUnit);
		}
	}
	m_vpTerrainLayer1List.clear();
	
	iterLayer = m_vpTerrainLayer2List.begin();
	for(; iterLayer != m_vpTerrainLayer2List.end(); ++iterLayer) {
		std::vector<SPTerrainUnit*>::iterator iterUnit;
		iterUnit = (*iterLayer).begin();
		for(; iterUnit != (*iterLayer).end(); ++iterUnit) {
			SAFE_DELETE(*iterUnit);
		}
	}
	m_vpTerrainLayer2List.clear();

	iterLayer = m_vpTerrainLayer3List.begin();
	for(; iterLayer != m_vpTerrainLayer3List.end(); ++iterLayer) {
		std::vector<SPTerrainUnit*>::iterator iterUnit;
		iterUnit = (*iterLayer).begin();
		for(; iterUnit != (*iterLayer).end(); ++iterUnit) {
			SAFE_DELETE(*iterUnit);
		}
	}
	m_vpTerrainLayer3List.clear();
	
	return true;
}


bool SPTerrainCluster::DeleteAllAttributeLayerList()
{
	std::vector<SPTerrainAttribute*>::iterator iterAttr;
	iterAttr = m_vpTerrainAttributeList.begin();
	for(; iterAttr != m_vpTerrainAttributeList.end(); ++iterAttr) {
		SAFE_DELETE(*iterAttr);
	}
	m_vpTerrainAttributeList.clear();

	return true;
}


bool SPTerrainCluster::Init(SPTerrainManager* pParent)
{
	m_pParent = pParent;

#if defined(_DEBUG)
	DXUTOutputDebugString("TerrainCluster::Init()\n");
#endif

#if !defined(_JUST_RUN)
	// Load Pallet, Form 	
	LoadPallet(NULL);
#endif

	return true;
}

bool SPTerrainCluster::LoadPallet( const char* pszFName)
{
	bool	bRet;
	SPTerrainPalletLoader		PalletLoader;	

	if(pszFName == NULL) {
		//Load Global TerrainPalette
		bRet = PalletLoader.LoadBinary( RES_FNAME_GLOBAL_TERRAINPALLET_BIN, &m_vpTerrainGlobalModel, "", true );
		if(!bRet) {	
			//assert(bRet && "공용 지형 팔레트 로딩 실패");			
			DXUTOutputDebugString("Load RES_FNAME_GLOBAL_TERRAINPALLET_BIN Fail... [%s]\n", RES_FNAME_GLOBAL_TERRAINPALLET_BIN);
			return false;
		}		
		DXUTOutputDebugString("Load RES_FNAME_GLOBAL_TERRAINPALLET_BIN OK... [%s] Size[%d]\n", RES_FNAME_GLOBAL_TERRAINPALLET_BIN, static_cast<int>(m_vpTerrainGlobalModel.size()));
	}
	else {
		//Load Local TerrainPalette
		bRet = PalletLoader.LoadBinary( pszFName, &m_vpTerrainLocalModel );
		if(!bRet) {	
			assert(bRet && "로컬 팔레트 로딩 실패");
			return false;
		}		
		DXUTOutputDebugString("Load Local Palette OK... [%s]\n", pszFName);
	}
	

	////Load Local TerrainPalette
	//bRet = PalletLoader.LoadBinary( pszFName, &m_vpTerrainLocalModel );
	//if(!bRet) {	
	//	assert(bRet && "로컬 팔레트 로딩 실패");
	//	return false;
	//}

	//
	//DXUTOutputDebugString("로컬 파레트 로드 완료 \n");

	return true;
}

// 백터인 경우
bool SPTerrainCluster::LoadForm( SPStage* pStage /*= NULL*/)
{
	bool	bRet;	
	SPTerrainFormLoader			FormLoader;	
	
	string strFile = "";
	
	if(0 == pStage)
		return false;
	
	std::vector<SPMapGroup*>::iterator iterMapGroup;
	iterMapGroup = pStage->m_vpMapGroup.begin();
	for(; iterMapGroup != pStage->m_vpMapGroup.end(); ++iterMapGroup) {
		std::vector<SPMapInfo*>::iterator iterMap;
		int iCounter = 0;	
		
		for(iterMap = (*iterMapGroup)->m_vpMapInfo.begin(), iCounter = 0;
			iterMap != (*iterMapGroup)->m_vpMapInfo.end();
			++iterMap, iCounter++)
		{
			strFile = "";
			//strFile += strPath;
			float fMapLeft = (*iterMap)->fMapLeft;
			float fMapTop = (*iterMap)->fMapTop;

			strFile = (*iterMap)->strFormFile;		

			std::vector<SPTerrainUnit*>		vpTerrainLayer1;
			std::vector<SPTerrainUnit*>		vpTerrainLayer2;
			std::vector<SPTerrainUnit*>		vpTerrainLayer3;

			FormLoader.SetCommonPallet(&m_vpTerrainGlobalModel);
			bRet = FormLoader.LoadBinary( strFile.c_str(),
				&vpTerrainLayer1,
				&vpTerrainLayer2,
				&vpTerrainLayer3,				
				&m_vpTerrainLocalModel,
				fMapLeft,
				fMapTop);

			if(false == bRet) {	
				assert(bRet && "지역 지형 팔레트, 터레인 유닛 로딩 실패");
				return false;
			}

			m_vpTerrainLayer1List.push_back(vpTerrainLayer1);
			m_vpTerrainLayer2List.push_back(vpTerrainLayer2);
			m_vpTerrainLayer3List.push_back(vpTerrainLayer3);
		}
	}

	return true;
}


//// 배열인 경우
//bool SPTerrainCluster::LoadForm( SPStage* pStage /*= NULL*/)
//{
//	bool	bRet;	
//	SPTerrainFormLoader			FormLoader;	
//
//	string strFile = "";
//
//	if(0 == pStage)
//		return false;
//
//	std::vector<SPMapGroup*>::iterator iterMapGroup;
//	iterMapGroup = pStage->m_vpMapGroup.begin();
//	for(; iterMapGroup != pStage->m_vpMapGroup.end(); ++iterMapGroup) {
//		//std::vector<SPMapInfo*>::iterator iterMap;
//		int iCounter = 0;	
//
//		//for(iterMap = (*iterMapGroup)->m_vpMapInfo.begin(), iCounter = 0;
//		//	iterMap != (*iterMapGroup)->m_vpMapInfo.end();
//		//	++iterMap, iCounter++)
//		int index = 0;
//		for(index = 0, iCounter = 0; index < (*iterMapGroup)->m_iMapCount ; index ++)
//		{
//			strFile = "";
//			//strFile += strPath;
//			float fMapLeft = (*iterMapGroup)->m_pMapInfo[index].fMapLeft;
//			float fMapTop = (*iterMapGroup)->m_pMapInfo[index].fMapTop;
//
//			strFile = (*iterMapGroup)->m_pMapInfo[index].strFormFile;		
//
//			std::vector<SPTerrainUnit*>		vpTerrainLayer1;
//			std::vector<SPTerrainUnit*>		vpTerrainLayer2;
//			std::vector<SPTerrainUnit*>		vpTerrainLayer3;
//
//			FormLoader.SetCommonPallet(&m_vpTerrainGlobalModel);
//			bRet = FormLoader.LoadDataText( strFile.c_str(),
//				&vpTerrainLayer1,
//				&vpTerrainLayer2,
//				&vpTerrainLayer3,				
//				&m_vpTerrainLocalModel,
//				fMapLeft,
//				fMapTop);
//
//			if(false == bRet) {	
//				assert(bRet && "지역 지형 팔레트, 터레인 유닛 로딩 실패");
//				return false;
//			}
//
//			m_vpTerrainLayer1List.push_back(vpTerrainLayer1);
//			m_vpTerrainLayer2List.push_back(vpTerrainLayer2);
//			m_vpTerrainLayer3List.push_back(vpTerrainLayer3);
//		}
//	}
//
//	return true;
//}


// 백터인 경우
bool SPTerrainCluster::LoadAttribute( SPStage* pStage /*= NULL*/)
{
	if(0 == pStage)
		return false;
	
	SPTerrainAttribute*	pTerrainAttribute;
	string strFile = "";
	bool bResult = false;

	//
	std::vector<SPMapGroup*>::iterator iterMapGroup;
	iterMapGroup = pStage->m_vpMapGroup.begin();
	for(; iterMapGroup != pStage->m_vpMapGroup.end(); ++iterMapGroup) {
		std::vector<SPMapInfo*>::iterator iterMap;
		int iCounter = 0;			

		for(iterMap = (*iterMapGroup)->m_vpMapInfo.begin(), iCounter = 0;
			iterMap != (*iterMapGroup)->m_vpMapInfo.end();
			++iterMap, iCounter++)
		{
			pTerrainAttribute = NULL;
			pTerrainAttribute = new SPTerrainAttribute;
			pTerrainAttribute->Init();

			strFile = "";		
			strFile = (*iterMap)->strAttributeFile;

			bResult = false;
			bResult = pTerrainAttribute->Load("", strFile, (*iterMapGroup)->m_lGroupWidth, (*iterMapGroup)->m_lGroupHeight);

			if(false == bResult){
				return false;
			}

			//각 맵별로 그룹내에서의 영역을 지정한다. 
			
			
			//전체 맵 사이즈... 루틴 나중에 외부로 뺄것
			if(MAP_LINK_HORIZONTAL == (*iterMapGroup)->m_iMapLink) {
				(*iterMap)->SetMapRect((*iterMapGroup)->m_lGroupWidth,
					0,
					pTerrainAttribute->GetWidth(),
					pTerrainAttribute->GetHeight());
				
				(*iterMapGroup)->m_lGroupWidth += pTerrainAttribute->GetWidth();
				(*iterMapGroup)->m_lGroupHeight = pTerrainAttribute->GetHeight();
			}
			else if(MAP_LINK_VERTICAL == (*iterMapGroup)->m_iMapLink) {
				(*iterMap)->SetMapRect(0,
					(*iterMapGroup)->m_lGroupHeight,
					pTerrainAttribute->GetWidth(),
					pTerrainAttribute->GetHeight());

				(*iterMapGroup)->m_lGroupWidth = pTerrainAttribute->GetWidth();
				(*iterMapGroup)->m_lGroupHeight += pTerrainAttribute->GetHeight(); 
			}
			else if(MAP_LINK_NULL == (*iterMapGroup)->m_iMapLink) {
				(*iterMap)->SetMapRect(0,
					0,
					pTerrainAttribute->GetWidth(),
					pTerrainAttribute->GetHeight());

				(*iterMapGroup)->m_lGroupWidth = pTerrainAttribute->GetWidth();
				(*iterMapGroup)->m_lGroupHeight = pTerrainAttribute->GetHeight();
			}
			
			m_vpTerrainAttributeList.push_back(pTerrainAttribute);		//삭제예정
			pStage->m_vpTerrainAttribute.push_back(pTerrainAttribute);
		}
	}

	//현재의 그룹에 해당하는 크기를 실제 화면에 적용해야 한다.
	//m_fLenX = pStage->GetMapGroup()->m_lGroupWidth;
	//m_fLenY = pStage->GetMapGroup()->m_lGroupHeight;
	//m_fLenX = g_StageManager.GetMapGroup()->m_lGroupWidth;
	//m_fLenY = g_StageManager.GetMapGroup()->m_lGroupHeight;

	//m_pParent->SetLength(m_fLenX, m_fLenY);
	
	return true;
}


////[2005/2/4] - 배열인 경우
//bool SPTerrainCluster::LoadAttribute( SPStage* pStage /*= NULL*/)
//{
//	if(0 == pStage)
//		return false;
//
//	SPTerrainAttribute*	pTerrainAttribute;
//	string strFile = "";
//	bool bResult = false;
//
//	//
//	std::vector<SPMapGroup*>::iterator iterMapGroup;
//	iterMapGroup = pStage->m_vpMapGroup.begin();
//	for(; iterMapGroup != pStage->m_vpMapGroup.end(); ++iterMapGroup) {
//		//std::vector<SPMapInfo*>::iterator iterMap;
//		int iCounter = 0;			
//
//		//for(iterMap = (*iterMapGroup)->m_vpMapInfo.begin(), iCounter = 0;
//		//	iterMap != (*iterMapGroup)->m_vpMapInfo.end();
//		//	++iterMap, iCounter++)
//		int index = 0;
//		for(index = 0, iCounter = 0; index < (*iterMapGroup)->m_iMapCount ; index ++)
//		{
//			pTerrainAttribute = NULL;
//			pTerrainAttribute = new SPTerrainAttribute;
//			pTerrainAttribute->Init();
//
//			strFile = "";		
//			//strFile = (*iterMap)->strAttributeFile;
//			strFile = (*iterMapGroup)->m_pMapInfo[index].strAttributeFile;
//
//			bResult = false;
//			bResult = pTerrainAttribute->Load("", strFile, (*iterMapGroup)->m_lGroupWidth, (*iterMapGroup)->m_lGroupHeight);
//
//			if(false == bResult){
//				return false;
//			}
//
//			//각 맵별로 그룹내에서의 영역을 지정한다. 
//
//
//			//전체 맵 사이즈... 루틴 나중에 외부로 뺄것
//			if(MAP_LINK_HORIZONTAL == (*iterMapGroup)->m_iMapLink) {
//				(*iterMapGroup)->m_pMapInfo[index].SetMapRect((*iterMapGroup)->m_lGroupWidth,
//					0,
//					pTerrainAttribute->GetWidth(),
//					pTerrainAttribute->GetHeight());
//
//				(*iterMapGroup)->m_lGroupWidth += pTerrainAttribute->GetWidth();
//				(*iterMapGroup)->m_lGroupHeight = pTerrainAttribute->GetHeight();
//			}
//			else if(MAP_LINK_VERTICAL == (*iterMapGroup)->m_iMapLink) {
//				(*iterMapGroup)->m_pMapInfo[index].SetMapRect(0,
//					(*iterMapGroup)->m_lGroupHeight,
//					pTerrainAttribute->GetWidth(),
//					pTerrainAttribute->GetHeight());
//
//				(*iterMapGroup)->m_lGroupWidth = pTerrainAttribute->GetWidth();
//				(*iterMapGroup)->m_lGroupHeight += pTerrainAttribute->GetHeight(); 
//			}
//			else if(MAP_LINK_NULL == (*iterMapGroup)->m_iMapLink) {
//				(*iterMapGroup)->m_pMapInfo[index].SetMapRect(0,
//					0,
//					pTerrainAttribute->GetWidth(),
//					pTerrainAttribute->GetHeight());
//
//				(*iterMapGroup)->m_lGroupWidth = pTerrainAttribute->GetWidth();
//				(*iterMapGroup)->m_lGroupHeight = pTerrainAttribute->GetHeight();
//			}
//
//			m_vpTerrainAttributeList.push_back(pTerrainAttribute);		//삭제예정
//			pStage->m_vpTerrainAttribute.push_back(pTerrainAttribute);
//		}
//	}
//
//	//현재의 그룹에 해당하는 크기를 실제 화면에 적용해야 한다.
//	m_fLenX = g_StageManager.GetMapGroup()->m_lGroupWidth;
//	m_fLenY = g_StageManager.GetMapGroup()->m_lGroupHeight;
//	m_pParent->SetLength(m_fLenX, m_fLenY);
//
//	return true;
//}


bool SPTerrainCluster::MoveDrop(float& fx, float& fy, BG_ID& bgLayer, const float fDropStep)
{
	CLIMB_STATE iClimbState = CLIMB_STATE_NONE ; //[2005/3/10] 임시입니다.
	int iClimbEnable = 0;	//[2005/3/15] 임시입니다.
	BG_ID bgRenderLayer = BG_ID_LAYER_NULL; //[2005/3/21] 임시입니다.
	//return m_pTerrainAttribute->MoveDrop(fx, fy, bgLayer, fDropStep, iClimbState, iClimbEnable, bgRenderLayer);
	return m_pTerrainAttribute->MoveDrop(fx, fy, bgLayer);
}

bool SPTerrainCluster::InspectStandLayer(float fx, float fy, BG_ID& bgLayer)
{	
	//return m_pTerrainAttribute->InspectStandLayer(fx, fy, bgLayer);

	//우선 임시로 해놓음...		//jinhee - [2005/1/29]
	float fVelocityX = 0.0f;	
	float fVelocityY = 0.0f;
	CLIMB_STATE iClimbState = CLIMB_STATE_NONE ;	//[2005/3/10] 임시입니다.
	int iClimbEnable = 0;	//[2005/3/15] 임시입니다.
	BG_ID bgRenderLayer = BG_ID_LAYER_NULL; //[2005/3/21] 임시입니다.
	//return m_pTerrainAttribute->InspectStandLayer(fx, fy, bgLayer, fx, fy, fVelocityX, fVelocityY, iClimbState, iClimbEnable, bgRenderLayer);
	//float fRenderPosX = 0.0f;
	//float fRenderPosY = 0.0f;
	//return m_pTerrainAttribute->InspectStandLayer(fx, fy, bgLayer, fRenderPosY);
	return true;
}

void SPTerrainCluster::SetCameraOffset(float fX, float fY)
{
	m_fCameraOffsetX = fX;
	m_fCameraOffsetY = fY;
}

void SPTerrainCluster::Process(float fTime)
{	
	m_iFirstMapID = 0;
	m_iSecondMapID = 0;
	m_bShowSecondLayer = false;

	//std::vector<SPRotateScrollImage*> pScrollImageList = m_vpBackRotatePicList[g_StageManager.GetMapGroup()->m_iBGID];
	
	//카메라에 따른 맵 체크
	if(g_StageManager.GetMapGroup() == NULL)		return;
	
	SPMapGroup* pMapGroup = g_StageManager.GetMapGroup();	

	if(MAP_LINK_NULL == pMapGroup->m_iMapLink){
		m_iFirstMapID = pMapGroup->FindMapID(m_fCameraOffsetX, m_fCameraOffsetY);
		m_iSecondMapID = -1;
	}
	else if(MAP_LINK_HORIZONTAL == pMapGroup->m_iMapLink) {
		m_iFirstMapID = pMapGroup->FindMapID(m_fCameraOffsetX, m_fCameraOffsetY); 
		//m_iSecondMapID = pMapGroup->FindMapID(m_fCameraOffsetX + 800, m_fCameraOffsetY);
		m_iSecondMapID = pMapGroup->FindMapID(m_fCameraOffsetX + g_pVideo->GetScreenLenX() + 128, m_fCameraOffsetY);
	}
	else if(MAP_LINK_VERTICAL == pMapGroup->m_iMapLink) {
		m_iFirstMapID = pMapGroup->FindMapID(m_fCameraOffsetX, m_fCameraOffsetY);
		//m_iSecondMapID = pMapGroup->FindMapID(m_fCameraOffsetX, m_fCameraOffsetY + 600);
		m_iSecondMapID = pMapGroup->FindMapID(m_fCameraOffsetX, m_fCameraOffsetY + g_pVideo->GetScreenLenY());
	}	

	//첫번재 맵은 항상 있으므로...	
	if( m_iFirstMapID < 0) return;
	if( m_vpTerrainLayer1List.size() - 1 < m_iFirstMapID)		return;		//[2005/5/31]	

	std::vector<SPTerrainUnit*>	vpFirstTerrainLayer1 = m_vpTerrainLayer1List[m_iFirstMapID];
	std::vector<SPTerrainUnit*>	vpFirstTerrainLayer2 = m_vpTerrainLayer2List[m_iFirstMapID];
	std::vector<SPTerrainUnit*>	vpFirstTerrainLayer3 = m_vpTerrainLayer3List[m_iFirstMapID];		

	std::vector<SPTerrainUnit*>::iterator Iter;	

	//[2005/11/9] 공용으로 사용되는 타일들은 여기서 한번에 처리한다 
	Iter = m_vpShareTerrainLayer1.begin();
	for(; Iter != m_vpShareTerrainLayer1.end(); ++Iter) {
		(*Iter)->Process(fTime);
	}

	Iter = m_vpShareTerrainLayer2.begin();
	for(; Iter != m_vpShareTerrainLayer2.end(); ++Iter) {
		(*Iter)->Process(fTime);
	}

	Iter = m_vpShareTerrainLayer3.begin();
	for(; Iter != m_vpShareTerrainLayer3.end(); ++Iter) {
		(*Iter)->Process(fTime);
	}

	Iter = vpFirstTerrainLayer1.begin();
	for(;Iter != vpFirstTerrainLayer1.end(); ++Iter) {
		if((*Iter)->GetClassID() == TERRAIN_MODEL_CLASS_FRAME_ANIMATION) {		//공용으로 사용되는 에니메이션 타일은 넘어간다
			if((*Iter)->GetTerrainModel()->GetMemberValue(MV_GET_FRAMEGROUP)) {
				continue;
			}
		}
		(*Iter)->Process(fTime);
	}

	Iter = vpFirstTerrainLayer2.begin();
	for(;Iter != vpFirstTerrainLayer2.end(); ++Iter) {
		if((*Iter)->GetClassID() == TERRAIN_MODEL_CLASS_FRAME_ANIMATION) {
			if((*Iter)->GetTerrainModel()->GetMemberValue(MV_GET_FRAMEGROUP)) {
				continue;
			}
		}
		(*Iter)->Process(fTime);
	}

	Iter = vpFirstTerrainLayer3.begin();
	for(;Iter != vpFirstTerrainLayer3.end(); ++Iter) {
		if((*Iter)->GetClassID() == TERRAIN_MODEL_CLASS_FRAME_ANIMATION) {
			if((*Iter)->GetTerrainModel()->GetMemberValue(MV_GET_FRAMEGROUP)) {
				continue;
			}
		}
		(*Iter)->Process(fTime);
	}

	if(MAP_LINK_NULL != g_StageManager.GetMapGroup()->m_iMapLink){
		if( (m_iFirstMapID == m_iSecondMapID) || (m_iSecondMapID < 0) ) {	//2번재 맵이 있는경우...
			return;
		}
		
		m_bShowSecondLayer = true;
		
		//if( m_vpTerrainLayer1List.size() - 1 < m_iSecondMapID)		return;		//[2005/5/31]
		std::vector<SPTerrainUnit*>	vpSecondTerrainLayer1 = m_vpTerrainLayer1List[m_iSecondMapID];
		std::vector<SPTerrainUnit*>	vpSecondTerrainLayer2 = m_vpTerrainLayer2List[m_iSecondMapID];
		std::vector<SPTerrainUnit*>	vpSecondTerrainLayer3 = m_vpTerrainLayer3List[m_iSecondMapID];
		
		Iter = vpSecondTerrainLayer1.begin();
		for(;Iter != vpSecondTerrainLayer1.end(); ++Iter) {
			if((*Iter)->GetClassID() == TERRAIN_MODEL_CLASS_FRAME_ANIMATION) {
				if((*Iter)->GetTerrainModel()->GetMemberValue(MV_GET_FRAMEGROUP)) {
					continue;
				}
			}
			(*Iter)->Process(fTime);
		}

		Iter = vpSecondTerrainLayer2.begin();
		for(;Iter != vpSecondTerrainLayer2.end(); ++Iter) {
			if((*Iter)->GetClassID() == TERRAIN_MODEL_CLASS_FRAME_ANIMATION) {
				if((*Iter)->GetTerrainModel()->GetMemberValue(MV_GET_FRAMEGROUP)) {
					continue;
				}
			}
			(*Iter)->Process(fTime);
		}

		Iter = vpSecondTerrainLayer3.begin();
		for(;Iter != vpSecondTerrainLayer3.end(); ++Iter) {
			if((*Iter)->GetClassID() == TERRAIN_MODEL_CLASS_FRAME_ANIMATION) {
				if((*Iter)->GetTerrainModel()->GetMemberValue(MV_GET_FRAMEGROUP)) {
					continue;
				}
			}
			(*Iter)->Process(fTime);
		}		
	}
}


void SPTerrainCluster::RenderLayer1(float fTime)
{	
	if( m_vpTerrainLayer1List.empty() ) return; //by metalgeni
	if( m_iFirstMapID < 0) return;	
	if( m_vpTerrainLayer1List.size() - 1 < m_iFirstMapID)		return;		//[2005/5/31]

	std::vector<SPTerrainUnit*>	vpFirstTerrainLayer1 = m_vpTerrainLayer1List[m_iFirstMapID];
	
	std::vector<SPTerrainUnit*>::iterator Iter;
	Iter = vpFirstTerrainLayer1.begin();
	for(;Iter != vpFirstTerrainLayer1.end(); ++Iter) {
		(*Iter)->Render(fTime);
		g_pVideo->Flush();
	}	
	
	RenderSecondLayer1(fTime);
}


void SPTerrainCluster::RenderSecondLayer1(float fTime) //[2005/1/20]
{
	if(m_bShowSecondLayer){		

		if( m_vpTerrainLayer1List.empty() ) return; //by metalgeni

		std::vector<SPTerrainUnit*>	vpSecondTerrainLayer1 = m_vpTerrainLayer1List[m_iSecondMapID];	

		std::vector<SPTerrainUnit*>::iterator Iter;
		Iter = vpSecondTerrainLayer1.begin();
		for(;Iter != vpSecondTerrainLayer1.end(); ++Iter) {
			(*Iter)->Render(fTime);
			g_pVideo->Flush();
		}
	}
	g_pVideo->Flush();
}


void SPTerrainCluster::RenderLayer2(float fTime)
{	//

	if( m_vpTerrainLayer2List.empty() ) return; //by metalgeni
	if( m_iFirstMapID < 0) return;
	if( m_vpTerrainLayer2List.size() - 1 < m_iFirstMapID)		return;		//[2005/5/31]

	std::vector<SPTerrainUnit*>	vpFirstTerrainLayer2 = m_vpTerrainLayer2List[m_iFirstMapID];

	std::vector<SPTerrainUnit*>::iterator Iter;
	Iter = vpFirstTerrainLayer2.begin();
	for(;Iter != vpFirstTerrainLayer2.end(); ++Iter) {
		(*Iter)->Render(fTime);
		g_pVideo->Flush();
	}
	
	RenderSecondLayer2(fTime);
}


void SPTerrainCluster::RenderSecondLayer2(float fTime) //[2005/1/20]
{

	if(m_bShowSecondLayer){
		if( m_vpTerrainLayer2List.empty() ) return; //by metalgeni

		std::vector<SPTerrainUnit*>	vpSecondTerrainLayer2 = m_vpTerrainLayer2List[m_iSecondMapID];

		std::vector<SPTerrainUnit*>::iterator Iter;
		Iter = vpSecondTerrainLayer2.begin();
		
		for(;Iter != vpSecondTerrainLayer2.end(); ++Iter) {
			(*Iter)->Render(fTime);
			g_pVideo->Flush();
		}
	}

	g_pVideo->Flush();
}


void SPTerrainCluster::RenderLayer3(float fTime)
{	
	if( m_vpTerrainLayer3List.empty() ) return; //by metalgeni
	if( m_iFirstMapID < 0) return;
	if( m_vpTerrainLayer1List.size() - 1 < m_iFirstMapID)		return;		//[2005/5/31]

	std::vector<SPTerrainUnit*>	vpFirstTerrainLayer3 = m_vpTerrainLayer3List[m_iFirstMapID];

	std::vector<SPTerrainUnit*>::iterator Iter;
	Iter = vpFirstTerrainLayer3.begin();
	for(;Iter != vpFirstTerrainLayer3.end(); ++Iter) {
		(*Iter)->Render(fTime);
		g_pVideo->Flush();
	}
	
	RenderSecondLayer3(fTime);
}


void SPTerrainCluster::RenderSecondLayer3(float fTime) //[2005/1/20]
{
	if(m_bShowSecondLayer){
		if( m_vpTerrainLayer3List.empty() ) return; //by metalgeni
		std::vector<SPTerrainUnit*>	vpSecondTerrainLayer3 = m_vpTerrainLayer3List[m_iSecondMapID];

		std::vector<SPTerrainUnit*>::iterator Iter;
		Iter = vpSecondTerrainLayer3.begin();
		for(;Iter != vpSecondTerrainLayer3.end(); ++Iter) {
			(*Iter)->Render(fTime);
			g_pVideo->Flush();
		}
	}

	g_pVideo->Flush();
}


void SPTerrainCluster::RenderAttr()
{
	if(!m_pParent->isRenderAttr()) return;

	return;									//[2005/11/2] 이방식으로는 어트리뷰트를 표현할수 없음
	
	BYTE	cAttr = 0;
	int		iLayer = 0;
	bool	bInspect = false;

	int iLenX = ATTR_PIXEL_TO_OFFSET_X(m_fLenX);
	int iLenY = ATTR_PIXEL_TO_OFFSET_Y(m_fLenY);

	for(int j=0; j<iLenY; ++j) {
		for(int i=0; i<iLenX; ++i) {

			bInspect = m_pTerrainAttribute->GetAttrType(i, j, cAttr, iLayer); // 
			if(bInspect == false) {
				assert(0 && "잘못된 어트리뷰트 좌표");
				iLayer = 0; // GetAttrType에서 모든 레이어 검색을 위해
				continue;
			}


			if(cAttr) {				
				if(iLayer == 1)
					m_pAttrImageLayer1->RenderTexture(i*32 - m_fCameraOffsetX,		
													(j)*16 - m_fCameraOffsetY);					
				
				if(iLayer == 2)
					m_pAttrImageLayer2->RenderTexture(i*32 - m_fCameraOffsetX,	
													(j)*16 - m_fCameraOffsetY);

				if(iLayer == 3)
					m_pAttrImageLayer2->RenderTexture(i*32 - m_fCameraOffsetX,	
													(j)*16 - m_fCameraOffsetY);

			}
			iLayer = 0; // GetAttrType에서 모든 레이어 검색을 위해
		}
	}
}


bool SPTerrainCluster::SetShareLayer()
{
	if(g_StageManager.GetMapGroup() == NULL)		return false;

	//기존의 리스트를 삭제한뒤...
	std::vector<SPTerrainUnit*>::iterator iter;
	//iter = m_vpShareTerrainLayer1.begin();
	//for(; iter != m_vpShareTerrainLayer1.end(); ++iter) {
	//	//m_vpShareTerrainLayer1.erase(iter);
	//}
	m_vpShareTerrainLayer1.clear();

	//iter = m_vpShareTerrainLayer2.begin();
	//for(; iter != m_vpShareTerrainLayer2.end(); ++iter) {
	//	//m_vpShareTerrainLayer1.erase(iter);
	//}
	m_vpShareTerrainLayer2.clear();

	//iter = m_vpShareTerrainLayer2.begin();
	//for(; iter != m_vpShareTerrainLayer2.end(); ++iter) {
	//	//m_vpShareTerrainLayer1.erase(iter);
	//}
	m_vpShareTerrainLayer3.clear();

	DXUTOutputDebugString("SetShareLayer -D- L1[%d] L2[%d] L3[%d]\n", m_vpShareTerrainLayer1.size(), m_vpShareTerrainLayer2.size(), m_vpShareTerrainLayer3.size());
	
	//공용으로 사용될 리스트를 생성해 낸다.
	SPMapGroup* pMapGroup = g_StageManager.GetMapGroup();	

	int iFirstMapID = pMapGroup->GetFirstMapID();
	int iSize = pMapGroup->GetGroupSize();

	for(int i = 0; i < iSize; i++) {
		std::vector<SPTerrainUnit*>	vpFirstTerrainLayer1 = m_vpTerrainLayer1List[iFirstMapID + i];
		std::vector<SPTerrainUnit*>	vpFirstTerrainLayer2 = m_vpTerrainLayer2List[iFirstMapID + i];
		std::vector<SPTerrainUnit*>	vpFirstTerrainLayer3 = m_vpTerrainLayer3List[iFirstMapID + i];
		for(iter = vpFirstTerrainLayer1.begin(); iter != vpFirstTerrainLayer1.end(); ++iter) {
			if((*iter)->GetClassID() == TERRAIN_MODEL_CLASS_FRAME_ANIMATION) {
				if((*iter)->GetTerrainModel()->GetMemberValue(MV_GET_FRAMEGROUP)) {
					m_vpShareTerrainLayer1.push_back((*iter));
				}
			}
		}

		for(iter = vpFirstTerrainLayer2.begin(); iter != vpFirstTerrainLayer2.end(); ++iter) {
			if((*iter)->GetClassID() == TERRAIN_MODEL_CLASS_FRAME_ANIMATION) {
				if((*iter)->GetTerrainModel()->GetMemberValue(MV_GET_FRAMEGROUP)) {
					m_vpShareTerrainLayer2.push_back((*iter));
				}
			}
		}

		for(iter = vpFirstTerrainLayer3.begin(); iter != vpFirstTerrainLayer3.end(); ++iter) {
			if((*iter)->GetClassID() == TERRAIN_MODEL_CLASS_FRAME_ANIMATION) {
				if((*iter)->GetTerrainModel()->GetMemberValue(MV_GET_FRAMEGROUP)) {
					m_vpShareTerrainLayer3.push_back((*iter));
				}
			}
		}
	}	
	
	DXUTOutputDebugString("SetShareLayer -C- L1[%d] L2[%d] L3[%d]\n", m_vpShareTerrainLayer1.size(), m_vpShareTerrainLayer2.size(), m_vpShareTerrainLayer3.size());

	return false;
}


bool SPTerrainCluster::IsTerrainLayer2Back( int x, int y )
{
	if( m_vpTerrainLayer2List.empty() ) return false;
	if( m_iFirstMapID < 0) return false;
	if( m_vpTerrainLayer2List.size() - 1 < m_iFirstMapID)		return false;

	std::vector<SPTerrainUnit*>	vpFirstTerrainLayer2 = m_vpTerrainLayer2List[m_iFirstMapID];

	std::vector<SPTerrainUnit*>::iterator Iter;
	Iter = vpFirstTerrainLayer2.begin();
	for(;Iter != vpFirstTerrainLayer2.end(); ++Iter)
	{
		if ( x >= (*Iter)->GetPosX() && x <= (*Iter)->GetPosX() + (*Iter)->GetWidth()
			&& y >= (*Iter)->GetPosY() && y <= (*Iter)->GetPosY() + (*Iter)->GetHeight() )
		{
			return true;
		}
	}


	if(m_bShowSecondLayer){
		if( m_vpTerrainLayer2List.empty() ) return false;

		std::vector<SPTerrainUnit*>	vpSecondTerrainLayer2 = m_vpTerrainLayer2List[m_iSecondMapID];

		std::vector<SPTerrainUnit*>::iterator Iter;
		Iter = vpSecondTerrainLayer2.begin();
		
		for(;Iter != vpSecondTerrainLayer2.end(); ++Iter)
		{
			if ( x >= (*Iter)->GetPosX() && x <= (*Iter)->GetPosX() + (*Iter)->GetWidth()
				&& y >= (*Iter)->GetPosY() && y <= (*Iter)->GetPosY() + (*Iter)->GetHeight() )
			{
				return true;
			}

		}
	}

	return false;
}


bool SPTerrainCluster::IsTerrainLayer3Back( int x, int y )
{
	if( m_vpTerrainLayer3List.empty() ) return false;
	if( m_iFirstMapID < 0) return false;
	if( m_vpTerrainLayer1List.size() - 1 < m_iFirstMapID)		return false;

	std::vector<SPTerrainUnit*>	vpFirstTerrainLayer3 = m_vpTerrainLayer3List[m_iFirstMapID];

	std::vector<SPTerrainUnit*>::iterator Iter;
	Iter = vpFirstTerrainLayer3.begin();
	for(;Iter != vpFirstTerrainLayer3.end(); ++Iter)
	{
		if ( x >= (*Iter)->GetPosX() && x <= (*Iter)->GetPosX() + (*Iter)->GetWidth()
			&& y >= (*Iter)->GetPosY() && y <= (*Iter)->GetPosY() + (*Iter)->GetHeight() )
		{
			return true;
		}
	}
	
	if(m_bShowSecondLayer){
		if( m_vpTerrainLayer3List.empty() ) return false;
		std::vector<SPTerrainUnit*>	vpSecondTerrainLayer3 = m_vpTerrainLayer3List[m_iSecondMapID];

		std::vector<SPTerrainUnit*>::iterator Iter;
		Iter = vpSecondTerrainLayer3.begin();
		for(;Iter != vpSecondTerrainLayer3.end(); ++Iter)
		{
			if ( x >= (*Iter)->GetPosX() && x <= (*Iter)->GetPosX() + (*Iter)->GetWidth()
				&& y >= (*Iter)->GetPosY() && y <= (*Iter)->GetPosY() + (*Iter)->GetHeight() )
			{
				return true;
			}
		}
	}

	return false;
}




