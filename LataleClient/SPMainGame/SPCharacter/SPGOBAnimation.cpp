#include <vector>

#ifdef _LATALE_CLIENT
#include "SPCommon.h"
//#include "SPDebug.h"
#else 
#include "SPToolCommon.h"
#endif

#include "SPUtil.h"
#include "SPResourceDef.h"
#include "SPGOBClusterDef.h"
#include "SPGOBAnimation.h"



//-SPGOBAniSeqPart----------------------------------------------------------------------------
SPGOBAniSeqPart::SPGOBAniSeqPart()
{
	SetNull();
}

void SPGOBAniSeqPart::SetNull()
{
	PartLayer = PL_NULL;
	//iAniNum = 0;
	iRX = 0;
	iRY = 0;
	// 2004.11.4 added by kkandori
	//iResourceIndex = 0 ;
	iRotationDegree = 0 ;

	bVisable = true;
	bFlip = false;
	
	//
	//DXUTOutputDebugString("SPGOBAniSeqPart::SetNull() call\n");

#ifdef _CHARTOOL	
	bSelect = false;
#endif

}

bool SPGOBAniSeqPart::isNull()
{
	return (PartLayer == PL_NULL);
}

// added by kkandori (11.15.2004)
SPGOBAniSeqPart& SPGOBAniSeqPart::operator=(const SPGOBAniSeqPart &src)
{
	PartLayer		= src.PartLayer;	
	iResourceIndex	= src.iResourceIndex;
	iRotationDegree	= src.iRotationDegree;
	iRX				= src.iRX;
	iRY				= src.iRY;
	bVisable		= src.bVisable;
	bFlip			= src.bFlip;

#ifdef _CHARTOOL	
	bSelect			= src.bSelect;
#endif

	return *this;
}


//-SPGOBAniSeq----------------------------------------------------------------------------
SPGOBAniSeq::SPGOBAniSeq()
{
	SetNull();
}

void SPGOBAniSeq::SetNull()
{
	vAniSeqPart.clear();
	iSeqNum = 0;
	fDelayTime = 0;
}

bool SPGOBAniSeq::isNull()
{
	return (iSeqNum == 0 && vAniSeqPart.empty());
}

// added by kkandori (11.15.2004)
SPGOBAniSeq& SPGOBAniSeq::operator=(const SPGOBAniSeq &src)
{
	vAniSeqPart		= src.vAniSeqPart;
	iSeqNum			= src.iSeqNum;
	fDelayTime		= src.fDelayTime;

	return *this;
}


//-SPGOBAniSeq----------------------------------------------------------------------------
SPGOBAnimation::SPGOBAnimation()
{
	SetNull();
}

void SPGOBAnimation::SetNull()
{
	vAniSeq.clear();
	uiAniInx = 0;
	fAccumulateTime = 0.0f;
}

bool SPGOBAnimation::isNull()
{
	return uiAniInx == 0;
}

// added by kkandori (11.15.2004)
SPGOBAnimation& SPGOBAnimation::operator=(const SPGOBAnimation &src)
{
	vAniSeq			= src.vAniSeq;
	uiAniInx		= src.uiAniInx;	
	fAccumulateTime	= src.fAccumulateTime;

	return *this;
}








