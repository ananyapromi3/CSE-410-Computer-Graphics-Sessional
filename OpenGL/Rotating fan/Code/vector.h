#pragma once

#include <stdexcept>
#include <cmath>

class Vector
{
public:
    double x, y, z;

    Vector();
    Vector(double x, double y, double z);
    Vector(const Vector &v);

    Vector operator=(const Vector &v);
    Vector operator+(const Vector &v) const;
    Vector operator-(const Vector &v) const;
    Vector operator*(const double &d) const;
    Vector operator/(const double &d) const;

    Vector operator+=(const Vector &v);
    Vector operator+=(const double &d);
    Vector operator-=(const double &d);
    Vector operator-=(const Vector &v);
    Vector operator*=(const double &d);
    Vector operator/=(const double &d);

    double dot(const Vector &v) const;
    Vector cross(const Vector &v) const;
    Vector normalize() const;
    Vector rotate(const Vector &axis, double angle) const;
    Vector rotateAroundAxis(const Vector &axis, double angle);
    double norm() const;

    void print() const;
};
