// Copyright (C) DURAGON
//***************************************************************************
// System Name : GUILD LIST WINDOW
// Comment     : 
// Creation    : DURAGON 2007-4-04    11:18 
//***************************************************************************

#include "SPCommon.h"
#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowButton.h"

#include <algorithm>

#include <WinSock2.h>
#include "Packet.h"
#include "PacketHandler.h"
#include "SockWnd.h"
#include "SPNetworkManager.h"

#include "SPGameObjectDEF.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGameObject.h"
#include "SPGOBCluster.h"
#include "SPMOBCluster.h"
#include "SPGOBModelUnit.h"
#include "SPGOBStatus.h"
#include "SPMonsterModelUnit.h"
#include "SPGOBManager.h"
#include "SPPlayer.h"
#include "SPPlayerInvenArchive.h"
//
//#include "SPGOBClusterDef.h"
//#include "SPAvatarModelUnitDef.h"
//#include "SPGOBManager.h"
//#include "SPGOBStatus.h"
//#include "SPGOBModel.h"
//#include "SPGOBModelUnit.h"
//#include "SPAvatarModelUnit.h"

//#include "SPGameObject.h"
//#include "SPGOBStatus.h"
//#include "SPGOBCoordPhysics.h"
//#include "SPPlayer.h"

#include "SPItem.h"
#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPItemCluster.h"

#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"
#include "SPResourceManager.h"
//#include "SPNoticeBox.h"

#include "SPUtil.h"
#include "SPDebug.h"

#include "SPGuildLDT.h"
#include "SPGuildArchive.h"
#include "SPCropArchive.h"
#include "SPWindowCrops.h"
#include "GlobalDefines_Share.h"
#include "_Error_log.h"



SPWindowGuildCrops::SPWindowGuildCrops(WND_ID WndClassID, INSTANCE_ID InstanceID)
: SPWindow(WndClassID, InstanceID)
{
	Init();
}

SPWindowGuildCrops::SPWindowGuildCrops(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent)
: SPWindow(WNDID_CROPS, InstanceID, iX, iY, iCX, iCY, pParent)	
{
	Init();
}

SPWindowGuildCrops::~SPWindowGuildCrops()
{
	Clean();
}

