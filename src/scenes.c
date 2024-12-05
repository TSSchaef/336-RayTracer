#include "scenes.h"
#include "hittable_list.h"
#include "instance.h"
#include "material.h"
#include "skybox.h"
#include "sphere.h"
#include "texture.h"

void cornell_box(){
    hittable_list world;
    init_list(&world);
    
    color r, w, g, l, al;
    init(&l, 15.0, 15.0, 15.0);
    init(&w, 0.73, 0.73, 0.73);
    init(&r, 0.65, 0.05, 0.05);
    init(&g, 0.12, 0.45, 0.15);
    init(&al, 0.8, 0.85, 0.88);

    material dif_light, red, white, green, aluminum, glass;

    init_lambertian(&red, r);
    init_lambertian(&white, w);
    init_lambertian(&green, g);
    init_metal(&aluminum, al, 0.0);
    init_dielectric(&glass, 1.5);
    init_diffuse_light(&dif_light, l);

    quad q1, q2, q3, q4, q5, q6;
    point3 q1Q, q2Q, q3Q, q4Q, q5Q, q6Q;
    vector3 q1u, q2u, q3u, q4u, q5u, q6u;
    vector3 q1v, q2v, q3v, q4v, q5v, q6v;
    
    init(&q1Q, 555, 0, 0);
    init(&q2Q, 0, 0, 0);
    init(&q3Q, 343, 554, 332);
    init(&q4Q, 0, 0, 0);
    init(&q5Q, 555, 555, 555);
    init(&q6Q, 0, 0, 555);

    init(&q1u, 0, 555, 0);
    init(&q2u, 0, 555, 0);
    init(&q3u, -130, 0, 0);
    init(&q4u, 555, 0, 0);
    init(&q5u, -555, 0, 0);
    init(&q6u, 555, 0, 0);

    init(&q1v, 0, 0, 555);
    init(&q2v, 0, 0, 555);
    init(&q3v, 0, 0, -105);
    init(&q4v, 0, 0, 555);
    init(&q5v, 0, 0, -555);
    init(&q6v, 0, 555, 0);

    init_quad(&q1, q1Q, q1u, q1v, green);
    init_quad(&q2, q2Q, q2u, q2v, red);
    init_quad(&q3, q3Q, q3u, q3v, dif_light);
    init_quad(&q4, q4Q, q4u, q4v, white);
    init_quad(&q5, q5Q, q5u, q5v, white);
    init_quad(&q6, q6Q, q6u, q6v, white);

    add_list_no_pdf(&world, &q1, &hit_quad, &get_quad_box);
    add_list_no_pdf(&world, &q2, &hit_quad, &get_quad_box);
    add_list_no_pdf(&world, &q3, &hit_quad, &get_quad_box);
    add_list_no_pdf(&world, &q4, &hit_quad, &get_quad_box);
    add_list_no_pdf(&world, &q5, &hit_quad, &get_quad_box);
    add_list_no_pdf(&world, &q6, &hit_quad, &get_quad_box);

    point3 p1, p2;
    init(&p1, 0, 0, 0);
    init(&p2, 165, 330, 165);

    hittable_list *cube1;
    cube1 = init_cube(p1, p2, aluminum);

    rotate r1;
    translate t1;
    vector3 o1;
    init_rotate(&r1, cube1, &hit, &cube_pdf_value, &cube_pdf_generate, cube1->box, 15);
    init(&o1, 265, 0, 295);
    init_translate(&t1, &r1, &hit_rotate, &rotate_pdf_value, &rotate_pdf_generate, r1.bbox, o1);

    add_list_no_pdf(&world, &t1, &hit_translate, &get_translate_box);
    
    point3 center;
    init(&center, 190, 90, 190);
    sphere s;
    //init_sphere(&s, center, 90, white);
    init_sphere(&s, center, 90, glass);
    add_list_no_pdf(&world, &s, &hit_sphere, &get_sphere_box);
    
    bvh_node root;
    init_bvh(&root, &world);

    delete_list(&world);
    init_list(&world);
    add_list_no_pdf(&world, &root, &hit_bvh, &get_bvh_box);

    hittable_list priorities;
    init_list(&priorities);
    add_list(&priorities, &q3, &hit_quad, &get_quad_box, &quad_pdf_value, &quad_pdf_generate);
    
    //initializing camera
    camera cam;
    cam.aspect_ratio = 1.0;
    cam.image_width = 1200;
    cam.samples_per_pixel = 3000;

    init(&(cam.background), 0, 0, 0);
    cam.sky = NULL;

    cam.max_depth = 50;
    cam.vfov = 40;
    
    point3 f, a, v;
    init(&f, 278, 278, -800);
    init(&a, 278, 278, 0);
    init(&v, 0, 1, 0);
    copy(&(cam.lookfrom), f);
    copy(&(cam.lookat), a);
    copy(&(cam.vup), v);

    cam.defocus_angle = 0;
    cam.focus_dist = 2;

    render(&cam, &world, &priorities);
    
    delete_texture(&(red.tex));
    delete_texture(&(white.tex));
    delete_texture(&(green.tex));
    delete_texture(&(aluminum.tex));
    delete_texture(&(glass.tex));
    delete_texture(&(dif_light.tex));
    delete_cube(cube1);
    //delete_cube(cube2);
    delete_bvh(&root);
    delete_list(&priorities); 
    delete_list(&world); 
}

