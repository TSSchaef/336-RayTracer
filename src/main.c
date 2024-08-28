#include <stdio.h>

int main(int argc, char *argv[]){
    int image_width = 256;
    int image_height = 256;

    printf("P3\n %d %d\n255\n", image_width, image_height);

    int i, j;
    for(i = 0; i < image_height; i++) {
        for(j = 0; j < image_width; j++){
            double g = (i + 0.0) / (image_height - 1);
            double r = (j + 0.0) / (image_width - 1);
            double b = 0.0;

            int ir = (int) 255.999 * r;
            int ig = (int) 255.999 * g;
            int ib = (int) 255.999 * b;

            printf("%d %d %d\n", ir, ig, ib);
        }
    }
    return 0;
}
