#ifndef UTIL_H
#define UTIL_H

#include <math.h>
#include <stdlib.h>
#include <float.h>

#include "vector3.h"
#include "ray.h"

#define NUM_THREADS 8

#define PI 3.1415926535897932385
#define DEG_TO_RAD(deg) ((deg * PI) / 180.0)

#define RAND_DOUBLE (rand() / (RAND_MAX + 1.0))
#define RAND_DBL(min, max) (min + (max - min) * RAND_DOUBLE)

#endif
