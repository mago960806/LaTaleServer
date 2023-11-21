#include <string>
#include <algorithm>
#include <io.h>
#include <fcntl.h>
#include <vector>
#include <assert.h>
#include <string>

#ifdef _LATALE_CLIENT
	#include "SPCommon.h"
#elif _LATALE_SERVER
	#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
	#include <WTypes.h>
#endif
#include "SPResourceDef.h"


#ifdef PLAN_TOOL
	#include "stdafx.h"
#else 
	#include "SPMainGameDEF.h"
	#include "SPGameObjectDEF.h"
	#include "SPTerrainAttribute.h"
#endif

#include "SPUtil.h"
#include "SPFileDEF.H"

#include "SPStage.h"
#include "SPStageManager.h"

using namespace std;

#pragma warning(disable:4100)



//////////////////////////////////////////////////////////////////////////
//
int SPMapGroup::FindMapID(float fPointX /*= 0.0f*/,
						  float fPointY /*= 0.0f*/)
{
	int iMapID = 0;
	
	if(m_vpMapInfo.empty())
		return -1;

	int iListID = FindMap(fPointX, fPointY);
	if(iListID < 0 || iListID > static_cast<int>(m_vpMapInfo.size()) - 1 )
		return -1;
	
	vector< SPMapInfo* >::iterator iter = m_vpMapInfo.begin() + iListID;
	iMapID = (*iter)->iMapID;
	return iMapID;
	
	//// 백터인 경우
	//std::vector<SPMapInfo*>::iterator iterMap;
	//iterMap = m_vpMapInfo.begin();
	//int index = 0;
	//
	////map의 Link 방식에 따라 계산하는 방식이 달라져야 한다 
	//for(; iterMap != m_vpMapInfo.end(); ++iterMap, index++){
	//	if(((*iterMap)->fMapLeft <= fPointX && fPointX <= (*iterMap)->fMapRight)
	//		&& ((*iterMap)->fMapTop <= fPointY && fPointY <= (*iterMap)->fMapBottom))
	//	{
	//		return (*iterMap)->iMapID;
	//	}
	//}
	
	
	//// 배열인 경우
	//int index = 0;
	//for(index = 0 ; index < m_iMapCount ; index++) {
	//	if((m_pMapInfo[index].fMapLeft <= fPointX && fPointX <= m_pMapInfo[index].fMapRight)
	//		&& (m_pMapInfo[index].fMapTop <= fPointY && fPointY <= m_pMapInfo[index].fMapBottom))
	//	{
	//		return m_pMapInfo[index].iMapID;
	//	}
	//}
	
	
	return -1;
}


int SPMapGroup::GetFirstMapID()
{
	int iMapID = 0;

	if(m_vpMapInfo.empty())
		return -1;

	iMapID = m_vpMapInfo.at(0)->iMapID;

	return iMapID;
}


int SPMapGroup::GetGroupSize()
{
	int iSize = static_cast<int>(m_vpMapInfo.size());
	return iSize;
}


