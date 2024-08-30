#include <stdio.h>

#include "camera.h"
#include "util.h"
#include "hittable_list.h"
#include "sphere.h"


int main(int argc, char *argv[]){
    //initializing world
    hittable_list world;

    point3 center;
    init(&center, 0, 0, -1);
    sphere s;
    init_sphere(&s, center, 0.5);
    
    init_list(&world, &s, &hit_sphere);

    point3 center2;
    init(&center2, 0, -100.5, -1);
    sphere s2;
    init_sphere(&s2, center2, 100);

    add_list(&world, &s2, &hit_sphere);

    //initializing camera
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    
    render(&cam, &world);

    delete_list(&world); 

    return 0;
}
