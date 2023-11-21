// ***************************************************************
//  SPWindowChattingRoom   version:  1.0   ·  date: 02/25/2008
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#ifndef SPWindowChattingRoom_h__
#define SPWindowChattingRoom_h__

#pragma once

#define WIID_CHATTING_ROOM_CLOSE								(WIID_CHATTING_ROOM + 1)
#define WIID_CHATTING_ROOM_SUBJECT								(WIID_CHATTING_ROOM + 2)

#define WIID_CHATTING_ROOM_MAKE									(WIID_CHATTING_ROOM + 10)
#define WIID_CHATTING_ROOM_MENU_START							WIID_CHATTING_ROOM_MAKE
#define WIID_CHATTING_ROOM_FIND									(WIID_CHATTING_ROOM + 20)
#define WIID_CHATTING_ROOM_PAL									(WIID_CHATTING_ROOM + 30)
#define WIID_CHATTING_ROOM_SETUP								(WIID_CHATTING_ROOM + 40)
#define WIID_CHATTING_ROOM_MENU_END								WIID_CHATTING_ROOM_SETUP

#define CHATTING_ROOM_MENU_STEP									(10)
#define CHATTING_ROOM_MENU_PANEL_STEP							(10)

//////////////////////////////////////////////////////////////////////////

#define WIID_CHATTING_ROOM_MAKE_PANEL							(WIID_CHATTING_ROOM + 100)

#define WIID_CHATTING_ROOM_MAKE_ARROW							(WIID_CHATTING_ROOM + 101)
#define WIID_CHATTING_ROOM_MAKE_SETUP_SUBJECT					(WIID_CHATTING_ROOM + 102)
#define WIID_CHATTING_ROOM_MAKE_SETUP_BG						(WIID_CHATTING_ROOM + 103)

#define WIID_CHATTING_ROOM_MAKE_SUBJECT_IMG						(WIID_CHATTING_ROOM + 105)
#define WIID_CHATTING_ROOM_MAKE_SUBJECT_EDIT					(WIID_CHATTING_ROOM + 106)
#define WIID_CHATTING_ROOM_MAKE_SUBJECT_EDIT_BG_LEFT			(WIID_CHATTING_ROOM + 107)
#define WIID_CHATTING_ROOM_MAKE_SUBJECT_EDIT_BG_CENTER			(WIID_CHATTING_ROOM + 108)
#define WIID_CHATTING_ROOM_MAKE_SUBJECT_EDIT_BG_RIGHT			(WIID_CHATTING_ROOM + 109)

#define WIID_CHATTING_ROOM_MAKE_ADVERTISEMENT_IMG				(WIID_CHATTING_ROOM + 110)
#define WIID_CHATTING_ROOM_MAKE_ADVERTISEMENT_EDIT				(WIID_CHATTING_ROOM + 111)
#define WIID_CHATTING_ROOM_MAKE_ADVERTISEMENT_EDIT_BG_LEFT		(WIID_CHATTING_ROOM + 112)
#define WIID_CHATTING_ROOM_MAKE_ADVERTISEMENT_EDIT_BG_CENTER	(WIID_CHATTING_ROOM + 113)
#define WIID_CHATTING_ROOM_MAKE_ADVERTISEMENT_EDIT_BG_RIGHT		(WIID_CHATTING_ROOM + 114)
#define WIID_CHATTING_ROOM_MAKE_ADVERTISEMENT_HELP				(WIID_CHATTING_ROOM + 115)
#define WIID_CHATTING_ROOM_MAKE_ADVERTISEMENT_USE				(WIID_CHATTING_ROOM + 116)
#define WIID_CHATTING_ROOM_MAKE_ADVERTISEMENT_USE_IMG			(WIID_CHATTING_ROOM + 117)

#define WIID_CHATTING_ROOM_MAKE_DETAIL_SETUP_IMG				(WIID_CHATTING_ROOM + 118)
#define WIID_CHATTING_ROOM_MAKE_DETAIL_BRACKET_LEFT_TOP			(WIID_CHATTING_ROOM + 119)
#define WIID_CHATTING_ROOM_MAKE_DETAIL_BRACKET_LEFT_MIDDLE		(WIID_CHATTING_ROOM + 120)
#define WIID_CHATTING_ROOM_MAKE_DETAIL_BRACKET_LEFT_BOTTOM		(WIID_CHATTING_ROOM + 121)
#define WIID_CHATTING_ROOM_MAKE_DETAIL_BRACKET_RIGHT_TOP		(WIID_CHATTING_ROOM + 122)
#define WIID_CHATTING_ROOM_MAKE_DETAIL_BRACKET_RIGHT_MIDDLE		(WIID_CHATTING_ROOM + 123)
#define WIID_CHATTING_ROOM_MAKE_DETAIL_BRACKET_RIGHT_BOTTOM		(WIID_CHATTING_ROOM + 124)

#define WIID_CHATTING_ROOM_MAKE_DETAIL_TITLE_ROOM_TYPE			(WIID_CHATTING_ROOM + 125)
#define WIID_CHATTING_ROOM_MAKE_DETAIL_TITLE_GENDER				(WIID_CHATTING_ROOM + 126)
#define WIID_CHATTING_ROOM_MAKE_DETAIL_TITLE_AGE				(WIID_CHATTING_ROOM + 127)
#define WIID_CHATTING_ROOM_MAKE_DETAIL_TITLE_NUMBER				(WIID_CHATTING_ROOM + 128)
#define WIID_CHATTING_ROOM_MAKE_DETAIL_TITLE_PASSWORD			(WIID_CHATTING_ROOM + 129)

