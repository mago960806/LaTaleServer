#ifndef __SPLOBBY_CHARCREATE_H__
#define __SPLOBBY_CHARCREATE_H__

class SPLobbyCharCreate : public SPSubGameManager
{
public:
	SPLobbyCharCreate();
	virtual ~SPLobbyCharCreate();

	virtual bool Init();
	virtual void Clean();
	virtual void Process(float fTime);
	virtual void Render(float fTime);

	virtual bool WndProc(UINT message, WPARAM wParam, LPARAM lParam);

protected:

};

#endif