#include "blackhole_3d.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <pthread.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

void blackhole_sim3d_init(BlackHoleSim3D *sim) {
    // initialize sagittarius a* black hole
    sim->blackhole.position.x = 0.0;
    sim->blackhole.position.y = 0.0;
    sim->blackhole.position.z = 0.0;
    sim->blackhole.mass = 8.54e36;  // sagittarius a* mass in kg
    sim->blackhole.radius = 0.0;  // will be calculated
    sim->blackhole.r_s = schwarzschild_radius(sim->blackhole.mass);
    
    sim->max_objects = 16;
    sim->num_objects = 0;
    sim->objects = malloc(sizeof(ObjectData) * sim->max_objects);
    sim->gravity_enabled = 0;
    sim->G = G_CONST;
    sim->timestep = 0.1;
    
    // detect number of cpu cores for threading
    #ifdef _WIN32
    #ifdef _MSC_VER
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    sim->num_threads = sysinfo.dwNumberOfProcessors;
    #else
    // mingw fallback - use environment variable or default
    const char *env_threads = getenv("OMP_NUM_THREADS");
    sim->num_threads = env_threads ? atoi(env_threads) : 4;
    #endif
    #else
    sim->num_threads = sysconf(_SC_NPROCESSORS_ONLN);
    #endif
    if (sim->num_threads < 1) sim->num_threads = 1;
    if (sim->num_threads > 8) sim->num_threads = 8;  // cap at 8 threads
    
    // add default objects (stars/planets)
    Vector3D yellow = {1.0, 1.0, 0.0};
    Vector3D red = {1.0, 0.0, 0.0};
    Vector3D black = {0.0, 0.0, 0.0};
    
    Vector3D pos1 = {4e11, 0.0, 0.0};
    blackhole_sim3d_add_object(sim, pos1, 4e10, yellow, 1.98892e30);
    
    Vector3D pos2 = {0.0, 0.0, 4e11};
    blackhole_sim3d_add_object(sim, pos2, 4e10, red, 1.98892e30);
    
    // add black hole as object for visualization
    Vector3D pos3 = {0.0, 0.0, 0.0};
    blackhole_sim3d_add_object(sim, pos3, sim->blackhole.r_s, black, sim->blackhole.mass);
}

void blackhole_sim3d_add_object(BlackHoleSim3D *sim, Vector3D pos, double radius, 
                                 Vector3D color, double mass) {
    if (sim->num_objects >= sim->max_objects) {
        return;  // max objects reached
    }
    
    ObjectData *obj = &sim->objects[sim->num_objects];
    obj->position = pos;
    obj->radius = radius;
    obj->color = color;
    obj->mass = mass;
    obj->velocity.x = 0.0;
    obj->velocity.y = 0.0;
    obj->velocity.z = 0.0;
    
    sim->num_objects++;
}

// threaded function to update a subset of objects
void *update_objects_3d_threaded(void *args) {
    ThreadArgs3D *targs = (ThreadArgs3D *)args;
    BlackHoleSim3D *sim = targs->sim;
    
    // update gravitational interactions for assigned objects
    for (int i = targs->start_idx; i < targs->end_idx; i++) {
        ObjectData *obj1 = &sim->objects[i];
        
        // calculate forces from all other objects
        for (int j = 0; j < sim->num_objects; j++) {
            if (i == j) continue;  // skip self-interaction
            
            ObjectData *obj2 = &sim->objects[j];
            
            // calculate distance and direction
            double dx = obj2->position.x - obj1->position.x;
            double dy = obj2->position.y - obj1->position.y;
            double dz = obj2->position.z - obj1->position.z;
            double distance = sqrt(dx * dx + dy * dy + dz * dz);
            
            if (distance > 0) {
                // calculate gravitational force
                double Gforce = (sim->G * obj1->mass * obj2->mass) / (distance * distance);
                double acc1 = Gforce / obj1->mass;
                
                // direction vector
                Vector3D direction = {
                    dx / distance,
                    dy / distance,
                    dz / distance
                };
                
                // update velocity
                obj1->velocity.x += direction.x * acc1 * sim->timestep;
                obj1->velocity.y += direction.y * acc1 * sim->timestep;
                obj1->velocity.z += direction.z * acc1 * sim->timestep;
            }
        }
        
        // update position
        obj1->position.x += obj1->velocity.x * sim->timestep;
        obj1->position.y += obj1->velocity.y * sim->timestep;
        obj1->position.z += obj1->velocity.z * sim->timestep;
    }
    
    return NULL;
}

