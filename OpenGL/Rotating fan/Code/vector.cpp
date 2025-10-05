#include "vector.h"
#include <iostream>

Vector::Vector() : x(0), y(0), z(0) {}
Vector::Vector(double x, double y, double z) : x(x), y(y), z(z) {}
Vector::Vector(const Vector &v) : x(v.x), y(v.y), z(v.z) {}

Vector Vector::operator=(const Vector &v)
{
    if (this != &v)
    {
        x = v.x;
        y = v.y;
        z = v.z;
    }
    return *this;
}

Vector Vector::operator+(const Vector &v) const
{
    return Vector(x + v.x, y + v.y, z + v.z);
}

Vector Vector::operator-(const Vector &v) const
{
    return Vector(x - v.x, y - v.y, z - v.z);
}

Vector Vector::operator*(const double &d) const
{
    return Vector(x * d, y * d, z * d);
}

Vector Vector::operator/(const double &d) const
{
    if (d == 0)
        throw std::invalid_argument("Division by zero is not allowed.");
    return Vector(x / d, y / d, z / d);
}

Vector Vector::operator+=(const Vector &v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

Vector Vector::operator+=(const double &d)
{
    x += d;
    y += d;
    z += d;
    return *this;
}

Vector Vector::operator-=(const double &d)
{
    x -= d;
    y -= d;
    z -= d;
    return *this;
}

Vector Vector::operator-=(const Vector &v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

Vector Vector::operator*=(const double &d)
{
    x *= d;
    y *= d;
    z *= d;
    return *this;
}

Vector Vector::operator/=(const double &d)
{
    if (d == 0)
        throw std::invalid_argument("Division by zero is not allowed.");
    x /= d;
    y /= d;
    z /= d;
    return *this;
}

double Vector::dot(const Vector &v) const
{
    return x * v.x + y * v.y + z * v.z;
}

Vector Vector::cross(const Vector &v) const
{
    return Vector(
        y * v.z - z * v.y,
        z * v.x - x * v.z,
        x * v.y - y * v.x);
}

Vector Vector::normalize() const
{
    double len = norm();
    if (len == 0)
        return Vector(0, 0, 0);
        // throw std::invalid_argument("Cannot normalize a zero vector.");
    return Vector(x / len, y / len, z / len);
}

Vector Vector::rotate(const Vector &axis, double angle) const
{
    double cosTheta = cos(angle);
    double sinTheta = sin(angle);
    Vector normalizedAxis = axis.normalize();
    return *this * cosTheta +
           (normalizedAxis.cross(*this)) * sinTheta +
           normalizedAxis * (normalizedAxis.dot(*this)) * (1 - cosTheta);
}

Vector Vector::rotateAroundAxis(const Vector &axis, double angle)
{
    double cosTheta = cos(angle);
    double sinTheta = sin(angle);
    Vector normalizedAxis = axis.normalize();
    *this = *this * cosTheta +
            (normalizedAxis.cross(*this)) * sinTheta +
            normalizedAxis * (normalizedAxis.dot(*this)) * (1 - cosTheta);
    return *this;
}

double Vector::norm() const
{
    return sqrt(x * x + y * y + z * z);
}


void Vector::print() const
{
    printf("Vector: (%.2f, %.2f, %.2f)\n", x, y, z);
}