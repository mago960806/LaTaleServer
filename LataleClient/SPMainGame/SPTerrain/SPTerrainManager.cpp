

#include <algorithm>

#include "SPCommon.h"
#include "SPResourceDef.h"
#include "SPMainGameDEF.H"

#include "SPGameObjectDEF.h"
#include "SPManager.h"

#include "SPBGILoader.h"

#include "SPTerrainDEF.h"
#include "SPTerrainCluster.h"
#include "SPTerrainManager.h"

#include "SPStage.h"
#include "SPStageManager.h"

#include "SPSubGameManager.h"
#include "SPMainGameManager.h"

#include "SPZone.h"


using namespace std;

SPTerrainManager* g_pTerrainManager = NULL;

//extern SPStageManager g_StageManager;

struct _IsEqualLayer : binary_function<SPRotateScrollImage* , BG_ID, bool> {
	bool operator() (SPRotateScrollImage* pScrollPic, BG_ID LayerID) const
	{
		if(pScrollPic->PicID == LayerID)
			return true;
		return false;
	}
} IsEqualLayer;



SPTerrainManager::SPTerrainManager()
: SPManager()
, m_fLenX(0.0f) 
, m_fLenY(0.0f) 
, m_bViewAttr(false)
, m_pTerrainCluster(NULL)
, m_pBGILoader(NULL)
{
	SetMgrID(MGR_ID_TERRAIN);
	g_pTerrainManager = this;
}


SPTerrainManager::~SPTerrainManager()
{
	Clean();		
	g_pTerrainManager = NULL;
}

bool SPTerrainManager::Init()
{
	SPManager::Init();	
	m_pBGILoader		= new SPBGILoader;	

	m_pTerrainCluster	= new SPTerrainCluster;
	m_pTerrainCluster->Init(this);

	return true;
}

void SPTerrainManager::Clean()
{
	SPManager::Clean();

	DeleteAllStageBG();

	SAFE_DELETE(m_pBGILoader);
	SAFE_DELETE(m_pTerrainCluster);

}

void SPTerrainManager::SetLength(float fX, float fY)
{
	m_fLenX = fX;
	m_fLenY = fY;
}

bool SPTerrainManager::PurgeAll()
{
	DeleteAllStageBG();

	m_pTerrainCluster->Clean();

	g_StageManager.DeleteAllStage();
	return true;
}


bool SPTerrainManager::DeleteAllStageBG(){
	std::vector<std::vector<SPRotateScrollImage*> >::iterator iterBGList;
	for(iterBGList = m_vpBackRotatePicList.begin();
		iterBGList != m_vpBackRotatePicList.end();
		++iterBGList)
	{
		std::vector<SPRotateScrollImage*>::iterator iterPic;
		for(iterPic = (*iterBGList).begin();
			iterPic != (*iterBGList).end();
			++iterPic)
		{
			SAFE_DELETE(*iterPic);
		}		
	}
	m_vpBackRotatePicList.clear();
	
	return true;
}