void blackhole_sim3d_update(BlackHoleSim3D *sim) {
    if (!sim->gravity_enabled) {
        return;
    }
    
    // use threading if we have multiple objects and threads
    if (sim->num_objects > 4 && sim->num_threads > 1) {
        pthread_t threads[sim->num_threads];
        ThreadArgs3D args[sim->num_threads];
        int chunk = sim->num_objects / sim->num_threads;
        
        // create all threads
        for (int t = 0; t < sim->num_threads; t++) {
            args[t].sim = sim;
            args[t].start_idx = t * chunk;
            args[t].end_idx = (t == sim->num_threads - 1) ? sim->num_objects : (t + 1) * chunk;
            pthread_create(&threads[t], NULL, update_objects_3d_threaded, &args[t]);
        }
        
        // wait for all threads to complete
        for (int t = 0; t < sim->num_threads; t++) {
            pthread_join(threads[t], NULL);
        }
    } else {
        // single-threaded fallback for small datasets
        ThreadArgs3D args = {sim, 0, sim->num_objects};
        update_objects_3d_threaded(&args);
    }
}

void blackhole_sim3d_cleanup(BlackHoleSim3D *sim) {
    if (sim->objects) {
        free(sim->objects);
        sim->objects = NULL;
    }
}

void spacetime_grid_init(SpacetimeGrid *grid, int grid_size) {
    grid->grid_size = grid_size;
    grid->num_vertices = (grid_size + 1) * (grid_size + 1);
    grid->vertices = malloc(sizeof(GridVertex) * grid->num_vertices);
    
    // calculate number of indices for line rendering
    // each grid cell has 4 edges (2 horizontal, 2 vertical)
    grid->num_indices = grid_size * grid_size * 4;
    grid->indices = malloc(sizeof(int) * grid->num_indices);
}

// threaded function to generate a subset of grid vertices
void *generate_grid_vertices_threaded(void *args) {
    GridThreadArgs *gargs = (GridThreadArgs *)args;
    SpacetimeGrid *grid = gargs->grid;
    BlackHoleSim3D *sim = gargs->sim;
    double spacing = gargs->spacing;
    int grid_size = gargs->grid_size;
    
    // generate vertices with spacetime curvature for assigned range
    for (int vertex_idx = gargs->start_vertex; vertex_idx < gargs->end_vertex; vertex_idx++) {
        int z = vertex_idx / (grid_size + 1);
        int x = vertex_idx % (grid_size + 1);
        
        double world_x = (x - grid_size / 2.0) * spacing;
        double world_z = (z - grid_size / 2.0) * spacing;
        double y = 0.0;
        
        // warp grid using schwarzschild geometry for each object
        for (int i = 0; i < sim->num_objects; i++) {
            ObjectData *obj = &sim->objects[i];
            double mass = obj->mass;
            
            double r_s = schwarzschild_radius(mass);
            double dx = world_x - obj->position.x;
            double dz = world_z - obj->position.z;
            double dist = sqrt(dx * dx + dz * dz);
            
            if (dist > r_s) {
                // outside event horizon
                double delta_y = spacetime_curvature_height(dist, r_s);
                y += delta_y - 3e10;  // offset for visualization
            } else {
                // inside or at event horizon: deep pit
                y += 2.0 * sqrt(r_s * r_s) - 3e10;
            }
        }
        
        grid->vertices[vertex_idx].position.x = world_x;
        grid->vertices[vertex_idx].position.y = y;
        grid->vertices[vertex_idx].position.z = world_z;
    }
    
    return NULL;
}

void spacetime_grid_generate(SpacetimeGrid *grid, BlackHoleSim3D *sim, double spacing) {
    int grid_size = grid->grid_size;
    int total_vertices = grid->num_vertices;
    
    // use threading if we have enough vertices and threads
    if (total_vertices > 100 && sim->num_threads > 1) {
        pthread_t threads[sim->num_threads];
        GridThreadArgs args[sim->num_threads];
        int chunk = total_vertices / sim->num_threads;
        
        // create all threads
        for (int t = 0; t < sim->num_threads; t++) {
            args[t].grid = grid;
            args[t].sim = sim;
            args[t].spacing = spacing;
            args[t].grid_size = grid_size;
            args[t].start_vertex = t * chunk;
            args[t].end_vertex = (t == sim->num_threads - 1) ? total_vertices : (t + 1) * chunk;
            pthread_create(&threads[t], NULL, generate_grid_vertices_threaded, &args[t]);
        }
        
        // wait for all threads to complete
        for (int t = 0; t < sim->num_threads; t++) {
            pthread_join(threads[t], NULL);
        }
    } else {
        // single-threaded fallback
        GridThreadArgs args = {grid, sim, spacing, 0, total_vertices, grid_size};
        generate_grid_vertices_threaded(&args);
    }
    
    // generate indices for line rendering
    int index_idx = 0;
    for (int z = 0; z < grid_size; z++) {
        for (int x = 0; x < grid_size; x++) {
            int i = z * (grid_size + 1) + x;
            
            // horizontal line
            grid->indices[index_idx++] = i;
            grid->indices[index_idx++] = i + 1;
            
            // vertical line
            grid->indices[index_idx++] = i;
            grid->indices[index_idx++] = i + grid_size + 1;
        }
    }
    grid->num_indices = index_idx;
}

void spacetime_grid_cleanup(SpacetimeGrid *grid) {
    if (grid->vertices) {
        free(grid->vertices);
        grid->vertices = NULL;
    }
    if (grid->indices) {
        free(grid->indices);
        grid->indices = NULL;
    }
}

