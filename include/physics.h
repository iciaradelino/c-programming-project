#ifndef PHYSICS_H
#define PHYSICS_H

typedef struct {
    double x, y;
} Vector2D;

typedef struct {
    Vector2D position;
    Vector2D velocity;
    double mass;
    int id;
} Particle;

typedef struct {
    double mass;
    Vector2D position;
} BlackHole;

Vector2D compute_acceleration(Particle *p, BlackHole *bh, double G);
Vector2D vector_add(Vector2D a, Vector2D b);
Vector2D vector_scale(Vector2D v, double s);
double vector_distance(Vector2D a, Vector2D b);

#endif