void SPWindowGuildCrops::Init()
{
	//m_pkModelUnit		= NULL ;
	//m_iCropImageID		= 0 ;
	m_strToolTip		=	"[SPWindowGuildCrops]";

	g_pVideo->GetTextureMgr()->LoadTexture( "DATA/INTERFACE/CONCEPT/UI105.PNG" , &m_pBGTexture );

	SPWindowStatic*	pWindowStatic;
	SPWindowStatic*	pWindowStaticBG;
	SPWindowButton*	pWindowButton;

	SetRect( &m_rcCropsBGSrc[0]	, 482 , 316 , 486 , 320 );		// 7
	SetRect( &m_rcCropsBGSrc[1]	, 488 , 316 , 490 , 320 );		// 8
	SetRect( &m_rcCropsBGSrc[2]	, 492 , 316 , 496 , 320 );		// 9
	SetRect( &m_rcCropsBGSrc[3]	, 482 , 322 , 486 , 324 );		// 4
	SetRect( &m_rcCropsBGSrc[4]	, 488 , 322 , 490 , 324 );		// 5
	SetRect( &m_rcCropsBGSrc[5]	, 492 , 322 , 496 , 324 );		// 6
	SetRect( &m_rcCropsBGSrc[6]	, 482 , 311 , 486 , 315 );		// 1
	SetRect( &m_rcCropsBGSrc[7]	, 488 , 311 , 490 , 315 );		// 2
	SetRect( &m_rcCropsBGSrc[8]	, 492 , 311 , 496 , 315 );		// 3
	SetRect( &m_rcCropsBGSrc[9]	, 486 , 387 , 488 , 388 );		// 서브 백그라운드 경계라인
	SetRect( &m_rcCropsBGSrc[10], 488 , 307 , 490 , 309 );		// 흰색 서브 백그라운드

	SetRect( &m_rcCropsBGSrc[11], 207 , 112 , 321 , 136 );		// 타이틀
	SetRect( &m_rcCropsBGSrc[12], 139 , 159 , 196 , 170 );		// 비료TEXT
	SetRect( &m_rcCropsBGSrc[13], 139 , 171 , 196 , 182 );		// 촉진제TEXT
	SetRect( &m_rcCropsBGSrc[14], 369 , 227 , 491 , 238 );		// 성장시간
	SetRect( &m_rcCropsBGSrc[15], 369 , 239 , 491 , 250 );		// 비료시간
	SetRect( &m_rcCropsBGSrc[16], 369 , 215 , 491 , 226 );		// 촉진시간
	ReposOwnImage() ;

	// Cutin 창에는 필요없음
	//pWindowButton	=	new	SPWindowButton( WIID_CROPS_CLOSE , 276 , 9 , 12 , 12 , this );
	//pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 217 );
	//pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 230 );
	//pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 243 );
	//pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 247 , 256 );
	//pWindowButton = NULL ;

	pWindowStaticBG	=	new SPWindowStatic( WIID_CROPS_NAME_BG , 18 , 43 , 261 , 36 , this );
	pWindowStaticBG->SetWindowType(WND_TYPE_LEFTBASE);
	pWindowStaticBG->SetImageNormalHead( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 327, 152, 329, 188) ;
	pWindowStaticBG->SetImageNormalBody( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 330, 152, 332, 188) ;
	pWindowStaticBG->SetImageNormalTail( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 333, 152, 335, 188) ;


	pWindowStatic	=	new SPWindowStatic( WIID_CROPS_ICON , 34 , 45 , 32 , 32 , this );
	pWindowStatic->SetImage			( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 139 , 205 );
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CROPS_NAME , 74 , 56 , 197 , 12 , this );
	pWindowStatic->SetFontColor(RGBA(49, 76, 97, 255));
	pWindowStatic->SetFormat(DT_VCENTER | DT_CENTER);
	//pWindowStatic->SetFont(FONT_12_BOLD) ;
	//pWindowStatic->SetWindowText(" Lv.1 작물이름나옴 ") ;
	pWindowStatic	=	NULL;

	//////////////////////////////////////////////////////////////////////////
	// 작물 Drag & Drop영역 
	pWindowStatic	=	new SPWindowStatic( WIID_CROPS_MANURE , 16 , 87 , 36 , 36 , this );
	pWindowStatic->SetImage			( "DATA/INTERFACE/CONCEPT/UI106.PNG" , 475 , 290 );
	pWindowStatic->SetImage			( "DATA/INTERFACE/CONCEPT/UI106.PNG" , 475 , 327 );		// Drag Over
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CROPS_HASTEN , 16 , 138 , 36 , 36 , this );
	pWindowStatic->SetImage			( "DATA/INTERFACE/CONCEPT/UI106.PNG" , 475 , 364 );
	pWindowStatic->SetImage			( "DATA/INTERFACE/CONCEPT/UI106.PNG" , 475 , 401 );		// Drag Over
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CROPS_TIME_BAR , 32 , 249 , 4 , 45 , this );
	pWindowStatic->SetWindowType(WND_TYPE_TOPBASE);
	pWindowStatic->SetImageNormalHead( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 120, 133, 124, 143) ;
	pWindowStatic->SetImageNormalBody( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 120, 147, 124, 158) ;
	pWindowStatic->SetImageNormalTail( "DATA/INTERFACE/CONCEPT/UI102.PNG" , 120, 160, 124, 170) ;
	pWindowStatic	=	NULL;

	pWindowStatic	=	new SPWindowStatic( WIID_CROPS_GROWTH_TIME , 136 , 247 , 126 , 12 , this );
	pWindowStatic->SetFontColor(RGBA(49, 76, 97, 255));
	pWindowStatic->SetFormat(DT_VCENTER | DT_LEFT);
	//pWindowStatic->SetWindowText("000시간 00분 00초 ") ;
	pWindowStatic	=	NULL;
	pWindowStatic	=	new SPWindowStatic( WIID_CROPS_MANURE_TIME , 136 , 264 , 126 , 12 , this );
	pWindowStatic->SetFontColor(RGBA(49, 76, 97, 255));
	pWindowStatic->SetFormat(DT_VCENTER | DT_LEFT);
	//pWindowStatic->SetWindowText("000시간 00분 00초 ") ;
	pWindowStatic	=	NULL;
	pWindowStatic	=	new SPWindowStatic( WIID_CROPS_HASTEN_TIME , 148 , 281 , 126 , 12 , this );
	pWindowStatic->SetFontColor(RGBA(49, 76, 97, 255));
	pWindowStatic->SetFormat(DT_VCENTER | DT_LEFT);
	//pWindowStatic->SetWindowText("000시간 00분 00초 ") ;
	pWindowStatic	=	NULL;

	pWindowButton	=	new	SPWindowButton( WIID_CROPS_ELIMINATE , 106 , 302 , 76 , 17 , this );
	pWindowButton->SetImage			( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 405 , 265 );
	pWindowButton->SetImageHit		( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 405 , 283 );
	pWindowButton->SetImagePush		( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 405 , 301 );
	pWindowButton->SetImageDisable	( "DATA/INTERFACE/CONCEPT/UI105.PNG" , 405 , 319 );
	pWindowButton = NULL ;

	//////////////////////////////////////////////////////////////////////////
	// 작물 이미지 - Size때문에 다른 컨트롤에 영향 없도록 마지막 생성
	pWindowStatic	=	new SPWindowStatic( WIID_CROPS_IMAGE , 20 , -7 , 256 , 256 , this );
	pWindowStatic	=	NULL;
}





