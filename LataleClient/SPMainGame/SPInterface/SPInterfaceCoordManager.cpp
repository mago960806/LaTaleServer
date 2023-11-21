
#include <map>
#include "SPCommon.h"

#include "SPStream.h"

#include "SPWindowDEF.h"
#include "SPWindow.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "SPInterfaceCoordManager.h"

const char* INTERFACE_POS_FILE = "INTERFACEPOS.DAT";
const int	INTERFACE_POS_VER = 1;

SPInterfaceCoordManager* SPInterfaceCoordManager::m_pInstance = NULL;


POINT* MakePT(int x, int y) 
{
	POINT* pPT = new POINT;	
	pPT->x = x;
	pPT->y = y;
	return pPT;
}


SPInterfaceCoordManager* SPInterfaceCoordManager::GetInstance()
{
	if(m_pInstance == NULL)
		m_pInstance = new SPInterfaceCoordManager;

	return m_pInstance;
}

void SPInterfaceCoordManager::DelInstance()
{
	SAFE_DELETE(m_pInstance);
}

SPInterfaceCoordManager::SPInterfaceCoordManager()
{
	//Init();
}

SPInterfaceCoordManager::~SPInterfaceCoordManager()
{
	Save();

	std::map<INSTANCE_ID, POINT*>::iterator mIter = m_mStartPoint.begin();	
	for(; mIter != m_mStartPoint.end() ; ++mIter) {
		SAFE_DELETE( ((*mIter).second) );
	}
	m_mStartPoint.clear();
}

void SPInterfaceCoordManager::Init()
{
	SetDeault();
	Load();
}

void SPInterfaceCoordManager::SetDeault()
{
	m_mStartPoint.insert(std::map<INSTANCE_ID, POINT*>::value_type (WIID_USERSHOP_SELL		, MakePT(149, 125)));
	m_mStartPoint.insert(std::map<INSTANCE_ID, POINT*>::value_type (WIID_USERSHOP_BUY		, MakePT(149, 125)));
	m_mStartPoint.insert(std::map<INSTANCE_ID, POINT*>::value_type (WIID_STATUS				, MakePT(9,   98 )));
	m_mStartPoint.insert(std::map<INSTANCE_ID, POINT*>::value_type (WIID_ITEM				, MakePT(101, 133)));
	m_mStartPoint.insert(std::map<INSTANCE_ID, POINT*>::value_type (WIID_EQUIP				, MakePT(299, 133)));
	m_mStartPoint.insert(std::map<INSTANCE_ID, POINT*>::value_type (WIID_TEAM				, MakePT(141, 85 )));
	m_mStartPoint.insert(std::map<INSTANCE_ID, POINT*>::value_type (WIID_FRIEND				, MakePT(370, 85 )));
	m_mStartPoint.insert(std::map<INSTANCE_ID, POINT*>::value_type (WIID_OPTION				, MakePT(254, 138)));
	m_mStartPoint.insert(std::map<INSTANCE_ID, POINT*>::value_type (WIID_LOOTING			, MakePT(16,  207)));
	m_mStartPoint.insert(std::map<INSTANCE_ID, POINT*>::value_type (WIID_PC_INFO			, MakePT(279, 166)));
	m_mStartPoint.insert(std::map<INSTANCE_ID, POINT*>::value_type (WIID_SKILL				, MakePT(256, 111)));
	m_mStartPoint.insert(std::map<INSTANCE_ID, POINT*>::value_type (WIID_QUEST				, MakePT(59,  70 )));
	m_mStartPoint.insert(std::map<INSTANCE_ID, POINT*>::value_type (WIID_QUESTVIEW			, MakePT(306, 70 )));
	m_mStartPoint.insert(std::map<INSTANCE_ID, POINT*>::value_type (WIID_HELP				, MakePT(144, 119)));
	m_mStartPoint.insert(std::map<INSTANCE_ID, POINT*>::value_type (WIID_EXIT				, MakePT(333, 247)));
	m_mStartPoint.insert(std::map<INSTANCE_ID, POINT*>::value_type (WIID_NOTICEBOX			, MakePT(273, 230)));
	m_mStartPoint.insert(std::map<INSTANCE_ID, POINT*>::value_type (WIID_PC_TRADE			, MakePT(202, 154)));
	m_mStartPoint.insert(std::map<INSTANCE_ID, POINT*>::value_type (WIID_CHANNEL_CHANGE		, MakePT(296, 165)));
	m_mStartPoint.insert(std::map<INSTANCE_ID, POINT*>::value_type (WIID_WAYPOINT			, MakePT(149, 125)));
	m_mStartPoint.insert(std::map<INSTANCE_ID, POINT*>::value_type (WIID_PET				, MakePT(149, 125)));
}


