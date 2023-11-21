#pragma once

#include "XTrace71.h"
/*#define __TRACE_BEGIN \
	DWORD time_s = timeGetTime();

#define __TRACE_END \
	DWORD time_e = timeGetTime(); \
	DWORD time_dis = time_e - time_s; \
	if (time_dis > 1) \
		xTrace( __FUNCTION__ "() used time: %d ms.\n", time_dis );*/

#define __TRACE_BEGIN 

#define __TRACE_END 