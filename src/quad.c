#include "quad.h"

static void set_bbox(quad *q){
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

void init_quad(quad *q, point3 Qc, vector3 uc, vector3 vc, material matc){
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

static bool is_interior(quad *q, double a, double b, hit_record *rec){
    if(a > 1 || a < 0 || b > 1 || b < 0) return false;

    rec->u = a;
    rec->v = b;
    return true;
}

bool hit_quad(void *q, ray r, double ray_tmin, double ray_tmax, hit_record *rec){
    quad *qu = ((quad *) q);
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

aabb get_quad_box(void *s){
    return ((quad *)s)->bbox;
}


hittable_list *init_cube(point3 a, point3 b, material mat){
  hittable_list *sides;
  sides = (hittable_list *) malloc(sizeof(hittable_list));
  init_list(sides);

  point3 min, max;
  init(&min, fmin(a.e[x], b.e[x]), fmin(a.e[y], b.e[y]), fmin(a.e[z], b.e[z]));
  init(&max, fmax(a.e[x], b.e[x]), fmax(a.e[y], b.e[y]), fmax(a.e[z], b.e[z]));

  vector3 dx, dy, dz, ndz, ndx;
  init(&dx, max.e[x] - min.e[x], 0, 0);
  copy(&ndx, dx);
  invert(&ndx);
  init(&dy, 0, max.e[y] - min.e[y], 0);
  init(&dz, 0, 0, max.e[z] - min.e[z]);
  copy(&ndz, dz);
  invert(&ndz);

  point3 f, r, ba, l, t, bo;
  init(&f, min.e[x], min.e[y], max.e[z]);
  init(&r, max.e[x], min.e[y], max.e[z]);
  init(&ba, max.e[x], min.e[y], min.e[z]);
  init(&l, min.e[x], min.e[y], min.e[z]);
  init(&t, min.e[x], max.e[y], max.e[z]);
  init(&bo, min.e[x], min.e[y], min.e[z]);

  quad *front, *right, *back, *left, *top, *bottom;
  front = (quad *) malloc(sizeof(quad));
  right = (quad *) malloc(sizeof(quad));
  back = (quad *) malloc(sizeof(quad));
  left = (quad *) malloc(sizeof(quad));
  top = (quad *) malloc(sizeof(quad));
  bottom = (quad *) malloc(sizeof(quad));

  init_quad(front, f, dx, dy, mat);
  init_quad(right, r, ndz, dy, mat);
  init_quad(back, ba, ndx, dy, mat);
  init_quad(left, l, dz, dy, mat);
  init_quad(top, t, dx, ndz, mat);
  init_quad(bottom, bo, dx, dz, mat);

  add_list(sides, front, &hit_quad, &get_quad_box);
  add_list(sides, right, &hit_quad, &get_quad_box);
  add_list(sides, back, &hit_quad, &get_quad_box);
  add_list(sides, left, &hit_quad, &get_quad_box);
  add_list(sides, top, &hit_quad, &get_quad_box);
  add_list(sides, bottom, &hit_quad, &get_quad_box);

  return sides;
}

void delete_cube(hittable_list *l){
    int i;
    for(i = 0; i < l->size; i++){
        free((l->list[i])->hittable);
    }
    delete_list(l);
    free(l);
}
