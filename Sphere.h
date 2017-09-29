
#ifndef _SPHERE_H_
#define _SPHERE_H_
#include "sVector.h"
#include "Fill.h"
using namespace std;

class Sphere {
public:
	//View constructor
	Sphere();
	sVector center;
	Fill fill;
	float r;
};

#endif