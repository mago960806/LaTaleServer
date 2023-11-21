
// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPReport
// Comment     : 
// Creation    : metalgeni 2005-09-02 ¿ÀÈÄ 2:56:36
//***************************************************************************

#pragma once 




#ifndef SPREPORT_H

	#include <wtypes.h>

	#define SPREPORT_H

	const BYTE		Key[] = "metalgeni";
	const int		ciKeyLen = 9;

	const BYTE		HeaderTitle[] = "LataleReportLog";
	const int		ciCurrentVersion = 1;

	const char*		ccEos = "EOS";
	const int		ciEosLen = strlen(ccEos);

	typedef unsigned int F_LRF_VERSION;
	struct ReportLogHeader {
		char szDesc[32];
	};


#endif





