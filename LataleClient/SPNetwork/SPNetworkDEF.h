// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPNetworkDEF
// Comment     : 
// Creation    : metalgeni 2004-11-16 ¿ÀÀü 10:26:19
//***************************************************************************

#pragma once

#define SERVER_IPADDR			"AbioTest"
#define LOGINSERVER_PORT		10000
#define GAMESERVER_PORT			5001


#define MAX_WORLD				10

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Network Message List

#include "PacketID.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Game Object Message Function Macro

#define SPNMESSAGE_FUNC SPNMessageMapFunc
#define SPNMESSAGE_MAP_DECLARE() \
protected :\
	virtual int SPNMESSAGE_FUNC( UINT spnm, WPARAM wParam, LPARAM lParam);

#define SPNMESSAGE_MAP_BEGIN( __class ) \
	int __class::SPNMESSAGE_FUNC( UINT spnm, WPARAM wParam, LPARAM lParam) {

#define SPNMESSAGE_MAP_END_BASE() \
	return -1;\
}

#define SPNMESSAGE_COMMAND( message, msg_func )		\
	if ( spnm == (UINT)message )	{ return msg_func( wParam, lParam ); }

#define SPNMESSAGE_COMMAND_RANGE( message1, message2, msg_func )		\
	if ( spnm >= message1 && spnm <= message2 )	{ return msg_func( wParam, lParam ); }




































