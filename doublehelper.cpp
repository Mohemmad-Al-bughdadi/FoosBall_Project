#include <cmath>
#include "doublehelper.h"
#define RESOLUTION 0.0001
bool floatsequal(const float &x, const float &y)
{
    float c=fabs(x-y);
    return c<=RESOLUTION;
}
