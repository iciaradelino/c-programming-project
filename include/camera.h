#ifndef CAMERA_H
#define CAMERA_H

#include "physics.h"

// camera system for orbiting around black hole
typedef struct {
    Vector3D target;        // always look at black hole center
    double radius;          // distance from target
    double min_radius;      // minimum zoom distance
    double max_radius;      // maximum zoom distance
    double azimuth;         // horizontal angle (0 to 2π)
    double elevation;       // vertical angle (0 to π)
    double orbit_speed;     // mouse sensitivity for orbiting
    double pan_speed;       // mouse sensitivity for panning
    double zoom_speed;      // scroll sensitivity
    int dragging;           // is mouse button held down
    int panning;            // is panning mode active
    int moving;             // optimization flag
    double last_x;          // last mouse x position
    double last_y;          // last mouse y position
} Camera;

// camera functions
void camera_init(Camera *cam);
Vector3D camera_get_position(Camera *cam);
void camera_update(Camera *cam);
void camera_process_mouse_move(Camera *cam, double x, double y);
void camera_process_mouse_button(Camera *cam, int button, int action, double x, double y);
void camera_process_scroll(Camera *cam, double xoffset, double yoffset);
void camera_clamp_elevation(Camera *cam);

#endif

