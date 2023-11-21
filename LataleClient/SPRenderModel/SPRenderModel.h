// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPRenderModel
// Comment     : 
// Creation    : metalgeni 2004-08-24 ¿ÀÀü 10:26:19
//***************************************************************************

#pragma once

enum MODEL_VALUE_TYPE;
enum MODEL_CLASS_ID;

class SPRenderModel {

public:

	SPRenderModel();
	virtual ~SPRenderModel();

	virtual void Init();
	virtual void SetNull();
	virtual void Destroy();
	virtual bool SetImage(SPTexture** ppImage, int iAlpha) { return false; }

	virtual LRESULT GetMemberValue(MODEL_VALUE_TYPE mvMsg, LPARAM lParam = 0); 

	MODEL_CLASS_ID	GetClassID()		{ return m_ClassID;}
	//int				GetInstanceID()		{ return m_iInstanceID; }
	INT64				GetInstanceID()		{ return m_iInstanceID; }

protected:	

	BOOL					m_BLightMap;
	MODEL_CLASS_ID			m_ClassID;
	//int						m_iInstanceID;	
	INT64					m_iInstanceID;
	float					m_fNextAnimation;
	
};






