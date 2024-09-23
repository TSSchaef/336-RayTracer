#include "camera.h"
#include "util.h"
#include <pthread.h>

typedef struct {
    const camera *c;
    const hittable_list *world;
    uint8_t *pixel_offset;
    int height, offset;
} portion_info;

color ray_color(ray r, int depth, const hittable_list *world, color background){
    if(depth <= 0){
        color black;
        init(&black, 0, 0, 0);
        return black;
    }

    hit_record h;

    if(!hit(world, r, 0.001, DBL_MAX, &h)) return background;

    ray bounce;
    color attenuation;
    color color_from_emmision = (*(h.mat.emit))(&(h.mat), h.u, h.v, h.p);

    if(!(*h.mat.scatter_func)(r, &h, &attenuation, &bounce)) {
        return color_from_emmision;
    }

    add_vector(&color_from_emmision, attenuate(attenuation, ray_color(bounce, depth - 1, world, background)));            
    return color_from_emmision;
}

void initialize(camera *c){
    c->aspect_ratio = (c->aspect_ratio > 0) ? c->aspect_ratio : 1.0; 
    c->image_width = (c->image_width > 0) ? c->image_width : 100; 
    c->samples_per_pixel = (c->samples_per_pixel > 0) ? c->samples_per_pixel : 10;
    c->max_depth = (c->max_depth > 0) ? c->max_depth : 10;

    c->pixel_samples_scale = 1.0 / c->samples_per_pixel;
    c->image_height = (int) c->image_width / c->aspect_ratio; 
    c->image_height = (c->image_height < 1) ? 1 : c->image_height;
    c->focus_dist = (c->focus_dist >= 0) ? c->focus_dist : 1;

    copy(&(c->center), c->lookfrom);

    c->vfov = (c->vfov > 0) ? c->vfov : 90;
    double theta = DEG_TO_RAD(c->vfov);
    double h = tan(theta/2);
    double viewport_height = 2 * h * c->focus_dist; 
    double viewport_width = viewport_height * (((double) c->image_width )/ c->image_height);

    invert(&(c->lookat));
    add_vector(&(c->lookfrom), c->lookat);
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
    scale(&viewport_upper_left, c->focus_dist);
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

    double defocus_radius = c->focus_dist * tan(DEG_TO_RAD(c->defocus_angle / 2.0));
    copy(&(c->defocus_disk_u), c->u);
    scale(&(c->defocus_disk_u), defocus_radius);
    copy(&(c->defocus_disk_v), c->v);
    scale(&(c->defocus_disk_v), defocus_radius);
}

point3 defocus_disk_sample(const camera *c){
    point3 p = random_in_unit_disk();
    point3 u, v;
    
    copy(&u, c->defocus_disk_u);
    scale(&u, p.e[0]);
    copy(&v, c->defocus_disk_v);
    scale(&v, p.e[1]);

    copy(&p, u);
    add_vector(&p, v);
    add_vector(&p, c->center); 

    return p;
}

ray get_ray(const camera *c, int i, int j){
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

    ray r;
    point3 ray_orig;
    (c->defocus_angle <= 0) ? copy(&ray_orig, c->center) : copy(&ray_orig, defocus_disk_sample(c));

    vector3 ray_direction;
    copy(&ray_direction, ray_orig); 
    invert(&ray_direction);
    add_vector(&ray_direction, pixel_sample);
    init_ray(&r, ray_orig, ray_direction);

    return r;
}

void *render_portion(void *context){
    portion_info *p = (portion_info *) context;
    color pixel_color;
    init(&pixel_color, 0, 0, 0);
    int i, j, sample;
    
    for(j = 0; j < p->height; j++){
        for(i = 0; i < p->c->image_width; i++){
            for(sample = 0; sample < p->c->samples_per_pixel; sample++){
                ray r = get_ray(p->c, i, p->offset + j);
                add_vector(&pixel_color, ray_color(r, p->c->max_depth, p->world, p->c->background));
            }

            scale(&pixel_color, p->c->pixel_samples_scale);

            print_color(pixel_color, p->pixel_offset + 3*((j * p->c->image_width) + i));

        }
    }

    //unused return value necessary for thread syntax
    return NULL;
}

void render(camera *c, const hittable_list *world){
    initialize(c);
    
    FILE *img = fopen("image.ppm", "wb");
    if(!img){
        fprintf(stderr, "ERROR: Unable to open image file to write to");
        return;
    }
    fprintf(img, "P3\n %d %d\n255\n", c->image_width, c->image_height);

    uint8_t *raster = (uint8_t *) malloc(c->image_height * c->image_width * 3 * sizeof(uint8_t));

    int i;
    portion_info *contexts[NUM_THREADS];
    pthread_t threads[NUM_THREADS];
    int height = c->image_height;
    for(i = 0; i < NUM_THREADS; i++){
        contexts[i] = (portion_info *) malloc(sizeof(portion_info));
        contexts[i]->c = c;
        contexts[i]->world = world;

        //adding 1 additional line of the render to each portion to eliminate
        //any potential extra portion at the end (if the height isn't divisible
        //by the number of threads)
        
        contexts[i]->offset = i * ((c->image_height / NUM_THREADS) + 1);
        contexts[i]->height = (height >= (c->image_height / NUM_THREADS) + 1)? (c->image_height / NUM_THREADS) + 1: height;
        contexts[i]->pixel_offset = raster + 3 * contexts[i]->offset * c->image_width;

        pthread_create(&threads[i], NULL, &render_portion, contexts[i]);
        height -= (c->image_height / NUM_THREADS) + 1;
    }
    
    for(i = 0; i < NUM_THREADS; i++){
        //waiting for all threads to finish
        pthread_join(threads[i], NULL);
        free(contexts[i]);
    }

    int j;
    for(j = 0; j < c->image_height; j++) {
        for(i = 0; i < c->image_width; i++){
            uint8_t *pixel = raster + 3*((j * c->image_width) + i);
            fprintf(img, "%d %d %d\n", pixel[r], pixel[g], pixel[b]);
        }
    }

    free(raster);
    fclose(img);

    fprintf(stderr, "\rDone.                       \n");
}
