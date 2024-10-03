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

double mixture_value(const pdf p, const vector3 v){
    mixture_pdf_data *m = (mixture_pdf_data *) p.data;
    return 0.5 * m->p1->value(*(m->p1), v) + 0.5 * m->p2->value(*(m->p2), v);
}

vector3 mixture_generate(const pdf p){
    mixture_pdf_data *m = (mixture_pdf_data *) p.data;
    if(rnd_double() < 0.5){
        return m->p1->generate(*(m->p1));
    } else {
        return m->p2->generate(*(m->p2));
    }
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

void init_mixture_pdf(pdf *p, pdf *p1, pdf *p2){
    p->value = &mixture_value;
    p->generate = &mixture_generate;
    p->data = malloc(sizeof(mixture_pdf_data));
    ((mixture_pdf_data *)p->data)->p1 = p1;
    ((mixture_pdf_data *)p->data)->p2 = p2;
}

void delete_pdf(pdf *p){
    if(p->data){
        free(p->data);
    }
}

