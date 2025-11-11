#include "utils.h"
#include <stdlib.h>

Particle random_particle(int id) {
    Particle p;
    p.id = id;
    p.mass = 1.0;
    p.position.x = ((rand() % 200) - 100) / 10.0;
    p.position.y = ((rand() % 200) - 100) / 10.0;
    p.velocity.x = ((rand() % 100) - 50) / 100.0;
    p.velocity.y = ((rand() % 100) - 50) / 100.0;
    return p;
}
