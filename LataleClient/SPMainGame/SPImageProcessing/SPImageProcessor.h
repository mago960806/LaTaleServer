#ifndef __SPIMAGE_PROCESSOR_H__
#define __SPIMAGE_PROCESSOR_H__

class SPImageProcessor
{
public:
	SPImageProcessor();
	SPImageProcessor(const char* pszImage, RECT rcSrc);
	virtual ~SPImageProcessor();

	virtual unsigned int Process(float fElapsedTime);
	virtual void Render();

protected:
	
	RECT m_rcSrcRgn;

	SPTexture* m_pkTexture;
};

#endif