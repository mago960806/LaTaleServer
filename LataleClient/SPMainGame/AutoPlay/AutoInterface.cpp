
#include "SPCommon.h"
#include "SPDebug.h"
#include "SPMainGameDEF.H"
#include "SPGameObjectDef.h"
#include "SPGameObject.h"

#include "SPGOBClusterDEF.h"
#include "SPGOBCluster.h"
#include "SPGOBModelUnit.h"
#include "SPAvatarModelUnit.h"
#include "SPAvatarModelUnitDef.h"
//#include "SPItem.h"
#include "SPItemCluster.h"
#include "SPItemAttr.h"
#include "SPItemStatus.h"
#include "SPItemStatusEquip.h"
#include "SPPlayerEquipInfo.h"
#include "SPGOBCoordPhysics.h"
#include "SPPlayerCoordPhysics.h"
#include "SPGOBStatus.h"
#include "SPPlayerStatus.h"
#include "SPPlayerStatusModel.h"
#include "SPCommandConvert.h"

#include "SPGameObjectDEF.h"
#include "SPManagerDef.h"
#include "SPManager.h"
#include "SPGOBManager.h"

#include <WinSock2.h>
#include "PacketID.h"
#include "GlobalDefines_Share.h"
#include "Packet.h"
#include "PacketHandler.h"

#include "SPSubGameManager.h"
#include "SPMainGameManager.h"
#include "SPCheckManager.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "SPUIUnit.h"
#include "SPUIItemUnit.h"
#include "SPUISkillUnit.h"
#include "SPUIUnitManager.h"

#include "SPTerrainDEF.h"
#include "SPTerrainCluster.h"
#include "SPTerrainManager.h"

#include "SPStage.h"
#include "SPStageManager.h"
#include "SPTerrainAttribute.h"

#include "autointerface.h"


CAutoInterface::CAutoInterface(void)
{
	m_eAutoStatus   = AUTO_MOVE;
	m_eLastStatus	= AUTO_MOVE;
	m_bAutoPlay		= false;
	m_bAutoRecord	= false;
	m_iCurSkillIndex   = 0;
	m_iCurActionIndex  = 0;

	m_pLocalPlayer			=	NULL;
	m_pPlayerActionManager  =   NULL;

	m_vSkillID.clear();				//玩家使用的技能
	m_vSkillID2.clear();			//玩家使用的辅助技能
	m_vHPItemID.clear();			//玩家使用的红
	m_vSPItemID.clear();			//玩家使用的兰
	m_vReturnTtemID.clear();		//玩家使用的回城物品

	m_iMonsterDst	= 400;			//寻怪距离
	m_iPlayerDstPos.x = 0;
	m_iPlayerDstPos.y = 0;
	m_eFacing	= FACING_LEFT;

	m_eLastGobStateType = GOB_STATE_NULL;

	m_vActionList.clear();

}

CAutoInterface::~CAutoInterface(void)
{
	if ( m_pkSkillUsed )
		delete m_pkSkillUsed;
}

//=========================================================================
bool   CAutoInterface::Initial()
{
	m_pkSkillUsed = new SPSkillUsed;
	return true;
}

//=========================================================================
void   CAutoInterface::InitQuickSlot()
{
	m_vSkillID.clear();				//玩家使用的技能
	m_vSkillID2.clear();			//玩家使用的辅助技能
	m_vHPItemID.clear();			//玩家使用的红
	m_vSPItemID.clear();			//玩家使用的兰
	m_vReturnTtemID.clear();		//玩家使用的回城物品

	SPWindow * pWindow = g_pInterfaceManager->FindWindow(WIID_QUICK_SLOT);
	if ( !pWindow )
		return;

	SPUIUnitManager	*	pQuickSlotUnitManager  = NULL;
	pWindow->SPSendMessage( SPIM_GET_QUICK_SLOT_UNIT, (WPARAM)&pQuickSlotUnitManager);
	if ( pQuickSlotUnitManager == NULL )
		return;

	//辅助技能
	for ( int i=0; i<=5; i++ )
	{
		SPUIUnit* pUnit	= pQuickSlotUnitManager->GetUnit(i);
		if ( pUnit )
			m_vSkillID2.push_back( i );
	}

	//红
	for ( int i=6; i<=8; i++ )
	{
		SPUIUnit* pUnit	= pQuickSlotUnitManager->GetUnit(i);
		if ( pUnit )
			m_vHPItemID.push_back( i );
	}

	//兰
	for ( int i=9; i<=11; i++ )
	{
		SPUIUnit* pUnit	= pQuickSlotUnitManager->GetUnit(i);
		if ( pUnit )
			m_vSPItemID.push_back( i );
	}

	//攻击技能
	m_iCurSkillIndex   = 0;
	for ( int i=12; i<=21; i++ )
	{
		SPUIUnit* pUnit	= pQuickSlotUnitManager->GetUnit(i);
		if ( pUnit )
			m_vSkillID.push_back( i );
	}

	//回城物品
	m_iCurSkillIndex   = 0;
	for ( int i=22; i<=23; i++ )
	{
		SPUIUnit* pUnit	= pQuickSlotUnitManager->GetUnit(i);
		if ( pUnit )
			m_vReturnTtemID.push_back( i );
	}
}

//=========================================================================
void   CAutoInterface::Destroy()
{

}

