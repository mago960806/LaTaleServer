#include "SPCommon.h"
#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"
#include "SPTextBookManager.h"

using namespace std;

SPTextBookManager* SPTextBookManager::m_pkInstance = NULL;
SPTextBookManager* g_pkBookManger = NULL;
//------------------------------------------------------------------------------------
SPTextBookManager* SPTextBookManager::GetInstance()
{
	if( m_pkInstance == NULL )
	{
		m_pkInstance = new SPTextBookManager;
		g_pkBookManger = m_pkInstance;
	}

	return m_pkInstance;
}

//------------------------------------------------------------------------------------
void SPTextBookManager::Release()
{
	if( m_pkInstance )
	{
		g_pkBookManger = NULL;

		delete m_pkInstance;
		m_pkInstance = NULL;
	}
}

//------------------------------------------------------------------------------------
SPTextBookManager::SPTextBookManager()
{
}

//------------------------------------------------------------------------------------
SPTextBookManager::~SPTextBookManager()
{
	m_mStoryFactor.clear();
}

//------------------------------------------------------------------------------------
bool SPTextBookManager::Init()
{
	bool bRet;

	m_iTextBookID = 0 ;
	bRet = LoadLDTFile();
	return bRet;
}

//------------------------------------------------------------------------------------
bool SPTextBookManager::LoadLDTFile()
{
	string strLDTFile;
	strLDTFile = "DATA/LDT/BOOK.LDT";

	SPLDTFile* pkLDTFile = NULL;
	if( !g_pResourceManager->GetLDTFromFileName(strLDTFile.c_str(), &pkLDTFile) )
		return false;

	int iDataNum, iItemID, i;
	LDT_Field Field;
	TEXTBOOK_FACTOR tFactor;

	iDataNum = pkLDTFile->GetItemCount();
	for( i = 0; i < iDataNum; i++ )
	{
		iItemID = pkLDTFile->GetPrimaryKey(i);
		tFactor.Clear();

		Field.Init() ;					// BGM
		pkLDTFile->GetField(iItemID, 0, Field);
		if( pkLDTFile->CheckLDTField(Field) )		tFactor.strBGM = Field.uData.pValue ;

		Field.Init() ;					// TEXT
		pkLDTFile->GetField(iItemID, 1, Field);
		if( pkLDTFile->CheckLDTField(Field) )		tFactor.strText = Field.uData.pValue ;

		Field.Init() ;					// PrePage
		pkLDTFile->GetField(iItemID, 2, Field);
		if( pkLDTFile->CheckLDTField(Field) )		tFactor.iPrePage = Field.uData.lValue ;

		Field.Init() ;					// NextPage
		pkLDTFile->GetField(iItemID, 3, Field);
		if( pkLDTFile->CheckLDTField(Field) )		tFactor.iNextPage = Field.uData.lValue ;

		Field.Init() ;					// Image
		pkLDTFile->GetField(iItemID, 4, Field);
		if( pkLDTFile->CheckLDTField(Field) )		tFactor.strImage = Field.uData.pValue ;

		Field.Init() ;					// ImageIndex
		pkLDTFile->GetField(iItemID, 5, Field);
		if( pkLDTFile->CheckLDTField(Field) )		tFactor.iImageIndex = Field.uData.lValue ;

		// Insert Data
		m_mStoryFactor.insert(make_pair(iItemID, tFactor));
	}

	return true;
}

//------------------------------------------------------------------------------------
TEXTBOOK_FACTOR* SPTextBookManager::GetStoryFactor(int iID)
{
	map< int, TEXTBOOK_FACTOR >::iterator iter = m_mStoryFactor.find(iID);
	if( iter == m_mStoryFactor.end() )
		return NULL;

	return &(*iter).second;
}


/**
	Image 삽화 소스는 1024 * 1024 (4*4)
	삽화 하나의 크기는 256*256
	삽화의 Index 는 1 Base로 들어가 있다. (1~16)
*/
int	SPTextBookManager::GetImageSrc(int iIndex, int& iPosX, int& iPosY)
{
	int iRow = 0 ;
	int iColum = 0 ;
	int iCalcIndex = iIndex - 1 ;

	if( iCalcIndex <= 0 ) {
		iPosX = iPosY = 0 ;
		return 0 ;
	}

	iRow	= iCalcIndex / 4 ;
	iColum	= iCalcIndex % 4 ;
	iPosX = iColum * 256 ;
	iPosY = iRow * 256 ;
	return 1 ;
}