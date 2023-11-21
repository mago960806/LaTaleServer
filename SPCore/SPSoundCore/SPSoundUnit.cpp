
#include "mss.h"
#include "SPSoundBase.h"
#include "SPSoundArchive.h"
#include "SPSoundUnit.h"


SPSoundUnit::SPSoundUnit(SPSoundArchive* pParent, float fDelay ) 
: m_hSample(0)
, m_pSampleAddress(0)
, m_fDelayPlay(fDelay)
, m_iPlayerState(SPS_PLAY_READY)
, m_fAccumulateTime(0.0f)
, m_pParent(pParent)
, m_bLoop(false)
{
}

SPSoundUnit::~SPSoundUnit()
{
}

void SPSoundUnit::Release()
{
	if(m_hSample) {
		AIL_release_3D_sample_handle(m_hSample);
	}

	if(m_pSampleAddress) {
		AIL_mem_free_lock( m_pSampleAddress ); 
	}
	delete this;	
}

void SPSoundUnit::UpdatePosition(POINT Pos)
{
	AIL_set_3D_position( m_hSample, (float)Pos.x, (float)Pos.y, 0.0f );
}

void SPSoundUnit::SetLoopState(bool bLoop)
{
	m_bLoop = bLoop;
}

bool SPSoundUnit::GetLoopState()
{
	return m_bLoop;
}

int SPSoundUnit::Process(float fTime)
{
	switch(m_iPlayerState) {
	case SPS_PLAY_READY:
		m_fAccumulateTime += fTime;
		if( m_fAccumulateTime > m_fDelayPlay ) {
			AIL_start_3D_sample( this->m_hSample );

			if(m_bLoop)
				AIL_set_3D_sample_loop_count(this->m_hSample, 100); // Not Forever

			m_iPlayerState = SPS_PLAY;
		}
		break;
	case SPS_PLAY:		
		if(SPSoundBase::GetInstance()->IsPlayDone(this) == true) {
			m_iPlayerState = SPS_RELEASE_READY;
		}
		break;

	case SPS_RELEASE_READY:
		break;	
	}
	return m_iPlayerState;
}









