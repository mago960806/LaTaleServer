#include "SPCommon.h"
#include "SPUtil.h"
#include "SPMainGameDEF.H"
#include "SPGameObjectDef.h"
#include "SPGameObject.h"

//
#include "SPMainGameDEF.H"
#include "SPSubGameManager.h"
#include "SPMainGameManager.h"
#include "SPMouseCursor.h"
#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPInterfaceManager.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPGOBModel.h"
#include "SPGOBModelUnit.h"
#include "SPMonsterModelUnit.h"
#include "SPAvatarModelUnit.h"
#include "SPGOBCluster.h"
#include "SPGameObject.h"
#include "SPGOBStatus.h"
#include "SPGOBCoordPhysics.h"
#include "SPGOBModelUnit.h"			
#include "SPPlayerEquipInfo.h"
#include "SPGOBStatus.h"			
#include "SPPlayerInvenArchive.h"	
#include "SPPlayerStatusModel.h"
#include "SPEventStatusModel.h"
#include "SPPlayerEquipInfo.h"
#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPPlayer.h"
#include "SPGuildLDT.h"
#include "SPGuildArchive.h"
#include "SPGOBManager.h"

//
#include "SPSubGameManager.h"

#include "SPRenderModel.h"

#include "SPFXDEF.h"
#include "SPFXModel.h"
#include "SPFXUnit.h"
#include "SPFXGroupUnit.h"
#include "SPFXArchive.h"

#include "SPCommandConvert.h"
#include "Packet.h"

#include "SPStage.h"
#include "SPStageManager.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPEventDEF.h"
#include "SPEvent.h"
#include "SPEventArchive.h"
#include "SPEventNpcHandler.h"
#include "SPQuestAttr.h"
#include "SPQuestArchive.h"
#include "SPQuestManager.h"
#include "SPEventManager.h"

#include "SPCashTradeManager.h"

#include "SPCheckManager.h"
#include "SPLocalizeManager.h"

#include "SPMOBCluster.h"

#include "SPDebug.h"



SPEvent::SPEvent() 
: m_pkModelUnit(NULL)
, m_pkStatusModel(NULL)
, m_pkFXArchive(NULL)
{
	Init();
}


SPEvent::SPEvent(INSTANCE_ID instanceID, CLASS_ID ClassID)
: m_pkModelUnit(NULL)
, m_pkStatusModel(NULL)
{
	SetClassID(ClassID);
	SetInstanceID(instanceID);
	Init();
}


SPEvent::~SPEvent()
{
	Destroy();
}


//////////////////////////////////////////////////////////////////////////
//virtual
void SPEvent::Init()
{
	SPGameObject::Init();
	SetNull();
	m_pkModelUnit = new SPMonsterModelUnit(this);
	m_pkFXArchive = new SPFXArchive(this);

	m_pkStatusModel = new SPEventStatusModel();
	m_pkStatusModel->Init();

	m_fAnimationTime = 0.0f;

	m_iEventQuestShow = EVENT_QUEST_SHOW_NULL;

}


void SPEvent::Destroy()
{
	m_vpCoreRequire.clear();
	DeleteAllQuest();
	SAFE_DELETE(m_pkModelUnit);
	SAFE_DELETE(m_pkStatusModel);
	SAFE_DELETE(m_pkFXArchive);
}

void SPEvent::RenderFX( float fTime , SP_FXM_RENDER_LAYER eRenderLayer )
{
	if( m_pkFXArchive != NULL )
		m_pkFXArchive->Render( fTime , eRenderLayer );
}

void SPEvent::Process(float fTime)	
{
	SPGameObject::Process(fTime);
	
	if(  g_pGOBManager->GetLocalPlayer() == NULL )
		return;

	m_fPlayerX = g_pGOBManager->GetLocalPlayer()->GetPosX();
	m_fPlayerY = g_pGOBManager->GetLocalPlayer()->GetPosY();
	
	m_fCamX = g_pMainGameManager->GetCameraManager()->GetCameraStruct()->fOffsetX;
	m_fCamY = g_pMainGameManager->GetCameraManager()->GetCameraStruct()->fOffsetY;

	//m_fMouseX = m_fCamX + g_pInput->GetMousePosX();
	//m_fMouseY = m_fCamY + g_pInput->GetMousePosY();
	m_fMouseX = m_fCamX + g_pInterfaceManager->GetCursorX();
	m_fMouseY = m_fCamY + g_pInterfaceManager->GetCursorY();

	SetRect(&m_rtEvent, m_MapEvent.rectLeft - m_fCamX, m_MapEvent.rectTop - m_fCamY, 
		m_MapEvent.rectRight - m_fCamX, m_MapEvent.rectBottom - m_fCamY);

	if( m_MapEvent.eventType == EVENT_NPC )
	{
		if( m_MapEvent.posX < m_fPlayerX )
			m_eFacing = FACING_RIGHT;
		else
			m_eFacing = FACING_LEFT;
	}

	m_fAnimationTime -= fTime;
	if( m_fAnimationTime <= 0.0f )
		ChangeAnimation();

	////
	//float	fParsingX, fParsingY;
	//WPARAM	wParsingX, wParsingY; 
	//fParsingX = m_MapEvent.posX-m_fCamX;
	//fParsingY = m_MapEvent.posY-m_fCamY;
	//DataConvert<float,WPARAM>(fParsingX, wParsingX);
	//DataConvert<float,WPARAM>(fParsingY, wParsingY);
	//m_pkModelUnit->SPSendMessage(MV_SET_RENDERPOSX, wParsingX );
	//m_pkModelUnit->SPSendMessage(MV_SET_RENDERPOSY, wParsingY );
	//m_pkModelUnit->Process(fTime);
	
	//[2006/2/22]
	if(g_pEventManager->IsEventBlock())
		return;

	//[2006/1/2]	사망한 경우
	if(g_pGOBManager->GetLocalPlayer()->IsDead()) {
		return;
	}
	
	CheckPlayerEvent();		

	ProcessImage(fTime);

	if( m_pkFXArchive != NULL )
		m_pkFXArchive->Process( fTime );
}


void SPEvent::ProcessImage(float fTime)
{
	float	fParsingX, fParsingY;
	WPARAM	wParsingX, wParsingY; 
	fParsingX = m_MapEvent.posX-m_fCamX;
	fParsingY = m_MapEvent.posY-m_fCamY;
	DataConvert<float,WPARAM>(fParsingX, wParsingX);
	DataConvert<float,WPARAM>(fParsingY, wParsingY);
	m_pkModelUnit->SPSendMessage(MV_SET_RENDERPOSX, wParsingX );
	m_pkModelUnit->SPSendMessage(MV_SET_RENDERPOSY, wParsingY );
	
	m_pkModelUnit->Process(fTime);
	SPMonsterModelUnit* pkMonsterUnit = (SPMonsterModelUnit*)m_pkModelUnit;

	bool bShadow = false;
	if( m_MapEvent.eventType == EVENT_NPC || m_MapEvent.eventType == EVENT_MAILBOX || m_MapEvent.eventType == EVENT_BEAUTY || m_MapEvent.eventType == EVENT_CROPS)
		bShadow = true;

	RECT rcRenderRgn;
	m_pkModelUnit->SPSendMessage(MV_GET_RENDER_RGN, (WPARAM)&rcRenderRgn);
	
	m_bClipping = false;
	if( (rcRenderRgn.left < 0 && rcRenderRgn.right < 0) ||
		(rcRenderRgn.left > 800 && rcRenderRgn.right > 800) ||
		(rcRenderRgn.bottom < 0 || rcRenderRgn.top > 600) )
		m_bClipping = true;

	//m_pkStatusModel->SetObjectRgn(rcRenderRgn,
	//	(int)pkMonsterUnit->GetRenderPosX(), (int)pkMonsterUnit->GetRenderPosY(), bShadow);

	m_pkStatusModel->SetObjectRgn(m_rtEvent,
		(int)pkMonsterUnit->GetRenderPosX(), (int)pkMonsterUnit->GetRenderPosY(), bShadow);
}


