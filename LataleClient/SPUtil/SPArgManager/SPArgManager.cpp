#include <assert.h>
#include <Windows.h>

#include "SPCommon.h"

#include "SPArgManager.h"
#include "SPUtil.h"
#include "SPDebug.h"
#include "PacketID.h"

#include "SPInputDEF.h"
#include "SPManagerDEF.h"
#include "SPManager.h"
#include "SPSubGameManager.h"

#include "GlobalDefines_Share.h"
#include "_Error_log.h"


#define MAX_BUFF	256

extern bool g_bWindowed;
extern bool g_bFSDA ;		// Sanda Admin (for Windowed Mode)

// Server Info(IP,PORT)
extern char g_szGameServerIP[];
extern int g_iGameServerPort;

extern char g_szLobbyServerIP[];
extern int g_iLobbyServerPort;
extern bool g_bForceLobbyServer;

extern char g_szLoginServerIP[];
extern int g_iLoginServerPort;

extern WORLD_ID g_uiWorldID;
extern char g_szWebArg[];

extern bool g_bUsePackFile;
extern bool g_bFileLog;
extern bool g_bNoSound;
extern int g_iUserID;

#ifdef _RES_PATH
	extern TCHAR RES_WORK_RESOURCE_PATH[MAX_PATH];
#endif


struct _IsEqualCommand : binary_function<tagARGSTRUCT, string, bool> {
	bool operator() (tagARGSTRUCT Arguments, string strCommand) const
	{			
		return (Arguments.m_strCommand == strCommand.c_str() );
	}
} IsEqualCommand;

SPArgManager::SPArgManager(char * arg)
{
	string			strCmdLine, strParsing;

	strCmdLine = arg;
	size_t	cmd_size, offset, offset_old = 0 ;
	do 
	{		
		offset = strCmdLine.find(" ", offset_old);
		if(offset == string::npos)
		{
			cmd_size = strCmdLine.length() - (offset_old);
			strParsing = strCmdLine.substr(offset_old, cmd_size);
			ParsingArgument(strParsing);
			break;
		}
		cmd_size = offset - (offset_old);
		strParsing = strCmdLine.substr(offset_old, cmd_size);
		ParsingArgument(strParsing);

		offset_old = offset+1;
	} while(1);
}

SPArgManager::~SPArgManager(void)
{
	m_lArgList.clear();
}

void	SPArgManager::ParsingArgument(string strArg)
{
	/*  /<command>:<arg1>:<arg2> */
	tagARGSTRUCT	pushArg;	
	string			strInsert;

	// command
	size_t offset_command = strArg.find(":");
	if(offset_command==string::npos)
	{	
		pushArg.m_strCommand = _strupr(const_cast<char*>(strArg.c_str()));
		m_lArgList.push_back( pushArg );
		return;
	}
	strInsert = strArg.substr(0, offset_command);
	pushArg.m_strCommand = _strupr(const_cast<char*>(strInsert.c_str()));

	// argument
	size_t arg_size, offset_arg;
	size_t offset_arg_old = offset_command+1;	
	do
	{
		offset_arg = strArg.find(":", offset_arg_old);
		if(offset_arg==string::npos)
		{
			arg_size = strArg.length() - (offset_arg_old);
			strInsert = strArg.substr(offset_arg_old, arg_size);
			pushArg.m_Arguments.push_back(strInsert);
			break;
		}
		arg_size = offset_arg - (offset_arg_old);
		strInsert = strArg.substr(offset_arg_old, arg_size);
		pushArg.m_Arguments.push_back(strInsert);

		offset_arg_old = offset_arg+1;
	}while(1);

	m_lArgList.push_back( pushArg );
}

