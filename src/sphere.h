#ifndef SPHERE_H
#define SPHERE_H

#include <math.h>
#include <stdbool.h>

#include "util.h"
#include "material.h"
#include "aabb.h"

typedef struct {
    double radius;
    point3 center;
    aabb bbox;
    material mat;
} sphere;

//some change
void init_sphere(sphere *s, point3 center, double radius, material mat);
bool hit_sphere(void *s, ray r, double ray_tmin, double ray_tmax, hit_record *rec);

aabb get_sphere_box(void *s);

#endif
