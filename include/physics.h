#ifndef PHYSICS_H
#define PHYSICS_H

#define M_PI 3.14159265358979323846
#define C_LIGHT 299792458.0  // speed of light in m/s
#define G_CONST 6.67430e-11  // gravitational constant

typedef struct {
    double x, y;
} Vector2D;

// 3d vector support for advanced black hole simulation
typedef struct {
    double x, y, z;
} Vector3D;

typedef struct {
    Vector2D position;
    Vector2D velocity;
    double mass;
    int id;
} Particle;

// 3d particle for advanced simulation
typedef struct {
    Vector3D position;
    Vector3D velocity;
    double mass;
    double radius;
    int id;
} Particle3D;

typedef struct {
    double mass;
    Vector2D position;
} BlackHole;

// advanced black hole with schwarzschild radius
typedef struct {
    Vector3D position;
    double mass;
    double radius;      // physical radius
    double r_s;         // schwarzschild radius (event horizon)
} BlackHole3D;

// 2d vector operations
Vector2D compute_acceleration(Particle *p, BlackHole *bh, double G);
Vector2D vector_add(Vector2D a, Vector2D b);
Vector2D vector_scale(Vector2D v, double s);
double vector_distance(Vector2D a, Vector2D b);

// 3d vector operations
Vector3D vector3d_add(Vector3D a, Vector3D b);
Vector3D vector3d_sub(Vector3D a, Vector3D b);
Vector3D vector3d_scale(Vector3D v, double s);
double vector3d_length(Vector3D v);
double vector3d_distance(Vector3D a, Vector3D b);
Vector3D vector3d_normalize(Vector3D v);
Vector3D vector3d_cross(Vector3D a, Vector3D b);
double vector3d_dot(Vector3D a, Vector3D b);

// black hole physics
double schwarzschild_radius(double mass);
int blackhole_intercept(BlackHole3D *bh, double px, double py, double pz);
Vector3D compute_gravitational_acceleration_3d(Vector3D pos, BlackHole3D *bh, double G);
double spacetime_curvature_height(double dist, double r_s);

#endif
