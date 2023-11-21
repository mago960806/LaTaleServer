#pragma once

class SPRewardAttr;

class SPRewardArchive {
public:
	SPRewardArchive();
	~SPRewardArchive();
	
	bool LoadRewardAttrLDT();
	//SPQuestAttr* GetQuestAttr(int iItemID);
	SPRewardAttr* GetReward(int iRewardID);

protected:
	void DeleteAllRewardAttr();

private:
	std::vector<SPRewardAttr*> m_vpRewardList;
};