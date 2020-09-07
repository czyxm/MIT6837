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
    PhongMaterial(const Vec3f& diffuseColor, const Vec3f& specularColor, float exponent,
                  const Vec3f& reflectiveColor, const Vec3f& transparentColor, float indexOfRefraction)
        : Material(diffuseColor), specularColor(specularColor), exponent(exponent),
        reflectiveColor(reflectiveColor), transparentColor(transparentColor), indexOfRefraction(indexOfRefraction) 
    {
        _isReflective = this->reflectiveColor != Vec3f(0, 0, 0);
        _isTransparent = this->transparentColor != Vec3f(0, 0, 0);
    }

    // ACCESSORS
    Vec3f getSpecularColor() const { return specularColor; }
    Vec3f getReflectiveColor() const { return reflectiveColor; }
    Vec3f getTransparentColor() const { return transparentColor; }
    float getExponent() const { return exponent; }
    float getIndexOfRefraction() const { return indexOfRefraction; }
    bool isReflective() const { return _isReflective; }
    bool isTransparent() const { return _isTransparent; }
    // METHOD
    virtual void glSetMaterial(void) const;
    virtual Vec3f Shade(const Ray& ray, const Hit& hit, const Vec3f& lightDir, const Vec3f& lightColor) const;
    

private:
    Vec3f specularColor;
    Vec3f reflectiveColor;
    Vec3f transparentColor;
    float exponent, indexOfRefraction;
    bool _isReflective, _isTransparent;
};

// ====================================================================
// ====================================================================

#endif