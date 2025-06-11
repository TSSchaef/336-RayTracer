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
    
    size_t s;
    s = fread(*data, 1, size, f);
    if(s != size){
        fprintf(stderr, "ERROR: Error reading mesh data\n");
        return;
    }
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

mesh *load_mesh(const char* filename, material mat){
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

    mesh *m = (mesh *) malloc(sizeof(mesh));
    m->list = (hittable_list *) malloc(sizeof(hittable_list));
    m->bvh = NULL;
    init_list(m->list);

    int i;
    point3 *points = (point3 *) malloc(d.attrib.num_vertices * sizeof(point3));
    for(i = 0 ; i < d.attrib.num_vertices; i++){
        init(points + i, d.attrib.vertices[3 * i], d.attrib.vertices[(3 * i) + 1], d.attrib.vertices[(3 * i) + 2]);
    }

    for(i = 0; i + 2 < d.attrib.num_faces; i += 3){
        int f0 = d.attrib.faces[i + 0].v_idx;
        int f1 = d.attrib.faces[i + 1].v_idx;
        int f2 = d.attrib.faces[i + 2].v_idx;

        if(f0 < 0 || f0 > d.attrib.num_vertices ||
                f1 < 0 || f1 > d.attrib.num_vertices ||
                f2 < 0 || f2 > d.attrib.num_vertices) {
            continue;
        }

        //no normal for triangle in obj file
        triangle *t = (triangle *) malloc(sizeof(triangle));
        init_triangle(t, points[f0], points[f1], points[f2], mat);
        add_list_no_pdf(m->list, t, &hit_triangle, &get_triangle_box);
    }

    free(points);

    tinyobj_attrib_free(&(d.attrib));
    tinyobj_shapes_free(d.shapes, d.num_shapes);
    tinyobj_materials_free(d.materials, d.num_materials);

    if(m->list->size < 1){
        delete_list(m->list);
        free(m->list);
        free(m);
        return NULL;
    }
        
    m->bvh = (bvh_node *) malloc(sizeof(bvh_node));
    init_bvh(m->bvh, m->list);

    return m;
}

void delete_mesh(mesh *m){
    if(m->bvh){
        delete_bvh(m->bvh);
        free(m->bvh);
    }
    if(m->list){
        int i;
        for(i = 0; i < m->list->size; i++){
            free(m->list->list[i]->hittable);
        }

        delete_list(m->list);
        free(m->list);
    }
    free(m);
}
