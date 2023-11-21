// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-9-29    11:31 
//***************************************************************************

#pragma once

class SPUIUnit;

class SPUIGOBUnit : public SPUIUnit
{
public:
	SPUIGOBUnit									(void);
	virtual ~SPUIGOBUnit						(void);

	//	기본 인터페이스
	virtual	void			Init				();
	virtual	void			Clear				();
	virtual	void			Process				( float fTime );
	virtual	void			Render				();
	virtual	BOOL			Use					();

	//	Class 아이디 인터페이스
	void					SetClassID			( CLASS_ID eClassID );
	CLASS_ID				GetClassID			();

	//	자기 자신 복사 인터페이스
	virtual SPUIUnit*		ClonePtr			();
	virtual BOOL			Copy				( SPUIUnit** ppDestUIUnit );

	//	아이디로 리소스 재 로드
	virtual	BOOL			ReResourceLoad		();

	//	GOB 이름 인터페이스
	void					SetName				( const char* pstrName );
	const char*				GetName				();

protected:

	//////////////////////////////////////////////////////////////////////////
	//
	//	변수들
	//

	CLASS_ID				m_eClassID;
	std::string				m_strName;

};