void SPEvent::Render(float fTime)
{	
	//EVENT_NULL,EVENT_NPC,EVENT_SAVEPOINT,EVENT_PORTAL,MAX_EVENT	

	RenderFX( fTime , FXM_RENDER_LAYER_OBJECT_BACK );

	if( !m_bClipping )
	{
		if( m_pkStatusModel &&
			(m_MapEvent.eventType == EVENT_NPC || m_MapEvent.eventType == EVENT_MAILBOX || m_MapEvent.eventType == EVENT_BEAUTY || m_MapEvent.eventType == EVENT_CROPS))
			m_pkStatusModel->Render(fTime);

		if(m_pkModelUnit)
			m_pkModelUnit->Render(fTime);
		
//#ifdef _DEBUG
//		RenderEventLine();
//#endif
	}

	//if( m_pkFXArchive != NULL )
	//	m_pkFXArchive->Render( fTime );

	RenderFX( fTime , FXM_RENDER_LAYER_OBJECT_FRONT );
}


void SPEvent::RenderEventLine()
{
	//
	if( g_pVideo == NULL )
		return;

	switch(m_MapEvent.eventType) {
		case EVENT_NULL:
			g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
			break;
		case EVENT_NPC:				
			g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			break;
		case EVENT_SAVEPOINT:
			g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
			break;
		case EVENT_PORTAL:
			g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
			break;
		case EVENT_QUEST:		
			g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));			
			break;
		case EVENT_CORE:
			g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
			break;
		default:
			g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
			//DXUTOutputDebugString("...");
			break;
	}

	g_pVideo->GetSysLine()->DrawRect(m_rtEvent);
	g_pVideo->GetSysLine()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	if( g_pVideo->GetFont() != NULL )
		g_pVideo->GetFont()->RenderText(m_MapEvent.name, &m_rtEvent, DT_TOP | DT_CENTER );
}


bool SPEvent::IsCursorIn			( int iX, int iY)
{
	if( (m_MapEvent.rectLeft < m_fMouseX && m_fMouseX < m_MapEvent.rectRight) && 
		(m_MapEvent.rectTop < m_fMouseY && m_fMouseY < m_MapEvent.rectBottom) )
	{
		//return m_MapEvent.id;
		return true;
	}

	//return EVENT_ID_NULL;	
	return false;
}

bool SPEvent::IsCursorIn				( RECT & rc )
{
	bool	bLeft			=	( m_MapEvent.rectLeft	<= rc.left	 && m_MapEvent.rectRight	>= rc.left		);
	bool	bTop			=	( m_MapEvent.rectTop	<= rc.top	 && m_MapEvent.rectBottom   >= rc.top		);
	bool	bRight			=	( m_MapEvent.rectLeft	<= rc.right	 && m_MapEvent.rectRight	>= rc.right		);
	bool	bBottom			=	( m_MapEvent.rectTop	<= rc.bottom && m_MapEvent.rectBottom   >= rc.bottom	);

	bool	bLeftTop		=	bLeft	& bTop;
	bool	bLeftBottom		=	bLeft	& bBottom;
	bool	bRightTop		=	bRight	& bTop;
	bool	bRightBottom	=	bRight	& bBottom;

	//
	bool	bLeft2			=	( m_MapEvent.rectLeft	>= rc.left	 && m_MapEvent.rectLeft		<= rc.right		);
	bool	bTop2			=	( m_MapEvent.rectTop	>= rc.top	 && m_MapEvent.rectTop		<= rc.bottom	);
	bool	bRight2			=	( m_MapEvent.rectRight	>= rc.left	 && m_MapEvent.rectRight	<= rc.right		);
	bool	bBottom2		=	( m_MapEvent.rectBottom	>= rc.top	 && m_MapEvent.rectBottom   <= rc.bottom	);

	bool	bLeftTop2		=	bLeft2	& bTop2;
	bool	bLeftBottom2	=	bLeft2	& bBottom2;
	bool	bRightTop2		=	bRight2	& bTop2;
	bool	bRightBottom2	=	bRight2	& bBottom2;

	return bLeftTop | bLeftBottom | bRightTop | bRightBottom
			|  bLeftTop2 | bLeftBottom2 | bRightTop2 | bRightBottom2;
}


int  SPEvent::PerformMouseMove		( int iX, int iY)
{
	if(IsCursorIn(iX, iY)) {		
		if(m_MapEvent.eventType == EVENT_NPC ||
		   m_MapEvent.eventType == EVENT_MAILBOX ||
		   m_MapEvent.eventType == EVENT_BEAUTY ||
		   m_MapEvent.eventType == EVENT_CROPS) {
			return 1;
		}
	}
	
	return 0;
}


int  SPEvent::PerformMouseLDown		( int iX, int iY)
{
//	//
//	if(IsCursorIn(iX, iY)) {
//		if(m_MapEvent.eventType == EVENT_NPC){
//			//[2005/5/12] -  단순하게 cut in 상황만 있는것이 아님...
//			if(g_pEventManager->GetCutInState() == CUT_IN_STATE_NULL){
//#ifdef _DEBUG
//				char szTemp[256];
//				wsprintf(szTemp, " ->Click Event NPC : ID[%d] Name[%s] Type[%d]\n", m_MapEvent.id, m_MapEvent.name, m_MapEvent.eventType);
//				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szTemp );				
//#endif
//				//
//				if( g_pGOBManager->GetLocalPlayer() )
//				{
//					float fPlayerPosX;
//
//					fPlayerPosX = g_pGOBManager->GetLocalPlayer()->GetPosX();
//				}
//				
//				g_pEventManager->SetCutInState(CUT_IN_STATE_SCROLL);
//				g_pEventManager->GetEventHandler()->LoadNpc(m_MapEvent.eventTypeID);
//				DXUTOutputDebugString("CUT_IN_STATE_SCROLL\n");
//			}
//			else {
//				//Talk 창 또는 거래 메시지 창에서 종료시 호출하도록 변경 
//				//g_pEventManager->SetCutInState(CUT_IN_STATE_NULL);
//				//DXUTOutputDebugString("CUT_IN_STATE_NULL\n");
//			}
//		}
//		return 1;
//	}
	return 0;
}


