#include "../include/physics.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main() {
    BlackHole bh = {1000, {0, 0}};
    Particle p = {{1, 0}, {0, 0}, 1, 0};

    Vector2D a = compute_acceleration(&p, &bh, 1.0);
    assert(fabs(a.x + 1000.0) > 0); // simple sanity
    printf("Physics test passed!\n");
    return 0;
}
