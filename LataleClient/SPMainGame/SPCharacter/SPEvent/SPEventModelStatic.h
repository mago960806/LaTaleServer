#pragma once

enum MODEL_CLASS_ID;

class SPEventModel;

class SPEventModelStatic : public SPEventModel {
public:
	SPEventModelStatic(SPID_EVENT ID);
	SPEventModelStatic(SPID_EVENT ID, int iInstanceID, SPTexture** ppImage, int iLightMap = 0);
	virtual ~SPEventModelStatic();

	virtual LRESULT GetMemberValue(MODEL_VALUE_TYPE mvMsg, LPARAM lParam);
	virtual bool SetImage(SPTexture** ppImage, int iAlpha) { return false;	}

	virtual void SetNull();
	virtual void Init();
	virtual void Destroy();

protected:	

	SPTexture*		m_pStaticImage;


};

