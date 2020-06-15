#ifndef PROG_PNM_H
#define PROG_PNM_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cmath>
#include "exception/ImageException.h"

#include "PixelMatrix.h"


class PNM{
private:
    std::string type;
    std::vector<std::string> comments;
    PixelMatrix* matrix;
    bool loaded;
public:
    PNM();
    ~PNM();
    void read(std::string filePath);
    void write(std::string filePath);
    void inverseColors();
    void horizontalReflection();
    void verticalReflection();
    void rotateRight90();
    void rotateLeft90();

    void ContrastAdaptiveSharpening(double sharpness);
    static COLOR minPixel(COLOR_LIST list);
    static COLOR maxPixel(COLOR_LIST list);
    static double getIntensiveWithOpacity(double background, double intensive, double opacity);
    static double toColor(double color);
    static double toIntensive(uchar color);
};



#endif //PROG_PNM_H
