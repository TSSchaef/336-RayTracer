#ifndef BVH_H
#define BVH_H

#include "aabb.h"
#include "hittable_list.h"
#include "material.h"
#include "util.h"

#include <stdbool.h>

typedef struct bvh_node{
    void *left;
    void *right;
    fptr_is_hit hit_left;
    fptr_is_hit hit_right;
    fptr_get_box box_left;
    fptr_get_box box_right;
    aabb bbox;
    bool is_leaf;
} bvh_node;

aabb get_bvh_box(void *b);
void init_bvh(bvh_node *b, hittable_list *list);

bool hit_bvh(void *b, ray r, double ray_tmin, double ray_tmax, hit_record *rec);

void delete_bvh(bvh_node *b);

#endif
