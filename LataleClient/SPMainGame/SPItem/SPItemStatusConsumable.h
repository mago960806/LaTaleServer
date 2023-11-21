
/**
* @class	SPItemStatusConsumable
* @brief	Status of consumable Item ..(ex. potion, food, instant usable item.. )
* @author	kkandori
* @date		Jan, 20, 2005
*/

#pragma once

class SPItemStatusConsumable : public SPItemStatus
{
public:
	SPItemStatusConsumable();
	virtual ~SPItemStatusConsumable();
	virtual LRESULT SPSendMessage(MODEL_VALUE_TYPE mvMsg, LPARAM lParam = 0);

protected:
	virtual bool Copy(SPItemStatusConsumable* pSrc);
	virtual void SetNull();

protected:
	//int				m_nItemCnt;			// Item °¹¼ö

};