#include "nagordola.h"
Nagordola::Nagordola(float r)
{
    count = 6;
    height = r;
    blocKSize = 0.5f;
    angleR = 0;
    speed = 0.0f;
    isRotating = false;
    for (int i = 0; i < count; i++)
    {
        float angle = i * 60.0f;
        float x = r * cos(angle * M_PI / 180.0f);
        float z = r * sin(angle * M_PI / 180.0f);
        Plane p = Plane(Vector(0, 1, 0), Point(x, -height, z));
        Point pp = Point(x, 0.0f, z);
        planes.push_back(p);
        points.push_back(pp);
    }
}

int Nagordola::getCount()
{
    return count;
}

void Nagordola::rotate()
{
    float angle = speed;
    for (int i = 0; i < count; i++)
    {
        planes[i].normal = planes[i].normal.rotate(Vector(0, 1, 0), angle);
        Vector p = planes[i].point - Point(0, 0, 0);
        p = p.rotate(Vector(0, 1, 0), angle);
        planes[i].point = Point(p.x, p.y, p.z);
        Vector p2 = points[i] - Point(0, 0, 0);
        p2 = p2.rotate(Vector(0, 1, 0), angle);
        points[i] = Point(p2.x, p2.y, p2.z);
    }
}

void Nagordola::spread()
{
    float angle = 10 * M_PI / 180.0f;
    if (angleR < 4)
    {
        angleR++;
        for (int i = 0; i < count; i++)
        {
            Vector p = planes[i].point - Point(0, 0, 0);
            // float c = planes[i].normal.dot(Vector(0.0, 1.0, 0.0));
            // printf("c: %f\n", c);
            Vector r = planes[i].normal.cross(p);
            planes[i].normal = planes[i].normal.rotate(r, -angle);
            p = p.rotate(r, -angle);
            planes[i].point = Point(p.x, p.y, p.z);
            speed += 0.01f;
        }
        // angleR += angle;
        printf("angleR: %f\n", angleR);
    }
}

void Nagordola::shrink()
{
    // float angle = 10.0 * M_PI / 180.0f;
    // for (int i = 0; i < count; i++)
    // {
    //     Vector p = planes[i].point - Point(0, 0, 0);
    //     if (angleR > 0.0f)
    //     {
    //         Vector r = planes[i].normal.cross(p);
    //         planes[i].normal = planes[i].normal.rotate(r, angle);
    //         p = p.rotate(r, angle);
    //         planes[i].point = Point(p.x, p.y, p.z);
    //         angleR -= 1.0f;
    //         speed -= 0.01f;
    //     }
    //     else
    //     {
    //         isRotating = false;
    //     }
    // }
    float angle = 10 * M_PI / 180.0f;
    if (angleR > 0)
    {
        angleR = fmax(0, angleR - 1);
        for (int i = 0; i < count; i++)
        {
            Vector p = planes[i].point - Point(0, 0, 0);
            // float c = planes[i].normal.dot(Vector(0.0, 1.0, 0.0));
            // printf("c: %f\n", c);
            Vector r = planes[i].normal.cross(p);
            planes[i].normal = planes[i].normal.rotate(r, angle);
            p = p.rotate(r, angle);
            planes[i].point = Point(p.x, p.y, p.z);
            speed -= 0.01f;
        }
        // angleR += angle;
        printf("angleR: %f\n", angleR);
    }
}