#ifndef	_CAMERA_H_
#define _CAMERA_H_

#include "vectors.h"
#include "matrix.h"

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
	const Vec3f& getScreenUp() const { return screenUp; }
	// MODIFIER
	void updateAxes()
	{
		Vec3f::Cross3(horizontal, direction, up);
		Vec3f::Cross3(screenUp, horizontal, direction);
		screenUp.Normalize();
		horizontal.Normalize();
		direction.Normalize();
	}
	// METHOD
	virtual Ray generateRay(const Vec2f& point) = 0;
	virtual float getTMin() const = 0;
	virtual void glInit(int w, int h) = 0;
	virtual void glPlaceCamera(void) = 0;
	virtual void dollyCamera(float dist) = 0;
	virtual void truckCamera(float dx, float dy) = 0;
	virtual void rotateCamera(float rx, float ry) = 0;

protected:
	Vec3f center, up, horizontal, direction, screenUp;
};

class OrthographicCamera : public Camera
{
public:
	// CONSTRUCTOR AND DESTRUCTOR
	OrthographicCamera(const Vec3f& center, const Vec3f& direction, const Vec3f& up, float size)
		: size(size), Camera(center, direction, up){}

	// ACCESSROR
	virtual float getSize() const { return size; }
	
	// MODIFIER

	// METHOD
	virtual Ray generateRay(const Vec2f &point);
	virtual float getTMin() const;
	virtual void glInit(int w, int h);
	virtual void glPlaceCamera(void);
	virtual void dollyCamera(float dist);
	virtual void truckCamera(float dx, float dy);
	virtual void rotateCamera(float rx, float ry);

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
	virtual float getSize() const { return 1; }
	// MODIFIER

	// METHOD
	virtual Ray generateRay(const Vec2f& point);
	virtual float getTMin() const;
	virtual void glInit(int w, int h);
	virtual void glPlaceCamera(void);
	virtual void dollyCamera(float dist);
	virtual void truckCamera(float dx, float dy);
	virtual void rotateCamera(float rx, float ry);

private:
	float angle;
};

#endif