#include "triangle.h"

Triangle::~Triangle()
{
}

void Triangle::setColor(const Color &color)
{
    this->color = color;
}

Color Triangle::getColor() const
{
    return color;
}

void Triangle::setP1(const Point &p)
{
    p1 = p;
}

void Triangle::setP2(const Point &p)
{
    p2 = p;
}

void Triangle::setP3(const Point &p)
{
    p3 = p;
}

Point Triangle::getP1() const
{
    return p1;
}

Point Triangle::getP2() const
{
    return p2;
}

Point Triangle::getP3() const
{
    return p3;
}

// bool Triangle::insideTriangle(const Point &p) const
// {
//     Vector v0 = p2 - p1;
//     Vector v1 = p3 - p1;
//     Vector v2 = p - p1;
//     double dot00 = v0.dot(v0);
//     double dot01 = v0.dot(v1);
//     double dot02 = v0.dot(v2);
//     double dot11 = v1.dot(v1);
//     double dot12 = v1.dot(v2);
//     double d = dot00 * dot11 - dot01 * dot01;
//     if (fabs(d) < 1e-8)
//     {
//         return false;
//     }
//     double invD = 1.0 / d;
//     double u = (dot11 * dot02 - dot01 * dot12) * invD;
//     double v = (dot00 * dot12 - dot01 * dot02) * invD;
//     return (u >= 0) && (v >= 0) && (u + v <= 1);
// }

double area(const Point &a, const Point &b, const Point &c)
{
    return 0.5 * fabs((a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y)));
}

bool Triangle::insideTriangle(const Point &p) const
{
    double A = fabs(area(p1, p2, p3));
    double A1 = fabs(area(p, p2, p3));
    double A2 = fabs(area(p1, p, p3));
    double A3 = fabs(area(p1, p2, p));
    return fabs((A1 + A2 + A3) - A) < 1e-8;
}

// bool Triangle::insideTriangle(const Point &p) const
// {
//     auto sign = [](const Point &a, const Point &b, const Point &c)
//     {
//         return (a.x - c.x) * (b.y - c.y) - (b.x - c.x) * (a.y - c.y);
//     };
//     bool b1 = sign(p, p1, p2) < 0.0;
//     bool b2 = sign(p, p2, p3) < 0.0;
//     bool b3 = sign(p, p3, p1) < 0.0;
//     return (b1 == b2) && (b2 == b3);
// }

void Triangle::reorderVerticesByY()
{
    Point pts[3] = {p1, p2, p3};
    std::sort(pts, pts + 3, [](const Point &a, const Point &b)
              {
        if (a.y != b.y)
            return a.y > b.y;
        return a.x < b.x; });
    p1 = pts[0];
    p2 = pts[1];
    p3 = pts[2];
}
