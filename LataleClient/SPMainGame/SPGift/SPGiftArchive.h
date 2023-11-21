#pragma once

class SPGiftAttr;

class SPGiftArchive {
public:
	SPGiftArchive();
	~SPGiftArchive();

	/**
	 * 모든 선물 리스트를 삭제한다 <br> 
	 @param 
	 @return
	*/
	void DeleteAllGift();

	/**
	 * 선물 리스트에 선물을 추가한다 <br>
	 @param 
	 @return
	*/
	int AddGift(SPGiftAttr* pGiftAttr);

	/**
	 * 선물 갯수를 가져온다 <br>
	 @param 
	 @return
	*/
	int	GetSize();

	/**
	 * 선물 데이터를 가져온다 <br>
	 @param 
	 @return
	*/
	SPGiftAttr* GetGiftAttr(int iIndex);

	/**
	 * 해당키의 선물을 리스트에서 삭제한다 <br> 
	 @param 
	 @return
	*/
	bool DeleteGift(INT64 iKey);

private:
	std::vector<SPGiftAttr*>	m_vpGift;		/**< 선물을 추가한다 <br> */
};



