#include <iostream>
#include "cube.h"

Cube::Cube(Point center, double width, double height, double depth)
    : center(center), width(width), height(height), depth(depth)
{
    axis[0] = Vector(1, 0, 0);
    axis[1] = Vector(0, 1, 0);
    axis[2] = Vector(0, 0, 1);

    // X-axis planes (left and right)
    Vector normalX = axis[0];
    planes[0] = Plane(normalX, center - normalX * (width / 2.0));        // Left
    planes[1] = Plane(normalX * (-1), center + normalX * (width / 2.0)); // Right

    // Y-axis planes (bottom and top)
    Vector normalY = axis[1];
    planes[2] = Plane(normalY, center - normalY * (height / 2.0));        // Bottom
    planes[3] = Plane(normalY * (-1), center + normalY * (height / 2.0)); // Top

    // Z-axis planes (front and back)
    Vector normalZ = axis[2];
    planes[4] = Plane(normalZ, center - normalZ * (depth / 2.0));        // Front
    planes[5] = Plane(normalZ * (-1), center + normalZ * (depth / 2.0)); // Back
}

void Cube::setAxis(const Vector *axes)
{
    for (int i = 0; i < 3; ++i)
    {
        axis[i] = axes[i].normalize();
    }
}

// Plane *Cube::getPlanes() const
// {
//     Plane *planes = new Plane[3];
//     for (int i = 0; i < 3; ++i)
//     {
//         planes[i].normal = axis[i];
//         planes[i].point = center + axis[i].normalize() * (width / 2);
//     }
//     return planes;
// }
