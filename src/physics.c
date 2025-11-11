#include "physics.h"
#include <math.h>

Vector2D vector_add(Vector2D a, Vector2D b) {
    Vector2D r = {a.x + b.x, a.y + b.y};
    return r;
}

Vector2D vector_scale(Vector2D v, double s) {
    Vector2D r = {v.x * s, v.y * s};
    return r;
}

double vector_distance(Vector2D a, Vector2D b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}

Vector2D compute_acceleration(Particle *p, BlackHole *bh, double G) {
    double dx = bh->position.x - p->position.x;
    double dy = bh->position.y - p->position.y;
    double r2 = dx * dx + dy * dy + 1e-6; // prevent div-by-zero
    double r = sqrt(r2);
    double accel = G * bh->mass / r2;

    Vector2D a = {accel * dx / r, accel * dy / r};
    return a;
}
