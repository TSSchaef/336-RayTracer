#include "material.h"
#include "onb.h"
#include "util.h"
#include "vector3.h"

color non_emitting(const material *m, const ray r_in, const hit_record rec, double u, double v, point3 p){
    color black;
    init(&black, 0, 0, 0);
    return black;
}

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
    h->u = to_copy.u;
    h->v = to_copy.v;
    h->front_face = to_copy.front_face;
    copy_material(&(h->mat), to_copy.mat);
}

void copy_material(material *m, material to_copy){
    copy(&(m->albedo), to_copy.albedo);
    m->fuzz = to_copy.fuzz;
    m->scatter_func = to_copy.scatter_func;
    m->emit = to_copy.emit;
    m->pdf = to_copy.pdf;
    copy_texture(&(m->tex), to_copy.tex);
}

bool lambertian_scatter(ray ray_in, 
        struct hit_record *rec, color *attenuation, ray *ray_out, double *pdf){
    onb uvw;
    init_axis(&uvw, rec->normal);
    vector3 dir = transform(uvw, random_cosine_direction());
    unit_vector(&dir);

    ray r;
    init_ray(&r, rec->p, dir);
    copy_ray(ray_out, r);
    copy(attenuation, (*(rec->mat.tex.value))(&(rec->mat.tex), rec->u, rec->v, rec->p));
    scale(&dir, 1 / PI);
    *pdf = dot(uvw.axis[2], dir);
    return true;
}

double lambertian_pdf(const ray r_in, const hit_record rec, const ray r_out){
    vector3 temp;
    copy(&temp, r_out.dir);
    unit_vector(&temp);
    double cos_theta = dot(rec.normal, temp);
    return cos_theta < 0 ? 0 : cos_theta / PI;
}

void init_lambertian(material *m, color a){
    copy(&(m->albedo), a);
    m->fuzz = 0;
    m->scatter_func = &lambertian_scatter;
    m->emit = &non_emitting;
    m->pdf = &lambertian_pdf;
    texture t;
    init_solid_tex(&t, a);
    copy_texture(&(m->tex), t);
}

void init_lambertian_tex(material *m, texture t){
    init(&(m->albedo), 0, 0, 0);
    m->fuzz = 0;
    m->scatter_func = &lambertian_scatter;
    m->emit = &non_emitting;
    m->pdf = &lambertian_pdf;
    copy_texture(&(m->tex), t);
}

bool isotropic_scatter(ray ray_in, 
        struct hit_record *rec, color *attenuation, ray *ray_out, double* pdf){
    init_ray(ray_out, rec->p, random_unit_vector());  
    copy(attenuation, (*(rec->mat.tex.value))(&(rec->mat.tex), rec->u, rec->v, rec->p));
    *pdf = 1 / (4 * PI);
    return true;
}

double isotropic_pdf(const ray r_in, const hit_record rec, const ray r_out){
    return 1 / (4 * PI);
}

void init_isotropic(material *m, color a){
    init(&(m->albedo), 0, 0, 0);
    m->fuzz = 0;
    m->scatter_func = &isotropic_scatter;
    m->emit = &non_emitting;
    m->pdf = &isotropic_pdf;
    texture t;
    init_solid_tex(&t, a);
    copy_texture(&(m->tex), t);
}

void init_isotropic_tex(material *m, texture t){
    init(&(m->albedo), 0, 0, 0);
    m->fuzz = 0;
    m->scatter_func = &isotropic_scatter;
    m->emit = &non_emitting;
    copy_texture(&(m->tex), t);
}

bool metal_scatter(ray ray_in, 
        struct hit_record *rec, color *attenuation, ray *ray_out, double *pdf){
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
    m->emit = &non_emitting;
    texture t;
    init_solid_tex_rgb(&t, 0, 0, 0);
    copy_texture(&(m->tex), t);
}

double reflectance(double cosine, double refraction_index) {
    //Using Schlick's approximation
    double r0 = (1 - refraction_index) / (1 + refraction_index);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

bool dielectric_scatter(ray ray_in, 
        struct hit_record *rec, color *attenuation, ray *ray_out, double *pdf){
   //using fuzz as refraction index and albedo as white
    copy(attenuation, rec->mat.albedo);

    double ri = rec->front_face ? (1.0/ rec->mat.fuzz) : rec->mat.fuzz;

    vector3 unit_dir;
    copy(&unit_dir, ray_in.dir);
    unit_vector(&unit_dir);

    invert(&unit_dir);
    double cos_theta = dot(unit_dir, rec->normal);
    invert(&unit_dir);
    cos_theta = (cos_theta > 1.0) ? 1.0 : cos_theta;
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_refract = (ri * sin_theta) > 1.0;

    vector3 direction;
    if(cannot_refract || reflectance(cos_theta, ri) > rnd_double()){
        copy(&direction, reflect(unit_dir, rec->normal));
    } else {
        copy(&direction, refract(unit_dir, rec->normal, ri));
    }

    init_ray(ray_out, rec->p, direction);
    return true;
}

void init_dielectric(material *m, double f){
    init(&(m->albedo), 1.0, 1.0, 1.0);
    m->fuzz = f; 
    m->scatter_func = &dielectric_scatter;
    m->emit = &non_emitting;
    texture t;
    init_solid_tex_rgb(&t, 0, 0, 0);
    copy_texture(&(m->tex), t);
}

bool non_scattering(ray ray_in, 
        struct hit_record *rec, color *attenuation, ray *ray_out, double *pdf){
    return false;
}

color emit(const material *m, const ray r_in, const hit_record rec, double u, double v, point3 p){
    if(!rec.front_face){
        color black;
        init(&black, 0, 0, 0);
        return black;
    }
    return (*(m->tex.value))(&(m->tex), u, v, p);
}

void init_diffuse_light_tex(material *m, texture *t){
    init(&(m->albedo), 1.0, 1.0, 1.0);
    m->fuzz = 0; 
    m->scatter_func = &non_scattering;
    m->emit = &emit;
    copy_texture(&(m->tex), *t);
}

void init_diffuse_light(material *m, color c){
    init(&(m->albedo), 1.0, 1.0, 1.0);
    m->fuzz = 0; 
    m->scatter_func = &non_scattering;
    m->emit = &emit;
    texture t;
    init_solid_tex(&t, c); 
    copy_texture(&(m->tex), t);
}
