#ifndef RAY_H
#define RAY_H

#include "vector3.h"

typedef struct {
    point3 orig;
    vector3 dir;
} ray;

void init_ray(ray *r, point3 origin, vector3 direction);
point3 at(ray r, double t);

#endif
