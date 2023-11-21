#ifndef __SPWINDOW_PRESENTATION_H__
#define __SPWINDOW_PRESENTATION_H__

class SPWindowPresentation : public SPImageProcessor
{
public:
	SPWindowPresentation(unsigned int uiWIID, const char* pszImage, RECT rcSrc,
		float fStrPosX, float fStrPosY, float fDstPosX, float fDstPosY,
		float fVelocityX, float fVelocityY, bool bShow = true);
	SPWindowPresentation(unsigned int uiWIID, const char* pszImage, RECT rcSrc,
		float fStrPosX, float fStrPosY, float fDstPosX, float fDstPosY,
		float fLifeTime, bool bShow = true);

	virtual ~SPWindowPresentation();

	virtual unsigned int Process(float fElapsedTime);
	virtual void Render();

protected:
	float m_fCurPosX;
	float m_fCurPosY;
	float m_fStrPosX;
	float m_fStrPosY;
	float m_fDstPosX;
	float m_fDstPosY;
	float m_fVelocityX;
	float m_fVelocityY;

	int m_iDirectionX;
	int m_iDirectionY;
	float m_fAlpha;
	bool m_bShow;

	unsigned int m_uiWIID;
};

#endif