
#include <assert.h>
#include <string>

// 서버
#ifdef _LATALE_SERVER
	#include <WTypes.h>
	#include "SPResourceDef.h"	// 화일명때문에..
	//TODO : Server Side
	extern char* g_pszTerrainAttributeFile;
#endif

// 클라이언트
#ifdef _LATALE_CLIENT	
	#include "SPCommon.h"	
	#include "SPResourceDef.h"	// 화일명때문에..
	//TODO : Client Side
	#include "SPDebug.h"
#endif

#ifdef _MAP_TOOL
	#include "SPCommon.h"	
	#include "SPResourceDef.h"	// 화일명때문에..
#endif

#ifndef _LATALE_SERVER
	#include "dxutil.h"
#else
	#include "dxUtil/dxutil.h"
#endif


//#include "SPUtil.h"

#include "SPGameObjectDEF.h"
#include "SPTerrainAttributeLoader.h"
#include "SPTerrainAttribute.h"

//SPTerrainAttribute* g_pTerrainAttribute = NULL;

SPTerrainAttribute::SPTerrainAttribute()
{
	for( int i = 0; i < MAX_ATTR_LAYER; i++ )
		m_pkAttribute[i] = NULL;

	m_iHalfTileX = TILE_WIDTH / 2;
	m_iHalfTileY = TILE_HEIGHT / 2;

	m_iQuadTileX = TILE_WIDTH / 4;
	m_iQuadTileY = TILE_HEIGHT / 4;
}


SPTerrainAttribute::~SPTerrainAttribute()
{
	Clean();
}


void SPTerrainAttribute::Clean()
{
	for( int i = 0; i < MAX_ATTR_LAYER; i++ )
	{
		SAFE_DELETE_ARRAY(m_pkAttribute[i]);
	}
	
	//g_pTerrainAttribute = NULL;
}


long SPTerrainAttribute::Create(int iGridWidth, int iGridHeight)
{
	Clean();
	int i;

	for( i = 0; i < MAX_ATTR_LAYER; i++ )
	{
		m_pkAttribute[i] = new BYTE[iGridWidth * iGridHeight];	
		ZeroMemory(m_pkAttribute[i], iGridWidth * iGridHeight);

		assert(0 != m_pkAttribute[i] && "m_pAttr CreateFail..\n");
	}

	SetLength(iGridWidth * 32.f, iGridHeight * 16.f);

	//g_pTerrainAttribute = this;
	
	return iGridWidth * iGridHeight;
}


bool SPTerrainAttribute::Load(std::string strPath /*= ""*/,
							  std::string strFile /*= ""*/,
							  long lGroupWidth /*= 0*/,
							  long lGroupHeight /*= 0*/)
{
	//g_pTerrainAttribute = this;
	
	SPTerrainAttributeLoader	AttrLoader;	
	int iSize;

	//strPath = GetUpperString2(strPath.c_str());
	//strFile = GetUpperString2(strFile.c_str());

#ifdef _MAP_TOOL
	for( int i = 0; i < MAX_ATTR_LAYER; i++ )
		m_pkAttribute[i] = NULL;

	bool bRet = AttrLoader.LoadDataText(strFile.c_str(), 
		&m_pkAttribute[0],	&m_pkAttribute[1], &m_pkAttribute[2],
		iSize, lGroupWidth, lGroupHeight, strPath.c_str());
#else	
	bool bRet = AttrLoader.LoadBinary(strFile.c_str(), 
		&m_pkAttribute[0],	&m_pkAttribute[1], &m_pkAttribute[2],
		iSize, lGroupWidth, lGroupHeight, strPath.c_str());	
#endif
	
	assert(bRet && "지역 애트리뷰트 로딩 실패");
	
	m_iLenX = AttrLoader.GetCXTerrain();
	m_iLenY = AttrLoader.GetCYTerrain();

	//this->SetLength(AttrLoader.GetCXTerrain(), AttrLoader.GetCYTerrain());
	
	return bRet;
}


