#include "instance.h"
#include "aabb.h"
#include "util.h"

void init_translate_no_pdf(translate *t, void *h, fptr_is_hit hit, aabb box, vector3 o){
    t->hittable = h;
    t->hit_object = hit;
    copy(&(t->offset), o);
    copy_box(&(t->bbox), box);
    shift_box(&(t->bbox), t->offset);
}

void init_translate(translate *t, void *hittable, fptr_is_hit hit, hittable_pdf_value v, hittable_pdf_generate g, aabb box, vector3 o){
    init_translate_no_pdf(t, hittable, hit, box, o);
    t->value = v;
    t->generate = g;
}

bool hit_translate(const void *s, ray r, double ray_tmin, double ray_tmax, hit_record *rec){
    translate *t = ((translate *)s);
    ray offset_r;
    point3 p;
    copy(&p, t->offset);
    invert(&p);
    add_vector(&p, r.orig);
    init_ray(&offset_r, p, r.dir);

    if(!(*(t->hit_object))(t->hittable, offset_r, ray_tmin, ray_tmax, rec)){
        return false;
    }

    add_vector(&(rec->p), ((translate*)s)->offset);
    return true;
}

aabb get_translate_box(const void *s){
    return ((translate *)s)->bbox;
}

double translate_pdf_value(const void *s, const point3 orig, const vector3 dir){
    const translate *t = ((translate *)s);
    point3 p;
    copy(&p, t->offset);
    invert(&p);
    add_vector(&p, orig);
    return (*(t->value))(t->hittable, p, dir); 
}

vector3 translate_pdf_generate(const void *s, const point3 orig){
    const translate *t = ((translate *)s);
    point3 p;
    copy(&p, t->offset);
    invert(&p);
    add_vector(&p, orig);

    return (*(t->generate))(t->hittable, p); 
}


void init_rotate_no_pdf(rotate *r, void *h, fptr_is_hit hit, aabb box, double theta){
    double rad = DEG_TO_RAD(theta);
    r->sin_theta = sin(rad);
    r->cos_theta = cos(rad);
    r->hit_object = hit;
    r->hittable = h;

    point3 min, max;
    init(&min, DBL_MIN, DBL_MIN, DBL_MIN);
    init(&max, DBL_MAX, DBL_MAX, DBL_MAX);

    int i, j, k, c;
    for(i = 0; i < 2; i++){
        for(j = 0; j < 2; j++){
            for(k = 0; k < 2; k++){
                double x = i*box.x[1] + (1 - i)*box.x[0];
                double y = j*box.y[1] + (1 - j)*box.y[0];
                double z = k*box.z[1] + (1 - k)*box.z[0];

                double nx = r->cos_theta*x + r->sin_theta*z;
                double nz = -1*r->sin_theta*x + r->cos_theta*z;
               
                vector3 test;
                init(&test, nx, y, nz);

                for(c = 0; c < 3; c++){
                   min.e[c] = fmin(min.e[c], test.e[c]); 
                   max.e[c] = fmax(max.e[c], test.e[c]); 
                }
            }
        }
    }

    init_aabb_points(&(r->bbox), min, max);
}

void init_rotate(rotate *r, void *hittable, fptr_is_hit hit, hittable_pdf_value v, hittable_pdf_generate g, aabb box, double theta){
    init_rotate_no_pdf(r, hittable, hit, box, theta);
    r->value = v;
    r->generate = g;
}

double rotate_pdf_value(const void *s, const point3 orig, const vector3 dir){
    const rotate *ro = ((rotate *)s);
    point3 origin;
    init(&origin, ro->cos_theta * orig.e[x] - ro->sin_theta * orig.e[z],
                orig.e[y],
                ro->sin_theta * orig.e[x] + ro->cos_theta * orig.e[z]);

    vector3 direction;
    init(&direction, ro->cos_theta * dir.e[x] - ro->sin_theta * dir.e[z],
                    dir.e[y],
                    ro->sin_theta * dir.e[x] + ro->cos_theta * dir.e[z]);

    return (*ro->value)(ro->hittable, origin, direction);
}

vector3 rotate_pdf_generate(const void *s, const point3 orig){
    const rotate *ro = ((rotate *)s);
    point3 origin;
    init(&origin, ro->cos_theta * orig.e[x] - ro->sin_theta * orig.e[z],
                orig.e[y],
                ro->sin_theta * orig.e[x] + ro->cos_theta * orig.e[z]);

    vector3 vec, ans;
    copy(&vec, (*ro->generate)(ro->hittable, origin));
    init(&ans, ro->cos_theta * vec.e[x] + ro->sin_theta * vec.e[z],
                vec.e[y],
                (-1 * ro->sin_theta * vec.e[x]) + ro->cos_theta * vec.e[z]);

    return ans;
}

