
// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-08-24 ¿ÀÀü 10:26:19
//***************************************************************************

#pragma once 

extern BG_ID; 

enum ANIMATION_INDEX;
enum CLASS_ID;
enum FACING;
enum SP_FXM_RENDER_LAYER;

class SPGameObject {
public:

	SPGameObject();
	SPGameObject(INSTANCE_ID instanceID, CLASS_ID ClassID);
	virtual ~SPGameObject();

	virtual void	Process(float fTime);
	virtual	void	Render(float fTime);
	virtual void	Init();
	virtual bool	IsLocalPlayer();
	virtual bool	IsShowName();
	virtual bool	IsShowHP();
	virtual	void	RenderFX( float fTime , SP_FXM_RENDER_LAYER eRenderLayer );

	void			SetClassID(CLASS_ID ClassID);
	CLASS_ID		GetClassID();
	void			SetInstanceID( INSTANCE_ID instanceID );
	INSTANCE_ID		GetInstanceID();
	GU_ID			GetGUID();		

	bool IsClipping();

	virtual int  PerformMouseMove		( int iX, int iY);		//[2005/5/6] - jinhee
	virtual int  PerformMouseLDown		( int iX, int iY);
	virtual int  PerformMouseLUp		( int iX, int iY);
	virtual int  PerformMouseRDown		( int iX, int iY);
	virtual int  PerformMouseRUp		( int iX, int iY);
	virtual bool IsCursorIn				( int iX, int iY);

	virtual int	 SPGOBPerformMessage(SPGM msg, LPARAM lParam = 0);
	

protected:

	SPGMESSAGE_OBJ_MAP_DECLARE()
	
protected:

	CLASS_ID		m_ClassID;
	INSTANCE_ID		m_InstanceID;	

	float			m_fAccumulateProcessTime;
	float			m_fAccumulateRenderTime;

	bool			m_bEnable;
	bool			m_bClipping;
};











