// SPWindow_DigitBoard.h

#pragma warning (disable:4065)

#ifndef SPWindow_DigitBoard_h
#define SPWindow_DigitBoard_h

#include "SPWindow_Base.h"

class SPWindow_DigitBoard : public SPWindow_Base
{
public:
	SPWindow_DigitBoard();
	~SPWindow_DigitBoard();

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
