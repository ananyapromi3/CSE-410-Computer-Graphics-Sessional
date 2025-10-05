#pragma once

#include "vector.h"
#include "point.h"

class Plane {
public:
    Vector normal; // Normal vector of the plane
    Point point;   // A point on the plane

    Plane();
    Plane(const Vector &normal, const Point &point);
    float distanceToPoint(const Point &p) const;
    void print() const;
};
