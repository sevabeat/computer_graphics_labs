#ifndef PROG_RECTANGLE_H
#define PROG_RECTANGLE_H

#include <iostream>
#include <vector>
#include <utility>
#include <cmath>
#include "Point.h"

/*
Vertices and Edges indexes:
      0
  0 ----- 1
  |       |
3 |       | 1
  |       |
  3 ----- 2
      2
*/

class Rectangle{
private:
    std::vector<Point> vertices;
    std::vector< std::pair<Point, Point> > edges;
public:
    Rectangle(Point UL, Point UR, Point DR, Point DL);
    bool isPointInArea(Point p);
    double distanceToPoint(Point p);

    double commonArea(Point p);

    double getMinX();

    double getMinY();

    double getMaxX();

    double getMaxY();
};

#endif //PROG_RECTANGLE_H
