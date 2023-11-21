#ifndef __SPSOUND_MANAGER_H__
#define __SPSOUND_MANAGER_H__

struct SOUND_TABLE_INFO
{
	float fDelayTime[3];
	bool bLoop[3];
	std::string strFileName[3];

	SOUND_TABLE_INFO()
	{
		fDelayTime[0] = fDelayTime[1] = fDelayTime[2] = 0.0f;
		bLoop[0] = bLoop[1] = bLoop[2] = false;
	}

	SOUND_TABLE_INFO& operator = (const SOUND_TABLE_INFO& Src)
	{
		for( int i = 0; i < 3; i++ )
		{
			fDelayTime[i] = Src.fDelayTime[i];
			bLoop[i] = Src.bLoop[i];
			strFileName[i] = Src.strFileName[i];
		}
	}
};

class SPSoundManager
{
public:
	static SPSoundManager* GetInstance();
	static void Release();

	void Init();

	SOUND_TABLE_INFO* GetGOBSoundTable(unsigned int uiAniIndex);
	SOUND_TABLE_INFO* GetMOBSoundTable(unsigned int uiClassID, ANIMATION_INDEX eIndex);
	SOUND_TABLE_INFO* GetSoundTable(unsigned int uiKeyValue);

protected:
	SPSoundManager();
	virtual ~SPSoundManager();

	bool LoadSoundList();
	bool LoadGOBMotionSoundList();
	bool LoadMOBMotionSoundList();

protected:
	static SPSoundManager* m_pkInstance;
	
	stdext::hash_map< unsigned int, SOUND_TABLE_INFO > m_hmSoundList;
	stdext::hash_map< unsigned int, unsigned int > m_hmGOBMotionSoundList;
	stdext::hash_map< unsigned int,
		stdext::hash_map< ANIMATION_INDEX, unsigned int > > m_hmMOBMotionSoundList;
};

#endif