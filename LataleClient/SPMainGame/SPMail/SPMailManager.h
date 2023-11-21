#pragma once

class SPMailAttr;
class SPMailArchive;

enum MAIL_VOLUME {
	MAIL_VOLUME_NULL = 0,
	MAIL_VOLUME_SMALL = 1,
	MAIL_VOLUME_MIDDLE = 2,
	MAIL_VOLUME_LARGE = 3,
	MAIL_VOLUME_MAX,
};

class SPMailManager {
public:
	SPMailManager();
	~SPMailManager();

	void Clear();

	int AddMail(SPMailAttr* pMail);

	int AddNewMail(SPMailAttr* pMail);

	void SetMailLoad(bool bLoad) { m_bMailLoad = bLoad; }
	
	bool IsMailListLoad() {	return m_bMailLoad; }

	int	GetMailListSize();
	
	/**
	 * 메일 리스트 윈도우가 열리면 자동적으로 새로운 메일은 없는것으로 처리됨 <br> 
	 @param 
	 @return
	*/
	void SetNewMail(bool bNew = false);	
	
	/**
	 * 새로운 메일이 있는지 확인 <br> 
	 @param 
	 @return
	*/
	bool IsNewMail() { return m_bNewMail; }

	/**
	* 
	@param 
	@return
	*/
	MAIL_VOLUME GetMailVolume();

	/**
	* 메일리스트를 조사해 새메일이 있는경우 새메일 표시 업데이트 <br> 
	@param 
	@return
	*/
	void CheckNewMail();

	/**
	* 해당 위치의 메일 데이터를 가져온다 <br> 
	@param 
	@return
	*/
	SPMailAttr* GetMail(int iIndex);

	/**
	 * 
	 @param 
	 @return
	*/
	void SetSelectMail(int iIndex);

	/**
	* 선택되어진 메일의 포인터 <br> 
	@param 
	@return
	*/
	SPMailAttr* GetSelectMail()		{ return m_pSelectMail; }

	/**
	* 선택되어진 메일의 인덱스 번호 <br>
	@param 
	@return
	*/
	int	GetSelectMailIndex()		{ return m_iSelectMail; }

	/**
	 * 메일 리스트 요청 <br> 
	 @param 
	 @return
	*/
	void SendGetMailList();

	/**
	 * MailList 인터페이스 업데이트 요청 <br> 
	 @param bool bMsg				메일알람 용
	 @return
	*/
	void OnUpdateMailList(bool bMsg = false);

	/**
	 * 메일 본문 내용 요청 <br>  
	 @param 
	 @return
	*/
	void SendGetMailBody(int iSelectIndex);
	
	/**
	 * 첨부물 받기 요청 <br>  
	 @param 
	 @return
	*/
	void SendReceiveBody(int iSelectIndex = -1);

	/**
	 * 메일 삭제 요청 <br> 
	 @param 
	 @return
	*/
	void SendDeleteMail(int iSelectIndex = -1);

	/**
	 * 해당 메일 아이디의 메일을 삭제 <br> 
	 @param 
	 @return
	*/
	bool OnDeleteMail(UINT64 iMailID);

	/**
	 * 해당 메일의 본문을 읽은 경우 프레그 세팅 <br>
	 @param 
	 @return
	*/
	void OnReadMail(UINT64 iMailID);

	/**
	 * 해당 메일 아이디의 메일의 첨부 내용을 초기화 <br> 
	 @param 
	 @return
	*/
	bool OnClearMailHeader(UINT64 iMailID);		
	

private:
	SPMailArchive* m_pMailArchive;

	bool			m_bMailLoad;			/**< 메일이 로드되었는지 <br> */
	int				m_iSelectMail;			/**< UI상 선택되어진 메일 인덱스 <br> */
	SPMailAttr*		m_pSelectMail;
	bool			m_bNewMail;				/**< 새로운 메일 있음, 리스트 열었을때 초기화 <br> */
};