void SPWindowGuildCrops::ReposOwnImage()
{
	//if( m_pkModelUnit ) {
	//	float	fParsingX, fParsingY;
	//	WPARAM	wParsingX, wParsingY; 
	//	fParsingX = (float)m_iAX + 153.0f;
	//	fParsingY = (float)m_iAY + 237.0f;
	//	DataConvert<float,WPARAM>(fParsingX, wParsingX);
	//	DataConvert<float,WPARAM>(fParsingY, wParsingY);
	//	m_pkModelUnit->SPSendMessage(MV_SET_RENDERPOSX, wParsingX );
	//	m_pkModelUnit->SPSendMessage(MV_SET_RENDERPOSY, wParsingY );
	//}

	SetRect( &m_rcCropsBGDst[0]		, m_iAX	 		, m_iAY	 		, m_iAX + 4		, m_iAY + 4 );		// 7
	SetRect( &m_rcCropsBGDst[1]		, m_iAX	+ 4		, m_iAY	 		, m_iAX + 293	, m_iAY + 4 );		// 8
	SetRect( &m_rcCropsBGDst[2]		, m_iAX	+ 293	, m_iAY	 		, m_iAX + 297	, m_iAY + 4 );		// 9
	SetRect( &m_rcCropsBGDst[3]		, m_iAX	 		, m_iAY	+ 4		, m_iAX + 4		, m_iAY + 333 );	// 4
	SetRect( &m_rcCropsBGDst[4]		, m_iAX	+ 4		, m_iAY	+ 4		, m_iAX + 293	, m_iAY + 333 );	// 5
	SetRect( &m_rcCropsBGDst[5]		, m_iAX	+ 293	, m_iAY	+ 4		, m_iAX + 297	, m_iAY + 333 );	// 6
	SetRect( &m_rcCropsBGDst[6]		, m_iAX	 		, m_iAY	+ 333	, m_iAX + 4		, m_iAY + 337 );	// 1
	SetRect( &m_rcCropsBGDst[7]		, m_iAX	+ 4		, m_iAY	+ 333	, m_iAX + 293	, m_iAY + 337 );	// 2
	SetRect( &m_rcCropsBGDst[8]		, m_iAX	+ 293	, m_iAY	+ 333	, m_iAX + 297	, m_iAY + 337 );	// 3
	SetRect( &m_rcCropsBGDst[9]		, m_iAX	+ 2		, m_iAY	+ 26	, m_iAX + 295	, m_iAY + 333 );	// Line
	SetRect( &m_rcCropsBGDst[10]	, m_iAX	+ 2		, m_iAY	+ 27	, m_iAX + 295	, m_iAY + 333 );	// 배경

	SetRect( &m_rcCropsBGDst[11]	, m_iAX	+ 8		, m_iAY	+ 2		, m_iAX + 122	, m_iAY + 26 );		// 타이틀
	SetRect( &m_rcCropsBGDst[12]	, m_iAX	+ 5		, m_iAY	+ 125	, m_iAX + 62	, m_iAY + 136 );	// 비료TEXT
	SetRect( &m_rcCropsBGDst[13]	, m_iAX	+ 5		, m_iAY	+ 176	, m_iAX + 62	, m_iAY + 187 );	// 촉진제TEXT
	SetRect( &m_rcCropsBGDst[14]	, m_iAX	+ 40	, m_iAY	+ 247	, m_iAX + 162	, m_iAY + 258 );	// 성장시간
	SetRect( &m_rcCropsBGDst[15]	, m_iAX	+ 40	, m_iAY	+ 264	, m_iAX + 162	, m_iAY + 275 );	// 비료시간
	SetRect( &m_rcCropsBGDst[16]	, m_iAX	+ 40	, m_iAY	+ 281	, m_iAX + 162	, m_iAY + 292 );	// 촉진시간
}