int  SPEvent::PerformMouseLUp		( int iX, int iY)
{
	//
	if(IsCursorIn(iX, iY)) {
		//공통 예외처리		
		//if(m_MapEvent.eventType == EVENT_NPC || m_MapEvent.eventType == EVENT_SAVEPOINT || m_MapEvent.eventType == EVENT_MAILBOX || m_MapEvent.eventType == EVENT_BEAUTY)		
		if(m_MapEvent.eventType != EVENT_NULL && /* m_MapEvent.eventType != EVENT_NPC && */
			m_MapEvent.eventType != EVENT_QUEST)
		{
			//
			if( g_pGOBManager->GetLocalPlayer() )
			{
				float fPlayerPosX;

				fPlayerPosX = g_pGOBManager->GetLocalPlayer()->GetPosX();
				//g_pGOBManager->GetLocalPlayer()->SPGOBPerformMessage(SPGM_INVEN_RECOVER_ICON , 0);
			}
			else {
				return 1;
			}

			//[2006/1/2]	사망한 경우
			if(g_pGOBManager->GetLocalPlayer()->IsDead()) {
				return 1;
			}

			//[2005/8/1]	거래중인 상황인 경우
			if(g_pGOBManager->GetLocalPlayer()->GetTradeState() == TRADE_STATE_TRADE) {
				return 1;
			}

			////[8/4/2005]	임시방편임 거래 신청을 한 사람의 경우에 컷인에 들어가지 못함
			//if(g_pGOBManager->GetLocalPlayer()->GetTradeState() == TRADE_STATE_REQUEST) {
			//	return 1;
			//}

			//[2006/2/20]	내가 응답해야할 상황
			if(g_pGOBManager->GetLocalPlayer()->GetTradeState() == TRADE_STATE_RESPONSE) {					
				g_pInterfaceManager->ForceReply();
			}

			//[2006/5/29] 내가 개인상점인 경우
			if(g_pGOBManager->GetLocalPlayer()->GetCurAction() & ACTION_USERSHOP) {
				return 1;
			}
			
			//[2006/7/24] 내가 개인상점 구매자인 경우
			if(g_pGOBManager->GetLocalPlayer()->GetUserShopID()) {
				return 1;
			}

			//[2007/6/21]-아이템을 들고 맵 이벤트를 찍어버린 경우
			SPPlayerInvenArchive* pInven = g_pGOBManager->GetLocalPlayer()->GetInvenArchive();			
			bool bPickUp = pInven->IsTrashItem();
			if(pInven && bPickUp == false) {
				return 1;
			}

			//[2008/4/25]-캐시 아이템 구매 응답을 기다리는 경우
			if(SPCashTradeManager::GetInstance()->GetCashTradeState() == CASH_TRADE_STATE_WAIT) {
				return 1;
			}
		}
		
		//개별 처리
		if(m_MapEvent.eventType == EVENT_NPC){
			//[2005/5/12] -  단순하게 cut in 상황만 있는것이 아님...
			if(g_pEventManager->GetCutInState() == CUT_IN_STATE_NULL){
#ifdef _DEBUG
				char szTemp[256];
				wsprintf(szTemp, " ->Click Event NPC : ID[%d] Name[%s] Type[%d]\n", m_MapEvent.id, m_MapEvent.name, m_MapEvent.eventType);
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szTemp );				
#endif
				//g_pEventManager->GetEventHandler()->LoadNpc(m_MapEvent.eventTypeID);				
				g_pEventManager->SetCutInState(CUT_IN_STATE_SCROLL, &m_MapEvent);
				DXUTOutputDebugString("CUT_IN_STATE_SCROLL\n");
			}
			else {
				//Talk 창 또는 거래 메시지 창에서 종료시 호출하도록 변경 
				//g_pEventManager->SetCutInState(CUT_IN_STATE_NULL);
				//DXUTOutputDebugString("CUT_IN_STATE_NULL\n");
			}
		}
		else if(m_MapEvent.eventType == EVENT_MAILBOX) {
			//[2005/5/12] -  단순하게 cut in 상황만 있는것이 아님...
			if(g_pEventManager->GetCutInState() == CUT_IN_STATE_NULL){
#ifdef _DEBUG
				char szTemp[256];
				wsprintf(szTemp, " ->Click Event NPC : ID[%d] Name[%s] Type[%d]\n", m_MapEvent.id, m_MapEvent.name, m_MapEvent.eventType);
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szTemp );				
#endif
				g_pEventManager->SetCutInState(CUT_IN_STATE_SCROLL, &m_MapEvent);
				DXUTOutputDebugString("CUT_IN_STATE_SCROLL\n");
				g_pEventManager->GetEventHandler()->SetNpcTalkImage(29900000);
			}
			else {
			}
		}
		else if(m_MapEvent.eventType == EVENT_BEAUTY) {
			if( SPLocalizeManager::GetInstance()->IsEnableValue(LV_BEAUTYSHOP) )
			{
				if(g_pEventManager->GetCutInState() == CUT_IN_STATE_NULL){
#ifdef _DEBUG
				char szTemp[256];
				wsprintf(szTemp, " ->Click Event NPC : ID[%d] Name[%s] Type[%d]\n", m_MapEvent.id, m_MapEvent.name, m_MapEvent.eventType);
				g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szTemp );				
#endif				
				g_pEventManager->SetCutInState(CUT_IN_STATE_SCROLL, &m_MapEvent);
				DXUTOutputDebugString("CUT_IN_STATE_SCROLL\n");
				g_pEventManager->GetEventHandler()->SetNpcTalkImage(29900001);
				}
			}
		}
		else if(m_MapEvent.eventType == EVENT_CROPS) {
			if(g_pEventManager->GetCutInState() == CUT_IN_STATE_NULL){
				g_pEventManager->SetCutInState(CUT_IN_STATE_SCROLL, &m_MapEvent);
				DXUTOutputDebugString("CUT_IN_STATE_SCROLL\n");				
				DISPLAY_CROP_UNIT Crop;
				g_pGOBManager->GetGuildArchive()->GetCropImageByEvent(m_MapEvent.id, -1, Crop);
				g_pEventManager->GetEventHandler()->SetNpcTalkImage(Crop.iCutInImageID);
			}
		}
		return 1;
	}
	return 0;
}


int  SPEvent::PerformMouseRDown		( int iX, int iY)
{
	return 0;
}


int  SPEvent::PerformMouseRUp		( int iX, int iY)
{
	return 0;
}


//////////////////////////////////////////////////////////////////////////
//
SPGMESSAGE_OBJ_MAP_BEGIN( SPEvent )
SPGMESSAGE_OBJ_COMMAND(  SPGM_GETFACING				,	OnGetFacing				)
SPGMESSAGE_OBJ_COMMAND(  SPGM_GETSTANDLAYER			,	OnGetStandlayer			)
SPGMESSAGE_OBJ_COMMAND(  SPGM_GETPOSX				,	OnGetPosX				)
SPGMESSAGE_OBJ_COMMAND(  SPGM_GETPOSY				,	OnGetPosY				)
SPGMESSAGE_OBJ_COMMAND(  SPGM_GETGOBNAME			,	OnGetGOBName			)
SPGMESSAGE_OBJ_COMMAND(  SPGM_GETFXARCHIVE			,	OnGetFXArchive			)

//
//SPGMESSAGE_OBJ_COMMAND(  SPGM_COMMAND_ACTION		,	OnSetActionCommand		)
//SPGMESSAGE_OBJ_COMMAND(  SPGM_COMMAND_ACTION_COMPLETE,	OnSetActionCompleteCommand )
//
//SPGMESSAGE_OBJ_COMMAND(  SPGM_SET_SC_MONSTERINFO	,	OnSetMonsterInfo		)
//SPGMESSAGE_OBJ_COMMAND(  SPGM_SET_SC_LPPOSITION		,	OnSetLPPosotion			)
//SPGMESSAGE_OBJ_COMMAND(  SPGM_GOB_MOVE				,	OnSetMoveCommand		)	
//SPGMESSAGE_OBJ_COMMAND(  SPGM_SET_SC_DAMAGE_FX		,	OnSetDamageFX			)
//
//SPGMESSAGE_OBJ_COMMAND_RANGE ( SPGM_STAT_MSG_BEGIN, SPGM_STAT_MSG_END, OnStatMessage )

SPGMESSAGE_OBJ_COMMAND(	SPGM_EVENT_SC_INFO			,	OnSetEventInfo			)

SPGMESSAGE_OBJ_MAP_END( SPGameObject )



