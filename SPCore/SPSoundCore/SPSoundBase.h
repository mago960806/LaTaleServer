// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPSoundBase
// Comment     : 
// Creation    : metalgeni 2005-01-26 오전 10:26:19
//***************************************************************************

#pragma once

#include "mss.h" // Miles Sound System
#include <map>

using namespace std;

class SPGameObject;
class SPStream;
class SPSoundArchive;


#define DIGITALPOOLCOUNT  32    // How many samples to play at once

#define PITCHMIN			0
#define PITCHMID			64
#define PITCHMAX			127

#define DIGITALRATE			22050
#define DIGITALBITS			16
#define DIGITALCHANNELS		2

#define MAX_PROVIDERS		32
#define FADETIME			1500.0f


#define SE_DEFAULT			0.9f
#define BGM_DEFAULT			0.5f


enum ECHO_TYPE {
	ECHO_GENERIC,                // default
	ECHO_PADDEDCELL,
	ECHO_ROOM,                   // standard environments
	ECHO_BATHROOM,
	ECHO_LIVINGROOM,
	ECHO_STONEROOM,
	ECHO_AUDITORIUM,
	ECHO_CONCERTHALL,
	ECHO_CAVE,
	ECHO_ARENA,
	ECHO_HANGAR,
	ECHO_CARPETEDHALLWAY,
	ECHO_HALLWAY,
	ECHO_STONECORRIDOR,
	ECHO_ALLEY,
	ECHO_FOREST,
	ECHO_CITY,
	ECHO_MOUNTAINS,
	ECHO_QUARRY,
	ECHO_PLAIN,
	ECHO_PARKINGLOT,
	ECHO_SEWERPIPE,
	ECHO_UNDERWATER,
	ECHO_DRUGGED,
	ECHO_DIZZY,
	ECHO_PSYCHOTIC,

	ECHO_COUNT					// total number of environments
};

enum SoundEnable {
	ENABLE_BGM		= 1,	// BGM 가능
	ENABLE_EFFECT	= 1<<1,	// EFFECT 가능
	ENABLE_SYSTEM	= 1<<2,	// 시스템적으로 가능할때, 예를들어 윈도우포커스가 없으면 모두 Disable
	ENABLE_OPTION	= 1<<3, // 옵션으로 가능할때
};

struct ProviderStuff
{
	char* pszName;
	HPROVIDER ProviderID;
};

extern bool g_bNoSound;
class SPSoundUnit;

class SPSoundBase {
public:
	static SPSoundBase* GetInstance();
	static SPSoundBase* Create();
	bool Init(HWND hWnd, const char* pszRedist);
	void Release();
	void Process(float fTime);

	virtual ~SPSoundBase();	

	float UpdateBGMVolume		( float fNewPos );
	float UpdateEffectVolume	( float fNewPos );

	void EnableEffect	( bool bEnable );
	void EnableBGM		( bool bEnable );
	void EnableSystem	( bool bEnable );

	bool IsEnableEffect();
	bool IsEnableBGM();

	bool SetMainBGM			(const char* pszFilename);	
	bool AllocateSample		(const char* pszFileName,		SPSoundUnit* pSoundStuff);
	

	bool AllocateArchive	(unsigned __int64& ui64Guid,	SPSoundArchive** ppSoundArchive);
	void FreeArchive		(unsigned __int64& ui64Guid);

	bool IsPlayDone(SPSoundUnit* pSoundStuff);

	void	SetRoomType(int iRoomType);
	int		GetSoundOption();
	float	GetBGMVolume();
	float	GetEffectVolume();

	int		SetSoundFocus(HWND hWnd);


	// Miles Core Callback
	unsigned int 	OpenSound		( const char* pszFilename, unsigned int* piHandleSoundPool	);
	bool			CloseSound		( unsigned int iHandleSoundPool	);
	unsigned int 	SeekSound		( unsigned int iHandleSoundPool, int iOffset, unsigned int uiOrigin  );
	unsigned int 	ReadSound		( unsigned int iHandleSoundPool, void *buffer, unsigned int uiSize	);

protected:

	enum FADETYPE {
		FADE_NULL		= 0,
		FADE_IN			= 1,
		FADE_OUT		= 2,
	};

	void			SetEchoType		(SPSoundUnit* pSoundStuff, int iVal);
	bool			SetRampBGM		(FADETYPE FadeType );

	void			ReleaseProviders	();
	void			EnumProviders		();
	bool			OpenProviders		();

	void*			LoadSampleFile		(const char* pszFilename);
	bool			AllocBGMStream		();
	

protected:
	SPSoundBase();
	static SPSoundBase* s_pMilesInstance;

protected:	
	
	// Resource Pack
	std::map<unsigned int,	SPStream*>			m_mpStream;	
	std::map<unsigned __int64, SPSoundArchive*>	m_mpArchive;

	unsigned int				m_uiLastResourceInstance;

	// MSS 
	ProviderStuff				m_Provider[MAX_PROVIDERS];
	int							m_iCurProvider;
	int							m_iNoProvider;
	int							m_iUsingEAX;		

	float						m_fEffectVolume;
	float						m_fBGMVolume;
	float						m_fRampTime;
	float						m_fFadeAccumulate;

	std::string					m_strBGMFileName;	

	int							m_iRTOption;

	HPROVIDER					m_OpendProvider;								
	HDIGDRIVER					m_DIG;
	HSTREAM						m_MainBGM;	
	FADETYPE					m_FadeType;

	int							m_iRoomType;

	H3DPOBJECT					m_listener;


};






