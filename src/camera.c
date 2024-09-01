#include "camera.h"

color ray_color(ray r, int depth, hittable_list *world){
    if(depth <= 0){
        color black;
        init(&black, 0, 0, 0);
        return black;
    }

    hit_record h;
    if(hit(world, r, 0.001, DBL_MAX, &h)){
        ray bounce;
        color attenuation;

        if((*h.mat.scatter_func)(r, &h, &attenuation, &bounce)){
            return attenuate(attenuation, ray_color(bounce, depth - 1, world));            
        }

        color black;
        init(&black, 0, 0, 0);
        return black;
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
    c->samples_per_pixel = (c->samples_per_pixel > 0) ? c->samples_per_pixel : 10;
    c->max_depth = (c->max_depth > 0) ? c->max_depth : 10;

    c->pixel_samples_scale = 1.0 / c->samples_per_pixel;
    c->image_height = (int) c->image_width / c->aspect_ratio; 
    c->image_height = (c->image_height < 1) ? 1 : c->image_height;

    copy(&(c->center), c->lookfrom);

    invert(&(c->lookat));
    add_vector(&(c->lookfrom), c->lookat);
    double focal_length = length(c->lookfrom);
    c->vfov = (c->vfov > 0) ? c->vfov : 90;
    double theta = DEG_TO_RAD(c->vfov);
    double h = tan(theta/2);
    double viewport_height = 2 * h * focal_length; 
    double viewport_width = viewport_height * (((double) c->image_width )/ c->image_height);

    unit_vector(&(c->lookfrom));
    copy(&(c->w), c->lookfrom); 

    copy(&(c->u), cross(c->vup, c->w));
    unit_vector(&(c->u));

    copy(&(c->v), cross(c->w, c->u));

    vector3 viewport_u, viewport_v;
    copy(&viewport_u, c->u);
    scale(&viewport_u, viewport_width);
    copy(&viewport_v, c->v);
    scale(&viewport_v, -1 * viewport_height);

    copy(&(c->pixel_delta_u), viewport_u);
    scale(&(c->pixel_delta_u), 1.0/c->image_width);
    copy(&(c->pixel_delta_v), viewport_v);
    scale(&(c->pixel_delta_v), 1.0/c->image_height); 
   
    point3 viewport_upper_left;
    copy(&viewport_upper_left, c->w);
    scale(&viewport_upper_left, focal_length);
    invert(&viewport_upper_left);
    scale(&viewport_u, -1.0/2);
    scale(&viewport_v, -1.0/2);
    add_vector(&viewport_upper_left, viewport_u);
    add_vector(&viewport_upper_left, viewport_v);
    add_vector(&viewport_upper_left, c->center);

    copy(&(c->pixel00_loc), (c->pixel_delta_u));
    add_vector(&(c->pixel00_loc), (c->pixel_delta_v));
    scale(&(c->pixel00_loc), 0.5);
    add_vector(&(c->pixel00_loc), viewport_upper_left);
}


ray get_ray(camera *c, int i, int j){
    //sample square
    point3 pixel_sample;
    init(&pixel_sample, RAND_DOUBLE - 0.5, RAND_DOUBLE - 0.5, 0);

    point3 u_offset, v_offset;
    copy(&u_offset, c->pixel_delta_u);
    copy(&v_offset, c->pixel_delta_v);
    scale(&u_offset, i + pixel_sample.e[x]);
    scale(&v_offset, j + pixel_sample.e[y]);
    copy(&pixel_sample, c->pixel00_loc);
    add_vector(&pixel_sample, u_offset);
    add_vector(&pixel_sample, v_offset);

    vector3 ray_direction;
    copy(&ray_direction, c->center);
    invert(&ray_direction);
    add_vector(&ray_direction, pixel_sample);
            
    ray r;
    init_ray(&r, c->center, ray_direction);

    return r;
}

void render(camera *c, hittable_list *world){
    initialize(c);

    printf("P3\n %d %d\n255\n", c->image_width, c->image_height);

    int i, j;
    for(j = 0; j < c->image_height; j++) {
        fprintf(stderr, "\rScanlines remaining: %d        ", c->image_height - j);
        fflush(stderr);
        for(i = 0; i < c->image_width; i++){
            color pixel_color;
            init(&pixel_color, 0, 0, 0);

            int sample;
            for(sample = 0; sample < c->samples_per_pixel; sample++){
                ray r = get_ray(c, i, j);
                add_vector(&pixel_color, ray_color(r, c->max_depth, world));
            }
            
            scale(&pixel_color, c->pixel_samples_scale);
            print_color(pixel_color);
        }
    }
    fprintf(stderr, "\rDone.                       \n");
}
