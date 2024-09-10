#ifndef AABB_H
#define AABB_H

#include "util.h"

typedef struct {
    double x[2];
    double y[2];
    double z[2];
} aabb;

typedef aabb (*fptr_get_box)(void *);

void init_aabb(aabb *box, double x0, double x1, double y0, double y1,
                double z0, double z1);
void init_aabb_points(aabb *box, point3 a, point3 b);
void add_boxes(aabb *box, aabb add);

void copy_box(aabb *box, aabb to_copy);
int longest_axis(aabb box);

bool hit_box(void *box, ray r, double ray_tmin, double ray_tmax);
void shift_box(aabb *box, vector3 offset);

#endif
