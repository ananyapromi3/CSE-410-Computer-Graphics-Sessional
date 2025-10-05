#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "camera.h"
#include "cube.h"
#include "point.h"
#include "vector.h"
#include "ball.h"
#include "color.h"
#include "globals.h"
#include "nagordola.h"

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h> // Use GLUT framework on macOS
#else
#include <GL/glut.h> // Use standard GLUT location on Linux/Windows
#endif

// --- Global Variables ---

GLfloat zFar = 100.0f; // Far clipping plane
GLfloat zNear = 0.05f; // Near clipping plane
GLfloat fov = 60.0f;   // Field of view

Color green = Color(0.0f, 1.0f, 0.0f);
Color red = Color(1.0f, 0.0f, 0.0f);

GLfloat NagordolaR = 2.5f;
GLfloat speed = 5.0f;
Nagordola nagordola(NagordolaR);

// Axes
Vector xAxis(1, 0, 0);
Vector yAxis(0, 1, 0);
Vector zAxis(0, 0, 1);

// Cube dimensions
GLfloat height = 2.5f;
GLfloat width = 5.0f;
GLfloat depth = 5.0f;
Cube cube(Point(0, 0, 0), 2 * width, 2 * height, 2 * depth); // Cube object
bool isCube = true;                                          // Toggle for cube
// cube.setAxes(new Vector[3]{xAxis, yAxis, zAxis}); // Set cube axes

// Camera position and orientation
GLfloat eyex = 10, eyey = 5, eyez = 10;        // Camera position coordinates
GLfloat centerx = 0, centery = 0, centerz = 0; // Look-at point coordinates
GLfloat upx = 0, upy = 1, upz = 0;             // Up vector coordinates
GLfloat cameraSpeed = 0.1f;                    // Camera movement speed
GLfloat cameraRotateSpeed = 0.1f;              // Camera rotation speed
Camera camera(Point(eyex, eyey, eyez), Point(centerx, centery, centerz), Vector(upx, upy, upz), cameraSpeed, cameraRotateSpeed);

// Ball
GLfloat radii = 0.2f;
GLfloat ballSpeed = 10.0f;
// Ball ball(radii, Point(0, -height + radii, 0), Vector(0.0f, 1.0f, 0.0f), 36, 18, Vector(1.0f, 1.0f, 1.0f), ballSpeed);
Ball ball(radii, 36, 18, cube); // Ball object

bool isAxes = true;
bool isArrow = false;

GLfloat ang = 0.0f; // Angle for nagordola rotation

// float yaw = 0, pitch = 0, roll = 0;
// float moveSpeed = 0.1f;
// float rotSpeed = 0.1f;

// --- Function Declarations ---
void initGL();
void display();
void reshapeListener(GLsizei width, GLsizei height);
void keyboardListener(unsigned char key, int x, int y);
void specialKeyListener(int key, int x, int y);
void drawAxes();
void drawCube();
// void drawPyramid();
void drawCheckerboardBottom(float x, float y, float z, float size, int rows, int cols);
void drawBall(Ball &ball);
void drawCone(Point baseCenter, Vector up, float height, float radius, Color color, int segments = 36);
void drawArrow(Point start, Vector dir, float len);
void drawLine(Point x, Point y);

/**
 * Initialize OpenGL settings
 * Sets up background color and enables depth testing
 */
void initGL()
{
    glClearColor(51.0f / 255.0f, 51.0f / 255.0f, 51.0f / 255.0f, 1.0f); // Background
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Enable depth testing for z-culling
}

void drawCircle(double radius)
{
    glBegin(GL_LINE_LOOP);
    // Draw the clock face
    for (int i = 0; i < 360; i++)
    {
        // Convert degrees to radians
        float angle = i * M_PI / 180.0;
        glVertex3f(radius * cos(angle), 0.0f, radius * sin(angle));
    }
    glEnd();
}

void drawCircle2(float x, float y, float radius)
{
    glBegin(GL_TRIANGLE_FAN);

    // Center of the circle
    glVertex2f(x, y);

    // Circle points around the perimeter
    for (int i = 0; i <= 360; i++)
    {
        float angle = i * M_PI / 180.0;
        glVertex2f(x + radius * cos(angle), y + radius * sin(angle));
    }

    glEnd();
}

