#pragma once

#include <cmath>
#include "vector.h"
#include "matrix.h"

class Vector; 

class Point
{
public:
    double x, y, z;

    Point();
    Point(double x, double y, double z);
    Point(const Point &p);

    Vector operator-(const Point &p) const;
    Point operator+(const Vector &v) const;
    Point operator-(const Vector &v) const;
    Point operator+=(const Vector &v);
    Point operator-=(const Vector &v);
    double distance(const Point &p) const;

    Vector toVector() const;

    void print() const;

    Matrix toMatrix() const;
    Point transform(const Matrix &m) const;
};
