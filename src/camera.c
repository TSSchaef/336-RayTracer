#include "camera.h"

color ray_color(ray r, hittable_list *world){
    hit_record h;
    if(hit(world, r, 0, DBL_MAX, &h)){
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

void initialize(camera *c){
    c->aspect_ratio = (c->aspect_ratio > 0) ? c->aspect_ratio : 1.0; 
    c->image_width = (c->image_width > 0) ? c->image_width : 100; 

    c->image_height = (int) c->image_width / c->aspect_ratio; 
    c->image_height = (c->image_height < 1) ? 1 : c->image_height;

    init(&(c->center), 0, 0, 0);

    double focal_length = 1.0;
    double viewport_height = 2.0;
    double viewport_width = viewport_height * (((double) c->image_width )/ c->image_height);

    vector3 viewport_u, viewport_v;
    init(&viewport_u, viewport_width, 0, 0);
    init(&viewport_v, 0, -viewport_height, 0);

    copy(&(c->pixel_delta_u), viewport_u);
    scale(&(c->pixel_delta_u), 1.0/c->image_width);
    copy(&(c->pixel_delta_v), viewport_v);
    scale(&(c->pixel_delta_v), 1.0/c->image_height); 
   
    point3 viewport_upper_left;
    init(&viewport_upper_left, -viewport_width/2, viewport_height/2, -focal_length);
    add_vector(&viewport_upper_left, c->center);

    copy(&(c->pixel00_loc), (c->pixel_delta_u));
    add_vector(&(c->pixel00_loc), (c->pixel_delta_v));
    scale(&(c->pixel00_loc), 0.5);
    add_vector(&(c->pixel00_loc), viewport_upper_left);
}

void render(camera *c, hittable_list *world){
    initialize(c);

    printf("P3\n %d %d\n255\n", c->image_width, c->image_height);

    int i, j;
    for(j = 0; j < c->image_height; j++) {
        fprintf(stderr, "\rScanlines remaining: %d", c->image_height - j);
        fflush(stderr);
        for(i = 0; i < c->image_width; i++){
            point3 pixel_center, u_offset, v_offset;
            copy(&u_offset, c->pixel_delta_u);
            copy(&v_offset, c->pixel_delta_v);
            copy(&pixel_center, c->pixel00_loc);
            scale(&u_offset, i);
            scale(&v_offset, j);
            add_vector(&pixel_center, u_offset);
            add_vector(&pixel_center, v_offset);

            vector3 ray_direction;
            copy(&ray_direction, c->center);
            invert(&ray_direction);
            add_vector(&ray_direction, pixel_center);

            ray r;
            init_ray(&r, c->center, ray_direction);

            color c = ray_color(r, world);

            print_color(c);
        }
    }
    fprintf(stderr, "\rDone.                       \n");
}
