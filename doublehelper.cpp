#include <cmath>
#include "doublehelper.h"
#define RESOLUTION 0.0001
bool doublesequal(const double &x, const double &y)
{
    double c=fabs(x-y);
    return c<=RESOLUTION;
}
