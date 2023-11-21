
#include <algorithm>
#include <map>

#include "SPCoreDef.h"

#include "SPStream.h"
#include "SPResourceBase.h"
#include "SPSoundUnit.h"
#include "SPSoundArchive.h"
#include "SPSoundBase.h"

bool g_bNoSound = false;

//////////////////////////////////////////////////////////////////////////
// Define Customized File Service Callback Lists
S32 AILCALLBACK FileSeekCallBack( U32 file_handle, S32 offset, U32 type  )
{
	return SPSoundBase::GetInstance()->SeekSound	( file_handle, offset, type );
}

void AILCALLBACK FileCloseCallback( U32 file_handle )
{
	SPSoundBase::GetInstance()->CloseSound	( file_handle );
}

U32 AILCALLBACK FileOpenCallback( char const * pszFilename, U32 *piFileHandle )
{
	return SPSoundBase::GetInstance()->OpenSound	( pszFilename, piFileHandle );
}

U32 AILCALLBACK FileReadCallback( U32 file_handle, void *buffer, U32 bytes )
{
	return SPSoundBase::GetInstance()->ReadSound	( file_handle, buffer, bytes );
}
//////////////////////////////////////////////////////////////////////////

SPSoundBase* SPSoundBase::s_pMilesInstance = 0;

SPSoundBase::SPSoundBase()
: m_uiLastResourceInstance	(0)
, m_MainBGM			(0)
, m_fEffectVolume	(SE_DEFAULT	)
, m_fBGMVolume		(BGM_DEFAULT)
, m_iNoProvider		(0)
, m_OpendProvider	(0)
, m_iUsingEAX		(0)
, m_iCurProvider	(0)
, m_iRoomType		(ECHO_GENERIC)
, m_iRTOption		(0)
, m_fRampTime		(0.0f)
, m_fFadeAccumulate (0)
, m_FadeType		(FADE_NULL)
, m_DIG				(0)
, m_listener		(0)
{	
}

SPSoundBase::~SPSoundBase()
{	
	if(m_listener) {
		 AIL_close_3D_listener(m_listener);
	}

	std::map<unsigned __int64, SPSoundArchive*>::iterator Iter = m_mpArchive.begin();
	for(;Iter != m_mpArchive.end() ; ++Iter) {
		SAFE_RELEASE((*Iter).second);
	}	

	if(m_MainBGM) {		
		AIL_close_stream( m_MainBGM );
		m_MainBGM = 0;
	}
	ReleaseProviders();
	// Close the digital driver
	if (m_DIG) {
		AIL_close_digital_driver( m_DIG );
		m_DIG = 0;
	}
	// Now shutdown Miles completely
	AIL_shutdown();	
}

SPSoundBase* SPSoundBase::GetInstance()
{
	return s_pMilesInstance;
}

SPSoundBase* SPSoundBase::Create()
{
	if( s_pMilesInstance == 0 ) {
		s_pMilesInstance = new SPSoundBase;		
	}
	return s_pMilesInstance;
}

void SPSoundBase::Release()
{
	SAFE_DELETE(s_pMilesInstance);
}


bool SPSoundBase::Init(HWND hWnd, const char* pszRedist)
{
	// Initialize the Miles Sound System	
	AIL_set_redist_directory( pszRedist );
	AIL_startup();

	//if (!AIL_quick_startup(1,0,44100,16,2)) {
	//	return false;
	//}

	AIL_quick_handles(&m_DIG,0,0);

	// Initialize digital driver
	m_DIG = AIL_open_digital_driver( DIGITALRATE, DIGITALBITS, DIGITALCHANNELS, 0 );
	//if(hWnd)
	//	AIL_set_DirectSound_HWND(m_DIG, hWnd);
	
	// Adjust Customized File System CallBack Function (SPResourceBase)	
	AIL_set_file_callbacks(	FileOpenCallback, FileCloseCallback, FileSeekCallBack, FileReadCallback	);
	
	EnumProviders();
	OpenProviders();

	m_listener = AIL_open_3D_listener(m_OpendProvider);

	if(m_listener) {	
		AIL_set_3D_position(m_listener,
			0.0F,
			0.0F,
			0.0F);

		AIL_set_3D_orientation(m_listener,
			0.0F, 0.0F, 1.0F,
			0.0F, 1.0F, 0.0F);
	}


	m_iRTOption = ENABLE_BGM | ENABLE_EFFECT | ENABLE_SYSTEM | ENABLE_OPTION;
	if(g_bNoSound == true) {
		m_iRTOption &= ~ENABLE_OPTION;
	}
	return true;

}

