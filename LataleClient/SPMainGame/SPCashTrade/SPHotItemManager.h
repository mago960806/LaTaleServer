#pragma once

class SPHotItemInfo;

class SPHotItemManager {
public:
	SPHotItemManager();
	~SPHotItemManager();

	bool AddHotItem(CASHSHOP_HOT_INFO* pInfo);
	
	int GetHotItemSize();

	SPHotItemInfo* GetHotItem(int iIndex);

	void DeleteAllHotItem();

	void SetShowFlag(bool bShow)			{	m_bListShow = bShow;	}
	bool IsShow();

protected:
	

private:
	std::vector<SPHotItemInfo*>			m_vpHotItemList;
	bool								m_bListShow;
};