//=========================================================================
void   CAutoInterface::Process(float fTime)
{
	if ( !m_bAutoPlay )
		return;

	m_pLocalPlayer			=	g_pGOBManager->GetLocalPlayer();
	m_pPlayerActionManager  =   SPPlayerActionManager::GetInstance();
	if ( !m_pLocalPlayer || !m_pPlayerActionManager )
		return;

	float fHP   = GetPlayerStatus(STATUS_HP);
    float fSP   = GetPlayerStatus(STATUS_SP);
	float fMHP  = GetPlayerStatus(STATUS_MHP);
	float fMSP  = GetPlayerStatus(STATUS_MSP);

	//补充HP
	if ( fHP / fMHP <= 0.4f )
	{
		for ( int i = 2; i >= 0; i-- )
			if ( UseHPItem(i) )
				break;
	}
	else if ( fHP / fMHP <= 0.6f )
	{
		for ( int i = 1; i >= 0; i-- )
			if ( UseHPItem(i) )
				break;
	}
	else if ( fHP / fMHP <= 0.8f )
	{
		UseHPItem(0);
	}

	//补充SP
	if ( fSP / fMSP <= 0.4f )
	{
		for ( int i = 2; i >= 0; i-- )
			if ( UseSPItem(i) )
				break;
	}
	else if ( fSP / fMSP <= 0.6f )
	{
		for ( int i = 1; i >= 0; i-- )
			if ( UseSPItem(i) )
				break;
	}
	else if ( fSP / fMSP <= 0.8f )
	{
		UseSPItem(0);
	}

	//回城处理
	if ( fHP / fMHP <= 0.2f )
		UseReturnItem();

	//辅助技功
	UseSkill2();

	//跳跃保护
	if ( fHP / fMHP <= 0.3f )
	{
		SetAction(ACTION_MOVE_JUMP);
	}
	else
	{
		//攻击，寻怪
		if ( UseSkillAttack() == false )
		{
			FindMonster();
			FindActionIndex();
			Move( fTime );
		}
	}

	m_eLastGobStateType = m_pLocalPlayer->GetCoordPhysics()->GetCurState();
	
}

//=========================================================================
void   CAutoInterface::Render( float fTime )
{
	if ( isRecord() )
	{
		RECT    rcText = { 20, 100, 300, 120 };
		g_pVideo->GetFont(FONT_18_BOLD)->SetColor( 0xFFFF0000 );
		g_pVideo->GetFont(FONT_18_BOLD)->SetShadow(	true );
		g_pVideo->GetFont(FONT_18_BOLD)->RenderText( "正在录制玩家动作..." , &rcText, DT_LEFT | DT_VCENTER );
	}

	if ( isStart() )
	{
		RECT    rcText = { 20, 120, 300, 140 };
		g_pVideo->GetFont(FONT_18_BOLD)->SetColor( 0xFFFF0000 );
		g_pVideo->GetFont(FONT_18_BOLD)->SetShadow(	true );
		g_pVideo->GetFont(FONT_18_BOLD)->RenderText( "自动练级中..." , &rcText, DT_LEFT | DT_VCENTER );
	}
}

//=========================================================================
void   CAutoInterface::Start(int iOper)
{
	m_pLocalPlayer			=	g_pGOBManager->GetLocalPlayer();
	m_pPlayerActionManager  =   SPPlayerActionManager::GetInstance();
	if ( !m_pLocalPlayer || !m_pPlayerActionManager )
		return;

	if ( iOper == 1 )
	{
		if ( isStart() )
			return;

		m_vActionList.clear();
		m_iCurActionIndex  = 0;
		m_bAutoRecord   = true;
	}
	else if ( iOper == 2 )
	{
		if ( isRecord() )
			return;

		InitQuickSlot();
		m_bAutoPlay		= true;
		m_eAutoStatus   = AUTO_MOVE;
		m_eLastStatus	= AUTO_MOVE;

		if ( m_pLocalPlayer )
		{
			m_eFacing = m_pLocalPlayer->GetDirection();
			m_eLastGobStateType = m_pLocalPlayer->GetCoordPhysics()->GetCurState();
		}
	}
}

//=========================================================================
void   CAutoInterface::Stop(int iOper)
{
	if ( iOper == 1 )
	{
		m_bAutoRecord   = false;
	}
	else if ( iOper == 2 )
	{
		m_bAutoPlay		= false;
	}
}

//=========================================================================
void   CAutoInterface::RecordAction(UINT64 uiAction)
{
	if ( isRecord() == false )
		return;

	m_pLocalPlayer			=	g_pGOBManager->GetLocalPlayer();
	if ( !m_pLocalPlayer )
		return;

	ACTION_LIST   action;
	action.fPosX	= m_pLocalPlayer->GetPosX();
	action.fPosY	= m_pLocalPlayer->GetPosY();
	action.eFacing	= m_pLocalPlayer->GetDirection();
	action.uiAction = uiAction;
	m_vActionList.push_back(action);
}

