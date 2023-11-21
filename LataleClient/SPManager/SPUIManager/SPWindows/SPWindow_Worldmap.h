// SPWindow_Worldmap.h

#pragma warning (disable:4065)

#ifndef SPWindow_Worldmap_h
#define SPWindow_Worldmap_h

#include "SPWindow_Base.h"

class SPWindow_WorldMap : public SPWindow_Base
{
public:
	SPWindow_WorldMap();
	~SPWindow_WorldMap();

	
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