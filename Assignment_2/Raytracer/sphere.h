#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "object3d.h"
#include "vectors.h"

class Sphere : public Object3D
{
public:
	// CONSTRUCTOR AND DECONSTRUCTOR
	Sphere(const Vec3f &center, float radius, Material *material)
		: center(center), radius(radius), Object3D(material) {}

	// ACCESSOR
	const Vec3f& getCenter() const { return center; }
	float getRadius() const { return radius; }

	// MODIFIER

	// METHOD
	virtual bool intersect(const Ray &r, Hit &h, float tmin);

private:
	// REPRESENTATION
	Vec3f center;
	float radius;
};

#endif