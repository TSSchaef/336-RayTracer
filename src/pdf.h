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
    onb uvw;
} pdf;


void init_sphere_pdf(pdf *p);
void init_cosine_pdf(pdf *p, const vector3 w);

double sphere_value(const pdf p, const vector3 v);
vector3 sphere_generate(const pdf p);

double cosine_value(const pdf p, const vector3 v);
vector3 cosine_generate(const pdf p);

#endif
