#include "scene_parser.h"
#include "object3d.h" 
#include "material.h"
#include "camera.h" 
#include "ray.h"
#include "hit.h"
#include "light.h"
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
int width = 100, height = 100, phiStep = 0, thetaStep = 0;
float depth_min = 0, depth_max = 1;
bool shade_back = false, gui = false, gouraud = false;
char *input_file = NULL, *output_file = NULL, *depth_file = NULL, *normal_file = NULL;

void parseCmd(int argc, char** argv);
Vec3f calcColor(const Ray& r, const Hit& h, const SceneParser& scene);
Vec3f absolute(const Vec3f& vec);
void render();

int main(int argc, char** argv)
{
	// Parse Command Line
	parseCmd(argc, argv);
	// Parse Scene
	scene = new SceneParser(input_file);
	GLCanvas canvas;
	if (gui) // GUI Previsualiztion
	{
		canvas.initialize(scene, width, height, "Raytracer Previewer", render);
	}
	else // Render to image
	{
		render();
	}
	delete scene;
	return 0;
}

void render()
{
	cout << "[INFO] Rendering Scene..." << endl;
	assert(scene);

	// Initialization
	Camera* camera = scene->getCamera();
	Group* group = scene->getGroup();
	Image renderImg(width, height), depthImg(width, height), normalImg(width, height);
	renderImg.SetAllPixels(scene->getBackgroundColor());
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
					renderImg.SetPixel(i, j, shade_back ? calcColor(ray, hit, *scene) : Vec3f(0, 0, 0));
				}
				else
				{
					// render mode
					renderImg.SetPixel(i, j, calcColor(ray, hit, *scene));
				}
				// depth mode
				if (depth_file != NULL)
				{
					float depth = 1 - Material::clamp(hit.getT(), depth_min, depth_max);
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
	cout << "[INFO] Done." << endl;
}

Vec3f calcColor(const Ray& r, const Hit& h, const SceneParser& scene)
{
	Vec3f color = scene.getAmbientLight() * h.getMaterial()->getDiffuseColor(), lightDir, lightColor;
	float distanceToLight;
	for (int i = 0; i < scene.getNumLights(); i++)
	{
		scene.getLight(i)->getIllumination(h.getIntersectionPoint(), lightDir, lightColor, distanceToLight);
		color += h.getMaterial()->Shade(r, h, lightDir, lightColor);
	}
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