#include <string>
#include <vector>
#include <GLUT/glut.h>

class Point;
class Vector;
class Plane;
class Camera;
class Color;
class Object;
class Sphere;
class Triangle;
class QuadraticSurface;
class Floor;
class Ray;
class Light;
class PointLight;
class SpotLight;

extern std::vector<Object *> objects;
extern std::vector<PointLight *> pointLights;
extern std::vector<SpotLight *> spotLights;

// template <typename T>
// T clamp(T value, T low, T high)
// {
//     return std::max(low, std::min(value, high));
// }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                  Point                                                         //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Point
{
public:
    double x, y, z;

    Point(double x = 0.0, double y = 0.0, double z = 0.0)
        : x(x), y(y), z(z) {}
    Point(const Point &p)
        : x(p.x), y(p.y), z(p.z) {}

    Vector operator-(const Point &p) const;
    Point operator+(const Vector &v) const;
    Point operator-(const Vector &v) const;
    Point operator+=(const Vector &v);
    Point operator-=(const Vector &v);
    double distance(const Point &p) const;
    Vector toVector() const;
    void print() const;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                  Vector                                                        //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Vector
{
public:
    double x, y, z;

    Vector(double x = 0.0, double y = 0.0, double z = 0.0)
        : x(x), y(y), z(z) {}
    Vector(const Vector &v) : x(v.x), y(v.y), z(v.z) {}

    Vector operator=(const Vector &v);
    Vector operator+(const Vector &v) const;
    Vector operator-(const Vector &v) const;
    Vector operator*(const double &d) const;
    Vector operator/(const double &d) const;
    Vector operator+=(const Vector &v);
    Vector operator+=(const double &d);
    Vector operator-=(const double &d);
    Vector operator-=(const Vector &v);
    Vector operator*=(const double &d);
    Vector operator/=(const double &d);
    double dot(const Vector &v) const;
    Vector cross(const Vector &v) const;
    Vector normalize() const;
    Vector rotate(const Vector &axis, double angle) const;
    Vector rotateAroundAxis(const Vector &axis, double angle);
    double norm() const;
    Vector reflect(const Plane &plane) const;
    Vector reflect(const Vector &normal) const;
    void print() const;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                  Plane                                                         //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Plane
{
public:
    Vector normal; // Normal vector of the plane
    Point point;   // A point on the plane

    Plane(const Vector &normal, const Point &point)
        : normal(normal), point(point) {}
    Plane(Point p1, Point p2, Point p3);

    float distanceToPoint(const Point &p) const;
    void print() const;
    double findZ(double x, double y) const;
    double intersect(const Ray &r) const;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                  Camera                                                        //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Camera
{
public:
    Point eye;    // Camera position
    Point center; // Look-at point
    Vector up;    // Up vector
    double moveSpeed;
    double rotateSpeed;

    Camera(Point eye = Point(1.0, 1.0, 1.0), Point center = Point(0.0, 0.0, 0.0), Vector up = Vector(0.0, 1.0, 0.0),
           double moveSpeed = 0.5, double rotateSpeed = 0.05)
        : eye(eye), center(center), up(up), moveSpeed(moveSpeed), rotateSpeed(rotateSpeed) {}
    Camera(const Camera &c)
        : eye(c.eye), center(c.center), up(c.up), moveSpeed(c.moveSpeed), rotateSpeed(c.rotateSpeed) {}

    Vector right();
    void setMoveSpeed(double speed);
    void setRotateSpeed(double speed);
    void move(const Vector &v);
    void rotate(const Vector &axis, double angle);
    void setView(Point eye, Point center, Vector up);
    void moveForward();
    void moveBackward();
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    void lookLeft();
    void lookRight();
    void lookUp();
    void lookDown();
    void tiltClockwise();
    void tiltAntiClockwise();
    void moveUp_wo_refPoint();
    void moveDown_wo_refPoint();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                  Color                                                         //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Color
{
public:
    std::string name;
    double r, g, b;

    Color(double r = 0.0, double g = 0.0, double b = 0.0)
        : r(r), g(g), b(b) {}

    Color operator+(const Color &c) const;
    Color operator*(const double &d) const;
    Color operator*(const Color &c) const;
    void clamp();
    void print() const;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                 Object                                                         //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Object
{
public:
    Point referencePoint;
    Color color;
    double width, height, length;
    double ambient, diffuse, specular, reflectionCoefficient;
    int shine;

    Object(const Point &referencePoint = Point(0.0, 0.0, 0.0))
        : referencePoint(referencePoint) {}
    Object(const Object &o)
        : referencePoint(o.referencePoint), color(o.color),
          width(o.width), height(o.height), length(o.length),
          ambient(o.ambient), diffuse(o.diffuse), specular(o.specular), reflectionCoefficient(o.reflectionCoefficient),
          shine(o.shine) {}
    virtual ~Object() {}

    virtual void draw() const = 0;
    virtual void print() const = 0;
    virtual Vector getNormal(const Point &point) const = 0;
    virtual double intersect(const Ray &r) const = 0;
    virtual Color getColor(const Point &p) const;
    // virtual void traceRay(const Ray &r, Color &color, int level) const;

    void traceRay(const Ray &r, Color &color, int level, const std::vector<PointLight *> &pointLights, const std::vector<SpotLight *> &spotLights) const;
    void setColor(const Color &c);
    void setCoefficients(double ambient, double diffuse, double specular, double reflectionCoefficient, int shine);
    Object *nextReflectionObject(const Ray &r) const;
    void setReferencePoint(const Point &p);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                Triangle                                                        //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Triangle : public Object
{
public:
    Point p1, p2, p3;

    Triangle(Point p1, Point p2, Point p3) : p1(p1), p2(p2), p3(p3) {}
    Triangle(const Triangle &t) : Object(t), p1(t.p1), p2(t.p2), p3(t.p3) {}
    virtual ~Triangle() {}

    virtual void draw() const override;
    virtual void print() const override;
    virtual Vector getNormal(const Point &point) const override;
    virtual double intersect(const Ray &r) const override;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                Sphere                                                          //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Sphere : public Object
{
public:
    Point center;
    double radius;

    Sphere(Point center, double radius)
        : Object(center), center(center), radius(radius) { length = radius; }
    Sphere(const Sphere &s) : Object(s), center(s.center), radius(s.radius) {}
    virtual ~Sphere() {}

    virtual void draw() const override;
    virtual void print() const override;
    virtual Vector getNormal(const Point &point) const override;
    virtual double intersect(const Ray &r) const override;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           QuadraticSurface                                                     //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class QuadraticSurface : public Object // Ax^2 + By^2 + Cz^2 + Dxy + Exz + Fyz + Gx + Hy + Iz + J = 0
{
public:
    double A, B, C, D, E, F, G, H, I, J;

    QuadraticSurface(const Point &ref, double height, double width, double length,
                     double A, double B, double C, double D, double E,
                     double F, double G, double H, double I, double J)
        : Object(ref), A(A), B(B), C(C), D(D), E(E), F(F), G(G), H(H), I(I), J(J)
    {
        this->height = height;
        this->width = width;
        this->length = length;
    }
    QuadraticSurface(const QuadraticSurface &qs)
        : Object(qs), A(qs.A), B(qs.B), C(qs.C), D(qs.D), E(qs.E), F(qs.F), G(qs.G), H(qs.H), I(qs.I), J(qs.J) {}
    virtual ~QuadraticSurface() {}

    virtual void draw() const override;
    virtual void print() const override;
    virtual Vector getNormal(const Point &point) const override;
    virtual double intersect(const Ray &r) const override;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                Floor                                                           //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Floor : public Object
{
public:
    double floorWidth, tileWidth;
    Plane plane;
    bool useTexture = false;
    unsigned char *textureData = nullptr;
    mutable GLuint glTextureID = 0; // OpenGL texture handle
    int textureWidth = 0, textureHeight = 0, textureChannels = 0;
    Floor(double floorWidth, double tileWidth)
        : Object(Point(-floorWidth / 2.0, -floorWidth / 2.0, 0.0)), floorWidth(floorWidth), tileWidth(tileWidth),
          useTexture(false), glTextureID(0), plane(Vector(0.0, 0.0, 1.0), Point(-floorWidth / 2.0, -floorWidth / 2.0, 0.0)) {}
    Floor(const Floor &f)
        : Object(f), floorWidth(f.floorWidth), tileWidth(f.tileWidth), plane(f.plane) {}
    virtual ~Floor() {}

    virtual void draw() const override;
    virtual void print() const override;
    virtual Vector getNormal(const Point &point) const override;
    virtual double intersect(const Ray &r) const override;
    virtual Color getColor(const Point &p) const override;

    void loadTexture(const std::string &path);
    Color sampleTexture(double u, double v) const;
    void setTexture(unsigned char *data, int width, int height, int channels);
    void setGLTextureID(GLuint id);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                  Ray                                                           //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Ray
{
public:
    Point origin;
    Vector direction;

    Ray(const Point &origin, const Vector &direction)
        : origin(origin), direction(direction.normalize()) {}
    Ray(const Ray &r) : origin(r.origin), direction(r.direction.normalize()) {}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                Light                                                           //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Light
{
public:
    Color color;
    Point position;

    Light(Color color = Color(1.0, 1.0, 1.0), Point position = Point(0.0, 0.0, 0.0))
        : color(color), position(position) {}
    virtual ~Light() {}

    virtual void draw() const = 0;
    virtual void print() const = 0;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                            PointLight                                                          //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class PointLight : public Light
{
public:
    PointLight(Point position = Point(0.0, 0.0, 0.0), Color color = Color(1.0, 1.0, 1.0))
        : Light(color, position) {}
    virtual ~PointLight() {}

    virtual void draw() const override;
    virtual void print() const override;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                              SpotLight                                                         //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SpotLight : public Light
{
public:
    Vector direction;
    double cutoffAngle;

    SpotLight(Point position = Point(0.0, 0.0, 0.0), Vector direction = Vector(0.0, 0.0, -1.0),
              double cutoffAngle = 30.0, Color color = Color(1.0, 1.0, 1.0))
        : Light(color, position), direction(direction.normalize()), cutoffAngle(cutoffAngle) {}
    virtual ~SpotLight() {}

    virtual void draw() const override;
    virtual void print() const override;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                 Matrix                                                         //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Matrix
{
private:
    int rows;
    int cols;
    std::vector<std::vector<double>> data;

public:
    Matrix(int rows, int cols) : rows(rows), cols(cols)
    {
        data.resize(rows, std::vector<double>(cols, 0.0));
    }
    Matrix(const Matrix &other) : rows(other.rows), cols(other.cols)
    {
        data = other.data;
    }
    Matrix(int rows, int cols, const std::vector<std::vector<double>> &values) : rows(rows), cols(cols)
    {
        data.resize(rows, std::vector<double>(cols, 0.0));
        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < cols; ++j)
                data[i][j] = values[i][j];
        }
    }
    ~Matrix() {}
    void setMatrix(std::vector<std::vector<double>> values);
    Matrix &operator=(const Matrix &other);
    void setValue(int row, int col, double value);
    void constructMatrix(const std::vector<std::vector<double>> &values);
    void constructMatrix(double **values);
    double getValue(int row, int col) const;
    void print() const;
    Matrix operator*(const Matrix &other) const;
    Matrix operator+(const Matrix &other) const;
    Matrix operator-(const Matrix &other) const;
    Matrix operator*(double scalar) const;
    Matrix operator/(double scalar) const;
    Matrix transpose() const;
    Matrix identity() const;
    double determinant() const;
};