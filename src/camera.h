#ifndef CAMERA_H
#define CAMERA_H

#include "util.h"
#include "hittable.h"
#include "hittable_list.h"

typedef struct {
    double aspect_ratio;
    int image_width;
    int image_height;
    point3 center;
    point3 pixel00_loc;
    vector3 pixel_delta_u;
    vector3 pixel_delta_v;
} camera;

void initialize(camera *c);
void render(camera *c, hittable_list *world);

#endif