void SPWindowGuildCrops::RefreshRelationCoord()
{
	SPWindow::RefreshRelationCoord();
	ReposOwnImage();
}

void SPWindowGuildCrops::Clean()
{
	SPWindow::Clean();

	SAFE_RELEASE( m_pBGTexture );
}



void SPWindowGuildCrops::Show(bool bWithChild)
{
	if( g_pGOBManager->GetGuildArchive()->GetBasicAvailable() == false )	
		return ;
	if( g_pGOBManager->GetGuildArchive()->hasCorps() == false )		
		return ;

	if(m_bShow)
		g_pInterfaceManager->RegisterSoundUnit("DATA/SOUND/SYSTEM_CLOSE_WINDOW.WAV");

	SPWindow::Show(bWithChild);

	ShowCropInfo() ;
}


void SPWindowGuildCrops::Hide(bool bSendServer)
{
	if(m_bShow)
		g_pInterfaceManager->RegisterSoundUnit("DATA/SOUND/SYSTEM_CLOSE_WINDOW.WAV");

	// CleanCropMonster() ;
	SPWindow::Hide(bSendServer);

	KillFocus() ;
}


void SPWindowGuildCrops::Process(float fTime)
{
	if( m_bShow == FALSE ) return;

	//if( m_pkModelUnit )		
	//{
	//	m_fAnimationTime -= fTime;
	//	if( m_fAnimationTime <= 0.0f )
	//		ChangeAnimation(m_iCropImageID);

	//	//m_iAX + 31 , m_iAY + 150
	//	float	fParsingX, fParsingY;
	//	WPARAM	wParsingX, wParsingY; 
	//	fParsingX = (float)m_iAX + 153.0f;
	//	fParsingY = (float)m_iAY + 237.0f;
	//	DataConvert<float,WPARAM>(fParsingX, wParsingX);
	//	DataConvert<float,WPARAM>(fParsingY, wParsingY);
	//	m_pkModelUnit->SPSendMessage(MV_SET_RENDERPOSX, wParsingX );
	//	m_pkModelUnit->SPSendMessage(MV_SET_RENDERPOSY, wParsingY );
	//	m_pkModelUnit->Process(fTime);
	//}

	SPWindow::Process( fTime );
}



void SPWindowGuildCrops::Render(float fTime)
{
	if( m_bShow == FALSE ) return;

	g_pVideo->Flush();

	if( m_pBGTexture )		// BackGround Image Process
	{
		for( int i = 0 ; i < GUILD_CROP_IMG ; i++ )		{
			m_pBGTexture->RenderTexture( &m_rcCropsBGDst[i] , &m_rcCropsBGSrc[i] );
		}
		g_pVideo->Flush();
	}

	//if(m_pkModelUnit)
	//	m_pkModelUnit->Render(fTime);

	SPWindow::Render(fTime);
}


