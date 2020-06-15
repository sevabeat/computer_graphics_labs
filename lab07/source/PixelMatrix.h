#ifndef PROG_PIXELMATRIX_H
#define PROG_PIXELMATRIX_H

#include <fstream>
#include <iostream>
#include <vector>
#include "exception/ImageException.h"

#define MATRIX_ROTATE_LEFT 0
#define MATRIX_ROTATE_RIGHT 1


typedef unsigned char PIXEL;
typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long long ull;

struct COLOR{
    double R;
    double G;
    double B;
};


typedef std::vector< COLOR > COLOR_LIST;

class PixelMatrix{
private:

    uint pixelBytes;
    uint width;
    uint height;
    uchar depth;

    ull _size;
public:
    PIXEL* matrix;
    ull getPosition(uint h, uint w, uint lineWidth = -1);
    PixelMatrix(uint width, uint height, uint pixelBytes, uchar depth);
    ~PixelMatrix();
    ull size();

    uint getWidth();
    uint getHeight();
    uint getDepth();

    void inverseColors();
    void horizontalReflection();
    void verticalReflection();
    void rotate(char rotationType);

    friend std::ifstream& operator>>(std::ifstream &in, PixelMatrix* pixelMatrix);
    friend std::ofstream& operator<<(std::ofstream &out, PixelMatrix* pixelMatrix);

//    COLOR getPixel(int h, int w);
//    void setPixel(int h, int w, COLOR color);
};

#endif
