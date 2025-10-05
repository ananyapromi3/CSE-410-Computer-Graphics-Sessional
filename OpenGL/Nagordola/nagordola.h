#include <vector>
#include "plane.h"
#include "globals.h"

class Nagordola
{
private:
public:
    std::vector<Plane> planes;
    std::vector<Point> points;
    int count;
    float radius;
    float height;
    float blocKSize;
    bool isRotating = false;
    float speed;
    int angleR;
    Nagordola(float r);
    int getCount();
    void rotate();
    void spread();
    void shrink();
};
