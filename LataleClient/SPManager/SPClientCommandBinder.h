// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPClientCommandBinder
// Comment : 
// Creation : metalgeni 2003-06-16 오후 4:50:38
//***************************************************************************


#pragma once

#include <vector>
#include <string>

using namespace std;

#define EXECUTE_CLIENT_COMMAND_MAP(__Command__, __Func__) \
	if(__Command__ == strCommand) __Func__( pszCommand, strArgument.c_str() );

class SPClientCommandBinder
{

public:
	SPClientCommandBinder();
	~SPClientCommandBinder();
	void ClientCommandExecute(const char* pszCommand);
	void ServerCommandExecute(const char* pszCommand);

private:
	// 누가 쓸라믄 쓰고 
	vector<string>	m_vCommandHistory;


private:
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// command map function list
	void OnLDTTest			(const char* pszCommand, const char* pszArg);
	void OnLDTTest2			(const char* pszCommand, const char* pszArg);
	void OnClean			(const char* pszCommand, const char* pszArg);
	void OnStat				(const char* pszCommand, const char* pszArg);
	void OnStatEx			(const char* pszCommand, const char* pszArg);
	void OnFXFroup			(const char* pszCommand, const char* pszArg);
	void OnFXFroupClear		(const char* pszCommand, const char* pszArg);
	void OnSetBGM			(const char* pszCommand, const char* pszArg);
	void OnSetEffect		(const char* pszCommand, const char* pszArg);
	void OnSetBGMVolume		(const char* pszCommand, const char* pszArg);
	void OnSetEffectVolume	(const char* pszCommand, const char* pszArg);	
	void OnTakeShot			(const char* pszCommand, const char* pszArg);
	void OnContrast			(const char* pszCommand, const char* pszArg);
	void OnSoundStat		(const char* pszCommand, const char* pszArg);
	void OnToolTipID		(const char* pszCommand, const char* pszArg);
	void OnInterfaceHide	(const char* pszCommand, const char* pszArg);
	void OnErrorBreak		(const char* pszCommand, const char* pszArg);		// Error Break

#ifdef _DEBUG
	void OnConnect			(const char* pszCommand, const char* pszArg);
	void OnDisconnect		(const char* pszCommand, const char* pszArg);
	void OnSendMsg			(const char* pszCommand, const char* pszArg);
	// duragon [2006/10/09] BeautyShop Window Test
	void OnBeautyShopOn		(const char* pszCommand, const char* pszArg);
	void OnLDTSaveTest		(const char* pszCommand, const char* pszArg);		// LDT Save TEST
	void OnGuildInfo		(const char* pszCommand, const char* pszArg);		// Guild Info
	void OnGuildLevelup		(const char* pszCommand, const char* pszArg);		// Guild LevelUp
	void OnGuildEmblem		(const char* pszCommand, const char* pszArg);		// Guild Emblem
	void OnCropInfo			(const char* pszCommand, const char* pszArg);		// Crop Info
	void OnCropSend			(const char* pszCommand, const char* pszArg);		// Crop Send Packet
	void OnGuildSend		(const char* pszCommand, const char* pszArg);		// Guild Packet Test
	void OnIndunEnter		(const char* pszCommand, const char* pszArg);		// Indun Enter
	void OnGetIndunInfo		(const char* pszCommand, const char* pszArg);		// Indun Info
	void OnFDSSend			(const char* pszCommand, const char* pszArg);		// FDS Query
	void OnSetGuildPoint	(const char* pszCommand, const char* pszArg);		// GuildPoint Set
	void OnSaveTexture		(const char* pszCommand, const char* pszArg);		// Save Texture to BMP
	void OnJoyPadMode		(const char* pszCommand, const char* pszArg);		// Pad Mode Change Test
	void OnJoyPadKey		(const char* pszCommand, const char* pszArg);		// Pad Key Setting Test
	void OnTextBook			(const char* pszCommand, const char* pszArg);		// TextBook Test
	void OnTriggerPlayer	(const char* pszCommand, const char* pszArg);		// Player Move Test
	void OnRSShow			(const char* pszCommand, const char* pszArg);		// RsClient Window Show
	void OnPVPReserve		(const char* pszCommand, const char* pszArg);		// Pvp Reserve Window Test
	void OnPVPLoadWait		(const char* pszCommand, const char* pszArg);		// Pvp Loading Wait
	void OnPVPRank			(const char* pszCommand, const char* pszArg);		// Pvp Rank
	void OnPVPResult		(const char* pszCommand, const char* pszArg);		// Pvp Result
	void OnPVPTimmer		(const char* pszCommand, const char* pszArg);		// Pvp Timmer
	void OnAuctionShow		(const char* pszCommand, const char* pszArg);		// Auction WindowShow
	void OnCashTradeMode	(const char* pszCommand, const char* pszArg);		// Cash(Point) Trade Mode
	void OnCashTradeSelect	(const char* pszCommand, const char* pszArg);		// Cash Trade Select	
	void OnLoveCutInEnd		(const char* pszCommand, const char* pszArg);		// Love CutIn End
	void OnTumbleReload		(const char* pszCommand, const char* pszArg);		// reload tumble ldt
#endif

#ifdef _PHANTOM_EDITOR_ENABLE
	void OnPhantom			(const char* pszCommand, const char* pszArg);
#endif

	bool	m_bInterfaceHide;

};



