// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment : 
// Creation : metalgeni 2005-04-11 오후 4:48:46
//***************************************************************************

#pragma once

// .NET2003 전용 해시맵 사용때문에.. 
#ifdef _DEFINE_DEPRECATED_HASH_CLASSES
#undef _DEFINE_DEPRECATED_HASH_CLASSES
#define _DEFINE_DEPRECATED_HASH_CLASSES 0	
#endif


#include <hash_map>
#include <string>

class SPLDTFile;
//class SPResourceMgr;

class SPLDTManager {
public:
	~SPLDTManager();

	static SPLDTManager* GetInstance();	
	//{
	//	return m_pInstance;
	//}

	static SPLDTManager* Create();
	//{
	//	if(m_pInstance == NULL) {
	//		m_pInstance = new SPLDTManager;			
	//	} 
	//	return m_pInstance;
	//}

	static void Release();
	//{
	//	if(m_pInstance != NULL) {
	//		SAFE_DELETE(m_pInstance);
	//	}
	//	m_pInstance = NULL;
	//}	
	
	bool		LoadLDTFileFromFile(const char* szFileName, SPLDTFile** ppLDTFile);
	bool		UnloadLDTFile(const char* szFileName);	

	///< 저장 루틴 추가 [200611:duragon]
	bool		SaveLDTFile(const char* szSrcFileName, const char* szDstFileName);
	bool		SaveLDTFile(SPLDTFile* pkLDTFile , const char* szDstFileName);
	bool		SaveLDTToTxt(SPLDTFile* pkLDTFile , const char* szDstFileName);
	bool		SaveLDTToTxt(const char* szSrcLDTName, const char* szDstTxtName);

#ifdef _LATALE_SERVER
	void		SetResourceDir(std::string sDir);
	std::string		GetResourceDir();
#endif

protected:
	SPLDTManager();

protected:
	stdext::hash_map<std::string, SPLDTFile*> m_hmLDTFileList;	

	//SPResourceMgr*			m_pResManager;
	static SPLDTManager*	m_pInstance;

#ifdef _LATALE_SERVER
	std::string m_sResourceDir;
#endif
};




