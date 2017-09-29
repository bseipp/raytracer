#include "Ray.h"
#include "sVector.h"

Ray::Ray() {
	this->ks = 1.0;
}

Ray::Ray(sVector origin, sVector direction){
	this->origin = origin;
	this->direction = direction;
	this->ks = 1.0;
}
