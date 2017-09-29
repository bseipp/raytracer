#include "sVector.h"
#include "math.h"
#include <iostream>

using namespace std;

sVector::sVector(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

//for debugging purposes, print the vector
ostream &operator<<(ostream &strm, const sVector &v) {
	strm << "[";
	strm << v.x; strm << ", ";
	strm << v.y; strm << ", ";
	strm << v.z; strm << "]";
	return strm;
}

sVector operator/(sVector lhs, float rhs) {
	sVector temp;
	temp.x = lhs.x;
	temp.y = lhs.y;
	temp.z = lhs.z;

	//sVector *temp = new sVector(lhs.x, lhs.y, lhs.z);
	temp.x /= rhs;
	temp.y /= rhs;
	temp.z /= rhs;

	return temp;
}

sVector operator*(sVector lhs, float rhs) {
	sVector temp;
	temp.x = lhs.x;
	temp.y = lhs.y;
	temp.z = lhs.z;

	//sVector *temp = new sVector(lhs.x, lhs.y, lhs.z);	temp.x *= rhs;
	temp.x *= rhs;
	temp.y *= rhs;
	temp.z *= rhs;

	return temp;
}

sVector operator+(sVector lhs, sVector rhs) {
	sVector temp;
	temp.x = lhs.x;
	temp.y = lhs.y;
	temp.z = lhs.z;

	//sVector *temp = new sVector(lhs.x, lhs.y, lhs.z);	temp.x += rhs.x;
	temp.x += rhs.x;
	temp.y += rhs.y;
	temp.z += rhs.z;

	return temp;
}

sVector operator-(sVector lhs, sVector rhs) {
	sVector temp;
	temp.x = lhs.x;
	temp.y = lhs.y;
	temp.z = lhs.z;

	//sVector *temp = new sVector(lhs.x, lhs.y, lhs.z);	temp.x -= rhs.x;
	temp.x -= rhs.x;
	temp.y -= rhs.y;
	temp.z -= rhs.z;

	return temp;
}

void sVector::operator=(sVector rhs) {
	this->x = rhs.x;
	this->y = rhs.y;
	this->z = rhs.z;
}


void sVector::normalize() {
	// Calculate length of itself
	float length = this->length();
	if (length > 0) {
		this->x /= length;
		this->y /= length;
		this->z /= length;
	}
}

float sVector::length() {
	return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

float sVector::dot(sVector rhs) {
	return this->x * rhs.x + this->y * rhs.y + this->z * rhs.z;
}

sVector sVector::cross(sVector rhs) {
	float detYZ = this->y * rhs.z - this->z * rhs.y;
	float detXZ = -1 * (this->x * rhs.z - this->z * rhs.x);
	float detXY = this->x * rhs.y - this->y * rhs.x;

	return *new sVector(detYZ, detXZ, detXY);
}
