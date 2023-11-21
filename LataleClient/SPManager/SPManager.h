// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-09-14 ¿ÀÈÄ 12:13:47
//***************************************************************************

#pragma once

class SPCameraManager;

class SPManager {
public:
	SPManager();
	virtual ~SPManager();

	virtual void	Process(float fTime);
	virtual	void	Render(float fTime);
	virtual bool	Init();
	virtual void	Clean();				

	virtual void	RenderTextDebug()	{}
	virtual void	RenderText()		{}
	virtual bool	PurgeAll()			{ return true; }	// Purge Instant Data for Subgame Change

	virtual int		SPPerformMessage	(UINT msg, WPARAM wparam = 0, LPARAM lParam = 0) { return -1;}

	MANAGER_ID	 GetMgrID();	
	void		 SetMgrCamera( SPCameraManager* pMgrCamera);
	bool		 isActive()			{	return m_bActive;  }	

protected:
	void		 SetMgrID(MANAGER_ID ClassID);

protected:

	MANAGER_ID			m_ClassID;
	SPCameraManager*	m_pMgrCamera;

	float				m_fAccumulateProcessTime;
	float				m_fAccumulateRenderTime;

	bool				m_bActive;	

};








