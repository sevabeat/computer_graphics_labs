#include "Rectangle.h"

Rectangle::Rectangle(Point UL, Point UR, Point DR, Point DL): vertices({UL, UR, DR, DL}){
    this->edges.emplace_back(UL, UR);
    this->edges.emplace_back(UR, DR);
    this->edges.emplace_back(DR, DL);
    this->edges.emplace_back(DL, UL);
}

bool Rectangle::isPointInArea(Point p) {
    double d = (this->vertices[2].x - this->vertices[3].x)
                   * (this->vertices[0].y - this->vertices[3].y)
                   - (this->vertices[0].x - this->vertices[3].x)
                     * (this->vertices[2].y - this->vertices[3].y);

    double new_x = ((this->vertices[0].y - this->vertices[3].y)
                   * (p.x - this->vertices[3].x)
                   - (this->vertices[0].x - this->vertices[3].x)
                     * (p.y - this->vertices[3].y)) / d;

    double new_y = ((this->vertices[2].x - this->vertices[3].x)
                   * (p.y - this->vertices[3].y)
                   - (this->vertices[2].y - this->vertices[3].y)
                     * (p.x - this->vertices[3].x)) / d;
    return new_x >= 0 && new_y >= 0 && new_x <= 1 && new_y <= 1;
}


double Rectangle::commonArea(Point p){
    if(p.x < 0 || p.y < 0) return 0.0;
    //если 4 вершины пикселя входят в прямоугльник то очев пиксель полностью входит
    bool isAllPeaksInArea = true;
    for(int i = 0; i <= 1; i++){
        for(int j = 0; j <= 1; j++){
            isAllPeaksInArea = isAllPeaksInArea && this->isPointInArea(p + Point(i, j));
        }
    }
    if(isAllPeaksInArea) return 1.0;

    double area = 0.0;

    //делим пиксель на 100 частей и смотрим сколько из них вошло в прямоугольник

    for(double dx = 0.05; dx <= 0.95; dx += 0.1){
        for(double dy = 0.05; dy <= 0.95; dy += 0.1){
            if(this->isPointInArea(p + Point(dx, dy))){
                area += 0.01;
            }
        }
    }

    return area;
}


//Изначальная идея была считать расстояние между центром пикселя и гранью прямоуогльника
double Rectangle::distanceToPoint(Point p) {
    double dist = 1.0;

    for(int i = 0; i < this->edges.size(); i++) {
        double r1 = p.distance(this->edges[i].first);
        double r2 = p.distance(this->edges[i].second);
        double r3 = this->edges[i].first.distance(this->edges[i].second);
        //если перпендикуляр за границами отрезка
        if(r1 >= Point(r2, r3).distance({0, 0})){
            dist = std::min(dist, r1);
            continue;
        }else if(r2 >= Point(r1, r3).distance({0, 0})){
            dist = std::min(dist, r2);
            continue;
        }

        double A = this->edges[i].first.y - this->edges[i].second.y;
        double B = this->edges[i].second.x - this->edges[i].first.x;
        double C = this->edges[i].first.x * this->edges[i].second.y
                   - this->edges[i].second.x * this->edges[i].first.y;
        double new_dist = abs(A * p.x + B * p.y + C) / sqrt(A * A + B * B);

        dist = std::min(dist, new_dist);
    }
    return dist;
}

double Rectangle::getMinX(){
    return std::min(std::min(this->vertices[0].x, this->vertices[1].x), std::min(this->vertices[2].x, this->vertices[3].x));
}
double Rectangle::getMinY(){
    return std::min(std::min(this->vertices[0].y, this->vertices[1].y), std::min(this->vertices[2].y, this->vertices[3].y));
}
double Rectangle::getMaxX(){
    return std::max(std::max(this->vertices[0].x, this->vertices[1].x), std::max(this->vertices[2].x, this->vertices[3].x));
}
double Rectangle::getMaxY(){
    return std::max(std::max(this->vertices[0].y, this->vertices[1].y), std::max(this->vertices[2].y, this->vertices[3].y));
}