#include "ray.h"

void init_ray(ray *r, point3 origin, vector3 direction){
  copy(&(r->orig), origin);
  copy(&(r->dir), direction);
}

void copy_ray(ray *r, ray to_copy){
  copy(&(r->orig), to_copy.orig);
  copy(&(r->dir), to_copy.dir);
}

point3 at(ray r, double t){
   point3 p;
   copy(&p, r.dir);
   scale(&p, t);
   add_vector(&p, r.orig);
   return p;
}
