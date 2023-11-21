// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPSoundUnit
// Comment     : 
// Creation    : metalgeni 2005-01-26 오전 10:26:19
//***************************************************************************

class SPSoundArchive;
class SPSoundBase;

enum SU_PLAY_STATE {
	SPS_PLAY_READY,
	SPS_PLAY,
	SPS_RELEASE_READY,
};

class SPSoundUnit {
public:
	int Process(float fTime);
	void UpdatePosition(POINT Pos);

protected:
	friend SPSoundBase;
	friend SPSoundArchive;	

	// SPSoundArchive에서만 생성삭제 한다.
	SPSoundUnit ( SPSoundArchive* pParent, float fDelay ); 
	virtual ~SPSoundUnit();	
	void Release();
	void SetLoopState(bool bLoop);
	bool GetLoopState();

protected:
	H3DSAMPLE	m_hSample;
	void*		m_pSampleAddress;

	float		m_fDelayPlay;
	float		m_fAccumulateTime;

	int			m_iPlayerState;

	SPSoundArchive* m_pParent;
	std::string		m_strFileName;
	bool		m_bLoop;

};


