#include <iostream>
#include "scene_parser.h"
#include "group.h" 
#include "material.h"
#include "camera.h" 
#include "ray.h"
#include "hit.h"
#include "image.h"
#include "light.h"

using namespace std;

// ========================================================
// ========================================================
// Some sample code you might like to use for parsing 
// command line arguments 
// sample command line:
// raytracer -input scene1_1.txt -size 200 200 -output output1_1.tga -depth 9 10 depth1_1.tga

char* input_file = NULL;
int width = 100;
int height = 100;
char* output_file = NULL;
float depth_min = 0;
float depth_max = 1;
char* depth_file = NULL;
char* normal_file = NULL;
bool shade_back = false;

float clamp(float value, float low, float high);
void parseCmd(int argc, char** argv);
Vec3f calcColor(const Hit& h, const SceneParser &scene);
Vec3f absolute(const Vec3f & vec)
{
	return Vec3f(fabsf(vec[0]), fabsf(vec[1]), fabsf(vec[2]));
}

int main(int argc, char** argv)
{
	// Parse Command Line
	parseCmd(argc, argv);
	
	// Initialization
	SceneParser scene(input_file);
	Camera* camera = scene.getCamera();
	Group* group = scene.getGroup();
	Image renderImg(width, height), depthImg(width, height), normalImg(width, height);
	renderImg.SetAllPixels(scene.getBackgroundColor());
	depthImg.SetAllPixels(Vec3f(0, 0, 0));
	normalImg.SetAllPixels(Vec3f(0, 0, 0));
	float halfWidth = width / 2.f, halfHeight = height / 2.f;
	
	// Scan Loop
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			Vec2f pos((i - halfWidth) * camera->getSize() / width, (j - halfHeight) * camera->getSize() / height);
			Ray ray = camera->generateRay(pos);
			Hit hit(camera->getTMin(), nullptr, Vec3f(0, 0, 0));
			if (group->intersect(ray, hit, camera->getTMin()) && hit.getMaterial() != nullptr)
			{
				// back face
				if (hit.getNormal().Dot3(ray.getDirection()) > 0)
				{
					hit.setNormal((shade_back ? -1.f : 0.f) * hit.getNormal());
					renderImg.SetPixel(i, j, shade_back ? calcColor(hit, scene) : Vec3f(0, 0, 0));
				}
				else
				{
					// render mode
					renderImg.SetPixel(i, j, calcColor(hit, scene));
				}
				// depth mode
				if (depth_file != NULL)
				{
					float depth = 1 - clamp(hit.getT(), depth_min, depth_max);
					depthImg.SetPixel(i, j, Vec3f(depth, depth, depth));
				}
				// normal mode
				if (normal_file != NULL)
				{
					normalImg.SetPixel(i, j, absolute(hit.getNormal()));
				}
			}
		}
	}

	// Save Image
	renderImg.SaveTGA(output_file);
	if (depth_file != NULL)
	{
		depthImg.SaveTGA(depth_file);
	}
	if (normal_file != NULL)
	{
		normalImg.SaveTGA(normal_file);
	}
	cout << "FINISHED!" << endl;
	return 0;
}

Vec3f calcColor(const Hit& h, const SceneParser& scene)
{
	Vec3f color = scene.getAmbientLight(), lightDir, lightColor, normal;
	for (int i = 0; i < scene.getNumLights(); i++)
	{
		scene.getLight(i)->getIllumination(h.getIntersectionPoint(), lightDir, lightColor);
		lightDir.Normalize();
		normal = h.getNormal();
		normal.Normalize();
		color += clamp(lightDir.Dot3(normal), 0, 1) * lightColor;
	}
	Vec3f::Mult(color, color, h.getMaterial()->getDiffuseColor());
	return color;
}

void parseCmd(int argc, char** argv)
{
	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-input")) {
			i++; assert(i < argc);
			input_file = argv[i];
		}
		else if (!strcmp(argv[i], "-size")) {
			i++; assert(i < argc);
			width = atoi(argv[i]);
			i++; assert(i < argc);
			height = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-output"))
		{
			i++; assert(i < argc);
			output_file = argv[i];
		}
		else if (!strcmp(argv[i], "-depth"))
		{
			i++; assert(i < argc);
			depth_min = atof(argv[i]);
			i++; assert(i < argc);
			depth_max = atof(argv[i]);
			i++; assert(i < argc);
			depth_file = argv[i];
		}
		else if (!strcmp(argv[i], "-normals"))
		{
			i++; assert(i < argc);
			normal_file = argv[i];
		}
		else if (!strcmp(argv[i], "-shade_back"))
		{
			i++;
			shade_back = true;
		}
		else
		{
			printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
			assert(0);
		}
	}
}

float clamp(float value, float low, float high)
{
	assert(low <= high);
	if (value <= low)
	{
		return 0;
	}
	else if (value >= high)
	{
		return 1;
	}
	else
	{
		return (value - low) / (high - low);
	}
}