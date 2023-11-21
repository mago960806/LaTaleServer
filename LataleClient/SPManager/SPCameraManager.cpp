

#include "SPCommon.h"
#include "SPDebug.h"

#include "SPMainGameDEF.H"
#include "SPGameObjectDEF.h"
#include "SPManager.h"

#include "SPRenderModelDEF.h"
#include "SPRenderModel.h"

#include "SPTerrainDEF.h"
#include "SPTerrainCluster.h"
#include "SPTerrainManager.h"
#include "SPGOBManager.h"

#include "SPGameObject.h"

#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPGOBStatus.h"
#include "SPPlayerStatus.h"
#include "SPPlayerStatusModel.h"
#include "SPGOBCoordPhysics.h"
#include "SPPlayerCoordPhysics.h"
#include "SPPlayer.h"

#include "SPCameraManager.h"

#include "SPSubGameManager.h"

#include "SPWindowDEF.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "SPZone.h"

extern int g_iTraceMsgLevel;

SPCameraManager* g_pCameraManagr = NULL;

SPCameraManager::SPCameraManager()
: SPManager()
{		
}

SPCameraManager::~SPCameraManager()
{
	g_pCameraManagr = NULL;
}

bool SPCameraManager::Init()
{
	SPManager::Init();

	SetMgrID(MGR_ID_CAMERA);	
	m_Focus = FOCUS_CHAR;
	m_CameraStruct.Init();

	g_pCameraManagr = this;

	return true;
}

void SPCameraManager::Clean()
{
	SPManager::Clean();
}

void SPCameraManager::SetCameraMode(const FOCUS focus)
{
	m_Focus = focus;
	g_pInputManager->PlayerTrigger();
}

SPCameraStruct* SPCameraManager::GetCameraStruct()
{
	return &m_CameraStruct;
}

void SPCameraManager::Adjust()
{
	float fTerrainSX	= g_pTerrainManager->GetSX();
	float fTerrainSY	= g_pTerrainManager->GetSY();
	SPCameraStruct*	pCameraStruct	= GetCameraStruct();

	//[xialin 2008/08/06]add >>>
	if ( g_pSubGameManager->GetGameState() == GS_CASHSHOP )
	{
		if(pCameraStruct->fOffsetX < -CASHSHOP_OFFSETCAMX ) pCameraStruct->fOffsetX = -CASHSHOP_OFFSETCAMX;
		if(pCameraStruct->fOffsetY < -CASHSHOP_OFFSETCAMY ) pCameraStruct->fOffsetY = -CASHSHOP_OFFSETCAMY;

		if(pCameraStruct->fOffsetX > fTerrainSX - g_pVideo->GetScreenLenX() + CASHSHOP_OFFSETCAMX) 
			pCameraStruct->fOffsetX = fTerrainSX - g_pVideo->GetScreenLenX() + CASHSHOP_OFFSETCAMX;
		if(pCameraStruct->fOffsetY > fTerrainSY - g_pVideo->GetScreenLenY()) 
			pCameraStruct->fOffsetY = fTerrainSY - g_pVideo->GetScreenLenY();
	}
	//[xialin 2008/08/06]add <<<
	else
	{
		if(pCameraStruct->fOffsetX < 0 ) pCameraStruct->fOffsetX = 0;
		if(pCameraStruct->fOffsetY < 0 ) pCameraStruct->fOffsetY = 0;

		if(pCameraStruct->fOffsetX > fTerrainSX - g_pVideo->GetScreenLenX()) 
			pCameraStruct->fOffsetX = fTerrainSX - g_pVideo->GetScreenLenX();
		if(pCameraStruct->fOffsetY > fTerrainSY - g_pVideo->GetScreenLenY()) 
			pCameraStruct->fOffsetY = fTerrainSY - g_pVideo->GetScreenLenY();
	}
}

void SPCameraManager::Process(float fTime)
{
	SPManager::Process(fTime);

	switch(m_Focus) {
	case FOCUS_FREE_CAMERA:
		ProcessFreeCamera();
		break;
	case FOCUS_CHAR:
		ProcessChar();
		break;
	default:
		break;
	}

}


