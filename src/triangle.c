#include "triangle.h"

static void set_bbox(triangle *t){
    point3 min, max;
    
    init(&min, fmin(t->a.e[x], fmin(t->b.e[x], t->c.e[x])), fmin(t->a.e[y], fmin(t->b.e[y], t->c.e[y])), fmin(t->a.e[z], fmin(t->b.e[z], t->c.e[z])));

    init(&max, fmax(t->a.e[x], fmax(t->b.e[x], t->c.e[x])), fmax(t->a.e[y], fmax(t->b.e[y], t->c.e[y])), fmax(t->a.e[z], fmax(t->b.e[z], t->c.e[z])));

    init_aabb_points(&(t->bbox), min, max);
}

/*void init_triangle_norm(triangle *t, point3 a, point3 b, point3 c, vector3 n, material mat){
    copy(&(t->a), a);  
    copy(&(t->b), b);  
    copy(&(t->c), c);  
    copy(&(t->normal), n);

    //printf("Triangle: \n");
    //print(t->a);
    //print(t->b);
    //print(t->c);
    //print(t->normal);
    //printf("\n \n");

    t->D = dot(t->normal, t->normal);

    copy_material(&(t->mat), mat);
    set_bbox(t);
}*/

void init_triangle(triangle *q, point3 a, point3 b, point3 c, material matc){
    copy(&(q->a), a);  
    copy(&(q->b), b);  
    copy(&(q->c), c);  

    point3 invA, ba, ca;
    copy(&invA, a);
    invert(&invA);

    copy(&(ba), b);  
    add_vector(&(ba), invA);
    copy(&(ca), c);  
    add_vector(&(ca), invA);
    
    copy(&(q->normal), cross(ba, ca));

    q->D = dot(q->normal, q->normal);

    copy_material(&(q->mat), matc);
    set_bbox(q);
}

bool hit_triangle(const void *q, ray r, double ray_tmin, double ray_tmax, hit_record *rec){
    triangle *qu = ((triangle *) q);
    double denom = dot(qu->normal, r.dir);
    
    if(denom < 1e-8 && denom > -1e-8) return false;
    
    double d = -1 * dot(qu->normal, qu->a);

    double t = -1 * (dot(qu->normal, r.orig) + d) / denom;
    if(t < ray_tmin || t > ray_tmax) return false;

    point3 intersection;
    copy(&intersection, at(r, t));

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
    rec->mat = &(qu->mat);
    set_face_normal(rec, r, qu->normal);

    return true;
}

aabb get_triangle_box(const void *s){
    return ((triangle *)s)->bbox;
}
