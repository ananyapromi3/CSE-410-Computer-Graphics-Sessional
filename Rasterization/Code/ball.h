#pragma once
#include "point.h"
#include "vector.h"
#include "plane.h"
#include "cube.h"
#include "globals.h"

#include <vector>

// int animationSpeed = 60; // Timer interval in milliseconds
// float g = 9.8f;          // Gravity constant
// float t = 0.06f;         // Time step for simulation

class Ball
{
public:
    float radius;
    Point center;
    Vector up;        // Direction of the ball
    int sector_count; // Number of sectors for the ball
    int stack_count;  // Number of stacks for the ball
    Vector velocity;
    float rotationSpeed = 10.0f;
    float speed;                   // Speed of the ball
    std::vector<Point> points;     // Points for the ball
    bool showVelocityArrow = true; // Flag to show velocity arrow
    bool isMoving = false;         // Flag to check if the ball is moving
    bool isReset = true;           // Flag to check if the ball is reset
    bool onGround = false;

    Ball(float radius, Point center, Vector up, int sector_count, int stack_count);
    Ball(float radius, int sector_count, int stack_count, Cube &cube);
    bool checkCollision(const Plane &plane);
    void reset(Cube &cube);
    void generatePoints();
    void rotateBall();
    void handleCollision(const Plane &plane);
    void handleCollision(const Cube &cube);
    void moveBall(const Cube &cube);

    void increaseSpeed(float increment);
    void decreaseSpeed(float decrement);
};
