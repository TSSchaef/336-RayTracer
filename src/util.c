#include "util.h"

static unsigned int seed = -1;

int rnd_int(int min, int max){
    if(seed == -1){
        seed = time(NULL);
    }
    return (rand_r(&seed) % (max - min + 1)) + min;
}

double rnd_double(){
    if(seed == -1){
        seed = time(NULL);
    }
    return (rand_r(&seed) / (RAND_MAX + 1.0));
}

double rnd_dbl(double min, double max){
    return (min + (max - min) * rnd_double());
}

