

#pragma once

enum MODEL_VALUE_TYPE;

class SPEventModel : public SPRenderModel {
public:
	SPEventModel(SPID_EVENT ID);
	virtual ~SPEventModel();

	virtual void Init();
	virtual void SetNull();
	virtual void Destroy();
	virtual bool SetImage(SPTexture** ppImage) = 0;

	virtual LRESULT GetMemberValue(MODEL_VALUE_TYPE mvMsg, LPARAM lParam = 0); 
	
	float	GetFrameDelay() { return m_fNextAnimation; }
	SPID_EVENT GetClassID();

protected:	
	SPID_EVENT			m_ID;
	float				m_fLifeTime;
	//int					m_iParentFollow;
	
};
