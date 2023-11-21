
#include "SPCoreDef.h"

#include "SPUtil.h"
//#include "SPResourceMgr.h"
#include "SPTextureMgr.h"

#include <string>
#include <sstream>
#include "../../LataleClient/SPUtil/SPXTrace.h"

using namespace std;
using namespace stdext;


#define TEX_SAFE_TIME 30000
extern FLOAT  g_fFPS;
extern bool   g_bTooSlow;
bool g_Loading = true;

hash_map<std::string, SPTexture*>	g_TexCache[TYPE_NUM];
vector<SPTexture*>		g_TexLoad;

CRITICAL_SECTION g_TexLoadSection;

float            g_TexSize[TYPE_NUM];
float            g_TexMaxSize[TYPE_NUM];
DWORD            g_RenderFrame = 0;

static const char *TEX_TYPE_STR[] =
{
	"Charactor",
		"Monster",
		"Item",
		"FX",
		"Interface",
		"Terrain",
		"Other"
};

DWORD WINAPI LoadTexthread(LPVOID lParam)
{
	SPTexture* pTextureCreate = NULL;

	while(1)
	{
		EnterCriticalSection(&g_TexLoadSection);
		
		vector<SPTexture*>::iterator itr = g_TexLoad.begin();
		if(g_Loading || itr == g_TexLoad.end())
		{
			LeaveCriticalSection(&g_TexLoadSection);
			Sleep(5);
		}
		else
		{
			//string strFileName;
			//const char * szFileName;

			//szFileName = ((*itr).first).c_str();

			//hash_map<string, SPTexture*>::iterator Iter = g_TexCache[(*itr).second].find(szFileName);
			//if(Iter == g_TexCache[(*itr).second].end())
			{
				pTextureCreate = (*itr);
				pTextureCreate->_autoReloadTexture();

				//if(SPResourceBase::GetInstance()->IsManaged() == false)
				//{
				//	strFileName = SPResourceBase::GetInstance()->GetWorkPath();
				//	string strKey;
				//	GetUpperString3(strKey, szFileName);
				//	strFileName += strKey;
			
				//	hr = pTextureCreate->LoadTexture(strFileName.c_str(), g_pVideo->GetTextureCreateFormat());
				//}
				//else
				//{
				//	GetUpperString3(strFileName, szFileName);
				//	hr = pTextureCreate->LoadTexturePack(strFileName.c_str(),g_pVideo->GetTextureCreateFormat());
				//}

				//if(FAILED(hr)) {
				//	SAFE_DELETE(pTextureCreate);
				//}
				//else
				//{
				//	g_TexCache[(*itr).second].insert(hash_map<string, SPTexture*>::value_type ( szFileName, pTextureCreate));
				//	g_TexSize[(*itr).second] += pTextureCreate->GetLenX() * pTextureCreate->GetLenY() / 262144.0f;
				//	pTextureCreate->setTexType((*itr).second);
				//	pTextureCreate->SetTexName(szFileName);
				//}

			}
            
			g_TexLoad.erase(itr);
			LeaveCriticalSection(&g_TexLoadSection);
			//Sleep(2);
		}
	};

	return 0;
}

