#ifndef IMAGE_H
#define IMAGE_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    int bytes_per_pixel;
    float *fdata;
    unsigned char *bdata;
    int image_width;
    int image_height;
    int bytes_per_scanline;
} image;

void load_image(image *i, const char *filename);
void delete_image(image *i);
const unsigned char* pixel_data(image i, int x, int y);

#endif
