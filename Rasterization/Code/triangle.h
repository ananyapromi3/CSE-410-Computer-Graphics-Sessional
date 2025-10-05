#pragma once
#include "point.h"
#include "vector.h"
#include "color.h"

class Triangle
{
private:
    Point p1, p2, p3;
    Color color;

public:
    Triangle(Point p1, Point p2, Point p3, const Color &color = Color(255, 255, 255)) : p1(p1), p2(p2), p3(p3), color(color) {};
    ~Triangle();
    void setColor(const Color &color);
    Color getColor() const;
    void setP1(const Point &p);
    void setP2(const Point &p);
    void setP3(const Point &p);
    Point getP1() const;
    Point getP2() const;
    Point getP3() const;
    bool insideTriangle(const Point &p) const;
    void reorderVerticesByY();
};
