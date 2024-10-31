#ifndef VEC3_H
#define VEC3_H

#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

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
void copy(vector3 *v, const vector3 w);

void invert(vector3 *v);

void add_vector(vector3 *v, const vector3 w);
void scale(vector3 *v, double t);
double length(const vector3 v);
double length_squared(const vector3 v);
bool near_zero(const vector3 v);
double illuminance(const color v);

vector3 attenuate(const vector3 v, const vector3 w);
vector3 reflect(const vector3 v, const vector3 n);
vector3 refract(const vector3 v, const vector3 n, double etai_over_etat);

double dot(const vector3 v, const vector3 w);
vector3 cross(const vector3 v, const vector3 w);
void unit_vector(vector3 *v);

vector3 random_in_unit_sphere();
vector3 random_in_unit_disk();
vector3 random_unit_vector();
vector3 random_on_hemisphere(const vector3 normal);
vector3 random_default_vector();
vector3 random_vector(double min, double max);
vector3 random_cosine_direction();

void print(const vector3 v);
void print_color(const color c, uint8_t pixel[3]);

#endif
