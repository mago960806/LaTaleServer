// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-7-13    18:39 
//***************************************************************************

#include "SPCommon.h"

#include "SPJoyPadManager.h"
#include "SPInputManager.h"

#include "SPCommandConvert.h"
#include "SPGOBManager.h"
#include "SPGOBCoordPhysics.h"
#include "SPGameObject.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPPlayerInvenArchive.h"
#include "SPPlayer.h"

#include "SPWindowDEF.h"
#include "SPIMEPool.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "SPSkill.h"
#include "SPSkillArchive.h"

#include "SPPlayerActionManager.h"
#include "SPLocalizeManager.h"

#include "SPSubGameManager.h"

#include "autointerface.h"


SPPlayerActionManager::SPPlayerActionManager()
{
	Init();
}

SPPlayerActionManager::~SPPlayerActionManager()
{
}

//////////////////////////////////////////////////////////////////////////
//
//	싱글톤 인터페이스
//

SPPlayerActionManager* SPPlayerActionManager::m_pPlayerActionManagerInstance	=	NULL;

SPPlayerActionManager* SPPlayerActionManager::GetInstance()
{
	if( m_pPlayerActionManagerInstance	==	NULL )
		m_pPlayerActionManagerInstance	=	new	SPPlayerActionManager;

	return	m_pPlayerActionManagerInstance;
}

void SPPlayerActionManager::DelInstance()
{
	delete m_pPlayerActionManagerInstance;
}

//////////////////////////////////////////////////////////////////////////
//
//	초기화
//

void SPPlayerActionManager::Init()
{
	m_bProcessSkip		=	FALSE;
	m_uiAction			=	ACTION_STOP;
	m_uiReserveRunAction=	ACTION_STOP;
	m_iRequestSkillID	=	0;

	m_stRunControl.m_fKeyTermAccmulateTime	= 0.0f;
	m_stRunControl.m_fKeyPushAccmulateTime	= 0.0f;
	m_stRunControl.m_fKeyTermTime			= 0.2f;
	m_stRunControl.m_iDirection				= 0;

	m_ItemSkillContainer.ContainerType = CONTAINER_TYPE_INVALID;
	m_ItemSkillContainer.SlotIndex = -1;
}

//////////////////////////////////////////////////////////////////////////
//
//	Action 인터페이스
//

void SPPlayerActionManager::SetAction( UINT64 uiAction )
{
	m_uiAction	=	uiAction;
}

void SPPlayerActionManager::AddAction( UINT64 uiAction )
{
	m_uiAction	|=	uiAction;
}

void SPPlayerActionManager::DelAction( UINT64 uiAction )
{
	if( GetAction( uiAction ) == TRUE )
		m_uiAction	^=	uiAction;
}

BOOL SPPlayerActionManager::GetAction( UINT64 uiAction )
{
	return ( ( m_uiAction & uiAction ) == uiAction );
}

UINT64 SPPlayerActionManager::GetAction()
{
	return m_uiAction;
}

//////////////////////////////////////////////////////////////////////////
//
//	Action 인터페이스
//

void SPPlayerActionManager::SetSkillID( int iSkillID )
{
	m_iRequestSkillID	=	iSkillID;

	m_ItemSkillContainer.ContainerType = CONTAINER_TYPE_INVALID;
	m_ItemSkillContainer.SlotIndex = -1;
}

void SPPlayerActionManager::SetItemSkillID(int iSkillID, CONTAINER_SLOT Container)
{
	m_iRequestSkillID = iSkillID;
	m_ItemSkillContainer = Container;
}

int SPPlayerActionManager::GetSkillID(CONTAINER_SLOT& Container)
{
	Container = m_ItemSkillContainer;
	return m_iRequestSkillID;
}

//////////////////////////////////////////////////////////////////////////
//
//	ProcessSkip 인터페이스
//

void SPPlayerActionManager::SetProcessSkip( BOOL bSkip )
{
	m_bProcessSkip	=	bSkip;
}

BOOL SPPlayerActionManager::GetProcessSkip()
{
	return m_bProcessSkip;
}

//////////////////////////////////////////////////////////////////////////
//
//	Process
//