/**
	작물 정보 보여주기
*/
void SPWindowGuildCrops::ShowCropInfo()
{
	int		iCropLevel = -1 ;
	POINT	ptSrcPos;
	SPWindowStatic*	pStatic ;
	SPWindowButton* pButton ;
	SPTexture* pTexture = NULL ;
	TCHAR	szText[64] ;

	if( g_pGOBManager->GetGuildArchive() == NULL )		return ;

	// Crop Icon
	pStatic = (SPWindowStatic*)Find(WIID_CROPS_ICON, true) ;
	pStatic->SPSendMessage(SPIM_SET_IMAGECLEAR);
	pTexture = g_pItemCluster->GetItemTexture(g_pGOBManager->GetGuildArchive()->GetCropArchive()->GetUseItemForIcon(), ptSrcPos);
	pStatic->SetImage(pTexture, ptSrcPos.x, ptSrcPos.y) ;

	// Crop Name
	iCropLevel = g_pGOBManager->GetGuildArchive()->GetCropArchive()->GetCropLevel() ;
	if( iCropLevel < 0 || iCropLevel >= CROP_LEVEL_MAX ){
		ErrorFLogs->Error_Log("##CropLevel is Invalid[%d]", iCropLevel) ;
		return ;
	}
	pStatic = (SPWindowStatic*)Find(WIID_CROPS_NAME, true) ;
	sprintf(szText, "Lv.%d %s", iCropLevel+1, g_pGOBManager->GetGuildArchive()->GetCropArchive()->GetCropName()) ;
	pStatic->SetWindowText(szText) ;

	pButton = (SPWindowButton*)Find(WIID_CROPS_ELIMINATE, true) ;
	g_pGOBManager->GetGuildArchive()->IsMaster() ? pButton->Show() : pButton->Hide() ;

	// Crop Image
	UpdateCropImage(iCropLevel) ;
	UpdateCropTime() ;
}

/**
	CropImage Update가 필요한경우 CropMonster를 삭제후 재 생성한다.
*/
void SPWindowGuildCrops::UpdateCropImage(int iCropLevel)
{
	UINT32	iCropDlgImageID ;
	SPWindowStatic*	pStatic ;
	SPTexture* pTexture = NULL ;

	iCropDlgImageID = g_pGOBManager->GetGuildArchive()->GetCropArchive()->GetCropDlgImageID(iCropLevel) ;
	if( iCropDlgImageID == 0 )	{
		ErrorFLogs->Error_Log("##CropImageID is Zero") ;
		return ;
	}
	pStatic = (SPWindowStatic*)Find(WIID_CROPS_IMAGE, true) ;
	pStatic->SPSendMessage(SPIM_SET_IMAGECLEAR);
	pStatic->SetImage(g_pResourceManager->GetGlobalFileName(iCropDlgImageID), 0, 0) ;

	//CleanCropMonster() ;
	//AddCropMonster() ;
}


/**
	[작물상태] 작물 성장시간 업데이트
*/
void SPWindowGuildCrops::UpdateCropTime()
{
	char* pTimeStr ;
	char szTime[64] ;
	SPWindowStatic*	pStatic ;
	pTimeStr = szTime ;

	if( g_pGOBManager->GetGuildArchive() == NULL )		return ;
	//WIID_GUILD_CROPS_GROWTH_TIME
	pStatic = (SPWindowStatic*)Find(WIID_CROPS_GROWTH_TIME, true) ;
	ComputeTimeString(g_pGOBManager->GetGuildArchive()->GetCropArchive()->GetRemainTime(), pTimeStr) ;
	pStatic->SetWindowText(pTimeStr);

	//WIID_GUILD_CROPS_MANURE_TIME
	pStatic = (SPWindowStatic*)Find(WIID_CROPS_MANURE_TIME, true) ;
	ComputeTimeString(g_pGOBManager->GetGuildArchive()->GetCropArchive()->GetNourishTime(), pTimeStr) ;
	pStatic->SetWindowText(pTimeStr);

	//WIID_GUILD_CROPS_HASTEN_TIME
	pStatic = (SPWindowStatic*)Find(WIID_CROPS_HASTEN_TIME, true) ;
	ComputeTimeString(g_pGOBManager->GetGuildArchive()->GetCropArchive()->GetHastenSumTime(), pTimeStr) ;
	pStatic->SetWindowText(pTimeStr);
}