#define WIID_CHATTING_ROOM_MAKE_DETAIL_ARROW_LEFT_ROOM_TYPE		(WIID_CHATTING_ROOM + 130)
#define WIID_CHATTING_ROOM_MAKE_DETAIL_ARROW_RIGHT_ROOM_TYPE	(WIID_CHATTING_ROOM + 131)
#define WIID_CHATTING_ROOM_MAKE_DETAIL_BG_LEFT_ROOM_TYPE		(WIID_CHATTING_ROOM + 132)
#define WIID_CHATTING_ROOM_MAKE_DETAIL_BG_CENTER_ROOM_TYPE		(WIID_CHATTING_ROOM + 133)
#define WIID_CHATTING_ROOM_MAKE_DETAIL_BG_RIGHT_ROOM_TYPE		(WIID_CHATTING_ROOM + 134)
#define WIID_CHATTING_ROOM_MAKE_DETAIL_TEXT_ROOM_TYPE			(WIID_CHATTING_ROOM + 135)

#define WIID_CHATTING_ROOM_MAKE_DETAIL_ARROW_LEFT_GENDER		(WIID_CHATTING_ROOM + 136)
#define WIID_CHATTING_ROOM_MAKE_DETAIL_ARROW_RIGHT_GENDER		(WIID_CHATTING_ROOM + 137)
#define WIID_CHATTING_ROOM_MAKE_DETAIL_BG_LEFT_GENDER			(WIID_CHATTING_ROOM + 138)
#define WIID_CHATTING_ROOM_MAKE_DETAIL_BG_CENTER_GENDER			(WIID_CHATTING_ROOM + 139)
#define WIID_CHATTING_ROOM_MAKE_DETAIL_BG_RIGHT_GENDER			(WIID_CHATTING_ROOM + 140)
#define WIID_CHATTING_ROOM_MAKE_DETAIL_TEXT_GENDER				(WIID_CHATTING_ROOM + 141)

#define WIID_CHATTING_ROOM_MAKE_DETAIL_ARROW_LEFT_AGE			(WIID_CHATTING_ROOM + 142)
#define WIID_CHATTING_ROOM_MAKE_DETAIL_ARROW_RIGHT_AGE			(WIID_CHATTING_ROOM + 143)
#define WIID_CHATTING_ROOM_MAKE_DETAIL_BG_LEFT_AGE				(WIID_CHATTING_ROOM + 144)
#define WIID_CHATTING_ROOM_MAKE_DETAIL_BG_CENTER_AGE			(WIID_CHATTING_ROOM + 145)
#define WIID_CHATTING_ROOM_MAKE_DETAIL_BG_RIGHT_AGE				(WIID_CHATTING_ROOM + 146)
#define WIID_CHATTING_ROOM_MAKE_DETAIL_TEXT_AGE					(WIID_CHATTING_ROOM + 147)

#define WIID_CHATTING_ROOM_MAKE_DETAIL_BG_LEFT_NUMBER			(WIID_CHATTING_ROOM + 148)
#define WIID_CHATTING_ROOM_MAKE_DETAIL_BG_CENTER_NUMBER			(WIID_CHATTING_ROOM + 149)
#define WIID_CHATTING_ROOM_MAKE_DETAIL_BG_RIGHT_NUMBER			(WIID_CHATTING_ROOM + 150)
#define WIID_CHATTING_ROOM_MAKE_DETAIL_EDIT_NUMBER				(WIID_CHATTING_ROOM + 151)
#define WIID_CHATTING_ROOM_MAKE_DETAIL_EDIT_NUMBER_LIMIT		(WIID_CHATTING_ROOM + 152)

#define WIID_CHATTING_ROOM_MAKE_DETAIL_EDIT_PASSWORD			(WIID_CHATTING_ROOM + 153)

#define WIID_CHATTING_ROOM_MAKE_RUN								(WIID_CHATTING_ROOM + 154)
#define WIID_CHATTING_ROOM_MAKE_CLOSE							(WIID_CHATTING_ROOM + 155)

//////////////////////////////////////////////////////////////////////////

#define WIID_CHATTING_ROOM_FIND_PANEL							(WIID_CHATTING_ROOM + 200)

#define WIID_CHATTING_ROOM_FIND_ARROW							(WIID_CHATTING_ROOM + 201)
#define WIID_CHATTING_ROOM_FIND_LIST_SUBJECT					(WIID_CHATTING_ROOM + 202)

#define WIID_CHATTING_ROOM_FIND_NORMAL							(WIID_CHATTING_ROOM + 203)
#define WIID_CHATTING_ROOM_FIND_MARKET							(WIID_CHATTING_ROOM + 204)
#define WIID_CHATTING_ROOM_FIND_WANTED							(WIID_CHATTING_ROOM + 205)
#define WIID_CHATTING_ROOM_FIND_MEETING							(WIID_CHATTING_ROOM + 206)

