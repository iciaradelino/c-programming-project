#include "physics.h"
#include <math.h>

// 2d vector operations
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

// 3d vector operations
Vector3D vector3d_add(Vector3D a, Vector3D b) {
    Vector3D r = {a.x + b.x, a.y + b.y, a.z + b.z};
    return r;
}

Vector3D vector3d_sub(Vector3D a, Vector3D b) {
    Vector3D r = {a.x - b.x, a.y - b.y, a.z - b.z};
    return r;
}

Vector3D vector3d_scale(Vector3D v, double s) {
    Vector3D r = {v.x * s, v.y * s, v.z * s};
    return r;
}

double vector3d_length(Vector3D v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

double vector3d_distance(Vector3D a, Vector3D b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    double dz = a.z - b.z;
    return sqrt(dx * dx + dy * dy + dz * dz);
}

Vector3D vector3d_normalize(Vector3D v) {
    double len = vector3d_length(v);
    if (len < 1e-10) {
        return (Vector3D){0.0, 0.0, 0.0};
    }
    return vector3d_scale(v, 1.0 / len);
}

Vector3D vector3d_cross(Vector3D a, Vector3D b) {
    Vector3D r = {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
    return r;
}

double vector3d_dot(Vector3D a, Vector3D b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// black hole physics calculations
double schwarzschild_radius(double mass) {
    // r_s = 2GM / c^2
    return 2.0 * G_CONST * mass / (C_LIGHT * C_LIGHT);
}

int blackhole_intercept(BlackHole3D *bh, double px, double py, double pz) {
    // check if point is inside event horizon
    double dx = px - bh->position.x;
    double dy = py - bh->position.y;
    double dz = pz - bh->position.z;
    double dist2 = dx * dx + dy * dy + dz * dz;
    return dist2 < (bh->r_s * bh->r_s);
}

Vector3D compute_gravitational_acceleration_3d(Vector3D pos, BlackHole3D *bh, double G) {
    double dx = bh->position.x - pos.x;
    double dy = bh->position.y - pos.y;
    double dz = bh->position.z - pos.z;
    double r2 = dx * dx + dy * dy + dz * dz + 1e-6; // prevent div-by-zero
    double r = sqrt(r2);
    double accel = G * bh->mass / r2;

    Vector3D a = {
        accel * dx / r,
        accel * dy / r,
        accel * dz / r
    };
    return a;
}

double spacetime_curvature_height(double dist, double r_s) {
    // calculate spacetime curvature using schwarzschild geometry
    // this represents how much the grid should "dip" at a given distance
    if (dist > r_s) {
        // outside event horizon: use schwarzschild metric approximation
        return 2.0 * sqrt(r_s * (dist - r_s));
    } else {
        // inside or at event horizon: deep pit
        return 2.0 * sqrt(r_s * r_s);
    }
}
