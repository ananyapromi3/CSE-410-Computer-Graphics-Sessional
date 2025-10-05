#include "ball.h"
#include <iostream>

Ball::Ball(float radius, Point center, Vector up, int sector_count, int stack_count)
    : radius(radius), center(center), up(up), sector_count(sector_count), stack_count(stack_count)
{
    showVelocityArrow = false;
    isMoving = false;
    isReset = true;
    float vx = (static_cast<float>(rand()) / RAND_MAX) * 2.0f - 1.0f;
    float vy = (static_cast<float>(rand()) / RAND_MAX) * 2.0f - 1.0f;
    float vz = (static_cast<float>(rand()) / RAND_MAX) * 2.0f - 1.0f;
    velocity = Vector(vx, vy, vz);
    velocity = Vector(velocity.x, velocity.y, velocity.z);
    generatePoints();
}

Ball::Ball(float radius, int sector_count, int stack_count, Cube &cube)
{
    this->radius = radius;
    this->sector_count = sector_count;
    this->stack_count = stack_count;
    reset(cube);
    generatePoints();
}

bool Ball::checkCollision(const Plane &plane)
{
    float dis = plane.distanceToPoint(center);
    return dis <= radius;
}

void Ball::reset(Cube &cube)
{
    float x = cube.center.x + ((rand() % 100) / 100.0f - 0.5f) * (cube.width - 2 * radius);
    float y = cube.center.y + ((rand() % 100) / 100.0f - 0.5f) * (cube.height - 2 * radius);
    float z = cube.center.z + ((rand() % 100) / 100.0f - 0.5f) * (cube.depth - 2 * radius);
    center = Point(x, y, z);
    float vx = (static_cast<float>(rand()) / RAND_MAX) * 2.0f - 1.0f;
    float vy = (static_cast<float>(rand()) / RAND_MAX) * 2.0f - 1.0f;
    float vz = (static_cast<float>(rand()) / RAND_MAX) * 2.0f - 1.0f;
    velocity = Vector(vx, vy, vz);
    velocity = Vector(velocity.x, velocity.y, velocity.z);
    up = Vector(0.0f, 1.0f, 0.0f);
    speed = 20.0f + (static_cast<float>(rand()) / RAND_MAX) * 20.0f;
    velocity = velocity.normalize() * speed;
    isReset = true;
    isMoving = false;
    onGround = false;
    generatePoints();
    printf("Ball speed: %f\n", speed);
}

void Ball::generatePoints()
{
    points.clear();
    for (int i = 0; i <= stack_count; ++i)
    {
        float stackAngle = M_PI / 2 - i * M_PI / stack_count;
        float xy = radius * cosf(stackAngle);
        float z = radius * sinf(stackAngle);
        for (int j = 0; j <= sector_count; ++j)
        {
            float sectorAngle = j * 2 * M_PI / sector_count;
            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);
            points.push_back(Point(
                center.x + x,
                center.y + y,
                center.z + z));
        }
    }
}

void Ball::rotateBall()
{
    float angle = speed * t / radius;
    for (Point &p : points)
    {
        // Vector dir = p - center;
        Vector dir = p.toVector() - center.toVector();
        Vector axis = up.cross(velocity);
        if (axis.norm())
        {
            dir.rotateAroundAxis(axis, angle);
            p = center + dir;
        }
        else
        {
            printf("No rotation axis found\n");
        }
        // printf("Rotated Point: (%f, %f, %f)\n", p.x, p.y, p.z);
    }
}

void Ball::moveBall(const Cube &cube)
{
    isReset = false;
    handleCollision(cube);
    // Vector v = velocity.normalize() * speed;
    // velocity.y -= g * t;
    // v.y -= g * t;
    // Vector gravity(0.0f, -g, 0.0f);
    // center = center + v * t + gravity * (0.5 * t * t);
    // velocity = velocity + gravity * t;
    velocity.y -= g * t;
    center = center + velocity * t;
    // center += velocity.normalize() * speed;
    for (Point &p : points)
    {
        p = p + velocity * t;
    }
    speed = velocity.norm();
    // if (velocity.norm() < 0.1f)
    // {
    //     isMoving = false;
    //     velocity = Vector(0.0f, 0.0f, 0.0f);
    //     speed = 0.0f;
    // }
}

void Ball::handleCollision(const Cube &cube)
{
    for (int j = 0; j < 6; j++)
    {
        int i = (j + 2) % 6;
        if (checkCollision(cube.planes[i]))
        {
            // cube.planes[i].print();
            // printf("Collision with plane %d\n", i);
            handleCollision(cube.planes[i]);
            if (i == 2 && velocity.y < 0.000001f)
            {
                isMoving = false;
                velocity = Vector(0.0f, 0.0f, 0.0f);
                speed = 0.0f;
                onGround = true;
                break;
            }
        }
    }
}

void Ball::handleCollision(const Plane &plane)
{
    // Vector normal = plane.normal.normalize();
    Vector newVelocity = velocity.reflect(plane);
    if (abs(velocity.dot(plane.normal)) > 0.000001f)
        velocity = newVelocity * 0.8f;
    speed = velocity.norm();
    // printf("Ball speed after collision: %f\n", speed);
    // center = center + normal * (radius - plane.distanceToPoint(center));
    // for (Point &p : points)
    // {
    //     Vector dir = p.toVector() - center.toVector();
    //     dir = dir.reflect(plane).normalize();
    //     p = center + dir * radius;
    // }
    // generatePoints();
    // velocity.y -= g * t;
    // center = center + velocity * t;
    // // center += velocity.normalize() * speed;
    // for (Point &p : points)
    // {
    //     p = p + velocity * t;
    // }
    // speed = velocity.norm();
}

void Ball::increaseSpeed(float increment)
{
    speed += increment;
    velocity = velocity.normalize() * speed;
    printf("Ball speed increased to: %f\n", speed);
}

void Ball::decreaseSpeed(float decrement)
{
    speed -= decrement;
    if (speed < 0.0f)
        speed = 0.0f;
    velocity = velocity.normalize() * speed;
    printf("Ball speed decreased to: %f\n", speed);
    // if (abs(velocity.y) < 0.01f)
    // {
    //     speed = 0.0f;
    //     velocity = Vector(0.0f, 0.0f, 0.0f);
    //     isMoving = false;
    // }
}