int SPTextureMgr::CheckTexSize()
{
#ifdef _DEBUG
	DWORD time_start = timeGetTime();
	char tt[256];
#endif

	return 0;

	SPTexture* pTexture;
	m_icheck ++;

	if(m_icheck == TEX_TERRAIN) //场景之前的才需要定时清理
	{
		m_icheck = 0;
	}
	
	EnterCriticalSection(&g_TexLoadSection);

#ifdef _DEBUG
	printDictSize();
#endif

	if(g_TexSize[m_icheck] > g_TexMaxSize[m_icheck])
	{
		//xTrace( "******type %s, size %f, max %f.\n", TEX_TYPE_STR[m_icheck], g_TexSize[m_icheck], g_TexMaxSize[m_icheck] );
		DWORD timenow = timeGetTime();

		hash_map<string, SPTexture*>::iterator Iter = g_TexCache[m_icheck].begin();
		while(Iter != g_TexCache[m_icheck].end())
		{
			pTexture = (*Iter).second;
			if(pTexture->GetD3DTexture() != NULL)
			{
				if(pTexture->GetRenderFrame() == g_RenderFrame || timenow - pTexture->GetCreateTime() < TEX_SAFE_TIME)
				{
				}
				else
				{
					if((pTexture->GetRenderTimes()* 1000) / (timenow - pTexture->GetCreateTime())  < (g_fFPS/10))
					{
						g_TexSize[m_icheck] -= pTexture->GetLenX() * pTexture->GetLenY() / 262144.0f;
						pTexture->ReleaseTexture();
						if(g_TexSize[m_icheck] < (g_TexMaxSize[m_icheck] *.8f))
							break;
					}
				}
			}

			++ Iter;
		}

		LeaveCriticalSection(&g_TexLoadSection);

#ifdef _DEBUG
		DWORD time_end = timeGetTime();
		DWORD time_diff = time_end - time_start;
		sprintf(tt, "CheckSize ok : %s, Used time: %d.\n", TEX_TYPE_STR[m_icheck], time_diff);
		OutputDebugString(tt);
#endif

		return -1;
	}

	LeaveCriticalSection(&g_TexLoadSection);
    
	return -1;
}

void SPTextureMgr::printDictSize()
{
	stringstream stream;
	stream.clear();
	stream << "[---] ";
	for (int idx = 0; idx < TYPE_NUM; ++idx)
	{
		stream << string( TEX_TYPE_STR[idx] ) << ": " << g_TexSize[idx] << " || ";
	}
	stream << "\n\0";

//#ifdef _DEBUG
//	DXUTOutputDebugString( stream.str().c_str() );
//#endif
}

bool GetCutWorkPath(const char* pszFileName, std::string& strCutName)
{
	bool bRetVal = false;
	string strFileName = pszFileName;
	string strWorkPath = SPResourceBase::GetInstance()->GetWorkPath();

	int iPos = (int)strFileName.find( strWorkPath );
	if( iPos != -1 ) {
		strFileName	= strFileName.substr( strWorkPath.length(), strFileName.length() - strWorkPath.length() );
		bRetVal = true;
	}

	strCutName	= strFileName.c_str();
	return true;
}




SPTextureMgr* SPTextureMgr::m_pInstance = NULL;


SPTextureMgr::SPTextureMgr()
{		
	//if( SPResourceBase::GetInstance()->IsManaged() == false) {
	//	pfLoadTexture = LoadTextureFromFile;
	//} else {
	//	pfLoadTexture = LoadTextureFromPack;
	//}
	//InitializeCriticalSection(&g_TextureLoadSection);

	Init();
}


SPTextureMgr::~SPTextureMgr()
{
	hash_map<string, SPTexture*>::iterator Iter = m_hmTextureList.begin();
	for(; Iter != m_hmTextureList.end(); ++Iter) {
		SAFE_DELETE((*Iter).second);
	}
	m_hmTextureList.clear();

	DeleteCriticalSection(&g_TextureLoadSection) ;
	EnterCriticalSection(&g_TexLoadSection);

	for(int i=0;i<TYPE_NUM;++i) //
	{
		hash_map<string, SPTexture*>::iterator Iter = g_TexCache[i].begin();
		for(;Iter != g_TexCache[i].end();++Iter)
		{
			SAFE_DELETE((*Iter).second);
		}
		g_TexCache[i].clear();
	}
}

