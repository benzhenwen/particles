#pragma once

// vector2 struct
typedef struct Vector {
    float x, y;
} Vector;

// the particle struct
typedef struct Particle {
    Vector pos;
    Vector vel;
} Particle;
