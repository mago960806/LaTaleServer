// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPCameraManager
// Comment     : 
// Creation    : metalgeni 2004-08-23 ¿ÀÈÄ 2:56:36
//***************************************************************************


#pragma once 


struct SPCameraStruct {
	// Camera Position
	float fOffsetX;
	float fOffsetY;	

	// Target to move
	float fAccelX;
	float fAccelY;
	float fTargetOffsetX;
	float fTargetOffsetY;

	SPCameraStruct()
		:fOffsetX		(0.f)
		,fOffsetY		(0.f)			
		,fAccelX		(1.0f)
		,fAccelY		(1.0f)
		,fTargetOffsetX	(0.f)
		,fTargetOffsetY	(0.f) 
	{}

	void Init()
	{
		fOffsetX = 0.0f;
		fOffsetY = 0.0f;
		fAccelX = 1.0f;
		fAccelY = 1.0f;
		fTargetOffsetX = 0.0f;
		fTargetOffsetY = 0.0f;
	}
};


class SPCameraManager : public SPManager {

public:

	SPCameraManager();
	virtual ~SPCameraManager();
	virtual void Process(float fTime);
	virtual bool Init();
	virtual void Clean();

	SPCameraStruct* GetCameraStruct();


	void ProcessFreeCamera();
	void ProcessChar();

	void Adjust();
	enum FOCUS {
		FOCUS_NULL			= 0x0000,
		FOCUS_FREE_CAMERA,
		FOCUS_CHAR,		
	};

	void SetCameraMode(const FOCUS focus);
	FOCUS GetCameraMode() { return m_Focus; }


protected:

	FOCUS				m_Focus;
	SPCameraStruct		m_CameraStruct;

};


extern SPCameraManager* g_pCameraManagr;