// c_Window_Edit.h

#pragma once

#include "SPWindow_Base.h"

class SPWindow_Edit : public SPWindow_Base
{
public:
	SPWindow_Edit();
	virtual ~SPWindow_Edit();
	
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

