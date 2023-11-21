#include "SPGameObjectDef.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

GU_ID GetGlobalID(CLASS_ID ClassID, INSTANCE_ID InstanceID ) 
{
	static GU_ID guid;
	guid = ClassID;
	guid <<= 32;
	guid |= InstanceID;
	
	return guid;
}

void GetLocalID(GU_ID guid, CLASS_ID& ClassID, INSTANCE_ID& InstanceID) 
{
	InstanceID = (INSTANCE_ID)(guid & 0xFFFFFFFF);
	ClassID = (CLASS_ID)(guid >> 32);	
}

INSTANCE_ID GetGUID2InstanceID	(GU_ID guid)
{
	return (INSTANCE_ID)(guid & 0xFFFFFFFF);
}

CLASS_ID	GetGUID2ClassID		(GU_ID guid)
{
	return (CLASS_ID)(guid >> 32);	
}

void SetMOBIdentify(unsigned int uiClassID, unsigned int& uiID, unsigned int& uiKind)
{
	uiKind = (unsigned char)(uiClassID & 0x000000FF);
	uiID = ((uiClassID & 0x0000FF00) >> 8) + ((uiClassID & 0x000F0000) >> 16) * 256;
}

void SetMOBIdentify(const char* pszName, unsigned int& uiID, unsigned int& uiKind)
{
	uiID = 0;
	uiKind = 0;

	char szID[4], szKind[3];
	if( pszName[0] != '2' )
		return;

	strncpy(szID, &pszName[2], 3 * sizeof(char));
	szID[3] = '\0';

	strncpy(szKind, &pszName[6], 2 * sizeof(char));
	szKind[2] = '\0';

	uiID = atoi(szID);
	uiKind = atoi(szKind);
}

unsigned int GetMOBClassIDConvert(unsigned int uiID, unsigned int uiKind)
{
	unsigned int uiClassID;

	uiClassID = 0x30300000 + ((uiID & 0x00000FFF) << 8) + (uiKind & 0x000000FF);
	return uiClassID;
}

const char* GetMOBFileName(unsigned int uiClassID)
{
	static char s_szBuf[256];
	
	unsigned int uiID, uiKind;
	SetMOBIdentify(uiClassID, uiID, uiKind);
	
	sprintf(s_szBuf, "2_%03d_%02d", uiID, uiKind);
	return s_szBuf;
}

void SetMOBLayerIdentify(const char* pszName, unsigned int& uiID, unsigned int& uiKind)
{
	uiID = 0;
	uiKind = 0;

	char szID[4], szKind[3];
	if( pszName[0] != '3' )
		return;

	strncpy(szID, &pszName[2], 3 * sizeof(char));
	szID[3] = '\0';

	strncpy(szKind, &pszName[6], 2 * sizeof(char));
	szKind[2] = '\0';

	uiID = atoi(szID);
	uiKind = atoi(szKind);
}

const char* GetMOBLayerFileName(unsigned int uiClassID)
{
	static char s_szBuf[256];

	unsigned int uiID, uiKind;
	SetMOBIdentify(uiClassID, uiID, uiKind);

	sprintf(s_szBuf, "3_%03d_%02d", uiID, uiKind);
	return s_szBuf;
}

void SetNPCIdentify(const char* pszName, unsigned int& uiID, unsigned int& uiKind)
{
	uiID = 0;
	uiKind = 0;

	char szID[4], szKind[3];
	if( pszName[0] != '1' )
		return;

	strncpy(szID, &pszName[2], 3 * sizeof(char));
	szID[3] = '\0';

	strncpy(szKind, &pszName[6], 2 * sizeof(char));
	szKind[2] = '\0';

	uiID = atoi(szID);
	uiKind = atoi(szKind);
}

const char* GetNPCFileName(unsigned int uiClassID)
{
	static char s_szBuf[256];

	unsigned int uiID, uiKind;
	SetMOBIdentify(uiClassID, uiID, uiKind);

	sprintf(s_szBuf, "1_%03d_%02d", uiID, uiKind);
	return s_szBuf;
}


unsigned int GetGOBSeqKey(unsigned int iItemID, unsigned int iSeqIndex, unsigned char iClassType /*= 0*/)
{
	unsigned int iRet = 0;
	
	//iRet = iItemID * 100 + iSeqIndex;
	
#ifndef _CLASS_CHANGE_VER
	iRet = iItemID * 100 + iSeqIndex;
#else
	if(iClassType) {
		iRet = (iClassType * 1000 + iItemID ) * 100 + iSeqIndex;
	}
	else{
		iRet = iItemID * 100 + iSeqIndex;
	}
#endif
	
	return iRet;
}

