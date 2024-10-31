#ifndef HIT_RECORD_H
#define HIT_RECORD_H

#include "util.h"

struct material;

typedef struct hit_record{
    point3 p;
    vector3 normal;
    double t;
    double u;
    double v;
    bool front_face;
    struct material *mat;
} hit_record;

void copy_hit_record(hit_record *h, hit_record to_copy);

#endif
