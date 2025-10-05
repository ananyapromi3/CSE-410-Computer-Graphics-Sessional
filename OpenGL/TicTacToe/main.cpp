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

// Global variables for animation and interaction
GLfloat squareRotation = 0.0f;   // Rotation angle for the first square
GLfloat triangleRotation = 0.0f; // Rotation angle for the triangle
GLfloat xPosition = 0.0f;        // X position for the second square
GLfloat yPosition = 0.0f;        // Y position for the second square
GLfloat moveSpeed = 0.01f;       // Speed of movement

// Animation control flags
bool animateSquare = true;   // Controls square rotation
bool animateTriangle = true; // Controls triangle rotation
int animationSpeed = 30;     // Timer interval in milliseconds

// Mouse tracking
int lastMouseX = 0;
int lastMouseY = 0;
bool mouseLeftDown = false; // Track if left mouse button is pressed

/**
 * Initializes OpenGL settings.
 * Called once at the beginning.
 */
void init()
{
    printf("Initialization complete. Here are the controls:\n");
    printf("----------------------------------------\n");
    printf("KEYBOARD CONTROLS:\n");
    printf("  S/s: Toggle square rotation\n");
    printf("  T/t: Toggle triangle rotation\n");
    printf("  +/-: Increase/decrease animation speed\n");
    printf("  Arrow keys: Move the green square\n");
    printf("  R/r: Reset all animations\n");
    printf("  ESC: Exit the program\n");
    printf("MOUSE CONTROLS:\n");
    printf("  Left click & drag: Rotate view\n");
    printf("  Right click: Reset view\n");
    printf("----------------------------------------\n");

    // Set the background (clear) color to dark blue
    glClearColor(0.0f, 0.0f, 0.2f, 1.0f);

    // Enable depth testing for proper 3D rendering
    glEnable(GL_DEPTH_TEST);
}

bool xTurn = true;
int board[3][3];
bool gameOver = false;
int winner = -1;
bool circles[3][3];
bool crosses[3][3];

void gamePlay()
{
    for (int i = 0; i < 3; i++)
    {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != -1)
        {
            gameOver = true;
            if (board[i][0] == 0)
                winner = 0;
            else
                winner = 1;
        }
    }
    for (int i = 0; i < 3; i++)
    {
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != -1)
        {
            gameOver = true;
            if (board[0][i] == 0)
                winner = 0;
            else
                winner = 1;
        }
    }
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != -1)
    {
        gameOver = true;
        if (board[0][0] == 0)
            winner = 0;
        else
            winner = 1;
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != -1)
    {
        gameOver = true;
        if (board[0][2] == 0)
            winner = 0;
        else
            winner = 1;
    }
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
    case '1':
        if (board[0][0] == -1)
        {
            if (xTurn)
            {
                board[0][0] = 0;
                crosses[0][0] = true;
                xTurn = false;
            }
            else
            {
                board[0][0] = 1;
                circles[0][0] = true;
                xTurn = true;
            }
            gamePlay();
        }
        break;
    case '2':
        if (board[0][1] == -1)
        {
            if (xTurn)
            {
                board[0][1] = 0;
                crosses[0][1] = true;
                xTurn = false;
            }
            else
            {
                board[0][1] = 1;
                circles[0][1] = true;
                xTurn = true;
            }
            gamePlay();
        }
        break;
    case '3':
        if (board[0][2] == -1)
        {
            if (xTurn)
            {
                board[0][2] = 0;
                crosses[0][2] = true;
                xTurn = false;
            }
            else
            {
                board[0][2] = 1;
                circles[0][2] = true;
                xTurn = true;
            }
            gamePlay();
        }
        break;
    case '4':
        if (board[1][0] == -1)
        {
            if (xTurn)
            {
                board[1][0] = 0;
                crosses[1][0] = true;
                xTurn = false;
            }
            else
            {
                board[1][0] = 1;
                circles[1][0] = true;
                xTurn = true;
            }
            gamePlay();
        }
        break;
    case '5':
        if (board[1][1] == -1)
        {
            if (xTurn)
            {
                board[1][1] = 0;
                crosses[1][1] = true;
                xTurn = false;
            }
            else
            {
                board[1][1] = 1;
                circles[1][1] = true;
                xTurn = true;
            }
            gamePlay();
        }
        break;
    case '6':
        if (board[1][2] == -1)
        {
            if (xTurn)
            {
                board[1][2] = 0;
                crosses[1][2] = true;
                xTurn = false;
            }
            else
            {
                board[1][2] = 1;
                circles[1][2] = true;
                xTurn = true;
            }
            gamePlay();
        }
        break;
    case '7':
        if (board[2][0] == -1)
        {
            if (xTurn)
            {
                board[2][0] = 0;
                crosses[2][0] = true;
                xTurn = false;
            }
            else
            {
                board[2][0] = 1;
                circles[2][0] = true;
                xTurn = true;
            }
            gamePlay();
        }
        break;
    case '8':
        if (board[2][1] == -1)
        {
            if (xTurn)
            {
                board[2][1] = 0;
                crosses[2][1] = true;
                xTurn = false;
            }
            else
            {
                board[2][1] = 1;
                circles[2][1] = true;
                xTurn = true;
            }
            gamePlay();
        }
        break;
    case '9':
        if (board[2][2] == -1)
        {
            if (xTurn)
            {
                board[2][2] = 0;
                crosses[2][2] = true;
                xTurn = false;
            }
            else
            {
                board[2][2] = 1;
                circles[2][2] = true;
                xTurn = true;
            }
            gamePlay();
        }
        break;
    case 'S':
    case 's':
        // Toggle square rotation
        animateSquare = !animateSquare;
        printf("Square rotation %s\n", animateSquare ? "enabled" : "disabled");
        break;

    case 'T':
    case 't':
        // Toggle triangle rotation
        animateTriangle = !animateTriangle;
        printf("Triangle rotation %s\n", animateTriangle ? "enabled" : "disabled");
        break;

    case '+':
        // Increase animation speed
        animationSpeed = (animationSpeed <= 10) ? 10 : animationSpeed - 5;
        printf("Animation speed increased (timer interval: %d ms)\n", animationSpeed);
        break;

    case '-':
        // Decrease animation speed
        animationSpeed += 5;
        printf("Animation speed decreased (timer interval: %d ms)\n", animationSpeed);
        break;

    case 'R':
    case 'r':
        // Reset animations
        squareRotation = 0.0f;
        triangleRotation = 0.0f;
        xPosition = 0.0f;
        yPosition = 0.0f;
        animateSquare = true;
        animateTriangle = true;
        printf("Animations reset\n");
        break;

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
 * Handles special keyboard input (non-ASCII keys like arrows, F1, etc.).
 * Triggered when any special key is pressed.
 *
 * @param key The code of the special key pressed
 * @param x The x-coordinate of mouse position when key was pressed
 * @param y The y-coordinate of mouse position when key was pressed
 */
