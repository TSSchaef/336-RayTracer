#include <stdio.h>

#include "util.h"
#include "hittable_list.h"
#include "sphere.h"

#define ASPECT_RATIO (16.0 / 9.0)

hittable_list world;

color ray_color(ray r){
    hit_record h;
    if(hit(&world, r, 0, DBL_MAX, &h)){
        color c;
        init(&c, 1, 1, 1);
        add_vector(&c, h.normal);
        scale(&c, 0.5);
        return c;
    }

    vector3 unit_direction;
    copy(&unit_direction, r.dir);
    unit_vector(&unit_direction);

    double a = 0.5 * (unit_direction.e[y] + 1.0);
    color white, blue;
    init(&white, 1.0, 1.0, 1.0);
    init(&blue, 0.5, 0.7, 1.0);
    scale(&blue, a);
    scale(&white, 1.0 - a);
    add_vector(&white, blue);
    return white;
}

int main(int argc, char *argv[]){
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

    int image_width = 400;
    int image_height = (int) image_width / ASPECT_RATIO; 
    image_height = (image_height < 1) ? 1 : image_height;
    double viewport_height = 2.0;
    double viewport_width = viewport_height * (((double) image_width )/ image_height);

    double focal_length = 1.0;
    point3 camera_center;
    init(&camera_center, 0, 0, 0);

    vector3 viewport_u, viewport_v;
    init(&viewport_u, viewport_width, 0, 0);
    init(&viewport_v, 0, -viewport_height, 0);

    vector3 pixel_delta_u, pixel_delta_v;
    copy(&pixel_delta_u, viewport_u);
    scale(&pixel_delta_u, 1.0/image_width);
    copy(&pixel_delta_v, viewport_v);
    scale(&pixel_delta_v, 1.0/image_height); 
   
    point3 viewport_upper_left;
    init(&viewport_upper_left, -viewport_width/2, viewport_height/2, -focal_length);
    add_vector(&viewport_upper_left, camera_center);

    point3 pixel00_loc;
    copy(&pixel00_loc, pixel_delta_u);
    add_vector(&pixel00_loc, pixel_delta_v);
    scale(&pixel00_loc, 0.5);
    add_vector(&pixel00_loc, viewport_upper_left);


    printf("P3\n %d %d\n255\n", image_width, image_height);

    int i, j;
    for(j = 0; j < image_height; j++) {
        fprintf(stderr, "\rScanlines remaining: %d", image_height - j);
        fflush(stderr);
        for(i = 0; i < image_width; i++){
            point3 pixel_center, u_offset, v_offset;
            copy(&u_offset, pixel_delta_u);
            copy(&v_offset, pixel_delta_v);
            copy(&pixel_center, pixel00_loc);
            scale(&u_offset, i);
            scale(&v_offset, j);
            add_vector(&pixel_center, u_offset);
            add_vector(&pixel_center, v_offset);

            vector3 ray_direction;
            copy(&ray_direction, camera_center);
            invert(&ray_direction);
            add_vector(&ray_direction, pixel_center);

            ray r;
            init_ray(&r, camera_center, ray_direction);

            color c = ray_color(r);

            print_color(c);
        }
    }
    fprintf(stderr, "\rDone.                       \n");

    delete_list(&world); 

    return 0;
}
