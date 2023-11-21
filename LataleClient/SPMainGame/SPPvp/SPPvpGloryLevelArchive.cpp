#include "SPCommon.h"
#include "SPMainGameDEF.H"

#include "SPGameObjectDEF.h"
#include "SPManagerDef.h"
#include "SPManager.h"
#include "SPGOBManager.h"
#include "SPSubGameManager.h"
#include "SPMainGameManager.h"

#include "Packet.h"
#include "PacketID.h"
#include "SPNetworkManager.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPPvpGloryLevelArchive.h"

#include "SPDebug.h"



SPPvpGloryLevelArchive::SPPvpGloryLevelArchive()
{
	LoadPvpGloryLevelAttr();
}


SPPvpGloryLevelArchive::~SPPvpGloryLevelArchive()
{
	DeleteAllLevelInfo();
}


bool SPPvpGloryLevelArchive::LoadPvpGloryLevelAttr()
{
	SPLDTFile* pLDTFile = NULL;
	g_pResourceManager->GetLDTFromFileName( RES_FNAME_LDT_PVPGLORYLEVEL , &pLDTFile);	
	if( pLDTFile == NULL )
	{
#ifdef _DEBUG
		DXUTOutputDebugString( "----------------------------------->PVP GLORY LEVEL LDT FILE LOAD FAIL [%s] \n" , RES_FNAME_LDT_PVPGLORYLEVEL );
#endif
		return false;
	}

	LDT_Field LDTField;
	int	iItemID = 0;
	int i = 0;
	int j = 0;

	int				iRecordCount	= pLDTFile->GetItemCount();
	SPPvpGloryLevelAttr* 	pGloryAttr = NULL;
	UINT64			iMinPoint = 0;

	for( i = 0; i < iRecordCount; ++i )
	{
		pGloryAttr = new SPPvpGloryLevelAttr;

		iItemID	=	pLDTFile->GetPrimaryKey( i );

		pGloryAttr->m_iID	=	iItemID;

		pGloryAttr->m_iMinPoint = iMinPoint;

		pLDTFile->GetField( iItemID , 0, LDTField );				//Max_Ppoint
		if(LDTField.uData.pValue) {
			pGloryAttr->m_iMaxPoint = _atoi64(LDTField.uData.pValue);
		}

		pLDTFile->GetField( iItemID , 1, LDTField );				//String
		if(LDTField.uData.pValue) {
			pGloryAttr->m_strLevel = LDTField.uData.pValue;
		}

		pLDTFile->GetField( iItemID , 2, LDTField );				//Icon
		pGloryAttr->m_iIconIndex = LDTField.uData.lValue;
		
		m_vpPvpGloryLevelInfo.push_back(pGloryAttr);

		iMinPoint = pGloryAttr->m_iMaxPoint;
	}

	SAFE_RELEASE(pLDTFile);
	return true;
}


void SPPvpGloryLevelArchive::DeleteAllLevelInfo()
{
	std::vector<SPPvpGloryLevelAttr*>::iterator iter = m_vpPvpGloryLevelInfo.begin();
	for(; iter != m_vpPvpGloryLevelInfo.end(); ++iter) {
		SAFE_DELETE((*iter));
	}

	m_vpPvpGloryLevelInfo.clear();
}


SPPvpGloryLevelAttr* SPPvpGloryLevelArchive::GetPvpGloryLevel(UINT64 iPvpPoint)
{
	return NULL;
}


SPPvpGloryLevelAttr* SPPvpGloryLevelArchive::GetPvpGloryLevel(int iPvpLevel)
{
	iPvpLevel -= 1;	
	
	int iSize = static_cast<int>(m_vpPvpGloryLevelInfo.size());
	if(iPvpLevel < 0 || iPvpLevel >  iSize - 1)
		return NULL;
	
	return m_vpPvpGloryLevelInfo.at(iPvpLevel);
}


std::string SPPvpGloryLevelArchive::GetPvpGloryLevelString(int iPvpLevel)
{
	SPPvpGloryLevelAttr* pPvpGloryLevelAttr = GetPvpGloryLevel(iPvpLevel);

	if(pPvpGloryLevelAttr) {
		return pPvpGloryLevelAttr->m_strLevel;
	}
	return "";
}
