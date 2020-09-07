#ifndef _RayTracer_H_
#define _RayTracer_H_

#include "vectors.h"

class SceneParser;
class Ray;
class Hit;

class RayTracer
{
public:
	RayTracer(SceneParser* scene, int maxBounces, float cutoffWeight, bool shadows, bool transparentShadow, bool shadeBack)
	: scene(scene), maxBounces(maxBounces), cutoffWeight(cutoffWeight), shadows(shadows), transparentShadow(transparentShadow), shadeBack(shadeBack) {}

	static float epsilon;
	Vec3f traceRay(Ray& ray, float tmin, int bounces, float weight, Hit& hit) const;
	Vec3f mirrorDirection(const Vec3f& normal, const Vec3f& incoming) const;
	Vec3f transmittedDirection(const Vec3f& normal, const Vec3f& incoming, float index_i, float index_t) const;

private:
	SceneParser* scene;
	int maxBounces;
	float cutoffWeight;
	bool shadows, transparentShadow, shadeBack;
};

#endif