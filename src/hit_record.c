#include "hit_record.h"

void copy_hit_record(hit_record *h, hit_record to_copy){
    copy(&h->p, to_copy.p);
    copy(&h->normal, to_copy.normal);
    h->t = to_copy.t;
    h->u = to_copy.u;
    h->v = to_copy.v;
    h->front_face = to_copy.front_face;
    h->mat = to_copy.mat;
}

