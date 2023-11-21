// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : 
// Creation    : AJJIYA 2005-9-23    11:58 
//***************************************************************************

#include "SPCommon.h"
#include "SPUIUnit.h"

#include "SPQuestManager.h"
#include "SPQuestArchive.h"
#include "SPQuestAttr.h"
#include "SPItemCluster.h"

#include "SPUIQuestUnit.h"

SPUIQuestUnit::SPUIQuestUnit()
{
	Init();
}

SPUIQuestUnit::~SPUIQuestUnit()
{
	Clear();
}

//////////////////////////////////////////////////////////////////////////
//
//	기본 인터페이스
//

void SPUIQuestUnit::Init()
{
	SPUIUnit::Init();

	m_iQuestID				=	0;
	m_eQuestType			=	QUEST_NPC_REWORD;
	m_pQuestArchive			=	NULL;

	m_pCompleteTexture		=	NULL;

	m_rtCompleteSrc.left	=	0;
	m_rtCompleteSrc.top		=	0;
	m_rtCompleteSrc.right	=	0;
	m_rtCompleteSrc.bottom	=	0;

	m_bComplete				=	FALSE;

	m_iCount				=	0;

	m_iCountLength			=	0;

	for( int i = 0 ; i < QUEST_LENGTH_MAX ; i++ )
	{
		m_rtCountSrc[ i ].left		=	0;
		m_rtCountSrc[ i ].top		=	0;
		m_rtCountSrc[ i ].right		=	0;
		m_rtCountSrc[ i ].bottom	=	0;

		m_ptCountPos[ i ].x			=	0;
		m_ptCountPos[ i ].y			=	0;
	}
}

void SPUIQuestUnit::Clear()
{
	SPUIUnit::Clear();

	SAFE_RELEASE( m_pCompleteTexture );
}

void SPUIQuestUnit::Process( float fTime )
{
	SPUIUnit::Process( fTime );
}

void SPUIQuestUnit::Render()
{
	if( IsTexture() == FALSE || IsRender() == FALSE || g_pVideo == NULL )
		return;

	g_pVideo->Flush();

	SPUIUnit::Render();

	g_pVideo->Flush();

	RenderCount();

	g_pVideo->Flush();

	if( GetComplete() == TRUE && GetCompleteTexture() != NULL )
	{
		m_pCompleteTexture->RenderTexture( m_ptPos.x , m_ptPos.y , &m_rtCompleteSrc );
		g_pVideo->Flush();
	}
}

