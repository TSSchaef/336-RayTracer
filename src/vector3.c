#include "vector3.h"

void init(vector3 *v, double e1, double e2, double e3){
    v->e[0] = e1;
    v->e[1] = e2;
    v->e[2] = e3;
}

void copy(vector3 *v, vector3 w){
    v->e[0] = w.e[0];
    v->e[1] = w.e[1];
    v->e[2] = w.e[2];
}

void invert(vector3 *v){
   v->e[0] *= -1;
   v->e[1] *= -1;
   v->e[2] *= -1;
}

void add_vector(vector3 *v, vector3 w){
    v->e[0] += w.e[0];
    v->e[1] += w.e[1];
    v->e[2] += w.e[2];
}

void scale(vector3 *v, double t){
    v->e[0] *= t;
    v->e[1] *= t;
    v->e[2] *= t;
}

double length(vector3 v){
    return sqrt(v.e[0]*v.e[0] + v.e[1]*v.e[1] + v.e[2]*v.e[2]);
}

double length_squared(vector3 v){
    return v.e[0]*v.e[0] + v.e[1]*v.e[1] + v.e[2]*v.e[2];
}

double dot(vector3 v, vector3 w){
    return v.e[0]*w.e[0] + v.e[1]*w.e[1] + v.e[2]*w.e[2];
}

void unit_vector(vector3 *v){
    double l = length(*v);

    v->e[0] /= l;
    v->e[1] /= l;
    v->e[2] /= l;
}

void print(vector3 v){
    printf("%lf %lf %lf\n", v.e[0], v.e[1], v.e[2]);
}

void print_color(color c){
    int rbyte = (int) 255.999 * c.e[r];
    int gbyte = (int) 255.999 * c.e[g];
    int bbyte = (int) 255.999 * c.e[b];

    printf("%d %d %d\n", rbyte, gbyte, bbyte);
}
