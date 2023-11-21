#include <vector>

#ifdef _LATALE_CLIENT
#include "SPCommon.h"
#else 
#include "SPToolCommon.h"
#endif

#include "SPUtil.h"

#include "SPResourceDef.h"
#include "SPGOBClusterDef.h"

#if defined(_LATALE_CLIENT)
#include "SPGOBCluster.h"
#endif

const char* g_cstrPartLayer[] = {
	"",
	"01",		//WEAPON_OUT			
	"02",		//BLOUSE_ARM_OUT		
	"03",		//GLOVE_OUT			
	"04",		//HAND_OUT			
	"05",		//HAND_ARM_OUT		
	"06",		//ARM_OUT				
	"07",		//PANTS_LEG_OUT		
	"08",		//SHOE_OUT			
	"09",		//FOOT_OUT			
	"10",		//FOOT_LEG_OUT		
	"11",		//LEG_OUT				
	"12",		//BLOUSE				
	"13",		//PANTS				
	"14",		//BODY				
	"15",		//PANTS_LEG_IN		
	"16",		//SHOE_IN				
	"17",		//FOOT_IN				
	"18",		//FOOT_LEG_IN			
	"19",		//LEG_IN				
	"20",		//CAP_FRONT			
	"21",		//GOGGLE				
	"22",		//EAR					
	"23",		//HAIR_FRONT			
	"24",		//HAIR_REAR			
	"25",		//FACE				
	"26",		//MAKEUP				
	"27",		//HEAD				
	"28",		//CAP_REAR			
	"29",		//BLOUSE_ARM_IN		
	"30",		//GLOVE_IN			
	"31",		//HAND_IN				
	"32",		//HAND_ARM_IN			
	"33",		//ARM_IN				
	"34",		//WEAPON_IN			
	"35",		//COAT				
	"36",		//Buckler				
};





//////////////////////////////////////////////////////////////////////////
//WILL 레이어 2007년 10월패치 변동사항
//		테섭릴리즈 먼저 테스트해야함.
//
//	PL_HAND_OUT (4,1) -> (4,2)
//  PL_FACE (8,5) -> (8,10)
//

const int g_ciCharResIndex[][4] = { //사이즈X, 사이즈Y, 리소스갯수X, 리소스갯수Y
	{0,		0,		0,		0},		//NULL
	{128,	170,	1,		3},		//WEAPON_OUT			
	{42,	64,		12,		2},		//BLOUSE_ARM_OUT	
// test
	{32,	32,		4,		1},		//GLOVE_OUT			//기존
//	{32,	32,		4,		2},		//GLOVE_OUT			//WILL 레이어 2007년 10월패치 변동사항
#ifdef _LATALE_CLIENT
	{32,	32,		4,		1},		//HAND_OUT			//기존
#else
	{32,	32,		4,		2},		//HAND_OUT			//WILL 레이어 2007년 10월패치 변동사항
#endif

	{42,	64,		12,		2},		//HAND_ARM_OUT		
	{42,	64,		12,		2},		//ARM_OUT				
	{64,	64,		16,		1},		//PANTS_LEG_OUT		
	{42,	32,		3,		2},		//SHOE_OUT			
	{42,	32,		3,		2},		//FOOT_OUT			
	{64,	64,		16,		1},		//FOOT_LEG_OUT		
	{64,	64,		16,		1},		//LEG_OUT				
	{42,	64,		6,		1},		//BLOUSE				
	{84,	128,	12,		2},		//PANTS				
	{102,	64,		10,		1},		//BODY				
	{64,	64,		16,		1},		//PANTS_LEG_IN		
	{42,	32,		3,		2},		//SHOE_IN				
	{42,	32,		3,		2},		//FOOT_IN				
	{64,	64,		16,		1},		//FOOT_LEG_IN			
	{64,	64,		16,		1},		//LEG_IN				
	{128,	102,	1,		5},		//CAP_FRONT			
	{64,	50,		1,		5},		//GOGGLE				
	{64,	50,		1,		5},		//EAR					
	{84,	64,		12,		2},		//HAIR_FRONT			
	{84,	102,	3,		5},		//HAIR_REAR			

// test
#ifdef _LATALE_CLIENT
	{64,	50,		8,		5},		//FACE				//기존
#else
	{64,	50,		8,		10},	//FACE				//WILL 레이어 2007년 10월패치 변동사항
#endif


	{64,	50,		1,		5},		//MAKEUP				
	{64,	50,		1,		5},		//HEAD				
	{128,	102,	1,		5},		//CAP_REAR			
	{42,	64,		12,		2},		//BLOUSE_ARM_IN		
	{32,	32,		16,		1},		//GLOVE_IN			
	{32,	32,		16,		1},		//HAND_IN				
	{42,	64,		12,		2},		//HAND_ARM_IN			
	{42,	64,		12,		2},		//ARM_IN				
	{84,	128,	3,		1},		//WEAPON_IN			
	{128,	128,	8,		1},		//COAT
};

const char*	GetHeadString(ANIMATION_INDEX index)
{
	if( index < 0 || index >= ANI_NUM )
		return NULL;
	
	return g_strAnimationIndex[(int)index];
}

bool GetResIndexRect(PART_LAYER PartLayer, int iIndex, RECT& rect )
{
	// iIndex는  0 Base가 아니고 1 Base이다.
	iIndex--; // 0Base로 고침.
	if(iIndex >= 0 && iIndex < g_ciCharResIndex[PartLayer][RI_NOX]*g_ciCharResIndex[PartLayer][RI_NOY]) {
		int iXStep = iIndex % g_ciCharResIndex[PartLayer][RI_NOX] ;		
		int iYStep = iIndex / g_ciCharResIndex[PartLayer][RI_NOX] ;

		rect.left = g_ciCharResIndex[PartLayer][RI_SX] * iXStep;
		rect.right = rect.left + g_ciCharResIndex[PartLayer][RI_SX];

		rect.top = g_ciCharResIndex[PartLayer][RI_SY] * iYStep;
		rect.bottom = rect.top + g_ciCharResIndex[PartLayer][RI_SY];
	} else {
		return false;
	}	

	return true;
}

#if defined(_USED_TEXTURE_TABLE)
FRAME_INFO* GetResIndexRectbyTable(TEXTURE_TABLE_INFO* pTableInfo, int iIndex, RECT& rect)
{
	iIndex--;

	FRAME_INFO* pFrameInfo = g_pGOBCluster->GetFrame(pTableInfo, iIndex);
	if( pFrameInfo == NULL )
		return NULL;

	rect = pFrameInfo->rcResourceRgn;
	return pFrameInfo;
}

#endif
