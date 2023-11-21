
#include "SPVector.h"

//------------------------------------------------------------------------------------
SPVector::SPVector()
: m_fX(0.0f)
, m_fY(0.0f)
, m_fZ(0.0f)
{		 
}

//------------------------------------------------------------------------------------
SPVector::SPVector(float fX, float fY, float fZ)
: m_fX(fX)
, m_fY(fY)
, m_fZ(fZ)
{		 
}

//------------------------------------------------------------------------------------
SPVector::SPVector(const SPVector& kVector)
: m_fX(kVector.m_fX)
, m_fY(kVector.m_fY)
, m_fZ(kVector.m_fZ)
{		 
}