void drawSquare(Plane p, float size, Point p2)
{
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    Vector normal = p.normal;
    Point center = p.point;
    Vector a = normal.cross(Vector(1, 0, 0)).normalize();
    Vector b = a.rotate(normal, 90.0f);
    Point a_ = center + a * size;
    glVertex3f(a_.x, a_.y, a_.z);
    Point c_ = center + b * size;
    glVertex3f(c_.x, c_.y, c_.z);
    Point b_ = center - a * size;
    glVertex3f(b_.x, b_.y, b_.z);
    Point d_ = center - b * size;
    glVertex3f(d_.x, d_.y, d_.z);
    glEnd();
    // glBegin(GL_LINE);
    // glVertex3f(a_.x, 0.0f, a_.z);
    // glVertex3f(a_.x, a_.y, a_.z);
    // glVertex3f(b_.x, 0.0f, b_.z);
    // glVertex3f(b_.x, b_.y, b_.z);
    // glVertex3f(c_.x, 0.0f, c_.z);
    // glVertex3f(c_.x, c_.y, c_.z);
    // glVertex3f(d_.x, 0.0f, d_.z);
    // glVertex3f(d_.x, d_.y, d_.z);
    // glEnd();
    // Point temp = center + normal.normalize() * nagordola.height;
    drawLine(a_, p2);
    drawLine(b_, p2);
    drawLine(c_, p2);
    drawLine(d_, p2);
}

/**
 * Main display function
 * Sets up the camera and renders visible objects
 */
void display()
{
    // Clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set up the model-view matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(camera.eye.x, camera.eye.y, camera.eye.z,          // Camera position
              camera.center.x, camera.center.y, camera.center.z, // Look-at point
              camera.up.x, camera.up.y, camera.up.z);            // Up vector

    // drawCube();
    glRotatef(ang, 0, 1, 0); // Rotate the nagordola
    if (isAxes)
        drawAxes();

    drawCircle(NagordolaR);

    glPushMatrix();
    for (int i = 0; i < nagordola.getCount(); i++)
    {
        float angle = i * 60.0f;
        float x = nagordola.radius * cos(angle * M_PI / 180.0f);
        float z = nagordola.radius * sin(angle * M_PI / 180.0f);
        Plane p = nagordola.planes[i];
        glPushMatrix();
        drawSquare(p, nagordola.blocKSize, nagordola.points[i]);
        glPopMatrix();
    }
    glPopMatrix();

    // drawBall(ball);

    // if (ball.showVelocityArrow)
    //     drawArrow(ball.center, ball.velocity.normalize(), ball.radius * 2);

    // drawCheckerboardBottom(-width, -height, -depth, 2 * width, 20, 20);

    // Swap buffers (double buffering)
    glutSwapBuffers();
}

/**
 * Window reshape callback
 * Handles window resizing and maintains aspect ratio
 */
void reshapeListener(GLsizei width, GLsizei height)
{
    // Prevent division by zero
    if (height == 0)
        height = 1;

    // Calculate aspect ratio
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set viewport to cover entire window
    glViewport(0, 0, width, height);

    // Set up perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // 45-degree field of view, aspect ratio, near and far clipping planes
    gluPerspective(fov, aspect, zNear, zFar);
}

/**
 * Keyboard input handler for standard keys
 * Manages camera position, object visibility, and program exit
 */
void keyboardListener(unsigned char key, int x, int y)
{
    switch (key)
    {
    case '1':
        camera.lookLeft();
        break;
    case '2':
        camera.lookRight();
        break;
    case '3':
        camera.lookUp();
        break;
    case '4':
        camera.lookDown();
        break;
    case '5':
        camera.tiltClockwise();
        break;
    case '6':
        camera.tiltAntiClockwise();
        break;
    case 'w':
        camera.moveUp_wo_refPoint();
        break;
    case 's':
        camera.moveDown_wo_refPoint();
        break;
    case ' ':
        ball.isMoving = !ball.isMoving;
        break;
    case 'r':
        if (!ball.isMoving)
            ball.reset(cube);
        break;
    case 'v':
        ball.showVelocityArrow = !ball.showVelocityArrow; // Toggle velocity arrow
        break;
    case '+':
        if (ball.isReset)
            ball.increaseSpeed(1.0f);
        break;
    case '-':
        if (ball.isReset)
            ball.decreaseSpeed(1.0f);
        break;
    case 'q':
        nagordola.isRotating = true;
        nagordola.spread();
        break;
    case 'a':
        nagordola.isRotating = true;
        nagordola.shrink();
        break;
    case 'm':
        ang += 1.0f; // Increase nagordola rotation angle
        break;
    case 'n':
        ang -= 1.0f; // Decrease nagordola rotation angle
        break;

    case 27:
        exit(0);
        break;
    }

    glutPostRedisplay(); // Request a screen refresh
}

/**
 * Special key input handler (arrow keys, function keys)
 * Provides camera orbit functionality
 */
