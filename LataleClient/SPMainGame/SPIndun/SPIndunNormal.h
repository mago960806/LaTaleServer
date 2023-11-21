#ifndef __SPINDUN_NORMAL_H__
#define __SPINDUN_NORMAL_H__

class SPIndun ;
class SPIndunNormal : public SPIndun
{
public:
	SPIndunNormal() ;
	SPIndunNormal(int indunID, UINT32 iInstanceID, INDUN_TYPE iType);
	virtual ~SPIndunNormal();
	virtual bool Init();
	virtual void Clear();
	virtual void Process(float fTime);
	virtual void Render();

protected:
};


#endif