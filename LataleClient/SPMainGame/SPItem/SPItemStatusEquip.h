// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPItemStatusEquip
// Comment     : 
// Creation    : metalgeni 2004-11-05 ¿ÀÀü 10:26:19
//***************************************************************************

#pragma once


class SPItemStatusEquip : public SPItemStatus {
public:
	SPItemStatusEquip(EQUIP_ID	EquipID = EQ_NULL);
	virtual ~SPItemStatusEquip();
	virtual LRESULT SPSendMessage(MODEL_VALUE_TYPE mvMsg, LPARAM lParam = 0);		

protected:
	virtual bool Copy(SPItemStatusEquip* pSrc);
	virtual void SetNull();

protected:
	EQUIP_CLASS	m_EquipClass;
	EQUIP_ID	m_EquipID;	
	int			m_iEquipItemID;
	int			m_iEquipItemColor;
};