bool SPTextureMgr::Init()
{
	InitializeCriticalSection(&g_TextureLoadSection);
	InitializeCriticalSection(&g_TexLoadSection);

	//m_hThreadHandle = NULL;
	//m_hThreadHandle = CreateThread(NULL, 0, LoadTexthread, NULL, 0, &m_dwID);
	m_icheck = 0;

//	MEMORYSTATUS mem;
//	GlobalMemoryStatus(&mem);
//	if(mem.dwTotalPhys/1000/1000 < 800) //小于 1G 的内存分配
//	{
//		g_TexMaxSize[0] = 120.5f;
//		g_TexMaxSize[1] = 18.5f;
//		g_TexMaxSize[2] = 2.5f;
//		g_TexMaxSize[3] = 5.5f;
//		g_TexMaxSize[4] = 15.5f;
//		g_TexMaxSize[5] = .5f;
//		g_TexMaxSize[6] = .5f;
//	}
//	else								 //大于 1G 的内存分配
//	{
//		g_TexMaxSize[0] = 180.5f;
//		g_TexMaxSize[1] = 20.5f;
//		g_TexMaxSize[2] = 5.5f;
//		g_TexMaxSize[3] = 15.5f;
//		g_TexMaxSize[4] = 25.5f;
//		g_TexMaxSize[5] = .5f;
//		g_TexMaxSize[6] = .5f;
///*
//		g_TexMaxSize[0] = 0.5f;
//		g_TexMaxSize[1] = 0.5f;
//		g_TexMaxSize[2] = .5f;
//		g_TexMaxSize[3] = .5f;
//		g_TexMaxSize[4] = .5f;
//		g_TexMaxSize[5] = .5f;
//		g_TexMaxSize[6] = .5f;
//
//*/
//	}

	//各类纹理处理方式
	// 人物是比较大的一类纹理，需要定时清理，为了避免频繁加载，当纹理小于一定值时暂停释放
	// 特效和人物处理类似
	// 怪物和人物处理类似
	//
	// 界面和道具，因为用到的几率比较小，纹理池可以给予比较小的内存，不用是立即释放，可以采用16位的纹理来管理
	// 场景的管理采用预先加载的方式，不用时立即释放。
	//

	return true;
}

bool SPTextureMgr::GetTexture(const char* szFileName, SPTexture** ppTexture, TEX_TYPE type, TEX_LEVEL level, D3DFORMAT D3DFormat)
{
	if(type >= TYPE_NUM || strlen(szFileName) < 3 ) return false;

	//if(strlen(szFileName) < 5 || strpbrk(szFileName, ".png") == 0)
	//{
	//	return false;
	//}

	return LoadTexture(szFileName, ppTexture, D3DFormat);

//	string strKey;
//	GetUpperString3(strKey, szFileName);
//
//
////	if(g_bTooSlow && (type == TEX_CHAR )) return false;
//
//	EnterCriticalSection(&g_TexLoadSection);
//
//	hash_map<string, SPTexture*>::iterator Iter = g_TexCache[type].find(strKey);
//	if(Iter != g_TexCache[type].end())
//	{
//		*ppTexture = (*Iter).second;
//		(*ppTexture)->IncreaseRef();
//
//		LeaveCriticalSection(&g_TexLoadSection);
//		return true;
//	}
//	else
//	{
//		//if(level == LEVEL_NOW)
//		{
//			bool bre = LoadTex(strKey.c_str(), ppTexture, type, D3DFormat,level);
//			
//			//hash_map<string, TEX_TYPE>::iterator itr = g_TexLoad.find(szFileName);
//			//if(itr != g_TexLoad.end())
//			//{
//			//	g_TexLoad.erase(itr);
//			//}
//			
//			LeaveCriticalSection(&g_TexLoadSection);
//			return bre;
//		}
//		//else
//		//{
//			//hash_map<string, TEX_TYPE>::iterator itr = g_TexLoad.find(szFileName);
//			//if(itr == g_TexLoad.end())
//			//{
//			//	g_TexLoad.insert( hash_map<string, TEX_TYPE>::value_type(szFileName, type));
//			//}
//		//}
//	}
//
//	LeaveCriticalSection(&g_TexLoadSection);
//	return false;
}

