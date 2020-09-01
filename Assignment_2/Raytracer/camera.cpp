#include "camera.h"
#include "ray.h"

Camera::Camera(const Vec3f& center, const Vec3f& direction, const Vec3f& up)
	: center(center), direction(direction), up(up)
{
	Vec3f::Cross3(horizontal, direction, up);
	Vec3f::Cross3(this->up, horizontal, direction);
	this->up.Normalize();
	this->direction.Normalize();
	this->horizontal.Normalize();
}

Ray OrthographicCamera::generateRay(const Vec2f &point)
{
	return Ray(getCenter() + point.x() * getHorizontal() + point.y() * getUp(), getDirection());
}

float OrthographicCamera::getTMin() const
{
	return -FLT_MAX;
}

Ray PerspectiveCamera::generateRay(const Vec2f& point)
{
	Vec3f direction = 0.5f / tanf(angle / 2) * getDirection() + point.x() * getHorizontal() + point.y() * getUp();
	direction.Normalize();
	return Ray(getCenter(), direction);
}

float PerspectiveCamera::getTMin() const
{
	return 0;
}