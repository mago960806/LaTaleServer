// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPSoundArchive
// Comment     : 
// Creation    : metalgeni 2005-1-26 ¿ÀÀü 10:26:19
//***************************************************************************

#pragma once

#include <vector>

class SPSoundUnit;
class SPSoundBase;
class SPGameObject;

class SPSoundArchive {
public:		
	virtual void Init();
	virtual void Release();
	virtual void Clean();
	virtual void Process(float fTime, POINT pos);

	bool	InsertItem(const char* pszFilename, float fDelay = 0.0f, bool bLoop = false);
	bool	ForceRelease ( const char* pszFilename );
	bool	UpdateEffect();	

protected:
	friend SPSoundBase;
	SPSoundArchive(unsigned __int64 ui64Guid);
	virtual ~SPSoundArchive();

protected:

	POINT							m_Position;
	unsigned __int64				m_ui64Guid;	
	std::vector<SPSoundUnit*>		m_vpSoundUnit;
	SPGameObject*					m_pParent;

};














