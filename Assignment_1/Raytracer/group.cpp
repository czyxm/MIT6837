#include <assert.h>
#include "group.h"
#include "hit.h"

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
			Hit _h(tmin, nullptr);
			bool isHit = objects[i]->intersect(r, _h, tmin);
			//if (isHit)
			//{
			//	cout << "OBJ-" << i << " is hitted by " << r << " at " << _h << endl;
			//}
			if (isHit && (h.getT() == tmin || _h.getT() < h.getT()))
			{
				res = true;
				//cout << "[REplace] " << h << " to " << _h << endl;
				h = _h;
			}
		}
	}
	return res;
}