void SPPlayerActionManager::Process( float fElapsedTime )
{
	//[xialin 2008/11/07]add
	if ( CAutoInterface::GetInstance().isStart() )
		return;

	if( m_bProcessSkip == TRUE || g_pGOBManager->GetLocalPlayer() == NULL )
		return;

	if( g_pInterfaceManager->GetIMEPool()->IsFocus() == TRUE )
		return;
	
#ifdef _JOY_PAD
	if(SPLocalizeManager::GetInstance()->IsEnableValue(LV_JOYPAD)) {
		ProcessJoyPad		(fElapsedTime);
	}
#endif
	
	
	ProcessDirection	( fElapsedTime );
	ProcessEtc			();
	ProcessEnd			( fElapsedTime );
}

void SPPlayerActionManager::ProcessDirection( float fElapsedTime )
{
	SPMInputKey*	pLeftInputKey	=	g_pInputManager->GetActionMap( KEY_AM_LEFT );
	SPMInputKey*	pRightInputKey	=	g_pInputManager->GetActionMap( KEY_AM_RIGHT );
	SPMInputKey*	pUpInputKey		=	g_pInputManager->GetActionMap( KEY_AM_UP );
	SPMInputKey*	pDownInputKey	=	g_pInputManager->GetActionMap( KEY_AM_DOWN );

	//[xialin 2008/08/06] edit >>>
	SPPlayer * pkPlayer = NULL;
	if ( g_pSubGameManager->GetGameState() == GS_CASHSHOP )
	{
		g_pInterfaceManager->SPChildWndSendMessage(WIID_CASHSHOP, SPIM_CASHSHOP_GETPLAYER, (WPARAM)&pkPlayer, 0);
	}
	else
	{
		pkPlayer = g_pGOBManager->GetLocalPlayer();
	}
	if ( !pkPlayer )
		return;
	//[xialin 2008/08/06] edit <<<

	GOB_STATE_TYPE eType =	pkPlayer->GetCoordPhysics()->GetCurState();

	if( pLeftInputKey->InputState == INPUT_PRESS_REPEAT )
	{
		if( eType != GOB_STATE_RUN )
		{
			if( CheckRun( fElapsedTime , 1 ) )
				m_uiAction	|=	ACTION_RUN_LEFT;
			else
			{
				if( eType == GOB_STATE_HANGING_STAND ||
					eType == GOB_STATE_HANGING )
					m_uiAction	|=	ACTION_MOVE_HANGING_LEFT;
				else
					m_uiAction	|=	ACTION_MOVE_LEFT;
			}
		}
		else
		{
			if( m_stRunControl.m_iDirection == 1 )
				m_uiAction	|=	ACTION_RUN_LEFT;
		}
	}
	else if( pRightInputKey->InputState == INPUT_PRESS_REPEAT )
	{
		if( eType != GOB_STATE_RUN )
		{
			if( CheckRun( fElapsedTime , 2 ) )
				m_uiAction	|=	ACTION_RUN_RIGHT;
			else
			{
				if( eType == GOB_STATE_HANGING_STAND ||
					eType == GOB_STATE_HANGING )
					m_uiAction	|=	ACTION_MOVE_HANGING_RIGHT;
				else
					m_uiAction	|=	ACTION_MOVE_RIGHT;
			}
		}
		else
		{
			if( m_stRunControl.m_iDirection == 2 )
				m_uiAction	|=	ACTION_RUN_RIGHT;
		}
	}

	if( pUpInputKey->InputState == INPUT_PRESS_REPEAT )
		m_uiAction |= ACTION_MOVE_UP;
	else if( pDownInputKey->InputState == INPUT_PRESS_REPEAT )
	{
		if( eType == GOB_STATE_GUARD )
			m_uiAction = ACTION_GUARD;
		else
			m_uiAction |= ACTION_MOVE_DOWN;
	}
}

