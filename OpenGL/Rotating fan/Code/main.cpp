#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctime>

#include "camera.h"
#include "point.h"
#include "vector.h"
#include "globals.h"

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h> // Use GLUT framework on macOS
#else
#include <GL/glut.h> // Use standard GLUT location on Linux/Windows
#endif

// --- Global Variables ---

GLfloat fanStandRotation = 0.0f;
GLfloat fanRotation = 0.0f;
GLfloat speedFan = 1.0f;

GLfloat zFar = 100.0f; // Far clipping plane
GLfloat zNear = 0.05f; // Near clipping plane
GLfloat fov = 60.0f;   // Field of view

;

// Axes
Vector xAxis(1, 0, 0);
Vector yAxis(0, 1, 0);
Vector zAxis(0, 0, 1);

// Cube dimensions
GLfloat height = 2.5f;
GLfloat width = 5.0f;
GLfloat depth = 5.0f;

bool isCube = true; // Toggle for cube
// cube.setAxes(new Vector[3]{xAxis, yAxis, zAxis}); // Set cube axes

// Camera position and orientation
GLfloat eyex = 10, eyey = 5, eyez = 10;        // Camera position coordinates
GLfloat centerx = 0, centery = 0, centerz = 0; // Look-at point coordinates
GLfloat upx = 0, upy = 1, upz = 0;             // Up vector coordinates
GLfloat cameraSpeed = 0.1f;                    // Camera movement speed
GLfloat cameraRotateSpeed = 0.1f;              // Camera rotation speed
Camera camera(Point(eyex, eyey, eyez), Point(centerx, centery, centerz), Vector(upx, upy, upz), cameraSpeed, cameraRotateSpeed);

bool isAxes = true;
bool isArrow = false;

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

void drawCuboidal(double width, double height, double depth);
void drawFan(double len);

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
    glPushMatrix();
    {
        // glTranslatef(0, 0, 0);
        glRotatef(fanStandRotation, 1, 0, 0);
        // glScalef(1.0f, 1.0f, 1.0f);
        drawCuboidal(5.0, 1.0, 5.0);
        drawCuboidal(0.5, 1.0, 0.5);
    }
    glPopMatrix();
    glPushMatrix();
    {
        glRotatef(fanStandRotation, 1, 0, 0);
        glRotatef(fanRotation, 0, 1, 0);
        drawFan(5.0);
    }
    glPopMatrix();

    // glPushMatrix();
    // {
    //     // glTranslatef(0, 0, 0);
    //     glRotatef(fanStandRotation, 1, 0, 0);
    //     // glScalef(1.0f, 1.0f, 1.0f);
    //     drawFan(0.75);
    // }

    if (isAxes)
        drawAxes();

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
    case 'n':
        fanStandRotation += 1.0;
        break;
    case 'm':
        fanStandRotation -= 1.0;
        break;
    case '+':
        speedFan += 1.0f;
        break;
    case '-':
        speedFan -= 1.0f;
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
    glVertex3f(0, 0, 0);
    glVertex3f(50, 0, 0);

    // Y axis (green)
    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 50, 0);

    // Z axis (blue)
    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 50);

    glEnd();
}

void drawLine(Point x, Point y)
{
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3f(x.x, x.y, x.z);
    glVertex3f(y.x, y.y, y.z);
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

void drawCuboidal(double width, double height, double depth)
{
    glBegin(GL_QUADS);

    // Front face  (z = 1.0f) - Cyan
    glColor4f(0.0f, 207.0f / 255.0f, 206.0f, 1.0f);
    glVertex3f(width * 0.5, height, depth);
    glVertex3f(-width * 0.5, height, depth);
    glVertex3f(-width * 0.5, -height, depth);
    glVertex3f(width * 0.5, -height, depth);

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glVertex3f(-width * 0.5, height, depth);
    glVertex3f(-width * 0.5, -height, depth);
    glVertex3f(-width, -height, depth * 0.5);
    glVertex3f(-width, height, depth * 0.5);

    // Left face (x = -1.0f) - Red
    glColor4f(221.0f / 255.0f, 64.0f / 255.0f, 71.0f / 255.0f, 1.0f);
    glVertex3f(-width, height, depth * 0.5);
    glVertex3f(-width, height, -depth * 0.5);
    glVertex3f(-width, -height, -depth * 0.5);
    glVertex3f(-width, -height, depth * 0.5);

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glVertex3f(-width, height, -depth * 0.5);
    glVertex3f(-width, -height, -depth * 0.5);
    glVertex3f(-width * 0.5, -height, -depth);
    glVertex3f(-width * 0.5, height, -depth);

    // Back face (z = -1.0f) - Yellow
    glColor4f(204.0f / 255.0f, 204.0f / 255.0f, 41.0f / 255.0f, 1.0f);
    glVertex3f(width * 0.5, -height, -depth);
    glVertex3f(-width * 0.5, -height, -depth);
    glVertex3f(-width * 0.5, height, -depth);
    glVertex3f(width * 0.5, height, -depth);

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glVertex3f(width * 0.5, height, -depth);
    glVertex3f(width * 0.5, -height, -depth);
    glVertex3f(width, -height, -depth * 0.5);
    glVertex3f(width, height, -depth * 0.5);

    // Right face (x = 1.0f) - Green
    glColor4f(0.0f, 207.0f / 255.0f, 51.0f / 255.0f, 1.0f);
    glVertex3f(width, height, -depth * 0.5);
    glVertex3f(width, height, depth * 0.5);
    glVertex3f(width, -height, depth * 0.5);
    glVertex3f(width, -height, -depth * 0.5);

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glVertex3f(width * 0.5, height, depth);
    glVertex3f(width * 0.5, -height, depth);
    glVertex3f(width, -height, depth * 0.5);
    glVertex3f(width, height, depth * 0.5);

    glEnd();
}

void drawFan(double len)
{
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(len / 2, 1.0, 0);
    glVertex3f(-len / 2, 1.0, 0);

    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(len * cos(M_PI / 3) / 2, 1.0, len * sin(M_PI / 3) / 2.0);
    glVertex3f(-len * cos(M_PI / 3) / 2, 1.0, -len * sin(M_PI / 3) / 2.0);

    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(len * cos(M_PI / 3) / 2, 1.0, -len * sin(M_PI / 3) / 2.0);
    glVertex3f(-len * cos(M_PI / 3) / 2, 1.0, len * sin(M_PI / 3) / 2.0);
    glEnd();
}

void timerFunction(int value)
{

    fanRotation += speedFan;
    if (fanRotation > 360.0f)
        fanRotation -= 360.0f;
    if (fanRotation < 0)
        fanRotation = 0.0;

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