#define _USE_MATH_DEFINES
#define DegreesToRadians(x) ((M_PI * x) / 180.0f)

#include "object3d.h"
#include "hit.h"
#include "ray.h"
#include "material.h"
#include "rayTracer.h"
#include <GL/glut.h>
#include <assert.h>
#include <iostream>
using namespace std;
extern int phiStep, thetaStep;
extern bool gouraud;

bool Object3D::intersectShadowRay(const Ray& r, Hit& h, float tmin, float tmax, bool transparentShadow, Vec3f& lightColor)
{
	if (intersect(r, h, tmin) && h.getT() < tmax)
	{
		PhongMaterial* m = dynamic_cast<PhongMaterial*>(h.getMaterial());
		if (transparentShadow && m->isTransparent())
		{
			lightColor = lightColor * m->getTransparentColor();
		}
		else
		{
			lightColor *= 0.f;
		}
		return true;
	}
	return false;
}

// Group
// ======================================================
Group::~Group()
{
	for (int i = 0; i < size; i++)
	{
		if (objects[i] != nullptr)
		{
			delete objects[i];
		}
	}
	delete [] objects;
}

void Group::addObject(int index, Object3D* obj)
{
	assert(index >= 0 && index < size);
	objects[index] = obj;
}

bool Group::intersect(const Ray& r, Hit& h, float tmin)
{
	bool res = false;
	for (int i = 0; i < size; i++)
	{
		if (objects[i] != nullptr)
		{
			Hit _h(tmin, nullptr, Vec3f(0, 0, 0));
			bool isHit = objects[i]->intersect(r, _h, tmin);
			if (isHit && (res == false || _h.getT() < h.getT()))
			{
				res = true;
				h = _h;
			}
		}
	}
	return res;
}

bool Group::intersectShadowRay(const Ray& r, Hit& h, float tmin, float tmax, bool transparentShadow, Vec3f& lightColor)
{
	bool res = false;
	int count = 0;
	for (int i = 0; i < size; i++)
	{
		if (objects[i] != nullptr && objects[i]->intersectShadowRay(r, h, tmin, tmax, transparentShadow, lightColor))
		{
			if (!transparentShadow)
			{
				return true;
			}
			res = true;
		}
	}
	return res;
}

void Group::paint(void)
{
	for (int i = 0; i < size; i++)
	{
		if (objects[i] != nullptr)
		{
			objects[i]->paint();
		}
	}
}

// Transform
// ======================================================
bool Transform::intersect(const Ray& r, Hit& h, float tmin)
{
	// Transform Ray
	Matrix M(matrix);
	M.Inverse();
	Vec4f _origin = M * Vec4f(r.getOrigin(), 1);
	Vec3f _direction = M * r.getDirection();
	float length = _direction.Length();
	_direction.Normalize();
	Ray _r(Vec3f(_origin[0], _origin[1], _origin[2]), _direction);
	// Intersect Object
	if (object->intersect(_r, h, tmin))
	{
		// Restore Hit
		M.Transpose();
		Vec3f normal = M * h.getNormal();
		normal.Normalize();
		h.set(h.getT() / length, h.getMaterial(), normal, r);
		return true;
	}
	return false;
}

bool Transform::intersectShadowRay(const Ray& r, Hit& h, float tmin, float tmax, bool transparentShadow, Vec3f& lightColor)
{
	// Transform Ray
	Matrix M(matrix);
	M.Inverse();
	Vec4f _origin = M * Vec4f(r.getOrigin(), 1);
	Vec3f _direction = M * r.getDirection();
	float length = _direction.Length();
	_direction.Normalize();
	Ray _r(Vec3f(_origin[0], _origin[1], _origin[2]), _direction);
	// Intersect Object
	if (object->intersectShadowRay(_r, h, tmin, tmax, transparentShadow, lightColor))
	{
		// Restore Hit
		M.Transpose();
		Vec3f normal = M * h.getNormal();
		normal.Normalize();
		h.set(h.getT() / length, h.getMaterial(), normal, r);
		return true;
	}
	return false;
}

void Transform::paint(void)
{
	// Push
	glPushMatrix();
	// Transform Matrix
	GLfloat* glMatrix = matrix.glGet();
	glMultMatrixf(glMatrix);
	delete[] glMatrix;
	// Paint Object
	object->paint();
	// Pop
	glPopMatrix();
}

// Triangle
// ======================================================
bool Triangle::intersect(const Ray& r, Hit& h, float tmin)
{
	Vec3f a_b = a - b, a_c = a - c, a_o = a - r.getOrigin(), rd = r.getDirection();
	float detA = Matrix::det3x3(
		a_b[0], a_c[0], rd[0],
		a_b[1], a_c[1], rd[1],
		a_b[2], a_c[2], rd[2]
	);
	//cout << a_b[0] << " " << a_c[0] << " " << rd[0] << endl;
	//cout << a_b[1] << " " << a_c[1] << " " << rd[1] << endl;
	//cout << a_b[2] << " " << a_c[2] << " " << rd[2] << endl;
	//cout << detA << endl;
	if (detA == 0)
	{
		return false;
	}
	float detX = Matrix::det3x3(
		a_o[0], a_c[0], rd[0],
		a_o[1], a_c[1], rd[1],
		a_o[2], a_c[2], rd[2]
	);
	float detY = Matrix::det3x3(
		a_b[0], a_o[0], rd[0],
		a_b[1], a_o[1], rd[1],
		a_b[2], a_o[2], rd[2]
	);
	float detZ = Matrix::det3x3(
		a_b[0], a_c[0], a_o[0],
		a_b[1], a_c[1], a_o[1],
		a_b[2], a_c[2], a_o[2]
	);
	float beta = detX / detA, gama = detY / detA, t = detZ / detA;
	//cout << detX << ", " << detY << ", " << detZ << ", " << detA << endl;
	//cout << beta << ", " << gama << " ," << t << endl;
	if (t >= tmin && beta >= 0 && gama >= 0 && beta + gama <= 1)
	{
		h.set(t, getMaterial(), normal, r);
		return true;
	}
	return false;
}

