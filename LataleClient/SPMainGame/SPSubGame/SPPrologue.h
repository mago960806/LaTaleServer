
#pragma once

class CPacket;
class SPPrologue : public SPSubGameManager
{
public:
	SPPrologue();
	virtual ~SPPrologue();

	virtual bool Init();
	virtual void Clean();
	virtual void Process(float fTime);
	virtual void Render(float fTime);

	virtual bool WndProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual int	SPPerformMessage(UINT msg, WPARAM wparam = 0, LPARAM lParam = 0);

protected:
	SPGMESSAGE_MAP_DECLARE()

	int OnWorldEnter(WPARAM wParam, LPARAM lParam);
	int OnCharacterSelect(WPARAM wParam, LPARAM lParam);
	int OnUserIn(WPARAM wParam, LPARAM lParam);
	int OnUserMove(WPARAM wParam, LPARAM lParam);

	int OnPrologueComplete(WPARAM wParam, LPARAM lParam);

	void ProcessFade(float fTime);
	void FadeRender();

protected:

	SPTexture* m_pkFadeTexture;
	RECT m_rcSrcFade;
	RECT m_rcDstFade;
	int m_iAlphaFade;
	float m_fFadeAccmulateTime;
};