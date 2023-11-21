// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-9-23    11:56 
//***************************************************************************

#pragma once

#define QUEST_COUNT_MAX		999
#define QUEST_LENGTH_MAX	3

enum QUEST_NPC;

class SPUIUnit;
class SPQuestArchive;

class SPUIQuestUnit : public SPUIUnit
{
public:
	SPUIQuestUnit								(void);
	virtual ~SPUIQuestUnit						(void);

	//	기본 인터페이스
	virtual	void			Init				();
	virtual	void			Clear				();
	virtual	void			Process				( float fTime );
	virtual	void			Render				();
	virtual	BOOL			Use					();

	//	퀘스트 아이디 인터페이스
	void					SetQuestID			( int iQuestID );
	int						GetQuestID			();

	//	퀘스트 타입 인터페이스
	void					SetQuestType		( QUEST_NPC eQuestType );
	QUEST_NPC				GetQuestType		();

	//	퀘스트 아카이브 인터페이스
	void					SetQuestArchive		( SPQuestArchive* pQuestArchive );
	SPQuestArchive*			GetQuestArchive		();

	//	자기 자신 복사 인터페이스
	virtual SPUIUnit*		ClonePtr			();
	virtual BOOL			Copy				( SPUIUnit** ppDestUIUnit );

	//	아이디로 리소스 재 로드
	virtual	BOOL			ReResourceLoad		();

	//	퀘스트 완료 텍스처 인터페이스
	void					SetCompleteTexture	( SPTexture* pTexture );
	BOOL					SetCompleteTexture	( const char* pstrTexture );
	SPTexture*				GetCompleteTexture	();
	
	//	퀘스트 완료 텍스처 영역 인터페이스
	void					SetCompleteRectSrc	( RECT rtSrc );
	void					SetCompleteRectSrc	( int ixLeft , int iyTop , int ixRight , int iyBottom );
	RECT					GetCompleteRectSrc	();

	//	퀘스트 완료 인터페이스
	void					SetComplete			( BOOL bComplete );
	BOOL					GetComplete			();

	//	퀘스트 횟수 체크 인터페이스
	void					SetCount			( int iCount );
	int						GetCount			();

protected:

	//	퀘스트 횟수 프로세서 인터페이스
	void					ProcessCount		();

	//	퀘스트 횟수 렌더 인터페이스
	void					RenderCount			();

	//////////////////////////////////////////////////////////////////////////
	//
	//	변수들
	//

	int						m_iQuestID;
	QUEST_NPC				m_eQuestType;
	SPQuestArchive*			m_pQuestArchive;

	SPTexture*				m_pCompleteTexture;
	RECT					m_rtCompleteSrc;
	BOOL					m_bComplete;

	int						m_iCount;

	int						m_iCountLength;

	RECT					m_rtCountSrc[ QUEST_LENGTH_MAX ];
	POINT					m_ptCountPos[ QUEST_LENGTH_MAX ];

};