void specialKeyListener(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        // Move the green square up
        yPosition += moveSpeed;
        printf("Moving square up (y = %.2f)\n", yPosition);
        break;

    case GLUT_KEY_DOWN:
        // Move the green square down
        yPosition -= moveSpeed;
        printf("Moving square down (y = %.2f)\n", yPosition);
        break;

    case GLUT_KEY_LEFT:
        // Move the green square left
        xPosition -= moveSpeed;
        printf("Moving square left (x = %.2f)\n", xPosition);
        break;

    case GLUT_KEY_RIGHT:
        // Move the green square right
        xPosition += moveSpeed;
        printf("Moving square right (x = %.2f)\n", xPosition);
        break;

    case GLUT_KEY_F1:
        printf("Help:\n");
        printf("  Use arrow keys to move the green square\n");
        printf("  Press S/s to toggle square rotation\n");
        printf("  Press T/t to toggle triangle rotation\n");
        break;

    default:
        printf("Special key pressed (code: %d)\n", key);
        break;
    }

    // Request a redisplay to update the scene
    glutPostRedisplay();
}

/**
 * Handles mouse button presses.
 *
 * @param button Which mouse button was pressed (GLUT_LEFT_BUTTON, GLUT_RIGHT_BUTTON, etc.)
 * @param state The state of the button (GLUT_DOWN or GLUT_UP)
 * @param x The x-coordinate where the mouse button was pressed
 * @param y The y-coordinate where the mouse button was pressed
 */
void mouseListener(int button, int state, int x, int y)
{
    // Store the mouse position
    lastMouseX = x;
    lastMouseY = y;

    if (button == GLUT_LEFT_BUTTON)
    {
        // Track left mouse button state
        mouseLeftDown = (state == GLUT_DOWN);
        printf("Left mouse button %s at (%d, %d)\n",
               (state == GLUT_DOWN) ? "pressed" : "released", x, y);
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        // Reset view on right click
        squareRotation = 0.0f;
        triangleRotation = 0.0f;
        printf("View reset via right mouse button\n");
        glutPostRedisplay();
    }
}