int SPMapGroup::FindMap(float fPointX /*= 0.0f*/,
						float fPointY /*= 0.0f*/)
{
	//// for문으로 검색하는 경우...
	//std::vector<SPMapInfo*>::iterator iterMap;
	//iterMap = m_vpMapInfo.begin();
	//int index = 0;
	//
	////map의 Link 방식에 따라 계산하는 방식이 달라져야 한다 
	//for(; iterMap != m_vpMapInfo.end(); ++iterMap, index++){
	//	if(((*iterMap)->fMapLeft <= fPointX && fPointX <= (*iterMap)->fMapRight)
	//		&& ((*iterMap)->fMapTop <= fPointY && fPointY <= (*iterMap)->fMapBottom))
	//	{
	//		return index;			
	//	}
	//}
	
	// for문 없이... 백터인 경우
	if(m_vpMapInfo.empty())
		return -1;
	
	vector< SPMapInfo* >::iterator iter = m_vpMapInfo.begin();
	SPMapInfo* pMap = (*iter);

	int iMapIndex = 0;
	if( MAP_LINK_NULL == m_iMapLink ) {
		return iMapIndex;
	}
	else if( MAP_LINK_HORIZONTAL == m_iMapLink ) {
		//SPMapInfo* pMap = m_vpMapInfo.at(0);
		iMapIndex = static_cast<int>(fPointX / pMap->fMapWidth);
		
		if(iMapIndex >= m_iMapCount){					//맵 인덱스가 실제 수보다 넘어가는 경우 강제 세팅
			return iMapIndex - 1;
		}	
		
		return iMapIndex;
		//return m_vpMapInfo[iMapIndex]->iMapID;
	}
	else if( MAP_LINK_VERTICAL == m_iMapLink ) {		//추후 다시 살펴볼것! 현재 대충 넣은 부분임
		//SPMapInfo* pMap = m_vpMapInfo.at(0);
		//int iMap = static_cast<int>(fPointY / pMap->fMapHeight);		
		//iMapIndex = (m_iMapCount - 1) - iMap;

		iMapIndex = static_cast<int>(fPointY / pMap->fMapHeight);
		
		if(iMapIndex >= m_iMapCount){					//맵 인덱스가 실제 수보다 넘어가는 경우 강제 세팅
			return iMapIndex - 1;
		}
		
		return iMapIndex;
		//return m_vpMapInfo[iMapIndex]->iMapID;
	}

	//// for문 없이... 배열인 경우
	//int iMapIndex = 0;
	//if( MAP_LINK_NULL == m_iMapLink ) {
	//	return iMapIndex;
	//}
	//else if( MAP_LINK_HORIZONTAL == m_iMapLink ) {
	//	//SPMapInfo* pMap = m_vpMapInfo.at(0);
	//	iMapIndex = static_cast<int>(fPointX / m_pMapInfo[0].fMapWidth);
	//	return iMapIndex;
	//}
	//else if( MAP_LINK_VERTICAL == m_iMapLink ) {		//추후 다시 살펴볼것! 현재 대충 넣은 부분임		
	//	int iMap = static_cast<int>(fPointY / m_pMapInfo[0].fMapHeight);
	//	iMapIndex = (m_iMapCount - 1) - iMap;
	//	return iMapIndex;
	//}
	
	return -1;
}

SPMapInfo* SPMapGroup::FindMapInfo(float fPosX, float fPosY)
{
	int iMapID = 0;
	
	if( m_vpMapInfo.empty() )
		return NULL;

	int iListID = FindMap(fPosX, fPosY);
	if(iListID < 0 || iListID > static_cast<int>(m_vpMapInfo.size()) - 1 )
		return NULL;
	
	vector< SPMapInfo* >::iterator iter = m_vpMapInfo.begin() + iListID;
	return (*iter);
}


float SPMapGroup::GetMapPosX(float fPointX, float fPointY, int iMapID /*= -1*/)
{
	float fResultX = 0.0f;
	
	//////////////////////////////////////////////////////////////////////////	
	// 백터인 경우
	if(iMapID < 0){
		//iMapID = FindMapID(fPointX, fPointY);			//여기서 이거 호출시 버그...
		// -1 인경우... 
		
		iMapID = FindMap(fPointX, fPointY);
	}
	
	if(iMapID < 0 || static_cast<int>(m_vpMapInfo.size()) - 1 < iMapID)		//[2005/4/29]
		return -1.0f;	
	
	vector< SPMapInfo* >::iterator iter = m_vpMapInfo.begin() + iMapID;
	SPMapInfo* pMap = (*iter);
	
	//map의 Link 방식에 따라 계산하는 방식이 달라져야 한다 
	if(fPointX < pMap->fMapLeft)	//FindMapID가 잘못됐다...
		return -1.0f;
	
	fResultX = fPointX - pMap->fMapLeft;
	
	//////////////////////////////////////////////////////////////////////////	
	//// 배열인 경우
	//if(iMapID < 0){
	//	//iMapID = FindMapID(fPointX, fPointY);			//여기서 이거 호출시 버그...
	//	// -1 인경우... 
	//	
	//	iMapID = FindMap(fPointX, fPointY);
	//}
	//
	////map의 Link 방식에 따라 계산하는 방식이 달라져야 한다 
	//if(fPointX < m_pMapInfo[iMapID].fMapLeft)	//FindMapID가 잘못됐다...
	//	return -1.0f;
	//
	//fResultX = fPointX - m_pMapInfo[iMapID].fMapLeft;

	return fResultX;
}


