#ifndef __SPLOBBY_CHARSELECT_H__
#define __SPLOBBY_CHARSELECT_H__

class SPLobbyCharSelect : public SPSubGameManager
{
public:
	SPLobbyCharSelect();
	virtual ~SPLobbyCharSelect();

	virtual bool Init();
	virtual void Clean();
	virtual void Process(float fTime);
	virtual void Render(float fTime);

	virtual bool WndProc(UINT message, WPARAM wParam, LPARAM lParam);

protected:

};

#endif