#include <algorithm>
#include <functional>
#include <string>
#include <vector>

#include "SPCoreDEF.h"

#include "SPSoundUnit.h"
#include "SPSoundBase.h"
#include "SPSoundArchive.h"

SPSoundArchive::SPSoundArchive(unsigned __int64 ui64Guid)
: m_ui64Guid( ui64Guid )
, m_pParent(NULL)
{
	Init();	
}

SPSoundArchive::~SPSoundArchive()
{	
	Clean();
}

void SPSoundArchive::Init()
{	
	m_Position.x = 0;
	m_Position.y = 0;
}

void SPSoundArchive::Release()
{
	SPSoundBase::GetInstance()->FreeArchive(m_ui64Guid);	
}

void SPSoundArchive::Clean()
{
	std::vector<SPSoundUnit*>::iterator Iter = m_vpSoundUnit.begin();
	for(;Iter != m_vpSoundUnit.end(); ++Iter) {
		SAFE_RELEASE(*Iter);
	}
	m_vpSoundUnit.clear();
}


bool SPSoundArchive::InsertItem(const char* pszFilename, float fDelay, bool bLoop)	
{
	if(SPSoundBase::GetInstance()->IsEnableEffect()) {
		SPSoundUnit* pUnit = new SPSoundUnit(this, fDelay);
		if(SPSoundBase::GetInstance()->AllocateSample( pszFilename, pUnit) == true) {		

			pUnit->UpdatePosition(m_Position);
			pUnit->SetLoopState(bLoop);

			m_vpSoundUnit.push_back(pUnit);
			return true;
		}

		OutputDebugString("Sound Allocate FAIL..!!");
		SAFE_RELEASE(pUnit);
	}
	return false;
}

bool SPSoundArchive::UpdateEffect()
{
	if(SPSoundBase::GetInstance()->IsEnableEffect() == false) {
		std::vector<SPSoundUnit*>::iterator Iter = m_vpSoundUnit.begin();
		for(;Iter != m_vpSoundUnit.end(); Iter++) {		
			SAFE_RELEASE(*Iter);		
		}
		m_vpSoundUnit.clear();
	}
	return true;
}

bool SPSoundArchive::ForceRelease ( const char* pszFilename )
{
	//if(SPSoundBase::GetInstance()->IsEnableEffect() == false) {
		std::vector<SPSoundUnit*>::iterator Iter = m_vpSoundUnit.begin();
		for(;Iter != m_vpSoundUnit.end(); Iter++) {		
			if((*Iter)->m_strFileName == pszFilename) {
				SAFE_RELEASE(*Iter);	
				m_vpSoundUnit.erase( Iter );
				return true;
			}
		}		
	//}
	return false;
}

void SPSoundArchive::Process(float fTime, POINT pos)
{	
	if(m_vpSoundUnit.empty())
		return;

	m_Position = pos;
	std::vector<SPSoundUnit*>::iterator Iter = m_vpSoundUnit.begin();
	for(;Iter != m_vpSoundUnit.end();) {
		if( (*Iter)->Process(fTime) == SPS_RELEASE_READY ) {
			SAFE_RELEASE(*Iter);
			Iter = m_vpSoundUnit.erase(Iter);
		} else {
			(*Iter)->UpdatePosition(pos);
			Iter++;
		}		
	}
}