/**
	시간 값으로 GlobalString의 TimeString을 만들어준다.
*/
bool SPWindowGuildCrops::ComputeTimeString(UINT32 iTime, char* &pstrTime)
{
	int iHour, iMin, iSec ;
	if( iTime == 0 )		{
		iHour	= 0 ;
		iMin	= 0 ;
		iSec	= 0 ;
	}
	else	{
		iHour	= iTime / 3600 ;
		iMin	= ( iTime % 3600 ) / 60 ;
		iSec	= iTime % 60 ;
	}
	sprintf(pstrTime, g_pResourceManager->GetGlobalString(27000076), iHour, iMin, iSec);
	return true ;
}



/**
	화면에 출력될 작물NPC이미지의 생성과 삭제
*/
//bool SPWindowGuildCrops::AddCropMonster()
//{
//	UINT32 iCropImageID = 0 ;
//	int iCropLevel = 0 ;
//	if( m_pkModelUnit != NULL )
//	{
//		SAFE_DELETE(m_pkModelUnit) ;
//	}
//
//	iCropLevel = g_pGOBManager->GetGuildArchive()->GetCropArchive()->GetCropLevel() ;
//	iCropImageID = g_pGOBManager->GetGuildArchive()->GetCropArchive()->GetCropImageID(iCropLevel) ;
//	if( iCropImageID == 0 )
//	{
//		ErrorFLogs->Error_Log("##CropImageID is Zero") ;
//		return false ;
//	}
//
//	m_fAnimationTime = 0.0f;
//	m_iCropImageID	= iCropImageID ;
//	m_pkModelUnit = new SPMonsterModelUnit(NULL) ;
//	if( m_pkModelUnit )
//	{
//		((SPMonsterModelUnit*)m_pkModelUnit)->SetNPC(m_iCropImageID);
//		m_pkModelUnit->Init();
//
//		//Animation 변경
//		ChangeAnimation(m_iCropImageID);
//	}	
//	return true ;
//}
//
//void SPWindowGuildCrops::CleanCropMonster()
//{
//	m_iCropImageID = 0 ;
//	SAFE_DELETE(m_pkModelUnit);
//}
//
//
//void SPWindowGuildCrops::ChangeAnimation(UINT32 iCropImageID)
//{
//	if( m_pkModelUnit == NULL )
//		return;
//
//	SPMOBAnimation* pkAnimation;
//	int iIndex = 0;
//
//	iIndex = (rand() % 3) + ANI_NPC_STAND01;
//	pkAnimation = g_pMOBCluster->GetAnimation(iCropImageID, (ANIMATION_INDEX)iIndex);
//
//	if( !pkAnimation )
//		iIndex = ANI_NPC_STAND01;
//
//	m_pkModelUnit->SPSendMessage(MV_SET_ANIMATION_INDEX, (ANIMATION_INDEX)iIndex, 1);
//	m_pkModelUnit->SPSendMessage(MV_GET_ANI_ACCUMUL_TIME, (WPARAM)&m_fAnimationTime);
//	m_pkModelUnit->SPSendMessage(MV_SET_ANI_LOOP, false);
//}





/**
	타입에 따른 아이템이 왔는지 여부를 확인해 준다.
	@return 있으면:SPItem* , 없으면 NULLL
*/
SPItem* SPWindowGuildCrops::GetDragItem()
{
	SPPlayerInvenArchive* pInven = NULL;
	if(g_pGOBManager->GetLocalPlayer())
		pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();
	else 
		return NULL;

	//픽업 상태가 아닌경우 넘어간다
	if(pInven->m_bPickup == false) {
		return NULL;
	}	

	SPSelectItem SelectItem = pInven->GetPickupItem();
	SPItem* pItem = pInven->GetInventoryItem(SelectItem.m_iContainer, SelectItem.m_iSlotIndex);	

	// 아이템이 있는지
	if(pItem == NULL || pItem->GetItemStatus() == NULL || pItem->GetItemStatus()->GetItemID() == 0) {
		return NULL;
	}

	// 이벤트아이템이 아닌경우 -> 원래는 소비 (INVENTORY_CONSUME)
	if( SelectItem.GetContainerType() != INVENTORY_EVENT ) {
		return NULL ;
	}


	return pItem ;
}