bool SPArgManager::ExcuteArgument()
{
	string str, strIndex;
	std::list<tagARGSTRUCT>::iterator Iter;
	
	Iter = m_lArgList.begin();
	while(Iter != m_lArgList.end())	
	{
		str = (*Iter).m_strCommand.c_str();
		
		// TODO : Add arg and method name
		
		//     /<command>:<arg1>:<arg2>... 
		// ex) /Char:kkandori /Full			

		strIndex = "/Full";
		EXCUTE_WORDCASE_MAP ( const_cast<char*>(strIndex.c_str())	, OnSetFullScreen)

		strIndex = "/Window";
		EXCUTE_WORDCASE_MAP ( const_cast<char*>(strIndex.c_str())	, OnSetWindowScreen)

		strIndex = "/LoginID";
		EXCUTE_WORDCASE_MAP ( const_cast<char*>(strIndex.c_str())	, OnSetLoginID) 

		strIndex = "/UserID";
		EXCUTE_WORDCASE_MAP ( const_cast<char*>(strIndex.c_str())	, OnSetUserID) 

		strIndex = "/Char";
		EXCUTE_WORDCASE_MAP ( const_cast<char*>(strIndex.c_str())	, OnSetChar) 

		strIndex = "/Server";
		EXCUTE_WORDCASE_MAP ( const_cast<char*>(strIndex.c_str())	, OnSetServer)

		strIndex = "/MoveDisable";
		EXCUTE_WORDCASE_MAP ( const_cast<char*>(strIndex.c_str())	, OnSetMoveDisable) // 서버테스트용

		strIndex = "/Mode";
		EXCUTE_WORDCASE_MAP ( const_cast<char*>(strIndex.c_str())	, OnGameMode) 

		strIndex = "/LoginServer";
		EXCUTE_WORDCASE_MAP ( const_cast<char*>(strIndex.c_str())	, OnSetLoginServer) 

		strIndex = "/LS";
		EXCUTE_WORDCASE_MAP ( const_cast<char*>(strIndex.c_str())	, OnSetLoginServer) 

		strIndex = "/LobbyServer";
		EXCUTE_WORDCASE_MAP ( const_cast<char*>(strIndex.c_str())	, OnSetLobbyServer) 

		strIndex = "/LBS";
		EXCUTE_WORDCASE_MAP ( const_cast<char*>(strIndex.c_str())	, OnSetLobbyServer) 

		strIndex = "/WorldID";
		EXCUTE_WORDCASE_MAP ( const_cast<char*>(strIndex.c_str())	, OnSetWorldID) 

		strIndex = "/NoSound";
		EXCUTE_WORDCASE_MAP ( const_cast<char*>(strIndex.c_str())	, OnNoSound ) 

		strIndex = "/Run";
		EXCUTE_WORDCASE_MAP ( const_cast<char*>(strIndex.c_str())	, OnRunApproval ) 

		strIndex = "/NoPack";
		EXCUTE_WORDCASE_MAP ( const_cast<char*>(strIndex.c_str())	, OnNoPack)

		strIndex = "/Pack";
		EXCUTE_WORDCASE_MAP ( const_cast<char*>(strIndex.c_str())	, OnPack)

		strIndex = "/FLOG";
		EXCUTE_WORDCASE_MAP ( const_cast<char*>(strIndex.c_str())	, OnFileLog)

		strIndex = "/ResPath";
		EXCUTE_WORDCASE_MAP ( const_cast<char*>(strIndex.c_str())	, OnResPath) 

		strIndex = "/FSDA";
		EXCUTE_WORDCASE_MAP ( const_cast<char*>(strIndex.c_str())	, OnFSDAdmin) 

		strIndex = CMD_LOCALE;
		strIndex.erase( strIndex.end() - 1 );
		EXCUTE_WORDCASE_MAP ( const_cast<char*>(strIndex.c_str())	, OnSetLocale ) 

		strIndex = CMD_WEBARG;
		strIndex.erase( strIndex.end() - 1 );
		EXCUTE_WORDCASE_MAP ( const_cast<char*>(strIndex.c_str())	, OnWebArg ) 

		strIndex = "/GameGuardPatch";
		EXCUTE_WORDCASE_MAP ( const_cast<char*>(strIndex.c_str())	, OnGameGuardPatch) 

		++Iter;
	}
	
	return true;
}

/*
	IterCommand : tagARGSTRUCT 
	bIsOnlyCommand - true: only command , false: arguments exists

	SET_ARGUMENT_HEADER();    // must use in head of function

	if(bIsOnlyCommand)		{}		// only command
	else					{}		// has arguments

	<< SAMPLE FUNCTION : OnSetChar >>
*/

WORDCASE_MAP ( OnSetFullScreen )
{
	SET_ARGUMENT_HEADER();

	if(bIsOnlyCommand)
	{
		g_bWindowed = false;
	}
	return true;
}

WORDCASE_MAP ( OnSetWindowScreen )
{
	SET_ARGUMENT_HEADER();

	if(bIsOnlyCommand)
	{
		g_bWindowed = true;
	}	
	return true;
}

WORDCASE_MAP ( OnSetLoginID)
{
	SET_ARGUMENT_HEADER();
	
	if(!bIsOnlyCommand)
	{			
		std::list<string>::iterator	IterArgument = CommandLine.m_Arguments.begin();
		while(IterArgument != CommandLine.m_Arguments.end())
		{
			string Argument = (*IterArgument);

			// Argument handling
			_tcsncpy(g_szLoginID, Argument.c_str(),MAX_LEN_ID);

			++IterArgument;
		}
	}
	return true;
}