int SPSoundBase::SetSoundFocus(HWND hWnd)
{
	if(hWnd)
		return AIL_set_DirectSound_HWND(m_DIG, hWnd);

	return 0;
}


unsigned int SPSoundBase::OpenSound(const char* pszFilename, unsigned int* piHandleSoundPool)
{
	SPStream* pStream;
	if(	SPResourceBase::GetInstance()->GetStreamData(pszFilename, &pStream) == true) {
		++m_uiLastResourceInstance;

		m_mpStream.insert( std::map< unsigned int, SPStream* >::value_type( m_uiLastResourceInstance, pStream)  );

		*piHandleSoundPool = m_uiLastResourceInstance;
		return m_uiLastResourceInstance;
	}
	return 0;	
}

bool SPSoundBase::CloseSound(unsigned int iHandleSoundPool)
{
	std::map< unsigned int, SPStream* >::iterator Iter = m_mpStream.find(iHandleSoundPool);
	if(Iter != m_mpStream.end() && 	(iHandleSoundPool == (*Iter).first) ) {
		SAFE_RELEASE((*Iter).second);
		m_mpStream.erase(Iter);
		return true;
	}
	return false;
}

unsigned int SPSoundBase::SeekSound( unsigned int iHandleSoundPool, int iOffset, unsigned int uiOrigin  )
{
	SPStream* pStream;
	std::map< unsigned int, SPStream* >::iterator Iter = m_mpStream.find(iHandleSoundPool);
	if(Iter != m_mpStream.end() && 	(iHandleSoundPool == (*Iter).first) ) {
		pStream = (*Iter).second;
		if(pStream)
			return pStream->Seek( iOffset, uiOrigin);
	}
	return 0;
}

unsigned int SPSoundBase::ReadSound( unsigned int iHandleSoundPool, void *buffer, unsigned int uiSize )
{
	SPStream* pStream;
	std::map< unsigned int, SPStream* >::iterator Iter = m_mpStream.find(iHandleSoundPool);
	if(Iter != m_mpStream.end() && 	(iHandleSoundPool == (*Iter).first) ) {
		pStream = (*Iter).second;
		if(pStream)
			return pStream->Read(buffer, uiSize);
	}
	return 0;
}


void SPSoundBase::EnumProviders()
{		
	HPROENUM next = HPROENUM_FIRST;
	m_iNoProvider = 0;
	while (AIL_enumerate_3D_providers(&next, &m_Provider[m_iNoProvider].ProviderID, &m_Provider[m_iNoProvider].pszName) && 
		(m_iNoProvider<MAX_PROVIDERS) ) {
		m_iCurProvider = m_iNoProvider;
		++m_iNoProvider;
	}	
}


void SPSoundBase::ReleaseProviders()
{
	if(m_OpendProvider != 0) {	
		AIL_close_3D_provider(m_OpendProvider);
		m_OpendProvider=0;
	}
}

bool SPSoundBase::OpenProviders()
{
	ReleaseProviders();

	M3DRESULT result;

	//load the new provider
	result = AIL_open_3D_provider(m_Provider[m_iCurProvider].ProviderID);
	if (result != M3D_NOERR) 
	{
		m_OpendProvider	= 0;		
		ReleaseProviders();
		return false;
	} 

	m_OpendProvider = m_Provider[m_iCurProvider].ProviderID;		
	//see if we're running under an EAX compatible provider
	result=AIL_3D_room_type(m_OpendProvider);
	m_iUsingEAX= (((S32)result)!=-1)?1:0;
	return true;
}