//////////////////////////////////////////////////////////////////////////
//
int SPEvent::OnSetEventInfo(LPARAM lParam)
{
	//SetEventInfo((MAPEVENT_INFO*)lParam);
	SetEventInfo((EVENT_INFO*)lParam);
	return 1;
}


int SPEvent::OnGetPosX( LPARAM lParam )
{
	return static_cast<int>( GetPosX() );
}


int SPEvent::OnGetFacing(LPARAM lParam)
{
	return (int)m_eFacing;
}


int SPEvent::OnGetPosY( LPARAM lParam )
{
	return static_cast<int>( GetPosY() );
}

int SPEvent::OnGetGOBName( LPARAM lParam )
{
	char* pStrName = (char*)lParam;

	strcpy( pStrName , m_MapEvent.name );

	return 0;
}

int SPEvent::OnGetStandlayer	( LPARAM lParam )
{	
	return m_MapEvent.iTerrainLayer;
}

int SPEvent::OnGetFXArchive(LPARAM lParam)
{
	SPFXArchive** ppFXArchive = (SPFXArchive**)lParam;

	*ppFXArchive = m_pkFXArchive;
	return 0;
}

//////////////////////////////////////////////////////////////////////////
//
void SPEvent::SetNull()
{
	//////////////////////////////////
	//MAPEVENT_INFO Struct
	//UINT32	id					
	//UINT16	stageID				
	//UINT16	mapGroupID			
	//UINT16	mapID				
	//float	posX				
	//float	posY				
	//float	rectTop				
	//float	rectLeft			
	//float	rectRight			
	//float	rectBottom			
	//UINT32	obejctImage			
	//UINT32	tipMessage			
	//UINT8	eventType			
	//UINT32	eventTypeID			
	//UINT8	statusTypeLimit1	
	//UINT32	statusValueLimit1	
	//UINT8	statusTypeLimit2	
	//UINT32	statusValueLimit2	
	//UINT8	inventoryTypeLimit	
	//UINT32	itemLimit			
	//TCHAR	name[LEN_NAME+1]	
	//TCHAR	dummy[3]
	///////////////////////////////////

	////UINT32	classID;
	//m_MapEvent.classID					= CLASS_ID_NULL;
	//m_MapEvent.id						= 0;
	//m_MapEvent.stageID					= 0;		
	//m_MapEvent.mapGroupID				= 0; 		
	//m_MapEvent.mapID					= 0;
	//m_MapEvent.posX						= 0.0f;
	//m_MapEvent.posY						= 0.0f;
	//m_MapEvent.rectTop					= 0.0f;
	//m_MapEvent.rectLeft					= 0.0f;
	//m_MapEvent.rectRight				= 0.0f;
	//m_MapEvent.rectBottom				= 0.0f;
	//m_MapEvent.obejctImage				= 0;
	//m_MapEvent.tipMessage				= 0;
	//m_MapEvent.eventType				= EVENT_NULL;
	//m_MapEvent.eventTypeID				= 0;
	//m_MapEvent.statusTypeLimit1			= 0;
	//m_MapEvent.statusValueLimit1		= 0;
	//m_MapEvent.statusTypeLimit2			= 0;
	//m_MapEvent.statusValueLimit2		= 0;
	//m_MapEvent.inventoryTypeLimit		= 0;
	//m_MapEvent.itemLimit				= 0;
	//m_MapEvent.iTerrainLayer			= 0;
	//ZeroMemory(&m_MapEvent.name, LEN_NAME+1);
	//ZeroMemory(&m_MapEvent.dummy, 3);	//??

	m_MapEvent.Clear();

	m_fCamX = 0.0f;
	m_fCamY = 0.0f;
	m_fPlayerX = 0.0f;
	m_fPlayerY = 0.0f;
	m_bShowWindow = false;
	m_bSendPacket = false;
	m_bRunCoreEvent = false;
}


EVENT_TYPE SPEvent::GetEventType()
{
	return (EVENT_TYPE)m_MapEvent.eventType;
}


float SPEvent::GetPosX()
{
	return m_MapEvent.posX;	
}


float SPEvent::GetPosY()
{
	return m_MapEvent.posY;
}

std::string SPEvent::GetEventName()
{
	return m_MapEvent.name;
}

//bool SPEvent::SetEventInfo(MAPEVENT_INFO* pEventInfo /*= NULL*/) 
bool SPEvent::SetEventInfo(EVENT_INFO* pEventInfo /*= NULL*/) 
{
	if(pEventInfo == NULL)			return false;
	
	SetNull();

	m_MapEvent.classID = GetClassID();
	m_MapEvent.instanceID = GetInstanceID();

	m_MapEvent.SetEventInfo(pEventInfo);

	m_MapEvent.posX = pEventInfo->posX;
	m_MapEvent.posY = pEventInfo->posY;

	//
	//
	//DXUTOutputDebugString("SetEventInfo clssID[%d]\n", (int)GetClassID());
	
	////
	//m_MapEvent.classID				=	pEventInfo->classID				;
	//m_MapEvent.id					=	pEventInfo->id					;	
	//m_MapEvent.stageID				=	pEventInfo->stageID				;
	//m_MapEvent.mapGroupID			=	pEventInfo->mapGroupID			;
	//m_MapEvent.mapID				=	pEventInfo->mapID				;
	//m_MapEvent.posX					=	pEventInfo->posX				;	
	//m_MapEvent.posY					=	pEventInfo->posY				;	
	//m_MapEvent.rectTop				=	pEventInfo->rectTop				;
	//m_MapEvent.rectLeft				=	pEventInfo->rectLeft			;	
	//m_MapEvent.rectRight			=	pEventInfo->rectRight			;
	//m_MapEvent.rectBottom			=	pEventInfo->rectBottom			;
	//
	//m_MapEvent.obejctImage			=	pEventInfo->obejctImage			;
	//m_MapEvent.tipMessage			=	pEventInfo->tipMessage			;

	//m_MapEvent.eventType			=	pEventInfo->eventType			;
	//m_MapEvent.eventTypeID			=	pEventInfo->eventTypeID			;
	//m_MapEvent.statusTypeLimit1		=	pEventInfo->statusTypeLimit1	;	
	//m_MapEvent.statusValueLimit1	=	pEventInfo->statusValueLimit1	;
	//m_MapEvent.statusTypeLimit2		=	pEventInfo->statusTypeLimit2	;	
	//m_MapEvent.statusValueLimit2	=	pEventInfo->statusValueLimit2	;
	//m_MapEvent.inventoryTypeLimit	=	pEventInfo->inventoryTypeLimit	;
	//m_MapEvent.itemLimit			=	pEventInfo->itemLimit			;
	//m_MapEvent.iTerrainLayer		=   pEventInfo->iTerrainLayer + BG_ID_LAYER_TERRAIN0 - 1;
	//m_MapEvent.iFacing				=	pEventInfo->iFacing		;
	//wsprintf(m_MapEvent.name, "%s", pEventInfo->name);
	//wsprintf(m_MapEvent.dummy, "%s", pEventInfo->dummy);

	//[2006/1/17]	 기존의 MapEvent에서 변경된...
	//LDT에서 로딩	
                       
	//[2006/1/26] Test...
	//return true;


	if(LoadEventLDT(m_MapEvent.id) == false) {
		//assert("SPEvent - MapEvent ID[%d] Data 없음", m_MapEvent.id);
		return false;
	 }

	////[2007/10/1] - 강제 이벤트인 경우
	//if(m_MapEvent.eventType == EVENT_MAILBOX){		//Test용 코드임
	//	m_MapEvent.eventType = EVENT_CORE;
	//	m_MapEvent.eventTypeID = 101100;
	//}
	if(m_MapEvent.eventType == EVENT_CORE) {
		bool bRet = LoadEventCore(m_MapEvent.eventTypeID);
		return bRet;
	}
	
	// FXArchive에서 사용합니다. [2/14/2006 AJJIYA]
	SetClassID((CLASS_ID)m_MapEvent.obejctImage);

	if( m_pkModelUnit )
	{
		((SPMonsterModelUnit*)m_pkModelUnit)->SetNPC(m_MapEvent.obejctImage);
		m_pkModelUnit->Init();

		//Animation 변경
		ChangeAnimation();
		m_pkStatusModel->SetObjectName(m_MapEvent.name);
	}	

	//[2006/1/26]
	//return true;
	
	//만일 MapEvnetType이 NPC라면...
	if(m_MapEvent.eventType == EVENT_NPC)
		LoadNpcLDT(m_MapEvent.eventTypeID);	

	SetDynimicFX() ;
	
	//
	//DXUTOutputDebugString("Create Event id[%d] type[%d] left[%d] top[%d] right[%d] bottom[%d]\n",	(int)m_MapEvent.id, (int)m_MapEvent.eventType,
	//	(int)m_MapEvent.rectLeft, (int)m_MapEvent.rectTop, (int)m_MapEvent.rectRight, (int)m_MapEvent.rectBottom);
	
	return true;
}

