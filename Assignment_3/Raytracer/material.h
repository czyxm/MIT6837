#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "vectors.h"
class Ray;
class Hit;

// ====================================================================
// ====================================================================

// You will extend this class in later assignments

class Material {

public:
    // CONSTRUCTORS & DESTRUCTOR
    Material() {}
    Material(const Vec3f& d_color) { diffuseColor = d_color; }
    virtual ~Material() {}

    // ACCESSORS
    Vec3f getDiffuseColor() const { return diffuseColor; }

    // METHOD
    static float clamp(float value, float low, float high);
    virtual void glSetMaterial(void) const = 0;
    virtual Vec3f Shade(const Ray& ray, const Hit& hit, const Vec3f& lightDir, const Vec3f& lightColor) const = 0;

protected:

    // REPRESENTATION
    Vec3f diffuseColor;

};

class PhongMaterial : public Material
{
public:
    // CONSTRUCTORS & DESTRUCTOR
    PhongMaterial(const Vec3f& diffuseColor, const Vec3f& specularColor, float exponent)
        : Material(diffuseColor), specularColor(specularColor), exponent(exponent){}

    // ACCESSORS
    Vec3f getSpecularColor() const { return specularColor; }

    // METHOD
    virtual void glSetMaterial(void) const;
    virtual Vec3f Shade(const Ray& ray, const Hit& hit, const Vec3f& lightDir, const Vec3f& lightColor) const;
    

private:
    Vec3f specularColor;
    float exponent;
};

// ====================================================================
// ====================================================================

#endif