bool SPSoundBase::SetRampBGM(FADETYPE FadeType)
{
	HPROVIDER volume_ramp;
	if ( AIL_find_filter( "Volume Ramp Filter", &volume_ramp ) ) {  
		AIL_set_stream_processor( m_MainBGM, DP_FILTER, volume_ramp );
		
		F32	fRampLevel;

		m_FadeType = FadeType;
		switch(m_FadeType) {
		case FADE_IN:
			{			
				m_fRampTime = 0.0f;
				fRampLevel = 0.0f;
				AIL_set_filter_stream_preference( m_MainBGM, "Ramp Time", &m_fRampTime );			
				AIL_set_filter_stream_preference( m_MainBGM, "Ramp To", &fRampLevel );


				m_fRampTime = FADETIME;
				fRampLevel = m_fBGMVolume;
				AIL_set_filter_stream_preference( m_MainBGM, "Ramp Time", &m_fRampTime );			
				AIL_set_filter_stream_preference( m_MainBGM, "Ramp To", &fRampLevel );

				m_fRampTime = FADE_NULL;
			}
			break;

		case FADE_OUT:
			{
				m_fRampTime = 0.0f;
				fRampLevel = m_fBGMVolume;
				AIL_set_filter_stream_preference( m_MainBGM, "Ramp Time", &m_fRampTime );			
				AIL_set_filter_stream_preference( m_MainBGM, "Ramp To", &fRampLevel );

				m_fRampTime = FADETIME;
				fRampLevel = 0.0f;
				AIL_set_filter_stream_preference( m_MainBGM, "Ramp Time", &m_fRampTime );			
				AIL_set_filter_stream_preference( m_MainBGM, "Ramp To", &fRampLevel );
			}
			break;

		default:
			break;
		}	

		return true;
	}

	return false;
}

int	SPSoundBase::GetSoundOption()
{
	return m_iRTOption;
}

float	SPSoundBase::GetBGMVolume()
{
	return m_fBGMVolume;
}

float	SPSoundBase::GetEffectVolume()
{
	return m_fEffectVolume;
}

bool SPSoundBase::IsPlayDone(SPSoundUnit* pSoundStuff)
{	
	/*
	SMP_FREE				 Sample is available for allocation
	SMP_DONE				 Sample has finished playing, or has never been started
	SMP_PLAYING				 Sample is playing
	SMP_STOPPED				 Sample has been stopped
	SMP_PLAYINGBUTRELEASED	 Sample is playing, but digital handle has been temporarily released
	*/
	if(SMP_DONE == AIL_3D_sample_status( pSoundStuff->m_hSample )) {
		return true;
	}
	return false;
}

void SPSoundBase::SetRoomType(int iRoomType)
{
	m_iRoomType = iRoomType;
}


bool SPSoundBase::AllocateArchive(unsigned __int64& ui64Guid, SPSoundArchive** ppSoundArchive)
{
	SPSoundArchive* pSoundArchiveCreate = NULL;
	(*ppSoundArchive) = NULL;

	map<unsigned __int64, SPSoundArchive*>::iterator Iter = m_mpArchive.find(ui64Guid);
	if(Iter != m_mpArchive.end() && (ui64Guid == (*Iter).first) ) {
		(*ppSoundArchive) = (*Iter).second;		

	} else {
		pSoundArchiveCreate = new SPSoundArchive(ui64Guid);
		m_mpArchive.insert ( map<unsigned __int64, SPSoundArchive*>::value_type ( ui64Guid, pSoundArchiveCreate) );
		(*ppSoundArchive) = pSoundArchiveCreate;
	}
	return true;
}

void SPSoundBase::FreeArchive(unsigned __int64& ui64Guid)
{
	map<unsigned __int64, SPSoundArchive*>::iterator Iter = m_mpArchive.find(ui64Guid);
	if(Iter != m_mpArchive.end() && (ui64Guid == (*Iter).first) ) {		
		SAFE_DELETE((*Iter).second);
		m_mpArchive.erase(Iter);
	} else {		
		//OutputDebugString("GU_ID not found.\n");
		//assert(0 && "GU_ID not found.");
	}	
}


