
// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPResourceBase
// Comment     : 
// Creation    : metalgeni 2005-05-10 오전 10:26:19
//***************************************************************************

//////////////////////////////////////////////////////////////////////////
// VERSION 2

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
	void ReserveInitFile(const char* apszReserveFiles[], int iNoFile);

	bool GetStreamData(const char* pszFilename,	SPStream** ppStream );
	bool GetStreamData(RESID iInstanceID,			SPStream** ppStream );
	void ReleaseStreamData(RESID iInstanceID);

	RESID FilenameToInstance(const char* pszFilename );

	bool IsManaged() const;

	void SetWorkPath(const char* pszPath);
	const char* GetWorkPath() const; 

	bool FindPackFirst(IN const char* pszPath, OUT char* pszFileNameWithPath, IN char* pszExt = NULL);
	bool FindPackNext(OUT char* pszFileNameWithPath);

	int				GetNoSPF();						// Number of SPF readed
	F_SPF_VERSION	GetSPFVersionByInx(const int iInx);	// 1 based Index. if invalid index, return 0;
	const char*		GetSPFNameByInx(const int iInx);		// 1 based Index. if invalid index, return NULL;

	

protected:
	SPResourceBase();	
	bool				GetData(int iFileID);

	FINFO*			FilenameToIndex(const char* pszFilename );

protected:

	static SPResourceBase* s_pResourceInstance;	


	stdext::hash_map<std::string,	FINFO*>			m_hmFInfoListName;
	stdext::hash_map<RESID,			FINFO*>			m_hmFInfoListIID;

	// Old Version
	//SPStream*		m_pPackStream[MAX_RESOURCE_FILE];	// _NEWPACK
	//F_SPF_VERSION	m_aiResVersion[MAX_RESOURCE_FILE];
	//string			m_astrResFileName[MAX_RESOURCE_FILE];

	int				m_iNoCurReadSPF;


	stdext::hash_map<RESID,			F_READSTREAM*>	m_hmStreamList;

	bool		m_bUsingResBase;	
	int			m_iCount;

	string		m_strWorkPath;

	vector<string>	m_vstrFileList;
	int				m_iFindInx;				
	string			m_strFindExt;
	string			m_strFindPath;


	//WILL 
	std::vector<std::string>		m_vstrFileNameForReserve;
	int								m_iNoFileNameForReserve;

	std::map<int, std::string>		m_mResFileRegisted;  // 등록된 화일명
	std::map<int, F_SPF_VERSION>	m_mSPFVerRegisted; 
	std::vector<SPStream*>			m_vStreamRegisted; 





};


