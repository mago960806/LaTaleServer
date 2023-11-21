
// Copyright (C) duragon
//***************************************************************************
// System Name : SPTimmer
// Comment     : 
// Creation    : duragon 2007-02-05 오후 2:56:36
//***************************************************************************

#pragma once

class SPTimer
{

public:
	SPTimer();
	~SPTimer();

	void  Clear();								// Init
	void  Start(int iTimerID, float fLimit);		// Timer Start
	bool  IsEnable();							// 
	bool  IsExpired() ;
	bool  CheckTimer(float fTime) ;				// Update Timer & Check Expired
	bool  CheckTimerEx( float fTime );
	float GetShowTime();						//[liuyang]add 2008.9.9
	float GetLimitTime();						//[liuyang]add 2008.9.9
	void  SetExpired();							//[liuyang]add 2008.9.9

private:
	bool	m_bEnable ;
	bool	m_bExpired ;
	int		m_iTimerID ;		// Timer 호출자
	float	m_fAccumulate ;
	float	m_fLimit ;			// Limit Time

};