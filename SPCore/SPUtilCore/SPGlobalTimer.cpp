

#include "SPGlobalTimer.h"
#include "assert.h"

//using namespace std;


double	GetGlobalAbsoluteTime()
{
	return SPGlobalTimer::GetInstance()->GetAbsoluteTime();
} 

float	GetGlobalElapsedTime()
{
	return SPGlobalTimer::GetInstance()->GetElapsedTime();
}




SPGlobalTimer* SPGlobalTimer::m_pInstance = NULL;


SPGlobalTimer::SPGlobalTimer()
: m_bTimerStopped     (true)
, m_llQPFTicksPerSec  (0)
, m_llStopTime        (0)
, m_llLastElapsedTime (0)
, m_llBaseTime        (0)
{
	LARGE_INTEGER qwTicksPerSec = { 0 };
	QueryPerformanceFrequency( &qwTicksPerSec );
	m_llQPFTicksPerSec = qwTicksPerSec.QuadPart;
}

SPGlobalTimer::~SPGlobalTimer()
{

}

SPGlobalTimer* SPGlobalTimer::GetInstance() 
{
	if(!m_pInstance) {
		m_pInstance =  new SPGlobalTimer;
	}
	return m_pInstance;
}


void SPGlobalTimer::Release()
{
	if(m_pInstance) {
		delete m_pInstance;
		m_pInstance = NULL;
	}
}


LARGE_INTEGER SPGlobalTimer::GetAdjustedCurrentTime()
{
	LARGE_INTEGER qwTime;
	if( m_llStopTime != 0 )
		qwTime.QuadPart = m_llStopTime;
	else
		QueryPerformanceCounter( &qwTime );
	return qwTime;
}

void SPGlobalTimer::Reset()
{
	LARGE_INTEGER qwTime = GetAdjustedCurrentTime();

	m_llBaseTime        = qwTime.QuadPart;
	m_llLastElapsedTime = qwTime.QuadPart;
	m_llStopTime        = 0;
	m_bTimerStopped     = FALSE;
}

void SPGlobalTimer::Start()
{
	// Get the current time
	LARGE_INTEGER qwTime = { 0 };
	QueryPerformanceCounter( &qwTime );

	if( m_bTimerStopped )
		m_llBaseTime += qwTime.QuadPart - m_llStopTime;
	m_llStopTime = 0;
	m_llLastElapsedTime = qwTime.QuadPart;
	m_bTimerStopped = FALSE;
}

void SPGlobalTimer::Stop()
{
	if( !m_bTimerStopped )	{
		LARGE_INTEGER qwTime = { 0 };
		QueryPerformanceCounter( &qwTime );
		m_llStopTime = qwTime.QuadPart;
		m_llLastElapsedTime = qwTime.QuadPart;
		m_bTimerStopped = TRUE;
	}

}

float SPGlobalTimer::GetElapsedTime()
{
	LARGE_INTEGER qwTime = GetAdjustedCurrentTime();

	double fElapsedTime = (float) ((double) ( qwTime.QuadPart - m_llLastElapsedTime ) / (double) m_llQPFTicksPerSec);
	m_llLastElapsedTime = qwTime.QuadPart;

	// See the explanation about clamping in CDXUTTimer::GetTimeValues()
	if( fElapsedTime < 0.0f )
		fElapsedTime = 0.0f;

	return (float)fElapsedTime;
}

double SPGlobalTimer::GetAbsoluteTime()
{
	LARGE_INTEGER qwTime = { 0 };
	QueryPerformanceCounter( &qwTime );

	double fTime = qwTime.QuadPart / (double) m_llQPFTicksPerSec;

	return fTime;
}

void SPGlobalTimer::GetTimeValues( double* pfTime, double* pfAbsoluteTime, float* pfElapsedTime )
{
	assert( pfTime && pfAbsoluteTime && pfElapsedTime );    

	LARGE_INTEGER qwTime = GetAdjustedCurrentTime();

	float fElapsedTime = (float) ((double) ( qwTime.QuadPart - m_llLastElapsedTime ) / (double) m_llQPFTicksPerSec);
	m_llLastElapsedTime = qwTime.QuadPart;

	// Clamp the timer to non-negative values to ensure the timer is accurate.
	// fElapsedTime can be outside this range if processor goes into a 
	// power save mode or we somehow get shuffled to another processor.  
	// However, the main thread should call SetThreadAffinityMask to ensure that 
	// we don't get shuffled to another processor.  Other worker threads should NOT call 
	// SetThreadAffinityMask, but use a shared copy of the timer data gathered from 
	// the main thread.
	if( fElapsedTime < 0.0f )
		fElapsedTime = 0.0f;

	*pfAbsoluteTime = qwTime.QuadPart / (double) m_llQPFTicksPerSec;
	*pfTime = ( qwTime.QuadPart - m_llBaseTime ) / (double) m_llQPFTicksPerSec;   
	*pfElapsedTime = fElapsedTime;
}

void SPGlobalTimer::LimitThreadAffinityToCurrentProc()
{
	HANDLE hCurrentProcess = GetCurrentProcess();

	// Get the processor affinity mask for this process
	DWORD_PTR dwProcessAffinityMask = 0;
	DWORD_PTR dwSystemAffinityMask = 0;

	if( GetProcessAffinityMask( hCurrentProcess, &dwProcessAffinityMask, &dwSystemAffinityMask ) != 0 && dwProcessAffinityMask )
	{
		// Find the lowest processor that our process is allows to run against
		DWORD_PTR dwAffinityMask = ( dwProcessAffinityMask & ((~dwProcessAffinityMask) + 1 ) );

		// Set this as the processor that our thread must always run against
		// This must be a subset of the process affinity mask
		HANDLE hCurrentThread = GetCurrentThread();
		if( INVALID_HANDLE_VALUE != hCurrentThread )
		{
			SetThreadAffinityMask( hCurrentThread, dwAffinityMask );
			CloseHandle( hCurrentThread );
		}
	}

	CloseHandle( hCurrentProcess );
}





