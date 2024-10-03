#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <stdlib.h>
#include <stdbool.h>

#include "util.h"
#include "hit_record.h"
#include "aabb.h"

#define INIT_SIZE 10

typedef bool (*fptr_is_hit)(const void *, ray, double, double, hit_record *);
typedef double (*hittable_pdf_value)(const void *, const point3, const vector3);
typedef vector3 (*hittable_pdf_generate)(const void *, const point3);

typedef struct hittable_node{
    void *hittable;
    //function pointer for is_hit function;
    fptr_is_hit is_hit;
    fptr_get_box get_box;
    hittable_pdf_value pdf_value;
    hittable_pdf_generate pdf_generate;
} hittable_node;

typedef struct hittable_list{
    hittable_node **list;
    aabb box;
    int size;
    int max_size;
} hittable_list;

void sort_list(hittable_list *l, int start, int end, int axis);
void init_list(hittable_list *l);

void add_list(hittable_list *l, void *h, fptr_is_hit f, fptr_get_box get, 
        hittable_pdf_value v, hittable_pdf_generate g);
void add_list_no_pdf(hittable_list *l, void *h, fptr_is_hit f, fptr_get_box get);

hittable_node *index_list(hittable_list *l, int i);
void delete_list(hittable_list *l);
bool hit(const void *l, ray r, double ray_tmin, 
        double ray_tmax, hit_record *rec);
aabb get_list_box(const void *l);
aabb get_list_box_interval(hittable_list *l, int start, int end);

double hittable_list_pdf_value(const void *list, const point3 orig, const vector3 dir);
vector3 hittable_list_pdf_generate(const void *list, const point3 orig);

#endif
