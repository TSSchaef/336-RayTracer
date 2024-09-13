#ifndef TRIANGLE_H 
#define TRIANGLE_H 

#include <math.h>
#include <stdbool.h>

#include "util.h"
#include "material.h"
#include "hittable_list.h"
#include "aabb.h"

typedef struct {
    point3 a, b, c;
    vector3 normal;
    double D;
    material mat;
    aabb bbox;
} triangle;

void init_triangle(triangle *t, point3 a, point3 b, point3 c, material mat);
void init_triangle_norm(triangle *t, point3 a, point3 b, point3 c, vector3 n, material mat);
bool hit_triangle(void *q, ray r, double ray_tmin, double ray_tmax, hit_record *rec);
aabb get_triangle_box(void *s);

#endif
