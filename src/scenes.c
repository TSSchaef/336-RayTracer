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
    cam.samples_per_pixel = 10;

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
    cam.image_width = 200;
    cam.samples_per_pixel = 20;
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
    cam.samples_per_pixel = 35;//00;

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

void space(){
    hittable_list world, priorities;
    init_list(&world);
    init_list(&priorities);
    
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

    /*mesh *skull = load_mesh("skull.obj", mat);
    if(!skull){
        delete_texture(&(mat.tex));
        delete_list(&world); 
        delete_list(&priorities); 
        return;
    }*/

    /*rotate r1, r2;
    init_rotate_no_pdf(&r1, skull->bvh, &hit_bvh, skull->bvh->bbox, 90);
    init_rotate_z_no_pdf(&r2, &r1, &hit_rotate, r1.bbox, 270);
    add_list_no_pdf(&skullObj, &r2, &hit_rotate_z, &get_rotate_box);*/


    /*translate t1, t2;
    vector3 vec1, vec2;
    init(&vec1, 0, 0, 26);
    init(&vec2, 0, 0, -26);
    init_translate_no_pdf(&t1, &skullObj, &hit, skullObj.box, vec1);
    init_translate_no_pdf(&t2, &skullObj, &hit, skullObj.box, vec2);*/

    //add_list_no_pdf(&world, &t1, &hit_translate, &get_translate_box);
    //add_list_no_pdf(&world, &t2, &hit_translate, &get_translate_box);

    init(&c3, 9, 0, -2);
    init_sphere(&l, c3, 8, mat);
    add_list(&world, &l, &hit_sphere, &get_sphere_box, &sphere_pdf_value, &sphere_pdf_generate);

    //add_list(&priorities, &l, &hit_sphere, &get_sphere_box, &sphere_pdf_value, &sphere_pdf_generate);
        
    texture perlin;
    init_perlin_tex(&perlin, 4);
    material perl;
    init_lambertian_tex(&perl, perlin);

    /*int i;
    const int NUM_SPHERES = 1000;
    sphere *s[NUM_SPHERES];
    point3 c;
    for(i = 0; i < NUM_SPHERES; i++){
        s[i] = (sphere *) malloc(sizeof(sphere));

        init(&c, rnd_int(-50, 50), rnd_int(-15, 0), rnd_int(-18, 18));
        init_sphere(s[i], c, 1.2, perl);
        add_list_no_pdf(&world, s[i], &hit_sphere, &get_sphere_box);
    }*/
    
    // turning world into bvh 
    bvh_node root;
    init_bvh(&root, &world);

    delete_list(&world);
    init_list(&world);
    add_list_no_pdf(&world, &root, &hit_bvh, &get_bvh_box);

    //initializing camera
    camera cam;
    cam.aspect_ratio = 1.0;
    cam.image_width = 1000;
    cam.samples_per_pixel = 35;//00;

    skybox sky;
    init_skybox(&sky, "blue-nebulae.hdr");
    cam.sky = &sky;

    cam.max_depth = 50;
    cam.vfov = 120;
    cam.defocus_angle = 10.0;
    cam.focus_dist = 15.0;
    
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

    /*for(i = 0; i < NUM_SPHERES; i++){
        free(s[i]);
    }*/
    
    delete_skybox(&sky);
    delete_texture(&(perlin));
    delete_texture(&(mat.tex));
    delete_texture(&(aluminum.tex));
    delete_texture(&(light.tex));
    //delete_mesh(skull);
    delete_bvh(&root);
    //delete_list(&skullObj); 
    delete_list(&priorities); 
    delete_list(&world); 
}

void teapot(){
    hittable_list world;
    init_list(&world);
    
    color r, red, l;
    init(&l, 15.0, 15.0, 15.0);
    init(&r, 0.9, 0.9, 0.75);
    init(&red, 100.0, 0, 0);

    material dif_red_light, dif_light, mat;

    init_lambertian(&mat, r);
    init_diffuse_light(&dif_red_light, red);
    init_diffuse_light(&dif_light, l);

    point3 center1, center2;
    init(&center1, 0, 4, 11);
    init(&center2, -1, 3.5, 0.5);
    sphere s1, s2;
    init_sphere(&s1, center1, 5.5, dif_light);
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
    cam.samples_per_pixel = 20;

    init(&(cam.background), 0, 0, 0);
    cam.max_depth = 50;
    cam.vfov = 70;
    cam.sky = NULL;
    
    point3 f, a, v;
    
    //teapot camera
    init(&f, 0, 4, 5);
    init(&a, 0.27, 1.25, 0);
    init(&v, 0, 1, 0);
    
    copy(&(cam.lookfrom), f);
    copy(&(cam.lookat), a);
    copy(&(cam.vup), v);

    cam.defocus_angle = 0;
    cam.focus_dist = 2;

    render(&cam, &world, &priorities);
    
    delete_texture(&(mat.tex));
    delete_texture(&(dif_red_light.tex));
    delete_texture(&(dif_light.tex));
    delete_mesh(teapot);
    delete_list(&priorities); 
    delete_list(&world); 
}


