#ifndef SPHERE_H
#define SPHERE_H

#include <math.h>
#include <stdbool.h>

#include "util.h"
#include "material.h"

typedef struct {
    double radius;
    point3 center;
    material mat;
} sphere;

void init_sphere(sphere *s, point3 center, double radius, material mat);
bool hit_sphere(void *s, ray r, double ray_tmin, double ray_tmax, hit_record *rec);


#endif
