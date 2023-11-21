// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment : 
// Creation : metalgeni 2005-04-7 오후 1:30:38
//***************************************************************************


#pragma once

#define CHEAT_STRING_MAX 256


/*
	Cheat Command Format

	<Cheat Command>	::= <Command>{<whitespace>|<comma>}<ParameterList>
	<ParamterList>	::= <Param1>[{<whitespace>|<comma>}<ParameterList>]

	
	EX)	warp 100 100

*/
/*
 *	history

	7/13 
		goto 추가 : 해당 캐릭터 근처로 이동
		haul 추가 : 해당 캐릭터를 자신의 근처로 소환
		where 수정 : 다른 채널상의 해당 캐릭터에 대한 지원 추가. 채널 정보만 유효

		* charName에 @(본인 캐릭터) 사용 가능 

 */

//////////////////////////////////////////////////////////////////////////
// TODO : Cheat list, command

// Server Side
// "warp" X Y : X,Y로 워프한다.

// portal charName,posX,posY[,stageID,mapGroupID]		: posX,posY로 포털
// hp charName,HP[,maxHP]								: HP 조정
// sp charName,SP[,maxSP]								: SP 조정
// ely charName,value									: Ely 조정
// Exp charName,value									: EXP 조정
// SkillPoint charName,value							: Skill Point 조정
// Level charName,value									: Level 조정
// GiveItem charName,itemID[,amount,rare]				: Item 강제 획득
// LearnSkill charName,skillID							: Skill 강제 획득
// AddTitle charName, TitleID							: 타이틀 강제 획득
// ExtendSlot charName, ContainerType, OptionalCount	: 컨테이나 확장.
// Speedup charname,x_Delta[,y_Delta]					: 캐릭터 속도 증감. 예) x_Delta = 100일경우, 기존 속도를 100% 시키라는 의미
// MapEvent CharName,mapEventType,mapEventID			: 해당 mapEvent로 이동
// Notice displayTime displayOption message				: displayTime = displayOption = 0(reserved). 메세지는 중간에 공백없게, 개행문자(\n) 가능. 
//														:EX) notice 0 0 서버다운예정\n로그아웃바랍니다.
// ServerStatus charName								: 서버 상태 얻기. FTP,nPC,nMOB,nLootingBag 정보
// Where charName targetName							: targetName 캐릭터 위치 얻기. charName 유저에게 알려줌
// Version charName										: 서버 버전 정보얻기. 정보를 charName 유저에게 전송

// goto			<charName>								// 해당 캐릭터 위치로 자신이 이동
// haul			<charName>								// 해당 캐릭터를 소환(자신의 위치로 이동) 시킴(데려옴)
// Where		[<TargetCharName> | "@"]				// return ="<targetName> [stageID,mapGroupID] posX=[%d] posY=[%d]"

// Client Side

// bgm [0,1]											: BGM On/Off
// cls													: 채팅창 내용 모두 지우기
// CLS													: 채팅창 내용 모두 지우기
// stat													: 기본 스탯 보여주기
// stat_ex												: 확장 스탯 보여주기
// fxg													: FX 그룹 보여주기
// se	[0,1]											: Sound Effect On/Off
// bgmvol	[0.0 ~ 1.0]									: BGM 볼륨
// sevol	[0.0 ~ 1.0]									: SE 볼륨
// TakeShot												: 스크린샷 찍기
// contrast [0.0 ~ 1.0]									: 풀스크린일때 감마 조절
// SoundStatus											: 모든 사운드 스테이터스 보여주기

// BSON													: Beautyshop 창 띄우기
// LDTTest												: LDT파일 저장 테스트




