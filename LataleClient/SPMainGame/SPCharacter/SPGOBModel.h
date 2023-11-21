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

// 시퀀스에서 읽어온다 (쿼리할때 마다 바뀐다)
struct SPGOBInfoSequencePart {
	PART_LAYER	PartLayer;	
	int			iResourceIndex;
	int			iRotationDegree;
	int			iRX;
	int			iRY;
	bool		bVisable;
	bool		bFlip;
#ifdef _CHARTOOL	
	bool			bSelect;
#endif
	SPGOBInfoSequencePart();
	void SetNull();	
	bool operator==(const SPGOBInfoSequencePart CompObj);	

	// added by kkandori (11.15.2004)
	SPGOBInfoSequencePart& operator=(const SPGOBInfoSequencePart &src);
};

struct SPGOBModelPart {
	PART_LAYER	PartLayer;
	EQUIP_CLASS	EquipClass;
	int			iItemID;
	int			iColor;

	int			iRotationDegree;
	RECT		SrcRect;
	RECT		TarRect;

#if defined(_USED_TEXTURE_TABLE)
	TEXTURE_TABLE_INFO* pTableInfo;
	TEXTURE_TABLE_INFO* pTableInfoReserve;
	FRAME_INFO* pFrameInfo;
#endif

	int			iRenderType;
	SPTexture*	pPartImage;	
	SPTexture*	pPartImageReserve;
	bool		ReloadTexture(TCHAR	*strImagePath=NULL);	
	void		Render();

	SPGOBModelPart();
	~SPGOBModelPart();
	void SetNull();
	bool isNull();	

	// added by kkandori (11.15.2004)
	SPGOBModelPart& operator=(const SPGOBModelPart &src);
};

struct SPGOBModel {
	std::vector<SPGOBInfoSequencePart>		vGOBInfoSequencePart;	// 쿼리해오는 애니메이션 시퀀스 정보
	float						fDelayTime;

	unsigned int 				uiAniIndex;
	int							iAniSequence;	// 0 BASE
	int							iMaxSequence;	// size()
	SPGOBModelPart				PartModel[PL_LAST];

	// 현재 PC는 머리만 쓰이기 때문에 현재모델에서 쓰이는지 만 알면 된다.
	bool						bPCEnable;		

	SPGOBModel();
	void SetNull();
	void Destroy();

	// added by kkandori (11.15.2004)
	SPGOBModel& operator=(const SPGOBModel &src);
};





