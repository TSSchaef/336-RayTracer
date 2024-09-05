#ifndef TEXTURE_H
#define TEXTURE_H

#include "util.h"

struct texture;
typedef color (*get_txt_value)(struct texture *, double, double, point3);

typedef struct texture{
   color albedo; 
   color second_color;
   double inv_scale;
   get_txt_value value;
} texture;

void copy_texture(texture *t, texture to_copy);

color solid_value(texture *t, double u, double v, point3 p);
void init_solid_txt(texture *t, color a);
void init_solid_txt_rgb(texture *t, double r, double g, double b);

color checker_value(texture *t, double u, double v, point3 p);
void init_checker_txt(texture *t, double scale, color even, color odd);

#endif
