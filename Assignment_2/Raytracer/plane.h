#ifndef _PLANE_H_
#define _PLANE_H_

#include "object3d.h"
#include "vectors.h"

class Plane : public Object3D
{
public:
	// CONSTRUCTOR AND DECONSTRUCTOR
	Plane(const Vec3f& normal, float d, Material* material)
		: normal(normal), d(d), Object3D(material) {}

	// ACCESSOR
	Vec3f getNormal() const { return normal; }
	float getD() const { return d; }

	// MODIFIER

	// METHOD
	virtual bool intersect(const Ray& r, Hit& h, float tmin);

private:
	// REPRESENTATION
	Vec3f normal;
	float d;
};

#endif