void SPPlayerActionManager::ProcessJoyPad(float fElapsedTime)
{

	//[xialin 2008/08/06] edit >>>
	SPPlayer * pkPlayer = NULL;
	if ( g_pSubGameManager->GetGameState() == GS_CASHSHOP )
	{
		g_pInterfaceManager->SPChildWndSendMessage(WIID_CASHSHOP, SPIM_CASHSHOP_GETPLAYER, (WPARAM)&pkPlayer, 0);
	}
	else
	{
		pkPlayer = g_pGOBManager->GetLocalPlayer();
	}
	if ( !pkPlayer )
		return;
	//[xialin 2008/08/06] edit <<<

	GOB_STATE_TYPE eType =	pkPlayer->GetCoordPhysics()->GetCurState();

	/*
	//[2007/5/16]-jinhee
	SPMInputBtn*	pLeftInputJS	=	g_pInputManager->GetActionMapJS( KEY_AM_AXLE_X );
	SPMInputBtn*	pUpInputJS		=	g_pInputManager->GetActionMapJS( KEY_AM_AXLE_Y );
	SPMInputBtn*	pPovInputJS		=	g_pInputManager->GetActionMapJS( KEY_AM_POV );	

	if( pLeftInputJS->iValue < -100 || (pPovInputJS->iIndex == 0 && pPovInputJS->iValue == 27000))
	{
	if( eType != GOB_STATE_RUN )
	{
	if( CheckRun( fElapsedTime , 1 ) )
	m_uiAction	|=	ACTION_RUN_LEFT;
	else
	{
	if( eType == GOB_STATE_HANGING_STAND ||
	eType == GOB_STATE_HANGING )
	m_uiAction	|=	ACTION_MOVE_HANGING_LEFT;
	else
	m_uiAction	|=	ACTION_MOVE_LEFT;
	}
	}
	else
	{
	if( m_stRunControl.m_iDirection == 1 )
	m_uiAction	|=	ACTION_RUN_LEFT;
	}
	//DXUTOutputDebugString("\t * XPosInput Value[%d]\n", pLeftInputJS->iValue);
	}
	else if( pLeftInputJS->iValue > 100 || (pPovInputJS->iIndex == 0 && pPovInputJS->iValue == 9000))
	{
	if( eType != GOB_STATE_RUN )
	{
	if( CheckRun( fElapsedTime , 2 ) )
	m_uiAction	|=	ACTION_RUN_RIGHT;
	else
	{
	if( eType == GOB_STATE_HANGING_STAND ||
	eType == GOB_STATE_HANGING )
	m_uiAction	|=	ACTION_MOVE_HANGING_RIGHT;
	else
	m_uiAction	|=	ACTION_MOVE_RIGHT;
	}
	}
	else
	{
	if( m_stRunControl.m_iDirection == 2 )
	m_uiAction	|=	ACTION_RUN_RIGHT;
	}
	//DXUTOutputDebugString("\t * XPosInput Value[%d]\n", pLeftInputJS->iValue);
	}

	if( pUpInputJS->iValue < -100 || (pPovInputJS->iIndex == 0 && pPovInputJS->iValue == 0))
	m_uiAction |= ACTION_MOVE_UP;
	else if( pUpInputJS->iValue > 100 || (pPovInputJS->iIndex == 0 && pPovInputJS->iValue == 18000))
	{
	if( eType == GOB_STATE_GUARD)
	m_uiAction = ACTION_GUARD;
	else
	m_uiAction |= ACTION_MOVE_DOWN;
	}
	*/

	if(!g_pInput->IsEquipJoystick()) 
		return;
	
	SPInputStructJoyStick* pStick = g_pInputManager->GetInputStructJoyStick();
	if(pStick == NULL)	return;

	if(g_pInputManager->GetJoyPadManager()->GetJoyStickMode() == JOYSTICK_MODE_ANALOG) {
		if(pStick->ibRgdwPOV[0].iValue < 0)
			return;

		if(pStick->ibRgdwPOV[0].iValue == 27000 || pStick->ibRgdwPOV[0].iValue == 31500) 
		{
			if( eType != GOB_STATE_RUN )
			{
				if( CheckRun( fElapsedTime , 1 ) )
					m_uiAction	|=	ACTION_RUN_LEFT;
				else
				{
					if( eType == GOB_STATE_HANGING_STAND ||
						eType == GOB_STATE_HANGING )
						m_uiAction	|=	ACTION_MOVE_HANGING_LEFT;
					else
						m_uiAction	|=	ACTION_MOVE_LEFT;
				}
			}
			else
			{
				if( m_stRunControl.m_iDirection == 1 )
					m_uiAction	|=	ACTION_RUN_LEFT;
			}
		}
		else if(pStick->ibRgdwPOV[0].iValue == 9000 || pStick->ibRgdwPOV[0].iValue == 4500)
		{
			if( eType != GOB_STATE_RUN )
			{
				if( CheckRun( fElapsedTime , 2 ) )
					m_uiAction	|=	ACTION_RUN_RIGHT;
				else
				{
					if( eType == GOB_STATE_HANGING_STAND ||
						eType == GOB_STATE_HANGING )
						m_uiAction	|=	ACTION_MOVE_HANGING_RIGHT;
					else
						m_uiAction	|=	ACTION_MOVE_RIGHT;
				}
			}
			else
			{
				if( m_stRunControl.m_iDirection == 2 )
					m_uiAction	|=	ACTION_RUN_RIGHT;
			}
		}

		if(pStick->ibRgdwPOV[0].iValue == 0 || pStick->ibRgdwPOV[0].iValue == 4500 || pStick->ibRgdwPOV[0].iValue == 31500)
			m_uiAction |= ACTION_MOVE_UP;
		else if(pStick->ibRgdwPOV[0].iValue == 18000)
		{
			if( eType == GOB_STATE_GUARD)
				m_uiAction = ACTION_GUARD;
			else
				m_uiAction |= ACTION_MOVE_DOWN;
		}
	}	
	else if(g_pInputManager->GetJoyPadManager()->GetJoyStickMode() == JOYSTICK_MODE_DIGITAL){
		if(pStick->ibLX.iValue < -200) 
		{
			if( eType != GOB_STATE_RUN )
			{
				if( CheckRun( fElapsedTime , 1 ) )
					m_uiAction	|=	ACTION_RUN_LEFT;
				else
				{
					if( eType == GOB_STATE_HANGING_STAND ||
						eType == GOB_STATE_HANGING )
						m_uiAction	|=	ACTION_MOVE_HANGING_LEFT;
					else
						m_uiAction	|=	ACTION_MOVE_LEFT;
				}
			}
			else
			{
				if( m_stRunControl.m_iDirection == 1 )
					m_uiAction	|=	ACTION_RUN_LEFT;
			}
		}
		else if(pStick->ibLX.iValue > 200)
		{
			if( eType != GOB_STATE_RUN )
			{
				if( CheckRun( fElapsedTime , 2 ) )
					m_uiAction	|=	ACTION_RUN_RIGHT;
				else
				{
					if( eType == GOB_STATE_HANGING_STAND ||
						eType == GOB_STATE_HANGING )
						m_uiAction	|=	ACTION_MOVE_HANGING_RIGHT;
					else
						m_uiAction	|=	ACTION_MOVE_RIGHT;
				}
			}
			else
			{
				if( m_stRunControl.m_iDirection == 2 )
					m_uiAction	|=	ACTION_RUN_RIGHT;
			}
		}

		if(pStick->ibLY.iValue < -200)
			m_uiAction |= ACTION_MOVE_UP;
		else if(pStick->ibLY.iValue > 200)
		{
			if( eType == GOB_STATE_GUARD)
				m_uiAction = ACTION_GUARD;
			else
				m_uiAction |= ACTION_MOVE_DOWN;
		}
	}
}