SPIMESSAGE_OBJ_MAP_BEGIN( SPWindowGuildCrops )
SPIMESSAGE_COMMAND(	SPIM_CURSOR_ENTER		,	OnCursorEnter	)
SPIMESSAGE_COMMAND(	SPIM_CURSOR_OUT			,	OnCursorOut		)
SPIMESSAGE_COMMAND(	SPIM_CURSOR_MOVE		,	OnCursorMove	)

SPIMESSAGE_COMMAND(	SPIM_LBUTTON_UP			,	OnLButtonUp		)
SPIMESSAGE_COMMAND(	SPIM_LBUTTON_DOWN		,	OnLButtonDown	)
SPIMESSAGE_COMMAND(	SPIM_RBUTTON_UP			,	OnRButtonUp		)

SPIMESSAGE_COMMAND(	SPIM_GUILD_CROP_UPDATE		,	OnGuildCropUpdate		)	// 작물 UPdate

SPIMESSAGE_CONTROL(	SPIM_CURSOR_ENTER,		WIID_CROPS_MANURE, 				OnManureCursorEnter )
SPIMESSAGE_CONTROL(	SPIM_CURSOR_OUT,		WIID_CROPS_MANURE, 				OnManureCursorOut )
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_CROPS_MANURE, 				OnManureLButtonUp )

SPIMESSAGE_CONTROL(	SPIM_CURSOR_ENTER,		WIID_CROPS_HASTEN, 				OnHastenCursorEnter )
SPIMESSAGE_CONTROL(	SPIM_CURSOR_OUT,		WIID_CROPS_HASTEN, 				OnHastenCursorOut )
SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_CROPS_HASTEN, 				OnHastenLButtonUp )

SPIMESSAGE_CONTROL(	SPIM_LBUTTON_UP,		WIID_CROPS_ELIMINATE, 			OnCropEliminate )

SPIMESSAGE_CONTROL_RANGE(SPIM_LBUTTON_UP,		WIID_CROPS_TITLE, WIID_CROPS_ELIMINATE,		OnClickTest )


SPIMESSAGE_OBJ_MAP_END	( SPWindow	)


/**
	작물 정보 및 이미지 업데이트
*/
int	SPWindowGuildCrops::OnGuildCropUpdate( WPARAM wParam , LPARAM lParam )
{
	if( !IsShow() )		return 0 ;
	if( g_pGOBManager->GetGuildArchive()->hasCorps() ) 
	{
		if( wParam == CROP_UPDATE_TIME )		UpdateCropTime() ;
		if( wParam == CROP_UPDATE_LEVEL )		ShowCropInfo() ;
	}
	return 1 ;
}


/**
	1) Drag 되는 아이템을 확인
	2) 비료 아이템인지 확인
	3) 이미지를 Drag-On 으로 바꿔준다
*/
int	SPWindowGuildCrops::OnManureCursorEnter	( WPARAM wParam, LPARAM lParam )
{
	//SPItem* pItem = GetDragItem() ;
	//if( pItem == NULL )	{
	//	return 1 ;
	//}

	// Drag-on 이미지로 변경
	SPWindow* pkWindow = Find(WIID_CROPS_MANURE, true);
	if( pkWindow )
		pkWindow->SPSendMessage(SPIM_SET_IMAGEINDEX, 1);

	return 1 ;
}

/**
	이미지를 Normal로 바꿔준다.
*/
int	SPWindowGuildCrops::OnManureCursorOut	( WPARAM wParam, LPARAM lParam )
{
	SPWindow* pkWindow = Find(WIID_CROPS_MANURE, true);
	if( pkWindow )
		pkWindow->SPSendMessage(SPIM_SET_IMAGEINDEX, 0);
	return 1 ;
}

