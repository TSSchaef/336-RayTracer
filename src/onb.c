#include "onb.h"
#include "vector3.h"

inline void init_axis(onb *o, const vector3 n){
    copy(&o->axis[2], n);  
    unit_vector(&o->axis[2]);

    vector3 a;
    if(o->axis[2].e[x] > 0.9 || o->axis[2].e[x] < -0.9){
        init(&a, 0, 1, 0);
    } else {
        init(&a, 1, 0, 0);
    }

    copy(&o->axis[1], cross(o->axis[2], a));
    unit_vector(&o->axis[1]);
    copy(&o->axis[0], cross(o->axis[2], o->axis[1]));
    unit_vector(&o->axis[0]);
}

inline void copy_axis(onb *o, const onb to_copy){
    copy(&o->axis[0], to_copy.axis[0]);
    copy(&o->axis[1], to_copy.axis[1]);
    copy(&o->axis[2], to_copy.axis[2]);
}

inline vector3 transform(const onb b, const vector3 v){
    vector3 t;
    onb temp;
    copy_axis(&temp, b);
    
    scale(&temp.axis[0], v.e[0]);
    copy(&t, temp.axis[0]);

    scale(&temp.axis[1], v.e[1]);
    add_vector(&t, temp.axis[1]);

    scale(&temp.axis[2], v.e[2]);
    add_vector(&t, temp.axis[2]);

    return t;
}
