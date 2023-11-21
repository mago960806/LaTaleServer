// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-7-13    18:40 
//***************************************************************************

#pragma once

class SPGOBCoordPhysics;

class SPPlayerActionManager
{
private:
	SPPlayerActionManager			();

	static	SPPlayerActionManager*	m_pPlayerActionManagerInstance;

public:
	virtual ~SPPlayerActionManager	();

	static	SPPlayerActionManager*	GetInstance(void);
	static	void					DelInstance(void);

	//	초기화
	void							Init				();

	//	Action 인터페이스
	void							SetAction			( UINT64 uiAction );
	void							AddAction			( UINT64 uiAction );
	void							DelAction			( UINT64 uiAction );
	BOOL							GetAction			( UINT64 uiAction );
	UINT64							GetAction			();

	//	Skill 인터페이스
	void							SetSkillID			( int iSkillID );
	void							SetItemSkillID		( int iSkillID, CONTAINER_SLOT Container );
	int								GetSkillID			( CONTAINER_SLOT& Container);

	//	ProcessSkip 인터페이스
	void							SetProcessSkip		( BOOL bSkip );
	BOOL							GetProcessSkip		();

	//	Process
	void							Process				( float fElapsedTime );


protected:

	struct RUN_CONTROL
	{
		float	m_fKeyTermTime;
		float	m_fKeyTermAccmulateTime;
		float	m_fKeyPushAccmulateTime;
		int		m_iDirection;
	};

	void							ProcessDirection	( float fElapsedTime );
	void							ProcessEtc			();
	void							ProcessEnd			( float fElapsedTime );
	
	void							ProcessJoyPad			(float fElapsedTime);

	BOOL							CheckRun			( float fElapsedTime , int iDirection );


	//////////////////////////////////////////////////////////////////////////
	//
	//	변수들
	//

	UINT64							m_uiAction;
	UINT64							m_uiReserveRunAction;
	int								m_iRequestSkillID;
	BOOL							m_bProcessSkip;

	RUN_CONTROL						m_stRunControl;
	CONTAINER_SLOT					m_ItemSkillContainer;


};
