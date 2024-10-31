#ifndef SCENES_H
#define SCENES_H

#include "camera.h"
#include "material.h"
#include "mesh.h"
#include "texture.h"
#include "hittable_list.h"
#include "sphere.h"
#include "bvh.h"
#include "quad.h"
#include "triangle.h"
#include "instance.h"
#include "constant_medium.h"
#include "util.h"
#include "skybox.h"

#define NUM_SCENES 3

void render_scene(int scene_id);

#endif
