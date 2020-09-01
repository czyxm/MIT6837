#include <iostream>
#include "transform.h"
#include "ray.h"
#include "hit.h"
using namespace std;

bool Transform::intersect(const Ray& r, Hit& h, float tmin)
{
	// Transform Ray
	Matrix M(matrix);
	M.Inverse();
	Vec4f _origin = M * Vec4f(r.getOrigin(), 1);
	Vec3f _direction = M * r.getDirection();
	float length = _direction.Length();
	_direction.Normalize();
	Ray _r(Vec3f(_origin[0], _origin[1], _origin[2]), _direction);
	// Intersect Object
	if (object->intersect(_r, h, tmin))
	{
		// Restore Hit
		M.Transpose();
		Vec3f normal = M * h.getNormal();
		normal.Normalize();
		h.set(h.getT() / length, h.getMaterial(), normal, r);
		return true;
	}
	return false;
}