#ifndef __SPSHAKE_SCENE_H__
#define __SPSHAKE_SCENE_H__

class SPShakeScene : public SPImageProcessor
{
public:
	SPShakeScene();
	virtual ~SPShakeScene();

	virtual unsigned int Process(float fElapsedTime);
	virtual void Render();

	void SetActive(bool bActive);

protected:
	bool InitializeSurface();
	bool InitializeValue();

	void Clear();

protected:
	
	int m_iScreenWidth;
	int m_iScreenHeight;
	float m_fAccmulateTime;
	bool m_bActive;

	RECT m_rcOffscreenRgn;
	POINT m_ptRenderTargetPos;

	LPDIRECT3DSURFACE9 m_pOffscreenSurface;

};

#endif