void specialKeyListener(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_LEFT:
        camera.moveLeft();
        break;

    case GLUT_KEY_RIGHT:
        camera.moveRight();
        break;

    case GLUT_KEY_UP:
        camera.moveForward();
        break;

    case GLUT_KEY_DOWN:
        camera.moveBackward();
        break;

    case GLUT_KEY_PAGE_UP:
        camera.moveUp();
        break;

    case GLUT_KEY_PAGE_DOWN:
        camera.moveDown();
        break;
    }

    glutPostRedisplay(); // Request a screen refresh
}

/**
 * Draw coordinate axes
 * X axis: red, Y axis: green, Z axis: blue
 */
void drawAxes()
{
    glLineWidth(3); // Set line thickness

    glBegin(GL_LINES);

    // X axis (red)
    glColor3f(1, 0, 0);
    glVertex3f(-5, 0, 0);
    glVertex3f(5, 0, 0);

    // Y axis (green)
    glColor3f(0, 1, 0);
    glVertex3f(0, -5, 0);
    glVertex3f(0, 5, 0);

    // Z axis (blue)
    glColor3f(0, 0, 1);
    glVertex3f(0, 0, -5);
    glVertex3f(0, 0, 5);

    glEnd();
}

void drawCheckerboardBottom(float x, float y, float z, float size, int rows, int cols)
{
    float tileSize = size / rows;
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            if ((i + j) % 2 == 0)
                glColor3f(1.0f, 1.0f, 1.0f); // White
            else
                glColor3f(0.0f, 0.0f, 0.0f); // Black
            float x0 = x + i * tileSize;
            float z0 = z + j * tileSize;
            float x1 = x0 + tileSize;
            float z1 = z0 + tileSize;
            glBegin(GL_QUADS);
            glVertex3f(x0, y, z0);
            glVertex3f(x1, y, z0);
            glVertex3f(x1, y, z1);
            glVertex3f(x0, y, z1);
            glEnd();
        }
    }
}

void drawLine(Point x, Point y)
{
    glBegin(GL_LINES);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(x.x, x.y, x.z);
    glVertex3f(y.x, y.y, y.z);
    glEnd();
}

void drawTriangle(Point x, Point y, Point z, Color color)
{
    glBegin(GL_TRIANGLES);
    glColor3f(color.r, color.g, color.b);
    glVertex3f(x.x, x.y, x.z);
    glVertex3f(y.x, y.y, y.z);
    glVertex3f(z.x, z.y, z.z);
    glEnd();
}

void drawArrow(Point start, Vector dir, float len)
{
    Vector d = dir.normalize();
    Point end = {start.x + d.x * len, start.y + d.y * len, start.z + d.z * len};
    glBegin(GL_LINES);
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(start.x, start.y, start.z);
    glVertex3f(end.x, end.y, end.z);
    glEnd();
    drawCone(end, dir, len / 10.0f, len / 10.0f, red, 36);
}

void drawCone(Point baseCenter, Vector up, float height, float radius, Color color, int segments)
{
    // Compute tip position
    Vector dir = up.normalize(); // Unit vector
    Point tip = baseCenter + dir * height;

    // Find orthonormal vectors u and v perpendicular to 'dir'
    Vector arbitrary = (fabs(dir.x) < 0.9f) ? Vector(1, 0, 0) : Vector(0, 0, 1);
    Vector u = dir.cross(arbitrary).normalize();
    Vector v = dir.cross(u).normalize();

    // Draw cone sides
    glColor3f(color.r, color.g, color.b);
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < segments; i++)
    {
        float theta1 = 2 * M_PI * i / segments;
        float theta2 = 2 * M_PI * (i + 1) / segments;

        // Point 1 on base circle
        Point p1 = baseCenter + u * (cos(theta1) * radius) + v * (sin(theta1) * radius);
        Point p2 = baseCenter + u * (cos(theta2) * radius) + v * (sin(theta2) * radius);

        glVertex3f(tip.x, tip.y, tip.z);
        glVertex3f(p1.x, p1.y, p1.z);
        glVertex3f(p2.x, p2.y, p2.z);
    }
    glEnd();

    // Draw base disk
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(baseCenter.x, baseCenter.y, baseCenter.z); // center
    for (int i = 0; i <= segments; i++)
    {
        float theta = 2 * M_PI * i / segments;
        Point p = baseCenter + u * (cos(theta) * radius) + v * (sin(theta) * radius);
        glVertex3f(p.x, p.y, p.z);
    }
    glEnd();
}

/**
 * Draw a colored cube centered at the origin
 * Each face has a different color
 */
