// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPMonsterStatus
// Comment     : 
// Creation    : metalgeni 2004-08-24 ¿ÀÀü 10:26:19
//***************************************************************************

#pragma once

class SPGameObject;
class SPGOBStatus;

class SPMonsterStatus : public SPGOBStatus {
public:
	SPMonsterStatus(SPGameObject* pParent);
	virtual ~SPMonsterStatus();
	virtual void Init();		
	virtual bool SetMonsterInfo(MONSTER_INFO* pInfo);

protected:
	SPGMESSAGE_OBJ_MAP_DECLARE()
	virtual int OnSetMonsterInfo( LPARAM lParam );

protected:



};