bool SPEvent::UpdateModelNpc(UINT32 ObjectImageID)
{
	if( m_pkModelUnit )
	{
		((SPMonsterModelUnit*)m_pkModelUnit)->SetNPC(ObjectImageID);
		m_pkModelUnit->Init();

		//Animation 변경
		ChangeAnimation();
		return true ;
	}
	return false ;
}


bool SPEvent::LoadEventCore(int iItemID)
{	
	SPCoreEventInfo* pInfo = g_pEventManager->GetEventArchive()->GetCorEventInfo(iItemID);
	
	if(pInfo == NULL)
		return false;

	m_vpCoreRequire.clear();
	
	int iRequireID = 0;
	SPCoreRequireAttr* pReqAttr = NULL;
	for(int i = 0; i < MAX_CORE_REQUIRE; i++) {
		iRequireID = 0;
		pReqAttr = NULL;
		
		iRequireID = pInfo->m_iRequireID[i];
		if(iRequireID) {
			pReqAttr = g_pEventManager->GetEventArchive()->GetCoreRequireAttr(iRequireID);
			if(pReqAttr) {
				m_vpCoreRequire.push_back(pReqAttr);
			}
		}		
	}
	return true;
}


bool SPEvent::LoadEventLDT(int iItemID)
{	
	MAPEVENT_INFO* pMapEvent = g_pEventManager->GetEventArchive()->GetEvent(iItemID);		

	if(pMapEvent == NULL)
		return false;
	
	wsprintf(m_MapEvent.name, "%s", pMapEvent->name);
	m_MapEvent.rectTop				= m_MapEvent.posY - pMapEvent->rectTop;
	m_MapEvent.rectLeft				= m_MapEvent.posX - pMapEvent->rectLeft;
	m_MapEvent.rectRight			= m_MapEvent.posX + pMapEvent->rectRight;
	m_MapEvent.rectBottom			= m_MapEvent.posY + pMapEvent->rectBottom;

	m_MapEvent.obejctImage			= pMapEvent->obejctImage;
	wsprintf(m_MapEvent.tipMessage, "%s", pMapEvent->tipMessage);
	m_MapEvent.eventType			= pMapEvent->eventType;
	m_MapEvent.eventTypeID			= pMapEvent->eventTypeID;
	
	m_MapEvent.requireInfo[0].SetRequireInfo(pMapEvent->requireInfo[0].requireType,
			pMapEvent->requireInfo[0].requireID,
			pMapEvent->requireInfo[0].requireValue1,
			pMapEvent->requireInfo[0].requireValue2);
	
	m_MapEvent.requireInfo[1].SetRequireInfo(pMapEvent->requireInfo[1].requireType,
		pMapEvent->requireInfo[1].requireID,
		pMapEvent->requireInfo[1].requireValue1,
		pMapEvent->requireInfo[1].requireValue2);
	
	m_MapEvent.iTerrainLayer		= pMapEvent->iTerrainLayer;	
	m_MapEvent.iFacing				= pMapEvent->iFacing;	
	m_MapEvent.iInsDungeunID		= pMapEvent->iInsDungeunID;
	m_MapEvent.iDynamicFX			= pMapEvent->iDynamicFX;
	wsprintf(m_MapEvent.errorMessage, "%s", pMapEvent->errorMessage);
	wsprintf(m_MapEvent.tipMessageOff, "%s", pMapEvent->tipMessageOff);

	m_eFacing = (FACING)m_MapEvent.iFacing;
	return true;
}


int SPEvent::LoadNpcLDT(int iItemID)
{
	//SPNpc* pNpc = new SPNpc;

	SPNpc* pNpc = g_pEventManager->GetEventArchive()->GetNpc(iItemID);

	if(pNpc == NULL) {
		//assert("SPEvent - Npc ID [%d] Data 없음 ", iItemID);
		return 0;
	}

	//int iType[4] = {0, 0, 0, 0};
	//int iTypeID[4] = {0, 0, 0, 0};	
	int i = 0;	
	
	m_iQuestListID = 0;
	for(i = 0; i < 4; i++) {
		if(pNpc->m_NpcEventType[i].m_iEventType == NPC_EVENT_TYPE_QUEST) {
			m_iQuestListID = pNpc->m_NpcEventType[i].m_iTypeID;
			break;
		}		
	}

	LoadNpcQuestList(m_iQuestListID);
	
	return 1;
}


int SPEvent::LoadNpcQuestList(int iItemID)
{
	SPNpcQuestList* pQuestList = g_pEventManager->GetEventArchive()->GetQuestList(iItemID);

	if( pQuestList != NULL )
	{
		//int recordCount = pLDTFile->GetItemCount();
		int i = 0;	
		int iQuestID = 0;

		for(i = 0; i < MAX_QUEST_LIST; i++) {
			//ZeroMemory(szFieldQuestID, 64);
			//wsprintf(szFieldQuestID, "_QuestID%d", i+1);		
			//pLDTFile->GetFieldFromLable(iItemID, szFieldQuestID,	ldtFieldQuestID);
			iQuestID = pQuestList->m_iQuestID[i];

			if(iQuestID == 0)
				break;

			if(AddQuestList(iQuestID) == false) {
#if defined(_DEBUG)
				char szErrMsg[256];
				ZeroMemory(szErrMsg, 256);
				wsprintf(szErrMsg, "SPEvent - Npc ID[%d] QuestList[%d] QuestID[%d] Data 없음", 
					m_MapEvent.eventTypeID, iItemID, iQuestID);				
				//MessageBox(NULL, szErrMsg, "Event Error", MB_OK);
				//assert(0 && szErrMsg);				
#endif
			}
		}		
	}
	else {
		//assert("SPEvent - Npc ID[%d] QuestList[%d] Data 없음", m_MapEvent.eventTypeID, iItemID);
	}

	//[2006/1/26]
	CheckEventQuestShow();
	
	return 1;
}


bool SPEvent::AddQuestList(int iQuestID)
{
	//SPQuestAttr* pQuestAttr = new SPQuestAttr;
	//g_pEventManager->GetQuestManager()->LoadLDTFile(iQuestID, pQuestAttr);

	SPQuestAttr* pQuestAttr = g_pEventManager->GetEventArchive()->GetQuestAttr(iQuestID);	

	if(pQuestAttr == NULL)
		return false;
	
	pQuestAttr->IsRequire();				//받을수 있는지는 여기서 결정한다 	

	m_vpQuest.push_back(pQuestAttr);

	return true;
}


