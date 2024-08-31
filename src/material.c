#include "material.h"

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
    copy_material(&(h->mat), to_copy.mat);
}

void copy_material(material *m, material to_copy){
    copy(&(m->albedo), to_copy.albedo);
    m->fuzz = to_copy.fuzz;
    m->scatter_func = to_copy.scatter_func;
}

bool lambertian_scatter(ray ray_in, 
        struct hit_record *rec, color *attenuation, ray *ray_out){

    vector3 dir = random_unit_vector();
    add_vector(&dir, rec->normal);
    
    if(near_zero(dir)){
        copy(&dir, rec->normal);
    }

    ray r;
    init_ray(&r, rec->p, dir);
    copy_ray(ray_out, r);
    copy(attenuation, rec->mat.albedo);
    return true;
}

void init_lambertian(material *m, color a){
    copy(&(m->albedo), a);
    m->fuzz = 0;
    m->scatter_func = &lambertian_scatter;
}

bool metal_scatter(ray ray_in, 
        struct hit_record *rec, color *attenuation, ray *ray_out){
    vector3 reflection = reflect(ray_in.dir, rec->normal);

    unit_vector(&reflection);
    vector3 rand;
    copy(&rand, random_unit_vector());
    scale(&rand, rec->mat.fuzz);
    add_vector(&reflection, rand);

    ray r;
    init_ray(&r, rec->p, reflection);
    copy_ray(ray_out, r);
    copy(attenuation, rec->mat.albedo);
    return (dot(r.dir, rec->normal) > 0);
}

void init_metal(material *m, color a, double f){
    copy(&(m->albedo), a);
    m->fuzz = (f < 1 && f >= 0) ? f : 1;
    m->scatter_func = &metal_scatter;
}
