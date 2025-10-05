/**
 * Advanced OpenGL Demo Application
 *
 * This program demonstrates basic OpenGL functionality including:
 * - Drawing and transformations
 * - Keyboard input (normal and special keys)
 * - Mouse interaction
 * - Animation using both idle and timer functions
 *
 * Use as a learning template for OpenGL with GLUT
 */

// Platform-specific includes
#ifdef __linux__
#include <GL/glut.h> // For Linux systems
#elif defined(_WIN32) || defined(WIN32)
#include <windows.h>
#include <GL/glut.h> // For Windows systems
#elif defined(__APPLE__)
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h> // For macOS systems
#else
#include <GL/glut.h> // Default fallback
#endif

#include <cstdio>
#include <cmath>
#include <ctime> // For time_t and time functions

// Global variables for animation and interaction
GLfloat secHandRotation = 0.0f;  // Rotation angle for the second hand
GLfloat minHandRotation = 0.0f;  // Rotation angle for the minute hand
GLfloat hourHandRotation = 0.0f; // Rotation angle for the hour hand
GLfloat xPosition = 0.0f;        // X position for the clock
GLfloat yPosition = 0.0f;        // Y position for the clock
GLfloat moveSpeed = 50.0f;       // Speed of movement

// Global variables for the current time
int hours, minutes, seconds;

void updateTime()
{
    time_t now = time(0);
    struct tm *currentTime = localtime(&now);
    hours = currentTime->tm_hour;
    minutes = currentTime->tm_min;
    seconds = currentTime->tm_sec;
    // printf("Current time: %02d:%02d:%02d\n", hours, minutes, seconds);
}

/**
 * Initializes OpenGL settings.
 * Called once at the beginning.
 */
void init()
{
    // Set the initial time
    // updateTime();
    // Calculate the initial rotation angles for the clock hands
    secHandRotation = 0.0f;
    minHandRotation = 0.0f;
    hourHandRotation = 0.0f;
    printf("Initialization complete. Here are the controls:\n");
    printf("----------------------------------------\n");
    printf("KEYBOARD CONTROLS:\n");
    printf("  ESC: Exit the program\n");
    printf("----------------------------------------\n");

    // Set the background (clear) color to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black

    // Enable depth testing for proper 3D rendering
    glEnable(GL_DEPTH_TEST);
}

/**
 * Handles standard keyboard input (ASCII keys).
 * Triggered when any ASCII key is pressed.
 *
 * @param key The ASCII code of the key pressed
 * @param x The x-coordinate of mouse position when key was pressed
 * @param y The y-coordinate of mouse position when key was pressed
 */
void keyboardListener(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27: // ESC key
        printf("Exiting program\n");
        exit(0);
        break;

    default:
        printf("Key pressed: '%c' (ASCII: %d)\n", key, key);
        break;
    }

    // Request a redisplay to update the scene
    glutPostRedisplay();
}

/**
 * Draws the clock with hour and minute markers centered at the origin in the XY-plane.
 * @param radius The radius of the circle
 */
void drawCircle(double radius)
{
    glBegin(GL_LINE_LOOP);
    // Draw the clock face
    for (int i = 0; i < 360; i++)
    {
        // Convert degrees to radians
        float angle = i * M_PI / 180.0;
        glVertex2f(radius * cos(angle), radius * sin(angle));
    }
    glEnd();
    // Draw hour and minute markers
    // glBegin(GL_LINES);
    // // Draw hour markers
    // for (int i = 0; i < 12; i++)
    // {
    //     // Draw hour markers
    //     float angle = i * 30.0 * M_PI / 180.0; // 30 degrees per hour
    //     glVertex2f((radius - 0.15) * cos(angle), (radius - 0.15) * sin(angle));
    //     glVertex2f((radius - 0.05) * cos(angle), (radius - 0.05) * sin(angle));
    // }
    // for (int i = 0; i < 60; i++)
    // {
    //     // Draw minute markers
    //     float angle = i * 6.0 * M_PI / 180.0; // 6 degrees per minute
    //     glVertex2f((radius - 0.1) * cos(angle), (radius - 0.1) * sin(angle));
    //     glVertex2f((radius - 0.05) * cos(angle), (radius - 0.05) * sin(angle));
    // }
    // glEnd();
}

