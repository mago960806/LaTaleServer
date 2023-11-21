#pragma once

enum MAIL_STATUS {
	MAIL_STATUS_NULL = 0,
	MAIL_STATUS_LIST = 1,
	MAIL_STATUS_VIEW_REQUEST = 2,
	MAIL_STATUS_VIEW = 3,
	MAIL_STATUS_DELETE = 4,
	MAIL_STATUS_MAX = 5,
};

class SPItem;

class SPMailAttr {
public:
	SPMailAttr();
	~SPMailAttr();

	void Init(MAIL_HEADER_ATTACHMENT* pHeaderAttach);

	void SetSubject(char* pSubject, int iLen);

	void SetBody(char* pMemo, int iLen);

	bool SetItem(/*CONTAINER_ITEM* pItem*/);

	/**
	 * 본문을 받았는지? <br> 
	 @param 
	 @return
	*/
	bool IsSendBody() { return m_bSendBody; }

	/**
	 * 해당 메일을 읽어봤는지? <br>  
	 @param 
	 @return
	*/
	bool IsRead();
	

protected:
	
	void Clear();	
	

public:
	MAIL_HEADER_ATTACHMENT*	m_pHeader;

	//_MAIL_HEADER_ATTACHMENT_SS
	std::string			m_strSubject;		// 제목
	int					m_iSubjectLen;		// 제목 길이
	
	//Same as _MAIL_INFO_SS
	std::string			m_strReceiver;		// 수신자, null included
	std::string			m_strMemo;			// 본문 (MAX_MAIl_BODY_LEN == 100)
	int					m_iMemoLen;			// 본문 길이	

	int					m_iMailType ;		// 메일타입 (이전버전과 호환을 위해 정의)
	bool				m_bSendBody;		/**< 본문을 받았는지 ? <br> */

	CONTAINER_ITEM		m_ContainerItem;
	SPItem*				m_pItem;			//
};