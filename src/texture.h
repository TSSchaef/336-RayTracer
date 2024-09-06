#ifndef TEXTURE_H
#define TEXTURE_H

#include "util.h"
#include "image.h"

struct texture;
typedef color (*get_tex_value)(struct texture *, double, double, point3);

typedef struct texture{
    void *pattern;
    get_tex_value value;
} texture;

typedef struct {
    color albedo;
} solid;

typedef struct {
    texture even;
    texture odd;
    double inv_scale;
} checker;

typedef struct {
    image img;
} image_tex;

void copy_texture(texture *t, texture to_copy);

color solid_value(texture *t, double u, double v, point3 p);
void init_solid_tex(texture *t, color a);
void init_solid_tex_rgb(texture *t, double r, double g, double b);

color checker_value(texture *t, double u, double v, point3 p);
void init_checker(texture *t, double scale, texture t1, texture t2);
void init_checker_tex(texture *t, double scale, color even, color odd);

void init_image_tex(texture *t, const char *filename);
color image_value(texture *t, double u, double v, point3 p);

void delete_image_tex(texture *t);

void delete_texture(texture *t);

#endif
