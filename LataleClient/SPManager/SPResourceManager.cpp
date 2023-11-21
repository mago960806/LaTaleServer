

#include "SPCommon.h"
#include "SPResourceBase.h"

#include "SPGameObjectDEF.h"
#include "SPManager.h"
//#include "SPItem.h"
#include "SPItemCluster.h"			//[2005/6/16] - jinhee

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"
#include "SPDebug.h"

SPResourceManager* g_pResourceManager = NULL;

SPResourceManager::SPResourceManager()
: SPManager()
, m_pItemCluster(NULL)
, m_pLDTDataTable(NULL)
, m_pLDTGlobalRes(NULL)
, m_pLDTGlobalString(NULL)
, m_pLDTGlobalColor(NULL)
//, m_pResourceBase(NULL)
{
	SetMgrID(MGR_ID_RESOURCE);
	g_pResourceManager = this;	
	//Init();
}

SPResourceManager::~SPResourceManager()
{
	Clean();
	g_pResourceManager = NULL;
}

bool SPResourceManager::Init()
{
#if defined(_DEBUG)
	DXUTOutputDebugString("ResourceManager::Init()\n");
#endif

	m_pLDTManager = SPLDTManager::Create();
	GetLDTFromFileName( RES_FNAME_LDT_GLOBAL_RES,		&m_pLDTGlobalRes	);
	GetLDTFromFileName( RES_FNAME_LDT_GLOBAL_STRING,	&m_pLDTGlobalString	);	

	GetLDTFromFileName( RES_FNAME_LDT_CHAT_COLOR,		&m_pLDTGlobalColor	);

	m_pItemCluster = new SPItemCluster;	
	SPManager::Init();
	return true;
}

bool SPResourceManager::InitItemCluster()
{
	if( m_pItemCluster )	m_pItemCluster->Init() ;
	return true ;
}

void SPResourceManager::Clean()
{
	SAFE_RELEASE(m_pLDTDataTable);
	SAFE_RELEASE(m_pLDTGlobalRes);
	SAFE_RELEASE(m_pLDTGlobalString);

	SAFE_RELEASE(m_pLDTGlobalColor);

	SAFE_DELETE( m_pItemCluster );
	SAFE_RELEASE( m_pLDTManager );

	SPManager::Clean();
}

bool SPResourceManager::GetLDTFromFileName	(const char* szFileName, SPLDTFile** ppLDTFile)
{
	return SPLDTManager::GetInstance()->LoadLDTFileFromFile(szFileName, ppLDTFile);
}

bool SPResourceManager::GetLDTFromInx(int iDbName, SPLDTFile** ppLDTFile)
{
	return false;
}


const char* SPResourceManager::GetGlobalFileName(int iItemID)
{
	static LDT_Field ldtField;
	if( m_pLDTGlobalRes == NULL )
		return NULL;

	if ( m_pLDTGlobalRes->GetFieldFromLable( iItemID, "_FILENAME",		ldtField) )
		return ldtField.uData.pValue;
	return NULL;
}

#ifdef _DEBUG
const char* SPResourceManager::GetGlobalString(int iItemID)
{
	static LDT_Field ldtField;
	bool bResult = false;
	
	if( m_pLDTGlobalString == NULL )	{
		_stprintf(m_szError, "LDTGlobalString is NULL") ;
		return m_szError;
	}

	bResult = m_pLDTGlobalString->GetFieldFromLable( iItemID, "_STRING", ldtField);
	
	if(!bResult)	{
		_stprintf(m_szError, "[%d] can't find", iItemID) ;
		return m_szError ;
	}
	
	return ldtField.uData.pValue;
}
#else
const char* SPResourceManager::GetGlobalString(int iItemID)
{
	static LDT_Field ldtField;
	bool bResult = false;
	
	if( m_pLDTGlobalString == NULL )
		return NULL;

	bResult = m_pLDTGlobalString->GetFieldFromLable( iItemID, "_STRING", ldtField);
	
	if(!bResult)						//[2005/5/6] - jinhee
		return NULL;
	
	return ldtField.uData.pValue;
}
#endif

const D3DXCOLOR SPResourceManager::GetGlobalColor(int iItemID)
{
	static LDT_Field ldtFieldR;
	static LDT_Field ldtFieldG;
	static LDT_Field ldtFieldB;
	static float fRed = 255.0f;
	static float fGreen = 255.0f;
	static float fBlue = 255.0f;
	bool bResult = true;
	
	D3DXCOLOR defaultColor(1.0f, 1.0f, 1.0f, 1.0f);

	if(m_pLDTGlobalColor == NULL || iItemID == 0)
		return defaultColor;

	bResult &= m_pLDTGlobalColor->GetFieldFromLable( iItemID, "_Red", ldtFieldR);
	bResult &= m_pLDTGlobalColor->GetFieldFromLable( iItemID, "_Green", ldtFieldG);
	bResult &= m_pLDTGlobalColor->GetFieldFromLable( iItemID, "_Blue", ldtFieldB);

	if(!bResult)						//[2005/5/6] - jinhee
		return defaultColor;

	fRed = ldtFieldR.uData.lValue;
	fGreen = ldtFieldG.uData.lValue;
	fBlue = ldtFieldB.uData.lValue;

	D3DXCOLOR color((fRed / 255.0f), (fGreen / 255.0f), (fBlue / 255.0f), 1.0f);
	
	return color;
}