WORDCASE_MAP ( OnSetChar )
{
	SET_ARGUMENT_HEADER();
	
	if(!bIsOnlyCommand)
	{			
		std::list<string>::iterator	IterArgument = CommandLine.m_Arguments.begin();
		while(IterArgument != CommandLine.m_Arguments.end())
		{
			string Argument = (*IterArgument);
			assert(Argument.size() < LEN_NAME);

			// Argument handling
			strcpy(g_szCharName, Argument.c_str());

			++IterArgument;
		}
	}
	return true;
}

// /server:<serverIP>:<port>
//
WORDCASE_MAP ( OnSetServer )
{
	SET_ARGUMENT_HEADER();

	int nIndex = 0;
	if(!bIsOnlyCommand)
	{		
		std::list<string>::iterator	IterArgument = CommandLine.m_Arguments.begin();
		while(IterArgument != CommandLine.m_Arguments.end())
		{
			string Argument = (*IterArgument);

			// Argument handling			
			switch(nIndex) // Arguments order
			{
				case 0:		// IP
				{
					strcpy(g_szGameServerIP, Argument.c_str());
					break;
				}					
				case 1:		// Port
				{
					g_iGameServerPort = atoi(Argument.c_str());
					break;
				}
			}		
			++nIndex;

			++IterArgument;
		}
	}
	return true;
}


// /LoginServer:<serverIP>:<port>
//
WORDCASE_MAP ( OnSetLoginServer )
{
	SET_ARGUMENT_HEADER();

	int nIndex = 0;
	if(!bIsOnlyCommand)
	{		
		std::list<string>::iterator	IterArgument = CommandLine.m_Arguments.begin();
		while(IterArgument != CommandLine.m_Arguments.end())
		{
			string Argument = (*IterArgument);

			// Argument handling			
			switch(nIndex) // Arguments order
			{
				case 0:		// IP
				{
					strcpy(g_szLoginServerIP, Argument.c_str());
					break;
				}					
				case 1:		// Port
				{
					g_iLoginServerPort = atoi(Argument.c_str());
					break;
				}
			}		
			++nIndex;

			++IterArgument;
		}
	}
	return true;
}


// /LobbyServer:<serverIP>:<port>
//
WORDCASE_MAP ( OnSetLobbyServer )
{
	SET_ARGUMENT_HEADER();

	int nIndex = 0;
	if(!bIsOnlyCommand)
	{		
		std::list<string>::iterator	IterArgument = CommandLine.m_Arguments.begin();
		while(IterArgument != CommandLine.m_Arguments.end())
		{
			string Argument = (*IterArgument);

			// Argument handling			
			switch(nIndex) // Arguments order
			{
			case 0:		// IP
				{
					strcpy(g_szLobbyServerIP, Argument.c_str());
					break;
				}					
			case 1:		// Port
				{
					g_iLobbyServerPort = atoi(Argument.c_str());
					break;
				}
			}		
			++nIndex;

			++IterArgument;
		}

		if( g_szLobbyServerIP != '\0' )
			g_bForceLobbyServer = true;
	}
	return true;
}


WORDCASE_MAP ( OnSetMoveDisable )
{
	SET_ARGUMENT_HEADER();

	if(bIsOnlyCommand)
	{
		g_LocalPlayerMoveDisable = true;
	}	
	return true;
}


WORDCASE_MAP ( OnGameMode )
{
	SET_ARGUMENT_HEADER();

	if(!bIsOnlyCommand)
	{			
		std::list<string>::iterator	IterArgument = CommandLine.m_Arguments.begin();
		while(IterArgument != CommandLine.m_Arguments.end())
		{
			string Argument = (*IterArgument);
			assert(Argument.size() < LEN_NAME);

			g_ExcutionMode = (EXECUTION_MODE)atoi(Argument.c_str());
			++IterArgument;
		}
	}
	return true;
}


WORDCASE_MAP ( OnSetWorldID )
{
	SET_ARGUMENT_HEADER();

	if(!bIsOnlyCommand)
	{			
		std::list<string>::iterator	IterArgument = CommandLine.m_Arguments.begin();
		while(IterArgument != CommandLine.m_Arguments.end())
		{
			string Argument = (*IterArgument);
			assert(Argument.size() < LEN_NAME);

			g_uiWorldID = (EXECUTION_MODE)atoi(Argument.c_str());
			++IterArgument;
		}
	}
	return true;
}

WORDCASE_MAP ( OnSetUserID )
{
	SET_ARGUMENT_HEADER();

	std::list<string>::iterator	IterArgument = CommandLine.m_Arguments.begin();
	if( IterArgument != CommandLine.m_Arguments.end() )
	{
		string Argument = (*IterArgument);
		g_iUserID = atoi(Argument.c_str());
	}

	return true;
}