bool SPTextureMgr::LoadTexture(const char* szFileName, SPTexture** ppTexture, D3DFORMAT D3DFormat /* = D3DFMT_UNKNOWN */)
{
	if(strlen(szFileName) < 5 || strpbrk(szFileName, ".png") == 0)
	{
		return false;
	}

	if( D3DFormat == D3DFMT_UNKNOWN)
		D3DFormat	=	g_pVideo->GetTextureCreateFormat();

	bool bRet;
	if(SPResourceBase::GetInstance()->IsManaged() == false) {
		bRet = LoadTextureFromFile(szFileName, ppTexture, D3DFormat);
	} else {
		bRet = LoadTextureFromPack(szFileName, ppTexture, D3DFormat);
	}
	
	return bRet;

	//if( D3DFormat == D3DFMT_UNKNOWN)
	//	D3DFormat	=	g_pVideo->GetTextureCreateFormat();

	//string ss;
	//GetUpperString3(ss, szFileName);

	////if(ss.find("DATA/TERRAIN") == 0 || ss.find("DATA/BGFORMAT") == 0 || ss.find("DATA/BACKGROUND") == 0)
	////{
	////	int kk = 0;
	////}
	//////if(ss.find("Data/GlobalRes/") == 0)
	//if(ss.find("DATA/GLOBALRES") == 0)
	//{
	//	return GetTexture(szFileName,ppTexture,TEX_OTHER, LEVEL_NOW, D3DFormat);
	//}
	//else
	//{
	//	return GetTexture(szFileName,ppTexture,TEX_INTERFACE, LEVEL_NOT, D3DFormat);
	//}

}

bool SPTextureMgr::LoadTex(const char* szFileName, SPTexture** ppTexture, TEX_TYPE type, D3DFORMAT D3DFormat, TEX_LEVEL level)
{
	string strFileName;
	//string strKey;

	SPTexture* pTextureCreate = NULL;
	(*ppTexture) = NULL;
	HRESULT hr;

	if( D3DFormat == D3DFMT_UNKNOWN)
		D3DFormat	=	g_pVideo->GetTextureCreateFormat();

	pTextureCreate = new SPTexture;

	if(SPResourceBase::GetInstance()->IsManaged() == false)
	{
		strFileName = SPResourceBase::GetInstance()->GetWorkPath();
		strFileName += szFileName;
	
		if(level == LEVEL_NOW)
			hr = pTextureCreate->LoadTexture(strFileName.c_str(), D3DFormat);
	}
	else
	{
		GetUpperString3(strFileName, szFileName);
		if(level == LEVEL_NOW)
			hr = pTextureCreate->LoadTexturePack(strFileName.c_str(),D3DFormat);
	}

	if(level == LEVEL_NOW)
	{
		if(FAILED(hr)) {
			SAFE_DELETE(pTextureCreate);
			return false;
		}
	}

	g_TexCache[type].insert(hash_map<string, SPTexture*>::value_type ( szFileName, pTextureCreate));
	* ppTexture = pTextureCreate;
	pTextureCreate->setTexType(type);
	pTextureCreate->IncreaseRef();
	pTextureCreate->SetTexName(szFileName);
	pTextureCreate->SetD3DFormat(D3DFormat);
	if(level == LEVEL_NOW)
		g_TexSize[type] += pTextureCreate->GetLenX() * pTextureCreate->GetLenY() / 262144.0f;
	else if(level == LEVEL_LOW)
		g_TexLoad.push_back(pTextureCreate);

	return true;
//	string strFileName;
//	SPTexture* pTextureCreate = NULL;
//	string strKey;
//
//	(*ppTexture) = NULL;
//	HRESULT hr;
//
//	GetUpperString3(strKey, szFileName);
//
//	if( D3DFormat == D3DFMT_UNKNOWN)
//		D3DFormat	=	g_pVideo->GetTextureCreateFormat();
//
//	pTextureCreate = new SPTexture;
//
//	if(SPResourceBase::GetInstance()->IsManaged() == false)
//	{
//		strFileName = SPResourceBase::GetInstance()->GetWorkPath();
//	
//		strFileName += strKey;
//		
//		hr = pTextureCreate->LoadTexture(strFileName.c_str(), D3DFormat);
//	}
//	else
//	{
//		//GetUpperString3(strFileName, szFileName);
//		hr = pTextureCreate->LoadTexturePack(strKey.c_str(),D3DFormat);
//	}
//
//	if(FAILED(hr)) {
//		SAFE_DELETE(pTextureCreate);
//		return false;
//	}
//
////	g_TexCache[type].insert(hash_map<string, SPTexture*>::value_type ( strKey, pTextureCreate));
//	* ppTexture = pTextureCreate;
//	pTextureCreate->setTexType(type);
//	pTextureCreate->IncreaseRef();
//	pTextureCreate->SetTexName(szFileName);
//
//	g_TexSize[type] += pTextureCreate->GetLenX() * pTextureCreate->GetLenY() / 262144.0f;
//
//	return true;
}

