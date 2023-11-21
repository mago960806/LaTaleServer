

// Modify by kkandori (11.9.2004)
#ifdef _LATALE_CLIENT
	#include "SPCommon.h"
	#include "SPGameObjectDEF.h"
	#include "SPGameObject.h"
	#include "SPRenderModelDEF.h"
	#include "SPGOBModelUnit.h"
#else 
	#include "SPToolCommon.h"
	#include "SPRenderModelDEF.h"
	#include "SPGameObjectDEF.h"	
	#include "SPGOBClusterDef.h"
	#include "SPAvatarModelUnitDef.h"		
	#include "SPGameMessageDEF.H"
	#include "SPGOBModel.h"
	#include "SPGameObjectToolCustom.h"	
	#include "SPGOBModelUnit.h"
#endif
// end

SPGOBModelUnit::SPGOBModelUnit(SPGameObject* pParent, MODEL_CLASS_ID ClassID)
: m_pParent		(pParent)
, m_ClassID		(ClassID)
{}

SPGOBModelUnit::~SPGOBModelUnit()
{
}


LRESULT SPGOBModelUnit::SPSendMessage(MODEL_VALUE_TYPE mvMsg, WPARAM wParam, LPARAM lParam)
{
	switch(mvMsg) {
	case MV_CLASSID:
		return (LRESULT)GetClassID();

	case MV_PARENT_GOB:
		return (LRESULT)m_pParent;
	}
	return -1;
}

