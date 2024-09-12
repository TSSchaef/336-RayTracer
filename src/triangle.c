#include "triangle.h"

static void set_bbox(triangle*q){
    point3 temp;
    copy(&temp, q->Q); 

    add_vector(&(temp), q->u);
    add_vector(&(temp), q->v);

    init_aabb_points(&(q->bbox), q->Q, temp);

    aabb temp_box;
    point3 temp2;
    copy(&temp, q->Q); 
    copy(&temp2, q->Q); 
    
    add_vector(&(temp), q->u);
    add_vector(&(temp2), q->v);

    init_aabb_points(&temp_box, temp, temp2);

    add_boxes(&(q->bbox), temp_box);
}

void init_triangle(triangle *q, point3 Qc, vector3 uc, vector3 vc, material matc){
    copy(&(q->Q), Qc);  
    copy(&(q->u), uc);  
    copy(&(q->v), vc);  

    q->normal = cross(uc, vc);

    copy(&(q->w), q->normal);
    scale(&(q->w), 1.0 / dot(q->normal, q->normal));
    
    unit_vector(&(q->normal));

    q->D = dot(q->normal, Qc);

    copy_material(&(q->mat), matc);
    set_bbox(q);
}

static bool is_interior(triangle *q, double a, double b, hit_record *rec){
    if(a < 0 || b < 0 || a + b > 1) return false;

    rec->u = a;
    rec->v = b;
    return true;
}

bool hit_triangle(void *q, ray r, double ray_tmin, double ray_tmax, hit_record *rec){
    triangle *qu = ((triangle *) q);
    double denom = dot(qu->normal, r.dir);
    
    if(denom < 1e-8 && denom > -1e-8) return false;

    double t = (qu->D - dot(qu->normal, r.orig)) / denom;
    if(t < ray_tmin || t > ray_tmax) return false;

    point3 intersection = at(r, t);
    vector3 planar_hitpt;
    copy(&planar_hitpt, qu->Q);
    invert(&planar_hitpt);
    add_vector(&planar_hitpt, intersection);

    double alpha = dot(qu->w, cross(planar_hitpt, qu->v));
    double beta = dot(qu->w, cross(qu->u, planar_hitpt));

    if(!is_interior(qu, alpha, beta, rec))
        return false;

    rec->t = t;
    copy(&(rec->p), intersection);
    copy_material(&(rec->mat), qu->mat);
    set_face_normal(rec, r, qu->normal);

    return true;
}

aabb get_triangle_box(void *s){
    return ((triangle *)s)->bbox;
}