bool SPTextureMgr::LoadTextureFromFile(const char* szFileName, SPTexture** ppTexture, D3DFORMAT D3DFormat)
{	
	string strFileName = SPResourceBase::GetInstance()->GetWorkPath();
	//string strKey = GetUpperString2(szFileName);

	string strKey;
	GetUpperString3(strKey, szFileName);

	strFileName += strKey;

	SPTexture* pTextureCreate = NULL;
	 (*ppTexture) = NULL;

	hash_map<string, SPTexture*>::iterator Iter = m_hmTextureList.find(strKey);
	if(Iter != m_hmTextureList.end() && (strKey == (*Iter).first) ) {
		(*ppTexture) = (*Iter).second;	
	} else {
		pTextureCreate = new SPTexture;
		HRESULT hr = pTextureCreate->LoadTexture(strFileName.c_str(), D3DFormat);
		if(FAILED(hr)) {
			SAFE_DELETE(pTextureCreate);
			return false;
		}

		pTextureCreate->m_strFileName = strKey;

		m_hmTextureList.insert ( hash_map<string, SPTexture*>::value_type ( strKey, pTextureCreate) );
		(*ppTexture) = pTextureCreate;
	}
	(*ppTexture)->IncreaseRef();
	return true;
}

bool SPTextureMgr::LoadTextureFromPack(const char* szFileName, SPTexture** ppTexture, D3DFORMAT D3DFormat)
{	
	//string strFileName = GetUpperString2(szFileName);
	string strFileName;
	GetUpperString3(strFileName, szFileName);

	SPTexture* pTextureCreate = NULL;
	(*ppTexture) = NULL;

	hash_map<string, SPTexture*>::iterator Iter = m_hmTextureList.find(strFileName);
	if(Iter != m_hmTextureList.end() && (strFileName == (*Iter).first) ) {
		(*ppTexture) = (*Iter).second;
	} else {
		pTextureCreate = new SPTexture;
		HRESULT hr = pTextureCreate->LoadTexturePack(strFileName.c_str(),D3DFormat);
		if(FAILED(hr)) {
			SAFE_DELETE(pTextureCreate);
			return false;
		}
		pTextureCreate->m_strFileName = strFileName.c_str();
		
		//GetCutWorkPath(strFileName.c_str(), pTextureCreate->m_strFileName );

		m_hmTextureList.insert ( hash_map<string, SPTexture*>::value_type ( pTextureCreate->m_strFileName, pTextureCreate) );
		(*ppTexture) = pTextureCreate;
	}
	(*ppTexture)->IncreaseRef();
	return true;
}

bool SPTextureMgr::LoadMemoryTexture(const char* szFileName, SPTexture** ppTexture)
{
	if(SPResourceBase::GetInstance()->IsManaged() == false) {
		return LoadMemoryTextureFromFile(szFileName, ppTexture);
	} else {
		return LoadMemoryTextureFromPack(szFileName, ppTexture);
	}
}

bool SPTextureMgr::LoadMemoryTextureFromFile(const char* szFileName, SPTexture** ppTexture)
{	
	//string strFileName = SPResourceBase::GetInstance()->GetWorkPath();
	//string strKey = GetUpperString2(szFileName);

	string strKey = szFileName;
	//GetUpperString3(strKey, szFileName);

	//strFileName += strKey;

	SPTexture* pTextureCreate = NULL;
	(*ppTexture) = NULL;

	//hash_map<string, SPTexture*>::iterator Iter = m_hmTextureList.find(strKey);
	//if(Iter != m_hmTextureList.end() && (strKey == (*Iter).first) )
	//	(*ppTexture) = (*Iter).second;	
	//else
	//	return false;

	hash_map<string, SPTexture*>::iterator Iter = g_TexCache[TEX_OTHER].find(strKey);
	if(Iter != g_TexCache[TEX_OTHER].end() && (strKey == (*Iter).first) )
		(*ppTexture) = (*Iter).second;	
	else
		return false;

	
	(*ppTexture)->IncreaseRef();
	return true;
}

