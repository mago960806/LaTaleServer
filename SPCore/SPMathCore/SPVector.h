#ifndef __SPVECTOR_H__
#define __SPVECTOR_H__

#include <math.h>

class SPVector
{
public:
	SPVector();
	SPVector(float fX, float fY, float fZ);
	SPVector(const SPVector& kVector);

	// operator
	SPVector& operator = (const SPVector& kVector);
	
	SPVector operator + (const SPVector& kVector);
	SPVector operator - (const SPVector& kVector);

	SPVector operator * (const float fValue);
	SPVector operator / (const float fValue);
	
	SPVector& operator += (const SPVector& kVector);
	SPVector& operator -= (const SPVector& kVector);

	SPVector& operator /= (const float fValue);

	float Dot(const SPVector& kVector);
	SPVector Cross(const SPVector& kVector);

	float Length();
	float LengthSquared();
	SPVector Unit();
	SPVector& Normalize();

public:
	float m_fX;
	float m_fY;
	float m_fZ;
};

#include "SPVector.inl"

#endif