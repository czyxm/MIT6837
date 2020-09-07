#include "scene_parser.h"
#include "camera.h" 
#include "ray.h"
#include "hit.h"
#include "rayTracer.h"
#include "rayTree.h"
#include "glCanvas.h"
#include "image.h"
#include <iostream>
using namespace std;

// ========================================================
// ========================================================
// Some sample code you might like to use for parsing 
// command line arguments 
// sample command line:
// raytracer -input scene1_1.txt -size 200 200 -output output1_1.tga -depth 9 10 depth1_1.tga

SceneParser *scene;
RayTracer *raytracer;
Camera *camera;
GLCanvas canvas;
int width = 100, height = 100, phiStep = 0, thetaStep = 0, maxBounces = 0;
float depth_min = 0, depth_max = 1, cutoffWeight = 0;
bool shade_back = false, gui = false, gouraud = false, shadows = false, transparent_shadow = false;
char *input_file = nullptr, *output_file = nullptr, *depth_file = nullptr, *normal_file = nullptr;

void parseCmd(int argc, char** argv);
float clamp(float value, float low, float high);
Vec3f absolute(const Vec3f& vec);
void render();
void trace(float x, float y);

int main(int argc, char** argv)
{
	// Parse Command Line
	parseCmd(argc, argv);
	// Parse Scene
	scene = new SceneParser(input_file);
	raytracer = new RayTracer(scene, maxBounces, cutoffWeight, shadows, transparent_shadow, shade_back);
	camera = scene->getCamera();
	if (gui) // GUI Previsualiztion
	{
		canvas.initialize(scene, width, height, "Raytracer Previewer", render, trace);
	}
	else // Render to image
	{
		render();
	}
	delete raytracer;
	delete scene;
	return 0;
}

void trace(float x, float y)
{
	Ray ray = camera->generateRay(Vec2f(x * camera->getSize(), y * camera->getSize()));
	Hit hit(camera->getTMin(), nullptr, Vec3f(0, 0, 0));
	Vec3f color = raytracer->traceRay(ray, camera->getTMin(), 0, 1, hit);
	RayTree::SetMainSegment(ray, 0, hit.getT());
	RayTree::Print();
}

void render()
{
	cout << "[INFO] Rendering Scene..." << endl;
	assert(scene);

	// Initialization
	Image renderImg(width, height), depthImg(width, height), normalImg(width, height);
	renderImg.SetAllPixels(scene->getBackgroundColor());
	depthImg.SetAllPixels(Vec3f(0, 0, 0));
	normalImg.SetAllPixels(Vec3f(0, 0, 0));
	float halfWidth = width / 2.f, halfHeight = height / 2.f;
	Vec3f color, zero(0, 0, 0);
	// Scan Loop
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			Ray ray = camera->generateRay(Vec2f((i - halfWidth) * camera->getSize() / width, (j - halfHeight) * camera->getSize() / height));
			Hit hit(camera->getTMin(), nullptr, Vec3f(0, 0, 0));
			color = raytracer->traceRay(ray, camera->getTMin(), 0, 1, hit);
			//cout << "COLOR: " << i << ", " << j << ", " << color << endl;
			if (hit.getT() > camera->getTMin())
			{
				// render mode
				renderImg.SetPixel(i, j, color);
				// depth mode
				if (depth_file)
				{
					float depth = 1 - clamp(hit.getT(), depth_min, depth_max);
					depthImg.SetPixel(i, j, Vec3f(depth, depth, depth));
				}
				// normal mode
				if (normal_file)
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
	cout << "[INFO] Done." << endl;
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
			shade_back = true;
		}
		else if (!strcmp(argv[i], "-tessellation"))
		{
			i++; assert(i < argc);
			thetaStep = atoi(argv[i]);
			i++; assert(i < argc);
			phiStep = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-gouraud"))
		{
			gouraud = true;
		}
		else if (!strcmp(argv[i], "-gui"))
		{
			gui = true;
		}
		else if (!strcmp(argv[i], "-shadows"))
		{
			shadows = true;
		}
		else if (!strcmp(argv[i], "-transparent_shadow"))
		{
			transparent_shadow = true;
		}
		else if (!strcmp(argv[i], "-bounces"))
		{
			i++; assert(i < argc);
			maxBounces = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-weight"))
		{
			i++; assert(i < argc);
			cutoffWeight = atof(argv[i]);
		}
		else
		{
			printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
			assert(0);
		}
	}
}

Vec3f absolute(const Vec3f& vec)
{
	return Vec3f(fabsf(vec[0]), fabsf(vec[1]), fabsf(vec[2]));
}

float clamp(float value, float low, float high)
{
	if (value < low)
	{
		return low;
	}
	else if (value > high)
	{
		return high;
	}
	else
	{
		return (value - low) / (high - low);
	}
}