/**
	1) 비료 아이템인지 확인
	2) 비료인경우 아이템 사용
*/
int	SPWindowGuildCrops::OnManureLButtonUp	( WPARAM wParam, LPARAM lParam )
{
	// 비료 사용
	int iRet ;
	SPItem* pDragItem = NULL ;
	pDragItem = GetDragItem() ;
	if( pDragItem != NULL )
	{
		if( ( iRet = g_pGOBManager->GetGuildArchive()->UseNourishItem(BOOSTER_TYPE_MANURE, pDragItem)) !=0 )
		{
			if( iRet == 2 )	{

			}
			else if( iRet == 3) {
				if( g_pResourceManager->GetGlobalString(27000070) )
					g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(27000070) );
			}
		}
	}
	return 1 ;
}


/**
	1) Drag 되는 아이템 확인
	2) 촉진제 아이템 인지 확인
	3) Drag-On으로 이미지 수정
*/
int	SPWindowGuildCrops::OnHastenCursorEnter	( WPARAM wParam, LPARAM lParam )
{
	//SPItem* pItem = GetDragItem() ;
	//if( pItem == NULL )	{
	//	return 1 ;
	//}

	// Drag-on 이미지로 변경
	SPWindow* pkWindow = Find(WIID_CROPS_HASTEN, true);
	if( pkWindow )
		pkWindow->SPSendMessage(SPIM_SET_IMAGEINDEX, 1);

	return 1 ;
}
/**
	Normal 이미지로 수정	WIID_CROPS_HASTEN
*/
int	SPWindowGuildCrops::OnHastenCursorOut	( WPARAM wParam, LPARAM lParam )
{
	SPWindow* pkWindow = Find(WIID_CROPS_HASTEN, true);
	if( pkWindow )
		pkWindow->SPSendMessage(SPIM_SET_IMAGEINDEX, 0);
	return 1 ;
}
/**
	1) Drag되는 아이템 확인
	2) 촉진제 아이템인지 확인
	3) 촉진제 사용
*/
int	SPWindowGuildCrops::OnHastenLButtonUp	( WPARAM wParam, LPARAM lParam )
{
	// 촉진제 사용
	SPItem* pDragItem = NULL ;
	pDragItem = GetDragItem() ;
	if( pDragItem != NULL )
	{
		if( g_pGOBManager->GetGuildArchive()->UseNourishItem(BOOSTER_TYPE_HASTEN, pDragItem) == false )
		{
			if( g_pResourceManager->GetGlobalString(27000071) )
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)g_pResourceManager->GetGlobalString(27000071) );
		}
	}
	return 1 ;
}


/**
	[작물 제거]
*/
int	SPWindowGuildCrops::OnCropEliminate	( WPARAM wParam, LPARAM lParam )
{
	if( g_pGOBManager->GetGuildArchive()->IsMaster() && g_pGOBManager->GetGuildArchive()->hasCorps() )
		g_pGOBManager->GetGuildArchive()->SendCropEliminate() ;
	return 1;
}



int SPWindowGuildCrops::OnCursorEnter( WPARAM wParam, LPARAM lParam )
{
	return 1;
}

int SPWindowGuildCrops::OnCursorOut( WPARAM wParam, LPARAM lParam )
{
	return 1;
}

int SPWindowGuildCrops::OnCursorMove( WPARAM wParam, LPARAM lParam )
{
	return 1;
}

int SPWindowGuildCrops::OnLButtonUp( WPARAM wParam, LPARAM lParam )
{
	return 1;
}

int SPWindowGuildCrops::OnLButtonDown( WPARAM wParam, LPARAM lParam )
{
	return 1;
}

int SPWindowGuildCrops::OnRButtonUp( WPARAM wParam, LPARAM lParam )
{
	return 1;
}


/**
	Click Event를 찾기 위한 Test
*/
int SPWindowGuildCrops::OnClickTest( unsigned int iID, WPARAM wParam, LPARAM lParam )
{
	return 1;
}