float SPMapGroup::GetMapPosY(float fPointX, float fPointY, int iMapID /*= -1*/)
{
	//return 781.f;
	
	float fResultY = 0.0f;
	
	//////////////////////////////////////////////////////////////////////////	
	// 백터인 경우
	if(iMapID < 0){
		//iMapID = FindMapID(fPointX, fPointY);		//여기서 이거 호출시 버그
		
		iMapID = FindMap(fPointX, fPointY);
	}
	
	if(iMapID < 0 || static_cast<int>(m_vpMapInfo.size()) - 1 < iMapID)		//[2005/4/29]
		return -1.0f;
	
	vector< SPMapInfo* >::iterator iter = m_vpMapInfo.begin() + iMapID;
	SPMapInfo* pMap = (*iter);
	
	//map의 Link 방식에 따라 계산하는 방식이 달라져야 한다 
	if(fPointY < pMap->fMapTop)
		return -1.0f;
	
	fResultY = fPointY - pMap->fMapTop;
	
	//////////////////////////////////////////////////////////////////////////	
	//// 배열인 경우
	//if(iMapID < 0){
	//	//iMapID = FindMapID(fPointX, fPointY);		//여기서 이거 호출시 버그
	//	
	//	iMapID = FindMap(fPointX, fPointY);
	//}
	//
	////map의 Link 방식에 따라 계산하는 방식이 달라져야 한다 
	//if(fPointY < m_pMapInfo[iMapID].fMapTop)
	//	return -1.0f;
	//
	//fResultY = fPointY - m_pMapInfo[iMapID].fMapTop;

	return fResultY;
}

void SPMapGroup::GetMapPos(float fPosX, float fPosY,
	float& fMapOffsetX, float& fMapOffsetY)
{
	fMapOffsetX = 0.0f;
	fMapOffsetY = 0.0f;

	int iMapID = FindMap(fPosX, fPosY);
	if(iMapID < 0 || static_cast<int>(m_vpMapInfo.size()) - 1 < iMapID)
		return;

	vector< SPMapInfo* >::iterator iter = m_vpMapInfo.begin() + iMapID;
	SPMapInfo* pMap = (*iter);

	fMapOffsetX = fPosX - pMap->fMapLeft;
	fMapOffsetY = fPosY - pMap->fMapTop;
}


float SPMapGroup::GetGroupPosX(int iMapID, float fPointX){
	if( MAP_LINK_NULL == m_iMapLink || MAP_LINK_VERTICAL == m_iMapLink) {
		return fPointX;
	}
	else if( MAP_LINK_HORIZONTAL == m_iMapLink ) {		
		float fPosX = 0.0f;

		if(m_vpMapInfo.empty())		return -1.0f;
		
		vector< SPMapInfo* >::iterator iter = m_vpMapInfo.begin();
		SPMapInfo* pMap = (*iter);
		fPosX =  pMap->fMapWidth * iMapID + fPointX;
		
		return fPosX;
	}

	return -1;
}


float SPMapGroup::GetGroupPosY(int iMapID, float fPointY){
	if( MAP_LINK_NULL == m_iMapLink || MAP_LINK_HORIZONTAL == m_iMapLink) {
		return fPointY;
	}
	else if(MAP_LINK_VERTICAL == m_iMapLink) {		
		float fPosY = 0.0f;

		if(m_vpMapInfo.empty())		return -1.0f;
		
		vector< SPMapInfo* >::iterator iter = m_vpMapInfo.begin();
		SPMapInfo* pMap = (*iter);
		fPosY =  pMap->fMapHeight * iMapID + fPointY;

		return fPosY;
	}

	return -1;
}


//////////////////////////////////////////////////////////////////////////
SPStage::SPStage() {
	Init();
}


SPStage::~SPStage() {
	ResetStage();
}


void SPStage::Init(){
	m_iStageID = -1;
	m_strStageName = "";	
	m_iGroupCount = 0;
	m_iSyncRegionWidth = 800;
	m_iSyncRegionHeight = 600;
	m_iCurMapGroupIndex = -1;
}


