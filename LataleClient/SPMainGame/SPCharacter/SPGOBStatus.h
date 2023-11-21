// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPGOBStatus
// Comment     : 
// Creation    : metalgeni 2004-08-24 오전 10:26:19
//***************************************************************************

#pragma once


class SPGameObject;
class SPGOBStatus {
public:
	SPGOBStatus(SPGameObject* pParent);
	virtual ~SPGOBStatus();
	virtual void	Init();	

	virtual const char*		GetGOBName();
	virtual unsigned int	GetCurHP();
	virtual unsigned int	GetMaxHP();	

	virtual CHAR_EXP*		GetCHAR_EXP()	{ return NULL;	}
	const UCHAR				GetLevel()		{ return m_cLevel; }
	CHAR_STS*				GetCHAR_STS()	{ return &m_CharSts; }

	virtual UINT			GetStatusValue(STATUS_TYPE StatusType);
	virtual bool			SetStatusValue(STATUS_TYPE StatusType, UINT uiValue);

	virtual UINT64			GetStatusValueEX(STATUS_EX_TYPE	 StatusType);
	virtual bool			SetStatusValueEX(STATUS_EX_TYPE	 StatusType, UINT64 uiValue);


	// 임시
	virtual void SetGender(GENDER Gender) {}
	

	int SPPerformMessage(SPGM msg, LPARAM lparam);

protected:	

	SPGMESSAGE_OBJ_MAP_DECLARE()

	virtual int OnStatChange	( LPARAM lParam );
	virtual int OnStatChangeEX	( LPARAM lParam );
	

protected:

	SPGameObject*			m_pParent;

	std::string				m_strName;		
	UCHAR					m_cLevel;

	UINT32					m_uiHitPoint;
	UINT32					m_uiSpiritPoint;

	UINT32					m_uiMaxHitPoint;
	UINT32					m_uiMaxSpiritPoint;	

	UINT32					m_uiMaxHPBase;
	UINT32					m_uiMaxSPBase;

	int						m_iRecoveryHP;
	int						m_iRecoverySP;

	CHAR_STS				m_CharSts;

};














