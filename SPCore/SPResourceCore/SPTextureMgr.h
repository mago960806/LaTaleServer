// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment : 
// Creation : metalgeni 2004-09-16 오후 4:48:46
//***************************************************************************

#pragma once

// .NET2003 전용 해시맵 사용때문에.. 
#ifdef _DEFINE_DEPRECATED_HASH_CLASSES
#undef _DEFINE_DEPRECATED_HASH_CLASSES
#define _DEFINE_DEPRECATED_HASH_CLASSES 0	
#endif


#include <hash_map>
#include <string>

class SPTexture;


enum TEX_TYPE
{
	TEX_CHAR = 0,
	TEX_MON,
	TEX_ITEM,
	TEX_FX,
	TEX_INTERFACE,
	TEX_TERRAIN,

	TEX_OTHER,

	TYPE_NUM,
};

enum TEX_LEVEL
{
	LEVEL_NOW,
	LEVEL_LOW,
	LEVEL_NOT,
};


bool GetCutWorkPath(const char* pszFileName, std::string& strCutName);

class SPTextureMgr {
public:
	~SPTextureMgr();

	static SPTextureMgr* GetInstance()
	{
		return m_pInstance;
	}

	static SPTextureMgr* Create()
	{
		if(m_pInstance == NULL) {
			m_pInstance = new SPTextureMgr;			
		}
		return m_pInstance;
	}

	static void Release()
	{
		if(m_pInstance != NULL) {
			SAFE_DELETE(m_pInstance);
		}
		m_pInstance = NULL;
	}

	bool	(SPTextureMgr::*pfLoadTexture) (const char* szFileName, SPTexture** ppTexture, D3DFORMAT D3DFormat);
	bool	LoadTexture  (const char* szFileName, SPTexture** ppTexture, D3DFORMAT D3DFormat = D3DFMT_UNKNOWN);
	bool	UnloadTexture(const char* szFileName,TEX_TYPE type = TYPE_NUM);

	bool	LoadMemoryTexture(const char* szFileName, SPTexture** ppTexture);

	bool    Init();
	bool    GetTexture(const char* szFileName, SPTexture** ppTexture, TEX_TYPE type, TEX_LEVEL level = LEVEL_LOW,D3DFORMAT D3DFormat = D3DFMT_UNKNOWN);
	int     CheckTexSize();
protected:

	SPTextureMgr();
	bool	LoadTextureFromFile(const char* szFileName, SPTexture** ppTexture, D3DFORMAT D3DFormat);
	bool	LoadTextureFromPack(const char* szFileName, SPTexture** ppTexture, D3DFORMAT D3DFormat);

	bool	LoadMemoryTextureFromFile(const char* szFileName, SPTexture** ppTexture);
	bool	LoadMemoryTextureFromPack(const char* szFileName, SPTexture** ppTexture);

	bool    LoadTex(const char* szFileName, SPTexture** ppTexture, TEX_TYPE type, D3DFORMAT D3DFormat,TEX_LEVEL level);
	void    CheckLoadTex(SPTexture *pTex);

protected:
	stdext::hash_map<std::string, SPTexture*> m_hmTextureList;
	int m_iNextID;	

	static SPTextureMgr*	m_pInstance;

	HANDLE m_hThreadHandle;
	DWORD m_dwID;
	int    m_icheck;
private:
	void printDictSize();


};




