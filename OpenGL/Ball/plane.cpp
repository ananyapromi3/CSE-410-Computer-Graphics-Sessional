#include "plane.h"
#include <iostream>

Plane::Plane() = default;

Plane::Plane(const Vector &normal, const Point &point)
    : normal(normal), point(point) {}

float Plane::distanceToPoint(const Point &p) const
{
    return normal.dot(p - point) / normal.norm();
}

void Plane::print() const
{
    printf("Plane normal: (%.2f, %.2f, %.2f), point: (%.2f, %.2f, %.2f)\n",
           normal.x, normal.y, normal.z,
           point.x, point.y, point.z);
}
