#ifndef TRIANGLE_H 
#define TRIANGLE_H 

#include <math.h>
#include <stdbool.h>

#include "util.h"
#include "material.h"
#include "hittable_list.h"
#include "aabb.h"

typedef struct {
    point3 Q;
    vector3 u, v;
    vector3 normal;
    vector3 w;
    double D;
    material mat;
    aabb bbox;
} triangle;

void init_triangle(triangle *t, point3 Q, vector3 u, vector3 v, material mat);
bool hit_triangle(void *q, ray r, double ray_tmin, double ray_tmax, hit_record *rec);
aabb get_triangle_box(void *s);

#endif
