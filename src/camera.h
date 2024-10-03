#ifndef CAMERA_H
#define CAMERA_H

#include "util.h"
#include "material.h"
#include "hittable_list.h"
#include "pthread.h"
#include "pdf.h"
#include "stdint.h"

#define SAMPLES_PER_BATCH 64
#define Z_95_VALUE_SQR 3.8416 
#define MAX_TOLERANCE_SQR 0.0025

typedef struct {
    double aspect_ratio;
    int image_width;
    int image_height;
    point3 center;
    point3 pixel00_loc;
    vector3 pixel_delta_u;
    vector3 pixel_delta_v;
    int samples_per_pixel;
    double pixel_samples_scale;
    int sqrt_spb;
    double recip_sqrt_spb;
    int max_depth;

    color background;

    double vfov;
    point3 lookfrom;
    point3 lookat;
    vector3 vup;
    vector3 u, v, w;

    double defocus_angle;
    double focus_dist;
    vector3 defocus_disk_u;
    vector3 defocus_disk_v;
} camera;

void initialize(camera *c);
void render(camera *c, const hittable_list *world, const hittable_list *priorities);

#endif
