
// Copyright (C) metalgeni
//***************************************************************************
// System Name : SPCommandConvert
// Comment     : 
// Creation    : metalgeni 2004-12-03 오후 2:56:36
//***************************************************************************

#pragma once 

#define		ACTION_NULL						(0)
#define		ACTION_STOP						(0)
#define		ACTION_VERTICAL_STOP			((unsigned __int64)1<<0)
#define		ACTION_HANGING_STOP				((unsigned __int64)1<<1)
#define		ACTION_MOVE_LEFT				((unsigned __int64)1<<2)
#define		ACTION_MOVE_RIGHT				((unsigned __int64)1<<3)
#define		ACTION_MOVE_JUMP				((unsigned __int64)1<<4)
#define		ACTION_MOVE_DROP				((unsigned __int64)1<<5)
#define		ACTION_MOVE_LAND				((unsigned __int64)1<<6)
#define		ACTION_MOVE_UP					((unsigned __int64)1<<7)
#define		ACTION_MOVE_DOWN				((unsigned __int64)1<<8)
#define		ACTION_MOVE_HANGING_LEFT		((unsigned __int64)1<<9)
#define		ACTION_MOVE_HANGING_RIGHT		((unsigned __int64)1<<10)
#define		ACTION_RUN_LEFT					((unsigned __int64)1<<11)
#define		ACTION_RUN_RIGHT				((unsigned __int64)1<<12)
#define		ACTION_RUN_STOP					((unsigned __int64)1<<13)
#define		ACTION_ATTACK_SPEEDY			((unsigned __int64)1<<14)
#define		ACTION_ATTACK_MIGHTY			((unsigned __int64)1<<15)
#define		ACTION_BEATTACKED				((unsigned __int64)1<<16)
#define		ACTION_CRITICAL_BEATTACKED		((unsigned __int64)1<<17)
#define		ACTION_GUARD					((unsigned __int64)1<<18)
#define		ACTION_DEAD						((unsigned __int64)1<<19)
#define		ACTION_LOOTING					((unsigned __int64)1<<20)
#define		ACTION_LOOTING_WAIT				((unsigned __int64)1<<21)
#define		ACTION_LOOTING_STAND			((unsigned __int64)1<<22)
#define		ACTION_SIT_DOWN					((unsigned __int64)1<<23)
#define		ACTION_SIT_WAIT					((unsigned __int64)1<<24)
#define		ACTION_SIT_STAND				((unsigned __int64)1<<25)
#define		ACTION_BEATTACKED_DOWN			((unsigned __int64)1<<26)
#define		ACTION_BEATTACKED_DOWN_STAND	((unsigned __int64)1<<27)
#define		ACTION_SKILL					((unsigned __int64)1<<28)
#define		ACTION_GUARD_DAMAGE				((unsigned __int64)1<<29)
#define		ACTION_BLOCK					((unsigned __int64)1<<30)
#define		ACTION_ENCHANTTING				((unsigned __int64)1<<31)
#define		ACTION_ENCHANT_RESULT			((unsigned __int64)1<<32)
#define		ACTION_USERSHOP					((unsigned __int64)1<<33)

namespace command_convert {
/*
 *	사용 예 
 *  int iResultCommandBuf;
 *	command_convert::Encode( ACTION_MOVE_LEFT | ACTION_MOVE_JUMP, ACTION_MOVE_RIGHT, iResultCommandBuf);
 */
bool Encode(UINT uiEnableCommandList, UINT uiDisableCommandList, UINT& uiResultCommandBuf);
bool Decode(UINT uiCommandBuf, UINT& uiResultEnableList, UINT& uiResultDisableList);

}








