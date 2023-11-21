// ***************************************************************
//  SPUIChatMacro   version:  1.0   ·  date: 08/09/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#pragma once

class SPUIUnit;

enum CHAT_MACRO_TYPE
{
	CHAT_MACRO_TYPE_NULL	=	0	,
	CHAT_MACRO_TYPE_NORMAL			,
	CHAT_MACRO_TYPE_SPECIAL			,
	CHAT_MACRO_TYPE_COUNT			,
};

struct CHAT_MACRO
{
	int					m_iIndex;
	std::string			m_strName;
	std::string			m_strContents;
};

struct CHAT_MACRO_LDT
{
	CHAT_MACRO_TYPE		m_eType;
	int					m_iSubType;
	int					m_iCount;
	int					m_iIconFile;
	int					m_iIconIndex;
	int					m_iRequireItem;
	std::string			m_strAppName;
	std::string			m_strKeyName;
};

struct CHAT_MACRO_POS
{
	CHAT_MACRO_TYPE		m_eType;
	int					m_iIndex;
};

class SPUIChatMacro : public SPUIUnit
{
public:
	SPUIChatMacro								(void);
	virtual ~SPUIChatMacro						(void);

	//	기본 인터페이스
	virtual	void			Init				();
	virtual	void			Clear				();
	virtual	void			Process				( float fTime );
	virtual	void			Render				();
	virtual	BOOL			Use					();

	//	자기 자신 복사 인터페이스
	virtual SPUIUnit*		ClonePtr			();
	virtual BOOL			Copy				( SPUIUnit** ppDestUIUnit );

	//	리소스 재 로드
	virtual	BOOL			ReResourceLoad		();

	//	채팅 매크로 데이터
	void					SetChatMacro		( CHAT_MACRO* pChatMacro );
	CHAT_MACRO*				GetChatMacro		();

	//	채팅 매크로 LDT 데이터
	void					SetChatMacroLDT		( CHAT_MACRO_LDT* pChatMacroLDT );
	CHAT_MACRO_LDT*			GetChatMacroLDT		();

	//	글자 렌더 위치 인터페이스
	void					SetTextRect			( RECT rtSrc );
	void					SetTextRect			( int ixLeft , int iyTop , int ixRight , int iyBottom );
	RECT					GetTextRect			();

	//	글자 색깔 인터페이스
	void					SetTextColor		( D3DXCOLOR cColor );
	D3DXCOLOR				GetTextColor		();

	//	글자 정렬 인터페이스
	void					SetTextAlign		( DWORD dwAlign );
	DWORD					GetTextAlign		();

	//	글자 폰트 그림자 인터페이스
	void					SetFontShadow		( BOOL bShow );
	BOOL					GetFontShadow		();

	//	글자 Period 인터페이스
	void					SetPeriod			( BOOL bPeriod );
	BOOL					GetPeriod			();

	//	글자 내용 인터페이스
	void					SetText				( const char* pszText );
	const char*				GetText				();

	//	글자 폰트 인터페이스
	void					SetFont				( FONT_ENUMERATE eFont );
	FONT_ENUMERATE			GetFont				();

protected:

	//	기타 인터페이스
	BOOL					IsText				();

	//////////////////////////////////////////////////////////////////////////
	//
	//	변수들
	//

	CHAT_MACRO*				m_pChatMacro;			//	채팅 매크로 데이터
	CHAT_MACRO_LDT*			m_pChatMacroLDT;		//	채팅 매크로 LDT 데이터

	RECT					m_rtTextDest;			//	글자 렌더 위치
	D3DXCOLOR				m_cTextRGBA;			//	글자 색깔
	DWORD					m_dwTextAlign;			//	글자 정렬
	BOOL					m_bTextFontShadow;		//	글자 폰트 그림자
	BOOL					m_bTextPeriod;			//	글자 Period
	std::string				m_strText;				//	글자

	FONT_ENUMERATE			m_eTextFont;			//	글자 폰트 타입
	SPFont*					m_pTextFont;			//	글자 폰트 포인터


};
