#pragma once

enum MODEL_CLASS_ID;

class SPEventModel;

class SPEventModelFrameAnimation : public SPEventModel {

public:
	SPEventModelFrameAnimation(SPID_EVENT ID);
	SPEventModelFrameAnimation(int iInstance, float fDelay, float fMinStart = 0.0f, float fMaxStart = 0.0f, BOOL BLightMap = FALSE);
	virtual ~SPEventModelFrameAnimation();

	virtual void SetNull();
	virtual void Init();
	virtual void Destroy();
	virtual bool SetImage(SPTexture** ppImage);	
	//virtual bool SetImage(SPTexture** ppImage, int iAlpha);	
	virtual LRESULT GetMemberValue(MODEL_VALUE_TYPE mvMsg, LPARAM lParam = 0);


protected:
	//TerrainModelFrameAnimation
	float	m_fMinStart;
	float	m_fMaxStart;
	std::vector<SPTexture*>	m_vpFrameImage;
	std::vector<int>		m_viFrameAlpha;
	std::vector<std::string> m_vstrFrameFile;		//jinhee 04-11-11
	int		m_iNoFrame;

	//FxModelFrameAnimation
	SPTexture*					m_pFrameImage;
	std::vector<SPEventFrameData*> m_vpFrameData;
	//int		m_iNoFrame;
	BOOL	m_BContinue;

};

