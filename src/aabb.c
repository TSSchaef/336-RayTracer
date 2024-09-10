#include "aabb.h"

static void pad_mins(aabb *box){
   double delta = 0.001;
   if(box->x[1] - box->x[0] < delta){
        box->x[1] += delta / 2;
        box->x[0] -= delta / 2;
   }
   if(box->y[1] - box->y[0] < delta){
        box->y[1] += delta / 2;
        box->y[0] -= delta / 2;
   }
   if(box->z[1] - box->z[0] < delta){
        box->z[1] += delta / 2;
        box->z[0] -= delta / 2;
   }
}

void init_aabb(aabb *box, double x0, double x1, double y0, double y1,
                double z0, double z1){
    
    box->x[0] = x0;
    box->x[1] = x1;
    box->y[0] = y0;
    box->y[1] = y1;
    box->z[0] = z0;
    box->z[1] = z1;
    pad_mins(box);
}

void init_aabb_points(aabb *box, point3 a, point3 b){
    box->x[0] = (a.e[0] <= b.e[0]) ? a.e[0] : b.e[0];
    box->x[1] = (a.e[0] <= b.e[0]) ? b.e[0] : a.e[0]; 
    box->y[0] = (a.e[1] <= b.e[1]) ? a.e[1] : b.e[1];  
    box->y[1] = (a.e[1] <= b.e[1]) ? b.e[1] : a.e[1];
    box->z[0] = (a.e[2] <= b.e[2]) ? a.e[2] : b.e[2]; 
    box->z[1] = (a.e[2] <= b.e[2]) ? b.e[2] : a.e[2];  
    pad_mins(box);
}

void add_boxes(aabb *box, aabb add){
    box->x[0] = (box->x[0] <= add.x[0]) ?  box->x[0] : add.x[0];
    box->x[1] = (box->x[1] >= add.x[1]) ?  box->x[1] : add.x[1];
    box->y[0] = (box->y[0] <= add.y[0]) ?  box->y[0] : add.y[0];
    box->y[1] = (box->y[1] >= add.y[1]) ?  box->y[1] : add.y[1];
    box->z[0] = (box->z[0] <= add.z[0]) ?  box->z[0] : add.z[0];
    box->z[1] = (box->z[1] >= add.z[1]) ?  box->z[1] : add.z[1];
}

void copy_box(aabb *box, aabb to_copy){
    box->x[0] = to_copy.x[0];
    box->x[1] = to_copy.x[1];
    box->y[0] = to_copy.y[0];
    box->y[1] = to_copy.y[1];
    box->z[0] = to_copy.z[0];
    box->z[1] = to_copy.z[1];
}

int longest_axis(aabb box){
    double x_size, y_size, z_size;
    x_size = box.x[1] - box.x[0];
    y_size = box.y[1] - box.y[0];
    z_size = box.z[1] - box.z[0];

    if(x_size > y_size){
        return (x_size > z_size) ? 0 : 2;
    } else {
        return (y_size > z_size) ? 1 : 2;
    }
}

bool hit_box(void *box, ray r, double ray_tmin, double ray_tmax){
    double t0 = (((aabb*)box)->x[0] - r.orig.e[x]) * (1.0 / r.dir.e[x]);
    double t1 = (((aabb*)box)->x[1] - r.orig.e[x]) * (1.0 / r.dir.e[x]);

    if(t0 < t1) {
        if(t0 > ray_tmin) ray_tmin = t0;
        if(t1 < ray_tmax) ray_tmax = t1;
    } else {
        if(t1 > ray_tmin) ray_tmin = t1;
        if(t0 < ray_tmax) ray_tmax = t0;
    }

    if(ray_tmax <= ray_tmin)
        return false;

    t0 = (((aabb*)box)->y[0] - r.orig.e[y]) * (1.0 / r.dir.e[y]);
    t1 = (((aabb*)box)->y[1] - r.orig.e[y]) * (1.0 / r.dir.e[y]);

    if(t0 < t1) {
        if(t0 > ray_tmin) ray_tmin = t0;
        if(t1 < ray_tmax) ray_tmax = t1;
    } else {
        if(t1 > ray_tmin) ray_tmin = t1;
        if(t0 < ray_tmax) ray_tmax = t0;
    }

    if(ray_tmax <= ray_tmin)
        return false;

    t0 = (((aabb*)box)->z[0] - r.orig.e[z]) * (1.0 / r.dir.e[z]);
    t1 = (((aabb*)box)->z[1] - r.orig.e[z]) * (1.0 / r.dir.e[z]);

    if(t0 < t1) {
        if(t0 > ray_tmin) ray_tmin = t0;
        if(t1 < ray_tmax) ray_tmax = t1;
    } else {
        if(t1 > ray_tmin) ray_tmin = t1;
        if(t0 < ray_tmax) ray_tmax = t0;
    }

    if(ray_tmax <= ray_tmin)
        return false;

    return true;
}

void shift_box(aabb *box, vector3 offset){  
    box->x[0] += offset.e[x];
    box->x[1] += offset.e[x];
    box->y[0] += offset.e[y];
    box->y[1] += offset.e[y];
    box->z[0] += offset.e[z];
    box->z[1] += offset.e[z];
}
