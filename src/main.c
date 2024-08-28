#include <stdio.h>

#include "vector3.h"

int main(int argc, char *argv[]){
    int image_width = 256;
    int image_height = 256;

    printf("P3\n %d %d\n255\n", image_width, image_height);

    color c;
    int i, j;
    for(i = 0; i < image_height; i++) {
        fprintf(stderr, "\rScanlines remaining: %d", image_height - j);
        fflush(stderr);
        for(j = 0; j < image_width; j++){
            init(&c, (j + 0.0) / (image_width - 1), (i + 0.0)/ (image_height - 1), 0.0);
            print_color(c);
        }
    }
    fprintf(stderr, "\rDone.                       \n");
    return 0;
}
