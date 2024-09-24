#include "vector3.h"
#include "util.h"

inline void init(vector3 *v, double e1, double e2, double e3){
    v->e[0] = e1;
    v->e[1] = e2;
    v->e[2] = e3;
}

inline void copy(vector3 *v, const vector3 w){
    v->e[0] = w.e[0];
    v->e[1] = w.e[1];
    v->e[2] = w.e[2];
}

inline void invert(vector3 *v){
   v->e[0] *= -1;
   v->e[1] *= -1;
   v->e[2] *= -1;
}

inline void add_vector(vector3 *v, const vector3 w){
    v->e[0] += w.e[0];
    v->e[1] += w.e[1];
    v->e[2] += w.e[2];
}

inline void scale(vector3 *v, double t){
    v->e[0] *= t;
    v->e[1] *= t;
    v->e[2] *= t;
}

inline double length(const vector3 v){
    return sqrt(v.e[0]*v.e[0] + v.e[1]*v.e[1] + v.e[2]*v.e[2]);
}

inline double length_squared(const vector3 v){
    return v.e[0]*v.e[0] + v.e[1]*v.e[1] + v.e[2]*v.e[2];
}

inline bool near_zero(const vector3 v){
    double s = 0.00000001;
    return (v.e[0] < s && v.e[0] > -1 * s) && (v.e[1] < s && v.e[1] > -1 * s) && (v.e[2] < s && v.e[2] > -1 * s);
}

inline double dot(const vector3 v, const vector3 w){
    return v.e[0]*w.e[0] + v.e[1]*w.e[1] + v.e[2]*w.e[2];
}

inline vector3 cross(const vector3 u, const vector3 v){
   vector3 ans;
   init(&ans, (u.e[1] * v.e[2]) - (u.e[2] * v.e[1]), (u.e[2] * v.e[0]) - (u.e[0] * v.e[2]), (u.e[0] * v.e[1]) - (u.e[1] * v.e[0]));

   return ans;
}

inline void unit_vector(vector3 *v){
    double l = length(*v);

    v->e[0] /= l;
    v->e[1] /= l;
    v->e[2] /= l;
}

inline vector3 attenuate(const vector3 v, const vector3 w){
    vector3 ans;
    copy(&ans, w);
    ans.e[0] *= v.e[0];
    ans.e[1] *= v.e[1];
    ans.e[2] *= v.e[2];
    return ans;
}

inline vector3 reflect(const vector3 v, const vector3 n){
    vector3 ans;
    copy(&ans, n);
    scale(&ans, -2 * dot(v, n));
    add_vector(&ans, v);
    return ans;
}

inline vector3 refract(const vector3 v, const vector3 n, double etai_over_etat){
    vector3 invV;
    copy(&invV, v);
    invert(&invV);
    double cos_theta = dot(invV, n); 
    cos_theta = (cos_theta > 1.0) ? 1.0 : cos_theta;

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

inline vector3 random_default_vector(){
    vector3 v;
    init(&v, rnd_double(), rnd_double(), rnd_double());
    return v;
}

inline vector3 random_vector(double min, double max){
    vector3 v;
    init(&v, rnd_dbl(min, max), rnd_dbl(min, max), rnd_dbl(min, max));
    return v;
}

inline vector3 random_in_unit_sphere(){
    while(1){
        vector3 v = random_vector(-1, 1);
        if(length_squared(v) < 1)
            return v;
    }
}

inline vector3 random_in_unit_disk(){
    while(1){
        vector3 p;
        init(&p, rnd_dbl(-1, 1), rnd_dbl(-1, 1), 0);
        if(length_squared(p) < 1)
            return p;
    }
}

inline vector3 random_unit_vector(){
    vector3 v = random_in_unit_sphere();
    unit_vector(&v);
    return v;
}

inline vector3 random_on_hemisphere(const vector3 normal){
    vector3 v = random_unit_vector();
    if(dot(v, normal) > 0.0){
        return v;
    } 
    invert(&v);
    return v;
}

inline void print(const vector3 v){
    printf("%lf %lf %lf\n", v.e[0], v.e[1], v.e[2]);
}

static inline double clamp_color(double x){
    if(x < 0.0) return 0.0;
    if(x > 0.999) return 0.999;
    return x;
}

static inline double linear_to_gamma(double x){
    if(x > 0)
        return sqrt(x);
    return 0;
}

void print_color(const color c, uint8_t pixel[3]){
    int rbyte = (int) 256 * clamp_color(linear_to_gamma(c.e[r]));
    int gbyte = (int) 256 * clamp_color(linear_to_gamma(c.e[g])); 
    int bbyte = (int) 256 * clamp_color(linear_to_gamma(c.e[b])); 

    //printf("%d %d %d\n", rbyte, gbyte, bbyte);
    pixel[r] = rbyte;
    pixel[g] = gbyte;
    pixel[b] = bbyte;
}