WORDCASE_MAP ( OnNoPack )
{
	SET_ARGUMENT_HEADER();

	g_bUsePackFile = false;
	return true;
}

WORDCASE_MAP ( OnPack )
{
	SET_ARGUMENT_HEADER();

	g_bUsePackFile = true;
	return true;
}

WORDCASE_MAP ( OnFileLog )
{
	SET_ARGUMENT_HEADER();

	g_bFileLog = true;
	ErrorFLog::GetInstance()->SetFileLog(g_bFileLog) ;
	return true;
}


WORDCASE_MAP ( OnNoSound )
{
	SET_ARGUMENT_HEADER();

	g_bNoSound = true;
	return true;
}


WORDCASE_MAP ( OnRunApproval )
{
	SET_ARGUMENT_HEADER();

	int nIndex = 0;
	if(!bIsOnlyCommand)
	{		
		std::list<string>::iterator	IterArgument = CommandLine.m_Arguments.begin();
		string Argument = (*IterArgument);			
	
		//Argument = GetUpperString2(Argument.c_str());
		GetUpperString3(Argument, Argument.c_str());

		// Texture Create Option 
		if(Argument == "VLOW") {
			g_VideoQuality = VIDEOQUAL_LOW;
		} else if(Argument == "VHIGH"){
			g_VideoQuality = VIDEOQUAL_HIGH;
		} else {
			g_VideoQuality = VIDEOQUAL_NULL;
		}
		g_bRunApproval = true;
	}	
	return true;
}

WORDCASE_MAP ( OnSetLocale )
{
	SET_ARGUMENT_HEADER();

	int nIndex = 0;
	if(!bIsOnlyCommand)
	{		
		std::list<string>::iterator	IterArgument = CommandLine.m_Arguments.begin();
		string Argument = (*IterArgument);			

		//Argument = GetUpperString2(Argument.c_str());
		GetUpperString3(Argument, Argument.c_str());

		for( int i = 1 ; i < CL_COUNT ; ++i )
		{
			if( Argument.compare( s_szLocaleStringBuffer[ i ] ) == 0 )
			{
				g_eCountryLocale	=	(COUNTRY_LOCALE)i;
				break;
			}
		}
	}	
	return true;
}

WORDCASE_MAP ( OnWebArg )
{
	// Parameter Data Parsing -> SPLogin
	string strTotArgument ;
	SET_ARGUMENT_HEADER();

	if(!bIsOnlyCommand)
	{			
		std::list<string>::iterator	IterArgument = CommandLine.m_Arguments.begin();
		while(IterArgument != CommandLine.m_Arguments.end())
		{
			string Argument = (*IterArgument);
			strTotArgument += Argument ;
			++IterArgument;
			if( IterArgument != CommandLine.m_Arguments.end() )
				strTotArgument += ":" ;
		}
		strcpy( g_szWebArg , strTotArgument.c_str() );
	}
	return true;
}

WORDCASE_MAP ( OnGameGuardPatch )
{
	SET_ARGUMENT_HEADER();

	int nIndex = 0;
	if(!bIsOnlyCommand)
	{		
		std::list<string>::iterator	IterArgument = CommandLine.m_Arguments.begin();
		string Argument = (*IterArgument);			

		//Argument = GetUpperString2(Argument.c_str());
//		GetUpperString3(Argument, Argument.c_str());

		// Texture Create Option 
		if(Argument.compare( "Use" ) == 0 )
		{
			g_bGameGuardPatch	=	true;
		}
		else
		{
			g_bGameGuardPatch	=	false;
		}
	}	
	return true;
}

WORDCASE_MAP ( OnFSDAdmin )
{
	SET_ARGUMENT_HEADER();

	g_bFSDA = true;
	return true;
}


WORDCASE_MAP ( OnResPath )
{
	// Parameter Data Parsing -> SPLogin
	string strTotArgument ;
	SET_ARGUMENT_HEADER();

	if(!bIsOnlyCommand)
	{			
		std::list<string>::iterator	IterArgument = CommandLine.m_Arguments.begin();
		while(IterArgument != CommandLine.m_Arguments.end())
		{
			string Argument = (*IterArgument);
			strTotArgument += Argument ;
			++IterArgument;
			if( IterArgument != CommandLine.m_Arguments.end() )
				strTotArgument += ":" ;
		}
#ifdef _RES_PATH
		_tcsncpy( RES_WORK_RESOURCE_PATH, strTotArgument.c_str(),MAX_PATH);
#endif
	}
	return true;
}
