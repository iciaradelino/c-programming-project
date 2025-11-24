#include "camera.h"
#include <math.h>

void camera_init(Camera *cam) {
    cam->target.x = 0.0;
    cam->target.y = 0.0;
    cam->target.z = 0.0;
    cam->radius = 6.34194e10;  // default distance
    cam->min_radius = 1e10;
    cam->max_radius = 1e12;
    cam->azimuth = 0.0;
    cam->elevation = M_PI / 2.0;
    cam->orbit_speed = 0.01;
    cam->pan_speed = 0.01;
    cam->zoom_speed = 25e9;
    cam->dragging = 0;
    cam->panning = 0;
    cam->moving = 0;
    cam->last_x = 0.0;
    cam->last_y = 0.0;
}

Vector3D camera_get_position(Camera *cam) {
    camera_clamp_elevation(cam);
    
    // orbit around (0,0,0) always
    Vector3D pos = {
        cam->radius * sin(cam->elevation) * cos(cam->azimuth),
        cam->radius * cos(cam->elevation),
        cam->radius * sin(cam->elevation) * sin(cam->azimuth)
    };
    return pos;
}

void camera_update(Camera *cam) {
    // always keep target at black hole center
    cam->target.x = 0.0;
    cam->target.y = 0.0;
    cam->target.z = 0.0;
    
    if (cam->dragging || cam->panning) {
        cam->moving = 1;
    } else {
        cam->moving = 0;
    }
}

void camera_clamp_elevation(Camera *cam) {
    if (cam->elevation < 0.01) cam->elevation = 0.01;
    if (cam->elevation > M_PI - 0.01) cam->elevation = M_PI - 0.01;
}

void camera_process_mouse_move(Camera *cam, double x, double y) {
    double dx = x - cam->last_x;
    double dy = y - cam->last_y;

    if (cam->dragging && !cam->panning) {
        // orbit: left mouse only
        cam->azimuth += dx * cam->orbit_speed;
        cam->elevation -= dy * cam->orbit_speed;
        camera_clamp_elevation(cam);
    }

    cam->last_x = x;
    cam->last_y = y;
    camera_update(cam);
}

void camera_process_mouse_button(Camera *cam, int button, int action, double x, double y) {
    // button: 0=left, 1=right, 2=middle
    // action: 1=press, 0=release
    if (button == 0 || button == 2) {  // left or middle mouse
        if (action == 1) {  // press
            cam->dragging = 1;
            cam->panning = 0;  // disable panning to keep camera centered
            cam->last_x = x;
            cam->last_y = y;
        } else if (action == 0) {  // release
            cam->dragging = 0;
            cam->panning = 0;
        }
    }
    camera_update(cam);
}

void camera_process_scroll(Camera *cam, double xoffset, double yoffset) {
    cam->radius -= yoffset * cam->zoom_speed;
    
    // clamp radius
    if (cam->radius < cam->min_radius) cam->radius = cam->min_radius;
    if (cam->radius > cam->max_radius) cam->radius = cam->max_radius;
    
    camera_update(cam);
}