void triangle_test(){
    hittable_list world;
    init_list(&world);
    
    color r, red, blue;
    init(&r, 0.9, 0.9, 0.75);
    init(&red, 100.0, 0, 0);
    init(&blue, 0, 0, 100.0);

    material dif_red_light, dif_blue_light, mat;

    init_lambertian(&mat, r);
    init_diffuse_light(&dif_red_light, red);
    init_diffuse_light(&dif_blue_light, blue);

    point3 center1, center2;
    init(&center1, 1, 3.5, 0.5);
    init(&center2, -1, 3.5, 0.5);
    sphere s1, s2;
    init_sphere(&s1, center1, 0.5, dif_blue_light);
    init_sphere(&s2, center2, 0.5, dif_red_light);

    mesh *teapot = load_mesh("teapot.obj", mat);
    if(!teapot){
        delete_texture(&(mat.tex));
        delete_list(&world); 
        return;
    }

    add_list_no_pdf(&world, teapot->bvh, &hit_bvh, &get_bvh_box);
    add_list_no_pdf(&world, &s1, &hit_sphere, &get_sphere_box);
    add_list_no_pdf(&world, &s2, &hit_sphere, &get_sphere_box);

    hittable_list priorities;
    init_list(&priorities);
    add_list(&world, &s1, &hit_sphere, &get_sphere_box, &sphere_pdf_value, &sphere_pdf_generate);
    add_list(&world, &s2, &hit_sphere, &get_sphere_box, &sphere_pdf_value, &sphere_pdf_generate);
    
    //initializing camera
    camera cam;
    cam.aspect_ratio = 1.0;
    cam.image_width = 2000;
    cam.samples_per_pixel = 2000;
    init(&(cam.background), 0, 0, 0);
    cam.max_depth = 50;
    cam.vfov = 70;
    cam.sky = NULL;
    
    point3 f, a, v;
    
    //teapot camera
    init(&f, 0, 4, 5);
    init(&a, 0.3, 0.5, 0);
    init(&v, 0, 1, 0);
    
    copy(&(cam.lookfrom), f);
    copy(&(cam.lookat), a);
    copy(&(cam.vup), v);

    cam.defocus_angle = 0;
    cam.focus_dist = 2;

    render(&cam, &world, &priorities);
    
    delete_texture(&(mat.tex));
    delete_texture(&(dif_red_light.tex));
    delete_texture(&(dif_blue_light.tex));
    delete_mesh(teapot);
    delete_list(&priorities); 
    delete_list(&world); 
}


