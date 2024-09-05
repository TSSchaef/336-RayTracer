#include "sphere.h"

aabb get_sphere_box(void *s){
    return ((sphere*)s)->bbox;
}

void init_sphere(sphere *s, point3 center, double radius, material m){
    copy(&(s->center), center);
    s->radius = (radius > 0) ? radius : 0;

    vector3 rvec, corner1, corner2;
    init(&rvec, radius, radius, radius);
    copy(&corner2, center);
    add_vector(&corner2, rvec);
    copy(&corner1, center);
    invert(&rvec);
    add_vector(&corner1, rvec);

    init_aabb_points(&(s->bbox), corner1, corner2);

    copy_material(&(s->mat), m);
}

bool hit_sphere(void *s, ray r, double ray_tmin, double ray_tmax, hit_record *rec){
    vector3 oc;
    
    copy(&oc, r.orig);
    invert(&oc);
    add_vector(&oc, ((sphere*)s)->center);

    double a = length_squared(r.dir);
    double h = dot(r.dir, oc);
    double c = length_squared(oc) - ((sphere*)s)->radius*((sphere*)s)->radius;

    double discriminant = h*h - a*c;
    //checking if the ray intersects the sphere
    if(discriminant < 0){
        return false;
    }

    double sqrtd = sqrt(discriminant);

    double root = (h - sqrtd) / a;
    //checking if ray collision is over or under the max/min allowed
    if(root <= ray_tmin || ray_tmax <= root) {
        root = (h + sqrtd) / a;
        if(root <= ray_tmin || ray_tmax <= root) {
            return false;
        }
    }

    //placing information about ray collision into hit record
    rec->t = root;
    rec->u = 0;
    rec->v = 0;
    rec->p = at(r, root);
    copy_material(&(rec->mat), ((sphere*)s)->mat);
    vector3 outward_normal;
    copy(&(outward_normal), ((sphere*)s)->center);
    invert(&(outward_normal));
    add_vector(&(outward_normal), rec->p);
    scale(&(outward_normal), 1.0 / ((sphere*)s)->radius);
    set_face_normal(rec, r, outward_normal);

    return true;
}

