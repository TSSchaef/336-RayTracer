#ifndef QUAD_H
#define QUAD_H

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
    double D, area;
    material mat;
    aabb bbox;
} quad;

void init_quad(quad *q, point3 Q, vector3 u, vector3 v, material mat);
bool hit_quad(const void *q, ray r, double ray_tmin, double ray_tmax, hit_record *rec);
aabb get_quad_box(const void *s);

double quad_pdf_value(const void *q, const point3 orig, const vector3 dir);
vector3 quad_pdf_generate(const void *q, const point3 orig);

hittable_list *init_cube(point3 a, point3 b, material mat);

double cube_pdf_value(const void *l, const point3 orig, const vector3 dir);
vector3 cube_pdf_generate(const void *list, const point3 orig);
void delete_cube(hittable_list *l);

#endif
