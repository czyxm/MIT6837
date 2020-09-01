#include <iostream>
#include "plane.h"
#include "ray.h"
#include "hit.h"
using namespace std;

bool Plane::intersect(const Ray& r, Hit& h, float tmin)
{
	float t = (d - r.getOrigin().Dot3(normal)) / r.getDirection().Dot3(normal);
	if (t < tmin || t < 0)
	{
		return false;
	}
	h.set(t, getMaterial(), normal, r);
	return true;
}