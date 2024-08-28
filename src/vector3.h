#ifndef VEC3_H
#define VEC3_H

#include <math.h>
#include <stdio.h>

typedef struct vector3{
    double e[3];
} vector3, color, point3;

enum {
    x, y, z
};

enum{
    r, g, b
};

void init(vector3 *v, double e1, double e2, double e3);
void copy(vector3 *v, vector3 w);

void invert(vector3 *v);

void add_vector(vector3 *v, vector3 w);
void scale(vector3 *v, double t);
double length(vector3 v);
double length_squared(vector3 v);

double dot(vector3 v, vector3 w);
void unit_vector(vector3 *v);

void print(vector3 v);
void print_color(color c);

#endif