void Triangle::paint(void)
{
	glBegin(GL_TRIANGLES);	
	getMaterial()->glSetMaterial();
	glNormal3f(normal.x(), normal.y(), normal.z());
	glVertex3f(a.x(), a.y(), a.z());
	glVertex3f(b.x(), b.y(), b.z());
	glVertex3f(c.x(), c.y(), c.z());
	glEnd();
}

// Sphere
// ======================================================
bool Sphere::intersect(const Ray& r, Hit& h, float tmin)
{
	Vec3f R0 = center - r.getOrigin(), Rd = r.getDirection(), normal;
	float r2 = radius * radius, d2 = R0.Dot3(R0), tp = R0.Dot3(Rd), h2 = d2 - tp * tp;
	// No hit
	if ((h2 > r2) || (d2 > r2 && tp < 0))
	{
		return false;
	}
	float _t = sqrtf(r2 - h2), t = (d2 - r2 > RayTracer::epsilon) ? (tp - _t) : (tp + _t);
	//cout << "d2: " << d2 << " r2: " << r2 << " tp: " << tp << " _t: " << _t << " t: " << t << "?" << tmin << endl;
	if (t < tmin)
	{
		return false;
	}
	normal = r.pointAtParameter(t) - center;
	normal.Normalize();
	h.set(t, getMaterial(), normal, r);
	return true;
}

Vec3f Sphere::getPositionAt(float _phi, float _theta) const
{
	Vec3f pos(0, -radius, 0);
	_phi = DegreesToRadians(_phi);
	_theta = DegreesToRadians(_theta);
	return center + Matrix::MakeYRotation(_phi) * Matrix::MakeXRotation(_theta) * pos;
}

void Sphere::paint(void)
{
	Vec3f p1, p2, p3, p4, n;
	glBegin(GL_QUADS);
	getMaterial()->glSetMaterial();
	p2 = getPositionAt(-90, 0);
	int deltaPhi = (int)(180.f / phiStep), deltaTheta = (int)(360.f / thetaStep);
	for (int iPhi = deltaPhi - 90; iPhi < deltaPhi + 90; iPhi += deltaPhi)
	{
		float _phi = fminf(90, iPhi);
		p1 = p2;
		p2 = getPositionAt(_phi, 0);
		for (int iTheta = 0; iTheta < 360; iTheta += deltaTheta)
		{
			// compute appropriate coordinates & normals
			float _theta = fminf(360, iTheta + deltaTheta);
			p3 = getPositionAt(_phi, _theta);
			p4 = getPositionAt(_phi - deltaPhi, _theta);
			// send gl vertex commands
			if (gouraud)
			{
				n = p1 - center;
				n.Normalize();
				glNormal3f(n.x(), n.y(), n.z());
				glVertex3f(p1.x(), p1.y(), p1.z());
				n = p2 - center;
				n.Normalize();
				glNormal3f(n.x(), n.y(), n.z());
				glVertex3f(p2.x(), p2.y(), p2.z());
				n = p3 - center;
				n.Normalize();
				glNormal3f(n.x(), n.y(), n.z());
				glVertex3f(p3.x(), p3.y(), p3.z());
				n = p4 - center;
				n.Normalize();
				glNormal3f(n.x(), n.y(), n.z());
				glVertex3f(p4.x(), p4.y(), p4.z());
			}
			else
			{
				Vec3f::Cross3(n, p3 - p1, p2 - p4);
				n.Normalize();
				glNormal3f(n.x(), n.y(), n.z());
				glVertex3f(p1.x(), p1.y(), p1.z());
				glVertex3f(p2.x(), p2.y(), p2.z());
				glVertex3f(p3.x(), p3.y(), p3.z());
				glVertex3f(p4.x(), p4.y(), p4.z());
			}
			p1 = p4;
			p2 = p3;
		}
	}
	glEnd();
}

// Plane
// ======================================================
bool Plane::intersect(const Ray& r, Hit& h, float tmin)
{
	float t = (d - r.getOrigin().Dot3(normal)) / r.getDirection().Dot3(normal);
	if (t < tmin || t < 0)
	{
		return false;
	}
	h.set(t, getMaterial(), normal, r);
	return true;
}

void Plane::paint(void)
{
	Vec3f v = fabsf(normal.Dot3(Vec3f(1, 0, 0))) == 1 ? Vec3f(0, 1, 0) : Vec3f(1, 0, 0);
	Vec3f o, b1, b2, p1, p2, p3, p4;
	Vec3f::Cross3(b1, v, normal);
	Vec3f::Cross3(b2, normal, b1);
	float halfBig = 1000.f;
	b1.Normalize();
	b2.Normalize();
	o = d * normal;
	p1 = o - halfBig * b1 - halfBig * b2;
	p2 = o - halfBig * b1 + halfBig * b2;
	p3 = o + halfBig * b1 + halfBig * b2;
	p4 = o + halfBig * b1 - halfBig * b2;
	
	glBegin(GL_QUADS);
	getMaterial()->glSetMaterial();
	glNormal3f(normal.x(), normal.y(), normal.z());
	glVertex3f(p1.x(), p1.y(), p1.z());
	glVertex3f(p2.x(), p2.y(), p2.z());
	glVertex3f(p3.x(), p3.y(), p3.z());
	glVertex3f(p4.x(), p4.y(), p4.z());
	glEnd();
}