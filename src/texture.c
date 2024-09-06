#include "texture.h"

void delete_texture(texture *t){
    free(t->pattern);
}

void copy_texture(texture *t, texture to_copy){
    t->pattern = to_copy.pattern;
    t->value = to_copy.value;
}

color solid_value(texture *t, double u, double v, point3 p){
    return ((solid*)(t->pattern))->albedo;
}

void init_solid_tex(texture *t, color a){
    solid *s = malloc(sizeof(solid));
    copy(&(s->albedo), a);
    t->pattern = s;
    t->value = &solid_value;
}

void init_solid_tex_rgb(texture *t, double r, double g, double b){
    solid *s = malloc(sizeof(solid));
    init(&(s->albedo), r, g, b);
    t->pattern = s;
    t->value = &solid_value;
}

int my_floor(double x){
    if(x > 0){
        return (int) x;
    }

    x -= 1;
    return (int) x;
}

void copy_checker(checker *c, checker to_copy){
    copy_texture(&(c->even), to_copy.even);
    copy_texture(&(c->odd), to_copy.odd);
    c->inv_scale = to_copy.inv_scale;
}

color checker_value(texture *t, double u, double v, point3 p){
    checker c;
    copy_checker(&c, *((checker *) t->pattern));
    int xf = my_floor(c.inv_scale * p.e[x]); 
    int yf = my_floor(c.inv_scale * p.e[y]); 
    int zf = my_floor(c.inv_scale * p.e[z]); 

    if((xf + yf + zf) % 2 == 0){
        return (*(c.even.value))(&(c.even), u, v, p);
    } else {
        return (*(c.odd.value))(&(c.odd), u, v, p);
    }
}

void init_checker(texture *t, double scale, texture t1, texture t2){
    checker *c = malloc(sizeof(checker));
    copy_texture(&(c->even), t1);
    copy_texture(&(c->odd), t2);
    c->inv_scale = 1.0 / scale;
    
    t->pattern = c;
    t->value = &checker_value;
}

void init_checker_tex(texture *t, double scale, color even, color odd){
    checker *c = malloc(sizeof(checker));
    init_solid_tex(&(c->even), even);
    init_solid_tex(&(c->odd), odd);
    c->inv_scale = 1.0 / scale;
    
    t->pattern = c;
    t->value = &checker_value;
}

void delete_image_tex(texture *t){
   delete_image(&((image_tex *)t->pattern)->img);
   free(t->pattern);
}

static double clamp(double x, double low, double high){
    if(x < low) return low;
    if(x < high) return x;
    return high;
}

color image_value(texture *t, double u, double v, point3 p){
    image *i = &(((image_tex *)t->pattern)->img);
    
    if(i->image_height <= 0){
        color c;
        init(&c, 0, 1, 1);
        return c;
    }

    u = clamp(u, 0, 1);
    v = 1.0 - clamp(v, 0, 1);

    int k = (int) (u * i->image_width);
    int j = (int) (v * i->image_height);
    const unsigned char *pixel = pixel_data(i, k, j);

    double color_scale = 1.0 / 255.0;
    
    color c;
    init(&c, color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
    return c;
}

void init_image_tex(texture *t, const char *filename){
    image_tex *i = malloc(sizeof(image_tex));
    load_image(&(i->img), filename);
    
    t->pattern = i;
    t->value = &image_value;
}
