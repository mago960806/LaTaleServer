
// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPResourceBaseDEF
// Comment     : 
// Creation    : metalgeni 2005-06-08 오전 10:26:19
//***************************************************************************

#pragma once

// _NEWPACK은 DEBUG_NEWPACK에서 정의 되있음.

//#define		MAX_RESOURCE_FILE	7 // 2006/2/14 추가
//#define		MAX_RESOURCE_FILE	27 //  [9/7/2007 metalgeni] 24->27
#define		MAX_RES_NAME		128

//extern const char* RESOURCE_FILES[];

typedef unsigned int		RES_INSTANCE_ID;	// Instance in SPF (It using just 3BYTE - 0 to 16777215 )  
typedef unsigned char		RES_FILE_ID;		// RESOURCE_FILE 
typedef unsigned int		RESID;				// RESID = RES_FILE_ID + RES_INSTANCE_ID

struct FINFO {
	char	szFileName[MAX_RES_NAME];
	int		iOffset;
	int		iSize;
	RESID	ResID;
};


//////////////////////////////////////////////////////////////////////////
//WILL  아..씨 인스턴스ID 테스트하구 바로 손볼것 ( #define으로 미리 정의하는걸루 )
struct F_READSTREAM {
	int			iRefCount;
	int			iSize;
	BYTE*		pBuffer;	
	F_READSTREAM();
	~F_READSTREAM();
};

typedef int F_SPF_VERSION;

struct F_SPF_HEADER {
	int		iHeaderSize;
	int		iFileID;
	char*	szDesc[32];
};


//////////////////////////////////////////////////////////////////////////
//	Hash Code 생성	GPG 카피~ ^^
DWORD			GetHashCode				( const char* pstrString );

//////////////////////////////////////////////////////////////////////////
// Get RESID
RESID			GetResID			(RES_FILE_ID ClassID, RES_INSTANCE_ID InstanceID );

// Get RES_INSTANCE_ID, RES_FILE_ID
void			GetFileInstanceID	(RESID guid, RES_FILE_ID& ClassID, RES_INSTANCE_ID& InstanceID); 
RES_INSTANCE_ID GetResID2InstanceID	(RESID guid);
RES_FILE_ID		GetResID2FileID		(RESID guid);




