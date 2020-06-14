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
    void showMetaData();
    void drawLine(Point from, Point to, double thickness, PIXEL_COLOR color, double gamma = -1);
};



#endif //PROG_PNM_H
