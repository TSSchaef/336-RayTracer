#include "image.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "../ext/stb_image.h"
#endif


static int clamp(int x, int low, int high){
    if(x < low) return low;
    if(x < high) return high;
    return high - 1;
}

static unsigned char float_to_byte(float value){
    if(value <= 0.0) return 0;
    if(1.0 <= value) return 255;
    return (unsigned char) 256.0 * value;
}

void convert_to_bytes(image *img){
    int total_bytes = img->image_width * img->image_height * img->bytes_per_pixel;
    img->bdata = malloc(total_bytes);
    
    int i;
    float *fptr = img->fdata;
    unsigned char *bptr = img->bdata;
    for(i = 0; i < total_bytes; i++, fptr++, bptr++){
        *bptr = float_to_byte(*fptr);
    }
}

bool load(image *i, char * filename){
    int n = i->bytes_per_pixel;
    i->fdata = (float *) stbi_load(filename, &(i->image_width), &(i->image_height), &n, i->bytes_per_pixel);

    if(!i->fdata) return false;

    i->bytes_per_scanline = i->image_width * i->bytes_per_pixel;
    convert_to_bytes(i);
    return true;
}

void load_image(image *i, const char *filename){
    i->bytes_per_pixel = 3;
    i->fdata = NULL;
    i->bdata = NULL;
    i->image_width = 0;
    i->image_height = 0;
    i->bytes_per_scanline = 0;

    if(load(i, strcat("img/", filename))) return; 
    if(load(i, strcat("../img/", filename))) return; 
    if(load(i, strcat("../../img/", filename))) return; 

    fprintf(stderr, "ERROR: Could not load image file %s\n", filename);
}

void delete_image(image *i){
    free(i->bdata);
    STBI_FREE(i->fdata);
}

const unsigned char* pixel_data(image i, int x, int y){
    static unsigned char magenta[] = {255, 0, 255};
    if(!i.bdata) return magenta;

    x = clamp(x, 0, i.image_width);
    y = clamp(y, 0, i.image_height);

    return i.bdata + y*i.bytes_per_scanline + x*i.bytes_per_pixel;
}