void SPPlayerActionManager::ProcessEtc()
{
	SPMInputKey*	pSwapInputKey	=	g_pInputManager->GetActionMap( KEY_AM_SWAP );

	if( pSwapInputKey->InputState == INPUT_UNPRESS )
	{
		if( g_pGOBManager->GetLocalPlayer() && g_pGOBManager->GetLocalPlayer()->GetInvenArchive() )
			g_pGOBManager->GetLocalPlayer()->GetInvenArchive()->SendReqWeaponExchange();
	}
}

void SPPlayerActionManager::ProcessEnd( float fElapsedTime )
{
	//[xialin 2008/08/06] edit >>>
	SPPlayer * pkPlayer = NULL;
	if ( g_pSubGameManager->GetGameState() == GS_CASHSHOP )
	{
		g_pInterfaceManager->SPChildWndSendMessage(WIID_CASHSHOP, SPIM_CASHSHOP_GETPLAYER, (WPARAM)&pkPlayer, 0);
	}
	else
	{
		pkPlayer = g_pGOBManager->GetLocalPlayer();
	}
	if ( !pkPlayer )
		return;
	//[xialin 2008/08/06] edit <<<

	GOB_STATE_TYPE eType =	pkPlayer->GetCoordPhysics()->GetCurState();

	if( m_uiAction == ACTION_STOP )
	{
		if( eType == GOB_STATE_VERTICAL_STAND ||
			eType == GOB_STATE_UP ||
			eType == GOB_STATE_DOWN ) 
			m_uiAction = ACTION_VERTICAL_STOP;
		else if( eType == GOB_STATE_HANGING_STAND ||
			eType == GOB_STATE_HANGING )
			m_uiAction = ACTION_HANGING_STOP;
		//else if( eType == GOB_STATE_RUN )
		//	m_uiAction = ACTION_RUN_STOP;
		else if( eType == GOB_STATE_SIT_WAIT)
			m_uiAction = ACTION_SIT_WAIT;

		//if( eType == GOB_STATE_IDLE ||
		//	eType == GOB_STATE_STAND ||
		//	eType == GOB_STATE_WALK )
			CheckRun( fElapsedTime , 0 );
		//else
		//	m_stRunControl.m_iDirection = 0;

		m_uiReserveRunAction = ACTION_STOP;
	}
	else
	{
		if( eType == GOB_STATE_SIT_WAIT )
			m_uiAction = ACTION_SIT_STAND;
	}

	if( m_uiReserveRunAction == ACTION_STOP )
	{
		if( m_uiAction & ACTION_RUN_LEFT )
			m_uiReserveRunAction = ACTION_RUN_LEFT;
		if( m_uiAction & ACTION_RUN_RIGHT ) 
			m_uiReserveRunAction = ACTION_RUN_RIGHT;
	}
	
	if( m_uiReserveRunAction )
	{
		if( (m_uiAction & ACTION_MOVE_LEFT) && (m_uiReserveRunAction & ACTION_RUN_RIGHT) )
			m_uiReserveRunAction = ACTION_STOP;
		if( (m_uiAction & ACTION_MOVE_RIGHT) && (m_uiReserveRunAction & ACTION_RUN_LEFT) )
			m_uiReserveRunAction = ACTION_STOP;
	}

	if( ((m_uiAction & ACTION_MOVE_LEFT) || (m_uiAction & ACTION_MOVE_RIGHT)) &&
		(!(m_uiAction & ACTION_ATTACK_SPEEDY) && !(m_uiAction & ACTION_ATTACK_MIGHTY)) )
	{
		if( m_uiReserveRunAction )
		{
			if( m_uiAction & ACTION_MOVE_LEFT )
			{
				m_uiAction &= ~ACTION_MOVE_LEFT;
				m_uiAction |= ACTION_RUN_LEFT;
			}
			else if( m_uiAction & ACTION_MOVE_RIGHT )
			{
				m_uiAction &= ~ACTION_MOVE_RIGHT;
				m_uiAction |= ACTION_RUN_RIGHT;
			}
		}
	}
}

