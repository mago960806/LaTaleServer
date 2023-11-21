// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-08-24 ¿ÀÀü 10:26:19
//***************************************************************************

#pragma once

#include "dxutil.h"


class SPFrameSkipper  
{
protected:
	DWORD m_dwOldTime;
	LONG  m_lElapsedTime;
	LONG  m_lMsPerFrm;
	void  UpdateTime()
	{
		DWORD dwCurrentTime = timeGetTime();

		m_lElapsedTime += dwCurrentTime - m_dwOldTime;
		m_dwOldTime     = dwCurrentTime;
	}

public:
	SPFrameSkipper  ()					{ Init( 30 ); }
	SPFrameSkipper  ( LONG lFPS )		{ Init( lFPS ); }
	virtual ~SPFrameSkipper()		{}

	void Init( LONG lFPS )
	{
		m_lElapsedTime = 0;
		m_dwOldTime    = timeGetTime();
		m_lMsPerFrm    = 1000 / lFPS;
	}

	void SetTime()
	{
		UpdateTime();
		while( m_lElapsedTime < m_lMsPerFrm )	UpdateTime();

		m_lElapsedTime -= m_lMsPerFrm;
	}

	BOOL CheckFrameSkip()
	{
		UpdateTime();
		return ( (m_lElapsedTime < m_lMsPerFrm) ? TRUE : FALSE );
	}
};


