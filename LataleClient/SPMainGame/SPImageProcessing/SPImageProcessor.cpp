
#include "SPCommon.h"
#include "SPImageProcessor.h"

using namespace std;
//------------------------------------------------------------------------------------
SPImageProcessor::SPImageProcessor()
: m_pkTexture(NULL)
{
}

//------------------------------------------------------------------------------------
SPImageProcessor::SPImageProcessor(const char* pszImage, RECT rcSrc)
: m_rcSrcRgn(rcSrc)
{
	m_pkTexture = NULL;	
	string strResource;

	strResource = pszImage;
	g_pVideo->GetTextureMgr()->LoadTexture(strResource.c_str(), &m_pkTexture);
}

//------------------------------------------------------------------------------------
SPImageProcessor::~SPImageProcessor()
{
	SAFE_RELEASE(m_pkTexture);
}

//------------------------------------------------------------------------------------
unsigned int SPImageProcessor::Process(float fElapsedTime)
{
	return 0;
}

//------------------------------------------------------------------------------------
void SPImageProcessor::Render()
{
}