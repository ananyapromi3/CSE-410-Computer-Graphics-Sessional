#include "camera.h"
#include <iostream>

Camera::Camera() : eye(1, 1, 1), center(0, 0, 0), up(0, 1, 0), moveSpeed(0.5), rotateSpeed(0.05) {}

Camera::Camera(Point eye, Point center, Vector up, double moveSpeed, double rotateSpeed)
    : eye(eye), center(center), up(up), moveSpeed(moveSpeed), rotateSpeed(rotateSpeed) {}

Camera::Camera(const Camera &c)
    : eye(c.eye), center(c.center), up(c.up), moveSpeed(c.moveSpeed), rotateSpeed(c.rotateSpeed) {}

void Camera::setMoveSpeed(double speed)
{
    moveSpeed = speed;
}

void Camera::setRotateSpeed(double speed)
{
    rotateSpeed = speed;
}

void Camera::move(const Vector &v)
{
    eye = eye + v;
    center = center + v;
}

void Camera::rotate(const Vector &axis, double angle)
{
    Vector direction = center - eye;
    direction = direction.rotate(axis, angle);
    center = eye + direction;
}

void Camera::setView(Point eye, Point center, Vector up)
{
    this->eye = eye;
    this->center = center;
    this->up = up;
}

void Camera::moveForward()
{
    eye += (center - eye).normalize() * moveSpeed;
}

void Camera::moveBackward()
{
    eye -= (center - eye).normalize() * moveSpeed;
}

void Camera::moveLeft()
{
    Vector direction = (center - eye).normalize();
    Vector left = up.cross(direction).normalize();
    eye += left * moveSpeed;
    center += left * moveSpeed;
}

void Camera::moveRight()
{
    Vector direction = (center - eye).normalize();
    Vector right = direction.cross(up).normalize();
    eye += right * moveSpeed;
    center += right * moveSpeed;
}

void Camera::moveUp()
{
    Vector upVector = up.normalize();
    eye += upVector * moveSpeed;
    center += upVector * moveSpeed;
}

void Camera::moveDown()
{
    Vector downVector = up.normalize() * (-1);
    eye += downVector * moveSpeed;
    center += downVector * moveSpeed;
}

void Camera::lookLeft()
{
    Vector direction = center - eye;
    direction = direction.rotate(up, rotateSpeed);
    center = eye + direction;
}

void Camera::lookRight()
{
    Vector direction = center - eye;
    direction = direction.rotate(up, -rotateSpeed);
    center = eye + direction;
}

void Camera::lookUp()
{
    Vector direction = center - eye;
    Vector right = up.cross(direction);
    direction = direction.rotate(right, -rotateSpeed);
    center = eye + direction;
    up = up.rotate(right, rotateSpeed);
}

void Camera::lookDown()
{
    Vector direction = center - eye;
    Vector right = up.cross(direction);
    direction = direction.rotate(right, rotateSpeed);
    center = eye + direction;
    up = up.rotate(right, -rotateSpeed);
}

void Camera::tiltClockwise()
{
    Vector direction = center - eye;
    up = up.rotate(direction, rotateSpeed);
}

void Camera::tiltAntiClockwise()
{
    Vector direction = center - eye;
    up = up.rotate(direction, -rotateSpeed);
}

void Camera::moveUp_wo_refPoint()
{
    Vector direction = up.normalize();
    eye += direction * moveSpeed;
}

void Camera::moveDown_wo_refPoint()
{
    Vector direction = up.normalize();
    eye -= direction * moveSpeed;
}