void test_skybox(){
    hittable_list world, priorities, skullObj;
    init_list(&world);
    init_list(&priorities);
    init_list(&skullObj);
    
    color al, bone, glow;
    init(&al, 0.8, 0.85, 0.88);
    init(&bone, 0.98, 0.94, 0.78);
    init(&glow, 0.754, 20, 0.566);

    material mat, aluminum, light;
    init_metal(&aluminum, al, 0);
    init_lambertian(&mat, bone);
    init_diffuse_light(&light, glow);

    point3 c1, c2, c3;
    init(&c1, 13, 13.2, 3.7);
    init(&c2, 13, 13.2, -3.7);
    sphere sph1, sph2, l; 
    init_sphere(&sph1, c1, 2.8, aluminum);
    init_sphere(&sph2, c2, 2.8, aluminum);
    add_list(&skullObj, &sph1, &hit_sphere, &get_sphere_box, &sphere_pdf_value, &sphere_pdf_generate);
    add_list(&skullObj, &sph2, &hit_sphere, &get_sphere_box, &sphere_pdf_value, &sphere_pdf_generate);

    mesh *skull = load_mesh("skull.obj", mat);
    if(!skull){
        delete_texture(&(mat.tex));
        delete_list(&world); 
        delete_list(&priorities); 
        return;
    }

    rotate r1, r2;
    init_rotate_no_pdf(&r1, skull->bvh, &hit_bvh, skull->bvh->bbox, 90);
    init_rotate_z_no_pdf(&r2, &r1, &hit_rotate, r1.bbox, 270);
    add_list_no_pdf(&skullObj, &r2, &hit_rotate_z, &get_rotate_box);


    translate t1, t2;
    vector3 vec1, vec2;
    init(&vec1, 0, 0, 26);
    init(&vec2, 0, 0, -26);
    init_translate_no_pdf(&t1, &skullObj, &hit, skullObj.box, vec1);
    init_translate_no_pdf(&t2, &skullObj, &hit, skullObj.box, vec2);

    add_list_no_pdf(&world, &t1, &hit_translate, &get_translate_box);
    add_list_no_pdf(&world, &t2, &hit_translate, &get_translate_box);

    init(&c3, 9, 0, -2);
    init_sphere(&l, c3, 8, light);
    add_list(&world, &l, &hit_sphere, &get_sphere_box, &sphere_pdf_value, &sphere_pdf_generate);

    add_list(&priorities, &l, &hit_sphere, &get_sphere_box, &sphere_pdf_value, &sphere_pdf_generate);
        
    texture perlin;
    init_perlin_tex(&perlin, 4);
    material perl;
    init_lambertian_tex(&perl, perlin);

    int i;
    const int NUM_SPHERES = 1000;
    sphere *s[NUM_SPHERES];
    point3 c;
    for(i = 0; i < NUM_SPHERES; i++){
        s[i] = (sphere *) malloc(sizeof(sphere));

        init(&c, rnd_int(-50, 50), rnd_int(-15, 0), rnd_int(-18, 18));
        init_sphere(s[i], c, 1.2, perl);
        add_list_no_pdf(&world, s[i], &hit_sphere, &get_sphere_box);
    }
    
    // turning world into bvh 
    bvh_node root;
    init_bvh(&root, &world);

    delete_list(&world);
    init_list(&world);
    add_list_no_pdf(&world, &root, &hit_bvh, &get_bvh_box);

    //initializing camera
    camera cam;
    cam.aspect_ratio = 1.0;
    cam.image_width = 2000;
    cam.samples_per_pixel = 3500;

    skybox sky;
    init_skybox(&sky, "relic.hdr");
    cam.sky = &sky;

    cam.max_depth = 50;
    cam.vfov = 70;
    
    point3 f, a, v;
    init(&f, 70, 10, 0);
    init(&a, 0, 12, 0);
    init(&v, 0, 1, 0);
     
    copy(&(cam.lookfrom), f);
    copy(&(cam.lookat), a);
    copy(&(cam.vup), v);

    cam.defocus_angle = 0;
    cam.focus_dist = 2;

    render(&cam, &world, &priorities);

    for(i = 0; i < NUM_SPHERES; i++){
        free(s[i]);
    }
    
    delete_skybox(&sky);
    delete_texture(&(perlin));
    delete_texture(&(mat.tex));
    delete_texture(&(aluminum.tex));
    delete_texture(&(light.tex));
    delete_mesh(skull);
    delete_bvh(&root);
    delete_list(&skullObj); 
    delete_list(&priorities); 
    delete_list(&world); 
}

void render_scene(int scene_id){
    switch(scene_id){
        case 1: cornell_box(); break;
        case 2: triangle_test(); break;
        case 3: test_skybox(); break;
    }   
}