BOOL SPPlayerActionManager::CheckRun( float fElapsedTime, int iDirection )
{
	if( m_stRunControl.m_iDirection )
	{
		if( m_stRunControl.m_iDirection == iDirection )
		{
			m_stRunControl.m_fKeyPushAccmulateTime += fElapsedTime;
			if( m_stRunControl.m_fKeyPushAccmulateTime >= 0.3f )
				return FALSE;
		}
	}
	else
		m_stRunControl.m_fKeyPushAccmulateTime = 0.0f;

	if( iDirection == 0 && m_stRunControl.m_iDirection != 0 )
	{
		m_stRunControl.m_fKeyTermAccmulateTime += fElapsedTime;
		if( m_stRunControl.m_fKeyTermAccmulateTime >= m_stRunControl.m_fKeyTermTime )
			m_stRunControl.m_iDirection = 0;

		if( m_stRunControl.m_fKeyPushAccmulateTime >= 0.3f )
			m_stRunControl.m_iDirection = 0;

		return FALSE;
	}

	if( m_stRunControl.m_iDirection != iDirection )
	{
		m_stRunControl.m_iDirection				= iDirection;
		m_stRunControl.m_fKeyTermAccmulateTime	= 0.0f;

		return FALSE;
	}

	if( m_stRunControl.m_fKeyTermAccmulateTime > 0.001f &&
		m_stRunControl.m_fKeyTermAccmulateTime < m_stRunControl.m_fKeyTermTime )
		return TRUE;

	return FALSE;
}
