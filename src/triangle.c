#include "triangle.h"

static void set_bbox(triangle *t){
    point3 min, max;

    init(&min, fmin(t->a.e[x], fmin(t->b.e[x], t->c.e[x])), fmin(t->a.e[y], fmin(t->b.e[y], t->c.e[y])), fmin(t->a.e[z], fmin(t->b.e[z], t->c.e[z])));

    init(&max, fmax(t->a.e[x], fmax(t->b.e[x], t->c.e[x])), fmax(t->a.e[y], fmax(t->b.e[y], t->c.e[y])), fmax(t->a.e[z], fmax(t->b.e[z], t->c.e[z])));

    init_aabb_points(&(t->bbox), min, max);
}

void init_triangle(triangle *q, point3 a, point3 b, point3 c, material matc){
    copy(&(q->a), a);  
    copy(&(q->b), b);  
    copy(&(q->c), c);  
    
    invert(&(a));
    add_vector(&(b), a);
    add_vector(&(c), a);

    copy(&(q->normal), cross(b, a));

    q->D = dot(q->normal, q->normal);

    copy_material(&(q->mat), matc);
    set_bbox(q);
}

bool hit_triangle(void *q, ray r, double ray_tmin, double ray_tmax, hit_record *rec){
    triangle *qu = ((triangle *) q);
    double denom = dot(qu->normal, r.dir);
    
    if(denom < 1e-8 && denom > -1e-8) return false;
    
    point3 temp;
    copy(&temp, r.orig);
    invert(&temp);
    add_vector(&temp, qu->a);

    double t = dot(qu->normal, temp) / denom;
    if(t < ray_tmin || t > ray_tmax) return false;

    point3 intersection = at(r, t);

    point3 bary;
    point3 negA, negB, negC;
    point3 d1, d2;
    point3 na, nb, nc;

    copy(&negA, qu->a);
    copy(&negB, qu->b);
    copy(&negC, qu->c);
    invert(&negA);
    invert(&negB);
    invert(&negC);

    copy(&d1, qu->c);
    copy(&d2, intersection);
    add_vector(&d1, negB);
    add_vector(&d2, negB);
    copy(&na, cross(d1, d2));

    copy(&d1, qu->a);
    copy(&d2, intersection);
    add_vector(&d1, negC);
    add_vector(&d2, negC);
    copy(&nb, cross(d1, d2));

    copy(&d1, qu->b);
    copy(&d2, intersection);
    add_vector(&d1, negA);
    add_vector(&d2, negA);
    copy(&nc, cross(d1, d2));

    init(&bary, dot(qu->normal, na) / qu->D, dot(qu->normal, nb) / qu->D, dot(qu->normal, nc) / qu->D);

    if(bary.e[0] < 0 || bary.e[1] < 0 || bary.e[2] < 0){
        return false;
    }

    rec->t = t;
    copy(&(rec->p), intersection);
    copy_material(&(rec->mat), qu->mat);
    set_face_normal(rec, r, qu->normal);

    return true;
}

aabb get_triangle_box(void *s){
    return ((triangle *)s)->bbox;
}
