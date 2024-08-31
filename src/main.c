#include <stdio.h>

#include "camera.h"
#include "util.h"
#include "material.h"
#include "hittable_list.h"
#include "sphere.h"



int main(int argc, char *argv[]){
    hittable_list world;

    //initializing world
    point3 center1;
    init(&center1, 0, 0, -1.2);
    sphere s1;
    color c1;
    init(&c1, 0.1, 0.2, 0.5);
    material m1;
    init_lambertian(&m1, c1);
    init_sphere(&s1, center1, 0.5, m1);
    
    init_list(&world, &s1, &hit_sphere);

    point3 center2;
    init(&center2, 0, -100.5, -1);
    sphere s2;
    color c2;
    init(&c2, 0.8, 0.8, 0);
    material m2;
    init_lambertian(&m2, c2);
    init_sphere(&s2, center2, 100, m2);

    add_list(&world, &s2, &hit_sphere);

    point3 center3;
    init(&center3, -1, 0, -1);
    sphere s3;
    color c3;
    init(&c3, 0.8, 0.8, 0.8);
    material m3;
    init_metal(&m3, c3, 0.3);
    init_sphere(&s3, center3, 0.5, m3);

    add_list(&world, &s3, &hit_sphere);

    point3 center4;
    init(&center4, 1, 0, -1);
    sphere s4;
    color c4;
    init(&c4, 0.8, 0.6, 0.2);
    material m4;
    init_metal(&m4, c4, 1.0);
    init_sphere(&s4, center4, 0.5, m4);

    add_list(&world, &s4, &hit_sphere);


    //initializing camera
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    
    render(&cam, &world);

    delete_list(&world); 

    return 0;
}
