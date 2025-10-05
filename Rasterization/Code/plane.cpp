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

Plane::Plane(Point p1, Point p2, Point p3)
{
    Vector v1 = p2 - p1;
    Vector v2 = p3 - p1;
    normal = v1.cross(v2);
    point = p1;
}

double Plane::findZ(double x, double y) const
{
    Vector n = normal.normalize();
    double A = n.x;
    double B = n.y;
    double C = n.z;
    if (fabs(C) == 0.0) 
    {
        return 1e8;
    }
    double D = -(A * point.x + B * point.y + C * point.z);
    return -(A * x + B * y + D) / C;
}