void drawCube()
{
    glBegin(GL_QUADS);

    // Top face (y = 1.0f) - Purple
    glColor4f(125.0f / 255.0f, 125.0f / 255.0f, 204.0f / 255.0f, 0.9f);
    glVertex3f(width, height, -depth);
    glVertex3f(-width, height, -depth);
    glVertex3f(-width, height, depth);
    glVertex3f(width, height, depth);

    // Bottom face (y = -1.0f) - White
    // glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
    // glVertex3f(width, -height, depth);
    // glVertex3f(-width, -height, depth);
    // glVertex3f(-width, -height, -depth);
    // glVertex3f(width, -height, -depth);

    // Front face  (z = 1.0f) - Green
    glColor4f(0.0f, 207.0f / 255.0f, 206.0f, 1.0f);
    glVertex3f(width, height, depth);
    glVertex3f(-width, height, depth);
    glVertex3f(-width, -height, depth);
    glVertex3f(width, -height, depth);

    // Back face (z = -1.0f) - Yellow
    glColor4f(204.0f / 255.0f, 204.0f / 255.0f, 41.0f / 255.0f, 1.0f);
    glVertex3f(width, -height, -depth);
    glVertex3f(-width, -height, -depth);
    glVertex3f(-width, height, -depth);
    glVertex3f(width, height, -depth);

    // Left face (x = -1.0f) - Red
    glColor4f(221.0f / 255.0f, 64.0f / 255.0f, 71.0f / 255.0f, 1.0f);
    glVertex3f(-width, height, depth);
    glVertex3f(-width, height, -depth);
    glVertex3f(-width, -height, -depth);
    glVertex3f(-width, -height, depth);

    // Right face (x = 1.0f) - Cyan
    glColor4f(0.0f, 207.0f / 255.0f, 51.0f / 255.0f, 1.0f);
    glVertex3f(width, height, -depth);
    glVertex3f(width, height, depth);
    glVertex3f(width, -height, depth);
    glVertex3f(width, -height, -depth);

    glEnd();
}

// Draw ball
void drawBall(Ball &ball)
{
    // Set ball color
    glColor3f(1.0f, 0.0f, 0.0f); // Red

    int k1, k2;
    for (int i = 0; i < ball.stack_count; ++i)
    {
        k1 = i * (ball.sector_count + 1); // beginning of current stack
        k2 = k1 + ball.sector_count + 1;  // beginning of next stack
        Color color = green;
        for (int j = 0; j < ball.sector_count; ++j, ++k1, ++k2)
        {
            if (i < ball.stack_count / 2)
            {
                if (j % 2)
                {
                    color = green;
                }
                else
                {
                    color = red;
                }
            }
            else
            {
                if (j % 2)
                {
                    color = red;
                }
                else
                {
                    color = green;
                }
            }
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if (i != 0)
            {
                drawTriangle(ball.points[k1], ball.points[k2], ball.points[k1 + 1], color);
                // indices.push_back(k1);
                // indices.push_back(k2);
                // indices.push_back(k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if (i != (ball.stack_count - 1))
            {
                drawTriangle(ball.points[k1 + 1], ball.points[k2], ball.points[k2 + 1], color);
                // indices.push_back(k1 + 1);
                // indices.push_back(k2);
                // indices.push_back(k2 + 1);
            }

            // store indices for lines
            // vertical lines for all stacks, k1 => k2
            // drawLine(ball.points[k1], ball.points[k2]);
            // lineIndices.push_back(k1);
            // lineIndices.push_back(k2);
            if (i != 0) // horizontal lines except 1st stack, k1 => k+1
            {

                // drawLine(ball.points[k1], ball.points[k1 + 1]);
                // lineIndices.push_back(k1);
                // lineIndices.push_back(k1 + 1);
            }
        }
    }
}
void timerFunction(int value)
{
    // if (ball.isMoving && !ball.onGround)
    // {
    //     ball.rotateBall();
    //     ball.moveBall(cube);
    //     // ball.handleCollision(cube);
    // }
    if (nagordola.isRotating)
    {
        nagordola.rotate();
    }

    // Request a redisplay
    glutPostRedisplay();

    // Register the timer again
    glutTimerFunc(animationSpeed, timerFunction, 0);
}

/**
 * Main function: Program entry point
 */
int main(int argc, char **argv)
{
    srand(time(0)); // Seed for random number generation
    // Initialize GLUT
    glutInit(&argc, argv);

    // Configure display mode and window
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Camera Movement");

    // Register callback functions
    glutDisplayFunc(display);
    glutReshapeFunc(reshapeListener);
    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);

    // Initialize OpenGL settings
    initGL();

    // Enter the GLUT event loop
    // Before calling glutMainLoop()
    glutSetKeyRepeat(GLUT_KEY_REPEAT_ON); // This enables key repeat events

    glutTimerFunc(animationSpeed, timerFunction, 0);

    glutMainLoop();

    return 0;
}