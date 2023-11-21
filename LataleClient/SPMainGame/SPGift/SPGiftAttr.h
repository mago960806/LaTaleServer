#pragma once

class SPGiftAttr {
public:
	SPGiftAttr();
	~SPGiftAttr();

	void Clear();
	void Init(UINT64 iKeyID);

public:
	UINT64			m_iKey;

	std::string		m_strName;
	std::string		m_strDesc;
	int				m_iItemID;
	int				m_iItemRare;
	int				m_iItemStack;
	UINT64			m_iEly;
};