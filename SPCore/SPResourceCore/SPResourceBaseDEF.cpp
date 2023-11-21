

#ifdef _SP_CORE
	#include "SPCoreDef.h"
#elif _LATALE_SERVER
	#include <vector>
	#include <WTypes.h>
	#include "GlobalDefines.h"
#elif _SP_RES_TOOL
	#include "stdafx.h"
#elif _LATALE_GMTOOL
	#include <WTypes.h>
	#include "GlobalDefines.h"
#endif

#include "SPResourceBaseDEF.h"


//const char* RESOURCE_FILES[] = {
//	//"LATALE128.SPF",
//	"TESTPACK.SPF",
//	"AJJIYA.SPF",	
//	"HOSHIM.SPF",
//	"ROWID.SPF",
//	"JINSSAGA.SPF",
//	"MAKO1298.SPF",
//	"METALGENI.SPF", //7
//
//	"DALBONG.SPF", // 2006/2/14 추가
//	"RYUMS.SPF",
//	"BANX.SPF",
//	"BARY.SPF",
//	"ZENNE.SPF",
//	"CLAIRE.SPF",
//	"CVOICE.SPF", // 이하 세개는 스페어화일명
//	"GUSTAV.SPF",	
//	"CRI.SPF",
//	
//	"DURAGON.SPF", // 3차 추가분 2005/4/18
//	"TOMA.SPF",
//	"CLETS.SPF",
//	"BORORU.SPF",
//	"JOOX3.SPF",
//	"BONGSIK.SPF",
//	"RM.SPF",
//	"JJALRAJO.SPF",
//
//	"JJANG.SPF", //  [9/7/2007 metalgeni] 2007/9/7 추가분(중국)
//	"FREE.SPF", 
//	"BAS.SPF",
//
//
//};
//

F_READSTREAM::F_READSTREAM()
: iRefCount(0)
, iSize(0)
, pBuffer(0)
{}


F_READSTREAM::~F_READSTREAM()
{
	GlobalFree((HGLOBAL)pBuffer);
}

DWORD GetHashCode( const char* pstrString )
{
	DWORD	dwlen		=	(DWORD)strlen( pstrString );
	DWORD	dwresult	=	5381;
	DWORD	dwch;

	for( DWORD dwi = 0 ; dwi < dwlen ; dwi++ )
	{
		dwch = (DWORD)pstrString[dwi];
		dwresult = (( dwresult << 5 ) + dwresult) + dwch; // hash * 33 + ch
	}	  
	return dwresult;
}


RESID GetResID(RES_FILE_ID ClassID, RES_INSTANCE_ID InstanceID ) 
{
	static RESID guid;
	guid = ClassID;
	guid <<= 24;
	guid |= InstanceID;

	return guid;
}

void GetFileInstanceID(RESID guid, RES_FILE_ID& ClassID, RES_INSTANCE_ID& InstanceID) 
{
	InstanceID	= (RES_INSTANCE_ID)(guid & 0xFFFFFF);
	ClassID		= (RES_FILE_ID)(guid >> 24);	
}

RES_INSTANCE_ID GetResID2InstanceID	(RESID guid)
{
	return (RES_INSTANCE_ID)(guid & 0xFFFFFF);
}

RES_FILE_ID	GetResID2FileID		(RESID guid)
{
	return (RES_FILE_ID)(guid >> 24);	
}

