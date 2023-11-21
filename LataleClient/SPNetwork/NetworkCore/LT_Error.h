// LT_Error.h

#pragma  once

// typedef UINT8	RESULTCODE

enum
{
	LT_NO_ERROR							= 0,	// 성공
	LT_ERROR_FAILED						= 1,	// 실패

	LT_ERROR_DUPLICATED					= 2,	// 중복 로긴								(변경 불허 : sp_authUser)
	LT_ERROR_PWMISMATCH					= 3,	// 암호 불일치								(변경 불허 : sp_authUser)
	LT_ERROR_UNAVAILABLEID				= 4,	// 없는 ID									(변경 불허 : sp_authUser)
	LT_ERROR_DISABLEDID					= 5,	// 사용 정지된 계정							(변경 불허 : sp_authUser)

	LT_ERROR_SYSTEMERROR				= 6,	// 시스템 에러
	LT_ERROR_DBERROR					= 7,	// DB 관련 에러								(변경 불허 : sp_RegisterCharName,AddChar,setPC)
	LT_ERROR_PERMISSION					= 8,	// 퍼미션 불가(방장퍼미션 필요)				(변경 불허 : sp_DeleteChar)
	LT_ERROR_ALREADYJOINED				= 9,	// 이미 방에 입장한 유저

	LT_ERROR_NEW						= 10,	// new 할당 에러
	LT_ERROR_INVALID_FILENAME			= 11,	// 맵 파일명 에러

	LT_ERROR_UNKNOWN					= 12,	// 알수 없는 에러


	LT_ERROR_INVALID_ACTION				= 13,	// 현재 상태에서 허용되지 않는 action 시도
	LT_ERROR_INVALID_TARGET				= 14,	// 잘못된 타겟

	LT_ERROR_USERFULL					= 15,	// 유저 full

	LT_ERROR_CHARNAME_RESERVED			= 16,	// 예약됨			(캐릭터 생성중 오류)	(변경 불허 : sp_CheckCharName)
	LT_ERROR_CHARNAME_USED				= 17,	// 이미 사용중		(캐릭터 생성중 오류)	(변경 불허 : sp_CheckCharName,AddChar,setPC)

	LT_ERROR_LEN_CHARNAME				= 18,	// 캐릭터 이름 길이 오류 (CharName:2~16)

	// 2007-5-3 by hoshim : ID/PW 길이 관련된 종속성 배제 목적
	//LT_ERROR_LEN_ID						= 19,	// ID 길이 오류 (ID:2~12)
	//LT_ERROR_LEN_PW						= 20,	// PW 길이 오류 (PW:6~12)

	LT_ERROR_INVALID_ITEM_ID			= 21,	// invalid item ID

	LT_ERROR_WEARABLE_MISMATCH_GENDER	= 22,	// 해당 성별이 착용할 수 없는 아이템
	LT_ERROR_WEARABLE_NOT_ENOUGH_LEVEL	= 23,	// 레벨 불충분

	LT_ERROR_DISTANCE					= 24,	// 거리 관련 오류.    LoogtingBag(50),NPC 거래시(500)

	LT_ERROR_NOT_FOUND					= 25,	// Not Found
	LT_ERROR_INVALID_PARAM				= 26,
	LT_ERROR_ALREADY_EXIST				= 27,	// find 결과 이미 존재
	LT_ERROR_EXCEED_LIMIT				= 28,	// 더 이상 등록 불가(Friend/Block 목록이 꽉참)

	LT_ERROR_BLOCKED					= 29,	// GMTool의 필요에 의해 해당 계정의 로긴 불허 	(변경 불허 : sp_authUser)
	LT_ERROR_REQUIREAGREEMENT			= 30,	// 액토즈계정은 있으나 라테일게임에 가입하지 않은 유저들

	LT_ERROR_TIMEOUT					= 31,	// Timeout !!!

	// insert here!!!
	LT_ERROR_EXCEED_CHAR_COUNT			= 40,	// 생성 가능 캐릭터 수 초과

	LT_ERROR_INVALID_CHARACTER			= 50,	// mask된 문자( ~!@#$%^&*()+-=|\\{}[]:;\"\'`<>.,?/)를 포함

	LT_ERROR_INVALID_ID_PW				= 51,	// ID/암호 오류 (MW 에서 올려줌)
	LT_ERROR_INVALID_EKEY_ID_PW			= 52,	// 잘못된 E-key Code나 암호
	
	// 2007.05.13 dhpark 인던입장시 오류
	LT_ERROR_INVALID_INDUNID			= 61,	// 인던서버에서 해당 인던을 못찾은 경우
	LT_ERROR_INVALID_DEAD				= 62,	// 죽은상태로 인던서버로 진입하는 경우
	LT_ERROR_INVALID_SERVERTYPE			= 63,	// 인던서버나 채널서버가 아닌 경우

	LT_ERROR_OSP_CANNOT_CONNECT			= 100,		// OSP 서버 미연결
	LT_ERROR_OSP_PACKET_FORMAT			= 101,		// OSP 연동 패킷 포맷 오류
	LT_ERROR_OSP_FAILED					= 102,		// OSP 연동 관련 오류
	LT_ERROR_OSP_TIMEOUT				= 103,		// OSP 연동 timeout

	LT_ERROR_EKEY_USERLOCKED			= 104,		// user_locked (5회 이상 EKey 오류시 5분간 Lock됨)
	
	LT_ERROR_VERSION_LOWER				= 200,		// Protocol Version 체크 : 현재 클라이언트 버전이 낮음
	LT_ERROR_VERSION_UNSURPORTED_OPTION = 201,		// Protocol Version 체크 : 미지원 Option value

	LT_ERROR_RESOURCE_VERSION_LOWER		= 202,		// 리소스 버전 체크 : NEED Patch

	LT_ERROR_SERVICE_NOT_READY			= 254,		// 서비스 점검중(임시/정기 점검시 사용)

	MAX_LT_ERROR						= 255,
};




//-----------------------------------------------------------------------------
// Error Code Definitions
//-----------------------------------------------------------------------------

// SUCCEEDED(hr) or FAILED(hr)
// int iErrCode = HRESULT_CODE(hr);
#ifndef HRESULT_CODE
	#define HRESULT_CODE(hr)				((hr) & 0xFFFF)
	#define HRESULT_FACILITY(hr)			(((hr) >> 16) & 0x1fff)
	#define HRESULT_SEVERITY(hr)			(((hr) >> 31) & 0x1)
#endif


#ifndef MAKE_HRESULT
	#define MAKE_HRESULT(sev,fac,code) \
	((HRESULT) (((unsigned long)(sev)<<31) | ((unsigned long)(fac)<<16) | ((unsigned long)(code))) )
#endif

#define FACILITY_LT							0x900              // LT Error Facility Code

#define MAKE_LT_HRESULT(code)				MAKE_HRESULT(1,FACILITY_LT,code)
#define MAKE_LT_STATUS(code)				MAKE_HRESULT(0,FACILITY_LT,code)


#define LT_ERR_INVALIDFORMAT				MAKE_LT_HRESULT(1)
#define LT_ERR_INVALIDPARAMS				MAKE_LT_HRESULT(2)
#define LT_ERR_OUTOFMEMORY					MAKE_LT_HRESULT(3)
#define LT_ERR_ACCESSDENIED					MAKE_LT_HRESULT(4)
#define LT_ERR_FILENOTOPEN					MAKE_LT_HRESULT(5)

