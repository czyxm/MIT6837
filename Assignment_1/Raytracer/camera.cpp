#include "camera.h"
#include "ray.h"

OrthographicCamera::OrthographicCamera(const Vec3f& center, const Vec3f& direction, const Vec3f& up, float size)
: center(center), direction(direction), up(up), size(size)
{
	Vec3f::Cross3(horizontal, direction, up);
	Vec3f::Cross3(this->up, horizontal, direction);
	this->up.Normalize();
	this->direction.Normalize();
	this->horizontal.Normalize();
}

Ray OrthographicCamera::generateRay(Vec2f point)
{
	return Ray(center + point.x() * horizontal + point.y() * up, direction);
}

float OrthographicCamera::getTMin() const
{
	return -FLT_MAX;
}