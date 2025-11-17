#include "camera.h"

#include "material.h"
#include "pdf.h"
#include "skybox.h"
#include "util.h"
#include "vector3.h"
#include <mpi.h>

color ray_color(ray r, int depth, const hittable_list *world, const hittable_list *priorities, color background, skybox *sky){
    if(depth <= 0){
        color black;
        init(&black, 0, 0, 0);
        return black;
    }

    hit_record h;
    scatter_record s;

    if(!hit(world, r, 0.001, DBL_MAX, &h)){
        if(!sky) return background;
        return skybox_value(sky, r);
    }

    double pdf_value;
    color color_from_emmision = (*(h.mat->emit))(h.mat, r, h, h.u, h.v, h.p);

    if(!(*h.mat->scatter_func)(r, &h, &s)) {
        return color_from_emmision;
    }

    
    //skips importance sampling for specular surfaces
    if(s.skip_pdf){
        return attenuate(s.attenuation, ray_color(s.skip_pdf_ray, depth - 1, world, priorities, background, sky));
    }

    //mixing a pdf of important objects and the material's inherent pdf 
    pdf hittable_pdf, mixture_pdf;
    // Handling if no importance sampling is used
    if(priorities->size > 0){
        init_hittable_pdf(&hittable_pdf, priorities, h.p);
        init_mixture_pdf(&mixture_pdf, &hittable_pdf, s.pdf_ptr);
    } else {
        mixture_pdf = *(s.pdf_ptr);
    }

    ray bounce;
    init_ray(&bounce, h.p, mixture_pdf.generate(mixture_pdf)); 
    pdf_value = mixture_pdf.value(mixture_pdf, bounce.dir);
    delete_pdf(s.pdf_ptr);
    free(s.pdf_ptr);

    if(priorities->size > 0){
        delete_pdf(&hittable_pdf);
        delete_pdf(&mixture_pdf);
    }

    double scatter_pdf = h.mat->pdf(r, h, bounce);


    color color_from_scatter;
    copy(&color_from_scatter, attenuate(s.attenuation, ray_color(bounce, depth - 1, world, priorities, background, sky)));
    
    scale(&color_from_scatter, scatter_pdf / pdf_value);
    
    add_vector(&color_from_emmision, color_from_scatter);            
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

    c->sqrt_spb = (int) sqrt(SAMPLES_PER_BATCH);
    c->recip_sqrt_spb = 1.0 / c->sqrt_spb;

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

vector3 sample_square_stratified(const camera *c, int s_i, int s_j){
    point3 pixel_sample;
    init(&pixel_sample, ((s_i + rnd_double()) * c->recip_sqrt_spb) - 0.5, ((s_j + rnd_double()) * c->recip_sqrt_spb) - 0.5, 0);
    return pixel_sample;
}

ray get_ray(const camera *c, int i, int j, int s_i, int s_j){
    //sample square
    point3 pixel_sample;
    copy(&pixel_sample, sample_square_stratified(c, s_i, s_j));

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

void render(camera *c, const hittable_list *world, const hittable_list *priorities){
    int rank, num_processes;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    
    initialize(c);
    
    // Calculate work distribution - each process gets a contiguous chunk of rows
    int rows_per_process = c->image_height / num_processes;
    int remainder = c->image_height % num_processes;
    
    int start_row = rank * rows_per_process + (rank < remainder ? rank : remainder);
    int end_row = start_row + rows_per_process + (rank < remainder ? 1 : 0);
    int local_rows = end_row - start_row;
    
    // Allocate local buffer for this process's rows
    uint8_t *local_raster = (uint8_t *) malloc(local_rows * c->image_width * 4 * sizeof(uint8_t));
    
    // Render assigned rows
    color pixel_color;
    int i, j, s_i, s_j, sample;
    double illum, illumSum, illumSqrSum, sigSqr;
    
    for(j = start_row; j < end_row; j++){
        if(rank == 0){
            int local_j = j - start_row;
            double percent = (100.0 * local_j) / local_rows;
            fprintf(stderr, "\rRendering: %.1f%%        ", percent);
            fflush(stderr);
        }
        
        int local_j = j - start_row; // Index into local_raster
        
        for(i = 0; i < c->image_width; i++){
            init(&pixel_color, 0, 0, 0);
            illumSum = 0;
            illumSqrSum = 0;
            
            for(sample = 1; sample <= c->samples_per_pixel; ){
                
                //doing batch of stratified sampling
                //checking for pixel illuminance convergence after each batch
                for(s_j = 0; s_j < c->sqrt_spb; s_j++){
                    for(s_i = 0; s_i < c->sqrt_spb; s_i++){
                        ray r = get_ray(c, i, j, s_i, s_j);

                        color sampleColor;
                        copy(&sampleColor, ray_color(r, c->max_depth, world, priorities, c->background, c->sky));
                        illum = illuminance(sampleColor);
                        illumSum += illum;
                        illumSqrSum += illum * illum;

                        add_vector(&pixel_color, sampleColor);

                        sample++;
                    }
                }

                //checking if pixel illuminance has converged and the average
                //sampled illuminance is within a 95% confidence interval.
                //EQ is reqrranged to avoid divisions and square roots
                //Stopping sampling early if so.
                sigSqr = ((illumSqrSum * sample) - (illumSum * illumSum)) / ((sample * sample) - sample);
                if(Z_95_VALUE_SQR * sample * sigSqr <= MAX_TOLERANCE_SQR * illumSum * illumSum) break;
            }

            scale(&pixel_color, 1.0 / sample);
            print_color(pixel_color, local_raster + 4*((local_j * c->image_width) + i), !c->sky);
        }
    }
    
    // Rank 0 gathers all data and writes the file
    if(rank == 0){
        fprintf(stderr, "\rRendering: 100.0%%        \n");
        fprintf(stderr, "Aggregating results...\n");
        fflush(stderr);
        
        uint8_t *full_raster = (uint8_t *) malloc(c->image_height * c->image_width * 4 * sizeof(uint8_t));
        
        // Copy rank 0's data
        memcpy(full_raster, local_raster, local_rows * c->image_width * 4 * sizeof(uint8_t));
        
        // Gather data from other processes
        for(int src = 1; src < num_processes; src++){
            int src_rows_per_process = c->image_height / num_processes;
            int src_start_row = src * src_rows_per_process + (src < remainder ? src : remainder);
            int src_local_rows = src_rows_per_process + (src < remainder ? 1 : 0);
            
            MPI_Recv(full_raster + 4 * src_start_row * c->image_width,
                     src_local_rows * c->image_width * 4,
                     MPI_UNSIGNED_CHAR,
                     src,
                     0,
                     MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);
        }
        
        // Write the image file
        FILE *img = fopen("image.hdr", "wb");
        if(!img){
            fprintf(stderr, "ERROR: Unable to open image file to write to\n");
        } else {
            fprintf(img, "#?RADIANCE\nFORMAT=32-bit_rle_rgbe\n\n-Y %d +X %d\n", c->image_height, c->image_width);
            fwrite(full_raster, 4 * c->image_height * c->image_width, 1, img);
            fclose(img);
            fprintf(stderr, "Done.                       \n");
        }
        
        free(full_raster);
    } else {
        // Send data to rank 0
        MPI_Send(local_raster,
                 local_rows * c->image_width * 4,
                 MPI_UNSIGNED_CHAR,
                 0,
                 0,
                 MPI_COMM_WORLD);
    }
    
    free(local_raster);
}