#define WIID_CHATTING_ROOM_FIND_MENU_START						WIID_CHATTING_ROOM_FIND_NORMAL
#define WIID_CHATTING_ROOM_FIND_MENU_END						WIID_CHATTING_ROOM_FIND_MEETING

#define WIID_CHATTING_ROOM_FIND_LINE_1							(WIID_CHATTING_ROOM + 207)
#define WIID_CHATTING_ROOM_FIND_LINE_2							(WIID_CHATTING_ROOM + 208)

#define WIID_CHATTING_ROOM_FIND_TAB_NO							(WIID_CHATTING_ROOM + 209)
#define WIID_CHATTING_ROOM_FIND_TAB_ROOM_SUBJECT				(WIID_CHATTING_ROOM + 210)
#define WIID_CHATTING_ROOM_FIND_TAB_AGE							(WIID_CHATTING_ROOM + 211)
#define WIID_CHATTING_ROOM_FIND_TAB_NUMBER						(WIID_CHATTING_ROOM + 212)

#define WIID_CHATTING_ROOM_FIND_SLIDER_UP						(WIID_CHATTING_ROOM + 213)
#define WIID_CHATTING_ROOM_FIND_SLIDER_DOWN						(WIID_CHATTING_ROOM + 214)
#define WIID_CHATTING_ROOM_FIND_SLIDER							(WIID_CHATTING_ROOM + 215)

#define WIID_CHATTING_ROOM_FIND_REFRESH							(WIID_CHATTING_ROOM + 216)

#define WIID_CHATTING_ROOM_FIND_ROOM_NUMBER_IMG					(WIID_CHATTING_ROOM + 217)
#define WIID_CHATTING_ROOM_FIND_ROOM_NUMBER						(WIID_CHATTING_ROOM + 218)
#define WIID_CHATTING_ROOM_FIND_JOIN							(WIID_CHATTING_ROOM + 219)
#define WIID_CHATTING_ROOM_FIND_CLOSE							(WIID_CHATTING_ROOM + 220)

#define CHATTING_ROOM_FIND_LIST_COUNT							(10)

#define WIID_CHATTING_ROOM_FIND_LIST_SELECT_BG					(WIID_CHATTING_ROOM + 230)
#define WIID_CHATTING_ROOM_FIND_LIST_NO							(WIID_CHATTING_ROOM_FIND_LIST_SELECT_BG			+ CHATTING_ROOM_FIND_LIST_COUNT)
#define WIID_CHATTING_ROOM_FIND_LIST_LOCK						(WIID_CHATTING_ROOM_FIND_LIST_NO				+ CHATTING_ROOM_FIND_LIST_COUNT)
#define WIID_CHATTING_ROOM_FIND_LIST_ROOM_SUBJECT				(WIID_CHATTING_ROOM_FIND_LIST_LOCK				+ CHATTING_ROOM_FIND_LIST_COUNT)
#define WIID_CHATTING_ROOM_FIND_LIST_AGE						(WIID_CHATTING_ROOM_FIND_LIST_ROOM_SUBJECT		+ CHATTING_ROOM_FIND_LIST_COUNT)
#define WIID_CHATTING_ROOM_FIND_LIST_NUMBER						(WIID_CHATTING_ROOM_FIND_LIST_AGE				+ CHATTING_ROOM_FIND_LIST_COUNT)

//////////////////////////////////////////////////////////////////////////

#define WIID_CHATTING_ROOM_PAL_PANEL							(WIID_CHATTING_ROOM + 300)

#define WIID_CHATTING_ROOM_PAL_ARROW							(WIID_CHATTING_ROOM + 301)

#define WIID_CHATTING_ROOM_PAL_TAB_GENDER						(WIID_CHATTING_ROOM + 302)
#define WIID_CHATTING_ROOM_PAL_TAB_ID							(WIID_CHATTING_ROOM + 303)
#define WIID_CHATTING_ROOM_PAL_TAB_LV							(WIID_CHATTING_ROOM + 304)
#define WIID_CHATTING_ROOM_PAL_TAB_CLASS						(WIID_CHATTING_ROOM + 305)

#define WIID_CHATTING_ROOM_PAL_SEARCH_BG						(WIID_CHATTING_ROOM + 306)

#define WIID_CHATTING_ROOM_PAL_SLIDER_UP						(WIID_CHATTING_ROOM + 307)
#define WIID_CHATTING_ROOM_PAL_SLIDER_DOWN						(WIID_CHATTING_ROOM + 308)
#define WIID_CHATTING_ROOM_PAL_SLIDER							(WIID_CHATTING_ROOM + 309)

#define WIID_CHATTING_ROOM_PAL_LINE								(WIID_CHATTING_ROOM + 310)

#define WIID_CHATTING_ROOM_PAL_REFRESH							(WIID_CHATTING_ROOM + 311)
#define WIID_CHATTING_ROOM_PAL_SEARCH							(WIID_CHATTING_ROOM + 312)
#define WIID_CHATTING_ROOM_PAL_INVITE							(WIID_CHATTING_ROOM + 313)

#define WIID_CHATTING_ROOM_PAL_TITLE							(WIID_CHATTING_ROOM + 314)
#define WIID_CHATTING_ROOM_PAL_CHAR								(WIID_CHATTING_ROOM + 315)
#define WIID_CHATTING_ROOM_PAL_CLASS							(WIID_CHATTING_ROOM + 316)
#define WIID_CHATTING_ROOM_PAL_LV_ID							(WIID_CHATTING_ROOM + 317)