void SPCameraManager::ProcessFreeCamera()
{
	SPInputStruct*		pInputStruct	= g_pInputManager->GetInputStruct();
	SPCameraStruct*		pCameraStruct	= GetCameraStruct();
	static bool bBigStep = true;

	if(pInputStruct->ikLShift.InputState == INPUT_PRESS) {
		if(bBigStep) {
			pCameraStruct->fAccelX = 8.f; 
			pCameraStruct->fAccelY = 8.f; 
		} else {
			pCameraStruct->fAccelX = 2.f; 
			pCameraStruct->fAccelY = 2.f;
		}
		bBigStep = !bBigStep;
	}	
	
	if(pInputStruct->ikArrowDown.InputState == INPUT_PRESS_REPEAT)  
		pCameraStruct->fOffsetY += pCameraStruct->fAccelY;

	if(pInputStruct->ikArrowUp.InputState == INPUT_PRESS_REPEAT)  
		pCameraStruct->fOffsetY -= pCameraStruct->fAccelY;

	if(pInputStruct->ikArrowLeft.InputState == INPUT_PRESS_REPEAT)  
		pCameraStruct->fOffsetX -= pCameraStruct->fAccelX;

	if(pInputStruct->ikArrowRight.InputState == INPUT_PRESS_REPEAT)  
		pCameraStruct->fOffsetX += pCameraStruct->fAccelX;

	Adjust();
}


