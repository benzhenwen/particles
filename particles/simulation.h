#pragma once

#include "particle.h"
#include "stdint.h"

// the project fps
#define FPS 60

#ifdef __cplusplus
extern "C" {
#endif

const float get_sim_width();
const float get_sim_height();
const float get_sim_particle_radius();

Particle * get_particles();
uint32_t get_particles_c();

void add_particle(Vector pos, Vector vel);

void tick_sim();

#ifdef __cplusplus
}
#endif