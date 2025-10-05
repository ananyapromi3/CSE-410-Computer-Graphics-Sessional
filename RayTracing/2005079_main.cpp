#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h> // Use GLUT framework on macOS
#else
#include <GL/glut.h> // Use standard GLUT location on Linux/Windows
#endif
#include <iostream>
#include <thread>
#include <vector>
#include "2005079_classes.hpp"
#include "bitmap_image.hpp"
#include <mutex>
std::mutex imageMutex;

using namespace std;

double viewAngle = 80.0;
int level = 0;
double zNear = 1.0, zFar = 500.0;
int capturedFrames = 0;
string inputFilename = "input.txt";
string textureFilename = "texture3.bmp";
Camera camera(Point(125.0, -125.0, 125.0), Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0), 1.0, 0.05);
vector<Object *> objects;
vector<PointLight *> pointLights;
vector<SpotLight *> spotLights;
int windowWidth = 1000, windowHeight = 1000;
int imageWidth = 1000, imageHeight = 1000;

void initGL();
void reshapeListener(GLsizei width, GLsizei height);
void load_data(const string &filename);
void capture();
void free_memory();

void initGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
}

void reshapeListener(GLsizei width, GLsizei height)
{
    if (height == 0)
        height = 1;
    GLfloat aspect = (GLfloat)width / (GLfloat)height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(viewAngle, aspect, zNear, zFar);
}

