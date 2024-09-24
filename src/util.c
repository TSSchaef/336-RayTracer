#include "util.h"

double rnd_double(){
    static unsigned int seed = -1;
    if(seed == -1){
        seed = time(NULL);
    }
    return (rand_r(&seed) / (RAND_MAX + 1.0));
}

double rnd_dbl(double min, double max){
    return (min + (max - min) * rnd_double());
}

