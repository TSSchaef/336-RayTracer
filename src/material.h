#ifndef MATERIAL_H
#define MATERIAL_H

#include <stdbool.h>

#include "util.h"
#include "onb.h"
#include "texture.h"


struct hit_record;
struct material;
typedef bool (*mat_scatter)(ray, struct hit_record *, color *, ray *, double *);
typedef color (*emitted)(const struct material *, double, double, point3);
typedef double (*scatter_pdf)(const ray, const struct hit_record, const ray);

typedef struct material {
    color albedo;
    double fuzz;
    texture tex;
    emitted emit;
    mat_scatter scatter_func;
    scatter_pdf pdf;
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

color non_emitting(const material *m, double u, double v, point3 p);
void set_face_normal(hit_record *h, ray r, vector3 outward_normal);
void copy_hit_record(hit_record *h, hit_record to_copy);

void copy_material(material *m, material to_copy);

void init_lambertian(material *m, color a);
void init_lambertian_tex(material *m, texture t);
bool lambertian_scatter(ray ray_in, 
        struct hit_record *rec, color *attenuation, ray *ray_out, double *pdf);
double lambertian_pdf(const ray r_in, const hit_record rec, const ray r_out);

void init_isotropic(material *m, color a);
void init_isotropic_tex(material *m, texture t);
bool isotropic_scatter(ray ray_in, 
        struct hit_record *rec, color *attenuation, ray *ray_out, double *pdf);
double isotropic_pdf(const ray r_in, const hit_record rec, const ray r_out);

void init_metal(material *m, color a, double fuzz);
bool metal_scatter(ray ray_in, 
        struct hit_record *rec, color *attenuation, ray *ray_out, double *pdf);

void init_dielectric(material *m, double fuzz);
bool dielectric_scatter(ray ray_in, 
        struct hit_record *rec, color *attenuation, ray *ray_out, double *pdf);

void init_diffuse_light_tex(material *m, texture *t);
void init_diffuse_light(material *m, color c);
color emit(const material *m, double u, double v, point3 p);

#endif
