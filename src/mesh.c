#include "mesh.h"

#define TINYOBJ_LOADER_C_IMPLEMENTATION
#include "../ext/tinyobj_loader_c.h"

typedef struct {
    tinyobj_attrib_t attrib;
    tinyobj_shape_t *shapes;
    size_t num_shapes;
    tinyobj_material_t *materials;
    size_t num_materials;
} obj_data;

typedef struct {
    char *mtl;
    char *obj;
} context;

static void get_file_data(void* ctx, const char* filename, const int is_mtl,
        const char* obj_filename, char** data, size_t* len) {
    static int call_cnt = 0;
    if (!filename) {
        fprintf(stderr, "Null filename\n");
        (*data) = NULL;
        (*len) = 0;
        return;
    }

    FILE *f = fopen(filename, "r");
    if(!f){
        (*data) = NULL;
        (*len) = 0;
        return;
    }

    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    rewind(f);
       
    *data = (char *) malloc(size + 1);
    if(!(*data)){
        (*len) = 0;
        fclose(f);
        return;
    }

    context *c = (context *) ctx;
    if(call_cnt == 0){
        c->mtl = *data;
    } else {
        c->obj = *data;
    }

    fread(*data, 1, size, f);
    (*data)[size] = '\0';

    fclose(f);

    (*len) = size;
    ctx = *data;
    call_cnt++;
}

static bool load(obj_data *d, char * filename){
    context c;
    c.mtl = NULL;
    c.obj = NULL;

    int ret = tinyobj_parse_obj(&(d->attrib), &(d->shapes), &(d->num_shapes), &(d->materials), &(d->num_materials), filename, &get_file_data, &c, TINYOBJ_FLAG_TRIANGULATE);
    
    if (ret == TINYOBJ_SUCCESS){
        if(c.mtl) free(c.mtl);
        if(c.obj) free(c.obj);
        return true;
    }
    return false;
}

hittable_list *load_mesh(const char* filename, material m){
    obj_data d;
    d.shapes = NULL;
    d.materials = NULL;

    char str1[25] = "img/";
    char str2[25] = "../img/";
    char str3[25] = "../../img/";

    if(!load(&d, strcat(str1, filename))){
        if(!load(&d, strcat(str2, filename))){
            if(!load(&d, strcat(str3, filename))){
                fprintf(stderr, "ERROR: Could not load image file %s\n", filename);
                return NULL;
            }
        }
    }

    hittable_list *list = malloc(sizeof(hittable_list));  
    init_list(list);

    int i, f;
    point3 *points = (point3 *) malloc(d.attrib.num_vertices * sizeof(point3));
    for(i = 0 ; i < d.attrib.num_vertices; i++){
        init(points + i, d.attrib.vertices[3 * i], d.attrib.vertices[(3 * i) + 1], d.attrib.vertices[(3 * i) + 2]);
    }

    
    
    for(i = 0; i < d.attrib.num_face_num_verts; i++){
        if(d.attrib.face_num_verts[i] % 3 != 0){
            fprintf(stderr, "ERROR: Face is not a triangle!");
        } else {
            for(f = 0; f < d.attrib.face_num_verts[i] / 3; f++){
                int f0 = d.attrib.faces[(3*f) + 0].v_idx;
                int f1 = d.attrib.faces[(3*f) + 1].v_idx;
                int f2 = d.attrib.faces[(3*f) + 2].v_idx;

                if(f0 < 0 || f0 > d.attrib.num_vertices ||
                   f1 < 0 || f1 > d.attrib.num_vertices ||
                   f2 < 0 || f2 > d.attrib.num_vertices) {
                    continue;
                }

                vector3 normal;
                int nid = d.attrib.faces[(3*f) + 0].vn_idx;
                init(&normal, d.attrib.normals[nid], d.attrib.normals[nid + 1], d.attrib.normals[nid + 2]);

                triangle *t = (triangle *) malloc(sizeof(triangle));
                init_triangle_norm(t, points[f2], points[f0], points[f1], normal, m);
                add_list(list, t, &hit_triangle, &get_triangle_box);
            }
        }
    }

    free(points);

    tinyobj_attrib_free(&(d.attrib));
    tinyobj_shapes_free(d.shapes, d.num_shapes);
    tinyobj_materials_free(d.materials, d.num_materials);

    if(list->size < 1){
        delete_list(list);
        return NULL;
    }

    return list;
        
    /*bvh_node *root = (bvh_node *) malloc(sizeof(bvh_node));
    init_bvh(root, &list);
    delete_list(&list);

    return root;*/
}

void delete_mesh(bvh_node *b){
    delete_bvh_and_data(b);
    free(b);
}