bool hit_rotate(const void *s, ray r, double ray_tmin, double ray_tmax, hit_record *rec){
    rotate *ro = ((rotate *)s);

    point3 origin;
    init(&origin, ro->cos_theta * r.orig.e[x] - ro->sin_theta * r.orig.e[z],
                r.orig.e[y],
                ro->sin_theta * r.orig.e[x] + ro->cos_theta * r.orig.e[z]);

    vector3 direction;
    init(&direction, ro->cos_theta * r.dir.e[x] - ro->sin_theta * r.dir.e[z],
                    r.dir.e[y],
                    ro->sin_theta * r.dir.e[x] + ro->cos_theta * r.dir.e[z]);

    ray rotated_r;
    init_ray(&rotated_r, origin, direction);


    if(!(*(ro->hit_object))(ro->hittable, rotated_r, ray_tmin, ray_tmax, rec)){
        return false;
    }

    point3 point;
    init(&point, ro->cos_theta * rec->p.e[x] + ro->sin_theta * rec->p.e[z],
                rec->p.e[y],
                (-1 * ro->sin_theta * rec->p.e[x]) + ro->cos_theta * rec->p.e[z]);

    copy(&(rec->p), point);

    vector3 vec;
    init(&vec, ro->cos_theta * rec->normal.e[x] + ro->sin_theta * rec->normal.e[z],
                rec->normal.e[y],
                (-1 * ro->sin_theta * rec->normal.e[x]) + ro->cos_theta * rec->normal.e[z]);

    copy(&(rec->normal), vec);

    return true;
}

aabb get_rotate_box(const void *s){
    return ((rotate *)s)->bbox;
}

void init_rotate_z_no_pdf(rotate *r, void *h, fptr_is_hit hit, aabb box, double theta){
    double rad = DEG_TO_RAD(theta);
    r->sin_theta = sin(rad);
    r->cos_theta = cos(rad);
    r->hit_object = hit;
    r->hittable = h;

    point3 min, max;
    init(&min, DBL_MIN, DBL_MIN, DBL_MIN);
    init(&max, DBL_MAX, DBL_MAX, DBL_MAX);

    int i, j, k, c;
    for(i = 0; i < 2; i++){
        for(j = 0; j < 2; j++){
            for(k = 0; k < 2; k++){
                double x = i*box.x[1] + (1 - i)*box.x[0];
                double y = j*box.y[1] + (1 - j)*box.y[0];
                double z = k*box.z[1] + (1 - k)*box.z[0];

                double nx = r->cos_theta*x + r->sin_theta*y;
                double ny = -1*r->sin_theta*x + r->cos_theta*y;
               
                vector3 test;
                init(&test, nx, ny, z);

                for(c = 0; c < 3; c++){
                   min.e[c] = fmin(min.e[c], test.e[c]); 
                   max.e[c] = fmax(max.e[c], test.e[c]); 
                }
            }
        }
    }

    init_aabb_points(&(r->bbox), min, max);
}


void init_rotate_z(rotate *r, void *hittable, fptr_is_hit hit, hittable_pdf_value v, hittable_pdf_generate g, aabb box, double theta){
    init_rotate_no_pdf(r, hittable, hit, box, theta);
    r->value = v;
    r->generate = g;
}

bool hit_rotate_z(const void *s, ray r, double ray_tmin, double ray_tmax, hit_record *rec){
    rotate *ro = ((rotate *)s);

    point3 origin;
    init(&origin, ro->cos_theta * r.orig.e[x] - ro->sin_theta * r.orig.e[y],
                ro->sin_theta * r.orig.e[x] + ro->cos_theta * r.orig.e[y],
                r.orig.e[z]);

    vector3 direction;
    init(&direction, ro->cos_theta * r.dir.e[x] - ro->sin_theta * r.dir.e[y],
                    ro->sin_theta * r.dir.e[x] + ro->cos_theta * r.dir.e[y],
                    r.dir.e[z]);

    ray rotated_r;
    init_ray(&rotated_r, origin, direction);


    if(!(*(ro->hit_object))(ro->hittable, rotated_r, ray_tmin, ray_tmax, rec)){
        return false;
    }

    point3 point;
    init(&point, ro->cos_theta * rec->p.e[x] + ro->sin_theta * rec->p.e[y],
                (-1 * ro->sin_theta * rec->p.e[x]) + ro->cos_theta * rec->p.e[y],
                rec->p.e[z]);

    copy(&(rec->p), point);

    vector3 vec;
    init(&vec, ro->cos_theta * rec->normal.e[x] + ro->sin_theta * rec->normal.e[y],
                (-1 * ro->sin_theta * rec->normal.e[x]) + ro->cos_theta * rec->normal.e[y],
                rec->normal.e[y]);

    copy(&(rec->normal), vec);

    return true;
}

double rotate_pdf_z_value(const void *s, const point3 orig, const vector3 dir){
    const rotate *ro = ((rotate *)s);
    point3 origin;
    init(&origin, ro->cos_theta * orig.e[x] - ro->sin_theta * orig.e[y],
            ro->sin_theta * orig.e[x] + ro->cos_theta * orig.e[y],
            orig.e[z]);

    vector3 direction;
    init(&direction, ro->cos_theta * dir.e[x] - ro->sin_theta * dir.e[y],
            ro->sin_theta * dir.e[x] + ro->cos_theta * dir.e[y],
            dir.e[z]);

    return (*ro->value)(ro->hittable, origin, direction);
}

vector3 rotate_pdf_z_generate(const void *s, const point3 orig){
    const rotate *ro = ((rotate *)s);
    point3 origin;
    init(&origin, ro->cos_theta * orig.e[x] - ro->sin_theta * orig.e[y],
            ro->sin_theta * orig.e[x] + ro->cos_theta * orig.e[y],
            orig.e[z]);

    vector3 vec, ans;
    copy(&vec, (*ro->generate)(ro->hittable, origin));
    init(&ans, ro->cos_theta * vec.e[x] + ro->sin_theta * vec.e[y],
            (-1 * ro->sin_theta * vec.e[x]) + ro->cos_theta * vec.e[y],
            vec.e[z]);

    return ans;
}
