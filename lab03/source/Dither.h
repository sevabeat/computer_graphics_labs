
#ifndef PROG_DITHER_H
#define PROG_DITHER_H

#include <vector>
#include <iostream>
#include <cmath>
#include "PixelMatrix.h"

#define D(x) ((double)x / 64.0) - 0.5
#define H(x) ((double)x / 17.0) - 0.5
#define MLine(x1, x2, x3, x4, x5, x6, x7, x8) {D(x1), D(x2), D(x3), D(x4), D(x5), D(x6), D(x7), D(x8)}
#define HLine(x1, x2, x3, x4) {H(x1), H(x2), H(x3), H(x4)}

typedef std::vector< std::vector<double> > MATRIX;
typedef unsigned char uchar;

class Dither{
private:
    int bits;
    int depth;
    double resizer;
    MATRIX orderedMatrix8x8;
    MATRIX halftoneMatrix4x4;
    PixelMatrix* matrix = nullptr;

    uchar nearestColor(uchar color);
    uchar colorSum(uchar color, double value);
public:
    Dither(int bits, PixelMatrix* matrix);
    void Ordered();
    void Random();
    void FloydSteinberg();
    void JJN();
    void Sierra3();
    void Atkinson();
    void Halftone();
};




#endif
