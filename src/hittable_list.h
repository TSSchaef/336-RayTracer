#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <stdlib.h>
#include <stdbool.h>

#include "util.h"
#include "material.h"
#include "aabb.h"

typedef bool (*fptr_is_hit)(void *, ray, double, double, hit_record *);

typedef struct hittable_node{
    void *hittable;
    //function pointer for is_hit function;
    fptr_is_hit is_hit;
    fptr_get_box get_box;
    struct hittable_node *next;
} hittable_node;

typedef struct hittable_list{
    aabb box;
    hittable_node *head;
    int size;
} hittable_list;

void sort_list(hittable_list *l, int start, int end, int axis);
void init_list(hittable_list *l);
void add_list(hittable_list *l, void *h, fptr_is_hit f, fptr_get_box get);
hittable_node *index_list(hittable_list *l, int i);
void delete_list(hittable_list *l);
bool hit(hittable_list *l, ray r, double ray_tmin, 
        double ray_tmax, hit_record *rec);

#endif
