// ***************************************************************
//  LataleCommonDef   version:  1.0   ·  date: 03/09/2006
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2006 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#pragma once

//	/LaTaleError-AJJIYA:프로그램종류/날짜/시간	이면 FTP에 프로그램종류/날짜/시간_%d 형태의 경로를 만든다.
#define UNIQUE_LATALE_ERROR			"/LaTaleError-AJJIYA:"

#define	CMD_INFO					"/INFO:"
#define	CMD_DMP						"/DMP:"

#define CMD_LOCALE					"/LOCALE:"
#define CMD_PUBLISHER				"/PUBLISHER:"

#define CMD_WEBARG					"/WEBARG:"

#define CMD_END						" /"

#define X_TRAP_ARG					"660970B478FBCD4F53316D9844CFE862EDFBF9A2EE301D55E6B19D58425EC81C6AB1583FF5D7D547442421941EF69BA25367040921B44CA63EA7FC7A9A7477370F7D04245246E403AA620352D451731EFE32C417F598020CE56AD50E4C822F8BC204D9B5CECFEBFEE6F172"

#define CPF_DRIVE_NAME				"GPP-LaTale"

//////////////////////////////////////////////////////////////////////////

enum COUNTRY_LOCALE
{
	CL_NULL								=	0	,
	CL_KOREA									,
	CL_JAPAN									,
	CL_CHINA									,
	CL_USA										,
	CL_COUNT									,
};

const char * const s_szLocaleStringBuffer [ CL_COUNT ] = 
{
	"NULL"	,
	"KOREA"	,
	"JAPAN"	,
	"CHINA"	,
	"USA"
};

enum PUBLISHER
{
	PUBLISHER_NULL						=	0	,
	PUBLISHER_ACTOZ								,
	PUBLISHER_GAMEPOT							,
	PUBLISHER_SNDA								,
	PUBLISHER_BUDDYGAMES						,
	PUBLISHER_ATGAMES							,
	PUBLISHER_OGPLANET							,
	PUBLISHER_COUNT								,
};

const char * const s_szPublisherStringBuffer [ PUBLISHER_COUNT ] = 
{
	"NULL"			,
	"ACTOZ"			,
	"GAMEPOT"		,
	"SNDA"			,
	"BUDDYGAMES"	,
	"ATGAMES"		,
	"OGPLANET"
};
