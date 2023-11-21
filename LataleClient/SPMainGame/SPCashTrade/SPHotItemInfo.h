#pragma once

const int MAX_CHECK_INVEN		= 4;


struct SPCheckInvenInfo {
	CONTAINER_TYPE	m_iContainer;
	int				m_iCheckCount;

	SPCheckInvenInfo() {
		Clear();
	}

	~SPCheckInvenInfo() {
		Clear();
	}

	void Clear() {
		m_iContainer		= CONTAINER_TYPE_INVALID;
		m_iCheckCount		= 0;		
	}
};

struct SPCashItemAttr;

class SPHotItemInfo {
public:	
	SPHotItemInfo(CASHSHOP_HOT_INFO* pInfo);
	~SPHotItemInfo();

	void Clear();
	void Init(CASHSHOP_HOT_INFO* pInfo);

	SPCashItemAttr*		GetCashItemAttr();

	bool IsShow();

private:
	SPHotItemInfo();

public:
	int					m_iCashShopID;
	int					m_iToolTipLength;
	std::string			m_strToolTip;

	SPCashItemAttr*		m_pCashShopAttr;

	SPCheckInvenInfo	m_CheckInven[MAX_CHECK_INVEN];		
};


