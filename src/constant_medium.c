#include "constant_medium.h"

void init_constant_medium(constant_medium *s, void *h, 
        fptr_is_hit hit, double density, aabb box, color albedo){
    copy_box(&(s->bbox), box);
    s->hittable = h;
    s->hit_object = hit;
    s->neg_inv_density = -1.0 / density;
    init_isotropic(&(s->phase_func), albedo);
}

void init_constant_medium_tex(constant_medium *s, void *h, 
        fptr_is_hit hit, double density, aabb box, texture tex){
    copy_box(&(s->bbox), box);
    s->hittable = h;
    s->hit_object = hit;
    s->neg_inv_density = -1.0 / density;
    init_isotropic_tex(&(s->phase_func), tex);
}

bool hit_constant_medium(void *c, ray r, double ray_tmin, double ray_tmax, hit_record *rec){
    hit_record r1, r2;
    
    constant_medium *m = ((constant_medium *)c);

    if(!(*(m->hit_object))(m->hittable, r, DBL_MIN, DBL_MAX, &r1)){
        return false;
    }

    if(!(*(m->hit_object))(m->hittable, r, r1.t + 0.0001, DBL_MAX, &r2)){
        return false;
    }

    if(r1.t >= r2.t){
        return false;
    }

    if(r1.t < 0){
        r1.t = 0;
    }

    double ray_length = length(r.dir);
    double dist_in_medium = (r2.t - r1.t) * ray_length;
    double hit_dist = m->neg_inv_density * log(RAND_DOUBLE);

    if(hit_dist > dist_in_medium){
        return false;
    }

    rec->t = r1.t + hit_dist / ray_length;
    rec->p = at(r, rec->t);

    init(&(rec->normal), 1, 0, 0);
    rec->front_face = true;
    copy_material(&(rec->mat), m->phase_func);

    return true;
}

aabb get_constant_medium_box(void *s){
    return ((constant_medium *)s)->bbox;
}
