


#ifdef _SP_CORE
#include "SPCoreDef.h"
#elif _LATALE_SERVER
#include <vector>
#include <WTypes.h>
#include "GlobalDefines.h"
#elif _LATALE_GMTOOL
#include <WTypes.h>
#include "GlobalDefines.h"
#endif

#include "SPResourceBaseDEF.h"
#include "SPUtil.h"
#include "SPStream.h"
#include "SPResourceBase.h"

bool g_bUsePackFile = false;

SPResourceBase* SPResourceBase::s_pResourceInstance = 0;
//CRITICAL_SECTION g_LoadStrSection;

SPResourceBase::SPResourceBase()
: m_iCount(0)
, m_bUsingResBase(false)
, m_iNoFileNameForReserve(0)
, m_iNoCurReadSPF(0)
{	
	//ZeroMemory(m_pPackStream, sizeof(m_pPackStream));
	//ZeroMemory(m_aiResVersion, sizeof(F_SPF_VERSION) * MAX_RESOURCE_FILE);
	//m_iNoCurReadSPF = 0;
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


	std::vector<SPStream*>::iterator vIter = m_vStreamRegisted.begin();
	for(; vIter != m_vStreamRegisted.end(); ++vIter) {
		SAFE_RELEASE((*vIter));
	}
	m_vStreamRegisted.clear();
	

	//for(int i = 0; i < MAX_RESOURCE_FILE; ++i) {
	//	SAFE_RELEASE(m_pPackStream[i]);
	//}

	m_vstrFileList.clear();

	//DeleteCriticalSection(&g_LoadStrSection);
}

SPResourceBase* SPResourceBase::GetInstance()
{
	return s_pResourceInstance;
}

SPResourceBase* SPResourceBase::Create()
{
	if( s_pResourceInstance == 0 ) {
		s_pResourceInstance = new SPResourceBase;		
	}
	return s_pResourceInstance;
}

void SPResourceBase::Release()
{
	SAFE_DELETE(s_pResourceInstance);
}

int SPResourceBase::GetNoSPF()
{
	return m_iNoCurReadSPF;
}

F_SPF_VERSION	SPResourceBase::GetSPFVersionByInx(const int iInx)
{
	if(iInx > m_iNoCurReadSPF ) {
		return -1;
	}

	std::map<int, F_SPF_VERSION>::iterator Iter = m_mSPFVerRegisted.find(iInx);
	if(Iter != m_mSPFVerRegisted.end()) {
		return (*Iter).second;
	}	
	return -1;
	//return m_aiResVersion[iInx];
}

const char* SPResourceBase::GetSPFNameByInx(const int iInx)
{
	if(iInx > m_iNoCurReadSPF ) {
		return NULL;
	}
	
	std::map<int, std::string>::iterator Iter = m_mResFileRegisted.find(iInx);
	if(Iter != m_mResFileRegisted.end()) {
		return ((*Iter).second).c_str();
	}	

	return NULL;
	//return (m_astrResFileName[iInx]).c_str();
}


void SPResourceBase::ReserveInitFile(const char* apszReserveFiles[], int iNoFile)
{
	std::string strReservFile;

	m_iNoFileNameForReserve = iNoFile;	
	m_vstrFileNameForReserve.clear();		
	
	for(int i = 0; i < m_iNoFileNameForReserve; ++i) {
		strReservFile = apszReserveFiles[i];
		m_vstrFileNameForReserve.push_back(strReservFile);
	}

	assert(m_vstrFileNameForReserve.size() == iNoFile);
}


