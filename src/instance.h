#ifndef INSTANCE_H
#define INSTANCE_H

#include <math.h>
#include <stdbool.h>

#include "util.h"
#include "hittable_list.h"
#include "material.h"
#include "aabb.h"
#include "pdf.h"

typedef struct {
    void *hittable;
    fptr_is_hit hit_object;
    hittable_pdf_value value;
    hittable_pdf_generate generate;
    vector3 offset;
    aabb bbox;
} translate;

typedef struct{
    void *hittable;
    fptr_is_hit hit_object;
    hittable_pdf_value value;
    hittable_pdf_generate generate;
    double sin_theta;
    double cos_theta;
    aabb bbox;
} rotate;

void init_translate_no_pdf(translate *t, void *hittable, fptr_is_hit hit, aabb box, vector3 o);
void init_translate(translate *t, void *hittable, fptr_is_hit hit, hittable_pdf_value v, hittable_pdf_generate g, aabb box, vector3 o);
bool hit_translate(const void *s, ray r, double ray_tmin, double ray_tmax, hit_record *rec);
aabb get_translate_box(const void *s);
double translate_pdf_value(const void *s, const point3 orig, const vector3 dir);
vector3 translate_pdf_generate(const void *s, const point3 orig);


void init_rotate_no_pdf(rotate *r, void *hittable, fptr_is_hit hit, aabb box, double theta);
void init_rotate(rotate *r, void *hittable, fptr_is_hit hit, hittable_pdf_value v, hittable_pdf_generate g, aabb box, double theta);
bool hit_rotate(const void *s, ray r, double ray_tmin, double ray_tmax, hit_record *rec);
aabb get_rotate_box(const void *s);
double rotate_pdf_value(const void *s, const point3 orig, const vector3 dir);
vector3 rotate_pdf_generate(const void *s, const point3 orig);

void init_rotate_z_no_pdf(rotate *r, void *hittable, fptr_is_hit hit, aabb box, double theta);
void init_rotate_z(rotate *r, void *hittable, fptr_is_hit hit, hittable_pdf_value v, hittable_pdf_generate g, aabb box, double theta);
bool hit_rotate_z(const void *s, ray r, double ray_tmin, double ray_tmax, hit_record *rec);
double rotate_pdf_z_value(const void *s, const point3 orig, const vector3 dir);
vector3 rotate_pdf_z_generate(const void *s, const point3 orig);

#endif