BOOL SPUIQuestUnit::Use()
{
	if( SPUIUnit::Use() == FALSE )
		return FALSE;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//
//	퀘스트 아이디 인터페이스
//

void SPUIQuestUnit::SetQuestID( int iQuestID )
{
	m_iQuestID	=	iQuestID;
}

int SPUIQuestUnit::GetQuestID()
{
	return m_iQuestID;
}

//////////////////////////////////////////////////////////////////////////
//
//	퀘스트 타입 인터페이스
//

void SPUIQuestUnit::SetQuestType( QUEST_NPC eQuestType )
{
	m_eQuestType	=	eQuestType;
}

QUEST_NPC SPUIQuestUnit::GetQuestType()
{
	return m_eQuestType;
}

//////////////////////////////////////////////////////////////////////////
//
//	퀘스트 아카이브 인터페이스
//

void SPUIQuestUnit::SetQuestArchive( SPQuestArchive* pQuestArchive )
{
	m_pQuestArchive	=	pQuestArchive;
}

SPQuestArchive* SPUIQuestUnit::GetQuestArchive()
{
	return m_pQuestArchive;
}

//////////////////////////////////////////////////////////////////////////
//
//	자기 자신 복사 인터페이스
//

SPUIUnit* SPUIQuestUnit::ClonePtr()
{
	return new SPUIQuestUnit;
}

BOOL SPUIQuestUnit::Copy( SPUIUnit** ppDestUIUnit )
{
	SPUIQuestUnit*	pUIQuestUnit	=	(SPUIQuestUnit*)(*ppDestUIUnit);

	if( pUIQuestUnit == NULL )
		return FALSE;

	SPTexture*		pCompleteTexture	=	pUIQuestUnit->GetCompleteTexture();

	SAFE_RELEASE( pCompleteTexture );

	pUIQuestUnit->SetCompleteTexture( pCompleteTexture );

	if( SPUIUnit::Copy( ppDestUIUnit ) == FALSE )
		return FALSE;

	pUIQuestUnit->SetQuestID( GetQuestID() );
	pUIQuestUnit->SetQuestType( GetQuestType() );
	pUIQuestUnit->SetQuestArchive( GetQuestArchive() );

	pUIQuestUnit->SetCompleteTexture( GetCompleteTexture() );
	pUIQuestUnit->SetCompleteRectSrc( GetCompleteRectSrc() );
	pUIQuestUnit->SetComplete( GetComplete() );

	pUIQuestUnit->SetCount( GetCount() );

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//
//	아이디로 리소스 재 로드
//

BOOL SPUIQuestUnit::ReResourceLoad()
{
	BOOL			bResult		=	FALSE;
	SPQuestAttr*	pQuestAttr	=	NULL;
	SPQuestAttr		QuestAttr;

	if( m_pQuestArchive == NULL )
	{
		if( SPQuestManager::GetInstance()->LoadLDTFile( m_iQuestID , &QuestAttr ) == false )
			return bResult;

		pQuestAttr	=	&QuestAttr;
	}
	else
		pQuestAttr	=	m_pQuestArchive->GetQuest( m_iQuestID );

	if( pQuestAttr == NULL )
		return bResult;

	SPQuestNpc*	pQuestNpc	=	&pQuestAttr->m_QuestNpc[ m_eQuestType ];

	return SetResource( pQuestNpc->m_iIcon , pQuestNpc->m_iIconIndex );
}

//////////////////////////////////////////////////////////////////////////
//
//	퀘스트 완료 텍스처 인터페이스
//

void SPUIQuestUnit::SetCompleteTexture( SPTexture* pTexture )
{
	if( pTexture != NULL )
		SetCompleteTexture( pTexture->GetResName() );
	else
		m_pCompleteTexture	=	pTexture;
}

BOOL SPUIQuestUnit::SetCompleteTexture( const char* pstrTexture )
{
	SAFE_RELEASE( m_pCompleteTexture );

	return g_pVideo->GetTextureMgr()->LoadTexture( pstrTexture , &m_pCompleteTexture );
}

SPTexture* SPUIQuestUnit::GetCompleteTexture()
{
	return m_pCompleteTexture;
}

//////////////////////////////////////////////////////////////////////////
//
//	퀘스트 완료 텍스처 영역 인터페이스
//

void SPUIQuestUnit::SetCompleteRectSrc( RECT rtSrc )
{
	m_rtCompleteSrc		=	rtSrc;
}

void SPUIQuestUnit::SetCompleteRectSrc( int ixLeft , int iyTop , int ixRight , int iyBottom )
{
	SetRect( &m_rtCompleteSrc , ixLeft , iyTop , ixRight , iyBottom );
}

RECT SPUIQuestUnit::GetCompleteRectSrc()
{
	return m_rtCompleteSrc;
}

//////////////////////////////////////////////////////////////////////////
//
//	퀘스트 완료 인터페이스
//

void SPUIQuestUnit::SetComplete( BOOL bComplete )
{
	m_bComplete	=	bComplete;
}

BOOL SPUIQuestUnit::GetComplete()
{
	return m_bComplete;
}

//////////////////////////////////////////////////////////////////////////
//
//	퀘스트 횟수 체크 인터페이스
//

void SPUIQuestUnit::SetCount( int iCount )
{
	m_iCount	=	iCount;

	ProcessCount();
}

int SPUIQuestUnit::GetCount()
{
	return m_iCount;
}

//////////////////////////////////////////////////////////////////////////
//
//	퀘스트 횟수 프로세서 인터페이스
//

void SPUIQuestUnit::ProcessCount()
{
	if( m_iCount <= 0 )
	{
		m_iCountLength	=	0;
		return;
	}

	if( m_iCount > QUEST_COUNT_MAX )
		m_iCount = QUEST_COUNT_MAX;

	char		szBuf[_MAX_PATH]	=	{};
	std::string	strBufOneChiper;

	itoa( m_iCount , szBuf , 10 );

	int			iOneChiper;
	m_iCountLength		=	(int)strlen( szBuf );

	for( int i = 0 ; i < m_iCountLength ; i++ )
	{
		strBufOneChiper	=	szBuf[i];
		iOneChiper		=	atoi( strBufOneChiper.c_str() );

		m_rtCountSrc[ i ].left		= iOneChiper				*	9;
		m_rtCountSrc[ i ].right		= m_rtCountSrc[ i ].left	+	9;
		m_rtCountSrc[ i ].top		= 0;
		m_rtCountSrc[ i ].bottom	= 10;

		m_ptCountPos[ i ].x	=	m_ptPos.x	+	( m_rtCountSrc[ i ].right	-	m_rtCountSrc[ i ].left) * i;
		m_ptCountPos[ i ].y	=	m_ptPos.y	+	m_ptSize.y	-	( m_rtCountSrc[ i ].bottom	-	m_rtCountSrc[ i ].top );
	}
}

//////////////////////////////////////////////////////////////////////////
//
//	퀘스트 횟수 렌더 인터페이스
//

void SPUIQuestUnit::RenderCount()
{
	if( m_iCount <= 1 || g_pVideo == NULL || g_pItemCluster == NULL )
		return;

	SPTexture*	pNumTexture	=	g_pItemCluster->GetNumTexture();

	if( pNumTexture == NULL )
		return;

	g_pVideo->Flush();

	for( int i = 0 ; i < m_iCountLength ; i++ )
		pNumTexture->RenderTexture( m_ptCountPos[ i ].x , m_ptCountPos[ i ].y , &m_rtCountSrc[ i ] );
}