void SPTerrainManager::Process(float fTime)
{
	SPManager::Process(fTime);

	// process running background.
	//if(m_vpBackRotatePic.empty()) return;

	if(g_StageManager.GetMapGroup() == NULL)		return;
	
	if(m_vpBackRotatePicList.empty()) return;

	//if(g_StageManager.GetMapGroup()->m_iType == GROUP_TYPE_HOUSE)	return;
	
	if(g_StageManager.GetMapGroup()->m_iBGID < 0 || g_StageManager.GetMapGroup()->m_iBGID >= m_vpBackRotatePicList.size()) return;	
	
	std::vector<SPRotateScrollImage*> pScrollImageList = m_vpBackRotatePicList[g_StageManager.GetMapGroup()->m_iBGID];	
	
	//if(g_pInputManager->GetInputStruct()->ikF1.InputState == INPUT_PRESS) {		
	//	m_bViewAttr  = !m_bViewAttr;
	//}
	
    float fCamX = g_pCameraManagr->GetCameraStruct()->fOffsetX;
	float fCamY = g_pCameraManagr->GetCameraStruct()->fOffsetY;

	DWORD dwHeight	= g_pVideo->GetScreenLenY();
	DWORD dwWidth	= g_pVideo->GetScreenLenX();

	//[xialin 2008/08/06]add >>>
	if ( g_pSubGameManager->GetGameState() == GS_CASHSHOP )
	{
		fCamX += CASHSHOP_OFFSETCAMX;
		fCamY += CASHSHOP_OFFSETCAMY;
	}
	//[xialin 2008/08/06]add <<<

	m_pTerrainCluster->SetCameraOffset(fCamX, fCamY);
	
	
	
	std::vector<SPRotateScrollImage*>::iterator Iter = pScrollImageList.begin();	

	//std::vector<SPRotateScrollImage*>::iterator Iter = m_vpBackRotatePic.begin();
	
	for(;Iter != pScrollImageList.end(); ++Iter) {
	//for(;Iter != m_vpBackRotatePic.end(); ++Iter) {
		switch((*Iter)->PicID) {
			case BG_ID_LAYER_BACKGROUND0: // BackGround
				{			  
					(*Iter)->fResultX = (*Iter)->fX - (fCamX * (((*Iter)->fSX) - dwWidth) / (m_fLenX - dwWidth));
					(*Iter)->fResultY = (*Iter)->fY - (fCamY * (((*Iter)->fSY) - dwHeight) / (m_fLenY - dwHeight));
				}
				break;

			case BG_ID_LAYER_TERRAIN0: // Terrain
			case BG_ID_LAYER_TERRAIN1:
			case BG_ID_LAYER_TERRAIN2:
				{
					(*Iter)->fResultX = (*Iter)->fX -  fCamX;
					(*Iter)->fResultY = (*Iter)->fY -  fCamY;
				}
				break;			

			case BG_ID_LAYER_BACKGROUND1:
			case BG_ID_LAYER_BACKGROUND2:
			case BG_ID_LAYER_BACKGROUND3:
			case BG_ID_LAYER_FOREGROUND0:
			case BG_ID_LAYER_FOREGROUND1:
			case BG_ID_LAYER_FOREGROUND2:
			case BG_ID_LAYER_FOREGROUND3:
				{
					// X축과 Y축 둘중 하나만 된다.
					if((*Iter)->pImage == NULL)		continue;			//[2005/5/17] - jinhee
					
					if((*Iter)->fRotateStepX) {							// is Rotate?
						
						if((*Iter)->fDX < -0.01f) {						//[2005/10/31]
							(*Iter)->iResultRotateX = (((m_fLenX * (((*Iter)->fDX * -1.0f) + 1)) - (*Iter)->fX) / (*Iter)->fRotateStepX) + 1;
						}
						else {
							(*Iter)->iResultRotateX = (((m_fLenX * ((*Iter)->fDX + 1)) - (*Iter)->fX) / (*Iter)->fRotateStepX) + 1;
						}
						
						//[2005/10/31]
						//(*Iter)->iResultRotateX = (((m_fLenX * ((*Iter)->fDX + 1)) - (*Iter)->fX) / (*Iter)->fRotateStepX) + 1;

						if( (*Iter)->fFlowDX ) {	// is Flow?							
							while((*Iter)->fX < -(float)((*Iter)->pImage->GetLenX())) 
								(*Iter)->fX += (*Iter)->fRotateStepX;
							while((*Iter)->fX > (*Iter)->fReservedX ) 
								(*Iter)->fX -= (*Iter)->fRotateStepX;
							(*Iter)->fX -= (((*Iter)->fFlowDX) * fTime);
						}
						else if( (*Iter)->fFlowDX < 0.0f ) {			//[2005/11/4] - 역으로 흐르는 경우 
							while((*Iter)->fX < -(float)((*Iter)->pImage->GetLenX())) 
								(*Iter)->fX += (*Iter)->fRotateStepX;
							while((*Iter)->fX > (*Iter)->fReservedX ) 
								(*Iter)->fX -= (*Iter)->fRotateStepX;
							(*Iter)->fX -= (((*Iter)->fFlowDX) * fTime);
						}

					} else {

						if((*Iter)->fRotateStepY) // is Rotate?
							(*Iter)->iResultRotateY = (((m_fLenY * ((*Iter)->fDY + 1)) - (*Iter)->fY) / (*Iter)->fRotateStepY) + 1;

						if( (*Iter)->fFlowDY ) {	// is Flow?
							while((*Iter)->fY < -(float)((*Iter)->pImage->GetLenY())) 
								(*Iter)->fY += (*Iter)->fRotateStepY;
							while((*Iter)->fY > (*Iter)->fReservedY ) 
								(*Iter)->fY -= (*Iter)->fRotateStepY;
							(*Iter)->fY -= (((*Iter)->fFlowDY) * fTime);		
						}
					}

					(*Iter)->fResultX = (*Iter)->fX - ( fCamX * ((*Iter)->fDX + 1));
					(*Iter)->fResultY = (*Iter)->fY - ( fCamY * ((*Iter)->fDY + 1));

				}
				break;

			case BG_ID_LAYER_NULL:
			default:
				//assert(0 && "NOT initialized layer.");
				break;				
		}
	}

	m_pTerrainCluster->Process(fTime);


}


