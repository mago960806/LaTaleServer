// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-08-24 ¿ÀÀü 10:26:19
//***************************************************************************

#pragma once

enum MODEL_CLASS_ID;

class SPTerrainModel;

class SPTerrainModelStatic : public SPTerrainModel {
public:
	SPTerrainModelStatic();
	//SPTerrainModelStatic(int iInstanceID, SPTexture** ppImage, int iLightMap = 0);
	SPTerrainModelStatic(INT64 iInstanceID, SPTexture** ppImage, int iLightMap = 0);
	virtual ~SPTerrainModelStatic();

	virtual LRESULT GetMemberValue(MODEL_VALUE_TYPE mvMsg, LPARAM lParam);
	virtual bool SetImage(SPTexture** ppImage, int iAlpha) { return false;	}

	virtual void SetNull();
	virtual void Init();
	virtual void Destroy();

protected:	

	SPTexture*		m_pStaticImage;


};