void drawCircle2(double x, double y, double radius)
{
    glBegin(GL_LINE_LOOP);
    // Draw the clock face
    for (int i = 0; i < 360; i++)
    {
        // Convert degrees to radians
        float angle = i * M_PI / 180.0;
        glVertex2f(x + radius * cos(angle), y + radius * sin(angle));
    }
    glEnd();
}

/**
 * Draws the hour hand of the clock with the pointer on the clock face.
 * @param x The x-coordinate of the hour hand
 * @param y The y-coordinate of the hour hand
 * @param radius The radius of the clock face
 */
void drawHourHands(double x, double y, double radius)
{
    glBegin(GL_QUADS);
    glVertex2f(x - 0.015, y);
    glVertex2f(x + 0.015, y);
    glVertex2f(x + 0.015, radius * 0.5);
    glVertex2f(x - 0.015, radius * 0.5);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(x - 0.02, radius + 0.02);
    glVertex2f(x + 0.02, radius + 0.02);
    glVertex2f(x + 0.02, radius - 0.02);
    glVertex2f(x - 0.02, radius - 0.02);
    glEnd();
}

/**
 * Draws the minute hand of the clock.
 * @param x The x-coordinate of the minute hand
 * @param y The y-coordinate of the minute hand
 * @param radius The radius of the clock face
 */
void drawMinuteHands(double x, double y, double radius)
{
    glBegin(GL_QUADS);
    glVertex2f(x - 0.01, 0);
    glVertex2f(x + 0.01, 0);
    glVertex2f(x + 0.01, radius * 0.65);
    glVertex2f(x - 0.01, radius * 0.65);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(x - 0.02, radius + 0.02);
    glVertex2f(x + 0.02, radius + 0.02);
    glVertex2f(x + 0.02, radius - 0.02);
    glVertex2f(x - 0.02, radius - 0.02);
    glEnd();
}

/**
 * Draws the second hand of the clock.
 * @param x The x-coordinate of the second hand
 * @param y The y-coordinate of the second hand
 * @param radius The radius of the clock face
 */
void drawSecondHands(double x, double y, double radius)
{
    glBegin(GL_QUADS);
    glVertex2f(x - 0.005, 0);
    glVertex2f(x + 0.005, 0);
    glVertex2f(x + 0.005, radius);
    glVertex2f(x - 0.005, radius);
    glEnd();
    // glBegin(GL_QUADS);
    // glVertex2f(x - 0.02, radius + 0.02);
    // glVertex2f(x + 0.02, radius + 0.02);
    // glVertex2f(x - 0.02, radius - 0.02);
    // glEnd();
}

/**
 * The display callback function.
 * This is where rendering happens. Called whenever the window needs to be redrawn.
 */
