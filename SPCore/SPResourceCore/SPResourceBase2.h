
// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPResourceBase
// Comment     : 
// Creation    : metalgeni 2005-05-10 오전 10:26:19
//***************************************************************************

#if 0 

#pragma once

#include <hash_map>
#include <map>

// .NET2003 전용 해시맵 사용때문에.. 
#ifdef _DEFINE_DEPRECATED_HASH_CLASSES
#undef _DEFINE_DEPRECATED_HASH_CLASSES
#define _DEFINE_DEPRECATED_HASH_CLASSES 0	
#endif

using namespace std;
using namespace stdext;

extern bool g_bUsePackFile;

struct FINFO;
struct F_READINDEX;
struct F_READSTREAM;

class SPStream;
class SPManagedStream;

class SPResourceBase {
public:
	~SPResourceBase();
	static SPResourceBase* GetInstance();
	static SPResourceBase* Create();
	void Release();
	bool Init(const char* pszBasePath);

	bool GetStreamData(const char* pszFilename,	SPStream** ppStream );
	bool GetStreamData(RESID iInstanceID,			SPStream** ppStream );
	void ReleaseStreamData(RESID iInstanceID);
	
	RESID FilenameToInstance(const char* pszFilename );

	bool IsManaged() const;

	void SetWorkPath(const char* pszPath);
	const char* GetWorkPath() const; 

protected:
	SPResourceBase();	
	bool				GetData(int iFileID);	


	FINFO*			FilenameToIndex(const char* pszFilename );

protected:

	static SPResourceBase* s_pMilesInstance;	


	stdext::hash_map<std::string,	FINFO*>			m_hmFInfoListName;
	stdext::hash_map<RESID,			FINFO*>			m_hmFInfoListIID;
	SPStream*	m_pPackStream[MAX_RESOURCE_FILE];	// _NEWPACK
	
	stdext::hash_map<RESID,			F_READSTREAM*>	m_hmStreamList;

	bool		m_bUsingResBase;	
	int			m_iCount;

	string		m_strWorkPath;

};

#endif
