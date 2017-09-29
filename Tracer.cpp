#include "Sphere.h"
#include "sVector.h"
#include "Ray.h"
#include "Fill.h"
#include "HitRecord.h"
#include <algorithm> 
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <stdlib.h>
#include <math.h> 

using namespace std;

typedef unsigned char Pixel[3];
typedef Pixel *Column;

#define PI 3.14159265
#define EPSILON 0.0001

string outputFile;
sVector from, at, up, localColor;
Fill backgroundFill, sphereFill;
float angle = 0.0;
int xResolution, yResolution;
unsigned char *pixels;
vector<string> parseLine(string line);
vector<Sphere> spheres;
vector<sVector> lights;
float diffuse, specular;

void parseNppFile(string filename) {
	string line;
	ifstream nffFile(filename.c_str());

	if (nffFile.is_open())
	{
		while (getline(nffFile, line))
		{
			vector<string> parsedLine = parseLine(line);

			if (parsedLine.at(0) == "from") {
				from.x = strtof(parsedLine.at(1).c_str(), 0);
				from.y = strtof(parsedLine.at(2).c_str(), 0);
				from.z = strtof(parsedLine.at(3).c_str(), 0);
			}
			else if (parsedLine.at(0) == "at") {
				at.x = strtof(parsedLine.at(1).c_str(), 0);
				at.y = strtof(parsedLine.at(2).c_str(), 0);
				at.z = strtof(parsedLine.at(3).c_str(), 0);
			}
			else if (parsedLine.at(0) == "up") {
				up.x = strtof(parsedLine.at(1).c_str(), 0);
				up.y = strtof(parsedLine.at(2).c_str(), 0);
				up.z = strtof(parsedLine.at(3).c_str(), 0);
			}
			else if (parsedLine.at(0) == "angle") {
				angle = strtof(parsedLine.at(1).c_str(), 0);
			}
			else if (parsedLine.at(0) == "resolution") {
				yResolution = atoi(parsedLine.at(1).c_str());
				xResolution = atoi(parsedLine.at(2).c_str());
			}
			else if (parsedLine.at(0) == "b") {
				backgroundFill.color.x = strtof(parsedLine.at(1).c_str(), 0);
				backgroundFill.color.y = strtof(parsedLine.at(2).c_str(), 0);
				backgroundFill.color.z = strtof(parsedLine.at(3).c_str(), 0);
			}
			else if (parsedLine.at(0) == "f") {
				sphereFill.color.x = strtof(parsedLine.at(1).c_str(), 0);
				sphereFill.color.y = strtof(parsedLine.at(2).c_str(), 0);
				sphereFill.color.z = strtof(parsedLine.at(3).c_str(), 0);
				sphereFill.kd = strtof(parsedLine.at(4).c_str(), 0);
				sphereFill.ks = strtof(parsedLine.at(5).c_str(), 0);
				sphereFill.e = strtof(parsedLine.at(6).c_str(), 0);
				sphereFill.kt = strtof(parsedLine.at(7).c_str(), 0);
				sphereFill.ir = strtof(parsedLine.at(8).c_str(), 0);
			}
			else if (parsedLine.at(0) == "s") {
				Sphere sphere;

				sphere.center.x = strtof(parsedLine.at(1).c_str(), 0);
				sphere.center.y = strtof(parsedLine.at(2).c_str(), 0);
				sphere.center.z = strtof(parsedLine.at(3).c_str(), 0);
				sphere.r = strtof(parsedLine.at(4).c_str(), 0);
				sphere.fill = sphereFill;
				spheres.push_back(sphere);
			}
			else if (parsedLine.at(0) == "l") {
				sVector light;

				light.x = strtof(parsedLine.at(1).c_str(), 0);
				light.y = strtof(parsedLine.at(2).c_str(), 0);
				light.z = strtof(parsedLine.at(3).c_str(), 0);
				lights.push_back(light);
			}
		}
		nffFile.close();
	}
}

vector<string> parseLine(string line) {
	char* currentLine = new char[256];
	vector<string> result;

	strcpy(currentLine, line.c_str());

	char * character;

	//tokenize line
	character = strtok(currentLine, " ");
	while (character != NULL)
	{
		//Throw on back of vector
		result.push_back(character);
		//fetch next character
		character = strtok(NULL, " ");
	}

	return result;
}

//Gets the smallest T value, if negative 
float getT(Ray ray, Sphere sphere) {
	float a = ray.direction.dot(ray.direction);
	sVector dc = ray.origin - sphere.center;
	float b = 2 * ray.direction.dot(dc);
	float c = dc.dot(dc) - (sphere.r*sphere.r);

	float discriminant = b*b - 4 * a*c;
	if (discriminant < 0)       // no intersection
		return -1.0;

	// solve quadratic equation for desired surface
	float dsq = sqrtf(discriminant);
	float t = (-b - dsq) / (2 * a);// first intersection within ray extent?
	if (t > EPSILON && t < 1)
		return t;

	t = (-b + dsq) / (2 * a);             // second intersection within ray extent?
	if (t > EPSILON && t < 1)
		return t;

	return -1.0;              // sphere entirely behind start point
}

bool recursionLimit(Ray ray) {
	if (ray.bounces > 5) {
		return true;
	}
	else if (ray.ks < (1 / 255)) {
		return true;
	}

	return false;
}

