
#ifndef PROG_POINT_H
#define PROG_POINT_H

//Называется точка, по сути вектор
class Point{
public:
    double x;
    double y;
    Point(double x, double y): x(x), y(y){};
    void show(){
        std::cout << "(" << this->x << "; " << this->y << ")";
    }
    Point operator*=(double scalar){
        this->x *= scalar;
        this->y *= scalar;
        return *this;
    }
    Point operator*(double scalar){
        Point new_p(this->x, this->y);
        new_p.x *= scalar;
        new_p.y *= scalar;
        return new_p;
    }
    double distance(Point p){
        return sqrt((this->x - p.x) * (this->x - p.x) + (this->y - p.y) * (this->y - p.y));
    }
    double length(){
        return sqrt(this->x * this->x + this->y * this->y);
    }
    Point operator+(Point p){
        return {this->x + p.x, this->y + p.y};
    }
    Point operator-(Point p){
        return {this->x - p.x, this->y - p.y};
    }
    Point operator-=(Point p){
        this->x -= p.x;
        this->y -= p.y;
        return *this;
    }
};

#endif //PROG_POINT_H
