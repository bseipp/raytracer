
#ifndef _HITRECORD_H_
#define _HITRECORD_H_
#include "sVector.h"
#include "Sphere.h"

using namespace std;

class HitRecord {
public:
	//View constructor
	HitRecord();
	Sphere sphere;
	float t;
	sVector n, p;
};

#endif