/**
 * Handles mouse movement while a button is pressed.
 *
 * @param x The current x-coordinate of the mouse
 * @param y The current y-coordinate of the mouse
 */
void mouseMotionListener(int x, int y)
{
    if (mouseLeftDown)
    {
        // Calculate the difference from the last position
        int deltaX = x - lastMouseX;

        // Rotate the scene based on mouse movement
        squareRotation += deltaX * 0.5f;

        // Update the stored position
        lastMouseX = x;
        lastMouseY = y;

        // Request a redisplay
        glutPostRedisplay();
    }
}

/**
 * Draws a square centered at the origin in the XY-plane.
 *
 * @param a Half-length of a side of the square
 */
void drawSquare(double a)
{
    glBegin(GL_QUADS);
    {
        glVertex3f(a, a, 0);   // Top-right corner
        glVertex3f(a, -a, 0);  // Bottom-right corner
        glVertex3f(-a, -a, 0); // Bottom-left corner
        glVertex3f(-a, a, 0);  // Top-left corner
    }
    glEnd();
}

/**
 * Draws a triangle centered at the origin in the XY-plane.
 *
 * @param a Half-length of the base of the triangle
 */
void drawTriangle(double a)
{
    glBegin(GL_TRIANGLES);
    {
        glVertex3f(0, a, 0);   // Top vertex
        glVertex3f(a, -a, 0);  // Bottom-right vertex
        glVertex3f(-a, -a, 0); // Bottom-left vertex
    }
    glEnd();
}

/**
 * Draws coordinate axes.
 *
 * @param length The length of each axis
 */
void drawAxes(double length)
{
    glBegin(GL_LINES);
    {
        // X-axis (red)
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-length, 0.0f, 0.0f);
        glVertex3f(length, 0.0f, 0.0f);

        // Y-axis (green)
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, -length, 0.0f);
        glVertex3f(0.0f, length, 0.0f);

        // Z-axis (blue)
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, -length);
        glVertex3f(0.0f, 0.0f, length);
    }
    glEnd();
}

void drawBoard()
{
    glBegin(GL_LINES);
    {
        // X-axis
        glColor3f(1.0f, 1.0f, 1.0f);

        glVertex3f(-1.0, 0.33f, 0.0f);
        glVertex3f(1.0, 0.33f, 0.0f);

        glVertex3f(-1.0, -0.33f, 0.0f);
        glVertex3f(1.0, -0.33f, 0.0f);

        // Y-axis
        glColor3f(1.0f, 1.0f, 1.0f);

        glVertex3f(0.33f, -1.0, 0.0f);
        glVertex3f(0.33f, 1.0, 0.0f);

        glVertex3f(-0.33f, -1.0, 0.0f);
        glVertex3f(-0.33f, 1.0, 0.0f);
    }
    glEnd();
}

void drawCircle(float radius, int segments = 36)
{
    // glBegin(GL_POLYGON); // or GL_LINE_LOOP for outline
    glBegin(GL_LINE_LOOP);
    glColor3f(0.0f, 1.0f, 0.0f);
    for (int i = 0; i < segments; i++)
    {
        float theta = 2.0f * M_PI * float(i) / float(segments);
        float x = radius * cosf(theta);
        float y = radius * sinf(theta);
        glVertex2f(x, y); // Use glVertex3f(x, y, z) if needed
    }
    glEnd();
}

void drawCross(double len)
{
    glBegin(GL_LINES);
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-len, -len, 0.0f);
        glVertex3f(len, len, 0.0f);

        glVertex3f(len, -len, 0.0f);
        glVertex3f(-len, len, 0.0f);
    }
    glEnd();
}