void SPTerrainManager::Render(float fTime)
{
	SPManager::Render(fTime);

	// Exception by metalgeni
	SPMapGroup* pMapGroup = g_StageManager.GetMapGroup();
	if( pMapGroup == NULL )
		return;
	if(m_vpBackRotatePicList.empty()) 
		return;

	/////////////////////////////////////////////////////////////////////////////////////////	
	// test 02

	if( pMapGroup->m_iBGID < 0 || pMapGroup->m_iBGID >= (int)m_vpBackRotatePicList.size() )
		return;

	std::vector<SPRotateScrollImage*> pScrollImageList = m_vpBackRotatePicList[pMapGroup->m_iBGID];
	std::vector<SPRotateScrollImage*>::iterator Iter = pScrollImageList.begin();
	
	int i;
	for(;Iter != pScrollImageList.end() && (*Iter)->PicID != BG_ID_LAYER_FOREGROUND0; ++Iter) {
		if( (*Iter) == NULL )
			continue;

		for( i = -1; i < (*Iter)->iResultRotateX; ++i) {
			if(0 == (*Iter)->pImage)//jinhee
				continue;

			if(FALSE < (*Iter)->iLightMap){	//Light Map - jinhee - 04.11.03
				g_pVideo->SetAlphaTexture(true);
			}			


			(*Iter)->pImage->SetScale((*Iter)->fScaleX, (*Iter)->fScaleY); //스케일 - jinhee - 04.11.03
			
			////  [2005/10/31]
			//(*Iter)->pImage->RenderTexture((*Iter)->fResultX + ((i+1) * (*Iter)->fRotateStepX),
			//							  (*Iter)->fResultY);
			(*Iter)->pImage->RenderTexture(
				static_cast<int>((*Iter)->fResultX) + ((i + 1) * static_cast<int>((*Iter)->fRotateStepX)),
				(*Iter)->fResultY);
			
			(*Iter)->pImage->SetScale(1.0f, 1.0f);
			if(FALSE < (*Iter)->iLightMap){
				g_pVideo->SetAlphaTexture(false);
			}

			g_pVideo->Flush();			
		}	

		for( i = 0; i < (*Iter)->iResultRotateY; ++i) {	
			if(0 == (*Iter)->pImage)//jinhee
				continue;

			if(FALSE < (*Iter)->iLightMap){	//Light Map - jinhee - 04.11.03
				g_pVideo->SetAlphaTexture(true);
			}

			(*Iter)->pImage->SetScale((*Iter)->fScaleX, (*Iter)->fScaleY); //스케일 - jinhee - 04.11.03
			(*Iter)->pImage->RenderTexture(	(*Iter)->fResultX,
											(*Iter)->fResultY + ((i+1) * (*Iter)->fRotateStepY));
			
			(*Iter)->pImage->SetScale(1.0f, 1.0f);
			if(FALSE < (*Iter)->iLightMap){
				g_pVideo->SetAlphaTexture(false);
			}

			g_pVideo->Flush();
		}
	}
	
	m_pTerrainCluster->RenderLayer1(fTime);

	g_pVideo->Flush();
}


void SPTerrainManager::RenderLayer2(float fTime)
{	
	m_pTerrainCluster->RenderLayer2(fTime);

	g_pVideo->Flush();
}


