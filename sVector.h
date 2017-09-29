
#ifndef _SVECTOR_H_
#define _SVECTOR_H_
#include <iostream>
#include <stdlib.h>

/*
Class for a standard vector with three components, [x, y, z]
*/
using namespace std;

class sVector {
public:
	//View constructor
	sVector(float x = 0.0, float y = 0.0, float z = 0.0);

	float x, y, z;
	friend ostream &operator<<(ostream &strm, const sVector &v);
	friend sVector operator/(sVector lhs, float rhs);
	friend sVector operator*(sVector lhs, float rhs);
	friend sVector operator+(sVector lhs, sVector rhs);
	friend sVector operator-(sVector lhs, sVector rhs);
	void operator=(sVector rhs);
	void normalize();
	float length();
	float dot(sVector rhs);
	sVector cross(sVector rhs);
};


#endif