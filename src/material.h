#ifndef MATERIAL_H
#define MATERIAL_H

#include <stdbool.h>

#include "util.h"
#include "onb.h"
#include "texture.h"
#include "hit_record.h"
#include "pdf.h"

struct scatter_record;

typedef bool (*mat_scatter)(ray, struct hit_record *, struct scatter_record *);
typedef color (*emitted)(const struct material *, const ray, const struct hit_record, double, double, point3);
typedef double (*scatter_pdf)(const ray, const struct hit_record, const ray);

typedef struct material {
    color albedo;
    double fuzz;
    texture tex;
    emitted emit;
    mat_scatter scatter_func;
    scatter_pdf pdf;
} material;

typedef struct scatter_record{
    color attenuation;
    pdf *pdf_ptr;
    bool skip_pdf;
    ray skip_pdf_ray;
} scatter_record;

color non_emitting(const material *m, const ray r_in, const hit_record rec, double u, double v, point3 p);
void set_face_normal(hit_record *h, ray r, vector3 outward_normal);

void copy_material(material *m, material to_copy);

void init_lambertian(material *m, color a);
void init_lambertian_tex(material *m, texture t);
bool lambertian_scatter(ray ray_in, 
        struct hit_record *rec, scatter_record *srec);
double lambertian_pdf(const ray r_in, const hit_record rec, const ray r_out);

void init_isotropic(material *m, color a);
void init_isotropic_tex(material *m, texture t);
bool isotropic_scatter(ray ray_in, 
        struct hit_record *rec, scatter_record *srec);
double isotropic_pdf(const ray r_in, const hit_record rec, const ray r_out);

void init_metal(material *m, color a, double fuzz);
bool metal_scatter(ray ray_in, 
        struct hit_record *rec, scatter_record *srec);

void init_dielectric(material *m, double fuzz);
bool dielectric_scatter(ray ray_in, 
        struct hit_record *rec, scatter_record *srec);

void init_diffuse_light_tex(material *m, texture *t);
void init_diffuse_light(material *m, color c);
color emit(const material *m, const ray r_in, const hit_record rec, double u, double v, point3 p);

#endif
