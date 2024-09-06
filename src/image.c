#include "image.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "../ext/stb_image.h"
#endif

bool load(image *i, char * filename){
    int n = i->bytes_per_pixel;
    i->bdata = stbi_load(filename, &(i->image_width), &(i->image_height), &n, i->bytes_per_pixel);

    if(!i->bdata) return false;

    i->bytes_per_scanline = i->image_width * i->bytes_per_pixel;
    return true;
}

void load_image(image *i, const char *filename){
    i->bytes_per_pixel = 3;
    i->bdata = NULL;
    i->image_width = 0;
    i->image_height = 0;
    i->bytes_per_scanline = 0;

    char str1[] = "img/";
    char str2[] = "../img/";
    char str3[] = "../../img/";
    if(load(i, strcat(str1, filename))) return; 
    if(load(i, strcat(str2, filename))) return; 
    if(load(i, strcat(str3, filename))) return; 

    fprintf(stderr, "ERROR: Could not load image file %s\n", filename);
}

void delete_image(image *i){
    STBI_FREE(i->bdata);
}

static int clamp(int x, int low, int high){
    if(x < low) return low;
    if(x < high) return x;
    return high - 1;
}

const unsigned char* pixel_data(image *i, int x, int y){
    static unsigned char magenta[] = {255, 0, 255};
    if(!i->bdata) return magenta;

    x = clamp(x, 0, i->image_width);
    y = clamp(y, 0, i->image_height);

    return  i->bdata + y*i->bytes_per_scanline + x*i->bytes_per_pixel; 
}
