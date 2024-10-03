#include "sphere.h"
#include "vector3.h"

aabb get_sphere_box(const void *s){
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

static void get_sphere_uv(point3 p, double *u, double *v){
    double theta = acos(-1 * p.e[y]);
    double phi = atan2(-1 * p.e[z], p.e[x]) + PI;

    *u = phi / (2 * PI);
    *v = theta / PI;
}

bool hit_sphere(const void *s, ray r, double ray_tmin, double ray_tmax, hit_record *rec){
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
    rec->p = at(r, root);
    rec->mat = &(((sphere*)s)->mat);
    vector3 outward_normal;
    copy(&(outward_normal), ((sphere*)s)->center);
    invert(&(outward_normal));
    add_vector(&(outward_normal), rec->p);
    scale(&(outward_normal), 1.0 / ((sphere*)s)->radius);
    set_face_normal(rec, r, outward_normal);
    get_sphere_uv(outward_normal, &(rec->u), &(rec->v));

    return true;
}

double sphere_pdf_value(const void *sph, const point3 orig, const vector3 dir){
    hit_record h;
    sphere *s = (sphere *)sph;
    ray r;
    init_ray(&r, orig, dir);
    if(!hit_sphere(sph, r, 0.001, DBL_MAX, &h)){
        return 0;
    }

    vector3 c;
    copy(&c, orig);
    invert(&c);
    add_vector(&c, s->center);
    double dist_sqr = length_squared(c);
    double value = 1.0 - (s->radius * s->radius/ dist_sqr);
    double cos_theta_max = value > 0 ? sqrt(value) : 0;
    double solid_angle = 2 * PI * (1.0 - cos_theta_max);

    return 1 / solid_angle;
}

static vector3 generate_to_sphere(double radius, double dist_sqr){
    double r1 = rnd_double();
    double r2 = rnd_double();
    double value = 1.0 - (radius * radius / dist_sqr);
    double z = value > 0 ? sqrt(value) : 0;
    z = 1 + r2*(z - 1);

    double phi = 2 * PI * r1;
    double x = cos(phi) * sqrt(1 - (z*z));
    double y = sin(phi) * sqrt(1 - (z*z));

    vector3 v;
    init(&v, x, y, z);
    return v;
}

vector3 sphere_pdf_generate(const void *sph, const point3 orig){
    sphere *s = (sphere *)sph;

    vector3 c;
    copy(&c, orig);
    invert(&c);
    add_vector(&c, s->center);
    double dist_sqr = length_squared(c);

    onb uvw;
    init_axis(&uvw, c);
    return transform(uvw, generate_to_sphere(s->radius, dist_sqr));
}
