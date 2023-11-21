// SPWindow_StatPallet.h

#pragma warning (disable:4065)

#ifndef SPWindow_StatPallet_h
#define SPWindow_StatPallet_h

#include "SPWindow_Base.h"

class SPWindow_StatPallet : public SPWindow_Base
{
public:
	SPWindow_StatPallet();
	~SPWindow_StatPallet();

	
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
