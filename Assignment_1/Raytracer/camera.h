#ifndef	_CAMERA_H_
#define _CAMERA_H_

#include "vectors.h"

class Ray;

class Camera
{
public:
	// CONSTRUCTOR AND DESTRUCTOR

	// ACCESSROR

	// MODIFIER

	// METHOD
	virtual Ray generateRay(Vec2f point) = 0;
	virtual float getTMin() const = 0;

private:
	
};

class OrthographicCamera : public Camera
{
public:
	// CONSTRUCTOR AND DESTRUCTOR
	OrthographicCamera(const Vec3f& center, const Vec3f& direction, const Vec3f& up, float size);

	// ACCESSROR
	float getSize() const { return size; }
	const Vec3f& getCenter() const { return center; }
	const Vec3f& getUp() const { return up; }
	const Vec3f& getDirection() const { return direction; }
	const Vec3f& getHorizontal() const { return horizontal; }
	// MODIFIER

	// METHOD
	virtual Ray generateRay(Vec2f point);
	virtual float getTMin() const;

private:
	Vec3f center, up, horizontal, direction;
	float size;
};

#endif