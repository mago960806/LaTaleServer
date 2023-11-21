
#include <algorithm>

#include "SPCommon.h"
//#include "SPQuestAttr.h"
//#include "SPQuestArchive.h"
//#include "SPQuestManager.h"
#include "SPEventDef.h"
//#include "SPEvent.h"
#include "SPEventManager.h"

#include "SPCheckManager.h"

#include "PacketID.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPGameObjectDEF.h"
#include "SPGameObject.h"
#include "SPGOBManager.h"

#include "SPWindowDEF.h"
#include "SPMouseCursor.h"
#include "SPInterfaceManager.h"

#include "SPCheckManager.h"

#include "SPUserShopAttr.h"
#include "SPUserShopArchive.h"

#include "SPDebug.h"


/**
* 주어진 ID의 QuestAttr 검사 <br>
*/
struct _IsEqualUserShopData : binary_function<SPUserShopAttr*, int, bool> {
	bool operator() (SPUserShopAttr* pUserShopAttr, int iUserShopType) const
	{
		if(pUserShopAttr->m_iType == iUserShopType)
			return true;
		return false;
	}
} IsEqualUserShopData;


SPUserShopArchive::SPUserShopArchive()
{
	LoadUserShopListLDT();
}


SPUserShopArchive::~SPUserShopArchive()
{
	DeleteAllUserShopInfo();
}


