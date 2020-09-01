#include <iostream>
#include "triangle.h"
#include "ray.h"
#include "hit.h"
#include "matrix.h"
using namespace std;

bool Triangle::intersect(const Ray& r, Hit& h, float tmin)
{
	Vec3f a_b = a - b, a_c = a - c, a_o = a - r.getOrigin(), rd = r.getDirection();
	float detA = Matrix::det3x3(
		a_b[0], a_c[0], rd[0],
		a_b[1], a_c[1], rd[1],
		a_b[2], a_c[2], rd[2]
	);
	//cout << a_b[0] << " " << a_c[0] << " " << rd[0] << endl;
	//cout << a_b[1] << " " << a_c[1] << " " << rd[1] << endl;
	//cout << a_b[2] << " " << a_c[2] << " " << rd[2] << endl;
	//cout << detA << endl;
	if (detA == 0)
	{
		return false;
	}
	float detX = Matrix::det3x3(
		a_o[0], a_c[0], rd[0],
		a_o[1], a_c[1], rd[1],
		a_o[2], a_c[2], rd[2]
	);
	float detY = Matrix::det3x3(
		a_b[0], a_o[0], rd[0],
		a_b[1], a_o[1], rd[1],
		a_b[2], a_o[2], rd[2]
	);
	float detZ = Matrix::det3x3(
		a_b[0], a_c[0], a_o[0],
		a_b[1], a_c[1], a_o[1],
		a_b[2], a_c[2], a_o[2]
	);
	float beta = detX / detA, gama = detY / detA, t = detZ / detA;
	//cout << detX << ", " << detY << ", " << detZ << ", " << detA << endl;
	//cout << beta << ", " << gama << " ," << t << endl;
	if (t > tmin && beta >= 0 && gama >= 0 && beta + gama <= 1)
	{
		Vec3f normal;
		Vec3f::Cross3(normal, a_b, a_c);
		normal.Normalize();
		h.set(t, getMaterial(), normal, r);
		return true;
	}
	return false;
}