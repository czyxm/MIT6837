#ifndef _GROUP_H_
#define _GROUP_H_

#include "object3d.h"
#include "vectors.h"

class Group : Object3D
{
public:
	// CONSTRUCTOR AND DECONSTRUCTOR
	Group(int size) : size(size)
	{
		objects = new Object3D*[size];
	}
	~Group();

	//	METHOD
	void addObject(int index, Object3D* obj);
	virtual bool intersect(const Ray& r, Hit& h, float tmin);

private:
	int size;
	Object3D **objects;
};

#endif // !_GROUP_H_