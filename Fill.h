
#ifndef _FILL_H_
#define _FILL_H_
#include "sVector.h"

using namespace std;

class Fill {
public:
	//View constructor

	Fill();

	Fill(sVector color, float kd, float ks, float e, float kt, float ir);
	sVector color;

	/*
	kd = coefficient to scale overall diffuse contribution
	ks = coefficient for both reflect rays and blinn-phong glossy specular component
	e = exponent for blinn-phong
	kt = coefficient for refracted rays
	ir = index of refraction
	*/
	float kd, ks, e, kt, ir;
};

#endif