void SPStage::ResetStage(){
	std::vector<std::string>::iterator IterPalette;
	//for(IterPalette = m_vpPalette.begin(); IterPalette != m_vpPalette.end(); IterPalette++) {
	//	delete (*IterPalette);
	//}
	m_vpPalette.clear();

	ResetMapGroup();	

#ifdef _LATALE_SERVER
	std::vector<SPTerrainAttribute*>::iterator pos = m_vpTerrainAttribute.begin();
	while(pos != m_vpTerrainAttribute.end())
	{
		delete *pos;
		pos++;
	}
	m_vpTerrainAttribute.clear();
#endif
	//클라이언트에서의 해제는 
	//SPTerrainCluster::DeleteAllAttributeLayerList() 함수에서 해준다.
}


void SPStage::SetCurMapGroupIndex(int iGroupIndex /*= -1*/) {
	if(iGroupIndex < 0)
		return ;
	
	m_iCurMapGroupIndex = iGroupIndex;
}


int SPStage::GetCurMapGroupIndex() {
	if(m_vpMapGroup.empty())
		return -1;

	int iSize = static_cast<int>(m_vpMapGroup.size());
	if(m_iCurMapGroupIndex > iSize - 1)
		return -1;

	return m_iCurMapGroupIndex;
}


bool SPStage::AddPaletteFile(std::string strPalette /*= ""*/){
	m_vpPalette.push_back(strPalette.c_str());

	return true;
}


bool SPStage::AddMapGroup(SPMapGroup* pMapGroup /*= NULL*/) {
	if(0 == pMapGroup)			return false;
	
	m_vpMapGroup.push_back(pMapGroup);

	return true;
}


//좀더 생각해봐야...
SPTerrainAttribute* SPStage::GetCurTerrainAttribute(float fPointX, float fPointY,
	float& fMapOffsetX, float& fMapOffsetY, int iGroupID /*= -1*/, int iMapID /*= -1*/)
{
	//이 부분을조금 변경해 바로 체크 할수도 있을듯하다... 
	if(iGroupID < 0){		
		iGroupID = m_iCurMapGroupIndex;
	}

	fMapOffsetX = 0.0f;
	fMapOffsetY = 0.0f;

	SPMapGroup* pMapGroup;
	pMapGroup = GetMapGroup(iGroupID);
	if( pMapGroup == NULL )
		return NULL;

	SPMapInfo* pMapInfo = pMapGroup->FindMapInfo(fPointX, fPointY);
	if( pMapInfo == NULL )
		return NULL;

	iMapID = pMapInfo->iMapID;
	if( iMapID < 0 || ((static_cast<int>(m_vpTerrainAttribute.size()) - 1) < iMapID))
		return NULL;
	
	fMapOffsetX = fPointX - pMapInfo->fMapLeft;
	fMapOffsetY = fPointY - pMapInfo->fMapTop;

	vector< SPTerrainAttribute* >::iterator iter = m_vpTerrainAttribute.begin() + iMapID;
	return (*iter);
	//return m_vpTerrainAttribute[iMapID];
}


bool SPStage::ResetMapGroup(){
	std::vector<SPMapGroup*>::iterator IterMapGroup;
	IterMapGroup = m_vpMapGroup.begin();
	for(; IterMapGroup != m_vpMapGroup.end(); ++ IterMapGroup){
		SAFE_DELETE(*IterMapGroup);
	}
	m_vpMapGroup.clear();

	return true;
}


SPMapGroup* SPStage::GetMapGroup(int iGroupIndex /*= -1*/) {
	//if(iGroupIndex < 0){
	//	iGroupIndex = g_StageManager.GetCurGroupIndex();	//이게 좀 문제임...
	//	return NULL;
	//}		
	
	if(iGroupIndex < 0)
	{
		if(m_iCurMapGroupIndex > 0)
			iGroupIndex = m_iCurMapGroupIndex;	
		else return NULL;
	}	

	if( iGroupIndex > m_iGroupCount - 1 ) {
		return NULL;
	}

	if( m_vpMapGroup.empty() ) {
		return NULL;
	}
	
	vector< SPMapGroup* >::iterator iter = m_vpMapGroup.begin() + iGroupIndex;
	return (*iter);
	//return m_vpMapGroup[iGroupIndex];
}


