#pragma once

#include "vector.h"
#include "point.h"

class Plane
{
public:
    Vector normal; // Normal vector of the plane
    Point point;   // A point on the plane

    Plane();
    Plane(const Vector &normal, const Point &point);
    Plane(Point p1, Point p2, Point p3);
    float distanceToPoint(const Point &p) const;
    void print() const;
    double findZ(double x, double y) const;
};
