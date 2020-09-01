#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "object3d.h"
#include "vectors.h"

class Triangle : public Object3D
{
public:
	// CONSTRUCTOR AND DECONSTRUCTOR
	Triangle(const Vec3f &a, const Vec3f& b, const Vec3f& c, Material *material)
		: a(a), b(b), c(c), Object3D(material) {}

	// ACCESSOR

	// MODIFIER

	// METHOD
	virtual bool intersect(const Ray& r, Hit& h, float tmin);

private:
	// REPRESENTATION
	Vec3f a, b, c;
};

#endif