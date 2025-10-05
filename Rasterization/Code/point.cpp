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
    std::cout << x << " " << y << " " << z << std::endl;
}

Vector Point::toVector() const
{
    return Vector(x, y, z);
}

Matrix Point::toMatrix() const
{
    Matrix m(4, 1);
    m.setValue(0, 0, x);
    m.setValue(1, 0, y);
    m.setValue(2, 0, z);
    m.setValue(3, 0, 1.0);
    return m;
}

Point Point::transform(const Matrix &m) const
{
    Matrix result = m * this->toMatrix();
    double x = result.getValue(0, 0);
    double y = result.getValue(1, 0);
    double z = result.getValue(2, 0);
    double w = result.getValue(3, 0);
    if (w != 0)
    {
        x /= w;
        y /= w;
        z /= w;
    }
    return Point(x, y, z);
}