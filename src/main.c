#include <stdlib.h>
#include <mpi.h>
#include "scenes.h"
#include "util.h"

int main(int argc, char *argv[]){
    // Initialize MPI
    MPI_Init(&argc, &argv);
    
    int rank, num_processes;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    
    // Initialize random seed with rank for different sequences per process
    init_random_seed(rank);
    
    int scene_id;
    if(argc >= 2){
        scene_id = atoi(argv[1]); 
    } else {
        if(rank == 0){
            fprintf(stderr, "Usage: mpirun -np <num_processes> %s <scene_id>\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }

    render_scene(scene_id);
    
    // Finalize MPI
    MPI_Finalize();
    return 0;
}
