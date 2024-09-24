#include <stdlib.h>
#include "scenes.h"

int main(int argc, char *argv[]){
    int scene_id;
    if(argc >= 2){
        scene_id = atoi(argv[1]); 
    } else {
        scene_id = NUM_SCENES; 
    }

    render_scene(scene_id);
    return 0;
}
