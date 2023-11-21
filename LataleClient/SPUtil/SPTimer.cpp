
// Copyright (C) duragon
//***************************************************************************
// System Name : SPTimmer
// Comment     : 
// Creation    : duragon 2007-02-05 ¿ÀÈÄ 2:56:36
//***************************************************************************

#include "SPTimer.h"

SPTimer::SPTimer()
{
	Clear();
}


SPTimer::~SPTimer()
{
	Clear() ;
}


void SPTimer::Clear()
{
	m_bEnable = false ;
	m_bExpired = false ;
	m_iTimerID = 0 ;
	m_fLimit = 0.0f ;
	m_fAccumulate = 0.0f ;
}


void SPTimer::Start(int iTimerID, float fLimit)
{
	m_bEnable = true ;
	m_bExpired = false ;
	m_iTimerID = iTimerID ;
	m_fLimit = fLimit ;
	m_fAccumulate = 0.0f ;
}

bool SPTimer::IsEnable()
{
	return m_bEnable ;
}

bool SPTimer::IsExpired()
{
	return m_bExpired ;
}


bool SPTimer::CheckTimer(float fTime) 
{
	if( m_bEnable == false )	return false ;
	if( m_bExpired == true )	return true ;

	m_fAccumulate += fTime ;
	m_bExpired = (m_fLimit < m_fAccumulate) ? true : false ;
	return m_bExpired ;
}

bool SPTimer::CheckTimerEx( float fTime )
{
	if( m_bEnable == false )	return false ;
	if( m_bExpired == true )	return true ;

	m_fAccumulate += fTime ;
	m_bExpired = ( m_fLimit <= m_fAccumulate ) ? true : false ;
	return m_bExpired ;
}

float SPTimer::GetShowTime()
{
	return m_fAccumulate;
}

float SPTimer::GetLimitTime()
{
	return m_fLimit;
}

void SPTimer::SetExpired()
{
	m_bExpired = true;
}

