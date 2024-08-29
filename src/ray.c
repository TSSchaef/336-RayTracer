#include "ray.h"

void init_ray(ray *r, point3 origin, vector3 direction){
  copy(&(r->orig), origin);
  copy(&(r->dir), direction);
}

point3 at(ray r, double t){
   point3 p;
   copy(&p, r.dir);
   scale(&p, t);
   add_vector(&p, r.orig);
   return p;
}
