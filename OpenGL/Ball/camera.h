#pragma once

#include "point.h"
#include "vector.h"

class Camera
{
public:
    Point eye;    // Camera position
    Point center; // Look-at point
    Vector up;    // Up vector

    double moveSpeed;
    double rotateSpeed;

    Camera();
    Camera(Point eye, Point center, Vector up, double moveSpeed, double rotateSpeed);
    Camera(const Camera &c);

    void setMoveSpeed(double speed);
    void setRotateSpeed(double speed);

    void move(const Vector &v);
    void rotate(const Vector &axis, double angle);
    void setView(Point eye, Point center, Vector up);

    void moveForward();
    void moveBackward();
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();

    void lookLeft();
    void lookRight();
    void lookUp();
    void lookDown();

    void tiltClockwise();
    void tiltAntiClockwise();

    void moveUp_wo_refPoint();
    void moveDown_wo_refPoint();
};
