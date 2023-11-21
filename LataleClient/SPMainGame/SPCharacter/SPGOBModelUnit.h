// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPGOBModelUnit
// Comment     : 
// Creation    : metalgeni 2004-10-29 ¿ÀÀü 10:26:19
//***************************************************************************

#pragma once

enum MODEL_CLASS_ID;
class SPGameObject;

class SPGOBModelUnit 
{
public:
	SPGOBModelUnit(SPGameObject* pParent, MODEL_CLASS_ID ClassID);
	virtual ~SPGOBModelUnit();

	virtual void Init()			{}
	virtual void Render(float fTime)	{};
	virtual void Process(float fTime)	{};
	virtual LRESULT			SPSendMessage(MODEL_VALUE_TYPE mvMsg, WPARAM wParam = 0, LPARAM lParam = 0);

	virtual SPGameObject*	GetParent()	{ return	m_pParent;	}
	MODEL_CLASS_ID	GetClassID()		{ return	m_ClassID;	}	

	virtual float GetRenderPosX() = 0;
	virtual float GetRenderPosY() = 0;

protected:

	MODEL_CLASS_ID	m_ClassID;
	SPGameObject*	m_pParent;
	
};


