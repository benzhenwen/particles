#include "simulation.h"

#include "particle.h"

#include "stdint.h"
#include "math.h"

#define MAX_PARTICLE_ARRAY_COUNT 1000

#define PARTICLE_RADIUS 0.01f
#define SIM_WIDTH 1
#define SIM_HEIGHT 1

#define SIM_SCALE 200 // x meter per 1 unit

#define GRAV_ACCEL (9.80665f / (FPS * SIM_SCALE))

#define GROUND_ELASTICITY 0.5f 
#define GROUND_FRICTION 0.98f // friction applied every frame an object touches the ground. linear relation
#define PARTICLE_ELASTICITY 0.5f

// array containing ALL particles
static Particle particles[MAX_PARTICLE_ARRAY_COUNT];
static uint32_t particles_c = 0; // max of 4294967295

// metadata info
const float get_sim_width() { return SIM_WIDTH; }
const float get_sim_height() { return SIM_HEIGHT; }
const float get_sim_particle_radius() { return PARTICLE_RADIUS; }

// fetch info
Particle * get_particles() { return particles; }
uint32_t get_particles_c() { return particles_c; }


// add a particle to the simulation
void add_particle(Vector pos, Vector vel) {
    if (particles_c >= MAX_PARTICLE_ARRAY_COUNT) {
        printf("warning: exceded max particle count, ignoring call to add_particle\n");
        return;
    }

    particles[particles_c].pos = pos;
    particles[particles_c].vel = vel;
    particles_c++;
}


void tick_sim() {
    
    // apply gravity and passive resistance (linear model)
    for (uint32_t i = 0; i < particles_c; i++) {
        particles[i].vel.y -= GRAV_ACCEL;
    }

    // apply movement
    for (uint32_t i = 0; i < particles_c; i++) {
        particles[i].pos.x += particles[i].vel.x;
        particles[i].pos.y += particles[i].vel.y;
    }
    
    for (int c = 0; c < 20; c++) {
    
        // bounds colision
        for (uint32_t i = 0; i < particles_c; i++) {
            // floor
            if (particles[i].pos.y < 0 + PARTICLE_RADIUS) {
                particles[i].pos.y = PARTICLE_RADIUS;
                particles[i].vel.y *= -GROUND_ELASTICITY;
                
                // ground friction
                particles[i].vel.x *= GROUND_FRICTION;
            }
            // ceiling
            else if (particles[i].pos.y > SIM_HEIGHT - PARTICLE_RADIUS) {
                particles[i].pos.y = SIM_HEIGHT - PARTICLE_RADIUS;
                particles[i].vel.y *= -GROUND_ELASTICITY;
            }

            // left wall
            if (particles[i].pos.x < 0 + PARTICLE_RADIUS) {
                particles[i].pos.x = PARTICLE_RADIUS;
                particles[i].vel.x *= -GROUND_ELASTICITY;
            }
            // right wall
            else if (particles[i].pos.x > SIM_WIDTH - PARTICLE_RADIUS) {
                particles[i].pos.x = SIM_WIDTH - PARTICLE_RADIUS;
                particles[i].vel.x *= -GROUND_ELASTICITY;
            }
        }

        // others colision
        for (uint32_t i = 0; i < particles_c; i++) {
            for (uint32_t j = i+1; j < particles_c; j++) {
                const Particle curr = particles[i];
                const Particle other = particles[j];

                // square bounds check first
                if (fabsf(other.pos.x - curr.pos.x) < 2*PARTICLE_RADIUS && fabsf(other.pos.y - curr.pos.y) < 2*PARTICLE_RADIUS) {
                    const float dist_between = sqrtf(powf(other.pos.x - curr.pos.x, 2) + powf(other.pos.y - curr.pos.y, 2));
                    if (dist_between < 2*PARTICLE_RADIUS) {
                        const float angle = atan2f(other.pos.y - curr.pos.y, other.pos.x - curr.pos.x);
                        const float x_push_factor = cosf(angle);
                        const float y_push_factor = sinf(angle);
                        
                        const float push_dist = (2*PARTICLE_RADIUS - dist_between) / 2;

                        // collision seperation
                        particles[i].pos.x -= push_dist * x_push_factor;
                        particles[i].pos.y -= push_dist * y_push_factor;

                        particles[j].pos.x += push_dist * x_push_factor;
                        particles[j].pos.y += push_dist * y_push_factor;

                        // collision velocities
                        const float nx = x_push_factor;
                        const float ny = y_push_factor;
                        const float tx = -ny;
                        const float ty = nx;

                        const float v1n = curr.vel.x * nx + curr.vel.y * ny;
                        const float v1t = curr.vel.x * tx + curr.vel.y * ty;
                        
                        const float v2n = other.vel.x * nx + other.vel.y * ny;
                        const float v2t = other.vel.x * tx + other.vel.y * ty;

                        if (v1n - v2n > 0.0f) {
                            const float v1n_prime = v2n;
                            const float v2n_prime = v1n;

                            const float v1t_prime = v1t;
                            const float v2t_prime = v2t;

                            particles[i].vel.x = v1n_prime * nx + v1t_prime * tx;
                            particles[i].vel.y = v1n_prime * ny + v1t_prime * ty;
                            
                            particles[j].vel.x = v2n_prime * nx + v2t_prime * tx;
                            particles[j].vel.y = v2n_prime * ny + v2t_prime * ty;
                        }

                    }
                }
            }
        }
    }

}