EVENT_QUEST_SHOW SPEvent::CheckEventQuestShow()
{
	ClearEventFX();	
	
	//플레이어 보상 Quest의 Event(Npc)인경우
	SPQuestArchive* pArchive = g_pEventManager->GetQuestManager()->GetPlayerArchive();	
	int iPlayerQuestList = static_cast<int>(pArchive->GetSize());
	
	SPQuestAttr* pQuestAttr = NULL;
	for(int i = 0; i < iPlayerQuestList; i++) {
		pQuestAttr = NULL;
		pQuestAttr = pArchive->GetQuestAttr(i);
		if(pQuestAttr && pQuestAttr->m_iUiShow == QUEST_UI_SHOW_COMPLETE) {
			if(pQuestAttr->m_iRewordNpcID == m_MapEvent.eventTypeID) {
				m_iEventQuestShow = EVENT_QUEST_SHOW_COMPLETE;
				
				//완료 FX 번호를 넣준다
				m_pkFXArchive->InsertFXGroup((SPID_FX_GROUP)2011);

				return m_iEventQuestShow;
			}
		}
	}

	//현 NPC 이벤트가 Quest List가 없는 경우
	if(m_vpQuest.empty()) {		
		return m_iEventQuestShow;
	}
	
	//보상이 아니고 플레이어가 받을 퀘스트가 있는 경우
	std::vector<SPQuestAttr*>::iterator iter;
	iter = m_vpQuest.begin();
	for(; iter != m_vpQuest.end(); ++iter) {		
		//[2006/1/24] - 이벤트에 로딩된 Quest자체의 업데이트도 해준다 
		(*iter)->m_iUiShow = QUEST_UI_SHOW_ABLE;
		(*iter)->IsRequire();

		if((*iter)->GetQuestUIShow() == QUEST_UI_SHOW_ABLE) {			
			
			//[2006/1/23]	현재 진행중인것은 숨긴다			
			if(g_pEventManager->GetQuestManager()->IsDoingQuest((*iter)->m_iQuestID)) {				
				continue;
			}
			
			m_iEventQuestShow = EVENT_QUEST_SHOW_ABLE;
			
			//수락가능한 FX 번호를 넣준다
			m_pkFXArchive->InsertFXGroup((SPID_FX_GROUP)2009);

			return m_iEventQuestShow;
		}
	}

	// 만약 DynimicFx 항목을 가지고 있으면 추가해 준다.
	SetDynimicFX() ;
		
	return m_iEventQuestShow;
}


void SPEvent::ClearEventFX()
{
	m_iEventQuestShow = EVENT_QUEST_SHOW_NULL;		//음...
	//여기서 Quest관련 FX를 삭제한다
	m_pkFXArchive->DeleteFXGroup(2009);
	m_pkFXArchive->DeleteFXGroup(2011);	
}


bool SPEvent::SendEventPacket()
{	
	if(!m_bSendPacket) {
		//m_bSendPacket = false;
#ifdef _DEBUG
		char szTemp[256];
		wsprintf(szTemp, " ->Send Event ID[%d] Name[%s] Type[%d]\n", m_MapEvent.id, m_MapEvent.name, m_MapEvent.eventType);
		g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szTemp );
#endif

		//CUT IN의 경우 이벤트 실행 없음.. //추후 Save Point 에서는 변경할것
		if(g_pEventManager->GetCutInState() != CUT_IN_STATE_NULL)
			return false;
		
		//		
		CPacket Packet;
		Packet.Add((UINT32)EVENT_CS_RAISE);
		Packet.Add((UINT8)m_MapEvent.eventType);
		Packet.Add((UINT32)m_MapEvent.id);
		
		//이부분 리턴값 문제 있을수 있음...
		g_pNetworkManager->SPPerformMessage(EVENT_CS_RAISE, 0, (LPARAM)&Packet);

		m_bSendPacket = true;
		return true;
	}	
	
	return false;
}


