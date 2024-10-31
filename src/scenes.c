#include "scenes.h"
#include "hittable_list.h"
#include "material.h"
#include "skybox.h"
#include "sphere.h"

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

    point3 p1, p2, p3, p4;
    init(&p1, 0, 0, 0);
    init(&p2, 165, 330, 165);

    //init(&p3, 0, 0, 0);
    //init(&p4, 165, 165, 165);
    
    hittable_list *cube1, *cube2;

    //cube1 = init_cube(p1, p2, white);
    cube1 = init_cube(p1, p2, aluminum);
    //cube2 = init_cube(p3, p4, white);

    rotate r1, r2;
    init_rotate(&r1, cube1, &hit, &cube_pdf_value, &cube_pdf_generate, cube1->box, 15);
    //init_rotate(&r2, cube2, &hit, cube2->box, -18);

    translate t1, t2;
    vector3 o1, o2;
    init(&o1, 265, 0, 295);
    //init(&o2, 130, 0, 65);
    //init_translate(&t1, cube1, &hit, &hittable_list_pdf_value, &hittable_list_pdf_generate, cube1->box, o1);
    init_translate(&t1, &r1, &hit_rotate, &rotate_pdf_value, &rotate_pdf_generate, r1.bbox, o1);
    //init_translate(&t2, &r2, &hit_rotate, r2.bbox, o2);

    add_list_no_pdf(&world, &t1, &hit_translate, &get_translate_box);
    //add_list_no_pdf(&world, &t2, &hit_translate, &get_translate_box);
    
    point3 center;
    init(&center, 190, 90, 190);
    sphere s;
    init_sphere(&s, center, 90, white);
    //init_sphere(&s, center, 90, glass);
    add_list_no_pdf(&world, &s, &hit_sphere, &get_sphere_box);
    
    bvh_node root;
    init_bvh(&root, &world);

    delete_list(&world);
    init_list(&world);
    add_list_no_pdf(&world, &root, &hit_bvh, &get_bvh_box);

    hittable_list priorities;
    init_list(&priorities);
    add_list(&priorities, &q3, &hit_quad, &get_quad_box, &quad_pdf_value, &quad_pdf_generate);
    //add_list(&priorities, &s, &hit_sphere, &get_sphere_box, &sphere_pdf_value, &sphere_pdf_generate);
    //add_list(&priorities, cube1, &hit, &get_list_box, &cube_pdf_value, &cube_pdf_generate);
    //Object Instance pdfs not currently working
    //add_list(&priorities, &t1, &hit_translate, &get_translate_box, &translate_pdf_value, &translate_pdf_generate);
    
    //initializing camera
    camera cam;
    cam.aspect_ratio = 1.0;
    cam.image_width = 1200;
    cam.samples_per_pixel = 3;//000;
    init(&(cam.background), 0, 0, 0);
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
    
    color r, l;
    init(&r, 0.9, 0.9, 0.75);
    init(&l, 100.0, 100.0, 100.0);

    material dif_light, mat;

    //init_dielectric(&mat, 1.00/1.33);
    //init_metal(&mat, r, 0);
    init_lambertian(&mat, r);
    init_diffuse_light(&dif_light, l);

    //mesh *tree = load_mesh("Tree.obj", mat);
    point3 center1, center2;
    init(&center1, 1, 3.5, 0.5);
    init(&center2, -1, 3.5, 0.5);
    sphere s1, s2;
    init_sphere(&s1, center1, 0.5, dif_light);
    init_sphere(&s2, center2, 0.5, dif_light);

    mesh *tree = load_mesh("teapot.obj", mat);
    //mesh *tree = load_mesh("untitled.obj", mat); //needs work
    //mesh *tree = load_mesh("cube.obj", mat);
    if(!tree){
        delete_texture(&(mat.tex));
        delete_list(&world); 
        return;
    }

    add_list_no_pdf(&world, tree->bvh, &hit_bvh, &get_bvh_box);
    add_list_no_pdf(&world, &s1, &hit_sphere, &get_sphere_box);
    add_list_no_pdf(&world, &s2, &hit_sphere, &get_sphere_box);


    hittable_list priorities;
    init_list(&priorities);
    add_list(&priorities, &s1, &hit_sphere, &get_sphere_box, &sphere_pdf_value, &sphere_pdf_generate);
    add_list(&priorities, &s2, &hit_sphere, &get_sphere_box, &sphere_pdf_value, &sphere_pdf_generate);
    
    //initializing camera
    camera cam;
    cam.aspect_ratio = 1.0;
    cam.image_width = 1000;
    cam.samples_per_pixel = 350;
    init(&(cam.background), 0, 0, 0);
    cam.max_depth = 50;
    cam.vfov = 70;
    
    point3 f, a, v;
    
    //teapot camera
    init(&f, 0, 4, 5);
    init(&a, 0.3, 0.5, 0);
    init(&v, 0, 1, 0);
     
    //tree camera
    /*init(&f, 1000, 0, 1000);
    init(&a, 0, 100, 0);
    init(&v, 0, 1, 0);*/
    
    copy(&(cam.lookfrom), f);
    copy(&(cam.lookat), a);
    copy(&(cam.vup), v);

    cam.defocus_angle = 0;
    cam.focus_dist = 2;

    render(&cam, &world, &priorities);
    
    delete_texture(&(mat.tex));
    delete_texture(&(dif_light.tex));
    delete_mesh(tree);
    delete_list(&priorities); 
    delete_list(&world); 
}


void test_skybox(){
    hittable_list world, priorities;
    init_list(&world);
    init_list(&priorities);
    
    color al;
    init(&al, 0.8, 0.85, 0.88);

    material aluminum;

    init_metal(&aluminum, al, 0.0);
    point3 c;
    init(&c, -1.8, 0.25, 0);
    sphere sph; 
    init_sphere(&sph, c, 0.5, aluminum);
    add_list_no_pdf(&world, &sph, &hit_sphere, &get_sphere_box);

    //initializing camera
    camera cam;
    cam.aspect_ratio = 1.0;
    cam.image_width = 1000;
    cam.samples_per_pixel = 350;
    //init(&(cam.background), 0, 0, 0);

    skybox s;
    init_skybox(&s, "forest.hdr");
    //init_skybox(&s, "earth.jpg");
    cam.sky = &s;

    cam.max_depth = 50;
    cam.vfov = 70;
    
    point3 f, a, v;
    
    //teapot camera
    init(&f, 0, 0, 0);
    init(&a, -1.8, 0.25, 0);
    init(&v, 0, 1, 0);
     
    copy(&(cam.lookfrom), f);
    copy(&(cam.lookat), a);
    copy(&(cam.vup), v);

    cam.defocus_angle = 0;
    cam.focus_dist = 2;

    render(&cam, &world, &priorities);
    
    delete_skybox(&s);
    delete_texture(&(aluminum.tex));
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
