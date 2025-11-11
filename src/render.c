#include "render.h"

void render_step(Simulation *sim, FILE *out, int step) {
    for (int i = 0; i < sim->num_particles; i++) {
        fprintf(out, "%d,%d,%.4f,%.4f\n", step, sim->particles[i].id,
                sim->particles[i].position.x, sim->particles[i].position.y);
    }
}
