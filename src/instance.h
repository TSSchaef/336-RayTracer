#ifndef INSTANCE_H
#define INSTANCE_H

#include <math.h>
#include <stdbool.h>

#include "util.h"
#include "hittable_list.h"
#include "material.h"
#include "aabb.h"

typedef struct {
    void *hittable;
    fptr_is_hit hit_object;
    vector3 offset;
    aabb bbox;
} translate;

typedef struct{
    void *hittable;
    fptr_is_hit hit_object;
    double sin_theta;
    double cos_theta;
    aabb bbox;
} rotate;

void init_translate(translate *t, void *hittable, fptr_is_hit hit, aabb box, vector3 o);
bool hit_translate(const void *s, ray r, double ray_tmin, double ray_tmax, hit_record *rec);
aabb get_translate_box(const void *s);


void init_rotate(rotate *r, void *hittable, fptr_is_hit hit, aabb box, double theta);
bool hit_rotate(const void *s, ray r, double ray_tmin, double ray_tmax, hit_record *rec);
aabb get_rotate_box(const void *s);

#endif
