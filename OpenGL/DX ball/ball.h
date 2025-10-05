#pragma once
#include "point.h"
#include "vector.h"
#include "plane.h"

class Ball
{
public:
    int len;
    Vector velocity;
    Ball (int len);
    void moveBall(int xR, int yR, int xL, int yL);
    void handleCollision(int xR, int yR, int xL, int yL);
    void handleCollision2(int xR, int yR, int xL, int yL);
    bool checkCollision(const Plane &plane);
};