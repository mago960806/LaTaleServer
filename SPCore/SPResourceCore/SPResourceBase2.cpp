
#if 0

#ifdef _SP_CORE
	#include "SPCoreDef.h"
#elif _LATALE_SERVER
	#include <vector>
	#include <WTypes.h>
	#include "GlobalDefines.h"
#endif

#include "SPResourceBaseDEF.h"
#include "SPUtil.h"
#include "SPStream.h"
#include "SPResourceBase.h"
#include "SPResourceBase2.h"


bool g_bUsePackFile = false;

SPResourceBase* SPResourceBase::s_pMilesInstance = 0;

SPResourceBase::SPResourceBase()
: m_iCount(0)
, m_bUsingResBase(false)
{	
	ZeroMemory(m_pPackStream, sizeof(m_pPackStream));
}

SPResourceBase::~SPResourceBase()
{
	stdext::hash_map<std::string, FINFO*>::iterator hmIter = m_hmFInfoListName.begin();
	for(; hmIter != m_hmFInfoListName.end() ; ++hmIter) {
		SAFE_DELETE((*hmIter).second);
	}
	m_hmFInfoListName.clear();
	m_hmFInfoListIID.clear();


	stdext::hash_map<RESID,	F_READSTREAM*>::iterator hmIterStream = m_hmStreamList.begin();	
	for(; hmIterStream != m_hmStreamList.end() ; ++hmIterStream) {
		SAFE_DELETE((*hmIterStream).second);
	}
	m_hmStreamList.clear();

	for(int i = 0; i < MAX_RESOURCE_FILE; ++i) {
		SAFE_RELEASE(m_pPackStream[i]);
	}
}

SPResourceBase* SPResourceBase::GetInstance()
{
	return s_pMilesInstance;
}

SPResourceBase* SPResourceBase::Create()
{
	if( s_pMilesInstance == 0 ) {
		s_pMilesInstance = new SPResourceBase;		
	}
	return s_pMilesInstance;
}

void SPResourceBase::Release()
{
	SAFE_DELETE(s_pMilesInstance);
}

bool SPResourceBase::Init(const char* pszBasePath)
{
	SetWorkPath(pszBasePath);

	

	for(int iRes = 0; iRes < MAX_RESOURCE_FILE; ++iRes) {	

		(m_pPackStream[iRes]) = new SPMemoryMappedFileStream(RESOURCE_FILES[iRes], SPFileStream::OPEN_READ);

		if((m_pPackStream[iRes])->Valid() == false) {	
			SAFE_RELEASE(m_pPackStream[iRes]);
			continue;		// TEST
			//return false; // REAL
		}	

		F_SPF_VERSION	spfVer;
		F_SPF_HEADER	spfHeader;
		int				iCount;

		(m_pPackStream[iRes])->Seek( -sizeof(F_SPF_VERSION), SEEK_END );	
		(m_pPackStream[iRes])->Read( &spfVer, sizeof(int));

		(m_pPackStream[iRes])->Seek( -(sizeof(F_SPF_VERSION) + sizeof(F_SPF_HEADER)), SEEK_END );		
		(m_pPackStream[iRes])->Read( &spfHeader, sizeof(F_SPF_HEADER));

		(m_pPackStream[iRes])->Seek( -(sizeof(F_SPF_VERSION) + sizeof(F_SPF_HEADER) + spfHeader.iHeaderSize), SEEK_END );
		iCount = (spfHeader.iHeaderSize) / sizeof(FINFO);	
		m_iCount += iCount;
		

		FINFO*			pFileInfo;
		string			strInstanceName;
		for(int i = 1; i <= iCount; ++i) {

			strInstanceName.clear();
			pFileInfo = new FINFO;
			(m_pPackStream[iRes])->Read( pFileInfo, sizeof(FINFO));			

			strInstanceName = GetUpperString( pFileInfo->szFileName );

			hash_map<string, FINFO*>::iterator hmIterName = m_hmFInfoListName.find(strInstanceName);
			if(hmIterName != m_hmFInfoListName.end() && (strInstanceName == (*hmIterName).first) ) {
				assert(0 && "Exist same data in packed file.");
				SAFE_DELETE ( pFileInfo );			
				continue;
			} else {
				m_hmFInfoListName.insert ( hash_map<string, FINFO*>::value_type ( strInstanceName, pFileInfo));		
			}

			hash_map<RESID, FINFO*>::iterator hmIterNum = m_hmFInfoListIID.find(pFileInfo->ResID);
			if(hmIterNum != m_hmFInfoListIID.end() && (pFileInfo->ResID == (*hmIterNum).first) ) {
				continue;
			} else {
				m_hmFInfoListIID.insert ( hash_map<RESID, FINFO*>::value_type ( pFileInfo->ResID, pFileInfo));		
			}		
		}
	}

	if(g_bUsePackFile)
		m_bUsingResBase = true;

	return true;
}