bool SPSoundBase::AllocateSample(const char* pszFileName, SPSoundUnit* pSoundStuff)
{
	if( IsEnableEffect() == false ) {
		return false;
	}

	//obtain a 3D sample handle
	pSoundStuff->m_hSample		=	AIL_allocate_3D_sample_handle(m_OpendProvider);
	if(pSoundStuff->m_hSample == 0) 
		return false;

	pSoundStuff->m_pSampleAddress = LoadSampleFile(pszFileName);
	if(pSoundStuff->m_pSampleAddress == 0) 
		return false;

	//now try to load the sample into the provider
	if (AIL_set_3D_sample_file(pSoundStuff->m_hSample, pSoundStuff->m_pSampleAddress) == 0) {
		AIL_mem_free_lock(pSoundStuff->m_pSampleAddress);
		AIL_release_3D_sample_handle(pSoundStuff->m_hSample);		
		return false;
	}

	// set the sample to loop and set the distances
	AIL_set_3D_sample_loop_count	(pSoundStuff->m_hSample,	1);
	AIL_set_3D_sample_distances		(pSoundStuff->m_hSample,	300,	40);
	AIL_set_3D_sample_cone			(pSoundStuff->m_hSample,	90,		120,	0.0f);
    AIL_set_3D_sample_playback_rate	(pSoundStuff->m_hSample,	DIGITALRATE);

	SetEchoType(pSoundStuff, m_iRoomType);
	AIL_set_3D_sample_volume(	pSoundStuff->m_hSample, m_fEffectVolume );
	pSoundStuff->m_strFileName = pszFileName;	

	return true;
}

void SPSoundBase::SetEchoType(SPSoundUnit* pSoundStuff, int iVal)
{
	if ((m_OpendProvider) && (m_iUsingEAX))
	{
		AIL_set_3D_room_type(m_OpendProvider, iVal);

		// turn off EAX, if they choose no reverb. iVal ordinary equal m_iRoomType.
		if (iVal == ENVIRONMENT_GENERIC) {
			AIL_set_3D_sample_effects_level(pSoundStuff->m_hSample, 0.0F);
		} else {
			// -1 is the provider default level
			AIL_set_3D_sample_effects_level(pSoundStuff->m_hSample, -1.0F);
		}
	}
}

bool SPSoundBase::SetMainBGM(const char* pszFilename)
{
	bool bRet = false;	

	if(pszFilename == NULL)
		m_strBGMFileName.clear();
	else {
		if(m_strBGMFileName == pszFilename) { // 같은 지역일때 다시 시작하는게 아니고 이어지게..
			return true;
		}
		m_strBGMFileName = pszFilename;
	}

	if(m_MainBGM) {		
		SetRampBGM(FADE_OUT);			
		bRet = true;
	} else {
		bRet = AllocBGMStream();
	}
	return bRet;	
}

bool SPSoundBase::AllocBGMStream()
{
	if(m_strBGMFileName.empty())
		return false;

	m_MainBGM = AIL_open_stream( m_DIG, m_strBGMFileName.c_str(), 0 );
	if (m_MainBGM)
	{
		AIL_set_stream_user_data( m_MainBGM, 0, AIL_stream_playback_rate( m_MainBGM ) );
		AIL_service_stream( m_MainBGM, 1 );
		AIL_start_stream( m_MainBGM );
		AIL_set_stream_loop_count(	m_MainBGM, 0 );	
		UpdateBGMVolume(m_fBGMVolume);
		SetRampBGM(FADE_IN);
		return true;
	}
	return false;
}


void SPSoundBase::Process(float fTime)
{
	if(m_FadeType == FADE_IN) return;
	if(m_fRampTime == 0.0f) return;
	
	m_fFadeAccumulate += fTime;
	if( (m_fFadeAccumulate * 1000) > m_fRampTime ) {
		m_fFadeAccumulate = 0.0f;
		m_fRampTime = 0.0f;
		m_FadeType = FADE_NULL;

		AIL_close_stream( m_MainBGM );
		m_MainBGM = 0;

		AllocBGMStream();
	}
}



