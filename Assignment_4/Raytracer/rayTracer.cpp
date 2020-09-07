#include "rayTracer.h"
#include "rayTree.h"
#include "scene_parser.h"
#include "object3d.h" 
#include "material.h"
#include "camera.h" 
#include "ray.h"
#include "hit.h"
#include "light.h"

float RayTracer::epsilon = 0.0001f;

Vec3f RayTracer::mirrorDirection(const Vec3f& normal, const Vec3f& incoming) const
{
	Vec3f mirriorDir = incoming - 2 * incoming.Dot3(normal) * normal;
	mirriorDir.Normalize();
	return mirriorDir;
}

Vec3f RayTracer::transmittedDirection(const Vec3f& normal, const Vec3f& incoming, float index_i, float index_t) const
{
	float nr = index_i / index_t, dotNI = normal.Dot3(incoming);
	Vec3f transmittedDir = (nr * dotNI - sqrtf(1 - nr * nr * (1 - dotNI * dotNI))) * normal - nr * incoming;
	transmittedDir.Normalize();
	return transmittedDir;
}

Vec3f RayTracer::traceRay(Ray& ray, float tmin, int bounces, float weight, Hit& hit) const
{
	Vec3f color = scene->getBackgroundColor();
	if (scene->getGroup()->intersect(ray, hit, tmin))
	{
		assert(hit.getMaterial() != nullptr);
		PhongMaterial* material = dynamic_cast<PhongMaterial*>(hit.getMaterial());
		Vec3f hitPoint = hit.getIntersectionPoint(), directionToLight, lightColor;
		float index_i = 1, index_t = material->getIndexOfRefraction();
		float distanceToLight;
		color = scene->getAmbientLight() * material->getDiffuseColor();
		// back face
		if (hit.getNormal().Dot3(ray.getDirection()) > 0)
		{
			index_i = index_t;
			index_t = 1;
			if (shadeBack)
			{
				hit.setNormal(-1.f * hit.getNormal());
			}
		}
		for (int i = 0; i < scene->getNumLights(); i++)
		{
			scene->getLight(i)->getIllumination(hitPoint, directionToLight, lightColor, distanceToLight);
			if (shadows)
			{
				Ray shadowRay(hitPoint, directionToLight);
				Hit shadowHit(distanceToLight, nullptr, Vec3f(0, 0, 0));
				scene->getGroup()->intersectShadowRay(shadowRay, shadowHit, RayTracer::epsilon, distanceToLight, transparentShadow, lightColor);
				RayTree::AddShadowSegment(shadowRay, RayTracer::epsilon, distanceToLight);
			}
			color += material->Shade(ray, hit, directionToLight, lightColor);
		}
		if (bounces < maxBounces && weight >= cutoffWeight)
		{
			float _weight = weight * material->getReflectiveColor().r();
			// Reflective material
			if (material->isReflective())
			{
				Ray reflectionRay(hitPoint, mirrorDirection(hit.getNormal(), ray.getDirection()));
				Hit reflectionHit(INFINITY, nullptr, Vec3f(0, 0, 0));
				color += material->getReflectiveColor() * traceRay(reflectionRay, RayTracer::epsilon, bounces + 1, _weight, reflectionHit);
				RayTree::AddReflectedSegment(reflectionRay, 0, reflectionHit.getT());
			}
			//else
			//{
			//	cout << "NOREF " << bounces << "," << weight << endl;
			//}
			// Transparent material
			if (material->isTransparent())
			{
				Ray transmittedRay(hitPoint, transmittedDirection(hit.getNormal(), -1 * ray.getDirection(), index_i, index_t));
				Hit transmittedHit(INFINITY, nullptr, Vec3f(0, 0, 0));
				color += material->getTransparentColor() * traceRay(transmittedRay, RayTracer::epsilon, bounces + 1, _weight, transmittedHit);
				RayTree::AddTransmittedSegment(transmittedRay, 0, transmittedHit.getT());
			}
			//else
			//{
			//	cout << "NOTRA " << bounces << "," << weight << endl;
			//}
		}
		//else
		//{
		//	cout << "STOP " << bounces << "," << weight << endl;
		//}
	}
	//else
	//{
	//	cout << "NOHIT " << bounces << "," << weight << endl;
	//}
	return color;
}
