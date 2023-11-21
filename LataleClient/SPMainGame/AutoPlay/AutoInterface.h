#pragma once

#include "SPCommandConvert.h"
#include "SPGOBManager.h"
#include "SPGOBCoordPhysics.h"
#include "SPGameObject.h"
#include "SPGOBClusterDef.h"
#include "SPAvatarModelUnitDef.h"
#include "SPPlayerInvenArchive.h"
#include "SPPlayer.h"
#include "SPPlayerActionManager.h"

#include "SPSoundManager.h"
#include "SPSkill.h"
#include "SPSkillArchive.h"
#include "SPMotionStatus.h"
#include "SPSkillManager.h"


enum  AUTO_STATUS
{
	AUTO_MOVE		= 0,	//范围巡走
	AUTO_MOVETO,			//移动到目标
	AUTO_MOVE_RECORD,		//按玩家自制移动
};


struct   ACTION_LIST
{
	float		fPosX;
	float		fPosY;
	FACING		eFacing;
	UINT64      uiAction;

	ACTION_LIST()
	{
		fPosX		= 0.0f;
		fPosY		= 0.0f;
		eFacing		= FACING_LEFT;
		uiAction	= ACTION_STOP;
	}
};


class CAutoInterface
{
public:
	CAutoInterface(void);
	~CAutoInterface(void);

	static CAutoInterface &  GetInstance()
	{
		static  CAutoInterface  _inst;
		return  _inst;
	}
	
	bool		Initial();
	void		Destroy();
	void		Process( float fTime );
	void		Render( float fTime );

	void		Start(int iOper);	//1,录制 2,挂机
	void		Stop(int iOper);
	bool		isStart() { return m_bAutoPlay; }
	bool		isRecord() { return m_bAutoRecord; }

	void        InitQuickSlot();

	void		RecordAction(UINT64 uiAction);

	bool		CheckAttackRgn(int iSkillID);
	bool		FindMonster();
	bool		FindActionIndex();
	void		Move(float fTime);

	//=====================================================
	float       GetPlayerPosX();
	float       GetPlayerPosY();
	int			GetDirection();
	float       GetMapWidth();
	float       GetMapHeight();
	UINT		GetPlayerStatus(STATUS_TYPE type);

	bool		GetMoveEnable();
	bool		CheckBlock(int iDir = 0);  //0:根根左右方向计算 1:上, 2:下

	void		SetAction( UINT64 uiAction );
	UINT64		GetAction();
	void		SetSkillAction( int iSkillID );

	bool		UseSkillAttack();
	void		UseSkill2();
	bool		UseHPItem(int iIndex = 0);
	bool		UseSPItem(int iIndex = 0);
	bool		UseReturnItem();


	//消息
	bool		WndProc(UINT message, WPARAM wParam, LPARAM lParam);

private:
	bool						m_bAutoPlay;
	bool						m_bAutoRecord;
	AUTO_STATUS					m_eAutoStatus;
	AUTO_STATUS					m_eLastStatus;			//最后的动作状态

	SPPlayer *					m_pLocalPlayer;
	SPPlayerActionManager*		m_pPlayerActionManager;
	SPSkillUsed	*				m_pkSkillUsed;

	//
	int							m_iCurSkillIndex;
	std::vector<UINT>			m_vSkillID;				//玩家使用的技能
	std::vector<UINT>			m_vSkillID2;			//玩家使用的辅助技能
	std::vector<UINT>			m_vHPItemID;			//玩家使用的红
	std::vector<UINT>			m_vSPItemID;			//玩家使用的兰
	std::vector<UINT>			m_vReturnTtemID;		//玩家使用的回城物品

	//
	int							m_iMonsterDst;			//寻怪距离
	POINT						m_iPlayerDstPos;		//玩家移动目标
	FACING						m_eFacing;				//目标方向
	bool						m_bLastAttack;			//最后动作是否为攻击
	GOB_STATE_TYPE				m_eLastGobStateType;

	int							m_iCurActionIndex;		//当前动作序号
	vector<ACTION_LIST>			m_vActionList;			//玩家动作列表
	
};
