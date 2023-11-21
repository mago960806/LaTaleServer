#pragma once

class SPGiftAttr;
class SPGiftArchive;

class SPGiftManager {
public:
	SPGiftManager();
	~SPGiftManager();

	/**
	 * 초기화 한다 <br> 
	 @param 
	 @return
	*/
	void Clear();

	/**
	 * 선물을 추가한다 <br> 
	 @param 
	 @return
	*/
	bool InsertGift(int iItemID, INT64 iKeyID);
	
	/**
	 * LDT에서 실 데이터 로드 <br> 
	 @param 
	 @return
	*/
	bool LoadGiftLDT(int iItemID, SPGiftAttr* pGiftAttr);

	/**
	 * 리스트 로드상태 설정<br>
	 @param 
	 @return
	*/
	void SetGiftLoad(bool bLoad) { m_bGiftListLoad = bLoad;	}
	
	/**
	 * 리스트가 로드되어있는지 확인 <br>
	 @param 
	 @return
	*/
	bool IsGiftListLoad() { return m_bGiftListLoad; }

	/**
	 * 선물 리스트 크기 확인 <br> 
	 @param 
	 @return
	*/
	int	GetGiftListSize();

	/**
	 * 해당 위치의 선물 데이터를 가져온다 <br> 
	 @param 
	 @return
	*/
	SPGiftAttr* GetGift(int iIndex);

	/**
	 * 해당 Key의 선물 데이터를 삭제한다 <br> 
	 @param 
	 @return
	*/
	bool DeleteGiftItem(UINT64 iKey);

	/**
	 * 선물리스트를 요청한다 <br> 
	 @param 
	 @return
	*/
	void SendGiftList();

	/**
	 * 해당 위치의 선물 받기 요청한다 <br> 
	 @param 
	 @return
	*/
	void SendGetGiftItem(int iIndex);

	SPGiftArchive*	GetGiftArchive() { return m_pGiftArchive; }
	

private:
	SPGiftArchive*	m_pGiftArchive;			/**< Gift List Data <br> */

	SPLDTFile*		m_pLDTGiftData;			/**< Gift Data LDT <br> */

	bool			m_bGiftListLoad;		/**< Gift List가 로드되어 있는지 <br> */
};