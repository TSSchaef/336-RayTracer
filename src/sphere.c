#include "sphere.h"

void init_sphere(sphere *s, point3 center, double radius){
    copy(&(s->center), center);
    s->radius = (radius > 0) ? radius : 0;
}

bool hit_sphere(sphere s, ray r, double ray_tmin, double ray_tmax, hit_record *rec){
    vector3 oc;
    copy(&oc, r.orig);
    invert(&oc);
    add_vector(&oc, s.center);

    double a = length_squared(r.dir);
    double h = dot(r.dir, oc);
    double c = length_squared(oc) - s.radius*s.radius;

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
    rec->p = at(r, root);
    vector3 outward_normal;
    copy(&(outward_normal), s.center);
    invert(&(outward_normal));
    add_vector(&(outward_normal), rec->p);
    scale(&(outward_normal), 1.0 / s.radius);
    set_face_normal(rec, r, outward_normal);

    return true;
}
