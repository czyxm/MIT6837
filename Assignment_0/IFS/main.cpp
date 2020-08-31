#include "ifs.h"
#include <string>
using namespace std;

int main(int argc, char *argv[])
{
    // ========================================================
    // ========================================================
    // Some sample code you might like to use for parsing 
    // command line arguments and the input IFS files

    // sample command line:
    // ifs -input fern.txt -points 10000 -iters 10 -size 100 -output fern.tga

    char* input_file = NULL;
    int num_points = 10000;
    int num_iters = 10;
    int size = 100;
    char* output_file = NULL;

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-input")) {
            i++; assert(i < argc);
            input_file = argv[i];
        }
        else if (!strcmp(argv[i], "-points")) {
            i++; assert(i < argc);
            num_points = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-iters")) {
            i++; assert(i < argc);
            num_iters = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-size")) {
            i++; assert(i < argc);
            size = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-output")) {
            i++; assert(i < argc);
            output_file = argv[i];
        }
        else {
            printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
            assert(0);
        }
    }

    // IFS
    IFS ifs;
    Image img(size, size);
    ifs.input(input_file);
    for (int i = 1; i <= num_iters; i++)
    {
        string name = string(output_file);
        name.insert(name.length() - 4, "_" + to_string(i));
        cout << name << endl;
        ifs.render(img, num_points, i);
        img.SaveTGA(name.c_str());
    }
    cout << "FINISHED!" << endl;
	return 0;
}