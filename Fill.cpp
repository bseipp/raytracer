#include "Fill.h"
#include "sVector.h"

Fill::Fill() {}

Fill::Fill(sVector color, float kd, float ks, float e, float kt, float ir)
{
	this->color = color;
	this->kd = kd;
	this->ks = ks;
	this->e = e;
	this->kt = kt;
	this->ir = ir;
}