void SPInterfaceCoordManager::Load()
{
	char FileName[128];
	std::string strFile = INTERFACE_POS_FILE;
	int iVer;

	SPStream* pFileStream = new SPFileStream(INTERFACE_POS_FILE, SPFileStream::OPEN_READ);
	if(pFileStream->Valid() == false) {
		pFileStream->Release();
		return;
	}

	pFileStream->Read( FileName, strlen(INTERFACE_POS_FILE));
	FileName[strlen(INTERFACE_POS_FILE)] = 0;
	if(strFile != FileName) {
		pFileStream->Release();
		return;
	}

	pFileStream->Read( &iVer, sizeof(int));
	if(iVer != INTERFACE_POS_VER) {
		pFileStream->Release();
		return;
	}

	int iNo;
	INSTANCE_ID wiid;
	POINT pt;

	std::map<INSTANCE_ID, POINT*>::iterator mIter;	
	pFileStream->Read( &iNo, sizeof(int));
	if(iNo > m_mStartPoint.size()) {
		pFileStream->Release();
		return;
	}

	for(int  i= 0; i < iNo; ++i) {
		pFileStream->Read( &wiid, sizeof(INSTANCE_ID));
		pFileStream->Read( &pt, sizeof(POINT));		

		mIter = m_mStartPoint.find( wiid );
		if( mIter != m_mStartPoint.end()) {
			(*mIter).second->x = pt.x;
			(*mIter).second->y = pt.y;
		}
	}
	pFileStream->Release();
}

void SPInterfaceCoordManager::Save()
{
	SPStream* pFileStream = new SPFileStream(INTERFACE_POS_FILE, SPFileStream::OPEN_WRITE);
	if(pFileStream->Valid() == false) {
		pFileStream->Release();
		return;
	}
	pFileStream->Write( INTERFACE_POS_FILE, strlen(INTERFACE_POS_FILE));
	pFileStream->Write( &INTERFACE_POS_VER, sizeof(int));

	int iSize = m_mStartPoint.size();
	pFileStream->Write(&iSize , sizeof(int));

	std::map<INSTANCE_ID, POINT*>::iterator mIter = m_mStartPoint.begin();	
	for(; mIter != m_mStartPoint.end() ; ++mIter) {		
		pFileStream->Write( &((*mIter).first), sizeof(INSTANCE_ID));
		pFileStream->Write( ((*mIter).second), sizeof(POINT));		
	}
	pFileStream->Release();
}

bool SPInterfaceCoordManager::GetWindowPT(IN INSTANCE_ID wiid, OUT POINT& pt)
{
	std::map<INSTANCE_ID, POINT*>::iterator mIter = m_mStartPoint.find(wiid);
	if( mIter != m_mStartPoint.end()) {
		pt.x = (*mIter).second->x;
		pt.y = (*mIter).second->y;
		return true;
	} 

	pt.x = 0;
	pt.y = 0;
	return false;
}

bool SPInterfaceCoordManager::SetWindowPT(IN INSTANCE_ID wiid, IN POINT pt)
{
	std::map<INSTANCE_ID, POINT*>::iterator mIter = m_mStartPoint.find(wiid);
	if( mIter != m_mStartPoint.end()) {
		(*mIter).second->x = pt.x;
		(*mIter).second->y = pt.y;
		return true;
	} 
	return false;
}