void SPTerrainManager::RenderLayer3(float fTime)
{	
	m_pTerrainCluster->RenderLayer3(fTime);
	
	////
	//static std::vector<SPRotateScrollImage*> pScrollImageList = m_vpBackRotatePicList[g_StageManager.GetMapGroup()->m_iBGID];
	//
	//static std::vector<SPRotateScrollImage*>::iterator Iter = 
	//	std::find_if(pScrollImageList.begin(),
	//		pScrollImageList.end(),
	//		std::bind2nd(IsEqualLayer,
	//		BG_ID_LAYER_FOREGROUND0));	
	//
	////[2005/3/25]
	//std::vector<SPRotateScrollImage*> pScrollImageList = m_vpBackRotatePicList[g_StageManager.GetMapGroup()->m_iBGID];
	//
	//std::vector<SPRotateScrollImage*>::iterator Iter = 
	//	std::find_if(pScrollImageList.begin(),
	//	pScrollImageList.end(),
	//	std::bind2nd(IsEqualLayer,
	//	BG_ID_LAYER_FOREGROUND0));	
	//
	//int i;
	//for(; Iter != pScrollImageList.end(); ++Iter) {
	//	for(i = -1; i < (*Iter)->iResultRotateX; ++i) {
	//		if(FALSE < (*Iter)->iLightMap){	//Light Map - jinhee - 04.11.03
	//			g_pVideo->SetAlphaTexture(true);
	//		}
	//		
	//		if(0 == (*Iter)->pImage)//jinhee
	//			continue;
	//
	//		(*Iter)->pImage->SetScale((*Iter)->fScaleX, (*Iter)->fScaleY); //스케일 - jinhee - 04.11.03
	//		
	//		(*Iter)->pImage->RenderTexture((*Iter)->fResultX + ((i+1) * (*Iter)->fRotateStepX),
	//									  (*Iter)->fResultY);
	//		
	//		(*Iter)->pImage->SetScale(1.0f, 1.0f);
	//		if(FALSE < (*Iter)->iLightMap){
	//			g_pVideo->SetAlphaTexture(false);
	//		}
	//		g_pVideo->Flush();
	//	}
	//
	//	for(i = 0; i < (*Iter)->iResultRotateY; ++i) {
	//		if(FALSE < (*Iter)->iLightMap){	//Light Map - jinhee - 04.11.03
	//			g_pVideo->SetAlphaTexture(true);
	//		}
	//
	//		if(0 == (*Iter)->pImage)//jinhee
	//			continue;
	//
	//		(*Iter)->pImage->SetScale((*Iter)->fScaleX, (*Iter)->fScaleY); //스케일 - jinhee - 04.11.03
	//		
	//		(*Iter)->pImage->RenderTexture((*Iter)->fResultX,
	//									  (*Iter)->fResultY + ((i+1) * (*Iter)->fRotateStepY));
	//		
	//		(*Iter)->pImage->SetScale(1.0f, 1.0f);
	//		if(FALSE < (*Iter)->iLightMap){
	//			g_pVideo->SetAlphaTexture(false);
	//		}
	//		g_pVideo->Flush();
	//	}
	//
	//}
	//// Rrender attribute map for test.
	//m_pTerrainCluster->RenderAttr();

	g_pVideo->Flush();
}


