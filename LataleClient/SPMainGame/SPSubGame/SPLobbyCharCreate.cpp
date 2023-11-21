
#include "SPCommon.h"
#include "SPUtil.h"
#include "LT_Error.h"
#include "SPResourceDEF.h"
#include "SPMainGameDEF.H"

#include "SPManager.h"
#include "SPSubGameManager.h"
#include "SPLobbyCharCreate.h"

//------------------------------------------------------------------------------------
SPLobbyCharCreate::SPLobbyCharCreate()
: SPSubGameManager(SUBGAME_LOBBY_CHAR_CREATE)
{
}

//------------------------------------------------------------------------------------
SPLobbyCharCreate::~SPLobbyCharCreate()
{
	Clean();
}

//------------------------------------------------------------------------------------
void SPLobbyCharCreate::Clean()
{

}

//------------------------------------------------------------------------------------
bool SPLobbyCharCreate::Init()
{
	return true;
}

//------------------------------------------------------------------------------------
void SPLobbyCharCreate::Process(float fTime)
{
}

//------------------------------------------------------------------------------------
void SPLobbyCharCreate::Render(float fTime)
{
}

//------------------------------------------------------------------------------------
bool SPLobbyCharCreate::WndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	return true;
}