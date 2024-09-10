#ifndef CONSTANT_MEDIUM_H
#define CONSTANT_MEDIUM_H

#include <math.h>
#include <stdbool.h>

#include "util.h"
#include "material.h"
#include "hittable_list.h"
#include "aabb.h"

typedef struct {
    void *hittable;
    fptr_is_hit hit_object;
    double neg_inv_density;
    aabb bbox;
    material phase_func;
} constant_medium;

void init_constant_medium(constant_medium *s, void *hittable, 
        fptr_is_hit hit, double density, aabb box, color albedo);
void init_constant_medium_tex(constant_medium *s, void *hittable, 
        fptr_is_hit hit, double density, aabb box, texture tex);
bool hit_constant_medium(void *c, ray r, double ray_tmin, double ray_tmax, hit_record *rec);

aabb get_constant_medium_box(void *s);

#endif

