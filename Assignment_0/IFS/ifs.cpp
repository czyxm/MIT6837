#define _CRT_SECURE_NO_WARNINGS
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <vector>
#include "ifs.h"

vector<Vec3f> IFS::colors = vector<Vec3f>({
        Vec3f(1, 0, 0),
        Vec3f(0, 1, 0),
        Vec3f(0, 0, 1),
        Vec3f(0, 1, 1),
        Vec3f(1, 0, 1),
        Vec3f(1, 1, 0)
});

void IFS::input(const char *input_file)
{
    // ========================================================
    // ========================================================
    // Some sample code you might like to use for
    // parsing the IFS transformation files

    // open the file
    FILE* input = fopen(input_file, "r");
    assert(input != NULL);

    // read the number of transforms
    int num_transforms;
    fscanf(input, "%d", &num_transforms);

    // < DO SOMETHING WITH num_transforms >

    // read in the transforms
    for (int i = 0; i < num_transforms; i++) {
        Trans trans;
        fscanf(input, "%f", &trans.probability);
        trans.matrix.Read3x3(input);
        this->transformations.push_back(trans);
    }
    // close the file
    fclose(input);

    // ========================================================
    // ========================================================
}

void IFS::render(Image &img, const int &pointNum, const int &iterNum)
{
    int w = img.Width(), h = img.Height();
    int K = this->transformations.size();
    vector<float> cumulativeP(K, 0.f);
    cumulativeP[0] = this->transformations[0].probability;
    for (int k = 1; k < K; k++)
    {
        cumulativeP[k] = cumulativeP[k - 1] + this->transformations[k].probability;
    }
    img.SetAllPixels(Vec3f(1, 1, 1));
    srand(time(0));
    for (int i = 1; i <= pointNum; i++)
    {
        Vec2f position(rand() * 1.f / RAND_MAX, rand() * 1.f / RAND_MAX);
        Vec3f color(1, 1, 1);
        for (int j = 0; j < iterNum; j++)
        {
            int k = nextK(cumulativeP);
            color = 0.5f * color + 0.5f * this->colors[k];
            this->transformations[k].matrix.Transform(position);
        }
        position.Set(fmax(fmin(w - 1, position.x() * w), 0), fmax(fmin(h - 1, position.y() * h), 0));
        int x = (int)(position.x() + 0.5f), y = (int)(position.y() + 0.5f);
        img.SetPixel(x, y, color);
    }
}

int IFS::nextK(const vector<float> &cumP)
{
    float p = rand() * 1.f / RAND_MAX;
    for (int k = 0; k < cumP.size(); k++)
    {
        if (p < cumP[k])
        {
            return k;
        }
    }
    return cumP.size() - 1;
}