#include "stdafx.h"
#include "queue.h"

CLock::CLock()
{
	::InitializeCriticalSection(&m_CS);
}
CLock::~CLock()
{
	::DeleteCriticalSection(&m_CS);
}

void CLock::lock()
{
	::EnterCriticalSection(&m_CS);
}

void CLock::unlock()
{
	::LeaveCriticalSection(&m_CS);
}