bool SPTerrainAttribute::Init()
{
	//g_pTerrainAttribute = this;
	return true;

//	std::string strResource;
//	bool	bRet;
//	SPTerrainAttributeLoader	AttrLoader;
//
//	int iSize;
//	// 서버
//#ifdef _LATALE_SERVER
//	bRet = AttrLoader.LoadDataText( RES_FNAME_TERRAINATTRIBUTE, &m_pAttr1, &m_pAttr2, &m_pAttr3, iSize);
//#else 
//	bRet = AttrLoader.LoadDataText( RES_FNAME_TERRAINATTRIBUTE, &m_pAttr1, &m_pAttr2, &m_pAttr3, iSize);
//#endif	
//	assert(bRet && "지역 애트리뷰트 로딩 실패");		
//	SetLength((float)AttrLoader.GetCXTerrain(), (float)AttrLoader.GetCYTerrain()); //jinhee - 04.11.25
//	
//	return true;

}

long SPTerrainAttribute::SetAttr1(BYTE bAttr /*= 0*/, float fX /*= 0.0f*/, float fY /*= 0.0f*/){
	int iIndexX = (int)ATTR_PIXEL_TO_OFFSET_X(fX);
	int iIndexY = (int)ATTR_PIXEL_TO_OFFSET_Y(fY);
	int iTLenX = (int)(m_iLenX / 32);
	int iTLenY = (int)(m_iLenY / 16);
	long lMaxSize = iTLenX * iTLenY;
	
	long lIndex = (iIndexY * iTLenX) + iIndexX;
	
	if(0 == m_pkAttribute[0] || lIndex > lMaxSize - 1)
		return -1;
	
	m_pkAttribute[0][lIndex] = bAttr;
	return lIndex;
}


long SPTerrainAttribute::SetAttr2(BYTE bAttr /*= 0*/, float fX /*= 0.0f*/, float fY /*= 0.0f*/){
	int iIndexX = (int)ATTR_PIXEL_TO_OFFSET_X(fX);
	int iIndexY = (int)ATTR_PIXEL_TO_OFFSET_Y(fY);
	int iTLenX = (int)(m_iLenX / 32);
	int iTLenY = (int)(m_iLenY / 16);
	long lMaxSize = iTLenX * iTLenY;
	
	long lIndex = (iIndexY * iTLenX) + iIndexX;
	
	if(0 == m_pkAttribute[1] || lIndex > lMaxSize - 1)
		return -1;
	
	m_pkAttribute[1][lIndex] = bAttr;
	return lIndex;
}


long SPTerrainAttribute::SetAttr3(BYTE bAttr /*= 0*/, float fX /*= 0.0f*/, float fY /*= 0.0f*/){
	int iIndexX = (int)ATTR_PIXEL_TO_OFFSET_X(fX);
	int iIndexY = (int)ATTR_PIXEL_TO_OFFSET_Y(fY);
	int iTLenX = (int)(m_iLenX / 32);
	int iTLenY = (int)(m_iLenY / 16);
	long lMaxSize = iTLenX * iTLenY;
	
	long lIndex = (iIndexY * iTLenX) + iIndexX;	

	if(0 == m_pkAttribute[2] || lIndex > lMaxSize - 1)
		return -1;

	m_pkAttribute[2][lIndex] = bAttr;
	return lIndex;
}


// 오프셋 좌표를 이용하여 애트리뷰트유닛을 검색하여 리턴한다.
// 레이어가 0번일경우 모든레이어 검색을 하고 검색된 레이어를 리턴한다
// 레이어 우선순위는 1번 (모든레이어가 다 존재할경우 1번을 리턴한다)
bool SPTerrainAttribute::GetAttrType(int iOffsetX, int iOffsetY, BYTE& cAttr, int& iLayer)
{	
	cAttr	= 0;

	float fX = (float)ATTR_OFFSET_TO_PIXEL_X(iOffsetX);
	float fY = (float)ATTR_OFFSET_TO_PIXEL_Y(iOffsetY);

	if( fX < 0.0f || fX >= m_iLenX )
		return false;

	if( fY < 0.0f || fY >= m_iLenY )
		return false;

	int iLenX = (int)ATTR_PIXEL_TO_OFFSET_X(m_iLenX);
	//int iLenY = (int)ATTR_PIXEL_TO_OFFSET_Y(m_fLenY);

	bool bRetVal = false;
	BYTE cAttrLayerValue;
	int i;
	if( iLayer == 0 )
	{
		for( i = MAX_ATTR_LAYER - 1; i >= 0; i-- )
		{
			if( m_pkAttribute[i] == NULL )
				continue;

			cAttrLayerValue = m_pkAttribute[i][iOffsetX + (iOffsetY * iLenX)];
			if( cAttrLayerValue != 0 )
			{
				cAttr = cAttrLayerValue;
				iLayer = i + 1;
			}
		}
		bRetVal = true;
	}
	else if( iLayer > 0 && iLayer <= MAX_ATTR_LAYER )
	{
		cAttr = m_pkAttribute[iLayer - 1][iOffsetX + (iOffsetY * iLenX)];
		bRetVal = true;
	}
	
	return bRetVal;
}

