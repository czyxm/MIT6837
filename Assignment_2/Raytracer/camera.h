#ifndef	_CAMERA_H_
#define _CAMERA_H_

#include "vectors.h"

class Ray;

class Camera
{
public:
	// CONSTRUCTOR AND DESTRUCTOR
	Camera(const Vec3f& center, const Vec3f& direction, const Vec3f& up);

	// ACCESSROR
	virtual float getSize() const = 0;
	const Vec3f& getCenter() const { return center; }
	const Vec3f& getUp() const { return up; }
	const Vec3f& getDirection() const { return direction; }
	const Vec3f& getHorizontal() const { return horizontal; }
	// MODIFIER

	// METHOD
	virtual Ray generateRay(const Vec2f& point) = 0;
	virtual float getTMin() const = 0;

private:
	Vec3f center, up, horizontal, direction;
};

class OrthographicCamera : public Camera
{
public:
	// CONSTRUCTOR AND DESTRUCTOR
	OrthographicCamera(const Vec3f& center, const Vec3f& direction, const Vec3f& up, float size)
		: size(size), Camera(center, direction, up){}

	// ACCESSROR
	float getSize() const { return size; }
	
	// MODIFIER

	// METHOD
	virtual Ray generateRay(const Vec2f &point);
	virtual float getTMin() const;

private:
	float size;
};

class PerspectiveCamera : public Camera
{
public:
	// CONSTRUCTOR AND DESTRUCTOR
	PerspectiveCamera(const Vec3f& center, const Vec3f& direction, const Vec3f& up, float angle)
		: angle(angle), Camera(center, direction, up) {}

	// ACCESSROR
	float getAngle() const { return angle; }
	float getSize() const { return 1; }
	// MODIFIER

	// METHOD
	virtual Ray generateRay(const Vec2f& point);
	virtual float getTMin() const;

private:
	float angle;
};

#endif