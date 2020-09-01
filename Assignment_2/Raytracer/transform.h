#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "object3d.h"
#include "vectors.h"
#include "matrix.h"

class Transform : public Object3D
{
public:
	// CONSTRUCTOR AND DECONSTRUCTOR
	Transform(Matrix &m, Object3D *o)
		: matrix(m), object(o) {}

	// ACCESSOR
	const Matrix& getMatrix() const { return matrix; }
	Object3D* getObject() const { return object; }

	// MODIFIER

	// METHOD
	virtual bool intersect(const Ray& r, Hit& h, float tmin);

private:
	// REPRESENTATION
	Matrix matrix;
	Object3D* object;
};

#endif