bool SPUserShopArchive::LoadUserShopListLDT()
{
	SPLDTFile* pLDTFile;
	g_pResourceManager->GetLDTFromFileName(RES_FNAME_LDT_USER_SHOP, &pLDTFile);	

	if(pLDTFile == NULL) {
		return false;
	}

	int iRecordCount = pLDTFile->GetItemCount();
	int iItemID = 0;

	LDT_Field ldtFieldType;	
	
	LDT_Field ldtFieldRequireType1;
	LDT_Field ldtFieldRequireID1;
	LDT_Field ldtFieldRequireVelueF1;
	LDT_Field ldtFieldRequireVelueS1;

	LDT_Field ldtFieldRequireType2;
	LDT_Field ldtFieldRequireID2;
	LDT_Field ldtFieldRequireVelueF2;
	LDT_Field ldtFieldRequireVelueS2;

	LDT_Field ldtFieldRequireType3;
	LDT_Field ldtFieldRequireID3;
	LDT_Field ldtFieldRequireVelueF3;
	LDT_Field ldtFieldRequireVelueS3;

	LDT_Field ldtFieldRequireType4;
	LDT_Field ldtFieldRequireID4;
	LDT_Field ldtFieldRequireVelueF4;
	LDT_Field ldtFieldRequireVelueS4;
	
	LDT_Field ldtFieldOverLapLeft;
	LDT_Field ldtFieldOverLapTop;
	LDT_Field ldtFieldOverLapRight;
	LDT_Field ldtFieldOverLapBottom;
	
	LDT_Field ldtFieldSlotPage;		

	LDT_Field ldtFieldImageFile1;
	LDT_Field ldtFieldSrcLeft1;
	LDT_Field ldtFieldSrcTop1;
	LDT_Field ldtFieldSrcRight1;
	LDT_Field ldtFieldSrcBottom1;
	LDT_Field ldtFieldDestX1;
	LDT_Field ldtFieldDestY1;
	LDT_Field ldtFieldNameDestX1;
	LDT_Field ldtFieldNameDestY1;
	LDT_Field ldtFieldTitleDestX1;
	LDT_Field ldtFieldTitleDestY1;
	LDT_Field ldtFieldDelay1;

	LDT_Field ldtFieldImageFile2;
	LDT_Field ldtFieldSrcLeft2;
	LDT_Field ldtFieldSrcTop2;
	LDT_Field ldtFieldSrcRight2;
	LDT_Field ldtFieldSrcBottom2;
	LDT_Field ldtFieldDestX2;
	LDT_Field ldtFieldDestY2;
	LDT_Field ldtFieldNameDestX2;
	LDT_Field ldtFieldNameDestY2;
	LDT_Field ldtFieldTitleDestX2;
	LDT_Field ldtFieldTitleDestY2;
	LDT_Field ldtFieldDelay2;

	LDT_Field ldtFieldImageFile3;
	LDT_Field ldtFieldSrcLeft3;
	LDT_Field ldtFieldSrcTop3;
	LDT_Field ldtFieldSrcRight3;
	LDT_Field ldtFieldSrcBottom3;
	LDT_Field ldtFieldDestX3;
	LDT_Field ldtFieldDestY3;
	LDT_Field ldtFieldNameDestX3;
	LDT_Field ldtFieldNameDestY3;
	LDT_Field ldtFieldTitleDestX3;
	LDT_Field ldtFieldTitleDestY3;
	LDT_Field ldtFieldDelay3;

	LDT_Field ldtFieldImageFile4;
	LDT_Field ldtFieldSrcLeft4;
	LDT_Field ldtFieldSrcTop4;
	LDT_Field ldtFieldSrcRight4;
	LDT_Field ldtFieldSrcBottom4;
	LDT_Field ldtFieldDestX4;
	LDT_Field ldtFieldDestY4;
	LDT_Field ldtFieldNameDestX4;
	LDT_Field ldtFieldNameDestY4;
	LDT_Field ldtFieldTitleDestX4;
	LDT_Field ldtFieldTitleDestY4;
	LDT_Field ldtFieldDelay4;
	
	LDT_Field ldtFieldNameWidth;
	LDT_Field ldtFieldNameHeight;
	LDT_Field ldtFieldTitleWidth;
	LDT_Field ldtFieldTitleHeight;
	
	LDT_Field ldtFieldNameRed;
	LDT_Field ldtFieldNameGreen;
	LDT_Field ldtFieldNameBlue;
	LDT_Field ldtFieldNameBlod;
	
	LDT_Field ldtFieldTitleRed;
	LDT_Field ldtFieldTitleGreen;
	LDT_Field ldtFieldTitleBlue;
	LDT_Field ldtFieldTitleBlod;

	LDT_Field ldtFieldMotion;

	for(int i = 0; i < iRecordCount; i++) {
		SPUserShopAttr* pShopAttr = new SPUserShopAttr;

		iItemID = pLDTFile->GetPrimaryKey( i );
		pShopAttr->m_iID = iItemID;

		pLDTFile->GetFieldFromLable( iItemID, "_Type",					ldtFieldType);		

		pShopAttr->m_iType = ldtFieldType.uData.lValue;		
		
		pLDTFile->GetFieldFromLable( iItemID, "_Require1_Type",			ldtFieldRequireType1);
		pLDTFile->GetFieldFromLable( iItemID, "_Require1_ID",			ldtFieldRequireID1);
		pLDTFile->GetFieldFromLable( iItemID, "_Require1_Value1",		ldtFieldRequireVelueF1);
		pLDTFile->GetFieldFromLable( iItemID, "_Require1_Value2",		ldtFieldRequireVelueS1);
		pShopAttr->m_Require[0].SetRequire(ldtFieldRequireType1.uData.lValue, ldtFieldRequireID1.uData.lValue,
			ldtFieldRequireVelueF1.uData.lValue, ldtFieldRequireVelueS1.uData.lValue);
		
		pLDTFile->GetFieldFromLable( iItemID, "_Require2_Type",			ldtFieldRequireType2);
		pLDTFile->GetFieldFromLable( iItemID, "_Require2_ID",			ldtFieldRequireID2);
		pLDTFile->GetFieldFromLable( iItemID, "_Require2_Value1",		ldtFieldRequireVelueF2);
		pLDTFile->GetFieldFromLable( iItemID, "_Require2_Value2",		ldtFieldRequireVelueS2);
		pShopAttr->m_Require[1].SetRequire(ldtFieldRequireType2.uData.lValue, ldtFieldRequireID2.uData.lValue,
			ldtFieldRequireVelueF2.uData.lValue, ldtFieldRequireVelueS2.uData.lValue);

		pLDTFile->GetFieldFromLable( iItemID, "_Require3_Type",			ldtFieldRequireType3);
		pLDTFile->GetFieldFromLable( iItemID, "_Require3_ID",			ldtFieldRequireID3);
		pLDTFile->GetFieldFromLable( iItemID, "_Require3_Value1",		ldtFieldRequireVelueF3);
		pLDTFile->GetFieldFromLable( iItemID, "_Require3_Value2",		ldtFieldRequireVelueS3);
		pShopAttr->m_Require[2].SetRequire(ldtFieldRequireType3.uData.lValue, ldtFieldRequireID3.uData.lValue,
			ldtFieldRequireVelueF3.uData.lValue, ldtFieldRequireVelueS3.uData.lValue);

		pLDTFile->GetFieldFromLable( iItemID, "_Require4_Type",			ldtFieldRequireType4);
		pLDTFile->GetFieldFromLable( iItemID, "_Require4_ID",			ldtFieldRequireID4);
		pLDTFile->GetFieldFromLable( iItemID, "_Require4_Value1",		ldtFieldRequireVelueF4);
		pLDTFile->GetFieldFromLable( iItemID, "_Require4_Value2",		ldtFieldRequireVelueS4);
		pShopAttr->m_Require[3].SetRequire(ldtFieldRequireType4.uData.lValue, ldtFieldRequireID4.uData.lValue,
			ldtFieldRequireVelueF4.uData.lValue, ldtFieldRequireVelueS4.uData.lValue);
		
		pLDTFile->GetFieldFromLable( iItemID, "_Require_Left",			ldtFieldOverLapLeft);
		pLDTFile->GetFieldFromLable( iItemID, "_Require_Top",			ldtFieldOverLapTop);
		pLDTFile->GetFieldFromLable( iItemID, "_Require_Right",			ldtFieldOverLapRight);
		pLDTFile->GetFieldFromLable( iItemID, "_Require_Bottom",		ldtFieldOverLapBottom);
		pShopAttr->SetOverLapRect(ldtFieldOverLapLeft.uData.lValue, ldtFieldOverLapTop.uData.lValue,
			ldtFieldOverLapRight.uData.lValue, ldtFieldOverLapBottom.uData.lValue);
		
		pLDTFile->GetFieldFromLable( iItemID, "_Slot_Count",			ldtFieldSlotPage);
		pShopAttr->m_iPage = ldtFieldSlotPage.uData.lValue;

		pLDTFile->GetFieldFromLable( iItemID, "_Source_File1",			ldtFieldImageFile1);
		pLDTFile->GetFieldFromLable( iItemID, "_Source_Left1",			ldtFieldSrcLeft1);
		pLDTFile->GetFieldFromLable( iItemID, "_Source_Top1",			ldtFieldSrcTop1);
		pLDTFile->GetFieldFromLable( iItemID, "_Source_Right1",			ldtFieldSrcRight1);
		pLDTFile->GetFieldFromLable( iItemID, "_Source_Bottom1",		ldtFieldSrcBottom1);
		pLDTFile->GetFieldFromLable( iItemID, "_Source_PosX1",			ldtFieldDestX1);
		pLDTFile->GetFieldFromLable( iItemID, "_Source_PosY1",			ldtFieldDestY1);
		pLDTFile->GetFieldFromLable( iItemID, "_Name_PosX1",			ldtFieldNameDestX1);
		pLDTFile->GetFieldFromLable( iItemID, "_Name_PosY1",			ldtFieldNameDestY1);
		pLDTFile->GetFieldFromLable( iItemID, "_Title_PosX1",			ldtFieldTitleDestX1);
		pLDTFile->GetFieldFromLable( iItemID, "_Title_PosY1",			ldtFieldTitleDestY1);
		pLDTFile->GetFieldFromLable( iItemID, "_Delay_Time1",			ldtFieldDelay1);
		if(ldtFieldImageFile1.uData.pValue) {
			pShopAttr->m_SignBoard[0].SetSrcFile(ldtFieldImageFile1.uData.pValue);
			if(!pShopAttr->m_SignBoard[0].m_strSource.empty() && pShopAttr->m_SignBoard[0].m_pTexture)
				pShopAttr->m_iMaxFrame = 1;
		}
		pShopAttr->m_SignBoard[0].SetSrcRect(ldtFieldSrcLeft1.uData.lValue, ldtFieldSrcTop1.uData.lValue,
			ldtFieldSrcRight1.uData.lValue, ldtFieldSrcBottom1.uData.lValue);
		pShopAttr->m_SignBoard[0].SetDest(ldtFieldDestX1.uData.lValue, ldtFieldDestY1.uData.lValue);
		pShopAttr->m_SignBoard[0].SetNameDeat(ldtFieldNameDestX1.uData.lValue, ldtFieldNameDestY1.uData.lValue);
		pShopAttr->m_SignBoard[0].SetTitleDest(ldtFieldTitleDestX1.uData.lValue, ldtFieldTitleDestY1.uData.lValue);
		pShopAttr->m_SignBoard[0].m_fDelay = (float)(ldtFieldDelay1.uData.lValue * 0.001f);

		pLDTFile->GetFieldFromLable( iItemID, "_Source_File2",			ldtFieldImageFile2);
		pLDTFile->GetFieldFromLable( iItemID, "_Source_Left2",			ldtFieldSrcLeft2);
		pLDTFile->GetFieldFromLable( iItemID, "_Source_Top2",			ldtFieldSrcTop2);
		pLDTFile->GetFieldFromLable( iItemID, "_Source_Right2",			ldtFieldSrcRight2);
		pLDTFile->GetFieldFromLable( iItemID, "_Source_Bottom2",		ldtFieldSrcBottom2);
		pLDTFile->GetFieldFromLable( iItemID, "_Source_PosX2",			ldtFieldDestX2);
		pLDTFile->GetFieldFromLable( iItemID, "_Source_PosY2",			ldtFieldDestY2);
		pLDTFile->GetFieldFromLable( iItemID, "_Name_PosX2",			ldtFieldNameDestX2);
		pLDTFile->GetFieldFromLable( iItemID, "_Name_PosY2",			ldtFieldNameDestY2);
		pLDTFile->GetFieldFromLable( iItemID, "_Title_PosX2",			ldtFieldTitleDestX2);
		pLDTFile->GetFieldFromLable( iItemID, "_Title_PosY2",			ldtFieldTitleDestY2);
		pLDTFile->GetFieldFromLable( iItemID, "_Delay_Time2",			ldtFieldDelay2);
		if(ldtFieldImageFile2.uData.pValue) {
			pShopAttr->m_SignBoard[1].SetSrcFile(ldtFieldImageFile2.uData.pValue);			
			if(!pShopAttr->m_SignBoard[1].m_strSource.empty() && pShopAttr->m_SignBoard[1].m_pTexture)
				pShopAttr->m_iMaxFrame = 2;
		}
		pShopAttr->m_SignBoard[1].SetSrcRect(ldtFieldSrcLeft2.uData.lValue, ldtFieldSrcTop2.uData.lValue,
			ldtFieldSrcRight2.uData.lValue, ldtFieldSrcBottom2.uData.lValue);
		pShopAttr->m_SignBoard[1].SetDest(ldtFieldDestX2.uData.lValue, ldtFieldDestY2.uData.lValue);
		pShopAttr->m_SignBoard[1].SetNameDeat(ldtFieldNameDestX2.uData.lValue, ldtFieldNameDestY2.uData.lValue);
		pShopAttr->m_SignBoard[1].SetTitleDest(ldtFieldTitleDestX2.uData.lValue, ldtFieldTitleDestY2.uData.lValue);
		pShopAttr->m_SignBoard[1].m_fDelay = (float)(ldtFieldDelay2.uData.lValue * 0.001f);

		pLDTFile->GetFieldFromLable( iItemID, "_Source_File3",			ldtFieldImageFile3);
		pLDTFile->GetFieldFromLable( iItemID, "_Source_Left3",			ldtFieldSrcLeft3);
		pLDTFile->GetFieldFromLable( iItemID, "_Source_Top3",			ldtFieldSrcTop3);
		pLDTFile->GetFieldFromLable( iItemID, "_Source_Right3",			ldtFieldSrcRight3);
		pLDTFile->GetFieldFromLable( iItemID, "_Source_Bottom3",		ldtFieldSrcBottom3);
		pLDTFile->GetFieldFromLable( iItemID, "_Source_PosX3",			ldtFieldDestX3);
		pLDTFile->GetFieldFromLable( iItemID, "_Source_PosY3",			ldtFieldDestY3);
		pLDTFile->GetFieldFromLable( iItemID, "_Name_PosX3",			ldtFieldNameDestX3);
		pLDTFile->GetFieldFromLable( iItemID, "_Name_PosY3",			ldtFieldNameDestY3);
		pLDTFile->GetFieldFromLable( iItemID, "_Title_PosX3",			ldtFieldTitleDestX3);
		pLDTFile->GetFieldFromLable( iItemID, "_Title_PosY3",			ldtFieldTitleDestY3);
		pLDTFile->GetFieldFromLable( iItemID, "_Delay_Time3",			ldtFieldDelay3);
		if(ldtFieldImageFile3.uData.pValue) {
			pShopAttr->m_SignBoard[2].SetSrcFile(ldtFieldImageFile3.uData.pValue);
			if(!pShopAttr->m_SignBoard[2].m_strSource.empty() && pShopAttr->m_SignBoard[2].m_pTexture)
				pShopAttr->m_iMaxFrame = 3;
		}
		pShopAttr->m_SignBoard[2].SetSrcRect(ldtFieldSrcLeft3.uData.lValue, ldtFieldSrcTop3.uData.lValue,
			ldtFieldSrcRight3.uData.lValue, ldtFieldSrcBottom3.uData.lValue);
		pShopAttr->m_SignBoard[2].SetDest(ldtFieldDestX3.uData.lValue, ldtFieldDestY3.uData.lValue);
		pShopAttr->m_SignBoard[2].SetNameDeat(ldtFieldNameDestX3.uData.lValue, ldtFieldNameDestY3.uData.lValue);
		pShopAttr->m_SignBoard[2].SetTitleDest(ldtFieldTitleDestX3.uData.lValue, ldtFieldTitleDestY3.uData.lValue);
		pShopAttr->m_SignBoard[2].m_fDelay = (float)(ldtFieldDelay3.uData.lValue * 0.001f);

		pLDTFile->GetFieldFromLable( iItemID, "_Source_File4",			ldtFieldImageFile4);
		pLDTFile->GetFieldFromLable( iItemID, "_Source_Left4",			ldtFieldSrcLeft4);
		pLDTFile->GetFieldFromLable( iItemID, "_Source_Top4",			ldtFieldSrcTop4);
		pLDTFile->GetFieldFromLable( iItemID, "_Source_Right4",			ldtFieldSrcRight4);
		pLDTFile->GetFieldFromLable( iItemID, "_Source_Bottom4",		ldtFieldSrcBottom4);
		pLDTFile->GetFieldFromLable( iItemID, "_Source_PosX4",			ldtFieldDestX4);
		pLDTFile->GetFieldFromLable( iItemID, "_Source_PosY4",			ldtFieldDestY4);
		pLDTFile->GetFieldFromLable( iItemID, "_Name_PosX4",			ldtFieldNameDestX4);
		pLDTFile->GetFieldFromLable( iItemID, "_Name_PosY4",			ldtFieldNameDestY4);
		pLDTFile->GetFieldFromLable( iItemID, "_Title_PosX4",			ldtFieldTitleDestX4);
		pLDTFile->GetFieldFromLable( iItemID, "_Title_PosY4",			ldtFieldTitleDestY4);
		pLDTFile->GetFieldFromLable( iItemID, "_Delay_Time4",			ldtFieldDelay4);
		if(ldtFieldImageFile4.uData.pValue) {
			pShopAttr->m_SignBoard[3].SetSrcFile(ldtFieldImageFile4.uData.pValue);
			if(!pShopAttr->m_SignBoard[3].m_strSource.empty() && pShopAttr->m_SignBoard[3].m_pTexture)
				pShopAttr->m_iMaxFrame = 4;
		}
		pShopAttr->m_SignBoard[3].SetSrcRect(ldtFieldSrcLeft4.uData.lValue, ldtFieldSrcTop4.uData.lValue,
			ldtFieldSrcRight4.uData.lValue, ldtFieldSrcBottom4.uData.lValue);
		pShopAttr->m_SignBoard[3].SetDest(ldtFieldDestX4.uData.lValue, ldtFieldDestY4.uData.lValue);
		pShopAttr->m_SignBoard[3].SetNameDeat(ldtFieldNameDestX4.uData.lValue, ldtFieldNameDestY4.uData.lValue);
		pShopAttr->m_SignBoard[3].SetTitleDest(ldtFieldTitleDestX4.uData.lValue, ldtFieldTitleDestY4.uData.lValue);
		pShopAttr->m_SignBoard[3].m_fDelay = (float)(ldtFieldDelay4.uData.lValue * 0.001f);

		pLDTFile->GetFieldFromLable( iItemID, "_Name_Width",			ldtFieldNameWidth);
		pLDTFile->GetFieldFromLable( iItemID, "_Name_Height",			ldtFieldNameHeight);
		pLDTFile->GetFieldFromLable( iItemID, "_Title_Width",			ldtFieldTitleWidth);
		pLDTFile->GetFieldFromLable( iItemID, "_Title_Height",			ldtFieldTitleHeight);
		pShopAttr->m_iNameWidth = ldtFieldNameWidth.uData.lValue;
		pShopAttr->m_iNameHeight = ldtFieldNameHeight.uData.lValue;
		pShopAttr->m_iTitleWidth = ldtFieldTitleWidth.uData.lValue;
		pShopAttr->m_iTitleHeight = ldtFieldTitleHeight.uData.lValue;
		
		pLDTFile->GetFieldFromLable( iItemID, "_Name_Red",				ldtFieldNameRed);
		pLDTFile->GetFieldFromLable( iItemID, "_Name_Green",			ldtFieldNameGreen);
		pLDTFile->GetFieldFromLable( iItemID, "_Name_Blue",				ldtFieldNameBlue);
		pLDTFile->GetFieldFromLable( iItemID, "_Name_Bold",				ldtFieldNameBlod);
		pShopAttr->SetNameColor(ldtFieldNameRed.uData.lValue, ldtFieldNameGreen.uData.lValue,
			ldtFieldNameBlue.uData.lValue);
		pShopAttr->m_bNameBold = static_cast<bool>(ldtFieldNameBlod.uData.lValue);
		
		pLDTFile->GetFieldFromLable( iItemID, "_Title_Red",				ldtFieldTitleRed);
		pLDTFile->GetFieldFromLable( iItemID, "_Title_Green",			ldtFieldTitleGreen);
		pLDTFile->GetFieldFromLable( iItemID, "_Title_Blue",			ldtFieldTitleBlue);		
		pLDTFile->GetFieldFromLable( iItemID, "_Title_Bold",			ldtFieldTitleBlod);
		pShopAttr->SetTitleColor(ldtFieldTitleRed.uData.lValue, ldtFieldTitleGreen.uData.lValue,
			ldtFieldTitleBlue.uData.lValue);
		pShopAttr->m_bTitleBold = static_cast<bool>(ldtFieldTitleBlod.uData.lValue);

		pLDTFile->GetFieldFromLable( iItemID, "_Motion",				ldtFieldMotion);
		pShopAttr->m_iMotion = ldtFieldMotion.uData.lValue;

		m_vpUserShopInfo.push_back(pShopAttr);
	}

	return true;
}


