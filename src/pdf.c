#include "pdf.h"
#include "onb.h"
#include "vector3.h"


double sphere_value(const pdf p, const vector3 v){
    return 1 / (4 * PI);
}

vector3 sphere_generate(const pdf p){
    return random_unit_vector();
}

double cosine_value(const pdf p, const vector3 v){
    vector3 temp;
    copy(&temp, v);
    unit_vector(&temp);
    double cosine_theta = dot(temp, p.uvw.axis[2]);
    return cosine_theta > 0 ? cosine_theta / PI : 0;
}

vector3 cosine_generate(const pdf p){
    return transform(p.uvw, random_cosine_direction());
}

void init_sphere_pdf(pdf *p){
    p->value = &sphere_value;
    p->generate = &sphere_generate;
}

void init_cosine_pdf(pdf *p, const vector3 w){
    p->value = &cosine_value;
    p->generate = &cosine_generate;
    init_axis(&(p->uvw), w);
}

