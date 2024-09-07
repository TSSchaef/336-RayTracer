#ifndef QUAD_H
#define QUAD_H

#include <math.h>
#include <stdbool.h>

#include "util.h"
#include "material.h"
#include "aabb.h"

typedef struct {
    point3 Q;
    vector3 u, v;
    vector3 normal;
    vector3 w;
    double D;
    material mat;
    aabb bbox;
} quad;

void init_quad(quad *q, point3 Q, vector3 u, vector3 v, material mat);
bool hit_quad(void *q, ray r, double ray_tmin, double ray_tmax, hit_record *rec);
aabb get_quad_box(void *s);

#endif