void load_data(const string &filename)
{
    ifstream input(filename);
    if (!input.is_open())
    {
        cerr << "Error: File not found" << endl;
        return;
    }

    bitmap_image texImage(textureFilename);
    if (!texImage)
        cerr << "Texture loading failed\n";

    unsigned char *texData = new unsigned char[texImage.width() * texImage.height() * 3];
    for (int y = 0; y < texImage.height(); y++)
    {
        for (int x = 0; x < texImage.width(); x++)
        {
            rgb_t color;
            texImage.get_pixel(x, y, color);
            int index = (y * texImage.width() + x) * 3;
            texData[index] = color.red;
            texData[index + 1] = color.green;
            texData[index + 2] = color.blue;
        }
    }
    // cout << "Texture loaded successfully: " << texImage.width() << "x" << texImage.height() << endl;
    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texImage.width(), texImage.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
    // cout << "Texture OpenGL ID: " << texID << endl;
    // Window setup
    int pixel;
    input >> level >> pixel;
    // Objects
    int numObjects;
    input >> numObjects;
    for (int i = 0; i < numObjects; i++)
    {
        string type;
        input >> type;
        if (type == "sphere")
        {
            double x, y, z, radius;
            input >> x >> y >> z >> radius;
            Point center(x, y, z);
            double r, g, b;
            input >> r >> g >> b;
            Color color(r, g, b);
            // cout << "DEBUG: Sphere color read: " << r << ", " << g << ", " << b << endl;
            // color.print();
            double ambient, diffuse, specular, reflectionCoefficient;
            input >> ambient >> diffuse >> specular >> reflectionCoefficient;
            int shine;
            input >> shine;
            Object *temp = new Sphere(center, radius);
            temp->setColor(color);
            // cout << "DEBUG: After setColor, sphere color: ";
            // temp->color.print();
            temp->setCoefficients(ambient, diffuse, specular, reflectionCoefficient, shine);
            objects.push_back(temp);
        }
        else if (type == "triangle")
        {
            double x1, y1, z1, x2, y2, z2, x3, y3, z3;
            input >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3;
            Point p1(x1, y1, z1), p2(x2, y2, z2), p3(x3, y3, z3);
            double r, g, b;
            input >> r >> g >> b;
            Color color(r, g, b);
            // cout << "DEBUG: Triangle color read: " << r << ", " << g << ", " << b << endl;
            // color.print();
            double ambient, diffuse, specular, reflection;
            input >> ambient >> diffuse >> specular >> reflection;
            int shine;
            input >> shine;
            Object *temp = new Triangle(p1, p2, p3);
            temp->setColor(color);
            // cout << "DEBUG: After setColor, triangle color: ";
            // temp->color.print();
            temp->setCoefficients(ambient, diffuse, specular, reflection, shine);
            objects.push_back(temp);
        }
        else if (type == "general")
        {
            double A, B, C, D, E, F, G, H, I, J;
            input >> A >> B >> C >> D >> E >> F >> G >> H >> I >> J;
            double x, y, z;
            input >> x >> y >> z;
            double length, width, height;
            input >> length >> width >> height;
            double r, g, b;
            input >> r >> g >> b;
            Color color(r, g, b);
            double ambient, diffuse, specular, reflection;
            input >> ambient >> diffuse >> specular >> reflection;
            int shine;
            input >> shine;
            Point reference(x, y, z);
            Object *temp = new QuadraticSurface(reference, height, width, length,
                                                A, B, C, D, E, F, G, H, I, J);
            temp->setColor(color);
            temp->setCoefficients(ambient, diffuse, specular, reflection, shine);
            objects.push_back(temp);
        }
        else
        {
            cerr << "Error reading file: Unknown object type" << endl;
            return;
        }
        // cout << "Object " << objects.size() << " loaded successfully." << endl;
    }
    // Floor
    Object *floor = new Floor(1000, 20);
    floor->setCoefficients(0.4, 0.2, 0.2, 0.2, 1.0);
    // floor->setCoefficients(0.4, 0.2, 0.2, 0.2, 5);
    // floor->setReferencePoint(camera.center);
    static_cast<Floor *>(floor)->setTexture(texData, texImage.width(), texImage.height(), 3);
    static_cast<Floor *>(floor)->setGLTextureID(texID);
    objects.push_back(floor);
    // Point Lights
    int numPointLights;
    input >> numPointLights;
    for (int i = 0; i < numPointLights; i++)
    {
        double x, y, z;
        input >> x >> y >> z;
        Point position(x, y, z);
        double r, g, b;
        input >> r >> g >> b;
        Color color(r, g, b);
        PointLight *pl = new PointLight(position, color);
        pointLights.push_back(pl);
    }
    // Spot Lights
    int numSpotLights;
    input >> numSpotLights;
    for (int i = 0; i < numSpotLights; i++)
    {
        double x, y, z;
        input >> x >> y >> z;
        Point position(x, y, z);
        double r, g, b;
        input >> r >> g >> b;
        Color color(r, g, b);
        input >> x >> y >> z;
        Vector direction(x, y, z);
        double angle;
        input >> angle;
        SpotLight *sl = new SpotLight(position, direction, angle, color);
        spotLights.push_back(sl);
    }
    input.close();
    cout << "Total objects loaded: " << objects.size() << endl;
    cout << "Total point lights loaded: " << pointLights.size() << endl;
    cout << "Total spot lights loaded: " << spotLights.size() << endl;
    cout << "Data loaded successfully from " << filename << endl;
    cout << "Press '0' to capture the image." << endl;
    // for (Object *obj : objects)
    // {
    //     obj->color.print();
    // }
}

