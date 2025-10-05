#include "point.h"
#include "vector.h"
#include <iostream>

Point::Point() : x(0), y(0), z(0) {}

Point::Point(double x, double y, double z) : x(x), y(y), z(z) {}

Point::Point(const Point &p) : x(p.x), y(p.y), z(p.z) {}

Vector Point::operator-(const Point &p) const
{
    return Vector(x - p.x, y - p.y, z - p.z);
}

Point Point::operator+(const Vector &v) const
{
    return Point(x + v.x, y + v.y, z + v.z);
}

Point Point::operator-(const Vector &v) const
{
    return Point(x - v.x, y - v.y, z - v.z);
}

Point Point::operator+=(const Vector &v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

Point Point::operator-=(const Vector &v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

double Point::distance(const Point &p) const
{
    return sqrt((x - p.x) * (x - p.x) +
                (y - p.y) * (y - p.y) +
                (z - p.z) * (z - p.z));
}

void Point::print() const
{
    std::cout << "Point(" << x << ", " << y << ", " << z << ")" << std::endl;
}

Vector Point::toVector() const
{
    return Vector(x, y, z);
}