#define WIID_CHATTING_ROOM_PAL_CHAR_SILHOUETTE		 			(WIID_CHATTING_ROOM + 318)
#define WIID_CHATTING_ROOM_PAL_STYLE_DEFAULT_BG					(WIID_CHATTING_ROOM + 319)
#define WIID_CHATTING_ROOM_PAL_STYLE_NOT_OPEN_PAL_BG			(WIID_CHATTING_ROOM + 320)
#define WIID_CHATTING_ROOM_PAL_STYLE_NOT_OPEN_ME_BG				(WIID_CHATTING_ROOM + 321)

#define WIID_CHATTING_ROOM_PAL_MY_STYLE							(WIID_CHATTING_ROOM + 322)
#define WIID_CHATTING_ROOM_PAL_CLOSE							(WIID_CHATTING_ROOM + 323)

#define CHATTING_ROOM_PAL_LIST_COUNT							(5)

#define WIID_CHATTING_ROOM_PAL_LIST_SELECT_BG					(WIID_CHATTING_ROOM + 330)
#define WIID_CHATTING_ROOM_PAL_LIST_GENDER						(WIID_CHATTING_ROOM_PAL_LIST_SELECT_BG	+ CHATTING_ROOM_PAL_LIST_COUNT)
#define WIID_CHATTING_ROOM_PAL_LIST_ID							(WIID_CHATTING_ROOM_PAL_LIST_GENDER		+ CHATTING_ROOM_PAL_LIST_COUNT)
#define WIID_CHATTING_ROOM_PAL_LIST_LV							(WIID_CHATTING_ROOM_PAL_LIST_ID			+ CHATTING_ROOM_PAL_LIST_COUNT)
#define WIID_CHATTING_ROOM_PAL_LIST_CLASS						(WIID_CHATTING_ROOM_PAL_LIST_LV			+ CHATTING_ROOM_PAL_LIST_COUNT)

#define CHATTING_ROOM_PAL_STYLE_COUNT							(4)

#define WIID_CHATTING_ROOM_PAL_STYLE_IMG						(WIID_CHATTING_ROOM + 360)
#define WIID_CHATTING_ROOM_PAL_STYLE_TEXT_BG_LEFT				(WIID_CHATTING_ROOM_PAL_STYLE_IMG				+ CHATTING_ROOM_PAL_STYLE_COUNT)
#define WIID_CHATTING_ROOM_PAL_STYLE_TEXT_BG_CENTER				(WIID_CHATTING_ROOM_PAL_STYLE_TEXT_BG_LEFT		+ CHATTING_ROOM_PAL_STYLE_COUNT)
#define WIID_CHATTING_ROOM_PAL_STYLE_TEXT_BG_RIGHT				(WIID_CHATTING_ROOM_PAL_STYLE_TEXT_BG_CENTER	+ CHATTING_ROOM_PAL_STYLE_COUNT)
#define WIID_CHATTING_ROOM_PAL_STYLE_TEXT						(WIID_CHATTING_ROOM_PAL_STYLE_TEXT_BG_RIGHT		+ CHATTING_ROOM_PAL_STYLE_COUNT)

//////////////////////////////////////////////////////////////////////////

#define WIID_CHATTING_ROOM_SETUP_PANEL							(WIID_CHATTING_ROOM + 400)

#define WIID_CHATTING_ROOM_SETUP_ARROW							(WIID_CHATTING_ROOM + 401)

#define WIID_CHATTING_ROOM_SETUP_TITLE							(WIID_CHATTING_ROOM + 402)
#define WIID_CHATTING_ROOM_SETUP_MY_INFO_SEARCH					(WIID_CHATTING_ROOM + 403)
#define WIID_CHATTING_ROOM_SETUP_MY_INFO_SEARCH_TEXT			(WIID_CHATTING_ROOM + 404)
#define WIID_CHATTING_ROOM_SETUP_CHAT_REQUEST					(WIID_CHATTING_ROOM + 405)
#define WIID_CHATTING_ROOM_SETUP_CHAT_REQUEST_TEXT				(WIID_CHATTING_ROOM + 406)
#define WIID_CHATTING_ROOM_SETUP_MY_STYLE_OPEN					(WIID_CHATTING_ROOM + 407)
#define WIID_CHATTING_ROOM_SETUP_MY_STYLE_OPEN_TEXT				(WIID_CHATTING_ROOM + 408)

#define WIID_CHATTING_ROOM_SETUP_SAVE							(WIID_CHATTING_ROOM + 423)
#define WIID_CHATTING_ROOM_SETUP_CLOSE							(WIID_CHATTING_ROOM + 424)

//////////////////////////////////////////////////////////////////////////

#define WIID_CHATTING_ROOM_MODAL_WAITTING						(WIID_CHATTING_ROOM + 700)
#define WIID_CHATTING_ROOM_MODAL_WAITTING_BG					(WIID_CHATTING_ROOM + 701)
#define WIID_CHATTING_ROOM_MODAL_WAITTING_TEXT					(WIID_CHATTING_ROOM + 702)
#define WIID_CHATTING_ROOM_MODAL_WAITTING_WHEEL_1				(WIID_CHATTING_ROOM + 703)
#define WIID_CHATTING_ROOM_MODAL_WAITTING_WHEEL_2				(WIID_CHATTING_ROOM + 704)

