#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h> // Use GLUT framework on macOS
#else
#include <GL/glut.h> // Use standard GLUT location on Linux/Windows
#endif
#include <iostream>
#include <stdexcept>
#include <cmath>
#include "2005079_classes.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                  Point                                                         //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Vector Point::operator-(const Point &p) const { return Vector(x - p.x, y - p.y, z - p.z); }
Point Point::operator+(const Vector &v) const { return Point(x + v.x, y + v.y, z + v.z); }
Point Point::operator-(const Vector &v) const { return Point(x - v.x, y - v.y, z - v.z); }
Point Point::operator+=(const Vector &v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}
Point Point::operator-=(const Vector &v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}
double Point::distance(const Point &p) const
{
    return sqrt((x - p.x) * (x - p.x) +
                (y - p.y) * (y - p.y) +
                (z - p.z) * (z - p.z));
}
void Point::print() const { cout << "Point(" << x << ", " << y << ", " << z << ")" << endl; }
Vector Point::toVector() const { return Vector(x, y, z); }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                  Vector                                                        //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Vector Vector::operator=(const Vector &v)
{
    if (this != &v)
    {
        x = v.x;
        y = v.y;
        z = v.z;
    }
    return *this;
}
Vector Vector::operator+(const Vector &v) const { return Vector(x + v.x, y + v.y, z + v.z); }
Vector Vector::operator-(const Vector &v) const { return Vector(x - v.x, y - v.y, z - v.z); }
Vector Vector::operator*(const double &d) const { return Vector(x * d, y * d, z * d); }
Vector Vector::operator/(const double &d) const
{
    if (d == 0)
        throw invalid_argument("Division by zero is not allowed.");
    return Vector(x / d, y / d, z / d);
}
Vector Vector::operator+=(const Vector &v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}
Vector Vector::operator+=(const double &d)
{
    x += d;
    y += d;
    z += d;
    return *this;
}
Vector Vector::operator-=(const double &d)
{
    x -= d;
    y -= d;
    z -= d;
    return *this;
}
Vector Vector::operator-=(const Vector &v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}
Vector Vector::operator*=(const double &d)
{
    x *= d;
    y *= d;
    z *= d;
    return *this;
}
Vector Vector::operator/=(const double &d)
{
    if (d == 0)
        throw invalid_argument("Division by zero is not allowed.");
    x /= d;
    y /= d;
    z /= d;
    return *this;
}
double Vector::dot(const Vector &v) const { return x * v.x + y * v.y + z * v.z; }
Vector Vector::cross(const Vector &v) const
{
    return Vector(
        y * v.z - z * v.y,
        z * v.x - x * v.z,
        x * v.y - y * v.x);
}
Vector Vector::normalize() const
{
    double len = norm();
    if (len == 0)
        return Vector(0, 0, 0);
    return Vector(x / len, y / len, z / len);
}
Vector Vector::rotate(const Vector &axis, double angle) const
{
    double cosTheta = cos(angle);
    double sinTheta = sin(angle);
    Vector normalizedAxis = axis.normalize();
    return *this * cosTheta +
           (normalizedAxis.cross(*this)) * sinTheta +
           normalizedAxis * (normalizedAxis.dot(*this)) * (1 - cosTheta);
}
Vector Vector::rotateAroundAxis(const Vector &axis, double angle)
{
    double cosTheta = cos(angle);
    double sinTheta = sin(angle);
    Vector normalizedAxis = axis.normalize();
    *this = *this * cosTheta +
            (normalizedAxis.cross(*this)) * sinTheta +
            normalizedAxis * (normalizedAxis.dot(*this)) * (1 - cosTheta);
    return *this;
}
double Vector::norm() const { return sqrt(x * x + y * y + z * z); }
Vector Vector::reflect(const Plane &plane) const
{
    Vector normal = plane.normal.normalize();
    return *this - normal * (2 * this->dot(normal));
}
Vector Vector::reflect(const Vector &normal) const { return *this - normal * (2 * this->dot(normal)); }
void Vector::print() const { printf("Vector: (%.2f, %.2f, %.2f)\n", x, y, z); }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                  Plane                                                         //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Plane::Plane(Point p1, Point p2, Point p3)
{
    Vector v1 = p2 - p1;
    Vector v2 = p3 - p1;
    normal = v1.cross(v2);
    point = p1;
}
float Plane::distanceToPoint(const Point &p) const { return normal.dot(p - point) / normal.norm(); }
void Plane::print() const
{
    printf("Plane normal: (%.2f, %.2f, %.2f), point: (%.2f, %.2f, %.2f)\n",
           normal.x, normal.y, normal.z,
           point.x, point.y, point.z);
}
double Plane::findZ(double x, double y) const
{
    Vector n = normal.normalize();
    double A = n.x;
    double B = n.y;
    double C = n.z;
    if (fabs(C) == 0.0)
        return 1e8;
    double D = -(A * point.x + B * point.y + C * point.z);
    return -(A * x + B * y + D) / C;
}
double Plane::intersect(const Ray &r) const
{
    double denom = normal.dot(r.direction);
    if (fabs(denom) < 1e-6)
        return -1.0;                     // Ray is parallel to plane
    Vector o_minus_p = r.origin - point; // point on plane
    double t = -normal.dot(o_minus_p) / denom;
    if (t < 0)
        return -1.0;
    return t;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                  Camera                                                        //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Camera::setMoveSpeed(double speed) { moveSpeed = speed; }
void Camera::setRotateSpeed(double speed) { rotateSpeed = speed; }
Vector Camera::right()
{
    // return (center - eye).cross(up).normalize();
    Vector look = (center - eye).normalize();
    if (fabs(look.dot(up)) <= 1e-6)
    {
        // up = up.normalize();
        return look.cross(up).normalize();
    }
    else
    {
        Vector right = Vector(look.y, -look.x, 0).normalize();
        // Vector up = right.cross(look).normalize();
        // setView(eye, center, up);
        return right;
    }
}
void Camera::move(const Vector &v)
{
    eye = eye + v;
    center = center + v;
}
void Camera::rotate(const Vector &axis, double angle)
{
    Vector direction = (center - eye);
    direction = direction.rotate(axis, angle);
    center = eye + direction;
}
void Camera::setView(Point eye, Point center, Vector up)
{
    this->eye = eye;
    this->center = center;
    this->up = up;
}
void Camera::moveForward() { eye += (center - eye).normalize() * moveSpeed; }
void Camera::moveBackward() { eye -= (center - eye).normalize() * moveSpeed; }
void Camera::moveLeft() { eye -= right() * moveSpeed; }
void Camera::moveRight() { eye += right() * moveSpeed; }
void Camera::moveUp() { eye += up.normalize() * moveSpeed; }
void Camera::moveDown() { eye -= up.normalize() * moveSpeed; }
void Camera::lookLeft()
{
    // Vector direction = (center - eye).normalize();
    // direction = direction.rotateAroundAxis(up, rotateSpeed);
    // center = eye + direction;
    Vector look = (center - eye).rotate(up, rotateSpeed);
    center = eye + look;
    up = up.rotateAroundAxis(right(), rotateSpeed).normalize();
}
void Camera::lookRight()
{
    // Vector direction = (center - eye).normalize();
    // direction = direction.rotateAroundAxis(up, -rotateSpeed);
    // center = eye + direction;
    Vector look = (center - eye).rotate(up, -rotateSpeed);
    center = eye + look;
    up = up.rotateAroundAxis(right(), -rotateSpeed).normalize();
}
void Camera::lookUp()
{
    // Vector direction = (center - eye).normalize();
    // Vector right = up.cross(direction);
    // direction = direction.rotateAroundAxis(right, -rotateSpeed);
    // center = eye + direction;
    // up = up.rotateAroundAxis(right, rotateSpeed);
    Vector r = right();
    Vector look = (center - eye).rotate(r, rotateSpeed);
    center = eye + look;
    up = up.rotateAroundAxis(r, rotateSpeed).normalize();
}
void Camera::lookDown()
{
    // Vector direction = (center - eye).normalize();
    // Vector right = up.cross(direction);
    // direction = direction.rotateAroundAxis(right, rotateSpeed);
    // center = eye + direction;
    // up = up.rotateAroundAxis(right, -rotateSpeed);
    Vector r = right();
    Vector look = (center - eye).rotate(r, -rotateSpeed);
    center = eye + look;
    up = up.rotateAroundAxis(r, -rotateSpeed).normalize();
}
void Camera::tiltClockwise()
{
    Vector look = (center - eye).normalize();
    up = up.rotateAroundAxis(look, rotateSpeed).normalize();
}
void Camera::tiltAntiClockwise()
{
    Vector look = (center - eye).normalize();
    up = up.rotateAroundAxis(look, -rotateSpeed).normalize();
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                  Color                                                         //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Color Color::operator+(const Color &c) const { return Color(r + c.r, g + c.g, b + c.b); }
Color Color::operator*(const double &d) const { return Color(r * d, g * d, b * d); }
Color Color::operator*(const Color &c) const { return Color(r * c.r, g * c.g, b * c.b); }
void Color::clamp()
{
    r = max(0.0, min(1.0, r));
    g = max(0.0, min(1.0, g));
    b = max(0.0, min(1.0, b));
}
void Color::print() const { cout << "Color(" << r << ", " << g << ", " << b << ")\n"; }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                  Object                                                        //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Object::setColor(const Color &c) { color = c; }
void Object::setCoefficients(double ambient, double diffuse, double specular, double reflectionCoefficient,
                             int shine)
{
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->reflectionCoefficient = reflectionCoefficient;
    this->shine = shine;
}
// void Object::traceRay(const Ray &r, Color &color, int level) const
void Object::traceRay(const Ray &r, Color &c, int level, const vector<PointLight *> &pointLights,
                      const vector<SpotLight *> &spotLights) const
{
    // std::cout << "Spotlights: " << spotLights.size() << ", PointLights: " << pointLights.size() << std::endl;
    double t = intersect(r);
    // Use a minimum threshold for intersection distance to avoid numerical issues
    if (level == 0 || t < 1e-6)
        return;
    Point intersection = r.origin + r.direction * t;
    // cout << "Intersection Point: ";
    // intersection.print();
    Color localColor = getColor(intersection);
    // localColor.print();
    // this->color.print();
    // cout << "Ambient: " << ambient << "\n";
    c = localColor * ambient;
    // color.print();
    Vector normal = getNormal(intersection);
    if (r.direction.dot(normal) > 0)
        normal = normal * (-1);
    // cout << "here...\n";
    for (SpotLight *s : spotLights)
    {
        Ray lightRay(s->position, intersection - s->position);
        double beta;
        double dot = lightRay.direction.dot(s->direction);
        double angle = acos(dot / (lightRay.direction.norm() * s->direction.norm())) * 180.0 / M_PI;
        beta = fabs(angle * M_PI / 180);
        if (fabs(angle) >= s->cutoffAngle)
            continue;
        double t_cur = (intersection - s->position).norm();
        if (t_cur < 1e-6)
            continue;
        bool obscured = false;
        for (Object *obj : objects)
        {
            double t = obj->intersect(lightRay);
            if (t > 1e-6 && t + 1e-6 < t_cur)
            {
                obscured = true;
                break;
            }
        }
        if (obscured)
            continue;
        double lambert_value = max(0.0, normal.dot(lightRay.direction * (-1)));
        if (lambert_value < 1e-6)
            continue;
        double epsilon = 2;
        c = c + s->color * diffuse * lambert_value * localColor * pow(cos(beta), epsilon);
        Ray reflected_ray(intersection, lightRay.direction.reflect(normal));
        double phongVal = max(0.0, reflected_ray.direction.dot(r.direction * (-1)));
        c = c + s->color * specular * pow(phongVal, shine) * localColor * pow(cos(beta), epsilon);
    }
    for (Light *p : pointLights)
    {
        Ray lightRay(p->position, (intersection - p->position).normalize());
        // lightRay.origin.print();
        // lightRay.direction.print();
        double t_cur = (intersection - p->position).norm();
        if (t_cur < 1e-6)
            continue;
        bool obscured = false;
        for (Object *obj : objects)
        {
            double t = obj->intersect(lightRay);
            if (t > 1e-6 && t + 1e-6 < t_cur)
            {
                obscured = true;
                break;
            }
        }
        if (obscured)
            continue;
        double lambert_value = max(0.0, normal.dot(lightRay.direction * (-1)));
        if (lambert_value < 1e-6)
            continue;
        c = c + p->color * diffuse * lambert_value * localColor;
        Ray reflected_ray(intersection, lightRay.direction.reflect(normal));
        double phongVal = max(0.0, reflected_ray.direction.dot(r.direction * (-1)));
        c = c + p->color * specular * pow(phongVal, shine) * localColor;
    }
    if (level == 0)
        return;
    Ray reflectedRay(intersection, r.direction.reflect(normal));
    // reflectedRay.origin.print();
    // reflectedRay.direction.print();
    reflectedRay.origin += reflectedRay.direction * 1e-6; // Offset to avoid self-intersection
    Object *nextObject = nextReflectionObject(reflectedRay);
    if (nextObject == nullptr)
        return;
    // else
    //     nextObject->color.print();
    Color reflectedColor(0.0, 0.0, 0.0);
    nextObject->traceRay(reflectedRay, reflectedColor, level - 1, pointLights, spotLights);
    c = c + reflectedColor * reflectionCoefficient;
    return;
}
Color Object::getColor(const Point &p) const { return this->color; }
Object *Object::nextReflectionObject(const Ray &r) const
{
    int nearest = -1;
    double tMin = 1e9;
    for (int i = 0; i < objects.size(); i++)
    {
        Object *o = objects[i];
        double t = o->intersect(r);
        if (t > 0 && t < tMin)
        {
            tMin = t;
            nearest = i;
        }
    }
    if (nearest == -1)
        return nullptr;
    return objects[nearest];
}
void Object::setReferencePoint(const Point &p) { referencePoint = p; }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                Triangle                                                        //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Triangle::draw() const
{
    // cout << "Drawing Triangle:";
    // print();
    glColor3f(color.r, color.g, color.b);
    glBegin(GL_TRIANGLES);
    {
        glVertex3f(p1.x, p1.y, p1.z);
        glVertex3f(p2.x, p2.y, p2.z);
        glVertex3f(p3.x, p3.y, p3.z);
    }
    glEnd();
}
void Triangle::print() const
{
    cout << "Triangle: \n";
    p1.print();
    p2.print();
    p3.print();
}
Vector Triangle::getNormal(const Point &point) const { return (p2 - p1).cross(p3 - p1).normalize(); }
double Triangle::intersect(const Ray &r) const
{
    // Plane plane = Plane(p1, p2, p3);
    // double t = plane.intersect(r);
    Matrix A = Matrix(3, 3);
    Matrix betaMat = Matrix(3, 3);
    Matrix gammaMat = Matrix(3, 3);
    Matrix tMat = Matrix(3, 3);
    A.setMatrix({{-r.direction.x, p2.x - p1.x, p3.x - p1.x},
                 {-r.direction.y, p2.y - p1.y, p3.y - p1.y},
                 {-r.direction.z, p2.z - p1.z, p3.z - p1.z}});
    double detA = A.determinant();
    // cout << "Determinant of A: " << detA << endl;
    if (fabs(detA) < 1e-6)
    {
        cout << "Ray is parallel to triangle, no intersection." << endl;
        return -1.0;
    }
    tMat.setMatrix({{r.origin.x - p1.x, p2.x - p1.x, p3.x - p1.x},
                    {r.origin.y - p1.y, p2.y - p1.y, p3.y - p1.y},
                    {r.origin.z - p1.z, p2.z - p1.z, p3.z - p1.z}});
    betaMat.setMatrix({{-r.direction.x, r.origin.x - p1.x, p3.x - p1.x},
                       {-r.direction.y, r.origin.y - p1.y, p3.y - p1.y},
                       {-r.direction.z, r.origin.z - p1.z, p3.z - p1.z}});
    gammaMat.setMatrix({{-r.direction.x, p2.x - p1.x, r.origin.x - p1.x},
                        {-r.direction.y, p2.y - p1.y, r.origin.y - p1.y},
                        {-r.direction.z, p2.z - p1.z, r.origin.z - p1.z}});
    double t = tMat.determinant() / detA;
    // cout << "Intersection t: " << t << endl;
    double beta = betaMat.determinant() / detA;
    double gamma = gammaMat.determinant() / detA;
    // cout << "Beta: " << beta << ", Gamma: " << gamma << endl;
    if (beta >= -1e-6 && gamma >= -1e-6 && beta + gamma <= 1 + 1e-6)
    {
        // cout << "Intersect detected with triangle." << endl;
        return t;
    }
    return -1.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                  Sphere                                                        //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Sphere::draw() const
{
    // cout << "Drawing Sphere:";
    // print();
    glColor3f(color.r, color.g, color.b);
    glPushMatrix();
    {
        glTranslatef(center.x, center.y, center.z);
        glutSolidSphere(radius, 50, 50);
    }
    glPopMatrix();
}
void Sphere::print() const
{
    cout << "Sphere: \n";
    cout << "Center: ";
    center.print();
    cout << "Radius: " << radius << "\n";
    cout << "Color: ";
    color.print();
}
Vector Sphere::getNormal(const Point &point) const { return (point - center).normalize(); }
double Sphere::intersect(const Ray &r) const
{
    // cout << "Checking intersection with Sphere at " << center.x << ", " << center.y << ", " << center.z
    //      << " with radius " << radius << endl;
    // cout << "Sphere intersection ray origin: " << r.origin.x << ", " << r.origin.y << ", " << r.origin.z
    //      << " direction: " << r.direction.x << ", " << r.direction.y << ", " << r.direction.z << endl;
    Vector shiftedOrigin = r.origin - referencePoint;
    double a = r.direction.dot(r.direction);
    double b = 2 * r.direction.dot(shiftedOrigin);
    double c = shiftedOrigin.dot(shiftedOrigin) - radius * radius;
    // cout << "Sphere intersection coefficients: a = " << a << ", b = " << b << ", c = " << c << endl;
    double discriminant = b * b - 4 * a * c;
    // cout << "Sphere intersection discriminant: " << discriminant << endl;
    if (discriminant < 0)
        return -1.0;
    double t1 = (-b - sqrt(discriminant)) / (2 * a);
    double t2 = (-b + sqrt(discriminant)) / (2 * a);
    // cout << "Sphere intersection t1: " << t1 << ", t2: " << t2 << endl;
    if (t1 < 0 && t2 < 0)
        return -1.0;
    if (t1 < 0)
        return t2;
    if (t2 < 0)
        return t1;
    return min(t1, t2);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           QuadraticSurface                                                     //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void QuadraticSurface::draw() const
{
    // cout << "Drawing Quadratic Surface:";
    // print();
}
void QuadraticSurface::print() const
{
    cout << "Equation: \n";
    cout << A << "x^2 + " << B << "y^2 + " << C << "z^2 + " << D << "xy + " << E << "xz + "
         << F << "yz + " << G << "x + " << H << "y + " << I << "z + " << J << " = 0\n";
}
Vector QuadraticSurface::getNormal(const Point &point) const
{
    double dx = 2 * A * point.x + D * point.y + E * point.z + G; // 2Ax + Dy + Ez + G
    double dy = 2 * B * point.y + D * point.x + F * point.z + H; // 2By + Dx + Fz + H
    double dz = 2 * C * point.z + E * point.x + F * point.y + I; // 2Cz + Ex + Fy + I
    return Vector(dx, dy, dz).normalize();
}
// double QuadraticSurface::intersect(const Ray &r) const
// {
//     Point origin = r.origin;
//     double a = A * r.direction.x * r.direction.x + B * r.direction.y * r.direction.y + C * r.direction.z * r.direction.z +
//                D * r.direction.x * r.direction.y + E * r.direction.x * r.direction.z + F * r.direction.y * r.direction.z;
//     double b = 2 * A * origin.x * r.direction.x + 2 * B * origin.y * r.direction.y + 2 * C * origin.z * r.direction.z +
//                D * (origin.x * r.direction.y + origin.y * r.direction.x) +
//                E * (origin.x * r.direction.z + origin.z * r.direction.x) +
//                F * (origin.y * r.direction.z + origin.z * r.direction.y) +
//                G * r.direction.x + H * r.direction.y + I * r.direction.z;
//     double c = A * origin.x * origin.x + B * origin.y * origin.y + C * origin.z * origin.z +
//                D * origin.x * origin.y + E * origin.x * origin.z + F * origin.y * origin.z +
//                G * origin.x + H * origin.y + I * origin.z + J;
//     double discriminant = b * b - 4 * a * c;
//     if (discriminant < 0)
//         return -1;
//     double t1 = (-b - sqrt(discriminant)) / (2 * a);
//     double t2 = (-b + sqrt(discriminant)) / (2 * a);
//     double t = -1;
//     if (t1 > 0)
//         t = t1;
//     if (t2 > 0 && (t < 0 || t2 < t))
//         t = t2;
//     if (t < 0)
//         return -1;
//     Point intersect = origin + r.direction * t;
//     if (fabs(length) > 1e-6 &&
//         (intersect.x < referencePoint.x - 1e-6 ||
//          intersect.x > referencePoint.x + length + 1e-6))
//         return -1;
//     if (fabs(width) > 1e-6 &&
//         (intersect.y < referencePoint.y - 1e-6 ||
//          intersect.y > referencePoint.y + width + 1e-6))
//         return -1;
//     if (fabs(height) > 1e-6 &&
//         (intersect.z < referencePoint.z - 1e-6 ||
//          intersect.z > referencePoint.z + height + 1e-6))
//         return -1;
//     // cout << "QuadraticSurface intersection t: " << t << endl;
//     // cout << "Intersection point: (" << intersect.x << ", " << intersect.y << ", " << intersect.z << ")\n";
//     return t;
// }
double QuadraticSurface::intersect(const Ray &r) const
{
    Point origin = r.origin;
    Vector dir = r.direction;

    double a = A * dir.x * dir.x + B * dir.y * dir.y + C * dir.z * dir.z +
               D * dir.x * dir.y + E * dir.x * dir.z + F * dir.y * dir.z;

    double b = 2 * A * origin.x * dir.x + 2 * B * origin.y * dir.y + 2 * C * origin.z * dir.z +
               D * (origin.x * dir.y + origin.y * dir.x) +
               E * (origin.x * dir.z + origin.z * dir.x) +
               F * (origin.y * dir.z + origin.z * dir.y) +
               G * dir.x + H * dir.y + I * dir.z;

    double c = A * origin.x * origin.x + B * origin.y * origin.y + C * origin.z * origin.z +
               D * origin.x * origin.y + E * origin.x * origin.z + F * origin.y * origin.z +
               G * origin.x + H * origin.y + I * origin.z + J;

    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
        return -1;

    double sqrt_disc = sqrt(discriminant);
    double t1 = (-b - sqrt_disc) / (2 * a);
    double t2 = (-b + sqrt_disc) / (2 * a);

    // Try both t1 and t2 — nearest valid one wins
    double t = -1;
    for (double candidate : {t1, t2})
    {
        if (candidate < 1e-6)
            continue;
        Point intersect = origin + dir * candidate;

        // Bounding box check with proper min/max handling
        if (fabs(length) > 1e-6)
        {
            double x_min = std::min(referencePoint.x, referencePoint.x + length);
            double x_max = std::max(referencePoint.x, referencePoint.x + length);
            if (intersect.x < x_min - 1e-6 || intersect.x > x_max + 1e-6)
                continue;
        }
        if (fabs(width) > 1e-6)
        {
            double y_min = std::min(referencePoint.y, referencePoint.y + width);
            double y_max = std::max(referencePoint.y, referencePoint.y + width);
            if (intersect.y < y_min - 1e-6 || intersect.y > y_max + 1e-6)
                continue;
        }
        if (fabs(height) > 1e-6)
        {
            double z_min = std::min(referencePoint.z, referencePoint.z + height);
            double z_max = std::max(referencePoint.z, referencePoint.z + height);
            if (intersect.z < z_min - 1e-6 || intersect.z > z_max + 1e-6)
                continue;
        }

        // Valid t found
        if (t < 0 || candidate < t)
            t = candidate;
    }

    return t;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                Floor                                                           //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Floor::print() const
{
    cout << "Floor: \n";
    cout << "Width: " << floorWidth << ", Tile Width: " << tileWidth << "\n";
}
Vector Floor::getNormal(const Point &point) const { return plane.normal; }
void Floor::setGLTextureID(GLuint id) { glTextureID = id; }
// void Floor::draw() const
// {
// int numTiles = floorWidth / tileWidth;
// double x = -floorWidth / 2.0;
// glPushMatrix();
// {
//     for (int i = 0; i < numTiles; i++)
//     {
//         double y = -floorWidth / 2.0;
//         for (int j = 0; j < numTiles; j++)
//         {
//             if ((i + j) % 2 == 0)
//                 glColor3f(0.0, 0.0, 0.0);
//             else
//                 glColor3f(1.0, 1.0, 1.0);
//             glBegin(GL_QUADS);
//             {
//                 glVertex3f(x, y, 0.0);
//                 glVertex3f(x + tileWidth, y, 0.0);
//                 glVertex3f(x + tileWidth, y + tileWidth, 0.0);
//                 glVertex3f(x, y + tileWidth, 0.0);
//             }
//             glEnd();
//             y += tileWidth;
//         }
//         x += tileWidth;
//     }
//     glPopMatrix();
// }
// }
void Floor::draw() const
{
    int halfGrid = floorWidth / 2;
    double tileSize = tileWidth;

    for (int i = -halfGrid; i < halfGrid; i++)
    {
        for (int j = -halfGrid; j < halfGrid; j++)
        {
            bool isBlack = (i + j) % 2 == 0;
            if (useTexture && glTextureID != 0)
            {
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, glTextureID);
                glColor3f(1, 1, 1);
            }
            else
            {
                glDisable(GL_TEXTURE_2D);
                if (isBlack)
                    glColor3f(0, 0, 0);
                else
                    glColor3f(1, 1, 1);
            }
            glBegin(GL_QUADS);
            if (useTexture && glTextureID != 0)
                glTexCoord2f(0, 0);
            glVertex3f(i * tileSize, j * tileSize, 0);
            if (useTexture && glTextureID != 0)
                glTexCoord2f(1, 0);
            glVertex3f((i + 1) * tileSize, j * tileSize, 0);
            if (useTexture && glTextureID != 0)
                glTexCoord2f(1, 1);
            glVertex3f((i + 1) * tileSize, (j + 1) * tileSize, 0);
            if (useTexture && glTextureID != 0)
                glTexCoord2f(0, 1);
            glVertex3f(i * tileSize, (j + 1) * tileSize, 0);
            glEnd();
        }
    }
    glDisable(GL_TEXTURE_2D); // Always disable after drawing
}
double Floor::intersect(const Ray &r) const
{
    double t = plane.intersect(r);
    if (t < 1e-6)
        return -1.0;
    Point intersection = r.origin + r.direction * t;
    if (intersection.x < referencePoint.x - 1e-6 || intersection.x > referencePoint.x + floorWidth + 1e-6 ||
        intersection.y < referencePoint.y - 1e-6 || intersection.y > referencePoint.y + floorWidth + 1e-6)
        return -1.0;
    // cout << "Floor intersection t: " << t << endl;
    // cout << "Intersection point: (" << intersection.x << ", " << intersection.y << ", " << intersection.z << ")\n";
    return t;
}
Color Floor::getColor(const Point &p) const
{
    if (!useTexture)
    {
        int i = (int)((p.x + floorWidth / 2.0) / tileWidth);
        int j = (int)((p.y + floorWidth / 2.0) / tileWidth);
        if (i < 0 || j < 0 || i >= floorWidth / tileWidth || j >= floorWidth / tileWidth)
            return Color(0.0, 0.0, 0.0); // Outside floor
        if ((i + j) % 2 == 0)
            return Color(0.0, 0.0, 0.0);
        else
            return Color(1.0, 1.0, 1.0);
    }
    else
    {
        int i = static_cast<int>((p.x - referencePoint.x) / tileWidth);
        int j = static_cast<int>((p.y - referencePoint.y) / tileWidth);
        double u = fmod(p.x - referencePoint.x, tileWidth) / tileWidth;
        double v = fmod(p.y - referencePoint.y, tileWidth) / tileWidth;
        u = clamp(u, 0.0, 1.0);
        v = clamp(v, 0.0, 1.0);
        return sampleTexture(u, v);
    }
}
void Floor::loadTexture(const string &path)
{
    textureData = stbi_load(path.c_str(), &textureWidth, &textureHeight, &textureChannels, 0);
    if (!textureData)
    {
        cerr << "Failed to load texture: " << path << endl;
        useTexture = false;
    }
    else
    {
        useTexture = true;
        cout << "Loaded texture: " << path << " (" << textureWidth << "x" << textureHeight << ")\n";
    }
}
Color Floor::sampleTexture(double u, double v) const
{
    if (!textureData || textureWidth <= 0 || textureHeight <= 0)
        return Color(0.5, 0.5, 0.5);
    u = clamp(u, 0.0, 1.0);
    v = clamp(v, 0.0, 1.0);
    int pixel_x = static_cast<int>(u * (textureWidth - 1));
    int pixel_y = static_cast<int>((1.0 - v) * (textureHeight - 1));
    pixel_x = clamp(pixel_x, 0, textureWidth - 1);
    pixel_y = clamp(pixel_y, 0, textureHeight - 1);
    int index = (pixel_y * textureWidth + pixel_x) * textureChannels;
    int max_index = textureWidth * textureHeight * textureChannels;
    if (index < 0 || index + 2 >= max_index)
        return Color(1.0, 0.0, 1.0);
    Color c;
    c.r = textureData[index] / 255.0;
    c.g = textureChannels >= 2 ? textureData[index + 1] / 255.0 : c.r;
    c.b = textureChannels >= 3 ? textureData[index + 2] / 255.0 : c.r;
    return c;
}
// void Floor::setTexture(unsigned char *data, int width, int height, int channels)
// {
//     textureData = data;
//     textureWidth = width;
//     textureHeight = height;
//     textureChannels = channels;
//     useTexture = true;
// }
void Floor::setTexture(unsigned char *data, int width, int height, int channels)
{
    textureData = data;
    textureWidth = width;
    textureHeight = height;
    textureChannels = channels;
    useTexture = false;
    if (glTextureID == 0)
    {
        glGenTextures(1, &glTextureID);
    }
    glBindTexture(GL_TEXTURE_2D, glTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                  Ray                                                           //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                Light                                                           //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                            PointLight                                                          //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PointLight::draw() const
{
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glutSolidSphere(0.4, 50, 50);
    glPopMatrix();
}
void PointLight::print() const
{
    cout << "PointLight: \n";
    cout << "Position: ";
    position.print();
    cout << "Color: ";
    color.print();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                              SpotLight                                                         //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SpotLight::draw() const
{
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glutSolidSphere(0.2, 50, 50);
    glPopMatrix();
}
void SpotLight::print() const
{
    cout << "SpotLight: \n";
    cout << "Position: ";
    position.print();
    cout << "Direction: ";
    direction.print();
    cout << "Cutoff Angle: " << cutoffAngle << "\n";
    cout << "Color: ";
    color.print();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                 Matrix                                                         //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Matrix &Matrix::operator=(const Matrix &other)
{
    if (this == &other)
        return *this;
    rows = other.rows;
    cols = other.cols;
    data = other.data;
    return *this;
}
void Matrix::setValue(int row, int col, double value)
{
    if (row >= 0 && row < rows && col >= 0 && col < cols)
    {
        data[row][col] = value;
    }
}
double Matrix::getValue(int row, int col) const
{
    if (row >= 0 && row < rows && col >= 0 && col < cols)
    {
        return data[row][col];
    }
    return 0.0;
}
void Matrix::constructMatrix(double **values)
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            data[i][j] = values[i][j];
        }
    }
}
void Matrix::constructMatrix(const std::vector<std::vector<double>> &values)
{
    if ((int)values.size() != rows || (int)values[0].size() != cols)
        throw std::invalid_argument("Matrix size mismatch in constructMatrix.");
    data = values;
}

Matrix Matrix::operator*(const Matrix &other) const
{
    if (cols != other.rows)
    {
        throw std::invalid_argument("Matrix dimensions do not match for multiplication.");
    }
    Matrix result(rows, other.cols);
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < other.cols; ++j)
        {
            for (int k = 0; k < cols; ++k)
            {
                result.data[i][j] += data[i][k] * other.data[k][j];
            }
        }
    }
    return result;
}
Matrix Matrix::operator+(const Matrix &other) const
{
    if (rows != other.rows || cols != other.cols)
    {
        throw std::invalid_argument("Matrix dimensions do not match for addition.");
    }
    Matrix result(rows, cols);
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            result.data[i][j] = data[i][j] + other.data[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator-(const Matrix &other) const
{
    if (rows != other.rows || cols != other.cols)
    {
        throw std::invalid_argument("Matrix dimensions do not match for subtraction.");
    }
    Matrix result(rows, cols);
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            result.data[i][j] = data[i][j] - other.data[i][j];
        }
    }
    return result;
}
Matrix Matrix::transpose() const
{
    Matrix result(cols, rows);
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            result.data[j][i] = data[i][j];
        }
    }
    return result;
}
Matrix Matrix::identity() const
{
    if (rows != cols)
    {
        throw std::invalid_argument("Matrix must be square to create an identity matrix.");
    }
    Matrix result(rows, cols);
    for (int i = 0; i < rows; ++i)
    {
        result.data[i][i] = 1.0;
    }
    return result;
}
void Matrix::print() const
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            cout << data[i][j] << " ";
        }
        cout << endl;
    }
}
double Matrix::determinant() const
{
    if (rows != cols)
    {
        throw invalid_argument("Matrix must be square.");
    }
    if (rows == 1)
    {
        return data[0][0];
    }
    if (rows == 2)
    {
        return data[0][0] * data[1][1] - data[0][1] * data[1][0];
    }
    double det = 0.0;
    for (int i = 0; i < cols; i++)
    {
        Matrix sub(rows - 1, cols - 1);
        for (int j = 1; j < rows; j++)
        {
            int colIndex = 0;
            for (int k = 0; k < cols; k++)
            {
                if (k == i)
                    continue;
                sub.data[j - 1][colIndex++] = data[j][k];
            }
        }
        double cofactor = ((i % 2 == 0) ? 1 : -1) * data[0][i] * sub.determinant();
        det += cofactor;
    }
    return det;
}
void Matrix::setMatrix(std::vector<std::vector<double>> values)
{
    if ((int)values.size() != rows || (int)values[0].size() != cols)
        throw std::invalid_argument("Matrix size mismatch in setMatrix.");
    data = values;
}
Matrix Matrix::operator*(double scalar) const
{
    Matrix result(rows, cols);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            result.data[i][j] = data[i][j] * scalar;
        }
    }
    return result;
}
Matrix Matrix::operator/(double scalar) const
{
    Matrix result(rows, cols);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            result.data[i][j] = data[i][j] / scalar;
        }
    }
    return result;
}