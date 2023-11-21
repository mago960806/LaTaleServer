// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPGlobalTimer
// Comment     : 
// Creation    : metalgeni 2005-10-7 ¿ÀÀü 10:26:19
//***************************************************************************

#pragma once


#include <windows.h>

double	GetGlobalAbsoluteTime(); 
float	GetGlobalElapsedTime(); 

class SPGlobalTimer {
public:
	static SPGlobalTimer* GetInstance();
	static void Release();

	void Reset(); // resets the timer
	void Start(); // starts the timer
	void Stop();  // stop (or pause) the timer

	float GetElapsedTime();
	double GetAbsoluteTime();
	void GetTimeValues( double* pfTime, double* pfAbsoluteTime, float* pfElapsedTime );
	static void LimitThreadAffinityToCurrentProc();

private:
	SPGlobalTimer();
	~SPGlobalTimer();
	LARGE_INTEGER GetAdjustedCurrentTime();


private:
	static SPGlobalTimer* m_pInstance;

	bool m_bUsingQPF;
	bool m_bTimerStopped;
	LONGLONG m_llQPFTicksPerSec;
	LONGLONG m_llStopTime;
	LONGLONG m_llLastElapsedTime;
	LONGLONG m_llBaseTime;

};



