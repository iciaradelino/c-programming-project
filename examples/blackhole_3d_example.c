#include "../include/blackhole_3d.h"
#include "../include/camera.h"
#include <stdio.h>
#include <math.h>

int main(void) {
    // initialize advanced black hole simulation
    BlackHoleSim3D sim;
    blackhole_sim3d_init(&sim);
    
    printf("black hole mass: %.2e kg\n", sim.blackhole.mass);
    printf("schwarzschild radius: %.2e m\n", sim.blackhole.r_s);
    printf("number of objects: %d\n", sim.num_objects);
    
    // initialize camera
    Camera cam;
    camera_init(&cam);
    
    Vector3D cam_pos = camera_get_position(&cam);
    printf("camera position: (%.2e, %.2e, %.2e)\n", 
           cam_pos.x, cam_pos.y, cam_pos.z);
    
    // initialize spacetime grid
    SpacetimeGrid grid;
    spacetime_grid_init(&grid, 25);
    spacetime_grid_generate(&grid, &sim, 1e10);
    
    printf("spacetime grid generated: %d vertices, %d indices\n",
           grid.num_vertices, grid.num_indices);
    
    // simulate a few steps
    sim.gravity_enabled = 1;
    printf("\nrunning simulation with gravity enabled...\n");
    
    for (int step = 0; step < 10; step++) {
        blackhole_sim3d_update(&sim);
        
        if (step % 2 == 0) {
            printf("step %d: object 0 position (%.2e, %.2e, %.2e)\n",
                   step,
                   sim.objects[0].position.x,
                   sim.objects[0].position.y,
                   sim.objects[0].position.z);
        }
    }
    
    // regenerate grid to show updated curvature
    spacetime_grid_generate(&grid, &sim, 1e10);
    printf("\nspacetime grid regenerated with updated positions\n");
    
    // cleanup
    spacetime_grid_cleanup(&grid);
    blackhole_sim3d_cleanup(&sim);
    
    printf("\nsimulation complete!\n");
    return 0;
}

