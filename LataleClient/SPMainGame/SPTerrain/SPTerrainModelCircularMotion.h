// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-08-24 ¿ÀÀü 10:26:19
//***************************************************************************

#pragma once

enum MODEL_CLASS_ID;

class SPTerrainModel;
class SPTexture;

class SPTerrainModelCircularMotion : public SPTerrainModel {
public:
	SPTerrainModelCircularMotion();
	//SPTerrainModelCircularMotion(int iInstance, float fDelay, float fRadian, SPTexture** ppImage, BOOL BLightMap = FALSE);
	SPTerrainModelCircularMotion(INT64 iInstance, float fDelay, float fRadian, SPTexture** ppImage, BOOL BLightMap = FALSE);
	virtual ~SPTerrainModelCircularMotion();

	virtual bool SetImage(SPTexture** ppImage, int iAlpha) { return false;	}

	virtual void SetNull();
	virtual void Init();
	virtual void Destroy();
	virtual LRESULT GetMemberValue(MODEL_VALUE_TYPE mvMsg, LPARAM lParam = 0); 
	

protected:

	SPTexture*		m_pStaticImage;	
	float		m_fRotateDegree;

};



