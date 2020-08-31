#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

class Material;
class Ray;
class Hit;

class Object3D
{
public:
	// CONSTRUCTOR AND DESTRUCTOR
	Object3D() {}
	Object3D(Material *material) : material(material) {}
	virtual ~Object3D() 
	{
		if (material != nullptr)
		{
			delete material;
		}
	}

	// ACCESSOR
	Material* getMaterial() const { return material; }

	// MODIFIER
	void setMaterial(Material *m) { material = m; }

	// METHOD
	virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;

private:
	Material *material;
};

#endif