void cinematic_room(){
    hittable_list world, priorities;
    init_list(&world);
    init_list(&priorities);
    
    // Colors
    color wall_color, floor_color, trim_color, dark_trim_color, glass_color, sky_bg;
    color warm_light, cool_light, cloud_white, cloud_gray, cloud_light;
    color ground_green, grass_green, rock_gray, tree_green, trunk_brown;
    
    init(&wall_color, 0.85, 0.82, 0.78);      // Warm beige walls
    init(&floor_color, 0.35, 0.25, 0.20);     // Dark wood floor (fallback)
    init(&trim_color, 0.95, 0.94, 0.92);      // Off-white trim
    init(&dark_trim_color, 0.25, 0.20, 0.18); // Dark contrasting trim/baseboards
    init(&glass_color, 0.88, 0.90, 0.92);     // Slight blue tint for glass
    init(&sky_bg, 0.6, 0.7, 0.85);            // Soft blue sky
    init(&warm_light, 15.0, 13.0, 10.0);      // Brighter warm sunlight
    init(&cool_light, 10.0, 11.0, 13.0);      // Brighter cool sky light
    init(&cloud_white, 1.0, 1.0, 1.0);        // Pure white for clouds
    init(&cloud_gray, 0.85, 0.87, 0.90);      // Light gray clouds
    init(&cloud_light, 0.95, 0.96, 0.98);     // Very light clouds
    init(&ground_green, 0.12, 0.2, 0.08);      // Dark ground green
    init(&grass_green, 0.3, 0.5, 0.2);        // Bright grass
    init(&rock_gray, 0.4, 0.4, 0.42);         // Gray rocks
    init(&tree_green, 0.15, 0.25, 0.12);      // Dark pine green
    init(&trunk_brown, 0.3, 0.2, 0.15);       // Tree trunk brown
    
    // Textures
    texture wallpaper_tex, floor_tex;
    init_image_tex(&wallpaper_tex, "wallpaper.jpg");
    init_image_tex(&floor_tex, "floor.jpg");
    
    // Materials
    material wall_mat, floor_mat, trim_mat, dark_trim_mat, glass_mat;
    material warm_light_mat, cool_light_mat;
    material cloud_white_mat, cloud_gray_mat, cloud_light_mat;
    material wallpaper_mat, floor_texture_mat;
    material ground_mat, grass_mat, rock_mat, tree_mat, trunk_mat;
    
    init_lambertian(&wall_mat, wall_color);
    init_lambertian(&floor_mat, floor_color);
    init_lambertian_tex(&floor_texture_mat, floor_tex);
    init_lambertian_tex(&wallpaper_mat, wallpaper_tex);
    init_lambertian(&trim_mat, trim_color);
    init_lambertian(&dark_trim_mat, dark_trim_color);
    init_dielectric(&glass_mat, 1.5);
    init_diffuse_light(&warm_light_mat, warm_light);
    init_diffuse_light(&cool_light_mat, cool_light);
    init_isotropic(&cloud_white_mat, cloud_white);
    init_isotropic(&cloud_gray_mat, cloud_gray);
    init_isotropic(&cloud_light_mat, cloud_light);
    init_lambertian(&ground_mat, ground_green);
    init_lambertian(&grass_mat, grass_green);
    init_lambertian(&rock_mat, rock_gray);
    init_lambertian(&tree_mat, tree_green);
    init_lambertian(&trunk_mat, trunk_brown);
    
    // Room dimensions
    double room_width = 800;
    double room_height = 350;
    double room_depth = 600;
    
    // Floor with wood texture (tiled 3x3)
    int floor_i, floor_j;
    double floor_tile_width = room_width / 3.0;
    double floor_tile_depth = room_depth / 3.0;
    quad floor_tiles[9];
    int floor_idx = 0;
    for(floor_i = 0; floor_i < 3; floor_i++){
        for(floor_j = 0; floor_j < 3; floor_j++){
            point3 floor_Q;
            vector3 floor_u, floor_v;
            init(&floor_Q, floor_i * floor_tile_width, 0, floor_j * floor_tile_depth);
            init(&floor_u, floor_tile_width, 0, 0);
            init(&floor_v, 0, 0, floor_tile_depth);
            init_quad(&floor_tiles[floor_idx], floor_Q, floor_u, floor_v, floor_texture_mat);
            add_list_no_pdf(&world, &floor_tiles[floor_idx], &hit_quad, &get_quad_box);
            floor_idx++;
        }
    }
    
    // Ceiling
    quad ceiling;
    point3 ceil_Q;
    vector3 ceil_u, ceil_v;
    init(&ceil_Q, 0, room_height, 0);
    init(&ceil_u, room_width, 0, 0);
    init(&ceil_v, 0, 0, room_depth);
    init_quad(&ceiling, ceil_Q, ceil_u, ceil_v, wall_mat);
    add_list_no_pdf(&world, &ceiling, &hit_quad, &get_quad_box);
    
    // Window dimensions
    double window_width = 300;
    double window_height = 200;
    double window_bottom = 100;
    double window_left = (room_width - window_width) / 2.0;
    double frame_thickness = 8;
    
    // Wallpaper strip dimensions (centered, taller)
    double wallpaper_strip_height = 120;
    double wallpaper_strip_bottom = (room_height - wallpaper_strip_height) / 2.0;
    
    // Left wall with wallpaper strip (tiled horizontally)
    // Bottom section
    quad left_wall_bottom;
    point3 lwall_bot_Q;
    vector3 lwall_bot_u, lwall_bot_v;
    init(&lwall_bot_Q, 0, 0, 0);
    init(&lwall_bot_u, 0, wallpaper_strip_bottom, 0);
    init(&lwall_bot_v, 0, 0, room_depth);
    init_quad(&left_wall_bottom, lwall_bot_Q, lwall_bot_u, lwall_bot_v, wall_mat);
    add_list_no_pdf(&world, &left_wall_bottom, &hit_quad, &get_quad_box);
    
    // Wallpaper strip (tiled horizontally)
    int left_wall_paper_tiles = (int)(room_depth / wallpaper_strip_height) + 1;
    quad *left_wall_papers = malloc(left_wall_paper_tiles * sizeof(quad));
    int lwp_idx;
    for(lwp_idx = 0; lwp_idx < left_wall_paper_tiles; lwp_idx++){
        point3 lwall_paper_Q;
        vector3 lwall_paper_u, lwall_paper_v;
        init(&lwall_paper_Q, 0, wallpaper_strip_bottom, lwp_idx * wallpaper_strip_height);
        init(&lwall_paper_u, 0, wallpaper_strip_height, 0);
        init(&lwall_paper_v, 0, 0, wallpaper_strip_height);
        init_quad(&left_wall_papers[lwp_idx], lwall_paper_Q, lwall_paper_u, lwall_paper_v, wallpaper_mat);
        add_list_no_pdf(&world, &left_wall_papers[lwp_idx], &hit_quad, &get_quad_box);
    }
    
    // Top section
    quad left_wall_top;
    point3 lwall_top_Q;
    vector3 lwall_top_u, lwall_top_v;
    init(&lwall_top_Q, 0, wallpaper_strip_bottom + wallpaper_strip_height, 0);
    init(&lwall_top_u, 0, room_height - wallpaper_strip_bottom - wallpaper_strip_height, 0);
    init(&lwall_top_v, 0, 0, room_depth);
    init_quad(&left_wall_top, lwall_top_Q, lwall_top_u, lwall_top_v, wall_mat);
    add_list_no_pdf(&world, &left_wall_top, &hit_quad, &get_quad_box);
    
    // Right wall with wallpaper strip (tiled horizontally)
    // Bottom section
    quad right_wall_bottom;
    point3 rwall_bot_Q;
    vector3 rwall_bot_u, rwall_bot_v;
    init(&rwall_bot_Q, room_width, 0, 0);
    init(&rwall_bot_u, 0, wallpaper_strip_bottom, 0);
    init(&rwall_bot_v, 0, 0, room_depth);
    init_quad(&right_wall_bottom, rwall_bot_Q, rwall_bot_u, rwall_bot_v, wall_mat);
    add_list_no_pdf(&world, &right_wall_bottom, &hit_quad, &get_quad_box);
    
    // Wallpaper strip (tiled horizontally)
    quad *right_wall_papers = malloc(left_wall_paper_tiles * sizeof(quad));
    int rwp_idx;
    for(rwp_idx = 0; rwp_idx < left_wall_paper_tiles; rwp_idx++){
        point3 rwall_paper_Q;
        vector3 rwall_paper_u, rwall_paper_v;
        init(&rwall_paper_Q, room_width, wallpaper_strip_bottom, rwp_idx * wallpaper_strip_height);
        init(&rwall_paper_u, 0, wallpaper_strip_height, 0);
        init(&rwall_paper_v, 0, 0, wallpaper_strip_height);
        init_quad(&right_wall_papers[rwp_idx], rwall_paper_Q, rwall_paper_u, rwall_paper_v, wallpaper_mat);
        add_list_no_pdf(&world, &right_wall_papers[rwp_idx], &hit_quad, &get_quad_box);
    }
    
    // Top section
    quad right_wall_top;
    point3 rwall_top_Q;
    vector3 rwall_top_u, rwall_top_v;
    init(&rwall_top_Q, room_width, wallpaper_strip_bottom + wallpaper_strip_height, 0);
    init(&rwall_top_u, 0, room_height - wallpaper_strip_bottom - wallpaper_strip_height, 0);
    init(&rwall_top_v, 0, 0, room_depth);
    init_quad(&right_wall_top, rwall_top_Q, rwall_top_u, rwall_top_v, wall_mat);
    add_list_no_pdf(&world, &right_wall_top, &hit_quad, &get_quad_box);
    
    // Back wall (behind camera, with light sources) - plain no wallpaper
    quad back_wall;
    point3 bwall_Q;
    vector3 bwall_u, bwall_v;
    init(&bwall_Q, 0, 0, 0);
    init(&bwall_u, room_width, 0, 0);
    init(&bwall_v, 0, room_height, 0);
    init_quad(&back_wall, bwall_Q, bwall_u, bwall_v, wall_mat);
    add_list_no_pdf(&world, &back_wall, &hit_quad, &get_quad_box);
    
    // Hidden light sources behind camera (brighter now)
    sphere hidden_light1, hidden_light2;
    point3 light1_pos, light2_pos;
    init(&light1_pos, 200, 280, -50);
    init(&light2_pos, 600, 280, -50);
    init_sphere(&hidden_light1, light1_pos, 40, warm_light_mat);
    init_sphere(&hidden_light2, light2_pos, 40, cool_light_mat);
    add_list(&world, &hidden_light1, &hit_sphere, &get_sphere_box, &sphere_pdf_value, &sphere_pdf_generate);
    add_list(&world, &hidden_light2, &hit_sphere, &get_sphere_box, &sphere_pdf_value, &sphere_pdf_generate);
    //add_list(&priorities, &hidden_light1, &hit_sphere, &get_sphere_box, &sphere_pdf_value, &sphere_pdf_generate);
    //add_list(&priorities, &hidden_light2, &hit_sphere, &get_sphere_box, &sphere_pdf_value, &sphere_pdf_generate);
    
    // Simple window pane
    quad window_pane;
    point3 win_Q;
    vector3 win_u, win_v;
    init(&win_Q, window_left, window_bottom, room_depth);
    init(&win_u, window_width, 0, 0);
    init(&win_v, 0, window_height, 0);
    init_quad(&window_pane, win_Q, win_u, win_v, glass_mat);
    add_list_no_pdf(&world, &window_pane, &hit_quad, &get_quad_box);
    
    // Window frame - dark contrasting color
    point3 win_frame_top_p1, win_frame_top_p2;
    init(&win_frame_top_p1, window_left - frame_thickness, window_bottom + window_height, room_depth - frame_thickness);
    init(&win_frame_top_p2, window_left + window_width + frame_thickness, window_bottom + window_height + frame_thickness, room_depth + frame_thickness);
    hittable_list *window_frame_top = init_cube(win_frame_top_p1, win_frame_top_p2, dark_trim_mat);
    add_list_no_pdf(&world, window_frame_top, &hit, &get_list_box);
    
    point3 win_frame_bottom_p1, win_frame_bottom_p2;
    init(&win_frame_bottom_p1, window_left - frame_thickness, window_bottom - frame_thickness*2, room_depth - frame_thickness);
    init(&win_frame_bottom_p2, window_left + window_width + frame_thickness, window_bottom, room_depth + frame_thickness);
    hittable_list *window_frame_bottom = init_cube(win_frame_bottom_p1, win_frame_bottom_p2, dark_trim_mat);
    add_list_no_pdf(&world, window_frame_bottom, &hit, &get_list_box);
    
    point3 win_frame_left_p1, win_frame_left_p2;
    init(&win_frame_left_p1, window_left - frame_thickness, window_bottom, room_depth - frame_thickness);
    init(&win_frame_left_p2, window_left, window_bottom + window_height, room_depth + frame_thickness);
    hittable_list *window_frame_left = init_cube(win_frame_left_p1, win_frame_left_p2, dark_trim_mat);
    add_list_no_pdf(&world, window_frame_left, &hit, &get_list_box);
    
    point3 win_frame_right_p1, win_frame_right_p2;
    init(&win_frame_right_p1, window_left + window_width, window_bottom, room_depth - frame_thickness);
    init(&win_frame_right_p2, window_left + window_width + frame_thickness, window_bottom + window_height, room_depth + frame_thickness);
    hittable_list *window_frame_right = init_cube(win_frame_right_p1, win_frame_right_p2, dark_trim_mat);
    add_list_no_pdf(&world, window_frame_right, &hit, &get_list_box);
    
    // Baseboards (contrasting dark color)
    double baseboard_height = 15;
    double baseboard_depth = 3;
    
    point3 left_base_p1, left_base_p2;
    init(&left_base_p1, 0, 0, 0);
    init(&left_base_p2, baseboard_depth, baseboard_height, room_depth);
    hittable_list *left_baseboard = init_cube(left_base_p1, left_base_p2, dark_trim_mat);
    add_list_no_pdf(&world, left_baseboard, &hit, &get_list_box);
    
    point3 right_base_p1, right_base_p2;
    init(&right_base_p1, room_width - baseboard_depth, 0, 0);
    init(&right_base_p2, room_width, baseboard_height, room_depth);
    hittable_list *right_baseboard = init_cube(right_base_p1, right_base_p2, dark_trim_mat);
    add_list_no_pdf(&world, right_baseboard, &hit, &get_list_box);
    
    point3 back_base_p1, back_base_p2;
    init(&back_base_p1, 0, 0, 0);
    init(&back_base_p2, room_width, baseboard_height, baseboard_depth);
    hittable_list *back_baseboard = init_cube(back_base_p1, back_base_p2, dark_trim_mat);
    add_list_no_pdf(&world, back_baseboard, &hit, &get_list_box);
    
    // Front baseboard - continuous all the way across
    point3 front_base_p1, front_base_p2;
    init(&front_base_p1, 0, 0, room_depth - baseboard_depth);
    init(&front_base_p2, room_width, baseboard_height, room_depth);
    hittable_list *front_baseboard = init_cube(front_base_p1, front_base_p2, dark_trim_mat);
    add_list_no_pdf(&world, front_baseboard, &hit, &get_list_box);
    
    // Wall sections around window with wallpaper strips
    // Left wall section - bottom plain
    quad left_wall_section_bottom;
    point3 lwsec_bot_Q;
    vector3 lwsec_bot_u, lwsec_bot_v;
    init(&lwsec_bot_Q, 0, 0, room_depth);
    init(&lwsec_bot_u, window_left - frame_thickness, 0, 0);
    init(&lwsec_bot_v, 0, wallpaper_strip_bottom, 0);
    init_quad(&left_wall_section_bottom, lwsec_bot_Q, lwsec_bot_u, lwsec_bot_v, wall_mat);
    add_list_no_pdf(&world, &left_wall_section_bottom, &hit_quad, &get_quad_box);
    
    // Left wallpaper section (tiled horizontally)
    double left_section_width = window_left - frame_thickness;
    int left_section_tiles = (int)(left_section_width / wallpaper_strip_height) + 1;
    quad *left_section_papers = malloc(left_section_tiles * sizeof(quad));
    int lsp_idx;
    for(lsp_idx = 0; lsp_idx < left_section_tiles - 1; lsp_idx++){
        point3 lsec_paper_Q;
        vector3 lsec_paper_u, lsec_paper_v;
        init(&lsec_paper_Q, lsp_idx * wallpaper_strip_height , wallpaper_strip_bottom, room_depth);
        init(&lsec_paper_u, wallpaper_strip_height, 0, 0);
        init(&lsec_paper_v, 0, wallpaper_strip_height, 0);
        init_quad(&left_section_papers[lsp_idx], lsec_paper_Q, lsec_paper_u, lsec_paper_v, wallpaper_mat);
        add_list_no_pdf(&world, &left_section_papers[lsp_idx], &hit_quad, &get_quad_box);
    }

    //Adding last strip to fit exactly
    point3 lsec_paper_Q;
    vector3 lsec_paper_u, lsec_paper_v;
    init(&lsec_paper_Q, lsp_idx * wallpaper_strip_height , wallpaper_strip_bottom, room_depth);
    // TO DO re-write to not be a constant
    init(&lsec_paper_u, 12, 0, 0);
    init(&lsec_paper_v, 0, wallpaper_strip_height, 0);
    init_quad(&left_section_papers[lsp_idx], lsec_paper_Q, lsec_paper_u, lsec_paper_v, wallpaper_mat);
    add_list_no_pdf(&world, &left_section_papers[lsp_idx], &hit_quad, &get_quad_box);



    
    // Left top plain
    quad left_wall_section_top;
    point3 lwsec_top_Q;
    vector3 lwsec_top_u, lwsec_top_v;
    init(&lwsec_top_Q, 0, wallpaper_strip_bottom + wallpaper_strip_height, room_depth);
    init(&lwsec_top_u, window_left - frame_thickness, 0, 0);
    init(&lwsec_top_v, 0, room_height - wallpaper_strip_bottom - wallpaper_strip_height, 0);
    init_quad(&left_wall_section_top, lwsec_top_Q, lwsec_top_u, lwsec_top_v, wall_mat);
    add_list_no_pdf(&world, &left_wall_section_top, &hit_quad, &get_quad_box);
    
    // Right wall section - bottom plain
    double right_section_width = room_width - window_left - window_width - frame_thickness;
    
    quad right_wall_section_bottom;
    point3 rwsec_bot_Q;
    vector3 rwsec_bot_u, rwsec_bot_v;
    init(&rwsec_bot_Q, window_left + window_width + frame_thickness, 0, room_depth);
    init(&rwsec_bot_u, right_section_width, 0, 0);
    init(&rwsec_bot_v, 0, wallpaper_strip_bottom, 0);
    init_quad(&right_wall_section_bottom, rwsec_bot_Q, rwsec_bot_u, rwsec_bot_v, wall_mat);
    add_list_no_pdf(&world, &right_wall_section_bottom, &hit_quad, &get_quad_box);
    
    // Right wallpaper section
    int right_section_tiles = (int)(right_section_width / wallpaper_strip_height) + 1;
    quad *right_section_papers = malloc(right_section_tiles * sizeof(quad));
    int rsp_idx;
    for(rsp_idx = 0; rsp_idx < right_section_tiles; rsp_idx++){
        point3 rsec_paper_Q;
        vector3 rsec_paper_u, rsec_paper_v;
        init(&rsec_paper_Q, window_left + window_width + frame_thickness + rsp_idx * wallpaper_strip_height, wallpaper_strip_bottom, room_depth);
        init(&rsec_paper_u, wallpaper_strip_height, 0, 0);
        init(&rsec_paper_v, 0, wallpaper_strip_height, 0);
        init_quad(&right_section_papers[rsp_idx], rsec_paper_Q, rsec_paper_u, rsec_paper_v, wallpaper_mat);
        add_list_no_pdf(&world, &right_section_papers[rsp_idx], &hit_quad, &get_quad_box);
    }
    
    // Right top plain
    quad right_wall_section_top;
    point3 rwsec_top_Q;
    vector3 rwsec_top_u, rwsec_top_v;
    init(&rwsec_top_Q, window_left + window_width + frame_thickness, wallpaper_strip_bottom + wallpaper_strip_height, room_depth);
    init(&rwsec_top_u, right_section_width, 0, 0);
    init(&rwsec_top_v, 0, room_height - wallpaper_strip_bottom - wallpaper_strip_height, 0);
    init_quad(&right_wall_section_top, rwsec_top_Q, rwsec_top_u, rwsec_top_v, wall_mat);
    add_list_no_pdf(&world, &right_wall_section_top, &hit_quad, &get_quad_box);
    
    // Above window wall section - plain
    quad above_window;
    point3 awin_Q;
    vector3 awin_u, awin_v;
    init(&awin_Q, window_left - frame_thickness, window_bottom + window_height + frame_thickness, room_depth);
    init(&awin_u, window_width + frame_thickness*2, 0, 0);
    init(&awin_v, 0, room_height - window_bottom - window_height - frame_thickness, 0);
    init_quad(&above_window, awin_Q, awin_u, awin_v, wall_mat);
    add_list_no_pdf(&world, &above_window, &hit_quad, &get_quad_box);
    
    // Below window wall section - plain
    quad below_window;
    point3 blwin_Q;
    vector3 blwin_u, blwin_v;
    init(&blwin_Q, window_left - frame_thickness, 0, room_depth);
    init(&blwin_u, window_width + frame_thickness*2, 0, 0);
    init(&blwin_v, 0, window_bottom - frame_thickness*2, 0);
    init_quad(&below_window, blwin_Q, blwin_u, blwin_v, wall_mat);
    add_list_no_pdf(&world, &below_window, &hit_quad, &get_quad_box);
   


    // Outdoor scene - ground plane (extended to behind room and wider)
    quad ground;
    point3 ground_Q;
    vector3 ground_u, ground_v;
    init(&ground_Q, -3000, -5, -500);  // Extended behind room and lowered
    init(&ground_u, 7000, 0, 0);       // Much wider
    init(&ground_v, 0, 0, 6000);       // Extended depth
    init_quad(&ground, ground_Q, ground_u, ground_v, ground_mat);
    add_list_no_pdf(&world, &ground, &hit_quad, &get_quad_box);
    
    // Outdoor light sources (positioned high and to sides, 25% brighter)
    sphere outdoor_light1, outdoor_light2, outdoor_light3;
    point3 out_light1_pos, out_light2_pos, out_light3_pos;
    init(&out_light1_pos, room_width / 2.0 - 500, 600, room_depth + 300);
    init(&out_light2_pos, room_width / 2.0 + 500, 600, room_depth + 300);
    init(&out_light3_pos, room_width / 2.0, 700, room_depth + 200);
    
    // Creating brighter outdoor light materials (25% increase)
    color warm_outdoor, cool_outdoor;
    init(&warm_outdoor, 187.5, 162.5, 125);
    init(&cool_outdoor, 125, 137.5, 162.5);
    material warm_outdoor_mat, cool_outdoor_mat;
    init_diffuse_light(&warm_outdoor_mat, warm_outdoor);
    init_diffuse_light(&cool_outdoor_mat, cool_outdoor);
    
    init_sphere(&outdoor_light1, out_light1_pos, 50, warm_outdoor_mat);
    init_sphere(&outdoor_light2, out_light2_pos, 50, warm_outdoor_mat);
    init_sphere(&outdoor_light3, out_light3_pos, 60, cool_outdoor_mat);
    add_list(&world, &outdoor_light1, &hit_sphere, &get_sphere_box, &sphere_pdf_value, &sphere_pdf_generate);
    add_list(&world, &outdoor_light2, &hit_sphere, &get_sphere_box, &sphere_pdf_value, &sphere_pdf_generate);
    add_list(&world, &outdoor_light3, &hit_sphere, &get_sphere_box, &sphere_pdf_value, &sphere_pdf_generate);
    //add_list(&priorities, &outdoor_light1, &hit_sphere, &get_sphere_box, &sphere_pdf_value, &sphere_pdf_generate);
    //add_list(&priorities, &outdoor_light2, &hit_sphere, &get_sphere_box, &sphere_pdf_value, &sphere_pdf_generate);
    //add_list(&priorities, &outdoor_light3, &hit_sphere, &get_sphere_box, &sphere_pdf_value, &sphere_pdf_generate);
    
    // Small rocks scattered on ground (more rocks, properly placed on visible ground)
    int num_rocks = 120;
    sphere *rocks = malloc(num_rocks * sizeof(sphere));
    int rock_i;
    for(rock_i = 0; rock_i < num_rocks; rock_i++){
        point3 rock_pos;
        init(&rock_pos, 
             rnd_dbl(room_width / 2.0 - 600, room_width / 2.0 + 600),
             rnd_dbl(0.2, 0.5),
             rnd_dbl(room_depth + 50, room_depth + 2000));
        init_sphere(&rocks[rock_i], rock_pos, rnd_dbl(0.2, 0.8), rock_mat);
        //add_list_no_pdf(&world, &rocks[rock_i], &hit_sphere, &get_sphere_box);
    }
    
    // Grass blades (more grass, visible through window)
    int num_grass = 250;
    triangle *grass_blades = malloc(num_grass * sizeof(triangle));
    int grass_i;
    for(grass_i = 0; grass_i < num_grass; grass_i++){
        point3 grass_base, grass_left, grass_right;
        double grass_x = rnd_dbl(room_width / 2.0 - 500, room_width / 2.0 + 500);
        double grass_z = rnd_dbl(room_depth + 50, room_depth + 1500);
        double grass_height = rnd_dbl(0.5, 2.0);
        init(&grass_base, grass_x, 0, grass_z);
        init(&grass_left, grass_x - 0.3, grass_height, grass_z);
        init(&grass_right, grass_x + 0.3, grass_height, grass_z);
        init_triangle(&grass_blades[grass_i], grass_base, grass_left, grass_right, grass_mat);
        //add_list_no_pdf(&world, &grass_blades[grass_i], &hit_triangle, &get_triangle_box);
    }
    
    // Pine trees (positioned to be visible through window)
    int num_trees = 20;
    triangle *tree_triangles = malloc(num_trees * sizeof(triangle));
    quad *tree_trunks = malloc(num_trees * sizeof(quad));
    int tree_i;
    for(tree_i = 0; tree_i < num_trees; tree_i++){
        double tree_x = rnd_dbl(room_width / 2.0 - 400, room_width / 2.0 + 400);
        double tree_z = rnd_dbl(room_depth + 1000, room_depth + 3500);
        double tree_height = rnd_dbl(50, 90);
        double trunk_height = tree_height * 0.25;
        double trunk_width = 4;
        double tree_width = tree_height * 0.5;
        
        // Trunk (vertical quad)
        point3 trunk_Q;
        vector3 trunk_u, trunk_v;
        init(&trunk_Q, tree_x - trunk_width/2.0, 0, tree_z);
        init(&trunk_u, trunk_width, 0, 0);
        init(&trunk_v, 0, trunk_height, 0);
        init_quad(&tree_trunks[tree_i], trunk_Q, trunk_u, trunk_v, trunk_mat);
        //add_list_no_pdf(&world, &tree_trunks[tree_i], &hit_quad, &get_quad_box);
        
        // Single triangle for pine shape
        point3 tree_top, tree_left, tree_right;
        init(&tree_top, tree_x, trunk_height + tree_height, tree_z);
        init(&tree_left, tree_x - tree_width, trunk_height, tree_z - tree_width * 0.5);
        init(&tree_right, tree_x + tree_width, trunk_height, tree_z + tree_width * 0.5);
        init_triangle(&tree_triangles[tree_i], tree_top, tree_left, tree_right, tree_mat);
        //add_list_no_pdf(&world, &tree_triangles[tree_i], &hit_triangle, &get_triangle_box);
    }
    
    // Cloud clumps (repositioned to be visible through window)
    int num_clouds = 120;
    sphere *clouds = malloc(num_clouds * sizeof(sphere));
    constant_medium *cloud_volumes = malloc(num_clouds * sizeof(constant_medium));
    point3 cloud_pos;
    
    // Clump 1 - left side, white
    int i;
    for(i = 0; i < 30; i++){
        init(&cloud_pos, 
             rnd_dbl(room_width / 2.0 - 350, room_width / 2.0 - 100), 
             rnd_dbl(200, 320), 
             rnd_dbl(room_depth + 800, room_depth + 2000));
        
        double cloud_size = rnd_dbl(20, 40);
        init_sphere(&clouds[i], cloud_pos, cloud_size, cloud_white_mat);
        
        init_constant_medium(&cloud_volumes[i], &clouds[i], &hit_sphere, 
                           0.015, clouds[i].bbox, cloud_white);
        
        //add_list_no_pdf(&world, &cloud_volumes[i], &hit_constant_medium, 
                       //&get_constant_medium_box);
    }
    
    // Clump 2 - center, light gray
    for(i = 30; i < 60; i++){
        init(&cloud_pos, 
             rnd_dbl(room_width / 2.0 - 150, room_width / 2.0 + 150), 
             rnd_dbl(220, 350), 
             rnd_dbl(room_depth + 1200, room_depth + 2500));
        
        double cloud_size = rnd_dbl(25, 45);
        init_sphere(&clouds[i], cloud_pos, cloud_size, cloud_light_mat);
        
        init_constant_medium(&cloud_volumes[i], &clouds[i], &hit_sphere, 
                           0.012, clouds[i].bbox, cloud_light);
        
        //add_list_no_pdf(&world, &cloud_volumes[i], &hit_constant_medium, 
                       //&get_constant_medium_box);
    }
    
    // Clump 3 - right side, gray
    for(i = 60; i < 90; i++){
        init(&cloud_pos, 
             rnd_dbl(room_width / 2.0 + 100, room_width / 2.0 + 350), 
             rnd_dbl(190, 310), 
             rnd_dbl(room_depth + 900, room_depth + 2200));
        
        double cloud_size = rnd_dbl(22, 42);
        init_sphere(&clouds[i], cloud_pos, cloud_size, cloud_gray_mat);
        
        init_constant_medium(&cloud_volumes[i], &clouds[i], &hit_sphere, 
                           0.013, clouds[i].bbox, cloud_gray);
        
        //add_list_no_pdf(&world, &cloud_volumes[i], &hit_constant_medium, 
                       //&get_constant_medium_box);
    }
    
    // Clump 4 - scattered background, very light
    for(i = 90; i < 120; i++){
        init(&cloud_pos, 
             rnd_dbl(room_width / 2.0 - 400, room_width / 2.0 + 400), 
             rnd_dbl(250, 380), 
             rnd_dbl(room_depth + 1800, room_depth + 3500));
        
        double cloud_size = rnd_dbl(30, 50);
        init_sphere(&clouds[i], cloud_pos, cloud_size, cloud_white_mat);
        
        init_constant_medium(&cloud_volumes[i], &clouds[i], &hit_sphere, 
                           0.010, clouds[i].bbox, cloud_white);
        
        //add_list_no_pdf(&world, &cloud_volumes[i], &hit_constant_medium, 
                       //&get_constant_medium_box);
    }
    
    // Build BVH
    bvh_node root;
    init_bvh(&root, &world);
    
    delete_list(&world);
    init_list(&world);
    add_list_no_pdf(&world, &root, &hit_bvh, &get_bvh_box);
    
    // Camera setup
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 3840;
    //cam.image_width = 1920;
    cam.samples_per_pixel = 5000;
    
    init(&(cam.background), sky_bg.e[0], sky_bg.e[1], sky_bg.e[2]);

    cam.sky = NULL;
    
    cam.max_depth = 50;
    cam.vfov = 60;
    
    point3 f, a, v;
    init(&f, 180, 140, 150);
    init(&a, room_width / 2.0, 160, room_depth + 20);
    init(&v, 0, 1, 0);
    
    copy(&(cam.lookfrom), f);
    copy(&(cam.lookat), a);
    copy(&(cam.vup), v);
    
    cam.defocus_angle = 0.8;
    cam.focus_dist = 450;
    
    render(&cam, &world, &priorities);
    
    // Cleanup
    for(i = 0; i < num_clouds; i++){
        delete_texture(&(cloud_volumes[i].phase_func.tex));
    }
    free(clouds);
    free(cloud_volumes);
    
    free(rocks);
    free(grass_blades);
    free(tree_triangles);
    free(tree_trunks);
    free(left_wall_papers);
    free(right_wall_papers);
    free(left_section_papers);
    free(right_section_papers);
    
    delete_texture(&(wall_mat.tex));
    delete_texture(&(floor_mat.tex));
    delete_texture(&(trim_mat.tex));
    delete_texture(&(dark_trim_mat.tex));
    delete_texture(&(glass_mat.tex));
    delete_texture(&(warm_light_mat.tex));
    delete_texture(&(cool_light_mat.tex));
    delete_texture(&(cloud_white_mat.tex));
    delete_texture(&(warm_outdoor_mat.tex));
    delete_texture(&(cool_outdoor_mat.tex));
    delete_texture(&(cloud_gray_mat.tex));
    delete_texture(&(cloud_light_mat.tex));
    delete_texture(&(ground_mat.tex));
    delete_texture(&(grass_mat.tex));
    delete_texture(&(rock_mat.tex));
    delete_texture(&(tree_mat.tex));
    delete_texture(&(trunk_mat.tex));
    delete_image_tex(&wallpaper_tex);
    delete_image_tex(&floor_tex);
    
    delete_cube(window_frame_top);
    delete_cube(window_frame_bottom);
    delete_cube(window_frame_left);
    delete_cube(window_frame_right);
    delete_cube(left_baseboard);
    delete_cube(right_baseboard);
    delete_cube(back_baseboard);
    delete_cube(front_baseboard);
    
    delete_bvh(&root);
    delete_list(&priorities);
    delete_list(&world);
}

void render_scene(int scene_id){
    switch(scene_id){
        case 1: cornell_box(); break;
        case 2: triangle_test(); break;
        case 3: test_skybox(); break;
        case 4: space(); break;
        case 5: teapot(); break;
        case 6: cinematic_room(); break;
    }   
}
