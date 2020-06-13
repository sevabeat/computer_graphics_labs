#ifndef PROG_PIXELMATRIX_H
#define PROG_PIXELMATRIX_H

#include <fstream>
#include <iostream>
#include <cmath>
#include "exception/ImageException.h"
#include "geometry/Rectangle.h"

#define MATRIX_ROTATE_LEFT 0
#define MATRIX_ROTATE_RIGHT 1

#define sRGB_GAMMA 2.4
#define sRGB_PHI 12.92
#define sRGB_K0 0.04045
#define sRGB_BETA (sRGB_K0 / sRGB_PHI)

typedef unsigned char PIXEL;
typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long long ull;
typedef std::vector<uchar> PIXEL_COLOR;


class PixelMatrix{
private:
    PIXEL* matrix;

    uint pixelBytes;
    uint width;
    uint height;
    uchar depth;

    ull _size;

    ull getPosition(uint h, uint w, int lineWidth = -1);
public:
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

    void drawPoint(Point p, PIXEL_COLOR color, double opacity, double gamma = -1);

    friend std::ifstream& operator>>(std::ifstream &in, PixelMatrix* pixelMatrix);
    friend std::ofstream& operator<<(std::ofstream &out, PixelMatrix* pixelMatrix);

    double static gamma(double value, double gamma);
    double static gamma_back(double value, double gamma);

};

#endif