unsigned int SPEvent::CheckPlayerEvent()
{
	if( (m_MapEvent.rectLeft < m_fPlayerX && m_fPlayerX < m_MapEvent.rectRight) && 
		(m_MapEvent.rectTop < m_fPlayerY && m_fPlayerY < m_MapEvent.rectBottom) )
	{
		//[2007/10/2] - 강제이벤트인 경우
		if(m_MapEvent.eventType == EVENT_CORE) {
			if(!m_bShowWindow && !m_bRunCoreEvent) {
				if(CheckCoreRequire()) {
					m_bShowWindow = true;
					m_bRunCoreEvent = true;
				}
				return 1;
			}
		}

		if(!m_bShowWindow) {
			if(m_MapEvent.eventType == EVENT_CORE) {
				return 1;
			}
			
			m_bShowWindow = true;

			//Send Control Message - Show
#ifdef _DEBUG
			char szTemp[256];			
			wsprintf(szTemp, "SPEvent Check[Show] ID[%d] Name[%s] Type[%d]\n", m_MapEvent.id, m_MapEvent.name, m_MapEvent.eventType);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szTemp );
#endif
			
			//[2006/1/4] - tipMessage가 있는 경우에만
			//SPWindow* pWindow = m_pSubGameZone->Find(WIID_MESSAGE, true);
			SPWindow* pWindow = g_pInterfaceManager->FindWindow(WIID_MESSAGE);		

			int iMsgLength = strlen(m_MapEvent.tipMessage);

			if(iMsgLength > 0) {
				//g_pInterfaceManager->SPPerformMessage( SPIM_SET_MESSAGE_TEXT, (WPARAM)((unsigned int)m_MapEvent.tipMessage), (LPARAM)((int)m_MapEvent.eventType));
				//g_pInterfaceManager->SPPerformMessage( SPIM_SET_MESSAGE_SHOW, (WPARAM)m_bShowWindow );
				
				//  [2006/1/17] Event의 Require에 의해 분기
				if(CheckRequire())
				{
					if(pWindow) 
					{
						int iStageID	= m_MapEvent.mapInfo.uiStageID;
						int iMapGroupID	= m_MapEvent.mapInfo.uiMapGroupID;
						if( m_MapEvent.eventType == EVENT_PORTAL
							&& !g_pEventManager->GetEventArchive()->isMapOpenTime(m_MapEvent.eventTypeID) )
							pWindow->SPSendMessage(SPIM_SET_MESSAGE_TEXT, (WPARAM)m_MapEvent.tipMessageOff, (LPARAM)((int)m_MapEvent.eventType));
						else
							pWindow->SPSendMessage(SPIM_SET_MESSAGE_TEXT, (WPARAM)m_MapEvent.tipMessage, (LPARAM)((int)m_MapEvent.eventType));
						pWindow->SPSendMessage(SPIM_SET_MESSAGE_SHOW, (WPARAM)m_bShowWindow);
					}
				}
				else 
				{
					if(pWindow) 
					{
						pWindow->SPSendMessage(SPIM_SET_MESSAGE_TEXT, (WPARAM)m_MapEvent.errorMessage, (LPARAM)((int)m_MapEvent.eventType));
						pWindow->SPSendMessage(SPIM_SET_MESSAGE_SHOW, (WPARAM)m_bShowWindow);
					}
				}
			}			
		}
		
		
		if(m_MapEvent.eventType != EVENT_NULL && /* m_MapEvent.eventType != EVENT_NPC && */
			m_MapEvent.eventType != EVENT_QUEST && 
			m_MapEvent.eventType != EVENT_CORE)
		{
			UINT uiControl = g_pGOBManager->GetLocalPlayer()->GetControl();
			if( ((uiControl & ACTION_MOVE_UP) == ACTION_MOVE_UP) && !m_bSendPacket) {				
				// 공통 예외상황 처리
				//[2006/1/2]	사망한 경우
				if(g_pGOBManager->GetLocalPlayer()->IsDead()) {
					return 1;
				}

				//[2005/8/1]	거래중인 상황인 경우
				if( g_pGOBManager->GetLocalPlayer()->GetTradeState() == TRADE_STATE_TRADE ) {
					return 1;
				}

				//[2006/5/29] 내가 상점인 경우
				if(g_pGOBManager->GetLocalPlayer()->GetCurAction() & ACTION_USERSHOP) {
					return 1;
				}

				//[2006/7/24] 내가 개인상점 구매자인 경우
				if(g_pGOBManager->GetLocalPlayer()->GetUserShopID()) {
					return 1;
				}

				////[8/4/2005]	임시방편임 거래 신청을 한 사람의 경우에 컷인에 들어가지 못함
				//if( g_pGOBManager->GetLocalPlayer()->GetTradeState() == TRADE_STATE_REQUEST ) {
				//	return 1;
				//}
				//[2006/2/20]	내가 응답해야할 상황
				if(g_pGOBManager->GetLocalPlayer()->GetTradeState() == TRADE_STATE_RESPONSE) {
					g_pInterfaceManager->ForceReply();
				}

				//[2007/4/19] - NoticeBox가 보이는 경우 캔슬
				SPWindow* pWindow = g_pInterfaceManager->GetNoticeBox();
				if(pWindow && pWindow->IsShow()) {
					return 1;
				}

				//[2006/1/19] 이벤트의 Require를 만족하지 못하면 넘어간다
				if(CheckRequire() == false)
					return 1;
				
				//[2008/4/25]-캐시 아이템 구매 응답을 기다리는 경우
				if(SPCashTradeManager::GetInstance()->GetCashTradeState() == CASH_TRADE_STATE_WAIT) {
					return 1;
				}
				
				// 각 맵이벤트 처리
				if(m_MapEvent.eventType == EVENT_SAVEPOINT)	{
					//[2005/8/30] 여기서 컷인 NPC 대화로 넘어 갈수 있도록 해주어야 한다 					
					g_pEventManager->GetEventHandler()->LoadSavePointData(m_MapEvent.eventTypeID);
					//[2005/9/30] - BGM 바꾸기
					std::string strBGM = g_pEventManager->GetEventHandler()->GetSavePointData()->m_MsgInfo[SAVE_POINT_0].m_strBGM;
					if(!strBGM.empty()){
						g_pEventManager->GetEventHandler()->SetNpcBGM(strBGM);
						//g_pSubGameManager->SPPerformMessage(SPGM_SUG_SETBGM , (WPARAM)strBGM.c_str());
					}

					g_pEventManager->SetCutInState(CUT_IN_STATE_SCROLL, &m_MapEvent);
					DXUTOutputDebugString("CUT_IN_STATE_SCROLL\n");
				}
				else if(m_MapEvent.eventType == EVENT_PORTAL) {					
					//m_bSendPacket = true;
					if(m_MapEvent.iInsDungeunID){
						if( g_pSubGameManager )
							g_pSubGameManager->SPPerformMessage(SPGM_INDUN_IN_REQUEST, m_MapEvent.iInsDungeunID);
					}
					else {
						SendEventPacket();
					}
				}
				else if(m_MapEvent.eventType == EVENT_NPC) {
					//여기서 NPC Open					
					g_pEventManager->SetCutInState(CUT_IN_STATE_SCROLL, &m_MapEvent);
					DXUTOutputDebugString("CUT_IN_STATE_SCROLL\n");
				}
				else if(m_MapEvent.eventType == EVENT_MAILBOX) {	//우체통					
					g_pEventManager->SetCutInState(CUT_IN_STATE_SCROLL, &m_MapEvent);
					DXUTOutputDebugString("CUT_IN_STATE_SCROLL\n");
					g_pEventManager->GetEventHandler()->SetNpcTalkImage(29900000);
				}
				else if(m_MapEvent.eventType == EVENT_BEAUTY) {		//Beauty Shop
					if( SPLocalizeManager::GetInstance()->IsEnableValue(LV_BEAUTYSHOP) )
					{
						g_pEventManager->SetCutInState(CUT_IN_STATE_SCROLL, &m_MapEvent);
						DXUTOutputDebugString("CUT_IN_STATE_SCROLL\n");
						g_pEventManager->GetEventHandler()->SetNpcTalkImage(29900001);
					}
				}
				else if(m_MapEvent.eventType == EVENT_CROPS) {
					g_pEventManager->SetCutInState(CUT_IN_STATE_SCROLL, &m_MapEvent);
					DXUTOutputDebugString("CUT_IN_STATE_SCROLL\n");
					DISPLAY_CROP_UNIT Crop;
					g_pGOBManager->GetGuildArchive()->GetCropImageByEvent(m_MapEvent.id, -1, Crop);
					g_pEventManager->GetEventHandler()->SetNpcTalkImage(Crop.iCutInImageID);
				}
				
				m_bShowWindow = false;
				//g_pInterfaceManager->SPPerformMessage( SPIM_SET_MESSAGE_SHOW, (WPARAM)m_bShowWindow );
			}
		}
		else if(m_MapEvent.eventType == EVENT_QUEST){
			//[2005/9/30] 현재 해당 맵이벤트의 퀘스트가 진행중인 상황인지 확인한다
			bool bResult = false;				
			bResult = g_pEventManager->GetQuestManager()->CheckMissionEvent(m_MapEvent.id);
			
			if(bResult) {
				SendEventPacket();
			}				
		}	

		return m_MapEvent.id;
	}
	else {
		if(m_bShowWindow) {
			m_bShowWindow = false;
			//m_bSendPacket = false;

			//Send Control Message - Hide
#ifdef _DEBUG
			char szTemp[256];
			wsprintf(szTemp, "SPEvent Check[Hide] ID[%d] Name[%s]\n", m_MapEvent.id, m_MapEvent.name);
			g_pInterfaceManager->SPPerformMessage( SPIM_WNDCHAT_MESSAGE_STRING , (WPARAM)szTemp );
#endif
			g_pInterfaceManager->SPPerformMessage( SPIM_SET_MESSAGE_SHOW, (WPARAM)m_bShowWindow );
		}

		if(m_MapEvent.eventType == EVENT_CORE) {
			m_bRunCoreEvent = false;
		}

		m_bSendPacket = false;
	}

	return EVENT_ID_NULL;
}


bool SPEvent::CheckRequire()
{	
	for( int i = 0 ; i < 2 ; i++ )
	{
		if(g_pCheckManager->CheckCondition(m_MapEvent.requireInfo[i].requireType , 
							m_MapEvent.requireInfo[i].requireID,
							m_MapEvent.requireInfo[i].requireValue1,
							m_MapEvent.requireInfo[i].requireValue2) == false )
		{
			return false;
		}
	}
	
	return true;
}