BYTE SPTerrainAttribute::GetAttrLayer(int iLayer, float fX, float fY)
{
	if( iLayer < 0 || iLayer >= MAX_ATTR_LAYER )
		return 0xFF;
	
	if( m_pkAttribute[iLayer] == NULL )
		return 0xFF;

	if(fX > m_iLenX || fY > m_iLenY )
		return 0xFF;

	int iTX = fX;
	int iTY = fY;
	int iTLenX = (m_iLenX/32);	

	iTX = iTX / 32;						//jinhee [2004/12/10]		
	iTY = iTY / 16;	
	
	int index = iTX + (iTLenX * iTY);

	BYTE bAttr = m_pkAttribute[iLayer][index];
	return bAttr;
}

BYTE SPTerrainAttribute::GetOffSetAttrLayer(int iLayer, int iOffSetX, int iOffSetY)
{
	if( iLayer < 0 || iLayer >= MAX_ATTR_LAYER )
		return 0xFF;
	
	if( m_pkAttribute[iLayer] == NULL )
		return 0xFF;

	int iTLenX = (m_iLenX/32);	
	int iTLenY = (m_iLenY/16);

	if(iOffSetX > iTLenX || iOffSetX < 0 || iOffSetY > iTLenY || iOffSetY < 0)
		return 0xFF;

	BYTE bAttr;
	int index = iOffSetX + (iTLenX * iOffSetY);
	bAttr = m_pkAttribute[iLayer][index];

	return bAttr;
}



bool SPTerrainAttribute::MoveDrop(float fX, float fY, BG_ID eLayer)
{
	if( eLayer == BG_ID_LAYER_TERRAIN0 ||
		eLayer == BG_ID_LAYER_TERRAIN1 ||
		eLayer == BG_ID_LAYER_TERRAIN2)
		return false;

	BYTE cAttr;
	int i;
	for( i = 0; i < MAX_ATTR_LAYER; i++ )
	{
		cAttr = GetAttr(i, fX, fY);
		if( IsGround(cAttr, fX, fY) )
			return false;
	}

	return true;
}

char SPTerrainAttribute::InspectStandLayer(int iX, int iY,
	BG_ID& eLayer, int& iRevision)
{
	BYTE cAttr = 0;
	int i;
	bool bReCalculate;
	
	iRevision = 0;
	for( i = 0; i < MAX_ATTR_LAYER; i++ )
	{
		cAttr = CheckStandLayer(i, iX, iY, iRevision, bReCalculate);
		if( cAttr > 0 )
		{
			if( bReCalculate )
			{
				cAttr = CheckStandLayer(i, iX, iY + TILE_HEIGHT, iRevision, bReCalculate);
				if( cAttr > 0 )
					iRevision += TILE_HEIGHT;
				else
				{
					cAttr = CheckStandLayer(i, iX, iY - TILE_HEIGHT, iRevision, bReCalculate);
					if( cAttr > 0 )
						iRevision -= TILE_HEIGHT;
				}
			}

			eLayer = (_BG_ID)(BG_ID_LAYER_TERRAIN0 + i);
			break;
		}
		else
		{
			// down check
			cAttr = CheckStandLayer(i, iX, iY + TILE_HEIGHT, iRevision, bReCalculate);
			if( cAttr > 0 )
			{
				iRevision += TILE_HEIGHT;
				eLayer = (_BG_ID)(BG_ID_LAYER_TERRAIN0 + i);
				break;
			}
			
			// up check
			cAttr = CheckStandLayer(i, iX, iY - TILE_HEIGHT, iRevision, bReCalculate);
			if( cAttr > 0 )
			{
				iRevision -= TILE_HEIGHT;
				eLayer = (_BG_ID)(BG_ID_LAYER_TERRAIN0 + i);
				break;
			}
		}
	}

	return cAttr;
}