//=========================================================================
bool   CAutoInterface::CheckAttackRgn(int iSkillID)
{
	if ( !m_pLocalPlayer || !m_pkSkillUsed )
		return false;

	GOB_STATE_TYPE  eGobStateType	= m_pLocalPlayer->GetCoordPhysics()->GetCurState();
	if ( eGobStateType == GOB_STATE_UP
		|| eGobStateType == GOB_STATE_DOWN
		|| eGobStateType == GOB_STATE_VERTICAL_STAND )
		return false;

	SPSkill * pSkill = SPSkillManager::GetInstance()->GetSkill(iSkillID);
	if ( pSkill )
	{
		m_pkSkillUsed->SetParent( m_pLocalPlayer );
		if ( SPGameObject* pGameObject = m_pkSkillUsed->CheckAttackRgn(pSkill) )
		{
			if ( m_pLocalPlayer->GetPosX() > pGameObject->SPGOBPerformMessage(SPGM_GETPOSX) )
				SetAction(ACTION_MOVE_LEFT);
			else
				SetAction(ACTION_MOVE_RIGHT);
			return true;
		}
	}
	else
	{
		SPActionInterface*	pActionInterface = SPSkillManager::GetInstance()->GetActionInterface( iSkillID );
		if( pActionInterface == NULL )
			return false;

		int iActionCommand	=	pActionInterface->GetActionCommand();
		int iIndex			=	m_pLocalPlayer->GetAniIndex(iActionCommand);

		m_pkSkillUsed->SetParent( m_pLocalPlayer );
		if ( SPGameObject* pGameObject = m_pkSkillUsed->CheckAttackRgn(iIndex) )
		{
			if ( m_pLocalPlayer->GetPosX() > pGameObject->SPGOBPerformMessage(SPGM_GETPOSX) )
				SetAction(ACTION_MOVE_LEFT);
			else
				SetAction(ACTION_MOVE_RIGHT);
			return true;
		}
	}

	return false;
}

//=========================================================================
bool   CAutoInterface::FindMonster()
{
	if ( m_eAutoStatus == AUTO_MOVETO )
		return false;

	GOB_STATE_TYPE  eGobStateType	= m_pLocalPlayer->GetCoordPhysics()->GetCurState();
	if ( eGobStateType == GOB_STATE_UP
		|| eGobStateType == GOB_STATE_DOWN
		|| eGobStateType == GOB_STATE_JUMP
		|| eGobStateType == GOB_STATE_DROP 
		|| eGobStateType == GOB_STATE_VERTICAL_STAND )
		return false;

	int iGOBCount = 0;
	int iTargetPosX, iTargetPosY;
	std::vector< SPGameObject* > vGOBList;

	int    iCasterPosX		= m_pLocalPlayer->SPGOBPerformMessage(SPGM_GETPOSX);
	int    iCasterPosY		= m_pLocalPlayer->SPGOBPerformMessage(SPGM_GETPOSY);
	FACING eCasterFacing	= (FACING)m_pLocalPlayer->SPGOBPerformMessage(SPGM_GETFACING);

	iGOBCount = g_pGOBManager->GetAllMonster(vGOBList);

	std::vector< SPGameObject* >::iterator iter = vGOBList.begin();
	for( ; iter != vGOBList.end(); ++iter )
	{
		if( (*iter)->SPGOBPerformMessage(SPGM_ISDEAD) )
			continue;

		if( (*iter)->SPGOBPerformMessage(SPGM_GETTYPE) > 100 )
			continue;

		if( (*iter)->SPGOBPerformMessage(SPGM_GETTYPE) == 21 )
			continue;

		if( (*iter)->IsShowName() == false )
			continue;

		if( (*iter)->IsShowHP() == false )
			continue;

		iTargetPosX = (*iter)->SPGOBPerformMessage(SPGM_GETPOSX);
		iTargetPosY = (*iter)->SPGOBPerformMessage(SPGM_GETPOSY);

		if ( abs(iCasterPosX - iTargetPosX) <= m_iMonsterDst 
			&& abs(iCasterPosY - iTargetPosY) <= 150 )
		{
			m_iPlayerDstPos.x = iTargetPosX;
			m_iPlayerDstPos.y = iTargetPosY;

			m_eLastStatus	  = AUTO_MOVE;
			m_eAutoStatus     = AUTO_MOVETO;
			return true;
		}
	}

	return false;
}

