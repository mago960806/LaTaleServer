// Copyright (C) AJJIYA
//***************************************************************************
// System Name : 
// Comment     : CPP로 분리하면 LINK에러가 발생해서 H로 묶음.
// Creation    : AJJIYA 2005-5-9    11:45 
//***************************************************************************

#pragma once

#include <map>

//	Class Factory
template< class ParentClass , class ChildClass >
ParentClass CreateObj()
{
	return new ChildClass;
}

template< class ParentClass , class UniqueID >
class SPFXFactory
{
public:
	typedef ParentClass (*CreateObjectFunc)();
	typedef typename std::map< UniqueID , CreateObjectFunc >::iterator spFXIterator;

	SPFXFactory()		{}
	~SPFXFactory()		{	m_mSpace.clear();		}

	template< class ChildClass >
		bool RegisterClass( UniqueID id )
	{
		if ( m_mSpace.find( id ) != m_mSpace.end() )
			return false;

		m_mSpace[id] = &CreateObj< ParentClass , ChildClass >;

		return true;
	}

	bool UnRegisterClass( UniqueID id )
	{
		spFXIterator it = m_mSpace.find( id );

		if( it != m_mSpace.end() )
		{
			m_mSpace.erase( it );
			return true;
		}
		return false;
	}

	ParentClass CreateClass( UniqueID id )
	{
		spFXIterator it = m_mSpace.begin();

		while ( it != m_mSpace.end() )
		{
			if( id == (*it).first )
				return ((*it).second)();
			it++;
		}

		return NULL;
	}

protected:
	std::map< UniqueID , CreateObjectFunc > m_mSpace;
};