void SPUserShopArchive::DeleteAllUserShopInfo()
{
	std::vector<SPUserShopAttr*>::iterator iter;
	iter = m_vpUserShopInfo.begin();
	for(; iter != m_vpUserShopInfo.end(); ++iter) {
		delete (*iter);
	}
	m_vpUserShopInfo.clear();
}


SPUserShopAttr* SPUserShopArchive::GetUserShopData(int iIndex)
{
	if(iIndex < 0 || iIndex >= static_cast<int>(m_vpUserShopInfo.size()))
		return NULL;

	return m_vpUserShopInfo.at(iIndex);
}


SPUserShopAttr* SPUserShopArchive::FindUserShopData(int iType)
{
	std::vector<SPUserShopAttr*>::iterator iter;
	iter = std::find_if(m_vpUserShopInfo.begin(), m_vpUserShopInfo.end(),
		std::bind2nd(IsEqualUserShopData, iType));	

	if(iter == m_vpUserShopInfo.end())
		return NULL;
	else 
		return (*iter);

	return NULL;
}

int SPUserShopArchive::CheckPetShopClass( void )
{
	std::vector<SPUserShopAttr*>::iterator iter;	
	iter = m_vpUserShopInfo.begin();
	bool m_bCheck[MAX_USER_SHOP_REQUIRE];
	for(; iter != m_vpUserShopInfo.end(); ++iter)
	{
		for (int i = 0; i < MAX_USER_SHOP_REQUIRE; ++i)
		{
			m_bCheck[i] = false;
			if ((*iter)->m_Require[i].m_iRequireStat == 2)
			{
				m_bCheck[i] = true;
			}
			else
			{
				if( g_pCheckManager->CheckCondition((*iter)->m_Require[i].m_iRequireStat,
					(*iter)->m_Require[i].m_iRequireStatValue, 
					(*iter)->m_Require[i].m_iValue1,
					(*iter)->m_Require[i].m_iValue2) == false)
				{				
					continue;
				}
				else
				{
					m_bCheck[i] = true;
				}
			}
		}

		bool bOk = true;
		for(int i = 0; i < MAX_USER_SHOP_REQUIRE; i++) {
			bOk &= m_bCheck[i];
		}

		if(bOk) {
			return (*iter)->m_iType;
		}		
	}

	return 0;
}