void capture()
{
    cout << "Capturing image..." << endl;
    auto start = std::chrono::steady_clock::now();
    bitmap_image image(imageWidth, imageHeight);
    image.set_all_channels(0, 0, 0);
    double planeDistance = (windowHeight / 2.0) / tan((viewAngle * M_PI / 360) / 2.0);
    // double planeDistance = 1.0;
    // double windowHeight = 2 * tan(viewAngle * M_PI / 360.0) * planeDistance;
    // double windowWidth = windowHeight;
    Vector r = camera.right().normalize();
    Point topLeft = camera.eye + (camera.center - camera.eye).normalize() * planeDistance -
                    r * windowWidth / 2.0 + camera.up.normalize() * windowHeight / 2.0;
    double du = windowWidth / imageWidth;
    double dv = windowHeight / imageHeight;
    topLeft = topLeft + r * 0.5 * du - camera.up * 0.5 * dv;
    auto renderSegment = [&](int startCol, int endCol)
    {
        bitmap_image localImage(imageWidth, imageHeight);
        localImage.set_all_channels(0, 0, 0);
        for (int i = startCol; i < endCol; i++)
        {
            for (int j = 0; j < imageHeight; j++)
            {
                Point curPixel = topLeft + r * i * du - camera.up * j * dv;
                Ray ray(camera.eye, (curPixel - camera.eye).normalize());
                int nearest = -1;
                double tMin = 1e9;
                for (int k = 0; k < objects.size(); k++)
                {
                    double t = objects[k]->intersect(ray);
                    if (t > 0 && t < tMin)
                    {
                        tMin = t;
                        nearest = k;
                    }
                }
                if (nearest == -1)
                    continue;
                double dist = (camera.center - camera.eye).normalize().dot(ray.direction * tMin);
                if (dist > zFar)
                    continue;
                Color color(0, 0, 0);
                objects[nearest]->traceRay(ray, color, level, pointLights, spotLights);
                color.clamp();
                localImage.set_pixel(i, j, 255 * color.r, 255 * color.g, 255 * color.b);
            }
        }
        std::lock_guard<std::mutex> guard(imageMutex);
        for (int i = startCol; i < endCol; i++)
        {
            for (int j = 0; j < imageHeight; j++)
            {
                unsigned char r, g, b;
                localImage.get_pixel(i, j, r, g, b);
                image.set_pixel(i, j, r, g, b);
            }
        }
    };
    int numThreads = std::thread::hardware_concurrency();
    // cout << "Using " << numThreads << " threads\n";
    int colsPerThread = imageWidth / numThreads;
    vector<thread> threads;
    for (int i = 0; i < numThreads; ++i)
    {
        int startCol = i * colsPerThread;
        int endCol = (i == numThreads - 1) ? imageWidth : (i + 1) * colsPerThread;
        threads.emplace_back(renderSegment, startCol, endCol);
    }
    for (auto &t : threads)
        t.join();
    string output_file = "Output_" + to_string(++capturedFrames) + ".bmp";
    image.save_image(output_file);
    auto end = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    cout << "Captured to " << output_file << " in " << (ms / 1000.0) << " seconds" << endl;
}

// void capture()
// {
//     bitmap_image image(imageWidth, imageHeight);
//     image.set_all_channels(0, 0, 0);
//     double planeDistance = (windowHeight / 2.0) / tan((viewAngle * M_PI / 360) / 2.0);
//     // double planeDistance = 1.0;
//     // double windowHeight = 2 * tan(viewAngle * M_PI / 360.0) * planeDistance;
//     // double windowWidth = windowHeight;
//     Point topLeft = camera.eye + (camera.center - camera.eye).normalize() * planeDistance -
//                     camera.right() * windowWidth / 2.0 + camera.up * windowHeight / 2.0;
//     double du = windowWidth / imageWidth;
//     double dv = windowHeight / imageHeight;
//     topLeft = topLeft + camera.right() * 0.5 * du - camera.up * 0.5 * dv;
//     int nearest;
//     double t, tMin;
//     for (int i = 0; i < imageWidth; i++)
//     {
//         for (int j = 0; j < imageHeight; j++)
//         {
//             Point curPixel = topLeft + camera.right() * i * du - camera.up * j * dv;
//             Ray ray(camera.eye, (curPixel - camera.eye).normalize());
//             // cout << "Ray: " << i << ", " << j << endl;
//             nearest = -1;
//             double tMin = 1e9;
//             for (int k = 0; k < objects.size(); k++)
//             {
//                 // cout << "Checking object " << k << endl;
//                 Object *o = objects[k];
//                 // o->color.print();
//                 double t = o->intersect(ray);
//                 // cout << "t: " << t << endl;
//                 if (t > 0 && t < tMin)
//                 {
//                     tMin = t;
//                     nearest = k;
//                 }
//             }
//             // cout << "nearest: " << nearest << endl;
//             if (nearest == -1)
//                 continue;
//             double dist = (camera.center - camera.eye).normalize().dot(ray.direction.normalize() * tMin);
//             // cout << "dist: " << dist << endl;
//             if (dist > zFar)
//                 continue;
//             Color color(0.0, 0.0, 0.0);
//             // cout << "here";
//             objects[nearest]->traceRay(ray, color, level, pointLights, spotLights);
//             // color.print();
//             color.clamp();
//             // color.print();
//             image.set_pixel(i, j, 255 * color.r, 255 * color.g, 255 * color.b);
//         }
//     }
//     string output_file = "Output_" + to_string(++capturedFrames) + ".bmp";
//     image.save_image(output_file);
//     cout << "Captured frame " << capturedFrames << " saved as " << output_file << endl;
// }