//=========================================================================
bool  CAutoInterface::FindActionIndex()
{
	if ( m_eAutoStatus == AUTO_MOVETO && m_eLastStatus == AUTO_MOVE )
		return false;

	int		iCasterPosX				= m_pLocalPlayer->SPGOBPerformMessage(SPGM_GETPOSX);
	int		iCasterPosY				= m_pLocalPlayer->SPGOBPerformMessage(SPGM_GETPOSY);
	FACING  eCasterFacing			= (FACING)m_pLocalPlayer->SPGOBPerformMessage(SPGM_GETFACING);
	GOB_STATE_TYPE  eGobStateType	= m_pLocalPlayer->GetCoordPhysics()->GetCurState();
	GOB_SEQ_INDEX   eGobSeqIndex	= m_pLocalPlayer->GetCurSeqIndex();
	int		iCount					= (int)m_vActionList.size();

	//根据速度计算灵敏度
	float fVelocityX = m_pLocalPlayer->GetCoordPhysics()->GetCurVelocityX();
	float fVelocityY = m_pLocalPlayer->GetCoordPhysics()->GetCurVelocityY();
	float fAgileX  =  0.3 * (abs(fVelocityX)/12);
	float fAgileY  =  0.3 * (abs(fVelocityY)/12);
	fAgileX = fAgileX <= 1.0f ? 3.0f : fAgileX;
	fAgileY = fAgileY <= 1.0f ? 3.0f : fAgileY;
	
	//继续
	if ( m_eAutoStatus == AUTO_MOVE_RECORD )
	{
		int  iIndex = m_iCurActionIndex;
		while ( 1 )
		{
			iIndex++;
			if ( iIndex >= iCount )
				iIndex = 0;

			int		iTargetPosX		= (int)m_vActionList[iIndex].fPosX;
			int		iTargetPosY		= (int)m_vActionList[iIndex].fPosY;
			FACING  eTargetFacing	= m_vActionList[iIndex].eFacing;
			UINT64  uiNextAction	= m_vActionList[iIndex].uiAction;
			UINT64  uiLastAction    = m_vActionList[m_iCurActionIndex].uiAction;

			//向上处理
			if ( (eGobStateType == GOB_STATE_UP && eGobSeqIndex == GOB_SEQ_VERROPE_MOVE_UP)
				|| (eGobStateType == GOB_STATE_UP && eGobSeqIndex == GOB_SEQ_LADDER_MOVE_UP) )
			{
				if ( !(uiNextAction & ACTION_MOVE_JUMP) && !(uiNextAction & ACTION_MOVE_DOWN) )
				{
					m_vActionList[iIndex].uiAction |= ACTION_MOVE_UP;
				}
			}

			//向下处理
			if ( (eGobStateType == GOB_STATE_DOWN && eGobSeqIndex == GOB_SEQ_VERROPE_MOVE_DOWN)
				|| (eGobStateType == GOB_STATE_DOWN && eGobSeqIndex == GOB_SEQ_LADDER_MOVE_DOWN) )
			{
				if ( !(uiNextAction & ACTION_MOVE_JUMP) && !(uiNextAction & ACTION_MOVE_UP) )
				{
					m_vActionList[iIndex].uiAction |= ACTION_MOVE_DOWN;
				}
			}

			//计算下一动作
			if ( abs(iCasterPosY - iTargetPosY) <= fAgileY )
			{
				int   iDisX = iTargetPosX - iCasterPosX;
				if ( eCasterFacing == FACING_LEFT )
					iDisX = iCasterPosX - iTargetPosX;

				if ( eCasterFacing == eTargetFacing
					&& iDisX <= fAgileX )
				{
					m_iCurActionIndex = iIndex;
					m_eAutoStatus = AUTO_MOVE_RECORD;
					return true;
				}
			}
			
			//动作纠正
			if ( uiLastAction == ACTION_STOP
				|| uiLastAction & ACTION_GUARD
				|| uiLastAction & ACTION_VERTICAL_STOP
				|| uiLastAction & ACTION_HANGING_STOP )
			{
				m_iCurActionIndex = iIndex;
				m_eAutoStatus = AUTO_MOVE_RECORD;
				return true;
			}

			//停止纠正
			if ( eGobStateType == GOB_STATE_NULL
				|| eGobStateType == GOB_STATE_STAND
				|| eGobStateType == GOB_STATE_GUARD
				|| eGobStateType == GOB_STATE_IDLE
				|| eGobStateType == GOB_STATE_HANGING_STAND
				|| eGobStateType == GOB_STATE_VERTICAL_STAND )
			{
				if ( eGobStateType == GOB_STATE_VERTICAL_STAND )
				{
					m_iCurActionIndex = iIndex;
					m_eAutoStatus = AUTO_MOVE_RECORD;
					return true;
				}
				else if ( (eCasterFacing == FACING_LEFT && iCasterPosX > iTargetPosX + fAgileX)
					|| (eCasterFacing == FACING_RIGHT && iCasterPosX < iTargetPosX - fAgileX) )
				{
					m_iPlayerDstPos.x = iTargetPosX;
					m_iPlayerDstPos.y = iTargetPosY;

					m_eLastStatus	  = AUTO_MOVE_RECORD;
					m_eAutoStatus     = AUTO_MOVETO;
				}
			}

			return false;
		}
	}
	//改变状态后寻找新节点
	else
	{
		for ( int i=m_iCurActionIndex; i<iCount; i++ )
		{
			int		iTargetPosX = (int)m_vActionList[i].fPosX;
			int		iTargetPosY = (int)m_vActionList[i].fPosY;
			FACING  eTargetFacing	= m_vActionList[i].eFacing;

			if ( eCasterFacing == eTargetFacing
				&& abs(iCasterPosX - iTargetPosX) <= fAgileX
				&& abs (iCasterPosY - iTargetPosY) <= fAgileY )
			{
				if ( m_vActionList[i].uiAction == ACTION_STOP
					|| m_vActionList[i].uiAction & ACTION_GUARD )
				{
					continue;
				}

				m_iCurActionIndex = i;
				m_eAutoStatus = AUTO_MOVE_RECORD;
				return true;
			}
		}
		for ( int i=0; i<m_iCurActionIndex; i++ )
		{
			int		iTargetPosX = (int)m_vActionList[i].fPosX;
			int		iTargetPosY = (int)m_vActionList[i].fPosY;
			FACING  eTargetFacing	= m_vActionList[i].eFacing;

			if ( eCasterFacing == eTargetFacing
				&& abs(iCasterPosX - iTargetPosX) <= fAgileX
				&& abs (iCasterPosY - iTargetPosY) <= fAgileY )
			{
				if ( m_vActionList[i].uiAction == ACTION_STOP
					|| m_vActionList[i].uiAction & ACTION_GUARD )
				{
					continue;
				}

				m_iCurActionIndex = i;
				m_eAutoStatus = AUTO_MOVE_RECORD;
				return true;
			}
		}
	}

	return false;
}

