
#include <WTypes.h>	

#include "SPCommandConvert.h"


bool command_convert::Encode(UINT uiEnableCommandList, UINT uiDisableCommandList, UINT& uiResultCommandBuf)
{	
	uiResultCommandBuf = ((0x0000FFFF) & uiEnableCommandList) << 16 | (0x0000FFFF & uiDisableCommandList);
	return true;
}

bool command_convert::Decode(UINT uiCommandBuf, UINT& uiResultEnableList, UINT& uiResultDisableList)
{
	unsigned int iCommandList  = uiCommandBuf;
	uiResultEnableList	= iCommandList >> 16; 
	uiResultDisableList = iCommandList & 0x0000FFFF;
	return true;
}


