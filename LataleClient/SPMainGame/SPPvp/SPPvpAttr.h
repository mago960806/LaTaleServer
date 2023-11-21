// Copyright (C) DURAGON
//********************************************************************
//	filename: 	SPPvpAttr.h
//	created:	2007/11/19   Time:11:32
//	purpose:	
//*********************************************************************/

#define PVP_EMPTY		-1



class SPPvpAttr
{
public:
	SPPvpAttr() ;
	SPPvpAttr(int indunID, UINT32 iInstanceID, int iType);
	virtual ~SPPvpAttr();
	virtual bool Init();
	virtual void Clear();
	virtual void Process(float fTime);
	virtual void Render();

	int			GetIndunID()		{	return m_iPvpLdtID;		}
	UINT32		GetInstanceID()		{	return m_iInstanceID;	}
	int			GetIndunType()		{	return m_iMatchType;	}
	int			GetWinValue()		{	return m_iWinValue;		}
	int			GetWarnValue()		{	return m_iWarnValue;	}

protected:
	UINT32		m_iInstanceID ;
	int			m_iPvpLdtID ;
	int			m_iMatchType ;		// PVP Match Type
	int			m_iWinValue ;		// 승리조건
	int			m_iWarnValue ;		// Warning 조건
	std::string m_strPvpName ;		
};

