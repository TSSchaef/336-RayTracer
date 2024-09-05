#include "texture.h"

void copy_texture(texture *t, texture to_copy){
    copy(&(t->albedo), to_copy.albedo);
    copy(&(t->second_color), to_copy.second_color);
    t->inv_scale = to_copy.inv_scale;
    t->value = to_copy.value;
}

color solid_value(texture *t, double u, double v, point3 p){
    return t->albedo;
}

void init_solid_txt(texture *t, color a){
    copy(&(t->albedo), a);
    init(&(t->second_color), 0, 0, 0);
    t->inv_scale = -1;
    t->value = &solid_value;
}

void init_solid_txt_rgb(texture *t, double r, double g, double b){
    init(&(t->albedo), r, g, b);
    init(&(t->second_color), 0, 0, 0);
    t->inv_scale = -1;
    t->value = &solid_value;
}

int my_floor(double x){
    if(x > 0){
        return (int) x;
    }

    x -= 1;
    return (int) x;
}

color checker_value(texture *t, double u, double v, point3 p){
   int xf = my_floor(t->inv_scale * p.e[x]); 
   int yf = my_floor(t->inv_scale * p.e[y]); 
   int zf = my_floor(t->inv_scale * p.e[z]); 

   return ((xf + yf + zf) % 2 == 0) ? t->albedo : t->second_color;
}

void init_checker_txt(texture *t, double scale, color even, color odd){
    copy(&(t->albedo), even);
    copy(&(t->second_color), odd);
    t->inv_scale = 1.0 / scale;
    t->value = &checker_value;
}
