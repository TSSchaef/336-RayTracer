#ifndef MESH_H
#define MESH_H

#include <string.h>

#include "util.h"
#include "bvh.h"
#include "hittable_list.h"
#include "triangle.h"
#include "material.h"

bvh_node *load_mesh(const char* filename, material m);
void delete_mesh(bvh_node *b);

#endif
