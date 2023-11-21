// SPWindow_Root.h

#pragma once

#include "SPWindow_Base.h"

class SPWindow_Root : public SPWindow_Base
{
public:
	SPWindow_Root();
	~SPWindow_Root();

	virtual void*	Get(int reSPID)
	{
		return SPWindow_Base::Get(reSPID);
	}
	virtual void	Set(int ID,char* Value)
	{
		SPWindow_Base::Set(ID,Value);
	}
protected:
private:
};

