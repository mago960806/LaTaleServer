// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-08-24 ¿ÀÀü 10:26:19
//***************************************************************************

#pragma once

enum MODEL_CLASS_ID;

class SPTerrainModel;

class SPTerrainModelFrameAnimation : public SPTerrainModel {

public:
	SPTerrainModelFrameAnimation();
	//SPTerrainModelFrameAnimation(int iInstance, float fDelay, float fMinStart = 0.0f, float fMaxStart = 0.0f, BOOL BLightMap = FALSE, int iSync = 0);
	SPTerrainModelFrameAnimation(INT64 iInstance, float fDelay, float fMinStart = 0.0f, float fMaxStart = 0.0f, BOOL BLightMap = FALSE, int iSync = 0);
	virtual ~SPTerrainModelFrameAnimation();

	virtual void SetNull();
	virtual void Init();
	virtual void Destroy();
	virtual bool SetImage(SPTexture** ppImage, int iAlpha);	
	virtual LRESULT GetMemberValue(MODEL_VALUE_TYPE mvMsg, LPARAM lParam = 0);


protected:

	float	m_fMinStart;
	float	m_fMaxStart;
	std::vector<SPTexture*>	m_vpFrameImage;
	std::vector<int>		m_viFrameAlpha;
	std::vector<std::string> m_vstrFrameFile;		//jinhee 04-11-11
	int		m_iNoFrame;

	int		m_iSyncTile;							//[2005/11/4]
	
};