char SPTerrainAttribute::CheckStandLayer(int iLayer, int iX, int iY,
	int& iRevision, bool& bReCalculate)
{
	BYTE cAttr = 0;
	int iDiffX, iDiffY;

	if( iX < 0 )
		iX = 0;
	if( iY < 0 )
		iY = 0;

	iRevision = 0;
	cAttr = GetAttr(iLayer, iX, iY);
	if( cAttr > 0 )
	{
		iDiffX = iX % TILE_WIDTH;
		iDiffY = iY % TILE_HEIGHT;

		iRevision = GetPosCorrect(cAttr, iDiffX, iDiffY, bReCalculate);
	}

	return cAttr;
}

// ftol2() 함수의 오버헤드를 개선함.
#ifdef _USING_MY_FTOL
inline int MY_ftol(float f) 
{ 
	int rvalue; 

	_asm { 
		fld		f 
		fistp	rvalue 
	} 

	return rvalue; 
} 
#endif

BYTE SPTerrainAttribute::GetAttr(int iLayer, int iX, int iY)
{
	if( iLayer < 0 || iLayer >= MAX_ATTR_LAYER )
		return 0;

	if( m_pkAttribute[iLayer] == NULL )
		return 0;

	if( iX >= m_iLenX || iY >= m_iLenY )
		return 0;

	if( iX < 0 )
		iX = 0;
	if( iY < 0 )
		iY = 0;

#ifdef _USING_MY_FTOL
	int iTX = MY_ftol(fx) / 32;
	int iTY = MY_ftol(fy) / 16;
	int iTLenX = MY_ftol(m_fLenX)/32;
#else
	int iTX = iX / 32;
	int iTY = iY / 16;
	int iTLenX = m_iLenX/32;
#endif

	int index = iTX + (iTLenX * iTY);
	BYTE cAttr = m_pkAttribute[iLayer][index];

	return cAttr;
}

int SPTerrainAttribute::GetPosCorrect(BYTE cAttr, int iX, int iY, bool& bReCalculate)
{
	int iRevision = 0;
	bReCalculate = false;

	switch( cAttr )
	{		
	case 0x01:
		iRevision = -iY;
		break;

	case 0x02:
		iRevision = m_iHalfTileY - 1 - iY;
		break;

	case 0x03:
		iRevision = TILE_HEIGHT - 1 - iY;
		break;

	case 0x06:
		if( iX < m_iHalfTileX )
			iRevision = m_iHalfTileX - 1 - iX - iY;
		else
			bReCalculate = true;
		break;
		
	case 0x16:
		if( iX >= m_iHalfTileX )
			iRevision = iX - m_iHalfTileX - iY;
		else
			bReCalculate = true;
		break;

	case 0x07:
		if( iX >= m_iHalfTileX )
			iRevision = TILE_WIDTH - 1 - iX - iY;
		else
			bReCalculate = true;
		break;

	case 0x17:
		if( iX < m_iHalfTileX )
			iRevision = iX - iY;
		else
			bReCalculate = true;
		break;

	case 0x08:
		iRevision = TILE_HEIGHT - 1 - (iX / 4) - iY;
		break;

	case 0x18:
		iRevision = m_iHalfTileY + (iX / 4) - iY;
		break;

	case 0x09:
		iRevision = m_iHalfTileY - 1 - (iX / 4) - iY;
		break;

	case 0x19:
		iRevision = (iX / 4) - iY;
		break;

	case 0x0A:
		iRevision = TILE_HEIGHT - 1 - (iX / 2) - iY;
		break;

	case 0x1A:
		iRevision = (iX / 2) - iY;
		break;

	case 0x0B:
		//if( iX >= m_iHalfTileX )
		//	iRevision = TILE_HEIGHT - 1 - iY;
		//else
		//	bReCalculate = true;
		iRevision = -iY;
		break;

	case 0x1B:
		//if( iX < m_iHalfTileX )
		//	iRevision = TILE_HEIGHT - 1 - iY;
		//else
		//	bReCalculate = true;
		iRevision = TILE_HEIGHT - 1 - iY;
		break;
	}

	return iRevision;
}