float SPSoundBase::UpdateEffectVolume( float fNewPos )
{	
	if (fNewPos < 0.0f)
		fNewPos = 0.0f;
	else if (fNewPos > 1.0f)
		fNewPos = 1.0f;
	
	m_fEffectVolume = fNewPos;

	return m_fEffectVolume;
}

float SPSoundBase::UpdateBGMVolume( float fNewPos )
{
	m_fBGMVolume = fNewPos;
	if (m_MainBGM) {
		float fVolBuf = IsEnableBGM() ? m_fBGMVolume : 0.0f;
		AIL_set_stream_volume_pan(	m_MainBGM, 
									fVolBuf, 
									0.5f );
	}
	return m_fBGMVolume;
}

void SPSoundBase::EnableEffect( bool bEnable )
{
	if(bEnable)
		m_iRTOption |= ENABLE_EFFECT;
	else 
		m_iRTOption &= ~ENABLE_EFFECT;

	std::map<unsigned __int64, SPSoundArchive*>::iterator Iter = m_mpArchive.begin();
	for(;Iter != m_mpArchive.end() ; ++Iter) {
		((*Iter).second)->UpdateEffect();
	}	
}

void SPSoundBase::EnableBGM( bool bEnable )
{
	if(bEnable)
		m_iRTOption |= ENABLE_BGM;
	else 
		m_iRTOption &= ~ENABLE_BGM;
	
	UpdateBGMVolume( m_fBGMVolume );
}

void SPSoundBase::EnableSystem( bool bEnable )
{
	if(bEnable)
		m_iRTOption |= ENABLE_SYSTEM;
	else 
		m_iRTOption &= ~ENABLE_SYSTEM;
	
	UpdateBGMVolume( m_fBGMVolume );

	std::map<unsigned __int64, SPSoundArchive*>::iterator Iter = m_mpArchive.begin();
	for(;Iter != m_mpArchive.end() ; ++Iter) {
		((*Iter).second)->UpdateEffect();
	}	
}

bool SPSoundBase::IsEnableEffect()
{	
	if( m_iRTOption & ENABLE_EFFECT && 
		m_iRTOption & ENABLE_SYSTEM &&
		m_iRTOption & ENABLE_OPTION) {
		return true;
	}
	return false;
}

bool SPSoundBase::IsEnableBGM()
{
	if( m_iRTOption & ENABLE_BGM	&& 
		m_iRTOption & ENABLE_SYSTEM &&
		m_iRTOption & ENABLE_OPTION) {
		return true;
	}
	return false;
}

void* SPSoundBase::LoadSampleFile(const char* pszFilename)
{
	void* pSourceStream;
	void* pDecompressedStream = 0;
	S32 iSoundType;
	AILSOUNDINFO SoundInfo;
	S32 iSize;
	int iRet;

	iSize=AIL_file_size(pszFilename);
	pSourceStream=AIL_file_read(pszFilename,	0);

	if (pSourceStream==0)
		return(0);

	iSoundType=AIL_file_type(pSourceStream,iSize);


	switch (iSoundType)
	{
	case AILFILETYPE_PCM_WAV:
		return(pSourceStream);

	case AILFILETYPE_ADPCM_WAV:
		AIL_WAV_info(pSourceStream,&SoundInfo);
		AIL_decompress_ADPCM(&SoundInfo, &pDecompressedStream, 0);
		AIL_mem_free_lock(pSourceStream);
		return(pDecompressedStream);

	case AILFILETYPE_MPEG_L3_AUDIO:
		iRet = AIL_decompress_ASI(pSourceStream, iSize, pszFilename, &pDecompressedStream, 0, 0);
		AIL_mem_free_lock(pSourceStream);
		return(pDecompressedStream);

	default:
		AIL_mem_free_lock(pSourceStream);
		return(0);
	}
}