//////////////////////////////////////////////////////////////////////////

#define WIID_CHATTING_ROOM_MODAL_STYLE							(WIID_CHATTING_ROOM + 810)

#define WIID_CHATTING_ROOM_MODAL_STYLE_BG						(WIID_CHATTING_ROOM + 811)
#define WIID_CHATTING_ROOM_MODAL_STYLE_CLOSE					(WIID_CHATTING_ROOM + 812)
#define WIID_CHATTING_ROOM_MODAL_STYLE_TITLE					(WIID_CHATTING_ROOM + 813)
#define WIID_CHATTING_ROOM_MODAL_STYLE_SAVE						(WIID_CHATTING_ROOM + 814)
#define WIID_CHATTING_ROOM_MODAL_STYLE_CANCEL					(WIID_CHATTING_ROOM + 815)

#define WIID_CHATTING_ROOM_MODAL_STYLE_LEFT						(WIID_CHATTING_ROOM + 816)
#define WIID_CHATTING_ROOM_MODAL_STYLE_RIGHT					(WIID_CHATTING_ROOM_MODAL_STYLE_LEFT			+ CHATTING_ROOM_PAL_STYLE_COUNT)
#define WIID_CHATTING_ROOM_MODAL_STYLE_IMG						(WIID_CHATTING_ROOM_MODAL_STYLE_RIGHT			+ CHATTING_ROOM_PAL_STYLE_COUNT)
#define WIID_CHATTING_ROOM_MODAL_STYLE_TEXT_BG_LEFT				(WIID_CHATTING_ROOM_MODAL_STYLE_IMG				+ CHATTING_ROOM_PAL_STYLE_COUNT)
#define WIID_CHATTING_ROOM_MODAL_STYLE_TEXT_BG_CENTER			(WIID_CHATTING_ROOM_MODAL_STYLE_TEXT_BG_LEFT	+ CHATTING_ROOM_PAL_STYLE_COUNT)
#define WIID_CHATTING_ROOM_MODAL_STYLE_TEXT_BG_RIGHT			(WIID_CHATTING_ROOM_MODAL_STYLE_TEXT_BG_CENTER	+ CHATTING_ROOM_PAL_STYLE_COUNT)
#define WIID_CHATTING_ROOM_MODAL_STYLE_TEXT						(WIID_CHATTING_ROOM_MODAL_STYLE_TEXT_BG_RIGHT	+ CHATTING_ROOM_PAL_STYLE_COUNT)

//////////////////////////////////////////////////////////////////////////

#define WIID_CHATTING_ROOM_MODAL_PAL_SEARCH						(WIID_CHATTING_ROOM + 850)
#define WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_BG					(WIID_CHATTING_ROOM + 851)
#define WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_CLOSE				(WIID_CHATTING_ROOM + 852)
#define WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_TITLE				(WIID_CHATTING_ROOM + 853)
#define WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_LV_IMG				(WIID_CHATTING_ROOM + 854)
#define WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_LV_BG				(WIID_CHATTING_ROOM + 855)
#define WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_LV_EDIT_MIN			(WIID_CHATTING_ROOM + 856)
#define WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_LV_EDIT_MAX			(WIID_CHATTING_ROOM + 857)

#define WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_GENDER_IMG			(WIID_CHATTING_ROOM + 859)
#define WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_GENDER_CHECK_ALL	(WIID_CHATTING_ROOM + 860)
#define WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_GENDER_CHECK_MAN	(WIID_CHATTING_ROOM + 861)
#define WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_GENDER_CHECK_WOMAN	(WIID_CHATTING_ROOM + 862)
#define WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_GENDER_IMG_ALL		(WIID_CHATTING_ROOM + 863)
#define WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_GENDER_IMG_MAN		(WIID_CHATTING_ROOM + 864)
#define WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_GENDER_IMG_WOMAN	(WIID_CHATTING_ROOM + 865)

#define WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_STYLE_IMG			(WIID_CHATTING_ROOM + 866)
#define WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_SEARCH				(WIID_CHATTING_ROOM + 867)
#define WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_CANCEL				(WIID_CHATTING_ROOM + 868)

#define WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_LEFT				(WIID_CHATTING_ROOM + 870)
#define WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_RIGHT				(WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_LEFT			+ CHATTING_ROOM_PAL_STYLE_COUNT)
#define WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_IMG					(WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_RIGHT			+ CHATTING_ROOM_PAL_STYLE_COUNT)
#define WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_TEXT_BG_LEFT		(WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_IMG			+ CHATTING_ROOM_PAL_STYLE_COUNT)
#define WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_TEXT_BG_CENTER		(WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_TEXT_BG_LEFT	+ CHATTING_ROOM_PAL_STYLE_COUNT)
#define WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_TEXT_BG_RIGHT		(WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_TEXT_BG_CENTER	+ CHATTING_ROOM_PAL_STYLE_COUNT)
#define WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_TEXT				(WIID_CHATTING_ROOM_MODAL_PAL_SEARCH_TEXT_BG_RIGHT	+ CHATTING_ROOM_PAL_STYLE_COUNT)

