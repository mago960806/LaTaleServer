
#ifndef __SPBEZIER_H__
#define __SPBEZIER_H__

class SPVector;

class SPBezier
{
public:
	SPBezier();
	SPBezier(const SPBezier& kBez);
	~SPBezier();

	void Clear();
	void SetControlPoint(int iPointNum, SPVector* pkPoint);

	// 0 <= t <= 1
	SPVector GetInterpolation(float t);


	SPBezier& operator = (const SPBezier& kBezier);

protected:
	int GetFactorial(int n);

	int m_iPointNum;
	SPVector* m_pkControlPoint;
};

#endif