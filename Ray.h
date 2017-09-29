
#ifndef _RAY_H_
#define _RAY_H_
#include "sVector.h"

using namespace std;

class Ray {
public:
	//View constructor

	Ray();

	Ray(sVector origin , sVector direction);
	sVector origin, direction;
	int bounces;
	float ks;
};

#endif