void SPResourceBase::SetWorkPath(const char* pszPath)
{
	m_strWorkPath = pszPath;
}

const char* SPResourceBase::GetWorkPath() const
{
	return m_strWorkPath.c_str();
}

bool SPResourceBase::IsManaged() const
{
	return m_bUsingResBase;
}

FINFO* SPResourceBase::FilenameToIndex(const char* pszFilename )
{
	string strInstanceName = GetUpperString(pszFilename);
	hash_map<string, FINFO*>::iterator Iter = m_hmFInfoListName.find(strInstanceName);
	if(Iter != m_hmFInfoListName.end() && (strInstanceName == (*Iter).first) ) {
		FINFO* pReadInx = (*Iter).second;
		return pReadInx;
	}
	return NULL;
}

RESID SPResourceBase::FilenameToInstance(const char* pszFilename )
{
	FINFO* pFInfo = FilenameToIndex(pszFilename);
	if(pFInfo)
		return pFInfo->ResID;

	return 0;
}


bool SPResourceBase::GetStreamData(const char* pszFilename,	SPStream** ppStream )
{
	FINFO* pFInfo = NULL;
	if(g_bUsePackFile) {
		pFInfo = FilenameToIndex(pszFilename );
		if(pFInfo) {
			return GetStreamData(pFInfo->ResID, ppStream);	
		} else  {
			*ppStream = NULL;
			return false;
		}
	} else {
		string strPullPath = m_strWorkPath;
		strPullPath += pszFilename;
		*ppStream = new SPFileStream(strPullPath.c_str());
		return true;
	}
}


bool SPResourceBase::GetStreamData(RESID iInstanceID,			SPStream** ppStream )
{
	SPStream*			pStreamCreate;
	F_READSTREAM*		pReadStream;

	// FIRST: Search stream data
	hash_map<RESID, F_READSTREAM*>::const_iterator IterStream = m_hmStreamList.find(iInstanceID);
	if(IterStream != m_hmStreamList.end() && (iInstanceID == (*IterStream).first) ) {		
		pReadStream = (*IterStream).second;

		++(pReadStream->iRefCount);
		pStreamCreate = new SPManagedStream( pReadStream->pBuffer, pReadStream->iSize, iInstanceID );
		(*ppStream) = pStreamCreate;

		return true;

	} else {		
		// SECOND: if not find stream data, search file info and create new readstream to stream pool.
		hash_map<RESID, FINFO*>::iterator IterFileInfo = m_hmFInfoListIID.find(iInstanceID);
		if(IterFileInfo != m_hmFInfoListIID.end() && (iInstanceID == (*IterFileInfo).first) ) {
			FINFO* pReadInx = (*IterFileInfo).second;
			pReadStream = new F_READSTREAM;

			//void* pBuff = new BYTE( pReadInx->FileInfo.iSize + 1);
			void* pBuff = GlobalAlloc(GMEM_FIXED, pReadInx->iSize + 1);

			RES_FILE_ID	iFileID = GetResID2FileID (iInstanceID);			

			int iRet = (m_pPackStream[iFileID])->Seek( pReadInx->iOffset, SEEK_SET );			
			(m_pPackStream[iFileID])->Read( pBuff, pReadInx->iSize );						

			pReadStream->pBuffer		= (BYTE*)pBuff;
			pReadStream->iSize			= pReadInx->iSize;
			//pReadStream->iInstanceID	= iInstanceID;			
			pReadStream->iRefCount		= 1;

			pStreamCreate = new SPManagedStream( pReadStream->pBuffer, pReadStream->iSize, iInstanceID );
			(*ppStream) = pStreamCreate;
			m_hmStreamList.insert ( hash_map<RESID, F_READSTREAM*>::value_type ( iInstanceID, pReadStream) );
				
			return true;
		}
	}

	// not exist file info
	(*ppStream) = NULL;	
	return false;
}


void SPResourceBase::ReleaseStreamData(RESID iInstanceID)
{
	F_READSTREAM* pReadStream;
	hash_map<RESID, F_READSTREAM*>::iterator hmIterStream = m_hmStreamList.find(iInstanceID);
	if(hmIterStream != m_hmStreamList.end() && (iInstanceID == (*hmIterStream).first) ) {
		pReadStream = (*hmIterStream).second;
		--(pReadStream->iRefCount);

		if(pReadStream->iRefCount == 0) {
			assert( pReadStream->iRefCount >= 0 ); 
			SAFE_DELETE(pReadStream);
			m_hmStreamList.erase(hmIterStream);
		}
	} else {
		assert(0);
	}	
}

#endif


