#pragma once
#include <iostream>
#include "point.h"
#include "vector.h"
#include "plane.h"

class Cube
{
public:
    Point center;                // Center of the cube
    double width, height, depth; // Dimensions of the cube
    Vector axis[3];              // Axes of the cube
    Plane planes[6];             // Planes of the cube

    Cube(Point center, double width, double height, double depth);

    void setAxis(const Vector *axes);
};
