#ifndef UTIL_H
#define UTIL_H

#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <float.h>

#include "vector3.h"
#include "ray.h"

#define NUM_THREADS 32

#define PI 3.1415926535897932385
#define DEG_TO_RAD(deg) ((deg * PI) / 180.0)

double rnd_double();
double rnd_dbl(double min, double max);

#endif