int SPUserShopArchive::CheckUserShopClass()
{
		
	//std::vector<SPUserShopAttr*>::reverse_iterator iter;
	//iter = m_vpUserShopInfo.rbegin();
	std::vector<SPUserShopAttr*>::iterator iter;	
	iter = m_vpUserShopInfo.begin();
	bool m_bCheck[MAX_USER_SHOP_REQUIRE];
	for(; iter != m_vpUserShopInfo.end(); ++iter) {
		for(int i = 0; i < MAX_USER_SHOP_REQUIRE; i++) {
			m_bCheck[i] = false;
			
			if( g_pCheckManager->CheckCondition((*iter)->m_Require[i].m_iRequireStat, 
				(*iter)->m_Require[i].m_iRequireStatValue, 
				(*iter)->m_Require[i].m_iValue1,
				(*iter)->m_Require[i].m_iValue2) == false)
			{				
				continue;
			}
			else {
				m_bCheck[i] = true;
			}
		}		
		
		bool bOk = true;
		for(int i = 0; i < MAX_USER_SHOP_REQUIRE; i++) {
			 bOk &= m_bCheck[i];
		}
		
		if(bOk) {
			DXUTOutputDebugString("CheckUserShopClass [%d]\n", (*iter)->m_iType);
			return (*iter)->m_iType;
		}		
	}
	DXUTOutputDebugString("CheckUserShopClass [0]\n");
	return 0;
}