//=========================================================================
void  CAutoInterface::Move(float fTime)
{
	int    iCasterPosX				= m_pLocalPlayer->SPGOBPerformMessage(SPGM_GETPOSX);
	int    iCasterPosY				= m_pLocalPlayer->SPGOBPerformMessage(SPGM_GETPOSY);
	FACING eCasterFacing			= m_pLocalPlayer->GetDirection();
	GOB_STATE_TYPE  eGobStateType	= m_pLocalPlayer->GetCoordPhysics()->GetCurState();
	GOB_SEQ_INDEX   eGobSeqIndex	= m_pLocalPlayer->GetCurSeqIndex();

	bool   bMoveEnable  =  GetMoveEnable();
	bool   bCheckBlock  =  CheckBlock(0);

	//左右纠正
	static float s_fCurBlockTime = 0;
	if ( bMoveEnable == false || bCheckBlock == false )
	{
		s_fCurBlockTime += fTime;
		if ( s_fCurBlockTime > 1.0f )
		{
			UINT64  uiAction  =  ACTION_RUN_LEFT;
			if ( eCasterFacing == FACING_LEFT )
				uiAction  =  ACTION_RUN_RIGHT;
			SetAction(uiAction);

			s_fCurBlockTime	= 0;
			m_eAutoStatus = AUTO_MOVE;
			return;
		}
	}
	else
	{
		s_fCurBlockTime	= 0;
	}

	//停止纠正
	static float s_fCurStopTime = 0;
	if ( eGobStateType == GOB_STATE_NULL
		|| eGobStateType == GOB_STATE_STAND
		|| eGobStateType == GOB_STATE_GUARD
		|| eGobStateType == GOB_STATE_IDLE
		|| eGobStateType == GOB_STATE_HANGING_STAND
		|| eGobStateType == GOB_STATE_VERTICAL_STAND )
	{
		s_fCurStopTime += fTime;
		if ( s_fCurStopTime > 1.0f )
		{
			s_fCurStopTime = 0.0f;
			m_eAutoStatus = AUTO_MOVE;
		}
	}
	else
	{
		s_fCurStopTime = 0;
	}

	//用于攻击结束后方向处理
	if ( eGobStateType == GOB_STATE_ATTACK_SPEEDY
		|| eGobStateType == GOB_STATE_ATTACK_MIGHTY
		|| eGobStateType == GOB_STATE_SKILL )
	{
		m_bLastAttack = true;
		m_eAutoStatus = AUTO_MOVE;
	}
	else
	{
		m_bLastAttack = false;
	}

	//移动处理
	if ( m_eAutoStatus == AUTO_MOVE )
	{
		UINT64  uiAction  =  ACTION_RUN_LEFT;
		if ( eGobStateType == GOB_STATE_HANGING_STAND )
			uiAction  =  ACTION_MOVE_HANGING_LEFT;

		if ( eCasterFacing == FACING_RIGHT )
		{
			if ( eGobStateType == GOB_STATE_HANGING_STAND )
				uiAction  =  ACTION_MOVE_HANGING_RIGHT;
			else
				uiAction  =  ACTION_RUN_RIGHT;
		}

		if ( bCheckBlock == false )
			uiAction |= ACTION_MOVE_JUMP;
		SetAction(uiAction);
	}
	else if ( m_eAutoStatus == AUTO_MOVE_RECORD )
	{
		if ( m_iCurActionIndex >=0 && m_iCurActionIndex < (int)m_vActionList.size() )
		{
			UINT64 uiAction	= m_vActionList[m_iCurActionIndex].uiAction;
			if ( uiAction & ACTION_MOVE_DOWN )
				if ( CheckBlock(2) == false 
					&& eGobStateType != GOB_STATE_DROP
					&& eGobStateType != GOB_STATE_HANGING
					&& eGobStateType != GOB_STATE_HANGING_STAND )
					return;

			SetAction( uiAction );
		}
	}
	else if ( m_eAutoStatus == AUTO_MOVETO )
	{
		float fAgileX = 10;
		float fAgileY = 10;
		if ( m_eLastStatus == AUTO_MOVE_RECORD )
		{
			float fVelocityX = m_pLocalPlayer->GetCoordPhysics()->GetCurVelocityX();
			float fVelocityY = m_pLocalPlayer->GetCoordPhysics()->GetCurVelocityY();
			fAgileX  =  0.3 * (abs(fVelocityX)/12);
			fAgileY  =  0.3 * (abs(fVelocityY)/12);
			fAgileX = fAgileX <= 1.0f ? 3.0f : fAgileX;
			fAgileY = fAgileY <= 1.0f ? 3.0f : fAgileY;
		}

		UINT64 uiAction  =  ACTION_STOP;
		if ( iCasterPosX > m_iPlayerDstPos.x + fAgileX )
		{
			uiAction = ACTION_MOVE_LEFT;
			if ( iCasterPosX - (m_iPlayerDstPos.x + fAgileX) > 100 )
				uiAction = ACTION_RUN_LEFT;
			if ( eGobStateType == GOB_STATE_HANGING_STAND )
				uiAction  =  ACTION_MOVE_HANGING_LEFT;
			else if ( m_eLastStatus == AUTO_MOVE_RECORD )
			{
				int iIndex = m_iCurActionIndex+1;
				iIndex = iIndex >= m_vActionList.size() ? m_iCurActionIndex : iIndex;
				if ( m_vActionList[iIndex].uiAction & ACTION_RUN_LEFT )
					uiAction = ACTION_RUN_LEFT;
			}
		}
		else if ( iCasterPosX < m_iPlayerDstPos.x - fAgileX )
		{
			uiAction = ACTION_MOVE_RIGHT;
			if ( (m_iPlayerDstPos.x - fAgileX) - iCasterPosX > 100 )
				uiAction = ACTION_RUN_RIGHT;
			if ( eGobStateType == GOB_STATE_HANGING_STAND )
				uiAction  =  ACTION_MOVE_HANGING_RIGHT;
			else if ( m_eLastStatus == AUTO_MOVE_RECORD )
			{
				int iIndex = m_iCurActionIndex+1;
				iIndex = iIndex >= m_vActionList.size() ? m_iCurActionIndex : iIndex;
				if ( m_vActionList[iIndex].uiAction & ACTION_RUN_RIGHT )
					uiAction = ACTION_RUN_RIGHT;
			}
		}
		else if ( (iCasterPosY > m_iPlayerDstPos.y + fAgileY) && CheckBlock(1) )
			uiAction = ACTION_MOVE_UP;
		else if ( (iCasterPosY < m_iPlayerDstPos.y - fAgileY) && CheckBlock(2) )
			uiAction = ACTION_MOVE_DOWN;
		else
			m_eAutoStatus = m_eLastStatus;

		if ( bCheckBlock == false )
			uiAction |= ACTION_MOVE_JUMP;
		SetAction(uiAction);
	}

	//上下纠正
	if ( m_eAutoStatus != AUTO_MOVE_RECORD )
	{
		if ( eGobStateType == GOB_STATE_VERTICAL_STAND )
			m_eAutoStatus = AUTO_MOVE_RECORD;
	}
}

