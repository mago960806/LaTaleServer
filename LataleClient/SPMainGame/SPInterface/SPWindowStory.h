#ifndef __SPWINDOW_STORY_H__
#define __SPWINDOW_STORY_H__

//------------------------------------------------------------------------------------
#define WIID_STORY_BUTTON_SKIN				(WIID_STORY + 1)
#define WIID_STORY_SKIP						(WIID_STORY + 2)
#define WIID_STORY_NEXT						(WIID_STORY + 3)

enum STORYVIEW_STATE {
	STORYVIEW_FADEIN,
	STORYVIEW_STORY,
	STORYVIEW_FADEOUT,
} ;


struct STORY_FACTOR;

class SPWindowStory : public SPWindow
{
public:
	SPWindowStory(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowStory(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);
	virtual ~SPWindowStory();

	virtual void Init();
	virtual void Clean();

	virtual void Process(float fTime);
	virtual void Render(float fTime);
	virtual void Show(bool bWithChild = true);

protected:
	SPIMESSAGE_OBJ_MAP_DECLARE()

	virtual int OnSkip(WPARAM wParam, LPARAM lParam);
	virtual int OnNext(WPARAM wParam, LPARAM lParam);

	virtual int OnSetFactor(WPARAM wParam, LPARAM lParam);

protected:
	void ImageRender();
	void ClearTexture();

	void ProcessFade(float fTime) ;
	void SetViewState(STORYVIEW_STATE iViewState);

protected:
	int m_iCurIndex;
	int m_iMaxIndex;
	STORY_FACTOR* m_pkCurStoryFactor;

	STORYVIEW_STATE	m_iViewState ;
	float	m_fFadeTime ;
	RECT	m_rtSrcFade ;
	RECT	m_rtDestFade ;
	int		m_iAlpha;
	SPTexture* m_pTexture ;

	std::vector< SPTexture* > m_vTexture;
};

#endif
