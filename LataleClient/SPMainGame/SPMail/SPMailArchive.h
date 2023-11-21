#pragma once

class SPMailArchive {
public:
	SPMailArchive();
	~SPMailArchive();

	/**
	 * 메일 리스트로 받는 경우 <br> 
	 * 순서 대로 추가 <br>
	 @param 
	 @return
	*/
	int AddMail(SPMailAttr* pMail);
	
	/**
	 * 새메일을 받는 경우 <br>
	 * 리스트의 앞에 추가 <br>
	 @param 
	 @return
	*/
	int AddNewMail(SPMailAttr* pMail);
	
	/**
	 * 모든 메일을 삭제 <br>
	 @param 
	 @return
	*/
	void DeleteAllMail();
	
	/**
	 * 메일 리스트의 크기를 반환 <br>
	 @param 
	 @return
	*/
	int GetSize();

	/**
	 * 주어진 인덱스 위치의 MailAttr을 반환 <br>
	 @param 
	 @return
	*/
	SPMailAttr* GetMailAttr(int iIndex);

	/**
	 * 주어진 메일 ID의 MailAttr을 반환 <br>
	 @param 
	 @return
	*/
	SPMailAttr* FindMail(UINT64 iMailID);

	/**
	 * 주어진 메일 ID의 MailAttr에서 헤더 내용중 아이템, 엘리관련을 삭제 <br> 
	 @param 
	 @return
	*/
	bool ClearMailHeader(UINT64 iMailID);
	
	/**
	 * 주어진 메일 ID의 메일을 삭제 <br>
	 @param 
	 @return
	*/
	bool DeleteMail(UINT64 iMailID);

	/**
	 * 내림차순 소트 <br> 
	 @param 
	 @return
	*/
	bool SetSort();

	/**
	 * 리스트내의 읽지 않은 메일이 있는지 확인한다 <br> 
	 * 하나라도 일지 않은 메일이 있는경우 false <br>
	 @param 
	 @return
	*/
	bool CheckReadMail();

protected:
	

private:
	std::vector<SPMailAttr*>	m_vpMail;
};