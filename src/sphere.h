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

void init_sphere(sphere *s, point3 center, double radius, material mat);
bool hit_sphere(const void *s, ray r, double ray_tmin, double ray_tmax, hit_record *rec);

aabb get_sphere_box(const void *s);

double sphere_pdf_value(const void *s, const point3 orig, const vector3 dir);
vector3 sphere_pdf_generate(const void *s, const point3 orig);

#endif