bool SPTerrainAttribute::EnableLadderUp(float fX, float fY)
{
	BYTE cAttr;
	int i;
	
	for( i = 0; i < MAX_ATTR_LAYER; i++ )
	{
		cAttr = GetAttr(i, fX, fY);
		if( IsLadder(cAttr) )
			return true;
		
		cAttr = GetAttr(i, fX, fY - TILE_HEIGHT);
		if( IsLadder(cAttr) )
			return true;
	}

	return false;
}

bool SPTerrainAttribute::EnableLadderDown(float fX, float fY)
{
	BYTE cAttr;
	int i;
	
	for( i = 0; i < MAX_ATTR_LAYER; i++ )
	{
		cAttr = GetAttr(i, fX, fY);
		if( IsLadder(cAttr) )
			return true;
		
		cAttr = GetAttr(i, fX, fY + TILE_HEIGHT);
		if( IsLadder(cAttr) )
			return true;
	}

	return false;
}

bool SPTerrainAttribute::EnableVerRopeUp(float fX, float fY)
{
	BYTE cAttr;
	int i;
	
	for( i = 0; i < MAX_ATTR_LAYER; i++ )
	{
		cAttr = GetAttr(i, fX, fY);
		if( IsVerRope(cAttr) )
			return true;
		
		cAttr = GetAttr(i, fX, fY - TILE_HEIGHT);
		if( IsVerRope(cAttr) )
			return true;
	}

	return false;
}

bool SPTerrainAttribute::EnableVerRopeDown(float fX, float fY)
{
	BYTE cAttr;
	int i;
	
	for( i = 0; i < MAX_ATTR_LAYER; i++ )
	{
		cAttr = GetAttr(i, fX, fY);
		if( IsVerRope(cAttr) )
			return true;
		
		cAttr = GetAttr(i, fX, fY + TILE_HEIGHT);
		if( IsVerRope(cAttr) )
			return true;
	}

	return false;
}

bool SPTerrainAttribute::EnableHozRopeUp(float fX, float fY)
{
	BYTE cAttr;
	int i;
	
	for( i = 0; i < MAX_ATTR_LAYER; i++ )
	{
		cAttr = GetAttr(i, fX, fY - TILE_HEIGHT);
		if( IsHozRope(cAttr) )
			return true;
	}

	return false;
}

bool SPTerrainAttribute::IsBlock(BYTE cAttr)
{
	if( cAttr == 0x04 || cAttr == 0x05 || cAttr == 0x14 )
		return true;

	return false;
}

bool SPTerrainAttribute::IsVerRope(BYTE cAttr)
{
	if( cAttr == 0x0C )
		return true;

	return false;
}

bool SPTerrainAttribute::IsLadder(BYTE cAttr)
{
	if( cAttr == 0x0D )
		return true;

	return false;
}


bool SPTerrainAttribute::IsHozRope(BYTE cAttr)
{
	if( cAttr == 0x0E )
		return true;

	return false;
}

bool SPTerrainAttribute::IsUpBlock(BYTE cAttr)
{
	if( cAttr == 0x0B || cAttr == 0x1B )
		return true;

	return false;
}

bool SPTerrainAttribute::IsGround(BYTE cAttr, int iX, int iY)
{
	if( cAttr == 0 || IsBlock(cAttr) || IsVerRope(cAttr) ||
		IsLadder(cAttr) || IsHozRope(cAttr) || IsUpBlock(cAttr) )
		return false;

	//if( cAttr == 0 || IsVerRope(cAttr) ||
	//	IsLadder(cAttr) || IsHozRope(cAttr) )
	//	return false;


	int iDiffX, iDiffY;
	iDiffX = iX % TILE_WIDTH;
	iDiffY = iY % TILE_HEIGHT;

	/*
	if( cAttr == 0x0B )
	{
		if( iDiffX < m_iHalfTileX )
			return false;
	}
	else if( cAttr == 0x1B )
	{
		if( iDiffX >= m_iHalfTileX )
			return false;
	}
	*/

	return true;
}