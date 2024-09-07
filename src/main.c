
#include "camera.h"
#include "material.h"
#include "texture.h"
#include "hittable_list.h"
#include "sphere.h"
#include "bvh.h"
#include "quad.h"


void orig_scene() {
    hittable_list world;
    init_list(&world);

    point3 center1;
    init(&center1, 0, 0, -1.2);
    sphere s1;
    color c1;
    init(&c1, 0.1, 0.2, 0.5);
    material m1;
    init_lambertian(&m1, c1);
    init_sphere(&s1, center1, 0.5, m1);
    
    add_list(&world, &s1, &hit_sphere, &get_sphere_box);

    point3 center2;
    texture t2;
    color texc1, texc2;
    init(&texc1, 0.2, 0.3, 0.1);
    init(&texc2, 0.9, 0.9, 0.9);
    init_checker_tex(&t2, 0.32, texc1, texc2); 
    init(&center2, 0, -100.5, -1);
    sphere s2;
    material m2;
    init_lambertian_tex(&m2, t2);
    //color c2;
    //init(&c2, 0.9, 0.2, 0.5);
    //init_lambertian(&m2, c2);
    init_sphere(&s2, center2, 100, m2);

    add_list(&world, &s2, &hit_sphere, &get_sphere_box);

    point3 center3;
    init(&center3, -1, 0, -1);
    sphere s3;
    material m3;
    init_dielectric(&m3, 1.00/1.33);
    init_sphere(&s3, center3, 0.5, m3);

    add_list(&world, &s3, &hit_sphere, &get_sphere_box);

    point3 center4;
    init(&center4, 1, 0, -1);
    sphere s4;
    color c4;
    init(&c4, 0.8, 0.6, 0.2);
    material m4;
    init_metal(&m4, c4, 0);
    init_sphere(&s4, center4, 0.5, m4);

    add_list(&world, &s4, &hit_sphere, &get_sphere_box); 

    point3 center5;
    init(&center5, -2.2, -0.4, -2);
    sphere s5;
    color c5;
    init(&c5, 0.8, 0, 0.8);
    material m5;
    init_lambertian(&m5, c5);
    init_sphere(&s5, center5, 0.1, m5);

    add_list(&world, &s5, &hit_sphere, &get_sphere_box); 

    point3 center6;
    init(&center6, -2, -0.4, -2);
    sphere s6;
    color c6;
    init(&c6, 0.2, 0.2, 0.2);
    material m6;
    init_metal(&m6, c6, 0.1);
    init_sphere(&s6, center6, 0.1, m6);

    add_list(&world, &s6, &hit_sphere, &get_sphere_box); 

    bool using_BVH = world.size > 10;
    bvh_node root;
    if(using_BVH){
        init_bvh(&root, &world);

        delete_list(&world);
        init_list(&world);
        add_list(&world, &root, &hit_bvh, &get_bvh_box);
    }

    //initializing camera
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 500;
    cam.max_depth = 50;
    cam.vfov = 90;
    
    point3 f, a, v;
    init(&f, 0, 0, 0);
    init(&a, 0, 0, -1);
    init(&v, 0, 1, 0);

    cam.defocus_angle = 0;
    cam.focus_dist = 3.4;

    copy(&(cam.lookfrom), f);
    copy(&(cam.lookat), a);
    copy(&(cam.vup), v);
    
    render(&cam, &world);

    if(using_BVH) delete_bvh(&root);

    delete_texture(&(((checker*)t2.pattern)->even));
    delete_texture(&(((checker*)t2.pattern)->odd));
    delete_texture(&t2);
    delete_list(&world); 
}

void checkered_spheres(){
    hittable_list world;
    init_list(&world);

    texture t2;
    color texc1, texc2;
    init(&texc1, 0.2, 0.3, 0.1);
    init(&texc2, 0.9, 0.9, 0.9);
    init_checker_tex(&t2, 0.32, texc1, texc2); 

    point3 center1;
    init(&center1, 0, -10, 0);
    sphere s1;
    material m1;
    init_lambertian_tex(&m1, t2);
    init_sphere(&s1, center1, 10, m1);
    
    add_list(&world, &s1, &hit_sphere, &get_sphere_box);

    point3 center2;
    init(&center2, 0, 10, 0);
    sphere s2;
    material m2;
    init_lambertian_tex(&m2, t2);
    init_sphere(&s2, center2, 10, m2);

    add_list(&world, &s2, &hit_sphere, &get_sphere_box);

    //initializing camera
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 5;//100;
    cam.max_depth = 50;
    cam.vfov = 20;
    
    point3 f, a, v;
    init(&f, 13, 2, 3);
    init(&a, 0, 0, 0);
    init(&v, 0, 1, 0);
    copy(&(cam.lookfrom), f);
    copy(&(cam.lookat), a);
    copy(&(cam.vup), v);

    cam.defocus_angle = 0;
    cam.focus_dist = 3.4;

    render(&cam, &world);
    
    delete_texture(&(((checker*)t2.pattern)->even));
    delete_texture(&(((checker*)t2.pattern)->odd));
    delete_texture(&t2);
    delete_list(&world); 
}