//=========================================================================
//=========================================================================
float  CAutoInterface::GetPlayerPosX()
{
	if ( !m_pLocalPlayer )
		return 0;

	return m_pLocalPlayer->GetPosX();
}

//=========================================================================
float  CAutoInterface::GetPlayerPosY()
{
	if ( !m_pLocalPlayer )
		return 0; 

	return m_pLocalPlayer->GetPosY();
}

//=========================================================================
int  CAutoInterface::GetDirection()
{
	if ( !m_pLocalPlayer )
		return 0; 

	SPGOBCoordPhysics * pCoordPhysics = m_pLocalPlayer->GetCoordPhysics();
	if ( !pCoordPhysics )
		return 0;

	return pCoordPhysics->GetDirection();
}

//=========================================================================
float  CAutoInterface::GetMapWidth()
{
	if ( !m_pLocalPlayer )
		return 0; 

	UINT   uiMapGroupID = m_pLocalPlayer->GetCurMapGroupID();
	UINT   uiStageID	= m_pLocalPlayer->GetCurStageID();
	SPMapGroup* pMapGroup   = g_StageManager.GetMapGroup( uiMapGroupID, uiStageID );
	if ( pMapGroup != NULL )
	{
		SPMapInfo* pMapInfo = pMapGroup->FindMapInfo( m_pLocalPlayer->GetPosX(), m_pLocalPlayer->GetPosY() );
		if ( pMapInfo != NULL )
		{
			return pMapInfo->fMapWidth;
		}
	}

	return 0;
}

//=========================================================================
float  CAutoInterface::GetMapHeight()
{
	if ( !m_pLocalPlayer )
		return 0; 

	UINT   uiMapGroupID = m_pLocalPlayer->GetCurMapGroupID();
	UINT   uiStageID	= m_pLocalPlayer->GetCurStageID();
	SPMapGroup* pMapGroup   = g_StageManager.GetMapGroup( uiMapGroupID, uiStageID );
	if ( pMapGroup != NULL )
	{
		SPMapInfo* pMapInfo = pMapGroup->FindMapInfo( m_pLocalPlayer->GetPosX(), m_pLocalPlayer->GetPosY() );
		if ( pMapInfo != NULL )
		{
			return pMapInfo->fMapHeight;
		}
	}

	return 0;
}

//=========================================================================
UINT  CAutoInterface::GetPlayerStatus( STATUS_TYPE type )
{
	if ( !m_pLocalPlayer )
		return 0; 

	SPPlayerStatus * pStatus = (SPPlayerStatus *)m_pLocalPlayer->GetStatus();
	if ( !pStatus )
		return 0;

	return pStatus->GetStatusValue( type );
}

