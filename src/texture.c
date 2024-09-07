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

static void permute(int *p, int n){
    int i;
    for(i = n - 1; i > 0; i--){
        int target = rand() % (i + 1);
        int tmp = p[i];
        p[i] = p[target];
        p[target] = tmp;
    }
}

static void perlin_generate_perm(int *p){
    int i;
    for(i = 0; i < POINT_COUNT; i++){
        p[i] = i;
    }

    permute(p, POINT_COUNT);
}

static double perlin_interp(vector3 c[2][2][2], double u, double v, double w) {
    double uu = u*u*(3-2*u);
    double vv = v*v*(3-2*v);
    double ww = w*w*(3-2*w);
    double accum = 0.0;

    int i, j, k;
    for(i = 0; i < 2; i++){
        for(j = 0; j < 2; j++){
            for(k = 0; k < 2; k++){
                vector3 weight;
                init(&weight, u - i, v - j, w - k);
                accum += (i*uu + (1 - i)*(1 - uu)) *
                         (j*vv + (1 - j)*(1 - vv)) *
                         (k*ww + (1 - k)*(1 - ww)) *
                         dot(c[i][j][k], weight);
            }
        }
    }
    return accum;
}

double noise(perlin *perl, point3 p){
    double u = p.e[x] - my_floor(p.e[x]);
    double v = p.e[y] - my_floor(p.e[y]);
    double w = p.e[z] - my_floor(p.e[z]);

    int i = my_floor(p.e[x]);
    int j = my_floor(p.e[y]);
    int k = my_floor(p.e[z]);
    vector3 c[2][2][2];

    int di, dj, dk;
    for(di = 0; di < 2; di++){
        for(dj = 0; dj < 2; dj++){
            for(dk = 0; dk < 2; dk++){
                c[di][dj][dk] = perl->randvec[
                    perl->perm_x[(i+di) & 255] ^
                    perl->perm_y[(j+dj) & 255] ^
                    perl->perm_z[(k+dk) & 255] 
                ];
            }
        }
    }

    return perlin_interp(c, u, v, w);
}

double turb(perlin *perl, point3 p, int depth){
    double accum = 0.0;
    point3 temp_p;
    copy(&temp_p, p);
    double weight = 1.0;

    int i;
    for(i = 0; i < depth; i++){
        accum += weight * noise(perl, temp_p);
        weight *= 0.5;
        scale(&temp_p, 2);
    }

    return (accum > 0) ? accum : -1 * accum;
}

void init_perlin(perlin *p, double s){
    int i;
    for(i = 0; i < POINT_COUNT; i++){
        vector3 v;
        init(&v, RAND_DBL(-1, 1), RAND_DBL(-1, 1), RAND_DBL(-1, 1));
        unit_vector(&v);
        
        copy(&(p->randvec[i]), v);
    }

    p->scale = s;

    perlin_generate_perm(p->perm_x);
    perlin_generate_perm(p->perm_y);
    perlin_generate_perm(p->perm_z);
}

color perlin_value(texture *t, double u, double v, point3 p){
    color c;
    init(&c, 0.5, 0.5, 0.5);
   
    double s = ((perlin *)t->pattern)->scale * p.e[z];
    //s += ((perlin *)t->pattern)->scale * p.e[x];
    s += 10*turb(((perlin *)t->pattern), p, 7);
    scale(&c, 1 + sin(s));
    return c;
}

void init_perlin_tex(texture *t, double scale){
    perlin *p = malloc(sizeof(perlin));
    init_perlin(p, scale);
    t->pattern = p;
    t->value = &perlin_value;
}
