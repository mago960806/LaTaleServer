
#include "SPCommon.h"
#include "SPUtil.h"
#include "LT_Error.h"
#include "SPResourceDEF.h"
#include "SPMainGameDEF.H"

#include "SPManager.h"
#include "SPSubGameManager.h"
#include "SPLobbyCharSelect.h"

//------------------------------------------------------------------------------------
SPLobbyCharSelect::SPLobbyCharSelect()
: SPSubGameManager(SUBGAME_LOBBY_CHAR_SELECT)
{
}

//------------------------------------------------------------------------------------
SPLobbyCharSelect::~SPLobbyCharSelect()
{
}

//------------------------------------------------------------------------------------
void SPLobbyCharSelect::Clean()
{
}

//------------------------------------------------------------------------------------
bool SPLobbyCharSelect::Init()
{
	return true;
}

//------------------------------------------------------------------------------------
void SPLobbyCharSelect::Process(float fTime)
{
}

//------------------------------------------------------------------------------------
void SPLobbyCharSelect::Render(float fTime)
{
}

//------------------------------------------------------------------------------------
bool SPLobbyCharSelect::WndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	return true;
}