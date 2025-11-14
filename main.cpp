#include <logger/logger.hpp>
#include "raylib.h"

#include <chrono>
#include <math.h>

#include "particles/simulation.h"


#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800


int main() {

    // basic init stuff
    static constexpr Logger logger = Logger("MAIN");
    logger << "meow";

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Particles");
    SetTargetFPS(FPS);

    // init the simulation
    static const float particle_radius = (get_sim_particle_radius() / get_sim_width()) * SCREEN_WIDTH;

    // particle tracking
    uint32_t particle_c = 0;
    uint32_t particle_spawn_spacing = 0;


    static std::chrono::time_point last_frame_time = std::chrono::steady_clock::now();
    while(!WindowShouldClose()) {
        const auto frame_time = std::chrono::steady_clock::now();

        // new frame
        float frame_delta = std::chrono::duration_cast<std::chrono::nanoseconds>(frame_time - last_frame_time).count() / 1000000000.0f;
        last_frame_time = frame_time;

        // drawing
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // show fps
        static char fps_print_buff[16];
        sprintf(fps_print_buff, "fps: %.2f", 1 / frame_delta);
        DrawText(fps_print_buff, 5, 5, 20, BLACK);

        // add particle
        if (particle_c < 1000) {
            if (particle_spawn_spacing == 0) {
                particle_c += 2;
                add_particle({0.1f, 0.35}, {0.01, 0.03});
                add_particle({0.8f, 0.38}, {-0.01, 0.03});
            }
            particle_spawn_spacing++;
            if (particle_spawn_spacing >= 2) particle_spawn_spacing = 0;
        }

        // sim
        tick_sim();

        // draw the sim
        Particle * particles = get_particles();
        uint32_t particles_c = get_particles_c();

        for (uint32_t i = 0; i < particles_c; i++) {
            DrawCircle((particles[i].pos.x / get_sim_width()) * SCREEN_WIDTH, ((get_sim_height() - particles[i].pos.y) / get_sim_height()) * SCREEN_HEIGHT, particle_radius, RED);
        }

        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}