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
    onb *uvw = ((onb *) p.data);
    vector3 temp;
    copy(&temp, v);
    unit_vector(&temp);
    double cosine_theta = dot(temp, uvw->axis[2]);
    return cosine_theta > 0 ? cosine_theta / PI : 0;
}

vector3 cosine_generate(const pdf p){
    onb *uvw = ((onb *) p.data);
    return transform(*uvw, random_cosine_direction());
}

double hittable_value(const pdf p, const vector3 v){
    hittable_pdf_data *info = (hittable_pdf_data *) p.data;
    return hittable_list_pdf_value(info->objects, info->origin, v);
}

vector3 hittable_generate(const pdf p){
    hittable_pdf_data *info = (hittable_pdf_data *) p.data;
    return hittable_list_pdf_generate(info->objects, info->origin);
}

void init_sphere_pdf(pdf *p){
    p->value = &sphere_value;
    p->generate = &sphere_generate;
    p->data = NULL;
}

void init_cosine_pdf(pdf *p, const vector3 w){
    p->value = &cosine_value;
    p->generate = &cosine_generate;
    p->data = malloc(sizeof(onb));
    init_axis((onb *) p->data, w);
}

void init_hittable_pdf(pdf *p, const hittable_list *objs, const point3 orig){
    p->value = &hittable_value;
    p->generate = &hittable_generate;
    p->data = malloc(sizeof(hittable_pdf_data));
    ((hittable_pdf_data *)p->data)->objects = objs;
    copy(&((hittable_pdf_data *)p->data)->origin, orig);
}

void delete_pdf(pdf *p){
    if(p->data){
        free(p->data);
    }
}

