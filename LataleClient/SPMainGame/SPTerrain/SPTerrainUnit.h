// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-08-24 오전 10:26:19
//***************************************************************************

#pragma once

enum MODEL_CLASS_ID;

class SPTerrainUnit {
public:
	SPTerrainUnit(SPTerrainModel** ppTerrainModel, float fX, float fY, int iArg0 = 0, int iArg1= 0, int iArg2=0, int iArg3=0, float fScaleX = 1.0f, float fScaleY = 1.0f );	
	~SPTerrainUnit();

	virtual void SetNull();
	virtual void Init();
	
	virtual void Process(float fTime);
	//이건 좀더 생각해봐야... 여기서 카메라를 계산해서 줘야 한다...
	//virtual void Process(float fTime, float fCameraX = -1.0f, float fCameraY = -1.0f); 
	virtual void Render(float fTime, float fAlpha = 1.0f);		//파라미터 alpah 추가 jinhee - 04.11.02 

	void	GetAbsCoord(int& ix, int& iy);

	//int		GetModelID();					//jinhee - 10.10.16
	INT64		GetModelID();
	float	GetPosX();						//jinhee - 10.10.16
	float	GetPosY();						//jinhee - 10.10.16
	float	GetWidth();						//jinhee - 10.10.16
	float	GetHeight();					//jinhee - 10.10.16	
	void	SetPosX(float fPosX = 0.0f);	//jinhee - [12/7/2004]
	void	SetPosY(float fPosY = 0.0f);	//jinhee - [12/7/2004]
	
	void	SetScale(float fScaleX = 1.0f, float fScaleY = 1.0f);//jinhee - [12/7/2004]
	float	GetScaleX();					//jinhee - [12/7/2004]
	float	GetScaleY();					//jinhee - [12/7/2004]

	SPTerrainModel* GetTerrainModel();		//[2005/11/2]
	MODEL_CLASS_ID	GetClassID();			//[2005/11/8]

protected:
	float				m_fPosX;
	float				m_fPosY;

	float				m_fCamX;
	float				m_fCamY;

	float				m_fAccumulateRender;

	// CircularMotion
	float				m_fCurRotateDegree;	

	// FrameAnimation
	float				m_fMinStart;
	float				m_fMaxStart;
	int					m_iNoFrame;
	int					m_iCurFrame;
	int					m_iCurAlpha;

	// Common
	float				m_fModelDelay;
	float				m_fCurDelay;

	//int					m_iModelID;				// SPTerrainModel instance id	
	INT64				m_iModelID;
	
	MODEL_CLASS_ID	m_ClassID;
	
	SPTerrainModel*		m_pTerrainModel;
	SPTexture**			m_ppCurImage;

	int					m_iArg0;
	int					m_iArg1;
	int					m_iArg2;
	int					m_iArg3;
	float				m_fScaleX;				// jinhee - [12/7/2004] //X 축 스케일
	float				m_fScaleY;				// jinhee - [12/7/2004] //Y 축 스케일
	
	bool				m_bRenderCancel;		// jinhee - [12/2/2004] //true시 랜더하지 않음

};











