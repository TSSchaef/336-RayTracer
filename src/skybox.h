#ifndef SKYBOX_H
#define SKYBOX_H

#include "sphere.h"
#include "image.h"
#include "material.h"
#include "texture.h"

typedef struct{
    texture t;
    point3 c;
} skybox;

void init_skybox(skybox *s, const char *filename);
void init_centered_skybox(skybox *s, point3 center, const char *filename);
color skybox_value(skybox *s, ray r_in);
void delete_skybox(skybox *s);

#endif
