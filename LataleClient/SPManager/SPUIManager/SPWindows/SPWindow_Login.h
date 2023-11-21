// SPWindow_Login.h

#pragma once

#include "SPWindow_Base.h"

class SPWindow_Login : public SPWindow_Base
{
public:
	SPWindow_Login(){};
	~SPWindow_Login(){};

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

