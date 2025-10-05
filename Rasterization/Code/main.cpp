#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <fstream>
#include <vector>
#include <stack>
#include <iomanip>
#include <algorithm>
#include <random>
#include <chrono>

#include "camera.h"
#include "cube.h"
#include "point.h"
#include "vector.h"
#include "color.h"
#include "bitmap_image.hpp"
#include "triangle.h"
#include "matrix.h"
#include "plane.h"
#include "triangle.h"

using namespace std;

string scene_file = "io/scene.txt";
string config_file = "io/config.txt";
string stage1_file = "io/stage1.txt";
string stage2_file = "io/stage2.txt";
string stage3_file = "io/stage3.txt";
string zbuffer_file = "io/z_buffer.txt";
string image_file = "io/out.bmp";

int main(int argc, char *argv[])
{
    Point eye;
    Point look;
    Vector up;
    double fovY;
    double aspectRatio;
    double near;
    double far;

    ifstream sceneFile(scene_file);
    ifstream configFile(config_file);
    if (!configFile.is_open())
    {
        cerr << "Could not open config file!" << endl;
        return -1;
    }
    if (!sceneFile.is_open())
    {
        cerr << "Could not open file!" << endl;
        return -1;
    }

    // Stage 1: Modeling Transformation
    ofstream stage1FileOut(stage1_file);
    stack<Matrix> S;
    stack<int> pushCount;
    Matrix identityMatrix(4, 4);
    identityMatrix = identityMatrix.identity();
    Matrix currentMatrix(4, 4);
    currentMatrix = currentMatrix.identity();
    // identityMatrix.setValue(0, 0, 10);
    S.push(identityMatrix);
    pushCount.push(0);
    double x, y, z;
    sceneFile >> x >> y >> z;
    eye = Point(x, y, z);
    sceneFile >> x >> y >> z;
    look = Point(x, y, z);
    sceneFile >> x >> y >> z;
    up = Vector(x, y, z);
    sceneFile >> fovY;
    sceneFile >> aspectRatio;
    sceneFile >> near;
    sceneFile >> far;
    string command;
    stage1FileOut << fixed << setprecision(7);
    while (sceneFile >> command)
    {
        if (command == "triangle")
        {
            Point p1, p2, p3;
            double x, y, z;
            sceneFile >> x >> y >> z;
            p1 = Point(x, y, z);
            p1 = p1.transform(S.top());
            // p1 = p1.transform(currentMatrix);
            stage1FileOut << p1.x << " " << p1.y << " " << p1.z << endl;
            sceneFile >> x >> y >> z;
            p2 = Point(x, y, z);
            p2 = p2.transform(S.top());
            // p2 = p2.transform(currentMatrix);
            stage1FileOut << p2.x << " " << p2.y << " " << p2.z << endl;
            sceneFile >> x >> y >> z;
            p3 = Point(x, y, z);
            p3 = p3.transform(S.top());
            // p3 = p3.transform(currentMatrix);
            stage1FileOut << p3.x << " " << p3.y << " " << p3.z << endl;
            stage1FileOut << endl;
        }
        else if (command == "translate")
        {
            double tx, ty, tz;
            sceneFile >> tx >> ty >> tz;
            Matrix translationMatrix(4, 4);
            translationMatrix = translationMatrix.identity();
            translationMatrix.setValue(0, 3, tx);
            translationMatrix.setValue(1, 3, ty);
            translationMatrix.setValue(2, 3, tz);
            S.push(S.top() * translationMatrix);
            // currentMatrix = currentMatrix * translationMatrix;
            pushCount.top()++;
        }
        else if (command == "scale")
        {
            double sx, sy, sz;
            sceneFile >> sx >> sy >> sz;
            Matrix scaleMatrix(4, 4);
            scaleMatrix = scaleMatrix.identity();
            scaleMatrix.setValue(0, 0, sx);
            scaleMatrix.setValue(1, 1, sy);
            scaleMatrix.setValue(2, 2, sz);
            S.push(S.top() * scaleMatrix);
            // currentMatrix = currentMatrix * scaleMatrix;
            pushCount.top()++;
        }
        else if (command == "rotate")
        {
            double angle, ax, ay, az;
            sceneFile >> angle >> ax >> ay >> az;
            angle = angle * M_PI / 180.0;
            Vector a(ax, ay, az);
            a = a.normalize();
            Vector i = Vector(1, 0, 0);
            Vector j = Vector(0, 1, 0);
            Vector k = Vector(0, 0, 1);
            Vector c[3];
            c[0] = i.rotate(a, angle);
            c[1] = j.rotate(a, angle);
            c[2] = k.rotate(a, angle);
            Matrix rotationMatrix(4, 4);
            rotationMatrix = rotationMatrix.identity();
            rotationMatrix.setValue(0, 0, c[0].x);
            rotationMatrix.setValue(0, 1, c[1].x);
            rotationMatrix.setValue(0, 2, c[2].x);
            rotationMatrix.setValue(1, 0, c[0].y);
            rotationMatrix.setValue(1, 1, c[1].y);
            rotationMatrix.setValue(1, 2, c[2].y);
            rotationMatrix.setValue(2, 0, c[0].z);
            rotationMatrix.setValue(2, 1, c[1].z);
            rotationMatrix.setValue(2, 2, c[2].z);
            S.push(S.top() * rotationMatrix);
            // currentMatrix = currentMatrix * rotationMatrix;
            pushCount.top()++;
        }
        else if (command == "push")
        {
            // S.push(currentMatrix);
            pushCount.push(0);
        }
        else if (command == "pop")
        {
            for (int i = 0; i < pushCount.top(); i++)
            {
                S.pop();
            }
            pushCount.pop();
            // currentMatrix = S.top();
            // S.pop();
        }
        else if (command == "end")
        {
            break;
        }
        else if (command == "end")
        {
            break;
        }
        else
        {
            cerr << "Unknown command: " << command << endl;
            return -1;
        }
    }
    stage1FileOut.close();
    sceneFile.close();

    // Stage 2: View Transformation
    ifstream stage1FileIn(stage1_file);
    ofstream stage2FileOut(stage2_file);
    Vector l = look - eye;
    l = l.normalize();
    Vector r = l.cross(up);
    r = r.normalize();
    Vector u = r.cross(l);
    Matrix translationMatrix(4, 4);
    translationMatrix = translationMatrix.identity();
    translationMatrix.setValue(0, 3, -eye.x);
    translationMatrix.setValue(1, 3, -eye.y);
    translationMatrix.setValue(2, 3, -eye.z);
    Matrix rotationMatrix(4, 4);
    rotationMatrix = rotationMatrix.identity();
    rotationMatrix.setValue(0, 0, r.x);
    rotationMatrix.setValue(0, 1, r.y);
    rotationMatrix.setValue(0, 2, r.z);
    rotationMatrix.setValue(1, 0, u.x);
    rotationMatrix.setValue(1, 1, u.y);
    rotationMatrix.setValue(1, 2, u.z);
    rotationMatrix.setValue(2, 0, -l.x);
    rotationMatrix.setValue(2, 1, -l.y);
    rotationMatrix.setValue(2, 2, -l.z);
    Matrix viewMatrix(4, 4);
    viewMatrix = rotationMatrix * translationMatrix;
    stage2FileOut << fixed << setprecision(7);
    int i = 0;
    while (stage1FileIn >> x >> y >> z)
    {
        Point p(x, y, z);
        p = p.transform(viewMatrix);
        stage2FileOut << p.x << " " << p.y << " " << p.z << endl;
        if (i % 3 == 2)
        {
            stage2FileOut << endl;
        }
        i++;
    }
    stage1FileIn.close();
    stage2FileOut.close();

    // Stage 3: Projection Transformation
    ifstream stage2FileIn(stage2_file);
    ofstream stage3FileOut(stage3_file);
    double fovX = fovY * aspectRatio;
    fovX = fovX * M_PI / 180.0;
    fovY = fovY * M_PI / 180.0;
    double t = near * tan(fovY / 2);
    double r2 = near * tan(fovX / 2);
    Matrix projectionMatrix(4, 4);
    projectionMatrix = projectionMatrix.identity();
    projectionMatrix.setValue(0, 0, near / r2);
    projectionMatrix.setValue(1, 1, near / t);
    projectionMatrix.setValue(2, 2, -(far + near) / (far - near));
    projectionMatrix.setValue(2, 3, -(2 * far * near) / (far - near));
    projectionMatrix.setValue(3, 2, -1);
    projectionMatrix.setValue(3, 3, 0);
    stage3FileOut << fixed << setprecision(7);
    i = 0;
    while (stage2FileIn >> x >> y >> z)
    {
        Point p(x, y, z);
        p = p.transform(projectionMatrix);
        stage3FileOut << p.x << " " << p.y << " " << p.z << endl;
        if (i % 3 == 2)
        {
            stage3FileOut << endl;
        }
        i++;
    }
    stage2FileIn.close();
    stage3FileOut.close();

    // Stage 4: Clipping & scan conversion using Z-buffer algorithm
    ifstream stage3FileIn(stage3_file);
    ofstream zbufferFileOut(zbuffer_file);
    ifstream configFileIn(config_file);
    int width, height;
    configFileIn >> width >> height;
    double xleft, xright, ytop, ybottom;
    configFileIn >> xleft;
    xright = xleft * (-1);
    configFileIn >> ybottom;
    ytop = ybottom * (-1);
    double zFront, zRear;
    configFileIn >> zFront >> zRear;
    vector<Triangle> triangles;
    srand(time(0));
    while (stage3FileIn >> x >> y >> z)
    {
        Point p1(x, y, z);
        stage3FileIn >> x >> y >> z;
        Point p2(x, y, z);
        stage3FileIn >> x >> y >> z;
        Point p3(x, y, z);
        Triangle t(p1, p2, p3);
        t.setColor(Color(rand() % 256, rand() % 256, rand() % 256));
        triangles.push_back(t);
    }
    // unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    // std::shuffle(triangles.begin(), triangles.end(), std::default_random_engine(seed));
    double dx = (xright - xleft) / width;
    double dy = (ytop - ybottom) / height;
    double topY = ytop - (dy / 2.0);
    double leftX = xleft + (dx / 2.0);
    // double bottomY = ybottom + dy / 2.0;
    // double rightX = xright - dx / 2;
    double z_max = 2.0;
    double **zBuffer = new double *[height];
    for (int i = 0; i < height; i++)
    {
        zBuffer[i] = new double[width];
        for (int j = 0; j < width; j++)
        {
            zBuffer[i][j] = z_max;
        }
    }
    bitmap_image image(width, height);
    image.set_all_channels(0, 0, 0);
    for (int i = 0; i < triangles.size(); i++)
    {
        Triangle t = triangles[i];
        t.reorderVerticesByY();
        Point p1 = t.getP1();
        Point p2 = t.getP2();
        Point p3 = t.getP3();
        Plane trianglePlane(p1, p2, p3);
        int top_scanline = round((topY - max(p1.y, max(p2.y, p3.y))) / dy);
        // cout << "top_scanline: " << top_scanline << endl;
        top_scanline = max(0, top_scanline);
        int bottom_scanline = round((topY - min(p1.y, min(p2.y, p3.y))) / dy);
        // int bottom_scanline = height - 1 - round(min(p1.y, min(p2.y, p3.y)) - bottomY / dy);
        // cout << "bottom_scanline: " << bottom_scanline << endl;
        bottom_scanline = min(height - 1, bottom_scanline);
        // double y_scanline = top_scanline;
        // while (y_scanline <= bottom_scanline)
        for (int row_no = top_scanline; row_no <= bottom_scanline; row_no++)
        {
            int left_intersecting_column = round((min(p1.x, min(p2.x, p3.x)) - leftX) / dx);
            // cout << "left_intersecting_column: " << left_intersecting_column << endl;
            left_intersecting_column = max(0, left_intersecting_column);
            int right_intersecting_column = round((max(p1.x, max(p2.x, p3.x)) - leftX) / dx);
            // int right_intersecting_column = width - 1 - round((rightX - max(p1.x, max(p2.x, p3.x))) / dx);
            // cout << "right_intersecting_column: " << right_intersecting_column << endl;
            right_intersecting_column = min(width - 1, right_intersecting_column);
            double y = topY - row_no * dy;
            for (int col_no = left_intersecting_column; col_no <= right_intersecting_column; col_no++)
            {
                double x = leftX + col_no * dx;
                if (t.insideTriangle(Point(x, y, 0.0)))
                {
                    double z = trianglePlane.findZ(x, y);
                    if (z < zBuffer[row_no][col_no] && z >= zFront && z <= zRear)
                    {
                        zBuffer[row_no][col_no] = z;
                        Color c = t.getColor();
                        image.set_pixel(col_no, row_no, c.r, c.g, c.b);
                    }
                }
            }
        }
        // image.save_image("io/out" + to_string(i) + ".bmp");
    }
    image.save_image(image_file);
    zbufferFileOut << fixed << setprecision(6);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (zBuffer[i][j] < z_max)
                zbufferFileOut << zBuffer[i][j] << "\t";
            // else
            //     zbufferFileOut << "\t";
        }
        zbufferFileOut << "\n";
    }

    for (int i = 0; i < height; i++)
        delete[] zBuffer[i];
    delete[] zBuffer;

    stage3FileIn.close();
    zbufferFileOut.close();
    configFileIn.close();

    return 0;
}
