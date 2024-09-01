#include "vector3.h"
#include "util.h"

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

bool near_zero(vector3 v){
    double s = 0.00000001;
    return (v.e[0] < s && v.e[0] > -1 * s) && (v.e[1] < s && v.e[1] > -1 * s) && (v.e[2] < s && v.e[2] > -1 * s);
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

vector3 attenuate(vector3 v, vector3 w){
    vector3 ans;
    copy(&ans, w);
    ans.e[0] *= v.e[0];
    ans.e[1] *= v.e[1];
    ans.e[2] *= v.e[2];
    return ans;
}

vector3 reflect(vector3 v, vector3 n){
    vector3 ans;
    copy(&ans, n);
    scale(&ans, -2 * dot(v, n));
    add_vector(&ans, v);
    return ans;
}

vector3 refract(vector3 v, vector3 n, double etai_over_etat){
    invert(&v);
    double cos_theta = dot(v, n); 
    cos_theta = (cos_theta > 1.0) ? 1.0 : cos_theta;
    invert(&v);

    vector3 r_out_perp;
    copy(&r_out_perp, n);
    scale(&r_out_perp, cos_theta);
    add_vector(&r_out_perp, v);
    scale(&r_out_perp, etai_over_etat);

    double num = 1.0 - length_squared(r_out_perp);
    num = (num < 0) ? -1 * num : num;
    num = -1 * sqrt(num);
    vector3 r_out_parallel;
    copy(&r_out_parallel, n);
    scale(&r_out_parallel, num);

    add_vector(&r_out_perp, r_out_parallel);

    return r_out_perp;
}

vector3 random_default_vector(){
    vector3 v;
    init(&v, RAND_DOUBLE, RAND_DOUBLE, RAND_DOUBLE);
    return v;
}

vector3 random_vector(double min, double max){
    vector3 v;
    init(&v, RAND_DBL(min, max), RAND_DBL(min, max), RAND_DBL(min, max));
    return v;
}

vector3 random_in_unit_sphere(){
    while(1){
        vector3 v = random_vector(-1, 1);
        if(length_squared(v) < 1)
            return v;
    }
}

vector3 random_unit_vector(){
    vector3 v = random_in_unit_sphere();
    unit_vector(&v);
    return v;
}

vector3 random_on_hemisphere(vector3 normal){
    vector3 v = random_unit_vector();
    if(dot(v, normal) > 0.0){
        return v;
    } 
    invert(&v);
    return v;
}

void print(vector3 v){
    printf("%lf %lf %lf\n", v.e[0], v.e[1], v.e[2]);
}

double clamp_color(double x){
    if(x < 0.0) return 0.0;
    if(x > 0.999) return 0.999;
    return x;
}

double linear_to_gamma(double x){
    if(x > 0)
        return sqrt(x);
    return 0;
}

void print_color(color c){
    int rbyte = (int) 256 * clamp_color(linear_to_gamma(c.e[r]));
    int gbyte = (int) 256 * clamp_color(linear_to_gamma(c.e[g])); 
    int bbyte = (int) 256 * clamp_color(linear_to_gamma(c.e[b])); 

    printf("%d %d %d\n", rbyte, gbyte, bbyte);
}
