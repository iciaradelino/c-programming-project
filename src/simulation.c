#include "simulation.h"
#include "utils.h"
#include "render.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>


void *update_particles(void *args) {
    ThreadArgs *targs = (ThreadArgs *)args;
    Simulation *sim = targs->sim;
    for (int i = targs->start_idx; i < targs->end_idx; i++) {
        Particle *p = &sim->particles[i];
        Vector2D a = compute_acceleration(p, &sim->blackhole, sim->G);
        p->velocity = vector_add(p->velocity, vector_scale(a, sim->timestep));
        p->position = vector_add(p->position, vector_scale(p->velocity, sim->timestep));
    }
    return NULL;
}

void init_simulation(Simulation *sim, int num_particles) {
    sim->num_particles = num_particles;
    sim->particles = malloc(sizeof(Particle) * num_particles);
    sim->blackhole.mass = 1e6;
    sim->blackhole.position.x = 0;
    sim->blackhole.position.y = 0;
    sim->G = 6.674e-3;
    sim->timestep = 0.05;
    sim->steps = 2000;
    sim->num_threads = 4;

    for (int i = 0; i < num_particles; i++) {
        sim->particles[i] = random_particle(i);
    }
}

void run_simulation(Simulation *sim) {
    FILE *out = fopen("output.csv", "w");
    fprintf(out, "step,id,x,y\n");

    for (int step = 0; step < sim->steps; step++) {
        pthread_t threads[sim->num_threads];
        ThreadArgs args[sim->num_threads];
        int chunk = sim->num_particles / sim->num_threads;

        for (int t = 0; t < sim->num_threads; t++) {
            args[t].sim = sim;
            args[t].start_idx = t * chunk;
            args[t].end_idx = (t == sim->num_threads - 1) ? sim->num_particles : (t + 1) * chunk;
            pthread_create(&threads[t], NULL, update_particles, &args[t]);
        }

        for (int t = 0; t < sim->num_threads; t++) pthread_join(threads[t], NULL);

        render_step(sim, out, step);
    }

    fclose(out);
}

void cleanup_simulation(Simulation *sim) {
    free(sim->particles);
}