bool SPTextureMgr::LoadMemoryTextureFromPack(const char* szFileName, SPTexture** ppTexture)
{	
	//string strFileName = GetUpperString2(szFileName);
	string strFileName;
	GetUpperString3(strFileName, szFileName);

	SPTexture* pTextureCreate = NULL;
	(*ppTexture) = NULL;

	hash_map<string, SPTexture*>::iterator Iter = m_hmTextureList.find(strFileName);
	if(Iter != m_hmTextureList.end() && (strFileName == (*Iter).first) )
		(*ppTexture) = (*Iter).second;
	else
		return false;
		
	(*ppTexture)->IncreaseRef();
	return true;
}

void SPTextureMgr::CheckLoadTex(SPTexture *pTex)
{
	vector<SPTexture*> ::iterator it = g_TexLoad.begin();
	for(;it != g_TexLoad.end(); ++it)
	{
		if(pTex == *it)
		{
			it = g_TexLoad.erase(it);
			if(it == g_TexLoad.end())
				break;
		}
	}
}

bool SPTextureMgr::UnloadTexture(const char* szFileName, TEX_TYPE type)
{
	////string strFileName = GetUpperString(szFileName);
	//string strFileName = szFileName;
	//hash_map<string, SPTexture*>::iterator Iter = m_hmTextureList.find(strFileName);

	//if(Iter != m_hmTextureList.end())  {		
	//	//KeyCompare = m_hmTextureList.key_comp();
	//	//if( m_hmTextureList.key_comp() (strFileName, (*Iter).first) == true) {

	//	if( strFileName == (*Iter).first) {
	//		SPTexture* pTexture = (*Iter).second;
	//		if(pTexture->GetRefCount() == 0) {
	//			SAFE_DELETE(pTexture);
	//			m_hmTextureList.erase(Iter);
	//			return true;
	//		} 
	//	}
	//}

	//assert(0 && "烙矫 攫肺靛眉农, UNLOAD FAILED");
	//return false;

		//以前的纹理池
try
{
	if(type == TYPE_NUM)
	{
		//string strFileName = GetUpperString(szFileName);
		string strFileName = szFileName;
		hash_map<string, SPTexture*>::iterator Iter = m_hmTextureList.find(strFileName);

		if(Iter != m_hmTextureList.end())  {		
			//KeyCompare = m_hmTextureList.key_comp();
			//if( m_hmTextureList.key_comp() (strFileName, (*Iter).first) == true) {

			if( strFileName == (*Iter).first) {
				SPTexture* pTexture = (*Iter).second;
				if(pTexture->GetRefCount() == 0) {
					SAFE_DELETE(pTexture);
					m_hmTextureList.erase(Iter);
					return true;
				} 
			}
		}
	}
	else
	{
		EnterCriticalSection(&g_TexLoadSection);

		string strFileName = szFileName;
		hash_map<string, SPTexture*>::iterator Iter = g_TexCache[type].find(strFileName);
		
		if(Iter != g_TexCache[type].end())
		{
			if(type == TEX_TERRAIN || type == TEX_OTHER) //直接删除
			{
				SPTexture* pTexture = (*Iter).second;
				g_TexSize[type] -= pTexture->GetLenX() * pTexture->GetLenY() / 262144.0f;
				CheckLoadTex(pTexture);
				SAFE_DELETE(pTexture);
				g_TexCache[type].erase(Iter);
			}
			else
			{
				if(g_TexSize[type] > (g_TexMaxSize[type]*.75))
				{
					SPTexture* pTexture = (*Iter).second;
					g_TexSize[type] -= pTexture->GetLenX() * pTexture->GetLenY() / 262144.0f;
					CheckLoadTex(pTexture);
					SAFE_DELETE(pTexture);
					g_TexCache[type].erase(Iter);
				}
			}
		}

		LeaveCriticalSection(&g_TexLoadSection);

		return true;
	}
}
catch(...)
{
	return false;
}
	return false;
}













