#include <iostream>
#include "sphere.h"
#include "ray.h"
#include "hit.h"
using namespace std;

bool Sphere::intersect(const Ray& r, Hit& h, float tmin)
{
	Vec3f R0 = center - r.getOrigin(), Rd = r.getDirection(), normal;
	float r2 = radius * radius, d2 = R0.Dot3(R0), tp = R0.Dot3(Rd), h2 = d2 - tp * tp;
	// No hit
	if ((h2 > r2) || (d2 > r2 && tp < 0))
	{
		return false;
	}
	float _t = sqrtf(r2 - h2), t = tmin;
	if (d2 != r2)
	{
		t = d2 > r2 ? tp - _t : tp + _t;
		normal = r.pointAtParameter(t) - center;
		normal.Normalize();
		h.set(t, getMaterial(), normal, r);
	}
	else
	{
		cout << "[LOG] Origin on the sphere" << endl;
	}
	return true;
}