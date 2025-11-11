#include "visualization.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 800
#define HEIGHT 800
#define SCALE 20.0f  // affects zoom level

static void draw_circle(float x, float y, float radius, int segments) {
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= segments; i++) {
        float theta = i * 2.0f * M_PI / segments;
        glVertex2f(x + radius * cosf(theta), y + radius * sinf(theta));
    }
    glEnd();
}

int run_visualization(Simulation *sim) {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Black Hole Simulation", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to create window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glewInit();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-SCALE, SCALE, -SCALE, SCALE, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    printf("Starting visualization...\n");

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw black hole
        glColor3f(0.0f, 0.0f, 0.0f);
        draw_circle(0.0f, 0.0f, 0.2f, 30);

        // Update simulation step-by-step
        pthread_t threads[sim->num_threads];
        for (int t = 0; t < sim->num_threads; t++) {
            ThreadArgs args = {sim, t * (sim->num_particles / sim->num_threads),
                               (t == sim->num_threads - 1)
                                   ? sim->num_particles
                                   : (t + 1) * (sim->num_particles / sim->num_threads)};
            pthread_create(&threads[t], NULL, update_particles, &args);
            pthread_join(threads[t], NULL);
        }

        // Draw particles
        glColor3f(0.2f, 0.7f, 1.0f);
        glBegin(GL_POINTS);
        for (int i = 0; i < sim->num_particles; i++) {
            float x = sim->particles[i].position.x;
            float y = sim->particles[i].position.y;
            glVertex2f(x, y);
        }
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
