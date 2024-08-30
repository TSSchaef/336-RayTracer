#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <stdlib.h>
#include <stdbool.h>

#include "hittable.h"
#include "util.h"

typedef bool (*fptr_is_hit)(void *, ray, double, double, hit_record *);

typedef struct hittable_list{
    void *hittable;
    //function pointer for is_hit function;
    fptr_is_hit is_hit;
    struct hittable_list *next;
} hittable_list;

void init_list(hittable_list *l, void *h, fptr_is_hit f);
void add_list(hittable_list *l, void *h, fptr_is_hit f);
void delete_list(hittable_list *l);
bool hit(hittable_list *l, ray r, double ray_tmin, 
        double ray_tmax, hit_record *rec);

#endif
