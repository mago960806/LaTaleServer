#pragma once

class SPUserShopAttr;

class SPUserShopArchive {
public:
	SPUserShopArchive();
	~SPUserShopArchive();

	SPUserShopAttr* GetUserShopData(int iIndex);
	SPUserShopAttr* FindUserShopData(int iType);
	int CheckUserShopClass();
	int CheckPetShopClass( void );

protected:
	bool LoadUserShopListLDT();
	void DeleteAllUserShopInfo();

private:
	std::vector<SPUserShopAttr*> m_vpUserShopInfo;
};