#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_
#include "vectors.h"
#include "matrix.h"

class Material;
class Ray;
class Hit;

class Object3D
{
public:
	// CONSTRUCTOR AND DESTRUCTOR
	Object3D() { material = nullptr; }
	Object3D(Material *material) : material(material) {}

	// ACCESSOR
	Material* getMaterial() const { return material; }

	// MODIFIER
	void setMaterial(Material *m) { material = m; }

	// METHOD
	virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;
	virtual void paint(void) = 0;

private:
	Material *material;
};

// Group
// ==================================================================
class Group : Object3D
{
public:
	// CONSTRUCTOR AND DECONSTRUCTOR
	Group(int size) : size(size)
	{
		objects = new Object3D * [size];
	}
	~Group();

	//	METHOD
	void addObject(int index, Object3D* obj);
	virtual bool intersect(const Ray& r, Hit& h, float tmin);
	virtual void paint(void);

private:
	int size;
	Object3D** objects;
};

// Transform
// ==================================================================
class Transform : public Object3D
{
public:
	// CONSTRUCTOR AND DECONSTRUCTOR
	Transform(Matrix& m, Object3D* o)
		: matrix(m), object(o) {}

	// ACCESSOR
	const Matrix& getMatrix() const { return matrix; }
	Object3D* getObject() const { return object; }

	// MODIFIER

	// METHOD
	virtual bool intersect(const Ray& r, Hit& h, float tmin);
	virtual void paint(void);

private:
	// REPRESENTATION
	Matrix matrix;
	Object3D* object;
};

// Triangle
// ==================================================================
class Triangle : public Object3D
{
public:
	// CONSTRUCTOR AND DECONSTRUCTOR
	Triangle(const Vec3f& a, const Vec3f& b, const Vec3f& c, Material* material)
		: a(a), b(b), c(c), Object3D(material)
	{
		Vec3f::Cross3(normal, a - b, a - c);
		normal.Normalize();
	}

	// ACCESSOR
	Vec3f getNormal() const { return normal; };
	// MODIFIER

	// METHOD
	virtual bool intersect(const Ray& r, Hit& h, float tmin);
	virtual void paint(void);

private:
	// REPRESENTATION
	Vec3f a, b, c, normal;
};

// Sphere
// ==================================================================
class Sphere : public Object3D
{
public:
	// CONSTRUCTOR AND DECONSTRUCTOR
	Sphere(const Vec3f& center, float radius, Material* material)
		: center(center), radius(radius), Object3D(material) {}

	// ACCESSOR
	const Vec3f& getCenter() const { return center; }
	float getRadius() const { return radius; }

	// MODIFIER

	// METHOD
	virtual bool intersect(const Ray& r, Hit& h, float tmin);
	virtual void paint(void);
	Vec3f getPositionAt(float _phi, float _theta) const;

private:
	// REPRESENTATION
	Vec3f center;
	float radius;
};

// Plane
// ==================================================================
class Plane : public Object3D
{
public:
	// CONSTRUCTOR AND DECONSTRUCTOR
	Plane(const Vec3f& normal, float d, Material* material)
		: normal(normal), d(d), Object3D(material) 
	{
		this->normal.Normalize();
	}

	// ACCESSOR
	Vec3f getNormal() const { return normal; }
	float getD() const { return d; }

	// MODIFIER

	// METHOD
	virtual bool intersect(const Ray& r, Hit& h, float tmin);
	virtual void paint(void);

private:
	// REPRESENTATION
	Vec3f normal;
	float d;
};

#endif