//=========================================================================
bool  CAutoInterface::GetMoveEnable()
{
	if ( !m_pLocalPlayer )
		return false; 

	SPGOBCoordPhysics * pCoordPhysics = m_pLocalPlayer->GetCoordPhysics();
	if ( !pCoordPhysics )
		return false;

	float	fTerrainLenX	=	g_pTerrainManager->GetSX();
	float	fPlayerPosX		=   m_pLocalPlayer->GetPosX();

	int    iCasterPosX		= m_pLocalPlayer->SPGOBPerformMessage(SPGM_GETPOSX);
	if ( fTerrainLenX / 2 >  iCasterPosX )		//越过中线时改变目标方向
		m_eFacing = FACING_RIGHT;
	else
		m_eFacing = FACING_LEFT;

	bool	bEnableLeft  = !( fPlayerPosX <= 34 );
	bool	bEnableRight = !( fPlayerPosX >= fTerrainLenX-34 );

	return bEnableLeft && bEnableRight;
}

//=========================================================================
//0:根根左右方向计算 1:上, 2:下
bool CAutoInterface::CheckBlock(int iDir)
{
	if ( !m_pLocalPlayer )
		return false; 

	SPPlayerCoordPhysics * pCoordPhysics = (SPPlayerCoordPhysics *)m_pLocalPlayer->GetCoordPhysics();
	if ( !pCoordPhysics )
		return false;

	float   fPosX = 0.0f;
	float   fPosY = 0.0f;
	float	fPlayerPosX		=   m_pLocalPlayer->GetPosX();
	float	fPlayerPosY		=   m_pLocalPlayer->GetPosY();
	FACING  eCasterFacing	=	m_pLocalPlayer->GetDirection();

	if ( iDir == 0 )  //左右检测
	{
		if ( eCasterFacing == FACING_LEFT )
			fPosX = fPlayerPosX - 30.0f;
		else
			fPosX = fPlayerPosX + 30.0f;
		fPosY = fPlayerPosY;
		return pCoordPhysics->GetMoveEnable(fPosX, fPosY, 0);
	}
	else if ( iDir == 1 )  //上检测
	{
		fPosX = fPlayerPosX;
		fPosY = fPlayerPosY - 15.0f;
		return pCoordPhysics->GetMoveEnable(fPosX, fPosY, 1);
	}
	else if ( iDir == 2 )  //下检测
	{
		fPosX = fPlayerPosX;
		fPosY = fPlayerPosY + 15.0f;
		return pCoordPhysics->GetMoveEnable(fPosX, fPosY, 2);
	}

	return false;
}

//=========================================================================
void  CAutoInterface::SetAction( UINT64 uiAction )
{
	m_pPlayerActionManager->SetAction( uiAction );
}

//=========================================================================
UINT64 CAutoInterface::GetAction()
{
	if ( !m_pLocalPlayer )
		return 0; 

	return m_pLocalPlayer->GetCurAction();
}

//=========================================================================
void  CAutoInterface::SetSkillAction( int iSkillID )
{
	m_pPlayerActionManager->AddAction( ACTION_SKILL );
	m_pPlayerActionManager->SetSkillID( iSkillID );
}

//=========================================================================
bool  CAutoInterface::UseSkillAttack()
{
	if ( !g_pCheckManager )
		return false;

	GOB_STATE_TYPE  eGobStateType	= m_pLocalPlayer->GetCoordPhysics()->GetCurState();
	if ( eGobStateType == GOB_STATE_JUMP
		|| eGobStateType == GOB_STATE_DROP 
		|| eGobStateType == GOB_STATE_VERTICAL_STAND 
		|| eGobStateType == GOB_STATE_HANGING_STAND )
		return false;

	SPWindow * pWindow = g_pInterfaceManager->FindWindow(WIID_QUICK_SLOT);
	if ( !pWindow )
		return false;

	SPUIUnitManager	*	pQuickSlotUnitManager  = NULL;
	pWindow->SPSendMessage( SPIM_GET_QUICK_SLOT_UNIT, (WPARAM)&pQuickSlotUnitManager);
	if ( pQuickSlotUnitManager == NULL )
		return false;

	bool bUseSkill    = false;
	int  iCount  = m_vSkillID.size();
	for ( int i = m_iCurSkillIndex; i < iCount; i++ )
	{
		SPUIUnit  * pUIUnit = pQuickSlotUnitManager->GetUnit(m_vSkillID[i]);
		if ( pUIUnit && pUIUnit->GetInstanceID() == WIID_SKILL )
		{
			SPUISkillUnit * pSkillUnit = (SPUISkillUnit *)pUIUnit;
			int iSkillID = pSkillUnit->GetSkillID();
			if ( ( !SPSkillManager::GetInstance()->GetSkill(iSkillID)    //为了普通攻击
				  || g_pCheckManager->CheckSkillUseRequire( iSkillID ) )
				&& CheckAttackRgn(iSkillID) )
			{
				if ( pSkillUnit->Use() )
				{
					bUseSkill = true;
					m_iCurSkillIndex = i+1;
					m_iCurSkillIndex = m_iCurSkillIndex >= iCount ? 0 : m_iCurSkillIndex;
					break;
				}
			}
		}
	}

	if ( !bUseSkill )
	{
		for ( int i = 0; i < m_iCurSkillIndex; i++ )
		{
			SPUIUnit  * pUIUnit = pQuickSlotUnitManager->GetUnit(m_vSkillID[i]);
			if ( pUIUnit && pUIUnit->GetInstanceID() == WIID_SKILL )
			{
				SPUISkillUnit * pSkillUnit = (SPUISkillUnit *)pUIUnit;
				int iSkillID = pSkillUnit->GetSkillID();
				if ( ( !SPSkillManager::GetInstance()->GetSkill(iSkillID)    //为了普通攻击
					 || g_pCheckManager->CheckSkillUseRequire( iSkillID ) )
					&& CheckAttackRgn(iSkillID) )
				{
					if ( pSkillUnit->Use() )
					{
						bUseSkill = true;
						m_iCurSkillIndex = i+1;
						m_iCurSkillIndex = m_iCurSkillIndex >= iCount ? 0 : m_iCurSkillIndex;
						break;
					}
				}
			}
		}
	}

	if ( m_iCurSkillIndex >= iCount )
		m_iCurSkillIndex = 0;

	return bUseSkill;
}