void SPCameraManager::ProcessChar()
{	
	SPCameraStruct*		pCameraStruct	= GetCameraStruct();
	SPPlayer* pPlayer = NULL;

	//[xialin 2008/08/06] add
	if ( g_pSubGameManager->GetGameState() == GS_CASHSHOP )
	{
		g_pInterfaceManager->SPChildWndSendMessage(WIID_CASHSHOP, SPIM_CASHSHOP_GETPLAYER, (WPARAM)&pPlayer, 0);
	}
	else
	{
		pPlayer = g_pGOBManager->GetLocalPlayer();
	}
	if(pPlayer == NULL) 
		return;


	//FACING Facing = pPlayer->GetDirection();	
	//if(Facing == FACING_LEFT) {
	//	pCameraStruct->fOffsetX = pPlayer->GetPosX() - (g_pVideo->GetScreenLenX()* 1/2); 
	//} else {
	//	pCameraStruct->fOffsetX = pPlayer->GetPosX() - (g_pVideo->GetScreenLenX()* 1/2); 
	//}

	//pCameraStruct->fOffsetY = pPlayer->GetPosY() - (g_pVideo->GetScreenLenY()/2);

	//Adjust();

	//return;


	FACING Facing = pPlayer->GetDirection();	
	float fTargetVelocityX = pPlayer->GetCoordPhysics()->GetCurVelocityX();	
	float fTargetVelocityY = pPlayer->GetCoordPhysics()->GetCurVelocityY();	
	float TargetX;
	float TargetY;

	//if(Facing == FACING_LEFT) {
	//	TargetX = pPlayer->GetPosX() - (g_pVideo->GetScreenLenX()* 3/4); 			
	//} else {
	//	TargetX = pPlayer->GetPosX() - (g_pVideo->GetScreenLenX()* 1/4); 			
	//}
	//TargetY =  pPlayer->GetPosY() - (g_pVideo->GetScreenLenY()* 1/2);

	//float fDist = (pCameraStruct->fOffsetX - TargetX);
	//float fDivide;
	
	if(Facing == FACING_LEFT) {
		TargetX = pPlayer->GetPosX() - (g_pVideo->GetScreenLenX()* 1/2); 			
	} else {
		TargetX = pPlayer->GetPosX() - (g_pVideo->GetScreenLenX()* 1/2); 			
	}
	
	//[xialin 2008/08/06] edit
	//TargetY =  pPlayer->GetPosY() - (g_pVideo->GetScreenLenY()* 2/3);
	if ( g_pSubGameManager->GetGameState() == GS_CASHSHOP )
	{
		TargetY =  pPlayer->GetPosY() - (g_pVideo->GetScreenLenY()* 4/5);
	}
	else
	{
		TargetY =  pPlayer->GetPosY() - (g_pVideo->GetScreenLenY()* 2/3);
	}

	float fDist = (pCameraStruct->fOffsetX - TargetX);

	pCameraStruct->fOffsetX -= fDist;
	//if(abs(fDist) > 800) {		
	//	pCameraStruct->fOffsetX -= fDist;
	//} else if(abs(fDist) >= 700) {		
	//	pCameraStruct->fOffsetX -= (fDist/1.f);
	//} else if(abs(fDist) >= 600) {		
	//	pCameraStruct->fOffsetX -= (fDist/1.f);
	//} else if(abs(fDist) >= 500) {	
	//	pCameraStruct->fOffsetX -= (fDist/1.f);
	//} else if(abs(fDist) >= 400) {		
	//	pCameraStruct->fOffsetX -= (fDist/1.f);
	//} else if(abs(fDist) >= 300) {		
	//	pCameraStruct->fOffsetX -= (fDist/1.f);
	//} else if(abs(fDist) >= 200) {	
	//	pCameraStruct->fOffsetX -= (fDist/1.f);
	//} else if(abs(fDist) >= 100) {	
	//	pCameraStruct->fOffsetX -= (fDist/1.f);
	//} else {
	//	pCameraStruct->fOffsetX -= (fDist/1.f);
	//}

	

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	

	//*********************************************
	//if(abs(fDist) > 800) {		
	//	pCameraStruct->fOffsetX -= fDist;
	//} else {
	//	fDivide = abs(abs(fDist) - 100.f) != 0 ? abs(abs(fDist) - 100.f) : 1;		
	//	pCameraStruct->fOffsetX -= (fDist/fDivide);
	//	if(Facing == FACING_LEFT) {
	//		if(pCameraStruct->fOffsetX > pPlayer->GetPosX() - (g_pVideo->GetScreenLenX()* 1/2)) {
	//			pCameraStruct->fOffsetX = pPlayer->GetPosX() - (g_pVideo->GetScreenLenX()* 1/2);
	//		}			
	//	} else {
	//		if(pCameraStruct->fOffsetX < pPlayer->GetPosX() - (g_pVideo->GetScreenLenX()* 1/2)) {
	//			pCameraStruct->fOffsetX = pPlayer->GetPosX() - (g_pVideo->GetScreenLenX()* 1/2);
	//		}			
	//	}
	//}



	//if(abs(fDist) < 800) {		
	//	pCameraStruct->fOffsetX -= fDist;
	//} else if(abs(fDist) < 400) {		
	//	pCameraStruct->fOffsetX -= (fDist/2);
	//} else if(abs(fDist) < 200) {		
	//	pCameraStruct->fOffsetX -= (fDist/2);
	//} else if(abs(fDist) < 100) {	
	//	pCameraStruct->fOffsetX -= (fDist/4);
	//} else /*if(abs(fDist) < 50)*/ {	
	//	pCameraStruct->fOffsetX -= (fDist);
	//} 


	fDist = (pCameraStruct->fOffsetY - TargetY);
	//pCameraStruct->fOffsetY -= fDist;

	if(abs(fDist) > 400) {		
		//pCameraStruct->fOffsetY -= (fDist / 2.f);
		pCameraStruct->fOffsetY -= fDist;
	} else if(abs(fDist) > 200) {		
		//pCameraStruct->fOffsetY -= (fDist / 3.f);
		pCameraStruct->fOffsetY -= fDist;
	} else if(abs(fDist) > 100) {		
		pCameraStruct->fOffsetY -= (fDist / 5.f);
		//pCameraStruct->fOffsetY -= fDist;
	} else if(abs(fDist) > 50) {		
		pCameraStruct->fOffsetY -= (fDist / 10.f);
	} else {
		pCameraStruct->fOffsetY -= (fDist / 20.f);
	}

	//if(abs(fTargetVelocityY) > 0.02f) {
	//	if(fDist > 400) {		
	//		pCameraStruct->fOffsetY -= (fDist / 2.f);
	//	} else if(fDist > 200) {		
	//		pCameraStruct->fOffsetY -= (fDist / 5.f);
	//	} else if(fDist > 100) {		
	//		pCameraStruct->fOffsetY -= (fDist / 10.f);
	//	} else if(fDist > 50) {		
	//		pCameraStruct->fOffsetY -= (fDist / 20.f);
	//	} else {
	//		pCameraStruct->fOffsetY -= (fDist / 40.f);
	//	}
	//} else {
	//	if(fDist > 400) {		
	//		pCameraStruct->fOffsetY -= (fDist / 2.f);
	//	} else if(fDist > 200) {		
	//		pCameraStruct->fOffsetY -= (fDist / 5.f);
	//	} else if(fDist > 100) {		
	//		pCameraStruct->fOffsetY -= (fDist / 20.f);
	//	} else if(fDist > 50) {		
	//		pCameraStruct->fOffsetY -= (fDist / 40.f);
	//	} else {
	//		pCameraStruct->fOffsetY -= (fDist / 80.f);
	//	}
	//}
	Adjust();

	return;
}










