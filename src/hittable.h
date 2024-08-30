#ifndef HITTABLE_H
#define HITTABLE_H

#include <stdbool.h>

#include "util.h"

typedef struct {
    point3 p;
    vector3 normal;
    double t;
    bool front_face;
} hit_record;

void set_face_normal(hit_record *h, ray r, vector3 outward_normal);
void copy_hit_record(hit_record *h, hit_record to_copy);

#endif
