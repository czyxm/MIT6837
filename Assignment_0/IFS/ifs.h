#ifndef _IFS_H_
#define _IFS_H_

#include "matrix.h"
#include "image.h"
#include <vector>
using namespace std;

class Trans
{
public:
	Matrix matrix;
	float probability;

	Trans() {}
};

class IFS {
private:
	vector<Trans> transformations;
	static vector<Vec3f> colors;

public:
	IFS() {}

	void input(const char *input_file);
	int nextK(const vector<float>& cumP);
	void render(Image &img, const int &pointNum, const int &iterNum);

	~IFS() {}
};

#endif