void renderBitmapString(float x, float y, void *font, const char *string)
{
    glRasterPos2f(x, y);
    while (*string)
    {
        glutBitmapCharacter(font, *string);
        string++;
    }
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

    // Draw coordinate axes
    // drawAxes(1.5);
    drawBoard();
    // drawCircle(0.25f);
    // drawCross(0.25f);
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (crosses[i][j])
            {
                glPushMatrix();
                // glTranslatef(i * 0.66f - 1.0f, j * 0.66f - 1.0f, 0);
                glTranslatef(-0.66f + j * 0.66f, 0.66f - i * 0.66f, 0);
                drawCross(0.25);
                glPopMatrix();
            }
            else if (circles[i][j])
            {
                glPushMatrix();
                glTranslatef(-0.66f + j * 0.66f, 0.66f - i * 0.66f, 0);
                drawCircle(0.25);
                glPopMatrix();
            }
        }
    }
    if (xTurn)
    {
        renderBitmapString(-0.25, 1.0, GLUT_BITMAP_HELVETICA_18, "Player X's Turn");
    }
    else
    {
        renderBitmapString(-0.25, 1.0, GLUT_BITMAP_HELVETICA_18, "Player Y's Turn");
    }
    if (gameOver)
    {
        if (winner == -1)
        {
            renderBitmapString(-0.25, -1.0, GLUT_BITMAP_HELVETICA_18, "Game Over! It's a Draw!");
        }
        else if (winner == 0)
        {
            renderBitmapString(-0.25, -1.0, GLUT_BITMAP_HELVETICA_18, "Game Over! Player X wins!");
        }
        else if (winner == 1)
        {
            renderBitmapString(-0.25, -1.0, GLUT_BITMAP_HELVETICA_18, "Game Over! Player Y wins!");
        }
    }

    // Draw a rotating red square
    // glPushMatrix();
    // {
    //     glRotatef(squareRotation, 0, 0, 1); // Rotate around Z-axis
    //     glTranslatef(0.5, 0.5, 0);          // Move away from center
    //     glColor3f(1.0f, 0.0f, 0.0f);        // Set color to red
    //     drawSquare(0.1);                    // Draw a square
    // }
    // glPopMatrix();

    // Draw a rotating blue triangle
    // glPushMatrix();
    // {
    //     glRotatef(-triangleRotation, 0, 0, 1); // Rotate in opposite direction
    //     glTranslatef(-0.5, 0.5, 0);            // Move to top-left
    //     glColor3f(0.0f, 0.0f, 1.0f);           // Set color to blue
    //     drawTriangle(0.15);                    // Draw a triangle
    // }
    // glPopMatrix();

    // Draw a movable green square (controlled by arrow keys)
    // glPushMatrix();
    // {
    //     glTranslatef(xPosition, yPosition, 0); // Position based on keyboard input
    //     glColor3f(0.0f, 1.0f, 0.0f);           // Set color to green
    //     drawSquare(0.1);                       // Draw a square
    // }
    // glPopMatrix();

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
 * Idle function for continuous animation.
 * This is called when the system is idle, allowing for continuous updates.
 */
void idleFunction()
{
    // Update animation values
    if (animateSquare)
    {
        squareRotation += 0.1f;
        if (squareRotation > 360.0f)
            squareRotation -= 360.0f;
    }

    if (animateTriangle)
    {
        triangleRotation += 0.075f;
        if (triangleRotation > 360.0f)
            triangleRotation -= 360.0f;
    }

    // Request a redisplay
    glutPostRedisplay();
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
    // Update animation values
    if (animateSquare)
    {
        squareRotation += 2.0f;
        if (squareRotation > 360.0f)
            squareRotation -= 360.0f;
    }

    if (animateTriangle)
    {
        triangleRotation += 1.5f;
        if (triangleRotation > 360.0f)
            triangleRotation -= 360.0f;
    }

    // Request a redisplay
    glutPostRedisplay();

    // Register the timer again
    glutTimerFunc(animationSpeed, timerFunction, 0);
}

/**
 * Main function: Entry point of the program.
 */
int main(int argc, char **argv)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            board[i][j] = -1;
            circles[i][j] = false;
            crosses[i][j] = false;
        }
    }
    // Initialize GLUT
    glutInit(&argc, argv);

    // Set initial window size and position
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);

    // Set display mode: RGB color, double buffering, and depth buffering
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    // Create a window with the title
    glutCreateWindow("OpenGL Interactive Demo");

    // Register callback functions
    glutDisplayFunc(display);            // Called when screen needs to be redrawn
    glutReshapeFunc(reshape);            // Called when window is resized
    glutKeyboardFunc(keyboardListener);  // Called on ASCII keyboard input
    glutSpecialFunc(specialKeyListener); // Called on special key input (arrows, etc.)
    glutMouseFunc(mouseListener);        // Called on mouse button presses
    glutMotionFunc(mouseMotionListener); // Called on mouse movement with button pressed

    // Register idle function for continuous animation
    // glutIdleFunc(idleFunction); // Uncomment to use idle function for animation

    // Use timer function instead of idle function for better control of animation speed
    glutTimerFunc(animationSpeed, timerFunction, 0);

    // Perform initial setup
    init();

    // Start the GLUT event loop
    printf("Starting GLUT main loop...\n");
    glutMainLoop();

    return 0;
}