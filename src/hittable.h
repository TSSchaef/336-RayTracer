#ifndef HITTABLE_H
#define HITTABLE_H

#include <stdbool.h>

#include "ray.h"
#include "vector3.h"

typedef struct {
    point3 p;
    vector3 normal;
    double t;
    bool front_face;
} hit_record;

void set_face_normal(hit_record *h, ray r, vector3 outward_normal);

#endif
