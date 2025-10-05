#include "ball.h"

Ball::Ball (int len)
{
    len = 1.0;
    velocity = Vector(1.0, 1.0, 0);
}

void Ball::moveBall(int xR, int yR, int xL, int yL)
{
    // isReset = false;
    handleCollision(xR, yR, xL, yL);
    // Vector v = velocity.normalize() * speed;
    // velocity.y -= g * t;
    // v.y -= g * t;
    // Vector gravity(0.0f, -g, 0.0f);
    // center = center + v * t + gravity * (0.5 * t * t);
    // velocity = velocity + gravity * t;
    // velocity.y -= g * t;
    // center = center + velocity * t;
    // center += velocity.normalize() * speed;
    // for (Point &p : points)
    // {
    //     p = p + velocity * t;
    // }
    // speed = velocity.norm();
    // if (velocity.norm() < 0.1f)
    // {
    //     isMoving = false;
    //     velocity = Vector(0.0f, 0.0f, 0.0f);
    //     speed = 0.0f;
    // }
}

void Ball::handleCollision2(int xR, int yR, int xL, int yL)
{
    // Vector normal = plane.normal.normalize();
    // Plane plane = new Plane(Point(xR, yR, 0.0));
    // Vector newVelocity = velocity.reflect(plane);
    // if (abs(velocity.dot(plane.normal)) > 0.000001f)
    //     velocity = newVelocity * 0.8f;
    // speed = velocity.norm();
}

void Ball::handleCollision(int xR, int yR, int xL, int yL)
{
    // for (int j = 0; j < 6; j++)
    // {
    //     int i = (j + 2) % 6;
    //     if (checkCollision(cube.planes[i]))
    //     {
    //         // cube.planes[i].print();
    //         // printf("Collision with plane %d\n", i);
    //         handleCollision(cube.planes[i]);
    //         if (i == 2 && velocity.y < 0.000001f)
    //         {
    //             isMoving = false;
    //             velocity = Vector(0.0f, 0.0f, 0.0f);
    //             speed = 0.0f;
    //             onGround = true;
    //             break;
    //         }
    //     }
    // }
}

bool Ball::checkCollision(const Plane &plane)
{
    // float dis = plane.distanceToPoint(center);
    // return dis <= radius;
}