bool SPResourceBase::Init(const char* pszBasePath)
{
	SetWorkPath(pszBasePath);

	string strResName;
	SPStream* pStream;


	for(int iRes = 0; iRes < m_iNoFileNameForReserve; ++iRes) {	
		
		pStream = NULL;

		

		strResName = m_vstrFileNameForReserve[iRes];
		if(strResName == "") {
			m_vStreamRegisted.push_back(NULL);
			continue;
		}

		pStream = new SPMemoryMappedFileStream(strResName.c_str(), SPFileStream::OPEN_READ);
		if(pStream->Valid() == false) {	
			SAFE_RELEASE(pStream);
			m_vStreamRegisted.push_back(NULL);
			continue;		
			//return false; 
		}	

		m_vStreamRegisted.push_back(pStream);

		

		F_SPF_VERSION	spfVer;
		F_SPF_HEADER	spfHeader;
		int				iCount;

		pStream->Seek( -sizeof(F_SPF_VERSION), SEEK_END );	
		pStream->Read( &spfVer, sizeof(int));

		++m_iNoCurReadSPF;

		//m_aiResVersion		[m_iNoCurReadSPF] = spfVer;		
		//m_astrResFileName	[m_iNoCurReadSPF] = strResName.c_str();

		m_mSPFVerRegisted.insert( std::map<int, F_SPF_VERSION>::value_type(m_iNoCurReadSPF, spfVer)  );
		m_mResFileRegisted.insert( std::map<int, std::string>::value_type(m_iNoCurReadSPF, strResName) );


		pStream->Seek( -(sizeof(F_SPF_VERSION) + sizeof(F_SPF_HEADER)), SEEK_END );		
		pStream->Read( &spfHeader, sizeof(F_SPF_HEADER));		

		pStream->Seek( -(sizeof(F_SPF_VERSION) + sizeof(F_SPF_HEADER) + spfHeader.iHeaderSize), SEEK_END );
		iCount = (spfHeader.iHeaderSize) / sizeof(FINFO);	
		m_iCount += iCount;

		FINFO*			pFileInfo;
		string			strInstanceName;
		for(int i = 1; i <= iCount; ++i) {

			strInstanceName.clear();
			pFileInfo = new FINFO;
			pStream->Read( pFileInfo, sizeof(FINFO));			

			//strInstanceName = GetUpperString2( pFileInfo->szFileName );
			strInstanceName = pFileInfo->szFileName;

			m_vstrFileList.push_back(strInstanceName);

			// force fail if already exist

			//hash_map<string, FINFO*>::iterator hmIterName = m_hmFInfoListName.find(strInstanceName);
			//if(hmIterName != m_hmFInfoListName.end() && (strInstanceName == (*hmIterName).first) ) {
			//	assert(0 && "Exist same data in packed file.");
			//	SAFE_DELETE ( pFileInfo );			
			//	continue;
			//} else {
			//	m_hmFInfoListName.insert ( hash_map<string, FINFO*>::value_type ( strInstanceName, pFileInfo));		
			//}

			//hash_map<RESID, FINFO*>::iterator hmIterNum = m_hmFInfoListIID.find(pFileInfo->ResID);
			//if(hmIterNum != m_hmFInfoListIID.end() && (pFileInfo->ResID == (*hmIterNum).first) ) {
			//	continue;
			//} else {
			//	m_hmFInfoListIID.insert ( hash_map<RESID, FINFO*>::value_type ( pFileInfo->ResID, pFileInfo));		
			//}


			// overwrite if already exist

			hash_map<string, FINFO*>::iterator hmIterName = m_hmFInfoListName.find(strInstanceName);
			if(hmIterName != m_hmFInfoListName.end() && (strInstanceName == (*hmIterName).first) ) {
				SAFE_DELETE((*hmIterName).second);
				(*hmIterName).second = pFileInfo;
			} else {
				m_hmFInfoListName.insert ( hash_map<string, FINFO*>::value_type ( strInstanceName, pFileInfo));
			}

			hash_map<RESID, FINFO*>::iterator hmIterNum = m_hmFInfoListIID.find(pFileInfo->ResID);
			if(hmIterNum != m_hmFInfoListIID.end() && (pFileInfo->ResID == (*hmIterNum).first) ) {
				SAFE_DELETE((*hmIterNum).second);
				(*hmIterNum).second = pFileInfo;
			} else {
				m_hmFInfoListIID.insert ( hash_map<RESID, FINFO*>::value_type ( pFileInfo->ResID, pFileInfo));
			}

		}
	}

	if(g_bUsePackFile)
		m_bUsingResBase = true;

	//InitializeCriticalSection(&g_LoadStrSection);

	return true;
}

bool SPResourceBase::FindPackFirst(IN const char* pszPath, OUT char* pszFileNameWithPath,IN char* pszExt)
{	
	int iLen;

	if(pszPath == NULL) {
		iLen= 0;
		m_strFindPath.clear();
	} else {
		iLen = strlen(pszPath);	
		GetUpperString3(m_strFindPath, pszPath);
	}

	vector<string>::iterator Iter = m_vstrFileList.begin();
	m_strFindExt.clear();
	if(pszExt) {
		GetUpperString3(m_strFindExt, pszExt);		
	}

	for (m_iFindInx = 0; Iter != m_vstrFileList.end(); ++Iter, ++m_iFindInx) {
		if(strncmp( (*Iter).c_str() , m_strFindPath.c_str(), iLen) == 0) { // 패스비교한다
			if(m_strFindExt.empty() == false) { // 입력된 확장자가 있는가?
				// 비교 확장자가 존재하면 확장자를 비교한다
				if( m_strFindExt != (*Iter).substr((*Iter).length()-m_strFindExt.length(), m_strFindExt.length())) { 
					continue;				
				}
			}
			strcpy(pszFileNameWithPath, (*Iter).c_str());
			return true;
		}
	}
	m_iFindInx = -1;
	return false;
}


