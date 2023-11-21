#pragma once
#include <SPMonster.h>
//--------------------------------------------------
class SPTransformMonster : public SPGameObject
{
public:
	SPTransformMonster( void );
	SPTransformMonster( INSTANCE_ID instanceID, CLASS_ID ClassID );
	virtual ~SPTransformMonster( void );

	virtual void Process(float fTime);
	virtual void Render(float fTime);
	virtual void Init( void );
	virtual void Destroy( void );

	void Reload( void );
	void SetAnimationLoop( void );
	void SetState( GOB_STATE_TYPE state );
	void SetAction( UINT64 uiAction );
	FACING GetDirection( void ) { return m_eFacing; }
	//--------------------------------------------------
	SPGOBModelUnit*			m_pkModelUnit;
	GOB_STATE_TYPE			m_eCurState;
	UINT64 m_uiCurAction;
	FACING m_eFacing;
	bool	m_bFly;
	//--------------------------------------------------
	SPGMESSAGE_OBJ_MAP_DECLARE()
	virtual int OnGetFacing(LPARAM lParam);
};
//--------------------------------------------------
// end of file
