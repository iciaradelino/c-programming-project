#ifndef BLACKHOLE_3D_H
#define BLACKHOLE_3D_H

#include "physics.h"
#include "camera.h"
#include <pthread.h>

// object data for 3d simulation
typedef struct {
    Vector3D position;  // xyz position
    double radius;     // w component (stored separately)
    Vector3D color;    // rgb color
    double mass;
    Vector3D velocity;
} ObjectData;

// advanced black hole simulation
typedef struct {
    BlackHole3D blackhole;
    ObjectData *objects;
    int num_objects;
    int max_objects;
    int gravity_enabled;
    double G;
    double timestep;
    int num_threads;  // threading support
} BlackHoleSim3D;

// threading support for 3d simulation
typedef struct {
    BlackHoleSim3D *sim;
    int start_idx;
    int end_idx;
} ThreadArgs3D;

// threading support for grid generation
typedef struct {
    SpacetimeGrid *grid;
    BlackHoleSim3D *sim;
    double spacing;
    int start_vertex;
    int end_vertex;
    int grid_size;
} GridThreadArgs;

// grid vertex for spacetime visualization
typedef struct {
    Vector3D position;
} GridVertex;

// spacetime grid for visualizing curvature
typedef struct {
    GridVertex *vertices;
    int *indices;
    int num_vertices;
    int num_indices;
    int grid_size;
} SpacetimeGrid;

// function declarations
void blackhole_sim3d_init(BlackHoleSim3D *sim);
void blackhole_sim3d_add_object(BlackHoleSim3D *sim, Vector3D pos, double radius, 
                                 Vector3D color, double mass);
void blackhole_sim3d_update(BlackHoleSim3D *sim);
void blackhole_sim3d_cleanup(BlackHoleSim3D *sim);

void spacetime_grid_init(SpacetimeGrid *grid, int grid_size);
void spacetime_grid_generate(SpacetimeGrid *grid, BlackHoleSim3D *sim, double spacing);
void spacetime_grid_cleanup(SpacetimeGrid *grid);

// threaded versions (internal use)
void *update_objects_3d_threaded(void *args);
void *generate_grid_vertices_threaded(void *args);

#endif