//////////////////////////////////////////////////////////////////////////


enum CHATTING_ROOM_BG
{
	CHATTING_ROOM_BG_LEFT_TOP		=	0	,
	CHATTING_ROOM_BG_CENTER_TOP				,
	CHATTING_ROOM_BG_RIGHT_TOP				,

	CHATTING_ROOM_BG_LEFT_MIDDLE			,
	CHATTING_ROOM_BG_CENTER_MIDDLE			,
	CHATTING_ROOM_BG_RIGHT_MIDDLE			,

	CHATTING_ROOM_BG_LEFT_BOTTOM			,
	CHATTING_ROOM_BG_CENTER_BOTTOM			,
	CHATTING_ROOM_BG_RIGHT_BOTTOM			,

	CHATTING_ROOM_BG_LINE_1					,
	CHATTING_ROOM_BG_SUB					,

	CHATTING_ROOM_BG_COUNT					,
};

enum ROOM_STYLE_ID;
enum MY_STYLE_ID;
enum ROOM_TYPE_ID;
enum CHAT_LDT_TYPE;

enum SLIDER_SELECT
{
	SLIDER_SELECT_ROOM_FIND		=	0		,
	SLIDER_SELECT_PAL						,
	SLIDER_SELECT_MAX						,
};

typedef	std::map< int , CHATROOM_INFO >				STD_MAP_ROOM;
typedef std::vector< CHARACTER_BASICINFO >			STD_VECTOR_PAL;

class SPWindow;
class SPWindowButton;
class SPWindowList;
class SPFXArchive;

class SPWindowChattingRoom : public SPWindow
{
public:
	SPWindowChattingRoom									( WND_ID WndClassID , INSTANCE_ID InstanceID );
	SPWindowChattingRoom									( INSTANCE_ID InstanceID , int iX , int iY , int iCX , int iCY , SPWindow* pParent = NULL );
	virtual ~SPWindowChattingRoom							();

	virtual void			Init							();
	virtual void			Clean							();

	virtual void			Process							( float fTime );
	virtual void			Render							( float fTime );

	virtual void			Show							( bool bWithChild = true );
	virtual void			Hide							( bool bSendServer = true );
//	virtual void			Close							();

	virtual void			RefreshRelationCoord			();

private:
	void					ReposOwnImage					();

protected:

	// {{ Message Map	
	SPIMESSAGE_OBJ_MAP_DECLARE()

	//	Common
	virtual		int			OnPurge							( WPARAM wParam, LPARAM lParam );
	virtual		int			OnPasswordOK					( WPARAM wParam, LPARAM lParam );
	virtual		int			OnPasswordCancel				( WPARAM wParam, LPARAM lParam );
	virtual		int			OnNoticeYes						( WPARAM wParam, LPARAM lParam );
	virtual		int			OnNoticeNo						( WPARAM wParam, LPARAM lParam );
	virtual		int			OnJoinClear						( WPARAM wParam, LPARAM lParam );

	//	Packet
	virtual		int			OnPacketCreate					( WPARAM wParam, LPARAM lParam );
	virtual		int			OnPacketRoomCheck				( WPARAM wParam, LPARAM lParam );
	virtual		int			OnPacketJoin					( WPARAM wParam, LPARAM lParam );
	virtual		int			OnPacketRoomList				( WPARAM wParam, LPARAM lParam );
	virtual		int			OnPacketUserList				( WPARAM wParam, LPARAM lParam );
	virtual		int			OnPacketInviteReq				( WPARAM wParam, LPARAM lParam );
	virtual		int			OnPacketInviteAns				( WPARAM wParam, LPARAM lParam );
	virtual		int			OnPacketOtherPCInfo				( WPARAM wParam, LPARAM lParam );
	virtual		int			OnPacketMyStyle					( WPARAM wParam, LPARAM lParam );
	virtual		int			OnRefreshOption					( WPARAM wParam, LPARAM lParam );

	virtual		int			OnClose							( WPARAM wParam, LPARAM lParam );

	virtual		int			OnRoomMenu						( unsigned int iID, WPARAM wParam , LPARAM lParam );

	//	Room Make
	virtual		int			OnRoomMakeRun					( WPARAM wParam, LPARAM lParam );
	virtual		int			OnRoomMakeClose					( WPARAM wParam, LPARAM lParam );
	virtual		int			OnRoomMakeAdvUse				( WPARAM wParam, LPARAM lParam );
	virtual		int			OnRoomMakeTypeLeft				( WPARAM wParam, LPARAM lParam );
	virtual		int			OnRoomMakeTypeRight				( WPARAM wParam, LPARAM lParam );
	virtual		int			OnRoomMakeGenderLeft			( WPARAM wParam, LPARAM lParam );
	virtual		int			OnRoomMakeGenderRight			( WPARAM wParam, LPARAM lParam );
	virtual		int			OnRoomMakeAgeLeft				( WPARAM wParam, LPARAM lParam );
	virtual		int			OnRoomMakeAgeRight				( WPARAM wParam, LPARAM lParam );
	virtual		int			OnEditSetFocus					( WPARAM wParam, LPARAM lParam );
	virtual		int			OnEditKillFocus					( WPARAM wParam, LPARAM lParam );
	virtual		int			OnNumberEditKillFocus			( WPARAM wParam, LPARAM lParam );