bool checkForHits(Ray ray, HitRecord& hr) {
	float t;

	// For each sphere, check the intersection
	for (int currSphere = 0; currSphere < (int)spheres.size(); currSphere++) {
		t = getT(ray, spheres[currSphere]);

		if (t >= EPSILON && (hr.t < EPSILON || t < hr.t)) {
			hr.t = t;
			hr.sphere = spheres[currSphere];
		}
	}

	if (hr.t >= 0) {
		//Calculate surface normal
		hr.p = ray.origin + ray.direction * hr.t;

		//Calculate surface normal
		hr.n = hr.p - hr.sphere.center;
		hr.n.normalize();

		return true;
	}

	return false;
}

sVector raycolor(Ray ray) {
	HitRecord sphereRecord, lightRecord;
	sphereRecord.t = -1.0;

	float I = 1 / (float)sqrt(lights.size());
	// If we hit anything, start doing stuff
	if (checkForHits(ray, sphereRecord)) {
		sVector n = sphereRecord.n;
		sVector p = sphereRecord.p;
		sVector d = p - ray.origin;
		d.normalize();

		float kd = sphereRecord.sphere.fill.kd;
		float ks = sphereRecord.sphere.fill.ks;
		ray.ks *= ks;
		float r = sphereRecord.sphere.fill.color.x;
		float g = sphereRecord.sphere.fill.color.y;
		float b = sphereRecord.sphere.fill.color.z;
		float e = sphereRecord.sphere.fill.e;

		ray.bounces++;

		sVector localColor; 
		localColor.x = 0;
		localColor.y = 0;
		localColor.z = 0;

		sVector v = ray.origin - p;
		v.normalize();

		//For each light
		for (int light = 0; light < (int)lights.size(); light++) {
			Ray shadowRay;
			shadowRay.origin = p;

			shadowRay.direction = lights[light] - p;
			//shadowRay.direction.normalize();

			sVector l = shadowRay.direction;
			l.normalize();

			//offset
			shadowRay.origin = p + (l * EPSILON);

			sVector h = v + l;
			h.normalize();

			if (!checkForHits(shadowRay, lightRecord)) {
				diffuse = max<float>(0, n.dot(l));
				specular = pow(max<float>(0, n.dot(h)), e);
				localColor.x += (kd*r + ks*specular) * diffuse * I;
				localColor.y += (kd*g + ks*specular) * diffuse * I;
				localColor.z += (kd*b + ks*specular) * diffuse * I;
			}
		}

		sVector totalColor = localColor;

		//Bounce the lightray
		if (ks > 0 && !recursionLimit(ray)) {
			sVector r = d - n * n.dot(d) * 2;
			r.normalize();

			Ray reflectionRay;
			reflectionRay.origin = p;
			reflectionRay.direction = r;
			reflectionRay.bounces = ray.bounces;
			reflectionRay.ks = ray.ks;
			sVector reflectionColor = raycolor(reflectionRay);

			totalColor = totalColor + (reflectionColor * ks);
		}

		return totalColor;
	}
	else
	{
		return backgroundFill.color;
	}
}

void renderImage() {
	Pixel *data = new Pixel[xResolution * yResolution];
	Column *pixels = new Column[yResolution];

	for (int y = 0; y < yResolution; y++) {
		pixels[y] = &data[xResolution * y];
	}


	//Calculate height
	float d = (float)(xResolution / 2) / (float)(tan(angle / 2 * PI / 180.0));

	//sVector e = from;

	//calculate w
	sVector w = from - at;
	w.normalize();

	// calculate u
	sVector u = up.cross(w);
	u.normalize();

	// calculate v
	sVector v = w.cross(u);
	v.normalize();

	int right = xResolution / 2;
	int left = -1 * right;
	int top = yResolution / 2;
	int bottom = -1 * top;

	Ray pixelRay;
	pixelRay.origin = from;
	float uScale = 0.0;
	float vScale = 0.0;

	//For each pixel, check for intersection
	for (int j = 0; j < yResolution; j++) {
		for (int i = 0; i < xResolution; i++) {
			//Origin
			uScale = left + (right - left) * (float)(i + 0.5) / xResolution;
			vScale = bottom + (top - bottom) * (float)(j + 0.5) / yResolution;

			sVector s = (u * uScale) + (v * vScale) - (w * d);

			pixelRay.direction = s;
			//pixelRay.direction.normalize();
			pixelRay.bounces = 0;

			sVector color = raycolor(pixelRay);

			pixels[yResolution - 1 - j][i][0] = (color.x < 0) ? 0 : (color.x > 1) ? 255 : (unsigned char)(color.x * 255);
			pixels[yResolution - 1 - j][i][1] = (color.y < 0) ? 0 : (color.y > 1) ? 255 : (unsigned char)(color.y * 255);
			pixels[yResolution - 1 - j][i][2] = (color.z < 0) ? 0 : (color.z > 1) ? 255 : (unsigned char)(color.z * 255);
		}
		if (j % 50 == 0)
			cout << "Processing Line: " << j << endl;
	}

	FILE *f = fopen("trace.ppm", "wb");
	fprintf(f, "P6\n%d %d\n%d\n", xResolution, yResolution, 255);
	fwrite(&pixels[0][0][0], 1, xResolution * yResolution * 3, f);
	fclose(f);
}

int main(int argc, char *argv[]) {
	parseNppFile(argv[1]);
	renderImage();

	return 0;
}
