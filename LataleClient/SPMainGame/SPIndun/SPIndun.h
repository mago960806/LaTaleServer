#ifndef __SPINDUN_UNIT_H__
#define __SPINDUN_UNIT_H__

#define INDUN_EMPTY		-1

enum INDUN_TYPE
{
	IDT_NULL = 0,
	IDT_GUILD,
	IDT_PARTY,
	IDT_SINGLE,
	IDT_EVENT,
	IDT_GINDUN,
};


class SPIndun
{
public:
	SPIndun() ;
	SPIndun(int indunID, UINT32 iInstanceID, INDUN_TYPE iType);
	virtual ~SPIndun();
	virtual bool Init();
	virtual void Clear();
	virtual void Process(float fTime);
	virtual void Render();

	int		GetIndunID()		{	return m_iIndunLdtID;	}
	UINT32	GetInstanceID()		{	return m_iInstanceID;	}
	INDUN_TYPE GetIndunType()	{	return m_iType;		}

protected:
	int			m_iIndunLdtID ;
	UINT32		m_iInstanceID ;
	INDUN_TYPE	m_iType ;
};


#endif