#ifndef PDF_H
#define PDF_H

#include "onb.h"
#include "hittable_list.h"
#include "util.h"

struct pdf;

typedef double (*pdf_value)(const struct pdf, const vector3);
typedef vector3 (*pdf_generate)(const struct pdf);

typedef struct pdf{
    pdf_value value;
    pdf_generate generate;
    void *data;
} pdf;

typedef struct {
    const hittable_list *objects;
    point3 origin;
} hittable_pdf_data;

typedef struct {
    pdf *p1;
    pdf *p2;
} mixture_pdf_data;

void init_sphere_pdf(pdf *p);
void init_cosine_pdf(pdf *p, const vector3 w);
void init_hittable_pdf(pdf *p, const hittable_list *objects, const point3 origin);
void init_mixture_pdf(pdf *p, pdf *p1, pdf *p2);

void delete_pdf(pdf *p);

double sphere_value(const pdf p, const vector3 v);
vector3 sphere_generate(const pdf p);

double cosine_value(const pdf p, const vector3 v);
vector3 cosine_generate(const pdf p);

double hittable_value(const pdf p, const vector3 v);
vector3 hittable_generate(const pdf p);

double mixture_value(const pdf p, const vector3 v);
vector3 mixture_generate(const pdf p);



#endif
