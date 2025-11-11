#ifndef SIMULATION_H
#define SIMULATION_H

#include "physics.h"
#include <pthread.h> // <-- needed for threads

typedef struct {
    Particle *particles;
    int num_particles;
    BlackHole blackhole;
    double timestep;
    int steps;
    int num_threads;
    double G;
} Simulation;

// --- Threading support types ---
typedef struct {
    Simulation *sim;
    int start_idx;
    int end_idx;
} ThreadArgs;

// Function declarations
void *update_particles(void *args);  // <-- make this visible to other files
void init_simulation(Simulation *sim, int num_particles);
void run_simulation(Simulation *sim);
void cleanup_simulation(Simulation *sim);

#endif
