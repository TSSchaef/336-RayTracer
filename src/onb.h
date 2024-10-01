#ifndef ONB_H
#define ONB_H

#include "vector3.h"

typedef struct {
   vector3 axis[3];
} onb;

void init_axis(onb *o, const vector3 n);
void copy_axis(onb *o, const onb to_copy);
vector3 transform(const onb b, const vector3 v);

#endif
