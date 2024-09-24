TSSchaef Ray Tracer
===================

Setup
------
    After cloning the repo, in order to build it place the dependencies into the /ext folder. (TODO add a script for automatically downloading dependencies). Then call **"make"** to build the ray tracer (Currently still on a development build). The ray tracer can then be run with the **run.sh** script.

### Dependencies
* [stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h "stb_image.h") - Used for loading images for textures
* [tinyobj_loader_c.h](https://github.com/syoyo/tinyobjloader-c/blob/master/tinyobj_loader_c.h "tinyobj_loader_c.h") - Used for loading object files for meshes of triangles



Overview
--------
### Intro
    This ray tracer was originally developed for CS336 (Intro to Computer Graphics).

### Features 
    The ray tracer currently implements:
     *Parallelization on CPU's
     *Loading of object files as Triangle Meshes
     *Object Instancing
     *Loading images as textures
     *Perlin Noise Texturing
     *Lambertian, Diffuse, and Dielectric Materials
     *Axis-Aligned Bounding Boxes
     *Bounding Volume Heirarchy
     *Participating Media
     *Positionable Camera
     *Depth of Field

### Usage 
     Scenes are currently written in **main.c**, there is a switch statement in the main function that can be used to select a given scene. (TODO write an example scene describing how to build a scene).

### Plans 
     Current plans for the project include adding features for:
     *GPU acceleration with Nvidia's CUDA
     *Optimizing Triangle Meshes
     *Optimizing the BVH
     *Adding Importance Sampling
     *Adding Adaptive Sampling
     *Extending Object Instancing
     *Adding a BRDF

### Resources 
     Credit for resources used in development:
     *[_Ray Tracing in One Weekend_](https://raytracing.github.io/)
     *[_Physically Based Rendering:From Theory to Implementation_](https://pbr-book.org/)

