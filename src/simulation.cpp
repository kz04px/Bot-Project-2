#include <cassert>
#include <chrono>
#include <cstddef>
#include <thread>
#include "defs.hpp"

void *simulate_world(void *ptr) {
    assert(ptr);
    SimData *sim_data = (SimData *)ptr;
    assert(sim_data);

    while (!sim_data->quit) {
        if (sim_data->paused) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        const auto t0 = std::chrono::high_resolution_clock::now();
        world_simulate_frame(sim_data->world);
        const auto t1 = std::chrono::high_resolution_clock::now();
        const auto diff =
            std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);

        if (diff.count() < 1.0 / sim_data->fps_max) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        // sim_data->fps = 1.0 / (glfwGetTime() - t0);
    }

    return nullptr;
}
