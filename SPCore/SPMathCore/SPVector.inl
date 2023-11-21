
//------------------------------------------------------------------------------------
inline SPVector& SPVector::operator = (const SPVector& kVector)
{
	m_fX = kVector.m_fX;
	m_fY = kVector.m_fY;
	m_fZ = kVector.m_fZ;

	return *this;
}

//------------------------------------------------------------------------------------
inline SPVector SPVector::operator + (const SPVector& kVector)
{
	SPVector kRet;

	kRet.m_fX = m_fX + kVector.m_fX;
	kRet.m_fY = m_fY + kVector.m_fY;
	kRet.m_fZ = m_fZ + kVector.m_fZ;
	
	return kRet;
}

//------------------------------------------------------------------------------------
inline SPVector SPVector::operator - (const SPVector& kVector)
{
	SPVector kRet;

	kRet.m_fX = m_fX - kVector.m_fX;
	kRet.m_fY = m_fY - kVector.m_fY;
	kRet.m_fZ = m_fZ - kVector.m_fZ;

	return kRet;
}

//------------------------------------------------------------------------------------
inline SPVector SPVector::operator * (const float fValue)
{
	SPVector kRet;

	kRet.m_fX = m_fX * fValue;
	kRet.m_fY = m_fY * fValue;
	kRet.m_fZ = m_fZ * fValue;

	return kRet;
}

//------------------------------------------------------------------------------------
inline SPVector SPVector::operator / (const float fValue)
{
	SPVector kRet;

	float fTemp = fValue;
	if( fTemp == 0.0f )
		fTemp = 1.0f;

	kRet.m_fX = m_fX / fTemp;
	kRet.m_fY = m_fY / fTemp;
	kRet.m_fZ = m_fZ / fTemp;

	return kRet;
}

//------------------------------------------------------------------------------------
inline SPVector& SPVector::operator += (const SPVector& kVector)
{
	m_fX += kVector.m_fX;
	m_fY += kVector.m_fY;
	m_fZ += kVector.m_fZ;

	return *this;
}

//------------------------------------------------------------------------------------
inline SPVector& SPVector::operator -= (const SPVector& kVector)
{
	m_fX -= kVector.m_fX;
	m_fY -= kVector.m_fY;
	m_fZ -= kVector.m_fZ;

	return *this;
}

//------------------------------------------------------------------------------------
inline SPVector& SPVector::operator /= (const float fValue)
{
	float fTemp = fValue;
	if( fTemp == 0.0f )
		fTemp = 1.0f;

	m_fX /= fTemp;
	m_fY /= fTemp;
	m_fZ /= fTemp;

	return *this;
}

//------------------------------------------------------------------------------------
inline float SPVector::Dot(const SPVector& kVector)
{
	return (m_fX * kVector.m_fX + m_fY * kVector.m_fY + m_fZ * kVector.m_fZ);
}

//------------------------------------------------------------------------------------
inline SPVector SPVector::Cross(const SPVector& kVector)
{
	SPVector kRet;

	kRet.m_fX = m_fY * kVector.m_fZ - m_fZ * kVector.m_fY;
	kRet.m_fY = m_fZ * kVector.m_fX - m_fX * kVector.m_fZ;
	kRet.m_fZ = m_fX * kVector.m_fY - m_fY * kVector.m_fX;

	return kRet;
}

//------------------------------------------------------------------------------------
inline float SPVector::Length()
{
	return sqrt(Dot(*this));
}

//------------------------------------------------------------------------------------
inline float SPVector::LengthSquared()
{
	return Dot(*this);
}

//------------------------------------------------------------------------------------
inline SPVector SPVector::Unit()
{
	return (*this) / Length();
}

//------------------------------------------------------------------------------------
inline SPVector& SPVector::Normalize()
{
	*this /= Length();
	return *this;
}