void display()
{
    // Clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Reset transformations
    glLoadIdentity();

    // Set the camera position (simple 2D view for this example)
    gluLookAt(
        0.0, 0.0, 3.0, // Eye position
        0.0, 0.0, 0.0, // Look at point
        0.0, 1.0, 0.0  // Up vector
    );

    // Draw the clock face
    glPushMatrix();
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        drawCircle(0.5); // Draw the clock face
    }
    glPopMatrix();
    glPushMatrix();
    {
        glRotatef(secHandRotation, 0.0f, 0.0f, 1.0f); // Rotate second hand
        glColor3f(1.0f, 0.0f, 0.0f);                  // Set color to red for second hand
        drawSecondHands(0.0, 0.0, 0.5f);              // Draw the second hand
    }
    glPopMatrix();

    glPushMatrix();
    {
        glRotatef(secHandRotation, 0.0f, 0.0f, 1.0f);
        glTranslatef(0.0, 0.5, 0);
        glColor3f(0.0f, 1.0f, 0.0f);
        drawCircle(0.25);
    }
    glPopMatrix();
    glPushMatrix();
    {
        glRotatef(secHandRotation, 0.0f, 0.0f, 1.0f);
        glTranslatef(0.0, 0.5, 0);
        glRotatef(secHandRotation, 0.0f, 0.0f, 1.0f); // Rotate second hand
        glColor3f(0.0f, 1.0f, 0.0f);                  // Set color to green for second hand
        drawSecondHands(0.0, 0.0, 0.25f);             // Draw the second hand
    }
    glPopMatrix();

    glPushMatrix();
    {
        glRotatef(secHandRotation, 0.0f, 0.0f, 1.0f);
        glTranslatef(0.0, 0.5, 0);
        glRotatef(secHandRotation, 0.0f, 0.0f, 1.0f);
        glTranslatef(0.0, 0.25, 0);
        glColor3f(0.0f, 0.0f, 1.0f);
        drawCircle(0.125);
    }
    glPopMatrix();
    glPushMatrix();
    {
        glRotatef(secHandRotation, 0.0f, 0.0f, 1.0f);
        glTranslatef(0.0, 0.5, 0);
        glRotatef(secHandRotation, 0.0f, 0.0f, 1.0f);
        glTranslatef(0.0, 0.25, 0);
        glRotatef(secHandRotation, 0.0f, 0.0f, 1.0f); // Rotate second hand
        glColor3f(0.0f, 0.0f, 1.0f);                  // Set color to blue for second hand
        drawSecondHands(0.0, 0.0, 0.125f);            // Draw the second hand
    }
    glPopMatrix();

    // Display the result using double buffering
    glutSwapBuffers();
}

/**
 * Called when the window is resized.
 *
 * @param width The new width of the window
 * @param height The new height of the window
 */
void reshape(int width, int height)
{
    // Prevent division by zero
    if (height == 0)
        height = 1;

    // Set the viewport to the entire window
    glViewport(0, 0, width, height);

    // Set the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Set perspective projection
    float aspectRatio = (float)width / (float)height;
    gluPerspective(45.0f, aspectRatio, 0.1f, 100.0f);

    // Switch back to modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    printf("Window resized to %d x %d\n", width, height);
}

/**
 * Timer function for animation.
 * This demonstrates the use of a timer instead of idle function.
 * Timer functions provide better control over animation speed.
 *
 * @param value Value passed to the timer function (not used here)
 */
void timerFunction(int value)
{
    float deltaSec = moveSpeed / 1000.0f;
    secHandRotation += 45.0f * deltaSec; // 6 degrees per second
    if (secHandRotation < 0.0f)
        secHandRotation += 360.0f;
    minHandRotation -= 0.1f * deltaSec; // 0.1 degrees per second
    if (minHandRotation < 0.0f)
        minHandRotation += 360.0f;
    hourHandRotation -= 0.0083f * deltaSec; // 0.0083 degrees per second
    if (hourHandRotation < 0.0f)
        hourHandRotation += 360.0f;
    // Update the current time
    updateTime();

    // Request a redisplay
    glutPostRedisplay();

    // Register the timer again
    glutTimerFunc(moveSpeed, timerFunction, 0);
}

/**
 * Main function: Entry point of the program.
 */
int main(int argc, char **argv)
{
    // Initialize GLUT
    glutInit(&argc, argv);

    // Set initial window size and position
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);

    // Set display mode: RGB color, double buffering, and depth buffering
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    // Create a window with the title
    glutCreateWindow("OpenGL Interactive Demo");

    // Register callback functions
    glutDisplayFunc(display);           // Called when screen needs to be redrawn
    glutReshapeFunc(reshape);           // Called when window is resized
    glutKeyboardFunc(keyboardListener); // Called on ASCII keyboard input
    // Register idle function for continuous animation
    // glutIdleFunc(idleFunction); // Uncomment to use idle function for animation

    // Use timer function instead of idle function for better control of animation speed
    glutTimerFunc(1000, timerFunction, 0); // Call timer function every 1000 ms (1 second)

    // Perform initial setup
    init();

    // Start the GLUT event loop
    printf("Starting GLUT main loop...\n");
    glutMainLoop();

    return 0;
}