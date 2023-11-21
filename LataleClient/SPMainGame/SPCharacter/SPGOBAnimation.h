// Copyright (C) metalgeni
//*****************************************************************************************************************************
// System Name : 
// Comment     : 
// Creation    : metalgeni 2004-08-19 오전 10:26:19
//
// modified history :
//
//					metalgeni 2004/10/12 ---------------------------------------------------------------------
//					removed SPGOBPartPool
//					removed SPGOBPart
//
//					appended
//					SPGOBInfoSequencePart	- 애니메숑 파트 시퀀스 정보
//					SPGOBPartModel			- 파츠고유번호, 색깔번호, 텍스쳐등 아이템에 따른 파트정보
//
//
//					metalgeni 2004/11/5 ---------------------------------------------------------------------
//					SPGOBClusterDEF에 있는 애니메이션과 모델을 두개의 모듈로분리.
//					SPGOBClusterDEF는 그대로 존재한다.
//					SPGOBAnimation	- 애니메이션에 관련된 모듈
//					SPGOBModel		- Part정보와 모델정보에 관련된 모듈
//										모듈만 분리되었을뿐 구조가 변경된건 아님.
//										단 SPGOBModel은 struct에서 class로 변경.
//
//					
//*****************************************************************************************************************************

#pragma once

struct SPGOBAniSeqPart {
	PART_LAYER		PartLayer;	
	int				iResourceIndex;
	int				iRotationDegree;
	int				iRX;
	int				iRY;
	bool			bVisable;
	bool			bFlip;
#ifdef _CHARTOOL	
	bool			bSelect;
#endif
	SPGOBAniSeqPart();
	void SetNull();
	bool isNull();

	// added by kkandori (11.15.2004)
	SPGOBAniSeqPart& operator=(const SPGOBAniSeqPart &src);
};

struct SPGOBAniSeq {
	std::vector<SPGOBAniSeqPart>	vAniSeqPart;
	int		iSeqNum;				// 1 BASE
	float	fDelayTime;

	SPGOBAniSeq();
	void SetNull();
	bool isNull();	

	// added by kkandori (11.15.2004)
	SPGOBAniSeq& operator=(const SPGOBAniSeq &src);
};

struct SPGOBAnimation {
	std::vector<SPGOBAniSeq>		vAniSeq;
	unsigned int 					uiAniInx;	
	float							fAccumulateTime;

	SPGOBAnimation();
	void SetNull();
	bool isNull();	

	// added by kkandori (11.15.2004)
	SPGOBAnimation& operator=(const SPGOBAnimation &src);
};








