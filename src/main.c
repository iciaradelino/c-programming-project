#include "simulation.h"
#include "visualization.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char **argv) {
    srand(time(NULL));

    int num_particles = 200;
    int visualize = 0;

    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--vis") == 0) visualize = 1;
        else num_particles = atoi(argv[i]);
    }

    Simulation sim;
    init_simulation(&sim, num_particles);

    if (visualize) {
        run_visualization(&sim);
    } else {
        printf("Running headless simulation (no visualization)...\n");
        run_simulation(&sim);
    }

    cleanup_simulation(&sim);
    return 0;
}