	//	Room Find
	virtual		int			OnRoomFindClose					( WPARAM wParam, LPARAM lParam );
	virtual		int			OnRoomFindJoin					( WPARAM wParam, LPARAM lParam );
	virtual		int			OnRoomFindSliderUp				( WPARAM wParam, LPARAM lParam );
	virtual		int			OnRoomFindSliderDown			( WPARAM wParam, LPARAM lParam );
	virtual		int			OnRoomFindRefresh				( WPARAM wParam, LPARAM lParam );
	virtual		int			OnRoomFindSlider				( WPARAM wParam, LPARAM lParam );
	virtual		int			OnRoomFindTypeMenu				( unsigned int iID, WPARAM wParam , LPARAM lParam );
	virtual		int			OnRoomFindBar					( unsigned int iID, WPARAM wParam , LPARAM lParam );
	virtual		int			OnRoomFindBarDblClick			( unsigned int iID, WPARAM wParam , LPARAM lParam );
	virtual		int			OnRoomFindSliderWheelUp			( unsigned int iID, WPARAM wParam , LPARAM lParam );
	virtual		int			OnRoomFindSliderWheelDown		( unsigned int iID, WPARAM wParam , LPARAM lParam );
	virtual		int			OnRoomFindToolTipEnter			( unsigned int iID, WPARAM wParam , LPARAM lParam );
	virtual		int			OnRoomFindToolTipOut			( unsigned int iID, WPARAM wParam , LPARAM lParam );

	//	Room Pal
	virtual		int			OnRoomPalClose					( WPARAM wParam, LPARAM lParam );
	virtual		int			OnRoomPalRefresh				( WPARAM wParam, LPARAM lParam );
	virtual		int			OnRoomPalSearch					( WPARAM wParam, LPARAM lParam );
	virtual		int			OnRoomPalInvite					( WPARAM wParam, LPARAM lParam );
	virtual		int			OnRoomPalMyStyle				( WPARAM wParam, LPARAM lParam );
	virtual		int			OnRoomPalSliderUp				( WPARAM wParam, LPARAM lParam );
	virtual		int			OnRoomPalSliderDown				( WPARAM wParam, LPARAM lParam );
	virtual		int			OnRoomPalSlider					( WPARAM wParam, LPARAM lParam );
	virtual		int			OnRoomPalBar					( unsigned int iID, WPARAM wParam , LPARAM lParam );
	virtual		int			OnRoomPalSliderWheelUp			( unsigned int iID, WPARAM wParam , LPARAM lParam );
	virtual		int			OnRoomPalSliderWheelDown		( unsigned int iID, WPARAM wParam , LPARAM lParam );

	//	Room Setup
	virtual		int			OnRoomSetupSave					( WPARAM wParam, LPARAM lParam );
	virtual		int			OnRoomSetupClose				( WPARAM wParam, LPARAM lParam );
	virtual		int			OnRoomSetupMyInfoSearch			( WPARAM wParam, LPARAM lParam );
	virtual		int			OnRoomSetupChatRequest			( WPARAM wParam, LPARAM lParam );
	virtual		int			OnRoomSetupMyStyleOpen			( WPARAM wParam, LPARAM lParam );

	//	Modal MyStyle
	virtual		int			OnModalMyStyleClose				( WPARAM wParam, LPARAM lParam );
	virtual		int			OnModalMyStyleSave				( WPARAM wParam, LPARAM lParam );
	virtual		int			OnModalMyStyleArrowLeft			( unsigned int iID, WPARAM wParam , LPARAM lParam );
	virtual		int			OnModalMyStyleArrowRight		( unsigned int iID, WPARAM wParam , LPARAM lParam );

	//	Modal PalSearch
	virtual		int			OnModalPalSearchClose			( WPARAM wParam, LPARAM lParam );
	virtual		int			OnModalPalSearchSearch			( WPARAM wParam, LPARAM lParam );
	virtual		int			OnModalPalSearchGender			( unsigned int iID, WPARAM wParam , LPARAM lParam );
	virtual		int			OnModalPalSearchArrowLeft		( unsigned int iID, WPARAM wParam , LPARAM lParam );
	virtual		int			OnModalPalSearchArrowRight		( unsigned int iID, WPARAM wParam , LPARAM lParam );

	// }}

	void					InitWindowRoomMake				( SPWindow* pParentWindow );
	void					InitWindowRoomFind				( SPWindow* pParentWindow );
	void					InitWindowRoomPal				( SPWindow* pParentWindow );
	void					InitWindowRoomSetup				( SPWindow* pParentWindow );

	void					InitWindowWaitting				( SPWindow* pParentWindow );
	void					InitWindowStyle					( SPWindow* pParentWindow );
	void					InitWindowPalSearch				( SPWindow* pParentWindow );

	void					SetModalWindowShow				( SPWindow* pWindow , bool bShow );
	void					SetButtonCheck					( SPWindow* pWindow , bool bCheck );
	void					SetButtonCheckInv				( SPWindow* pWindow );

	void					SetViewTitle					( CHAT_LDT_TYPE eChatLDTType , int iStyleID , int& iIndex , SPWindow* pWindow );
	void					SetShowWindow					( SPWindow* pWindow , bool bShow );