void free_memory()
{
    for (Object *object : objects)
        delete object;
    objects.clear();
    for (PointLight *pl : pointLights)
        delete pl;
    pointLights.clear();
    for (SpotLight *sl : spotLights)
        delete sl;
    spotLights.clear();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // gluLookAt(camera.eye.x, camera.eye.y, camera.eye.z,                                                       // Camera position
    //           camera.center.x - camera.eye.x, camera.center.y - camera.eye.y, camera.center.z - camera.eye.z, // Look-at point
    //           camera.up.x, camera.up.y, camera.up.z);                                                         // Up vector
    gluLookAt(camera.eye.x, camera.eye.y, camera.eye.z,
              camera.center.x, camera.center.y, camera.center.z,
              camera.up.x, camera.up.y, camera.up.z);
    // draw_axes();
    for (Object *obj : objects)
        obj->draw();
    for (PointLight *pl : pointLights)
        pl->draw();
    for (SpotLight *sl : spotLights)
        sl->draw();
    glutSwapBuffers();
}

void idle() { glutPostRedisplay(); }

void keyboardListener(unsigned char key, int x, int y)
{
    switch (key)
    {
    case '0':
        capture();
        break;
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
    case 'p':
        cout << "Camera Position: (" << camera.eye.x << ", "
             << camera.eye.y << ", " << camera.eye.z << ")" << endl;
        break;
    case 't':
        static_cast<Floor *>(objects.back())->useTexture = !static_cast<Floor *>(objects.back())->useTexture;
        cout << "Toggled floor texture: " << (static_cast<Floor *>(objects.back())->useTexture ? "ON" : "OFF") << endl;
        break;
    case 27:
        exit(0);
        break;
    default:
        printf("Unknown key pressed\n");
        break;
    }
    glutPostRedisplay();
}

void specialKeyListener(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        camera.moveForward();
        break;
    case GLUT_KEY_DOWN:
        camera.moveBackward();
        break;
    case GLUT_KEY_LEFT:
        camera.moveLeft();
        break;
    case GLUT_KEY_RIGHT:
        camera.moveRight();
        break;
    case GLUT_KEY_PAGE_UP:
        camera.moveUp();
        break;
    case GLUT_KEY_PAGE_DOWN:
        camera.moveDown();
        break;
    default:
        printf("Unknown key pressed\n");
        break;
    }
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(50, 50);
    // glEnable(GLUT_MULTISAMPLE);
    // glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutCreateWindow("Ray Tracing");
    initGL();
    load_data(inputFilename);
    // glutDisplayFunc(display);
    // glutKeyboardFunc(handle_keys);
    // glutSpecialFunc(handle_special_keys);
    glutDisplayFunc(display);
    glutReshapeFunc(reshapeListener);
    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutIdleFunc(idle);
    // glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
    // Note: glutSetOption may not be available in all GLUT implementations.
    // glutCloseFunc(free_memory);
    glutMainLoop();
    free_memory();
    return 0;
}