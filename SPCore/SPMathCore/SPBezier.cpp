
#include <windows.h>
#include <math.h>
#include "SPVector.h"
#include "SPBezier.h"

//------------------------------------------------------------------------------------
SPBezier::SPBezier()
: m_iPointNum(0)
, m_pkControlPoint(NULL)
{
}

//------------------------------------------------------------------------------------
SPBezier::SPBezier(const SPBezier& kBez)
{
	m_iPointNum = kBez.m_iPointNum;
	m_pkControlPoint = NULL;

	if( m_iPointNum )
	{
		m_pkControlPoint = new SPVector[m_iPointNum];
		memcpy(m_pkControlPoint, kBez.m_pkControlPoint, sizeof(SPVector) * m_iPointNum);
	}
}

//------------------------------------------------------------------------------------
SPBezier::~SPBezier()
{
	Clear();
}

//------------------------------------------------------------------------------------
void SPBezier::Clear()
{
	m_iPointNum = 0;
	if( m_pkControlPoint )
	{
		delete[] m_pkControlPoint;
		m_pkControlPoint = NULL;
	}
}

//------------------------------------------------------------------------------------
void SPBezier::SetControlPoint(int iPointNum, SPVector* pkPoint)
{
	if( pkPoint == NULL || iPointNum == 0 )
		return;

	Clear();

	m_pkControlPoint = new SPVector[iPointNum];
	for( int i = 0; i < iPointNum; i++ )
		m_pkControlPoint[i] = pkPoint[i];

	m_iPointNum = iPointNum;
}

//------------------------------------------------------------------------------------
int SPBezier::GetFactorial(int n)
{
	int iRet = 1;

	for(int i = 1; i <= n; i++ )
		iRet *= i;

	return iRet;
}

//------------------------------------------------------------------------------------
SPVector SPBezier::GetInterpolation(float t)
{
	if( m_pkControlPoint == NULL || m_iPointNum == 0 )
		return SPVector(0.0f, 0.0f, 0.0f);

	if( m_iPointNum == 1 )
		return m_pkControlPoint[0];

	int n, i, j;
	n = m_iPointNum - 1;

	float fInvT;
	fInvT = 1.0f - t;

	// Bernstein
	SPVector kSum;
	float fWeight;
	
	float fiT;		// i!
	float fnT;		// n!
	float fnsiT;	// (n - i)!
	float fBT;
	float ftpow, fInvtpow;

	for( i = 0; i <= n; i++ )
	{
		fWeight = 1.0f;
		if( i == n )
		{
			for( j = 0; j < n; j++ )
				fWeight *= t;
		}
		else
		{
			if( i == 0 )
				fBT = 1.0f;
			else
			{
				fiT = (float)GetFactorial(i);
				fnT = (float)GetFactorial(n);
				fnsiT = (float)GetFactorial(n - i);

				fBT = fnT / (fiT * fnsiT);
			}

			ftpow = pow((float)t, (float)i);
			fInvtpow = pow((float)fInvT, (float)(n - i));

			fWeight = fBT * ftpow * fInvtpow;
		}

		kSum += (m_pkControlPoint[i] * fWeight);
	}

	return kSum;
}

//------------------------------------------------------------------------------------
SPBezier& SPBezier::operator = (const SPBezier& kBezier)
{
	Clear();
	m_iPointNum = kBezier.m_iPointNum;
	if( m_iPointNum )
	{
		m_pkControlPoint = new SPVector[m_iPointNum];
		memcpy(m_pkControlPoint, kBezier.m_pkControlPoint, sizeof(SPVector) * m_iPointNum);
	}

	return *this;
}