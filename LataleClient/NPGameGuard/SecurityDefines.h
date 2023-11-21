#pragma once

#ifdef _LATALE_SERVER
	// XTrap
	#include "XTrap/XTrap4Server.h"
	// nProtect
	#include "nProtect/ggsrv25.h"
	// CrackProof
	#include "CrackProof/HtGECASrv.h"
#elif _LATALE_CLIENT
	// XTrap
	#include "./XTrapLib/XTrap4Client.h"
	// nProtect
	#include "NPGameLib.h"
	// CrackProof
	#include "./CrackProof/HtGECACln.h"
#endif

#define SECURITY_CNT_DATA							3			// 보안툴검사용 데이터를 로드할수있는 최대수. 더 증가하면(클라이언트 패치가 3번이상이면) 서버를 재시작해야함.
#define SECURITY_MAX_PATH							1024
#define SECURITY_REGULAR_COUNT						3			// KeepAlive패킷(10초간격)이 검사주기(1분)동안 몇회이상이면 정상인지에 대한 판단값
#define SECURITY_UPDATE_ACTIVEUSERCOUNT				100			// 보안모듈을 업데이트하기위한 3가지 조건 중 최신보안모듈을 적용하여 접속한 유저의 수.(동접자의 1/3권장)
#define SECURITY_UPDATE_CHECKTIME					30			// 보안모듈을 업데이트하기위한 3가지 조건 중 통계데이터를 수집하는 시간. 분단위
#define SECURITY_UPDATE_ACTIVEUSERPERCENT			50			// 보안모듈을 업데이트하기위한 3가지 조건 중 현재 접속유저 중 최신보안모듈을 적용하여 접속한 유저의 %.
#define SECURITY_TIME_TIMEOUT						300			// 300초. 클라이언트 응답을 기다리는 최대값. 초단위 -- 테스트결과 검사주기가 1분인 경우는 100으로 해야 5분후에 에러검출되었음. 검사주기가 20초이면 300으로해야 5분후 에러검출.
#define SECURITY_TIME_CHECK							10*1000		// 10초. 초기검증을 위한 검사주기. 초기검증이 완료되면 각모듈별 검사주기로 변경해야합니다.
#define SECURITY_TIME_CHECK_XTRAP					20*1000		// 20초. XTrap 검사주기
#define SECURITY_TIME_CHECK_NPROTECT				5*60*1000	// 5분. nProtect 검사주기
#define SECURITY_TIME_UPDATE						5*60*1000	// 5분
#define SECURITY_TIME_RELOAD						9*60*1000	// 9분
#define SECURITY_TIME_SWAP							1*60*1000	// 1분. 리로드쓰레드 호출후 1분후 메모리스왑시도. 리로드시간 총10분.
#define SECURITY_SHAKE_CNT							2			// 10초주기로 2회까지의 송수신완료를 검사합니다. 이후 각모듈별 검사주기로 변경해야합니다.
#define SECURITY_CPF_DATA_SIZE						512
#define SECURITY_CPF_FULL_SIZE						SECURITY_CPF_DATA_SIZE + sizeof(HtGECAData) - 1

enum SECURITY_MODE { SECURITY_MODE_NONE = 0, SECURITY_MODE_XTRAP, SECURITY_MODE_NPROTECT, SECURITY_MODE_CRACKPROOF, SECURITY_MODE_MAX };
enum SECURITY_RELOAD { SECURITY_RELOAD_NONE = 0, SECURITY_RELOAD_DOING, SECURITY_RELOAD_NEW, SECURITY_RELOAD_FAILED, SECURITY_RELOAD_MAX };

typedef struct _tagSECURITY_PACKDATA
{
	unsigned char	arrBufPackData[XTRAP_CS4_COMSIZE_PACKDATA];
	GG_AUTH_DATA	stAuthData;
	BYTE			arrCPFData[SECURITY_CPF_FULL_SIZE];

} SECURITY_PACKDATA, *PSECURITY_PACKDATA;
