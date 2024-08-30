#include "hittable.h"

void set_face_normal(hit_record *h, ray r, vector3 outward_normal){
    h->front_face = dot(r.dir, outward_normal) < 0;
    copy(&(h->normal), outward_normal);
    if(!h->front_face){
        invert(&(h->normal));
    }
}

void copy_hit_record(hit_record *h, hit_record to_copy){
    copy(&(h->p), to_copy.p);
    copy(&(h->normal), to_copy.normal);
    h->t = to_copy.t;
    h->front_face = to_copy.front_face;
}
