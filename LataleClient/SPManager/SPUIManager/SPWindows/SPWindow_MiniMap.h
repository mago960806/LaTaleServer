// SPWindow_MiniMap.h

#pragma warning (disable:4065)

#ifndef SPWindow_MiniMap_h
#define SPWindow_MiniMap_h

#include "SPWindow_Base.h"

class SPWindow_MiniMap : public SPWindow_Base
{
public:
	SPWindow_MiniMap();
	~SPWindow_MiniMap();

	virtual void* Get(int ID)
	{
		switch(ID)
		{
		default:
			return SPWindow_Base::Get(ID);
		}
	}

	virtual void Set(int ID,char* Value)
	{
		switch(ID)
		{
		default:
			SPWindow_Base::Set(ID,Value);
		}
	}
protected:
private:
};


#endif
