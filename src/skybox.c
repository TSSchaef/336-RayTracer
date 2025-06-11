#include "skybox.h"
#include "texture.h"

void init_skybox(skybox *s, const char *filename){
    init_image_tex(&(s->t), filename);
    init(&(s->c), 0, 0, 0); 
}

void init_centered_skybox(skybox *s, point3 center, const char *filename){
    init_image_tex(&(s->t), filename);
    copy(&(s->c), center); 
}

color skybox_value(skybox *s, ray r_in){
    vector3 normal;
    copy(&(normal), s->c);
    invert(&(normal));
    add_vector(&(normal), r_in.dir);
    unit_vector(&(normal));

    double u, v;
    double theta = acos(-1 * normal.e[y]);
    double phi = atan2(-1 * normal.e[z], normal.e[x]) + PI;

    u = phi / (2 * PI);
    v = theta / PI;
    
    return image_value(&(s->t), u, v, normal);
}

void delete_skybox(skybox *s){
    delete_image_tex(&(s->t));
}