//=========================================================================
bool  CAutoInterface::UseHPItem(int iIndex)
{
	if ( iIndex < 0 || iIndex >= m_vHPItemID.size() )
		return false;

	SPWindow * pWindow = g_pInterfaceManager->FindWindow(WIID_QUICK_SLOT);
	if ( !pWindow )
		return false;

	SPUIUnitManager	*	pQuickSlotUnitManager  = NULL;
	pWindow->SPSendMessage( SPIM_GET_QUICK_SLOT_UNIT, (WPARAM)&pQuickSlotUnitManager);
	if ( pQuickSlotUnitManager == NULL )
		return false;

	bool bUse = false;
	SPUIUnit  * pItemUnit = pQuickSlotUnitManager->GetUnit(m_vHPItemID[iIndex]);
	if ( pItemUnit && pItemUnit->GetInstanceID() == WIID_ITEM )
	{
		bUse = pItemUnit->Use();
	}

	if ( !bUse )
	{
		//std::vector<UINT>::iterator  it;
		//for ( it = m_vHPItemID.begin(); it != m_vHPItemID.end(); it++ )
		//{
		//	SPUIUnit  * pItemUnit = pQuickSlotUnitManager->GetUnit(*it);
		//	if ( pItemUnit && pItemUnit->GetInstanceID() == WIID_ITEM )
		//	{
		//		if ( bUse = pItemUnit->Use() )
		//			break;
		//	}
		//}
	}
	return bUse;
}

//=========================================================================
bool  CAutoInterface::UseSPItem(int iIndex)
{
	if ( iIndex < 0 || iIndex >= m_vSPItemID.size() )
		return false;

	SPWindow * pWindow = g_pInterfaceManager->FindWindow(WIID_QUICK_SLOT);
	if ( !pWindow )
		return false;

	SPUIUnitManager	*	pQuickSlotUnitManager  = NULL;
	pWindow->SPSendMessage( SPIM_GET_QUICK_SLOT_UNIT, (WPARAM)&pQuickSlotUnitManager);
	if ( pQuickSlotUnitManager == NULL )
		return false;

	bool bUse = false;
	SPUIUnit  * pItemUnit = pQuickSlotUnitManager->GetUnit(m_vSPItemID[iIndex]);
	if ( pItemUnit && pItemUnit->GetInstanceID() == WIID_ITEM )
	{
		bUse = pItemUnit->Use();
	}

	if ( !bUse )
	{
		//std::vector<UINT>::iterator  it;
		//for ( it = m_vSPItemID.begin(); it != m_vSPItemID.end(); it++ )
		//{
		//	SPUIUnit  * pItemUnit = pQuickSlotUnitManager->GetUnit(*it);
		//	if ( pItemUnit && pItemUnit->GetInstanceID() == WIID_ITEM )
		//	{
		//		if ( bUse = pItemUnit->Use() )
		//			break;
		//	}
		//}
	}
	return bUse;
}

//=========================================================================
void  CAutoInterface::UseSkill2()
{
	return;
}

//=========================================================================
bool  CAutoInterface::UseReturnItem()
{
	//在城里不使用
	if ( g_pCheckManager->CheckCondition(REQUIRE_TYPE_MAP, 2, GROUP_TYPE_FIELD, 0) )
		return false;

	SPWindow * pWindow = g_pInterfaceManager->FindWindow(WIID_QUICK_SLOT);
	if ( !pWindow )
		return false;

	SPUIUnitManager	*	pQuickSlotUnitManager  = NULL;
	pWindow->SPSendMessage( SPIM_GET_QUICK_SLOT_UNIT, (WPARAM)&pQuickSlotUnitManager);
	if ( pQuickSlotUnitManager == NULL )
		return false;

	std::vector<UINT>::iterator  it;
	for ( it = m_vReturnTtemID.begin(); it != m_vReturnTtemID.end(); it++ )
	{
		SPUIUnit  * pItemUnit = pQuickSlotUnitManager->GetUnit(*it);
		if ( pItemUnit && pItemUnit->GetInstanceID() == WIID_ITEM )
		{
			if ( pItemUnit->Use() == true )
			{
				Stop(2);	//回城后停止挂机
				return true;
			}
		}
	}
	return false;
}


//=========================================================================
bool  CAutoInterface::WndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	//内挂功能屏蔽
#ifndef _DEBUG
	return 0;
#endif

	switch(message)
	{
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		{
			if( wParam == VK_F8 )
			{
				if ( m_bAutoRecord )
					Stop(1);
				else
					Start(1);
				return 1;
			}
			else if( wParam == VK_F10 )
			{
				if ( m_bAutoPlay )
					Stop(2);
				else
					Start(2);
				return 1;
			}
		}
		break;
	}
	return 0;
}