bool SPEvent::CheckCoreRequire()
{	
	if(m_MapEvent.eventType != EVENT_CORE)
		return false;

	bool bRet = true;

	if(m_vpCoreRequire.empty())
		return false;	

	std::vector<SPCoreRequireAttr*>::iterator iter = m_vpCoreRequire.begin();
	int i = 0;
	for(; iter != m_vpCoreRequire.end(); ++iter) {
		bRet = true;
		for(i = 0; i < MAX_CORE_EVENT_REQUIRE; i++) {
			bRet &= g_pCheckManager->CheckCondition((*iter)->m_Require[i].requireType,
				(*iter)->m_Require[i].requireID,
				(*iter)->m_Require[i].requireValue1,
				(*iter)->m_Require[i].requireValue2);			
		}

		//인벤 체크
		bRet &= IsEnableInven((*iter));
		if(bRet == false)
			continue;

		//Quest여유공간 체크
		bRet &= IsEnableQuestSize((*iter));
		if(bRet == false)
			continue;

		//조건체크를 만족하는 경우
		if(bRet && (*iter)->m_iEventType && (*iter)->m_iEventID) {				
			//해당 이벤트 발동
			SPWindow* pWindow = NULL;
			SPNpcTalk* pNpcTalk = NULL;
			switch((*iter)->m_iEventType){
				case NPC_EVENT_TYPE_TALK:
					pNpcTalk = g_pEventManager->GetEventHandler()->LoadNpcTalkDataLDT((*iter)->m_iEventID);
					if(pNpcTalk && !pNpcTalk->m_strMessage.empty()) {
						pWindow = g_pInterfaceManager->FindWindow(WIID_NPCTALK);
						if(pWindow) {
							pWindow->SPSendMessage(SPIM_SET_NPCTALK);
						}
						g_pEventManager->GetEventHandler()->SetNpcTalkImage(pNpcTalk->m_iCutInImage);
					}
					else{
						return false;
					}
					break;
				case NPC_EVENT_TYPE_TRADE:
					g_pEventManager->GetEventHandler()->LoadNpcShopDataLDT((*iter)->m_iEventID);
					break;
				case NPC_EVENT_TYPE_STORAGE:
					g_pEventManager->GetEventHandler()->LoadNpcStorageDataLDT((*iter)->m_iEventID);
					break;
				case NPC_EVENT_TYPE_QUEST:
					g_pEventManager->GetEventHandler()->LoadNpcQuestDataLDT((*iter)->m_iEventID);
					break;
				case NPC_EVENT_TYPE_GIFT:
					g_pEventManager->GetEventHandler()->LoadNpcGiftDataLDT((*iter)->m_iEventID);
					break;
				case NPC_EVENT_TYPE_GUILD_CREATE:
					g_pEventManager->GetEventHandler()->LoadNpcGuildDataLDT((*iter)->m_iEventID);
					break;
				case NPC_EVENT_TYPE_GUILD_DISSOLVE:
					g_pEventManager->GetEventHandler()->LoadNpcGuildDataLDT((*iter)->m_iEventID);
					break;
				case NPC_EVENT_TYPE_GUILD_ENTER:
					if( g_pSubGameManager )
						g_pSubGameManager->SPPerformMessage(SPGM_GUILD_INDUN_ENTER);
					return true;
					break;
				case NPC_EVENT_TYPE_PORTAL:
					g_pEventManager->GetEventHandler()->LoadNpcPortalDataLDT((*iter)->m_iEventID);
					break;
				case NPC_EVENT_TYPE_CHANGE:
					g_pEventManager->GetEventHandler()->LoadNpcEventListDataLDT((*iter)->m_iEventID);
					break;
				case NPC_EVENT_TYPE_GUILD_LEVELUP:
					g_pEventManager->GetEventHandler()->LoadNpcGuildDataLDT((*iter)->m_iEventID);
					break;
				case NPC_EVENT_TYPE_GUILD_EMBLEM:
					g_pEventManager->GetEventHandler()->LoadNpcGuildDataLDT((*iter)->m_iEventID);
					break;
				default:
					break;
			}
			
			if(g_pGOBManager->GetLocalPlayer()) {
				//g_pGOBManager->GetLocalPlayer()->SetAction(ACTION_STOP);
				g_pGOBManager->GetLocalPlayer()->SetControlAction(ACTION_STOP);
				g_pGOBManager->GetLocalPlayer()->SendActionPacket();
			}

			g_pEventManager->SetCoreRequireAttr((*iter));
			g_pEventManager->SetCutInState(CUT_IN_STATE_SCROLL, &m_MapEvent);
			return true;
		}
	}

	return false;
}


bool SPEvent::IsEnableInven(SPCoreRequireAttr* pCoreReq)
{
	if(pCoreReq == NULL || pCoreReq->m_iID == 0)
		return false;
	
	bool bRet = true;

	int i = 0;	
	
	//인벤토리 체크

	CONTAINER_TYPE iType = CONTAINER_TYPE_INVALID;
	int iCheckSize = 0;
	for(i = 0; i < MAX_CORE_EVENT_REQINVEN; i++) {		
		iType = CONTAINER_TYPE_INVALID;
		iCheckSize = 0;
		
		iType = pCoreReq->m_InvenReq[i].m_iContainerType;
		iCheckSize = pCoreReq->m_InvenReq[i].m_iSize;

		if(iType > CONTAINER_TYPE_INVALID && iType < FIGURE_EQUIP)
		{
			if(iCheckSize) {
				if(false == g_pCheckManager->CheckRequireSlot(iType, iCheckSize)) {					
					bRet &= false;
					break;
				}
			}
		}
	}

	return bRet;
}


bool SPEvent::IsEnableQuestSize(SPCoreRequireAttr* pCoreReq)
{
	bool bRet = true;
	
	//Quest여유공간 체크
	if(pCoreReq->m_iReqQuestSize) {
		int iCurQuestSize = g_pEventManager->GetQuestManager()->GetPlayerArchive()->GetSize();
		int iCheckSize = iCurQuestSize + pCoreReq->m_iReqQuestSize;

		if(iCheckSize > QUEST_MAX_ACTIVE) {
			bRet = false;
		}
	}

	return bRet;
}


void SPEvent::ChangeAnimation()
{
	if( m_pkModelUnit == NULL )
		return;

	SPMOBAnimation* pkAnimation;
	int iIndex = 0;

	iIndex = (rand() % 3) + ANI_NPC_STAND01;
	pkAnimation = g_pMOBCluster->GetAnimation(
		m_MapEvent.obejctImage, (ANIMATION_INDEX)iIndex);
	
	if( !pkAnimation )
		iIndex = ANI_NPC_STAND01;
	
	m_pkModelUnit->SPSendMessage(MV_SET_ANIMATION_INDEX, (ANIMATION_INDEX)iIndex, 1);
	m_pkModelUnit->SPSendMessage(MV_GET_ANI_ACCUMUL_TIME, (WPARAM)&m_fAnimationTime);
	m_pkModelUnit->SPSendMessage(MV_SET_ANI_LOOP, false);

	const char* pszTalkMessage;
	pszTalkMessage = g_pMOBCluster->GetTalkMessage(m_MapEvent.obejctImage, (ANIMATION_INDEX)iIndex);
	m_pkStatusModel->SetTalkBalloon(pszTalkMessage);

	if( m_pkFXArchive != NULL )
	{
		m_pkFXArchive->InsertFXGroup( (UINT)iIndex );
	}
}


void SPEvent::DeleteAllQuest()
{
	std::vector<SPQuestAttr*>::iterator iter;
	for(iter = m_vpQuest.begin(); iter != m_vpQuest.end(); iter++) {
		//SAFE_DELETE((*iter));
	}
	m_vpQuest.clear();

	DXUTOutputDebugString("Delete MapEvent Quest : size[%d]\n", static_cast<int>(m_vpQuest.size()));
}


void SPEvent::SetDynimicFX()
{
	if( m_MapEvent.iDynamicFX > 0 )
		m_pkFXArchive->InsertFXGroup( (SPID_FX_GROUP)m_MapEvent.iDynamicFX );
}