void earth(){
    hittable_list world;
    init_list(&world);

    texture earth_tex;
    init_image_tex(&earth_tex, "earthmap.jpg");

    point3 center1;
    init(&center1, 0, 0, 0);
    sphere s1;
    material m1;
    init_lambertian_tex(&m1, earth_tex);
    init_sphere(&s1, center1, 2, m1);
    
    add_list(&world, &s1, &hit_sphere, &get_sphere_box);

    //initializing camera
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 5;//100;
    cam.max_depth = 50;
    cam.vfov = 20;
    
    point3 f, a, v;
    init(&f, 0, 0, 12);
    init(&a, 0, 0, 0);
    init(&v, 0, 1, 0);
    copy(&(cam.lookfrom), f);
    copy(&(cam.lookat), a);
    copy(&(cam.vup), v);

    cam.defocus_angle = 0;
    cam.focus_dist = 2;

    render(&cam, &world);
    
    delete_image_tex(&earth_tex);
    delete_list(&world); 
}

void perlin_spheres(){
    hittable_list world;
    init_list(&world);

    texture p_tex;
    init_perlin_tex(&p_tex, 4);

    point3 center1;
    init(&center1, 0, -1000, 0);
    sphere s1;
    material m1;
    init_lambertian_tex(&m1, p_tex);
    init_sphere(&s1, center1, 1000, m1);
    
    add_list(&world, &s1, &hit_sphere, &get_sphere_box);

    point3 center2;
    init(&center2, 0, 2, 0);
    sphere s2;
    init_sphere(&s2, center2, 2, m1);
    
    add_list(&world, &s2, &hit_sphere, &get_sphere_box);

    //initializing camera
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.vfov = 20;
    
    point3 f, a, v;
    init(&f, 13, 2, 3);
    init(&a, 0, 0, 0);
    init(&v, 0, 1, 0);
    copy(&(cam.lookfrom), f);
    copy(&(cam.lookat), a);
    copy(&(cam.vup), v);

    cam.defocus_angle = 0;
    cam.focus_dist = 2;

    render(&cam, &world);
    
    delete_texture(&p_tex);
    delete_list(&world); 
}

void quads(){
    hittable_list world;
    init_list(&world);
    
    color red, green, blue, orange, teal;
    init(&red, 1.0, 0.2, 0.2);
    init(&green, 0.2, 1.0, 0.2);
    init(&blue, 0.2, 0.2, 1.0);
    init(&orange, 1.0, 0.5, 0);
    init(&teal, 0.2, 0.8, 0.8);

    material left_red, back_green, right_blue, upper_orange, lower_teal;

    init_lambertian(&left_red, red);
    init_lambertian(&back_green, green);
    init_lambertian(&right_blue, blue);
    init_lambertian(&upper_orange, orange);
    init_lambertian(&lower_teal, teal);

    quad left, back, right, upper, lower;
    
    point3 leftQ, backQ, rightQ, upperQ, lowerQ;
    vector3 leftu, leftv, backu, backv, rightu, rightv, upperu, upperv, loweru, lowerv;
    
    init(&leftQ, -3, -2, 5);
    init(&leftu, 0, 0, -4);
    init(&leftv, 0, 4, 0);

    init(&backQ, -2, -2, 0);
    init(&backu, 4, 0, 0);
    init(&backv, 0, 4, 0);

    init(&rightQ, 3, -2, 1);
    init(&rightu, 0, 0, 4);
    init(&rightv, 0, 4, 0);

    init(&upperQ, -2, 3, 1);
    init(&upperu, 4, 0, 0);
    init(&upperv, 0, 0, 4);

    init(&lowerQ, -2, -3, 5);
    init(&loweru, 4, 0, 0);
    init(&lowerv, 0, 0, -4);

    init_quad(&left,  leftQ,  leftu,  leftv, left_red);
    init_quad(&right,  rightQ,  rightu,  rightv, right_blue);
    init_quad(&back,  backQ,  backu,  backv, back_green);
    init_quad(&upper,  upperQ,  upperu,  upperv, upper_orange);
    init_quad(&lower,  lowerQ,  loweru,  lowerv, lower_teal);

    add_list(&world, &left, &hit_quad, &get_quad_box);
    add_list(&world, &right, &hit_quad, &get_quad_box);
    add_list(&world, &back, &hit_quad, &get_quad_box);
    add_list(&world, &upper, &hit_quad, &get_quad_box);
    add_list(&world, &lower, &hit_quad, &get_quad_box);

    //initializing camera
    camera cam;
    cam.aspect_ratio = 1.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.vfov = 80;
    
    point3 f, a, v;
    init(&f, 0, 0, 9);
    init(&a, 0, 0, 0);
    init(&v, 0, 1, 0);
    copy(&(cam.lookfrom), f);
    copy(&(cam.lookat), a);
    copy(&(cam.vup), v);

    cam.defocus_angle = 0;
    cam.focus_dist = 2;

    render(&cam, &world);
    
    delete_list(&world); 
}

int main(int argc, char *argv[]){
    switch(5){
        case 1: orig_scene(); break;
        case 2: checkered_spheres(); break;
        case 3: earth(); break;
        case 4: perlin_spheres(); break;
        case 5: quads(); break;
    }   
    return 0;
}
