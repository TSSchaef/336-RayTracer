#ifndef MATERIAL_H
#define MATERIAL_H

#include <stdbool.h>

#include "util.h"
#include "texture.h"


struct hit_record;
typedef bool (*mat_scatter)(ray, struct hit_record *, color *, ray *);

typedef struct material {
    color albedo;
    double fuzz;
    texture tex;
    mat_scatter scatter_func;
} material;

typedef struct hit_record{
    point3 p;
    vector3 normal;
    double t;
    double u;
    double v;
    bool front_face;
    struct material mat;
} hit_record;

void set_face_normal(hit_record *h, ray r, vector3 outward_normal);
void copy_hit_record(hit_record *h, hit_record to_copy);

void copy_material(material *m, material to_copy);

void init_lambertian(material *m, color a);
void init_lambertian_tex(material *m, texture t);
bool lambertian_scatter(ray ray_in, 
        struct hit_record *rec, color *attenuation, ray *ray_out);

void init_metal(material *m, color a, double fuzz);
bool metal_scatter(ray ray_in, 
        struct hit_record *rec, color *attenuation, ray *ray_out);

void init_dielectric(material *m, double fuzz);
bool dielectric_scatter(ray ray_in, 
        struct hit_record *rec, color *attenuation, ray *ray_out);

#endif