	void					SetDefaultWindow				( INSTANCE_ID eInstanceID );
	void					SetDefaultWindowRoomMake		();
	void					SetDefaultWindowRoomFind		();
	void					SetDefaultWindowRoomPal			();
	void					SetDefaultWindowRoomSetup		();
	void					SetDefaultWindowModalStyle		();
	void					SetDefaultWindowModalPalSearch	();

	void					SetRoomMakeMax					( int iMax );

	INSTANCE_ID				GetActiveMenuWindow				( INSTANCE_ID eInstanceStart , INSTANCE_ID eInstanceEnd , int iStep , bool bShow );

	void					SetAutoRoomSubject				( SPWindow* pWindow , int iIndex );

	int						GetRequireItem					();

	void					UpdateRoomFindData				( ROOM_TYPE_ID eRoomTypeID );
	void					SetRoomFindData					( int iIndex , int iSelect , CHATROOM_INFO* pRoomFindData );
	bool					SetRoomFindRoomNo				( int iIndex );
	void					ClearRoomFindRoomNo				();

	int						GetSliderCurPage				( INSTANCE_ID eInstanceID );
	void					SetSliderCurPage				( INSTANCE_ID eInstanceID , int iScreenMaxCount , int iCurPage , int iMaxPage );
	void					SetSliderUpPage					( INSTANCE_ID eInstanceID );
	void					SetSliderDownPage				( INSTANCE_ID eInstanceID );

	void					UpdateRoomPalData				();
	void					SetRoomPalData					( int iIndex , int iSelect , CHARACTER_BASICINFO* pRoomPalData );
	void					SetRoomPalInfo					();
	void					ClearRoomPalData				();

	void					SetWaittingShow					( bool bShow , const char* pszMsg );

	void					PrintErrorMsg					( GLOBAL_STRING_ID uiGlobalStringID , const char* pszName = NULL );

	void					SendPacketRoomJoin				( UINT32 uiID , UINT32 uiRoomNo , UINT8	iCategory , TCHAR* pstrPassword , bool bLocalSend );
	bool					SendPacketPCInfo				( int iSelectIndex );
	void					SendPacketInviteAns				( UINT8 uiAllowed );

	void					AddRoomData						( int iKey , CHATROOM_INFO& stCharRoomInfo );
	CHATROOM_INFO*			GetRoomData						( int iKey , int iCategory );
	bool					GetRoomData						( STD_MAP_ROOM::iterator& mOutIter , STD_MAP_ROOM::iterator& mOutIterEnd , int iIndex , int iCategory );
	CHATROOM_INFO*			GetRoomData						( int iIndex , int iCurPage , int iCategory );
	void					ClearRoomData					( int iCategory );
	int						GetRoomDataCount				( int iCategory );

	void					UpdateWheel						();

	bool					IsPlayerAction					();


protected:

	//////////////////////////////////////////////////////////////////////////
	//
	//	변수들
	//

	SPTexture*								m_pBGTexture;
	RECT									m_rcBGDest[ CHATTING_ROOM_BG_COUNT ];
	RECT									m_rcBGSrc[ CHATTING_ROOM_BG_COUNT ];

	bool									m_bUse;

	SPWindow*								m_pRoomPalWindow;
	SPPlayer*								m_pPalPlayer;
	bool									m_bPalPlayerRender;

	int										m_iCurIndexRoomMake[ ROOM_STYLE_MAX ];
	int										m_iCurIndexMyStyle[ MY_STYLE_MAX ];
	int										m_iCurIndexPalSearchStyle[ MY_STYLE_MAX ];

	bool									m_bRoomDataZero[ ROOM_TYPE_MAX ];
	STD_MAP_ROOM							m_vRoomData[ ROOM_TYPE_MAX ];
	int										m_iCurRoomFindPage[ ROOM_TYPE_MAX ];

	int										m_iSelectIndex[ SLIDER_SELECT_MAX ];

	STD_VECTOR_PAL							m_vRoomPal;

	//////////////////////////////////////////////////////////////////////////

	float									m_fAccumulateTime;
	float									m_fAccumulateLimitTime;
	int										m_iFrameIndex;
	int										m_iFrameMax;

	//////////////////////////////////////////////////////////////////////////

	UINT8									m_iRoomPalSex;
	UINT16									m_iRoomPalMinLevel;
	UINT16									m_iRoomPalMaxLevel;
	PCSTATUS_STYLE							m_stRoomPalStyle;

	bool									m_bPalSytleOpen;
	int										m_iSelectPalStyle[ MY_STYLE_MAX ];

	PCSTATUS_STYLE							m_stRoomMyStyle;

	//////////////////////////////////////////////////////////////////////////
	//
	//	초대 관련 정보 버퍼
	//

	TCHAR									m_szActorName[ LEN_NAME + 1 ];
	UINT8									m_uiInviteInRoom;
	CHATROOM_INFO							m_stChatRoomInfo;

	//////////////////////////////////////////////////////////////////////////

	std::vector< GLOBAL_STRING_ID >			m_vPrintNameBuffer;

	//////////////////////////////////////////////////////////////////////////

	D3DXCOLOR								m_cRoomTextColor[ 2 ];

};

#endif // SPWindowChattingRoom_h__