bool SPResourceBase::FindPackNext(OUT char* pszFileNameWithPath)
{	
	if(m_iFindInx == -1) {
		assert(0 || "Invalid Find Index");
		return false;
	}
	if(m_iFindInx >= m_vstrFileList.size()) {
		assert(0 || "Invalid Find Index");
		m_iFindInx = -1;
		return false;
	}

	string strExt; 
	int iLen = m_strFindPath.length();;	
	vector<string>::iterator Iter = m_vstrFileList.begin() + m_iFindInx + 1;
	for (; Iter != m_vstrFileList.end(); ++Iter, ++m_iFindInx) {
		if(strncmp( (*Iter).c_str() , m_strFindPath.c_str(), iLen) == 0) {
			if(m_strFindExt.empty() == false) {
				// 비교 확장자가 존재하면 확장자를 비교한다
				if( m_strFindExt != (*Iter).substr((*Iter).length()-m_strFindExt.length(), m_strFindExt.length())	) {
					continue;
				}
			}
			++m_iFindInx;
			strcpy(pszFileNameWithPath, (*Iter).c_str());
			return true;
		}
	}

	m_iFindInx = -1;
	return false;	
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
	//string strInstanceName = GetUpperString(pszFilename);
	string strInstanceName = pszFilename;
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
	if(pszFilename == NULL) {
		*ppStream = NULL;
		return false;
	}

	string strFName;
	strFName = pszFilename;
	if(strFName == "") {
		*ppStream = NULL;
		return false;
	}

	strFName.clear();


	FINFO* pFInfo = NULL;
	if(SPResourceBase::GetInstance()->IsManaged()) {

		//string strFName = GetUpperString2(pszFilename);

	
		GetUpperString3(strFName, pszFilename);

		pFInfo = FilenameToIndex( strFName.c_str() );
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

	if(iInstanceID == 0) {
		(*ppStream) = NULL;	
		return false;
	}


	//EnterCriticalSection(&g_LoadStrSection);

	// FIRST: Search stream data
	hash_map<RESID, F_READSTREAM*>::const_iterator IterStream = m_hmStreamList.find(iInstanceID);
	if(IterStream != m_hmStreamList.end() && (iInstanceID == (*IterStream).first) ) {		
		pReadStream = (*IterStream).second;

		++(pReadStream->iRefCount);
		pStreamCreate = new SPManagedStream( pReadStream->pBuffer, pReadStream->iSize, iInstanceID );
		(*ppStream) = pStreamCreate;

		//LeaveCriticalSection(&g_LoadStrSection);

		return true;

	} else {		
		// SECOND: if not find stream data, search file info and create new readstream to stream pool.
		hash_map<RESID, FINFO*>::iterator IterFileInfo = m_hmFInfoListIID.find(iInstanceID);
		if(IterFileInfo != m_hmFInfoListIID.end() && (iInstanceID == (*IterFileInfo).first) ) {
			FINFO* pReadInx = (*IterFileInfo).second;
			

			//void* pBuff = new BYTE( pReadInx->FileInfo.iSize + 1);
			void* pBuff = GlobalAlloc(GMEM_FIXED, pReadInx->iSize + 1);
			if(pBuff == NULL) {
				(*ppStream) = NULL;	

				//LeaveCriticalSection(&g_LoadStrSection);


				return false;
			}

			RES_FILE_ID	iFileID = GetResID2FileID (iInstanceID);

			assert( m_vStreamRegisted.size() > iFileID);
			//assert( m_vStreamRegisted.begin() + iFileID != m_vStreamRegisted.end());
			if(m_vStreamRegisted.size() <= iFileID) {
				(*ppStream) = NULL;	

				//LeaveCriticalSection(&g_LoadStrSection);

				return false;
			}

			SPStream* pStream;
			pStream = m_vStreamRegisted[iFileID];


			int iRet = pStream->Seek( pReadInx->iOffset, SEEK_SET );			
			pStream->Read( pBuff, pReadInx->iSize );						

			pReadStream = new F_READSTREAM;
			pReadStream->pBuffer		= (BYTE*)pBuff;
			pReadStream->iSize			= pReadInx->iSize;
			//pReadStream->iInstanceID	= iInstanceID;			
			pReadStream->iRefCount		= 1;

			pStreamCreate = new SPManagedStream( pReadStream->pBuffer, pReadStream->iSize, iInstanceID );
			(*ppStream) = pStreamCreate;
			m_hmStreamList.insert ( hash_map<RESID, F_READSTREAM*>::value_type ( iInstanceID, pReadStream) );

			//LeaveCriticalSection(&g_LoadStrSection);

            return true;
		}
	}

	//LeaveCriticalSection(&g_LoadStrSection);

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