void SPTerrainManager::RenderForeGround(float fTime){
	//[2005/3/25]

	SPMapGroup* pMapGroup = g_StageManager.GetMapGroup();
	if(!pMapGroup)
		return;

	if( pMapGroup->m_iBGID < 0 || pMapGroup->m_iBGID >= (int)m_vpBackRotatePicList.size() )
		return;

	std::vector<SPRotateScrollImage*> pScrollImageList = m_vpBackRotatePicList[pMapGroup->m_iBGID];

	std::vector<SPRotateScrollImage*>::iterator Iter = 
		std::find_if(pScrollImageList.begin(),
		pScrollImageList.end(),
		std::bind2nd(IsEqualLayer,
		BG_ID_LAYER_FOREGROUND0));	

	int i;
	for(; Iter != pScrollImageList.end(); ++Iter) {
		if( (*Iter) == NULL )
			continue;

		for(i = -1; i < (*Iter)->iResultRotateX; ++i) {
			if(0 == (*Iter)->pImage)//jinhee
				continue;

			if(FALSE < (*Iter)->iLightMap){	//Light Map - jinhee - 04.11.03
				g_pVideo->SetAlphaTexture(true);
			}


			(*Iter)->pImage->SetScale((*Iter)->fScaleX, (*Iter)->fScaleY); //스케일 - jinhee - 04.11.03

			////[2005/10/31]
			//(*Iter)->pImage->RenderTexture((*Iter)->fResultX + ((i+1) * (*Iter)->fRotateStepX),
			//	(*Iter)->fResultY);
			(*Iter)->pImage->RenderTexture(
				static_cast<int>((*Iter)->fResultX) + ((i + 1) * static_cast<int>((*Iter)->fRotateStepX)),
				(*Iter)->fResultY);

			(*Iter)->pImage->SetScale(1.0f, 1.0f);
			if(FALSE < (*Iter)->iLightMap){
				g_pVideo->SetAlphaTexture(false);
			}
			g_pVideo->Flush();
		}

		for(i = 0; i < (*Iter)->iResultRotateY; ++i) {
			if(0 == (*Iter)->pImage)//jinhee
				continue;

			if(FALSE < (*Iter)->iLightMap){	//Light Map - jinhee - 04.11.03
				g_pVideo->SetAlphaTexture(true);
			}


			(*Iter)->pImage->SetScale((*Iter)->fScaleX, (*Iter)->fScaleY); //스케일 - jinhee - 04.11.03

			(*Iter)->pImage->RenderTexture((*Iter)->fResultX,
				(*Iter)->fResultY + ((i+1) * (*Iter)->fRotateStepY));

			(*Iter)->pImage->SetScale(1.0f, 1.0f);
			if(FALSE < (*Iter)->iLightMap){
				g_pVideo->SetAlphaTexture(false);
			}
			g_pVideo->Flush();
		}

	}
	// Rrender attribute map for test.
	m_pTerrainCluster->RenderAttr();
	
	g_pVideo->Flush();
}


int SPTerrainManager::SPPerformMessage(UINT msg, WPARAM wparam, LPARAM lParam) 
{ 
	return SPTMESSAGE_FUNC((SPTM)msg, wparam, lParam);	
}


SPTMESSAGE_MAP_BEGIN( SPTerrainManager )
	SPTMESSAGE_COMMAND( SPTM_PURGEALL		,		OnPurgeAll		)
	SPTMESSAGE_COMMAND( SPTM_INIT			,		OnInit			)
	SPTMESSAGE_COMMAND( SPTM_LOADPALLET		,		OnLoadPallet	)
SPTMESSAGE_MAP_END_BASE()


int SPTerrainManager::OnPurgeAll	(WPARAM wparam, LPARAM lparam)
{
	//	destroy	m_TerrainCluster
	//	destroy	m_vpBackRotatePic
	return -1;
}


int SPTerrainManager::OnInit		(WPARAM wparam, LPARAM lparam)
{
	//	Init	m_TerrainCluster
	//	Init	m_vpBackRotatePic
	return -1; 
}


int SPTerrainManager::OnLoadPallet	(WPARAM wparam, LPARAM lparam)
{
	if(m_pTerrainCluster->LoadPallet( (const char*) wparam ))
		return 0;
	return -1;
}


int SPTerrainManager::LoadBG(SPStage* pStage /*= NULL*/) {	
	if(0 == pStage)
		return -1;
	
	string strPath;  //= RES_WORK_RESOURCE_PATH;
	string strFile = "";
	int iCounter = 0;	

	std::vector<SPMapGroup*>::iterator iterMapGroup;
	
	iterMapGroup = pStage->m_vpMapGroup.begin();
	for(; iterMapGroup != pStage->m_vpMapGroup.end(); ++iterMapGroup)
	{
		strFile = "";
		strFile = (*iterMapGroup)->m_strBGFile;
		
		std::vector<SPRotateScrollImage*> vpBackRotatePic;
		if(false == (m_pBGILoader->LoadBGBinary(&vpBackRotatePic, strPath, strFile))) {
			return 0;
		}

		m_vpBackRotatePicList.push_back(vpBackRotatePic);
		iCounter++;
	}
	
	return iCounter;
}


int SPTerrainManager::LoadForm(SPStage* pStage /*= NULL*/) {
	if(0 == pStage)
		return -1;
	
	if(false == m_pTerrainCluster->LoadForm( pStage ))		
		return 0;
	
	return 1;
}


int SPTerrainManager::LoadAttribute(SPStage* pStage /*= NULL*/) {
	if(0 == pStage)
		return -1;

	if(false == m_pTerrainCluster->LoadAttribute( pStage))
		return 0;
	
	return 1;
}




